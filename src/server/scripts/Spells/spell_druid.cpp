/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/*
 * Scripts for spells with SPELLFAMILY_DRUID and SPELLFAMILY_GENERIC spells used by druid players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_dru_".
 */

#include "Player.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "SpellMgr.h"
#include "Containers.h"
#include "ObjectAccessor.h"
#include "EventProcessor.h"
#include "ScriptedCreature.h"
#include "Battleground.h"
#include "Group.h"
#include "TemporarySummon.h"
#include "PassiveAI.h"
#include "PetDefines.h"
#include "DynamicObject.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

enum DruidSpells
{
    SPELL_DRUID_WRATH                       = 5176,
    SPELL_DRUID_STARFIRE                    = 2912,
    SPELL_DRUID_STARSURGE                   = 78674,
    SPELL_DRUID_ECLIPSE_GENERAL_ENERGIZE    = 89265,
    SPELL_DRUID_ECLIPSE_MANA_ENERGIZE       = 81070,
    SPELL_DRUID_EUPHORIA                    = 81062,
    SPELL_DRUID_LUNAR_ECLIPSE_MARKER        = 67484, // Will make the yellow arrow on eclipse bar point to the blue side (lunar)
    SPELL_DRUID_SOLAR_ECLIPSE_MARKER        = 67483, // Will make the yellow arrow on eclipse bar point to the yellow side (solar)
    SPELL_DRUID_SOLAR_ECLIPSE               = 48517,
    SPELL_DRUID_LUNAR_ECLIPSE               = 48518,
    SPELL_DRUID_LUNAR_ECLIPSE_OVERRIDE      = 107095,
    SPELL_DRUID_STARFALL                     = 48505,
    SPELL_DRUID_SOLAR_BEAM                   = 78675,
    SPELL_DRUID_SOLAR_BEAM_SILENCE          = 81261,
    SPELL_DRUID_SOLAR_BEAM_SYMBIOSIS        = 113286,
    SPELL_DRUID_SOLAR_BEAM_SILENCE_SYMBIOSIS = 113287,
    SPELL_DRUID_FORCE_OF_NATURE_VISUAL      = 37846,
    SPELL_DRUID_FORCE_OF_NATURE_ROOT        = 113770,
    SPELL_DRUID_FORCE_OF_NATURE_WRATH       = 113769,
    SPELL_DRUID_FORCE_OF_NATURE_SWIFTMEND   = 142421,
    SPELL_DRUID_FORCE_OF_NATURE_HEAL        = 113828,
    SPELL_DRUID_FORCE_OF_NATURE_TAUNT       = 113830,
    SPELL_DRUID_FERAL_CHARGE_BEAR           = 16979,
    SPELL_DRUID_FERAL_CHARGE_CAT            = 49376,
    SPELL_DRUID_GLYPH_OF_INNERVATE          = 54833,
    SPELL_DRUID_GLYPH_OF_STARFIRE           = 54846,
    SPELL_DRUID_INCREASED_MOONFIRE_DURATION = 38414,
    SPELL_DRUID_LIFEBLOOM_ENERGIZE          = 64372,
    SPELL_DRUID_LIFEBLOOM_FINAL_HEAL        = 33778,
    SPELL_DRUID_LIFEBLOOM                   = 33763,
    SPELL_DRUID_GLYPH_OF_BLOOMING           = 121840,
    SPELL_DRUID_LIVING_SEED_HEAL            = 48503,
    SPELL_DRUID_LIVING_SEED_PROC            = 48504,
    SPELL_DRUID_NATURES_GRACE               = 16880,
    SPELL_DRUID_NATURES_GRACE_TRIGGER       = 16886,
    SPELL_DRUID_SURVIVAL_INSTINCTS          = 50322,
    SPELL_DRUID_SAVAGE_DEFENSE_AURA         = 132402, // Dodge chance buff from Savage Defense
    SPELL_DRUID_SAVAGE_ROAR                 = 52610,
    SPELL_DRUID_SAVAGE_ROAR_TRIGGER         = 62071,
    SPELL_DRUID_STAMPEDE_BAER_RANK_1        = 81016,
    SPELL_DRUID_STAMPEDE_CAT_RANK_1         = 81021,
    SPELL_DRUID_STAMPEDE_CAT_STATE          = 109881,
    SPELL_DRUID_TIGER_S_FURY_ENERGIZE       = 51178,
    SPELL_DRUID_CAT_FORM                    = 768,
    SPELL_DRUID_BEAR_FORM                   = 5487,
    SPELL_DRUID_MOONKIN_FORM                = 24858,
    SPELL_DRUID_INCARNATION_KING_OF_JUNGLE  = 102543,
    SPELL_DRUID_INCARNATION_SON_OF_URSOC    = 102558,
    SPELL_DRUID_INCARNATION_CHOSEN_OF_ELUNE = 102560,
    SPELL_DRUID_INCARNATION_TREE_OF_LIFE    = 33891,
    SPELL_DRUID_INCARNATION_AURA            = 117679,
    SPELL_DRUID_CHOSEN_OF_ELUNE_DAMAGE      = 122114,
    SPELL_DRUID_SKULL_BASH_MANA_COST        = 82365,
    SPELL_DRUID_SKULL_BASH_INTERRUPT        = 93985,
    SPELL_DRUID_SKULL_BASH_CHARGE           = 93983,
    SPELL_DRUID_RIP                         = 1079,
    SPELL_DRUID_GLYPH_OF_FEROCIOUS_BITE     = 67598,
    SPELL_DRUID_GLYPH_OF_FEROCIOUS_BITE_HEAL= 101024,
    SPELL_DRUID_RAKE                        = 1822,
    SPELL_DRUID_REJUVENATION                = 774,
    SPELL_DRUID_MANGLE_BEAR                 = 33878,
    SPELL_DRUID_MANGLE_BEAR_STUB            = 33917,
    SPELL_DRUID_MANGLE_PASSIVE              = 93622, // Mangle! — supplies the 25% reset chance
    SPELL_DRUID_FAERIE_FIRE                 = 770,
    SPELL_DRUID_FAERIE_SWARM                = 102355,
    SPELL_DRUID_FAERIE_SWARM_SLOW           = 102354,
    SPELL_DRUID_WEAKENED_ARMOR              = 113746,
    SPELL_DRUID_YSERAS_GIFT                 = 145108,
    SPELL_DRUID_YSERAS_GIFT_HEAL_SELF       = 145109,
    SPELL_DRUID_YSERAS_GIFT_HEAL_ALLY       = 145110,
    SPELL_DRUID_SOUL_OF_THE_FOREST          = 114107,
    SPELL_DRUID_SOUL_OF_THE_FOREST_HASTE    = 114108,
    SPELL_DRUID_SOUL_OF_THE_FOREST_ENERGIZE = 114113,
    SPELL_DRUID_ASTRAL_INSIGHT              = 145138,
    SPELL_DRUID_CELESTIAL_ALIGNMENT         = 112071,
    SPELL_DRUID_MUSHROOM_BIRTH              = 94081,
    SPELL_DRUID_MUSHROOM_INVISIBLE          = 92661,
    SPELL_DRUID_MUSHROOM_DEATH              = 116305,
    SPELL_DRUID_MUSHROOM_SUICIDE            = 116302,
    SPELL_DRUID_MUSHROOM_DAMAGE             = 78777,
    SPELL_DRUID_MUSHROOM_HEAL               = 102792,
    SPELL_DRUID_MUSHROOM_OVERHEAL_AURA      = 138611,
    SPELL_DRUID_MUSHROOM_ACCUMULATOR        = 138616,
    SPELL_DRUID_MUSHROOM_BLOOM_UI_FLASH     = 138664,
    SPELL_DRUID_FUNGAL_GROWTH               = 81283,
    SPELL_DRUID_FUNGAL_GROWTH_SLOW          = 81282,
    SPELL_DRUID_MUSHROOM_SLOW_VISUAL        = 94339,
    SPELL_DRUID_WILD_MUSHROOM_RESTO         = 145205,
    SPELL_DRUID_WILD_MUSHROOM_RESTO_GLYPH   = 147349,
    SPELL_DRUID_FRENZIED_REGENERATION       = 22842,
    SPELL_DRUID_GLYPH_OF_FRENZIED_REGEN     = 54810,
    SPELL_DRUID_FRENZIED_REGEN_HEAL_TAKE    = 124769,
    SPELL_DRUID_BEAR_HUG                    = 102795,
    SPELL_DRUID_CENARION_WARD_HEAL          = 102352,
    SPELL_DRUID_URSOLS_VORTEX               = 102793,
    SPELL_DRUID_URSOLS_VORTEX_SNARE         = 127797,
    SPELL_DRUID_URSOLS_VORTEX_JUMP_DEST     = 118283,
    SPELL_DRUID_MOONFIRE                    = 8921,
    SPELL_DRUID_SUNFIRE                     = 93402,
    SPELL_DRUID_GLYPH_OF_GUIDED_STARS       = 146655,
    SPELL_DRUID_GLYPH_OF_OMENS              = 54812,
    SPELL_DRUID_GLYPH_OF_HURRICANE          = 54831,
    SPELL_DRUID_LUNAR_BOLT                  = 144770,
    SPELL_DRUID_SOLAR_BOLT                  = 144772,
    SPELL_DRUID_TIGERS_FURY                 = 5217,
    SPELL_DRUID_BERSERK_CAT                 = 106951,
    SPELL_DRUID_GLYPH_OF_SHRED              = 114234,
    SPELL_DRUID_GLYPH_OF_SHRED_OVERRIDE     = 114235,
    SPELL_DRUID_SAVAGE_ROAR_GLYPHED         = 127538,
    SPELL_DRUID_PREDATORY_SWIFTNESS         = 69369,
    SPELL_DRUID_PRIMAL_FURY_BEAR            = 16959,
    SPELL_DRUID_PRIMAL_FURY_CAT             = 16953,
    SPELL_DRUID_STAMPEDE                    = 81022,
    SPELL_DRUID_STAMPEDE_TIMER              = 131538,
    SPELL_DRUID_RAVAGE_STAMPEDE             = 102545,
    SPELL_DRUID_T16_FERAL_2P_BONUS          = 144864,
    SPELL_DRUID_FERAL_RAGE                  = 146874,
    SPELL_DRUID_TIGERS_FURY_COMBO           = 144859,
    SPELL_DRUID_DREAM_OF_CENARIUS_TALENT    = 108373,
    SPELL_DRUID_DREAM_OF_CENARIUS_BALANCE   = 145151,
    SPELL_DRUID_DREAM_OF_CENARIUS_FERAL     = 145152,
    SPELL_DRUID_DREAM_OF_CENARIUS_RESTO     = 145153,
    SPELL_DRUID_DREAM_OF_CENARIUS_GUARDIAN  = 145162,
    SPELL_DRUID_TOOTH_AND_CLAW_AURA         = 135286,
    SPELL_DRUID_TOOTH_AND_CLAW_ABSORB       = 135597,
    SPELL_DRUID_TOOTH_AND_CLAW_VISUAL       = 135601,
    SPELL_DRUID_THICK_HIDE                  = 16931,
    SPELL_DRUID_BEAR_FORM_PASSIVE           = 1178,
    SPELL_DRUID_GLYPH_OF_FAE_SILENCE        = 114237,
    SPELL_DRUID_FAE_SILENCE                 = 114238,
    SPELL_DRUID_T15_GUARDIAN_4P_BONUS       = 138222,
    SPELL_DRUID_EFFLORESCENCE               = 81262,
    SPELL_DRUID_EFFLORESCENCE_TREANT        = 142423,
    SPELL_DRUID_EFFLORESCENCE_HEAL          = 81269,
    SPELL_DRUID_EFFLORESCENCE_HEAL_TREANT   = 142424,
    SPELL_DRUID_GLYPH_OF_EFFLORESCENCE      = 145529,
    SPELL_DRUID_T15_RESTORATION_2P_BONUS    = 138284,
    SPELL_DRUID_T15_RESTORATION_4P_BONUS    = 138286,
    SPELL_DRUID_T16_RESTORATION_4P_BONUS    = 144875,
    SPELL_DRUID_SPARK_OF_LIFE               = 144876,
    SPELL_DRUID_HARMONY                     = 100977,
    SPELL_DRUID_GLYPH_OF_REGROWTH           = 116218,
    SPELL_DRUID_NATURES_SWIFTNESS           = 132158,
    SPELL_DRUID_HOTW_BALANCE                = 108291,
    SPELL_DRUID_HOTW_FERAL                  = 108292,
    SPELL_DRUID_HOTW_GUARDIAN               = 108293,
    SPELL_DRUID_HOTW_RESTORATION            = 108294,
    SPELL_DRUID_HOTW_CAT                    = 123737,
    SPELL_DRUID_HOTW_BEAR                   = 123738,
    SPELL_DRUID_KILLER_INSTINCT_PASSIVE     = 108299,
    SPELL_DRUID_KILLER_INSTINCT             = 108300,
    SPELL_DRUID_NATURES_VIGIL_HEAL          = 124988,
    SPELL_DRUID_NATURES_VIGIL_DAMAGE        = 124991,
    SPELL_DRUID_SYMBIOSIS_FOR_CASTER        = 110309,
    SPELL_DRUID_SYMBIOSIS_DEATH_KNIGHT      = 110478,
    SPELL_DRUID_SYMBIOSIS_HUNTER            = 110479,
    SPELL_DRUID_SYMBIOSIS_MAGE              = 110482,
    SPELL_DRUID_SYMBIOSIS_MONK              = 110483,
    SPELL_DRUID_SYMBIOSIS_PALADIN           = 110484,
    SPELL_DRUID_SYMBIOSIS_PRIEST            = 110485,
    SPELL_DRUID_SYMBIOSIS_ROGUE             = 110486,
    SPELL_DRUID_SYMBIOSIS_SHAMAN            = 110488,
    SPELL_DRUID_SYMBIOSIS_WARLOCK           = 110490,
    SPELL_DRUID_SYMBIOSIS_WARRIOR           = 110491,
    SPELL_DRUID_SHATTERING_BLOW             = 112997,
    SPELL_DRUID_TREANT_FORM                 = 114282,
    SPELL_DRUID_GLYPH_OF_PROWL              = 116186,
    SPELL_DRUID_STAMPEDING_ROAR             = 106898,
    SPELL_DRUID_GLYPH_OF_STAMPEDING_ROAR    = 114300,
    SPELL_DRUID_FORCE_OF_NATURE_RAKE        = 150017,
    SPELL_DRUID_TRAVEL_FORM                 = 783,
    SPELL_DRUID_GLYPH_OF_THE_STAG           = 114338,
    SPELL_DRUID_GLYPH_OF_THE_CHEETAH        = 131113,
    SPELL_DRUID_STAG_FORM                   = 115034,
    SPELL_DRUID_SOUND_EFFECT_STAG_FORM      = 126056,
    SPELL_DRUID_T16_GUARDIAN_2P_BONUS       = 144879,
    SPELL_DRUID_GLYPH_OF_REJUVENATION       = 17076,
    SPELL_DRUID_GLYPH_OF_REJUVENATION_EFFECT= 96206,
    SPELL_DRUID_TRACK_HUMANOIDS             = 5225,
    SPELL_DRUID_GLYPH_OF_THE_PREDATOR       = 114280,
    SPELL_DRUID_GLYPH_OF_THE_CHAMELEON      = 107059,
    SPELL_WARLOCK_DEMONIC_CIRCLE_SUMMON     = 48018,
    SPELL_MAGE_GLYPH_OF_ICE_BLOCK_IMMUNITY  = 115760,
};

enum DruidMisc
{
    ZONE_MOONGLADE                          = 493,
};

enum DruidCreatureIds
{
    NPC_DRUID_WILD_MUSHROOM                 = 47649,
    NPC_DRUID_FORCE_OF_NATURE_BALANCE       = 1964,
    NPC_DRUID_FORCE_OF_NATURE_RESTORATION   = 54983,
    NPC_DRUID_FORCE_OF_NATURE_FERAL         = 54984,
    NPC_DRUID_FORCE_OF_NATURE_GUARDIAN      = 54985
};

// Faerie Fire / Faerie Swarm: apply 3 stacks of Weakened Armor (113746).
static void ApplyWeakenedArmorStacks(Unit* caster, Unit* target)
{
    if (!caster || !target)
        return;

    if (Aura* aura = target->GetAura(SPELL_DRUID_WEAKENED_ARMOR))
    {
        aura->SetStackAmount(3);
        aura->RefreshDuration();
    }
    else
        caster->CastCustomSpell(SPELL_DRUID_WEAKENED_ARMOR, SPELLVALUE_AURA_STACK, 3, target, true);
}

// Lacerate / Thrash (Bear) / Faerie Fire (Bear): chance from Mangle! (93622) to clear Mangle CD.
static void TryResetMangleCooldown(Unit* caster)
{
    Player* player = caster ? caster->ToPlayer() : NULL;
    if (!player)
        return;

    int32 chance = 25;
    if (SpellInfo const* manglePassive = sSpellMgr->GetSpellInfo(SPELL_DRUID_MANGLE_PASSIVE))
        if (manglePassive->Effects[EFFECT_0].IsEffect())
            chance = manglePassive->Effects[EFFECT_0].CalcValue(caster);

    if (chance <= 0)
        chance = 25;

    if (!roll_chance_i(chance))
        return;

    // Clear both the real Bear Mangle and the action-bar stub clients may track.
    player->RemoveSpellCooldown(SPELL_DRUID_MANGLE_BEAR, true);
    player->RemoveSpellCooldown(SPELL_DRUID_MANGLE_BEAR_STUB, true);
}

static void HandleEclipseReached(Unit* druid)
{
    int32 power = druid->GetPower(POWER_ECLIPSE);
    if (power >= 100 && !druid->HasAura(SPELL_DRUID_SOLAR_ECLIPSE))
    {
        druid->RemoveAurasDueToSpell(SPELL_DRUID_SOLAR_ECLIPSE_MARKER);
        druid->CastSpell(druid, SPELL_DRUID_LUNAR_ECLIPSE_MARKER, true);
        druid->CastSpell(druid, SPELL_DRUID_SOLAR_ECLIPSE, true);
    }
    else if (power <= -100 && !druid->HasAura(SPELL_DRUID_LUNAR_ECLIPSE))
    {
        druid->RemoveAurasDueToSpell(SPELL_DRUID_LUNAR_ECLIPSE_MARKER);
        druid->CastSpell(druid, SPELL_DRUID_SOLAR_ECLIPSE_MARKER, true);
        druid->CastSpell(druid, SPELL_DRUID_LUNAR_ECLIPSE, true);
    }
}

// Positive amounts move the bar toward Solar, negative toward Lunar.
static void ModifyEclipsePower(Unit* druid, int32 amount)
{
    if (!amount)
        return;

    druid->CastCustomSpell(druid, SPELL_DRUID_ECLIPSE_GENERAL_ENERGIZE, &amount, NULL, NULL, true);

    if (!druid->HasAura(SPELL_DRUID_SOLAR_ECLIPSE_MARKER) && !druid->HasAura(SPELL_DRUID_LUNAR_ECLIPSE_MARKER))
        druid->CastSpell(druid, amount > 0 ? SPELL_DRUID_SOLAR_ECLIPSE_MARKER : SPELL_DRUID_LUNAR_ECLIPSE_MARKER, true);

    int32 power = druid->GetPower(POWER_ECLIPSE);
    if (power >= 0 && druid->HasAura(SPELL_DRUID_LUNAR_ECLIPSE))
    {
        druid->RemoveAurasDueToSpell(SPELL_DRUID_LUNAR_ECLIPSE);
        druid->RemoveAurasDueToSpell(SPELL_DRUID_LUNAR_ECLIPSE_OVERRIDE);
    }
    else if (power <= 0 && druid->HasAura(SPELL_DRUID_SOLAR_ECLIPSE))
        druid->RemoveAurasDueToSpell(SPELL_DRUID_SOLAR_ECLIPSE);

    HandleEclipseReached(druid);
}

// Glyph of Omens / Sudden Eclipse: push the bar in its current direction while no Eclipse is active.
static void ApplyEclipseBonus(Unit* druid, int32 amount)
{
    if (druid->HasAura(SPELL_DRUID_LUNAR_ECLIPSE) || druid->HasAura(SPELL_DRUID_SOLAR_ECLIPSE) || druid->HasAura(SPELL_DRUID_CELESTIAL_ALIGNMENT))
        return;

    if (druid->HasAura(SPELL_DRUID_LUNAR_ECLIPSE_MARKER) || !druid->HasAura(SPELL_DRUID_SOLAR_ECLIPSE_MARKER))
        amount = -amount;

    ModifyEclipsePower(druid, amount);
}

// Keeps the alive, most injured units first.
static void SelectMostInjuredTargets(std::list<WorldObject*>& targets, uint32 maxTargets)
{
    targets.remove_if([](WorldObject* obj)
    {
        Unit* unit = obj->ToUnit();
        return !unit || !unit->IsAlive();
    });

    targets.sort([](WorldObject* a, WorldObject* b)
    {
        return Skyfire::HealthPctOrderPred()(a->ToUnit(), b->ToUnit());
    });

    if (targets.size() > maxTargets)
        targets.resize(maxTargets);
}

class DelayedCastSpellEvent : public BasicEvent
{
public:
    DelayedCastSpellEvent(Unit* caster, uint64 targetGuid, uint32 spellId) : _caster(caster), _targetGuid(targetGuid), _spellId(spellId) { }

    bool Execute(uint64 /*time*/, uint32 /*diff*/) override
    {
        if (Unit* target = ObjectAccessor::GetUnit(*_caster, _targetGuid))
            _caster->CastSpell(target, _spellId, true);
        return true;
    }

private:
    Unit* _caster;
    uint64 _targetGuid;
    uint32 _spellId;
};

// 1850 - Dash
class spell_dru_dash : public SpellScriptLoader
{
public:
    spell_dru_dash() : SpellScriptLoader("spell_dru_dash") { }

    class spell_dru_dash_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_dash_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            // do not set speed if not in cat form
            if (GetUnitOwner()->GetShapeshiftForm() != FORM_CAT)
                amount = 0;
        }

        // CalculateAmount() above only runs once, when this AuraEffect is first
        // constructed - which happens while Dash's own spell is still being
        // handled, effect by effect. When Dash is used from outside Cat Form,
        // the client auto-shifts as part of casting it, but that shift is a
        // second effect of the SAME cast that resolves AFTER this aura object
        // already exists: the amount gets cached at 0 (still the old form) and
        // is then pushed to the target using that stale value once this effect
        // is actually applied - the buff icon shows up, but with no speed bonus,
        // and nothing ever revisits it afterward. By the time this effect is
        // actually applied to the target (AURA_EFFECT_HANDLE_REAL), the form
        // change has already resolved, so re-checking it here and recalculating
        // picks up the correct value.
        void RecheckFormAfterApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetEffect(EFFECT_0)->RecalculateAmount();
        }

        void Register() override
        {
            // MoP Dash uses SPELL_AURA_MOD_SPEED_ALWAYS (129), not MOD_INCREASE_SPEED (31).
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_dash_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_SPEED_ALWAYS);
            AfterEffectApply += AuraEffectApplyFn(spell_dru_dash_AuraScript::RecheckFormAfterApply, EFFECT_0, SPELL_AURA_MOD_SPEED_ALWAYS, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_dash_AuraScript();
    }
};

// 48517 - Eclipse (Solar)
// 48518 - Eclipse (Lunar)
class spell_dru_eclipse : public SpellScriptLoader
{
public:
    spell_dru_eclipse(char const* scriptName) : SpellScriptLoader(scriptName) { }

    class spell_dru_eclipse_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_eclipse_AuraScript);

        bool Load() override
        {
            return GetCaster() && GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void ApplyEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            caster->CastSpell(caster, SPELL_DRUID_NATURES_GRACE_TRIGGER, true);
            caster->CastSpell(caster, SPELL_DRUID_ECLIPSE_MANA_ENERGIZE, true);

            if (GetId() == SPELL_DRUID_LUNAR_ECLIPSE)
            {
                caster->CastSpell(caster, SPELL_DRUID_LUNAR_ECLIPSE_OVERRIDE, true);
                caster->ToPlayer()->RemoveSpellCooldown(SPELL_DRUID_STARFALL, true);
            }
            else
                caster->RemoveAurasDueToSpell(SPELL_DRUID_LUNAR_ECLIPSE_OVERRIDE);
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_dru_eclipse_AuraScript::ApplyEffect, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_eclipse_AuraScript();
    }
};

// 2912, 5176, 78674 - Starfire, Wrath, and Starsurge
class spell_dru_eclipse_energize : public SpellScriptLoader
{
public:
    spell_dru_eclipse_energize() : SpellScriptLoader("spell_dru_eclipse_energize") { }

