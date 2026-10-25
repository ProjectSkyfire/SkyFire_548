/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/*
 * Scripts for spells with SPELLFAMILY_PRIEST and SPELLFAMILY_GENERIC spells used by priest players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_pri_".
 */

#include <array>
#include <map>
#include <set>
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "GridNotifiers.h"
#include "DBCStores.h"
#include "DynamicObject.h"
#include "TemporarySummon.h"
#include "ObjectAccessor.h"
#include "AreaTrigger.h"
#include "InstanceScript.h"

enum PriestSpells
{
    SPELL_PRIEST_INNER_FOCUS                        = 89485,
    SPELL_PRIEST_INNER_FOCUS_IMMUNITY               = 96267,

    SPELL_PRIEST_EVANGELISM_PROC                    = 81661,
    SPELL_PRIEST_BODY_AND_SOUL_PASSIVE              = 64129,
    SPELL_PRIEST_BODY_AND_SOUL_SPEED                = 65081,
    SPELL_PRIEST_WEAKENED_SOUL                      = 6788,
    SPELL_PRIEST_DIVINE_AEGIS                       = 47753,
    SPELL_PRIEST_DIVINE_AEGIS_PASSIVE               = 47515,
    SPELL_PRIEST_DIVINE_TOUCH                       = 63544,
    SPELL_PRIEST_GLYPH_OF_SHADOW                    = 107906,
    SPELL_PRIEST_ITEM_EFFICIENCY                    = 37595,
    SPELL_PRIEST_LEAP_OF_FAITH_EFFECT               = 92832,
    SPELL_PRIEST_LEAP_OF_FAITH_GLYPH                = 119850,
    SPELL_PRIEST_MANA_LEECH_PROC                    = 34650,
    SPELL_PRIEST_PENANCE_R1                         = 47540,
    SPELL_PRIEST_PENANCE_R1_DAMAGE                  = 47758,
    SPELL_PRIEST_PENANCE_R1_HEAL                    = 47757,
    SPELL_PRIEST_PENANCE_DAMAGE                     = 47666,
    SPELL_PRIEST_PENANCE_HEAL                       = 47750,

    SPELL_PRIEST_REFLECTIVE_SHIELD_GLYPH            = 33202,
    SPELL_PRIEST_REFLECTIVE_SHIELD_TRIGGERED        = 33619,

    SPELL_PRIEST_SHADOWFORM                         = 15473,
    SPELL_PRIEST_SHADOWFORM_VISUAL_WITHOUT_GLYPH    = 107903,
    SPELL_PRIEST_SHADOWFORM_VISUAL_WITH_GLYPH       = 107904,
    SPELL_PRIEST_SHADOW_WORD_DEATH                  = 32409,
    SPELL_PRIEST_SHADOW_WORD_DEATH_ENERGIZE         = 125927,
    SPELL_PRIEST_SHADOW_WORD_DEATH_COOLDOWN         = 95652,
    SPELL_PRIEST_T9_HEALING_2P                      = 67201,
    SPELL_PRIEST_VAMPIRIC_EMBRACE_HEAL              = 15290,

    // Shadow
    SPELL_PRIEST_SHADOW_WORD_PAIN                   = 589,
    SPELL_PRIEST_SHADOW_WORD_PAIN_SR                = 124464,
    SPELL_PRIEST_VAMPIRIC_TOUCH                     = 34914,
    SPELL_PRIEST_VAMPIRIC_TOUCH_SR                  = 124465,
    SPELL_PRIEST_DEVOURING_PLAGUE                   = 2944,
    SPELL_PRIEST_DEVOURING_PLAGUE_SR                = 124467,
    SPELL_PRIEST_DEVOURING_PLAGUE_HEAL              = 127626,
    SPELL_PRIEST_MIND_FLAY                          = 15407,
    SPELL_PRIEST_MIND_FLAY_INSANITY                 = 129197,
    SPELL_PRIEST_MIND_FLAY_SR                       = 124468,
    SPELL_PRIEST_MIND_SEAR_SR                       = 124469,
    SPELL_PRIEST_MIND_BLAST                         = 8092,
    SPELL_PRIEST_MASTERY_SHADOWY_RECALL             = 77486,
    SPELL_PRIEST_SHADOWY_APPARITION                 = 147193,
    SPELL_PRIEST_SOLACE_AND_INSANITY                = 139139,
    SPELL_PRIEST_GLYPH_OF_MIND_FLAY                 = 120585,
    SPELL_PRIEST_GLYPH_OF_MIND_FLAY_SPEED           = 120587,
    SPELL_PRIEST_SURGE_OF_DARKNESS                  = 87160,
    SPELL_PRIEST_SURGE_OF_DARKNESS_DOUBLE           = 126083,
    SPELL_PRIEST_DIVINE_INSIGHT_TALENT              = 109175,
    SPELL_PRIEST_DIVINE_INSIGHT_DISCIPLINE          = 123266,
    SPELL_PRIEST_DIVINE_INSIGHT_HOLY                = 123267,
    SPELL_PRIEST_VOID_TENDRILS_SUMMON               = 127665,
    SPELL_PRIEST_VOID_TENDRILS_GRASP                = 114404,
    SPELL_PRIEST_T13_SHADOW_2P                      = 105843,
    SPELL_PRIEST_S12_SHADOW_2P                      = 92711,
    SPELL_PRIEST_DISPERSION_SPRINT                  = 129960,
    SPELL_PRIEST_S12_SHADOW_4P                      = 131556,
    SPELL_PRIEST_SIN_AND_PUNISHMENT                 = 87204,
    SPELL_PRIEST_T15_SHADOW_2P                      = 138156,
    SPELL_PRIEST_DIVINE_INSIGHT_SHADOW              = 124430,
    SPELL_PRIEST_GLYPH_OF_MIND_SPIKE_PROC           = 81292,
    SPELL_PRIEST_GLYPH_OF_PSYCHIC_SCREAM            = 55676,

    // Discipline
    SPELL_PRIEST_SMITE                              = 585,
    SPELL_PRIEST_GLYPH_OF_SMITE                     = 55692,
    SPELL_PRIEST_POWER_WORD_SOLACE                  = 129250,
    SPELL_PRIEST_SOLACE_ENERGIZE                    = 129253,
    SPELL_PRIEST_SOLACE_HEAL                        = 140815,
    SPELL_PRIEST_SOLACE_HEAL_CRIT                   = 140816,
    SPELL_PRIEST_ATONEMENT_HEAL                     = 81751,
    SPELL_PRIEST_ATONEMENT_HEAL_CRIT                = 94472,
    SPELL_PRIEST_SPIRIT_SHELL                       = 109964,
    SPELL_PRIEST_SPIRIT_SHELL_ABSORB                = 114908,
    SPELL_PRIEST_STRENGTH_OF_SOUL                   = 89488,
    SPELL_PRIEST_EVANGELISM                         = 81662,
    SPELL_PRIEST_ARCHANGEL                          = 81700,
    SPELL_PRIEST_RAPTURE                            = 47536,
    SPELL_PRIEST_RAPTURE_ENERGIZE                   = 47755,
    SPELL_PRIEST_RAPTURE_COOLDOWN                   = 63853,
    SPELL_PRIEST_T13_HEALER_4P                      = 105832,
    SPELL_PRIEST_GUARDIAN_SPIRIT_HEAL               = 48153,
    SPELL_PRIEST_ANGELIC_BULWARK                    = 114214,
    SPELL_PRIEST_ANGELIC_BULWARK_COOLDOWN           = 114216,

    // Holy
    SPELL_PRIEST_RENEW                              = 139,
    SPELL_PRIEST_RAPID_RENEWAL                      = 95649,
    SPELL_PRIEST_GLYPH_OF_RENEW                     = 119872,
    SPELL_PRIEST_ECHO_OF_LIGHT_HEAL                 = 77489,
    SPELL_PRIEST_HOLY_WORD_SANCTUARY_AREA           = 88685,
    SPELL_PRIEST_HOLY_WORD_SANCTUARY_HEAL           = 88686,
    SPELL_PRIEST_HOLY_WORD_CHASTISE                 = 88625,
    SPELL_PRIEST_CIRCLE_OF_HEALING                  = 34861,
    SPELL_PRIEST_PRAYER_OF_MENDING                  = 33076,
    SPELL_PRIEST_PRAYER_OF_MENDING_DIVINE_INSIGHT   = 123259,
    SPELL_PRIEST_GLYPH_OF_BINDING_HEAL              = 63248,
    SPELL_PRIEST_SURGE_OF_LIGHT                     = 114255,
    SPELL_PRIEST_SURGE_OF_LIGHT_DOUBLE              = 128654,
    SPELL_PRIEST_LIGHTWELL_RENEW                    = 7001,

    // Glyphs and misc
    SPELL_PRIEST_LEVITATE_EFFECT                    = 111758,
    SPELL_PRIEST_GLYPH_OF_THE_HEAVENS               = 120581,
    SPELL_PRIEST_GLYPH_OF_THE_HEAVENS_VISUAL        = 124433,
    SPELL_PRIEST_GLYPH_OF_LEVITATE                  = 108939,
    SPELL_PRIEST_PATH_OF_THE_DEVOUT                 = 111757,
    SPELL_PRIEST_GLYPH_OF_MASS_DISPEL               = 55691,
    SPELL_PRIEST_GLYPH_OF_PURIFY                    = 55677,
    SPELL_PRIEST_GLYPH_OF_PURIFY_HEAL               = 56131,
    SPELL_PRIEST_GLYPH_OF_DISPEL_MAGIC              = 119864,
    SPELL_PRIEST_GLYPH_OF_DISPEL_MAGIC_DAMAGE       = 119856,
    SPELL_PRIEST_GLYPH_OF_INNER_SANCTUM             = 14771,
    SPELL_PRIEST_SPELL_WARDING                      = 91724,
    SPELL_PRIEST_GLYPH_OF_INSPIRED_HYMNS            = 147072,
    SPELL_PRIEST_GLYPH_OF_INSPIRED_HYMNS_VISUAL     = 147065,
    SPELL_PRIEST_PHANTASM_PROC                      = 114239,
    SPELL_PRIEST_POWER_WORD_BARRIER_EFFECT          = 81782,
    SPELL_PRIEST_ANGELIC_FEATHER_SPEED              = 121557,
    SPELL_PRIEST_VOID_SHIFT_EFFECT                  = 118594,
    SPELL_PRIEST_SPECTRAL_GUISE_CHARGES             = 119030,
    SPELL_PRIEST_SPECTRAL_GUISE_STEALTH             = 119032,
    SPELL_PRIEST_INHERIT_MASTERS_THREAT             = 58838,
    SPELL_PRIEST_CLONE_ME                           = 45204,
    SPELL_PRIEST_COPY_WEAPON                        = 41054,
    SPELL_PRIEST_PSYCHIC_TERROR                     = 113792,

    // Level 90 talents
    SPELL_PRIEST_HALO_HOLY                          = 120517,
    SPELL_PRIEST_HALO_SHADOW                        = 120644,
    SPELL_PRIEST_HALO_VISUAL_HOLY                   = 120630,
    SPELL_PRIEST_HALO_VISUAL_SHADOW                 = 120643,
    SPELL_PRIEST_HALO_EFFECT_HOLY                   = 120692,
    SPELL_PRIEST_HALO_EFFECT_SHADOW                 = 120696,
    SPELL_PRIEST_CASCADE_HOLY                       = 121135,
    SPELL_PRIEST_CASCADE_HOLY_DAMAGE                = 120785,
    SPELL_PRIEST_CASCADE_HOLY_SELECTOR              = 120786,
    SPELL_PRIEST_CASCADE_HOLY_MARKER                = 120840,
    SPELL_PRIEST_CASCADE_HOLY_MISSILE               = 121146,
    SPELL_PRIEST_CASCADE_HOLY_HEAL                  = 121148,
    SPELL_PRIEST_CASCADE_SHADOW_MISSILE             = 127627,
    SPELL_PRIEST_CASCADE_SHADOW_DAMAGE              = 127628,
    SPELL_PRIEST_CASCADE_SHADOW_HEAL                = 127629,
    SPELL_PRIEST_CASCADE_SHADOW_SELECTOR            = 127630,
    SPELL_PRIEST_CASCADE_SHADOW_MARKER              = 127631,
    SPELL_PRIEST_DIVINE_STAR_HOLY                   = 110744,
    SPELL_PRIEST_DIVINE_STAR_SHADOW                 = 122121,
    SPELL_PRIEST_DIVINE_STAR_SHADOW_DAMAGE          = 122128,

    // Set bonuses
    SPELL_PRIEST_S12_HEALER_2P                      = 33333,
    SPELL_PRIEST_HOLY_SPARK                         = 131567,
    SPELL_PRIEST_PIOUS_HEALER                       = 145330,
    SPELL_PRIEST_ABSOLUTION                         = 145336,
    SPELL_PRIEST_RESOLUTE_SPIRIT                    = 145374,
};

enum PriestSpellIcons
{
    PRIEST_ICON_ID_BORROWED_TIME                    = 2899,
    PRIEST_ICON_ID_PAIN_AND_SUFFERING               = 2874
};

enum PriestCreatures
{
    NPC_PRIEST_SPECTRAL_GUISE                       = 59607
};

// Shared with npc_pet_pri_psyfiend
enum PsyfiendData
{
    PSYFIEND_DATA_TERROR_TARGET                     = 1,
    PSYFIEND_DATA_TERRIFIED                         = 2
};

static bool IsPriestInSpec(Unit const* unit, uint32 spec)
{
    if (Player const* player = unit->ToPlayer())
        return player->GetTalentSpecialization(player->GetActiveSpec()) == spec;
    return false;
}

// Flat + base mana percent cost of a spell, before spell mods.
static int32 GetSpellBaseManaCost(Unit const* caster, uint32 spellId)
{
    for (uint32 i = 0; i < sSpellPowerStore.GetNumRows(); ++i)
    {
        SpellPowerEntry const* spellPower = sSpellPowerStore.LookupEntry(i);
        if (!spellPower || spellPower->spellId != spellId || spellPower->powerType != POWER_MANA)
            continue;

        return int32(spellPower->manaCost + CalculatePct(caster->GetCreateMana(), spellPower->ManaCostPercentageFloat));
    }
    return 0;
}

static bool SpellCostsPower(uint32 spellId, Powers power)
{
    for (uint32 i = 0; i < sSpellPowerStore.GetNumRows(); ++i)
    {
        SpellPowerEntry const* spellPower = sSpellPowerStore.LookupEntry(i);
        if (spellPower && spellPower->spellId == spellId && spellPower->powerType == uint32(power))
            return true;
    }
    return false;
}

// Keeps the most injured raid members of the target list.
static void SelectMostInjuredTargets(std::list<WorldObject*>& targets, Unit* caster, uint32 maxTargets)
{
    std::list<Unit*> units;
    for (WorldObject* target : targets)
        if (Unit* unit = target->ToUnit())
            if (unit->IsAlive() && (unit == caster || unit->IsInRaidWith(caster)))
                units.push_back(unit);

    units.sort(Skyfire::HealthPctOrderPred());
    if (units.size() > maxTargets)
        units.resize(maxTargets);

    targets.clear();
    for (Unit* unit : units)
        targets.push_back(unit);
}

static void RemoveInnerFocus(Unit* caster)
{
    if (caster->HasAura(SPELL_PRIEST_INNER_FOCUS))
        caster->RemoveAura(SPELL_PRIEST_INNER_FOCUS);
    caster->RemoveAurasDueToSpell(SPELL_PRIEST_INNER_FOCUS_IMMUNITY);
}

class spell_pri_prayer_of_healing : public SpellScriptLoader
{
public:
    spell_pri_prayer_of_healing() : SpellScriptLoader("spell_pri_prayer_of_healing") { }
    class spell_pri_prayer_of_healing_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_prayer_of_healing_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PRIEST_INNER_FOCUS))
                return false;
            return true;
        }
        void HandleRemoveAura(SpellEffIndex /*effIndex*/)
        {
            RemoveInnerFocus(GetCaster());
        }
        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_pri_prayer_of_healing_SpellScript::HandleRemoveAura, EFFECT_0, SPELL_EFFECT_HEAL);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_pri_prayer_of_healing_SpellScript();
    }
};

class spell_pri_greater_heal : public SpellScriptLoader
{
public:
    spell_pri_greater_heal() : SpellScriptLoader("spell_pri_greater_heal") { }
    class spell_pri_greater_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_greater_heal_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PRIEST_INNER_FOCUS))
                return false;
            return true;
        }
        void HandleRemoveAura(SpellEffIndex /*effIndex*/)
        {
            RemoveInnerFocus(GetCaster());
        }
        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_pri_greater_heal_SpellScript::HandleRemoveAura, EFFECT_0, SPELL_EFFECT_HEAL);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_pri_greater_heal_SpellScript();
    }
};

class spell_pri_flash_heal : public SpellScriptLoader
{
public:
    spell_pri_flash_heal() : SpellScriptLoader("spell_pri_flash_heal") { }
    class spell_pri_flash_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_flash_heal_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PRIEST_INNER_FOCUS))
                return false;
            return true;
        }
        void HandleRemoveAura(SpellEffIndex /*effIndex*/)
        {
            RemoveInnerFocus(GetCaster());
        }
        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_pri_flash_heal_SpellScript::HandleRemoveAura, EFFECT_0, SPELL_EFFECT_HEAL);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_pri_flash_heal_SpellScript();
    }
};

