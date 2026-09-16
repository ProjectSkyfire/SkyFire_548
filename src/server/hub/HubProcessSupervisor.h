/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_HUB_PROCESS_SUPERVISOR_H
#define SKYFIRE_HUB_PROCESS_SUPERVISOR_H

#include "Define.h"

#include <chrono>
#include <string>
#include <unordered_map>
#include <vector>

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
};

class HubProcessSupervisor
{
public:
    HubProcessSupervisor() = default;
    ~HubProcessSupervisor();

    HubProcessSupervisor(HubProcessSupervisor const&) = delete;
    HubProcessSupervisor& operator=(HubProcessSupervisor const&) = delete;

    bool Start(std::string const& serviceKey, std::string& error);
    bool Stop(std::string const& serviceKey, std::string& error);
    bool ReloadDatabaseRecords(std::string& error);
    void Update();
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
        std::string StatusBuffer;
        std::chrono::steady_clock::time_point StartedAt;
        std::chrono::steady_clock::time_point LastHeartbeat;
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

    std::unordered_map<std::string, ManagedServiceRuntime> _services;
};

#endif
