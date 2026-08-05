/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "PvpResilience.h"

#include <cmath>
#include <iostream>

namespace
{
    bool Expect(bool condition, char const* message)
    {
        if (!condition)
            std::cerr << message << '\n';

        return condition;
    }

    bool NearlyEqual(float left, float right, float tolerance = 0.001f)
    {
        return std::fabs(left - right) < tolerance;
    }

    bool TestBasePvpResilienceByLevel()
    {
        bool passed = true;

        passed &= Expect(NearlyEqual(Skyfire::Combat::CalculatePvpResilienceReductionPercent(1, 0), 40.0f),
            "Low-level players should have 40 percent base PvP resilience");
        passed &= Expect(NearlyEqual(Skyfire::Combat::CalculatePvpResilienceReductionPercent(89, 0), 40.0f),
            "Players below level 90 should keep 40 percent base PvP resilience");
        passed &= Expect(NearlyEqual(Skyfire::Combat::CalculatePvpResilienceReductionPercent(90, 0), 77.0f),
            "Level 90 players should have 77 percent base PvP resilience");

        return passed;
    }

    bool TestMop548Level90PvpResilienceCurve()
    {
        bool passed = true;

        passed &= Expect(NearlyEqual(Skyfire::Combat::CalculatePvpResilienceReductionPercent(90, 310), 77.2441f),
            "Level 90 PvP resilience should follow the MoP 5.4.8 rating curve at 310 rating");
        passed &= Expect(NearlyEqual(Skyfire::Combat::CalculatePvpResilienceReductionPercent(90, 10000), 82.9126f),
            "Level 90 PvP resilience should follow the MoP 5.4.8 rating curve at high rating");
        passed &= Expect(NearlyEqual(Skyfire::Combat::CalculatePvpResilienceReductionPercent(90, 28900), 88.5f),
            "Level 90 PvP resilience should approach 100 percent through diminishing returns");

        return passed;
    }
}

int main()
{
    bool passed = true;

    passed &= TestBasePvpResilienceByLevel();
    passed &= TestMop548Level90PvpResilienceCurve();

    return passed ? 0 : 1;
}
