/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "HubConsole.h"
#include "HubClusterServer.h"
#include "HubAuthProxy.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <utility>

#include <openssl/evp.h>

#ifdef _WIN32
#include <conio.h>
#else
#include <sys/select.h>
#include <unistd.h>
#endif

#include "Configuration/Config.h"
#include "Cryptography/CryptoRandom.h"
#include "Database/DatabaseEnv.h"
#include "HubProcessSupervisor.h"
#include "Log.h"
#include "Utilities/Util.h"

namespace
{
    constexpr int HubAdminPasswordIterations = 210000;

    std::string ToLower(std::string value)
    {
        std::transform(value.begin(), value.end(), value.begin(),
            [](unsigned char character) { return char(std::tolower(character)); });
        return value;
    }

    char const* GetNodeTypeName(uint8 nodeType)
    {
        switch (nodeType)
        {
            case 1: return "auth";
            case 2: return "world";
            case 3: return "service";
            default: return "unspecified";
        }
    }

    char const* GetNodeStatusName(uint8 status)
    {
        switch (status)
        {
            case 1: return "online";
            case 2: return "offline";
            case 3: return "degraded";
            default: return "unknown";
        }
    }

    bool IsValidAdminUsername(std::string const& username)
    {
        return username.size() >= 3 && username.size() <= 64 &&
            std::all_of(username.begin(), username.end(), [](unsigned char character)
            {
                return std::isalnum(character) || character == '.' || character == '_' ||
                    character == '-' || character == '@';
            });
    }

    bool ParseAdminAccessFlags(std::string const& value, uint64& accessFlags)
    {
        try
        {
            size_t parsedLength = 0;
            accessFlags = std::stoull(value, &parsedLength, 0);
            return parsedLength == value.size() && accessFlags != 0 &&
                (accessFlags & ~uint64(HUB_ADMIN_ACCESS_ALL_LOCAL)) == 0;
        }
        catch (std::invalid_argument const&)
        {
            return false;
        }
        catch (std::out_of_range const&)
        {
            return false;
        }
    }

    bool CreateAdminPasswordHash(std::string const& password, std::string& passwordHash)
    {
        std::array<uint8, 16> salt = SkyFire::Crypto::GetRandomBytes<16>();
        std::array<uint8, 32> digest = { };
        if (PKCS5_PBKDF2_HMAC(password.data(), int(password.size()), salt.data(), int(salt.size()),
            HubAdminPasswordIterations, EVP_sha256(), int(digest.size()), digest.data()) != 1)
            return false;

        passwordHash = "$pbkdf2-sha256$" + std::to_string(HubAdminPasswordIterations) + '$' +
            ByteArrayToHexStr(salt) + '$' + ByteArrayToHexStr(digest);
        return true;
    }
}

void HubConsoleInput::PrintPrompt() const
{
    std::printf("Hub> ");
    std::fflush(stdout);
}

HubConsolePollResult HubConsoleInput::Poll(std::string& command)
{
#ifdef _WIN32
    while (_kbhit())
    {
        int const character = _getch();
        if (character == 0 || character == 0xE0)
        {
            if (_kbhit())
                (void)_getch();
            continue;
        }

        if (character == '\r' || character == '\n')
        {
            std::printf("\n");
            command = std::move(_buffer);
            _buffer.clear();
            return HubConsolePollResult::Command;
        }

        if (character == '\b')
        {
            if (!_buffer.empty())
            {
                _buffer.pop_back();
                std::printf("\b \b");
                std::fflush(stdout);
            }
            continue;
        }

        if (character == 26)
            return HubConsolePollResult::Closed;

        if (character >= 32 && character <= 126)
        {
            _buffer.push_back(char(character));
            std::putchar(character);
            std::fflush(stdout);
        }
    }
#else
    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(STDIN_FILENO, &readSet);
    timeval timeout = { 0, 0 };

    int const result = select(STDIN_FILENO + 1, &readSet, nullptr, nullptr, &timeout);
    if (result < 0)
        return HubConsolePollResult::Closed;

    if (result > 0 && FD_ISSET(STDIN_FILENO, &readSet))
    {
        if (!std::getline(std::cin, command))
            return HubConsolePollResult::Closed;
        return HubConsolePollResult::Command;
    }
#endif

    return HubConsolePollResult::None;
}

