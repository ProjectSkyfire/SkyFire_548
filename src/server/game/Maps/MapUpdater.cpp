/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "MapUpdater.h"
#include "DelayExecutor.h"
#include "Map.h"
#include "DatabaseEnv.h"

#include <ace/Method_Request.h>

class WDBThreadStartReq1 : public ACE_Method_Request
{
public:
    WDBThreadStartReq1() { }

    virtual int call() { return 0; }
};

class WDBThreadEndReq1 : public ACE_Method_Request
{
public:
    WDBThreadEndReq1() { }

    virtual int call() { return 0; }
};

class MapUpdateRequest : public ACE_Method_Request
{
private:
    Map& m_map;
    MapUpdater& m_updater;
    ACE_UINT32 m_diff;

public:
    MapUpdateRequest(Map& m, MapUpdater& u, ACE_UINT32 d)
        : m_map(m), m_updater(u), m_diff(d) { }

    virtual int call()
    {
        m_map.Update(m_diff);
        m_updater.update_finished();
        return 0;
    }
};

MapUpdater::MapUpdater():
m_executor(), pending_requests(0) { }

MapUpdater::~MapUpdater()
{
    deactivate();
}

int MapUpdater::activate(size_t num_threads)
{
    return m_executor.start((int)num_threads, new WDBThreadStartReq1, new WDBThreadEndReq1);
}

int MapUpdater::deactivate()
{
    wait();
    return m_executor.deactivate();
}

int MapUpdater::wait()
{
    std::unique_lock<std::mutex> ulock(Lock);

    while (pending_requests > 0)
        condition.wait(ulock);

    ulock.unlock();

    return 0;
}

int MapUpdater::schedule_update(Map& map, ACE_UINT32 diff)
{
    std::lock_guard<std::mutex> guard(Lock);

    ++pending_requests;

    if (m_executor.execute(new MapUpdateRequest(map, *this, diff)) == -1)
    {
        ACE_DEBUG((LM_ERROR, ACE_TEXT("(%t) \n"), ACE_TEXT("Failed to schedule Map Update")));

        --pending_requests;
        return -1;
    }

    return 0;
}

bool MapUpdater::activated()
{
    return m_executor.activated();
}

void MapUpdater::update_finished()
{
    std::lock_guard<std::mutex> guard(Lock);

    if (pending_requests == 0)
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("(%t)\n"), ACE_TEXT("MapUpdater::update_finished BUG, report to devs")));
        return;
    }

    --pending_requests;

    condition.notify_all();
}
