/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef ScenarioProgress_h__
#define ScenarioProgress_h__

#include "Define.h"

#include <map>
#include <set>
#include <utility>
#include <vector>

// Step tracking of one running scenario, independent of the client data
// stores so it can be tested on hand-built trees. ScenarioMgr builds the
// ScenarioData from Scenario.dbc, ScenarioStep.dbc and the criteria trees.
namespace Skyfire::Scenarios
{
    // One CriteriaTree.dbc node. A leaf carries a criteria (CriteriaId != 0),
    // an inner node combines its children by Operator (CriteriaTreeOperator).
    struct CriteriaNode
    {
        uint32 TreeId = 0;
        uint32 CriteriaId = 0;
        uint32 CriteriaType = 0;        // Criteria.dbc type, e.g. 0 = kill creature
        uint32 Asset = 0;               // Criteria.dbc asset, e.g. the creature entry
        uint64 Amount = 0;
        uint32 Operator = 0;
        std::vector<CriteriaNode> Children;
    };

    struct StepData
    {
        uint32 Step = 0;                // ScenarioStep.dbc Step, starts at 0
        bool Bonus = false;             // bonus objective, never part of the main line
        CriteriaNode Tree;
    };

    struct ScenarioData
    {
        uint32 ScenarioId = 0;
        std::vector<StepData> Steps;    // ordered by Step
    };

    typedef std::vector<std::pair<uint32 /*criteriaId*/, uint64 /*count*/>> CriteriaCounts;

    class ScenarioProgress
    {
    public:
        explicit ScenarioProgress(ScenarioData const& data);

        uint32 GetScenarioId() const { return _data.ScenarioId; }

        // Step number to send to the client: the current main step, or one past
        // the last main step once the scenario is complete.
        int32 GetCurrentStep() const;
        bool IsComplete() const { return _complete; }

        // Counts an event against the current step's leaves of that type and
        // asset. Returns the criteria whose count changed.
        std::vector<uint32> UpdateCriteria(uint32 criteriaType, uint32 asset, uint64 amount);

        // Counts a criteria of the current step by id; false if the current
        // step has no such criteria or it is already at its amount.
        bool AddCriteriaProgress(uint32 criteriaId, uint64 amount);

        // Moves on while the current step is done. Returns true if the step
        // changed or the scenario completed.
        bool AdvanceIfStepDone();

        // Script overrides: jump to a main step (counts start at zero) or end.
        bool SetStep(uint32 step);
        void Complete();

        uint64 GetCount(uint32 criteriaId) const;
        CriteriaCounts GetCurrentCounts() const;
        bool IsCurrentStepDone() const;

    private:
        StepData const* CurrentStep() const;
        bool IsNodeDone(CriteriaNode const& node) const;
        uint64 NodeCount(CriteriaNode const& node) const;
        uint64 NodeCount(CriteriaNode const& node, std::set<uint32>& counted) const;
        void CollectLeaves(CriteriaNode const& node, std::vector<CriteriaNode const*>& leaves) const;
        void EnterStep(size_t index);

        ScenarioData _data;
        size_t _stepIndex;              // index into _data.Steps
        bool _complete;
        std::map<uint32, uint64> _counts; // current step only
    };
}

#endif // ScenarioProgress_h__
