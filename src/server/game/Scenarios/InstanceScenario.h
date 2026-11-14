/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef InstanceScenario_h__
#define InstanceScenario_h__

#include "Common.h"
#include "ScenarioProgress.h"

#include <ctime>

class InstanceScript;
class Map;
class Player;

// The scenario running in one instance map: owns the step tracking, tells the
// players in the map what changed and pays out the finder reward at the end.
class InstanceScenario
{
public:
    InstanceScenario(Map* map, Skyfire::Scenarios::ScenarioData const& data, uint32 dungeonId, uint64 groupGuid);

    uint32 GetScenarioId() const { return _progress.GetScenarioId(); }
    Skyfire::Scenarios::ScenarioProgress const& GetProgress() const { return _progress; }

    // Called once after creation, before the first player gets the state.
    void Start();

    // Full state with every count of the current step, for one player.
    void SendState(Player* player) const;

    // A creature of this entry died in the map.
    void OnCreatureKilled(uint32 entry);

    // Counts a criteria of the current step by id, for objectives the core
    // cannot see itself (script events, objects, spells).
    void AddCriteria(uint32 criteriaId, uint64 amount = 1);

    // Script overrides.
    void ForceStep(uint32 step);
    void Complete();

private:
    void AfterChange(std::vector<uint32> const& changed, bool force = false);
    void SendStateToAll() const;
    void SendProgress(uint32 criteriaId) const;
    void AfterStepChange();
    InstanceScript* GetScript() const;
    void Finish();

    Map* _map;
    Skyfire::Scenarios::ScenarioProgress _progress;
    uint32 _dungeonId;                  // LfgDungeons row the group queued for, 0 if none
    uint64 _groupGuid;                  // the LFG group that entered, kept so a later group change cannot lose the reward
    time_t _startTime;
    bool _finished;
};

#endif // InstanceScenario_h__
