/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_LOOT_COUNT_H
#define SKYFIRE_LOOT_COUNT_H

#include "Define.h"

namespace Skyfire
{
namespace Looting
{
    inline uint32 CalculateLootItemCount(uint32 minCount, uint32 maxCount, uint32 roll)
    {
        if (maxCount <= minCount)
            return minCount;

        return minCount + (roll % (maxCount - minCount + 1));
    }
}
}

#endif
