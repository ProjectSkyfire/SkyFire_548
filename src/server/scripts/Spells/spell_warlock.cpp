/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/*
 * Scripts for spells with SPELLFAMILY_WARLOCK and SPELLFAMILY_GENERIC spells used by warlock players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_warl_".
 */

#include "DBCStores.h"
#include "GridNotifiers.h"
#include "PathGenerator.h"
#include "Pet.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"

enum WarlockSpells
{
    SPELL_WARLOCK_AFTERMATH_STUN                    = 85387,
    SPELL_WARLOCK_CREATE_HEALTHSTONE                = 34130,

    SPELL_WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST         = 62388, 
    SPELL_WARLOCK_DEMONIC_CIRCLE_SUMMON             = 48018,
    SPELL_WARLOCK_DEMONIC_CIRCLE_TELEPORT           = 48020,
    SPELL_WARLOCK_DEMONIC_GATEWAY_AURA              = 113901,
    SPELL_WARLOCK_DEMONIC_GATEWAY_SUMMON_PURPLE     = 113890,
    SPELL_WARLOCK_DRAIN_LIFE_HEAL                   = 89653,

    SPELL_WARLOCK_FEL_SYNERGY_HEAL                  = 54181,

    SPELL_WARLOCK_LIFE_TAP_ENERGIZE                 = 31818,

    SPELL_WARLOCK_RAIN_OF_FIRE                      = 42223,

    SPELL_WARLOCK_SHADOW_TRANCE                     = 17941,
    SPELL_WARLOCK_SIPHON_LIFE_HEAL                  = 63106,
    
    SPELL_WARLOCK_SOULSHATTER                       = 32835,

    SPELL_WARLOCK_SOUL_SWAP_CD_MARKER               = 94229,
    SPELL_WARLOCK_SOUL_SWAP_OVERRIDE                = 86211,
    SPELL_WARLOCK_SOUL_SWAP_MOD_COST                = 92794,
    SPELL_WARLOCK_SOUL_SWAP_DOT_MARKER              = 92795,

    SPELL_WARLOCK_UNSTABLE_AFFLICTION_DISPEL        = 31117,

    // Destruction
    SPELL_WARLOCK_BACKDRAFT                         = 117828,
    SPELL_WARLOCK_BACKLASH                          = 34936,
    SPELL_WARLOCK_BACKLASH_CODEX                    = 140076,
    SPELL_WARLOCK_CHAOS_BOLT                        = 116858,
    SPELL_WARLOCK_CONFLAGRATE                       = 17962,
    SPELL_WARLOCK_CONFLAGRATE_FNB                   = 108685,
    SPELL_WARLOCK_FEL_FLAME                         = 77799,
    SPELL_WARLOCK_FIRE_AND_BRIMSTONE                = 108683,
    SPELL_WARLOCK_GLYPH_OF_CONFLAGRATE              = 56235,
    SPELL_WARLOCK_GLYPH_OF_EMBER_TAP                = 63304,
    SPELL_WARLOCK_HAVOC                             = 80240,
    SPELL_WARLOCK_IMMOLATE                          = 348,
    SPELL_WARLOCK_IMMOLATE_FNB                      = 108686,
    SPELL_WARLOCK_INCINERATE                        = 29722,
    SPELL_WARLOCK_INCINERATE_FNB                    = 114654,
    SPELL_WARLOCK_MASTERY_EMBERSTORM                = 77220,
    SPELL_WARLOCK_PYROCLASM                         = 123686,
    SPELL_WARLOCK_RAIN_OF_FIRE_DAMAGE               = 104233,
    SPELL_WARLOCK_SHADOWBURN_ENERGIZE               = 125882,
    SPELL_WARLOCK_T15_4P_BONUS                      = 138134,
    SPELL_WARLOCK_THE_CODEX_OF_XERRATH              = 101508,

    // Demonology
    SPELL_WARLOCK_DARK_APOTHEOSIS                   = 114168,
    SPELL_WARLOCK_DECIMATION                        = 108869,
    SPELL_WARLOCK_DEMONIC_CALLING                   = 114925,
    SPELL_WARLOCK_DEMONIC_CIRCLE_SUMMON_DEMO        = 104135,
    SPELL_WARLOCK_DEMONIC_CIRCLE_TELEPORT_ANIM      = 104163,
    SPELL_WARLOCK_DEMONIC_FURY                      = 109145,
    SPELL_WARLOCK_DEMONIC_FURY_ENERGIZE             = 104314,
    SPELL_WARLOCK_DEMONIC_LEAP_JUMP                 = 109163,
    SPELL_WARLOCK_DEMONIC_SLASH                     = 114175,
    SPELL_WARLOCK_DISRUPTED_NETHER                  = 114736,
    SPELL_WARLOCK_GLYPH_OF_DEMON_HUNTING            = 63303,
    SPELL_WARLOCK_GLYPH_OF_FALLING_METEOR           = 56247,
    SPELL_WARLOCK_GLYPH_OF_IMP_SWARM                = 56242,
    SPELL_WARLOCK_GLYPH_OF_SHADOW_BOLT              = 56240,
    SPELL_WARLOCK_GRIMOIRE_OF_SACRIFICE             = 108503,
    SPELL_WARLOCK_HAND_OF_GULDAN_DAMAGE             = 86040,
    SPELL_WARLOCK_HELLFIRE                          = 1949,
    SPELL_WARLOCK_IMP_SWARM                         = 104316,
    SPELL_WARLOCK_MASTER_DEMONOLOGIST               = 115556,
    SPELL_WARLOCK_MASTERY_MASTER_DEMONOLOGIST       = 77219,
    SPELL_WARLOCK_METAMORPHOSIS                     = 103958,
    SPELL_WARLOCK_METAMORPHOSIS_CHAOS_WAVE          = 124917,
    SPELL_WARLOCK_METAMORPHOSIS_CURSED_AURAS        = 116208,
    SPELL_WARLOCK_METAMORPHOSIS_TOUCH_OF_CHAOS      = 112089,
    SPELL_WARLOCK_METEOR_SLAM                       = 124315,
    SPELL_WARLOCK_MOLTEN_CORE                       = 122355,
    SPELL_WARLOCK_MOLTEN_CORE_CODEX                 = 140074,
    SPELL_WARLOCK_MOLTEN_CORE_PASSIVE               = 122351,
    SPELL_WARLOCK_MOLTEN_CORE_VISUAL                = 126090,
    SPELL_WARLOCK_MOLTEN_CORE_VISUAL_CODEX          = 140075,
    SPELL_WARLOCK_OVERRIDE_COMMAND_DEMON            = 119904,
    SPELL_WARLOCK_PET_FEL_ENERGY                    = 123746,
    SPELL_WARLOCK_SHADOW_BOLT                       = 686,
    SPELL_WARLOCK_SUMMON_ABYSSAL                    = 112921,
    SPELL_WARLOCK_SUMMON_FEL_IMP                    = 112866,
    SPELL_WARLOCK_SUMMON_OBSERVER                   = 112869,
    SPELL_WARLOCK_SUMMON_SHIVARRA                   = 112868,
    SPELL_WARLOCK_SUMMON_TERRORGUARD                = 112927,
    SPELL_WARLOCK_SUMMON_VOIDLORD                   = 112867,
    SPELL_WARLOCK_SUMMON_WRATHGUARD                 = 112870,
    SPELL_WARLOCK_WILD_IMPS                         = 114592,
    SPELL_WARLOCK_WILD_IMP_SUMMON                   = 104317,

    // Affliction
    SPELL_WARLOCK_AGONY                             = 980,
    SPELL_WARLOCK_AGONY_INSTANT                     = 131737,
    SPELL_WARLOCK_CORRUPTION                        = 172,
    SPELL_WARLOCK_CORRUPTION_DOT                    = 146739,
    SPELL_WARLOCK_CORRUPTION_INSTANT                = 131740,
    SPELL_WARLOCK_DARK_REFUND                       = 145159,
    SPELL_WARLOCK_MASTERY_POTENT_AFFLICTIONS        = 77215,
    SPELL_WARLOCK_SEED_OF_CORRUPTION                = 27243,
    SPELL_WARLOCK_SEED_OF_CORRUPTION_INSTANT        = 132566,
    SPELL_WARLOCK_SEED_OF_CORRUPTION_SOULBURN_DAMAGE = 87385,
    SPELL_WARLOCK_SOULSHARD                         = 87388,
    SPELL_WARLOCK_SOULSHARD_ENERGIZE                = 95810,
    SPELL_WARLOCK_UNSTABLE_AFFLICTION               = 30108,
    SPELL_WARLOCK_UNSTABLE_AFFLICTION_INSTANT       = 131736,

    // Soulburn
    SPELL_WARLOCK_HEALTH_FUNNEL_SOULBURN            = 104220,
    SPELL_WARLOCK_SOULBURN                          = 74434,
    SPELL_WARLOCK_SOULBURN_CURSE                    = 103112,
    SPELL_WARLOCK_SOULBURN_DEMONIC_CIRCLE_TELEPORT  = 104243,
    SPELL_WARLOCK_SOULBURN_HEALTH_FUNNEL            = 103101,
    SPELL_WARLOCK_SOULBURN_OVERRIDE_BASE            = 93312,
    SPELL_WARLOCK_SOULBURN_OVERRIDE_CURSE1          = 104245,
    SPELL_WARLOCK_SOULBURN_OVERRIDE_CURSE2          = 104250,
    SPELL_WARLOCK_SOULBURN_OVERRIDE_SEED            = 93313,
    SPELL_WARLOCK_SOULBURN_OVERRIDE_SOUL_SWAP       = 104251,
    SPELL_WARLOCK_SOULBURN_OVERRIDE_TELEPORT        = 104249,
    SPELL_WARLOCK_SOULBURN_SEED_OF_CORRUPTION       = 86664,
    SPELL_WARLOCK_SOULBURN_SOUL_SWAP                = 141931,
    SPELL_WARLOCK_UNENDING_BREATH_WATERWALK         = 104242,

    // Talents, glyphs, set bonuses and misc
    SPELL_WARLOCK_BLOOD_HORROR_FEAR                 = 137143,
    SPELL_WARLOCK_COMMAND_CAUTERIZE_MASTER          = 119905,
    SPELL_WARLOCK_COMMAND_DISARM                    = 119907,
    SPELL_WARLOCK_COMMAND_FELLASH                   = 119913,
    SPELL_WARLOCK_COMMAND_FELSTORM                  = 119914,
    SPELL_WARLOCK_COMMAND_OPTICAL_BLAST             = 119911,
    SPELL_WARLOCK_COMMAND_SPELL_LOCK                = 119910,
    SPELL_WARLOCK_COMMAND_WHIPLASH                  = 119909,
    SPELL_WARLOCK_COMMAND_WRATHSTORM                = 119915,
    SPELL_WARLOCK_CREATE_HEALTHSTONE_CHARGES        = 120038,
    SPELL_WARLOCK_CREATE_HEALTHSTONE_ITEM           = 23517,
    SPELL_WARLOCK_CURSE_OF_EXHAUSTION               = 18223,
    SPELL_WARLOCK_DARK_BARGAIN_DOT                  = 110914,
    SPELL_WARLOCK_DARK_REGENERATION                 = 108359,
    SPELL_WARLOCK_DESTRUCTIVE_INFLUENCE             = 145075,
    SPELL_WARLOCK_DEVOUR_MAGIC_HEAL                 = 19658,
    SPELL_WARLOCK_EMPOWERED_GRASP                   = 145082,
    SPELL_WARLOCK_FEAR_EFFECT                       = 118699,
    SPELL_WARLOCK_FEL_ARMOR                         = 104938,
    SPELL_WARLOCK_FIERY_WRATH                       = 145085,
    SPELL_WARLOCK_FURY_WARD                         = 119839,
    SPELL_WARLOCK_GLYPH_OF_DEMON_TRAINING           = 56249,
    SPELL_WARLOCK_GLYPH_OF_DRAIN_LIFE               = 63302,
    SPELL_WARLOCK_GLYPH_OF_FEAR                     = 56244,
    SPELL_WARLOCK_GLYPH_OF_FEAR_EFFECT              = 130616,
    SPELL_WARLOCK_GLYPH_OF_HEALTHSTONE              = 56224,
    SPELL_WARLOCK_GLYPH_OF_SOUL_CONSUMPTION         = 58068,
    SPELL_WARLOCK_GLYPH_OF_SOULWELL                 = 58094,
    SPELL_WARLOCK_GLYPH_OF_SOULWELL_VISUAL          = 34145,
    SPELL_WARLOCK_HAND_OF_GULDAN_T16                = 145158,
    SPELL_WARLOCK_HARVEST_LIFE                      = 108371,
    SPELL_WARLOCK_PET_CAUTERIZE_MASTER              = 119899,
    SPELL_WARLOCK_PET_DISARM                        = 118093,
    SPELL_WARLOCK_PET_FELLASH                       = 115770,
    SPELL_WARLOCK_PET_FELSTORM                      = 89751,
    SPELL_WARLOCK_PET_LEGION_STRIKE                 = 30213,
    SPELL_WARLOCK_PET_MORTAL_CLEAVE                 = 115625,
    SPELL_WARLOCK_PET_OPTICAL_BLAST                 = 115781,
    SPELL_WARLOCK_PET_SPELL_LOCK                    = 19647,
    SPELL_WARLOCK_PET_WHIPLASH                      = 6360,
    SPELL_WARLOCK_PET_WRATHSTORM                    = 115831,
    SPELL_WARLOCK_PVP_SET_2P_BONUS                  = 131632,
    SPELL_WARLOCK_SHADOW_WORD_DEATH_BACKLASH        = 32409,
    SPELL_WARLOCK_SOUL_FIRE                         = 6353,
    SPELL_WARLOCK_SOUL_FIRE_METAMORPHOSIS           = 104027,
    SPELL_WARLOCK_SOUL_LEECH_ABSORB                 = 108366,
    SPELL_WARLOCK_SOUL_LINK                         = 108415,
    SPELL_WARLOCK_SOUL_LINK_HEAL                    = 108447,
    SPELL_WARLOCK_SOULSTONE_RESURRECT               = 95750,
    SPELL_WARLOCK_T16_4P_BONUS                      = 145091,
    SPELL_WARLOCK_TWILIGHT_WARD_METAMORPHOSIS_S12   = 131624,
    SPELL_WARLOCK_TWILIGHT_WARD_S12                 = 131623,
    SPELL_WARLOCK_DARK_SOUL_INSTABILITY             = 113858,
    SPELL_WARLOCK_DARK_SOUL_KNOWLEDGE               = 113861,
    SPELL_WARLOCK_DARK_SOUL_MISERY                  = 113860
};

enum WarlockNpcs
{
    NPC_WARLOCK_IMP                                 = 416,
    NPC_WARLOCK_FELHUNTER                           = 417,
    NPC_WARLOCK_VOIDWALKER                          = 1860,
    NPC_WARLOCK_SUCCUBUS                            = 1863,
    NPC_WARLOCK_FELGUARD                            = 17252,
    NPC_WARLOCK_FEL_IMP                             = 58959,
    NPC_WARLOCK_VOIDLORD                            = 58960,
    NPC_WARLOCK_SHIVARRA                            = 58963,
    NPC_WARLOCK_OBSERVER                            = 58964,
    NPC_WARLOCK_WRATHGUARD                          = 58965,
    NPC_WARLOCK_GATEWAY_GREEN                       = 59262,
    NPC_WARLOCK_GATEWAY_PURPLE                      = 59271
};

enum WarlockSpellIcons
{
    WARLOCK_ICON_ID_IMPROVED_LIFE_TAP               = 208,
    WARLOCK_ICON_ID_MANA_FEED                       = 1982
};

enum MiscSpells
{
    SPELL_GEN_REPLENISHMENT                         = 57669,
    SPELL_PRIEST_SHADOW_WORD_DEATH                  = 32409
};

// 111546 - Chaotic Energy
class spell_warl_chaotic_energy : public SpellScriptLoader
{
public:
    spell_warl_chaotic_energy() : SpellScriptLoader("spell_warl_chaotic_energy") { }

    class spell_warl_chaotic_energy_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_chaotic_energy_AuraScript);

        void HandleEffectCalcSpellMod(AuraEffect const* aurEff, SpellModifier*& spellMod)
        {
            if (!spellMod)
            {
                spellMod = new SpellModifier(GetAura());
                spellMod->op = SPELLMOD_COST;
                spellMod->type = SPELLMOD_PCT;
                spellMod->spellId = GetId();
                spellMod->mask = GetSpellInfo()->Effects[aurEff->GetEffIndex()].SpellClassMask;
            }

            spellMod->value = (float)(aurEff->GetAmount() / 100);
        }

        void Register() OVERRIDE
        {
            DoEffectCalcSpellMod += AuraEffectCalcSpellModFn(spell_warl_chaotic_energy_AuraScript::HandleEffectCalcSpellMod, EFFECT_1, SPELL_AURA_ADD_PCT_MODIFIER);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_chaotic_energy_AuraScript();
    }
};

// 710 - Banish
class spell_warl_banish : public SpellScriptLoader
{
public:
    spell_warl_banish() : SpellScriptLoader("spell_warl_banish") { }

    class spell_warl_banish_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_banish_SpellScript);

        bool Load() OVERRIDE
        {
            _removed = false;
            return true;
        }

        void HandleBanish()
        {
            /// Casting Banish on a banished target will cancel the effect
            /// Check if the target already has Banish, if so, do nothing.
            if (Unit* target = GetHitUnit())
            {
                if (target->GetAuraEffect(SPELL_AURA_SCHOOL_IMMUNITY, SPELLFAMILY_WARLOCK, 0, 0x08000000, 0))
                {
                    // No need to remove old aura since its removed due to not stack by current Banish aura
                    PreventHitDefaultEffect(EFFECT_0);
                    PreventHitDefaultEffect(EFFECT_1);
                    PreventHitDefaultEffect(EFFECT_2);
                    _removed = true;
                }
            }
        }

        void RemoveAura()
        {
            if (_removed)
                PreventHitAura();
        }

        void Register() OVERRIDE
        {
            BeforeHit += SpellHitFn(spell_warl_banish_SpellScript::HandleBanish);
            AfterHit += SpellHitFn(spell_warl_banish_SpellScript::RemoveAura);
        }

        bool _removed;
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_banish_SpellScript();
    }
};

// 48018, 104135 - Demonic Circle: Summon
class spell_warl_demonic_circle_summon : public SpellScriptLoader
{
public:
    spell_warl_demonic_circle_summon() : SpellScriptLoader("spell_warl_demonic_circle_summon") { }

    class spell_warl_demonic_circle_summon_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_demonic_circle_summon_AuraScript);

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes mode)
        {
            // If effect is removed by expire remove the summoned demonic circle too.
            if (!(mode & AURA_EFFECT_HANDLE_REAPPLY))
                GetTarget()->RemoveGameObject(GetId(), true);

            GetTarget()->RemoveAura(SPELL_WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST);
        }

        void HandleDummyTick(AuraEffect const* /*aurEff*/)
        {
            if (GameObject* circle = GetTarget()->GetGameObject(GetId()))
            {
                // Here we check if player is in demonic circle teleport range, if so add
                // WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST; allowing him to cast the WARLOCK_DEMONIC_CIRCLE_TELEPORT.
                // If not in range remove the WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST.

                SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(SPELL_WARLOCK_DEMONIC_CIRCLE_TELEPORT);

                if (GetTarget()->IsWithinDist(circle, spellInfo->GetMaxRange(true)))
                {
                    if (!GetTarget()->HasAura(SPELL_WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST))
                        GetTarget()->CastSpell(GetTarget(), SPELL_WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST, true);
                }
                else
                    GetTarget()->RemoveAura(SPELL_WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST);
            }
        }

        void Register() OVERRIDE
        {
            OnEffectRemove += AuraEffectApplyFn(spell_warl_demonic_circle_summon_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_demonic_circle_summon_AuraScript::HandleDummyTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_demonic_circle_summon_AuraScript();
    }
};

// 48020, 104136 - Demonic Circle: Teleport, 114794 - Soulburn: Demonic Circle: Teleport
class spell_warl_demonic_circle_teleport : public SpellScriptLoader
{
public:
    spell_warl_demonic_circle_teleport() : SpellScriptLoader("spell_warl_demonic_circle_teleport") { }

    class spell_warl_demonic_circle_teleport_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_demonic_circle_teleport_AuraScript);

        void HandleTeleport(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* player = GetTarget()->ToPlayer())
            {
                player->CastSpell(player, SPELL_WARLOCK_DEMONIC_CIRCLE_TELEPORT_ANIM, true);

                GameObject* circle = player->GetGameObject(SPELL_WARLOCK_DEMONIC_CIRCLE_SUMMON);
                if (!circle)
                    circle = player->GetGameObject(SPELL_WARLOCK_DEMONIC_CIRCLE_SUMMON_DEMO);

                if (circle)
                {
                    player->NearTeleportTo(circle->GetPositionX(), circle->GetPositionY(), circle->GetPositionZ(), circle->GetOrientation());
                    player->RemoveMovementImpairingAuras();
                }
            }
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_warl_demonic_circle_teleport_AuraScript::HandleTeleport, EFFECT_0, SPELL_AURA_MECHANIC_IMMUNITY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_demonic_circle_teleport_AuraScript();
    }
};

// 689 - Drain Life, 89420 - Drain Life (Soulburn), 103990 - Drain Life (Metamorphosis)
class spell_warl_drain_life : public SpellScriptLoader
{
public:
    spell_warl_drain_life() : SpellScriptLoader("spell_warl_drain_life") { }

