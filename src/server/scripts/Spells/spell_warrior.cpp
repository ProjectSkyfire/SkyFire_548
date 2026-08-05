/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/*
 * Scripts for spells with SPELLFAMILY_WARRIOR and SPELLFAMILY_GENERIC spells used by warrior players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_warr_".
 */

#include "Player.h"
#include "Item.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Map.h"
#include "PathGenerator.h"
#include <cmath>

enum WarriorSpells
{
    SPELL_WARRIOR_BLOODTHIRST_DAMAGE                = 23881,
    SPELL_WARRIOR_BLOODTHIRST_HEAL                  = 117313,
    SPELL_WARRIOR_CHARGE_STUN                       = 7922,
    SPELL_WARRIOR_DOUBLE_TIME                       = 103827,
    SPELL_WARRIOR_DOUBLE_TIME_MARKER                = 124184,
    SPELL_WARRIOR_COLOSSUS_SMASH                    = 86346,

    SPELL_WARRIOR_LAST_STAND_TRIGGERED              = 12976, // obsolete
    SPELL_WARRIOR_RALLYING_CRY                      = 97463,
    SPELL_WARRIOR_RETALIATION_DAMAGE                = 22858,
    SPELL_WARRIOR_SECOUND_WIND_PROC_RANK_1          = 29834, // obsolete
    SPELL_WARRIOR_SECOUND_WIND_PROC_RANK_2          = 29838,
    SPELL_WARRIOR_SECOUND_WIND_TRIGGER_RANK_1       = 29841, // obsolete
    SPELL_WARRIOR_SECOND_WIND_HEAL                  = 125667, // Second Wind healing aura (server handled)

    SPELL_WARRIOR_UNBRIDLED_WRATH                   = 143268, // Arms/Fury passive
    SPELL_WARRIOR_UNBRIDLED_WRATH_EFFECT            = 29842,  // 15 Rage over 10 sec
    SPELL_WARRIOR_SHIELD_SLAM                       = 23922,
    
    SPELL_WARRIOR_SWEEPING_STRIKES_EXTRA_ATTACK     = 26654,
    SPELL_WARRIOR_SLAM                              = 1464,
    SPELL_WARRIOR_SLAM_CLEAVE                       = 146361, // SS: 35% Slam damage to enemies within 2yd

    SPELL_WARRIOR_WARBRINGER                        = 103828,
    SPELL_WARRIOR_WARBRINGER_ROOT                   = 105771,
    SPELL_WARRIOR_WARBRINGER_SLOW                   = 137637,
    SPELL_WARRIOR_BLOODSURGE                        = 46915,
    SPELL_WARRIOR_BLOODSURGE_EFFECT                 = 46916,
    SPELL_WARRIOR_THUNDER_CLAP                      = 6343,
    SPELL_WARRIOR_BLOOD_AND_THUNDER                 = 84615,
    SPELL_WARRIOR_DEEP_WOUNDS                       = 115767,
    SPELL_WARRIOR_MORTAL_STRIKE_AURA                = 12294,
    SPELL_WARRIOR_T16_DPS_4P_BONUS                  = 144441,
    SPELL_WARRIOR_T16_DPS_4P_BONUS_PROC             = 144442,

    SPELL_WARRIOR_SHOCKWAVE_STUN                    = 132168,

    SPELL_WARRIOR_VICTORIOUS                        = 32216,
    SPELL_WARRIOR_VICTORY_RUSH_HEAL                 = 118779,

    SPELL_WARRIOR_ENRAGE                            = 12880,

    SPELL_WARRIOR_TASTE_FOR_BLOOD                   = 56636, // passive
    SPELL_WARRIOR_TASTE_FOR_BLOOD_EFFECT            = 60503, // stacks enabling Overpower
    SPELL_WARRIOR_OVERPOWER                         = 7384,

    // Shield Wall / Spell Reflection cosmetic auras
    SPELL_WARRIOR_SHIELD_WALL_VISUAL_ALLIANCE       = 147925,
    SPELL_WARRIOR_SHIELD_WALL_VISUAL_HORDE          = 146127,
    SPELL_WARRIOR_SHIELD_WALL_VISUAL_WITH_SHIELD    = 146128,
    SPELL_WARRIOR_SPELL_REFLECTION_VISUAL_WITH_SHIELD = 146120,
    SPELL_WARRIOR_SPELL_REFLECTION_VISUAL_ALLIANCE  = 147923,
    SPELL_WARRIOR_SPELL_REFLECTION_VISUAL_HORDE     = 146122,
};

enum WarriorSpellIcons
{
    WARRIOR_ICON_ID_SUDDEN_DEATH                    = 1989
};

enum MiscSpells
{
    SPELL_PALADIN_BLESSING_OF_SANCTUARY             = 20911,
    SPELL_PALADIN_GREATER_BLESSING_OF_SANCTUARY     = 25899,
    SPELL_PRIEST_RENEWED_HOPE                       = 63944,
    SPELL_MAGE_GLYPH_OF_ICE_BLOCK                   = 115760 // excepted from Shattering Throw remove
};

// Bloodthirst - 23881
class spell_warr_bloodthirst : public SpellScriptLoader
{
public:
    spell_warr_bloodthirst() : SpellScriptLoader("spell_warr_bloodthirst") { }

