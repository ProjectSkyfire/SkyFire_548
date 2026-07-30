/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_PACKETLOGSERVER_H
#define SKYFIRE_PACKETLOGSERVER_H

#include "Common.h"
#include "Platform/Singleton.h"
#include <cstddef>
#include <cstdio>
#include <mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace Skyfire
{
enum PacketLogServerDirection
{
    PACKET_LOG_CLIENT_TO_SERVER,
    PACKET_LOG_SERVER_TO_CLIENT
};

struct PacketLogServerSessionInfo
{
    PacketLogServerSessionInfo();

    std::string RemoteAddress;
    uint32 AccountId;
    std::string AccountName;
    uint32 RealmId;
    std::string CharacterName;
    std::string SessionName;
    std::string FilePrefix;
};

class PacketLogServer
{
    friend class Singleton<PacketLogServer, Mutex>;

private:
    PacketLogServer();
    ~PacketLogServer();

public:
    void Initialize(std::string const& controlFileDefault, std::string const& outputDirDefault);
    void Configure(std::string const& controlFile, std::string const& outputDir, std::string logsDir = "");
    void ConfigureLegacyPacketLogFile(std::string const& packetLogFile);

    bool CanLogPacket() const;
    void EnableGlobalLogging();
    void DisableAllLogging();
    void EnableCharacterLogging(std::string const& characterName);
    void DisableCharacterLogging(std::string const& characterName);
    bool IsGlobalLoggingEnabled() const;
    size_t GetCharacterLoggingCount() const;

    void LogPacket(void const* sessionKey, PacketLogServerSessionInfo const& sessionInfo,
        PacketLogServerDirection direction, uint32 opcode, std::string const& opcodeName,
        void const* payload, size_t payloadSize);
    void LogBinaryPacket(PacketLogServerDirection direction, uint32 opcode, void const* payload, size_t payloadSize);
    void LogMarker(void const* sessionKey, PacketLogServerSessionInfo const& sessionInfo, std::string const& marker);
    void RefreshSessionInfo(void const* sessionKey, PacketLogServerSessionInfo const& sessionInfo);
    void CloseSession(void const* sessionKey);

private:
    struct SessionLog
    {
        FILE* file;
        uint64 sequence;
        std::string path;
        PacketLogServerSessionInfo info;
    };

    bool IsControlFileLoggingEnabled() const;
    bool ShouldLogSession(PacketLogServerSessionInfo const& sessionInfo) const;
    FILE* OpenSessionLog(void const* sessionKey, PacketLogServerSessionInfo const& sessionInfo);
    void CloseAllSessions();
    void CloseCharacterSessions(std::string const& normalizedName);

    std::string _logsDir;
    FILE* _binaryFile;
    std::vector<std::string> _controlFiles;
    std::string _sessionLogDir;
    bool _globalSessionLogging;
    std::unordered_set<std::string> _characterSessionLogging;
    mutable std::mutex _sessionLock;
    std::unordered_map<void const*, SessionLog> _sessionLogs;
};
}

#define sPacketLogServer Skyfire::Singleton<Skyfire::PacketLogServer, Skyfire::Mutex>::instance()

#endif
