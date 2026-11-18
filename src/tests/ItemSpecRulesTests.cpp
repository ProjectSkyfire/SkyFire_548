/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ItemSpecRules.h"

#include <iostream>

using namespace Skyfire::ItemSpec;

namespace
{
    // ChrSpecialization ids
    constexpr std::uint32_t WARRIOR_ARMS = 71, WARRIOR_FURY = 72, WARRIOR_PROT = 73;
    constexpr std::uint32_t PALADIN_HOLY = 65;

    // Plate rows in ItemSpec.dbc order: a strength row with a secondary stat, then strength alone.
    std::vector<ItemSpecRow> const ROWS =
    {
        { 1, 90, ITEM_SPEC_TYPE_CLOTH, ITEM_SPEC_STAT_INTELLECT, ITEM_SPEC_STAT_NONE, 62 },
        { 1, 90, ITEM_SPEC_TYPE_PLATE, ITEM_SPEC_STAT_STRENGTH, ITEM_SPEC_STAT_DODGE, WARRIOR_PROT },
        { 1, 90, ITEM_SPEC_TYPE_PLATE, ITEM_SPEC_STAT_STRENGTH, ITEM_SPEC_STAT_NONE, WARRIOR_ARMS },
        { 1, 90, ITEM_SPEC_TYPE_PLATE, ITEM_SPEC_STAT_STRENGTH, ITEM_SPEC_STAT_NONE, WARRIOR_FURY },
        { 1, 90, ITEM_SPEC_TYPE_PLATE, ITEM_SPEC_STAT_INTELLECT, ITEM_SPEC_STAT_NONE, PALADIN_HOLY },
        { 1, 90, ITEM_SPEC_TYPE_WEAPON, ITEM_SPEC_STAT_STRENGTH, ITEM_SPEC_STAT_NONE, 999 },
    };

    ItemSpecProfile Plate(std::initializer_list<std::uint32_t> stats)
    {
        ItemSpecProfile profile;
        profile.Type = ITEM_SPEC_TYPE_PLATE;
        for (std::uint32_t stat : stats)
            profile.AddStat(stat, true);
        return profile;
    }

    bool Expect(bool condition, char const* message)
    {
        if (!condition)
            std::cerr << message << '\n';

        return condition;
    }

    bool TestStrengthPlateSuitsDpsWarriors()
    {
        std::set<std::uint32_t> specs = MatchSpecializations(Plate({ ITEM_SPEC_STAT_STRENGTH, ITEM_SPEC_STAT_CRIT }), 90, ROWS);
        return Expect(specs == std::set<std::uint32_t>{ WARRIOR_ARMS, WARRIOR_FURY },
            "Strength plate without dodge should suit Arms and Fury only");
    }

    bool TestTankStatNarrowsToTank()
    {
        std::set<std::uint32_t> specs = MatchSpecializations(Plate({ ITEM_SPEC_STAT_STRENGTH, ITEM_SPEC_STAT_DODGE }), 90, ROWS);
        return Expect(specs == std::set<std::uint32_t>{ WARRIOR_PROT },
            "Strength plate with dodge should suit Protection only");
    }

    bool TestIntellectPlateSuitsHoly()
    {
        std::set<std::uint32_t> specs = MatchSpecializations(Plate({ ITEM_SPEC_STAT_INTELLECT }), 90, ROWS);
        return Expect(specs == std::set<std::uint32_t>{ PALADIN_HOLY }, "Intellect plate should suit Holy");
    }

    bool TestNoItemStatMeansNoRestriction()
    {
        ItemSpecProfile shield;
        shield.AddStat(ITEM_SPEC_STAT_SHIELD, false);
        return Expect(MatchSpecializations(shield, 90, ROWS).empty() && MatchSpecializations(ItemSpecProfile(), 90, ROWS).empty(),
            "An item without stats from its stat list should carry no spec restriction");
    }

    bool TestLevelOutsideRowsMatchesNothing()
    {
        return Expect(MatchSpecializations(Plate({ ITEM_SPEC_STAT_STRENGTH }), 91, ROWS).empty(),
            "Rows outside the level range should not match");
    }

    bool TestStopsAfterItsTypeBlock()
    {
        // the weapon row after the plate block must not be read for a plate item
        std::set<std::uint32_t> specs = MatchSpecializations(Plate({ ITEM_SPEC_STAT_STRENGTH }), 90, ROWS);
        return Expect(specs.count(999) == 0, "Matching should stop at the end of the item type's rows");
    }

    bool TestNarrowingIgnoresRowOrderAndKeepsEveryPairing()
    {
        // weapon block with two paired primaries, their primary-only rows listed before and after the pairings
        constexpr std::uint32_t ROGUE_COMBAT = 260, ROGUE_ASSASSINATION = 259, DK_FROST = 251, DK_UNHOLY = 252;
        std::vector<ItemSpecRow> const rows =
        {
            { 1, 90, ITEM_SPEC_TYPE_WEAPON, ITEM_SPEC_STAT_AGILITY, ITEM_SPEC_STAT_NONE, ROGUE_COMBAT },
            { 1, 90, ITEM_SPEC_TYPE_WEAPON, ITEM_SPEC_STAT_AGILITY, ITEM_SPEC_STAT_DAGGER, ROGUE_ASSASSINATION },
            { 1, 90, ITEM_SPEC_TYPE_WEAPON, ITEM_SPEC_STAT_STRENGTH, ITEM_SPEC_STAT_DAGGER, DK_FROST },
            { 1, 90, ITEM_SPEC_TYPE_WEAPON, ITEM_SPEC_STAT_STRENGTH, ITEM_SPEC_STAT_NONE, DK_UNHOLY },
        };

        ItemSpecProfile profile;
        profile.Type = ITEM_SPEC_TYPE_WEAPON;
        profile.AddStat(ITEM_SPEC_STAT_AGILITY, true);
        profile.AddStat(ITEM_SPEC_STAT_STRENGTH, true);
        profile.AddStat(ITEM_SPEC_STAT_DAGGER, false);

        std::set<std::uint32_t> specs = MatchSpecializations(profile, 90, rows);
        return Expect(specs == std::set<std::uint32_t>{ ROGUE_ASSASSINATION, DK_FROST },
            "Every paired primary stat should narrow to its paired specs, whatever the row order");
    }

    bool TestNoneStatIsIgnored()
    {
        ItemSpecProfile profile;
        profile.AddStat(ITEM_SPEC_STAT_NONE, true);
        return Expect(profile.Stats.empty() && !profile.HasItemStat, "A stat without a spec meaning should not count");
    }
}

int main()
{
    bool ok = TestStrengthPlateSuitsDpsWarriors();
    ok = TestTankStatNarrowsToTank() && ok;
    ok = TestIntellectPlateSuitsHoly() && ok;
    ok = TestNoItemStatMeansNoRestriction() && ok;
    ok = TestLevelOutsideRowsMatchesNothing() && ok;
    ok = TestStopsAfterItsTypeBlock() && ok;
    ok = TestNarrowingIgnoresRowOrderAndKeepsEveryPairing() && ok;
    ok = TestNoneStatIsIgnored() && ok;
    return ok ? 0 : 1;
}