    class spell_dru_eclipse_energize_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_eclipse_energize_SpellScript);

        int32 energizeAmount;

        bool Load() override
        {
            if (GetCaster()->GetTypeId() != TypeID::TYPEID_PLAYER)
                return false;

            if (GetCaster()->ToPlayer()->getClass() != CLASS_DRUID)
                return false;

            energizeAmount = 0;

            return true;
        }

        void HandleEnergize(SpellEffIndex effIndex)
        {
            Player* caster = GetCaster()->ToPlayer();

            // No boomy, no deal.
            if (caster->GetTalentSpecialization(caster->GetActiveSpec()) != SPEC_DRUID_BALANCE)
                return;

            // Celestial Alignment freezes the Eclipse bar.
            if (caster->HasAura(SPELL_DRUID_CELESTIAL_ALIGNMENT))
                return;

            bool hasSolarMarker = caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE_MARKER);
            bool hasLunarMarker = caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE_MARKER);
            if (hasSolarMarker == hasLunarMarker)
            {
                caster->RemoveAurasDueToSpell(SPELL_DRUID_SOLAR_ECLIPSE_MARKER);
                caster->RemoveAurasDueToSpell(SPELL_DRUID_LUNAR_ECLIPSE_MARKER);

                if (caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE))
                    caster->CastSpell(caster, SPELL_DRUID_SOLAR_ECLIPSE_MARKER, true);
                else if (caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE))
                    caster->CastSpell(caster, SPELL_DRUID_LUNAR_ECLIPSE_MARKER, true);
                else if (GetSpellInfo()->Id == SPELL_DRUID_STARFIRE)
                    caster->CastSpell(caster, SPELL_DRUID_SOLAR_ECLIPSE_MARKER, true);
                else
                    caster->CastSpell(caster, SPELL_DRUID_LUNAR_ECLIPSE_MARKER, true);
            }

            switch (GetSpellInfo()->Id)
            {
            case SPELL_DRUID_WRATH:
            {
                energizeAmount = -GetSpellInfo()->Effects[effIndex].BasePoints; // -13
                if (caster->HasAura(SPELL_DRUID_EUPHORIA) &&
                    !caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE) &&
                    !caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE))
                    energizeAmount *= 2;
                // If we are set to fill the lunar side or we've just logged in with 0 power..
                if ((!caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE_MARKER) && caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE_MARKER))
                    || caster->GetPower(POWER_ECLIPSE) == 0)
                {
                    caster->CastCustomSpell(caster, SPELL_DRUID_ECLIPSE_GENERAL_ENERGIZE, &energizeAmount, 0, 0, true);
                    // If the energize was due to 0 power, cast the eclipse marker aura
                    if (!caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE_MARKER))
                        caster->CastSpell(caster, SPELL_DRUID_LUNAR_ECLIPSE_MARKER, true);
                }
                // The energizing effect brought us out of the solar eclipse, remove the aura
                if (caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE) && caster->GetPower(POWER_ECLIPSE) <= 0)
                    caster->RemoveAurasDueToSpell(SPELL_DRUID_SOLAR_ECLIPSE);
                break;
            }
            case SPELL_DRUID_STARFIRE:
            {
                energizeAmount = GetSpellInfo()->Effects[effIndex].BasePoints; // 20
                if (caster->HasAura(SPELL_DRUID_EUPHORIA) &&
                    !caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE) &&
                    !caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE))
                    energizeAmount *= 2;
                // If we are set to fill the solar side or we've just logged in with 0 power..
                if ((!caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE_MARKER) && caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE_MARKER))
                    || caster->GetPower(POWER_ECLIPSE) == 0)
                {
                    caster->CastCustomSpell(caster, SPELL_DRUID_ECLIPSE_GENERAL_ENERGIZE, &energizeAmount, 0, 0, true);
                    // If the energize was due to 0 power, cast the eclipse marker aura
                    if (!caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE_MARKER))
                        caster->CastSpell(caster, SPELL_DRUID_SOLAR_ECLIPSE_MARKER, true);
                }
                // The energizing effect brought us out of the lunar eclipse, remove the aura
                if (caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE) && caster->GetPower(POWER_ECLIPSE) >= 0)
                {
                    caster->RemoveAura(SPELL_DRUID_LUNAR_ECLIPSE);
                    caster->RemoveAurasDueToSpell(SPELL_DRUID_LUNAR_ECLIPSE_OVERRIDE);
                }
                break;
            }
            case SPELL_DRUID_STARSURGE:
            {
                // If we are set to fill the solar side or we've just logged in with 0 power (confirmed with sniffs)
                if ((!caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE_MARKER) && caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE_MARKER))
                    || caster->GetPower(POWER_ECLIPSE) == 0)
                {
                    energizeAmount = GetSpellInfo()->Effects[effIndex].BasePoints; // 15
                    if (caster->HasAura(SPELL_DRUID_EUPHORIA) &&
                        !caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE) &&
                        !caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE))
                        energizeAmount *= 2;
                    caster->CastCustomSpell(caster, SPELL_DRUID_ECLIPSE_GENERAL_ENERGIZE, &energizeAmount, 0, 0, true);

                    // If the energize was due to 0 power, cast the eclipse marker aura
                    if (!caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE_MARKER))
                        caster->CastSpell(caster, SPELL_DRUID_SOLAR_ECLIPSE_MARKER, true);
                }
                else if (!caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE_MARKER) && caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE_MARKER))
                {
                    energizeAmount = -GetSpellInfo()->Effects[effIndex].BasePoints; // -15
                    if (caster->HasAura(SPELL_DRUID_EUPHORIA) &&
                        !caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE) &&
                        !caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE))
                        energizeAmount *= 2;
                    caster->CastCustomSpell(caster, SPELL_DRUID_ECLIPSE_GENERAL_ENERGIZE, &energizeAmount, 0, 0, true);
                }
                // The energizing effect brought us out of the lunar eclipse, remove the aura
                if (caster->HasAura(SPELL_DRUID_LUNAR_ECLIPSE) && caster->GetPower(POWER_ECLIPSE) >= 0)
                {
                    caster->RemoveAura(SPELL_DRUID_LUNAR_ECLIPSE);
                    caster->RemoveAurasDueToSpell(SPELL_DRUID_LUNAR_ECLIPSE_OVERRIDE);
                }
                // The energizing effect brought us out of the solar eclipse, remove the aura
                else if (caster->HasAura(SPELL_DRUID_SOLAR_ECLIPSE) && caster->GetPower(POWER_ECLIPSE) <= 0)
                    caster->RemoveAura(SPELL_DRUID_SOLAR_ECLIPSE);
                break;
            }
            }

            HandleEclipseReached(caster);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_dru_eclipse_energize_SpellScript::HandleEnergize, EFFECT_1, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_eclipse_energize_SpellScript;
    }
};

// 54832 - Glyph of Innervate
class spell_dru_glyph_of_innervate : public SpellScriptLoader
{
public:
    spell_dru_glyph_of_innervate() : SpellScriptLoader("spell_dru_glyph_of_innervate") { }

    class spell_dru_glyph_of_innervate_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_glyph_of_innervate_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_GLYPH_OF_INNERVATE))
                return false;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            // Not proc from self Innervate
            return GetTarget() != eventInfo.GetProcTarget();
        }

        void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            GetTarget()->CastSpell(GetTarget(), SPELL_DRUID_GLYPH_OF_INNERVATE, true, NULL, aurEff);
        }

        void Register() override
        {
            // Glyph spell 54832 uses EFFECT_0 DUMMY. Do not bind this script to Innervate (29166).
            DoCheckProc += AuraCheckProcFn(spell_dru_glyph_of_innervate_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_dru_glyph_of_innervate_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_glyph_of_innervate_AuraScript();
    }
};

// 54846 - Glyph of Starfire
class spell_dru_glyph_of_starfire : public SpellScriptLoader
{
public:
    spell_dru_glyph_of_starfire() : SpellScriptLoader("spell_dru_glyph_of_starfire") { }

    class spell_dru_glyph_of_starfire_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_glyph_of_starfire_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_INCREASED_MOONFIRE_DURATION))
                return false;
            return true;
        }

        void HandleScriptEffect(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (Unit* unitTarget = GetHitUnit())
                if (AuraEffect const* aurEff = unitTarget->GetAuraEffect(SPELL_AURA_PERIODIC_DAMAGE, SPELLFAMILY_DRUID, 0x2, 0, 0, caster->GetGUID()))
                {
                    Aura* aura = aurEff->GetBase();

                    uint32 countMin = aura->GetMaxDuration();
                    uint32 countMax = aura->GetSpellInfo()->GetMaxDuration() + 9000;
                    if (caster->HasAura(SPELL_DRUID_INCREASED_MOONFIRE_DURATION))
                        countMax += 3000;

                    if (countMin < countMax)
                    {
                        aura->SetDuration(uint32(aura->GetDuration() + 3000));
                        aura->SetMaxDuration(countMin + 3000);
                    }
                }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_dru_glyph_of_starfire_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_glyph_of_starfire_SpellScript();
    }
};

// 54845 - Glyph of Starfire
class spell_dru_glyph_of_starfire_proc : public SpellScriptLoader
{
public:
    spell_dru_glyph_of_starfire_proc() : SpellScriptLoader("spell_dru_glyph_of_starfire_proc") { }

    class spell_dru_glyph_of_starfire_proc_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_glyph_of_starfire_proc_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_GLYPH_OF_STARFIRE))
                return false;
            return true;
        }

        void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            GetTarget()->CastSpell(eventInfo.GetProcTarget(), SPELL_DRUID_GLYPH_OF_STARFIRE, true, NULL, aurEff);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_dru_glyph_of_starfire_proc_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_glyph_of_starfire_proc_AuraScript();
    }
};

// 29166 - Innervate
class spell_dru_innervate : public SpellScriptLoader
{
public:
    spell_dru_innervate() : SpellScriptLoader("spell_dru_innervate") { }

    class spell_dru_innervate_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_innervate_AuraScript);

        void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
        {
            amount = CalculatePct(int32(GetUnitOwner()->GetCreatePowers(POWER_MANA) / aurEff->GetTotalTicks()), amount);
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_innervate_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_ENERGIZE);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_innervate_AuraScript();
    }
};

// 33763 - Lifebloom
class spell_dru_lifebloom : public SpellScriptLoader
{
public:
    spell_dru_lifebloom() : SpellScriptLoader("spell_dru_lifebloom") { }

    class spell_dru_lifebloom_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_lifebloom_AuraScript);

        bool Validate(SpellInfo const* /*spell*/) override
        {
            // SPELL_DRUID_LIFEBLOOM_ENERGIZE (64372) does not exist in MoP DBC
            if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_LIFEBLOOM_FINAL_HEAL))
                return false;
            return true;
        }

        void AfterRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            // Final heal only on duration end
            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
                return;

            // final heal
            int32 stack = GetStackAmount();
            int32 healAmount = aurEff->GetAmount();
            if (Unit* caster = GetCaster())
            {
                healAmount = caster->SpellHealingBonusDone(GetTarget(), GetSpellInfo(), healAmount, HEAL, stack);
                healAmount = GetTarget()->SpellHealingBonusTaken(caster, GetSpellInfo(), healAmount, HEAL, stack);
            }

            GetTarget()->CastCustomSpell(GetTarget(), SPELL_DRUID_LIFEBLOOM_FINAL_HEAL, &healAmount, NULL, NULL, true, NULL, aurEff, GetCasterGUID());
        }

        void HandleDispel(DispelInfo* dispelInfo)
        {
            if (Unit* target = GetUnitOwner())
            {
                if (AuraEffect const* aurEff = GetEffect(EFFECT_1))
                {
                    // final heal
                    int32 healAmount = aurEff->GetAmount();
                    if (Unit* caster = GetCaster())
                    {
                        healAmount = caster->SpellHealingBonusDone(target, GetSpellInfo(), healAmount, HEAL, dispelInfo->GetRemovedCharges());
                        healAmount = target->SpellHealingBonusTaken(caster, GetSpellInfo(), healAmount, HEAL, dispelInfo->GetRemovedCharges());
                    }

                    target->CastCustomSpell(target, SPELL_DRUID_LIFEBLOOM_FINAL_HEAL, &healAmount, NULL, NULL, true, NULL, NULL, GetCasterGUID());
                }
            }
        }

        void Register() override
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_dru_lifebloom_AuraScript::AfterRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            AfterDispel += AuraDispelFn(spell_dru_lifebloom_AuraScript::HandleDispel);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_lifebloom_AuraScript();
    }
};

// 5185 - Healing Touch, 50464 - Nourish, 8936 - Regrowth
// Refresh Lifebloom duration unless Glyph of Blooming is active.
class spell_dru_lifebloom_refresh : public SpellScriptLoader
{
public:
    spell_dru_lifebloom_refresh() : SpellScriptLoader("spell_dru_lifebloom_refresh") { }

    class spell_dru_lifebloom_refresh_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_lifebloom_refresh_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return sSpellMgr->GetSpellInfo(SPELL_DRUID_LIFEBLOOM);
        }

        void HandleHit()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!caster || !target || caster->HasAura(SPELL_DRUID_GLYPH_OF_BLOOMING))
                return;

            if (Aura* lifebloom = target->GetAura(SPELL_DRUID_LIFEBLOOM, caster->GetGUID()))
                lifebloom->RefreshTimers();
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_lifebloom_refresh_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_lifebloom_refresh_SpellScript();
    }
};

// 33745 - Lacerate
// Bake AP into DoT amount so client $w1 tooltip and ticks match MoP (AP * 0.0512 / tick).
// Each hit has a 25% chance (Mangle! 93622) to reset Mangle (Bear).
class spell_dru_lacerate : public SpellScriptLoader
{
public:
    spell_dru_lacerate() : SpellScriptLoader("spell_dru_lacerate") { }

    class spell_dru_lacerate_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_lacerate_SpellScript);

        void HandleHit()
        {
            TryResetMangleCooldown(GetCaster());
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_lacerate_SpellScript::HandleHit);
        }
    };

    class spell_dru_lacerate_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_lacerate_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
        {
            if (Unit* caster = GetCaster())
            {
                canBeRecalculated = false;
                amount += int32(caster->GetTotalAttackPowerValue(WeaponAttackType::BASE_ATTACK) * 0.0512f);
            }
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_lacerate_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_lacerate_SpellScript();
    }

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_lacerate_AuraScript();
    }
};

// 77758 - Thrash (Bear)
// Each hit has a 25% chance (Mangle! 93622) to reset Mangle (Bear).
class spell_dru_thrash_bear : public SpellScriptLoader
{
public:
    spell_dru_thrash_bear() : SpellScriptLoader("spell_dru_thrash_bear") { }

    class spell_dru_thrash_bear_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_thrash_bear_SpellScript);

        void HandleHit()
        {
            TryResetMangleCooldown(GetCaster());
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_thrash_bear_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_thrash_bear_SpellScript();
    }
};

// 770 - Faerie Fire
// Effect 0 Dummy: apply 3 stacks of Weakened Armor.
// In Bear Form, has a 25% chance (Mangle! 93622) to reset Mangle (Bear).
class spell_dru_faerie_fire : public SpellScriptLoader
{
public:
    spell_dru_faerie_fire() : SpellScriptLoader("spell_dru_faerie_fire") { }

    class spell_dru_faerie_fire_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_faerie_fire_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return sSpellMgr->GetSpellInfo(SPELL_DRUID_WEAKENED_ARMOR) != NULL
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_MANGLE_PASSIVE) != NULL;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            ApplyWeakenedArmorStacks(GetCaster(), GetHitUnit());
        }

        void HandleHit()
        {
            Unit* caster = GetCaster();
            if (!caster || caster->GetShapeshiftForm() != FORM_BEAR)
                return;

            TryResetMangleCooldown(caster);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_dru_faerie_fire_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            OnHit += SpellHitFn(spell_dru_faerie_fire_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_faerie_fire_SpellScript();
    }
};

// 102355 - Faerie Swarm (replaces Faerie Fire via talent 106707 OVERRIDE_ACTIONBAR)
// Effect 0 Dummy: 3 stacks of Weakened Armor + Faerie Swarm snare (102354).
// In Bear Form, same Mangle! reset chance as Faerie Fire.
class spell_dru_faerie_swarm : public SpellScriptLoader
{
public:
    spell_dru_faerie_swarm() : SpellScriptLoader("spell_dru_faerie_swarm") { }

    class spell_dru_faerie_swarm_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_faerie_swarm_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return sSpellMgr->GetSpellInfo(SPELL_DRUID_WEAKENED_ARMOR) != NULL
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_FAERIE_SWARM_SLOW) != NULL
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_MANGLE_PASSIVE) != NULL;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            ApplyWeakenedArmorStacks(caster, target);
            if (caster && target)
                caster->CastSpell(target, SPELL_DRUID_FAERIE_SWARM_SLOW, true);
        }

        void HandleHit()
        {
            Unit* caster = GetCaster();
            if (!caster || caster->GetShapeshiftForm() != FORM_BEAR)
                return;

            TryResetMangleCooldown(caster);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_dru_faerie_swarm_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            OnHit += SpellHitFn(spell_dru_faerie_swarm_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_faerie_swarm_SpellScript();
    }
};

// 1822 - Rake
// MoP: initial and each tick use 0.3 AP. Bake tick AP into aura amount so $w2 matches damage.
class spell_dru_rake : public SpellScriptLoader
{
public:
    spell_dru_rake() : SpellScriptLoader("spell_dru_rake") { }

    class spell_dru_rake_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_rake_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
        {
            if (Unit* caster = GetCaster())
            {
                canBeRecalculated = false;
                amount += int32(caster->GetTotalAttackPowerValue(WeaponAttackType::BASE_ATTACK) * 0.3f);
            }
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_rake_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_rake_AuraScript();
    }
};

// 774 - Rejuvenation
// MoP has two PERIODIC_HEAL effects (EFFECT_0 and EFFECT_2); suppress EFFECT_2 to avoid double ticks.
class spell_dru_rejuvenation : public SpellScriptLoader
{
public:
    spell_dru_rejuvenation() : SpellScriptLoader("spell_dru_rejuvenation") { }

    class spell_dru_rejuvenation_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_rejuvenation_AuraScript);

        void HandlePeriodic(AuraEffect const* /*aurEff*/)
        {
            PreventDefaultAction();
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_rejuvenation_AuraScript::HandlePeriodic, EFFECT_2, SPELL_AURA_PERIODIC_HEAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_rejuvenation_AuraScript();
    }
};

// -48496 - Living Seed
class spell_dru_living_seed : public SpellScriptLoader
{
public:
    spell_dru_living_seed() : SpellScriptLoader("spell_dru_living_seed") { }

    class spell_dru_living_seed_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_living_seed_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_LIVING_SEED_PROC))
                return false;
            return true;
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            Unit* target = eventInfo.GetProcTarget();
            if (!target || !eventInfo.GetHealInfo())
                return;

            int32 amount = CalculatePct(eventInfo.GetHealInfo()->GetHeal(), aurEff->GetAmount());
            if (amount <= 0)
                return;

            // Seeds stack, capped at half of the druid's maximum health.
            if (AuraEffect const* existing = target->GetAuraEffect(SPELL_DRUID_LIVING_SEED_PROC, EFFECT_0))
                amount += existing->GetAmount();
            amount = std::min(amount, int32(GetTarget()->GetMaxHealth() / 2));

            GetTarget()->CastCustomSpell(SPELL_DRUID_LIVING_SEED_PROC, SPELLVALUE_BASE_POINT0, amount, target, true, NULL, aurEff);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_dru_living_seed_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_living_seed_AuraScript();
    }
};

// 48504 - Living Seed (Proc)
class spell_dru_living_seed_proc : public SpellScriptLoader
{
public:
    spell_dru_living_seed_proc() : SpellScriptLoader("spell_dru_living_seed_proc") { }

    class spell_dru_living_seed_proc_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_living_seed_proc_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_LIVING_SEED_HEAL))
                return false;
            return true;
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            GetTarget()->CastCustomSpell(SPELL_DRUID_LIVING_SEED_HEAL, SPELLVALUE_BASE_POINT0, aurEff->GetAmount(), GetTarget(), true, NULL, aurEff);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_dru_living_seed_proc_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_living_seed_proc_AuraScript();
    }
};

// 79577 - Eclipse
class spell_dru_eclipse_remove : public SpellScriptLoader
{
public:
    spell_dru_eclipse_remove() : SpellScriptLoader("spell_dru_eclipse_remove") { }

    class spell_dru_eclipse_remove_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_eclipse_remove_AuraScript);

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* druid = GetTarget();
            druid->RemoveAurasDueToSpell(SPELL_DRUID_SOLAR_ECLIPSE_MARKER);
            druid->RemoveAurasDueToSpell(SPELL_DRUID_LUNAR_ECLIPSE_MARKER);
            druid->SetPower(POWER_ECLIPSE, 0);
            druid->RemoveAurasDueToSpell(SPELL_DRUID_SOLAR_ECLIPSE);
            druid->RemoveAurasDueToSpell(SPELL_DRUID_LUNAR_ECLIPSE);
            druid->RemoveAurasDueToSpell(SPELL_DRUID_LUNAR_ECLIPSE_OVERRIDE);
        }

        void Register() override
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_eclipse_remove_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PROC_ON_POWER_AMOUNT, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_eclipse_remove_AuraScript();
    }
};

// 112071 - Celestial Alignment
class spell_dru_celestial_alignment : public SpellScriptLoader
{
public:
    spell_dru_celestial_alignment() : SpellScriptLoader("spell_dru_celestial_alignment") { }

    class spell_dru_celestial_alignment_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_celestial_alignment_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Player* druid = GetTarget()->ToPlayer();
            if (!druid)
                return;

            druid->SetPower(POWER_ECLIPSE, 0);
            druid->RemoveAurasDueToSpell(SPELL_DRUID_SOLAR_ECLIPSE);
            druid->RemoveAurasDueToSpell(SPELL_DRUID_LUNAR_ECLIPSE);
            druid->RemoveAurasDueToSpell(SPELL_DRUID_SOLAR_ECLIPSE_MARKER);
            druid->RemoveAurasDueToSpell(SPELL_DRUID_LUNAR_ECLIPSE_MARKER);

            druid->CastSpell(druid, SPELL_DRUID_ECLIPSE_MANA_ENERGIZE, true);
            druid->CastSpell(druid, SPELL_DRUID_NATURES_GRACE_TRIGGER, true);
            druid->CastSpell(druid, SPELL_DRUID_LUNAR_ECLIPSE_OVERRIDE, true);
            druid->RemoveSpellCooldown(SPELL_DRUID_STARFALL, true);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->RemoveAurasDueToSpell(SPELL_DRUID_NATURES_GRACE_TRIGGER);
            GetTarget()->RemoveAurasDueToSpell(SPELL_DRUID_LUNAR_ECLIPSE_OVERRIDE);
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_dru_celestial_alignment_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_dru_celestial_alignment_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_celestial_alignment_AuraScript();
    }
};

// 8921 - Moonfire
class spell_dru_moonfire : public SpellScriptLoader
{
public:
    spell_dru_moonfire() : SpellScriptLoader("spell_dru_moonfire") { }

    class spell_dru_moonfire_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_moonfire_SpellScript);

        void HandleHit()
        {
            // Celestial Alignment: Moonfire also applies Sunfire.
            if (GetCaster()->HasAura(SPELL_DRUID_CELESTIAL_ALIGNMENT))
                GetCaster()->CastSpell(GetHitUnit(), SPELL_DRUID_SUNFIRE, true);
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_moonfire_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_moonfire_SpellScript();
    }
};

// 99 - Disorienting Roar, 339 - Entangling Roots, 770 - Faerie Fire, 5211 - Mighty Bash, 33786 - Cyclone,
// 102355 - Faerie Swarm, 102359 - Mass Entanglement, 102793 - Ursol's Vortex, 132469 - Typhoon
class spell_dru_glyph_of_omens : public SpellScriptLoader
{
public:
    spell_dru_glyph_of_omens() : SpellScriptLoader("spell_dru_glyph_of_omens") { }

    class spell_dru_glyph_of_omens_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_glyph_of_omens_SpellScript);

        bool Load() override
        {
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER && GetCaster()->HasAura(SPELL_DRUID_GLYPH_OF_OMENS);
        }

        void HandleCast()
        {
            int32 amount = 10;
            if (AuraEffect const* glyph = GetCaster()->GetAuraEffect(SPELL_DRUID_GLYPH_OF_OMENS, EFFECT_0))
                amount = glyph->GetAmount();
            ApplyEclipseBonus(GetCaster(), amount);
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_dru_glyph_of_omens_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_glyph_of_omens_SpellScript();
    }
};

// 46832 - Sudden Eclipse
class spell_dru_sudden_eclipse : public SpellScriptLoader
{
public:
    spell_dru_sudden_eclipse() : SpellScriptLoader("spell_dru_sudden_eclipse") { }

    class spell_dru_sudden_eclipse_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_sudden_eclipse_AuraScript);

        // Script-handled procs bypass the spell_proc_event cooldown.
        static uint32 const InternalCooldown = 6 * IN_MILLISECONDS;
        uint32 _lastProcTime = 0;

        bool CheckProc(ProcEventInfo& /*eventInfo*/)
        {
            return _lastProcTime == 0 || getMSTimeDiff(_lastProcTime, getMSTime()) >= InternalCooldown;
        }

        void HandleProc(ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            _lastProcTime = getMSTime();

            int32 amount = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetTarget());
            ApplyEclipseBonus(GetTarget(), amount);
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_dru_sudden_eclipse_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_dru_sudden_eclipse_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_sudden_eclipse_AuraScript();
    }
};

// 16914 - Hurricane, 106996 - Astral Storm
class spell_dru_glyph_of_hurricane : public SpellScriptLoader
{
public:
    spell_dru_glyph_of_hurricane() : SpellScriptLoader("spell_dru_glyph_of_hurricane") { }

    class spell_dru_glyph_of_hurricane_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_glyph_of_hurricane_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* druid = GetCaster())
                if (AuraEffect const* glyph = druid->GetAuraEffect(SPELL_DRUID_GLYPH_OF_HURRICANE, EFFECT_0))
                    amount = glyph->GetAmount();
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_glyph_of_hurricane_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_DECREASE_SPEED);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_glyph_of_hurricane_AuraScript();
    }
};

// 144767 - Item - Druid T16 Balance 2P Bonus
class spell_dru_t16_balance_2p_bonus : public SpellScriptLoader
{
public:
    spell_dru_t16_balance_2p_bonus() : SpellScriptLoader("spell_dru_t16_balance_2p_bonus") { }

    class spell_dru_t16_balance_2p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_t16_balance_2p_bonus_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (eventInfo.GetTypeMask() & PROC_FLAG_DONE_PERIODIC)
                return false;
            return eventInfo.GetSpellInfo() && eventInfo.GetActionTarget();
        }

        void HandleProc(ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            Unit* druid = GetTarget();
            Unit* target = eventInfo.GetActionTarget();
            uint32 schoolMask = eventInfo.GetSpellInfo()->SchoolMask;
            bool alignment = druid->HasAura(SPELL_DRUID_CELESTIAL_ALIGNMENT);

            if ((alignment || druid->HasAura(SPELL_DRUID_LUNAR_ECLIPSE)) && (schoolMask & SPELL_SCHOOL_MASK_ARCANE))
                druid->CastSpell(target, SPELL_DRUID_LUNAR_BOLT, true);
            else if ((alignment || druid->HasAura(SPELL_DRUID_SOLAR_ECLIPSE)) && (schoolMask & SPELL_SCHOOL_MASK_NATURE))
                druid->CastSpell(target, SPELL_DRUID_SOLAR_BOLT, true);
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_dru_t16_balance_2p_bonus_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_dru_t16_balance_2p_bonus_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_t16_balance_2p_bonus_AuraScript();
    }
};

// 135597 - Tooth and Claw
class spell_dru_tooth_and_claw_absorb : public SpellScriptLoader
{
public:
    spell_dru_tooth_and_claw_absorb() : SpellScriptLoader("spell_dru_tooth_and_claw_absorb") { }

    class spell_dru_tooth_and_claw_absorb_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_tooth_and_claw_absorb_AuraScript);

        // Only absorbs damage from the attacker marked by Tooth and Claw.
        void Absorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& /*absorbAmount*/)
        {
            Unit* attacker = dmgInfo.GetAttacker();
            if (!attacker || !attacker->HasAura(SPELL_DRUID_TOOTH_AND_CLAW_VISUAL, GetCasterGUID()))
                PreventDefaultAction();
        }

        // One hit consumes the whole shield.
        void AfterAbsorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& /*absorbAmount*/)
        {
            if (Unit* attacker = dmgInfo.GetAttacker())
                attacker->RemoveAurasDueToSpell(SPELL_DRUID_TOOTH_AND_CLAW_VISUAL, GetCasterGUID());
            aurEff->SetAmount(0);
        }

        void Register() override
        {
            OnEffectAbsorb += AuraEffectAbsorbFn(spell_dru_tooth_and_claw_absorb_AuraScript::Absorb, EFFECT_0);
            AfterEffectAbsorb += AuraEffectAbsorbFn(spell_dru_tooth_and_claw_absorb_AuraScript::AfterAbsorb, EFFECT_0);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_tooth_and_claw_absorb_AuraScript();
    }
};

