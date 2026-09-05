/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "MonkSpellCalculations.h"

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
        return std::fabs(left - right) < 0.01f;
    }

    Skyfire::Spells::Monk::MeleeAbilityDamageData TwoHander()
    {
        Skyfire::Spells::Monk::MeleeAbilityDamageData data;
        data.MainHandDamage = 3600.0f;      // 1000 damage per second
        data.MainHandSpeed = 3.6f;
        data.AttackPower = 14000.0f;        // 1000 once divided by 14
        return data;
    }

    bool TestTwoHandedWindwalkerAddsWeaponDpsAndAttackPower()
    {
        float const damage = Skyfire::Spells::Monk::CalculateMeleeAbilityDamage(TwoHander(),
            Skyfire::Spells::Monk::BLACKOUT_KICK_COEFFICIENT);

        return Expect(NearlyEqual(damage, 7.12f * 2000.0f),
            "Blackout Kick should be its coefficient times weapon damage per second plus attack power over 14");
    }

    bool TestWeaponSpeedDoesNotChangeTheResult()
    {
        Skyfire::Spells::Monk::MeleeAbilityDamageData fast = TwoHander();
        fast.MainHandDamage = 1300.0f;      // same 1000 damage per second, much faster weapon
        fast.MainHandSpeed = 1.3f;

        float const slowDamage = Skyfire::Spells::Monk::CalculateMeleeAbilityDamage(TwoHander(),
            Skyfire::Spells::Monk::BLACKOUT_KICK_COEFFICIENT);
        float const fastDamage = Skyfire::Spells::Monk::CalculateMeleeAbilityDamage(fast,
            Skyfire::Spells::Monk::BLACKOUT_KICK_COEFFICIENT);

        return Expect(NearlyEqual(slowDamage, fastDamage),
            "Two weapons of equal damage per second should hit for the same amount");
    }

    bool TestDualWieldHalvesTheOffHandAndTakesThePenalty()
    {
        Skyfire::Spells::Monk::MeleeAbilityDamageData data = TwoHander();
        data.MainHandDamage = 2600.0f;      // 1000 damage per second
        data.MainHandSpeed = 2.6f;
        data.OffHandDamage = 2000.0f;       // 1000 damage per second, worth half of that
        data.OffHandSpeed = 2.0f;
        data.DualWield = true;

        float const damage = Skyfire::Spells::Monk::CalculateMeleeAbilityDamage(data,
            Skyfire::Spells::Monk::BLACKOUT_KICK_COEFFICIENT);

        float const expectedWeaponShare = 1500.0f * Skyfire::Spells::Monk::DUAL_WIELD_PENALTY;
        return Expect(NearlyEqual(damage, 7.12f * (expectedWeaponShare + 1000.0f)),
            "Dual wielding should add half the off hand and then lose the dual wield penalty");
    }

    bool TestBrewmasterKeepsLessWeaponAndConvertsAttackPowerFaster()
    {
        Skyfire::Spells::Monk::MeleeAbilityDamageData data = TwoHander();
        data.AttackPower = 11000.0f;        // 1000 once divided by 11
        data.Brewmaster = true;

        float const damage = Skyfire::Spells::Monk::CalculateMeleeAbilityDamage(data,
            Skyfire::Spells::Monk::BLACKOUT_KICK_COEFFICIENT);

        return Expect(NearlyEqual(damage, 7.12f * (1000.0f * 0.4f + 1000.0f)),
            "Brewmaster should keep 40 percent of the weapon share and divide attack power by 11");
    }

    bool TestCoefficientMultipliesTheWholeSum()
    {
        Skyfire::Spells::Monk::MeleeAbilityDamageData const data = TwoHander();

        float const jab = Skyfire::Spells::Monk::CalculateMeleeAbilityDamage(data,
            Skyfire::Spells::Monk::JAB_COEFFICIENT);
        float const tigerPalm = Skyfire::Spells::Monk::CalculateMeleeAbilityDamage(data,
            Skyfire::Spells::Monk::TIGER_PALM_COEFFICIENT);

        bool passed = true;
        passed &= Expect(NearlyEqual(jab, 1.5f * 2000.0f),
            "Jab should multiply the whole weapon plus attack power sum by its coefficient");
        passed &= Expect(NearlyEqual(tigerPalm, jab * 2.0f),
            "Tiger Palm's coefficient is twice Jab's, so the same stats should hit twice as hard");
        return passed;
    }

    bool TestUnarmedMonkStillScalesWithAttackPower()
    {
        Skyfire::Spells::Monk::MeleeAbilityDamageData data;
        data.AttackPower = 14000.0f;

        float const damage = Skyfire::Spells::Monk::CalculateMeleeAbilityDamage(data,
            Skyfire::Spells::Monk::BLACKOUT_KICK_COEFFICIENT);

        return Expect(NearlyEqual(damage, 7.12f * 1000.0f),
            "A monk without a weapon speed should still get the attack power share");
    }
}

int main()
{
    bool passed = true;

    passed &= TestTwoHandedWindwalkerAddsWeaponDpsAndAttackPower();
    passed &= TestWeaponSpeedDoesNotChangeTheResult();
    passed &= TestDualWieldHalvesTheOffHandAndTakesThePenalty();
    passed &= TestBrewmasterKeepsLessWeaponAndConvertsAttackPowerFaster();
    passed &= TestCoefficientMultipliesTheWholeSum();
    passed &= TestUnarmedMonkStillScalesWithAttackPower();

    return passed ? 0 : 1;
}
