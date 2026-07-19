/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ByteBuffer.h"
#include "Config.h"
#include "Opcodes.h"
#include "PacketLog.h"
#include "WorldPacket.h"
#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdint>
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

std::string NormalizeCharacterName(std::string value)
{
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch)
    {
        return static_cast<char>(std::tolower(ch));
    });

    return value;
}

std::filesystem::path BuildSessionLogPath(std::string const& sessionLogDir, void const* sessionKey, PacketLogSessionInfo const& sessionInfo)
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
    filename << "session_"
        << std::put_time(&localTime, "%Y%m%d_%H%M%S")
        << "_"
        << SanitizePathPart(sessionInfo.RemoteAddress)
        << "_account"
        << sessionInfo.AccountId
        << "_realm"
        << sessionInfo.RealmId
        << "_"
        << SanitizePathPart(sessionInfo.CharacterName.empty() ? std::string("none") : sessionInfo.CharacterName)
        << "_"
        << reinterpret_cast<uintptr_t>(sessionKey)
        << ".pktlog";

    return std::filesystem::path(sessionLogDir) / filename.str();
}
}

PacketLog::PacketLog() : _file(NULL), _logsDir(), _controlFiles(), _sessionLogDir(),
    _globalSessionLogging(false), _characterSessionLogging(), _sessionLogs()
{
    Initialize();
}

PacketLog::~PacketLog()
{
    if (_file)
        fclose(_file);

    for (auto& session : _sessionLogs)
        if (session.second.file)
            fclose(session.second.file);

    _file = NULL;
    _sessionLogs.clear();
}

void PacketLog::Initialize()
{
    _logsDir = sConfigMgr->GetStringDefault("LogsDir", "");

    if (!_logsDir.empty())
        if ((_logsDir.at(_logsDir.length() - 1) != '/') && (_logsDir.at(_logsDir.length() - 1) != '\\'))
            _logsDir.push_back('/');

    std::string logname = sConfigMgr->GetStringDefault("PacketLogFile", "");
    if (!logname.empty())
        _file = fopen((_logsDir + logname).c_str(), "wb");

    std::string controlFile = sConfigMgr->GetStringDefault("PacketLogServerControlFile", "packetlogserver.active");
    if (!controlFile.empty())
    {
        _controlFiles.push_back(controlFile);

        std::string legacyControlFile = JoinPath(_logsDir, controlFile);
        if (legacyControlFile != controlFile)
            _controlFiles.push_back(legacyControlFile);
    }

    _sessionLogDir = sConfigMgr->GetStringDefault("PacketLogServerOutputDir", "PacketLogs");
}

bool PacketLog::CanLogPacket() const
{
    if (_file)
        return true;

    {
        std::lock_guard<std::mutex> guard(_sessionLock);
        if (_globalSessionLogging || !_characterSessionLogging.empty())
            return true;
    }

    return IsControlFileLoggingEnabled();
}

void PacketLog::LogPacket(WorldPacket const& packet, Direction direction)
{
    if (!_file)
        return;

    ByteBuffer data(4 + 4 + 4 + 1 + packet.size());
    uint32 opcode = direction == CLIENT_TO_SERVER ? const_cast<WorldPacket&>(packet).GetReceivedOpcode() : serverOpcodeTable[packet.GetOpcode()]->OpcodeNumber;

    data << int32(opcode);
    data << int32(packet.size());
    data << uint32(time(NULL));
    data << uint8(direction);

    for (uint32 i = 0; i < packet.size(); i++)
        data << packet[i];

    fwrite(data.contents(), 1, data.size(), _file);
    fflush(_file);
}

bool PacketLog::IsSessionLoggingEnabled() const
{
    std::lock_guard<std::mutex> guard(_sessionLock);
    return _globalSessionLogging || !_characterSessionLogging.empty() || IsControlFileLoggingEnabled();
}

bool PacketLog::IsControlFileLoggingEnabled() const
{
    for (std::string const& controlFile : _controlFiles)
    {
        std::error_code error;
        if (!controlFile.empty() && std::filesystem::exists(controlFile, error) && !error)
            return true;
    }

    return false;
}

void PacketLog::EnableGlobalLogging()
{
    std::lock_guard<std::mutex> guard(_sessionLock);
    _globalSessionLogging = true;
}

void PacketLog::DisableAllLogging()
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

void PacketLog::EnableCharacterLogging(std::string const& characterName)
{
    std::lock_guard<std::mutex> guard(_sessionLock);
    _characterSessionLogging.insert(NormalizeCharacterName(characterName));
}

void PacketLog::DisableCharacterLogging(std::string const& characterName)
{
    std::lock_guard<std::mutex> guard(_sessionLock);
    std::string normalizedName = NormalizeCharacterName(characterName);
    _characterSessionLogging.erase(normalizedName);

    if (!_globalSessionLogging && !IsControlFileLoggingEnabled())
        CloseCharacterSessions(normalizedName);
}

