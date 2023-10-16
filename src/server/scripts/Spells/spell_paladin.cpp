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

enum PaladinSpells
{
    SPELL_PALADIN_LONG_ARM_OF_THE_LAW_TALENT     = 87172,
    SPELL_PALADIN_LONG_ARM_OF_THE_LAW_SPEEDBUFF  = 87173,

    SPELL_PALADIN_AVENGERS_SHIELD                = 31935,
    SPELL_PALADIN_AURA_MASTERY_IMMUNE            = 64364,
    SPELL_PALADIN_BEACON_OF_LIGHT_MARKER         = 53563,
    SPELL_PALADIN_BEACON_OF_LIGHT_HEAL           = 53652,
    SPELL_PALADIN_BLESSING_OF_LOWER_CITY_DRUID   = 37878,
    SPELL_PALADIN_BLESSING_OF_LOWER_CITY_PALADIN = 37879,
    SPELL_PALADIN_BLESSING_OF_LOWER_CITY_PRIEST  = 37880,
    SPELL_PALADIN_BLESSING_OF_LOWER_CITY_SHAMAN  = 37881,
    SPELL_PALADIN_CONCENTRACTION_AURA            = 19746,
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
    SPELL_PALADIN_SEAL_OF_RIGHTEOUSNESS          = 25742,
};

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
                !sSpellMgr->GetSpellInfo(SPELL_PALADIN_LONG_ARM_OF_THE_LAW_SPEEDBUFF))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (GetCaster()->HasAura(SPELL_PALADIN_LONG_ARM_OF_THE_LAW_TALENT))
                GetCaster()->CastSpell(GetCaster(), SPELL_PALADIN_LONG_ARM_OF_THE_LAW_SPEEDBUFF);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_pal_judgment_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            
        }
    }; 

    SpellScript* GetSpellScript() const override
    {
        return new spell_pal_judgment_SpellScript();
    }
};

/*
// 31850 - Ardent Defender
class spell_pal_ardent_defender : public SpellScriptLoader
{
    public:
        spell_pal_ardent_defender() : SpellScriptLoader("spell_pal_ardent_defender") { }

        class spell_pal_ardent_defender_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_ardent_defender_AuraScript);

            uint32 absorbPct, healPct;

            enum Spell
            {
                PAL_SPELL_ARDENT_DEFENDER_HEAL = 66235,
            };

            bool Load() override
            {
                healPct = GetSpellInfo()->Effects[EFFECT_1].CalcValue();
                absorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue();
                return GetUnitOwner()->GetTypeId() == TYPEID_PLAYER;
            }

            void CalculateAmount(AuraEffect const* aurEff, int32 & amount, bool & canBeRecalculated)
            {
                // Set absorbtion amount to unlimited
                amount = -1;
            }

            void Absorb(AuraEffect* aurEff, DamageInfo & dmgInfo, uint32 & absorbAmount)
            {
                Unit* victim = GetTarget();
                int32 remainingHealth = victim->GetHealth() - dmgInfo.GetDamage();
                uint32 allowedHealth = victim->CountPctFromMaxHealth(35);
                // If damage kills us
                if (remainingHealth <= 0 && !victim->ToPlayer()->HasSpellCooldown(PAL_SPELL_ARDENT_DEFENDER_HEAL))
                {
                    // Cast healing spell, completely avoid damage
                    absorbAmount = dmgInfo.GetDamage();

                    uint32 defenseSkillValue = victim->GetDefenseSkillValue();
                    // Max heal when defense skill denies critical hits from raid bosses
                    // Formula: max defense at level + 140 (raiting from gear)
                    uint32 reqDefForMaxHeal  = victim->getLevel() * 5 + 140;
                    float pctFromDefense = (defenseSkillValue >= reqDefForMaxHeal)
                        ? 1.0f
                        : float(defenseSkillValue) / float(reqDefForMaxHeal);

                    int32 healAmount = int32(victim->CountPctFromMaxHealth(uint32(healPct * pctFromDefense)));
                    victim->CastCustomSpell(victim, PAL_SPELL_ARDENT_DEFENDER_HEAL, &healAmount, NULL, NULL, true, NULL, aurEff);
                    victim->ToPlayer()->AddSpellCooldown(PAL_SPELL_ARDENT_DEFENDER_HEAL, 0, time(NULL) + 120);
                }
                else if (remainingHealth < int32(allowedHealth))
                {
                    // Reduce damage that brings us under 35% (or full damage if we are already under 35%) by x%
                    uint32 damageToReduce = (victim->GetHealth() < allowedHealth)
                        ? dmgInfo.GetDamage()
                        : allowedHealth - remainingHealth;
                    absorbAmount = CalculatePct(damageToReduce, absorbPct);
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
*/

