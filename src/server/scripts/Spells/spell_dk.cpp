/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/*
 * Scripts for spells with SPELLFAMILY_DEATHKNIGHT and SPELLFAMILY_GENERIC spells used by deathknight players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_dk_".
 */

#include "Player.h"
#include "Pet.h"
#include "Battleground.h"
#include "DBCStores.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "DynamicObject.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"

enum DeathKnightSpells
{
    SPELL_DK_ANTI_MAGIC_ZONE_AREA = 51052,
    SPELL_DK_ANTI_MAGIC_ZONE_REDUCTION = 145629,

    SPELL_DK_BLOOD_CHARGE = 114851,
    SPELL_DK_BLOOD_GORGED_HEAL = 50454,
    SPELL_DK_BLOOD_PLAGUE = 55078,
    SPELL_DK_BLOOD_PRESENCE = 48263,
    SPELL_DK_BLOOD_SHIELD_MASTERY = 77513,
    SPELL_DK_BLOOD_SHIELD_ABSORB = 77535,
    SPELL_DK_BLOOD_TAP = 45529,
    SPELL_DK_BONE_WALL = 144948,
    SPELL_DK_CHAINS_OF_ICE = 45524,
    SPELL_DK_CHAINS_OF_ICE_ROOT = 96294,
    SPELL_DK_CHILBLAINS = 50041,
    SPELL_DK_CORPSE_EXPLOSION_TRIGGERED = 43999,
    SPELL_DK_CORPSE_EXPLOSION_VISUAL = 51270,
    SPELL_DK_DANCING_RUNE_WEAPON_PARRY = 81256,
    SPELL_DK_DANCING_RUNE_WEAPON_VISUAL = 53160,
    SPELL_DK_DARK_SIMULACRUM_OVERRIDE = 77616,
    SPELL_DK_DARK_SIMULACRUM_SPELL_POWER = 94984,
    SPELL_DK_DARK_TRANSFORMATION = 63560,
    SPELL_DK_DARK_TRANSFORMATION_ENABLER = 93426,
    SPELL_DK_DEATH_AND_DECAY_SLOW = 143375,
    SPELL_DK_DEATH_COIL = 47541,
    SPELL_DK_DEATH_COIL_DAMAGE = 47632,
    SPELL_DK_DEATH_COIL_HEAL = 47633,
    SPELL_DK_DEATH_GRIP = 49576,
    SPELL_DK_DEATH_GRIP_ONLY_JUMP = 49575,
    SPELL_DK_DEATH_SIPHON_HEAL = 116783,
    SPELL_DK_DEATH_STRIKE_HEAL = 45470,
    SPELL_DK_DEATH_STRIKE_ENABLER = 89832,
    SPELL_DK_DESECRATED_GROUND = 118009,
    SPELL_DK_DESECRATED_GROUND_IMMUNE = 115018,
    SPELL_DK_EBON_PLAGUEBRINGER = 51160,
    SPELL_DK_FESTERING_BLOOD = 146650,
    SPELL_DK_FROST_FEVER = 55095,
    SPELL_DK_FROST_PRESENCE = 48266,
    SPELL_DK_GEIST_TRANSFORM = 121916,
    SPELL_DK_GHOUL_EXPLODE = 47496,
    SPELL_DK_GHOUL_AS_GUARDIAN = 46585,
    SPELL_DK_GHOUL_AS_PET = 52150,
    SPELL_DK_GLYPH_OF_DANCING_RUNE_WEAPON = 63330,
    SPELL_DK_GLYPH_OF_DEATH_AND_DECAY = 58629,
    SPELL_DK_GLYPH_OF_DEATHS_EMBRACE = 58677,
    SPELL_DK_GLYPH_OF_HORN_OF_WINTER = 58680,
    SPELL_DK_GLYPH_OF_HORN_OF_WINTER_EFFECT = 121920,
    SPELL_DK_GLYPH_OF_ICEBOUND_FORTITUDE = 58625,
    SPELL_DK_GLYPH_OF_PILLAR_OF_FROST = 58635,
    SPELL_DK_GLYPH_OF_SHIFTING_PRESENCES = 58647,
    SPELL_DK_GLYPH_OF_STRANGULATE = 58618,
    SPELL_DK_GLYPH_OF_SWIFT_DEATH = 146645,
    SPELL_DK_GLYPH_OF_THE_GEIST = 58640,
    SPELL_DK_GLYPH_OF_THE_LOUD_HORN = 146646,
    SPELL_DK_GLYPH_OF_THE_LOUD_HORN_ENERGIZE = 147078,
    SPELL_DK_GLYPH_OF_UNHOLY_FRENZY = 58616,
    SPELL_DK_GOREFIENDS_GRASP_GRIP_VISUAL = 114869,
    SPELL_DK_IMPROVED_DEATH_STRIKE = 62905,
    SPELL_DK_ITEM_T8_MELEE_4P_BONUS = 64736,
    SPELL_DK_MASTER_OF_GHOULS = 52143,
    SPELL_DK_MASTERY_DREADBLADE = 77515,
    SPELL_DK_PERDITION = 123981,
    SPELL_DK_PESTILENCE_BLOOD_BOIL = 116617,
    SPELL_DK_PESTILENCE_VISUAL = 91939,
    SPELL_DK_PHYSICAL_VULNERABILITY = 81326,
    SPELL_DK_PILLAR_OF_FROST = 51271,
    SPELL_DK_PILLAR_OF_FROST_GLYPHED = 90259,
    SPELL_DK_PURGATORY_INSTAKILL = 123982,
    SPELL_DK_RELENTLESS_GRIP = 131547,
    SPELL_DK_REMORSELESS_WINTER = 108200,
    SPELL_DK_REMORSELESS_WINTER_STUN = 115001,
    SPELL_DK_ROILING_BLOOD = 108170,
    SPELL_DK_RUNE_WEAPON_SCALING = 51906,
    SPELL_DK_RUNIC_CORRUPTION_REGEN = 51460,
    SPELL_DK_RUNIC_EMPOWERMENT = 81229,
    SPELL_DK_RUNIC_POWER_ENERGIZE = 49088,
    SPELL_DK_RUNIC_POWER_T15_BLOOD = 138214,
    SPELL_DK_RUNE_TAP = 48982,
    SPELL_DK_SCARLET_FEVER = 81132,
    SPELL_DK_SCENT_OF_BLOOD_PASSIVE = 49509,
    SPELL_DK_SCENT_OF_BLOOD = 50421,
    SPELL_DK_SCOURGE_STRIKE_TRIGGERED = 70890,
    SPELL_DK_SHADOW_INFUSION = 91342,
    SPELL_DK_SHROUD_OF_PURGATORY = 116888,
    SPELL_DK_SKELETON_TRANSFORM = 147157,
    SPELL_DK_SOUL_REAPER_DAMAGE = 114867,
    SPELL_DK_SOUL_REAPER_HASTE = 114868,
    SPELL_DK_STRANGULATE = 47476,
    SPELL_DK_T15_BLOOD_4P_BONUS = 138197,
    SPELL_DK_T15_DPS_4P_BONUS = 138347,
    SPELL_DK_T16_BLOOD_2P_BONUS = 144934,
    SPELL_DK_T16_DPS_4P_BONUS_HIDDEN = 144909,
    SPELL_DK_UNHOLY_PRESENCE = 48265,
    SPELL_DK_WEAKENED_BLOWS = 115798,
    SPELL_DK_WILD_MUSHROOM_PLAGUE_TRIGGER = 113518,
};

enum DeathKnightCreatures
{
    NPC_DK_RISEN_GHOUL = 26125,
    NPC_DK_RUNE_WEAPON = 27893,
};

enum DeathKnightSpellValues
{
    SCENT_OF_BLOOD_DEATH_STRIKE_BONUS_PCT = 20,
    BLOOD_TAP_CHARGE_COST = 5,
};

// Client values missing from SpellCustomErrors
enum DeathKnightCustomErrors
{
    DK_CUSTOM_ERROR_NO_FULLY_DEPLETED_RUNES = 154,
    DK_CUSTOM_ERROR_BOTH_FROST_FEVER_BLOOD_PLAGUE = 159,
};

// Runes regenerate one per pair; a rune is fully depleted while its pair partner is the one regenerating.
static bool IsRuneFullyDepleted(Player const* player, uint8 index)
{
    uint8 const first = index & ~1;
    uint8 const second = first + 1;
    uint32 const firstCooldown = player->GetRuneCooldown(first);
    uint32 const secondCooldown = player->GetRuneCooldown(second);
    if (!firstCooldown || !secondCooldown)
        return false;

    uint8 const regenerating = firstCooldown > secondCooldown ? second : first;
    return index != regenerating;
}

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

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_dk_gorefiends_grasp_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
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
                            if (_player->GetRuneCooldown(i) > 0)
                                runes.insert(i);
                        if (!runes.empty())
                        {
                            std::set<uint8>::iterator itr = runes.begin();
                            std::advance(itr, std::rand() % runes.size());
                            _player->SetRuneCooldown((*itr), 0);
                            _player->ResyncRunes(MAX_RUNES);
                        }
                    }
                }
            }
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_dk_runic_empowerment_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
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

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_dk_runic_corruption_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
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

                if (_player->HasAura(SPELL_DK_GLYPH_OF_THE_GEIST))
                {
                    std::list<Creature*> ghouls;
                    _player->GetAllMinionsByEntry(ghouls, NPC_DK_RISEN_GHOUL);
                    for (Creature* ghoul : ghouls)
                        ghoul->CastSpell(ghoul, SPELL_DK_GEIST_TRANSFORM, true);
                }
            }
        }

        void Register() OVERRIDE
        {
            OnEffectHit += SpellEffectFn(spell_dk_raise_dead_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
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

        bool Load() OVERRIDE
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

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_anti_magic_shell_raid_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_shell_raid_AuraScript::Absorb, EFFECT_0);
        }
    private:
        uint32 absorbPct = 0;
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_anti_magic_shell_raid_AuraScript();
    }
};

// 48707 - Anti-Magic Shell (on self)
// 110570 - Anti-Magic Shell (Symbiosis)
class spell_dk_anti_magic_shell_self : public SpellScriptLoader
{
public:
    spell_dk_anti_magic_shell_self() : SpellScriptLoader("spell_dk_anti_magic_shell_self") { }

