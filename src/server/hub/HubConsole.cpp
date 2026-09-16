/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "HubConsole.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <utility>

#ifdef _WIN32
#include <conio.h>
#else
#include <sys/select.h>
#include <unistd.h>
#endif

#include "Configuration/Config.h"
#include "Database/DatabaseEnv.h"
#include "Log.h"

namespace
{
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

HubCommandHandler::HubCommandHandler(std::string bindIp, uint16 port)
    : _bindIp(std::move(bindIp)), _port(port), _startedAt(std::chrono::steady_clock::now())
{
}

bool HubCommandHandler::Execute(std::string const& commandLine) const
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
    else if (command == "nodes")
        PrintNodes();
    else if (command == "admins")
        PrintAdmins();
    else if (command == "reload")
        ReloadConfiguration();
    else if (command == "stop" || command == "quit" || command == "exit")
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
    std::printf("  admins     List hub administrator identities and access flags.\n");
    std::printf("  reload     Reload configuration and logging settings.\n");
    std::printf("  stop       Stop the hub server.\n");
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
    std::printf("  Active nodes:  %llu\n",
        static_cast<unsigned long long>(nodes ? nodes->GetRowCount() : 0));
    std::printf("  Administrators: %llu\n",
        static_cast<unsigned long long>(admins ? admins->GetRowCount() : 0));
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

    std::printf("%-5s %-24s %-18s %-9s %-20s\n", "ID", "Username", "Access flags", "Enabled", "Last login");
    do
    {
        Field* fields = result->Fetch();
        std::ostringstream flags;
        flags << "0x" << std::hex << std::uppercase << fields[2].GetUInt64();
        std::string const lastLogin = fields[4].IsNull() ? "never" : fields[4].GetString();

        std::printf("%-5u %-24.24s %-18s %-9s %-20.20s\n", fields[0].GetUInt32(),
            fields[1].GetString().c_str(), flags.str().c_str(), fields[3].GetBool() ? "yes" : "no",
            lastLogin.c_str());
    } while (result->NextRow());
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