// 31821 - Aura Mastery
class spell_pal_aura_mastery : public SpellScriptLoader
{
public:
    spell_pal_aura_mastery() : SpellScriptLoader("spell_pal_aura_mastery") { }

    class spell_pal_aura_mastery_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_aura_mastery_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_AURA_MASTERY_IMMUNE))
                return false;
            return true;
        }

        void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->CastSpell(GetTarget(), SPELL_PALADIN_AURA_MASTERY_IMMUNE, true);
        }

        void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->RemoveOwnedAura(SPELL_PALADIN_AURA_MASTERY_IMMUNE, GetCasterGUID());
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_pal_aura_mastery_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_pal_aura_mastery_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_pal_aura_mastery_AuraScript();
    }
};

// 64364 - Aura Mastery Immune
class spell_pal_aura_mastery_immune : public SpellScriptLoader
{
public:
    spell_pal_aura_mastery_immune() : SpellScriptLoader("spell_pal_aura_mastery_immune") { }

    class spell_pal_aura_mastery_immune_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_aura_mastery_immune_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_CONCENTRACTION_AURA))
                return false;
            return true;
        }

        bool CheckAreaTarget(Unit* target)
        {
            return target->HasAura(SPELL_PALADIN_CONCENTRACTION_AURA, GetCasterGUID());
        }

        void Register() override
        {
            DoCheckAreaTarget += AuraCheckAreaTargetFn(spell_pal_aura_mastery_immune_AuraScript::CheckAreaTarget);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_pal_aura_mastery_immune_AuraScript();
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

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_BEACON_OF_LIGHT_HEAL))
                return false;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (eventInfo.GetActionTarget()->GetAura(SPELL_PALADIN_BEACON_OF_LIGHT_MARKER, GetCasterGUID()))
                return false;
            return true;
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            int32 heal = eventInfo.GetHealInfo()->GetHeal();

            if (eventInfo.GetDamageInfo()->GetSpellInfo()->Id != SPELL_PALADIN_HOLY_LIGHT)
                heal = int32(CalculatePct(heal, aurEff->GetAmount()));

            Unit::AuraList const& auras = GetCaster()->GetSingleCastAuras();
            for (Unit::AuraList::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
            {
                if ((*itr)->GetId() == SPELL_PALADIN_BEACON_OF_LIGHT_MARKER)
                {
                    std::list<AuraApplication*> applications;
                    (*itr)->GetApplicationList(applications);
                    if (applications.empty())
                        return;

                    GetCaster()->CastCustomSpell(SPELL_PALADIN_BEACON_OF_LIGHT_HEAL, SPELLVALUE_BASE_POINT0, heal, applications.front()->GetTarget(), true, NULL, aurEff);
                    return;
                }
            }
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_pal_beacon_of_light_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_pal_beacon_of_light_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
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

// -75806 - Grand Crusader
class spell_pal_grand_crusader : public SpellScriptLoader
{
public:
    spell_pal_grand_crusader() : SpellScriptLoader("spell_pal_grand_crusader") { }

    class spell_pal_grand_crusader_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_grand_crusader_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_AVENGERS_SHIELD))
                return false;
            return true;
        }

        bool CheckProc(ProcEventInfo& /*eventInfo*/)
        {
            return GetTarget()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void HandleEffectProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
        {
            GetTarget()->ToPlayer()->RemoveSpellCooldown(SPELL_PALADIN_AVENGERS_SHIELD, true);
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_pal_grand_crusader_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_pal_grand_crusader_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_pal_grand_crusader_AuraScript();
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

        bool Validate(SpellInfo const* spellInfo) override
        {
            SpellInfo const* firstRankSpellInfo = sSpellMgr->GetSpellInfo(SPELL_PALADIN_HOLY_SHOCK_R1);
            if (!firstRankSpellInfo)
                return false;

            // can't use other spell than holy shock due to spell_ranks dependency
            if (!spellInfo->IsRankOf(firstRankSpellInfo))
                return false;

            uint8 rank = spellInfo->GetRank();
            if (!sSpellMgr->GetSpellWithRank(SPELL_PALADIN_HOLY_SHOCK_R1_DAMAGE, rank, true) || !sSpellMgr->GetSpellWithRank(SPELL_PALADIN_HOLY_SHOCK_R1_HEALING, rank, true))
                return false;

            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (Unit* unitTarget = GetHitUnit())
            {
                uint8 rank = GetSpellInfo()->GetRank();
                if (caster->IsFriendlyTo(unitTarget))
                    caster->CastSpell(unitTarget, sSpellMgr->GetSpellWithRank(SPELL_PALADIN_HOLY_SHOCK_R1_HEALING, rank), true);
                else
                    caster->CastSpell(unitTarget, sSpellMgr->GetSpellWithRank(SPELL_PALADIN_HOLY_SHOCK_R1_DAMAGE, rank), true);
            }
        }

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            if (Unit* target = GetExplTargetUnit())
            {
                if (!caster->IsFriendlyTo(target))
                {
                    if (!caster->IsValidAttackTarget(target))
                        return SpellCastResult::SPELL_FAILED_BAD_TARGETS;

                    if (!caster->isInFront(target))
                        return SpellCastResult::SPELL_FAILED_UNIT_NOT_INFRONT;
                }
            }
            else
                return SpellCastResult::SPELL_FAILED_BAD_TARGETS;
            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_pal_holy_shock_SpellScript::CheckCast);
            OnEffectHitTarget += SpellEffectFn(spell_pal_holy_shock_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
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

// 85285 - Sacred Shield
class spell_pal_sacred_shield : public SpellScriptLoader
{
public:
    spell_pal_sacred_shield() : SpellScriptLoader("spell_pal_sacred_shield") { }

    class spell_pal_sacred_shield_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_sacred_shield_SpellScript);

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            if (caster->GetTypeId() != TypeID::TYPEID_PLAYER)
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;

            if (!caster->HealthBelowPct(30))
                return SpellCastResult::SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_pal_sacred_shield_SpellScript::CheckCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_pal_sacred_shield_SpellScript();
    }
};