    class spell_dk_anti_magic_shell_self_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_anti_magic_shell_self_AuraScript);

        bool Load() OVERRIDE
        {
            absorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
            hpPct = GetSpellInfo()->Effects[EFFECT_1].CalcValue(GetCaster());
            return true;
        }

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
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

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_anti_magic_shell_self_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_shell_self_AuraScript::Absorb, EFFECT_0);
            if (m_scriptSpellId == 48707)
                AfterEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_shell_self_AuraScript::Trigger, EFFECT_0);
        }
    private:
        uint32 absorbPct = 0;
        uint32 hpPct = 0;
    };

    AuraScript* GetAuraScript() const OVERRIDE
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

        bool Load() OVERRIDE
        {
            SpellInfo const* reductionSpell = sSpellMgr->GetSpellInfo(SPELL_DK_ANTI_MAGIC_ZONE_REDUCTION);
            if (!reductionSpell)
                return false;

            int32 const reductionPct = reductionSpell->Effects[EFFECT_0].CalcValue(GetCaster());
            absorbPct = uint32(reductionPct < 0 ? -reductionPct : reductionPct);
            return true;
        }

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DK_ANTI_MAGIC_ZONE_AREA))
                return false;

            if (!sSpellMgr->GetSpellInfo(SPELL_DK_ANTI_MAGIC_ZONE_REDUCTION))
                return false;

            return true;
        }

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            amount = -1;
        }

        void Absorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
        {
            absorbAmount = CalculatePct(dmgInfo.GetDamage(), absorbPct);
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_anti_magic_zone_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_zone_AuraScript::Absorb, EFFECT_0);
        }
    private:
        uint32 absorbPct = 0;
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_anti_magic_zone_AuraScript();
    }
};

class spell_dk_anti_magic_zone_dynamic : public DynamicObjectScript
{
public:
    spell_dk_anti_magic_zone_dynamic() : DynamicObjectScript("spell_dk_anti_magic_zone_dynamic") { }

    void OnUpdate(DynamicObject* dynObj, uint32 /*diff*/) OVERRIDE
    {
        if (dynObj->GetSpellId() != SPELL_DK_ANTI_MAGIC_ZONE_AREA)
            return;

        Unit* caster = dynObj->GetCaster();
        if (!caster)
            return;

        float const radius = dynObj->GetRadius();
        std::list<Unit*> targets;
        Skyfire::AnyFriendlyUnitInObjectRangeCheck check(dynObj, caster, radius);
        Skyfire::UnitListSearcher<Skyfire::AnyFriendlyUnitInObjectRangeCheck> searcher(dynObj, targets, check);
        dynObj->VisitNearbyObject(radius, searcher);

        for (std::list<Unit*>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
        {
            Unit* target = *itr;
            if (!target->HasAura(SPELL_DK_ANTI_MAGIC_ZONE_REDUCTION, caster->GetGUID()))
                caster->AddAura(SPELL_DK_ANTI_MAGIC_ZONE_REDUCTION, target);
        }
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

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DK_BLOOD_GORGED_HEAL))
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

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            int32 heal = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), 150));
            GetTarget()->CastCustomSpell(SPELL_DK_BLOOD_GORGED_HEAL, SPELLVALUE_BASE_POINT0, heal, _procTarget, true, NULL, aurEff);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_dk_blood_gorged_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_dk_blood_gorged_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }

    private:
        Unit* _procTarget = NULL;
    };

    AuraScript* GetAuraScript() const OVERRIDE
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

        bool Validate(SpellInfo const* /*spell*/) OVERRIDE
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

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_dk_death_coil_SpellScript::CheckCast);
            OnEffectHitTarget += SpellEffectFn(spell_dk_death_coil_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
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

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_dk_death_gate_SpellScript::CheckClass);
            OnEffectHitTarget += SpellEffectFn(spell_dk_death_gate_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
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

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_dk_death_grip_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
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

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_dk_death_pact_SpellScript::CheckCast);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dk_death_pact_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ALLY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
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

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
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

                int32 minimumHeal = int32(GetCaster()->CountPctFromMaxHealth(GetEffectValue()));

                if (Aura* scentOfBlood = GetCaster()->GetAura(SPELL_DK_SCENT_OF_BLOOD))
                {
                    uint8 const scentOfBloodStacks = scentOfBlood->GetStackAmount();
                    AddPct(heal, scentOfBloodStacks * SCENT_OF_BLOOD_DEATH_STRIKE_BONUS_PCT);
                    AddPct(minimumHeal, scentOfBloodStacks * SCENT_OF_BLOOD_DEATH_STRIKE_BONUS_PCT);
                    GetCaster()->RemoveAura(SPELL_DK_SCENT_OF_BLOOD);
                }

                heal = std::max(heal, minimumHeal);
                GetCaster()->CastCustomSpell(SPELL_DK_DEATH_STRIKE_HEAL, SPELLVALUE_BASE_POINT0, heal, GetCaster(), true);
            }

            if (!GetCaster()->HasAura(SPELL_DK_BLOOD_PRESENCE))
                return;

            if (AuraEffect const* aurEff = GetCaster()->GetAuraEffect(SPELL_DK_BLOOD_SHIELD_MASTERY, EFFECT_0))
                GetCaster()->CastCustomSpell(SPELL_DK_BLOOD_SHIELD_ABSORB, SPELLVALUE_BASE_POINT0, GetCaster()->CountPctFromMaxHealth(aurEff->GetAmount()), GetCaster());
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_dk_death_strike_SpellScript::HandleDummy, EFFECT_2, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
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

        bool Load() OVERRIDE
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

        void Register() OVERRIDE
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

    AuraScript* GetAuraScript() const OVERRIDE
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

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
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

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_dk_ghoul_explode_SpellScript::Suicide, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
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

        bool Load() OVERRIDE
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

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_icebound_fortitude_AuraScript::CalculateAmount, EFFECT_2, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
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
                amount = int32(caster->GetTotalAttackPowerValue(WeaponAttackType::BASE_ATTACK) * 2.25f);
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_necrotic_strike_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_HEAL_ABSORB);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
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

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dk_rune_tap_party_SpellScript::CheckTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_PARTY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dk_rune_tap_party_SpellScript();
    }
};

// 49509 - Scent of Blood
class spell_dk_scent_of_blood : public SpellScriptLoader
{
public:
    spell_dk_scent_of_blood() : SpellScriptLoader("spell_dk_scent_of_blood") { }

    class spell_dk_scent_of_blood_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_scent_of_blood_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DK_SCENT_OF_BLOOD_PASSIVE) ||
                !sSpellMgr->GetSpellInfo(SPELL_DK_SCENT_OF_BLOOD))
                return false;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            uint32 const procFlags = eventInfo.GetTypeMask();
            if (!(procFlags & PROC_FLAG_DONE_MELEE_AUTO_ATTACK))
                return false;

            if (!(procFlags & PROC_FLAG_DONE_MAINHAND_ATTACK) || (procFlags & PROC_FLAG_DONE_OFFHAND_ATTACK))
                return false;

            return (eventInfo.GetHitMask() & (PROC_EX_NORMAL_HIT | PROC_EX_CRITICAL_HIT)) != 0;
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_dk_scent_of_blood_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
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

        bool Load() OVERRIDE
        {
            multiplier = 1.0f;
            return true;
        }

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
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

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_dk_scourge_strike_SpellScript::HandleDummy, EFFECT_2, SPELL_EFFECT_DUMMY);
            AfterHit += SpellHitFn(spell_dk_scourge_strike_SpellScript::HandleAfterHit);
        }
    private:
        float multiplier = 0.0f;
    };

    SpellScript* GetSpellScript() const OVERRIDE
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

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_vampiric_blood_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_INCREASE_HEALTH);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_vampiric_blood_AuraScript();
    }
};

// 47541 - Death Coil, 49143 - Frost Strike, 56815 - Rune Strike
class spell_dk_blood_charges : public SpellScriptLoader
{
public:
    spell_dk_blood_charges() : SpellScriptLoader("spell_dk_blood_charges") { }

    class spell_dk_blood_charges_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_blood_charges_SpellScript);

        void HandleHit()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!target || !caster->HasSpell(SPELL_DK_BLOOD_TAP))
                return;

            if (caster->HasAura(SPELL_DK_GLYPH_OF_DEATHS_EMBRACE) && !caster->IsValidAttackTarget(target))
                return;

            caster->CastSpell(caster, SPELL_DK_BLOOD_CHARGE, true);
            caster->CastSpell(caster, SPELL_DK_BLOOD_CHARGE, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_dk_blood_charges_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dk_blood_charges_SpellScript();
    }
};

// 45529 - Blood Tap
class spell_dk_blood_tap : public SpellScriptLoader
{
public:
    spell_dk_blood_tap() : SpellScriptLoader("spell_dk_blood_tap") { }

    class spell_dk_blood_tap_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_blood_tap_SpellScript);

        SpellCastResult CheckCast()
        {
            Player* player = GetCaster()->ToPlayer();
            if (!player)
                return SpellCastResult::SPELL_CAST_OK;

            Aura* bloodCharges = player->GetAura(SPELL_DK_BLOOD_CHARGE);
            if (!bloodCharges || bloodCharges->GetStackAmount() < BLOOD_TAP_CHARGE_COST)
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;

            for (uint8 i = 0; i < MAX_RUNES; ++i)
                if (player->GetCurrentRune(i) != RuneType::RUNE_DEATH && player->GetRuneCooldown(i))
                    return SpellCastResult::SPELL_CAST_OK;

            return SpellCastResult::SPELL_FAILED_DONT_REPORT;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Player* player = GetCaster()->ToPlayer();
            if (!player)
                return;

            Aura* bloodCharges = player->GetAura(SPELL_DK_BLOOD_CHARGE);
            if (!bloodCharges)
                return;

            std::vector<uint8> runes;
            for (uint8 i = 0; i < MAX_RUNES; ++i)
                if (player->GetRuneCooldown(i) && player->GetCurrentRune(i) != RuneType::RUNE_DEATH)
                    runes.push_back(i);

            if (runes.empty())
                return;

            if (bloodCharges->GetStackAmount() <= BLOOD_TAP_CHARGE_COST)
                player->RemoveAura(SPELL_DK_BLOOD_CHARGE);
            else
                bloodCharges->SetStackAmount(bloodCharges->GetStackAmount() - BLOOD_TAP_CHARGE_COST);

            uint8 const rune = runes[urand(0, uint32(runes.size()) - 1)];
            player->ConvertRune(rune, RuneType::RUNE_DEATH);
            player->SetRuneCooldown(rune, 0);
            player->ResyncRunes(MAX_RUNES);
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_dk_blood_tap_SpellScript::CheckCast);
            OnEffectHitTarget += SpellEffectFn(spell_dk_blood_tap_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dk_blood_tap_SpellScript();
    }
};

