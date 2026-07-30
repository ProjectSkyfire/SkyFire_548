/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Opcodes.h"
#include "PacketLog.h"
#include "WorldPacket.h"

namespace
{
Skyfire::PacketLogServerDirection ToPacketLogServerDirection(Direction direction)
{
    return direction == CLIENT_TO_SERVER ? Skyfire::PACKET_LOG_CLIENT_TO_SERVER : Skyfire::PACKET_LOG_SERVER_TO_CLIENT;
}
}

PacketLog::PacketLog()
{
    Initialize();
}

PacketLog::~PacketLog()
{
}

void PacketLog::Initialize()
{
    sPacketLogServer->Initialize("packetlogserver.active", "PacketLogs");
}

bool PacketLog::CanLogPacket() const
{
    return sPacketLogServer->CanLogPacket();
}

void PacketLog::LogPacket(WorldPacket const& packet, Direction direction)
{
    uint32 opcode = direction == CLIENT_TO_SERVER ? const_cast<WorldPacket&>(packet).GetReceivedOpcode() : serverOpcodeTable[packet.GetOpcode()]->OpcodeNumber;
    sPacketLogServer->LogBinaryPacket(ToPacketLogServerDirection(direction), opcode,
        packet.size() ? packet.contents() : NULL, packet.size());
}

void PacketLog::EnableGlobalLogging()
{
    sPacketLogServer->EnableGlobalLogging();
}

void PacketLog::DisableAllLogging()
{
    sPacketLogServer->DisableAllLogging();
}

void PacketLog::EnableCharacterLogging(std::string const& characterName)
{
    sPacketLogServer->EnableCharacterLogging(characterName);
}

void PacketLog::DisableCharacterLogging(std::string const& characterName)
{
    sPacketLogServer->DisableCharacterLogging(characterName);
}

bool PacketLog::IsGlobalLoggingEnabled() const
{
    return sPacketLogServer->IsGlobalLoggingEnabled();
}

size_t PacketLog::GetCharacterLoggingCount() const
{
    return sPacketLogServer->GetCharacterLoggingCount();
}

void PacketLog::LogPacket(void const* sessionKey, PacketLogSessionInfo const& sessionInfo, WorldPacket const& packet, Direction direction)
{
    LogPacket(packet, direction);

    uint32 opcode = direction == CLIENT_TO_SERVER ? const_cast<WorldPacket&>(packet).GetReceivedOpcode() : serverOpcodeTable[packet.GetOpcode()]->OpcodeNumber;
    std::string opcodeName = GetOpcodeNameForLogging(packet.GetOpcode(), direction == SERVER_TO_CLIENT);
    sPacketLogServer->LogPacket(sessionKey, sessionInfo, ToPacketLogServerDirection(direction), opcode, opcodeName,
        packet.size() ? packet.contents() : NULL, packet.size());
}

void PacketLog::LogMarker(void const* sessionKey, PacketLogSessionInfo const& sessionInfo, std::string const& marker)
{
    sPacketLogServer->LogMarker(sessionKey, sessionInfo, marker);
}

void PacketLog::RefreshSessionInfo(void const* sessionKey, PacketLogSessionInfo const& sessionInfo)
{
    sPacketLogServer->RefreshSessionInfo(sessionKey, sessionInfo);
}

void PacketLog::CloseSession(void const* sessionKey)
{
    sPacketLogServer->CloseSession(sessionKey);
}
