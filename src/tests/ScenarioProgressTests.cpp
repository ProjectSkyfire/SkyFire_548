/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

// Step logic of a running scenario on hand-built criteria trees. The trees
// copy the shapes of the 5.4.8 client's 5.0 scenario steps (ScenarioStep.dbc,
// CriteriaTree.dbc, Criteria.dbc of build 18414).

#include "ScenarioProgress.h"

#include <iostream>
#include <vector>

using namespace Skyfire::Scenarios;

namespace
{
    uint32 const TYPE_KILL_CREATURE = 0;
    uint32 const TYPE_SCRIPT_EVENT = 73;
    uint32 const TYPE_USE_GAMEOBJECT = 28;

    bool Expect(bool condition, char const* message)
    {
        if (!condition)
            std::cerr << "FAIL: " << message << std::endl;

        return condition;
    }

    CriteriaNode Leaf(uint32 criteriaId, uint32 type, uint32 asset, uint64 amount, uint32 op = 0)
    {
        CriteriaNode node;
        node.CriteriaId = criteriaId;
        node.CriteriaType = type;
        node.Asset = asset;
        node.Amount = amount;
        node.Operator = op;
        return node;
    }

    CriteriaNode Tree(uint32 op, uint64 amount, std::vector<CriteriaNode> children)
    {
        CriteriaNode node;
        node.Operator = op;
        node.Amount = amount;
        node.Children = std::move(children);
        return node;
    }

    StepData Step(uint32 step, CriteriaNode tree, bool bonus = false)
    {
        StepData data;
        data.Step = step;
        data.Bonus = bonus;
        data.Tree = std::move(tree);
        return data;
    }

    // Arena of Annihilation (scenario 58): every step is "gong rung" (the same
    // criteria 20810 each time) plus "challenger slain"; the last root has amount 1.
    ScenarioData Arena()
    {
        ScenarioData data;
        data.ScenarioId = 58;
        data.Steps.push_back(Step(0, Tree(4, 0, { Leaf(20810, TYPE_SCRIPT_EVENT, 32548, 1), Leaf(19336, TYPE_KILL_CREATURE, 63311, 1) })));
        data.Steps.push_back(Step(1, Tree(4, 1, { Leaf(20810, TYPE_SCRIPT_EVENT, 32548, 1), Leaf(20587, TYPE_KILL_CREATURE, 64894, 1) })));
        return data;
    }

    bool TestStepNeedsEveryObjective()
    {
        ScenarioProgress progress(Arena());
        bool passed = Expect(progress.GetScenarioId() == 58 && progress.GetCurrentStep() == 0 && !progress.IsComplete(),
            "a new scenario starts at step 0");

        std::vector<uint32> changed = progress.UpdateCriteria(TYPE_KILL_CREATURE, 63311, 1);
        passed &= Expect(changed.size() == 1 && changed[0] == 19336, "killing Scar-Shell counts its criteria");
        passed &= Expect(!progress.AdvanceIfStepDone() && progress.GetCurrentStep() == 0,
            "the step waits for the gong as well");

        passed &= Expect(progress.AddCriteriaProgress(20810, 1), "a script counts the gong by criteria id");
        passed &= Expect(progress.AdvanceIfStepDone() && progress.GetCurrentStep() == 1, "both objectives move to step 1");
        passed &= Expect(progress.GetCount(20810) == 0 && progress.GetCurrentCounts().empty(),
            "the shared gong criteria starts again at zero in the new step");
        return passed;
    }

    bool TestLastStepCompletesIgnoringRootAmount()
    {
        ScenarioProgress progress(Arena());
        progress.SetStep(1);

        progress.AddCriteriaProgress(20810, 1);
        bool passed = Expect(!progress.AdvanceIfStepDone(),
            "complete-all needs both objectives even with a root amount of 1");

        progress.UpdateCriteria(TYPE_KILL_CREATURE, 64894, 1);
        passed &= Expect(progress.AdvanceIfStepDone() && progress.IsComplete(), "the last step completes the scenario");
        passed &= Expect(progress.GetCurrentStep() == 2, "a complete scenario reports one past its last step");
        passed &= Expect(progress.UpdateCriteria(TYPE_KILL_CREATURE, 64894, 1).empty(),
            "a complete scenario counts nothing");
        return passed;
    }

