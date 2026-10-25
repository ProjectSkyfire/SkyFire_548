/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/*
 * Scripts for spells with SPELLFAMILY_SHAMAN and SPELLFAMILY_GENERIC spells used by shaman players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_sha_".
 */

#include "Player.h"
#include "ScriptMgr.h"
#include "GridNotifiers.h"
#include "ObjectAccessor.h"
#include "Unit.h"
#include "DynamicObject.h"
#include "Pet.h"
#include "Spell.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "TemporarySummon.h"

enum ShamanSpells
{
    SPELL_HUNTER_INSANITY                       = 95809,
    SPELL_MAGE_TEMPORAL_DISPLACEMENT            = 80354,
    SPELL_SHAMAN_ANCESTRAL_AWAKENING            = 52759,
    SPELL_SHAMAN_ANCESTRAL_AWAKENING_PROC       = 52752,
    SPELL_SHAMAN_ANCESTRAL_GUIDANCE_HEAL        = 114911,
    SPELL_SHAMAN_ANCESTRAL_VIGOR                = 105284,
    SPELL_SHAMAN_CHAIN_HEAL                     = 1064,
    SPELL_SHAMAN_CLEANSING_WATERS               = 86961,

    SPELL_SHAMAN_EARTH_ELEMENTAL_SUMMON         = 117753,
    SPELL_SHAMAN_EARTH_ELEMENTAL_TOTEM          = 2062,
    SPELL_SHAMAN_EARTH_SHIELD                   = 974,
    SPELL_SHAMAN_EARTH_SHIELD_HEAL              = 379,
    SPELL_SHAMAN_EARTHBIND_FOR_EARTHGRAB        = 116947,
    SPELL_SHAMAN_EARTHGRAB_IMMUNITY             = 116946,
    SPELL_SHAMAN_EARTHQUAKE                     = 61882,
    SPELL_SHAMAN_EARTHQUAKE_KNOCK_DOWN          = 77505,
    SPELL_SHAMAN_EARTHQUAKE_TICK                = 77478,
    SPELL_SHAMAN_ELEMENTAL_BLAST                = 117014,
    SPELL_SHAMAN_ELEMENTAL_BLAST_FROST_VISUAL   = 118515,
    SPELL_SHAMAN_ELEMENTAL_BLAST_NATURE_VISUAL  = 118517,
    SPELL_SHAMAN_ELEMENTAL_BLAST_RATING_BONUS   = 118522,
    SPELL_SHAMAN_ELEMENTAL_DISCHARGE            = 144999,
    SPELL_SHAMAN_ELEMENTAL_FURY                 = 60188,
    SPELL_SHAMAN_ELEMENTAL_TOTEM                = 34967,

    SPELL_SHAMAN_EXHAUSTION                     = 57723,
    SPELL_SHAMAN_FERAL_SPIRIT                   = 51533,
    SPELL_SHAMAN_FIRE_ELEMENTAL_SUMMON          = 117663,
    SPELL_SHAMAN_FIRE_ELEMENTAL_TOTEM           = 2894,
    SPELL_SHAMAN_FIRE_NOVA_TRIGGERED_R1         = 8349,
    SPELL_SHAMAN_FLAME_SHOCK                    = 8050,
    SPELL_SHAMAN_FLAMES_OF_LIFE                 = 137808,
    SPELL_SHAMAN_FULMINATION                    = 88766,
    SPELL_SHAMAN_FULMINATION_INFO               = 95774,
    SPELL_SHAMAN_FULMINATION_TRIGGERED          = 88767,
    SPELL_SHAMAN_GHOST_WOLF                     = 2645,

    SPELL_SHAMAN_GLYPH_OF_CLEANSING_WATERS      = 55445,
    SPELL_SHAMAN_GLYPH_OF_HEALING_STREAM        = 119523,
    SPELL_SHAMAN_GLYPH_OF_HEALING_STREAM_TOTEM  = 55456,
    SPELL_SHAMAN_GLYPH_OF_HEALING_WAVE          = 55533,
    SPELL_SHAMAN_GLYPH_OF_LAKESTRIDER           = 55448,
    SPELL_SHAMAN_GLYPH_OF_LAVA_LASH             = 55444,
    SPELL_SHAMAN_GLYPH_OF_LIGHTNING_SHIELD      = 101052,
    SPELL_SHAMAN_GLYPH_OF_LIGHTNING_SHIELD_MOD  = 142912,
    SPELL_SHAMAN_GLYPH_OF_PURGING               = 147762,
    SPELL_SHAMAN_GLYPH_OF_SHAMANISTIC_RAGE      = 63280,

    SPELL_SHAMAN_GLYPH_OF_THUNDERSTORM          = 62132,
    SPELL_SHAMAN_GROUNDING_TOTEM_EFFECT         = 8178,
    SPELL_SHAMAN_GROUNDING_TOTEM_REFLECT        = 89523,
    SPELL_SHAMAN_HEALING_RAIN                   = 73920,
    SPELL_SHAMAN_HEALING_RAIN_TICK              = 73921,
    SPELL_SHAMAN_HEALING_STREAM_TOTEM_BONUS     = 138304,
    SPELL_SHAMAN_ITEM_T14_ELEMENTAL_4P          = 123124,
    SPELL_SHAMAN_ITEM_T14_RESTORATION_4P        = 123135,
    SPELL_SHAMAN_ITEM_T15_ELEMENTAL_4P          = 138144,
    SPELL_SHAMAN_ITEM_T15_ENHANCEMENT_2P        = 138136,
    SPELL_SHAMAN_ITEM_T15_ENHANCEMENT_4P        = 138141,
    SPELL_SHAMAN_ITEM_T15_RESTORATION_2P        = 138303,
    SPELL_SHAMAN_ITEM_T15_RESTORATION_4P        = 138305,
    SPELL_SHAMAN_ITEM_T16_RESTORATION_2P        = 145378,
    SPELL_SHAMAN_LAVA_BURST                     = 51505,
    SPELL_SHAMAN_LAVA_LASH                      = 60103,
    SPELL_SHAMAN_LAVA_LASH_SPREAD_FLAME_SHOCK   = 105792,
    SPELL_SHAMAN_LAVA_SURGE                     = 77762,
    SPELL_SHAMAN_ITEM_LIGHTNING_SHIELD          = 23552,
    SPELL_SHAMAN_ITEM_LIGHTNING_SHIELD_DAMAGE   = 27635,
    SPELL_SHAMAN_ITEM_MANA_SURGE                = 23571,
    SPELL_SHAMAN_LIGHTNING_BOLT                 = 403,
    SPELL_SHAMAN_LIGHTNING_SHIELD               = 324,
    SPELL_SHAMAN_LIGHTNING_SHIELD_DAMAGE        = 26364,
    SPELL_SHAMAN_MAELSTROM_WEAPON               = 53817,
    SPELL_SHAMAN_MAELSTROM_WEAPON_FULL_STACKS   = 60349,
    SPELL_SHAMAN_MASTERY_ENHANCED_ELEMENTS      = 77223,
    SPELL_SHAMAN_NATURE_GUARDIAN                = 31616,
    SPELL_SHAMAN_NATURES_BARRIER                = 145379,
    SPELL_SHAMAN_PRIMAL_EARTH_ELEMENTAL_SUMMON  = 118323,
    SPELL_SHAMAN_PRIMAL_ELEMENTALIST            = 117013,
    SPELL_SHAMAN_PRIMAL_FIRE_ELEMENTAL_SUMMON   = 118291,
    SPELL_SHAMAN_RESTORATIVE_MISTS              = 114083,
    SPELL_SHAMAN_RESURGENCE                     = 101033,
    SPELL_SHAMAN_RUSHING_STREAMS                = 147074,
    SPELL_SHAMAN_SATED                          = 57724,
    SPELL_SHAMAN_SEARING_FLAMES                 = 77661,
    SPELL_SHAMAN_SPIRIT_LINK                    = 98021,
    SPELL_SHAMAN_SPIRITWALKERS_AEGIS            = 131557,
    SPELL_SHAMAN_SPIRITWALKERS_AEGIS_EFFECT     = 131558,
    SPELL_SHAMAN_STONE_BULWARK_ABSORB           = 114893,
    SPELL_SHAMAN_STONE_BULWARK_TOTEM_PASSIVE    = 114889,
    SPELL_SHAMAN_STORMBLAST                     = 115356,
    SPELL_SHAMAN_STORMLASH                      = 120687,
    SPELL_SHAMAN_STORMSTRIKE                    = 17364,
    SPELL_SHAMAN_TELLURIC_CURRENTS              = 82987,
    SPELL_SHAMAN_TOTEM_HEALING_STREAM_HEAL      = 52042,
    SPELL_SHAMAN_TOTEMIC_ENCIRCLEMENT_AIR       = 120219,
    SPELL_SHAMAN_TOTEMIC_ENCIRCLEMENT_EARTH     = 120218,
    SPELL_SHAMAN_TOTEMIC_ENCIRCLEMENT_FIRE      = 120217,
    SPELL_SHAMAN_TOTEMIC_ENCIRCLEMENT_WATER     = 120214,
    SPELL_SHAMAN_TIDAL_WAVES                    = 53390,
    SPELL_SHAMAN_UNLEASH_EARTH                  = 73684,
    SPELL_SHAMAN_UNLEASH_FLAME                  = 73683,
    SPELL_SHAMAN_UNLEASH_FROST                  = 73682,
    SPELL_SHAMAN_UNLEASH_LIFE                   = 73685,
    SPELL_SHAMAN_UNLEASH_WIND                   = 73681,
    SPELL_SHAMAN_UNLEASHED_FURY_EARTHLIVING     = 118473,
    SPELL_SHAMAN_UNLEASHED_FURY_FLAMETONGUE     = 118470,
    SPELL_SHAMAN_UNLEASHED_FURY_FROSTBRAND      = 118474,
    SPELL_SHAMAN_UNLEASHED_FURY_ROCKBITER       = 118475,
    SPELL_SHAMAN_UNLEASHED_FURY_TALENT          = 117012,
    SPELL_SHAMAN_UNLEASHED_FURY_WINDFURY        = 118472,
    SPELL_SHAMAN_WATER_SHIELD                   = 52127,
    SPELL_SHAMAN_WATER_WALKING                  = 546,
    SPELL_SHAMAN_WINDFURY_ATTACK_MAINHAND       = 25504,
    SPELL_SHAMAN_WINDFURY_ATTACK_OFFHAND        = 33750,
    SPELL_SHAMAN_WINDFURY_WEAPON                = 8232,
    SPELL_SHAMAN_ASCENDANCE                     = 114049,
    SPELL_SHAMAN_ASCENDANCE_ELEMENTAL           = 114050,
    SPELL_SHAMAN_ASCENDANCE_ENHANCEMENT         = 114051,
    SPELL_SHAMAN_ASCENDANCE_RESTORATION         = 114052,
    SPELL_ROGUE_SINISTER_STRIKE                 = 1752,
    SPELL_WARLOCK_DRAIN_SOUL                    = 1120
};

enum ShamanNpcs
{
    NPC_SHAMAN_EARTH_ELEMENTAL                  = 15352,
    NPC_SHAMAN_FIRE_ELEMENTAL                   = 15438,
    NPC_SHAMAN_PRIMAL_EARTH_ELEMENTAL           = 61056,
    NPC_SHAMAN_PRIMAL_FIRE_ELEMENTAL            = 61029
};

enum ShamanSpellIcons
{
    SHAMAN_ICON_ID_SOOTHING_RAIN                = 2011,
    SHAMAN_ICON_ID_SHAMAN_LAVA_FLOW             = 3087
};

enum ShamanWeaponEnchants
{
    SHAMAN_ENCHANT_FROSTBRAND                   = 2,
    SHAMAN_ENCHANT_FLAMETONGUE                  = 5,
    SHAMAN_ENCHANT_WINDFURY                     = 283,
    SHAMAN_ENCHANT_ROCKBITER                    = 3021,
    SHAMAN_ENCHANT_EARTHLIVING                  = 3345
};

static bool ShamanHealthPctOrder(WorldObject* a, WorldObject* b)
{
    Unit* unitA = a->ToUnit();
    Unit* unitB = b->ToUnit();
    return (unitA ? unitA->GetHealthPct() : 100.0f) < (unitB ? unitB->GetHealthPct() : 100.0f);
}

static void ShamanUnleashElement(Player* player, uint32 enchantment, Unit* target)
{
    uint32 unleashSpell = 0;
    uint32 furySpell = 0;
    bool hasFuryTalent = player->HasAura(SPELL_SHAMAN_UNLEASHED_FURY_TALENT);
    bool hostileTarget = player->IsValidAttackTarget(target);
    bool hostileSpell = true;

    switch (enchantment)
    {
        case SHAMAN_ENCHANT_EARTHLIVING:
            unleashSpell = SPELL_SHAMAN_UNLEASH_LIFE;
            hostileSpell = false;
            if (hasFuryTalent)
                furySpell = SPELL_SHAMAN_UNLEASHED_FURY_EARTHLIVING;
            break;
        case SHAMAN_ENCHANT_FLAMETONGUE:
            unleashSpell = SPELL_SHAMAN_UNLEASH_FLAME;
            if (hasFuryTalent)
                furySpell = SPELL_SHAMAN_UNLEASHED_FURY_FLAMETONGUE;
            break;
        case SHAMAN_ENCHANT_FROSTBRAND:
            unleashSpell = SPELL_SHAMAN_UNLEASH_FROST;
            if (hasFuryTalent)
                furySpell = SPELL_SHAMAN_UNLEASHED_FURY_FROSTBRAND;
            break;
        case SHAMAN_ENCHANT_ROCKBITER:
            unleashSpell = SPELL_SHAMAN_UNLEASH_EARTH;
            if (hasFuryTalent)
                furySpell = SPELL_SHAMAN_UNLEASHED_FURY_ROCKBITER;
            break;
        case SHAMAN_ENCHANT_WINDFURY:
            unleashSpell = SPELL_SHAMAN_UNLEASH_WIND;
            if (hasFuryTalent)
                furySpell = SPELL_SHAMAN_UNLEASHED_FURY_WINDFURY;
            break;
        default:
            return;
    }

    if (hostileSpell && !hostileTarget)
        return;

    if (!hostileSpell && hostileTarget)
        target = player;

    if (unleashSpell)
        player->CastSpell(target, unleashSpell, true);

    if (furySpell)
        player->CastSpell(target, furySpell, true);
}

