/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "LootCount.h"

#include <iostream>

namespace
{
    bool Expect(bool condition, char const* message)
    {
        if (!condition)
            std::cerr << message << '\n';

        return condition;
    }

    bool TestExactLootCounts()
    {
        bool passed = true;

        passed &= Expect(Skyfire::Looting::CalculateLootItemCount(5, 5, 0) == 5,
            "Loot count should be exact when minimum and maximum are both five");
        passed &= Expect(Skyfire::Looting::CalculateLootItemCount(5, 5, 42) == 5,
            "Loot count should ignore the roll when minimum and maximum match");
        passed &= Expect(Skyfire::Looting::CalculateLootItemCount(20, 20, 13) == 20,
            "Loot count should be exact when minimum and maximum are both twenty");

        return passed;
    }

    bool TestInclusiveLootCountRange()
    {
        bool passed = true;

        passed &= Expect(Skyfire::Looting::CalculateLootItemCount(5, 9, 0) == 5,
            "Loot count should include the minimum bound");
        passed &= Expect(Skyfire::Looting::CalculateLootItemCount(5, 9, 4) == 9,
            "Loot count should include the maximum bound");
        passed &= Expect(Skyfire::Looting::CalculateLootItemCount(5, 9, 5) == 5,
            "Loot count should wrap rolls into the inclusive range");

        return passed;
    }
}

int main()
{
    bool passed = true;

    passed &= TestExactLootCounts();
    passed &= TestInclusiveLootCountRange();

    return passed ? 0 : 1;
}
