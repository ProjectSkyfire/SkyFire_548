/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Log.h"
#include "Opcodes.h"
#include "WorldPacket.h"
#include "WorldSession.h"

#include <ctime>

void WorldSession::HandleGetChallengeModeRewards(WorldPacket& /*recvData*/)
{
    SF_LOG_DEBUG("network", "CMSG_GET_CHALLENGE_MODE_REWARDS %s", GetPlayerInfo().c_str());

    WorldPacket data(SMSG_CHALLENGE_MODE_REWARDS);
    data.WriteBits(0, 21); // Rewards count
    data.WriteBits(0, 20); // Tier rewards count
    data.FlushBits();

    SendPacket(&data);
}

void WorldSession::HandleChallengeModeRequestLeaders(WorldPacket& recvData)
{
    uint32 mapId = recvData.read<uint32>();
    recvData.read_skip<uint32>(); // Last guild update
    recvData.read_skip<uint32>(); // Last realm update

    SF_LOG_DEBUG("network", "CMSG_CHALLENGE_MODE_REQUEST_LEADERS %s map: %u", GetPlayerInfo().c_str(), mapId);

    WorldPacket data(SMSG_CHALLENGE_MODE_REQUEST_LEADERS_RESULT);
    data << uint32(time(nullptr));
    data << uint32(time(nullptr));
    data << uint32(mapId);
    data.WriteBits(0, 19); // Guild leaders count
    data.WriteBits(0, 19); // Realm leaders count
    data.FlushBits();

    SendPacket(&data);
}

void WorldSession::HandleChallengeModeRequestMapStats(WorldPacket& /*recvData*/)
{
    SF_LOG_DEBUG("network", "CMSG_CHALLENGE_MODE_REQUEST_MAP_STATS %s", GetPlayerInfo().c_str());

    WorldPacket data(SMSG_CHALLENGE_MODE_ALL_MAP_STATS);
    data.WriteBits(0, 19); // Map data count
    data.FlushBits();

    SendPacket(&data);
}
