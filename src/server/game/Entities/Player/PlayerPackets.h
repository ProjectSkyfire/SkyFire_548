/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_PLAYER_PACKETS_H
#define SKYFIRE_PLAYER_PACKETS_H

#include "WorldPacket.h"

namespace Skyfire::PlayerPackets
{
    inline WorldPacket BuildCancelCombatPacket()
    {
        return WorldPacket(SMSG_CANCEL_COMBAT, 0);
    }

    inline WorldPacket BuildPreResurrectPacket(ObjectGuid playerGuid)
    {
        WorldPacket data(SMSG_PRE_RESURRECT, 8);
        data.WriteGuidMask(playerGuid, 1, 7, 5, 2, 6, 0, 3, 4);
        data.WriteGuidBytes(playerGuid, 5, 1, 7, 0, 6, 4, 2, 3);

        return data;
    }
}

#endif