    class spell_warr_bloodthirst_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_bloodthirst_SpellScript);

        bool Validate(SpellInfo const* /*SpellEntry*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_BLOODTHIRST_DAMAGE) ||
                !sSpellMgr->GetSpellInfo(SPELL_WARRIOR_BLOODTHIRST_HEAL))
                return false;
            return true;
        }
        void HandleOnHit()
        {
            if (Player* _player = GetCaster()->ToPlayer())
                if (Unit* target = GetHitUnit())
                    if (GetHitDamage())
                    {
                        _player->CastSpell(_player, SPELL_WARRIOR_BLOODTHIRST_HEAL, true);
                        if (_player->HasAura(SPELL_WARRIOR_BLOODSURGE))
                            if (roll_chance_i(20))
                                _player->CastSpell(_player, SPELL_WARRIOR_BLOODSURGE_EFFECT, true);
                    }
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_warr_bloodthirst_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_bloodthirst_SpellScript();
    }
};

/// Updated 4.3.4
class spell_warr_charge : public SpellScriptLoader
{
public:
    spell_warr_charge() : SpellScriptLoader("spell_warr_charge") { }

    class spell_warr_charge_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_charge_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_WARBRINGER_ROOT) ||
                !sSpellMgr->GetSpellInfo(SPELL_WARRIOR_WARBRINGER_SLOW) ||
                !sSpellMgr->GetSpellInfo(SPELL_WARRIOR_WARBRINGER) ||
                !sSpellMgr->GetSpellInfo(SPELL_WARRIOR_CHARGE_STUN) ||
                !sSpellMgr->GetSpellInfo(SPELL_WARRIOR_DOUBLE_TIME) ||
                !sSpellMgr->GetSpellInfo(SPELL_WARRIOR_DOUBLE_TIME_MARKER))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            // Double Time: Charge grants Rage only once every 12 sec
            if (caster->HasAura(SPELL_WARRIOR_DOUBLE_TIME) && caster->HasAura(SPELL_WARRIOR_DOUBLE_TIME_MARKER))
                return;

            // EFFECT_1 Dummy BasePoints are already in rage power units (e.g. 200 = 20 Rage)
            caster->EnergizeBySpell(caster, GetSpellInfo()->Id, GetEffectValue(), POWER_RAGE);

            if (caster->HasAura(SPELL_WARRIOR_DOUBLE_TIME))
                caster->CastSpell(caster, SPELL_WARRIOR_DOUBLE_TIME_MARKER, true);
        }

        void HandleCharge(SpellEffIndex /*effIndex*/)
        {
            if (Unit* target = GetHitUnit())
            {
                if (GetCaster()->HasAura(SPELL_WARRIOR_WARBRINGER))
                {
                    GetCaster()->CastSpell(target, SPELL_WARRIOR_WARBRINGER_ROOT, true);
                    GetCaster()->CastSpell(target, SPELL_WARRIOR_WARBRINGER_SLOW, true);
                }
                else
                    GetCaster()->CastSpell(target, SPELL_WARRIOR_CHARGE_STUN, true);
            }
        }

        void Register() OVERRIDE
        {
            // Rage on launch so Dummy EFFECT_1 always fires (hit-target can miss for caster-only effects)
            OnEffectLaunch += SpellEffectFn(spell_warr_charge_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
            OnEffectHitTarget += SpellEffectFn(spell_warr_charge_SpellScript::HandleCharge, EFFECT_0, SPELL_EFFECT_CHARGE);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_charge_SpellScript();
    }
};

/// Updated 4.3.4
class spell_warr_concussion_blow : public SpellScriptLoader
{
public:
    spell_warr_concussion_blow() : SpellScriptLoader("spell_warr_concussion_blow") { }

    class spell_warr_concussion_blow_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_concussion_blow_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            SetHitDamage(CalculatePct(GetCaster()->GetTotalAttackPowerValue(WeaponAttackType::BASE_ATTACK), GetEffectValue()));
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_warr_concussion_blow_SpellScript::HandleDummy, EFFECT_2, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_concussion_blow_SpellScript();
    }
};

// Called By Thunder Clap - 6343, Mortal Strike - 12294, Bloodthirst - 23881 and Devastate - 20243
// Deep Wounds - 115767
class spell_warr_deep_wounds : public SpellScriptLoader
{
public:
    spell_warr_deep_wounds() : SpellScriptLoader("spell_warr_deep_wounds") { }

    class spell_warr_deep_wounds_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_deep_wounds_SpellScript);

        void HandleOnHit()
        {
            if (Player* _player = GetCaster()->ToPlayer())
            {
                if (Unit* target = GetHitUnit())
                {
                    if (target->GetGUID() == _player->GetGUID())
                        return;

                    if (_player->getLevel() >= 32)
                        if (GetSpellInfo()->Id != SPELL_WARRIOR_THUNDER_CLAP || _player->HasAura(SPELL_WARRIOR_BLOOD_AND_THUNDER))
                            _player->CastSpell(target, SPELL_WARRIOR_DEEP_WOUNDS, true);

                    // Item - Warrior T16 DPS 4P Bonus - 144441
                    if (GetSpellInfo()->Id == SPELL_WARRIOR_MORTAL_STRIKE_AURA || GetSpellInfo()->Id == SPELL_WARRIOR_BLOODTHIRST_DAMAGE)
                        if (_player->HasAura(SPELL_WARRIOR_T16_DPS_4P_BONUS))
                            if (roll_chance_i(10))
                                _player->CastSpell(_player, SPELL_WARRIOR_T16_DPS_4P_BONUS_PROC, true);
                }
            }
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_warr_deep_wounds_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_deep_wounds_SpellScript();
    }
};

/// Updated 4.3.4
class spell_warr_execute : public SpellScriptLoader
{
public:
    spell_warr_execute() : SpellScriptLoader("spell_warr_execute") { }