// 51558 - Ancestral Awakening
// 138305 - Item - Shaman T15 Restoration 4P Bonus
class spell_sha_ancestral_awakening : public SpellScriptLoader
{
public:
    spell_sha_ancestral_awakening() : SpellScriptLoader("spell_sha_ancestral_awakening") { }

    class spell_sha_ancestral_awakening_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_ancestral_awakening_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SHAMAN_ANCESTRAL_AWAKENING))
                return false;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            if (!spellInfo || spellInfo->Id == SPELL_SHAMAN_CHAIN_HEAL || !eventInfo.GetHealInfo())
                return false;

            Spell const* procSpell = eventInfo.TriggeredBySpell();
            if (!procSpell || !procSpell->m_targets.GetUnitTarget())
                return false;

            if (GetId() == SPELL_SHAMAN_ITEM_T15_RESTORATION_4P)
                return !(eventInfo.GetHitMask() & PROC_EX_CRITICAL_HIT);
            return (eventInfo.GetHitMask() & PROC_EX_CRITICAL_HIT) != 0;
        }

        void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            int32 heal = int32(CalculatePct(eventInfo.GetHealInfo()->GetHeal(), aurEff->GetAmount()));

            GetTarget()->CastCustomSpell(SPELL_SHAMAN_ANCESTRAL_AWAKENING, SPELLVALUE_BASE_POINT0, heal, (Unit*)NULL, true, NULL, aurEff);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_sha_ancestral_awakening_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_sha_ancestral_awakening_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_ancestral_awakening_AuraScript();
    }
};

// 52759 - Ancestral Awakening
class spell_sha_ancestral_awakening_proc : public SpellScriptLoader
{
public:
    spell_sha_ancestral_awakening_proc() : SpellScriptLoader("spell_sha_ancestral_awakening_proc") { }

    class spell_sha_ancestral_awakening_proc_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_ancestral_awakening_proc_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SHAMAN_ANCESTRAL_AWAKENING_PROC))
                return false;
            return true;
        }

        void SelectTarget(std::list<WorldObject*>& targets)
        {
            targets.sort(ShamanHealthPctOrder);
            if (targets.size() > 1)
                targets.resize(1);
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            GetCaster()->CastCustomSpell(SPELL_SHAMAN_ANCESTRAL_AWAKENING_PROC, SPELLVALUE_BASE_POINT0, GetEffectValue(), GetHitUnit(), true);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_ancestral_awakening_proc_SpellScript::SelectTarget, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
            OnEffectHitTarget += SpellEffectFn(spell_sha_ancestral_awakening_proc_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_ancestral_awakening_proc_SpellScript();
    }
};

// 108281 - Ancestral Guidance
class spell_sha_ancestral_guidance : public SpellScriptLoader
{
public:
    spell_sha_ancestral_guidance() : SpellScriptLoader("spell_sha_ancestral_guidance") { }

    class spell_sha_ancestral_guidance_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_ancestral_guidance_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_SHAMAN_ANCESTRAL_GUIDANCE_HEAL) != NULL;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            return eventInfo.GetActionTarget() && eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage();
        }

        void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            bool isHeal = (eventInfo.GetTypeMask() & PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_POS) != 0;
            int32 amount = int32(eventInfo.GetDamageInfo()->GetDamage());
            amount = CalculatePct(amount, GetSpellInfo()->Effects[isHeal ? EFFECT_1 : EFFECT_0].CalcValue(GetUnitOwner()));
            if (amount <= 0)
                return;

            GetUnitOwner()->CastCustomSpell(SPELL_SHAMAN_ANCESTRAL_GUIDANCE_HEAL, SPELLVALUE_BASE_POINT0, amount, eventInfo.GetActionTarget(), true, NULL, aurEff);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_sha_ancestral_guidance_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_sha_ancestral_guidance_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_ancestral_guidance_AuraScript();
    }
};

// 114911 - Ancestral Guidance
class spell_sha_ancestral_guidance_heal : public SpellScriptLoader
{
public:
    spell_sha_ancestral_guidance_heal() : SpellScriptLoader("spell_sha_ancestral_guidance_heal") { }

    class spell_sha_ancestral_guidance_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_ancestral_guidance_heal_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            Unit* caster = GetCaster();
            targets.remove_if([caster](WorldObject* target)
            {
                Unit* unit = target->ToUnit();
                return !unit || !caster->IsInRaidWith(unit);
            });
            targets.sort(ShamanHealthPctOrder);
            if (targets.size() > 3)
                targets.resize(3);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_ancestral_guidance_heal_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_ancestral_guidance_heal_SpellScript();
    }
};

// 2825 - Bloodlust
/// Updated 4.3.4
class spell_sha_bloodlust : public SpellScriptLoader
{
public:
    spell_sha_bloodlust() : SpellScriptLoader("spell_sha_bloodlust") { }

    class spell_sha_bloodlust_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_bloodlust_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SHAMAN_SATED))
                return false;
            return true;
        }

        void RemoveInvalidTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if(Skyfire::UnitAuraCheck(true, SPELL_SHAMAN_SATED));
            targets.remove_if(Skyfire::UnitAuraCheck(true, SPELL_HUNTER_INSANITY));
            targets.remove_if(Skyfire::UnitAuraCheck(true, SPELL_MAGE_TEMPORAL_DISPLACEMENT));
        }

        void ApplyDebuff()
        {
            if (Unit* target = GetHitUnit())
                target->CastSpell(target, SPELL_SHAMAN_SATED, true);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_bloodlust_SpellScript::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_bloodlust_SpellScript::RemoveInvalidTargets, EFFECT_1, TARGET_UNIT_CASTER_AREA_RAID);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_bloodlust_SpellScript::RemoveInvalidTargets, EFFECT_2, TARGET_UNIT_CASTER_AREA_RAID);
            AfterHit += SpellHitFn(spell_sha_bloodlust_SpellScript::ApplyDebuff);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_bloodlust_SpellScript();
    }
};

// 108285 - Call of the Elements
class spell_sha_call_of_the_elements : public SpellScriptLoader
{
public:
    spell_sha_call_of_the_elements() : SpellScriptLoader("spell_sha_call_of_the_elements") { }

    class spell_sha_call_of_the_elements_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_call_of_the_elements_SpellScript);

        void HandleOnHit()
        {
            static uint32 const totemSpells[] = { 2484, 5394, 8143, 8177, 51485, 108269, 108270, 108273 };

            if (Player* player = GetCaster()->ToPlayer())
                for (uint32 spellId : totemSpells)
                    if (player->HasSpellCooldown(spellId))
                        player->RemoveSpellCooldown(spellId, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_sha_call_of_the_elements_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_call_of_the_elements_SpellScript();
    }
};

// 1064 - Chain Heal
/// Updated 4.3.4
class spell_sha_chain_heal : public SpellScriptLoader
{
public:
    spell_sha_chain_heal() : SpellScriptLoader("spell_sha_chain_heal") { }

    class spell_sha_chain_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_chain_heal_SpellScript);

        bool Load() OVERRIDE
        {
            firstHeal = true;
            riptide = false;
            amount = 0;
            return true;
        }

        void HandleHeal(SpellEffIndex /*effIndex*/)
        {
            if (firstHeal)
            {
                // Check if the target has Riptide
                if (AuraEffect* aurEff = GetHitUnit()->GetAuraEffect(SPELL_AURA_PERIODIC_HEAL, SPELLFAMILY_SHAMAN, 0, 0, 0x10, GetCaster()->GetGUID()))
                {
                    riptide = true;
                    amount = aurEff->GetSpellInfo()->Effects[EFFECT_2].CalcValue();
                    // Consume it
                    GetHitUnit()->RemoveAura(aurEff->GetBase());
                }
                firstHeal = false;
            }
            // Riptide increases the Chain Heal effect by 25%
            if (riptide)
            {
                uint32 bonus = CalculatePct(GetHitHeal(), amount);
                SetHitHeal(GetHitHeal() + bonus);
            }
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_sha_chain_heal_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
        }

        bool firstHeal;
        bool riptide;
        uint32 amount;
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_chain_heal_SpellScript();
    }
};

// 108282 - Conductivity
class spell_sha_conductivity : public SpellScriptLoader
{
public:
    spell_sha_conductivity() : SpellScriptLoader("spell_sha_conductivity") { }

    class spell_sha_conductivity_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_conductivity_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            if (!spellInfo)
                return false;

            // Only the primary target of Chain Heal extends Healing Rain
            if (spellInfo->Id == SPELL_SHAMAN_CHAIN_HEAL)
            {
                Spell const* procSpell = eventInfo.TriggeredBySpell();
                if (!procSpell || procSpell->m_targets.GetUnitTarget() != eventInfo.GetActionTarget())
                    return false;
            }

            // Purge, Fire Blast (Fire Elemental) and Deluge share family flags with the triggering spells
            return spellInfo->Id != 370 && spellInfo->Id != 57984 && spellInfo->Id != 126371;
        }

        void HandleProc(ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();

            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (AuraEffect* healingRain = caster->GetAuraEffect(SPELL_SHAMAN_HEALING_RAIN, EFFECT_1, caster->GetGUID()))
            {
                Aura* aura = healingRain->GetBase();
                int32 const maxAllowed = 40 * IN_MILLISECONDS;
                if (aura->GetMaxDuration() < maxAllowed)
                {
                    int32 add = healingRain->GetAmplitude() * 2;
                    aura->SetMaxDuration(std::min(aura->GetMaxDuration() + add, maxAllowed));
                    aura->SetDuration(aura->GetDuration() + add);
                }
            }
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_sha_conductivity_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_sha_conductivity_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_conductivity_AuraScript();
    }
};

// 974 - Earth Shield
class spell_sha_earth_shield : public SpellScriptLoader
{
public:
    spell_sha_earth_shield() : SpellScriptLoader("spell_sha_earth_shield") { }

    class spell_sha_earth_shield_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_earth_shield_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SHAMAN_EARTH_SHIELD_HEAL))
                return false;
            return true;
        }

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* caster = GetCaster())
            {
                amount = caster->SpellHealingBonusDone(GetUnitOwner(), GetSpellInfo(), amount, HEAL);
                amount = GetUnitOwner()->SpellHealingBonusTaken(caster, GetSpellInfo(), amount, HEAL);
            }
        }

        bool CheckProc(ProcEventInfo& /*eventInfo*/)
        {
            //! HACK due to currenct proc system implementation
            if (Player* player = GetTarget()->ToPlayer())
                if (player->HasSpellCooldown(SPELL_SHAMAN_EARTH_SHIELD_HEAL))
                    return false;
            return true;
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();

            GetTarget()->CastCustomSpell(SPELL_SHAMAN_EARTH_SHIELD_HEAL, SPELLVALUE_BASE_POINT0, aurEff->GetAmount(), GetTarget(), true, NULL, aurEff, GetCasterGUID());

            /// @hack: due to currenct proc system implementation
            if (Player* player = GetTarget()->ToPlayer())
                player->AddSpellCooldown(SPELL_SHAMAN_EARTH_SHIELD_HEAL, 0, time(NULL) + 3);
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
                caster->BindAura(GetAura());
        }

        void Register() OVERRIDE
        {
            AfterEffectApply += AuraEffectApplyFn(spell_sha_earth_shield_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_earth_shield_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_DUMMY);
            DoCheckProc += AuraCheckProcFn(spell_sha_earth_shield_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_sha_earth_shield_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_earth_shield_AuraScript();
    }
};

// 64695 - Earthgrab
class spell_sha_earthgrab : public SpellScriptLoader
{
public:
    spell_sha_earthgrab() : SpellScriptLoader("spell_sha_earthgrab") { }

    class spell_sha_earthgrab_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_earthgrab_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_SHAMAN_EARTHGRAB_IMMUNITY) && sSpellMgr->GetSpellInfo(SPELL_SHAMAN_EARTHBIND_FOR_EARTHGRAB);
        }

        void SelectTargets(std::list<WorldObject*>& targets)
        {
            Unit* caster = GetCaster();
            targets.remove_if([caster](WorldObject* obj)
            {
                if (Unit* target = obj->ToUnit())
                {
                    if (target->HasAura(SPELL_SHAMAN_EARTHGRAB_IMMUNITY))
                    {
                        caster->CastSpell(target, SPELL_SHAMAN_EARTHBIND_FOR_EARTHGRAB, true);
                        return true;
                    }
                }
                return false;
            });
        }

        void HandleHit()
        {
            if (Unit* target = GetHitUnit())
                GetCaster()->CastSpell(target, SPELL_SHAMAN_EARTHGRAB_IMMUNITY, true);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_earthgrab_SpellScript::SelectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnHit += SpellHitFn(spell_sha_earthgrab_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_earthgrab_SpellScript();
    }
};

// 61882 - Earthquake
class spell_sha_earthquake : public SpellScriptLoader
{
public:
    spell_sha_earthquake() : SpellScriptLoader("spell_sha_earthquake") { }

    class spell_sha_earthquake_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_earthquake_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_SHAMAN_EARTHQUAKE_TICK) != NULL;
        }

        void HandlePeriodic(AuraEffect const* aurEff)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (DynamicObject* dynObj = caster->GetDynObject(SPELL_SHAMAN_EARTHQUAKE))
                caster->CastSpell(dynObj->GetPositionX(), dynObj->GetPositionY(), dynObj->GetPositionZ(), SPELL_SHAMAN_EARTHQUAKE_TICK, true, NULL, aurEff);
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_earthquake_AuraScript::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_earthquake_AuraScript();
    }
};

// 77478 - Earthquake
class spell_sha_earthquake_tick : public SpellScriptLoader
{
public:
    spell_sha_earthquake_tick() : SpellScriptLoader("spell_sha_earthquake_tick") { }

    class spell_sha_earthquake_tick_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_earthquake_tick_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_SHAMAN_EARTHQUAKE_KNOCK_DOWN) != NULL;
        }

        void HandleScript(SpellEffIndex /*effIndex*/)
        {
            if (Unit* target = GetHitUnit())
                if (roll_chance_i(GetEffectValue()))
                    GetCaster()->CastSpell(target, SPELL_SHAMAN_EARTHQUAKE_KNOCK_DOWN, true);
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_sha_earthquake_tick_SpellScript::HandleScript, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_earthquake_tick_SpellScript();
    }
};

// 117014 - Elemental Blast
class spell_sha_elemental_blast : public SpellScriptLoader
{
public:
    spell_sha_elemental_blast() : SpellScriptLoader("spell_sha_elemental_blast") { }

