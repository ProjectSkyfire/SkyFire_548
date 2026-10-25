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
#include "Creature.h"
#include "Spell.h"
#include "Item.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Map.h"
#include "MoveSpline.h"
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

    SPELL_WARRIOR_VICTORIOUS_T15                    = 138279,
    SPELL_WARRIOR_IMPENDING_VICTORY                 = 103840,
    SPELL_WARRIOR_IMPENDING_VICTORY_HEAL            = 118340,
    SPELL_WARRIOR_GLYPH_OF_VICTORY_RUSH             = 58382,
    SPELL_WARRIOR_GLYPH_OF_HINDERING_STRIKES        = 58366,
    SPELL_WARRIOR_SLUGGISH                          = 129923,
    SPELL_WARRIOR_SHIELD_BLOCK_TRIGGERED            = 132404,
    SPELL_WARRIOR_STORM_BOLT_STUN                   = 132169,
    SPELL_WARRIOR_MOCKING_BANNER_TAUNT              = 114198,
    SPELL_WARRIOR_DEFENSIVE_STANCE                  = 71,
    SPELL_WARRIOR_REVENGE                           = 6572,
    SPELL_WARRIOR_WEAKENED_BLOWS                    = 115798,
    SPELL_WARRIOR_GLYPH_OF_BURNING_ANGER            = 115946,
    SPELL_WARRIOR_BURNING_ANGER_VISUAL              = 115993,
    SPELL_WARRIOR_OPPORTUNITY_STRIKE                = 76858,
    SPELL_WARRIOR_BLOODBATH_DOT                     = 113344,
    SPELL_WARRIOR_BLOODBATH_SLOW                    = 147531,
    SPELL_WARRIOR_EXECUTE                           = 5308,
    SPELL_WARRIOR_SUDDEN_DEATH                      = 52437,
    SPELL_WARRIOR_SUDDEN_EXECUTE                    = 139958,
    SPELL_WARRIOR_MEAT_CLEAVER                      = 85739,
    SPELL_WARRIOR_RAGING_BLOW_ENABLER               = 131116,
    SPELL_WARRIOR_GLYPH_OF_COLOSSUS_SMASH           = 89003,
    SPELL_WARRIOR_PHYSICAL_VULNERABILITY            = 81326,
    SPELL_WARRIOR_WEAKENED_ARMOR                    = 113746,
    SPELL_WARRIOR_DIE_BY_THE_SWORD                  = 118038,
    SPELL_WARRIOR_GLYPH_OF_DIE_BY_THE_SWORD         = 58386,
    SPELL_WARRIOR_DEMORALIZING_SHOUT                = 1160,
    SPELL_WARRIOR_SHIELD_BARRIER                    = 112048,
    SPELL_WARRIOR_COLOSSAL_RAGE                     = 144438,
    SPELL_WARRIOR_T16_PROTECTION_2P_BONUS           = 144503,
    SPELL_WARRIOR_T16_PROTECTION_4P_BONUS           = 144502,
    SPELL_WARRIOR_SHIELD_GUARD                      = 144551,
    SPELL_WARRIOR_RECKLESS_DEFENSE                  = 144500,
    SPELL_WARRIOR_RECKLESS_DEFENSE_ENERGIZE         = 144499,
    SPELL_WARRIOR_DRAGON_ROAR_KNOCK_BACK            = 118895,
    SPELL_WARRIOR_PVP_SET_4P                        = 133277,
    SPELL_WARRIOR_HEROIC_LEAP_SPEED                 = 133278,
    SPELL_WARRIOR_GLYPH_OF_MORTAL_STRIKE            = 58368,
    SPELL_WARRIOR_GLYPH_OF_BLOODY_HEALING           = 58369,
    SPELL_WARRIOR_BLOODY_HEALING                    = 126665,
    SPELL_WARRIOR_GLYPH_OF_THE_BLAZING_TRAIL        = 123779,
};

enum WarriorCreatures
{
    NPC_WARRIOR_MOCKING_BANNER                      = 59390
};

enum WarriorSpellIcons
{
    WARRIOR_ICON_ID_SUDDEN_DEATH                    = 1989
};

enum WarriorSpellVisuals
{
    WARRIOR_VISUAL_BLAZING_TRAIL                    = 26423
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

        // Glyph of the Blazing Trail
        void HandleBlazingTrail(SpellEffIndex effIndex)
        {
            if (!GetCaster()->HasAura(SPELL_WARRIOR_GLYPH_OF_THE_BLAZING_TRAIL))
                PreventHitDefaultEffect(effIndex);
        }

        void Register() OVERRIDE
        {
            // Rage on launch so Dummy EFFECT_1 always fires (hit-target can miss for caster-only effects)
            OnEffectLaunch += SpellEffectFn(spell_warr_charge_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
            OnEffectHitTarget += SpellEffectFn(spell_warr_charge_SpellScript::HandleCharge, EFFECT_0, SPELL_EFFECT_CHARGE);
            OnEffectLaunch += SpellEffectFn(spell_warr_charge_SpellScript::HandleBlazingTrail, EFFECT_2, SPELL_EFFECT_TRIGGER_SPELL);
            OnEffectLaunchTarget += SpellEffectFn(spell_warr_charge_SpellScript::HandleBlazingTrail, EFFECT_2, SPELL_EFFECT_TRIGGER_SPELL);
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
                            {
                                // Above 20% health Death Sentence only enables Execute, it does not make it free
                                if (target->HealthAbovePct(20))
                                    _player->CastCustomSpell(SPELL_WARRIOR_T16_DPS_4P_BONUS_PROC, SPELLVALUE_BASE_POINT1, 0, _player, true);
                                else
                                    _player->CastSpell(_player, SPELL_WARRIOR_T16_DPS_4P_BONUS_PROC, true);
                            }
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
            if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_ENRAGE) ||
                !sSpellMgr->GetSpellInfo(SPELL_WARRIOR_RAGING_BLOW_ENABLER))
                return false;
            return true;
        }

        void HandleAfterCast()
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            caster->CastSpell(caster, SPELL_WARRIOR_ENRAGE, true);

            // Becoming Enraged enables Raging Blow (crit Enrage procs are handled by the core).
            if (Player* player = caster->ToPlayer())
                if (player->GetTalentSpecialization(player->GetActiveSpec()) == SPEC_WARRIOR_FURY && player->getLevel() >= 30)
                    player->CastSpell(player, SPELL_WARRIOR_RAGING_BLOW_ENABLER, true);
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

            // Siegecrafter Blackfuse platforms (SoO) can't be leapt onto
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