    class spell_warr_execute_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_execute_SpellScript);

        void HandleEffect(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (GetHitUnit())
            {
                /*
                SpellInfo const* spellInfo = GetSpellInfo();
                int32 rageUsed = std::min<int32>(200 - spellInfo->CalcPowerCost(caster, SpellSchoolMask(spellInfo->SchoolMask)), caster->GetPower(POWER_RAGE));
                int32 newRage = std::max<int32>(0, caster->GetPower(POWER_RAGE) - rageUsed);

                // Sudden Death rage save
                if (AuraEffect* aurEff = caster->GetAuraEffect(SPELL_AURA_PROC_TRIGGER_SPELL, SPELLFAMILY_GENERIC, WARRIOR_ICON_ID_SUDDEN_DEATH, EFFECT_0))
                {
                    int32 ragesave = aurEff->GetSpellInfo()->Effects[EFFECT_0].CalcValue() * 10;
                    newRage = std::max(newRage, ragesave);
                }

                caster->SetPower(POWER_RAGE, uint32(newRage));

                /// Formula taken from the DBC: "${10+$AP*0.437*$m1/100}"
                int32 baseDamage = int32(10 + caster->GetTotalAttackPowerValue(WeaponAttackType::BASE_ATTACK) * 0.437f * GetEffectValue() / 100.0f);
                /// Formula taken from the DBC: "${$ap*0.874*$m1/100-1} = 20 rage"
                int32 moreDamage = int32(rageUsed * (caster->GetTotalAttackPowerValue(WeaponAttackType::BASE_ATTACK) * 0.874f * GetEffectValue() / 100.0f - 1) / 200);
                SetHitDamage(baseDamage + moreDamage);
                */
            }
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_warr_execute_SpellScript::HandleEffect, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_execute_SpellScript();
    }
};

// 59725 - Improved Spell Reflection
class spell_warr_improved_spell_reflection : public SpellScriptLoader
{
public:
    spell_warr_improved_spell_reflection() : SpellScriptLoader("spell_warr_improved_spell_reflection") { }

    class spell_warr_improved_spell_reflection_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_improved_spell_reflection_SpellScript);

        void FilterTargets(std::list<WorldObject*>& unitList)
        {
            if (GetCaster())
                unitList.remove(GetCaster());
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warr_improved_spell_reflection_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_improved_spell_reflection_SpellScript();
    }
};

// 5246 - Intimidating Shout
class spell_warr_intimidating_shout : public SpellScriptLoader
{
public:
    spell_warr_intimidating_shout() : SpellScriptLoader("spell_warr_intimidating_shout") { }

    class spell_warr_intimidating_shout_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_intimidating_shout_SpellScript);

        void FilterTargets(std::list<WorldObject*>& unitList)
        {
            unitList.remove(GetExplTargetWorldObject());
        }

        void Register() OVERRIDE
        {
            // MoP: primary target is EFFECT_0/1 (unit target); AoE fear/root are EFFECT_2/3 (src area enemy).
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warr_intimidating_shout_SpellScript::FilterTargets, EFFECT_2, TARGET_UNIT_SRC_AREA_ENEMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warr_intimidating_shout_SpellScript::FilterTargets, EFFECT_3, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_intimidating_shout_SpellScript();
    }
};

// 12975 - Last Stand
// MoP applies SPELL_AURA_MOD_INCREASE_HEALTH_2 with base points 30 (flat).
// Recalculate amount to 30% of the caster's max health.
class spell_warr_last_stand : public SpellScriptLoader
{
public:
    spell_warr_last_stand() : SpellScriptLoader("spell_warr_last_stand") { }

    class spell_warr_last_stand_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_last_stand_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* caster = GetCaster())
                amount = int32(caster->CountPctFromMaxHealth(amount > 0 ? amount : 30));
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warr_last_stand_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH_2);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_last_stand_AuraScript();
    }
};

// 97462 - Rallying Cry
class spell_warr_rallying_cry : public SpellScriptLoader
{
public:
    spell_warr_rallying_cry() : SpellScriptLoader("spell_warr_rallying_cry") { }

    class spell_warr_rallying_cry_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_rallying_cry_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_RALLYING_CRY))
                return false;
            return true;
        }

        bool Load() OVERRIDE
        {
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void HandleScript(SpellEffIndex /*effIndex*/)
        {
            int32 basePoints0 = int32(GetHitUnit()->CountPctFromMaxHealth(GetEffectValue()));

            GetCaster()->CastCustomSpell(GetHitUnit(), SPELL_WARRIOR_RALLYING_CRY, &basePoints0, NULL, NULL, true);
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_warr_rallying_cry_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_rallying_cry_SpellScript();
    }
};

// 94009 - Rend
class spell_warr_rend : public SpellScriptLoader
{
public:
    spell_warr_rend() : SpellScriptLoader("spell_warr_rend") { }

    class spell_warr_rend_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_rend_AuraScript);

        void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& canBeRecalculated)
        {
            if (Unit* caster = GetCaster())
            {
                canBeRecalculated = false;

                // $0.25 * (($MWB + $mwb) / 2 + $AP / 14 * $MWS) bonus per tick
                float ap = caster->GetTotalAttackPowerValue(WeaponAttackType::BASE_ATTACK);
                int32 mws = caster->GetAttackTime(WeaponAttackType::BASE_ATTACK);
                float mwbMin = caster->GetWeaponDamageRange(WeaponAttackType::BASE_ATTACK, WeaponDamageRange::MINDAMAGE);
                float mwbMax = caster->GetWeaponDamageRange(WeaponAttackType::BASE_ATTACK, WeaponDamageRange::MAXDAMAGE);
                float mwb = ((mwbMin + mwbMax) / 2 + ap * mws / 14000) * 0.25f;
                amount += int32(caster->ApplyEffectModifiers(GetSpellInfo(), aurEff->GetEffIndex(), mwb));
            }
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warr_rend_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_rend_AuraScript();
    }
};

