/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_HUB_CONSOLE_H
#define SKYFIRE_HUB_CONSOLE_H

#include <chrono>
#include <string>

#include "Define.h"

enum class HubConsolePollResult
{
    None,
    Command,
    Closed
};

class HubConsoleInput
{
public:
    void PrintPrompt() const;
    HubConsolePollResult Poll(std::string& command);

private:
#ifdef _WIN32
    std::string _buffer;
#endif
};

class HubCommandHandler
{
public:
    HubCommandHandler(std::string bindIp, uint16 port);

    // Returns false when the command requests server shutdown.
    bool Execute(std::string const& commandLine) const;

private:
    void PrintHelp() const;
    void PrintStatus() const;
    void PrintNodes() const;
    void PrintAdmins() const;
    void ReloadConfiguration() const;

    std::string _bindIp;
    uint16 _port;
    std::chrono::steady_clock::time_point _startedAt;
};

#endif
