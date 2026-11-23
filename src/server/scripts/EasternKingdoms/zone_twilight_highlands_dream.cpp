/*
* This file is part of the Pandaria 5.4.8 Project. See THANKS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "ScriptMgr.h"
#include "Player.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellScript.h"

enum DreamOfIsorath
{
    SPELL_ISORATH_DEATH_PREVENTION      = 89444,
    SPELL_ISORATH_FAKE_DEATH            = 89458,
    SPELL_ISORATH_FAKE_DEATH_STAGE_2    = 89470,
    SPELL_MAELSTROM_DEATH_PREVENTION    = 89655,
    SPELL_MAELSTROM_FAKE_DEATH          = 89656,
    SPELL_MAELSTROM_FAKE_DEATH_STAGE_2  = 89657,

    DEATH_PREVENTION_ABSORB             = 1000000000,
    DEATH_PREVENTION_LENGTH             = 150000,   // a player who leaves the dream does not keep it
    DEATH_PREVENTION_AFTER_FAKE_DEATH   = 10000     // the fake death and the teleport take nine seconds
};

// Iso'rath Death Prevention Aura 89444 and Maelstrom Death Prevention Aura 89655: the player of "Devoured" and of "The Worldbreaker"
// cannot die in the dream. The blow that would kill does no damage and starts the fake death of the dream, which credits the quest and
// carries the player on
class spell_twilight_dream_death_prevention : public AuraScript
{
    PrepareAuraScript(spell_twilight_dream_death_prevention);

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        amount = DEATH_PREVENTION_ABSORB;
    }

    void AfterApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        GetAura()->SetMaxDuration(DEATH_PREVENTION_LENGTH);
        GetAura()->SetDuration(DEATH_PREVENTION_LENGTH);
    }

    void Absorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        Unit* target = GetTarget();
        if (dmgInfo.GetDamage() < target->GetHealth())
        {
            absorbAmount = 0;
            return;
        }

        absorbAmount = dmgInfo.GetDamage();

        bool isorath = GetId() == SPELL_ISORATH_DEATH_PREVENTION;
        uint32 fakeDeath = isorath ? SPELL_ISORATH_FAKE_DEATH : SPELL_MAELSTROM_FAKE_DEATH;
        uint32 fakeDeathStage2 = isorath ? SPELL_ISORATH_FAKE_DEATH_STAGE_2 : SPELL_MAELSTROM_FAKE_DEATH_STAGE_2;
        if (target->HasAura(fakeDeath) || target->HasAura(fakeDeathStage2))
            return;

        target->CastSpell(target, fakeDeath, true);
        GetAura()->SetDuration(DEATH_PREVENTION_AFTER_FAKE_DEATH);
    }

    void Register() OVERRIDE
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_twilight_dream_death_prevention::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        AfterEffectApply += AuraEffectApplyFn(spell_twilight_dream_death_prevention::AfterApply, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
        OnEffectAbsorb += AuraEffectAbsorbFn(spell_twilight_dream_death_prevention::Absorb, EFFECT_0);
    }
};

void AddSC_twilight_highlands_dream()
{
    RegisterAuraScript(spell_twilight_dream_death_prevention);
}
