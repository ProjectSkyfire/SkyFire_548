/*
 * Copyright (C) 2011-2019 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2019 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2019 MaNGOS <https://www.getmangos.eu/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Scripts for spells with SPELLFAMILY_SHAMAN and SPELLFAMILY_GENERIC spells used by shaman players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_sha_".
 */

#include "ScriptMgr.h"
#include "GridNotifiers.h"
#include "Unit.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"

enum ShamanSpells
{
    SHAMAN_SPELL_SATED                      = 57724,
    SHAMAN_SPELL_EXHAUSTION                 = 57723,
    HUNTER_SPELL_INSANITY                   = 95809,
    MAGE_SPELL_TEMPORAL_DISPLACEMENT        = 80354,
    SPELL_SHA_LIGHTNING_SHIELD_AURA         = 324,
    SPELL_SHA_ASCENDANCE_ELEMENTAL	        = 114050,
    SPELL_SHA_ASCENDANCE_RESTORATION        = 114052,
    SPELL_SHA_ASCENDANCE_ENHANCED	        = 114051,
    SPELL_SHA_ASCENDANCE			        = 114049,
    SPELL_SHA_HEALING_RAIN                  = 142923,
    SPELL_SHA_HEALING_RAIN_TICK             = 73921,
    SPELL_SHA_HEALING_RAIN_AURA             = 73920,
    SPELL_SHA_EARTHQUAKE                    = 61882,
    SPELL_SHA_EARTHQUAKE_TICK               = 77478,
    SPELL_SHA_EARTHQUAKE_KNOCKING_DOWN      = 77505,
    SPELL_SHA_ELEMENTAL_BLAST               = 117014,
    SPELL_SHA_ELEMENTAL_BLAST_RATING_BONUS  = 118522,
    SPELL_SHA_ELEMENTAL_BLAST_NATURE_VISUAL = 118517,
    SPELL_SHA_ELEMENTAL_BLAST_FROST_VISUAL  = 118515,
    SPELL_SHA_LAVA_LASH                     = 60103,
    SPELL_SHA_FLAME_SHOCK                   = 8050,
    SPELL_SHA_STORMSTRIKE                   = 17364,
    SPELL_SHA_LIGHTNING_SHIELD_ORB_DAMAGE   = 26364,
    SPELL_SHA_HEALING_STREAM                = 52042,
    SPELL_SHA_GLYPH_OF_HEALING_STREAM       = 119523,
    SPELL_SHA_FULMINATION                   = 88766,
    SPELL_SHA_FULMINATION_TRIGGERED         = 88767,
    SPELL_SHA_FULMINATION_INFO              = 95774,
    SPELL_SHA_ROLLING_THUNDER_AURA          = 88764,
    SPELL_SHA_ROLLING_THUNDER_ENERGIZE      = 88765,
    SPELL_SHA_UNLEASH_ELEMENTS              = 73680,
    SPELL_SHA_SEARING_FLAMES_DAMAGE_DONE    = 77661,
    SPELL_SHA_FIRE_NOVA                     = 1535,
    SPELL_SHA_FIRE_NOVA_TRIGGERED           = 131786,
    SPELL_SHA_TIDAL_WAVES                   = 51564,
    SPELL_SHA_TIDAL_WAVES_PROC              = 53390,
    SPELL_SHA_MANA_TIDE                     = 16191,
    SPELL_SHA_FROST_SHOCK_FREEZE            = 63685,
    SPELL_SHA_FROZEN_POWER                  = 63374,
    SPELL_SHA_MAIL_SPECIALIZATION_AGI       = 86099,
    SPELL_SHA_MAIL_SPECIALISATION_INT       = 86100,
    SPELL_SHA_UNLEASHED_FURY_TALENT         = 117012,
    SPELL_SHA_UNLEASHED_FURY_FLAMETONGUE    = 118470,
    SPELL_SHA_UNLEASHED_FURY_WINDFURY       = 118472,
    SPELL_SHA_UNLEASHED_FURY_EARTHLIVING    = 118473,
    SPELL_SHA_UNLEASHED_FURY_FROSTBRAND     = 118474,
    SPELL_SHA_UNLEASHED_FURY_ROCKBITER      = 118475,
    SPELL_SHA_STONE_BULWARK_ABSORB          = 114893,
    SPELL_SHA_EARTHGRAB_IMMUNITY            = 116946,
    SPELL_SHA_EARTHBIND_FOR_EARTHGRAB_TOTEM = 116947,
    SPELL_SHA_ECHO_OF_THE_ELEMENTS          = 108283,
    SPELL_SHA_ANCESTRAL_GUIDANCE            = 114911,
    SPELL_SHA_CONDUCTIVITY_TALENT           = 108282,
    SPELL_SHA_CONDUCTIVITY_HEAL             = 118800,
    SPELL_SHA_GLYPH_OF_LAKESTRIDER          = 55448,
    SPELL_SHA_WATER_WALKING                 = 546,
    SPELL_SHA_GLYPH_OF_SHAMANISTIC_RAGE     = 63280,
    SPELL_SHA_SOLAR_BEAM                    = 113286,
    SPELL_SHA_SOLAR_BEAM_SILENCE            = 113288,
    SPELL_SHA_GHOST_WOLF                    = 2645,
    SPELL_SHA_ITEM_T14_4P                   = 123124,
    SPELL_SHA_GLYPH_OF_HEALING_STREAM_TOTEM = 55456,
    SPELL_SHA_LAVA_BURST                    = 51505,
    SPELL_SHA_ITEM_S12_4P_ENHANCEMENT_BONUS = 131554,
    SPELL_SHA_RESTORATIVE_MISTS             = 114083,
    SPELL_SHA_GLYPH_OF_TELLURIC_CURRENTS    = 55453,
    SPELL_SHA_LAVA_SURGE_AURA		        = 77762
};

// Totemic Projection - 108287
class spell_sha_totemic_projection : public SpellScriptLoader
{
    public:
        spell_sha_totemic_projection() : SpellScriptLoader("spell_sha_totemic_projection") { }

        class spell_sha_totemic_projection_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_totemic_projection_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    std::list<Creature*> leapList;
                    Unit* leapTarget = NULL;

                    caster->GetCreatureListWithEntryInGrid(leapList, 47319, 45.0f);

                    for (auto itr : leapList)
                    {
                        if (!itr->ToUnit() || !itr->ToTempSummon())
                            continue;

                        if (!itr->ToTempSummon()->GetSummoner())
                            continue;

                        if (itr->ToTempSummon()->GetSummoner()->GetGUID() != caster->GetGUID())
                            continue;

                        leapTarget = itr->ToUnit();
                        break;
                    }

                    if (!leapTarget)
                        return;

                    for (Unit::ControlList::const_iterator itr = caster->m_Controlled.begin(); itr != caster->m_Controlled.end(); ++itr)
                    {
                        if ((*itr)->isTotem())
                        {
                            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo((*itr)->GetUInt32Value(UNIT_CREATED_BY_SPELL));
                            if (!spellInfo)
                                continue;

                            float objSize = caster->GetObjectSize();
                            float angle = spellInfo->Effects[0].TargetA.CalcDirectionAngle();
                            float dist = spellInfo->Effects[0].CalcRadius(caster);

                            if (dist < objSize)
                                dist = objSize;

                            Position pos;
                            leapTarget->GetPosition(&pos);

                            angle += (*itr)->GetOrientation();
                            float destx, desty, destz, ground, floor;
                            destx = pos.m_positionX + dist * std::cos(angle);
                            desty = pos.m_positionY + dist * std::sin(angle);
                            ground = (*itr)->GetMap()->GetHeight((*itr)->GetPhaseMask(), destx, desty, MAX_HEIGHT, true);
                            floor = (*itr)->GetMap()->GetHeight((*itr)->GetPhaseMask(), destx, desty, pos.m_positionZ, true);
                            destz = fabs(ground - pos.m_positionZ) <= fabs(floor - pos.m_positionZ) ? ground : floor;

                            (*itr)->NearTeleportTo(destx, desty, destz, (*itr)->GetOrientation());
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_totemic_projection_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_totemic_projection_SpellScript();
        }
};

// Hex - 51514
class spell_sha_hex : public SpellScriptLoader
{

