/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "HubProcessSupervisor.h"
#include "HubClusterServer.h"
#include "HubBackupGuard.h"
#include "Config.h"
#include "Log.h"
#include "Platform/WorldOwnershipLock.h"
#include <algorithm>
#include <filesystem>
#include <regex>

namespace
{
    std::string Value(Config const& config, char const* key)
    {
        for (auto const& section : config)
        {
            auto entry = section.second.find(key);
            if (entry != section.second.end()) return entry->second;
        }
        return {};
    }
    bool Enabled(Config const& config, char const* key)
    {
        auto value = Value(config, key);
        return value == "1" || value == "true" || value == "TRUE" || value == "yes" || value == "YES";
    }
    struct InternalStart
    {
        bool& Flag;
        explicit InternalStart(bool& flag) : Flag(flag) { Flag = true; }
        ~InternalStart() { Flag = false; }
    };
}

void HubProcessSupervisor::ConfigureFallback()
{
    // Deliberately startup-only: reloading configuration cannot rebind an active pair.
    _fallbackEnabled = sConfigMgr->GetBoolDefault("Hub.Fallback.Enable", false);
    _fallbackAutomatic = sConfigMgr->GetBoolDefault("Hub.Fallback.Automatic", false);
    _fallbackPrimary = sConfigMgr->GetStringDefault("Hub.Fallback.Primary", "world");
    _fallbackStandby = sConfigMgr->GetStringDefault("Hub.Fallback.Standby", "world-standby");
    _fallbackCountdown = std::clamp(sConfigMgr->GetIntDefault("Hub.Fallback.Countdown", 60), 0, 3600);
    _fallbackDegradedMs = uint32(std::clamp(sConfigMgr->GetIntDefault("Hub.Fallback.DegradedUpdateMs", 0), 0, 60000));
    _fallbackDegradedSeconds = uint32(std::clamp(sConfigMgr->GetIntDefault("Hub.Fallback.DegradedSeconds", 60), 15, 3600));
    _fallbackState = _fallbackEnabled ? "idle" : "disabled";
    _fallbackMessage = _fallbackEnabled ? "Standby promotion requires confirmed source exit and exclusive local ownership." : "World fallback is disabled.";
}

