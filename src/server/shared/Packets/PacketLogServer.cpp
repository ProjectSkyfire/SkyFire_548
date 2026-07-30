/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Config.h"
#include "ByteBuffer.h"
#include "PacketLogServer.h"
#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <filesystem>
#include <iomanip>
#include <sstream>

namespace
{
bool IsAbsolutePath(std::string const& path)
{
    if (path.empty())
        return false;

    if (path[0] == '/' || path[0] == '\\')
        return true;

    return path.size() > 2 && std::isalpha(static_cast<unsigned char>(path[0])) && path[1] == ':';
}

std::string JoinPath(std::string base, std::string const& path)
{
    if (path.empty() || IsAbsolutePath(path) || base.empty())
        return path;

    if (base.back() != '/' && base.back() != '\\')
        base.push_back('/');

    return base + path;
}

std::string SanitizePathPart(std::string value)
{
    for (char& ch : value)
        if (!std::isalnum(static_cast<unsigned char>(ch)) && ch != '.' && ch != '-' && ch != '_')
            ch = '_';

    if (value.empty())
        value = "unknown";

    return value;
}

std::string SanitizeMarker(std::string marker)
{
    for (char& ch : marker)
        if (ch == '\r' || ch == '\n')
            ch = ' ';

    return marker;
}

std::string NormalizeFilterName(std::string value)
{
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch)
    {
        return static_cast<char>(std::tolower(ch));
    });

    return value;
}

std::string GetAccountPathPart(Skyfire::PacketLogServerSessionInfo const& sessionInfo)
{
    if (!sessionInfo.AccountName.empty())
        return sessionInfo.AccountName;

    return std::to_string(sessionInfo.AccountId);
}

std::string GetSessionPathPart(Skyfire::PacketLogServerSessionInfo const& sessionInfo)
{
    if (!sessionInfo.CharacterName.empty())
        return sessionInfo.CharacterName;

    if (!sessionInfo.SessionName.empty())
        return sessionInfo.SessionName;

    return "none";
}

std::string GetFilePrefix(Skyfire::PacketLogServerSessionInfo const& sessionInfo)
{
    return sessionInfo.FilePrefix.empty() ? "session" : sessionInfo.FilePrefix;
}

std::string GetFilterName(Skyfire::PacketLogServerSessionInfo const& sessionInfo)
{
    if (!sessionInfo.CharacterName.empty())
        return sessionInfo.CharacterName;

    return sessionInfo.SessionName;
}

std::string BuildSessionIdentity(Skyfire::PacketLogServerSessionInfo const& sessionInfo)
{
    std::ostringstream identity;
    identity << GetFilePrefix(sessionInfo)
        << '|'
        << sessionInfo.RemoteAddress
        << '|'
        << GetAccountPathPart(sessionInfo)
        << '|'
        << sessionInfo.RealmId
        << '|'
        << GetSessionPathPart(sessionInfo);

    return identity.str();
}

std::filesystem::path BuildSessionLogPath(std::string const& sessionLogDir, void const* sessionKey,
    Skyfire::PacketLogServerSessionInfo const& sessionInfo)
{
    auto now = std::chrono::system_clock::now();
    std::time_t timestamp = std::chrono::system_clock::to_time_t(now);
    std::tm localTime;
#if PLATFORM == PLATFORM_WINDOWS
    localtime_s(&localTime, &timestamp);
#else
    localtime_r(&timestamp, &localTime);
#endif

    std::ostringstream filename;
    filename << GetFilePrefix(sessionInfo)
        << "_"
        << std::put_time(&localTime, "%Y%m%d_%H%M%S")
        << "_"
        << SanitizePathPart(sessionInfo.RemoteAddress)
        << "_account"
        << SanitizePathPart(GetAccountPathPart(sessionInfo))
        << "_realm"
        << sessionInfo.RealmId
        << "_"
        << SanitizePathPart(GetSessionPathPart(sessionInfo))
        << "_"
        << reinterpret_cast<uintptr_t>(sessionKey)
        << ".pktlog";

    return std::filesystem::path(sessionLogDir) / filename.str();
}
}