    public:
        spell_sha_hex() : SpellScriptLoader("spell_sha_hex") { }

        class spell_sha_hex_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_hex_AuraScript);

            void OnApply(constAuraEffectPtr aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* target = GetTarget())
                {
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
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_sha_hex_AuraScript::OnApply, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_hex_AuraScript();
        }
};

// Water Ascendant - 114052
class spell_sha_water_ascendant : public SpellScriptLoader
{
    public:
        spell_sha_water_ascendant() : SpellScriptLoader("spell_sha_water_ascendant") { }

        class spell_sha_water_ascendant_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_water_ascendant_AuraScript);

            void OnProc(constAuraEffectPtr aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                if (!GetCaster())
                    return;

                Player* _player = GetCaster()->ToPlayer();
                if (!_player)
                    return;

                if (_player->HasSpellCooldown(SPELL_SHA_RESTORATIVE_MISTS))
                    return;

                if (eventInfo.GetActor()->GetGUID() != _player->GetGUID())
                    return;

                if (!eventInfo.GetDamageInfo()->GetSpellInfo())
                    return;

                if (eventInfo.GetDamageInfo()->GetSpellInfo()->Id == SPELL_SHA_RESTORATIVE_MISTS)
                    return;

                if (eventInfo.GetDamageInfo()->GetSpellInfo()->Id == SPELL_SHA_ANCESTRAL_GUIDANCE)
                    return;

                if (!(eventInfo.GetHealInfo()->GetHeal()))
                    return;

                if (!(eventInfo.GetDamageInfo()->GetSpellInfo()->IsPositive()))
                    return;

                if (Unit* target = eventInfo.GetActionTarget())
                {
                    std::list<Unit*> tempList;
                    std::list<Unit*> alliesList;
                    target->GetAttackableUnitListInRange(tempList, 15.0f);

                    for (auto itr : tempList)
                    {
                        if (!_player->IsWithinLOSInMap(itr))
                            continue;

                        if (itr->IsHostileTo(_player))
                            continue;

                        alliesList.push_back(itr);
                    }

                    if (!alliesList.empty())
                    {
                        // Heal amount distribued for all allies, caster included
                        int32 bp = eventInfo.GetHealInfo()->GetHeal() / alliesList.size();

                        if (bp > 0)
                            _player->CastCustomSpell((*alliesList.begin()), SPELL_SHA_RESTORATIVE_MISTS, &bp, NULL, NULL, true);   // Restorative Mists

                        _player->AddSpellCooldown(SPELL_SHA_RESTORATIVE_MISTS, 0, time(NULL) + 1);               // This prevent from multiple procs
                    }
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_water_ascendant_AuraScript::OnProc, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_water_ascendant_AuraScript();
        }
};

// Prowl - 113289
class spell_sha_prowl : public SpellScriptLoader
{
    public:
        spell_sha_prowl() : SpellScriptLoader("spell_sha_prowl") { }

        class spell_sha_prowl_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_prowl_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    _player->CastSpell(_player, SPELL_SHA_GHOST_WOLF, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_prowl_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_prowl_SpellScript();
        }
};

// Solar beam - 113286
class spell_sha_solar_beam : public SpellScriptLoader
{
    public:
        spell_sha_solar_beam() : SpellScriptLoader("spell_sha_solar_beam") { }

        class spell_sha_solar_beam_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_solar_beam_AuraScript);

            void OnTick(constAuraEffectPtr aurEff)
            {
                if (DynamicObject* dynObj = GetCaster()->GetDynObject(SPELL_SHA_SOLAR_BEAM))
                    GetCaster()->CastSpell(dynObj->GetPositionX(), dynObj->GetPositionY(), dynObj->GetPositionZ(), SPELL_SHA_SOLAR_BEAM_SILENCE, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_solar_beam_AuraScript::OnTick, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_solar_beam_AuraScript();
        }
};

// Called by Shamanistic Rage - 30823
// Glyph of Shamanistic Rage - 63280
class spell_sha_glyph_of_shamanistic_rage : public SpellScriptLoader
{
    public:
        spell_sha_glyph_of_shamanistic_rage() : SpellScriptLoader("spell_sha_glyph_of_shamanistic_rage") { }

        class spell_sha_glyph_of_shamanistic_rage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_glyph_of_shamanistic_rage_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(SPELL_SHA_GLYPH_OF_SHAMANISTIC_RAGE))
                    {
                        DispelChargesList dispelList;
                        _player->GetDispellableAuraList(_player, DISPEL_ALL_MASK, dispelList);
                        if (!dispelList.empty())
                        {
                            for (auto itr : dispelList)
                                if (_player->HasAura(itr.first->GetId()))
                                    _player->RemoveAura(itr.first);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_glyph_of_shamanistic_rage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_glyph_of_shamanistic_rage_SpellScript();
        }
};

// Called by Ghost Wolf - 2645
// Glyph of Lakestrider - 55448
class spell_sha_glyph_of_lakestrider : public SpellScriptLoader
{
    public:
        spell_sha_glyph_of_lakestrider() : SpellScriptLoader("spell_sha_glyph_of_lakestrider") { }

        class spell_sha_glyph_of_lakestrider_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_glyph_of_lakestrider_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (_player->HasAura(SPELL_SHA_GLYPH_OF_LAKESTRIDER))
                        _player->CastSpell(_player, SPELL_SHA_WATER_WALKING, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_glyph_of_lakestrider_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_glyph_of_lakestrider_SpellScript();
        }
};

// Call of the Elements - 108285
class spell_sha_call_of_the_elements : public SpellScriptLoader
{
    public:
        spell_sha_call_of_the_elements() : SpellScriptLoader("spell_sha_call_of_the_elements") { }

        class spell_sha_call_of_the_elements_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_call_of_the_elements_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    // immediately finishes the cooldown on totems with less than 3min cooldown
                    const SpellCooldowns& cm = _player->GetSpellCooldownMap();
                    for (SpellCooldowns::const_iterator itr = cm.begin(); itr != cm.end();)
                    {
                        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(itr->first);
                        if (!spellInfo)
                            continue;

                        if (spellInfo->Id == 51485 || spellInfo->Id == 108273 || spellInfo->Id == 108270
                            || spellInfo->Id == 108269 || spellInfo->Id == 8143 || spellInfo->Id == 8177
                            || spellInfo->Id == 5394 || spellInfo->Id == 2484 || spellInfo->Id == 108273
                            && spellInfo->GetRecoveryTime() > 0)
                            _player->RemoveSpellCooldown((itr++)->first, true);
                        else
                            ++itr;
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_call_of_the_elements_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_call_of_the_elements_SpellScript();
        }
};