// 6807 - Maul
class spell_dru_tooth_and_claw_trigger : public SpellScriptLoader
{
public:
    spell_dru_tooth_and_claw_trigger() : SpellScriptLoader("spell_dru_tooth_and_claw_trigger") { }

    class spell_dru_tooth_and_claw_trigger_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_tooth_and_claw_trigger_SpellScript);

        void HandleCast()
        {
            Unit* caster = GetCaster();
            Unit* target = GetExplTargetUnit();
            if (!target || !caster->HasAura(SPELL_DRUID_TOOTH_AND_CLAW_AURA))
                return;

            int32 fromAttackPower = int32((caster->GetTotalAttackPowerValue(WeaponAttackType::BASE_ATTACK) - 2 * caster->GetStat(STAT_AGILITY)) * 2.2f * 0.4f);
            int32 fromStamina = int32(caster->GetStat(STAT_STAMINA));
            int32 absorb = std::max(fromAttackPower, fromStamina);

            caster->RemoveAurasDueToSpell(SPELL_DRUID_TOOTH_AND_CLAW_AURA);
            caster->CastCustomSpell(SPELL_DRUID_TOOTH_AND_CLAW_ABSORB, SPELLVALUE_BASE_POINT0, absorb, caster, true);
            caster->CastCustomSpell(SPELL_DRUID_TOOTH_AND_CLAW_VISUAL, SPELLVALUE_BASE_POINT0, absorb, target, true);
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_dru_tooth_and_claw_trigger_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_tooth_and_claw_trigger_SpellScript();
    }
};

// 50334 - Berserk (Bear)
class spell_dru_berserk_bear : public SpellScriptLoader
{
public:
    spell_dru_berserk_bear() : SpellScriptLoader("spell_dru_berserk_bear") { }

    class spell_dru_berserk_bear_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_berserk_bear_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* druid = GetTarget()->ToPlayer())
                druid->RemoveSpellCooldown(SPELL_DRUID_MANGLE_BEAR, true);
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_dru_berserk_bear_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_ADD_FLAT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_berserk_bear_AuraScript();
    }
};

// 5487 - Bear Form
class spell_dru_bear_form : public SpellScriptLoader
{
public:
    spell_dru_bear_form() : SpellScriptLoader("spell_dru_bear_form") { }

    class spell_dru_bear_form_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_bear_form_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (AuraEffect const* thickHide = GetUnitOwner()->GetAuraEffect(SPELL_DRUID_THICK_HIDE, EFFECT_1))
                amount = thickHide->GetAmount();
            else
                amount = 120;
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_bear_form_AuraScript::CalculateAmount, EFFECT_2, SPELL_AURA_MOD_BASE_RESISTANCE_PCT);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_bear_form_AuraScript();
    }
};

// 1178 - Bear Form (Passive)
class spell_dru_bear_form_passive : public SpellScriptLoader
{
public:
    spell_dru_bear_form_passive() : SpellScriptLoader("spell_dru_bear_form_passive") { }

    class spell_dru_bear_form_passive_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_bear_form_passive_AuraScript);

        void CalculateCooldownMod(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
        {
            canBeRecalculated = true;
            if (GetUnitOwner()->HasAura(SPELL_DRUID_GLYPH_OF_FAE_SILENCE))
                amount += 9 * IN_MILLISECONDS;
        }

        void CalculateRageFromDamage(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
        {
            canBeRecalculated = true;
            if (AuraEffect const* setBonus = GetUnitOwner()->GetAuraEffect(SPELL_DRUID_T15_GUARDIAN_4P_BONUS, EFFECT_0))
                AddPct(amount, setBonus->GetAmount());
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_bear_form_passive_AuraScript::CalculateCooldownMod, EFFECT_2, SPELL_AURA_ADD_FLAT_MODIFIER);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_bear_form_passive_AuraScript::CalculateRageFromDamage, EFFECT_4, SPELL_AURA_MOD_RAGE_FROM_DAMAGE_DEALT);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_bear_form_passive_AuraScript();
    }
};

// 114237 - Glyph of Fae Silence
class spell_dru_glyph_of_fae_silence_cooldown : public SpellScriptLoader
{
public:
    spell_dru_glyph_of_fae_silence_cooldown() : SpellScriptLoader("spell_dru_glyph_of_fae_silence_cooldown") { }

    class spell_dru_glyph_of_fae_silence_cooldown_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_glyph_of_fae_silence_cooldown_AuraScript);

        void HandleEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (AuraEffect* bearForm = GetTarget()->GetAuraEffect(SPELL_DRUID_BEAR_FORM_PASSIVE, EFFECT_2))
                bearForm->RecalculateAmount();
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_dru_glyph_of_fae_silence_cooldown_AuraScript::HandleEffect, EFFECT_0, SPELL_AURA_NONE, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_dru_glyph_of_fae_silence_cooldown_AuraScript::HandleEffect, EFFECT_0, SPELL_AURA_NONE, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_glyph_of_fae_silence_cooldown_AuraScript();
    }
};

// 138222 - Item - Druid T15 Guardian 4P Bonus
class spell_dru_t15_guardian_4p_bonus : public SpellScriptLoader
{
public:
    spell_dru_t15_guardian_4p_bonus() : SpellScriptLoader("spell_dru_t15_guardian_4p_bonus") { }

    class spell_dru_t15_guardian_4p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_t15_guardian_4p_bonus_AuraScript);

        void HandleEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (AuraEffect* bearForm = GetTarget()->GetAuraEffect(SPELL_DRUID_BEAR_FORM_PASSIVE, EFFECT_4))
                bearForm->RecalculateAmount();
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_dru_t15_guardian_4p_bonus_AuraScript::HandleEffect, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_dru_t15_guardian_4p_bonus_AuraScript::HandleEffect, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_t15_guardian_4p_bonus_AuraScript();
    }
};

// 770 - Faerie Fire, 102355 - Faerie Swarm
class spell_dru_glyph_of_fae_silence : public SpellScriptLoader
{
public:
    spell_dru_glyph_of_fae_silence() : SpellScriptLoader("spell_dru_glyph_of_fae_silence") { }

    class spell_dru_glyph_of_fae_silence_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_glyph_of_fae_silence_SpellScript);

        void HandleHit()
        {
            Unit* caster = GetCaster();
            if (caster->GetShapeshiftForm() == FORM_BEAR && caster->HasAura(SPELL_DRUID_GLYPH_OF_FAE_SILENCE))
                caster->CastSpell(GetHitUnit(), SPELL_DRUID_FAE_SILENCE, true);
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_glyph_of_fae_silence_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_glyph_of_fae_silence_SpellScript();
    }
};

// 6795 - Growl
class spell_dru_growl : public SpellScriptLoader
{
public:
    spell_dru_growl() : SpellScriptLoader("spell_dru_growl") { }

    class spell_dru_growl_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_growl_SpellScript);

        void HandleCast()
        {
            if (GetCaster()->GetShapeshiftForm() != FORM_BEAR)
                GetCaster()->CastSpell(GetCaster(), SPELL_DRUID_BEAR_FORM, true);
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_dru_growl_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_growl_SpellScript();
    }
};

// 81262 - Efflorescence, 142423 - Efflorescence (Force of Nature)
class spell_dru_efflorescence : public SpellScriptLoader
{
public:
    spell_dru_efflorescence() : SpellScriptLoader("spell_dru_efflorescence") { }

    class spell_dru_efflorescence_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_efflorescence_AuraScript);

        void HandleTick(AuraEffect const* /*aurEff*/)
        {
            Unit* owner = GetTarget();
            Player* druid = owner->GetCharmerOrOwnerPlayerOrPlayerItself();
            if (!druid)
                return;

            if (DynamicObject* dynObj = owner->GetDynObject(GetId()))
            {
                uint32 healSpell = GetId() == SPELL_DRUID_EFFLORESCENCE ? SPELL_DRUID_EFFLORESCENCE_HEAL : SPELL_DRUID_EFFLORESCENCE_HEAL_TREANT;
                druid->CastSpell(dynObj->GetPositionX(), dynObj->GetPositionY(), dynObj->GetPositionZ(), healSpell, true);
            }
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_efflorescence_AuraScript::HandleTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_efflorescence_AuraScript();
    }
};

// 81269, 142424 - Efflorescence (heal)
class spell_dru_efflorescence_heal : public SpellScriptLoader
{
public:
    spell_dru_efflorescence_heal() : SpellScriptLoader("spell_dru_efflorescence_heal") { }

    class spell_dru_efflorescence_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_efflorescence_heal_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            SelectMostInjuredTargets(targets, GetCaster()->HasAura(SPELL_DRUID_T15_RESTORATION_2P_BONUS) ? 4 : 3);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_efflorescence_heal_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_efflorescence_heal_SpellScript();
    }
};

// 48438 - Wild Growth
class spell_dru_wild_growth : public SpellScriptLoader
{
public:
    spell_dru_wild_growth() : SpellScriptLoader("spell_dru_wild_growth") { }

    class spell_dru_wild_growth_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_wild_growth_SpellScript);

        std::list<WorldObject*> _targets;

        void SelectTargets(std::list<WorldObject*>& targets)
        {
            Unit* caster = GetCaster();
            targets.remove_if([caster](WorldObject* target)
            {
                Unit* unit = target->ToUnit();
                return !unit || !caster->IsInRaidWith(unit);
            });

            uint32 maxTargets = uint32(GetSpellInfo()->Effects[EFFECT_2].CalcValue(caster));
            if (caster->GetShapeshiftForm() == FORM_TREE)
                maxTargets += 2;

            SelectMostInjuredTargets(targets, maxTargets);
            _targets = targets;
        }

        void CopyTargets(std::list<WorldObject*>& targets)
        {
            targets = _targets;
        }

        void HandleHit()
        {
            if (GetCaster()->HasAura(SPELL_DRUID_T16_RESTORATION_4P_BONUS))
                GetCaster()->CastSpell(GetHitUnit(), SPELL_DRUID_SPARK_OF_LIFE, true);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_wild_growth_SpellScript::SelectTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_wild_growth_SpellScript::CopyTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ALLY);
            OnHit += SpellHitFn(spell_dru_wild_growth_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_wild_growth_SpellScript();
    }
};

// 77495 - Mastery: Harmony
class spell_dru_harmony : public SpellScriptLoader
{
public:
    spell_dru_harmony() : SpellScriptLoader("spell_dru_harmony") { }

    class spell_dru_harmony_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_harmony_AuraScript);

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();

            Unit* druid = GetTarget();
            int32 bp = aurEff->GetAmount();
            druid->CastCustomSpell(druid, SPELL_DRUID_HARMONY, &bp, &bp, NULL, true);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_dru_harmony_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_harmony_AuraScript();
    }
};

// 8936 - Regrowth
// Glyph of Regrowth removes the HoT; its increased critical chance is not handled here.
class spell_dru_glyph_of_regrowth : public SpellScriptLoader
{
public:
    spell_dru_glyph_of_regrowth() : SpellScriptLoader("spell_dru_glyph_of_regrowth") { }

    class spell_dru_glyph_of_regrowth_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_glyph_of_regrowth_SpellScript);

        void HandleHoT(SpellEffIndex /*effIndex*/)
        {
            if (GetCaster()->HasAura(SPELL_DRUID_GLYPH_OF_REGROWTH))
                PreventHitAura();
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_dru_glyph_of_regrowth_SpellScript::HandleHoT, EFFECT_1, SPELL_EFFECT_APPLY_AURA);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_glyph_of_regrowth_SpellScript();
    }
};

// 54825 - Glyph of Healing Touch
class spell_dru_glyph_of_healing_touch : public SpellScriptLoader
{
public:
    spell_dru_glyph_of_healing_touch() : SpellScriptLoader("spell_dru_glyph_of_healing_touch") { }

    class spell_dru_glyph_of_healing_touch_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_glyph_of_healing_touch_AuraScript);

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            if (Player* druid = GetTarget()->ToPlayer())
                druid->ModifySpellCooldown(SPELL_DRUID_NATURES_SWIFTNESS, -aurEff->GetAmount() * IN_MILLISECONDS);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_dru_glyph_of_healing_touch_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_glyph_of_healing_touch_AuraScript();
    }
};

// 144871 - Sage Mender (Item - Druid T16 Restoration 2P Bonus)
class spell_dru_sage_mender : public SpellScriptLoader
{
public:
    spell_dru_sage_mender() : SpellScriptLoader("spell_dru_sage_mender") { }

    class spell_dru_sage_mender_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_sage_mender_AuraScript);

        void HandleProc(ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            Remove();
        }

        void Register() override
        {
            OnProc += AuraProcFn(spell_dru_sage_mender_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_sage_mender_AuraScript();
    }
};

// 774 - Rejuvenation
class spell_dru_t15_restoration_4p_bonus : public SpellScriptLoader
{
public:
    spell_dru_t15_restoration_4p_bonus() : SpellScriptLoader("spell_dru_t15_restoration_4p_bonus") { }

    class spell_dru_t15_restoration_4p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_t15_restoration_4p_bonus_AuraScript);

        void HandleTick(AuraEffect const* aurEff)
        {
            Unit* caster = GetCaster();
            if (!caster || aurEff->GetTickNumber() < 2)
                return;

            if (AuraEffect const* setBonus = caster->GetAuraEffect(SPELL_DRUID_T15_RESTORATION_4P_BONUS, EFFECT_0))
            {
                AuraEffect* rejuvenation = const_cast<AuraEffect*>(aurEff);
                int32 amount = rejuvenation->GetAmount();
                AddPct(amount, setBonus->GetAmount());
                rejuvenation->ChangeAmount(amount);
            }
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_t15_restoration_4p_bonus_AuraScript::HandleTick, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_t15_restoration_4p_bonus_AuraScript();
    }
};

// 108291, 108292, 108293, 108294 - Heart of the Wild
class spell_dru_heart_of_the_wild : public SpellScriptLoader
{
public:
    spell_dru_heart_of_the_wild() : SpellScriptLoader("spell_dru_heart_of_the_wild") { }

    class spell_dru_heart_of_the_wild_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_heart_of_the_wild_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* druid = GetTarget();
            bool cat = druid->HasAura(SPELL_DRUID_CAT_FORM);
            bool bear = druid->HasAura(SPELL_DRUID_BEAR_FORM);

            switch (GetId())
            {
                case SPELL_DRUID_HOTW_BALANCE:
                case SPELL_DRUID_HOTW_RESTORATION:
                    if (cat)
                        druid->CastSpell(druid, SPELL_DRUID_HOTW_CAT, true);
                    else if (bear)
                        druid->CastSpell(druid, SPELL_DRUID_HOTW_BEAR, true);
                    break;
                case SPELL_DRUID_HOTW_FERAL:
                    if (bear)
                        druid->CastSpell(druid, SPELL_DRUID_HOTW_BEAR, true);
                    break;
                case SPELL_DRUID_HOTW_GUARDIAN:
                    if (cat)
                        druid->CastSpell(druid, SPELL_DRUID_HOTW_CAT, true);
                    break;
                default:
                    break;
            }
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->RemoveAurasDueToSpell(SPELL_DRUID_HOTW_CAT);
            GetTarget()->RemoveAurasDueToSpell(SPELL_DRUID_HOTW_BEAR);
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_dru_heart_of_the_wild_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_heart_of_the_wild_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_heart_of_the_wild_AuraScript();
    }
};

// 768 - Cat Form, 5487 - Bear Form
class spell_dru_heart_of_the_wild_bonus : public SpellScriptLoader
{
public:
    spell_dru_heart_of_the_wild_bonus() : SpellScriptLoader("spell_dru_heart_of_the_wild_bonus") { }

    class spell_dru_heart_of_the_wild_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_heart_of_the_wild_bonus_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Player* druid = GetTarget()->ToPlayer();
            if (!druid)
                return;

            uint32 spec = druid->GetTalentSpecialization(druid->GetActiveSpec());
            switch (GetId())
            {
                case SPELL_DRUID_CAT_FORM:
                    if (spec != SPEC_DRUID_FERAL && (druid->HasAura(SPELL_DRUID_HOTW_BALANCE) || druid->HasAura(SPELL_DRUID_HOTW_GUARDIAN) || druid->HasAura(SPELL_DRUID_HOTW_RESTORATION)))
                        druid->CastSpell(druid, SPELL_DRUID_HOTW_CAT, true);
                    break;
                case SPELL_DRUID_BEAR_FORM:
                    if (spec != SPEC_DRUID_GUARDIAN && (druid->HasAura(SPELL_DRUID_HOTW_BALANCE) || druid->HasAura(SPELL_DRUID_HOTW_FERAL) || druid->HasAura(SPELL_DRUID_HOTW_RESTORATION)))
                        druid->CastSpell(druid, SPELL_DRUID_HOTW_BEAR, true);
                    break;
                default:
                    break;
            }
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->RemoveAurasDueToSpell(GetId() == SPELL_DRUID_CAT_FORM ? SPELL_DRUID_HOTW_CAT : SPELL_DRUID_HOTW_BEAR);
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_dru_heart_of_the_wild_bonus_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_heart_of_the_wild_bonus_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_heart_of_the_wild_bonus_AuraScript();
    }
};

// 108300 - Killer Instinct
class spell_dru_killer_instinct : public SpellScriptLoader
{
public:
    spell_dru_killer_instinct() : SpellScriptLoader("spell_dru_killer_instinct") { }

    class spell_dru_killer_instinct_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_killer_instinct_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            amount = int32(GetUnitOwner()->GetStat(STAT_INTELLECT));
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_killer_instinct_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_STAT);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_killer_instinct_AuraScript();
    }
};

// 768 - Cat Form, 5487 - Bear Form
class spell_dru_killer_instinct_form : public SpellScriptLoader
{
public:
    spell_dru_killer_instinct_form() : SpellScriptLoader("spell_dru_killer_instinct_form") { }

    class spell_dru_killer_instinct_form_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_killer_instinct_form_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetTarget()->HasAura(SPELL_DRUID_KILLER_INSTINCT_PASSIVE))
                GetTarget()->CastSpell(GetTarget(), SPELL_DRUID_KILLER_INSTINCT, true);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->RemoveAurasDueToSpell(SPELL_DRUID_KILLER_INSTINCT);
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_dru_killer_instinct_form_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_dru_killer_instinct_form_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_killer_instinct_form_AuraScript();
    }
};

// 124974 - Nature's Vigil
class spell_dru_natures_vigil : public SpellScriptLoader
{
public:
    spell_dru_natures_vigil() : SpellScriptLoader("spell_dru_natures_vigil") { }

    class spell_dru_natures_vigil_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_natures_vigil_AuraScript);

        // Only single-target spells are copied.
        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            Unit* target = eventInfo.GetActionTarget();
            if (!spellInfo || !target)
                return;

            for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
            {
                SpellEffectInfo const& effect = spellInfo->Effects[i];
                if (effect.Effect == SPELL_EFFECT_PERSISTENT_AREA_AURA || effect.IsAreaAuraEffect() || effect.IsTargetingArea() || effect.ChainTarget > 1)
                    return;
            }

            bool positive = spellInfo->IsPositive();
            int32 bp = 0;
            if (positive)
            {
                if (HealInfo* healInfo = eventInfo.GetHealInfo())
                    bp = CalculatePct(int32(healInfo->GetHeal()), aurEff->GetAmount());
            }
            else if (DamageInfo* damageInfo = eventInfo.GetDamageInfo())
                bp = CalculatePct(int32(damageInfo->GetDamage()), aurEff->GetAmount());

            if (bp <= 0)
                return;

            Unit* druid = GetTarget();
            CustomSpellValues values;
            values.AddSpellMod(SPELLVALUE_BASE_POINT0, bp);
            druid->CastCustomSpell(SPELL_DRUID_NATURES_VIGIL_HEAL, values, target, TRIGGERED_FULL_MASK, NULL, aurEff);

            if (positive)
                druid->CastCustomSpell(SPELL_DRUID_NATURES_VIGIL_DAMAGE, values, target, TRIGGERED_FULL_MASK, NULL, aurEff);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_dru_natures_vigil_AuraScript::HandleProc, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_natures_vigil_AuraScript();
    }
};

// 124991 - Nature's Vigil (damage)
class spell_dru_natures_vigil_damage : public SpellScriptLoader
{
public:
    spell_dru_natures_vigil_damage() : SpellScriptLoader("spell_dru_natures_vigil_damage") { }

    class spell_dru_natures_vigil_damage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_natures_vigil_damage_SpellScript);

        void SelectTarget(std::list<WorldObject*>& targets)
        {
            Unit* caster = GetCaster();
            WorldLocation const* dest = GetExplTargetDest();
            targets.remove_if([dest](WorldObject* target)
            {
                Unit* unit = target->ToUnit();
                if (!unit || (dest && !unit->IsInDist(dest, 40.0f)))
                    return true;
                return unit->HasAuraType(SPELL_AURA_MOD_STEALTH) || unit->HasAuraType(SPELL_AURA_MOD_INVISIBILITY);
            });

            if (targets.empty())
                return;

            targets.sort([caster](WorldObject* a, WorldObject* b) { return caster->GetExactDist(a) < caster->GetExactDist(b); });
            targets.resize(1);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_natures_vigil_damage_SpellScript::SelectTarget, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_natures_vigil_damage_SpellScript();
    }
};

// 124988 - Nature's Vigil (heal)
class spell_dru_natures_vigil_heal : public SpellScriptLoader
{
public:
    spell_dru_natures_vigil_heal() : SpellScriptLoader("spell_dru_natures_vigil_heal") { }

    class spell_dru_natures_vigil_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_natures_vigil_heal_SpellScript);

        // Heals the nearest injured ally, or the nearest ally if nobody is injured.
        void SelectTarget(std::list<WorldObject*>& targets)
        {
            Unit* caster = GetCaster();
            WorldLocation const* dest = GetExplTargetDest();
            targets.remove_if([dest](WorldObject* target)
            {
                Unit* unit = target->ToUnit();
                return !unit || !unit->IsAlive() || (dest && !unit->IsInDist(dest, 40.0f));
            });

            if (targets.empty())
                return;

            targets.sort([caster](WorldObject* a, WorldObject* b) { return caster->GetExactDist(a) < caster->GetExactDist(b); });

            WorldObject* selected = targets.front();
            for (std::list<WorldObject*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
            {
                if (!(*itr)->ToUnit()->IsFullHealth())
                {
                    selected = *itr;
                    break;
                }
            }

            targets.clear();
            targets.push_back(selected);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_natures_vigil_heal_SpellScript::SelectTarget, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_natures_vigil_heal_SpellScript();
    }
};

enum SymbiosisRole
{
    SYMBIOSIS_ROLE_DPS,
    SYMBIOSIS_ROLE_TANK,
    SYMBIOSIS_ROLE_HEALER
};

static SymbiosisRole GetSymbiosisRole(uint32 spec)
{
    switch (spec)
    {
        case SPEC_PALADIN_PROTECTION:
        case SPEC_WARRIOR_PROTECTION:
        case SPEC_DRUID_GUARDIAN:
        case SPEC_DEATH_KNIGHT_BLOOD:
        case SPEC_MONK_BREWMASTER:
            return SYMBIOSIS_ROLE_TANK;
        case SPEC_PALADIN_HOLY:
        case SPEC_DRUID_RESTORATION:
        case SPEC_PRIEST_DISCIPLINE:
        case SPEC_PRIEST_HOLY:
        case SPEC_SHAMAN_RESTORATION:
        case SPEC_MONK_MISTWEAVER:
            return SYMBIOSIS_ROLE_HEALER;
        default:
            return SYMBIOSIS_ROLE_DPS;
    }
}

static void UnsummonSymbiosisGuardians(Unit* owner, uint32 spellId)
{
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo)
        return;

    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if (spellInfo->Effects[i].Effect != SPELL_EFFECT_SUMMON)
            continue;

        std::list<TempSummon*> summons;
        owner->GetSummons(summons, spellInfo->Effects[i].MiscValue);
        for (std::list<TempSummon*>::const_iterator itr = summons.begin(); itr != summons.end(); ++itr)
            (*itr)->UnSummon();
    }
}

// 110309 - Symbiosis (caster aura)
class spell_dru_symbiosis_caster_aura : public SpellScriptLoader
{
public:
    spell_dru_symbiosis_caster_aura() : SpellScriptLoader("spell_dru_symbiosis_caster_aura") { }

    class spell_dru_symbiosis_caster_aura_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_symbiosis_caster_aura_AuraScript);

        uint32 _targetSpellId = 0;
        uint64 _targetGuid = 0;

    public:
        void Bind(uint32 overrideSpellId, uint32 targetSpellId, uint64 targetGuid)
        {
            if (AuraEffect* effect = GetEffect(EFFECT_0))
                effect->ChangeAmount(int32(overrideSpellId));
            _targetSpellId = targetSpellId;
            _targetGuid = targetGuid;
        }

    private:
        void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            Unit* druid = GetTarget();
            if (_targetGuid)
                if (Unit* target = ObjectAccessor::GetUnit(*druid, _targetGuid))
                    target->RemoveAurasDueToSpell(_targetSpellId, druid->GetGUID());

            if (aurEff->GetAmount() > 0)
            {
                druid->RemoveAurasDueToSpell(uint32(aurEff->GetAmount()));
                UnsummonSymbiosisGuardians(druid, uint32(aurEff->GetAmount()));
            }
        }

        void HandleUpdate(uint32 /*diff*/)
        {
            if (!_targetGuid)
                return;

            Player* druid = GetTarget()->ToPlayer();
            Player* target = ObjectAccessor::GetPlayer(*GetTarget(), _targetGuid);
            if (!druid || !target || !target->IsInSameRaidWith(druid))
                Remove();
        }

        void Register() override
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_symbiosis_caster_aura_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS, AURA_EFFECT_HANDLE_REAL);
            OnAuraUpdate += AuraUpdateFn(spell_dru_symbiosis_caster_aura_AuraScript::HandleUpdate);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_symbiosis_caster_aura_AuraScript();
    }
};

// 110478, 110479, 110482, 110483, 110484, 110485, 110486, 110488, 110490, 110491 - Symbiosis (target aura)
class spell_dru_symbiosis_target_check : public SpellScriptLoader
{
public:
    spell_dru_symbiosis_target_check() : SpellScriptLoader("spell_dru_symbiosis_target_check") { }

