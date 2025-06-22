/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Common.h"
#include "Log.h"
#include "Opcodes.h"
#include "WorldPacket.h"
#include "WorldSession.h"

void WorldSession::HandleVoiceSessionEnableOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_VOICE_SESSION_ENABLE");
    bool isVoiceEnabled, isMicrophoneEnabled;
    isVoiceEnabled = recvData.ReadBit();
    isMicrophoneEnabled = recvData.ReadBit();
}

void WorldSession::HandleChannelVoiceOnOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_CHANNEL_VOICE_ON");
    // Enable Voice button in channel context menu
    recvData.ReadString(recvData.ReadBits(8));
    //channel->EnableVoice(recvData.GetOpcode() == CMSG_CHANNEL_VOICE_ON);
}

void WorldSession::HandleSetActiveVoiceChannel(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_SET_ACTIVE_VOICE_CHANNEL");
    recvData.read_skip<uint32>();
    recvData.read_skip<char*>();
}
