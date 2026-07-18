/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "BattlePetSpawnMgr.h"
#include "Creature.h"
#include "DatabaseEnv.h"
#include "DBCStores.h"
#include "DB2Stores.h"
#include "Log.h"
#include "Map.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "SharedDefines.h"
#include "Util.h"
#include "World.h"

#include <unordered_map>

namespace
{
    uint32 const BattlePetSpawnMgrUpdateInterval = 2 * IN_MILLISECONDS;

    int32 BattlePetWildPoolMapForZone(uint32 zoneId)
    {
        typedef std::unordered_map<uint32, int32> BattlePetWildPoolZoneMapCache;
        static BattlePetWildPoolZoneMapCache zoneMapCache;

        BattlePetWildPoolZoneMapCache::const_iterator cacheItr = zoneMapCache.find(zoneId);
        if (cacheItr != zoneMapCache.end())
            return cacheItr->second;

        for (uint32 i = 0; i < sAreaStore.GetNumRows(); ++i)
        {
            AreaTableEntry const* areaEntry = sAreaStore.LookupEntry(i);
            if (areaEntry && areaEntry->m_ID == zoneId)
            {
                zoneMapCache[zoneId] = int32(areaEntry->m_ContinentID);
                return int32(areaEntry->m_ContinentID);
            }
        }

        zoneMapCache[zoneId] = -1;
        return -1;
    }

    uint8 BattlePetWildPoolLevel(Creature const& source, uint8 minLevel, uint8 maxLevel)
    {
        if (!minLevel)
            minLevel = 1;

        if (!maxLevel || maxLevel < minLevel)
            maxLevel = minLevel;

        if (maxLevel == minLevel)
            return minLevel;

        return uint8(minLevel + (rand32() % (uint32(maxLevel) - minLevel + 1)));
    }

    uint8 BattlePetWildPoolMaxCount(uint8 databaseMax)
    {
        if (!databaseMax)
            return 0;

        uint32 const configuredMin = sWorld->getIntConfig(WorldIntConfigs::CONFIG_BATTLE_PET_WILD_SPAWN_MIN_COUNT);
        if (!configuredMin || databaseMax >= configuredMin)
            return databaseMax;

        return uint8(configuredMin > 255 ? 255 : configuredMin);
    }

    uint32 BattlePetWildPoolZoneForCreature(Creature const* creature)
    {
        if (!creature || !creature->GetMap())
            return 0;

        return creature->GetMap()->GetZoneId(creature->GetPositionX(), creature->GetPositionY(), creature->GetPositionZ());
    }
}

BattlePetSpawnMgr* BattlePetSpawnMgr::instance()
{
    static BattlePetSpawnMgr instance;
    return &instance;
}

