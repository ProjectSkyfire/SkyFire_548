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
 * Scripts for spells with SPELLFAMILY_DEATHKNIGHT and SPELLFAMILY_GENERIC spells used by deathknight players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_dk_".
 */

#include "Player.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"

enum DeathKnightSpells
{
    SPELL_DK_ANTI_MAGIC_SHELL_TALENT = 51052,
    SPELL_DK_BLOOD_BOIL_TRIGGERED = 65658,
    SPELL_DK_BLOOD_GORGED_HEAL = 50454,
    SPELL_DK_BLOOD_PRESENCE = 48263,
    SPELL_DK_BLOOD_SHIELD_MASTERY = 77513,
    SPELL_DK_BLOOD_SHIELD_ABSORB = 77535,
    SPELL_DK_CORPSE_EXPLOSION_TRIGGERED = 43999,
    SPELL_DK_CORPSE_EXPLOSION_VISUAL = 51270,
    SPELL_DK_DEATH_COIL_DAMAGE = 47632,
    SPELL_DK_DEATH_COIL_HEAL = 47633,
    SPELL_DK_DEATH_GRIP_ONLY_JUMP = 49575,
    SPELL_DK_DEATH_STRIKE_HEAL = 45470,
    SPELL_DK_DEATH_STRIKE_ENABLER = 89832,
    SPELL_DK_FROST_PRESENCE = 48266,
    SPELL_DK_GHOUL_EXPLODE = 47496,
    SPELL_DK_GHOUL_AS_GUARDIAN = 46585,
    SPELL_DK_GHOUL_AS_PET = 52150,
    SPELL_DK_GLYPH_OF_ICEBOUND_FORTITUDE = 58625,
    SPELL_DK_GOREFIENDS_GRASP_GRIP_VISUAL = 114869,
    SPELL_DK_IMPROVED_DEATH_STRIKE = 62905,
    SPELL_DK_ITEM_T8_MELEE_4P_BONUS = 64736,
    SPELL_DK_MASTER_OF_GHOULS = 52143,
    SPELL_DK_RUNIC_CORRUPTION_REGEN = 51460,
    SPELL_DK_RUNIC_EMPOWERMENT = 81229,
    SPELL_DK_RUNIC_POWER_ENERGIZE = 49088,
    SPELL_DK_RUNE_TAP = 48982,
    SPELL_DK_SCENT_OF_BLOOD = 50422,
    SPELL_DK_SCOURGE_STRIKE_TRIGGERED = 70890,
    SPELL_DK_UNHOLY_PRESENCE = 48265,
};

// Gorefiend's Grasp - 108199
class spell_dk_gorefiends_grasp : public SpellScriptLoader
{
public:
    spell_dk_gorefiends_grasp() : SpellScriptLoader("spell_dk_gorefiends_grasp") { }

    class spell_dk_gorefiends_grasp_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_gorefiends_grasp_SpellScript);

        void HandleOnHit()
        {
            if (Player* _player = GetCaster()->ToPlayer())
            {
                if (Unit* target = GetHitUnit())
                {
                    std::list<Unit*> tempList;
                    std::list<Unit*> gripList;

                    target->GetAttackableUnitListInRange(tempList, 20.0f);

                    for (std::list<Unit*>::iterator itr = tempList.begin(); itr != tempList.end(); ++itr)
                    {
                        Unit* tlist = (*itr);

                        if (tlist->GetGUID())
                            if (tlist->GetGUID() == target->GetGUID())
                                continue;

                        if (!_player->IsValidAttackTarget(tlist))
                            continue;

                        if (tlist->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE)
                            || tlist->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED)
                            || tlist->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED))
                            continue;

                        if (tlist->IsImmunedToSpell(sSpellMgr->GetSpellInfo(SPELL_DK_DEATH_GRIP_ONLY_JUMP)))
                            continue;

                        if (_player->HasVisionObscured(tlist))
                            continue;

                        if (!tlist->IsWithinLOSInMap(target))
                            continue;

                        gripList.push_back(tlist);
                    }

                    for (std::list<Unit*>::iterator itr = gripList.begin(); itr != gripList.end(); ++itr)
                    {
                        Unit* glist = (*itr);
                        glist->CastSpell(target, SPELL_DK_DEATH_GRIP_ONLY_JUMP, true);
                        glist->CombatStart(_player, true);
                        target->CombatStart(_player, true);
                        target->CastSpell(glist, SPELL_DK_GOREFIENDS_GRASP_GRIP_VISUAL, true);
                    }
                }
            }
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dk_gorefiends_grasp_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dk_gorefiends_grasp_SpellScript();
    }
};