    class spell_sha_elemental_blast_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_elemental_blast_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_SHAMAN_ELEMENTAL_BLAST_RATING_BONUS) != NULL;
        }

        void HandleAfterCast()
        {
            Player* player = GetCaster()->ToPlayer();
            if (!player)
                return;

            player->CastSpell(player, SPELL_SHAMAN_ELEMENTAL_BLAST_RATING_BONUS, true);

            if (Aura* aura = player->GetAura(SPELL_SHAMAN_ELEMENTAL_BLAST_RATING_BONUS, player->GetGUID()))
            {
                // Agility (effect 3) is only rolled for Enhancement
                uint32 maxEffect = player->GetTalentSpecialization(player->GetActiveSpec()) == TALENT_TREE_SHAMAN_ENHANCEMENT ? 3 : 2;
                uint32 randomEffect = urand(0, maxEffect);

                for (uint8 i = 0; i < 4; ++i)
                    if (AuraEffect* aurEff = aura->GetEffect(i))
                        aurEff->ChangeAmount(i == randomEffect ? 3500 : 0);
            }

            if (Unit* target = GetExplTargetUnit())
            {
                player->CastSpell(target, SPELL_SHAMAN_ELEMENTAL_BLAST_FROST_VISUAL, true);
                player->CastSpell(target, SPELL_SHAMAN_ELEMENTAL_BLAST_NATURE_VISUAL, true);
            }
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_sha_elemental_blast_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_elemental_blast_SpellScript();
    }
};

// 1535 Fire Nova
/// Updated 4.3.4
class spell_sha_fire_nova : public SpellScriptLoader
{
public:
    spell_sha_fire_nova() : SpellScriptLoader("spell_sha_fire_nova") { }

    class spell_sha_fire_nova_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_fire_nova_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (Unit* target = GetHitUnit())
            {
                if (target->HasAura(SPELL_SHAMAN_FLAME_SHOCK))
                {
                    caster->CastSpell(target, SPELL_SHAMAN_FIRE_NOVA_TRIGGERED_R1, true);
                    target->RemoveAurasDueToSpell(SPELL_SHAMAN_FLAME_SHOCK);
                }
            }
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_sha_fire_nova_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_fire_nova_SpellScript();
    }
};

// 144964 - Flow of the Elements
class spell_sha_flow_of_the_elements : public SpellScriptLoader
{
public:
    spell_sha_flow_of_the_elements() : SpellScriptLoader("spell_sha_flow_of_the_elements") { }

    class spell_sha_flow_of_the_elements_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_flow_of_the_elements_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            return GetUnitOwner()->GetTypeId() == TypeID::TYPEID_PLAYER && eventInfo.GetActionTarget();
        }

        void HandleProc(ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            static uint32 const enchants[] = { SHAMAN_ENCHANT_FROSTBRAND, SHAMAN_ENCHANT_FLAMETONGUE, SHAMAN_ENCHANT_WINDFURY };
            if (Player* player = GetUnitOwner()->ToPlayer())
                ShamanUnleashElement(player, enchants[urand(0, 2)], eventInfo.GetActionTarget());
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_sha_flow_of_the_elements_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_sha_flow_of_the_elements_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_flow_of_the_elements_AuraScript();
    }
};

// 8042 - Earth Shock
class spell_sha_fulmination : public SpellScriptLoader
{
public:
    spell_sha_fulmination() : SpellScriptLoader("spell_sha_fulmination") { }

    class spell_sha_fulmination_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_fulmination_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_SHAMAN_FULMINATION_TRIGGERED) && sSpellMgr->GetSpellInfo(SPELL_SHAMAN_LIGHTNING_SHIELD_DAMAGE);
        }

        void HandleAfterCast()
        {
            Unit* caster = GetCaster();
            Unit* target = GetExplTargetUnit();
            if (!target || !caster->HasAura(SPELL_SHAMAN_FULMINATION))
                return;

            Aura* lightningShield = caster->GetAura(SPELL_SHAMAN_LIGHTNING_SHIELD);
            if (!lightningShield || lightningShield->GetCharges() <= 1)
                return;

            int32 usedCharges = lightningShield->GetCharges() - 1;
            SpellInfo const* orbInfo = sSpellMgr->GetSpellInfo(SPELL_SHAMAN_LIGHTNING_SHIELD_DAMAGE);
            int32 basePoints = caster->CalculateSpellDamage(target, orbInfo, EFFECT_0);
            int32 damage = usedCharges * int32(caster->SpellDamageBonusDone(target, orbInfo, uint32(std::max(basePoints, 0)), SPELL_DIRECT_DAMAGE));

            caster->CastCustomSpell(SPELL_SHAMAN_FULMINATION_TRIGGERED, SPELLVALUE_BASE_POINT0, damage, target, true);
            lightningShield->SetCharges(1);
            caster->RemoveAurasDueToSpell(SPELL_SHAMAN_FULMINATION_INFO);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_sha_fulmination_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_fulmination_SpellScript();
    }
};

// 51886 - Cleanse Spirit
// 77130 - Purify Spirit
class spell_sha_glyph_of_cleansing_waters : public SpellScriptLoader
{
public:
    spell_sha_glyph_of_cleansing_waters() : SpellScriptLoader("spell_sha_glyph_of_cleansing_waters") { }

    class spell_sha_glyph_of_cleansing_waters_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_glyph_of_cleansing_waters_SpellScript);

        void HandleHit()
        {
            Unit* caster = GetCaster();
            if (GetDispelCount() && GetHitUnit() && caster->HasAura(SPELL_SHAMAN_GLYPH_OF_CLEANSING_WATERS))
                caster->CastCustomSpell(SPELL_SHAMAN_CLEANSING_WATERS, SPELLVALUE_BASE_POINT0, int32(caster->CountPctFromMaxHealth(5)), GetHitUnit(), true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_sha_glyph_of_cleansing_waters_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_glyph_of_cleansing_waters_SpellScript();
    }
};

// 55447 - Glyph of Flame Shock
class spell_sha_glyph_of_flame_shock : public SpellScriptLoader
{
public:
    spell_sha_glyph_of_flame_shock() : SpellScriptLoader("spell_sha_glyph_of_flame_shock") { }

    class spell_sha_glyph_of_flame_shock_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_glyph_of_flame_shock_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_SHAMAN_FLAMES_OF_LIFE) != NULL;
        }

        void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            if (!eventInfo.GetDamageInfo())
                return;

            int32 heal = CalculatePct(int32(eventInfo.GetDamageInfo()->GetDamage()), aurEff->GetAmount());
            if (heal > 0)
                GetUnitOwner()->CastCustomSpell(SPELL_SHAMAN_FLAMES_OF_LIFE, SPELLVALUE_BASE_POINT0, heal, GetUnitOwner(), true);
        }

        void Register() OVERRIDE
        {
            OnEffectProc += AuraEffectProcFn(spell_sha_glyph_of_flame_shock_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_glyph_of_flame_shock_AuraScript();
    }
};

// 55440 - Glyph of Healing Wave
class spell_sha_glyph_of_healing_wave : public SpellScriptLoader
{
public:
    spell_sha_glyph_of_healing_wave() : SpellScriptLoader("spell_sha_glyph_of_healing_wave") { }

    class spell_sha_glyph_of_healing_wave_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_glyph_of_healing_wave_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SHAMAN_GLYPH_OF_HEALING_WAVE))
                return false;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            // Not proc from self heals
            return GetTarget() != eventInfo.GetProcTarget();
        }

        void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            int32 heal = CalculatePct(int32(eventInfo.GetHealInfo()->GetHeal()), aurEff->GetAmount());

            GetTarget()->CastCustomSpell(SPELL_SHAMAN_GLYPH_OF_HEALING_WAVE, SPELLVALUE_BASE_POINT0, heal, (Unit*)NULL, true, NULL, aurEff);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_sha_glyph_of_healing_wave_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_sha_glyph_of_healing_wave_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_glyph_of_healing_wave_AuraScript();
    }
};

// 2645 - Ghost Wolf
class spell_sha_glyph_of_lakestrider : public SpellScriptLoader
{
public:
    spell_sha_glyph_of_lakestrider() : SpellScriptLoader("spell_sha_glyph_of_lakestrider") { }

    class spell_sha_glyph_of_lakestrider_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_glyph_of_lakestrider_SpellScript);

        void HandleOnHit()
        {
            if (Player* player = GetCaster()->ToPlayer())
                if (player->HasAura(SPELL_SHAMAN_GLYPH_OF_LAKESTRIDER))
                    player->CastSpell(player, SPELL_SHAMAN_WATER_WALKING, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_sha_glyph_of_lakestrider_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_glyph_of_lakestrider_SpellScript();
    }
};

// 370 - Purge
class spell_sha_glyph_of_purging : public SpellScriptLoader
{
public:
    spell_sha_glyph_of_purging() : SpellScriptLoader("spell_sha_glyph_of_purging") { }

    class spell_sha_glyph_of_purging_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_glyph_of_purging_SpellScript);

        void HandleHit()
        {
            if (GetDispelCount() && GetCaster()->HasAura(SPELL_SHAMAN_GLYPH_OF_PURGING))
                GetCaster()->CastSpell(GetCaster(), SPELL_SHAMAN_MAELSTROM_WEAPON, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_sha_glyph_of_purging_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_glyph_of_purging_SpellScript();
    }
};

// 30823 - Shamanistic Rage
class spell_sha_glyph_of_shamanistic_rage : public SpellScriptLoader
{
public:
    spell_sha_glyph_of_shamanistic_rage() : SpellScriptLoader("spell_sha_glyph_of_shamanistic_rage") { }

    class spell_sha_glyph_of_shamanistic_rage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_glyph_of_shamanistic_rage_SpellScript);

        void HandleHit()
        {
            Unit* caster = GetCaster();
            if (!caster->HasAura(SPELL_SHAMAN_GLYPH_OF_SHAMANISTIC_RAGE))
                return;

            DispelChargesList dispelList;
            caster->GetDispellableAuraList(caster, DISPEL_ALL_MASK, dispelList);
            for (DispelChargesList::const_iterator itr = dispelList.begin(); itr != dispelList.end(); ++itr)
                caster->RemoveAurasDueToSpellByDispel(itr->first->GetId(), GetSpellInfo()->Id, itr->first->GetCasterGUID(), caster, itr->second);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_sha_glyph_of_shamanistic_rage_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_glyph_of_shamanistic_rage_SpellScript();
    }
};

// 73920 - Healing Rain
class spell_sha_healing_rain : public SpellScriptLoader
{
public:
    spell_sha_healing_rain() : SpellScriptLoader("spell_sha_healing_rain") { }

    class spell_sha_healing_rain_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_healing_rain_AuraScript);

    public:
        void SetDest(float x, float y, float z)
        {
            _hasDest = true;
            _x = x;
            _y = y;
            _z = z;
        }

    private:
        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_SHAMAN_HEALING_RAIN_TICK) != NULL;
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
            {
                if (Aura* unleashLife = caster->GetAura(SPELL_SHAMAN_UNLEASH_LIFE))
                {
                    _unleashLife = true;
                    caster->RemoveAura(unleashLife);
                }
            }
        }

        void HandlePeriodic(AuraEffect const* aurEff)
        {
            Unit* caster = GetCaster();
            if (!caster || !_hasDest)
                return;

            SpellInfo const* tickInfo = sSpellMgr->GetSpellInfo(SPELL_SHAMAN_HEALING_RAIN_TICK);
            SpellCastTargets targets;
            targets.SetDst(_x, _y, _z, caster->GetOrientation(), caster->GetMapId());

            CustomSpellValues values;
            if (_unleashLife)
                values.AddSpellMod(SPELLVALUE_BASE_POINT0, CalculatePct(caster->CalculateSpellDamage(caster, tickInfo, EFFECT_0), 130));

            caster->CastSpell(targets, tickInfo, &values, TRIGGERED_FULL_MASK, NULL, aurEff);
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_sha_healing_rain_AuraScript::HandleApply, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_healing_rain_AuraScript::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        }

        bool _hasDest = false;
        bool _unleashLife = false;
        float _x = 0.0f;
        float _y = 0.0f;
        float _z = 0.0f;
    };

    class spell_sha_healing_rain_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_healing_rain_SpellScript);

        void HandleAfterCast()
        {
            WorldLocation const* dest = GetExplTargetDest();
            if (!dest)
                return;

            Unit* caster = GetCaster();
            if (Aura* aura = caster->GetAura(SPELL_SHAMAN_HEALING_RAIN, caster->GetGUID()))
                if (spell_sha_healing_rain_AuraScript* script = dynamic_cast<spell_sha_healing_rain_AuraScript*>(aura->GetScriptByName("spell_sha_healing_rain")))
                    script->SetDest(dest->GetPositionX(), dest->GetPositionY(), dest->GetPositionZ());
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_sha_healing_rain_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_healing_rain_SpellScript();
    }

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_healing_rain_AuraScript();
    }
};

// 52042 - Healing Stream
class spell_sha_healing_stream : public SpellScriptLoader
{
public:
    spell_sha_healing_stream() : SpellScriptLoader("spell_sha_healing_stream") { }

    class spell_sha_healing_stream_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_healing_stream_SpellScript);

        bool Load() OVERRIDE
        {
            _bonusTarget = 0;
            return true;
        }

        void SelectTarget(std::list<WorldObject*>& targets)
        {
            Unit* shaman = GetCaster()->GetOwner();
            if (!shaman)
            {
                targets.clear();
                return;
            }

            targets.remove_if([shaman](WorldObject* target)
            {
                Unit* unit = target->ToUnit();
                return !unit || !shaman->IsInRaidWith(unit);
            });
            targets.sort(ShamanHealthPctOrder);

            size_t targetCount = shaman->HasAura(SPELL_SHAMAN_RUSHING_STREAMS) ? 2 : 1;
            if (shaman->HasAura(SPELL_SHAMAN_ITEM_T15_RESTORATION_2P) && targets.size() > targetCount)
            {
                std::list<WorldObject*>::iterator itr = targets.begin();
                std::advance(itr, targetCount);
                _bonusTarget = (*itr)->GetGUID();
            }

            if (targets.size() > targetCount)
                targets.resize(targetCount);
        }

        void HandleHit()
        {
            if (Unit* shaman = GetCaster()->GetOwner())
                if (Unit* target = GetHitUnit())
                    if (shaman->HasAura(SPELL_SHAMAN_GLYPH_OF_HEALING_STREAM_TOTEM))
                        shaman->CastSpell(target, SPELL_SHAMAN_GLYPH_OF_HEALING_STREAM, true);
        }

        void HandleAfterCast()
        {
            if (_bonusTarget)
                if (Unit* target = ObjectAccessor::GetUnit(*GetCaster(), _bonusTarget))
                    GetCaster()->CastSpell(target, SPELL_SHAMAN_HEALING_STREAM_TOTEM_BONUS, true);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_healing_stream_SpellScript::SelectTarget, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
            OnHit += SpellHitFn(spell_sha_healing_stream_SpellScript::HandleHit);
            AfterCast += SpellCastFn(spell_sha_healing_stream_SpellScript::HandleAfterCast);
        }

        uint64 _bonusTarget;
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_healing_stream_SpellScript();
    }
};

