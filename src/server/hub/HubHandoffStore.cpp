/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifdef _WIN32
#include <winsock2.h>
#endif
#include <mysql.h>

#include "HubHandoffStore.h"
#include "Cluster/HandoffClient.h"
#include <array>
#include <sstream>
#include <cstdlib>
#include <vector>

using namespace Skyfire::Cluster::Handoff;

struct HubHandoffStore::State
{
    MYSQL* Connection = nullptr;
    std::array<std::string, 5> Info;
    Counters Counts;

    ~State() { Disconnect(); }
    void Disconnect() { if (Connection) mysql_close(Connection); Connection = nullptr; }
    bool Connect()
    {
        if (Connection) return true;
        MYSQL* connection = mysql_init(nullptr);
        if (!connection) return false;
        unsigned timeout = 2;
        mysql_options(connection, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);
        mysql_options(connection, MYSQL_OPT_READ_TIMEOUT, &timeout);
        mysql_options(connection, MYSQL_OPT_WRITE_TIMEOUT, &timeout);
        mysql_options(connection, MYSQL_SET_CHARSET_NAME, "utf8mb4");
        std::string host = Info[0];
        char const* socket = nullptr;
        unsigned port = unsigned(std::strtoul(Info[1].c_str(), nullptr, 10));
#ifdef _WIN32
        if (host == ".")
        {
            unsigned protocol = MYSQL_PROTOCOL_PIPE;
            mysql_options(connection, MYSQL_OPT_PROTOCOL, &protocol);
        }
#else
        if (host == ".") { host = "localhost"; socket = Info[1].c_str(); port = 0; }
#endif
        // Never enable automatic reconnection: losing a transaction must fail closed.
        if (!mysql_real_connect(connection, host.c_str(), Info[2].c_str(), Info[3].c_str(),
            Info[4].c_str(), port, socket, 0))
        { mysql_close(connection); return false; }
        Connection = connection;
        return Exec("SET SESSION innodb_lock_wait_timeout=2");
    }
    bool Exec(std::string const& sql)
    {
        if (!Connection || mysql_real_query(Connection, sql.data(), static_cast<unsigned long>(sql.size())))
        { Disconnect(); return false; }
        return true;
    }
    bool Rows(std::string const& sql, std::vector<std::vector<std::string>>& rows)
    {
        rows.clear();
        if (!Exec(sql)) return false;
        std::unique_ptr<MYSQL_RES, decltype(&mysql_free_result)> result(mysql_store_result(Connection), mysql_free_result);
        if (!result) { Disconnect(); return false; }
        while (MYSQL_ROW row = mysql_fetch_row(result.get()))
        {
            auto lengths = mysql_fetch_lengths(result.get());
            std::vector<std::string> fields;
            for (unsigned i = 0; i < mysql_num_fields(result.get()); ++i)
                fields.emplace_back(row[i] ? std::string(row[i], lengths[i]) : std::string());
            rows.push_back(std::move(fields));
        }
        return true;
    }
    std::string Quote(std::string const& value)
    {
        // Hex literals do not depend on SQL mode or character escaping rules.
        static char const* hex = "0123456789abcdef";
        std::string result = "X'";
        for (unsigned char c : value) { result += hex[c >> 4]; result += hex[c & 15]; }
        return result + "'";
    }
    bool Begin()
    {
        if (!Connect() || !Exec("START TRANSACTION")) return false;
        std::vector<std::vector<std::string>> rows;
        if (!Rows("SELECT id FROM hub_handoff_lock WHERE id=1 FOR UPDATE", rows) || rows.size() != 1)
        { Disconnect(); return false; }
        return true;
    }
    Result Finish(Result result)
    {
        // A failed/ambiguous commit never reports success or retries a mutation.
        if (!Exec(result == Result::Ok ? "COMMIT" : "ROLLBACK")) result = Result::Unavailable;
        if (result != Result::Ok)
        { ++Counts.Rejected; if (result == Result::Replay) ++Counts.Replayed; }
        return result;
    }
    Result Reject(Result result) { ++Counts.Rejected; return result; }
    std::string Slot(Binding const& binding)
    { return "account=" + std::to_string(binding.Account) + " AND purpose=" + std::to_string(unsigned(binding.Use)); }
    Result Read(Binding const& binding, std::string const& token, bool latest, std::string& selected)
    {
        std::vector<std::vector<std::string>> rows;
        if (!Rows("SELECT token,account,purpose,realm,address,destination,evidence,state,expires_at<=UTC_TIMESTAMP(6) "
            "FROM hub_handoff_tokens WHERE " + (latest ? Slot(binding) + " AND is_latest=1" : "token=" + Quote(token)) +
            " FOR UPDATE", rows)) return Result::Unavailable;
        if (rows.empty()) return Result::Missing;
        auto const& row = rows.front();
        if (row[1] != std::to_string(binding.Account) || row[2] != std::to_string(unsigned(binding.Use)) ||
            row[3] != std::to_string(binding.Realm) || row[4] != binding.Address ||
            row[5] != binding.Destination || row[6] != binding.Evidence) return Result::Denied;
        if (row[8] != "0") return Result::Expired;
        if (row[7] != "0") return Result::Replay;
        selected = row[0];
        return Result::Ok;
    }
    Result Change(std::string const& token, Binding const& binding, unsigned state)
    {
        if (!Valid(binding) || !IsToken(token)) return Reject(Result::Invalid);
        if (!Begin()) return Reject(Result::Unavailable);
        std::string selected;
        auto result = Read(binding, token, false, selected);
        if (result == Result::Ok && !Exec("UPDATE hub_handoff_tokens SET state=" + std::to_string(state) +
            " WHERE token=" + Quote(selected))) result = Result::Unavailable;
        result = Finish(result);
        if (result == Result::Ok) { if (state == 1) ++Counts.Consumed; else ++Counts.Revoked; }
        return result;
    }
};