// Called by Healing Wave - 331, Greater Healing Wave - 77472 and Healing Surge - 8004
// Called by Lightning Bolt - 403, Chain Lightning - 421, Earth Shock - 8042 and Stormstrike - 17364
// Called by Lightning Bolt - 45284, Chain Lightning - 45297
// Conductivity - 108282
class spell_sha_conductivity : public SpellScriptLoader
{
    public:
        spell_sha_conductivity() : SpellScriptLoader("spell_sha_conductivity") { }

        class spell_sha_conductivity_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_conductivity_SpellScript);

            void HandleAfterHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (AuraPtr conductivity = _player->GetAura(SPELL_SHA_CONDUCTIVITY_TALENT))
                        {
                            if (DynamicObject* dynObj = _player->GetDynObject(SPELL_SHA_HEALING_RAIN))
                            {
                                uint32 remainingDuration = conductivity->GetEffect(2)->GetAmount();
                                uint32 addDuration = std::min(remainingDuration, uint32(4000));

                                dynObj->SetDuration(dynObj->GetDuration() + addDuration);
                                conductivity->GetEffect(2)->SetAmount(remainingDuration - addDuration);

                                if (AuraPtr healingRain = _player->GetAura(SPELL_SHA_HEALING_RAIN_AURA))
                                {
                                    healingRain->SetDuration(healingRain->GetDuration() + addDuration);
                                    healingRain->SetMaxDuration(healingRain->GetMaxDuration() + addDuration);
                                }
                            }
                        }
                    }
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_sha_conductivity_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_conductivity_SpellScript();
        }
};

// Ancestral Guidance - 108281
class spell_sha_ancestral_guidance : public SpellScriptLoader
{
    public:
        spell_sha_ancestral_guidance() : SpellScriptLoader("spell_sha_ancestral_guidance") { }

        class spell_sha_ancestral_guidance_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_ancestral_guidance_AuraScript);

            void OnProc(constAuraEffectPtr aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                if (!GetCaster())
                    return;

                Player* _player = GetCaster()->ToPlayer();
                if (!_player)
                    return;

                if (eventInfo.GetActor()->GetGUID() != _player->GetGUID())
                    return;

                if (!eventInfo.GetDamageInfo()->GetSpellInfo())
                    return;

                if (eventInfo.GetDamageInfo()->GetSpellInfo()->Id == SPELL_SHA_ANCESTRAL_GUIDANCE)
                    return;

                if (eventInfo.GetDamageInfo()->GetSpellInfo()->Id == SPELL_SHA_RESTORATIVE_MISTS)
                    return;

                if (!(eventInfo.GetDamageInfo()->GetDamage()) && !(eventInfo.GetHealInfo()->GetHeal()))
                    return;

                if (!(eventInfo.GetDamageInfo()->GetDamageType() == SPELL_DIRECT_DAMAGE) && !(eventInfo.GetDamageInfo()->GetDamageType() == HEAL))
                    return;

                if (Unit* target = eventInfo.GetActionTarget())
                {
                    int32 bp = eventInfo.GetDamageInfo()->GetDamage() > eventInfo.GetHealInfo()->GetHeal() ? eventInfo.GetDamageInfo()->GetDamage() : eventInfo.GetHealInfo()->GetHeal();
                    if (!bp)
                        return;

                    bp = int32(bp * 0.40f);

                    _player->CastCustomSpell(target, SPELL_SHA_ANCESTRAL_GUIDANCE, &bp, NULL, NULL, true);
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_ancestral_guidance_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_ancestral_guidance_AuraScript();
        }
};

// Echo of the Elements - 108283
class spell_sha_echo_of_the_elements : public SpellScriptLoader
{
    public:
        spell_sha_echo_of_the_elements() : SpellScriptLoader("spell_sha_echo_of_the_elements") { }

        class spell_sha_echo_of_the_elements_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_echo_of_the_elements_AuraScript);

            void OnProc(constAuraEffectPtr aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                if (!GetCaster())
                    return;

                Player* _player = GetCaster()->ToPlayer();
                Unit* target = eventInfo.GetActionTarget();
                if (!_player || !target)
                    return;

                if (_player->HasSpellCooldown(SPELL_SHA_ECHO_OF_THE_ELEMENTS))
                    return;

                if (eventInfo.GetActor()->GetGUID() != _player->GetGUID())
                    return;

                SpellInfo const* procSpell = eventInfo.GetDamageInfo()->GetSpellInfo();
                if (!procSpell)
                    return;

                bool singleTarget = false;
                for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                    if ((procSpell->Effects[i].TargetA.GetTarget() == TARGET_UNIT_TARGET_ALLY ||
                        procSpell->Effects[i].TargetA.GetTarget() == TARGET_UNIT_TARGET_ENEMY) &&
                        procSpell->Effects[i].TargetB.GetTarget() == 0)
                        singleTarget = true;

                if (!singleTarget)
                    return;

                int32 chance = 6;

                // devs told that proc chance is 6% for Elemental and Restoration specs and 30% for Enhancement
                if (_player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_SHAMAN_ENHANCEMENT && procSpell->Id != SPELL_SHA_ELEMENTAL_BLAST)
                    chance = 30;

                if (!(eventInfo.GetDamageInfo()->GetDamage()) && !(eventInfo.GetHealInfo()->GetHeal()))
                    return;

                if (!(eventInfo.GetDamageInfo()->GetDamageType() == SPELL_DIRECT_DAMAGE) && !(eventInfo.GetDamageInfo()->GetDamageType() == HEAL))
                    return;

                if (!roll_chance_i(chance))
                    return;

                _player->AddSpellCooldown(SPELL_SHA_ECHO_OF_THE_ELEMENTS, 0, time(NULL) + 6); // This prevent from multiple procs
                _player->CastSpell(target, procSpell->Id, true);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_echo_of_the_elements_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_echo_of_the_elements_AuraScript();
        }
};

// Earthgrab - 64695
class spell_sha_earthgrab : public SpellScriptLoader
{
    public:
        spell_sha_earthgrab() : SpellScriptLoader("spell_sha_earthgrab") { }

        class spell_sha_earthgrab_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_earthgrab_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (target->HasAura(SPELL_SHA_EARTHGRAB_IMMUNITY, caster->GetGUID()))
                            caster->CastSpell(target, SPELL_SHA_EARTHBIND_FOR_EARTHGRAB_TOTEM, true);
                        else
                            caster->CastSpell(target, SPELL_SHA_EARTHGRAB_IMMUNITY, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_earthgrab_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_earthgrab_SpellScript();
        }
};

// Stone Bulwark - 114893
class spell_sha_stone_bulwark : public SpellScriptLoader
{
    public:
        spell_sha_stone_bulwark() : SpellScriptLoader("spell_sha_stone_bulwark") { }

        class spell_sha_stone_bulwark_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_stone_bulwark_AuraScript);

            void CalculateAmount(constAuraEffectPtr , int32 & amount, bool & )
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* owner = caster->GetOwner())
                    {
                        if (owner->ToPlayer() && !owner->HasAura(SPELL_SHA_STONE_BULWARK_ABSORB))
                        {
                            int32 AP = owner->GetTotalAttackPowerValue(BASE_ATTACK);
                            int32 spellPower = owner->ToPlayer()->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_ALL);
                            amount += (AP > spellPower) ? int32(1.641f * AP) : int32(2.625f * spellPower);
                        }
                        else if (owner->ToPlayer() && owner->HasAura(SPELL_SHA_STONE_BULWARK_ABSORB))
                        {
                            int32 AP = owner->GetTotalAttackPowerValue(BASE_ATTACK);
                            int32 spellPower = owner->ToPlayer()->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_ALL);
                            amount += (AP > spellPower) ? int32(0.547f * AP) : int32(0.875f * spellPower);
                            amount += owner->GetAura(SPELL_SHA_STONE_BULWARK_ABSORB)->GetEffect(0)->GetAmount();
                        }
                    }
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_stone_bulwark_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_stone_bulwark_AuraScript();
        }
};

