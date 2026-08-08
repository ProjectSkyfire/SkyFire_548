/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/*
 * Scripts for spells with SPELLFAMILY_ROGUE and SPELLFAMILY_GENERIC spells used by rogue players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_rog_".
 */

#include "Player.h"
#include "Item.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "SpellMgr.h"
#include "ObjectAccessor.h"
#include "EventProcessor.h"
#include "Spell.h"
#include "Containers.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"
#include "InstanceScript.h"
#include "DynamicObject.h"
#include <set>

enum RogueSpells
{
    SPELL_ROGUE_ADRENALINE_RUSH                     = 13750,
    SPELL_ROGUE_BANDITS_GUILE_DAMAGE                = 84748,
    SPELL_ROGUE_BANDITS_GUILE_DEEP                  = 84747,
    SPELL_ROGUE_BANDITS_GUILE_MODERATE              = 84746,
    SPELL_ROGUE_BANDITS_GUILE_SHALLOW               = 84745,
    SPELL_ROGUE_BLADE_FLURRY                        = 13877,
    SPELL_ROGUE_BLADE_FLURRY_EXTRA_ATTACK           = 22482,
    SPELL_ROGUE_CHEAT_DEATH_COOLDOWN                = 31231,
    SPELL_ROGUE_COMBO_POINT                         = 139546,
    SPELL_ROGUE_CRIMSON_TEMPEST                     = 121411,
    SPELL_ROGUE_CRIMSON_TEMPEST_DOT                 = 122233,
    SPELL_ROGUE_CRIPPLING_POISON                    = 3409,
    SPELL_ROGUE_ENVENOM                             = 32645,
    SPELL_ROGUE_FAN_OF_KNIVES                       = 51723,
    SPELL_ROGUE_GARROTE                             = 703,
    SPELL_ROGUE_GLYPH_OF_HEMORRHAGE                 = 56807,
    SPELL_ROGUE_GLYPH_OF_HEMORRHAGING_VEINS         = 146631,
    SPELL_ROGUE_GLYPH_OF_KILLING_SPREE              = 63252,
    SPELL_ROGUE_HEMORRHAGE                          = 16511,
    SPELL_ROGUE_HEMORRHAGE_DOT                      = 89775,
    SPELL_ROGUE_HONOR_AMONG_THIEVES                 = 51699,
    SPELL_ROGUE_KIDNEY_SHOT                         = 408,
    SPELL_ROGUE_KILLING_SPREE                       = 51690,
    SPELL_ROGUE_KILLING_SPREE_DAMAGE                = 57841,
    SPELL_ROGUE_KILLING_SPREE_DAMAGE_AURA           = 61851,
    SPELL_ROGUE_KILLING_SPREE_TELEPORT              = 57840,
    SPELL_ROGUE_MAIN_GAUCHE                         = 86392,
    SPELL_ROGUE_MASTER_OF_SUBTLETY_DAMAGE_PERCENT   = 31665,
    SPELL_ROGUE_MASTER_OF_SUBTLETY_PASSIVE          = 31223,
    SPELL_ROGUE_MASTER_OF_SUBTLETY_PERIODIC         = 31666,
    SPELL_ROGUE_NIGHTSTALKER                        = 130493,
    SPELL_ROGUE_NIGHTSTALKER_TALENT                 = 14062,
    SPELL_ROGUE_REDIRECT                            = 73981,
    SPELL_ROGUE_REVEALED_WEAKNESS                   = 115238,
    SPELL_ROGUE_REVEALING_STRIKE                    = 84617,
    SPELL_ROGUE_RUPTURE                             = 1943,
    SPELL_ROGUE_SANGUINARY_VEIN                     = 79147,
    SPELL_ROGUE_SANGUINARY_VEIN_DEBUFF              = 124271,
    SPELL_ROGUE_SAP                                 = 6770,
    SPELL_ROGUE_SHADOW_BLADE_OFFHAND                = 121474,
    SPELL_ROGUE_SHADOW_BLADES                       = 121471,
    SPELL_ROGUE_SHADOW_FOCUS                        = 112942,
    SPELL_ROGUE_SHADOW_FOCUS_TALENT                 = 108209,
    SPELL_ROGUE_SHADOW_SIGHT                        = 34709,
    SPELL_ROGUE_SHADOWSTEP                          = 36554,
    SPELL_ROGUE_SINISTER_STRIKE                     = 1752,
    SPELL_ROGUE_SLICE_AND_DICE                      = 5171,
    SPELL_ROGUE_SPRINT                              = 2983,
    SPELL_ROGUE_STEALTH                             = 1784,
    SPELL_ROGUE_SUBTERFUGE                          = 115192,
    SPELL_ROGUE_SUBTERFUGE_STEALTH                  = 115191,
    SPELL_ROGUE_SUBTERFUGE_TALENT                   = 108208,
    SPELL_ROGUE_SUBTERFUGE_VANISH                   = 115193,
    SPELL_ROGUE_THROW                               = 121733,
    SPELL_ROGUE_TRICKS_OF_THE_TRADE_DMG_BOOST       = 57933,
    SPELL_ROGUE_TRICKS_OF_THE_TRADE_PROC            = 59628,
    SPELL_ROGUE_VANISH                              = 1856,
    SPELL_ROGUE_VANISH_AURA                         = 11327,
    SPELL_ROGUE_VENOMOUS_VIM                        = 51637,
    SPELL_ROGUE_VENOMOUS_WOUND                      = 79136,
    SPELL_ROGUE_VENOMOUS_WOUNDS                     = 79134,
    SPELL_ROGUE_CHEAP_SHOT                          = 1833,
    SPELL_ROGUE_AMBUSH                              = 8676,
    SPELL_ROGUE_CLOAK_AND_DAGGER                    = 138106,
    SPELL_ROGUE_CLOAK_AND_DAGGER_TELEPORT           = 132987,
    SPELL_ROGUE_DEADLY_BREW                         = 51626,
    SPELL_ROGUE_DIRTY_TRICKS                        = 108216,
    SPELL_ROGUE_DISMANTLE                           = 51722,
    SPELL_ROGUE_EVASION                             = 5277,
    SPELL_ROGUE_GLYPH_OF_BLIND                      = 91299,
    SPELL_ROGUE_MASTER_POISONER                     = 58410,
    SPELL_ROGUE_MASTER_POISONER_DEBUFF              = 93068,
    SPELL_ROGUE_NERVE_STRIKE                        = 108210,
    SPELL_ROGUE_NERVE_STRIKE_EFFECT                 = 112947,
    SPELL_ROGUE_PREPARATION                         = 14185,
    SPELL_ROGUE_RELENTLESS_STRIKES_ENERGIZE         = 98440,
    SPELL_ROGUE_SMOKE_BOMB_AURA                     = 76577,
    SPELL_ROGUE_SMOKE_BOMB_INTERFERE                = 88611,
    SPELL_ROGUE_WOUND_POISON                        = 8680,
};

namespace RogueStealthHelpers
{
    inline void HandleStealthApply(Unit* rogue)
    {
        if (!rogue)
            return;

        if (AuraEffect const* aurEff = rogue->GetAuraEffect(SPELL_ROGUE_MASTER_OF_SUBTLETY_PASSIVE, EFFECT_0))
        {
            int32 basepoints0 = aurEff->GetAmount();
            rogue->CastCustomSpell(rogue, SPELL_ROGUE_MASTER_OF_SUBTLETY_DAMAGE_PERCENT, &basepoints0, NULL, NULL, true);
        }

        if (rogue->HasAura(SPELL_ROGUE_NIGHTSTALKER_TALENT))
            rogue->CastSpell(rogue, SPELL_ROGUE_NIGHTSTALKER, true);

        if (rogue->HasAura(SPELL_ROGUE_SHADOW_FOCUS_TALENT))
            rogue->CastSpell(rogue, SPELL_ROGUE_SHADOW_FOCUS, true);
    }

    inline void HandleStealthRemove(Unit* rogue)
    {
        if (!rogue)
            return;

        if (rogue->HasAura(SPELL_ROGUE_MASTER_OF_SUBTLETY_PASSIVE))
            rogue->CastSpell(rogue, SPELL_ROGUE_MASTER_OF_SUBTLETY_PERIODIC, true);

        // Keep Nightstalker briefly so Ambush/Garrote still see the damage bonus
        if (Aura* nightstalker = rogue->GetAura(SPELL_ROGUE_NIGHTSTALKER))
            nightstalker->SetDuration(100);

        rogue->RemoveAurasDueToSpell(SPELL_ROGUE_SHADOW_FOCUS);
    }
}

// 139569 - Combo Point Delayed (Ruthlessness / similar)
class DelayedRogueComboPointEvent : public BasicEvent
{
public:
    DelayedRogueComboPointEvent(uint64 playerGuid, uint64 targetGuid)
        : _playerGuid(playerGuid), _targetGuid(targetGuid) { }

    bool Execute(uint64 /*e_time*/, uint32 /*p_time*/) OVERRIDE
    {
        if (Player* player = ObjectAccessor::FindPlayer(_playerGuid))
            if (Unit* target = ObjectAccessor::GetUnit(*player, _targetGuid))
                player->CastSpell(target, SPELL_ROGUE_COMBO_POINT, true);
        return true;
    }

private:
    uint64 _playerGuid;
    uint64 _targetGuid;
};