    bool TestCountsStopAtAmount()
    {
        ScenarioData data;
        data.ScenarioId = 40;
        data.Steps.push_back(Step(0, Tree(4, 0, { Leaf(19229, TYPE_USE_GAMEOBJECT, 122935, 6) })));
        data.Steps.push_back(Step(1, Tree(4, 0, { Leaf(19205, TYPE_KILL_CREATURE, 61156, 1) })));
        ScenarioProgress progress(data);

        progress.UpdateCriteria(TYPE_USE_GAMEOBJECT, 122935, 4);
        bool passed = Expect(progress.GetCount(19229) == 4 && !progress.AdvanceIfStepDone(), "4 of 6 keeps the step open");

        progress.UpdateCriteria(TYPE_USE_GAMEOBJECT, 122935, 5);
        passed &= Expect(progress.GetCount(19229) == 6, "the count stops at the amount");
        passed &= Expect(!progress.AddCriteriaProgress(19229, 1), "a full criteria reports no change");
        passed &= Expect(progress.UpdateCriteria(TYPE_KILL_CREATURE, 999, 1).empty(), "an unrelated kill counts nothing");
        passed &= Expect(!progress.AddCriteriaProgress(19205, 1), "a later step's criteria does not count early");
        passed &= Expect(progress.AdvanceIfStepDone() && progress.GetCurrentStep() == 1, "6 of 6 moves on");
        return passed;
    }

    bool TestBonusStepsAreSkipped()
    {
        // Crypt of Forgotten Kings (39): step 4 is a bonus objective after the last main step.
        ScenarioData data;
        data.ScenarioId = 39;
        data.Steps.push_back(Step(4, Tree(4, 0, { Leaf(23351, TYPE_KILL_CREATURE, 61707, 1) }), true));
        data.Steps.push_back(Step(3, Tree(4, 0, { Leaf(19225, TYPE_KILL_CREATURE, 61707, 1) })));
        ScenarioProgress progress(data);

        bool passed = Expect(progress.GetCurrentStep() == 3, "steps are ordered and bonus steps never start");
        passed &= Expect(!progress.SetStep(4), "a script cannot jump into a bonus step");
        progress.UpdateCriteria(TYPE_KILL_CREATURE, 61707, 1);
        passed &= Expect(progress.AdvanceIfStepDone() && progress.IsComplete() && progress.GetCurrentStep() == 4,
            "the last main step completes the scenario past the bonus step");
        return passed;
    }

    bool TestHiddenAndFailLeavesDoNotBlock()
    {
        // Unga Ingoo (15): an amount-0 hidden leaf and a "not complete" leaf next to the real objective.
        ScenarioData data;
        data.ScenarioId = 15;
        data.Steps.push_back(Step(0, Tree(4, 0, {
            Leaf(19210, TYPE_USE_GAMEOBJECT, 123201, 1),
            Leaf(19860, TYPE_USE_GAMEOBJECT, 125205, 0),
            Leaf(21895, 21, 21895, 0, 1) })));
        data.Steps.push_back(Step(1, Tree(4, 0, { Leaf(20696, TYPE_KILL_CREATURE, 62465, 1) })));
        ScenarioProgress progress(data);

        progress.UpdateCriteria(TYPE_USE_GAMEOBJECT, 123201, 1);
        return Expect(progress.AdvanceIfStepDone() && progress.GetCurrentStep() == 1,
            "only the counted objective decides the step");
    }

    bool TestAggregateOperators()
    {
        ScenarioData data;
        data.ScenarioId = 1;
        // sum: 5 kills of either creature
        data.Steps.push_back(Step(0, Tree(5, 5, { Leaf(1, TYPE_KILL_CREATURE, 100, 5), Leaf(2, TYPE_KILL_CREATURE, 200, 5) })));
        // at least 2 of 3 objectives done
        data.Steps.push_back(Step(1, Tree(8, 2, { Leaf(3, TYPE_KILL_CREATURE, 300, 1), Leaf(4, TYPE_KILL_CREATURE, 400, 1), Leaf(5, TYPE_KILL_CREATURE, 500, 1) })));
        // highest child reaches 3
        data.Steps.push_back(Step(2, Tree(6, 3, { Leaf(6, TYPE_KILL_CREATURE, 600, 3), Leaf(7, TYPE_KILL_CREATURE, 700, 3) })));
        ScenarioProgress progress(data);

        progress.UpdateCriteria(TYPE_KILL_CREATURE, 100, 3);
        progress.UpdateCriteria(TYPE_KILL_CREATURE, 200, 1);
        bool passed = Expect(!progress.AdvanceIfStepDone(), "sum: 4 of 5 keeps the step open");
        progress.UpdateCriteria(TYPE_KILL_CREATURE, 200, 1);
        passed &= Expect(progress.AdvanceIfStepDone() && progress.GetCurrentStep() == 1, "sum: 5 of 5 moves on");

        progress.UpdateCriteria(TYPE_KILL_CREATURE, 300, 1);
        passed &= Expect(!progress.AdvanceIfStepDone(), "at least: 1 of 2 keeps the step open");
        progress.UpdateCriteria(TYPE_KILL_CREATURE, 500, 1);
        passed &= Expect(progress.AdvanceIfStepDone() && progress.GetCurrentStep() == 2, "at least: 2 of 3 moves on");

        progress.UpdateCriteria(TYPE_KILL_CREATURE, 600, 2);
        progress.UpdateCriteria(TYPE_KILL_CREATURE, 700, 2);
        passed &= Expect(!progress.AdvanceIfStepDone(), "highest: 2 and 2 keep the step open");
        progress.UpdateCriteria(TYPE_KILL_CREATURE, 700, 1);
        passed &= Expect(progress.AdvanceIfStepDone() && progress.IsComplete(), "highest: 3 completes");
        return passed;
    }

