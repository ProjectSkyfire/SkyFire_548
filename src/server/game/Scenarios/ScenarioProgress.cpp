/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScenarioProgress.h"

#include <algorithm>
#include <set>

namespace Skyfire::Scenarios
{
    namespace
    {
        // CriteriaTreeOperator values (DBCEnums.h), repeated here so this file
        // needs no client data headers.
        uint32 const OPERATOR_COMPLETE = 0;
        uint32 const OPERATOR_NOT_COMPLETE = 1;
        uint32 const OPERATOR_COMPLETE_ALL = 4;
        uint32 const OPERATOR_SUM = 5;
        uint32 const OPERATOR_HIGHEST = 6;
        uint32 const OPERATOR_STARTED_AT_LEAST = 7;
        uint32 const OPERATOR_COMPLETE_AT_LEAST = 8;
    }

    ScenarioProgress::ScenarioProgress(ScenarioData const& data) : _data(data), _stepIndex(0), _complete(false)
    {
        std::stable_sort(_data.Steps.begin(), _data.Steps.end(),
            [](StepData const& a, StepData const& b) { return a.Step < b.Step; });

        // the first main step; a scenario without one has nothing to track
        while (_stepIndex < _data.Steps.size() && _data.Steps[_stepIndex].Bonus)
            ++_stepIndex;

        if (_stepIndex >= _data.Steps.size())
            _complete = true;
    }

    int32 ScenarioProgress::GetCurrentStep() const
    {
        if (!_complete)
            return int32(_data.Steps[_stepIndex].Step);

        int32 last = -1;
        for (StepData const& step : _data.Steps)
            if (!step.Bonus)
                last = std::max(last, int32(step.Step));

        return last + 1;
    }

    StepData const* ScenarioProgress::CurrentStep() const
    {
        return _complete ? nullptr : &_data.Steps[_stepIndex];
    }

    void ScenarioProgress::CollectLeaves(CriteriaNode const& node, std::vector<CriteriaNode const*>& leaves) const
    {
        if (node.CriteriaId)
            leaves.push_back(&node);

        for (CriteriaNode const& child : node.Children)
            CollectLeaves(child, leaves);
    }

    std::vector<uint32> ScenarioProgress::UpdateCriteria(uint32 criteriaType, uint32 asset, uint64 amount)
    {
        std::vector<uint32> changed;
        StepData const* step = CurrentStep();
        if (!step || !amount)
            return changed;

        std::vector<CriteriaNode const*> leaves;
        CollectLeaves(step->Tree, leaves);

        // one event counts once per criteria, however many leaves carry it
        std::set<uint32> matching;
        for (CriteriaNode const* leaf : leaves)
            if (leaf->CriteriaType == criteriaType && leaf->Asset == asset)
                matching.insert(leaf->CriteriaId);

        for (uint32 criteriaId : matching)
            if (AddCriteriaProgress(criteriaId, amount))
                changed.push_back(criteriaId);

        return changed;
    }

    bool ScenarioProgress::AddCriteriaProgress(uint32 criteriaId, uint64 amount)
    {
        StepData const* step = CurrentStep();
        if (!step || !criteriaId || !amount)
            return false;

        std::vector<CriteriaNode const*> leaves;
        CollectLeaves(step->Tree, leaves);

        // the same criteria can sit in several leaves; count up to the largest amount
        uint64 cap = 0;
        bool found = false;
        for (CriteriaNode const* leaf : leaves)
        {
            if (leaf->CriteriaId != criteriaId)
                continue;

            found = true;
            cap = std::max(cap, leaf->Amount ? leaf->Amount : 1);
        }

        if (!found)
            return false;

        uint64& count = _counts[criteriaId];
        if (count >= cap)
            return false;

        count = amount >= cap - count ? cap : count + amount;
        return true;
    }

    uint64 ScenarioProgress::NodeCount(CriteriaNode const& node) const
    {
        std::set<uint32> counted;
        return NodeCount(node, counted);
    }

    // a criteria that sits in several leaves adds its count once
    uint64 ScenarioProgress::NodeCount(CriteriaNode const& node, std::set<uint32>& counted) const
    {
        if (node.CriteriaId)
            return counted.insert(node.CriteriaId).second ? GetCount(node.CriteriaId) : 0;

        uint64 sum = 0;
        for (CriteriaNode const& child : node.Children)
            sum += NodeCount(child, counted);

        return sum;
    }

    bool ScenarioProgress::IsNodeDone(CriteriaNode const& node) const
    {
        if (node.CriteriaId)
        {
            // A leaf with no amount is a hidden marker, and a "not complete"
            // leaf is a fail condition this tracker does not model; neither
            // holds a step back.
            if (!node.Amount || node.Operator == OPERATOR_NOT_COMPLETE)
                return true;

            return GetCount(node.CriteriaId) >= node.Amount;
        }

        if (node.Children.empty())
            return true;

        switch (node.Operator)
        {
            case OPERATOR_SUM:
                return NodeCount(node) >= node.Amount;
            case OPERATOR_HIGHEST:
            {
                uint64 highest = 0;
                for (CriteriaNode const& child : node.Children)
                    highest = std::max(highest, NodeCount(child));
                return highest >= node.Amount;
            }
            case OPERATOR_STARTED_AT_LEAST:
            case OPERATOR_COMPLETE_AT_LEAST:
            {
                uint64 matching = 0;
                for (CriteriaNode const& child : node.Children)
                    if (node.Operator == OPERATOR_STARTED_AT_LEAST ? NodeCount(child) > 0 : IsNodeDone(child))
                        ++matching;
                return matching >= node.Amount;
            }
            case OPERATOR_COMPLETE:
            case OPERATOR_COMPLETE_ALL:
            default:
                // complete-all ignores the node's amount, as the 5.0 step trees
                // show (Arena of Annihilation's last step: amount 1, two objectives)
                for (CriteriaNode const& child : node.Children)
                    if (!IsNodeDone(child))
                        return false;
                return true;
        }
    }

    bool ScenarioProgress::IsCurrentStepDone() const
    {
        StepData const* step = CurrentStep();
        return step && IsNodeDone(step->Tree);
    }

    void ScenarioProgress::EnterStep(size_t index)
    {
        _counts.clear();
        _stepIndex = index;
        _complete = false;
    }

    bool ScenarioProgress::AdvanceIfStepDone()
    {
        bool changed = false;
        while (IsCurrentStepDone())
        {
            size_t next = _stepIndex + 1;
            while (next < _data.Steps.size() && _data.Steps[next].Bonus)
                ++next;

            if (next >= _data.Steps.size())
            {
                Complete();
                return true;
            }

            EnterStep(next);
            changed = true;
        }

        return changed;
    }

    bool ScenarioProgress::SetStep(uint32 step)
    {
        for (size_t i = 0; i < _data.Steps.size(); ++i)
        {
            if (_data.Steps[i].Step == step && !_data.Steps[i].Bonus)
            {
                EnterStep(i);
                return true;
            }
        }

        return false;
    }

    void ScenarioProgress::Complete()
    {
        _counts.clear();
        _complete = true;
    }

    uint64 ScenarioProgress::GetCount(uint32 criteriaId) const
    {
        auto itr = _counts.find(criteriaId);
        return itr != _counts.end() ? itr->second : 0;
    }

    CriteriaCounts ScenarioProgress::GetCurrentCounts() const
    {
        CriteriaCounts counts;
        for (auto const& count : _counts)
            if (count.second)
                counts.emplace_back(count.first, count.second);

        return counts;
    }
}
