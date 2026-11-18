/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_KILL_REWARD_H
#define SKYFIRE_KILL_REWARD_H

#include "Define.h"

#include <vector>

class Creature;
class Player;

// One row of `creature_loot_currency`: currency paid straight into the currency tab when the creature dies.
struct CreatureLootCurrency
{
    uint32 LootMode;    // difficulty bitmask (1 << DifficultyID), 0 = any difficulty
    uint32 CurrencyId;
    uint32 Count;       // storage units: hundredths for high precision currencies such as Justice and Valor
};

typedef std::vector<CreatureLootCurrency> CreatureLootCurrencyList;

// Rewards a creature kill gives beyond experience and corpse loot.
// Today: currency on kill. Personal loot and the bonus roll trigger extend Reward().
namespace Skyfire::KillReward
{
    void LoadCreatureLootCurrency();
    CreatureLootCurrencyList const* GetCreatureLootCurrency(uint32 entry);

    // Called once per creature kill with the player credited for it (the loot recipient).
    // Rewards each group member in reward range, or the killer alone.
    void Reward(Player* killer, Creature* victim);
}

#endif
