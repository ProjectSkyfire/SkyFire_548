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
#include "Cell.h"
#include "CellImpl.h"
#include "DBCStores.h"
#include "EventProcessor.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "ObjectAccessor.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "PetDefines.h"

#include <algorithm>

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
};

enum HunterCreatures
{
    NPC_HUNTER_DIRE_BEAST                           = 62005
};

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
                    player->CastSpell(target, SPELL_HUNTER_CROW_TRIGGER, true); // crow trigger+visual + (Dummy: cdr at 20% target HP NYI)
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

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_hun_a_murder_of_crows_AuraScript();
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
                !sSpellMgr->GetSpellInfo(SPELL_HUNTER_SERPENT_STING))
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

            if (Aura* aur = GetHitUnit()->GetAura(SPELL_HUNTER_SERPENT_STING, GetCaster()->GetGUID()))
                aur->SetDuration(aur->GetSpellInfo()->GetMaxDuration(), true);
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
                !sSpellMgr->GetSpellInfo(SPELL_HUNTER_SERPENT_STING))
                return false;
            return true;
        }

        bool Load() OVERRIDE
        {
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void HandleScriptEffect(SpellEffIndex /*effIndex*/)
        {
            GetCaster()->CastSpell(GetCaster(), SPELL_HUNTER_GENERIC_ENERGIZE_FOCUS, true);

            if (Aura* aur = GetHitUnit()->GetAura(SPELL_HUNTER_SERPENT_STING, GetCaster()->GetGUID()))
            {
                int32 newDuration = aur->GetDuration() + GetEffectValue() * IN_MILLISECONDS;
                aur->SetDuration(std::min(newDuration, aur->GetMaxDuration()), true);
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

        void HandleEffect(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!target)
                return;

            caster->CastSpell(target, SPELL_HUNTER_DIRE_BEAST_SUMMON, true);

            std::list<Creature*> minions;
            caster->GetAllMinionsByEntry(minions, NPC_HUNTER_DIRE_BEAST);
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

// 53412 - Invigoration
class spell_hun_invigoration : public SpellScriptLoader
{
public:
    spell_hun_invigoration() : SpellScriptLoader("spell_hun_invigoration") { }

    class spell_hun_invigoration_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_invigoration_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_INVIGORATION_TRIGGERED))
                return false;
            return true;
        }

        void HandleScriptEffect(SpellEffIndex /*effIndex*/)
        {
            if (Unit* unitTarget = GetHitUnit())
                if (AuraEffect* aurEff = unitTarget->GetDummyAuraEffect(SPELLFAMILY_HUNTER, 3487, 0))
                    if (roll_chance_i(aurEff->GetAmount()))
                        unitTarget->CastSpell(unitTarget, SPELL_HUNTER_INVIGORATION_TRIGGERED, true);
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_hun_invigoration_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_hun_invigoration_SpellScript();
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

// 34477 - Misdirection
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
            GetTarget()->CastSpell(GetTarget(), SPELL_HUNTER_MISDIRECTION_PROC, true, NULL, aurEff);
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

// 35079 - Misdirection (Proc)
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
            if (GetStackAmount() == 5)
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

        void HandleOnHit()
        {
            GetCaster()->CastSpell(GetCaster(), SPELL_HUNTER_STEADY_SHOT_FOCUS, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_hun_steady_shot_SpellScript::HandleOnHit);
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
}
