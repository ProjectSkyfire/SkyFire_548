/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include <algorithm>
#include <csignal>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <utility>

#include "Common.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include "Configuration/Config.h"
#include "Configuration/ConfigVersion.h"
#include "Database/DatabaseEnv.h"
#include "HubConsole.h"
#include "HubDatabaseSetup.h"
#include "HubBackupGuard.h"
#include "HubClusterServer.h"
#include "Cluster/MapDataStartCheck.h"
#include <fstream>
#include "HubAuthProxy.h"
#include "Auth/AccountAdministration.h"
#include "HubProcessSupervisor.h"
#include "HubWebServer.h"
#include "Log.h"
#include "Packets/PacketLogServer.h"
#include "Platform/TimeUtils.h"
#include "SystemConfig.h"

#ifndef _SKYFIRE_HUB_CONFIG
#define _SKYFIRE_HUB_CONFIG "hubserver.conf"
#endif

// LoginDatabase also backs optional hub player-account administration.
LoginDatabaseWorkerPool LoginDatabase;
HubDatabaseWorkerPool HubDatabase;

namespace
{
    volatile std::sig_atomic_t StopEvent = 0;

    bool OpenAuthIngress(HubAuthProxy& proxy, bool authnet, bool clusterEnabled, std::string& error)
    {
        std::string const prefix = authnet ? "Hub.AuthnetIngress." : "Hub.LegacyIngress.";
        if (!sConfigMgr->GetBoolDefault((prefix + "Enable").c_str(), false)) return true;
        if (!clusterEnabled) { error = prefix + "Enable requires Hub.Cluster.Enable."; return false; }
        HubAuthProxyOptions options;
        options.Authnet = authnet;
        options.Address = sConfigMgr->GetStringDefault((prefix + "BindIP").c_str(), "127.0.0.1");
        int const port = sConfigMgr->GetIntDefault((prefix + "Port").c_str(), authnet ? 1118 : 3723);
        if (port < 1 || port > 65535) { error = prefix + "Port must be 1..65535."; return false; }
        options.Port = uint16(port);
        options.PreserveClientIP = sConfigMgr->GetBoolDefault((prefix + "PreserveClientIP").c_str(), true);
        options.MaxConnections = sConfigMgr->GetIntDefault((prefix + "MaxConnections").c_str(), 4096);
        options.ConnectTimeoutSeconds = sConfigMgr->GetIntDefault((prefix + "ConnectTimeout").c_str(), 3);
        options.IdleTimeoutSeconds = sConfigMgr->GetIntDefault((prefix + "IdleTimeout").c_str(), 300);
        options.MaxAttempts = sConfigMgr->GetIntDefault((prefix + "MaxAttempts").c_str(), 3);
        std::istringstream weights(sConfigMgr->GetStringDefault((prefix + "Weights").c_str(), ""));
        std::string entry;
        while (weights >> entry)
        {
            auto const split = entry.find('=');
            std::string const key = entry.substr(0, split);
            std::string const value = split == std::string::npos ? "" : entry.substr(split + 1);
            if (!Skyfire::Cluster::ValidKey(key) || value.empty() || value.size() > 4 ||
                value.find_first_not_of("0123456789") != std::string::npos || options.Weights.count(key) || options.Weights.size() >= 128)
            { error = prefix + "Weights requires unique node-key=weight entries separated by spaces."; return false; }
            options.Weights.emplace(key, unsigned(std::stoul(value)));
        }
        if (!proxy.Open(std::move(options), error)) { error = prefix + error; return false; }
        auto const status = proxy.Status();
        SF_LOG_INFO("server.hub", "%s listening on %s:%u; client IP preservation %s.", status.Name.c_str(),
            status.Address.c_str(), unsigned(status.Port), sConfigMgr->GetBoolDefault((prefix + "PreserveClientIP").c_str(), true) ? "enabled" : "disabled");
        return true;
    }

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

        if (!SetupHubDatabase(connectionInfo))
        {
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
            SF_LOG_ERROR("server.hub", "Cannot open the hub database. Check HubDatabaseInfo and HubDatabase.SqlPath; the hub schema must match this build.");
            MySQL::Library_End();
            return false;
        }