class DelayedHonorAmongThievesEvent : public BasicEvent
{
public:
    DelayedHonorAmongThievesEvent(uint64 playerGuid, uint64 targetGuid)
        : _playerGuid(playerGuid), _targetGuid(targetGuid) { }

    bool Execute(uint64 /*e_time*/, uint32 /*p_time*/) OVERRIDE
    {
        Player* player = ObjectAccessor::FindPlayer(_playerGuid);
        if (!player || !player->IsInCombat())
            return true;

        // Apply ICD after the cast. Adding it beforehand makes CheckCast fail with
        // SPELL_FAILED_NOT_READY (triggered casts still honor player spell cooldowns).
        if (Unit* target = ObjectAccessor::GetUnit(*player, _targetGuid))
        {
            player->CastSpell(target, SPELL_ROGUE_HONOR_AMONG_THIEVES, true);
            player->AddSpellCooldown(SPELL_ROGUE_HONOR_AMONG_THIEVES, 0, time(NULL) + 2);
        }
        return true;
    }

private:
    uint64 _playerGuid;
    uint64 _targetGuid;
};

enum RogueSpellIcons
{
    ICON_ROGUE_IMPROVED_RECUPERATE                  = 4819
};

// 13877, 33735, (check 51211, 65956) - Blade Flurry
class spell_rog_blade_flurry : public SpellScriptLoader
{
public:
    spell_rog_blade_flurry() : SpellScriptLoader("spell_rog_blade_flurry") { }

    class spell_rog_blade_flurry_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_blade_flurry_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_ROGUE_BLADE_FLURRY_EXTRA_ATTACK))
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
            _procTarget = GetTarget()->SelectNearbyTarget(eventInfo.GetProcTarget());
            return _procTarget && eventInfo.GetDamageInfo();
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            SF_LOG_ERROR("misc", "damage: %u procSpell: %u",
                eventInfo.GetDamageInfo()->GetDamage(), eventInfo.GetDamageInfo()->GetSpellInfo() ? eventInfo.GetDamageInfo()->GetSpellInfo()->Id : 0);

            GetTarget()->CastCustomSpell(SPELL_ROGUE_BLADE_FLURRY_EXTRA_ATTACK, SPELLVALUE_BASE_POINT0, eventInfo.GetDamageInfo()->GetDamage(), _procTarget, true, NULL, aurEff);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_rog_blade_flurry_AuraScript::CheckProc);
            if (m_scriptSpellId == SPELL_ROGUE_BLADE_FLURRY)
                OnEffectProc += AuraEffectProcFn(spell_rog_blade_flurry_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_MOD_POWER_REGEN_PERCENT);
            else
                OnEffectProc += AuraEffectProcFn(spell_rog_blade_flurry_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_MOD_MELEE_HASTE);
        }

    private:
        Unit* _procTarget;
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_rog_blade_flurry_AuraScript();
    }
};

// 31228 - Cheat Death
class spell_rog_cheat_death : public SpellScriptLoader
{
public:
    spell_rog_cheat_death() : SpellScriptLoader("spell_rog_cheat_death") { }

    class spell_rog_cheat_death_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_cheat_death_AuraScript);

        uint32 absorbChance;

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_ROGUE_CHEAT_DEATH_COOLDOWN))
                return false;
            return true;
        }

        bool Load() OVERRIDE
        {
            absorbChance = GetSpellInfo()->Effects[EFFECT_0].CalcValue();
            return GetUnitOwner()->ToPlayer();
        }

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            // Set absorbtion amount to unlimited
            amount = -1;
        }

        void Absorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
        {
            Player* target = GetTarget()->ToPlayer();
            if (dmgInfo.GetDamage() < target->GetHealth() || target->HasSpellCooldown(SPELL_ROGUE_CHEAT_DEATH_COOLDOWN) || !roll_chance_i(absorbChance))
                return;

            target->CastSpell(target, SPELL_ROGUE_CHEAT_DEATH_COOLDOWN, true);
            target->AddSpellCooldown(SPELL_ROGUE_CHEAT_DEATH_COOLDOWN, 0, time(NULL) + 60);

            uint32 health10 = target->CountPctFromMaxHealth(10);

            // hp > 10% - absorb hp till 10%
            if (target->GetHealth() > health10)
                absorbAmount = dmgInfo.GetDamage() - target->GetHealth() + health10;
            // hp lower than 10% - absorb everything
            else
                absorbAmount = dmgInfo.GetDamage();
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_rog_cheat_death_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            OnEffectAbsorb += AuraEffectAbsorbFn(spell_rog_cheat_death_AuraScript::Absorb, EFFECT_0);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_rog_cheat_death_AuraScript();
    }
};

// -51625 - Deadly Brew
class spell_rog_crippling_poison : public SpellScriptLoader
{
public:
    spell_rog_crippling_poison() : SpellScriptLoader("spell_rog_crippling_poison") { }

    class spell_rog_crippling_poison_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_crippling_poison_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_ROGUE_CRIPPLING_POISON))
                return false;
            return true;
        }

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            GetTarget()->CastSpell(eventInfo.GetProcTarget(), SPELL_ROGUE_CRIPPLING_POISON, true, NULL, aurEff);
        }

        void Register() OVERRIDE
        {
            OnEffectProc += AuraEffectProcFn(spell_rog_crippling_poison_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_rog_crippling_poison_AuraScript();
    }
};

// -51664 - Cut to the Chase
class spell_rog_cut_to_the_chase : public SpellScriptLoader
{
public:
    spell_rog_cut_to_the_chase() : SpellScriptLoader("spell_rog_cut_to_the_chase") { }

    class spell_rog_cut_to_the_chase_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_cut_to_the_chase_AuraScript);

        void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            // Refresh Slice and Dice to its 5 combo point maximum (36 seconds).
            if (Aura* sliceAndDice = GetTarget()->GetAura(SPELL_ROGUE_SLICE_AND_DICE))
            {
                int32 duration = 36 * IN_MILLISECONDS;
                sliceAndDice->SetMaxDuration(duration);
                sliceAndDice->SetDuration(duration);
            }
        }

        void Register() OVERRIDE
        {
            OnEffectProc += AuraEffectProcFn(spell_rog_cut_to_the_chase_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_rog_cut_to_the_chase_AuraScript();
    }
};

// 2818 - Deadly Poison
class spell_rog_deadly_poison : public SpellScriptLoader
{
public:
    spell_rog_deadly_poison() : SpellScriptLoader("spell_rog_deadly_poison") { }

    class spell_rog_deadly_poison_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_deadly_poison_SpellScript);

        bool Load() OVERRIDE
        {
            _stackAmount = 0;
            // at this point CastItem must already be initialized
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER && GetCastItem();
        }

        void HandleBeforeHit()
        {
            if (Unit* target = GetHitUnit())
                // Deadly Poison
                if (AuraEffect const* aurEff = target->GetAuraEffect(SPELL_AURA_PERIODIC_DAMAGE, SPELLFAMILY_ROGUE, 0x10000, 0x80000, 0, GetCaster()->GetGUID()))
                    _stackAmount = aurEff->GetBase()->GetStackAmount();
        }

        void HandleAfterHit()
        {
            if (_stackAmount < 5)
                return;

            Player* player = GetCaster()->ToPlayer();

            if (Unit* target = GetHitUnit())
            {
                Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);

                if (item == GetCastItem())
                    item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                if (!item)
                    return;

                // item combat enchantments
                for (uint8 slot = 0; slot < MAX_ENCHANTMENT_SLOT; ++slot)
                {
                    if (slot > PRISMATIC_ENCHANTMENT_SLOT&& slot < PROP_ENCHANTMENT_SLOT_0)    // not holding enchantment id
                        continue;

                    SpellItemEnchantmentEntry const* enchant = sSpellItemEnchantmentStore.LookupEntry(item->GetEnchantmentId(EnchantmentSlot(slot)));
                    if (!enchant)
                        continue;

                    for (uint8 s = 0; s < 3; ++s)
                    {
                        if (enchant->type[s] != ITEM_ENCHANTMENT_TYPE_COMBAT_SPELL)
                            continue;

                        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(enchant->spellid[s]);
                        if (!spellInfo)
                        {
                            SF_LOG_ERROR("spells", "Player::CastItemCombatSpell Enchant %i, player (Name: %s, GUID: %u) cast unknown spell %i", enchant->ID, player->GetName().c_str(), player->GetGUIDLow(), enchant->spellid[s]);
                            continue;
                        }

                        // Proc only rogue poisons
                        if (spellInfo->SpellFamilyName != SPELLFAMILY_ROGUE || spellInfo->Dispel != DISPEL_POISON)
                            continue;

                        // Do not reproc deadly
                        if (spellInfo->SpellFamilyFlags.IsEqual(0x10000, 0x80000, 0))
                            continue;

                        if (spellInfo->IsPositive())
                            player->CastSpell(player, enchant->spellid[s], true, item);
                        else
                            player->CastSpell(target, enchant->spellid[s], true, item);
                    }
                }
            }
        }

        void Register() OVERRIDE
        {
            BeforeHit += SpellHitFn(spell_rog_deadly_poison_SpellScript::HandleBeforeHit);
            AfterHit += SpellHitFn(spell_rog_deadly_poison_SpellScript::HandleAfterHit);
        }

        uint8 _stackAmount;
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_rog_deadly_poison_SpellScript();
    }
};