// 32216 - Victorious
// 138279 - Victorious (T15 Protection 2P)
class spell_warr_victorious_state : public SpellScriptLoader
{
public:
    spell_warr_victorious_state() : SpellScriptLoader("spell_warr_victorious_state") { }

    class spell_warr_victorious_state_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_victorious_state_SpellScript);

        void HandleOnHit()
        {
            if (Player* player = GetCaster()->ToPlayer())
                player->RemoveSpellCooldown(SPELL_WARRIOR_IMPENDING_VICTORY, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_warr_victorious_state_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_victorious_state_SpellScript();
    }
};

// 78 - Heroic Strike, 845 - Cleave
// Glyph of Hindering Strikes - 58366
class spell_warr_glyph_of_hindering_strikes : public SpellScriptLoader
{
public:
    spell_warr_glyph_of_hindering_strikes() : SpellScriptLoader("spell_warr_glyph_of_hindering_strikes") { }

    class spell_warr_glyph_of_hindering_strikes_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_glyph_of_hindering_strikes_SpellScript);

        void HandleOnHit()
        {
            Unit* caster = GetCaster();
            if (Unit* target = GetHitUnit())
                if (caster->HasAura(SPELL_WARRIOR_GLYPH_OF_HINDERING_STRIKES))
                    caster->CastSpell(target, SPELL_WARRIOR_SLUGGISH, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_warr_glyph_of_hindering_strikes_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_glyph_of_hindering_strikes_SpellScript();
    }
};

// 78 - Heroic Strike, 845 - Cleave
// Deal 40% more damage while wielding a one-handed weapon.
class spell_warr_heroic_strike_one_hand : public SpellScriptLoader
{
public:
    spell_warr_heroic_strike_one_hand() : SpellScriptLoader("spell_warr_heroic_strike_one_hand") { }

    class spell_warr_heroic_strike_one_hand_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_heroic_strike_one_hand_SpellScript);

        void HandleOnHit()
        {
            if (Player* warrior = GetCaster()->ToPlayer())
                if (Item* item = warrior->GetWeaponForAttack(WeaponAttackType::BASE_ATTACK))
                    if (item->GetTemplate()->InventoryType != INVTYPE_2HWEAPON)
                        SetHitDamage(int32(GetHitDamage() * 1.4f));
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_warr_heroic_strike_one_hand_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_heroic_strike_one_hand_SpellScript();
    }
};

// 122294 - Stampeding Shout
class spell_warr_stampeding_shout : public SpellScriptLoader
{
public:
    spell_warr_stampeding_shout() : SpellScriptLoader("spell_warr_stampeding_shout") { }

    class spell_warr_stampeding_shout_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_stampeding_shout_SpellScript);

        void HandleOnHit()
        {
            if (Unit* target = GetHitUnit())
                target->RemoveMovementImpairingAuras();
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_warr_stampeding_shout_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_stampeding_shout_SpellScript();
    }
};

// 2565 - Shield Block
class spell_warr_shield_block : public SpellScriptLoader
{
public:
    spell_warr_shield_block() : SpellScriptLoader("spell_warr_shield_block") { }

    class spell_warr_shield_block_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_shield_block_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_WARRIOR_SHIELD_BLOCK_TRIGGERED) != NULL;
        }

        void HandleOnHit()
        {
            GetCaster()->CastSpell(GetCaster(), SPELL_WARRIOR_SHIELD_BLOCK_TRIGGERED, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_warr_shield_block_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_shield_block_SpellScript();
    }
};

// 107570 - Storm Bolt
class spell_warr_storm_bolt : public SpellScriptLoader
{
public:
    spell_warr_storm_bolt() : SpellScriptLoader("spell_warr_storm_bolt") { }

    class spell_warr_storm_bolt_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_storm_bolt_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_WARRIOR_STORM_BOLT_STUN) != NULL;
        }

        void HandleStun(SpellEffIndex /*effIndex*/)
        {
            if (Unit* target = GetHitUnit())
                GetCaster()->CastSpell(target, SPELL_WARRIOR_STORM_BOLT_STUN, true);
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_warr_storm_bolt_SpellScript::HandleStun, EFFECT_1, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_storm_bolt_SpellScript();
    }
};

// 107570 - Storm Bolt, 145585 - Storm Bolt Off-Hand
// Quadruple damage against creatures immune to the stun.
class spell_warr_storm_bolt_damage : public SpellScriptLoader
{
public:
    spell_warr_storm_bolt_damage() : SpellScriptLoader("spell_warr_storm_bolt_damage") { }

    class spell_warr_storm_bolt_damage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_storm_bolt_damage_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_WARRIOR_STORM_BOLT_STUN) != NULL;
        }

        void HandleOnHit()
        {
            Unit* target = GetHitUnit();
            if (!target || target->GetTypeId() == TypeID::TYPEID_PLAYER)
                return;

            if (target->IsImmunedToSpellEffect(sSpellMgr->GetSpellInfo(SPELL_WARRIOR_STORM_BOLT_STUN), EFFECT_0))
                SetHitDamage(GetHitDamage() * 4);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_warr_storm_bolt_damage_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_storm_bolt_damage_SpellScript();
    }
};

// 107566 - Staggering Shout (roots only targets that are already snared)
class spell_warr_staggering_shout : public SpellScriptLoader
{
public:
    spell_warr_staggering_shout() : SpellScriptLoader("spell_warr_staggering_shout") { }

    class spell_warr_staggering_shout_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_staggering_shout_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if([](WorldObject* object)
            {
                Unit* unit = object->ToUnit();
                return !unit || !unit->HasAuraType(SPELL_AURA_MOD_DECREASE_SPEED);
            });
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warr_staggering_shout_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_staggering_shout_SpellScript();
    }
};

// 114192 - Mocking Banner
class spell_warr_mocking_banner : public SpellScriptLoader
{
public:
    spell_warr_mocking_banner() : SpellScriptLoader("spell_warr_mocking_banner") { }