        std::string const loginInfo = sConfigMgr->GetStringDefault("LoginDatabaseInfo", "");
        if (!loginInfo.empty())
        {
            if (!LoginDatabase.Open(loginInfo, 1, 2))
            {
                SF_LOG_ERROR("server.hub", "Cannot open the account database. Apply auth updates and check LoginDatabaseInfo.");
                LoginDatabase.Close();
                HubDatabase.Close();
                MySQL::Library_End();
                return false;
            }
            Skyfire::Auth::AccountAdministration::SetEnabled(true);
        }
        SF_LOG_INFO("server.hub", "Started hub database connection pool.");
        return true;
    }

    void StopDatabase()
    {
        if (Skyfire::Auth::AccountAdministration::IsEnabled())
            LoginDatabase.Close();
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

    sPacketLogServer->Configure(sConfigMgr->GetStringDefault("PacketLogServerControlFile", ""),
        sConfigMgr->GetStringDefault("PacketLogServerOutputDir", "PacketLogs"),
        sConfigMgr->GetStringDefault("LogsDir", ""));
    if (sConfigMgr->GetBoolDefault("Hub.PacketLog.Enable", false))
        sPacketLogServer->EnableGlobalLogging();

    if (!StartDatabase())
        return 1;

    HubProcessSupervisor processSupervisor;
    processSupervisor.ConfigureFallback();
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

    std::string controlToken;
    auto controlTokenPath = sConfigMgr->GetStringDefault("Web.ControlTokenFile", "");
    if (!controlTokenPath.empty())
    {
        std::filesystem::path path(controlTokenPath);
        if (path.is_relative()) path = std::filesystem::absolute(sConfigMgr->GetFilename()).parent_path() / path;
        std::ifstream tokenFile(path); std::getline(tokenFile,controlToken);
        if (!controlToken.empty() && controlToken.back() == '\r') controlToken.pop_back();
        boost::system::error_code ec; auto address = boost::asio::ip::make_address(webBindIp,ec);
        if (ec || !address.is_loopback() || webAllowRemote || controlToken.size() != 64 ||
            controlToken.find_first_not_of("0123456789abcdef") != std::string::npos)
        {
            SF_LOG_ERROR("server.hub","Control gateway requires a loopback-only web listener and a 64-character lowercase hex token file.");
            StopDatabase(); return 1;
        }
    }
    HubWebServer webServer;
    if (webEnabled && !webServer.Open(webBindIp, uint16(webPort), webRoot.string(), webAllowRemote,
        uint32(webSessionTimeout),controlToken))
    {
        SF_LOG_ERROR("server.hub", "Unable to start the hub web console.");
        StopDatabase();
        return 1;
    }

    HubClusterServer clusterServer;
    processSupervisor.SetClusterServer(&clusterServer);
    processSupervisor.SetWorldStartCheck([&clusterServer](std::string const& config, std::string& error)
    {
        Config settings;
        if (!ConfigLoader::Load(config, settings)) { error = "Cannot read world configuration."; return false; }
        auto value = [&](char const* key, std::string fallback = "") {
            for (auto const& section : settings) { auto entry=section.second.find(key); if(entry!=section.second.end()) return entry->second; }
            return fallback;
        };
        auto enabled=value("CharacterService.Enable");
        auto now = std::uint64_t(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count());
        if (enabled=="1" || enabled=="true" || enabled=="TRUE" || enabled=="yes" || enabled=="YES")
        {
            bool found=false;
            for(auto const& node:clusterServer.Snapshot())
                if(node.Type==Skyfire::Cluster::Service::Character && node.Key==value("CharacterService.NodeKey","characters-1") &&
                    node.Address==value("CharacterService.Host","127.0.0.1") && std::to_string(node.Port)==value("CharacterService.Port","54930") &&
                    node.Live && node.ExpiresAt>now && node.Ready && node.Admin==Skyfire::Cluster::Administration::Enabled && (!node.Capacity || node.Load<node.Capacity)) found=true;
            if(!found) { error="Required character server is offline, unready, full or its endpoint differs from the world configuration."; return false; }
        }
        std::ifstream input(config);
        return Skyfire::Cluster::MapData::CheckWorldStart(input, clusterServer.Snapshot(), now, error);
    });
    bool const clusterEnabled = sConfigMgr->GetBoolDefault("Hub.Cluster.Enable", false);
    if (clusterEnabled)
    {
        auto clusterPath = [&](char const* key)
        {
            std::filesystem::path path = sConfigMgr->GetStringDefault(key, "");
            if (path.empty()) return std::string();
            if (path.is_relative()) path = GetExecutableDirectory(argv[0]) / path;
            return path.lexically_normal().string();
        };
        int const lease = sConfigMgr->GetIntDefault("Hub.Cluster.LeaseSeconds", 15);
        int const limit = sConfigMgr->GetIntDefault("Hub.Cluster.MaxConnections", 128);
        if (lease < 5 || lease > 300 || limit < 1 || limit > 1024 ||
            !clusterServer.Open(bindIp, uint16(port), clusterPath("Hub.Cluster.Certificate"),
                clusterPath("Hub.Cluster.PrivateKey"), clusterPath("Hub.Cluster.CA"), uint32(lease), size_t(limit)))
        {
            SF_LOG_ERROR("server.hub", "Cluster listener startup failed. Check TLS files, Hub.Port, lease (5..300) and connection limit (1..1024).");
            webServer.Close();
            StopDatabase();
            return 1;
        }
    }

    HubAuthProxy authnetProxy, legacyProxy;
    std::string ingressError;
    if (clusterEnabled && !clusterServer.LoadAdministration(ingressError))
    {
        SF_LOG_ERROR("server.hub", "%s", ingressError.c_str());
        clusterServer.Close(); webServer.Close(); StopDatabase(); return 1;
    }
    if (!OpenAuthIngress(authnetProxy, true, clusterEnabled, ingressError) ||
        !OpenAuthIngress(legacyProxy, false, clusterEnabled, ingressError))
    {
        SF_LOG_ERROR("server.hub", "Authentication ingress startup failed: %s", ingressError.c_str());
        authnetProxy.Close(); legacyProxy.Close(); clusterServer.Close(); webServer.Close(); StopDatabase();
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
    SF_LOG_INFO("server.hub", "Hub runtime initialized. Use 'routing' for authentication ingress status.");
    SF_LOG_INFO("server.hub", "Press Ctrl-C to stop.");

    bool const consoleEnabled = sConfigMgr->GetBoolDefault("Console.Enable", true);
    auto const hubStartedAt = std::chrono::steady_clock::now();
    HubConsoleInput console;
    HubCommandHandler commandHandler(bindIp, uint16(port), processSupervisor, clusterServer, authnetProxy, legacyProxy);
    if (consoleEnabled)
        console.PrintPrompt();

    while (!StopEvent)
    {
        clusterServer.Update();
        processSupervisor.Update();
        processSupervisor.UpdateNodeRestart();
        processSupervisor.UpdateFallback();

        HubWebServiceCommand webCommand;
        while (webEnabled && webServer.PollServiceCommand(webCommand))
        {
            std::string error;
            if (!webCommand.ClusterAction.empty())
            {
                bool applied = false;
                if (HubNodeRestartActive || HubBackupMaintenance()) error = "Node operations are blocked during maintenance.";
                else if (webCommand.ClusterAction == "restart") applied = processSupervisor.CheckDataServiceStop(error) && clusterServer.RestartMap(webCommand.ServiceKey,error);
                else applied = clusterServer.SetAdministration(webCommand.ServiceKey, webCommand.ClusterAction, webCommand.Actor, error);
                webServer.CompleteControlCommand(webCommand,applied);
                webCommand.DispatchResult->set_value(applied ? "" : error);
                continue;
            }
            if (webCommand.AccountResult)
            {
                std::lock_guard<std::mutex> backupLock(HubBackupAdmission);
                if (HubBackupMaintenance())
                {
                    webCommand.AccountResult->set_value({503,"{\"error\":\"Database recovery maintenance is active.\"}"});
                    continue;
                }
                using Skyfire::Auth::AccountAdministration;
                using Skyfire::Auth::AccountAdminReply;
                auto const promise = webCommand.AccountResult;
                if (!processSupervisor.HasActiveWorld())
                {
                    // An absent lease cannot prove a remote world is offline. Until remote
                    // account dispatch exists, cluster mode must never fall back to direct writes.
                    if (clusterEnabled)
                        promise->set_value({409, "{\"error\":\"Cluster mode requires an available managed world for account changes; remote dispatch is not implemented. No direct database fallback was attempted.\"}"});
                    else
                    {
                        // Dispatch runs on the same main thread as Start/Stop: no online fallback race.
                        promise->set_value(AccountAdministration::HandleEncodedRequest(webCommand.AccountRequest));
                    }
                }
                else if (!processSupervisor.SendAccountRequest(webCommand.AccountRequest,
                    [promise](std::string const& result)
                    {
                        size_t const start = result.find("ACCOUNT ");
                        if (start == std::string::npos || result.size() < start + 12)
                            promise->set_value({503, "{\"error\":\"Invalid worldserver reply. Check account state before retrying.\"}"});
                        else
                        {
                            std::string const code = result.substr(start + 8, 3);
                            int status = code == "200" ? 200 : code == "400" ? 400 : code == "404" ? 404 : code == "409" ? 409 : 503;
                            promise->set_value({status, result.substr(start + 12)});
                        }
                    }, error))
                    promise->set_value({409, "{\"error\":\"Worldserver is busy, transitioning, or needs an update. No direct database fallback was attempted.\"}"});
                continue;
            }
            bool const accepted = webCommand.RestartAll
                ? processSupervisor.RestartNodes(error)
                : webCommand.RestartService
                ? processSupervisor.RestartDataService(webCommand.ServiceKey,error)
                : webCommand.Promote
                ? processSupervisor.PromoteWorld(webCommand.ServiceKey, error)
                : webCommand.Configure
                ? processSupervisor.SaveWorldNode(webCommand.ServiceKey, webCommand.Name, webCommand.ExecutablePath,
                    webCommand.ConfigPath, webCommand.WorkingDirectory, error)
                : !webCommand.WorldCommand.empty()
                ? processSupervisor.SendWorldCommand(webCommand.WorldCommand, error, webCommand.ServiceKey)
                : webCommand.Start
                ? processSupervisor.Start(webCommand.ServiceKey, error)
                : processSupervisor.Stop(webCommand.ServiceKey, error);
            if (webCommand.RestartAll) SF_LOG_INFO("server.hub", "Node restart requested by %s: %s",webCommand.Actor.c_str(),accepted ? "accepted" : error.c_str());
            if (webCommand.Promote) SF_LOG_INFO("server.hub", "World promotion to %s requested by %s: %s", webCommand.ServiceKey.c_str(), webCommand.Actor.c_str(), accepted ? "accepted" : error.c_str());
            webServer.CompleteControlCommand(webCommand,accepted);
            if (webCommand.DispatchResult)
                webCommand.DispatchResult->set_value(accepted ? "" : error);
            if (!accepted)
                SF_LOG_WARN("server.hub", "Web console could not %s managed service '%s': %s.",
                    webCommand.RestartAll ? "restart all" : webCommand.Promote ? "promote" : webCommand.Configure ? "configure" : !webCommand.WorldCommand.empty() ? "send command to" : webCommand.Start ? "start" : "stop", webCommand.ServiceKey.c_str(), error.c_str());
        }

        auto const liveNodes = clusterServer.Snapshot();
        processSupervisor.UpdateBackupCycle(std::none_of(liveNodes.begin(), liveNodes.end(),
            [](Skyfire::Cluster::Node const& node) { return node.Type != Skyfire::Cluster::Service::Map && node.Type != Skyfire::Cluster::Service::Chat &&
                (node.Type != Skyfire::Cluster::Service::Character || node.Load != 0); }));
        authnetProxy.Update(liveNodes);
        legacyProxy.Update(liveNodes);

        if (webEnabled)
        {
            HubWebStatusSnapshot status;
            status.RestartState = processSupervisor.NodeRestartState();
            status.RestartMessage = processSupervisor.NodeRestartMessage();
            status.RestartActive = HubNodeRestartActive;
            status.FallbackEnabled = processSupervisor.FallbackEnabled();
            status.FallbackAutomatic = processSupervisor.FallbackAutomatic();
            status.FallbackPrimary = processSupervisor.FallbackPrimary();
            status.FallbackStandby = processSupervisor.FallbackStandby();
            status.FallbackState = processSupervisor.FallbackState();
            status.FallbackMessage = processSupervisor.FallbackMessage();
            status.UptimeSeconds = uint64(std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::steady_clock::now() - hubStartedAt).count());
            for (HubManagedServiceStatus const& service : processSupervisor.GetStatuses())
            {
                HubWebManagedServiceStatus webService;
                webService.Key = service.Key;
                webService.Name = service.Name;
                webService.IsWorld = service.IsWorld;
                webService.ServiceKind = service.ServiceKind;
                webService.ClusterKey = service.ClusterKey;
                webService.ExecutablePath = service.ExecutablePath;
                webService.ConfigPath = service.ConfigPath;
                webService.WorkingDirectory = service.WorkingDirectory;
                webService.CpuBasisPoints = service.CpuBasisPoints;
                webService.UptimeSeconds = service.UptimeSeconds;
                webService.MetricsAvailable = service.MetricsAvailable;
                webService.Players = service.Players;
                webService.UpdateTimeMs = service.UpdateTimeMs;
                webService.State = HubProcessSupervisor::GetStateName(service.State);
                webService.ProcessId = service.ProcessId;
                webService.LastExitCode = service.LastExitCode;
                webService.Enabled = service.Enabled;
                webService.CanSendCommands = service.CanSendCommands;
                webService.CommandPending = service.CommandPending;
                webService.CommandResult = service.CommandResult;
                status.Services.push_back(std::move(webService));
            }
            status.ClusterNodes = clusterServer.Directory();
            status.AuthIngress = {authnetProxy.Status(), legacyProxy.Status()};
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
            if (Skyfire::Auth::AccountAdministration::IsEnabled())
                LoginDatabase.KeepAlive();
            nextDatabasePing = now + databasePingInterval;
        }

        Skyfire::SleepForMilliseconds(50);
    }

    authnetProxy.Close(); legacyProxy.Close();
    webServer.Close();
    processSupervisor.StopAll();
    clusterServer.Close();
    StopDatabase();
    SF_LOG_INFO("server.hub", "Hub server stopped.");
    return 0;
}
