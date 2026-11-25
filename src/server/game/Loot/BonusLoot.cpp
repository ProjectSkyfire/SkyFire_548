/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "BonusLoot.h"
#include "BonusLootRules.h"

#include "DatabaseEnv.h"
#include "DBCStores.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "Timer.h"

#include <unordered_map>

namespace
{
    std::unordered_map<uint32, BonusLootTemplate> BonusLootStore;
}

void Skyfire::BonusLoot::Load()
{
    uint32 oldMSTime = getMSTime();

    BonusLootStore.clear();

    //                                               0      1      2         3               4               5
    QueryResult result = WorldDatabase.Query("SELECT entry, spell, currency, loot_id_normal, loot_id_heroic, loot_source+0 FROM bonus_loot_template");
    if (!result)
    {
        SF_LOG_INFO("server.loading", ">> Loaded 0 bonus loot templates. DB table `bonus_loot_template` is empty.");
        return;
    }

    do
    {
        Field* fields = result->Fetch();
        BonusLootTemplate loot;
        loot.Entry = fields[0].GetUInt32();
        loot.Spell = fields[1].GetUInt32();
        loot.Currency = fields[2].GetUInt16();
        loot.LootIdNormal = fields[3].GetUInt32();
        loot.LootIdHeroic = fields[4].GetUInt32();
        loot.Source = uint8(fields[5].GetUInt32());

        if (!sObjectMgr->GetCreatureTemplate(loot.Entry))
        {
            SF_LOG_ERROR("sql.sql", "Table `bonus_loot_template` has a row for creature %u, which does not exist. Skipped.", loot.Entry);
            continue;
        }

        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(loot.Spell);
        if (!spellInfo || !spellInfo->HasAura(SPELL_AURA_BONUS_ROLL_TRIGGER))
        {
            SF_LOG_ERROR("sql.sql", "Table `bonus_loot_template` has spell %u for creature %u, which is no bonus roll trigger. Skipped.", loot.Spell, loot.Entry);
            continue;
        }

        if (!sCurrencyTypesStore.LookupEntry(loot.Currency))
        {
            SF_LOG_ERROR("sql.sql", "Table `bonus_loot_template` has currency %u for creature %u, which does not exist. Skipped.", loot.Currency, loot.Entry);
            continue;
        }

        if (loot.Source != SOURCE_CREATURE && loot.Source != SOURCE_GAMEOBJECT)
        {
            SF_LOG_ERROR("sql.sql", "Table `bonus_loot_template` has loot_source %u for creature %u, which is unknown. Skipped.", loot.Source, loot.Entry);
            continue;
        }

        BonusLootStore[loot.Entry] = loot;
    }
    while (result->NextRow());

    SF_LOG_INFO("server.loading", ">> Loaded %u bonus loot templates in %u ms", uint32(BonusLootStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

BonusLootTemplate const* Skyfire::BonusLoot::Get(uint32 entry)
{
    auto itr = BonusLootStore.find(entry);
    return itr != BonusLootStore.end() ? &itr->second : nullptr;
}

BonusLootTemplate const* Skyfire::BonusLoot::GetForChestLoot(uint32 lootId)
{
    if (!lootId)
        return nullptr;

    for (auto const& itr : BonusLootStore)
        if (itr.second.Source == SOURCE_GAMEOBJECT && (itr.second.LootIdNormal == lootId || itr.second.LootIdHeroic == lootId))
            return &itr.second;

    return nullptr;
}