    class spell_warl_drain_life_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_drain_life_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_DRAIN_LIFE_HEAL))
                return false;
            return true;
        }

        void HandlePeriodic(AuraEffect const* aurEff)
        {
            Unit* caster = GetCaster();
            if (!caster || !caster->IsAlive())
                return;

            // Effect 1 is a dummy on the caster holding the percentage of maximum health returned
            // per tick. Read it from the spell so glyphs and talents that change it are picked up.
            int32 pct = GetSpellInfo()->Effects[EFFECT_1].CalcValue(caster);
            if (pct <= 0)
                return;

            int32 heal = caster->CountPctFromMaxHealth(pct);
            if (!heal)
                return;

            if (Aura const* harvestLife = caster->GetAura(SPELL_WARLOCK_HARVEST_LIFE))
                AddPct(heal, harvestLife->GetSpellInfo()->Effects[EFFECT_1].BasePoints);
            if (AuraEffect const* glyph = caster->GetAuraEffect(SPELL_WARLOCK_GLYPH_OF_DRAIN_LIFE, EFFECT_0))
                AddPct(heal, glyph->GetAmount());

            if (Player* warlock = caster->ToPlayer())
                if (warlock->GetTalentSpecialization(warlock->GetActiveSpec()) == SPEC_WARLOCK_DEMONOLOGY && warlock->GetShapeshiftForm() != FORM_METAMORPHOSIS)
                    warlock->CastCustomSpell(SPELL_WARLOCK_DEMONIC_FURY_ENERGIZE, SPELLVALUE_BASE_POINT1, GetSpellInfo()->Effects[EFFECT_2].CalcValue(warlock), warlock, true);

            caster->CastCustomSpell(SPELL_WARLOCK_DRAIN_LIFE_HEAL, SPELLVALUE_BASE_POINT0, heal, caster, true, nullptr, aurEff);
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
                caster->RemoveAurasDueToSpell(SPELL_WARLOCK_SOULBURN);
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_drain_life_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            AfterEffectApply += AuraEffectApplyFn(spell_warl_drain_life_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_drain_life_AuraScript();
    }
};

// -47230 - Fel Synergy
class spell_warl_fel_synergy : public SpellScriptLoader
{
public:
    spell_warl_fel_synergy() : SpellScriptLoader("spell_warl_fel_synergy") { }

    class spell_warl_fel_synergy_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_fel_synergy_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_FEL_SYNERGY_HEAL))
                return false;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            return GetTarget()->GetGuardianPet() && eventInfo.GetDamageInfo()->GetDamage();
        }

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            int32 heal = CalculatePct(int32(eventInfo.GetDamageInfo()->GetDamage()), aurEff->GetAmount());
            GetTarget()->CastCustomSpell(SPELL_WARLOCK_FEL_SYNERGY_HEAL, SPELLVALUE_BASE_POINT0, heal, (Unit*)NULL, true, NULL, aurEff); // TARGET_UNIT_PET
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_warl_fel_synergy_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_warl_fel_synergy_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_fel_synergy_AuraScript();
    }
};

// 6262 - Healthstone
class spell_warl_healthstone : public SpellScriptLoader
{
public:
    spell_warl_healthstone() : SpellScriptLoader("spell_warl_healthstone") { }

    class spell_warl_healthstone_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_healthstone_SpellScript);

        // The glyph turns the instant heal into a heal over time
        void HandleHeal(SpellEffIndex effIndex)
        {
            Unit* caster = GetCaster();
            if (caster->HasAura(SPELL_WARLOCK_GLYPH_OF_HEALTHSTONE))
                PreventHitDefaultEffect(effIndex);
            else
                SetEffectValue(int32(caster->CountPctFromMaxHealth(20)));
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_healthstone_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_healthstone_SpellScript();
    }
};

// 6262 - Healthstone (glyphed)
class spell_warl_healthstone_glyphed : public SpellScriptLoader
{
public:
    spell_warl_healthstone_glyphed() : SpellScriptLoader("spell_warl_healthstone_glyphed") { }

    class spell_warl_healthstone_glyphed_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_healthstone_glyphed_AuraScript);

        void CalculateHeal(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
        {
            Unit* caster = GetCaster();
            if (!caster || !aurEff->GetBase()->GetDuration())
                return;

            int32 total = int32(caster->CountPctFromMaxHealth(amount));
            amount = total / std::max<int32>(aurEff->GetTotalTicks(), 1);
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_healthstone_glyphed_AuraScript::CalculateHeal, EFFECT_1, SPELL_AURA_PERIODIC_HEAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_healthstone_glyphed_AuraScript();
    }
};

// 1454 - Life Tap
/// Updated 4.3.4
class spell_warl_life_tap : public SpellScriptLoader
{
public:
    spell_warl_life_tap() : SpellScriptLoader("spell_warl_life_tap") { }

    class spell_warl_life_tap_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_life_tap_SpellScript);

        bool Load() OVERRIDE
        {
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_LIFE_TAP_ENERGIZE))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            PreventHitDefaultEffect(EFFECT_0);

            Player* caster = GetCaster()->ToPlayer();
            if (Unit* target = GetHitUnit())
            {
                int32 damage = caster->CountPctFromMaxHealth(GetSpellInfo()->Effects[EFFECT_2].CalcValue());
                int32 mana = CalculatePct(damage, GetSpellInfo()->Effects[EFFECT_1].CalcValue());

                // Shouldn't Appear in Combat Log
                target->ModifyHealth(-damage);

                // Improved Life Tap mod
                if (AuraEffect const* aurEff = caster->GetDummyAuraEffect(SPELLFAMILY_WARLOCK, WARLOCK_ICON_ID_IMPROVED_LIFE_TAP, 0))
                    AddPct(mana, aurEff->GetAmount());

                caster->CastCustomSpell(target, SPELL_WARLOCK_LIFE_TAP_ENERGIZE, &mana, NULL, NULL, false);
            }
        }

        SpellCastResult CheckCast()
        {
            if (int32(GetCaster()->GetHealth()) > int32(GetCaster()->CountPctFromMaxHealth(GetSpellInfo()->Effects[EFFECT_2].CalcValue())))
                return SpellCastResult::SPELL_CAST_OK;
            return SpellCastResult::SPELL_FAILED_FIZZLE;
        }

        void Register() OVERRIDE
        {
            // MoP EFFECT_0 is SPELL_EFFECT_ENERGIZE (not DUMMY)
            OnEffectHitTarget += SpellEffectFn(spell_warl_life_tap_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_ENERGIZE);
            OnCheckCast += SpellCheckCastFn(spell_warl_life_tap_SpellScript::CheckCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_life_tap_SpellScript();
    }
};

// 27285 - Seed of Corruption
/// Updated 4.3.4
class spell_warl_seed_of_corruption : public SpellScriptLoader
{
public:
    spell_warl_seed_of_corruption() : SpellScriptLoader("spell_warl_seed_of_corruption") { }

    class spell_warl_seed_of_corruption_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_seed_of_corruption_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (GetExplTargetUnit())
                targets.remove(GetExplTargetUnit());
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warl_seed_of_corruption_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_seed_of_corruption_SpellScript();
    }
};

// 108558 - Nightfall
class spell_warl_shadow_trance_proc : public SpellScriptLoader
{
public:
    spell_warl_shadow_trance_proc() : SpellScriptLoader("spell_warl_shadow_trance_proc") { }

    class spell_warl_shadow_trance_proc_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_shadow_trance_proc_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_SHADOW_TRANCE))
                return false;
            return true;
        }

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();

            // get shadow trace proc chance
            int32 procChance = std::max(aurEff->GetAmount(), 0);

            if (roll_chance_i(procChance))
                GetTarget()->CastSpell(GetTarget(), SPELL_WARLOCK_SHADOW_TRANCE, true, NULL, aurEff);
        }

        void Register() OVERRIDE
        {
            OnEffectProc += AuraEffectProcFn(spell_warl_shadow_trance_proc_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_shadow_trance_proc_AuraScript();
    }
};

// 86121 - Soul Swap
class spell_warl_soul_swap : public SpellScriptLoader
{
public:
    spell_warl_soul_swap() : SpellScriptLoader("spell_warl_soul_swap") { }

    class spell_warl_soul_swap_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_soul_swap_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            // 94229 (CD marker) is absent in 5.4.8 DBC; override aura is enough to validate.
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_SOUL_SWAP_OVERRIDE))
                return false;
            return true;
        }

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            GetCaster()->CastSpell(GetCaster(), SPELL_WARLOCK_SOUL_SWAP_OVERRIDE, true);
            GetHitUnit()->CastSpell(GetCaster(), SPELL_WARLOCK_SOUL_SWAP_DOT_MARKER, true);
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_soul_swap_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_soul_swap_SpellScript();
    }
};

// 86211 - Soul Swap Override - Also acts as a dot container
class spell_warl_soul_swap_override : public SpellScriptLoader
{
public:
    spell_warl_soul_swap_override() : SpellScriptLoader("spell_warl_soul_swap_override") { }

    class spell_warl_soul_swap_override_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_soul_swap_override_AuraScript);

        bool Load() OVERRIDE
        {
            _swapCaster = NULL;
            return true;
        }

        //! Forced to, pure virtual functions must have a body when linking
        void Register() OVERRIDE { }

    public:
        void AddDot(uint32 id) { _dotList.push_back(id); }
        std::list<uint32> const GetDotList() const { return _dotList; }
        Unit* GetOriginalSwapSource() const { return _swapCaster; }
        void SetOriginalSwapSource(Unit* victim) { _swapCaster = victim; }

    private:
        std::list<uint32> _dotList;
        Unit* _swapCaster;
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_soul_swap_override_AuraScript();
    }
};

typedef spell_warl_soul_swap_override::spell_warl_soul_swap_override_AuraScript SoulSwapOverrideAuraScript;

//! Soul Swap Copy Spells - 92795 - Simply copies spell IDs.
class spell_warl_soul_swap_dot_marker : public SpellScriptLoader
{
public:
    spell_warl_soul_swap_dot_marker() : SpellScriptLoader("spell_warl_soul_swap_dot_marker") { }

    class spell_warl_soul_swap_dot_marker_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_soul_swap_dot_marker_SpellScript);

        void HandleHit(SpellEffIndex effIndex)
        {
            Unit* swapVictim = GetCaster();
            Unit* warlock = GetHitUnit();
            if (!warlock || !swapVictim)
                return;

            flag128 classMask = GetSpellInfo()->Effects[effIndex].SpellClassMask;

            Unit::AuraApplicationMap const& appliedAuras = swapVictim->GetAppliedAuras();
            SoulSwapOverrideAuraScript* swapSpellScript = NULL;
            if (Aura* swapOverrideAura = warlock->GetAura(SPELL_WARLOCK_SOUL_SWAP_OVERRIDE))
                swapSpellScript = dynamic_cast<SoulSwapOverrideAuraScript*>(swapOverrideAura->GetScriptByName("spell_warl_soul_swap_override"));

            if (swapSpellScript == NULL)
                return;

            for (Unit::AuraApplicationMap::const_iterator itr = appliedAuras.begin(); itr != appliedAuras.end(); ++itr)
            {
                SpellInfo const* spellProto = itr->second->GetBase()->GetSpellInfo();
                if (itr->second->GetBase()->GetCaster() == warlock)
                    if (spellProto->SpellFamilyName == SPELLFAMILY_WARLOCK && (spellProto->SpellFamilyFlags & classMask))
                        swapSpellScript->AddDot(itr->first);
            }

            swapSpellScript->SetOriginalSwapSource(swapVictim);
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_soul_swap_dot_marker_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_soul_swap_dot_marker_SpellScript();
    }
};

// 86213 - Soul Swap Exhale
class spell_warl_soul_swap_exhale : public SpellScriptLoader
{
public:
    spell_warl_soul_swap_exhale() : SpellScriptLoader("spell_warl_soul_swap_exhale") { }

    class spell_warl_soul_swap_exhale_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_soul_swap_exhale_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_SOUL_SWAP_MOD_COST) ||
                !sSpellMgr->GetSpellInfo(SPELL_WARLOCK_SOUL_SWAP_OVERRIDE))
                return false;
            return true;
        }

        SpellCastResult CheckCast()
        {
            Unit* currentTarget = GetExplTargetUnit();
            Unit* swapTarget = NULL;
            if (Aura const* swapOverride = GetCaster()->GetAura(SPELL_WARLOCK_SOUL_SWAP_OVERRIDE))
                if (SoulSwapOverrideAuraScript* swapScript = dynamic_cast<SoulSwapOverrideAuraScript*>(swapOverride->GetScriptByName("spell_warl_soul_swap_override")))
                    swapTarget = swapScript->GetOriginalSwapSource();

            // Soul Swap Exhale can't be cast on the same target than Soul Swap
            if (swapTarget && currentTarget && swapTarget == currentTarget)
                return SpellCastResult::SPELL_FAILED_BAD_TARGETS;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void OnEffectHit(SpellEffIndex /*effIndex*/)
        {
            GetCaster()->CastSpell(GetCaster(), SPELL_WARLOCK_SOUL_SWAP_MOD_COST, true);

            std::list<uint32> dotList;
            Unit* swapSource = NULL;
            if (Aura const* swapOverride = GetCaster()->GetAura(SPELL_WARLOCK_SOUL_SWAP_OVERRIDE))
            {
                SoulSwapOverrideAuraScript* swapScript = dynamic_cast<SoulSwapOverrideAuraScript*>(swapOverride->GetScriptByName("spell_warl_soul_swap_override"));
                if (!swapScript)
                    return;
                dotList = swapScript->GetDotList();
                swapSource = swapScript->GetOriginalSwapSource();
            }

            if (dotList.empty())
                return;

            for (std::list<uint32>::const_iterator itr = dotList.begin(); itr != dotList.end(); ++itr)
            {
                GetCaster()->AddAura(*itr, GetHitUnit());
                if (swapSource)
                    swapSource->RemoveAurasDueToSpell(*itr);
            }

            // Remove Soul Swap Exhale buff
            GetCaster()->RemoveAurasDueToSpell(SPELL_WARLOCK_SOUL_SWAP_OVERRIDE);
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_warl_soul_swap_exhale_SpellScript::CheckCast);
            OnEffectHitTarget += SpellEffectFn(spell_warl_soul_swap_exhale_SpellScript::OnEffectHit, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_soul_swap_exhale_SpellScript();
    }
};

// 29858 - Soulshatter
/// Updated 4.3.4
class spell_warl_soulshatter : public SpellScriptLoader
{
public:
    spell_warl_soulshatter() : SpellScriptLoader("spell_warl_soulshatter") { }

    class spell_warl_soulshatter_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_soulshatter_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_SOULSHATTER))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (Unit* target = GetHitUnit())
                if (target->CanHaveThreatList() && target->getThreatManager().getThreat(caster) > 0.0f)
                    caster->CastSpell(target, SPELL_WARLOCK_SOULSHATTER, true);
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_soulshatter_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_soulshatter_SpellScript();
    }
};

// 30108, 34438, 34439, 35183 - Unstable Affliction
/// Updated 4.3.4
class spell_warl_unstable_affliction : public SpellScriptLoader
{
public:
    spell_warl_unstable_affliction() : SpellScriptLoader("spell_warl_unstable_affliction") { }

    class spell_warl_unstable_affliction_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_unstable_affliction_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_WARLOCK_UNSTABLE_AFFLICTION_DISPEL))
                return false;
            return true;
        }

        void HandleDispel(DispelInfo* dispelInfo)
        {
            if (Unit* caster = GetCaster())
                if (AuraEffect const* aurEff = GetEffect(EFFECT_1))
                {
                    int32 damage = aurEff->GetAmount() * 9;
                    // backfire damage and silence
                    caster->CastCustomSpell(dispelInfo->GetDispeller(), SPELL_WARLOCK_UNSTABLE_AFFLICTION_DISPEL, &damage, NULL, NULL, true, NULL, aurEff);
                }
        }

        void Register() OVERRIDE
        {
            AfterDispel += AuraDispelFn(spell_warl_unstable_affliction_AuraScript::HandleDispel);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_unstable_affliction_AuraScript();
    }
};

// 108647 - Burning Embers
class spell_warl_burning_embers : public SpellScriptLoader
{
public:
    spell_warl_burning_embers() : SpellScriptLoader("spell_warl_burning_embers") { }

    class spell_warl_burning_embers_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_burning_embers_AuraScript);

        void HandleProc(ProcEventInfo& eventInfo)
        {
            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            if (!spellInfo)
                return;

            Unit* warlock = GetUnitOwner();
            bool isCrit = (eventInfo.GetHitMask() & PROC_EX_CRITICAL_HIT) != 0;
            int32 embers = 0;

            if (!(eventInfo.GetTypeMask() & PROC_FLAG_DONE_PERIODIC))
            {
                switch (spellInfo->Id)
                {
                    case SPELL_WARLOCK_CONFLAGRATE:
                    case SPELL_WARLOCK_INCINERATE:
                    case SPELL_WARLOCK_FEL_FLAME:
                    case SPELL_WARLOCK_CONFLAGRATE_FNB:
                    case SPELL_WARLOCK_INCINERATE_FNB:
                    case SPELL_WARLOCK_IMMOLATE_FNB:
                        embers = isCrit ? 2 : 1;
                        break;
                    case SPELL_WARLOCK_RAIN_OF_FIRE_DAMAGE:
                        if (roll_chance_i(10))
                            embers = 1;
                        break;
                    default:
                        break;
                }

                // Conflagrate and Incinerate have a 15% chance to generate an additional ember
                if ((spellInfo->SpellFamilyFlags[1] & 0x00800040) && roll_chance_i(15))
                    ++embers;
            }
            else
            {
                switch (spellInfo->Id)
                {
                    case SPELL_WARLOCK_IMMOLATE:
                    case SPELL_WARLOCK_IMMOLATE_FNB:
                        if (isCrit)
                            embers = 1;
                        break;
                    default:
                        break;
                }
            }

            if ((spellInfo->SpellFamilyFlags[1] & 0x40) && warlock->HasAura(SPELL_WARLOCK_T15_4P_BONUS) && roll_chance_i(8))
                embers *= 2;

            if (embers)
                warlock->ModifyPower(POWER_BURNING_EMBERS, embers);
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_warl_burning_embers_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_burning_embers_AuraScript();
    }
};

// 17877 - Shadowburn, 116858 - Chaos Bolt
class spell_warl_ember_consumer : public SpellScriptLoader
{
public:
    spell_warl_ember_consumer() : SpellScriptLoader("spell_warl_ember_consumer") { }

    class spell_warl_ember_consumer_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_ember_consumer_SpellScript);

        void HandleHit()
        {
            if (AuraEffect const* mastery = GetCaster()->GetAuraEffect(SPELL_WARLOCK_MASTERY_EMBERSTORM, EFFECT_0))
                SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), mastery->GetAmount()));
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_warl_ember_consumer_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_ember_consumer_SpellScript();
    }
};

// 116858 - Chaos Bolt (periodic part)
class spell_warl_chaos_bolt_dot : public SpellScriptLoader
{
public:
    spell_warl_chaos_bolt_dot() : SpellScriptLoader("spell_warl_chaos_bolt_dot") { }

    class spell_warl_chaos_bolt_dot_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_chaos_bolt_dot_AuraScript);

        void CalculateTick(AuraEffect const* /*aurEff*/, int32& damage)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (AuraEffect const* mastery = caster->GetAuraEffect(SPELL_WARLOCK_MASTERY_EMBERSTORM, EFFECT_0))
                AddPct(damage, mastery->GetAmount());

            // The periodic part benefits from critical strike chance the same way the direct hit does
            if (caster->GetTypeId() == TypeID::TYPEID_PLAYER)
            {
                float crit = caster->GetFloatValue(PLAYER_FIELD_SPELL_CRIT_PERCENTAGE + GetFirstSchoolInMask(GetSpellInfo()->GetSchoolMask()));
                crit += GetUnitOwner()->GetTotalAuraModifierByMiscMask(SPELL_AURA_MOD_ATTACKER_SPELL_CRIT_CHANCE, GetSpellInfo()->SchoolMask);
                crit += GetUnitOwner()->GetTotalAuraModifier(SPELL_AURA_MOD_ATTACKER_SPELL_AND_WEAPON_CRIT_CHANCE);
                if (crit > 0.0f)
                    AddPct(damage, crit);
            }
        }

        void Register() OVERRIDE
        {
            DoEffectCalcTickAmount += AuraEffectCalcTickAmountFn(spell_warl_chaos_bolt_dot_AuraScript::CalculateTick, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_chaos_bolt_dot_AuraScript();
    }
};

// 6353, 104027 - Soul Fire, 116858 - Chaos Bolt
class spell_warl_always_crit : public SpellScriptLoader
{
public:
    spell_warl_always_crit() : SpellScriptLoader("spell_warl_always_crit") { }

    class spell_warl_always_crit_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_always_crit_SpellScript);

        bool Load() OVERRIDE
        {
            _critMod = 0.0f;
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        // These spells always crit, so critical strike chance is converted into extra damage
        void CalculateCrit(SpellEffIndex /*effIndex*/)
        {
            Unit* target = GetHitUnit();
            if (!target)
                return;

            float crit = GetCaster()->GetFloatValue(PLAYER_FIELD_SPELL_CRIT_PERCENTAGE + GetFirstSchoolInMask(GetSpellInfo()->GetSchoolMask()));
            crit += target->GetTotalAuraModifierByMiscMask(SPELL_AURA_MOD_ATTACKER_SPELL_CRIT_CHANCE, GetSpellInfo()->SchoolMask);
            crit += target->GetTotalAuraModifier(SPELL_AURA_MOD_ATTACKER_SPELL_AND_WEAPON_CRIT_CHANCE);
            _critMod = std::max(crit, 0.0f);
        }

        void HandleHit()
        {
            if (_critMod <= 0.0f)
                return;

            float damage = float(GetHitDamage());
            AddPct(damage, _critMod);
            SetHitDamage(int32(damage));
        }

        void Register() OVERRIDE
        {
            OnEffectLaunchTarget += SpellEffectFn(spell_warl_always_crit_SpellScript::CalculateCrit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            OnHit += SpellHitFn(spell_warl_always_crit_SpellScript::HandleHit);
        }

        float _critMod;
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_always_crit_SpellScript();
    }
};