// 20230 - Retaliation
class spell_warr_retaliation : public SpellScriptLoader
{
public:
    spell_warr_retaliation() : SpellScriptLoader("spell_warr_retaliation") { }

    class spell_warr_retaliation_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_retaliation_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_RETALIATION_DAMAGE))
                return false;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            // check attack comes not from behind
            return GetTarget()->isInFront(eventInfo.GetProcTarget(), M_PI);
        }

        void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            GetTarget()->CastSpell(eventInfo.GetProcTarget(), SPELL_WARRIOR_RETALIATION_DAMAGE, true, NULL, aurEff);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_warr_retaliation_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_warr_retaliation_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_retaliation_AuraScript();
    }
};

// 64382/65940/112997 damage casts + 64380/65941/113000 linked missiles.
// Strip Ice Block / Divine Shield before damage/armor effects so the throw both
// breaks invulnerability and deals damage (DBC pierce attrs applied in SpellMgr).
class spell_warr_shattering_throw : public SpellScriptLoader
{
public:
    spell_warr_shattering_throw() : SpellScriptLoader("spell_warr_shattering_throw") { }

    class spell_warr_shattering_throw_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_shattering_throw_SpellScript);

        void HandleBeforeHit()
        {
            if (Unit* target = GetHitUnit())
                target->RemoveAurasWithMechanic(1 << MECHANIC_IMMUNE_SHIELD, AURA_REMOVE_BY_ENEMY_SPELL, SPELL_MAGE_GLYPH_OF_ICE_BLOCK);
        }

        void Register() OVERRIDE
        {
            BeforeHit += SpellHitFn(spell_warr_shattering_throw_SpellScript::HandleBeforeHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_shattering_throw_SpellScript();
    }
};

class spell_warr_second_wind_proc : public SpellScriptLoader
{
public:
    spell_warr_second_wind_proc() : SpellScriptLoader("spell_warr_second_wind_proc") { }

    class spell_warr_second_wind_proc_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_second_wind_proc_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            // Validate the proc spell and the healing aura (Second Wind)
            if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_SECOUND_WIND_PROC_RANK_2) ||
                !sSpellMgr->GetSpellInfo(SPELL_WARRIOR_SECOND_WIND_HEAL))
                return false;
            return true;
        }

        void ApplySecondWindHeal(Unit* target, AuraEffect const* aurEff)
        {
            if (!target)
                return;

            target->CastSpell(target, SPELL_WARRIOR_SECOND_WIND_HEAL, true, NULL, aurEff);
        }

        void HandleEffectApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            if (!target || !target->IsAlive())
                return;

            if (target->HealthBelowPct(35) && !target->HasAura(SPELL_WARRIOR_SECOND_WIND_HEAL))
                ApplySecondWindHeal(target, aurEff);
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            Unit* target = GetTarget();
            if (!target || !target->IsAlive())
                return false;

            if (target->HasAura(SPELL_WARRIOR_SECOND_WIND_HEAL))
                return false;

            DamageInfo* damageInfo = eventInfo.GetDamageInfo();
            if (!damageInfo)
                return false;

            uint32 damage = damageInfo->GetDamage();
            return target->HealthBelowPctDamaged(35, damage) || target->HealthBelowPct(35);
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();

            Unit* target = GetTarget();
            if (!target || target->HasAura(SPELL_WARRIOR_SECOND_WIND_HEAL))
                return;

            ApplySecondWindHeal(target, aurEff);
        }

        void Register() OVERRIDE
        {
            AfterEffectApply += AuraEffectApplyFn(spell_warr_second_wind_proc_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            DoCheckProc += AuraCheckProcFn(spell_warr_second_wind_proc_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_warr_second_wind_proc_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_second_wind_proc_AuraScript();
    }
};

// 143268 - Unbridled Wrath (passive): on stun/immobilize, generate 15 Rage over 10 sec via 29842.
class spell_warr_unbridled_wrath : public SpellScriptLoader
{
public:
    spell_warr_unbridled_wrath() : SpellScriptLoader("spell_warr_unbridled_wrath") { }

    class spell_warr_unbridled_wrath_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_unbridled_wrath_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_UNBRIDLED_WRATH_EFFECT))
                return false;
            return true;
        }

        static bool IsStunOrImmobilize(SpellInfo const* spellInfo)
        {
            if (!spellInfo)
                return false;

            if (spellInfo->Mechanic == MECHANIC_STUN || spellInfo->Mechanic == MECHANIC_ROOT)
                return true;

            uint32 const stunRootMask = (1 << MECHANIC_STUN) | (1 << MECHANIC_ROOT);
            if (spellInfo->GetAllEffectsMechanicMask() & stunRootMask)
                return true;

            for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
            {
                if (!spellInfo->Effects[i].IsAura())
                    continue;

                AuraType const aura = AuraType(spellInfo->Effects[i].ApplyAuraName);
                if (aura == SPELL_AURA_MOD_STUN || aura == SPELL_AURA_MOD_ROOT)
                    return true;
            }

            return false;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            // Prefer stun/root on the striking spell (tooltip: struck by Stun or Immobilize)
            if (IsStunOrImmobilize(eventInfo.GetSpellInfo()))
                return true;

            // Fallback when effect data is incomplete but CC was just applied on this hit
            Unit* warrior = GetTarget();
            if (!warrior || !eventInfo.GetSpellInfo())
                return false;

            bool hasLoadedEffects = false;
            for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
            {
                if (eventInfo.GetSpellInfo()->Effects[i].IsEffect())
                {
                    hasLoadedEffects = true;
                    break;
                }
            }
            if (hasLoadedEffects)
                return false;

            return warrior->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED) || warrior->HasAuraType(SPELL_AURA_MOD_ROOT);
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();

            Unit* warrior = GetTarget();
            if (!warrior)
                return;

            warrior->CastSpell(warrior, SPELL_WARRIOR_UNBRIDLED_WRATH_EFFECT, true, NULL, aurEff);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_warr_unbridled_wrath_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_warr_unbridled_wrath_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_unbridled_wrath_AuraScript();
    }
};

