/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "HubProcessSupervisor.h"
#include "HubBackupGuard.h"
#include "Log.h"
#include <algorithm>
#include <sstream>

namespace
{
    // Encode data rather than interpolating SQL syntax, including database-provided keys.
    std::string SqlText(std::string const& value)
    {
        static char const hex[] = "0123456789abcdef";
        std::string out = "CONVERT(X'";
        for (unsigned char c : value) { out += hex[c >> 4]; out += hex[c & 15]; }
        return out + "' USING utf8mb4)";
    }
    struct FlagScope
    {
        bool& Flag;
        explicit FlagScope(bool& flag) : Flag(flag) { Flag = true; }
        ~FlagScope() { Flag = false; }
    };
}

void HubProcessSupervisor::UpdateBackupCycle(bool clusterIdle)
{
    auto const tick = std::chrono::steady_clock::now();
    if (tick - _backupUpdateAt < std::chrono::seconds(1)) return;
    _backupUpdateAt = tick;
    bool const localIdle = std::none_of(_services.begin(),_services.end(),[](auto const& entry) { return !entry.second.Definition.ServiceKind && IsActive(entry.second); });
    // Keep worker health valid even when the web listener is disabled.
    auto health = HubDatabase.GetPreparedStatement(HUB_UPD_BACKUP_HUB_HEALTH);
    health->setUInt8(0,localIdle && clusterIdle ? 1 : 0); HubDatabase.DirectExecute(health);
    auto result = HubDatabase.Query(HubDatabase.GetPreparedStatement(HUB_SEL_BACKUP_CYCLE));
    if (!result) return;
    auto row = result->Fetch();
    std::string const id = row[0].GetString(), state = row[1].GetString(), owner = row[5].GetString();
    uint32 const warning = row[2].GetUInt32();
    uint64 const scheduled = row[3].GetUInt64(), stopAt = row[4].GetUInt64(), now = row[7].GetUInt64();
    std::string const where = " WHERE id=" + SqlText(id) + " AND state=" + SqlText(state);
    auto execute = [](std::string const& sql) { HubDatabase.DirectExecute(sql.c_str()); };
    auto transition = [&](char const* next, std::string const& message)
    {
        execute("UPDATE hub_backup_cycles SET state=" + SqlText(next) + ",message=" + SqlText(message) + where);
        execute("INSERT IGNORE INTO hub_control_audit(request_id,phase,actor,action,target,outcome) VALUES(" +
            SqlText(id) + ',' + SqlText(next) + ",'hub','backup.cycle','set'," + SqlText(next) + ')');
    };
    auto fail = [&](std::string const& message)
    {
        transition("failed", message);
        // Maintenance remains persisted; no force-kill or automatic launch on ambiguous state.
        SF_LOG_ERROR("server.hub", "Scheduled backup set failed: %s", message.c_str());
    };
    if (_backupBoot.empty())
    {
        auto boot = HubDatabase.Query("SELECT REPLACE(UUID(),'-','')");
        if (!boot) return;
        _backupBoot = boot->Fetch()[0].GetString();
    }
    if (state != "requested" && (owner != _backupBoot || _backupCycle != id))
    {
        fail("Hub restarted during backup maintenance. Review processes and pending countdowns; stop services before releasing maintenance.");
        return;
    }
    auto worker = HubDatabase.Query(HubDatabase.GetPreparedStatement(HUB_SEL_BACKUP_WORKER));
    if (!worker || !worker->Fetch()[4].GetBool() || !worker->Fetch()[5].GetBool())
    {
        fail("Backup maintenance or recovery safety changed. Automatic restart blocked."); return;
    }
    if (worker->Fetch()[0].GetUInt64() == 0)
    {
        fail("Backup worker went offline. Review pending shutdowns and jobs before ending maintenance."); return;
    }
    if (state == "requested")
    {
        if (warning < 60 || warning > 86400) { fail("Invalid backup shutdown warning."); return; }
        std::string services;
        bool hasWorld = false;
        for (auto const& entry : _services)
        {
            auto const& runtime = entry.second;
            if (runtime.Definition.ServiceKind) continue; // Dependencies remain available while world saves finish.
            if (!IsActive(runtime)) continue;
            if ((runtime.State != HubManagedProcessState::Running && runtime.State != HubManagedProcessState::Standby) || runtime.CommandPending || runtime.AccountCallback ||
                (IsWorldKey(entry.first) && !runtime.WarmStandby && !runtime.CanSendCommands))
            { fail("A managed service is not ready for a graceful backup countdown; no backup was started."); return; }
            if (!services.empty()) services += ',';
            services += entry.first;
            hasWorld = hasWorld || (IsWorldKey(entry.first) && !runtime.WarmStandby);
        }
        // Commit the restart set before sending any shutdown command.
        uint64 const deadline = std::max(scheduled, now + (hasWorld ? warning : 0));
        execute("UPDATE hub_backup_cycles SET state='countdown',hub_owner=" + SqlText(_backupBoot) +
            ",services=" + SqlText(services) + ",stop_at=" + std::to_string(deadline) +
            ",message='Player shutdown countdown; waiting for graceful world exit'" + where);
        _backupCycle = id; _backupSent.clear(); _backupAcknowledged.clear(); _backupStarted.clear();
        return;
    }
    std::vector<std::string> services;
    std::istringstream input(row[6].GetString());
    for (std::string key; std::getline(input,key,',');)
    {
        if (_services.find(key) == _services.end()) { fail("A service in the persisted restart set is missing. Review launch configuration."); return; }
        services.push_back(key);
    }
    if (state == "countdown" || state == "stopping")
    {
        bool worldsStopped = true;
        for (auto const& key : services)
        {
            if (!IsWorldKey(key)) continue;
            auto& runtime = _services.at(key);
            runtime.BackupControlled = true; runtime.SuppressRestart = true;
            if (runtime.WarmStandby)
            {
                if (!_backupSent.count(key))
                {
                    std::string error; FlagScope commandScope(_backupInternalCommand);
                    if (!Stop(key, error)) { fail("Could not stop the prepared standby for backup."); return; }
                    _backupSent.insert(key);
                }
                if (IsActive(runtime)) worldsStopped = false;
                else if (runtime.LastExitCode != 0) { fail("Standby exited abnormally before backup."); return; }
                continue;
            }
            if (!_backupSent.count(key))
            {
                if (state != "countdown" || stopAt <= now || runtime.State != HubManagedProcessState::Running)
                { fail("World changed state before its backup countdown could be sent."); return; }
                std::string error;
                FlagScope commandScope(_backupInternalCommand);
                if (!SendWorldCommand("server shutdown " + std::to_string(warning),error,key))
                { fail("Could not send a world shutdown countdown. Review world console; maintenance remains active."); return; }
                _backupSent.insert(key);
                execute("UPDATE hub_backup_cycles SET stop_at=GREATEST(stop_at," + std::to_string(now+warning) + ')' + where);
            }
            if (!_backupAcknowledged.count(key) && !runtime.CommandPending)
            {
                if (runtime.CommandResult.compare(0,3,"OK ") != 0)
                { fail("World rejected the backup shutdown countdown. Review world console."); return; }
                _backupAcknowledged.insert(key);
            }
            if (IsActive(runtime)) worldsStopped = false;
            else if (runtime.LastExitCode != 0 || !_backupAcknowledged.count(key))
            { fail("World did not confirm a clean shutdown; backup and restart blocked."); return; }
        }
        if (now > stopAt + 600)
        { fail("Graceful shutdown timed out or a cluster node is still live. No server was force-killed."); return; }
        if (now < stopAt || !worldsStopped) return;
        bool stopped = true;
        for (auto const& key : services)
        {
            auto& runtime = _services.at(key);
            if (IsActive(runtime))
            {
                stopped = false;
                if (!IsWorldKey(key) && runtime.State != HubManagedProcessState::Stopping)
                {
                    runtime.BackupControlled = true;
                    std::string error; FlagScope commandScope(_backupInternalCommand);
                    if (!Stop(key,error)) { fail("Could not stop an authentication service gracefully."); return; }
                }
            }
            else if (runtime.LastExitCode != 0) { fail("A managed service exited abnormally; backup blocked."); return; }
        }
        if (!stopped || !clusterIdle)
        {
            if (state == "countdown") transition("stopping","Worlds saved; waiting for auth services and cluster leases to stop");
            return;
        }
        if (!localIdle) { fail("A service outside the recorded restart set is running; backup blocked."); return; }
        transition("backup","All services stopped cleanly; creating the scheduled backup set");
        return;
    }
    if (state == "backup")
    {
        if (!localIdle || !clusterIdle) fail("A server became active during backup maintenance. Automatic restart blocked.");
        return;
    }
    if (state == "restarting")
    {
        bool ready = true;
        // Only the recorded running services are restarted; intentionally stopped nodes stay off.
        for (auto const& key : services)
        {
            auto& runtime = _services.at(key);
            if (!_backupStarted.count(key))
            {
                if (IsActive(runtime)) { fail("A service was started outside backup coordination; review restart state."); return; }
                std::string error; FlagScope launchScope(_backupLaunching);
                if (!Start(key,error)) { fail("Could not restart a previously running service. Review hub log and launch configuration."); return; }
                _backupStarted.insert(key);
            }
            if (runtime.State == HubManagedProcessState::Unresponsive || !IsActive(runtime))
            { fail("Restarted service '" + key + "' failed its readiness check (" + GetStateName(runtime.State) + "). Maintenance remains active."); return; }
            ready = ready && (runtime.State == HubManagedProcessState::Running || runtime.State == HubManagedProcessState::Standby);
        }
        if (!ready) return;
        // Complete and release in one database transaction after the full restart set is ready.
        auto complete = HubDatabase.BeginTransaction();
        complete->Append(("UPDATE hub_backup_cycles SET state='completed',message='Backup set complete; previously running services restarted'" + where).c_str());
        complete->Append(("UPDATE hub_backup_worker SET maintenance=0 WHERE id=1 AND recovery_safe=1 AND EXISTS(SELECT 1 FROM hub_backup_cycles WHERE id=" + SqlText(id) + " AND state='completed')").c_str());
        if (!HubDatabase.DirectCommitTransactionWithResult(complete)) return;
        execute("INSERT IGNORE INTO hub_control_audit(request_id,phase,actor,action,target,outcome) VALUES(" + SqlText(id) + ",'completed','hub','backup.cycle','set','completed')");
    }
}
