/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_PVP_RESILIENCE_H
#define SF_PVP_RESILIENCE_H

#include "Define.h"

#include <algorithm>

namespace Skyfire
{
namespace Combat
{
    constexpr float Mop548LowLevelBasePvpResiliencePercent = 40.0f;
    constexpr float Mop548Level90BasePvpResiliencePercent = 77.0f;
    constexpr float Mop548Level90PvpResilienceRatingConstant = 28900.0f;

    inline float GetBasePvpResilienceReductionPercent(uint8 level)
    {
        return level >= 90 ? Mop548Level90BasePvpResiliencePercent : Mop548LowLevelBasePvpResiliencePercent;
    }

    inline float CalculatePvpResilienceReductionPercent(uint8 level, uint32 resilienceRating)
    {
        float basePercent = GetBasePvpResilienceReductionPercent(level);
        float rating = float(resilienceRating);

        if (level < 90)
            return std::min(basePercent, 100.0f);

        float base = basePercent / 100.0f;
        float reduction = ((Mop548Level90PvpResilienceRatingConstant * base) + rating)
            / (Mop548Level90PvpResilienceRatingConstant + rating);

        return std::min(reduction * 100.0f, 100.0f);
    }
}
}

#endif
