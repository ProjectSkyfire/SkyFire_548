/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "GridStates.h"
#include "GridNotifiers.h"
#include "Log.h"

#ifdef SKYFIRE_DEBUG
bool GridState::checkMagic()
{
    if (i_Magic != MAGIC_TESTVAL)
    {
        SF_LOG_ERROR("misc", "!!! GridState: Magic value gone !!!");
        return false;
    }
    return true;
}
#endif

void InvalidState::Update(Map&, NGridType&, GridInfo&, uint32) const
{ }

void ActiveState::Update(Map& map, NGridType& grid, GridInfo&  info, uint32 diff) const
{
    // Only check grid activity every (grid_expiry/10) ms, because it's really useless to do it every cycle
    info.UpdateTimeTracker(diff);
    if (info.getTimeTracker().Passed())
    {
        if (!grid.GetWorldObjectCountInNGrid<Player>() && !map.ActiveObjectsNearGrid(grid))
        {
            ObjectGridStoper worker;
            TypeContainerVisitor<ObjectGridStoper, GridTypeMapContainer> visitor(worker);
            grid.VisitAllGrids(visitor);
            grid.SetGridState(GRID_STATE_IDLE);
            SF_LOG_DEBUG("maps", "Grid[%u, %u] on map %u moved to IDLE state", grid.getX(), grid.getY(), map.GetId());
        }
        else
            map.ResetGridExpiry(grid, 0.1f);
    }
}

void IdleState::Update(Map& map, NGridType& grid, GridInfo&, uint32) const
{
    map.ResetGridExpiry(grid);
    grid.SetGridState(GRID_STATE_REMOVAL);
    SF_LOG_DEBUG("maps", "Grid[%u, %u] on map %u moved to REMOVAL state", grid.getX(), grid.getY(), map.GetId());
}

void RemovalState::Update(Map& map, NGridType& grid, GridInfo& info, uint32 diff) const
{
    if (!info.getUnloadLock())
    {
        info.UpdateTimeTracker(diff);
        if (info.getTimeTracker().Passed() && !map.UnloadGrid(grid, false))
        {
            SF_LOG_DEBUG("maps", "Grid[%u, %u] for map %u differed unloading due to players or active objects nearby", grid.getX(), grid.getY(), map.GetId());
            map.ResetGridExpiry(grid);
        }
    }
}
