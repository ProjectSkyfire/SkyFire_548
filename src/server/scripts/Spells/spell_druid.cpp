/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/*
 * Scripts for spells with SPELLFAMILY_DRUID and SPELLFAMILY_GENERIC spells used by druid players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_dru_".
 */

#include "Player.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "SpellMgr.h"
#include "Containers.h"
#include "ObjectAccessor.h"
#include "EventProcessor.h"
#include "ScriptedCreature.h"
#include "Battleground.h"
#include "TemporarySummon.h"
#include "PassiveAI.h"
#include "PetDefines.h"
#include <algorithm>
#include <limits>

enum DruidSpells
{
    SPELL_DRUID_WRATH                       = 5176,
    SPELL_DRUID_STARFIRE                    = 2912,
    SPELL_DRUID_STARSURGE                   = 78674,
    SPELL_DRUID_ECLIPSE_GENERAL_ENERGIZE    = 89265,
    SPELL_DRUID_ECLIPSE_MANA_ENERGIZE       = 81070,
    SPELL_DRUID_EUPHORIA                    = 81062,
    SPELL_DRUID_LUNAR_ECLIPSE_MARKER        = 67484, // Will make the yellow arrow on eclipse bar point to the blue side (lunar)
    SPELL_DRUID_SOLAR_ECLIPSE_MARKER        = 67483, // Will make the yellow arrow on eclipse bar point to the yellow side (solar)
    SPELL_DRUID_SOLAR_ECLIPSE               = 48517,
    SPELL_DRUID_LUNAR_ECLIPSE               = 48518,
    SPELL_DRUID_LUNAR_ECLIPSE_OVERRIDE      = 107095,
    SPELL_DRUID_STARFALL                     = 48505,
    SPELL_DRUID_SOLAR_BEAM                   = 78675,
    SPELL_DRUID_SOLAR_BEAM_SILENCE          = 81261,
    SPELL_DRUID_SOLAR_BEAM_SYMBIOSIS        = 113286,
    SPELL_DRUID_SOLAR_BEAM_SILENCE_SYMBIOSIS = 113287,
    SPELL_DRUID_FORCE_OF_NATURE_VISUAL      = 37846,
    SPELL_DRUID_FORCE_OF_NATURE_ROOT        = 113770,
    SPELL_DRUID_FORCE_OF_NATURE_WRATH       = 113769,
    SPELL_DRUID_FORCE_OF_NATURE_SWIFTMEND   = 142421,
    SPELL_DRUID_FORCE_OF_NATURE_HEAL        = 113828,
    SPELL_DRUID_FORCE_OF_NATURE_TAUNT       = 113830,
    SPELL_DRUID_FERAL_CHARGE_BEAR           = 16979,
    SPELL_DRUID_FERAL_CHARGE_CAT            = 49376,
    SPELL_DRUID_GLYPH_OF_INNERVATE          = 54833,
    SPELL_DRUID_GLYPH_OF_STARFIRE           = 54846,
    SPELL_DRUID_INCREASED_MOONFIRE_DURATION = 38414,
    SPELL_DRUID_LIFEBLOOM_ENERGIZE          = 64372,
    SPELL_DRUID_LIFEBLOOM_FINAL_HEAL        = 33778,
    SPELL_DRUID_LIFEBLOOM                   = 33763,
    SPELL_DRUID_GLYPH_OF_BLOOMING           = 121840,
    SPELL_DRUID_LIVING_SEED_HEAL            = 48503,
    SPELL_DRUID_LIVING_SEED_PROC            = 48504,
    SPELL_DRUID_NATURES_GRACE               = 16880,
    SPELL_DRUID_NATURES_GRACE_TRIGGER       = 16886,
    SPELL_DRUID_SURVIVAL_INSTINCTS          = 50322,
    SPELL_DRUID_SAVAGE_DEFENSE_AURA         = 132402, // Dodge chance buff from Savage Defense
    SPELL_DRUID_SAVAGE_ROAR                 = 52610,
    SPELL_DRUID_SAVAGE_ROAR_TRIGGER         = 62071,
    SPELL_DRUID_STAMPEDE_BAER_RANK_1        = 81016,
    SPELL_DRUID_STAMPEDE_CAT_RANK_1         = 81021,
    SPELL_DRUID_STAMPEDE_CAT_STATE          = 109881,
    SPELL_DRUID_TIGER_S_FURY_ENERGIZE       = 51178,
    SPELL_DRUID_BEAR_FORM                   = 5487,
    SPELL_DRUID_SKULL_BASH_MANA_COST        = 82365,
    SPELL_DRUID_SKULL_BASH_INTERRUPT        = 93985,
    SPELL_DRUID_SKULL_BASH_CHARGE           = 93983,
    SPELL_DRUID_RIP                         = 1079,
    SPELL_DRUID_GLYPH_OF_FEROCIOUS_BITE     = 67598,
    SPELL_DRUID_GLYPH_OF_FEROCIOUS_BITE_HEAL= 101024,
    SPELL_DRUID_RAKE                        = 1822,
    SPELL_DRUID_REJUVENATION                = 774,
    SPELL_DRUID_MANGLE_BEAR                 = 33878,
    SPELL_DRUID_MANGLE_BEAR_STUB            = 33917,
    SPELL_DRUID_MANGLE_PASSIVE              = 93622, // Mangle! — supplies the 25% reset chance
    SPELL_DRUID_FAERIE_FIRE                 = 770,
    SPELL_DRUID_FAERIE_SWARM                = 102355,
    SPELL_DRUID_FAERIE_SWARM_SLOW           = 102354,
    SPELL_DRUID_WEAKENED_ARMOR              = 113746,
    SPELL_DRUID_YSERAS_GIFT                 = 145108,
    SPELL_DRUID_YSERAS_GIFT_HEAL_SELF       = 145109,
    SPELL_DRUID_YSERAS_GIFT_HEAL_ALLY       = 145110,
    SPELL_DRUID_MUSHROOM_BIRTH              = 94081,
    SPELL_DRUID_MUSHROOM_INVISIBLE          = 92661,
    SPELL_DRUID_MUSHROOM_DEATH              = 116305,
    SPELL_DRUID_MUSHROOM_SUICIDE            = 116302,
    SPELL_DRUID_MUSHROOM_DAMAGE             = 78777,
    SPELL_DRUID_MUSHROOM_HEAL               = 102792,
    SPELL_DRUID_MUSHROOM_OVERHEAL_AURA      = 138611,
    SPELL_DRUID_MUSHROOM_ACCUMULATOR        = 138616,
    SPELL_DRUID_MUSHROOM_BLOOM_UI_FLASH     = 138664,
    SPELL_DRUID_FUNGAL_GROWTH               = 81283,
    SPELL_DRUID_FUNGAL_GROWTH_SLOW          = 81282,
    SPELL_DRUID_MUSHROOM_SLOW_VISUAL        = 94339,
    SPELL_DRUID_WILD_MUSHROOM_RESTO         = 145205,
    SPELL_DRUID_WILD_MUSHROOM_RESTO_GLYPH   = 147349,
    SPELL_DRUID_FRENZIED_REGENERATION       = 22842,
    SPELL_DRUID_GLYPH_OF_FRENZIED_REGEN     = 54810,
    SPELL_DRUID_FRENZIED_REGEN_HEAL_TAKE    = 124769,
    SPELL_DRUID_BEAR_HUG                    = 102795,
};

enum DruidCreatureIds
{
    NPC_DRUID_WILD_MUSHROOM                 = 47649,
    NPC_DRUID_FORCE_OF_NATURE_BALANCE       = 1964,
    NPC_DRUID_FORCE_OF_NATURE_RESTORATION   = 54983,
    NPC_DRUID_FORCE_OF_NATURE_FERAL         = 54984,
    NPC_DRUID_FORCE_OF_NATURE_GUARDIAN      = 54985
};

// Faerie Fire / Faerie Swarm: apply 3 stacks of Weakened Armor (113746).
static void ApplyWeakenedArmorStacks(Unit* caster, Unit* target)
{
    if (!caster || !target)
        return;

    if (Aura* aura = target->GetAura(SPELL_DRUID_WEAKENED_ARMOR))
    {
        aura->SetStackAmount(3);
        aura->RefreshDuration();
    }
    else
        caster->CastCustomSpell(SPELL_DRUID_WEAKENED_ARMOR, SPELLVALUE_AURA_STACK, 3, target, true);
}

// Lacerate / Thrash (Bear) / Faerie Fire (Bear): chance from Mangle! (93622) to clear Mangle CD.
static void TryResetMangleCooldown(Unit* caster)
{
    Player* player = caster ? caster->ToPlayer() : NULL;
    if (!player)
        return;

    int32 chance = 25;
    if (SpellInfo const* manglePassive = sSpellMgr->GetSpellInfo(SPELL_DRUID_MANGLE_PASSIVE))
        if (manglePassive->Effects[EFFECT_0].IsEffect())
            chance = manglePassive->Effects[EFFECT_0].CalcValue(caster);

    if (chance <= 0)
        chance = 25;

    if (!roll_chance_i(chance))
        return;

    // Clear both the real Bear Mangle and the action-bar stub clients may track.
    player->RemoveSpellCooldown(SPELL_DRUID_MANGLE_BEAR, true);
    player->RemoveSpellCooldown(SPELL_DRUID_MANGLE_BEAR_STUB, true);
}

// 1850 - Dash
class spell_dru_dash : public SpellScriptLoader
{
public:
    spell_dru_dash() : SpellScriptLoader("spell_dru_dash") { }

    class spell_dru_dash_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_dash_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            // do not set speed if not in cat form
            if (GetUnitOwner()->GetShapeshiftForm() != FORM_CAT)
                amount = 0;
        }

        void Register() override
        {
            // MoP Dash uses SPELL_AURA_MOD_SPEED_ALWAYS (129), not MOD_INCREASE_SPEED (31).
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_dash_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_SPEED_ALWAYS);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_dash_AuraScript();
    }
};

// 48517 - Eclipse (Solar)
// 48518 - Eclipse (Lunar)
class spell_dru_eclipse : public SpellScriptLoader
{
public:
    spell_dru_eclipse(char const* scriptName) : SpellScriptLoader(scriptName) { }

    class spell_dru_eclipse_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_eclipse_AuraScript);

        bool Load() override
        {
            return GetCaster() && GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void ApplyEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            caster->CastSpell(caster, SPELL_DRUID_NATURES_GRACE_TRIGGER, true);
            caster->CastSpell(caster, SPELL_DRUID_ECLIPSE_MANA_ENERGIZE, true);

            if (GetId() == SPELL_DRUID_LUNAR_ECLIPSE)
            {
                caster->CastSpell(caster, SPELL_DRUID_LUNAR_ECLIPSE_OVERRIDE, true);
                caster->ToPlayer()->RemoveSpellCooldown(SPELL_DRUID_STARFALL, true);
            }
            else
                caster->RemoveAurasDueToSpell(SPELL_DRUID_LUNAR_ECLIPSE_OVERRIDE);
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_dru_eclipse_AuraScript::ApplyEffect, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_eclipse_AuraScript();
    }
};

// 2912, 5176, 78674 - Starfire, Wrath, and Starsurge
class spell_dru_eclipse_energize : public SpellScriptLoader
{
public:
    spell_dru_eclipse_energize() : SpellScriptLoader("spell_dru_eclipse_energize") { }

