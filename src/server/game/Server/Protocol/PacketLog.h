/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#ifndef SKYFIRE_PACKETLOG_H
#define SKYFIRE_PACKETLOG_H

#include "Common.h"
#include "PacketLogServer.h"
#include "Platform/Singleton.h"
#include <string>

enum Direction
{
    CLIENT_TO_SERVER,
    SERVER_TO_CLIENT
};

class WorldPacket;

typedef Skyfire::PacketLogServerSessionInfo PacketLogSessionInfo;

class PacketLog
{
    friend class Skyfire::Singleton<PacketLog, Skyfire::Mutex>;

private:
    PacketLog();
    ~PacketLog();

public:
    void Initialize();
    bool CanLogPacket() const;
    void EnableGlobalLogging();
    void DisableAllLogging();
    void EnableCharacterLogging(std::string const& characterName);
    void DisableCharacterLogging(std::string const& characterName);
    bool IsGlobalLoggingEnabled() const;
    size_t GetCharacterLoggingCount() const;
    void LogPacket(WorldPacket const& packet, Direction direction);
    void LogPacket(void const* sessionKey, PacketLogSessionInfo const& sessionInfo, WorldPacket const& packet, Direction direction);
    void LogMarker(void const* sessionKey, PacketLogSessionInfo const& sessionInfo, std::string const& marker);
    void RefreshSessionInfo(void const* sessionKey, PacketLogSessionInfo const& sessionInfo);
    void CloseSession(void const* sessionKey);
};

#define sPacketLog Skyfire::Singleton<PacketLog, Skyfire::Mutex>::instance()
#endif
