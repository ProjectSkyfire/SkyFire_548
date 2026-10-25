/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/*
 * Scripts for spells with SPELLFAMILY_MONK and SPELLFAMILY_GENERIC spells used by monk players.
 * Scriptnames of files in this file should be prefixed with "spell_monk_".
 */

#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "AreaTrigger.h"
#include "Containers.h"
#include "Group.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"
#include "PetAI.h"
#include "TemporarySummon.h"
#include <algorithm>
#include <map>
#include <set>

enum MonkSpells
{
    MONK_NPC_BLACK_OX_STATUE = 61146,

    SPELL_MONK_ROLL_TRIGGER = 107427,               // 5.4.8 18414
    SPELL_MONK_ITEM_PVP_GLOVES_BONUS = 124489,      // 5.4.8 18414
    SPELL_MONK_LEGACY_OF_THE_EMPEROR_RAID = 117666, // 5.4.8 18414
    SPELL_MONK_LEGACY_OF_THE_EMPEROR_ALLY = 117667, // 5.4.8 18414
    SPELL_MONK_DISABLE_ROOT = 116706,               // 5.4.8 18414
    SPELL_MONK_PARALYSIS = 115078,                  // 5.4.8 18414

    SPELL_MONK_FLYING_SERPENT_KICK = 101545,            // 5.4.8 18414
    SPELL_MONK_FLYING_SERPENT_KICK_AOE = 123586,        // 5.4.8 18414
    SPELL_MONK_TOUCH_OF_KARMA_REDIRECT_DAMAGE = 124280, // 5.4.8 18414

    SPELL_MONK_PROVOKE_AOE = 118635,    // 5.4.8 18414
    SPELL_MONK_PROVOKE_TARGET = 116189, // 5.4.8 18414
    //

    SPELL_MONK_SOOTHING_MIST_VISUAL = 125955,
    SPELL_MONK_SOOTHING_MIST_ENERGIZE = 116335,
    SPELL_MONK_STAGGER = 124255,
    SPELL_MONK_LIGHT_STAGGER = 124275,
    SPELL_MONK_MODERATE_STAGGER = 124274,
    SPELL_MONK_HEAVY_STAGGER = 124273,
    SPELL_MONK_MASTERY_ELUSIVE_BRAWLER = 117906,
    SPELL_MONK_SHUFFLE = 115307,
    SPELL_MONK_STAGGERING = 138233,
    SPELL_MONK_T15_BREWMASTER_4P_BONUS = 138236,
    SPELL_MONK_VENGEANCE = 120267,
    SPELL_MONK_FISTS_OF_FURY = 113656,
    SPELL_MONK_FISTS_OF_FURY_VISUAL = 123154,
    SPELL_MONK_RISING_SUN_KICK_DEBUFF = 130320,
    SPELL_MONK_COMBO_BREAKER_KICK = 116768,
    SPELL_MONK_COMBO_BREAKER_PALM = 118864,
    SPELL_MONK_MUSCLE_MEMORY = 139598,
    SPELL_MONK_MUSCLE_MEMORY_BUFF = 139597,
    SPELL_MONK_RENEWING_MIST = 115151,
    SPELL_MONK_THUNDER_FOCUS_TEA = 116680,
    SPELL_MONK_INTERNAL_MEDICINE = 115451,
    SPELL_MONK_GLYPH_OF_DETOX = 146954,
    SPELL_MONK_GLYPH_OF_LIFE_COCOON = 124989,
    SPELL_MONK_CHI_TORPEDO_HEAL = 124040,
    SPELL_MONK_CHI_TORPEDO_DAMAGE = 117993,
    SPELL_MONK_TIGEREYE_BREW = 116740,
    SPELL_MONK_TIGEREYE_BREW_STACKS = 125195,
    SPELL_MONK_ELUSIVE_BREW_STACKS = 128939,
    SPELL_MONK_ELUSIVE_BREW = 115308,
    SPELL_MONK_KEG_SMASH_VISUAL = 123662,
    SPELL_MONK_KEG_SMASH_ENERGIZE = 127796,
    SPELL_MONK_WEAKENED_BLOWS = 115798,
    SPELL_MONK_DIZZYING_HAZE = 116330,
    SPELL_MONK_CLASH_CHARGE = 126452,
    SPELL_MONK_SPEAR_HAND_STRIKE_SILENCE = 116709,
    SPELL_MONK_CHI_BURST_DAMAGE = 130651,
    SPELL_MONK_CHI_BURST_HEAL = 130654,
    SPELL_MONK_ZEN_SPHERE_DAMAGE = 124098,
    SPELL_MONK_ZEN_SPHERE_HEAL = 124081,
    SPELL_MONK_ZEN_SPHERE_DETONATE_HEAL = 124101,
    SPELL_MONK_ZEN_SPHERE_DETONATE_DAMAGE = 125033,
    SPELL_MONK_HEALING_ELIXIRS_AURA = 122280,
    SPELL_MONK_HEALING_ELIXIRS_RESTORE_HEALTH = 122281,
    SPELL_MONK_CRACKLING_JADE_LIGHTNING_CHANNEL = 117952,
    SPELL_MONK_CRACKLING_JADE_LIGHTNING_HIDDEN_CD = 117953,
    SPELL_MONK_CRACKLING_JADE_LIGHTNING_AURA = 117959,
    SPELL_MONK_CRACKLING_JADE_LIGHTNING_KNOCKBACK = 117962,
    SPELL_MONK_CRACKLING_JADE_LIGHTNING_CHI_PROC = 123332,
    SPELL_MONK_FORTIFYING_BREW_AURA = 120954,
    SPELL_MONK_GLYPH_OF_FORTIFYING_BREW = 124997,
    SPELL_MONK_BREATH_OF_FIRE_PERIODIC = 123725,
    SPELL_MONK_BREATH_OF_FIRE_CONFUSE = 123393,
    SPELL_MONK_GLYPH_OF_BREATH_OF_FIRE = 123394,


    SPELL_MONK_EXPEL_HARM_AREA_DMG = 115129,
    SPELL_MONK_TOUCH_OF_DEATH_PLAYER = 124490,
    SPELL_MONK_SPINNING_CRANE_KICK = 107270,
    SPELL_MONK_SPINNING_CRANE_KICK_ENERGIZE = 129881,
    SPELL_MONK_BREWING_TIGEREYE_BREW = 123980,
    SPELL_MONK_TIGEREYE_BREW_BUFF = 125195,
    SPELL_MONK_TIGER_STRIKES = 120273,
    SPELL_MONK_ZEN_PILGRIMAGE = 126892,
    SPELL_MONK_ZEN_PILGRIMAGE_RETURN = 126895,
    SPELL_MONK_HEALING_SPHERE = 115460,
    SPELL_MONK_HEALING_SPHERE_HEAL = 115464,
    SPELL_MONK_HEALING_SPHERE_STACK = 124458,
    SPELL_MONK_HEALING_SPHERE_EXPIRE_HEAL = 135914,
    SPELL_MONK_CHI_SPHERE_ENERGIZE = 121283,
    SPELL_MONK_GIFT_OF_THE_OX_RIGHT = 124503,
    SPELL_MONK_GIFT_OF_THE_OX_LEFT = 124506,
    SPELL_MONK_GIFT_OF_THE_OX_HEAL = 124507,
    SPELL_MONK_GIFT_OF_THE_SERPENT_SUMMON = 119031,
    SPELL_MONK_GIFT_OF_THE_SERPENT_SELECTOR = 119036,
    SPELL_MONK_GIFT_OF_THE_SERPENT_HEAL = 124041,
    SPELL_MONK_GIFT_OF_THE_SERPENT_HEAL_EXPIRED = 135920,
    SPELL_MONK_ENERGY_SPHERE_SUMMON_LEFT = 138310,
    SPELL_MONK_ENERGY_SPHERE_SUMMON_RIGHT = 140674,
    SPELL_MONK_ENERGY_SPHERE = 138311,
    SPELL_MONK_AFTERLIFE_SUMMON = 117032,
    SPELL_MONK_AFTERLIFE_HEALING_SPHERE = 125355,
    SPELL_MONK_BLACKOUT_KICK = 100784,
    SPELL_MONK_RING_OF_PEACE_AURA = 140023,
    SPELL_MONK_RING_OF_PEACE_SILENCE = 137460,
    SPELL_MONK_RING_OF_PEACE_DISARM = 137461,
    SPELL_MONK_SANCTUARY_OF_THE_OX = 126119,
    SPELL_MONK_BLACK_OX_STATUE_GUARD_SELECTOR = 118605,
    SPELL_MONK_BLACK_OX_STATUE_GUARD = 118604,
    SPELL_MONK_BLACK_OX_STATUE_GUARD_MAGIC = 136070,
    SPELL_MONK_GLYPH_OF_GUARD = 123401,
    SPELL_MONK_SOOTHING_MISTS_MISSILE = 117899,
    SPELL_MONK_SOOTHING_MIST_STATUE_CHANNEL = 125950,
    SPELL_MONK_SOOTHING_MIST_STATUE_HEAL = 125953,
    SPELL_MONK_SOOTHING_MIST_STATUE_TRIGGER = 125962,
    SPELL_MONK_STORM_EARTH_AND_FIRE = 137639,
    SPELL_MONK_SEF_SUMMON_EARTH_SPIRIT = 138121,
    SPELL_MONK_SEF_SUMMON_STORM_SPIRIT = 138122,
    SPELL_MONK_SEF_SUMMON_FIRE_SPIRIT = 138123,
    SPELL_MONK_SEF_STORM_VISUAL = 138080,
    SPELL_MONK_SEF_FIRE_VISUAL = 138081,
    SPELL_MONK_SEF_EARTH_VISUAL = 138083,
    SPELL_MONK_SEF_FIXATE_AND_SCALING = 138130,
    SPELL_MONK_SEF_SPIRIT_PASSIVE = 138131,
    SPELL_MONK_TRANSCENDENCE_CLONE = 119051,
    SPELL_MONK_TRANSCENDENCE_CLONE_TRIGGER = 119052,
    SPELL_MONK_FLYING_KICK = 138104,
    SPELL_MONK_XUEN_THREAT = 123995,
    SPELL_MONK_XUEN_CRACKLING_TIGER_LIGHTNING = 123999,
    SPELL_MONK_XUEN_EMINENCE = 130324,
    SPELL_MONK_SPAWN_SMOKE = 36747,

    MONK_NPC_STORM_SPIRIT = 69680,
    MONK_NPC_FIRE_SPIRIT = 69791,
    MONK_NPC_EARTH_SPIRIT = 69792,
};

// 5.4.8 18414
// 122278 - Dampen Harm
class spell_monk_dampen_harm : public SpellScriptLoader
{
public:
    spell_monk_dampen_harm() : SpellScriptLoader("spell_monk_dampen_harm")
    { }

    class spell_monk_dampen_harm_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_dampen_harm_AuraScript);

        uint32 healthPct;

        bool Load()
        {
            healthPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
            return GetUnitOwner()->ToPlayer();
        }

        void CalculateAmount(AuraEffect const* /*auraEffect*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            amount = -1;
        }

        void Absorb(AuraEffect* auraEffect, DamageInfo& dmgInfo, uint32& absorbAmount)
        {
            Unit* target = GetTarget();

            uint32 health = target->CountPctFromMaxHealth(healthPct);

            if (dmgInfo.GetDamage() < health)
                return;

            // The next 3 attacks within 45 sec that deal damage equal to 10% or more of your total health are reduced by half
            absorbAmount = dmgInfo.GetDamage() / 2;
            auraEffect->GetBase()->DropCharge();
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_dampen_harm_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            OnEffectAbsorb += AuraEffectAbsorbFn(spell_monk_dampen_harm_AuraScript::Absorb, EFFECT_0);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_monk_dampen_harm_AuraScript();
    }
};

// 5.4.8 18414
// Provoke - 115546
class spell_monk_provoke : public SpellScriptLoader
{
public:
    spell_monk_provoke() : SpellScriptLoader("spell_monk_provoke") { }

    class spell_monk_provoke_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_provoke_SpellScript);

        bool Validate(SpellInfo const* /*spell*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MONK_PROVOKE_AOE) ||
                !sSpellMgr->GetSpellInfo(SPELL_MONK_PROVOKE_TARGET))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Unit* caster = GetCaster())
            {
                if (Creature* target = GetHitUnit()->ToCreature())
                {
                    if (target == GetCaster()->FindNearestCreature(MONK_NPC_BLACK_OX_STATUE, 40.0f))
                        caster->CastSpell(target, SPELL_MONK_PROVOKE_AOE, true);
                    else
                        caster->CastSpell(target, SPELL_MONK_PROVOKE_TARGET, true);
                }
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_monk_provoke_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_provoke_SpellScript();
    }
};


// 5.4.8 18414
// 115057 - Flying Serpent Kick
class spell_monk_flying_serpent_kick : public SpellScriptLoader
{
public:
    spell_monk_flying_serpent_kick() : SpellScriptLoader("spell_monk_flying_serpent_kick") { }

    class spell_monk_flying_serpent_kick_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_flying_serpent_kick_SpellScript);

        bool Validate(SpellInfo const* /*spell*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MONK_FLYING_SERPENT_KICK) ||
                !sSpellMgr->GetSpellInfo(SPELL_MONK_ITEM_PVP_GLOVES_BONUS) ||
                !sSpellMgr->GetSpellInfo(SPELL_MONK_FLYING_SERPENT_KICK_AOE))
                return false;
            return true;
        }

        void HandleOnCast()
        {
            if (Unit* caster = GetCaster())
            {
                if (Player* _player = caster->ToPlayer())
                {
                    if (_player->HasAura(SPELL_MONK_FLYING_SERPENT_KICK))
                    {
                        _player->CastSpell(_player, SPELL_MONK_FLYING_SERPENT_KICK_AOE, true);
                        _player->RemoveAura(SPELL_MONK_FLYING_SERPENT_KICK);
                    }

                    if (caster->HasAura(SPELL_MONK_ITEM_PVP_GLOVES_BONUS))
                        caster->RemoveAurasByType(SPELL_AURA_MOD_DECREASE_SPEED);
                }
            }
        }

        void Register()
        {
            OnCast += SpellCastFn(spell_monk_flying_serpent_kick_SpellScript::HandleOnCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_flying_serpent_kick_SpellScript();
    }
};

// 5.4.8 18414
// 122470 - Touch of Karma
class spell_monk_touch_of_karma : public SpellScriptLoader
{
public:
    spell_monk_touch_of_karma() : SpellScriptLoader("spell_monk_touch_of_karma") { }

    class spell_monk_touch_of_karma_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_touch_of_karma_AuraScript);

        uint32 totalAbsorbAmount;

        bool Validate(SpellInfo const* /*spell*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MONK_TOUCH_OF_KARMA_REDIRECT_DAMAGE))
                return false;
            return true;
        }

        bool Load()
        {
            totalAbsorbAmount = 0;
            return true;
        }

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (GetCaster())
                amount = GetCaster()->GetMaxHealth();
        }

        void OnAbsorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
        {
            if (Unit* caster = dmgInfo.GetVictim())
            {
                if (Unit* attacker = dmgInfo.GetAttacker())
                {
                    totalAbsorbAmount += dmgInfo.GetDamage();

                    if (attacker->HasAura(aurEff->GetSpellInfo()->Id, caster->GetGUID()))
                        caster->CastCustomSpell(SPELL_MONK_TOUCH_OF_KARMA_REDIRECT_DAMAGE, SPELLVALUE_BASE_POINT0, (totalAbsorbAmount / 6), attacker);
                }
            }
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_touch_of_karma_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_SCHOOL_ABSORB);
            OnEffectAbsorb += AuraEffectAbsorbFn(spell_monk_touch_of_karma_AuraScript::OnAbsorb, EFFECT_1);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_monk_touch_of_karma_AuraScript();
    }
};

// 5.4.8 18414
// 115078 - Paralysis
class spell_monk_paralysis : public SpellScriptLoader
{
public:
    spell_monk_paralysis() : SpellScriptLoader("spell_monk_paralysis") { }

    class spell_monk_paralysis_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_paralysis_SpellScript);

        void HandleOnHit()
        {
            if (Unit* caster = GetCaster())
            {
                if (Unit* target = GetHitUnit())
                {
                    if (target->isInBack(caster))
                    {
                        if (AuraApplication* aura = target->GetAuraApplication(SPELL_MONK_PARALYSIS))
                        {
                            int32 maxDuration = aura->GetBase()->GetMaxDuration();
                            int32 newDuration = maxDuration * 1.5;
                            aura->GetBase()->SetDuration(newDuration);
                            aura->GetBase()->SetMaxDuration(newDuration);
                        }
                    }
                }
            }
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_monk_paralysis_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_paralysis_SpellScript();
    }
};

// 5.4.8 18414
// 116095 - Disable
class spell_monk_disable : public SpellScriptLoader
{
public:
    spell_monk_disable() : SpellScriptLoader("spell_monk_disable") { }

    class spell_monk_disable_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_disable_AuraScript);

        bool Validate(SpellInfo const* /*spell*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MONK_DISABLE_ROOT))
                return false;
            return true;
        }

        void OnPeriodic(AuraEffect const* aurEff)
        {
            if (aurEff->GetTickNumber() % aurEff->GetTotalTicks() != 0) // last tick of duration
                return;

            if (Unit* caster = GetCaster())
                if (Unit* owner = GetUnitOwner())
                    if ((owner->GetDistance2d(caster) < 10.0f) && caster->IsAlive())
                        aurEff->GetBase()->RefreshDuration();
        }

        void AfterReapply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
                if (Unit* target = GetTarget())
                    caster->CastSpell(target, SPELL_MONK_DISABLE_ROOT, true);
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_monk_disable_AuraScript::OnPeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            AfterEffectApply += AuraEffectRemoveFn(spell_monk_disable_AuraScript::AfterReapply, EFFECT_0, SPELL_AURA_MOD_DECREASE_SPEED, AURA_EFFECT_HANDLE_REAPPLY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_monk_disable_AuraScript();
    }
};

// 5.4.8 18414
// 115921 - Legacy of the Emperor
class spell_monk_legacy_of_the_emperor : public SpellScriptLoader
{
public:
    spell_monk_legacy_of_the_emperor() : SpellScriptLoader("spell_monk_legacy_of_the_emperor") { }

    class spell_monk_legacy_of_the_emperor_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_legacy_of_the_emperor_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MONK_LEGACY_OF_THE_EMPEROR_RAID) ||
                !sSpellMgr->GetSpellInfo(SPELL_MONK_LEGACY_OF_THE_EMPEROR_ALLY))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (!GetHitUnit())
                return;

            if (GetCaster()->IsInRaidWith(GetHitUnit()))
                GetCaster()->CastSpell(GetCaster(), SPELL_MONK_LEGACY_OF_THE_EMPEROR_RAID, true);
            else
                GetCaster()->CastSpell(GetHitUnit(), SPELL_MONK_LEGACY_OF_THE_EMPEROR_ALLY, true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_monk_legacy_of_the_emperor_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_monk_legacy_of_the_emperor_SpellScript();
    }
};

// 5.4.8 18414
// Roll - 109132
class spell_monk_roll : public SpellScriptLoader
{
public:
    spell_monk_roll() : SpellScriptLoader("spell_monk_roll") { }

    class spell_monk_roll_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_roll_SpellScript);

        bool Validate(SpellInfo const* /*spell*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MONK_ROLL_TRIGGER) ||
                !sSpellMgr->GetSpellInfo(SPELL_MONK_ITEM_PVP_GLOVES_BONUS))
                return false;
            return true;
        }

        void HandleBeforeCast()
        {
            Aura* aur = GetCaster()->AddAura(SPELL_MONK_ROLL_TRIGGER, GetCaster());
            if (!aur)
                return;

            AuraApplication* app = aur->GetApplicationOfTarget(GetCaster()->GetGUID());
            if (!app)
                return;

            app->ClientUpdate();
        }

        void HandleAfterCast()
        {
            Player* caster = GetCaster()->ToPlayer();
            if (!caster)
                return;

            if (caster->HasAura(SPELL_MONK_ITEM_PVP_GLOVES_BONUS))
                caster->RemoveAurasByType(SPELL_AURA_MOD_DECREASE_SPEED);

            caster->CastSpell(caster, SPELL_MONK_ROLL_TRIGGER, true);
        }

        void Register()
        {
            BeforeCast += SpellCastFn(spell_monk_roll_SpellScript::HandleBeforeCast);
            AfterCast += SpellCastFn(spell_monk_roll_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_roll_SpellScript();
    }
};



// 117952 - Crackling Jade Lightning
class spell_monk_crackling_jade_lightning : public SpellScriptLoader
{
    public:
    spell_monk_crackling_jade_lightning() : SpellScriptLoader("spell_monk_crackling_jade_lightning")
    { }

    class spell_monk_crackling_jade_lightning_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_crackling_jade_lightning_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MONK_CRACKLING_JADE_LIGHTNING_AURA))
                return false;
            return true;
        }

        void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (GetCaster())
            {
                GetCaster()->CastSpell(GetCaster(), SPELL_MONK_CRACKLING_JADE_LIGHTNING_AURA, true);
                GetCaster()->CastSpell(GetCaster(), SPELL_MONK_CRACKLING_JADE_LIGHTNING_AURA, true, NULL, aurEff);
                GetCaster()->CastSpell(GetCaster(), SPELL_MONK_CRACKLING_JADE_LIGHTNING_CHI_PROC, true);
            }
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetCaster())
            {
                GetCaster()->RemoveAura(SPELL_MONK_CRACKLING_JADE_LIGHTNING_AURA);
                GetCaster()->RemoveAura(SPELL_MONK_CRACKLING_JADE_LIGHTNING_CHI_PROC);
            }
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_monk_crackling_jade_lightning_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_monk_crackling_jade_lightning_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_monk_crackling_jade_lightning_AuraScript();
    }
};

// 117959 Crackling - Jade Lightning
class spell_monk_crackling_jade_lightning_aura : public SpellScriptLoader
{
    public:
    spell_monk_crackling_jade_lightning_aura() : SpellScriptLoader("spell_monk_crackling_jade_lightning_aura")
    { }

    class spell_monk_crackling_jade_lightning_aura_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_crackling_jade_lightning_aura_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MONK_CRACKLING_JADE_LIGHTNING_HIDDEN_CD))
                return false;
            return true;
        }

        bool CheckProc(ProcEventInfo& /*eventInfo*/)
        {
            if (GetTarget()->HasAura(SPELL_MONK_CRACKLING_JADE_LIGHTNING_HIDDEN_CD))
                return false;

            // Just prevention against buggers
            Spell* currentChanneledSpell = GetTarget()->GetCurrentSpell(CURRENT_CHANNELED_SPELL);
            if (!currentChanneledSpell || currentChanneledSpell->GetSpellInfo()->Id != SPELL_MONK_CRACKLING_JADE_LIGHTNING_CHANNEL)
                return false;

            return true;
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            GetTarget()->CastSpell(eventInfo.GetActor(), SPELL_MONK_CRACKLING_JADE_LIGHTNING_KNOCKBACK, true);
            GetTarget()->CastSpell(GetTarget(), SPELL_MONK_CRACKLING_JADE_LIGHTNING_HIDDEN_CD, true);
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_monk_crackling_jade_lightning_aura_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_monk_crackling_jade_lightning_aura_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_monk_crackling_jade_lightning_aura_AuraScript();
    }
};

