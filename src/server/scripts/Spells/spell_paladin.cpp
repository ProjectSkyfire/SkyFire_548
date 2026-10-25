/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/*
 * Scripts for spells with SPELLFAMILY_PALADIN and SPELLFAMILY_GENERIC spells used by paladin players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_pal_".
 */

#include "Player.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Group.h"
#include "Containers.h"
#include "TemporarySummon.h"
#include "PetAI.h"
#include "PaladinSpellCalculations.h"
#include <map>

enum PaladinSpells
{
    SPELL_PALADIN_SELFLESS_HEALER_TALENT         = 85804,
    SPELL_PALADIN_SELFLESS_HEALER_BUFF           = 114250,
    SPELL_PALADIN_SELFLESS_HEALER_ENERGIZE       = 148502,
    SPELL_PALADIN_SELFLESS_HEALER_CHECKER        = 128863,

    SPELL_PALADIN_LONG_ARM_OF_THE_LAW_TALENT     = 87172,
    SPELL_PALADIN_LONG_ARM_OF_THE_LAW_SPEEDBUFF  = 87173,

    SPELL_PALADIN_AVENGERS_SHIELD                = 31935,
    SPELL_PALADIN_ARDENT_DEFENDER_HEAL           = 66235,
    SPELL_PALADIN_BEACON_OF_LIGHT_MARKER         = 53563,
    SPELL_PALADIN_BEACON_OF_LIGHT_HEAL           = 53652,
    SPELL_PALADIN_BLESSING_OF_LOWER_CITY_DRUID   = 37878,
    SPELL_PALADIN_BLESSING_OF_LOWER_CITY_PALADIN = 37879,
    SPELL_PALADIN_BLESSING_OF_LOWER_CITY_PRIEST  = 37880,
    SPELL_PALADIN_BLESSING_OF_LOWER_CITY_SHAMAN  = 37881,
    SPELL_PALADIN_DIVINE_PURPOSE_PROC            = 90174,
    SPELL_PALADIN_EYE_FOR_AN_EYE_RANK_1          = 9799,
    SPELL_PALADIN_EYE_FOR_AN_EYE_DAMAGE          = 25997,
    SPELL_PALADIN_HAND_OF_SACRIFICE              = 6940,
    SPELL_PALADIN_HOLY_LIGHT                     = 635,
    SPELL_PALADIN_HOLY_SHOCK_R1                  = 20473,
    SPELL_PALADIN_HOLY_SHOCK_R1_DAMAGE           = 25912,
    SPELL_PALADIN_HOLY_SHOCK_R1_HEALING          = 25914,
    SPELL_PALADIN_ITEM_HEALING_TRANCE            = 37706,
    SPELL_PALADIN_RIGHTEOUS_DEFENSE_TAUNT        = 31790,
    SPELL_PALADIN_SEAL_OF_RIGHTEOUSNESS          = 101423, // MoP SoR melee AoE (was 25742 in Cata)

    SPELL_PALADIN_ANCIENT_FURY                   = 86704,
    SPELL_PALADIN_ANCIENT_POWER                  = 86700,
    SPELL_PALADIN_ARCING_LIGHT_DAMAGE            = 114919,
    SPELL_PALADIN_BASTION_OF_GLORY               = 114637,
    SPELL_PALADIN_BASTION_OF_POWER               = 144569,
    SPELL_PALADIN_BLESSING_OF_KINGS              = 20217,
    SPELL_PALADIN_BLESSING_OF_THE_GUARDIANS      = 144581,
    SPELL_PALADIN_BLINDING_LIGHT_CONFUSE         = 105421,
    SPELL_PALADIN_BLINDING_LIGHT_STUN            = 115752,
    SPELL_PALADIN_CENSURE                        = 31803,
    SPELL_PALADIN_CONSECRATION_DAMAGE            = 81297,
    SPELL_PALADIN_CONTEMPLATION                  = 121183,
    SPELL_PALADIN_DAYBREAK_HEAL                  = 121129,
    SPELL_PALADIN_DIVINE_CRUSADER                = 144595,
    SPELL_PALADIN_DIVINE_PURPOSE_TALENT          = 86172,
    SPELL_PALADIN_DIVINE_STORM                   = 53385,
    SPELL_PALADIN_ETERNAL_FLAME                  = 114163,
    SPELL_PALADIN_EXECUTION_SENTENCE             = 114916,
    SPELL_PALADIN_EXECUTION_SENTENCE_DISPEL      = 146585,
    SPELL_PALADIN_EXORCISM                       = 879,
    SPELL_PALADIN_EXORCISM_ENERGIZE              = 147715,
    SPELL_PALADIN_EXORCISM_GLYPHED               = 122032,
    SPELL_PALADIN_FLASH_OF_LIGHT                 = 19750,
    SPELL_PALADIN_FORBEARANCE                    = 25771,
    SPELL_PALADIN_GLYPH_OF_BLINDING_LIGHT        = 54934,
    SPELL_PALADIN_GLYPH_OF_BURDEN_OF_GUILT       = 54931,
    SPELL_PALADIN_GLYPH_OF_BURDEN_OF_GUILT_SLOW  = 110300,
    SPELL_PALADIN_GLYPH_OF_DEVOTION_AURA         = 146955,
    SPELL_PALADIN_GLYPH_OF_DIVINE_PLEA           = 63223,
    SPELL_PALADIN_GLYPH_OF_DIVINE_SHIELD         = 146956,
    SPELL_PALADIN_GLYPH_OF_DIVINE_STORM          = 63220,
    SPELL_PALADIN_GLYPH_OF_DIVINE_STORM_HEAL     = 115515,
    SPELL_PALADIN_GLYPH_OF_DIVINITY              = 54939,
    SPELL_PALADIN_GLYPH_OF_DIVINITY_ENERGIZE     = 54986,
    SPELL_PALADIN_GLYPH_OF_DOUBLE_JEOPARDY       = 54922,
    SPELL_PALADIN_GLYPH_OF_DOUBLE_JEOPARDY_BUFF  = 121027,
    SPELL_PALADIN_GLYPH_OF_FINAL_WRATH           = 54935,
    SPELL_PALADIN_GLYPH_OF_FOCUSED_WRATH         = 115738,
    SPELL_PALADIN_GLYPH_OF_HAMMER_OF_THE_RIGHTEOUS = 63219,
    SPELL_PALADIN_GLYPH_OF_HAND_OF_SACRIFICE     = 146957,
    SPELL_PALADIN_GLYPH_OF_HOLY_WRATH            = 54923,
    SPELL_PALADIN_GLYPH_OF_LIGHT_OF_DAWN         = 54940,
    SPELL_PALADIN_GLYPH_OF_PROTECTOR_OF_THE_INNOCENT = 93466,
    SPELL_PALADIN_GLYPH_OF_THE_BATTLE_HEALER     = 119477,
    SPELL_PALADIN_GLYPH_OF_THE_FALLING_AVENGER   = 115931,
    SPELL_PALADIN_GLYPH_OF_THE_MOUNTED_KING      = 57958,
    SPELL_PALADIN_GLYPH_OF_WORD_OF_GLORY         = 54936,
    SPELL_PALADIN_GLYPH_OF_WORD_OF_GLORY_EFFECT  = 115522,
    SPELL_PALADIN_HAMMER_OF_THE_RIGHTEOUS_AOE    = 88263,
    SPELL_PALADIN_HAMMER_OF_WRATH                = 24275,
    SPELL_PALADIN_HOLY_AVENGER                   = 105809,
    SPELL_PALADIN_HOLY_POWER_ENERGIZE            = 138248,
    SPELL_PALADIN_HOLY_PRISM_ALLY                = 114871,
    SPELL_PALADIN_HOLY_PRISM_ENEMY               = 114852,
    SPELL_PALADIN_HOLY_PRISM_DAMAGE_VISUAL       = 114862,
    SPELL_PALADIN_HOLY_PRISM_DAMAGE_VISUAL_AOE   = 121551,
    SPELL_PALADIN_HOLY_PRISM_HEAL_VISUAL         = 114870,
    SPELL_PALADIN_HOLY_PRISM_HEAL_VISUAL_AOE     = 121552,
    SPELL_PALADIN_HOLY_RADIANCE                  = 82327,
    SPELL_PALADIN_HOLY_SHOCK_ENERGIZE            = 148976,
    SPELL_PALADIN_ILLUMINATED_HEALING            = 86273,
    SPELL_PALADIN_INQUISITION                    = 84963,
    SPELL_PALADIN_JUDGMENT                       = 20271,
    SPELL_PALADIN_JUDGMENTS_OF_THE_BOLD          = 111529,
    SPELL_PALADIN_JUDGMENTS_OF_THE_WISE          = 105424,
    SPELL_PALADIN_LICHBORNE                      = 50397,
    SPELL_PALADIN_LIGHT_OF_DAWN                  = 85222,
    SPELL_PALADIN_LIGHT_OF_THE_ANCIENT_KINGS     = 86678,
    SPELL_PALADIN_LIGHTS_HAMMER_HEAL             = 119952,
    SPELL_PALADIN_PHYSICAL_VULNERABILITY         = 81326,
    SPELL_PALADIN_PROTECTOR_OF_THE_INNOCENT_HEAL = 115536,
    SPELL_PALADIN_PVP_HOLY_4P_BONUS              = 131665,
    SPELL_PALADIN_PVP_HOLY_4P_ENERGIZE           = 141462,
    SPELL_PALADIN_SACRED_SHIELD_ABSORB           = 65148,
    SPELL_PALADIN_SANCTIFIED_WRATH               = 114232,
    SPELL_PALADIN_SANCTIFIED_WRATH_TALENT        = 53376,
    SPELL_PALADIN_SHIELD_OF_GLORY                = 138242,
    SPELL_PALADIN_SHIELD_OF_THE_RIGHTEOUS_PROC   = 132403,
    SPELL_PALADIN_STAY_OF_EXECUTION              = 114917,
    SPELL_PALADIN_STAY_OF_EXECUTION_DISPEL       = 146586,
    SPELL_PALADIN_SWORD_OF_LIGHT_DAMAGE_BONUS    = 20113,
    SPELL_PALADIN_T15_HOLY_2P_BONUS              = 138291,
    SPELL_PALADIN_T15_HOLY_4P_BONUS              = 138292,
    SPELL_PALADIN_T15_PROTECTION_2P_BONUS        = 138238,
    SPELL_PALADIN_T15_PROTECTION_4P_BONUS        = 138244,
    SPELL_PALADIN_T15_PROTECTION_4P_DIVINE_PROTECTION = 138250,
    SPELL_PALADIN_T15_RETRIBUTION_2P_BONUS       = 138159,
    SPELL_PALADIN_T15_RETRIBUTION_2P_EXORCISM    = 138162,
    SPELL_PALADIN_T15_RETRIBUTION_4P_BONUS       = 138164,
    SPELL_PALADIN_T16_PROTECTION_2P_BONUS        = 144580,
    SPELL_PALADIN_T16_PROTECTION_2P_DIVINE_PROTECTION = 149150,
    SPELL_PALADIN_T16_PROTECTION_4P_BONUS        = 144566,
    SPELL_PALADIN_T16_RETRIBUTION_2P_BONUS       = 144586,
    SPELL_PALADIN_T16_RETRIBUTION_4P_BONUS       = 144593,
    SPELL_PALADIN_TEMPLARS_VERDICT_OVERRIDE      = 138169,
    SPELL_PALADIN_TOWER_OF_RADIANCE              = 85512,
    SPELL_PALADIN_TOWER_OF_RADIANCE_ENERGIZE     = 88852,
    SPELL_PALADIN_WEAKENED_BLOWS                 = 115798,
    SPELL_PALADIN_WORD_OF_GLORY                  = 85673,
    SPELL_PALADIN_WORD_OF_GLORY_GLYPHED          = 136494,
    SPELL_PALADIN_WORD_OF_GLORY_DAMAGE           = 130552,
    SPELL_PALADIN_WORD_OF_GLORY_HEAL             = 130551,
};

enum PaladinCreatures
{
    NPC_PALADIN_GUARDIAN_OF_ANCIENT_KINGS_HOLY   = 46499,
};

enum PaladinDisplayIds
{
    DISPLAY_PALADIN_LIGHTS_HAMMER                = 11686,
};

static bool HasActivePaladinSpec(Unit* unit, uint32 spec)
{
    Player* player = unit->ToPlayer();
    return player && player->GetTalentSpecialization(player->GetActiveSpec()) == spec;
}

static bool IsFreeHolyPowerCast(Unit* caster, bool bastionOfPowerApplies)
{
    if (caster->HasAura(SPELL_PALADIN_DIVINE_PURPOSE_PROC))
        return true;

    return bastionOfPowerApplies && caster->HasAura(SPELL_PALADIN_BASTION_OF_POWER);
}

static bool IsPositiveSpellProc(ProcEventInfo& eventInfo)
{
    if (eventInfo.GetTypeMask() & (PROC_FLAG_DONE_SPELL_NONE_DMG_CLASS_POS | PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_POS))
        return true;

    return (eventInfo.GetHitMask() & PROC_EX_INTERNAL_HOT) != 0;
}

static void ApplyShieldOfGlory(Unit* paladin, int32 holyPower)
{
    int32 previousDuration = 0;
    if (Aura* shield = paladin->GetAura(SPELL_PALADIN_SHIELD_OF_GLORY))
        previousDuration = shield->GetDuration();

    paladin->CastSpell(paladin, SPELL_PALADIN_SHIELD_OF_GLORY, true);

    if (Aura* shield = paladin->GetAura(SPELL_PALADIN_SHIELD_OF_GLORY))
    {
        int32 duration = std::max(previousDuration, 5 * IN_MILLISECONDS * holyPower);
        shield->SetMaxDuration(duration);
        shield->SetDuration(duration);
    }
}

static void SelectMostInjuredTargets(std::list<WorldObject*>& targets, uint32 maxTargets)
{
    targets.remove_if([](WorldObject* obj)
    {
        Unit* unit = obj->ToUnit();
        return !unit || !unit->IsAlive();
    });

    targets.sort([](WorldObject* a, WorldObject* b)
    {
        return Skyfire::HealthPctOrderPred()(a->ToUnit(), b->ToUnit());
    });

    if (targets.size() > maxTargets)
        targets.resize(maxTargets);
}

