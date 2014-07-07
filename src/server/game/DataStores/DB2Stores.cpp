/*
 * Copyright (C) 2011-2014 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2014 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "BattlePet.h"
#include "DB2Stores.h"
#include "DB2fmt.h"
#include "DB2Utility.h"
#include "Common.h"
#include "Log.h"
#include "World.h"
#include "DBCStores.h"

DB2Storage<BattlePetAbilityEntry> sBattlePetAbilityStore(BattlePetAbilityfmt);
DB2Storage<BattlePetAbilityStateEntry> sBattlePetAbilityStateStore(BattlePetAbilityStatefmt);
DB2Storage<BattlePetBreedStateEntry> sBattlePetBreedStateStore(BattlePetBreedStatefmt);
DB2Storage<BattlePetSpeciesEntry> sBattlePetSpeciesStore(BattlePetSpeciesfmt);
DB2Storage<BattlePetSpeciesStateEntry> sBattlePetSpeciesStateStore(BattlePetSpeciesStatefmt);
DB2Storage<BattlePetSpeciesXAbilityEntry> sBattlePetSpeciesXAbilityStore(BattlePetSpeciesXAbilityfmt);
DB2Storage<BattlePetStateEntry> sBattlePetStateStore(BattlePetStatefmt);
DB2Storage<ItemToBattlePetEntry> sItemToBattlePetStore(ItemToBattlePetfmt);

BattlePetBreedSet sBattlePetBreedSet;
BattlePetItemXSpeciesStore sBattlePetItemXSpeciesStore;

DB2Storage<BroadcastTextEntry> sBroadcastTextStore(BroadcastTextfmt/*, &DB2Utilities::HasBroadcastTextEntry, &DB2Utilities::WriteBroadcastTextDbReply*/);
DB2Storage<ItemEntry> sItemStore(Itemfmt, &DB2Utilities::HasItemEntry, &DB2Utilities::WriteItemDbReply);
DB2Storage<ItemCurrencyCostEntry> sItemCurrencyCostStore(ItemCurrencyCostfmt);
DB2Storage<ItemExtendedCostEntry> sItemExtendedCostStore(ItemExtendedCostEntryfmt);
DB2Storage<ItemSparseEntry> sItemSparseStore(ItemSparsefmt, &DB2Utilities::HasItemSparseEntry, &DB2Utilities::WriteItemSparseDbReply);
DB2Storage<KeyChainEntry> sKeyChainStore(KeyChainfmt);
DB2Storage<QuestPackageItemEntry> sQuestPackageItemStore(QuestPackageItemfmt);
DB2Storage<SceneScriptEntry> sSceneScriptStore(SceneScriptfmt);
DB2Storage<SpellReagentsEntry> sSpellReagentsStore(SpellReagentsfmt);

typedef std::list<std::string> DB2StoreProblemList;

typedef std::map<uint32 /*hash*/, DB2StorageBase*> DB2StorageMap;
DB2StorageMap DB2Stores;

uint32 DB2FilesCount = 0;

static bool LoadDB2_assert_print(uint32 fsize, uint32 rsize, std::string const& filename)
{
    TC_LOG_ERROR("misc", "Size of '%s' setted by format string (%u) not equal size of C++ structure (%u).", filename.c_str(), fsize, rsize);

    // ASSERT must fail after function call
    return false;
}