// 32182 - Heroism
/// Updated 4.3.4
class spell_sha_heroism : public SpellScriptLoader
{
public:
    spell_sha_heroism() : SpellScriptLoader("spell_sha_heroism") { }

    class spell_sha_heroism_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_heroism_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SHAMAN_EXHAUSTION))
                return false;
            return true;
        }

        void RemoveInvalidTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if(Skyfire::UnitAuraCheck(true, SPELL_SHAMAN_EXHAUSTION));
            targets.remove_if(Skyfire::UnitAuraCheck(true, SPELL_HUNTER_INSANITY));
            targets.remove_if(Skyfire::UnitAuraCheck(true, SPELL_MAGE_TEMPORAL_DISPLACEMENT));
        }

        void ApplyDebuff()
        {
            if (Unit* target = GetHitUnit())
                target->CastSpell(target, SPELL_SHAMAN_EXHAUSTION, true);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_heroism_SpellScript::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_heroism_SpellScript::RemoveInvalidTargets, EFFECT_1, TARGET_UNIT_CASTER_AREA_RAID);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_heroism_SpellScript::RemoveInvalidTargets, EFFECT_2, TARGET_UNIT_CASTER_AREA_RAID);
            AfterHit += SpellHitFn(spell_sha_heroism_SpellScript::ApplyDebuff);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_heroism_SpellScript();
    }
};

// 51514 - Hex
class spell_sha_hex : public SpellScriptLoader
{
public:
    spell_sha_hex() : SpellScriptLoader("spell_sha_hex") { }

    class spell_sha_hex_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_hex_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            if (target->IsMounted())
            {
                target->Dismount();
                target->RemoveAurasByType(SPELL_AURA_MOUNTED);
            }

            if (target->HasUnitState(UNIT_STATE_CASTING))
            {
                target->InterruptSpell(CURRENT_GENERIC_SPELL);
                target->InterruptSpell(CURRENT_CHANNELED_SPELL);
            }
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_sha_hex_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_hex_AuraScript();
    }
};

// 23551 - Lightning Shield
class spell_sha_item_lightning_shield : public SpellScriptLoader
{
public:
    spell_sha_item_lightning_shield() : SpellScriptLoader("spell_sha_item_lightning_shield") { }

    class spell_sha_item_lightning_shield_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_item_lightning_shield_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SHAMAN_ITEM_LIGHTNING_SHIELD))
                return false;
            return true;
        }

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            GetTarget()->CastSpell(eventInfo.GetProcTarget(), SPELL_SHAMAN_ITEM_LIGHTNING_SHIELD, true, NULL, aurEff);
        }

        void Register() OVERRIDE
        {
            OnEffectProc += AuraEffectProcFn(spell_sha_item_lightning_shield_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_item_lightning_shield_AuraScript();
    }
};

// 23552 - Lightning Shield
class spell_sha_item_lightning_shield_trigger : public SpellScriptLoader
{
public:
    spell_sha_item_lightning_shield_trigger() : SpellScriptLoader("spell_sha_item_lightning_shield_trigger") { }

    class spell_sha_item_lightning_shield_trigger_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_item_lightning_shield_trigger_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SHAMAN_ITEM_MANA_SURGE))
                return false;
            return true;
        }

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            GetTarget()->CastSpell(GetTarget(), SPELL_SHAMAN_ITEM_LIGHTNING_SHIELD_DAMAGE, true, NULL, aurEff);
        }

        void Register() OVERRIDE
        {
            OnEffectProc += AuraEffectProcFn(spell_sha_item_lightning_shield_trigger_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_item_lightning_shield_trigger_AuraScript();
    }
};

// 23572 - Mana Surge
class spell_sha_item_mana_surge : public SpellScriptLoader
{
public:
    spell_sha_item_mana_surge() : SpellScriptLoader("spell_sha_item_mana_surge") { }

    class spell_sha_item_mana_surge_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_item_mana_surge_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SHAMAN_ITEM_LIGHTNING_SHIELD_DAMAGE))
                return false;
            return true;
        }

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            //int32 mana = eventInfo.GetDamageInfo()->GetSpellInfo()->CalcPowerCost(GetTarget(), SpellSchoolMask(eventInfo.GetDamageInfo()->GetSchoolMask()));
            //mana = int32(CalculatePct(mana, 35));

            //GetTarget()->CastCustomSpell(SPELL_SHAMAN_ITEM_MANA_SURGE, SPELLVALUE_BASE_POINT0, mana, GetTarget(), true, NULL, aurEff);
        }

        void Register() OVERRIDE
        {
            OnEffectProc += AuraEffectProcFn(spell_sha_item_mana_surge_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_item_mana_surge_AuraScript();
    }
};

// 324 - Lightning Shield
class spell_sha_lightning_shield : public SpellScriptLoader
{
public:
    spell_sha_lightning_shield() : SpellScriptLoader("spell_sha_lightning_shield") { }

    class spell_sha_lightning_shield_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_lightning_shield_AuraScript);

        void HandleProc(ProcEventInfo& eventInfo)
        {
            if (Unit* actor = eventInfo.GetActor())
            {
                if (actor->IsTotem())
                {
                    PreventDefaultAction();
                    return;
                }
            }

            GetAura()->SetUsingCharges(false);

            if (GetUnitOwner()->HasAura(SPELL_SHAMAN_GLYPH_OF_LIGHTNING_SHIELD))
                GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_SHAMAN_GLYPH_OF_LIGHTNING_SHIELD_MOD, true);
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_sha_lightning_shield_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_lightning_shield_AuraScript();
    }
};

// 138146 - Lightning Strike
class spell_sha_lightning_strike : public SpellScriptLoader
{
public:
    spell_sha_lightning_strike() : SpellScriptLoader("spell_sha_lightning_strike") { }

    class spell_sha_lightning_strike_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_lightning_strike_SpellScript);

        bool Load() OVERRIDE
        {
            _targetCount = 0;
            return true;
        }

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if([](WorldObject* target)
            {
                Unit* unit = target->ToUnit();
                return unit && unit->HasBreakableByDamageCrowdControlAura();
            });
            _targetCount = uint32(targets.size());
        }

        void HandleHit()
        {
            if (_targetCount)
                SetHitDamage(GetHitDamage() / int32(_targetCount));
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_lightning_strike_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            OnHit += SpellHitFn(spell_sha_lightning_strike_SpellScript::HandleHit);
        }

        uint32 _targetCount;
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_lightning_strike_SpellScript();
    }
};

// 51505 - Lava Burst
// 77451 - Lava Burst (Elemental Overload)
class spell_sha_lava_burst : public SpellScriptLoader
{
public:
    spell_sha_lava_burst() : SpellScriptLoader("spell_sha_lava_burst") { }

    class spell_sha_lava_burst_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_lava_burst_SpellScript);

        void HandleDamage(SpellEffIndex /*effIndex*/)
        {
            Unit* target = GetHitUnit();
            if (target && target->HasAura(SPELL_SHAMAN_FLAME_SHOCK, GetCaster()->GetGUID()))
                SetHitDamage(int32(GetHitDamage() * 1.5f));
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_sha_lava_burst_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_lava_burst_SpellScript();
    }
};

// 60103 - Lava Lash
class spell_sha_lava_lash : public SpellScriptLoader
{
public:
    spell_sha_lava_lash() : SpellScriptLoader("spell_sha_lava_lash") { }

    class spell_sha_lava_lash_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_lava_lash_SpellScript);

        bool Load() OVERRIDE
        {
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void HandleOnHit()
        {
            Player* caster = GetCaster()->ToPlayer();
            Unit* target = GetHitUnit();
            if (!caster || !target)
                return;

            Item* offhand = caster->GetWeaponForAttack(WeaponAttackType::OFF_ATTACK, true);
            if (!offhand)
                return;

            if (target->HasAura(SPELL_SHAMAN_FLAME_SHOCK, caster->GetGUID()) && !caster->HasAura(SPELL_SHAMAN_GLYPH_OF_LAVA_LASH))
                caster->CastSpell(target, SPELL_SHAMAN_LAVA_LASH_SPREAD_FLAME_SHOCK, true);

            int32 bonusPct = 0;
            if (offhand->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT) == SHAMAN_ENCHANT_FLAMETONGUE)
                bonusPct += GetSpellInfo()->Effects[EFFECT_1].CalcValue(caster);

            if (Aura* searingFlames = caster->GetAura(SPELL_SHAMAN_SEARING_FLAMES))
            {
                bonusPct += searingFlames->GetStackAmount() * 20;
                caster->RemoveAura(searingFlames);
            }

            int32 damage = GetHitDamage();
            AddPct(damage, bonusPct);
            SetHitDamage(damage);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_sha_lava_lash_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_lava_lash_SpellScript();
    }
};

// 105792 - Lava Lash
class spell_sha_lava_lash_spread_flame_shock : public SpellScriptLoader
{
public:
    spell_sha_lava_lash_spread_flame_shock() : SpellScriptLoader("spell_sha_lava_lash_spread_flame_shock") { }

    class spell_sha_lava_lash_spread_flame_shock_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_lava_lash_spread_flame_shock_SpellScript);

        void SelectTargets(std::list<WorldObject*>& targets)
        {
            uint64 casterGuid = GetCaster()->GetGUID();
            Unit* target = GetExplTargetUnit();
            if (!target || !target->HasAura(SPELL_SHAMAN_FLAME_SHOCK, casterGuid))
            {
                targets.clear();
                return;
            }

            targets.remove_if(Skyfire::UnitAuraCheck(true, SPELL_SHAMAN_FLAME_SHOCK, casterGuid));
            if (targets.size() > 4)
                targets.resize(4);
        }

        void HandleHit()
        {
            if (Unit* target = GetHitUnit())
                GetCaster()->AddAura(SPELL_SHAMAN_FLAME_SHOCK, target);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_lava_lash_spread_flame_shock_SpellScript::SelectTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            OnHit += SpellHitFn(spell_sha_lava_lash_spread_flame_shock_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_lava_lash_spread_flame_shock_SpellScript();
    }
};

class spell_sha_lava_surge : public SpellScriptLoader
{
public:
    spell_sha_lava_surge() : SpellScriptLoader("spell_sha_lava_surge") { }

    class spell_sha_lava_surge_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_lava_surge_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SHAMAN_LAVA_SURGE))
                return false;
            return true;
        }

        void HandleEffectProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            GetTarget()->CastSpell(GetTarget(), SPELL_SHAMAN_LAVA_SURGE, true);
        }

        void Register() OVERRIDE
        {
            OnEffectProc += AuraEffectProcFn(spell_sha_lava_surge_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_lava_surge_AuraScript();
    }
};

// 51505 - Lava Burst
class spell_sha_lava_surge_cooldown_fix : public SpellScriptLoader
{
public:
    spell_sha_lava_surge_cooldown_fix() : SpellScriptLoader("spell_sha_lava_surge_cooldown_fix") { }

    class spell_sha_lava_surge_cooldown_fix_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_lava_surge_cooldown_fix_SpellScript);

        bool instant = false;

        void HandleSpellStart()
        {
            instant = GetCaster()->HasAura(SPELL_SHAMAN_LAVA_SURGE);
        }

        void HandleAfterCast()
        {
            // A Lava Surge that procced while this Lava Burst was being cast must not be eaten by its cooldown
            if (instant)
                return;

            if (Player* player = GetCaster()->ToPlayer())
                if (player->HasAura(SPELL_SHAMAN_LAVA_SURGE))
                    player->RemoveSpellCooldown(SPELL_SHAMAN_LAVA_BURST, true);
        }

        void Register() OVERRIDE
        {
            OnSpellStart += SpellCastFn(spell_sha_lava_surge_cooldown_fix_SpellScript::HandleSpellStart);
            AfterCast += SpellCastFn(spell_sha_lava_surge_cooldown_fix_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_lava_surge_cooldown_fix_SpellScript();
    }
};

class spell_sha_lava_surge_proc : public SpellScriptLoader
{
public:
    spell_sha_lava_surge_proc() : SpellScriptLoader("spell_sha_lava_surge_proc") { }

    class spell_sha_lava_surge_proc_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_lava_surge_proc_SpellScript)

            bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SHAMAN_LAVA_BURST))
                return false;
            return true;
        }

        bool Load() OVERRIDE
        {
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            GetCaster()->ToPlayer()->RemoveSpellCooldown(SPELL_SHAMAN_LAVA_BURST, true);
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_sha_lava_surge_proc_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_lava_surge_proc_SpellScript();
    }
};

// 53817 - Maelstrom Weapon
class spell_sha_maelstrom_weapon : public SpellScriptLoader
{
public:
    spell_sha_maelstrom_weapon() : SpellScriptLoader("spell_sha_maelstrom_weapon") { }

    class spell_sha_maelstrom_weapon_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_maelstrom_weapon_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_SHAMAN_MAELSTROM_WEAPON_FULL_STACKS) != NULL;
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            if (GetStackAmount() >= GetSpellInfo()->StackAmount && !target->HasAura(SPELL_SHAMAN_MAELSTROM_WEAPON_FULL_STACKS))
                target->CastSpell(target, SPELL_SHAMAN_MAELSTROM_WEAPON_FULL_STACKS, true);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->RemoveAurasDueToSpell(SPELL_SHAMAN_MAELSTROM_WEAPON_FULL_STACKS);
        }

        void Register() OVERRIDE
        {
            AfterEffectApply += AuraEffectApplyFn(spell_sha_maelstrom_weapon_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            AfterEffectRemove += AuraEffectRemoveFn(spell_sha_maelstrom_weapon_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_maelstrom_weapon_AuraScript();
    }
};

// 16191 - Mana Tide
/// Updated 4.3.4
class spell_sha_mana_tide_totem : public SpellScriptLoader
{
public:
    spell_sha_mana_tide_totem() : SpellScriptLoader("spell_sha_mana_tide_totem") { }

    class spell_sha_mana_tide_totem_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_mana_tide_totem_AuraScript);

        void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
        {
            ///@TODO: Exclude the "short term" buffs from the stat value
            if (Unit* caster = GetCaster())
                if (Unit* owner = caster->GetOwner())
                    amount = CalculatePct(owner->GetStat(STAT_SPIRIT), aurEff->GetAmount());
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_mana_tide_totem_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_STAT);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_mana_tide_totem_AuraScript();
    }
};