class spell_monk_fortifying_brew : public SpellScriptLoader
{
    public:
    spell_monk_fortifying_brew() : SpellScriptLoader("spell_monk_fortifying_brew")
    { }

    class spell_monk_fortifying_brew_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_fortifying_brew_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MONK_FORTIFYING_BREW_AURA) || !sSpellMgr->GetSpellInfo(SPELL_MONK_GLYPH_OF_FORTIFYING_BREW))
                return false;
            return true;
        }

        void HandleOnHit(SpellEffIndex /*effIndex*/)
        {
            if (Unit* target = GetHitUnit())
            {
                int32 bp0 = GetEffectValue();
                int32 bp1 = -bp0;

                if (Aura* aura = target->GetAura(SPELL_MONK_GLYPH_OF_FORTIFYING_BREW))
                {
                    bp0 -= aura->GetEffect(EFFECT_1)->GetAmount();
                    bp1 -= aura->GetEffect(EFFECT_0)->GetAmount();
                }

                bp0 = CalculatePct(target->GetMaxHealth(), bp0);
                target->CastCustomSpell(target, SPELL_MONK_FORTIFYING_BREW_AURA, &bp0, &bp1, NULL, true);
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_monk_fortifying_brew_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_monk_fortifying_brew_SpellScript();
    }
};

class spell_monk_breath_of_fire : public SpellScriptLoader
{
    public:
    spell_monk_breath_of_fire() : SpellScriptLoader("spell_monk_breath_of_fire")
    { }

    class spell_monk_breath_of_fire_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_breath_of_fire_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MONK_BREATH_OF_FIRE_PERIODIC) || !sSpellMgr->GetSpellInfo(SPELL_MONK_DIZZYING_HAZE))
                return false;
            return true;
        }

        void HandleOnHit(SpellEffIndex /*effIndex*/)
        {
            if (!GetHitUnit())
                return;

            if (GetHitUnit()->HasAura(SPELL_MONK_DIZZYING_HAZE))
            {
                GetCaster()->CastSpell(GetHitUnit(), SPELL_MONK_BREATH_OF_FIRE_PERIODIC, true);
                if (GetCaster()->HasAura(SPELL_MONK_GLYPH_OF_BREATH_OF_FIRE))
                    GetCaster()->CastSpell(GetHitUnit(), SPELL_MONK_BREATH_OF_FIRE_CONFUSE, true);
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_monk_breath_of_fire_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_monk_breath_of_fire_SpellScript();
    }
};



class spell_monk_expel_harm : public SpellScriptLoader
{
    public:
    spell_monk_expel_harm() : SpellScriptLoader("spell_monk_expel_harm")
    { }

    class spell_monk_expel_harm_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_expel_harm_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MONK_EXPEL_HARM_AREA_DMG))
                return false;
            return true;
        }

        void DealAreaDamage(SpellEffIndex index)
        {
            if (!GetHitUnit())
                return;

            int32 dmg = GetHitHeal() / 2;
            if (dmg > 0)
                GetHitUnit()->CastCustomSpell(SPELL_MONK_EXPEL_HARM_AREA_DMG, SPELLVALUE_BASE_POINT0, dmg, GetHitUnit(), true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_monk_expel_harm_SpellScript::DealAreaDamage, EFFECT_0, SPELL_EFFECT_HEAL);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_monk_expel_harm_SpellScript();
    }
};

class spell_monk_touch_of_death : public SpellScriptLoader
{
    public:
    spell_monk_touch_of_death() : SpellScriptLoader("spell_monk_touch_of_death") { }

    class spell_monk_touch_of_death_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_touch_of_death_SpellScript);

        SpellCastResult CheckRequirement()
        {
            if (Unit* caster = GetCaster())
            {
                if (Unit* target = GetExplTargetUnit())
                {
                    if (target->ToPlayer())
                    {
                        if (caster->HasAura(SPELL_MONK_TOUCH_OF_DEATH_PLAYER))
                        {
                            if (target->HealthAbovePct(GetSpellInfo()->Effects [EFFECT_1].BasePoints))
                                return SpellCastResult::SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
                        }
                        else
                            return SpellCastResult::SPELL_FAILED_TARGET_IS_PLAYER;
                    }
                    else
                    {
                        if (target->GetHealth() > caster->GetHealth())
                            return SpellCastResult::SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
                    }
                }
            }
            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_monk_touch_of_death_SpellScript::CheckRequirement);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_monk_touch_of_death_SpellScript();
    }
};






/*
// Spinning Crane Kick - 107270
class spell_monk_spinning_crane_kick : public SpellScriptLoader
{
    public:
    spell_monk_spinning_crane_kick() : SpellScriptLoader("spell_monk_spinning_crane_kick")
    { }

    class spell_monk_spinning_crane_kick_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_spinning_crane_kick_SpellScript);

        bool Validate(SpellInfo const* spell)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MONK_SPINNING_CRANE_KICK))
                return false;
            return true;
        }

        void HandleOnCast()
        {
            if (Unit* caster = GetCaster())
            {
                std::list<Unit*> targets;
                Skyfire::AnyUnfriendlyUnitInObjectRangeCheck u_check(caster, caster, 8.0f);
                Skyfire::UnitListSearcher<Skyfire::AnyUnfriendlyUnitInObjectRangeCheck> searcher(caster, targets, u_check);
                caster->VisitNearbyObject(8.0f, searcher);
                if (targets.size() >= 3)
                    caster->CastSpell(caster, SPELL_MONK_SPINNING_CRANE_KICK_ENERGIZE, true);
            }
        }

        void Register()
        {
            OnCast += SpellCastFn(spell_monk_spinning_crane_kick_SpellScript::HandleOnCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_spinning_crane_kick_SpellScript();
    }
};
*/
// Brewing: Tigereye Brew - 123980
class spell_monk_brewing_tigereye_brew : public SpellScriptLoader
{
    public:
    spell_monk_brewing_tigereye_brew() : SpellScriptLoader("spell_monk_brewing_tigereye_brew")
    { }

    class spell_monk_brewing_tigereye_brew_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_brewing_tigereye_brew_AuraScript);

        bool Validate(SpellInfo const* /*spell*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MONK_BREWING_TIGEREYE_BREW))
                return false;
            return true;
        }

        uint32 chiConsumed;

        void HandleOnEffectApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            chiConsumed = 0;
        }

        void SetData(uint32 type, uint32 data)
        {
            if (Unit* caster = GetCaster())
            {
                chiConsumed += data;
                if (chiConsumed >= 4)
                {
                    caster->CastSpell(caster, SPELL_MONK_TIGEREYE_BREW_BUFF, true);
                    chiConsumed -= 4;
                }
            }
        }

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_monk_brewing_tigereye_brew_AuraScript::HandleOnEffectApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };


    AuraScript* GetAuraScript() const
    {
        return new spell_monk_brewing_tigereye_brew_AuraScript();
    }
};

#define maxStack 10

// Tigereye Brew - 116740
class spell_monk_tigereye_brew : public SpellScriptLoader
{
    public:
    spell_monk_tigereye_brew() : SpellScriptLoader("spell_monk_tigereye_brew")
    { }

    class spell_monk_tigereye_brew_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_tigereye_brew_AuraScript);

        bool Validate(SpellInfo const* /*spell*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MONK_TIGEREYE_BREW))
                return false;
            return true;
        }

        void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* caster = GetCaster())
            {
                if (Aura* aur = caster->GetAura(SPELL_MONK_TIGEREYE_BREW_BUFF))
                {
                    uint16 stack = aur->GetStackAmount();

                    // Max bonus = 60%
                    if (stack > maxStack)
                        amount *= maxStack;
                    else
                        amount *= stack;
                }
            }
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_tigereye_brew_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_tigereye_brew_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_HEALING_DONE_PERCENT);
        }
    };


    AuraScript* GetAuraScript() const
    {
        return new spell_monk_tigereye_brew_AuraScript();
    }

    class spell_monk_tigereye_brew_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_tigereye_brew_SpellScript);

        void HandleAfterCast()
        {
            if (Unit* caster = GetCaster())
            {
                if (Aura* aur = caster->GetAura(SPELL_MONK_TIGEREYE_BREW_BUFF))
                {
                    uint16 stack = aur->GetStackAmount();
                    // Removes at most 10 stacks
                    if (stack > maxStack)
                    {
                        // One of the stacks is already removed automatically
                        aur->SetStackAmount(stack - maxStack + 1);
                    }
                    else
                        caster->RemoveAura(SPELL_MONK_TIGEREYE_BREW_BUFF);
                }
            }
        }

        void Register()
        {
            AfterCast += SpellCastFn(spell_monk_tigereye_brew_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_tigereye_brew_SpellScript();
    }
};

// Tiger Strikes - 120273
class spell_monk_tiger_strikes : public SpellScriptLoader
{
    public:
    spell_monk_tiger_strikes() : SpellScriptLoader("spell_monk_tiger_strikes")
    { }

    class spell_monk_tiger_strikes_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_tiger_strikes_AuraScript);

        bool Validate(SpellInfo const* /*spell*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MONK_TIGER_STRIKES))
                return false;
            return true;
        }

        void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
        {
            if (Unit* target = eventInfo.GetActionTarget())
            {
                uint32 spellId = (eventInfo.GetTypeMask() & PROC_FLAG_DONE_OFFHAND_ATTACK) ? 120278 : 120274;
                GetUnitOwner()->CastSpell(target, spellId, true);
            }
        }

        void Register()
        {
            OnEffectProc += AuraEffectProcFn(spell_monk_tiger_strikes_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_MOD_MELEE_HASTE_3);
        }
    };


    AuraScript* GetAuraScript() const
    {
        return new spell_monk_tiger_strikes_AuraScript();
    }
};

// Zen Pilgrimage - 126892
class spell_monk_zen_pilgrimage : public SpellScriptLoader
{
public:
    spell_monk_zen_pilgrimage() : SpellScriptLoader("spell_monk_zen_pilgrimage")
    { }

    class spell_monk_zen_pilgrimage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_zen_pilgrimage_SpellScript);

        bool Validate(SpellInfo const* /*spell*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MONK_ZEN_PILGRIMAGE))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Unit* caster = GetCaster())
                if (Player* player = caster->ToPlayer())
                    player->SaveRecallPosition();
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_monk_zen_pilgrimage_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_TELEPORT_UNITS);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_zen_pilgrimage_SpellScript();
    }
};

// Zen Pilgrimage: Return - 126895
class spell_monk_zen_pilgrimage_return : public SpellScriptLoader
{
public:
    spell_monk_zen_pilgrimage_return() : SpellScriptLoader("spell_monk_zen_pilgrimage_return")
    { }

    class spell_monk_zen_pilgrimage_return_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_zen_pilgrimage_return_SpellScript);

        bool Validate(SpellInfo const* /*spell*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MONK_ZEN_PILGRIMAGE_RETURN))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Unit* caster = GetCaster())
                if (Player* player = caster->ToPlayer())
                    player->TeleportTo(player->m_recallMap, player->m_recallX, player->m_recallY, player->m_recallZ, player->m_recallO);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_monk_zen_pilgrimage_return_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_zen_pilgrimage_return_SpellScript();
    }
};

enum Enveloping
{
    SPELL_MONK_ENVELOPING_MIST_HEAL = 132120
};

// Enveloping Mist - 124682
class spell_monk_enveloping_mist : public SpellScriptLoader
{
    public:
    spell_monk_enveloping_mist() : SpellScriptLoader("spell_monk_enveloping_mist")
    { }

    class spell_monk_enveloping_mist_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_enveloping_mist_SpellScript);

        void HandleAfterCast()
        {
            if (Player* _player = GetCaster()->ToPlayer())
                if (Unit* target = GetExplTargetUnit())
                    _player->CastSpell(target, SPELL_MONK_ENVELOPING_MIST_HEAL, true);
        }

        void Register()
        {
            AfterCast += SpellCastFn(spell_monk_enveloping_mist_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_enveloping_mist_SpellScript();
    }
};

enum Surging
{
    SPELL_MONK_SURGING_MIST_HEAL = 116995
};

// Surging Mist - 116694
class spell_monk_surging_mist : public SpellScriptLoader
{
    public:
    spell_monk_surging_mist() : SpellScriptLoader("spell_monk_surging_mist")
    { }

    class spell_monk_surging_mist_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_surging_mist_SpellScript);

        void HandleAfterCast()
        {
            if (Player* _player = GetCaster()->ToPlayer())
                if (Unit* target = GetExplTargetUnit())
                    _player->CastSpell(target, SPELL_MONK_SURGING_MIST_HEAL, true);
        }

        void Register()
        {
            AfterCast += SpellCastFn(spell_monk_surging_mist_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_surging_mist_SpellScript();
    }
};

enum Renewing
{
    CREATURE_SERPENT_ID = 60849,
    SPELL_MONK_GLYPH_OF_RENEWING_MIST = 123334,
    SPELL_MONK_RENEWING_MIST_HOT = 119611,
    SPELL_MONK_RENEWING_MIST_JUMP_AURA = 119607
};

enum Uplift
{
    SPELL_MONK_UPLIFT_ALLOWING_CAST = 123757,
    SPELL_MONK_ITEM_2_S12_MISTWEAVER = 131561
};

enum ManaTea
{
    SPELL_MONK_MANA_TEA_REGEN = 115294,
    SPELL_MONK_MANA_TEA_STACKS = 115867,
    SPELL_MONK_PLUS_ONE_MANA_TEA = 123760
};

/*
// Renewing Mist - 119611
class spell_monk_renewing_mist : public SpellScriptLoader
{
    public:
    spell_monk_renewing_mist() : SpellScriptLoader("spell_monk_renewing_mist")
    { }

    class spell_monk_renewing_mist_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_renewing_mist_SpellScript);

        void HandleOnHit()
        {
            if (Player* _player = GetCaster()->ToPlayer())
            {
                std::list<Unit*> playerList;
                std::list<Creature*> tempList;
                std::list<Creature*> statueList;
                Creature* statue;

                _player->GetPartyMembers(playerList);

                if (playerList.size() > 1)
                {
                    playerList.sort(Skyfire::HealthPctOrderPred());
                    playerList.resize(1);
                }

                _player->GetCreatureListWithEntryInGrid(tempList, CREATURE_SERPENT_ID, 100.0f);
                _player->GetCreatureListWithEntryInGrid(statueList, CREATURE_SERPENT_ID, 100.0f);

                // Remove other players jade statue
                for (std::list<Creature*>::iterator i = tempList.begin(); i != tempList.end(); ++i)
                {
                    Unit* owner = (*i)->GetOwner();
                    if (owner && owner == _player && (*i)->IsSummon())
                        continue;

                    statueList.remove((*i));
                }

                for (auto itr : playerList)
                {
                    if (statueList.size() == 1)
                    {
                        for (auto itrBis : statueList)
                            statue = itrBis;

                        if (statue && (statue->IsPet() || statue->IsGuardian()))
                        {
                            if (statue->GetOwner() && statue->GetOwner()->GetGUID() == _player->GetGUID())
                            {
                                _player->AddAura(SPELL_MONK_RENEWING_MIST_HOT, itr);
                                _player->CastSpell(itr, SPELL_MONK_RENEWING_MIST_JUMP_AURA, true);
                            }
                        }
                    }
                }
            }
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_monk_renewing_mist_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_renewing_mist_SpellScript();
    }

    class spell_monk_renewing_mist_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_renewing_mist_AuraScript);

        uint32 update;
        uint8  spreadCount;

        bool Validate(SpellInfo const* /*spell)
        {
            update = 0;
            spreadCount = 1;

            if (!sSpellMgr->GetSpellInfo(119611))
                return false;
            return true;
        }

        void OnUpdate(uint32 diff, AuraEffect* aurEff)
        {
            update += diff;

            if (update >= 2500)
            {
                if (GetCaster())
                    if (Player* _player = GetCaster()->ToPlayer())
                        _player->CastSpell(_player, SPELL_MONK_UPLIFT_ALLOWING_CAST, true);

                update = 0;
            }
        }

        void OnTick(AuraEffect const* aurEff)
        {
            if (Unit* caster = GetCaster())
            {
                if (Player* _player = caster->ToPlayer())
                {
                    Player* target = GetTarget()->ToPlayer();
                    Unit* newTarget;

                    if (!target)
                        return;

                    if (target->HasAura(SPELL_MONK_RENEWING_MIST_JUMP_AURA, _player->GetGUID()))
                    {
                        if (_player->HasAura(SPELL_MONK_GLYPH_OF_RENEWING_MIST))
                            newTarget = target->GetNextRandomRaidMemberOrPet(40.0f);
                        else
                            newTarget = target->GetNextRandomRaidMemberOrPet(20.0f);

                        if (!newTarget)
                            return;

                        if (Aura* renewingMistJump = target->GetAura(SPELL_MONK_RENEWING_MIST_JUMP_AURA, _player->GetGUID()))
                        {
                            if (renewingMistJump->GetCharges() > 1)
                            {
                                renewingMistJump->DropCharge();

                                // Spreads Renewing Mists 3 times maximum
                                if (Aura* renewingMistHot = target->GetAura(SPELL_MONK_RENEWING_MIST_HOT, _player->GetGUID()))
                                    _player->AddAura(SPELL_MONK_RENEWING_MIST_HOT, newTarget);
                            }
                            else
                            {
                                target->RemoveAura(SPELL_MONK_RENEWING_MIST_JUMP_AURA, _player->GetGUID());

                                // Spreads Renewing Mists 3 times maximum
                                if (Aura* renewingMistHot = target->GetAura(SPELL_MONK_RENEWING_MIST_HOT, _player->GetGUID()))
                                    _player->AddAura(SPELL_MONK_RENEWING_MIST_HOT, newTarget);
                            }
                        }
                    }
                }
            }
        }

        void HandleRemove(AuraEffect const* /*aurEff, AuraEffectHandleModes mode)
        {
            if (GetCaster())
            {
                if (Aura* uplift = GetCaster()->GetAura(SPELL_MONK_UPLIFT_ALLOWING_CAST, GetCaster()->GetGUID()))
                    GetCaster()->RemoveAura(SPELL_MONK_UPLIFT_ALLOWING_CAST, GetCaster()->GetGUID());

                if (GetCaster()->HasAura(SPELL_MONK_ITEM_2_S12_MISTWEAVER))
                {
                    AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
                    if (removeMode == AURA_REMOVE_BY_EXPIRE)
                    {
                        GetCaster()->CastSpell(GetCaster(), SPELL_MONK_MANA_TEA_STACKS, true);
                        GetCaster()->CastSpell(GetCaster(), SPELL_MONK_PLUS_ONE_MANA_TEA, true);
                    }
                }
            }
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_monk_renewing_mist_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
            OnEffectUpdatePeriodic += AuraEffectUpdatePeriodicFn(spell_monk_renewing_mist_AuraScript::Update, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
            OnEffectRemove += AuraEffectApplyFn(spell_monk_renewing_mist_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_monk_renewing_mist_AuraScript();
    }
};

// Healing Sphere 115460
class spell_monk_healing_sphere : public SpellScriptLoader
{
    public:
    spell_monk_healing_sphere() : SpellScriptLoader("spell_monk_healing_sphere")
    { }

    class spell_monk_healing_sphere_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_healing_sphere_SpellScript);

        void Sphere(SpellEffIndex /*eff)
        {
            int32 count = GetCaster()->CountAreaTrigger(SPELL_MONK_HEALING_SPHERE);
            if (count >= 3)
            {
                std::list<AreaTrigger*> sphereList;
                GetCaster()->GetAreaTriggerList(sphereList, SPELL_MONK_HEALING_SPHERE);
                if (!sphereList.empty())
                {
                    sphereList.sort(Skyfire::AreaTriggerDurationPctOrderPred());
                    for (auto itr : sphereList)
                    {
                        AreaTrigger* sphere = itr;
                        sphere->SetDuration(0);
                        break;
                    }
                }
            }
        }

        void Register()
        {
            OnEffectHit += SpellEffectFn(spell_monk_healing_sphere_SpellScript::Sphere, EFFECT_0, SPELL_EFFECT_CREATE_AREATRIGGER);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_healing_sphere_SpellScript();
    }
};
*/
/*######
## npc_transcendence_spirit -- 54569
######*/
/*
enum TranscendenceSpiritSpells
{
    SPELL_SPAWN_TRANS = 102141,
    SPELL_INITIALIZE_IMAGES = 102284,
    SPELL_CLONE_CASTER = 119051,
    SPELL_VISUAL_SPIRIT = 119053,
    SPELL_MEDITATE = 124416,
    SPELL_ROOT_FOR_EVER = 31366
};

enum transcendenceActions
{
    ACTION_TELEPORT = 1
};

class npc_transcendence_spirit : public CreatureScript
{
    public:
    npc_transcendence_spirit() : CreatureScript("npc_transcendence_spirit")
    { }

    struct npc_transcendence_spiritAI : public ScriptedAI
    {
        npc_transcendence_spiritAI(Creature* c) : ScriptedAI(c)
        {
            me->SetReactState(REACT_PASSIVE);
        }

        void JustDied(Unit* who) override
        {
            if (me->GetOwner() && me->GetOwner()->GetTypeId() == TYPEID_PLAYER)
                me->GetOwner()->ToPlayer()->SetTransSpirit(NULL);
        }

        void EnterCombat(Unit* /*who*) override
        { }
        void AttackStart(Unit* /*who*) override
        { }

        void Reset() override
        {
            if (!me->HasAura(SPELL_MEDITATE))
                me->AddAura(SPELL_MEDITATE, me);
        }

        void CorpseRemoved(uint32& /*respawnDelay*) override
        {
            if (me->GetOwner() && me->GetOwner()->GetTypeId() == TYPEID_PLAYER)
                me->GetOwner()->ToPlayer()->SetTransSpirit(NULL);
        }

        void IsSummonedBy(Unit* owner) override
        {
            if (owner && owner->GetTypeId() == TYPEID_PLAYER)
            {
                owner->ToPlayer()->SetTransSpirit(me);
            }
            else
                me->DespawnOrUnsummon();
        }
    };

    CreatureAI* GetAI(Creature *creature) const
    {
        return new npc_transcendence_spiritAI(creature);
    }
};

// 102141 - Transcendence
class spell_monk_transcendence : public SpellScriptLoader
{
    public:
    spell_monk_transcendence() : SpellScriptLoader("spell_monk_transcendence")
    { }

    class spell_monk_transcendence_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_transcendence_SpellScript);

        void Despawn()
        {
            if (Player* player = GetCaster()->ToPlayer())
                if (Creature* spirit = player->GetTransSpirit())
                    spirit->DespawnOrUnsummon();
        }

        void Visualize()
        {
            if (Player* player = GetCaster()->ToPlayer())
            {
                if (Creature* spirit = player->GetTransSpirit())
                {
                    player->CastSpell(spirit, SPELL_CLONE_CASTER);
                    spirit->AddAura(SPELL_VISUAL_SPIRIT, spirit);
                    spirit->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    spirit->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1);
                    spirit->CastSpell(spirit, SPELL_MEDITATE);
                    spirit->AddAura(SPELL_ROOT_FOR_EVER, spirit);
                }
            }
        }

        void Register()
        {
            BeforeCast += SpellCastFn(spell_monk_transcendence_SpellScript::Despawn);
            AfterCast += SpellCastFn(spell_monk_transcendence_SpellScript::Visualize);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_transcendence_SpellScript();
    }
};

// 119996 - Transcendence Transfer
class spell_monk_transcendence_transfer : public SpellScriptLoader
{
    public:
    spell_monk_transcendence_transfer() : SpellScriptLoader("spell_monk_transcendence_transfer")
    { }

    class spell_monk_transcendence_transfer_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_transcendence_transfer_SpellScript);

        SpellCastResult CheckCast()
        {
            if (GetCaster() && GetCaster()->ToPlayer())
            {
                Player* player = GetCaster()->ToPlayer();
                if (Creature* spirit = player->GetTransSpirit())
                {
                    float distance = 40.0f;

                    if (player->GetExactDist2d(spirit->GetPositionX(), spirit->GetPositionY()) <= distance)
                        return SpellCastResult::SPELL_CAST_OK;
                    else if (player->GetExactDist2d(spirit->GetPositionX(), spirit->GetPositionY()) >= distance)
                        return SPELL_FAILED_OUT_OF_RANGE;
                }
                else
                    return SPELL_FAILED_NO_PET;
            }

            return SPELL_FAILED_DONT_REPORT;
        }

        void Cast()
        {
            if (GetCaster() && GetCaster()->ToPlayer())
            {
                Player* player = GetCaster()->ToPlayer();

                if (Creature* spirit = player->GetTransSpirit())
                {
                    float distance = 40.0f;
                    if (player->GetExactDist2d(spirit->GetPositionX(), spirit->GetPositionY()) <= distance)
                    {
                        float petX, petY, petZ;
                        uint32 petMapId;
                        spirit->GetPosition(petX, petY, petZ);
                        petMapId = spirit->GetMapId();

                        spirit->DespawnOrUnsummon();
                        player->TeleportTo(petMapId, petX, petY, petZ, 0.0f);
                        player->CastSpell(player, SPELL_SPAWN_TRANS);
                    }
                }
            }
        }

        void Register()
        {
            OnCast += SpellCastFn(spell_monk_transcendence_transfer_SpellScript::Cast);
            OnCheckCast += SpellCheckCastFn(spell_monk_transcendence_transfer_SpellScript::CheckCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_transcendence_transfer_SpellScript();
    }
};
*/



struct auraData
{
    auraData(uint32 id, uint64 casterGUID) : m_id(id), m_casterGuid(casterGUID)
    { }
    uint32 m_id;
    uint64 m_casterGuid;
};

// Diffuse Magic - 122783
class spell_monk_diffuse_magic : public SpellScriptLoader
{
    public:
    spell_monk_diffuse_magic() : SpellScriptLoader("spell_monk_diffuse_magic")
    { }

    class spell_monk_diffuse_magic_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_diffuse_magic_SpellScript);

        void HandleOnHit()
        {
            if (Player* _player = GetCaster()->ToPlayer())
            {
                std::set<auraData*> auraListToRemove;
                Unit::AuraApplicationMap AuraList = _player->GetAppliedAuras();
                for (Unit::AuraApplicationMap::iterator iter = AuraList.begin(); iter != AuraList.end(); ++iter)
                {
                    Aura* aura = iter->second->GetBase();
                    if (!aura)
                        continue;

                    Unit* caster = aura->GetCaster();
                    if (!caster || caster->GetGUID() == _player->GetGUID())
                        continue;

                    if (!caster->IsWithinDist(_player, 40.0f))
                        continue;

                    if (aura->GetSpellInfo()->IsPositive())
                        continue;

                    if (!(aura->GetSpellInfo()->GetSchoolMask() & SPELL_SCHOOL_MASK_MAGIC))
                        continue;

                    _player->AddAura(aura->GetSpellInfo()->Id, caster);

                    if (Aura* targetAura = caster->GetAura(aura->GetSpellInfo()->Id, _player->GetGUID()))
                    {
                        for (int i = 0; i < MAX_SPELL_EFFECTS; ++i)
                        {
                            if (targetAura->GetEffect(i) && aura->GetEffect(i))
                            {
                                AuraEffect* auraEffect = _player->GetAuraEffect(aura->GetSpellInfo()->Id, i);
                                if (!auraEffect)
                                    continue;

                                int32 damage = auraEffect->GetAmount();

                                // need to check auraEffect pointer here !!
                                if (auraEffect->GetAuraType() == SPELL_AURA_PERIODIC_DAMAGE ||
                                    auraEffect->GetAuraType() == SPELL_AURA_PERIODIC_DAMAGE_PERCENT)
                                    damage = caster->SpellDamageBonusDone(_player, aura->GetSpellInfo(), damage, DOT, auraEffect->GetBase()->GetStackAmount());

                                targetAura->GetEffect(i)->SetAmount(damage);
                            }
                        }
                    }

                    auraListToRemove.insert(new auraData(aura->GetSpellInfo()->Id, caster->GetGUID()));
                }

                for (std::set<auraData*>::iterator itr = auraListToRemove.begin(); itr != auraListToRemove.end(); ++itr)
                {
                    _player->RemoveAura((*itr)->m_id, (*itr)->m_casterGuid);
                    delete (*itr);
                }

                auraListToRemove.clear();
            }
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_monk_diffuse_magic_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_diffuse_magic_SpellScript();
    }
};