void BattlePetSpawnMgr::LoadFromDB()
{
    _mapPools.clear();
    _updateTimer = 0;

    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT zone, species, entry, max, minLevel, maxLevel FROM battle_pet_wild_pool");
    if (!result)
    {
        SF_LOG_INFO("server.loading", ">> Loaded 0 battle pet wild pool definitions. DB table `battle_pet_wild_pool` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();
        uint32 const zoneId = fields[0].GetUInt32();
        uint16 const species = fields[1].GetUInt16();
        uint32 const entry = fields[2].GetUInt32();
        uint8 const max = fields[3].GetUInt8();
        uint8 const minLevel = fields[4].GetUInt8();
        uint8 const maxLevel = fields[5].GetUInt8();

        BattlePetSpeciesEntry const* speciesEntry = sBattlePetSpeciesStore.LookupEntry(species);
        if (!speciesEntry)
        {
            SF_LOG_ERROR("sql.sql", "Table `battle_pet_wild_pool` has invalid species %u for creature entry %u, skipped.", species, entry);
            continue;
        }

        if (!sObjectMgr->GetCreatureTemplate(entry))
        {
            SF_LOG_ERROR("sql.sql", "Table `battle_pet_wild_pool` has invalid replacement source entry %u for species %u, skipped.", entry, species);
            continue;
        }

        if (!sObjectMgr->GetCreatureTemplate(speciesEntry->NpcId))
        {
            SF_LOG_ERROR("sql.sql", "Table `battle_pet_wild_pool` species %u references missing creature template %u, skipped.", species, speciesEntry->NpcId);
            continue;
        }

        int32 const mapId = BattlePetWildPoolMapForZone(zoneId);
        if (mapId < 0)
        {
            SF_LOG_ERROR("sql.sql", "Table `battle_pet_wild_pool` has invalid zone %u for species %u, skipped.", zoneId, species);
            continue;
        }

        BattlePetWildPoolTemplate spawnTemplate;
        spawnTemplate.Species = species;
        spawnTemplate.Entry = entry;
        spawnTemplate.Max = BattlePetWildPoolMaxCount(max);
        spawnTemplate.MinLevel = minLevel ? minLevel : 1;
        spawnTemplate.MaxLevel = maxLevel >= spawnTemplate.MinLevel ? maxLevel : spawnTemplate.MinLevel;

        _mapPools[uint32(mapId)][zoneId].AddTemplate(spawnTemplate);
        ++count;
    } while (result->NextRow());

    SF_LOG_INFO("server.loading", ">> Loaded %u battle pet wild pool definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void BattlePetSpawnMgr::Update(uint32 diff)
{
    _updateTimer += diff;
    if (_updateTimer < BattlePetSpawnMgrUpdateInterval)
        return;

    _updateTimer = 0;
    for (BattlePetMapPoolMap::iterator mapItr = _mapPools.begin(); mapItr != _mapPools.end(); ++mapItr)
        for (BattlePetZonePoolMap::iterator zoneItr = mapItr->second.begin(); zoneItr != mapItr->second.end(); ++zoneItr)
            zoneItr->second.Populate();
}

void BattlePetSpawnMgr::OnCreatureAdded(Creature* creature)
{
    if (!creature || creature->m_isTempWorldObject)
        return;

    if (BattlePetWildZonePool* zonePool = GetZonePool(creature))
        zonePool->OnCreatureAdded(creature);
}

void BattlePetSpawnMgr::OnCreatureRemoved(Creature* creature)
{
    if (!creature || creature->m_isTempWorldObject)
        return;

    if (BattlePetWildZonePool* zonePool = GetZonePool(creature))
        zonePool->OnCreatureRemoved(creature);
}

bool BattlePetSpawnMgr::ResolveWildBattle(Creature* creature)
{
    if (!creature)
        return false;

    if (BattlePetWildZonePool* zonePool = GetZonePool(creature))
        return zonePool->RemoveReplacement(creature);

    return false;
}

bool BattlePetSpawnMgr::IsWildBattlePetReplacement(Creature const* creature) const
{
    if (!creature)
        return false;

    if (BattlePetWildZonePool const* zonePool = GetZonePool(creature))
        return zonePool->IsReplacement(creature->GetGUID());

    return false;
}

BattlePetWildZonePool* BattlePetSpawnMgr::GetZonePool(Creature const* creature)
{
    if (!creature)
        return NULL;

    BattlePetMapPoolMap::iterator mapItr = _mapPools.find(creature->GetMapId());
    if (mapItr == _mapPools.end())
        return NULL;

    uint32 const zoneId = BattlePetWildPoolZoneForCreature(creature);
    if (!zoneId)
        return NULL;

    BattlePetZonePoolMap::iterator zoneItr = mapItr->second.find(zoneId);
    if (zoneItr == mapItr->second.end())
        return NULL;

    return &zoneItr->second;
}

BattlePetWildZonePool const* BattlePetSpawnMgr::GetZonePool(Creature const* creature) const
{
    if (!creature)
        return NULL;

    BattlePetMapPoolMap::const_iterator mapItr = _mapPools.find(creature->GetMapId());
    if (mapItr == _mapPools.end())
        return NULL;

    uint32 const zoneId = BattlePetWildPoolZoneForCreature(creature);
    if (!zoneId)
        return NULL;

    BattlePetZonePoolMap::const_iterator zoneItr = mapItr->second.find(zoneId);
    if (zoneItr == mapItr->second.end())
        return NULL;

    return &zoneItr->second;
}

void BattlePetWildZonePool::AddTemplate(BattlePetWildPoolTemplate const& spawnTemplate)
{
    for (std::vector<BattlePetWildPoolTemplate>::const_iterator itr = _templates.begin(); itr != _templates.end(); ++itr)
        if (itr->Entry == spawnTemplate.Entry && itr->Species == spawnTemplate.Species)
            return;

    _templates.push_back(spawnTemplate);
}

void BattlePetWildZonePool::Populate()
{
    for (std::vector<BattlePetWildPoolTemplate>::iterator itr = _templates.begin(); itr != _templates.end(); ++itr)
    {
        BattlePetWildPoolTemplate& spawnTemplate = *itr;
        if (!spawnTemplate.Max || spawnTemplate.Replacements.size() >= spawnTemplate.Max)
            continue;

        std::set<uint64> ready = spawnTemplate.ReadyForReplacement;
        for (std::set<uint64>::const_iterator guidItr = ready.begin(); guidItr != ready.end(); ++guidItr)
        {
            if (spawnTemplate.Replacements.size() >= spawnTemplate.Max)
                break;

            SpawnReplacement(*guidItr, spawnTemplate);
        }
    }
}

void BattlePetWildZonePool::OnCreatureAdded(Creature* creature)
{
    if (!creature)
        return;

    for (std::vector<BattlePetWildPoolTemplate>::iterator itr = _templates.begin(); itr != _templates.end(); ++itr)
    {
        BattlePetWildPoolTemplate& spawnTemplate = *itr;
        if (spawnTemplate.Entry == creature->GetEntry())
            spawnTemplate.ReadyForReplacement.insert(creature->GetGUID());
    }
}

void BattlePetWildZonePool::OnCreatureRemoved(Creature* creature)
{
    if (!creature)
        return;

    for (std::vector<BattlePetWildPoolTemplate>::iterator itr = _templates.begin(); itr != _templates.end(); ++itr)
    {
        BattlePetWildPoolTemplate& spawnTemplate = *itr;
        spawnTemplate.ReadyForReplacement.erase(creature->GetGUID());
        RemoveReplacement(creature->GetGUID(), spawnTemplate);
    }
}

bool BattlePetWildZonePool::RemoveReplacement(Creature* replacement)
{
    if (!replacement)
        return false;

    uint64 const replacementGuid = replacement->GetGUID();
    for (std::vector<BattlePetWildPoolTemplate>::iterator itr = _templates.begin(); itr != _templates.end(); ++itr)
    {
        BattlePetWildPoolTemplate& spawnTemplate = *itr;
        for (std::map<uint64, uint64>::const_iterator relationItr = spawnTemplate.Replacements.begin();
            relationItr != spawnTemplate.Replacements.end(); ++relationItr)
        {
            if (relationItr->second != replacementGuid)
                continue;

            return RemoveReplacement(relationItr->first, spawnTemplate);
        }
    }

    return false;
}

bool BattlePetWildZonePool::IsReplacement(uint64 guid) const
{
    for (std::vector<BattlePetWildPoolTemplate>::const_iterator itr = _templates.begin(); itr != _templates.end(); ++itr)
        for (std::map<uint64, uint64>::const_iterator relationItr = itr->Replacements.begin();
            relationItr != itr->Replacements.end(); ++relationItr)
            if (relationItr->second == guid)
                return true;

    return false;
}

void BattlePetWildZonePool::SpawnReplacement(uint64 originalGuid, BattlePetWildPoolTemplate& spawnTemplate)
{
    if (spawnTemplate.Replacements.find(originalGuid) != spawnTemplate.Replacements.end())
    {
        spawnTemplate.ReadyForReplacement.erase(originalGuid);
        return;
    }

    Creature* original = ObjectAccessor::GetObjectInWorld(originalGuid, static_cast<Creature*>(nullptr));
    if (!original || !original->IsInWorld() || original->m_isTempWorldObject || original->GetEntry() != spawnTemplate.Entry)
    {
        spawnTemplate.ReadyForReplacement.erase(originalGuid);
        return;
    }

    BattlePetSpeciesEntry const* speciesEntry = sBattlePetSpeciesStore.LookupEntry(spawnTemplate.Species);
    if (!speciesEntry)
    {
        spawnTemplate.ReadyForReplacement.erase(originalGuid);
        return;
    }

    Creature* replacement = new Creature();
    replacement->m_isTempWorldObject = true;

    if (!replacement->Create(sObjectMgr->GenerateLowGuid(HIGHGUID_UNIT), original->GetMap(),
        speciesEntry->NpcId, 0, 0, original->GetPositionX(), original->GetPositionY(),
        original->GetPositionZ(), original->GetOrientation()))
    {
        delete replacement;
        spawnTemplate.ReadyForReplacement.erase(originalGuid);
        return;
    }

    uint8 const level = BattlePetWildPoolLevel(*original, spawnTemplate.MinLevel, spawnTemplate.MaxLevel);
    replacement->SetLevel(level);
    replacement->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_WILDPET_CAPTURABLE);
    replacement->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC);
    replacement->SetUInt32Value(UNIT_FIELD_WILD_BATTLE_PET_LEVEL, level);

    if (!original->GetMap()->AddToMap(replacement))
    {
        delete replacement;
        spawnTemplate.ReadyForReplacement.erase(originalGuid);
        return;
    }

    original->DespawnOrUnsummon();
    original->SetRespawnTime(MONTH);
    original->RemoveCorpse(false);

    spawnTemplate.Replacements[originalGuid] = replacement->GetGUID();
    spawnTemplate.ReadyForReplacement.erase(originalGuid);
}

bool BattlePetWildZonePool::RemoveReplacement(uint64 originalGuid, BattlePetWildPoolTemplate& spawnTemplate)
{
    std::map<uint64, uint64>::iterator relationItr = spawnTemplate.Replacements.find(originalGuid);
    if (relationItr == spawnTemplate.Replacements.end())
        return false;

    uint64 const replacementGuid = relationItr->second;
    Creature* replacement = ObjectAccessor::GetObjectInWorld(replacementGuid, static_cast<Creature*>(nullptr));
    if (replacement)
    {
        replacement->RemoveFromWorld();
        replacement->AddObjectToRemoveList();
    }

    if (Creature* original = ObjectAccessor::GetObjectInWorld(originalGuid, static_cast<Creature*>(nullptr)))
    {
        if (CreatureData const* creatureData = original->GetCreatureData())
            original->SetRespawnTime(creatureData->spawntimesecs);
        else
            original->SetRespawnTime(0);
    }

    spawnTemplate.Replacements.erase(relationItr);
    spawnTemplate.ReadyForReplacement.erase(originalGuid);
    return true;
}
