/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include <csignal>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <string>
#include <utility>

#include "Common.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include "Configuration/Config.h"
#include "Configuration/ConfigVersion.h"
#include "Database/DatabaseEnv.h"
#include "HubConsole.h"
#include "HubProcessSupervisor.h"
#include "HubWebServer.h"
#include "Log.h"
#include "Platform/TimeUtils.h"
#include "SystemConfig.h"

#ifndef _SKYFIRE_HUB_CONFIG
#define _SKYFIRE_HUB_CONFIG "hubserver.conf"
#endif

// The shared database logger resolves this symbol even when no database appender is configured.
LoginDatabaseWorkerPool LoginDatabase;
HubDatabaseWorkerPool HubDatabase;

namespace
{
    volatile std::sig_atomic_t StopEvent = 0;

    std::filesystem::path GetExecutableDirectory(char const* program)
    {
#ifdef _WIN32
        char path[SKYFIRE_PATH_MAX];
        DWORD const length = GetModuleFileNameA(nullptr, path, SKYFIRE_PATH_MAX);
        if (length > 0 && length < SKYFIRE_PATH_MAX)
            return std::filesystem::path(std::string(path, length)).parent_path();
#endif

        std::error_code error;
#if defined(__linux__)
        std::filesystem::path const executable = std::filesystem::read_symlink("/proc/self/exe", error);
        if (!error)
            return executable.parent_path();
#endif

        std::filesystem::path const absoluteProgram = std::filesystem::absolute(program, error);
        if (!error)
            return absoluteProgram.parent_path();
        return std::filesystem::current_path();
    }

    void HubServerSignalHandler(int signal)
    {
        switch (signal)
        {
            case SIGINT:
            case SIGTERM:
#ifdef _WIN32
            case SIGBREAK:
#endif
                StopEvent = 1;
                break;
        }
    }

    void PrintUsage(char const* program)
    {
        std::printf("Usage:\n");
        std::printf(" %s [<options>]\n", program);
        std::printf("    -c config_file    use config_file as configuration file\n");
        std::printf("    --help            display this help and exit\n");
    }

    bool StartDatabase()
    {
        MySQL::Library_Init();

        std::string const connectionInfo = sConfigMgr->GetStringDefault("HubDatabaseInfo", "");
        if (connectionInfo.empty())
        {
            SF_LOG_ERROR("server.hub", "HubDatabaseInfo is not configured.");
            MySQL::Library_End();
            return false;
        }

        int32 workerThreads = sConfigMgr->GetIntDefault("HubDatabase.WorkerThreads", 1);
        if (workerThreads < 1 || workerThreads > 32)
        {
            SF_LOG_WARN("server.hub", "HubDatabase.WorkerThreads must be between 1 and 32; using 1.");
            workerThreads = 1;
        }

        int32 synchronousThreads = sConfigMgr->GetIntDefault("HubDatabase.SynchThreads", 1);
        if (synchronousThreads < 1 || synchronousThreads > 32)
        {
            SF_LOG_WARN("server.hub", "HubDatabase.SynchThreads must be between 1 and 32; using 1.");
            synchronousThreads = 1;
        }

        if (!HubDatabase.Open(connectionInfo, uint8(workerThreads), uint8(synchronousThreads)))
        {
            SF_LOG_ERROR("server.hub", "Cannot connect to the hub database.");
            MySQL::Library_End();
            return false;
        }

        SF_LOG_INFO("server.hub", "Started hub database connection pool.");
        return true;
    }

    void StopDatabase()
    {
        HubDatabase.Close();
        MySQL::Library_End();
    }
}

