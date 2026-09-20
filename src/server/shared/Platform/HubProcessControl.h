/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_HUB_PROCESS_CONTROL_H
#define SKYFIRE_HUB_PROCESS_CONTROL_H

#include "Define.h"

#include <string>
#include <chrono>
#include <mutex>
#include <vector>

namespace Skyfire::HubControl
{
    char constexpr LaunchToken[] = "SKYFIRE-HUB-SERVICE-1\n";
    char constexpr StartingMessage[] = "STARTING";
    char constexpr ReadyMessage[] = "READY";
    char constexpr WorldReadyMessage[] = "READY_WORLD_COMMANDS_1";
    char constexpr AccountReadyMessage[] = "READY_WORLD_ACCOUNTS_1";
    constexpr size_t MaxCommandLength = 1024;
    constexpr int WorldRestartExitCode = 2;
    char constexpr HeartbeatMessage[] = "HEARTBEAT";
    char constexpr StoppingMessage[] = "STOPPING";
    char constexpr StopCommand[] = "STOP\n";
    char constexpr StandbyMessage[] = "STANDBY_READY_1";
    char constexpr ActivateCommand[] = "ACTIVATE_STANDBY_1\n";

    class ChildChannel
    {
    public:
        ChildChannel();
        ~ChildChannel();

        ChildChannel(ChildChannel const&) = delete;
        ChildChannel& operator=(ChildChannel const&) = delete;

        bool Initialize(uint64 controlReadHandle, uint64 statusWriteHandle, std::string& error);
        bool SendStatus(char const* status) const;
        // Percentage of total logical CPU capacity, in hundredths; -1 until sampled.
        int32 SampleCpuUsage();
        bool StopRequested(std::vector<std::string>* commands = nullptr, std::vector<std::string>* accounts = nullptr,
            bool* activateStandby = nullptr);
        void AppendCommandOutput(char const* text);
        void FinishCommand(bool success);
        void Close();

    private:
        std::chrono::steady_clock::time_point _cpuSampleTime;
        uint64 _cpuSampleNs = 0;
        uint64 _controlReadHandle;
        uint64 _statusWriteHandle;
        std::string _controlBuffer;
        std::string _commandOutput; // Accessed only by the world command queue.
        mutable std::mutex _statusMutex;
    };
}

#endif