// 17962 - Conflagrate
class spell_warl_conflagrate_aura : public SpellScriptLoader
{
public:
    spell_warl_conflagrate_aura() : SpellScriptLoader("spell_warl_conflagrate_aura") { }

    class spell_warl_conflagrate_aura_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_conflagrate_aura_SpellScript);

        bool Load() OVERRIDE
        {
            _preventSnare = false;
            return true;
        }

        void CheckImmolate()
        {
            if (Unit* target = GetHitUnit())
                _preventSnare = !target->HasAura(SPELL_WARLOCK_IMMOLATE, GetCaster()->GetGUID()) && !GetCaster()->HasAura(SPELL_WARLOCK_GLYPH_OF_CONFLAGRATE);
        }

        // The snare from effect 1 is only applied to targets burning from Immolate
        void HandleAfterHit()
        {
            if (_preventSnare)
                PreventHitAura();
        }

        void Register() OVERRIDE
        {
            BeforeHit += SpellHitFn(spell_warl_conflagrate_aura_SpellScript::CheckImmolate);
            AfterHit += SpellHitFn(spell_warl_conflagrate_aura_SpellScript::HandleAfterHit);
        }

        bool _preventSnare;
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_conflagrate_aura_SpellScript();
    }
};

// 29341 - Shadowburn
class spell_warl_shadowburn : public SpellScriptLoader
{
public:
    spell_warl_shadowburn() : SpellScriptLoader("spell_warl_shadowburn") { }

    class spell_warl_shadowburn_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_shadowburn_AuraScript);

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            Player* warlock = caster ? caster->ToPlayer() : NULL;
            if (!warlock)
                return;

            AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
            if (removeMode == AURA_REMOVE_BY_DEATH && warlock->isHonorOrXPTarget(GetTarget()))
                warlock->ModifyPower(POWER_BURNING_EMBERS, 20);
            else if (removeMode == AURA_REMOVE_BY_EXPIRE)
                warlock->CastSpell(warlock, SPELL_WARLOCK_SHADOWBURN_ENERGIZE, true);
        }

        void Register() OVERRIDE
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_warl_shadowburn_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_shadowburn_AuraScript();
    }
};

// 114635 - Ember Tap
class spell_warl_ember_tap : public SpellScriptLoader
{
public:
    spell_warl_ember_tap() : SpellScriptLoader("spell_warl_ember_tap") { }

    class spell_warl_ember_tap_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_ember_tap_SpellScript);

        // With the glyph the heal is applied over time by effect 2 instead
        void HandleHeal(SpellEffIndex effIndex)
        {
            if (GetCaster()->HasAura(SPELL_WARLOCK_GLYPH_OF_EMBER_TAP))
                PreventHitDefaultEffect(effIndex);
        }

        void HandleHit()
        {
            if (AuraEffect const* mastery = GetCaster()->GetAuraEffect(SPELL_WARLOCK_MASTERY_EMBERSTORM, EFFECT_0))
                SetHitHeal(GetHitHeal() + CalculatePct(GetHitHeal(), mastery->GetAmount()));
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_ember_tap_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL_PCT);
            OnHit += SpellHitFn(spell_warl_ember_tap_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_ember_tap_SpellScript();
    }
};

// 114635 - Ember Tap (glyphed heal over time)
class spell_warl_glyph_of_ember_tap : public SpellScriptLoader
{
public:
    spell_warl_glyph_of_ember_tap() : SpellScriptLoader("spell_warl_glyph_of_ember_tap") { }

    class spell_warl_glyph_of_ember_tap_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_glyph_of_ember_tap_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* caster = GetCaster())
                if (AuraEffect const* mastery = caster->GetAuraEffect(SPELL_WARLOCK_MASTERY_EMBERSTORM, EFFECT_0))
                    AddPct(amount, mastery->GetAmount());
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_glyph_of_ember_tap_AuraScript::CalculateAmount, EFFECT_2, SPELL_AURA_OBS_MOD_HEALTH);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_glyph_of_ember_tap_AuraScript();
    }
};

// 104225 - Curse of the Elements, 108685 - Conflagrate, 108686 - Immolate, 109468 - Curse of Enfeeblement, 114654 - Incinerate
class spell_warl_fire_and_brimstone : public SpellScriptLoader
{
public:
    spell_warl_fire_and_brimstone() : SpellScriptLoader("spell_warl_fire_and_brimstone") { }

    class spell_warl_fire_and_brimstone_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_fire_and_brimstone_SpellScript);

        void HandleAfterCast()
        {
            if (GetCaster()->GetPower(POWER_BURNING_EMBERS) < 10)
                GetCaster()->RemoveAurasDueToSpell(SPELL_WARLOCK_FIRE_AND_BRIMSTONE);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_warl_fire_and_brimstone_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_fire_and_brimstone_SpellScript();
    }
};

// 108685 - Conflagrate, 108686 - Immolate, 114654 - Incinerate
class spell_warl_fire_and_brimstone_spell : public SpellScriptLoader
{
public:
    spell_warl_fire_and_brimstone_spell() : SpellScriptLoader("spell_warl_fire_and_brimstone_spell") { }

    class spell_warl_fire_and_brimstone_spell_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_fire_and_brimstone_spell_SpellScript);

        // Fire and Brimstone versions deal 35% damage, increased by Mastery: Emberstorm
        void HandleHit()
        {
            int32 mastery = 0;
            if (AuraEffect const* masteryEff = GetCaster()->GetAuraEffect(SPELL_WARLOCK_MASTERY_EMBERSTORM, EFFECT_0))
                mastery = masteryEff->GetAmount();
            SetHitDamage(CalculatePct(GetHitDamage(), 35 * (100 + mastery) / 100));
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_warl_fire_and_brimstone_spell_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_fire_and_brimstone_spell_SpellScript();
    }
};

// 108686 - Immolate (Fire and Brimstone)
class spell_warl_fire_and_brimstone_immolate : public SpellScriptLoader
{
public:
    spell_warl_fire_and_brimstone_immolate() : SpellScriptLoader("spell_warl_fire_and_brimstone_immolate") { }

    class spell_warl_fire_and_brimstone_immolate_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_fire_and_brimstone_immolate_AuraScript);

        void CalculateTick(AuraEffect const* /*aurEff*/, int32& damage)
        {
            int32 mastery = 0;
            if (Unit* caster = GetCaster())
                if (AuraEffect const* masteryEff = caster->GetAuraEffect(SPELL_WARLOCK_MASTERY_EMBERSTORM, EFFECT_0))
                    mastery = masteryEff->GetAmount();
            damage = CalculatePct(damage, 35 * (100 + mastery) / 100);
        }

        void Register() OVERRIDE
        {
            DoEffectCalcTickAmount += AuraEffectCalcTickAmountFn(spell_warl_fire_and_brimstone_immolate_AuraScript::CalculateTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_fire_and_brimstone_immolate_AuraScript();
    }
};

// 108563 - Backlash
class spell_warl_backlash : public SpellScriptLoader
{
public:
    spell_warl_backlash() : SpellScriptLoader("spell_warl_backlash") { }

    class spell_warl_backlash_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_backlash_AuraScript);

        void HandleProc(ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            uint32 spellId = GetUnitOwner()->HasAura(SPELL_WARLOCK_THE_CODEX_OF_XERRATH) ? SPELL_WARLOCK_BACKLASH_CODEX : SPELL_WARLOCK_BACKLASH;
            GetUnitOwner()->CastSpell(GetUnitOwner(), spellId, true);
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_warl_backlash_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_backlash_AuraScript();
    }
};

// 80240 - Havoc
class spell_warl_havoc : public SpellScriptLoader
{
public:
    spell_warl_havoc() : SpellScriptLoader("spell_warl_havoc") { }

    class spell_warl_havoc_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_havoc_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetAura()->SetStackAmount(GetSpellInfo()->StackAmount);
        }

        void Register() OVERRIDE
        {
            AfterEffectApply += AuraEffectApplyFn(spell_warl_havoc_AuraScript::HandleApply, EFFECT_1, SPELL_AURA_ADD_FLAT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_havoc_AuraScript();
    }
};

// 348 - Immolate, 1490 - Curse of the Elements, 6789 - Mortal Coil, 17877 - Shadowburn, 17962 - Conflagrate,
// 29722 - Incinerate, 77799 - Fel Flame, 109466 - Curse of Enfeeblement, 116858 - Chaos Bolt, 132409 - Spell Lock
class spell_warl_havoc_target_selector : public SpellScriptLoader
{
public:
    spell_warl_havoc_target_selector() : SpellScriptLoader("spell_warl_havoc_target_selector") { }

    class spell_warl_havoc_target_selector_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_havoc_target_selector_SpellScript);

        // Duplicates the spell onto the Havoc target; the copy is cast triggered so it can't chain
        void HandleAfterCast()
        {
            if (GetSpell()->IsTriggered())
                return;

            Unit* caster = GetCaster();
            Aura* havoc = caster->GetAura(SPELL_WARLOCK_HAVOC, caster->GetGUID());
            if (!havoc)
                return;

            uint8 charges = GetSpellInfo()->Id == SPELL_WARLOCK_CHAOS_BOLT ? 3 : 1;
            if (havoc->GetStackAmount() < charges)
                return;

            float range = GetSpellInfo()->GetMaxRange(false, caster);
            std::list<Unit*> units;
            caster->GetAttackableUnitListInRange(units, range);
            units.remove_if(Skyfire::UnitAuraCheck(false, SPELL_WARLOCK_HAVOC, caster->GetGUID()));

            Unit* explTarget = GetExplTargetUnit();
            for (std::list<Unit*>::const_iterator itr = units.begin(); itr != units.end(); ++itr)
            {
                Unit* target = *itr;
                if (target == caster || target == explTarget || !caster->IsWithinLOSInMap(target))
                    continue;

                caster->CastSpell(target, GetSpellInfo()->Id, true);
                havoc->ModStackAmount(-int32(charges));
                break;
            }
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_warl_havoc_target_selector_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_havoc_target_selector_SpellScript();
    }
};

// 104232 - Rain of Fire
class spell_warl_rain_of_fire : public SpellScriptLoader
{
public:
    spell_warl_rain_of_fire() : SpellScriptLoader("spell_warl_rain_of_fire") { }

    class spell_warl_rain_of_fire_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_rain_of_fire_AuraScript);

        void HandleTick(AuraEffect const* aurEff)
        {
            PreventDefaultAction();

            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (DynamicObject* dynObj = caster->GetDynObject(GetId()))
                caster->CastSpell(dynObj->GetPositionX(), dynObj->GetPositionY(), dynObj->GetPositionZ(), GetSpellInfo()->Effects[aurEff->GetEffIndex()].TriggerSpell, true);
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_rain_of_fire_AuraScript::HandleTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_rain_of_fire_AuraScript();
    }
};

// 77219 - Mastery: Master Demonologist
class spell_warl_mastery_master_demonologist : public SpellScriptLoader
{
public:
    spell_warl_mastery_master_demonologist() : SpellScriptLoader("spell_warl_mastery_master_demonologist") { }

    class spell_warl_mastery_master_demonologist_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_mastery_master_demonologist_AuraScript);

        void HandleApply(AuraEffect const* mastery, AuraEffectHandleModes /*mode*/)
        {
            Unit* warlock = GetTarget();

            if (AuraEffect* metamorphosis = warlock->GetAuraEffect(SPELL_WARLOCK_METAMORPHOSIS, EFFECT_1))
                metamorphosis->ChangeAmount(mastery->GetAmount() * 3);
            if (AuraEffect* apotheosis = warlock->GetAuraEffect(SPELL_WARLOCK_DARK_APOTHEOSIS, EFFECT_1))
                apotheosis->RecalculateAmount();
            if (AuraEffect* demonicFury = warlock->GetAuraEffect(SPELL_WARLOCK_DEMONIC_FURY, EFFECT_1))
                demonicFury->ChangeAmount(warlock->HasAura(SPELL_WARLOCK_METAMORPHOSIS) ? 0 : mastery->GetAmount());

            if (Player* player = warlock->ToPlayer())
                if (Pet* pet = player->GetPet())
                    if (AuraEffect* petBuff = pet->GetAuraEffect(SPELL_WARLOCK_MASTER_DEMONOLOGIST, EFFECT_0))
                        petBuff->ChangeAmount(mastery->GetAmount());
        }

        void Register() OVERRIDE
        {
            AfterEffectApply += AuraEffectApplyFn(spell_warl_mastery_master_demonologist_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_mastery_master_demonologist_AuraScript();
    }
};

// 115556 - Master Demonologist (pet buff)
class spell_warl_master_demonologist : public SpellScriptLoader
{
public:
    spell_warl_master_demonologist() : SpellScriptLoader("spell_warl_master_demonologist") { }

    class spell_warl_master_demonologist_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_master_demonologist_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* owner = GetUnitOwner()->GetOwner())
                if (AuraEffect const* mastery = owner->GetAuraEffect(SPELL_WARLOCK_MASTERY_MASTER_DEMONOLOGIST, EFFECT_0))
                    amount = mastery->GetAmount();
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_master_demonologist_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_master_demonologist_AuraScript();
    }
};

// 109145 - Demonic Fury
class spell_warl_demonic_fury : public SpellScriptLoader
{
public:
    spell_warl_demonic_fury() : SpellScriptLoader("spell_warl_demonic_fury") { }

    class spell_warl_demonic_fury_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_demonic_fury_AuraScript);

        // Outside of Metamorphosis the mastery increases damage done by its base value
        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            Unit* warlock = GetUnitOwner();
            if (warlock->HasAura(SPELL_WARLOCK_METAMORPHOSIS))
                amount = 0;
            else if (AuraEffect const* mastery = warlock->GetAuraEffect(SPELL_WARLOCK_MASTERY_MASTER_DEMONOLOGIST, EFFECT_0))
                amount = mastery->GetAmount();
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_demonic_fury_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_demonic_fury_AuraScript();
    }
};

// 103958 - Metamorphosis
class spell_warl_metamorphosis : public SpellScriptLoader
{
public:
    spell_warl_metamorphosis() : SpellScriptLoader("spell_warl_metamorphosis") { }

    class spell_warl_metamorphosis_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_metamorphosis_AuraScript);

        // In Metamorphosis the mastery bonus is tripled
        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (Unit* caster = GetCaster())
                if (AuraEffect const* mastery = caster->GetAuraEffect(SPELL_WARLOCK_MASTERY_MASTER_DEMONOLOGIST, EFFECT_0))
                    amount = mastery->GetAmount() * 3;
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (AuraEffect* demonicFury = GetTarget()->GetAuraEffect(SPELL_WARLOCK_DEMONIC_FURY, EFFECT_1))
                demonicFury->ChangeAmount(0);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* warlock = GetTarget();
            if (AuraEffect const* mastery = warlock->GetAuraEffect(SPELL_WARLOCK_MASTERY_MASTER_DEMONOLOGIST, EFFECT_0))
                if (AuraEffect* demonicFury = warlock->GetAuraEffect(SPELL_WARLOCK_DEMONIC_FURY, EFFECT_1))
                    demonicFury->ChangeAmount(mastery->GetAmount());
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_metamorphosis_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
            OnEffectApply += AuraEffectApplyFn(spell_warl_metamorphosis_AuraScript::HandleApply, EFFECT_1, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_warl_metamorphosis_AuraScript::HandleRemove, EFFECT_1, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_metamorphosis_AuraScript();
    }
};

// 114168 - Dark Apotheosis
class spell_warl_dark_apotheosis : public SpellScriptLoader
{
public:
    spell_warl_dark_apotheosis() : SpellScriptLoader("spell_warl_dark_apotheosis") { }

    class spell_warl_dark_apotheosis_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_dark_apotheosis_AuraScript);

        void CalculateDamageTaken(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (AuraEffect const* mastery = GetUnitOwner()->GetAuraEffect(SPELL_WARLOCK_MASTERY_MASTER_DEMONOLOGIST, EFFECT_0))
            {
                float value = float(mastery->GetAmount());
                amount = -int32(10.0f + value / (value + 200.0f) * 100.0f);
            }
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetTarget()->GetPower(POWER_DEMONIC_FURY) > 200)
                GetTarget()->SetPower(POWER_DEMONIC_FURY, 200);
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_dark_apotheosis_AuraScript::CalculateDamageTaken, EFFECT_1, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN);
            OnEffectRemove += AuraEffectRemoveFn(spell_warl_dark_apotheosis_AuraScript::HandleRemove, EFFECT_1, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_dark_apotheosis_AuraScript();
    }
};

// 114168 - Dark Apotheosis
class spell_warl_fury_ward_fix : public SpellScriptLoader
{
public:
    spell_warl_fury_ward_fix() : SpellScriptLoader("spell_warl_fury_ward_fix") { }

    class spell_warl_fury_ward_fix_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_fury_ward_fix_AuraScript);

        void HandleBonus(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (AuraEffect* effect = GetTarget()->GetAuraEffect(SPELL_WARLOCK_PVP_SET_2P_BONUS, EFFECT_0))
                effect->RecalculateAmount();
            if (AuraEffect* effect = GetTarget()->GetAuraEffect(SPELL_WARLOCK_PVP_SET_2P_BONUS, EFFECT_1))
                effect->RecalculateAmount();
        }

        void Register() OVERRIDE
        {
            AfterEffectApply += AuraEffectApplyFn(spell_warl_fury_ward_fix_AuraScript::HandleBonus, EFFECT_2, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_warl_fury_ward_fix_AuraScript::HandleBonus, EFFECT_2, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_fury_ward_fix_AuraScript();
    }
};

// 131632 - Item - Warlock PvP Set 2P Bonus
class spell_warl_twilight_ward_fix : public SpellScriptLoader
{
public:
    spell_warl_twilight_ward_fix() : SpellScriptLoader("spell_warl_twilight_ward_fix") { }

    class spell_warl_twilight_ward_fix_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_twilight_ward_fix_AuraScript);

        // Dark Apotheosis overrides Twilight Ward with Fury Ward, which the set bonus must not replace
        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            if (GetUnitOwner()->HasAura(SPELL_WARLOCK_DARK_APOTHEOSIS))
                amount = 0;
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_twilight_ward_fix_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_twilight_ward_fix_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_twilight_ward_fix_AuraScript();
    }
};

// 122355, 140074 - Molten Core
class spell_warl_molten_core : public SpellScriptLoader
{
public:
    spell_warl_molten_core() : SpellScriptLoader("spell_warl_molten_core") { }

    class spell_warl_molten_core_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_molten_core_AuraScript);

        uint32 GetVisualSpellId() const
        {
            return GetId() == SPELL_WARLOCK_MOLTEN_CORE ? SPELL_WARLOCK_MOLTEN_CORE_VISUAL : SPELL_WARLOCK_MOLTEN_CORE_VISUAL_CODEX;
        }

        // Every new proc adds one charge, up to 10
        void HandleRefreshCharges(uint8& charges)
        {
            charges = uint8(std::min(10, GetCharges() + 1));
            if (charges > 1)
                GetUnitOwner()->CastSpell(GetUnitOwner(), GetVisualSpellId(), true);
        }

        void HandleDropModCharge(Spell* /*bySpell*/)
        {
            if (GetCharges() == 2)
                GetUnitOwner()->RemoveAurasDueToSpell(GetVisualSpellId());
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->RemoveAurasDueToSpell(GetVisualSpellId());
        }

        void Register() OVERRIDE
        {
            OnRefreshCharges += AuraRefreshChargesFn(spell_warl_molten_core_AuraScript::HandleRefreshCharges);
            OnDropModCharge += AuraDropModChargeFn(spell_warl_molten_core_AuraScript::HandleDropModCharge);
            OnEffectRemove += AuraEffectRemoveFn(spell_warl_molten_core_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_molten_core_AuraScript();
    }
};

static uint32 GetMoltenCoreSpellId(Unit const* warlock)
{
    return warlock->HasAura(SPELL_WARLOCK_THE_CODEX_OF_XERRATH) ? SPELL_WARLOCK_MOLTEN_CORE_CODEX : SPELL_WARLOCK_MOLTEN_CORE;
}

// 686, 112092 - Shadow Bolt, 6353, 104027 - Soul Fire
class spell_warl_decimate : public SpellScriptLoader
{
public:
    spell_warl_decimate() : SpellScriptLoader("spell_warl_decimate") { }

    class spell_warl_decimate_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_decimate_SpellScript);

        void HandleAfterCast()
        {
            Unit* caster = GetCaster();
            Unit* target = GetExplTargetUnit();
            if (target && target->HealthBelowPct(25) && caster->HasAura(SPELL_WARLOCK_DECIMATION))
                caster->CastSpell(caster, GetMoltenCoreSpellId(caster), true);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_warl_decimate_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_decimate_SpellScript();
    }
};

// 124916 - Chaos Wave
class spell_warl_chaos_wave : public SpellScriptLoader
{
public:
    spell_warl_chaos_wave() : SpellScriptLoader("spell_warl_chaos_wave") { }

    class spell_warl_chaos_wave_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_chaos_wave_SpellScript);

        void HandleAfterCast()
        {
            GetCaster()->CastSpell(GetCaster(), GetMoltenCoreSpellId(GetCaster()), true);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_warl_chaos_wave_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_chaos_wave_SpellScript();
    }
};

// 143381 - Hand of Gul'dan
class spell_warl_hand_of_guldan : public SpellScriptLoader
{
public:
    spell_warl_hand_of_guldan() : SpellScriptLoader("spell_warl_hand_of_guldan") { }

    class spell_warl_hand_of_guldan_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_hand_of_guldan_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Unit* target = GetHitUnit())
                GetCaster()->CastSpell(target, SPELL_WARLOCK_HAND_OF_GULDAN_DAMAGE, true);
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_hand_of_guldan_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_hand_of_guldan_SpellScript();
    }
};

// 123194 - Hand of Gul'dan (Glyph of Hand of Gul'dan)
class spell_warl_hand_of_guldan_glyphed : public SpellScriptLoader
{
public:
    spell_warl_hand_of_guldan_glyphed() : SpellScriptLoader("spell_warl_hand_of_guldan_glyphed") { }

