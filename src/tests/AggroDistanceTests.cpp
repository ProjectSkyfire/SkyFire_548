/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "AggroDistance.h"

#include <cmath>
#include <iostream>

namespace
{
    bool ExpectNear(float actual, float expected, char const* message)
    {
        if (std::fabs(actual - expected) < 0.001f)
            return true;

        std::cerr << message << ": expected " << expected << ", got " << actual << '\n';
        return false;
    }
}

int main()
{
    bool passed = true;

    passed &= ExpectNear(Skyfire::Aggro::CalculateAttackDistance(4, 4, 0.0f, 1.0f), 15.0f,
        "Equal-level creatures should use the Classic baseline");
    passed &= ExpectNear(Skyfire::Aggro::CalculateAttackDistance(4, 7, 0.0f, 1.0f), 18.0f,
        "Higher-level creatures should gain one yard per level");
    passed &= ExpectNear(Skyfire::Aggro::CalculateAttackDistance(9, 4, 0.0f, 1.0f), 10.0f,
        "Lower-level creatures should lose one yard per level");
    passed &= ExpectNear(Skyfire::Aggro::CalculateAttackDistance(80, 4, 0.0f, 1.0f), 5.0f,
        "Aggro distance should retain the five-yard minimum");
    passed &= ExpectNear(Skyfire::Aggro::CalculateAttackDistance(4, 4, 3.0f, 1.0f), 18.0f,
        "Detection modifiers should adjust aggro distance");
    passed &= ExpectNear(Skyfire::Aggro::CalculateAttackDistance(4, 4, 0.0f, 0.5f), 7.5f,
        "Creature aggro rate should scale the result");
    passed &= ExpectNear(Skyfire::Aggro::CalculateAttackDistance(4, 4, 0.0f, 0.0f), 0.0f,
        "Zero creature aggro rate should disable proximity aggro");

    return passed ? 0 : 1;
}
