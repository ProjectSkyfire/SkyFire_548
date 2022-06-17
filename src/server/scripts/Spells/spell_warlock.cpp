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
 * Scripts for spells with SPELLFAMILY_WARLOCK and SPELLFAMILY_GENERIC spells used by warlock players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_warl_".
 */

#include "Player.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"

enum WarlockSpells
{
    SPELL_WARLOCK_AFTERMATH_STUN                    = 85387,
    SPELL_WARLOCK_CREATE_HEALTHSTONE                = 34130,

    SPELL_WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST         = 62388, 
    SPELL_WARLOCK_DEMONIC_CIRCLE_SUMMON             = 48018,
    SPELL_WARLOCK_DEMONIC_CIRCLE_TELEPORT           = 48020,

    SPELL_WARLOCK_FEL_SYNERGY_HEAL                  = 54181,

    SPELL_WARLOCK_LIFE_TAP_ENERGIZE                 = 31818,

    SPELL_WARLOCK_RAIN_OF_FIRE                      = 42223,

    SPELL_WARLOCK_SHADOW_TRANCE                     = 17941,
    SPELL_WARLOCK_SIPHON_LIFE_HEAL                  = 63106,
    
    SPELL_WARLOCK_SOULSHATTER                       = 32835,

    SPELL_WARLOCK_SOUL_SWAP_CD_MARKER               = 94229,
    SPELL_WARLOCK_SOUL_SWAP_OVERRIDE                = 86211,
    SPELL_WARLOCK_SOUL_SWAP_MOD_COST                = 92794,
    SPELL_WARLOCK_SOUL_SWAP_DOT_MARKER              = 92795,

    SPELL_WARLOCK_UNSTABLE_AFFLICTION_DISPEL        = 31117
};

enum WarlockSpellIcons
{
    WARLOCK_ICON_ID_IMPROVED_LIFE_TAP               = 208,
    WARLOCK_ICON_ID_MANA_FEED                       = 1982
};

enum MiscSpells
{
    SPELL_GEN_REPLENISHMENT                         = 57669,
    SPELL_PRIEST_SHADOW_WORD_DEATH                  = 32409
};

// 111546 - Chaotic Energy
class spell_warl_chaotic_energy : public SpellScriptLoader
{
public:
    spell_warl_chaotic_energy() : SpellScriptLoader("spell_warl_chaotic_energy") { }

    class spell_warl_chaotic_energy_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_chaotic_energy_AuraScript);

        void HandleEffectCalcSpellMod(AuraEffect const* aurEff, SpellModifier*& spellMod)
        {
            if (!spellMod)
            {
                spellMod = new SpellModifier(GetAura());
                spellMod->op = SPELLMOD_COST;
                spellMod->type = SPELLMOD_PCT;
                spellMod->spellId = GetId();
                spellMod->mask = GetSpellInfo()->Effects[aurEff->GetEffIndex()].SpellClassMask;
            }

            spellMod->value = (float)(aurEff->GetAmount() / 100);
        }

        void Register() override
        {
            DoEffectCalcSpellMod += AuraEffectCalcSpellModFn(spell_warl_chaotic_energy_AuraScript::HandleEffectCalcSpellMod, EFFECT_1, SPELL_AURA_ADD_PCT_MODIFIER);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warl_chaotic_energy_AuraScript();
    }
};

// 710 - Banish
class spell_warl_banish : public SpellScriptLoader
{
public:
    spell_warl_banish() : SpellScriptLoader("spell_warl_banish") { }

    class spell_warl_banish_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_banish_SpellScript);

        bool Load() override
        {
            _removed = false;
            return true;
        }

        void HandleBanish()
        {
            /// Casting Banish on a banished target will cancel the effect
            /// Check if the target already has Banish, if so, do nothing.
            if (Unit* target = GetHitUnit())
            {
                if (target->GetAuraEffect(SPELL_AURA_SCHOOL_IMMUNITY, SPELLFAMILY_WARLOCK, 0, 0x08000000, 0))
                {
                    // No need to remove old aura since its removed due to not stack by current Banish aura
                    PreventHitDefaultEffect(EFFECT_0);
                    PreventHitDefaultEffect(EFFECT_1);
                    PreventHitDefaultEffect(EFFECT_2);
                    _removed = true;
                }
            }
        }

        void RemoveAura()
        {
            if (_removed)
                PreventHitAura();
        }

        void Register() override
        {
            BeforeHit += SpellHitFn(spell_warl_banish_SpellScript::HandleBanish);
            AfterHit += SpellHitFn(spell_warl_banish_SpellScript::RemoveAura);
        }

        bool _removed;
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_banish_SpellScript();
    }
};