    class spell_warl_hand_of_guldan_glyphed_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_hand_of_guldan_glyphed_SpellScript);

        void HandleCast()
        {
            if (WorldLocation const* dest = GetExplTargetDest())
                GetCaster()->SendPlaySpellVisual(21982, dest->GetPositionX(), dest->GetPositionY(), dest->GetPositionZ(), GetSpellInfo()->Speed);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_warl_hand_of_guldan_glyphed_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_hand_of_guldan_glyphed_SpellScript();
    }
};

// 47960 - Shadowflame
class spell_warl_shadowflame : public SpellScriptLoader
{
public:
    spell_warl_shadowflame() : SpellScriptLoader("spell_warl_shadowflame") { }

    class spell_warl_shadowflame_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_shadowflame_AuraScript);

        void HandleTick(AuraEffect const* /*aurEff*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (caster->HasAura(SPELL_WARLOCK_MOLTEN_CORE_PASSIVE) && roll_chance_i(8))
                caster->CastSpell(caster, GetMoltenCoreSpellId(caster), true);

            caster->CastCustomSpell(SPELL_WARLOCK_DEMONIC_FURY_ENERGIZE, SPELLVALUE_BASE_POINT1, 2, caster, true);
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_shadowflame_AuraScript::HandleTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_shadowflame_AuraScript();
    }
};

// 104316 - Imp Swarm
class spell_warl_imp_swarm : public SpellScriptLoader
{
public:
    spell_warl_imp_swarm() : SpellScriptLoader("spell_warl_imp_swarm") { }

    class spell_warl_imp_swarm_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_imp_swarm_SpellScript);

        SpellCastResult CheckSpec()
        {
            Player* caster = GetCaster()->ToPlayer();
            if (!caster || caster->GetTalentSpecialization(caster->GetActiveSpec()) != SPEC_WARLOCK_DEMONOLOGY)
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void SummonImps(SpellEffIndex effIndex)
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!target)
                return;

            for (int32 i = 0; i < GetSpellInfo()->Effects[effIndex].BasePoints; ++i)
            {
                caster->AddAura(SPELL_WARLOCK_DEMONIC_CALLING, caster);
                caster->CastSpell(target, SPELL_WARLOCK_WILD_IMP_SUMMON, true);
            }
            caster->RemoveAurasDueToSpell(SPELL_WARLOCK_DEMONIC_CALLING);
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_warl_imp_swarm_SpellScript::CheckSpec);
            OnEffectHitTarget += SpellEffectFn(spell_warl_imp_swarm_SpellScript::SummonImps, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_imp_swarm_SpellScript();
    }
};

// 56242 - Glyph of Imp Swarm
class spell_warl_glyph_of_imp_swarm : public SpellScriptLoader
{
public:
    spell_warl_glyph_of_imp_swarm() : SpellScriptLoader("spell_warl_glyph_of_imp_swarm") { }

    class spell_warl_glyph_of_imp_swarm_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_glyph_of_imp_swarm_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* player = GetTarget()->ToPlayer())
                player->learnSpell(SPELL_WARLOCK_IMP_SWARM, false);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* player = GetTarget()->ToPlayer())
                if (player->HasSpell(SPELL_WARLOCK_IMP_SWARM))
                    player->removeSpell(SPELL_WARLOCK_IMP_SWARM, false, false);
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_warl_glyph_of_imp_swarm_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_warl_glyph_of_imp_swarm_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_glyph_of_imp_swarm_AuraScript();
    }
};

// 114592 - Wild Imps
class spell_warl_wild_imp : public SpellScriptLoader
{
public:
    spell_warl_wild_imp() : SpellScriptLoader("spell_warl_wild_imp") { }

    class spell_warl_wild_imp_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_wild_imp_AuraScript);

        // Glyph of Imp Swarm adds 4 seconds (affected by haste) to the summon interval
        void CalculatePeriodic(AuraEffect const* /*aurEff*/, bool& /*isPeriodic*/, int32& amplitude)
        {
            Unit* owner = GetUnitOwner();
            if (owner->HasAura(SPELL_WARLOCK_GLYPH_OF_IMP_SWARM))
            {
                float castSpeed = owner->GetFloatValue(UNIT_FIELD_MOD_CASTING_SPEED);
                if (castSpeed > 0.0f)
                    amplitude += int32(4000.0f / castSpeed);
            }
        }

        void HandleTick(AuraEffect const* /*aurEff*/)
        {
            if (GetUnitOwner()->HasAura(SPELL_WARLOCK_DISRUPTED_NETHER))
                PreventDefaultAction();
        }

        void Register() OVERRIDE
        {
            DoEffectCalcPeriodic += AuraEffectCalcPeriodicFn(spell_warl_wild_imp_AuraScript::CalculatePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_wild_imp_AuraScript::HandleTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_wild_imp_AuraScript();
    }
};

// 114925 - Demonic Calling
class spell_warl_demonic_calling : public SpellScriptLoader
{
public:
    spell_warl_demonic_calling() : SpellScriptLoader("spell_warl_demonic_calling") { }

    class spell_warl_demonic_calling_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_demonic_calling_AuraScript);

        bool CheckProc(ProcEventInfo& /*eventInfo*/)
        {
            return !GetUnitOwner()->HasAura(SPELL_WARLOCK_DISRUPTED_NETHER);
        }

        void HandleProc(ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();

            Unit* warlock = GetUnitOwner();
            warlock->CastSpell(warlock, SPELL_WARLOCK_WILD_IMP_SUMMON, true);
            if (AuraEffect* timer = warlock->GetAuraEffect(SPELL_WARLOCK_WILD_IMPS, EFFECT_0))
                timer->CalculatePeriodic(warlock, true);
            Remove();
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_warl_demonic_calling_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_warl_demonic_calling_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_demonic_calling_AuraScript();
    }
};

// 104318 - Fel Firebolt (Wild Imp)
class spell_warl_fel_firebolt : public SpellScriptLoader
{
public:
    spell_warl_fel_firebolt() : SpellScriptLoader("spell_warl_fel_firebolt") { }

    class spell_warl_fel_firebolt_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_fel_firebolt_SpellScript);

        void HandleHit()
        {
            Unit* owner = GetCaster()->GetOwner();
            if (!owner)
                return;

            if (AuraEffect const* grimoire = owner->GetAuraEffect(SPELL_WARLOCK_GRIMOIRE_OF_SACRIFICE, EFFECT_3))
            {
                int32 damage = GetHitDamage();
                AddPct(damage, grimoire->GetAmount());
                SetHitDamage(damage);
            }

            if (owner->HasAura(SPELL_WARLOCK_MOLTEN_CORE_PASSIVE) && roll_chance_i(8))
                owner->CastSpell(owner, GetMoltenCoreSpellId(owner), true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_warl_fel_firebolt_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_fel_firebolt_SpellScript();
    }
};

// 5857 - Hellfire
class spell_warl_hellfire : public SpellScriptLoader
{
public:
    spell_warl_hellfire() : SpellScriptLoader("spell_warl_hellfire") { }

    class spell_warl_hellfire_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_hellfire_SpellScript);

        bool Load() OVERRIDE
        {
            _targetCount = 0;
            return true;
        }

        void CountTargets(std::list<WorldObject*>& targets)
        {
            _targetCount = uint32(targets.size());
        }

        // Fury per tick is a base amount from Hellfire's effect 2 plus effect 3 per target hit
        void HandleAfterCast()
        {
            Unit* caster = GetCaster();
            if (caster->GetShapeshiftForm() == FORM_METAMORPHOSIS)
                return;

            SpellInfo const* hellfire = sSpellMgr->GetSpellInfo(SPELL_WARLOCK_HELLFIRE);
            if (!hellfire)
                return;

            float demonicFury = float(hellfire->Effects[EFFECT_2].CalcValue(caster));
            float perTarget = float(hellfire->Effects[EFFECT_3].CalcValue(caster));
            if (caster->HasAura(SPELL_WARLOCK_T15_4P_BONUS))
                perTarget *= 1.1f;

            demonicFury += _targetCount * perTarget;
            caster->CastCustomSpell(SPELL_WARLOCK_DEMONIC_FURY_ENERGIZE, SPELLVALUE_BASE_POINT1, int32(demonicFury), caster, true);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warl_hellfire_SpellScript::CountTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            AfterCast += SpellCastFn(spell_warl_hellfire_SpellScript::HandleAfterCast);
        }

        uint32 _targetCount;
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_hellfire_SpellScript();
    }
};

// 103964 - Touch of Chaos
class spell_warl_touch_of_chaos : public SpellScriptLoader
{
public:
    spell_warl_touch_of_chaos() : SpellScriptLoader("spell_warl_touch_of_chaos") { }

    class spell_warl_touch_of_chaos_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_touch_of_chaos_SpellScript);

        // Extends the caster's Corruption by 6 seconds, up to 150% of its normal duration
        void HandleScript(SpellEffIndex /*effIndex*/)
        {
            Unit* target = GetHitUnit();
            if (!target)
                return;

            if (Aura* corruption = target->GetAura(SPELL_WARLOCK_CORRUPTION_DOT, GetCaster()->GetGUID()))
            {
                int32 maxDuration = int32(corruption->CalcMaxDuration() * 1.5f);
                int32 newDuration = std::min(corruption->GetDuration() + 6000, maxDuration);
                corruption->SetMaxDuration(newDuration);
                corruption->SetDuration(newDuration);
                corruption->RecalculateAmountOfEffects();
            }
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_touch_of_chaos_SpellScript::HandleScript, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_touch_of_chaos_SpellScript();
    }
};

// 146739 - Corruption
class spell_warl_corruption : public SpellScriptLoader
{
public:
    spell_warl_corruption() : SpellScriptLoader("spell_warl_corruption") { }

    class spell_warl_corruption_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_corruption_AuraScript);

        void HandleTick(AuraEffect const* /*aurEff*/)
        {
            Unit* caster = GetCaster();
            Player* warlock = caster ? caster->ToPlayer() : NULL;
            if (!warlock)
                return;

            if (warlock->GetTalentSpecialization(warlock->GetActiveSpec()) == SPEC_WARLOCK_DEMONOLOGY)
                warlock->CastCustomSpell(SPELL_WARLOCK_DEMONIC_FURY_ENERGIZE, SPELLVALUE_BASE_POINT1, 4, warlock, true);
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_corruption_AuraScript::HandleTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_corruption_AuraScript();
    }
};

// 109151 - Demonic Leap
class spell_warl_demonic_leap : public SpellScriptLoader
{
public:
    spell_warl_demonic_leap() : SpellScriptLoader("spell_warl_demonic_leap") { }

    class spell_warl_demonic_leap_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_demonic_leap_SpellScript);

        SpellCastResult CheckRoot()
        {
            if (GetCaster()->HasAuraType(SPELL_AURA_MOD_ROOT))
                return SpellCastResult::SPELL_FAILED_ROOTED;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void HandleAfterCast()
        {
            Unit* warlock = GetCaster();
            if (!warlock->HasAura(SPELL_WARLOCK_METAMORPHOSIS) && !warlock->HasAura(SPELL_WARLOCK_DARK_APOTHEOSIS))
                warlock->CastSpell(warlock, SPELL_WARLOCK_METAMORPHOSIS, true);
            warlock->CastSpell(warlock, SPELL_WARLOCK_DEMONIC_LEAP_JUMP, true);
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_warl_demonic_leap_SpellScript::CheckRoot);
            AfterCast += SpellCastFn(spell_warl_demonic_leap_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_demonic_leap_SpellScript();
    }
};

// 54785, 109163 - Demonic Leap (jump)
class spell_warl_demonic_leap_jump : public SpellScriptLoader
{
public:
    spell_warl_demonic_leap_jump() : SpellScriptLoader("spell_warl_demonic_leap_jump") { }

    class spell_warl_demonic_leap_jump_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_demonic_leap_jump_SpellScript);

        // Glyph of Falling Meteor: Meteor Slam when leaping from at least ~15 yards above the ground
        void HandleAfterCast()
        {
            Unit* caster = GetCaster();
            if (caster->HasAura(SPELL_WARLOCK_GLYPH_OF_FALLING_METEOR) && caster->HasUnitMovementFlag(MOVEMENTFLAG_FALLING))
            {
                float z = caster->GetPositionZ();
                caster->UpdateAllowedPositionZ(caster->GetPositionX(), caster->GetPositionY(), z);
                if (caster->GetPositionZ() - z >= 14.57f)
                    caster->CastSpell(caster, SPELL_WARLOCK_METEOR_SLAM, true);
            }
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_warl_demonic_leap_jump_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_demonic_leap_jump_SpellScript();
    }
};

// 124315 - Meteor Slam
class spell_warl_meteor_slam : public SpellScriptLoader
{
public:
    spell_warl_meteor_slam() : SpellScriptLoader("spell_warl_meteor_slam") { }

    class spell_warl_meteor_slam_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_meteor_slam_AuraScript);

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_DEFAULT)
                if (Player* warlock = GetTarget()->ToPlayer())
                    warlock->SetFallInformation(0, warlock->GetPositionZ());
        }

        void Register() OVERRIDE
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_warl_meteor_slam_AuraScript::HandleRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_meteor_slam_AuraScript();
    }
};

// 63303 - Glyph of Demon Hunting
class spell_warl_glyph_of_demon_hunting : public SpellScriptLoader
{
public:
    spell_warl_glyph_of_demon_hunting() : SpellScriptLoader("spell_warl_glyph_of_demon_hunting") { }

    class spell_warl_glyph_of_demon_hunting_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_glyph_of_demon_hunting_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* warlock = GetTarget()->ToPlayer())
                warlock->learnSpell(SPELL_WARLOCK_DARK_APOTHEOSIS, false);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* warlock = GetTarget()->ToPlayer())
                warlock->removeSpell(SPELL_WARLOCK_DARK_APOTHEOSIS);
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_warl_glyph_of_demon_hunting_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_warl_glyph_of_demon_hunting_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_glyph_of_demon_hunting_AuraScript();
    }
};

// 56240 - Glyph of Shadow Bolt
class spell_warl_glyph_of_shadow_bolt : public SpellScriptLoader
{
public:
    spell_warl_glyph_of_shadow_bolt() : SpellScriptLoader("spell_warl_glyph_of_shadow_bolt") { }

    class spell_warl_glyph_of_shadow_bolt_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_glyph_of_shadow_bolt_AuraScript);

        void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (Player* warlock = GetTarget()->ToPlayer())
                if (warlock->GetTalentSpecialization(warlock->GetActiveSpec()) == SPEC_WARLOCK_DEMONOLOGY)
                    warlock->learnSpell(aurEff->GetAmount(), true);
        }

        void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (Player* warlock = GetTarget()->ToPlayer())
                warlock->removeSpell(aurEff->GetAmount());
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_warl_glyph_of_shadow_bolt_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_warl_glyph_of_shadow_bolt_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_glyph_of_shadow_bolt_AuraScript();
    }
};

// 104315 - Demonic Fury (Demonology passive)
class spell_warl_glyph_of_shadow_bolt_spec_fix : public SpellScriptLoader
{
public:
    spell_warl_glyph_of_shadow_bolt_spec_fix() : SpellScriptLoader("spell_warl_glyph_of_shadow_bolt_spec_fix") { }

    class spell_warl_glyph_of_shadow_bolt_spec_fix_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_glyph_of_shadow_bolt_spec_fix_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* warlock = GetTarget()->ToPlayer())
                if (AuraEffect const* glyph = warlock->GetAuraEffect(SPELL_WARLOCK_GLYPH_OF_SHADOW_BOLT, EFFECT_0))
                    warlock->learnSpell(glyph->GetAmount(), true);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* warlock = GetTarget()->ToPlayer())
                if (SpellInfo const* glyph = sSpellMgr->GetSpellInfo(SPELL_WARLOCK_GLYPH_OF_SHADOW_BOLT))
                    warlock->removeSpell(glyph->Effects[EFFECT_0].BasePoints);
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_warl_glyph_of_shadow_bolt_spec_fix_AuraScript::HandleApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_warl_glyph_of_shadow_bolt_spec_fix_AuraScript::HandleRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_glyph_of_shadow_bolt_spec_fix_AuraScript();
    }
};

// 112092 - Shadow Bolt (Glyph of Shadow Bolt)
class spell_warl_shadow_bolt_glyphed : public SpellScriptLoader
{
public:
    spell_warl_shadow_bolt_glyphed() : SpellScriptLoader("spell_warl_shadow_bolt_glyphed") { }

    class spell_warl_shadow_bolt_glyphed_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_shadow_bolt_glyphed_SpellScript);

        bool Load() OVERRIDE
        {
            _damage = 0;
            return true;
        }

        // The regular Shadow Bolt damage is split between three bolts
        void CalculateDamage(SpellEffIndex /*effIndex*/)
        {
            SpellInfo const* shadowBolt = sSpellMgr->GetSpellInfo(SPELL_WARLOCK_SHADOW_BOLT);
            if (!shadowBolt)
                return;

            Unit* caster = GetCaster();
            _damage = shadowBolt->Effects[EFFECT_0].CalcValue(caster) + int32(shadowBolt->Effects[EFFECT_2].BonusMultiplier * caster->SpellBaseDamageBonusDone(shadowBolt->GetSchoolMask()));
            _damage /= 3;
            SetEffectValue(_damage);
        }

        void HandleTrigger(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            if (Unit* target = GetHitUnit())
                GetCaster()->CastCustomSpell(GetSpellInfo()->Effects[effIndex].TriggerSpell, SPELLVALUE_BASE_POINT0, _damage, target, true);
        }

        void Register() OVERRIDE
        {
            OnEffectLaunchTarget += SpellEffectFn(spell_warl_shadow_bolt_glyphed_SpellScript::CalculateDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            OnEffectLaunchTarget += SpellEffectFn(spell_warl_shadow_bolt_glyphed_SpellScript::HandleTrigger, EFFECT_1, SPELL_EFFECT_TRIGGER_SPELL);
            OnEffectLaunchTarget += SpellEffectFn(spell_warl_shadow_bolt_glyphed_SpellScript::HandleTrigger, EFFECT_2, SPELL_EFFECT_TRIGGER_SPELL);
        }

        int32 _damage;
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_shadow_bolt_glyphed_SpellScript();
    }
};

// 18540 - Summon Doomguard, 112927 - Summon Terrorguard
class spell_warl_summon_doomguard : public SpellScriptLoader
{
public:
    spell_warl_summon_doomguard() : SpellScriptLoader("spell_warl_summon_doomguard") { }

    class spell_warl_summon_doomguard_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_summon_doomguard_SpellScript);

        void HandleAfterCast()
        {
            Unit* target = GetExplTargetUnit();
            if (!target)
                return;

            SpellInfo const* summonSpell = sSpellMgr->GetSpellInfo(GetSpellInfo()->Effects[EFFECT_1].TriggerSpell);
            if (!summonSpell)
                return;

            std::list<Creature*> summons;
            GetCaster()->GetAllMinionsByEntry(summons, uint32(summonSpell->Effects[EFFECT_0].MiscValue));
            for (std::list<Creature*>::const_iterator itr = summons.begin(); itr != summons.end(); ++itr)
                if ((*itr)->IsAIEnabled)
                    (*itr)->AI()->AttackStart(target);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_warl_summon_doomguard_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_summon_doomguard_SpellScript();
    }
};

// 85692 - Doom Bolt
class spell_warl_doombolt : public SpellScriptLoader
{
public:
    spell_warl_doombolt() : SpellScriptLoader("spell_warl_doombolt") { }

    class spell_warl_doombolt_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_doombolt_SpellScript);

        void HandleHit()
        {
            Unit* target = GetHitUnit();
            if (target && target->HealthBelowPct(GetSpellInfo()->Effects[EFFECT_1].CalcValue()))
                SetHitDamage(int32(GetHitDamage() * 1.2f));
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_warl_doombolt_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_doombolt_SpellScript();
    }
};

// 123746 - Fel Energy
class spell_warl_fel_energy : public SpellScriptLoader
{
public:
    spell_warl_fel_energy() : SpellScriptLoader("spell_warl_fel_energy") { }

    class spell_warl_fel_energy_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_fel_energy_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* pet = GetTarget();
            pet->setPowerType(POWER_ENERGY);
            pet->SetPower(POWER_ENERGY, pet->GetCreatePowers(POWER_ENERGY));
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_warl_fel_energy_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_402, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_fel_energy_AuraScript();
    }
};

// 103967 - Carrion Swarm
class spell_warl_glyph_of_carrion_swarm : public SpellScriptLoader
{
public:
    spell_warl_glyph_of_carrion_swarm() : SpellScriptLoader("spell_warl_glyph_of_carrion_swarm") { }

    class spell_warl_glyph_of_carrion_swarm_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_glyph_of_carrion_swarm_SpellScript);

        // The knock back is disabled by the glyph stored as this effect's trigger spell
        void HandleKnockBack(SpellEffIndex effIndex)
        {
            if (GetCaster()->HasAura(GetSpellInfo()->Effects[effIndex].TriggerSpell))
                PreventHitDefaultEffect(effIndex);
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_glyph_of_carrion_swarm_SpellScript::HandleKnockBack, EFFECT_1, SPELL_EFFECT_KNOCK_BACK);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_glyph_of_carrion_swarm_SpellScript();
    }
};