// Mail Specialization - 86529
class spell_sha_mail_specialization : public SpellScriptLoader
{
    public:
        spell_sha_mail_specialization() : SpellScriptLoader("spell_sha_mail_specialization") { }

        class spell_sha_mail_specialization_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_mail_specialization_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetCaster())
                    return;

                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_SHAMAN_ELEMENTAL
                            || _player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_SHAMAN_RESTORATION)
                        _player->CastSpell(_player, SPELL_SHA_MAIL_SPECIALISATION_INT, true);
                    else if (_player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_SHAMAN_ENHANCEMENT)
                        _player->CastSpell(_player, SPELL_SHA_MAIL_SPECIALIZATION_AGI, true);
                }
            }

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetCaster())
                    return;

                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(SPELL_SHA_MAIL_SPECIALISATION_INT))
                        _player->RemoveAura(SPELL_SHA_MAIL_SPECIALISATION_INT);
                    else if (_player->HasAura(SPELL_SHA_MAIL_SPECIALIZATION_AGI))
                        _player->RemoveAura(SPELL_SHA_MAIL_SPECIALIZATION_AGI);
                }
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_sha_mail_specialization_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_sha_mail_specialization_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_mail_specialization_AuraScript();
        }
};

// Frost Shock - 8056
class spell_sha_frozen_power : public SpellScriptLoader
{
    public:
        spell_sha_frozen_power() : SpellScriptLoader("spell_sha_frozen_power") { }

        class spell_sha_frozen_power_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_frozen_power_SpellScript);

            bool Validate(SpellEntry const * spellEntry)
            {
                if (!sSpellMgr->GetSpellInfo(8056))
                    return false;
                return true;
            }

            void HandleAfterHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (_player->HasAura(SPELL_SHA_FROZEN_POWER))
                            _player->CastSpell(target, SPELL_SHA_FROST_SHOCK_FREEZE, true);
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_sha_frozen_power_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_frozen_power_SpellScript();
        }
};

// Spirit Link - 98020 : triggered by 98017
// Spirit Link Totem
class spell_sha_spirit_link : public SpellScriptLoader
{
    public:
        spell_sha_spirit_link() : SpellScriptLoader("spell_sha_spirit_link") { }

        class spell_sha_spirit_link_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_spirit_link_SpellScript);

            void HandleAfterCast()
            {
                if (Unit* caster = GetCaster())
                {
                    if (caster->GetEntry() == 53006)
                    {
                        if (caster->GetOwner())
                        {
                            if (Player* _player = caster->GetOwner()->ToPlayer())
                            {
                                std::list<Unit*> memberList;
                                _player->GetPartyMembers(memberList);

                                float totalRaidHealthPct = 0;

                                uint32 amountOfAffected = 0;

                                for (auto itr : memberList)
                                    if (itr->IsWithinDistInMap(_player, 10.0f))
                                    {
                                        ++amountOfAffected;
                                        totalRaidHealthPct += itr->GetHealthPct();
                                    }

                                totalRaidHealthPct /= amountOfAffected * 100.0f;

                                for (auto itr : memberList)
                                    if (itr->IsWithinDistInMap(_player, 10.0f))
                                        itr->SetHealth(uint32(totalRaidHealthPct * itr->GetMaxHealth()));
                            }
                        }
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_sha_spirit_link_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_spirit_link_SpellScript();
        }
};

// Mana Tide - 16191
class spell_sha_mana_tide : public SpellScriptLoader
{
    public:
        spell_sha_mana_tide() : SpellScriptLoader("spell_sha_mana_tide") { }

        class spell_sha_mana_tide_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_mana_tide_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SHA_ELEMENTAL_BLAST))
                    return false;
                return true;
            }

            void HandleOnHit()
            {
                if (Unit* target = GetHitUnit())
                {
                    if (Player* _player = GetCaster()->GetOwner()->ToPlayer())
                    {
                        AuraApplication* aura = target->GetAuraApplication(SPELL_SHA_MANA_TIDE, GetCaster()->GetGUID());

                        aura->GetBase()->GetEffect(0)->ChangeAmount(0);

                        int32 spirit = _player->GetStat(STAT_SPIRIT) * 2;

                        aura->GetBase()->GetEffect(0)->ChangeAmount(spirit);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_mana_tide_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_mana_tide_SpellScript();
        }
};

// Called by Chain Heal - 1064 or Riptide - 61295
// Tidal Waves - 51564
class spell_sha_tidal_waves : public SpellScriptLoader
{
    public:
        spell_sha_tidal_waves() : SpellScriptLoader("spell_sha_tidal_waves") { }

        class spell_sha_tidal_waves_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_tidal_waves_SpellScript)

            bool Validate(SpellEntry const * /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(1064) || !sSpellMgr->GetSpellInfo(61295))
                    return false;
                return true;
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(SPELL_SHA_TIDAL_WAVES))
                    {
                        if (Unit* target = GetHitUnit())
                        {
                            int32 bp0 = -(sSpellMgr->GetSpellInfo(SPELL_SHA_TIDAL_WAVES)->Effects[0].BasePoints);
                            int32 bp1 = sSpellMgr->GetSpellInfo(SPELL_SHA_TIDAL_WAVES)->Effects[1].BasePoints;
                            _player->CastCustomSpell(_player, SPELL_SHA_TIDAL_WAVES_PROC, &bp0, &bp1, NULL, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_tidal_waves_SpellScript::HandleOnHit);
            }
        };

        SpellScript *GetSpellScript() const
        {
            return new spell_sha_tidal_waves_SpellScript();
        }
};

// Fire Nova - 1535
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
                    if (target->HasAura(SPELL_SHA_FLAME_SHOCK))
                    {
                        caster->CastSpell(target, SPELL_SHA_FIRE_NOVA_TRIGGERED, true);
                    }
                }
            }
            SpellCastResult HandleCheckCast()
            {
                UnitList targets;
                JadeCore::AnyUnitHavingBuffInObjectRangeCheck u_check(GetCaster(), GetCaster(), 100, SPELL_SHA_FLAME_SHOCK, false);
                JadeCore::UnitListSearcher<JadeCore::AnyUnitHavingBuffInObjectRangeCheck> searcher(GetCaster(), targets, u_check);
                GetCaster()->VisitNearbyObject(100, searcher);
                
                return targets.size() == 0 ? SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW : SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_sha_fire_nova_SpellScript::HandleCheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_sha_fire_nova_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_fire_nova_SpellScript();
        }
};

// Unleash Elements - 73680
class spell_sha_unleash_elements : public SpellScriptLoader
{
    public:
        spell_sha_unleash_elements() : SpellScriptLoader("spell_sha_unleash_elements") { }

