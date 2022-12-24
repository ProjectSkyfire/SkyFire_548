/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/
#ifndef SKYFIRE_PACKETLOG_H
#define SKYFIRE_PACKETLOG_H

#include "Common.h"
#include <ace/Singleton.h>

enum Direction
{
    CLIENT_TO_SERVER,
    SERVER_TO_CLIENT
};

class WorldPacket;

class PacketLog
{
    friend class ACE_Singleton<PacketLog, ACE_Thread_Mutex>;

    private:
        PacketLog();
        ~PacketLog();

    public:
        void Initialize();
        bool CanLogPacket() const { return (_file != NULL); }
        void LogPacket(WorldPacket const& packet, Direction direction);

    private:
        FILE* _file;
};

#define sPacketLog ACE_Singleton<PacketLog, ACE_Thread_Mutex>::instance()
#endif
