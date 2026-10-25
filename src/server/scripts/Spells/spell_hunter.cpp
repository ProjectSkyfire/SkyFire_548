/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/*
 * Scripts for spells with SPELLFAMILY_HUNTER, SPELLFAMILY_PET and SPELLFAMILY_GENERIC spells used by hunter players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_hun_".
 */

#include "Pet.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "AreaTrigger.h"
#include "Battleground.h"
#include "Cell.h"
#include "CellImpl.h"
#include "Containers.h"
#include "DBCStores.h"
#include "DynamicObject.h"
#include "EventProcessor.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "ObjectAccessor.h"
#include "Spell.h"
#include "SpellScript.h"
#include "SpellAuras.h"
#include "SpellAuraEffects.h"
#include "PetDefines.h"

#include <algorithm>
#include <set>

enum HunterSpells
{
    SPELL_HUNTER_A_MURDER_OF_CROWS                  = 131894,
    SPELL_HUNTER_CROW_TRIGGER                       = 131951,
    SPELL_HUNTER_DELAYED_CROW                       = 131637,
    SPELL_HUNTER_DELAYED_CROW2                      = 131952,
    SPELL_HUNTER_CROW_DMG                           = 131900,

    SPELL_HUNTER_BESTIAL_WRATH                      = 19574,
    SPELL_HUNTER_CHIMERA_SHOT_HEAL                  = 53353,
    SPELL_HUNTER_DIRE_BEAST_SUMMON                  = 132764,
    SPELL_HUNTER_FIRE                               = 82926,
    SPELL_HUNTER_GENERIC_ENERGIZE_FOCUS             = 91954,

    SPELL_HUNTER_IMPROVED_SERPENT_STING             = 82834,
    SPELL_HUNTER_IMPROVED_SERPENT_STING_DAMAGE      = 83077,
    SPELL_HUNTER_INVIGORATION_TRIGGERED             = 53398,
    SPELL_HUNTER_LOCK_AND_LOAD                      = 56453,
    SPELL_HUNTER_MASTERS_CALL_TRIGGERED             = 62305,
    SPELL_HUNTER_MISDIRECTION_PROC                  = 35079,
    SPELL_HUNTER_PET_LAST_STAND_TRIGGERED           = 53479,
    SPELL_HUNTER_PET_HEART_OF_THE_PHOENIX           = 55709,
    SPELL_HUNTER_PET_HEART_OF_THE_PHOENIX_TRIGGERED = 54114,
    SPELL_HUNTER_PET_HEART_OF_THE_PHOENIX_DEBUFF    = 55711,
    SPELL_HUNTER_PET_CARRION_FEEDER_TRIGGERED       = 54045,

    SPELL_HUNTER_SERPENT_STING                      = 1978,
    SPELL_HUNTER_SERPENT_STING_AURA                 = 118253,

    SPELL_HUNTER_STEADY_SHOT_FOCUS                  = 77443,

    SPELL_HUNTER_STAMPEDE                           = 121818,
    SPELL_HUNTER_STAMPEDE_DAMAGE_REDUCTION          = 130201,
    SPELL_HUNTER_GLYPH_OF_STAMPEDE                  = 57902,

    SPELL_HUNTER_GLAIVE_TOSS_AURA                   = 117050,
    SPELL_HUNTER_GLAIVE_TOSS_RIGHT                  = 120755,
    SPELL_HUNTER_GLAIVE_TOSS_LEFT                   = 120756,
    SPELL_HUNTER_GLAIVE_TOSS_DAMAGE_RIGHT           = 121414,
    SPELL_HUNTER_GLAIVE_TOSS_DAMAGE_LEFT            = 120761,

    SPELL_HUNTER_ARCANE_SHOT                        = 3044,
    SPELL_HUNTER_ASPECT_OF_THE_BEAST                = 61648,
    SPELL_HUNTER_ASPECTS_COOLDOWN                   = 119522,
    SPELL_HUNTER_AUTO_SHOT                          = 75,
    SPELL_HUNTER_BARRAGE                            = 120361,
    SPELL_HUNTER_BEAST_CLEAVE_AURA                  = 115939,
    SPELL_HUNTER_BEAST_CLEAVE_PROC                  = 118455,
    SPELL_HUNTER_BEAST_CLEAVE_DAMAGE                = 118459,
    SPELL_HUNTER_BLINK_STRIKES                      = 130392,
    SPELL_HUNTER_BLINK_STRIKE                       = 130393,
    SPELL_HUNTER_CAMOUFLAGE_DRIVER                  = 51753,
    SPELL_HUNTER_CAMOUFLAGE_AURA                    = 51755,
    SPELL_HUNTER_CAMOUFLAGE_INVISIBILITY            = 80325,
    SPELL_HUNTER_CAMOUFLAGE_INVISIBILITY_DRIVER     = 80326,
    SPELL_HUNTER_CAMOUFLAGE_INVISIBILITY_GLYPHED    = 119450,
    SPELL_HUNTER_COBRA_STRIKES_STACKS               = 53257,
    SPELL_HUNTER_ENTRAPMENT                         = 64803,
    SPELL_HUNTER_EXPLOSIVE_SHOT                     = 53301,
    SPELL_HUNTER_FOCUS_FIRE_READY                   = 88843,
    SPELL_HUNTER_FRENZY_ENERGIZE                    = 83468,
    SPELL_HUNTER_FRENZY_STACKS                      = 19615,
    SPELL_HUNTER_GLYPH_OF_ASPECT_OF_THE_CHEETAH     = 119462,
    SPELL_HUNTER_GLYPH_OF_ASPECTS                   = 122492,
    SPELL_HUNTER_GLYPH_OF_CAMOUFLAGE                = 119449,
    SPELL_HUNTER_GLYPH_OF_CHIMERA_SHOT              = 119447,
    SPELL_HUNTER_GLYPH_OF_DETERRENCE                = 56850,
    SPELL_HUNTER_GLYPH_OF_ENDLESS_WRATH             = 119410,
    SPELL_HUNTER_GLYPH_OF_EXPLOSIVE_TRAP            = 119403,
    SPELL_HUNTER_GLYPH_OF_MEND_PET                  = 19573,
    SPELL_HUNTER_GLYPH_OF_MEND_PET_DISPEL           = 24406,
    SPELL_HUNTER_GLYPH_OF_MISDIRECTION              = 56829,
    SPELL_HUNTER_HUNTERS_MARK                       = 1130,
    SPELL_HUNTER_KILL_COMMAND_CHARGE                = 118171,
    SPELL_HUNTER_KILL_COMMAND_DAMAGE                = 83381,
    SPELL_HUNTER_LESSER_PROPORTION                  = 57894,
    SPELL_HUNTER_LYNX_RUSH                          = 120697,
    SPELL_HUNTER_LYNX_RUSH_DOT                      = 120699,
    SPELL_HUNTER_MISDIRECTION_SYMBIOSIS             = 110588,
    SPELL_HUNTER_MISDIRECTION_SYMBIOSIS_PROC        = 110591,
    SPELL_HUNTER_NARROW_ESCAPE                      = 109298,
    SPELL_HUNTER_NARROW_ESCAPE_ROOT                 = 128405,
    SPELL_HUNTER_NARROW_ESCAPE_SELECTOR             = 115928,
    SPELL_HUNTER_PIERCING_SHOTS                     = 63468,
    SPELL_HUNTER_POSTHASTE                          = 109215,
    SPELL_HUNTER_POSTHASTE_SPEED                    = 118922,
    SPELL_HUNTER_RAPID_FIRE                         = 3045,
    SPELL_HUNTER_SERPENT_SPREAD                     = 87935,
    SPELL_HUNTER_SNIPER_TRAINING_BUFF               = 64418,
    SPELL_HUNTER_SPIKED_COLLAR                      = 53184,
    SPELL_HUNTER_SPIRIT_BEAST_BLESSING              = 127830,
    SPELL_HUNTER_SPIRIT_BOND_HEAL                   = 149254,
    SPELL_HUNTER_STEADY_FOCUS                       = 53220,
    SPELL_HUNTER_STEADY_SHOT                        = 56641,
    SPELL_HUNTER_T13_2P_BONUS                       = 105732,
    SPELL_HUNTER_TRACK_BEASTS                       = 1494,
    SPELL_HUNTER_TRACK_DEMONS                       = 19878,
    SPELL_HUNTER_TRACK_DRAGONKIN                    = 19879,
    SPELL_HUNTER_TRACK_ELEMENTALS                   = 19880,
    SPELL_HUNTER_TRACK_GIANTS                       = 19882,
    SPELL_HUNTER_TRACK_HUMANOIDS                    = 19883,
    SPELL_HUNTER_TRACK_UNDEAD                       = 19884,
    SPELL_HUNTER_TRACK_HIDDEN                       = 19885,
    SPELL_HUNTER_WEAKENED_ARMOR                     = 113746,
    SPELL_HUNTER_WILD_QUIVER                        = 76663,

    SPELL_HUNTER_BINDING_SHOT_AREA                  = 109248,
    SPELL_HUNTER_BINDING_SHOT_LINK                  = 117405,
    SPELL_HUNTER_BINDING_SHOT_STUN                  = 117526,
    SPELL_HUNTER_BINDING_SHOT_IMMUNE                = 117553,
    SPELL_HUNTER_BINDING_SHOT_VISUAL_STUN           = 117614,
    SPELL_HUNTER_BINDING_SHOT_VISUAL_MISSILE        = 118306,
    SPELL_HUNTER_ICE_TRAP_AREA                      = 13810,
    SPELL_HUNTER_ICE_TRAP_SLOW                      = 135299,
    SPELL_HUNTER_ICE_TRAP_SYMBIOSIS_AREA            = 135381,
    SPELL_HUNTER_ICE_TRAP_SYMBIOSIS_SLOW            = 135382,
    SPELL_HUNTER_GLYPH_OF_BLACK_ICE                 = 109263,
    SPELL_HUNTER_GLYPH_OF_BLACK_ICE_AREA            = 122243,
    SPELL_HUNTER_BLACK_ICE                          = 83559,
    SPELL_HUNTER_FLARE_DEBUFF                       = 132951,
    SPELL_HUNTER_THE_BEAST_WITHIN                   = 34471,
    SPELL_HUNTER_T16_4P_BONUS                       = 144641,
    SPELL_HUNTER_KEEN_EYE                           = 144659,
    SPELL_HUNTER_BRUTAL_KINSHIP                     = 144670,
    SPELL_HUNTER_BRUTAL_KINSHIP_PET                 = 145737,

    SPELL_HUNTER_DIRE_BEAST_JADE_FOREST             = 121118,
    SPELL_HUNTER_DIRE_BEAST_KALIMDOR                = 122802,
    SPELL_HUNTER_DIRE_BEAST_EASTERN_KINGDOMS        = 122804,
    SPELL_HUNTER_DIRE_BEAST_OUTLAND                 = 122806,
    SPELL_HUNTER_DIRE_BEAST_NORTHREND               = 122807,
    SPELL_HUNTER_DIRE_BEAST_KRASARANG_WILDS         = 122809,
    SPELL_HUNTER_DIRE_BEAST_VALLEY_OF_FOUR_WINDS    = 122811,
    SPELL_HUNTER_DIRE_BEAST_VALE_OF_BLOSSOMS        = 126213,
    SPELL_HUNTER_DIRE_BEAST_KUN_LAI_SUMMIT          = 126214,
    SPELL_HUNTER_DIRE_BEAST_TOWNLONG_STEPPES        = 126215,
    SPELL_HUNTER_DIRE_BEAST_DREAD_WASTES            = 126216,
};

enum HunterCreatures
{
    NPC_HUNTER_DIRE_BEAST                           = 62005
};

static void EnergizeFromShot(Unit* caster, uint32 energizeSpellId, bool steadyShot)
{
    SpellInfo const* energizeInfo = sSpellMgr->GetSpellInfo(energizeSpellId);
    if (!energizeInfo)
        return;

    int32 amount = energizeInfo->Effects[EFFECT_0].CalcValue(caster);

    if (steadyShot)
        if (AuraEffect const* steadyFocus = caster->GetAuraEffect(SPELL_HUNTER_STEADY_FOCUS, EFFECT_1))
            amount += steadyFocus->GetAmount();

    if (caster->HasAura(SPELL_HUNTER_T13_2P_BONUS))
        amount *= 2;

    caster->CastCustomSpell(caster, energizeSpellId, &amount, NULL, NULL, true);
}

static uint32 GetGlaiveTossDamageSpell(uint32 missileSpellId)
{
    return missileSpellId == SPELL_HUNTER_GLAIVE_TOSS_RIGHT
        ? SPELL_HUNTER_GLAIVE_TOSS_DAMAGE_RIGHT
        : SPELL_HUNTER_GLAIVE_TOSS_DAMAGE_LEFT;
}

static uint32 GetGlaiveTossOppositeMissile(uint32 missileSpellId)
{
    return missileSpellId == SPELL_HUNTER_GLAIVE_TOSS_RIGHT
        ? SPELL_HUNTER_GLAIVE_TOSS_LEFT
        : SPELL_HUNTER_GLAIVE_TOSS_RIGHT;
}

class DelayedGlaiveTossReturnEvent : public BasicEvent
{
public:
    DelayedGlaiveTossReturnEvent(uint64 hunterGuid, uint64 targetGuid, uint32 bounceMissileSpell)
        : _hunterGuid(hunterGuid), _targetGuid(targetGuid), _bounceMissileSpell(bounceMissileSpell) { }

    bool Execute(uint64 /*e_time*/, uint32 /*p_time*/) OVERRIDE
    {
        Player* hunter = ObjectAccessor::FindPlayer(_hunterGuid);
        if (!hunter)
            return true;

        Unit* target = ObjectAccessor::GetUnit(*hunter, _targetGuid);
        if (!target || !hunter->IsValidAttackTarget(target))
            return true;

        hunter->CastSpell(hunter, GetGlaiveTossDamageSpell(_bounceMissileSpell), true);
        return true;
    }

private:
    uint64 _hunterGuid;
    uint64 _targetGuid;
    uint32 _bounceMissileSpell;
};

class spell_hun_a_murder_of_crows : public SpellScriptLoader
{
public:
    spell_hun_a_murder_of_crows() : SpellScriptLoader("spell_hun_a_murder_of_crows") { }

