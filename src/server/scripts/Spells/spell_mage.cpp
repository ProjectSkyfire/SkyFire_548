/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/*
 * Scripts for spells with SPELLFAMILY_MAGE and SPELLFAMILY_GENERIC spells used by mage players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_mage_".
 */

#include "Containers.h"
#include "Player.h"
#include "GridNotifiers.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Pet.h"
#include "AreaTrigger.h"
#include "InstanceScript.h"
#include "ObjectAccessor.h"
#include "TemporarySummon.h"

enum MageSpells
{
    SPELL_MAGE_FROSTJAW                          = 102051,
    SPELL_MAGE_COLD_SNAP                         = 11958,
    SPELL_MAGE_FROST_NOVA                        = 122,
    SPELL_MAGE_SLOW                              = 31589,
    SPELL_MAGE_SQUIRREL_FORM                     = 32813,
    SPELL_MAGE_GIRAFFE_FORM                      = 32816,
    SPELL_MAGE_SERPENT_FORM                      = 32817,
    SPELL_MAGE_DRAGONHAWK_FORM                   = 32818,
    SPELL_MAGE_WORGEN_FORM                       = 32819,
    SPELL_MAGE_SHEEP_FORM                        = 32820,

    SPELL_MAGE_FLAMESTRIKE                       = 2120,

    SPELL_MAGE_RING_OF_FROST_SUMMON              = 113724, // MoP cast spell (was 82676 in Cata)
    SPELL_MAGE_RING_OF_FROST_FREEZE              = 82691,
    SPELL_MAGE_RING_OF_FROST_DUMMY               = 91264,
    SPELL_MAGE_RING_OF_FROST_TICK                = 136511, // periodic aura triggered by 113724
    SPELL_MAGE_RING_OF_FROST_PRESENCE_OF_MIND    = 140384,

    SPELL_MAGE_FINGERS_OF_FROST                  = 44544,

    // Time Warp - 80353
    SPELL_MAGE_TEMPORAL_DISPLACEMENT             = 80354,
    SPELL_SHAMAN_SATED                           = 57724,
    SPELL_HUNTER_INSANITY                        = 95809,

    // Pyroblast!
    SPELL_MAGE_PYROBLAST_CLEARCAST               = 48108,

    // Mirror Image
    SPELL_MAGE_GLYPH_OF_MIRROR_IMAGE             = 63093,
    SPELL_MAGE_SUMMON_IMAGES_FROST               = 58832,
    SPELL_MAGE_SUMMON_IMAGES_FIRE                = 88092,
    SPELL_MAGE_SUMMON_IMAGES_ARCANE              = 88091,

    // Fire
    SPELL_MAGE_HEATING_UP                        = 48107,
    SPELL_MAGE_PRESENCE_OF_MIND                  = 12043,
    SPELL_MAGE_PYROBLAST                         = 11366,
    SPELL_MAGE_IGNITE                            = 12654,
    SPELL_MAGE_INFERNO_BLAST                     = 108853,
    SPELL_MAGE_INFERNO_BLAST_IMPACT              = 118280,
    SPELL_MAGE_GLYPH_OF_INFERNO_BLAST            = 89926,
    SPELL_MAGE_COMBUSTION_DOT                    = 83853,
    SPELL_MAGE_COMBUSTION_IMPACT                 = 118271,
    SPELL_MAGE_GLYPH_OF_COMBUSTION               = 56368,
    SPELL_MAGE_BLAST_WAVE_SNARE                  = 11113,
    SPELL_MAGE_PYROMANIAC_AURA                   = 132209,
    SPELL_MAGE_PYROMANIAC_DAMAGE_DONE            = 132210,

    // Frost
    SPELL_MAGE_FROSTBOLT                         = 116,
    SPELL_MAGE_FROSTFIRE_BOLT                    = 44614,
    SPELL_MAGE_BLIZZARD                          = 42208,
    SPELL_MAGE_FINGERS_OF_FROST_PASSIVE          = 112965,
    SPELL_MAGE_FINGERS_OF_FROST_DOUBLE           = 126084,
    SPELL_MAGE_BRAIN_FREEZE                      = 44549,
    SPELL_MAGE_BRAIN_FREEZE_TRIGGERED            = 57761,
    SPELL_MAGE_MASTERY_ICICLES                   = 76613,
    SPELL_MAGE_ICICLE_STORE                      = 148012,
    SPELL_MAGE_ICICLE_VISUAL                     = 148017,
    SPELL_MAGE_ICICLE_DAMAGE                     = 148022,
    SPELL_MAGE_ICICLE_TRIGGER_AURA               = 148023,
    SPELL_MAGE_FROSTBOLT_HEAL                    = 126201,
    SPELL_MAGE_ICY_VEINS                         = 12472,
    SPELL_MAGE_ICY_VEINS_GLYPHED                 = 131078,
    SPELL_MAGE_BLIZZARD_SLOW                     = 12486,
    SPELL_MAGE_FROZEN_ORB_DAMAGE                 = 84721,
    SPELL_MAGE_FROST_BOMB                        = 112948,
    SPELL_MAGE_FROST_BOMB_TRIGGERED              = 113092,
    SPELL_MAGE_ICE_LANCE                         = 30455,
    SPELL_MAGE_WATERBOLT                         = 31707,
    SPELL_MAGE_FROSTBOLT_ICY_VEINS               = 131079,
    SPELL_MAGE_ICE_LANCE_ICY_VEINS               = 131080,
    SPELL_MAGE_FROSTFIRE_BOLT_ICY_VEINS          = 131081,
    SPELL_MAGE_WATERBOLT_ICY_VEINS               = 131581,

    // Arcane
    SPELL_MAGE_ARCANE_CHARGE                     = 36032,
    SPELL_MAGE_ARCANE_BARRAGE                    = 44425,
    SPELL_MAGE_ARCANE_MISSILES                   = 79683,
    SPELL_MAGE_ARCANE_MISSILES_DOUBLE            = 79808,
    SPELL_MAGE_ICE_FLOES                         = 108839,

    // Talents
    SPELL_MAGE_INVOCATION                        = 114003,
    SPELL_MAGE_INVOKERS_ENERGY                   = 116257,
    SPELL_MAGE_GLYPH_OF_EVOCATION                = 56380,
    SPELL_MAGE_INCANTERS_WARD_ENERGIZE           = 113842,
    SPELL_MAGE_INCANTERS_ABSORPTION              = 116267,
    SPELL_MAGE_INCANTERS_ABSORPTION_PASSIVE      = 118858,
    SPELL_MAGE_RUNE_OF_POWER                     = 116011,
    SPELL_MAGE_RUNE_OF_POWER_AURA                = 116014,
    SPELL_MAGE_NETHER_TEMPEST_SELECTOR           = 114924,
    SPELL_MAGE_NETHER_TEMPEST_DIRECT_DAMAGE      = 114954,
    SPELL_MAGE_NETHER_TEMPEST_MISSILE            = 114956,
    SPELL_MAGE_INVISIBILITY                      = 32612,
    SPELL_MAGE_GREATER_INVISIBILITY              = 110960,
    SPELL_MAGE_GREATER_INVISIBILITY_LESS_DAMAGE  = 113862,
    SPELL_MAGE_REMOVE_INVISIBILITY_REMOVED_TIMER = 122293,
    SPELL_MAGE_TEMPORAL_SHIELD_HEAL              = 115611,
    SPELL_MAGE_CAUTERIZE                         = 87023,
    SPELL_MAGE_CAUTERIZE_MARKER                  = 87024,
    SPELL_MAGE_ALTER_TIME                        = 110909,

    // Glyphs
    SPELL_MAGE_GLYPH_OF_SLOW                     = 86209,
    SPELL_MAGE_GLYPH_OF_SPELLSTEAL               = 115713,
    SPELL_MAGE_GLYPH_OF_SPELLSTEAL_HEAL          = 115714,
    SPELL_MAGE_GLYPH_OF_ILLUSION_ACTIVE          = 131784,
    SPELL_MAGE_CONJURE_FAMILIAR                  = 126578,
    SPELL_MAGE_UNBOUND_WATER_ELEMENTAL_TRANSFORM = 147358,
    SPELL_MAGE_GLYPH_OF_RAPID_DISPLACEMENT       = 146659,
    SPELL_MAGE_GLYPH_OF_MOMENTUM                 = 56384,
    SPELL_MAGE_GLYPH_OF_MOMENTUM_BLINK           = 119415,
    SPELL_MAGE_GLYPH_OF_RAPID_TELEPORTATION      = 89749,
    SPELL_MAGE_RAPID_TELEPORTATION               = 46989,
    SPELL_MAGE_MAGE_ARMOR                        = 6117,
    SPELL_MAGE_GLYPH_OF_THE_MONKEY               = 57927,
    SPELL_MAGE_GLYPH_OF_THE_PORCUPINE            = 57924,
    SPELL_MAGE_GLYPH_OF_THE_BEAR_CUB             = 58136,

    // Set bonuses
    SPELL_MAGE_T15_2P_BONUS                      = 138316,
    SPELL_MAGE_TIME_LORD                         = 138317,
    SPELL_MAGE_T15_4P_BONUS                      = 138376,
    SPELL_MAGE_T16_2P_BONUS                      = 145251,
    SPELL_MAGE_POTENT_FLAMES                     = 145254,
    SPELL_MAGE_FROZEN_THOUGHTS                   = 146557,
    SPELL_MAGE_T16_4P_BONUS                      = 145257,
    SPELL_MAGE_FIERY_ADEPT                       = 145261,
    SPELL_MAGE_FRIGID_BLAST                      = 145264,
    SPELL_MAGE_PVP_2P_BONUS                      = 131618,
};

enum MageCreatures
{
    NPC_MAGE_WATER_ELEMENTAL                     = 510,
    NPC_MAGE_PAST_SELF                           = 58542,
};

enum MageDisplayIds
{
    DISPLAY_MAGE_POLYMORPH_MONKEY                = 29963,
    DISPLAY_MAGE_POLYMORPH_BEAR_CUB              = 23948,
};

uint32 const MagePolymorphPorcupineDisplayIds[3] = { 40119, 40122, 40123 };
uint8 const MAGE_MAX_ICICLES = 5;

enum MageIcons
{
    ICON_MAGE_SHATTER                            = 976,
    ICON_MAGE_IMPROVED_FLAMESTRIKE               = 37,
    ICON_MAGE_IMPROVED_FREEZE                    = 94,
    ICON_MAGE_INCANTER_S_ABSORPTION              = 2941,
    ICON_MAGE_IMPROVED_MANA_GEM                  = 1036
};

enum MiscSpells
{
    SPELL_PRIEST_SHADOW_WORD_DEATH                  = 32409
};

// Frostjaw - 102051 // 5.4.8
class spell_mage_frostjaw : public SpellScriptLoader
{
public:
    spell_mage_frostjaw() : SpellScriptLoader("spell_mage_frostjaw") { }

    class spell_mage_frostjaw_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_frostjaw_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_FROSTJAW))
                return false;
            return true;
        }

        void HandleOnHit()
        {
            if (Player* player = GetCaster()->ToPlayer())
                if (Unit* target = GetHitUnit())
                    if (target->GetTypeId() == TypeID::TYPEID_PLAYER)
                        if (Aura* aura = target->GetAura(SPELL_MAGE_FROSTJAW, player->GetGUID()))
                        {
                            aura->SetDuration(aura->GetDuration() / 2);
                            aura->SetMaxDuration(aura->GetMaxDuration() / 2);
                        }
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_mage_frostjaw_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_frostjaw_SpellScript();
    }
};

// Pyroblast - 11366 // 5.4.8
class spell_mage_pyroblast : public SpellScriptLoader
{
public:
    spell_mage_pyroblast() : SpellScriptLoader("spell_mage_pyroblast") { }
    class spell_mage_pyroblast_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_pyroblast_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_PYROBLAST_CLEARCAST))
                return false;
            return true;
        }

        bool Load() override
        {
            _amplify = false;
            return true;
        }

        void HandleCast()
        {
            Aura* pyroblast = GetCaster()->GetAura(SPELL_MAGE_PYROBLAST_CLEARCAST);
            if (!pyroblast || GetSpell()->m_appliedMods.find(pyroblast) == GetSpell()->m_appliedMods.end())
                return;

            _amplify = true;
            // An instant Pyroblast! must not also eat a Presence of Mind charge
            if (Player* player = GetCaster()->ToPlayer())
                player->RestoreSpellMods(GetSpell(), SPELL_MAGE_PRESENCE_OF_MIND);
        }

        void HandleDamage(SpellEffIndex /*effIndex*/)
        {
            if (!_amplify)
                return;

            int32 damage = GetHitDamage();
            AddPct(damage, GetSpellInfo()->Effects[EFFECT_2].CalcValue(GetCaster()));
            SetHitDamage(damage);
        }

        void HandleAfterHit()
        {
            if (!_amplify)
                return;

            if (Aura* dot = GetHitAura())
                if (AuraEffect* periodic = dot->GetEffect(EFFECT_1))
                {
                    int32 amount = periodic->GetAmount();
                    AddPct(amount, GetSpellInfo()->Effects[EFFECT_2].CalcValue(GetCaster()));
                    periodic->ChangeAmount(amount, false);
                }
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_mage_pyroblast_SpellScript::HandleCast);
            OnEffectHitTarget += SpellEffectFn(spell_mage_pyroblast_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            AfterHit += SpellHitFn(spell_mage_pyroblast_SpellScript::HandleAfterHit);
        }

    private:
        bool _amplify;
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_mage_pyroblast_SpellScript();
    }
};

// Time Warp - 80353
class spell_mage_time_warp : public SpellScriptLoader
{
public:
    spell_mage_time_warp() : SpellScriptLoader("spell_mage_time_warp") { }
    class spell_mage_time_warp_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_time_warp_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_TEMPORAL_DISPLACEMENT))
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
                target->CastSpell(target, SPELL_MAGE_TEMPORAL_DISPLACEMENT, true);
        }
        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_time_warp_SpellScript::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_time_warp_SpellScript::RemoveInvalidTargets, EFFECT_1, TARGET_UNIT_CASTER_AREA_RAID);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_time_warp_SpellScript::RemoveInvalidTargets, EFFECT_2, TARGET_UNIT_CASTER_AREA_RAID);
            AfterHit += SpellHitFn(spell_mage_time_warp_SpellScript::ApplyDebuff);
        }
    };
    SpellScript* GetSpellScript() const override
    {
        return new spell_mage_time_warp_SpellScript();
    }
};

// 11113 - Blast Wave
class spell_mage_blast_wave : public SpellScriptLoader
{
public:
    spell_mage_blast_wave() : SpellScriptLoader("spell_mage_blast_wave") { }

    class spell_mage_blast_wave_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_blast_wave_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_FLAMESTRIKE))
                return false;
            return true;
        }

        void CountTargets(std::list<WorldObject*>& targetList)
        {
            _targetCount = targetList.size();
        }

        void HandleImprovedFlamestrike()
        {
            if (_targetCount >= 2)
                if (AuraEffect* aurEff = GetCaster()->GetAuraEffect(SPELL_AURA_DUMMY, SPELLFAMILY_MAGE, ICON_MAGE_IMPROVED_FLAMESTRIKE, EFFECT_0))
                    if (roll_chance_i(aurEff->GetAmount()))
                    {
                        float x, y, z;
                        WorldLocation const* loc = GetExplTargetDest();
                        if (!loc)
                            return;

                        loc->GetPosition(x, y, z);
                        GetCaster()->CastSpell(x, y, z, SPELL_MAGE_FLAMESTRIKE, true);
                    }
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_blast_wave_SpellScript::CountTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            AfterCast += SpellCastFn(spell_mage_blast_wave_SpellScript::HandleImprovedFlamestrike);
        }

    private:
        uint32 _targetCount;
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_mage_blast_wave_SpellScript();
    }
};