HubCommandHandler::HubCommandHandler(std::string bindIp, uint16 port,
    HubProcessSupervisor& processSupervisor, HubClusterServer& clusterServer, HubAuthProxy& authnetProxy, HubAuthProxy& legacyProxy)
    : _bindIp(std::move(bindIp)), _port(port), _startedAt(std::chrono::steady_clock::now()),
      _processSupervisor(processSupervisor), _clusterServer(clusterServer), _authnetProxy(authnetProxy), _legacyProxy(legacyProxy)
{
}

bool HubCommandHandler::Execute(std::string const& commandLine, HubCommandOrigin origin) const
{
    std::istringstream input(commandLine);
    std::string command;
    input >> command;
    command = ToLower(command);

    if (command.empty())
        return true;

    if (command == "help" || command == "?")
        PrintHelp();
    else if (command == "status")
        PrintStatus();
    else if (command == "registry")
        PrintRegistry();
    else if (command == "routing")
        PrintRouting();
    else if (command == "handoffs")
    {
        auto const status = _clusterServer.HandoffStatus();
        std::printf("Handoffs: issued %llu, consumed %llu, revoked %llu, expired %llu, rejected %llu, replayed %llu.\n",
            static_cast<unsigned long long>(status.Issued),static_cast<unsigned long long>(status.Consumed),
            static_cast<unsigned long long>(status.Revoked),static_cast<unsigned long long>(status.Expired),
            static_cast<unsigned long long>(status.Rejected),static_cast<unsigned long long>(status.Replayed));
    }
    else if (command == "cluster")
    {
        std::string action, key, extra, error;
        input >> action >> key;
        if (origin != HubCommandOrigin::LocalConsole)
            std::printf("Cluster maintenance requires the local hub console.\n");
        else if (key.empty() || input >> extra)
            std::printf("Usage: cluster <drain|disable|enable> <node-key>\n");
        else if (!_clusterServer.SetAdministration(key,action,"local-console",error))
            std::printf("Cluster maintenance rejected: %s\n",error.c_str());
        else
            std::printf("Cluster policy saved for %s. Existing connections remain pinned; use registry for hub connection counts.\n",key.c_str());
    }
    else if (command == "nodes")
        PrintNodes();
    else if (command == "admins")
        PrintAdmins();
    else if (command == "start")
    {
        std::string service;
        std::string extra;
        input >> service;
        service = ToLower(service);
        if (service.empty() || input >> extra)
        {
            std::printf("Usage: start <service>\n");
            return true;
        }

        if (origin != HubCommandOrigin::LocalConsole)
        {
            std::printf("Managed services can only be started from the local hub console.\n");
            return true;
        }

        std::string error;
        if (!_processSupervisor.Start(service, error))
            std::printf("Unable to start %s: %s.\n", service.c_str(), error.c_str());
        else
            std::printf("%s start requested; waiting for the child process to report ready.\n", service.c_str());
    }
    else if (command == "world" || command == ".server" || command == "server" || HubProcessSupervisor::IsWorldKey(command))
    {
        if (origin != HubCommandOrigin::LocalConsole)
        {
            std::printf("World commands require the local hub console.\n");
            return true;
        }
        std::string text;
        std::getline(input >> std::ws, text);
        if (!HubProcessSupervisor::IsWorldKey(command))
            text = "server " + text;
        std::string error;
        if (!_processSupervisor.SendWorldCommand(text, error, HubProcessSupervisor::IsWorldKey(command) ? command : "world"))
            std::printf("World command rejected: %s.\n", error.c_str());
        else
            std::printf("World command sent; waiting for its result.\n");
    }
    else if (command == "admin")
    {
        std::string subcommand;
        input >> subcommand;
        subcommand = ToLower(subcommand);

        if (subcommand != "create")
        {
            std::printf("Usage: admin create <username> <password> [access_flags]\n");
            return true;
        }

        if (origin != HubCommandOrigin::LocalConsole)
        {
            std::printf("Administrator accounts can only be created from the local hub console.\n");
            return true;
        }

        std::string username;
        std::string password;
        std::string accessText;
        std::string extra;
        input >> username >> password >> accessText;
        if (username.empty() || password.empty() || input >> extra)
        {
            std::printf("Usage: admin create <username> <password> [access_flags]\n");
            return true;
        }

        uint64 accessFlags = HUB_ADMIN_ACCESS_ALL_LOCAL;
        if (!accessText.empty() && !ParseAdminAccessFlags(accessText, accessFlags))
        {
            std::printf("Invalid access flags. Use a decimal or 0x value containing only bits 0x1 through 0x8.\n");
            return true;
        }

        CreateAdmin(username, password, accessFlags);
    }
    else if (command == "reload" || command == ".reload")
    {
        std::string target;
        std::string extra;
        input >> target >> extra;
        target = ToLower(target);
        if (!extra.empty())
            std::printf("Usage: reload [db_records]\n");
        else if (target.empty())
            ReloadConfiguration();
        else if (target == "db_records")
        {
            std::string error;
            if (!_processSupervisor.ReloadDatabaseRecords(error))
                std::printf("Database record reload failed: %s.\n", error.c_str());
            else
                std::printf("Managed service database records reloaded into memory.\n");
        }
        else
            std::printf("Usage: reload [db_records]\n");
    }
    else if (command == "stop")
    {
        std::string service;
        std::string extra;
        input >> service;
        service = ToLower(service);
        if (service.empty())
        {
            std::printf("Stopping hub server...\n");
            return false;
        }
        if (input >> extra)
        {
            std::printf("Usage: stop [service]\n");
            return true;
        }
        if (origin != HubCommandOrigin::LocalConsole)
        {
            std::printf("Managed services can only be stopped from the local hub console.\n");
            return true;
        }
        std::string error;
        if (!_processSupervisor.Stop(service, error))
            std::printf("Unable to stop %s: %s.\n", service.c_str(), error.c_str());
        else
            std::printf("%s stop requested.\n", service.c_str());
    }
    else if (command == "quit" || command == "exit")
    {
        std::printf("Stopping hub server...\n");
        return false;
    }
    else
        std::printf("Unknown command '%s'. Enter 'help' for available commands.\n", command.c_str());

    return true;
}