// 81662 - Evangelism
class spell_pri_evangelism : public SpellScriptLoader
{
public:
    spell_pri_evangelism() : SpellScriptLoader("spell_pri_evangelism") { }

    class spell_pri_evangelism_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_evangelism_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PRIEST_EVANGELISM_PROC))
                return false;
            return true;
        }
        void HandleEffectStackProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            // Proc only with Penance, Holy Fire, Smite
            if (!(eventInfo.GetDamageInfo()->GetSpellInfo()->SpellFamilyFlags[1] & 0x00800000 ||
                eventInfo.GetDamageInfo()->GetSpellInfo()->SpellFamilyFlags[0] & 0x00100000 ||
                eventInfo.GetDamageInfo()->GetSpellInfo()->SpellFamilyFlags[0] & 0x00000080))
                return;

            GetTarget()->CastCustomSpell(SPELL_PRIEST_EVANGELISM_PROC, SPELLVALUE_BASE_POINT0, aurEff->GetAmount(), eventInfo.GetProcTarget(), true, NULL, aurEff);
        }
        void Register() OVERRIDE
        {
            OnEffectProc += AuraEffectProcFn(spell_pri_evangelism_AuraScript::HandleEffectStackProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };
    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_evangelism_AuraScript();
    }
};

// 47515 - Divine Aegis
class spell_pri_divine_aegis : public SpellScriptLoader
{
public:
    spell_pri_divine_aegis() : SpellScriptLoader("spell_pri_divine_aegis") { }

    class spell_pri_divine_aegis_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_divine_aegis_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PRIEST_DIVINE_AEGIS))
                return false;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            return eventInfo.GetProcTarget() && eventInfo.GetHealInfo();
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            Unit* priest = GetTarget();
            Unit* target = eventInfo.GetProcTarget();
            int32 absorb = CalculatePct(int32(eventInfo.GetHealInfo()->GetHeal()), aurEff->GetAmount());

            // Multiple effects stack, so let's try to find this aura.
            if (AuraEffect const* aegis = target->GetAuraEffect(SPELL_PRIEST_DIVINE_AEGIS, EFFECT_0, priest->GetGUID()))
                absorb += aegis->GetAmount();

            absorb = std::min<int32>(absorb, priest->CountPctFromMaxHealth(60));

            priest->CastCustomSpell(SPELL_PRIEST_DIVINE_AEGIS, SPELLVALUE_BASE_POINT0, absorb, target, true, NULL, aurEff);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_pri_divine_aegis_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_pri_divine_aegis_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_divine_aegis_AuraScript();
    }
};

// 37594 - Greater Heal Refund
class spell_pri_item_greater_heal_refund : public SpellScriptLoader
{
public:
    spell_pri_item_greater_heal_refund() : SpellScriptLoader("spell_pri_item_greater_heal_refund") { }

    class spell_pri_item_greater_heal_refund_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_item_greater_heal_refund_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PRIEST_ITEM_EFFICIENCY))
                return false;
            return true;
        }

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            GetTarget()->CastSpell(GetTarget(), SPELL_PRIEST_ITEM_EFFICIENCY, true, NULL, aurEff);
        }

        void Register() OVERRIDE
        {
            OnEffectProc += AuraEffectProcFn(spell_pri_item_greater_heal_refund_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_item_greater_heal_refund_AuraScript();
    }
};

// 73325 - Leap of Faith
class spell_pri_leap_of_faith : public SpellScriptLoader
{
public:
    spell_pri_leap_of_faith() : SpellScriptLoader("spell_pri_leap_of_faith") { }

    class spell_pri_leap_of_faith_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_leap_of_faith_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PRIEST_LEAP_OF_FAITH_EFFECT))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_PRIEST_BODY_AND_SOUL_PASSIVE))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_PRIEST_BODY_AND_SOUL_SPEED))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_PRIEST_LEAP_OF_FAITH_GLYPH))
                return false;
            return true;
        }

        void HandleEffectDummy(SpellEffIndex /*effIndex*/)
        {
            Position destPos;
            GetHitDest()->GetPosition(&destPos);

            SpellCastTargets targets;
            targets.SetDst(destPos);
            targets.SetUnitTarget(GetCaster());
            GetHitUnit()->CastSpell(targets, sSpellMgr->GetSpellInfo(SPELL_PRIEST_LEAP_OF_FAITH_EFFECT), NULL);

            if (GetCaster()->HasAura(SPELL_PRIEST_BODY_AND_SOUL_PASSIVE))
                GetCaster()->CastSpell(GetHitUnit(), SPELL_PRIEST_BODY_AND_SOUL_SPEED, true);

            if (GetCaster()->HasAura(SPELL_PRIEST_LEAP_OF_FAITH_GLYPH))
                GetHitUnit()->RemoveMovementImpairingAuras();
        }


        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_pri_leap_of_faith_SpellScript::HandleEffectDummy, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_leap_of_faith_SpellScript();
    }
};

// 7001, 126154 - Lightwell Renew
class spell_pri_lightwell_renew : public SpellScriptLoader
{
public:
    spell_pri_lightwell_renew() : SpellScriptLoader("spell_pri_lightwell_renew") { }

    class spell_pri_lightwell_renew_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_lightwell_renew_AuraScript);

        bool Load() OVERRIDE
        {
            _receivedDamage = 0;
            return true;
        }

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (GetId() == SPELL_PRIEST_LIGHTWELL_RENEW)
                amount = amount * 3 / 2;
        }

        // The single proc charge must not be dropped by every hit, the renew only breaks after 30% of max health taken.
        void PrepareProc(ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
        }

        void HandleProc(ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            if (!eventInfo.GetDamageInfo())
                return;

            _receivedDamage += eventInfo.GetDamageInfo()->GetDamage();
            if (_receivedDamage >= GetUnitOwner()->CountPctFromMaxHealth(30))
                Remove();
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pri_lightwell_renew_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
            DoPrepareProc += AuraProcFn(spell_pri_lightwell_renew_AuraScript::PrepareProc);
            OnProc += AuraProcFn(spell_pri_lightwell_renew_AuraScript::HandleProc);
        }

    private:
        uint32 _receivedDamage;
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_lightwell_renew_AuraScript();
    }
};

// 59907 - Lightwell Charges
class spell_pri_lightwell_charges : public SpellScriptLoader
{
public:
    spell_pri_lightwell_charges() : SpellScriptLoader("spell_pri_lightwell_charges") { }

    class spell_pri_lightwell_charges_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_lightwell_charges_AuraScript);

        // Each Lightwell click consumes one proc charge, but the client data only gives the aura a single charge.
        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            uint8 charges = uint8(GetSpellInfo()->StackAmount);
            if (GetAura()->GetCharges() < charges)
                GetAura()->SetCharges(charges);
        }

        void Register() OVERRIDE
        {
            AfterEffectApply += AuraEffectApplyFn(spell_pri_lightwell_charges_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_lightwell_charges_AuraScript();
    }
};

// 28305 - Mana Leech (Passive) (Priest Pet Aura)
class spell_pri_mana_leech : public SpellScriptLoader
{
public:
    spell_pri_mana_leech() : SpellScriptLoader("spell_pri_mana_leech") { }

    class spell_pri_mana_leech_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_mana_leech_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PRIEST_MANA_LEECH_PROC))
                return false;
            return true;
        }

        bool Load() OVERRIDE
        {
            _procTarget = NULL;
            return true;
        }

        bool CheckProc(ProcEventInfo& /*eventInfo*/)
        {
            _procTarget = GetTarget()->GetOwner();
            return _procTarget;
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            GetTarget()->CastSpell(_procTarget, SPELL_PRIEST_MANA_LEECH_PROC, true, NULL, aurEff);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_pri_mana_leech_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_pri_mana_leech_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }

    private:
        Unit* _procTarget;
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_mana_leech_AuraScript();
    }
};

// 49821 - Mind Sear
class spell_pri_mind_sear : public SpellScriptLoader
{
public:
    spell_pri_mind_sear() : SpellScriptLoader("spell_pri_mind_sear") { }

    class spell_pri_mind_sear_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_mind_sear_SpellScript);

        void FilterTargets(std::list<WorldObject*>& unitList)
        {
            unitList.remove_if(Skyfire::ObjectGUIDCheck(GetCaster()->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT)));
        }

        void HandleMastery()
        {
            // Mastery: Shadowy Recall
            if (AuraEffect const* mastery = GetCaster()->GetAuraEffect(SPELL_PRIEST_MASTERY_SHADOWY_RECALL, EFFECT_0))
                if (roll_chance_i(mastery->GetAmount()))
                    GetCaster()->CastSpell(GetHitUnit(), SPELL_PRIEST_MIND_SEAR_SR, true);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_mind_sear_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            OnHit += SpellHitFn(spell_pri_mind_sear_SpellScript::HandleMastery);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_mind_sear_SpellScript();
    }
};

// 47540 - Penance
class spell_pri_penance : public SpellScriptLoader
{
public:
    spell_pri_penance() : SpellScriptLoader("spell_pri_penance") { }

    class spell_pri_penance_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_penance_SpellScript);

        bool Load() OVERRIDE
        {
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        bool Validate(SpellInfo const* spellInfo) OVERRIDE
        {
            SpellInfo const* firstRankSpellInfo = sSpellMgr->GetSpellInfo(SPELL_PRIEST_PENANCE_R1);
            if (!firstRankSpellInfo)
                return false;

            // can't use other spell than this penance due to spell_ranks dependency
            if (!spellInfo->IsRankOf(firstRankSpellInfo))
                return false;

            uint8 rank = spellInfo->GetRank();
            if (!sSpellMgr->GetSpellWithRank(SPELL_PRIEST_PENANCE_R1_DAMAGE, rank, true))
                return false;
            if (!sSpellMgr->GetSpellWithRank(SPELL_PRIEST_PENANCE_R1_HEAL, rank, true))
                return false;

            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (Unit* unitTarget = GetHitUnit())
            {
                if (!unitTarget->IsAlive())
                    return;

                uint8 rank = GetSpellInfo()->GetRank();

                if (caster->IsFriendlyTo(unitTarget))
                    caster->CastSpell(unitTarget, sSpellMgr->GetSpellWithRank(SPELL_PRIEST_PENANCE_R1_HEAL, rank), false);
                else
                {
                    caster->CastSpell(unitTarget, sSpellMgr->GetSpellWithRank(SPELL_PRIEST_PENANCE_R1_DAMAGE, rank), false);
                    if (caster->HasAura(SPELL_PRIEST_EVANGELISM))
                        caster->CastSpell(caster, SPELL_PRIEST_EVANGELISM_PROC, true);
                }

                if (caster->HasAura(SPELL_PRIEST_DIVINE_INSIGHT_TALENT))
                    caster->CastSpell(caster, SPELL_PRIEST_DIVINE_INSIGHT_DISCIPLINE, true);
            }
        }

        SpellCastResult CheckCast()
        {
            Player* caster = GetCaster()->ToPlayer();
            if (Unit* target = GetExplTargetUnit())
                if (!caster->IsFriendlyTo(target) && !caster->IsValidAttackTarget(target))
                    return SpellCastResult::SPELL_FAILED_BAD_TARGETS;
            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_pri_penance_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            OnCheckCast += SpellCheckCastFn(spell_pri_penance_SpellScript::CheckCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_penance_SpellScript;
    }
};

// 17 - Power Word: Shield, 123258 - Power Word: Shield (Divine Insight)
class spell_pri_power_word_shield : public SpellScriptLoader
{
public:
    spell_pri_power_word_shield() : SpellScriptLoader("spell_pri_power_word_shield") { }

    class spell_pri_power_word_shield_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_power_word_shield_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PRIEST_REFLECTIVE_SHIELD_TRIGGERED))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_PRIEST_REFLECTIVE_SHIELD_GLYPH))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_PRIEST_BODY_AND_SOUL_SPEED))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_PRIEST_WEAKENED_SOUL))
                return false;
            return true;
        }

        void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& canBeRecalculated)
        {
            canBeRecalculated = false;
            if (Unit* caster = GetCaster())
            {
                // +80.68% from sp bonus
                float bonus = 0.8068f;

                // Borrowed Time
                if (AuraEffect const* borrowedTime = caster->GetDummyAuraEffect(SPELLFAMILY_PRIEST, PRIEST_ICON_ID_BORROWED_TIME, EFFECT_1))
                    bonus += CalculatePct(1.0f, borrowedTime->GetAmount());

                bonus *= caster->SpellBaseHealingBonusDone(GetSpellInfo()->GetSchoolMask());

                // Improved PW: Shield: its weird having a SPELLMOD_ALL_EFFECTS here but its blizzards doing :)
                // Improved PW: Shield is only applied at the spell healing bonus because it was already applied to the base value in CalculateSpellDamage
                bonus = caster->ApplyEffectModifiers(GetSpellInfo(), aurEff->GetEffIndex(), bonus);
                bonus *= caster->CalculateLevelPenalty(GetSpellInfo());

                amount += int32(bonus);

                // Focused Power
                amount *= caster->GetTotalAuraMultiplier(SPELL_AURA_MOD_HEALING_DONE_PERCENT);

                // Divine Aegis: the shield can critically absorb
                if (caster->HasAura(SPELL_PRIEST_DIVINE_AEGIS_PASSIVE))
                    if (Player* player = caster->ToPlayer())
                        if (roll_chance_f(player->GetFloatValue(PLAYER_FIELD_SPELL_CRIT_PERCENTAGE + GetFirstSchoolInMask(GetSpellInfo()->GetSchoolMask()))))
                            amount *= 2;
            }
        }

        void ReflectDamage(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
        {
            Unit* target = GetTarget();
            if (dmgInfo.GetAttacker() == target)
                return;

            if (AuraEffect const* glyphAurEff = target->GetAuraEffectOfRankedSpell(SPELL_PRIEST_REFLECTIVE_SHIELD_GLYPH, EFFECT_0))
            {
                int32 bp = CalculatePct(absorbAmount, glyphAurEff->GetAmount());
                target->CastCustomSpell(dmgInfo.GetAttacker(), SPELL_PRIEST_REFLECTIVE_SHIELD_TRIGGERED, &bp, NULL, NULL, true, NULL, aurEff);
            }
        }

        void HandleOnEffectApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->CastSpell(GetTarget(), SPELL_PRIEST_WEAKENED_SOUL, true);

            // body and soul
            if (AuraEffect const* bodyandsoul = GetCaster()->GetAuraEffectOfRankedSpell(64129, EFFECT_0))
            {
                int32 bp = bodyandsoul->GetAmount();
                GetTarget()->CastCustomSpell(SPELL_PRIEST_BODY_AND_SOUL_SPEED, SPELLVALUE_BASE_POINT0, bp, NULL, true, NULL, aurEff);
            }
        }

        // Rapture
        void HandleAfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster || GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_ENEMY_SPELL)
                return;

            if (!caster->HasAura(SPELL_PRIEST_RAPTURE) || caster->HasAura(SPELL_PRIEST_RAPTURE_COOLDOWN))
                return;

            int32 amount = GetSpellBaseManaCost(caster, GetId());
            // Priest T13 Healer 4P Bonus
            if (AuraEffect const* setBonus = caster->GetAuraEffect(SPELL_PRIEST_T13_HEALER_4P, EFFECT_1))
                AddPct(amount, setBonus->GetAmount());

            if (amount > 0)
                caster->CastCustomSpell(SPELL_PRIEST_RAPTURE_ENERGIZE, SPELLVALUE_BASE_POINT0, amount, caster, true);
            caster->CastSpell(caster, SPELL_PRIEST_RAPTURE_COOLDOWN, true);
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pri_power_word_shield_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            AfterEffectAbsorb += AuraEffectAbsorbFn(spell_pri_power_word_shield_AuraScript::ReflectDamage, EFFECT_0);
            OnEffectApply += AuraEffectApplyFn(spell_pri_power_word_shield_AuraScript::HandleOnEffectApply, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            AfterEffectRemove += AuraEffectRemoveFn(spell_pri_power_word_shield_AuraScript::HandleAfterRemove, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_power_word_shield_AuraScript();
    }
};

// 33110 - Prayer of Mending Heal
class spell_pri_prayer_of_mending_heal : public SpellScriptLoader
{
public:
    spell_pri_prayer_of_mending_heal() : SpellScriptLoader("spell_pri_prayer_of_mending_heal") { }

    class spell_pri_prayer_of_mending_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_prayer_of_mending_heal_SpellScript);

        void HandleHeal(SpellEffIndex /*effIndex*/)
        {
            if (Unit* caster = GetOriginalCaster())
            {
                if (AuraEffect* aurEff = caster->GetAuraEffect(SPELL_PRIEST_T9_HEALING_2P, EFFECT_0))
                {
                    int32 heal = GetHitHeal();
                    AddPct(heal, aurEff->GetAmount());
                    SetHitHeal(heal);
                }
            }
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_pri_prayer_of_mending_heal_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_prayer_of_mending_heal_SpellScript();
    }
};

// 139 - Renew
class spell_pri_renew : public SpellScriptLoader
{
public:
    spell_pri_renew() : SpellScriptLoader("spell_pri_renew") { }