// Malefic Grasp and Drain Soul make the caster's other damage over time effects on the target
// instantly deal a percentage of a tick
static void CastInstantDotTicks(Unit* caster, Unit* target, int32 damagePct)
{
    if (!caster || !target || damagePct <= 0)
        return;

    int32 mastery = 0;
    if (AuraEffect const* masteryEff = caster->GetAuraEffect(SPELL_WARLOCK_MASTERY_POTENT_AFFLICTIONS, EFFECT_0))
        mastery = masteryEff->GetAmount();

    std::list<std::pair<uint32, int32> > toCast;
    Unit::AuraEffectList const& periodic = target->GetAuraEffectsByType(SPELL_AURA_PERIODIC_DAMAGE);
    for (Unit::AuraEffectList::const_iterator itr = periodic.begin(); itr != periodic.end(); ++itr)
    {
        AuraEffect const* dot = *itr;
        if (dot->GetCasterGUID() != caster->GetGUID())
            continue;

        uint32 spellId = 0;
        switch (dot->GetId())
        {
            case SPELL_WARLOCK_AGONY:               spellId = SPELL_WARLOCK_AGONY_INSTANT;               break;
            case SPELL_WARLOCK_CORRUPTION_DOT:      spellId = SPELL_WARLOCK_CORRUPTION_INSTANT;          break;
            case SPELL_WARLOCK_UNSTABLE_AFFLICTION: spellId = SPELL_WARLOCK_UNSTABLE_AFFLICTION_INSTANT; break;
            case SPELL_WARLOCK_SEED_OF_CORRUPTION:  spellId = SPELL_WARLOCK_SEED_OF_CORRUPTION_INSTANT;  break;
            default:
                break;
        }

        if (!spellId)
            continue;

        int32 tick = int32(caster->SpellDamageBonusDone(target, dot->GetSpellInfo(), uint32(std::max(dot->GetAmount(), 0)), DOT, dot->GetBase()->GetStackAmount()));
        toCast.push_back(std::make_pair(spellId, tick));
    }

    for (std::list<std::pair<uint32, int32> >::const_iterator itr = toCast.begin(); itr != toCast.end(); ++itr)
    {
        // Mastery: Potent Afflictions also applies to the instant damage spell
        int32 damage = int32(CalculatePct(float(itr->second), damagePct) * 100.0f / (100.0f + mastery));
        if (damage > 0)
            caster->CastCustomSpell(itr->first, SPELLVALUE_BASE_POINT0, damage, target, true);
    }
}

// 103103 - Malefic Grasp
class spell_warl_malefic_grasp : public SpellScriptLoader
{
public:
    spell_warl_malefic_grasp() : SpellScriptLoader("spell_warl_malefic_grasp") { }

    class spell_warl_malefic_grasp_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_malefic_grasp_AuraScript);

        void HandleTick(AuraEffect const* /*aurEff*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            caster->SendPlaySpellVisual(25955, GetTarget()->GetGUID(), 20.0f);
            if (AuraEffect const* dummy = GetEffect(EFFECT_2))
                CastInstantDotTicks(caster, GetTarget(), dummy->GetAmount());
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_malefic_grasp_AuraScript::HandleTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_malefic_grasp_AuraScript();
    }
};

// 1120 - Drain Soul
class spell_warl_drain_soul : public SpellScriptLoader
{
public:
    spell_warl_drain_soul() : SpellScriptLoader("spell_warl_drain_soul") { }

    class spell_warl_drain_soul_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_drain_soul_AuraScript);

        // Below 20% health Drain Soul deals double damage and triggers the other damage over time effects
        void CalculateTick(AuraEffect const* /*aurEff*/, int32& damage)
        {
            if (GetTarget()->HasAuraState(AURA_STATE_HEALTHLESS_20_PERCENT))
                damage *= 2;
        }

        void HandleTick(AuraEffect const* /*aurEff*/)
        {
            if (!GetTarget()->HasAuraState(AURA_STATE_HEALTHLESS_20_PERCENT))
                return;

            if (AuraEffect const* dummy = GetEffect(EFFECT_4))
                CastInstantDotTicks(GetCaster(), GetTarget(), dummy->GetAmount());
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_DEATH)
                return;

            if (Unit* caster = GetCaster())
                caster->CastSpell(caster, SPELL_WARLOCK_SOULSHARD_ENERGIZE, true);
        }

        void Register() OVERRIDE
        {
            DoEffectCalcTickAmount += AuraEffectCalcTickAmountFn(spell_warl_drain_soul_AuraScript::CalculateTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_drain_soul_AuraScript::HandleTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            OnEffectRemove += AuraEffectRemoveFn(spell_warl_drain_soul_AuraScript::HandleRemove, EFFECT_4, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_drain_soul_AuraScript();
    }
};

// 980 - Agony
class spell_warl_agony : public SpellScriptLoader
{
public:
    spell_warl_agony() : SpellScriptLoader("spell_warl_agony") { }

    class spell_warl_agony_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_agony_AuraScript);

        // Every tick adds a stack, increasing the damage of the following ticks
        void HandleTick(AuraEffect const* /*aurEff*/)
        {
            if (GetStackAmount() < GetSpellInfo()->StackAmount)
                SetStackAmount(GetStackAmount() + 1);
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_agony_AuraScript::HandleTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_agony_AuraScript();
    }
};

// 131737 - Agony (instant tick)
class spell_warl_agony_instant_tick : public SpellScriptLoader
{
public:
    spell_warl_agony_instant_tick() : SpellScriptLoader("spell_warl_agony_instant_tick") { }

    class spell_warl_agony_instant_tick_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_agony_instant_tick_SpellScript);

        void HandleHit()
        {
            Unit* target = GetHitUnit();
            if (!target)
                return;

            if (Aura* agony = target->GetAura(SPELL_WARLOCK_AGONY, GetCaster()->GetGUID()))
                if (agony->GetStackAmount() < agony->GetSpellInfo()->StackAmount)
                    agony->SetStackAmount(agony->GetStackAmount() + 1);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_warl_agony_instant_tick_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_agony_instant_tick_SpellScript();
    }
};

// 48181 - Haunt
class spell_warl_haunt : public SpellScriptLoader
{
public:
    spell_warl_haunt() : SpellScriptLoader("spell_warl_haunt") { }

    class spell_warl_haunt_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_haunt_AuraScript);

        // Dispelling Haunt refunds its Soul Shard
        void HandleDispel(DispelInfo* /*dispelInfo*/)
        {
            if (Unit* caster = GetCaster())
                caster->CastSpell(caster, SPELL_WARLOCK_DARK_REFUND, true);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
                if (AuraEffect const* bonus = caster->GetAuraEffect(SPELL_WARLOCK_T16_4P_BONUS, EFFECT_0))
                    if (roll_chance_i(bonus->GetAmount()))
                        caster->CastSpell(caster, SPELL_WARLOCK_DARK_REFUND, true);
        }

        void Register() OVERRIDE
        {
            AfterDispel += AuraDispelFn(spell_warl_haunt_AuraScript::HandleDispel);
            OnEffectRemove += AuraEffectRemoveFn(spell_warl_haunt_AuraScript::HandleRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_haunt_AuraScript();
    }
};

// 119678 - Soul Swap (Soulburn)
class spell_warl_soul_swap_soulburn : public SpellScriptLoader
{
public:
    spell_warl_soul_swap_soulburn() : SpellScriptLoader("spell_warl_soul_swap_soulburn") { }

    class spell_warl_soul_swap_soulburn_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_soul_swap_soulburn_SpellScript);

        void HandleAfterHit()
        {
            Unit* warlock = GetCaster();
            Unit* target = GetHitUnit();
            if (!target)
                return;

            warlock->AddAura(SPELL_WARLOCK_CORRUPTION_DOT, target);
            warlock->AddAura(SPELL_WARLOCK_UNSTABLE_AFFLICTION, target);
            warlock->AddAura(SPELL_WARLOCK_AGONY, target);
            warlock->RemoveAurasDueToSpell(SPELL_WARLOCK_SOULBURN);
        }

        void Register() OVERRIDE
        {
            AfterHit += SpellHitFn(spell_warl_soul_swap_soulburn_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_soul_swap_soulburn_SpellScript();
    }
};

// 56218 - Glyph of Siphon Life
class spell_warl_glyph_of_siphon_life : public SpellScriptLoader
{
public:
    spell_warl_glyph_of_siphon_life() : SpellScriptLoader("spell_warl_glyph_of_siphon_life") { }

    class spell_warl_glyph_of_siphon_life_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_glyph_of_siphon_life_AuraScript);

        void HandleProc(ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_WARLOCK_SIPHON_LIFE_HEAL, true);
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_warl_glyph_of_siphon_life_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_glyph_of_siphon_life_AuraScript();
    }
};

// 63106 - Siphon Life
class spell_warl_glyph_of_siphon_life_heal : public SpellScriptLoader
{
public:
    spell_warl_glyph_of_siphon_life_heal() : SpellScriptLoader("spell_warl_glyph_of_siphon_life_heal") { }

    class spell_warl_glyph_of_siphon_life_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_glyph_of_siphon_life_heal_SpellScript);

        // The base points hold 50 for a 0.5% maximum health heal
        void HandleHit()
        {
            SetHitHeal(GetHitHeal() / 100);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_warl_glyph_of_siphon_life_heal_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_glyph_of_siphon_life_heal_SpellScript();
    }
};

// 755 - Health Funnel
class spell_warl_health_funnel : public SpellScriptLoader
{
public:
    spell_warl_health_funnel() : SpellScriptLoader("spell_warl_health_funnel") { }

    class spell_warl_health_funnel_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_health_funnel_AuraScript);

        // Each tick costs the warlock a percentage of maximum health stored in effect 2
        void HandleTick(AuraEffect const* /*aurEff*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            AuraEffect const* dummy = caster->GetAuraEffect(GetId(), EFFECT_2);
            if (!dummy)
                return;

            uint32 damage = caster->CountPctFromMaxHealth(dummy->GetAmount());
            uint32 absorb = 0;
            caster->DealDamageMods(caster, damage, &absorb);
            caster->SendSpellNonMeleeDamageLog(caster, GetId(), damage, GetSpellInfo()->GetSchoolMask(), absorb, 0, false, 0, false);

            CleanDamage cleanDamage = CleanDamage(0, 0, WeaponAttackType::BASE_ATTACK, MeleeHitOutcome::MELEE_HIT_NORMAL);
            caster->DealDamage(caster, damage, &cleanDamage, NODAMAGE, GetSpellInfo()->GetSchoolMask(), GetSpellInfo(), true);
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_health_funnel_AuraScript::HandleTick, EFFECT_0, SPELL_AURA_OBS_MOD_HEALTH);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_health_funnel_AuraScript();
    }
};

// 755, 114189 - Health Funnel
class spell_warl_health_funnel_soulburn : public SpellScriptLoader
{
public:
    spell_warl_health_funnel_soulburn() : SpellScriptLoader("spell_warl_health_funnel_soulburn") { }

    class spell_warl_health_funnel_soulburn_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_health_funnel_soulburn_SpellScript);

        void HandleHit()
        {
            Unit* caster = GetCaster();
            if (caster->HasAura(SPELL_WARLOCK_SOULBURN) && caster->HasSpell(SPELL_WARLOCK_SOULBURN_HEALTH_FUNNEL))
            {
                caster->CastSpell(caster, SPELL_WARLOCK_HEALTH_FUNNEL_SOULBURN, true);
                caster->RemoveAurasDueToSpell(SPELL_WARLOCK_SOULBURN);
            }
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_warl_health_funnel_soulburn_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_health_funnel_soulburn_SpellScript();
    }
};

// 74434 - Soulburn
class spell_warl_soulburn_override : public SpellScriptLoader
{
public:
    spell_warl_soulburn_override() : SpellScriptLoader("spell_warl_soulburn_override") { }

    class spell_warl_soulburn_override_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_soulburn_override_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* warlock = GetTarget();
            warlock->CastSpell(warlock, SPELL_WARLOCK_SOULBURN_OVERRIDE_BASE, true);
            if (warlock->HasSpell(SPELL_WARLOCK_SOULBURN_CURSE))
            {
                warlock->CastSpell(warlock, SPELL_WARLOCK_SOULBURN_OVERRIDE_CURSE1, true);
                warlock->CastSpell(warlock, SPELL_WARLOCK_SOULBURN_OVERRIDE_CURSE2, true);
            }
            if (warlock->HasSpell(SPELL_WARLOCK_SOULBURN_SEED_OF_CORRUPTION))
                warlock->CastSpell(warlock, SPELL_WARLOCK_SOULBURN_OVERRIDE_SEED, true);
            if (warlock->HasSpell(SPELL_WARLOCK_SOULBURN_DEMONIC_CIRCLE_TELEPORT))
                warlock->CastSpell(warlock, SPELL_WARLOCK_SOULBURN_OVERRIDE_TELEPORT, true);
            if (warlock->HasSpell(SPELL_WARLOCK_SOULBURN_SOUL_SWAP))
                warlock->CastSpell(warlock, SPELL_WARLOCK_SOULBURN_OVERRIDE_SOUL_SWAP, true);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* warlock = GetTarget();
            warlock->RemoveAurasDueToSpell(SPELL_WARLOCK_SOULBURN_OVERRIDE_BASE);
            warlock->RemoveAurasDueToSpell(SPELL_WARLOCK_SOULBURN_OVERRIDE_SEED);
            warlock->RemoveAurasDueToSpell(SPELL_WARLOCK_SOULBURN_OVERRIDE_CURSE1);
            warlock->RemoveAurasDueToSpell(SPELL_WARLOCK_SOULBURN_OVERRIDE_CURSE2);
            warlock->RemoveAurasDueToSpell(SPELL_WARLOCK_SOULBURN_OVERRIDE_TELEPORT);
            warlock->RemoveAurasDueToSpell(SPELL_WARLOCK_SOULBURN_OVERRIDE_SOUL_SWAP);
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_warl_soulburn_override_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_warl_soulburn_override_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_soulburn_override_AuraScript();
    }
};

// 104223 - Curse of Exhaustion, 104225 - Curse of the Elements, 109468 - Curse of Enfeeblement,
// 114790 - Seed of Corruption, 114794 - Demonic Circle: Teleport (Soulburn versions)
class spell_soulburn_consumer : public SpellScriptLoader
{
public:
    spell_soulburn_consumer() : SpellScriptLoader("spell_soulburn_consumer") { }

    class spell_soulburn_consumer_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_soulburn_consumer_SpellScript);

        void HandleCast()
        {
            GetCaster()->RemoveAurasDueToSpell(SPELL_WARLOCK_SOULBURN);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_soulburn_consumer_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_soulburn_consumer_SpellScript();
    }
};

// 104223 - Curse of Exhaustion (Soulburn)
class spell_warl_curse_of_exhaustion_soulburn : public SpellScriptLoader
{
public:
    spell_warl_curse_of_exhaustion_soulburn() : SpellScriptLoader("spell_warl_curse_of_exhaustion_soulburn") { }

    class spell_warl_curse_of_exhaustion_soulburn_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_curse_of_exhaustion_soulburn_SpellScript);

        // The Soulburn version shares the cooldown of the regular curse
        void HandleAfterCast()
        {
            if (Player* warlock = GetCaster()->ToPlayer())
                if (SpellInfo const* curse = sSpellMgr->GetSpellInfo(SPELL_WARLOCK_CURSE_OF_EXHAUSTION))
                    warlock->SendCooldownEvent(curse);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_warl_curse_of_exhaustion_soulburn_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_curse_of_exhaustion_soulburn_SpellScript();
    }
};

// 5697 - Unending Breath
class spell_warl_unending_breath_soulburn : public SpellScriptLoader
{
public:
    spell_warl_unending_breath_soulburn() : SpellScriptLoader("spell_warl_unending_breath_soulburn") { }

    class spell_warl_unending_breath_soulburn_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_unending_breath_soulburn_SpellScript);

        void HandleCast()
        {
            Unit* caster = GetCaster();
            if (Aura* soulburn = caster->GetAura(SPELL_WARLOCK_SOULBURN))
            {
                caster->CastSpell(caster, SPELL_WARLOCK_UNENDING_BREATH_WATERWALK, true);
                caster->RemoveAura(soulburn);
            }
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_warl_unending_breath_soulburn_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_unending_breath_soulburn_SpellScript();
    }
};

// 29893 - Create Soulwell
class spell_warl_glyph_of_soulwell : public SpellScriptLoader
{
public:
    spell_warl_glyph_of_soulwell() : SpellScriptLoader("spell_warl_glyph_of_soulwell") { }

    class spell_warl_glyph_of_soulwell_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_glyph_of_soulwell_SpellScript);

        void HandleAfterCast()
        {
            WorldLocation const* dest = GetExplTargetDest();
            if (!dest || !GetCaster()->HasAura(SPELL_WARLOCK_GLYPH_OF_SOULWELL))
                return;

            GetCaster()->CastSpell(dest->GetPositionX(), dest->GetPositionY(), dest->GetPositionZ(), SPELL_WARLOCK_GLYPH_OF_SOULWELL_VISUAL, true);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_warl_glyph_of_soulwell_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_glyph_of_soulwell_SpellScript();
    }
};

// 108482 - Unbound Will
class spell_warl_unbound_will : public SpellScriptLoader
{
public:
    spell_warl_unbound_will() : SpellScriptLoader("spell_warl_unbound_will") { }

    class spell_warl_unbound_will_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_unbound_will_SpellScript);

        SpellCastResult CheckHealth()
        {
            if (GetCaster()->GetHealthPct() <= 20.0f)
            {
                SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_NOT_ENOUGH_HEALTH);
                return SpellCastResult::SPELL_FAILED_CUSTOM_ERROR;
            }
            return SpellCastResult::SPELL_CAST_OK;
        }

        void HandleCast()
        {
            Player* warlock = GetCaster()->ToPlayer();
            if (!warlock)
                return;

            warlock->ModifyHealth(-int32(warlock->CountPctFromMaxHealth(20)));
            warlock->RemoveAurasWithMechanic(IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK);

            Pet* pet = warlock->GetPet();
            if (!pet)
                return;

            pet->RemoveAurasWithMechanic(IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK);

            DispelChargesList dispelList;
            pet->GetDispellableAuraList(warlock, SpellInfo::GetDispelMask(DISPEL_MAGIC), dispelList);
            for (DispelChargesList::const_iterator itr = dispelList.begin(); itr != dispelList.end(); ++itr)
                if (int32 chance = itr->first->CalcDispelChance(pet, false))
                    if (roll_chance_i(chance))
                        pet->RemoveAurasDueToSpellByDispel(itr->first->GetId(), GetSpellInfo()->Id, itr->first->GetCasterGUID(), warlock, itr->second);
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_warl_unbound_will_SpellScript::CheckHealth);
            OnCast += SpellCastFn(spell_warl_unbound_will_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_unbound_will_SpellScript();
    }
};

// 108446 - Soul Link
class spell_warl_soul_link : public SpellScriptLoader
{
public:
    spell_warl_soul_link() : SpellScriptLoader("spell_warl_soul_link") { }

    class spell_warl_soul_link_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_soul_link_AuraScript);

        bool Load() OVERRIDE
        {
            _healAmount = 0;
            return true;
        }

        // Healing done is collected and shared with the warlock on the next periodic tick
        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            if (eventInfo.GetDamageInfo())
                _healAmount += CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount());
        }

        void HandleTick(AuraEffect const* aurEff)
        {
            Unit* warlock = GetUnitOwner()->GetOwner();
            if (!warlock || !_healAmount)
                return;

            CustomSpellValues values;
            values.AddSpellMod(SPELLVALUE_BASE_POINT0, int32(_healAmount));
            values.AddSpellMod(SPELLVALUE_BASE_POINT1, int32(_healAmount));
            warlock->CastCustomSpell(SPELL_WARLOCK_SOUL_LINK_HEAL, values, warlock, TRIGGERED_FULL_MASK, NULL, aurEff);
            _healAmount = 0;
        }

        void Register() OVERRIDE
        {
            OnEffectProc += AuraEffectProcFn(spell_warl_soul_link_AuraScript::HandleProc, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_soul_link_AuraScript::HandleTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        }

        uint32 _healAmount;
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_soul_link_AuraScript();
    }
};

// 108370 - Soul Leech
class spell_warl_soul_leech : public SpellScriptLoader
{
public:
    spell_warl_soul_leech() : SpellScriptLoader("spell_warl_soul_leech") { }

    class spell_warl_soul_leech_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_soul_leech_AuraScript);

        void HandleProc(ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            Player* warlock = GetUnitOwner()->ToPlayer();
            if (!warlock || !eventInfo.GetDamageInfo())
                return;

            uint32 absorbPct = warlock->GetTalentSpecialization(warlock->GetActiveSpec()) == SPEC_WARLOCK_AFFLICTION ? 20 : 10;
            int32 absorb = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), absorbPct));
            if (!absorb)
                return;

            // The shield stacks with itself but is capped at 15% of the owner's maximum health
            if (AuraEffect const* soulLeech = warlock->GetAuraEffect(SPELL_WARLOCK_SOUL_LEECH_ABSORB, EFFECT_0))
                absorb += soulLeech->GetAmount();

            absorb = std::min(absorb, int32(warlock->CountPctFromMaxHealth(15)));
            warlock->CastCustomSpell(SPELL_WARLOCK_SOUL_LEECH_ABSORB, SPELLVALUE_BASE_POINT0, absorb, warlock, true);

            if (Pet* pet = warlock->GetPet())
            {
                absorb = std::min(absorb, int32(pet->CountPctFromMaxHealth(15)));
                warlock->CastCustomSpell(SPELL_WARLOCK_SOUL_LEECH_ABSORB, SPELLVALUE_BASE_POINT0, absorb, pet, true);
            }
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_warl_soul_leech_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_soul_leech_AuraScript();
    }
};

// 110913 - Dark Bargain
class spell_warl_dark_bargain_on_absorb : public SpellScriptLoader
{
public:
    spell_warl_dark_bargain_on_absorb() : SpellScriptLoader("spell_warl_dark_bargain_on_absorb") { }

    class spell_warl_dark_bargain_on_absorb_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_dark_bargain_on_absorb_AuraScript);

        bool Load() OVERRIDE
        {
            _absorbed = 0;
            return true;
        }

        // All damage is absorbed while the shield lasts
        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            amount = -1;
        }

        void Absorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
        {
            absorbAmount = dmgInfo.GetDamage();
            _absorbed += absorbAmount;
        }

        // When it fades, a share of the absorbed damage (effect 1) is dealt over time
        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            SpellInfo const* dot = sSpellMgr->GetSpellInfo(SPELL_WARLOCK_DARK_BARGAIN_DOT);
            AuraEffect const* pctEff = GetEffect(EFFECT_1);
            if (!dot || !pctEff || !_absorbed)
                return;

            uint32 ticks = std::max<uint32>(dot->GetMaxTicks(), 1);
            int32 damage = int32(CalculatePct(uint64(_absorbed), pctEff->GetAmount()) / ticks);
            if (damage > 0)
                GetTarget()->CastCustomSpell(SPELL_WARLOCK_DARK_BARGAIN_DOT, SPELLVALUE_BASE_POINT0, damage, GetTarget(), true);
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_dark_bargain_on_absorb_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            OnEffectAbsorb += AuraEffectAbsorbFn(spell_warl_dark_bargain_on_absorb_AuraScript::Absorb, EFFECT_0);
            AfterEffectRemove += AuraEffectRemoveFn(spell_warl_dark_bargain_on_absorb_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
        }

        uint32 _absorbed;
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_dark_bargain_on_absorb_AuraScript();
    }
};

// 108359 - Dark Regeneration
class spell_warl_dark_regeneration : public SpellScriptLoader
{
public:
    spell_warl_dark_regeneration() : SpellScriptLoader("spell_warl_dark_regeneration") { }

    class spell_warl_dark_regeneration_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_dark_regeneration_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Guardian* pet = GetTarget()->GetGuardianPet())
                pet->CastSpell(pet, SPELL_WARLOCK_DARK_REGENERATION, true);
        }

        void CalculateTick(AuraEffect const* /*aurEff*/, int32& heal)
        {
            if (AuraEffect const* felArmor = GetTarget()->GetAuraEffect(SPELL_WARLOCK_FEL_ARMOR, EFFECT_2))
                AddPct(heal, felArmor->GetAmount());
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_warl_dark_regeneration_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_OBS_MOD_HEALTH, AURA_EFFECT_HANDLE_REAL);
            DoEffectCalcTickAmount += AuraEffectCalcTickAmountFn(spell_warl_dark_regeneration_AuraScript::CalculateTick, EFFECT_0, SPELL_AURA_OBS_MOD_HEALTH);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_dark_regeneration_AuraScript();
    }
};

