/*
 * Copyright (C) 2011-2021 Project SkyFire <https://www.projectskyfire.org/>
 * Copyright (C) 2008-2021 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2021 MaNGOS <https://www.getmangos.eu/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
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

/*
 * Scripts for spells with SPELLFAMILY_PRIEST and SPELLFAMILY_GENERIC spells used by priest players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_pri_".
 */

#include "Player.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "GridNotifiers.h"

enum PriestSpells
{
    SPELL_PRIEST_EVANGELISM_PROC                    = 81661,
    SPELL_PRIEST_BODY_AND_SOUL_PASSIVE              = 64129,
    SPELL_PRIEST_BODY_AND_SOUL_SPEED                = 65081,
    SPELL_PRIEST_WEAKENED_SOUL                      = 6788,
    SPELL_PRIEST_DIVINE_AEGIS                       = 47753,
    SPELL_PRIEST_DIVINE_TOUCH                       = 63544,
    SPELL_PRIEST_GLYPH_OF_LIGHTWELL                 = 55673,
    SPELL_PRIEST_GLYPH_OF_SHADOW                    = 107906,
    SPELL_PRIEST_ITEM_EFFICIENCY                    = 37595,
    SPELL_PRIEST_LEAP_OF_FAITH_EFFECT               = 92832,
    SPELL_PRIEST_LEAP_OF_FAITH_GLYPH                = 119850,
    SPELL_PRIEST_MANA_LEECH_PROC                    = 34650,
    SPELL_PRIEST_PENANCE_R1                         = 47540,
    SPELL_PRIEST_PENANCE_R1_DAMAGE                  = 47758,
    SPELL_PRIEST_PENANCE_R1_HEAL                    = 47757,

    SPELL_PRIEST_REFLECTIVE_SHIELD_GLYPH            = 33202,
    SPELL_PRIEST_REFLECTIVE_SHIELD_TRIGGERED        = 33619,

    SPELL_PRIEST_SHADOWFORM_VISUAL_WITHOUT_GLYPH    = 107903,
    SPELL_PRIEST_SHADOWFORM_VISUAL_WITH_GLYPH       = 107904,
    SPELL_PRIEST_SHADOW_WORD_DEATH                  = 32409,
    SPELL_PRIEST_T9_HEALING_2P                      = 67201,
    SPELL_PRIEST_VAMPIRIC_EMBRACE_HEAL              = 15290,
};

enum PriestSpellIcons
{
    PRIEST_ICON_ID_BORROWED_TIME                    = 2899,
    PRIEST_ICON_ID_DIVINE_TOUCH_TALENT              = 3021,
    PRIEST_ICON_ID_PAIN_AND_SUFFERING               = 2874
};

enum MiscSpells
{
    SPELL_GEN_REPLENISHMENT                         = 57669
};

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
            return eventInfo.GetProcTarget();
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            int32 absorb = CalculatePct(int32(eventInfo.GetHealInfo()->GetHeal()), aurEff->GetAmount());

            // Multiple effects stack, so let's try to find this aura.
            if (AuraEffect const* aegis = eventInfo.GetProcTarget()->GetAuraEffect(SPELL_PRIEST_DIVINE_AEGIS, EFFECT_0))
                absorb += aegis->GetAmount();

            absorb = std::min(absorb, eventInfo.GetProcTarget()->getLevel() * 125);

            GetTarget()->CastCustomSpell(SPELL_PRIEST_DIVINE_AEGIS, SPELLVALUE_BASE_POINT0, absorb, eventInfo.GetProcTarget(), true, NULL, aurEff);
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

// 7001 - Lightwell Renew
class spell_pri_lightwell_renew : public SpellScriptLoader
{
public:
    spell_pri_lightwell_renew() : SpellScriptLoader("spell_pri_lightwell_renew") { }