bool PacketLog::IsGlobalLoggingEnabled() const
{
    std::lock_guard<std::mutex> guard(_sessionLock);
    return _globalSessionLogging;
}

size_t PacketLog::GetCharacterLoggingCount() const
{
    std::lock_guard<std::mutex> guard(_sessionLock);
    return _characterSessionLogging.size();
}

bool PacketLog::ShouldLogSession(PacketLogSessionInfo const& sessionInfo) const
{
    if (_globalSessionLogging || IsControlFileLoggingEnabled())
        return true;

    if (sessionInfo.CharacterName.empty())
        return false;

    return _characterSessionLogging.find(NormalizeCharacterName(sessionInfo.CharacterName)) != _characterSessionLogging.end();
}

FILE* PacketLog::OpenSessionLog(void const* sessionKey, PacketLogSessionInfo const& sessionInfo)
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
    fprintf(file, "# RealmId: %u\n", sessionInfo.RealmId);
    fprintf(file, "# CharacterName: %s\n", sessionInfo.CharacterName.empty() ? "none" : sessionInfo.CharacterName.c_str());
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

void PacketLog::LogPacket(void const* sessionKey, PacketLogSessionInfo const& sessionInfo, WorldPacket const& packet, Direction direction)
{
    if (_file)
        LogPacket(packet, direction);

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
    uint32 opcode = direction == CLIENT_TO_SERVER ? const_cast<WorldPacket&>(packet).GetReceivedOpcode() : serverOpcodeTable[packet.GetOpcode()]->OpcodeNumber;
    std::string opcodeName = GetOpcodeNameForLogging(packet.GetOpcode(), direction == SERVER_TO_CLIENT);

    fprintf(file, "%llu %u %s 0x%04X %s %u ",
        static_cast<unsigned long long>(++session.sequence),
        static_cast<uint32>(time(NULL)),
        direction == CLIENT_TO_SERVER ? "CMSG" : "SMSG",
        opcode,
        opcodeName.c_str(),
        static_cast<uint32>(packet.size()));

    for (uint32 i = 0; i < packet.size(); ++i)
        fprintf(file, "%02X", packet[i]);

    fputc('\n', file);
    fflush(file);
}

void PacketLog::LogMarker(void const* sessionKey, PacketLogSessionInfo const& sessionInfo, std::string const& marker)
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

void PacketLog::RefreshSessionInfo(void const* sessionKey, PacketLogSessionInfo const& sessionInfo)
{
    std::lock_guard<std::mutex> guard(_sessionLock);
    auto itr = _sessionLogs.find(sessionKey);
    if (itr == _sessionLogs.end())
        return;

    SessionLog& session = itr->second;
    bool const gainedCharacterName = (session.info.CharacterName.empty() || session.info.CharacterName == "none")
        && !sessionInfo.CharacterName.empty() && sessionInfo.CharacterName != "none";

    session.info = sessionInfo;

    if (!gainedCharacterName || session.path.empty())
        return;

    if (session.file)
    {
        fflush(session.file);
        fclose(session.file);
        session.file = NULL;
    }

    std::filesystem::path newPath = BuildSessionLogPath(_sessionLogDir, sessionKey, sessionInfo);
    std::error_code error;
    std::filesystem::rename(session.path, newPath, error);
    if (!error)
        session.path = newPath.string();

    session.file = fopen(session.path.c_str(), "a");
    if (!session.file)
    {
        _sessionLogs.erase(itr);
        return;
    }

    fprintf(session.file, "# %llu %u MARKER refreshed session metadata account=%u realm=%u character=%s\n",
        static_cast<unsigned long long>(++session.sequence),
        static_cast<uint32>(time(NULL)),
        sessionInfo.AccountId,
        sessionInfo.RealmId,
        sessionInfo.CharacterName.c_str());
    fflush(session.file);
}

void PacketLog::CloseSession(void const* sessionKey)
{
    std::lock_guard<std::mutex> guard(_sessionLock);
    auto itr = _sessionLogs.find(sessionKey);
    if (itr == _sessionLogs.end())
        return;

    if (itr->second.file)
        fclose(itr->second.file);

    _sessionLogs.erase(itr);
}

void PacketLog::CloseAllSessions()
{
    for (auto& session : _sessionLogs)
        if (session.second.file)
            fclose(session.second.file);

    _sessionLogs.clear();
}

void PacketLog::CloseCharacterSessions(std::string const& normalizedName)
{
    for (auto itr = _sessionLogs.begin(); itr != _sessionLogs.end();)
    {
        if (NormalizeCharacterName(itr->second.info.CharacterName) != normalizedName)
        {
            ++itr;
            continue;
        }

        if (itr->second.file)
            fclose(itr->second.file);

        itr = _sessionLogs.erase(itr);
    }
}