    class spell_dru_eclipse_energize_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_eclipse_energize_SpellScript);

        int32 energizeAmount;

        bool Load() override
        {
            if (GetCaster()->GetTypeId() != TypeID::TYPEID_PLAYER)
                return false;

            if (GetCaster()->ToPlayer()->getClass() != CLASS_DRUID)
                return false;

            energizeAmount = 0;

            return true;
        }

        void HandleEclipseReached(Player* caster)
        {
            int32 power = caster->GetPower(POWER_ECLIPSE);
            if (power >= 100 && !caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE))
            {
                caster->RemoveAurasDueToSpell(SPELL_DRUID_SOLAR_ECLIPSE_MARKER);
                caster->CastSpell(caster, SPELL_DRUID_LUNAR_ECLIPSE_MARKER, true);
                caster->CastSpell(caster, SPELL_DRUID_SOLAR_ECLIPSE, true);
            }
            else if (power <= -100 && !caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE))
            {
                caster->RemoveAurasDueToSpell(SPELL_DRUID_LUNAR_ECLIPSE_MARKER);
                caster->CastSpell(caster, SPELL_DRUID_SOLAR_ECLIPSE_MARKER, true);
                caster->CastSpell(caster, SPELL_DRUID_LUNAR_ECLIPSE, true);
            }
        }

        void HandleEnergize(SpellEffIndex effIndex)
        {
            Player* caster = GetCaster()->ToPlayer();

            // No boomy, no deal.
            if (caster->GetTalentSpecialization(caster->GetActiveSpec()) != SPEC_DRUID_BALANCE)
                return;

            bool hasSolarMarker = caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE_MARKER);
            bool hasLunarMarker = caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE_MARKER);
            if (hasSolarMarker == hasLunarMarker)
            {
                caster->RemoveAurasDueToSpell(SPELL_DRUID_SOLAR_ECLIPSE_MARKER);
                caster->RemoveAurasDueToSpell(SPELL_DRUID_LUNAR_ECLIPSE_MARKER);

                if (caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE))
                    caster->CastSpell(caster, SPELL_DRUID_SOLAR_ECLIPSE_MARKER, true);
                else if (caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE))
                    caster->CastSpell(caster, SPELL_DRUID_LUNAR_ECLIPSE_MARKER, true);
                else if (GetSpellInfo()->Id == SPELL_DRUID_STARFIRE)
                    caster->CastSpell(caster, SPELL_DRUID_SOLAR_ECLIPSE_MARKER, true);
                else
                    caster->CastSpell(caster, SPELL_DRUID_LUNAR_ECLIPSE_MARKER, true);
            }

            switch (GetSpellInfo()->Id)
            {
            case SPELL_DRUID_WRATH:
            {
                energizeAmount = -GetSpellInfo()->Effects[effIndex].BasePoints; // -13
                if (caster->HasAura(SPELL_DRUID_EUPHORIA) &&
                    !caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE) &&
                    !caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE))
                    energizeAmount *= 2;
                // If we are set to fill the lunar side or we've just logged in with 0 power..
                if ((!caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE_MARKER) && caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE_MARKER))
                    || caster->GetPower(POWER_ECLIPSE) == 0)
                {
                    caster->CastCustomSpell(caster, SPELL_DRUID_ECLIPSE_GENERAL_ENERGIZE, &energizeAmount, 0, 0, true);
                    // If the energize was due to 0 power, cast the eclipse marker aura
                    if (!caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE_MARKER))
                        caster->CastSpell(caster, SPELL_DRUID_LUNAR_ECLIPSE_MARKER, true);
                }
                // The energizing effect brought us out of the solar eclipse, remove the aura
                if (caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE) && caster->GetPower(POWER_ECLIPSE) <= 0)
                    caster->RemoveAurasDueToSpell(SPELL_DRUID_SOLAR_ECLIPSE);
                break;
            }
            case SPELL_DRUID_STARFIRE:
            {
                energizeAmount = GetSpellInfo()->Effects[effIndex].BasePoints; // 20
                if (caster->HasAura(SPELL_DRUID_EUPHORIA) &&
                    !caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE) &&
                    !caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE))
                    energizeAmount *= 2;
                // If we are set to fill the solar side or we've just logged in with 0 power..
                if ((!caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE_MARKER) && caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE_MARKER))
                    || caster->GetPower(POWER_ECLIPSE) == 0)
                {
                    caster->CastCustomSpell(caster, SPELL_DRUID_ECLIPSE_GENERAL_ENERGIZE, &energizeAmount, 0, 0, true);
                    // If the energize was due to 0 power, cast the eclipse marker aura
                    if (!caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE_MARKER))
                        caster->CastSpell(caster, SPELL_DRUID_SOLAR_ECLIPSE_MARKER, true);
                }
                // The energizing effect brought us out of the lunar eclipse, remove the aura
                if (caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE) && caster->GetPower(POWER_ECLIPSE) >= 0)
                {
                    caster->RemoveAura(SPELL_DRUID_LUNAR_ECLIPSE);
                    caster->RemoveAurasDueToSpell(SPELL_DRUID_LUNAR_ECLIPSE_OVERRIDE);
                }
                break;
            }
            case SPELL_DRUID_STARSURGE:
            {
                // If we are set to fill the solar side or we've just logged in with 0 power (confirmed with sniffs)
                if ((!caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE_MARKER) && caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE_MARKER))
                    || caster->GetPower(POWER_ECLIPSE) == 0)
                {
                    energizeAmount = GetSpellInfo()->Effects[effIndex].BasePoints; // 15
                    if (caster->HasAura(SPELL_DRUID_EUPHORIA) &&
                        !caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE) &&
                        !caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE))
                        energizeAmount *= 2;
                    caster->CastCustomSpell(caster, SPELL_DRUID_ECLIPSE_GENERAL_ENERGIZE, &energizeAmount, 0, 0, true);

                    // If the energize was due to 0 power, cast the eclipse marker aura
                    if (!caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE_MARKER))
                        caster->CastSpell(caster, SPELL_DRUID_SOLAR_ECLIPSE_MARKER, true);
                }
                else if (!caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE_MARKER) && caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE_MARKER))
                {
                    energizeAmount = -GetSpellInfo()->Effects[effIndex].BasePoints; // -15
                    if (caster->HasAura(SPELL_DRUID_EUPHORIA) &&
                        !caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE) &&
                        !caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE))
                        energizeAmount *= 2;
                    caster->CastCustomSpell(caster, SPELL_DRUID_ECLIPSE_GENERAL_ENERGIZE, &energizeAmount, 0, 0, true);
                }
                // The energizing effect brought us out of the lunar eclipse, remove the aura
                if (caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE) && caster->GetPower(POWER_ECLIPSE) >= 0)
                {
                    caster->RemoveAura(SPELL_DRUID_LUNAR_ECLIPSE);
                    caster->RemoveAurasDueToSpell(SPELL_DRUID_LUNAR_ECLIPSE_OVERRIDE);
                }
                // The energizing effect brought us out of the solar eclipse, remove the aura
                else if (caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE) && caster->GetPower(POWER_ECLIPSE) <= 0)
                    caster->RemoveAura(SPELL_DRUID_SOLAR_ECLIPSE);
                break;
            }
            }

            HandleEclipseReached(caster);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_dru_eclipse_energize_SpellScript::HandleEnergize, EFFECT_1, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_eclipse_energize_SpellScript;
    }
};

// 54832 - Glyph of Innervate
class spell_dru_glyph_of_innervate : public SpellScriptLoader
{
public:
    spell_dru_glyph_of_innervate() : SpellScriptLoader("spell_dru_glyph_of_innervate") { }

    class spell_dru_glyph_of_innervate_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_glyph_of_innervate_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_GLYPH_OF_INNERVATE))
                return false;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            // Not proc from self Innervate
            return GetTarget() != eventInfo.GetProcTarget();
        }

        void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            GetTarget()->CastSpell(GetTarget(), SPELL_DRUID_GLYPH_OF_INNERVATE, true, NULL, aurEff);
        }

        void Register() override
        {
            // Glyph spell 54832 uses EFFECT_0 DUMMY. Do not bind this script to Innervate (29166).
            DoCheckProc += AuraCheckProcFn(spell_dru_glyph_of_innervate_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_dru_glyph_of_innervate_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_glyph_of_innervate_AuraScript();
    }
};

// 54846 - Glyph of Starfire
class spell_dru_glyph_of_starfire : public SpellScriptLoader
{
public:
    spell_dru_glyph_of_starfire() : SpellScriptLoader("spell_dru_glyph_of_starfire") { }

    class spell_dru_glyph_of_starfire_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_glyph_of_starfire_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_INCREASED_MOONFIRE_DURATION))
                return false;
            return true;
        }

        void HandleScriptEffect(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (Unit* unitTarget = GetHitUnit())
                if (AuraEffect const* aurEff = unitTarget->GetAuraEffect(SPELL_AURA_PERIODIC_DAMAGE, SPELLFAMILY_DRUID, 0x2, 0, 0, caster->GetGUID()))
                {
                    Aura* aura = aurEff->GetBase();

                    uint32 countMin = aura->GetMaxDuration();
                    uint32 countMax = aura->GetSpellInfo()->GetMaxDuration() + 9000;
                    if (caster->HasAura(SPELL_DRUID_INCREASED_MOONFIRE_DURATION))
                        countMax += 3000;

                    if (countMin < countMax)
                    {
                        aura->SetDuration(uint32(aura->GetDuration() + 3000));
                        aura->SetMaxDuration(countMin + 3000);
                    }
                }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_dru_glyph_of_starfire_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_glyph_of_starfire_SpellScript();
    }
};

// 54845 - Glyph of Starfire
class spell_dru_glyph_of_starfire_proc : public SpellScriptLoader
{
public:
    spell_dru_glyph_of_starfire_proc() : SpellScriptLoader("spell_dru_glyph_of_starfire_proc") { }

    class spell_dru_glyph_of_starfire_proc_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_glyph_of_starfire_proc_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_GLYPH_OF_STARFIRE))
                return false;
            return true;
        }

        void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            GetTarget()->CastSpell(eventInfo.GetProcTarget(), SPELL_DRUID_GLYPH_OF_STARFIRE, true, NULL, aurEff);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_dru_glyph_of_starfire_proc_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_glyph_of_starfire_proc_AuraScript();
    }
};

// 29166 - Innervate
class spell_dru_innervate : public SpellScriptLoader
{
public:
    spell_dru_innervate() : SpellScriptLoader("spell_dru_innervate") { }

    class spell_dru_innervate_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_innervate_AuraScript);

        void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
        {
            amount = CalculatePct(int32(GetUnitOwner()->GetCreatePowers(POWER_MANA) / aurEff->GetTotalTicks()), amount);
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_innervate_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_ENERGIZE);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_innervate_AuraScript();
    }
};

// 33763 - Lifebloom
class spell_dru_lifebloom : public SpellScriptLoader
{
public:
    spell_dru_lifebloom() : SpellScriptLoader("spell_dru_lifebloom") { }