template<class T>
inline void LoadDB2(uint32& availableDb2Locales, DB2StoreProblemList& errlist, DB2Storage<T>& storage, std::string const& db2_path, std::string const& filename)
{
    // compatibility format and C++ structure sizes
    ASSERT(DB2FileLoader::GetFormatRecordSize(storage.GetFormat()) == sizeof(T) || LoadDB2_assert_print(DB2FileLoader::GetFormatRecordSize(storage.GetFormat()), sizeof(T), filename));

    ++DB2FilesCount;

    std::string db2_filename = db2_path + filename;
    if (storage.Load(db2_filename.c_str(), uint32(sWorld->GetDefaultDbcLocale())))
    {
        for (uint32 i = 0; i < TOTAL_LOCALES; ++i)
        {
            if (!(availableDb2Locales & (1 << i)))
                continue;

            if (uint32(sWorld->GetDefaultDbcLocale()) == i)
                continue;

            std::string localizedName(db2_path);
            localizedName.append(localeNames[i]);
            localizedName.push_back('/');
            localizedName.append(filename);

            if (!storage.LoadStringsFrom(localizedName.c_str(), i))
                availableDb2Locales &= ~(1<<i);             // mark as not available for speedup next checks
        }
    }
    else
    {
        // sort problematic db2 to (1) non compatible and (2) nonexistent
        if (FILE* f = fopen(db2_filename.c_str(), "rb"))
        {
            char buf[100];
            snprintf(buf, 100,"(exist, but have %d fields instead " SIZEFMTD ") Wrong client version DBC file?", storage.GetFieldCount(), strlen(storage.GetFormat()));
            errlist.push_back(db2_filename + buf);
            fclose(f);
        }
        else
            errlist.push_back(db2_filename);
    }

    DB2Stores[storage.GetHash()] = &storage;
}