// 19750 - Flash of Light, 82326 - Divine Light, 82327 - Holy Radiance
class spell_pal_selfless_healer_heal_SpellScript : public SpellScript
{
    PrepareSpellScript(spell_pal_selfless_healer_heal_SpellScript);

    bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
    {
        if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_SELFLESS_HEALER_BUFF) ||
            !sSpellMgr->GetSpellInfo(SPELL_PALADIN_SELFLESS_HEALER_CHECKER))
            return false;
        return true;
    }

    void HandleHit()
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        int32 heal = GetHitHeal();
        if (!target || !heal)
            return;

        Aura* selflessHealer = caster->GetAura(SPELL_PALADIN_SELFLESS_HEALER_BUFF);
        if (!selflessHealer)
            return;

        if (target != caster)
        {
            if (AuraEffect const* bonus = selflessHealer->GetEffect(EFFECT_1))
                SetHitHeal(AddPct(heal, bonus->GetAmount()));
        }
        else if (GetSpellInfo()->Id == SPELL_PALADIN_FLASH_OF_LIGHT)
        {
            if (AuraEffect const* bastion = caster->GetAuraEffect(SPELL_PALADIN_BASTION_OF_GLORY, EFFECT_2))
            {
                SetHitHeal(AddPct(heal, bastion->GetAmount() * selflessHealer->GetStackAmount()));
                caster->RemoveAurasDueToSpell(SPELL_PALADIN_BASTION_OF_GLORY);
            }
        }
    }

    void HandleAfterCast()
    {
        Unit* caster = GetCaster();
        if (caster->HasAura(SPELL_PALADIN_SELFLESS_HEALER_CHECKER))
        {
            caster->RemoveAurasDueToSpell(SPELL_PALADIN_SELFLESS_HEALER_BUFF);
            caster->RemoveAurasDueToSpell(SPELL_PALADIN_SELFLESS_HEALER_CHECKER);
        }
    }

    void Register() OVERRIDE
    {
        OnHit += SpellHitFn(spell_pal_selfless_healer_heal_SpellScript::HandleHit);
        AfterCast += SpellCastFn(spell_pal_selfless_healer_heal_SpellScript::HandleAfterCast);
    }
};

// 82327 - Holy Radiance
class spell_pal_holy_radiance : public SpellScriptLoader
{
public:
    spell_pal_holy_radiance() : SpellScriptLoader("spell_pal_holy_radiance") { }

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_selfless_healer_heal_SpellScript();
    }
};

// 82326 - Divine Light
class spell_pal_divine_light : public SpellScriptLoader
{
public:
    spell_pal_divine_light() : SpellScriptLoader("spell_pal_divine_light") { }

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_selfless_healer_heal_SpellScript();
    }
};

// 19750 - Flash of Light
class spell_pal_flash_of_light : public SpellScriptLoader
{
public:
    spell_pal_flash_of_light() : SpellScriptLoader("spell_pal_flash_of_light") { }

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_selfless_healer_heal_SpellScript();
    }
};

// 54922 - Glyph of Double Jeopardy
class spell_pal_glyph_of_double_jeopardy : public SpellScriptLoader
{
public:
    spell_pal_glyph_of_double_jeopardy() : SpellScriptLoader("spell_pal_glyph_of_double_jeopardy") { }

    class spell_pal_glyph_of_double_jeopardy_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_glyph_of_double_jeopardy_AuraScript);

        uint64 _lastTargetGuid = 0;

        void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
        {
            if (Unit* target = eventInfo.GetActionTarget())
                _lastTargetGuid = target->GetGUID();
        }

        void Register() OVERRIDE
        {
            OnEffectProc += AuraEffectProcFn(spell_pal_glyph_of_double_jeopardy_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }

    public:
        uint64 GetLastTarget() const { return _lastTargetGuid; }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pal_glyph_of_double_jeopardy_AuraScript();
    }
};

// 20271 - Judgment
class spell_pal_judgment : public SpellScriptLoader
{
public:
    spell_pal_judgment() : SpellScriptLoader("spell_pal_judgment") { }

    class spell_pal_judgment_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_judgment_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_LONG_ARM_OF_THE_LAW_TALENT) ||
                !sSpellMgr->GetSpellInfo(SPELL_PALADIN_LONG_ARM_OF_THE_LAW_SPEEDBUFF) ||
                !sSpellMgr->GetSpellInfo(SPELL_PALADIN_SELFLESS_HEALER_TALENT) ||
                !sSpellMgr->GetSpellInfo(SPELL_PALADIN_SELFLESS_HEALER_BUFF) ||
                !sSpellMgr->GetSpellInfo(SPELL_PALADIN_SELFLESS_HEALER_ENERGIZE) ||
                !sSpellMgr->GetSpellInfo(SPELL_PALADIN_PHYSICAL_VULNERABILITY) ||
                !sSpellMgr->GetSpellInfo(SPELL_PALADIN_GLYPH_OF_BURDEN_OF_GUILT_SLOW))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();

            if (caster->HasAura(SPELL_PALADIN_LONG_ARM_OF_THE_LAW_TALENT))
                caster->CastSpell(caster, SPELL_PALADIN_LONG_ARM_OF_THE_LAW_SPEEDBUFF);

            if (caster->HasAura(SPELL_PALADIN_SELFLESS_HEALER_TALENT))
            {
                caster->CastSpell(caster, SPELL_PALADIN_SELFLESS_HEALER_CHECKER);
            }

            bool bold = caster->HasAura(SPELL_PALADIN_JUDGMENTS_OF_THE_BOLD);
            if (bold || caster->HasAura(SPELL_PALADIN_JUDGMENTS_OF_THE_WISE))
            {
                int32 holyPower = 1;
                if (caster->HasAura(SPELL_PALADIN_SANCTIFIED_WRATH) && HasActivePaladinSpec(caster, SPEC_PALADIN_PROTECTION))
                    holyPower = 2;
                if (caster->HasAura(SPELL_PALADIN_HOLY_AVENGER))
                    holyPower = 3;

                caster->ModifyPower(POWER_HOLY_POWER, holyPower);
            }

            if (!target)
                return;

            if (bold)
                caster->CastSpell(target, SPELL_PALADIN_PHYSICAL_VULNERABILITY, true);

            if (caster->HasAura(SPELL_PALADIN_GLYPH_OF_BURDEN_OF_GUILT))
                caster->CastSpell(target, SPELL_PALADIN_GLYPH_OF_BURDEN_OF_GUILT_SLOW, true);
        }

        // Runs before the hit procs the glyph, so the stored target is still the previous one.
        void HandleDoubleJeopardy()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!target)
                return;

            AuraEffect const* bonus = caster->GetAuraEffect(SPELL_PALADIN_GLYPH_OF_DOUBLE_JEOPARDY_BUFF, EFFECT_0);
            Aura* glyph = caster->GetAura(SPELL_PALADIN_GLYPH_OF_DOUBLE_JEOPARDY);
            if (!bonus || !glyph)
                return;

            typedef spell_pal_glyph_of_double_jeopardy::spell_pal_glyph_of_double_jeopardy_AuraScript GlyphScript;
            if (GlyphScript* script = dynamic_cast<GlyphScript*>(glyph->GetScriptByName("spell_pal_glyph_of_double_jeopardy")))
                if (script->GetLastTarget() && script->GetLastTarget() != target->GetGUID())
                    SetHitDamage(CalculatePct(GetHitDamage(), 100 + bonus->GetAmount()));
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_pal_judgment_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);            
            OnHit += SpellHitFn(spell_pal_judgment_SpellScript::HandleDoubleJeopardy);
        }
    }; 

    SpellScript* GetSpellScript() const override
    {
        return new spell_pal_judgment_SpellScript();
    }
};

class spell_pal_selfless_healer : public SpellScriptLoader
{
public:
    spell_pal_selfless_healer() : SpellScriptLoader("spell_pal_selfless_healer") { }
    class spell_pal_selfless_healer_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_selfless_healer_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_SELFLESS_HEALER_BUFF) || 
                !sSpellMgr->GetSpellInfo(SPELL_PALADIN_SELFLESS_HEALER_CHECKER))
                return false;
            return true;
        }

        void HandleEffectStackProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            
            if (!GetTarget()->HasAura(SPELL_PALADIN_SELFLESS_HEALER_CHECKER))
                return;

            GetTarget()->CastSpell(GetTarget(), SPELL_PALADIN_SELFLESS_HEALER_BUFF);
            
            if (HasActivePaladinSpec(GetTarget(), SPEC_PALADIN_HOLY))
                GetTarget()->CastSpell(GetTarget(), SPELL_PALADIN_SELFLESS_HEALER_ENERGIZE);                
        }
        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_pal_selfless_healer_AuraScript::HandleEffectStackProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_pal_selfless_healer_AuraScript();
    }
};


// 31850 - Ardent Defender
class spell_pal_ardent_defender : public SpellScriptLoader
{
public:
    spell_pal_ardent_defender() : SpellScriptLoader("spell_pal_ardent_defender") { }

    class spell_pal_ardent_defender_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_ardent_defender_AuraScript);

        uint32 _absorbPct;
        uint32 _healPct;
        bool _deathPreventionUsed;

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_ARDENT_DEFENDER_HEAL))
                return false;

            return true;
        }

        bool Load() override
        {
            _absorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue();
            _healPct = GetSpellInfo()->Effects[EFFECT_1].CalcValue();
            _deathPreventionUsed = false;
            return GetUnitOwner()->ToPlayer();
        }

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
        {
            // Keep the school absorb aura from being consumed as a finite shield.
            amount = -1;
            canBeRecalculated = false;
        }

        void Absorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
        {
            Player* target = GetTarget()->ToPlayer();
            if (!target)
                return;

            Skyfire::Spells::Paladin::ArdentDefenderAbsorbResult const result =
                Skyfire::Spells::Paladin::CalculateArdentDefenderAbsorb(target->GetHealth(), target->GetMaxHealth(),
                    dmgInfo.GetDamage(), _absorbPct, _healPct, _deathPreventionUsed);

            absorbAmount = result.AbsorbAmount;
            if (!result.PreventedDeath)
                return;

            _deathPreventionUsed = true;
            if (result.HealAmount)
            {
                int32 healAmount = int32(result.HealAmount);
                target->CastCustomSpell(target, SPELL_PALADIN_ARDENT_DEFENDER_HEAL, &healAmount, NULL, NULL, true, NULL, aurEff);
            }
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pal_ardent_defender_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            OnEffectAbsorb += AuraEffectAbsorbFn(spell_pal_ardent_defender_AuraScript::Absorb, EFFECT_0);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_pal_ardent_defender_AuraScript();
    }
};

// 53651 - Beacon of Light
class spell_pal_beacon_of_light : public SpellScriptLoader
{
public:
    spell_pal_beacon_of_light() : SpellScriptLoader("spell_pal_beacon_of_light") { }

    class spell_pal_beacon_of_light_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_beacon_of_light_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_BEACON_OF_LIGHT_HEAL))
                return false;
            return true;
        }

        Aura* GetBeacon()
        {
            Unit::AuraList& auras = GetUnitOwner()->GetSingleCastAuras();
            for (Unit::AuraList::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
                if ((*itr)->GetId() == SPELL_PALADIN_BEACON_OF_LIGHT_MARKER && !(*itr)->IsRemoved())
                    return *itr;
            return NULL;
        }

        Unit* GetBeaconTarget()
        {
            if (Aura* beacon = GetBeacon())
                return beacon->GetOwner()->ToUnit();
            return NULL;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (!IsPositiveSpellProc(eventInfo))
                return false;

            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            if (!spellInfo || spellInfo->Id == SPELL_PALADIN_BEACON_OF_LIGHT_HEAL)
                return false;

            HealInfo* healInfo = eventInfo.GetHealInfo();
            if (!healInfo || !healInfo->GetHeal())
                return false;

            Unit* beaconTarget = GetBeaconTarget();
            return beaconTarget && beaconTarget != eventInfo.GetActionTarget();
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            Unit* beaconTarget = GetBeaconTarget();
            if (!beaconTarget)
                return;

            static std::map<uint32, int32> const transferPct =
            {
                { SPELL_PALADIN_HOLY_LIGHT,         100 },
                { SPELL_PALADIN_HOLY_RADIANCE,       15 },
                { SPELL_PALADIN_LIGHT_OF_DAWN,       15 },
                { SPELL_PALADIN_LIGHTS_HAMMER_HEAL,  15 },
                { SPELL_PALADIN_HOLY_PRISM_ALLY,     15 },
            };

            std::map<uint32, int32>::const_iterator itr = transferPct.find(eventInfo.GetSpellInfo()->Id);
            int32 pct = itr != transferPct.end() ? itr->second : aurEff->GetAmount();

            int32 heal = CalculatePct(int32(eventInfo.GetHealInfo()->GetHeal()), pct);
            if (AuraEffect const* setBonus = GetUnitOwner()->GetAuraEffect(SPELL_PALADIN_T15_HOLY_4P_BONUS, EFFECT_0))
                AddPct(heal, setBonus->GetAmount());

            if (heal > 0)
                GetUnitOwner()->CastCustomSpell(SPELL_PALADIN_BEACON_OF_LIGHT_HEAL, SPELLVALUE_BASE_POINT0, heal, beaconTarget, true, NULL, aurEff);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Aura* beacon = GetBeacon())
                beacon->Remove();
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_pal_beacon_of_light_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_pal_beacon_of_light_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
            AfterEffectRemove += AuraEffectRemoveFn(spell_pal_beacon_of_light_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pal_beacon_of_light_AuraScript();
    }
};

// 37877 - Blessing of Faith
class spell_pal_blessing_of_faith : public SpellScriptLoader
{
public:
    spell_pal_blessing_of_faith() : SpellScriptLoader("spell_pal_blessing_of_faith") { }