    class spell_dru_lifebloom_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_lifebloom_AuraScript);

        bool Validate(SpellInfo const* /*spell*/) override
        {
            // SPELL_DRUID_LIFEBLOOM_ENERGIZE (64372) does not exist in MoP DBC
            if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_LIFEBLOOM_FINAL_HEAL))
                return false;
            return true;
        }

        void AfterRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            // Final heal only on duration end
            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
                return;

            // final heal
            int32 stack = GetStackAmount();
            int32 healAmount = aurEff->GetAmount();
            if (Unit* caster = GetCaster())
            {
                healAmount = caster->SpellHealingBonusDone(GetTarget(), GetSpellInfo(), healAmount, HEAL, stack);
                healAmount = GetTarget()->SpellHealingBonusTaken(caster, GetSpellInfo(), healAmount, HEAL, stack);
            }

            GetTarget()->CastCustomSpell(GetTarget(), SPELL_DRUID_LIFEBLOOM_FINAL_HEAL, &healAmount, NULL, NULL, true, NULL, aurEff, GetCasterGUID());
        }

        void HandleDispel(DispelInfo* dispelInfo)
        {
            if (Unit* target = GetUnitOwner())
            {
                if (AuraEffect const* aurEff = GetEffect(EFFECT_1))
                {
                    // final heal
                    int32 healAmount = aurEff->GetAmount();
                    if (Unit* caster = GetCaster())
                    {
                        healAmount = caster->SpellHealingBonusDone(target, GetSpellInfo(), healAmount, HEAL, dispelInfo->GetRemovedCharges());
                        healAmount = target->SpellHealingBonusTaken(caster, GetSpellInfo(), healAmount, HEAL, dispelInfo->GetRemovedCharges());
                    }

                    target->CastCustomSpell(target, SPELL_DRUID_LIFEBLOOM_FINAL_HEAL, &healAmount, NULL, NULL, true, NULL, NULL, GetCasterGUID());
                }
            }
        }

        void Register() override
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_dru_lifebloom_AuraScript::AfterRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            AfterDispel += AuraDispelFn(spell_dru_lifebloom_AuraScript::HandleDispel);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_lifebloom_AuraScript();
    }
};

// 5185 - Healing Touch, 50464 - Nourish, 8936 - Regrowth
// Refresh Lifebloom duration unless Glyph of Blooming is active.
class spell_dru_lifebloom_refresh : public SpellScriptLoader
{
public:
    spell_dru_lifebloom_refresh() : SpellScriptLoader("spell_dru_lifebloom_refresh") { }

    class spell_dru_lifebloom_refresh_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_lifebloom_refresh_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return sSpellMgr->GetSpellInfo(SPELL_DRUID_LIFEBLOOM);
        }

        void HandleHit()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!caster || !target || caster->HasAura(SPELL_DRUID_GLYPH_OF_BLOOMING))
                return;

            if (Aura* lifebloom = target->GetAura(SPELL_DRUID_LIFEBLOOM, caster->GetGUID()))
                lifebloom->RefreshTimers();
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_lifebloom_refresh_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_lifebloom_refresh_SpellScript();
    }
};

// 33745 - Lacerate
// Bake AP into DoT amount so client $w1 tooltip and ticks match MoP (AP * 0.0512 / tick).
// Each hit has a 25% chance (Mangle! 93622) to reset Mangle (Bear).
class spell_dru_lacerate : public SpellScriptLoader
{
public:
    spell_dru_lacerate() : SpellScriptLoader("spell_dru_lacerate") { }

    class spell_dru_lacerate_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_lacerate_SpellScript);

        void HandleHit()
        {
            TryResetMangleCooldown(GetCaster());
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_lacerate_SpellScript::HandleHit);
        }
    };

    class spell_dru_lacerate_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_lacerate_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
        {
            if (Unit* caster = GetCaster())
            {
                canBeRecalculated = false;
                amount += int32(caster->GetTotalAttackPowerValue(WeaponAttackType::BASE_ATTACK) * 0.0512f);
            }
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_lacerate_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_lacerate_SpellScript();
    }

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_lacerate_AuraScript();
    }
};

// 77758 - Thrash (Bear)
// Each hit has a 25% chance (Mangle! 93622) to reset Mangle (Bear).
class spell_dru_thrash_bear : public SpellScriptLoader
{
public:
    spell_dru_thrash_bear() : SpellScriptLoader("spell_dru_thrash_bear") { }

    class spell_dru_thrash_bear_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_thrash_bear_SpellScript);

        void HandleHit()
        {
            TryResetMangleCooldown(GetCaster());
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_thrash_bear_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_thrash_bear_SpellScript();
    }
};

// 770 - Faerie Fire
// Effect 0 Dummy: apply 3 stacks of Weakened Armor.
// In Bear Form, has a 25% chance (Mangle! 93622) to reset Mangle (Bear).
class spell_dru_faerie_fire : public SpellScriptLoader
{
public:
    spell_dru_faerie_fire() : SpellScriptLoader("spell_dru_faerie_fire") { }

    class spell_dru_faerie_fire_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_faerie_fire_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return sSpellMgr->GetSpellInfo(SPELL_DRUID_WEAKENED_ARMOR) != NULL
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_MANGLE_PASSIVE) != NULL;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            ApplyWeakenedArmorStacks(GetCaster(), GetHitUnit());
        }

        void HandleHit()
        {
            Unit* caster = GetCaster();
            if (!caster || caster->GetShapeshiftForm() != FORM_BEAR)
                return;

            TryResetMangleCooldown(caster);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_dru_faerie_fire_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            OnHit += SpellHitFn(spell_dru_faerie_fire_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_faerie_fire_SpellScript();
    }
};

// 102355 - Faerie Swarm (replaces Faerie Fire via talent 106707 OVERRIDE_ACTIONBAR)
// Effect 0 Dummy: 3 stacks of Weakened Armor + Faerie Swarm snare (102354).
// In Bear Form, same Mangle! reset chance as Faerie Fire.
class spell_dru_faerie_swarm : public SpellScriptLoader
{
public:
    spell_dru_faerie_swarm() : SpellScriptLoader("spell_dru_faerie_swarm") { }

    class spell_dru_faerie_swarm_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_faerie_swarm_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return sSpellMgr->GetSpellInfo(SPELL_DRUID_WEAKENED_ARMOR) != NULL
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_FAERIE_SWARM_SLOW) != NULL
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_MANGLE_PASSIVE) != NULL;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            ApplyWeakenedArmorStacks(caster, target);
            if (caster && target)
                caster->CastSpell(target, SPELL_DRUID_FAERIE_SWARM_SLOW, true);
        }

        void HandleHit()
        {
            Unit* caster = GetCaster();
            if (!caster || caster->GetShapeshiftForm() != FORM_BEAR)
                return;

            TryResetMangleCooldown(caster);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_dru_faerie_swarm_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            OnHit += SpellHitFn(spell_dru_faerie_swarm_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_faerie_swarm_SpellScript();
    }
};

// 1822 - Rake
// MoP: initial and each tick use 0.3 AP. Bake tick AP into aura amount so $w2 matches damage.
class spell_dru_rake : public SpellScriptLoader
{
public:
    spell_dru_rake() : SpellScriptLoader("spell_dru_rake") { }

    class spell_dru_rake_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_rake_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
        {
            if (Unit* caster = GetCaster())
            {
                canBeRecalculated = false;
                amount += int32(caster->GetTotalAttackPowerValue(WeaponAttackType::BASE_ATTACK) * 0.3f);
            }
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_rake_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_rake_AuraScript();
    }
};

// 774 - Rejuvenation
// MoP has two PERIODIC_HEAL effects (EFFECT_0 and EFFECT_2); suppress EFFECT_2 to avoid double ticks.
class spell_dru_rejuvenation : public SpellScriptLoader
{
public:
    spell_dru_rejuvenation() : SpellScriptLoader("spell_dru_rejuvenation") { }

    class spell_dru_rejuvenation_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_rejuvenation_AuraScript);

        void HandlePeriodic(AuraEffect const* /*aurEff*/)
        {
            PreventDefaultAction();
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_rejuvenation_AuraScript::HandlePeriodic, EFFECT_2, SPELL_AURA_PERIODIC_HEAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_rejuvenation_AuraScript();
    }
};

// -48496 - Living Seed
class spell_dru_living_seed : public SpellScriptLoader
{
public:
    spell_dru_living_seed() : SpellScriptLoader("spell_dru_living_seed") { }

    class spell_dru_living_seed_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_living_seed_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_LIVING_SEED_PROC))
                return false;
            return true;
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            int32 amount = CalculatePct(eventInfo.GetHealInfo()->GetHeal(), aurEff->GetAmount());
            GetTarget()->CastCustomSpell(SPELL_DRUID_LIVING_SEED_PROC, SPELLVALUE_BASE_POINT0, amount, eventInfo.GetProcTarget(), true, NULL, aurEff);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_dru_living_seed_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_living_seed_AuraScript();
    }
};

// 48504 - Living Seed (Proc)
class spell_dru_living_seed_proc : public SpellScriptLoader
{
public:
    spell_dru_living_seed_proc() : SpellScriptLoader("spell_dru_living_seed_proc") { }

    class spell_dru_living_seed_proc_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_living_seed_proc_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_LIVING_SEED_HEAL))
                return false;
            return true;
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            GetTarget()->CastCustomSpell(SPELL_DRUID_LIVING_SEED_HEAL, SPELLVALUE_BASE_POINT0, aurEff->GetAmount(), GetTarget(), true, NULL, aurEff);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_dru_living_seed_proc_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_living_seed_proc_AuraScript();
    }
};

// 106922 - Might of Ursoc
class spell_dru_might_of_ursoc: public SpellScriptLoader
{
public:
    spell_dru_might_of_ursoc() : SpellScriptLoader("spell_dru_might_of_ursoc") { }

    class spell_dru_might_of_ursoc_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_might_of_ursoc_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_BEAR_FORM))
                return false;
            return true;
        }

        void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* caster = GetCaster())
            {
                // cast bear form first
                if (caster->GetShapeshiftForm() != FORM_BEAR)
                    caster->CastSpell(caster, SPELL_DRUID_BEAR_FORM, true); // activate bear form
                // then calculate amount
                amount = aurEff->GetBase()->GetUnitOwner()->CountPctFromMaxHealth(amount);
            }
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_might_of_ursoc_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH_2);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dru_might_of_ursoc_AuraScript();
    }
};

// -16972 - Predatory Strikes
class spell_dru_predatory_strikes : public SpellScriptLoader
{
public:
    spell_dru_predatory_strikes() : SpellScriptLoader("spell_dru_predatory_strikes") { }

    class spell_dru_predatory_strikes_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_predatory_strikes_AuraScript);

        void UpdateAmount(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* target = GetTarget()->ToPlayer())
                target->UpdateAttackPowerAndDamage();
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_dru_predatory_strikes_AuraScript::UpdateAmount, EFFECT_ALL, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
            AfterEffectRemove += AuraEffectRemoveFn(spell_dru_predatory_strikes_AuraScript::UpdateAmount, EFFECT_ALL, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_predatory_strikes_AuraScript();
    }
};

// 62606 - Savage Defense
// MoP: Dummy cast that applies dodge aura 132402 (not the old Cata absorb shield).
class spell_dru_savage_defense : public SpellScriptLoader
{
public:
    spell_dru_savage_defense() : SpellScriptLoader("spell_dru_savage_defense") { }

    class spell_dru_savage_defense_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_savage_defense_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_SAVAGE_DEFENSE_AURA))
                return false;
            return true;
        }

        void HandleCast()
        {
            if (Unit* caster = GetCaster())
                caster->CastSpell(caster, SPELL_DRUID_SAVAGE_DEFENSE_AURA, true);
        }

        void Register() override
        {
            AfterCast += SpellCastFn(spell_dru_savage_defense_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_savage_defense_SpellScript();
    }
};

// 52610 - Savage Roar
class spell_dru_savage_roar : public SpellScriptLoader
{
public:
    spell_dru_savage_roar() : SpellScriptLoader("spell_dru_savage_roar") { }