// 108416 - Sacrificial Pact
class spell_warl_sacrificial_pact : public SpellScriptLoader
{
public:
    spell_warl_sacrificial_pact() : SpellScriptLoader("spell_warl_sacrificial_pact") { }

    class spell_warl_sacrificial_pact_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_sacrificial_pact_AuraScript);

        // Sacrifices a share (effect 1) of the demon's current health, or the warlock's without a demon
        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            Unit* victim = caster->GetGuardianPet();
            if (!victim)
                victim = caster;

            int32 sacrificed = int32(victim->CountPctFromCurHealth(GetSpellInfo()->Effects[EFFECT_1].CalcValue(caster)));
            victim->ModifyHealth(-sacrificed);
            amount = CalculatePct(sacrificed, amount);
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_sacrificial_pact_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_sacrificial_pact_AuraScript();
    }
};

// 6229 - Twilight Ward, 104048 - Twilight Ward (Metamorphosis)
class spell_warl_twilight_ward_s12 : public SpellScriptLoader
{
public:
    spell_warl_twilight_ward_s12() : SpellScriptLoader("spell_warl_twilight_ward_s12") { }

    class spell_warl_twilight_ward_s12_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_twilight_ward_s12_SpellScript);

        // The PvP set bonus replaces the ward with an improved version
        void HandleHit()
        {
            Unit* caster = GetCaster();
            if (!caster->HasAura(SPELL_WARLOCK_PVP_SET_2P_BONUS))
                return;

            uint32 spellId = GetSpellInfo()->Id == 6229 ? SPELL_WARLOCK_TWILIGHT_WARD_S12 : SPELL_WARLOCK_TWILIGHT_WARD_METAMORPHOSIS_S12;
            caster->RemoveAurasDueToSpell(GetSpellInfo()->Id);
            caster->CastSpell(caster, spellId, true);
        }

        void Register() OVERRIDE
        {
            AfterHit += SpellHitFn(spell_warl_twilight_ward_s12_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_twilight_ward_s12_SpellScript();
    }
};

// 111400 - Burning Rush
class spell_warl_burning_rush : public SpellScriptLoader
{
public:
    spell_warl_burning_rush() : SpellScriptLoader("spell_warl_burning_rush") { }

    class spell_warl_burning_rush_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_burning_rush_AuraScript);

        void HandleTick(AuraEffect const* /*aurEff*/)
        {
            Unit* warlock = GetUnitOwner();
            if (warlock->GetHealthPct() <= 4.0f)
            {
                Remove();
                return;
            }

            uint32 damage = warlock->CountPctFromMaxHealth(4);
            warlock->DealDamage(warlock, damage, NULL, NODAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_burning_rush_AuraScript::HandleTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_burning_rush_AuraScript();
    }
};

// 101976 - Soul Harvest
class spell_warl_soul_harvest : public SpellScriptLoader
{
public:
    spell_warl_soul_harvest() : SpellScriptLoader("spell_warl_soul_harvest") { }

    class spell_warl_soul_harvest_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_soul_harvest_AuraScript);

        bool Load() OVERRIDE
        {
            _timer = 0;
            return true;
        }

        // Out of combat the warlock and the demon regenerate 2% of maximum health per second
        void HandleUpdate(uint32 diff)
        {
            _timer += diff;
            if (_timer < 1000)
                return;

            _timer = 0;

            Player* player = GetUnitOwner()->ToPlayer();
            if (!player || player->IsInCombat() || player->InArena() || !player->IsAlive())
                return;

            player->ModifyHealth(int32(player->GetMaxHealth() / 50));
            if (Pet* pet = player->GetPet())
                if (pet->IsAlive())
                    pet->ModifyHealth(int32(pet->GetMaxHealth() / 50));
        }

        void Register() OVERRIDE
        {
            OnAuraUpdate += AuraUpdateFn(spell_warl_soul_harvest_AuraScript::HandleUpdate);
        }

        uint32 _timer;
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_soul_harvest_AuraScript();
    }
};

// 5782 - Fear
class spell_warl_fear : public SpellScriptLoader
{
public:
    spell_warl_fear() : SpellScriptLoader("spell_warl_fear") { }

    class spell_warl_fear_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_fear_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* target = GetHitUnit();
            if (!target)
                return;

            Unit* caster = GetCaster();
            caster->CastSpell(target, caster->HasAura(SPELL_WARLOCK_GLYPH_OF_FEAR) ? SPELL_WARLOCK_GLYPH_OF_FEAR_EFFECT : SPELL_WARLOCK_FEAR_EFFECT, true);
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_fear_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_fear_SpellScript();
    }
};

// 111397 - Blood Horror
class spell_warl_blood_horror : public SpellScriptLoader
{
public:
    spell_warl_blood_horror() : SpellScriptLoader("spell_warl_blood_horror") { }

    class spell_warl_blood_horror_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_blood_horror_SpellScript);

        SpellCastResult CheckHealth()
        {
            if (GetCaster()->GetHealthPct() < 5.0f)
                return SpellCastResult::SPELL_FAILED_FIZZLE;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void HandleHit()
        {
            GetCaster()->ModifyHealth(-int32(GetCaster()->CountPctFromMaxHealth(5)));
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_warl_blood_horror_SpellScript::CheckHealth);
            OnHit += SpellHitFn(spell_warl_blood_horror_SpellScript::HandleHit);
        }
    };

    class spell_warl_blood_horror_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_blood_horror_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (!eventInfo.GetDamageInfo() || !eventInfo.GetDamageInfo()->GetDamage())
                return false;

            Unit* actor = eventInfo.GetActor();
            return actor && (!actor->IsSummon() || !actor->GetCharmerOrOwnerPlayerOrPlayerItself());
        }

        void HandleProc(ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            GetUnitOwner()->CastSpell(eventInfo.GetActor(), SPELL_WARLOCK_BLOOD_HORROR_FEAR, true);
            DropCharge();
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_warl_blood_horror_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_warl_blood_horror_AuraScript::HandleProc);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_blood_horror_SpellScript();
    }

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_blood_horror_AuraScript();
    }
};

// 1454 - Life Tap
class spell_warl_life_tap_heal_absorb : public SpellScriptLoader
{
public:
    spell_warl_life_tap_heal_absorb() : SpellScriptLoader("spell_warl_life_tap_heal_absorb") { }

    class spell_warl_life_tap_heal_absorb_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_life_tap_heal_absorb_AuraScript);

        // The heal absorb is a percentage of maximum health and stacks with the remaining one
        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            amount = int32(GetUnitOwner()->CountPctFromMaxHealth(amount));
            if (AuraEffect const* existing = GetUnitOwner()->GetAuraEffect(GetId(), EFFECT_2))
                amount += existing->GetAmount();
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_life_tap_heal_absorb_AuraScript::CalculateAmount, EFFECT_2, SPELL_AURA_SCHOOL_HEAL_ABSORB);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_life_tap_heal_absorb_AuraScript();
    }
};

// 20707 - Soulstone
class spell_warl_soulstone : public SpellScriptLoader
{
public:
    spell_warl_soulstone() : SpellScriptLoader("spell_warl_soulstone") { }

    class spell_warl_soulstone_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_soulstone_SpellScript);

        // Living targets get the soulstone buff, dead ones are resurrected
        void HandleAura(SpellEffIndex effIndex)
        {
            if (Unit* target = GetHitUnit())
                if (!target->IsAlive())
                    PreventHitDefaultEffect(effIndex);
        }

        void HandleResurrect(SpellEffIndex effIndex)
        {
            if (Unit* target = GetHitUnit())
                if (target->IsAlive())
                    PreventHitDefaultEffect(effIndex);
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_soulstone_SpellScript::HandleAura, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            OnEffectHitTarget += SpellEffectFn(spell_warl_soulstone_SpellScript::HandleResurrect, EFFECT_1, SPELL_EFFECT_TRIGGER_SPELL);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_soulstone_SpellScript();
    }
};

// 6203 - Soulstone
class spell_warl_soulstone_resurrect : public SpellScriptLoader
{
public:
    spell_warl_soulstone_resurrect() : SpellScriptLoader("spell_warl_soulstone_resurrect") { }

    class spell_warl_soulstone_resurrect_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_soulstone_resurrect_SpellScript);

        void HandleHit()
        {
            Unit* target = GetHitUnit();
            if (target && !target->IsAlive())
                GetCaster()->CastSpell(target, SPELL_WARLOCK_SOULSTONE_RESURRECT, true);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_warl_soulstone_resurrect_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_soulstone_resurrect_SpellScript();
    }
};

// 58070 - Glyph of Soul Consumption
class spell_warl_glyph_of_soul_consumption : public SpellScriptLoader
{
public:
    spell_warl_glyph_of_soul_consumption() : SpellScriptLoader("spell_warl_glyph_of_soul_consumption") { }

    class spell_warl_glyph_of_soul_consumption_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_glyph_of_soul_consumption_AuraScript);

        bool Load() OVERRIDE
        {
            return GetUnitOwner()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        // Killing blows with the spec's signature ability heal the warlock
        bool CheckProc(ProcEventInfo& eventInfo)
        {
            Unit* target = eventInfo.GetActionTarget();
            if (!eventInfo.GetSpellInfo() || !target || !eventInfo.GetDamageInfo())
                return false;

            if (target->IsAlive() && eventInfo.GetDamageInfo()->GetDamage() < target->GetHealth())
                return false;

            Player* warlock = GetUnitOwner()->ToPlayer();
            if (!warlock->isHonorOrXPTarget(target))
                return false;

            switch (warlock->GetTalentSpecialization(warlock->GetActiveSpec()))
            {
                case SPEC_WARLOCK_AFFLICTION:
                    return warlock->FindCurrentSpellBySpellId(1120) != NULL;
                case SPEC_WARLOCK_DEMONOLOGY:
                    return warlock->GetShapeshiftForm() == FORM_METAMORPHOSIS || warlock->HasAura(SPELL_WARLOCK_DARK_APOTHEOSIS);
                case SPEC_WARLOCK_DESTRUCTION:
                    return eventInfo.GetSpellInfo()->Id == SPELL_WARLOCK_CHAOS_BOLT;
                default:
                    return false;
            }
        }

        void HandleProc(ProcEventInfo& /*eventInfo*/)
        {
            PreventDefaultAction();
            GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_WARLOCK_GLYPH_OF_SOUL_CONSUMPTION, true);
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_warl_glyph_of_soul_consumption_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_warl_glyph_of_soul_consumption_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_glyph_of_soul_consumption_AuraScript();
    }
};

// 145072 - Item - Warlock T16 2P Bonus
class spell_warl_t16_2p_bonus : public SpellScriptLoader
{
public:
    spell_warl_t16_2p_bonus() : SpellScriptLoader("spell_warl_t16_2p_bonus") { }

    class spell_warl_t16_2p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_t16_2p_bonus_AuraScript);

        void HandleProc(ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
            if (!spellInfo)
                return;

            Unit* warlock = GetUnitOwner();
            bool isCrit = (eventInfo.GetHitMask() & PROC_EX_CRITICAL_HIT) != 0;
            int32 chance = 0;
            uint32 spellId = 0;

            switch (spellInfo->Id)
            {
                case SPELL_WARLOCK_CONFLAGRATE:
                case SPELL_WARLOCK_CONFLAGRATE_FNB:
                    if (isCrit)
                    {
                        chance = GetSpellInfo()->Effects[EFFECT_3].CalcValue(warlock);
                        spellId = SPELL_WARLOCK_DESTRUCTIVE_INFLUENCE;
                    }
                    break;
                case SPELL_WARLOCK_UNSTABLE_AFFLICTION:
                    if (isCrit)
                    {
                        chance = GetSpellInfo()->Effects[EFFECT_1].CalcValue(warlock);
                        spellId = SPELL_WARLOCK_EMPOWERED_GRASP;
                    }
                    break;
                case SPELL_WARLOCK_SOUL_FIRE:
                case SPELL_WARLOCK_SOUL_FIRE_METAMORPHOSIS:
                    chance = GetSpellInfo()->Effects[EFFECT_3].CalcValue(warlock);
                    spellId = SPELL_WARLOCK_FIERY_WRATH;
                    break;
                default:
                    break;
            }

            if (spellId && roll_chance_i(chance))
                warlock->CastSpell(warlock, spellId, true);
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_warl_t16_2p_bonus_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_t16_2p_bonus_AuraScript();
    }
};

// 145091 - Item - Warlock T16 4P Bonus
class spell_warl_t16_4p_bonus : public SpellScriptLoader
{
public:
    spell_warl_t16_4p_bonus() : SpellScriptLoader("spell_warl_t16_4p_bonus") { }

    class spell_warl_t16_4p_bonus_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_t16_4p_bonus_AuraScript);

        void HandleProc(ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            if (Unit* target = eventInfo.GetActionTarget())
                GetUnitOwner()->CastSpell(target, SPELL_WARLOCK_HAND_OF_GULDAN_T16, true);
        }

        void Register() OVERRIDE
        {
            OnProc += AuraProcFn(spell_warl_t16_4p_bonus_AuraScript::HandleProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_t16_4p_bonus_AuraScript();
    }
};

// 115232 - Shadow Shield, 115236 - Void Shield
class spell_warl_shadow_shield : public SpellScriptLoader
{
public:
    spell_warl_shadow_shield() : SpellScriptLoader("spell_warl_shadow_shield") { }

    class spell_warl_shadow_shield_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_shadow_shield_SpellScript);

        void HandleAfterHit()
        {
            if (Aura* aura = GetHitAura())
                aura->SetStackAmount(3);
        }

        void Register() OVERRIDE
        {
            AfterHit += SpellHitFn(spell_warl_shadow_shield_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_shadow_shield_SpellScript();
    }
};

// 115232 - Shadow Shield, 115236 - Void Shield
class spell_warl_shadow_shield_aura : public SpellScriptLoader
{
public:
    spell_warl_shadow_shield_aura() : SpellScriptLoader("spell_warl_shadow_shield_aura") { }

    class spell_warl_shadow_shield_aura_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_shadow_shield_aura_AuraScript);

        bool Load() OVERRIDE
        {
            bool shadowShield = GetSpellInfo()->Id == 115232;
            _stack1 = shadowShield ? 115245 : 115241;
            _stack2 = shadowShield ? 115246 : 115242;
            _stack3 = shadowShield ? 115247 : 115243;
            _droppingStack = false;
            return true;
        }

        void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
        {
            if (_droppingStack)
                return;

            HandleRemove(aurEff, mode);

            Unit* owner = GetTarget();
            owner->CastSpell(owner, _stack1, true);
            owner->CastSpell(owner, _stack2, true);
            owner->CastSpell(owner, _stack3, true);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* owner = GetTarget();
            owner->RemoveAurasDueToSpell(_stack1);
            owner->RemoveAurasDueToSpell(_stack2);
            owner->RemoveAurasDueToSpell(_stack3);
        }

        // Each proc consumes one of the three charges, shown by the stack visuals
        void HandleProc(ProcEventInfo& /*eventInfo*/)
        {
            uint8 oldStacks = GetStackAmount();

            _droppingStack = true;
            ModStackAmount(-1);
            _droppingStack = false;

            if (oldStacks == 3)
                GetUnitOwner()->RemoveAurasDueToSpell(_stack3);
            else if (oldStacks == 2)
                GetUnitOwner()->RemoveAurasDueToSpell(_stack2);
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_warl_shadow_shield_aura_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            OnEffectRemove += AuraEffectRemoveFn(spell_warl_shadow_shield_aura_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_REAL);
            OnProc += AuraProcFn(spell_warl_shadow_shield_aura_AuraScript::HandleProc);
        }

        uint32 _stack1;
        uint32 _stack2;
        uint32 _stack3;
        bool _droppingStack;
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_shadow_shield_aura_AuraScript();
    }
};

// 30213 - Legion Strike, 115625 - Mortal Cleave
class spell_warl_legion_strike : public SpellScriptLoader
{
public:
    spell_warl_legion_strike() : SpellScriptLoader("spell_warl_legion_strike") { }

    class spell_warl_legion_strike_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_legion_strike_SpellScript);

        bool Load() OVERRIDE
        {
            _targetCount = 1;
            return true;
        }

        // Damage is split between all targets hit
        void CountTargets(std::list<WorldObject*>& targets)
        {
            _targetCount = uint32(targets.size());
        }

        void HandleHit()
        {
            if (_targetCount > 1)
                SetHitDamage(GetHitDamage() / int32(_targetCount));
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warl_legion_strike_SpellScript::CountTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
            OnHit += SpellHitFn(spell_warl_legion_strike_SpellScript::HandleHit);
        }

        uint32 _targetCount;
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_legion_strike_SpellScript();
    }
};

// 19505 - Devour Magic, 115284 - Clone Magic
class spell_warl_devour_magic : public SpellScriptLoader
{
public:
    spell_warl_devour_magic() : SpellScriptLoader("spell_warl_devour_magic") { }

    class spell_warl_devour_magic_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_devour_magic_SpellScript);

        void HandleHit()
        {
            if (!GetDispelCount())
                return;

            Unit* caster = GetCaster();
            caster->CastSpell(caster, SPELL_WARLOCK_DEVOUR_MAGIC_HEAL, true);
            if (Unit* warlock = caster->GetOwner())
                if (warlock->HasAura(SPELL_WARLOCK_GLYPH_OF_DEMON_TRAINING))
                    caster->CastSpell(warlock, SPELL_WARLOCK_DEVOUR_MAGIC_HEAL, true);
        }

        void Register() OVERRIDE
        {
            AfterHit += SpellHitFn(spell_warl_devour_magic_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_devour_magic_SpellScript();
    }
};

// 19658 - Devour Magic (heal)
class spell_warl_devour_magic_heal : public SpellScriptLoader
{
public:
    spell_warl_devour_magic_heal() : SpellScriptLoader("spell_warl_devour_magic_heal") { }

    class spell_warl_devour_magic_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_devour_magic_heal_SpellScript);

        // The Observer's version scales with the warlock's spell power
        void CalculateHeal(SpellEffIndex /*effIndex*/)
        {
            if (GetCaster()->GetEntry() != NPC_WARLOCK_OBSERVER)
                return;

            if (Unit* owner = GetCaster()->GetOwner())
                SetEffectValue(GetEffectValue() + int32(owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_ALL) * 0.15f));
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_devour_magic_heal_SpellScript::CalculateHeal, EFFECT_0, SPELL_EFFECT_HEAL);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_devour_magic_heal_SpellScript();
    }
};

// 6358 - Seduction, 115268 - Mesmerize
class spell_warl_glyph_of_demon_training_seduction : public SpellScriptLoader
{
public:
    spell_warl_glyph_of_demon_training_seduction() : SpellScriptLoader("spell_warl_glyph_of_demon_training_seduction") { }

    class spell_warl_glyph_of_demon_training_seduction_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_glyph_of_demon_training_seduction_SpellScript);

        // With the glyph the target loses its damage over time effects, except Shadow Word: Death's backlash
        void HandleHit()
        {
            Unit* warlock = GetCaster()->GetOwner();
            Unit* target = GetHitUnit();
            if (!warlock || !target || !warlock->HasAura(SPELL_WARLOCK_GLYPH_OF_DEMON_TRAINING))
                return;

            target->RemoveAurasByType(SPELL_AURA_PERIODIC_DAMAGE, 0, target->GetAura(SPELL_WARLOCK_SHADOW_WORD_DEATH_BACKLASH));
            target->RemoveAurasByType(SPELL_AURA_PERIODIC_DAMAGE_PERCENT);
            target->RemoveAurasByType(SPELL_AURA_PERIODIC_LEECH);
        }

        void Register() OVERRIDE
        {
            OnHit += SpellHitFn(spell_warl_glyph_of_demon_training_seduction_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_glyph_of_demon_training_seduction_SpellScript();
    }
};

// 119904 - Override Command Demon
class spell_warl_override_command_demon : public SpellScriptLoader
{
public:
    spell_warl_override_command_demon() : SpellScriptLoader("spell_warl_override_command_demon") { }

