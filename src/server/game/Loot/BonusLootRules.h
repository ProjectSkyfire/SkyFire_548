/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_BONUS_LOOT_RULES_H
#define SKYFIRE_BONUS_LOOT_RULES_H

#include <cstdint>

// Pure rules of the bonus roll, kept free of game headers so unit tests can include them.
namespace Skyfire::BonusLoot
{
    // `bonus_loot_template`.`loot_source` read as a number (enum position).
    constexpr std::uint8_t SOURCE_CREATURE = 1;
    constexpr std::uint8_t SOURCE_GAMEOBJECT = 2;

    // World bosses (open world), raid finder and flex roll on the boss's personal loot list.
    constexpr bool RollsOnPersonalLoot(std::uint8_t difficulty)
    {
        return difficulty == 0 || difficulty == 7 || difficulty == 14;
    }

    // Loot id the roll draws from on the other difficulties: 10 and 25 normal use the normal id,
    // 10 and 25 heroic the heroic id. Any other difficulty has no bonus roll loot.
    constexpr std::uint32_t LootIdForDifficulty(std::uint32_t normalId, std::uint32_t heroicId, std::uint8_t difficulty)
    {
        return difficulty == 3 || difficulty == 4 ? normalId : (difficulty == 5 || difficulty == 6 ? heroicId : 0);
    }

    // Kor'kron Shaman's Treasure, Astral Cloud Serpent and Kor'kron Juggernaut never come from a
    // bonus roll (alexkulya).
    constexpr bool IsExcludedItem(std::uint32_t itemId)
    {
        return itemId == 105751 || itemId == 87777 || itemId == 104253;
    }
}

#endif
