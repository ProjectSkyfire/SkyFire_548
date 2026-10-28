/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_SCENARIOMGR_H
#define SKYFIRE_SCENARIOMGR_H

#include "Common.h"

class Player;

class ScenarioMgr
{
private:
    ScenarioMgr() { }
    ~ScenarioMgr() { }

public:
    static ScenarioMgr* Instance()
    {
        static ScenarioMgr instance;
        return &instance;
    }

    uint32 GetScenarioIdForMap(uint32 map) const;
    uint32 GetCriteriaIdForScenarioStep(uint32 map, uint32 step) const;

    // Maps hosting several scenarios (Isle of Thunder) need the criteria tree and scenario passed explicitly.
    void SendScenarioState(Player* player, uint32 map, int32 step, uint32 startTime, uint32 miscCriteriaTreeId = 0, uint32 scenarioId = 0, uint32 bonusData = 0);
};

#define sScenarioMgr ScenarioMgr::Instance()

#endif
