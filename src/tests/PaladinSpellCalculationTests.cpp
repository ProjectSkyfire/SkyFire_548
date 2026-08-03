/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "PaladinSpellCalculations.h"

#include <iostream>

namespace
{
    bool Expect(bool condition, char const* message)
    {
        if (!condition)
            std::cerr << message << '\n';

        return condition;
    }

    bool TestArdentDefenderReducesNonLethalDamage()
    {
        Skyfire::Spells::Paladin::ArdentDefenderAbsorbResult const result =
            Skyfire::Spells::Paladin::CalculateArdentDefenderAbsorb(1000, 1000, 400, 20, 15, false);

        bool passed = true;
        passed &= Expect(result.AbsorbAmount == 80, "Ardent Defender should absorb 20 percent of non-lethal damage");
        passed &= Expect(result.HealAmount == 0, "Ardent Defender should not heal on non-lethal damage");
        passed &= Expect(!result.PreventedDeath, "Ardent Defender should not mark non-lethal hits as death prevention");
        return passed;
    }

    bool TestArdentDefenderLeavesTargetAtRescueHealth()
    {
        Skyfire::Spells::Paladin::ArdentDefenderAbsorbResult const result =
            Skyfire::Spells::Paladin::CalculateArdentDefenderAbsorb(500, 1000, 800, 20, 15, false);

        bool passed = true;
        passed &= Expect(result.AbsorbAmount == 450, "Ardent Defender should absorb enough lethal damage to leave 15 percent health");
        passed &= Expect(result.HealAmount == 0, "Ardent Defender should not add a heal when current health is above the rescue floor");
        passed &= Expect(result.PreventedDeath, "Ardent Defender should mark lethal hits as death prevention");
        return passed;
    }

    bool TestArdentDefenderHealsToRescueFloorWhenBelowIt()
    {
        Skyfire::Spells::Paladin::ArdentDefenderAbsorbResult const result =
            Skyfire::Spells::Paladin::CalculateArdentDefenderAbsorb(100, 1000, 400, 20, 15, false);

        bool passed = true;
        passed &= Expect(result.AbsorbAmount == 400, "Ardent Defender should fully absorb lethal damage below the rescue floor");
        passed &= Expect(result.HealAmount == 50, "Ardent Defender should heal up to the 15 percent rescue floor");
        passed &= Expect(result.PreventedDeath, "Ardent Defender should mark lethal hits below the rescue floor as death prevention");
        return passed;
    }

    bool TestArdentDefenderDoesNotPreventDeathTwice()
    {
        Skyfire::Spells::Paladin::ArdentDefenderAbsorbResult const result =
            Skyfire::Spells::Paladin::CalculateArdentDefenderAbsorb(150, 1000, 400, 20, 15, true);

        bool passed = true;
        passed &= Expect(result.AbsorbAmount == 80, "Ardent Defender should still reduce damage after the rescue was used");
        passed &= Expect(result.HealAmount == 0, "Ardent Defender should not heal after the rescue was used");
        passed &= Expect(!result.PreventedDeath, "Ardent Defender should not prevent death twice from one aura");
        return passed;
    }
}

int main()
{
    bool passed = true;

    passed &= TestArdentDefenderReducesNonLethalDamage();
    passed &= TestArdentDefenderLeavesTargetAtRescueHealth();
    passed &= TestArdentDefenderHealsToRescueFloorWhenBelowIt();
    passed &= TestArdentDefenderDoesNotPreventDeathTwice();

    return passed ? 0 : 1;
}