// 125667 - Second Wind (heal buff). Dummy aura; force 1s ticks for 3% max HP regen while below 35%.
class spell_warr_second_wind_heal : public SpellScriptLoader
{
public:
    spell_warr_second_wind_heal() : SpellScriptLoader("spell_warr_second_wind_heal") { }

    class spell_warr_second_wind_heal_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_second_wind_heal_AuraScript);

        void CalcPeriodic(AuraEffect const* /*aurEff*/, bool& isPeriodic, int32& amplitude)
        {
            isPeriodic = true;
            amplitude = 1 * IN_MILLISECONDS;
        }

        void HandlePeriodic(AuraEffect const* aurEff)
        {
            PreventDefaultAction();

            Unit* target = GetTarget();
            if (!target || !target->IsAlive())
                return;

            if (!target->HealthBelowPct(35))
            {
                Remove(AURA_REMOVE_BY_DEFAULT);
                return;
            }

            // Keep the short DBC duration refreshed while still below the threshold.
            if (Aura* aura = GetAura())
            {
                int32 duration = aura->GetMaxDuration();
                if (duration <= 0)
                    duration = 3 * IN_MILLISECONDS;
                aura->SetDuration(duration);
            }

            uint32 healAmount = target->CountPctFromMaxHealth(aurEff->GetAmount() > 0 ? aurEff->GetAmount() : 3);
            target->HealBySpell(target, GetSpellInfo(), healAmount);
        }

        void Register() OVERRIDE
        {
            DoEffectCalcPeriodic += AuraEffectCalcPeriodicFn(spell_warr_second_wind_heal_AuraScript::CalcPeriodic, EFFECT_0, SPELL_AURA_DUMMY);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warr_second_wind_heal_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_second_wind_heal_AuraScript();
    }
};

// 52437 - Sudden Death
class spell_warr_sudden_death : public SpellScriptLoader
{
public:
    spell_warr_sudden_death() : SpellScriptLoader("spell_warr_sudden_death") { }

    class spell_warr_sudden_death_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_sudden_death_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_COLOSSUS_SMASH))
                return false;
            return true;
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            // Remove cooldown on Colossus Smash
            if (Player* player = GetTarget()->ToPlayer())
                player->RemoveSpellCooldown(SPELL_WARRIOR_COLOSSUS_SMASH, true);
        }

        void Register() OVERRIDE
        {
            AfterEffectApply += AuraEffectRemoveFn(spell_warr_sudden_death_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL); // correct?
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_sudden_death_AuraScript();
    }
};

// 12328, 18765, 35429 - Sweeping Strikes
class spell_warr_sweeping_strikes : public SpellScriptLoader
{
public:
    spell_warr_sweeping_strikes() : SpellScriptLoader("spell_warr_sweeping_strikes") { }

    class spell_warr_sweeping_strikes_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_sweeping_strikes_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_SWEEPING_STRIKES_EXTRA_ATTACK) ||
                !sSpellMgr->GetSpellInfo(SPELL_WARRIOR_SLAM_CLEAVE))
                return false;
            return true;
        }

        bool Load() OVERRIDE
        {
            _procTarget = NULL;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            _procTarget = eventInfo.GetActor()->SelectNearbyTarget(eventInfo.GetProcTarget());
            if (_procTarget)
                return true;

            // Slam cleaves everyone within 2 yards of the primary target (not caster melee range alone)
            if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
                return spellInfo->Id == SPELL_WARRIOR_SLAM && eventInfo.GetActionTarget();

            return false;
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            Unit* caster = GetTarget();
            DamageInfo* damageInfo = eventInfo.GetDamageInfo();
            if (!caster || !damageInfo || !damageInfo->GetDamage())
                return;

            // Slam: while SS is active, deal 35% damage to all other enemies within 2 yards of the target
            if (SpellInfo const* procSpell = eventInfo.GetSpellInfo())
            {
                if (procSpell->Id == SPELL_WARRIOR_SLAM)
                {
                    Unit* slamTarget = eventInfo.GetActionTarget();
                    if (!slamTarget)
                        return;

                    int32 slamDamage = CalculatePct(damageInfo->GetDamage(), 35);
                    SpellInfo const* cleaveInfo = sSpellMgr->GetSpellInfo(SPELL_WARRIOR_SLAM_CLEAVE);
                    float radius = cleaveInfo ? cleaveInfo->Effects[EFFECT_0].CalcRadius(caster) : 2.0f;
                    if (radius <= 0.0f)
                        radius = 2.0f;

                    // Area version of 146361: one cast around the Slam target (script filters the primary)
                    if (cleaveInfo && cleaveInfo->Effects[EFFECT_0].IsTargetingArea())
                    {
                        caster->CastCustomSpell(SPELL_WARRIOR_SLAM_CLEAVE, SPELLVALUE_BASE_POINT0, slamDamage, slamTarget, true, NULL, aurEff);
                        return;
                    }

                    // Fallback if DBC targets are single-unit: hit each nearby enemy for 35%
                    std::list<Unit*> targets;
                    Skyfire::AnyUnfriendlyUnitInObjectRangeCheck u_check(slamTarget, caster, radius);
                    Skyfire::UnitListSearcher<Skyfire::AnyUnfriendlyUnitInObjectRangeCheck> searcher(slamTarget, targets, u_check);
                    slamTarget->VisitNearbyObject(radius, searcher);
                    targets.remove(slamTarget);

                    for (Unit* target : targets)
                    {
                        if (!caster->IsValidAttackTarget(target) || !caster->IsWithinLOSInMap(target))
                            continue;
                        if (target->IsTotem() || target->IsSpiritService() || target->GetCreatureType() == CREATURE_TYPE_CRITTER)
                            continue;

                        caster->CastCustomSpell(SPELL_WARRIOR_SLAM_CLEAVE, SPELLVALUE_BASE_POINT0, slamDamage, target, true, NULL, aurEff);
                    }
                    return;
                }
            }

            if (!_procTarget)
                return;

            int32 damage = CalculatePct(damageInfo->GetDamage(), aurEff->GetAmount());
            caster->CastCustomSpell(SPELL_WARRIOR_SWEEPING_STRIKES_EXTRA_ATTACK, SPELLVALUE_BASE_POINT0, damage, _procTarget, true, NULL, aurEff);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_warr_sweeping_strikes_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_warr_sweeping_strikes_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }

    private:
        Unit* _procTarget;
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_sweeping_strikes_AuraScript();
    }
};