    class spell_hun_a_murder_of_crows_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_a_murder_of_crows_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_A_MURDER_OF_CROWS))
                return false;
            return true;
        }

        void HandleEffectPeriodic(AuraEffect const* /*aurEff*/)
        {
            if (Unit* target = GetTarget())
            {
                if (Player* player = GetCaster()->ToPlayer())
                {
                    player->CastSpell(target, SPELL_HUNTER_CROW_TRIGGER, true);
                    player->CastSpell(target, SPELL_HUNTER_CROW_DMG, true);
                    player->CastSpell(target, SPELL_HUNTER_DELAYED_CROW, true);
                    player->CastSpell(target, SPELL_HUNTER_CROW_DMG, true);
                    player->CastSpell(target, SPELL_HUNTER_DELAYED_CROW2, true);
                    player->CastSpell(target, SPELL_HUNTER_CROW_DMG, true);
                }
            }
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_hun_a_murder_of_crows_AuraScript::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    class spell_hun_a_murder_of_crows_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_a_murder_of_crows_SpellScript);

        void HandleAfterCast()
        {
            Unit* target = GetExplTargetUnit();
            if (!target || !target->HealthBelowPct(20))
                return;

            if (Player* player = GetCaster()->ToPlayer())
                player->ModifySpellCooldown(SPELL_HUNTER_A_MURDER_OF_CROWS, -(GetSpellInfo()->Effects[EFFECT_0].BasePoints * IN_MILLISECONDS));
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_hun_a_murder_of_crows_SpellScript::HandleAfterCast);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_a_murder_of_crows_AuraScript();
    }

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_a_murder_of_crows_SpellScript();
    }
};

// 53209 - Chimera Shot
class spell_hun_chimera_shot : public SpellScriptLoader
{
public:
    spell_hun_chimera_shot() : SpellScriptLoader("spell_hun_chimera_shot") { }

    class spell_hun_chimera_shot_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_chimera_shot_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_CHIMERA_SHOT_HEAL) ||
                !sSpellMgr->GetSpellInfo(SPELL_HUNTER_SERPENT_STING_AURA))
                return false;
            return true;
        }

        bool Load() OVERRIDE
        {
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void HandleScriptEffect(SpellEffIndex /*effIndex*/)
        {
            GetCaster()->CastSpell(GetCaster(), SPELL_HUNTER_CHIMERA_SHOT_HEAL, true);

            if (Aura* aur = GetHitUnit()->GetAura(SPELL_HUNTER_SERPENT_STING_AURA, GetCaster()->GetGUID()))
                aur->RefreshDuration();
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_hun_chimera_shot_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_chimera_shot_SpellScript();
    }
};

// 77767 - Cobra Shot
class spell_hun_cobra_shot : public SpellScriptLoader
{
public:
    spell_hun_cobra_shot() : SpellScriptLoader("spell_hun_cobra_shot") { }

    class spell_hun_cobra_shot_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_cobra_shot_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_GENERIC_ENERGIZE_FOCUS) ||
                !sSpellMgr->GetSpellInfo(SPELL_HUNTER_SERPENT_STING_AURA))
                return false;
            return true;
        }

        bool Load() OVERRIDE
        {
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void HandleScriptEffect(SpellEffIndex /*effIndex*/)
        {
            EnergizeFromShot(GetCaster(), SPELL_HUNTER_GENERIC_ENERGIZE_FOCUS, false);

            if (Aura* aur = GetHitUnit()->GetAura(SPELL_HUNTER_SERPENT_STING_AURA, GetCaster()->GetGUID()))
            {
                int32 newDuration = aur->GetDuration() + GetEffectValue() * IN_MILLISECONDS;
                if (newDuration > aur->GetMaxDuration())
                    aur->SetMaxDuration(newDuration);
                aur->SetDuration(newDuration);
            }
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_hun_cobra_shot_SpellScript::HandleScriptEffect, EFFECT_2, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_cobra_shot_SpellScript();
    }
};

// 120679 - Dire Beast
class spell_hun_dire_beast : public SpellScriptLoader
{
public:
    spell_hun_dire_beast() : SpellScriptLoader("spell_hun_dire_beast") { }

    class spell_hun_dire_beast_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_dire_beast_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_DIRE_BEAST_SUMMON))
                return false;
            return true;
        }

        bool Load() OVERRIDE
        {
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        uint32 GetSummonSpell(Unit* caster) const
        {
            switch (caster->GetZoneId())
            {
                case 5785: return SPELL_HUNTER_DIRE_BEAST_JADE_FOREST;
                case 5805: return SPELL_HUNTER_DIRE_BEAST_VALLEY_OF_FOUR_WINDS;
                case 5840: return SPELL_HUNTER_DIRE_BEAST_VALE_OF_BLOSSOMS;
                case 5841: return SPELL_HUNTER_DIRE_BEAST_KUN_LAI_SUMMIT;
                case 5842: return SPELL_HUNTER_DIRE_BEAST_TOWNLONG_STEPPES;
                case 6134: return SPELL_HUNTER_DIRE_BEAST_KRASARANG_WILDS;
                case 6138: return SPELL_HUNTER_DIRE_BEAST_DREAD_WASTES;
                default:
                    break;
            }

            switch (caster->GetMapId())
            {
                case 0:   return SPELL_HUNTER_DIRE_BEAST_EASTERN_KINGDOMS;
                case 1:   return SPELL_HUNTER_DIRE_BEAST_KALIMDOR;
                case 530: return SPELL_HUNTER_DIRE_BEAST_OUTLAND;
                case 571: return SPELL_HUNTER_DIRE_BEAST_NORTHREND;
                default:
                    break;
            }

            if (caster->GetMap()->IsDungeon())
                return SPELL_HUNTER_DIRE_BEAST_SUMMON;
            return SPELL_HUNTER_DIRE_BEAST_EASTERN_KINGDOMS;
        }

        void HandleEffect(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!target)
                return;

            uint32 summonSpellId = GetSummonSpell(caster);
            SpellInfo const* summonInfo = sSpellMgr->GetSpellInfo(summonSpellId);
            if (!summonInfo)
            {
                summonSpellId = SPELL_HUNTER_DIRE_BEAST_SUMMON;
                summonInfo = sSpellMgr->GetSpellInfo(summonSpellId);
            }

            caster->CastSpell(target, summonSpellId, true);

            uint32 entry = summonInfo ? uint32(summonInfo->Effects[EFFECT_0].MiscValue) : 0;
            if (!entry)
                entry = NPC_HUNTER_DIRE_BEAST;

            std::list<Creature*> minions;
            caster->GetAllMinionsByEntry(minions, entry);
            for (std::list<Creature*>::iterator itr = minions.begin(); itr != minions.end(); ++itr)
                if ((*itr)->IsAlive() && (*itr)->AI())
                    (*itr)->AI()->AttackStart(target);
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_hun_dire_beast_SpellScript::HandleEffect, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_dire_beast_SpellScript();
    }
};

// 82926 - Fire!
class spell_hun_fire : public SpellScriptLoader
{
public:
    spell_hun_fire() : SpellScriptLoader("spell_hun_fire") { }

    class spell_hun_fire_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_fire_AuraScript);

        void HandleEffectCalcSpellMod(AuraEffect const* aurEff, SpellModifier*& spellMod)
        {
            if (!spellMod)
            {
                spellMod = new SpellModifier(GetAura());
                spellMod->op = SPELLMOD_CASTING_TIME;
                spellMod->type = SPELLMOD_PCT;
                spellMod->spellId = GetId();
                spellMod->mask = GetSpellInfo()->Effects[aurEff->GetEffIndex()].SpellClassMask;
            }

            spellMod->value = -(float)aurEff->GetAmount();
        }

        void Register() OVERRIDE
        {
            DoEffectCalcSpellMod += AuraEffectCalcSpellModFn(spell_hun_fire_AuraScript::HandleEffectCalcSpellMod, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_fire_AuraScript();
    }
};

// 82834 - Improved Serpent Sting
class spell_hun_improved_serpent_sting : public SpellScriptLoader
{
public:
    spell_hun_improved_serpent_sting() : SpellScriptLoader("spell_hun_improved_serpent_sting") { }

    class spell_hun_improved_serpent_sting_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_improved_serpent_sting_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_IMPROVED_SERPENT_STING))
                return false;
            return true;
        }

        void HandleEffectCalcSpellMod(AuraEffect const* aurEff, SpellModifier*& spellMod)
        {
            if (!spellMod)
            {
                spellMod = new SpellModifier(GetAura());
                spellMod->op = SpellModOp(aurEff->GetMiscValue());
                spellMod->type = SPELLMOD_PCT;
                spellMod->spellId = GetId();
                spellMod->mask = GetSpellInfo()->Effects[aurEff->GetEffIndex()].SpellClassMask;
            }

            spellMod->value = (float)aurEff->GetAmount();
        }

        void Register() OVERRIDE
        {
            DoEffectCalcSpellMod += AuraEffectCalcSpellModFn(spell_hun_improved_serpent_sting_AuraScript::HandleEffectCalcSpellMod, EFFECT_1, SPELL_AURA_ADD_PCT_MODIFIER);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_improved_serpent_sting_AuraScript();
    }
};

// 118253 - Serpent Sting
class spell_hun_serpent_sting : public SpellScriptLoader
{
public:
    spell_hun_serpent_sting() : SpellScriptLoader("spell_hun_serpent_sting") { }

    class spell_hun_serpent_sting_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_serpent_sting_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_SERPENT_STING_AURA) ||
                !sSpellMgr->GetSpellInfo(SPELL_HUNTER_IMPROVED_SERPENT_STING) ||
                !sSpellMgr->GetSpellInfo(SPELL_HUNTER_IMPROVED_SERPENT_STING_DAMAGE))
                return false;
            return true;
        }

        void HandleEffectApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetTarget();
            if (!caster || !target)
                return;

            AuraEffect const* improvedSerpentSting = caster->GetAuraEffect(SPELL_HUNTER_IMPROVED_SERPENT_STING, EFFECT_0);
            if (!improvedSerpentSting)
                return;

            int32 const periodicTotal = aurEff->GetAmount() * int32(aurEff->GetTotalTicks());
            int32 const instantDamage = periodicTotal * improvedSerpentSting->GetAmount() / 100;
            if (instantDamage <= 0)
                return;

            caster->CastCustomSpell(target, SPELL_HUNTER_IMPROVED_SERPENT_STING_DAMAGE, &instantDamage, NULL, NULL, true);
        }

        void Register() OVERRIDE
        {
            AfterEffectApply += AuraEffectApplyFn(spell_hun_serpent_sting_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_serpent_sting_AuraScript();
    }
};

// 53397 - Invigoration
class spell_hun_invigoration : public SpellScriptLoader
{
public:
    spell_hun_invigoration() : SpellScriptLoader("spell_hun_invigoration") { }

    class spell_hun_invigoration_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_invigoration_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_INVIGORATION_TRIGGERED))
                return false;
            return true;
        }

        bool Load() OVERRIDE
        {
            return GetUnitOwner()->IsPet();
        }

        void HandleProc(ProcEventInfo& /*eventInfo*/)
        {
            if (Player* owner = GetUnitOwner()->ToPet()->GetOwner())
                owner->CastSpell(owner, SPELL_HUNTER_INVIGORATION_TRIGGERED, true);
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_hun_invigoration_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_invigoration_AuraScript();
    }
};

// 53478 - Last Stand Pet
class spell_hun_last_stand_pet : public SpellScriptLoader
{
public:
    spell_hun_last_stand_pet() : SpellScriptLoader("spell_hun_last_stand_pet") { }

    class spell_hun_last_stand_pet_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_last_stand_pet_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_PET_LAST_STAND_TRIGGERED))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            int32 healthModSpellBasePoints0 = int32(caster->CountPctFromMaxHealth(30));
            caster->CastCustomSpell(caster, SPELL_HUNTER_PET_LAST_STAND_TRIGGERED, &healthModSpellBasePoints0, NULL, NULL, true, NULL);
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_hun_last_stand_pet_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_last_stand_pet_SpellScript();
    }
};

// 53271 - Masters Call
class spell_hun_masters_call : public SpellScriptLoader
{
public:
    spell_hun_masters_call() : SpellScriptLoader("spell_hun_masters_call") { }

    class spell_hun_masters_call_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_masters_call_SpellScript);

        bool Validate(SpellInfo const* spellInfo) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_MASTERS_CALL_TRIGGERED) ||
                !sSpellMgr->GetSpellInfo(spellInfo->Effects[EFFECT_0].CalcValue()))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Unit* ally = GetHitUnit())
                if (Player* caster = GetCaster()->ToPlayer())
                    if (Pet* target = caster->GetPet())
                    {
                        TriggerCastFlags castMask = TriggerCastFlags(TRIGGERED_FULL_MASK & ~TRIGGERED_IGNORE_CASTER_AURASTATE);
                        target->CastSpell(ally, GetEffectValue(), castMask);
                    }
        }

        void HandleScriptEffect(SpellEffIndex /*effIndex*/)
        {
            if (Unit* target = GetHitUnit())
            {
                // Cannot be processed while pet is dead
                TriggerCastFlags castMask = TriggerCastFlags(TRIGGERED_FULL_MASK & ~TRIGGERED_IGNORE_CASTER_AURASTATE);
                target->CastSpell(target, SPELL_HUNTER_MASTERS_CALL_TRIGGERED, castMask);
            }
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_hun_masters_call_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            OnEffectHitTarget += SpellEffectFn(spell_hun_masters_call_SpellScript::HandleScriptEffect, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_masters_call_SpellScript();
    }
};

// 34477 - Misdirection, 110588 - Misdirection (Symbiosis)
class spell_hun_misdirection : public SpellScriptLoader
{
public:
    spell_hun_misdirection() : SpellScriptLoader("spell_hun_misdirection") { }

    class spell_hun_misdirection_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_misdirection_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_MISDIRECTION_PROC))
                return false;
            return true;
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_DEFAULT)
                GetTarget()->ResetRedirectThreat();
        }

        bool CheckProc(ProcEventInfo& /*eventInfo*/)
        {
            return GetTarget()->GetRedirectThreatTarget();
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();

            Unit* caster = GetTarget();
            Unit* redirectTarget = caster->GetRedirectThreatTarget();
            uint32 procSpellId = GetId() == SPELL_HUNTER_MISDIRECTION_SYMBIOSIS ? SPELL_HUNTER_MISDIRECTION_SYMBIOSIS_PROC : SPELL_HUNTER_MISDIRECTION_PROC;
            caster->CastSpell(caster, procSpellId, true, NULL, aurEff);

            if (redirectTarget && redirectTarget == caster->GetGuardianPet() && caster->HasAura(SPELL_HUNTER_GLYPH_OF_MISDIRECTION))
                if (Player* player = caster->ToPlayer())
                    player->RemoveSpellCooldown(GetId(), true);
        }

        void Register() OVERRIDE
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_hun_misdirection_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            DoCheckProc += AuraCheckProcFn(spell_hun_misdirection_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_hun_misdirection_AuraScript::HandleProc, EFFECT_1, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_misdirection_AuraScript();
    }
};