// Called by Death Coil (damage) - 47632, Frost Strike - 49143 and Runic Strike - 56815
// Runic Empowerment - 81229
class spell_dk_runic_empowerment : public SpellScriptLoader
{
public:
    spell_dk_runic_empowerment() : SpellScriptLoader("spell_dk_runic_empowerment") { }

    class spell_dk_runic_empowerment_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_runic_empowerment_SpellScript);

        void HandleOnHit()
        {
            if (Player* _player = GetCaster()->ToPlayer())
            {
                if (_player->HasAura(SPELL_DK_RUNIC_EMPOWERMENT))
                {
                    if (roll_chance_i(45))
                    {
                        std::set<uint8> runes;
                        for (uint8 i = 0; i < MAX_RUNES; i++)
                            if (_player->GetRuneCooldown(i) == _player->GetRuneBaseCooldown(i))
                                runes.insert(i);
                        if (!runes.empty())
                        {
                            std::set<uint8>::iterator itr = runes.begin();
                            std::advance(itr, urand(0, runes.size() - 1));
                            _player->SetRuneCooldown((*itr), 0);
                            _player->ResyncRunes(MAX_RUNES);
                        }
                    }
                }
            }
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dk_runic_empowerment_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dk_runic_empowerment_SpellScript();
    }
};

// Called by Death Coil (damage) - 47632, Frost Strike - 49143 and Runic Strike - 56815
// Runic Corruption - 51462
class spell_dk_runic_corruption : public SpellScriptLoader
{
public:
    spell_dk_runic_corruption() : SpellScriptLoader("spell_dk_runic_corruption") { }

    class spell_dk_runic_corruption_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_runic_corruption_SpellScript);

        void HandleOnHit()
        {
            if (Player* _player = GetCaster()->ToPlayer())
            {
                if (AuraEffect* const runicCorruption = _player->GetDummyAuraEffect(SPELLFAMILY_DEATHKNIGHT, 4068, 0))
                {
                    if (roll_chance_i(45))
                    {
                        int32 basepoints0 = runicCorruption->GetAmount();
                        if (Aura* aur = _player->GetAura(SPELL_DK_RUNIC_CORRUPTION_REGEN))
                            aur->SetDuration(aur->GetDuration() + 3000);
                        else
                            _player->CastCustomSpell(_player, SPELL_DK_RUNIC_CORRUPTION_REGEN, &basepoints0, NULL, NULL, true);
                    }
                }
            }
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dk_runic_corruption_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dk_runic_corruption_SpellScript();
    }
};

// Raise Dead - 46584
class spell_dk_raise_dead : public SpellScriptLoader
{
public:
    spell_dk_raise_dead() : SpellScriptLoader("spell_dk_raise_dead") { }

    class spell_dk_raise_dead_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_raise_dead_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Player* _player = GetCaster()->ToPlayer())
            {
                if (_player->HasAura(SPELL_DK_MASTER_OF_GHOULS))
                    _player->CastSpell(_player, SPELL_DK_GHOUL_AS_PET, true);
                else
                    _player->CastSpell(_player, SPELL_DK_GHOUL_AS_GUARDIAN, true);
            }
        }

        void Register() override
        {
            OnEffectHit += SpellEffectFn(spell_dk_raise_dead_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dk_raise_dead_SpellScript();
    }
};

// 50462 - Anti-Magic Shell (on raid member)
class spell_dk_anti_magic_shell_raid : public SpellScriptLoader
{
public:
    spell_dk_anti_magic_shell_raid() : SpellScriptLoader("spell_dk_anti_magic_shell_raid") { }

    class spell_dk_anti_magic_shell_raid_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_anti_magic_shell_raid_AuraScript);

        bool Load() override
        {
            absorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
            return true;
        }

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            /// @todo this should absorb limited amount of damage, but no info on calculation formula
            amount = -1;
        }

        void Absorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
        {
            absorbAmount = CalculatePct(dmgInfo.GetDamage(), absorbPct);
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_anti_magic_shell_raid_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_shell_raid_AuraScript::Absorb, EFFECT_0);
        }
    private:
        uint32 absorbPct = 0;
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dk_anti_magic_shell_raid_AuraScript();
    }
};