    class spell_warr_mocking_banner_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_mocking_banner_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_WARRIOR_MOCKING_BANNER_TAUNT) != NULL;
        }

        void OnTick(AuraEffect const* /*aurEff*/)
        {
            Player* player = GetTarget()->ToPlayer();
            if (!player)
                return;

            std::list<Creature*> banners;
            player->GetCreatureListWithEntryInGrid(banners, NPC_WARRIOR_MOCKING_BANNER, 30.0f);

            for (Creature* banner : banners)
                if (banner->IsSummon() && banner->GetOwner() == player)
                    player->CastSpell(banner, SPELL_WARRIOR_MOCKING_BANNER_TAUNT, true);
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warr_mocking_banner_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_mocking_banner_AuraScript();
    }
};

// 6343 - Thunder Clap
class spell_warr_thunder_clap : public SpellScriptLoader
{
public:
    spell_warr_thunder_clap() : SpellScriptLoader("spell_warr_thunder_clap") { }

    class spell_warr_thunder_clap_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_thunder_clap_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_WARRIOR_WEAKENED_BLOWS) != NULL;
        }

        void HandleOnHit()
        {
            if (Unit* target = GetHitUnit())
                GetCaster()->CastSpell(target, SPELL_WARRIOR_WEAKENED_BLOWS, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_warr_thunder_clap_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_thunder_clap_SpellScript();
    }
};

// 145674 - Riposte (crit rating equal to 75% of parry + dodge rating)
class spell_warr_riposte : public SpellScriptLoader
{
public:
    spell_warr_riposte() : SpellScriptLoader("spell_warr_riposte") { }

    class spell_warr_riposte_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_riposte_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
        {
            canBeRecalculated = false;

            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (Player* player = caster->ToPlayer())
            {
                uint32 rating = player->GetUInt32Value(PLAYER_FIELD_COMBAT_RATINGS + uint8(CombatRating::CR_PARRY));
                rating += player->GetUInt32Value(PLAYER_FIELD_COMBAT_RATINGS + uint8(CombatRating::CR_DODGE));
                amount = int32(CalculatePct(rating, 75));
            }
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warr_riposte_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_RATING);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_riposte_AuraScript();
    }
};

// 6572 - Revenge, 23922 - Shield Slam
// Rage is only generated in Defensive Stance.
class spell_warr_revenge_shield_slam : public SpellScriptLoader
{
public:
    spell_warr_revenge_shield_slam() : SpellScriptLoader("spell_warr_revenge_shield_slam") { }

    class spell_warr_revenge_shield_slam_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_revenge_shield_slam_SpellScript);

        void HandleEnergize(SpellEffIndex effIndex)
        {
            if (!GetCaster()->HasAura(SPELL_WARRIOR_DEFENSIVE_STANCE))
                PreventHitEffect(effIndex);
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_warr_revenge_shield_slam_SpellScript::HandleEnergize, EFFECT_ALL, SPELL_EFFECT_ENERGIZE);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_revenge_shield_slam_SpellScript();
    }
};

// 5302 - Revenge!
class spell_warr_revenge : public SpellScriptLoader
{
public:
    spell_warr_revenge() : SpellScriptLoader("spell_warr_revenge") { }

    class spell_warr_revenge_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_revenge_SpellScript);

        void HandleCast()
        {
            if (Player* player = GetCaster()->ToPlayer())
                player->RemoveSpellCooldown(SPELL_WARRIOR_REVENGE, true);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_warr_revenge_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_revenge_SpellScript();
    }
};

// 23922 - Shield Slam (attack power scaling)
class spell_warr_shield_slam : public SpellScriptLoader
{
public:
    spell_warr_shield_slam() : SpellScriptLoader("spell_warr_shield_slam") { }

    class spell_warr_shield_slam_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_shield_slam_SpellScript);

        void CalculateDamage(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            uint8 level = caster->getLevel();
            float ap = caster->GetTotalAttackPowerValue(WeaponAttackType::BASE_ATTACK);
            float apMult = 0.35f;
            if (level >= 80)
                apMult += 0.4f;
            if (level >= 85)
                apMult += 0.75f;
            SetEffectValue(int32(ap * apMult) + GetEffectValue());
        }

        void Register() OVERRIDE
        {
            OnEffectLaunchTarget += SpellEffectFn(spell_warr_shield_slam_SpellScript::CalculateDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_shield_slam_SpellScript();
    }
};

// 76856 - Mastery: Unshackled Fury
class spell_warr_unshackled_fury : public SpellScriptLoader
{
public:
    spell_warr_unshackled_fury() : SpellScriptLoader("spell_warr_unshackled_fury") { }

    class spell_warr_unshackled_fury_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_unshackled_fury_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            // The mastery bonus only applies while Enraged and is carried by Enrage (12880) itself
            // (spell_mastery_unshackled_fury); this aura is only recalculated on stat updates,
            // so it cannot follow the Enrage state and must not grant a permanent bonus.
            amount = 0;
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warr_unshackled_fury_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_unshackled_fury_AuraScript();
    }
};

// 12880 - Enrage
// Glyph of Burning Anger - 115946
class spell_warr_glyph_of_burning_anger : public SpellScriptLoader
{
public:
    spell_warr_glyph_of_burning_anger() : SpellScriptLoader("spell_warr_glyph_of_burning_anger") { }

    class spell_warr_glyph_of_burning_anger_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_glyph_of_burning_anger_SpellScript);

        void HandleBurning(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (Unit* target = GetHitUnit())
                if (caster->HasAura(SPELL_WARRIOR_GLYPH_OF_BURNING_ANGER))
                    caster->CastSpell(target, SPELL_WARRIOR_BURNING_ANGER_VISUAL, true);
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_warr_glyph_of_burning_anger_SpellScript::HandleBurning, EFFECT_0, SPELL_EFFECT_ENERGIZE);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_glyph_of_burning_anger_SpellScript();
    }
};

// 85288 - Raging Blow
class spell_warr_raging_blow : public SpellScriptLoader
{
public:
    spell_warr_raging_blow() : SpellScriptLoader("spell_warr_raging_blow") { }

