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
    SPELL_ROGUE_CRIPPLING_POISON                    = 3409,
    SPELL_ROGUE_KILLING_SPREE                       = 51690,
    SPELL_ROGUE_MAIN_GAUCHE                         = 86392,
    SPELL_ROGUE_MASTER_OF_SUBTLETY_DAMAGE_PERCENT   = 31665,
    SPELL_ROGUE_MASTER_OF_SUBTLETY_PASSIVE          = 31223,
    SPELL_ROGUE_MASTER_OF_SUBTLETY_PERIODIC         = 31666,
    SPELL_ROGUE_NIGHTSTALKER                        = 130493,
    SPELL_ROGUE_NIGHTSTALKER_TALENT                 = 14062,
    SPELL_ROGUE_REDIRECT                            = 73981,
    SPELL_ROGUE_REVEALING_STRIKE                    = 84617,
    SPELL_ROGUE_SHADOW_BLADE_OFFHAND                = 121474,
    SPELL_ROGUE_SHADOW_BLADES                       = 121471,
    SPELL_ROGUE_SINISTER_STRIKE                     = 1752,
    SPELL_ROGUE_SLICE_AND_DICE                      = 5171,
    SPELL_ROGUE_SPRINT                              = 2983,
    SPELL_ROGUE_TRICKS_OF_THE_TRADE_DMG_BOOST       = 57933,
    SPELL_ROGUE_TRICKS_OF_THE_TRADE_PROC            = 59628
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
            if (Aura* aur = GetTarget()->GetAura(SPELL_ROGUE_SLICE_AND_DICE))
                aur->SetDuration(aur->GetSpellInfo()->GetMaxDuration(), true);
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

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_rog_rupture_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
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
        }

        void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            RogueStealthHelpers::HandleStealthRemove(GetTarget());
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

void AddSC_rogue_spell_scripts()
{
    new spell_rog_bandits_guile();
    new spell_rog_blade_flurry();
    new spell_rog_cheat_death();
    new spell_rog_combat_potency();
    new spell_rog_combo_point_delayed();
    new spell_rog_crippling_poison();
    new spell_rog_cut_to_the_chase();
    new spell_rog_deadly_poison();
    new spell_rog_master_of_subtlety();
    new spell_rog_recuperate();
    new spell_rog_restless_blades();
    new spell_rog_rupture();
    new spell_rog_stealth();
    new spell_rog_tricks_of_the_trade();
    new spell_rog_tricks_of_the_trade_proc();
}