    class spell_pal_blessing_of_faith_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_blessing_of_faith_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_BLESSING_OF_LOWER_CITY_DRUID) || !sSpellMgr->GetSpellInfo(SPELL_PALADIN_BLESSING_OF_LOWER_CITY_PALADIN) || !sSpellMgr->GetSpellInfo(SPELL_PALADIN_BLESSING_OF_LOWER_CITY_PRIEST) || !sSpellMgr->GetSpellInfo(SPELL_PALADIN_BLESSING_OF_LOWER_CITY_SHAMAN))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Unit* unitTarget = GetHitUnit())
            {
                uint32 spell_id = 0;
                switch (unitTarget->getClass())
                {
                case CLASS_DRUID:
                    spell_id = SPELL_PALADIN_BLESSING_OF_LOWER_CITY_DRUID;
                    break;
                case CLASS_PALADIN:
                    spell_id = SPELL_PALADIN_BLESSING_OF_LOWER_CITY_PALADIN;
                    break;
                case CLASS_PRIEST:
                    spell_id = SPELL_PALADIN_BLESSING_OF_LOWER_CITY_PRIEST;
                    break;
                case CLASS_SHAMAN:
                    spell_id = SPELL_PALADIN_BLESSING_OF_LOWER_CITY_SHAMAN;
                    break;
                default:
                    return; // ignore for non-healing classes
                }
                Unit* caster = GetCaster();
                caster->CastSpell(caster, spell_id, true);
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_pal_blessing_of_faith_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_pal_blessing_of_faith_SpellScript();
    }
};

// 85416 - Grand Crusader
class spell_pal_grand_crusader : public SpellScriptLoader
{
public:
    spell_pal_grand_crusader() : SpellScriptLoader("spell_pal_grand_crusader") { }

    class spell_pal_grand_crusader_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_grand_crusader_SpellScript);

        void HandleCast()
        {
            if (Player* player = GetCaster()->ToPlayer())
                player->RemoveSpellCooldown(SPELL_PALADIN_AVENGERS_SHIELD, true);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_pal_grand_crusader_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_grand_crusader_SpellScript();
    }
};

// -9799 - Eye for an Eye
class spell_pal_eye_for_an_eye : public SpellScriptLoader
{
public:
    spell_pal_eye_for_an_eye() : SpellScriptLoader("spell_pal_eye_for_an_eye") { }

    class spell_pal_eye_for_an_eye_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_eye_for_an_eye_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_EYE_FOR_AN_EYE_DAMAGE))
                return false;
            return true;
        }

        void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            int32 damage = CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount());
            GetTarget()->CastCustomSpell(SPELL_PALADIN_EYE_FOR_AN_EYE_DAMAGE, SPELLVALUE_BASE_POINT0, damage, eventInfo.GetProcTarget(), true, NULL, aurEff);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_pal_eye_for_an_eye_AuraScript::HandleEffectProc, EFFECT_0, m_scriptSpellId == SPELL_PALADIN_EYE_FOR_AN_EYE_RANK_1 ? SPELL_AURA_DUMMY : SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_pal_eye_for_an_eye_AuraScript();
    }
};

// 6940 - Hand of Sacrifice
class spell_pal_hand_of_sacrifice : public SpellScriptLoader
{
public:
    spell_pal_hand_of_sacrifice() : SpellScriptLoader("spell_pal_hand_of_sacrifice") { }

    class spell_pal_hand_of_sacrifice_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_hand_of_sacrifice_AuraScript);

        int32 remainingAmount;

        bool Load() override
        {
            if (Unit* caster = GetCaster())
            {
                remainingAmount = caster->GetMaxHealth();
                return true;
            }
            return false;
        }

        void CalculateSplitAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* caster = GetCaster())
                if (caster->HasAura(SPELL_PALADIN_GLYPH_OF_HAND_OF_SACRIFICE))
                    amount = 0;
        }

        void CalculateDamageReduction(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            Unit* caster = GetCaster();
            if (!caster || !caster->HasAura(SPELL_PALADIN_GLYPH_OF_HAND_OF_SACRIFICE))
                amount = 0;
        }

        void Split(AuraEffect* /*aurEff*/, DamageInfo& /*dmgInfo*/, uint32& splitAmount)
        {
            remainingAmount -= splitAmount;

            if (remainingAmount <= 0)
            {
                GetTarget()->RemoveAura(SPELL_PALADIN_HAND_OF_SACRIFICE);
            }
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pal_hand_of_sacrifice_AuraScript::CalculateSplitAmount, EFFECT_0, SPELL_AURA_SPLIT_DAMAGE_PCT);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pal_hand_of_sacrifice_AuraScript::CalculateDamageReduction, EFFECT_2, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN);
            OnEffectSplit += AuraEffectSplitFn(spell_pal_hand_of_sacrifice_AuraScript::Split, EFFECT_0);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_pal_hand_of_sacrifice_AuraScript();
    }
};

// 20473 - Holy Shock
class spell_pal_holy_shock : public SpellScriptLoader
{
public:
    spell_pal_holy_shock() : SpellScriptLoader("spell_pal_holy_shock") { }

    class spell_pal_holy_shock_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_holy_shock_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_PALADIN_HOLY_SHOCK_R1_DAMAGE)
                && sSpellMgr->GetSpellInfo(SPELL_PALADIN_HOLY_SHOCK_R1_HEALING)
                && sSpellMgr->GetSpellInfo(SPELL_PALADIN_HOLY_SHOCK_ENERGIZE);
        }

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            Unit* target = GetExplTargetUnit();
            if (!target)
                return SpellCastResult::SPELL_FAILED_BAD_TARGETS;

            if (!caster->IsFriendlyTo(target))
            {
                if (!caster->IsValidAttackTarget(target))
                    return SpellCastResult::SPELL_FAILED_BAD_TARGETS;

                if (!caster->isInFront(target))
                    return SpellCastResult::SPELL_FAILED_UNIT_NOT_INFRONT;
            }

            return SpellCastResult::SPELL_CAST_OK;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!target)
                return;

            caster->CastSpell(target, caster->IsFriendlyTo(target) ? SPELL_PALADIN_HOLY_SHOCK_R1_HEALING : SPELL_PALADIN_HOLY_SHOCK_R1_DAMAGE, true);
            caster->CastSpell(caster, SPELL_PALADIN_HOLY_SHOCK_ENERGIZE, true);
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_pal_holy_shock_SpellScript::CheckCast);
            OnEffectHitTarget += SpellEffectFn(spell_pal_holy_shock_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_holy_shock_SpellScript();
    }
};

// 37705 - Healing Discount
class spell_pal_item_healing_discount : public SpellScriptLoader
{
public:
    spell_pal_item_healing_discount() : SpellScriptLoader("spell_pal_item_healing_discount") { }

    class spell_pal_item_healing_discount_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_item_healing_discount_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_ITEM_HEALING_TRANCE))
                return false;
            return true;
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            GetTarget()->CastSpell(GetTarget(), SPELL_PALADIN_ITEM_HEALING_TRANCE, true, NULL, aurEff);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_pal_item_healing_discount_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_pal_item_healing_discount_AuraScript();
    }
};

// 31789 - Righteous Defense
class spell_pal_righteous_defense : public SpellScriptLoader
{
public:
    spell_pal_righteous_defense() : SpellScriptLoader("spell_pal_righteous_defense") { }

    class spell_pal_righteous_defense_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_righteous_defense_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_RIGHTEOUS_DEFENSE_TAUNT))
                return false;
            return true;
        }

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            if (caster->GetTypeId() != TypeID::TYPEID_PLAYER)
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;

            if (Unit* target = GetExplTargetUnit())
            {
                if (!target->IsFriendlyTo(caster) || target->getAttackers().empty())
                    return SpellCastResult::SPELL_FAILED_BAD_TARGETS;
            }
            else
                return SpellCastResult::SPELL_FAILED_BAD_TARGETS;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void HandleTriggerSpellLaunch(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
        }

        void HandleTriggerSpellHit(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            if (Unit* target = GetHitUnit())
                GetCaster()->CastSpell(target, SPELL_PALADIN_RIGHTEOUS_DEFENSE_TAUNT, true);
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_pal_righteous_defense_SpellScript::CheckCast);
            //! WORKAROUND
            //! target select will be executed in hitphase of effect 0
            //! so we must handle trigger spell also in hit phase (default execution in launch phase)
            //! see issue #3718
            OnEffectLaunchTarget += SpellEffectFn(spell_pal_righteous_defense_SpellScript::HandleTriggerSpellLaunch, EFFECT_1, SPELL_EFFECT_TRIGGER_SPELL);
            OnEffectHitTarget += SpellEffectFn(spell_pal_righteous_defense_SpellScript::HandleTriggerSpellHit, EFFECT_1, SPELL_EFFECT_TRIGGER_SPELL);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_pal_righteous_defense_SpellScript();
    }
};

// 20925, 148039 - Sacred Shield
class spell_pal_sacred_shield : public SpellScriptLoader
{
public:
    spell_pal_sacred_shield() : SpellScriptLoader("spell_pal_sacred_shield") { }

    class spell_pal_sacred_shield_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_sacred_shield_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_PALADIN_SACRED_SHIELD_ABSORB) != NULL;
        }

        void HandleTick(AuraEffect const* /*aurEff*/)
        {
            if (Unit* caster = GetCaster())
                caster->CastSpell(GetTarget(), SPELL_PALADIN_SACRED_SHIELD_ABSORB, true);
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_pal_sacred_shield_AuraScript::HandleTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pal_sacred_shield_AuraScript();
    }
};

// 65148 - Sacred Shield
class spell_pal_sacred_shield_absorb : public SpellScriptLoader
{
public:
    spell_pal_sacred_shield_absorb() : SpellScriptLoader("spell_pal_sacred_shield_absorb") { }

    class spell_pal_sacred_shield_absorb_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_sacred_shield_absorb_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* caster = GetCaster())
                amount = int32(30 + caster->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_HOLY) * 1.17f);
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pal_sacred_shield_absorb_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pal_sacred_shield_absorb_AuraScript();
    }
};

// 20154 - Seal of Righteousness (MoP: melee attacks deal weapon % Holy to nearby enemies via 101423)
class spell_pal_seal_of_righteousness : public SpellScriptLoader
{
public:
    spell_pal_seal_of_righteousness() : SpellScriptLoader("spell_pal_seal_of_righteousness") { }

    class spell_pal_seal_of_righteousness_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_seal_of_righteousness_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_SEAL_OF_RIGHTEOUSNESS))
                return false;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            return eventInfo.GetProcTarget();
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            // MoP: DBC spell 101423 is 9% weapon damage AoE; no AP/SP custom BP.
            GetTarget()->CastSpell(eventInfo.GetProcTarget(), SPELL_PALADIN_SEAL_OF_RIGHTEOUSNESS, true, NULL, aurEff);
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_pal_seal_of_righteousness_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_pal_seal_of_righteousness_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_pal_seal_of_righteousness_AuraScript();
    }
};

// 85673 - Word of Glory, 136494 - Word of Glory (glyphed)
class spell_pal_word_of_glory : public SpellScriptLoader
{
public:
    spell_pal_word_of_glory() : SpellScriptLoader("spell_pal_word_of_glory") { }

    class spell_pal_word_of_glory_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_word_of_glory_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_PALADIN_WORD_OF_GLORY_HEAL)
                && sSpellMgr->GetSpellInfo(SPELL_PALADIN_WORD_OF_GLORY_DAMAGE)
                && sSpellMgr->GetSpellInfo(SPELL_PALADIN_GLYPH_OF_WORD_OF_GLORY_EFFECT);
        }

        void HandleHit()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!target)
                return;

            bool freeCast = IsFreeHolyPowerCast(caster, true);
            int32 holyPower = Skyfire::Spells::Paladin::CalculateSpentHolyPower(caster->GetPower(POWER_HOLY_POWER) + 1, freeCast);

            bool harmful = caster->IsValidAttackTarget(target);
            caster->CastSpell(target, harmful ? SPELL_PALADIN_WORD_OF_GLORY_DAMAGE : SPELL_PALADIN_WORD_OF_GLORY_HEAL, true);

            if (!freeCast && holyPower > 1)
                caster->ModifyPower(POWER_HOLY_POWER, -(holyPower - 1));

            if (!harmful)
                if (AuraEffect const* glyph = caster->GetAuraEffect(SPELL_PALADIN_GLYPH_OF_WORD_OF_GLORY, EFFECT_0))
                    caster->CastCustomSpell(SPELL_PALADIN_GLYPH_OF_WORD_OF_GLORY_EFFECT, SPELLVALUE_BASE_POINT0, glyph->GetAmount() * holyPower, caster, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pal_word_of_glory_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_word_of_glory_SpellScript();
    }
};

// 130551 - Word of Glory
class spell_pal_word_of_glory_heal : public SpellScriptLoader
{
public:
    spell_pal_word_of_glory_heal() : SpellScriptLoader("spell_pal_word_of_glory_heal") { }

    class spell_pal_word_of_glory_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_word_of_glory_heal_SpellScript);

        void HandleHit()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!target)
                return;

            int32 holyPower = Skyfire::Spells::Paladin::CalculateSpentHolyPower(caster->GetPower(POWER_HOLY_POWER) + 1, IsFreeHolyPowerCast(caster, true));

            int32 heal = GetHitHeal();
            if (target == caster)
            {
                if (AuraEffect const* bastion = caster->GetAuraEffect(SPELL_PALADIN_BASTION_OF_GLORY, EFFECT_0))
                {
                    AddPct(heal, bastion->GetAmount());
                    caster->RemoveAurasDueToSpell(SPELL_PALADIN_BASTION_OF_GLORY);
                }
            }

            heal *= holyPower;
            SetHitHeal(heal);

            // This heal is a triggered spell, so the glyph aura cannot proc from it.
            if (target != caster)
                if (AuraEffect const* glyph = caster->GetAuraEffect(SPELL_PALADIN_GLYPH_OF_PROTECTOR_OF_THE_INNOCENT, EFFECT_0))
                    if (int32 selfHeal = CalculatePct(heal, glyph->GetAmount()))
                        caster->CastCustomSpell(SPELL_PALADIN_PROTECTOR_OF_THE_INNOCENT_HEAL, SPELLVALUE_BASE_POINT0, selfHeal, caster, true);

            if (caster->HasAura(SPELL_PALADIN_T15_PROTECTION_2P_BONUS))
                ApplyShieldOfGlory(caster, holyPower);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pal_word_of_glory_heal_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_word_of_glory_heal_SpellScript();
    }
};