    class spell_warr_raging_blow_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_raging_blow_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            // Meat Cleaver jump targets are applied to the triggered main/off-hand strikes, not to this spell
            WorldObject* explTarget = GetExplTargetWorldObject();
            targets.remove_if([explTarget](WorldObject* target) { return target != explTarget; });
        }

        void HandleAfterHit()
        {
            Unit* caster = GetCaster();
            caster->RemoveAurasDueToSpell(SPELL_WARRIOR_MEAT_CLEAVER);
            if (Aura* ragingBlow = caster->GetAura(SPELL_WARRIOR_RAGING_BLOW_ENABLER))
                ragingBlow->ModStackAmount(-1);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warr_raging_blow_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_TARGET_ENEMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warr_raging_blow_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_TARGET_ENEMY);
            AfterHit += SpellHitFn(spell_warr_raging_blow_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_raging_blow_SpellScript();
    }
};

// 12950 - Meat Cleaver (one stack per Whirlwind, not one per target hit)
class spell_warr_meat_cleaver : public SpellScriptLoader
{
public:
    spell_warr_meat_cleaver() : SpellScriptLoader("spell_warr_meat_cleaver") { }

    class spell_warr_meat_cleaver_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_meat_cleaver_AuraScript);

        bool Load() OVERRIDE
        {
            _lastProcTime = 0;
            return true;
        }

        bool CheckProc(ProcEventInfo& /*eventInfo*/)
        {
            uint32 now = getMSTime();
            if (_lastProcTime && getMSTimeDiff(_lastProcTime, now) < 100)
                return false;

            _lastProcTime = now;
            return true;
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_warr_meat_cleaver_AuraScript::CheckProc);
        }

    private:
        uint32 _lastProcTime;
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_meat_cleaver_AuraScript();
    }
};

// 76838 - Mastery: Strikes of Opportunity
class spell_warr_strikes_of_opportunity : public SpellScriptLoader
{
public:
    spell_warr_strikes_of_opportunity() : SpellScriptLoader("spell_warr_strikes_of_opportunity") { }

    class spell_warr_strikes_of_opportunity_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_strikes_of_opportunity_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_WARRIOR_OPPORTUNITY_STRIKE) != NULL;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            Unit* target = eventInfo.GetActionTarget();
            if (!target || target == GetUnitOwner())
                return false;

            if (eventInfo.GetHitMask() & PROC_EX_INTERNAL_DOT)
                return false;

            // Only the primary target of a spell can trigger it (no chain / cleave targets)
            if (Spell const* triggeringSpell = eventInfo.TriggeredBySpell())
                if (triggeringSpell->m_targets.GetUnitTarget() != target)
                    return false;

            if (SpellInfo const* procSpell = eventInfo.GetSpellInfo())
            {
                if (procSpell->Id == SPELL_WARRIOR_OPPORTUNITY_STRIKE)
                    return false;

                if (procSpell->SpellFamilyName != SPELLFAMILY_WARRIOR || procSpell->IsAffectingArea())
                    return false;

                // Excluded abilities (non-damaging spells, Deep Wounds...) are listed in the effect class mask
                if (procSpell->SpellFamilyFlags & GetSpellInfo()->Effects[EFFECT_0].SpellClassMask)
                    return false;
            }

            return true;
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            Unit* warrior = GetUnitOwner();
            float chance = float(aurEff->GetAmount());
            if (chance <= 0.0f && warrior->GetTypeId() == TypeID::TYPEID_PLAYER)
                chance = warrior->GetFloatValue(PLAYER_FIELD_MASTERY) * GetSpellInfo()->Effects[EFFECT_0].BonusMultiplier;

            if (!roll_chance_f(chance))
                return;

            warrior->CastSpell(eventInfo.GetActionTarget(), SPELL_WARRIOR_OPPORTUNITY_STRIKE, true, NULL, aurEff);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_warr_strikes_of_opportunity_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_warr_strikes_of_opportunity_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_strikes_of_opportunity_AuraScript();
    }
};

// 29725 - Sudden Death (passive)
// Auto attacks and Opportunity Strike have a chance to grant Sudden Death (52437);
// Execute grants Sudden Execute.
class spell_warr_sudden_death_proc : public SpellScriptLoader
{
public:
    spell_warr_sudden_death_proc() : SpellScriptLoader("spell_warr_sudden_death_proc") { }

    class spell_warr_sudden_death_proc_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_sudden_death_proc_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_WARRIOR_SUDDEN_DEATH) && sSpellMgr->GetSpellInfo(SPELL_WARRIOR_SUDDEN_EXECUTE);
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (eventInfo.GetActionTarget() == GetTarget())
                return false;

            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            return !spellInfo || spellInfo->Id == SPELL_WARRIOR_OPPORTUNITY_STRIKE || spellInfo->Id == SPELL_WARRIOR_EXECUTE;
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            Unit* warrior = GetTarget();
            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            if (spellInfo && spellInfo->Id == SPELL_WARRIOR_EXECUTE)
            {
                warrior->CastSpell(warrior, SPELL_WARRIOR_SUDDEN_EXECUTE, true, NULL, aurEff);
                return;
            }

            // Colossus Smash cooldown reset is handled by the 52437 aura script.
            if (roll_chance_i(aurEff->GetAmount()))
                warrior->CastSpell(warrior, SPELL_WARRIOR_SUDDEN_DEATH, true, NULL, aurEff);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_warr_sudden_death_proc_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_warr_sudden_death_proc_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_sudden_death_proc_AuraScript();
    }
};

// 86346 - Colossus Smash
class spell_warr_colossus_smash : public SpellScriptLoader
{
public:
    spell_warr_colossus_smash() : SpellScriptLoader("spell_warr_colossus_smash") { }

    class spell_warr_colossus_smash_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_colossus_smash_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_WARRIOR_PHYSICAL_VULNERABILITY) && sSpellMgr->GetSpellInfo(SPELL_WARRIOR_WEAKENED_ARMOR);
        }

        void HandleOnHit()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!target)
                return;

            caster->CastSpell(target, SPELL_WARRIOR_PHYSICAL_VULNERABILITY, true);
            if (caster->HasAura(SPELL_WARRIOR_GLYPH_OF_COLOSSUS_SMASH))
                caster->CastSpell(target, SPELL_WARRIOR_WEAKENED_ARMOR, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_warr_colossus_smash_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_colossus_smash_SpellScript();
    }
};

