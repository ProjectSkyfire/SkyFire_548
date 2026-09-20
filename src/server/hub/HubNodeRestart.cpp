/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "HubProcessSupervisor.h"
#include "HubClusterServer.h"
#include "HubBackupGuard.h"
#include "Cluster/MapDataDirectory.h"
#include "Config.h"
#include "Log.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <regex>

namespace
{
    struct RestartScope
    {
        bool& Flag;
        explicit RestartScope(bool& flag) : Flag(flag) { Flag = true; }
        ~RestartScope() { Flag = false; }
    };
}

bool HubProcessSupervisor::RestartNodes(std::string& error)
{
    std::lock_guard<std::mutex> lock(HubBackupAdmission);
    if (!_clusterServer || _clusterServer->HasPendingMapRestarts() || HubNodeRestartActive || HubBackupMaintenance())
    { error = "Restart is blocked by maintenance or another restart."; return false; }
    _restartTargets.clear(); _restartMapGenerations.clear(); _restartStarted.clear();
    std::set<std::string> identities;
    for (auto const& entry : _services)
    {
        auto const& runtime = entry.second;
        if (runtime.Definition.ServiceKind) continue; // Map refresh uses its cluster channel; persistence stays online.
        if (!IsActive(runtime)) continue;
        if (!runtime.Definition.Enabled || (runtime.State != HubManagedProcessState::Running && runtime.State != HubManagedProcessState::Standby) || runtime.CommandPending || runtime.AccountCallback ||
            (IsWorldKey(entry.first) && !runtime.WarmStandby && !runtime.CanSendCommands))
        { error = "All running services must be ready and idle before restarting."; return false; }
        std::filesystem::path working(runtime.Definition.WorkingDirectory);
        auto config = std::filesystem::path(runtime.Definition.ConfigPath);
        if (config.is_relative()) config = working / config;
        std::error_code pathError;
        config = std::filesystem::absolute(config,pathError);
        if (pathError) { error = "Cannot resolve service configuration path."; return false; }
        auto executable = std::filesystem::path(runtime.Definition.ExecutablePath);
        if (executable.is_relative()) executable = working / executable;
#ifdef _WIN32
        if (executable.extension().empty()) executable += ".exe";
#endif
        std::error_code ec;
        if (!std::filesystem::is_regular_file(config,ec) || !std::filesystem::is_regular_file(executable,ec))
        { error = "Restart preflight failed: configuration or executable is missing for '" + entry.first + "'."; return false; }
        if (IsWorldKey(entry.first) && (!_worldStartCheck || !_worldStartCheck(config.string(),error))) return false;
        std::ifstream input(config); std::string line;
        if (!input) { error = "Cannot read a managed service configuration."; return false; }
        std::regex identity("\\s*Cluster\\.NodeKey\\s*=\\s*[\"']?([A-Za-z0-9_.-]+)[\"']?\\s*(?:[#;].*)?");
        while (std::getline(input,line)) { std::smatch match; if (std::regex_match(line,match,identity)) identities.insert(match[1].str()); }
        _restartTargets.push_back(entry.first);
    }
    for (auto const& node : _clusterServer->Snapshot())
    {
        if (node.Type == Skyfire::Cluster::Service::Character) continue; // Persistence stays available throughout world restarts.
        if (node.Type != Skyfire::Cluster::Service::Map)
        {
            if (!identities.count(node.Key))
            { error = "Restart all cannot control remote/unmanaged node '" + node.Key + "'."; return false; }
            continue;
        }
        if (!node.Ready || node.Metrics.Generation.empty() || !(node.Capabilities & Skyfire::Cluster::MapData::RestartCapability))
        { error = "Mapserver '" + node.Key + "' must be ready and running the restart-capable Python script."; return false; }
        _restartMapGenerations[node.Key] = node.Metrics.Generation;
    }
    if (_restartTargets.empty() && _restartMapGenerations.empty())
    { error = "No running server nodes to restart."; return false; }
    auto token = HubDatabase.Query("SELECT REPLACE(UUID(),'-','')");
    if (!token) { error = "Cannot obtain restart ownership."; return false; }
    _nodeRestartToken = token->Fetch()[0].GetString();
    if (!std::regex_match(_nodeRestartToken,std::regex("[0-9a-fA-F]{32}"))) return false;
    // The conditional write competes atomically with scheduled backup admission.
    HubDatabase.DirectExecute(("UPDATE hub_backup_worker SET maintenance=1,services_stopped=0,restart_owner='" + _nodeRestartToken +
        "' WHERE id=1 AND maintenance=0 AND recovery_safe=1 AND NOT EXISTS(SELECT 1 FROM hub_backup_jobs WHERE state IN ('queued','running','deleting')) "
        "AND NOT EXISTS(SELECT 1 FROM hub_backup_cycles WHERE active_slot=1)").c_str());
    auto owned = HubDatabase.Query(("SELECT restart_owner='" + _nodeRestartToken + "' AND maintenance=1 FROM hub_backup_worker WHERE id=1").c_str());
    if (!owned || !owned->Fetch()[0].GetBool())
    { error = "Cannot reserve restart maintenance. Check backup activity and apply 002_node_restarts.sql."; return false; }
    HubNodeRestartActive = true;
    _nodeRestartState = "countdown";
    _nodeRestartMessage = "Sending graceful world shutdown countdown; hub stays online.";
    int const countdown = std::clamp(sConfigMgr->GetIntDefault("Hub.Restart.Countdown",60),0,3600);
    _nodeRestartDeadline = std::chrono::steady_clock::now() + std::chrono::seconds(countdown + 300);
    RestartScope scope(_restartInternal);
    for (auto const& key : _restartTargets)
        if (IsWorldKey(key) && !(_services.at(key).WarmStandby ? Stop(key, error) : SendWorldCommand("server shutdown " + std::to_string(countdown),error,key)))
        {
            _nodeRestartState = "failed"; _nodeRestartMessage = error + " Maintenance remains active; review any countdown already sent.";
            HubNodeRestartActive = false; return false;
        }
    SF_LOG_INFO("server.hub","Coordinated node restart started with %u second countdown.",unsigned(countdown));
    return true;
}