// -30881 - Nature's Guardian
class spell_sha_nature_guardian : public SpellScriptLoader
{
public:
    spell_sha_nature_guardian() : SpellScriptLoader("spell_sha_nature_guardian") { }

    class spell_sha_nature_guardian_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_nature_guardian_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SHAMAN_NATURE_GUARDIAN))
                return false;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            //! HACK due to currenct proc system implementation
            if (Player* player = GetTarget()->ToPlayer())
                if (player->HasSpellCooldown(GetSpellInfo()->Id))
                    return false;

            return GetTarget()->HealthBelowPctDamaged(30, eventInfo.GetDamageInfo()->GetDamage());
        }

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            int32 basePoints0 = GetTarget()->CountPctFromMaxHealth(aurEff->GetAmount());

            GetTarget()->CastCustomSpell(GetTarget(), SPELL_SHAMAN_NATURE_GUARDIAN, &basePoints0, NULL, NULL, true);

            if (eventInfo.GetProcTarget() && eventInfo.GetProcTarget()->IsAlive())
                eventInfo.GetProcTarget()->getThreatManager().modifyThreatPercent(GetTarget(), -10);

            if (Player* player = GetTarget()->ToPlayer())
                player->AddSpellCooldown(GetSpellInfo()->Id, 0, time(NULL) + aurEff->GetSpellInfo()->Effects[EFFECT_1].CalcValue());
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_sha_nature_guardian_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_sha_nature_guardian_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_nature_guardian_AuraScript();
    }
};

// 113289 - Prowl
class spell_sha_prowl : public SpellScriptLoader
{
public:
    spell_sha_prowl() : SpellScriptLoader("spell_sha_prowl") { }

    class spell_sha_prowl_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_prowl_SpellScript);

        void HandleOnHit()
        {
            if (Player* player = GetCaster()->ToPlayer())
                player->CastSpell(player, SPELL_SHAMAN_GHOST_WOLF, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_sha_prowl_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_prowl_SpellScript();
    }
};

// 16213 - Purification
class spell_sha_purification : public SpellScriptLoader
{
public:
    spell_sha_purification() : SpellScriptLoader("spell_sha_purification") { }

    class spell_sha_purification_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_purification_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_SHAMAN_ANCESTRAL_VIGOR) != NULL;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            return eventInfo.GetActionTarget() && eventInfo.GetHealInfo() && eventInfo.GetHealInfo()->GetHeal();
        }

        void HandleProc(ProcEventInfo& eventInfo)
        {
            Unit* target = eventInfo.GetActionTarget();
            int32 amount = CalculatePct(int32(eventInfo.GetHealInfo()->GetHeal()), 10);

            // Capped at 10% of the target's unbuffed maximum health
            float pctMod = target->GetModifierValue(UNIT_MOD_HEALTH, TOTAL_PCT);
            if (pctMod <= 0.0f)
                return;

            int32 max = int32((target->GetMaxHealth() / pctMod - target->GetModifierValue(UNIT_MOD_HEALTH, TOTAL_VALUE)) * 0.1f);
            amount = std::min(max, amount);
            if (amount <= 0)
                return;

            if (AuraEffect* existing = target->GetAuraEffect(SPELL_SHAMAN_ANCESTRAL_VIGOR, EFFECT_0))
            {
                existing->ChangeAmount(std::min(existing->GetAmount() + amount, max));
                existing->GetBase()->RefreshDuration();
            }
            else
                GetUnitOwner()->CastCustomSpell(SPELL_SHAMAN_ANCESTRAL_VIGOR, SPELLVALUE_BASE_POINT0, amount, target, true);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_sha_purification_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_sha_purification_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_purification_AuraScript();
    }
};

// 114083 - Restorative Mists
class spell_sha_restorative_mists : public SpellScriptLoader
{
public:
    spell_sha_restorative_mists() : SpellScriptLoader("spell_sha_restorative_mists") { }

    class spell_sha_restorative_mists_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_restorative_mists_SpellScript);

        bool Load() OVERRIDE
        {
            _targetCount = 0;
            return true;
        }

        void SelectTarget(std::list<WorldObject*>& targets)
        {
            targets.remove_if([](WorldObject* target)
            {
                Unit* unit = target->ToUnit();
                return !unit || (unit->IsSummon() && !unit->IsGuardian());
            });
            _targetCount = uint32(targets.size());
        }

        void HandleHit()
        {
            if (_targetCount)
                SetHitHeal(GetHitHeal() / int32(_targetCount));
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_restorative_mists_SpellScript::SelectTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
            OnHit += SpellHitFn(spell_sha_restorative_mists_SpellScript::HandleHit);
        }

        uint32 _targetCount;
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_restorative_mists_SpellScript();
    }
};

// 16196 - Resurgence
class spell_sha_resurgence : public SpellScriptLoader
{
public:
    spell_sha_resurgence() : SpellScriptLoader("spell_sha_resurgence") { }

    class spell_sha_resurgence_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_resurgence_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_SHAMAN_RESURGENCE) != NULL;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            return eventInfo.GetSpellInfo() && GetUnitOwner()->HasAura(SPELL_SHAMAN_WATER_SHIELD);
        }

        void HandleProc(ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            float mult;
            switch (eventInfo.GetSpellInfo()->Id)
            {
                case 331:   // Healing Wave
                case 77472: // Greater Healing Wave
                    mult = 1.0f;
                    break;
                case 8004:  // Healing Surge
                case 61295: // Riptide
                case 73685: // Unleash Life
                    mult = 0.6f;
                    break;
                case 1064:  // Chain Heal
                    mult = 0.333f;
                    break;
                default:
                    return;
            }

            Unit* owner = GetUnitOwner();
            SpellInfo const* resurgence = sSpellMgr->GetSpellInfo(SPELL_SHAMAN_RESURGENCE);
            int32 mana = int32(owner->CalculateSpellDamage(owner, resurgence, EFFECT_0) * mult);
            if (mana > 0)
                owner->CastCustomSpell(SPELL_SHAMAN_RESURGENCE, SPELLVALUE_BASE_POINT0, mana, owner, true);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_sha_resurgence_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_sha_resurgence_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_resurgence_AuraScript();
    }
};

// 88764 - Rolling Thunder
class spell_sha_rolling_thunder : public SpellScriptLoader
{
public:
    spell_sha_rolling_thunder() : SpellScriptLoader("spell_sha_rolling_thunder") { }

    class spell_sha_rolling_thunder_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_rolling_thunder_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SHAMAN_LIGHTNING_SHIELD))
                return false;
            return true;
        }

        void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            Unit* target = GetTarget();
            if (Aura* aura = target->GetAura(SPELL_SHAMAN_LIGHTNING_SHIELD))
            {
                int32 maxCharges = aurEff->GetAmount();
                int32 add = 1;
                if (AuraEffect const* setBonus = target->GetAuraEffect(SPELL_SHAMAN_ITEM_T14_ELEMENTAL_4P, EFFECT_0))
                    add += setBonus->GetAmount();

                int32 charges = std::min(int32(aura->GetCharges()) + add, maxCharges);
                aura->SetCharges(uint8(charges));
                aura->RefreshDuration();

                if (charges >= maxCharges && target->HasAura(SPELL_SHAMAN_FULMINATION))
                    target->CastSpell(target, SPELL_SHAMAN_FULMINATION_INFO, true);
            }
        }

        void Register() OVERRIDE
        {
            OnEffectProc += AuraEffectProcFn(spell_sha_rolling_thunder_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_rolling_thunder_AuraScript();
    }
};

// 98020 - Spirit Link
class spell_sha_spirit_link : public SpellScriptLoader
{
public:
    spell_sha_spirit_link() : SpellScriptLoader("spell_sha_spirit_link") { }

    class spell_sha_spirit_link_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_spirit_link_SpellScript);

        bool Load() OVERRIDE
        {
            _averageHealthPct = 0.0f;
            return true;
        }

        void SelectTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if([](WorldObject* target) { return target->GetTypeId() != TypeID::TYPEID_PLAYER; });
            if (targets.empty())
                return;

            for (std::list<WorldObject*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
                if (Unit* target = (*itr)->ToUnit())
                    _averageHealthPct += target->GetHealthPct();
            _averageHealthPct /= float(targets.size());
        }

        void HandleHit()
        {
            Unit* target = GetHitUnit();
            if (!target)
                return;

            int32 expected = int32(target->GetMaxHealth() * _averageHealthPct / 100.0f);
            int32 diff = expected - int32(target->GetHealth());
            int32 value = std::abs(diff);
            if (value < 2)
                return;

            SpellValueMod mod = diff < 0 ? SPELLVALUE_BASE_POINT0 : SPELLVALUE_BASE_POINT1;
            GetCaster()->CastCustomSpell(SPELL_SHAMAN_SPIRIT_LINK, mod, value, target, true);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_spirit_link_SpellScript::SelectTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
            OnHit += SpellHitFn(spell_sha_spirit_link_SpellScript::HandleHit);
        }

        float _averageHealthPct;
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_spirit_link_SpellScript();
    }
};

// 79206 - Spiritwalker's Grace
class spell_sha_spiritwalkers_grace : public SpellScriptLoader
{
public:
    spell_sha_spiritwalkers_grace() : SpellScriptLoader("spell_sha_spiritwalkers_grace") { }

    class spell_sha_spiritwalkers_grace_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_spiritwalkers_grace_SpellScript);

        void HandleCast()
        {
            if (GetCaster()->HasAura(SPELL_SHAMAN_SPIRITWALKERS_AEGIS))
                GetCaster()->CastSpell(GetCaster(), SPELL_SHAMAN_SPIRITWALKERS_AEGIS_EFFECT, true);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_sha_spiritwalkers_grace_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_spiritwalkers_grace_SpellScript();
    }
};

// 114893 - Stone Bulwark
class spell_sha_stone_bulwark : public SpellScriptLoader
{
public:
    spell_sha_stone_bulwark() : SpellScriptLoader("spell_sha_stone_bulwark") { }

    class spell_sha_stone_bulwark_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_stone_bulwark_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (AuraEffect* triggerEffect = caster->GetAuraEffect(SPELL_SHAMAN_STONE_BULWARK_TOTEM_PASSIVE, EFFECT_0))
            {
                amount = triggerEffect->GetAmount();
                if (triggerEffect->GetTickNumber() == 1)
                    amount *= 4;
            }
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_stone_bulwark_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_stone_bulwark_AuraScript();
    }
};

// 114889 - Stone Bulwark Totem Passive
class spell_sha_stone_bulwark_totem_passive : public SpellScriptLoader
{
public:
    spell_sha_stone_bulwark_totem_passive() : SpellScriptLoader("spell_sha_stone_bulwark_totem_passive") { }

    class spell_sha_stone_bulwark_totem_passive_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_stone_bulwark_totem_passive_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (Unit* owner = caster->GetOwner())
            {
                float ap = owner->GetTotalAttackPowerValue(WeaponAttackType::BASE_ATTACK);
                float sp = float(owner->SpellBaseDamageBonusDone(GetSpellInfo()->GetSchoolMask()));
                amount = int32(std::max(0.625f * ap, sp) * 0.875f);
            }
        }

        void HandlePeriodic(AuraEffect const* aurEff)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (Unit* owner = caster->GetOwner())
                if (AuraEffect* shield = owner->GetAuraEffect(SPELL_SHAMAN_STONE_BULWARK_ABSORB, EFFECT_0, GetCasterGUID()))
                    if (shield->GetAmount() >= aurEff->GetAmount() && shield->GetBase()->GetDuration() > 2000)
                        PreventDefaultAction();
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_stone_bulwark_totem_passive_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_stone_bulwark_totem_passive_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_stone_bulwark_totem_passive_AuraScript();
    }
};

// 17364 - Stormstrike
class spell_sha_stormstrike : public SpellScriptLoader
{
public:
    spell_sha_stormstrike() : SpellScriptLoader("spell_sha_stormstrike") { }

    class spell_sha_stormstrike_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_stormstrike_SpellScript);

        void CheckWeapon(SpellEffIndex effIndex)
        {
            if (Player* player = GetCaster()->ToPlayer())
                if (!player->GetWeaponForAttack(WeaponAttackType::OFF_ATTACK, true))
                    PreventHitEffect(effIndex);
        }

        void Register() OVERRIDE
        {
            OnEffectLaunchTarget += SpellEffectFn(spell_sha_stormstrike_SpellScript::CheckWeapon, EFFECT_2, SPELL_EFFECT_TRIGGER_SPELL);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_stormstrike_SpellScript();
    }
};

// 51505 - Lava Burst
class spell_sha_t15_elemental_4p_bonus : public SpellScriptLoader
{
public:
    spell_sha_t15_elemental_4p_bonus() : SpellScriptLoader("spell_sha_t15_elemental_4p_bonus") { }

    class spell_sha_t15_elemental_4p_bonus_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_t15_elemental_4p_bonus_SpellScript);

        void HandleCast()
        {
            if (Player* player = GetCaster()->ToPlayer())
                if (AuraEffect const* setBonus = player->GetAuraEffect(SPELL_SHAMAN_ITEM_T15_ELEMENTAL_4P, EFFECT_0))
                    player->ModifySpellCooldown(SPELL_SHAMAN_ASCENDANCE, -setBonus->GetAmount() * IN_MILLISECONDS);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_sha_t15_elemental_4p_bonus_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_t15_elemental_4p_bonus_SpellScript();
    }
};

// 17364 - Stormstrike
// 115356 - Stormblast
class spell_sha_t15_enhancement_2p_bonus : public SpellScriptLoader
{
public:
    spell_sha_t15_enhancement_2p_bonus() : SpellScriptLoader("spell_sha_t15_enhancement_2p_bonus") { }

    class spell_sha_t15_enhancement_2p_bonus_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_t15_enhancement_2p_bonus_SpellScript);

        void HandleCast()
        {
            Unit* caster = GetCaster();
            if (!caster->HasAura(SPELL_SHAMAN_ITEM_T15_ENHANCEMENT_2P))
                return;

            caster->CastSpell(caster, SPELL_SHAMAN_MAELSTROM_WEAPON, true);
            caster->CastSpell(caster, SPELL_SHAMAN_MAELSTROM_WEAPON, true);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_sha_t15_enhancement_2p_bonus_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_t15_enhancement_2p_bonus_SpellScript();
    }
};