        class spell_sha_unleash_elements_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_unleash_elements_SpellScript);

            SpellCastResult HandleCheckCast()
            {
                 Unit* caster = GetCaster();
                 if (!caster)
                     return SPELL_FAILED_DONT_REPORT;

                 Player* plr = caster->ToPlayer();
                 if (!plr)
                     return SPELL_FAILED_DONT_REPORT;

                 if (!GetExplTargetUnit())
                     return SPELL_FAILED_BAD_TARGETS;

                 Item* weapons[2];
                 weapons[0] = plr->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
                 weapons[1] = plr->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                 bool found_enchantment = false;

                 for (int i = 0; i < 2; i++)
                 {
                     if (!weapons[i])
                          continue;

                     int32 enchant = weapons[i]->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT);
 
                     if (enchant == 0)
                         continue;

                     found_enchantment = true;

                     switch (enchant)
                     {
                         case 5: // Flametongue Weapon
                         case 2: // Frostbrand Weapon
                         case 3021: // Rockbiter Weapon
                         case 283: // Windfury Weapon
                         {
                             if (!plr->IsValidAttackTarget(GetExplTargetUnit()))
                                 return SPELL_FAILED_BAD_TARGETS;
                             break;
                         }
                     }
                 }
                 return found_enchantment == false ? SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW : SPELL_CAST_OK;
            }

            void HandleOnHit()
            {
                       Unit* caster = GetCaster();
                       if (!caster)
                           return;
                       Player* plr = caster->ToPlayer();
                       if (!plr)
                           return;

                       if (!GetExplTargetUnit())
                           return;

                        Item* weapons[2];
                        weapons[0] = plr->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
                        weapons[1] = plr->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                        for (int i = 0; i < 2; i++)
                        {
                            if (!weapons[i])
                                continue;

                            uint32 unleashSpell = 0;
                            uint32 furySpell = 0;
                            Unit *target = GetExplTargetUnit();
                            bool hasFuryTalent = plr->HasAura(SPELL_SHA_UNLEASHED_FURY_TALENT);
                            bool hostileTarget = plr->IsValidAttackTarget(target);
                            bool hostileSpell = true;
                            switch (weapons[i]->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT))
                            {
                                case 3345: // Earthliving Weapon
                                    unleashSpell = 73685; // Unleash Life
                                    hostileSpell = false;

                                    if (hasFuryTalent)
                                        furySpell = SPELL_SHA_UNLEASHED_FURY_EARTHLIVING;
                                    break;
                                case 5: // Flametongue Weapon
                                    unleashSpell = 73683; // Unleash Flame

                                    if (hasFuryTalent)
                                        furySpell = SPELL_SHA_UNLEASHED_FURY_FLAMETONGUE;
                                    break;
                                case 2: // Frostbrand Weapon
                                    unleashSpell = 73682; // Unleash Frost

                                    if (hasFuryTalent)
                                        furySpell = SPELL_SHA_UNLEASHED_FURY_FROSTBRAND;
                                    break;
                                case 3021: // Rockbiter Weapon
                                    unleashSpell = 73684; // Unleash Earth

                                    if (hasFuryTalent)
                                        furySpell = SPELL_SHA_UNLEASHED_FURY_ROCKBITER;
                                    break;
                                case 283: // Windfury Weapon
                                    unleashSpell = 73681; // Unleash Wind

                                    if (hasFuryTalent)
                                        furySpell = SPELL_SHA_UNLEASHED_FURY_WINDFURY;
                                    break;
                            }

                            if (hostileSpell && !hostileTarget)
                                return; // don't allow to attack non-hostile targets. TODO: check this before cast

                            if (!hostileSpell && hostileTarget)
                                target = plr;   // heal ourselves instead of the enemy

                            if (unleashSpell)
                                plr->CastSpell(target, unleashSpell, true);

                            if (furySpell)
                                plr->CastSpell(target, furySpell, true);

                            target = GetHitUnit();

                            // If weapons are enchanted by same enchantment, only one should be unleashed
                            if (i == 0 && weapons[0] && weapons[1])
                                if (weapons[0]->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT) == weapons[1]->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT))
                                    return;
                   }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_sha_unleash_elements_SpellScript::HandleCheckCast);
                OnHit += SpellHitFn(spell_sha_unleash_elements_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_unleash_elements_SpellScript();
        }
};

// Called by Lightning Bolt - 403 and Chain Lightning - 421
// Lightning Bolt (Mastery) - 45284, Chain Lightning - 45297 and Lava Beam (Fire Ascendant) - 114074
// Rolling Thunder - 88764
class spell_sha_rolling_thunder : public SpellScriptLoader
{
    public:
        spell_sha_rolling_thunder() : SpellScriptLoader("spell_sha_rolling_thunder") { }

        class spell_sha_rolling_thunder_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_rolling_thunder_SpellScript)

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        // Glyph of Telluric Currents - 10% for Enhancement
                        if (_player->HasAura(55453) && GetSpellInfo()->Id == 403 && !_player->HasAura(123099) && !_player->HasAura(112858))
                            _player->EnergizeBySpell(_player, 55453, _player->CountPctFromMaxMana(10), POWER_MANA);
                        // 2% for Elemental and Restoration
                        else if (_player->HasAura(55453) && GetSpellInfo()->Id == 403 && (_player->HasAura(123099) || _player->HasAura(112858)))
                            _player->EnergizeBySpell(_player, 55453, _player->CountPctFromMaxMana(2), POWER_MANA);

                        if (roll_chance_i(60) && _player->HasAura(SPELL_SHA_ROLLING_THUNDER_AURA))
                        {
                            if (AuraPtr lightningShield = _player->GetAura(SPELL_SHA_LIGHTNING_SHIELD_AURA))
                            {
                                _player->CastSpell(_player, SPELL_SHA_ROLLING_THUNDER_ENERGIZE, true);

                                uint8 lsCharges = lightningShield->GetCharges();

                                if (lsCharges < 6)
                                {
                                    uint8 chargesBonus = _player->HasAura(SPELL_SHA_ITEM_T14_4P) ? 2 : 1;
                                    lightningShield->SetCharges(lsCharges + chargesBonus);
                                    lightningShield->RefreshDuration();
                                }
                                else if (lsCharges < 7)
                                {
                                    lightningShield->SetCharges(lsCharges + 1);
                                    lightningShield->RefreshDuration();
                                }

                                // refresh to handle Fulmination visual
                                lsCharges = lightningShield->GetCharges();

                                if (lsCharges >= 7 && _player->HasAura(SPELL_SHA_FULMINATION))
                                    _player->CastSpell(_player, SPELL_SHA_FULMINATION_INFO, true);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_rolling_thunder_SpellScript::HandleOnHit);
            }
        };

        SpellScript *GetSpellScript() const
        {
            return new spell_sha_rolling_thunder_SpellScript();
        }
};

// 88766 Fulmination handled in 8042 Earth Shock
class spell_sha_fulmination : public SpellScriptLoader
{
    public:
        spell_sha_fulmination() : SpellScriptLoader("spell_sha_fulmination") { }

        class spell_sha_fulmination_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_fulmination_SpellScript)

            void HandleAfterHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (!GetHitDamage())
                            return;

                        AuraEffectPtr fulminationAura = _player->GetDummyAuraEffect(SPELLFAMILY_SHAMAN, 2010, 0);
                        if (!fulminationAura)
                            return;

                        AuraPtr lightningShield = _player->GetAura(SPELL_SHA_LIGHTNING_SHIELD_AURA);
                        if (!lightningShield)
                            return;

                        uint8 lsCharges = lightningShield->GetCharges();
                        if (lsCharges <= 1)
                            return;

                        uint8 usedCharges = lsCharges - 1;

                        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(SPELL_SHA_LIGHTNING_SHIELD_ORB_DAMAGE);
                        if (!spellInfo)
                            return;
                        int32 basePoints = _player->CalculateSpellDamage(target, spellInfo, 0);
                        uint32 damage = usedCharges * _player->SpellDamageBonusDone(target, spellInfo, basePoints, SPELL_DIRECT_DAMAGE);

                        _player->CastCustomSpell(SPELL_SHA_FULMINATION_TRIGGERED, SPELLVALUE_BASE_POINT0, damage, target, true, NULL, fulminationAura);
                        lightningShield->SetCharges(lsCharges - usedCharges);

                        _player->RemoveAura(SPELL_SHA_FULMINATION_INFO);
                    }
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_sha_fulmination_SpellScript::HandleAfterHit);
            }
        };

        SpellScript *GetSpellScript() const
        {
            return new spell_sha_fulmination_SpellScript();
        }
};