// 31666 - Master of Subtlety
class spell_rog_master_of_subtlety : public SpellScriptLoader
{
public:
    spell_rog_master_of_subtlety() : SpellScriptLoader("spell_rog_master_of_subtlety") { }

    class spell_rog_master_of_subtlety_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_master_of_subtlety_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_ROGUE_MASTER_OF_SUBTLETY_DAMAGE_PERCENT))
                return false;
            return true;
        }

        void HandleEffectPeriodic(AuraEffect const* /*aurEff*/)
        {
            Unit* target = GetTarget();

            if (!target->HasAuraType(SPELL_AURA_MOD_STEALTH))
                target->RemoveAurasDueToSpell(SPELL_ROGUE_MASTER_OF_SUBTLETY_DAMAGE_PERCENT);
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_rog_master_of_subtlety_AuraScript::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_rog_master_of_subtlety_AuraScript();
    }
};

// 73651 - Recuperate
class spell_rog_recuperate : public SpellScriptLoader
{
public:
    spell_rog_recuperate() : SpellScriptLoader("spell_rog_recuperate") { }

    class spell_rog_recuperate_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_recuperate_AuraScript);

        bool Load() OVERRIDE
        {
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void OnPeriodic(AuraEffect const* /*aurEff*/)
        {
            if (Unit* caster = GetCaster())
                if (AuraEffect* effect = GetAura()->GetEffect(EFFECT_0))
                    effect->RecalculateAmount(caster);
        }

        void CalculateBonus(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
        {
            canBeRecalculated = false;
            if (Unit* caster = GetCaster())
            {
                int32 baseAmount = GetSpellInfo()->Effects[EFFECT_0].CalcValue(caster) * 1000;
                // Improved Recuperate
                if (AuraEffect const* auraEffect = caster->GetDummyAuraEffect(SPELLFAMILY_ROGUE, ICON_ROGUE_IMPROVED_RECUPERATE, EFFECT_0))
                    baseAmount += auraEffect->GetAmount();

                amount = CalculatePct(caster->GetMaxHealth(), float(baseAmount) / 1000.0f);
            }
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_rog_recuperate_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_rog_recuperate_AuraScript::CalculateBonus, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_rog_recuperate_AuraScript();
    }
};

// 1943 - Rupture
class spell_rog_rupture : public SpellScriptLoader
{
public:
    spell_rog_rupture() : SpellScriptLoader("spell_rog_rupture") { }

    class spell_rog_rupture_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_rupture_AuraScript);

        bool Load() OVERRIDE
        {
            Unit* caster = GetCaster();
            return caster && caster->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
        {
            if (Unit* caster = GetCaster())
            {
                canBeRecalculated = false;

                float const attackpowerPerCombo[6] =
                {
                    0.0f,
                    0.015f,         // 1 point:  ${($m1 + $b1*1 + 0.015 * $AP) * 4} damage over 8 secs
                    0.024f,         // 2 points: ${($m1 + $b1*2 + 0.024 * $AP) * 5} damage over 10 secs
                    0.03f,          // 3 points: ${($m1 + $b1*3 + 0.03 * $AP) * 6} damage over 12 secs
                    0.03428571f,    // 4 points: ${($m1 + $b1*4 + 0.03428571 * $AP) * 7} damage over 14 secs
                    0.0375f         // 5 points: ${($m1 + $b1*5 + 0.0375 * $AP) * 8} damage over 16 secs
                };

                uint8 cp = caster->ToPlayer()->GetComboPoints();
                if (cp > 5)
                    cp = 5;

                amount += int32(caster->GetTotalAttackPowerValue(WeaponAttackType::BASE_ATTACK) * attackpowerPerCombo[cp]);
            }
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster || !caster->HasAura(SPELL_ROGUE_VENOMOUS_WOUNDS))
                return;

            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_DEATH)
                return;

            // 5 Energy per remaining second of Rupture
            int32 energy = 5 * GetAura()->GetDuration() / IN_MILLISECONDS;
            if (energy > 0)
                caster->CastCustomSpell(SPELL_ROGUE_VENOMOUS_VIM, SPELLVALUE_BASE_POINT0, energy, caster, true);
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_rog_rupture_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            OnEffectRemove += AuraEffectRemoveFn(spell_rog_rupture_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_rog_rupture_AuraScript();
    }
};

// 1784 - Stealth
class spell_rog_stealth : public SpellScriptLoader
{
public:
    spell_rog_stealth() : SpellScriptLoader("spell_rog_stealth") { }

    class spell_rog_stealth_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_stealth_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_ROGUE_MASTER_OF_SUBTLETY_PASSIVE) ||
                !sSpellMgr->GetSpellInfo(SPELL_ROGUE_MASTER_OF_SUBTLETY_DAMAGE_PERCENT) ||
                !sSpellMgr->GetSpellInfo(SPELL_ROGUE_MASTER_OF_SUBTLETY_PERIODIC) ||
                !sSpellMgr->GetSpellInfo(SPELL_ROGUE_NIGHTSTALKER))
                return false;
            return true;
        }

        void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            RogueStealthHelpers::HandleStealthApply(GetTarget());
            GetTarget()->ForceValuesUpdateAtIndex(UNIT_FIELD_SHAPESHIFT_FORM);
        }

        void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            RogueStealthHelpers::HandleStealthRemove(GetTarget());
            GetTarget()->ForceValuesUpdateAtIndex(UNIT_FIELD_SHAPESHIFT_FORM);
        }

        void Register() OVERRIDE
        {
            AfterEffectApply += AuraEffectApplyFn(spell_rog_stealth_AuraScript::HandleEffectApply, EFFECT_1, SPELL_AURA_MOD_STEALTH, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_rog_stealth_AuraScript::HandleEffectRemove, EFFECT_1, SPELL_AURA_MOD_STEALTH, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_rog_stealth_AuraScript();
    }
};

// Shared Subterfuge break logic for 115191 / 115193
class spell_rog_subterfuge_break_AuraScript : public AuraScript
{
protected:
    bool _breakImmediately;

    spell_rog_subterfuge_break_AuraScript() : _breakImmediately(false) { }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        _breakImmediately = false;

        SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
        if (!spellInfo)
            return !GetTarget()->HasAura(SPELL_ROGUE_SUBTERFUGE);

        Spell const* procSpell = eventInfo.GetSpell();

        // Food / drink from consumable items should drop Subterfuge stealth immediately
        if (procSpell && procSpell->m_CastItem)
        {
            ItemTemplate const* proto = procSpell->m_CastItem->GetTemplate();
            if (proto->Class == ITEM_CLASS_CONSUMABLE && proto->SubClass == ITEM_SUBCLASS_FOOD_DRINK)
            {
                _breakImmediately = true;
                return true;
            }
        }

        if (eventInfo.GetTypeMask() & TAKEN_HIT_PROC_FLAG_MASK)
        {
            // Vanish (115193) tolerates incoming damage without breaking into Subterfuge
            if (GetId() == SPELL_ROGUE_SUBTERFUGE_VANISH)
                return false;

            DamageInfo* damageInfo = eventInfo.GetDamageInfo();
            if (damageInfo && !damageInfo->GetDamage() && eventInfo.GetActor()
                && !eventInfo.GetActor()->IsValidAttackTarget(GetTarget()))
                return false;

            if (spellInfo->Id == SPELL_ROGUE_SAP)
                _breakImmediately = true;

            if (spellInfo->GetAllEffectsMechanicMask() & ((1 << MECHANIC_DISORIENTED) | (1 << MECHANIC_FREEZE)))
                _breakImmediately = true;

            if (_breakImmediately)
                return true;
        }
        else if (eventInfo.GetTypeMask() & DONE_HIT_PROC_FLAG_MASK)
        {
            if (spellInfo->AttributesEx & SPELL_ATTR1_NOT_BREAK_STEALTH)
                return false;

            if (spellInfo->IsPositive())
            {
                if (procSpell && procSpell->IsTriggered())
                    return false;
                if (spellInfo->SpellFamilyName != SPELLFAMILY_ROGUE)
                    return false;
            }
            else if (procSpell)
            {
                if (procSpell->IsTriggered() && spellInfo->SpellFamilyName != SPELLFAMILY_ROGUE)
                    return false;
            }

            if (spellInfo->Id == GetId())
                return false;
        }

        return !GetTarget()->HasAura(SPELL_ROGUE_SUBTERFUGE);
    }

    void HandleProc(ProcEventInfo& /*eventInfo*/)
    {
        if (!_breakImmediately)
        {
            PreventDefaultAction();
            GetTarget()->CastSpell(GetTarget(), SPELL_ROGUE_SUBTERFUGE, true);
        }
    }
};

// 115191 - Stealth (Subterfuge)
class spell_rog_stealth_subterfuge : public SpellScriptLoader
{
public:
    spell_rog_stealth_subterfuge() : SpellScriptLoader("spell_rog_stealth_subterfuge") { }

