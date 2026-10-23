/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef BATTLE_PET_SPAWN_MGR_H
#define BATTLE_PET_SPAWN_MGR_H

#include "Common.h"

#include <map>
#include <set>
#include <vector>

class Creature;

struct BattlePetWildPoolTemplate
{
    uint16 Species = 0;
    uint32 Entry = 0;
    uint8 Max = 0;
    uint8 MinLevel = 1;
    uint8 MaxLevel = 1;

    std::map<uint64, uint64> Replacements;
    std::set<uint64> ReadyForReplacement;
};

class BattlePetWildZonePool
{
public:
    void AddTemplate(BattlePetWildPoolTemplate const& spawnTemplate);
    void Populate();
    void OnCreatureAdded(Creature* creature);
    void OnCreatureRemoved(Creature* creature);
    void OnCreatureUnloaded(Creature* creature);
    bool RemoveReplacement(Creature* replacement);
    bool IsReplacement(uint64 guid) const;

private:
    void SpawnReplacement(uint64 originalGuid, BattlePetWildPoolTemplate& spawnTemplate);
    bool RemoveReplacement(uint64 originalGuid, BattlePetWildPoolTemplate& spawnTemplate);

    std::vector<BattlePetWildPoolTemplate> _templates;
};

class BattlePetSpawnMgr
{
public:
    static BattlePetSpawnMgr* instance();

    void LoadFromDB();
    void Update(uint32 diff);
    void OnCreatureAdded(Creature* creature);
    void OnCreatureRemoved(Creature* creature);
    void OnCreatureUnloaded(Creature* creature);
    bool ResolveWildBattle(Creature* creature);
    bool IsWildBattlePetReplacement(Creature const* creature) const;

private:
    typedef std::map<uint32, BattlePetWildZonePool> BattlePetZonePoolMap;
    typedef std::map<uint32, BattlePetZonePoolMap> BattlePetMapPoolMap;

    BattlePetWildZonePool* GetZonePool(Creature const* creature);
    BattlePetWildZonePool const* GetZonePool(Creature const* creature) const;

    BattlePetMapPoolMap _mapPools;
    uint32 _updateTimer = 0;
};

#define sBattlePetSpawnMgr BattlePetSpawnMgr::instance()

#endif // BATTLE_PET_SPAWN_MGR_H
