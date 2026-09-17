/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/** \file
    \ingroup Skyfired
*/

#include "BattlegroundMgr.h"
#include "Common.h"
#include "Database/DatabaseEnv.h"
#include "Errors.h"
#include "MapManager.h"
#include "ObjectAccessor.h"
#include "OutdoorPvPMgr.h"
#include "Platform/TimeUtils.h"
#include "ScriptMgr.h"
#include "Timer.h"
#include "World.h"
#include "WorldRunnable.h"
#include "WorldShutdownLifecycle.h"
#include "WorldSocketMgr.h"

std::atomic<uint64> HubWorldMetrics{0};
std::atomic<uint64> HubWorldTick{0};

#define WORLD_SLEEP_CONST 50

#ifdef _WIN32
#include "ServiceWin32.h"
extern int m_ServiceStatus;
#endif

namespace
{
    void AdvanceShutdownStep(Skyfire::WorldShutdown::WorldShutdownStep& current,
        Skyfire::WorldShutdown::WorldShutdownStep next)
    {
        ASSERT(Skyfire::WorldShutdown::IsExpectedShutdownTransition(current, next));
        current = next;
    }
}

/// Heartbeat for the World
void WorldRunnable::Run()
{
    uint32 realCurrTime = 0;
    uint32 realPrevTime = getMSTime();

    uint32 prevSleepTime = 0;                               // used for balanced full tick time length near WORLD_SLEEP_CONST

    sScriptMgr->OnStartup();

    ///- While we have not World::m_stopEvent, update the world
    while (!World::IsStopped())
    {
        ++World::m_worldLoopCounter;
        realCurrTime = getMSTime();

        uint32 diff = getMSTimeDiff(realPrevTime, realCurrTime);

        sWorld->Update(diff);
        uint32 const workTime = getMSTimeDiff(realCurrTime, getMSTime());
        HubWorldMetrics.store((uint64(sWorld->GetPlayerCount()) << 32) | workTime, std::memory_order_relaxed);
        HubWorldTick.fetch_add(1, std::memory_order_relaxed);
        realPrevTime = realCurrTime;

        // diff (D0) include time of previous sleep (d0) + tick time (t0)
        // we want that next d1 + t1 == WORLD_SLEEP_CONST
        // we can't know next t1 and then can use (t0 + d1) == WORLD_SLEEP_CONST requirement
        // d1 = WORLD_SLEEP_CONST - t0 = WORLD_SLEEP_CONST - (D0 - d0) = WORLD_SLEEP_CONST + d0 - D0
        if (diff <= WORLD_SLEEP_CONST + prevSleepTime)
        {
            prevSleepTime = WORLD_SLEEP_CONST + prevSleepTime - diff;
            Skyfire::SleepForMilliseconds(prevSleepTime);
        }
        else
            prevSleepTime = 0;

#ifdef _WIN32
        if (m_ServiceStatus == 0)
            World::StopNow(SHUTDOWN_EXIT_CODE);

        while (m_ServiceStatus == 2)
            Skyfire::SleepForMilliseconds(1000);
#endif
    }

    Skyfire::WorldShutdown::WorldShutdownStep shutdownStep = Skyfire::WorldShutdown::WORLD_SHUTDOWN_START;

    AdvanceShutdownStep(shutdownStep, Skyfire::WorldShutdown::WORLD_SHUTDOWN_SCRIPT_SHUTDOWN);
    sScriptMgr->OnShutdown();

    // Players must be kicked and sessions updated before maps can be unloaded.
    AdvanceShutdownStep(shutdownStep, Skyfire::WorldShutdown::WORLD_SHUTDOWN_KICK_PLAYERS);
    sWorld->KickAll();                                       // save and kick all players
    AdvanceShutdownStep(shutdownStep, Skyfire::WorldShutdown::WORLD_SHUTDOWN_UPDATE_SESSIONS);
    sWorld->UpdateSessions(1);                             // real players unload required UpdateSessions call

    // Battleground and network teardown must finish before map/object storage is destroyed.
    AdvanceShutdownStep(shutdownStep, Skyfire::WorldShutdown::WORLD_SHUTDOWN_DELETE_BATTLEGROUNDS);
    sBattlegroundMgr->DeleteAllBattlegrounds();
    AdvanceShutdownStep(shutdownStep, Skyfire::WorldShutdown::WORLD_SHUTDOWN_STOP_NETWORK);
    sWorldSocketMgr->StopNetwork();

    // Maps own live world objects; ObjectAccessor corpse storage is unloaded after maps.
    AdvanceShutdownStep(shutdownStep, Skyfire::WorldShutdown::WORLD_SHUTDOWN_UNLOAD_MAPS);
    sMapMgr->UnloadAll();                     // unload all grids (including locked in memory)
    AdvanceShutdownStep(shutdownStep, Skyfire::WorldShutdown::WORLD_SHUTDOWN_UNLOAD_OBJECT_ACCESSOR);
    sObjectAccessor->UnloadAll();             // unload 'i_player2corpse' storage and remove from world
    AdvanceShutdownStep(shutdownStep, Skyfire::WorldShutdown::WORLD_SHUTDOWN_UNLOAD_SCRIPTS);
    sScriptMgr->Unload();
    AdvanceShutdownStep(shutdownStep, Skyfire::WorldShutdown::WORLD_SHUTDOWN_OUTDOOR_PVP_DIE);
    sOutdoorPvPMgr->Die();
    AdvanceShutdownStep(shutdownStep, Skyfire::WorldShutdown::WORLD_SHUTDOWN_COMPLETE);
}