// 50034 - Blood Rites, 56835 - Reaping
class spell_dk_rune_converter : public SpellScriptLoader
{
public:
    spell_dk_rune_converter() : SpellScriptLoader("spell_dk_rune_converter") { }

    class spell_dk_rune_converter_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_rune_converter_AuraScript);

        bool Load() OVERRIDE
        {
            Player* player = GetOwner()->ToPlayer();
            return player && player->getClass() == CLASS_DEATH_KNIGHT;
        }

        // Runes spent by the proccing spell are the ones whose cooldown was just reset to the full base cooldown.
        uint8 GetSpentRuneMask(ProcEventInfo& eventInfo) const
        {
            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            if (!spellInfo)
                return 0;

            SpellRuneCostEntry const* runeCost = sSpellRuneCostStore.LookupEntry(spellInfo->RuneCostID);
            if (!runeCost)
                return 0;

            Player* player = GetUnitOwner()->ToPlayer();
            if (!player)
                return 0;

            uint8 mask = 0;
            for (uint8 i = 0; i < MAX_RUNES; ++i)
            {
                RuneType const rune = player->GetCurrentRune(i);
                if (rune == RuneType::RUNE_DEATH || !runeCost->RuneCost[uint8(rune)])
                    continue;

                if (!player->GetRuneCooldown(i) || player->GetRuneCooldown(i) != player->GetRuneBaseCooldown(i))
                    continue;

                mask |= 1 << i;
            }
            return mask;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            return GetSpentRuneMask(eventInfo) != 0;
        }

        void HandleProc(ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            Player* player = GetUnitOwner()->ToPlayer();
            AuraEffect* aurEff = GetEffect(EFFECT_0);
            if (!player || !aurEff)
                return;

            uint8 const mask = GetSpentRuneMask(eventInfo);
            if (!mask)
                return;

            aurEff->ResetPeriodic(true);
            for (uint8 i = 0; i < MAX_RUNES; ++i)
                if (mask & (1 << i))
                    player->AddRuneByAuraEffect(i, RuneType::RUNE_DEATH, aurEff);
        }

        void HandleTick(AuraEffect const* aurEff)
        {
            if (Player* player = GetUnitOwner()->ToPlayer())
                player->RemoveRunesByAuraEffect(aurEff);
        }

        void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (Player* player = GetUnitOwner()->ToPlayer())
                player->RemoveRunesByAuraEffect(aurEff);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_dk_rune_converter_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_dk_rune_converter_AuraScript::HandleProc);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_dk_rune_converter_AuraScript::HandleTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            OnEffectRemove += AuraEffectRemoveFn(spell_dk_rune_converter_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_rune_converter_AuraScript();
    }
};

// 49222 - Bone Shield
class spell_dk_bone_shield_check : public SpellScriptLoader
{
public:
    spell_dk_bone_shield_check() : SpellScriptLoader("spell_dk_bone_shield_check") { }

    class spell_dk_bone_shield_check_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_bone_shield_check_SpellScript);

        SpellCastResult CheckCast()
        {
            if (Aura* shield = GetCaster()->GetAura(GetSpellInfo()->Id))
                if (shield->GetCharges() > 6)
                    return SpellCastResult::SPELL_FAILED_AURA_BOUNCED;
            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_dk_bone_shield_check_SpellScript::CheckCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dk_bone_shield_check_SpellScript();
    }
};

// 49222 - Bone Shield
class spell_dk_bone_shield : public SpellScriptLoader
{
public:
    spell_dk_bone_shield() : SpellScriptLoader("spell_dk_bone_shield") { }

    class spell_dk_bone_shield_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_bone_shield_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (eventInfo.GetTypeMask() & PROC_FLAG_TAKEN_SPELL_MAGIC_DMG_CLASS_POS)
                return false;
            return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0;
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_dk_bone_shield_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_bone_shield_AuraScript();
    }
};

// 49028 - Dancing Rune Weapon
class spell_dk_dancing_rune_weapon : public SpellScriptLoader
{
public:
    spell_dk_dancing_rune_weapon() : SpellScriptLoader("spell_dk_dancing_rune_weapon") { }

    class spell_dk_dancing_rune_weapon_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_dancing_rune_weapon_SpellScript);

        void HandleAfterCast()
        {
            Unit* caster = GetCaster();
            caster->CastSpell(caster, SPELL_DK_DANCING_RUNE_WEAPON_PARRY, true);

            std::vector<Creature*> runeWeapons;
            for (Unit::ControlList::const_iterator itr = caster->m_Controlled.begin(); itr != caster->m_Controlled.end(); ++itr)
                if (Creature* runeWeapon = (*itr)->ToCreature())
                    if (runeWeapon->GetEntry() == NPC_DK_RUNE_WEAPON)
                        runeWeapons.push_back(runeWeapon);

            Unit* target = GetExplTargetUnit();
            for (Creature* runeWeapon : runeWeapons)
            {
                runeWeapon->CastSpell(runeWeapon, SPELL_DK_DANCING_RUNE_WEAPON_VISUAL, true);
                runeWeapon->CastSpell(runeWeapon, SPELL_DK_RUNE_WEAPON_SCALING, true);
                if (target && runeWeapon->IsAIEnabled)
                    runeWeapon->AI()->AttackStart(target);
            }
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_dk_dancing_rune_weapon_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dk_dancing_rune_weapon_SpellScript();
    }
};

// 51906 - Death Knight Rune Weapon Scaling
class spell_dk_rune_weapon_scaling : public SpellScriptLoader
{
public:
    spell_dk_rune_weapon_scaling() : SpellScriptLoader("spell_dk_rune_weapon_scaling") { }

    class spell_dk_rune_weapon_scaling_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_rune_weapon_scaling_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            Unit* owner = caster->GetOwner();
            if (!owner)
                return;

            float multiplier = owner->GetTotalAuraMultiplier(SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
            if (AuraEffect const* glyph = owner->GetAuraEffect(SPELL_DK_GLYPH_OF_DANCING_RUNE_WEAPON, EFFECT_1))
                AddPct(multiplier, glyph->GetAmount());
            amount = int32((multiplier - 1.0f) * 100.0f);
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_rune_weapon_scaling_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_rune_weapon_scaling_AuraScript();
    }
};

// 51271 - Pillar of Frost
class spell_dk_pillar_of_frost : public SpellScriptLoader
{
public:
    spell_dk_pillar_of_frost() : SpellScriptLoader("spell_dk_pillar_of_frost") { }

    class spell_dk_pillar_of_frost_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_pillar_of_frost_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            PreventDefaultAction();

            Unit* target = GetTarget();
            target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
            target->ApplySpellImmune(GetId(), IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            target->ApplySpellImmune(GetId(), IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK_DEST, true);
            if (target->HasAura(SPELL_DK_GLYPH_OF_PILLAR_OF_FROST))
                target->CastSpell(target, SPELL_DK_PILLAR_OF_FROST_GLYPHED, true);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            PreventDefaultAction();

            Unit* target = GetTarget();
            target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, MECHANIC_GRIP, false);
            target->ApplySpellImmune(GetId(), IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, false);
            target->ApplySpellImmune(GetId(), IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK_DEST, false);
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_dk_pillar_of_frost_AuraScript::HandleApply, EFFECT_1, SPELL_AURA_MECHANIC_IMMUNITY_MASK, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_dk_pillar_of_frost_AuraScript::HandleRemove, EFFECT_1, SPELL_AURA_MECHANIC_IMMUNITY_MASK, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_pillar_of_frost_AuraScript();
    }
};

// 49184 - Howling Blast
class spell_dk_howling_blast : public SpellScriptLoader
{
public:
    spell_dk_howling_blast() : SpellScriptLoader("spell_dk_howling_blast") { }

    class spell_dk_howling_blast_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_howling_blast_SpellScript);

        void HandleHit()
        {
            Unit* target = GetHitUnit();
            if (!target)
                return;

            if (target != GetExplTargetUnit())
                SetHitDamage(int32(CalculatePct(GetHitDamage(), GetSpellInfo()->Effects[EFFECT_2].BasePoints)));

            GetCaster()->CastSpell(target, SPELL_DK_FROST_FEVER, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_dk_howling_blast_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dk_howling_blast_SpellScript();
    }
};

// 49530 - Sudden Doom
class spell_dk_sudden_doom : public SpellScriptLoader
{
public:
    spell_dk_sudden_doom() : SpellScriptLoader("spell_dk_sudden_doom") { }

    class spell_dk_sudden_doom_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_sudden_doom_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            return !(eventInfo.GetTypeMask() & PROC_FLAG_DONE_OFFHAND_ATTACK);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_dk_sudden_doom_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_sudden_doom_AuraScript();
    }
};

// 49572 - Shadow Infusion
class spell_dk_shadow_infusion_proc : public SpellScriptLoader
{
public:
    spell_dk_shadow_infusion_proc() : SpellScriptLoader("spell_dk_shadow_infusion_proc") { }

    class spell_dk_shadow_infusion_proc_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_shadow_infusion_proc_AuraScript);

        bool Load() OVERRIDE
        {
            return GetOwner()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        bool CheckProc(ProcEventInfo& /*eventInfo*/)
        {
            Player* player = GetOwner()->ToPlayer();
            if (Pet* ghoul = player->GetPet())
                if (ghoul->HasAura(SPELL_DK_DARK_TRANSFORMATION))
                    return false;

            if (Battleground* bg = player->GetBattleground())
                if (bg->GetStatus() == STATUS_WAIT_JOIN)
                    return false;

            return true;
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_dk_shadow_infusion_proc_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_shadow_infusion_proc_AuraScript();
    }
};

// 91342 - Shadow Infusion
class spell_dk_shadow_infusion : public SpellScriptLoader
{
public:
    spell_dk_shadow_infusion() : SpellScriptLoader("spell_dk_shadow_infusion") { }