    class spell_dru_savage_roar_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_savage_roar_SpellScript);

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            if (caster->GetShapeshiftForm() != FORM_CAT)
                return SpellCastResult::SPELL_FAILED_ONLY_SHAPESHIFT;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dru_savage_roar_SpellScript::CheckCast);
        }
    };

    class spell_dru_savage_roar_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_savage_roar_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_SAVAGE_ROAR))
                return false;
            return true;
        }

        void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (Player* caster = GetCaster()->ToPlayer())
            {
                if (Aura* savageRoar = caster->GetAura(SPELL_DRUID_SAVAGE_ROAR))
                {
                    caster->CastSpell(caster, SPELL_DRUID_SAVAGE_ROAR_TRIGGER, true, NULL, aurEff, GetCasterGUID());
                    uint8 comboPoints = caster->GetComboPoints();

                    int32 minduration = GetSpellInfo()->GetDuration();
                    int32 maxduration = GetSpellInfo()->GetMaxDuration();
                    int32 duration = minduration + int32((maxduration - minduration) * comboPoints / 5);

                    savageRoar->SetDuration(duration);
                }
            }
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_dru_savage_roar_AuraScript::OnApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_savage_roar_SpellScript();
    }

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_savage_roar_AuraScript();
    }
};

// 50286 - Starfall (Dummy)
class spell_dru_starfall_dummy : public SpellScriptLoader
{
public:
    spell_dru_starfall_dummy() : SpellScriptLoader("spell_dru_starfall_dummy") { }

    class spell_dru_starfall_dummy_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_starfall_dummy_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            Skyfire::Containers::RandomResizeList(targets, 2);
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            // Shapeshifting into an animal form or mounting cancels the effect
            if (caster->GetCreatureType() == CREATURE_TYPE_BEAST || caster->IsMounted())
            {
                if (SpellInfo const* spellInfo = GetTriggeringSpell())
                    caster->RemoveAurasDueToSpell(spellInfo->Id);
                return;
            }

            // Any effect which causes you to lose control of your character will supress the starfall effect.
            if (caster->HasUnitState(UNIT_STATE_CONTROLLED))
                return;

            caster->CastSpell(GetHitUnit(), uint32(GetEffectValue()), true);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_starfall_dummy_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_dru_starfall_dummy_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_starfall_dummy_SpellScript();
    }
};

// -78892 - Stampede
class spell_dru_stampede : public SpellScriptLoader
{
public:
    spell_dru_stampede() : SpellScriptLoader("spell_dru_stampede") { }

    class spell_dru_stampede_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_stampede_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_STAMPEDE_BAER_RANK_1) ||
                !sSpellMgr->GetSpellInfo(SPELL_DRUID_STAMPEDE_CAT_RANK_1) ||
                !sSpellMgr->GetSpellInfo(SPELL_DRUID_STAMPEDE_CAT_STATE) ||
                !sSpellMgr->GetSpellInfo(SPELL_DRUID_FERAL_CHARGE_CAT) ||
                !sSpellMgr->GetSpellInfo(SPELL_DRUID_FERAL_CHARGE_BEAR))
                return false;
            return true;
        }

        void HandleEffectCatProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            if (GetTarget()->GetShapeshiftForm() != FORM_CAT || eventInfo.GetDamageInfo()->GetSpellInfo()->Id != SPELL_DRUID_FERAL_CHARGE_CAT)
                return;

            GetTarget()->CastSpell(GetTarget(), sSpellMgr->GetSpellWithRank(SPELL_DRUID_STAMPEDE_CAT_RANK_1, GetSpellInfo()->GetRank()), true, NULL, aurEff);
            GetTarget()->CastSpell(GetTarget(), SPELL_DRUID_STAMPEDE_CAT_STATE, true, NULL, aurEff);
        }

        void HandleEffectBearProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            if (GetTarget()->GetShapeshiftForm() != FORM_BEAR || eventInfo.GetDamageInfo()->GetSpellInfo()->Id != SPELL_DRUID_FERAL_CHARGE_BEAR)
                return;

            GetTarget()->CastSpell(GetTarget(), sSpellMgr->GetSpellWithRank(SPELL_DRUID_STAMPEDE_BAER_RANK_1, GetSpellInfo()->GetRank()), true, NULL, aurEff);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_dru_stampede_AuraScript::HandleEffectCatProc, EFFECT_0, SPELL_AURA_DUMMY);
            OnEffectProc += AuraEffectProcFn(spell_dru_stampede_AuraScript::HandleEffectBearProc, EFFECT_1, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_stampede_AuraScript();
    }
};

// 61336 - Survival Instincts
class spell_dru_survival_instincts : public SpellScriptLoader
{
public:
    spell_dru_survival_instincts() : SpellScriptLoader("spell_dru_survival_instincts") { }

