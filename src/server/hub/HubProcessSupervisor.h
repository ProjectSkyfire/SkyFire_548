/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_HUB_PROCESS_SUPERVISOR_H
#define SKYFIRE_HUB_PROCESS_SUPERVISOR_H

#include "Define.h"

#include <chrono>
#include <functional>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>

enum class HubManagedProcessState
{
    Stopped,
    Starting,
    Running,
    Unresponsive,
    Stopping,
    Exited
};

struct HubManagedServiceStatus
{
    std::string Key;
    std::string Name;
    HubManagedProcessState State = HubManagedProcessState::Stopped;
    uint64 ProcessId = 0;
    int64 LastExitCode = 0;
    bool Enabled = false;
    bool CommandPending = false;
    bool CanSendCommands = false;
    std::string CommandResult;
    std::string ExecutablePath, ConfigPath, WorkingDirectory;
    bool IsWorld = false;
    uint64 UptimeSeconds = 0;
    bool MetricsAvailable = false;
    uint32 Players = 0;
    uint32 UpdateTimeMs = 0;
    int32 CpuBasisPoints = -1;
};

class HubProcessSupervisor
{
public:
    HubProcessSupervisor() = default;
    ~HubProcessSupervisor();

    HubProcessSupervisor(HubProcessSupervisor const&) = delete;
    HubProcessSupervisor& operator=(HubProcessSupervisor const&) = delete;

    void SetWorldStartCheck(std::function<bool(std::string const&, std::string&)> check) { _worldStartCheck = std::move(check); }
    bool Start(std::string const& serviceKey, std::string& error);
    bool Stop(std::string const& serviceKey, std::string& error);
    static bool IsWorldKey(std::string const& key);
    bool HasActiveWorld() const;
    bool SaveWorldNode(std::string const& key, std::string const& name, std::string const& executable,
        std::string const& config, std::string const& directory, std::string& error);
    bool SendWorldCommand(std::string command, std::string& error, std::string const& key = "world");
    bool SendAccountRequest(std::string const& request, std::function<void(std::string const&)> callback, std::string& error);
    bool ReloadDatabaseRecords(std::string& error);
    void Update();
    void UpdateBackupCycle(bool clusterIdle);
    void StopAll();

    HubManagedServiceStatus GetStatus(std::string const& serviceKey) const;
    std::vector<HubManagedServiceStatus> GetStatuses() const;
    static char const* GetStateName(HubManagedProcessState state);

private:
    struct ManagedServiceDefinition
    {
        std::string Name;
        std::string ExecutablePath;
        std::string ConfigPath;
        std::string WorkingDirectory;
        bool Enabled = false;
    };

    struct ManagedServiceRuntime
    {
        ManagedServiceDefinition Definition;
        HubManagedProcessState State = HubManagedProcessState::Stopped;
        uint64 ProcessId = 0;
        uint64 ProcessHandle = 0;
        uint64 ControlWriteHandle = 0;
        uint64 StatusReadHandle = 0;
        int64 LastExitCode = 0;
        bool Ready = false;
        bool CanSendCommands = false;
        bool CanManageAccounts = false;
        std::function<void(std::string const&)> AccountCallback;
        bool CommandPending = false;
        bool RestartPending = false;
        bool SuppressRestart = false;
        bool BackupControlled = false;
        std::string CommandResult;
        bool MetricsAvailable = false;
        uint32 Players = 0;
        uint32 UpdateTimeMs = 0;
        int32 CpuBasisPoints = -1;

        std::string StatusBuffer;
        std::chrono::steady_clock::time_point StartedAt;
        std::chrono::steady_clock::time_point LastHeartbeat;
        std::chrono::steady_clock::time_point LastMetrics;
        uint64 LastTick = 0;
        std::chrono::steady_clock::time_point StopRequestedAt;
    };

    static bool IsActive(ManagedServiceRuntime const& runtime);
    bool Launch(std::string const& serviceKey, ManagedServiceRuntime& runtime,
        std::string const& executablePath, std::string const& configPath,
        std::string const& workingDirectory, std::string& error);
    static bool WriteControl(ManagedServiceRuntime const& runtime, char const* message);
    void Update(std::string const& serviceKey, ManagedServiceRuntime& runtime);
    void ReadStatusMessages(std::string const& serviceKey, ManagedServiceRuntime& runtime);
    void ProcessStatusMessage(std::string const& serviceKey, ManagedServiceRuntime& runtime,
        std::string const& message);
    void MarkExited(std::string const& serviceKey, ManagedServiceRuntime& runtime, int64 exitCode);
    static void CloseHandles(ManagedServiceRuntime& runtime);
    void ForceStop(std::string const& serviceKey, ManagedServiceRuntime& runtime);

    std::function<bool(std::string const&, std::string&)> _worldStartCheck;
    std::unordered_map<std::string, ManagedServiceRuntime> _services;
    bool _shuttingDown = false;
    bool _backupInternalCommand = false;
    bool _backupLaunching = false;
    std::string _backupBoot, _backupCycle;
    std::set<std::string> _backupSent, _backupAcknowledged, _backupStarted;
    std::chrono::steady_clock::time_point _backupUpdateAt;
};

#endif