    class spell_pri_renew_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_renew_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PRIEST_DIVINE_TOUCH))
                return false;
            return true;
        }

        bool Load() OVERRIDE
        {
            return GetCaster() && GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void HandleApplyEffect(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
            {
                // Rapid Renewal
                if (AuraEffect const* rapidRenewal = caster->GetAuraEffect(SPELL_PRIEST_RAPID_RENEWAL, EFFECT_2))
                {
                    uint32 heal = caster->SpellHealingBonusDone(GetTarget(), GetSpellInfo(), aurEff->GetAmount(), DOT);
                    heal = GetTarget()->SpellHealingBonusTaken(caster, GetSpellInfo(), heal, DOT);
                    int32 basepoints0 = CalculatePct(int32(heal) * aurEff->GetTotalTicks(), rapidRenewal->GetAmount());
                    caster->CastCustomSpell(GetTarget(), SPELL_PRIEST_DIVINE_TOUCH, &basepoints0, NULL, NULL, true, NULL, aurEff);
                }
            }
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_pri_renew_AuraScript::HandleApplyEffect, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_renew_AuraScript();
    }
};

// 32379 - Shadow Word: Death
class spell_pri_shadow_word_death : public SpellScriptLoader
{
public:
    spell_pri_shadow_word_death() : SpellScriptLoader("spell_pri_shadow_word_death") { }

    class spell_pri_shadow_word_death_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_shadow_word_death_SpellScript);

        void HandleAfterHit()
        {
            Player* priest = GetCaster()->ToPlayer();
            if (!priest || !IsPriestInSpec(priest, SPEC_PRIEST_SHADOW))
                return;

            bool cooldown = priest->HasAura(SPELL_PRIEST_SHADOW_WORD_DEATH_COOLDOWN);
            if (!cooldown)
                priest->CastSpell(priest, SPELL_PRIEST_SHADOW_WORD_DEATH_ENERGIZE, true);

            if (GetHitUnit()->IsAlive() && !cooldown)
            {
                priest->RemoveSpellCooldown(GetSpellInfo()->Id, true);
                priest->CastSpell(priest, SPELL_PRIEST_SHADOW_WORD_DEATH_COOLDOWN, true);
            }
        }

        void Register() OVERRIDE
        {
            AfterHit += SpellHitFn(spell_pri_shadow_word_death_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_shadow_word_death_SpellScript();
    }
};

// 129176 - Shadow Word: Death (Glyph of Shadow Word: Death)
class spell_pri_shadow_word_death_glyphed : public SpellScriptLoader
{
public:
    spell_pri_shadow_word_death_glyphed() : SpellScriptLoader("spell_pri_shadow_word_death_glyphed") { }

    class spell_pri_shadow_word_death_glyphed_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_shadow_word_death_glyphed_SpellScript);

        bool Load() OVERRIDE
        {
            _below20 = false;
            return true;
        }

        void HandleOnHit()
        {
            Unit* target = GetHitUnit();
            if (target && target->HealthBelowPct(20))
            {
                _below20 = true;
                SetHitDamage(GetHitDamage() * 4);
            }
        }

        void HandleAfterHit()
        {
            Player* priest = GetCaster()->ToPlayer();
            Unit* target = GetHitUnit();
            if (!priest || !target)
                return;

            if (_below20 && IsPriestInSpec(priest, SPEC_PRIEST_SHADOW))
            {
                bool cooldown = priest->HasAura(SPELL_PRIEST_SHADOW_WORD_DEATH_COOLDOWN);
                if (!cooldown)
                    priest->CastSpell(priest, SPELL_PRIEST_SHADOW_WORD_DEATH_ENERGIZE, true);

                if (target->IsAlive() && !cooldown)
                {
                    priest->RemoveSpellCooldown(GetSpellInfo()->Id, true);
                    priest->CastSpell(priest, SPELL_PRIEST_SHADOW_WORD_DEATH_COOLDOWN, true);
                }
            }

            if (target->IsAlive())
            {
                int32 damage = GetHitDamage();
                // Priest T13 Shadow 2P Bonus
                if (AuraEffect const* setBonus = priest->GetAuraEffect(SPELL_PRIEST_T13_SHADOW_2P, EFFECT_1))
                    AddPct(damage, -setBonus->GetAmount());
                if (damage > 0)
                    priest->CastCustomSpell(SPELL_PRIEST_SHADOW_WORD_DEATH, SPELLVALUE_BASE_POINT0, damage, priest, true);
            }
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pri_shadow_word_death_glyphed_SpellScript::HandleOnHit);
            AfterHit += SpellHitFn(spell_pri_shadow_word_death_glyphed_SpellScript::HandleAfterHit);
        }

    private:
        bool _below20;
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_shadow_word_death_glyphed_SpellScript();
    }
};

// 15473 - Shadowform
class spell_pri_shadowform : public SpellScriptLoader
{
public:
    spell_pri_shadowform() : SpellScriptLoader("spell_pri_shadowform") { }

    class spell_pri_shadowform_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_shadowform_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PRIEST_SHADOWFORM_VISUAL_WITHOUT_GLYPH) ||
                !sSpellMgr->GetSpellInfo(SPELL_PRIEST_SHADOWFORM_VISUAL_WITH_GLYPH))
                return false;
            return true;
        }

        void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->CastSpell(GetTarget(), GetTarget()->HasAura(SPELL_PRIEST_GLYPH_OF_SHADOW) ? SPELL_PRIEST_SHADOWFORM_VISUAL_WITH_GLYPH : SPELL_PRIEST_SHADOWFORM_VISUAL_WITHOUT_GLYPH, true);
        }

        void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->RemoveAurasDueToSpell(SPELL_PRIEST_SHADOWFORM_VISUAL_WITH_GLYPH);
            GetTarget()->RemoveAurasDueToSpell(SPELL_PRIEST_SHADOWFORM_VISUAL_WITHOUT_GLYPH);
        }

        void Register() OVERRIDE
        {
            AfterEffectApply += AuraEffectApplyFn(spell_pri_shadowform_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            AfterEffectRemove += AuraEffectRemoveFn(spell_pri_shadowform_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_shadowform_AuraScript();
    }
};

// 107906 - Glyph of Shadow
class spell_pri_glyph_of_shadow : public SpellScriptLoader
{
public:
    spell_pri_glyph_of_shadow() : SpellScriptLoader("spell_pri_glyph_of_shadow") { }

    class spell_pri_glyph_of_shadow_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_glyph_of_shadow_AuraScript);

        bool Load() OVERRIDE
        {
            return GetUnitOwner()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* priest = GetUnitOwner();
            if (priest->HasAura(SPELL_PRIEST_SHADOWFORM))
            {
                priest->RemoveAurasDueToSpell(SPELL_PRIEST_SHADOWFORM_VISUAL_WITHOUT_GLYPH);
                priest->CastSpell(priest, SPELL_PRIEST_SHADOWFORM_VISUAL_WITH_GLYPH, true);
            }
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* priest = GetUnitOwner();
            if (priest->HasAura(SPELL_PRIEST_SHADOWFORM))
            {
                priest->RemoveAurasDueToSpell(SPELL_PRIEST_SHADOWFORM_VISUAL_WITH_GLYPH);
                priest->CastSpell(priest, SPELL_PRIEST_SHADOWFORM_VISUAL_WITHOUT_GLYPH, true);
            }
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_pri_glyph_of_shadow_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_pri_glyph_of_shadow_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_glyph_of_shadow_AuraScript();
    }
};

// 15286 - Vampiric Embrace
class spell_pri_vampiric_embrace : public SpellScriptLoader
{
public:
    spell_pri_vampiric_embrace() : SpellScriptLoader("spell_pri_vampiric_embrace") { }

    class spell_pri_vampiric_embrace_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_vampiric_embrace_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PRIEST_VAMPIRIC_EMBRACE_HEAL))
                return false;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            // Not proc from Mind Sear
            return !(eventInfo.GetDamageInfo()->GetSpellInfo()->SpellFamilyFlags[1] & 0x80000);
        }

        void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            int32 self = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount()));
            int32 team = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount() / 2));

            GetTarget()->CastCustomSpell((Unit*)NULL, SPELL_PRIEST_VAMPIRIC_EMBRACE_HEAL, &team, &self, NULL, true, NULL, aurEff);
        }

        void Register() OVERRIDE
        {
            // MoP Vampiric Embrace uses SPELL_AURA_PERIODIC_DUMMY on EFFECT_0.
            DoCheckProc += AuraCheckProcFn(spell_pri_vampiric_embrace_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_pri_vampiric_embrace_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_vampiric_embrace_AuraScript();
    }
};

// 15290 - Vampiric Embrace (heal)
class spell_pri_vampiric_embrace_target : public SpellScriptLoader
{
public:
    spell_pri_vampiric_embrace_target() : SpellScriptLoader("spell_pri_vampiric_embrace_target") { }

    class spell_pri_vampiric_embrace_target_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_vampiric_embrace_target_SpellScript);

        void FilterTargets(std::list<WorldObject*>& unitList)
        {
            unitList.remove(GetCaster());
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_vampiric_embrace_target_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_vampiric_embrace_target_SpellScript();
    }
};

// 34914 - Vampiric Touch
class spell_pri_vampiric_touch : public SpellScriptLoader
{
public:
    spell_pri_vampiric_touch() : SpellScriptLoader("spell_pri_vampiric_touch") { }

    class spell_pri_vampiric_touch_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_vampiric_touch_AuraScript);

        void HandleTick(AuraEffect const* /*aurEff*/)
        {
            if (Unit* priest = GetCaster())
                priest->EnergizeBySpell(priest, GetSpellInfo()->Id, CalculatePct(priest->GetMaxPower(POWER_MANA), 2), POWER_MANA);
        }

        // Priest S12 4P Bonus
        void HandleDispel(DispelInfo* dispelInfo)
        {
            if (Unit* priest = GetCaster())
                if (Unit* dispeller = dispelInfo->GetDispeller())
                    if (priest->HasAura(SPELL_PRIEST_S12_SHADOW_4P))
                        dispeller->CastSpell(dispeller, SPELL_PRIEST_SIN_AND_PUNISHMENT, true, NULL, NULL, priest->GetGUID());
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_pri_vampiric_touch_AuraScript::HandleTick, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
            AfterDispel += AuraDispelFn(spell_pri_vampiric_touch_AuraScript::HandleDispel);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_vampiric_touch_AuraScript();
    }
};

// 95740 - Shadow Orbs
class spell_pri_shadow_orbs : public SpellScriptLoader
{
public:
    spell_pri_shadow_orbs() : SpellScriptLoader("spell_pri_shadow_orbs") { }

    class spell_pri_shadow_orbs_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_shadow_orbs_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetUnitOwner()->SetMaxPower(POWER_SHADOW_ORBS, GetUnitOwner()->GetCreatePowers(POWER_SHADOW_ORBS));
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetUnitOwner()->SetMaxPower(POWER_SHADOW_ORBS, 0);
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_pri_shadow_orbs_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_MOD_POWER_REGEN, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_pri_shadow_orbs_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_MOD_POWER_REGEN, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_shadow_orbs_AuraScript();
    }
};

// 78203 - Shadowy Apparitions, 138158 - Item - Priest T15 Shadow 4P Bonus
class spell_pri_shadowy_apparitions : public SpellScriptLoader
{
public:
    spell_pri_shadowy_apparitions(char const* scriptName) : SpellScriptLoader(scriptName) { }

    class spell_pri_shadowy_apparitions_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_shadowy_apparitions_AuraScript);

        void HandleProc(ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            // Shadowy Apparitions do not pursue stealthed targets.
            Unit* target = eventInfo.GetActionTarget();
            if (target && GetUnitOwner()->CanSeeOrDetect(target))
                GetUnitOwner()->CastSpell(target, SPELL_PRIEST_SHADOWY_APPARITION, true, NULL, GetEffect(EFFECT_0));
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_pri_shadowy_apparitions_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_shadowy_apparitions_AuraScript();
    }
};

// 147193 - Shadowy Apparition
class spell_pri_shadowy_apparition : public SpellScriptLoader
{
public:
    spell_pri_shadowy_apparition() : SpellScriptLoader("spell_pri_shadowy_apparition") { }

    class spell_pri_shadowy_apparition_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_shadowy_apparition_SpellScript);

        void HandleCast()
        {
            typedef std::array<uint32, 2> Visual;
            static std::map<uint32, Visual> const visuals =
            {
                //  race                      male   female
                { RACE_HUMAN,               { { 33554, 33566 } } },
                { RACE_DWARF,               { { 33569, 33570 } } },
                { RACE_NIGHTELF,            { { 33571, 33572 } } },
                { RACE_UNDEAD_PLAYER,       { { 33573, 33574 } } },
                { RACE_TAUREN,              { { 33575, 33576 } } },
                { RACE_GNOME,               { { 33577, 33578 } } },
                { RACE_TROLL,               { { 33579, 33580 } } },
                { RACE_GOBLIN,              { { 33581, 33582 } } },
                { RACE_BLOODELF,            { { 33583, 33584 } } },
                { RACE_DRAENEI,             { { 33585, 33586 } } },
                { RACE_WORGEN,              { { 33587, 33588 } } },
                { RACE_PANDAREN_NEUTRAL,    { { 33589, 33590 } } },
                { RACE_PANDAREN_ALLIANCE,   { { 33589, 33590 } } },
                { RACE_PANDAREN_HORDE,      { { 33589, 33590 } } },
            };

            Unit* caster = GetCaster();
            Unit* target = GetExplTargetUnit();
            auto itr = visuals.find(caster->getRace());
            if (itr != visuals.end() && target)
                caster->SendPlaySpellVisual(itr->second[caster->getGender() ? 1 : 0], target->GetGUID(), GetSpellInfo()->Speed);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_pri_shadowy_apparition_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_shadowy_apparition_SpellScript();
    }
};

// 148859 - Shadowy Apparition (damage)
class spell_pri_t15_shadow_2p_bonus : public SpellScriptLoader
{
public:
    spell_pri_t15_shadow_2p_bonus() : SpellScriptLoader("spell_pri_t15_shadow_2p_bonus") { }

    class spell_pri_t15_shadow_2p_bonus_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_t15_shadow_2p_bonus_SpellScript);

        bool Load() OVERRIDE
        {
            return GetCaster()->HasAura(SPELL_PRIEST_T15_SHADOW_2P);
        }

        void HandleHit()
        {
            Unit* caster = GetCaster();
            AuraEffect const* setBonus = caster->GetAuraEffect(SPELL_PRIEST_T15_SHADOW_2P, EFFECT_0);
            if (!setBonus || !roll_chance_i(setBonus->GetAmount()))
                return;

            Unit::AuraEffectList const& effects = GetHitUnit()->GetAuraEffectsByType(SPELL_AURA_PERIODIC_DAMAGE);
            for (Unit::AuraEffectList::const_iterator itr = effects.begin(); itr != effects.end(); ++itr)
            {
                AuraEffect* effect = *itr;
                if (effect->GetCasterGUID() != caster->GetGUID())
                    continue;

                if (effect->GetId() != SPELL_PRIEST_SHADOW_WORD_PAIN && effect->GetId() != SPELL_PRIEST_VAMPIRIC_TOUCH)
                    continue;

                int32 tickDuration = effect->GetAmplitude();
                Aura* aura = effect->GetBase();
                if (aura->GetMaxDuration() < aura->GetDuration() + tickDuration)
                    aura->SetMaxDuration(aura->GetDuration() + tickDuration);
                aura->SetDuration(aura->GetDuration() + tickDuration);
            }
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pri_t15_shadow_2p_bonus_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_t15_shadow_2p_bonus_SpellScript();
    }
};

// 2944 - Devouring Plague
class spell_pri_devouring_plague : public SpellScriptLoader
{
public:
    spell_pri_devouring_plague() : SpellScriptLoader("spell_pri_devouring_plague") { }

    class spell_pri_devouring_plague_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_devouring_plague_SpellScript);

        bool Load() OVERRIDE
        {
            _orbs = 1;
            return true;
        }

        void HandleCast()
        {
            if (GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER)
                _orbs = std::max<int32>(1, GetCaster()->GetPower(POWER_SHADOW_ORBS));
        }

        void HandleDamage(SpellEffIndex /*effIndex*/)
        {
            SetHitDamage(GetHitDamage() * _orbs);
        }

        void HandleAfterHit()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!target)
                return;

            if (AuraEffect* dot = target->GetAuraEffect(SPELL_PRIEST_DEVOURING_PLAGUE, EFFECT_1, caster->GetGUID()))
            {
                dot->SetAmount(dot->CalculateAmount(caster) * _orbs);

                // The periodic dummy keeps the consumed orbs as percent of max health healed per tick.
                if (AuraEffect* dummy = target->GetAuraEffect(SPELL_PRIEST_DEVOURING_PLAGUE, EFFECT_2, caster->GetGUID()))
                {
                    dummy->SetAmount(_orbs * 100);
                    dummy->SetAmplitude(dot->GetAmplitude());
                    dummy->SetPeriodicTimer(dot->GetPeriodicTimer());
                }
            }
        }

        void HandleAfterCast()
        {
            if (GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER)
                GetCaster()->SetPower(POWER_SHADOW_ORBS, 0);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_pri_devouring_plague_SpellScript::HandleCast);
            OnEffectHitTarget += SpellEffectFn(spell_pri_devouring_plague_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            AfterHit += SpellHitFn(spell_pri_devouring_plague_SpellScript::HandleAfterHit);
            AfterCast += SpellCastFn(spell_pri_devouring_plague_SpellScript::HandleAfterCast);
        }

    private:
        int32 _orbs;
    };

    class spell_pri_devouring_plague_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_devouring_plague_AuraScript);

        void HandleTick(AuraEffect const* aurEff)
        {
            if (Unit* priest = GetCaster())
                if (int32 heal = aurEff->GetAmount() / 100)
                    priest->CastCustomSpell(SPELL_PRIEST_DEVOURING_PLAGUE_HEAL, SPELLVALUE_BASE_POINT0, heal, priest, true);
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_pri_devouring_plague_AuraScript::HandleTick, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_devouring_plague_SpellScript();
    }

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_devouring_plague_AuraScript();
    }
};

