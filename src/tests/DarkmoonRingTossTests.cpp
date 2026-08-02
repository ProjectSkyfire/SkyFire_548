/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "darkmoon_faire.h"

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

    bool NearlyEqual(float left, float right)
    {
        return std::fabs(left - right) < 0.001f;
    }

    bool TestRingTossThrowVisualRules()
    {
        bool passed = true;

        passed &= Expect(DarkmoonRingToss::GetThrowSpellVisual(0) == DarkmoonRingToss::ThrowSpellVisual,
            "Ring Toss should fall back to the client-visible throw visual");
        passed &= Expect(DarkmoonRingToss::GetThrowSpellVisual(12345) == 12345,
            "Ring Toss should preserve a loaded DBC spell visual");
        passed &= Expect(DarkmoonRingToss::ThrowSpellVisual == 21471,
            "Ring Toss throw visual should match the 5.4.8 spell data");
        passed &= Expect(NearlyEqual(DarkmoonRingToss::GetThrowDestinationVisualZ(13.117f), 14.367f),
            "Ring Toss miss visuals should play above the terrain");

        return passed;
    }
}

int main()
{
    bool passed = true;

    passed &= TestRingTossThrowVisualRules();

    return passed ? 0 : 1;
}