// 11958 - Cold Snap // 5.4.8
class spell_mage_cold_snap : public SpellScriptLoader
{
public:
    spell_mage_cold_snap() : SpellScriptLoader("spell_mage_cold_snap") { }

    class spell_mage_cold_snap_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_cold_snap_SpellScript);

        bool Load() override
        {
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Player* caster = GetCaster()->ToPlayer();
            // immediately finishes the cooldown of Frost Nova, Cone of Cold, Ice Block
            const SpellCooldowns& cm = caster->GetSpellCooldownMap();
            for (SpellCooldowns::const_iterator itr = cm.begin(); itr != cm.end();)
            {
                SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(itr->first);

                if (spellInfo->SpellFamilyName == SPELLFAMILY_MAGE &&
                    (
                        // Frost Nova
                        (spellInfo->SpellIconID == 193 && spellInfo->SpellFamilyFlags[0] == 0x00000040) ||
                        // Cone of Cold
                        (spellInfo->SpellIconID == 35 && spellInfo->SpellFamilyFlags[0] == 0x00000200) ||
                        // Ice Block
                        (spellInfo->SpellIconID == 14 && spellInfo->SpellFamilyFlags[1] == 0x00000080 && spellInfo->SpellFamilyFlags[2] == 0x00000008)
                    ))
                {
                    caster->RemoveSpellCooldown((itr++)->first, true);
                }
                else
                    ++itr;
            }
        }

        void Register() override
        {
            OnEffectHit += SpellEffectFn(spell_mage_cold_snap_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_mage_cold_snap_SpellScript();
    }
};

struct ConjureRefreshmentTableData
{
    uint32 minLevel;
    uint32 maxLevel;
    uint32 spellId;
};

uint8 const MAX_CONJURE_REFRESHMENT_TABLE_SPELLS = 4;
ConjureRefreshmentTableData const _conjureTableData[MAX_CONJURE_REFRESHMENT_TABLE_SPELLS] =
{
    { 72, 79, 120056 }, // R5: Item: 43518, Spell: 43988, Object: 186812
    { 80, 84, 120055 }, // R6: Item: 43523, Spell: 92822, Object: 207386
    { 85, 89, 120054 }, // R7: Item: 65499, Spell: 92826, Object: 207387
    { 90, 90, 120053 }, // R8: Item: 80610, Spell: 116136, Object: 211363
};

// 43987 - Conjure Refreshment Table
class spell_mage_conjure_refreshment_table : public SpellScriptLoader
{
public:
    spell_mage_conjure_refreshment_table() : SpellScriptLoader("spell_mage_conjure_refreshment_table") { }

    class spell_mage_conjure_refreshment_table_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_conjure_refreshment_table_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            for (uint8 i = 0; i < MAX_CONJURE_REFRESHMENT_TABLE_SPELLS; ++i)
                if (!sSpellMgr->GetSpellInfo(_conjureTableData[i].spellId))
                    return false;
            return true;
        }

        bool Load() override
        {
            if (GetCaster()->GetTypeId() != TypeID::TYPEID_PLAYER)
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            uint8 level = GetHitUnit()->getLevel();
            for (uint8 i = 0; i < MAX_CONJURE_REFRESHMENT_TABLE_SPELLS; ++i)
            {
                ConjureRefreshmentTableData const& spellData = _conjureTableData[i];
                if (level < spellData.minLevel || level > spellData.maxLevel)
                    continue;
                GetHitUnit()->CastSpell(GetHitUnit(), spellData.spellId);
                break;
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_mage_conjure_refreshment_table_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_mage_conjure_refreshment_table_SpellScript();
    }
};


// 42955 Conjure Refreshment
/// Updated 4.3.4
struct ConjureRefreshmentData
{
    uint32 minLevel;
    uint32 maxLevel;
    uint32 spellId;
};

uint8 const MAX_CONJURE_REFRESHMENT_SPELLS = 8;
ConjureRefreshmentData const _conjureData[MAX_CONJURE_REFRESHMENT_SPELLS] =
{
    { 33, 43, 92739 },
    { 44, 53, 92799 },
    { 54, 63, 92802 },
    { 64, 73, 92805 },
    { 74, 79, 74625 },
    { 80, 84, 42956 },
    { 85, 89, 92727 },
    { 90, 90, 116130 }, 
};

// 42955 - Conjure Refreshment
class spell_mage_conjure_refreshment : public SpellScriptLoader
{
public:
    spell_mage_conjure_refreshment() : SpellScriptLoader("spell_mage_conjure_refreshment") { }

    class spell_mage_conjure_refreshment_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_conjure_refreshment_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            for (uint8 i = 0; i < MAX_CONJURE_REFRESHMENT_SPELLS; ++i)
                if (!sSpellMgr->GetSpellInfo(_conjureData[i].spellId))
                    return false;
            return true;
        }

        bool Load() override
        {
            if (GetCaster()->GetTypeId() != TypeID::TYPEID_PLAYER)
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            uint8 level = GetHitUnit()->getLevel();
            for (uint8 i = 0; i < MAX_CONJURE_REFRESHMENT_SPELLS; ++i)
            {
                ConjureRefreshmentData const& spellData = _conjureData[i];
                if (level < spellData.minLevel || level > spellData.maxLevel)
                    continue;
                GetHitUnit()->CastSpell(GetHitUnit(), spellData.spellId);
                break;
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_mage_conjure_refreshment_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_mage_conjure_refreshment_SpellScript();
    }
};

// 56372 - Glyph of Ice Block
class spell_mage_glyph_of_ice_block : public SpellScriptLoader
{
public:
    spell_mage_glyph_of_ice_block() : SpellScriptLoader("spell_mage_glyph_of_ice_block") { }

    class spell_mage_glyph_of_ice_block_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_glyph_of_ice_block_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_FROST_NOVA))
                return false;
            return true;
        }

        bool CheckProc(ProcEventInfo& /*eventInfo*/)
        {
            return GetTarget()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void HandleEffectProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            // Remove Frost Nova cooldown
            GetTarget()->ToPlayer()->RemoveSpellCooldown(SPELL_MAGE_FROST_NOVA, true);
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_mage_glyph_of_ice_block_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_mage_glyph_of_ice_block_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_mage_glyph_of_ice_block_AuraScript();
    }
};

// 56375 - Glyph of Polymorph
class spell_mage_glyph_of_polymorph : public SpellScriptLoader
{
public:
    spell_mage_glyph_of_polymorph() : SpellScriptLoader("spell_mage_glyph_of_polymorph") { }

    class spell_mage_glyph_of_polymorph_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_glyph_of_polymorph_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PRIEST_SHADOW_WORD_DEATH))
                return false;
            return true;
        }

        void HandleEffectProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            Unit* target = eventInfo.GetProcTarget();

            target->RemoveAurasByType(SPELL_AURA_PERIODIC_DAMAGE, 0, target->GetAura(SPELL_PRIEST_SHADOW_WORD_DEATH)); // SW:D shall not be removed.
            target->RemoveAurasByType(SPELL_AURA_PERIODIC_DAMAGE_PERCENT);
            target->RemoveAurasByType(SPELL_AURA_PERIODIC_LEECH);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_mage_glyph_of_polymorph_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_mage_glyph_of_polymorph_AuraScript();
    }
};

// 44457 - Living Bomb
class spell_mage_living_bomb : public SpellScriptLoader
{
public:
    spell_mage_living_bomb() : SpellScriptLoader("spell_mage_living_bomb") { }

    class spell_mage_living_bomb_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_living_bomb_AuraScript);

        bool Validate(SpellInfo const* spellInfo) override
        {
            if (!sSpellMgr->GetSpellInfo(uint32(spellInfo->Effects[EFFECT_1].CalcValue())))
                return false;
            return true;
        }

        void TryBrainFreeze(Unit* caster)
        {
            if (caster->HasAura(SPELL_MAGE_BRAIN_FREEZE) && roll_chance_i(25))
                caster->CastSpell(caster, SPELL_MAGE_BRAIN_FREEZE_TRIGGERED, true);
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
                caster->BindAura(GetAura());
        }

        void HandleTick(AuraEffect const* /*aurEff*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            // Only the most recent bomb rolls for Brain Freeze on ticks
            if (Unit::AuraList const* bombs = caster->GetBoundAurasBySpellId(GetId()))
                if (bombs->back() != GetAura())
                    return;

            TryBrainFreeze(caster);
        }

        void AfterRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
            if (removeMode != AURA_REMOVE_BY_ENEMY_SPELL && removeMode != AURA_REMOVE_BY_EXPIRE)
                return;

            if (Unit* caster = GetCaster())
            {
                // Cap AOE selection: bomb target + up to 3 extra enemies nearby.
                caster->CastCustomSpell(uint32(aurEff->GetAmount()), SPELLVALUE_MAX_TARGETS, 4, GetTarget(), true, NULL, aurEff);
                TryBrainFreeze(caster);
            }
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_mage_living_bomb_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_mage_living_bomb_AuraScript::HandleTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            AfterEffectRemove += AuraEffectRemoveFn(spell_mage_living_bomb_AuraScript::AfterRemove, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_mage_living_bomb_AuraScript();
    }
};

// 44461 - Living Bomb (explosion)
// Always hit the bomb target, then at most 3 additional nearby enemies.
class spell_mage_living_bomb_explosion : public SpellScriptLoader
{
public:
    spell_mage_living_bomb_explosion() : SpellScriptLoader("spell_mage_living_bomb_explosion") { }

    class spell_mage_living_bomb_explosion_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_living_bomb_explosion_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            Unit* bombTarget = GetExplTargetUnit();
            if (!bombTarget)
            {
                if (targets.size() > 3)
                    Skyfire::Containers::RandomResizeList(targets, 3);
                return;
            }

            std::list<WorldObject*> extraTargets;
            for (std::list<WorldObject*>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
            {
                if (*itr != bombTarget)
                    extraTargets.push_back(*itr);
            }

            if (extraTargets.size() > 3)
                Skyfire::Containers::RandomResizeList(extraTargets, 3);

            targets.clear();
            targets.push_back(bombTarget);
            targets.splice(targets.end(), extraTargets);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_living_bomb_explosion_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_mage_living_bomb_explosion_SpellScript();
    }
};

// 86181 - Nether Vortex
class spell_mage_nether_vortex : public SpellScriptLoader
{
public:
    spell_mage_nether_vortex() : SpellScriptLoader("spell_mage_nether_vortex") { }

    class spell_mage_nether_vortex_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_nether_vortex_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_SLOW))
                return false;
            return true;
        }

        bool DoCheck(ProcEventInfo& eventInfo)
        {
            if (Aura* aura = eventInfo.GetProcTarget()->GetAura(SPELL_MAGE_SLOW))
                if (aura->GetCasterGUID() != GetTarget()->GetGUID())
                    return false;

            return true;
        }

        void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            GetTarget()->CastSpell(eventInfo.GetProcTarget(), SPELL_MAGE_SLOW, true, NULL, aurEff);
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_mage_nether_vortex_AuraScript::DoCheck);
            OnEffectProc += AuraEffectProcFn(spell_mage_nether_vortex_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_mage_nether_vortex_AuraScript();
    }
};

enum SilvermoonPolymorph
{
    NPC_AUROSALIA = 18744
};

/// @todo move out of here and rename - not a mage spell
// 32826 - Polymorph (Visual)
class spell_mage_polymorph_cast_visual : public SpellScriptLoader
{
public:
    spell_mage_polymorph_cast_visual() : SpellScriptLoader("spell_mage_polymorph_visual") { }

    class spell_mage_polymorph_cast_visual_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_polymorph_cast_visual_SpellScript);

        static const uint32 PolymorhForms[6];

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            // check if spell ids exist in dbc
            for (uint32 i = 0; i < 6; i++)
                if (!sSpellMgr->GetSpellInfo(PolymorhForms[i]))
                    return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Unit* target = GetCaster()->FindNearestCreature(NPC_AUROSALIA, 30.0f))
                if (target->GetTypeId() == TypeID::TYPEID_UNIT)
                    target->CastSpell(target, PolymorhForms[std::rand() % 6], true);
        }

        void Register() override
        {
            // add dummy effect spell handler to Polymorph visual
            OnEffectHitTarget += SpellEffectFn(spell_mage_polymorph_cast_visual_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_mage_polymorph_cast_visual_SpellScript();
    }
};

uint32 const spell_mage_polymorph_cast_visual::spell_mage_polymorph_cast_visual_SpellScript::PolymorhForms[6] =
{
    SPELL_MAGE_SQUIRREL_FORM,
    SPELL_MAGE_GIRAFFE_FORM,
    SPELL_MAGE_SERPENT_FORM,
    SPELL_MAGE_DRAGONHAWK_FORM,
    SPELL_MAGE_WORGEN_FORM,
    SPELL_MAGE_SHEEP_FORM
};

// 136511 - Ring of Frost (periodic, triggered by 113724)
/// MoP: periodic aura lives on EFFECT_0 (Cata used EFFECT_1 on a different spell).
class spell_mage_ring_of_frost : public SpellScriptLoader
{
public:
    spell_mage_ring_of_frost() : SpellScriptLoader("spell_mage_ring_of_frost") { }

    class spell_mage_ring_of_frost_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_ring_of_frost_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_RING_OF_FROST_SUMMON))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_RING_OF_FROST_FREEZE))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_RING_OF_FROST_DUMMY))
                return false;
            return true;
        }

        bool Load() override
        {
            ringOfFrost = NULL;
            return true;
        }

        void HandleEffectPeriodic(AuraEffect const* /*aurEff*/)
        {
            if (ringOfFrost)
                if (GetMaxDuration() - (int32)ringOfFrost->GetTimer() >= sSpellMgr->GetSpellInfo(SPELL_MAGE_RING_OF_FROST_DUMMY)->GetDuration())
                    GetTarget()->CastSpell(ringOfFrost->GetPositionX(), ringOfFrost->GetPositionY(), ringOfFrost->GetPositionZ(), SPELL_MAGE_RING_OF_FROST_FREEZE, true);
        }

        void Apply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            std::list<Creature*> MinionList;
            // MoP summon creature entry is on the cast spell (113724) EFFECT_0, not this aura.
            SpellInfo const* summonInfo = sSpellMgr->GetSpellInfo(SPELL_MAGE_RING_OF_FROST_SUMMON);
            if (!summonInfo)
                return;

            GetTarget()->GetAllMinionsByEntry(MinionList, summonInfo->Effects[EFFECT_0].MiscValue);

            // Get the last summoned RoF, save it and despawn older ones
            for (std::list<Creature*>::iterator itr = MinionList.begin(); itr != MinionList.end(); ++itr)
            {
                TempSummon* summon = (*itr)->ToTempSummon();

                if (ringOfFrost && summon)
                {
                    if (summon->GetTimer() > ringOfFrost->GetTimer())
                    {
                        ringOfFrost->DespawnOrUnsummon();
                        ringOfFrost = summon;
                    }
                    else
                        summon->DespawnOrUnsummon();
                }
                else if (summon)
                    ringOfFrost = summon;
            }
        }

        TempSummon* ringOfFrost;

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_mage_ring_of_frost_AuraScript::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            OnEffectApply += AuraEffectApplyFn(spell_mage_ring_of_frost_AuraScript::Apply, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_mage_ring_of_frost_AuraScript();
    }
};

// 82691 - Ring of Frost (freeze efect)
/// Updated 4.3.4
class spell_mage_ring_of_frost_freeze : public SpellScriptLoader
{
public:
    spell_mage_ring_of_frost_freeze() : SpellScriptLoader("spell_mage_ring_of_frost_freeze") { }

