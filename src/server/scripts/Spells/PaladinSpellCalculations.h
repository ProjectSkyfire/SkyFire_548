/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_PALADIN_SPELL_CALCULATIONS_H
#define SKYFIRE_PALADIN_SPELL_CALCULATIONS_H

#include "Define.h"
#include "Util.h"

namespace Skyfire
{
namespace Spells
{
namespace Paladin
{
    struct ArdentDefenderAbsorbResult
    {
        uint32 AbsorbAmount = 0;
        uint32 HealAmount = 0;
        bool PreventedDeath = false;
    };

    inline ArdentDefenderAbsorbResult CalculateArdentDefenderAbsorb(uint32 currentHealth, uint32 maxHealth,
        uint32 damage, uint32 absorbPct, uint32 rescueHealthPct, bool deathPreventionUsed)
    {
        ArdentDefenderAbsorbResult result;
        result.AbsorbAmount = CalculatePct(damage, absorbPct);

        if (!damage)
            return result;

        if (result.AbsorbAmount > damage)
            result.AbsorbAmount = damage;

        uint32 const reducedDamage = damage - result.AbsorbAmount;
        if (reducedDamage < currentHealth || deathPreventionUsed)
            return result;

        uint32 rescueHealth = CalculatePct(maxHealth, rescueHealthPct);
        if (!rescueHealth && maxHealth && rescueHealthPct)
            rescueHealth = 1;

        result.PreventedDeath = true;
        if (currentHealth > rescueHealth)
            result.AbsorbAmount = damage - currentHealth + rescueHealth;
        else
        {
            result.AbsorbAmount = damage;
            result.HealAmount = rescueHealth - currentHealth;
        }

        if (result.AbsorbAmount > damage)
            result.AbsorbAmount = damage;

        return result;
    }
}
}
}

#endif