    class spell_dru_symbiosis_target_check_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_symbiosis_target_check_AuraScript);

        void HandleUpdate(uint32 /*diff*/)
        {
            Unit* caster = GetCaster();
            Player* druid = caster ? caster->ToPlayer() : NULL;
            Player* target = GetTarget()->ToPlayer();
            if (!druid || !target || !target->IsInSameRaidWith(druid) || target->GetMapId() != druid->GetMapId())
                Remove();
        }

        void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
                caster->RemoveAurasDueToSpell(SPELL_DRUID_SYMBIOSIS_FOR_CASTER);

            if (aurEff->GetAmount() > 0)
            {
                GetTarget()->RemoveAurasDueToSpell(uint32(aurEff->GetAmount()));
                UnsummonSymbiosisGuardians(GetTarget(), uint32(aurEff->GetAmount()));
            }
        }

        void Register() override
        {
            OnAuraUpdate += AuraUpdateFn(spell_dru_symbiosis_target_check_AuraScript::HandleUpdate);
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_symbiosis_target_check_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_symbiosis_target_check_AuraScript();
    }
};

// 110309 - Symbiosis
class spell_dru_symbiosis : public SpellScriptLoader
{
public:
    spell_dru_symbiosis() : SpellScriptLoader("spell_dru_symbiosis") { }

    class spell_dru_symbiosis_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_symbiosis_SpellScript);

        uint64 _targetGuid = 0;

        bool Load() override
        {
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        SpellCastResult CheckCast()
        {
            Player* target = GetExplTargetUnit() ? GetExplTargetUnit()->ToPlayer() : NULL;
            if (!target || target == GetCaster() || target->getClass() == CLASS_DRUID || target->GetTalentSpecialization(target->GetActiveSpec()) == SPEC_NONE)
                return SpellCastResult::SPELL_FAILED_BAD_TARGETS;
            if (!GetCaster()->IsInRaidWith(target))
                return SpellCastResult::SPELL_FAILED_TARGET_NOT_IN_PARTY;
            return SpellCastResult::SPELL_CAST_OK;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Unit* target = GetHitUnit())
                if (target != GetCaster())
                    _targetGuid = target->GetGUID();
        }

        // The caster aura is applied by the same cast, so binding waits until all targets are hit.
        void HandleAfterCast()
        {
            Player* druid = GetCaster()->ToPlayer();
            Player* target = _targetGuid ? ObjectAccessor::GetPlayer(*druid, _targetGuid) : NULL;
            Aura* symbiosis = druid->GetAura(SPELL_DRUID_SYMBIOSIS_FOR_CASTER);
            if (!target || !symbiosis)
                return;

            uint32 druidSpec = druid->GetTalentSpecialization(druid->GetActiveSpec());
            uint32 targetSpec = target->GetTalentSpecialization(target->GetActiveSpec());
            if (druidSpec == SPEC_NONE || targetSpec == SPEC_NONE)
                return;

            spell_dru_symbiosis_caster_aura::spell_dru_symbiosis_caster_aura_AuraScript* script =
                dynamic_cast<spell_dru_symbiosis_caster_aura::spell_dru_symbiosis_caster_aura_AuraScript*>(symbiosis->GetScriptByName("spell_dru_symbiosis_caster_aura"));
            if (!script)
                return;

            uint32 targetSpell = 0;
            int32 targetOverride = 0;
            uint32 casterSpell = GetSpells(druidSpec, target->getClass(), GetSymbiosisRole(targetSpec), targetSpell, targetOverride);
            if (!casterSpell)
                return;

            script->Bind(casterSpell, targetSpell, target->GetGUID());
            if (targetSpell && targetOverride)
                druid->CastCustomSpell(targetSpell, SPELLVALUE_BASE_POINT0, targetOverride, target, true);
        }

        static uint32 PickBySpec(uint32 druidSpec, uint32 balance, uint32 guardian, uint32 feral, uint32 restoration)
        {
            switch (druidSpec)
            {
                case SPEC_DRUID_BALANCE:
                    return balance;
                case SPEC_DRUID_GUARDIAN:
                    return guardian;
                case SPEC_DRUID_FERAL:
                    return feral;
                case SPEC_DRUID_RESTORATION:
                    return restoration;
                default:
                    return 0;
            }
        }

        static uint32 GetSpells(uint32 druidSpec, uint8 targetClass, SymbiosisRole targetRole, uint32& targetSpell, int32& targetOverride)
        {
            switch (targetClass)
            {
                case CLASS_DEATH_KNIGHT:
                    targetSpell = SPELL_DRUID_SYMBIOSIS_DEATH_KNIGHT;
                    if (targetRole == SYMBIOSIS_ROLE_DPS)
                        targetOverride = 113516;    // Wild Mushroom: Plague
                    else if (targetRole == SYMBIOSIS_ROLE_TANK)
                        targetOverride = 113072;    // Might of Ursoc
                    // Anti-Magic Shell, Bone Shield, Death Coil, Icebound Fortitude
                    return PickBySpec(druidSpec, 110570, 122285, 122282, 110575);
                case CLASS_HUNTER:
                    targetSpell = SPELL_DRUID_SYMBIOSIS_HUNTER;
                    targetOverride = 113073;        // Dash
                    // Misdirection, Ice Trap, Play Dead, Deterrence
                    return PickBySpec(druidSpec, 110588, 110600, 110597, 110617);
                case CLASS_MAGE:
                    targetSpell = SPELL_DRUID_SYMBIOSIS_MAGE;
                    targetOverride = 113074;        // Healing Touch
                    // Mirror Image, Frost Armor, Frost Nova, Ice Block
                    return PickBySpec(druidSpec, 110621, 110694, 110693, 110696);
                case CLASS_MONK:
                    targetSpell = SPELL_DRUID_SYMBIOSIS_MONK;
                    if (targetRole == SYMBIOSIS_ROLE_TANK)
                        targetOverride = 113306;    // Survival Instincts
                    else if (targetRole == SYMBIOSIS_ROLE_DPS)
                        targetOverride = 127361;    // Bear Hug
                    else
                        targetOverride = 113275;    // Entangling Roots
                    // Grapple Weapon, Elusive Brew, Clash, Fortifying Brew
                    return PickBySpec(druidSpec, 126458, 126453, 126449, 126456);
                case CLASS_PALADIN:
                    targetSpell = SPELL_DRUID_SYMBIOSIS_PALADIN;
                    if (targetRole == SYMBIOSIS_ROLE_TANK)
                        targetOverride = 113075;    // Barkskin
                    else if (targetRole == SYMBIOSIS_ROLE_DPS)
                        targetOverride = 122287;    // Wrath
                    else
                        targetOverride = 113269;    // Rebirth
                    // Hammer of Justice, Consecration, Divine Shield, Cleanse
                    return PickBySpec(druidSpec, 110698, 110701, 110700, 122288);
                case CLASS_PRIEST:
                    targetSpell = SPELL_DRUID_SYMBIOSIS_PRIEST;
                    if (targetRole == SYMBIOSIS_ROLE_DPS)
                        targetOverride = 113277;    // Tranquility
                    else if (targetRole == SYMBIOSIS_ROLE_HEALER)
                        targetOverride = 113506;    // Cyclone
                    // Mass Dispel, Fear Ward, Dispersion, Leap of Faith
                    return PickBySpec(druidSpec, 110707, 110717, 110715, 110718);
                case CLASS_ROGUE:
                    targetSpell = SPELL_DRUID_SYMBIOSIS_ROGUE;
                    targetOverride = 113613;        // Growl
                    // Cloak of Shadows, Feint, Redirect, Evasion
                    return PickBySpec(druidSpec, 110788, 122289, 110730, 110791);
                case CLASS_SHAMAN:
                    targetSpell = SPELL_DRUID_SYMBIOSIS_SHAMAN;
                    if (targetRole == SYMBIOSIS_ROLE_HEALER)
                        targetOverride = 113289;    // Prowl
                    else
                        targetOverride = 113286;    // Solar Beam
                    // Purge, Lightning Shield, Feral Spirit, Spiritwalker's Grace
                    return PickBySpec(druidSpec, 110802, 110803, 110807, 110806);
                case CLASS_WARLOCK:
                    targetSpell = SPELL_DRUID_SYMBIOSIS_WARLOCK;
                    targetOverride = 113295;        // Rejuvenation
                    // Unending Resolve, Life Tap, Soul Swap, Demonic Circle: Teleport
                    return PickBySpec(druidSpec, 122291, 122290, 110810, 112970);
                case CLASS_WARRIOR:
                    targetSpell = SPELL_DRUID_SYMBIOSIS_WARRIOR;
                    if (targetRole == SYMBIOSIS_ROLE_TANK)
                        targetOverride = 122286;    // Savage Defense
                    else
                        targetOverride = 122294;    // Stampeding Shout
                    // Intervene, Spell Reflection, Shattering Blow, Intimidating Roar
                    return PickBySpec(druidSpec, 122292, 113002, 112997, 113004);
                default:
                    return 0;
            }
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dru_symbiosis_SpellScript::CheckCast);
            OnEffectHitTarget += SpellEffectFn(spell_dru_symbiosis_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
            AfterCast += SpellCastFn(spell_dru_symbiosis_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_symbiosis_SpellScript();
    }
};

// 126456 - Fortifying Brew (Symbiosis)
class spell_dru_symbiosis_fortifying_brew : public SpellScriptLoader
{
public:
    spell_dru_symbiosis_fortifying_brew() : SpellScriptLoader("spell_dru_symbiosis_fortifying_brew") { }

    class spell_dru_symbiosis_fortifying_brew_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_symbiosis_fortifying_brew_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            amount = int32(GetUnitOwner()->CountPctFromMaxHealth(amount));
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_symbiosis_fortifying_brew_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_symbiosis_fortifying_brew_AuraScript();
    }
};

// 110597 - Play Dead (Symbiosis)
class spell_dru_play_death : public SpellScriptLoader
{
public:
    spell_dru_play_death() : SpellScriptLoader("spell_dru_play_death") { }

    class spell_dru_play_death_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_play_death_AuraScript);

        uint32 _health = 0;
        int32 _mana = 0;

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            _health = GetTarget()->GetHealth();
            _mana = GetTarget()->GetPower(POWER_MANA);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (_health && GetTarget()->IsAlive())
            {
                GetTarget()->SetHealth(_health);
                GetTarget()->SetPower(POWER_MANA, _mana);
            }
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_dru_play_death_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_FEIGN_DEATH, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_dru_play_death_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_FEIGN_DEATH, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_play_death_AuraScript();
    }
};

// 122290 - Life Tap (Symbiosis)
class spell_dru_life_tap : public SpellScriptLoader
{
public:
    spell_dru_life_tap() : SpellScriptLoader("spell_dru_life_tap") { }

    class spell_dru_life_tap_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_life_tap_SpellScript);

        void HandleHit()
        {
            GetCaster()->ModifyHealth(-int32(GetCaster()->CountPctFromMaxHealth(20)));
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_life_tap_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_life_tap_SpellScript();
    }
};

// 112970 - Demonic Circle: Teleport (Symbiosis)
class spell_dru_demonic_circle_teleport : public SpellScriptLoader
{
public:
    spell_dru_demonic_circle_teleport() : SpellScriptLoader("spell_dru_demonic_circle_teleport") { }

    class spell_dru_demonic_circle_teleport_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_demonic_circle_teleport_SpellScript);

        // Uses the circle of the warlock this druid shares Symbiosis with.
        GameObject* GetDemonicCircle()
        {
            Player* druid = GetCaster()->ToPlayer();
            Group* group = druid ? druid->GetGroup() : NULL;
            if (!group)
                return NULL;

            for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
                if (Player* member = itr->GetSource())
                    if (member->HasAura(SPELL_DRUID_SYMBIOSIS_WARLOCK, druid->GetGUID()))
                        return member->GetGameObject(SPELL_WARLOCK_DEMONIC_CIRCLE_SUMMON);

            return NULL;
        }

        SpellCastResult CheckCast()
        {
            GameObject* circle = GetDemonicCircle();
            if (!circle)
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;
            return GetCaster()->IsWithinDist(circle, GetSpellInfo()->GetMaxRange(true)) ? SpellCastResult::SPELL_CAST_OK : SpellCastResult::SPELL_FAILED_OUT_OF_RANGE;
        }

        void HandleHit()
        {
            if (GameObject* circle = GetDemonicCircle())
            {
                GetCaster()->NearTeleportTo(circle->GetPositionX(), circle->GetPositionY(), circle->GetPositionZ(), circle->GetOrientation());
                GetCaster()->RemoveMovementImpairingAuras();
            }
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dru_demonic_circle_teleport_SpellScript::CheckCast);
            OnHit += SpellHitFn(spell_dru_demonic_circle_teleport_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_demonic_circle_teleport_SpellScript();
    }
};

// 112997 - Shattering Blow (Symbiosis)
class spell_dru_shattering_blow : public SpellScriptLoader
{
public:
    spell_dru_shattering_blow() : SpellScriptLoader("spell_dru_shattering_blow") { }

    class spell_dru_shattering_blow_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_shattering_blow_SpellScript);

        void HandleHit()
        {
            Unit* target = GetHitUnit();
            if (target->HasAuraWithMechanic(1 << MECHANIC_IMMUNE_SHIELD))
            {
                target->RemoveAurasDueToSpell(SPELL_DRUID_SHATTERING_BLOW);
                target->RemoveAurasWithMechanic(1 << MECHANIC_IMMUNE_SHIELD, AURA_REMOVE_BY_ENEMY_SPELL, SPELL_MAGE_GLYPH_OF_ICE_BLOCK_IMMUNITY);
            }

            if (GetCaster()->GetShapeshiftForm() != FORM_CAT)
                GetCaster()->CastSpell(GetCaster(), SPELL_DRUID_CAT_FORM, true);
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_shattering_blow_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_shattering_blow_SpellScript();
    }
};

// 113004 - Intimidating Roar (Symbiosis)
class spell_dru_intimidating_roar : public SpellScriptLoader
{
public:
    spell_dru_intimidating_roar() : SpellScriptLoader("spell_dru_intimidating_roar") { }

    class spell_dru_intimidating_roar_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_intimidating_roar_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (Unit* target = GetExplTargetUnit())
                targets.remove(target);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_intimidating_roar_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_intimidating_roar_SpellScript::FilterTargets, EFFECT_2, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_intimidating_roar_SpellScript();
    }
};

// 125047 - Glyph of the Treant
class spell_dru_glyph_of_the_treant : public SpellScriptLoader
{
public:
    spell_dru_glyph_of_the_treant() : SpellScriptLoader("spell_dru_glyph_of_the_treant") { }

    class spell_dru_glyph_of_the_treant_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_glyph_of_the_treant_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* druid = GetTarget()->ToPlayer())
                druid->learnSpell(SPELL_DRUID_TREANT_FORM, false);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* druid = GetTarget()->ToPlayer())
                druid->removeSpell(SPELL_DRUID_TREANT_FORM, false, false);
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_dru_glyph_of_the_treant_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_glyph_of_the_treant_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_glyph_of_the_treant_AuraScript();
    }
};

// 5215 - Prowl
class spell_dru_prowl : public SpellScriptLoader
{
public:
    spell_dru_prowl() : SpellScriptLoader("spell_dru_prowl") { }

    class spell_dru_prowl_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_prowl_AuraScript);

        void CalculateDecreaseSpeed(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (GetUnitOwner()->HasAura(SPELL_DRUID_GLYPH_OF_PROWL))
                amount = 0;
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_prowl_AuraScript::CalculateDecreaseSpeed, EFFECT_1, SPELL_AURA_MOD_DECREASE_SPEED);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_prowl_AuraScript();
    }
};

// 106898 - Stampeding Roar, 77761 - Stampeding Roar (Bear), 77764 - Stampeding Roar (Cat), 97993 - Stampeding Roar
class spell_dru_stampeding_roar : public SpellScriptLoader
{
public:
    spell_dru_stampeding_roar() : SpellScriptLoader("spell_dru_stampeding_roar") { }

    class spell_dru_stampeding_roar_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_stampeding_roar_SpellScript);

        void HandleCast()
        {
            Unit* druid = GetCaster();
            if (GetSpellInfo()->Id != SPELL_DRUID_STAMPEDING_ROAR || druid->HasAura(SPELL_DRUID_GLYPH_OF_STAMPEDING_ROAR))
                return;

            ShapeshiftForm form = druid->GetShapeshiftForm();
            if (form != FORM_CAT && form != FORM_BEAR)
                druid->CastSpell(druid, SPELL_DRUID_BEAR_FORM, true);
        }

        void HandleHit()
        {
            GetHitUnit()->RemoveMovementImpairingAuras();
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_dru_stampeding_roar_SpellScript::HandleCast);
            OnHit += SpellHitFn(spell_dru_stampeding_roar_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_stampeding_roar_SpellScript();
    }
};

// 18960 - Teleport: Moonglade
class spell_dru_teleport_moonglade : public SpellScriptLoader
{
public:
    spell_dru_teleport_moonglade() : SpellScriptLoader("spell_dru_teleport_moonglade") { }

    class spell_dru_teleport_moonglade_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_teleport_moonglade_SpellScript);

        SpellCastResult CheckCast()
        {
            if (GetCaster()->GetMap()->IsDungeon())
                return SpellCastResult::SPELL_FAILED_NOT_HERE;

            if (Player* player = GetCaster()->ToPlayer())
                if (Battleground* bg = player->GetBattleground())
                    if (bg->isRated())
                        return SpellCastResult::SPELL_FAILED_NOT_HERE;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void HandleBeforeCast()
        {
            if (Player* player = GetCaster()->ToPlayer())
                if (player->GetZoneId() != ZONE_MOONGLADE)
                    player->SaveRecallPosition();
        }

        // Casting it again from Moonglade returns the druid to where they left from.
        void SelectDest(SpellDestination& dest)
        {
            Player* player = GetCaster()->ToPlayer();
            if (!player || player->GetZoneId() != ZONE_MOONGLADE)
                return;

            Position pos;
            pos.Relocate(player->m_recallX, player->m_recallY, player->m_recallZ, player->m_recallO);
            dest._position.m_mapId = player->m_recallMap;
            dest.Relocate(pos);
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dru_teleport_moonglade_SpellScript::CheckCast);
            BeforeCast += SpellCastFn(spell_dru_teleport_moonglade_SpellScript::HandleBeforeCast);
            OnDestinationTargetSelect += SpellDestinationTargetSelectFn(spell_dru_teleport_moonglade_SpellScript::SelectDest, EFFECT_0, TARGET_DEST_DB);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_teleport_moonglade_SpellScript();
    }
};

// 5215 - Prowl, 102547 - Prowl (Incarnation), 102280 - Displacer Beast
class spell_dru_activate_cat_form : public SpellScriptLoader
{
public:
    spell_dru_activate_cat_form() : SpellScriptLoader("spell_dru_activate_cat_form") { }

    class spell_dru_activate_cat_form_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_activate_cat_form_SpellScript);

        void HandleHit()
        {
            if (GetCaster()->GetShapeshiftForm() != FORM_CAT)
                GetCaster()->CastSpell(GetCaster(), SPELL_DRUID_CAT_FORM, true);
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_activate_cat_form_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_activate_cat_form_SpellScript();
    }
};

// 102793 - Ursol's Vortex
// CREATE_AREATRIGGER is spawned as a DynamicObject (no aura). Apply the snare while
// enemies remain inside and pull them back to the center the first time they leave.
class spell_dru_ursols_vortex_dynamic : public DynamicObjectScript
{
public:
    spell_dru_ursols_vortex_dynamic() : DynamicObjectScript("spell_dru_ursols_vortex_dynamic") { }

    void OnUpdate(DynamicObject* dynObj, uint32 /*diff*/) override
    {
        if (dynObj->GetSpellId() != SPELL_DRUID_URSOLS_VORTEX)
            return;

        Unit* caster = dynObj->GetCaster();
        if (!caster)
            return;

        float const radius = dynObj->GetRadius();
        if (radius <= 0.0f)
            return;

        uint64 const dynGuid = dynObj->GetGUID();
        std::unordered_set<uint64>& inside = _unitsInside[dynGuid];

        std::list<Unit*> targets;
        Skyfire::AnyAoETargetUnitInObjectRangeCheck check(dynObj, caster, radius);
        Skyfire::UnitListSearcher<Skyfire::AnyAoETargetUnitInObjectRangeCheck> searcher(dynObj, targets, check);
        dynObj->VisitNearbyObject(radius, searcher);

        std::unordered_set<uint64> current;
        for (Unit* target : targets)
        {
            if (!target)
                continue;

            current.insert(target->GetGUID());
            if (!target->HasAura(SPELL_DRUID_URSOLS_VORTEX_SNARE, caster->GetGUID()))
                caster->CastSpell(target, SPELL_DRUID_URSOLS_VORTEX_SNARE, true);
        }

        // DynObject expire skips OnUpdate; clean snares on the last ticks instead.
        bool const vortexEnding = dynObj->GetDuration() <= 200;

        for (uint64 guid : inside)
        {
            if (current.find(guid) != current.end())
                continue;

            Unit* target = ObjectAccessor::GetUnit(*dynObj, guid);
            if (!target)
                continue;

            if (!vortexEnding && !target->HasAura(SPELL_DRUID_URSOLS_VORTEX_JUMP_DEST))
            {
                if (SpellInfo const* jumpInfo = sSpellMgr->GetSpellInfo(SPELL_DRUID_URSOLS_VORTEX_JUMP_DEST))
                {
                    if (!target->IsImmunedToSpell(jumpInfo) && !target->IsImmunedToDamage(jumpInfo)
                        && caster->MagicSpellHitResult(target, jumpInfo) == SPELL_MISS_NONE)
                        target->CastSpell(dynObj->GetPositionX(), dynObj->GetPositionY(), dynObj->GetPositionZ(),
                            SPELL_DRUID_URSOLS_VORTEX_JUMP_DEST, true);
                }
            }

            target->RemoveAurasDueToSpell(SPELL_DRUID_URSOLS_VORTEX_SNARE, caster->GetGUID());
        }

        if (vortexEnding)
        {
            for (uint64 guid : current)
                if (Unit* target = ObjectAccessor::GetUnit(*dynObj, guid))
                    target->RemoveAurasDueToSpell(SPELL_DRUID_URSOLS_VORTEX_SNARE, caster->GetGUID());

            _unitsInside.erase(dynGuid);
        }
        else
            inside.swap(current);
    }

private:
    std::unordered_map<uint64, std::unordered_set<uint64>> _unitsInside;
};

// 102560 - Incarnation: Chosen of Elune
// Applies the Arcane/Nature damage buff (122114) for the Incarnation duration.
class spell_dru_incarnation_chosen_of_elune : public SpellScriptLoader
{
public:
    spell_dru_incarnation_chosen_of_elune() : SpellScriptLoader("spell_dru_incarnation_chosen_of_elune") { }

    class spell_dru_incarnation_chosen_of_elune_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_incarnation_chosen_of_elune_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_CHOSEN_OF_ELUNE_DAMAGE))
                return false;
            return true;
        }

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* target = GetTarget())
                target->CastSpell(target, SPELL_DRUID_CHOSEN_OF_ELUNE_DAMAGE, true);
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* target = GetTarget())
                target->RemoveAurasDueToSpell(SPELL_DRUID_CHOSEN_OF_ELUNE_DAMAGE);
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_dru_incarnation_chosen_of_elune_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_incarnation_chosen_of_elune_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_incarnation_chosen_of_elune_AuraScript();
    }
};

// 102543 / 102558 / 102560 - Incarnation skins
// Enter the matching shapeshift (if needed) and refresh display to the armored model.
class spell_dru_incarnation_skins : public SpellScriptLoader
{
public:
    spell_dru_incarnation_skins() : SpellScriptLoader("spell_dru_incarnation_skins") { }

    class spell_dru_incarnation_skins_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_incarnation_skins_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* owner = GetUnitOwner();
            if (!owner)
                return;

            uint32 formSpell = 0;
            ShapeshiftForm neededForm = FORM_NONE;
            switch (GetId())
            {
                case SPELL_DRUID_INCARNATION_KING_OF_JUNGLE:
                    formSpell = SPELL_DRUID_CAT_FORM;
                    neededForm = FORM_CAT;
                    break;
                case SPELL_DRUID_INCARNATION_SON_OF_URSOC:
                    formSpell = SPELL_DRUID_BEAR_FORM;
                    neededForm = FORM_BEAR;
                    break;
                case SPELL_DRUID_INCARNATION_CHOSEN_OF_ELUNE:
                    formSpell = SPELL_DRUID_MOONKIN_FORM;
                    neededForm = FORM_MOONKIN;
                    break;
                default:
                    break;
            }

            if (formSpell && owner->GetShapeshiftForm() != neededForm)
                owner->CastSpell(owner, formSpell, true);

            if (uint32 model = owner->GetModelForForm(owner->GetShapeshiftForm()))
                owner->SetDisplayId(model);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* owner = GetUnitOwner())
                owner->RestoreDisplayId();
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_dru_incarnation_skins_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_incarnation_skins_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_incarnation_skins_AuraScript();
    }
};

// 33891 - Incarnation: Tree of Life
// Apply the 30s Incarnation wrapper so the form can be freely re-entered without CD.
class spell_dru_tree_of_life : public SpellScriptLoader
{
public:
    spell_dru_tree_of_life() : SpellScriptLoader("spell_dru_tree_of_life") { }

    class spell_dru_tree_of_life_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_tree_of_life_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_INCARNATION_AURA))
                return false;
            return true;
        }

        void HandleCast()
        {
            Unit* caster = GetCaster();
            if (!caster->HasAura(SPELL_DRUID_INCARNATION_AURA))
                caster->CastSpell(caster, SPELL_DRUID_INCARNATION_AURA, true);
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_dru_tree_of_life_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_tree_of_life_SpellScript();
    }
};

// 117679 - Incarnation (Tree of Life duration wrapper)
// When the window ends, force-cancel Tree of Life form.
class spell_dru_incarnation_tree_aura : public SpellScriptLoader
{
public:
    spell_dru_incarnation_tree_aura() : SpellScriptLoader("spell_dru_incarnation_tree_aura") { }

    class spell_dru_incarnation_tree_aura_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_incarnation_tree_aura_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_INCARNATION_TREE_OF_LIFE))
                return false;
            return true;
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* target = GetTarget())
                target->RemoveAurasDueToSpell(SPELL_DRUID_INCARNATION_TREE_OF_LIFE);
        }

        void Register() override
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_incarnation_tree_aura_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_IGNORE_SPELL_COOLDOWN, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_incarnation_tree_aura_AuraScript();
    }
};

// 102351 - Cenarion Ward
// Dummy aura procs on damage taken; default HandleDummyAuraProc casts the heal on the
// attacker. Redirect the HoT (102352) onto the ward bearer and consume the charge.
class spell_dru_cenarion_ward : public SpellScriptLoader
{
public:
    spell_dru_cenarion_ward() : SpellScriptLoader("spell_dru_cenarion_ward") { }