// 146361 - Slam (Sweeping Strikes cleave)
class spell_warr_slam_cleave : public SpellScriptLoader
{
public:
    spell_warr_slam_cleave() : SpellScriptLoader("spell_warr_slam_cleave") { }

    class spell_warr_slam_cleave_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_slam_cleave_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            // "all other enemies" - exclude the primary Slam target used as the cast destination
            if (WorldObject* explTarget = GetExplTargetWorldObject())
                targets.remove(explTarget);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warr_slam_cleave_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_slam_cleave_SpellScript();
    }
};

// -46951 - Sword and Board
class spell_warr_sword_and_board : public SpellScriptLoader
{
public:
    spell_warr_sword_and_board() : SpellScriptLoader("spell_warr_sword_and_board") { }

    class spell_warr_sword_and_board_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_sword_and_board_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_SHIELD_SLAM))
                return false;
            return true;
        }

        void OnProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
        {
            // Remove cooldown on Shield Slam
            if (Player* player = GetTarget()->ToPlayer())
                player->RemoveSpellCooldown(SPELL_WARRIOR_SHIELD_SLAM, true);
        }

        void Register() OVERRIDE
        {
            // MoP talent 46953 uses SPELL_AURA_DUMMY, not PROC_TRIGGER_SPELL.
            OnEffectProc += AuraEffectProcFn(spell_warr_sword_and_board_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_sword_and_board_AuraScript();
    }
};

// 34428 - Victory Rush
// Dummy + school damage; heal is a separate MoP spell. Consumes Victorious so the
// ability cannot be spammed for the rest of the buff window.
class spell_warr_victory_rush : public SpellScriptLoader
{
public:
    spell_warr_victory_rush() : SpellScriptLoader("spell_warr_victory_rush") { }

    class spell_warr_victory_rush_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_victory_rush_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_VICTORIOUS) ||
                !sSpellMgr->GetSpellInfo(SPELL_WARRIOR_VICTORY_RUSH_HEAL))
                return false;
            return true;
        }

        void HandleAfterCast()
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            // Heal applies even on miss/dodge/parry (retail).
            caster->CastSpell(caster, SPELL_WARRIOR_VICTORY_RUSH_HEAL, true);
            caster->RemoveAurasDueToSpell(SPELL_WARRIOR_VICTORIOUS);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_warr_victory_rush_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_victory_rush_SpellScript();
    }
};

// 46968 - Shockwave
class spell_warr_shockwave : public SpellScriptLoader
{
public:
    spell_warr_shockwave() : SpellScriptLoader("spell_warr_shockwave") { }

    class spell_warr_shockwave_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_shockwave_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_SHOCKWAVE_STUN))
                return false;
            return true;
        }

        uint32 m_targetCount = 0;

        void CountTargets(std::list<WorldObject*>& targetList)
        {
            m_targetCount = targetList.size();
        }

        void HandleCast()
        {
            if (m_targetCount >= 3)
                if (GetCaster() && GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER)
                    GetCaster()->ToPlayer()->ModifySpellCooldown(GetSpellInfo()->Id, -(GetSpellInfo()->Effects[EFFECT_3].BasePoints * IN_MILLISECONDS));
        }

        void HandleHit(SpellEffIndex)
        {
            GetCaster()->CastSpell(GetHitUnit(), SPELL_WARRIOR_SHOCKWAVE_STUN, true);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warr_shockwave_SpellScript::CountTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_104);
            AfterCast += SpellCastFn(spell_warr_shockwave_SpellScript::HandleCast);
            OnEffectHitTarget += SpellEffectFn(spell_warr_shockwave_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warr_shockwave_SpellScript();
    }
};

// 18499 - Berserker Rage
// MoP: "You become Enraged" — apply Enrage (12880) for rage + physical damage amp.
class spell_warr_berserker_rage : public SpellScriptLoader
{
public:
    spell_warr_berserker_rage() : SpellScriptLoader("spell_warr_berserker_rage") { }