// 7384 - Overpower, 100130 - Wild Strike
// Glyph of Die by the Sword - 58386
class spell_warr_glyph_of_die_by_the_sword : public SpellScriptLoader
{
public:
    spell_warr_glyph_of_die_by_the_sword() : SpellScriptLoader("spell_warr_glyph_of_die_by_the_sword") { }

    class spell_warr_glyph_of_die_by_the_sword_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_glyph_of_die_by_the_sword_SpellScript);

        void HandleCast()
        {
            Unit* caster = GetCaster();
            if (!caster->HasAura(SPELL_WARRIOR_GLYPH_OF_DIE_BY_THE_SWORD))
                return;

            if (Aura* aura = caster->GetAura(SPELL_WARRIOR_DIE_BY_THE_SWORD))
            {
                int32 duration = aura->GetDuration() + (GetSpellInfo()->Id == SPELL_WARRIOR_OVERPOWER ? 1000 : 500);
                if (aura->GetMaxDuration() < duration)
                    aura->SetMaxDuration(duration);
                aura->SetDuration(duration);
            }
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_warr_glyph_of_die_by_the_sword_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_glyph_of_die_by_the_sword_SpellScript();
    }
};

// 12292 - Bloodbath
class spell_warr_bloodbath : public SpellScriptLoader
{
public:
    spell_warr_bloodbath() : SpellScriptLoader("spell_warr_bloodbath") { }

    class spell_warr_bloodbath_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_bloodbath_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_WARRIOR_BLOODBATH_DOT) && sSpellMgr->GetSpellInfo(SPELL_WARRIOR_BLOODBATH_SLOW);
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (!eventInfo.GetActionTarget() || eventInfo.GetActor() == eventInfo.GetActionTarget())
                return false;

            DamageInfo* damageInfo = eventInfo.GetDamageInfo();
            return eventInfo.GetSpellInfo() && damageInfo && damageInfo->GetDamage();
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            Unit* warrior = GetTarget();
            Unit* target = eventInfo.GetActionTarget();

            // The bleed lasts 6 sec with a 1 sec tick
            int32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount()) / 6);
            if (amount <= 0)
                return;

            warrior->CastCustomSpell(SPELL_WARRIOR_BLOODBATH_DOT, SPELLVALUE_BASE_POINT0, amount, target, true);
            warrior->CastSpell(target, SPELL_WARRIOR_BLOODBATH_SLOW, true);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_warr_bloodbath_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_warr_bloodbath_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_bloodbath_AuraScript();
    }
};

// 115767 - Deep Wounds (Arms deals double damage)
class spell_warr_deep_wounds_dot : public SpellScriptLoader
{
public:
    spell_warr_deep_wounds_dot() : SpellScriptLoader("spell_warr_deep_wounds_dot") { }

    class spell_warr_deep_wounds_dot_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_deep_wounds_dot_AuraScript);

        void CalculateTick(AuraEffect const* /*aurEff*/, int32& amount)
        {
            if (Unit* caster = GetCaster())
                if (Player* player = caster->ToPlayer())
                    if (player->GetTalentSpecialization(player->GetActiveSpec()) == SPEC_WARRIOR_ARMS)
                        amount *= 2;
        }

        void Register() OVERRIDE
        {
            DoEffectCalcTickAmount += AuraEffectCalcTickAmountFn(spell_warr_deep_wounds_dot_AuraScript::CalculateTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_deep_wounds_dot_AuraScript();
    }
};

// 118779 - Victory Rush (heal)
// Glyph of Victory Rush - 58382
class spell_warr_victory_rush_heal : public SpellScriptLoader
{
public:
    spell_warr_victory_rush_heal() : SpellScriptLoader("spell_warr_victory_rush_heal") { }

    class spell_warr_victory_rush_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_victory_rush_heal_SpellScript);

        void HandleOnHit()
        {
            if (AuraEffect const* glyph = GetCaster()->GetAuraEffect(SPELL_WARRIOR_GLYPH_OF_VICTORY_RUSH, EFFECT_0))
                SetHitHeal(int32(GetHitHeal() * (1.0f + glyph->GetAmount() / 100.0f)));
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_warr_victory_rush_heal_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_victory_rush_heal_SpellScript();
    }
};

// 103840 - Impending Victory
class spell_warr_impending_victory : public SpellScriptLoader
{
public:
    spell_warr_impending_victory() : SpellScriptLoader("spell_warr_impending_victory") { }

    class spell_warr_impending_victory_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_impending_victory_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_WARRIOR_IMPENDING_VICTORY_HEAL) != NULL;
        }

        void HandleOnHit()
        {
            Unit* caster = GetCaster();
            SpellInfo const* healInfo = sSpellMgr->GetSpellInfo(SPELL_WARRIOR_IMPENDING_VICTORY_HEAL);
            int32 healPct = healInfo->Effects[EFFECT_0].CalcValue(caster);

            if (AuraEffect const* victorious = caster->GetAuraEffect(SPELL_WARRIOR_VICTORIOUS_T15, EFFECT_0))
            {
                AddPct(healPct, victorious->GetAmount());
                caster->RemoveAura(victorious->GetBase());
            }

            caster->CastCustomSpell(SPELL_WARRIOR_IMPENDING_VICTORY_HEAL, SPELLVALUE_BASE_POINT0, healPct, caster, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_warr_impending_victory_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_impending_victory_SpellScript();
    }
};

// 3411 - Intervene (party or raid members only)
class spell_warr_intervene : public SpellScriptLoader
{
public:
    spell_warr_intervene() : SpellScriptLoader("spell_warr_intervene") { }

    class spell_warr_intervene_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_intervene_SpellScript);

        SpellCastResult CheckCast()
        {
            Unit* target = GetExplTargetUnit();
            Player* caster = GetCaster()->ToPlayer();
            if (!target || !caster)
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;

            if (target == caster)
                return SpellCastResult::SPELL_FAILED_BAD_TARGETS;

            Player* player = target->GetCharmerOrOwnerPlayerOrPlayerItself();
            if (!player || !player->IsInSameRaidWith(caster))
                return SpellCastResult::SPELL_FAILED_BAD_TARGETS;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_warr_intervene_SpellScript::CheckCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_intervene_SpellScript();
    }
};