    class spell_rog_stealth_subterfuge_AuraScript : public spell_rog_subterfuge_break_AuraScript
    {
        PrepareAuraScript(spell_rog_stealth_subterfuge_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_ROGUE_SUBTERFUGE) ||
                !sSpellMgr->GetSpellInfo(SPELL_ROGUE_NIGHTSTALKER))
                return false;
            return true;
        }

        void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            RogueStealthHelpers::HandleStealthApply(GetTarget());
            GetTarget()->ForceValuesUpdateAtIndex(UNIT_FIELD_SHAPESHIFT_FORM);
        }

        void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            RogueStealthHelpers::HandleStealthRemove(GetTarget());
            GetTarget()->ForceValuesUpdateAtIndex(UNIT_FIELD_SHAPESHIFT_FORM);
        }

        void Register() OVERRIDE
        {
            AfterEffectApply += AuraEffectApplyFn(spell_rog_stealth_subterfuge_AuraScript::HandleEffectApply, EFFECT_1, SPELL_AURA_MOD_STEALTH, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_rog_stealth_subterfuge_AuraScript::HandleEffectRemove, EFFECT_1, SPELL_AURA_MOD_STEALTH, AURA_EFFECT_HANDLE_REAL);
            DoCheckProc += AuraCheckProcFn(spell_rog_stealth_subterfuge_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_rog_stealth_subterfuge_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_rog_stealth_subterfuge_AuraScript();
    }
};

// 11327 / 115193 - Vanish
class spell_rog_vanish : public SpellScriptLoader
{
public:
    spell_rog_vanish() : SpellScriptLoader("spell_rog_vanish") { }

    class spell_rog_vanish_AuraScript : public spell_rog_subterfuge_break_AuraScript
    {
        PrepareAuraScript(spell_rog_vanish_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_ROGUE_STEALTH) ||
                !sSpellMgr->GetSpellInfo(SPELL_ROGUE_SUBTERFUGE_STEALTH) ||
                !sSpellMgr->GetSpellInfo(SPELL_ROGUE_SUBTERFUGE) ||
                !sSpellMgr->GetSpellInfo(SPELL_ROGUE_NIGHTSTALKER))
                return false;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
                if (spellInfo->HasAura(SPELL_AURA_MOD_STEALTH))
                    return false;

            return spell_rog_subterfuge_break_AuraScript::CheckProc(eventInfo);
        }

        void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            RogueStealthHelpers::HandleStealthApply(GetTarget());
            GetTarget()->ForceValuesUpdateAtIndex(UNIT_FIELD_SHAPESHIFT_FORM);
        }

        void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* owner = GetTarget();
            if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_EXPIRE)
            {
                uint32 spellId = owner->HasSpell(SPELL_ROGUE_SUBTERFUGE_TALENT)
                    ? SPELL_ROGUE_SUBTERFUGE_STEALTH
                    : SPELL_ROGUE_STEALTH;
                if (!owner->HasAura(spellId))
                    owner->CastSpell(owner, spellId, true);
            }
            else
            {
                RogueStealthHelpers::HandleStealthRemove(owner);
                owner->ForceValuesUpdateAtIndex(UNIT_FIELD_SHAPESHIFT_FORM);
            }
        }

        void Register() OVERRIDE
        {
            AfterEffectApply += AuraEffectApplyFn(spell_rog_vanish_AuraScript::HandleEffectApply, EFFECT_1, SPELL_AURA_MOD_STEALTH, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_rog_vanish_AuraScript::HandleEffectRemove, EFFECT_1, SPELL_AURA_MOD_STEALTH, AURA_EFFECT_HANDLE_REAL);
            DoCheckProc += AuraCheckProcFn(spell_rog_vanish_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_rog_vanish_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_rog_vanish_AuraScript();
    }
};

// 1856 - Vanish (cast)
class spell_rog_vanish_initial : public SpellScriptLoader
{
public:
    spell_rog_vanish_initial() : SpellScriptLoader("spell_rog_vanish_initial") { }

    class spell_rog_vanish_initial_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_vanish_initial_SpellScript);

        SpellCastResult CheckCast()
        {
            if (GetCaster()->HasAura(SPELL_ROGUE_SHADOW_SIGHT))
                return SpellCastResult::SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_rog_vanish_initial_SpellScript::CheckCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_rog_vanish_initial_SpellScript();
    }
};

// 121411 - Crimson Tempest
class spell_rog_crimson_tempest : public SpellScriptLoader
{
public:
    spell_rog_crimson_tempest() : SpellScriptLoader("spell_rog_crimson_tempest") { }

    class spell_rog_crimson_tempest_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_crimson_tempest_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_ROGUE_CRIMSON_TEMPEST_DOT))
                return false;
            return true;
        }

        void HandleHit()
        {
            Player* rogue = GetCaster()->ToPlayer();
            Unit* target = GetHitUnit();
            if (!rogue || !target)
                return;

            // Tooltip: 240% of the direct damage as a 6-tick bleed
            int32 damage = CalculatePct(GetHitDamage(), 240) / 6;
            if (damage <= 0)
                return;

            Unit::AuraEffectList const& bleedTaken = target->GetAuraEffectsByType(SPELL_AURA_MOD_MECHANIC_DAMAGE_TAKEN_PERCENT);
            for (Unit::AuraEffectList::const_iterator itr = bleedTaken.begin(); itr != bleedTaken.end(); ++itr)
                if ((1 << (*itr)->GetMiscValue()) & (1 << MECHANIC_BLEED))
                    AddPct(damage, (*itr)->GetAmount());

            AddPct(damage, rogue->GetTotalAuraModifierByMiscValue(SPELL_AURA_MOD_DAMAGE_DONE_FOR_MECHANIC, MECHANIC_BLEED));

            rogue->CastCustomSpell(SPELL_ROGUE_CRIMSON_TEMPEST_DOT, SPELLVALUE_BASE_POINT0, damage, target, true);
        }

        void Register() OVERRIDE
        {
            AfterHit += SpellHitFn(spell_rog_crimson_tempest_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_rog_crimson_tempest_SpellScript();
    }
};

// 51723 - Fan of Knives
class spell_rog_fan_of_knives : public SpellScriptLoader
{
public:
    spell_rog_fan_of_knives() : SpellScriptLoader("spell_rog_fan_of_knives") { }

    class spell_rog_fan_of_knives_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_fan_of_knives_SpellScript);

        void FilterComboTargets(std::list<WorldObject*>& targets)
        {
            if (targets.empty())
                return;

            Unit* preferred = NULL;
            if (Player* rogue = GetCaster()->ToPlayer())
            {
                if (uint64 comboGuid = rogue->GetComboTarget())
                    preferred = ObjectAccessor::GetUnit(*rogue, comboGuid);

                if (!preferred)
                {
                    if (Unit* selected = ObjectAccessor::GetUnit(*rogue, rogue->GetTarget()))
                        if (rogue->IsValidAttackTarget(selected))
                            preferred = selected;
                }
            }

            targets.clear();
            if (preferred)
                targets.push_back(preferred);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_rog_fan_of_knives_SpellScript::FilterComboTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_rog_fan_of_knives_SpellScript();
    }
};

// 51723 - Fan of Knives, 121411 - Crimson Tempest
class spell_rog_subterfuge_cast_trigger : public SpellScriptLoader
{
public:
    spell_rog_subterfuge_cast_trigger() : SpellScriptLoader("spell_rog_subterfuge_cast_trigger") { }

    class spell_rog_subterfuge_cast_trigger_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_subterfuge_cast_trigger_SpellScript);

        void ActivateSubterfuge()
        {
            Unit* rogue = GetCaster();
            if (rogue->HasAura(SPELL_ROGUE_SUBTERFUGE_STEALTH) || rogue->HasAura(SPELL_ROGUE_SUBTERFUGE_VANISH))
                if (!rogue->HasAura(SPELL_ROGUE_SUBTERFUGE))
                    rogue->CastSpell(rogue, SPELL_ROGUE_SUBTERFUGE, true);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_rog_subterfuge_cast_trigger_SpellScript::ActivateSubterfuge);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_rog_subterfuge_cast_trigger_SpellScript();
    }
};

// 57934 - Tricks of the Trade
class spell_rog_tricks_of_the_trade : public SpellScriptLoader
{
public:
    spell_rog_tricks_of_the_trade() : SpellScriptLoader("spell_rog_tricks_of_the_trade") { }

    class spell_rog_tricks_of_the_trade_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_tricks_of_the_trade_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_ROGUE_TRICKS_OF_THE_TRADE_DMG_BOOST))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_ROGUE_TRICKS_OF_THE_TRADE_PROC))
                return false;
            return true;
        }

        bool Load() OVERRIDE
        {
            _redirectTarget = NULL;
            return true;
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_DEFAULT)
                GetTarget()->ResetRedirectThreat();
        }

        bool CheckProc(ProcEventInfo& /*eventInfo*/)
        {
            _redirectTarget = GetTarget()->GetRedirectThreatTarget();
            return _redirectTarget;
        }

        void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();

            Unit* target = GetTarget();
            target->CastSpell(_redirectTarget, SPELL_ROGUE_TRICKS_OF_THE_TRADE_DMG_BOOST, true);
            target->CastSpell(target, SPELL_ROGUE_TRICKS_OF_THE_TRADE_PROC, true);
            Remove(AURA_REMOVE_BY_DEFAULT); // maybe handle by proc charges
        }

        void Register() OVERRIDE
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_rog_tricks_of_the_trade_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            DoCheckProc += AuraCheckProcFn(spell_rog_tricks_of_the_trade_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_rog_tricks_of_the_trade_AuraScript::HandleProc, EFFECT_1, SPELL_AURA_DUMMY);
        }

    private:
        Unit* _redirectTarget;
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_rog_tricks_of_the_trade_AuraScript();
    }
};

