/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#ifndef SKYFIRE_PACKETLOG_H
#define SKYFIRE_PACKETLOG_H

#include "Common.h"
#include "Platform/Singleton.h"
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

enum Direction
{
    CLIENT_TO_SERVER,
    SERVER_TO_CLIENT
};

class WorldPacket;

class PacketLog
{
    friend class Skyfire::Singleton<PacketLog, Skyfire::Mutex>;

private:
    PacketLog();
    ~PacketLog();

public:
    void Initialize();
    bool CanLogPacket() const;
    void LogPacket(WorldPacket const& packet, Direction direction);
    void LogPacket(void const* sessionKey, std::string const& remoteAddress, WorldPacket const& packet, Direction direction);
    void LogMarker(void const* sessionKey, std::string const& remoteAddress, std::string const& marker);
    void CloseSession(void const* sessionKey);

private:
    struct SessionLog
    {
        FILE* file;
        uint64 sequence;
    };

    bool IsSessionLoggingEnabled() const;
    FILE* OpenSessionLog(void const* sessionKey, std::string const& remoteAddress);

    FILE* _file;
    std::string _logsDir;
    std::vector<std::string> _controlFiles;
    std::string _sessionLogDir;
    mutable std::mutex _sessionLock;
    std::unordered_map<void const*, SessionLog> _sessionLogs;
};

#define sPacketLog Skyfire::Singleton<PacketLog, Skyfire::Mutex>::instance()
#endif
