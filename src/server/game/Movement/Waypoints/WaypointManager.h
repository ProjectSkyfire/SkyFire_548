/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_WAYPOINTMANAGER_H
#define SKYFIRE_WAYPOINTMANAGER_H

#include "Platform/Singleton.h"
#include <vector>
#include <cmath>

struct WaypointData
{
    uint32 id;
    float x, y, z, orientation;
    uint32 delay;
    uint32 event_id;
    bool run;
    bool smoothCyclic = false;
    uint8 event_chance;

    void SetMoveFlags(uint8 flags)
    {
        // Explicit opt-in; preserve the historic boolean meaning of other values.
        smoothCyclic = flags == 64 || flags == 65;
        run = smoothCyclic ? (flags & 1) != 0 : flags != 0;
    }
};

typedef std::vector<WaypointData*> WaypointPath;

inline bool IsSmoothCyclicWaypointPath(WaypointPath const& path)
{
    if (path.size() < 4)
        return false;
    for (size_t i = 0; i < path.size(); ++i)
    {
        WaypointData const* node = path[i];
        WaypointData const* next = path[(i + 1) % path.size()];
        if (!node || !next || !node->smoothCyclic || node->run != path.front()->run ||
            node->delay || node->event_id || !std::isfinite(node->x) || !std::isfinite(node->y) || !std::isfinite(node->z) ||
            (node->x == next->x && node->y == next->y && node->z == next->z))
            return false;
    }
    return true;
}
typedef UNORDERED_MAP<uint32, WaypointPath> WaypointPathContainer;

class WaypointMgr
{
    friend class Skyfire::Singleton<WaypointMgr, Skyfire::NullMutex>;

public:
    // Attempts to reload a single path from database
    void ReloadPath(uint32 id);

    // Loads all paths from database, should only run on startup
    void Load();

    // Returns the path from a given id
    WaypointPath const* GetPath(uint32 id) const
    {
        WaypointPathContainer::const_iterator itr = _waypointStore.find(id);
        if (itr != _waypointStore.end())
            return &itr->second;

        return NULL;
    }

private:
    // Only allow instantiation from the singleton wrapper.
    WaypointMgr();
    ~WaypointMgr();

    WaypointPathContainer _waypointStore;
};

#define sWaypointMgr Skyfire::Singleton<WaypointMgr, Skyfire::NullMutex>::instance()

#endif