// 59628 - Tricks of the Trade (Proc)
class spell_rog_tricks_of_the_trade_proc : public SpellScriptLoader
{
public:
    spell_rog_tricks_of_the_trade_proc() : SpellScriptLoader("spell_rog_tricks_of_the_trade_proc") { }

    class spell_rog_tricks_of_the_trade_proc_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_tricks_of_the_trade_proc_AuraScript);

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->ResetRedirectThreat();
        }

        void Register() OVERRIDE
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_rog_tricks_of_the_trade_proc_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_rog_tricks_of_the_trade_proc_AuraScript();
    }
};

// 84654 - Bandit's Guile
class spell_rog_bandits_guile : public SpellScriptLoader
{
public:
    spell_rog_bandits_guile() : SpellScriptLoader("spell_rog_bandits_guile") { }

    class spell_rog_bandits_guile_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_bandits_guile_AuraScript);

        bool Load() OVERRIDE
        {
            _insightCounter = 0;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            if (!spellInfo)
                return false;

            if (spellInfo->Id != SPELL_ROGUE_REVEALING_STRIKE && spellInfo->Id != SPELL_ROGUE_SINISTER_STRIKE)
                return false;

            return !GetUnitOwner()->HasAura(SPELL_ROGUE_BANDITS_GUILE_DEEP);
        }

        void HandleProc(ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            ++_insightCounter;

            Unit* rogue = GetUnitOwner();
            uint32 level = 0;
            if (rogue->HasAura(SPELL_ROGUE_BANDITS_GUILE_SHALLOW))
                level = 1;
            else if (rogue->HasAura(SPELL_ROGUE_BANDITS_GUILE_MODERATE))
                level = 2;

            if (_insightCounter == 4)
            {
                _insightCounter = 0;

                if (level)
                    rogue->RemoveAurasDueToSpell(SPELL_ROGUE_BANDITS_GUILE_SHALLOW + level - 1);

                rogue->CastSpell(rogue, SPELL_ROGUE_BANDITS_GUILE_SHALLOW + level, true);
                rogue->CastCustomSpell(SPELL_ROGUE_BANDITS_GUILE_DAMAGE, SPELLVALUE_BASE_POINT0, int32((level + 1) * 10), rogue, true);
            }
            else if (level)
            {
                rogue->CastSpell(rogue, SPELL_ROGUE_BANDITS_GUILE_SHALLOW + level - 1, true);
                rogue->CastCustomSpell(SPELL_ROGUE_BANDITS_GUILE_DAMAGE, SPELLVALUE_BASE_POINT0, int32(level * 10), rogue, true);
            }
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_rog_bandits_guile_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_rog_bandits_guile_AuraScript::HandleProc);
        }

    private:
        uint32 _insightCounter;
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_rog_bandits_guile_AuraScript();
    }
};

// 35551 - Combat Potency
class spell_rog_combat_potency : public SpellScriptLoader
{
public:
    spell_rog_combat_potency() : SpellScriptLoader("spell_rog_combat_potency") { }

    class spell_rog_combat_potency_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_combat_potency_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            Player* player = eventInfo.GetActor() ? eventInfo.GetActor()->ToPlayer() : NULL;
            if (!player)
                return false;

            if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
            {
                if (spellInfo->Id != SPELL_ROGUE_MAIN_GAUCHE && spellInfo->Id != SPELL_ROGUE_SHADOW_BLADE_OFFHAND)
                    return false;
            }
            else if (!(eventInfo.GetTypeMask() & PROC_FLAG_DONE_OFFHAND_ATTACK))
                return false;

            float offHandSpeed = player->GetAttackTime(WeaponAttackType::OFF_ATTACK) / float(IN_MILLISECONDS);
            if (Item* offItem = player->GetWeaponForAttack(WeaponAttackType::OFF_ATTACK))
                if (ItemTemplate const* proto = offItem->GetTemplate())
                    offHandSpeed = float(proto->Delay) / float(IN_MILLISECONDS);

            // ~20% at 1.4s OH weapon speed; slower weapons scale up proportionally
            return roll_chance_f(20.0f * offHandSpeed / 1.4f);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_rog_combat_potency_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_rog_combat_potency_AuraScript();
    }
};

// 139569 - Combo Point Delayed (Ruthlessness)
class spell_rog_combo_point_delayed : public SpellScriptLoader
{
public:
    spell_rog_combo_point_delayed() : SpellScriptLoader("spell_rog_combo_point_delayed") { }

    class spell_rog_combo_point_delayed_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_combo_point_delayed_SpellScript);

        void HandleHit()
        {
            Player* caster = GetCaster() ? GetCaster()->ToPlayer() : NULL;
            if (!caster)
                return;

            Unit* target = GetHitUnit();
            if (!target && caster->GetComboTarget())
                target = ObjectAccessor::GetUnit(*caster, caster->GetComboTarget());

            if (!target)
                return;

            caster->m_Events.AddEvent(new DelayedRogueComboPointEvent(caster->GetGUID(), target->GetGUID()),
                caster->m_Events.CalculateTime(1));
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_rog_combo_point_delayed_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_rog_combo_point_delayed_SpellScript();
    }
};

// 79096 - Restless Blades
class spell_rog_restless_blades : public SpellScriptLoader
{
public:
    spell_rog_restless_blades() : SpellScriptLoader("spell_rog_restless_blades") { }

    class spell_rog_restless_blades_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_restless_blades_AuraScript);

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            Player* rogue = GetUnitOwner()->ToPlayer();
            if (!rogue)
                return;

            int8 comboPoints = rogue->GetComboPoints();
            if (comboPoints <= 0)
                return;

            int32 cooldownReduction = aurEff->GetAmount() * comboPoints;
            static uint32 const restlessBladeSpells[] =
            {
                SPELL_ROGUE_ADRENALINE_RUSH,
                SPELL_ROGUE_KILLING_SPREE,
                SPELL_ROGUE_REDIRECT,
                SPELL_ROGUE_SHADOW_BLADES,
                SPELL_ROGUE_SPRINT
            };

            for (uint32 spellId : restlessBladeSpells)
                rogue->ModifySpellCooldown(spellId, -cooldownReduction);
        }

        void Register() OVERRIDE
        {
            OnEffectProc += AuraEffectProcFn(spell_rog_restless_blades_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_rog_restless_blades_AuraScript();
    }
};

struct KillingSpreeInvalidTargetCheck
{
    explicit KillingSpreeInvalidTargetCheck(Unit* caster) : _caster(caster) { }

    bool operator()(WorldObject* target) const
    {
        Unit* unit = target->ToUnit();
        if (!unit || !_caster->IsValidAttackTarget(unit))
            return true;

        if (unit->HasInvisibilityAura() || unit->HasStealthAura())
            return true;

        if (unit->IsTotem())
            return true;

        switch (unit->GetCreatureType())
        {
            case CREATURE_TYPE_CRITTER:
            case CREATURE_TYPE_NON_COMBAT_PET:
            case CREATURE_TYPE_WILD_PET:
                return true;
            default:
                break;
        }

        if (unit->isFeared() || unit->HasAuraType(SPELL_AURA_MOD_FEAR_2) || unit->HasAuraType(SPELL_AURA_MOD_CONFUSE))
            return true;

        return !_caster->IsWithinLOSInMap(unit);
    }

private:
    Unit* _caster;
};

// 51690 - Killing Spree
class spell_rog_killing_spree : public SpellScriptLoader
{
public:
    spell_rog_killing_spree() : SpellScriptLoader("spell_rog_killing_spree") { }

    class spell_rog_killing_spree_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_killing_spree_AuraScript);

        bool Load() OVERRIDE
        {
            _caster = GetUnitOwner()->ToPlayer();
            _mainTarget = 0;
            _glyphed = false;
            return _caster != nullptr;
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (_caster->HasAura(SPELL_ROGUE_GLYPH_OF_KILLING_SPREE))
            {
                _glyphed = true;
                _caster->GetPosition(&_startPos);
            }

            _caster->CastSpell(_caster, SPELL_ROGUE_KILLING_SPREE_DAMAGE_AURA, true);

            if (_caster->HasAura(SPELL_ROGUE_SUBTERFUGE_STEALTH) || _caster->HasAura(SPELL_ROGUE_SUBTERFUGE_VANISH))
            {
                if (!_caster->HasAura(SPELL_ROGUE_SUBTERFUGE))
                    _caster->CastSpell(_caster, SPELL_ROGUE_SUBTERFUGE, true);
            }
            else
                _caster->RemoveAurasByType(SPELL_AURA_MOD_STEALTH);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (_glyphed && _caster->GetExactDist(&_startPos) < 100.0f)
                _caster->NearTeleportTo(_startPos.GetPositionX(), _startPos.GetPositionY(), _startPos.GetPositionZ(), _startPos.GetOrientation());

            _caster->RemoveAurasDueToSpell(SPELL_ROGUE_KILLING_SPREE_DAMAGE_AURA);
        }

        void HandlePeriodic(AuraEffect const* /*aurEff*/)
        {
            PreventDefaultAction();

            KillingSpreeInvalidTargetCheck notValidTarget(_caster);
            Unit* target = nullptr;

            if (_mainTarget)
            {
                target = ObjectAccessor::GetUnit(*_caster, _mainTarget);
                if (!target || !target->IsAlive())
                    target = nullptr;
            }
            else
            {
                while (!_targets.empty())
                {
                    uint64 guid = Skyfire::Containers::SelectRandomContainerElement(_targets);
                    target = ObjectAccessor::GetUnit(*_caster, guid);
                    if (target && target->IsAlive() && !notValidTarget(target))
                        break;

                    target = nullptr;
                    _targets.erase(guid);
                }
            }

            if (target)
            {
                _caster->CastSpell(target, SPELL_ROGUE_KILLING_SPREE_TELEPORT, true);
                if (!_mainTarget || !notValidTarget(target))
                {
                    _caster->CastSpell(target, SPELL_ROGUE_KILLING_SPREE_DAMAGE, true);
                    if (Unit* selection = _caster->GetSelectedUnit())
                        _caster->Attack(selection, true);
                }
                else
                    _caster->AttackStop();
            }
            else
            {
                SetDuration(0);
                _caster->AttackStop();
            }
        }

    public:
        void AddTarget(Unit* unit)
        {
            _targets.insert(unit->GetGUID());
            if (unit->GetGUID() == _caster->GetTarget() || (!_caster->GetTarget() && !_mainTarget))
                if (!_caster->HasAura(SPELL_ROGUE_BLADE_FLURRY))
                    _mainTarget = unit->GetGUID();
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_rog_killing_spree_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_rog_killing_spree_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_rog_killing_spree_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }

    private:
        std::set<uint64> _targets;
        uint64 _mainTarget;
        bool _glyphed;
        Position _startPos;
        Player* _caster;
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_rog_killing_spree_AuraScript();
    }
};