    class spell_pri_lightwell_renew_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pri_lightwell_renew_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* caster = GetCaster())
            {
                // Bonus from Glyph of Lightwell
                if (AuraEffect* modHealing = caster->GetAuraEffect(SPELL_PRIEST_GLYPH_OF_LIGHTWELL, EFFECT_0))
                    AddPct(amount, modHealing->GetAmount());
            }
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pri_lightwell_renew_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_lightwell_renew_AuraScript();
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

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_mind_sear_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
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
                    caster->CastSpell(unitTarget, sSpellMgr->GetSpellWithRank(SPELL_PRIEST_PENANCE_R1_DAMAGE, rank), false);
            }
        }

        SpellCastResult CheckCast()
        {
            Player* caster = GetCaster()->ToPlayer();
            if (Unit* target = GetExplTargetUnit())
                if (!caster->IsFriendlyTo(target) && !caster->IsValidAttackTarget(target))
                    return SPELL_FAILED_BAD_TARGETS;
            return SPELL_CAST_OK;
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

// 17 - Power Word: Shield
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

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pri_power_word_shield_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            AfterEffectAbsorb += AuraEffectAbsorbFn(spell_pri_power_word_shield_AuraScript::ReflectDamage, EFFECT_0);
            OnEffectApply += AuraEffectApplyFn(spell_pri_power_word_shield_AuraScript::HandleOnEffectApply, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
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
                // Divine Touch
                if (AuraEffect const* empoweredRenewAurEff = caster->GetDummyAuraEffect(SPELLFAMILY_PRIEST, PRIEST_ICON_ID_DIVINE_TOUCH_TALENT, EFFECT_0))
                {
                    uint32 heal = caster->SpellHealingBonusDone(GetTarget(), GetSpellInfo(), aurEff->GetAmount(), DOT);
                    heal = GetTarget()->SpellHealingBonusTaken(caster, GetSpellInfo(), heal, DOT);
                    int32 basepoints0 = CalculatePct(int32(heal) * aurEff->GetTotalTicks(), empoweredRenewAurEff->GetAmount());
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

// 32379 - Shadow Word Death
class spell_pri_shadow_word_death : public SpellScriptLoader
{
public:
    spell_pri_shadow_word_death() : SpellScriptLoader("spell_pri_shadow_word_death") { }

    class spell_pri_shadow_word_death_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pri_shadow_word_death_SpellScript);

        void HandleDamage()
        {
            int32 damage = GetHitDamage();
            GetCaster()->CastCustomSpell(GetCaster(), SPELL_PRIEST_SHADOW_WORD_DEATH, &damage, 0, 0, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_pri_shadow_word_death_SpellScript::HandleDamage);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_pri_shadow_word_death_SpellScript();
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
            GetTarget()->RemoveAurasDueToSpell(GetTarget()->HasAura(SPELL_PRIEST_GLYPH_OF_SHADOW) ? SPELL_PRIEST_SHADOWFORM_VISUAL_WITH_GLYPH : SPELL_PRIEST_SHADOWFORM_VISUAL_WITHOUT_GLYPH);
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
            DoCheckProc += AuraCheckProcFn(spell_pri_vampiric_embrace_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_pri_vampiric_embrace_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
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
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pri_vampiric_embrace_target_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_PARTY);
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

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_GEN_REPLENISHMENT))
                return false;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            return eventInfo.GetProcTarget() == GetCaster();
        }

        void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            eventInfo.GetProcTarget()->CastSpell((Unit*)NULL, SPELL_GEN_REPLENISHMENT, true, NULL, aurEff);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_pri_vampiric_touch_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_pri_vampiric_touch_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_pri_vampiric_touch_AuraScript();
    }
};

void AddSC_priest_spell_scripts()
{
    new spell_pri_evangelism();
    new spell_pri_divine_aegis();
    new spell_pri_item_greater_heal_refund();
    new spell_pri_leap_of_faith();
    new spell_pri_lightwell_renew();
    new spell_pri_mana_leech();
    new spell_pri_mind_sear();
    new spell_pri_penance();
    new spell_pri_power_word_shield();
    new spell_pri_prayer_of_mending_heal();
    new spell_pri_renew();
    new spell_pri_shadow_word_death();
    new spell_pri_shadowform();
    new spell_pri_vampiric_embrace();
    new spell_pri_vampiric_embrace_target();
    new spell_pri_vampiric_touch();
}
