/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_BONUS_LOOT_H
#define SKYFIRE_BONUS_LOOT_H

#include "Define.h"

// One row of `bonus_loot_template`.
struct BonusLootTemplate
{
    uint32 Entry = 0;           // base creature entry of the boss
    uint32 Spell = 0;           // prompt aura (SPELL_AURA_BONUS_ROLL_TRIGGER)
    uint32 Currency = 0;        // spent by the roll
    uint32 LootIdNormal = 0;
    uint32 LootIdHeroic = 0;
    uint8 Source = 0;           // Skyfire::BonusLoot::SOURCE_*
};

// Bonus rolls: a player spends one boss currency for an extra roll on the boss's loot,
// once per boss and difficulty a week.
namespace Skyfire::BonusLoot
{
    void Load();                                            // after the spell info store and creature templates
    BonusLootTemplate const* Get(uint32 entry);
    BonusLootTemplate const* GetForChestLoot(uint32 lootId); // gameobject source whose chest has this loot id
}

#endif
