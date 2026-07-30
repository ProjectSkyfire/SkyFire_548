/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_COMBAT_PACKETS_H
#define SKYFIRE_COMBAT_PACKETS_H

namespace Skyfire::CombatPackets
{
    inline bool GetAttackStopVictimDeadBit(bool hasVictim, bool victimIsDead)
    {
        return hasVictim && victimIsDead;
    }
}

#endif