    class spell_dk_shadow_infusion_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_shadow_infusion_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetStackAmount() == GetSpellInfo()->StackAmount)
                GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_DK_DARK_TRANSFORMATION_ENABLER, true);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetUnitOwner()->RemoveAurasDueToSpell(SPELL_DK_DARK_TRANSFORMATION_ENABLER);
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_dk_shadow_infusion_AuraScript::HandleApply, EFFECT_2, SPELL_AURA_408, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            OnEffectRemove += AuraEffectRemoveFn(spell_dk_shadow_infusion_AuraScript::HandleRemove, EFFECT_2, SPELL_AURA_408, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_shadow_infusion_AuraScript();
    }
};

// 63560 - Dark Transformation
class spell_dk_dark_transformation : public SpellScriptLoader
{
public:
    spell_dk_dark_transformation() : SpellScriptLoader("spell_dk_dark_transformation") { }

    class spell_dk_dark_transformation_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_dark_transformation_SpellScript);

        void HandleHit()
        {
            GetCaster()->RemoveAurasDueToSpell(SPELL_DK_SHADOW_INFUSION);
            if (Unit* target = GetHitUnit())
                target->RemoveAurasDueToSpell(SPELL_DK_SHADOW_INFUSION);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_dk_dark_transformation_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dk_dark_transformation_SpellScript();
    }
};

// 47468 - Claw, 47481 - Gnaw, 47482 - Leap, 47484 - Huddle
class spell_dk_ghoul_ability : public SpellScriptLoader
{
public:
    spell_dk_ghoul_ability() : SpellScriptLoader("spell_dk_ghoul_ability") { }

    class spell_dk_ghoul_ability_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_ghoul_ability_SpellScript);

        void HandleHit()
        {
            Unit* target = GetHitUnit();
            if (!target)
                return;

            uint32 normalSpell = 0;
            uint32 transformedSpell = 0;
            switch (GetSpellInfo()->Id)
            {
                case 47468: normalSpell = 91776; transformedSpell = 91778; break; // Claw / Sweeping Claws
                case 47481: normalSpell = 91800; transformedSpell = 91797; break; // Gnaw / Monstrous Blow
                case 47482: normalSpell = 91809; transformedSpell = 91802; break; // Leap / Shambling Rush
                case 47484: normalSpell = 91838; transformedSpell = 91837; break; // Huddle / Putrid Bulwark
                default:
                    return;
            }

            Unit* ghoul = GetCaster();
            ghoul->CastSpell(target, ghoul->HasAura(SPELL_DK_DARK_TRANSFORMATION) ? transformedSpell : normalSpell, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_dk_ghoul_ability_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dk_ghoul_ability_SpellScript();
    }
};

// 85948 - Festering Strike
class spell_dk_festering_strike : public SpellScriptLoader
{
public:
    spell_dk_festering_strike() : SpellScriptLoader("spell_dk_festering_strike") { }

    class spell_dk_festering_strike_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_festering_strike_SpellScript);

        static void ExtendAura(Aura* aura, int32 extension)
        {
            if (!aura)
                return;

            int32 const duration = aura->GetDuration() + extension;
            if (duration > aura->GetMaxDuration())
                aura->SetMaxDuration(duration);
            aura->SetDuration(duration);
        }

        void HandleHit()
        {
            Unit* target = GetHitUnit();
            if (!target)
                return;

            uint64 const casterGuid = GetCaster()->GetGUID();
            int32 const extension = GetSpellInfo()->Effects[EFFECT_2].CalcValue(GetCaster()) * IN_MILLISECONDS;

            if (Aura* bloodPlague = target->GetAura(SPELL_DK_BLOOD_PLAGUE, casterGuid))
            {
                ExtendAura(bloodPlague, extension);
                if (Aura* vulnerability = target->GetAura(SPELL_DK_PHYSICAL_VULNERABILITY, casterGuid))
                {
                    if (bloodPlague->GetDuration() > vulnerability->GetMaxDuration())
                        vulnerability->SetMaxDuration(bloodPlague->GetDuration());
                    vulnerability->SetDuration(bloodPlague->GetDuration());
                }
            }

            ExtendAura(target->GetAura(SPELL_DK_FROST_FEVER, casterGuid), extension);
            ExtendAura(target->GetAura(SPELL_DK_CHAINS_OF_ICE, casterGuid), extension);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_dk_festering_strike_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dk_festering_strike_SpellScript();
    }
};

// 45462 - Plague Strike
class spell_dk_plague_strike : public SpellScriptLoader
{
public:
    spell_dk_plague_strike() : SpellScriptLoader("spell_dk_plague_strike") { }

    class spell_dk_plague_strike_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_plague_strike_SpellScript);

        void HandleTrigger(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (Unit* target = GetHitUnit())
                if (caster->HasAura(SPELL_DK_EBON_PLAGUEBRINGER))
                    caster->CastSpell(target, SPELL_DK_FROST_FEVER, true);
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_dk_plague_strike_SpellScript::HandleTrigger, EFFECT_2, SPELL_EFFECT_TRIGGER_SPELL);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dk_plague_strike_SpellScript();
    }
};

// 45524 - Chains of Ice
class spell_dk_chilblains : public SpellScriptLoader
{
public:
    spell_dk_chilblains() : SpellScriptLoader("spell_dk_chilblains") { }

    class spell_dk_chilblains_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_chilblains_SpellScript);

        void HandleHit()
        {
            if (Unit* target = GetHitUnit())
                if (GetCaster()->HasAura(SPELL_DK_CHILBLAINS))
                    GetCaster()->CastSpell(target, SPELL_DK_CHAINS_OF_ICE_ROOT, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_dk_chilblains_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dk_chilblains_SpellScript();
    }
};

// 115994 - Unholy Blight
class spell_dk_unholy_blight : public SpellScriptLoader
{
public:
    spell_dk_unholy_blight() : SpellScriptLoader("spell_dk_unholy_blight") { }

    class spell_dk_unholy_blight_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_unholy_blight_SpellScript);

        void HandleHit()
        {
            if (Unit* target = GetHitUnit())
            {
                GetCaster()->CastSpell(target, SPELL_DK_BLOOD_PLAGUE, true);
                GetCaster()->CastSpell(target, SPELL_DK_FROST_FEVER, true);
            }
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_dk_unholy_blight_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dk_unholy_blight_SpellScript();
    }
};

// 50842 - Pestilence, 116617 - Pestilence (Roiling Blood)
class spell_dk_pestilence : public SpellScriptLoader
{
public:
    spell_dk_pestilence() : SpellScriptLoader("spell_dk_pestilence") { }

    class spell_dk_pestilence_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_pestilence_SpellScript);

        void HandleScript(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* hitTarget = GetHitUnit();
            Unit* mainTarget = GetExplTargetUnit();
            if (!hitTarget || !mainTarget || hitTarget == mainTarget)
                return;

            bool spread = false;
            if (mainTarget->HasAura(SPELL_DK_BLOOD_PLAGUE, caster->GetGUID()))
            {
                caster->CastSpell(hitTarget, SPELL_DK_BLOOD_PLAGUE, true);
                spread = true;
            }
            if (mainTarget->HasAura(SPELL_DK_FROST_FEVER, caster->GetGUID()))
            {
                caster->CastSpell(hitTarget, SPELL_DK_FROST_FEVER, true);
                spread = true;
            }
            if (spread)
                mainTarget->CastSpell(hitTarget, SPELL_DK_PESTILENCE_VISUAL, true);
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_dk_pestilence_SpellScript::HandleScript, EFFECT_2, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dk_pestilence_SpellScript();
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

        void HandleHit()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            Player* deathKnight = caster->GetAffectingPlayer();
            if (!deathKnight || !target)
                return;

            if (deathKnight->HasAura(SPELL_DK_SCARLET_FEVER))
            {
                caster->CastSpell(target, SPELL_DK_WEAKENED_BLOWS, true);
                if (Aura* bloodPlague = target->GetAura(SPELL_DK_BLOOD_PLAGUE, caster->GetGUID()))
                    bloodPlague->RefreshDuration();
                if (Aura* frostFever = target->GetAura(SPELL_DK_FROST_FEVER, caster->GetGUID()))
                    frostFever->RefreshDuration();
            }

            if (deathKnight->HasAura(SPELL_DK_FESTERING_BLOOD) || target->HasAura(SPELL_DK_FROST_FEVER) || target->HasAura(SPELL_DK_BLOOD_PLAGUE))
            {
                SetHitDamage(int32(GetHitDamage() * 1.5f));

                if (deathKnight->HasAura(SPELL_DK_ROILING_BLOOD))
                    caster->CastSpell(target, SPELL_DK_PESTILENCE_BLOOD_BOIL, true);
            }
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_dk_blood_boil_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dk_blood_boil_SpellScript();
    }
};

// 108196 - Death Siphon
class spell_dk_death_siphon : public SpellScriptLoader
{
public:
    spell_dk_death_siphon() : SpellScriptLoader("spell_dk_death_siphon") { }

    class spell_dk_death_siphon_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_death_siphon_SpellScript);

        void HandleAfterHit()
        {
            Unit* target = GetHitUnit();
            if (!target)
                return;

            int32 damage = GetHitDamage();
            if (Player* targetPlayer = target->GetAffectingPlayer())
                if (GetCaster()->GetAffectingPlayer())
                    damage = int32(damage / (1.0f + targetPlayer->GetFloatValue(PLAYER_FIELD_MOD_RESILIENCE_PERCENT) / 100.0f));

            if (int32 heal = CalculatePct(damage, GetSpellInfo()->Effects[EFFECT_1].BasePoints))
                GetCaster()->CastCustomSpell(SPELL_DK_DEATH_SIPHON_HEAL, SPELLVALUE_BASE_POINT0, heal, GetCaster(), true);
        }

        void Register() OVERRIDE
        {
            AfterHit += SpellHitFn(spell_dk_death_siphon_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dk_death_siphon_SpellScript();
    }
};

// 108200 - Remorseless Winter
class spell_dk_remorseless_winter_targets : public SpellScriptLoader
{
public:
    spell_dk_remorseless_winter_targets() : SpellScriptLoader("spell_dk_remorseless_winter_targets") { }

    class spell_dk_remorseless_winter_targets_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_remorseless_winter_targets_AuraScript);

    public:
        std::set<uint64> Stunned;

        void Register() OVERRIDE { }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_remorseless_winter_targets_AuraScript();
    }
};

// 115000 - Remorseless Winter
class spell_dk_remorseless_winter : public SpellScriptLoader
{
public:
    spell_dk_remorseless_winter() : SpellScriptLoader("spell_dk_remorseless_winter") { }