// 35079 - Misdirection (Proc), 110591 - Misdirection (Symbiosis) (Proc)
class spell_hun_misdirection_proc : public SpellScriptLoader
{
public:
    spell_hun_misdirection_proc() : SpellScriptLoader("spell_hun_misdirection_proc") { }

    class spell_hun_misdirection_proc_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_misdirection_proc_AuraScript);

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->ResetRedirectThreat();
        }

        void Register() OVERRIDE
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_hun_misdirection_proc_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_misdirection_proc_AuraScript();
    }
};

// 54044 - Pet Carrion Feeder
class spell_hun_pet_carrion_feeder : public SpellScriptLoader
{
public:
    spell_hun_pet_carrion_feeder() : SpellScriptLoader("spell_hun_pet_carrion_feeder") { }

    class spell_hun_pet_carrion_feeder_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_pet_carrion_feeder_SpellScript);

        bool Load() OVERRIDE
        {
            if (!GetCaster()->IsPet())
                return false;
            return true;
        }

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_PET_CARRION_FEEDER_TRIGGERED))
                return false;
            return true;
        }

        SpellCastResult CheckIfCorpseNear()
        {
            Unit* caster = GetCaster();
            float max_range = GetSpellInfo()->GetMaxRange(false);
            WorldObject* result = NULL;
            // search for nearby enemy corpse in range
            Skyfire::AnyDeadUnitSpellTargetInRangeCheck check(caster, max_range, GetSpellInfo(), TARGET_CHECK_ENEMY);
            Skyfire::WorldObjectSearcher<Skyfire::AnyDeadUnitSpellTargetInRangeCheck> searcher(caster, result, check);
            caster->GetMap()->VisitFirstFound(caster->m_positionX, caster->m_positionY, max_range, searcher);
            if (!result)
                return SpellCastResult::SPELL_FAILED_NO_EDIBLE_CORPSES;
            return SpellCastResult::SPELL_CAST_OK;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            caster->CastSpell(caster, SPELL_HUNTER_PET_CARRION_FEEDER_TRIGGERED, false);
        }

        void Register() OVERRIDE
        {
            OnEffectHit += SpellEffectFn(spell_hun_pet_carrion_feeder_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            OnCheckCast += SpellCheckCastFn(spell_hun_pet_carrion_feeder_SpellScript::CheckIfCorpseNear);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_pet_carrion_feeder_SpellScript();
    }
};

// 55709 - Pet Heart of the Phoenix
class spell_hun_pet_heart_of_the_phoenix : public SpellScriptLoader
{
public:
    spell_hun_pet_heart_of_the_phoenix() : SpellScriptLoader("spell_hun_pet_heart_of_the_phoenix") { }

    class spell_hun_pet_heart_of_the_phoenix_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_pet_heart_of_the_phoenix_SpellScript);

        bool Load() OVERRIDE
        {
            if (!GetCaster()->IsPet())
                return false;
            return true;
        }

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_PET_HEART_OF_THE_PHOENIX_TRIGGERED) || !sSpellMgr->GetSpellInfo(SPELL_HUNTER_PET_HEART_OF_THE_PHOENIX_DEBUFF))
                return false;
            return true;
        }

        void HandleScript(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (Unit* owner = caster->GetOwner())
                if (!caster->HasAura(SPELL_HUNTER_PET_HEART_OF_THE_PHOENIX_DEBUFF))
                {
                    owner->CastCustomSpell(SPELL_HUNTER_PET_HEART_OF_THE_PHOENIX_TRIGGERED, SPELLVALUE_BASE_POINT0, 100, caster, true);
                    caster->CastSpell(caster, SPELL_HUNTER_PET_HEART_OF_THE_PHOENIX_DEBUFF, true);
                }
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_hun_pet_heart_of_the_phoenix_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_pet_heart_of_the_phoenix_SpellScript();
    }
};

// 82925 - Ready, Set, Aim...
class spell_hun_ready_set_aim : public SpellScriptLoader
{
public:
    spell_hun_ready_set_aim() : SpellScriptLoader("spell_hun_ready_set_aim") { }

    class spell_hun_ready_set_aim_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_ready_set_aim_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_FIRE))
                return false;
            return true;
        }

        void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (GetStackAmount() >= GetSpellInfo()->StackAmount)
            {
                GetTarget()->CastSpell(GetTarget(), SPELL_HUNTER_FIRE, true, NULL, aurEff);
                GetTarget()->RemoveAura(GetId());
            }
        }

        void Register() OVERRIDE
        {
            AfterEffectApply += AuraEffectApplyFn(spell_hun_ready_set_aim_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_ready_set_aim_AuraScript();
    }
};

// 37506 - Scatter Shot
class spell_hun_scatter_shot : public SpellScriptLoader
{
public:
    spell_hun_scatter_shot() : SpellScriptLoader("spell_hun_scatter_shot") { }

    class spell_hun_scatter_shot_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_scatter_shot_SpellScript);

        bool Load() OVERRIDE
        {
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Player* caster = GetCaster()->ToPlayer();
            // break Auto Shot and autohit
            caster->InterruptSpell(CURRENT_AUTOREPEAT_SPELL);
            caster->AttackStop();
            caster->SendAttackSwingCancelAttack();
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_hun_scatter_shot_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_scatter_shot_SpellScript();
    }
};

// 56641 - Steady Shot
class spell_hun_steady_shot : public SpellScriptLoader
{
public:
    spell_hun_steady_shot() : SpellScriptLoader("spell_hun_steady_shot") { }

    class spell_hun_steady_shot_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_steady_shot_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_STEADY_SHOT_FOCUS))
                return false;
            return true;
        }

        bool Load() OVERRIDE
        {
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void HandleAfterCast()
        {
            EnergizeFromShot(GetCaster(), SPELL_HUNTER_STEADY_SHOT_FOCUS, true);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_hun_steady_shot_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_steady_shot_SpellScript();
    }
};

// 1515 - Tame Beast
class spell_hun_tame_beast : public SpellScriptLoader
{
public:
    spell_hun_tame_beast() : SpellScriptLoader("spell_hun_tame_beast") { }

    class spell_hun_tame_beast_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_tame_beast_SpellScript);

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            if (caster->GetTypeId() != TypeID::TYPEID_PLAYER)
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;

            if (!GetExplTargetUnit())
                return SpellCastResult::SPELL_FAILED_BAD_IMPLICIT_TARGETS;

            if (Creature* target = GetExplTargetUnit()->ToCreature())
            {
                if (target->getLevel() > caster->getLevel())
                    return SpellCastResult::SPELL_FAILED_HIGHLEVEL;

                // use SMSG_PET_TAME_FAILURE?
                if (!target->GetCreatureTemplate()->IsTameable(caster->ToPlayer()->CanTameExoticPets()))
                    return SpellCastResult::SPELL_FAILED_BAD_TARGETS;

                if (caster->GetPetGUID())
                    return SpellCastResult::SPELL_FAILED_ALREADY_HAVE_SUMMON;

                if (caster->GetCharmGUID())
                    return SpellCastResult::SPELL_FAILED_ALREADY_HAVE_CHARM;
            }
            else
                return SpellCastResult::SPELL_FAILED_BAD_IMPLICIT_TARGETS;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_hun_tame_beast_SpellScript::CheckCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_tame_beast_SpellScript();
    }
};

// 121818 - Stampede
class spell_hun_stampede : public SpellScriptLoader
{
public:
    spell_hun_stampede() : SpellScriptLoader("spell_hun_stampede") { }

    class spell_hun_stampede_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_stampede_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_STAMPEDE_DAMAGE_REDUCTION))
                return false;
            return true;
        }

        void ApplyStampedePetScale(Pet* pet, Player* player)
        {
            if (!pet || !player)
                return;

            // Stats/level are already set during LoadPetFromDB. Do not call InitStatsForLevel
            // again here — that can re-apply undersized family minScale if level is wrong.
            float scale = 1.0f;
            if (Pet* active = player->GetPet())
                scale = active->GetObjectScale();

            // Stampede pets are full-power summons: never below CreatureFamily maxScale.
            if (CreatureTemplate const* cinfo = pet->GetCreatureTemplate())
                if (CreatureFamilyEntry const* family = sCreatureFamilyStore.LookupEntry(cinfo->family))
                    if (family->maxScale > 0.0f)
                        scale = std::max(scale, family->maxScale);

            pet->SetObjectScale(scale);
            pet->SetHealth(pet->GetMaxHealth());
            if (pet->getPowerType() == POWER_FOCUS)
                pet->SetPower(POWER_FOCUS, pet->GetMaxPower(POWER_FOCUS));
        }

        void InitStampedePet(Pet* pet, Unit* target, int32 duration)
        {
            if (!pet || !target)
                return;

            // Ensure spawn is at the enemy even if load relocated to the owner.
            Position spawnPos;
            target->GetRandomNearPosition(spawnPos, PET_FOLLOW_DIST + target->GetObjectSize());
            pet->NearTeleportTo(spawnPos.GetPositionX(), spawnPos.GetPositionY(), spawnPos.GetPositionZ(),
                spawnPos.GetOrientation());

            pet->SetDuration(duration);

            if (Player* owner = pet->GetOwner())
                ApplyStampedePetScale(pet, owner);

            // Assist: only fight the Stampede / hunter target (no nearest-hostile aggro).
            pet->SetReactState(REACT_ASSIST);
            if (CharmInfo* ci = pet->GetCharmInfo())
            {
                ci->SetCommandState(COMMAND_FOLLOW);
                ci->SetIsCommandAttack(true);
                ci->SetIsCommandFollow(false);
                ci->SetIsAtStay(false);
                ci->SetIsFollowing(false);
                ci->SetIsReturning(false);
            }

            // Treant-style engage: always chase. Do not call PetAI::AttackStart after Attack() —
            // Attack() already on the same victim makes DoAttack no-op and skips MoveChase.
            pet->ClearUnitState(UNIT_STATE_EVADE);
            pet->AttackStop();
            if (!pet->Attack(target, true))
            {
                pet->SetInCombatWith(target);
                target->SetInCombatWith(pet);
                pet->SetTarget(target->GetGUID());
                pet->AddUnitState(UNIT_STATE_MELEE_ATTACKING);
            }
            pet->GetMotionMaster()->Clear(true);
            pet->GetMotionMaster()->MoveChase(target);

            if (target->GetCharmerOrOwnerPlayerOrPlayerItself() || pet->GetMap()->IsBattlegroundOrArena())
                pet->CastSpell(pet, SPELL_HUNTER_STAMPEDE_DAMAGE_REDUCTION, true);
        }

        Pet* SummonStampedeClone(Player* player, uint32 petNumber, Position const& spawnPos)
        {
            Pet* pet = new Pet(player, PetType::HUNTER_PET);
            if (!pet->LoadPetFromDB(player, 0, petNumber, false, -1, true, &spawnPos))
            {
                delete pet;
                return NULL;
            }
            return pet;
        }

        Unit* ResolveStampedeTarget(Player* player)
        {
            // Stampede's OnHit unit is often the caster (dummy / summon effects). Use the
            // explicit enemy the hunter cast at / has selected.
            if (Unit* expl = GetExplTargetUnit())
                if (player->IsValidAttackTarget(expl))
                    return expl;

            if (Unit* selected = player->GetSelectedUnit())
                if (player->IsValidAttackTarget(selected))
                    return selected;

            if (Unit* victim = player->GetVictim())
                if (player->IsValidAttackTarget(victim))
                    return victim;

            return NULL;
        }

        void HandleAfterCast()
        {
            Player* player = GetCaster() ? GetCaster()->ToPlayer() : NULL;
            if (!player)
                return;

            Unit* target = ResolveStampedeTarget(player);
            if (!target)
                return;

            Pet* currentPet = player->GetPet();
            if (!currentPet || !currentPet->GetCharmInfo())
                return;

            int32 duration = player->CalcSpellDuration(GetSpellInfo());
            if (duration <= 0)
                duration = 20 * IN_MILLISECONDS;

            uint8 const currentSlot = currentPet->GetSlot();
            uint32 const currentPetNumber = currentPet->GetCharmInfo()->GetPetNumber();
            bool const glyph = player->HasAura(SPELL_HUNTER_GLYPH_OF_STAMPEDE);

            for (int8 slot = PET_SAVE_FIRST_ACTIVE_SLOT; slot <= PET_SAVE_LAST_ACTIVE_SLOT; ++slot)
            {
                if (uint8(slot) == currentSlot)
                    continue;

                Position spawnPos;
                target->GetRandomNearPosition(spawnPos, PET_FOLLOW_DIST + target->GetObjectSize());

                Pet* pet = NULL;
                if (glyph)
                    pet = SummonStampedeClone(player, currentPetNumber, spawnPos);
                else
                {
                    pet = new Pet(player, PetType::HUNTER_PET);
                    if (!pet->LoadPetFromDB(player, 0, 0, false, slot, true, &spawnPos))
                    {
                        delete pet;
                        pet = SummonStampedeClone(player, currentPetNumber, spawnPos);
                    }
                }

                InitStampedePet(pet, target, duration);
            }
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_hun_stampede_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_stampede_SpellScript();
    }
};

// Glaive Toss damage - 120761 / 121414
class spell_hun_glaive_toss_damage : public SpellScriptLoader
{
public:
    spell_hun_glaive_toss_damage() : SpellScriptLoader("spell_hun_glaive_toss_damage") { }

