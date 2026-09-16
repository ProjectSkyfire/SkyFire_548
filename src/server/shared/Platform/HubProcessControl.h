/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_HUB_PROCESS_CONTROL_H
#define SKYFIRE_HUB_PROCESS_CONTROL_H

#include "Define.h"

#include <string>
#include <mutex>
#include <vector>

namespace Skyfire::HubControl
{
    char constexpr LaunchToken[] = "SKYFIRE-HUB-SERVICE-1\n";
    char constexpr StartingMessage[] = "STARTING";
    char constexpr ReadyMessage[] = "READY";
    char constexpr WorldReadyMessage[] = "READY_WORLD_COMMANDS_1";
    constexpr size_t MaxCommandLength = 1024;
    constexpr int WorldRestartExitCode = 2;
    char constexpr HeartbeatMessage[] = "HEARTBEAT";
    char constexpr StoppingMessage[] = "STOPPING";
    char constexpr StopCommand[] = "STOP\n";

    class ChildChannel
    {
    public:
        ChildChannel();
        ~ChildChannel();

        ChildChannel(ChildChannel const&) = delete;
        ChildChannel& operator=(ChildChannel const&) = delete;

        bool Initialize(uint64 controlReadHandle, uint64 statusWriteHandle, std::string& error);
        bool SendStatus(char const* status) const;
        bool StopRequested(std::vector<std::string>* commands = nullptr);
        void AppendCommandOutput(char const* text);
        void FinishCommand(bool success);
        void Close();

    private:
        uint64 _controlReadHandle;
        uint64 _statusWriteHandle;
        std::string _controlBuffer;
        std::string _commandOutput; // Accessed only by the world command queue.
        mutable std::mutex _statusMutex;
    };
}

#endif