    class spell_dk_remorseless_winter_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_remorseless_winter_SpellScript);

        typedef spell_dk_remorseless_winter_targets::spell_dk_remorseless_winter_targets_AuraScript TargetsScript;

        TargetsScript* GetTargetsScript()
        {
            if (Aura* aura = GetCaster()->GetAura(SPELL_DK_REMORSELESS_WINTER))
                return dynamic_cast<TargetsScript*>(aura->GetScriptByName("spell_dk_remorseless_winter_targets"));
            return NULL;
        }

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            TargetsScript* script = GetTargetsScript();
            uint32 const spellId = GetSpellInfo()->Id;
            targets.remove_if([script, spellId](WorldObject const* object)
            {
                Unit const* unit = object->ToUnit();
                if (!unit)
                    return true;
                if (Aura const* winter = unit->GetAura(spellId))
                    if (winter->GetStackAmount() >= 5)
                        return true;
                return script && script->Stunned.count(object->GetGUID()) != 0;
            });
        }

        void HandleHit()
        {
            Unit* target = GetHitUnit();
            if (!target)
                return;

            Aura const* winter = target->GetAura(GetSpellInfo()->Id);
            if (!winter || winter->GetStackAmount() != 5 || target->HasAura(SPELL_DK_REMORSELESS_WINTER_STUN))
                return;

            GetCaster()->CastSpell(target, SPELL_DK_REMORSELESS_WINTER_STUN, true);
            if (TargetsScript* script = GetTargetsScript())
                script->Stunned.insert(target->GetGUID());
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dk_remorseless_winter_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dk_remorseless_winter_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
            OnHit += SpellHitFn(spell_dk_remorseless_winter_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dk_remorseless_winter_SpellScript();
    }
};

// 118009 - Desecrated Ground
class spell_dk_desecrated_ground : public SpellScriptLoader
{
public:
    spell_dk_desecrated_ground() : SpellScriptLoader("spell_dk_desecrated_ground") { }

    class spell_dk_desecrated_ground_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_desecrated_ground_AuraScript);

        void HandleTick(AuraEffect const* /*aurEff*/)
        {
            if (Unit* caster = GetCaster())
                if (DynamicObject* dynObj = caster->GetDynObject(SPELL_DK_DESECRATED_GROUND))
                    if (caster->GetDistance(dynObj) <= 8.0f)
                        caster->CastSpell(caster, SPELL_DK_DESECRATED_GROUND_IMMUNE, true);
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_dk_desecrated_ground_AuraScript::HandleTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_desecrated_ground_AuraScript();
    }
};

// 123693 - Plague Leech
class spell_dk_plague_leech : public SpellScriptLoader
{
public:
    spell_dk_plague_leech() : SpellScriptLoader("spell_dk_plague_leech") { }

    class spell_dk_plague_leech_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_plague_leech_SpellScript);

        SpellCastResult CheckCast()
        {
            Unit* target = GetExplTargetUnit();
            Player* player = GetCaster()->ToPlayer();
            if (!target || !player)
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;

            if (target->GetDiseasesByCaster(player->GetGUID()) < 2)
            {
                SetCustomCastResultMessage(SpellCustomErrors(DK_CUSTOM_ERROR_BOTH_FROST_FEVER_BLOOD_PLAGUE));
                return SpellCastResult::SPELL_FAILED_CUSTOM_ERROR;
            }

            for (uint8 i = 0; i < MAX_RUNES; ++i)
                if (IsRuneFullyDepleted(player, i))
                    return SpellCastResult::SPELL_CAST_OK;

            SetCustomCastResultMessage(SpellCustomErrors(DK_CUSTOM_ERROR_NO_FULLY_DEPLETED_RUNES));
            return SpellCastResult::SPELL_FAILED_CUSTOM_ERROR;
        }

        void HandleCast()
        {
            Player* player = GetCaster()->ToPlayer();
            if (!player)
                return;

            uint32 const spec = player->GetTalentSpecialization(player->GetActiveSpec());
            uint8 const bloodOrFrostPriority[3] = { 2, 4, 0 };
            uint8 const unholyPriority[3] = { 0, 2, 4 };
            uint8 const* priority = (spec == SPEC_DEATH_KNIGHT_BLOOD || spec == SPEC_DEATH_KNIGHT_FROST) ? bloodOrFrostPriority : unholyPriority;

            uint8 restored = 0;
            for (uint8 i = 0; i < 3 && restored < 2; ++i)
            {
                uint8 const first = priority[i];
                int8 rune = -1;
                if (IsRuneFullyDepleted(player, first))
                    rune = first;
                else if (IsRuneFullyDepleted(player, first + 1))
                    rune = first + 1;

                if (rune < 0)
                    continue;

                player->SetRuneCooldown(uint8(rune), 0);
                player->ConvertRune(uint8(rune), RuneType::RUNE_DEATH);
                ++restored;
            }

            player->ResyncRunes(MAX_RUNES);
        }

        void HandleHit()
        {
            if (Unit* target = GetHitUnit())
            {
                target->RemoveAurasDueToSpell(SPELL_DK_BLOOD_PLAGUE, GetCaster()->GetGUID());
                target->RemoveAurasDueToSpell(SPELL_DK_FROST_FEVER, GetCaster()->GetGUID());
            }
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_dk_plague_leech_SpellScript::CheckCast);
            OnCast += SpellCastFn(spell_dk_plague_leech_SpellScript::HandleCast);
            OnHit += SpellHitFn(spell_dk_plague_leech_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dk_plague_leech_SpellScript();
    }
};

// 116888 - Shroud of Purgatory
class spell_dk_purgatory : public SpellScriptLoader
{
public:
    spell_dk_purgatory() : SpellScriptLoader("spell_dk_purgatory") { }

    class spell_dk_purgatory_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_purgatory_AuraScript);

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
                return;

            if (Player* player = GetTarget()->ToPlayer())
                player->CastSpell(player, SPELL_DK_PURGATORY_INSTAKILL, true);
        }

        void Register() OVERRIDE
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_dk_purgatory_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_SCHOOL_HEAL_ABSORB, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_purgatory_AuraScript();
    }
};

// 114556 - Purgatory
class spell_dk_purgatory_absorb : public SpellScriptLoader
{
public:
    spell_dk_purgatory_absorb() : SpellScriptLoader("spell_dk_purgatory_absorb") { }

    class spell_dk_purgatory_absorb_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_purgatory_absorb_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            amount = -1;
        }

        void Absorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
        {
            Unit* target = GetTarget();
            absorbAmount = 0;

            if (dmgInfo.GetDamage() < target->GetHealth())
                return;

            if (target->HasAura(SPELL_DK_SHROUD_OF_PURGATORY))
            {
                absorbAmount = dmgInfo.GetDamage();
                return;
            }

            if (target->HasAura(SPELL_DK_PERDITION))
                return;

            int32 bp = int32(dmgInfo.GetDamage());
            target->CastCustomSpell(target, SPELL_DK_SHROUD_OF_PURGATORY, &bp, NULL, NULL, true);
            target->CastSpell(target, SPELL_DK_PERDITION, true);
            target->SetHealth(1);
            absorbAmount = dmgInfo.GetDamage();
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_purgatory_absorb_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_purgatory_absorb_AuraScript::Absorb, EFFECT_0);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_purgatory_absorb_AuraScript();
    }
};

// 96268 - Death's Advance
class spell_dk_deaths_advance : public SpellScriptLoader
{
public:
    spell_dk_deaths_advance() : SpellScriptLoader("spell_dk_deaths_advance") { }

    class spell_dk_deaths_advance_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_deaths_advance_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* caster = GetCaster())
                if (caster->HasAura(SPELL_DK_PILLAR_OF_FROST_GLYPHED))
                    amount = 0;
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_deaths_advance_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_SPEED_NOT_STACK);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_deaths_advance_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_MINIMUM_SPEED);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_deaths_advance_AuraScript();
    }
};

// 48263 - Blood Presence, 48265 - Unholy Presence, 48266 - Frost Presence
class spell_dk_presence : public SpellScriptLoader
{
public:
    spell_dk_presence() : SpellScriptLoader("spell_dk_presence") { }

    class spell_dk_presence_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_presence_SpellScript);

        void HandleCast()
        {
            Unit* caster = GetCaster();
            int32 amount = 0;
            if (Aura* glyph = caster->GetAura(SPELL_DK_GLYPH_OF_SHIFTING_PRESENCES))
                amount = CalculatePct(caster->GetPower(POWER_RUNIC_POWER), glyph->GetSpellInfo()->Effects[EFFECT_0].BasePoints);
            caster->SetPower(POWER_RUNIC_POWER, amount);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_dk_presence_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dk_presence_SpellScript();
    }
};

// 57330 - Horn of Winter
class spell_dk_glyph_of_horn_of_winter : public SpellScriptLoader
{
public:
    spell_dk_glyph_of_horn_of_winter() : SpellScriptLoader("spell_dk_glyph_of_horn_of_winter") { }

    class spell_dk_glyph_of_horn_of_winter_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_glyph_of_horn_of_winter_SpellScript);

        void HandleAfterCast()
        {
            Unit* caster = GetCaster();
            if (!caster->IsInCombat() && caster->HasAura(SPELL_DK_GLYPH_OF_HORN_OF_WINTER))
                caster->CastSpell(caster, SPELL_DK_GLYPH_OF_HORN_OF_WINTER_EFFECT, true);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_dk_glyph_of_horn_of_winter_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dk_glyph_of_horn_of_winter_SpellScript();
    }
};

// 57330 - Horn of Winter
class spell_dk_glyph_of_the_loud_horn : public SpellScriptLoader
{
public:
    spell_dk_glyph_of_the_loud_horn() : SpellScriptLoader("spell_dk_glyph_of_the_loud_horn") { }

    class spell_dk_glyph_of_the_loud_horn_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_glyph_of_the_loud_horn_SpellScript);

        void HandleCast()
        {
            Unit* caster = GetCaster();
            if (caster->HasAura(SPELL_DK_GLYPH_OF_THE_LOUD_HORN))
                caster->CastSpell(caster, SPELL_DK_GLYPH_OF_THE_LOUD_HORN_ENERGIZE, true);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_dk_glyph_of_the_loud_horn_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dk_glyph_of_the_loud_horn_SpellScript();
    }
};