// 48707 - Anti-Magic Shell (on self)
class spell_dk_anti_magic_shell_self : public SpellScriptLoader
{
public:
    spell_dk_anti_magic_shell_self() : SpellScriptLoader("spell_dk_anti_magic_shell_self") { }

    class spell_dk_anti_magic_shell_self_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_anti_magic_shell_self_AuraScript);

        bool Load() override
        {
            absorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
            hpPct = GetSpellInfo()->Effects[EFFECT_1].CalcValue(GetCaster());
            return true;
        }

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DK_RUNIC_POWER_ENERGIZE))
                return false;
            return true;
        }

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            amount = GetCaster()->CountPctFromMaxHealth(hpPct);
        }

        void Absorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
        {
            absorbAmount = std::min(CalculatePct(dmgInfo.GetDamage(), absorbPct), GetTarget()->CountPctFromMaxHealth(hpPct));
        }

        void Trigger(AuraEffect* aurEff, DamageInfo& /*dmgInfo*/, uint32& absorbAmount)
        {
            Unit* target = GetTarget();
            // damage absorbed by Anti-Magic Shell energizes the DK with additional runic power.
            // This, if I'm not mistaken, shows that we get back ~20% of the absorbed damage as runic power.
            int32 bp = absorbAmount * 2 / 10;
            target->CastCustomSpell(target, SPELL_DK_RUNIC_POWER_ENERGIZE, &bp, NULL, NULL, true, NULL, aurEff);
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_anti_magic_shell_self_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_shell_self_AuraScript::Absorb, EFFECT_0);
            AfterEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_shell_self_AuraScript::Trigger, EFFECT_0);
        }
    private:
        uint32 absorbPct = 0;
        uint32 hpPct = 0;
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dk_anti_magic_shell_self_AuraScript();
    }
};

// 50461 - Anti-Magic Zone
class spell_dk_anti_magic_zone : public SpellScriptLoader
{
public:
    spell_dk_anti_magic_zone() : SpellScriptLoader("spell_dk_anti_magic_zone") { }

    class spell_dk_anti_magic_zone_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_anti_magic_zone_AuraScript);

        bool Load() override
        {
            absorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
            return true;
        }

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DK_ANTI_MAGIC_SHELL_TALENT))
                return false;
            return true;
        }

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            SpellInfo const* talentSpell = sSpellMgr->GetSpellInfo(SPELL_DK_ANTI_MAGIC_SHELL_TALENT);
            amount = talentSpell->Effects[EFFECT_0].CalcValue(GetCaster());
            if (Player* player = GetCaster()->ToPlayer())
                amount += int32(2 * player->GetTotalAttackPowerValue(WeaponAttackType::BASE_ATTACK));
        }

        void Absorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
        {
            absorbAmount = CalculatePct(dmgInfo.GetDamage(), absorbPct);
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_anti_magic_zone_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_zone_AuraScript::Absorb, EFFECT_0);
        }
    private:
        uint32 absorbPct = 0;
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dk_anti_magic_zone_AuraScript();
    }
};

// 48721 - Blood Boil
class spell_dk_blood_boil : public SpellScriptLoader
{
public:
    spell_dk_blood_boil() : SpellScriptLoader("spell_dk_blood_boil") { }

    class spell_dk_blood_boil_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_blood_boil_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DK_BLOOD_BOIL_TRIGGERED))
                return false;
            return true;
        }

        bool Load() override
        {
            _executed = false;
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER && GetCaster()->getClass() == CLASS_DEATH_KNIGHT;
        }

        void HandleAfterHit()
        {
            if (_executed || !GetHitUnit())
                return;

            _executed = true;
            GetCaster()->CastSpell(GetCaster(), SPELL_DK_BLOOD_BOIL_TRIGGERED, true);
        }

        void Register() override
        {
            AfterHit += SpellHitFn(spell_dk_blood_boil_SpellScript::HandleAfterHit);
        }

    private:
        bool _executed = false;
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dk_blood_boil_SpellScript();
    }
};

// 50453 - Bloodworms Health Leech
class spell_dk_blood_gorged : public SpellScriptLoader
{
public:
    spell_dk_blood_gorged() : SpellScriptLoader("spell_dk_blood_gorged") { }