void HubCommandHandler::PrintHelp() const
{
    std::printf("Available hub commands:\n");
    std::printf("  help       Show this command list.\n");
    std::printf("  status     Show hub uptime, endpoint, and database record counts.\n");
    std::printf("  nodes      List enabled routing nodes.\n");
    std::printf("  registry   List authenticated live cluster registrations.\n");
    std::printf("  routing    Show authentication ingress connections, routes and failures.\n");
    std::printf("  handoffs   Show shared handoff counters (no token material).\n");
    std::printf("  cluster <drain|disable|enable> <node-key>  Persist authentication routing policy.\n");
    std::printf("  start <service>\n");
    std::printf("             Start and supervise a database-configured service.\n");
    std::printf("  stop <service>\n");
    std::printf("             Gracefully stop a managed service.\n");
    std::printf("  world <command>  Execute a command on the default world node (optional leading dot).\n");
    std::printf("  world-<id> <command>  Execute a command on a named world node.\n");
    std::printf("  .server restart 300 | .server shutdown 300  Graceful countdown in seconds.\n");
    std::printf("  .server shutdown time 23:00 | .server restart time 23:00  Server local time.\n");
    std::printf("  .server shutdown cancel | .server restart cancel  Cancel the countdown.\n");
    std::printf("  admins     List hub administrator identities and access flags.\n");
    std::printf("  admin create <username> <password> [access_flags]\n");
    std::printf("             Create a local-only administrator (default access flags: 0xF).\n");
    std::printf("  reload     Reload configuration and logging settings.\n");
    std::printf("  .reload db_records\n");
    std::printf("             Refresh cached managed service records from the hub database.\n");
    std::printf("  stop       Stop the hub server when no service is supplied.\n");
}