// 43265 - Death and Decay
class spell_dk_glyph_of_death_and_decay : public SpellScriptLoader
{
public:
    spell_dk_glyph_of_death_and_decay() : SpellScriptLoader("spell_dk_glyph_of_death_and_decay") { }

    class spell_dk_glyph_of_death_and_decay_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_glyph_of_death_and_decay_SpellScript);

        void HandleAfterCast()
        {
            Unit* caster = GetCaster();
            if (!caster->HasAura(SPELL_DK_GLYPH_OF_DEATH_AND_DECAY))
                return;

            if (WorldLocation const* dest = GetExplTargetDest())
                caster->CastSpell(dest->GetPositionX(), dest->GetPositionY(), dest->GetPositionZ(), SPELL_DK_DEATH_AND_DECAY_SLOW, true);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_dk_glyph_of_death_and_decay_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dk_glyph_of_death_and_decay_SpellScript();
    }
};

// 59327 - Glyph of Unholy Command
class spell_dk_glyph_of_unholy_command : public SpellScriptLoader
{
public:
    spell_dk_glyph_of_unholy_command() : SpellScriptLoader("spell_dk_glyph_of_unholy_command") { }

    class spell_dk_glyph_of_unholy_command_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_glyph_of_unholy_command_AuraScript);

        void HandleProc(ProcEventInfo& /*eventInfo*/)
        {
            if (Player* player = GetOwner()->ToPlayer())
                player->RemoveSpellCooldown(SPELL_DK_DEATH_GRIP, true);
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_dk_glyph_of_unholy_command_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_glyph_of_unholy_command_AuraScript();
    }
};

// 47476 - Strangulate
class spell_dk_glyph_of_strangulate : public SpellScriptLoader
{
public:
    spell_dk_glyph_of_strangulate() : SpellScriptLoader("spell_dk_glyph_of_strangulate") { }

    class spell_dk_glyph_of_strangulate_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_glyph_of_strangulate_SpellScript);

        void HandleBeforeHit()
        {
            Unit* target = GetHitUnit();
            if (!target || !GetCaster()->HasAura(SPELL_DK_GLYPH_OF_STRANGULATE))
                return;

            for (uint32 i = CURRENT_MELEE_SPELL; i < CURRENT_MAX_SPELL; ++i)
                if (Spell* spell = target->GetCurrentSpell(CurrentSpellTypes(i)))
                    if (spell->GetSpellInfo()->PreventionType & SPELL_PREVENTION_TYPE_SILENCE)
                        _interrupted = true;
        }

        void HandleAfterHit()
        {
            if (!_interrupted)
                return;

            if (Aura* strangulate = GetHitAura())
            {
                strangulate->SetMaxDuration(strangulate->GetMaxDuration() + 2 * IN_MILLISECONDS);
                strangulate->SetDuration(strangulate->GetDuration() + 2 * IN_MILLISECONDS);
            }
        }

        void Register() OVERRIDE
        {
            BeforeHit += SpellHitFn(spell_dk_glyph_of_strangulate_SpellScript::HandleBeforeHit);
            AfterHit += SpellHitFn(spell_dk_glyph_of_strangulate_SpellScript::HandleAfterHit);
        }

    private:
        bool _interrupted = false;
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dk_glyph_of_strangulate_SpellScript();
    }
};

// 108194 - Asphyxiate
class spell_dk_asphyxiate : public SpellScriptLoader
{
public:
    spell_dk_asphyxiate() : SpellScriptLoader("spell_dk_asphyxiate") { }

    class spell_dk_asphyxiate_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_asphyxiate_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DK_STRANGULATE))
                return false;
            return true;
        }

        void HandleAfterHit()
        {
            Unit* target = GetHitUnit();
            if (!target)
                return;

            if (GetSpell()->GetDiminishingLevel() != DIMINISHING_LEVEL_IMMUNE && !target->HasUnitState(UNIT_STATE_STUNNED))
                GetCaster()->CastSpell(target, SPELL_DK_STRANGULATE, true);
        }

        void Register() OVERRIDE
        {
            AfterHit += SpellHitFn(spell_dk_asphyxiate_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dk_asphyxiate_SpellScript();
    }
};

// 146652 - Glyph of the Skeleton
class spell_dk_glyph_of_the_skeleton : public SpellScriptLoader
{
public:
    spell_dk_glyph_of_the_skeleton() : SpellScriptLoader("spell_dk_glyph_of_the_skeleton") { }

    class spell_dk_glyph_of_the_skeleton_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_glyph_of_the_skeleton_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Guardian* ghoul = GetUnitOwner()->GetGuardianPet())
                ghoul->CastSpell(ghoul, SPELL_DK_SKELETON_TRANSFORM, true);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Guardian* ghoul = GetUnitOwner()->GetGuardianPet())
                ghoul->RemoveAurasDueToSpell(SPELL_DK_SKELETON_TRANSFORM);
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_dk_glyph_of_the_skeleton_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_dk_glyph_of_the_skeleton_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_glyph_of_the_skeleton_AuraScript();
    }
};

// 58640 - Glyph of the Geist
class spell_dk_glyph_of_the_geist : public SpellScriptLoader
{
public:
    spell_dk_glyph_of_the_geist() : SpellScriptLoader("spell_dk_glyph_of_the_geist") { }

    class spell_dk_glyph_of_the_geist_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_glyph_of_the_geist_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            std::list<Creature*> ghouls;
            GetUnitOwner()->GetAllMinionsByEntry(ghouls, NPC_DK_RISEN_GHOUL);
            for (Creature* ghoul : ghouls)
                ghoul->CastSpell(ghoul, SPELL_DK_GEIST_TRANSFORM, true);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            std::list<Creature*> ghouls;
            GetUnitOwner()->GetAllMinionsByEntry(ghouls, NPC_DK_RISEN_GHOUL);
            for (Creature* ghoul : ghouls)
                ghoul->RemoveAurasDueToSpell(SPELL_DK_GEIST_TRANSFORM);
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_dk_glyph_of_the_geist_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_dk_glyph_of_the_geist_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_glyph_of_the_geist_AuraScript();
    }
};

// 81164 - Will of the Necropolis
class spell_dk_will_of_the_necropolis : public SpellScriptLoader
{
public:
    spell_dk_will_of_the_necropolis() : SpellScriptLoader("spell_dk_will_of_the_necropolis") { }

    class spell_dk_will_of_the_necropolis_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_will_of_the_necropolis_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            DamageInfo* damageInfo = eventInfo.GetDamageInfo();
            return damageInfo && GetUnitOwner()->HealthBelowPctDamaged(30, damageInfo->GetDamage());
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_dk_will_of_the_necropolis_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_will_of_the_necropolis_AuraScript();
    }
};

// 96171 - Will of the Necropolis
class spell_dk_will_of_the_necropolis_rune_tap : public SpellScriptLoader
{
public:
    spell_dk_will_of_the_necropolis_rune_tap() : SpellScriptLoader("spell_dk_will_of_the_necropolis_rune_tap") { }

    class spell_dk_will_of_the_necropolis_rune_tap_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_will_of_the_necropolis_rune_tap_SpellScript);

        void HandleCast()
        {
            if (Player* player = GetCaster()->ToPlayer())
                player->RemoveSpellCooldown(SPELL_DK_RUNE_TAP, true);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_dk_will_of_the_necropolis_rune_tap_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dk_will_of_the_necropolis_rune_tap_SpellScript();
    }
};

// 81136 - Crimson Scourge
class spell_dk_crimson_scourge : public SpellScriptLoader
{
public:
    spell_dk_crimson_scourge() : SpellScriptLoader("spell_dk_crimson_scourge") { }

    class spell_dk_crimson_scourge_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_crimson_scourge_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            Unit* target = eventInfo.GetActionTarget();
            return target && target->HasAura(SPELL_DK_BLOOD_PLAGUE, GetCasterGUID());
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_dk_crimson_scourge_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_crimson_scourge_AuraScript();
    }
};

// 131542 - Relentless Grip
class spell_dk_relentless_grip : public SpellScriptLoader
{
public:
    spell_dk_relentless_grip() : SpellScriptLoader("spell_dk_relentless_grip") { }

    class spell_dk_relentless_grip_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_relentless_grip_AuraScript);

        void HandleProc(ProcEventInfo& /*eventInfo*/)
        {
            GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_DK_RELENTLESS_GRIP, true);
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_dk_relentless_grip_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_relentless_grip_AuraScript();
    }
};

// 51963 - Gargoyle Strike
class spell_dk_gargoyle_strike : public SpellScriptLoader
{
public:
    spell_dk_gargoyle_strike() : SpellScriptLoader("spell_dk_gargoyle_strike") { }

    class spell_dk_gargoyle_strike_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_gargoyle_strike_SpellScript);

        void HandleHit()
        {
            if (Unit* owner = GetCaster()->GetOwner())
            {
                if (AuraEffect const* mastery = owner->GetAuraEffect(SPELL_DK_MASTERY_DREADBLADE, EFFECT_0))
                {
                    int32 damage = GetHitDamage();
                    AddPct(damage, mastery->GetAmount());
                    SetHitDamage(damage);
                }
            }
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_dk_gargoyle_strike_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dk_gargoyle_strike_SpellScript();
    }
};

// 145677 - Riposte
class spell_dk_riposte : public SpellScriptLoader
{
public:
    spell_dk_riposte() : SpellScriptLoader("spell_dk_riposte") { }

    class spell_dk_riposte_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_riposte_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
        {
            canBeRecalculated = false;

            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (Player* player = caster->ToPlayer())
            {
                uint32 rating = player->GetUInt32Value(PLAYER_FIELD_COMBAT_RATINGS + uint16(CombatRating::CR_PARRY));
                rating += player->GetUInt32Value(PLAYER_FIELD_COMBAT_RATINGS + uint16(CombatRating::CR_DODGE));
                amount = int32(CalculatePct(rating, 75));
            }
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_riposte_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_RATING);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_riposte_AuraScript();
    }
};

// 113072 - Might of Ursoc (Symbiosis)
class spell_dk_might_of_ursoc : public SpellScriptLoader
{
public:
    spell_dk_might_of_ursoc() : SpellScriptLoader("spell_dk_might_of_ursoc") { }

    class spell_dk_might_of_ursoc_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_might_of_ursoc_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
                if (caster->GetHealthPct() < 15.0f)
                    caster->SetHealth(caster->CountPctFromMaxHealth(15));
        }

        void Register() OVERRIDE
        {
            AfterEffectApply += AuraEffectApplyFn(spell_dk_might_of_ursoc_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH_2, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_might_of_ursoc_AuraScript();
    }
};

