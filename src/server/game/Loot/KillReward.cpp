/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "KillReward.h"
#include "KillRewardRules.h"

#include "Creature.h"
#include "DatabaseEnv.h"
#include "DBCEnums.h"
#include "DBCStores.h"
#include "Group.h"
#include "Log.h"
#include "Map.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "Timer.h"

#include <limits>
#include <unordered_map>

static_assert(Skyfire::KillReward::DIFFICULTY_ID_RAID_FINDER == DIFFICULTY_25MAN_LFR, "raid finder DifficultyID changed");

namespace
{
    std::unordered_map<uint32, CreatureLootCurrencyList> CreatureLootCurrencyStore;

    void RewardPlayer(Player* player, uint8 difficulty, CreatureLootCurrencyList const* currencies)
    {
        if (currencies && Skyfire::KillReward::DifficultyPaysKillCurrency(difficulty))
            for (CreatureLootCurrency const& row : *currencies)
                if (Skyfire::KillReward::CurrencyRowMatchesDifficulty(row.LootMode, difficulty))
                    player->ModifyCurrency(row.CurrencyId, int32(row.Count)); // weekly and total caps apply here
    }
}

void Skyfire::KillReward::LoadCreatureLootCurrency()
{
    uint32 oldMSTime = getMSTime();

    CreatureLootCurrencyStore.clear();

    //                                               0      1         2         3
    QueryResult result = WorldDatabase.Query("SELECT entry, lootmode, currency, count FROM creature_loot_currency");
    if (!result)
    {
        SF_LOG_INFO("server.loading", ">> Loaded 0 creature kill currency rows. DB table `creature_loot_currency` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();
        CreatureLootCurrency row;
        row.LootMode = fields[1].GetUInt32();
        row.CurrencyId = fields[2].GetUInt16();
        row.Count = fields[3].GetUInt32();

        if (!sObjectMgr->GetCreatureTemplate(entry))
        {
            SF_LOG_ERROR("sql.sql", "Table `creature_loot_currency` has a row for creature %u, which does not exist. Skipped.", entry);
            continue;
        }

        if (!sCurrencyTypesStore.LookupEntry(row.CurrencyId))
        {
            SF_LOG_ERROR("sql.sql", "Table `creature_loot_currency` has currency %u for creature %u, which does not exist. Skipped.", row.CurrencyId, entry);
            continue;
        }

        if (!row.Count || row.Count > uint32(std::numeric_limits<int32>::max()))
        {
            SF_LOG_ERROR("sql.sql", "Table `creature_loot_currency` has count %u for currency %u of creature %u, out of range. Skipped.", row.Count, row.CurrencyId, entry);
            continue;
        }

        CreatureLootCurrencyStore[entry].push_back(row);
        ++count;
    }
    while (result->NextRow());

    SF_LOG_INFO("server.loading", ">> Loaded %u creature kill currency rows for %u creatures in %u ms", count, uint32(CreatureLootCurrencyStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

CreatureLootCurrencyList const* Skyfire::KillReward::GetCreatureLootCurrency(uint32 entry)
{
    auto itr = CreatureLootCurrencyStore.find(entry);
    return itr != CreatureLootCurrencyStore.end() ? &itr->second : nullptr;
}

void Skyfire::KillReward::Reward(Player* killer, Creature* victim)
{
    if (!killer || !victim)
        return;

    // SkyFire keeps the base entry at every difficulty, so rows key the base entry.
    CreatureLootCurrencyList const* currencies = GetCreatureLootCurrency(victim->GetEntry());
    if (!currencies)
        return;

    uint8 difficulty = uint8(victim->GetMap()->GetDifficulty());

    if (Group* group = killer->GetGroup())
    {
        for (GroupReference* itr = group->GetFirstMember(); itr != nullptr; itr = itr->next())
            if (Player* member = itr->GetSource())
                if (member->IsAtGroupRewardDistance(victim))
                    RewardPlayer(member, difficulty, currencies);
    }
    else
        RewardPlayer(killer, difficulty, currencies);
}