    class spell_warl_override_command_demon_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_override_command_demon_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            Unit* caster = GetCaster();
            Pet* pet = caster ? caster->ToPet() : NULL;
            if (!pet)
                return;

            switch (pet->GetEntry())
            {
                case NPC_WARLOCK_IMP:
                case NPC_WARLOCK_FEL_IMP:      amount = SPELL_WARLOCK_COMMAND_CAUTERIZE_MASTER; break;
                case NPC_WARLOCK_VOIDWALKER:
                case NPC_WARLOCK_VOIDLORD:     amount = SPELL_WARLOCK_COMMAND_DISARM;           break;
                case NPC_WARLOCK_FELHUNTER:    amount = SPELL_WARLOCK_COMMAND_SPELL_LOCK;       break;
                case NPC_WARLOCK_OBSERVER:     amount = SPELL_WARLOCK_COMMAND_OPTICAL_BLAST;    break;
                case NPC_WARLOCK_SUCCUBUS:     amount = SPELL_WARLOCK_COMMAND_WHIPLASH;         break;
                case NPC_WARLOCK_SHIVARRA:     amount = SPELL_WARLOCK_COMMAND_FELLASH;          break;
                case NPC_WARLOCK_FELGUARD:     amount = SPELL_WARLOCK_COMMAND_FELSTORM;         break;
                case NPC_WARLOCK_WRATHGUARD:   amount = SPELL_WARLOCK_COMMAND_WRATHSTORM;       break;
                default:                       amount = 0;                                      break;
            }
        }

        void Register() OVERRIDE
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_override_command_demon_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_override_command_demon_AuraScript();
    }
};

// 119905, 119907, 119909, 119910, 119911, 119913, 119914, 119915 - Command Demon abilities
class spell_warl_command_demon : public SpellScriptLoader
{
public:
    spell_warl_command_demon() : SpellScriptLoader("spell_warl_command_demon") { }

    class spell_warl_command_demon_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_command_demon_SpellScript);

        // Felstorm and Wrathstorm store the pet spell as trigger spell, the others in the base points
        uint32 GetPetSpellId()
        {
            if (uint32 spellId = GetSpellInfo()->Effects[EFFECT_0].TriggerSpell)
                return spellId;

            int32 spellId = GetSpellInfo()->Effects[EFFECT_0].BasePoints;
            return spellId > 0 ? uint32(spellId) : 119899;
        }

        // The pet casts its own ability so it pays the energy cost and starts its cooldown
        SpellCastResult CheckCast()
        {
            Player* warlock = GetCaster()->ToPlayer();
            if (!warlock)
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;

            Pet* pet = warlock->GetPet();
            if (!pet)
                return SpellCastResult::SPELL_FAILED_NO_PET;

            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(GetPetSpellId());
            if (!spellInfo)
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;

            Spell* spell = new Spell(pet, spellInfo, TRIGGERED_NONE);
            spell->m_targets = GetSpell()->m_targets;
            SpellCastResult result = spell->CheckPetCast(spell->m_targets.GetUnitTarget());
            if (result != SpellCastResult::SPELL_CAST_OK)
                delete spell;
            else
                spell->prepare(&spell->m_targets);
            return result;
        }

        // Felstorm and Wrathstorm force-cast the pet spell, which would bypass its energy cost
        void SuppressDefault(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_warl_command_demon_SpellScript::CheckCast);
            OnEffectHitTarget += SpellEffectFn(spell_warl_command_demon_SpellScript::SuppressDefault, EFFECT_0, SPELL_EFFECT_ANY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_command_demon_SpellScript();
    }
};

// 108499 - Grimoire of Supremacy
class spell_warl_grimoire_of_supremacy : public SpellScriptLoader
{
public:
    spell_warl_grimoire_of_supremacy() : SpellScriptLoader("spell_warl_grimoire_of_supremacy") { }

    class spell_warl_grimoire_of_supremacy_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_grimoire_of_supremacy_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Player* player = GetTarget()->ToPlayer();
            if (!player)
                return;

            for (uint8 i = 0; i < MAX_SUPREMACY_SPELLS; ++i)
                player->learnSpell(SupremacySpells[i], false);
            if (player->GetTalentSpecialization(player->GetActiveSpec()) == SPEC_WARLOCK_DEMONOLOGY)
                player->learnSpell(SPELL_WARLOCK_SUMMON_WRATHGUARD, false);

            // The current demon is replaced by its greater version on its next summon
            if (!player->GetSession()->PlayerLoading())
                player->RemovePet(NULL, PET_SAVE_NOT_IN_SLOT);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Player* player = GetTarget()->ToPlayer();
            if (!player)
                return;

            for (uint8 i = 0; i < MAX_SUPREMACY_SPELLS; ++i)
                player->removeSpell(SupremacySpells[i]);
            player->removeSpell(SPELL_WARLOCK_SUMMON_WRATHGUARD);

            if (!player->GetSession()->PlayerLogout() && GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_DEFAULT)
                player->RemovePet(NULL, PET_SAVE_NOT_IN_SLOT);
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_warl_grimoire_of_supremacy_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_warl_grimoire_of_supremacy_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }

        static uint8 const MAX_SUPREMACY_SPELLS = 6;
        static uint32 const SupremacySpells[MAX_SUPREMACY_SPELLS];
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_grimoire_of_supremacy_AuraScript();
    }
};

uint32 const spell_warl_grimoire_of_supremacy::spell_warl_grimoire_of_supremacy_AuraScript::SupremacySpells[spell_warl_grimoire_of_supremacy::spell_warl_grimoire_of_supremacy_AuraScript::MAX_SUPREMACY_SPELLS] =
{
    SPELL_WARLOCK_SUMMON_FEL_IMP,
    SPELL_WARLOCK_SUMMON_VOIDLORD,
    SPELL_WARLOCK_SUMMON_SHIVARRA,
    SPELL_WARLOCK_SUMMON_OBSERVER,
    SPELL_WARLOCK_SUMMON_ABYSSAL,
    SPELL_WARLOCK_SUMMON_TERRORGUARD
};

// 108503 - Grimoire of Sacrifice
class spell_warl_grimoire_of_sacrifice_selector : public SpellScriptLoader
{
public:
    spell_warl_grimoire_of_sacrifice_selector() : SpellScriptLoader("spell_warl_grimoire_of_sacrifice_selector") { }

    class spell_warl_grimoire_of_sacrifice_selector_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_grimoire_of_sacrifice_selector_SpellScript);

        // The sacrificed demon decides which ability Command Demon grants
        void HandleInstakill(SpellEffIndex /*effIndex*/)
        {
            Player* warlock = GetCaster()->ToPlayer();
            Unit* target = GetHitUnit();
            if (!warlock || !target)
                return;

            uint32 spellId = 0;
            switch (target->GetEntry())
            {
                case NPC_WARLOCK_IMP:        spellId = 132411; break;
                case NPC_WARLOCK_VOIDWALKER: spellId = 132413; break;
                case NPC_WARLOCK_SUCCUBUS:   spellId = 137706; break;
                case NPC_WARLOCK_FELHUNTER:  spellId = 132409; break;
                case NPC_WARLOCK_FELGUARD:   spellId = 132410; break;
                default:
                    return;
            }

            target->RemoveOwnedAura(SPELL_WARLOCK_OVERRIDE_COMMAND_DEMON);
            warlock->RemoveAurasDueToSpell(SPELL_WARLOCK_OVERRIDE_COMMAND_DEMON);
            warlock->CastSpell(warlock, SPELL_WARLOCK_OVERRIDE_COMMAND_DEMON, true);
            if (AuraEffect* overrideEff = warlock->GetAuraEffect(SPELL_WARLOCK_OVERRIDE_COMMAND_DEMON, EFFECT_0))
                overrideEff->ChangeAmount(int32(spellId));
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_grimoire_of_sacrifice_selector_SpellScript::HandleInstakill, EFFECT_0, SPELL_EFFECT_INSTAKILL);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_grimoire_of_sacrifice_selector_SpellScript();
    }
};

// 120451 - Flames of Xoroth
class spell_warl_flames_of_xoroth : public SpellScriptLoader
{
public:
    spell_warl_flames_of_xoroth() : SpellScriptLoader("spell_warl_flames_of_xoroth") { }

    class spell_warl_flames_of_xoroth_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_flames_of_xoroth_SpellScript);

        bool Load() OVERRIDE
        {
            return GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        SpellCastResult CheckPet()
        {
            Player* player = GetCaster()->ToPlayer();
            if (player->GetPet())
                return SpellCastResult::SPELL_FAILED_ALREADY_HAVE_SUMMON;

            if (!player->GetLastPetNumber())
                return SpellCastResult::SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

            return SpellCastResult::SPELL_CAST_OK;
        }

        // Brings back the last demon at full health
        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Player* player = GetCaster()->ToPlayer();
            uint32 petNumber = player->GetLastPetNumber();
            if (!petNumber || player->GetPet())
                return;

            Pet* pet = new Pet(player, PetType::SUMMON_PET);
            if (!pet->LoadPetFromDB(player, 0, petNumber, false))
            {
                delete pet;
                return;
            }

            if (pet->getDeathState() != DeathState::ALIVE)
                pet->setDeathState(DeathState::ALIVE);

            pet->SetFullHealth();
            pet->SetPower(pet->getPowerType(), pet->GetMaxPower(pet->getPowerType()));
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_warl_flames_of_xoroth_SpellScript::CheckPet);
            OnEffectHitTarget += SpellEffectFn(spell_warl_flames_of_xoroth_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_flames_of_xoroth_SpellScript();
    }
};

// 117896 - Backdraft
class spell_warl_backdraft_proc_driver : public SpellScriptLoader
{
public:
    spell_warl_backdraft_proc_driver() : SpellScriptLoader("spell_warl_backdraft_proc_driver") { }

    class spell_warl_backdraft_proc_driver_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_backdraft_proc_driver_AuraScript);

        // Copies duplicated onto the Havoc target don't grant charges
        bool CheckProc(ProcEventInfo& eventInfo)
        {
            Spell const* spell = eventInfo.TriggeredBySpell();
            Unit* target = eventInfo.GetActionTarget();
            if (!spell || !target || spell->IsTriggered())
                return false;

            return spell->m_targets.GetUnitTargetGUID() == target->GetGUID();
        }

        void Register() OVERRIDE
        {
            DoCheckProc += AuraCheckProcFn(spell_warl_backdraft_proc_driver_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_backdraft_proc_driver_AuraScript();
    }
};

// 117828 - Backdraft
class spell_warl_backdraft : public SpellScriptLoader
{
public:
    spell_warl_backdraft() : SpellScriptLoader("spell_warl_backdraft") { }

    class spell_warl_backdraft_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_backdraft_AuraScript);

        // The Chaos Bolt modifier needs Pyroclasm
        void HandleInitEffects(uint32& effectMask)
        {
            Unit* caster = GetCaster();
            if (!caster || !caster->HasAura(SPELL_WARLOCK_PYROCLASM))
                effectMask &= ~(1 << EFFECT_2);
        }

        // Charges stack up to 6; the Chaos Bolt modifier is only active with at least 3 of them
        void HandleRefreshCharges(uint8& charges)
        {
            charges = uint8(std::min<uint32>(GetCharges() + GetSpellInfo()->ProcCharges, 6));

            if (charges >= 3 && HasEffect(EFFECT_2))
            {
                AuraEffect* chaosBoltEff = GetEffect(EFFECT_2);
                chaosBoltEff->ApplySpellMod(GetUnitOwner(), false);
                chaosBoltEff->ApplySpellMod(GetUnitOwner(), true);
            }
        }

        void Register() OVERRIDE
        {
            OnInitEffects += AuraInitEffectsFn(spell_warl_backdraft_AuraScript::HandleInitEffects);
            OnRefreshCharges += AuraRefreshChargesFn(spell_warl_backdraft_AuraScript::HandleRefreshCharges);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_backdraft_AuraScript();
    }
};

// 29722 - Incinerate, 116858 - Chaos Bolt
class spell_warl_backdraft_consumer : public SpellScriptLoader
{
public:
    spell_warl_backdraft_consumer() : SpellScriptLoader("spell_warl_backdraft_consumer") { }

    class spell_warl_backdraft_consumer_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_backdraft_consumer_SpellScript);

        // Backdraft charges are spent here instead of through the spell mod system:
        // Chaos Bolt takes 3 of them and an Incinerate made instant by Backlash takes none
        void HandleAfterCast()
        {
            Unit* warlock = GetCaster();
            Aura* backdraft = warlock->GetAura(SPELL_WARLOCK_BACKDRAFT);
            if (!backdraft)
                return;

            Spell::UsedSpellMods::iterator itr = GetSpell()->m_appliedMods.find(backdraft);
            if (itr == GetSpell()->m_appliedMods.end())
                return;

            GetSpell()->m_appliedMods.erase(itr);

            bool chaosBolt = GetSpellInfo()->Id == SPELL_WARLOCK_CHAOS_BOLT;
            if (chaosBolt || (!warlock->HasAura(SPELL_WARLOCK_BACKLASH) && !warlock->HasAura(SPELL_WARLOCK_BACKLASH_CODEX)))
                if (backdraft->ModCharges(chaosBolt ? -3 : -1))
                    return;

            for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                if (AuraEffect* aurEff = backdraft->GetEffect(i))
                    if (SpellModifier* mod = aurEff->GetSpellModifier())
                        mod->charges = backdraft->GetCharges();

            if (backdraft->GetCharges() < 3 && backdraft->HasEffect(EFFECT_2))
                backdraft->GetEffect(EFFECT_2)->ApplySpellMod(warlock, false);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_warl_backdraft_consumer_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_backdraft_consumer_SpellScript();
    }
};

// 103965 - Metamorphosis
class spell_warl_metamorphosis_overrides : public SpellScriptLoader
{
public:
    spell_warl_metamorphosis_overrides() : SpellScriptLoader("spell_warl_metamorphosis_overrides") { }

    class spell_warl_metamorphosis_overrides_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_metamorphosis_overrides_AuraScript);

        // Shared by Metamorphosis and Dark Apotheosis, each form only keeps its own overrides
        void HandleInitEffects(uint32& effectMask)
        {
            Unit* warlock = GetUnitOwner();
            if (warlock->HasAura(SPELL_WARLOCK_DARK_APOTHEOSIS))
                effectMask &= ~((1 << EFFECT_1) | (1 << EFFECT_3) | (1 << EFFECT_14));
            else
            {
                effectMask &= ~((1 << EFFECT_7) | (1 << EFFECT_8));
                if (!warlock->HasSpell(SPELL_WARLOCK_METAMORPHOSIS_CHAOS_WAVE))
                    effectMask &= ~(1 << EFFECT_3);
                if (!warlock->HasSpell(SPELL_WARLOCK_METAMORPHOSIS_CURSED_AURAS))
                    effectMask &= ~(1 << EFFECT_6);
            }
        }

        void CalculateShadowBoltOverride(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            Unit* warlock = GetUnitOwner();
            if (warlock->HasAura(SPELL_WARLOCK_DARK_APOTHEOSIS))
                amount = SPELL_WARLOCK_DEMONIC_SLASH;
            else if (!warlock->HasSpell(SPELL_WARLOCK_METAMORPHOSIS_TOUCH_OF_CHAOS))
                amount = 0;
        }

        void Register() OVERRIDE
        {
            OnInitEffects += AuraInitEffectsFn(spell_warl_metamorphosis_overrides_AuraScript::HandleInitEffects);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_metamorphosis_overrides_AuraScript::CalculateShadowBoltOverride, EFFECT_0, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_metamorphosis_overrides_AuraScript();
    }
};

// 108503 - Grimoire of Sacrifice
class spell_warl_grimoire_of_sacrifice : public SpellScriptLoader
{
public:
    spell_warl_grimoire_of_sacrifice() : SpellScriptLoader("spell_warl_grimoire_of_sacrifice") { }

    class spell_warl_grimoire_of_sacrifice_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_grimoire_of_sacrifice_AuraScript);

        // Only the bonuses of the warlock's specialization apply
        void HandleInitEffects(uint32& effectMask)
        {
            Unit* caster = GetCaster();
            Player* warlock = caster ? caster->ToPlayer() : NULL;
            if (!warlock)
                return;

            effectMask &= ~((1 << EFFECT_9) | (1 << EFFECT_10));
            switch (warlock->GetTalentSpecialization(warlock->GetActiveSpec()))
            {
                case SPEC_WARLOCK_AFFLICTION:
                    effectMask &= ~((1 << EFFECT_3) | (1 << EFFECT_4));
                    break;
                case SPEC_WARLOCK_DEMONOLOGY:
                    effectMask &= ~((1 << EFFECT_2) | (1 << EFFECT_4) | (1 << EFFECT_5) | (1 << EFFECT_7));
                    break;
                case SPEC_WARLOCK_DESTRUCTION:
                    effectMask &= ~((1 << EFFECT_2) | (1 << EFFECT_3) | (1 << EFFECT_5) | (1 << EFFECT_7));
                    break;
                default:
                    effectMask &= ~((1 << EFFECT_2) | (1 << EFFECT_3) | (1 << EFFECT_4) | (1 << EFFECT_5) | (1 << EFFECT_7));
                    break;
            }
        }

        // The health bonus is part of Soul Link
        void CalculateHealthBonus(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            Player* warlock = GetUnitOwner()->ToPlayer();
            if (!warlock || !warlock->HasSpell(SPELL_WARLOCK_SOUL_LINK))
                amount = 0;
        }

        void Register() OVERRIDE
        {
            OnInitEffects += AuraInitEffectsFn(spell_warl_grimoire_of_sacrifice_AuraScript::HandleInitEffects);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_grimoire_of_sacrifice_AuraScript::CalculateHealthBonus, EFFECT_6, SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_grimoire_of_sacrifice_AuraScript();
    }
};

// 114790 - Seed of Corruption (Soulburn)
class spell_warl_seed_of_corruption_soulburn : public SpellScriptLoader
{
public:
    spell_warl_seed_of_corruption_soulburn() : SpellScriptLoader("spell_warl_seed_of_corruption_soulburn") { }

    class spell_warl_seed_of_corruption_soulburn_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_seed_of_corruption_soulburn_AuraScript);

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            DamageInfo* damageInfo = eventInfo.GetDamageInfo();
            if (!damageInfo)
                return;

            SpellInfo const* procSpell = eventInfo.GetSpellInfo();
            if (procSpell && procSpell->SpellFamilyFlags[1] & 0x8000)
                return;

            Unit* target = GetUnitOwner();
            uint32 damage = damageInfo->GetDamage();
            if (aurEff->GetAmount() <= int32(damage) || target->GetHealth() <= damage)
            {
                Unit* caster = GetCaster();
                Remove();
                if (caster)
                    caster->CastSpell(target, SPELL_WARLOCK_SEED_OF_CORRUPTION_SOULBURN_DAMAGE, true);
                return;
            }

            GetEffect(EFFECT_2)->SetAmount(aurEff->GetAmount() - int32(damage));
        }

        void Register() OVERRIDE
        {
            OnEffectProc += AuraEffectProcFn(spell_warl_seed_of_corruption_soulburn_AuraScript::HandleProc, EFFECT_2, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_seed_of_corruption_soulburn_AuraScript();
    }
};

// 87385 - Seed of Corruption (Soulburn)
class spell_warl_seed_of_corruption_soulburn_explode : public SpellScriptLoader
{
public:
    spell_warl_seed_of_corruption_soulburn_explode() : SpellScriptLoader("spell_warl_seed_of_corruption_soulburn_explode") { }

    class spell_warl_seed_of_corruption_soulburn_explode_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_seed_of_corruption_soulburn_explode_SpellScript);

        void HandleCast()
        {
            GetCaster()->CastSpell(GetCaster(), SPELL_WARLOCK_SOULSHARD, true);
        }

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            if (Unit* target = GetHitUnit())
                GetCaster()->CastSpell(target, SPELL_WARLOCK_CORRUPTION, true);
        }

        void Register() OVERRIDE
        {
            OnCast += SpellCastFn(spell_warl_seed_of_corruption_soulburn_explode_SpellScript::HandleCast);
            OnEffectHitTarget += SpellEffectFn(spell_warl_seed_of_corruption_soulburn_explode_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_seed_of_corruption_soulburn_explode_SpellScript();
    }
};

// 6201, 34130 - Create Healthstone
class spell_warl_create_healthstone : public SpellScriptLoader
{
public:
    spell_warl_create_healthstone() : SpellScriptLoader("spell_warl_create_healthstone") { }

    class spell_warl_create_healthstone_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_create_healthstone_SpellScript);

        // A carried Healthstone gets its charges restored instead of a new one being created
        void HandleScript(SpellEffIndex /*effIndex*/)
        {
            Player* player = GetCaster()->ToPlayer();
            SpellInfo const* createInfo = sSpellMgr->GetSpellInfo(SPELL_WARLOCK_CREATE_HEALTHSTONE_ITEM);
            if (!player || !createInfo)
                return;

            uint32 spellId = player->HasItemCount(createInfo->Effects[EFFECT_0].ItemType, 1) ? SPELL_WARLOCK_CREATE_HEALTHSTONE_CHARGES : SPELL_WARLOCK_CREATE_HEALTHSTONE_ITEM;
            player->CastSpell(player, spellId, true);
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_warl_create_healthstone_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_create_healthstone_SpellScript();
    }
};

// 6360, 19647, 89751, 115770, 115781, 115831, 118093, 119899 - Command Demon pet abilities
class spell_warl_command_demon_cooldown : public SpellScriptLoader
{
public:
    spell_warl_command_demon_cooldown() : SpellScriptLoader("spell_warl_command_demon_cooldown") { }