// 130552 - Harsh Word
class spell_pal_harsh_word : public SpellScriptLoader
{
public:
    spell_pal_harsh_word() : SpellScriptLoader("spell_pal_harsh_word") { }

    class spell_pal_harsh_word_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_harsh_word_SpellScript);

        void HandleHit()
        {
            Unit* caster = GetCaster();
            int32 holyPower = Skyfire::Spells::Paladin::CalculateSpentHolyPower(caster->GetPower(POWER_HOLY_POWER) + 1, IsFreeHolyPowerCast(caster, true));
            SetHitDamage(GetHitDamage() * holyPower);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pal_harsh_word_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_harsh_word_SpellScript();
    }
};

// 114163 - Eternal Flame
class spell_pal_eternal_flame : public SpellScriptLoader
{
public:
    spell_pal_eternal_flame() : SpellScriptLoader("spell_pal_eternal_flame") { }

    class spell_pal_eternal_flame_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_eternal_flame_SpellScript);

        void HandleHit()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!target)
                return;

            bool freeCast = IsFreeHolyPowerCast(caster, true);
            int32 holyPower = Skyfire::Spells::Paladin::CalculateSpentHolyPower(caster->GetPower(POWER_HOLY_POWER) + 1, freeCast);

            if (!freeCast && holyPower > 1)
                caster->ModifyPower(POWER_HOLY_POWER, -(holyPower - 1));

            bool selfCast = target == caster;
            int32 bastionPct = 0;
            if (selfCast)
            {
                if (AuraEffect const* bastion = caster->GetAuraEffect(SPELL_PALADIN_BASTION_OF_GLORY, EFFECT_0))
                {
                    bastionPct = bastion->GetAmount();
                    caster->RemoveAurasDueToSpell(SPELL_PALADIN_BASTION_OF_GLORY);
                }
            }

            int32 heal = GetHitHeal();
            AddPct(heal, bastionPct);
            SetHitHeal(heal * holyPower);

            if (Aura* aura = GetHitAura())
            {
                if (AuraEffect* hot = aura->GetEffect(EFFECT_1))
                {
                    int32 amount = hot->GetAmount() * holyPower;
                    AddPct(amount, bastionPct);
                    if (selfCast)
                        amount = int32(amount * 1.5f);
                    hot->SetAmount(amount);
                }
            }

            if (AuraEffect const* glyph = caster->GetAuraEffect(SPELL_PALADIN_GLYPH_OF_WORD_OF_GLORY, EFFECT_0))
                caster->CastCustomSpell(SPELL_PALADIN_GLYPH_OF_WORD_OF_GLORY_EFFECT, SPELLVALUE_BASE_POINT0, glyph->GetAmount() * holyPower, caster, true);

            if (caster->HasAura(SPELL_PALADIN_T15_PROTECTION_2P_BONUS))
                ApplyShieldOfGlory(caster, holyPower);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pal_eternal_flame_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_eternal_flame_SpellScript();
    }
};

// 85222 - Light of Dawn
class spell_pal_light_of_dawn : public SpellScriptLoader
{
public:
    spell_pal_light_of_dawn() : SpellScriptLoader("spell_pal_light_of_dawn") { }

    class spell_pal_light_of_dawn_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_light_of_dawn_SpellScript);

        int32 _holyPower = 1;

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            uint32 maxTargets = GetCaster()->HasAura(SPELL_PALADIN_GLYPH_OF_LIGHT_OF_DAWN) ? 4 : 6;

            targets.remove_if([](WorldObject* target) { return !target->ToUnit(); });
            if (targets.size() <= maxTargets)
                return;

            targets.sort([](WorldObject* a, WorldObject* b)
            {
                return a->ToUnit()->GetHealthPct() < b->ToUnit()->GetHealthPct();
            });
            targets.resize(maxTargets);
        }

        // OnCast runs before the spell's own holy power cost is taken.
        void HandleCast()
        {
            Unit* caster = GetCaster();
            bool freeCast = IsFreeHolyPowerCast(caster, false);
            _holyPower = Skyfire::Spells::Paladin::CalculateSpentHolyPower(caster->GetPower(POWER_HOLY_POWER), freeCast);

            if (!freeCast && _holyPower > 1)
                caster->ModifyPower(POWER_HOLY_POWER, -(_holyPower - 1));
        }

        void HandleHit()
        {
            SetHitHeal(GetHitHeal() * _holyPower);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pal_light_of_dawn_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
            OnCast += SpellCastFn(spell_pal_light_of_dawn_SpellScript::HandleCast);
            OnHit += SpellHitFn(spell_pal_light_of_dawn_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_light_of_dawn_SpellScript();
    }
};

// 84963 - Inquisition
class spell_pal_inquisition : public SpellScriptLoader
{
public:
    spell_pal_inquisition() : SpellScriptLoader("spell_pal_inquisition") { }

    class spell_pal_inquisition_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_inquisition_SpellScript);

        bool _freeCast = false;
        int32 _holyPower = 1;
        int32 _remainingDuration = 0;

        // Checked before the spell's own holy power cost is taken.
        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            _freeCast = IsFreeHolyPowerCast(caster, false);
            _holyPower = Skyfire::Spells::Paladin::CalculateSpentHolyPower(caster->GetPower(POWER_HOLY_POWER), _freeCast);
            _remainingDuration = 0;

            if (Aura* inquisition = caster->GetAura(SPELL_PALADIN_INQUISITION))
            {
                if (inquisition->GetDuration() <= 2 * IN_MILLISECONDS)
                    _remainingDuration = inquisition->GetDuration();

                if (inquisition->CalcMaxDuration(caster) * _holyPower < inquisition->GetDuration())
                    return SpellCastResult::SPELL_FAILED_AURA_BOUNCED;
            }

            return SpellCastResult::SPELL_CAST_OK;
        }

        void HandleHit()
        {
            Unit* caster = GetCaster();
            if (Aura* inquisition = caster->GetAura(SPELL_PALADIN_INQUISITION))
            {
                int32 duration = inquisition->CalcMaxDuration(caster) * _holyPower + _remainingDuration;
                inquisition->SetMaxDuration(duration);
                inquisition->SetDuration(duration);
            }

            if (!_freeCast && _holyPower > 1)
                caster->ModifyPower(POWER_HOLY_POWER, -(_holyPower - 1));
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_pal_inquisition_SpellScript::CheckCast);
            OnHit += SpellHitFn(spell_pal_inquisition_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_inquisition_SpellScript();
    }
};

// 53385 - Divine Storm, 53600 - Shield of the Righteous, 84963 - Inquisition, 85222 - Light of Dawn,
// 85256 - Templar's Verdict, 85673 - Word of Glory, 114163 - Eternal Flame, 136494 - Word of Glory,
// 138165 - Templar's Verdict
class spell_pal_divine_purpose : public SpellScriptLoader
{
public:
    spell_pal_divine_purpose() : SpellScriptLoader("spell_pal_divine_purpose") { }

    class spell_pal_divine_purpose_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_divine_purpose_SpellScript);

        void HandleAfterCast()
        {
            Unit* caster = GetCaster();
            uint32 spellId = GetSpellInfo()->Id;

            bool divineCrusaderConsumed = spellId == SPELL_PALADIN_DIVINE_STORM && caster->HasAura(SPELL_PALADIN_DIVINE_CRUSADER);
            bool bastionOfPowerConsumed = (spellId == SPELL_PALADIN_WORD_OF_GLORY || spellId == SPELL_PALADIN_WORD_OF_GLORY_GLYPHED ||
                spellId == SPELL_PALADIN_ETERNAL_FLAME) && caster->HasAura(SPELL_PALADIN_BASTION_OF_POWER);

            if (divineCrusaderConsumed)
                caster->RemoveAurasDueToSpell(SPELL_PALADIN_DIVINE_CRUSADER);
            if (bastionOfPowerConsumed)
                caster->RemoveAurasDueToSpell(SPELL_PALADIN_BASTION_OF_POWER);
            if (!divineCrusaderConsumed && !bastionOfPowerConsumed)
                caster->RemoveAurasDueToSpell(SPELL_PALADIN_DIVINE_PURPOSE_PROC);

            if (caster->HasAura(SPELL_PALADIN_T16_RETRIBUTION_4P_BONUS) && roll_chance_i(25))
                caster->CastSpell(caster, SPELL_PALADIN_DIVINE_CRUSADER, true);

            if (caster->HasAura(SPELL_PALADIN_DIVINE_PURPOSE_TALENT) && roll_chance_i(25))
                caster->CastSpell(caster, SPELL_PALADIN_DIVINE_PURPOSE_PROC, true);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_pal_divine_purpose_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_divine_purpose_SpellScript();
    }
};

// 53385 - Divine Storm
class spell_pal_glyph_of_divine_storm : public SpellScriptLoader
{
public:
    spell_pal_glyph_of_divine_storm() : SpellScriptLoader("spell_pal_glyph_of_divine_storm") { }

    class spell_pal_glyph_of_divine_storm_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_glyph_of_divine_storm_SpellScript);

        void HandleAfterCast()
        {
            Unit* caster = GetCaster();
            if (caster->HasAura(SPELL_PALADIN_GLYPH_OF_DIVINE_STORM))
                caster->CastSpell(caster, SPELL_PALADIN_GLYPH_OF_DIVINE_STORM_HEAL, true);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_pal_glyph_of_divine_storm_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_glyph_of_divine_storm_SpellScript();
    }
};

// 59578 - The Art of War
class spell_pal_art_of_war : public SpellScriptLoader
{
public:
    spell_pal_art_of_war() : SpellScriptLoader("spell_pal_art_of_war") { }

    class spell_pal_art_of_war_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_art_of_war_SpellScript);

        void HandleHit()
        {
            Player* paladin = GetCaster()->ToPlayer();
            if (!paladin)
                return;

            if (paladin->HasSpellCooldown(SPELL_PALADIN_EXORCISM))
                paladin->RemoveSpellCooldown(SPELL_PALADIN_EXORCISM, true);
            if (paladin->HasSpellCooldown(SPELL_PALADIN_EXORCISM_GLYPHED))
                paladin->RemoveSpellCooldown(SPELL_PALADIN_EXORCISM_GLYPHED, true);

            // The set bonus procs from this buff, which is applied by a triggered spell and cannot proc it.
            if (AuraEffect const* setBonus = paladin->GetAuraEffect(SPELL_PALADIN_T16_RETRIBUTION_2P_BONUS, EFFECT_0))
                paladin->CastSpell(paladin, setBonus->GetSpellInfo()->Effects[EFFECT_0].TriggerSpell, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pal_art_of_war_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_art_of_war_SpellScript();
    }
};

// 879 - Exorcism
class spell_pal_exorcism : public SpellScriptLoader
{
public:
    spell_pal_exorcism() : SpellScriptLoader("spell_pal_exorcism") { }

    class spell_pal_exorcism_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_exorcism_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_PALADIN_EXORCISM_ENERGIZE)
                && sSpellMgr->GetSpellInfo(SPELL_PALADIN_T15_RETRIBUTION_2P_EXORCISM);
        }

        void HandleAfterHit()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!target)
                return;

            caster->CastSpell(target, SPELL_PALADIN_EXORCISM_ENERGIZE, true);

            if (caster->HasAura(SPELL_PALADIN_T15_RETRIBUTION_2P_BONUS))
                caster->CastSpell(target, SPELL_PALADIN_T15_RETRIBUTION_2P_EXORCISM, true);
        }

        void Register() OVERRIDE
        {
            AfterHit += SpellHitFn(spell_pal_exorcism_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_exorcism_SpellScript();
    }
};

// 122032 - Exorcism (Glyph of Mass Exorcism)
class spell_pal_exorcism_glyphed : public SpellScriptLoader
{
public:
    spell_pal_exorcism_glyphed() : SpellScriptLoader("spell_pal_exorcism_glyphed") { }

    class spell_pal_exorcism_glyphed_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_exorcism_glyphed_SpellScript);

        void HandleHit()
        {
            if (GetHitUnit() != GetExplTargetUnit())
                SetHitDamage(GetHitDamage() / 4);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pal_exorcism_glyphed_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_exorcism_glyphed_SpellScript();
    }
};

// 35395 - Crusader Strike
class spell_pal_crusader_strike : public SpellScriptLoader
{
public:
    spell_pal_crusader_strike() : SpellScriptLoader("spell_pal_crusader_strike") { }

    class spell_pal_crusader_strike_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_crusader_strike_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_PALADIN_WEAKENED_BLOWS) != NULL;
        }

        void HandleHit()
        {
            Unit* target = GetHitUnit();
            if (target && target != GetCaster())
                GetCaster()->CastSpell(target, SPELL_PALADIN_WEAKENED_BLOWS, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pal_crusader_strike_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_crusader_strike_SpellScript();
    }
};

// 35395 - Crusader Strike
class spell_pal_t15_retribution_4p_bonus : public SpellScriptLoader
{
public:
    spell_pal_t15_retribution_4p_bonus() : SpellScriptLoader("spell_pal_t15_retribution_4p_bonus") { }

    class spell_pal_t15_retribution_4p_bonus_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_t15_retribution_4p_bonus_SpellScript);

        void HandleCast()
        {
            Unit* caster = GetCaster();
            if (AuraEffect const* setBonus = caster->GetAuraEffect(SPELL_PALADIN_T15_RETRIBUTION_4P_BONUS, EFFECT_0))
                if (roll_chance_i(setBonus->GetAmount()))
                    caster->CastSpell(caster, SPELL_PALADIN_TEMPLARS_VERDICT_OVERRIDE, true);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_pal_t15_retribution_4p_bonus_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_t15_retribution_4p_bonus_SpellScript();
    }
};

// 24275 - Hammer of Wrath
class spell_pal_sword_of_light_damage_bonus_holy : public SpellScriptLoader
{
public:
    spell_pal_sword_of_light_damage_bonus_holy() : SpellScriptLoader("spell_pal_sword_of_light_damage_bonus_holy") { }

