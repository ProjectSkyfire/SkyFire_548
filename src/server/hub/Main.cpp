/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include <csignal>
#include <cstdio>
#include <cstring>
#include <string>

#include "Common.h"
#include "Configuration/Config.h"
#include "Configuration/ConfigVersion.h"
#include "Database/DatabaseEnv.h"
#include "Log.h"
#include "Platform/TimeUtils.h"
#include "SystemConfig.h"

#ifndef _SKYFIRE_HUB_CONFIG
#define _SKYFIRE_HUB_CONFIG "hubserver.conf"
#endif

// The shared database logger resolves this symbol even when no database appender is configured.
LoginDatabaseWorkerPool LoginDatabase;

namespace
{
    volatile std::sig_atomic_t StopEvent = 0;

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

    std::signal(SIGINT, HubServerSignalHandler);
    std::signal(SIGTERM, HubServerSignalHandler);
#ifdef _WIN32
    std::signal(SIGBREAK, HubServerSignalHandler);
#endif

    SF_LOG_INFO("server.hub", "Configured hub endpoint: %s:%d.", bindIp.c_str(), port);
    SF_LOG_INFO("server.hub", "Hub runtime shell initialized; connection distribution is not enabled yet.");
    SF_LOG_INFO("server.hub", "Press Ctrl-C to stop.");

    while (!StopEvent)
        Skyfire::SleepForSeconds(1);

    SF_LOG_INFO("server.hub", "Hub server stopped.");
    return 0;
}