    class spell_warl_command_demon_cooldown_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_command_demon_cooldown_SpellScript);

        // Command Demon on the warlock's bar shares the cooldown of the ability the demon used
        void HandleAfterCast()
        {
            Pet* pet = GetCaster()->ToPet();
            Player* warlock = pet ? pet->GetOwner() : NULL;
            if (!warlock)
                return;

            uint32 commandSpellId = 0;
            switch (GetSpellInfo()->Id)
            {
                case SPELL_WARLOCK_PET_CAUTERIZE_MASTER: commandSpellId = SPELL_WARLOCK_COMMAND_CAUTERIZE_MASTER; break;
                case SPELL_WARLOCK_PET_DISARM:           commandSpellId = SPELL_WARLOCK_COMMAND_DISARM;           break;
                case SPELL_WARLOCK_PET_SPELL_LOCK:       commandSpellId = SPELL_WARLOCK_COMMAND_SPELL_LOCK;       break;
                case SPELL_WARLOCK_PET_WHIPLASH:         commandSpellId = SPELL_WARLOCK_COMMAND_WHIPLASH;         break;
                case SPELL_WARLOCK_PET_FELSTORM:         commandSpellId = SPELL_WARLOCK_COMMAND_FELSTORM;         break;
                case SPELL_WARLOCK_PET_OPTICAL_BLAST:    commandSpellId = SPELL_WARLOCK_COMMAND_OPTICAL_BLAST;    break;
                case SPELL_WARLOCK_PET_FELLASH:          commandSpellId = SPELL_WARLOCK_COMMAND_FELLASH;          break;
                case SPELL_WARLOCK_PET_WRATHSTORM:       commandSpellId = SPELL_WARLOCK_COMMAND_WRATHSTORM;       break;
                default:
                    return;
            }

            uint32 cooldown = GetSpellInfo()->GetRecoveryTime();
            warlock->ApplySpellMod(GetSpellInfo()->Id, SPELLMOD_COOLDOWN, cooldown);
            if (!cooldown)
                return;

            warlock->AddSpellCooldown(commandSpellId, 0, time(NULL) + cooldown / IN_MILLISECONDS);
            warlock->SendSpellCooldown(commandSpellId, cooldown);
        }

        void Register() OVERRIDE
        {
            AfterCast += SpellCastFn(spell_warl_command_demon_cooldown_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_command_demon_cooldown_SpellScript();
    }
};

static uint32 const WarlockPetBasicAttacks[] =
{
    3110,   // Firebolt
    3716,   // Torment
    7814,   // Lash of Pain
    30213,  // Legion Strike
    54049,  // Shadow Bite
    115625, // Mortal Cleave
    115746, // Felbolt
    115748, // Bladedance
    115778  // Tongue Lash
};

// 3110, 3716, 7814, 30213, 54049, 115625, 115746, 115748, 115778 - Demon basic attacks
class spell_warl_pet_auto_cast : public SpellScriptLoader
{
public:
    spell_warl_pet_auto_cast() : SpellScriptLoader("spell_warl_pet_auto_cast")
    {
        for (uint32 i = 0; i < sSpellPowerStore.GetNumRows(); ++i)
        {
            SpellPowerEntry const* spellPower = sSpellPowerStore.LookupEntry(i);
            if (!spellPower || spellPower->powerType != uint32(POWER_ENERGY))
                continue;

            for (uint8 j = 0; j < sizeof(WarlockPetBasicAttacks) / sizeof(WarlockPetBasicAttacks[0]); ++j)
                if (spellPower->spellId == WarlockPetBasicAttacks[j])
                    _energyCosts[spellPower->spellId] = spellPower->manaCost;
        }
    }

    class spell_warl_pet_auto_cast_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_pet_auto_cast_SpellScript);

    public:
        spell_warl_pet_auto_cast_SpellScript(std::map<uint32, uint32> const* energyCosts) : _energyCosts(energyCosts) { }

    private:
        // Auto-cast keeps enough energy for the demon's special ability
        SpellCastResult CheckAutoCast()
        {
            uint32 spellId = GetSpellInfo()->Id;
            int32 reserve = (spellId == SPELL_WARLOCK_PET_LEGION_STRIKE || spellId == SPELL_WARLOCK_PET_MORTAL_CLEAVE) ? 40 : 25;

            std::map<uint32, uint32>::const_iterator itr = _energyCosts->find(spellId);
            int32 cost = itr != _energyCosts->end() ? int32(itr->second) : 0;
            if (GetCaster()->GetPower(POWER_ENERGY) < cost + reserve)
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() OVERRIDE
        {
            OnCheckAutoCast += SpellCheckCastFn(spell_warl_pet_auto_cast_SpellScript::CheckAutoCast);
        }

        std::map<uint32, uint32> const* _energyCosts;
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_pet_auto_cast_SpellScript(&_energyCosts);
    }

private:
    std::map<uint32, uint32> _energyCosts;
};

// 3110, 3716, 7814, 30213, 54049, 115625, 115746, 115748, 115778 - Demon basic attacks
class spell_warl_pet_demonic_fury : public SpellScriptLoader
{
public:
    spell_warl_pet_demonic_fury() : SpellScriptLoader("spell_warl_pet_demonic_fury") { }

    class spell_warl_pet_demonic_fury_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_pet_demonic_fury_SpellScript);

        // The triggered spell doesn't exist; its value is Demonic Fury for a Demonology owner
        void HandleDemonicFury(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);

            Unit* owner = GetCaster()->GetOwner();
            Player* warlock = owner ? owner->ToPlayer() : NULL;
            if (warlock && warlock->GetTalentSpecialization(warlock->GetActiveSpec()) == SPEC_WARLOCK_DEMONOLOGY)
                warlock->CastCustomSpell(SPELL_WARLOCK_DEMONIC_FURY_ENERGIZE, SPELLVALUE_BASE_POINT1, GetEffectValue(), warlock, true);
        }

        void Register() OVERRIDE
        {
            OnEffectLaunch += SpellEffectFn(spell_warl_pet_demonic_fury_SpellScript::HandleDemonicFury, EFFECT_2, SPELL_EFFECT_TRIGGER_SPELL_WITH_VALUE);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_pet_demonic_fury_SpellScript();
    }
};

// 17767 - Shadow Bulwark
class spell_warl_shadow_bulwark : public SpellScriptLoader
{
public:
    spell_warl_shadow_bulwark() : SpellScriptLoader("spell_warl_shadow_bulwark") { }

    class spell_warl_shadow_bulwark_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_shadow_bulwark_SpellScript);

        SpellCastResult CheckAutoCast()
        {
            if (GetCaster()->GetHealthPct() >= 20.0f)
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;
            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() OVERRIDE
        {
            OnCheckAutoCast += SpellCheckCastFn(spell_warl_shadow_bulwark_SpellScript::CheckAutoCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_shadow_bulwark_SpellScript();
    }
};

// 17735 - Suffering
class spell_warl_suffering : public SpellScriptLoader
{
public:
    spell_warl_suffering() : SpellScriptLoader("spell_warl_suffering") { }

    class spell_warl_suffering_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_suffering_SpellScript);

        SpellCastResult CheckAutoCast()
        {
            Unit* target = GetExplTargetUnit();
            if (target && target->GetVictim() == GetCaster())
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;
            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() OVERRIDE
        {
            OnCheckAutoCast += SpellCheckCastFn(spell_warl_suffering_SpellScript::CheckAutoCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_suffering_SpellScript();
    }
};

// 6358 - Seduction, 115268 - Mesmerize
class spell_warl_seduction : public SpellScriptLoader
{
public:
    spell_warl_seduction() : SpellScriptLoader("spell_warl_seduction") { }

    class spell_warl_seduction_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_seduction_SpellScript);

        // Only auto-cast on someone holding the warlock under fear, confuse, silence or stun
        SpellCastResult CheckAutoCast()
        {
            Unit* warlock = GetCaster()->GetOwner();
            Unit* target = GetExplTargetUnit();
            if (!warlock || !target)
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;

            Unit::AuraApplicationMap const& auras = warlock->GetAppliedAuras();
            for (Unit::AuraApplicationMap::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
            {
                if (itr->second->IsPositive())
                    continue;

                Aura const* aura = itr->second->GetBase();
                if (aura->GetCasterGUID() != target->GetGUID())
                    continue;

                if (aura->HasEffectType(SPELL_AURA_MOD_FEAR) || aura->HasEffectType(SPELL_AURA_MOD_FEAR_2) ||
                    aura->HasEffectType(SPELL_AURA_MOD_CONFUSE) || aura->HasEffectType(SPELL_AURA_MOD_SILENCE) ||
                    aura->HasEffectType(SPELL_AURA_MOD_STUN))
                    return SpellCastResult::SPELL_CAST_OK;
            }

            return SpellCastResult::SPELL_FAILED_DONT_REPORT;
        }

        void Register() OVERRIDE
        {
            OnCheckAutoCast += SpellCheckCastFn(spell_warl_seduction_SpellScript::CheckAutoCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_seduction_SpellScript();
    }
};

// 89808 - Singe Magic
class spell_warl_singe_magic : public SpellScriptLoader
{
public:
    spell_warl_singe_magic() : SpellScriptLoader("spell_warl_singe_magic") { }

    class spell_warl_singe_magic_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_singe_magic_SpellScript);

        SpellCastResult CheckAutoCast()
        {
            Unit* owner = GetCaster()->GetOwner();
            if (!owner)
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;

            if (!owner->HasCrowdControlAura(NULL, DISPEL_MAGIC))
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() OVERRIDE
        {
            OnCheckAutoCast += SpellCheckCastFn(spell_warl_singe_magic_SpellScript::CheckAutoCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_singe_magic_SpellScript();
    }
};

// 108505 - Archimonde's Darkness
class spell_warl_archimondes_darkness : public SpellScriptLoader
{
public:
    spell_warl_archimondes_darkness() : SpellScriptLoader("spell_warl_archimondes_darkness") { }

    class spell_warl_archimondes_darkness_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_archimondes_darkness_AuraScript);

        bool Load() OVERRIDE
        {
            return GetUnitOwner()->GetTypeId() == TypeID::TYPEID_PLAYER;
        }

        // Without the extra charge, a Dark Soul still recharging goes back on a normal cooldown
        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            static uint32 const darkSouls[] = { SPELL_WARLOCK_DARK_SOUL_INSTABILITY, SPELL_WARLOCK_DARK_SOUL_MISERY, SPELL_WARLOCK_DARK_SOUL_KNOWLEDGE };

            Player* warlock = GetUnitOwner()->ToPlayer();
            for (uint8 i = 0; i < sizeof(darkSouls) / sizeof(darkSouls[0]); ++i)
            {
                if (uint32 cooldown = warlock->GetChargeCooldown(darkSouls[i]))
                {
                    warlock->RemoveSpellCooldown(darkSouls[i]);
                    warlock->AddCustomCooldown(darkSouls[i], cooldown);
                }
            }
        }

        void Register() OVERRIDE
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_warl_archimondes_darkness_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_MOD_CHARGES, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_archimondes_darkness_AuraScript();
    }
};

static uint32 const DemonicGatewayPurpleCharge[5] = { 113915, 113916, 113917, 113918, 113919 };
static uint32 const DemonicGatewayGreenCharge[5]  = { 113903, 113911, 113912, 113913, 113914 };

// 111771 - Demonic Gateway
class spell_warl_demonic_gateway : public SpellScriptLoader
{
public:
    spell_warl_demonic_gateway() : SpellScriptLoader("spell_warl_demonic_gateway") { }

    class spell_warl_demonic_gateway_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_demonic_gateway_SpellScript);

        SpellCastResult CheckCast()
        {
            WorldLocation const* dest = GetExplTargetDest();
            if (!dest)
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;

            PathGenerator path(GetCaster());
            path.CalculatePath(dest->GetPositionX(), dest->GetPositionY(), dest->GetPositionZ());
            if (path.GetPathType() & (PATHFIND_NOPATH | PATHFIND_INCOMPLETE))
                return SpellCastResult::SPELL_FAILED_NOPATH;

            Movement::PointsArray const& points = path.GetPath();
            if (points.size() < 2)
                return SpellCastResult::SPELL_FAILED_NOPATH;

            float length = 0.0f;
            for (size_t i = 1; i < points.size(); ++i)
                length += (points[i] - points[i - 1]).length();

            float maxLength = GetCaster()->GetExactDist(dest);
            maxLength *= std::abs(GetCaster()->GetPositionZ() - dest->GetPositionZ()) > 10.0f ? 1.1f : 1.3f;
            if (length > maxLength)
                return SpellCastResult::SPELL_FAILED_NOPATH;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void HandleCast()
        {
            Unit* warlock = GetCaster();
            warlock->RemoveAurasDueToSpell(SPELL_WARLOCK_DEMONIC_GATEWAY_AURA);
            warlock->m_Events.Schedule(10000, [warlock]()
            {
                warlock->CastSpell(warlock, SPELL_WARLOCK_DEMONIC_GATEWAY_AURA, true);
            });
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_warl_demonic_gateway_SpellScript::CheckCast);
            OnCast += SpellCastFn(spell_warl_demonic_gateway_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_demonic_gateway_SpellScript();
    }
};

// 113890 - Demonic Gateway (purple), 113886 - Demonic Gateway (green)
class spell_warl_demonic_gateway_summon : public SpellScriptLoader
{
public:
    spell_warl_demonic_gateway_summon() : SpellScriptLoader("spell_warl_demonic_gateway_summon") { }

    class spell_warl_demonic_gateway_summon_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warl_demonic_gateway_summon_SpellScript);

        void SelectDestPurple(SpellDestination& dest)
        {
            dest.Relocate(*GetCaster());
        }

        void SelectDestGreen(SpellDestination& dest)
        {
            WorldLocation const* explDest = GetExplTargetDest();
            if (!explDest)
                return;

            Position pos;
            explDest->GetPosition(&pos);
            pos.SetOrientation(GetCaster()->GetOrientation());
            dest.Relocate(pos);
        }

        void Register() OVERRIDE
        {
            if (m_scriptSpellId == SPELL_WARLOCK_DEMONIC_GATEWAY_SUMMON_PURPLE)
                OnDestinationTargetSelect += SpellDestinationTargetSelectFn(spell_warl_demonic_gateway_summon_SpellScript::SelectDestPurple, EFFECT_0, TARGET_UNK_125);
            else
                OnDestinationTargetSelect += SpellDestinationTargetSelectFn(spell_warl_demonic_gateway_summon_SpellScript::SelectDestGreen, EFFECT_0, TARGET_UNK_138);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_warl_demonic_gateway_summon_SpellScript();
    }
};

// 113901 - Demonic Gateway
class spell_warl_demonic_gateway_aura : public SpellScriptLoader
{
public:
    spell_warl_demonic_gateway_aura() : SpellScriptLoader("spell_warl_demonic_gateway_aura") { }

    class spell_warl_demonic_gateway_aura_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warl_demonic_gateway_aura_AuraScript);

        void ApplyCharges(uint8 charges)
        {
            if (!charges || charges > 5)
                return;

            Unit* warlock = GetTarget();
            std::list<TempSummon*> summons;
            warlock->GetSummons(summons, NPC_WARLOCK_GATEWAY_PURPLE);
            for (std::list<TempSummon*>::const_iterator itr = summons.begin(); itr != summons.end(); ++itr)
                (*itr)->CastSpell(*itr, DemonicGatewayPurpleCharge[charges - 1], true);

            summons.clear();
            warlock->GetSummons(summons, NPC_WARLOCK_GATEWAY_GREEN);
            for (std::list<TempSummon*>::const_iterator itr = summons.begin(); itr != summons.end(); ++itr)
                (*itr)->CastSpell(*itr, DemonicGatewayGreenCharge[charges - 1], true);

            if (AuraEffect* aurEff = GetEffect(EFFECT_0))
                aurEff->SetAmount(charges);
        }

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            ApplyCharges(1);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* warlock = GetTarget();
            std::list<TempSummon*> summons;
            warlock->GetSummons(summons, NPC_WARLOCK_GATEWAY_PURPLE);
            warlock->GetSummons(summons, NPC_WARLOCK_GATEWAY_GREEN);
            for (std::list<TempSummon*>::const_iterator itr = summons.begin(); itr != summons.end(); ++itr)
                (*itr)->DespawnOrUnsummon();
        }

        void HandlePeriodic(AuraEffect const* /*aurEff*/)
        {
            if (GetCharges() < 5)
            {
                SetCharges(GetCharges() + 1);
                ApplyCharges(GetCharges());
            }
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_warl_demonic_gateway_aura_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_warl_demonic_gateway_aura_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_demonic_gateway_aura_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_warl_demonic_gateway_aura_AuraScript();
    }
};

void AddSC_warlock_spell_scripts()
{
    new spell_warl_chaotic_energy();

    new spell_warl_banish();

    new spell_warl_demonic_circle_summon();
    new spell_warl_demonic_circle_teleport();
    new spell_warl_drain_life();
    
    new spell_warl_fel_synergy();
    
    new spell_warl_healthstone();
    new spell_warl_healthstone_glyphed();
    new spell_warl_life_tap();
    new spell_warl_seed_of_corruption();
    new spell_warl_shadow_trance_proc();
    
    new spell_warl_soul_swap();
    new spell_warl_soul_swap_dot_marker();
    new spell_warl_soul_swap_exhale();
    new spell_warl_soul_swap_override();
    new spell_warl_soulshatter();
    new spell_warl_unstable_affliction();

    // Destruction
    new spell_warl_burning_embers();
    new spell_warl_ember_consumer();
    new spell_warl_chaos_bolt_dot();
    new spell_warl_always_crit();
    new spell_warl_conflagrate_aura();
    new spell_warl_shadowburn();
    new spell_warl_ember_tap();
    new spell_warl_glyph_of_ember_tap();
    new spell_warl_fire_and_brimstone();
    new spell_warl_fire_and_brimstone_spell();
    new spell_warl_fire_and_brimstone_immolate();
    new spell_warl_backlash();
    new spell_warl_havoc();
    new spell_warl_havoc_target_selector();
    new spell_warl_rain_of_fire();

    // Demonology
    new spell_warl_mastery_master_demonologist();
    new spell_warl_master_demonologist();
    new spell_warl_demonic_fury();
    new spell_warl_metamorphosis();
    new spell_warl_dark_apotheosis();
    new spell_warl_fury_ward_fix();
    new spell_warl_twilight_ward_fix();
    new spell_warl_molten_core();
    new spell_warl_decimate();
    new spell_warl_chaos_wave();
    new spell_warl_hand_of_guldan();
    new spell_warl_hand_of_guldan_glyphed();
    new spell_warl_shadowflame();
    new spell_warl_imp_swarm();
    new spell_warl_glyph_of_imp_swarm();
    new spell_warl_wild_imp();
    new spell_warl_demonic_calling();
    new spell_warl_fel_firebolt();
    new spell_warl_hellfire();
    new spell_warl_touch_of_chaos();
    new spell_warl_corruption();
    new spell_warl_demonic_leap();
    new spell_warl_demonic_leap_jump();
    new spell_warl_meteor_slam();
    new spell_warl_glyph_of_demon_hunting();
    new spell_warl_glyph_of_shadow_bolt();
    new spell_warl_glyph_of_shadow_bolt_spec_fix();
    new spell_warl_shadow_bolt_glyphed();
    new spell_warl_summon_doomguard();
    new spell_warl_doombolt();
    new spell_warl_fel_energy();
    new spell_warl_glyph_of_carrion_swarm();

    // Affliction
    new spell_warl_malefic_grasp();
    new spell_warl_drain_soul();
    new spell_warl_agony();
    new spell_warl_agony_instant_tick();
    new spell_warl_haunt();
    new spell_warl_glyph_of_siphon_life();
    new spell_warl_glyph_of_siphon_life_heal();

    // Soulburn
    new spell_warl_soul_swap_soulburn();
    new spell_warl_health_funnel();
    new spell_warl_health_funnel_soulburn();
    new spell_warl_soulburn_override();
    new spell_soulburn_consumer();
    new spell_warl_curse_of_exhaustion_soulburn();
    new spell_warl_unending_breath_soulburn();

    // Talents, glyphs, demons and set bonuses
    new spell_warl_glyph_of_soulwell();
    new spell_warl_unbound_will();
    new spell_warl_soul_link();
    new spell_warl_soul_leech();
    new spell_warl_dark_bargain_on_absorb();
    new spell_warl_dark_regeneration();
    new spell_warl_sacrificial_pact();
    new spell_warl_twilight_ward_s12();
    new spell_warl_burning_rush();
    new spell_warl_soul_harvest();
    new spell_warl_fear();
    new spell_warl_blood_horror();
    new spell_warl_life_tap_heal_absorb();
    new spell_warl_soulstone();
    new spell_warl_soulstone_resurrect();
    new spell_warl_glyph_of_soul_consumption();
    new spell_warl_t16_2p_bonus();
    new spell_warl_t16_4p_bonus();
    new spell_warl_shadow_shield();
    new spell_warl_shadow_shield_aura();
    new spell_warl_legion_strike();
    new spell_warl_devour_magic();
    new spell_warl_devour_magic_heal();
    new spell_warl_glyph_of_demon_training_seduction();
    new spell_warl_override_command_demon();
    new spell_warl_command_demon();
    new spell_warl_grimoire_of_supremacy();
    new spell_warl_grimoire_of_sacrifice_selector();
    new spell_warl_flames_of_xoroth();
    new spell_warl_backdraft_proc_driver();
    new spell_warl_backdraft();
    new spell_warl_backdraft_consumer();
    new spell_warl_metamorphosis_overrides();
    new spell_warl_grimoire_of_sacrifice();
    new spell_warl_seed_of_corruption_soulburn();
    new spell_warl_seed_of_corruption_soulburn_explode();
    new spell_warl_create_healthstone();
    new spell_warl_command_demon_cooldown();
    new spell_warl_demonic_gateway();
    new spell_warl_demonic_gateway_summon();
    new spell_warl_demonic_gateway_aura();
    new spell_warl_pet_auto_cast();
    new spell_warl_pet_demonic_fury();
    new spell_warl_shadow_bulwark();
    new spell_warl_suffering();
    new spell_warl_seduction();
    new spell_warl_singe_magic();
    new spell_warl_archimondes_darkness();
}