    class spell_hun_glaive_toss_damage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_glaive_toss_damage_SpellScript);

        uint64 _mainTargetGUID;

        bool Load() OVERRIDE
        {
            _mainTargetGUID = 0;
            return true;
        }

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.clear();

            Unit* caster = GetCaster();
            if (!caster)
                return;

            std::list<Unit*> targetList;
            float const radius = 50.0f;
            Skyfire::AnyUnfriendlyAttackableVisibleUnitInObjectRangeCheck u_check(caster, radius);
            Skyfire::UnitListSearcher<Skyfire::AnyUnfriendlyAttackableVisibleUnitInObjectRangeCheck> searcher(caster, targetList, u_check);
            caster->VisitNearbyObject(radius, searcher);

            for (Unit* unit : targetList)
            {
                if (unit->HasAura(SPELL_HUNTER_GLAIVE_TOSS_AURA, caster->GetGUID()))
                {
                    _mainTargetGUID = unit->GetGUID();
                    break;
                }
            }

            if (!_mainTargetGUID)
            {
                for (Unit* unit : targetList)
                {
                    if (unit->HasAura(SPELL_HUNTER_GLAIVE_TOSS_AURA))
                    {
                        _mainTargetGUID = unit->GetGUID();
                        break;
                    }
                }
            }

            if (!_mainTargetGUID)
                return;

            Unit* mainTarget = ObjectAccessor::GetUnit(*caster, _mainTargetGUID);
            if (!mainTarget)
                return;

            targets.push_back(mainTarget);
            for (Unit* unit : targetList)
            {
                if (unit == mainTarget)
                    continue;
                if (!caster->IsValidAttackTarget(unit))
                    continue;
                if (unit->IsInBetween(caster, mainTarget, 5.0f))
                    targets.push_back(unit);
            }
        }

        void HandleOnHit()
        {
            if (!_mainTargetGUID)
                return;

            Unit* mainTarget = ObjectAccessor::GetUnit(*GetCaster(), _mainTargetGUID);
            if (!mainTarget || GetHitUnit() != mainTarget)
                return;

            SetHitDamage(GetHitDamage() * 4);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hun_glaive_toss_damage_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hun_glaive_toss_damage_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ENEMY);
            OnHit += SpellHitFn(spell_hun_glaive_toss_damage_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_glaive_toss_damage_SpellScript();
    }
};

// Glaive Toss missiles - 120755 / 120756
class spell_hun_glaive_toss_missile : public SpellScriptLoader
{
public:
    spell_hun_glaive_toss_missile() : SpellScriptLoader("spell_hun_glaive_toss_missile") { }

    class spell_hun_glaive_toss_missile_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_glaive_toss_missile_SpellScript);

        void HandleAfterCast()
        {
            Unit* caster = GetCaster();
            Unit* originalCaster = GetOriginalCaster();
            if (!caster || !originalCaster)
                return;

            Player* hunter = originalCaster->ToPlayer();
            if (!hunter)
                return;

            uint32 const missileId = GetSpellInfo()->Id;
            uint32 const damageSpell = GetGlaiveTossDamageSpell(missileId);

            // Outbound glaive: mark primary, hit the line once, schedule the return pass.
            if (caster == originalCaster)
            {
                Unit* target = GetExplTargetUnit();
                if (target)
                    hunter->AddAura(SPELL_HUNTER_GLAIVE_TOSS_AURA, target);

                hunter->CastSpell(hunter, damageSpell, true);

                if (target)
                {
                    uint32 const bounceSpell = GetGlaiveTossOppositeMissile(missileId);
                    float const speed = GetSpellInfo()->Speed > 0.0f ? GetSpellInfo()->Speed : 18.0f;
                    uint32 const delay = std::max<uint32>(50, uint32((hunter->GetExactDist2d(target) / speed) * 1000.0f));

                    hunter->m_Events.AddEvent(new DelayedGlaiveTossReturnEvent(hunter->GetGUID(), target->GetGUID(), bounceSpell),
                        hunter->m_Events.CalculateTime(delay));
                }
                return;
            }
        }

        void HandleOnHit()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!caster || !target || caster != GetOriginalCaster())
                return;

            // Keep client bounce visuals in sync; return damage is handled above.
            target->CastSpell(caster, GetGlaiveTossOppositeMissile(GetSpellInfo()->Id), true, NULL, NULL, caster->GetGUID());
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_hun_glaive_toss_missile_SpellScript::HandleAfterCast);
            OnHit += SpellHitFn(spell_hun_glaive_toss_missile_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_glaive_toss_missile_SpellScript();
    }
};

// 34026 - Kill Command
class spell_hun_kill_command : public SpellScriptLoader
{
public:
    spell_hun_kill_command() : SpellScriptLoader("spell_hun_kill_command") { }

    class spell_hun_kill_command_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_kill_command_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_KILL_COMMAND_DAMAGE) ||
                !sSpellMgr->GetSpellInfo(SPELL_HUNTER_KILL_COMMAND_CHARGE))
                return false;
            return true;
        }

        SpellCastResult CheckCast()
        {
            Player* player = GetCaster()->ToPlayer();
            if (!player)
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;

            Pet* pet = player->GetPet();
            if (!pet || pet->isDead())
                return SpellCastResult::SPELL_FAILED_NO_PET;

            Unit* target = GetExplTargetUnit();
            if (!target || !pet->IsWithinDist(target, 25.0f))
                return SpellCastResult::SPELL_FAILED_OUT_OF_RANGE;

            uint32 petUnitFlags = pet->GetUInt32Value(UNIT_FIELD_FLAGS);
            if (petUnitFlags & UNIT_FLAG_STUNNED)
                return SpellCastResult::SPELL_FAILED_STUNNED;
            if (petUnitFlags & UNIT_FLAG_CONFUSED)
                return SpellCastResult::SPELL_FAILED_CONFUSED;
            if (petUnitFlags & UNIT_FLAG_FLEEING)
                return SpellCastResult::SPELL_FAILED_FLEEING;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Player* player = GetCaster()->ToPlayer();
            Unit* target = GetHitUnit();
            if (!player || !target)
                return;

            if (Pet* pet = player->GetPet())
            {
                if (pet->AI())
                    pet->AI()->AttackStart(target);
                pet->CastSpell(target, SPELL_HUNTER_KILL_COMMAND_DAMAGE, true);
                pet->CastSpell(target, SPELL_HUNTER_KILL_COMMAND_CHARGE, true);
            }
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_hun_kill_command_SpellScript::CheckCast);
            OnEffectHitTarget += SpellEffectFn(spell_hun_kill_command_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_kill_command_SpellScript();
    }
};

// 83381 - Kill Command (damage)
class spell_hun_kill_command_damage : public SpellScriptLoader
{
public:
    spell_hun_kill_command_damage() : SpellScriptLoader("spell_hun_kill_command_damage") { }

    class spell_hun_kill_command_damage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_kill_command_damage_SpellScript);

        void CalculateDamage(SpellEffIndex effIndex)
        {
            Unit* hunter = GetCaster()->GetOwner();
            if (!hunter)
                return;

            float weaponDamage = hunter->GetWeaponDamageRange(WeaponAttackType::RANGED_ATTACK, WeaponDamageRange::MAXDAMAGE);
            SetEffectValue(GetSpellInfo()->Effects[effIndex].CalcValue(hunter) + int32(weaponDamage * 0.5f));
        }

        void HandleOnHit()
        {
            if (AuraEffect const* huntersMark = GetHitUnit()->GetAuraEffect(SPELL_HUNTER_HUNTERS_MARK, EFFECT_1))
                SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), huntersMark->GetAmount()));
        }

        void Register() OVERRIDE
        {
            OnEffectLaunchTarget += SpellEffectFn(spell_hun_kill_command_damage_SpellScript::CalculateDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            OnHit += SpellHitFn(spell_hun_kill_command_damage_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_kill_command_damage_SpellScript();
    }
};

// 3044 - Arcane Shot, 34026 - Kill Command, 53209 - Chimera Shot, 53301 - Explosive Shot
class spell_hun_hunters_mark_trigger : public SpellScriptLoader
{
public:
    spell_hun_hunters_mark_trigger() : SpellScriptLoader("spell_hun_hunters_mark_trigger") { }

    class spell_hun_hunters_mark_trigger_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_hunters_mark_trigger_SpellScript);

        void HandleCast()
        {
            if (Unit* target = GetExplTargetUnit())
                if (GetCaster()->HasSpell(SPELL_HUNTER_HUNTERS_MARK))
                    GetCaster()->CastSpell(target, SPELL_HUNTER_HUNTERS_MARK, true);
        }

        void Register() OVERRIDE
        {
            OnSpellStart += SpellCastFn(spell_hun_hunters_mark_trigger_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_hunters_mark_trigger_SpellScript();
    }
};

// 19615 - Frenzy
class spell_hun_frenzy : public SpellScriptLoader
{
public:
    spell_hun_frenzy() : SpellScriptLoader("spell_hun_frenzy") { }

    class spell_hun_frenzy_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_frenzy_SpellScript);

        void HandleOnHit()
        {
            Unit* caster = GetCaster();
            Unit* owner = caster->GetOwner();
            if (!owner)
                return;

            if (Aura* frenzy = caster->GetAura(SPELL_HUNTER_FRENZY_STACKS))
                if (frenzy->GetStackAmount() >= 5)
                    owner->CastSpell(owner, SPELL_HUNTER_FOCUS_FIRE_READY, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_hun_frenzy_SpellScript::HandleOnHit);
        }
    };

    class spell_hun_frenzy_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_frenzy_AuraScript);

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* owner = GetTarget()->GetOwner())
                owner->RemoveAurasDueToSpell(SPELL_HUNTER_FOCUS_FIRE_READY);
        }

        void Register() OVERRIDE
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_hun_frenzy_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_MOD_MELEE_HASTE_3, AURA_EFFECT_HANDLE_REAL);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_frenzy_SpellScript();
    }

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_frenzy_AuraScript();
    }
};

// 82692 - Focus Fire
class spell_hun_focus_fire : public SpellScriptLoader
{
public:
    spell_hun_focus_fire() : SpellScriptLoader("spell_hun_focus_fire") { }

    class spell_hun_focus_fire_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_focus_fire_SpellScript);

        SpellCastResult CheckFrenzy()
        {
            Player* player = GetCaster()->ToPlayer();
            if (!player)
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;

            Pet* pet = player->GetPet();
            if (!pet)
                return SpellCastResult::SPELL_FAILED_NO_PET;

            if (!pet->HasAura(SPELL_HUNTER_FRENZY_STACKS))
                return SpellCastResult::SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_hun_focus_fire_SpellScript::CheckFrenzy);
        }
    };

    class spell_hun_focus_fire_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_focus_fire_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_FRENZY_ENERGIZE))
                return false;
            return true;
        }

        bool Load() OVERRIDE
        {
            Unit* caster = GetCaster();
            if (!caster || caster->GetTypeId() != TypeID::TYPEID_PLAYER)
                return false;

            Pet* pet = caster->ToPlayer()->GetPet();
            return pet && pet->HasAura(SPELL_HUNTER_FRENZY_STACKS);
        }

        uint8 GetFrenzyStacks()
        {
            if (Unit* caster = GetCaster())
                if (Player* player = caster->ToPlayer())
                    if (Pet* pet = player->GetPet())
                        if (Aura* frenzy = pet->GetAura(SPELL_HUNTER_FRENZY_STACKS))
                            return frenzy->GetStackAmount();
            return 0;
        }

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (uint8 stacks = GetFrenzyStacks())
                amount *= stacks;
        }

        void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            Player* player = caster->ToPlayer();
            Pet* pet = player ? player->GetPet() : NULL;
            if (!pet)
                return;

            Aura* frenzy = pet->GetAura(SPELL_HUNTER_FRENZY_STACKS);
            if (!frenzy)
                return;

            int32 amount = aurEff->GetAmount() * frenzy->GetStackAmount();
            caster->CastCustomSpell(pet, SPELL_HUNTER_FRENZY_ENERGIZE, &amount, NULL, NULL, true);
            pet->RemoveAurasDueToSpell(SPELL_HUNTER_FRENZY_STACKS);
            caster->RemoveAurasDueToSpell(SPELL_HUNTER_FRENZY_STACKS);
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_focus_fire_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_RANGED_HASTE);
            AfterEffectApply += AuraEffectApplyFn(spell_hun_focus_fire_AuraScript::HandleApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_focus_fire_SpellScript();
    }

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_focus_fire_AuraScript();
    }
};

// 19574 - Bestial Wrath
class spell_hun_bestial_wrath : public SpellScriptLoader
{
public:
    spell_hun_bestial_wrath() : SpellScriptLoader("spell_hun_bestial_wrath") { }

    class spell_hun_bestial_wrath_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_bestial_wrath_AuraScript);

        // A non-negative amount is consumed by absorbed damage and drops the whole aura when it reaches zero.
        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            amount = -1;
        }

        void HandleAbsorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
        {
            absorbAmount = 0;

            Pet* pet = GetTarget()->ToPet();
            Player* owner = pet ? pet->GetOwner() : NULL;
            if (!owner || !owner->HasAura(SPELL_HUNTER_GLYPH_OF_ENDLESS_WRATH))
                return;

            if (GetTarget()->GetHealth() <= dmgInfo.GetDamage())
                absorbAmount = dmgInfo.GetDamage();
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_bestial_wrath_AuraScript::CalculateAmount, EFFECT_3, SPELL_AURA_SCHOOL_ABSORB);
            OnEffectAbsorb += AuraEffectAbsorbFn(spell_hun_bestial_wrath_AuraScript::HandleAbsorb, EFFECT_3);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_bestial_wrath_AuraScript();
    }
};

// 118455 - Beast Cleave
class spell_hun_beast_cleave_proc : public SpellScriptLoader
{
public:
    spell_hun_beast_cleave_proc() : SpellScriptLoader("spell_hun_beast_cleave_proc") { }

    class spell_hun_beast_cleave_proc_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_beast_cleave_proc_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_BEAST_CLEAVE_DAMAGE))
                return false;
            return true;
        }

        void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            DamageInfo* damageInfo = eventInfo.GetDamageInfo();
            if (!damageInfo || !damageInfo->GetDamage())
                return;

            if (eventInfo.GetSpellInfo() && eventInfo.GetSpellInfo()->Id == SPELL_HUNTER_BEAST_CLEAVE_DAMAGE)
                return;

            Unit* pet = GetTarget();
            int32 pct = 75;
            if (Unit* owner = pet->GetOwner())
                if (AuraEffect const* beastCleave = owner->GetAuraEffect(SPELL_HUNTER_BEAST_CLEAVE_AURA, EFFECT_0))
                    pct = beastCleave->GetAmount();

            int32 damage = CalculatePct(int32(damageInfo->GetDamage()), pct);
            if (damage > 0)
                pet->CastCustomSpell(SPELL_HUNTER_BEAST_CLEAVE_DAMAGE, SPELLVALUE_BASE_POINT0, damage, pet, true);
        }

        void Register() OVERRIDE
        {
            OnEffectProc += AuraEffectProcFn(spell_hun_beast_cleave_proc_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_beast_cleave_proc_AuraScript();
    }
};

