/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "BonusLootRules.h"

#include <iostream>

namespace
{
    using Skyfire::BonusLoot::IsExcludedItem;
    using Skyfire::BonusLoot::LootIdForDifficulty;
    using Skyfire::BonusLoot::RollsOnPersonalLoot;

    // DifficultyID values of 5.4.8 (DBCEnums.h)
    constexpr std::uint8_t NONE = 0;
    constexpr std::uint8_t DUNGEON_HEROIC = 2;
    constexpr std::uint8_t RAID_10_NORMAL = 3;
    constexpr std::uint8_t RAID_25_NORMAL = 4;
    constexpr std::uint8_t RAID_10_HEROIC = 5;
    constexpr std::uint8_t RAID_25_HEROIC = 6;
    constexpr std::uint8_t RAID_FINDER = 7;
    constexpr std::uint8_t FLEX = 14;

    bool Expect(bool condition, char const* message)
    {
        if (!condition)
            std::cerr << message << '\n';

        return condition;
    }

    bool TestPersonalLootDifficulties()
    {
        return Expect(RollsOnPersonalLoot(NONE) && RollsOnPersonalLoot(RAID_FINDER) && RollsOnPersonalLoot(FLEX)
                && !RollsOnPersonalLoot(RAID_10_NORMAL) && !RollsOnPersonalLoot(RAID_25_HEROIC),
            "World bosses, raid finder and flex should roll on the personal loot list, other raids not");
    }

    bool TestNormalAndHeroicLootIds()
    {
        return Expect(LootIdForDifficulty(100, 200, RAID_10_NORMAL) == 100 && LootIdForDifficulty(100, 200, RAID_25_NORMAL) == 100
                && LootIdForDifficulty(100, 200, RAID_10_HEROIC) == 200 && LootIdForDifficulty(100, 200, RAID_25_HEROIC) == 200,
            "10 and 25 normal should use the normal loot id, 10 and 25 heroic the heroic one");
    }

    bool TestOtherDifficultiesHaveNoLootId()
    {
        return Expect(LootIdForDifficulty(100, 200, DUNGEON_HEROIC) == 0 && LootIdForDifficulty(100, 200, RAID_FINDER) == 0,
            "Dungeons and raid finder should have no normal or heroic loot id");
    }

    bool TestExcludedItems()
    {
        return Expect(IsExcludedItem(105751) && IsExcludedItem(87777) && IsExcludedItem(104253) && !IsExcludedItem(104400),
            "The two mounts and the Kor'kron chest should never come from a bonus roll");
    }
}

int main()
{
    bool ok = true;
    ok &= TestPersonalLootDifficulties();
    ok &= TestNormalAndHeroicLootIds();
    ok &= TestOtherDifficultiesHaveNoLootId();
    ok &= TestExcludedItems();
    return ok ? 0 : 1;
}
