/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/*
 * Scripts for spells with SPELLFAMILY_MAGE and SPELLFAMILY_GENERIC spells used by mage players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_mage_".
 */

#include "Player.h"
#include "GridNotifiers.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Pet.h"

enum MageSpells
{
    SPELL_MAGE_FROSTJAW                          = 102051,
    SPELL_MAGE_COLD_SNAP                         = 11958,
    SPELL_MAGE_FROST_NOVA                        = 122,
    SPELL_MAGE_IGNITE                            = 12654,
    SPELL_MAGE_SLOW                              = 31589,
    SPELL_MAGE_SQUIRREL_FORM                     = 32813,
    SPELL_MAGE_GIRAFFE_FORM                      = 32816,
    SPELL_MAGE_SERPENT_FORM                      = 32817,
    SPELL_MAGE_DRAGONHAWK_FORM                   = 32818,
    SPELL_MAGE_WORGEN_FORM                       = 32819,
    SPELL_MAGE_SHEEP_FORM                        = 32820,

    SPELL_MAGE_FLAMESTRIKE                       = 2120,

    SPELL_MAGE_RING_OF_FROST_SUMMON              = 82676, // obsolete SpellID
    SPELL_MAGE_RING_OF_FROST_FREEZE              = 82691,
    SPELL_MAGE_RING_OF_FROST_DUMMY               = 91264,

    SPELL_MAGE_FINGERS_OF_FROST                  = 44544,

    // Time Warp - 80353
    SPELL_MAGE_TEMPORAL_DISPLACEMENT             = 80354,
    SPELL_SHAMAN_SATED                           = 57724,
    SPELL_HUNTER_INSANITY                        = 95809,

    // Pyroblast!
    SPELL_MAGE_PYROBLAST_CLEARCAST               = 48108,
};

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
        void RecalculateDamage(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();

            if (caster->HasAura(SPELL_MAGE_PYROBLAST_CLEARCAST))
            {
                int basedmg = GetHitDamage();
                int bonusdmg = CalculatePct(basedmg, GetSpellInfo()->Effects[EFFECT_1].CalcValue(GetCaster()));
                SetHitDamage(basedmg + bonusdmg);
                caster->RemoveAurasDueToSpell(SPELL_MAGE_PYROBLAST_CLEARCAST);
            }
        }
        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_mage_pyroblast_SpellScript::RecalculateDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
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

        void AfterRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
            if (removeMode != AURA_REMOVE_BY_ENEMY_SPELL && removeMode != AURA_REMOVE_BY_EXPIRE)
                return;

            if (Unit* caster = GetCaster())
                caster->CastSpell(GetTarget(), uint32(aurEff->GetAmount()), true, NULL, aurEff);
        }

        void Register() override
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_mage_living_bomb_AuraScript::AfterRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_mage_living_bomb_AuraScript();
    }
};

// -11119 - Ignite
class spell_mage_ignite : public SpellScriptLoader
{
public:
    spell_mage_ignite() : SpellScriptLoader("spell_mage_ignite") { }

    class spell_mage_ignite_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mage_ignite_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_IGNITE))
                return false;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            return eventInfo.GetProcTarget();
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            SpellInfo const* igniteDot = sSpellMgr->GetSpellInfo(SPELL_MAGE_IGNITE);
            int32 pct = 8 * GetSpellInfo()->GetRank();

            int32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), pct) / igniteDot->GetMaxTicks());
            amount += eventInfo.GetProcTarget()->GetRemainingPeriodicAmount(eventInfo.GetActor()->GetGUID(), SPELL_MAGE_IGNITE, SPELL_AURA_PERIODIC_DAMAGE);
            GetTarget()->CastCustomSpell(SPELL_MAGE_IGNITE, SPELLVALUE_BASE_POINT0, amount, eventInfo.GetProcTarget(), true, NULL, aurEff);
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_mage_ignite_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_mage_ignite_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_mage_ignite_AuraScript();
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
                    target->CastSpell(target, PolymorhForms[urand(0, 5)], true);
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

// 82676 - Ring of Frost
/// Updated 4.3.4
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
            GetTarget()->GetAllMinionsByEntry(MinionList, GetSpellInfo()->Effects[EFFECT_0].MiscValue);

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
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_mage_ring_of_frost_AuraScript::HandleEffectPeriodic, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            OnEffectApply += AuraEffectApplyFn(spell_mage_ring_of_frost_AuraScript::Apply, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
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
            float outRadius = sSpellMgr->GetSpellInfo(SPELL_MAGE_RING_OF_FROST_SUMMON)->Effects[EFFECT_0].CalcRadius();
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

// 33395 Water Elemental's Freeze
/// Updated 4.3.4
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

        void CountTargets(std::list<WorldObject*>& targetList)
        {
            _didHit = !targetList.empty();
        }

        void HandleImprovedFreeze()
        {
            if (!_didHit)
                return;

            Unit* owner = GetCaster()->GetOwner();
            if (!owner)
                return;

            if (AuraEffect* aurEff = owner->GetAuraEffect(SPELL_AURA_DUMMY, SPELLFAMILY_MAGE, ICON_MAGE_IMPROVED_FREEZE, EFFECT_0))
            {
                if (roll_chance_i(aurEff->GetAmount()))
                    owner->CastCustomSpell(SPELL_MAGE_FINGERS_OF_FROST, SPELLVALUE_AURA_STACK, 2, owner, true);
            }
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_mage_water_elemental_freeze_SpellScript::CountTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            AfterCast += SpellCastFn(spell_mage_water_elemental_freeze_SpellScript::HandleImprovedFreeze);
        }

    private:
        bool _didHit;
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_mage_water_elemental_freeze_SpellScript();
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
    new spell_mage_ignite();
    new spell_mage_glyph_of_ice_block();
    new spell_mage_glyph_of_polymorph();
    new spell_mage_living_bomb();
    new spell_mage_nether_vortex();
    new spell_mage_polymorph_cast_visual();
    new spell_mage_ring_of_frost();
    new spell_mage_ring_of_frost_freeze();
    new spell_mage_water_elemental_freeze();
}
