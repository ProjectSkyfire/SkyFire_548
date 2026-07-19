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
#include <unordered_set>
#include <vector>

enum Direction
{
    CLIENT_TO_SERVER,
    SERVER_TO_CLIENT
};

class WorldPacket;

struct PacketLogSessionInfo
{
    std::string RemoteAddress;
    uint32 AccountId;
    uint32 RealmId;
    std::string CharacterName;
};

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

private:
    struct SessionLog
    {
        FILE* file;
        uint64 sequence;
        std::string path;
        PacketLogSessionInfo info;
    };

    bool IsSessionLoggingEnabled() const;
    bool IsControlFileLoggingEnabled() const;
    bool ShouldLogSession(PacketLogSessionInfo const& sessionInfo) const;
    FILE* OpenSessionLog(void const* sessionKey, PacketLogSessionInfo const& sessionInfo);
    void CloseAllSessions();
    void CloseCharacterSessions(std::string const& normalizedName);

    FILE* _file;
    std::string _logsDir;
    std::vector<std::string> _controlFiles;
    std::string _sessionLogDir;
    bool _globalSessionLogging;
    std::unordered_set<std::string> _characterSessionLogging;
    mutable std::mutex _sessionLock;
    std::unordered_map<void const*, SessionLog> _sessionLogs;
};

#define sPacketLog Skyfire::Singleton<PacketLog, Skyfire::Mutex>::instance()
#endif
