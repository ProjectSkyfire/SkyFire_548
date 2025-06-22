/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_MAP_INSTANCED_H
#define SKYFIRE_MAP_INSTANCED_H

#include "DBCEnums.h"
#include "InstanceSaveMgr.h"
#include "Map.h"

class MapInstanced : public Map
{
    friend class MapManager;
public:
    typedef UNORDERED_MAP< uint32, Map*> InstancedMaps;

    MapInstanced(uint32 id, time_t expiry);
    ~MapInstanced() { }

    // functions overwrite Map versions
    void Update(const uint32) OVERRIDE;
    void DelayedUpdate(const uint32 diff) OVERRIDE;
    //void RelocationNotify();
    void UnloadAll() OVERRIDE;
    bool CanEnter(Player* player) OVERRIDE;

    Map* CreateInstanceForPlayer(const uint32 mapId, Player* player);
    Map* FindInstanceMap(uint32 instanceId) const
    {
        InstancedMaps::const_iterator i = m_InstancedMaps.find(instanceId);
        return (i == m_InstancedMaps.end() ? NULL : i->second);
    }
    bool DestroyInstance(InstancedMaps::iterator& itr);

    void AddGridMapReference(const GridCoord& p)
    {
        ++GridMapReference[p.x_coord][p.y_coord];
        SetUnloadReferenceLock(GridCoord(63 - p.x_coord, 63 - p.y_coord), true);
    }

    void RemoveGridMapReference(GridCoord const& p)
    {
        --GridMapReference[p.x_coord][p.y_coord];
        if (!GridMapReference[p.x_coord][p.y_coord])
            SetUnloadReferenceLock(GridCoord(63 - p.x_coord, 63 - p.y_coord), false);
    }

    InstancedMaps& GetInstancedMaps() { return m_InstancedMaps; }
    virtual void InitVisibilityDistance() OVERRIDE;

private:
    InstanceMap* CreateInstance(uint32 InstanceId, InstanceSave* save, DifficultyID difficulty);
    BattlegroundMap* CreateBattleground(uint32 InstanceId, Battleground* bg);

    InstancedMaps m_InstancedMaps;

    uint16 GridMapReference[MAX_NUMBER_OF_GRIDS][MAX_NUMBER_OF_GRIDS];
};
#endif