bool HubProcessSupervisor::ValidateFallbackPair(std::string& error)
{
    auto reject = [&](char const* message) { error = message; return false; };
    if (!_fallbackEnabled || !_clusterServer || !_clusterServer->IsOpen() || !IsWorldKey(_fallbackPrimary) || !IsWorldKey(_fallbackStandby) ||
        _fallbackPrimary == _fallbackStandby)
        return reject("Configure an enabled fallback pair with two distinct managed world keys.");
    auto first = _services.find(_fallbackPrimary), second = _services.find(_fallbackStandby);
    if (first == _services.end() || second == _services.end() || !first->second.Definition.Enabled || !second->second.Definition.Enabled)
        return reject("Both fallback services must exist and be enabled in the hub database.");
    try
    {
        auto file = [](ManagedServiceDefinition const& definition, std::string const& path)
        {
            auto result = std::filesystem::path(path);
            if (result.is_relative()) result = std::filesystem::path(definition.WorkingDirectory) / result;
            return std::filesystem::weakly_canonical(std::filesystem::absolute(result));
        };
        auto const& a = first->second.Definition;
        auto const& b = second->second.Definition;
        auto aExe = file(a, a.ExecutablePath), bExe = file(b, b.ExecutablePath);
#ifdef _WIN32
        if (aExe.extension().empty()) aExe += ".exe";
        if (bExe.extension().empty()) bExe += ".exe";
#endif
        if (!std::filesystem::equivalent(aExe, bExe))
            return reject("Same-host fallback requires the same worldserver executable for both services.");
        Config primary, standby;
        if (!ConfigLoader::Load(file(a, a.ConfigPath).string(), primary) || !ConfigLoader::Load(file(b, b.ConfigPath).string(), standby))
            return reject("Cannot read both fallback world configurations.");
        for (char const* key : {"LoginDatabaseInfo", "WorldDatabaseInfo", "CharacterDatabaseInfo", "RealmID", "Cluster.HubHost", "Cluster.HubPort"})
            if (Value(primary, key).empty() || Value(primary, key) != Value(standby, key))
                return reject("Fallback worlds must explicitly configure identical databases, realm ID and hub endpoint.");
        for (char const* key : {"Cluster.Enable", "Cluster.Handoff.Enable"})
            if (!Enabled(primary, key) || !Enabled(standby, key))
                return reject("Both fallback worlds require cluster registration and handoffs.");
        auto const primaryNode = Value(primary, "Cluster.NodeKey"), standbyNode = Value(standby, "Cluster.NodeKey");
        if (!Skyfire::Cluster::ValidKey(primaryNode) || !Skyfire::Cluster::ValidKey(standbyNode) || primaryNode == standbyNode)
            return reject("Fallback worlds need distinct valid cluster node keys and certificates.");
        std::string const realm = Value(primary, "RealmID");
        if (!std::regex_match(realm, std::regex("[1-9][0-9]{0,8}"))) return reject("Invalid fallback RealmID.");
        std::filesystem::path aLock(Value(primary, "World.OwnershipLock")), bLock(Value(standby, "World.OwnershipLock"));
        if (!aLock.is_absolute() || !bLock.is_absolute()) return reject("Both worlds require an absolute local World.OwnershipLock path.");
        aLock = std::filesystem::weakly_canonical(aLock); bLock = std::filesystem::weakly_canonical(bLock);
        if (aLock != bLock || !std::filesystem::is_directory(aLock.parent_path()))
            return reject("Fallback worlds must use the same ownership lock file in an existing local directory.");
        for (auto const* runtime : {&first->second, &second->second})
            if (!runtime->OwnershipPath.empty() && std::filesystem::path(runtime->OwnershipPath) != aLock)
                return reject("The running world's ownership lock differs from its configuration; restore the configuration and stop it safely.");
        _fallbackLock = aLock.string();
        _fallbackRealm = uint32(std::stoul(realm));
        _fallbackPrimaryNode = primaryNode; _fallbackStandbyNode = standbyNode;
        return true;
    }
    catch (std::exception const&)
    { return reject("Fallback preflight could not resolve the executable, configurations or ownership path."); }
}

bool HubProcessSupervisor::CheckFallbackStart(std::string const& key, std::string& error)
{
    if (!_fallbackEnabled || (key != _fallbackPrimary && key != _fallbackStandby)) return true;
    if (!ValidateFallbackPair(error)) return false;
    auto const& peer = _services.at(key == _fallbackPrimary ? _fallbackStandby : _fallbackPrimary);
    if (IsActive(peer)) { error = "The other fallback world still has a live or unconfirmed process. Use Promote for a graceful switchover."; return false; }
    if (_fallbackActive && !_fallbackStarting) { error = "Fallback promotion owns this world pair."; return false; }
    // Test the real OS lock, rather than trusting a stale heartbeat or an empty process cache.
    WorldOwnershipLock probe;
    return probe.Acquire(_fallbackLock, error);
}