// 118000 - Dragon Roar (damage reduced per additional target)
class spell_warr_dragon_roar : public SpellScriptLoader
{
public:
    spell_warr_dragon_roar() : SpellScriptLoader("spell_warr_dragon_roar") { }

    class spell_warr_dragon_roar_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_dragon_roar_SpellScript);

        bool Load() OVERRIDE
        {
            _targetCount = 0;
            return true;
        }

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_WARRIOR_DRAGON_ROAR_KNOCK_BACK) != NULL;
        }

        void CountTargets(std::list<WorldObject*>& targets)
        {
            _targetCount = uint32(targets.size());
        }

        void HandleOnHit()
        {
            Unit* target = GetHitUnit();
            if (!target)
                return;

            float damage = float(GetHitDamage());
            switch (_targetCount)
            {
                case 0:
                case 1:
                    break;
                case 2:
                    damage *= 0.75f;
                    break;
                case 3:
                    damage *= 0.65f;
                    break;
                case 4:
                    damage *= 0.55f;
                    break;
                default:
                    if (_targetCount <= 10)
                        damage *= 0.50f;
                    else
                        damage = damage * 5 / _targetCount;
                    break;
            }

            SetHitDamage(int32(damage));
            GetCaster()->CastSpell(target, SPELL_WARRIOR_DRAGON_ROAR_KNOCK_BACK, true);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warr_dragon_roar_SpellScript::CountTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnHit += SpellHitFn(spell_warr_dragon_roar_SpellScript::HandleOnHit);
        }

    private:
        uint32 _targetCount;
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_dragon_roar_SpellScript();
    }
};

// 52174 - Heroic Leap (damage)
// Item - Warrior PvP Set 4P Bonus - 133277
class spell_warr_heroic_leap_damage : public SpellScriptLoader
{
public:
    spell_warr_heroic_leap_damage() : SpellScriptLoader("spell_warr_heroic_leap_damage") { }

    class spell_warr_heroic_leap_damage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_heroic_leap_damage_SpellScript);

        void HandleCast()
        {
            Unit* caster = GetCaster();
            if (caster->HasAura(SPELL_WARRIOR_PVP_SET_4P))
                caster->CastSpell(caster, SPELL_WARRIOR_HEROIC_LEAP_SPEED, true);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_warr_heroic_leap_damage_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warr_heroic_leap_damage_SpellScript();
    }
};

// 125565 - Demoralizing Shout (absorbs 20% of damage from attackers affected by the caster's 1160)
class spell_warr_demoralizing_shout : public SpellScriptLoader
{
public:
    spell_warr_demoralizing_shout() : SpellScriptLoader("spell_warr_demoralizing_shout") { }

    class spell_warr_demoralizing_shout_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_demoralizing_shout_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            amount = -1;
        }

        void HandleAbsorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
        {
            absorbAmount = 0;
            if (Unit* attacker = dmgInfo.GetAttacker())
                if (attacker->HasAura(SPELL_WARRIOR_DEMORALIZING_SHOUT, GetCasterGUID()))
                    absorbAmount = CalculatePct(dmgInfo.GetDamage(), 20);
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warr_demoralizing_shout_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            OnEffectAbsorb += AuraEffectAbsorbFn(spell_warr_demoralizing_shout_AuraScript::HandleAbsorb, EFFECT_0);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_demoralizing_shout_AuraScript();
    }
};

// 112048 - Shield Barrier (consumes up to 60 rage)
class spell_warr_shield_barrier : public SpellScriptLoader
{
public:
    spell_warr_shield_barrier() : SpellScriptLoader("spell_warr_shield_barrier") { }

    class spell_warr_shield_barrier_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_shield_barrier_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
        {
            canBeRecalculated = false;

            Unit* warrior = GetCaster();
            if (!warrior)
                return;

            int32 rageCost = 0;
            if (Spell const* currentSpell = warrior->FindCurrentSpellBySpellId(SPELL_WARRIOR_SHIELD_BARRIER))
                rageCost = currentSpell->GetPowerCost();

            int32 totalRage = warrior->GetPower(POWER_RAGE) + rageCost;
            int32 consumedRage = std::min(600, totalRage);
            warrior->SetPower(POWER_RAGE, totalRage - consumedRage);

            if (rageCost <= 0)
                rageCost = 200;

            float ap = warrior->GetTotalAttackPowerValue(WeaponAttackType::BASE_ATTACK);
            float strength = warrior->GetStat(STAT_STRENGTH);
            float stamina = warrior->GetStat(STAT_STAMINA);

            float m2 = float(GetSpellInfo()->Effects[EFFECT_1].CalcValue(warrior));
            float m3 = float(GetSpellInfo()->Effects[EFFECT_2].CalcValue(warrior));

            float rageMod = std::floor(consumedRage / 10.0f) / (rageCost / 10.0f);
            amount = int32(std::max((ap - 2 * strength) * m2 / 100.0f, stamina * m3 / 100.0f) / 3.0f * rageMod);
        }

        void HandleAbsorb(AuraEffect* /*aurEff*/, DamageInfo& damageInfo, uint32& absorbAmount)
        {
            Unit* owner = GetUnitOwner();
            if (AuraEffect const* bonus = owner->GetAuraEffect(SPELL_WARRIOR_T16_PROTECTION_2P_BONUS, EFFECT_0))
            {
                uint32 absorb = std::min(absorbAmount, damageInfo.GetDamage());
                int32 heal = int32(CalculatePct(absorb, bonus->GetAmount()));
                if (heal > 0)
                    owner->CastCustomSpell(SPELL_WARRIOR_SHIELD_GUARD, SPELLVALUE_BASE_POINT0, heal, owner, true);
            }
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warr_shield_barrier_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            OnEffectAbsorb += AuraEffectAbsorbFn(spell_warr_shield_barrier_AuraScript::HandleAbsorb, EFFECT_0);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_shield_barrier_AuraScript();
    }
};

// 144436 - Item - Warrior T16 DPS 2P Bonus
class spell_warr_t16_dps_2p_bonus : public SpellScriptLoader
{
public:
    spell_warr_t16_dps_2p_bonus() : SpellScriptLoader("spell_warr_t16_dps_2p_bonus") { }