    class spell_mage_ring_of_frost_freeze_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_ring_of_frost_freeze_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_RING_OF_FROST_SUMMON))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_RING_OF_FROST_FREEZE))
                return false;
            return true;
        }

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            // MoP: outer radius comes from cast spell 113724 EFFECT_2 dummy radius (was summon spell EFFECT_0 in Cata).
            SpellInfo const* ringInfo = sSpellMgr->GetSpellInfo(SPELL_MAGE_RING_OF_FROST_SUMMON);
            float outRadius = ringInfo ? ringInfo->Effects[EFFECT_2].CalcRadius() : 10.0f;
            float inRadius = 4.7f;

            for (std::list<WorldObject*>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
                if (Unit* unit = (*itr)->ToUnit())
                    if (unit->HasAura(SPELL_MAGE_RING_OF_FROST_DUMMY) || unit->HasAura(SPELL_MAGE_RING_OF_FROST_FREEZE) || unit->GetExactDist(GetExplTargetDest()) > outRadius || unit->GetExactDist(GetExplTargetDest()) < inRadius)
                        targets.erase(itr--);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_ring_of_frost_freeze_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_mage_ring_of_frost_freeze_SpellScript();
    }

    class spell_mage_ring_of_frost_freeze_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_ring_of_frost_freeze_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_RING_OF_FROST_DUMMY))
                return false;
            return true;
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
                if (GetCaster())
                    GetCaster()->CastSpell(GetTarget(), SPELL_MAGE_RING_OF_FROST_DUMMY, true);
        }

        void Register() override
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_mage_ring_of_frost_freeze_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_mage_ring_of_frost_freeze_AuraScript();
    }
};

// 33395 - Freeze (Water Elemental)
class spell_mage_water_elemental_freeze : public SpellScriptLoader
{
public:
    spell_mage_water_elemental_freeze() : SpellScriptLoader("spell_mage_water_elemental_freeze") { }

    class spell_mage_water_elemental_freeze_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_water_elemental_freeze_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_FINGERS_OF_FROST))
                return false;
            return true;
        }

        void HandleOnHit()
        {
            if (Unit* owner = GetCaster()->GetOwner())
                if (owner->HasSpell(SPELL_MAGE_FINGERS_OF_FROST_PASSIVE))
                    owner->CastSpell(owner, SPELL_MAGE_FINGERS_OF_FROST, true);
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_mage_water_elemental_freeze_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_mage_water_elemental_freeze_SpellScript();
    }
};

// 55342 - Mirror Image
class spell_mage_mirror_image : public SpellScriptLoader
{
public:
    spell_mage_mirror_image() : SpellScriptLoader("spell_mage_mirror_image") { }

    class spell_mage_mirror_image_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_mirror_image_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_SUMMON_IMAGES_FROST) || !sSpellMgr->GetSpellInfo(SPELL_MAGE_SUMMON_IMAGES_FIRE) ||
                !sSpellMgr->GetSpellInfo(SPELL_MAGE_SUMMON_IMAGES_ARCANE) || !sSpellMgr->GetSpellInfo(SPELL_MAGE_GLYPH_OF_MIRROR_IMAGE))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();

            uint32 spellId = SPELL_MAGE_SUMMON_IMAGES_FROST;

            if (Player* player = caster->ToPlayer())
            {
                if (GetCaster()->HasSpell(SPELL_MAGE_GLYPH_OF_MIRROR_IMAGE))
                {
                    switch (player->GetTalentSpecialization(player->GetActiveSpec()))
                    {
                    case SPEC_MAGE_ARCANE:
                        spellId = SPELL_MAGE_SUMMON_IMAGES_ARCANE;
                        break;
                    case SPEC_MAGE_FIRE:
                        spellId = SPELL_MAGE_SUMMON_IMAGES_FIRE;
                        break;
                    case SPEC_MAGE_FROST:
                        spellId = SPELL_MAGE_SUMMON_IMAGES_FROST;
                        break;
                    }
                }
            }

            caster->CastSpell(caster, spellId, true);
        }

        void Register() override
        {
            OnEffectHit += SpellEffectFn(spell_mage_mirror_image_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_mage_mirror_image_SpellScript();
    }
};

static Unit* GetFrozenOrbOwner(Unit* orb)
{
    if (Unit* owner = orb->GetOwner())
        return owner;
    if (TempSummon* summon = orb->ToTempSummon())
        return summon->GetSummoner();
    return NULL;
}

static void CopyAuraToTarget(Unit* caster, Aura const* source, Unit* target)
{
    Aura* copy = caster->AddAura(source->GetSpellInfo(), source->GetEffectMask(), target);
    if (!copy)
        return;

    copy->SetMaxDuration(source->GetDuration());
    copy->SetDuration(source->GetDuration());
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        if (AuraEffect const* sourceEffect = source->GetEffect(i))
            if (AuraEffect* copyEffect = copy->GetEffect(i))
                copyEffect->SetAmount(sourceEffect->GetAmount());
}

// 44448 - Pyroblast Clearcasting Driver (Heating Up / Pyroblast!)
class spell_mage_pyroblast_clearcasting : public SpellScriptLoader
{
public:
    spell_mage_pyroblast_clearcasting() : SpellScriptLoader("spell_mage_pyroblast_clearcasting") { }

    class spell_mage_pyroblast_clearcasting_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_pyroblast_clearcasting_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_MAGE_HEATING_UP) && sSpellMgr->GetSpellInfo(SPELL_MAGE_PYROBLAST_CLEARCAST);
        }

        bool Load() OVERRIDE
        {
            _delayedRemove = std::make_shared<bool>(false);
            return GetUnitOwner()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        static void HandleCritical(Unit* mage)
        {
            if (Aura* heatingUp = mage->GetAura(SPELL_MAGE_HEATING_UP))
            {
                mage->CastSpell(mage, SPELL_MAGE_PYROBLAST_CLEARCAST, true);
                heatingUp->Remove();
            }
            else
                mage->CastSpell(mage, SPELL_MAGE_HEATING_UP, true);
        }

        void HandleProc(ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            if (eventInfo.GetHitMask() & (PROC_EX_INTERNAL_TRIGGERED | PROC_EX_INTERNAL_DOT))
                return;

            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            if (!spellInfo || !eventInfo.GetDamageInfo() || !(eventInfo.GetDamageInfo()->GetSchoolMask() & SPELL_SCHOOL_MASK_FIRE))
                return;

            Unit* mage = GetUnitOwner();
            if (eventInfo.GetHitMask() & PROC_EX_CRITICAL_HIT)
            {
                *_delayedRemove = false;
                // Instant crits are counted after any travelling spell that is still in the air
                if (spellInfo->Speed > 0.0f)
                    HandleCritical(mage);
                else
                    mage->m_Events.Schedule(250, [mage]() { HandleCritical(mage); });
            }
            else
            {
                // A non-crit only clears Heating Up if no crit arrives within the grace window
                *_delayedRemove = true;
                std::shared_ptr<bool> delayedRemove = _delayedRemove;
                mage->m_Events.Schedule(300, [mage, delayedRemove]()
                {
                    if (!*delayedRemove)
                        return;
                    *delayedRemove = false;
                    mage->RemoveAurasDueToSpell(SPELL_MAGE_HEATING_UP);
                });
            }
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_mage_pyroblast_clearcasting_AuraScript::HandleProc);
        }

    private:
        std::shared_ptr<bool> _delayedRemove;
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_mage_pyroblast_clearcasting_AuraScript();
    }
};

// 11129 - Combustion
class spell_mage_combustion : public SpellScriptLoader
{
public:
    spell_mage_combustion() : SpellScriptLoader("spell_mage_combustion") { }

    class spell_mage_combustion_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_combustion_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_MAGE_COMBUSTION_IMPACT) && sSpellMgr->GetSpellInfo(SPELL_MAGE_COMBUSTION_DOT);
        }

        void HandleHit()
        {
            Player* mage = GetCaster()->ToPlayer();
            Unit* target = GetHitUnit();
            if (!mage || !target)
                return;

            mage->CastSpell(target, SPELL_MAGE_COMBUSTION_IMPACT, true);

            if (mage->HasSpellCooldown(SPELL_MAGE_INFERNO_BLAST))
            {
                mage->RemoveSpellCooldown(SPELL_MAGE_INFERNO_BLAST, true);
                mage->RemoveSpellCooldown(SPELL_MAGE_INFERNO_BLAST_IMPACT, true);
            }

            if (AuraEffect const* ignite = target->GetAuraEffect(SPELL_MAGE_IGNITE, EFFECT_0, mage->GetGUID()))
            {
                int32 damage = CalculatePct(ignite->GetAmount(), GetSpellInfo()->Effects[EFFECT_0].CalcValue(mage));
                if (damage > 0)
                    mage->CastCustomSpell(SPELL_MAGE_COMBUSTION_DOT, SPELLVALUE_BASE_POINT0, damage, target, true);
            }
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_mage_combustion_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_combustion_SpellScript();
    }
};

// 108853 - Inferno Blast
class spell_mage_inferno_blast : public SpellScriptLoader
{
public:
    spell_mage_inferno_blast() : SpellScriptLoader("spell_mage_inferno_blast") { }

    class spell_mage_inferno_blast_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_inferno_blast_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_MAGE_INFERNO_BLAST_IMPACT);
        }

        void HandleHit()
        {
            if (Unit* target = GetHitUnit())
                GetCaster()->CastSpell(target, SPELL_MAGE_INFERNO_BLAST_IMPACT, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_mage_inferno_blast_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_inferno_blast_SpellScript();
    }
};

// 118280 - Inferno Blast (spread)
class spell_mage_inferno_blast_spread : public SpellScriptLoader
{
public:
    spell_mage_inferno_blast_spread() : SpellScriptLoader("spell_mage_inferno_blast_spread") { }

    class spell_mage_inferno_blast_spread_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_inferno_blast_spread_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (Unit* mainTarget = GetExplTargetUnit())
                targets.remove(mainTarget);

            targets.remove_if([](WorldObject* target)
            {
                Unit* unit = target->ToUnit();
                return !unit || unit->HasBreakableByDamageCrowdControlAura();
            });

            uint32 maxTargets = GetCaster()->HasAura(SPELL_MAGE_GLYPH_OF_INFERNO_BLAST) ? 4 : 3;
            if (targets.size() > maxTargets)
                Skyfire::Containers::RandomResizeList(targets, maxTargets);
        }

        void HandleSpread(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* mainTarget = GetExplTargetUnit();
            Unit* target = GetHitUnit();
            if (!mainTarget || !target)
                return;

            std::vector<Aura*> spreadAuras;
            Unit::AuraEffectList const& periodicAuras = mainTarget->GetAuraEffectsByType(SPELL_AURA_PERIODIC_DAMAGE);
            for (Unit::AuraEffectList::const_iterator itr = periodicAuras.begin(); itr != periodicAuras.end(); ++itr)
            {
                if ((*itr)->GetCasterGUID() != caster->GetGUID())
                    continue;

                uint32 spellId = (*itr)->GetId();
                if (spellId == SPELL_MAGE_IGNITE || spellId == SPELL_MAGE_PYROBLAST || spellId == SPELL_MAGE_COMBUSTION_DOT)
                    spreadAuras.push_back((*itr)->GetBase());
            }

            for (std::vector<Aura*>::const_iterator itr = spreadAuras.begin(); itr != spreadAuras.end(); ++itr)
                CopyAuraToTarget(caster, *itr, target);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_inferno_blast_spread_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_mage_inferno_blast_spread_SpellScript::HandleSpread, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_inferno_blast_spread_SpellScript();
    }
};

// 2120 - Flamestrike
class spell_mage_flamestrike : public SpellScriptLoader
{
public:
    spell_mage_flamestrike() : SpellScriptLoader("spell_mage_flamestrike") { }

    class spell_mage_flamestrike_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_flamestrike_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_MAGE_BLAST_WAVE_SNARE);
        }

        void HandleAfterCast()
        {
            Player* caster = GetCaster()->ToPlayer();
            WorldLocation const* dest = GetExplTargetDest();
            if (!caster || !dest || caster->GetTalentSpecialization(caster->GetActiveSpec()) != SPEC_MAGE_FIRE)
                return;

            caster->CastSpell(dest->GetPositionX(), dest->GetPositionY(), dest->GetPositionZ(), SPELL_MAGE_BLAST_WAVE_SNARE, true);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_mage_flamestrike_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_flamestrike_SpellScript();
    }
};

// 44457 - Living Bomb, 112948 - Frost Bomb, 114923 - Nether Tempest
class spell_mage_pyromaniac : public SpellScriptLoader
{
public:
    spell_mage_pyromaniac() : SpellScriptLoader("spell_mage_pyromaniac") { }

    class spell_mage_pyromaniac_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_pyromaniac_SpellScript);

        void HandleHit()
        {
            if (Unit* target = GetHitUnit())
                if (GetCaster()->HasAura(SPELL_MAGE_PYROMANIAC_AURA))
                    GetCaster()->CastSpell(target, SPELL_MAGE_PYROMANIAC_DAMAGE_DONE, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_mage_pyromaniac_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_pyromaniac_SpellScript();
    }
};

// 44544 - Fingers of Frost
class spell_mage_fingers_of_frost : public SpellScriptLoader
{
public:
    spell_mage_fingers_of_frost() : SpellScriptLoader("spell_mage_fingers_of_frost") { }

    class spell_mage_fingers_of_frost_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_fingers_of_frost_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* owner = GetUnitOwner();
            if (GetStackAmount() >= 2)
                owner->CastSpell(owner, SPELL_MAGE_FINGERS_OF_FROST_DOUBLE, true);
            else
                owner->RemoveAurasDueToSpell(SPELL_MAGE_FINGERS_OF_FROST_DOUBLE);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetUnitOwner()->RemoveAurasDueToSpell(SPELL_MAGE_FINGERS_OF_FROST_DOUBLE);
        }

        void Register() OVERRIDE
        {
            AfterEffectApply += AuraEffectApplyFn(spell_mage_fingers_of_frost_AuraScript::HandleApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            AfterEffectRemove += AuraEffectRemoveFn(spell_mage_fingers_of_frost_AuraScript::HandleRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_mage_fingers_of_frost_AuraScript();
    }
};

// 116 - Frostbolt, 44614 - Frostfire Bolt, 42208 - Blizzard
class spell_mage_fingers_of_frost_proc : public SpellScriptLoader
{
public:
    spell_mage_fingers_of_frost_proc() : SpellScriptLoader("spell_mage_fingers_of_frost_proc") { }

    class spell_mage_fingers_of_frost_proc_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_fingers_of_frost_proc_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_MAGE_FINGERS_OF_FROST_PASSIVE) && sSpellMgr->GetSpellInfo(SPELL_MAGE_FINGERS_OF_FROST);
        }

        void HandleOnHit()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!target || !caster->HasSpell(SPELL_MAGE_FINGERS_OF_FROST_PASSIVE) || !caster->IsValidAttackTarget(target))
                return;

            SpellInfo const* passive = sSpellMgr->GetSpellInfo(SPELL_MAGE_FINGERS_OF_FROST_PASSIVE);
            int32 chance = 0;
            switch (GetSpellInfo()->Id)
            {
                case SPELL_MAGE_FROSTBOLT:
                    chance = passive->Effects[EFFECT_0].BasePoints;
                    if (AuraEffect const* bonus = caster->GetAuraEffect(SPELL_MAGE_T15_4P_BONUS, EFFECT_2))
                        chance += bonus->GetAmount();
                    break;
                case SPELL_MAGE_FROSTFIRE_BOLT:
                    chance = passive->Effects[EFFECT_0].BasePoints;
                    break;
                case SPELL_MAGE_BLIZZARD:
                    chance = passive->Effects[EFFECT_1].BasePoints;
                    break;
                default:
                    return;
            }

            if (roll_chance_i(chance))
                caster->CastSpell(caster, SPELL_MAGE_FINGERS_OF_FROST, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_mage_fingers_of_frost_proc_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_fingers_of_frost_proc_SpellScript();
    }
};

