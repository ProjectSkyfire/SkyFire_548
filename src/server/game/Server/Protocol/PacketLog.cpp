/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ByteBuffer.h"
#include "Config.h"
#include "Opcodes.h"
#include "PacketLog.h"
#include "WorldPacket.h"
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
}

PacketLog::PacketLog() : _file(NULL), _logsDir(), _controlFiles(), _sessionLogDir(), _sessionLogs()
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
    return _file != NULL || IsSessionLoggingEnabled();
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
    for (std::string const& controlFile : _controlFiles)
    {
        std::error_code error;
        if (!controlFile.empty() && std::filesystem::exists(controlFile, error) && !error)
            return true;
    }

    return false;
}

FILE* PacketLog::OpenSessionLog(void const* sessionKey, std::string const& remoteAddress)
{
    auto itr = _sessionLogs.find(sessionKey);
    if (itr != _sessionLogs.end())
        return itr->second.file;

    std::error_code error;
    std::filesystem::create_directories(_sessionLogDir, error);
    if (error)
        return NULL;

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
        << SanitizePathPart(remoteAddress)
        << "_"
        << reinterpret_cast<uintptr_t>(sessionKey)
        << ".pktlog";

    std::filesystem::path path = std::filesystem::path(_sessionLogDir) / filename.str();
    FILE* file = fopen(path.string().c_str(), "w");
    if (!file)
        return NULL;

    fprintf(file, "# SkyFire packet log session\n");
    fprintf(file, "# RemoteAddress: %s\n", remoteAddress.c_str());
    fprintf(file, "# Format: sequence unix_time direction opcode_number opcode_name size payload_hex\n");
    fflush(file);

    SessionLog session;
    session.file = file;
    session.sequence = 0;
    _sessionLogs[sessionKey] = session;
    return file;
}

void PacketLog::LogPacket(void const* sessionKey, std::string const& remoteAddress, WorldPacket const& packet, Direction direction)
{
    if (_file)
        LogPacket(packet, direction);

    if (!IsSessionLoggingEnabled())
    {
        CloseSession(sessionKey);
        return;
    }

    std::lock_guard<std::mutex> guard(_sessionLock);
    FILE* file = OpenSessionLog(sessionKey, remoteAddress);
    if (!file)
        return;

    SessionLog& session = _sessionLogs[sessionKey];
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

void PacketLog::LogMarker(void const* sessionKey, std::string const& remoteAddress, std::string const& marker)
{
    if (!IsSessionLoggingEnabled())
    {
        CloseSession(sessionKey);
        return;
    }

    std::lock_guard<std::mutex> guard(_sessionLock);
    FILE* file = OpenSessionLog(sessionKey, remoteAddress);
    if (!file)
        return;

    SessionLog& session = _sessionLogs[sessionKey];
    fprintf(file, "# %llu %u MARKER %s\n",
        static_cast<unsigned long long>(++session.sequence),
        static_cast<uint32>(time(NULL)),
        SanitizeMarker(marker).c_str());
    fflush(file);
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