    class spell_warr_berserker_rage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_berserker_rage_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_ENRAGE))
                return false;
            return true;
        }

        void HandleAfterCast()
        {
            if (Unit* caster = GetCaster())
                caster->CastSpell(caster, SPELL_WARRIOR_ENRAGE, true);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_warr_berserker_rage_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_berserker_rage_SpellScript();
    }
};

// 871 - Shield Wall
// 23920 - Spell Reflection
// Applies faction / shield-equipped cosmetic auras while the defensive buff is active.
class spell_warr_shield_visual : public SpellScriptLoader
{
public:
    spell_warr_shield_visual() : SpellScriptLoader("spell_warr_shield_visual") { }

    class spell_warr_shield_visual_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_shield_visual_AuraScript);

        uint32 _spellWithShield = 0;
        uint32 _spellAlliance = 0;
        uint32 _spellHorde = 0;

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_WARRIOR_SHIELD_WALL_VISUAL_ALLIANCE)
                && sSpellMgr->GetSpellInfo(SPELL_WARRIOR_SHIELD_WALL_VISUAL_HORDE)
                && sSpellMgr->GetSpellInfo(SPELL_WARRIOR_SHIELD_WALL_VISUAL_WITH_SHIELD)
                && sSpellMgr->GetSpellInfo(SPELL_WARRIOR_SPELL_REFLECTION_VISUAL_WITH_SHIELD)
                && sSpellMgr->GetSpellInfo(SPELL_WARRIOR_SPELL_REFLECTION_VISUAL_ALLIANCE)
                && sSpellMgr->GetSpellInfo(SPELL_WARRIOR_SPELL_REFLECTION_VISUAL_HORDE);
        }

        bool Load() OVERRIDE
        {
            bool const isShieldWall = GetId() == 871;
            _spellWithShield = isShieldWall
                ? SPELL_WARRIOR_SHIELD_WALL_VISUAL_WITH_SHIELD
                : SPELL_WARRIOR_SPELL_REFLECTION_VISUAL_WITH_SHIELD;
            _spellAlliance = isShieldWall
                ? SPELL_WARRIOR_SHIELD_WALL_VISUAL_ALLIANCE
                : SPELL_WARRIOR_SPELL_REFLECTION_VISUAL_ALLIANCE;
            _spellHorde = isShieldWall
                ? SPELL_WARRIOR_SHIELD_WALL_VISUAL_HORDE
                : SPELL_WARRIOR_SPELL_REFLECTION_VISUAL_HORDE;
            return true;
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Player* warrior = GetUnitOwner()->ToPlayer();
            if (!warrior)
                return;

            uint32 spellId = 0;
            if (Item* offhand = warrior->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND))
                if (ItemTemplate const* proto = offhand->GetTemplate())
                    if (proto->Class == ITEM_CLASS_ARMOR && proto->SubClass == ITEM_SUBCLASS_ARMOR_SHIELD)
                        spellId = _spellWithShield;

            if (!spellId)
                spellId = (warrior->getRaceMask() & RACEMASK_ALLIANCE) ? _spellAlliance : _spellHorde;

            warrior->CastSpell(warrior, spellId, true);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* owner = GetUnitOwner();
            owner->RemoveAurasDueToSpell(_spellWithShield);
            owner->RemoveAurasDueToSpell(_spellAlliance);
            owner->RemoveAurasDueToSpell(_spellHorde);
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_warr_shield_visual_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_warr_shield_visual_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_shield_visual_AuraScript();
    }
};

// 23920 - Spell Reflection
// 114028 - Mass Spell Reflection
// MoP 5.3: reflection is not consumed by spells from player pets/guardians.
class spell_warr_spell_reflection : public SpellScriptLoader
{
public:
    spell_warr_spell_reflection() : SpellScriptLoader("spell_warr_spell_reflection") { }

    class spell_warr_spell_reflection_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_spell_reflection_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            Unit* actor = eventInfo.GetActor();
            if (!actor)
                return false;

            Unit* owner = actor->GetOwner();
            return !owner || owner->GetTypeId() != TypeID::TYPEID_PLAYER;
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_warr_spell_reflection_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_spell_reflection_AuraScript();
    }
};

// 56636 - Taste for Blood (Arms passive)
// Mortal Strike hit -> 2 stacks of 60503; target dodge -> 1 stack. Max 5, 12 sec.
class spell_warr_taste_for_blood : public SpellScriptLoader
{
public:
    spell_warr_taste_for_blood() : SpellScriptLoader("spell_warr_taste_for_blood") { }

    class spell_warr_taste_for_blood_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_taste_for_blood_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_TASTE_FOR_BLOOD_EFFECT) ||
                !sSpellMgr->GetSpellInfo(SPELL_WARRIOR_MORTAL_STRIKE_AURA))
                return false;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (eventInfo.GetActor() == eventInfo.GetActionTarget())
                return false;

            if (eventInfo.GetHitMask() & PROC_EX_DODGE)
                return true;

            if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
                if (spellInfo->Id == SPELL_WARRIOR_MORTAL_STRIKE_AURA)
                    return true;

            return false;
        }

        void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            Unit* caster = eventInfo.GetActor();
            if (!caster)
                return;

            int32 stacks = 0;
            if (eventInfo.GetHitMask() & PROC_EX_DODGE)
                stacks = GetSpellInfo()->Effects[EFFECT_0].CalcValue(caster);
            else
                stacks = GetSpellInfo()->Effects[EFFECT_1].CalcValue(caster);

            if (stacks <= 0)
                return;

            // Avoid SPELLVALUE_AURA_STACK on refresh (core adds +1 then ModStackAmount again).
            if (Aura* aura = caster->GetAura(SPELL_WARRIOR_TASTE_FOR_BLOOD_EFFECT))
            {
                aura->ModStackAmount(stacks);
            }
            else if (stacks > 1)
                caster->CastCustomSpell(SPELL_WARRIOR_TASTE_FOR_BLOOD_EFFECT, SPELLVALUE_AURA_STACK, stacks, caster, true);
            else
                caster->CastSpell(caster, SPELL_WARRIOR_TASTE_FOR_BLOOD_EFFECT, true);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_warr_taste_for_blood_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_warr_taste_for_blood_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_taste_for_blood_AuraScript();
    }
};