// 48018 - Demonic Circle: Summon
/// Updated 4.3.4
class spell_warl_demonic_circle_summon : public SpellScriptLoader
{
public:
    spell_warl_demonic_circle_summon() : SpellScriptLoader("spell_warl_demonic_circle_summon") { }

    class spell_warl_demonic_circle_summon_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_demonic_circle_summon_AuraScript);

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes mode)
        {
            // If effect is removed by expire remove the summoned demonic circle too.
            if (!(mode & AURA_EFFECT_HANDLE_REAPPLY))
                GetTarget()->RemoveGameObject(GetId(), true);

            GetTarget()->RemoveAura(SPELL_WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST);
        }

        void HandleDummyTick(AuraEffect const* /*aurEff*/)
        {
            if (GameObject* circle = GetTarget()->GetGameObject(GetId()))
            {
                // Here we check if player is in demonic circle teleport range, if so add
                // WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST; allowing him to cast the WARLOCK_DEMONIC_CIRCLE_TELEPORT.
                // If not in range remove the WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST.

                SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(SPELL_WARLOCK_DEMONIC_CIRCLE_TELEPORT);

                if (GetTarget()->IsWithinDist(circle, spellInfo->GetMaxRange(true)))
                {
                    if (!GetTarget()->HasAura(SPELL_WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST))
                        GetTarget()->CastSpell(GetTarget(), SPELL_WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST, true);
                }
                else
                    GetTarget()->RemoveAura(SPELL_WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST);
            }
        }

        void Register() override
        {
            OnEffectRemove += AuraEffectApplyFn(spell_warl_demonic_circle_summon_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_demonic_circle_summon_AuraScript::HandleDummyTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warl_demonic_circle_summon_AuraScript();
    }
};

// 48020 - Demonic Circle: Teleport
/// Updated 4.3.4
class spell_warl_demonic_circle_teleport : public SpellScriptLoader
{
public:
    spell_warl_demonic_circle_teleport() : SpellScriptLoader("spell_warl_demonic_circle_teleport") { }

    class spell_warl_demonic_circle_teleport_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_demonic_circle_teleport_AuraScript);

        void HandleTeleport(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* player = GetTarget()->ToPlayer())
            {
                if (GameObject* circle = player->GetGameObject(SPELL_WARLOCK_DEMONIC_CIRCLE_SUMMON))
                {
                    player->NearTeleportTo(circle->GetPositionX(), circle->GetPositionY(), circle->GetPositionZ(), circle->GetOrientation());
                    player->RemoveMovementImpairingAuras();
                }
            }
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_warl_demonic_circle_teleport_AuraScript::HandleTeleport, EFFECT_0, SPELL_AURA_MECHANIC_IMMUNITY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warl_demonic_circle_teleport_AuraScript();
    }
};

// -47230 - Fel Synergy
class spell_warl_fel_synergy : public SpellScriptLoader
{
public:
    spell_warl_fel_synergy() : SpellScriptLoader("spell_warl_fel_synergy") { }

    class spell_warl_fel_synergy_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_fel_synergy_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_FEL_SYNERGY_HEAL))
                return false;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            return GetTarget()->GetGuardianPet() && eventInfo.GetDamageInfo()->GetDamage();
        }

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            int32 heal = CalculatePct(int32(eventInfo.GetDamageInfo()->GetDamage()), aurEff->GetAmount());
            GetTarget()->CastCustomSpell(SPELL_WARLOCK_FEL_SYNERGY_HEAL, SPELLVALUE_BASE_POINT0, heal, (Unit*)NULL, true, NULL, aurEff); // TARGET_UNIT_PET
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_warl_fel_synergy_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_warl_fel_synergy_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warl_fel_synergy_AuraScript();
    }
};

