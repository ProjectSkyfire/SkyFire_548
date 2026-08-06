/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "SpellCalculations.h"

#include "SpellAuraDefines.h"
#include "Util.h"

namespace Skyfire
{
namespace Spells
{
    int32 CalculateBaseCastTime(uint8 level, int32 castTimeMin, int32 castTimeMax, int32 castTimeMaxLevel, int32 fallbackCastTime)
    {
        if (!level || castTimeMax <= 0)
            return fallbackCastTime;

        if (castTimeMaxLevel > level)
            return castTimeMin + int32(level - 1) * (castTimeMax - castTimeMin) / (castTimeMaxLevel - 1);

        return castTimeMax;
    }

    uint32 FinalizeCastTime(int32 castTime, bool addRangedAmmoDelay)
    {
        if (addRangedAmmoDelay)
            castTime += 500;

        return castTime > 0 ? uint32(castTime) : 0;
    }

    int32 NormalizeDurationValue(int32 duration)
    {
        if (duration == -1)
            return -1;

        return duration < 0 ? -duration : duration;
    }

    int32 CalculateDuration(bool hasDurationEntry, int32 duration)
    {
        return hasDurationEntry ? NormalizeDurationValue(duration) : 0;
    }

    namespace
    {
        bool IsCountedPeriodicAura(uint32 auraName)
        {
            switch (auraName)
            {
                case SPELL_AURA_PERIODIC_DAMAGE:
                case SPELL_AURA_PERIODIC_HEAL:
                case SPELL_AURA_PERIODIC_LEECH:
                    return true;
                default:
                    return false;
            }
        }
    }

    uint32 CalculateMaxTicks(int32 duration, SpellTickEffectInfo const* effects, uint32 effectCount)
    {
        if (duration == 0)
            return 1;

        int32 tickDuration = duration;
        if (tickDuration > 30000)
            tickDuration = 30000;

        for (uint32 i = 0; i < effectCount; ++i)
        {
            if (effects[i].Effect == SPELL_EFFECT_APPLY_AURA &&
                IsCountedPeriodicAura(effects[i].ApplyAuraName) &&
                effects[i].ApplyAuraTickCount != 0)
                return tickDuration / effects[i].ApplyAuraTickCount;
        }

        return 6;
    }

    int32 CalculateEffectBaseValue(int32 value, int32 dieSides)
    {
        return dieSides == 0 ? value : value - 1;
    }

    float CalculateRadius(SpellRadiusCalculationData const& data)
    {
        if (!data.HasRadius)
            return data.HasMaxRadius ? data.MaxRadiusMin : 0.0f;

        float radius = data.RadiusMin;
        if (data.HasCaster)
        {
            radius += data.RadiusPerLevel * data.CasterLevel;
            if (radius > data.RadiusMax)
                radius = data.RadiusMax;
        }

        return radius;
    }

    float SelectSpellRange(bool hasRangeEntry, float friendRange, float hostileRange, bool positive)
    {
        if (!hasRangeEntry)
            return 0.0f;

        return positive ? friendRange : hostileRange;
    }

    uint32 CalculateRecoveryTime(uint32 recoveryTime, uint32 categoryRecoveryTime)
    {
        return recoveryTime > categoryRecoveryTime ? recoveryTime : categoryRecoveryTime;
    }

    SpellPowerCostCalculationResult CalculateSpellPowerCosts(SpellPowerCostCalculationData const& data)
    {
        SpellPowerCostCalculationResult result =
        {
            data.BasePowerCost,
            data.BasePeriodicPowerCost,
            SPELL_POWER_COST_OK
        };

        if (data.ChannelCostPercentage)
            result.PeriodicPowerCost += CalculatePct(data.MaxPower, data.ChannelCostPercentage);

        if (!data.PowerCostPercentage)
            return result;

        switch (data.PowerType)
        {
            case POWER_HEALTH:
                result.PowerCost += CalculatePct(data.CreatedHealth, data.PowerCostPercentage);
                break;
            case POWER_MANA:
                // Percentage mana costs are of create/base mana (client tooltip), not max mana.
                result.PowerCost += CalculatePct(data.MaxPower, data.PowerCostPercentage);
                break;
            case POWER_DEMONIC_FURY:
                result.PowerCost += CalculatePct(data.MaxPower, data.PowerCostPercentage);
                break;
            case POWER_RUNIC_POWER:
                result.Status = SPELL_POWER_COST_UNSUPPORTED_POWER_TYPE;
                break;
            default:
                result.PowerCost = 0;
                result.Status = SPELL_POWER_COST_UNKNOWN_POWER_TYPE;
                break;
        }

        return result;
    }
}
}