    class spell_pal_sword_of_light_damage_bonus_holy_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_sword_of_light_damage_bonus_holy_SpellScript);

        void HandleHit()
        {
            if (AuraEffect const* bonus = GetCaster()->GetAuraEffect(SPELL_PALADIN_SWORD_OF_LIGHT_DAMAGE_BONUS, EFFECT_0))
            {
                int32 damage = GetHitDamage();
                SetHitDamage(AddPct(damage, bonus->GetAmount()));
            }
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pal_sword_of_light_damage_bonus_holy_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_sword_of_light_damage_bonus_holy_SpellScript();
    }
};

// 42463 - Seal of Truth
class spell_pal_seal_of_truth_hotfix : public SpellScriptLoader
{
public:
    spell_pal_seal_of_truth_hotfix() : SpellScriptLoader("spell_pal_seal_of_truth_hotfix") { }

    class spell_pal_seal_of_truth_hotfix_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_seal_of_truth_hotfix_SpellScript);

        void HandleHit()
        {
            // Seal of Truth and Censure deal 80% less damage for Protection paladins.
            if (HasActivePaladinSpec(GetCaster(), SPEC_PALADIN_PROTECTION))
                SetHitDamage(int32(GetHitDamage() * 0.2f));
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pal_seal_of_truth_hotfix_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_seal_of_truth_hotfix_SpellScript();
    }
};

// 31803 - Censure
class spell_pal_censure_hotfix : public SpellScriptLoader
{
public:
    spell_pal_censure_hotfix() : SpellScriptLoader("spell_pal_censure_hotfix") { }

    class spell_pal_censure_hotfix_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_censure_hotfix_AuraScript);

        void CalculateTickAmount(AuraEffect const* /*aurEff*/, int32& amount)
        {
            // Seal of Truth and Censure deal 80% less damage for Protection paladins.
            if (Unit* caster = GetCaster())
                if (HasActivePaladinSpec(caster, SPEC_PALADIN_PROTECTION))
                    amount = int32(amount * 0.2f);
        }

        void Register() OVERRIDE
        {
            DoEffectCalcTickAmount += AuraEffectCalcTickAmountFn(spell_pal_censure_hotfix_AuraScript::CalculateTickAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pal_censure_hotfix_AuraScript();
    }
};

// 20164 - Seal of Justice
class spell_pal_seal_of_justice : public SpellScriptLoader
{
public:
    spell_pal_seal_of_justice() : SpellScriptLoader("spell_pal_seal_of_justice") { }

    class spell_pal_seal_of_justice_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_seal_of_justice_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
                return spellInfo->Id != SPELL_PALADIN_JUDGMENT && spellInfo->Id != SPELL_PALADIN_HAMMER_OF_WRATH;
            return true;
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_pal_seal_of_justice_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pal_seal_of_justice_AuraScript();
    }
};

// 63225 - Glyph of Inquisition
class spell_pal_glyph_of_inquisition : public SpellScriptLoader
{
public:
    spell_pal_glyph_of_inquisition() : SpellScriptLoader("spell_pal_glyph_of_inquisition") { }

    class spell_pal_glyph_of_inquisition_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_glyph_of_inquisition_AuraScript);

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();

            if (Aura* inquisition = GetUnitOwner()->GetAura(SPELL_PALADIN_INQUISITION))
            {
                int32 duration = std::min(inquisition->GetDuration() + aurEff->GetAmount() * IN_MILLISECONDS, 2 * MINUTE * IN_MILLISECONDS);
                inquisition->SetMaxDuration(duration);
                inquisition->RefreshDuration();
            }
        }

        void Register() OVERRIDE
        {
            OnEffectProc += AuraEffectProcFn(spell_pal_glyph_of_inquisition_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pal_glyph_of_inquisition_AuraScript();
    }
};

// 119072 - Holy Wrath
class spell_pal_holy_wrath : public SpellScriptLoader
{
public:
    spell_pal_holy_wrath() : SpellScriptLoader("spell_pal_holy_wrath") { }

    class spell_pal_holy_wrath_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_holy_wrath_SpellScript);

        std::list<WorldObject*> _targets;

        void SelectTargets(std::list<WorldObject*>& targets)
        {
            Unit* caster = GetCaster();
            if (caster->HasAura(SPELL_PALADIN_GLYPH_OF_FOCUSED_WRATH) && !targets.empty())
            {
                WorldObject* fallback = Skyfire::Containers::SelectRandomContainerElement(targets);
                uint64 currentTarget = caster->GetTarget();
                targets.remove_if([currentTarget](WorldObject* target) { return target->GetGUID() != currentTarget; });

                if (targets.empty())
                    targets.push_back(fallback);
            }

            _targets = targets;
        }

        void CopyTargets(std::list<WorldObject*>& targets)
        {
            targets = _targets;
        }

        void HandleStun(SpellEffIndex effIndex)
        {
            Unit* target = GetHitUnit();
            if (!target)
                return;

            if (Creature* creature = target->ToCreature())
            {
                uint32 typeMask = CREATURE_TYPEMASK_DEMON_OR_UNDEAD;
                if (GetCaster()->HasAura(SPELL_PALADIN_GLYPH_OF_HOLY_WRATH))
                    typeMask |= (1 << (CREATURE_TYPE_DRAGONKIN - 1)) | (1 << (CREATURE_TYPE_ELEMENTAL - 1)) | (1 << (CREATURE_TYPE_ABERRATION - 1));

                if (creature->GetCreatureTypeMask() & typeMask)
                    return;
            }
            else if (target->HasAura(SPELL_PALADIN_LICHBORNE))
                return;

            PreventHitDefaultEffect(effIndex);
        }

        void HandleHit()
        {
            Unit* target = GetHitUnit();
            if (!target)
                return;

            int32 damage = GetHitDamage();
            if (target->HealthBelowPct(20))
                if (AuraEffect const* glyph = GetCaster()->GetAuraEffect(SPELL_PALADIN_GLYPH_OF_FINAL_WRATH, EFFECT_0))
                    AddPct(damage, glyph->GetAmount());

            int32 targetCount = _targets.empty() ? 1 : int32(_targets.size());
            SetHitDamage(damage / targetCount);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pal_holy_wrath_SpellScript::SelectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pal_holy_wrath_SpellScript::CopyTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_pal_holy_wrath_SpellScript::HandleStun, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            OnHit += SpellHitFn(spell_pal_holy_wrath_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_holy_wrath_SpellScript();
    }
};

// 53600 - Shield of the Righteous
class spell_pal_shield_of_the_righteous : public SpellScriptLoader
{
public:
    spell_pal_shield_of_the_righteous() : SpellScriptLoader("spell_pal_shield_of_the_righteous") { }

    class spell_pal_shield_of_the_righteous_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_shield_of_the_righteous_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_PALADIN_SHIELD_OF_THE_RIGHTEOUS_PROC)
                && sSpellMgr->GetSpellInfo(SPELL_PALADIN_BASTION_OF_GLORY)
                && sSpellMgr->GetSpellInfo(SPELL_PALADIN_BASTION_OF_POWER);
        }

        void HandleHit()
        {
            Unit* caster = GetCaster();

            if (Aura* proc = caster->GetAura(SPELL_PALADIN_SHIELD_OF_THE_RIGHTEOUS_PROC))
            {
                int32 duration = proc->GetDuration() + 3 * IN_MILLISECONDS;
                proc->SetMaxDuration(duration);
                proc->SetDuration(duration);
            }
            else
                caster->CastSpell(caster, SPELL_PALADIN_SHIELD_OF_THE_RIGHTEOUS_PROC, true);

            caster->CastSpell(caster, SPELL_PALADIN_BASTION_OF_GLORY, true);

            if (caster->HasAura(SPELL_PALADIN_T16_PROTECTION_4P_BONUS))
                if (Aura* bastion = caster->GetAura(SPELL_PALADIN_BASTION_OF_GLORY))
                    if (bastion->GetStackAmount() >= 3)
                        caster->CastSpell(caster, SPELL_PALADIN_BASTION_OF_POWER, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pal_shield_of_the_righteous_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_shield_of_the_righteous_SpellScript();
    }
};

// 498 - Divine Protection
class spell_pal_divine_protection : public SpellScriptLoader
{
public:
    spell_pal_divine_protection() : SpellScriptLoader("spell_pal_divine_protection") { }

    class spell_pal_divine_protection_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_divine_protection_SpellScript);

        void HandleAfterCast()
        {
            Unit* caster = GetCaster();
            if (caster->HasAura(SPELL_PALADIN_T15_PROTECTION_4P_BONUS))
                caster->CastSpell(caster, SPELL_PALADIN_T15_PROTECTION_4P_DIVINE_PROTECTION, true);
            if (caster->HasAura(SPELL_PALADIN_T16_PROTECTION_2P_BONUS))
                caster->CastSpell(caster, SPELL_PALADIN_T16_PROTECTION_2P_DIVINE_PROTECTION, true);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_pal_divine_protection_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_divine_protection_SpellScript();
    }
};

// 138250 - Divine Protection (T15 Protection 4P)
class spell_pal_t15_protection_4p_bonus : public SpellScriptLoader
{
public:
    spell_pal_t15_protection_4p_bonus() : SpellScriptLoader("spell_pal_t15_protection_4p_bonus") { }

    class spell_pal_t15_protection_4p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_t15_protection_4p_bonus_AuraScript);

        uint32 _damageTaken = 0;

        void HandleProc(ProcEventInfo& eventInfo)
        {
            if (eventInfo.GetTypeMask() & (PROC_FLAG_TAKEN_SPELL_NONE_DMG_CLASS_POS | PROC_FLAG_TAKEN_SPELL_MAGIC_DMG_CLASS_POS))
                return;

            DamageInfo* damageInfo = eventInfo.GetDamageInfo();
            if (!damageInfo)
                return;

            _damageTaken += damageInfo->GetDamage();

            uint32 threshold = GetUnitOwner()->CountPctFromMaxHealth(20);
            if (threshold && _damageTaken >= threshold)
            {
                _damageTaken -= threshold;
                GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_PALADIN_HOLY_POWER_ENERGIZE, true);
            }
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_pal_t15_protection_4p_bonus_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pal_t15_protection_4p_bonus_AuraScript();
    }
};

// 149150 - Divine Protection (T16 Protection 2P)
class spell_pal_t16_protection_2p_bonus : public SpellScriptLoader
{
public:
    spell_pal_t16_protection_2p_bonus() : SpellScriptLoader("spell_pal_t16_protection_2p_bonus") { }

    class spell_pal_t16_protection_2p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_t16_protection_2p_bonus_AuraScript);

        uint32 _damageTaken = 0;

        void HandleProc(ProcEventInfo& eventInfo)
        {
            if (eventInfo.GetTypeMask() & (PROC_FLAG_TAKEN_SPELL_NONE_DMG_CLASS_POS | PROC_FLAG_TAKEN_SPELL_MAGIC_DMG_CLASS_POS))
                return;

            if (DamageInfo* damageInfo = eventInfo.GetDamageInfo())
                _damageTaken += damageInfo->GetDamage();
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* owner = GetUnitOwner();
            if (AuraEffect const* setBonus = owner->GetAuraEffect(SPELL_PALADIN_T16_PROTECTION_2P_BONUS, EFFECT_0))
                if (int32 heal = int32(CalculatePct(_damageTaken, setBonus->GetAmount()) / 10))
                    owner->CastCustomSpell(SPELL_PALADIN_BLESSING_OF_THE_GUARDIANS, SPELLVALUE_BASE_POINT0, heal, owner, true);
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_pal_t16_protection_2p_bonus_AuraScript::HandleProc);
            AfterEffectRemove += AuraEffectRemoveFn(spell_pal_t16_protection_2p_bonus_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pal_t16_protection_2p_bonus_AuraScript();
    }
};

// 76669 - Mastery: Illuminated Healing
class spell_pal_mastery_illuminated_healing : public SpellScriptLoader
{
public:
    spell_pal_mastery_illuminated_healing() : SpellScriptLoader("spell_pal_mastery_illuminated_healing") { }

    class spell_pal_mastery_illuminated_healing_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_mastery_illuminated_healing_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_PALADIN_ILLUMINATED_HEALING) != NULL;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (GetUnitOwner()->GetTypeId() != TypeID::TYPEID_PLAYER || !IsPositiveSpellProc(eventInfo))
                return false;

            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            HealInfo* healInfo = eventInfo.GetHealInfo();
            return spellInfo && spellInfo->Id != SPELL_PALADIN_BEACON_OF_LIGHT_HEAL && eventInfo.GetActionTarget()
                && healInfo && healInfo->GetHeal();
        }

        void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            Unit* owner = GetUnitOwner();
            Unit* target = eventInfo.GetActionTarget();

            float masteryPct = owner->GetFloatValue(PLAYER_FIELD_MASTERY) * GetSpellInfo()->Effects[EFFECT_1].BasePoints / 100.0f;
            int32 absorb = int32(CalculatePct(float(eventInfo.GetHealInfo()->GetHeal()), masteryPct));
            if (absorb <= 0)
                return;

            int32 cap = int32(owner->GetMaxHealth() / 3);

            if (AuraEffect* shield = target->GetAuraEffect(SPELL_PALADIN_ILLUMINATED_HEALING, EFFECT_0, owner->GetGUID()))
            {
                shield->ChangeAmount(std::min(shield->GetAmount() + absorb, cap));
                shield->GetBase()->RefreshDuration();
            }
            else
                owner->CastCustomSpell(SPELL_PALADIN_ILLUMINATED_HEALING, SPELLVALUE_BASE_POINT0, std::min(absorb, cap), target, true);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_pal_mastery_illuminated_healing_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_pal_mastery_illuminated_healing_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pal_mastery_illuminated_healing_AuraScript();
    }
};

// 88819 - Daybreak
class spell_pal_daybreak : public SpellScriptLoader
{
public:
    spell_pal_daybreak() : SpellScriptLoader("spell_pal_daybreak") { }

    class spell_pal_daybreak_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_daybreak_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_PALADIN_DAYBREAK_HEAL) != NULL;
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            Unit* target = eventInfo.GetActionTarget();
            HealInfo* healInfo = eventInfo.GetHealInfo();
            if (!target || !healInfo)
                return;

            if (int32 heal = CalculatePct(int32(healInfo->GetHeal()), aurEff->GetAmount()))
                GetTarget()->CastCustomSpell(SPELL_PALADIN_DAYBREAK_HEAL, SPELLVALUE_BASE_POINT1, heal, target, true, NULL, aurEff);
        }

        void Register() OVERRIDE
        {
            OnEffectProc += AuraEffectProcFn(spell_pal_daybreak_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pal_daybreak_AuraScript();
    }
};