enum BlackOx // tehee
{
    SPELL_MONK_GUARD = 115295
};

// Guard - 115295
class spell_monk_guard : public SpellScriptLoader
{
    public:
    spell_monk_guard() : SpellScriptLoader("spell_monk_guard")
    { }

    class spell_monk_guard_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_guard_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
        {
            if (!GetCaster())
                return;

            if (Player* _plr = GetCaster()->ToPlayer())
                amount += int32(_plr->GetTotalAttackPowerValue(WeaponAttackType::BASE_ATTACK) * 1.971f);
            else if (GetCaster()->GetOwner()) // For Black Ox Statue
            {
                if (Player* _plr = GetCaster()->GetOwner()->ToPlayer())
                    amount += int32(_plr->GetTotalAttackPowerValue(WeaponAttackType::BASE_ATTACK) * 1.971f);
            }
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_guard_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_monk_guard_AuraScript();
    }
};

enum Zen
{
    SPELL_MONK_ZEN_FLIGHT = 125883
};

// Glyph of Zen Flight - 125893
class spell_monk_glyph_of_zen_flight : public SpellScriptLoader
{
    public:
    spell_monk_glyph_of_zen_flight() : SpellScriptLoader("spell_monk_glyph_of_zen_flight")
    { }

    class spell_monk_glyph_of_zen_flight_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_glyph_of_zen_flight_AuraScript);

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* _player = GetTarget()->ToPlayer())
                _player->learnSpell(SPELL_MONK_ZEN_FLIGHT, false);
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* _player = GetTarget()->ToPlayer())
                if (_player->HasSpell(SPELL_MONK_ZEN_FLIGHT))
                    _player->removeSpell(SPELL_MONK_ZEN_FLIGHT, false, false);
        }

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_monk_glyph_of_zen_flight_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_monk_glyph_of_zen_flight_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_monk_glyph_of_zen_flight_AuraScript();
    }
};

enum Serpent
{
    MONK_NPC_JADE_SERPENT_STATUE = 60849
};



/*
enum Fists
{
    SPELL_MONK_FISTS_OF_FURY_VISUAL = 123154
};

// Fists of Fury (stun effect) - 120086
class spell_monk_fists_of_fury_stun : public SpellScriptLoader
{
    public:
    spell_monk_fists_of_fury_stun() : SpellScriptLoader("spell_monk_fists_of_fury_stun")
    { }

    class spell_monk_fists_of_fury_stun_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_fists_of_fury_stun_SpellScript);

        void RemoveInvalidTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if(Skyfire::UnitAuraCheck(true, GetSpellInfo()->Id));
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_fists_of_fury_stun_SpellScript::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_24);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_fists_of_fury_stun_SpellScript();
    }

    class spell_monk_fists_of_fury_stun_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_fists_of_fury_stun_AuraScript);

        void OnRemove(AuraEffect const* /*aurEff*, AuraEffectHandleModes /*mode*)
        {
            if (Player* _player = GetTarget()->ToPlayer())
                if (_player->HasAura(SPELL_MONK_FISTS_OF_FURY_VISUAL))
                    _player->RemoveAura(SPELL_MONK_FISTS_OF_FURY_VISUAL);
        }

        void Register()
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_monk_fists_of_fury_stun_AuraScript::OnRemove, EFFECT_0, SPELL_EFFECT_APPLY_AURA, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_monk_fists_of_fury_stun_AuraScript();
    }
};
*/
enum Grapple
{
    SPELL_MONK_GRAPPLE_WEAPON_TANK_UPGRADE = 123232,
    SPELL_MONK_GRAPPLE_WEAPON_HEAL_UPGRADE = 123234,
    SPELL_MONK_GRAPPLE_WEAPON_DPS_UPGRADE = 123231
};

// Grapple Weapon - 117368
class spell_monk_grapple_weapon : public SpellScriptLoader
{
    public:
    spell_monk_grapple_weapon() : SpellScriptLoader("spell_monk_grapple_weapon")
    { }

    class spell_monk_grapple_weapon_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_grapple_weapon_SpellScript)

            void HandleBeforeHit()
        {
            if (Player* _player = GetCaster()->ToPlayer())
            {
                if (Unit* target = GetHitUnit())
                {
                    if (target->ToPlayer())
                    {
                        Item* mainItem = _player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
                        Item* targetMainItem = target->ToPlayer()->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);

                        if (mainItem && targetMainItem)
                        {
                            if (targetMainItem->GetTemplate()->ItemLevel > mainItem->GetTemplate()->ItemLevel)
                            {
                                switch (_player->GetTalentSpecialization(_player->GetActiveSpec()))
                                {
                                    case SPEC_MONK_BREWMASTER:
                                        _player->CastSpell(_player, SPELL_MONK_GRAPPLE_WEAPON_TANK_UPGRADE, true);
                                        break;
                                    case SPEC_MONK_MISTWEAVER:
                                        _player->CastSpell(_player, SPELL_MONK_GRAPPLE_WEAPON_HEAL_UPGRADE, true);
                                        break;
                                    case SPEC_MONK_WINDWALKER:
                                        _player->CastSpell(_player, SPELL_MONK_GRAPPLE_WEAPON_DPS_UPGRADE, true);
                                        break;
                                    default:
                                        break;
                                }
                            }
                        }
                    }
                    else if (target->GetTypeId() == TypeID::TYPEID_UNIT)
                    {
                        if (target->getLevel() > _player->getLevel())
                        {
                            switch (_player->GetTalentSpecialization(_player->GetActiveSpec()))
                            {
                                case SPEC_MONK_BREWMASTER:
                                    _player->CastSpell(_player, SPELL_MONK_GRAPPLE_WEAPON_TANK_UPGRADE, true);
                                    break;
                                case SPEC_MONK_MISTWEAVER:
                                    _player->CastSpell(_player, SPELL_MONK_GRAPPLE_WEAPON_HEAL_UPGRADE, true);
                                    break;
                                case SPEC_MONK_WINDWALKER:
                                    _player->CastSpell(_player, SPELL_MONK_GRAPPLE_WEAPON_DPS_UPGRADE, true);
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                }
            }
        }

        void Register()
        {
            BeforeHit += SpellHitFn(spell_monk_grapple_weapon_SpellScript::HandleBeforeHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_grapple_weapon_SpellScript();
    }
};

enum SerpentZeal
{
    SPELL_MONK_EMINENCE_HEAL = 126890
};

// Serpent's Zeal - 127722
class spell_monk_serpents_zeal : public SpellScriptLoader
{
    public:
    spell_monk_serpents_zeal() : SpellScriptLoader("spell_monk_serpents_zeal")
    { }

    class spell_monk_serpents_zeal_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_serpents_zeal_AuraScript);

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            if (!GetCaster())
                return;

            if (eventInfo.GetActor()->GetGUID() != GetTarget()->GetGUID())
                return;

            if (eventInfo.GetDamageInfo()->GetSpellInfo())
                return;

            if (!(eventInfo.GetDamageInfo()->GetDamage()))
                return;

            if (eventInfo.GetDamageInfo()->GetAttackType() != WeaponAttackType::BASE_ATTACK && eventInfo.GetDamageInfo()->GetAttackType() != WeaponAttackType::OFF_ATTACK)
                return;

            int32 bp = eventInfo.GetDamageInfo()->GetDamage();

            if (Player* _player = GetCaster()->ToPlayer())
            {
                std::list<Creature*> tempList;
                std::list<Creature*> statueList;
                Creature* statue = NULL;

                if (Aura* serpentsZeal = _player->GetAura(aurEff->GetSpellInfo()->Id))
                {
                    if (serpentsZeal->GetStackAmount() < 2)
                        bp /= 4;
                    else
                        bp /= 2;
                }

                _player->GetCreatureListWithEntryInGrid(tempList, MONK_NPC_JADE_SERPENT_STATUE, 100.0f);
                _player->GetCreatureListWithEntryInGrid(statueList, MONK_NPC_JADE_SERPENT_STATUE, 100.0f);

                // Remove other players jade statue
                for (std::list<Creature*>::iterator i = tempList.begin(); i != tempList.end(); ++i)
                {
                    Unit* owner = (*i)->GetOwner();
                    if (owner && owner == _player && (*i)->IsSummon())
                        continue;

                    statueList.remove((*i));
                }

                // you gain Serpent's Zeal causing you to heal nearby injured targets equal to 25% of your auto-attack damage. Stacks up to 2 times.
                _player->CastCustomSpell(_player, SPELL_MONK_EMINENCE_HEAL, &bp, NULL, NULL, true);

                if (statueList.size() == 1)
                {
                    for (auto itrBis : statueList)
                        statue = itrBis;

                    if (statue && (statue->IsPet() || statue->IsGuardian()))
                        if (statue->GetOwner() && statue->GetOwner()->GetGUID() == _player->GetGUID())
                            statue->CastCustomSpell(statue, SPELL_MONK_EMINENCE_HEAL, &bp, NULL, NULL, true, 0, NULL, _player->GetGUID()); // Eminence - statue
                }
            }
        }

        void Register()
        {
            OnEffectProc += AuraEffectProcFn(spell_monk_serpents_zeal_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_monk_serpents_zeal_AuraScript();
    }
};



enum bearHug
{
    SPELL_MONK_BEAR_HUG = 127361
};

// Bear Hug - 127361
class spell_monk_bear_hug : public SpellScriptLoader
{
    public:
    spell_monk_bear_hug() : SpellScriptLoader("spell_monk_bear_hug")
    { }

    class spell_monk_bear_hug_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_bear_hug_SpellScript);

        void HandleOnHit()
        {
            if (Player* _player = GetCaster()->ToPlayer())
                if (Unit* target = GetHitUnit())
                    if (Aura* bearHug = target->GetAura(SPELL_MONK_BEAR_HUG, _player->GetGUID()))
                        if (bearHug->GetEffect(EFFECT_1))
                            bearHug->GetEffect(EFFECT_1)->SetAmount(_player->CountPctFromMaxHealth(2));
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_monk_bear_hug_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_bear_hug_SpellScript();
    }
};

// Zen Flight - 125883
class spell_monk_zen_flight_check : public SpellScriptLoader
{
    public:
    spell_monk_zen_flight_check() : SpellScriptLoader("spell_monk_zen_flight_check")
    { }

