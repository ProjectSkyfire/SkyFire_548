/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_PVP_POWER_H
#define SF_PVP_POWER_H

#include "Define.h"
#include "SharedDefines.h"

namespace Skyfire
{
namespace Combat
{
    constexpr float Mop548HybridDamageSpecPvpPowerHealingPct = 70.0f;
    constexpr float Mop548OtherSpecPvpPowerHealingPct = 40.0f;

    inline bool IsPvpPowerHealingSpecialization(uint32 specializationId)
    {
        switch (specializationId)
        {
            case SPEC_PALADIN_HOLY:
            case SPEC_DRUID_RESTORATION:
            case SPEC_PRIEST_DISCIPLINE:
            case SPEC_PRIEST_HOLY:
            case SPEC_SHAMAN_RESTORATION:
            case SPEC_MONK_MISTWEAVER:
                return true;
            default:
                return false;
        }
    }

    inline bool IsPvpPowerHybridDamageSpecialization(uint32 specializationId)
    {
        switch (specializationId)
        {
            case SPEC_DRUID_BALANCE:
            case SPEC_DRUID_FERAL:
            case SPEC_MONK_WINDWALKER:
            case SPEC_PALADIN_RETRIBUTION:
            case SPEC_PRIEST_SHADOW:
            case SPEC_SHAMAN_ELEMENTAL:
            case SPEC_SHAMAN_ENHANCEMENT:
                return true;
            default:
                return false;
        }
    }

    inline float CalculatePvpPowerDamagePercent(float pvpPowerPercent, uint32 specializationId)
    {
        return IsPvpPowerHealingSpecialization(specializationId) ? 0.0f : pvpPowerPercent;
    }

    inline float CalculatePvpPowerHealingPercent(float pvpPowerPercent, uint32 specializationId)
    {
        if (IsPvpPowerHealingSpecialization(specializationId))
            return pvpPowerPercent;

        if (IsPvpPowerHybridDamageSpecialization(specializationId))
            return pvpPowerPercent * Mop548HybridDamageSpecPvpPowerHealingPct / 100.0f;

        return pvpPowerPercent * Mop548OtherSpecPvpPowerHealingPct / 100.0f;
    }
}
}

#endif
