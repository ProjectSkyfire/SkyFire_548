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

enum class HubManagedProcessState
{
    Stopped,
    Starting,
    Running,
    Unresponsive,
    Stopping,
    Exited
};

class HubProcessSupervisor
{
public:
    HubProcessSupervisor();
    ~HubProcessSupervisor();

    HubProcessSupervisor(HubProcessSupervisor const&) = delete;
    HubProcessSupervisor& operator=(HubProcessSupervisor const&) = delete;

    bool Start(std::string const& serviceKey, std::string& error);
    bool ReloadDatabaseRecords(std::string& error);
    void Update();
    void Stop();

    HubManagedProcessState GetState() const { return _state; }
    char const* GetStateName() const;
    std::string const& GetServiceKey() const { return _serviceKey; }
    uint64 GetProcessId() const { return _processId; }
    int64 GetLastExitCode() const { return _lastExitCode; }

private:
    struct ManagedServiceDefinition
    {
        std::string Name;
        std::string ExecutablePath;
        std::string ConfigPath;
        std::string WorkingDirectory;
        bool Enabled = false;
    };

    bool IsActive() const;
    bool Launch(std::string const& executablePath, std::string const& configPath,
        std::string const& workingDirectory, std::string& error);
    bool WriteControl(char const* message);
    void ReadStatusMessages();
    void ProcessStatusMessage(std::string const& message);
    void MarkExited(int64 exitCode);
    void CloseHandles();

    HubManagedProcessState _state;
    std::string _serviceKey;
    uint64 _processId;
    uint64 _processHandle;
    uint64 _controlWriteHandle;
    uint64 _statusReadHandle;
    int64 _lastExitCode;
    bool _ready;
    std::string _statusBuffer;
    std::chrono::steady_clock::time_point _startedAt;
    std::chrono::steady_clock::time_point _lastHeartbeat;
    std::unordered_map<std::string, ManagedServiceDefinition> _services;
};

#endif