void HubCommandHandler::PrintStatus() const
{
    auto const uptime = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::steady_clock::now() - _startedAt).count();

    PreparedQueryResult nodes = HubDatabase.Query(
        HubDatabase.GetPreparedStatement(HUB_SEL_ACTIVE_ROUTING_NODES));
    PreparedQueryResult admins = HubDatabase.Query(
        HubDatabase.GetPreparedStatement(HUB_SEL_ADMINS));

    std::printf("Hub status:\n");
    std::printf("  Endpoint:      %s:%u\n", _bindIp.c_str(), unsigned(_port));
    std::printf("  Uptime:        %lld seconds\n", static_cast<long long>(uptime));
    std::printf("  Configured active routes: %llu\n",
        static_cast<unsigned long long>(nodes ? nodes->GetRowCount() : 0));
    std::printf("  Live cluster services: %llu\n", static_cast<unsigned long long>(_clusterServer.Snapshot().size()));
    std::printf("  Administrators: %llu\n",
        static_cast<unsigned long long>(admins ? admins->GetRowCount() : 0));
    for (HubManagedServiceStatus const& service : _processSupervisor.GetStatuses())
    {
        std::printf("  %-14s %s", service.Name.c_str(), HubProcessSupervisor::GetStateName(service.State));
        if (service.ProcessId)
            std::printf(" (process %llu)", static_cast<unsigned long long>(service.ProcessId));
        if (service.State == HubManagedProcessState::Exited)
            std::printf(" (exit code %lld)", static_cast<long long>(service.LastExitCode));
        std::printf(" [%s] uptime %llu seconds", service.Key.c_str(), static_cast<unsigned long long>(service.UptimeSeconds));
        if (service.IsWorld && service.MetricsAvailable)
        {
            std::printf(" | players %u | update %u ms", service.Players, service.UpdateTimeMs);
            if (service.CpuBasisPoints >= 0)
                std::printf(" | CPU %.2f%%", service.CpuBasisPoints / 100.0);
            else
                std::printf(" | CPU unavailable");
        }
        else if (service.IsWorld)
            std::printf(" | players/load unavailable");
        if (!service.Enabled)
            std::printf(" (disabled)");
        std::printf("\n");
    }
}

void HubCommandHandler::PrintRegistry() const
{
    if (!_clusterServer.IsOpen())
    {
        std::printf("Cluster listener is disabled. Configure Hub.Cluster.Enable and TLS certificates.\n");
        return;
    }
    auto const nodes = _clusterServer.Directory();
    auto counts = _authnetProxy.Status().ConnectionsByNode;
    for (auto const& entry : _legacyProxy.Status().ConnectionsByNode) counts[entry.first] += entry.second;
    std::printf("Cluster directory: %u node(s), including saved offline policies, protocol v1.\n", unsigned(nodes.size()));
    for (auto const& node : nodes)
    {
        std::printf("  %s (%s) %s %s:%u realm %u build %u load %u/%u %s\n", node.Key.c_str(), node.Name.c_str(),
            node.Type == Skyfire::Cluster::Service::Auth ? ((node.Capabilities & 16) ? "authnet" : "auth") : "world", node.Address.c_str(), unsigned(node.Port),
            node.Realm, node.Build, node.Load, node.Capacity, node.Ready ? "ready" : "not ready");
        std::printf("    %s | policy %s | hub connections %llu\n",node.Live ? "registered" : "offline",
            Skyfire::Cluster::AdministrationName(node.Admin),static_cast<unsigned long long>(counts[node.Key]));
        if (!node.Realms.empty())
        {
            std::printf("    Realms:");
            for (auto realm : node.Realms) std::printf(" %u", realm);
            std::printf("\n");
        }
    }
}

void HubCommandHandler::PrintRouting() const
{
    for (auto proxy : {&_authnetProxy, &_legacyProxy})
    {
        auto const s = proxy->Status();
        if (!s.Enabled) { std::printf("  Authentication ingress listener disabled.\n"); continue; }
        std::printf("  %s %s:%u active %llu accepted %llu routed %llu rejected %llu\n"
            "    attempts %llu connect failures %llu retries %llu stream failures %llu bytes up/down %llu/%llu\n",
            s.Name.c_str(), s.Address.c_str(), unsigned(s.Port), (unsigned long long)s.Active,
            (unsigned long long)s.Accepted, (unsigned long long)s.Routed, (unsigned long long)s.Rejected,
            (unsigned long long)s.Attempts, (unsigned long long)s.ConnectFailures, (unsigned long long)s.Retries,
            (unsigned long long)s.StreamFailures, (unsigned long long)s.ClientBytes, (unsigned long long)s.BackendBytes);
        std::printf("    unavailable %llu limit rejected %llu withdrawn connects %llu backoff nodes %llu\n",
            (unsigned long long)s.NoBackend,(unsigned long long)s.LimitRejected,(unsigned long long)s.Withdrawn,(unsigned long long)s.BackoffNodes);
        if (!s.LastRejection.empty()) std::printf("    Last rejection: %s\n",s.LastRejection.c_str());
        std::printf("    Eligible nodes now: %llu\n",(unsigned long long)s.AvailableNodes);
    }
}

