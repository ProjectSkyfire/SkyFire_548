/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "AllowOnlyAbilityCast.h"

#include <iostream>

namespace
{
    bool Expect(bool condition, char const* message)
    {
        if (!condition)
            std::cerr << message << '\n';

        return condition;
    }

    bool TestNoAuraAllowsCast()
    {
        return Expect(!Skyfire::Spells::ShouldBlockCastForAllowOnlyAbility(false, false, false),
            "Casts should not be blocked when ALLOW_ONLY_ABILITY is absent");
    }

    bool TestMatchingMaskAllowsCast()
    {
        return Expect(!Skyfire::Spells::ShouldBlockCastForAllowOnlyAbility(false, true, true),
            "Spells matching the ALLOW_ONLY_ABILITY mask should be allowed");
    }

    bool TestNonMatchingMaskBlocksCast()
    {
        return Expect(Skyfire::Spells::ShouldBlockCastForAllowOnlyAbility(false, true, false),
            "Spells outside the ALLOW_ONLY_ABILITY mask should be blocked");
    }

    bool TestIgnoreAuraStateAllowsTriggeredCast()
    {
        return Expect(!Skyfire::Spells::ShouldBlockCastForAllowOnlyAbility(true, true, false),
            "Triggered casts that ignore caster aura state should bypass ALLOW_ONLY_ABILITY");
    }
}

int main()
{
    bool passed = true;

    passed &= TestNoAuraAllowsCast();
    passed &= TestMatchingMaskAllowsCast();
    passed &= TestNonMatchingMaskBlocksCast();
    passed &= TestIgnoreAuraStateAllowsTriggeredCast();

    return passed ? 0 : 1;
}