// 6262 - Healthstone
class spell_warl_healthstone_heal : public SpellScriptLoader
{
public:
    spell_warl_healthstone_heal() : SpellScriptLoader("spell_warl_healthstone_heal") { }

    class spell_warl_healthstone_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_healthstone_heal_SpellScript);

        void HandleOnHit()
        {
            int32 heal = int32(CalculatePct(GetCaster()->GetCreateHealth(), GetHitHeal()));
            SetHitHeal(heal);
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_warl_healthstone_heal_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_healthstone_heal_SpellScript();
    }
};

// 1454 - Life Tap
/// Updated 4.3.4
class spell_warl_life_tap : public SpellScriptLoader
{
public:
    spell_warl_life_tap() : SpellScriptLoader("spell_warl_life_tap") { }

    class spell_warl_life_tap_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_life_tap_SpellScript);

        bool Load() override
        {
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_LIFE_TAP_ENERGIZE))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Player* caster = GetCaster()->ToPlayer();
            if (Unit* target = GetHitUnit())
            {
                int32 damage = caster->CountPctFromMaxHealth(GetSpellInfo()->Effects[EFFECT_2].CalcValue());
                int32 mana = CalculatePct(damage, GetSpellInfo()->Effects[EFFECT_1].CalcValue());

                // Shouldn't Appear in Combat Log
                target->ModifyHealth(-damage);

                // Improved Life Tap mod
                if (AuraEffect const* aurEff = caster->GetDummyAuraEffect(SPELLFAMILY_WARLOCK, WARLOCK_ICON_ID_IMPROVED_LIFE_TAP, 0))
                    AddPct(mana, aurEff->GetAmount());

                caster->CastCustomSpell(target, SPELL_WARLOCK_LIFE_TAP_ENERGIZE, &mana, NULL, NULL, false);
            }
        }

        SpellCastResult CheckCast()
        {
            if (int32(GetCaster()->GetHealth()) > int32(GetCaster()->CountPctFromMaxHealth(GetSpellInfo()->Effects[EFFECT_2].CalcValue())))
                return SpellCastResult::SPELL_CAST_OK;
            return SpellCastResult::SPELL_FAILED_FIZZLE;
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_life_tap_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            OnCheckCast += SpellCheckCastFn(spell_warl_life_tap_SpellScript::CheckCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_life_tap_SpellScript();
    }
};

// 27285 - Seed of Corruption
/// Updated 4.3.4
class spell_warl_seed_of_corruption : public SpellScriptLoader
{
public:
    spell_warl_seed_of_corruption() : SpellScriptLoader("spell_warl_seed_of_corruption") { }

    class spell_warl_seed_of_corruption_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_seed_of_corruption_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (GetExplTargetUnit())
                targets.remove(GetExplTargetUnit());
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warl_seed_of_corruption_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_seed_of_corruption_SpellScript();
    }
};

// 108558 - Nightfall
class spell_warl_shadow_trance_proc : public SpellScriptLoader
{
public:
    spell_warl_shadow_trance_proc() : SpellScriptLoader("spell_warl_shadow_trance_proc") { }

    class spell_warl_shadow_trance_proc_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_shadow_trance_proc_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_SHADOW_TRANCE))
                return false;
            return true;
        }

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();

            // get shadow trace proc chance
            int32 procChance = std::max(aurEff->GetAmount(), 0);

            if (roll_chance_i(procChance))
                GetTarget()->CastSpell(GetTarget(), SPELL_WARLOCK_SHADOW_TRANCE, true, NULL, aurEff);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_warl_shadow_trance_proc_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warl_shadow_trance_proc_AuraScript();
    }
};

// 86121 - Soul Swap
class spell_warl_soul_swap : public SpellScriptLoader
{
public:
    spell_warl_soul_swap() : SpellScriptLoader("spell_warl_soul_swap") { }

    class spell_warl_soul_swap_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_soul_swap_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_SOUL_SWAP_CD_MARKER) ||
                !sSpellMgr->GetSpellInfo(SPELL_WARLOCK_SOUL_SWAP_OVERRIDE))
                return false;
            return true;
        }

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            GetCaster()->CastSpell(GetCaster(), SPELL_WARLOCK_SOUL_SWAP_OVERRIDE, true);
            GetHitUnit()->CastSpell(GetCaster(), SPELL_WARLOCK_SOUL_SWAP_DOT_MARKER, true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_soul_swap_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_soul_swap_SpellScript();
    }
};