    class spell_dru_survival_instincts_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_survival_instincts_SpellScript);

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            if (!caster->IsInFeralForm())
                return SpellCastResult::SPELL_FAILED_ONLY_SHAPESHIFT;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dru_survival_instincts_SpellScript::CheckCast);
        }
    };

    class spell_dru_survival_instincts_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_survival_instincts_AuraScript);

        bool Validate(SpellInfo const* /*spell*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_SURVIVAL_INSTINCTS))
                return false;
            return true;
        }

        void AfterApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            int32 bp0 = target->CountPctFromMaxHealth(aurEff->GetAmount());
            target->CastCustomSpell(target, SPELL_DRUID_SURVIVAL_INSTINCTS, &bp0, NULL, NULL, true);
        }

        void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->RemoveAurasDueToSpell(SPELL_DRUID_SURVIVAL_INSTINCTS);
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_dru_survival_instincts_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
            AfterEffectRemove += AuraEffectRemoveFn(spell_dru_survival_instincts_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_survival_instincts_SpellScript();
    }

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_survival_instincts_AuraScript();
    }
};

// 40121 - Swift Flight Form (Passive)
class spell_dru_swift_flight_passive : public SpellScriptLoader
{
public:
    spell_dru_swift_flight_passive() : SpellScriptLoader("spell_dru_swift_flight_passive") { }

    class spell_dru_swift_flight_passive_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_swift_flight_passive_AuraScript);

        bool Load() override
        {
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Player* caster = GetCaster()->ToPlayer())
                if (caster->GetSkillValue(SKILL_RIDING) >= 375)
                    amount = 310;
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_swift_flight_passive_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_INCREASE_VEHICLE_FLIGHT_SPEED);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_swift_flight_passive_AuraScript();
    }
};

// 70691 - Item T10 Restoration 4P Bonus
class spell_dru_t10_restoration_4p_bonus : public SpellScriptLoader
{
public:
    spell_dru_t10_restoration_4p_bonus() : SpellScriptLoader("spell_dru_t10_restoration_4p_bonus") { }

    class spell_dru_t10_restoration_4p_bonus_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_t10_restoration_4p_bonus_SpellScript);

        bool Load() override
        {
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (!GetCaster()->ToPlayer()->GetGroup())
            {
                targets.clear();
                targets.push_back(GetCaster());
            }
            else
            {
                targets.remove(GetExplTargetUnit());
                std::list<Unit*> tempTargets;
                for (std::list<WorldObject*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
                    if ((*itr)->GetTypeId() == TypeID::TYPEID_PLAYER && GetCaster()->IsInRaidWith((*itr)->ToUnit()))
                        tempTargets.push_back((*itr)->ToUnit());

                if (tempTargets.empty())
                {
                    targets.clear();
                    FinishCast(SpellCastResult::SPELL_FAILED_DONT_REPORT);
                    return;
                }

                Unit* target = Skyfire::Containers::SelectRandomContainerElement(tempTargets);
                targets.clear();
                targets.push_back(target);
            }
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_t10_restoration_4p_bonus_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_t10_restoration_4p_bonus_SpellScript();
    }
};

// 106839 - Skull Bash (MoP, Cat + Bear)
// 80964 / 80965 - legacy form-specific casts still present on some characters
class spell_dru_skull_bash : public SpellScriptLoader
{
public:
    spell_dru_skull_bash() : SpellScriptLoader("spell_dru_skull_bash") { }

    class spell_dru_skull_bash_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_skull_bash_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return sSpellMgr->GetSpellInfo(SPELL_DRUID_SKULL_BASH_MANA_COST)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_SKULL_BASH_INTERRUPT)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_SKULL_BASH_CHARGE);
        }

        void HandleHit()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!caster || !target)
                return;

            caster->CastSpell(target, SPELL_DRUID_SKULL_BASH_CHARGE, true);
            caster->CastSpell(target, SPELL_DRUID_SKULL_BASH_INTERRUPT, true);
            caster->CastSpell(target, SPELL_DRUID_SKULL_BASH_MANA_COST, true);
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_skull_bash_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_skull_bash_SpellScript();
    }
};

// 22568 - Ferocious Bite
class spell_dru_ferocious_bite : public SpellScriptLoader
{
public:
    spell_dru_ferocious_bite() : SpellScriptLoader("spell_dru_ferocious_bite") { }

    class spell_dru_ferocious_bite_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_ferocious_bite_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return sSpellMgr->GetSpellInfo(SPELL_DRUID_RIP)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_GLYPH_OF_FEROCIOUS_BITE)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_GLYPH_OF_FEROCIOUS_BITE_HEAL);
        }

        void HandleHit()
        {
            Player* player = GetCaster()->ToPlayer();
            Unit* target = GetHitUnit();
            if (!player || !target)
                return;

            int32 energyCost = GetSpell()->GetPowerCost();

            if (int32 energyBoost = player->GetPower(POWER_ENERGY))
            {
                energyBoost = std::min(energyBoost, 25);
                SetHitDamage(int32(GetHitDamage() * (1.0f + energyBoost / 25.0f)));
                player->ModifyPower(POWER_ENERGY, -energyBoost);
                energyCost += energyBoost;
            }

            if (AuraEffect const* glyph = player->GetAuraEffect(SPELL_DRUID_GLYPH_OF_FEROCIOUS_BITE, EFFECT_1))
                if (glyph->GetAmount() > 0)
                    player->CastCustomSpell(SPELL_DRUID_GLYPH_OF_FEROCIOUS_BITE_HEAL, SPELLVALUE_BASE_POINT0,
                        energyCost * 2 / glyph->GetAmount(), player, true);

            if (target->GetHealthPct() < 25.0f)
                if (Aura* rip = target->GetAura(SPELL_DRUID_RIP, player->GetGUID()))
                    rip->RefreshDuration();
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_ferocious_bite_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_ferocious_bite_SpellScript();
    }
};

// 33831, 102693, 102703, 102706 - Force of Nature
class spell_dru_force_of_nature : public SpellScriptLoader
{
public:
    enum TreantRole
    {
        TREANT_BALANCE,
        TREANT_RESTORATION,
        TREANT_FERAL,
        TREANT_GUARDIAN
    };

    spell_dru_force_of_nature(char const* scriptName, TreantRole role)
        : SpellScriptLoader(scriptName), _role(role) { }

    class spell_dru_force_of_nature_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_force_of_nature_SpellScript);

    public:
        explicit spell_dru_force_of_nature_SpellScript(TreantRole role)
            : _role(role), _targetGuid(0) { }

        void CaptureTarget()
        {
            if (Unit* target = GetExplTargetUnit())
                _targetGuid = target->GetGUID();
        }

        void HandleCast()
        {
            Unit* caster = GetCaster();
            Unit* target = caster && _targetGuid ? ObjectAccessor::GetUnit(*caster, _targetGuid) : NULL;
            if (!target && caster && caster->GetTypeId() == TypeID::TYPEID_PLAYER)
                target = caster->ToPlayer()->GetSelectedUnit();
            if (!caster || !target)
                return;

            // Resolve the summoned NPC entry from the actual SUMMON effect (not a hard-coded effect index).
            uint32 entry = 0;
            SpellInfo const* spellInfo = GetSpellInfo();
            for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
            {
                if (spellInfo->Effects[i].IsEffect(SPELL_EFFECT_SUMMON))
                {
                    entry = spellInfo->Effects[i].MiscValue;
                    break;
                }
            }
            if (!entry)
                entry = spellInfo->Effects[EFFECT_1].MiscValue;

            std::list<Creature*> minions;
            caster->GetAllMinionsByEntry(minions, entry);
            if (minions.empty())
                caster->GetCreatureListWithEntryInGrid(minions, entry, 100.0f);

            TempSummon* treant = NULL;
            uint32 newestTimer = 0;
            for (Creature* minion : minions)
            {
                TempSummon* summon = minion->ToTempSummon();
                if (!summon || !summon->IsAlive() || summon->GetSummonerGUID() != caster->GetGUID())
                    continue;

                if (!treant || summon->GetTimer() >= newestTimer)
                {
                    treant = summon;
                    newestTimer = summon->GetTimer();
                }
            }

            if (!treant)
                return;

            treant->CastSpell(treant, SPELL_DRUID_FORCE_OF_NATURE_VISUAL, true);
            treant->SetReactState(REACT_AGGRESSIVE);

            switch (_role)
            {
                case TREANT_BALANCE:
                    if (treant->IsAIEnabled)
                        treant->AI()->SetGUID(target->GetGUID());
                    break;
                case TREANT_RESTORATION:
                    treant->CastSpell(target, SPELL_DRUID_FORCE_OF_NATURE_SWIFTMEND, true);
                    break;
                case TREANT_FERAL:
                {
                    float damage = spellInfo->Effects[EFFECT_0].CalcValue(caster) + 1.0f;
                    treant->SetBaseWeaponDamage(WeaponAttackType::BASE_ATTACK, WeaponDamageRange::MINDAMAGE, damage);
                    treant->SetBaseWeaponDamage(WeaponAttackType::BASE_ATTACK, WeaponDamageRange::MAXDAMAGE, damage);
                    treant->UpdateAttackPowerAndDamage();
                    // Tooltip: immediately root the current target, then melee.
                    treant->CastSpell(target, SPELL_DRUID_FORCE_OF_NATURE_ROOT, true);
                    treant->Attack(target, true);
                    treant->GetMotionMaster()->Clear(true);
                    treant->GetMotionMaster()->MoveChase(target);
                    if (treant->IsAIEnabled)
                        treant->AI()->SetGUID(target->GetGUID());
                    break;
                }
                case TREANT_GUARDIAN:
                {
                    float damage = spellInfo->Effects[EFFECT_0].CalcValue(caster) * 0.2f + 1.0f;
                    treant->SetBaseWeaponDamage(WeaponAttackType::BASE_ATTACK, WeaponDamageRange::MINDAMAGE, damage);
                    treant->SetBaseWeaponDamage(WeaponAttackType::BASE_ATTACK, WeaponDamageRange::MAXDAMAGE, damage);
                    treant->UpdateAttackPowerAndDamage();
                    treant->CastSpell(target, SPELL_DRUID_FORCE_OF_NATURE_TAUNT, true);
                    treant->Attack(target, true);
                    treant->GetMotionMaster()->Clear(true);
                    treant->GetMotionMaster()->MoveChase(target);
                    if (treant->IsAIEnabled)
                        treant->AI()->SetGUID(target->GetGUID());
                    break;
                }
            }
        }

        void Register() override
        {
            BeforeCast += SpellCastFn(spell_dru_force_of_nature_SpellScript::CaptureTarget);
            AfterCast += SpellCastFn(spell_dru_force_of_nature_SpellScript::HandleCast);
        }

    private:
        TreantRole _role;
        uint64 _targetGuid;
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_force_of_nature_SpellScript(_role);
    }

private:
    TreantRole _role;
};

class npc_force_of_nature_balance : public CreatureScript
{
public:
    npc_force_of_nature_balance() : CreatureScript("npc_force_of_nature_balance") { }

    struct npc_force_of_nature_balanceAI : public ScriptedAI
    {
        explicit npc_force_of_nature_balanceAI(Creature* creature)
            : ScriptedAI(creature), _focusGuid(0), _needRoot(false), _awaitingInitial(true) { }

        void IsSummonedBy(Unit* summoner) OVERRIDE
        {
            me->SetReactState(REACT_AGGRESSIVE);
            if (me->HasUnitTypeMask(UNIT_MASK_MINION))
                static_cast<Minion*>(me)->SetFollowAngle(PET_FOLLOW_ANGLE);

            if (Unit* target = SelectInitialTarget(summoner))
                EngageTarget(target);
        }

        void SetGUID(uint64 guid, int32 /*id*/) OVERRIDE
        {
            if (Unit* target = ObjectAccessor::GetUnit(*me, guid))
                EngageTarget(target);
        }

        void OwnerAttacked(Unit* target) OVERRIDE
        {
            if (_awaitingInitial || !target || !target->IsAlive() || !me->IsValidAttackTarget(target))
                return;

            Unit* focus = _focusGuid ? ObjectAccessor::GetUnit(*me, _focusGuid) : NULL;
            if (focus && focus->IsAlive())
                return;

            EngageTarget(target);
        }

        void EnterEvadeMode() OVERRIDE
        {
            Unit* focus = _focusGuid ? ObjectAccessor::GetUnit(*me, _focusGuid) : NULL;
            if (focus && focus->IsAlive() && me->IsValidAttackTarget(focus))
            {
                AttackStart(focus);
                return;
            }

            if (Unit* owner = me->GetCharmerOrOwner())
                FollowOwner(owner);
            else
                ScriptedAI::EnterEvadeMode();
        }

        void AttackStart(Unit* target) OVERRIDE
        {
            if (!target || !me->Attack(target, false))
                return;

            float castRange = GetSpellRange(SPELL_DRUID_FORCE_OF_NATURE_WRATH);
            if (me->IsWithinDist(target, castRange))
            {
                me->StopMoving();
                me->GetMotionMaster()->Clear(false);
                me->SetFacingToObject(target);
            }
            else
                me->GetMotionMaster()->MoveChase(target, castRange * 0.8f);
        }

        void UpdateAI(uint32 /*diff*/) OVERRIDE
        {
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            Unit* owner = me->GetCharmerOrOwner();
            if (_awaitingInitial)
                return;

            Unit* focus = _focusGuid ? ObjectAccessor::GetUnit(*me, _focusGuid) : NULL;
            if (!focus || !focus->IsAlive())
            {
                _focusGuid = 0;
                _needRoot = false;
                me->AttackStop();
                me->InterruptNonMeleeSpells(false);

                if (Unit* newTarget = SelectCombatTarget(owner))
                {
                    EngageTarget(newTarget);
                    return;
                }

                if (owner)
                    FollowOwner(owner);
                return;
            }

            if (_needRoot)
            {
                me->CastSpell(focus, SPELL_DRUID_FORCE_OF_NATURE_ROOT, true);
                _needRoot = false;
            }

            float castRange = GetSpellRange(SPELL_DRUID_FORCE_OF_NATURE_WRATH);
            if (!me->IsWithinDist(focus, castRange) || !me->IsWithinLOSInMap(focus))
            {
                if (!me->HasUnitState(UNIT_STATE_CHASE))
                    me->GetMotionMaster()->MoveChase(focus, castRange * 0.8f);
                return;
            }

            if (me->isMoving() || me->HasUnitState(UNIT_STATE_CHASE))
            {
                me->StopMoving();
                me->GetMotionMaster()->Clear(false);
            }

            me->SetFacingToObject(focus);
            DoCast(focus, SPELL_DRUID_FORCE_OF_NATURE_WRATH, false);
        }

    private:
        float GetSpellRange(uint32 spellId) const
        {
            if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId))
                return spellInfo->GetMaxRange(false);
            return 40.0f;
        }

        Unit* SelectInitialTarget(Unit* owner) const
        {
            if (!owner)
                return NULL;

            if (Player* player = owner->ToPlayer())
                if (Unit* selected = player->GetSelectedUnit())
                    if (me->IsValidAttackTarget(selected))
                        return selected;

            if (Unit* victim = owner->GetVictim())
                if (me->IsValidAttackTarget(victim))
                    return victim;

            return NULL;
        }

        Unit* SelectCombatTarget(Unit* owner) const
        {
            if (!owner || !owner->IsInCombat())
                return NULL;

            if (Unit* victim = owner->GetVictim())
                if (me->IsValidAttackTarget(victim))
                    return victim;

            if (Player* player = owner->ToPlayer())
                if (Unit* selected = player->GetSelectedUnit())
                    if (me->IsValidAttackTarget(selected))
                        return selected;

            if (Unit* helper = owner->getAttackerForHelper())
                if (me->IsValidAttackTarget(helper))
                    return helper;

            return NULL;
        }

        void EngageTarget(Unit* target)
        {
            _focusGuid = target->GetGUID();
            _awaitingInitial = false;
            _needRoot = true;
            AttackStart(target);
        }

        void FollowOwner(Unit* owner)
        {
            if (!owner)
                return;

            me->GetMotionMaster()->Clear(false);
            me->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);
        }

        uint64 _focusGuid;
        bool _needRoot;
        bool _awaitingInitial;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_force_of_nature_balanceAI(creature);
    }
};

class npc_force_of_nature_restoration : public CreatureScript
{
public:
    npc_force_of_nature_restoration() : CreatureScript("npc_force_of_nature_restoration") { }

    struct npc_force_of_nature_restorationAI : public ScriptedAI
    {
        explicit npc_force_of_nature_restorationAI(Creature* creature)
            : ScriptedAI(creature) { }

        void IsSummonedBy(Unit* summoner) OVERRIDE
        {
            me->SetReactState(REACT_PASSIVE);
            if (me->HasUnitTypeMask(UNIT_MASK_MINION))
                static_cast<Minion*>(me)->SetFollowAngle(PET_FOLLOW_ANGLE);

            FollowOwner(summoner);
        }

        void EnterEvadeMode() OVERRIDE
        {
            me->InterruptNonMeleeSpells(false);
            if (Unit* owner = me->GetCharmerOrOwner())
                FollowOwner(owner);
            else
                ScriptedAI::EnterEvadeMode();
        }

        void UpdateAI(uint32 /*diff*/) OVERRIDE
        {
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            Unit* owner = me->GetCharmerOrOwner();
            if (!owner)
                return;

            float castRange = GetSpellRange(SPELL_DRUID_FORCE_OF_NATURE_HEAL);
            if (!me->IsWithinDist(owner, castRange))
            {
                if (!me->HasUnitState(UNIT_STATE_FOLLOW) && !me->HasUnitState(UNIT_STATE_CHASE))
                    FollowOwner(owner);
                return;
            }

            if (me->isMoving() || me->HasUnitState(UNIT_STATE_CHASE))
            {
                me->StopMoving();
                me->GetMotionMaster()->Clear(false);
            }

            // Healing Touch (113828) selects the most injured nearby friendly target.
            DoCast(me, SPELL_DRUID_FORCE_OF_NATURE_HEAL, false);
        }

    private:
        float GetSpellRange(uint32 spellId) const
        {
            if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId))
                return spellInfo->GetMaxRange(true);
            return 40.0f;
        }

        void FollowOwner(Unit* owner)
        {
            if (!owner)
                return;

            me->GetMotionMaster()->Clear(false);
            me->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_force_of_nature_restorationAI(creature);
    }
};

class npc_force_of_nature_melee : public CreatureScript
{
public:
    enum MeleeRole
    {
        MELEE_FERAL,
        MELEE_GUARDIAN
    };