    class spell_warr_t16_dps_2p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_t16_dps_2p_bonus_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            Unit* target = eventInfo.GetActionTarget();
            return target && target != GetTarget() && target->HasAura(SPELL_WARRIOR_COLOSSUS_SMASH, GetTarget()->GetGUID());
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            GetTarget()->CastSpell(GetTarget(), SPELL_WARRIOR_COLOSSAL_RAGE, true, NULL, aurEff);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_warr_t16_dps_2p_bonus_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_warr_t16_dps_2p_bonus_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_t16_dps_2p_bonus_AuraScript();
    }
};

// 1160 - Demoralizing Shout
// Item - Warrior T16 Protection 4P Bonus - 144502: Reckless Defense when the shout expires.
class spell_warr_t16_protection_4p_bonus : public SpellScriptLoader
{
public:
    spell_warr_t16_protection_4p_bonus() : SpellScriptLoader("spell_warr_t16_protection_4p_bonus") { }

    class spell_warr_t16_protection_4p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_t16_protection_4p_bonus_AuraScript);

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
                return;

            Unit* caster = GetCaster();
            Player* warrior = caster ? caster->ToPlayer() : NULL;
            if (!warrior || !warrior->HasAura(SPELL_WARRIOR_T16_PROTECTION_4P_BONUS))
                return;

            // Every affected enemy loses the shout at the same time; trigger only once.
            if (warrior->HasSpellCooldown(SPELL_WARRIOR_RECKLESS_DEFENSE))
                return;

            warrior->CastSpell(warrior, SPELL_WARRIOR_RECKLESS_DEFENSE, true);
            warrior->AddSpellCooldown(SPELL_WARRIOR_RECKLESS_DEFENSE, 0, time(NULL) + 1);
        }

        void Register() OVERRIDE
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_warr_t16_protection_4p_bonus_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_t16_protection_4p_bonus_AuraScript();
    }
};

// 144503 - Item - Warrior T16 Protection 2P Bonus
class spell_warr_t16_protection_2p_bonus : public SpellScriptLoader
{
public:
    spell_warr_t16_protection_2p_bonus() : SpellScriptLoader("spell_warr_t16_protection_2p_bonus") { }

    class spell_warr_t16_protection_2p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_t16_protection_2p_bonus_AuraScript);

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            DamageInfo* damageInfo = eventInfo.GetDamageInfo();
            if (!damageInfo || !damageInfo->GetBlock())
                return;

            int32 heal = int32(CalculatePct(damageInfo->GetBlock(), aurEff->GetAmount()));
            if (heal > 0)
                GetTarget()->CastCustomSpell(SPELL_WARRIOR_SHIELD_GUARD, SPELLVALUE_BASE_POINT0, heal, GetTarget(), true);
        }

        void Register() OVERRIDE
        {
            OnEffectProc += AuraEffectProcFn(spell_warr_t16_protection_2p_bonus_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_t16_protection_2p_bonus_AuraScript();
    }
};

// 58372 - Glyph of Rude Interruption
class spell_warr_glyph_of_rude_interruption : public SpellScriptLoader
{
public:
    spell_warr_glyph_of_rude_interruption() : SpellScriptLoader("spell_warr_glyph_of_rude_interruption") { }

    class spell_warr_glyph_of_rude_interruption_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_glyph_of_rude_interruption_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            return eventInfo.TriggeredBySpell() && eventInfo.TriggeredBySpell()->IsSuccessfulInterrupt();
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_warr_glyph_of_rude_interruption_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_glyph_of_rude_interruption_AuraScript();
    }
};

// 144500 - Reckless Defense (1 rage per 1% of maximum health taken as damage)
class spell_warr_reckless_defence : public SpellScriptLoader
{
public:
    spell_warr_reckless_defence() : SpellScriptLoader("spell_warr_reckless_defence") { }

    class spell_warr_reckless_defence_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_reckless_defence_AuraScript);

        bool Load() OVERRIDE
        {
            _accumulator = 0.0f;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            DamageInfo* damageInfo = eventInfo.GetDamageInfo();
            return damageInfo && damageInfo->GetDamage() && GetTarget()->GetMaxHealth();
        }

        void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            Unit* warrior = GetTarget();
            _accumulator += eventInfo.GetDamageInfo()->GetDamage() * 100.0f / warrior->GetMaxHealth();
            if (_accumulator < 1.0f)
                return;

            int32 rage = int32(_accumulator);
            _accumulator -= rage;
            warrior->CastCustomSpell(SPELL_WARRIOR_RECKLESS_DEFENSE_ENERGIZE, SPELLVALUE_BASE_POINT0, rage * 10, warrior, true);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_warr_reckless_defence_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_warr_reckless_defence_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }

    private:
        float _accumulator;
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_reckless_defence_AuraScript();
    }
};

// 12294 - Mortal Strike
// Glyph of Mortal Strike - 58368: the healing taken bonus (EFFECT_4) requires the glyph.
class spell_warr_mortal_strike : public SpellScriptLoader
{
public:
    spell_warr_mortal_strike() : SpellScriptLoader("spell_warr_mortal_strike") { }

    class spell_warr_mortal_strike_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_mortal_strike_AuraScript);

        void HandleInitEffects(uint32& effectMask)
        {
            Unit* caster = GetCaster();
            if (!caster || !caster->HasAura(SPELL_WARRIOR_GLYPH_OF_MORTAL_STRIKE))
                effectMask &= ~(1 << EFFECT_4);
        }

        void Register() OVERRIDE
        {
            OnInitEffects += AuraInitEffectsFn(spell_warr_mortal_strike_AuraScript::HandleInitEffects);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_mortal_strike_AuraScript();
    }
};

// 58385 - Glyph of Hamstring (only a Hamstring that cost rage triggers it)
class spell_warr_glyph_of_hamstring : public SpellScriptLoader
{
public:
    spell_warr_glyph_of_hamstring() : SpellScriptLoader("spell_warr_glyph_of_hamstring") { }

    class spell_warr_glyph_of_hamstring_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_glyph_of_hamstring_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            Spell const* procSpell = eventInfo.TriggeredBySpell();
            return procSpell && procSpell->GetPowerCost() > 0;
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_warr_glyph_of_hamstring_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_glyph_of_hamstring_AuraScript();
    }
};

