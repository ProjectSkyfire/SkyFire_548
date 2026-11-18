/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "KillRewardRules.h"

#include <iostream>

namespace
{
    using Skyfire::KillReward::CurrencyRowMatchesDifficulty;
    using Skyfire::KillReward::DifficultyPaysKillCurrency;

    // DifficultyID values of 5.4.8 (DBCEnums.h)
    constexpr std::uint8_t NONE = 0;
    constexpr std::uint8_t DUNGEON_NORMAL = 1;
    constexpr std::uint8_t DUNGEON_HEROIC = 2;
    constexpr std::uint8_t RAID_10_NORMAL = 3;
    constexpr std::uint8_t RAID_FINDER = 7;
    constexpr std::uint8_t FLEX = 14;

    bool Expect(bool condition, char const* message)
    {
        if (!condition)
            std::cerr << message << '\n';

        return condition;
    }

    bool TestEmptyMaskMatchesEveryDifficulty()
    {
        return Expect(CurrencyRowMatchesDifficulty(0, NONE)
                && CurrencyRowMatchesDifficulty(0, DUNGEON_NORMAL)
                && CurrencyRowMatchesDifficulty(0, DUNGEON_HEROIC)
                && CurrencyRowMatchesDifficulty(0, FLEX),
            "A row with lootmode 0 should apply at every difficulty");
    }

    bool TestHeroicRowSkipsNormal()
    {
        std::uint32_t heroic = 1u << DUNGEON_HEROIC;
        return Expect(CurrencyRowMatchesDifficulty(heroic, DUNGEON_HEROIC)
                && !CurrencyRowMatchesDifficulty(heroic, DUNGEON_NORMAL)
                && !CurrencyRowMatchesDifficulty(heroic, NONE),
            "A heroic-only row should apply on heroic and nowhere else");
    }

    bool TestMultiBitMask()
    {
        std::uint32_t raids = (1u << RAID_10_NORMAL) | (1u << FLEX);
        return Expect(CurrencyRowMatchesDifficulty(raids, RAID_10_NORMAL)
                && CurrencyRowMatchesDifficulty(raids, FLEX)
                && !CurrencyRowMatchesDifficulty(raids, DUNGEON_HEROIC),
            "A row with several bits should apply on each of them only");
    }

    bool TestDifficultyOutsideMask()
    {
        return Expect(!CurrencyRowMatchesDifficulty(0xFFFFFFFFu, 32) && !CurrencyRowMatchesDifficulty(0xFFFFFFFFu, 255),
            "A difficulty above 31 should never match a non-empty mask");
    }

    bool TestRaidFinderPaysNothing()
    {
        return Expect(!DifficultyPaysKillCurrency(RAID_FINDER)
                && DifficultyPaysKillCurrency(NONE)
                && DifficultyPaysKillCurrency(DUNGEON_HEROIC)
                && DifficultyPaysKillCurrency(FLEX),
            "Raid finder should pay no currency on kill; other difficulties should");
    }
}

int main()
{
    bool ok = true;
    ok &= TestEmptyMaskMatchesEveryDifficulty();
    ok &= TestHeroicRowSkipsNormal();
    ok &= TestMultiBitMask();
    ok &= TestDifficultyOutsideMask();
    ok &= TestRaidFinderPaysNothing();
    return ok ? 0 : 1;
}