// 113517 - Wild Mushroom: Plague (Symbiosis)
class spell_dk_wild_mushroom_plague_periodic : public SpellScriptLoader
{
public:
    spell_dk_wild_mushroom_plague_periodic() : SpellScriptLoader("spell_dk_wild_mushroom_plague_periodic") { }

    class spell_dk_wild_mushroom_plague_periodic_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_wild_mushroom_plague_periodic_AuraScript);

        void HandleTick(AuraEffect const* /*aurEff*/)
        {
            Unit* mushroom = GetUnitOwner();
            if (Unit* owner = mushroom->GetOwner())
                owner->CastSpell(mushroom, SPELL_DK_WILD_MUSHROOM_PLAGUE_TRIGGER, true);
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_dk_wild_mushroom_plague_periodic_AuraScript::HandleTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_wild_mushroom_plague_periodic_AuraScript();
    }
};

// 113518 - Wild Mushroom: Plague (Symbiosis)
class spell_dk_wild_mushroom_plague_trigger : public SpellScriptLoader
{
public:
    spell_dk_wild_mushroom_plague_trigger() : SpellScriptLoader("spell_dk_wild_mushroom_plague_trigger") { }

    class spell_dk_wild_mushroom_plague_trigger_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_wild_mushroom_plague_trigger_SpellScript);

        void HandleHit()
        {
            if (Unit* target = GetHitUnit())
            {
                GetCaster()->CastSpell(target, SPELL_DK_BLOOD_PLAGUE, true);
                GetCaster()->CastSpell(target, SPELL_DK_FROST_FEVER, true);
            }
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_dk_wild_mushroom_plague_trigger_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dk_wild_mushroom_plague_trigger_SpellScript();
    }
};

// 147620 - Frozen Power
class spell_dk_frozen_power : public SpellScriptLoader
{
public:
    spell_dk_frozen_power() : SpellScriptLoader("spell_dk_frozen_power") { }

    class spell_dk_frozen_power_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_frozen_power_SpellScript);

        void HandleHit()
        {
            if (Player* player = GetCaster()->ToPlayer())
                if (Item const* weapon = player->GetWeaponForAttack(WeaponAttackType::BASE_ATTACK))
                    if (weapon->GetTemplate()->InventoryType == INVTYPE_2HWEAPON)
                        return;

            SetHitDamage(GetHitDamage() / 2);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_dk_frozen_power_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dk_frozen_power_SpellScript();
    }
};

// 51124 - Killing Machine
class spell_dk_t15_dps_4p_bonus : public SpellScriptLoader
{
public:
    spell_dk_t15_dps_4p_bonus() : SpellScriptLoader("spell_dk_t15_dps_4p_bonus") { }

    class spell_dk_t15_dps_4p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_t15_dps_4p_bonus_AuraScript);

        void CalculateSpellMod(AuraEffect const* /*aurEff*/, SpellModifier*& spellMod)
        {
            if (spellMod && GetUnitOwner()->HasAura(SPELL_DK_T15_DPS_4P_BONUS))
                spellMod->mask[2] |= 0x04000000; // Soul Reaper
        }

        void Register() OVERRIDE
        {
            DoEffectCalcSpellMod += AuraEffectCalcSpellModFn(spell_dk_t15_dps_4p_bonus_AuraScript::CalculateSpellMod, EFFECT_0, SPELL_AURA_ADD_FLAT_MODIFIER);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_t15_dps_4p_bonus_AuraScript();
    }
};

// 138197 - Item - Death Knight T15 Blood 4P Bonus
class spell_dk_t15_blood_4p_bonus : public SpellScriptLoader
{
public:
    spell_dk_t15_blood_4p_bonus() : SpellScriptLoader("spell_dk_t15_blood_4p_bonus") { }

    class spell_dk_t15_blood_4p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_t15_blood_4p_bonus_AuraScript);

        void HandleProc(ProcEventInfo& /*eventInfo*/)
        {
            if (GetUnitOwner()->HasAura(SPELL_DK_T15_BLOOD_4P_BONUS))
                GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_DK_RUNIC_POWER_T15_BLOOD, true);
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_dk_t15_blood_4p_bonus_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_t15_blood_4p_bonus_AuraScript();
    }
};

// 144901 - Death Shroud
class spell_dk_death_shroud_duration : public SpellScriptLoader
{
public:
    spell_dk_death_shroud_duration() : SpellScriptLoader("spell_dk_death_shroud_duration") { }

    class spell_dk_death_shroud_duration_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dk_death_shroud_duration_SpellScript);

        bool Load() OVERRIDE
        {
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void HandleHit()
        {
            Aura* aura = GetHitAura();
            if (!aura)
                return;

            Player* player = GetCaster()->ToPlayer();
            if (player->GetTalentSpecialization(player->GetActiveSpec()) == SPEC_DEATH_KNIGHT_UNHOLY)
                aura->SetMaxDuration(15 * IN_MILLISECONDS);
            else
            {
                int32 hands = 1;
                Item const* weapon = player->GetWeaponForAttack(WeaponAttackType::BASE_ATTACK);
                if (weapon && weapon->GetTemplate()->InventoryType == INVTYPE_2HWEAPON)
                    hands = 2;

                aura->SetMaxDuration((hands * 2 + 4) * IN_MILLISECONDS);
            }

            aura->SetDuration(aura->GetMaxDuration());
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_dk_death_shroud_duration_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_dk_death_shroud_duration_SpellScript();
    }
};

// 144901 - Death Shroud
class spell_dk_death_shroud : public SpellScriptLoader
{
public:
    spell_dk_death_shroud() : SpellScriptLoader("spell_dk_death_shroud") { }

    class spell_dk_death_shroud_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_death_shroud_AuraScript);

        bool Load() OVERRIDE
        {
            Player* player = GetOwner()->ToPlayer();
            if (!player)
                return false;

            _haste = player->GetRatingBonusValue(CombatRating::CR_HASTE_MELEE) > player->GetRatingBonusValue(CombatRating::CR_MASTERY);
            return true;
        }

        void CalculateHaste(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (!_haste)
                amount = 0;
        }

        void CalculateMastery(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (_haste)
                amount = 0;
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_death_shroud_AuraScript::CalculateHaste, EFFECT_0, SPELL_AURA_MOD_RATING);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_death_shroud_AuraScript::CalculateMastery, EFFECT_1, SPELL_AURA_MOD_RATING);
        }

    private:
        bool _haste = false;
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_death_shroud_AuraScript();
    }
};

// 144907 - Item - Death Knight T16 DPS 4P Bonus
class spell_dk_t16_dps_4p_bonus : public SpellScriptLoader
{
public:
    spell_dk_t16_dps_4p_bonus() : SpellScriptLoader("spell_dk_t16_dps_4p_bonus") { }

    class spell_dk_t16_dps_4p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_t16_dps_4p_bonus_AuraScript);

        void HandleProc(ProcEventInfo& eventInfo)
        {
            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            if (!spellInfo)
                return;

            Unit* owner = GetUnitOwner();
            if (spellInfo->Id == SPELL_DK_DEATH_COIL)
            {
                if (Guardian* ghoul = owner->GetGuardianPet())
                {
                    if (Aura* darkTransformation = ghoul->GetAura(SPELL_DK_DARK_TRANSFORMATION))
                    {
                        int32 const duration = darkTransformation->GetDuration() + 2 * IN_MILLISECONDS;
                        if (darkTransformation->GetMaxDuration() < duration)
                            darkTransformation->SetMaxDuration(duration);
                        darkTransformation->SetDuration(duration);
                    }
                }
            }
            else if (spellInfo->Id == SPELL_DK_PILLAR_OF_FROST)
                owner->CastSpell(owner, SPELL_DK_T16_DPS_4P_BONUS_HIDDEN, true);
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_dk_t16_dps_4p_bonus_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_t16_dps_4p_bonus_AuraScript();
    }
};

// 144934 - Item - Death Knight T16 Blood 2P Bonus
class spell_dk_t16_blood_2p_bonus : public SpellScriptLoader
{
public:
    spell_dk_t16_blood_2p_bonus() : SpellScriptLoader("spell_dk_t16_blood_2p_bonus") { }

    class spell_dk_t16_blood_2p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_t16_blood_2p_bonus_AuraScript);

        void HandleProc(ProcEventInfo& /*eventInfo*/)
        {
            Unit* owner = GetUnitOwner();
            if (++_counter >= 10 && owner->HasAura(SPELL_DK_T16_BLOOD_2P_BONUS))
            {
                _counter = 0;
                owner->CastSpell(owner, SPELL_DK_BONE_WALL, true);
            }
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_dk_t16_blood_2p_bonus_AuraScript::HandleProc);
        }

    private:
        uint32 _counter = 0;
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_t16_blood_2p_bonus_AuraScript();
    }
};

// 144950 - Item - Death Knight T16 Blood 4P Bonus
class spell_dk_t16_blood_4p_bonus : public SpellScriptLoader
{
public:
    spell_dk_t16_blood_4p_bonus() : SpellScriptLoader("spell_dk_t16_blood_4p_bonus") { }

    class spell_dk_t16_blood_4p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_t16_blood_4p_bonus_AuraScript);

        bool Load() OVERRIDE
        {
            Player* player = GetOwner()->ToPlayer();
            return player && player->getClass() == CLASS_DEATH_KNIGHT;
        }

        void HandleProc(ProcEventInfo& /*eventInfo*/)
        {
            Player* player = GetUnitOwner()->ToPlayer();
            if (!player)
                return;

            for (uint8 i = 0; i < MAX_RUNES; ++i)
            {
                RuneType const baseRune = player->GetBaseRune(i);
                if (baseRune != RuneType::RUNE_FROST && baseRune != RuneType::RUNE_UNHOLY)
                    continue;

                player->SetRuneCooldown(i, 0);
                player->ConvertRune(i, RuneType::RUNE_DEATH);
            }

            player->ResyncRunes(MAX_RUNES);
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_dk_t16_blood_4p_bonus_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_t16_blood_4p_bonus_AuraScript();
    }
};

// 114866 - Soul Reaper (Blood), 130735 - Soul Reaper (Frost), 130736 - Soul Reaper (Unholy)
class spell_dk_soul_reaper : public SpellScriptLoader
{
public:
    spell_dk_soul_reaper() : SpellScriptLoader("spell_dk_soul_reaper") { }