// 124467 - Devouring Plague (Mastery: Shadowy Recall)
class spell_pri_devouring_plague_shadowy_recall : public SpellScriptLoader
{
public:
    spell_pri_devouring_plague_shadowy_recall() : SpellScriptLoader("spell_pri_devouring_plague_shadowy_recall") { }

    class spell_pri_devouring_plague_shadowy_recall_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_devouring_plague_shadowy_recall_SpellScript);

        void HandleHit()
        {
            if (AuraEffect const* dummy = GetHitUnit()->GetAuraEffect(SPELL_PRIEST_DEVOURING_PLAGUE, EFFECT_2, GetCaster()->GetGUID()))
                SetHitDamage(CalculatePct(GetHitDamage(), dummy->GetAmount()));
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pri_devouring_plague_shadowy_recall_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_devouring_plague_shadowy_recall_SpellScript();
    }
};

// 77486 - Mastery: Shadowy Recall
class spell_pri_shadowy_recall : public SpellScriptLoader
{
public:
    spell_pri_shadowy_recall() : SpellScriptLoader("spell_pri_shadowy_recall") { }

    class spell_pri_shadowy_recall_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_shadowy_recall_AuraScript);

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            Unit* target = eventInfo.GetActionTarget();
            if (!spellInfo || !target)
                return;

            uint32 spellId = 0;
            switch (spellInfo->Id)
            {
                case SPELL_PRIEST_SHADOW_WORD_PAIN:
                    spellId = SPELL_PRIEST_SHADOW_WORD_PAIN_SR;
                    break;
                case SPELL_PRIEST_VAMPIRIC_TOUCH:
                    spellId = SPELL_PRIEST_VAMPIRIC_TOUCH_SR;
                    break;
                case SPELL_PRIEST_DEVOURING_PLAGUE:
                    spellId = SPELL_PRIEST_DEVOURING_PLAGUE_SR;
                    break;
                case SPELL_PRIEST_MIND_FLAY:
                case SPELL_PRIEST_MIND_FLAY_INSANITY:
                    spellId = SPELL_PRIEST_MIND_FLAY_SR;
                    break;
                default:
                    return;
            }

            if (!roll_chance_i(aurEff->GetAmount()))
                return;

            Unit* priest = GetUnitOwner();
            priest->CastSpell(target, spellId, true);

            switch (spellInfo->Id)
            {
                case SPELL_PRIEST_VAMPIRIC_TOUCH:
                    priest->EnergizeBySpell(priest, SPELL_PRIEST_VAMPIRIC_TOUCH, CalculatePct(priest->GetMaxPower(POWER_MANA), 2), POWER_MANA);
                    break;
                case SPELL_PRIEST_DEVOURING_PLAGUE:
                    if (AuraEffect const* plague = target->GetAuraEffect(SPELL_PRIEST_DEVOURING_PLAGUE, EFFECT_2, priest->GetGUID()))
                        if (int32 heal = plague->GetAmount() / 100)
                            priest->CastCustomSpell(SPELL_PRIEST_DEVOURING_PLAGUE_HEAL, SPELLVALUE_BASE_POINT0, heal, priest, true);
                    break;
                default:
                    break;
            }
        }

        void Register() OVERRIDE
        {
            OnEffectProc += AuraEffectProcFn(spell_pri_shadowy_recall_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_shadowy_recall_AuraScript();
    }
};

// 15407 - Mind Flay
class spell_pri_mind_flay_selector : public SpellScriptLoader
{
public:
    spell_pri_mind_flay_selector() : SpellScriptLoader("spell_pri_mind_flay_selector") { }

    class spell_pri_mind_flay_selector_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_mind_flay_selector_SpellScript);

        SpellCastResult CheckCast()
        {
            Unit* priest = GetCaster();
            Unit* target = GetExplTargetUnit();
            if (target && priest->HasAura(SPELL_PRIEST_SOLACE_AND_INSANITY) &&
                target->HasAura(SPELL_PRIEST_DEVOURING_PLAGUE, priest->GetGUID()))
            {
                uint64 targetGuid = target->GetGUID();
                priest->m_Events.Schedule(1, [priest, targetGuid]()
                {
                    if (Unit* insanityTarget = ObjectAccessor::GetUnit(*priest, targetGuid))
                        priest->CastSpell(insanityTarget, SPELL_PRIEST_MIND_FLAY_INSANITY, false);
                });
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;
            }
            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_pri_mind_flay_selector_SpellScript::CheckCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_mind_flay_selector_SpellScript();
    }
};

// 15407 - Mind Flay, 129197 - Mind Flay (Insanity)
class spell_pri_mind_flay : public SpellScriptLoader
{
public:
    spell_pri_mind_flay() : SpellScriptLoader("spell_pri_mind_flay") { }

    class spell_pri_mind_flay_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_mind_flay_AuraScript);

        // Glyph of Mind Flay removes the slow
        void HandleInitEffects(uint32& effectMask)
        {
            Unit* priest = GetCaster();
            if (priest && priest->HasAura(SPELL_PRIEST_GLYPH_OF_MIND_FLAY))
                effectMask &= ~(1 << EFFECT_1);
        }

        // Glyph of Mind Flay
        void HandleTick(AuraEffect const* /*aurEff*/)
        {
            if (Unit* priest = GetCaster())
                if (priest->HasAura(SPELL_PRIEST_GLYPH_OF_MIND_FLAY))
                    priest->CastSpell(priest, SPELL_PRIEST_GLYPH_OF_MIND_FLAY_SPEED, true);
        }

        void Register() OVERRIDE
        {
            OnInitEffects += AuraInitEffectsFn(spell_pri_mind_flay_AuraScript::HandleInitEffects);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_pri_mind_flay_AuraScript::HandleTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_mind_flay_AuraScript();
    }
};

// 129197 - Mind Flay (Insanity)
class spell_pri_mind_flay_insanity : public SpellScriptLoader
{
public:
    spell_pri_mind_flay_insanity() : SpellScriptLoader("spell_pri_mind_flay_insanity") { }

    class spell_pri_mind_flay_insanity_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_mind_flay_insanity_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (AuraEffect const* plague = GetUnitOwner()->GetAuraEffect(SPELL_PRIEST_DEVOURING_PLAGUE, EFFECT_2, GetCasterGUID()))
                AddPct(amount, CalculatePct(33.0f, plague->GetAmount()));
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pri_mind_flay_insanity_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_mind_flay_insanity_AuraScript();
    }
};

// 124468 - Mind Flay (Mastery: Shadowy Recall)
class spell_pri_mind_flay_shadowy_recall : public SpellScriptLoader
{
public:
    spell_pri_mind_flay_shadowy_recall() : SpellScriptLoader("spell_pri_mind_flay_shadowy_recall") { }

    class spell_pri_mind_flay_shadowy_recall_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_mind_flay_shadowy_recall_SpellScript);

        void HandleHit()
        {
            if (!GetCaster()->HasAura(SPELL_PRIEST_SOLACE_AND_INSANITY))
                return;

            if (AuraEffect const* plague = GetHitUnit()->GetAuraEffect(SPELL_PRIEST_DEVOURING_PLAGUE, EFFECT_2, GetCaster()->GetGUID()))
            {
                int32 damage = GetHitDamage();
                AddPct(damage, CalculatePct(33.0f, plague->GetAmount()));
                SetHitDamage(damage);
            }
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pri_mind_flay_shadowy_recall_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_mind_flay_shadowy_recall_SpellScript();
    }
};

// 73510 - Mind Spike
class spell_pri_mind_spike : public SpellScriptLoader
{
public:
    spell_pri_mind_spike() : SpellScriptLoader("spell_pri_mind_spike") { }

    class spell_pri_mind_spike_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_mind_spike_SpellScript);

        bool Load() OVERRIDE
        {
            _surge = false;
            return true;
        }

        void HandleCast()
        {
            if (Aura* surge = GetCaster()->GetAura(SPELL_PRIEST_SURGE_OF_DARKNESS))
            {
                surge->ModStackAmount(-1);
                _surge = true;
            }
        }

        void HandleHit()
        {
            Unit* target = GetHitUnit();
            if (_surge)
                SetHitDamage(GetHitDamage() * 3 / 2);
            else
            {
                target->RemoveAurasDueToSpell(SPELL_PRIEST_SHADOW_WORD_PAIN, GetCaster()->GetGUID());
                target->RemoveAurasDueToSpell(SPELL_PRIEST_DEVOURING_PLAGUE, GetCaster()->GetGUID());
                target->RemoveAurasDueToSpell(SPELL_PRIEST_VAMPIRIC_TOUCH, GetCaster()->GetGUID());
            }
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_pri_mind_spike_SpellScript::HandleCast);
            OnHit += SpellHitFn(spell_pri_mind_spike_SpellScript::HandleHit);
        }

    private:
        bool _surge;
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_mind_spike_SpellScript();
    }
};

// 48045 - Mind Sear
class spell_pri_mind_sear_check : public SpellScriptLoader
{
public:
    spell_pri_mind_sear_check() : SpellScriptLoader("spell_pri_mind_sear_check") { }

    class spell_pri_mind_sear_check_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_mind_sear_check_SpellScript);

        SpellCastResult CheckCast()
        {
            if (GetExplTargetUnit() == GetCaster())
                return SpellCastResult::SPELL_FAILED_BAD_TARGETS;
            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_pri_mind_sear_check_SpellScript::CheckCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_mind_sear_check_SpellScript();
    }
};

// 64044 - Psychic Horror
class spell_pri_psychic_horror : public SpellScriptLoader
{
public:
    spell_pri_psychic_horror() : SpellScriptLoader("spell_pri_psychic_horror") { }

    class spell_pri_psychic_horror_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_psychic_horror_SpellScript);

        bool Load() OVERRIDE
        {
            _orbs = 0;
            return true;
        }

        void HandleCast()
        {
            _orbs = GetCaster()->GetPower(POWER_SHADOW_ORBS);
            GetCaster()->SetPower(POWER_SHADOW_ORBS, 0);
        }

        void HandleHit()
        {
            if (Aura* psychicHorror = GetHitAura())
            {
                int32 newDuration = psychicHorror->GetMaxDuration() + _orbs * IN_MILLISECONDS;
                psychicHorror->SetDuration(newDuration);
                psychicHorror->SetMaxDuration(newDuration);
            }
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_pri_psychic_horror_SpellScript::HandleCast);
            OnHit += SpellHitFn(spell_pri_psychic_horror_SpellScript::HandleHit);
        }

    private:
        int32 _orbs;
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_psychic_horror_SpellScript();
    }
};

// 109175 - Divine Insight (Talent)
class spell_pri_divine_insight_talent : public SpellScriptLoader
{
public:
    spell_pri_divine_insight_talent() : SpellScriptLoader("spell_pri_divine_insight_talent") { }

    class spell_pri_divine_insight_talent_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_divine_insight_talent_AuraScript);

        bool CheckProc(ProcEventInfo& /*eventInfo*/)
        {
            return IsPriestInSpec(GetUnitOwner(), SPEC_PRIEST_SHADOW);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_pri_divine_insight_talent_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_divine_insight_talent_AuraScript();
    }
};

// 124430 - Divine Insight (Shadow)
class spell_pri_divine_insight_shadow : public SpellScriptLoader
{
public:
    spell_pri_divine_insight_shadow() : SpellScriptLoader("spell_pri_divine_insight_shadow") { }

    class spell_pri_divine_insight_shadow_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_divine_insight_shadow_SpellScript);

        void ResetCooldown()
        {
            if (Player* player = GetCaster()->ToPlayer())
                player->RemoveSpellCooldown(SPELL_PRIEST_MIND_BLAST, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pri_divine_insight_shadow_SpellScript::ResetCooldown);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_divine_insight_shadow_SpellScript();
    }
};

// 596 - Prayer of Healing, 2060 - Greater Heal
class spell_pri_divine_insight_holy : public SpellScriptLoader
{
public:
    spell_pri_divine_insight_holy() : SpellScriptLoader("spell_pri_divine_insight_holy") { }

    class spell_pri_divine_insight_holy_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_divine_insight_holy_SpellScript);

        void HandleCast()
        {
            Unit* priest = GetCaster();
            if (!IsPriestInSpec(priest, SPEC_PRIEST_HOLY))
                return;

            if (AuraEffect const* talent = priest->GetAuraEffect(SPELL_PRIEST_DIVINE_INSIGHT_TALENT, EFFECT_0))
                if (roll_chance_i(talent->GetAmount()))
                    priest->CastSpell(priest, SPELL_PRIEST_DIVINE_INSIGHT_HOLY, true);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_pri_divine_insight_holy_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_divine_insight_holy_SpellScript();
    }
};

// 109186 - From Darkness, Comes Light
class spell_pri_from_darkness_comes_light : public SpellScriptLoader
{
public:
    spell_pri_from_darkness_comes_light() : SpellScriptLoader("spell_pri_from_darkness_comes_light") { }

    class spell_pri_from_darkness_comes_light_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_from_darkness_comes_light_AuraScript);

        bool Load() OVERRIDE
        {
            return GetUnitOwner()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            if (!spellInfo || spellInfo->SpellFamilyName != SPELLFAMILY_PRIEST)
                return false;

            Player* priest = GetUnitOwner()->ToPlayer();
            switch (priest->GetTalentSpecialization(priest->GetActiveSpec()))
            {
                case SPEC_PRIEST_SHADOW:
                    if (spellInfo->Id == SPELL_PRIEST_VAMPIRIC_TOUCH || spellInfo->Id == SPELL_PRIEST_VAMPIRIC_TOUCH_SR)
                        return roll_chance_i(GetSpellInfo()->Effects[EFFECT_1].CalcValue());
                    return false;
                case SPEC_PRIEST_DISCIPLINE:
                case SPEC_PRIEST_HOLY:
                {
                    if (eventInfo.GetTypeMask() & PROC_FLAG_DONE_PERIODIC)
                        return false;

                    bool isHeal = spellInfo->HasEffect(SPELL_EFFECT_HEAL) ||
                        spellInfo->Id == SPELL_PRIEST_PRAYER_OF_MENDING ||
                        spellInfo->Id == SPELL_PRIEST_PRAYER_OF_MENDING_DIVINE_INSIGHT;

                    if (!isHeal && spellInfo->Id != SPELL_PRIEST_SMITE)
                        return false;

                    return roll_chance_i(GetSpellInfo()->Effects[EFFECT_0].CalcValue());
                }
                default:
                    return false;
            }
        }

        void HandleProc(ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();

            Unit* priest = GetUnitOwner();
            uint32 spellId = IsPriestInSpec(priest, SPEC_PRIEST_SHADOW) ? SPELL_PRIEST_SURGE_OF_DARKNESS : SPELL_PRIEST_SURGE_OF_LIGHT;
            priest->CastSpell(priest, spellId, true);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_pri_from_darkness_comes_light_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_pri_from_darkness_comes_light_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_from_darkness_comes_light_AuraScript();
    }
};

// 2061 - Flash Heal
class spell_pri_surge_of_light_consumer : public SpellScriptLoader
{
public:
    spell_pri_surge_of_light_consumer() : SpellScriptLoader("spell_pri_surge_of_light_consumer") { }

    class spell_pri_surge_of_light_consumer_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_surge_of_light_consumer_SpellScript);

        void HandleCast()
        {
            if (Aura* surgeOfLight = GetCaster()->GetAura(SPELL_PRIEST_SURGE_OF_LIGHT))
                surgeOfLight->ModStackAmount(-1);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_pri_surge_of_light_consumer_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_surge_of_light_consumer_SpellScript();
    }
};

// 87160 - Surge of Darkness, 114255 - Surge of Light
class spell_pri_surge_of : public SpellScriptLoader
{
public:
    spell_pri_surge_of() : SpellScriptLoader("spell_pri_surge_of") { }

    class spell_pri_surge_of_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_surge_of_AuraScript);

        uint32 GetDoubleVisualSpellId() const
        {
            return GetId() == SPELL_PRIEST_SURGE_OF_LIGHT ? SPELL_PRIEST_SURGE_OF_LIGHT_DOUBLE : SPELL_PRIEST_SURGE_OF_DARKNESS_DOUBLE;
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            uint32 visualId = GetDoubleVisualSpellId();
            if (GetStackAmount() >= 2)
            {
                if (!target->HasAura(visualId))
                    target->CastSpell(target, visualId, true);
            }
            else
                target->RemoveAurasDueToSpell(visualId);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->RemoveAurasDueToSpell(GetDoubleVisualSpellId());
        }

        void Register() OVERRIDE
        {
            AfterEffectApply += AuraEffectApplyFn(spell_pri_surge_of_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
            AfterEffectRemove += AuraEffectRemoveFn(spell_pri_surge_of_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_surge_of_AuraScript();
    }
};

// 109142 - Twist of Fate
class spell_pri_twist_of_fate : public SpellScriptLoader
{
public:
    spell_pri_twist_of_fate() : SpellScriptLoader("spell_pri_twist_of_fate") { }

    class spell_pri_twist_of_fate_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_twist_of_fate_AuraScript);

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            Unit* target = eventInfo.GetActionTarget();
            if (!target || target->GetHealthPct() >= float(aurEff->GetAmount()))
                PreventDefaultAction();
        }

        void Register() OVERRIDE
        {
            OnEffectProc += AuraEffectProcFn(spell_pri_twist_of_fate_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_twist_of_fate_AuraScript();
    }
};