// 118459 - Beast Cleave (damage)
class spell_hun_beast_cleave_damage : public SpellScriptLoader
{
public:
    spell_hun_beast_cleave_damage() : SpellScriptLoader("spell_hun_beast_cleave_damage") { }

    class spell_hun_beast_cleave_damage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_beast_cleave_damage_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            Unit* caster = GetCaster();
            Unit* primaryTarget = caster->GetVictim();
            if (Spell* spell = caster->GetCurrentSpell(CURRENT_GENERIC_SPELL))
                if (Unit* spellTarget = spell->m_targets.GetUnitTarget())
                    primaryTarget = spellTarget;

            if (primaryTarget)
                targets.remove(primaryTarget);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hun_beast_cleave_damage_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_beast_cleave_damage_SpellScript();
    }
};

// 2643 - Multi-Shot
class spell_hun_multi_shot : public SpellScriptLoader
{
public:
    spell_hun_multi_shot() : SpellScriptLoader("spell_hun_multi_shot") { }

    class spell_hun_multi_shot_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_multi_shot_SpellScript);

        bool Load() OVERRIDE
        {
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void HandleOnHit()
        {
            if (Unit* target = GetHitUnit())
                if (GetCaster()->HasSpell(SPELL_HUNTER_SERPENT_SPREAD))
                    GetCaster()->CastSpell(target, SPELL_HUNTER_SERPENT_STING_AURA, true);
        }

        void HandleCast()
        {
            Player* player = GetCaster()->ToPlayer();
            if (!player->HasAura(SPELL_HUNTER_BEAST_CLEAVE_AURA))
                return;

            if (Pet* pet = player->GetPet())
                player->CastSpell(pet, SPELL_HUNTER_BEAST_CLEAVE_PROC, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_hun_multi_shot_SpellScript::HandleOnHit);
            OnCast += SpellCastFn(spell_hun_multi_shot_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_multi_shot_SpellScript();
    }
};

// 53260 - Cobra Strikes
class spell_hun_cobra_strikes : public SpellScriptLoader
{
public:
    spell_hun_cobra_strikes() : SpellScriptLoader("spell_hun_cobra_strikes") { }

    class spell_hun_cobra_strikes_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_cobra_strikes_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            return GetUnitOwner()->GetPetGUID() != 0
                && eventInfo.GetSpellInfo() && eventInfo.GetSpellInfo()->Id == SPELL_HUNTER_ARCANE_SHOT;
        }

        void HandleProc(ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            GetUnitOwner()->CastCustomSpell(SPELL_HUNTER_COBRA_STRIKES_STACKS, SPELLVALUE_AURA_STACK, 2, GetUnitOwner(), true);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_hun_cobra_strikes_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_hun_cobra_strikes_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_cobra_strikes_AuraScript();
    }
};

// 16827 - Claw, 17253 - Bite, 49966 - Smack, 145625 - Bite
class spell_hun_cobra_strikes_stacks : public SpellScriptLoader
{
public:
    spell_hun_cobra_strikes_stacks() : SpellScriptLoader("spell_hun_cobra_strikes_stacks") { }

    class spell_hun_cobra_strikes_stacks_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_cobra_strikes_stacks_SpellScript);

        void RemoveStack(Unit* auraOwner)
        {
            if (!auraOwner)
                return;

            if (Aura* aura = auraOwner->GetAura(SPELL_HUNTER_COBRA_STRIKES_STACKS))
            {
                uint8 stacks = aura->GetStackAmount();
                if (stacks < 2)
                    aura->Remove();
                else
                    aura->SetStackAmount(stacks - 1);
            }
        }

        void HandleAfterHit()
        {
            RemoveStack(GetCaster());
            RemoveStack(GetCaster()->GetOwner());
        }

        void Register() OVERRIDE
        {
            AfterHit += SpellHitFn(spell_hun_cobra_strikes_stacks_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_cobra_strikes_stacks_SpellScript();
    }
};

// 82726 - Fervor
class spell_hun_fervor : public SpellScriptLoader
{
public:
    spell_hun_fervor() : SpellScriptLoader("spell_hun_fervor") { }

    class spell_hun_fervor_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_fervor_SpellScript);

        void HandleCast()
        {
            if (Player* player = GetCaster()->ToPlayer())
                if (Pet* pet = player->GetPet())
                    pet->CastSpell(pet, GetSpellInfo()->Id, true);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_hun_fervor_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_fervor_SpellScript();
    }
};

// 53353 - Chimera Shot (heal)
class spell_hun_chimera_shot_heal : public SpellScriptLoader
{
public:
    spell_hun_chimera_shot_heal() : SpellScriptLoader("spell_hun_chimera_shot_heal") { }

    class spell_hun_chimera_shot_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_chimera_shot_heal_SpellScript);

        void HandleHeal(SpellEffIndex /*effIndex*/)
        {
            if (AuraEffect const* glyph = GetCaster()->GetAuraEffect(SPELL_HUNTER_GLYPH_OF_CHIMERA_SHOT, EFFECT_0))
                SetEffectValue(GetEffectValue() + glyph->GetAmount());
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_hun_chimera_shot_heal_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL_PCT);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_chimera_shot_heal_SpellScript();
    }
};

// 34487 - Master Marksman
class spell_hun_master_marksman : public SpellScriptLoader
{
public:
    spell_hun_master_marksman() : SpellScriptLoader("spell_hun_master_marksman") { }

    class spell_hun_master_marksman_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_master_marksman_AuraScript);

        bool CheckProc(ProcEventInfo& /*eventInfo*/)
        {
            return !GetUnitOwner()->HasAura(SPELL_HUNTER_FIRE);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_hun_master_marksman_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_master_marksman_AuraScript();
    }
};

// 53224 - Steady Focus
class spell_hun_steady_focus : public SpellScriptLoader
{
public:
    spell_hun_steady_focus() : SpellScriptLoader("spell_hun_steady_focus") { }

    class spell_hun_steady_focus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_steady_focus_AuraScript);

        uint32 _steadyShotCount;

        bool Load() OVERRIDE
        {
            _steadyShotCount = 0;
            return true;
        }

        void HandleProc(ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            if (!spellInfo || spellInfo->Id == SPELL_HUNTER_AUTO_SHOT || (eventInfo.GetHitMask() & PROC_EX_INTERNAL_TRIGGERED))
                return;

            if (spellInfo->Id != SPELL_HUNTER_STEADY_SHOT)
            {
                _steadyShotCount = 0;
                return;
            }

            if (++_steadyShotCount >= 2)
            {
                _steadyShotCount = 0;
                GetTarget()->CastSpell(GetTarget(), SPELL_HUNTER_STEADY_FOCUS, true);
            }
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_hun_steady_focus_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_steady_focus_AuraScript();
    }
};

// 76659 - Mastery: Wild Quiver
class spell_hun_wild_quiver : public SpellScriptLoader
{
public:
    spell_hun_wild_quiver() : SpellScriptLoader("spell_hun_wild_quiver") { }

    class spell_hun_wild_quiver_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_wild_quiver_AuraScript);

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            Unit* target = eventInfo.GetActionTarget();
            if (!target)
                return;

            Unit* hunter = GetTarget();
            float chance = float(aurEff->GetAmount());
            if (hunter->GetTypeId() == TypeID::TYPEID_PLAYER)
            {
                float bonusMultiplier = GetSpellInfo()->Effects[aurEff->GetEffIndex()].BonusMultiplier;
                if (bonusMultiplier > 0.0f)
                    chance = hunter->GetFloatValue(PLAYER_FIELD_MASTERY) * bonusMultiplier;
            }

            if (eventInfo.GetSpellInfo() && eventInfo.GetSpellInfo()->Id == SPELL_HUNTER_BARRAGE)
                chance /= 5.0f;

            if (roll_chance_f(chance))
                hunter->CastSpell(target, SPELL_HUNTER_WILD_QUIVER, true, NULL, aurEff);
        }

        void Register() OVERRIDE
        {
            OnEffectProc += AuraEffectProcFn(spell_hun_wild_quiver_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_wild_quiver_AuraScript();
    }
};

// 53302 - Sniper Training
class spell_hun_sniper_training : public SpellScriptLoader
{
public:
    spell_hun_sniper_training() : SpellScriptLoader("spell_hun_sniper_training") { }

    class spell_hun_sniper_training_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_sniper_training_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_SNIPER_TRAINING_BUFF))
                return false;
            return true;
        }

        void HandlePeriodic(AuraEffect const* aurEff)
        {
            PreventDefaultAction();
            if (aurEff->GetAmount() > 0)
                return;

            Unit* target = GetTarget();
            if (target->HasAura(SPELL_HUNTER_SNIPER_TRAINING_BUFF))
                return;

            SpellInfo const* buffInfo = sSpellMgr->GetSpellInfo(SPELL_HUNTER_SNIPER_TRAINING_BUFF);
            Unit* triggerCaster = buffInfo->NeedsToBeTriggeredByCaster(GetSpellInfo()) ? GetCaster() : target;
            if (triggerCaster)
                triggerCaster->CastSpell(target, buffInfo, true, NULL, aurEff);
        }

        void HandleUpdatePeriodic(AuraEffect* aurEff)
        {
            Player* player = GetUnitOwner()->ToPlayer();
            if (!player)
                return;

            int32 amount = player->isMoving()
                ? player->CalculateSpellDamage(player, GetSpellInfo(), aurEff->GetEffIndex())
                : aurEff->GetAmount() - 1;
            aurEff->SetAmount(amount);
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_hun_sniper_training_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            OnEffectUpdatePeriodic += AuraEffectUpdatePeriodicFn(spell_hun_sniper_training_AuraScript::HandleUpdatePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_sniper_training_AuraScript();
    }
};

// 56343 - Lock and Load
class spell_hun_lock_and_load : public SpellScriptLoader
{
public:
    spell_hun_lock_and_load() : SpellScriptLoader("spell_hun_lock_and_load") { }

    class spell_hun_lock_and_load_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_lock_and_load_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (eventInfo.GetTypeMask() & PROC_FLAG_DONE_PERIODIC)
                return roll_chance_i(20);
            return true;
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_hun_lock_and_load_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_lock_and_load_AuraScript();
    }
};

// 56453 - Lock and Load (proc)
class spell_hun_lock_and_load_cooldown : public SpellScriptLoader
{
public:
    spell_hun_lock_and_load_cooldown() : SpellScriptLoader("spell_hun_lock_and_load_cooldown") { }

    class spell_hun_lock_and_load_cooldown_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_lock_and_load_cooldown_SpellScript);

        void HandleOnHit()
        {
            if (Player* player = GetCaster()->ToPlayer())
                player->RemoveSpellCooldown(SPELL_HUNTER_EXPLOSIVE_SHOT, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_hun_lock_and_load_cooldown_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_lock_and_load_cooldown_SpellScript();
    }
};

// 13812 - Explosive Trap
class spell_hun_explosive_trap : public SpellScriptLoader
{
public:
    spell_hun_explosive_trap() : SpellScriptLoader("spell_hun_explosive_trap") { }

    class spell_hun_explosive_trap_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_explosive_trap_SpellScript);

        bool HasGlyph()
        {
            Unit* hunter = GetCaster()->GetCharmerOrOwnerOrSelf();
            return hunter && hunter->HasAura(SPELL_HUNTER_GLYPH_OF_EXPLOSIVE_TRAP);
        }

        void ResolveDamage(SpellEffIndex effIndex)
        {
            if (HasGlyph())
                PreventHitDefaultEffect(effIndex);
        }

        void ResolveKnockback(SpellEffIndex effIndex)
        {
            if (!HasGlyph())
                PreventHitDefaultEffect(effIndex);
        }

        void Register() OVERRIDE
        {
            OnEffectLaunchTarget += SpellEffectFn(spell_hun_explosive_trap_SpellScript::ResolveDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            OnEffectLaunch += SpellEffectFn(spell_hun_explosive_trap_SpellScript::ResolveKnockback, EFFECT_2, SPELL_EFFECT_TRIGGER_SPELL);
            OnEffectLaunchTarget += SpellEffectFn(spell_hun_explosive_trap_SpellScript::ResolveKnockback, EFFECT_2, SPELL_EFFECT_TRIGGER_SPELL);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_explosive_trap_SpellScript();
    }
};

// 19387 - Entrapment
class spell_hun_entrapment : public SpellScriptLoader
{
public:
    spell_hun_entrapment() : SpellScriptLoader("spell_hun_entrapment") { }

    class spell_hun_entrapment_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_entrapment_AuraScript);

        void HandleProc(ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            Unit* actor = eventInfo.GetActor();
            Unit* target = eventInfo.GetActionTarget();
            if (actor && target)
                actor->CastSpell(target, SPELL_HUNTER_ENTRAPMENT, true);
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_hun_entrapment_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_entrapment_AuraScript();
    }
};

// 3355 - Freezing Trap, 19503 - Scatter Shot
class spell_hun_crowd_control_dot_hack : public SpellScriptLoader
{
public:
    spell_hun_crowd_control_dot_hack() : SpellScriptLoader("spell_hun_crowd_control_dot_hack") { }

    class spell_hun_crowd_control_dot_hack_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_crowd_control_dot_hack_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (!(eventInfo.GetTypeMask() & PROC_FLAG_TAKEN_PERIODIC))
                return true;

            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            return !spellInfo || (spellInfo->Id != SPELL_HUNTER_PIERCING_SHOTS && spellInfo->Id != SPELL_HUNTER_EXPLOSIVE_SHOT);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_hun_crowd_control_dot_hack_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_crowd_control_dot_hack_AuraScript();
    }
};

// 16827 - Claw, 17253 - Bite, 49966 - Smack
class spell_hun_pet_basic_attack : public SpellScriptLoader
{
public:
    spell_hun_pet_basic_attack() : SpellScriptLoader("spell_hun_pet_basic_attack") { }

    class spell_hun_pet_basic_attack_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_pet_basic_attack_SpellScript);

        void CalculateDamage(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* hunter = caster->GetOwner();
            if (!hunter)
                return;

            float damage = float(GetEffectValue()) + hunter->GetTotalAttackPowerValue(WeaponAttackType::RANGED_ATTACK) * 0.168f;
            if (AuraEffect const* spikedCollar = caster->GetAuraEffect(SPELL_HUNTER_SPIKED_COLLAR, EFFECT_0))
                AddPct(damage, spikedCollar->GetAmount());

            // The ability cost has already been paid when the launch phase runs.
            int32 cost = GetSpell()->GetPowerCost();
            if (cost > 0 && caster->GetPower(POWER_FOCUS) >= cost)
            {
                damage *= 2.0f;
                caster->ModifyPower(POWER_FOCUS, -cost);
            }

            SetEffectValue(int32(damage));
        }

        void Register() OVERRIDE
        {
            OnEffectLaunchTarget += SpellEffectFn(spell_hun_pet_basic_attack_SpellScript::CalculateDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_pet_basic_attack_SpellScript();
    }
};