    bool TestSharedCriteriaCountsOnce()
    {
        // the same criteria twice in one step: once counted, once a hidden marker
        ScenarioData data;
        data.ScenarioId = 2;
        data.Steps.push_back(Step(0, Tree(4, 0, { Leaf(9, TYPE_KILL_CREATURE, 100, 2), Leaf(9, TYPE_KILL_CREATURE, 100, 0) })));
        data.Steps.push_back(Step(1, Tree(5, 4, { Leaf(10, TYPE_KILL_CREATURE, 200, 4), Leaf(10, TYPE_KILL_CREATURE, 200, 4) })));
        ScenarioProgress progress(data);

        std::vector<uint32> changed = progress.UpdateCriteria(TYPE_KILL_CREATURE, 100, 1);
        bool passed = Expect(changed.size() == 1 && progress.GetCount(9) == 1, "one kill counts once for a criteria in two leaves");
        passed &= Expect(!progress.AdvanceIfStepDone(), "1 of 2 keeps the step open");
        progress.UpdateCriteria(TYPE_KILL_CREATURE, 100, 1);
        passed &= Expect(progress.AdvanceIfStepDone() && progress.GetCurrentStep() == 1, "2 of 2 moves on");

        progress.UpdateCriteria(TYPE_KILL_CREATURE, 200, 2);
        passed &= Expect(!progress.AdvanceIfStepDone(), "sum: a criteria in two leaves adds its count once");
        progress.UpdateCriteria(TYPE_KILL_CREATURE, 200, 2);
        passed &= Expect(progress.AdvanceIfStepDone() && progress.IsComplete(), "sum: 4 of 4 completes");
        return passed;
    }

    bool TestStartedAtLeast()
    {
        ScenarioData data;
        data.ScenarioId = 3;
        data.Steps.push_back(Step(0, Tree(7, 2, { Leaf(11, TYPE_KILL_CREATURE, 110, 5), Leaf(12, TYPE_KILL_CREATURE, 120, 5), Leaf(13, TYPE_KILL_CREATURE, 130, 5) })));
        ScenarioProgress progress(data);

        progress.UpdateCriteria(TYPE_KILL_CREATURE, 110, 1);
        bool passed = Expect(!progress.AdvanceIfStepDone(), "started at least: 1 of 2 started keeps the step open");
        progress.UpdateCriteria(TYPE_KILL_CREATURE, 130, 1);
        passed &= Expect(progress.AdvanceIfStepDone() && progress.IsComplete(), "started at least: 2 started completes");
        return passed;
    }

    bool TestHugeAmountDoesNotWrap()
    {
        ScenarioProgress progress(Arena());
        progress.UpdateCriteria(TYPE_KILL_CREATURE, 63311, ~uint64(0));
        return Expect(progress.GetCount(19336) == 1, "a huge amount stops at the leaf amount");
    }

    bool TestScriptOverrides()
    {
        ScenarioProgress progress(Arena());
        progress.AddCriteriaProgress(20810, 1);

        bool passed = Expect(progress.SetStep(1) && progress.GetCurrentStep() == 1 && progress.GetCount(20810) == 0,
            "a forced step starts with zero counts");
        passed &= Expect(!progress.SetStep(7), "an unknown step is refused");

        progress.Complete();
        passed &= Expect(progress.IsComplete() && progress.GetCurrentStep() == 2, "a script can end the scenario");
        passed &= Expect(!progress.AdvanceIfStepDone(), "nothing advances after the end");
        return passed;
    }

    bool TestScenarioWithoutSteps()
    {
        ScenarioData data;
        data.ScenarioId = 70;
        ScenarioProgress progress(data);
        return Expect(progress.IsComplete() && progress.GetCurrentStep() == 0 && progress.UpdateCriteria(TYPE_KILL_CREATURE, 1, 1).empty(),
            "a scenario without main steps tracks nothing");
    }
}

int main()
{
    bool passed = true;
    passed &= TestStepNeedsEveryObjective();
    passed &= TestLastStepCompletesIgnoringRootAmount();
    passed &= TestCountsStopAtAmount();
    passed &= TestBonusStepsAreSkipped();
    passed &= TestHiddenAndFailLeavesDoNotBlock();
    passed &= TestAggregateOperators();
    passed &= TestSharedCriteriaCountsOnce();
    passed &= TestStartedAtLeast();
    passed &= TestHugeAmountDoesNotWrap();
    passed &= TestScriptOverrides();
    passed &= TestScenarioWithoutSteps();

    if (!passed)
        return 1;

    std::cout << "scenario progress tests passed" << std::endl;
    return 0;
}