// Healing Stream - 52042
class spell_sha_healing_stream : public SpellScriptLoader
{
    public:
        spell_sha_healing_stream() : SpellScriptLoader("spell_sha_healing_stream") { }

        class spell_sha_healing_stream_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_healing_stream_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SHA_HEALING_STREAM))
                    return false;
                return true;
            }

            void HandleOnHit()
            {
                if (!GetCaster()->GetOwner())
                    return;

                if (Player* _player = GetCaster()->GetOwner()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        // Glyph of Healing Stream Totem
                        if (target->GetGUID() != _player->GetGUID() && _player->HasAura(SPELL_SHA_GLYPH_OF_HEALING_STREAM_TOTEM))
                            _player->CastSpell(target, SPELL_SHA_GLYPH_OF_HEALING_STREAM, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_healing_stream_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_healing_stream_SpellScript();
        }
};

// Called by Stormstrike - 17364 and Lava Lash - 60103
// Static Shock - 51527
class spell_sha_static_shock : public SpellScriptLoader
{
    public:
        spell_sha_static_shock() : SpellScriptLoader("spell_sha_static_shock") { }

        class spell_sha_static_shock_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_static_shock_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        // While have Lightning Shield active
                        if (target && _player->HasAura(SPELL_SHA_LIGHTNING_SHIELD_AURA) && _player->HasAura(51527))
                        {
                            // Item - Shaman T9 Enhancement 2P Bonus (Static Shock)
                            if (_player->HasAura(67220))
                            {
                                if (roll_chance_i(48))
                                {
                                    _player->CastSpell(target, SPELL_SHA_LIGHTNING_SHIELD_ORB_DAMAGE, true);
                                }
                            }
                            else
                            {
                                if (roll_chance_i(45))
                                {
                                    _player->CastSpell(target, SPELL_SHA_LIGHTNING_SHIELD_ORB_DAMAGE, true);
                                }
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_static_shock_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_static_shock_SpellScript();
        }
};

// Elemental Blast - 117014
class spell_sha_elemental_blast : public SpellScriptLoader
{
    public:
        spell_sha_elemental_blast() : SpellScriptLoader("spell_sha_elemental_blast") { }

        class spell_sha_elemental_blast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_elemental_blast_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SHA_ELEMENTAL_BLAST))
                    return false;
                return true;
            }

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetExplTargetUnit())
                    {
                        _player->CastSpell(target, SPELL_SHA_ELEMENTAL_BLAST_FROST_VISUAL, true);
                        _player->CastSpell(target, SPELL_SHA_ELEMENTAL_BLAST_NATURE_VISUAL, true);
                    }
                }
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    int32 randomEffect = irand(0, 2);

                    _player->CastSpell(_player, SPELL_SHA_ELEMENTAL_BLAST_RATING_BONUS, true);

                    AuraApplication* aura = _player->GetAuraApplication(SPELL_SHA_ELEMENTAL_BLAST_RATING_BONUS, _player->GetGUID());

                    if (aura)
                    {
                        switch (randomEffect)
                        {
                            case 0:
                            {
                                aura->GetBase()->GetEffect(1)->ChangeAmount(0);
                                aura->GetBase()->GetEffect(2)->ChangeAmount(0);
                                break;
                            }
                            case 1:
                            {
                                aura->GetBase()->GetEffect(0)->ChangeAmount(0);
                                aura->GetBase()->GetEffect(2)->ChangeAmount(0);
                                break;
                            }
                            case 2:
                            {
                                aura->GetBase()->GetEffect(0)->ChangeAmount(0);
                                aura->GetBase()->GetEffect(1)->ChangeAmount(0);
                                break;
                            }
                            default:
                                break;
                        }
                    }

                    // Unleash Flame increase Elemental Blast damage for 30%
                    if (_player->HasAura(73683))
                        SetHitDamage(int32(GetHitDamage() * 1.3f));
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_sha_elemental_blast_SpellScript::HandleAfterCast);
                OnHit += SpellHitFn(spell_sha_elemental_blast_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_elemental_blast_SpellScript();
        }
};

// Earthquake : Ticks - 77478
class spell_sha_earthquake_tick : public SpellScriptLoader
{
    public:
        spell_sha_earthquake_tick() : SpellScriptLoader("spell_sha_earthquake_tick") { }

        class spell_sha_earthquake_tick_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_earthquake_tick_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SHA_EARTHQUAKE_TICK))
                    return false;
                return true;
            }

            void HandleOnHit()
            {
                // With a 10% chance of knocking down affected targets
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (roll_chance_i(10))
                            _player->CastSpell(target, SPELL_SHA_EARTHQUAKE_KNOCKING_DOWN, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_earthquake_tick_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_earthquake_tick_SpellScript();
        }
};

// Earthquake - 61882
class spell_sha_earthquake : public SpellScriptLoader
{
    public:
        spell_sha_earthquake() : SpellScriptLoader("spell_sha_earthquake") { }

        class spell_sha_earthquake_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_earthquake_AuraScript);

            void OnTick(constAuraEffectPtr aurEff)
            {
                if (!GetCaster())
                    return;

                if (DynamicObject* dynObj = GetCaster()->GetDynObject(SPELL_SHA_EARTHQUAKE))
                    GetCaster()->CastSpell(dynObj->GetPositionX(), dynObj->GetPositionY(), dynObj->GetPositionZ(), SPELL_SHA_EARTHQUAKE_TICK, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_earthquake_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_earthquake_AuraScript();
        }
};

// Healing Rain - 73920
class spell_sha_healing_rain : public SpellScriptLoader
{
    public:
        spell_sha_healing_rain() : SpellScriptLoader("spell_sha_healing_rain") { }