HubHandoffStore::HubHandoffStore() : _state(new State) { }
HubHandoffStore::~HubHandoffStore() = default;
bool HubHandoffStore::Open(std::string const& connectionString)
{
    Close();
    std::istringstream input(connectionString);
    for (auto& field : _state->Info)
        if (!std::getline(input, field, ';')) return false;
    if (input.peek() != std::char_traits<char>::eof()) return false;
    if (_state->Info[4].empty() || !_state->Begin()) return false;
    std::vector<std::vector<std::string>> rows;
    // Refuse an absent/incompatible schema before opening the cluster listener.
    bool engines = _state->Rows("SELECT COUNT(*) FROM information_schema.tables WHERE table_schema=DATABASE() "
        "AND table_name IN ('hub_handoff_lock','hub_handoff_tokens') AND engine='InnoDB'", rows) &&
        rows.size() == 1 && rows[0][0] == "2";
    bool const ready = engines && _state->Rows("SELECT token,account,purpose,realm,address,destination,evidence,state,is_latest,expires_at "
        "FROM hub_handoff_tokens LIMIT 0", rows);
    return _state->Finish(ready ? Result::Ok : Result::Unavailable) == Result::Ok;
}
void HubHandoffStore::Close() { _state->Disconnect(); }
Counters HubHandoffStore::Status() const { return _state->Counts; }
Result HubHandoffStore::Issue(Binding const& binding, std::uint32_t ttl, std::uint64_t, std::string& token)
{
    token.clear();
    if (!Valid(binding) || !ttl || ttl > 900) return _state->Reject(Result::Invalid);
    auto candidate = Skyfire::Cluster::Handoff::RandomToken();
    if (!IsToken(candidate) || !_state->Begin()) return _state->Reject(Result::Unavailable);
    std::vector<std::vector<std::string>> rows;
    if (!_state->Exec("DELETE FROM hub_handoff_tokens WHERE expires_at<=UTC_TIMESTAMP(6)") ||
        !_state->Rows("SELECT COUNT(*) FROM hub_handoff_tokens", rows) || rows.size() != 1)
        return _state->Finish(Result::Unavailable);
    if (std::strtoull(rows[0][0].c_str(), nullptr, 10) >= 65536) return _state->Finish(Result::Capacity);
    if (!_state->Exec("UPDATE hub_handoff_tokens SET state=IF(state=0,2,state),is_latest=0 WHERE " +
        _state->Slot(binding) + " AND is_latest=1")) return _state->Finish(Result::Unavailable);
    auto sql = "INSERT INTO hub_handoff_tokens(token,account,purpose,realm,address,destination,evidence,expires_at) VALUES (" +
        _state->Quote(candidate) + "," + std::to_string(binding.Account) + "," + std::to_string(unsigned(binding.Use)) + "," +
        std::to_string(binding.Realm) + "," + _state->Quote(binding.Address) + "," + _state->Quote(binding.Destination) + "," +
        _state->Quote(binding.Evidence) + ",DATE_ADD(UTC_TIMESTAMP(6),INTERVAL " + std::to_string(ttl) + " SECOND))";
    if (!_state->Exec(sql)) return _state->Finish(Result::Unavailable);
    auto result = _state->Finish(Result::Ok);
    if (result == Result::Ok) { token = std::move(candidate); ++_state->Counts.Issued; }
    return result;
}
Result HubHandoffStore::Inspect(Binding const& binding, std::uint64_t, std::string& token)
{
    token.clear();
    if (!Valid(binding)) return _state->Reject(Result::Invalid);
    if (!_state->Begin()) return _state->Reject(Result::Unavailable);
    std::string selected;
    auto result = _state->Finish(_state->Read(binding, "", true, selected));
    if (result == Result::Ok) token = std::move(selected);
    return result;
}
Result HubHandoffStore::Consume(std::string const& token, Binding const& binding, std::uint64_t)
{ return _state->Change(token, binding, 1); }
Result HubHandoffStore::Revoke(std::string const& token, Binding const& binding, std::uint64_t)
{ return _state->Change(token, binding, 2); }
void HubHandoffStore::Cleanup(std::uint64_t)
{
    if (!_state->Begin()) return;
    std::vector<std::vector<std::string>> rows;
    if (!_state->Rows("SELECT COUNT(*) FROM hub_handoff_tokens WHERE state=0 AND expires_at<=UTC_TIMESTAMP(6)", rows) ||
        rows.size() != 1) { _state->Finish(Result::Unavailable); return; }
    auto expired = std::strtoull(rows[0][0].c_str(), nullptr, 10);
    if (!_state->Exec("DELETE FROM hub_handoff_tokens WHERE expires_at<=UTC_TIMESTAMP(6)"))
    { _state->Finish(Result::Unavailable); return; }
    if (_state->Finish(Result::Ok) == Result::Ok) _state->Counts.Expired += expired;
}
