/*
 * This file is part of Project SkyFire https://www.projectskyfire.org.
 * See LICENSE.md file for Copyright information
 */

#ifndef SKYFIRE_OBJECT_ENTRY_CONDITION_H
#define SKYFIRE_OBJECT_ENTRY_CONDITION_H

#include <cstdint>

namespace Skyfire
{
    inline bool MatchesObjectEntry(std::uint32_t entry, std::uint32_t spawnGuid,
        std::uint32_t requiredEntry, std::uint32_t requiredSpawnGuid)
    {
        return (!requiredEntry || entry == requiredEntry) &&
            (!requiredSpawnGuid || spawnGuid == requiredSpawnGuid);
    }
}

#endif