// 34433 - Shadowfiend, 123040 - Mindbender
class spell_pri_shadowfiend : public SpellScriptLoader
{
public:
    spell_pri_shadowfiend() : SpellScriptLoader("spell_pri_shadowfiend") { }

    class spell_pri_shadowfiend_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_shadowfiend_SpellScript);

        void HandleAfterHit()
        {
            Player* player = GetCaster()->ToPlayer();
            Unit* target = GetExplTargetUnit();
            if (!player || !target)
                return;

            if (Guardian* pet = player->GetGuardianPet())
            {
                pet->InitCharmInfo();
                pet->SetReactState(REACT_DEFENSIVE);
                if (pet->AI())
                    pet->AI()->AttackStart(target);
            }
        }

        void Register() OVERRIDE
        {
            AfterHit += SpellHitFn(spell_pri_shadowfiend_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_shadowfiend_SpellScript();
    }
};

// 108920 - Void Tendrils
class spell_pri_void_tendrils : public SpellScriptLoader
{
public:
    spell_pri_void_tendrils() : SpellScriptLoader("spell_pri_void_tendrils") { }

    class spell_pri_void_tendrils_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_void_tendrils_SpellScript);

        void HandleAfterHit()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!target)
                return;

            caster->CastSpell(target, SPELL_PRIEST_VOID_TENDRILS_SUMMON, true);

            SpellInfo const* summonSpell = sSpellMgr->GetSpellInfo(SPELL_PRIEST_VOID_TENDRILS_SUMMON);
            if (!summonSpell)
                return;

            std::list<TempSummon*> tendrils;
            caster->GetSummons(tendrils, summonSpell->Effects[EFFECT_0].MiscValue);
            if (tendrils.empty())
                return;

            TempSummon* tendril = tendrils.back();
            tendril->CastSpell(target, SPELL_PRIEST_VOID_TENDRILS_GRASP, true);
            tendril->m_Events.Schedule(200, [tendril]()
            {
                if (Unit* grasped = ObjectAccessor::GetUnit(*tendril, tendril->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT)))
                    if (tendril->GetExactDist(grasped) > 0.1f)
                        tendril->NearTeleportTo(grasped->GetPositionX(), grasped->GetPositionY(), grasped->GetPositionZ(), grasped->GetOrientation());
            });
        }

        void Register() OVERRIDE
        {
            AfterHit += SpellHitFn(spell_pri_void_tendrils_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_void_tendrils_SpellScript();
    }
};

// 114404 - Void Tendril's Grasp
class spell_pri_void_tendrils_grasp : public SpellScriptLoader
{
public:
    spell_pri_void_tendrils_grasp() : SpellScriptLoader("spell_pri_void_tendrils_grasp") { }

    class spell_pri_void_tendrils_grasp_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_void_tendrils_grasp_AuraScript);

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
                if (Creature* tendril = caster->ToCreature())
                    tendril->DespawnOrUnsummon(1);
        }

        void Register() OVERRIDE
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_pri_void_tendrils_grasp_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_MOD_ROOT, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_void_tendrils_grasp_AuraScript();
    }
};

// 81749 - Atonement
class spell_pri_atonement : public SpellScriptLoader
{
public:
    spell_pri_atonement() : SpellScriptLoader("spell_pri_atonement") { }

    class spell_pri_atonement_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_atonement_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            // Power Word: Solace shares the family mask of Holy Fire and has its own heal.
            return eventInfo.GetSpellInfo() && eventInfo.GetSpellInfo()->Id != SPELL_PRIEST_POWER_WORD_SOLACE && eventInfo.GetDamageInfo();
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            int32 heal = CalculatePct(int32(eventInfo.GetDamageInfo()->GetDamage()), aurEff->GetAmount());
            if (!heal)
                return;

            uint32 spellId = SPELL_PRIEST_ATONEMENT_HEAL;
            if (eventInfo.GetHitMask() & PROC_EX_CRITICAL_HIT)
            {
                spellId = SPELL_PRIEST_ATONEMENT_HEAL_CRIT;
                heal /= 2;
            }

            Unit* priest = GetUnitOwner();
            // Glyph of Smite
            if (eventInfo.GetSpellInfo()->Id == SPELL_PRIEST_SMITE && eventInfo.GetActionTarget())
                if (AuraEffect const* glyph = priest->GetAuraEffect(SPELL_PRIEST_GLYPH_OF_SMITE, EFFECT_0))
                    if (eventInfo.GetActionTarget()->GetAuraEffect(SPELL_AURA_PERIODIC_DAMAGE, SPELLFAMILY_PRIEST, 0x100000, 0, 0, priest->GetGUID()))
                        ApplyPct(heal, 100 - glyph->GetAmount());

            // Spells triggered from a proc cannot proc themselves, so the heal is cast on the next update.
            priest->m_Events.Schedule(1, [priest, spellId, heal]()
            {
                priest->CastCustomSpell(spellId, SPELLVALUE_BASE_POINT0, heal, priest, true);
            });
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_pri_atonement_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_pri_atonement_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_atonement_AuraScript();
    }
};

// 81751, 94472 - Atonement, 140815, 140816 - Power Word: Solace
class spell_pri_atonement_heal : public SpellScriptLoader
{
public:
    spell_pri_atonement_heal() : SpellScriptLoader("spell_pri_atonement_heal") { }

    class spell_pri_atonement_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_atonement_heal_SpellScript);

        void SelectTarget(std::list<WorldObject*>& targets)
        {
            SelectMostInjuredTargets(targets, GetCaster(), 1);
            if (targets.empty())
                targets.push_back(GetCaster());
        }

        void HandleHit()
        {
            if (GetHitUnit() == GetCaster())
                SetHitHeal(GetHitHeal() / 2);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_atonement_heal_SpellScript::SelectTarget, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
            OnHit += SpellHitFn(spell_pri_atonement_heal_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_atonement_heal_SpellScript();
    }
};

// 596 - Prayer of Healing, 2050 - Heal, 2060 - Greater Heal, 2061 - Flash Heal
class spell_pri_spirit_shell : public SpellScriptLoader
{
public:
    spell_pri_spirit_shell() : SpellScriptLoader("spell_pri_spirit_shell") { }

    class spell_pri_spirit_shell_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_spirit_shell_SpellScript);

        void HandleHit()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!target || !caster->HasAura(SPELL_PRIEST_SPIRIT_SHELL))
                return;

            int32 absorb = GetHitHeal();
            SetHitHeal(0);

            // Mastery: Shield Discipline is applied by the absorb aura itself.
            if (Player* player = caster->ToPlayer())
                AddPct(absorb, player->GetFloatValue(PLAYER_FIELD_SPELL_CRIT_PERCENTAGE + SPELL_SCHOOL_HOLY));

            // Multiple effects stack, so let's try to find this aura.
            if (AuraEffect const* shell = target->GetAuraEffect(SPELL_PRIEST_SPIRIT_SHELL_ABSORB, EFFECT_0, caster->GetGUID()))
                absorb += shell->GetAmount();

            absorb = std::min<int32>(absorb, caster->CountPctFromMaxHealth(60));
            caster->CastCustomSpell(SPELL_PRIEST_SPIRIT_SHELL_ABSORB, SPELLVALUE_BASE_POINT0, absorb, target, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pri_spirit_shell_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_spirit_shell_SpellScript();
    }
};

// 81700 - Archangel
class spell_pri_archangel : public SpellScriptLoader
{
public:
    spell_pri_archangel() : SpellScriptLoader("spell_pri_archangel") { }

    class spell_pri_archangel_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_archangel_SpellScript);

        void HandleHit()
        {
            Unit* caster = GetCaster();
            Aura* evangelism = caster->GetAura(SPELL_PRIEST_EVANGELISM_PROC);
            Aura* archangel = GetHitAura();
            if (!evangelism || !archangel)
                return;

            if (AuraEffect* aurEff = archangel->GetEffect(EFFECT_0))
            {
                aurEff->ChangeAmount(aurEff->GetAmount() * evangelism->GetStackAmount());
                caster->RemoveAurasDueToSpell(SPELL_PRIEST_EVANGELISM_PROC);
            }
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pri_archangel_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_archangel_SpellScript();
    }
};

// 2050 - Heal, 2060 - Greater Heal, 2061 - Flash Heal
class spell_pri_strength_of_soul : public SpellScriptLoader
{
public:
    spell_pri_strength_of_soul() : SpellScriptLoader("spell_pri_strength_of_soul") { }

    class spell_pri_strength_of_soul_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_strength_of_soul_SpellScript);

        void HandleHit()
        {
            Unit* target = GetHitUnit();
            if (!target || !GetCaster()->HasAura(SPELL_PRIEST_STRENGTH_OF_SOUL))
                return;

            // Weakened Soul is cast by its target on itself.
            if (Aura* weakenedSoul = target->GetAura(SPELL_PRIEST_WEAKENED_SOUL))
            {
                if (weakenedSoul->GetDuration() > 2 * IN_MILLISECONDS)
                    weakenedSoul->SetDuration(weakenedSoul->GetDuration() - 2 * IN_MILLISECONDS);
                else
                    target->RemoveAurasDueToSpell(SPELL_PRIEST_WEAKENED_SOUL);
            }
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pri_strength_of_soul_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_strength_of_soul_SpellScript();
    }
};

// 129250 - Power Word: Solace
class spell_pri_power_word_solace : public SpellScriptLoader
{
public:
    spell_pri_power_word_solace() : SpellScriptLoader("spell_pri_power_word_solace") { }

    class spell_pri_power_word_solace_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_power_word_solace_SpellScript);

        void HandleHit()
        {
            if (Unit* target = GetHitUnit())
                GetCaster()->CastSpell(target, SPELL_PRIEST_SOLACE_ENERGIZE, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pri_power_word_solace_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_power_word_solace_SpellScript();
    }
};

// 139139 - Solace and Insanity
class spell_pri_solace_and_insanity : public SpellScriptLoader
{
public:
    spell_pri_solace_and_insanity() : SpellScriptLoader("spell_pri_solace_and_insanity") { }

    class spell_pri_solace_and_insanity_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_solace_and_insanity_AuraScript);

        void HandleProc(ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            if (!eventInfo.GetDamageInfo())
                return;

            int32 heal = int32(eventInfo.GetDamageInfo()->GetDamage());
            uint32 spellId = SPELL_PRIEST_SOLACE_HEAL;
            if (eventInfo.GetHitMask() & PROC_EX_CRITICAL_HIT)
            {
                spellId = SPELL_PRIEST_SOLACE_HEAL_CRIT;
                heal /= 2;
            }

            if (heal > 0)
                GetUnitOwner()->CastCustomSpell(spellId, SPELLVALUE_BASE_POINT0, heal, GetUnitOwner(), true);
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_pri_solace_and_insanity_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_solace_and_insanity_AuraScript();
    }
};

// 92297 - Train of Thought
class spell_pri_train_of_thought : public SpellScriptLoader
{
public:
    spell_pri_train_of_thought() : SpellScriptLoader("spell_pri_train_of_thought") { }

    class spell_pri_train_of_thought_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_train_of_thought_AuraScript);

        void HandleProc(ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            Player* priest = GetUnitOwner()->ToPlayer();
            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            if (!priest || !spellInfo)
                return;

            if (spellInfo->Id == SPELL_PRIEST_SMITE)
                priest->ModifySpellCooldown(SPELL_PRIEST_PENANCE_R1, -GetSpellInfo()->Effects[EFFECT_1].CalcValue());
            else
                priest->ModifySpellCooldown(SPELL_PRIEST_INNER_FOCUS, -GetSpellInfo()->Effects[EFFECT_0].CalcValue() * IN_MILLISECONDS);
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_pri_train_of_thought_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_train_of_thought_AuraScript();
    }
};

// 47788 - Guardian Spirit
class spell_pri_guardian_spirit : public SpellScriptLoader
{
public:
    spell_pri_guardian_spirit() : SpellScriptLoader("spell_pri_guardian_spirit") { }

    class spell_pri_guardian_spirit_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_guardian_spirit_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PRIEST_GUARDIAN_SPIRIT_HEAL))
                return false;
            return true;
        }

        bool Load() OVERRIDE
        {
            _healPct = GetSpellInfo()->Effects[EFFECT_1].CalcValue();
            return true;
        }

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            // Set absorbtion amount to unlimited
            amount = -1;
        }

        void Absorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
        {
            Unit* target = GetTarget();
            if (dmgInfo.GetDamage() < target->GetHealth())
                return;

            int32 healAmount = int32(target->CountPctFromMaxHealth(_healPct));
            // remove the aura now, we don't want 40% healing bonus
            Remove(AURA_REMOVE_BY_ENEMY_SPELL);
            target->CastCustomSpell(target, SPELL_PRIEST_GUARDIAN_SPIRIT_HEAL, &healAmount, NULL, NULL, true);
            absorbAmount = dmgInfo.GetDamage();
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pri_guardian_spirit_AuraScript::CalculateAmount, EFFECT_2, SPELL_AURA_SCHOOL_ABSORB);
            OnEffectAbsorb += AuraEffectAbsorbFn(spell_pri_guardian_spirit_AuraScript::Absorb, EFFECT_2);
        }

    private:
        int32 _healPct;
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_guardian_spirit_AuraScript();
    }
};

// 108945 - Angelic Bulwark
class spell_pri_angelic_bulwark : public SpellScriptLoader
{
public:
    spell_pri_angelic_bulwark() : SpellScriptLoader("spell_pri_angelic_bulwark") { }

    class spell_pri_angelic_bulwark_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_angelic_bulwark_AuraScript);

        bool CheckProc(ProcEventInfo& /*eventInfo*/)
        {
            Unit* priest = GetUnitOwner();
            return priest->HealthBelowPct(GetSpellInfo()->Effects[EFFECT_0].CalcValue()) && !priest->HasAura(SPELL_PRIEST_ANGELIC_BULWARK_COOLDOWN);
        }

        void HandleProc(ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();

            Unit* priest = GetUnitOwner();
            int32 absorb = priest->CountPctFromMaxHealth(GetSpellInfo()->Effects[EFFECT_1].CalcValue());
            priest->CastCustomSpell(SPELL_PRIEST_ANGELIC_BULWARK, SPELLVALUE_BASE_POINT0, absorb, priest, true);
            priest->CastSpell(priest, SPELL_PRIEST_ANGELIC_BULWARK_COOLDOWN, true);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_pri_angelic_bulwark_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_pri_angelic_bulwark_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_angelic_bulwark_AuraScript();
    }
};

// 45243 - Focused Will
class spell_pri_focused_will : public SpellScriptLoader
{
public:
    spell_pri_focused_will() : SpellScriptLoader("spell_pri_focused_will") { }

    class spell_pri_focused_will_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_focused_will_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (eventInfo.GetHitMask() & PROC_EX_CRITICAL_HIT)
                return true;

            return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() >= GetUnitOwner()->CountPctFromMaxHealth(10);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_pri_focused_will_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_focused_will_AuraScript();
    }
};

// 114239 - Phantasm
class spell_pri_phantasm_proc : public SpellScriptLoader
{
public:
    spell_pri_phantasm_proc() : SpellScriptLoader("spell_pri_phantasm_proc") { }

    class spell_pri_phantasm_proc_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_phantasm_proc_SpellScript);

        void HandleHit()
        {
            GetCaster()->RemoveMovementImpairingAuras();
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pri_phantasm_proc_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_phantasm_proc_SpellScript();
    }
};

// 77485 - Mastery: Echo of Light
class spell_pri_echo_of_light : public SpellScriptLoader
{
public:
    spell_pri_echo_of_light() : SpellScriptLoader("spell_pri_echo_of_light") { }

    class spell_pri_echo_of_light_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_echo_of_light_AuraScript);

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            Unit* target = eventInfo.GetActionTarget();
            if (!target || !eventInfo.GetHealInfo() || !eventInfo.GetSpellInfo())
                return;

            int32 heal = int32(eventInfo.GetHealInfo()->GetHeal());
            // Spread over the two ticks of the heal over time.
            int32 amount = CalculatePct(heal, aurEff->GetAmount()) / 2;
            if (amount > 0)
                GetUnitOwner()->CastCustomSpell(SPELL_PRIEST_ECHO_OF_LIGHT_HEAL, SPELLVALUE_BASE_POINT0, amount, target, true, NULL, aurEff);
        }

        void Register() OVERRIDE
        {
            OnEffectProc += AuraEffectProcFn(spell_pri_echo_of_light_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_echo_of_light_AuraScript();
    }
};

// 88685 - Holy Word: Sanctuary
class spell_pri_holy_word_sanctuary : public SpellScriptLoader
{
public:
    spell_pri_holy_word_sanctuary() : SpellScriptLoader("spell_pri_holy_word_sanctuary") { }