// 121129 - Daybreak
class spell_pal_daybreak_heal : public SpellScriptLoader
{
public:
    spell_pal_daybreak_heal() : SpellScriptLoader("spell_pal_daybreak_heal") { }

    class spell_pal_daybreak_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_daybreak_heal_SpellScript);

        int32 _targetCount = 1;

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if([](WorldObject* target)
            {
                Unit* unit = target->ToUnit();
                return !unit || (unit->IsSummon() && unit->ToTempSummon()->IsMinorGuardian());
            });
            targets.remove(GetExplTargetUnit());
            _targetCount = std::max(1, int32(targets.size()));
        }

        void HandleHeal(SpellEffIndex /*effIndex*/)
        {
            int32 heal = GetHitHeal();
            if (AuraEffect const* setBonus = GetCaster()->GetAuraEffect(SPELL_PALADIN_T15_HOLY_2P_BONUS, EFFECT_0))
                AddPct(heal, setBonus->GetAmount());
            SetHitHeal(heal / _targetCount);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pal_daybreak_heal_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ALLY);
            OnEffectHitTarget += SpellEffectFn(spell_pal_daybreak_heal_SpellScript::HandleHeal, EFFECT_1, SPELL_EFFECT_HEAL);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_daybreak_heal_SpellScript();
    }
};

// 19750 - Flash of Light, 82326 - Divine Light
class spell_pal_tower_of_radiance : public SpellScriptLoader
{
public:
    spell_pal_tower_of_radiance() : SpellScriptLoader("spell_pal_tower_of_radiance") { }

    class spell_pal_tower_of_radiance_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_tower_of_radiance_SpellScript);

        void HandleHit()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (target && caster->HasAura(SPELL_PALADIN_TOWER_OF_RADIANCE) && target->HasAura(SPELL_PALADIN_BEACON_OF_LIGHT_MARKER, caster->GetGUID()))
                caster->CastSpell(caster, SPELL_PALADIN_TOWER_OF_RADIANCE_ENERGIZE, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pal_tower_of_radiance_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_tower_of_radiance_SpellScript();
    }
};

// 19750 - Flash of Light
class spell_pal_pvp_p14_holy : public SpellScriptLoader
{
public:
    spell_pal_pvp_p14_holy() : SpellScriptLoader("spell_pal_pvp_p14_holy") { }

    class spell_pal_pvp_p14_holy_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_pvp_p14_holy_SpellScript);

        void HandleCast()
        {
            Unit* caster = GetCaster();
            if (caster->HasAura(SPELL_PALADIN_PVP_HOLY_4P_BONUS))
                caster->CastSpell(caster, SPELL_PALADIN_PVP_HOLY_4P_ENERGIZE, true);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_pal_pvp_p14_holy_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_pvp_p14_holy_SpellScript();
    }
};

// 93466 - Glyph of Protector of the Innocent
class spell_pal_glyph_of_protector_of_the_innocent : public SpellScriptLoader
{
public:
    spell_pal_glyph_of_protector_of_the_innocent() : SpellScriptLoader("spell_pal_glyph_of_protector_of_the_innocent") { }

    class spell_pal_glyph_of_protector_of_the_innocent_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_glyph_of_protector_of_the_innocent_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            HealInfo* healInfo = eventInfo.GetHealInfo();
            return eventInfo.GetSpellInfo() && eventInfo.GetActionTarget() != GetUnitOwner() && healInfo && healInfo->GetHeal();
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            if (int32 heal = CalculatePct(int32(eventInfo.GetHealInfo()->GetHeal()), aurEff->GetAmount()))
                GetUnitOwner()->CastCustomSpell(SPELL_PALADIN_PROTECTOR_OF_THE_INNOCENT_HEAL, SPELLVALUE_BASE_POINT0, heal, GetUnitOwner(), true, NULL, aurEff);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_pal_glyph_of_protector_of_the_innocent_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_pal_glyph_of_protector_of_the_innocent_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pal_glyph_of_protector_of_the_innocent_AuraScript();
    }
};

// 114165 - Holy Prism
class spell_pal_holy_prism : public SpellScriptLoader
{
public:
    spell_pal_holy_prism() : SpellScriptLoader("spell_pal_holy_prism") { }

    class spell_pal_holy_prism_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_holy_prism_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_PALADIN_HOLY_PRISM_ALLY)
                && sSpellMgr->GetSpellInfo(SPELL_PALADIN_HOLY_PRISM_ENEMY)
                && sSpellMgr->GetSpellInfo(SPELL_PALADIN_HOLY_PRISM_DAMAGE_VISUAL)
                && sSpellMgr->GetSpellInfo(SPELL_PALADIN_HOLY_PRISM_HEAL_VISUAL);
        }

        SpellCastResult CheckCast()
        {
            Unit* target = GetExplTargetUnit();
            if (!target)
                return SpellCastResult::SPELL_FAILED_BAD_TARGETS;

            if (!GetCaster()->IsValidAttackTarget(target) && !GetCaster()->IsValidAssistTarget(target))
                return SpellCastResult::SPELL_FAILED_BAD_TARGETS;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void HandleHit()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!target)
                return;

            if (caster->IsValidAttackTarget(target))
            {
                caster->CastSpell(target, SPELL_PALADIN_HOLY_PRISM_ENEMY, true);
                caster->CastSpell(target, SPELL_PALADIN_HOLY_PRISM_DAMAGE_VISUAL, true);
            }
            else
            {
                caster->CastSpell(target, SPELL_PALADIN_HOLY_PRISM_ALLY, true);
                caster->CastSpell(target, SPELL_PALADIN_HOLY_PRISM_HEAL_VISUAL, true);
            }
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_pal_holy_prism_SpellScript::CheckCast);
            OnHit += SpellHitFn(spell_pal_holy_prism_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_holy_prism_SpellScript();
    }
};

// 114852, 114871 - Holy Prism
class spell_pal_holy_prism_effect : public SpellScriptLoader
{
public:
    spell_pal_holy_prism_effect() : SpellScriptLoader("spell_pal_holy_prism_effect") { }

    class spell_pal_holy_prism_effect_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_holy_prism_effect_SpellScript);

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            Unit* prismTarget = GetExplTargetUnit();
            Unit* target = GetHitUnit();
            if (!prismTarget || !target)
                return;

            prismTarget->CastSpell(target, GetSpellInfo()->Id == SPELL_PALADIN_HOLY_PRISM_ENEMY ? SPELL_PALADIN_HOLY_PRISM_DAMAGE_VISUAL_AOE : SPELL_PALADIN_HOLY_PRISM_HEAL_VISUAL_AOE, true);
        }

        void SelectTargets(std::list<WorldObject*>& targets)
        {
            Skyfire::Containers::RandomResizeList(targets, 5);
        }

        void ClearTargets(std::list<WorldObject*>& targets)
        {
            targets.clear();
        }

        void Register() OVERRIDE
        {
            uint16 areaTarget = m_scriptSpellId == SPELL_PALADIN_HOLY_PRISM_ENEMY ? TARGET_UNIT_DEST_AREA_ALLY : TARGET_UNIT_DEST_AREA_ENEMY;
            OnEffectHitTarget += SpellEffectFn(spell_pal_holy_prism_effect_SpellScript::HandleHit, EFFECT_1, SPELL_EFFECT_ANY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pal_holy_prism_effect_SpellScript::SelectTargets, EFFECT_1, areaTarget);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pal_holy_prism_effect_SpellScript::ClearTargets, EFFECT_2, TARGET_UNIT_DEST_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_holy_prism_effect_SpellScript();
    }
};

// 114157 - Execution Sentence
class spell_pal_execution_sentence : public SpellScriptLoader
{
public:
    spell_pal_execution_sentence() : SpellScriptLoader("spell_pal_execution_sentence") { }

    class spell_pal_execution_sentence_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_execution_sentence_SpellScript);

        void HandleAfterCast()
        {
            Unit* caster = GetCaster();
            if (Unit* target = GetExplTargetUnit())
                caster->CastSpell(target, caster->IsFriendlyTo(target) ? SPELL_PALADIN_STAY_OF_EXECUTION : SPELL_PALADIN_EXECUTION_SENTENCE, true);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_pal_execution_sentence_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_execution_sentence_SpellScript();
    }
};

// 114916 - Execution Sentence, 114917 - Stay of Execution
class spell_pal_execution_sentence_effect : public SpellScriptLoader
{
public:
    spell_pal_execution_sentence_effect() : SpellScriptLoader("spell_pal_execution_sentence_effect") { }

    class spell_pal_execution_sentence_effect_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_execution_sentence_effect_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
        {
            canBeRecalculated = false;
            if (Unit* caster = GetCaster())
            {
                int32 spellPower = caster->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_HOLY);
                int32 coefficient = GetSpellInfo()->Effects[EFFECT_1].CalcValue(caster);
                amount = int32(spellPower * coefficient / 1000.0f + 26.72716306f * amount);
            }
        }

        void CalculateTickAmount(AuraEffect const* aurEff, int32& amount)
        {
            static float const tickMultipliers[] =
            {
                0.04115f, 0.04525f, 0.04975f, 0.05475f, 0.06025f, 0.06625f, 0.07295f, 0.08025f, 0.08815f, 0.44125f
            };

            uint32 tick = aurEff->GetTickNumber();
            float multiplier = tick >= 1 && tick <= 10 ? tickMultipliers[tick - 1] : 0.0f;
            amount = int32(amount * multiplier);
        }

        void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_ENEMY_SPELL)
                return;

            Unit* caster = GetCaster();
            if (!caster)
                return;

            uint32 spellId = GetId() == SPELL_PALADIN_EXECUTION_SENTENCE ? SPELL_PALADIN_EXECUTION_SENTENCE_DISPEL : SPELL_PALADIN_STAY_OF_EXECUTION_DISPEL;
            caster->CastCustomSpell(spellId, SPELLVALUE_BASE_POINT0, int32(aurEff->GetAmount() * 0.44125f), GetTarget(), true);
        }

        void Register() OVERRIDE
        {
            AuraType auraType = m_scriptSpellId == SPELL_PALADIN_EXECUTION_SENTENCE ? SPELL_AURA_PERIODIC_DAMAGE : SPELL_AURA_PERIODIC_HEAL;
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pal_execution_sentence_effect_AuraScript::CalculateAmount, EFFECT_0, auraType);
            DoEffectCalcTickAmount += AuraEffectCalcTickAmountFn(spell_pal_execution_sentence_effect_AuraScript::CalculateTickAmount, EFFECT_0, auraType);
            OnEffectRemove += AuraEffectRemoveFn(spell_pal_execution_sentence_effect_AuraScript::HandleRemove, EFFECT_0, auraType, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pal_execution_sentence_effect_AuraScript();
    }
};

// 633 - Lay on Hands
class spell_pal_lay_on_hands : public SpellScriptLoader
{
public:
    spell_pal_lay_on_hands() : SpellScriptLoader("spell_pal_lay_on_hands") { }

    class spell_pal_lay_on_hands_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_lay_on_hands_SpellScript);

        SpellCastResult CheckCast()
        {
            if (Unit* target = GetExplTargetUnit())
                if (target->HasAura(SPELL_PALADIN_FORBEARANCE))
                    return SpellCastResult::SPELL_FAILED_TARGET_AURASTATE;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void HandleHit()
        {
            Unit* caster = GetCaster();
            if (Unit* target = GetHitUnit())
                caster->CastSpell(target, SPELL_PALADIN_FORBEARANCE, true);

            if (caster->HasAura(SPELL_PALADIN_GLYPH_OF_DIVINITY))
                caster->CastSpell(caster, SPELL_PALADIN_GLYPH_OF_DIVINITY_ENERGIZE, true);
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_pal_lay_on_hands_SpellScript::CheckCast);
            OnHit += SpellHitFn(spell_pal_lay_on_hands_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_lay_on_hands_SpellScript();
    }
};

// 642 - Divine Shield
class spell_pal_divine_shield : public SpellScriptLoader
{
public:
    spell_pal_divine_shield() : SpellScriptLoader("spell_pal_divine_shield") { }

    class spell_pal_divine_shield_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_divine_shield_SpellScript);

        int32 _harmfulEffects = 0;

        SpellCastResult CheckCast()
        {
            if (GetCaster()->HasAura(SPELL_PALADIN_FORBEARANCE))
                return SpellCastResult::SPELL_FAILED_CASTER_AURASTATE;

            return SpellCastResult::SPELL_CAST_OK;
        }

        // The immunity removes harmful effects without dispelling them, so count them before it lands.
        void CountHarmfulEffects()
        {
            _harmfulEffects = 0;
            Unit::AuraApplicationMap const& auras = GetCaster()->GetAppliedAuras();
            for (Unit::AuraApplicationMap::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
                if (!itr->second->IsPositive() && !itr->second->GetBase()->IsPassive())
                    ++_harmfulEffects;
        }

        void CalculateHeal(SpellEffIndex /*effIndex*/)
        {
            if (Aura* glyph = GetCaster()->GetAura(SPELL_PALADIN_GLYPH_OF_DIVINE_SHIELD))
            {
                AuraEffect const* perEffect = glyph->GetEffect(EFFECT_0);
                AuraEffect const* maxEffects = glyph->GetEffect(EFFECT_1);
                if (perEffect && maxEffects)
                    SetEffectValue(std::min(_harmfulEffects, maxEffects->GetAmount()) * perEffect->GetAmount());
            }
        }

        void HandleHit()
        {
            GetCaster()->CastSpell(GetCaster(), SPELL_PALADIN_FORBEARANCE, true);
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_pal_divine_shield_SpellScript::CheckCast);
            BeforeCast += SpellCastFn(spell_pal_divine_shield_SpellScript::CountHarmfulEffects);
            OnEffectHitTarget += SpellEffectFn(spell_pal_divine_shield_SpellScript::CalculateHeal, EFFECT_3, SPELL_EFFECT_HEAL_PCT);
            OnHit += SpellHitFn(spell_pal_divine_shield_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_divine_shield_SpellScript();
    }
};