    class spell_dk_blood_gorged_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_blood_gorged_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DK_BLOOD_GORGED_HEAL))
                return false;
            return true;
        }

        bool Load() override
        {
            _procTarget = NULL;
            return true;
        }

        bool CheckProc(ProcEventInfo& /*eventInfo*/)
        {
            _procTarget = GetTarget()->GetOwner();
            return _procTarget;
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            int32 heal = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), 150));
            GetTarget()->CastCustomSpell(SPELL_DK_BLOOD_GORGED_HEAL, SPELLVALUE_BASE_POINT0, heal, _procTarget, true, NULL, aurEff);
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_dk_blood_gorged_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_dk_blood_gorged_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }

    private:
        Unit* _procTarget = NULL;
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dk_blood_gorged_AuraScript();
    }
};

// 47541, 52375, 59134, -62900 - Death Coil
class spell_dk_death_coil : public SpellScriptLoader
{
public:
    spell_dk_death_coil() : SpellScriptLoader("spell_dk_death_coil") { }

    class spell_dk_death_coil_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_death_coil_SpellScript);

        bool Validate(SpellInfo const* /*spell*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DK_DEATH_COIL_DAMAGE) || !sSpellMgr->GetSpellInfo(SPELL_DK_DEATH_COIL_HEAL))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            int32 damage = GetEffectValue();
            Unit* caster = GetCaster();
            if (Unit* target = GetHitUnit())
            {
                if (caster->IsFriendlyTo(target))
                {
                    int32 bp = int32(damage * 1.5f);
                    caster->CastCustomSpell(target, SPELL_DK_DEATH_COIL_HEAL, &bp, NULL, NULL, true);
                }
                else
                    caster->CastCustomSpell(target, SPELL_DK_DEATH_COIL_DAMAGE, &damage, NULL, NULL, true);
            }
        }

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            if (Unit* target = GetExplTargetUnit())
            {
                if (!caster->IsFriendlyTo(target) && !caster->isInFront(target))
                    return SpellCastResult::SPELL_FAILED_UNIT_NOT_INFRONT;

                if (target->IsFriendlyTo(caster) && target->GetCreatureType() != CREATURE_TYPE_UNDEAD)
                    return SpellCastResult::SPELL_FAILED_BAD_TARGETS;
            }
            else
                return SpellCastResult::SPELL_FAILED_BAD_TARGETS;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dk_death_coil_SpellScript::CheckCast);
            OnEffectHitTarget += SpellEffectFn(spell_dk_death_coil_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dk_death_coil_SpellScript();
    }
};

// 52751 - Death Gate
class spell_dk_death_gate : public SpellScriptLoader
{
public:
    spell_dk_death_gate() : SpellScriptLoader("spell_dk_death_gate") { }

    class spell_dk_death_gate_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_death_gate_SpellScript);

        SpellCastResult CheckClass()
        {
            if (GetCaster()->getClass() != CLASS_DEATH_KNIGHT)
            {
                SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_MUST_BE_DEATH_KNIGHT);
                return SpellCastResult::SPELL_FAILED_CUSTOM_ERROR;
            }

            return SpellCastResult::SPELL_CAST_OK;
        }

        void HandleScript(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            if (Unit* target = GetHitUnit())
                target->CastSpell(target, GetEffectValue(), false);
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dk_death_gate_SpellScript::CheckClass);
            OnEffectHitTarget += SpellEffectFn(spell_dk_death_gate_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dk_death_gate_SpellScript();
    }
};

// 49560 - Death Grip
class spell_dk_death_grip : public SpellScriptLoader
{
public:
    spell_dk_death_grip() : SpellScriptLoader("spell_dk_death_grip") { }

    class spell_dk_death_grip_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_death_grip_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            int32 damage = GetEffectValue();
            Position const* pos = GetExplTargetDest();
            if (Unit* target = GetHitUnit())
            {
                if (!target->HasAuraType(SPELL_AURA_DEFLECT_SPELLS)) // Deterrence
                    target->CastSpell(pos->GetPositionX(), pos->GetPositionY(), pos->GetPositionZ(), damage, true);
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_dk_death_grip_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dk_death_grip_SpellScript();
    }
};

// 48743 - Death Pact
class spell_dk_death_pact : public SpellScriptLoader
{
public:
    spell_dk_death_pact() : SpellScriptLoader("spell_dk_death_pact") { }

    class spell_dk_death_pact_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_death_pact_SpellScript);

        SpellCastResult CheckCast()
        {
            // Check if we have valid targets, otherwise skip spell casting here
            if (Player* player = GetCaster()->ToPlayer())
                for (Unit::ControlList::const_iterator itr = player->m_Controlled.begin(); itr != player->m_Controlled.end(); ++itr)
                    if (Creature* undeadPet = (*itr)->ToCreature())
                        if (undeadPet->IsAlive() &&
                            undeadPet->GetOwnerGUID() == player->GetGUID() &&
                            undeadPet->GetCreatureType() == CREATURE_TYPE_UNDEAD &&
                            undeadPet->IsWithinDist(player, 100.0f, false))
                            return SpellCastResult::SPELL_CAST_OK;

            return SpellCastResult::SPELL_FAILED_NO_PET;
        }

        void FilterTargets(std::list<WorldObject*>& targetList)
        {
            Unit* target = NULL;
            for (std::list<WorldObject*>::iterator itr = targetList.begin(); itr != targetList.end(); ++itr)
            {
                if (Unit* unit = (*itr)->ToUnit())
                    if (unit->GetOwnerGUID() == GetCaster()->GetGUID() && unit->GetCreatureType() == CREATURE_TYPE_UNDEAD)
                    {
                        target = unit;
                        break;
                    }
            }

            targetList.clear();
            if (target)
                targetList.push_back(target);
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dk_death_pact_SpellScript::CheckCast);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dk_death_pact_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ALLY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dk_death_pact_SpellScript();
    }
};

// 49998 - Death Strike
class spell_dk_death_strike : public SpellScriptLoader
{
public:
    spell_dk_death_strike() : SpellScriptLoader("spell_dk_death_strike") { }

    class spell_dk_death_strike_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_death_strike_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DK_DEATH_STRIKE_ENABLER) ||
                !sSpellMgr->GetSpellInfo(SPELL_DK_DEATH_STRIKE_HEAL) ||
                !sSpellMgr->GetSpellInfo(SPELL_DK_BLOOD_SHIELD_MASTERY) ||
                !sSpellMgr->GetSpellInfo(SPELL_DK_BLOOD_SHIELD_ABSORB))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (AuraEffect* enabler = GetCaster()->GetAuraEffect(SPELL_DK_DEATH_STRIKE_ENABLER, EFFECT_0, GetCaster()->GetGUID()))
            {
                // Call CalculateAmount() to constantly fire the AuraEffect's HandleCalcAmount method
                int32 heal = CalculatePct(enabler->CalculateAmount(GetCaster()), GetSpellInfo()->Effects[EFFECT_0].DamageMultiplier);

                if (AuraEffect const* aurEff = GetCaster()->GetAuraEffectOfRankedSpell(SPELL_DK_IMPROVED_DEATH_STRIKE, EFFECT_2))
                    heal = AddPct(heal, aurEff->GetAmount());

                heal = std::max(heal, int32(GetCaster()->CountPctFromMaxHealth(GetEffectValue())));
                GetCaster()->CastCustomSpell(SPELL_DK_DEATH_STRIKE_HEAL, SPELLVALUE_BASE_POINT0, heal, GetCaster(), true);
            }

            if (!GetCaster()->HasAura(SPELL_DK_BLOOD_PRESENCE))
                return;

            if (AuraEffect const* aurEff = GetCaster()->GetAuraEffect(SPELL_DK_BLOOD_SHIELD_MASTERY, EFFECT_0))
                GetCaster()->CastCustomSpell(SPELL_DK_BLOOD_SHIELD_ABSORB, SPELLVALUE_BASE_POINT0, GetCaster()->CountPctFromMaxHealth(aurEff->GetAmount()), GetCaster());
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_dk_death_strike_SpellScript::HandleDummy, EFFECT_2, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dk_death_strike_SpellScript();
    }
};

// 89832 - Death Strike (Save damage taken in last 5 sec)
class spell_dk_death_strike_enabler : public SpellScriptLoader
{
public:
    spell_dk_death_strike_enabler() : SpellScriptLoader("spell_dk_death_strike_enabler") { }

    class spell_dk_death_strike_enabler_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_death_strike_enabler_AuraScript);

        bool Load() override
        {
            for (uint8 i = 0; i < 5; ++i)
                _damagePerSecond[i] = 0;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            return eventInfo.GetDamageInfo();
        }

        void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
        {
            if (!GetUnitOwner()->HasAura(SPELL_DK_BLOOD_PRESENCE))
            {
                for (uint8 i = 0; i < 5; ++i)
                    _damagePerSecond[i] = 0;
            }
            else
                _damagePerSecond[0] += eventInfo.GetDamageInfo()->GetDamage();
        }

        // Cheap hack to have update calls
        void CalcPeriodic(AuraEffect const* /*aurEff*/, bool& isPeriodic, int32& amplitude)
        {
            isPeriodic = true;
            amplitude = 1000;
        }

        void Update(AuraEffect* /*aurEff*/)
        {
            // Move backwards all datas by one
            for (uint8 i = 4; i > 0; --i)
                _damagePerSecond[i] = _damagePerSecond[i - 1];
            _damagePerSecond[0] = 0;
        }

        void HandleCalcAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
        {
            canBeRecalculated = true;
            amount = 0;
            for (uint8 i = 0; i < 5; ++i)
                amount += int32(_damagePerSecond[i]);
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_dk_death_strike_enabler_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_dk_death_strike_enabler_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
            DoEffectCalcPeriodic += AuraEffectCalcPeriodicFn(spell_dk_death_strike_enabler_AuraScript::CalcPeriodic, EFFECT_0, SPELL_AURA_DUMMY);
            OnEffectUpdatePeriodic += AuraEffectUpdatePeriodicFn(spell_dk_death_strike_enabler_AuraScript::Update, EFFECT_0, SPELL_AURA_DUMMY);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_death_strike_enabler_AuraScript::HandleCalcAmount, EFFECT_0, SPELL_AURA_DUMMY);
        }
    private:
        uint32 _damagePerSecond[5] = { };
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dk_death_strike_enabler_AuraScript();
    }
};