    class spell_dk_soul_reaper_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_soul_reaper_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DK_SOUL_REAPER_DAMAGE) || !sSpellMgr->GetSpellInfo(SPELL_DK_SOUL_REAPER_HASTE))
                return false;
            return true;
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            AuraRemoveMode const removeMode = GetTargetApplication()->GetRemoveMode();
            if (removeMode == AURA_REMOVE_BY_DEATH)
                caster->CastSpell(caster, SPELL_DK_SOUL_REAPER_HASTE, true);
            else if (removeMode == AURA_REMOVE_BY_EXPIRE)
            {
                int32 healthPct = 35;
                if (AuraEffect const* bonus = caster->GetAuraEffect(SPELL_DK_T15_DPS_4P_BONUS, EFFECT_0))
                    healthPct = bonus->GetAmount();

                if (GetTarget()->HealthBelowPct(healthPct))
                    caster->CastSpell(GetTarget(), SPELL_DK_SOUL_REAPER_DAMAGE, true);
            }
        }

        void Register() OVERRIDE
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_dk_soul_reaper_AuraScript::HandleRemove, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_soul_reaper_AuraScript();
    }
};

// 114868 - Soul Reaper
class spell_dk_soul_reaper_haste : public SpellScriptLoader
{
public:
    spell_dk_soul_reaper_haste() : SpellScriptLoader("spell_dk_soul_reaper_haste") { }

    class spell_dk_soul_reaper_haste_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_soul_reaper_haste_AuraScript);

        void HandleInitEffects(uint32& effectMask)
        {
            Unit* caster = GetCaster();
            if (!caster || !caster->HasAura(SPELL_DK_GLYPH_OF_SWIFT_DEATH))
                effectMask &= ~(1 << EFFECT_1);
        }

        void Register() OVERRIDE
        {
            OnInitEffects += AuraInitEffectsFn(spell_dk_soul_reaper_haste_AuraScript::HandleInitEffects);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_soul_reaper_haste_AuraScript();
    }
};

// 49016 - Unholy Frenzy
class spell_dk_unholy_frenzy : public SpellScriptLoader
{
public:
    spell_dk_unholy_frenzy() : SpellScriptLoader("spell_dk_unholy_frenzy") { }

    class spell_dk_unholy_frenzy_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_unholy_frenzy_AuraScript);

        void HandleInitEffects(uint32& effectMask)
        {
            Unit* caster = GetCaster();
            if (caster && caster->HasAura(SPELL_DK_GLYPH_OF_UNHOLY_FRENZY))
                effectMask &= ~(1 << EFFECT_1);
        }

        void Register() OVERRIDE
        {
            OnInitEffects += AuraInitEffectsFn(spell_dk_unholy_frenzy_AuraScript::HandleInitEffects);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_unholy_frenzy_AuraScript();
    }
};

// 77606 - Dark Simulacrum
class spell_dk_dark_simulacrum : public SpellScriptLoader
{
public:
    spell_dk_dark_simulacrum() : SpellScriptLoader("spell_dk_dark_simulacrum") { }

    class spell_dk_dark_simulacrum_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_dark_simulacrum_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DK_DARK_SIMULACRUM_OVERRIDE) || !sSpellMgr->GetSpellInfo(SPELL_DK_DARK_SIMULACRUM_SPELL_POWER))
                return false;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            Unit* caster = GetCaster();
            if (!caster || caster->GetTypeId() != TypeID::TYPEID_PLAYER)
                return false;

            Spell const* spell = eventInfo.TriggeredBySpell();
            if (!spell || spell->IsTriggered())
                return false;

            if (spell->GetPowerType() != POWER_MANA || spell->GetPowerCost() <= 0)
                return false;

            if (Creature* spellCaster = GetUnitOwner()->ToCreature())
                if (spellCaster->IsDungeonBoss() || spellCaster->isWorldBoss() || spellCaster->GetCreatureTemplate()->rank == CREATURE_ELITE_WORLDBOSS)
                    return false;

            SpellInfo const* spellInfo = spell->GetSpellInfo();

            // Combat states
            if (spellInfo->GetCategory() == 47)
                return false;

            if (spellInfo->IsPassive() || spellInfo->GetMaxDuration() == -1)
                return false;

            Player* player = caster->ToPlayer();
            if (!player->HasItemFitToSpellRequirements(spellInfo))
                return false;

            if (spellInfo->AttributesEx3 & SPELL_ATTR3_MAIN_HAND)
            {
                Item* item = player->GetWeaponForAttack(WeaponAttackType::BASE_ATTACK);
                if (!item || !item->IsFitToSpellRequirements(spellInfo))
                    return false;
            }

            if (spellInfo->AttributesEx3 & SPELL_ATTR3_REQ_OFFHAND)
            {
                Item* item = player->GetWeaponForAttack(WeaponAttackType::OFF_ATTACK);
                if (!item || !item->IsFitToSpellRequirements(spellInfo))
                    return false;
            }

            for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
            {
                switch (spellInfo->Effects[i].Effect)
                {
                    case SPELL_EFFECT_SUMMON:
                    case SPELL_EFFECT_SUMMON_PET:
                    case SPELL_EFFECT_SUMMON_OBJECT_SLOT1:
                    case SPELL_EFFECT_SUMMON_OBJECT_SLOT2:
                    case SPELL_EFFECT_SUMMON_OBJECT_SLOT3:
                    case SPELL_EFFECT_SUMMON_OBJECT_SLOT4:
                    case SPELL_EFFECT_SUMMON_OBJECT_WILD:
                        return false;
                    default:
                        break;
                }

                if (spellInfo->Effects[i].ApplyAuraName == SPELL_AURA_MOD_SHAPESHIFT)
                    return false;
            }

            return true;
        }

        void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            Unit* caster = GetCaster();
            if (!caster)
                return;

            SpellInfo const* spellInfo = eventInfo.TriggeredBySpell()->GetSpellInfo();
            int32 const spellPower = GetUnitOwner()->SpellBaseDamageBonusDone(SpellSchoolMask(spellInfo->SchoolMask));

            caster->CastCustomSpell(SPELL_DK_DARK_SIMULACRUM_OVERRIDE, SPELLVALUE_BASE_POINT0, int32(spellInfo->Id), caster, true);

            CustomSpellValues values;
            values.AddSpellMod(SPELLVALUE_BASE_POINT0, spellPower);
            values.AddSpellMod(SPELLVALUE_BASE_POINT1, spellPower);
            caster->CastCustomSpell(SPELL_DK_DARK_SIMULACRUM_SPELL_POWER, values, caster, TRIGGERED_FULL_MASK);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_dk_dark_simulacrum_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_dk_dark_simulacrum_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_dk_dark_simulacrum_AuraScript();
    }
};

void AddSC_deathknight_spell_scripts()
{
    new spell_dk_anti_magic_shell_raid();
    new spell_dk_anti_magic_shell_self();
    new spell_dk_anti_magic_zone();
    new spell_dk_anti_magic_zone_dynamic();
    new spell_dk_blood_gorged();
    new spell_dk_death_coil();
    new spell_dk_death_gate();
    new spell_dk_death_grip();
    new spell_dk_death_pact();
    new spell_dk_death_strike();
    new spell_dk_death_strike_enabler();
    new spell_dk_gorefiends_grasp();
    new spell_dk_ghoul_explode();
    new spell_dk_icebound_fortitude();
    new spell_dk_necrotic_strike();
    new spell_dk_raise_dead();
    new spell_dk_runic_corruption();
    new spell_dk_runic_empowerment();
    new spell_dk_rune_tap_party();
    new spell_dk_scent_of_blood();
    new spell_dk_scourge_strike(); // 5.4.8 18414
    new spell_dk_vampiric_blood();
    new spell_dk_blood_charges();
    new spell_dk_blood_tap();
    new spell_dk_rune_converter();
    new spell_dk_bone_shield_check();
    new spell_dk_bone_shield();
    new spell_dk_dancing_rune_weapon();
    new spell_dk_rune_weapon_scaling();
    new spell_dk_pillar_of_frost();
    new spell_dk_howling_blast();
    new spell_dk_sudden_doom();
    new spell_dk_shadow_infusion_proc();
    new spell_dk_shadow_infusion();
    new spell_dk_dark_transformation();
    new spell_dk_ghoul_ability();
    new spell_dk_festering_strike();
    new spell_dk_plague_strike();
    new spell_dk_chilblains();
    new spell_dk_unholy_blight();
    new spell_dk_pestilence();
    new spell_dk_blood_boil();
    new spell_dk_death_siphon();
    new spell_dk_remorseless_winter_targets();
    new spell_dk_remorseless_winter();
    new spell_dk_desecrated_ground();
    new spell_dk_plague_leech();
    new spell_dk_purgatory();
    new spell_dk_purgatory_absorb();
    new spell_dk_deaths_advance();
    new spell_dk_presence();
    new spell_dk_glyph_of_horn_of_winter();
    new spell_dk_glyph_of_the_loud_horn();
    new spell_dk_glyph_of_death_and_decay();
    new spell_dk_glyph_of_unholy_command();
    new spell_dk_glyph_of_strangulate();
    new spell_dk_asphyxiate();
    new spell_dk_glyph_of_the_skeleton();
    new spell_dk_glyph_of_the_geist();
    new spell_dk_will_of_the_necropolis();
    new spell_dk_will_of_the_necropolis_rune_tap();
    new spell_dk_crimson_scourge();
    new spell_dk_relentless_grip();
    new spell_dk_gargoyle_strike();
    new spell_dk_riposte();
    new spell_dk_might_of_ursoc();
    new spell_dk_wild_mushroom_plague_periodic();
    new spell_dk_wild_mushroom_plague_trigger();
    new spell_dk_frozen_power();
    new spell_dk_t15_dps_4p_bonus();
    new spell_dk_t15_blood_4p_bonus();
    new spell_dk_death_shroud_duration();
    new spell_dk_death_shroud();
    new spell_dk_t16_dps_4p_bonus();
    new spell_dk_t16_blood_2p_bonus();
    new spell_dk_t16_blood_4p_bonus();
    new spell_dk_soul_reaper();
    new spell_dk_soul_reaper_haste();
    new spell_dk_unholy_frenzy();
    new spell_dk_dark_simulacrum();
}