// 1022 - Hand of Protection
class spell_pal_hand_of_protection : public SpellScriptLoader
{
public:
    spell_pal_hand_of_protection() : SpellScriptLoader("spell_pal_hand_of_protection") { }

    class spell_pal_hand_of_protection_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_hand_of_protection_SpellScript);

        SpellCastResult CheckCast()
        {
            if (Unit* target = GetExplTargetUnit())
                if (target->HasAura(SPELL_PALADIN_FORBEARANCE))
                    return SpellCastResult::SPELL_FAILED_TARGET_AURASTATE;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void HandleHit()
        {
            if (Unit* target = GetHitUnit())
                GetCaster()->CastSpell(target, SPELL_PALADIN_FORBEARANCE, true);
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_pal_hand_of_protection_SpellScript::CheckCast);
            OnHit += SpellHitFn(spell_pal_hand_of_protection_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_hand_of_protection_SpellScript();
    }
};

// 114039 - Hand of Purity
class spell_pal_hand_of_purity : public SpellScriptLoader
{
public:
    spell_pal_hand_of_purity() : SpellScriptLoader("spell_pal_hand_of_purity") { }

    class spell_pal_hand_of_purity_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_hand_of_purity_AuraScript);

        int32 _absorbPct = 0;

        bool Load() OVERRIDE
        {
            _absorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
            return true;
        }

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
        {
            // Keep the school absorb aura from being consumed as a finite shield.
            amount = -1;
            canBeRecalculated = false;
        }

        void Absorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
        {
            SpellInfo const* spellInfo = dmgInfo.GetSpellInfo();
            if (!spellInfo || (!spellInfo->HasAura(SPELL_AURA_PERIODIC_DAMAGE) && !spellInfo->HasAura(SPELL_AURA_PERIODIC_DAMAGE_PERCENT)))
            {
                absorbAmount = 0;
                return;
            }

            absorbAmount = CalculatePct(dmgInfo.GetDamage(), _absorbPct);
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pal_hand_of_purity_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            OnEffectAbsorb += AuraEffectAbsorbFn(spell_pal_hand_of_purity_AuraScript::Absorb, EFFECT_0);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pal_hand_of_purity_AuraScript();
    }
};

// 121783 - Emancipate
class spell_pal_emancipate : public SpellScriptLoader
{
public:
    spell_pal_emancipate() : SpellScriptLoader("spell_pal_emancipate") { }

    class spell_pal_emancipate_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_emancipate_SpellScript);

        void HandleHit()
        {
            Unit* caster = GetCaster();
            uint32 const mechanicMask = (1 << MECHANIC_SNARE) | (1 << MECHANIC_ROOT);

            std::list<Aura*> movementImpairing;
            Unit::AuraApplicationMap const& auras = caster->GetAppliedAuras();
            for (Unit::AuraApplicationMap::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
            {
                Aura* aura = itr->second->GetBase();
                if (!itr->second->IsPositive() && (aura->GetSpellInfo()->GetAllEffectsMechanicMask() & mechanicMask))
                    movementImpairing.push_back(aura);
            }

            if (movementImpairing.empty())
                return;

            Skyfire::Containers::RandomResizeList(movementImpairing, 1);
            caster->RemoveAura(movementImpairing.front());
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pal_emancipate_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_emancipate_SpellScript();
    }
};

// 115750 - Blinding Light
class spell_pal_blinding_light : public SpellScriptLoader
{
public:
    spell_pal_blinding_light() : SpellScriptLoader("spell_pal_blinding_light") { }

    class spell_pal_blinding_light_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_blinding_light_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_PALADIN_BLINDING_LIGHT_CONFUSE)
                && sSpellMgr->GetSpellInfo(SPELL_PALADIN_BLINDING_LIGHT_STUN);
        }

        void HandleHit()
        {
            Unit* caster = GetCaster();
            if (Unit* target = GetHitUnit())
                caster->CastSpell(target, caster->HasAura(SPELL_PALADIN_GLYPH_OF_BLINDING_LIGHT) ? SPELL_PALADIN_BLINDING_LIGHT_STUN : SPELL_PALADIN_BLINDING_LIGHT_CONFUSE, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pal_blinding_light_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_blinding_light_SpellScript();
    }
};

// 31821 - Devotion Aura
class spell_pal_glyph_of_devotion_aura : public SpellScriptLoader
{
public:
    spell_pal_glyph_of_devotion_aura() : SpellScriptLoader("spell_pal_glyph_of_devotion_aura") { }

    class spell_pal_glyph_of_devotion_aura_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_glyph_of_devotion_aura_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (GetCaster()->HasAura(SPELL_PALADIN_GLYPH_OF_DEVOTION_AURA))
            {
                targets.clear();
                targets.push_back(GetCaster());
            }
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pal_glyph_of_devotion_aura_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_glyph_of_devotion_aura_SpellScript();
    }
};

// 64364 - Devotion Aura
class spell_pal_glyph_of_devotion_aura_trigger : public SpellScriptLoader
{
public:
    spell_pal_glyph_of_devotion_aura_trigger() : SpellScriptLoader("spell_pal_glyph_of_devotion_aura_trigger") { }

    class spell_pal_glyph_of_devotion_aura_trigger_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_glyph_of_devotion_aura_trigger_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (GetCaster()->HasAura(SPELL_PALADIN_GLYPH_OF_DEVOTION_AURA))
            {
                targets.clear();
                targets.push_back(GetCaster());
            }
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pal_glyph_of_devotion_aura_trigger_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pal_glyph_of_devotion_aura_trigger_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_CASTER_AREA_RAID);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pal_glyph_of_devotion_aura_trigger_SpellScript::FilterTargets, EFFECT_2, TARGET_UNIT_CASTER_AREA_RAID);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_glyph_of_devotion_aura_trigger_SpellScript();
    }
};

// 54943 - Glyph of Blessed Life
class spell_pal_glyph_of_blessed_life : public SpellScriptLoader
{
public:
    spell_pal_glyph_of_blessed_life() : SpellScriptLoader("spell_pal_glyph_of_blessed_life") { }

    class spell_pal_glyph_of_blessed_life_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_glyph_of_blessed_life_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            uint32 const mechanicMask = (1 << MECHANIC_STUN) | (1 << MECHANIC_FEAR) | (1 << MECHANIC_ROOT);
            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            return spellInfo && (spellInfo->GetAllEffectsMechanicMask() & mechanicMask);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_pal_glyph_of_blessed_life_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pal_glyph_of_blessed_life_AuraScript();
    }
};

// 20066 - Repentance, 105421 - Blinding Light
class spell_pal_censure_control_fix : public SpellScriptLoader
{
public:
    spell_pal_censure_control_fix() : SpellScriptLoader("spell_pal_censure_control_fix") { }

    class spell_pal_censure_control_fix_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_censure_control_fix_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            // Damage from any source other than Censure breaks the effect.
            if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
                return spellInfo->Id != SPELL_PALADIN_CENSURE;
            return true;
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_pal_censure_control_fix_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pal_censure_control_fix_AuraScript();
    }
};

// 125043 - Glyph of Contemplation
class spell_pal_glyph_of_contemplation : public SpellScriptLoader
{
public:
    spell_pal_glyph_of_contemplation() : SpellScriptLoader("spell_pal_glyph_of_contemplation") { }

    class spell_pal_glyph_of_contemplation_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_glyph_of_contemplation_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* player = GetTarget()->ToPlayer())
                player->learnSpell(SPELL_PALADIN_CONTEMPLATION, false);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* player = GetTarget()->ToPlayer())
                if (player->HasSpell(SPELL_PALADIN_CONTEMPLATION))
                    player->removeSpell(SPELL_PALADIN_CONTEMPLATION, false, false);
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_pal_glyph_of_contemplation_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_pal_glyph_of_contemplation_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pal_glyph_of_contemplation_AuraScript();
    }
};

// 54428 - Divine Plea
class spell_pal_divine_plea : public SpellScriptLoader
{
public:
    spell_pal_divine_plea() : SpellScriptLoader("spell_pal_divine_plea") { }

    class spell_pal_divine_plea_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_divine_plea_AuraScript);

        void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
        {
            Unit* owner = GetUnitOwner();

            int32 minAmount = CalculatePct(owner->GetMaxPower(POWER_MANA), GetSpellInfo()->Effects[EFFECT_1].BasePoints) / std::max(aurEff->GetTotalTicks(), 1);
            if (owner->HasAura(SPELL_PALADIN_GLYPH_OF_DIVINE_PLEA))
                minAmount /= 2;

            amount = std::max(int32(CalculatePct(owner->GetStat(STAT_SPIRIT), amount)), minAmount);
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pal_divine_plea_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_ENERGIZE);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pal_divine_plea_AuraScript();
    }
};

// 26573, 116467 - Consecration
class spell_pal_consecration : public SpellScriptLoader
{
public:
    spell_pal_consecration() : SpellScriptLoader("spell_pal_consecration") { }

    class spell_pal_consecration_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_consecration_AuraScript);

        bool Validate(SpellInfo const* spellInfo) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_CONSECRATION_DAMAGE))
                return false;

            if (spellInfo->Effects[EFFECT_2].Effect == SPELL_EFFECT_TRIGGER_SPELL)
                return sSpellMgr->GetSpellInfo(spellInfo->Effects[EFFECT_2].TriggerSpell) != NULL;

            return true;
        }

        void HandleTick(AuraEffect const* /*aurEff*/)
        {
            Unit* caster = GetCaster();
            if (!caster || caster != GetOwner())
                return;

            SpellInfo const* summonSpell = GetSpellInfo();
            if (summonSpell->Effects[EFFECT_2].Effect == SPELL_EFFECT_TRIGGER_SPELL)
                summonSpell = sSpellMgr->GetSpellInfo(summonSpell->Effects[EFFECT_2].TriggerSpell);

            std::list<TempSummon*> summons;
            caster->GetSummons(summons, uint32(summonSpell->Effects[EFFECT_2].MiscValue));
            for (std::list<TempSummon*>::const_iterator itr = summons.begin(); itr != summons.end(); ++itr)
                caster->CastSpell((*itr)->GetPositionX(), (*itr)->GetPositionY(), (*itr)->GetPositionZ(), SPELL_PALADIN_CONSECRATION_DAMAGE, true);
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_pal_consecration_AuraScript::HandleTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pal_consecration_AuraScript();
    }
};

// 88263 - Hammer of the Righteous
class spell_pal_hammer_of_the_righteous : public SpellScriptLoader
{
public:
    spell_pal_hammer_of_the_righteous() : SpellScriptLoader("spell_pal_hammer_of_the_righteous") { }

    class spell_pal_hammer_of_the_righteous_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_hammer_of_the_righteous_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_PALADIN_WEAKENED_BLOWS) != NULL;
        }

        void HandleHit()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!target)
                return;

            caster->CastSpell(target, SPELL_PALADIN_WEAKENED_BLOWS, true);

            if (AuraEffect const* glyph = caster->GetAuraEffect(SPELL_PALADIN_GLYPH_OF_HAMMER_OF_THE_RIGHTEOUS, EFFECT_0))
            {
                if (Aura* weakenedBlows = target->GetAura(SPELL_PALADIN_WEAKENED_BLOWS, caster->GetGUID()))
                {
                    int32 duration = weakenedBlows->CalcMaxDuration(caster);
                    AddPct(duration, glyph->GetAmount());
                    weakenedBlows->SetMaxDuration(duration);
                    weakenedBlows->SetDuration(duration);
                }
            }
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pal_hammer_of_the_righteous_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_hammer_of_the_righteous_SpellScript();
    }
};

// 86674 - Ancient Healer
class spell_pal_ancient_healer : public SpellScriptLoader
{
public:
    spell_pal_ancient_healer() : SpellScriptLoader("spell_pal_ancient_healer") { }

    class spell_pal_ancient_healer_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_ancient_healer_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            HealInfo* healInfo = eventInfo.GetHealInfo();
            return IsPositiveSpellProc(eventInfo) && eventInfo.GetActionTarget() && healInfo && healInfo->GetHeal();
        }

        void HandleProc(ProcEventInfo& eventInfo)
        {
            int32 heal = CalculatePct(int32(eventInfo.GetHealInfo()->GetHeal()), GetSpellInfo()->Effects[EFFECT_1].BasePoints);
            if (!heal)
                return;

            std::list<TempSummon*> summons;
            GetTarget()->GetSummons(summons, NPC_PALADIN_GUARDIAN_OF_ANCIENT_KINGS_HOLY);
            if (!summons.empty())
                summons.front()->CastCustomSpell(SPELL_PALADIN_LIGHT_OF_THE_ANCIENT_KINGS, SPELLVALUE_BASE_POINT0, heal, eventInfo.GetActionTarget(), true);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_pal_ancient_healer_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_pal_ancient_healer_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pal_ancient_healer_AuraScript();
    }
};

// 86698 - Guardian of Ancient Kings, 86703 - Ancient Crusader
class spell_pal_ancient_crusader : public SpellScriptLoader
{
public:
    spell_pal_ancient_crusader() : SpellScriptLoader("spell_pal_ancient_crusader") { }

    class spell_pal_ancient_crusader_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_ancient_crusader_AuraScript);

        void HandleProc(ProcEventInfo& /*eventInfo*/)
        {
            Unit* owner = GetUnitOwner();
            Unit* paladin = owner->GetTypeId() == TypeID::TYPEID_PLAYER ? owner : owner->GetOwner();
            if (paladin)
                paladin->CastSpell(paladin, SPELL_PALADIN_ANCIENT_POWER, true);
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_pal_ancient_crusader_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pal_ancient_crusader_AuraScript();
    }
};

// 86698 - Guardian of Ancient Kings
class spell_pal_guardian_of_ancient_kings : public SpellScriptLoader
{
public:
    spell_pal_guardian_of_ancient_kings() : SpellScriptLoader("spell_pal_guardian_of_ancient_kings") { }