    class spell_monk_zen_flight_check_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_zen_flight_check_SpellScript);

        SpellCastResult CheckTarget()
        {
            if (Player* _player = GetCaster()->ToPlayer())
            {
                if (_player->GetMap()->IsBattlegroundOrArena())
                    return SpellCastResult::SPELL_FAILED_NOT_IN_BATTLEGROUND;

                // In Kalimdor or Eastern Kingdom with Flight Master's License
                if (!_player->HasSpell(90267) && (_player->GetMapId() == 1 || _player->GetMapId() == 0))
                    return SpellCastResult::SPELL_FAILED_NOT_HERE;

                // In Pandaria with Wisdom of the Four Winds
                if (!_player->HasSpell(115913) && (_player->GetMapId() == 870))
                    return SpellCastResult::SPELL_FAILED_NOT_HERE;
            }

            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register()
        {
            OnCheckCast += SpellCheckCastFn(spell_monk_zen_flight_check_SpellScript::CheckTarget);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_zen_flight_check_SpellScript();
    }

    class spell_monk_zen_flight_check_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_zen_flight_check_AuraScript);

        bool Load()
        {
            return GetCaster() && GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
        {
            if (!GetCaster())
                return;

            if (Player* caster = GetCaster()->ToPlayer())
                if (caster->GetSkillValue(SKILL_RIDING) >= 375)
                    amount = 310;
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_zen_flight_check_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_INCREASE_VEHICLE_FLIGHT_SPEED);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_monk_zen_flight_check_AuraScript();
    }
};

enum PowerStrikes
{
    SPELL_MONK_POWER_STRIKES_TALENT = 121817,
    SPELL_MONK_CREATE_CHI_SPHERE = 121286
};

// Called by Jab - 100780
// Power Strikes - 121817
class spell_monk_power_strikes : public SpellScriptLoader
{
    public:
    spell_monk_power_strikes() : SpellScriptLoader("spell_monk_power_strikes")
    { }

    class spell_monk_power_strikes_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_power_strikes_SpellScript)

            void HandleOnHit()
        {
            if (Player* _player = GetCaster()->ToPlayer())
            {
                if (Unit* target = GetHitUnit())
                {
                    if (target->GetGUID() != _player->GetGUID())
                    {
                        if (_player->HasAura(SPELL_MONK_POWER_STRIKES_TALENT))
                        {
                            if (!_player->HasSpellCooldown(SPELL_MONK_POWER_STRIKES_TALENT))
                            {
                                if (_player->GetPower(POWER_CHI) < _player->GetMaxPower(POWER_CHI))
                                {
                                    _player->EnergizeBySpell(_player, GetSpellInfo()->Id, 1, POWER_CHI);
                                    _player->AddSpellCooldown(SPELL_MONK_POWER_STRIKES_TALENT, 0, time(NULL) + 20);
                                }
                                else
                                    _player->CastSpell(_player, SPELL_MONK_CREATE_CHI_SPHERE, true);
                            }
                        }
                    }
                }
            }
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_monk_power_strikes_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_power_strikes_SpellScript();
    }
};

enum SpinningFireBlossom
{
    SPELL_MONK_SPINNING_FIRE_BLOSSOM_ROOT = 123407,
    SPELL_MONK_SPINNING_FIRE_BLOSSOM_DAMAGE = 123408,
    SPELL_MONK_SPINNING_FIRE_BLOSSOM_MISSILE = 118852
};

// Spinning Fire Blossom - 123408
class spell_monk_spinning_fire_blossom_damage : public SpellScriptLoader
{
    public:
    spell_monk_spinning_fire_blossom_damage() : SpellScriptLoader("spell_monk_spinning_fire_blossom_damage")
    { }

    class spell_monk_spinning_fire_blossom_damage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_spinning_fire_blossom_damage_SpellScript);

        SpellCastResult CheckTarget()
        {
            if (Player* _player = GetCaster()->ToPlayer())
                if (Unit* target = GetExplTargetUnit())
                    if (_player->IsFriendlyTo(target))
                        return SpellCastResult::SPELL_FAILED_BAD_TARGETS;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void HandleAfterHit()
        {
            if (Player* _player = GetCaster()->ToPlayer())
            {
                if (Unit* target = GetHitUnit())
                {
                    if (target->GetDistance(_player) > 10.0f)
                    {
                        SetHitDamage(int32(GetHitDamage() * 1.5f));
                        _player->CastSpell(target, SPELL_MONK_SPINNING_FIRE_BLOSSOM_ROOT, true);
                    }
                }
            }
        }

        void Register()
        {
            OnCheckCast += SpellCheckCastFn(spell_monk_spinning_fire_blossom_damage_SpellScript::CheckTarget);
            AfterHit += SpellHitFn(spell_monk_spinning_fire_blossom_damage_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_spinning_fire_blossom_damage_SpellScript();
    }
};

// Spinning Fire Blossom - 115073
class spell_monk_spinning_fire_blossom : public SpellScriptLoader
{
    public:
    spell_monk_spinning_fire_blossom() : SpellScriptLoader("spell_monk_spinning_fire_blossom")
    { }

    class spell_monk_spinning_fire_blossom_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_spinning_fire_blossom_SpellScript)

            void HandleAfterCast()
        {
            if (Player* _player = GetCaster()->ToPlayer())
            {
                std::list<Unit*> tempList;
                std::list<Unit*> targetList;

                _player->GetAttackableUnitListInRange(tempList, 50.0f);

                for (auto itr : tempList)
                {
                    if (!_player->IsValidAttackTarget(itr))
                        continue;

                    if (!_player->isInFront(itr))
                        continue;

                    if (!itr->IsWithinLOSInMap(_player))
                        continue;

                    if (itr->GetGUID() == _player->GetGUID())
                        continue;

                    targetList.push_back(itr);
                }

                if (!targetList.empty())
                {
                    Skyfire::Containers::RandomResizeList(targetList, 1);

                    for (auto itr : targetList)
                        _player->CastSpell(itr, SPELL_MONK_SPINNING_FIRE_BLOSSOM_DAMAGE, true);
                }
                else
                    _player->CastSpell(_player, SPELL_MONK_SPINNING_FIRE_BLOSSOM_MISSILE, true);
            }
        }

        void Register()
        {
            AfterCast += SpellCastFn(spell_monk_spinning_fire_blossom_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_spinning_fire_blossom_SpellScript();
    }
};
/*
enum ThunderTea
{
    SPELL_MONK_THUNDER_FOCUS_TEA = 116680
};


// Called by Uplift - 116670 and Uplift - 130316
// Thunder Focus Tea - 116680
class spell_monk_thunder_focus_tea : public SpellScriptLoader
{
    public:
    spell_monk_thunder_focus_tea() : SpellScriptLoader("spell_monk_thunder_focus_tea")
    { }

    class spell_monk_thunder_focus_tea_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_thunder_focus_tea_SpellScript)

            void FilterTargets(std::list<WorldObject*>& unitList)
        {
            unitList.remove_if(Skyfire::UnitAuraCheck(false, SPELL_MONK_RENEWING_MIST_HOT, GetCaster()->GetGUID()));
        }

        void HandleOnHit()
        {
            if (Player* _player = GetCaster()->ToPlayer())
            {
                if (Unit* target = GetHitUnit())
                {
                    if (_player->HasAura(SPELL_MONK_THUNDER_FOCUS_TEA))
                    {
                        std::list<Unit*> groupList;

                        _player->GetPartyMembers(groupList);

                        for (auto itr : groupList)
                            if (Aura* renewingMistGroup = itr->GetAura(SPELL_MONK_RENEWING_MIST_HOT, _player->GetGUID()))
                                renewingMistGroup->RefreshDuration();

                        _player->RemoveAura(SPELL_MONK_THUNDER_FOCUS_TEA);
                    }
                }
            }
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_thunder_focus_tea_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_CASTER_AREA_RAID);
            OnHit += SpellHitFn(spell_monk_thunder_focus_tea_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_thunder_focus_tea_SpellScript();
    }
};
*/
enum Teachings
{
    SPELL_MONK_SPINNING_CRANE_KICK_HEAL = 117640
};

// Called by Spinning Crane Kick - 107270
// Teachings of the Monastery - 116645
class spell_monk_teachings_of_the_monastery : public SpellScriptLoader
{
    public:
    spell_monk_teachings_of_the_monastery() : SpellScriptLoader("spell_monk_teachings_of_the_monastery")
    { }

    class spell_monk_teachings_of_the_monastery_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_teachings_of_the_monastery_SpellScript);

        void HandleAfterCast()
        {
            if (GetCaster())
                if (GetCaster()->HasAura(118672))
                    GetCaster()->CastSpell(GetCaster(), SPELL_MONK_SPINNING_CRANE_KICK_HEAL, true);
        }

        void Register()
        {
            AfterCast += SpellCastFn(spell_monk_teachings_of_the_monastery_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_teachings_of_the_monastery_SpellScript();
    }
};

// Glyph of Mana Tea - 115294
class spell_monk_glyph_of_mana_tea : public SpellScriptLoader
{
    public:
    spell_monk_glyph_of_mana_tea() : SpellScriptLoader("spell_monk_glyph_of_mana_tea")
    { }

    class spell_monk_glyph_of_mana_tea_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_glyph_of_mana_tea_SpellScript);

        SpellModifier* spellMod;

        void HandleBeforeCast()
        {
            if (Player* _player = GetCaster()->ToPlayer())
            {
                int32 stacks = 0;

                if (Aura* manaTeaStacks = _player->GetAura(SPELL_MONK_MANA_TEA_STACKS))
                    stacks = manaTeaStacks->GetStackAmount();

                int32 newDuration = stacks * IN_MILLISECONDS;

                spellMod = new SpellModifier();
                spellMod->op = SPELLMOD_DURATION;
                spellMod->type = SPELLMOD_FLAT;
                spellMod->spellId = SPELL_MONK_MANA_TEA_STACKS;
                spellMod->value = newDuration;
                spellMod->mask [1] = 0x200000;
                spellMod->mask [2] = 0x1;

                _player->AddSpellMod(spellMod, true);
            }
        }

        void HandleOnHit()
        {
            if (GetCaster())
            {
                if (Aura* manaTea = GetCaster()->GetAura(SPELL_MONK_MANA_TEA_STACKS))
                {
                    if (manaTea->GetStackAmount() > 1)
                        manaTea->SetStackAmount(manaTea->GetStackAmount() - 1);
                    else
                        GetCaster()->RemoveAura(SPELL_MONK_MANA_TEA_STACKS);
                }
            }
        }

        void HandleAfterCast()
        {
            if (Player* _player = GetCaster()->ToPlayer())
                _player->AddSpellMod(spellMod, false);
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_monk_glyph_of_mana_tea_SpellScript::HandleOnHit);
            BeforeCast += SpellCastFn(spell_monk_glyph_of_mana_tea_SpellScript::HandleBeforeCast);
            AfterCast += SpellCastFn(spell_monk_glyph_of_mana_tea_SpellScript::HandleAfterCast);
        }
    };
};

// Mana Tea - 115294
class spell_monk_mana_tea : public SpellScriptLoader
{
    public:
    spell_monk_mana_tea() : SpellScriptLoader("spell_monk_mana_tea")
    { }

    class spell_monk_mana_tea_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_mana_tea_SpellScript);

        SpellModifier* spellMod;

        void HandleBeforeCast()
        {
            if (Player* _player = GetCaster()->ToPlayer())
            {
                int32 stacks = 0;

                if (Aura* manaTeaStacks = _player->GetAura(SPELL_MONK_MANA_TEA_STACKS))
                    stacks = manaTeaStacks->GetStackAmount();

                int32 newDuration = stacks * IN_MILLISECONDS;

                spellMod = new SpellModifier();
                spellMod->op = SPELLMOD_DURATION;
                spellMod->type = SPELLMOD_FLAT;
                spellMod->spellId = SPELL_MONK_MANA_TEA_REGEN;
                spellMod->value = newDuration;
                spellMod->mask [1] = 0x200000;
                spellMod->mask [2] = 0x1;

                _player->AddSpellMod(spellMod, true);
            }
        }

        void HandleAfterCast()
        {
            if (Player* _player = GetCaster()->ToPlayer())
                _player->AddSpellMod(spellMod, false);
        }

        void Register()
        {
            BeforeCast += SpellCastFn(spell_monk_mana_tea_SpellScript::HandleBeforeCast);
            AfterCast += SpellCastFn(spell_monk_mana_tea_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_mana_tea_SpellScript();
    }

    class spell_monk_mana_tea_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_mana_tea_AuraScript);

        void OnTick(AuraEffect const* aurEff)
        {
            if (GetCaster())
            {
                // remove one charge per tick instead of remove aura on cast
                // "Cancelling the channel will not waste stacks"
                if (Aura* manaTea = GetCaster()->GetAura(SPELL_MONK_MANA_TEA_STACKS))
                {
                    if (manaTea->GetStackAmount() > 1)
                        manaTea->SetStackAmount(manaTea->GetStackAmount() - 1);
                    else
                        GetCaster()->RemoveAura(SPELL_MONK_MANA_TEA_STACKS);
                }
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_monk_mana_tea_AuraScript::OnTick, EFFECT_0, SPELL_AURA_OBS_MOD_POWER);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_monk_mana_tea_AuraScript();
    }
};

// Brewing : Mana Tea - 123766
class spell_monk_mana_tea_stacks : public SpellScriptLoader
{
    public:
    spell_monk_mana_tea_stacks() : SpellScriptLoader("spell_monk_mana_tea_stacks")
    { }

    class spell_monk_mana_tea_stacks_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_mana_tea_stacks_AuraScript);

        uint32 chiConsumed;

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            chiConsumed = 0;
        }

        void SetData(uint32 type, uint32 data)
        {
            while ((chiConsumed += data) >= 4)
            {
                chiConsumed = 0;
                data = data > 4 ? data - 4 : 0;

                if (GetCaster())
                {
                    GetCaster()->CastSpell(GetCaster(), SPELL_MONK_MANA_TEA_STACKS, true);
                    GetCaster()->CastSpell(GetCaster(), SPELL_MONK_PLUS_ONE_MANA_TEA, true);
                }
            }
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_monk_mana_tea_stacks_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_monk_mana_tea_stacks_AuraScript();
    }
};

// Called by : Fortifying Brew - 115203, Chi Brew - 115399, Elusive Brew - 115308, Tigereye Brew - 116740
// Purifying Brew - 119582, Mana Tea - 115294, Thunder Focus Tea - 116680 and Energizing Brew - 115288
// Healing Elixirs - 122280
class spell_monk_healing_elixirs : public SpellScriptLoader
{
    public:
    spell_monk_healing_elixirs() : SpellScriptLoader("spell_monk_healing_elixirs")
    { }

    class spell_monk_healing_elixirs_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_healing_elixirs_SpellScript);

        void HandleOnHit()
        {
            if (Player* _player = GetCaster()->ToPlayer())
            {
                if (_player->HasAura(SPELL_MONK_HEALING_ELIXIRS_AURA))
                {
                    int32 bp = 10;

                    if (!_player->HasSpellCooldown(SPELL_MONK_HEALING_ELIXIRS_RESTORE_HEALTH))
                    {
                        _player->CastCustomSpell(_player, SPELL_MONK_HEALING_ELIXIRS_RESTORE_HEALTH, &bp, NULL, NULL, true);
                        // This effect cannot occur more than once per 18s
                        _player->AddSpellCooldown(SPELL_MONK_HEALING_ELIXIRS_RESTORE_HEALTH, 0, time(NULL) + 18);
                    }
                }
            }
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_monk_healing_elixirs_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_healing_elixirs_SpellScript();
    }
};

// Zen Sphere - 124081
class spell_monk_zen_sphere : public SpellScriptLoader
{
    public:
    spell_monk_zen_sphere() : SpellScriptLoader("spell_monk_zen_sphere")
    { }

    class spell_monk_zen_sphere_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_zen_sphere_SpellScript);

        bool active;

        void HandleBeforeHit()
        {
            active = false;

            if (Player* _player = GetCaster()->ToPlayer())
                if (Unit* target = GetHitUnit())
                    if (target->HasAura(SPELL_MONK_ZEN_SPHERE_HEAL))
                        active = true;
        }

        void HandleAfterHit()
        {
            if (Player* _player = GetCaster()->ToPlayer())
            {
                if (Unit* target = GetHitUnit())
                {
                    if (active)
                    {
                        _player->CastSpell(_player, SPELL_MONK_ZEN_SPHERE_DETONATE_HEAL, true);
                        _player->CastSpell(_player, SPELL_MONK_ZEN_SPHERE_DETONATE_DAMAGE, true);
                        _player->RemoveAura(SPELL_MONK_ZEN_SPHERE_HEAL);
                        active = false;
                    }
                }
            }
        }

        void Register()
        {
            BeforeHit += SpellHitFn(spell_monk_zen_sphere_SpellScript::HandleBeforeHit);
            AfterHit += SpellHitFn(spell_monk_zen_sphere_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_zen_sphere_SpellScript();
    }
};

// Zen Sphere - 124081
class spell_monk_zen_sphere_hot : public SpellScriptLoader
{
    public:
    spell_monk_zen_sphere_hot() : SpellScriptLoader("spell_monk_zen_sphere_hot")
    { }

    class spell_monk_zen_sphere_hot_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_zen_sphere_hot_AuraScript);

        void OnTick(AuraEffect const* aurEff)
        {
            if (Player* _player = GetCaster()->ToPlayer())
                _player->CastSpell(_player, SPELL_MONK_ZEN_SPHERE_DAMAGE, true);
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_monk_zen_sphere_hot_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_monk_zen_sphere_hot_AuraScript();
    }
};

// Chi Burst - 123986
class spell_monk_chi_burst : public SpellScriptLoader
{
    public:
    spell_monk_chi_burst() : SpellScriptLoader("spell_monk_chi_burst")
    { }

    class spell_monk_chi_burst_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_chi_burst_SpellScript);

        void HandleOnHit()
        {
            if (Player* _player = GetCaster()->ToPlayer())
            {
                if (Unit* target = GetHitUnit())
                {
                    std::list<Unit*> tempUnitMap;
                    _player->GetAttackableUnitListInRange(tempUnitMap, _player->GetDistance(target));

                    // Chi Burst will always heal the Monk, but not heal twice if Monk targets himself
                    if (target->GetGUID() != _player->GetGUID())
                        _player->CastSpell(_player, SPELL_MONK_CHI_BURST_HEAL, true);

                    if (_player->IsValidAttackTarget(target))
                        _player->CastSpell(target, SPELL_MONK_CHI_BURST_DAMAGE, true);
                    else
                        _player->CastSpell(target, SPELL_MONK_CHI_BURST_HEAL, true);

                    for (auto itr : tempUnitMap)
                    {
                        if (itr->GetGUID() == _player->GetGUID())
                            continue;

                        if (!itr->IsInBetween(_player, target, 3.0f))
                            continue;

                        uint32 spell = _player->IsValidAttackTarget(itr) ? SPELL_MONK_CHI_BURST_DAMAGE : SPELL_MONK_CHI_BURST_HEAL;
                        _player->CastSpell(itr, spell, true);
                    }
                }
            }
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_monk_chi_burst_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_chi_burst_SpellScript();
    }
};

// Energizing Brew - 115288
class spell_monk_energizing_brew : public SpellScriptLoader
{
    public:
    spell_monk_energizing_brew() : SpellScriptLoader("spell_monk_energizing_brew")
    { }

    class spell_monk_energizing_brew_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_energizing_brew_SpellScript);

        SpellCastResult CheckFight()
        {
            if (!GetCaster()->IsInCombat())
                return SpellCastResult::SPELL_FAILED_CASTER_AURASTATE;
            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register()
        {
            OnCheckCast += SpellCheckCastFn(spell_monk_energizing_brew_SpellScript::CheckFight);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_energizing_brew_SpellScript();
    }
};

// Spear Hand Strike - 116705
class spell_monk_spear_hand_strike : public SpellScriptLoader
{
    public:
    spell_monk_spear_hand_strike() : SpellScriptLoader("spell_monk_spear_hand_strike")
    { }

    class spell_monk_spear_hand_strike_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_spear_hand_strike_SpellScript);

        void HandleOnHit()
        {
            if (Player* _player = GetCaster()->ToPlayer())
            {
                if (Unit* target = GetHitUnit())
                {
                    if (target->isInFront(_player))
                    {
                        _player->CastSpell(target, SPELL_MONK_SPEAR_HAND_STRIKE_SILENCE, true);
                        _player->AddSpellCooldown(116705, 0, time(NULL) + 15);
                    }
                }
            }
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_monk_spear_hand_strike_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_spear_hand_strike_SpellScript();
    }
};

