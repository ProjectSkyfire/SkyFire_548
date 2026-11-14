/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScenarioMgr.h"
#include "DBCStores.h"
#include "Log.h"
#include "SharedDefines.h"

#include <algorithm>

using namespace Skyfire::Scenarios;

ScenarioMgr* ScenarioMgr::instance()
{
    static ScenarioMgr mgr;
    return &mgr;
}

namespace
{
    // Depth limit guards against a cyclic parentID chain in a bad DBC.
    CriteriaNode BuildNode(uint32 treeId, std::multimap<uint32, uint32> const& children, uint32 depth)
    {
        CriteriaNode node;
        CriteriaTreeEntry const* tree = sCriteriaTreeStore.LookupEntry(treeId);
        if (!tree)
            return node;

        node.TreeId = treeId;
        node.CriteriaId = tree->criteriaID;
        node.Amount = tree->criteriaCount;
        node.Operator = tree->op;
        if (CriteriaEntry const* criteria = sCriteriaStore.LookupEntry(tree->criteriaID))
        {
            node.CriteriaType = criteria->type;
            node.Asset = criteria->raw.criteriaArg1;
        }

        if (depth < 8)
        {
            auto range = children.equal_range(treeId);
            for (auto it = range.first; it != range.second; ++it)
                node.Children.push_back(BuildNode(it->second, children, depth + 1));
        }
        return node;
    }
}

void ScenarioMgr::Load()
{
    _scenarios.clear();

    std::multimap<uint32, uint32> children;
    for (uint32 i = 0; i < sCriteriaTreeStore.GetNumRows(); ++i)
        if (CriteriaTreeEntry const* tree = sCriteriaTreeStore.LookupEntry(i))
            if (tree->parentID)
                children.insert({ tree->parentID, tree->ID });

    for (uint32 i = 0; i < sScenarioStepStore.GetNumRows(); ++i)
    {
        ScenarioStepEntry const* step = sScenarioStepStore.LookupEntry(i);
        if (!step || !sScenarioStore.LookupEntry(step->ScenarioID))
            continue;

        ScenarioData& data = _scenarios[step->ScenarioID];
        data.ScenarioId = step->ScenarioID;

        StepData stepData;
        stepData.Step = step->Step;
        stepData.Bonus = step->IsBonusObjective();
        stepData.Tree = BuildNode(step->CriteriaTreeID, children, 0);
        data.Steps.push_back(stepData);
    }

    for (auto& pair : _scenarios)
        std::sort(pair.second.Steps.begin(), pair.second.Steps.end(),
            [](StepData const& a, StepData const& b) { return a.Step < b.Step; });

    SF_LOG_INFO("server.loading", ">> Loaded %u scenarios with step trees", uint32(_scenarios.size()));
}

ScenarioData const* ScenarioMgr::GetData(uint32 scenarioId) const
{
    auto it = _scenarios.find(scenarioId);
    return it != _scenarios.end() ? &it->second : nullptr;
}

uint32 ScenarioMgr::ResolveScenarioId(uint32 mapId, uint32 difficulty, uint32 teamId, uint32 queuedDungeonId) const
{
    // LfgDungeons faction: -1 both, 0 Horde, 1 Alliance.
    int32 const faction = teamId == TEAM_ALLIANCE ? 1 : 0;

    if (LFGDungeonEntry const* queued = sLFGDungeonStore.LookupEntry(queuedDungeonId))
        if (queued->m_ScenarioID && queued->m_ContinentID == int32(mapId) && queued->m_DifficultyID == difficulty)
            return queued->m_ScenarioID;

    uint32 found = 0;
    for (uint32 i = 0; i < sLFGDungeonStore.GetNumRows(); ++i)
    {
        LFGDungeonEntry const* row = sLFGDungeonStore.LookupEntry(i);
        if (!row || !row->m_ScenarioID || row->m_ContinentID != int32(mapId) || row->m_DifficultyID != difficulty)
            continue;
        if (row->m_Faction != -1 && row->m_Faction != faction)
            continue;
        if (found && found != row->m_ScenarioID)
            return 0;
        found = row->m_ScenarioID;
    }
    return found;
}