// 7384 - Overpower
// Consumes one Taste for Blood stack and reduces Mortal Strike cooldown by 0.5 sec.
class spell_warr_overpower : public SpellScriptLoader
{
public:
    spell_warr_overpower() : SpellScriptLoader("spell_warr_overpower") { }

    class spell_warr_overpower_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_overpower_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_TASTE_FOR_BLOOD_EFFECT) ||
                !sSpellMgr->GetSpellInfo(SPELL_WARRIOR_MORTAL_STRIKE_AURA))
                return false;
            return true;
        }

        void HandleAfterCast()
        {
            Player* player = GetCaster() ? GetCaster()->ToPlayer() : NULL;
            if (!player)
                return;

            if (Aura* taste = player->GetAura(SPELL_WARRIOR_TASTE_FOR_BLOOD_EFFECT))
                taste->ModStackAmount(-1);

            if (player->HasSpellCooldown(SPELL_WARRIOR_MORTAL_STRIKE_AURA))
                player->ModifySpellCooldown(SPELL_WARRIOR_MORTAL_STRIKE_AURA, -500);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_warr_overpower_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_overpower_SpellScript();
    }
};

// 6544 - Heroic Leap (reject invalid destinations the client already paints as unusable)
class spell_warr_heroic_leap : public SpellScriptLoader
{
public:
    spell_warr_heroic_leap() : SpellScriptLoader("spell_warr_heroic_leap") { }

    class spell_warr_heroic_leap_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_heroic_leap_SpellScript);

        SpellCastResult CheckDestination()
        {
            Unit* caster = GetCaster();
            WorldLocation const* dest = GetExplTargetDest();
            if (!dest)
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;

            if (caster->HasUnitState(UNIT_STATE_ROOT))
                return SpellCastResult::SPELL_FAILED_ROOTED;

            // Cannot leap onto elevated ledges / up vertical faces (client paints these invalid).
            if (dest->GetPositionZ() > caster->GetPositionZ() + 4.0f)
                return SpellCastResult::SPELL_FAILED_NOPATH;

            // Siegecrafter Blackfuse platforms (SoO) — LoA parity.
            if (caster->GetMapId() == 1136 && dest->GetPositionZ() > -308.0f && dest->GetPositionZ() < -290.0f
                && dest->GetPositionX() > 1900.0f)
                return SpellCastResult::SPELL_FAILED_NOPATH;

            // Wall / cliff-face clicks: client dest Z sits mid-face while map ground
            // under that XY is the floor far below (or missing). Matches the cursor
            // "circle with a line" visual the client already shows as unusable.
            float const ground = caster->GetMap()->GetHeight(caster->GetPhaseMask(),
                dest->GetPositionX(), dest->GetPositionY(), dest->GetPositionZ() + 2.0f, true);
            if (ground <= INVALID_HEIGHT || std::fabs(dest->GetPositionZ() - ground) > 2.5f)
                return SpellCastResult::SPELL_FAILED_NOPATH;

            // Instances: require a navmesh path (TC parity). Outdoor skips this so
            // leaps across gaps / down cliffs still work without a walkable path.
            if (caster->GetMap()->Instanceable())
            {
                float const range = GetSpellInfo()->GetMaxRange(true, caster) * 1.5f;
                PathGenerator path(caster);
                path.SetPathLengthLimit(range);
                bool const ok = path.CalculatePath(dest->GetPositionX(), dest->GetPositionY(), dest->GetPositionZ());
                if (path.GetPathType() & PATHFIND_SHORT)
                    return SpellCastResult::SPELL_FAILED_OUT_OF_RANGE;
                if (!ok || (path.GetPathType() & PATHFIND_NOPATH))
                    return SpellCastResult::SPELL_FAILED_NOPATH;
            }

            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_warr_heroic_leap_SpellScript::CheckDestination);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_heroic_leap_SpellScript();
    }
};

void AddSC_warrior_spell_scripts()
{
    new spell_warr_bloodthirst();
    new spell_warr_charge();
    new spell_warr_concussion_blow();
    new spell_warr_deep_wounds();
    new spell_warr_execute();
    new spell_warr_heroic_leap();
    new spell_warr_improved_spell_reflection();
    new spell_warr_intimidating_shout();
    new spell_warr_last_stand();
    new spell_warr_rallying_cry();
    new spell_warr_retaliation();
    new spell_warr_second_wind_proc();
    new spell_warr_second_wind_heal();
    new spell_warr_unbridled_wrath();
    new spell_warr_shattering_throw();
    new spell_warr_sudden_death();
    new spell_warr_sweeping_strikes();
    new spell_warr_slam_cleave();
    new spell_warr_sword_and_board();
    new spell_warr_victory_rush();
    new spell_warr_shockwave();
    new spell_warr_berserker_rage();
    new spell_warr_shield_visual();
    new spell_warr_spell_reflection();
    new spell_warr_taste_for_blood();
    new spell_warr_overpower();
}




