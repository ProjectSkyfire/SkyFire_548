/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_AGGRO_DISTANCE_H
#define SKYFIRE_AGGRO_DISTANCE_H

#include "Define.h"

namespace Skyfire
{
namespace Aggro
{
    inline float CalculateAttackDistance(uint32 playerLevel, uint32 creatureLevel, float detectionModifier, float aggroRate)
    {
        if (aggroRate == 0.0f)
            return 0.0f;

        int32 levelDifference = int32(playerLevel) - int32(creatureLevel);
        if (levelDifference < -25)
            levelDifference = -25;

        float distance = 15.0f - float(levelDifference) + detectionModifier;
        if (distance < 5.0f)
            distance = 5.0f;

        return distance * aggroRate;
    }
}
}

#endif
