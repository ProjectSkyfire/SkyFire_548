/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "AccountAdministration.h"
#include "AccountIdentity.h"
#include "LoginIdentity.h"
#include "Cryptography/Authentication/AuthnetSRP6.h"
#include "Database/DatabaseEnv.h"
#include <algorithm>
#include <boost/asio/ip/address_v4.hpp>
#include <ctime>
#include <mutex>
#include <sstream>
#include <stdexcept>

namespace Skyfire::Auth
{
namespace
{
    bool Enabled = false; // Set before the hub starts accepting HTTP requests.
    std::mutex MutationMutex;
    using Form = AccountAdministration::Form;
    std::string Value(Form const& form, std::string const& key)
    {
        auto const it = form.find(key);
        return it == form.end() ? "" : it->second;
    }
    uint32 Number(std::string const& text, uint32 maximum, bool allowZero = true)
    {
        if (text.empty() || text.size() > 10 ||
            !std::all_of(text.begin(), text.end(), [](char c) { return c >= '0' && c <= '9'; }))
            throw std::invalid_argument("A required numeric field is invalid.");
        uint64 value = std::stoull(text);
        if (value > maximum || (!allowZero && !value))
            throw std::invalid_argument("A numeric field is outside its allowed range.");
        return uint32(value);
    }
    std::string Quote(std::string const& text)
    {
        std::string out = "\"";
        char const* hex = "0123456789abcdef";
        for (unsigned char c : text)
        {
            if (c == '"' || c == '\\') { out += '\\'; out += char(c); }
            else if (c < 32) { out += "\\u00"; out += hex[c >> 4]; out += hex[c & 15]; }
            else out += char(c);
        }
        return out + '"';
    }
    AccountAdminReply Error(int status, std::string const& error) { return { status, "{\"error\":" + Quote(error) + "}" }; }
    void Text(std::string const& text, size_t limit, bool required = true)
    {
        if ((required && text.empty()) || text.size() > limit ||
            std::any_of(text.begin(), text.end(), [](unsigned char c) { return c < 32 || c == 127; }))
            throw std::invalid_argument("A text field is empty, too long, or contains control characters.");
    }
    PreparedStatement* Stmt(uint32 id) { return LoginDatabase.GetPreparedStatement(id); }
    PreparedQueryResult Detail(uint32 id)
    {
        auto* stmt = Stmt(LOGIN_HUB_ACCOUNT_DETAIL); stmt->setUInt32(0, id);
        return LoginDatabase.Query(stmt);
    }
    void AppendId(SQLTransaction& transaction, uint32 statement, uint32 id)
    {
        auto* stmt = Stmt(statement); stmt->setUInt32(0, id); transaction->Append(stmt);
    }
    void Event(SQLTransaction& transaction, std::string const& action, uint32 id,
        std::string const& ip, std::string const& actor)
    {
        auto* stmt = Stmt(LOGIN_HUB_INS_EVENT);
        stmt->setString(0, action); stmt->setUInt32(1, id); stmt->setString(2, ip); stmt->setString(3, actor);
        transaction->Append(stmt);
    }
    bool Commit(SQLTransaction& transaction)
    {
        // Unlike the void pool helper, retain the actual commit outcome for HTTP clients.
        return LoginDatabase.DirectCommitTransactionWithResult(transaction);
    }
    int32 Realm(Form const& form)
    {
        auto const text = Value(form, "realm");
        if (text == "-1") return -1;
        uint32 id = Number(text, 2147483647, false);
        auto* stmt = Stmt(LOGIN_SEL_REALMNAME_BY_ID); stmt->setUInt32(0, id);
        if (!LoginDatabase.Query(stmt)) throw std::invalid_argument("Realm does not exist.");
        return int32(id);
    }
    void Identity(SQLTransaction& transaction, std::string const& username, std::string identity,
        std::string const& password, uint8 type)
    {
        if (!NormalizeCredential(identity, MAX_EMAIL_STR))
            throw std::invalid_argument("Invalid login identity.");
        auto const normalized = NormalizeLoginIdentity(identity);
        auto [salt, verifier] = SkyFire::Crypto::SRP6::MakeRegistrationData(identity, password);
        auto* stmt = Stmt(LOGIN_HUB_INS_IDENTITY);
        stmt->setUInt8(0, type); stmt->setString(1, identity); stmt->setString(2, normalized.Canonical);
        stmt->setBinary(3, salt); stmt->setBinary(4, verifier);
        if (type == 1)
        {
            auto authnet = SkyFire::Crypto::AuthnetSRP6::MakeRegistrationData(identity, password);
            stmt->setBinary(5, authnet.salt); stmt->setBinary(6, authnet.verifier);
        }
        else { stmt->setNull(5); stmt->setNull(6); }
        stmt->setString(7, username); transaction->Append(stmt);
    }
    AccountAdminReply Read(std::string const& action, Form const& form)
    {
        std::ostringstream json;
        if (action == "search" || action == "permissions")
        {
            std::string const query = Value(form, "query"); Text(query, 64, false);
            uint32 after = Value(form, "after").empty() ? 0 : Number(Value(form, "after"), UINT32_MAX);
            uint32 numericQuery = 0;
            if (!query.empty() && query.size() <= 10 && std::all_of(query.begin(), query.end(), [](char c) { return c >= '0' && c <= '9'; }))
                numericQuery = Number(query, UINT32_MAX);
            auto* stmt = Stmt(action == "search" ? LOGIN_HUB_ACCOUNT_SEARCH : LOGIN_HUB_PERMISSION_SEARCH);
            stmt->setUInt32(0, after); stmt->setString(1, query); stmt->setString(2, query);
            if (action == "search") { stmt->setString(3, query); stmt->setUInt32(4, numericQuery); }
            else stmt->setUInt32(3, numericQuery);
            auto rows = LoginDatabase.Query(stmt);
            json << "{\"items\":["; size_t count = 0; uint32 next = 0;
            if (rows) do
            {
                if (count == 50) break;
                Field* f = rows->Fetch(); if (count++) json << ','; next = f[0].GetUInt32();
                json << "{\"id\":" << next << ",\"name\":" << Quote(f[1].GetString());
                if (action == "search") json << ",\"email\":" << Quote(f[2].GetString()) << ",\"ip\":" << Quote(f[3].GetString())
                    << ",\"online\":" << (f[4].GetBool() ? "true" : "false") << ",\"banned\":" << (f[5].GetBool() ? "true" : "false");
                json << '}';
            } while (rows->NextRow());
            json << "],\"next\":" << (rows && rows->GetRowCount() > 50 ? next : 0) << '}';
        }
        else if (action == "detail")
        {
            uint32 const id = Number(Value(form, "id"), UINT32_MAX, false);
            auto result = Detail(id); if (!result) return Error(404, "Account not found.");
            Field* f = result->Fetch();
            json << "{\"id\":" << id << ",\"username\":" << Quote(f[1].GetString()) << ",\"email\":" << Quote(f[2].GetString())
                << ",\"expansion\":" << unsigned(f[4].GetUInt8()) << ",\"ip\":" << Quote(f[5].GetString())
                << ",\"online\":" << (f[6].GetBool() ? "true" : "false") << ",\"muteUntil\":" << f[7].GetInt64()
                << ",\"muteReason\":" << Quote(f[8].GetString()) << ",\"muteBy\":" << Quote(f[9].GetString())
                << ",\"banned\":" << (f[10].GetBool() ? "true" : "false") << ",\"gm\":[";
            auto* stmt = Stmt(LOGIN_HUB_ACCOUNT_ACCESS); stmt->setUInt32(0, id); auto rows = LoginDatabase.Query(stmt); bool comma = false;
            if (rows) do { auto* r = rows->Fetch(); if (comma) json << ','; comma = true;
                json << "{\"realm\":" << r[0].GetInt32() << ",\"level\":" << unsigned(r[1].GetUInt8()) << '}'; } while (rows->NextRow());
            json << "],\"rbac\":["; comma = false;
            stmt = Stmt(LOGIN_HUB_ACCOUNT_RBAC); stmt->setUInt32(0, id); rows = LoginDatabase.Query(stmt);
            if (rows) do { auto* r = rows->Fetch(); if (comma) json << ','; comma = true;
                json << "{\"realm\":" << r[0].GetInt32() << ",\"permission\":" << r[1].GetUInt32()
                    << ",\"granted\":" << (r[2].GetBool() ? "true" : "false") << ",\"name\":" << Quote(r[3].GetString()) << '}'; } while (rows->NextRow());
            json << "],\"bans\":["; comma = false;
            stmt = Stmt(LOGIN_HUB_ACCOUNT_BANS); stmt->setUInt32(0, id); rows = LoginDatabase.Query(stmt);
            if (rows) do { auto* r = rows->Fetch(); if (comma) json << ','; comma = true;
                json << "{\"start\":" << r[0].GetUInt32() << ",\"end\":" << r[1].GetUInt32()
                    << ",\"by\":" << Quote(r[2].GetString()) << ",\"reason\":" << Quote(r[3].GetString()) << '}'; } while (rows->NextRow());
            json << "]}";
        }
        else if (action == "realms")
        {
            json << "{\"items\":[{\"id\":-1,\"name\":\"All realms\"}";
            auto rows = LoginDatabase.Query(Stmt(LOGIN_HUB_REALMS));
            if (rows) do { auto* f = rows->Fetch(); json << ",{\"id\":" << f[0].GetUInt32() << ",\"name\":" << Quote(f[1].GetString()) << '}'; } while (rows->NextRow());
            json << "]}";
        }
        else if (action == "ip-list")
        {
            std::string query = Value(form, "query"), after = Value(form, "after"); Text(query, 15, false); Text(after, 15, false);
            auto* stmt = Stmt(LOGIN_HUB_IP_LIST); stmt->setString(0, after); stmt->setString(1, query); stmt->setString(2, query);
            auto rows = LoginDatabase.Query(stmt); size_t count = 0; std::string next;
            json << "{\"items\":[";
            if (rows) do { if (count == 50) break; auto* f = rows->Fetch(); if (count++) json << ','; next = f[0].GetString();
                json << "{\"ip\":" << Quote(next) << ",\"start\":" << f[1].GetUInt32() << ",\"end\":" << f[2].GetUInt32()
                    << ",\"by\":" << Quote(f[3].GetString()) << ",\"reason\":" << Quote(f[4].GetString()) << '}'; } while (rows->NextRow());
            json << "],\"next\":" << Quote(rows && rows->GetRowCount() > 50 ? next : "") << '}';
        }
        else return Error(404, "Unknown account action.");
        return { 200, json.str() };
    }
}

void AccountAdministration::SetEnabled(bool enabled) { Enabled = enabled; }
bool AccountAdministration::IsEnabled() { return Enabled; }
bool AccountAdministration::IsReadAction(std::string const& action)
{
    return action == "search" || action == "detail" || action == "permissions" || action == "realms" || action == "ip-list";
}

AccountAdminReply AccountAdministration::Handle(std::string const& action, Form const& form, std::string const& actor)
{
    try
    {
        if (IsReadAction(action)) return Read(action, form);
        Text(actor, 64);
        std::lock_guard<std::mutex> lock(MutationMutex);
        auto transaction = LoginDatabase.BeginTransaction();
        uint32 id = 0; std::string ip; std::string eventAction = action;
        bool const ipAction = action == "ip-ban" || action == "ip-unban";
        PreparedQueryResult existing;
        if (action != "create" && !ipAction)
        {
            id = Number(Value(form, "id"), UINT32_MAX, false); existing = Detail(id);
            if (!existing) return Error(404, "Account not found.");
        }
        if (action == "create" || action == "update")
        {
            std::string username = Value(form, "username"), email = Value(form, "email"), password = Value(form, "password");
            Text(username, MAX_ACCOUNT_STR); Text(email, MAX_EMAIL_STR); Text(password, MAX_ACCOUNT_STR, false);
            if (!std::all_of(username.begin(), username.end(), [](unsigned char c)
                { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '-' || c == '.'; }))
                return Error(400, "Username must use 1-16 letters, digits, dots, underscores or hyphens.");
            NormalizeCredential(username, MAX_ACCOUNT_STR);
            auto const login = NormalizeLoginIdentity(email);
            if (!login.Valid || login.Kind != LoginIdentityKind::Email ||
                std::any_of(email.begin(), email.end(), [](unsigned char c) { return c <= 32 || c >= 127; }))
                return Error(400, "Enter a valid email address (up to 64 ASCII characters).");
            uint8 expansion = uint8(Number(Value(form, "expansion"), 4));
            uint32 const other = GetId(username);
            if ((other && other != id) || IsEmailLoginAssignedToAnotherAccount(id, login.Canonical))
                return Error(409, "Username or email is already assigned to another account.");
            bool const changedIdentity = !existing || username != (*existing)[1].GetString() || email != (*existing)[2].GetString();
            if (changedIdentity && password.empty()) return Error(400, "A new password is required when creating an account or changing its username/email.");
            if (!password.empty())
            {
                if (!NormalizeCredential(password, MAX_ACCOUNT_STR)) return Error(400, "Invalid password.");
                auto [salt, verifier] = SkyFire::Crypto::SRP6::MakeRegistrationData(username, password);
                auto* stmt = Stmt(action == "create" ? LOGIN_INS_ACCOUNT : LOGIN_HUB_UPD_CREDENTIALS);
                stmt->setString(0, username);
                if (action == "create")
                {
                    stmt->setBinary(1, salt); stmt->setBinary(2, verifier); stmt->setString(3, email); stmt->setString(4, email);
                }
                else
                {
                    stmt->setString(1, email); stmt->setString(2, email); stmt->setBinary(3, salt); stmt->setBinary(4, verifier);
                    stmt->setUInt8(5, expansion); stmt->setUInt32(6, id);
                    AppendId(transaction, LOGIN_DEL_ACCOUNT_LOGIN_IDENTITIES, id);
                    AppendId(transaction, LOGIN_HUB_DEL_AUTHNET_SESSIONS, id);
                    eventAction = "credentials";
                }
                transaction->Append(stmt);
                Identity(transaction, username, username, password, 0);
                Identity(transaction, username, email, password, 1);
            }
            if (action == "create")
            {
                // The account insert and unique identity inserts commit atomically.
                auto* stmt = Stmt(LOGIN_HUB_INS_CREATE_EVENT); stmt->setString(0, actor); stmt->setString(1, username); transaction->Append(stmt);
                stmt = Stmt(LOGIN_HUB_CREATE_EXPANSION); stmt->setUInt8(0, expansion); stmt->setString(1, username); transaction->Append(stmt);
                transaction->Append(Stmt(LOGIN_INS_REALM_CHARACTERS_INIT));
            }
            else { auto* stmt = Stmt(LOGIN_HUB_UPD_PROFILE); stmt->setUInt8(0, expansion); stmt->setUInt32(1, id); transaction->Append(stmt); }
        }
        else if (action == "ban" || action == "unban")
        {
            AppendId(transaction, LOGIN_HUB_UNBAN, id);
            if (action == "ban")
            {
                auto reason = Value(form, "reason"); Text(reason, 255);
                auto* stmt = Stmt(LOGIN_HUB_BAN); stmt->setUInt32(0, id); stmt->setUInt32(1, Number(Value(form, "minutes"), 525600) * 60);
                stmt->setString(2, actor.substr(0, 50)); stmt->setString(3, reason); transaction->Append(stmt);
            }
        }
        else if (action == "mute" || action == "unmute")
        {
            auto reason = action == "mute" ? Value(form, "reason") : ""; Text(reason, 255, action == "mute");
            int64 until = action == "mute" ? int64(std::time(nullptr)) + Number(Value(form, "minutes"), 525600, false) * 60 : 0;
            auto* stmt = Stmt(LOGIN_HUB_MUTE); stmt->setInt64(0, until); stmt->setString(1, reason); stmt->setString(2, actor.substr(0, 50)); stmt->setUInt32(3, id); transaction->Append(stmt);
        }
        else if (action == "gm")
        {
            int32 realm = Realm(form); uint8 level = uint8(Number(Value(form, "level"), 3));
            auto* stmt = Stmt(LOGIN_HUB_GM_CLEAR); stmt->setUInt32(0, id); stmt->setInt32(1, realm); stmt->setInt32(2, realm); transaction->Append(stmt);
            if (level) { stmt = Stmt(LOGIN_HUB_GM_SET); stmt->setUInt32(0, id); stmt->setUInt8(1, level); stmt->setInt32(2, realm); transaction->Append(stmt); }
        }
        else if (action == "rbac")
        {
            uint32 permission = Number(Value(form, "permission"), UINT32_MAX, false); int32 realm = Realm(form);
            auto* check = Stmt(LOGIN_HUB_PERMISSION_EXISTS); check->setUInt32(0, permission);
            if (!LoginDatabase.Query(check)) return Error(404, "RBAC permission does not exist.");
            auto mode = Value(form, "mode");
            if (mode != "grant" && mode != "deny" && mode != "revoke") return Error(400, "Select grant, deny or revoke.");
            auto* stmt = Stmt(mode == "revoke" ? LOGIN_HUB_RBAC_REVOKE : LOGIN_HUB_RBAC_SET);
            stmt->setUInt32(0, id); stmt->setUInt32(1, permission);
            if (mode == "revoke") stmt->setInt32(2, realm);
            else { stmt->setBool(2, mode == "grant"); stmt->setInt32(3, realm); }
            transaction->Append(stmt);
        }
        else if (ipAction)
        {
            boost::system::error_code error;
            auto address = boost::asio::ip::make_address_v4(Value(form, "ip"), error);
            if (error) return Error(400, "Enter a single valid IPv4 address.");
            ip = address.to_string();
            auto* stmt = Stmt(LOGIN_HUB_IP_UNBAN); stmt->setString(0, ip); transaction->Append(stmt);
            if (action == "ip-ban")
            {
                auto reason = Value(form, "reason"); Text(reason, 255);
                stmt = Stmt(LOGIN_HUB_IP_BAN); stmt->setString(0, ip); stmt->setUInt32(1, Number(Value(form, "minutes"), 525600) * 60);
                stmt->setString(2, actor.substr(0, 50)); stmt->setString(3, reason); transaction->Append(stmt);
            }
        }
        else return Error(404, "Unknown account action.");
        if (action != "create") Event(transaction, eventAction, id, ip, actor);
        if (!Commit(transaction)) return Error(409, "Could not confirm the account change. Refresh the account and check database errors before retrying.");
        if (action == "create")
        {
            std::string username = Value(form, "username");
            NormalizeCredential(username, MAX_ACCOUNT_STR);
            id = GetId(username);
        }
        SF_LOG_INFO("server.hub", "Administrator '%s' completed account action '%s' (account %u).", actor.c_str(), action.c_str(), id);
        return { 200, "{\"ok\":true,\"id\":" + std::to_string(id) + ",\"message\":\"Change saved. Online moderation updates are applied within a few seconds; credential and permission changes require reconnecting.\"}" };
    }
    catch (std::invalid_argument const& error) { return Error(400, error.what()); }
    catch (std::out_of_range const&) { return Error(400, "A numeric field is outside its allowed range."); }
}

std::string AccountAdministration::EncodeRequest(std::string const& action, Form form, std::string const& actor)
{
    form["action"] = action; form["actor"] = actor;
    std::string bytes;
    for (auto const& field : form)
    {
        if (field.first.size() > 64 || field.second.size() > 1024) return "";
        for (size_t length : {field.first.size(), field.second.size()}) { bytes += char(length >> 8); bytes += char(length & 255); }
        bytes += field.first; bytes += field.second;
    }
    if (bytes.size() > 4096) return "";
    char const* hex = "0123456789abcdef"; std::string encoded;
    for (unsigned char byte : bytes) { encoded += hex[byte >> 4]; encoded += hex[byte & 15]; }
    return encoded;
}

AccountAdminReply AccountAdministration::HandleEncodedRequest(std::string const& encoded)
{
    if (encoded.empty() || encoded.size() > 8192 || encoded.size() % 2) return Error(400, "Invalid account request.");
    auto nibble = [](char c) -> int { return c >= '0' && c <= '9' ? c-'0' : c >= 'a' && c <= 'f' ? c-'a'+10 : -1; };
    std::string bytes;
    for (size_t i=0; i<encoded.size(); i+=2) { int a=nibble(encoded[i]), b=nibble(encoded[i+1]); if (a<0 || b<0) return Error(400,"Invalid account request."); bytes += char(a*16+b); }
    Form form; size_t offset=0;
    while (offset<bytes.size())
    {
        if (bytes.size()-offset<4) return Error(400,"Invalid account request.");
        size_t key=uint8(bytes[offset])*256+uint8(bytes[offset+1]), value=uint8(bytes[offset+2])*256+uint8(bytes[offset+3]); offset+=4;
        if (!key || key>64 || value>1024 || key+value>bytes.size()-offset) return Error(400,"Invalid account request.");
        if (!form.emplace(bytes.substr(offset,key), bytes.substr(offset+key,value)).second) return Error(400,"Duplicate account field.");
        offset+=key+value;
    }
    std::string const action=Value(form,"action"), actor=Value(form,"actor");
    if (IsReadAction(action)) return Error(400,"Only account changes use the world channel.");
    return Handle(action,form,actor);
}
}