// 86211 - Soul Swap Override - Also acts as a dot container
class spell_warl_soul_swap_override : public SpellScriptLoader
{
public:
    spell_warl_soul_swap_override() : SpellScriptLoader("spell_warl_soul_swap_override") { }

    class spell_warl_soul_swap_override_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_soul_swap_override_AuraScript);

        bool Load() override
        {
            _swapCaster = NULL;
            return true;
        }

        //! Forced to, pure virtual functions must have a body when linking
        void Register() override { }

    public:
        void AddDot(uint32 id) { _dotList.push_back(id); }
        std::list<uint32> const GetDotList() const { return _dotList; }
        Unit* GetOriginalSwapSource() const { return _swapCaster; }
        void SetOriginalSwapSource(Unit* victim) { _swapCaster = victim; }

    private:
        std::list<uint32> _dotList;
        Unit* _swapCaster;
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warl_soul_swap_override_AuraScript();
    }
};

typedef spell_warl_soul_swap_override::spell_warl_soul_swap_override_AuraScript SoulSwapOverrideAuraScript;

//! Soul Swap Copy Spells - 92795 - Simply copies spell IDs.
class spell_warl_soul_swap_dot_marker : public SpellScriptLoader
{
public:
    spell_warl_soul_swap_dot_marker() : SpellScriptLoader("spell_warl_soul_swap_dot_marker") { }

    class spell_warl_soul_swap_dot_marker_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_soul_swap_dot_marker_SpellScript);

        void HandleHit(SpellEffIndex effIndex)
        {
            Unit* swapVictim = GetCaster();
            Unit* warlock = GetHitUnit();
            if (!warlock || !swapVictim)
                return;

            flag128 classMask = GetSpellInfo()->Effects[effIndex].SpellClassMask;

            Unit::AuraApplicationMap const& appliedAuras = swapVictim->GetAppliedAuras();
            SoulSwapOverrideAuraScript* swapSpellScript = NULL;
            if (Aura* swapOverrideAura = warlock->GetAura(SPELL_WARLOCK_SOUL_SWAP_OVERRIDE))
                swapSpellScript = dynamic_cast<SoulSwapOverrideAuraScript*>(swapOverrideAura->GetScriptByName("spell_warl_soul_swap_override"));

            if (swapSpellScript == NULL)
                return;

            for (Unit::AuraApplicationMap::const_iterator itr = appliedAuras.begin(); itr != appliedAuras.end(); ++itr)
            {
                SpellInfo const* spellProto = itr->second->GetBase()->GetSpellInfo();
                if (itr->second->GetBase()->GetCaster() == warlock)
                    if (spellProto->SpellFamilyName == SPELLFAMILY_WARLOCK && (spellProto->SpellFamilyFlags & classMask))
                        swapSpellScript->AddDot(itr->first);
            }

            swapSpellScript->SetOriginalSwapSource(swapVictim);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_soul_swap_dot_marker_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_soul_swap_dot_marker_SpellScript();
    }
};

// 86213 - Soul Swap Exhale
class spell_warl_soul_swap_exhale : public SpellScriptLoader
{
public:
    spell_warl_soul_swap_exhale() : SpellScriptLoader("spell_warl_soul_swap_exhale") { }