// Tiger's Lust - 116841
class spell_monk_tigers_lust : public SpellScriptLoader
{
    public:
    spell_monk_tigers_lust() : SpellScriptLoader("spell_monk_tigers_lust")
    { }

    class spell_monk_tigers_lust_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_tigers_lust_SpellScript);

        void HandleOnHit()
        {
            if (Player* _player = GetCaster()->ToPlayer())
                if (Unit* target = GetHitUnit())
                    target->RemoveMovementImpairingAuras();
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_monk_tigers_lust_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_tigers_lust_SpellScript();
    }
};


// Chi Torpedo - 115008 or Chi Torpedo (3 charges) - 121828
class spell_monk_chi_torpedo : public SpellScript
{
    PrepareSpellScript(spell_monk_chi_torpedo);

    SpellCastResult CheckCast()
    {
        if (GetCaster()->HasUnitState(UNIT_STATE_ROOT))
            return SpellCastResult::SPELL_FAILED_ROOTED;

        return SpellCastResult::SPELL_CAST_OK;
    }

    void HandleAfterCast()
    {
        if (GetCaster()->HasAura(SPELL_MONK_ITEM_PVP_GLOVES_BONUS))
            GetCaster()->RemoveAurasByType(SPELL_AURA_MOD_DECREASE_SPEED);
    }

    void Register() OVERRIDE
    {
        OnCheckCast += SpellCheckCastFn(spell_monk_chi_torpedo::CheckCast);
        AfterCast += SpellCastFn(spell_monk_chi_torpedo::HandleAfterCast);
    }
};

// 339 - created by Chi Torpedo, hits everything the monk rolls through once
class sat_monk_chi_torpedo : public IAreaTrigger
{
    std::set<uint64> m_hit;
    Position m_lastPos;

    void OnInit() OVERRIDE
    {
        GetCaster()->GetPosition(&m_lastPos);
    }

    void OnUpdate(uint32 /*diff*/) OVERRIDE
    {
        Unit* caster = GetCaster();

        std::list<Unit*> targets;
        Skyfire::AnyUnitInObjectRangeCheck check(caster, 4.0f);
        Skyfire::UnitListSearcher<Skyfire::AnyUnitInObjectRangeCheck> searcher(caster, targets, check);
        caster->VisitNearbyObject(4.0f, searcher);

        SpellInfo const* damageSpell = sSpellMgr->GetSpellInfo(SPELL_MONK_CHI_TORPEDO_DAMAGE);
        for (Unit* target : targets)
        {
            if (target->IsTotem() || m_hit.count(target->GetGUID()))
                continue;
            if (!m_lastPos.HasInArc(float(2 * M_PI / 3), target) || !caster->IsWithinLOSInMap(target))
                continue;

            if (caster->_IsValidAttackTarget(target, damageSpell))
                caster->CastSpell(target, SPELL_MONK_CHI_TORPEDO_DAMAGE, true);
            else if (caster->IsValidAssistTarget(target))
                caster->CastSpell(target, SPELL_MONK_CHI_TORPEDO_HEAL, true);
            m_hit.insert(target->GetGUID());
        }

        caster->GetPosition(&m_lastPos);
    }
};

static uint32 GetStaggerLevelSpell(Unit const* monk, int32 tickAmount)
{
    if (tickAmount < int32(monk->CountPctFromMaxHealth(3)))
        return SPELL_MONK_LIGHT_STAGGER;
    if (tickAmount < int32(monk->CountPctFromMaxHealth(6)))
        return SPELL_MONK_MODERATE_STAGGER;
    return SPELL_MONK_HEAVY_STAGGER;
}

// 124255 - Stagger
class spell_monk_stagger : public SpellScriptLoader
{
public:
    spell_monk_stagger() : SpellScriptLoader("spell_monk_stagger") { }

    class spell_monk_stagger_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_stagger_AuraScript);

        int32 tick = 0;
        float addAmount = 0.0f;

        // Changing the amount here only affects the following ticks
        void HandleTick(AuraEffect const* aurEff, int32& /*amount*/)
        {
            ++tick;

            AuraEffect* stagger = const_cast<AuraEffect*>(aurEff);
            if (uint32 damage = uint32(addAmount))
            {
                addAmount -= float(damage);

                Unit* monk = GetUnitOwner();

                int32 remain = (10 - tick) * stagger->GetAmount();
                int32 total = remain + int32(damage);
                int32 amount = total / 10;

                stagger->SetAmount(amount);
                if (AuraEffect* totalEff = GetEffect(EFFECT_1))
                    totalEff->SetAmount(total);

                uint32 spellId = GetStaggerLevelSpell(monk, amount);
                if (Aura* dummy = monk->GetAura(spellId))
                {
                    dummy->RefreshDuration();
                    if (AuraEffect* eff = dummy->GetEffect(EFFECT_0))
                        eff->SetAmount(amount);
                    if (AuraEffect* eff = dummy->GetEffect(EFFECT_1))
                        eff->SetAmount(total);
                }
                else
                {
                    for (uint32 id : { SPELL_MONK_LIGHT_STAGGER, SPELL_MONK_MODERATE_STAGGER, SPELL_MONK_HEAVY_STAGGER })
                        monk->RemoveAurasDueToSpell(id);
                    monk->CastCustomSpell(monk, spellId, &amount, &total, NULL, true);
                }

                tick = 0;

                if (AuraEffect const* bonus = monk->GetAuraEffect(SPELL_MONK_T15_BREWMASTER_4P_BONUS, EFFECT_0))
                    if (roll_chance_i(bonus->GetSpellInfo()->ProcChance))
                        monk->CastSpell(monk, bonus->GetSpellInfo()->Effects[EFFECT_0].TriggerSpell, true);
            }
            else if (tick == 9)
                Remove();
        }

    public:
        void AddAmount(float amount)
        {
            addAmount += amount;
        }

        void Register() OVERRIDE
        {
            DoEffectCalcTickAmount += AuraEffectCalcTickAmountFn(spell_monk_stagger_AuraScript::HandleTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_monk_stagger_AuraScript();
    }
};

// 115069 - Stance of the Sturdy Ox
class spell_monk_stance_of_the_sturdy_ox : public SpellScriptLoader
{
public:
    spell_monk_stance_of_the_sturdy_ox() : SpellScriptLoader("spell_monk_stance_of_the_sturdy_ox") { }

    class spell_monk_stance_of_the_sturdy_ox_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_stance_of_the_sturdy_ox_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            amount = -1;
        }

        void HandleStagger(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
        {
            if (!dmgInfo.GetDamage() || (dmgInfo.GetSpellInfo() && dmgInfo.GetSpellInfo()->Id == SPELL_MONK_STAGGER))
                return;

            Unit* monk = GetUnitOwner();

            float pct = 20.0f;
            if (Aura* mastery = monk->GetAura(SPELL_MONK_MASTERY_ELUSIVE_BRAWLER))
                if (monk->GetTypeId() == TypeID::TYPEID_PLAYER)
                    pct += monk->GetFloatValue(PLAYER_FIELD_MASTERY) * mastery->GetSpellInfo()->Effects[EFFECT_0].CalcBonusMultiplier(monk);
            if (monk->HasAura(SPELL_MONK_FORTIFYING_BREW_AURA))
                pct += 20.0f;
            if (monk->HasAura(SPELL_MONK_SHUFFLE))
                pct += 20.0f;
            if (AuraEffect const* staggering = monk->GetAuraEffect(SPELL_MONK_STAGGERING, EFFECT_0))
                pct += staggering->GetAmount();

            if (pct > 100.0f)
                pct = 100.0f;

            float staggerAmount = CalculatePct(float(dmgInfo.GetDamage()), pct);
            absorbAmount = uint32(staggerAmount);

            if (Aura* stagger = monk->GetAura(SPELL_MONK_STAGGER))
            {
                if (spell_monk_stagger::spell_monk_stagger_AuraScript* script = dynamic_cast<spell_monk_stagger::spell_monk_stagger_AuraScript*>(stagger->GetScriptByName("spell_monk_stagger")))
                    script->AddAmount(staggerAmount);
            }
            else
            {
                int32 bp0 = int32(staggerAmount / 10);
                int32 bp1 = int32(staggerAmount);

                monk->CastCustomSpell(monk, GetStaggerLevelSpell(monk, bp0), &bp0, &bp1, NULL, true);
                monk->CastCustomSpell(monk, SPELL_MONK_STAGGER, &bp0, &bp1, NULL, true);
            }
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_stance_of_the_sturdy_ox_AuraScript::CalculateAmount, EFFECT_2, SPELL_AURA_SCHOOL_ABSORB);
            OnEffectAbsorb += AuraEffectAbsorbFn(spell_monk_stance_of_the_sturdy_ox_AuraScript::HandleStagger, EFFECT_2);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_monk_stance_of_the_sturdy_ox_AuraScript();
    }
};

// 124275 - Light Stagger, 124274 - Moderate Stagger, 124273 - Heavy Stagger
class spell_monk_stagger_dummy : public SpellScriptLoader
{
public:
    spell_monk_stagger_dummy() : SpellScriptLoader("spell_monk_stagger_dummy") { }

    class spell_monk_stagger_dummy_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_stagger_dummy_AuraScript);

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_EXPIRE)
                GetUnitOwner()->RemoveAurasDueToSpell(SPELL_MONK_STAGGER);
        }

        void Register() OVERRIDE
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_monk_stagger_dummy_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_monk_stagger_dummy_AuraScript();
    }
};

// Purifying Brew - 119582
class spell_monk_purifying_brew : public SpellScriptLoader
{
    public:
    spell_monk_purifying_brew() : SpellScriptLoader("spell_monk_purifying_brew")
    { }

    class spell_monk_purifying_brew_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_purifying_brew_SpellScript);

        void HandleHit()
        {
            if (Unit* monk = GetHitUnit())
                for (uint32 id : { SPELL_MONK_STAGGER, SPELL_MONK_LIGHT_STAGGER, SPELL_MONK_MODERATE_STAGGER, SPELL_MONK_HEAVY_STAGGER })
                    monk->RemoveAurasDueToSpell(id);
        }

        void Register()
        {
            AfterHit += SpellHitFn(spell_monk_purifying_brew_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_purifying_brew_SpellScript();
    }
};

// Clash - 122057 and Clash - 126449
class spell_monk_clash : public SpellScriptLoader
{
    public:
    spell_monk_clash() : SpellScriptLoader("spell_monk_clash")
    { }

    class spell_monk_clash_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_clash_SpellScript);

        void HandleOnHit()
        {
            if (Unit* caster = GetCaster())
            {
                if (Player* _player = caster->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        int32 basePoint = 2;
                        _player->CastCustomSpell(target, SPELL_MONK_CLASH_CHARGE, &basePoint, NULL, NULL, true);
                        target->CastSpell(_player, SPELL_MONK_CLASH_CHARGE, true);
                    }
                }
            }
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_monk_clash_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_clash_SpellScript();
    }
};

// Keg Smash - 121253
class spell_monk_keg_smash : public SpellScriptLoader
{
    public:
    spell_monk_keg_smash() : SpellScriptLoader("spell_monk_keg_smash")
    { }

    class spell_monk_keg_smash_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_keg_smash_SpellScript);

        void HandleOnHit()
        {
            if (Unit* caster = GetCaster())
            {
                if (Player* _player = caster->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        _player->CastSpell(target, SPELL_MONK_KEG_SMASH_VISUAL, true);
                        _player->CastSpell(target, SPELL_MONK_WEAKENED_BLOWS, true);
                        _player->CastSpell(_player, SPELL_MONK_KEG_SMASH_ENERGIZE, true);
                        // Prevent to receive 2 CHI more than once time per cast
                        _player->AddSpellCooldown(SPELL_MONK_KEG_SMASH_ENERGIZE, 0, time(NULL) + 1);
                        _player->CastSpell(target, SPELL_MONK_DIZZYING_HAZE, true);
                    }
                }
            }
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_monk_keg_smash_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_keg_smash_SpellScript();
    }
};

// Elusive Brew - 115308
class spell_monk_elusive_brew : public SpellScriptLoader
{
    public:
    spell_monk_elusive_brew() : SpellScriptLoader("spell_monk_elusive_brew")
    { }

    class spell_monk_elusive_brew_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_elusive_brew_SpellScript);

        void HandleOnHit()
        {
            int32 stackAmount = 0;

            if (Unit* caster = GetCaster())
            {
                if (Player* _player = caster->ToPlayer())
                {
                    if (AuraApplication* brewStacks = _player->GetAuraApplication(SPELL_MONK_ELUSIVE_BREW_STACKS))
                        stackAmount = brewStacks->GetBase()->GetStackAmount();

                    _player->AddAura(SPELL_MONK_ELUSIVE_BREW, _player);

                    if (AuraApplication* aura = _player->GetAuraApplication(SPELL_MONK_ELUSIVE_BREW))
                    {
                        Aura* elusiveBrew = aura->GetBase();
                        int32 maxDuration = elusiveBrew->GetMaxDuration();
                        int32 newDuration = stackAmount * 1000;
                        elusiveBrew->SetDuration(newDuration);

                        if (newDuration > maxDuration)
                            elusiveBrew->SetMaxDuration(newDuration);

                        _player->RemoveAura(SPELL_MONK_ELUSIVE_BREW_STACKS);
                    }
                }
            }
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_monk_elusive_brew_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_elusive_brew_SpellScript();
    }
};

// Soothing Mist - 115175
class spell_monk_soothing_mist : public SpellScriptLoader
{
    public:
    spell_monk_soothing_mist() : SpellScriptLoader("spell_monk_soothing_mist")
    { }

    class spell_monk_soothing_mist_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_soothing_mist_AuraScript);

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (!GetCaster())
                return;

            if (Unit* target = GetTarget())
                target->CastSpell(target, SPELL_MONK_SOOTHING_MIST_VISUAL, true);

            if (Player* _player = GetCaster()->ToPlayer())
            {
                std::list<TempSummon*> summons;
                _player->GetSummons(summons, MONK_NPC_JADE_SERPENT_STATUE);
                if (!summons.empty() && _player->IsWithinDistInMap(summons.front(), 100.0f))
                    summons.front()->CastSpell(summons.front(), SPELL_MONK_SOOTHING_MIST_STATUE_TRIGGER, true);
            }
        }

        void HandleEffectPeriodic(AuraEffect const* /*aurEff*/)
        {
            if (Unit* caster = GetCaster())
                if (Unit* target = GetTarget())
                    // 25% to give 1 chi per tick
                    if (roll_chance_i(25))
                        caster->CastSpell(caster, SPELL_MONK_SOOTHING_MIST_ENERGIZE, true);
        }

        void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
            {
                if (Unit* target = GetTarget())
                    if (target->HasAura(SPELL_MONK_SOOTHING_MIST_VISUAL))
                        target->RemoveAura(SPELL_MONK_SOOTHING_MIST_VISUAL);

                if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
                {
                    std::list<TempSummon*> summons;
                    caster->GetSummons(summons, MONK_NPC_JADE_SERPENT_STATUE);
                    if (!summons.empty())
                        summons.front()->InterruptSpell(CURRENT_CHANNELED_SPELL);
                }
            }
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_monk_soothing_mist_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_monk_soothing_mist_AuraScript::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_monk_soothing_mist_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_monk_soothing_mist_AuraScript();
    }
};

enum Blackout
{
    SPELL_MONK_COMBAT_CONDITIONING = 128595,
    SPELL_MONK_BLACKOUT_KICK_DOT = 128531,
    SPELL_MONK_GLYPH_OF_BLACKOUT_KICK = 132005,
    SPELL_MONK_BLACKOUT_KICK_HEAL = 128591,
};

// Blackout Kick - 100784
class spell_monk_blackout_kick : public SpellScriptLoader
{
    public:
    spell_monk_blackout_kick() : SpellScriptLoader("spell_monk_blackout_kick")
    { }

    class spell_monk_blackout_kick_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_blackout_kick_SpellScript);

        void HandleOnHit()
        {
            if (Unit* caster = GetCaster())
            {
                if (Unit* target = GetHitUnit())
                {
                    // Second effect by spec : Instant heal or DoT
                    if (caster->GetTypeId() == TypeID::TYPEID_PLAYER && caster->ToPlayer()->GetTalentSpecialization(caster->ToPlayer()->GetActiveSpec()) == SPEC_MONK_WINDWALKER
                        && caster->ToPlayer()->HasAura(SPELL_MONK_COMBAT_CONDITIONING))
                    {
                        // Your Blackout Kick always deals 20% additional damage over 4 sec regardless of positioning but you're unable to trigger the healing effect.
                        if (caster->HasAura(SPELL_MONK_GLYPH_OF_BLACKOUT_KICK))
                        {
                            int32 bp = int32(GetHitDamage() * 0.2f) / 4;
                            caster->CastCustomSpell(target, SPELL_MONK_BLACKOUT_KICK_DOT, &bp, NULL, NULL, true);
                        }
                        else
                        {
                            // If behind : 20% damage on DoT
                            if (target->isInBack(caster))
                            {
                                int32 bp = int32(GetHitDamage() * 0.2f) / 4;
                                caster->CastCustomSpell(target, SPELL_MONK_BLACKOUT_KICK_DOT, &bp, NULL, NULL, true);
                            }
                            // else : 20% damage on instant heal
                            else
                            {
                                int32 bp = int32(GetHitDamage() * 0.2f);
                                caster->CastCustomSpell(caster, SPELL_MONK_BLACKOUT_KICK_HEAL, &bp, NULL, NULL, true);
                            }
                        }
                    }
                    // Brewmaster : Training - you gain Shuffle, increasing parry chance and stagger amount by 20%
                    else if (caster->GetTypeId() == TypeID::TYPEID_PLAYER && caster->ToPlayer()->GetTalentSpecialization(caster->ToPlayer()->GetActiveSpec()) == SPEC_MONK_BREWMASTER)
                        caster->CastSpell(caster, SPELL_MONK_SHUFFLE, true);
                }
            }
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_monk_blackout_kick_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_blackout_kick_SpellScript();
    }
};

// Brewing : Tigereye Brew - 123980
class spell_monk_tigereye_brew_stacks : public SpellScriptLoader
{
    public:
    spell_monk_tigereye_brew_stacks() : SpellScriptLoader("spell_monk_tigereye_brew_stacks")
    { }

    class spell_monk_tigereye_brew_stacks_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_monk_tigereye_brew_stacks_AuraScript);

        uint32 chiConsumed;

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            chiConsumed = 0;
        }

        void SetData(uint32 type, uint32 data)
        {
            while ((chiConsumed += data) >= 4)
            {
                chiConsumed = 0;
                data = data > 4 ? data - 4 : 0;

                if (GetCaster())
                    GetCaster()->CastSpell(GetCaster(), SPELL_MONK_TIGEREYE_BREW_STACKS, true);
            }
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_monk_tigereye_brew_stacks_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_monk_tigereye_brew_stacks_AuraScript();
    }
};