    class spell_dru_cenarion_ward_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_cenarion_ward_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_CENARION_WARD_HEAL))
                return false;
            return true;
        }

        void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();

            Unit* caster = GetCaster();
            Unit* target = GetTarget();
            if (!caster || !target)
                return;

            caster->CastSpell(target, SPELL_DRUID_CENARION_WARD_HEAL, true);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_dru_cenarion_ward_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_cenarion_ward_AuraScript();
    }
};

// 106922 - Might of Ursoc
class spell_dru_might_of_ursoc: public SpellScriptLoader
{
public:
    spell_dru_might_of_ursoc() : SpellScriptLoader("spell_dru_might_of_ursoc") { }

    class spell_dru_might_of_ursoc_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_might_of_ursoc_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_BEAR_FORM))
                return false;
            return true;
        }

        void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* caster = GetCaster())
            {
                // cast bear form first
                if (caster->GetShapeshiftForm() != FORM_BEAR)
                    caster->CastSpell(caster, SPELL_DRUID_BEAR_FORM, true); // activate bear form
                // then calculate amount
                amount = aurEff->GetBase()->GetUnitOwner()->CountPctFromMaxHealth(amount);
            }
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_might_of_ursoc_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH_2);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dru_might_of_ursoc_AuraScript();
    }
};

// -16972 - Predatory Strikes
class spell_dru_predatory_strikes : public SpellScriptLoader
{
public:
    spell_dru_predatory_strikes() : SpellScriptLoader("spell_dru_predatory_strikes") { }

    class spell_dru_predatory_strikes_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_predatory_strikes_AuraScript);

        void UpdateAmount(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* target = GetTarget()->ToPlayer())
                target->UpdateAttackPowerAndDamage();
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_dru_predatory_strikes_AuraScript::UpdateAmount, EFFECT_ALL, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
            AfterEffectRemove += AuraEffectRemoveFn(spell_dru_predatory_strikes_AuraScript::UpdateAmount, EFFECT_ALL, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_predatory_strikes_AuraScript();
    }
};

// 62606 - Savage Defense
// MoP: Dummy cast that applies dodge aura 132402 (not the old Cata absorb shield).
class spell_dru_savage_defense : public SpellScriptLoader
{
public:
    spell_dru_savage_defense() : SpellScriptLoader("spell_dru_savage_defense") { }

    class spell_dru_savage_defense_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_savage_defense_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_SAVAGE_DEFENSE_AURA))
                return false;
            return true;
        }

        void HandleCast()
        {
            Unit* caster = GetCaster();
            if (Aura* aura = caster->GetAura(SPELL_DRUID_SAVAGE_DEFENSE_AURA))
            {
                aura->SetDuration(aura->GetDuration() + 6 * IN_MILLISECONDS);
                if (aura->GetDuration() > aura->GetMaxDuration())
                    aura->SetMaxDuration(aura->GetDuration());
            }
            else
                caster->CastSpell(caster, SPELL_DRUID_SAVAGE_DEFENSE_AURA, true);
        }

        void Register() override
        {
            AfterCast += SpellCastFn(spell_dru_savage_defense_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_savage_defense_SpellScript();
    }
};

// 52610 - Savage Roar
class spell_dru_savage_roar : public SpellScriptLoader
{
public:
    spell_dru_savage_roar() : SpellScriptLoader("spell_dru_savage_roar") { }

    class spell_dru_savage_roar_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_savage_roar_SpellScript);

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            if (caster->GetShapeshiftForm() != FORM_CAT)
                return SpellCastResult::SPELL_FAILED_ONLY_SHAPESHIFT;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dru_savage_roar_SpellScript::CheckCast);
        }
    };

    class spell_dru_savage_roar_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_savage_roar_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_SAVAGE_ROAR))
                return false;
            return true;
        }

        void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            Player* player = caster ? caster->ToPlayer() : NULL;
            if (!player)
                return;

            if (player->GetShapeshiftForm() == FORM_CAT)
                player->CastSpell(player, SPELL_DRUID_SAVAGE_ROAR_TRIGGER, true, NULL, aurEff, GetCasterGUID());

            // The glyphed version sets its own duration once combo points are spent.
            if (GetId() != SPELL_DRUID_SAVAGE_ROAR)
                return;

            uint8 comboPoints = player->GetComboPoints();
            int32 minduration = GetSpellInfo()->GetDuration();
            int32 maxduration = GetSpellInfo()->GetMaxDuration();
            int32 duration = minduration + int32((maxduration - minduration) * comboPoints / 5);

            SetDuration(duration);
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->RemoveAurasDueToSpell(SPELL_DRUID_SAVAGE_ROAR_TRIGGER);
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_dru_savage_roar_AuraScript::OnApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_savage_roar_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_savage_roar_SpellScript();
    }

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_savage_roar_AuraScript();
    }
};

// 52610, 127538 - Savage Roar
class spell_dru_savage_roar_duration_check : public SpellScriptLoader
{
public:
    spell_dru_savage_roar_duration_check() : SpellScriptLoader("spell_dru_savage_roar_duration_check") { }

    class spell_dru_savage_roar_duration_check_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_savage_roar_duration_check_SpellScript);

        SpellCastResult CheckCast()
        {
            Player* druid = GetCaster()->ToPlayer();
            if (!druid)
                return SpellCastResult::SPELL_CAST_OK;

            Aura* savageRoar = druid->GetAura(GetSpellInfo()->Id);
            if (!savageRoar)
                return SpellCastResult::SPELL_CAST_OK;

            int32 newDuration = 12 * IN_MILLISECONDS;
            if (AuraEffect const* periodic = savageRoar->GetEffect(EFFECT_2))
                newDuration += periodic->GetPeriodicTimer();
            newDuration += druid->GetComboPoints() * 6 * IN_MILLISECONDS;

            if (savageRoar->GetDuration() > newDuration)
                return SpellCastResult::SPELL_FAILED_AURA_BOUNCED;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dru_savage_roar_duration_check_SpellScript::CheckCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_savage_roar_duration_check_SpellScript();
    }
};

// 127538 - Savage Roar (Glyph of Savagery)
class spell_dru_savage_roar_glyphed : public SpellScriptLoader
{
public:
    spell_dru_savage_roar_glyphed() : SpellScriptLoader("spell_dru_savage_roar_glyphed") { }

    class spell_dru_savage_roar_glyphed_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_savage_roar_glyphed_SpellScript);

        bool Load() override
        {
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void HandleAfterHit()
        {
            Player* druid = GetCaster()->ToPlayer();
            if (int8 comboPoints = druid->GetComboPoints())
            {
                if (Aura* roar = druid->GetAura(GetSpellInfo()->Id))
                {
                    roar->SetMaxDuration(roar->CalcMaxDuration(druid) + comboPoints * 6 * IN_MILLISECONDS);
                    roar->SetDuration(roar->GetMaxDuration());
                }
                druid->ClearComboPoints();
            }
            else if (roll_chance_i(10))
                druid->CastSpell(druid, SPELL_DRUID_PREDATORY_SWIFTNESS, true);
        }

        void Register() override
        {
            AfterHit += SpellHitFn(spell_dru_savage_roar_glyphed_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_savage_roar_glyphed_SpellScript();
    }
};

// 5221 - Shred, 6785 - Ravage, 33876 - Mangle (Cat), 102545 - Ravage!
class spell_dru_rip_duration : public SpellScriptLoader
{
public:
    spell_dru_rip_duration() : SpellScriptLoader("spell_dru_rip_duration") { }

    class spell_dru_rip_duration_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_rip_duration_SpellScript);

        void HandleHit()
        {
            Unit* caster = GetCaster();
            if (caster->GetShapeshiftForm() != FORM_CAT)
                return;

            Aura* rip = GetHitUnit()->GetAura(SPELL_DRUID_RIP, caster->GetGUID());
            if (!rip)
                return;

            // Each hit extends Rip by 2 sec, up to 6 sec over its normal duration.
            int32 maxDuration = rip->GetMaxDuration();
            int32 maxPossibleDuration = rip->CalcMaxDuration(caster) + 6 * IN_MILLISECONDS;
            if (maxDuration >= maxPossibleDuration)
                return;

            int32 newMaxDuration = std::min(maxDuration + 2 * IN_MILLISECONDS, maxPossibleDuration);
            int32 newDuration = std::min(rip->GetDuration() + 2 * IN_MILLISECONDS, newMaxDuration);
            rip->SetMaxDuration(newMaxDuration);
            rip->SetDuration(newDuration);
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_rip_duration_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_rip_duration_SpellScript();
    }
};

// 5221 - Shred, 114236 - Shred!
class spell_dru_shred : public SpellScriptLoader
{
public:
    spell_dru_shred() : SpellScriptLoader("spell_dru_shred") { }

    class spell_dru_shred_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_shred_SpellScript);

        void HandleHit()
        {
            Unit::AuraEffectList const& periodicAuras = GetHitUnit()->GetAuraEffectsByType(SPELL_AURA_PERIODIC_DAMAGE);
            for (Unit::AuraEffectList::const_iterator itr = periodicAuras.begin(); itr != periodicAuras.end(); ++itr)
            {
                if ((*itr)->GetSpellInfo()->GetAllEffectsMechanicMask() & (1 << MECHANIC_BLEED))
                {
                    SetHitDamage(int32(GetHitDamage() * 1.2f));
                    break;
                }
            }
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_shred_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_shred_SpellScript();
    }
};

// 5217 - Tiger's Fury
class spell_dru_tigers_fury : public SpellScriptLoader
{
public:
    spell_dru_tigers_fury() : SpellScriptLoader("spell_dru_tigers_fury") { }

    class spell_dru_tigers_fury_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_tigers_fury_SpellScript);

        SpellCastResult CheckBerserk()
        {
            if (GetCaster()->HasAura(SPELL_DRUID_BERSERK_CAT))
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;
            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dru_tigers_fury_SpellScript::CheckBerserk);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_tigers_fury_SpellScript();
    }
};

// 5217 - Tiger's Fury, 106951 - Berserk (Cat)
class spell_dru_glyph_of_shred : public SpellScriptLoader
{
public:
    spell_dru_glyph_of_shred() : SpellScriptLoader("spell_dru_glyph_of_shred") { }

    class spell_dru_glyph_of_shred_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_glyph_of_shred_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            if (target->HasAura(SPELL_DRUID_GLYPH_OF_SHRED))
                target->CastSpell(target, SPELL_DRUID_GLYPH_OF_SHRED_OVERRIDE, true);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            uint32 otherSpell = GetId() == SPELL_DRUID_BERSERK_CAT ? SPELL_DRUID_TIGERS_FURY : SPELL_DRUID_BERSERK_CAT;
            if (!target->HasAura(otherSpell))
                target->RemoveAurasDueToSpell(SPELL_DRUID_GLYPH_OF_SHRED_OVERRIDE);
        }

        void Register() override
        {
            if (m_scriptSpellId == SPELL_DRUID_BERSERK_CAT)
            {
                OnEffectApply += AuraEffectApplyFn(spell_dru_glyph_of_shred_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_dru_glyph_of_shred_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
            }
            else
            {
                OnEffectApply += AuraEffectApplyFn(spell_dru_glyph_of_shred_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_dru_glyph_of_shred_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
            }
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_glyph_of_shred_AuraScript();
    }
};

// 16961 - Primal Fury
class spell_dru_primal_fury : public SpellScriptLoader
{
public:
    spell_dru_primal_fury() : SpellScriptLoader("spell_dru_primal_fury") { }

    class spell_dru_primal_fury_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_primal_fury_AuraScript);

        void HandleProc(ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            Unit* druid = GetTarget();
            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            switch (druid->GetShapeshiftForm())
            {
                case FORM_BEAR:
                    if (!spellInfo || spellInfo->Id == SPELL_DRUID_MANGLE_BEAR)
                        druid->CastSpell(druid, SPELL_DRUID_PRIMAL_FURY_BEAR, true);
                    break;
                case FORM_CAT:
                    if (spellInfo && eventInfo.GetProcTarget())
                        druid->CastSpell(eventInfo.GetProcTarget(), SPELL_DRUID_PRIMAL_FURY_CAT, true);
                    break;
                default:
                    break;
            }
        }

        void Register() override
        {
            OnProc += AuraProcFn(spell_dru_primal_fury_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_primal_fury_AuraScript();
    }
};

// 62078 - Swipe (Cat)
class spell_dru_swipe : public SpellScriptLoader
{
public:
    spell_dru_swipe() : SpellScriptLoader("spell_dru_swipe") { }

    class spell_dru_swipe_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_swipe_SpellScript);

        // The combo point only goes to the current target.
        void FilterTargets(std::list<WorldObject*>& targets)
        {
            uint64 targetGuid = GetCaster()->GetTarget();
            targets.remove_if([targetGuid](WorldObject* target) { return target->GetGUID() != targetGuid; });
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_swipe_SpellScript::FilterTargets, EFFECT_2, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_swipe_SpellScript();
    }
};

// 131537 - Item - Druid PvP Set Feral 4P Bonus
class spell_dru_item_pvp_feral_4p : public SpellScriptLoader
{
public:
    spell_dru_item_pvp_feral_4p() : SpellScriptLoader("spell_dru_item_pvp_feral_4p") { }

    class spell_dru_item_pvp_feral_4p_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_item_pvp_feral_4p_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->CastSpell(GetTarget(), SPELL_DRUID_STAMPEDE_TIMER, true);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->RemoveAurasDueToSpell(SPELL_DRUID_STAMPEDE);
            GetTarget()->RemoveAurasDueToSpell(SPELL_DRUID_STAMPEDE_TIMER);
        }

        void HandleProc(ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            if (spellInfo && spellInfo->Id == SPELL_DRUID_RAVAGE_STAMPEDE && GetTarget()->HasAura(SPELL_DRUID_STAMPEDE))
            {
                GetTarget()->RemoveAurasDueToSpell(SPELL_DRUID_STAMPEDE);
                GetTarget()->CastSpell(GetTarget(), SPELL_DRUID_STAMPEDE_TIMER, true);
            }
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_dru_item_pvp_feral_4p_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_item_pvp_feral_4p_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnProc += AuraProcFn(spell_dru_item_pvp_feral_4p_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_item_pvp_feral_4p_AuraScript();
    }
};

// 81022 - Stampede
class spell_dru_stampede_effect : public SpellScriptLoader
{
public:
    spell_dru_stampede_effect() : SpellScriptLoader("spell_dru_stampede_effect") { }

    class spell_dru_stampede_effect_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_stampede_effect_AuraScript);

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->CastSpell(GetTarget(), SPELL_DRUID_STAMPEDE_TIMER, true);
        }

        void Register() override
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_stampede_effect_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_stampede_effect_AuraScript();
    }
};

// 131538 - Stampede (timer)
class spell_dru_stampede_timer : public SpellScriptLoader
{
public:
    spell_dru_stampede_timer() : SpellScriptLoader("spell_dru_stampede_timer") { }

    class spell_dru_stampede_timer_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_stampede_timer_AuraScript);

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_DEFAULT)
                GetTarget()->CastSpell(GetTarget(), SPELL_DRUID_STAMPEDE, true);
        }

        void Register() override
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_stampede_timer_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_stampede_timer_AuraScript();
    }
};

// 135700 - Clearcasting (Item - Druid T16 Feral 2P Bonus)
class spell_dru_t16_feral_2p_bonus : public SpellScriptLoader
{
public:
    spell_dru_t16_feral_2p_bonus() : SpellScriptLoader("spell_dru_t16_feral_2p_bonus") { }

    class spell_dru_t16_feral_2p_bonus_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_t16_feral_2p_bonus_SpellScript);

        // Clearcasting cannot trigger procs, so the set bonus is applied here.
        void HandleCast()
        {
            if (AuraEffect const* bonus = GetCaster()->GetAuraEffect(SPELL_DRUID_T16_FERAL_2P_BONUS, EFFECT_0))
                if (uint32 triggerSpell = bonus->GetSpellInfo()->Effects[EFFECT_0].TriggerSpell)
                    GetCaster()->CastSpell(GetCaster(), triggerSpell, true);
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_dru_t16_feral_2p_bonus_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_t16_feral_2p_bonus_SpellScript();
    }
};

// 144841 - Item - Druid T16 Feral 4P Bonus
class spell_dru_t16_feral_4p_bonus : public SpellScriptLoader
{
public:
    spell_dru_t16_feral_4p_bonus() : SpellScriptLoader("spell_dru_t16_feral_4p_bonus") { }

    class spell_dru_t16_feral_4p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_t16_feral_4p_bonus_AuraScript);

        void HandleProc(ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            GetTarget()->CastSpell(GetTarget(), SPELL_DRUID_FERAL_RAGE, true);
        }

        void Register() override
        {
            OnProc += AuraProcFn(spell_dru_t16_feral_4p_bonus_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_t16_feral_4p_bonus_AuraScript();
    }
};

// 146874 - Feral Rage
class spell_dru_feral_rage : public SpellScriptLoader
{
public:
    spell_dru_feral_rage() : SpellScriptLoader("spell_dru_feral_rage") { }

    class spell_dru_feral_rage_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_feral_rage_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            if (!eventInfo.GetActionTarget() || !spellInfo)
                return false;
            if (spellInfo->Id == SPELL_DRUID_SAVAGE_ROAR_GLYPHED)
                return true;
            return spellInfo->SpellFamilyName == SPELLFAMILY_DRUID && spellInfo->NeedsComboPoints();
        }

        void HandleProc(ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            Unit* druid = GetTarget();
            uint64 targetGuid = eventInfo.GetActionTarget()->GetGUID();
            if (eventInfo.GetActionTarget() == druid)
                targetGuid = druid->GetTarget();

            // Delayed so the finisher has consumed its combo points first.
            druid->m_Events.AddEvent(new DelayedCastSpellEvent(druid, targetGuid, SPELL_DRUID_TIGERS_FURY_COMBO), druid->m_Events.CalculateTime(1));
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_dru_feral_rage_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_dru_feral_rage_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_feral_rage_AuraScript();
    }
};

// 5185 - Healing Touch
class spell_dru_healing_touch_dream_of_cenarius : public SpellScriptLoader
{
public:
    spell_dru_healing_touch_dream_of_cenarius() : SpellScriptLoader("spell_dru_healing_touch_dream_of_cenarius") { }

    class spell_dru_healing_touch_dream_of_cenarius_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_healing_touch_dream_of_cenarius_SpellScript);

        bool Load() override
        {
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        // Guardian Dream of Cenarius: the heal scales with attack power instead of spell power.
        void CalculateHeal(SpellEffIndex effIndex)
        {
            Unit* caster = GetCaster();
            if (!caster->HasAura(SPELL_DRUID_DREAM_OF_CENARIUS_GUARDIAN))
                return;

            int32 spellPower = caster->SpellBaseHealingBonusDone(GetSpellInfo()->GetSchoolMask());
            int32 attackPower = int32(caster->GetTotalAttackPowerValue(WeaponAttackType::BASE_ATTACK));
            SetEffectValue(GetEffectValue() + int32((attackPower - spellPower) * GetSpellInfo()->Effects[effIndex].BonusMultiplier));
        }

        void HandleHit()
        {
            Player* druid = GetCaster()->ToPlayer();
            AuraEffect const* talent = druid->GetAuraEffect(SPELL_DRUID_DREAM_OF_CENARIUS_TALENT, EFFECT_1);
            if (!talent)
                return;

            switch (druid->GetTalentSpecialization(druid->GetActiveSpec()))
            {
                case SPEC_DRUID_BALANCE:
                    druid->CastSpell(druid, SPELL_DRUID_DREAM_OF_CENARIUS_BALANCE, true);
                    break;
                case SPEC_DRUID_FERAL:
                    druid->CastSpell(druid, SPELL_DRUID_DREAM_OF_CENARIUS_FERAL, true);
                    break;
                case SPEC_DRUID_GUARDIAN:
                    break;
                default:
                    return;
            }

            SetHitHeal(GetHitHeal() + CalculatePct(GetHitHeal(), talent->GetAmount()));
        }

        void Register() override
        {
            OnEffectLaunchTarget += SpellEffectFn(spell_dru_healing_touch_dream_of_cenarius_SpellScript::CalculateHeal, EFFECT_0, SPELL_EFFECT_HEAL);
            OnHit += SpellHitFn(spell_dru_healing_touch_dream_of_cenarius_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_healing_touch_dream_of_cenarius_SpellScript();
    }
};

// 5176 - Wrath
class spell_dru_wrath_dream_of_cenarius : public SpellScriptLoader
{
public:
    spell_dru_wrath_dream_of_cenarius() : SpellScriptLoader("spell_dru_wrath_dream_of_cenarius") { }

    class spell_dru_wrath_dream_of_cenarius_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_wrath_dream_of_cenarius_SpellScript);

        void HandleHit()
        {
            Player* druid = GetCaster()->ToPlayer();
            if (!druid || druid->GetTalentSpecialization(druid->GetActiveSpec()) != SPEC_DRUID_RESTORATION)
                return;

            if (AuraEffect const* talent = druid->GetAuraEffect(SPELL_DRUID_DREAM_OF_CENARIUS_TALENT, EFFECT_2))
                SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), talent->GetAmount()));
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_wrath_dream_of_cenarius_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_wrath_dream_of_cenarius_SpellScript();
    }
};

// 108373 - Dream of Cenarius
class spell_dru_dream_of_cenarius : public SpellScriptLoader
{
public:
    spell_dru_dream_of_cenarius() : SpellScriptLoader("spell_dru_dream_of_cenarius") { }

    class spell_dru_dream_of_cenarius_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_dream_of_cenarius_AuraScript);

        bool Load() override
        {
            return GetOwner()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            Player* druid = GetTarget()->ToPlayer();
            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            if (!druid || !spellInfo)
                return false;

            switch (druid->GetTalentSpecialization(druid->GetActiveSpec()))
            {
                case SPEC_DRUID_RESTORATION:
                    return spellInfo->Id == SPELL_DRUID_WRATH && eventInfo.GetActionTarget();
                case SPEC_DRUID_GUARDIAN:
                    return spellInfo->Id == SPELL_DRUID_MANGLE_BEAR && (eventInfo.GetHitMask() & PROC_EX_CRITICAL_HIT)
                        && roll_chance_i(GetSpellInfo()->Effects[EFFECT_0].CalcValue(druid));
                default:
                    return false;
            }
        }

        void HandleProc(ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            Player* druid = GetTarget()->ToPlayer();
            switch (druid->GetTalentSpecialization(druid->GetActiveSpec()))
            {
                case SPEC_DRUID_GUARDIAN:
                    druid->CastSpell(druid, SPELL_DRUID_DREAM_OF_CENARIUS_GUARDIAN, true);
                    break;
                case SPEC_DRUID_RESTORATION:
                    if (DamageInfo* damageInfo = eventInfo.GetDamageInfo())
                        if (damageInfo->GetDamage())
                            druid->CastCustomSpell(SPELL_DRUID_DREAM_OF_CENARIUS_RESTO, SPELLVALUE_BASE_POINT0, int32(damageInfo->GetDamage()), eventInfo.GetActionTarget(), true);
                    break;
                default:
                    break;
            }
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_dru_dream_of_cenarius_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_dru_dream_of_cenarius_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_dream_of_cenarius_AuraScript();
    }
};

// 50286 - Starfall (Dummy)
class spell_dru_starfall_dummy : public SpellScriptLoader
{
public:
    spell_dru_starfall_dummy() : SpellScriptLoader("spell_dru_starfall_dummy") { }

    class spell_dru_starfall_dummy_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_starfall_dummy_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            Unit* druid = GetCaster();
            bool guidedStars = druid->HasAura(SPELL_DRUID_GLYPH_OF_GUIDED_STARS);
            targets.remove_if([druid, guidedStars](WorldObject* target)
            {
                Unit* unit = target->ToUnit();
                if (!unit || !druid->CanSeeOrDetect(unit))
                    return true;
                if (unit->HasBreakableByDamageCrowdControlAura())
                    return true;
                if (guidedStars && !unit->HasAura(SPELL_DRUID_MOONFIRE, druid->GetGUID()) && !unit->HasAura(SPELL_DRUID_SUNFIRE, druid->GetGUID()))
                    return true;
                return false;
            });

            Skyfire::Containers::RandomResizeList(targets, 2);
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            // Shapeshifting into an animal form or mounting cancels the effect
            if (caster->GetCreatureType() == CREATURE_TYPE_BEAST || caster->IsMounted())
            {
                if (SpellInfo const* spellInfo = GetTriggeringSpell())
                    caster->RemoveAurasDueToSpell(spellInfo->Id);
                return;
            }

            // Any effect which causes you to lose control of your character will supress the starfall effect.
            if (caster->HasUnitState(UNIT_STATE_CONTROLLED))
                return;

            caster->CastSpell(GetHitUnit(), uint32(GetEffectValue()), true);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_starfall_dummy_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_dru_starfall_dummy_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_starfall_dummy_SpellScript();
    }
};

// -78892 - Stampede
class spell_dru_stampede : public SpellScriptLoader
{
public:
    spell_dru_stampede() : SpellScriptLoader("spell_dru_stampede") { }

    class spell_dru_stampede_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_stampede_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_STAMPEDE_BAER_RANK_1) ||
                !sSpellMgr->GetSpellInfo(SPELL_DRUID_STAMPEDE_CAT_RANK_1) ||
                !sSpellMgr->GetSpellInfo(SPELL_DRUID_STAMPEDE_CAT_STATE) ||
                !sSpellMgr->GetSpellInfo(SPELL_DRUID_FERAL_CHARGE_CAT) ||
                !sSpellMgr->GetSpellInfo(SPELL_DRUID_FERAL_CHARGE_BEAR))
                return false;
            return true;
        }

        void HandleEffectCatProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            if (GetTarget()->GetShapeshiftForm() != FORM_CAT || eventInfo.GetDamageInfo()->GetSpellInfo()->Id != SPELL_DRUID_FERAL_CHARGE_CAT)
                return;

            GetTarget()->CastSpell(GetTarget(), sSpellMgr->GetSpellWithRank(SPELL_DRUID_STAMPEDE_CAT_RANK_1, GetSpellInfo()->GetRank()), true, NULL, aurEff);
            GetTarget()->CastSpell(GetTarget(), SPELL_DRUID_STAMPEDE_CAT_STATE, true, NULL, aurEff);
        }

        void HandleEffectBearProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            if (GetTarget()->GetShapeshiftForm() != FORM_BEAR || eventInfo.GetDamageInfo()->GetSpellInfo()->Id != SPELL_DRUID_FERAL_CHARGE_BEAR)
                return;

            GetTarget()->CastSpell(GetTarget(), sSpellMgr->GetSpellWithRank(SPELL_DRUID_STAMPEDE_BAER_RANK_1, GetSpellInfo()->GetRank()), true, NULL, aurEff);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_dru_stampede_AuraScript::HandleEffectCatProc, EFFECT_0, SPELL_AURA_DUMMY);
            OnEffectProc += AuraEffectProcFn(spell_dru_stampede_AuraScript::HandleEffectBearProc, EFFECT_1, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_stampede_AuraScript();
    }
};