bool HubProcessSupervisor::PromoteWorld(std::string const& target, std::string& error)
{
    std::lock_guard<std::mutex> lock(HubBackupAdmission);
    if (_shuttingDown || _fallbackActive || HubNodeRestartActive || HubBackupMaintenance())
    { error = "Promotion is blocked by shutdown, backup/recovery maintenance or another node operation."; return false; }
    if (!ValidateFallbackPair(error)) return false;
    if (target != _fallbackPrimary && target != _fallbackStandby)
    { error = "Promotion target is not a member of the configured fallback pair."; return false; }
    std::string const source = target == _fallbackPrimary ? _fallbackStandby : _fallbackPrimary;
    auto& old = _services.at(source);
    auto& next = _services.at(target);
    if (IsActive(next)) { error = "Promotion target already has a process."; return false; }
    if (!old.EverReady || old.OwnershipPath.empty() ||
        (!IsActive(old) && old.State != HubManagedProcessState::Exited))
    { error = "Source ownership/exit has not been observed by this hub. Start a fenced world normally before using promotion."; return false; }
    if (IsActive(old) && (old.State != HubManagedProcessState::Running || !old.CanSendCommands || old.CommandPending))
    { error = "Source is not responsive to graceful commands. Promotion will not kill it or treat heartbeat loss as exit."; return false; }
    auto targetConfig = std::filesystem::path(next.Definition.ConfigPath);
    if (targetConfig.is_relative()) targetConfig = std::filesystem::path(next.Definition.WorkingDirectory) / targetConfig;
    if (!_worldStartCheck || !_worldStartCheck(targetConfig.string(), error)) return false;
    auto token = HubDatabase.Query("SELECT REPLACE(UUID(),'-','')");
    if (!token) { error = "Cannot obtain promotion maintenance ownership."; return false; }
    auto const owner = token->Fetch()[0].GetString();
    if (!std::regex_match(owner, std::regex("[0-9a-fA-F]{32}"))) { error = "Invalid maintenance owner."; return false; }
    HubDatabase.DirectExecute(("UPDATE hub_backup_worker SET maintenance=1,services_stopped=0,restart_owner='" + owner +
        "' WHERE id=1 AND maintenance=0 AND recovery_safe=1 AND NOT EXISTS(SELECT 1 FROM hub_backup_jobs WHERE state IN ('queued','running','deleting')) "
        "AND NOT EXISTS(SELECT 1 FROM hub_backup_cycles WHERE active_slot=1)").c_str());
    auto owned = HubDatabase.Query(("SELECT maintenance=1 AND restart_owner='" + owner + "' FROM hub_backup_worker WHERE id=1").c_str());
    if (!owned || !owned->Fetch()[0].GetBool())
    { error = "Cannot reserve promotion maintenance. Check backup activity and the node-restart schema."; return false; }
    _fallbackSource = source; _fallbackTarget = target; _fallbackToken = owner;
    _fallbackGraceful = IsActive(old);
    _fallbackActive = true; HubNodeRestartActive = true;
    _fallbackState = "stopping";
    _fallbackMessage = "Waiting for " + source + " to save and exit before promoting " + target + ".";
    _fallbackDeadline = std::chrono::steady_clock::now() + std::chrono::seconds(_fallbackCountdown + 300);
    old.SuppressRestart = true; old.RestartPending = false;
    if (IsActive(old))
    {
        InternalStart scope(_restartInternal);
        if (!SendWorldCommand("server shutdown " + std::to_string(_fallbackCountdown), error, source))
        { FailFallback(error); return false; }
    }
    SF_LOG_INFO("server.hub", "World promotion %s -> %s admitted.", source.c_str(), target.c_str());
    return true;
}

void HubProcessSupervisor::FailFallback(std::string const& error)
{
    _fallbackState = "failed";
    _fallbackMessage = error + " No automatic retry or failback. Review services and recovery maintenance.";
    _fallbackActive = false;
    HubNodeRestartActive = false;
    SF_LOG_ERROR("server.hub", "%s", _fallbackMessage.c_str());
}