// 148023 - Icicles (fires the stored icicles one per tick)
class spell_mage_icicles_periodic : public SpellScriptLoader
{
public:
    spell_mage_icicles_periodic() : SpellScriptLoader("spell_mage_icicles_periodic") { }

    class spell_mage_icicles_periodic_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_icicles_periodic_AuraScript);

    public:
        void SetTarget(uint64 targetGuid)
        {
            _targetGuid = targetGuid;
        }

    private:
        bool Load() OVERRIDE
        {
            _targetGuid = 0;
            return true;
        }

        void HandleTick(AuraEffect const* /*aurEff*/)
        {
            Unit* mage = GetUnitOwner();
            Unit* target = ObjectAccessor::GetUnit(*mage, _targetGuid);
            if (target && target->IsAlive())
            {
                for (uint8 i = 0; i < MAGE_MAX_ICICLES; ++i)
                {
                    if (AuraEffect const* icicle = mage->GetAuraEffect(SPELL_MAGE_ICICLE_STORE + i, EFFECT_0))
                    {
                        int32 amount = icicle->GetAmount();
                        mage->CastCustomSpell(target, SPELL_MAGE_ICICLE_VISUAL + i, &amount, NULL, NULL, true);
                        mage->CastCustomSpell(target, SPELL_MAGE_ICICLE_DAMAGE, &amount, NULL, NULL, true);
                        mage->RemoveAurasDueToSpell(SPELL_MAGE_ICICLE_STORE + i);
                        return;
                    }
                }
            }

            SetDuration(0);
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_mage_icicles_periodic_AuraScript::HandleTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }

        uint64 _targetGuid;
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_mage_icicles_periodic_AuraScript();
    }
};

// 76613 - Mastery: Icicles
class spell_mage_mastery_icicles : public SpellScriptLoader
{
public:
    spell_mage_mastery_icicles() : SpellScriptLoader("spell_mage_mastery_icicles") { }

    class spell_mage_mastery_icicles_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_mastery_icicles_AuraScript);

        bool Load() OVERRIDE
        {
            return GetUnitOwner()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            return eventInfo.GetActionTarget() && eventInfo.GetDamageInfo()->GetDamage();
        }

        void HandleProc(AuraEffect const* mastery, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            Player* mage = GetUnitOwner()->ToPlayer();
            Unit* target = eventInfo.GetActionTarget();
            float damage = float(eventInfo.GetDamageInfo()->GetDamage());

            // Stored damage is based on the pre-PvP-modifier hit
            if (Player* targetPlayer = target->GetAffectingPlayer())
            {
                damage /= 1.0f + targetPlayer->GetFloatValue(PLAYER_FIELD_MOD_RESILIENCE_PERCENT) / 100.0f;
                if (float pvpPower = mage->GetFloatValue(PLAYER_FIELD_PVP_POWER_DAMAGE))
                    damage /= 1.0f + pvpPower / 100.0f;
            }

            int32 stored = int32(CalculatePct(damage, mastery->GetAmount()));
            if (stored <= 0)
                return;

            for (uint8 i = 0; i < MAGE_MAX_ICICLES; ++i)
            {
                if (!mage->HasAura(SPELL_MAGE_ICICLE_STORE + i))
                {
                    mage->CastCustomSpell(SPELL_MAGE_ICICLE_STORE + i, SPELLVALUE_BASE_POINT0, stored, mage, true);
                    return;
                }
            }

            // All slots are full: launch the oldest icicle and store the new one in its place
            if (AuraEffect const* oldest = mage->GetAuraEffect(SPELL_MAGE_ICICLE_STORE, EFFECT_0))
            {
                int32 amount = oldest->GetAmount();
                mage->CastSpell(target, SPELL_MAGE_ICICLE_VISUAL, true);
                mage->CastCustomSpell(SPELL_MAGE_ICICLE_DAMAGE, SPELLVALUE_BASE_POINT0, amount, target, true);
                mage->RemoveAurasDueToSpell(SPELL_MAGE_ICICLE_STORE);
                mage->CastCustomSpell(SPELL_MAGE_ICICLE_STORE, SPELLVALUE_BASE_POINT0, stored, mage, true);
            }
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_mage_mastery_icicles_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_mage_mastery_icicles_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_mage_mastery_icicles_AuraScript();
    }
};

// 30455 - Ice Lance
class spell_mage_ice_lance : public SpellScriptLoader
{
public:
    spell_mage_ice_lance() : SpellScriptLoader("spell_mage_ice_lance") { }

    class spell_mage_ice_lance_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_ice_lance_SpellScript);

        bool Load() OVERRIDE
        {
            _fingersOfFrost = false;
            _frozenAtLaunch = false;
            return true;
        }

        void HandleLaunch(SpellEffIndex /*effIndex*/)
        {
            if (Unit* target = GetHitUnit())
                _frozenAtLaunch = target->HasAuraState(AURA_STATE_FROZEN, GetSpellInfo(), GetCaster());
        }

        void HandleAfterCast()
        {
            Unit* caster = GetCaster();
            if (Aura* fingers = caster->GetAura(SPELL_MAGE_FINGERS_OF_FROST))
            {
                _fingersOfFrost = true;
                fingers->ModStackAmount(-1);
            }

            Unit* target = GetExplTargetUnit();
            if (!target || !caster->HasAura(SPELL_MAGE_MASTERY_ICICLES))
                return;

            caster->CastSpell(caster, SPELL_MAGE_ICICLE_TRIGGER_AURA, true);
            if (Aura* aura = caster->GetAura(SPELL_MAGE_ICICLE_TRIGGER_AURA))
                if (spell_mage_icicles_periodic::spell_mage_icicles_periodic_AuraScript* script = dynamic_cast<spell_mage_icicles_periodic::spell_mage_icicles_periodic_AuraScript*>(aura->GetScriptByName("spell_mage_icicles_periodic")))
                    script->SetTarget(target->GetGUID());
        }

        void HandleHit()
        {
            int32 damage = GetHitDamage();

            // Quadruple damage against frozen targets; the core already doubles it when the target was frozen at launch
            if (_fingersOfFrost || _frozenAtLaunch)
                damage *= _frozenAtLaunch ? 2 : 4;

            if (_fingersOfFrost)
                damage = int32(damage * 1.25f);

            SetHitDamage(damage);
        }

        void Register() OVERRIDE
        {
            OnEffectLaunchTarget += SpellEffectFn(spell_mage_ice_lance_SpellScript::HandleLaunch, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            AfterCast += SpellCastFn(spell_mage_ice_lance_SpellScript::HandleAfterCast);
            OnHit += SpellHitFn(spell_mage_ice_lance_SpellScript::HandleHit);
        }

    private:
        bool _fingersOfFrost;
        bool _frozenAtLaunch;
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_ice_lance_SpellScript();
    }
};

// 44572 - Deep Freeze
class spell_mage_deep_freeze : public SpellScriptLoader
{
public:
    spell_mage_deep_freeze() : SpellScriptLoader("spell_mage_deep_freeze") { }

    class spell_mage_deep_freeze_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_deep_freeze_SpellScript);

        void HandleHit()
        {
            if (Aura* fingers = GetCaster()->GetAura(SPELL_MAGE_FINGERS_OF_FROST))
                fingers->ModStackAmount(-1);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_mage_deep_freeze_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_deep_freeze_SpellScript();
    }
};

// 116 - Frostbolt, 131079 - Frostbolt (Glyph of Icy Veins)
class spell_mage_frostbolt : public SpellScriptLoader
{
public:
    spell_mage_frostbolt() : SpellScriptLoader("spell_mage_frostbolt") { }

    class spell_mage_frostbolt_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_frostbolt_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_MAGE_FROSTBOLT_HEAL);
        }

        bool Load() OVERRIDE
        {
            _heal = false;
            return true;
        }

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            Unit* target = GetExplTargetUnit();
            if (!target)
                return SpellCastResult::SPELL_FAILED_NO_VALID_TARGETS;

            if (caster->IsValidAttackTarget(target))
            {
                _heal = false;
                if (!caster->HasInArc(static_cast<float>(M_PI), target))
                    return SpellCastResult::SPELL_FAILED_UNIT_NOT_INFRONT;
                return SpellCastResult::SPELL_CAST_OK;
            }

            if (Guardian* pet = caster->GetGuardianPet())
            {
                if (pet->GetEntry() == NPC_MAGE_WATER_ELEMENTAL)
                {
                    _heal = true;
                    GetSpell()->m_targets.SetUnitTarget(pet);
                    return SpellCastResult::SPELL_CAST_OK;
                }
            }

            return SpellCastResult::SPELL_FAILED_BAD_TARGETS;
        }

        void PreventHostileEffect(SpellEffIndex effIndex)
        {
            if (_heal)
                PreventHitDefaultEffect(effIndex);
        }

        void HandleHeal(SpellEffIndex /*effIndex*/)
        {
            if (!_heal)
                return;

            if (Guardian* pet = GetCaster()->GetGuardianPet())
                GetCaster()->CastSpell(pet, SPELL_MAGE_FROSTBOLT_HEAL, true);
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_mage_frostbolt_SpellScript::CheckCast);
            OnEffectLaunchTarget += SpellEffectFn(spell_mage_frostbolt_SpellScript::PreventHostileEffect, EFFECT_ALL, SPELL_EFFECT_SCHOOL_DAMAGE);
            if (m_scriptSpellId == SPELL_MAGE_FROSTBOLT)
                OnEffectHitTarget += SpellEffectFn(spell_mage_frostbolt_SpellScript::PreventHostileEffect, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            OnEffectHitTarget += SpellEffectFn(spell_mage_frostbolt_SpellScript::HandleHeal, EFFECT_ALL, SPELL_EFFECT_SCRIPT_EFFECT);
        }

    private:
        bool _heal;
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_frostbolt_SpellScript();
    }
};

// 12472 - Icy Veins, 131078 - Icy Veins (Glyph of Icy Veins)
class spell_mage_icy_veins_cooldown : public SpellScriptLoader
{
public:
    spell_mage_icy_veins_cooldown() : SpellScriptLoader("spell_mage_icy_veins_cooldown") { }

    class spell_mage_icy_veins_cooldown_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_icy_veins_cooldown_SpellScript);

        bool Load() OVERRIDE
        {
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void HandleAfterCast()
        {
            Player* player = GetCaster()->ToPlayer();
            uint32 otherSpell = GetSpellInfo()->Id == SPELL_MAGE_ICY_VEINS ? SPELL_MAGE_ICY_VEINS_GLYPHED : SPELL_MAGE_ICY_VEINS;
            uint32 cooldown = player->GetSpellCooldownDelay(GetSpellInfo()->Id);
            if (!cooldown)
                return;

            player->AddSpellCooldown(otherSpell, 0, time(NULL) + cooldown);
            player->SendSpellCooldown(otherSpell, cooldown * IN_MILLISECONDS);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_mage_icy_veins_cooldown_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_icy_veins_cooldown_SpellScript();
    }
};

// 44614 - Frostfire Bolt
class spell_mage_frostfire_bolt : public SpellScriptLoader
{
public:
    spell_mage_frostfire_bolt() : SpellScriptLoader("spell_mage_frostfire_bolt") { }

    class spell_mage_frostfire_bolt_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_frostfire_bolt_SpellScript);

        void HandleAfterCast()
        {
            Unit* caster = GetCaster();
            Aura* brainFreeze = caster->GetAura(SPELL_MAGE_BRAIN_FREEZE_TRIGGERED);
            if (!brainFreeze || GetSpell()->m_appliedMods.find(brainFreeze) == GetSpell()->m_appliedMods.end())
                return;

            // An instant Brain Freeze bolt must not also eat a Presence of Mind charge
            if (Player* player = caster->ToPlayer())
                player->RestoreSpellMods(GetSpell(), SPELL_MAGE_PRESENCE_OF_MIND);

            if (Aura const* bonus = caster->GetAura(SPELL_MAGE_T16_4P_BONUS))
                if (Unit* target = GetExplTargetUnit())
                    if (roll_chance_i(bonus->GetSpellInfo()->Effects[EFFECT_1].CalcValue(caster)))
                        caster->CastSpell(target, SPELL_MAGE_FRIGID_BLAST, true);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_mage_frostfire_bolt_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_frostfire_bolt_SpellScript();
    }
};

// 42208 - Blizzard
class spell_mage_blizzard : public SpellScriptLoader
{
public:
    spell_mage_blizzard() : SpellScriptLoader("spell_mage_blizzard") { }

    class spell_mage_blizzard_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_blizzard_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_MAGE_BLIZZARD_SLOW);
        }

        void AddChillEffect(SpellEffIndex /*effIndex*/)
        {
            if (Unit* target = GetHitUnit())
                GetCaster()->CastSpell(target, SPELL_MAGE_BLIZZARD_SLOW, true);
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_mage_blizzard_SpellScript::AddChillEffect, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_blizzard_SpellScript();
    }
};

// 84718 - Frozen Orb (damage trigger, cast by the orb)
class spell_mage_frozen_orb_damage_trigger : public SpellScriptLoader
{
public:
    spell_mage_frozen_orb_damage_trigger() : SpellScriptLoader("spell_mage_frozen_orb_damage_trigger") { }

    class spell_mage_frozen_orb_damage_trigger_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_frozen_orb_damage_trigger_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_MAGE_FROZEN_ORB_DAMAGE);
        }

        void HandleAfterCast()
        {
            Unit* orb = GetCaster();
            if (Unit* mage = GetFrozenOrbOwner(orb))
                mage->CastSpell(orb->GetPositionX(), orb->GetPositionY(), orb->GetPositionZ(), SPELL_MAGE_FROZEN_ORB_DAMAGE, true);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_mage_frozen_orb_damage_trigger_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_frozen_orb_damage_trigger_SpellScript();
    }
};

// 84721 - Frozen Orb (damage)
class spell_mage_frozen_orb_damage : public SpellScriptLoader
{
public:
    spell_mage_frozen_orb_damage() : SpellScriptLoader("spell_mage_frozen_orb_damage") { }

    class spell_mage_frozen_orb_damage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_frozen_orb_damage_SpellScript);

        void HandleHit()
        {
            if (roll_chance_i(15))
                GetCaster()->CastSpell(GetCaster(), SPELL_MAGE_FINGERS_OF_FROST, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_mage_frozen_orb_damage_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_frozen_orb_damage_SpellScript();
    }
};

// 112948 - Frost Bomb
class spell_mage_frost_bomb_aura : public SpellScriptLoader
{
public:
    spell_mage_frost_bomb_aura() : SpellScriptLoader("spell_mage_frost_bomb_aura") { }

    class spell_mage_frost_bomb_aura_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_frost_bomb_aura_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_MAGE_FROST_BOMB_TRIGGERED);
        }

        void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
                return;

            if (Unit* caster = GetCaster())
            {
                caster->CastSpell(GetTarget(), SPELL_MAGE_FROST_BOMB_TRIGGERED, true);
                if (caster->HasAura(SPELL_MAGE_BRAIN_FREEZE))
                    caster->CastSpell(caster, SPELL_MAGE_BRAIN_FREEZE_TRIGGERED, true);
            }
        }

        void Register() OVERRIDE
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_mage_frost_bomb_aura_AuraScript::AfterRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_mage_frost_bomb_aura_AuraScript();
    }
};

// 113092 - Frost Bomb (explosion)
class spell_mage_frost_bomb : public SpellScriptLoader
{
public:
    spell_mage_frost_bomb() : SpellScriptLoader("spell_mage_frost_bomb") { }

    class spell_mage_frost_bomb_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_frost_bomb_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (Unit* mainTarget = GetExplTargetUnit())
                targets.remove(mainTarget);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_frost_bomb_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_frost_bomb_SpellScript();
    }
};

// 79684 - Arcane Missiles (proc aura)
class spell_mage_arcane_missiles_proc : public SpellScriptLoader
{
public:
    spell_mage_arcane_missiles_proc() : SpellScriptLoader("spell_mage_arcane_missiles_proc") { }