struct spell_monk_specific_damage_base
{
    static float CalculateDamage(Player* player, uint32 spellId)
    {
        struct Modificator
        {
            float Default;
            bool BrewmasterPenalty;
        };

        static std::map<uint32, Modificator> const map =
        {
            { 100780, { 1.5f,                   false } }, // Jab
            { 108557, { 1.5f,                   false } }, // Jab (Staff)
            { 115698, { 1.5f,                   false } }, // Jab (Polearm)
            { 115687, { 1.5f,                   false } }, // Jab (Axes)
            { 115693, { 1.5f,                   false } }, // Jab (Maces)
            { 115695, { 1.5f,                   false } }, // Jab (Swords)
            { 121253, { 10.0f,                  false } }, // Keg Smash
            { 100784, { 7.12f,                  false } }, // Blackout Kick
            { 100787, { 3.0f,                   false } }, // Tiger Palm
            { 115073, { 2.1f,                   false } }, // Spinning Fire Blossom
            { 123408, { 2.1f,                   false } }, // Spinning Fire Blossom (glyphed)
            { 107428, { 14.4f * 0.89f,          false } }, // Rising Sun Kick
            { 124335, { 0.3f * 7.5f,            true  } }, // Swift Reflexes
            { 107270, { 1.75f,                  true  } }, // Spinning Crane Kick
            { 148187, { 1.4f,                   true  } }, // Rushing Jade Wind
            { 113656, { 7.5f * 0.89f,           false } }, // Fists of Fury
        };

        auto it = map.find(spellId);
        if (it == map.end())
        {
            SF_LOG_ERROR("sql.sql", "spell_monk_specific_damage is bound to spell %u, which has no damage modifier", spellId);
            return 0.0f;
        }

        bool const penalty = it->second.BrewmasterPenalty && player->HasSpell(SPELL_MONK_VENGEANCE);
        float const spellCoeff = it->second.Default;
        float const basemod = penalty ? 0.4f : 1.0f;
        float const apmod = penalty ? 11.0f : 14.0f;

        Item* mainItem = player->GetWeaponForAttack(WeaponAttackType::BASE_ATTACK, true);
        Item* offItem = player->GetWeaponForAttack(WeaponAttackType::OFF_ATTACK, true);

        float mainMin = 0.0f;
        float mainMax = 0.0f;
        float offMin = 0.0f;
        float offMax = 0.0f;
        float dualmod = 0.898882275f;
        float offmod = 1.0f;

        // normalised to weapon dps
        if (mainItem)
        {
            ItemTemplate const* proto = mainItem->GetTemplate();
            if (proto->Delay)
            {
                mainMin = player->GetWeaponDamageRange(WeaponAttackType::BASE_ATTACK, WeaponDamageRange::MINDAMAGE) / proto->Delay * 1000.0f;
                mainMax = player->GetWeaponDamageRange(WeaponAttackType::BASE_ATTACK, WeaponDamageRange::MAXDAMAGE) / proto->Delay * 1000.0f;
            }
            if (proto->InventoryType == INVTYPE_2HWEAPON)
            {
                dualmod = 1.0f;
                offmod = 0.0f;
            }
        }

        if (offItem)
        {
            ItemTemplate const* proto = offItem->GetTemplate();
            if (proto->Delay)
            {
                offMin = player->GetWeaponDamageRange(WeaponAttackType::OFF_ATTACK, WeaponDamageRange::MINDAMAGE) / proto->Delay * 1000.0f / 2.0f;
                offMax = player->GetWeaponDamageRange(WeaponAttackType::OFF_ATTACK, WeaponDamageRange::MAXDAMAGE) / proto->Delay * 1000.0f / 2.0f;
            }
        }
        else
        {
            offMin = mainMin / 2.0f;
            offMax = mainMax / 2.0f;
        }

        float const apc = player->GetTotalAttackPowerValue(WeaponAttackType::BASE_ATTACK) / apmod;

        float const min = spellCoeff * (basemod * dualmod * (mainMin + offmod * offMin) + apc - 1);
        float const max = spellCoeff * (basemod * dualmod * (mainMax + offmod * offMax) + apc + 1);

        return frand(min, std::max(min, max));
    }
};

// Jab (all weapon variants), Keg Smash, Blackout Kick, Tiger Palm, Spinning Fire Blossom,
// Rising Sun Kick, Swift Reflexes, Spinning Crane Kick, Rushing Jade Wind
class spell_monk_specific_damage : public SpellScript
{
    PrepareSpellScript(spell_monk_specific_damage);

    void CalculateDamage(SpellEffIndex /*effIndex*/)
    {
        Player* player = GetCaster()->ToPlayer();
        if (!player)
            if (Unit* owner = GetCaster()->GetOwner())
                player = owner->ToPlayer();
        if (!player)
            return;

        SetEffectValue(int32(spell_monk_specific_damage_base::CalculateDamage(player, GetSpellInfo()->Id)));
    }

    void Register() OVERRIDE
    {
        OnEffectLaunchTarget += SpellEffectFn(spell_monk_specific_damage::CalculateDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// 113656 - Fists of Fury
class spell_monk_fists_of_fury : public AuraScript
{
    PrepareAuraScript(spell_monk_fists_of_fury);

    void CalculateDamage(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        if (Player* monk = GetUnitOwner()->GetAffectingPlayer())
            amount = int32(spell_monk_specific_damage_base::CalculateDamage(monk, GetSpellInfo()->Id));
    }

    // The visual on the targets has no duration of its own
    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* owner = GetUnitOwner();
        std::list<Unit*> targets;
        Skyfire::AnyUnitInObjectRangeCheck check(owner, 10.0f);
        Skyfire::UnitListSearcher<Skyfire::AnyUnitInObjectRangeCheck> searcher(owner, targets, check);
        owner->VisitNearbyObject(10.0f, searcher);
        for (Unit* target : targets)
            target->RemoveAura(SPELL_MONK_FISTS_OF_FURY_VISUAL, GetCasterGUID());
    }

    void Register() OVERRIDE
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_fists_of_fury::CalculateDamage, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL_WITH_VALUE);
        OnEffectRemove += AuraEffectRemoveFn(spell_monk_fists_of_fury::HandleRemove, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL_WITH_VALUE, AURA_EFFECT_HANDLE_REAL);
    }
};

// 117418 - Fists of Fury (damage)
class spell_monk_fists_of_fury_damage : public SpellScript
{
    PrepareSpellScript(spell_monk_fists_of_fury_damage);

    uint32 targetCount = 1;

    void CountTargets(std::list<WorldObject*>& targets)
    {
        targetCount = std::max<uint32>(1, uint32(targets.size()));
    }

    void HandleLaunch(SpellEffIndex /*effIndex*/)
    {
        if (AuraEffect const* effect = GetCaster()->GetAuraEffect(SPELL_MONK_FISTS_OF_FURY, EFFECT_1))
            SetEffectValue(effect->GetAmount());
    }

    // The damage is split between all targets
    void HandleHit()
    {
        SetHitDamage(GetHitDamage() / int32(targetCount));
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_fists_of_fury_damage::CountTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_24);
        OnEffectLaunchTarget += SpellEffectFn(spell_monk_fists_of_fury_damage::HandleLaunch, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        OnHit += SpellHitFn(spell_monk_fists_of_fury_damage::HandleHit);
    }
};

// 120086 - Fists of Fury (stun)
class spell_monk_fists_of_fury_stun : public SpellScript
{
    PrepareSpellScript(spell_monk_fists_of_fury_stun);

    void RemoveInvalidTargets(std::list<WorldObject*>& targets)
    {
        if (GetCaster()->GetTypeId() != TypeID::TYPEID_PLAYER)
            targets.clear();
        else
            targets.remove_if(Skyfire::UnitAuraCheck(true, GetSpellInfo()->Id));
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_fists_of_fury_stun::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_24);
    }
};

// 107428 - Rising Sun Kick
class spell_monk_rising_sun_kick : public SpellScript
{
    PrepareSpellScript(spell_monk_rising_sun_kick);

    void HandleHit()
    {
        if (Unit* target = GetHitUnit())
            GetCaster()->CastSpell(target, SPELL_MONK_RISING_SUN_KICK_DEBUFF, true);
    }

    void Register() OVERRIDE
    {
        AfterHit += SpellHitFn(spell_monk_rising_sun_kick::HandleHit);
    }
};

// 137384 - Combo Breaker
class spell_monk_combo_breaker : public AuraScript
{
    PrepareAuraScript(spell_monk_combo_breaker);

    void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* owner = GetUnitOwner();
        // Jab's energize also procs this
        if (eventInfo.GetActionTarget() == owner)
            return;

        if (roll_chance_i(aurEff->GetAmount()))
            owner->CastSpell(owner, SPELL_MONK_COMBO_BREAKER_KICK, true);

        if (roll_chance_i(aurEff->GetAmount()))
            owner->CastSpell(owner, SPELL_MONK_COMBO_BREAKER_PALM, true);
    }

    void Register() OVERRIDE
    {
        OnEffectProc += AuraEffectProcFn(spell_monk_combo_breaker::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 128938 - Brewing: Elusive Brew
class spell_monk_brewing_elusive_brew : public AuraScript
{
    PrepareAuraScript(spell_monk_brewing_elusive_brew);

    float partialStack = 0.0f;

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return (eventInfo.GetTypeMask() & PROC_FLAG_DONE_MELEE_AUTO_ATTACK) && (eventInfo.GetHitMask() & PROC_EX_CRITICAL_HIT);
    }

    // Stacks scale with weapon speed, so the accumulated fraction is carried over to the next crit
    void HandleProc(ProcEventInfo& /*eventInfo*/)
    {
        Player* monk = GetUnitOwner()->ToPlayer();
        if (!monk)
            return;

        float const speed = monk->GetAttackTime(WeaponAttackType::BASE_ATTACK) / 1000.0f;
        Item* weapon = monk->GetWeaponForAttack(WeaponAttackType::BASE_ATTACK);
        if (weapon && weapon->GetTemplate()->InventoryType == INVTYPE_2HWEAPON)
            partialStack += 3.0f * speed / 3.6f;
        else
            partialStack += 1.5f * speed / 2.6f;

        int32 stacks = int32(partialStack);
        if (stacks <= 0)
            return;
        partialStack -= float(stacks);

        if (Aura* brewStacks = monk->GetAura(SPELL_MONK_ELUSIVE_BREW_STACKS))
        {
            brewStacks->ModStackAmount(stacks);
            brewStacks->RefreshDuration();
        }
        else
            monk->CastCustomSpell(SPELL_MONK_ELUSIVE_BREW_STACKS, SPELLVALUE_AURA_STACK, stacks, monk, true);
    }

    void Register() OVERRIDE
    {
        DoCheckProc += AuraCheckProcFn(spell_monk_brewing_elusive_brew::CheckProc);
        OnProc += AuraProcFn(spell_monk_brewing_elusive_brew::HandleProc);
    }
};

// 116330 - Dizzying Haze
class spell_monk_dizzying_haze : public SpellScript
{
    PrepareSpellScript(spell_monk_dizzying_haze);

    void CalculateThreat(SpellEffIndex /*effIndex*/)
    {
        SetEffectValue(int32(GetCaster()->GetTotalAttackPowerValue(WeaponAttackType::BASE_ATTACK)));
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_monk_dizzying_haze::CalculateThreat, EFFECT_2, SPELL_EFFECT_THREAT);
    }
};

// 100780, 108557, 115687, 115693, 115695, 115698 - Jab
class spell_monk_muscle_memory : public SpellScript
{
    PrepareSpellScript(spell_monk_muscle_memory);

    void HandleCast()
    {
        if (GetCaster()->HasSpell(SPELL_MONK_MUSCLE_MEMORY))
            GetCaster()->CastSpell(GetCaster(), SPELL_MONK_MUSCLE_MEMORY_BUFF, true);
    }

    void Register() OVERRIDE
    {
        OnCast += SpellCastFn(spell_monk_muscle_memory::HandleCast);
    }
};

// 107270 - Spinning Crane Kick, 148187 - Rushing Jade Wind
class spell_monk_spinning_crane_kick_damage : public SpellScript
{
    PrepareSpellScript(spell_monk_spinning_crane_kick_damage);

    uint32 hitCount = 0;

    // Muscle Memory triggers once a single tick hits 3 targets
    void HandleHit()
    {
        Player* monk = GetCaster()->ToPlayer();
        if (!monk || !monk->HasSpell(SPELL_MONK_MUSCLE_MEMORY))
            return;

        if (hitCount != 3 && ++hitCount == 3)
            monk->CastSpell(monk, SPELL_MONK_MUSCLE_MEMORY_BUFF, true);
    }

    void Register() OVERRIDE
    {
        OnHit += SpellHitFn(spell_monk_spinning_crane_kick_damage::HandleHit);
    }
};

// 119611 - Renewing Mist
class spell_monk_renewing_mist : public AuraScript
{
    PrepareAuraScript(spell_monk_renewing_mist);

    // The heal is defined on the cast spell, 115151
    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        if (Unit* caster = GetCaster())
            if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(SPELL_MONK_RENEWING_MIST))
                amount = spellInfo->Effects[EFFECT_2].CalcValue(caster);
    }

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* monk = GetCaster())
            monk->CastSpell(monk, SPELL_MONK_UPLIFT_ALLOWING_CAST, true);
    }

    // EFFECT_1 holds the number of targets the mist may still reach, this one included
    void HandleTick(AuraEffect const* /*aurEff*/)
    {
        Unit* monk = GetCaster();
        if (!monk)
            return;

        monk->CastSpell(monk, SPELL_MONK_UPLIFT_ALLOWING_CAST, true);

        AuraEffect const* jumps = GetEffect(EFFECT_1);
        if (!jumps || jumps->GetAmount() <= 1)
            return;

        monk->CastSpell(GetTarget(), SPELL_MONK_RENEWING_MIST_JUMP_AURA, true);
    }

    void Register() OVERRIDE
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_renewing_mist::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
        OnEffectApply += AuraEffectApplyFn(spell_monk_renewing_mist::HandleApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_monk_renewing_mist::HandleTick, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
    }
};

// 119607 - Renewing Mist (jump)
class spell_monk_renewing_mist_jump : public SpellScript
{
    PrepareSpellScript(spell_monk_renewing_mist_jump);

    int32 remainingJumps = 0;

    void SelectTarget(std::list<WorldObject*>& targets)
    {
        uint64 casterGuid = GetCaster()->GetGUID();
        Unit* source = GetExplTargetUnit();
        AuraEffect* jumps = source ? source->GetAuraEffect(SPELL_MONK_RENEWING_MIST_HOT, EFFECT_1, casterGuid) : NULL;
        if (!jumps || jumps->GetAmount() <= 1)
        {
            targets.clear();
            return;
        }

        targets.remove_if([casterGuid](WorldObject const* target)
        {
            Unit const* unit = target->ToUnit();
            if (!unit || unit->HasAura(SPELL_MONK_RENEWING_MIST_HOT, casterGuid))
                return true;
            return unit->IsSummon() && unit->ToTempSummon()->IsMinorGuardian();
        });

        if (targets.empty())
            return;

        // Glyph of Renewing Mist prefers the farthest ally instead of the nearest
        bool const nearestFirst = !GetCaster()->HasAura(SPELL_MONK_GLYPH_OF_RENEWING_MIST);
        targets.sort([source, nearestFirst](WorldObject const* a, WorldObject const* b)
        {
            return nearestFirst ? source->GetExactDist2dSq(a) < source->GetExactDist2dSq(b)
                                : source->GetExactDist2dSq(a) > source->GetExactDist2dSq(b);
        });

        WorldObject* selected = targets.front();
        for (WorldObject* target : targets)
        {
            if (!target->ToUnit()->IsFullHealth())
            {
                selected = target;
                break;
            }
        }

        targets.clear();
        targets.push_back(selected);

        remainingJumps = jumps->GetAmount() - 1;
        jumps->SetAmount(0);
    }

    void HandleHit(SpellEffIndex /*effIndex*/)
    {
        GetCaster()->CastCustomSpell(SPELL_MONK_RENEWING_MIST_HOT, SPELLVALUE_BASE_POINT1, remainingJumps, GetHitUnit(), true);
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_renewing_mist_jump::SelectTarget, EFFECT_1, TARGET_UNIT_DEST_AREA_ALLY);
        OnEffectHitTarget += SpellEffectFn(spell_monk_renewing_mist_jump::HandleHit, EFFECT_1, SPELL_EFFECT_DUMMY);
    }
};

// 116670 - Uplift, 130316 - Uplift
class spell_monk_thunder_focus_tea_uplift : public SpellScript
{
    PrepareSpellScript(spell_monk_thunder_focus_tea_uplift);

    std::list<WorldObject*> mistTargets;

    // Only allies with the monk's Renewing Mist are healed; Thunder Focus Tea also refreshes the mists
    void FilterTargetsRefresh(std::list<WorldObject*>& targets)
    {
        targets.remove_if(Skyfire::UnitAuraCheck(false, SPELL_MONK_RENEWING_MIST_HOT, GetCaster()->GetGUID()));
        mistTargets = targets;
        if (!GetCaster()->HasAura(SPELL_MONK_THUNDER_FOCUS_TEA))
            targets.clear();
    }

    void FilterTargetsHeal(std::list<WorldObject*>& targets)
    {
        targets = mistTargets;
    }

    void RefreshRenewingMist(SpellEffIndex /*effIndex*/)
    {
        if (Aura* aura = GetHitUnit()->GetAura(SPELL_MONK_RENEWING_MIST_HOT, GetCaster()->GetGUID()))
            aura->RefreshDuration();
    }

    void HandleCast()
    {
        GetCaster()->RemoveAurasDueToSpell(SPELL_MONK_THUNDER_FOCUS_TEA);
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_thunder_focus_tea_uplift::FilterTargetsRefresh, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_thunder_focus_tea_uplift::FilterTargetsHeal, EFFECT_1, TARGET_UNIT_SRC_AREA_ALLY);
        OnEffectHitTarget += SpellEffectFn(spell_monk_thunder_focus_tea_uplift::RefreshRenewingMist, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        AfterCast += SpellCastFn(spell_monk_thunder_focus_tea_uplift::HandleCast);
    }
};

// 115310 - Revival
class spell_monk_revival : public SpellScript
{
    PrepareSpellScript(spell_monk_revival);

    void SelectTargets(std::list<WorldObject*>& targets)
    {
        targets.remove_if([](WorldObject const* target)
        {
            Unit const* unit = target->ToUnit();
            return unit && unit->IsSummon() && unit->ToTempSummon()->IsMinorGuardian();
        });
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_revival::SelectTargets, EFFECT_ALL, TARGET_UNIT_CASTER_AREA_RAID);
    }
};

// 116849 - Life Cocoon
class spell_monk_life_cocoon : public SpellScript
{
    PrepareSpellScript(spell_monk_life_cocoon);

    SpellCastResult CheckCast()
    {
        if (GetCaster()->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED) && !GetCaster()->HasAura(SPELL_MONK_GLYPH_OF_LIFE_COCOON))
            return SpellCastResult::SPELL_FAILED_STUNNED;
        return SpellCastResult::SPELL_CAST_OK;
    }

    void Register() OVERRIDE
    {
        OnCheckCast += SpellCheckCastFn(spell_monk_life_cocoon::CheckCast);
    }
};

// 115450 - Detox
class spell_monk_detox : public SpellScript
{
    PrepareSpellScript(spell_monk_detox);

    // Magic is only dispelled by Mistweavers
    void HandleDispelMagic(SpellEffIndex effIndex)
    {
        if (!GetCaster()->HasAura(SPELL_MONK_INTERNAL_MEDICINE))
            PreventHitDefaultEffect(effIndex);
    }

    void HandleHit()
    {
        if (!GetDispelCount() || !GetCaster()->HasAura(SPELL_MONK_GLYPH_OF_DETOX))
            PreventHitHeal();
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_monk_detox::HandleDispelMagic, EFFECT_2, SPELL_EFFECT_DISPEL);
        OnHit += SpellHitFn(spell_monk_detox::HandleHit);
    }
};

static bool HealthPctLess(WorldObject* a, WorldObject* b)
{
    Unit* unitA = a->ToUnit();
    Unit* unitB = b->ToUnit();
    if (!unitA || !unitB)
        return unitA != NULL;
    return unitA->GetHealthPct() < unitB->GetHealthPct();
}

static bool NeedsHealing(Unit const* unit)
{
    return !unit->IsFullHealth() || unit->HasAuraType(SPELL_AURA_SCHOOL_HEAL_ABSORB);
}

// 718 - Ring of Peace
class sat_monk_ring_of_peace : public IAreaTriggerAura
{
    bool CheckTriggering(WorldObject* triggering) OVERRIDE
    {
        Unit* unit = triggering->ToUnit();
        if (!unit)
            return false;

        if (!m_target->IsWithinDistInMap(unit, m_range))
            return false;

        return m_caster->_IsValidAttackTarget(unit, m_spellInfo, m_target);
    }

    void OnTriggeringApply(WorldObject* triggering) OVERRIDE
    {
        ApplyAura(triggering);
    }

    void OnTriggeringUpdate(WorldObject* triggering) OVERRIDE
    {
        ApplyAura(triggering);
    }

    void OnTriggeringRemove(WorldObject* triggering) OVERRIDE
    {
        if (Unit* target = triggering->ToUnit())
            target->RemoveAurasDueToSpell(SPELL_MONK_RING_OF_PEACE_AURA, m_caster->GetGUID());
    }

    void ApplyAura(WorldObject* triggering)
    {
        if (Unit* target = triggering->ToUnit())
            if (!target->HasAura(SPELL_MONK_RING_OF_PEACE_AURA, m_caster->GetGUID()))
                m_caster->CastSpell(target, SPELL_MONK_RING_OF_PEACE_AURA, true);
    }
};

// 140023 - Ring of Peace
class spell_monk_ring_of_peace_aura : public AuraScript
{
    PrepareAuraScript(spell_monk_ring_of_peace_aura);

    void HandleProc(ProcEventInfo& /*eventInfo*/)
    {
        PreventDefaultAction();

        if (Unit* monk = GetCaster())
        {
            monk->CastSpell(GetTarget(), SPELL_MONK_RING_OF_PEACE_DISARM, true);
            monk->CastSpell(GetTarget(), SPELL_MONK_RING_OF_PEACE_SILENCE, true);
        }
    }

