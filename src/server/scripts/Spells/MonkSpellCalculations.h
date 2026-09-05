/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_MONK_SPELL_CALCULATIONS_H
#define SKYFIRE_MONK_SPELL_CALCULATIONS_H

#include "Define.h"

namespace Skyfire
{
namespace Spells
{
namespace Monk
{
    // Monk melee abilities read weapon damage per SECOND: the roll is divided by the
    // weapon speed instead of being taken per swing, and the attack power share is
    // added without a speed multiplier. A slow staff and a fast pair of fists therefore
    // feed almost the same number into the ability. The client bakes the per-ability
    // multiplier into the tooltip text -- Blackout Kick reads "[7.12 * <low>] to
    // [7.12 * <high>] Physical damage" -- so it is not in the spell's DBC base points,
    // which carry a placeholder 1.
    float const JAB_COEFFICIENT = 1.5f;
    float const TIGER_PALM_COEFFICIENT = 3.0f;
    float const BLACKOUT_KICK_COEFFICIENT = 7.12f;

    // SimulationCraft's mop branch runs Rising Sun Kick 11% under its tooltip
    // (14.4 * 0.89), a late hotfix Blizzard never wrote back into the spell text.
    // Blackout Kick's text did get rewritten for that hotfix, which is why its 7.12
    // already matches. Follow the tooltip here: it is the number players read.
    float const RISING_SUN_KICK_COEFFICIENT = 14.4f;

    // Two weapons contribute the off hand at half rate and then lose ~10% of the total,
    // which is what keeps dual wield and a two-hander close to each other.
    float const OFF_HAND_SHARE = 0.5f;
    float const DUAL_WIELD_PENALTY = 0.898882275f;

    // Brewmaster keeps far less of the weapon and converts attack power faster instead.
    float const BREWMASTER_WEAPON_SHARE = 0.4f;
    float const ATTACK_POWER_DIVISOR = 14.0f;
    float const BREWMASTER_ATTACK_POWER_DIVISOR = 11.0f;

    struct MeleeAbilityDamageData
    {
        float MainHandDamage = 0.0f;    // one roll out of the weapon's own min/max, attack power excluded
        float MainHandSpeed = 0.0f;     // seconds, unhasted
        float OffHandDamage = 0.0f;
        float OffHandSpeed = 0.0f;
        float AttackPower = 0.0f;
        bool DualWield = false;
        bool Brewmaster = false;
    };

    inline float CalculateMeleeAbilityDamage(MeleeAbilityDamageData const& data, float coefficient)
    {
        float weaponShare = 0.0f;

        if (data.MainHandSpeed > 0.0f)
            weaponShare += data.MainHandDamage / data.MainHandSpeed;

        if (data.DualWield)
        {
            if (data.OffHandSpeed > 0.0f)
                weaponShare += data.OffHandDamage / data.OffHandSpeed * OFF_HAND_SHARE;

            weaponShare *= DUAL_WIELD_PENALTY;
        }

        if (data.Brewmaster)
            weaponShare *= BREWMASTER_WEAPON_SHARE;

        float const attackPowerShare = data.AttackPower /
            (data.Brewmaster ? BREWMASTER_ATTACK_POWER_DIVISOR : ATTACK_POWER_DIVISOR);

        return coefficient * (weaponShare + attackPowerShare);
    }
}
}
}

#endif
