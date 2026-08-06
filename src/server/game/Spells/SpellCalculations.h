/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_SPELLCALCULATIONS_H
#define SF_SPELLCALCULATIONS_H

#include "SharedDefines.h"

namespace Skyfire
{
namespace Spells
{
    struct SpellTickEffectInfo
    {
        uint32 Effect;
        uint32 ApplyAuraName;
        uint32 ApplyAuraTickCount;
    };

    struct SpellRadiusCalculationData
    {
        bool HasRadius;
        float RadiusMin;
        float RadiusPerLevel;
        float RadiusMax;
        bool HasMaxRadius;
        float MaxRadiusMin;
        bool HasCaster;
        uint8 CasterLevel;
    };

    enum SpellPowerCostCalculationStatus
    {
        SPELL_POWER_COST_OK,
        SPELL_POWER_COST_UNSUPPORTED_POWER_TYPE,
        SPELL_POWER_COST_UNKNOWN_POWER_TYPE
    };

    struct SpellPowerCostCalculationData
    {
        uint32 BasePowerCost;
        uint32 BasePeriodicPowerCost;
        float PowerCostPercentage;
        float ChannelCostPercentage;
        int32 PowerType;
        uint32 CreatedHealth;
        // For POWER_MANA % costs: create/base mana. For other powers: max power.
        uint32 MaxPower;
    };

    struct SpellPowerCostCalculationResult
    {
        uint32 PowerCost;
        uint32 PeriodicPowerCost;
        SpellPowerCostCalculationStatus Status;
    };

    int32 CalculateBaseCastTime(uint8 level, int32 castTimeMin, int32 castTimeMax, int32 castTimeMaxLevel, int32 fallbackCastTime);
    uint32 FinalizeCastTime(int32 castTime, bool addRangedAmmoDelay);
    int32 NormalizeDurationValue(int32 duration);
    int32 CalculateDuration(bool hasDurationEntry, int32 duration);
    uint32 CalculateMaxTicks(int32 duration, SpellTickEffectInfo const* effects, uint32 effectCount);
    int32 CalculateEffectBaseValue(int32 value, int32 dieSides);
    float CalculateRadius(SpellRadiusCalculationData const& data);
    float SelectSpellRange(bool hasRangeEntry, float friendRange, float hostileRange, bool positive);
    uint32 CalculateRecoveryTime(uint32 recoveryTime, uint32 categoryRecoveryTime);
    SpellPowerCostCalculationResult CalculateSpellPowerCosts(SpellPowerCostCalculationData const& data);
}
}

#endif