    void Register() OVERRIDE
    {
        OnProc += AuraProcFn(spell_monk_ring_of_peace_aura::HandleProc);
    }
};

// 115460 - Healing Sphere
class spell_monk_healing_sphere : public SpellScript
{
    PrepareSpellScript(spell_monk_healing_sphere);

    void RemoveOldSphere()
    {
        std::list<AreaTrigger*> spheres;
        GetCaster()->GetAreaTriggerList(spheres, SPELL_MONK_HEALING_SPHERE);
        while (spheres.size() > 2)
        {
            spheres.front()->Remove();
            spheres.pop_front();
        }
    }

    void Register() OVERRIDE
    {
        OnCast += SpellCastFn(spell_monk_healing_sphere::RemoveOldSphere);
    }
};

// 228 - created by Healing Sphere
class sat_monk_healing_sphere : public IAreaTriggerOnce
{
    void OnInit() OVERRIDE
    {
        GetCaster()->CastSpell(GetCaster(), SPELL_MONK_HEALING_SPHERE_STACK, true);
    }

    void OnDestroy() OVERRIDE
    {
        if (Aura* stacks = GetCaster()->GetAura(SPELL_MONK_HEALING_SPHERE_STACK))
            stacks->ModStackAmount(-1);
    }

    void OnExpire() OVERRIDE
    {
        std::list<Unit*> targets;
        Skyfire::AnyFriendlyUnitInObjectRangeCheck check(GetTarget(), GetCaster(), 6.0f);
        Skyfire::UnitListSearcher<Skyfire::AnyFriendlyUnitInObjectRangeCheck> searcher(GetTarget(), targets, check);
        GetTarget()->VisitNearbyObject(12.0f, searcher);
        if (targets.empty())
            return;

        targets.sort(Skyfire::HealthPctOrderPred());
        Unit* target = targets.front();
        GetCaster()->CastSpell(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), SPELL_MONK_HEALING_SPHERE_EXPIRE_HEAL, true);
    }

    bool CheckTriggering(WorldObject* object) OVERRIDE
    {
        Unit* unit = object->ToUnit();
        return unit && NeedsHealing(unit) && unit->IsFriendlyTo(GetCaster());
    }

    void OnTrigger(WorldObject* target) OVERRIDE
    {
        GetCaster()->CastSpell(target->ToUnit(), SPELL_MONK_HEALING_SPHERE_HEAL, true);
    }
};

// 336 - created by Chi Sphere (121286)
class sat_monk_chi_sphere : public IAreaTriggerOnce
{
    bool CheckTriggering(WorldObject* object) OVERRIDE
    {
        return object == GetCaster();
    }

    void OnTrigger(WorldObject* /*target*/) OVERRIDE
    {
        GetCaster()->CastSpell(GetCaster(), SPELL_MONK_CHI_SPHERE_ENERGIZE, true);
    }
};

// 124502 - Gift of the Ox
class spell_monk_gift_of_the_ox : public AuraScript
{
    PrepareAuraScript(spell_monk_gift_of_the_ox);

    bool m_left = true;

    void HandleProc(ProcEventInfo& eventInfo)
    {
        Player* monk = GetUnitOwner()->ToPlayer();
        if (!monk)
            return;

        float chance;
        if (eventInfo.GetTypeMask() & PROC_FLAG_DONE_SPELL_MELEE_DMG_CLASS)
            chance = 10.0f;
        else
        {
            chance = 3.0f;
            if (Item const* weapon = monk->GetWeaponForAttack(WeaponAttackType::BASE_ATTACK))
                if (weapon->GetTemplate()->InventoryType == INVTYPE_2HWEAPON)
                    chance = 6.0f;

            chance *= monk->GetAttackTime(WeaponAttackType::BASE_ATTACK) / 1000.0f;
        }

        if (roll_chance_f(chance))
        {
            monk->CastSpell(monk, m_left ? SPELL_MONK_GIFT_OF_THE_OX_LEFT : SPELL_MONK_GIFT_OF_THE_OX_RIGHT, true);
            m_left = !m_left;
        }
    }

    void Register() OVERRIDE
    {
        OnProc += AuraProcFn(spell_monk_gift_of_the_ox::HandleProc);
    }
};

// 124503, 124506 - Gift of the Ox
class spell_monk_summon_gift_of_the_ox : public SpellScript
{
    PrepareSpellScript(spell_monk_summon_gift_of_the_ox);

    void SelectDest(SpellDestination& dest)
    {
        bool right = GetSpellInfo()->Id == SPELL_MONK_GIFT_OF_THE_OX_RIGHT;
        float minAngle = (right ? 260.0f : 45.0f) * float(M_PI) / 180.0f;
        float maxAngle = (right ? 315.0f : 100.0f) * float(M_PI) / 180.0f;

        Position pos;
        GetCaster()->GetFirstCollisionPosition(pos, frand(1.5f, 2.0f), frand(minAngle, maxAngle));
        dest.Relocate(pos);
    }

    void Register() OVERRIDE
    {
        uint16 target = m_scriptSpellId == SPELL_MONK_GIFT_OF_THE_OX_RIGHT ? TARGET_DEST_CASTER_RIGHT : TARGET_DEST_DEST_LEFT;
        OnDestinationTargetSelect += SpellDestinationTargetSelectFn(spell_monk_summon_gift_of_the_ox::SelectDest, EFFECT_0, target);
    }
};

// 373 - created by Gift of the Ox
class sat_monk_gift_of_the_ox : public IAreaTriggerOnce
{
    bool CheckTriggering(WorldObject* object) OVERRIDE
    {
        if (object != GetCaster() || !NeedsHealing(GetCaster()))
            return false;
        return GetCaster()->IsInDist(GetTarget(), m_range);
    }

    void OnTrigger(WorldObject* target) OVERRIDE
    {
        GetCaster()->CastSpell(target->ToUnit(), SPELL_MONK_GIFT_OF_THE_OX_HEAL, true);
    }
};

// 117907 - Mastery: Gift of the Serpent
class spell_monk_mastery_gift_of_the_serpent : public AuraScript
{
    PrepareAuraScript(spell_monk_mastery_gift_of_the_serpent);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
        if (!spellInfo || !spellInfo->IsPositive())
            return false;
        if (spellInfo->Id == SPELL_MONK_GIFT_OF_THE_SERPENT_HEAL || spellInfo->Id == SPELL_MONK_GIFT_OF_THE_SERPENT_HEAL_EXPIRED)
            return false;
        return eventInfo.GetHealInfo() && eventInfo.GetHealInfo()->GetHeal() > 0;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        static std::map<uint32, float> const chanceMultipliers =
        {
            { 115151, 0.15f }, // Renewing Mist
            { 132463, 0.25f }, // Chi Wave
            { 124081, 0.25f }, // Zen Sphere (Periodic)
            { 124101, 0.15f }, // Zen Sphere (Detonate)
            { 115175, 0.3f  }, // Soothing Mist
            { 125953, 0.15f }, // Soothing Mist (Statue)
            { 132120, 0.2f  }, // Enveloping Mist
            { 116670, 0.25f }, // Uplift
            { 115310, 0.15f }, // Revival
            { 126890, 0.2f  }, // Eminence
            { 117895, 0.1f  }, // Eminence (Statue)
            { 117640, 0.1f  }, // Spinning Crane Kick
        };

        float chance = float(aurEff->GetAmount());
        std::map<uint32, float>::const_iterator itr = chanceMultipliers.find(eventInfo.GetSpellInfo()->Id);
        if (itr != chanceMultipliers.end())
            chance *= itr->second;

        if (roll_chance_f(chance))
            GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_MONK_GIFT_OF_THE_SERPENT_SELECTOR, true);
    }

    void Register() OVERRIDE
    {
        DoCheckProc += AuraCheckProcFn(spell_monk_mastery_gift_of_the_serpent::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_monk_mastery_gift_of_the_serpent::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// 119036 - Gift of the Serpent
class spell_monk_gift_of_the_serpent_selector : public SpellScript
{
    PrepareSpellScript(spell_monk_gift_of_the_serpent_selector);

    void SelectTarget(std::list<WorldObject*>& targets)
    {
        targets.sort(HealthPctLess);
        if (!targets.empty())
            targets.resize(1);
    }

    void HandleHit()
    {
        if (!GetHitUnit()->IsFullHealth())
            GetCaster()->CastSpell(GetHitUnit(), SPELL_MONK_GIFT_OF_THE_SERPENT_SUMMON, true);
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_gift_of_the_serpent_selector::SelectTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
        OnHit += SpellHitFn(spell_monk_gift_of_the_serpent_selector::HandleHit);
    }
};

// 119031 - Gift of the Serpent
class spell_monk_gift_of_the_serpent_summon : public SpellScript
{
    PrepareSpellScript(spell_monk_gift_of_the_serpent_summon);

    void PlacementCorrection(SpellDestination& dest)
    {
        Unit* target = GetSpell()->m_targets.GetUnitTarget();
        if (target && target->IsWithinDist3d(&dest._position, 1.0f))
            target->MovePositionToFirstCollision(dest._position, target->GetObjectSize() + 1.1f, target->GetAngle(&dest._position) - target->GetOrientation());
    }

    void Register() OVERRIDE
    {
        OnDestinationTargetSelect += SpellDestinationTargetSelectFn(spell_monk_gift_of_the_serpent_summon::PlacementCorrection, EFFECT_0, TARGET_DEST_DEST_RANDOM);
    }
};

// 321 - created by Gift of the Serpent
class sat_monk_gift_of_the_serpent : public IAreaTriggerOnce
{
    bool CheckTriggering(WorldObject* object) OVERRIDE
    {
        Unit* unit = object->ToUnit();
        return unit && NeedsHealing(unit) && unit->IsFriendlyTo(GetCaster());
    }

    void OnTrigger(WorldObject* target) OVERRIDE
    {
        GetCaster()->CastSpell(target->ToUnit(), SPELL_MONK_GIFT_OF_THE_SERPENT_HEAL, true);
    }

    void OnExpire() OVERRIDE
    {
        GetCaster()->CastSpell(GetTarget()->GetPositionX(), GetTarget()->GetPositionY(), GetTarget()->GetPositionZ(), SPELL_MONK_GIFT_OF_THE_SERPENT_HEAL_EXPIRED, true);
    }
};

// 138177 - Item - Monk T15 Windwalker 2P Bonus
class spell_monk_t15_windwalker_2p_bonus : public AuraScript
{
    PrepareAuraScript(spell_monk_t15_windwalker_2p_bonus);

    bool m_right = false;

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
        if (!spellInfo)
            return false;

        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
            if (spellInfo->Effects[i].Effect == SPELL_EFFECT_ENERGIZE && spellInfo->Effects[i].MiscValue == POWER_CHI)
                return true;

        return false;
    }

    void HandleProc(ProcEventInfo& /*eventInfo*/)
    {
        GetUnitOwner()->CastSpell(GetUnitOwner(), m_right ? SPELL_MONK_ENERGY_SPHERE_SUMMON_RIGHT : SPELL_MONK_ENERGY_SPHERE_SUMMON_LEFT, true);
        m_right = !m_right;
    }

    void Register() OVERRIDE
    {
        DoCheckProc += AuraCheckProcFn(spell_monk_t15_windwalker_2p_bonus::CheckProc);
        OnProc += AuraProcFn(spell_monk_t15_windwalker_2p_bonus::HandleProc);
    }
};

// 138310, 140674 - Energy Sphere
class spell_monk_summon_energy_sphere : public SpellScript
{
    PrepareSpellScript(spell_monk_summon_energy_sphere);

    void SelectDest(SpellDestination& dest)
    {
        bool right = GetSpellInfo()->Id == SPELL_MONK_ENERGY_SPHERE_SUMMON_RIGHT;
        float minAngle = (right ? 200.0f : 20.0f) * float(M_PI) / 180.0f;
        float maxAngle = (right ? 340.0f : 160.0f) * float(M_PI) / 180.0f;
        float size = GetCaster()->GetObjectSize();

        Position pos;
        GetCaster()->GetFirstCollisionPosition(pos, frand(size + 1.5f, size + 4.0f), frand(minAngle, maxAngle));
        dest.Relocate(pos);
    }

    void Register() OVERRIDE
    {
        uint16 target = m_scriptSpellId == SPELL_MONK_ENERGY_SPHERE_SUMMON_RIGHT ? TARGET_DEST_CASTER_RIGHT : TARGET_DEST_CASTER_LEFT;
        OnDestinationTargetSelect += SpellDestinationTargetSelectFn(spell_monk_summon_energy_sphere::SelectDest, EFFECT_0, target);
    }
};

// 796 - created by Energy Sphere
class sat_monk_energy_sphere : public IAreaTriggerOnce
{
    bool CheckTriggering(WorldObject* object) OVERRIDE
    {
        return object == GetCaster();
    }

    void OnTrigger(WorldObject* target) OVERRIDE
    {
        target->ToUnit()->CastSpell(target->ToUnit(), SPELL_MONK_ENERGY_SPHERE, true);
    }
};

// 116092 - Afterlife
class spell_monk_afterlife : public AuraScript
{
    PrepareAuraScript(spell_monk_afterlife);

    void HandleProc(ProcEventInfo& eventInfo)
    {
        Unit* monk = GetUnitOwner();
        if (eventInfo.GetSpellInfo() && eventInfo.GetSpellInfo()->Id == SPELL_MONK_BLACKOUT_KICK)
        {
            monk->CastSpell(monk, SPELL_MONK_CREATE_CHI_SPHERE, true);
            return;
        }

        Position dest;
        monk->GetPosition(&dest);
        monk->MovePositionToFirstCollision(dest, 4.0f + monk->GetObjectSize(), float(rand_norm()) * 2.0f * float(M_PI));
        monk->CastSpell(dest.GetPositionX(), dest.GetPositionY(), dest.GetPositionZ(), SPELL_MONK_AFTERLIFE_SUMMON, true);
    }

    void Register() OVERRIDE
    {
        OnProc += AuraProcFn(spell_monk_afterlife::HandleProc);
    }
};

// 383 - created by Afterlife (117032)
class sat_monk_afterlife_healing_sphere : public IAreaTriggerOnce
{
    bool CheckTriggering(WorldObject* object) OVERRIDE
    {
        return object == GetCaster();
    }

    void OnTrigger(WorldObject* /*target*/) OVERRIDE
    {
        GetCaster()->CastSpell(GetCaster(), SPELL_MONK_AFTERLIFE_HEALING_SPHERE, true);
    }
};

// 61146 - Black Ox Statue
struct npc_black_ox_statue : public ScriptedAI
{
    npc_black_ox_statue(Creature* creature) : ScriptedAI(creature) { }

    void InitializeAI() OVERRIDE
    {
        me->SetReactState(REACT_PASSIVE);
    }

    void Unsummoned() OVERRIDE
    {
        if (Unit* owner = me->GetOwner())
            owner->RemoveAurasDueToSpell(SPELL_MONK_SANCTUARY_OF_THE_OX, me->GetGUID());
    }

    void UpdateAI(uint32 /*diff*/) OVERRIDE { }
};

// 126119 - Sanctuary of the Ox
class spell_monk_sanctuary_of_the_ox : public AuraScript
{
    PrepareAuraScript(spell_monk_sanctuary_of_the_ox);

    uint32 m_damageDealt = 0;

    void HandleProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetDamageInfo())
            return;

        m_damageDealt += eventInfo.GetDamageInfo()->GetDamage();
        if (m_damageDealt >= 16 * GetUnitOwner()->GetTotalAttackPowerValue(WeaponAttackType::BASE_ATTACK))
        {
            m_damageDealt = 0;
            if (Unit* statue = GetCaster())
                statue->CastSpell(statue, SPELL_MONK_BLACK_OX_STATUE_GUARD_SELECTOR, true);
        }
    }

    void Register() OVERRIDE
    {
        OnProc += AuraProcFn(spell_monk_sanctuary_of_the_ox::HandleProc);
    }
};

// 118605 - Guard
class spell_monk_black_ox_statue_guard_selector : public SpellScript
{
    PrepareSpellScript(spell_monk_black_ox_statue_guard_selector);

    void SelectTarget(std::list<WorldObject*>& targets)
    {
        Unit* monk = GetCaster()->GetOwner();
        if (!monk)
        {
            targets.clear();
            return;
        }

        targets.remove_if([monk](WorldObject* target)
        {
            Unit* unit = target->ToUnit();
            return !unit || unit == monk || !unit->IsInRaidWith(monk);
        });
        targets.sort(HealthPctLess);
        if (!targets.empty())
            targets.resize(1);
    }

    void HandleHit()
    {
        if (Unit* monk = GetCaster()->GetOwner())
            monk->CastSpell(GetHitUnit(), monk->HasAura(SPELL_MONK_GLYPH_OF_GUARD) ?
                SPELL_MONK_BLACK_OX_STATUE_GUARD_MAGIC : SPELL_MONK_BLACK_OX_STATUE_GUARD, true);
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_black_ox_statue_guard_selector::SelectTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
        OnHit += SpellHitFn(spell_monk_black_ox_statue_guard_selector::HandleHit);
    }
};

// 125950 - Soothing Mist (Jade Serpent Statue)
class spell_monk_soothing_mist_statue : public AuraScript
{
    PrepareAuraScript(spell_monk_soothing_mist_statue);

    void HandleTick(AuraEffect const* /*aurEff*/)
    {
        if (Unit* statue = GetCaster())
            if (Unit* monk = statue->GetOwner())
                monk->CastSpell(GetTarget(), SPELL_MONK_SOOTHING_MIST_STATUE_HEAL, true);
    }

    void Register() OVERRIDE
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_monk_soothing_mist_statue::HandleTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

// 125962 - Soothing Mist <Statue> Trigger
class spell_monk_soothing_mist_statue_trigger : public SpellScript
{
    PrepareSpellScript(spell_monk_soothing_mist_statue_trigger);

    void SelectTarget(std::list<WorldObject*>& targets)
    {
        targets.sort(HealthPctLess);
        if (!targets.empty())
            targets.resize(1);
    }

    void HandleHit()
    {
        GetCaster()->CastSpell(GetHitUnit(), SPELL_MONK_SOOTHING_MIST_STATUE_CHANNEL, true);
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_soothing_mist_statue_trigger::SelectTarget, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
        OnHit += SpellHitFn(spell_monk_soothing_mist_statue_trigger::HandleHit);
    }
};

// 123904 - Invoke Xuen, the White Tiger
class spell_monk_invoke_xuen : public SpellScript
{
    PrepareSpellScript(spell_monk_invoke_xuen);

    void HandleAfterCast()
    {
        Unit* target = GetExplTargetUnit();
        uint32 entry = uint32(GetSpellInfo()->Effects[EFFECT_0].MiscValue);

        std::list<TempSummon*> summons;
        GetCaster()->GetSummons(summons, entry);
        for (TempSummon* summon : summons)
        {
            summon->CastSpell(summon, SPELL_MONK_SPAWN_SMOKE, true);
            if (target && summon->IsAIEnabled)
                summon->AI()->AttackStart(target);
        }
    }

    void Register() OVERRIDE
    {
        AfterCast += SpellCastFn(spell_monk_invoke_xuen::HandleAfterCast);
    }
};

// 123999 - Crackling Tiger Lightning Driver
class spell_monk_crackling_tiger_lightning : public AuraScript
{
    PrepareAuraScript(spell_monk_crackling_tiger_lightning);

    void HandleTick(AuraEffect const* /*aurEff*/)
    {
        if (Unit* caster = GetCaster())
            if (caster->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_CONFUSED | UNIT_FLAG_STUNNED | UNIT_FLAG_FLEEING))
                PreventDefaultAction();
    }

    void Register() OVERRIDE
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_monk_crackling_tiger_lightning::HandleTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};

// 63508 - Xuen
struct npc_monk_xuen : public PetAI
{
    npc_monk_xuen(Creature* creature) : PetAI(creature) { }

    void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
    {
        me->CastSpell(me, SPELL_MONK_XUEN_THREAT, true);
        me->CastSpell(me, SPELL_MONK_XUEN_CRACKLING_TIGER_LIGHTNING, true);
        me->CastSpell(me, SPELL_MONK_XUEN_EMINENCE, true);
    }
};

static uint32 VisualForSEFSpell(uint32 spellId)
{
    switch (spellId)
    {
        case SPELL_MONK_SEF_SUMMON_STORM_SPIRIT: return SPELL_MONK_SEF_STORM_VISUAL;
        case SPELL_MONK_SEF_SUMMON_FIRE_SPIRIT: return SPELL_MONK_SEF_FIRE_VISUAL;
        case SPELL_MONK_SEF_SUMMON_EARTH_SPIRIT: return SPELL_MONK_SEF_EARTH_VISUAL;
    }
    return 0;
}

