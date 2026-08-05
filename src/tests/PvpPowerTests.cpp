/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "PvpPower.h"

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

    bool TestHealingSpecializationsReceiveHealingOnly()
    {
        bool passed = true;

        passed &= Expect(NearlyEqual(Skyfire::Combat::CalculatePvpPowerDamagePercent(10.0f, SPEC_PALADIN_HOLY), 0.0f),
            "Healing specs should not receive PvP Power damage bonus");
        passed &= Expect(NearlyEqual(Skyfire::Combat::CalculatePvpPowerHealingPercent(10.0f, SPEC_PALADIN_HOLY), 10.0f),
            "Healing specs should receive 100 percent of PvP Power as healing bonus");
        passed &= Expect(NearlyEqual(Skyfire::Combat::CalculatePvpPowerDamagePercent(10.0f, SPEC_MONK_MISTWEAVER), 0.0f),
            "Mistweaver should not receive PvP Power damage bonus");
        passed &= Expect(NearlyEqual(Skyfire::Combat::CalculatePvpPowerHealingPercent(10.0f, SPEC_MONK_MISTWEAVER), 10.0f),
            "Mistweaver should receive 100 percent of PvP Power as healing bonus");

        return passed;
    }

    bool TestHybridDamageSpecializationsReceiveReducedHealing()
    {
        bool passed = true;

        passed &= Expect(NearlyEqual(Skyfire::Combat::CalculatePvpPowerDamagePercent(10.0f, SPEC_DRUID_BALANCE), 10.0f),
            "Hybrid damage specs should receive full PvP Power damage bonus");
        passed &= Expect(NearlyEqual(Skyfire::Combat::CalculatePvpPowerHealingPercent(10.0f, SPEC_DRUID_BALANCE), 7.0f),
            "Hybrid damage specs should receive 70 percent of PvP Power as healing bonus");
        passed &= Expect(NearlyEqual(Skyfire::Combat::CalculatePvpPowerHealingPercent(10.0f, SPEC_PRIEST_SHADOW), 7.0f),
            "Shadow should receive 70 percent of PvP Power as healing bonus");
        passed &= Expect(NearlyEqual(Skyfire::Combat::CalculatePvpPowerHealingPercent(10.0f, SPEC_SHAMAN_ENHANCEMENT), 7.0f),
            "Enhancement should receive 70 percent of PvP Power as healing bonus");

        return passed;
    }

    bool TestOtherSpecializationsReceiveReducedHealing()
    {
        bool passed = true;

        passed &= Expect(NearlyEqual(Skyfire::Combat::CalculatePvpPowerDamagePercent(10.0f, SPEC_WARRIOR_ARMS), 10.0f),
            "Non-healing specs should receive full PvP Power damage bonus");
        passed &= Expect(NearlyEqual(Skyfire::Combat::CalculatePvpPowerHealingPercent(10.0f, SPEC_WARRIOR_ARMS), 4.0f),
            "Other specs should receive 40 percent of PvP Power as healing bonus");
        passed &= Expect(NearlyEqual(Skyfire::Combat::CalculatePvpPowerHealingPercent(10.0f, SPEC_PALADIN_PROTECTION), 4.0f),
            "Tank specs should receive 40 percent of PvP Power as healing bonus");
        passed &= Expect(NearlyEqual(Skyfire::Combat::CalculatePvpPowerHealingPercent(10.0f, SPEC_NONE), 4.0f),
            "Characters without a selected spec should receive 40 percent of PvP Power as healing bonus");

        return passed;
    }
}

int main()
{
    bool passed = true;

    passed &= TestHealingSpecializationsReceiveHealingOnly();
    passed &= TestHybridDamageSpecializationsReceiveReducedHealing();
    passed &= TestOtherSpecializationsReceiveReducedHealing();

    return passed ? 0 : 1;
}
