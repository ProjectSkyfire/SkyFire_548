/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef ScenarioMgr_h__
#define ScenarioMgr_h__

#include "Common.h"
#include "ScenarioProgress.h"

#include <map>

// Builds the step trees of every scenario from Scenario.dbc, ScenarioStep.dbc,
// CriteriaTree.dbc and Criteria.dbc once at startup, and resolves which
// scenario a map instance runs.
class ScenarioMgr
{
public:
    static ScenarioMgr* instance();

    void Load();

    Skyfire::Scenarios::ScenarioData const* GetData(uint32 scenarioId) const;

    // Scenario of the LfgDungeons row the group queued for when it fits the
    // map, else the only scenario of that map, difficulty and faction.
    // 0 if none or ambiguous. teamId is TEAM_ALLIANCE or TEAM_HORDE.
    uint32 ResolveScenarioId(uint32 mapId, uint32 difficulty, uint32 teamId, uint32 queuedDungeonId) const;

private:
    std::map<uint32, Skyfire::Scenarios::ScenarioData> _scenarios;
};

#define sScenarioMgr ScenarioMgr::instance()

#endif // ScenarioMgr_h__