void HubCommandHandler::PrintNodes() const
{
    PreparedQueryResult result = HubDatabase.Query(
        HubDatabase.GetPreparedStatement(HUB_SEL_ACTIVE_ROUTING_NODES));
    if (!result)
    {
        std::printf("No enabled routing nodes are configured.\n");
        return;
    }

    std::printf("%-5s %-20s %-11s %-30s %-7s %-8s %-8s %-11s\n",
        "ID", "Name", "Type", "Endpoint", "Realm", "Load", "Weight", "Status");

    do
    {
        Field* fields = result->Fetch();
        std::ostringstream endpoint;
        endpoint << fields[4].GetString() << ':' << fields[5].GetUInt16();
        std::string const realm = fields[6].IsNull() ? "-" : std::to_string(fields[6].GetUInt32());
        std::string const load = std::to_string(fields[11].GetUInt32()) + "/" +
            (fields[10].GetUInt32() ? std::to_string(fields[10].GetUInt32()) : "-");

        std::printf("%-5u %-20.20s %-11s %-30.30s %-7s %-8s %-8u %-11s%s\n",
            fields[0].GetUInt32(), fields[2].GetString().c_str(), GetNodeTypeName(fields[3].GetUInt8()),
            endpoint.str().c_str(), realm.c_str(), load.c_str(), unsigned(fields[9].GetUInt16()),
            GetNodeStatusName(fields[14].GetUInt8()), fields[13].GetBool() ? " maintenance" : "");
    } while (result->NextRow());
}

void HubCommandHandler::PrintAdmins() const
{
    PreparedQueryResult result = HubDatabase.Query(HubDatabase.GetPreparedStatement(HUB_SEL_ADMINS));
    if (!result)
    {
        std::printf("No hub administrators are configured.\n");
        return;
    }

    std::printf("%-5s %-24s %-18s %-11s %-9s %-20s\n",
        "ID", "Username", "Access flags", "Scope", "Enabled", "Last login");
    do
    {
        Field* fields = result->Fetch();
        std::ostringstream flags;
        flags << "0x" << std::hex << std::uppercase << fields[2].GetUInt64();
        std::string const lastLogin = fields[4].IsNull() ? "never" : fields[4].GetString();

        std::printf("%-5u %-24.24s %-18s %-11s %-9s %-20.20s\n", fields[0].GetUInt32(),
            fields[1].GetString().c_str(), flags.str().c_str(),
            HubAdminCanLoginRemotely(fields[2].GetUInt64()) ? "remote" : "local-only",
            fields[3].GetBool() ? "yes" : "no", lastLogin.c_str());
    } while (result->NextRow());
}

void HubCommandHandler::CreateAdmin(std::string const& username, std::string const& password,
    uint64 accessFlags) const
{
    if (!IsValidAdminUsername(username))
    {
        std::printf("Invalid username. Use 3-64 letters, numbers, periods, underscores, hyphens, or @ characters.\n");
        return;
    }

    if (password.size() < 8 || password.size() > 128)
    {
        std::printf("Invalid password. Passwords must contain 8-128 characters and cannot contain spaces.\n");
        return;
    }

    PreparedStatement* selectAdmin = HubDatabase.GetPreparedStatement(HUB_SEL_ADMIN_BY_USERNAME);
    selectAdmin->setString(0, username);
    if (HubDatabase.Query(selectAdmin))
    {
        std::printf("Administrator '%s' already exists.\n", username.c_str());
        return;
    }

    std::string passwordHash;
    if (!CreateAdminPasswordHash(password, passwordHash))
    {
        std::printf("Unable to create the administrator password verifier.\n");
        return;
    }

    PreparedStatement* insertAdmin = HubDatabase.GetPreparedStatement(HUB_INS_ADMIN);
    insertAdmin->setString(0, username);
    insertAdmin->setString(1, passwordHash);
    insertAdmin->setUInt64(2, accessFlags);
    HubDatabase.DirectExecute(insertAdmin);

    std::printf("Created local-only administrator '%s' with access flags 0x%llX and no remote privileges.\n",
        username.c_str(), static_cast<unsigned long long>(accessFlags));
}

void HubCommandHandler::ReloadConfiguration() const
{
    if (!sConfigMgr->Reload())
    {
        std::printf("Configuration reload failed.\n");
        return;
    }

    sLog->LoadFromConfig();
    std::printf("Configuration and logging settings reloaded. Database, endpoint, and console changes require a restart.\n");
}