// 16827 - Claw, 17253 - Bite, 49966 - Smack, 145625 - Bite
class spell_hun_blink_strikes : public SpellScriptLoader
{
public:
    spell_hun_blink_strikes() : SpellScriptLoader("spell_hun_blink_strikes") { }

    class spell_hun_blink_strikes_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_blink_strikes_SpellScript);

        // Pet spells run the cast check twice; only the first pass may decide to teleport.
        bool _checked;
        bool _teleport;

        bool Load() OVERRIDE
        {
            _checked = false;
            _teleport = false;
            return GetCaster()->IsPet();
        }

        SpellCastResult CheckCast()
        {
            Unit* target = GetExplTargetUnit();
            if (!target)
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;

            if (_checked)
                return SpellCastResult::SPELL_CAST_OK;
            _checked = true;

            Pet* pet = GetCaster()->ToPet();
            Player* hunter = pet->GetOwner();
            if (!hunter || !hunter->HasAura(SPELL_HUNTER_BLINK_STRIKES))
                return SpellCastResult::SPELL_CAST_OK;

            if (pet->IsWithinMeleeRange(target))
                return SpellCastResult::SPELL_CAST_OK;

            if (pet->HasAuraType(SPELL_AURA_MOD_ROOT) || target->IsWithinDist(pet, 10.0f) || hunter->HasSpellCooldown(SPELL_HUNTER_BLINK_STRIKE))
                return SpellCastResult::SPELL_FAILED_OUT_OF_RANGE;

            _teleport = true;
            return SpellCastResult::SPELL_CAST_OK;
        }

        void HandleOnHit()
        {
            if (!_teleport)
                return;

            Pet* pet = GetCaster()->ToPet();
            Player* hunter = pet->GetOwner();
            Unit* target = GetHitUnit();
            if (!hunter || !target)
                return;

            pet->CastSpell(target, SPELL_HUNTER_BLINK_STRIKE, true);
            hunter->AddSpellCooldown(SPELL_HUNTER_BLINK_STRIKE, 0, time(NULL) + 20);
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_hun_blink_strikes_SpellScript::CheckCast);
            OnHit += SpellHitFn(spell_hun_blink_strikes_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_blink_strikes_SpellScript();
    }
};

// 50285 - Dust Cloud
class spell_hun_pet_dust_cloud : public SpellScriptLoader
{
public:
    spell_hun_pet_dust_cloud() : SpellScriptLoader("spell_hun_pet_dust_cloud") { }

    class spell_hun_pet_dust_cloud_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_pet_dust_cloud_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Unit* target = GetHitUnit())
                for (uint8 i = 0; i < 3; ++i)
                    GetCaster()->CastSpell(target, SPELL_HUNTER_WEAKENED_ARMOR, true);
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_hun_pet_dust_cloud_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_pet_dust_cloud_SpellScript();
    }
};

// 95725 - Froststorm Breath, 63900 - Thunderstomp, 95714 - Burrow Attack
class spell_hun_pet_ap_scaling_damage : public SpellScriptLoader
{
public:
    spell_hun_pet_ap_scaling_damage() : SpellScriptLoader("spell_hun_pet_ap_scaling_damage") { }

    class spell_hun_pet_ap_scaling_damage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_pet_ap_scaling_damage_SpellScript);

        float GetCoefficient()
        {
            switch (GetSpellInfo()->Id)
            {
                case 95725: return 0.144f;          // Froststorm Breath
                case 63900: return 0.03428f;        // Thunderstomp
                case 95714: return 0.4f * 0.288f;   // Burrow Attack
                default:    return 0.0f;
            }
        }

        void CalculateDamage(SpellEffIndex /*effIndex*/)
        {
            if (Unit* hunter = GetCaster()->GetOwner())
                SetEffectValue(GetEffectValue() + int32(hunter->GetTotalAttackPowerValue(WeaponAttackType::RANGED_ATTACK) * GetCoefficient()));
        }

        void Register() OVERRIDE
        {
            OnEffectLaunchTarget += SpellEffectFn(spell_hun_pet_ap_scaling_damage_SpellScript::CalculateDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_pet_ap_scaling_damage_SpellScript();
    }
};

// 90361 - Spirit Mend
class spell_hun_spirit_mend : public SpellScriptLoader
{
public:
    spell_hun_spirit_mend() : SpellScriptLoader("spell_hun_spirit_mend") { }

    class spell_hun_spirit_mend_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_spirit_mend_SpellScript);

        void CalculateHeal(SpellEffIndex /*effIndex*/)
        {
            if (Unit* hunter = GetCaster()->GetOwner())
            {
                float rap = hunter->GetTotalAttackPowerValue(WeaponAttackType::RANGED_ATTACK);
                SetEffectValue(GetEffectValue() + int32(rap * 0.35f * 0.5f + rap * 0.61f * 0.5f));
            }
        }

        void Register() OVERRIDE
        {
            OnEffectLaunchTarget += SpellEffectFn(spell_hun_spirit_mend_SpellScript::CalculateHeal, EFFECT_0, SPELL_EFFECT_HEAL);
        }
    };

    class spell_hun_spirit_mend_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_spirit_mend_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* caster = GetCaster())
            {
                if (Unit* hunter = caster->GetOwner())
                {
                    float rap = hunter->GetTotalAttackPowerValue(WeaponAttackType::RANGED_ATTACK);
                    amount += int32(rap * 0.35f * 0.335f + rap * 0.61f * 0.335f);
                }
            }
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_spirit_mend_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_PERIODIC_HEAL);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_spirit_mend_SpellScript();
    }

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_spirit_mend_AuraScript();
    }
};

// 128997 - Spirit Beast Blessing
class spell_hun_spirit_beast_blessing : public SpellScriptLoader
{
public:
    spell_hun_spirit_beast_blessing() : SpellScriptLoader("spell_hun_spirit_beast_blessing") { }

    class spell_hun_spirit_beast_blessing_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_spirit_beast_blessing_SpellScript);

        void HandleCast()
        {
            GetCaster()->CastSpell(GetCaster(), SPELL_HUNTER_SPIRIT_BEAST_BLESSING, true);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_hun_spirit_beast_blessing_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_spirit_beast_blessing_SpellScript();
    }
};

// 120697 - Lynx Rush
class spell_hun_lynx_rush : public SpellScriptLoader
{
public:
    spell_hun_lynx_rush() : SpellScriptLoader("spell_hun_lynx_rush") { }

    class spell_hun_lynx_rush_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_lynx_rush_AuraScript);

        std::set<uint64> _targets;

        void HandleTick(AuraEffect const* /*aurEff*/)
        {
            Unit* pet = GetTarget();

            Unit* target = NULL;
            while (!_targets.empty())
            {
                uint64 guid = Skyfire::Containers::SelectRandomContainerElement(_targets);
                target = ObjectAccessor::GetUnit(*pet, guid);
                if (target && target->IsAlive())
                    break;
                target = NULL;
                _targets.erase(guid);
            }

            if (!target)
                return;

            float x, y, z;
            target->GetRandomContactPoint(pet, x, y, z, 5.0f, 5.0f);
            pet->GetMotionMaster()->MoveCharge(x, y, z);
            pet->CastSpell(target, SPELL_HUNTER_LYNX_RUSH_DOT, true);
        }

    public:
        void AddTarget(Unit* target)
        {
            _targets.insert(target->GetGUID());
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_hun_lynx_rush_AuraScript::HandleTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    class spell_hun_lynx_rush_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_lynx_rush_SpellScript);

        void SelectTargets(std::list<WorldObject*>& targets)
        {
            if (Guardian* pet = GetCaster()->GetGuardianPet())
                targets.remove_if([pet](WorldObject const* target) { return !target->IsWithinDist(pet, 10.0f); });

            if (targets.empty())
                FinishCast(SpellCastResult::SPELL_FAILED_OUT_OF_RANGE);
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* target = GetHitUnit();
            Guardian* pet = GetCaster()->GetGuardianPet();
            if (!target || !pet)
                return;

            if (Aura* lynxRush = pet->GetAura(SPELL_HUNTER_LYNX_RUSH))
                if (spell_hun_lynx_rush_AuraScript* script = dynamic_cast<spell_hun_lynx_rush_AuraScript*>(lynxRush->GetScriptByName("spell_hun_lynx_rush")))
                    script->AddTarget(target);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hun_lynx_rush_SpellScript::SelectTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_hun_lynx_rush_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_lynx_rush_AuraScript();
    }

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_lynx_rush_SpellScript();
    }
};

// 120699 - Lynx Rush (DoT)
class spell_hun_lynx_rush_dot : public SpellScriptLoader
{
public:
    spell_hun_lynx_rush_dot() : SpellScriptLoader("spell_hun_lynx_rush_dot") { }

    class spell_hun_lynx_rush_dot_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_lynx_rush_dot_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* pet = GetCaster())
                if (Unit* hunter = pet->GetOwner())
                    amount += int32(CalculatePct(hunter->GetTotalAttackPowerValue(WeaponAttackType::RANGED_ATTACK), 19.0f) / 5.0f);
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_lynx_rush_dot_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_lynx_rush_dot_AuraScript();
    }
};

// 118694 - Spirit Bond
class spell_hun_spirit_bond : public SpellScriptLoader
{
public:
    spell_hun_spirit_bond() : SpellScriptLoader("spell_hun_spirit_bond") { }

    class spell_hun_spirit_bond_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_spirit_bond_AuraScript);

        void HandleTick(AuraEffect const* /*aurEff*/)
        {
            if (GetTarget() == GetUnitOwner())
                GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_HUNTER_SPIRIT_BOND_HEAL, true);
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_hun_spirit_bond_AuraScript::HandleTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_spirit_bond_AuraScript();
    }
};

// 781 - Disengage
class spell_hun_disengage : public SpellScriptLoader
{
public:
    spell_hun_disengage() : SpellScriptLoader("spell_hun_disengage") { }

    class spell_hun_disengage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_disengage_SpellScript);

        void HandleCast()
        {
            Unit* caster = GetCaster();
            if (caster->HasAura(SPELL_HUNTER_POSTHASTE))
            {
                caster->RemoveMovementImpairingAuras();
                caster->CastSpell(caster, SPELL_HUNTER_POSTHASTE_SPEED, true);
            }

            if (caster->HasAura(SPELL_HUNTER_NARROW_ESCAPE))
                caster->CastSpell(caster, SPELL_HUNTER_NARROW_ESCAPE_SELECTOR, true);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_hun_disengage_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_disengage_SpellScript();
    }
};

// 115928 - Narrow Escape
class spell_hun_narrow_escape : public SpellScriptLoader
{
public:
    spell_hun_narrow_escape() : SpellScriptLoader("spell_hun_narrow_escape") { }

    class spell_hun_narrow_escape_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_narrow_escape_SpellScript);

        void HandleOnHit()
        {
            if (Unit* target = GetHitUnit())
                GetCaster()->CastSpell(target, SPELL_HUNTER_NARROW_ESCAPE_ROOT, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_hun_narrow_escape_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_narrow_escape_SpellScript();
    }
};

// 113073 - Dash
class spell_hun_dash : public SpellScriptLoader
{
public:
    spell_hun_dash() : SpellScriptLoader("spell_hun_dash") { }

    class spell_hun_dash_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_dash_SpellScript);

        void HandleOnHit()
        {
            GetCaster()->RemoveMovementImpairingAuras();
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_hun_dash_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_dash_SpellScript();
    }
};

// 51753 - Camouflage
class spell_hun_camouflage_driver : public SpellScriptLoader
{
public:
    spell_hun_camouflage_driver() : SpellScriptLoader("spell_hun_camouflage_driver") { }

    class spell_hun_camouflage_driver_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_camouflage_driver_AuraScript);

        bool Load() OVERRIDE
        {
            return GetOwner()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Player* hunter = GetTarget()->ToPlayer();
            bool inCombat = hunter->IsInCombat();
            if (inCombat)
                SetDuration(6 * IN_MILLISECONDS);

            std::list<Unit*> targets;
            targets.push_back(hunter);
            if (Pet* pet = hunter->GetPet())
                targets.push_back(pet);

            for (Unit* target : targets)
            {
                target->CastSpell(target, SPELL_HUNTER_CAMOUFLAGE_AURA, true);
                if (inCombat)
                    if (Aura* aura = target->GetAura(SPELL_HUNTER_CAMOUFLAGE_AURA))
                        aura->SetDuration(6 * IN_MILLISECONDS);
                target->CastSpell(target, SPELL_HUNTER_CAMOUFLAGE_INVISIBILITY_DRIVER, true);
            }
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Player* hunter = GetTarget()->ToPlayer();

            std::list<Unit*> targets;
            targets.push_back(hunter);
            if (Pet* pet = hunter->GetPet())
                targets.push_back(pet);

            for (Unit* target : targets)
            {
                target->RemoveAurasDueToSpell(SPELL_HUNTER_CAMOUFLAGE_AURA);
                target->RemoveAurasDueToSpell(SPELL_HUNTER_CAMOUFLAGE_INVISIBILITY_DRIVER);
                target->RemoveAurasDueToSpell(SPELL_HUNTER_CAMOUFLAGE_INVISIBILITY);
                target->RemoveAurasDueToSpell(SPELL_HUNTER_CAMOUFLAGE_INVISIBILITY_GLYPHED);
            }
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_hun_camouflage_driver_AuraScript::HandleApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_hun_camouflage_driver_AuraScript::HandleRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_camouflage_driver_AuraScript();
    }
};

// 51755 - Camouflage
class spell_hun_camouflage : public SpellScriptLoader
{
public:
    spell_hun_camouflage() : SpellScriptLoader("spell_hun_camouflage") { }

    class spell_hun_camouflage_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_camouflage_AuraScript);

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            Player* hunter = target->ToPlayer();
            if (!hunter)
                if (Pet* pet = target->ToPet())
                    hunter = pet->GetOwner();

            if (hunter)
                hunter->RemoveAurasDueToSpell(SPELL_HUNTER_CAMOUFLAGE_DRIVER);
        }

        void Register() OVERRIDE
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_hun_camouflage_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_INTERFERE_TARGETTING, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_camouflage_AuraScript();
    }
};