void HubProcessSupervisor::UpdateFallback()
{
    if (_shuttingDown || !_fallbackEnabled) return;
    auto const now = std::chrono::steady_clock::now();
    if (now - _fallbackUpdated < std::chrono::seconds(1)) return;
    _fallbackUpdated = now;
    auto primary = _services.find(_fallbackPrimary);
    bool const healthyChannel = primary != _services.end() && primary->second.State == HubManagedProcessState::Running &&
        primary->second.Ready && primary->second.MetricsAvailable && !primary->second.OwnershipPath.empty() &&
        !primary->second.ExpectedExit && !primary->second.SuppressRestart && !primary->second.CommandPending &&
        now - primary->second.LastMetrics <= std::chrono::seconds(15);
    bool degraded = false;
    bool const eligible = _fallbackAutomatic && healthyChannel && !_fallbackActive && !HubNodeRestartActive;
    bool const newReport = primary != _services.end() &&
        (primary->second.LastTick != _fallbackObservedTick || primary->second.ProcessId != _fallbackObservedProcess);
    // Polling the same metric repeatedly must not turn one slow tick into sustained overload.
    if (!eligible || newReport)
    {
        if (newReport)
        {
            if (primary->second.ProcessId != _fallbackObservedProcess) _fallbackDegradation = {};
            _fallbackObservedTick = primary->second.LastTick;
            _fallbackObservedProcess = primary->second.ProcessId;
        }
        degraded = _fallbackDegradation.Observe(
            uint64(std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count()),
            eligible, primary == _services.end() ? 0 : primary->second.UpdateTimeMs, _fallbackDegradedMs, _fallbackDegradedSeconds);
    }
    if (degraded)
    {
        std::string error;
        SF_LOG_INFO("server.hub", "Sustained world tick delay triggered a graceful standby promotion request; character saves remain on the normal world shutdown path.");
        if (!PromoteWorld(_fallbackStandby, error))
        {
            _fallbackState = "failed"; _fallbackMessage = "Preventive switchover blocked: " + error;
            SF_LOG_ERROR("server.hub", "%s", _fallbackMessage.c_str());
        }
    }
    if (_fallbackCrashPending)
    {
        _fallbackCrashPending = false;
        std::string error;
        if (!PromoteWorld(_fallbackStandby, error))
        {
            // Do not clear another operation's admission flag when this promotion was not admitted.
            _fallbackState = "failed"; _fallbackMessage = "Automatic promotion blocked: " + error + " Review before retrying.";
            SF_LOG_ERROR("server.hub", "%s", _fallbackMessage.c_str());
        }
    }
    if (!_fallbackActive) return;
    if (now > _fallbackDeadline) { FailFallback("Promotion timed out; no process was force-killed."); return; }
    auto owned = HubDatabase.Query(("SELECT maintenance=1 AND recovery_safe=1 AND restart_owner='" + _fallbackToken + "' FROM hub_backup_worker WHERE id=1").c_str());
    if (!owned || !owned->Fetch()[0].GetBool()) { FailFallback("Promotion maintenance ownership was lost."); return; }
    auto const nodes = _clusterServer->Snapshot();
    std::string const targetNode = _fallbackTarget == _fallbackPrimary ? _fallbackPrimaryNode : _fallbackStandbyNode;
    if (_fallbackState == "stopping")
    {
        if (IsActive(_services.at(_fallbackSource))) return;
        if (_fallbackGraceful && _services.at(_fallbackSource).LastExitCode != 0)
        { FailFallback("Source exited abnormally during the planned switchover; verify character state before retrying."); return; }
        // Wait for the old registration lease to disappear. Never bypass a duplicate owner.
        for (auto const& node : nodes)
            if (node.Type == Skyfire::Cluster::Service::World &&
                (node.Realm == _fallbackRealm || std::find(node.Realms.begin(), node.Realms.end(), _fallbackRealm) != node.Realms.end())) return;
        std::string error;
        InternalStart maintenance(_restartInternal), promotion(_fallbackStarting);
        if (!Start(_fallbackTarget, error)) { FailFallback(error); return; }
        _fallbackState = "starting";
        _fallbackMessage = "Starting " + _fallbackTarget + "; waiting for ownership, process readiness and the live realm route.";
        _fallbackDeadline = now + std::chrono::minutes(30);
        return;
    }
    auto const& next = _services.at(_fallbackTarget);
    if (!IsActive(next)) { FailFallback("Replacement world exited before promotion completed."); return; }
    if (!next.Ready || next.State != HubManagedProcessState::Running || next.OwnershipPath != _fallbackLock) return;
    bool routeReady = false;
    for (auto const& node : nodes)
        if (node.Type == Skyfire::Cluster::Service::World &&
            (node.Realm == _fallbackRealm || std::find(node.Realms.begin(), node.Realms.end(), _fallbackRealm) != node.Realms.end()))
        {
            if (node.Key != targetNode) { FailFallback("Another world claims the promoted realm."); return; }
            routeReady = node.Ready && node.Admin == Skyfire::Cluster::Administration::Enabled;
        }
    if (!routeReady) return;
    HubDatabase.DirectExecute(("UPDATE hub_backup_worker SET maintenance=0,restart_owner='' WHERE id=1 AND recovery_safe=1 AND restart_owner='" + _fallbackToken + "'").c_str());
    if (HubBackupMaintenance()) { FailFallback("Replacement is ready but maintenance release failed."); return; }
    _fallbackState = "completed";
    _fallbackMessage = _fallbackTarget + " owns the realm. Players must reconnect; the previous world remains stopped.";
    _fallbackActive = false; HubNodeRestartActive = false;
    SF_LOG_INFO("server.hub", "%s", _fallbackMessage.c_str());
}
