/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_KILL_REWARD_RULES_H
#define SKYFIRE_KILL_REWARD_RULES_H

#include <cstdint>

// Pure rules of the kill reward hook, kept free of game headers so unit tests can include them.
namespace Skyfire::KillReward
{
    // DifficultyID of raid finder, mirrored from DBCEnums.h (checked by a static_assert in KillReward.cpp).
    constexpr std::uint8_t DIFFICULTY_ID_RAID_FINDER = 7;

    // A creature_loot_currency row applies when its lootmode is 0 (any difficulty)
    // or has the bit (1 << DifficultyID) of the map the creature died on.
    constexpr bool CurrencyRowMatchesDifficulty(std::uint32_t lootMode, std::uint8_t difficulty)
    {
        if (!lootMode)
            return true;

        return difficulty < 32 && (lootMode & (std::uint32_t(1) << difficulty)) != 0;
    }

    // Raid finder bosses pay no currency on kill in 5.4.8.
    constexpr bool DifficultyPaysKillCurrency(std::uint8_t difficulty)
    {
        return difficulty != DIFFICULTY_ID_RAID_FINDER;
    }
}

#endif
