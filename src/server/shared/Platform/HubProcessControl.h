/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_HUB_PROCESS_CONTROL_H
#define SKYFIRE_HUB_PROCESS_CONTROL_H

#include "Define.h"

#include <string>

namespace Skyfire::HubControl
{
    char constexpr LaunchToken[] = "SKYFIRE-HUB-AUTHNET-1\n";
    char constexpr StartingMessage[] = "STARTING";
    char constexpr ReadyMessage[] = "READY";
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
        bool StopRequested();
        void Close();

    private:
        uint64 _controlReadHandle;
        uint64 _statusWriteHandle;
        std::string _controlBuffer;
    };
}

#endif