    class spell_pri_holy_word_sanctuary_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_holy_word_sanctuary_AuraScript);

        // The periodic dummy keeps the Absolution healing bonus snapshotted at cast
        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            amount = 0;
            if (Unit* caster = GetCaster())
                if (AuraEffect const* absolution = caster->GetAuraEffect(SPELL_PRIEST_ABSOLUTION, EFFECT_0))
                    amount = absolution->GetAmount();
        }

        void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (aurEff->GetAmount())
                if (Unit* caster = GetCaster())
                    caster->RemoveAurasDueToSpell(SPELL_PRIEST_ABSOLUTION);
        }

        void HandleTick(AuraEffect const* /*aurEff*/)
        {
            if (Unit* caster = GetCaster())
                if (DynamicObject* dynObj = caster->GetDynObject(SPELL_PRIEST_HOLY_WORD_SANCTUARY_AREA))
                    caster->CastSpell(dynObj->GetPositionX(), dynObj->GetPositionY(), dynObj->GetPositionZ(), SPELL_PRIEST_HOLY_WORD_SANCTUARY_HEAL, true);
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pri_holy_word_sanctuary_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            AfterEffectApply += AuraEffectApplyFn(spell_pri_holy_word_sanctuary_AuraScript::HandleApply, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_pri_holy_word_sanctuary_AuraScript::HandleTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_holy_word_sanctuary_AuraScript();
    }
};

// 88686 - Holy Word: Sanctuary (heal)
class spell_pri_holy_word_sanctuary_heal : public SpellScriptLoader
{
public:
    spell_pri_holy_word_sanctuary_heal() : SpellScriptLoader("spell_pri_holy_word_sanctuary_heal") { }

    class spell_pri_holy_word_sanctuary_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_holy_word_sanctuary_heal_SpellScript);

        void HandleHit()
        {
            if (AuraEffect const* sanctuary = GetCaster()->GetAuraEffect(SPELL_PRIEST_HOLY_WORD_SANCTUARY_AREA, EFFECT_1))
            {
                if (int32 bonus = sanctuary->GetAmount())
                {
                    int32 heal = GetHitHeal();
                    AddPct(heal, bonus);
                    SetHitHeal(heal);
                }
            }
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pri_holy_word_sanctuary_heal_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_holy_word_sanctuary_heal_SpellScript();
    }
};

// 145336 - Absolution
class spell_pri_absolution : public SpellScriptLoader
{
public:
    spell_pri_absolution() : SpellScriptLoader("spell_pri_absolution") { }

    class spell_pri_absolution_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_absolution_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            if (!spellInfo)
                return false;

            for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                if (AuraEffect const* aurEff = GetEffect(i))
                    if (aurEff->IsAffectingSpell(spellInfo))
                        return true;
            return false;
        }

        void HandleProc(ProcEventInfo& /*eventInfo*/)
        {
            Remove();
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_pri_absolution_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_pri_absolution_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_absolution_AuraScript();
    }
};

// 81208 - Chakra: Serenity
class spell_pri_chakra_serenity : public SpellScriptLoader
{
public:
    spell_pri_chakra_serenity() : SpellScriptLoader("spell_pri_chakra_serenity") { }

    class spell_pri_chakra_serenity_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_chakra_serenity_AuraScript);

        void HandleProc(ProcEventInfo& eventInfo)
        {
            Unit* target = eventInfo.GetActionTarget();
            if (!target)
                return;

            if (Aura* renew = target->GetAura(SPELL_PRIEST_RENEW, GetCasterGUID()))
            {
                renew->RefreshTimers();
                if (GetUnitOwner()->HasAura(SPELL_PRIEST_GLYPH_OF_RENEW))
                {
                    if (AuraEffect const* hot = renew->GetEffect(EFFECT_0))
                    {
                        renew->SetMaxDuration(renew->GetMaxDuration() + hot->GetAmplitude());
                        renew->SetDuration(renew->GetMaxDuration());
                    }
                }
            }
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_pri_chakra_serenity_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_chakra_serenity_AuraScript();
    }
};

// 81209 - Chakra: Chastise
class spell_pri_chakra_chastise : public SpellScriptLoader
{
public:
    spell_pri_chakra_chastise() : SpellScriptLoader("spell_pri_chakra_chastise") { }

    class spell_pri_chakra_chastise_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_chakra_chastise_AuraScript);

        void HandleProc(ProcEventInfo& /*eventInfo*/)
        {
            if (Player* priest = GetUnitOwner()->ToPlayer())
                priest->RemoveSpellCooldown(SPELL_PRIEST_HOLY_WORD_CHASTISE, true);
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_pri_chakra_chastise_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_chakra_chastise_AuraScript();
    }
};

// 32546 - Binding Heal
class spell_pri_binding_heal : public SpellScriptLoader
{
public:
    spell_pri_binding_heal() : SpellScriptLoader("spell_pri_binding_heal") { }

    class spell_pri_binding_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_binding_heal_SpellScript);

        void SelectTargets(std::list<WorldObject*>& targets)
        {
            Unit* explicitTarget = GetExplTargetUnit();
            if (!GetCaster()->HasAura(SPELL_PRIEST_GLYPH_OF_BINDING_HEAL))
            {
                targets.clear();
                if (explicitTarget)
                    targets.push_back(explicitTarget);
                return;
            }

            targets.remove(explicitTarget);
            targets.remove(GetCaster());

            SelectMostInjuredTargets(targets, GetCaster(), 1);

            if (explicitTarget)
                targets.push_back(explicitTarget);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_binding_heal_SpellScript::SelectTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ALLY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_binding_heal_SpellScript();
    }
};

// 41635 - Prayer of Mending
class spell_pri_prayer_of_mending_proc : public SpellScriptLoader
{
public:
    spell_pri_prayer_of_mending_proc() : SpellScriptLoader("spell_pri_prayer_of_mending_proc") { }

    class spell_pri_prayer_of_mending_proc_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_prayer_of_mending_proc_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (eventInfo.GetHitMask() & PROC_EX_ABSORB)
                if (!eventInfo.GetDamageInfo() || !eventInfo.GetDamageInfo()->GetDamage())
                    return false;

            return true;
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_pri_prayer_of_mending_proc_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_prayer_of_mending_proc_AuraScript();
    }
};

// 1706 - Levitate
class spell_pri_levitate : public SpellScriptLoader
{
public:
    spell_pri_levitate() : SpellScriptLoader("spell_pri_levitate") { }

    class spell_pri_levitate_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_levitate_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Unit* target = GetHitUnit())
                GetCaster()->CastSpell(target, SPELL_PRIEST_LEVITATE_EFFECT, true);
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_pri_levitate_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_levitate_SpellScript();
    }
};

// 111758 - Levitate
class spell_pri_glyph_of_heavens : public SpellScriptLoader
{
public:
    spell_pri_glyph_of_heavens() : SpellScriptLoader("spell_pri_glyph_of_heavens") { }

    class spell_pri_glyph_of_heavens_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_glyph_of_heavens_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* priest = GetCaster())
                if (priest->HasAura(SPELL_PRIEST_GLYPH_OF_THE_HEAVENS))
                    GetTarget()->CastSpell(GetTarget(), SPELL_PRIEST_GLYPH_OF_THE_HEAVENS_VISUAL, true);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->RemoveAurasDueToSpell(SPELL_PRIEST_GLYPH_OF_THE_HEAVENS_VISUAL);
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_pri_glyph_of_heavens_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_FEATHER_FALL, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_pri_glyph_of_heavens_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_FEATHER_FALL, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_glyph_of_heavens_AuraScript();
    }
};

// 111758 - Levitate
class spell_pri_glyph_of_levitate : public SpellScriptLoader
{
public:
    spell_pri_glyph_of_levitate() : SpellScriptLoader("spell_pri_glyph_of_levitate") { }

    class spell_pri_glyph_of_levitate_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_glyph_of_levitate_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetUnitOwner() == GetCaster() && GetUnitOwner()->HasAura(SPELL_PRIEST_GLYPH_OF_LEVITATE))
                GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_PRIEST_PATH_OF_THE_DEVOUT, true);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Aura* aura = GetUnitOwner()->GetAura(SPELL_PRIEST_PATH_OF_THE_DEVOUT))
            {
                aura->SetMaxDuration(10 * IN_MILLISECONDS);
                aura->SetDuration(10 * IN_MILLISECONDS);
            }
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_pri_glyph_of_levitate_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_FEATHER_FALL, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_pri_glyph_of_levitate_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_FEATHER_FALL, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_glyph_of_levitate_AuraScript();
    }
};

// 32592 - Mass Dispel
class spell_pri_mass_dispel : public SpellScriptLoader
{
public:
    spell_pri_mass_dispel() : SpellScriptLoader("spell_pri_mass_dispel") { }

    class spell_pri_mass_dispel_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_mass_dispel_SpellScript);

        bool Load() OVERRIDE
        {
            _hasImmunity = false;
            return true;
        }

        void CheckAuras()
        {
            if (Unit* target = GetHitUnit())
                _hasImmunity = target->HasAuraWithMechanic(1 << MECHANIC_IMMUNE_SHIELD);
        }

        void HandleDispel(SpellEffIndex effIndex)
        {
            // Glyph of Mass Dispel: immunities can only be dispelled with the glyph
            if (_hasImmunity && !GetCaster()->HasAura(SPELL_PRIEST_GLYPH_OF_MASS_DISPEL))
                PreventHitEffect(effIndex);
        }

        void Register() OVERRIDE
        {
            BeforeHit += SpellHitFn(spell_pri_mass_dispel_SpellScript::CheckAuras);
            OnEffectHitTarget += SpellEffectFn(spell_pri_mass_dispel_SpellScript::HandleDispel, EFFECT_0, SPELL_EFFECT_DISPEL);
        }

    private:
        bool _hasImmunity;
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_mass_dispel_SpellScript();
    }
};

// 6346 - Fear Ward
class spell_pri_fear_ward : public SpellScriptLoader
{
public:
    spell_pri_fear_ward() : SpellScriptLoader("spell_pri_fear_ward") { }

    class spell_pri_fear_ward_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_fear_ward_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            if (!spellInfo)
                return false;

            uint32 const mechanicMask = (1 << MECHANIC_FEAR) | (1 << MECHANIC_TURN);
            for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                if (spellInfo->Effects[i].ApplyAuraName == SPELL_AURA_MOD_FEAR || spellInfo->Effects[i].ApplyAuraName == SPELL_AURA_MOD_FEAR_2)
                    return (spellInfo->GetAllEffectsMechanicMask() & mechanicMask) != 0;
            return false;
        }

        void HandleProc(ProcEventInfo& /*eventInfo*/)
        {
            Remove();
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_pri_fear_ward_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_pri_fear_ward_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_fear_ward_AuraScript();
    }
};

// 527 - Purify
class spell_pri_glyph_of_purify : public SpellScriptLoader
{
public:
    spell_pri_glyph_of_purify() : SpellScriptLoader("spell_pri_glyph_of_purify") { }

    class spell_pri_glyph_of_purify_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_glyph_of_purify_SpellScript);

        void HandleHit()
        {
            Unit* target = GetHitUnit();
            if (!target || !GetDispelCount() || !GetCaster()->HasAura(SPELL_PRIEST_GLYPH_OF_PURIFY))
                return;

            int32 heal = target->CountPctFromMaxHealth(5);
            // Limited by the priest's health so it can't heal bosses for huge amounts.
            heal = std::min<int32>(heal, GetCaster()->CountPctFromCurHealth(10));

            GetCaster()->CastCustomSpell(SPELL_PRIEST_GLYPH_OF_PURIFY_HEAL, SPELLVALUE_BASE_POINT0, heal, target, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pri_glyph_of_purify_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_glyph_of_purify_SpellScript();
    }
};

// 528 - Dispel Magic
class spell_pri_glyph_of_dispel_magic : public SpellScriptLoader
{
public:
    spell_pri_glyph_of_dispel_magic() : SpellScriptLoader("spell_pri_glyph_of_dispel_magic") { }

    class spell_pri_glyph_of_dispel_magic_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_glyph_of_dispel_magic_SpellScript);

        void HandleHit()
        {
            if (Unit* target = GetHitUnit())
                if (GetDispelCount() && GetCaster()->HasAura(SPELL_PRIEST_GLYPH_OF_DISPEL_MAGIC))
                    GetCaster()->CastSpell(target, SPELL_PRIEST_GLYPH_OF_DISPEL_MAGIC_DAMAGE, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pri_glyph_of_dispel_magic_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_glyph_of_dispel_magic_SpellScript();
    }
};

// 585 - Smite
class spell_pri_glyph_of_smite : public SpellScriptLoader
{
public:
    spell_pri_glyph_of_smite() : SpellScriptLoader("spell_pri_glyph_of_smite") { }

    class spell_pri_glyph_of_smite_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_glyph_of_smite_SpellScript);

        void HandleHit()
        {
            Unit* target = GetHitUnit();
            if (!target)
                return;

            if (AuraEffect const* glyph = GetCaster()->GetAuraEffect(SPELL_PRIEST_GLYPH_OF_SMITE, EFFECT_0))
            {
                // Holy Fire / Power Word: Solace damage over time
                if (target->GetAuraEffect(SPELL_AURA_PERIODIC_DAMAGE, SPELLFAMILY_PRIEST, 0x100000, 0, 0, GetCaster()->GetGUID()))
                {
                    int32 damage = GetHitDamage();
                    AddPct(damage, glyph->GetAmount());
                    SetHitDamage(damage);
                }
            }
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pri_glyph_of_smite_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_glyph_of_smite_SpellScript();
    }
};

// 588 - Inner Fire
class spell_pri_glyph_of_inner_sanctum : public SpellScriptLoader
{
public:
    spell_pri_glyph_of_inner_sanctum() : SpellScriptLoader("spell_pri_glyph_of_inner_sanctum") { }

    class spell_pri_glyph_of_inner_sanctum_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_glyph_of_inner_sanctum_SpellScript);

        void HandleCast()
        {
            if (AuraEffect const* glyph = GetCaster()->GetAuraEffect(SPELL_PRIEST_GLYPH_OF_INNER_SANCTUM, EFFECT_0))
                GetCaster()->CastCustomSpell(SPELL_PRIEST_SPELL_WARDING, SPELLVALUE_BASE_POINT0, glyph->GetAmount(), GetCaster(), true);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_pri_glyph_of_inner_sanctum_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_glyph_of_inner_sanctum_SpellScript();
    }
};

// 64843 - Divine Hymn, 64901 - Hymn of Hope
class spell_pri_glyph_of_inspired_hymns : public SpellScriptLoader
{
public:
    spell_pri_glyph_of_inspired_hymns() : SpellScriptLoader("spell_pri_glyph_of_inspired_hymns") { }

    class spell_pri_glyph_of_inspired_hymns_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_glyph_of_inspired_hymns_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetUnitOwner()->HasAura(SPELL_PRIEST_GLYPH_OF_INSPIRED_HYMNS))
                GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_PRIEST_GLYPH_OF_INSPIRED_HYMNS_VISUAL, true);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetUnitOwner()->RemoveAurasDueToSpell(SPELL_PRIEST_GLYPH_OF_INSPIRED_HYMNS_VISUAL);
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_pri_glyph_of_inspired_hymns_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_pri_glyph_of_inspired_hymns_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_glyph_of_inspired_hymns_AuraScript();
    }
};

// 33076 - Prayer of Mending
class spell_pri_item_s12_2p_heal : public SpellScriptLoader
{
public:
    spell_pri_item_s12_2p_heal() : SpellScriptLoader("spell_pri_item_s12_2p_heal") { }

    class spell_pri_item_s12_2p_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_item_s12_2p_heal_SpellScript);

        void HandleHit()
        {
            if (Unit* target = GetHitUnit())
                if (GetCaster()->HasAura(SPELL_PRIEST_S12_HEALER_2P))
                    GetCaster()->CastSpell(target, SPELL_PRIEST_HOLY_SPARK, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pri_item_s12_2p_heal_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_item_s12_2p_heal_SpellScript();
    }
};

// 131567 - Holy Spark
class spell_pri_holy_spark : public SpellScriptLoader
{
public:
    spell_pri_holy_spark() : SpellScriptLoader("spell_pri_holy_spark") { }

    class spell_pri_holy_spark_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_holy_spark_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            Unit* actor = eventInfo.GetActor();
            return actor && actor->GetGUID() == GetCasterGUID();
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_pri_holy_spark_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_holy_spark_AuraScript();
    }
};

// 47585 - Dispersion
class spell_pri_item_s12_2p_shadow : public SpellScriptLoader
{
public:
    spell_pri_item_s12_2p_shadow() : SpellScriptLoader("spell_pri_item_s12_2p_shadow") { }

    class spell_pri_item_s12_2p_shadow_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_item_s12_2p_shadow_SpellScript);

        void HandleHit()
        {
            if (GetCaster()->HasAura(SPELL_PRIEST_S12_SHADOW_2P))
                GetCaster()->CastSpell(GetCaster(), SPELL_PRIEST_DISPERSION_SPRINT, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pri_item_s12_2p_shadow_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_item_s12_2p_shadow_SpellScript();
    }
};

// 138301 - Item - Priest T15 Healer 4P Bonus
class spell_pri_t15_healer_4p_bonus : public SpellScriptLoader
{
public:
    spell_pri_t15_healer_4p_bonus() : SpellScriptLoader("spell_pri_t15_healer_4p_bonus") { }

    class spell_pri_t15_healer_4p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_t15_healer_4p_bonus_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            if (!spellInfo)
                return false;

