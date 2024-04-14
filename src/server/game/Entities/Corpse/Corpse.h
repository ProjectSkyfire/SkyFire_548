/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRESERVER_CORPSE_H
#define SKYFIRESERVER_CORPSE_H

#include "DatabaseEnv.h"
#include "GridDefines.h"
#include "LootMgr.h"
#include "Object.h"

enum class CorpseType
{
    CORPSE_BONES = 0,
    CORPSE_RESURRECTABLE_PVE = 1,
    CORPSE_RESURRECTABLE_PVP = 2,
    MAX_CORPSE_TYPE = 3
};

// Value equal client resurrection dialog show radius.
#define CORPSE_RECLAIM_RADIUS 39

enum CorpseFlags
{
    CORPSE_FLAG_NONE = 0x00,
    CORPSE_FLAG_BONES = 0x01,
    CORPSE_FLAG_UNK1 = 0x02,
    CORPSE_FLAG_UNK2 = 0x04,
    CORPSE_FLAG_HIDE_HELM = 0x08,
    CORPSE_FLAG_HIDE_CLOAK = 0x10,
    CORPSE_FLAG_LOOTABLE = 0x20
};

class Corpse : public WorldObject, public GridObject<Corpse>
{
public:
    explicit Corpse(CorpseType type = CorpseType::CORPSE_BONES);
    ~Corpse() { }

    void AddToWorld();
    void RemoveFromWorld();

    bool Create(uint32 guidlow, Map* map);
    bool Create(uint32 guidlow, Player* owner);

    void SaveToDB();
    bool LoadCorpseFromDB(uint32 guid, Field* fields);

    void DeleteBonesFromWorld();
    void DeleteFromDB(SQLTransaction& trans);

    uint64 GetOwnerGUID() const { return GetUInt64Value(CORPSE_FIELD_OWNER); }

    time_t const& GetGhostTime() const { return m_time; }
    void ResetGhostTime() { m_time = time(NULL); }
    CorpseType GetType() const { return m_type; }

    GridCoord const& GetGridCoord() const { return _gridCoord; }
    void SetGridCoord(GridCoord const& gridCoord) { _gridCoord = gridCoord; }

    Loot loot;                                          // remove insignia ONLY at BG
    Player* lootRecipient;
    bool lootForBody;
    bool IsExpired(time_t t) const;

private:
    CorpseType m_type;
    time_t m_time;
    GridCoord _gridCoord;                                    // gride for corpse position for fast search
};
#endif