// 80326 - Camouflage
class spell_hun_camouflage_invisibility_driver : public SpellScriptLoader
{
public:
    spell_hun_camouflage_invisibility_driver() : SpellScriptLoader("spell_hun_camouflage_invisibility_driver") { }

    class spell_hun_camouflage_invisibility_driver_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_camouflage_invisibility_driver_AuraScript);

        void HandleTick(AuraEffect const* /*aurEff*/)
        {
            Unit* target = GetTarget();
            Player* hunter = target->GetAffectingPlayer();
            if (!hunter)
                return;

            if (hunter->HasAura(SPELL_HUNTER_GLYPH_OF_CAMOUFLAGE))
            {
                if (!target->HasAura(SPELL_HUNTER_CAMOUFLAGE_INVISIBILITY_GLYPHED))
                    target->CastSpell(target, SPELL_HUNTER_CAMOUFLAGE_INVISIBILITY_GLYPHED, true);
            }
            else if (!target->isMoving())
            {
                if (!target->HasAura(SPELL_HUNTER_CAMOUFLAGE_INVISIBILITY))
                    target->CastSpell(target, SPELL_HUNTER_CAMOUFLAGE_INVISIBILITY, true);
            }
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_hun_camouflage_invisibility_driver_AuraScript::HandleTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_camouflage_invisibility_driver_AuraScript();
    }
};

// 125042 - Glyph of Aspect of the Beast
class spell_hun_glyph_of_aspect_of_the_beast : public SpellScriptLoader
{
public:
    spell_hun_glyph_of_aspect_of_the_beast() : SpellScriptLoader("spell_hun_glyph_of_aspect_of_the_beast") { }

    class spell_hun_glyph_of_aspect_of_the_beast_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_glyph_of_aspect_of_the_beast_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* player = GetTarget()->ToPlayer())
                player->learnSpell(SPELL_HUNTER_ASPECT_OF_THE_BEAST, false);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* player = GetTarget()->ToPlayer())
                if (player->HasSpell(SPELL_HUNTER_ASPECT_OF_THE_BEAST))
                    player->removeSpell(SPELL_HUNTER_ASPECT_OF_THE_BEAST, false, false);
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_hun_glyph_of_aspect_of_the_beast_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_hun_glyph_of_aspect_of_the_beast_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_glyph_of_aspect_of_the_beast_AuraScript();
    }
};

// 118424 - Tracking
class spell_hun_tracking : public SpellScriptLoader
{
public:
    spell_hun_tracking() : SpellScriptLoader("spell_hun_tracking") { }

    class spell_hun_tracking_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_tracking_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Player* player = GetTarget()->ToPlayer();
            if (!player)
                return;

            static uint32 const trackingSpells[] =
            {
                SPELL_HUNTER_TRACK_BEASTS, SPELL_HUNTER_TRACK_DEMONS, SPELL_HUNTER_TRACK_DRAGONKIN, SPELL_HUNTER_TRACK_ELEMENTALS,
                SPELL_HUNTER_TRACK_GIANTS, SPELL_HUNTER_TRACK_HUMANOIDS, SPELL_HUNTER_TRACK_UNDEAD, SPELL_HUNTER_TRACK_HIDDEN
            };

            for (uint32 spellId : trackingSpells)
                if (!player->HasSpell(spellId))
                    player->learnSpell(spellId, false);
        }

        void Register() OVERRIDE
        {
            AfterEffectApply += AuraEffectApplyFn(spell_hun_tracking_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_tracking_AuraScript();
    }
};

// 136 - Mend Pet
class spell_hun_glyph_of_mend_pet : public SpellScriptLoader
{
public:
    spell_hun_glyph_of_mend_pet() : SpellScriptLoader("spell_hun_glyph_of_mend_pet") { }

    class spell_hun_glyph_of_mend_pet_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_glyph_of_mend_pet_AuraScript);

        void HandleTick(AuraEffect const* /*aurEff*/)
        {
            if (Unit* hunter = GetCaster())
                if (hunter->HasAura(SPELL_HUNTER_GLYPH_OF_MEND_PET))
                    hunter->CastSpell(GetTarget(), SPELL_HUNTER_GLYPH_OF_MEND_PET_DISPEL, true);
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_hun_glyph_of_mend_pet_AuraScript::HandleTick, EFFECT_0, SPELL_AURA_OBS_MOD_HEALTH);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_glyph_of_mend_pet_AuraScript();
    }
};

// 5118 - Aspect of the Cheetah
class spell_hun_aspect_of_the_cheetah : public SpellScriptLoader
{
public:
    spell_hun_aspect_of_the_cheetah() : SpellScriptLoader("spell_hun_aspect_of_the_cheetah") { }

    class spell_hun_aspect_of_the_cheetah_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_aspect_of_the_cheetah_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
                return !spellInfo->IsPositive();
            return true;
        }

        void HandleProc(ProcEventInfo& /*eventInfo*/)
        {
            Unit* owner = GetUnitOwner();
            if (!owner->HasAura(SPELL_HUNTER_GLYPH_OF_ASPECT_OF_THE_CHEETAH))
                return;

            PreventDefaultAction();
            owner->CastSpell(owner, SPELL_HUNTER_ASPECTS_COOLDOWN, TriggerCastFlags(TRIGGERED_FULL_MASK & ~TRIGGERED_IGNORE_SPELL_AND_CATEGORY_CD));
            Remove();
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_hun_aspect_of_the_cheetah_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_hun_aspect_of_the_cheetah_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_aspect_of_the_cheetah_AuraScript();
    }
};

// 5118 - Aspect of the Cheetah, 13159 - Aspect of the Pack, 13165 - Aspect of the Hawk,
// 61648 - Aspect of the Beast, 109260 - Aspect of the Iron Hawk
class spell_hun_glyph_of_the_aspects : public SpellScriptLoader
{
public:
    spell_hun_glyph_of_the_aspects() : SpellScriptLoader("spell_hun_glyph_of_the_aspects") { }

    class spell_hun_glyph_of_the_aspects_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_glyph_of_the_aspects_SpellScript);

        bool Load() OVERRIDE
        {
            return GetCaster()->HasAura(SPELL_HUNTER_GLYPH_OF_ASPECTS);
        }

        void HandleCast()
        {
            static uint32 const aspectSummonSpells[] = { 122487, 122489, 122490, 130221 };

            Unit* caster = GetCaster();
            for (uint32 summonSpellId : aspectSummonSpells)
            {
                SpellInfo const* summonInfo = sSpellMgr->GetSpellInfo(summonSpellId);
                if (!summonInfo)
                    continue;

                std::list<Creature*> minions;
                caster->GetAllMinionsByEntry(minions, summonInfo->Effects[EFFECT_0].MiscValue);
                for (Creature* minion : minions)
                    if (TempSummon* summon = minion->ToTempSummon())
                        summon->UnSummon();
            }

            uint32 summonSpellId = 0;
            switch (GetSpellInfo()->Id)
            {
                case 13165:     // Aspect of the Hawk
                case 109260:    // Aspect of the Iron Hawk
                    summonSpellId = 122487;
                    break;
                case 5118:      // Aspect of the Cheetah
                    summonSpellId = 122489;
                    break;
                case 13159:     // Aspect of the Pack
                    summonSpellId = 122490;
                    break;
                case 61648:     // Aspect of the Beast
                    summonSpellId = 130221;
                    break;
                default:
                    return;
            }

            caster->CastSpell(caster, summonSpellId, true);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_hun_glyph_of_the_aspects_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_glyph_of_the_aspects_SpellScript();
    }
};

// 19263, 67801, 110617, 110618, 148467 - Deterrence
class spell_hun_deterrence : public SpellScriptLoader
{
public:
    spell_hun_deterrence() : SpellScriptLoader("spell_hun_deterrence") { }

    class spell_hun_deterrence_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_deterrence_AuraScript);

        // -100% attacker hit chance can still be overcome by the attacker's own hit bonus.
        void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
        {
            switch (aurEff->GetAuraType())
            {
                case SPELL_AURA_MOD_ATTACKER_MELEE_HIT_CHANCE:
                case SPELL_AURA_MOD_ATTACKER_RANGED_HIT_CHANCE:
                    amount *= 2;
                    break;
                default:
                    break;
            }
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_deterrence_AuraScript::CalculateAmount, EFFECT_ALL, SPELL_AURA_ANY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_deterrence_AuraScript();
    }
};

// 19263, 148467 - Deterrence
class spell_hun_glyph_of_deterrence : public SpellScriptLoader
{
public:
    spell_hun_glyph_of_deterrence() : SpellScriptLoader("spell_hun_glyph_of_deterrence") { }

    class spell_hun_glyph_of_deterrence_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_glyph_of_deterrence_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (AuraEffect const* glyph = GetUnitOwner()->GetAuraEffect(SPELL_HUNTER_GLYPH_OF_DETERRENCE, EFFECT_0))
                amount += glyph->GetAmount();
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_hun_glyph_of_deterrence_AuraScript::CalculateAmount, EFFECT_4, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_glyph_of_deterrence_AuraScript();
    }
};

// 2641 - Dismiss Pet
class spell_hun_dismiss_pet : public SpellScriptLoader
{
public:
    spell_hun_dismiss_pet() : SpellScriptLoader("spell_hun_dismiss_pet") { }

    class spell_hun_dismiss_pet_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_dismiss_pet_SpellScript);

        SpellCastResult CheckCast()
        {
            if (Player* player = GetCaster()->ToPlayer())
                if (Battleground* bg = player->GetBattleground())
                    if (bg->isArena() && bg->GetStatus() == STATUS_IN_PROGRESS)
                        return SpellCastResult::SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_hun_dismiss_pet_SpellScript::CheckCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_dismiss_pet_SpellScript();
    }
};

// 127933 - Fireworks
class spell_hun_fireworks : public SpellScriptLoader
{
public:
    spell_hun_fireworks() : SpellScriptLoader("spell_hun_fireworks") { }

    class spell_hun_fireworks_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_fireworks_SpellScript);

        void HandleCast()
        {
            uint32 mainFirework = roll_chance_i(50) ? 127936 : 127961; // Red Streak or Blue Dots
            uint32 sideFirework = roll_chance_i(50) ? 127951 : 127937; // Yellow Rose or Roman Candle

            GetCaster()->CastSpell(GetCaster(), mainFirework, true);
            GetCaster()->CastSpell(GetCaster(), sideFirework, true);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_hun_fireworks_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_fireworks_SpellScript();
    }
};

// 57870 - Glyph of Lesser Proportion
class spell_hun_glyph_of_lesser_proportion : public SpellScriptLoader
{
public:
    spell_hun_glyph_of_lesser_proportion() : SpellScriptLoader("spell_hun_glyph_of_lesser_proportion") { }

    class spell_hun_glyph_of_lesser_proportion_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_glyph_of_lesser_proportion_AuraScript);

        void ApplyToPet()
        {
            if (Guardian* pet = GetUnitOwner()->GetGuardianPet())
                if (!pet->HasAura(SPELL_HUNTER_LESSER_PROPORTION))
                    pet->CastSpell(pet, SPELL_HUNTER_LESSER_PROPORTION, true);
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            ApplyToPet();
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Guardian* pet = GetUnitOwner()->GetGuardianPet())
                pet->RemoveAurasDueToSpell(SPELL_HUNTER_LESSER_PROPORTION);
        }

        void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            ApplyToPet();
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_hun_glyph_of_lesser_proportion_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_hun_glyph_of_lesser_proportion_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectProc += AuraEffectProcFn(spell_hun_glyph_of_lesser_proportion_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_glyph_of_lesser_proportion_AuraScript();
    }
};

// 144637 - Item - Hunter T16 2P Bonus
class spell_hun_t16_2p_bonus : public SpellScriptLoader
{
public:
    spell_hun_t16_2p_bonus() : SpellScriptLoader("spell_hun_t16_2p_bonus") { }

    class spell_hun_t16_2p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_t16_2p_bonus_AuraScript);

        bool Load() OVERRIDE
        {
            return GetOwner()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void HandleProc(ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();

            Player* player = GetTarget()->ToPlayer();
            uint8 effIndex;
            switch (player->GetTalentSpecialization(player->GetActiveSpec()))
            {
                case SPEC_HUNTER_BEAST_MASTERY: effIndex = EFFECT_1; break;
                case SPEC_HUNTER_MARKSMANSHIP:  effIndex = EFFECT_2; break;
                default:                        effIndex = EFFECT_3; break;
            }

            player->ModifySpellCooldown(SPELL_HUNTER_RAPID_FIRE, -(GetSpellInfo()->Effects[effIndex].BasePoints * IN_MILLISECONDS));
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_hun_t16_2p_bonus_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_t16_2p_bonus_AuraScript();
    }
};

// 144641 - Item - Hunter T16 4P Bonus
class spell_hun_t16_4p_bonus : public SpellScriptLoader
{
public:
    spell_hun_t16_4p_bonus() : SpellScriptLoader("spell_hun_t16_4p_bonus") { }

    class spell_hun_t16_4p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_t16_4p_bonus_AuraScript);

        bool Load() OVERRIDE
        {
            return GetOwner()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            Player* player = GetTarget()->ToPlayer();
            switch (player->GetTalentSpecialization(player->GetActiveSpec()))
            {
                case SPEC_HUNTER_MARKSMANSHIP:
                {
                    // Instant Aimed Shots only
                    Spell const* spell = eventInfo.TriggeredBySpell();
                    if (!spell || !(spell->GetSpellInfo()->SpellFamilyFlags[0] & 0x00020000))
                        return false;
                    return spell->GetCastTime() == 0;
                }
                case SPEC_HUNTER_BEAST_MASTERY:
                    return player->HasAura(SPELL_HUNTER_THE_BEAST_WITHIN);
                default:
                    return false;
            }
        }

        void HandleProc(ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();

            Player* player = GetTarget()->ToPlayer();
            if (player->GetTalentSpecialization(player->GetActiveSpec()) == SPEC_HUNTER_MARKSMANSHIP)
                player->CastSpell(player, SPELL_HUNTER_KEEN_EYE, true);
            else
            {
                player->CastSpell(player, SPELL_HUNTER_BRUTAL_KINSHIP, true);
                player->CastSpell(player, SPELL_HUNTER_BRUTAL_KINSHIP_PET, true);
            }
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_hun_t16_4p_bonus_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_hun_t16_4p_bonus_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_t16_4p_bonus_AuraScript();
    }
};