typedef spell_rog_killing_spree::spell_rog_killing_spree_AuraScript KillingSpreeAuraScript;

// 51690 - Killing Spree (target selector / cast checks)
class spell_rog_killing_spree_target_selector : public SpellScriptLoader
{
public:
    spell_rog_killing_spree_target_selector() : SpellScriptLoader("spell_rog_killing_spree_target_selector") { }

    class spell_rog_killing_spree_target_selector_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_killing_spree_target_selector_SpellScript);

        SpellCastResult CheckCast()
        {
            Player* rogue = GetCaster()->ToPlayer();
            if (!rogue)
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;

            Unit* target = rogue->GetSelectedUnit();
            if (!target)
                return SpellCastResult::SPELL_FAILED_BAD_TARGETS;

            if (!rogue->IsValidAttackTarget(target))
                return SpellCastResult::SPELL_FAILED_BAD_TARGETS;

            if (!target->IsWithinDist3d(rogue, GetSpellInfo()->GetMaxRange(false)))
                return SpellCastResult::SPELL_FAILED_OUT_OF_RANGE;

            if (!rogue->IsWithinLOSInMap(target))
                return SpellCastResult::SPELL_FAILED_LINE_OF_SIGHT;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if(KillingSpreeInvalidTargetCheck(GetCaster()));
            if (targets.empty() || GetCaster()->GetVehicleBase())
            {
                if (Player* rogue = GetCaster()->ToPlayer())
                    rogue->GetGlobalCooldownMgr().CancelGlobalCooldown(GetSpellInfo());
                FinishCast(SpellCastResult::SPELL_FAILED_OUT_OF_RANGE);
            }
        }

        void AddTarget(SpellEffIndex /*effIndex*/)
        {
            if (Aura* aura = GetCaster()->GetAura(SPELL_ROGUE_KILLING_SPREE))
                if (KillingSpreeAuraScript* script = dynamic_cast<KillingSpreeAuraScript*>(aura->GetScriptByName("spell_rog_killing_spree")))
                    script->AddTarget(GetHitUnit());
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_rog_killing_spree_target_selector_SpellScript::CheckCast);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_rog_killing_spree_target_selector_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_rog_killing_spree_target_selector_SpellScript::AddTarget, EFFECT_1, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_rog_killing_spree_target_selector_SpellScript();
    }
};

// 408 - Kidney Shot (Revealing Strike duration)
class spell_rog_kidney_shot : public SpellScriptLoader
{
public:
    spell_rog_kidney_shot() : SpellScriptLoader("spell_rog_kidney_shot") { }

    class spell_rog_kidney_shot_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_kidney_shot_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (AuraEffect const* revealing = GetTarget()->GetAuraEffect(SPELL_ROGUE_REVEALING_STRIKE, EFFECT_2, GetCasterGUID()))
            {
                int32 duration = GetAura()->GetDuration();
                AddPct(duration, revealing->GetAmount());
                GetAura()->SetMaxDuration(duration);
                GetAura()->SetDuration(duration);
            }
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_rog_kidney_shot_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_rog_kidney_shot_AuraScript();
    }
};

// 1752 - Sinister Strike (Revealing Strike extra combo point)
class spell_rog_sinister_strike : public SpellScriptLoader
{
public:
    spell_rog_sinister_strike() : SpellScriptLoader("spell_rog_sinister_strike") { }

    class spell_rog_sinister_strike_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_sinister_strike_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_ROGUE_REVEALING_STRIKE) &&
                sSpellMgr->GetSpellInfo(SPELL_ROGUE_REVEALED_WEAKNESS);
        }

        void HandleHit()
        {
            Unit* target = GetHitUnit();
            Unit* caster = GetCaster();
            if (!target || !caster)
                return;

            Aura* revealing = target->GetAura(SPELL_ROGUE_REVEALING_STRIKE, caster->GetGUID());
            if (!revealing)
                return;

            if (roll_chance_i(revealing->GetSpellInfo()->ProcChance))
                caster->CastSpell(target, SPELL_ROGUE_REVEALED_WEAKNESS, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_rog_sinister_strike_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_rog_sinister_strike_SpellScript();
    }
};

// 79134 - Venomous Wounds
class spell_rog_venomous_wounds : public SpellScriptLoader
{
public:
    spell_rog_venomous_wounds() : SpellScriptLoader("spell_rog_venomous_wounds") { }

    class spell_rog_venomous_wounds_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_venomous_wounds_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_ROGUE_VENOMOUS_WOUND) &&
                sSpellMgr->GetSpellInfo(SPELL_ROGUE_VENOMOUS_VIM) &&
                sSpellMgr->GetSpellInfo(SPELL_ROGUE_GARROTE) &&
                sSpellMgr->GetSpellInfo(SPELL_ROGUE_RUPTURE);
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (!(eventInfo.GetTypeMask() & PROC_FLAG_DONE_PERIODIC))
                return false;

            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            Unit* target = eventInfo.GetActionTarget();
            if (!spellInfo || !target)
                return false;

            if (spellInfo->Id != SPELL_ROGUE_GARROTE && spellInfo->Id != SPELL_ROGUE_RUPTURE)
                return false;

            // Garrote does not trigger if the enemy also has your Rupture
            if (spellInfo->Id == SPELL_ROGUE_GARROTE && target->HasAura(SPELL_ROGUE_RUPTURE, GetCasterGUID()))
                return false;

            Unit::AuraApplicationMap const& auras = target->GetAppliedAuras();
            for (Unit::AuraApplicationMap::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
            {
                if (itr->second->GetBase()->GetCasterGUID() != GetCasterGUID())
                    continue;
                if (itr->second->GetBase()->GetSpellInfo()->Dispel == DISPEL_POISON)
                    return true;
            }
            return false;
        }

        void HandleProc(ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            Unit* caster = GetUnitOwner();
            Unit* target = eventInfo.GetProcTarget();
            if (!caster || !target)
                return;

            caster->CastSpell(target, SPELL_ROGUE_VENOMOUS_WOUND, true);
            caster->CastCustomSpell(SPELL_ROGUE_VENOMOUS_VIM, SPELLVALUE_BASE_POINT0, 10, caster, true);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_rog_venomous_wounds_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_rog_venomous_wounds_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_rog_venomous_wounds_AuraScript();
    }
};

// 51701 - Honor Among Thieves
class spell_rog_honor_among_thieves : public SpellScriptLoader
{
public:
    spell_rog_honor_among_thieves() : SpellScriptLoader("spell_rog_honor_among_thieves") { }

    class spell_rog_honor_among_thieves_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_honor_among_thieves_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_ROGUE_HONOR_AMONG_THIEVES);
        }

        bool Load() OVERRIDE
        {
            return GetUnitOwner()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (!(eventInfo.GetHitMask() & PROC_EX_CRITICAL_HIT))
                return false;

            if (eventInfo.GetSpellInfo() && eventInfo.GetSpellInfo()->Id == SPELL_ROGUE_THROW)
                return false;

            return GetUnitOwner()->IsInCombat();
        }

        void HandleProc(ProcEventInfo& /*eventInfo*/)
        {
            Player* rogue = GetUnitOwner()->ToPlayer();
            if (!rogue)
                return;

            // Proc cooldown is applied to the actor, not the aura owner — enforce ICD on the rogue.
            if (rogue->HasSpellCooldown(SPELL_ROGUE_HONOR_AMONG_THIEVES))
                return;

            Unit* target = NULL;
            if (uint64 comboGuid = rogue->GetComboTarget())
                target = ObjectAccessor::GetUnit(*rogue, comboGuid);
            if (!target)
                target = rogue->GetVictim();
            if (!target)
            {
                if (Unit* selected = rogue->GetSelectedUnit())
                    if (rogue->IsValidAttackTarget(selected))
                        target = selected;
            }

            if (!target)
                return;

            rogue->m_Events.AddEvent(new DelayedHonorAmongThievesEvent(rogue->GetGUID(), target->GetGUID()),
                rogue->m_Events.CalculateTime(1));
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_rog_honor_among_thieves_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_rog_honor_among_thieves_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_rog_honor_among_thieves_AuraScript();
    }
};