// 47496 - Explode, Ghoul spell for Corpse Explosion
class spell_dk_ghoul_explode : public SpellScriptLoader
{
public:
    spell_dk_ghoul_explode() : SpellScriptLoader("spell_dk_ghoul_explode") { }

    class spell_dk_ghoul_explode_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_ghoul_explode_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DK_CORPSE_EXPLOSION_TRIGGERED))
                return false;
            return true;
        }

        void Suicide(SpellEffIndex /*effIndex*/)
        {
            if (Unit* unitTarget = GetHitUnit())
            {
                // Corpse Explosion (Suicide)
                unitTarget->CastSpell(unitTarget, SPELL_DK_CORPSE_EXPLOSION_TRIGGERED, true);
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_dk_ghoul_explode_SpellScript::Suicide, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dk_ghoul_explode_SpellScript();
    }
};

// 48792 - Icebound Fortitude
class spell_dk_icebound_fortitude : public SpellScriptLoader
{
public:
    spell_dk_icebound_fortitude() : SpellScriptLoader("spell_dk_icebound_fortitude") { }

    class spell_dk_icebound_fortitude_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_icebound_fortitude_AuraScript);

        bool Load() override
        {
            Unit* caster = GetCaster();
            return caster && caster->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* caster = GetCaster())
            {
                int32 value = amount;
                uint32 defValue = uint32(caster->ToPlayer()->GetSkillValue(SKILL_DEFENSE) + caster->ToPlayer()->GetRatingBonusValue(CombatRating::CR_DEFENSE_SKILL));

                if (defValue > 400)
                    value -= int32((defValue - 400) * 0.15);

                // Glyph of Icebound Fortitude
                if (AuraEffect const* aurEff = caster->GetAuraEffect(SPELL_DK_GLYPH_OF_ICEBOUND_FORTITUDE, EFFECT_0))
                {
                    int32 valMax = -aurEff->GetAmount();
                    if (value > valMax)
                        value = valMax;
                }
                amount = value;
            }
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_icebound_fortitude_AuraScript::CalculateAmount, EFFECT_2, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dk_icebound_fortitude_AuraScript();
    }
};