void HubProcessSupervisor::UpdateNodeRestart()
{
    if (!HubNodeRestartActive || _fallbackActive) return;
    auto tick = std::chrono::steady_clock::now();
    if (tick - _nodeRestartUpdated < std::chrono::seconds(1)) return;
    _nodeRestartUpdated = tick;
    auto fail = [&](std::string const& message)
    {
        _nodeRestartState = "failed"; _nodeRestartMessage = message + " Maintenance remains active. Review node state before releasing it.";
        HubNodeRestartActive = false;
        SF_LOG_ERROR("server.hub","%s",_nodeRestartMessage.c_str());
    };
    if (std::chrono::steady_clock::now() > _nodeRestartDeadline)
    { fail("Node restart timed out; no world process was force-killed."); return; }
    auto ownership = HubDatabase.Query(("SELECT maintenance=1 AND recovery_safe=1 AND restart_owner='" + _nodeRestartToken + "' FROM hub_backup_worker WHERE id=1").c_str());
    if (!ownership || !ownership->Fetch()[0].GetBool()) { fail("Restart maintenance ownership was lost."); return; }
    RestartScope scope(_restartInternal);
    if (_nodeRestartState == "countdown")
    {
        for (auto const& key : _restartTargets)
            if (IsWorldKey(key))
            {
                auto const& runtime = _services.at(key);
                if (IsActive(runtime)) return;
                if (runtime.LastExitCode != 0) { fail("World exited abnormally; automatic restart stopped."); return; }
            }
        for (auto const& key : _restartTargets)
            if (!IsWorldKey(key)) { std::string error; if (!Stop(key,error)) { fail(error); return; } }
        _nodeRestartState = "stopping"; _nodeRestartMessage = "Worlds saved and stopped; waiting for authentication services to stop.";
        return;
    }
    if (_nodeRestartState == "stopping")
    {
        for (auto const& key : _restartTargets) if (IsActive(_services.at(key))) return;
        for (auto const& entry : _restartMapGenerations)
        { std::string error; if (!_clusterServer->RestartMap(entry.first,error)) { fail(error); return; } }
        _nodeRestartState = "maps"; _nodeRestartMessage = "Refreshing mapserver processes and waiting for new ready registrations.";
        _nodeRestartDeadline = std::chrono::steady_clock::now() + std::chrono::minutes(30);
        return;
    }
    if (_nodeRestartState == "maps")
    {
        auto nodes = _clusterServer->Snapshot();
        auto now = uint64(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
        for (auto const& entry : _restartMapGenerations)
            if (std::none_of(nodes.begin(),nodes.end(),[&](auto const& node)
                { return node.Key == entry.first && Skyfire::Cluster::MapData::RestartReady(node,entry.second,now); })) return;
        _nodeRestartState = "auth"; _nodeRestartMessage = "Mapservers ready; starting authentication services.";
    }
    if (_nodeRestartState == "auth" || _nodeRestartState == "worlds")
    {
        bool worlds = _nodeRestartState == "worlds";
        bool ready = true;
        for (auto const& key : _restartTargets)
        {
            if (IsWorldKey(key) != worlds) continue;
            auto& runtime = _services.at(key);
            if (!_restartStarted.count(key))
            { std::string error; if (!Start(key,error)) { fail(error); return; } _restartStarted.insert(key); }
            if (!IsActive(runtime)) { fail("A restarted service exited before readiness."); return; }
            ready = ready && runtime.Ready && (runtime.State == HubManagedProcessState::Running || runtime.State == HubManagedProcessState::Standby);
        }
        if (!ready) return;
        if (!worlds)
        { _nodeRestartState = "worlds"; _nodeRestartMessage = "Starting worlds after mapserver readiness checks."; return; }
        HubDatabase.DirectExecute(("UPDATE hub_backup_worker SET maintenance=0,restart_owner='' WHERE id=1 AND recovery_safe=1 AND restart_owner='" + _nodeRestartToken + "'").c_str());
        if (HubBackupMaintenance()) { fail("All services are ready but maintenance release failed."); return; }
        _nodeRestartState = "completed"; _nodeRestartMessage = "Server nodes refreshed and ready. Hub/web remained online.";
        HubNodeRestartActive = false;
        SF_LOG_INFO("server.hub","%s",_nodeRestartMessage.c_str());
    }
}