// 144998 - Item - Shaman T16 Elemental 2P Bonus
class spell_sha_t16_elemental_2p_bonus : public SpellScriptLoader
{
public:
    spell_sha_t16_elemental_2p_bonus() : SpellScriptLoader("spell_sha_t16_elemental_2p_bonus") { }

    class spell_sha_t16_elemental_2p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_t16_elemental_2p_bonus_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_SHAMAN_ELEMENTAL_DISCHARGE) != NULL;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (!eventInfo.GetSpellInfo() || !eventInfo.GetActionTarget())
                return false;
            if (eventInfo.GetSpellInfo()->Id != SPELL_SHAMAN_FULMINATION_TRIGGERED)
                return false;

            Aura* lightningShield = GetUnitOwner()->GetAura(SPELL_SHAMAN_LIGHTNING_SHIELD);
            return lightningShield && lightningShield->GetCharges() > 1;
        }

        void HandleProc(ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            Unit* shaman = GetUnitOwner();
            Aura* lightningShield = shaman->GetAura(SPELL_SHAMAN_LIGHTNING_SHIELD);
            if (!lightningShield)
                return;

            int32 duration = 2 * (lightningShield->GetCharges() - 1) * IN_MILLISECONDS;
            Unit* target = eventInfo.GetActionTarget();
            shaman->CastSpell(target, SPELL_SHAMAN_ELEMENTAL_DISCHARGE, true, NULL, GetEffect(EFFECT_0));
            if (Aura* discharge = target->GetAura(SPELL_SHAMAN_ELEMENTAL_DISCHARGE, shaman->GetGUID()))
            {
                discharge->SetMaxDuration(duration);
                discharge->SetDuration(duration);
            }
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_sha_t16_elemental_2p_bonus_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_sha_t16_elemental_2p_bonus_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_t16_elemental_2p_bonus_AuraScript();
    }
};

// 144966 - Item - Shaman T16 Enhancement 4P Bonus
class spell_sha_t16_enhancement_4p_bonus : public SpellScriptLoader
{
public:
    spell_sha_t16_enhancement_4p_bonus() : SpellScriptLoader("spell_sha_t16_enhancement_4p_bonus") { }

    class spell_sha_t16_enhancement_4p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_t16_enhancement_4p_bonus_AuraScript);

        void HandleProc(ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();

            if (Player* player = GetUnitOwner()->ToPlayer())
            {
                player->RemoveSpellCooldown(SPELL_SHAMAN_LAVA_LASH, true);
                player->CastCustomSpell(SPELL_SHAMAN_SEARING_FLAMES, SPELLVALUE_AURA_STACK, 5, player, true);
            }
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_sha_t16_enhancement_4p_bonus_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_t16_enhancement_4p_bonus_AuraScript();
    }
};

// 379 - Earth Shield
class spell_sha_t16_restoration_2p_bonus : public SpellScriptLoader
{
public:
    spell_sha_t16_restoration_2p_bonus() : SpellScriptLoader("spell_sha_t16_restoration_2p_bonus") { }

    class spell_sha_t16_restoration_2p_bonus_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_t16_restoration_2p_bonus_SpellScript);

        void HandleHit()
        {
            Unit* shaman = GetOriginalCaster();
            Unit* target = GetHitUnit();
            if (!shaman || !target)
                return;

            if (AuraEffect const* bonus = shaman->GetAuraEffect(SPELL_SHAMAN_ITEM_T16_RESTORATION_2P, EFFECT_0))
            {
                int32 absorb = CalculatePct(GetHitHeal(), bonus->GetAmount());
                if (absorb > 0)
                    shaman->CastCustomSpell(SPELL_SHAMAN_NATURES_BARRIER, SPELLVALUE_BASE_POINT0, absorb, target, true);
            }
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_sha_t16_restoration_2p_bonus_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_t16_restoration_2p_bonus_SpellScript();
    }
};

// 55453 - Glyph of Telluric Currents
class spell_sha_telluric_currents : public SpellScriptLoader
{
public:
    spell_sha_telluric_currents() : SpellScriptLoader("spell_sha_telluric_currents") { }

    class spell_sha_telluric_currents_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_telluric_currents_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SHAMAN_TELLURIC_CURRENTS))
                return false;
            return true;
        }

        void HandleEffectProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();

            Player* player = GetTarget()->ToPlayer();
            if (!player)
                return;

            float pct = player->GetTalentSpecialization(player->GetActiveSpec()) == TALENT_TREE_SHAMAN_ENHANCEMENT ? 10.0f : 2.0f;
            int32 basePoints0 = int32(CalculatePct(float(player->GetMaxPower(POWER_MANA)), pct));

            player->CastCustomSpell(player, SPELL_SHAMAN_TELLURIC_CURRENTS, &basePoints0, NULL, NULL, true);
        }

        void Register() OVERRIDE
        {
            OnEffectProc += AuraEffectProcFn(spell_sha_telluric_currents_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_telluric_currents_AuraScript();
    }
};

// 51490 - Thunderstorm
class spell_sha_thunderstorm : public SpellScriptLoader
{
public:
    spell_sha_thunderstorm() : SpellScriptLoader("spell_sha_thunderstorm") { }

    class spell_sha_thunderstorm_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_thunderstorm_SpellScript);

        void HandleKnockBack(SpellEffIndex effIndex)
        {
            // Glyph of Thunderstorm
            if (GetCaster()->HasAura(SPELL_SHAMAN_GLYPH_OF_THUNDERSTORM))
                PreventHitDefaultEffect(effIndex);
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_sha_thunderstorm_SpellScript::HandleKnockBack, EFFECT_1, SPELL_EFFECT_KNOCK_BACK);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_thunderstorm_SpellScript();
    }
};

// 51564 - Tidal Waves
class spell_sha_tidal_waves : public SpellScriptLoader
{
public:
    spell_sha_tidal_waves() : SpellScriptLoader("spell_sha_tidal_waves") { }

    class spell_sha_tidal_waves_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_tidal_waves_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SHAMAN_TIDAL_WAVES))
                return false;
            return true;
        }

        void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            int32 amount = aurEff->GetAmount();
            if (AuraEffect const* setBonus = GetTarget()->GetAuraEffect(SPELL_SHAMAN_ITEM_T14_RESTORATION_4P, EFFECT_0))
                amount += setBonus->GetAmount();

            int32 basePoints0 = -amount;
            int32 basePoints1 = amount;

            GetTarget()->CastCustomSpell(GetTarget(), SPELL_SHAMAN_TIDAL_WAVES, &basePoints0, &basePoints1, NULL, true, NULL, aurEff);
        }

        void Register() OVERRIDE
        {
            OnEffectProc += AuraEffectProcFn(spell_sha_tidal_waves_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_tidal_waves_AuraScript();
    }
};

// 108287 - Totemic Projection
class spell_sha_totemic_projection : public SpellScriptLoader
{
public:
    spell_sha_totemic_projection() : SpellScriptLoader("spell_sha_totemic_projection") { }

    class spell_sha_totemic_projection_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_totemic_projection_SpellScript);

        void MoveTotems(SpellEffIndex /*effIndex*/)
        {
            Unit* shaman = GetCaster();
            WorldLocation* dest = GetHitDest();
            if (!dest)
                return;

            for (uint8 slot = SUMMON_SLOT_TOTEM; slot < MAX_TOTEM_SLOT; ++slot)
            {
                Creature* totem = ObjectAccessor::GetCreature(*shaman, shaman->m_SummonSlot[slot]);
                if (!totem || !totem->IsTotem())
                    continue;

                SpellInfo const* summonInfo = sSpellMgr->GetSpellInfo(totem->GetUInt32Value(UNIT_FIELD_CREATED_BY_SPELL));
                if (!summonInfo)
                    continue;

                float angle = summonInfo->Effects[EFFECT_0].TargetA.CalcDirectionAngle();
                float dist = std::max(summonInfo->Effects[EFFECT_0].CalcRadius(shaman), shaman->GetObjectSize());

                Position pos;
                pos.Relocate(*dest);
                totem->MovePositionToFirstCollision(pos, dist, angle);
                totem->NearTeleportTo(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), totem->GetOrientation());
            }
        }

        void Register() OVERRIDE
        {
            OnEffectHit += SpellEffectFn(spell_sha_totemic_projection_SpellScript::MoveTotems, EFFECT_1, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_totemic_projection_SpellScript();
    }
};

// 73680 - Unleash Elements
class spell_sha_unleash_elements : public SpellScriptLoader
{
public:
    spell_sha_unleash_elements() : SpellScriptLoader("spell_sha_unleash_elements") { }

    class spell_sha_unleash_elements_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_unleash_elements_SpellScript);

        SpellCastResult CheckEnchant()
        {
            Player* player = GetCaster()->ToPlayer();
            if (!player)
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;

            Unit* target = GetExplTargetUnit();
            if (!target)
                return SpellCastResult::SPELL_FAILED_BAD_TARGETS;

            Item* weapons[2];
            weapons[0] = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
            weapons[1] = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
            bool enchantFound = false;
            bool badTarget = false;

            for (Item* weapon : weapons)
            {
                if (!weapon)
                    continue;

                switch (weapon->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT))
                {
                    case SHAMAN_ENCHANT_EARTHLIVING:
                        if (player->IsValidAttackTarget(target))
                            badTarget = true;
                        else
                            enchantFound = true;
                        break;
                    case SHAMAN_ENCHANT_FLAMETONGUE:
                    case SHAMAN_ENCHANT_FROSTBRAND:
                    case SHAMAN_ENCHANT_ROCKBITER:
                    case SHAMAN_ENCHANT_WINDFURY:
                        if (!player->IsValidAttackTarget(target))
                            badTarget = true;
                        else
                            enchantFound = true;
                        break;
                    default:
                        break;
                }
            }

            if (badTarget)
                return SpellCastResult::SPELL_FAILED_BAD_TARGETS;

            if (!enchantFound)
                return SpellCastResult::SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void HandleHit()
        {
            Player* player = GetCaster()->ToPlayer();
            Unit* target = GetHitUnit();
            if (!player || !target)
                return;

            Item* mainHand = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
            Item* offHand = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

            uint32 mainHandEnchant = mainHand ? mainHand->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT) : 0;
            uint32 offHandEnchant = offHand ? offHand->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT) : 0;

            if (mainHand)
                ShamanUnleashElement(player, mainHandEnchant, target);

            // Two weapons with the same enchantment only unleash it once
            if (offHand && (!mainHand || offHandEnchant != mainHandEnchant))
                ShamanUnleashElement(player, offHandEnchant, target);
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_sha_unleash_elements_SpellScript::CheckEnchant);
            OnHit += SpellHitFn(spell_sha_unleash_elements_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_unleash_elements_SpellScript();
    }
};

// 73682 - Unleash Frost
class spell_sha_unleash_frost : public SpellScriptLoader
{
public:
    spell_sha_unleash_frost() : SpellScriptLoader("spell_sha_unleash_frost") { }

    class spell_sha_unleash_frost_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_unleash_frost_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            Unit::AuraApplicationMap const& auras = GetUnitOwner()->GetAppliedAuras();
            for (Unit::AuraApplicationMap::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
            {
                SpellInfo const* spellInfo = itr->second->GetBase()->GetSpellInfo();
                if ((spellInfo->SchoolMask & SPELL_SCHOOL_MASK_FROST) && (spellInfo->GetAllEffectsMechanicMask() & (1 << MECHANIC_SNARE)))
                {
                    amount = -70;
                    break;
                }
            }
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_unleash_frost_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_DECREASE_SPEED);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_unleash_frost_AuraScript();
    }
};

// 331 - Healing Wave
// 8004 - Healing Surge
// 61295 - Riptide
// 77472 - Greater Healing Wave
// 126371 - Deluge
class spell_sha_unleashed_fury_earthliving : public SpellScriptLoader
{
public:
    spell_sha_unleashed_fury_earthliving() : SpellScriptLoader("spell_sha_unleashed_fury_earthliving") { }

    class spell_sha_unleashed_fury_earthliving_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_unleashed_fury_earthliving_SpellScript);

        void HandleHit()
        {
            Unit* caster = GetCaster();
            if (AuraEffect* unleashedFury = caster->GetAuraEffect(SPELL_SHAMAN_UNLEASHED_FURY_EARTHLIVING, EFFECT_0))
            {
                int32 heal = GetHitHeal();
                AddPct(heal, unleashedFury->GetAmount());
                SetHitHeal(heal);
                caster->RemoveAura(unleashedFury->GetBase());
            }
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_sha_unleashed_fury_earthliving_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_unleashed_fury_earthliving_SpellScript();
    }
};

// 61295 - Riptide
class spell_sha_unleashed_fury_earthliving_riptide : public SpellScriptLoader
{
public:
    spell_sha_unleashed_fury_earthliving_riptide() : SpellScriptLoader("spell_sha_unleashed_fury_earthliving_riptide") { }

    class spell_sha_unleashed_fury_earthliving_riptide_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_unleashed_fury_earthliving_riptide_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* caster = GetCaster())
                if (AuraEffect* unleashedFury = caster->GetAuraEffect(SPELL_SHAMAN_UNLEASHED_FURY_EARTHLIVING, EFFECT_0))
                    AddPct(amount, unleashedFury->GetAmount());
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_unleashed_fury_earthliving_riptide_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_PERIODIC_HEAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_unleashed_fury_earthliving_riptide_AuraScript();
    }
};

// 118475 - Unleashed Fury
class spell_sha_unleashed_fury_rockbiter : public SpellScriptLoader
{
public:
    spell_sha_unleashed_fury_rockbiter() : SpellScriptLoader("spell_sha_unleashed_fury_rockbiter") { }

    class spell_sha_unleashed_fury_rockbiter_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_unleashed_fury_rockbiter_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            amount = -1;
        }

        void Absorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
        {
            absorbAmount = 0;
            if (dmgInfo.GetAttacker() && dmgInfo.GetAttacker()->HasAura(SPELL_SHAMAN_UNLEASH_EARTH, GetCasterGUID()))
                absorbAmount = CalculatePct(dmgInfo.GetDamage(), aurEff->GetBaseAmount());
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_unleashed_fury_rockbiter_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            OnEffectAbsorb += AuraEffectAbsorbFn(spell_sha_unleashed_fury_rockbiter_AuraScript::Absorb, EFFECT_0);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_unleashed_fury_rockbiter_AuraScript();
    }
};