    npc_force_of_nature_melee(char const* scriptName, MeleeRole role)
        : CreatureScript(scriptName), _role(role) { }

    struct npc_force_of_nature_meleeAI : public ScriptedAI
    {
        explicit npc_force_of_nature_meleeAI(Creature* creature, MeleeRole role)
            : ScriptedAI(creature), _role(role), _focusGuid(0), _hasEngaged(false), _focusFinished(false), _didSpecial(false) { }

        void IsSummonedBy(Unit* summoner) OVERRIDE
        {
            me->SetReactState(REACT_AGGRESSIVE);
            if (me->HasUnitTypeMask(UNIT_MASK_MINION))
                static_cast<Minion*>(me)->SetFollowAngle(PET_FOLLOW_ANGLE);

            // Engage immediately — never MoveFollow before we have a focus (that sends them to the caster).
            if (Unit* target = SelectInitialTarget(summoner))
                EngageTarget(target);
        }

        void SetGUID(uint64 guid, int32 /*id*/) OVERRIDE
        {
            if (_focusFinished)
                return;

            if (Unit* target = ObjectAccessor::GetUnit(*me, guid))
                EngageTarget(target);
        }

        // Player-owned guardians evade when the owner is OOC; PetAI ignores evade. Do the same while focused.
        void EnterEvadeMode() OVERRIDE
        {
            if (_hasEngaged && !_focusFinished)
            {
                if (Unit* focus = ObjectAccessor::GetUnit(*me, _focusGuid))
                {
                    if (focus->IsAlive())
                    {
                        StartAttackAndChase(focus);
                        return;
                    }
                }
            }

            // Not engaged yet, or focus is dead: stay put (do NOT follow owner / "cast position").
            me->AttackStop();
            me->GetMotionMaster()->Clear(false);
            me->GetMotionMaster()->MoveIdle();
        }

        void MoveInLineOfSight(Unit* who) OVERRIDE
        {
            if (_hasEngaged && !_focusFinished)
                return;

            ScriptedAI::MoveInLineOfSight(who);
        }

        void AttackStart(Unit* target) OVERRIDE
        {
            StartAttackAndChase(target);
        }

        void UpdateAI(uint32 /*diff*/) OVERRIDE
        {
            Unit* owner = me->GetCharmerOrOwner();
            if (!_hasEngaged)
            {
                if (Unit* target = SelectInitialTarget(owner))
                    EngageTarget(target);
                return;
            }

            Unit* focus = (!_focusFinished && _focusGuid) ? ObjectAccessor::GetUnit(*me, _focusGuid) : NULL;
            if (!focus || !focus->IsAlive())
            {
                _focusGuid = 0;
                _focusFinished = true;
                me->AttackStop();
                me->InterruptNonMeleeSpells(false);
                if (owner)
                    FollowOwner(owner);
                return;
            }

            StartAttackAndChase(focus);

            if (!me->IsWithinMeleeRange(focus))
                return;

            DoMeleeAttackIfReady();
        }

    private:
        Unit* SelectInitialTarget(Unit* owner) const
        {
            if (!owner)
                return NULL;

            // Prefer the unit the player has targeted (same as the FoN cast target).
            if (uint64 targetGuid = owner->GetUInt64Value(UNIT_FIELD_TARGET))
                if (Unit* hardTarget = ObjectAccessor::GetUnit(*me, targetGuid))
                    if (hardTarget->IsAlive() && !me->IsFriendlyTo(hardTarget))
                        return hardTarget;

            if (Player* player = owner->ToPlayer())
                if (Unit* selected = player->GetSelectedUnit())
                    if (selected->IsAlive() && !me->IsFriendlyTo(selected))
                        return selected;

            if (Unit* victim = owner->GetVictim())
                if (victim->IsAlive() && !me->IsFriendlyTo(victim))
                    return victim;

            return NULL;
        }

        void StartAttackAndChase(Unit* target)
        {
            if (!target)
                return;

            me->SetReactState(REACT_AGGRESSIVE);
            me->ClearUnitState(UNIT_STATE_EVADE);

            if (!me->Attack(target, true))
            {
                me->SetInCombatWith(target);
                target->SetInCombatWith(me);
                me->SetTarget(target->GetGUID());
            }

            // Always (re)apply chase so a prior MoveFollow to the caster cannot stick.
            if (!me->IsWithinMeleeRange(target) || me->GetVictim() != target ||
                me->HasUnitState(UNIT_STATE_FOLLOW) || !me->HasUnitState(UNIT_STATE_CHASE))
            {
                me->GetMotionMaster()->Clear(true);
                me->GetMotionMaster()->MoveChase(target);
            }
        }

        void EngageTarget(Unit* target)
        {
            _focusGuid = target->GetGUID();
            _hasEngaged = true;
            _focusFinished = false;
            StartAttackAndChase(target);

            if (!_didSpecial)
            {
                if (_role == MELEE_FERAL)
                    me->CastSpell(target, SPELL_DRUID_FORCE_OF_NATURE_ROOT, true);
                else
                    me->CastSpell(target, SPELL_DRUID_FORCE_OF_NATURE_TAUNT, true);
                _didSpecial = true;
            }
        }

        void FollowOwner(Unit* owner)
        {
            if (!owner)
                return;

            me->GetMotionMaster()->Clear(false);
            me->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);
            me->SetReactState(REACT_AGGRESSIVE);
        }

        MeleeRole _role;
        uint64 _focusGuid;
        bool _hasEngaged;
        bool _focusFinished;
        bool _didSpecial;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_force_of_nature_meleeAI(creature, _role);
    }

private:
    MeleeRole _role;
};

class DelayedWildMushroomInvisEvent : public BasicEvent
{
public:
    explicit DelayedWildMushroomInvisEvent(uint64 mushroomGuid) : _mushroomGuid(mushroomGuid) { }

    bool Execute(uint64 /*e_time*/, uint32 /*p_time*/) OVERRIDE
    {
        if (Creature* mushroom = HashMapHolder<Creature>::Find(_mushroomGuid))
            if (mushroom->IsAlive())
                mushroom->CastSpell(mushroom, SPELL_DRUID_MUSHROOM_INVISIBLE, true);
        return true;
    }

private:
    uint64 _mushroomGuid;
};

static void GetWildMushrooms(Unit* owner, std::list<Creature*>& mushrooms)
{
    owner->GetAllMinionsByEntry(mushrooms, NPC_DRUID_WILD_MUSHROOM);
}

static Creature* FindNewestWildMushroom(std::list<Creature*> const& mushrooms)
{
    Creature* newest = NULL;
    uint32 newestTimer = 0;
    for (Creature* mushroom : mushrooms)
    {
        if (!mushroom->IsAlive())
            continue;

        // Just-summoned mushrooms have not received birth/invis auras yet.
        if (!mushroom->HasAura(SPELL_DRUID_MUSHROOM_BIRTH)
            && !mushroom->HasAura(SPELL_DRUID_MUSHROOM_INVISIBLE))
            return mushroom;

        TempSummon const* summon = mushroom->ToTempSummon();
        if (!summon)
            continue;

        uint32 timer = summon->GetTimer();
        if (!newest || timer >= newestTimer)
        {
            newest = mushroom;
            newestTimer = timer;
        }
    }
    return newest;
}

static Creature* FindOldestWildMushroom(std::list<Creature*> const& mushrooms, Creature const* exclude)
{
    Creature* oldest = NULL;
    uint32 oldestTimer = std::numeric_limits<uint32>::max();
    for (Creature* mushroom : mushrooms)
    {
        if (mushroom == exclude || !mushroom->IsAlive())
            continue;

        TempSummon const* summon = mushroom->ToTempSummon();
        if (!summon)
            continue;

        uint32 timer = summon->GetTimer();
        if (timer < oldestTimer)
        {
            oldest = mushroom;
            oldestTimer = timer;
        }
    }
    return oldest;
}

// 88747 - Wild Mushroom (Balance)
// 145205 - Wild Mushroom (Restoration)
// 147349 - Wild Mushroom (Restoration with glyph)
class spell_dru_wild_mushroom : public SpellScriptLoader
{
public:
    spell_dru_wild_mushroom() : SpellScriptLoader("spell_dru_wild_mushroom") { }

    class spell_dru_wild_mushroom_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_wild_mushroom_SpellScript);

        int32 _amount0 = 0;
        int32 _amount1 = 0;

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return sSpellMgr->GetSpellInfo(SPELL_DRUID_MUSHROOM_BIRTH)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_MUSHROOM_INVISIBLE)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_MUSHROOM_ACCUMULATOR)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_MUSHROOM_OVERHEAL_AURA);
        }

        void GetBonus()
        {
            Unit* druid = GetCaster();
            if (!druid)
                return;

            std::list<Creature*> mushrooms;
            GetWildMushrooms(druid, mushrooms);
            if (mushrooms.empty())
                return;

            if (!druid->HasAura(SPELL_DRUID_MUSHROOM_OVERHEAL_AURA))
                return;

            if (Aura const* aura = mushrooms.front()->GetAura(SPELL_DRUID_MUSHROOM_ACCUMULATOR))
            {
                if (aura->HasEffect(EFFECT_0))
                    _amount0 = aura->GetEffect(EFFECT_0)->GetAmount();
                if (aura->HasEffect(EFFECT_1))
                    _amount1 = aura->GetEffect(EFFECT_1)->GetAmount();
            }
        }

        void HandleCast()
        {
            Unit* druid = GetCaster();
            if (!druid)
                return;

            uint32 spellId = GetSpellInfo()->Id;
            if (spellId == SPELL_DRUID_WILD_MUSHROOM_RESTO || spellId == SPELL_DRUID_WILD_MUSHROOM_RESTO_GLYPH)
                if (!druid->HasAura(SPELL_DRUID_MUSHROOM_OVERHEAL_AURA))
                    druid->CastSpell(druid, SPELL_DRUID_MUSHROOM_OVERHEAL_AURA, true);

            std::list<Creature*> mushrooms;
            GetWildMushrooms(druid, mushrooms);
            if (mushrooms.empty())
                return;

            Creature* mushroomCreature = FindNewestWildMushroom(mushrooms);
            if (!mushroomCreature)
                return;

            TempSummon* mushroom = mushroomCreature->ToTempSummon();
            if (!mushroom)
                return;

            // Minion InitStats does not copy owner level (unlike totems/guardians).
            mushroom->SetLevel(druid->getLevel());

            // EFFECT_0 is mushroom HP (5), not level.
            int32 health = GetSpellInfo()->Effects[EFFECT_0].BasePoints;
            if (health > 0)
            {
                mushroom->SetMaxHealth(health);
                mushroom->SetHealth(health);
            }

            mushroom->CastSpell(mushroom, SPELL_DRUID_MUSHROOM_BIRTH, true);

            int32 invisSeconds = GetSpellInfo()->Effects[EFFECT_2].BasePoints;
            if (invisSeconds > 0)
                mushroom->m_Events.AddEvent(new DelayedWildMushroomInvisEvent(mushroom->GetGUID()),
                    mushroom->m_Events.CalculateTime(uint32(invisSeconds) * IN_MILLISECONDS));

            if (druid->HasAura(SPELL_DRUID_MUSHROOM_OVERHEAL_AURA))
                druid->CastCustomSpell(mushroom, SPELL_DRUID_MUSHROOM_ACCUMULATOR, &_amount0, &_amount1, NULL, true);

            int32 limit = GetSpellInfo()->Effects[EFFECT_1].BasePoints;
            if (limit <= 0)
                limit = 1;

            while (mushrooms.size() > size_t(limit))
            {
                Creature* oldest = FindOldestWildMushroom(mushrooms, mushroomCreature);
                if (!oldest)
                    break;

                if (TempSummon* old = oldest->ToTempSummon())
                    old->UnSummon();
                mushrooms.remove(oldest);
            }
        }

        void Register() override
        {
            BeforeCast += SpellCastFn(spell_dru_wild_mushroom_SpellScript::GetBonus);
            AfterCast += SpellCastFn(spell_dru_wild_mushroom_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_wild_mushroom_SpellScript();
    }
};