    class spell_mage_arcane_missiles_proc_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_arcane_missiles_proc_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (eventInfo.GetHitMask() & PROC_EX_INTERNAL_TRIGGERED)
                return false;

            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            if (!spellInfo)
                return false;

            return spellInfo->HasEffect(SPELL_EFFECT_SCHOOL_DAMAGE) || spellInfo->HasAura(SPELL_AURA_PERIODIC_DAMAGE) || spellInfo->Id == SPELL_MAGE_FROST_BOMB;
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_mage_arcane_missiles_proc_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_mage_arcane_missiles_proc_AuraScript();
    }
};

// 79683 - Arcane Missiles!
class spell_mage_arcane_missiles_proc_visual : public SpellScriptLoader
{
public:
    spell_mage_arcane_missiles_proc_visual() : SpellScriptLoader("spell_mage_arcane_missiles_proc_visual") { }

    class spell_mage_arcane_missiles_proc_visual_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_arcane_missiles_proc_visual_AuraScript);

        // Stacks are spent by the Arcane Missiles channel, never by procs
        bool CheckProc(ProcEventInfo& /*eventInfo*/)
        {
            return false;
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* owner = GetUnitOwner();
            if (GetStackAmount() >= 2)
                owner->CastSpell(owner, SPELL_MAGE_ARCANE_MISSILES_DOUBLE, true);
            else
                owner->RemoveAurasDueToSpell(SPELL_MAGE_ARCANE_MISSILES_DOUBLE);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetUnitOwner()->RemoveAurasDueToSpell(SPELL_MAGE_ARCANE_MISSILES_DOUBLE);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_mage_arcane_missiles_proc_visual_AuraScript::CheckProc);
            AfterEffectApply += AuraEffectApplyFn(spell_mage_arcane_missiles_proc_visual_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            AfterEffectRemove += AuraEffectRemoveFn(spell_mage_arcane_missiles_proc_visual_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_mage_arcane_missiles_proc_visual_AuraScript();
    }
};

// 5143 - Arcane Missiles
class spell_mage_arcane_missiles : public SpellScriptLoader
{
public:
    spell_mage_arcane_missiles() : SpellScriptLoader("spell_mage_arcane_missiles") { }

    class spell_mage_arcane_missiles_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_arcane_missiles_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_MAGE_ARCANE_CHARGE) && sSpellMgr->GetSpellInfo(SPELL_MAGE_ARCANE_MISSILES);
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* mage = GetUnitOwner();
            if (AuraEffect const* bonus = mage->GetAuraEffect(SPELL_MAGE_T16_4P_BONUS, EFFECT_0))
                if (roll_chance_i(bonus->GetAmount()))
                    return;

            if (Aura* missiles = mage->GetAura(SPELL_MAGE_ARCANE_MISSILES))
                missiles->ModStackAmount(-1);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* mage = GetUnitOwner();
            mage->CastSpell(mage, SPELL_MAGE_ARCANE_CHARGE, true);

            // The channel is still finishing here; consuming Ice Floes now would interrupt it
            mage->m_Events.Schedule(1, [mage]()
            {
                if (Aura* iceFloes = mage->GetAura(SPELL_MAGE_ICE_FLOES))
                    iceFloes->ModStackAmount(-1);
            });
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_mage_arcane_missiles_AuraScript::HandleApply, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_mage_arcane_missiles_AuraScript::HandleRemove, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_mage_arcane_missiles_AuraScript();
    }
};

// 44425 - Arcane Barrage
class spell_mage_arcane_barrage : public SpellScriptLoader
{
public:
    spell_mage_arcane_barrage() : SpellScriptLoader("spell_mage_arcane_barrage") { }

    class spell_mage_arcane_barrage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_arcane_barrage_SpellScript);

        void HandleHit()
        {
            if (GetHitUnit() != GetExplTargetUnit())
                SetHitDamage(CalculatePct(GetHitDamage(), GetSpellInfo()->Effects[EFFECT_1].CalcValue(GetCaster())));
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_mage_arcane_barrage_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_arcane_barrage_SpellScript();
    }
};

// 36032 - Arcane Charge
// The aura has a single proc charge shared by all its spell mods; only Arcane Barrage consumes the stacks
class spell_mage_arcane_charge : public SpellScriptLoader
{
public:
    spell_mage_arcane_charge() : SpellScriptLoader("spell_mage_arcane_charge") { }

    class spell_mage_arcane_charge_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_arcane_charge_AuraScript);

        void HandleDropModCharge(Spell* bySpell)
        {
            if (!bySpell || bySpell->GetSpellInfo()->Id != SPELL_MAGE_ARCANE_BARRAGE)
                PreventDefaultAction();
        }

        void Register() OVERRIDE
        {
            OnDropModCharge += AuraDropModChargeFn(spell_mage_arcane_charge_AuraScript::HandleDropModCharge);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_mage_arcane_charge_AuraScript();
    }
};

// 1449 - Arcane Explosion
class spell_mage_arcane_explosion : public SpellScriptLoader
{
public:
    spell_mage_arcane_explosion() : SpellScriptLoader("spell_mage_arcane_explosion") { }

    class spell_mage_arcane_explosion_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_arcane_explosion_SpellScript);

        bool Load() OVERRIDE
        {
            _canGenerateCharge = false;
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            _canGenerateCharge = !targets.empty();
        }

        void HandleAfterCast()
        {
            Player* caster = GetCaster()->ToPlayer();
            if (caster->GetTalentSpecialization(caster->GetActiveSpec()) != SPEC_MAGE_ARCANE)
                return;

            if (Aura* charges = caster->GetAura(SPELL_MAGE_ARCANE_CHARGE))
                charges->RefreshDuration();

            if (_canGenerateCharge && roll_chance_i(GetSpellInfo()->Effects[EFFECT_1].CalcValue(caster)))
                caster->CastSpell(caster, SPELL_MAGE_ARCANE_CHARGE, true);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_arcane_explosion_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            AfterCast += SpellCastFn(spell_mage_arcane_explosion_SpellScript::HandleAfterCast);
        }

    private:
        bool _canGenerateCharge;
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_arcane_explosion_SpellScript();
    }
};

// 12051 - Evocation
class spell_mage_evocation : public SpellScriptLoader
{
public:
    spell_mage_evocation() : SpellScriptLoader("spell_mage_evocation") { }

    class spell_mage_evocation_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_evocation_AuraScript);

        void CalculateHealthGain(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            Unit* caster = GetCaster();
            if (!caster || !caster->HasAura(SPELL_MAGE_GLYPH_OF_EVOCATION) || caster->HasAura(SPELL_MAGE_INVOCATION))
                amount = 0;
        }

        void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            caster->RemoveAurasDueToSpell(SPELL_MAGE_ARCANE_CHARGE);

            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE || !caster->HasAura(SPELL_MAGE_INVOCATION))
                return;

            caster->CastSpell(caster, SPELL_MAGE_INVOKERS_ENERGY, true);

            if (caster->HasAura(SPELL_MAGE_GLYPH_OF_EVOCATION))
            {
                uint32 healthGain = caster->CountPctFromMaxHealth(10);
                healthGain = caster->SpellHealingBonusDone(caster, GetSpellInfo(), healthGain, HEAL);
                healthGain = caster->SpellHealingBonusTaken(caster, GetSpellInfo(), healthGain, HEAL);
                caster->HealBySpell(caster, GetSpellInfo(), healthGain, false);
            }
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mage_evocation_AuraScript::CalculateHealthGain, EFFECT_1, SPELL_AURA_OBS_MOD_HEALTH);
            AfterEffectRemove += AuraEffectRemoveFn(spell_mage_evocation_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_OBS_MOD_POWER, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_mage_evocation_AuraScript();
    }
};

// 1463 - Incanter's Ward
class spell_mage_incanters_ward : public SpellScriptLoader
{
public:
    spell_mage_incanters_ward() : SpellScriptLoader("spell_mage_incanters_ward") { }

    class spell_mage_incanters_ward_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_incanters_ward_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_MAGE_INCANTERS_WARD_ENERGIZE) && sSpellMgr->GetSpellInfo(SPELL_MAGE_INCANTERS_ABSORPTION);
        }

        bool Load() OVERRIDE
        {
            _absorbTotal = 0.0f;
            _absorbMax = 0.0f;
            return true;
        }

        void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            _absorbMax = float(aurEff->GetAmount());
        }

        void HandleAbsorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
        {
            if (_absorbMax <= 0.0f)
                return;

            Unit* mage = GetUnitOwner();
            uint32 absorbed = std::min(dmgInfo.GetDamage(), absorbAmount);
            _absorbTotal = std::min(_absorbTotal + float(absorbed), _absorbMax);

            if (AuraEffect const* manaEffect = GetEffect(EFFECT_1))
            {
                float pct = manaEffect->GetAmount() * float(absorbed) / _absorbMax;
                int32 manaGain = CalculatePct(int32(mage->GetMaxPower(POWER_MANA)), pct);
                if (manaGain > 0)
                    mage->CastCustomSpell(SPELL_MAGE_INCANTERS_WARD_ENERGIZE, SPELLVALUE_BASE_POINT0, manaGain, mage, true);
            }
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (_absorbMax <= 0.0f)
                return;

            int32 cap = sSpellMgr->GetSpellInfo(SPELL_MAGE_INCANTERS_ABSORPTION)->Effects[EFFECT_0].BasePoints;
            int32 damageGain = CalculatePct(cap, _absorbTotal / _absorbMax * 100.0f);
            if (damageGain > 0)
                GetUnitOwner()->CastCustomSpell(SPELL_MAGE_INCANTERS_ABSORPTION, SPELLVALUE_BASE_POINT0, damageGain, GetUnitOwner(), true);
        }

        void Register() OVERRIDE
        {
            AfterEffectApply += AuraEffectApplyFn(spell_mage_incanters_ward_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
            OnEffectAbsorb += AuraEffectAbsorbFn(spell_mage_incanters_ward_AuraScript::HandleAbsorb, EFFECT_0);
            OnEffectRemove += AuraEffectRemoveFn(spell_mage_incanters_ward_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
        }

    private:
        float _absorbTotal;
        float _absorbMax;
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_mage_incanters_ward_AuraScript();
    }
};

// 118859 - Incanter's Ward (cooldown marker)
class spell_mage_incanters_ward_cooldown : public SpellScriptLoader
{
public:
    spell_mage_incanters_ward_cooldown() : SpellScriptLoader("spell_mage_incanters_ward_cooldown") { }

    class spell_mage_incanters_ward_cooldown_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_incanters_ward_cooldown_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_MAGE_INCANTERS_ABSORPTION_PASSIVE);
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->RemoveAurasDueToSpell(SPELL_MAGE_INCANTERS_ABSORPTION_PASSIVE);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            if (target->HasSpell(SPELL_MAGE_INCANTERS_ABSORPTION_PASSIVE) && !target->HasAura(SPELL_MAGE_INCANTERS_ABSORPTION_PASSIVE))
                target->CastSpell(target, SPELL_MAGE_INCANTERS_ABSORPTION_PASSIVE, true);
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_mage_incanters_ward_cooldown_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_mage_incanters_ward_cooldown_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_mage_incanters_ward_cooldown_AuraScript();
    }
};

// 116011 - Rune of Power
class spell_mage_rune_of_power : public SpellScriptLoader
{
public:
    spell_mage_rune_of_power() : SpellScriptLoader("spell_mage_rune_of_power") { }

    class spell_mage_rune_of_power_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_rune_of_power_SpellScript);

        // At most two runes: the one being placed and the newest existing one
        void RemoveOldRunes()
        {
            std::list<AreaTrigger*> runes;
            GetCaster()->GetAreaTriggerList(runes, SPELL_MAGE_RUNE_OF_POWER);
            while (runes.size() > 1)
            {
                runes.front()->Remove();
                runes.pop_front();
            }
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_mage_rune_of_power_SpellScript::RemoveOldRunes);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_rune_of_power_SpellScript();
    }
};

// 304 - Rune of Power (area trigger)
class sat_mage_rune_of_power : public IAreaTriggerAura
{
    bool CheckTriggering(WorldObject* triggering) OVERRIDE
    {
        return triggering == m_caster && m_target->IsWithinDistInMap(triggering, m_range);
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
        Player* mage = triggering->ToPlayer();
        if (!mage)
            return;

        std::list<AreaTrigger*> runes;
        mage->GetAreaTriggerList(runes, SPELL_MAGE_RUNE_OF_POWER);
        for (std::list<AreaTrigger*>::const_iterator itr = runes.begin(); itr != runes.end(); ++itr)
            if (*itr != GetAreaTrigger() && (*itr)->GetExactDist(mage) < GetRange())
                return;

        mage->RemoveAurasDueToSpell(SPELL_MAGE_RUNE_OF_POWER_AURA);
    }

    void ApplyAura(WorldObject* triggering)
    {
        if (Player* mage = triggering->ToPlayer())
            if (!mage->HasAura(SPELL_MAGE_RUNE_OF_POWER_AURA))
                mage->CastSpell(mage, SPELL_MAGE_RUNE_OF_POWER_AURA, true);
    }
};

// 114924 - Nether Tempest (secondary target selector)
class spell_mage_nether_tempest_selector : public SpellScriptLoader
{
public:
    spell_mage_nether_tempest_selector() : SpellScriptLoader("spell_mage_nether_tempest_selector") { }

    class spell_mage_nether_tempest_selector_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_nether_tempest_selector_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_MAGE_NETHER_TEMPEST_DIRECT_DAMAGE) && sSpellMgr->GetSpellInfo(SPELL_MAGE_NETHER_TEMPEST_MISSILE);
        }

        void SelectTarget(std::list<WorldObject*>& targets)
        {
            if (Unit* mainTarget = GetExplTargetUnit())
                targets.remove(mainTarget);

            Unit* caster = GetCaster();
            targets.remove_if([caster](WorldObject* target)
            {
                Unit* unit = target->ToUnit();
                if (!unit || !unit->IsInCombat())
                    return true;

                if (unit->GetTypeId() == TypeID::TYPEID_PLAYER)
                    return false;

                return !unit->CanHaveThreatList() || !unit->getThreatManager().getThreat(caster);
            });

            if (targets.size() > 1)
                Skyfire::Containers::RandomResizeList(targets, 1);
        }

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            Unit* mainTarget = GetExplTargetUnit();
            Unit* target = GetHitUnit();
            if (!mainTarget || !target)
                return;

            GetCaster()->CastSpell(target, SPELL_MAGE_NETHER_TEMPEST_DIRECT_DAMAGE, true);
            mainTarget->CastSpell(target, SPELL_MAGE_NETHER_TEMPEST_MISSILE, true);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_nether_tempest_selector_SpellScript::SelectTarget, EFFECT_1, TARGET_UNIT_DEST_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_mage_nether_tempest_selector_SpellScript::HandleHit, EFFECT_1, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_nether_tempest_selector_SpellScript();
    }
};

// 114923 - Nether Tempest
class spell_mage_nether_tempest : public SpellScriptLoader
{
public:
    spell_mage_nether_tempest() : SpellScriptLoader("spell_mage_nether_tempest") { }

