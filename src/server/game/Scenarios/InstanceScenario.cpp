/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "InstanceScenario.h"
#include "GroupMgr.h"
#include "LFGMgr.h"
#include "Log.h"
#include "Map.h"
#include "Player.h"
#include "ScenarioPackets.h"
#include "WorldSession.h"

using namespace Skyfire;
using namespace Skyfire::Scenarios;

namespace
{
    // Criteria type 0, ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE.
    uint32 const CRITERIA_TYPE_KILL_CREATURE = 0;
}

InstanceScenario::InstanceScenario(Map* map, ScenarioData const& data, uint32 dungeonId, uint64 groupGuid)
    : _map(map), _progress(data), _dungeonId(dungeonId), _groupGuid(groupGuid), _startTime(time(NULL)), _finished(false)
{
}

void InstanceScenario::SendState(Player* player) const
{
    ScenarioPackets::ScenarioState state;
    state.ScenarioId = _progress.GetScenarioId();
    state.CurrentStep = _progress.GetCurrentStep();

    uint32 const elapsed = uint32(time(NULL) - _startTime);
    for (auto const& count : _progress.GetCurrentCounts())
    {
        ScenarioPackets::CriteriaProgress entry;
        entry.CriteriaId = count.first;
        entry.Quantity = count.second;
        entry.PlayerGuid = player->GetGUID();
        entry.Date = time(NULL);
        entry.TimeFromStart = elapsed;
        entry.TimeFromCreate = elapsed;
        state.Progress.push_back(entry);
    }

    WorldPacket data = ScenarioPackets::BuildScenarioState(state);
    player->GetSession()->SendPacket(&data);
}

void InstanceScenario::SendStateToAll() const
{
    for (Map::PlayerList::const_iterator itr = _map->GetPlayers().begin(); itr != _map->GetPlayers().end(); ++itr)
        if (Player* player = itr->GetSource())
            SendState(player);
}

void InstanceScenario::SendProgress(uint32 criteriaId) const
{
    uint32 const elapsed = uint32(time(NULL) - _startTime);
    for (Map::PlayerList::const_iterator itr = _map->GetPlayers().begin(); itr != _map->GetPlayers().end(); ++itr)
    {
        Player* player = itr->GetSource();
        if (!player)
            continue;

        ScenarioPackets::CriteriaProgress entry;
        entry.CriteriaId = criteriaId;
        entry.Quantity = _progress.GetCount(criteriaId);
        entry.PlayerGuid = player->GetGUID();
        entry.Date = time(NULL);
        entry.TimeFromStart = elapsed;
        entry.TimeFromCreate = elapsed;

        WorldPacket data = ScenarioPackets::BuildScenarioProgressUpdate(entry);
        player->GetSession()->SendPacket(&data);
    }
}

void InstanceScenario::OnCreatureKilled(uint32 entry)
{
    AfterChange(_progress.UpdateCriteria(CRITERIA_TYPE_KILL_CREATURE, entry, 1));
}

void InstanceScenario::AddCriteria(uint32 criteriaId, uint64 amount)
{
    std::vector<uint32> changed;
    if (_progress.AddCriteriaProgress(criteriaId, amount))
        changed.push_back(criteriaId);
    AfterChange(changed);
}

void InstanceScenario::ForceStep(uint32 step)
{
    if (_progress.SetStep(step))
    {
        SendStateToAll();
        if (_progress.IsComplete())
            Finish();
    }
}

void InstanceScenario::Complete()
{
    _progress.Complete();
    SendStateToAll();
    Finish();
}

void InstanceScenario::Start()
{
    // A first step whose tree is already satisfied (nothing to count) moves on
    // now instead of at the next unrelated kill.
    AfterChange(std::vector<uint32>(), true);

    if (_progress.IsComplete())
        Finish();
}

void InstanceScenario::AfterChange(std::vector<uint32> const& changed, bool force)
{
    // Only a count that changed can finish a step.
    if (changed.empty() && !force)
        return;

    for (uint32 criteriaId : changed)
        SendProgress(criteriaId);

    if (_progress.AdvanceIfStepDone())
    {
        SF_LOG_DEBUG("scenario", "Scenario %u in instance %u is now at step %d", _progress.GetScenarioId(), _map->GetInstanceId(), _progress.GetCurrentStep());
        SendStateToAll();
        if (_progress.IsComplete())
            Finish();
    }
}

void InstanceScenario::Finish()
{
    if (_finished)
        return;
    _finished = true;

    SF_LOG_INFO("scenario", "Scenario %u in instance %u completed", _progress.GetScenarioId(), _map->GetInstanceId());

    // A group that has since disbanded has no finder data left; asking the
    // finder about it would only create an empty entry.
    if (_dungeonId && _groupGuid && sGroupMgr->GetGroupByGUID(GUID_LOPART(_groupGuid)))
        sLFGMgr->FinishDungeon(_groupGuid, _dungeonId);
}
