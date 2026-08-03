/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_ALLOW_ONLY_ABILITY_CAST_H
#define SF_ALLOW_ONLY_ABILITY_CAST_H

namespace Skyfire
{
namespace Spells
{
    // SPELL_AURA_ALLOW_ONLY_ABILITY (Bladestorm, Killing Spree, ...):
    // only spells matching the aura effect SpellClassMask may be cast.
    // true => CheckCast should return SPELL_FAILED_SPELL_IN_PROGRESS
    inline bool ShouldBlockCastForAllowOnlyAbility(
        bool ignoreCasterAuraState,
        bool hasAllowOnlyAbilityAura,
        bool spellMatchesAllowOnlyAbilityMask)
    {
        if (ignoreCasterAuraState)
            return false;
        if (!hasAllowOnlyAbilityAura)
            return false;
        return !spellMatchesAllowOnlyAbilityMask;
    }
}
}

#endif // SF_ALLOW_ONLY_ABILITY_CAST_H