namespace Skyfire
{
PacketLogServerSessionInfo::PacketLogServerSessionInfo() :
    RemoteAddress(), AccountId(0), AccountName(), RealmId(0), CharacterName(), SessionName(), FilePrefix()
{
}

PacketLogServer::PacketLogServer() : _logsDir(), _binaryFile(NULL), _controlFiles(), _sessionLogDir(),
    _globalSessionLogging(false), _characterSessionLogging(), _sessionLogs()
{
}

PacketLogServer::~PacketLogServer()
{
    std::lock_guard<std::mutex> guard(_sessionLock);
    CloseAllSessions();

    if (_binaryFile)
        fclose(_binaryFile);

    _binaryFile = NULL;
}

void PacketLogServer::Initialize(std::string const& controlFileDefault, std::string const& outputDirDefault)
{
    std::string logsDir = sConfigMgr->GetStringDefault("LogsDir", "");
    Configure(
        sConfigMgr->GetStringDefault("PacketLogServerControlFile", controlFileDefault),
        sConfigMgr->GetStringDefault("PacketLogServerOutputDir", outputDirDefault),
        logsDir);
    ConfigureLegacyPacketLogFile(sConfigMgr->GetStringDefault("PacketLogFile", ""));
}

void PacketLogServer::Configure(std::string const& controlFile, std::string const& outputDir, std::string logsDir)
{
    if (!logsDir.empty())
        if ((logsDir.at(logsDir.length() - 1) != '/') && (logsDir.at(logsDir.length() - 1) != '\\'))
            logsDir.push_back('/');

    std::lock_guard<std::mutex> guard(_sessionLock);
    _logsDir = logsDir;
    _controlFiles.clear();
    _sessionLogDir = outputDir.empty() ? "PacketLogs" : outputDir;
    _globalSessionLogging = false;
    _characterSessionLogging.clear();
    CloseAllSessions();

    if (!controlFile.empty())
    {
        _controlFiles.push_back(controlFile);

        std::string legacyControlFile = JoinPath(_logsDir, controlFile);
        if (legacyControlFile != controlFile)
            _controlFiles.push_back(legacyControlFile);
    }
}

void PacketLogServer::ConfigureLegacyPacketLogFile(std::string const& packetLogFile)
{
    std::lock_guard<std::mutex> guard(_sessionLock);

    if (_binaryFile)
    {
        fclose(_binaryFile);
        _binaryFile = NULL;
    }

    if (!packetLogFile.empty())
        _binaryFile = fopen(JoinPath(_logsDir, packetLogFile).c_str(), "wb");
}

bool PacketLogServer::CanLogPacket() const
{
    std::lock_guard<std::mutex> guard(_sessionLock);
    return _binaryFile || _globalSessionLogging || !_characterSessionLogging.empty() || IsControlFileLoggingEnabled();
}

bool PacketLogServer::IsControlFileLoggingEnabled() const
{
    for (std::string const& controlFile : _controlFiles)
    {
        std::error_code error;
        if (!controlFile.empty() && std::filesystem::exists(controlFile, error) && !error)
            return true;
    }

    return false;
}

void PacketLogServer::EnableGlobalLogging()
{
    std::lock_guard<std::mutex> guard(_sessionLock);
    _globalSessionLogging = true;
}

void PacketLogServer::DisableAllLogging()
{
    std::lock_guard<std::mutex> guard(_sessionLock);
    _globalSessionLogging = false;
    _characterSessionLogging.clear();

    for (std::string const& controlFile : _controlFiles)
    {
        std::error_code error;
        std::filesystem::remove(controlFile, error);
    }

    CloseAllSessions();
}

void PacketLogServer::EnableCharacterLogging(std::string const& characterName)
{
    std::lock_guard<std::mutex> guard(_sessionLock);
    _characterSessionLogging.insert(NormalizeFilterName(characterName));
}

void PacketLogServer::DisableCharacterLogging(std::string const& characterName)
{
    std::lock_guard<std::mutex> guard(_sessionLock);
    std::string normalizedName = NormalizeFilterName(characterName);
    _characterSessionLogging.erase(normalizedName);

    if (!_globalSessionLogging && !IsControlFileLoggingEnabled())
        CloseCharacterSessions(normalizedName);
}

bool PacketLogServer::IsGlobalLoggingEnabled() const
{
    std::lock_guard<std::mutex> guard(_sessionLock);
    return _globalSessionLogging;
}

size_t PacketLogServer::GetCharacterLoggingCount() const
{
    std::lock_guard<std::mutex> guard(_sessionLock);
    return _characterSessionLogging.size();
}

bool PacketLogServer::ShouldLogSession(PacketLogServerSessionInfo const& sessionInfo) const
{
    if (_globalSessionLogging || IsControlFileLoggingEnabled())
        return true;

    std::string filterName = GetFilterName(sessionInfo);
    if (filterName.empty())
        return false;

    return _characterSessionLogging.find(NormalizeFilterName(filterName)) != _characterSessionLogging.end();
}

FILE* PacketLogServer::OpenSessionLog(void const* sessionKey, PacketLogServerSessionInfo const& sessionInfo)
{
    auto itr = _sessionLogs.find(sessionKey);
    if (itr != _sessionLogs.end())
        return itr->second.file;

    std::error_code error;
    std::filesystem::create_directories(_sessionLogDir, error);
    if (error)
        return NULL;

    std::filesystem::path path = BuildSessionLogPath(_sessionLogDir, sessionKey, sessionInfo);
    FILE* file = fopen(path.string().c_str(), "w");
    if (!file)
        return NULL;

    fprintf(file, "# SkyFire packet log session\n");
    fprintf(file, "# RemoteAddress: %s\n", sessionInfo.RemoteAddress.c_str());
    fprintf(file, "# AccountId: %u\n", sessionInfo.AccountId);
    if (!sessionInfo.AccountName.empty())
        fprintf(file, "# AccountName: %s\n", sessionInfo.AccountName.c_str());
    fprintf(file, "# RealmId: %u\n", sessionInfo.RealmId);
    fprintf(file, "# CharacterName: %s\n", sessionInfo.CharacterName.empty() ? "none" : sessionInfo.CharacterName.c_str());
    if (!sessionInfo.SessionName.empty())
        fprintf(file, "# SessionName: %s\n", sessionInfo.SessionName.c_str());
    fprintf(file, "# Format: sequence unix_time direction opcode_number opcode_name size payload_hex\n");
    fflush(file);

    SessionLog session;
    session.file = file;
    session.sequence = 0;
    session.path = path.string();
    session.info = sessionInfo;
    _sessionLogs[sessionKey] = session;
    return file;
}

void PacketLogServer::LogPacket(void const* sessionKey, PacketLogServerSessionInfo const& sessionInfo,
    PacketLogServerDirection direction, uint32 opcode, std::string const& opcodeName,
    void const* payload, size_t payloadSize)
{
    std::lock_guard<std::mutex> guard(_sessionLock);
    if (!ShouldLogSession(sessionInfo))
    {
        auto itr = _sessionLogs.find(sessionKey);
        if (itr != _sessionLogs.end())
        {
            if (itr->second.file)
                fclose(itr->second.file);

            _sessionLogs.erase(itr);
        }

        return;
    }

    FILE* file = OpenSessionLog(sessionKey, sessionInfo);
    if (!file)
        return;

    SessionLog& session = _sessionLogs[sessionKey];
    session.info = sessionInfo;

    fprintf(file, "%llu %u %s 0x%04X %s %u ",
        static_cast<unsigned long long>(++session.sequence),
        static_cast<uint32>(time(NULL)),
        direction == PACKET_LOG_CLIENT_TO_SERVER ? "CMSG" : "SMSG",
        opcode,
        opcodeName.c_str(),
        static_cast<uint32>(payloadSize));

    uint8 const* bytes = static_cast<uint8 const*>(payload);
    for (size_t i = 0; i < payloadSize && bytes; ++i)
        fprintf(file, "%02X", bytes[i]);

    fputc('\n', file);
    fflush(file);
}

void PacketLogServer::LogBinaryPacket(PacketLogServerDirection direction, uint32 opcode, void const* payload, size_t payloadSize)
{
    std::lock_guard<std::mutex> guard(_sessionLock);
    if (!_binaryFile)
        return;

    ByteBuffer data(uint32(4 + 4 + 4 + 1 + payloadSize));
    data << int32(opcode);
    data << int32(payloadSize);
    data << uint32(time(NULL));
    data << uint8(direction == PACKET_LOG_CLIENT_TO_SERVER ? 0 : 1);

    if (payload && payloadSize)
        data.append(static_cast<uint8 const*>(payload), payloadSize);

    fwrite(data.contents(), 1, data.size(), _binaryFile);
    fflush(_binaryFile);
}

void PacketLogServer::LogMarker(void const* sessionKey, PacketLogServerSessionInfo const& sessionInfo, std::string const& marker)
{
    std::lock_guard<std::mutex> guard(_sessionLock);
    if (!ShouldLogSession(sessionInfo))
    {
        auto itr = _sessionLogs.find(sessionKey);
        if (itr != _sessionLogs.end())
        {
            if (itr->second.file)
                fclose(itr->second.file);

            _sessionLogs.erase(itr);
        }

        return;
    }

    FILE* file = OpenSessionLog(sessionKey, sessionInfo);
    if (!file)
        return;

    SessionLog& session = _sessionLogs[sessionKey];
    session.info = sessionInfo;
    fprintf(file, "# %llu %u MARKER %s\n",
        static_cast<unsigned long long>(++session.sequence),
        static_cast<uint32>(time(NULL)),
        SanitizeMarker(marker).c_str());
    fflush(file);
}

void PacketLogServer::RefreshSessionInfo(void const* sessionKey, PacketLogServerSessionInfo const& sessionInfo)
{
    std::lock_guard<std::mutex> guard(_sessionLock);
    auto itr = _sessionLogs.find(sessionKey);
    if (itr == _sessionLogs.end())
        return;

    SessionLog& session = itr->second;
    bool const identityChanged = BuildSessionIdentity(session.info) != BuildSessionIdentity(sessionInfo);
    session.info = sessionInfo;

    if (!identityChanged || session.path.empty())
        return;

    if (session.file)
    {
        fflush(session.file);
        fclose(session.file);
        session.file = NULL;
    }

    std::filesystem::path oldPath = session.path;
    std::filesystem::path newPath = BuildSessionLogPath(_sessionLogDir, sessionKey, sessionInfo);
    std::error_code error;
    std::filesystem::rename(oldPath, newPath, error);
    if (!error)
        session.path = newPath.string();

    session.file = fopen(session.path.c_str(), "a");
    if (!session.file)
    {
        _sessionLogs.erase(itr);
        return;
    }

    fprintf(session.file, "# %llu %u MARKER refreshed session metadata accountId=%u accountName=%s realm=%u character=%s session=%s\n",
        static_cast<unsigned long long>(++session.sequence),
        static_cast<uint32>(time(NULL)),
        sessionInfo.AccountId,
        sessionInfo.AccountName.empty() ? "none" : sessionInfo.AccountName.c_str(),
        sessionInfo.RealmId,
        sessionInfo.CharacterName.empty() ? "none" : sessionInfo.CharacterName.c_str(),
        sessionInfo.SessionName.empty() ? "none" : sessionInfo.SessionName.c_str());
    fflush(session.file);
}

void PacketLogServer::CloseSession(void const* sessionKey)
{
    std::lock_guard<std::mutex> guard(_sessionLock);
    auto itr = _sessionLogs.find(sessionKey);
    if (itr == _sessionLogs.end())
        return;

    if (itr->second.file)
        fclose(itr->second.file);

    _sessionLogs.erase(itr);
}

void PacketLogServer::CloseAllSessions()
{
    for (auto& session : _sessionLogs)
        if (session.second.file)
            fclose(session.second.file);

    _sessionLogs.clear();
}

void PacketLogServer::CloseCharacterSessions(std::string const& normalizedName)
{
    for (auto itr = _sessionLogs.begin(); itr != _sessionLogs.end();)
    {
        if (NormalizeFilterName(GetFilterName(itr->second.info)) != normalizedName)
        {
            ++itr;
            continue;
        }

        if (itr->second.file)
            fclose(itr->second.file);

        itr = _sessionLogs.erase(itr);
    }
}
}