static uint32 VisualForSEFEntry(uint32 entry)
{
    switch (entry)
    {
        case MONK_NPC_STORM_SPIRIT: return SPELL_MONK_SEF_STORM_VISUAL;
        case MONK_NPC_FIRE_SPIRIT: return SPELL_MONK_SEF_FIRE_VISUAL;
        case MONK_NPC_EARTH_SPIRIT: return SPELL_MONK_SEF_EARTH_VISUAL;
    }
    return 0;
}

static void GetSEFSpirits(Unit* monk, std::list<TempSummon*>& spirits)
{
    monk->GetSummons(spirits, MONK_NPC_STORM_SPIRIT);
    monk->GetSummons(spirits, MONK_NPC_EARTH_SPIRIT);
    monk->GetSummons(spirits, MONK_NPC_FIRE_SPIRIT);
}

// 137639 - Storm, Earth, and Fire
class spell_monk_s_e_f_aura : public AuraScript
{
    PrepareAuraScript(spell_monk_s_e_f_aura);

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        if (GetStackAmount() != 2)
            return;

        Unit* monk = GetUnitOwner();
        uint32 spellId = GetId();
        uint8 effIndex = aurEff->GetEffIndex();
        // The amount can't be changed from inside its own apply handler
        monk->m_Events.Schedule(1, [monk, spellId, effIndex]()
        {
            if (AuraEffect* effect = monk->GetAuraEffect(spellId, effIndex))
                effect->ChangeAmount(-45);
        });
    }

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* monk = GetUnitOwner();
        std::list<TempSummon*> spirits;
        GetSEFSpirits(monk, spirits);
        for (TempSummon* spirit : spirits)
            if (spirit->IsAIEnabled)
                spirit->AI()->DoAction(-1);

        monk->RemoveAurasDueToSpell(SPELL_MONK_SEF_STORM_VISUAL);
        monk->RemoveAurasDueToSpell(SPELL_MONK_SEF_FIRE_VISUAL);
        monk->RemoveAurasDueToSpell(SPELL_MONK_SEF_EARTH_VISUAL);
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
        if (!spellInfo)
            return false;

        switch (spellInfo->Id)
        {
            case 100780: // Jab
            case 108557: // Jab (staff)
            case 115687: // Jab (polearm)
            case 115693: // Jab (axe)
            case 115695: // Jab (mace)
            case 115698: // Jab (sword)
            case 100787: // Tiger Palm
            case 117952: // Crackling Jade Lightning
            case 100784: // Blackout Kick
            case 116847: // Rushing Jade Wind
            case 107428: // Rising Sun Kick
            case 123986: // Chi Burst
            case 101546: // Spinning Crane Kick
            case 115098: // Chi Wave
            case 113656: // Fists of Fury
            case 124081: // Zen Sphere
                return true;
            default:
                return false;
        }
    }

    void HandleProc(ProcEventInfo& eventInfo)
    {
        uint32 spellId = eventInfo.GetSpellInfo()->Id;

        std::list<TempSummon*> spirits;
        GetSEFSpirits(GetUnitOwner(), spirits);
        for (TempSummon* spirit : spirits)
        {
            Unit* victim = spirit->GetVictim();
            if (!victim || victim == eventInfo.GetActionTarget())
                continue;

            spirit->CastSpell(spellId == SPELL_MONK_ZEN_SPHERE_HEAL ? spirit : victim, spellId, true);
        }
    }

    void Register() OVERRIDE
    {
        OnEffectApply += AuraEffectApplyFn(spell_monk_s_e_f_aura::HandleApply, EFFECT_1, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
        OnEffectApply += AuraEffectApplyFn(spell_monk_s_e_f_aura::HandleApply, EFFECT_3, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
        OnEffectApply += AuraEffectApplyFn(spell_monk_s_e_f_aura::HandleApply, EFFECT_4, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
        OnEffectRemove += AuraEffectRemoveFn(spell_monk_s_e_f_aura::HandleRemove, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        DoCheckProc += AuraCheckProcFn(spell_monk_s_e_f_aura::CheckProc);
        OnProc += AuraProcFn(spell_monk_s_e_f_aura::HandleProc);
    }
};

// 137639 - Storm, Earth, and Fire
class spell_monk_s_e_f : public SpellScript
{
    PrepareSpellScript(spell_monk_s_e_f);

    uint64 m_toDespawn = 0;
    bool m_despawnAll = false;

    static uint32 SelectSpell(std::list<uint32>& spells)
    {
        if (spells.empty())
            return 0;

        uint32 spellId = Skyfire::Containers::SelectRandomContainerElement(spells);
        spells.remove(spellId);
        return spellId;
    }

    SpellCastResult CheckCast()
    {
        if (Unit* target = GetExplTargetUnit())
        {
            std::list<TempSummon*> spirits;
            GetSEFSpirits(GetCaster(), spirits);
            for (TempSummon* spirit : spirits)
            {
                if (spirit->IsAIEnabled && spirit->AI()->GetGUID() == target->GetGUID())
                {
                    m_toDespawn = spirit->GetGUID();
                    return SpellCastResult::SPELL_CAST_OK;
                }
            }

            if (Aura* aura = GetCaster()->GetAura(GetSpellInfo()->Id))
                if (aura->GetStackAmount() > 1)
                    m_despawnAll = true;
        }
        return SpellCastResult::SPELL_CAST_OK;
    }

    void HandleHit(SpellEffIndex /*effIndex*/)
    {
        std::list<TempSummon*> spirits;
        GetSEFSpirits(GetCaster(), spirits);

        if (m_toDespawn || m_despawnAll)
        {
            for (TempSummon* spirit : spirits)
                if ((m_despawnAll || spirit->GetGUID() == m_toDespawn) && spirit->IsAIEnabled)
                    spirit->AI()->DoAction(-1);
            return;
        }

        std::list<uint32> spells = { SPELL_MONK_SEF_SUMMON_STORM_SPIRIT, SPELL_MONK_SEF_SUMMON_EARTH_SPIRIT, SPELL_MONK_SEF_SUMMON_FIRE_SPIRIT };

        if (Aura* aura = GetCaster()->GetAura(GetSpellInfo()->Id))
        {
            if (aura->GetStackAmount() >= 2)
                return;

            for (TempSummon* spirit : spirits)
            {
                switch (spirit->GetEntry())
                {
                    case MONK_NPC_EARTH_SPIRIT: spells.remove(SPELL_MONK_SEF_SUMMON_EARTH_SPIRIT); break;
                    case MONK_NPC_FIRE_SPIRIT: spells.remove(SPELL_MONK_SEF_SUMMON_FIRE_SPIRIT); break;
                    case MONK_NPC_STORM_SPIRIT: spells.remove(SPELL_MONK_SEF_SUMMON_STORM_SPIRIT); break;
                }
            }

            for (uint32 spellId : { SPELL_MONK_SEF_SUMMON_STORM_SPIRIT, SPELL_MONK_SEF_SUMMON_EARTH_SPIRIT, SPELL_MONK_SEF_SUMMON_FIRE_SPIRIT })
                if (GetCaster()->HasAura(VisualForSEFSpell(spellId)))
                    spells.remove(spellId);

            Summon(SelectSpell(spells));
        }
        else if (uint32 monkSpell = SelectSpell(spells))
        {
            GetCaster()->CastSpell(GetCaster(), VisualForSEFSpell(monkSpell), true);
            Summon(SelectSpell(spells));
        }
    }

    void Summon(uint32 spellId)
    {
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
        if (!spellInfo)
            return;

        GetCaster()->CastSpell(GetHitUnit(), spellId, true);

        std::list<TempSummon*> summons;
        GetCaster()->GetSummons(summons, uint32(spellInfo->Effects[EFFECT_0].MiscValue));
        if (!summons.empty() && summons.back()->IsAIEnabled)
        {
            summons.back()->AI()->SetGUID(GetHitUnit()->GetGUID());
            summons.back()->AI()->AttackStart(GetHitUnit());
        }
    }

    void PreventAura(WorldObject*& target)
    {
        if (m_toDespawn || m_despawnAll)
            target = NULL;
    }

    void Register() OVERRIDE
    {
        OnCheckCast += SpellCheckCastFn(spell_monk_s_e_f::CheckCast);
        OnEffectHitTarget += SpellEffectFn(spell_monk_s_e_f::HandleHit, EFFECT_0, SPELL_EFFECT_DUMMY);
        OnObjectTargetSelect += SpellObjectTargetSelectFn(spell_monk_s_e_f::PreventAura, EFFECT_1, TARGET_UNIT_CASTER);
        OnObjectTargetSelect += SpellObjectTargetSelectFn(spell_monk_s_e_f::PreventAura, EFFECT_2, TARGET_UNIT_CASTER);
        OnObjectTargetSelect += SpellObjectTargetSelectFn(spell_monk_s_e_f::PreventAura, EFFECT_3, TARGET_UNIT_CASTER);
        OnObjectTargetSelect += SpellObjectTargetSelectFn(spell_monk_s_e_f::PreventAura, EFFECT_4, TARGET_UNIT_CASTER);
    }
};

// 69680, 69791, 69792 - Storm, Earth and Fire spirits
struct npc_monk_s_e_f_spirit : public ScriptedAI
{
    enum class State
    {
        Jumping,
        Attacking,
        Despawning,
    };

    npc_monk_s_e_f_spirit(Creature* creature) : ScriptedAI(creature) { }

    uint64 m_victimGuid = 0;
    State m_state = State::Jumping;
    bool m_initialized = false;
    bool m_unsummoned = false;
    uint32 m_damageTimer = 1000;
    uint32 m_acquireTimer = 5000;

    // The owner is only assigned after the summon has been added to the map
    void Setup()
    {
        if (m_initialized)
            return;

        Unit* owner = me->GetOwner();
        if (!owner)
            return;

        m_initialized = true;
        owner->CastSpell(me, SPELL_MONK_TRANSCENDENCE_CLONE, true);
        me->CastSpell(me, SPELL_MONK_TRANSCENDENCE_CLONE_TRIGGER, false);
        me->CastSpell(me, VisualForSEFEntry(me->GetEntry()), true);
        me->CastSpell(me, SPELL_MONK_SEF_SPIRIT_PASSIVE, false);
        UpdateDamage();
    }

    void SetGUID(uint64 guid, int32 /*id*/) OVERRIDE
    {
        Setup();

        m_victimGuid = guid;
        if (Unit* target = ObjectAccessor::GetUnit(*me, guid))
        {
            me->CastSpell(target, SPELL_MONK_SEF_FIXATE_AND_SCALING, true);
            me->CastSpell(target, SPELL_MONK_FLYING_KICK, true);
        }
    }

    uint64 GetGUID(int32 /*id*/) const OVERRIDE
    {
        return m_victimGuid;
    }

    void Unsummoned() OVERRIDE
    {
        if (m_unsummoned)
            return;

        m_unsummoned = true;
        m_state = State::Despawning;
        if (Unit* owner = me->GetOwner())
            if (Aura* aura = owner->GetAura(SPELL_MONK_STORM_EARTH_AND_FIRE))
                aura->ModStackAmount(-1);
    }

    void DoAction(int32 action) OVERRIDE
    {
        if (action == -1)
            DoDespawn();
    }

    void AttackStart(Unit* target) OVERRIDE
    {
        if (m_state != State::Attacking || !target)
            return;

        if (me->Attack(target, true))
        {
            me->GetMotionMaster()->MoveChase(target, CONTACT_DISTANCE, float(M_PI));
            m_victimGuid = target->GetGUID();
        }
    }

    void MovementInform(uint32 type, uint32 /*id*/) OVERRIDE
    {
        if (type != EFFECT_MOTION_TYPE)
            return;

        if (m_state == State::Jumping)
            AcquireTarget();
        else if (m_state == State::Despawning)
            me->DespawnOrUnsummon();
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        Setup();

        if (m_damageTimer <= diff)
        {
            UpdateDamage();
            m_damageTimer = 1000;
        }
        else
            m_damageTimer -= diff;

        if (m_state == State::Jumping)
        {
            if (m_acquireTimer <= diff)
                AcquireTarget();
            else
                m_acquireTimer -= diff;
        }

        if (m_state != State::Attacking)
            return;

        Unit* target = ObjectAccessor::GetUnit(*me, m_victimGuid);
        if (!target || !target->IsAlive())
        {
            DoDespawn();
            return;
        }

        if (me->GetVictim() != target)
            AttackStart(target);

        DoMeleeAttackIfReady();
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        me->DespawnOrUnsummon();
    }

    void EnterEvadeMode() OVERRIDE
    {
        if (m_state == State::Attacking && me->IsAlive())
            DoDespawn();
    }

    void AcquireTarget()
    {
        m_state = State::Attacking;

        Unit* target = ObjectAccessor::GetUnit(*me, m_victimGuid);
        if (!target)
            target = me->SelectVictim();
        AttackStart(target);
    }

    void UpdateDamage()
    {
        Unit* monk = me->GetOwner();
        if (!monk)
            return;

        me->SetCanDualWield(monk->CanDualWield());

        for (uint8 i = 0; i < 2; ++i)
        {
            WeaponAttackType attType = i == 0 ? WeaponAttackType::BASE_ATTACK : WeaponAttackType::OFF_ATTACK;
            float damageMin = monk->GetWeaponDamageRange(attType, WeaponDamageRange::MINDAMAGE);
            float damageMax = monk->GetWeaponDamageRange(attType, WeaponDamageRange::MAXDAMAGE);
            if (damageMin != me->GetWeaponDamageRange(attType, WeaponDamageRange::MINDAMAGE) || damageMax != me->GetWeaponDamageRange(attType, WeaponDamageRange::MAXDAMAGE))
            {
                me->SetBaseWeaponDamage(attType, WeaponDamageRange::MINDAMAGE, damageMin);
                me->SetBaseWeaponDamage(attType, WeaponDamageRange::MAXDAMAGE, damageMax);
                me->UpdateDamagePhysical(attType);
            }
        }

        int32 pct = int32(monk->GetTotalAuraMultiplier(SPELL_AURA_MOD_DAMAGE_PERCENT_DONE) * 100.0f) - 100;
        if (AuraEffect* scaling = me->GetAuraEffect(SPELL_MONK_SEF_FIXATE_AND_SCALING, EFFECT_1))
            if (scaling->GetAmount() != pct)
                scaling->ChangeAmount(pct);
    }

    void DoDespawn()
    {
        if (m_state == State::Despawning)
            return;

        if (!me->IsAlive())
        {
            me->DespawnOrUnsummon();
            return;
        }

        m_state = State::Despawning;
        me->GetMotionMaster()->MovementExpired();
        me->CombatStop();
        if (Unit* owner = me->GetOwner())
            me->CastSpell(owner, SPELL_MONK_FLYING_KICK, true);
        me->DespawnOrUnsummon(uint32(std::max(me->GetSplineDuration(), 1)));
    }
};

void AddSC_monk_spell_scripts()
{
    new spell_monk_roll();
    new spell_monk_touch_of_karma();
    new spell_monk_legacy_of_the_emperor();
    new spell_monk_paralysis();
    new spell_monk_flying_serpent_kick();
    new spell_monk_provoke();
    new spell_monk_disable();
    new spell_monk_dampen_harm();

    //new spell_monk_fists_of_fury_stun();
    new spell_monk_expel_harm();
    new spell_monk_grapple_weapon();
    //new npc_transcendence_spirit();
    //new spell_monk_transcendence();
    //new spell_monk_transcendence_transfer();
    new spell_monk_serpents_zeal();
    new spell_monk_diffuse_magic();
    new spell_monk_guard();
    new spell_monk_bear_hug();
    new spell_monk_zen_flight_check();
    new spell_monk_glyph_of_zen_flight();
    new spell_monk_power_strikes();
    new spell_monk_crackling_jade_lightning();
    new spell_monk_spinning_fire_blossom_damage();
    new spell_monk_spinning_fire_blossom();
    //new spell_monk_thunder_focus_tea();
    new spell_monk_teachings_of_the_monastery();
    new spell_monk_mana_tea();
    new spell_monk_mana_tea_stacks();
    new spell_monk_enveloping_mist();
    new spell_monk_surging_mist();
    //new spell_monk_renewing_mist();
    new spell_monk_healing_elixirs();
    new spell_monk_zen_sphere();
    new spell_monk_zen_sphere_hot();
    new spell_monk_chi_burst();
    new spell_monk_energizing_brew();
    new spell_monk_spear_hand_strike();
    new spell_monk_tigereye_brew();
    new spell_monk_tigers_lust();
    RegisterSpellScript(spell_monk_chi_torpedo);
    new spell_monk_stagger();
    new spell_monk_stance_of_the_sturdy_ox();
    new spell_monk_stagger_dummy();
    new spell_monk_purifying_brew();
    new spell_monk_clash();
    new spell_monk_keg_smash();
    new spell_monk_elusive_brew();
    new spell_monk_breath_of_fire();
    new spell_monk_soothing_mist();
    new spell_monk_zen_pilgrimage();
    new spell_monk_zen_pilgrimage_return();
    new spell_monk_blackout_kick();
    new spell_monk_fortifying_brew();
    new spell_monk_touch_of_death();
    new spell_monk_tigereye_brew_stacks();
    new spell_monk_tiger_strikes();
    RegisterSpellScript(spell_monk_specific_damage);
    RegisterAuraScript(spell_monk_fists_of_fury);
    RegisterSpellScript(spell_monk_fists_of_fury_damage);
    RegisterSpellScript(spell_monk_fists_of_fury_stun);
    RegisterSpellScript(spell_monk_rising_sun_kick);
    RegisterAuraScript(spell_monk_combo_breaker);
    RegisterAuraScript(spell_monk_brewing_elusive_brew);
    RegisterSpellScript(spell_monk_dizzying_haze);
    RegisterSpellScript(spell_monk_muscle_memory);
    RegisterSpellScript(spell_monk_spinning_crane_kick_damage);
    RegisterAuraScript(spell_monk_renewing_mist);
    RegisterSpellScript(spell_monk_renewing_mist_jump);
    RegisterSpellScript(spell_monk_thunder_focus_tea_uplift);
    RegisterSpellScript(spell_monk_revival);
    RegisterSpellScript(spell_monk_life_cocoon);
    RegisterSpellScript(spell_monk_detox);
    new atrigger_script<sat_monk_chi_torpedo>("sat_monk_chi_torpedo");
    new atrigger_script<sat_monk_ring_of_peace>("sat_monk_ring_of_peace");
    RegisterAuraScript(spell_monk_ring_of_peace_aura);
    RegisterSpellScript(spell_monk_healing_sphere);
    new atrigger_script<sat_monk_healing_sphere>("sat_monk_healing_sphere");
    new atrigger_script<sat_monk_chi_sphere>("sat_monk_chi_sphere");
    RegisterAuraScript(spell_monk_gift_of_the_ox);
    RegisterSpellScript(spell_monk_summon_gift_of_the_ox);
    new atrigger_script<sat_monk_gift_of_the_ox>("sat_monk_gift_of_the_ox");
    RegisterAuraScript(spell_monk_mastery_gift_of_the_serpent);
    RegisterSpellScript(spell_monk_gift_of_the_serpent_selector);
    RegisterSpellScript(spell_monk_gift_of_the_serpent_summon);
    new atrigger_script<sat_monk_gift_of_the_serpent>("sat_monk_gift_of_the_serpent");
    RegisterAuraScript(spell_monk_t15_windwalker_2p_bonus);
    RegisterSpellScript(spell_monk_summon_energy_sphere);
    new atrigger_script<sat_monk_energy_sphere>("sat_monk_energy_sphere");
    RegisterAuraScript(spell_monk_afterlife);
    new atrigger_script<sat_monk_afterlife_healing_sphere>("sat_monk_afterlife_healing_sphere");
    new creature_script<npc_black_ox_statue>("npc_black_ox_statue");
    RegisterAuraScript(spell_monk_sanctuary_of_the_ox);
    RegisterSpellScript(spell_monk_black_ox_statue_guard_selector);
    RegisterAuraScript(spell_monk_soothing_mist_statue);
    RegisterSpellScript(spell_monk_soothing_mist_statue_trigger);
    RegisterSpellScript(spell_monk_invoke_xuen);
    RegisterAuraScript(spell_monk_crackling_tiger_lightning);
    new creature_script<npc_monk_xuen>("npc_monk_xuen");
    RegisterAuraScript(spell_monk_s_e_f_aura);
    RegisterSpellScript(spell_monk_s_e_f);
    new creature_script<npc_monk_s_e_f_spirit>("npc_monk_s_e_f_spirit");
}