    class spell_pal_guardian_of_ancient_kings_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_guardian_of_ancient_kings_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_PALADIN_ANCIENT_FURY) && sSpellMgr->GetSpellInfo(SPELL_PALADIN_ANCIENT_POWER);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* paladin = GetTarget();
            Aura* ancientPower = paladin->GetAura(SPELL_PALADIN_ANCIENT_POWER);
            if (!ancientPower)
                return;

            int32 damage = sSpellMgr->GetSpellInfo(SPELL_PALADIN_ANCIENT_FURY)->Effects[EFFECT_0].CalcValue(paladin) * ancientPower->GetStackAmount();
            paladin->CastCustomSpell(SPELL_PALADIN_ANCIENT_FURY, SPELLVALUE_BASE_POINT0, damage, paladin, true);
            paladin->RemoveAura(ancientPower);
        }

        void Register() OVERRIDE
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_pal_guardian_of_ancient_kings_AuraScript::HandleRemove, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pal_guardian_of_ancient_kings_AuraScript();
    }
};

// 31884 - Avenging Wrath
class spell_pal_avenging_wrath : public SpellScriptLoader
{
public:
    spell_pal_avenging_wrath() : SpellScriptLoader("spell_pal_avenging_wrath") { }

    class spell_pal_avenging_wrath_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_avenging_wrath_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_PALADIN_SANCTIFIED_WRATH) != NULL;
        }

        void InitEffects(uint32& effectMask)
        {
            Unit* paladin = GetCaster();
            if (!paladin || paladin->GetTypeId() != TypeID::TYPEID_PLAYER)
                return;

            if (!HasActivePaladinSpec(paladin, SPEC_PALADIN_RETRIBUTION))
                effectMask &= ~(1 << EFFECT_2);

            if (!paladin->HasAura(SPELL_PALADIN_GLYPH_OF_THE_FALLING_AVENGER))
                effectMask &= ~(1 << EFFECT_3);
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* paladin = GetTarget();
            if (!paladin->HasAura(SPELL_PALADIN_SANCTIFIED_WRATH_TALENT))
                return;

            paladin->CastSpell(paladin, SPELL_PALADIN_SANCTIFIED_WRATH, true);
            if (Aura* sanctifiedWrath = paladin->GetAura(SPELL_PALADIN_SANCTIFIED_WRATH))
            {
                sanctifiedWrath->SetMaxDuration(GetMaxDuration());
                sanctifiedWrath->SetDuration(GetDuration());
            }
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->RemoveAurasDueToSpell(SPELL_PALADIN_SANCTIFIED_WRATH);
        }

        void Register() OVERRIDE
        {
            OnInitEffects += AuraInitEffectsFn(spell_pal_avenging_wrath_AuraScript::InitEffects);
            AfterEffectApply += AuraEffectApplyFn(spell_pal_avenging_wrath_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_pal_avenging_wrath_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pal_avenging_wrath_AuraScript();
    }
};

// 114232 - Sanctified Wrath
class spell_pal_sanctified_wrath : public SpellScriptLoader
{
public:
    spell_pal_sanctified_wrath() : SpellScriptLoader("spell_pal_sanctified_wrath") { }

    class spell_pal_sanctified_wrath_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_sanctified_wrath_AuraScript);

        bool Load() OVERRIDE
        {
            return GetUnitOwner()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void InitEffects(uint32& effectMask)
        {
            Player* paladin = GetUnitOwner()->ToPlayer();
            uint32 specMask = 0;
            switch (paladin->GetTalentSpecialization(paladin->GetActiveSpec()))
            {
                case SPEC_PALADIN_HOLY:
                    specMask = (1 << EFFECT_0) | (1 << EFFECT_4);
                    break;
                case SPEC_PALADIN_PROTECTION:
                    specMask = (1 << EFFECT_1) | (1 << EFFECT_3);
                    break;
                case SPEC_PALADIN_RETRIBUTION:
                    specMask = (1 << EFFECT_2);
                    break;
                default:
                    return;
            }

            effectMask &= specMask;
        }

        void Register() OVERRIDE
        {
            OnInitEffects += AuraInitEffectsFn(spell_pal_sanctified_wrath_AuraScript::InitEffects);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pal_sanctified_wrath_AuraScript();
    }
};

// 88821 - Daybreak
class spell_pal_daybreak_passive : public SpellScriptLoader
{
public:
    spell_pal_daybreak_passive() : SpellScriptLoader("spell_pal_daybreak_passive") { }

    class spell_pal_daybreak_passive_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_daybreak_passive_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            Spell const* spell = eventInfo.TriggeredBySpell();
            if (!spell)
                return false;

            return eventInfo.GetActionTarget() == spell->m_targets.GetUnitTarget();
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_pal_daybreak_passive_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pal_daybreak_passive_AuraScript();
    }
};

// 26023 - Pursuit of Justice
class spell_pal_pursuit_of_justice : public SpellScriptLoader
{
public:
    spell_pal_pursuit_of_justice() : SpellScriptLoader("spell_pal_pursuit_of_justice") { }

    class spell_pal_pursuit_of_justice_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_pursuit_of_justice_AuraScript);

        void HandleUpdate(uint32 /*diff*/, AuraEffect* aurEff)
        {
            Unit* paladin = GetUnitOwner();
            int32 holyPower = std::min(paladin->GetPower(POWER_HOLY_POWER), 3);
            int32 amount = GetSpellInfo()->Effects[EFFECT_0].CalcValue(paladin) + holyPower * GetSpellInfo()->Effects[EFFECT_7].CalcValue(paladin);
            if (aurEff->GetAmount() != amount)
                aurEff->ChangeAmount(amount);
        }

        void Register() OVERRIDE
        {
            OnEffectUpdate += AuraEffectUpdateFn(spell_pal_pursuit_of_justice_AuraScript::HandleUpdate, EFFECT_0, SPELL_AURA_MOD_INCREASE_SPEED);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pal_pursuit_of_justice_AuraScript();
    }
};

// 114918 - Light's Hammer
class spell_pal_lights_hammer : public SpellScriptLoader
{
public:
    spell_pal_lights_hammer() : SpellScriptLoader("spell_pal_lights_hammer") { }

    class spell_pal_lights_hammer_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_lights_hammer_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_PALADIN_ARCING_LIGHT_DAMAGE) && sSpellMgr->GetSpellInfo(SPELL_PALADIN_LIGHTS_HAMMER_HEAL);
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetUnitOwner()->SetNativeDisplayId(DISPLAY_PALADIN_LIGHTS_HAMMER);
            GetUnitOwner()->SetDisplayId(DISPLAY_PALADIN_LIGHTS_HAMMER);
        }

        void HandleTick(AuraEffect const* aurEff)
        {
            Unit* hammer = GetUnitOwner();
            Unit* paladin = hammer->GetOwner();
            if (!paladin)
                return;

            hammer->CastSpell(hammer->GetPositionX(), hammer->GetPositionY(), hammer->GetPositionZ(), SPELL_PALADIN_ARCING_LIGHT_DAMAGE, true, NULL, aurEff, paladin->GetGUID());
            hammer->CastSpell(hammer->GetPositionX(), hammer->GetPositionY(), hammer->GetPositionZ(), SPELL_PALADIN_LIGHTS_HAMMER_HEAL, true, NULL, aurEff, paladin->GetGUID());
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_pal_lights_hammer_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_pal_lights_hammer_AuraScript::HandleTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pal_lights_hammer_AuraScript();
    }
};

// 119952 - Arcing Light
class spell_pal_arcing_light_heal : public SpellScriptLoader
{
public:
    spell_pal_arcing_light_heal() : SpellScriptLoader("spell_pal_arcing_light_heal") { }

    class spell_pal_arcing_light_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_arcing_light_heal_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            SelectMostInjuredTargets(targets, 6);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pal_arcing_light_heal_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_arcing_light_heal_SpellScript();
    }
};

// 20165 - Seal of Insight
class spell_pal_seal_of_insight : public SpellScriptLoader
{
public:
    spell_pal_seal_of_insight() : SpellScriptLoader("spell_pal_seal_of_insight") { }

    class spell_pal_seal_of_insight_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_seal_of_insight_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            return !spellInfo || spellInfo->Id != SPELL_PALADIN_HAMMER_OF_THE_RIGHTEOUS_AOE;
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_pal_seal_of_insight_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pal_seal_of_insight_AuraScript();
    }
};

// 20167 - Seal of Insight (heal)
class spell_pal_seal_of_insight_heal : public SpellScriptLoader
{
public:
    spell_pal_seal_of_insight_heal() : SpellScriptLoader("spell_pal_seal_of_insight_heal") { }

    class spell_pal_seal_of_insight_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_seal_of_insight_heal_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (!GetCaster()->HasAura(SPELL_PALADIN_GLYPH_OF_THE_BATTLE_HEALER))
            {
                targets.clear();
                return;
            }

            SelectMostInjuredTargets(targets, 1);
        }

        void HandleSelfHeal(SpellEffIndex effIndex)
        {
            if (GetCaster()->HasAura(SPELL_PALADIN_GLYPH_OF_THE_BATTLE_HEALER))
                PreventHitDefaultEffect(effIndex);
        }

        void HandleHit()
        {
            if (AuraEffect const* glyph = GetCaster()->GetAuraEffect(SPELL_PALADIN_GLYPH_OF_THE_BATTLE_HEALER, EFFECT_0))
                SetHitHeal(CalculatePct(GetHitHeal(), glyph->GetAmount()));
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pal_seal_of_insight_heal_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_CASTER_AREA_RAID);
            OnEffectHitTarget += SpellEffectFn(spell_pal_seal_of_insight_heal_SpellScript::HandleSelfHeal, EFFECT_0, SPELL_EFFECT_HEAL);
            OnHit += SpellHitFn(spell_pal_seal_of_insight_heal_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_seal_of_insight_heal_SpellScript();
    }
};

// 13819, 23214, 34767, 34769, 66906, 69820, 69826, 73629, 73630 - Paladin class mounts
class spell_pal_glyph_of_mounted_king : public SpellScriptLoader
{
public:
    spell_pal_glyph_of_mounted_king() : SpellScriptLoader("spell_pal_glyph_of_mounted_king") { }

    class spell_pal_glyph_of_mounted_king_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_glyph_of_mounted_king_SpellScript);

        void HandleCast()
        {
            Unit* caster = GetCaster();
            if (caster->HasAura(SPELL_PALADIN_GLYPH_OF_THE_MOUNTED_KING))
                caster->AddAura(SPELL_PALADIN_BLESSING_OF_KINGS, caster);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_pal_glyph_of_mounted_king_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pal_glyph_of_mounted_king_SpellScript();
    }
};

// 46506 - Guardian of Ancient Kings (Retribution)
struct npc_pal_guardian_of_ancient_kings : public PetAI
{
    npc_pal_guardian_of_ancient_kings(Creature* creature) : PetAI(creature)
    {
        me->InitCharmInfo();
        me->SetReactState(REACT_ASSIST);
    }
};

void AddSC_paladin_spell_scripts()
{
    new spell_pal_holy_radiance();
    new spell_pal_divine_light();
    new spell_pal_flash_of_light();
    new spell_pal_judgment();
    new spell_pal_selfless_healer();
    new spell_pal_ardent_defender();
    new spell_pal_beacon_of_light();
    new spell_pal_blessing_of_faith();
    new spell_pal_eye_for_an_eye();
    new spell_pal_grand_crusader();
    new spell_pal_hand_of_sacrifice();
    new spell_pal_holy_shock();
    new spell_pal_item_healing_discount();
    new spell_pal_righteous_defense();
    new spell_pal_sacred_shield();
    new spell_pal_sacred_shield_absorb();
    new spell_pal_seal_of_righteousness();
    new spell_pal_word_of_glory();
    new spell_pal_word_of_glory_heal();
    new spell_pal_harsh_word();
    new spell_pal_eternal_flame();
    new spell_pal_light_of_dawn();
    new spell_pal_inquisition();
    new spell_pal_divine_purpose();
    new spell_pal_glyph_of_divine_storm();
    new spell_pal_art_of_war();
    new spell_pal_exorcism();
    new spell_pal_exorcism_glyphed();
    new spell_pal_crusader_strike();
    new spell_pal_t15_retribution_4p_bonus();
    new spell_pal_sword_of_light_damage_bonus_holy();
    new spell_pal_seal_of_truth_hotfix();
    new spell_pal_censure_hotfix();
    new spell_pal_seal_of_justice();
    new spell_pal_glyph_of_inquisition();
    new spell_pal_holy_wrath();
    new spell_pal_shield_of_the_righteous();
    new spell_pal_divine_protection();
    new spell_pal_t15_protection_4p_bonus();
    new spell_pal_t16_protection_2p_bonus();
    new spell_pal_mastery_illuminated_healing();
    new spell_pal_daybreak();
    new spell_pal_daybreak_heal();
    new spell_pal_tower_of_radiance();
    new spell_pal_pvp_p14_holy();
    new spell_pal_glyph_of_protector_of_the_innocent();
    new spell_pal_holy_prism();
    new spell_pal_holy_prism_effect();
    new spell_pal_execution_sentence();
    new spell_pal_execution_sentence_effect();
    new spell_pal_lay_on_hands();
    new spell_pal_divine_shield();
    new spell_pal_hand_of_protection();
    new spell_pal_hand_of_purity();
    new spell_pal_emancipate();
    new spell_pal_blinding_light();
    new spell_pal_glyph_of_devotion_aura();
    new spell_pal_glyph_of_devotion_aura_trigger();
    new spell_pal_glyph_of_blessed_life();
    new spell_pal_censure_control_fix();
    new spell_pal_glyph_of_contemplation();
    new spell_pal_divine_plea();
    new spell_pal_consecration();
    new spell_pal_hammer_of_the_righteous();
    new spell_pal_ancient_healer();
    new spell_pal_ancient_crusader();
    new spell_pal_guardian_of_ancient_kings();
    new spell_pal_glyph_of_double_jeopardy();
    new spell_pal_avenging_wrath();
    new spell_pal_sanctified_wrath();
    new spell_pal_daybreak_passive();
    new spell_pal_pursuit_of_justice();
    new spell_pal_lights_hammer();
    new spell_pal_arcing_light_heal();
    new spell_pal_seal_of_insight();
    new spell_pal_seal_of_insight_heal();
    new spell_pal_glyph_of_mounted_king();
    new creature_script<npc_pal_guardian_of_ancient_kings>("npc_pal_guardian_of_ancient_kings");
}