// 61336 - Survival Instincts
class spell_dru_survival_instincts : public SpellScriptLoader
{
public:
    spell_dru_survival_instincts() : SpellScriptLoader("spell_dru_survival_instincts") { }

    class spell_dru_survival_instincts_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_survival_instincts_SpellScript);

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            if (!caster->IsInFeralForm())
                return SpellCastResult::SPELL_FAILED_ONLY_SHAPESHIFT;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dru_survival_instincts_SpellScript::CheckCast);
        }
    };

    class spell_dru_survival_instincts_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_survival_instincts_AuraScript);

        bool Validate(SpellInfo const* /*spell*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_SURVIVAL_INSTINCTS))
                return false;
            return true;
        }

        void AfterApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            int32 bp0 = target->CountPctFromMaxHealth(aurEff->GetAmount());
            target->CastCustomSpell(target, SPELL_DRUID_SURVIVAL_INSTINCTS, &bp0, NULL, NULL, true);
        }

        void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->RemoveAurasDueToSpell(SPELL_DRUID_SURVIVAL_INSTINCTS);
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_dru_survival_instincts_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
            AfterEffectRemove += AuraEffectRemoveFn(spell_dru_survival_instincts_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_survival_instincts_SpellScript();
    }

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_survival_instincts_AuraScript();
    }
};

// 40121 - Swift Flight Form (Passive)
class spell_dru_swift_flight_passive : public SpellScriptLoader
{
public:
    spell_dru_swift_flight_passive() : SpellScriptLoader("spell_dru_swift_flight_passive") { }

    class spell_dru_swift_flight_passive_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_swift_flight_passive_AuraScript);

        bool Load() override
        {
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Player* caster = GetCaster()->ToPlayer())
                if (caster->GetSkillValue(SKILL_RIDING) >= 375)
                    amount = 310;
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_swift_flight_passive_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_INCREASE_VEHICLE_FLIGHT_SPEED);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_swift_flight_passive_AuraScript();
    }
};

// 70691 - Item T10 Restoration 4P Bonus
class spell_dru_t10_restoration_4p_bonus : public SpellScriptLoader
{
public:
    spell_dru_t10_restoration_4p_bonus() : SpellScriptLoader("spell_dru_t10_restoration_4p_bonus") { }

    class spell_dru_t10_restoration_4p_bonus_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_t10_restoration_4p_bonus_SpellScript);

        bool Load() override
        {
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (!GetCaster()->ToPlayer()->GetGroup())
            {
                targets.clear();
                targets.push_back(GetCaster());
            }
            else
            {
                targets.remove(GetExplTargetUnit());
                std::list<Unit*> tempTargets;
                for (std::list<WorldObject*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
                    if ((*itr)->GetTypeId() == TypeID::TYPEID_PLAYER && GetCaster()->IsInRaidWith((*itr)->ToUnit()))
                        tempTargets.push_back((*itr)->ToUnit());

                if (tempTargets.empty())
                {
                    targets.clear();
                    FinishCast(SpellCastResult::SPELL_FAILED_DONT_REPORT);
                    return;
                }

                Unit* target = Skyfire::Containers::SelectRandomContainerElement(tempTargets);
                targets.clear();
                targets.push_back(target);
            }
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_t10_restoration_4p_bonus_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_t10_restoration_4p_bonus_SpellScript();
    }
};

// 106839 - Skull Bash (MoP, Cat + Bear)
// 80964 / 80965 - legacy form-specific casts still present on some characters
class spell_dru_skull_bash : public SpellScriptLoader
{
public:
    spell_dru_skull_bash() : SpellScriptLoader("spell_dru_skull_bash") { }

    class spell_dru_skull_bash_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_skull_bash_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return sSpellMgr->GetSpellInfo(SPELL_DRUID_SKULL_BASH_MANA_COST)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_SKULL_BASH_INTERRUPT)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_SKULL_BASH_CHARGE);
        }

        void HandleHit()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!caster || !target)
                return;

            caster->CastSpell(target, SPELL_DRUID_SKULL_BASH_CHARGE, true);
            caster->CastSpell(target, SPELL_DRUID_SKULL_BASH_INTERRUPT, true);
            caster->CastSpell(target, SPELL_DRUID_SKULL_BASH_MANA_COST, true);
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_skull_bash_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_skull_bash_SpellScript();
    }
};

// 22568 - Ferocious Bite
class spell_dru_ferocious_bite : public SpellScriptLoader
{
public:
    spell_dru_ferocious_bite() : SpellScriptLoader("spell_dru_ferocious_bite") { }

    class spell_dru_ferocious_bite_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_ferocious_bite_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return sSpellMgr->GetSpellInfo(SPELL_DRUID_RIP)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_GLYPH_OF_FEROCIOUS_BITE)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_GLYPH_OF_FEROCIOUS_BITE_HEAL);
        }

        void HandleHit()
        {
            Player* player = GetCaster()->ToPlayer();
            Unit* target = GetHitUnit();
            if (!player || !target)
                return;

            int32 energyCost = GetSpell()->GetPowerCost();

            if (int32 energyBoost = player->GetPower(POWER_ENERGY))
            {
                energyBoost = std::min(energyBoost, 25);
                SetHitDamage(int32(GetHitDamage() * (1.0f + energyBoost / 25.0f)));
                player->ModifyPower(POWER_ENERGY, -energyBoost);
                energyCost += energyBoost;
            }

            if (AuraEffect const* glyph = player->GetAuraEffect(SPELL_DRUID_GLYPH_OF_FEROCIOUS_BITE, EFFECT_1))
                if (glyph->GetAmount() > 0)
                    player->CastCustomSpell(SPELL_DRUID_GLYPH_OF_FEROCIOUS_BITE_HEAL, SPELLVALUE_BASE_POINT0,
                        energyCost * 2 / glyph->GetAmount(), player, true);

            if (target->GetHealthPct() < 25.0f)
                if (Aura* rip = target->GetAura(SPELL_DRUID_RIP, player->GetGUID()))
                    rip->RefreshDuration();
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_ferocious_bite_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_ferocious_bite_SpellScript();
    }
};

// 33831, 102693, 102703, 102706 - Force of Nature
class spell_dru_force_of_nature : public SpellScriptLoader
{
public:
    enum TreantRole
    {
        TREANT_BALANCE,
        TREANT_RESTORATION,
        TREANT_FERAL,
        TREANT_GUARDIAN
    };

    spell_dru_force_of_nature(char const* scriptName, TreantRole role)
        : SpellScriptLoader(scriptName), _role(role) { }

    class spell_dru_force_of_nature_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_force_of_nature_SpellScript);

    public:
        explicit spell_dru_force_of_nature_SpellScript(TreantRole role)
            : _role(role), _targetGuid(0) { }

        void CaptureTarget()
        {
            if (Unit* target = GetExplTargetUnit())
                _targetGuid = target->GetGUID();
        }

        void HandleCast()
        {
            Unit* caster = GetCaster();
            Unit* target = caster && _targetGuid ? ObjectAccessor::GetUnit(*caster, _targetGuid) : NULL;
            if (!target && caster && caster->GetTypeId() == TypeID::TYPEID_PLAYER)
                target = caster->ToPlayer()->GetSelectedUnit();
            if (!caster || !target)
                return;

            // Resolve the summoned NPC entry from the actual SUMMON effect (not a hard-coded effect index).
            uint32 entry = 0;
            SpellInfo const* spellInfo = GetSpellInfo();
            for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
            {
                if (spellInfo->Effects[i].IsEffect(SPELL_EFFECT_SUMMON))
                {
                    entry = spellInfo->Effects[i].MiscValue;
                    break;
                }
            }
            if (!entry)
                entry = spellInfo->Effects[EFFECT_1].MiscValue;

            std::list<Creature*> minions;
            caster->GetAllMinionsByEntry(minions, entry);
            if (minions.empty())
                caster->GetCreatureListWithEntryInGrid(minions, entry, 100.0f);

            TempSummon* treant = NULL;
            uint32 newestTimer = 0;
            for (Creature* minion : minions)
            {
                TempSummon* summon = minion->ToTempSummon();
                if (!summon || !summon->IsAlive() || summon->GetSummonerGUID() != caster->GetGUID())
                    continue;

                if (!treant || summon->GetTimer() >= newestTimer)
                {
                    treant = summon;
                    newestTimer = summon->GetTimer();
                }
            }

            if (!treant)
                return;

            treant->CastSpell(treant, SPELL_DRUID_FORCE_OF_NATURE_VISUAL, true);
            treant->SetReactState(REACT_AGGRESSIVE);

            switch (_role)
            {
                case TREANT_BALANCE:
                    if (treant->IsAIEnabled)
                        treant->AI()->SetGUID(target->GetGUID());
                    break;
                case TREANT_RESTORATION:
                    treant->CastSpell(target, SPELL_DRUID_FORCE_OF_NATURE_SWIFTMEND, true);
                    break;
                case TREANT_FERAL:
                {
                    float damage = spellInfo->Effects[EFFECT_0].CalcValue(caster) + 1.0f;
                    treant->SetBaseWeaponDamage(WeaponAttackType::BASE_ATTACK, WeaponDamageRange::MINDAMAGE, damage);
                    treant->SetBaseWeaponDamage(WeaponAttackType::BASE_ATTACK, WeaponDamageRange::MAXDAMAGE, damage);
                    treant->UpdateAttackPowerAndDamage();
                    // Tooltip: immediately root the current target, then melee.
                    treant->CastSpell(target, SPELL_DRUID_FORCE_OF_NATURE_ROOT, true);
                    treant->CastSpell(target, SPELL_DRUID_FORCE_OF_NATURE_RAKE, true);
                    treant->Attack(target, true);
                    treant->GetMotionMaster()->Clear(true);
                    treant->GetMotionMaster()->MoveChase(target);
                    if (treant->IsAIEnabled)
                        treant->AI()->SetGUID(target->GetGUID());
                    break;
                }
                case TREANT_GUARDIAN:
                {
                    float damage = spellInfo->Effects[EFFECT_0].CalcValue(caster) * 0.2f + 1.0f;
                    treant->SetBaseWeaponDamage(WeaponAttackType::BASE_ATTACK, WeaponDamageRange::MINDAMAGE, damage);
                    treant->SetBaseWeaponDamage(WeaponAttackType::BASE_ATTACK, WeaponDamageRange::MAXDAMAGE, damage);
                    treant->UpdateAttackPowerAndDamage();
                    treant->CastSpell(target, SPELL_DRUID_FORCE_OF_NATURE_TAUNT, true);
                    treant->Attack(target, true);
                    treant->GetMotionMaster()->Clear(true);
                    treant->GetMotionMaster()->MoveChase(target);
                    if (treant->IsAIEnabled)
                        treant->AI()->SetGUID(target->GetGUID());
                    break;
                }
            }
        }

        void Register() override
        {
            BeforeCast += SpellCastFn(spell_dru_force_of_nature_SpellScript::CaptureTarget);
            AfterCast += SpellCastFn(spell_dru_force_of_nature_SpellScript::HandleCast);
        }

    private:
        TreantRole _role;
        uint64 _targetGuid;
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_force_of_nature_SpellScript(_role);
    }

private:
    TreantRole _role;
};

class npc_force_of_nature_balance : public CreatureScript
{
public:
    npc_force_of_nature_balance() : CreatureScript("npc_force_of_nature_balance") { }

    struct npc_force_of_nature_balanceAI : public ScriptedAI
    {
        explicit npc_force_of_nature_balanceAI(Creature* creature)
            : ScriptedAI(creature), _focusGuid(0), _needRoot(false), _awaitingInitial(true) { }

        void IsSummonedBy(Unit* summoner) OVERRIDE
        {
            me->SetReactState(REACT_AGGRESSIVE);
            if (me->HasUnitTypeMask(UNIT_MASK_MINION))
                static_cast<Minion*>(me)->SetFollowAngle(PET_FOLLOW_ANGLE);

            if (Unit* target = SelectInitialTarget(summoner))
                EngageTarget(target);
        }

        void SetGUID(uint64 guid, int32 /*id*/) OVERRIDE
        {
            if (Unit* target = ObjectAccessor::GetUnit(*me, guid))
                EngageTarget(target);
        }

        void OwnerAttacked(Unit* target) OVERRIDE
        {
            if (_awaitingInitial || !target || !target->IsAlive() || !me->IsValidAttackTarget(target))
                return;

            Unit* focus = _focusGuid ? ObjectAccessor::GetUnit(*me, _focusGuid) : NULL;
            if (focus && focus->IsAlive())
                return;

            EngageTarget(target);
        }

        void EnterEvadeMode() OVERRIDE
        {
            Unit* focus = _focusGuid ? ObjectAccessor::GetUnit(*me, _focusGuid) : NULL;
            if (focus && focus->IsAlive() && me->IsValidAttackTarget(focus))
            {
                AttackStart(focus);
                return;
            }

            if (Unit* owner = me->GetCharmerOrOwner())
                FollowOwner(owner);
            else
                ScriptedAI::EnterEvadeMode();
        }

        void AttackStart(Unit* target) OVERRIDE
        {
            if (!target || !me->Attack(target, false))
                return;

            float castRange = GetSpellRange(SPELL_DRUID_FORCE_OF_NATURE_WRATH);
            if (me->IsWithinDist(target, castRange))
            {
                me->StopMoving();
                me->GetMotionMaster()->Clear(false);
                me->SetFacingToObject(target);
            }
            else
                me->GetMotionMaster()->MoveChase(target, castRange * 0.8f);
        }

        void UpdateAI(uint32 /*diff*/) OVERRIDE
        {
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            Unit* owner = me->GetCharmerOrOwner();
            if (_awaitingInitial)
                return;

            Unit* focus = _focusGuid ? ObjectAccessor::GetUnit(*me, _focusGuid) : NULL;
            if (!focus || !focus->IsAlive())
            {
                _focusGuid = 0;
                _needRoot = false;
                me->AttackStop();
                me->InterruptNonMeleeSpells(false);

                if (Unit* newTarget = SelectCombatTarget(owner))
                {
                    EngageTarget(newTarget);
                    return;
                }

                if (owner)
                    FollowOwner(owner);
                return;
            }

            if (_needRoot)
            {
                me->CastSpell(focus, SPELL_DRUID_FORCE_OF_NATURE_ROOT, true);
                _needRoot = false;
            }

            float castRange = GetSpellRange(SPELL_DRUID_FORCE_OF_NATURE_WRATH);
            if (!me->IsWithinDist(focus, castRange) || !me->IsWithinLOSInMap(focus))
            {
                if (!me->HasUnitState(UNIT_STATE_CHASE))
                    me->GetMotionMaster()->MoveChase(focus, castRange * 0.8f);
                return;
            }

            if (me->isMoving() || me->HasUnitState(UNIT_STATE_CHASE))
            {
                me->StopMoving();
                me->GetMotionMaster()->Clear(false);
            }

            me->SetFacingToObject(focus);
            DoCast(focus, SPELL_DRUID_FORCE_OF_NATURE_WRATH, false);
        }

    private:
        float GetSpellRange(uint32 spellId) const
        {
            if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId))
                return spellInfo->GetMaxRange(false);
            return 40.0f;
        }

        Unit* SelectInitialTarget(Unit* owner) const
        {
            if (!owner)
                return NULL;

            if (Player* player = owner->ToPlayer())
                if (Unit* selected = player->GetSelectedUnit())
                    if (me->IsValidAttackTarget(selected))
                        return selected;

            if (Unit* victim = owner->GetVictim())
                if (me->IsValidAttackTarget(victim))
                    return victim;

            return NULL;
        }

        Unit* SelectCombatTarget(Unit* owner) const
        {
            if (!owner || !owner->IsInCombat())
                return NULL;

            if (Unit* victim = owner->GetVictim())
                if (me->IsValidAttackTarget(victim))
                    return victim;

            if (Player* player = owner->ToPlayer())
                if (Unit* selected = player->GetSelectedUnit())
                    if (me->IsValidAttackTarget(selected))
                        return selected;

            if (Unit* helper = owner->getAttackerForHelper())
                if (me->IsValidAttackTarget(helper))
                    return helper;

            return NULL;
        }

        void EngageTarget(Unit* target)
        {
            _focusGuid = target->GetGUID();
            _awaitingInitial = false;
            _needRoot = true;
            AttackStart(target);
        }

        void FollowOwner(Unit* owner)
        {
            if (!owner)
                return;

            me->GetMotionMaster()->Clear(false);
            me->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);
        }

        uint64 _focusGuid;
        bool _needRoot;
        bool _awaitingInitial;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_force_of_nature_balanceAI(creature);
    }
};

class npc_force_of_nature_restoration : public CreatureScript
{
public:
    npc_force_of_nature_restoration() : CreatureScript("npc_force_of_nature_restoration") { }

    struct npc_force_of_nature_restorationAI : public ScriptedAI
    {
        explicit npc_force_of_nature_restorationAI(Creature* creature)
            : ScriptedAI(creature) { }

        void IsSummonedBy(Unit* summoner) OVERRIDE
        {
            me->SetReactState(REACT_PASSIVE);
            if (me->HasUnitTypeMask(UNIT_MASK_MINION))
                static_cast<Minion*>(me)->SetFollowAngle(PET_FOLLOW_ANGLE);

            FollowOwner(summoner);
        }

        void EnterEvadeMode() OVERRIDE
        {
            me->InterruptNonMeleeSpells(false);
            if (Unit* owner = me->GetCharmerOrOwner())
                FollowOwner(owner);
            else
                ScriptedAI::EnterEvadeMode();
        }

        void UpdateAI(uint32 /*diff*/) OVERRIDE
        {
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            Unit* owner = me->GetCharmerOrOwner();
            if (!owner)
                return;

            float castRange = GetSpellRange(SPELL_DRUID_FORCE_OF_NATURE_HEAL);
            if (!me->IsWithinDist(owner, castRange))
            {
                if (!me->HasUnitState(UNIT_STATE_FOLLOW) && !me->HasUnitState(UNIT_STATE_CHASE))
                    FollowOwner(owner);
                return;
            }

            if (me->isMoving() || me->HasUnitState(UNIT_STATE_CHASE))
            {
                me->StopMoving();
                me->GetMotionMaster()->Clear(false);
            }

            // Healing Touch (113828) selects the most injured nearby friendly target.
            DoCast(me, SPELL_DRUID_FORCE_OF_NATURE_HEAL, false);
        }

    private:
        float GetSpellRange(uint32 spellId) const
        {
            if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId))
                return spellInfo->GetMaxRange(true);
            return 40.0f;
        }

        void FollowOwner(Unit* owner)
        {
            if (!owner)
                return;

            me->GetMotionMaster()->Clear(false);
            me->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_force_of_nature_restorationAI(creature);
    }
};

class npc_force_of_nature_melee : public CreatureScript
{
public:
    enum MeleeRole
    {
        MELEE_FERAL,
        MELEE_GUARDIAN
    };

    npc_force_of_nature_melee(char const* scriptName, MeleeRole role)
        : CreatureScript(scriptName), _role(role) { }

    struct npc_force_of_nature_meleeAI : public ScriptedAI
    {
        explicit npc_force_of_nature_meleeAI(Creature* creature, MeleeRole role)
            : ScriptedAI(creature), _role(role), _focusGuid(0), _hasEngaged(false), _focusFinished(false), _didSpecial(false) { }

        void IsSummonedBy(Unit* summoner) OVERRIDE
        {
            me->SetReactState(REACT_AGGRESSIVE);
            if (me->HasUnitTypeMask(UNIT_MASK_MINION))
                static_cast<Minion*>(me)->SetFollowAngle(PET_FOLLOW_ANGLE);

            // Engage immediately — never MoveFollow before we have a focus (that sends them to the caster).
            if (Unit* target = SelectInitialTarget(summoner))
                EngageTarget(target);
        }

        void SetGUID(uint64 guid, int32 /*id*/) OVERRIDE
        {
            if (_focusFinished)
                return;

            if (Unit* target = ObjectAccessor::GetUnit(*me, guid))
                EngageTarget(target);
        }

        // Player-owned guardians evade when the owner is OOC; PetAI ignores evade. Do the same while focused.
        void EnterEvadeMode() OVERRIDE
        {
            if (_hasEngaged && !_focusFinished)
            {
                if (Unit* focus = ObjectAccessor::GetUnit(*me, _focusGuid))
                {
                    if (focus->IsAlive())
                    {
                        StartAttackAndChase(focus);
                        return;
                    }
                }
            }

            // Not engaged yet, or focus is dead: stay put (do NOT follow owner / "cast position").
            me->AttackStop();
            me->GetMotionMaster()->Clear(false);
            me->GetMotionMaster()->MoveIdle();
        }

        void MoveInLineOfSight(Unit* who) OVERRIDE
        {
            if (_hasEngaged && !_focusFinished)
                return;

            ScriptedAI::MoveInLineOfSight(who);
        }

        void AttackStart(Unit* target) OVERRIDE
        {
            StartAttackAndChase(target);
        }

        void UpdateAI(uint32 /*diff*/) OVERRIDE
        {
            Unit* owner = me->GetCharmerOrOwner();
            if (!_hasEngaged)
            {
                if (Unit* target = SelectInitialTarget(owner))
                    EngageTarget(target);
                return;
            }

            Unit* focus = (!_focusFinished && _focusGuid) ? ObjectAccessor::GetUnit(*me, _focusGuid) : NULL;
            if (!focus || !focus->IsAlive())
            {
                _focusGuid = 0;
                _focusFinished = true;
                me->AttackStop();
                me->InterruptNonMeleeSpells(false);
                if (owner)
                    FollowOwner(owner);
                return;
            }

            StartAttackAndChase(focus);

            if (!me->IsWithinMeleeRange(focus))
                return;

            DoMeleeAttackIfReady();
        }

    private:
        Unit* SelectInitialTarget(Unit* owner) const
        {
            if (!owner)
                return NULL;

            // Prefer the unit the player has targeted (same as the FoN cast target).
            if (uint64 targetGuid = owner->GetUInt64Value(UNIT_FIELD_TARGET))
                if (Unit* hardTarget = ObjectAccessor::GetUnit(*me, targetGuid))
                    if (hardTarget->IsAlive() && !me->IsFriendlyTo(hardTarget))
                        return hardTarget;

            if (Player* player = owner->ToPlayer())
                if (Unit* selected = player->GetSelectedUnit())
                    if (selected->IsAlive() && !me->IsFriendlyTo(selected))
                        return selected;

            if (Unit* victim = owner->GetVictim())
                if (victim->IsAlive() && !me->IsFriendlyTo(victim))
                    return victim;

            return NULL;
        }

        void StartAttackAndChase(Unit* target)
        {
            if (!target)
                return;

            me->SetReactState(REACT_AGGRESSIVE);
            me->ClearUnitState(UNIT_STATE_EVADE);

            if (!me->Attack(target, true))
            {
                me->SetInCombatWith(target);
                target->SetInCombatWith(me);
                me->SetTarget(target->GetGUID());
            }

            // Always (re)apply chase so a prior MoveFollow to the caster cannot stick.
            if (!me->IsWithinMeleeRange(target) || me->GetVictim() != target ||
                me->HasUnitState(UNIT_STATE_FOLLOW) || !me->HasUnitState(UNIT_STATE_CHASE))
            {
                me->GetMotionMaster()->Clear(true);
                me->GetMotionMaster()->MoveChase(target);
            }
        }

        void EngageTarget(Unit* target)
        {
            _focusGuid = target->GetGUID();
            _hasEngaged = true;
            _focusFinished = false;
            StartAttackAndChase(target);

            if (!_didSpecial)
            {
                if (_role == MELEE_FERAL)
                    me->CastSpell(target, SPELL_DRUID_FORCE_OF_NATURE_ROOT, true);
                else
                    me->CastSpell(target, SPELL_DRUID_FORCE_OF_NATURE_TAUNT, true);
                _didSpecial = true;
            }
        }

        void FollowOwner(Unit* owner)
        {
            if (!owner)
                return;

            me->GetMotionMaster()->Clear(false);
            me->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);
            me->SetReactState(REACT_AGGRESSIVE);
        }

        MeleeRole _role;
        uint64 _focusGuid;
        bool _hasEngaged;
        bool _focusFinished;
        bool _didSpecial;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_force_of_nature_meleeAI(creature, _role);
    }

private:
    MeleeRole _role;
};

class DelayedWildMushroomInvisEvent : public BasicEvent
{
public:
    explicit DelayedWildMushroomInvisEvent(uint64 mushroomGuid) : _mushroomGuid(mushroomGuid) { }

    bool Execute(uint64 /*e_time*/, uint32 /*p_time*/) OVERRIDE
    {
        if (Creature* mushroom = HashMapHolder<Creature>::Find(_mushroomGuid))
            if (mushroom->IsAlive())
                mushroom->CastSpell(mushroom, SPELL_DRUID_MUSHROOM_INVISIBLE, true);
        return true;
    }

private:
    uint64 _mushroomGuid;
};

static void GetWildMushrooms(Unit* owner, std::list<Creature*>& mushrooms)
{
    owner->GetAllMinionsByEntry(mushrooms, NPC_DRUID_WILD_MUSHROOM);
}

static Creature* FindNewestWildMushroom(std::list<Creature*> const& mushrooms)
{
    Creature* newest = NULL;
    uint32 newestTimer = 0;
    for (Creature* mushroom : mushrooms)
    {
        if (!mushroom->IsAlive())
            continue;

        // Just-summoned mushrooms have not received birth/invis auras yet.
        if (!mushroom->HasAura(SPELL_DRUID_MUSHROOM_BIRTH)
            && !mushroom->HasAura(SPELL_DRUID_MUSHROOM_INVISIBLE))
            return mushroom;

        TempSummon const* summon = mushroom->ToTempSummon();
        if (!summon)
            continue;

        uint32 timer = summon->GetTimer();
        if (!newest || timer >= newestTimer)
        {
            newest = mushroom;
            newestTimer = timer;
        }
    }
    return newest;
}

static Creature* FindOldestWildMushroom(std::list<Creature*> const& mushrooms, Creature const* exclude)
{
    Creature* oldest = NULL;
    uint32 oldestTimer = std::numeric_limits<uint32>::max();
    for (Creature* mushroom : mushrooms)
    {
        if (mushroom == exclude || !mushroom->IsAlive())
            continue;

        TempSummon const* summon = mushroom->ToTempSummon();
        if (!summon)
            continue;

        uint32 timer = summon->GetTimer();
        if (timer < oldestTimer)
        {
            oldest = mushroom;
            oldestTimer = timer;
        }
    }
    return oldest;
}

// 88747 - Wild Mushroom (Balance)
// 145205 - Wild Mushroom (Restoration)
// 147349 - Wild Mushroom (Restoration with glyph)
class spell_dru_wild_mushroom : public SpellScriptLoader
{
public:
    spell_dru_wild_mushroom() : SpellScriptLoader("spell_dru_wild_mushroom") { }