// 138611 - Wild Mushroom (Restoration overheal accumulator driver)
class spell_dru_wild_mushroom_overheal : public SpellScriptLoader
{
public:
    spell_dru_wild_mushroom_overheal() : SpellScriptLoader("spell_dru_wild_mushroom_overheal") { }

    class spell_dru_wild_mushroom_overheal_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_wild_mushroom_overheal_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return sSpellMgr->GetSpellInfo(SPELL_DRUID_MUSHROOM_ACCUMULATOR)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_MUSHROOM_BLOOM_UI_FLASH)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_REJUVENATION);
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            // MoP: only Rejuvenation overhealing feeds the mushroom.
            return spellInfo && spellInfo->Id == SPELL_DRUID_REJUVENATION;
        }

        void HandleProc(ProcEventInfo& eventInfo)
        {
            HealInfo* healInfo = eventInfo.GetHealInfo();
            if (!healInfo)
                return;

            int32 over = int32(healInfo->GetOverheal());
            Player* druid = GetUnitOwner()->ToPlayer();
            if (!over || !druid)
                return;

            if (druid->GetMap()->IsBattlegroundOrArena())
                if (Battleground* bg = druid->GetBattleground())
                    if (bg->GetStatus() == STATUS_WAIT_JOIN)
                        return;

            std::list<Creature*> mushrooms;
            GetWildMushrooms(druid, mushrooms);
            if (mushrooms.empty())
                return;

            Unit* mushroom = mushrooms.front();
            Aura* accumulator = mushroom->GetAura(SPELL_DRUID_MUSHROOM_ACCUMULATOR);
            if (!accumulator)
                return;

            AuraEffect* scale = accumulator->GetEffect(EFFECT_0);
            AuraEffect* bonus = accumulator->GetEffect(EFFECT_1);
            if (!scale || !bonus)
                return;

            // EFFECT_0 of 138611 is "% of overhealing" (normally 100).
            int32 pct = 100;
            if (AuraEffect const* pctEff = GetEffect(EFFECT_0))
                if (pctEff->GetAmount() > 0)
                    pct = pctEff->GetAmount();

            over = CalculatePct(over, pct);
            if (over <= 0)
                return;

            int32 max = int32(druid->GetMaxHealth()) * 2;
            int32 val = std::min(bonus->GetAmount() + over, max);
            if (bonus->GetAmount() != val)
            {
                int32 scaleAmount = int32(val * 100.f / float(max));
                scale->ChangeAmount(scaleAmount);
                bonus->ChangeAmount(val);
                // ChangeAmount does not mark dummy amounts for aura tooltip refresh.
                accumulator->SetNeedClientUpdateForTargets();
                if (val == max && !druid->HasAura(SPELL_DRUID_MUSHROOM_BLOOM_UI_FLASH))
                    druid->CastSpell(druid, SPELL_DRUID_MUSHROOM_BLOOM_UI_FLASH, true);
            }
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_dru_wild_mushroom_overheal_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_dru_wild_mushroom_overheal_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_wild_mushroom_overheal_AuraScript();
    }
};

// 47649 - Wild Mushroom
class npc_wild_mushroom : public CreatureScript
{
public:
    npc_wild_mushroom() : CreatureScript("npc_wild_mushroom") { }

    struct npc_wild_mushroomAI : public ScriptedAI
    {
        npc_wild_mushroomAI(Creature* creature) : ScriptedAI(creature)
        {
            me->SetReactState(REACT_PASSIVE);
        }

        void IsSummonedBy(Unit* /*summoner*/) override
        {
            // Stay planted; owned minions otherwise MoveFollow the owner after evade.
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            me->AddUnitState(UNIT_STATE_ROOT);
            me->GetMotionMaster()->Clear(false);
            me->GetMotionMaster()->MoveIdle();
        }

        void AttackStart(Unit* /*who*/) override { }

        void EnterCombat(Unit* /*who*/) override { }

        void EnterEvadeMode() override
        {
            if (!_EnterEvadeMode())
                return;

            me->GetMotionMaster()->Clear(false);
            me->GetMotionMaster()->MoveIdle();
            Reset();
        }

        void JustDied(Unit* /*killer*/) override
        {
            me->DespawnOrUnsummon(1);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_wild_mushroomAI(creature);
    }
};

// 102791 - Wild Mushroom: Bloom
class spell_dru_wild_mushroom_bloom : public SpellScriptLoader
{
public:
    spell_dru_wild_mushroom_bloom() : SpellScriptLoader("spell_dru_wild_mushroom_bloom") { }

    class spell_dru_wild_mushroom_bloom_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_wild_mushroom_bloom_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return sSpellMgr->GetSpellInfo(SPELL_DRUID_MUSHROOM_HEAL)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_MUSHROOM_DEATH)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_MUSHROOM_SUICIDE);
        }

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            if (!caster)
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;

            std::list<Creature*> mushrooms;
            GetWildMushrooms(caster, mushrooms);
            if (mushrooms.empty())
                return SpellCastResult::SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

            for (Creature* mushroom : mushrooms)
                if (mushroom->IsAlive() && caster->IsWithinDistInMap(mushroom, 40.0f))
                    return SpellCastResult::SPELL_CAST_OK;

            return SpellCastResult::SPELL_FAILED_OUT_OF_RANGE;
        }

        void HandleCast()
        {
            Unit* druid = GetCaster();
            if (!druid)
                return;

            std::list<Creature*> mushrooms;
            GetWildMushrooms(druid, mushrooms);
            for (Creature* mushroom : mushrooms)
            {
                if (!mushroom->IsAlive() || !druid->IsWithinDistInMap(mushroom, 40.0f))
                    continue;

                druid->CastSpell(mushroom->GetPositionX(), mushroom->GetPositionY(), mushroom->GetPositionZ(),
                    SPELL_DRUID_MUSHROOM_HEAL, true);
                mushroom->CastSpell(mushroom, SPELL_DRUID_MUSHROOM_DEATH, true);
                mushroom->DespawnOrUnsummon(500);
            }

            druid->RemoveAurasDueToSpell(SPELL_DRUID_MUSHROOM_BLOOM_UI_FLASH);
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dru_wild_mushroom_bloom_SpellScript::CheckCast);
            OnCast += SpellCastFn(spell_dru_wild_mushroom_bloom_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_wild_mushroom_bloom_SpellScript();
    }
};

// 102792 - Wild Mushroom: Bloom heal
class spell_dru_wild_mushroom_heal : public SpellScriptLoader
{
public:
    spell_dru_wild_mushroom_heal() : SpellScriptLoader("spell_dru_wild_mushroom_heal") { }

    class spell_dru_wild_mushroom_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_wild_mushroom_heal_SpellScript);

        int32 _bonusHeal = 0;

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return sSpellMgr->GetSpellInfo(SPELL_DRUID_MUSHROOM_ACCUMULATOR);
        }

        void SelectTargets(std::list<WorldObject*>& targets)
        {
            WorldLocation const* dest = GetExplTargetDest();
            if (!dest)
            {
                targets.clear();
                return;
            }

            float x = dest->GetPositionX();
            float y = dest->GetPositionY();
            float z = dest->GetPositionZ();

            targets.remove_if([x, y, z](WorldObject* target)
            {
                if (!target->IsWithinLOS(x, y, z))
                    return true;
                return target->GetEntry() == NPC_DRUID_WILD_MUSHROOM;
            });

            _bonusHeal = 0;
            if (targets.empty())
                return;

            Unit* druid = GetCaster();
            if (!druid)
                return;

            std::list<Creature*> mushrooms;
            GetWildMushrooms(druid, mushrooms);
            if (mushrooms.empty())
                return;

            if (AuraEffect const* eff = mushrooms.front()->GetAuraEffect(SPELL_DRUID_MUSHROOM_ACCUMULATOR, EFFECT_1))
                _bonusHeal = eff->GetAmount() / int32(targets.size());
        }

        void CalculateHeal(SpellEffIndex /*effIndex*/)
        {
            if (_bonusHeal)
                SetHitHeal(GetHitHeal() + _bonusHeal);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_wild_mushroom_heal_SpellScript::SelectTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
            OnEffectHitTarget += SpellEffectFn(spell_dru_wild_mushroom_heal_SpellScript::CalculateHeal, EFFECT_0, SPELL_EFFECT_HEAL);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_wild_mushroom_heal_SpellScript();
    }
};

// 88751 - Wild Mushroom: Detonate
class spell_dru_wild_mushroom_detonate : public SpellScriptLoader
{
public:
    spell_dru_wild_mushroom_detonate() : SpellScriptLoader("spell_dru_wild_mushroom_detonate") { }

    class spell_dru_wild_mushroom_detonate_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_wild_mushroom_detonate_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return sSpellMgr->GetSpellInfo(SPELL_DRUID_MUSHROOM_DAMAGE)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_FUNGAL_GROWTH)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_MUSHROOM_DEATH)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_MUSHROOM_SUICIDE);
        }

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            if (!caster)
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;

            std::list<Creature*> mushrooms;
            GetWildMushrooms(caster, mushrooms);
            if (mushrooms.empty())
                return SpellCastResult::SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

            for (Creature* mushroom : mushrooms)
                if (mushroom->IsAlive() && caster->IsWithinDistInMap(mushroom, 40.0f))
                    return SpellCastResult::SPELL_CAST_OK;

            return SpellCastResult::SPELL_FAILED_OUT_OF_RANGE;
        }

        void HandleCast()
        {
            Unit* druid = GetCaster();
            if (!druid)
                return;

            std::list<Creature*> mushrooms;
            GetWildMushrooms(druid, mushrooms);
            for (Creature* mushroom : mushrooms)
            {
                if (!mushroom->IsAlive() || !druid->IsWithinDistInMap(mushroom, 40.0f))
                    continue;

                druid->CastSpell(mushroom, SPELL_DRUID_MUSHROOM_DAMAGE, true);
                druid->CastSpell(mushroom->GetPositionX(), mushroom->GetPositionY(), mushroom->GetPositionZ(),
                    SPELL_DRUID_FUNGAL_GROWTH, true);
                mushroom->CastSpell(mushroom, SPELL_DRUID_MUSHROOM_DEATH, true);
                // Suicide aura (116302) is 500ms and races its first tick; force despawn so
                // the Wild Mushroom model is replaced by Fungal Growth (invisible stalker + ground FX).
                mushroom->DespawnOrUnsummon(500);
            }
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dru_wild_mushroom_detonate_SpellScript::CheckCast);
            OnCast += SpellCastFn(spell_dru_wild_mushroom_detonate_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_wild_mushroom_detonate_SpellScript();
    }
};

// 43484 - Fungal Growth
class npc_fungal_growth : public CreatureScript
{
public:
    npc_fungal_growth() : CreatureScript("npc_fungal_growth") { }