// 114052 - Ascendance (Water)
class spell_sha_water_ascendant : public SpellScriptLoader
{
public:
    spell_sha_water_ascendant() : SpellScriptLoader("spell_sha_water_ascendant") { }

    class spell_sha_water_ascendant_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_water_ascendant_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_SHAMAN_RESTORATIVE_MISTS) != NULL;
        }

        bool Load() OVERRIDE
        {
            _amountHealed = 0;
            return true;
        }

        void HandleProc(ProcEventInfo& eventInfo)
        {
            // The transform effect would otherwise drop the aura on the first heal
            PreventDefaultAction();

            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            if (!spellInfo || !spellInfo->IsPositive() || !eventInfo.GetHealInfo())
                return;

            if (!(eventInfo.GetTypeMask() & (PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_POS | PROC_FLAG_DONE_SPELL_NONE_DMG_CLASS_POS | PROC_FLAG_DONE_PERIODIC)))
                return;

            _amountHealed += eventInfo.GetHealInfo()->GetHeal();
        }

        void HandlePeriodic(AuraEffect const* aurEff)
        {
            if (!_amountHealed)
                return;

            GetUnitOwner()->CastCustomSpell(SPELL_SHAMAN_RESTORATIVE_MISTS, SPELLVALUE_BASE_POINT0, int32(_amountHealed), GetUnitOwner(), true, NULL, aurEff);
            _amountHealed = 0;
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_sha_water_ascendant_AuraScript::HandleProc);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_water_ascendant_AuraScript::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        }

        uint32 _amountHealed;
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_water_ascendant_AuraScript();
    }
};

// 114049 - Ascendance
class spell_sha_ascendance : public SpellScriptLoader
{
public:
    spell_sha_ascendance() : SpellScriptLoader("spell_sha_ascendance") { }

    class spell_sha_ascendance_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_ascendance_SpellScript);

        SpellCastResult CheckRequirement()
        {
            if (Player* player = GetCaster()->ToPlayer())
            {
                switch (player->GetTalentSpecialization(player->GetActiveSpec()))
                {
                case TALENT_TREE_SHAMAN_ENHANCEMENT:
                case TALENT_TREE_SHAMAN_ELEMENTAL:
                case TALENT_TREE_SHAMAN_RESTORATION:
                    return SpellCastResult::SPELL_CAST_OK;
                default:
                    return SpellCastResult::SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
                }
            }

            return SpellCastResult::SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
        }

        void HandleOnHit()
        {
            if (Player* player = GetCaster()->ToPlayer())
            {
                switch (player->GetTalentSpecialization(player->GetActiveSpec()))
                {
                case TALENT_TREE_SHAMAN_ENHANCEMENT:
                    player->RemoveSpellCooldown(SPELL_SHAMAN_STORMSTRIKE, true);
                    player->RemoveSpellCooldown(SPELL_SHAMAN_STORMBLAST, true);
                    player->CastSpell(player, SPELL_SHAMAN_ASCENDANCE_ENHANCEMENT, true);
                    break;
                case TALENT_TREE_SHAMAN_ELEMENTAL:
                    player->RemoveSpellCooldown(SPELL_SHAMAN_LAVA_BURST, true);
                    player->CastSpell(player, SPELL_SHAMAN_ASCENDANCE_ELEMENTAL, true);
                    break;
                case TALENT_TREE_SHAMAN_RESTORATION:
                    player->CastSpell(player, SPELL_SHAMAN_ASCENDANCE_RESTORATION, true);
                    break;
                default:
                    break;
                }
            }
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_sha_ascendance_SpellScript::CheckRequirement);
            OnHit += SpellHitFn(spell_sha_ascendance_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_ascendance_SpellScript();
    }
};

// 16246 - Clearcasting
class spell_sha_clearcasting : public SpellScriptLoader
{
public:
    spell_sha_clearcasting() : SpellScriptLoader("spell_sha_clearcasting") { }

    class spell_sha_clearcasting_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_clearcasting_AuraScript);

        bool Load() OVERRIDE
        {
            _lastProcSpell = NULL;
            _lastProcTime = 0;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            Spell const* procSpell = eventInfo.TriggeredBySpell();
            if (!procSpell || procSpell->GetPowerType() != POWER_MANA || procSpell->GetPowerCost() <= 0)
                return false;

            // A spell hitting several targets only consumes one charge
            Unit* primaryTarget = procSpell->m_targets.GetUnitTarget();
            if (primaryTarget && primaryTarget != eventInfo.GetActionTarget())
                return false;

            uint32 now = getMSTime();
            if (procSpell == _lastProcSpell && now == _lastProcTime)
                return false;

            _lastProcSpell = procSpell;
            _lastProcTime = now;
            return true;
        }

        void CalculateSpellMod(AuraEffect const* /*aurEff*/, SpellModifier*& spellMod)
        {
            if (spellMod)
                spellMod->mask[0] |= 0x40; // Healing Wave
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_sha_clearcasting_AuraScript::CheckProc);
            DoEffectCalcSpellMod += AuraEffectCalcSpellModFn(spell_sha_clearcasting_AuraScript::CalculateSpellMod, EFFECT_3, SPELL_AURA_ADD_PCT_MODIFIER);
        }

    private:
        Spell const* _lastProcSpell;
        uint32 _lastProcTime;
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_clearcasting_AuraScript();
    }
};

// 108283 - Echo of the Elements
class spell_sha_echo_of_the_elements : public SpellScriptLoader
{
public:
    spell_sha_echo_of_the_elements() : SpellScriptLoader("spell_sha_echo_of_the_elements") { }

    class spell_sha_echo_of_the_elements_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_echo_of_the_elements_AuraScript);

        bool Load() OVERRIDE
        {
            return GetUnitOwner()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            Spell const* procSpell = eventInfo.TriggeredBySpell();
            Unit* target = eventInfo.GetActionTarget();
            if (!procSpell || !target || (eventInfo.GetTypeMask() & PROC_FLAG_DONE_PERIODIC))
                return false;

            SpellInfo const* spellInfo = procSpell->GetSpellInfo();
            if (spellInfo->SpellFamilyName != SPELLFAMILY_SHAMAN)
                return false;

            if (procSpell->IsTriggered() && !(spellInfo->SpellFamilyFlags[2] & 0x00008000))
                return false;

            // Area spells and the side hits of chain spells don't echo
            if (procSpell->m_targets.GetUnitTarget() != target)
                return false;

            return !GetUnitOwner()->ToPlayer()->HasSpellCooldown(GetId());
        }

        void HandleProc(ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            Player* player = GetUnitOwner()->ToPlayer();
            Unit* target = eventInfo.GetActionTarget();
            SpellInfo const* spellInfo = eventInfo.TriggeredBySpell()->GetSpellInfo();

            int32 chance = 6;
            if (player->GetTalentSpecialization(player->GetActiveSpec()) == TALENT_TREE_SHAMAN_ENHANCEMENT && spellInfo->Id != SPELL_SHAMAN_ELEMENTAL_BLAST)
                chance = 30;

            if (!roll_chance_i(chance))
                return;

            if (target->GetAffectingPlayer() && player->IsHostileTo(target))
                player->AddSpellCooldown(GetId(), 0, time(NULL) + 4);

            player->CastSpell(target, spellInfo->Id, true, NULL, GetEffect(EFFECT_0));
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_sha_echo_of_the_elements_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_sha_echo_of_the_elements_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_echo_of_the_elements_AuraScript();
    }
};

// 52007 - Earthliving Weapon (Passive)
class spell_sha_earthliving_weapon : public SpellScriptLoader
{
public:
    spell_sha_earthliving_weapon() : SpellScriptLoader("spell_sha_earthliving_weapon") { }

    class spell_sha_earthliving_weapon_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_earthliving_weapon_AuraScript);

        void HandleProc(ProcEventInfo& eventInfo)
        {
            Unit* target = eventInfo.GetActionTarget();
            if (target && !target->HasAuraState(AURA_STATE_HEALTHLESS_35_PERCENT) && !roll_chance_i(20))
                PreventDefaultAction();
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_sha_earthliving_weapon_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_earthliving_weapon_AuraScript();
    }
};

// 34967 - Elemental Totem
class spell_sha_elemental_totem : public SpellScriptLoader
{
public:
    spell_sha_elemental_totem() : SpellScriptLoader("spell_sha_elemental_totem") { }

    class spell_sha_elemental_totem_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_elemental_totem_AuraScript);

        Unit* GetShaman()
        {
            if (TempSummon* summon = GetUnitOwner()->ToTempSummon())
                return summon->GetSummoner();
            return NULL;
        }

        void CalculateCritBonus(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* shaman = GetShaman())
                if (AuraEffect const* elementalFury = shaman->GetAuraEffect(SPELL_SHAMAN_ELEMENTAL_FURY, EFFECT_0))
                    amount = elementalFury->GetAmount();
        }

        void CalculateDamageBonus(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* shaman = GetShaman())
                if (AuraEffect const* mastery = shaman->GetAuraEffect(SPELL_SHAMAN_MASTERY_ENHANCED_ELEMENTS, EFFECT_0))
                    amount = mastery->GetAmount();
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_elemental_totem_AuraScript::CalculateCritBonus, EFFECT_1, SPELL_AURA_MOD_CRIT_DAMAGE_BONUS);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_elemental_totem_AuraScript::CalculateDamageBonus, EFFECT_2, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_elemental_totem_AuraScript();
    }
};

// 2062 - Earth Elemental Totem
// 2894 - Fire Elemental Totem
class spell_sha_elemental_totem_summon : public SpellScriptLoader
{
public:
    spell_sha_elemental_totem_summon() : SpellScriptLoader("spell_sha_elemental_totem_summon") { }

    class spell_sha_elemental_totem_summon_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_elemental_totem_summon_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_SHAMAN_FIRE_ELEMENTAL_SUMMON) && sSpellMgr->GetSpellInfo(SPELL_SHAMAN_EARTH_ELEMENTAL_SUMMON)
                && sSpellMgr->GetSpellInfo(SPELL_SHAMAN_PRIMAL_FIRE_ELEMENTAL_SUMMON) && sSpellMgr->GetSpellInfo(SPELL_SHAMAN_PRIMAL_EARTH_ELEMENTAL_SUMMON);
        }

        void HandleCast()
        {
            Unit* caster = GetCaster();
            bool primal = caster->HasAura(SPELL_SHAMAN_PRIMAL_ELEMENTALIST);
            uint32 spellId;
            if (GetSpellInfo()->Id == SPELL_SHAMAN_FIRE_ELEMENTAL_TOTEM)
                spellId = primal ? SPELL_SHAMAN_PRIMAL_FIRE_ELEMENTAL_SUMMON : SPELL_SHAMAN_FIRE_ELEMENTAL_SUMMON;
            else
                spellId = primal ? SPELL_SHAMAN_PRIMAL_EARTH_ELEMENTAL_SUMMON : SPELL_SHAMAN_EARTH_ELEMENTAL_SUMMON;

            caster->CastSpell(caster, spellId, true);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_sha_elemental_totem_summon_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_elemental_totem_summon_SpellScript();
    }
};

// 77223 - Mastery: Enhanced Elements
class spell_sha_enhanced_elements : public SpellScriptLoader
{
public:
    spell_sha_enhanced_elements() : SpellScriptLoader("spell_sha_enhanced_elements") { }

    class spell_sha_enhanced_elements_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_enhanced_elements_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            static uint32 const elementalEntries[] =
            {
                NPC_SHAMAN_FIRE_ELEMENTAL, NPC_SHAMAN_EARTH_ELEMENTAL, NPC_SHAMAN_PRIMAL_FIRE_ELEMENTAL, NPC_SHAMAN_PRIMAL_EARTH_ELEMENTAL
            };

            std::list<TempSummon*> elementals;
            for (uint32 entry : elementalEntries)
                GetUnitOwner()->GetSummons(elementals, entry);

            for (TempSummon* elemental : elementals)
                if (Aura* aura = elemental->GetAura(SPELL_SHAMAN_ELEMENTAL_TOTEM))
                    aura->RecalculateAmountOfEffects();
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_sha_enhanced_elements_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_enhanced_elements_AuraScript();
    }
};

// 147781 - Glyph of Eternal Earth
class spell_sha_glyph_of_eternal_earth : public SpellScriptLoader
{
public:
    spell_sha_glyph_of_eternal_earth() : SpellScriptLoader("spell_sha_glyph_of_eternal_earth") { }

    class spell_sha_glyph_of_eternal_earth_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_glyph_of_eternal_earth_AuraScript);

        void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();

            if (!roll_chance_i(15))
                return;

            if (Unit::AuraList const* earthShields = GetUnitOwner()->GetBoundAurasBySpellId(SPELL_SHAMAN_EARTH_SHIELD))
            {
                Aura* earthShield = earthShields->front();
                if (earthShield->GetCharges() < aurEff->GetAmount())
                    earthShield->ModCharges(1);
            }
        }

        void Register() OVERRIDE
        {
            OnEffectProc += AuraEffectProcFn(spell_sha_glyph_of_eternal_earth_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_glyph_of_eternal_earth_AuraScript();
    }
};

// 58057 - Glyph of Totemic Encirclement
class spell_sha_glyph_of_totemic_encirclement : public SpellScriptLoader
{
public:
    spell_sha_glyph_of_totemic_encirclement() : SpellScriptLoader("spell_sha_glyph_of_totemic_encirclement") { }

    class spell_sha_glyph_of_totemic_encirclement_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_glyph_of_totemic_encirclement_AuraScript);

        void HandleEffectProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();

            // The totem that triggered the proc only occupies its slot once the cast has finished
            Unit* shaman = GetUnitOwner();
            shaman->m_Events.Schedule(100, [shaman]()
            {
                static uint32 const totemSpells[MAX_TOTEM_SLOT - SUMMON_SLOT_TOTEM] =
                {
                    SPELL_SHAMAN_TOTEMIC_ENCIRCLEMENT_FIRE, SPELL_SHAMAN_TOTEMIC_ENCIRCLEMENT_EARTH,
                    SPELL_SHAMAN_TOTEMIC_ENCIRCLEMENT_WATER, SPELL_SHAMAN_TOTEMIC_ENCIRCLEMENT_AIR
                };

                for (uint8 slot = SUMMON_SLOT_TOTEM; slot < MAX_TOTEM_SLOT; ++slot)
                    if (!shaman->m_SummonSlot[slot])
                        shaman->CastSpell(shaman, totemSpells[slot - SUMMON_SLOT_TOTEM], true);
            });
        }

        void Register() OVERRIDE
        {
            OnEffectProc += AuraEffectProcFn(spell_sha_glyph_of_totemic_encirclement_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_glyph_of_totemic_encirclement_AuraScript();
    }
};