// 36554 - Shadowstep
class spell_rog_shadowstep : public SpellScriptLoader
{
public:
    spell_rog_shadowstep() : SpellScriptLoader("spell_rog_shadowstep") { }

    class spell_rog_shadowstep_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_shadowstep_SpellScript);

        SpellCastResult CheckCast()
        {
            if (GetCaster()->HasUnitState(UNIT_STATE_ROOT))
                return SpellCastResult::SPELL_FAILED_ROOTED;

            // Antipersonnel Cannon (Strand of the Ancients)
            if (Unit* target = GetExplTargetUnit())
            {
                if (target->GetEntry() == 27894)
                    return SpellCastResult::SPELL_FAILED_BAD_TARGETS;

                // "Auto Self Cast" can make the client select the caster
                if (target == GetCaster())
                    return SpellCastResult::SPELL_FAILED_BAD_TARGETS;
            }

            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_rog_shadowstep_SpellScript::CheckCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_rog_shadowstep_SpellScript();
    }
};

// 16511 - Hemorrhage
class spell_rog_hemorrhage : public SpellScriptLoader
{
public:
    spell_rog_hemorrhage() : SpellScriptLoader("spell_rog_hemorrhage") { }

    class spell_rog_hemorrhage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_hemorrhage_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_ROGUE_HEMORRHAGE_DOT);
        }

        void HandleOnHit()
        {
            Player* caster = GetCaster()->ToPlayer();
            if (!caster)
                return;

            if (Item* weapon = caster->GetWeaponForAttack(WeaponAttackType::BASE_ATTACK))
                if (weapon->GetTemplate()->SubClass == ITEM_SUBCLASS_WEAPON_DAGGER)
                    SetHitDamage(int32(GetHitDamage() * 1.45f));
        }

        void HandleAfterHit()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!caster || !target)
                return;

            // Glyph of Hemorrhage: only apply the bleed if the target is already bleeding
            if (caster->HasAura(SPELL_ROGUE_GLYPH_OF_HEMORRHAGE) && !target->HasAuraState(AURA_STATE_BLEEDING))
                return;

            // 50% of direct damage over 8 ticks
            int32 damage = GetHitDamage() / 2 / 8;
            if (damage <= 0)
                return;

            caster->CastCustomSpell(SPELL_ROGUE_HEMORRHAGE_DOT, SPELLVALUE_BASE_POINT0, damage, target, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_rog_hemorrhage_SpellScript::HandleOnHit);
            AfterHit += SpellHitFn(spell_rog_hemorrhage_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_rog_hemorrhage_SpellScript();
    }
};

// 1943, 703, 122233, 89775 - Sanguinary Vein (called by bleed DoTs)
class spell_rog_sanguinary_vein : public SpellScriptLoader
{
public:
    spell_rog_sanguinary_vein() : SpellScriptLoader("spell_rog_sanguinary_vein") { }

    class spell_rog_sanguinary_vein_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_sanguinary_vein_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_ROGUE_SANGUINARY_VEIN) &&
                sSpellMgr->GetSpellInfo(SPELL_ROGUE_SANGUINARY_VEIN_DEBUFF);
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetTarget();
            if (!caster || !target)
                return;

            // Glyph of Hemorrhaging Veins: Hemorrhage DoT only counts with the glyph
            if (GetId() == SPELL_ROGUE_HEMORRHAGE_DOT && !caster->HasAura(SPELL_ROGUE_GLYPH_OF_HEMORRHAGING_VEINS))
                return;

            if (caster->HasAura(SPELL_ROGUE_SANGUINARY_VEIN))
                caster->CastSpell(target, SPELL_ROGUE_SANGUINARY_VEIN_DEBUFF, true);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetTarget();
            if (!caster || !target)
                return;

            if (target->HasAura(SPELL_ROGUE_RUPTURE, GetCasterGUID()) ||
                target->HasAura(SPELL_ROGUE_GARROTE, GetCasterGUID()) ||
                target->HasAura(SPELL_ROGUE_CRIMSON_TEMPEST_DOT, GetCasterGUID()))
                return;

            if (caster->HasAura(SPELL_ROGUE_GLYPH_OF_HEMORRHAGING_VEINS) &&
                target->HasAura(SPELL_ROGUE_HEMORRHAGE_DOT, GetCasterGUID()))
                return;

            target->RemoveAurasDueToSpell(SPELL_ROGUE_SANGUINARY_VEIN_DEBUFF, GetCasterGUID());
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_rog_sanguinary_vein_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_rog_sanguinary_vein_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_rog_sanguinary_vein_AuraScript();
    }
};

// 14185 - Preparation
class spell_rog_preparation : public SpellScriptLoader
{
public:
    spell_rog_preparation() : SpellScriptLoader("spell_rog_preparation") { }

    class spell_rog_preparation_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_preparation_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_ROGUE_VANISH) &&
                sSpellMgr->GetSpellInfo(SPELL_ROGUE_SPRINT) &&
                sSpellMgr->GetSpellInfo(SPELL_ROGUE_EVASION);
        }

        void HandleCast()
        {
            Player* rogue = GetCaster()->ToPlayer();
            if (!rogue)
                return;

            rogue->RemoveSpellCooldown(SPELL_ROGUE_VANISH, true);
            rogue->RemoveSpellCooldown(SPELL_ROGUE_SPRINT, true);
            rogue->RemoveSpellCooldown(SPELL_ROGUE_EVASION, true);
            rogue->RemoveSpellCooldown(SPELL_ROGUE_DISMANTLE, true);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_rog_preparation_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_rog_preparation_SpellScript();
    }
};

// 73981, 110730 - Redirect
class spell_rog_redirect : public SpellScriptLoader
{
public:
    spell_rog_redirect() : SpellScriptLoader("spell_rog_redirect") { }

    class spell_rog_redirect_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_redirect_SpellScript);

        SpellCastResult CheckCast()
        {
            Player* rogue = GetCaster()->ToPlayer();
            if (!rogue)
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;

            if (!rogue->GetComboPoints())
                return SpellCastResult::SPELL_FAILED_NO_COMBO_POINTS;

            Unit* unitTarget = GetExplTargetUnit();
            if (!unitTarget || !rogue->GetComboTarget() || rogue->GetComboTarget() == unitTarget->GetGUID())
                return SpellCastResult::SPELL_FAILED_BAD_TARGETS;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void HandleHit()
        {
            Player* rogue = GetCaster()->ToPlayer();
            Unit* target = GetHitUnit();
            if (!rogue || !target)
                return;

            int8 cp = rogue->GetComboPoints();
            rogue->ClearComboPoints();
            rogue->AddComboPoints(target, cp, GetSpell());
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_rog_redirect_SpellScript::CheckCast);
            OnHit += SpellHitFn(spell_rog_redirect_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_rog_redirect_SpellScript();
    }
};

// 14181 - Relentless Strikes (triggered SCRIPT_EFFECT -> energize)
class spell_rog_relentless_strikes : public SpellScriptLoader
{
public:
    spell_rog_relentless_strikes() : SpellScriptLoader("spell_rog_relentless_strikes") { }

    class spell_rog_relentless_strikes_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_relentless_strikes_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_ROGUE_RELENTLESS_STRIKES_ENERGIZE);
        }

        void HandleHit()
        {
            GetCaster()->CastSpell(GetCaster(), SPELL_ROGUE_RELENTLESS_STRIKES_ENERGIZE, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_rog_relentless_strikes_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_rog_relentless_strikes_SpellScript();
    }
};

// 408 Kidney Shot, 1833 Cheap Shot - Nerve Strike talent applies debuff when stun ends
class spell_rog_nerve_strike : public SpellScriptLoader
{
public:
    spell_rog_nerve_strike() : SpellScriptLoader("spell_rog_nerve_strike") { }

    class spell_rog_nerve_strike_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_nerve_strike_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_ROGUE_NERVE_STRIKE) &&
                sSpellMgr->GetSpellInfo(SPELL_ROGUE_NERVE_STRIKE_EFFECT);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetTarget();
            if (!caster || !target)
                return;

            if (caster->HasAura(SPELL_ROGUE_NERVE_STRIKE))
                caster->CastSpell(target, SPELL_ROGUE_NERVE_STRIKE_EFFECT, true);
        }

        void Register() OVERRIDE
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_rog_nerve_strike_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_rog_nerve_strike_AuraScript();
    }
};

// 112947 - Nerve Strike (halve damage reduction vs players)
class spell_rog_nerve_strike_effect : public SpellScriptLoader
{
public:
    spell_rog_nerve_strike_effect() : SpellScriptLoader("spell_rog_nerve_strike_effect") { }

    class spell_rog_nerve_strike_effect_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_nerve_strike_effect_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (GetUnitOwner()->GetCharmerOrOwnerPlayerOrPlayerItself())
                amount /= 2;
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_rog_nerve_strike_effect_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_rog_nerve_strike_effect_AuraScript();
    }
};

// Poison applications - Master Poisoner
class spell_rog_master_poisoner : public SpellScriptLoader
{
public:
    spell_rog_master_poisoner() : SpellScriptLoader("spell_rog_master_poisoner") { }