// 73975 - Necrotic Strike
class spell_dk_necrotic_strike : public SpellScriptLoader
{
public:
    spell_dk_necrotic_strike() : SpellScriptLoader("spell_dk_necrotic_strike") { }

    class spell_dk_necrotic_strike_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_necrotic_strike_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* caster = GetCaster())
                amount = int32(caster->GetTotalAttackPowerValue(WeaponAttackType::BASE_ATTACK) * 0.7f);
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_necrotic_strike_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_HEAL_ABSORB);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dk_necrotic_strike_AuraScript();
    }
};

// 59754 Rune Tap - Party
class spell_dk_rune_tap_party : public SpellScriptLoader
{
public:
    spell_dk_rune_tap_party() : SpellScriptLoader("spell_dk_rune_tap_party") { }

    class spell_dk_rune_tap_party_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_rune_tap_party_SpellScript);

        void CheckTargets(std::list<WorldObject*>& targets)
        {
            targets.remove(GetCaster());
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dk_rune_tap_party_SpellScript::CheckTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_PARTY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dk_rune_tap_party_SpellScript();
    }
};

// 50421 - Scent of Blood
class spell_dk_scent_of_blood : public SpellScriptLoader
{
public:
    spell_dk_scent_of_blood() : SpellScriptLoader("spell_dk_scent_of_blood") { }