// 85256 - Templar's Verdict
/// Updated 4.3.4
class spell_pal_templar_s_verdict : public SpellScriptLoader
{
public:
    spell_pal_templar_s_verdict() : SpellScriptLoader("spell_pal_templar_s_verdict") { }

    class spell_pal_templar_s_verdict_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_templar_s_verdict_SpellScript);

        bool Validate(SpellInfo const* /*spellEntry*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PALADIN_DIVINE_PURPOSE_PROC))
                return false;

            return true;
        }

        bool Load() override
        {
            if (GetCaster()->GetTypeId() != TypeID::TYPEID_PLAYER)
                return false;

            if (GetCaster()->ToPlayer()->getClass() != CLASS_PALADIN)
                return false;

            return true;
        }

        void ChangeDamage(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            int32 damage = GetHitDamage();

            if (caster->HasAura(SPELL_PALADIN_DIVINE_PURPOSE_PROC))
                damage *= 7.5;  // 7.5*30% = 225%
            else
            {
                switch (caster->GetPower(POWER_HOLY_POWER))
                {
                case 0: // 1 Holy Power
                    damage = damage;
                    break;
                case 1: // 2 Holy Power
                    damage *= 3;    // 3*30 = 90%
                    break;
                case 2: // 3 Holy Power
                    damage *= 7.5;  // 7.5*30% = 225%
                    break;
                }
            }

            SetHitDamage(damage);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_pal_templar_s_verdict_SpellScript::ChangeDamage, EFFECT_0, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_pal_templar_s_verdict_SpellScript();
    }
};

// 20154, 21084 - Seal of Righteousness - melee proc dummy (addition ${$MWS*(0.022*$AP+0.044*$SPH)} damage)
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

            float ap = GetTarget()->GetTotalAttackPowerValue(WeaponAttackType::BASE_ATTACK);
            int32 holy = GetTarget()->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_HOLY);
            holy += eventInfo.GetProcTarget()->SpellBaseDamageBonusTaken(SPELL_SCHOOL_MASK_HOLY);
            int32 bp = int32((ap * 0.022f + 0.044f * holy) * GetTarget()->GetAttackTime(WeaponAttackType::BASE_ATTACK) / 1000);
            GetTarget()->CastCustomSpell(SPELL_PALADIN_SEAL_OF_RIGHTEOUSNESS, SPELLVALUE_BASE_POINT0, bp, eventInfo.GetProcTarget(), true, NULL, aurEff);
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


void AddSC_paladin_spell_scripts()
{
    new spell_pal_judgment();
    //new spell_pal_ardent_defender();
    new spell_pal_aura_mastery();
    new spell_pal_aura_mastery_immune();
    new spell_pal_beacon_of_light();
    new spell_pal_blessing_of_faith();
    new spell_pal_eye_for_an_eye();
    new spell_pal_grand_crusader();
    new spell_pal_hand_of_sacrifice();
    new spell_pal_holy_shock();
    new spell_pal_item_healing_discount();
    new spell_pal_righteous_defense();
    new spell_pal_sacred_shield();
    new spell_pal_templar_s_verdict();
    new spell_pal_seal_of_righteousness();
}