    class spell_mage_nether_tempest_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_nether_tempest_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_MAGE_NETHER_TEMPEST_SELECTOR);
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
                caster->BindAura(GetAura());
        }

        void HandleTick(AuraEffect const* /*aurEff*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            caster->CastSpell(GetTarget(), SPELL_MAGE_NETHER_TEMPEST_SELECTOR, true);

            // Only the most recent Nether Tempest rolls for Brain Freeze
            if (Unit::AuraList const* tempests = caster->GetBoundAurasBySpellId(GetId()))
                if (tempests->back() != GetAura())
                    return;

            if (caster->HasAura(SPELL_MAGE_BRAIN_FREEZE) && roll_chance_i(10))
                caster->CastSpell(caster, SPELL_MAGE_BRAIN_FREEZE_TRIGGERED, true);
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_mage_nether_tempest_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_mage_nether_tempest_AuraScript::HandleTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_mage_nether_tempest_AuraScript();
    }
};

// 32612 - Invisibility
class spell_mage_invisibility : public SpellScriptLoader
{
public:
    spell_mage_invisibility() : SpellScriptLoader("spell_mage_invisibility") { }

    class spell_mage_invisibility_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_invisibility_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* mage = GetTarget()->ToPlayer())
                if (Pet* pet = mage->GetPet())
                    pet->CastSpell(pet, GetId(), true);
        }

        void Register() OVERRIDE
        {
            AfterEffectApply += AuraEffectApplyFn(spell_mage_invisibility_AuraScript::HandleApply, EFFECT_1, SPELL_AURA_MOD_INVISIBILITY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_mage_invisibility_AuraScript();
    }
};

// 110960 - Greater Invisibility
class spell_mage_greater_invisibility : public SpellScriptLoader
{
public:
    spell_mage_greater_invisibility() : SpellScriptLoader("spell_mage_greater_invisibility") { }

    class spell_mage_greater_invisibility_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_greater_invisibility_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_MAGE_GREATER_INVISIBILITY_LESS_DAMAGE) && sSpellMgr->GetSpellInfo(SPELL_MAGE_REMOVE_INVISIBILITY_REMOVED_TIMER);
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            if (Player* mage = target->ToPlayer())
                if (Pet* pet = mage->GetPet())
                    pet->CastSpell(pet, GetId(), true);

            if (!target->GetInstanceScript() || !target->GetInstanceScript()->IsEncounterInProgress())
                target->CombatStop();

            uint32 remaining = 2;
            AuraType const dotTypes[2] = { SPELL_AURA_PERIODIC_DAMAGE, SPELL_AURA_PERIODIC_DAMAGE_PERCENT };
            for (uint8 i = 0; i < 2 && remaining; ++i)
            {
                while (remaining && !target->GetAuraEffectsByType(dotTypes[i]).empty())
                {
                    target->GetAuraEffectsByType(dotTypes[i]).front()->GetBase()->Remove();
                    --remaining;
                }
            }

            target->CastSpell(target, SPELL_MAGE_GREATER_INVISIBILITY_LESS_DAMAGE, true);
            if (target->CanHaveThreatList())
                target->getThreatManager().resetAllAggro();
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            target->CastSpell(target, SPELL_MAGE_REMOVE_INVISIBILITY_REMOVED_TIMER, true);

            Aura* timer = target->GetAura(SPELL_MAGE_REMOVE_INVISIBILITY_REMOVED_TIMER);
            Aura* reduction = target->GetAura(SPELL_MAGE_GREATER_INVISIBILITY_LESS_DAMAGE);
            if (!timer || !reduction)
                return;

            // Glyph of Combustion's duration mod also hits the timer; undo it
            if (target->HasAura(SPELL_MAGE_GLYPH_OF_COMBUSTION))
            {
                timer->SetMaxDuration(timer->GetMaxDuration() / 2);
                timer->SetDuration(timer->GetMaxDuration());
            }

            reduction->SetMaxDuration(timer->GetDuration());
            reduction->SetDuration(timer->GetDuration());
        }

        void Register() OVERRIDE
        {
            AfterEffectApply += AuraEffectApplyFn(spell_mage_greater_invisibility_AuraScript::HandleApply, EFFECT_1, SPELL_AURA_MOD_INVISIBILITY, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_mage_greater_invisibility_AuraScript::HandleRemove, EFFECT_1, SPELL_AURA_MOD_INVISIBILITY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_mage_greater_invisibility_AuraScript();
    }
};

// 113862 - Greater Invisibility (damage reduction)
class spell_mage_greater_invisibility_damage_reduction : public SpellScriptLoader
{
public:
    spell_mage_greater_invisibility_damage_reduction() : SpellScriptLoader("spell_mage_greater_invisibility_damage_reduction") { }

    class spell_mage_greater_invisibility_damage_reduction_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_greater_invisibility_damage_reduction_AuraScript);

        void HandleUpdate(uint32 /*diff*/)
        {
            Unit* owner = GetUnitOwner();
            if (!owner->HasAura(SPELL_MAGE_GREATER_INVISIBILITY) && !owner->HasAura(SPELL_MAGE_REMOVE_INVISIBILITY_REMOVED_TIMER))
                Remove();
        }

        void Register() OVERRIDE
        {
            OnAuraUpdate += AuraUpdateFn(spell_mage_greater_invisibility_damage_reduction_AuraScript::HandleUpdate);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_mage_greater_invisibility_damage_reduction_AuraScript();
    }
};

// 115610 - Temporal Shield
class spell_mage_temporal_shield : public SpellScriptLoader
{
public:
    spell_mage_temporal_shield() : SpellScriptLoader("spell_mage_temporal_shield") { }

    class spell_mage_temporal_shield_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_temporal_shield_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_MAGE_TEMPORAL_SHIELD_HEAL);
        }

        bool Load() OVERRIDE
        {
            _damageTaken = 0;
            return true;
        }

        void HandleProc(ProcEventInfo& eventInfo)
        {
            if (!eventInfo.GetDamageInfo())
                return;

            uint32 damage = eventInfo.GetDamageInfo()->GetDamage();
            if (damage && eventInfo.GetActor() && !GetUnitOwner()->IsFriendlyTo(eventInfo.GetActor()))
                _damageTaken += damage;
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (!_damageTaken)
                return;

            Unit* mage = GetUnitOwner();
            int32 heal = CalculatePct(int32(_damageTaken), GetSpellInfo()->Effects[EFFECT_0].CalcValue(mage));
            // Healed back over the three ticks of the periodic heal
            if (heal / 3 > 0)
                mage->CastCustomSpell(SPELL_MAGE_TEMPORAL_SHIELD_HEAL, SPELLVALUE_BASE_POINT0, heal / 3, mage, true);
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_mage_temporal_shield_AuraScript::HandleProc);
            OnEffectRemove += AuraEffectRemoveFn(spell_mage_temporal_shield_AuraScript::HandleRemove, EFFECT_1, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_REAL);
        }

    private:
        uint32 _damageTaken;
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_mage_temporal_shield_AuraScript();
    }
};

// 86949 - Cauterize
class spell_mage_cauterize : public SpellScriptLoader
{
public:
    spell_mage_cauterize() : SpellScriptLoader("spell_mage_cauterize") { }

    class spell_mage_cauterize_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_cauterize_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_MAGE_CAUTERIZE) && sSpellMgr->GetSpellInfo(SPELL_MAGE_CAUTERIZE_MARKER);
        }

        void Absorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
        {
            absorbAmount = 0;

            Unit* target = GetTarget();
            if (dmgInfo.GetDamage() < target->GetHealth() || target->HasAura(SPELL_MAGE_CAUTERIZE_MARKER))
                return;

            AuraEffect const* healthEffect = GetEffect(EFFECT_1);
            if (!healthEffect)
                return;

            int32 bp1 = int32(target->CountPctFromMaxHealth(healthEffect->GetAmount()));
            target->CastCustomSpell(target, SPELL_MAGE_CAUTERIZE, NULL, &bp1, NULL, true);
            target->CastSpell(target, SPELL_MAGE_CAUTERIZE_MARKER, true);

            absorbAmount = dmgInfo.GetDamage();
        }

        void Register() OVERRIDE
        {
            OnEffectAbsorb += AuraEffectAbsorbFn(spell_mage_cauterize_AuraScript::Absorb, EFFECT_0);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_mage_cauterize_AuraScript();
    }
};

// 140468 - Flameglow
class spell_mage_flameglow : public SpellScriptLoader
{
public:
    spell_mage_flameglow() : SpellScriptLoader("spell_mage_flameglow") { }

    class spell_mage_flameglow_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_flameglow_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            amount = -1;
        }

        void Absorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
        {
            Unit* target = GetTarget();
            if (target->GetTypeId() != TypeID::TYPEID_PLAYER)
            {
                absorbAmount = 0;
                return;
            }

            int32 spellPowerPct = GetSpellInfo()->Effects[EFFECT_1].BasePoints;
            int32 damagePct = GetSpellInfo()->Effects[EFFECT_2].BasePoints;
            uint32 spellAbsorb = uint32(CalculatePct(std::max(target->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FIRE), 0), spellPowerPct));
            uint32 absorbLimit = CalculatePct(dmgInfo.GetDamage(), damagePct);
            absorbAmount = std::min(spellAbsorb, absorbLimit);
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mage_flameglow_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            OnEffectAbsorb += AuraEffectAbsorbFn(spell_mage_flameglow_AuraScript::Absorb, EFFECT_0);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_mage_flameglow_AuraScript();
    }
};

// 63092 - Glyph of Illusion
class spell_mage_glyph_of_illusion : public SpellScriptLoader
{
public:
    spell_mage_glyph_of_illusion() : SpellScriptLoader("spell_mage_glyph_of_illusion") { }

    class spell_mage_glyph_of_illusion_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_glyph_of_illusion_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_MAGE_GLYPH_OF_ILLUSION_ACTIVE);
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* player = GetTarget()->ToPlayer())
                player->learnSpell(SPELL_MAGE_GLYPH_OF_ILLUSION_ACTIVE, false);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* player = GetTarget()->ToPlayer())
                player->removeSpell(SPELL_MAGE_GLYPH_OF_ILLUSION_ACTIVE);
        }

        void Register() OVERRIDE
        {
            AfterEffectApply += AuraEffectApplyFn(spell_mage_glyph_of_illusion_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_mage_glyph_of_illusion_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_mage_glyph_of_illusion_AuraScript();
    }
};

// 126748 - Glyph of Conjure Familiar
class spell_mage_glyph_of_conjure_familiar : public SpellScriptLoader
{
public:
    spell_mage_glyph_of_conjure_familiar() : SpellScriptLoader("spell_mage_glyph_of_conjure_familiar") { }

    class spell_mage_glyph_of_conjure_familiar_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_glyph_of_conjure_familiar_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_MAGE_CONJURE_FAMILIAR);
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* player = GetTarget()->ToPlayer())
                player->learnSpell(SPELL_MAGE_CONJURE_FAMILIAR, false);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* player = GetTarget()->ToPlayer())
                player->removeSpell(SPELL_MAGE_CONJURE_FAMILIAR);
        }

        void Register() OVERRIDE
        {
            AfterEffectApply += AuraEffectApplyFn(spell_mage_glyph_of_conjure_familiar_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_mage_glyph_of_conjure_familiar_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_mage_glyph_of_conjure_familiar_AuraScript();
    }
};

// 146976 - Glyph of the Unbound Elemental
class spell_mage_glyph_of_unbound_elemental : public SpellScriptLoader
{
public:
    spell_mage_glyph_of_unbound_elemental() : SpellScriptLoader("spell_mage_glyph_of_unbound_elemental") { }

    class spell_mage_glyph_of_unbound_elemental_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_glyph_of_unbound_elemental_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_MAGE_UNBOUND_WATER_ELEMENTAL_TRANSFORM);
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Guardian* pet = GetUnitOwner()->GetGuardianPet())
                pet->CastSpell(pet, SPELL_MAGE_UNBOUND_WATER_ELEMENTAL_TRANSFORM, true);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Guardian* pet = GetUnitOwner()->GetGuardianPet())
                pet->RemoveAurasDueToSpell(SPELL_MAGE_UNBOUND_WATER_ELEMENTAL_TRANSFORM);
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_mage_glyph_of_unbound_elemental_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_mage_glyph_of_unbound_elemental_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_mage_glyph_of_unbound_elemental_AuraScript();
    }
};

// 1953 - Blink
class spell_mage_glyph_of_rapid_displacement : public SpellScriptLoader
{
public:
    spell_mage_glyph_of_rapid_displacement() : SpellScriptLoader("spell_mage_glyph_of_rapid_displacement") { }

    class spell_mage_glyph_of_rapid_displacement_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_glyph_of_rapid_displacement_SpellScript);

        SpellCastResult CheckCast()
        {
            if (GetCaster()->HasAura(SPELL_MAGE_GLYPH_OF_RAPID_DISPLACEMENT) && GetCaster()->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED))
                return SpellCastResult::SPELL_FAILED_STUNNED;
            return SpellCastResult::SPELL_CAST_OK;
        }

        void PreventImmunity(SpellEffIndex effIndex)
        {
            if (GetCaster()->HasAura(SPELL_MAGE_GLYPH_OF_RAPID_DISPLACEMENT))
                PreventHitDefaultEffect(effIndex);
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_mage_glyph_of_rapid_displacement_SpellScript::CheckCast);
            OnEffectHitTarget += SpellEffectFn(spell_mage_glyph_of_rapid_displacement_SpellScript::PreventImmunity, EFFECT_1, SPELL_EFFECT_APPLY_AURA);
            OnEffectHitTarget += SpellEffectFn(spell_mage_glyph_of_rapid_displacement_SpellScript::PreventImmunity, EFFECT_2, SPELL_EFFECT_APPLY_AURA);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_glyph_of_rapid_displacement_SpellScript();
    }
};

// 1953 - Blink
class spell_mage_glyph_of_momentum : public SpellScriptLoader
{
public:
    spell_mage_glyph_of_momentum() : SpellScriptLoader("spell_mage_glyph_of_momentum") { }

    class spell_mage_glyph_of_momentum_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_glyph_of_momentum_SpellScript);

        void HandleCast()
        {
            if (GetCaster()->HasAura(SPELL_MAGE_GLYPH_OF_MOMENTUM))
                GetCaster()->CastSpell(GetCaster(), SPELL_MAGE_GLYPH_OF_MOMENTUM_BLINK, true);
        }

        void PreventBlink(SpellEffIndex effIndex)
        {
            if (GetCaster()->HasAura(SPELL_MAGE_GLYPH_OF_MOMENTUM))
                PreventHitEffect(effIndex);
        }

        void PreventAura(SpellEffIndex effIndex)
        {
            if (GetCaster()->HasAura(SPELL_MAGE_GLYPH_OF_MOMENTUM))
                PreventHitDefaultEffect(effIndex);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_mage_glyph_of_momentum_SpellScript::HandleCast);
            OnEffectHitTarget += SpellEffectFn(spell_mage_glyph_of_momentum_SpellScript::PreventBlink, EFFECT_0, SPELL_EFFECT_LEAP);
            OnEffectHitTarget += SpellEffectFn(spell_mage_glyph_of_momentum_SpellScript::PreventAura, EFFECT_1, SPELL_EFFECT_APPLY_AURA);
            OnEffectHitTarget += SpellEffectFn(spell_mage_glyph_of_momentum_SpellScript::PreventAura, EFFECT_2, SPELL_EFFECT_APPLY_AURA);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_glyph_of_momentum_SpellScript();
    }
};

// 119415 - Blink
class spell_mage_glyph_of_momentum_blink : public SpellScriptLoader
{
public:
    spell_mage_glyph_of_momentum_blink() : SpellScriptLoader("spell_mage_glyph_of_momentum_blink") { }

    class spell_mage_glyph_of_momentum_blink_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_glyph_of_momentum_blink_SpellScript);

        void SelectDest(SpellDestination& dest)
        {
            // Cast from Blink's OnCast after Blink selected its own destination; both must land on the same spot.
            if (Spell* spell = GetCaster()->GetCurrentSpell(CURRENT_GENERIC_SPELL))
                spell->m_targets.SetDst(dest);
        }

        void Register() OVERRIDE
        {
            OnDestinationTargetSelect += SpellDestinationTargetSelectFn(spell_mage_glyph_of_momentum_blink_SpellScript::SelectDest, EFFECT_0, TARGET_DEST_CASTER_MOVEMENT_DIR);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_glyph_of_momentum_blink_SpellScript();
    }
};