    class spell_rog_master_poisoner_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_master_poisoner_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_ROGUE_MASTER_POISONER) &&
                sSpellMgr->GetSpellInfo(SPELL_ROGUE_MASTER_POISONER_DEBUFF);
        }

        void HandleHit()
        {
            Player* player = GetCaster()->ToPlayer();
            Unit* target = GetHitUnit();
            if (!player || !target)
                return;

            if (player->HasAura(SPELL_ROGUE_MASTER_POISONER))
                player->CastSpell(target, SPELL_ROGUE_MASTER_POISONER_DEBUFF, true);

            // Deadly Brew: certain poisons also apply Crippling Poison
            switch (GetSpellInfo()->Id)
            {
                case 5760:   // Mind-numbing Poison
                case 112961: // Leeching Poison
                case 113952: // Paralytic Poison
                    if (player->HasAura(SPELL_ROGUE_DEADLY_BREW))
                        player->CastSpell(target, SPELL_ROGUE_CRIPPLING_POISON, true);
                    break;
                default:
                    break;
            }
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_rog_master_poisoner_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_rog_master_poisoner_SpellScript();
    }
};

// 2094 - Blind (Glyph of Blind removes DoTs)
class spell_rog_blind : public SpellScriptLoader
{
public:
    spell_rog_blind() : SpellScriptLoader("spell_rog_blind") { }

    class spell_rog_blind_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_blind_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_ROGUE_GLYPH_OF_BLIND);
        }

        void HandleHit()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!caster || !target || !caster->HasAura(SPELL_ROGUE_GLYPH_OF_BLIND))
                return;

            bool dirtyTricks = caster->HasAura(SPELL_ROGUE_DIRTY_TRICKS);
            Unit::AuraEffectList const& periodicDamage = target->GetAuraEffectsByType(SPELL_AURA_PERIODIC_DAMAGE);
            Unit::AuraEffectList const& periodicDamagePct = target->GetAuraEffectsByType(SPELL_AURA_PERIODIC_DAMAGE_PERCENT);
            Unit::AuraEffectList const& periodicLeech = target->GetAuraEffectsByType(SPELL_AURA_PERIODIC_LEECH);

            std::set<Aura*> toRemove;
            auto collect = [&](Unit::AuraEffectList const& list)
            {
                for (AuraEffect const* aurEff : list)
                {
                    SpellInfo const* spellInfo = aurEff->GetSpellInfo();
                    AuraApplication const* app = aurEff->GetBase()->GetApplicationOfTarget(target->GetGUID());
                    if (!app || app->IsPositive() || spellInfo->Id == 32409) // SW:D
                        continue;

                    if (dirtyTricks && aurEff->GetCasterGUID() == caster->GetGUID())
                    {
                        if (spellInfo->Dispel == DISPEL_POISON ||
                            (spellInfo->GetAllEffectsMechanicMask() & (1 << MECHANIC_BLEED)) ||
                            spellInfo->Id == SPELL_ROGUE_VENOMOUS_WOUND)
                            continue;
                    }

                    toRemove.insert(aurEff->GetBase());
                }
            };

            collect(periodicDamage);
            collect(periodicDamagePct);
            collect(periodicLeech);

            for (Aura* aura : toRemove)
                target->RemoveAura(aura);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_rog_blind_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_rog_blind_SpellScript();
    }
};

// 8680 - Wound Poison (ensure healing reduction tooltip/amount is 25%)
class spell_rog_wound_poison : public SpellScriptLoader
{
public:
    spell_rog_wound_poison() : SpellScriptLoader("spell_rog_wound_poison") { }

    class spell_rog_wound_poison_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_wound_poison_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            // MoP: healing effects reduced by 25%. Force amount so client tooltip is correct.
            amount = -25;
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_rog_wound_poison_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_HEALING_PCT);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_rog_wound_poison_AuraScript();
    }
};

// 115834 - Shroud of Concealment (do not cloak combat/casting party members)
class spell_rog_shroud_of_concealment : public SpellScriptLoader
{
public:
    spell_rog_shroud_of_concealment() : SpellScriptLoader("spell_rog_shroud_of_concealment") { }

    class spell_rog_shroud_of_concealment_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_shroud_of_concealment_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if([](WorldObject* target)
            {
                Unit* unit = target->ToUnit();
                if (!unit)
                    return true;
                if (unit->IsVehicle() && unit->GetTypeId() != TypeID::TYPEID_PLAYER)
                    return true;
                return unit->IsInCombat() || unit->HasUnitState(UNIT_STATE_CASTING);
            });
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_rog_shroud_of_concealment_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_rog_shroud_of_concealment_SpellScript();
    }
};

// 76577 / 128829 - Smoke Bomb (periodic applies interfere targeting cloud)
class spell_rog_smoke_bomb : public SpellScriptLoader
{
public:
    spell_rog_smoke_bomb() : SpellScriptLoader("spell_rog_smoke_bomb") { }

    class spell_rog_smoke_bomb_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_smoke_bomb_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_ROGUE_SMOKE_BOMB_INTERFERE);
        }

        void HandlePeriodic(AuraEffect const* aurEff)
        {
            Unit* caster = GetTarget();
            DynamicObject* dyn = caster->GetDynObject(aurEff->GetId());
            if (!dyn)
                return;

            // Refresh interfere + ally damage reduction on everyone in the smoke cloud.
            caster->CastSpell(dyn->GetPositionX(), dyn->GetPositionY(), dyn->GetPositionZ(), SPELL_ROGUE_SMOKE_BOMB_INTERFERE, true);
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_rog_smoke_bomb_AuraScript::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_rog_smoke_bomb_AuraScript();
    }
};

// 703 - Garrote, 1833 - Cheap Shot, 8676 - Ambush
// Cloak and Dagger (138106): extend range (DBC) and teleport behind the target.
class spell_rog_cloak_and_dagger : public SpellScriptLoader
{
public:
    spell_rog_cloak_and_dagger() : SpellScriptLoader("spell_rog_cloak_and_dagger") { }

    class spell_rog_cloak_and_dagger_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_cloak_and_dagger_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_ROGUE_CLOAK_AND_DAGGER) ||
                !sSpellMgr->GetSpellInfo(SPELL_ROGUE_CLOAK_AND_DAGGER_TELEPORT))
                return false;
            return true;
        }

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            Unit* target = GetExplTargetUnit();
            if (!target)
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;

            if (!caster->HasAura(SPELL_ROGUE_CLOAK_AND_DAGGER))
                return SpellCastResult::SPELL_CAST_OK;

            // Rooted: can still open if already in melee (and behind for Ambush); no teleport.
            if (caster->HasUnitState(UNIT_STATE_ROOT))
            {
                if (!caster->IsWithinMeleeRange(target))
                    return SpellCastResult::SPELL_FAILED_ROOTED;

                if (GetSpellInfo()->Id == SPELL_ROGUE_AMBUSH && target->HasInArc(static_cast<float>(M_PI), caster))
                    return SpellCastResult::SPELL_FAILED_ROOTED;
            }

            return SpellCastResult::SPELL_CAST_OK;
        }

        void HandleHit()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!caster || !target)
                return;

            if (!caster->HasAura(SPELL_ROGUE_CLOAK_AND_DAGGER))
                return;

            // Cannot step through shadows while rooted.
            if (caster->HasUnitState(UNIT_STATE_ROOT))
                return;

            caster->CastSpell(target, SPELL_ROGUE_CLOAK_AND_DAGGER_TELEPORT, true);
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_rog_cloak_and_dagger_SpellScript::CheckCast);
            OnHit += SpellHitFn(spell_rog_cloak_and_dagger_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_rog_cloak_and_dagger_SpellScript();
    }
};

void AddSC_rogue_spell_scripts()
{
    new spell_rog_bandits_guile();
    new spell_rog_blade_flurry();
    new spell_rog_blind();
    new spell_rog_cheat_death();
    new spell_rog_cloak_and_dagger();
    new spell_rog_combat_potency();
    new spell_rog_combo_point_delayed();
    new spell_rog_crimson_tempest();
    new spell_rog_crippling_poison();
    new spell_rog_cut_to_the_chase();
    new spell_rog_deadly_poison();
    new spell_rog_fan_of_knives();
    new spell_rog_hemorrhage();
    new spell_rog_honor_among_thieves();
    new spell_rog_kidney_shot();
    new spell_rog_killing_spree();
    new spell_rog_killing_spree_target_selector();
    new spell_rog_master_of_subtlety();
    new spell_rog_master_poisoner();
    new spell_rog_nerve_strike();
    new spell_rog_nerve_strike_effect();
    new spell_rog_preparation();
    new spell_rog_recuperate();
    new spell_rog_redirect();
    new spell_rog_relentless_strikes();
    new spell_rog_restless_blades();
    new spell_rog_rupture();
    new spell_rog_sanguinary_vein();
    new spell_rog_shadowstep();
    new spell_rog_shroud_of_concealment();
    new spell_rog_sinister_strike();
    new spell_rog_smoke_bomb();
    new spell_rog_stealth();
    new spell_rog_stealth_subterfuge();
    new spell_rog_subterfuge_cast_trigger();
    new spell_rog_tricks_of_the_trade();
    new spell_rog_tricks_of_the_trade_proc();
    new spell_rog_vanish();
    new spell_rog_vanish_initial();
    new spell_rog_venomous_wounds();
    new spell_rog_wound_poison();

}