        class spell_sha_healing_rain_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_healing_rain_SpellScript);

            void HandleOnHit()
            {
                if (WorldLocation const* loc = GetExplTargetDest())
                {
                    if (Unit* caster = GetCaster())
                    {
                        // Casting a second healing rain after prolonging the previous one using conductivity
                        // will replace the old healing rain with base amount of duration (in other words, you will not have 2 healing rains).
                        if (DynamicObject* dynObj = caster->GetDynObject(SPELL_SHA_HEALING_RAIN))
                            caster->RemoveDynObject(SPELL_SHA_HEALING_RAIN);

                        caster->CastSpell(loc->GetPositionX(), loc->GetPositionY(), loc->GetPositionZ(), SPELL_SHA_HEALING_RAIN, true);

                        // Reset amount of Conductivity
                        if (AuraPtr conductivity = caster->GetAura(SPELL_SHA_CONDUCTIVITY_TALENT))
                            conductivity->GetEffect(2)->SetAmount(40000);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_healing_rain_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_healing_rain_SpellScript();
        }

        class spell_sha_healing_rain_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_healing_rain_AuraScript);

            void OnTick(constAuraEffectPtr aurEff)
            {
                if (!GetCaster())
                    return;

                if (DynamicObject* dynObj = GetCaster()->GetDynObject(SPELL_SHA_HEALING_RAIN))
                    GetCaster()->CastSpell(dynObj->GetPositionX(), dynObj->GetPositionY(), dynObj->GetPositionZ(), SPELL_SHA_HEALING_RAIN_TICK, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_healing_rain_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_healing_rain_AuraScript();
        }
};

// Ascendance - 114049
class spell_sha_ascendance : public SpellScriptLoader
{
    public:
        spell_sha_ascendance() : SpellScriptLoader("spell_sha_ascendance") { }

        class spell_sha_ascendance_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_ascendance_SpellScript);

            bool Validate(SpellInfo const* spellEntry)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SHA_ASCENDANCE))
                    return false;
                return true;
            }

            SpellCastResult CheckCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_NONE)
                    {
                        SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_MUST_SELECT_TALENT_SPECIAL);
                        return SPELL_FAILED_CUSTOM_ERROR;
                    }

                    return SPELL_CAST_OK;
                }

                return SPELL_CAST_OK;
            }

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    switch(_player->GetSpecializationId(_player->GetActiveSpec()))
                    {
                        case SPEC_SHAMAN_ELEMENTAL:
                            _player->CastSpell(_player, SPELL_SHA_ASCENDANCE_ELEMENTAL, true);

                            // If we has a cooldown on Lava Burst, we should remove it.
                            if (_player->HasSpellCooldown(SPELL_SHA_LAVA_BURST))
                                _player->RemoveSpellCooldown(SPELL_SHA_LAVA_BURST, true);
                            break;
                        case SPEC_SHAMAN_ENHANCEMENT:
                            _player->CastSpell(_player, SPELL_SHA_ASCENDANCE_ENHANCED, true);

                            if (_player->HasSpellCooldown(SPELL_SHA_STORMSTRIKE))
                                _player->RemoveSpellCooldown(SPELL_SHA_STORMSTRIKE, true);
                            break;
                        case SPEC_SHAMAN_RESTORATION:
                            _player->CastSpell(_player, SPELL_SHA_ASCENDANCE_RESTORATION, true);
                            break;
                        default:
                            break;
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_sha_ascendance_SpellScript::CheckCast);
                AfterCast += SpellCastFn(spell_sha_ascendance_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_ascendance_SpellScript();
        }
};

class EarthenPowerTargetSelector
{
    public:
        EarthenPowerTargetSelector() { }

        bool operator() (WorldObject* target)
        {
            if (!target->ToUnit())
                return true;

            if (!target->ToUnit()->HasAuraWithMechanic(1 << MECHANIC_SNARE))
                return true;

            return false;
        }
};

class spell_sha_bloodlust : public SpellScriptLoader
{
    public:
        spell_sha_bloodlust() : SpellScriptLoader("spell_sha_bloodlust") { }

        class spell_sha_bloodlust_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_bloodlust_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SHAMAN_SPELL_SATED))
                    return false;
                return true;
            }

            void RemoveInvalidTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(JadeCore::UnitAuraCheck(true, SHAMAN_SPELL_SATED));
                targets.remove_if(JadeCore::UnitAuraCheck(true, HUNTER_SPELL_INSANITY));
                targets.remove_if(JadeCore::UnitAuraCheck(true, MAGE_SPELL_TEMPORAL_DISPLACEMENT));
            }

            void ApplyDebuff()
            {
                if (Unit* target = GetHitUnit())
                    target->CastSpell(target, SHAMAN_SPELL_SATED, true);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_bloodlust_SpellScript::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_bloodlust_SpellScript::RemoveInvalidTargets, EFFECT_1, TARGET_UNIT_CASTER_AREA_RAID);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_bloodlust_SpellScript::RemoveInvalidTargets, EFFECT_2, TARGET_UNIT_CASTER_AREA_RAID);
                AfterHit += SpellHitFn(spell_sha_bloodlust_SpellScript::ApplyDebuff);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_bloodlust_SpellScript();
        }
};

class spell_sha_heroism : public SpellScriptLoader
{
    public:
        spell_sha_heroism() : SpellScriptLoader("spell_sha_heroism") { }

        class spell_sha_heroism_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_heroism_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SHAMAN_SPELL_EXHAUSTION))
                    return false;
                return true;
            }

            void RemoveInvalidTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(JadeCore::UnitAuraCheck(true, SHAMAN_SPELL_EXHAUSTION));
                targets.remove_if(JadeCore::UnitAuraCheck(true, HUNTER_SPELL_INSANITY));
                targets.remove_if(JadeCore::UnitAuraCheck(true, MAGE_SPELL_TEMPORAL_DISPLACEMENT));
            }

            void ApplyDebuff()
            {
                if (Unit* target = GetHitUnit())
                    GetCaster()->CastSpell(target, SHAMAN_SPELL_EXHAUSTION, true);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_heroism_SpellScript::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_heroism_SpellScript::RemoveInvalidTargets, EFFECT_1, TARGET_UNIT_CASTER_AREA_RAID);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_heroism_SpellScript::RemoveInvalidTargets, EFFECT_2, TARGET_UNIT_CASTER_AREA_RAID);
                AfterHit += SpellHitFn(spell_sha_heroism_SpellScript::ApplyDebuff);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_heroism_SpellScript();
        }
};

enum AncestralAwakeningProc
{
    SPELL_ANCESTRAL_AWAKENING_PROC   = 52752,
};

class spell_sha_ancestral_awakening_proc : public SpellScriptLoader
{
    public:
        spell_sha_ancestral_awakening_proc() : SpellScriptLoader("spell_sha_ancestral_awakening_proc") { }