// 8178 - Grounding Totem Effect
// 89523 - Grounding Totem
class spell_sha_grounding_totem_effect : public SpellScriptLoader
{
public:
    spell_sha_grounding_totem_effect() : SpellScriptLoader("spell_sha_grounding_totem_effect") { }

    class spell_sha_grounding_totem_effect_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_grounding_totem_effect_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            // Redirection drops the charge when the spell target is chosen
            if (GetId() == SPELL_SHAMAN_GROUNDING_TOTEM_EFFECT)
                return false;

            return (eventInfo.GetHitMask() & PROC_EX_REFLECT) != 0;
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE || GetTarget() != GetUnitOwner())
                return;

            if (GetUnitOwner()->IsTotem())
                GetUnitOwner()->ToTempSummon()->UnSummon(1);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_sha_grounding_totem_effect_AuraScript::CheckProc);
            AfterEffectRemove += AuraEffectRemoveFn(spell_sha_grounding_totem_effect_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_grounding_totem_effect_AuraScript();
    }
};

// 108270 - Stone Bulwark Totem
// 108280 - Healing Tide Totem
class spell_sha_hp_based_totem : public SpellScriptLoader
{
public:
    spell_sha_hp_based_totem() : SpellScriptLoader("spell_sha_hp_based_totem") { }

    class spell_sha_hp_based_totem_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_hp_based_totem_SpellScript);

        void CalculateHealth(SpellEffIndex /*effIndex*/)
        {
            SetEffectValue(int32(GetCaster()->CountPctFromMaxHealth(10)));
        }

        void Register() OVERRIDE
        {
            OnEffectHit += SpellEffectFn(spell_sha_hp_based_totem_SpellScript::CalculateHealth, EFFECT_0, SPELL_EFFECT_SUMMON);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_sha_hp_based_totem_SpellScript();
    }
};

// 117013 - Primal Elementalist
class spell_sha_primal_elementalist : public SpellScriptLoader
{
public:
    spell_sha_primal_elementalist() : SpellScriptLoader("spell_sha_primal_elementalist") { }

    class spell_sha_primal_elementalist_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_primal_elementalist_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_SHAMAN_FIRE_ELEMENTAL_TOTEM) && sSpellMgr->GetSpellInfo(SPELL_SHAMAN_EARTH_ELEMENTAL_TOTEM);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            static uint32 const totemSpells[] = { SPELL_SHAMAN_FIRE_ELEMENTAL_TOTEM, SPELL_SHAMAN_EARTH_ELEMENTAL_TOTEM };

            std::list<TempSummon*> summons;
            for (uint32 spellId : totemSpells)
                GetUnitOwner()->GetSummons(summons, uint32(sSpellMgr->GetSpellInfo(spellId)->Effects[EFFECT_0].MiscValue));

            for (TempSummon* summon : summons)
                summon->UnSummon();
        }

        void Register() OVERRIDE
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_sha_primal_elementalist_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_primal_elementalist_AuraScript();
    }
};

// 51527 - Static Shock
class spell_sha_static_shock : public SpellScriptLoader
{
public:
    spell_sha_static_shock() : SpellScriptLoader("spell_sha_static_shock") { }

    class spell_sha_static_shock_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_static_shock_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (!eventInfo.GetActionTarget() || !GetUnitOwner()->HasAura(SPELL_SHAMAN_LIGHTNING_SHIELD))
                return false;

            if (eventInfo.GetTypeMask() & PROC_FLAG_DONE_MELEE_AUTO_ATTACK)
                return GetUnitOwner()->HasAura(SPELL_SHAMAN_UNLEASHED_FURY_WINDFURY);

            return true;
        }

        void HandleProc(ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            GetUnitOwner()->CastSpell(eventInfo.GetActionTarget(), SPELL_SHAMAN_LIGHTNING_SHIELD_DAMAGE, true);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_sha_static_shock_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_sha_static_shock_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_static_shock_AuraScript();
    }
};

// 120676 - Stormlash Totem
class spell_sha_stormlash_totem : public SpellScriptLoader
{
public:
    spell_sha_stormlash_totem() : SpellScriptLoader("spell_sha_stormlash_totem") { }

    class spell_sha_stormlash_totem_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_stormlash_totem_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            Unit* actor = eventInfo.GetActor();
            if (!actor || !eventInfo.GetProcTarget())
                return false;

            // Periodic damage only triggers it for channeled spells (Mind Flay, Malefic Grasp, Drain Soul)
            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            if ((eventInfo.GetTypeMask() & PROC_FLAG_DONE_PERIODIC) && (!spellInfo || !spellInfo->IsChanneled()))
                return false;

            if (actor->IsPet() && actor->ToPet()->isTemporarySummoned())
                return false;

            return actor->GetTypeId() == TypeID::TYPEID_PLAYER || actor->IsPet() || actor->HasUnitTypeMask(UNIT_MASK_CONTROLABLE_GUARDIAN);
        }

        void HandleProc(ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            Unit* actor = eventInfo.GetActor();
            Player* player = actor->GetAffectingPlayer();
            if (!player)
                return;

            WeaponAttackType attack = actor->getClass() == CLASS_HUNTER ? WeaponAttackType::RANGED_ATTACK : WeaponAttackType::BASE_ATTACK;
            if (eventInfo.GetTypeMask() & PROC_FLAG_DONE_OFFHAND_ATTACK)
                attack = WeaponAttackType::OFF_ATTACK;

            float spellPowerDamage = actor->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_ALL) * 0.6f;
            float attackPowerDamage = actor->GetTotalAttackPowerValue(attack) * 0.4f;
            if (actor->GetEntry() == NPC_SHAMAN_FIRE_ELEMENTAL || actor->GetEntry() == NPC_SHAMAN_PRIMAL_FIRE_ELEMENTAL)
                attackPowerDamage = 0.0f;

            float damage = std::max(spellPowerDamage, attackPowerDamage);

            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            if (spellInfo && !spellInfo->IsAutoRepeatRangedSpell())
            {
                uint32 castTime = spellInfo->CalcCastTime(actor->getLevel());
                if (castTime > 1500)
                    damage *= castTime / 1500.0f;

                switch (spellInfo->Id)
                {
                    case SPELL_SHAMAN_LIGHTNING_BOLT:
                    case SPELL_SHAMAN_LAVA_BURST:
                    case SPELL_WARLOCK_DRAIN_SOUL:
                        damage *= 2.0f;
                        break;
                    case SPELL_ROGUE_SINISTER_STRIKE:
                        damage *= 0.5f;
                        break;
                    default:
                        break;
                }
            }
            else
            {
                damage *= 0.4f;

                uint32 attackTimer = BASE_ATTACK_TIME;
                if (actor == player)
                {
                    if (Item* item = player->GetWeaponForAttack(attack))
                        attackTimer = item->GetTemplate()->Delay;
                }
                else if (Creature* creature = actor->ToCreature())
                    attackTimer = creature->GetCreatureTemplate()->baseattacktime;

                damage *= attackTimer / 1000.0f / 2.6f;

                if (attack == WeaponAttackType::OFF_ATTACK)
                    damage *= 0.5f;
            }

            if (actor != player)
                damage *= 0.2f;

            float delta = CalculatePct(damage, 15);
            damage += frand(-delta, delta);
            actor->CastCustomSpell(SPELL_SHAMAN_STORMLASH, SPELLVALUE_BASE_POINT0, int32(damage), eventInfo.GetProcTarget(), true);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_sha_stormlash_totem_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_sha_stormlash_totem_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_stormlash_totem_AuraScript();
    }
};

// 33757 - Windfury Weapon (Passive)
class spell_sha_windfury_weapon : public SpellScriptLoader
{
public:
    spell_sha_windfury_weapon() : SpellScriptLoader("spell_sha_windfury_weapon") { }

    class spell_sha_windfury_weapon_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_windfury_weapon_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_SHAMAN_WINDFURY_WEAPON) != NULL;
        }

        bool Load() OVERRIDE
        {
            return GetUnitOwner()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            Player* player = GetUnitOwner()->ToPlayer();
            Unit* target = eventInfo.GetActionTarget();
            if (!target || !target->IsAlive() || player->HasSpellCooldown(GetId()))
                return false;

            Item* item = NULL;
            if (eventInfo.GetTypeMask() & PROC_FLAG_DONE_MAINHAND_ATTACK)
                item = player->GetWeaponForAttack(WeaponAttackType::BASE_ATTACK);
            else if (eventInfo.GetTypeMask() & PROC_FLAG_DONE_OFFHAND_ATTACK)
                item = player->GetWeaponForAttack(WeaponAttackType::OFF_ATTACK);

            return item && item->GetGUID() == GetAura()->GetCastItemGUID();
        }

        void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            Player* player = GetUnitOwner()->ToPlayer();
            uint32 chance = 20;
            player->ApplySpellMod(GetId(), SPELLMOD_CHANCE_OF_SUCCESS, chance);
            if (!roll_chance_i(chance))
                return;

            player->AddSpellCooldown(GetId(), 0, time(NULL) + 3);

            bool offHand = (eventInfo.GetTypeMask() & PROC_FLAG_DONE_OFFHAND_ATTACK) != 0;
            WeaponAttackType attType = offHand ? WeaponAttackType::OFF_ATTACK : WeaponAttackType::BASE_ATTACK;
            Unit* target = eventInfo.GetActionTarget();

            int32 extraAttackPower = player->CalculateSpellDamage(target, sSpellMgr->GetSpellInfo(SPELL_SHAMAN_WINDFURY_WEAPON), EFFECT_1);
            int32 extraDamage = int32(extraAttackPower / 14.0f * player->GetAttackTime(attType) / 1000.0f);
            uint32 attackSpell = offHand ? SPELL_SHAMAN_WINDFURY_ATTACK_OFFHAND : SPELL_SHAMAN_WINDFURY_ATTACK_MAINHAND;
            Item* castItem = player->GetItemByGuid(GetAura()->GetCastItemGUID());

            for (uint8 i = 0; i < 3; ++i)
                player->CastCustomSpell(attackSpell, SPELLVALUE_BASE_POINT0, extraDamage, target, true, castItem, aurEff);

            if (AuraEffect const* setBonus = player->GetAuraEffect(SPELL_SHAMAN_ITEM_T15_ENHANCEMENT_4P, EFFECT_0))
                player->ModifySpellCooldown(SPELL_SHAMAN_FERAL_SPIRIT, -setBonus->GetAmount() * IN_MILLISECONDS);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_sha_windfury_weapon_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_sha_windfury_weapon_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_windfury_weapon_AuraScript();
    }
};

void AddSC_shaman_spell_scripts()
{
    new spell_sha_ancestral_awakening();
    new spell_sha_ancestral_awakening_proc();
    new spell_sha_ancestral_guidance();
    new spell_sha_ancestral_guidance_heal();
    new spell_sha_bloodlust();
    new spell_sha_call_of_the_elements();
    new spell_sha_chain_heal();
    new spell_sha_conductivity();
    new spell_sha_earth_shield();
    new spell_sha_earthgrab();
    new spell_sha_earthquake();
    new spell_sha_earthquake_tick();
    new spell_sha_elemental_blast();
    new spell_sha_fire_nova();
    new spell_sha_flow_of_the_elements();
    new spell_sha_fulmination();
    new spell_sha_glyph_of_cleansing_waters();
    new spell_sha_glyph_of_flame_shock();
    new spell_sha_glyph_of_healing_wave();
    new spell_sha_glyph_of_lakestrider();
    new spell_sha_glyph_of_purging();
    new spell_sha_glyph_of_shamanistic_rage();
    new spell_sha_healing_rain();
    new spell_sha_healing_stream();
    new spell_sha_heroism();
    new spell_sha_hex();
    new spell_sha_item_lightning_shield();
    new spell_sha_item_lightning_shield_trigger();
    new spell_sha_item_mana_surge();
    new spell_sha_lightning_shield();
    new spell_sha_lightning_strike();
    new spell_sha_lava_burst();
    new spell_sha_lava_lash();
    new spell_sha_lava_lash_spread_flame_shock();
    new spell_sha_lava_surge();
    new spell_sha_lava_surge_cooldown_fix();
    new spell_sha_lava_surge_proc();
    new spell_sha_maelstrom_weapon();
    new spell_sha_mana_tide_totem();
    new spell_sha_nature_guardian();
    new spell_sha_prowl();
    new spell_sha_purification();
    new spell_sha_restorative_mists();
    new spell_sha_resurgence();
    new spell_sha_rolling_thunder();
    new spell_sha_spirit_link();
    new spell_sha_spiritwalkers_grace();
    new spell_sha_stone_bulwark();
    new spell_sha_stone_bulwark_totem_passive();
    new spell_sha_stormstrike();
    new spell_sha_t15_elemental_4p_bonus();
    new spell_sha_t15_enhancement_2p_bonus();
    new spell_sha_t16_elemental_2p_bonus();
    new spell_sha_t16_enhancement_4p_bonus();
    new spell_sha_t16_restoration_2p_bonus();
    new spell_sha_telluric_currents();
    new spell_sha_thunderstorm();
    new spell_sha_tidal_waves();
    new spell_sha_totemic_projection();
    new spell_sha_unleash_elements();
    new spell_sha_unleash_frost();
    new spell_sha_unleashed_fury_earthliving();
    new spell_sha_unleashed_fury_earthliving_riptide();
    new spell_sha_unleashed_fury_rockbiter();
    new spell_sha_water_ascendant();
    new spell_sha_ascendance();
    new spell_sha_clearcasting();
    new spell_sha_echo_of_the_elements();
    new spell_sha_earthliving_weapon();
    new spell_sha_elemental_totem();
    new spell_sha_elemental_totem_summon();
    new spell_sha_enhanced_elements();
    new spell_sha_glyph_of_eternal_earth();
    new spell_sha_glyph_of_totemic_encirclement();
    new spell_sha_grounding_totem_effect();
    new spell_sha_hp_based_totem();
    new spell_sha_primal_elementalist();
    new spell_sha_static_shock();
    new spell_sha_stormlash_totem();
    new spell_sha_windfury_weapon();
}