    class spell_warl_soul_swap_exhale_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_soul_swap_exhale_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_SOUL_SWAP_MOD_COST) ||
                !sSpellMgr->GetSpellInfo(SPELL_WARLOCK_SOUL_SWAP_OVERRIDE))
                return false;
            return true;
        }

        SpellCastResult CheckCast()
        {
            Unit* currentTarget = GetExplTargetUnit();
            Unit* swapTarget = NULL;
            if (Aura const* swapOverride = GetCaster()->GetAura(SPELL_WARLOCK_SOUL_SWAP_OVERRIDE))
                if (SoulSwapOverrideAuraScript* swapScript = dynamic_cast<SoulSwapOverrideAuraScript*>(swapOverride->GetScriptByName("spell_warl_soul_swap_override")))
                    swapTarget = swapScript->GetOriginalSwapSource();

            // Soul Swap Exhale can't be cast on the same target than Soul Swap
            if (swapTarget && currentTarget && swapTarget == currentTarget)
                return SpellCastResult::SPELL_FAILED_BAD_TARGETS;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void OnEffectHit(SpellEffIndex /*effIndex*/)
        {
            GetCaster()->CastSpell(GetCaster(), SPELL_WARLOCK_SOUL_SWAP_MOD_COST, true);

            std::list<uint32> dotList;
            Unit* swapSource = NULL;
            if (Aura const* swapOverride = GetCaster()->GetAura(SPELL_WARLOCK_SOUL_SWAP_OVERRIDE))
            {
                SoulSwapOverrideAuraScript* swapScript = dynamic_cast<SoulSwapOverrideAuraScript*>(swapOverride->GetScriptByName("spell_warl_soul_swap_override"));
                if (!swapScript)
                    return;
                dotList = swapScript->GetDotList();
                swapSource = swapScript->GetOriginalSwapSource();
            }

            if (dotList.empty())
                return;

            for (std::list<uint32>::const_iterator itr = dotList.begin(); itr != dotList.end(); ++itr)
            {
                GetCaster()->AddAura(*itr, GetHitUnit());
                if (swapSource)
                    swapSource->RemoveAurasDueToSpell(*itr);
            }

            // Remove Soul Swap Exhale buff
            GetCaster()->RemoveAurasDueToSpell(SPELL_WARLOCK_SOUL_SWAP_OVERRIDE);
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_warl_soul_swap_exhale_SpellScript::CheckCast);
            OnEffectHitTarget += SpellEffectFn(spell_warl_soul_swap_exhale_SpellScript::OnEffectHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_soul_swap_exhale_SpellScript();
    }
};

// 29858 - Soulshatter
/// Updated 4.3.4
class spell_warl_soulshatter : public SpellScriptLoader
{
public:
    spell_warl_soulshatter() : SpellScriptLoader("spell_warl_soulshatter") { }

    class spell_warl_soulshatter_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_soulshatter_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_SOULSHATTER))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (Unit* target = GetHitUnit())
                if (target->CanHaveThreatList() && target->getThreatManager().getThreat(caster) > 0.0f)
                    caster->CastSpell(target, SPELL_WARLOCK_SOULSHATTER, true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_soulshatter_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warl_soulshatter_SpellScript();
    }
};

// 30108, 34438, 34439, 35183 - Unstable Affliction
/// Updated 4.3.4
class spell_warl_unstable_affliction : public SpellScriptLoader
{
public:
    spell_warl_unstable_affliction() : SpellScriptLoader("spell_warl_unstable_affliction") { }

    class spell_warl_unstable_affliction_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_unstable_affliction_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_UNSTABLE_AFFLICTION_DISPEL))
                return false;
            return true;
        }

        void HandleDispel(DispelInfo* dispelInfo)
        {
            if (Unit* caster = GetCaster())
                if (AuraEffect const* aurEff = GetEffect(EFFECT_1))
                {
                    int32 damage = aurEff->GetAmount() * 9;
                    // backfire damage and silence
                    caster->CastCustomSpell(dispelInfo->GetDispeller(), SPELL_WARLOCK_UNSTABLE_AFFLICTION_DISPEL, &damage, NULL, NULL, true, NULL, aurEff);
                }
        }

        void Register() override
        {
            AfterDispel += AuraDispelFn(spell_warl_unstable_affliction_AuraScript::HandleDispel);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_warl_unstable_affliction_AuraScript();
    }
};

void AddSC_warlock_spell_scripts()
{
    new spell_warl_chaotic_energy();

    new spell_warl_banish();

    new spell_warl_demonic_circle_summon();
    new spell_warl_demonic_circle_teleport();
    
    new spell_warl_fel_synergy();
    
    new spell_warl_healthstone_heal();
    new spell_warl_life_tap();
    new spell_warl_seed_of_corruption();
    new spell_warl_shadow_trance_proc();
    
    new spell_warl_soul_swap();
    new spell_warl_soul_swap_dot_marker();
    new spell_warl_soul_swap_exhale();
    new spell_warl_soul_swap_override();
    new spell_warl_soulshatter();
    new spell_warl_unstable_affliction();
}