            return spellInfo->Id == SPELL_PRIEST_PENANCE_HEAL ||
                spellInfo->Id == SPELL_PRIEST_PENANCE_DAMAGE ||
                spellInfo->Id == SPELL_PRIEST_CIRCLE_OF_HEALING;
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_pri_t15_healer_4p_bonus_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_t15_healer_4p_bonus_AuraScript();
    }
};

// 145306 - Item - Priest T16 Healer 2P Bonus
class spell_pri_t16_healer_2p_bonus : public SpellScriptLoader
{
public:
    spell_pri_t16_healer_2p_bonus() : SpellScriptLoader("spell_pri_t16_healer_2p_bonus") { }

    class spell_pri_t16_healer_2p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_t16_healer_2p_bonus_AuraScript);

        void HandleProc(ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            if (eventInfo.GetSpellInfo() && eventInfo.GetSpellInfo()->Id == SPELL_PRIEST_ARCHANGEL)
                GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_PRIEST_PIOUS_HEALER, true);
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_pri_t16_healer_2p_bonus_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_t16_healer_2p_bonus_AuraScript();
    }
};

// 145334 - Item - Priest T16 Healer 4P Bonus
class spell_pri_t16_healer_4p_bonus : public SpellScriptLoader
{
public:
    spell_pri_t16_healer_4p_bonus() : SpellScriptLoader("spell_pri_t16_healer_4p_bonus") { }

    class spell_pri_t16_healer_4p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_t16_healer_4p_bonus_AuraScript);

        bool Load() OVERRIDE
        {
            return GetOwner()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void HandleProc(ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            if (!spellInfo)
                return;

            Unit* priest = GetUnitOwner();
            switch (spellInfo->Id)
            {
                case SPELL_PRIEST_CIRCLE_OF_HEALING:
                case SPELL_PRIEST_PRAYER_OF_MENDING:
                case SPELL_PRIEST_PRAYER_OF_MENDING_DIVINE_INSIGHT:
                    if (IsPriestInSpec(priest, SPEC_PRIEST_HOLY))
                        priest->CastSpell(priest, SPELL_PRIEST_ABSOLUTION, true);
                    break;
                case SPELL_PRIEST_SPIRIT_SHELL:
                    priest->CastSpell(priest, SPELL_PRIEST_RESOLUTE_SPIRIT, true);
                    break;
                default:
                    break;
            }
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_pri_t16_healer_4p_bonus_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_t16_healer_4p_bonus_AuraScript();
    }
};

// 145179 - Item - Priest T16 Shadow 4P Bonus
class spell_pri_t16_shadow_4p_bonus : public SpellScriptLoader
{
public:
    spell_pri_t16_shadow_4p_bonus() : SpellScriptLoader("spell_pri_t16_shadow_4p_bonus") { }

    class spell_pri_t16_shadow_4p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_t16_shadow_4p_bonus_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            return spellInfo && SpellCostsPower(spellInfo->Id, POWER_SHADOW_ORBS);
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();

            Unit* priest = GetUnitOwner();
            int32 orbs = priest->GetPower(POWER_SHADOW_ORBS) + 1;
            priest->CastCustomSpell(GetSpellInfo()->Effects[aurEff->GetEffIndex()].TriggerSpell, SPELLVALUE_BASE_POINT0, orbs * aurEff->GetAmount(), priest, true);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_pri_t16_shadow_4p_bonus_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_pri_t16_shadow_4p_bonus_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_t16_shadow_4p_bonus_AuraScript();
    }
};

// 8092 - Mind Blast
class spell_pri_mind_blast : public SpellScriptLoader
{
public:
    spell_pri_mind_blast() : SpellScriptLoader("spell_pri_mind_blast") { }

    class spell_pri_mind_blast_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_mind_blast_SpellScript);

        void HandleAfterCast()
        {
            Player* player = GetCaster()->ToPlayer();
            if (!player)
                return;

            player->RemoveAurasDueToSpell(SPELL_PRIEST_GLYPH_OF_MIND_SPIKE_PROC);

            if (!player->HasAura(SPELL_PRIEST_DIVINE_INSIGHT_SHADOW))
                return;

            // An instant Mind Blast used Divine Insight; one that was being cast when it procced keeps it
            if (!GetSpell()->GetCastTime())
                player->RemoveAurasDueToSpell(SPELL_PRIEST_DIVINE_INSIGHT_SHADOW);
            else
                player->RemoveSpellCooldown(SPELL_PRIEST_MIND_BLAST, true);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_pri_mind_blast_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_mind_blast_SpellScript();
    }
};

// 33371 - Glyph of Mind Spike
class spell_pri_glyph_of_mind_spike : public SpellScriptLoader
{
public:
    spell_pri_glyph_of_mind_spike() : SpellScriptLoader("spell_pri_glyph_of_mind_spike") { }

    class spell_pri_glyph_of_mind_spike_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_glyph_of_mind_spike_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            Spell const* spell = eventInfo.TriggeredBySpell();
            return spell && spell->GetCastTime() > 0;
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_pri_glyph_of_mind_spike_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_glyph_of_mind_spike_AuraScript();
    }
};

// 8122 - Psychic Scream
class spell_pri_psychic_scream : public SpellScriptLoader
{
public:
    spell_pri_psychic_scream() : SpellScriptLoader("spell_pri_psychic_scream") { }

    class spell_pri_psychic_scream_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_psychic_scream_AuraScript);

        // Glyph of Psychic Scream turns the fear into a root
        void HandleInitEffects(uint32& effectMask)
        {
            Unit* caster = GetCaster();
            if (caster && caster->HasAura(SPELL_PRIEST_GLYPH_OF_PSYCHIC_SCREAM))
                effectMask &= ~(1 << EFFECT_0);
            else
                effectMask &= ~(1 << EFFECT_1);
        }

        void Register() OVERRIDE
        {
            OnInitEffects += AuraInitEffectsFn(spell_pri_psychic_scream_AuraScript::HandleInitEffects);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_psychic_scream_AuraScript();
    }
};

// 113792 - Psychic Terror
class spell_pri_psychic_terror : public SpellScriptLoader
{
public:
    spell_pri_psychic_terror() : SpellScriptLoader("spell_pri_psychic_terror") { }

    class spell_pri_psychic_terror_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_psychic_terror_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            Unit* psyfiend = GetCaster();
            uint64 targetGuid = psyfiend->IsAIEnabled ? psyfiend->GetAI()->GetGUID(PSYFIEND_DATA_TERROR_TARGET) : 0;
            targets.remove_if([targetGuid](WorldObject const* target) { return target->GetGUID() != targetGuid; });
        }

        void HandleHit()
        {
            Unit* psyfiend = GetCaster();
            if (psyfiend->IsAIEnabled)
                psyfiend->GetAI()->SetGUID(GetHitUnit()->GetGUID(), PSYFIEND_DATA_TERRIFIED);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_psychic_terror_SpellScript::FilterTargets, EFFECT_ALL, TARGET_UNIT_SRC_AREA_ENEMY);
            OnHit += SpellHitFn(spell_pri_psychic_terror_SpellScript::HandleHit);
        }
    };

    class spell_pri_psychic_terror_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_psychic_terror_AuraScript);

        void HandleInitEffects(uint32& effectMask)
        {
            if (Unit* psyfiend = GetCaster())
                if (Unit* priest = psyfiend->GetOwner())
                    if (!priest->HasAura(SPELL_PRIEST_GLYPH_OF_PSYCHIC_SCREAM))
                        effectMask &= ~(1 << EFFECT_1);
        }

        void Register() OVERRIDE
        {
            OnInitEffects += AuraInitEffectsFn(spell_pri_psychic_terror_AuraScript::HandleInitEffects);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_psychic_terror_SpellScript();
    }

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_psychic_terror_AuraScript();
    }
};

// 108968, 142723 - Void Shift
class spell_pri_void_shift : public SpellScriptLoader
{
public:
    spell_pri_void_shift() : SpellScriptLoader("spell_pri_void_shift") { }

    class spell_pri_void_shift_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_void_shift_SpellScript);

        SpellCastResult CheckTarget()
        {
            if (GetExplTargetUnit() && GetExplTargetUnit()->GetTypeId() != TypeID::TYPEID_PLAYER)
                return SpellCastResult::SPELL_FAILED_BAD_TARGETS;
            return SpellCastResult::SPELL_CAST_OK;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* priest = GetCaster();
            Unit* target = GetHitUnit();
            if (!target || target == priest)
                return;

            float priestPct = priest->GetHealthPct();
            float targetPct = target->GetHealthPct();
            float& lowest = priestPct > targetPct ? targetPct : priestPct;
            lowest = std::max(lowest, 25.0f);
            std::swap(priestPct, targetPct);

            Unit* units[2] = { priest, target };
            for (Unit* unit : units)
            {
                uint32 health = uint32(unit->GetMaxHealth() / 100.0f * (unit == priest ? priestPct : targetPct));
                SpellValueMod valueMod = unit->GetHealth() > health ? SPELLVALUE_BASE_POINT0 : SPELLVALUE_BASE_POINT1;
                int32 value = std::abs(int32(unit->GetHealth()) - int32(health));
                priest->CastCustomSpell(SPELL_PRIEST_VOID_SHIFT_EFFECT, valueMod, value, unit, true);
            }
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_pri_void_shift_SpellScript::CheckTarget);
            OnEffectHitTarget += SpellEffectFn(spell_pri_void_shift_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_void_shift_SpellScript();
    }
};

// 112833 - Spectral Guise
class spell_pri_spectral_guise : public SpellScriptLoader
{
public:
    spell_pri_spectral_guise() : SpellScriptLoader("spell_pri_spectral_guise") { }

    class spell_pri_spectral_guise_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_spectral_guise_SpellScript);

        void HandleAfterCast()
        {
            Unit* priest = GetCaster();
            std::list<TempSummon*> summons;
            priest->GetSummons(summons, NPC_PRIEST_SPECTRAL_GUISE);
            for (TempSummon* summon : summons)
            {
                summon->CastSpell(summon, SPELL_PRIEST_SPECTRAL_GUISE_CHARGES, true);
                summon->CastSpell(summon, SPELL_PRIEST_INHERIT_MASTERS_THREAT, true);
                summon->CastSpell(summon, SPELL_PRIEST_SPECTRAL_GUISE_STEALTH, true);
                priest->CastSpell(summon, SPELL_PRIEST_CLONE_ME, true);
                priest->CastSpell(summon, SPELL_PRIEST_COPY_WEAPON, true);

                if (priest->GetShapeshiftForm() == FORM_SHADOW)
                    summon->CastSpell(summon, SPELL_PRIEST_SHADOWFORM_VISUAL_WITHOUT_GLYPH, true);
            }

            InstanceScript* instance = priest->GetInstanceScript();
            if (!instance || !instance->IsEncounterInProgress())
                priest->CombatStop();
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_pri_spectral_guise_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_spectral_guise_SpellScript();
    }
};

// 119032 - Spectral Guise (stealth)
class spell_pri_spectral_guise_remove : public SpellScriptLoader
{
public:
    spell_pri_spectral_guise_remove() : SpellScriptLoader("spell_pri_spectral_guise_remove") { }

    class spell_pri_spectral_guise_remove_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_spectral_guise_remove_AuraScript);

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            std::list<TempSummon*> summons;
            GetUnitOwner()->GetSummons(summons, NPC_PRIEST_SPECTRAL_GUISE);
            for (TempSummon* summon : summons)
                summon->DespawnOrUnsummon(1);
        }

        void Register() OVERRIDE
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_pri_spectral_guise_remove_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_MOD_STEALTH, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_spectral_guise_remove_AuraScript();
    }
};

// 119030 - Spectral Guise (charges)
class spell_pri_spectral_guise_stacks : public SpellScriptLoader
{
public:
    spell_pri_spectral_guise_stacks() : SpellScriptLoader("spell_pri_spectral_guise_stacks") { }

    class spell_pri_spectral_guise_stacks_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_spectral_guise_stacks_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            return !(eventInfo.GetHitMask() & PROC_EX_INTERNAL_DOT);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* priest = GetUnitOwner()->GetOwner())
                priest->RemoveAurasDueToSpell(SPELL_PRIEST_SPECTRAL_GUISE_STEALTH);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_pri_spectral_guise_stacks_AuraScript::CheckProc);
            OnEffectRemove += AuraEffectRemoveFn(spell_pri_spectral_guise_stacks_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_spectral_guise_stacks_AuraScript();
    }
};

// 1489 - Power Word: Barrier
class sat_pri_power_word_barrier : public IAreaTriggerAura
{
    bool CheckTriggering(WorldObject* triggering) OVERRIDE
    {
        Unit* unit = triggering->ToUnit();
        return unit && unit->IsAlive() && unit->IsFriendlyTo(m_caster);
    }

    void OnTriggeringApply(WorldObject* triggering) OVERRIDE
    {
        Unit* unit = triggering->ToUnit();
        unit->CastSpell(unit, SPELL_PRIEST_POWER_WORD_BARRIER_EFFECT, true, NULL, NULL, m_caster->GetGUID());
    }

    void OnTriggeringRemove(WorldObject* triggering) OVERRIDE
    {
        if (Unit* unit = triggering->ToUnit())
            unit->RemoveAurasDueToSpell(SPELL_PRIEST_POWER_WORD_BARRIER_EFFECT, m_caster->GetGUID());
    }
};

// 121536 - Angelic Feather
class spell_pri_angelic_feather : public SpellScriptLoader
{
public:
    spell_pri_angelic_feather() : SpellScriptLoader("spell_pri_angelic_feather") { }

    class spell_pri_angelic_feather_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_angelic_feather_SpellScript);

        // At most three feathers may be placed
        void HandleCast()
        {
            std::list<AreaTrigger*> feathers;
            GetCaster()->GetAreaTriggerList(feathers, GetSpellInfo()->Id);
            while (feathers.size() > 2)
            {
                feathers.front()->Remove();
                feathers.pop_front();
            }
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_pri_angelic_feather_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_angelic_feather_SpellScript();
    }
};

// 337 - Angelic Feather
class sat_pri_angelic_feather : public IAreaTriggerOnce
{
    bool CheckTriggering(WorldObject* triggering) OVERRIDE
    {
        Player* player = triggering->ToPlayer();
        return player && player->IsAlive() && m_caster->IsValidAssistTarget(player);
    }

    void OnTrigger(WorldObject* triggering) OVERRIDE
    {
        Unit* unit = triggering->ToUnit();
        unit->CastSpell(unit, SPELL_PRIEST_ANGELIC_FEATHER_SPEED, true);
    }
};

// 120517 - Halo (Holy), 120644 - Halo (Shadow)
class spell_pri_halo : public SpellScriptLoader
{
public:
    spell_pri_halo() : SpellScriptLoader("spell_pri_halo") { }

    class spell_pri_halo_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_halo_SpellScript);

        void HandleCast()
        {
            uint32 visual = GetSpellInfo()->Id == SPELL_PRIEST_HALO_HOLY ? SPELL_PRIEST_HALO_VISUAL_HOLY : SPELL_PRIEST_HALO_VISUAL_SHADOW;
            GetCaster()->CastSpell(GetCaster(), visual, true);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_pri_halo_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_halo_SpellScript();
    }
};

// 120630, 120643 - Halo (visual)
class spell_pri_halo_visual : public SpellScriptLoader
{
public:
    spell_pri_halo_visual() : SpellScriptLoader("spell_pri_halo_visual") { }

    class spell_pri_halo_visual_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_halo_visual_SpellScript);

        void SelectDest(SpellDestination& dest)
        {
            Position const offset = { 35.0f, 0.0f, 0.0f, 0.0f };
            dest._position.RelocateOffset(offset);
        }

        void Register() OVERRIDE
        {
            OnDestinationTargetSelect += SpellDestinationTargetSelectFn(spell_pri_halo_visual_SpellScript::SelectDest, EFFECT_0, TARGET_DEST_CASTER);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_halo_visual_SpellScript();
    }
};

// 120692 - Halo (Holy), 120696 - Halo (Shadow)
class spell_pri_halo_effect : public SpellScriptLoader
{
public:
    spell_pri_halo_effect() : SpellScriptLoader("spell_pri_halo_effect") { }