    class spell_dru_wild_mushroom_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_wild_mushroom_SpellScript);

        int32 _amount0 = 0;
        int32 _amount1 = 0;

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return sSpellMgr->GetSpellInfo(SPELL_DRUID_MUSHROOM_BIRTH)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_MUSHROOM_INVISIBLE)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_MUSHROOM_ACCUMULATOR)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_MUSHROOM_OVERHEAL_AURA);
        }

        void GetBonus()
        {
            Unit* druid = GetCaster();
            if (!druid)
                return;

            std::list<Creature*> mushrooms;
            GetWildMushrooms(druid, mushrooms);
            if (mushrooms.empty())
                return;

            if (!druid->HasAura(SPELL_DRUID_MUSHROOM_OVERHEAL_AURA))
                return;

            if (Aura const* aura = mushrooms.front()->GetAura(SPELL_DRUID_MUSHROOM_ACCUMULATOR))
            {
                if (aura->HasEffect(EFFECT_0))
                    _amount0 = aura->GetEffect(EFFECT_0)->GetAmount();
                if (aura->HasEffect(EFFECT_1))
                    _amount1 = aura->GetEffect(EFFECT_1)->GetAmount();
            }
        }

        void HandleCast()
        {
            Unit* druid = GetCaster();
            if (!druid)
                return;

            uint32 spellId = GetSpellInfo()->Id;
            if (spellId == SPELL_DRUID_WILD_MUSHROOM_RESTO || spellId == SPELL_DRUID_WILD_MUSHROOM_RESTO_GLYPH)
                if (!druid->HasAura(SPELL_DRUID_MUSHROOM_OVERHEAL_AURA))
                    druid->CastSpell(druid, SPELL_DRUID_MUSHROOM_OVERHEAL_AURA, true);

            std::list<Creature*> mushrooms;
            GetWildMushrooms(druid, mushrooms);
            if (mushrooms.empty())
                return;

            Creature* mushroomCreature = FindNewestWildMushroom(mushrooms);
            if (!mushroomCreature)
                return;

            TempSummon* mushroom = mushroomCreature->ToTempSummon();
            if (!mushroom)
                return;

            // Minion InitStats does not copy owner level (unlike totems/guardians).
            mushroom->SetLevel(druid->getLevel());

            // EFFECT_0 is mushroom HP (5), not level.
            int32 health = GetSpellInfo()->Effects[EFFECT_0].BasePoints;
            if (health > 0)
            {
                mushroom->SetMaxHealth(health);
                mushroom->SetHealth(health);
            }

            mushroom->CastSpell(mushroom, SPELL_DRUID_MUSHROOM_BIRTH, true);

            int32 invisSeconds = GetSpellInfo()->Effects[EFFECT_2].BasePoints;
            if (invisSeconds > 0)
                mushroom->m_Events.AddEvent(new DelayedWildMushroomInvisEvent(mushroom->GetGUID()),
                    mushroom->m_Events.CalculateTime(uint32(invisSeconds) * IN_MILLISECONDS));

            if (druid->HasAura(SPELL_DRUID_MUSHROOM_OVERHEAL_AURA))
                druid->CastCustomSpell(mushroom, SPELL_DRUID_MUSHROOM_ACCUMULATOR, &_amount0, &_amount1, NULL, true);

            int32 limit = GetSpellInfo()->Effects[EFFECT_1].BasePoints;
            if (limit <= 0)
                limit = 1;

            while (mushrooms.size() > size_t(limit))
            {
                Creature* oldest = FindOldestWildMushroom(mushrooms, mushroomCreature);
                if (!oldest)
                    break;

                if (TempSummon* old = oldest->ToTempSummon())
                    old->UnSummon();
                mushrooms.remove(oldest);
            }
        }

        void Register() override
        {
            BeforeCast += SpellCastFn(spell_dru_wild_mushroom_SpellScript::GetBonus);
            AfterCast += SpellCastFn(spell_dru_wild_mushroom_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_wild_mushroom_SpellScript();
    }
};

// 138611 - Wild Mushroom (Restoration overheal accumulator driver)
class spell_dru_wild_mushroom_overheal : public SpellScriptLoader
{
public:
    spell_dru_wild_mushroom_overheal() : SpellScriptLoader("spell_dru_wild_mushroom_overheal") { }

    class spell_dru_wild_mushroom_overheal_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_wild_mushroom_overheal_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return sSpellMgr->GetSpellInfo(SPELL_DRUID_MUSHROOM_ACCUMULATOR)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_MUSHROOM_BLOOM_UI_FLASH)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_REJUVENATION);
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            // MoP: only Rejuvenation overhealing feeds the mushroom.
            return spellInfo && spellInfo->Id == SPELL_DRUID_REJUVENATION;
        }

        void HandleProc(ProcEventInfo& eventInfo)
        {
            HealInfo* healInfo = eventInfo.GetHealInfo();
            if (!healInfo)
                return;

            int32 over = int32(healInfo->GetOverheal());
            Player* druid = GetUnitOwner()->ToPlayer();
            if (!over || !druid)
                return;

            if (druid->GetMap()->IsBattlegroundOrArena())
                if (Battleground* bg = druid->GetBattleground())
                    if (bg->GetStatus() == STATUS_WAIT_JOIN)
                        return;

            std::list<Creature*> mushrooms;
            GetWildMushrooms(druid, mushrooms);
            if (mushrooms.empty())
                return;

            Unit* mushroom = mushrooms.front();
            Aura* accumulator = mushroom->GetAura(SPELL_DRUID_MUSHROOM_ACCUMULATOR);
            if (!accumulator)
                return;

            AuraEffect* scale = accumulator->GetEffect(EFFECT_0);
            AuraEffect* bonus = accumulator->GetEffect(EFFECT_1);
            if (!scale || !bonus)
                return;

            // EFFECT_0 of 138611 is "% of overhealing" (normally 100).
            int32 pct = 100;
            if (AuraEffect const* pctEff = GetEffect(EFFECT_0))
                if (pctEff->GetAmount() > 0)
                    pct = pctEff->GetAmount();

            over = CalculatePct(over, pct);
            if (over <= 0)
                return;

            int32 max = int32(druid->GetMaxHealth()) * 2;
            int32 val = std::min(bonus->GetAmount() + over, max);
            if (bonus->GetAmount() != val)
            {
                int32 scaleAmount = int32(val * 100.f / float(max));
                scale->ChangeAmount(scaleAmount);
                bonus->ChangeAmount(val);
                // ChangeAmount does not mark dummy amounts for aura tooltip refresh.
                accumulator->SetNeedClientUpdateForTargets();
                if (val == max && !druid->HasAura(SPELL_DRUID_MUSHROOM_BLOOM_UI_FLASH))
                    druid->CastSpell(druid, SPELL_DRUID_MUSHROOM_BLOOM_UI_FLASH, true);
            }
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_dru_wild_mushroom_overheal_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_dru_wild_mushroom_overheal_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_wild_mushroom_overheal_AuraScript();
    }
};

// 47649 - Wild Mushroom
class npc_wild_mushroom : public CreatureScript
{
public:
    npc_wild_mushroom() : CreatureScript("npc_wild_mushroom") { }

    struct npc_wild_mushroomAI : public ScriptedAI
    {
        npc_wild_mushroomAI(Creature* creature) : ScriptedAI(creature)
        {
            me->SetReactState(REACT_PASSIVE);
        }

        void IsSummonedBy(Unit* /*summoner*/) override
        {
            // Stay planted; owned minions otherwise MoveFollow the owner after evade.
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            me->AddUnitState(UNIT_STATE_ROOT);
            me->GetMotionMaster()->Clear(false);
            me->GetMotionMaster()->MoveIdle();
        }

        void AttackStart(Unit* /*who*/) override { }

        void EnterCombat(Unit* /*who*/) override { }

        void EnterEvadeMode() override
        {
            if (!_EnterEvadeMode())
                return;

            me->GetMotionMaster()->Clear(false);
            me->GetMotionMaster()->MoveIdle();
            Reset();
        }

        void JustDied(Unit* /*killer*/) override
        {
            me->DespawnOrUnsummon(1);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_wild_mushroomAI(creature);
    }
};

// 102791 - Wild Mushroom: Bloom
class spell_dru_wild_mushroom_bloom : public SpellScriptLoader
{
public:
    spell_dru_wild_mushroom_bloom() : SpellScriptLoader("spell_dru_wild_mushroom_bloom") { }

    class spell_dru_wild_mushroom_bloom_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_wild_mushroom_bloom_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return sSpellMgr->GetSpellInfo(SPELL_DRUID_MUSHROOM_HEAL)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_MUSHROOM_DEATH)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_MUSHROOM_SUICIDE);
        }

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            if (!caster)
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;

            std::list<Creature*> mushrooms;
            GetWildMushrooms(caster, mushrooms);
            if (mushrooms.empty())
                return SpellCastResult::SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

            for (Creature* mushroom : mushrooms)
                if (mushroom->IsAlive() && caster->IsWithinDistInMap(mushroom, 40.0f))
                    return SpellCastResult::SPELL_CAST_OK;

            return SpellCastResult::SPELL_FAILED_OUT_OF_RANGE;
        }

        void HandleCast()
        {
            Unit* druid = GetCaster();
            if (!druid)
                return;

            std::list<Creature*> mushrooms;
            GetWildMushrooms(druid, mushrooms);
            for (Creature* mushroom : mushrooms)
            {
                if (!mushroom->IsAlive() || !druid->IsWithinDistInMap(mushroom, 40.0f))
                    continue;

                druid->CastSpell(mushroom->GetPositionX(), mushroom->GetPositionY(), mushroom->GetPositionZ(),
                    SPELL_DRUID_MUSHROOM_HEAL, true);
                mushroom->CastSpell(mushroom, SPELL_DRUID_MUSHROOM_DEATH, true);
                mushroom->DespawnOrUnsummon(500);
            }

            druid->RemoveAurasDueToSpell(SPELL_DRUID_MUSHROOM_BLOOM_UI_FLASH);
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dru_wild_mushroom_bloom_SpellScript::CheckCast);
            OnCast += SpellCastFn(spell_dru_wild_mushroom_bloom_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_wild_mushroom_bloom_SpellScript();
    }
};

// 102792 - Wild Mushroom: Bloom heal
class spell_dru_wild_mushroom_heal : public SpellScriptLoader
{
public:
    spell_dru_wild_mushroom_heal() : SpellScriptLoader("spell_dru_wild_mushroom_heal") { }

    class spell_dru_wild_mushroom_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_wild_mushroom_heal_SpellScript);

        int32 _bonusHeal = 0;

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return sSpellMgr->GetSpellInfo(SPELL_DRUID_MUSHROOM_ACCUMULATOR);
        }

        void SelectTargets(std::list<WorldObject*>& targets)
        {
            WorldLocation const* dest = GetExplTargetDest();
            if (!dest)
            {
                targets.clear();
                return;
            }

            float x = dest->GetPositionX();
            float y = dest->GetPositionY();
            float z = dest->GetPositionZ();

            targets.remove_if([x, y, z](WorldObject* target)
            {
                if (!target->IsWithinLOS(x, y, z))
                    return true;
                return target->GetEntry() == NPC_DRUID_WILD_MUSHROOM;
            });

            _bonusHeal = 0;
            if (targets.empty())
                return;

            Unit* druid = GetCaster();
            if (!druid)
                return;

            std::list<Creature*> mushrooms;
            GetWildMushrooms(druid, mushrooms);
            if (mushrooms.empty())
                return;

            if (AuraEffect const* eff = mushrooms.front()->GetAuraEffect(SPELL_DRUID_MUSHROOM_ACCUMULATOR, EFFECT_1))
                _bonusHeal = eff->GetAmount() / int32(targets.size());
        }

        void CalculateHeal(SpellEffIndex /*effIndex*/)
        {
            if (_bonusHeal)
                SetHitHeal(GetHitHeal() + _bonusHeal);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_wild_mushroom_heal_SpellScript::SelectTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
            OnEffectHitTarget += SpellEffectFn(spell_dru_wild_mushroom_heal_SpellScript::CalculateHeal, EFFECT_0, SPELL_EFFECT_HEAL);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_wild_mushroom_heal_SpellScript();
    }
};

// 88751 - Wild Mushroom: Detonate
class spell_dru_wild_mushroom_detonate : public SpellScriptLoader
{
public:
    spell_dru_wild_mushroom_detonate() : SpellScriptLoader("spell_dru_wild_mushroom_detonate") { }

    class spell_dru_wild_mushroom_detonate_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_wild_mushroom_detonate_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return sSpellMgr->GetSpellInfo(SPELL_DRUID_MUSHROOM_DAMAGE)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_FUNGAL_GROWTH)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_MUSHROOM_DEATH)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_MUSHROOM_SUICIDE);
        }

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            if (!caster)
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;

            std::list<Creature*> mushrooms;
            GetWildMushrooms(caster, mushrooms);
            if (mushrooms.empty())
                return SpellCastResult::SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

            for (Creature* mushroom : mushrooms)
                if (mushroom->IsAlive() && caster->IsWithinDistInMap(mushroom, 40.0f))
                    return SpellCastResult::SPELL_CAST_OK;

            return SpellCastResult::SPELL_FAILED_OUT_OF_RANGE;
        }

        void HandleCast()
        {
            Unit* druid = GetCaster();
            if (!druid)
                return;

            std::list<Creature*> mushrooms;
            GetWildMushrooms(druid, mushrooms);
            for (Creature* mushroom : mushrooms)
            {
                if (!mushroom->IsAlive() || !druid->IsWithinDistInMap(mushroom, 40.0f))
                    continue;

                druid->CastSpell(mushroom, SPELL_DRUID_MUSHROOM_DAMAGE, true);
                druid->CastSpell(mushroom->GetPositionX(), mushroom->GetPositionY(), mushroom->GetPositionZ(),
                    SPELL_DRUID_FUNGAL_GROWTH, true);
                mushroom->CastSpell(mushroom, SPELL_DRUID_MUSHROOM_DEATH, true);
                // Suicide aura (116302) is 500ms and races its first tick; force despawn so
                // the Wild Mushroom model is replaced by Fungal Growth (invisible stalker + ground FX).
                mushroom->DespawnOrUnsummon(500);
            }
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dru_wild_mushroom_detonate_SpellScript::CheckCast);
            OnCast += SpellCastFn(spell_dru_wild_mushroom_detonate_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_wild_mushroom_detonate_SpellScript();
    }
};

// 43484 - Fungal Growth
class npc_fungal_growth : public CreatureScript
{
public:
    npc_fungal_growth() : CreatureScript("npc_fungal_growth") { }

    struct npc_fungal_growthAI : public NullCreatureAI
    {
        npc_fungal_growthAI(Creature* creature) : NullCreatureAI(creature)
        {
            me->SetReactState(REACT_PASSIVE);
            me->CastSpell(me, SPELL_DRUID_MUSHROOM_SLOW_VISUAL, true);
            me->CastSpell(me, SPELL_DRUID_FUNGAL_GROWTH_SLOW, true);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_fungal_growthAI(creature);
    }
};

// 779 - Swipe (Bear), 62078 - Swipe (Cat), 6807 - Maul
// Extra damage vs bleeding targets (MoP: Swipe $s2%, Maul same script).
class spell_dru_swipe_and_maul_bleed : public SpellScriptLoader
{
public:
    spell_dru_swipe_and_maul_bleed() : SpellScriptLoader("spell_dru_swipe_and_maul_bleed") { }

    class spell_dru_swipe_and_maul_bleed_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_swipe_and_maul_bleed_SpellScript);

        void HandleHit()
        {
            Unit* target = GetHitUnit();
            if (!target || !target->HasAuraState(AURA_STATE_BLEEDING))
                return;

            int32 bonusPct = 20;
            // Bear/Cat Swipe store the bleed bonus on EFFECT_1 (dummy).
            if (GetSpellInfo()->Id == 779 || GetSpellInfo()->Id == 62078)
                bonusPct = GetSpellInfo()->Effects[EFFECT_1].CalcValue(GetCaster());

            if (bonusPct > 0)
                SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), bonusPct));
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_swipe_and_maul_bleed_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_swipe_and_maul_bleed_SpellScript();
    }
};

// 102795 - Bear Hug
class spell_dru_bear_hug : public SpellScriptLoader
{
public:
    spell_dru_bear_hug() : SpellScriptLoader("spell_dru_bear_hug") { }

    class spell_dru_bear_hug_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_bear_hug_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (caster->GetShapeshiftForm() != FORM_BEAR)
                caster->CastSpell(caster, SPELL_DRUID_BEAR_FORM, true);

            // EFFECT_1 base points are % of caster max health per tick.
            amount = int32(caster->CountPctFromMaxHealth(amount));
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_bear_hug_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_bear_hug_AuraScript();
    }
};

// 93400 - Shooting Stars
class spell_dru_shooting_stars : public SpellScriptLoader
{
public:
    spell_dru_shooting_stars() : SpellScriptLoader("spell_dru_shooting_stars") { }

    class spell_dru_shooting_stars_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_shooting_stars_SpellScript);

        void HandleHit()
        {
            if (Player* druid = GetCaster()->ToPlayer())
                druid->RemoveSpellCooldown(SPELL_DRUID_STARSURGE, true);
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_shooting_stars_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_shooting_stars_SpellScript();
    }
};

// 78675 - Solar Beam, 113286 - Solar Beam (Symbiosis)
class spell_dru_solar_beam : public SpellScriptLoader
{
public:
    spell_dru_solar_beam() : SpellScriptLoader("spell_dru_solar_beam") { }

    class spell_dru_solar_beam_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_solar_beam_AuraScript);

        uint32 GetSilenceSpell() const
        {
            return GetId() == SPELL_DRUID_SOLAR_BEAM
                ? SPELL_DRUID_SOLAR_BEAM_SILENCE
                : SPELL_DRUID_SOLAR_BEAM_SILENCE_SYMBIOSIS;
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetTarget();
            if (caster && target)
                caster->AddAura(GetSilenceSpell(), target);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetTarget();
            if (caster && target)
                target->RemoveAurasDueToSpell(GetSilenceSpell(), caster->GetGUID());
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_dru_solar_beam_AuraScript::HandleApply, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_solar_beam_AuraScript::HandleRemove, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_solar_beam_AuraScript();
    }
};

// 22842 - Frenzied Regeneration
class spell_dru_frenzied_regeneration : public SpellScriptLoader
{
public:
    spell_dru_frenzied_regeneration() : SpellScriptLoader("spell_dru_frenzied_regeneration") { }

    class spell_dru_frenzied_regeneration_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_frenzied_regeneration_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return sSpellMgr->GetSpellInfo(SPELL_DRUID_GLYPH_OF_FRENZIED_REGEN)
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_FRENZIED_REGEN_HEAL_TAKE);
        }

        void HandleHit()
        {
            Player* druid = GetCaster()->ToPlayer();
            if (!druid)
                return;

            // Item - Druid T16 Guardian 2P Bonus: a free cast worth a fixed amount of rage.
            int32 setBonusRage = 0;
            if (SpellInfo const* triggeredBy = GetTriggeringSpell())
                if (triggeredBy->Id == SPELL_DRUID_T16_GUARDIAN_2P_BONUS)
                    if (AuraEffect const* bonus = druid->GetAuraEffect(SPELL_DRUID_T16_GUARDIAN_2P_BONUS, EFFECT_0))
                        setBonusRage = bonus->GetAmount() * 10;

            // Glyph: fixed rage cost, no rage-to-health convert; +healing taken instead.
            if (Aura const* glyph = druid->GetAura(SPELL_DRUID_GLYPH_OF_FRENZIED_REGEN))
            {
                SetHitHeal(0);
                druid->CastSpell(druid, SPELL_DRUID_FRENZIED_REGEN_HEAL_TAKE, true);
                // Tooltip ${$m3/10}: EFFECT_2 base points are rage*10 (power units).
                int32 rageCost = glyph->GetSpellInfo()->Effects[EFFECT_2].CalcValue(druid);
                if (rageCost > 0 && !setBonusRage)
                {
                    int32 have = druid->GetPower(POWER_RAGE);
                    druid->EnergizeBySpell(druid, GetSpellInfo()->Id, -std::min(rageCost, have), POWER_RAGE);
                }
                return;
            }

            // Convert up to EFFECT_4 rage (tooltip "60") into health. Rage is stored *10.
            int32 maxRage = GetSpellInfo()->Effects[EFFECT_4].CalcValue(druid) * 10;
            if (maxRage <= 0)
                maxRage = 600;

            int32 rageUsed = std::min(maxRage, setBonusRage ? setBonusRage : druid->GetPower(POWER_RAGE));
            int32 attackPower = int32(druid->GetTotalAttackPowerValue(WeaponAttackType::BASE_ATTACK));
            int32 agility = int32(druid->GetStat(STAT_AGILITY));
            int32 stamina = int32(druid->GetStat(STAT_STAMINA));

            // EFFECT_1 = 220% of (AP - 2*Agi); EFFECT_2 = 250% of Stamina; take the larger.
            int32 fromAttackPower = CalculatePct(std::max(attackPower - agility * 2, 0),
                GetSpellInfo()->Effects[EFFECT_1].CalcValue(druid));
            int32 fromStamina = CalculatePct(stamina, GetSpellInfo()->Effects[EFFECT_2].CalcValue(druid));
            int32 healAmount = std::max(fromAttackPower, fromStamina);
            healAmount = int32(int64(rageUsed) * healAmount / maxRage);

            if (healAmount > 0)
            {
                healAmount = int32(druid->SpellHealingBonusDone(druid, GetSpellInfo(), uint32(healAmount), HEAL));
                healAmount = int32(druid->SpellHealingBonusTaken(druid, GetSpellInfo(), uint32(healAmount), HEAL));
            }

            SetHitHeal(healAmount);

            if (rageUsed > 0 && !setBonusRage)
                druid->EnergizeBySpell(druid, GetSpellInfo()->Id, -rageUsed, POWER_RAGE);
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_frenzied_regeneration_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_frenzied_regeneration_SpellScript();
    }
};

// 114107 - Soul of the Forest
// Balance: Wrath / Starfire / Starsurge have a chance to grant Astral Insight.
class spell_dru_soul_of_the_forest : public SpellScriptLoader
{
public:
    spell_dru_soul_of_the_forest() : SpellScriptLoader("spell_dru_soul_of_the_forest") { }

    class spell_dru_soul_of_the_forest_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_soul_of_the_forest_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return sSpellMgr->GetSpellInfo(SPELL_DRUID_ASTRAL_INSIGHT) != NULL;
        }

        bool CheckProc(ProcEventInfo& /*eventInfo*/)
        {
            Player* druid = GetUnitOwner()->ToPlayer();
            return druid && druid->GetTalentSpecialization(druid->GetActiveSpec()) == SPEC_DRUID_BALANCE;
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            if (!roll_chance_i(aurEff->GetAmount()))
                return;

            GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_DRUID_ASTRAL_INSIGHT, true);
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_dru_soul_of_the_forest_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_dru_soul_of_the_forest_AuraScript::HandleProc, EFFECT_2, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_soul_of_the_forest_AuraScript();
    }
};

// 33878 - Mangle (Bear)
// Guardian Soul of the Forest: +30% Rage, +15% damage.
class spell_dru_soul_of_the_forest_mangle : public SpellScriptLoader
{
public:
    spell_dru_soul_of_the_forest_mangle() : SpellScriptLoader("spell_dru_soul_of_the_forest_mangle") { }

    class spell_dru_soul_of_the_forest_mangle_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_soul_of_the_forest_mangle_SpellScript);

        bool Load() override
        {
            Player* caster = GetCaster()->ToPlayer();
            return caster && caster->GetTalentSpecialization(caster->GetActiveSpec()) == SPEC_DRUID_GUARDIAN;
        }

        void HandleRage(SpellEffIndex effIndex)
        {
            AuraEffect const* rageBonus = GetCaster()->GetAuraEffect(SPELL_DRUID_SOUL_OF_THE_FOREST, EFFECT_4);
            if (!rageBonus)
                return;

            int32 rage = GetEffectValue();
            AddPct(rage, rageBonus->GetAmount());
            PreventHitDefaultEffect(effIndex);
            GetCaster()->EnergizeBySpell(GetCaster(), GetSpellInfo()->Id, rage, POWER_RAGE);
        }

        void HandleHit()
        {
            AuraEffect const* damageBonus = GetCaster()->GetAuraEffect(SPELL_DRUID_SOUL_OF_THE_FOREST, EFFECT_5);
            if (!damageBonus)
                return;

            int32 damage = GetHitDamage();
            AddPct(damage, damageBonus->GetAmount());
            SetHitDamage(damage);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_dru_soul_of_the_forest_mangle_SpellScript::HandleRage, EFFECT_2, SPELL_EFFECT_ENERGIZE);
            OnHit += SpellHitFn(spell_dru_soul_of_the_forest_mangle_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_soul_of_the_forest_mangle_SpellScript();
    }
};

// 1079 - Rip, 22568 - Ferocious Bite, 22570 - Maim, 52610 / 127538 - Savage Roar
// Feral Soul of the Forest: finishing moves refund 4 Energy per combo point.
class spell_dru_soul_of_the_forest_feral : public SpellScriptLoader
{
public:
    spell_dru_soul_of_the_forest_feral() : SpellScriptLoader("spell_dru_soul_of_the_forest_feral") { }

    class spell_dru_soul_of_the_forest_feral_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_soul_of_the_forest_feral_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return sSpellMgr->GetSpellInfo(SPELL_DRUID_SOUL_OF_THE_FOREST_ENERGIZE) != NULL;
        }

        bool Load() override
        {
            Player* caster = GetCaster()->ToPlayer();
            return caster && caster->GetTalentSpecialization(caster->GetActiveSpec()) == SPEC_DRUID_FERAL;
        }

        void HandleHit()
        {
            Player* caster = GetCaster()->ToPlayer();
            AuraEffect const* energyPerCp = caster->GetAuraEffect(SPELL_DRUID_SOUL_OF_THE_FOREST, EFFECT_0);
            if (!energyPerCp)
                return;

            int8 comboPoints = caster->GetComboPoints();
            if (comboPoints <= 0)
                return;

            int32 energy = comboPoints * energyPerCp->GetAmount();
            caster->CastCustomSpell(SPELL_DRUID_SOUL_OF_THE_FOREST_ENERGIZE, SPELLVALUE_BASE_POINT0, energy, caster, true);
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_soul_of_the_forest_feral_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_soul_of_the_forest_feral_SpellScript();
    }
};

// 18562 - Swiftmend
// Restoration Soul of the Forest: grant 100% spell haste for the next spell.
class spell_dru_swiftmend : public SpellScriptLoader
{
public:
    spell_dru_swiftmend() : SpellScriptLoader("spell_dru_swiftmend") { }