int main(int argc, char** argv)
{
    char const* configFile = _SKYFIRE_HUB_CONFIG;

    for (int index = 1; index < argc; ++index)
    {
        if (std::strcmp(argv[index], "--help") == 0)
        {
            PrintUsage(argv[0]);
            return 0;
        }

        if (std::strcmp(argv[index], "-c") == 0)
        {
            if (++index >= argc)
            {
                std::printf("Runtime-Error: -c option requires an input argument\n");
                PrintUsage(argv[0]);
                return 1;
            }

            configFile = argv[index];
            continue;
        }

        std::printf("Runtime-Error: unknown option '%s'\n", argv[index]);
        PrintUsage(argv[0]);
        return 1;
    }

    if (!sConfigMgr->LoadInitial(configFile))
    {
        std::printf("Invalid or missing configuration file: %s\n", configFile);
        std::printf("Verify that the file exists and has '[hubserver]' at the top.\n");
        return 1;
    }

    SF_LOG_INFO("server.hub", "hubserver. revision: %s", SKYFIRE_VER_PRODUCTVERSION_STR);
    SF_LOG_INFO("server.hub", "Using configuration file %s.", configFile);

    uint32 const confVersion = sConfigMgr->GetIntDefault("ConfVersion", 0);
    if (confVersion < Skyfire::ConfigVersion::Hub)
    {
        SF_LOG_WARN("server.hub", "Your hubserver.conf is out of date (found %u, expected %u).",
            confVersion, Skyfire::ConfigVersion::Hub);
    }

    std::string const bindIp = sConfigMgr->GetStringDefault("Hub.BindIP", "0.0.0.0");
    int const port = sConfigMgr->GetIntDefault("Hub.Port", 9100);
    if (port <= 0 || port > 65535)
    {
        SF_LOG_ERROR("server.hub", "Hub.Port must be between 1 and 65535; found %d.", port);
        return 1;
    }

    if (!StartDatabase())
        return 1;

    HubProcessSupervisor processSupervisor;
    std::string databaseRecordError;
    if (!processSupervisor.ReloadDatabaseRecords(databaseRecordError))
    {
        SF_LOG_ERROR("server.hub", "Unable to cache managed service database records: %s.",
            databaseRecordError.c_str());
        StopDatabase();
        return 1;
    }

    bool const webEnabled = sConfigMgr->GetBoolDefault("Web.Enable", true);
    std::string const webBindIp = sConfigMgr->GetStringDefault("Web.BindIP", "127.0.0.1");
    int const webPort = sConfigMgr->GetIntDefault("Web.Port", 54880);
    std::filesystem::path webRoot = sConfigMgr->GetStringDefault("Web.Root", "web");
    if (webRoot.is_relative())
        webRoot = GetExecutableDirectory(argv[0]) / webRoot;
    webRoot = webRoot.lexically_normal();
    bool const webAllowRemote = sConfigMgr->GetBoolDefault("Web.AllowRemote", false);
    int webSessionTimeout = sConfigMgr->GetIntDefault("Web.SessionTimeout", 1800);
    if (webPort <= 0 || webPort > 65535)
    {
        SF_LOG_ERROR("server.hub", "Web.Port must be between 1 and 65535; found %d.", webPort);
        StopDatabase();
        return 1;
    }
    if (webSessionTimeout < 300 || webSessionTimeout > 86400)
    {
        SF_LOG_WARN("server.hub", "Web.SessionTimeout must be between 300 and 86400 seconds; using 1800.");
        webSessionTimeout = 1800;
    }

    HubWebServer webServer;
    if (webEnabled && !webServer.Open(webBindIp, uint16(webPort), webRoot.string(), webAllowRemote,
        uint32(webSessionTimeout)))
    {
        SF_LOG_ERROR("server.hub", "Unable to start the hub web console.");
        StopDatabase();
        return 1;
    }

    int32 maxPingTime = sConfigMgr->GetIntDefault("MaxPingTime", 30);
    if (maxPingTime < 1 || maxPingTime > 1440)
    {
        SF_LOG_WARN("server.hub", "MaxPingTime must be between 1 and 1440 minutes; using 30.");
        maxPingTime = 30;
    }

    auto const databasePingInterval = std::chrono::minutes(maxPingTime);
    auto nextDatabasePing = std::chrono::steady_clock::now() + databasePingInterval;

    std::signal(SIGINT, HubServerSignalHandler);
    std::signal(SIGTERM, HubServerSignalHandler);
#ifdef _WIN32
    std::signal(SIGBREAK, HubServerSignalHandler);
#else
    std::signal(SIGPIPE, SIG_IGN);
#endif

    SF_LOG_INFO("server.hub", "Configured hub endpoint: %s:%d.", bindIp.c_str(), port);
    SF_LOG_INFO("server.hub", "Hub runtime shell initialized; connection distribution is not enabled yet.");
    SF_LOG_INFO("server.hub", "Press Ctrl-C to stop.");

    bool const consoleEnabled = sConfigMgr->GetBoolDefault("Console.Enable", true);
    auto const hubStartedAt = std::chrono::steady_clock::now();
    HubConsoleInput console;
    HubCommandHandler commandHandler(bindIp, uint16(port), processSupervisor);
    if (consoleEnabled)
        console.PrintPrompt();

    while (!StopEvent)
    {
        processSupervisor.Update();

        HubWebServiceCommand webCommand;
        while (webEnabled && webServer.PollServiceCommand(webCommand))
        {
            std::string error;
            bool const accepted = webCommand.Start
                ? processSupervisor.Start(webCommand.ServiceKey, error)
                : processSupervisor.Stop(webCommand.ServiceKey, error);
            if (!accepted)
                SF_LOG_WARN("server.hub", "Web console could not %s managed service '%s': %s.",
                    webCommand.Start ? "start" : "stop", webCommand.ServiceKey.c_str(), error.c_str());
        }

        if (webEnabled)
        {
            HubWebStatusSnapshot status;
            status.UptimeSeconds = uint64(std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::steady_clock::now() - hubStartedAt).count());
            for (HubManagedServiceStatus const& service : processSupervisor.GetStatuses())
            {
                HubWebManagedServiceStatus webService;
                webService.Key = service.Key;
                webService.Name = service.Name;
                webService.State = HubProcessSupervisor::GetStateName(service.State);
                webService.ProcessId = service.ProcessId;
                webService.LastExitCode = service.LastExitCode;
                webService.Enabled = service.Enabled;
                status.Services.push_back(std::move(webService));
            }
            webServer.UpdateStatus(status);
        }

        if (consoleEnabled)
        {
            std::string command;
            HubConsolePollResult const pollResult = console.Poll(command);
            if (pollResult == HubConsolePollResult::Closed)
                StopEvent = 1;
            else if (pollResult == HubConsolePollResult::Command)
            {
                if (!commandHandler.Execute(command, HubCommandOrigin::LocalConsole))
                    StopEvent = 1;
                else
                    console.PrintPrompt();
            }
        }

        auto const now = std::chrono::steady_clock::now();
        if (now >= nextDatabasePing)
        {
            HubDatabase.KeepAlive();
            nextDatabasePing = now + databasePingInterval;
        }

        Skyfire::SleepForMilliseconds(50);
    }

    webServer.Close();
    processSupervisor.StopAll();
    StopDatabase();
    SF_LOG_INFO("server.hub", "Hub server stopped.");
    return 0;
}