void LoadDB2Stores(std::string const& dataPath)
{
    std::string db2Path = dataPath + "dbc/";

    DB2StoreProblemList bad_db2_files;
    uint32 availableDb2Locales = 0xFF;

    LoadDB2(availableDb2Locales, bad_db2_files, sBattlePetAbilityStore, db2Path, "BattlePetAbility.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sBattlePetAbilityStateStore, db2Path, "BattlePetAbilityState.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sBattlePetBreedStateStore, db2Path, "BattlePetBreedState.db2");

    for (uint32 i = 0; i < sBattlePetBreedStateStore.GetNumRows(); i++)
        if (BattlePetBreedStateEntry const* breedStateEntry = sBattlePetBreedStateStore.LookupEntry(i))
            if (sBattlePetBreedSet.find(breedStateEntry->BreedId) == sBattlePetBreedSet.end())
                sBattlePetBreedSet.insert(breedStateEntry->BreedId);

    LoadDB2(availableDb2Locales, bad_db2_files, sBattlePetSpeciesStore, db2Path, "BattlePetSpecies.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sBattlePetSpeciesStateStore, db2Path, "BattlePetSpeciesState.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sBattlePetSpeciesXAbilityStore, db2Path, "BattlePetSpeciesXAbility.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sBattlePetStateStore, db2Path, "BattlePetState.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sItemToBattlePetStore, db2Path, "ItemToBattlePet.db2");

    for (uint32 i = 0; i < sItemToBattlePetStore.GetNumRows(); i++)
        if (ItemToBattlePetEntry const* itemEntry = sItemToBattlePetStore.LookupEntry(i))
            sBattlePetItemXSpeciesStore.insert(std::make_pair(itemEntry->ItemId, itemEntry->SpeciesId));

    LoadDB2(availableDb2Locales, bad_db2_files, sBroadcastTextStore, db2Path, "BroadcastText.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sItemStore, db2Path, "Item.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sItemCurrencyCostStore, db2Path, "ItemCurrencyCost.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sItemSparseStore, db2Path, "Item-sparse.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sItemExtendedCostStore, db2Path, "ItemExtendedCost.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sKeyChainStore, db2Path, "KeyChain.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sQuestPackageItemStore, db2Path, "QuestPackageItem.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sSceneScriptStore, db2Path, "SceneScript.db2");
    LoadDB2(availableDb2Locales, bad_db2_files, sSpellReagentsStore, db2Path, "SpellReagents.db2");

    // error checks
    if (bad_db2_files.size() >= DB2FilesCount)
    {
        TC_LOG_ERROR("misc", "\nIncorrect DataDir value in worldserver.conf or ALL required *.db2 files (%d) not found by path: %sdb2", DB2FilesCount, dataPath.c_str());
        exit(1);
    }
    else if (!bad_db2_files.empty())
    {
        std::string str;
        for (std::list<std::string>::iterator i = bad_db2_files.begin(); i != bad_db2_files.end(); ++i)
            str += *i + "\n";

        TC_LOG_ERROR("misc", "\nSome required *.db2 files (%u from %d) not found or not compatible:\n%s", (uint32)bad_db2_files.size(), DB2FilesCount, str.c_str());
        exit(1);
    }

    // Check loaded DB2 files proper version
    if (!sBattlePetAbilityStore.LookupEntry(1238)           // last battle pet ability added in 5.4.8 (18414)
        || !sBattlePetSpeciesStore.LookupEntry(1386)        // last battle pet species added in 5.4.8 (18414)
        || !sBattlePetStateStore.LookupEntry(176)           // last battle pet state added in 5.4.8 (18414)
        || !sItemToBattlePetStore.LookupEntry(109014)       // last battle pet item added in 5.4.8 (18414)
        || !sBroadcastTextStore.LookupEntry(77161)          // last broadcast text added in 5.4.8 (18414)
        || !sItemStore.LookupEntry(112353)                  // last item added in 5.4.8 (18414)
        || !sItemExtendedCostStore.LookupEntry(5280)        // last item extended cost added in 5.4.8 (18414)
        || !sQuestPackageItemStore.LookupEntry(2256)        // last quest package item in 5.4.8 (18414)
        || !sSceneScriptStore.LookupEntry(11156))           // last scene script added in 5.4.8 (18414)
    {
        TC_LOG_ERROR("misc", "You have _outdated_ DB2 files, Please extract correct db2 files from client 5.4.8 18414.");
        exit(1);
    }

    TC_LOG_INFO("misc", ">> Initialized %d DB2 data stores.", DB2FilesCount);
}

DB2StorageBase const* GetDB2Storage(uint32 type)
{
    DB2StorageMap::const_iterator itr = DB2Stores.find(type);
    if (itr != DB2Stores.end())
        return itr->second;

    return NULL;
}

#define BATTLE_PET_MAIN_STAT_VALUE  8.0f
#define BATTLE_PET_MAIN_STAT_DIV    200.0f
#define BATTLE_PET_MAIN_STAT_OFFSET 1600.0f

float BattlePetSpeciesMainStat(uint16 stateId, uint16 speciesId)
{
    if (stateId != BATTLE_PET_STATE_STAT_POWER
        && stateId != BATTLE_PET_STATE_STAT_STAMINA
        && stateId != BATTLE_PET_STATE_STAT_SPEED)
        return 0;

    for (uint32 i = 0; i < sBattlePetSpeciesStateStore.GetNumRows(); i++)
    {
        BattlePetSpeciesStateEntry const* stateEntry = sBattlePetSpeciesStateStore.LookupEntry(i);
        if (!stateEntry)
            continue;

        if (stateEntry->StateId == stateId && stateEntry->SpeciesId == speciesId)
            return BATTLE_PET_MAIN_STAT_VALUE + ((float)stateEntry->Modifier / BATTLE_PET_MAIN_STAT_DIV);
    }

    return BATTLE_PET_MAIN_STAT_VALUE;
}

float BattlePetBreedMainStatModifier(uint16 stateId, uint8 breedId)
{
    if (stateId != BATTLE_PET_STATE_STAT_POWER
        && stateId != BATTLE_PET_STATE_STAT_STAMINA
        && stateId != BATTLE_PET_STATE_STAT_SPEED)
        return 0;

    for (uint32 i = 0; i < sBattlePetBreedStateStore.GetNumRows(); i++)
    {
        BattlePetBreedStateEntry const* stateEntry = sBattlePetBreedStateStore.LookupEntry(i);
        if (!stateEntry)
            continue;

        if (stateEntry->StateId == stateId && stateEntry->BreedId == breedId)
            return ((float)stateEntry->Value - BATTLE_PET_MAIN_STAT_OFFSET) / BATTLE_PET_MAIN_STAT_DIV;
    }

    return 0;
}

uint32 BattlePetGetSummonSpell(uint16 speciesId)
{
    BattlePetSpeciesEntry const* speciesEntry = sBattlePetSpeciesStore.LookupEntry(speciesId);
    if (!speciesEntry)
        return 0;

    return speciesEntry->SpellId;
}

bool BattlePetSpeciesHasFlag(uint16 speciesId, uint16 flag)
{
    BattlePetSpeciesEntry const* speciesEntry = sBattlePetSpeciesStore.LookupEntry(speciesId);
    if (!speciesEntry)
        return false;

    return (speciesEntry->Flags & flag) != 0;
}