// Mage teleports and portals (EFFECT_1 script effect)
class spell_mage_glyph_of_rapid_teleportation : public SpellScriptLoader
{
public:
    spell_mage_glyph_of_rapid_teleportation() : SpellScriptLoader("spell_mage_glyph_of_rapid_teleportation") { }

    class spell_mage_glyph_of_rapid_teleportation_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_glyph_of_rapid_teleportation_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_MAGE_RAPID_TELEPORTATION);
        }

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            Unit* target = GetHitUnit();
            if (!target || !target->HasAura(SPELL_MAGE_GLYPH_OF_RAPID_TELEPORTATION))
                return;

            // Applied after the teleport has been processed
            target->m_Events.Schedule(1, [target]()
            {
                target->CastSpell(target, SPELL_MAGE_RAPID_TELEPORTATION, true);
            });
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_mage_glyph_of_rapid_teleportation_SpellScript::HandleHit, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_glyph_of_rapid_teleportation_SpellScript();
    }
};

// 98397 - Glyph of Armors
class spell_mage_glyph_of_armors : public SpellScriptLoader
{
public:
    spell_mage_glyph_of_armors() : SpellScriptLoader("spell_mage_glyph_of_armors") { }

    class spell_mage_glyph_of_armors_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_glyph_of_armors_AuraScript);

        void Recalculate(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (AuraEffect* mageArmor = GetUnitOwner()->GetAuraEffect(SPELL_MAGE_MAGE_ARMOR, EFFECT_1))
                mageArmor->RecalculateAmount();
        }

        void Register() OVERRIDE
        {
            AfterEffectApply += AuraEffectApplyFn(spell_mage_glyph_of_armors_AuraScript::Recalculate, EFFECT_0, SPELL_AURA_ADD_FLAT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_mage_glyph_of_armors_AuraScript::Recalculate, EFFECT_0, SPELL_AURA_ADD_FLAT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_mage_glyph_of_armors_AuraScript();
    }
};

// 118 - Polymorph
class spell_mage_polymorph : public SpellScriptLoader
{
public:
    spell_mage_polymorph() : SpellScriptLoader("spell_mage_polymorph") { }

    class spell_mage_polymorph_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_polymorph_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* mage = GetCaster();
            if (!mage)
                return;

            uint32 displayId = 0;
            if (mage->HasAura(SPELL_MAGE_GLYPH_OF_THE_MONKEY))
                displayId = DISPLAY_MAGE_POLYMORPH_MONKEY;
            else if (mage->HasAura(SPELL_MAGE_GLYPH_OF_THE_PORCUPINE))
                displayId = MagePolymorphPorcupineDisplayIds[urand(0, 2)];
            else if (mage->HasAura(SPELL_MAGE_GLYPH_OF_THE_BEAR_CUB))
                displayId = DISPLAY_MAGE_POLYMORPH_BEAR_CUB;

            if (displayId)
                GetTarget()->SetDisplayId(displayId);
        }

        void Register() OVERRIDE
        {
            AfterEffectApply += AuraEffectApplyFn(spell_mage_polymorph_AuraScript::HandleApply, EFFECT_1, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_mage_polymorph_AuraScript();
    }
};

// 31661 - Dragon's Breath
class spell_mage_dragons_breath : public SpellScriptLoader
{
public:
    spell_mage_dragons_breath() : SpellScriptLoader("spell_mage_dragons_breath") { }

    class spell_mage_dragons_breath_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_dragons_breath_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            return !spellInfo || spellInfo->Id != SPELL_MAGE_NETHER_TEMPEST_DIRECT_DAMAGE;
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_mage_dragons_breath_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_mage_dragons_breath_AuraScript();
    }
};

// 31589 - Slow
class spell_mage_slow : public SpellScriptLoader
{
public:
    spell_mage_slow() : SpellScriptLoader("spell_mage_slow") { }

    class spell_mage_slow_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_slow_SpellScript);

        void HandleOnHit()
        {
            Unit* target = GetHitUnit();
            if (!target || target->GetTypeId() != TypeID::TYPEID_PLAYER)
                return;

            if (Aura* slow = target->GetAura(SPELL_MAGE_SLOW, GetCaster()->GetGUID()))
            {
                slow->SetDuration(slow->GetMaxDuration() / 2);
                slow->SetMaxDuration(slow->GetDuration());
            }
        }

        // Glyph of Slow looks the active Slow up through the caster's bound auras
        void HandleAfterHit()
        {
            if (Aura* slow = GetHitAura())
                GetCaster()->BindAura(slow);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_mage_slow_SpellScript::HandleOnHit);
            AfterHit += SpellHitFn(spell_mage_slow_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_slow_SpellScript();
    }
};

// 110909 - Alter Time (Item - Mage T15 2P Bonus)
class spell_mage_t15_2p_bonus : public SpellScriptLoader
{
public:
    spell_mage_t15_2p_bonus() : SpellScriptLoader("spell_mage_t15_2p_bonus") { }

    class spell_mage_t15_2p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_t15_2p_bonus_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_MAGE_TIME_LORD);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* mage = GetUnitOwner();
            if (!mage->HasAura(SPELL_MAGE_T15_2P_BONUS))
                return;

            // Alter Time is still restoring the old state at this point
            mage->m_Events.Schedule(1, [mage]()
            {
                mage->CastSpell(mage, SPELL_MAGE_TIME_LORD, true);
            });
        }

        void Register() OVERRIDE
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_mage_t15_2p_bonus_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_mage_t15_2p_bonus_AuraScript();
    }
};

// 48108 - Pyroblast!, 57761 - Brain Freeze (Item - Mage T16 2P Bonus)
class spell_mage_t16_2p_bonus : public SpellScriptLoader
{
public:
    spell_mage_t16_2p_bonus() : SpellScriptLoader("spell_mage_t16_2p_bonus") { }

    class spell_mage_t16_2p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_t16_2p_bonus_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_MAGE_POTENT_FLAMES) && sSpellMgr->GetSpellInfo(SPELL_MAGE_FROZEN_THOUGHTS);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* owner = GetUnitOwner();
            if (!owner->HasAura(SPELL_MAGE_T16_2P_BONUS))
                return;

            // Spending the last charge removes the aura by expire while time is still left on it
            AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
            bool consumed = removeMode == AURA_REMOVE_BY_DEFAULT || (removeMode == AURA_REMOVE_BY_EXPIRE && GetDuration() > 0);
            if (!consumed)
                return;

            owner->CastSpell(owner, GetId() == SPELL_MAGE_BRAIN_FREEZE_TRIGGERED ? SPELL_MAGE_FROZEN_THOUGHTS : SPELL_MAGE_POTENT_FLAMES, true);
        }

        void Register() OVERRIDE
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_mage_t16_2p_bonus_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_mage_t16_2p_bonus_AuraScript();
    }
};

// 145257 - Item - Mage T16 4P Bonus
class spell_mage_t16_4p_bonus : public SpellScriptLoader
{
public:
    spell_mage_t16_4p_bonus() : SpellScriptLoader("spell_mage_t16_4p_bonus") { }

    class spell_mage_t16_4p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_t16_4p_bonus_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_MAGE_FIERY_ADEPT);
        }

        void HandleProc(ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            if (spellInfo && spellInfo->Id == SPELL_MAGE_INFERNO_BLAST)
                GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_MAGE_FIERY_ADEPT, true);
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_mage_t16_4p_bonus_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_mage_t16_4p_bonus_AuraScript();
    }
};

// 30451 - Arcane Blast (Glyph of Slow)
class spell_mage_glyph_of_slow : public SpellScriptLoader
{
public:
    spell_mage_glyph_of_slow() : SpellScriptLoader("spell_mage_glyph_of_slow") { }

    class spell_mage_glyph_of_slow_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_glyph_of_slow_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_MAGE_SLOW);
        }

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!target || !caster->HasAura(SPELL_MAGE_GLYPH_OF_SLOW))
                return;

            if (caster->GetBoundAurasBySpellId(SPELL_MAGE_SLOW))
                return;

            caster->CastSpell(target, SPELL_MAGE_SLOW, true);
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_mage_glyph_of_slow_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_glyph_of_slow_SpellScript();
    }
};

// 2139 - Counterspell (Item - Mage PvP Set 2P Bonus)
class spell_mage_counterspell : public SpellScriptLoader
{
public:
    spell_mage_counterspell() : SpellScriptLoader("spell_mage_counterspell") { }

    class spell_mage_counterspell_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_counterspell_SpellScript);

        // Must match the conditions of Spell::EffectInterruptCast
        static bool CanInterrupt(Unit* target)
        {
            for (uint32 i = CURRENT_FIRST_NON_MELEE_SPELL; i < CURRENT_AUTOREPEAT_SPELL; ++i)
            {
                Spell* spell = target->GetCurrentSpell(CurrentSpellTypes(i));
                if (!spell)
                    continue;

                SpellInfo const* spellInfo = spell->GetSpellInfo();
                if (spell->getState() != SPELL_STATE_CASTING && (spell->getState() != SPELL_STATE_PREPARING || spell->GetCastTime() <= 0))
                    continue;

                if (spellInfo->PreventionType != SPELL_PREVENTION_TYPE_SILENCE && spellInfo->PreventionType != SPELL_PREVENTION_TYPE_UNK)
                    continue;

                if ((i == CURRENT_GENERIC_SPELL && spellInfo->InterruptFlags & SPELL_INTERRUPT_FLAG_INTERRUPT)
                    || (i == CURRENT_CHANNELED_SPELL && spellInfo->ChannelInterruptFlags & CHANNEL_INTERRUPT_FLAG_INTERRUPT))
                    return true;
            }
            return false;
        }

        void HandleInterrupt(SpellEffIndex /*effIndex*/)
        {
            Player* mage = GetCaster()->ToPlayer();
            Unit* target = GetHitUnit();
            if (!mage || !target || !target->IsAlive())
                return;

            AuraEffect const* bonus = mage->GetAuraEffect(SPELL_MAGE_PVP_2P_BONUS, EFFECT_0);
            if (!bonus || !CanInterrupt(target))
                return;

            mage->ModifySpellCooldown(GetSpellInfo()->Id, bonus->GetAmount());
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_mage_counterspell_SpellScript::HandleInterrupt, EFFECT_0, SPELL_EFFECT_INTERRUPT_CAST);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_counterspell_SpellScript();
    }
};

struct MageAlterTimeAuraState
{
    uint32 SpellId;
    uint64 CasterGuid;
    uint32 EffectMask;
    uint32 RecalculateMask;
    int32 BaseAmount[MAX_SPELL_EFFECTS];
    int32 Amount[MAX_SPELL_EFFECTS];
    int32 Duration;
    int32 MaxDuration;
    uint8 StackAmount;
    uint8 Charges;
};

struct MageAlterTimeState
{
    std::vector<MageAlterTimeAuraState> Auras;
    uint32 Health = 0;
    uint32 Mana = 0;
    uint32 MapId = 0;
    float X = 0.0f;
    float Y = 0.0f;
    float Z = 0.0f;
    float Orientation = 0.0f;
};

// Auras Alter Time saves and restores: everything temporary the mage owns
static bool IsMageAlterTimeAura(Unit const* mage, Aura const* aura)
{
    if (aura->GetId() == SPELL_MAGE_ALTER_TIME || aura->IsRemoved())
        return false;

    if (aura->GetType() != UNIT_AURA_TYPE || aura->GetUnitOwner() != mage)
        return false;

    if (!(aura->GetSpellInfo()->Attributes & SPELL_ATTR0_DISABLED_WHILE_ACTIVE) && (aura->IsPassive() || aura->GetMaxDuration() == -1))
        return false;

    return true;
}

// 110909 - Alter Time
class spell_mage_alter_time : public SpellScriptLoader
{
public:
    spell_mage_alter_time() : SpellScriptLoader("spell_mage_alter_time") { }

    class spell_mage_alter_time_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_alter_time_AuraScript);

    public:
        bool Load() OVERRIDE
        {
            _hasState = false;
            return true;
        }

        void SetState(MageAlterTimeState const& state)
        {
            _state = state;
            _hasState = true;
        }

    private:
        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Player* mage = GetTarget()->ToPlayer();
            if (!mage || !_hasState || !mage->IsAlive() || mage->GetMapId() != _state.MapId)
                return;

            AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
            if (removeMode != AURA_REMOVE_BY_EXPIRE && removeMode != AURA_REMOVE_BY_DEFAULT)
                return;

            std::list<Creature*> pastSelves;
            mage->GetCreatureListWithEntryInGrid(pastSelves, NPC_MAGE_PAST_SELF, 100.0f);
            bool found = false;
            for (std::list<Creature*>::const_iterator itr = pastSelves.begin(); itr != pastSelves.end(); ++itr)
            {
                if (TempSummon* summon = (*itr)->ToTempSummon())
                {
                    if (summon->GetSummonerGUID() == mage->GetGUID())
                    {
                        found = true;
                        summon->DespawnOrUnsummon();
                    }
                }
            }

            if (!found)
                return;

            std::set<Aura*> restored;
            for (std::vector<MageAlterTimeAuraState>::const_iterator itr = _state.Auras.begin(); itr != _state.Auras.end(); ++itr)
            {
                SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(itr->SpellId);
                if (!spellInfo)
                    continue;

                Aura* aura = mage->GetAura(itr->SpellId, itr->CasterGuid);
                if (aura)
                {
                    aura->SetMaxDuration(itr->MaxDuration);
                    aura->SetDuration(itr->Duration);
                    if (aura->GetStackAmount() != itr->StackAmount)
                        aura->SetStackAmount(itr->StackAmount);
                    aura->SetCharges(itr->Charges);
                    for (uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                        if (AuraEffect* aurEff = aura->GetEffect(i))
                            if ((itr->EffectMask & (1 << i)) && aurEff->GetAmount() != itr->Amount[i])
                                aurEff->ChangeAmount(itr->Amount[i]);
                }
                else
                {
                    int32 baseAmount[MAX_SPELL_EFFECTS];
                    int32 amount[MAX_SPELL_EFFECTS];
                    memcpy(baseAmount, itr->BaseAmount, sizeof(baseAmount));
                    memcpy(amount, itr->Amount, sizeof(amount));

                    Unit* caster = ObjectAccessor::GetUnit(*mage, itr->CasterGuid);
                    aura = Aura::TryCreate(spellInfo, itr->EffectMask, mage, caster, &baseAmount[0], NULL, itr->CasterGuid);
                    if (!aura)
                        continue;

                    aura->SetLoadedState(itr->MaxDuration, itr->Duration, itr->Charges, itr->StackAmount, itr->RecalculateMask, &amount[0]);
                    aura->ApplyForTargets();
                }
                restored.insert(aura);
            }

            std::vector<std::pair<uint32, uint64> > toRemove;
            Unit::AuraApplicationMap& appliedAuras = mage->GetAppliedAuras();
            for (Unit::AuraApplicationMap::iterator itr = appliedAuras.begin(); itr != appliedAuras.end(); ++itr)
            {
                Aura* aura = itr->second->GetBase();
                if (IsMageAlterTimeAura(mage, aura) && restored.find(aura) == restored.end())
                    toRemove.push_back(std::make_pair(aura->GetId(), aura->GetCasterGUID()));
            }

            for (std::vector<std::pair<uint32, uint64> >::const_iterator itr = toRemove.begin(); itr != toRemove.end(); ++itr)
                mage->RemoveAura(itr->first, itr->second);

            mage->SetHealth(std::min(_state.Health, mage->GetMaxHealth()));
            mage->SetPower(POWER_MANA, int32(_state.Mana));
            mage->NearTeleportTo(_state.X, _state.Y, _state.Z, _state.Orientation);
        }

        void Register() OVERRIDE
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_mage_alter_time_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS, AURA_EFFECT_HANDLE_REAL);
        }

        MageAlterTimeState _state;
        bool _hasState;
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_mage_alter_time_AuraScript();
    }
};