    class spell_dru_swiftmend_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_swiftmend_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return sSpellMgr->GetSpellInfo(SPELL_DRUID_SOUL_OF_THE_FOREST_HASTE) != NULL;
        }

        void HandleHit()
        {
            Unit* caster = GetCaster();
            if (!caster->HasAura(SPELL_DRUID_GLYPH_OF_EFFLORESCENCE))
                caster->CastSpell(GetHitUnit(), SPELL_DRUID_EFFLORESCENCE, true);

            if (caster->HasAura(SPELL_DRUID_SOUL_OF_THE_FOREST))
                caster->CastSpell(caster, SPELL_DRUID_SOUL_OF_THE_FOREST_HASTE, true);
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_swiftmend_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_swiftmend_SpellScript();
    }
};

// 127663 - Astral Communion
// Periodic eclipse energize; with Astral Insight (Soul of the Forest), grant ±100 instantly and end.
class spell_dru_astral_communion : public SpellScriptLoader
{
public:
    spell_dru_astral_communion() : SpellScriptLoader("spell_dru_astral_communion") { }

    class spell_dru_astral_communion_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_astral_communion_AuraScript);

        bool _hasAstralInsight = false;
        bool _towardLunar = true;

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return sSpellMgr->GetSpellInfo(SPELL_DRUID_ECLIPSE_GENERAL_ENERGIZE) != NULL
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_ASTRAL_INSIGHT) != NULL;
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* druid = GetUnitOwner();
            bool hasLunarMarker = druid->HasAura(SPELL_DRUID_LUNAR_ECLIPSE_MARKER);
            bool hasSolarMarker = druid->HasAura(SPELL_DRUID_SOLAR_ECLIPSE_MARKER);

            // Match Wrath/Starfire direction: lunar marker (or neither) fills lunar (negative).
            _towardLunar = hasLunarMarker || !hasSolarMarker;

            if (druid->HasAura(SPELL_DRUID_ASTRAL_INSIGHT))
            {
                _hasAstralInsight = true;
                HandleTick(NULL);
            }
        }

        void HandleTick(AuraEffect const* /*aurEff*/)
        {
            Unit* druid = GetUnitOwner();
            if (druid->HasAura(SPELL_DRUID_CELESTIAL_ALIGNMENT))
                return;

            int32 eclipse = _hasAstralInsight ? 100 : 25;
            if (_towardLunar)
                eclipse = -eclipse;

            ModifyEclipsePower(druid, eclipse);

            if (_hasAstralInsight)
            {
                _hasAstralInsight = false;
                druid->RemoveAurasDueToSpell(SPELL_DRUID_ASTRAL_INSIGHT);
                SetDuration(1);
            }
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_dru_astral_communion_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_astral_communion_AuraScript::HandleTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_astral_communion_AuraScript();
    }
};

// 145108 - Ysera's Gift
// Every 5 sec: heal self for 5% max health, or the most injured nearby raid ally if at full health.
class spell_dru_yseras_gift : public SpellScriptLoader
{
public:
    spell_dru_yseras_gift() : SpellScriptLoader("spell_dru_yseras_gift") { }

    class spell_dru_yseras_gift_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_yseras_gift_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return sSpellMgr->GetSpellInfo(SPELL_DRUID_YSERAS_GIFT_HEAL_SELF) != NULL
                && sSpellMgr->GetSpellInfo(SPELL_DRUID_YSERAS_GIFT_HEAL_ALLY) != NULL;
        }

        void HandlePeriodic(AuraEffect const* aurEff)
        {
            Unit* target = GetTarget();
            if (!target)
                return;

            int32 healAmount = int32(target->CountPctFromMaxHealth(aurEff->GetAmount()));
            if (healAmount <= 0)
                return;

            if (!target->IsFullHealth())
                target->CastCustomSpell(SPELL_DRUID_YSERAS_GIFT_HEAL_SELF, SPELLVALUE_BASE_POINT0, healAmount, target, true, NULL, aurEff);
            else
                target->CastCustomSpell(SPELL_DRUID_YSERAS_GIFT_HEAL_ALLY, SPELLVALUE_BASE_POINT0, healAmount, target, true, NULL, aurEff);
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_yseras_gift_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_yseras_gift_AuraScript();
    }
};

// 145110 - Ysera's Gift (ally heal): only the most injured raid member in range.
class spell_dru_yseras_gift_heal_ally : public SpellScriptLoader
{
public:
    spell_dru_yseras_gift_heal_ally() : SpellScriptLoader("spell_dru_yseras_gift_heal_ally") { }

    class spell_dru_yseras_gift_heal_ally_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_yseras_gift_heal_ally_SpellScript);

        void SelectTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if([](WorldObject* obj)
            {
                Unit* unit = obj->ToUnit();
                return !unit || !unit->IsAlive() || unit->IsFullHealth();
            });

            if (targets.size() <= 1)
                return;

            targets.sort([](WorldObject* a, WorldObject* b)
            {
                return Skyfire::HealthPctOrderPred()(a->ToUnit(), b->ToUnit());
            });
            targets.resize(1);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_yseras_gift_heal_ally_SpellScript::SelectTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_yseras_gift_heal_ally_SpellScript();
    }
};

// 145518 - Genesis
class spell_dru_genesis : public SpellScriptLoader
{
public:
    spell_dru_genesis() : SpellScriptLoader("spell_dru_genesis") { }

    class spell_dru_genesis_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_genesis_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            Unit* druid = GetCaster();
            targets.remove_if([druid](WorldObject* target)
            {
                Unit* unit = target->ToUnit();
                return !unit || !unit->IsInRaidWith(druid) || !unit->HasAura(SPELL_DRUID_REJUVENATION, druid->GetGUID());
            });
        }

        void HandleHit()
        {
            AuraEffect* rejuvenation = GetHitUnit()->GetAuraEffect(SPELL_DRUID_REJUVENATION, EFFECT_0, GetCaster()->GetGUID());
            if (!rejuvenation)
                return;

            int32 rate = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
            if (rate <= 100)
                return;

            Aura* aura = rejuvenation->GetBase();
            aura->SetDuration(int32(int64(aura->GetDuration()) * 100 / rate));
            rejuvenation->SetAmplitude(std::max(1, int32(int64(rejuvenation->GetAmplitude()) * 100 / rate)));
            rejuvenation->SetPeriodicTimer(std::max(1, int32(int64(rejuvenation->GetPeriodicTimer()) * 100 / rate)));
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_genesis_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
            OnHit += SpellHitFn(spell_dru_genesis_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_genesis_SpellScript();
    }
};

// 102383 - Wild Charge (Moonkin)
class spell_dru_wild_charge_moonkin : public SpellScriptLoader
{
public:
    spell_dru_wild_charge_moonkin() : SpellScriptLoader("spell_dru_wild_charge_moonkin") { }

    class spell_dru_wild_charge_moonkin_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_wild_charge_moonkin_SpellScript);

        SpellCastResult CheckCombat()
        {
            if (!GetCaster()->IsInCombat())
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;
            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_dru_wild_charge_moonkin_SpellScript::CheckCombat);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_wild_charge_moonkin_SpellScript();
    }
};

// 110810 - Soul Swap (Symbiosis)
class spell_dru_soul_swap : public SpellScriptLoader
{
public:
    spell_dru_soul_swap() : SpellScriptLoader("spell_dru_soul_swap") { }

    class spell_dru_soul_swap_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_soul_swap_SpellScript);

        void HandleHit()
        {
            Unit* druid = GetCaster();
            Unit* target = GetHitUnit();
            if (!target)
                return;

            uint64 const druidGuid = druid->GetGUID();

            std::list<Unit*> nearby;
            druid->GetAttackableUnitListInRange(nearby, 40.0f);

            // The most recent target is the one with the freshest Rip.
            Unit* source = NULL;
            int32 sourceDuration = 0;
            for (Unit* unit : nearby)
            {
                if (unit == target || unit == druid || !druid->IsValidAttackTarget(unit))
                    continue;

                Aura* rip = unit->GetAura(SPELL_DRUID_RIP, druidGuid);
                if (!rip || !unit->HasAura(SPELL_DRUID_RAKE, druidGuid))
                    continue;

                if (!source || rip->GetDuration() > sourceDuration)
                {
                    source = unit;
                    sourceDuration = rip->GetDuration();
                }
            }

            if (!source)
                return;

            MoveDot(source, target, SPELL_DRUID_RIP, EFFECT_0);
            MoveDot(source, target, SPELL_DRUID_RAKE, EFFECT_1);
        }

        void MoveDot(Unit* source, Unit* target, uint32 spellId, uint8 effIndex)
        {
            Unit* druid = GetCaster();
            Aura* old = source->GetAura(spellId, druid->GetGUID());
            if (!old)
                return;

            int32 duration = old->GetDuration();
            int32 maxDuration = old->GetMaxDuration();
            AuraEffect const* oldEffect = old->GetEffect(effIndex);
            int32 amount = oldEffect ? oldEffect->GetAmount() : 0;

            source->RemoveAura(spellId, druid->GetGUID());

            if (Aura* moved = druid->AddAura(spellId, target))
            {
                moved->SetMaxDuration(maxDuration);
                moved->SetDuration(duration);
                if (AuraEffect* effect = moved->GetEffect(effIndex))
                    effect->SetAmount(amount);
            }
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_dru_soul_swap_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_soul_swap_SpellScript();
    }
};

// 8921 - Moonfire, 93402 - Sunfire
class spell_dru_shooting_stars_trigger : public SpellScriptLoader
{
public:
    spell_dru_shooting_stars_trigger() : SpellScriptLoader("spell_dru_shooting_stars_trigger") { }

    class spell_dru_shooting_stars_trigger_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_shooting_stars_trigger_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* druid = GetCaster())
                druid->BindAura(GetAura());
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_dru_shooting_stars_trigger_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_shooting_stars_trigger_AuraScript();
    }
};

// 93399 - Shooting Stars
// The chance per tick is divided by sqrt(number of targets with the druid's Moonfire / Sunfire).
class spell_dru_shooting_stars_proc : public SpellScriptLoader
{
public:
    spell_dru_shooting_stars_proc() : SpellScriptLoader("spell_dru_shooting_stars_proc") { }

    class spell_dru_shooting_stars_proc_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_shooting_stars_proc_AuraScript);

        bool Load() override
        {
            return GetOwner()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        static void AddTargets(Unit::AuraList const* auras, std::unordered_set<Unit*>& targets)
        {
            if (!auras)
                return;
            for (Aura* aura : *auras)
                if (Unit* owner = aura->GetUnitOwner())
                    targets.insert(owner);
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            Player* druid = GetUnitOwner()->ToPlayer();
            if (!druid)
                return false;

            std::unordered_set<Unit*> targets;
            AddTargets(druid->GetBoundAurasBySpellId(SPELL_DRUID_MOONFIRE), targets);
            AddTargets(druid->GetBoundAurasBySpellId(SPELL_DRUID_SUNFIRE), targets);

            // The critical tick can be the last one of the only Moonfire / Sunfire: the aura is unbound by then.
            if (targets.empty())
            {
                SpellInfo const* procSpell = eventInfo.GetSpellInfo();
                if (!procSpell || (procSpell->Id != SPELL_DRUID_MOONFIRE && procSpell->Id != SPELL_DRUID_SUNFIRE))
                    return false;
            }

            float targetCount = float(std::max<size_t>(1, targets.size()));
            float chance = float(GetSpellInfo()->ProcChance);
            druid->ApplySpellMod(GetId(), SPELLMOD_CHANCE_OF_SUCCESS, chance);
            return roll_chance_f(chance / std::sqrt(targetCount));
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_dru_shooting_stars_proc_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_shooting_stars_proc_AuraScript();
    }
};

// 150017 - Rake (Force of Nature)
class spell_dru_force_of_nature_rake : public SpellScriptLoader
{
public:
    spell_dru_force_of_nature_rake() : SpellScriptLoader("spell_dru_force_of_nature_rake") { }

    class spell_dru_force_of_nature_rake_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_force_of_nature_rake_AuraScript);

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
                return;

            if (Unit* caster = GetCaster())
                if (TempSummon* treant = caster->ToTempSummon())
                    treant->UnSummon();
        }

        void Register() override
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_force_of_nature_rake_AuraScript::HandleRemove, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_force_of_nature_rake_AuraScript();
    }
};

// 783 - Travel Form
class spell_dru_travel_form : public SpellScriptLoader
{
public:
    spell_dru_travel_form() : SpellScriptLoader("spell_dru_travel_form") { }

    class spell_dru_travel_form_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_travel_form_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* druid = GetTarget();
            // Stag Form must be applied after the shapeshift has finished.
            druid->m_Events.Schedule(1, [druid]()
            {
                if (!druid->HasAura(SPELL_DRUID_TRAVEL_FORM) || druid->HasAura(SPELL_DRUID_GLYPH_OF_THE_CHEETAH))
                    return;

                if (druid->HasAura(SPELL_DRUID_GLYPH_OF_THE_STAG))
                    druid->CastSpell(druid, SPELL_DRUID_STAG_FORM, true);
                druid->CastSpell(druid, SPELL_DRUID_SOUND_EFFECT_STAG_FORM, true);
            });
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->RemoveAurasDueToSpell(SPELL_DRUID_STAG_FORM);
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_dru_travel_form_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_travel_form_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_travel_form_AuraScript();
    }
};

// 22812 - Barkskin
class spell_dru_t16_guardian_2p_bonus : public SpellScriptLoader
{
public:
    spell_dru_t16_guardian_2p_bonus() : SpellScriptLoader("spell_dru_t16_guardian_2p_bonus") { }

    class spell_dru_t16_guardian_2p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_t16_guardian_2p_bonus_AuraScript);

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* druid = GetTarget();
            AuraEffect const* bonus = druid->GetAuraEffect(SPELL_DRUID_T16_GUARDIAN_2P_BONUS, EFFECT_0);
            if (!bonus)
                return;

            if (Aura* savageDefense = druid->GetAura(SPELL_DRUID_SAVAGE_DEFENSE_AURA))
            {
                savageDefense->SetDuration(savageDefense->GetDuration() + 3 * IN_MILLISECONDS);
                if (savageDefense->GetDuration() > savageDefense->GetMaxDuration())
                    savageDefense->SetMaxDuration(savageDefense->GetDuration());
            }
            else
            {
                druid->CastSpell(druid, SPELL_DRUID_SAVAGE_DEFENSE_AURA, true);
                if (Aura* applied = druid->GetAura(SPELL_DRUID_SAVAGE_DEFENSE_AURA))
                {
                    applied->SetMaxDuration(3 * IN_MILLISECONDS);
                    applied->SetDuration(3 * IN_MILLISECONDS);
                }
            }

            druid->CastSpell(druid, SPELL_DRUID_FRENZIED_REGENERATION, true, NULL, bonus);
        }

        void Register() override
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_t16_guardian_2p_bonus_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_REDUCE_PUSHBACK, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_t16_guardian_2p_bonus_AuraScript();
    }
};

// 774 - Rejuvenation
class spell_dru_glyph_of_rejuvenation : public SpellScriptLoader
{
public:
    spell_dru_glyph_of_rejuvenation() : SpellScriptLoader("spell_dru_glyph_of_rejuvenation") { }

    class spell_dru_glyph_of_rejuvenation_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_glyph_of_rejuvenation_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster || !caster->HasAura(SPELL_DRUID_GLYPH_OF_REJUVENATION))
                return;

            caster->BindAura(GetAura());
            if (Unit::AuraList const* auras = caster->GetBoundAurasBySpellId(GetId()))
                if (auras->size() >= 3)
                    caster->CastSpell(caster, SPELL_DRUID_GLYPH_OF_REJUVENATION_EFFECT, true);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            Unit::AuraList const* auras = caster->GetBoundAurasBySpellId(GetId());
            if (!auras || auras->size() < 3)
                caster->RemoveAurasDueToSpell(SPELL_DRUID_GLYPH_OF_REJUVENATION_EFFECT);
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_dru_glyph_of_rejuvenation_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_glyph_of_rejuvenation_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_glyph_of_rejuvenation_AuraScript();
    }
};

// 5225 - Track Humanoids, 114280 - Glyph of the Predator
class spell_dru_glyph_of_the_predator : public SpellScriptLoader
{
public:
    spell_dru_glyph_of_the_predator() : SpellScriptLoader("spell_dru_glyph_of_the_predator") { }

    class spell_dru_glyph_of_the_predator_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_glyph_of_the_predator_AuraScript);

        bool Load() override
        {
            return GetOwner()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            uint32 otherSpell = GetId() == SPELL_DRUID_GLYPH_OF_THE_PREDATOR ? SPELL_DRUID_TRACK_HUMANOIDS : SPELL_DRUID_GLYPH_OF_THE_PREDATOR;
            if (GetTarget()->HasAura(otherSpell))
                GetTarget()->SetFlag(PLAYER_FIELD_TRACK_CREATURE_MASK, uint32(1) << (CREATURE_TYPE_BEAST - 1));
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->RemoveFlag(PLAYER_FIELD_TRACK_CREATURE_MASK, uint32(1) << (CREATURE_TYPE_BEAST - 1));
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_dru_glyph_of_the_predator_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_dru_glyph_of_the_predator_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_glyph_of_the_predator_AuraScript();
    }
};

// 107059 - Glyph of the Chameleon
class spell_dru_glyph_of_the_chameleon_fix : public SpellScriptLoader
{
public:
    spell_dru_glyph_of_the_chameleon_fix() : SpellScriptLoader("spell_dru_glyph_of_the_chameleon_fix") { }

    class spell_dru_glyph_of_the_chameleon_fix_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_glyph_of_the_chameleon_fix_AuraScript);

    public:
        uint32 LastModelId = 0;

    private:
        // The glyph's spell modifier has no use for the player.
        void CalculateSpellMod(AuraEffect const* /*aurEff*/, SpellModifier*& spellMod)
        {
            delete spellMod;
            spellMod = NULL;
        }

        void Register() override
        {
            DoEffectCalcSpellMod += AuraEffectCalcSpellModFn(spell_dru_glyph_of_the_chameleon_fix_AuraScript::CalculateSpellMod, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_glyph_of_the_chameleon_fix_AuraScript();
    }
};

// 768 - Cat Form, 5487 - Bear Form
class spell_dru_glyph_of_the_chameleon : public SpellScriptLoader
{
public:
    spell_dru_glyph_of_the_chameleon() : SpellScriptLoader("spell_dru_glyph_of_the_chameleon") { }

    class spell_dru_glyph_of_the_chameleon_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_glyph_of_the_chameleon_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* druid = GetTarget();
            Aura* glyph = druid->GetAura(SPELL_DRUID_GLYPH_OF_THE_CHAMELEON);
            if (!glyph)
                return;

            typedef spell_dru_glyph_of_the_chameleon_fix::spell_dru_glyph_of_the_chameleon_fix_AuraScript GlyphScript;
            GlyphScript* script = dynamic_cast<GlyphScript*>(glyph->GetScriptByName("spell_dru_glyph_of_the_chameleon_fix"));
            if (!script)
                return;

            static std::map<uint32, std::set<uint32>> const catModels =
            {
                { RACE_NIGHTELF, { 29405, 29406, 29407, 29408, 892   } },
                { RACE_TROLL,    { 33668, 33667, 33666, 33665, 33669 } },
                { RACE_WORGEN,   { 33662, 33661, 33664, 33663, 33660 } },
                { RACE_TAUREN,   { 29409, 29410, 29411, 29412, 8571  } },
            };
            static std::map<uint32, std::set<uint32>> const bearModels =
            {
                { RACE_NIGHTELF, { 29413, 29414, 29416, 29415, 2281  } },
                { RACE_TROLL,    { 33657, 33659, 33656, 33658, 33655 } },
                { RACE_WORGEN,   { 33652, 33651, 33654, 33653, 33650 } },
                { RACE_TAUREN,   { 29418, 29419, 29420, 29421, 2289  } },
            };

            std::map<uint32, std::set<uint32>> const& raceModels = druid->GetShapeshiftForm() == FORM_CAT ? catModels : bearModels;
            std::map<uint32, std::set<uint32>>::const_iterator itr = raceModels.find(druid->getRace());
            if (itr == raceModels.end())
                return;

            std::set<uint32> models = itr->second;
            models.erase(script->LastModelId);
            uint32 modelId = Skyfire::Containers::SelectRandomContainerElement(models);
            druid->SetDisplayId(modelId);
            script->LastModelId = modelId;
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_dru_glyph_of_the_chameleon_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_glyph_of_the_chameleon_AuraScript();
    }
};

void AddSC_druid_spell_scripts()
{
    new spell_dru_dash();
    new spell_dru_bear_hug();
    new spell_dru_cenarion_ward();
    new spell_dru_incarnation_chosen_of_elune();
    new spell_dru_incarnation_skins();
    new spell_dru_incarnation_tree_aura();
    new spell_dru_tree_of_life();
    new spell_dru_ursols_vortex_dynamic();
    new spell_dru_eclipse("spell_dru_eclipse_lunar");
    new spell_dru_eclipse("spell_dru_eclipse_solar");
    new spell_dru_eclipse_energize();
    new spell_dru_ferocious_bite();
    new spell_dru_force_of_nature("spell_dru_force_of_nature_balance", spell_dru_force_of_nature::TREANT_BALANCE);
    new spell_dru_force_of_nature("spell_dru_force_of_nature_restoration", spell_dru_force_of_nature::TREANT_RESTORATION);
    new spell_dru_force_of_nature("spell_dru_force_of_nature_feral", spell_dru_force_of_nature::TREANT_FERAL);
    new spell_dru_force_of_nature("spell_dru_force_of_nature_guardian", spell_dru_force_of_nature::TREANT_GUARDIAN);
    new spell_dru_frenzied_regeneration();
    new spell_dru_glyph_of_innervate();
    new spell_dru_innervate();
    new spell_dru_lacerate();
    new spell_dru_thrash_bear();
    new spell_dru_faerie_fire();
    new spell_dru_faerie_swarm();
    new spell_dru_lifebloom();
    new spell_dru_lifebloom_refresh();
    new spell_dru_rejuvenation();
    new spell_dru_living_seed();
    new spell_dru_living_seed_proc();
    new spell_dru_might_of_ursoc();
    new spell_dru_predatory_strikes();
    new spell_dru_rake();
    new spell_dru_savage_defense();
    new spell_dru_savage_roar();
    new spell_dru_shooting_stars();
    new spell_dru_skull_bash();
    new spell_dru_solar_beam();
    new spell_dru_soul_of_the_forest();
    new spell_dru_soul_of_the_forest_feral();
    new spell_dru_soul_of_the_forest_mangle();
    new spell_dru_starfall_dummy();
    new spell_dru_survival_instincts();
    new spell_dru_swift_flight_passive();
    new spell_dru_swiftmend();
    new spell_dru_swipe_and_maul_bleed();
    new spell_dru_t10_restoration_4p_bonus();
    new spell_dru_astral_communion();
    new spell_dru_wild_mushroom();
    new spell_dru_wild_mushroom_bloom();
    new spell_dru_wild_mushroom_detonate();
    new spell_dru_wild_mushroom_heal();
    new spell_dru_wild_mushroom_overheal();
    new spell_dru_yseras_gift();
    new spell_dru_yseras_gift_heal_ally();
    new spell_dru_eclipse_remove();
    new spell_dru_celestial_alignment();
    new spell_dru_moonfire();
    new spell_dru_glyph_of_omens();
    new spell_dru_sudden_eclipse();
    new spell_dru_glyph_of_hurricane();
    new spell_dru_t16_balance_2p_bonus();
    new spell_dru_savage_roar_duration_check();
    new spell_dru_savage_roar_glyphed();
    new spell_dru_rip_duration();
    new spell_dru_shred();
    new spell_dru_tigers_fury();
    new spell_dru_glyph_of_shred();
    new spell_dru_primal_fury();
    new spell_dru_swipe();
    new spell_dru_item_pvp_feral_4p();
    new spell_dru_stampede_effect();
    new spell_dru_stampede_timer();
    new spell_dru_t16_feral_2p_bonus();
    new spell_dru_t16_feral_4p_bonus();
    new spell_dru_feral_rage();
    new spell_dru_healing_touch_dream_of_cenarius();
    new spell_dru_wrath_dream_of_cenarius();
    new spell_dru_dream_of_cenarius();
    new spell_dru_tooth_and_claw_absorb();
    new spell_dru_tooth_and_claw_trigger();
    new spell_dru_berserk_bear();
    new spell_dru_bear_form();
    new spell_dru_bear_form_passive();
    new spell_dru_glyph_of_fae_silence_cooldown();
    new spell_dru_t15_guardian_4p_bonus();
    new spell_dru_glyph_of_fae_silence();
    new spell_dru_growl();
    new spell_dru_efflorescence();
    new spell_dru_efflorescence_heal();
    new spell_dru_wild_growth();
    new spell_dru_harmony();
    new spell_dru_glyph_of_regrowth();
    new spell_dru_glyph_of_healing_touch();
    new spell_dru_sage_mender();
    new spell_dru_t15_restoration_4p_bonus();
    new spell_dru_heart_of_the_wild();
    new spell_dru_heart_of_the_wild_bonus();
    new spell_dru_killer_instinct();
    new spell_dru_killer_instinct_form();
    new spell_dru_natures_vigil();
    new spell_dru_natures_vigil_damage();
    new spell_dru_natures_vigil_heal();
    new spell_dru_symbiosis_caster_aura();
    new spell_dru_symbiosis_target_check();
    new spell_dru_symbiosis();
    new spell_dru_symbiosis_fortifying_brew();
    new spell_dru_play_death();
    new spell_dru_life_tap();
    new spell_dru_demonic_circle_teleport();
    new spell_dru_shattering_blow();
    new spell_dru_intimidating_roar();
    new spell_dru_glyph_of_the_treant();
    new spell_dru_prowl();
    new spell_dru_stampeding_roar();
    new spell_dru_teleport_moonglade();
    new spell_dru_activate_cat_form();
    new spell_dru_genesis();
    new spell_dru_wild_charge_moonkin();
    new spell_dru_soul_swap();
    new spell_dru_shooting_stars_trigger();
    new spell_dru_shooting_stars_proc();
    new spell_dru_force_of_nature_rake();
    new spell_dru_travel_form();
    new spell_dru_t16_guardian_2p_bonus();
    new spell_dru_glyph_of_rejuvenation();
    new spell_dru_glyph_of_the_predator();
    new spell_dru_glyph_of_the_chameleon_fix();
    new spell_dru_glyph_of_the_chameleon();
    new npc_force_of_nature_balance();
    new npc_force_of_nature_restoration();
    new npc_force_of_nature_melee("npc_force_of_nature_feral", npc_force_of_nature_melee::MELEE_FERAL);
    new npc_force_of_nature_melee("npc_force_of_nature_guardian", npc_force_of_nature_melee::MELEE_GUARDIAN);
    new npc_fungal_growth();
    new npc_wild_mushroom();
}