    class spell_pri_halo_effect_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_halo_effect_SpellScript);

        void PreventHeal(SpellEffIndex effIndex)
        {
            if (GetCaster()->_IsValidAttackTarget(GetHitUnit(), GetSpellInfo()))
                PreventHitDefaultEffect(effIndex);
        }

        void PreventDamage(SpellEffIndex effIndex)
        {
            if (!GetCaster()->_IsValidAttackTarget(GetHitUnit(), GetSpellInfo()))
                PreventHitDefaultEffect(effIndex);
        }

        // Full effect at 25 yards from the center, less when closer
        void HandleHit()
        {
            std::list<AreaTrigger*> halos;
            GetCaster()->GetAreaTriggerList(halos, GetSpellInfo()->Id == SPELL_PRIEST_HALO_EFFECT_HOLY ? SPELL_PRIEST_HALO_HOLY : SPELL_PRIEST_HALO_SHADOW);
            if (halos.empty())
                return;

            float dist = std::max(halos.back()->GetExactDist2d(GetHitUnit()), 1.5f);
            float coeff = std::min(dist / 25.0f, 1.0f);
            if (int32 damage = GetHitDamage())
                SetHitDamage(int32(damage * coeff));
            if (int32 heal = GetHitHeal())
                SetHitHeal(int32(heal * coeff));
        }

        void Register() OVERRIDE
        {
            OnEffectLaunchTarget += SpellEffectFn(spell_pri_halo_effect_SpellScript::PreventHeal, EFFECT_0, SPELL_EFFECT_HEAL);
            OnEffectLaunchTarget += SpellEffectFn(spell_pri_halo_effect_SpellScript::PreventDamage, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
            OnHit += SpellHitFn(spell_pri_halo_effect_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_halo_effect_SpellScript();
    }
};

// 657, 658 - Halo
class sat_pri_halo : public IAreaTriggerAura
{
    std::set<uint64> m_targets;

    bool CheckTriggering(WorldObject* triggering) OVERRIDE
    {
        Unit* unit = triggering->ToUnit();
        if (!unit || !unit->IsAlive())
            return false;

        if (m_targets.find(unit->GetGUID()) != m_targets.end())
            return false;

        // The ring travels outwards at 10 yards per second
        float dist = std::min((GetAreaTrigger()->GetMaxDuration() - GetAreaTrigger()->GetDuration()) * 10.0f / 1000.0f, 30.0f);
        if (m_target->GetExactDist2d(unit) - unit->GetObjectSize() > dist)
            return false;

        if (!m_caster->CanSeeOrDetect(unit))
            return false;

        if (!unit->IsWithinLOS(m_target->GetPositionX(), m_target->GetPositionY(), m_target->GetPositionZ() + 5.0f))
            return false;

        return m_caster->IsValidAssistTarget(unit) || m_caster->_IsValidAttackTarget(unit, sSpellMgr->GetSpellInfo(SPELL_PRIEST_HALO_EFFECT_HOLY));
    }

    void OnTriggeringApply(WorldObject* triggering) OVERRIDE
    {
        m_targets.insert(triggering->GetGUID());
        uint32 spellId = GetAreaTrigger()->GetSpellId() == SPELL_PRIEST_HALO_HOLY ? SPELL_PRIEST_HALO_EFFECT_HOLY : SPELL_PRIEST_HALO_EFFECT_SHADOW;
        m_caster->CastSpell(triggering->ToUnit(), spellId, true);
    }
};

// 121135, 127632 - Cascade
class spell_pri_cascade : public SpellScriptLoader
{
public:
    spell_pri_cascade() : SpellScriptLoader("spell_pri_cascade") { }

    class spell_pri_cascade_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_cascade_SpellScript);

        SpellCastResult CheckCast()
        {
            if (Unit* target = GetExplTargetUnit())
                if (!GetCaster()->IsValidAssistTarget(target) && !GetCaster()->_IsValidAttackTarget(target, sSpellMgr->GetSpellInfo(SPELL_PRIEST_CASCADE_HOLY_DAMAGE)))
                    return SpellCastResult::SPELL_FAILED_BAD_TARGETS;
            return SpellCastResult::SPELL_CAST_OK;
        }

        void HandleHit()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            bool positive = caster->IsValidAssistTarget(target);
            bool holy = GetSpellInfo()->Id == SPELL_PRIEST_CASCADE_HOLY;

            uint32 spellId;
            if (holy)
                spellId = positive ? SPELL_PRIEST_CASCADE_HOLY_MISSILE : SPELL_PRIEST_CASCADE_HOLY_DAMAGE;
            else
                spellId = positive ? SPELL_PRIEST_CASCADE_SHADOW_MISSILE : SPELL_PRIEST_CASCADE_SHADOW_DAMAGE;

            caster->CastSpell(target, holy ? SPELL_PRIEST_CASCADE_HOLY_MARKER : SPELL_PRIEST_CASCADE_SHADOW_MARKER, true);
            // Base point 1 holds the number of remaining bounces
            caster->CastCustomSpell(spellId, SPELLVALUE_BASE_POINT1, 4, target, true);
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_pri_cascade_SpellScript::CheckCast);
            OnHit += SpellHitFn(spell_pri_cascade_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_cascade_SpellScript();
    }
};

// 121146, 127627, 120785, 127628 - Cascade (bolt)
class spell_pri_cascade_missile : public SpellScriptLoader
{
public:
    spell_pri_cascade_missile() : SpellScriptLoader("spell_pri_cascade_missile") { }

    class spell_pri_cascade_missile_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_cascade_missile_SpellScript);

        void HandleHit()
        {
            Unit* priest = GetOriginalCaster();
            Unit* target = GetHitUnit();
            if (!priest || !target)
                return;

            uint32 spellId = GetSpellInfo()->Id;
            bool holy = spellId == SPELL_PRIEST_CASCADE_HOLY_MISSILE || spellId == SPELL_PRIEST_CASCADE_HOLY_DAMAGE;
            if (spellId == SPELL_PRIEST_CASCADE_HOLY_MISSILE || spellId == SPELL_PRIEST_CASCADE_SHADOW_MISSILE)
                target->CastSpell(target, holy ? SPELL_PRIEST_CASCADE_HOLY_HEAL : SPELL_PRIEST_CASCADE_SHADOW_HEAL, true, NULL, NULL, priest->GetGUID());

            Aura const* aura = GetHitAura();
            AuraEffect const* bounces = aura ? aura->GetEffect(EFFECT_1) : NULL;
            if (!bounces || !bounces->GetAmount())
                return;

            // Each bolt splits into two
            uint32 selector = holy ? SPELL_PRIEST_CASCADE_HOLY_SELECTOR : SPELL_PRIEST_CASCADE_SHADOW_SELECTOR;
            for (uint8 i = 0; i < 2; ++i)
                target->CastCustomSpell(selector, SPELLVALUE_BASE_POINT1, bounces->GetAmount(), target, true, NULL, bounces, priest->GetGUID());
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pri_cascade_missile_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_cascade_missile_SpellScript();
    }
};

// 120786, 127630 - Cascade (target selector)
class spell_pri_cascade_selector : public SpellScriptLoader
{
public:
    spell_pri_cascade_selector() : SpellScriptLoader("spell_pri_cascade_selector") { }

    class spell_pri_cascade_selector_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_cascade_selector_SpellScript);

        int32 _bounces = 0;

        static uint32 GetMarker(uint32 triggerId)
        {
            return triggerId == SPELL_PRIEST_CASCADE_HOLY_MISSILE || triggerId == SPELL_PRIEST_CASCADE_HOLY_DAMAGE ?
                SPELL_PRIEST_CASCADE_HOLY_MARKER : SPELL_PRIEST_CASCADE_SHADOW_MARKER;
        }

        void SelectTarget(std::list<WorldObject*>& targets)
        {
            Unit* priest = GetOriginalCaster();
            SpellInfo const* trigger = GetTriggeringSpell();
            _bounces = GetSpellValue()->EffectBasePoints[EFFECT_1];
            if (!_bounces || !trigger || !priest)
            {
                targets.clear();
                return;
            }
            --_bounces;

            Unit* caster = GetCaster();
            bool heal = trigger->Id == SPELL_PRIEST_CASCADE_HOLY_MISSILE || trigger->Id == SPELL_PRIEST_CASCADE_SHADOW_MISSILE;
            uint32 marker = GetMarker(trigger->Id);
            SpellInfo const* healInfo = sSpellMgr->GetSpellInfo(SPELL_PRIEST_CASCADE_HOLY_HEAL);
            SpellInfo const* damageInfo = sSpellMgr->GetSpellInfo(SPELL_PRIEST_CASCADE_HOLY_DAMAGE);
            targets.remove_if([=](WorldObject const* target)
            {
                Unit const* unit = target->ToUnit();
                if (!unit)
                    return true;

                if (unit->HasAura(marker, priest->GetGUID()))
                    return true;

                if (TempSummon const* summon = unit->ToTempSummon())
                    if (!summon->IsGuardian() || summon->IsMinorGuardian())
                        return true;

                if (!caster->IsWithinLOSInMap(unit))
                    return true;

                return heal ? !priest->_IsValidAssistTarget(unit, healInfo) : !priest->_IsValidAttackTarget(unit, damageInfo);
            });

            // Bolts prefer the farthest valid target
            if (!targets.empty())
            {
                targets.sort(Skyfire::ObjectDistanceOrderPred(caster, false));
                targets.resize(1);
            }
        }

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            Unit* priest = GetOriginalCaster();
            SpellInfo const* trigger = GetTriggeringSpell();
            if (!priest || !trigger)
                return;

            priest->CastSpell(GetHitUnit(), GetMarker(trigger->Id), true);
            GetCaster()->CastCustomSpell(trigger->Id, SPELLVALUE_BASE_POINT1, _bounces, GetHitUnit(), true, NULL, NULL, priest->GetGUID());
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_cascade_selector_SpellScript::SelectTarget, EFFECT_1, TARGET_UNIT_DEST_AREA_ENTRY);
            OnEffectHitTarget += SpellEffectFn(spell_pri_cascade_selector_SpellScript::HandleHit, EFFECT_1, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_cascade_selector_SpellScript();
    }
};

// 110744, 122121 - Divine Star
class spell_pri_divine_star : public SpellScriptLoader
{
public:
    spell_pri_divine_star() : SpellScriptLoader("spell_pri_divine_star") { }

    class spell_pri_divine_star_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_divine_star_AuraScript);

        MissileData m_data;

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            uint32 spellId = GetSpellInfo()->Effects[EFFECT_1].TriggerSpell;
            BasicEvent* evt = GetUnitOwner()->m_Events.FindEvent([spellId](BasicEvent const* e)
            {
                SpellEvent const* se = dynamic_cast<SpellEvent const*>(e);
                return se && se->GetSpell()->GetSpellInfo()->Id == spellId;
            });
            if (evt)
                m_data.Initialize(static_cast<SpellEvent const*>(evt)->GetSpell(), 24.0f);
        }

        void HandleTick(AuraEffect const* aurEff)
        {
            m_data.Update(getMSTime(), GetUnitOwner(), aurEff);
        }

    public:
        MissileData* GetData() { return m_data.Initialized() ? &m_data : NULL; }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_pri_divine_star_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_pri_divine_star_AuraScript::HandleTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_divine_star_AuraScript();
    }
};

// 58880, 122127 - Divine Star (missile)
class spell_pri_divine_star_dummy : public SpellScriptLoader
{
public:
    spell_pri_divine_star_dummy() : SpellScriptLoader("spell_pri_divine_star_dummy") { }

    class spell_pri_divine_star_dummy_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_divine_star_dummy_SpellScript);

        void SelectDest(SpellDestination& dest)
        {
            Unit* caster = GetCaster();
            bool inAir = false;
            if (caster->HasUnitMovementFlag(MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_DISABLE_GRAVITY | MOVEMENTFLAG_FALLING))
            {
                float z = caster->GetMap()->GetHeight(caster->GetPhaseMask(), caster->GetPositionX(), caster->GetPositionY(), caster->GetPositionZ(), true);
                if (std::abs(z - caster->GetPositionZ()) > 10.0f)
                    inAir = true;
            }
            dest._position.WorldRelocate(*caster);
            caster->MovePositionToFirstCollosionBySteps(dest._position, 24.0f, 0.0f, 2.0f, inAir);
            dest._position.RelocateOffset(0.0f, 6.0f);
        }

        void Register() OVERRIDE
        {
            OnDestinationTargetSelect += SpellDestinationTargetSelectFn(spell_pri_divine_star_dummy_SpellScript::SelectDest, EFFECT_0, TARGET_UNK_138);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_divine_star_dummy_SpellScript();
    }
};

// 110745, 122128 - Divine Star (damage and heal)
class spell_pri_divine_star_damage_heal : public SpellScriptLoader
{
public:
    spell_pri_divine_star_damage_heal() : SpellScriptLoader("spell_pri_divine_star_damage_heal") { }

    class spell_pri_divine_star_damage_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_divine_star_damage_heal_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            MissileData* data = NULL;
            uint32 auraId = GetSpellInfo()->Id == SPELL_PRIEST_DIVINE_STAR_SHADOW_DAMAGE ? SPELL_PRIEST_DIVINE_STAR_SHADOW : SPELL_PRIEST_DIVINE_STAR_HOLY;
            if (Aura* trigger = GetCaster()->GetAura(auraId))
                if (spell_pri_divine_star::spell_pri_divine_star_AuraScript* script = dynamic_cast<spell_pri_divine_star::spell_pri_divine_star_AuraScript*>(trigger->GetScriptByName("spell_pri_divine_star")))
                    data = script->GetData();

            if (!data)
            {
                targets.clear();
                return;
            }

            targets.remove_if([data](WorldObject* target)
            {
                return !data->CheckTarget(target, 4.0f);
            });
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_divine_star_damage_heal_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_divine_star_damage_heal_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ALLY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_divine_star_damage_heal_SpellScript();
    }
};

void AddSC_priest_spell_scripts()
{
    new spell_pri_prayer_of_healing();
    new spell_pri_greater_heal();
    new spell_pri_flash_heal();

    new spell_pri_evangelism();
    new spell_pri_divine_aegis();
    new spell_pri_item_greater_heal_refund();
    new spell_pri_leap_of_faith();
    new spell_pri_lightwell_renew();
    new spell_pri_lightwell_charges();
    new spell_pri_mana_leech();
    new spell_pri_mind_sear();
    new spell_pri_penance();
    new spell_pri_power_word_shield();
    new spell_pri_prayer_of_mending_heal();
    new spell_pri_renew();
    new spell_pri_shadow_word_death();
    new spell_pri_shadow_word_death_glyphed();
    new spell_pri_shadowform();
    new spell_pri_glyph_of_shadow();
    new spell_pri_vampiric_embrace();
    new spell_pri_vampiric_embrace_target();
    new spell_pri_vampiric_touch();

    // Shadow
    new spell_pri_shadow_orbs();
    new spell_pri_shadowy_apparitions("spell_pri_shadowy_apparitions");
    new spell_pri_shadowy_apparitions("spell_pri_t15_shadow_4p_bonus");
    new spell_pri_shadowy_apparition();
    new spell_pri_t15_shadow_2p_bonus();
    new spell_pri_devouring_plague();
    new spell_pri_devouring_plague_shadowy_recall();
    new spell_pri_shadowy_recall();
    new spell_pri_mind_flay_selector();
    new spell_pri_mind_flay();
    new spell_pri_mind_flay_insanity();
    new spell_pri_mind_flay_shadowy_recall();
    new spell_pri_mind_spike();
    new spell_pri_mind_blast();
    new spell_pri_glyph_of_mind_spike();
    new spell_pri_psychic_scream();
    new spell_pri_psychic_terror();
    new spell_pri_mind_sear_check();
    new spell_pri_psychic_horror();
    new spell_pri_divine_insight_talent();
    new spell_pri_divine_insight_shadow();
    new spell_pri_divine_insight_holy();
    new spell_pri_from_darkness_comes_light();
    new spell_pri_surge_of_light_consumer();
    new spell_pri_surge_of();
    new spell_pri_twist_of_fate();
    new spell_pri_shadowfiend();
    new spell_pri_void_tendrils();
    new spell_pri_void_tendrils_grasp();

    // Talents
    new spell_pri_void_shift();
    new spell_pri_spectral_guise();
    new spell_pri_spectral_guise_remove();
    new spell_pri_spectral_guise_stacks();
    new spell_pri_angelic_feather();
    new atrigger_script<sat_pri_angelic_feather>("sat_pri_angelic_feather");
    new spell_pri_halo();
    new spell_pri_halo_visual();
    new spell_pri_halo_effect();
    new atrigger_script<sat_pri_halo>("sat_pri_halo");
    new spell_pri_cascade();
    new spell_pri_cascade_missile();
    new spell_pri_cascade_selector();
    new spell_pri_divine_star();
    new spell_pri_divine_star_dummy();
    new spell_pri_divine_star_damage_heal();

    // Discipline
    new spell_pri_atonement();
    new spell_pri_atonement_heal();
    new spell_pri_spirit_shell();
    new spell_pri_archangel();
    new spell_pri_strength_of_soul();
    new spell_pri_power_word_solace();
    new spell_pri_solace_and_insanity();
    new spell_pri_train_of_thought();
    new spell_pri_guardian_spirit();
    new spell_pri_angelic_bulwark();
    new spell_pri_focused_will();
    new spell_pri_phantasm_proc();
    new atrigger_script<sat_pri_power_word_barrier>("sat_pri_power_word_barrier");

    // Holy
    new spell_pri_echo_of_light();
    new spell_pri_holy_word_sanctuary();
    new spell_pri_holy_word_sanctuary_heal();
    new spell_pri_absolution();
    new spell_pri_chakra_serenity();
    new spell_pri_chakra_chastise();
    new spell_pri_binding_heal();
    new spell_pri_prayer_of_mending_proc();

    // Glyphs and utility
    new spell_pri_levitate();
    new spell_pri_glyph_of_heavens();
    new spell_pri_glyph_of_levitate();
    new spell_pri_mass_dispel();
    new spell_pri_fear_ward();
    new spell_pri_glyph_of_purify();
    new spell_pri_glyph_of_dispel_magic();
    new spell_pri_glyph_of_smite();
    new spell_pri_glyph_of_inner_sanctum();
    new spell_pri_glyph_of_inspired_hymns();

    // Set bonuses
    new spell_pri_item_s12_2p_heal();
    new spell_pri_holy_spark();
    new spell_pri_item_s12_2p_shadow();
    new spell_pri_t15_healer_4p_bonus();
    new spell_pri_t16_healer_2p_bonus();
    new spell_pri_t16_healer_4p_bonus();
    new spell_pri_t16_shadow_4p_bonus();
}