// 108978 - Alter Time
class spell_mage_alter_time_cast : public SpellScriptLoader
{
public:
    spell_mage_alter_time_cast() : SpellScriptLoader("spell_mage_alter_time_cast") { }

    class spell_mage_alter_time_cast_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_alter_time_cast_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_MAGE_ALTER_TIME);
        }

        bool Load() OVERRIDE
        {
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        void SaveState()
        {
            Unit* mage = GetCaster();
            _state.Auras.clear();
            _state.Health = mage->GetHealth();
            _state.Mana = uint32(mage->GetPower(POWER_MANA));
            _state.MapId = mage->GetMapId();
            _state.X = mage->GetPositionX();
            _state.Y = mage->GetPositionY();
            _state.Z = mage->GetPositionZ();
            _state.Orientation = mage->GetOrientation();

            Unit::AuraApplicationMap& appliedAuras = mage->GetAppliedAuras();
            for (Unit::AuraApplicationMap::iterator itr = appliedAuras.begin(); itr != appliedAuras.end(); ++itr)
            {
                Aura* aura = itr->second->GetBase();
                if (!IsMageAlterTimeAura(mage, aura))
                    continue;

                MageAlterTimeAuraState data;
                data.SpellId = aura->GetId();
                data.CasterGuid = aura->GetCasterGUID();
                data.EffectMask = 0;
                data.RecalculateMask = 0;
                for (uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                {
                    data.BaseAmount[i] = 0;
                    data.Amount[i] = 0;
                    if (AuraEffect const* aurEff = aura->GetEffect(i))
                    {
                        data.BaseAmount[i] = aurEff->GetBaseAmount();
                        data.Amount[i] = aurEff->GetAmount();
                        data.EffectMask |= 1 << i;
                        if (aurEff->CanBeRecalculated())
                            data.RecalculateMask |= 1 << i;
                    }
                }
                data.Duration = aura->GetDuration();
                data.MaxDuration = aura->GetMaxDuration();
                data.StackAmount = aura->GetStackAmount();
                data.Charges = aura->GetCharges();
                _state.Auras.push_back(data);
            }
        }

        void HandleAfterHit()
        {
            Unit* mage = GetCaster();
            if (GetHitUnit() != mage)
                return;

            if (Aura* aura = mage->GetAura(SPELL_MAGE_ALTER_TIME))
                if (spell_mage_alter_time::spell_mage_alter_time_AuraScript* script = dynamic_cast<spell_mage_alter_time::spell_mage_alter_time_AuraScript*>(aura->GetScriptByName("spell_mage_alter_time")))
                    script->SetState(_state);
        }

        void Register() OVERRIDE
        {
            BeforeCast += SpellCastFn(spell_mage_alter_time_cast_SpellScript::SaveState);
            AfterHit += SpellHitFn(spell_mage_alter_time_cast_SpellScript::HandleAfterHit);
        }

    private:
        MageAlterTimeState _state;
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_alter_time_cast_SpellScript();
    }
};

// 127140 - Alter Time (overridden)
class spell_mage_alter_time_overridden : public SpellScriptLoader
{
public:
    spell_mage_alter_time_overridden() : SpellScriptLoader("spell_mage_alter_time_overridden") { }

    class spell_mage_alter_time_overridden_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_alter_time_overridden_SpellScript);

        void HandleAfterCast()
        {
            GetCaster()->RemoveAurasDueToSpell(SPELL_MAGE_ALTER_TIME);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_mage_alter_time_overridden_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_alter_time_overridden_SpellScript();
    }
};

// 140376 - Ring of Frost (Presence of Mind)
class spell_mage_ring_of_frost_overridden : public SpellScriptLoader
{
public:
    spell_mage_ring_of_frost_overridden() : SpellScriptLoader("spell_mage_ring_of_frost_overridden") { }

    class spell_mage_ring_of_frost_overridden_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_ring_of_frost_overridden_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_MAGE_RING_OF_FROST_PRESENCE_OF_MIND);
        }

        void HandleAfterCast()
        {
            WorldLocation const* dest = GetExplTargetDest();
            if (!dest)
                return;

            Unit* mage = GetCaster();
            uint32 delay = 2 * IN_MILLISECONDS;
            if (Player* modOwner = mage->GetSpellModOwner())
                modOwner->ApplySpellMod(GetSpellInfo()->Id, SPELLMOD_CASTING_TIME, delay);
            delay = uint32(delay * mage->GetFloatValue(UNIT_FIELD_MOD_CASTING_SPEED));

            float x = dest->GetPositionX();
            float y = dest->GetPositionY();
            float z = dest->GetPositionZ();
            mage->m_Events.Schedule(delay, [mage, x, y, z]()
            {
                mage->CastSpell(x, y, z, SPELL_MAGE_RING_OF_FROST_PRESENCE_OF_MIND, true);
            });
            mage->RemoveAurasDueToSpell(SPELL_MAGE_PRESENCE_OF_MIND);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_mage_ring_of_frost_overridden_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_ring_of_frost_overridden_SpellScript();
    }
};

// Damage of the extra Glyph of Icy Veins missiles relative to the original spell
static int32 GetIcyVeinsMissileDamagePct()
{
    if (SpellInfo const* icyVeins = sSpellMgr->GetSpellInfo(SPELL_MAGE_ICY_VEINS_GLYPHED))
        return 100 + icyVeins->Effects[EFFECT_0].BasePoints;
    return 40;
}

// 116 - Frostbolt, 30455 - Ice Lance, 44614 - Frostfire Bolt, 31707 - Waterbolt
class spell_mage_glyph_of_icy_veins : public SpellScriptLoader
{
public:
    spell_mage_glyph_of_icy_veins() : SpellScriptLoader("spell_mage_glyph_of_icy_veins") { }

    class spell_mage_glyph_of_icy_veins_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_glyph_of_icy_veins_SpellScript);

        static void CastMissile(Unit* caster, uint64 targetGuid, uint32 spellId, int32 damagePct)
        {
            Unit* target = ObjectAccessor::GetUnit(*caster, targetGuid);
            if (!target || !target->IsAlive())
                return;

            if (spellId == SPELL_MAGE_ICE_LANCE_ICY_VEINS)
                caster->CastCustomSpell(spellId, SPELLVALUE_BASE_POINT1, damagePct, target, true);
            else
                caster->CastSpell(target, spellId, true);
        }

        uint32 GetMissileSpellId() const
        {
            switch (m_scriptSpellId)
            {
                case SPELL_MAGE_FROSTBOLT:
                    return SPELL_MAGE_FROSTBOLT_ICY_VEINS;
                case SPELL_MAGE_ICE_LANCE:
                    return SPELL_MAGE_ICE_LANCE_ICY_VEINS;
                case SPELL_MAGE_FROSTFIRE_BOLT:
                    return SPELL_MAGE_FROSTFIRE_BOLT_ICY_VEINS;
                case SPELL_MAGE_WATERBOLT:
                    return SPELL_MAGE_WATERBOLT_ICY_VEINS;
                default:
                    return 0;
            }
        }

        void HandleCast()
        {
            Unit* caster = GetCaster();
            Unit* mage = caster->GetCharmerOrOwnerOrSelf();
            if (!mage || !mage->HasAura(SPELL_MAGE_ICY_VEINS_GLYPHED))
                return;

            Unit* target = GetExplTargetUnit();
            uint32 spellId = GetMissileSpellId();
            if (!target || !spellId || !caster->IsValidAttackTarget(target))
                return;

            // Ice Lance missiles carry their final damage percentage in the EFFECT_1 dummy base points
            int32 damagePct = 0;
            if (spellId == SPELL_MAGE_ICE_LANCE_ICY_VEINS)
            {
                bool fingers = mage->HasAura(SPELL_MAGE_FINGERS_OF_FROST);
                damagePct = GetIcyVeinsMissileDamagePct();
                if (fingers || target->HasAuraState(AURA_STATE_FROZEN, GetSpellInfo(), caster))
                    damagePct *= 4;
                if (fingers)
                    damagePct = damagePct * 5 / 4;
            }

            uint64 targetGuid = target->GetGUID();
            caster->m_Events.Schedule(400, [caster, targetGuid, spellId, damagePct]()
            {
                CastMissile(caster, targetGuid, spellId, damagePct);
            });
            caster->m_Events.Schedule(800, [caster, targetGuid, spellId, damagePct]()
            {
                CastMissile(caster, targetGuid, spellId, damagePct);
            });
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_mage_glyph_of_icy_veins_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_glyph_of_icy_veins_SpellScript();
    }
};

// 131079 - Frostbolt, 131080 - Ice Lance, 131081 - Frostfire Bolt, 131581 - Waterbolt (Glyph of Icy Veins)
class spell_mage_glyph_of_icy_veins_damage : public SpellScriptLoader
{
public:
    spell_mage_glyph_of_icy_veins_damage() : SpellScriptLoader("spell_mage_glyph_of_icy_veins_damage") { }

    class spell_mage_glyph_of_icy_veins_damage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_glyph_of_icy_veins_damage_SpellScript);

        bool Load() OVERRIDE
        {
            _frozenAtLaunch = false;
            return true;
        }

        void HandleLaunch(SpellEffIndex /*effIndex*/)
        {
            if (Unit* target = GetHitUnit())
                _frozenAtLaunch = target->HasAuraState(AURA_STATE_FROZEN, GetSpellInfo(), GetCaster());
        }

        void HandleHit()
        {
            int32 damagePct = GetIcyVeinsMissileDamagePct();
            if (m_scriptSpellId == SPELL_MAGE_ICE_LANCE_ICY_VEINS)
            {
                damagePct = GetSpellValue()->EffectBasePoints[EFFECT_1];
                // The core already doubles Ice Lance damage against frozen targets
                if (_frozenAtLaunch)
                    damagePct /= 2;
            }

            SetHitDamage(CalculatePct(GetHitDamage(), damagePct));
        }

        void Register() OVERRIDE
        {
            OnEffectLaunchTarget += SpellEffectFn(spell_mage_glyph_of_icy_veins_damage_SpellScript::HandleLaunch, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            OnHit += SpellHitFn(spell_mage_glyph_of_icy_veins_damage_SpellScript::HandleHit);
        }

    private:
        bool _frozenAtLaunch;
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_glyph_of_icy_veins_damage_SpellScript();
    }
};

// 30449 - Spellsteal (Glyph of Spellsteal)
class spell_mage_glyph_of_spellsteal : public SpellScriptLoader
{
public:
    spell_mage_glyph_of_spellsteal() : SpellScriptLoader("spell_mage_glyph_of_spellsteal") { }

    class spell_mage_glyph_of_spellsteal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mage_glyph_of_spellsteal_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            return sSpellMgr->GetSpellInfo(SPELL_MAGE_GLYPH_OF_SPELLSTEAL_HEAL);
        }

        bool Load() OVERRIDE
        {
            _stealableCharges = 0;
            return true;
        }

        uint32 CountStealableCharges(Unit* target)
        {
            DispelChargesList stealable;
            target->GetDispellableAuraList(GetCaster(), 1 << DISPEL_MAGIC, stealable);
            uint32 charges = 0;
            for (DispelChargesList::const_iterator itr = stealable.begin(); itr != stealable.end(); ++itr)
                charges += itr->second;
            return charges;
        }

        void HandleBeforeHit()
        {
            if (Unit* target = GetHitUnit())
                _stealableCharges = CountStealableCharges(target);
        }

        // Steal effects don't count towards GetDispelCount, so compare the target's stealable auras instead
        void HandleAfterHit()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!target || !caster->HasAura(SPELL_MAGE_GLYPH_OF_SPELLSTEAL))
                return;

            if (CountStealableCharges(target) < _stealableCharges)
                caster->CastSpell(caster, SPELL_MAGE_GLYPH_OF_SPELLSTEAL_HEAL, true);
        }

        void Register() OVERRIDE
        {
            BeforeHit += SpellHitFn(spell_mage_glyph_of_spellsteal_SpellScript::HandleBeforeHit);
            AfterHit += SpellHitFn(spell_mage_glyph_of_spellsteal_SpellScript::HandleAfterHit);
        }

    private:
        uint32 _stealableCharges;
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_mage_glyph_of_spellsteal_SpellScript();
    }
};

void AddSC_mage_spell_scripts()
{
    new spell_mage_frostjaw(); // 5.4.8 18414
    new spell_mage_pyroblast(); // 5.4.8 18414
    new spell_mage_time_warp(); // 5.4.8 18414
    new spell_mage_blast_wave();
    new spell_mage_cold_snap();
    new spell_mage_conjure_refreshment_table();
    new spell_mage_conjure_refreshment();
    new spell_mage_glyph_of_ice_block();
    new spell_mage_glyph_of_polymorph();
    new spell_mage_living_bomb();
    new spell_mage_living_bomb_explosion();
    new spell_mage_nether_vortex();
    new spell_mage_polymorph_cast_visual();
    new spell_mage_ring_of_frost();
    new spell_mage_ring_of_frost_freeze();
    new spell_mage_water_elemental_freeze();
    new spell_mage_mirror_image();
    new spell_mage_pyroblast_clearcasting();
    new spell_mage_combustion();
    new spell_mage_inferno_blast();
    new spell_mage_inferno_blast_spread();
    new spell_mage_flamestrike();
    new spell_mage_pyromaniac();
    new spell_mage_fingers_of_frost();
    new spell_mage_fingers_of_frost_proc();
    new spell_mage_icicles_periodic();
    new spell_mage_mastery_icicles();
    new spell_mage_ice_lance();
    new spell_mage_deep_freeze();
    new spell_mage_frostbolt();
    new spell_mage_icy_veins_cooldown();
    new spell_mage_frostfire_bolt();
    new spell_mage_blizzard();
    new spell_mage_frozen_orb_damage_trigger();
    new spell_mage_frozen_orb_damage();
    new spell_mage_frost_bomb_aura();
    new spell_mage_frost_bomb();
    new spell_mage_arcane_missiles_proc();
    new spell_mage_arcane_missiles_proc_visual();
    new spell_mage_arcane_missiles();
    new spell_mage_arcane_barrage();
    new spell_mage_arcane_charge();
    new spell_mage_arcane_explosion();
    new spell_mage_evocation();
    new spell_mage_incanters_ward();
    new spell_mage_incanters_ward_cooldown();
    new spell_mage_rune_of_power();
    new atrigger_script<sat_mage_rune_of_power>("sat_mage_rune_of_power");
    new spell_mage_nether_tempest_selector();
    new spell_mage_nether_tempest();
    new spell_mage_invisibility();
    new spell_mage_greater_invisibility();
    new spell_mage_greater_invisibility_damage_reduction();
    new spell_mage_temporal_shield();
    new spell_mage_cauterize();
    new spell_mage_flameglow();
    new spell_mage_glyph_of_illusion();
    new spell_mage_glyph_of_conjure_familiar();
    new spell_mage_glyph_of_unbound_elemental();
    new spell_mage_glyph_of_rapid_displacement();
    new spell_mage_glyph_of_momentum();
    new spell_mage_glyph_of_momentum_blink();
    new spell_mage_glyph_of_rapid_teleportation();
    new spell_mage_glyph_of_armors();
    new spell_mage_polymorph();
    new spell_mage_dragons_breath();
    new spell_mage_slow();
    new spell_mage_t15_2p_bonus();
    new spell_mage_t16_2p_bonus();
    new spell_mage_t16_4p_bonus();
    new spell_mage_glyph_of_slow();
    new spell_mage_counterspell();
    new spell_mage_alter_time();
    new spell_mage_alter_time_cast();
    new spell_mage_alter_time_overridden();
    new spell_mage_ring_of_frost_overridden();
    new spell_mage_glyph_of_icy_veins();
    new spell_mage_glyph_of_icy_veins_damage();
    new spell_mage_glyph_of_spellsteal();
}