// 115767 - Deep Wounds
// Glyph of Bloody Healing - 58369
class spell_warr_glyph_of_bloody_healing : public SpellScriptLoader
{
public:
    spell_warr_glyph_of_bloody_healing() : SpellScriptLoader("spell_warr_glyph_of_bloody_healing") { }

    class spell_warr_glyph_of_bloody_healing_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_glyph_of_bloody_healing_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_WARRIOR_BLOODY_HEALING) != NULL;
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* warrior = GetCaster())
                if (warrior->HasAura(SPELL_WARRIOR_GLYPH_OF_BLOODY_HEALING))
                    warrior->CastSpell(warrior, SPELL_WARRIOR_BLOODY_HEALING, true);
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_warr_glyph_of_bloody_healing_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_glyph_of_bloody_healing_AuraScript();
    }
};

// First Aid (bandages) - healing increased by 20% while under Bloody Healing (126665)
class spell_warr_glyph_of_bloody_healing_effect : public SpellScriptLoader
{
public:
    spell_warr_glyph_of_bloody_healing_effect() : SpellScriptLoader("spell_warr_glyph_of_bloody_healing_effect") { }

    class spell_warr_glyph_of_bloody_healing_effect_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_glyph_of_bloody_healing_effect_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* caster = GetCaster())
                if (caster->HasAura(SPELL_WARRIOR_BLOODY_HEALING))
                    AddPct(amount, 20);
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warr_glyph_of_bloody_healing_effect_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_glyph_of_bloody_healing_effect_AuraScript();
    }
};

// 126661 - Warrior Charge Drop Fire Periodic (Glyph of the Blazing Trail)
// Leaves fire visuals along the charge path behind the warrior.
class spell_warr_charge_drop_fire_periodic : public SpellScriptLoader
{
public:
    spell_warr_charge_drop_fire_periodic() : SpellScriptLoader("spell_warr_charge_drop_fire_periodic") { }

    class spell_warr_charge_drop_fire_periodic_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_charge_drop_fire_periodic_AuraScript);

        bool Load() OVERRIDE
        {
            _lastIndex = 0;
            _initialized = false;
            return true;
        }

        void HandlePeriodic(AuraEffect const* /*aurEff*/)
        {
            PreventDefaultAction();

            Unit* owner = GetUnitOwner();

            // The charge spline is not launched yet when the aura is applied
            if (!_initialized)
            {
                if (!owner->movespline->Initialized() || owner->movespline->Finalized())
                    return;

                _initialized = true;

                int32 splineDuration = owner->GetSplineDuration();
                if (splineDuration <= 0)
                    return;

                GetAura()->SetMaxDuration(splineDuration);
                GetAura()->SetDuration(splineDuration);

                Movement::MoveSpline::MySpline const& spline = owner->movespline->_Spline();
                Movement::MoveSplineInitArgs args;
                for (int32 i = spline.first(); i <= spline.last(); ++i)
                    args.path.push_back(spline.getPoint(i));

                if (args.path.size() < 2)
                    return;

                args.HasVelocity = true;
                args.velocity = 1.0f;

                // Sample the path once per yard
                Movement::MoveSpline sampler;
                sampler.Initialize(args);
                while (!sampler.Finalized())
                {
                    sampler.updateState(1000);
                    _points.push_back(sampler.ComputePosition());
                }
            }

            if (_points.empty() || GetAura()->GetMaxDuration() <= 0)
                return;

            float progress = float(GetAura()->GetMaxDuration() - GetAura()->GetDuration()) / float(GetAura()->GetMaxDuration());
            size_t newIndex = std::min(size_t(_points.size() * progress), _points.size());

            // The trail starts behind the warrior, not under its feet
            if (newIndex < 2)
                return;

            --newIndex;

            for (size_t i = _lastIndex; i < newIndex; ++i)
                owner->SendPlaySpellVisual(WARRIOR_VISUAL_BLAZING_TRAIL, _points[i].x, _points[i].y, _points[i].z, 1.0f);

            _lastIndex = newIndex;
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warr_charge_drop_fire_periodic_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }

    private:
        std::vector<G3D::Vector3> _points;
        size_t _lastIndex;
        bool _initialized;
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warr_charge_drop_fire_periodic_AuraScript();
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
    new spell_warr_victorious_state();
    new spell_warr_glyph_of_hindering_strikes();
    new spell_warr_heroic_strike_one_hand();
    new spell_warr_stampeding_shout();
    new spell_warr_shield_block();
    new spell_warr_storm_bolt();
    new spell_warr_storm_bolt_damage();
    new spell_warr_staggering_shout();
    new spell_warr_mocking_banner();
    new spell_warr_thunder_clap();
    new spell_warr_riposte();
    new spell_warr_revenge_shield_slam();
    new spell_warr_revenge();
    new spell_warr_shield_slam();
    new spell_warr_unshackled_fury();
    new spell_warr_glyph_of_burning_anger();
    new spell_warr_raging_blow();
    new spell_warr_meat_cleaver();
    new spell_warr_strikes_of_opportunity();
    new spell_warr_sudden_death_proc();
    new spell_warr_colossus_smash();
    new spell_warr_glyph_of_die_by_the_sword();
    new spell_warr_bloodbath();
    new spell_warr_deep_wounds_dot();
    new spell_warr_victory_rush_heal();
    new spell_warr_impending_victory();
    new spell_warr_intervene();
    new spell_warr_dragon_roar();
    new spell_warr_heroic_leap_damage();
    new spell_warr_demoralizing_shout();
    new spell_warr_shield_barrier();
    new spell_warr_t16_dps_2p_bonus();
    new spell_warr_t16_protection_4p_bonus();
    new spell_warr_t16_protection_2p_bonus();
    new spell_warr_glyph_of_rude_interruption();
    new spell_warr_reckless_defence();
    new spell_warr_mortal_strike();
    new spell_warr_glyph_of_hamstring();
    new spell_warr_glyph_of_bloody_healing();
    new spell_warr_glyph_of_bloody_healing_effect();
    new spell_warr_charge_drop_fire_periodic();
}