        class spell_sha_ancestral_awakening_proc_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_ancestral_awakening_proc_SpellScript);

            bool Validate(SpellInfo const* /*SpellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_ANCESTRAL_AWAKENING_PROC))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                int32 damage = GetEffectValue();
                if (GetCaster() && GetHitUnit())
                    GetCaster()->CastCustomSpell(GetHitUnit(), SPELL_ANCESTRAL_AWAKENING_PROC, &damage, NULL, NULL, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_ancestral_awakening_proc_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_ancestral_awakening_proc_SpellScript();
        }
};

// Lava Lash - 60103
class spell_sha_lava_lash : public SpellScriptLoader
{
    public:
        spell_sha_lava_lash() : SpellScriptLoader("spell_sha_lava_lash") { }

        class spell_sha_lava_lash_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_lava_lash_SpellScript)

            int32 searingFlameAmount;

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        int32 hitDamage = GetHitDamage();
                        if (Item* weapon = _player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND))
                        {
                            // Damage increased by 40% if off-hand weapon enchanted by Frostbrand weapon.
                            if (_player->HasAura(SPELL_SHA_ITEM_S12_4P_ENHANCEMENT_BONUS) &&
                                weapon->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT) == 2)
                                AddPct(hitDamage, 40);

                            // Damage is increased by 23%(custom)/40% if your off-hand weapon is enchanted with Flametongue.
                            if (_player->GetAuraEffect(SPELL_AURA_DUMMY, SPELLFAMILY_SHAMAN, 0x200000, 0, 0) && _player->HasAura(77661))
                                AddPct(hitDamage, 21);
                            else if (_player->GetAuraEffect(SPELL_AURA_DUMMY, SPELLFAMILY_SHAMAN, 0x200000, 0, 0) && !_player->HasAura(77661))
                                AddPct(hitDamage, 40);

                            // Unleash Flame increase Lava Lash damage for 30%
                            if (_player->HasAura(73683))
                                AddPct(hitDamage, 30);

                            // Your Lava Lash ability will consume Searing Flame effect, dealing 20% increased damage for each application
                            if (AuraApplication* searingFlame = _player->GetAuraApplication(SPELL_SHA_SEARING_FLAMES_DAMAGE_DONE))
                            {
                                searingFlameAmount = searingFlame->GetBase()->GetStackAmount();
                                searingFlameAmount *= 20;
                                AddPct(hitDamage, searingFlameAmount);

                                _player->RemoveAura(SPELL_SHA_SEARING_FLAMES_DAMAGE_DONE);
                            }

                            SetHitDamage(hitDamage);

                            // Spreading your Flame shock from the target to up to four ennemies within 12 yards
                            // Effect desactivated if has Glyph of Lava Lash
                            if (!_player->HasAura(55444))
                            {
                                if (AuraPtr flameShock = target->GetAura(SPELL_SHA_FLAME_SHOCK, _player->GetGUID()))
                                {
                                    std::list<Unit*> targetList;

                                    _player->GetAttackableUnitListInRange(targetList, 12.0f);

                                    int32 flameShockDuration = flameShock->GetDuration();

                                    int32 hitTargets = 0;

                                    for (auto itr : targetList)
                                    {
                                        if (!_player->IsValidAttackTarget(itr))
                                            continue;

                                        if (itr->GetGUID() == target->GetGUID())
                                            continue;

                                        if (itr->GetGUID() == _player->GetGUID())
                                            continue;

                                        if (hitTargets >= 4)
                                            continue;

                                        // We should add flame shock with the same duration, as it on the original target
                                        _player->AddAura(SPELL_SHA_FLAME_SHOCK, itr);
                                        if (AuraPtr flameShockNew = itr->GetAura(SPELL_SHA_FLAME_SHOCK, _player->GetGUID()))
                                            flameShockNew->SetDuration(flameShockDuration);

                                        hitTargets++;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_lava_lash_SpellScript::HandleOnHit);
            }

        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_lava_lash_SpellScript();
        }
};

// Chain Heal - 1064
class spell_sha_chain_heal : public SpellScriptLoader
{
    public:
        spell_sha_chain_heal() : SpellScriptLoader("spell_sha_chain_heal") { }

        class spell_sha_chain_heal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_chain_heal_SpellScript);

            bool Load()
            {
                firstHeal = true;
                riptide = false;
                return true;
            }

            void HandleHeal(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                if (firstHeal)
                {
                    // Check if the target has Riptide
                    if (AuraEffectPtr aurEff = GetHitUnit()->GetAuraEffect(SPELL_AURA_PERIODIC_HEAL, SPELLFAMILY_SHAMAN, 0, 0, 0x10, GetCaster()->GetGUID()))
                    {
                        riptide = true;

                        // Consume it
                        // if caster has no Item - Shaman T12 Restoration 4P Bonus
                        if (!GetCaster()->HasAura(99195))
                            GetHitUnit()->RemoveAura(aurEff->GetBase());
                    }
                    firstHeal = false;
                }
                // Riptide increases the Chain Heal effect by 25%
                if (riptide)
                    SetHitHeal(GetHitHeal() * 1.25f);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_chain_heal_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
            }

            bool firstHeal;
            bool riptide;
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_chain_heal_SpellScript();
        }
};

// Glyph of Lightning Shield - 101052
class spell_sha_glyph_of_lightning_shield : public SpellScriptLoader
{
    public:
        spell_sha_glyph_of_lightning_shield() : SpellScriptLoader("spell_sha_glyph_of_lightning_shield") { }

        class spell_sha_glyph_of_lightning_shield_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_glyph_of_lightning_shield_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (GetSpellInfo()->Id == 26364 && _player->HasAura(101052))
                        _player->CastSpell(_player, 142912, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_glyph_of_lightning_shield_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_glyph_of_lightning_shield_SpellScript();
        }
};

// Glyph of Telluric Currents - 55453
class spell_sha_glyph_of_telluric_currents : public SpellScriptLoader
{
    public:
        spell_sha_glyph_of_telluric_currents() : SpellScriptLoader("spell_sha_glyph_of_telluric_currents") { }

        class spell_sha_glyph_of_telluric_currents_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_glyph_of_telluric_currents_AuraScript);

            void OnProc(constAuraEffectPtr aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                if (!GetCaster() || !eventInfo.GetDamageInfo()->GetVictim())
                    return;

                if ((eventInfo.GetDamageInfo()->GetSpellInfo() && eventInfo.GetDamageInfo()->GetSpellInfo()->Id != 403) || 
                       !GetCaster()->HasAura(SPELL_SHA_GLYPH_OF_TELLURIC_CURRENTS))
                    return;

                uint32 pct = 10;
                // Elemental and Restoration shamans have 300k mana, so 0.02% for them
                if (GetCaster()->HasAura(123099) || GetCaster()->HasAura(112858))
                    pct = 2;

                int32 restoreMana = GetCaster()->CountPctFromMaxMana(pct);

                GetCaster()->EnergizeBySpell(GetCaster(), 82987, restoreMana, POWER_MANA);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_glyph_of_telluric_currents_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_glyph_of_telluric_currents_AuraScript();
        }
};

// 51505 - Lava Burst
class spell_sha_lava_burst : public SpellScriptLoader
{
public:
	spell_sha_lava_burst() : SpellScriptLoader("spell_sha_lava_burst") { }

	class spell_sha_lava_burst_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_sha_lava_burst_SpellScript);

		void HandleBeforeCast()
		{
			if (Player* _player = GetCaster()->ToPlayer())
			{
				if (_player->HasAura(SPELL_SHA_LAVA_SURGE_AURA))
					_player->RemoveAura(SPELL_SHA_LAVA_SURGE_AURA);
			}
		}

		void Register()
		{
			BeforeCast += SpellCastFn(spell_sha_lava_burst_SpellScript::HandleBeforeCast);
		}
	};

	SpellScript* GetSpellScript() const
	{
		return new spell_sha_lava_burst_SpellScript();
	}
};

void AddSC_shaman_spell_scripts()
{
    new spell_sha_totemic_projection();
    new spell_sha_hex();
    new spell_sha_water_ascendant();
    new spell_sha_prowl();
    new spell_sha_solar_beam();
    new spell_sha_glyph_of_shamanistic_rage();
    new spell_sha_glyph_of_lakestrider();
    new spell_sha_call_of_the_elements();
    new spell_sha_conductivity();
    new spell_sha_ancestral_guidance();
    new spell_sha_echo_of_the_elements();
    new spell_sha_earthgrab();
    new spell_sha_stone_bulwark();
    new spell_sha_mail_specialization();
    new spell_sha_frozen_power();
    new spell_sha_spirit_link();
    new spell_sha_mana_tide();
    new spell_sha_tidal_waves();
    new spell_sha_fire_nova();
    new spell_sha_unleash_elements();
    new spell_sha_rolling_thunder();
    new spell_sha_fulmination();
    new spell_sha_healing_stream();
    new spell_sha_static_shock();
    new spell_sha_elemental_blast();
    new spell_sha_earthquake_tick();
    new spell_sha_earthquake();
    new spell_sha_healing_rain();
    new spell_sha_ascendance();
    new spell_sha_bloodlust();
    new spell_sha_heroism();
    new spell_sha_ancestral_awakening_proc();
    new spell_sha_lava_lash();
    new spell_sha_chain_heal();
    new spell_sha_glyph_of_lightning_shield();
    new spell_sha_glyph_of_telluric_currents();
    new spell_sha_lava_burst();
}