    class spell_dk_scent_of_blood_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_scent_of_blood_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DK_SCENT_OF_BLOOD))
                return false;
            return true;
        }

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            GetTarget()->CastSpell(GetTarget(), SPELL_DK_SCENT_OF_BLOOD, true, NULL, aurEff);
            GetTarget()->RemoveAuraFromStack(GetId());
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_dk_scent_of_blood_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dk_scent_of_blood_AuraScript();
    }
};

// 55090 - Scourge Strike
class spell_dk_scourge_strike : public SpellScriptLoader
{
public:
    spell_dk_scourge_strike() : SpellScriptLoader("spell_dk_scourge_strike") { }

    class spell_dk_scourge_strike_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_scourge_strike_SpellScript);

        bool Load() override
        {
            multiplier = 1.0f;
            return true;
        }

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DK_SCOURGE_STRIKE_TRIGGERED) ||
                !sSpellMgr->GetSpellInfo(SPELL_DK_ITEM_T8_MELEE_4P_BONUS))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (Unit* unitTarget = GetHitUnit())
            {
                multiplier = (GetEffectValue() * unitTarget->GetDiseasesByCaster(caster->GetGUID()) / 100.f);
                // Death Knight T8 Melee 4P Bonus
                if (AuraEffect const* aurEff = caster->GetAuraEffect(SPELL_DK_ITEM_T8_MELEE_4P_BONUS, EFFECT_0))
                    AddPct(multiplier, aurEff->GetAmount());
            }
        }

        void HandleAfterHit()
        {
            Unit* caster = GetCaster();
            if (Unit* unitTarget = GetHitUnit())
            {
                int32 bp = GetHitDamage() * multiplier;
                caster->CastCustomSpell(unitTarget, SPELL_DK_SCOURGE_STRIKE_TRIGGERED, &bp, NULL, NULL, true);
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_dk_scourge_strike_SpellScript::HandleDummy, EFFECT_2, SPELL_EFFECT_DUMMY);
            AfterHit += SpellHitFn(spell_dk_scourge_strike_SpellScript::HandleAfterHit);
        }
    private:
        float multiplier = 0.0f;
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dk_scourge_strike_SpellScript();
    }
};

// 55233 - Vampiric Blood
class spell_dk_vampiric_blood : public SpellScriptLoader
{
public:
    spell_dk_vampiric_blood() : SpellScriptLoader("spell_dk_vampiric_blood") { }

    class spell_dk_vampiric_blood_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_vampiric_blood_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            amount = GetUnitOwner()->CountPctFromMaxHealth(amount);
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_vampiric_blood_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_INCREASE_HEALTH);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dk_vampiric_blood_AuraScript();
    }
};

void AddSC_deathknight_spell_scripts()
{
    new spell_dk_anti_magic_shell_raid();
    new spell_dk_anti_magic_shell_self();
    new spell_dk_anti_magic_zone();
    new spell_dk_blood_boil();
    new spell_dk_blood_gorged();
    new spell_dk_death_coil();
    new spell_dk_death_gate();
    new spell_dk_death_grip();
    new spell_dk_death_pact();
    new spell_dk_death_strike();
    new spell_dk_death_strike_enabler();
    new spell_dk_ghoul_explode();
    new spell_dk_icebound_fortitude();
    new spell_dk_necrotic_strike();
    new spell_dk_rune_tap_party();
    new spell_dk_scent_of_blood();
    new spell_dk_scourge_strike(); // 5.4.8 18414
    new spell_dk_vampiric_blood();
}