// 56453 - Lock and Load (Item - Hunter T16 4P Bonus, Survival)
class spell_hun_t16_4p_bonus_survival : public SpellScriptLoader
{
public:
    spell_hun_t16_4p_bonus_survival() : SpellScriptLoader("spell_hun_t16_4p_bonus_survival") { }

    class spell_hun_t16_4p_bonus_survival_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_t16_4p_bonus_survival_AuraScript);

        void HandleDropModCharge(Spell* /*bySpell*/)
        {
            if (AuraEffect const* bonus = GetUnitOwner()->GetAuraEffect(SPELL_HUNTER_T16_4P_BONUS, EFFECT_0))
                if (roll_chance_i(bonus->GetAmount()))
                    PreventDefaultAction();
        }

        void Register() OVERRIDE
        {
            OnDropModCharge += AuraDropModChargeFn(spell_hun_t16_4p_bonus_survival_AuraScript::HandleDropModCharge);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_t16_4p_bonus_survival_AuraScript();
    }
};

// 34471 - The Beast Within
class spell_hun_remove_brutal_kinship : public SpellScriptLoader
{
public:
    spell_hun_remove_brutal_kinship() : SpellScriptLoader("spell_hun_remove_brutal_kinship") { }

    class spell_hun_remove_brutal_kinship_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_remove_brutal_kinship_AuraScript);

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            target->RemoveAurasDueToSpell(SPELL_HUNTER_BRUTAL_KINSHIP);
            if (Guardian* pet = target->GetGuardianPet())
                pet->RemoveAurasDueToSpell(SPELL_HUNTER_BRUTAL_KINSHIP_PET);
        }

        void Register() OVERRIDE
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_hun_remove_brutal_kinship_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_MOD_POWER_COST_SCHOOL_PCT, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_remove_brutal_kinship_AuraScript();
    }
};

// 109306 - Thrill of the Hunt
class spell_hun_thrill_of_the_hunt : public SpellScriptLoader
{
public:
    spell_hun_thrill_of_the_hunt() : SpellScriptLoader("spell_hun_thrill_of_the_hunt") { }

    class spell_hun_thrill_of_the_hunt_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_thrill_of_the_hunt_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            // Once per focus-costing shot, on its main target
            Spell const* spell = eventInfo.TriggeredBySpell();
            if (!spell)
                return false;

            if (spell->m_targets.GetUnitTarget() != eventInfo.GetActionTarget())
                return false;

            return (spell->GetPowerType() == POWER_FOCUS && spell->GetPowerCost() > 0)
                || spell->GetSpellInfo()->Id == SPELL_HUNTER_KILL_COMMAND_DAMAGE;
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_hun_thrill_of_the_hunt_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_thrill_of_the_hunt_AuraScript();
    }
};

// 109248 - Binding Shot
class spell_hun_binding_shot : public SpellScriptLoader
{
public:
    spell_hun_binding_shot() : SpellScriptLoader("spell_hun_binding_shot") { }

    class spell_hun_binding_shot_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_binding_shot_SpellScript);

        void HandleCast()
        {
            if (WorldLocation const* dest = GetExplTargetDest())
                GetCaster()->CastSpell(dest->GetPositionX(), dest->GetPositionY(), dest->GetPositionZ(), SPELL_HUNTER_BINDING_SHOT_VISUAL_MISSILE, true);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_hun_binding_shot_SpellScript::HandleCast);
        }
    };

    class spell_hun_binding_shot_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_binding_shot_AuraScript);

        void HandleTick(AuraEffect const* /*aurEff*/)
        {
            Unit* hunter = GetCaster();
            if (!hunter)
                return;

            DynamicObject* dynObj = hunter->GetDynObject(SPELL_HUNTER_BINDING_SHOT_AREA);
            if (!dynObj)
                return;

            hunter->CastSpell(dynObj->GetPositionX(), dynObj->GetPositionY(), dynObj->GetPositionZ(), SPELL_HUNTER_BINDING_SHOT_LINK, true);

            // Tethers that were not refreshed by this tick belong to targets that left the area
            if (Unit::AuraList const* tethers = hunter->GetBoundAurasBySpellId(SPELL_HUNTER_BINDING_SHOT_LINK))
            {
                Unit::AuraList copy(*tethers);
                for (Unit::AuraList::const_iterator itr = copy.begin(); itr != copy.end(); ++itr)
                    if ((*itr)->GetDuration() != (*itr)->GetMaxDuration())
                        (*itr)->Remove();
            }
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_hun_binding_shot_AuraScript::HandleTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_binding_shot_SpellScript();
    }

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_binding_shot_AuraScript();
    }
};

// 117405 - Binding Shot (tether)
class spell_hun_binding_shot_link : public SpellScriptLoader
{
public:
    spell_hun_binding_shot_link() : SpellScriptLoader("spell_hun_binding_shot_link") { }

    class spell_hun_binding_shot_link_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_binding_shot_link_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* hunter = GetCaster())
                hunter->BindAura(GetAura());
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_DEFAULT)
                return;

            Unit* hunter = GetCaster();
            if (!hunter)
                return;

            DynamicObject* dynObj = hunter->GetDynObject(SPELL_HUNTER_BINDING_SHOT_AREA);
            if (!dynObj)
                return;

            Unit* target = GetTarget();
            if (target->HasAura(SPELL_HUNTER_BINDING_SHOT_IMMUNE))
                return;

            target->CastSpell(target, SPELL_HUNTER_BINDING_SHOT_STUN, true, NULL, NULL, hunter->GetGUID());
            target->CastSpell(target, SPELL_HUNTER_BINDING_SHOT_IMMUNE, true);
            target->CastSpell(dynObj->GetPositionX(), dynObj->GetPositionY(), dynObj->GetPositionZ(), SPELL_HUNTER_BINDING_SHOT_VISUAL_STUN, true);
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_hun_binding_shot_link_AuraScript::HandleApply, EFFECT_1, SPELL_AURA_MOD_DAMAGE_FROM_CASTER, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_hun_binding_shot_link_AuraScript::HandleRemove, EFFECT_1, SPELL_AURA_MOD_DAMAGE_FROM_CASTER, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_binding_shot_link_AuraScript();
    }
};

// 63487, 110601 - Ice Trap
class spell_hun_ice_trap : public SpellScriptLoader
{
public:
    spell_hun_ice_trap() : SpellScriptLoader("spell_hun_ice_trap") { }

    class spell_hun_ice_trap_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_ice_trap_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* trap = GetCaster();
            Unit* owner = trap->GetOwner();
            if (!owner)
                return;

            uint32 areaSpellId = owner->getClass() == CLASS_DRUID ? SPELL_HUNTER_ICE_TRAP_SYMBIOSIS_AREA : SPELL_HUNTER_ICE_TRAP_AREA;
            owner->CastSpell(trap->GetPositionX(), trap->GetPositionY(), trap->GetPositionZ(), areaSpellId, true);

            if (owner->HasAura(SPELL_HUNTER_GLYPH_OF_BLACK_ICE))
                owner->CastSpell(trap->GetPositionX(), trap->GetPositionY(), trap->GetPositionZ(), SPELL_HUNTER_GLYPH_OF_BLACK_ICE_AREA, true);
        }

        void Register() OVERRIDE
        {
            OnEffectHit += SpellEffectFn(spell_hun_ice_trap_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_ice_trap_SpellScript();
    }
};

// 621, 628 - Ice Trap (area trigger)
class sat_hun_ice_trap : public IAreaTriggerAura
{
    uint32 _slowSpellId = SPELL_HUNTER_ICE_TRAP_SLOW;

    void OnInit() OVERRIDE
    {
        _slowSpellId = GetAreaTrigger()->GetSpellId() == SPELL_HUNTER_ICE_TRAP_SYMBIOSIS_AREA ? SPELL_HUNTER_ICE_TRAP_SYMBIOSIS_SLOW : SPELL_HUNTER_ICE_TRAP_SLOW;
    }

    bool CheckTriggering(WorldObject* triggering) OVERRIDE
    {
        Unit* caster = GetAreaTrigger()->GetCaster();
        Unit* unit = triggering->ToUnit();
        return caster && unit && caster->_IsValidAttackTarget(unit, sSpellMgr->GetSpellInfo(_slowSpellId));
    }

    void OnTriggeringApply(WorldObject* triggering) OVERRIDE
    {
        Unit* unit = triggering->ToUnit();
        unit->CastSpell(unit, _slowSpellId, true);
    }

    void OnTriggeringUpdate(WorldObject* triggering) OVERRIDE
    {
        Unit* unit = triggering->ToUnit();
        if (!unit->HasAura(_slowSpellId))
            unit->CastSpell(unit, _slowSpellId, true);
    }

    void OnTriggeringRemove(WorldObject* triggering) OVERRIDE
    {
        if (Unit* unit = triggering->ToUnit())
            unit->RemoveAurasDueToSpell(_slowSpellId);
    }
};

// 347 - Glyph of Black Ice (area trigger)
class sat_hun_black_ice : public IAreaTriggerAura
{
    bool CheckTriggering(WorldObject* triggering) OVERRIDE
    {
        return triggering == GetAreaTrigger()->GetCaster();
    }

    void OnTriggeringApply(WorldObject* triggering) OVERRIDE
    {
        Unit* hunter = triggering->ToUnit();
        hunter->CastSpell(hunter, SPELL_HUNTER_BLACK_ICE, true);
    }

    void OnTriggeringUpdate(WorldObject* triggering) OVERRIDE
    {
        Unit* hunter = triggering->ToUnit();
        if (!hunter->HasAura(SPELL_HUNTER_BLACK_ICE))
            hunter->CastSpell(hunter, SPELL_HUNTER_BLACK_ICE, true);
    }

    void OnTriggeringRemove(WorldObject* triggering) OVERRIDE
    {
        if (Unit* hunter = triggering->ToUnit())
            hunter->RemoveAurasDueToSpell(SPELL_HUNTER_BLACK_ICE);
    }
};

// 510 - Flare (area trigger)
class sat_hun_flare : public IAreaTriggerAura
{
    bool CheckTriggering(WorldObject* triggering) OVERRIDE
    {
        Unit* caster = GetAreaTrigger()->GetCaster();
        Unit* target = triggering->ToUnit();
        if (!caster || !target || !target->IsAlive() || !GetAreaTrigger()->IsWithinLOSInMap(target))
            return false;

        // The target is passed as the viewer so stealthed and invisible units still qualify
        return caster->_IsValidAttackTarget(target, NULL, target);
    }

    void OnTriggeringApply(WorldObject* triggering) OVERRIDE
    {
        Unit* target = triggering->ToUnit();
        target->CastSpell(target, SPELL_HUNTER_FLARE_DEBUFF, true);
    }

    void OnTriggeringRemove(WorldObject* triggering) OVERRIDE
    {
        if (Unit* target = triggering->ToUnit())
            target->RemoveAurasDueToSpell(SPELL_HUNTER_FLARE_DEBUFF);
    }
};

void AddSC_hunter_spell_scripts()
{
    new spell_hun_a_murder_of_crows();
    new spell_hun_chimera_shot();
    new spell_hun_cobra_shot();
    new spell_hun_dire_beast();
    new spell_hun_fire();

    new spell_hun_glaive_toss_damage();
    new spell_hun_glaive_toss_missile();

    new spell_hun_improved_serpent_sting();
    new spell_hun_last_stand_pet();
    new spell_hun_masters_call();
    new spell_hun_misdirection();
    new spell_hun_misdirection_proc();
    new spell_hun_pet_carrion_feeder();
    new spell_hun_pet_heart_of_the_phoenix();

    new spell_hun_ready_set_aim();
    new spell_hun_scatter_shot();
    new spell_hun_serpent_sting();

    new spell_hun_stampede();
    new spell_hun_steady_shot();
    new spell_hun_tame_beast();

    new spell_hun_invigoration();
    new spell_hun_kill_command();
    new spell_hun_kill_command_damage();
    new spell_hun_hunters_mark_trigger();
    new spell_hun_frenzy();
    new spell_hun_focus_fire();
    new spell_hun_bestial_wrath();
    new spell_hun_beast_cleave_proc();
    new spell_hun_beast_cleave_damage();
    new spell_hun_multi_shot();
    new spell_hun_cobra_strikes();
    new spell_hun_cobra_strikes_stacks();
    new spell_hun_fervor();
    new spell_hun_chimera_shot_heal();
    new spell_hun_master_marksman();
    new spell_hun_steady_focus();
    new spell_hun_wild_quiver();
    new spell_hun_sniper_training();
    new spell_hun_lock_and_load();
    new spell_hun_lock_and_load_cooldown();
    new spell_hun_explosive_trap();
    new spell_hun_entrapment();
    new spell_hun_crowd_control_dot_hack();
    new spell_hun_pet_basic_attack();
    new spell_hun_blink_strikes();
    new spell_hun_pet_dust_cloud();
    new spell_hun_pet_ap_scaling_damage();
    new spell_hun_spirit_mend();
    new spell_hun_spirit_beast_blessing();
    new spell_hun_lynx_rush();
    new spell_hun_lynx_rush_dot();
    new spell_hun_spirit_bond();
    new spell_hun_disengage();
    new spell_hun_narrow_escape();
    new spell_hun_dash();
    new spell_hun_camouflage_driver();
    new spell_hun_camouflage();
    new spell_hun_camouflage_invisibility_driver();
    new spell_hun_glyph_of_aspect_of_the_beast();
    new spell_hun_tracking();
    new spell_hun_glyph_of_mend_pet();
    new spell_hun_aspect_of_the_cheetah();
    new spell_hun_glyph_of_the_aspects();
    new spell_hun_deterrence();
    new spell_hun_glyph_of_deterrence();
    new spell_hun_dismiss_pet();
    new spell_hun_fireworks();
    new spell_hun_glyph_of_lesser_proportion();
    new spell_hun_t16_2p_bonus();
    new spell_hun_t16_4p_bonus();
    new spell_hun_t16_4p_bonus_survival();
    new spell_hun_remove_brutal_kinship();
    new spell_hun_thrill_of_the_hunt();
    new spell_hun_binding_shot();
    new spell_hun_binding_shot_link();
    new spell_hun_ice_trap();
    new atrigger_script<sat_hun_ice_trap>("sat_hun_ice_trap");
    new atrigger_script<sat_hun_black_ice>("sat_hun_black_ice");
    new atrigger_script<sat_hun_flare>("sat_hun_flare");
}