    struct npc_fungal_growthAI : public NullCreatureAI
    {
        npc_fungal_growthAI(Creature* creature) : NullCreatureAI(creature)
        {
            me->SetReactState(REACT_PASSIVE);
            me->CastSpell(me, SPELL_DRUID_MUSHROOM_SLOW_VISUAL, true);
            me->CastSpell(me, SPELL_DRUID_FUNGAL_GROWTH_SLOW, true);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_fungal_growthAI(creature);
    }
};

// 779 - Swipe (Bear), 62078 - Swipe (Cat), 6807 - Maul
// Extra damage vs bleeding targets (MoP: Swipe $s2%, Maul same script).
class spell_dru_swipe_and_maul_bleed : public SpellScriptLoader
{
public:
    spell_dru_swipe_and_maul_bleed() : SpellScriptLoader("spell_dru_swipe_and_maul_bleed") { }

    class spell_dru_swipe_and_maul_bleed_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_swipe_and_maul_bleed_SpellScript);

        void HandleHit()
        {
            Unit* target = GetHitUnit();
            if (!target || !target->HasAuraState(AURA_STATE_BLEEDING))
                return;

            int32 bonusPct = 20;
            // Bear/Cat Swipe store the bleed bonus on EFFECT_1 (dummy).
            if (GetSpellInfo()->Id == 779 || GetSpellInfo()->Id == 62078)
                bonusPct = GetSpellInfo()->Effects[EFFECT_1].CalcValue(GetCaster());

            if (bonusPct > 0)
                SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), bonusPct));
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_swipe_and_maul_bleed_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_swipe_and_maul_bleed_SpellScript();
    }
};

// 102795 - Bear Hug
class spell_dru_bear_hug : public SpellScriptLoader
{
public:
    spell_dru_bear_hug() : SpellScriptLoader("spell_dru_bear_hug") { }

    class spell_dru_bear_hug_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_bear_hug_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (caster->GetShapeshiftForm() != FORM_BEAR)
                caster->CastSpell(caster, SPELL_DRUID_BEAR_FORM, true);

            // EFFECT_1 base points are % of caster max health per tick.
            amount = int32(caster->CountPctFromMaxHealth(amount));
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_bear_hug_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_bear_hug_AuraScript();
    }
};

// 93400 - Shooting Stars
class spell_dru_shooting_stars : public SpellScriptLoader
{
public:
    spell_dru_shooting_stars() : SpellScriptLoader("spell_dru_shooting_stars") { }

    class spell_dru_shooting_stars_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_shooting_stars_SpellScript);

        void HandleHit()
        {
            if (Player* druid = GetCaster()->ToPlayer())
                druid->RemoveSpellCooldown(SPELL_DRUID_STARSURGE, true);
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_shooting_stars_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_shooting_stars_SpellScript();
    }
};

// 78675 - Solar Beam, 113286 - Solar Beam (Symbiosis)
class spell_dru_solar_beam : public SpellScriptLoader
{
public:
    spell_dru_solar_beam() : SpellScriptLoader("spell_dru_solar_beam") { }

    class spell_dru_solar_beam_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_solar_beam_AuraScript);

        uint32 GetSilenceSpell() const
        {
            return GetId() == SPELL_DRUID_SOLAR_BEAM
                ? SPELL_DRUID_SOLAR_BEAM_SILENCE
                : SPELL_DRUID_SOLAR_BEAM_SILENCE_SYMBIOSIS;
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetTarget();
            if (caster && target)
                caster->AddAura(GetSilenceSpell(), target);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetTarget();
            if (caster && target)
                target->RemoveAurasDueToSpell(GetSilenceSpell(), caster->GetGUID());
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_dru_solar_beam_AuraScript::HandleApply, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_solar_beam_AuraScript::HandleRemove, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_solar_beam_AuraScript();
    }
};

// 22842 - Frenzied Regeneration
class spell_dru_frenzied_regeneration : public SpellScriptLoader
{
public:
    spell_dru_frenzied_regeneration() : SpellScriptLoader("spell_dru_frenzied_regeneration") { }

    class spell_dru_frenzied_regeneration_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_frenzied_regeneration_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return sSpellMgr->GetSpellInfo(SPELL_DRUID_GLYPH_OF_FRENZIED_REGEN)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_FRENZIED_REGEN_HEAL_TAKE);
        }

        void HandleHit()
        {
            Player* druid = GetCaster()->ToPlayer();
            if (!druid)
                return;

            // Glyph: fixed rage cost, no rage-to-health convert; +healing taken instead.
            if (Aura const* glyph = druid->GetAura(SPELL_DRUID_GLYPH_OF_FRENZIED_REGEN))
            {
                SetHitHeal(0);
                druid->CastSpell(druid, SPELL_DRUID_FRENZIED_REGEN_HEAL_TAKE, true);
                // Tooltip ${$m3/10}: EFFECT_2 base points are rage*10 (power units).
                int32 rageCost = glyph->GetSpellInfo()->Effects[EFFECT_2].CalcValue(druid);
                if (rageCost > 0)
                {
                    int32 have = druid->GetPower(POWER_RAGE);
                    druid->EnergizeBySpell(druid, GetSpellInfo()->Id, -std::min(rageCost, have), POWER_RAGE);
                }
                return;
            }

            // Convert up to EFFECT_4 rage (tooltip "60") into health. Rage is stored *10.
            int32 maxRage = GetSpellInfo()->Effects[EFFECT_4].CalcValue(druid) * 10;
            if (maxRage <= 0)
                maxRage = 600;

            int32 rageUsed = std::min(maxRage, druid->GetPower(POWER_RAGE));
            int32 attackPower = int32(druid->GetTotalAttackPowerValue(WeaponAttackType::BASE_ATTACK));
            int32 agility = int32(druid->GetStat(STAT_AGILITY));
            int32 stamina = int32(druid->GetStat(STAT_STAMINA));

            // EFFECT_1 = 220% of (AP - 2*Agi); EFFECT_2 = 250% of Stamina; take the larger.
            int32 fromAttackPower = CalculatePct(std::max(attackPower - agility * 2, 0),
                GetSpellInfo()->Effects[EFFECT_1].CalcValue(druid));
            int32 fromStamina = CalculatePct(stamina, GetSpellInfo()->Effects[EFFECT_2].CalcValue(druid));
            int32 healAmount = std::max(fromAttackPower, fromStamina);
            healAmount = int32(int64(rageUsed) * healAmount / maxRage);

            if (healAmount > 0)
            {
                healAmount = int32(druid->SpellHealingBonusDone(druid, GetSpellInfo(), uint32(healAmount), HEAL));
                healAmount = int32(druid->SpellHealingBonusTaken(druid, GetSpellInfo(), uint32(healAmount), HEAL));
            }

            SetHitHeal(healAmount);

            if (rageUsed > 0)
                druid->EnergizeBySpell(druid, GetSpellInfo()->Id, -rageUsed, POWER_RAGE);
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_frenzied_regeneration_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_frenzied_regeneration_SpellScript();
    }
};

// 145108 - Ysera's Gift
// Every 5 sec: heal self for 5% max health, or the most injured nearby raid ally if at full health.
class spell_dru_yseras_gift : public SpellScriptLoader
{
public:
    spell_dru_yseras_gift() : SpellScriptLoader("spell_dru_yseras_gift") { }

    class spell_dru_yseras_gift_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_yseras_gift_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return sSpellMgr->GetSpellInfo(SPELL_DRUID_YSERAS_GIFT_HEAL_SELF) != NULL
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_YSERAS_GIFT_HEAL_ALLY) != NULL;
        }

        void HandlePeriodic(AuraEffect const* aurEff)
        {
            Unit* target = GetTarget();
            if (!target)
                return;

            int32 healAmount = int32(target->CountPctFromMaxHealth(aurEff->GetAmount()));
            if (healAmount <= 0)
                return;

            if (!target->IsFullHealth())
                target->CastCustomSpell(SPELL_DRUID_YSERAS_GIFT_HEAL_SELF, SPELLVALUE_BASE_POINT0, healAmount, target, true, NULL, aurEff);
            else
                target->CastCustomSpell(SPELL_DRUID_YSERAS_GIFT_HEAL_ALLY, SPELLVALUE_BASE_POINT0, healAmount, target, true, NULL, aurEff);
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_yseras_gift_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_yseras_gift_AuraScript();
    }
};

// 145110 - Ysera's Gift (ally heal): only the most injured raid member in range.
class spell_dru_yseras_gift_heal_ally : public SpellScriptLoader
{
public:
    spell_dru_yseras_gift_heal_ally() : SpellScriptLoader("spell_dru_yseras_gift_heal_ally") { }

    class spell_dru_yseras_gift_heal_ally_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_yseras_gift_heal_ally_SpellScript);

        void SelectTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if([](WorldObject* obj)
            {
                Unit* unit = obj->ToUnit();
                return !unit || !unit->IsAlive() || unit->IsFullHealth();
            });

            if (targets.size() <= 1)
                return;

            targets.sort([](WorldObject* a, WorldObject* b)
            {
                return Skyfire::HealthPctOrderPred()(a->ToUnit(), b->ToUnit());
            });
            targets.resize(1);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_yseras_gift_heal_ally_SpellScript::SelectTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_yseras_gift_heal_ally_SpellScript();
    }
};

void AddSC_druid_spell_scripts()
{
    new spell_dru_dash();
    new spell_dru_bear_hug();
    new spell_dru_eclipse("spell_dru_eclipse_lunar");
    new spell_dru_eclipse("spell_dru_eclipse_solar");
    new spell_dru_eclipse_energize();
    new spell_dru_ferocious_bite();
    new spell_dru_force_of_nature("spell_dru_force_of_nature_balance", spell_dru_force_of_nature::TREANT_BALANCE);
    new spell_dru_force_of_nature("spell_dru_force_of_nature_restoration", spell_dru_force_of_nature::TREANT_RESTORATION);
    new spell_dru_force_of_nature("spell_dru_force_of_nature_feral", spell_dru_force_of_nature::TREANT_FERAL);
    new spell_dru_force_of_nature("spell_dru_force_of_nature_guardian", spell_dru_force_of_nature::TREANT_GUARDIAN);
    new spell_dru_frenzied_regeneration();
    new spell_dru_glyph_of_innervate();
    new spell_dru_innervate();
    new spell_dru_lacerate();
    new spell_dru_thrash_bear();
    new spell_dru_faerie_fire();
    new spell_dru_faerie_swarm();
    new spell_dru_lifebloom();
    new spell_dru_lifebloom_refresh();
    new spell_dru_rejuvenation();
    new spell_dru_living_seed();
    new spell_dru_living_seed_proc();
    new spell_dru_might_of_ursoc();
    new spell_dru_predatory_strikes();
    new spell_dru_rake();
    new spell_dru_savage_defense();
    new spell_dru_savage_roar();
    new spell_dru_shooting_stars();
    new spell_dru_skull_bash();
    new spell_dru_solar_beam();
    new spell_dru_starfall_dummy();
    new spell_dru_survival_instincts();
    new spell_dru_swift_flight_passive();
    new spell_dru_swipe_and_maul_bleed();
    new spell_dru_t10_restoration_4p_bonus();
    new spell_dru_wild_mushroom();
    new spell_dru_wild_mushroom_bloom();
    new spell_dru_wild_mushroom_detonate();
    new spell_dru_wild_mushroom_heal();
    new spell_dru_wild_mushroom_overheal();
    new spell_dru_yseras_gift();
    new spell_dru_yseras_gift_heal_ally();
    new npc_force_of_nature_balance();
    new npc_force_of_nature_restoration();
    new npc_force_of_nature_melee("npc_force_of_nature_feral", npc_force_of_nature_melee::MELEE_FERAL);
    new npc_force_of_nature_melee("npc_force_of_nature_guardian", npc_force_of_nature_melee::MELEE_GUARDIAN);
    new npc_fungal_growth();
    new npc_wild_mushroom();
}
