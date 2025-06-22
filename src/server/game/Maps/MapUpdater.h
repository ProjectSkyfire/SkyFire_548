/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_MAP_UPDATER_H_INCLUDED
#define SF_MAP_UPDATER_H_INCLUDED

#include <condition_variable>
#include <mutex>

#include "DelayExecutor.h"

class Map;

class MapUpdater
{
public:
    MapUpdater();
    virtual ~MapUpdater();
    friend class MapUpdateRequest;

    int schedule_update(Map& map, ACE_UINT32 diff);
    int wait();
    int activate(size_t num_threads);
    int deactivate();
    bool activated();

private:
    DelayExecutor m_executor;
    std::mutex Lock;
    std::condition_variable condition;
    size_t pending_requests;

    void update_finished();
};

#endif //_MAP_UPDATER_H_INCLUDED
