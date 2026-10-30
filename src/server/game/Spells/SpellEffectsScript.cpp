/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "AccountMgr.h"
#include "AreaTrigger.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "BattlePetMgr.h"
#include "CellImpl.h"
#include "Common.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "DatabaseEnv.h"
#include "DynamicObject.h"
#include "Formulas.h"
#include "GameObject.h"
#include "GameObjectAI.h"
#include "GossipDef.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Group.h"
#include "Guild.h"
#include "GuildMgr.h"
#include "InstanceScript.h"
#include "Language.h"
#include "Log.h"
#include "MapManager.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "OutdoorPvPMgr.h"
#include "PathGenerator.h"
#include "Pet.h"
#include "Player.h"
#include "ReputationMgr.h"
#include "ScriptMgr.h"
#include "SharedDefines.h"
#include "SkillDiscovery.h"
#include "SkillExtraItems.h"
#include "SocialMgr.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellEffectMetadata.h"
#include "SpellMgr.h"
#include "SpellValidation.h"
#include "TemporarySummon.h"
#include "Totem.h"
#include "Unit.h"
#include "UpdateData.h"
#include "UpdateMask.h"
#include "Util.h"
#include "Vehicle.h"
#include "VMapFactory.h"
#include "World.h"
#include "WorldPacket.h"

void Spell::EffectDummy(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    if (!unitTarget && !gameObjTarget && !itemTarget)
        return;

    uint32 spell_id = 0;
    int32 bp = 0;
    bool triggered = true;
    SpellCastTargets targets;

    // selection by spell family
    switch (m_spellInfo->SpellFamilyName)
    {
        case SPELLFAMILY_PALADIN:
            switch (m_spellInfo->Id)
            {
                case 31789:                                 // Righteous Defense (step 1)
                {
                    // Clear targets for eff 1
                    for (std::list<TargetInfo>::iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
                        ihit->effectMask &= ~(1 << 1);

                    // not empty (checked), copy
                    Unit::AttackerSet attackers = unitTarget->getAttackers();

                    // remove invalid attackers
                    for (Unit::AttackerSet::iterator aItr = attackers.begin(); aItr != attackers.end();)
                        if (!(*aItr)->IsValidAttackTarget(m_caster))
                            attackers.erase(aItr++);
                        else
                            ++aItr;

                    // selected from list 3
                    uint32 maxTargets = std::min<uint32>(3, attackers.size());
                    for (uint32 i = 0; i < maxTargets; ++i)
                    {
                        Unit* attacker = Skyfire::Containers::SelectRandomContainerElement(attackers);
                        AddUnitTarget(attacker, 1 << 1);
                        attackers.erase(attacker);
                    }

                    // now let next effect cast spell at each target.
                    return;
                }
            }
            break;
    }

    //spells triggered by dummy effect should not miss
    if (spell_id)
    {
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spell_id);

        if (!spellInfo)
        {
            SF_LOG_ERROR("spells", "EffectDummy of spell %u: triggering unknown spell id %i\n", m_spellInfo->Id, spell_id);
            return;
        }

        targets.SetUnitTarget(unitTarget);
        Spell* spell = new Spell(m_caster, spellInfo, triggered ? TRIGGERED_FULL_MASK : TRIGGERED_NONE, m_originalCasterGUID, true);
        if (bp) spell->SetSpellValue(SPELLVALUE_BASE_POINT0, bp);
        spell->prepare(&targets);
    }

    // pet auras
    if (PetAura const* petSpell = sSpellMgr->GetPetAura(m_spellInfo->Id, effIndex))
    {
        m_caster->AddPetAura(petSpell);
        return;
    }

    // normal DB scripted effect
    SF_LOG_DEBUG("spells", "Spell ScriptStart spellid %u in EffectDummy(%u)", m_spellInfo->Id, effIndex);
    m_caster->GetMap()->ScriptsStart(sSpellScripts, uint32(m_spellInfo->Id | (effIndex << 24)), m_caster, unitTarget);

    // Script based implementation. Must be used only for not good for implementation in core spell effects
    // So called only for not proccessed cases
    if (gameObjTarget)
        sScriptMgr->OnDummyEffect(m_caster, m_spellInfo->Id, effIndex, gameObjTarget);
    else if (unitTarget && unitTarget->GetTypeId() == TypeID::TYPEID_UNIT)
        sScriptMgr->OnDummyEffect(m_caster, m_spellInfo->Id, effIndex, unitTarget->ToCreature());
    else if (itemTarget)
        sScriptMgr->OnDummyEffect(m_caster, m_spellInfo->Id, effIndex, itemTarget);
}

void Spell::EffectScriptEffect(SpellEffIndex effIndex)
{
    if (effectHandleMode != SPELL_EFFECT_HANDLE_HIT_TARGET)
        return;

    /// @todo we must implement hunter pet summon at login there (spell 6962)

    switch (m_spellInfo->SpellFamilyName)
    {
        case SPELLFAMILY_GENERIC:
        {
            switch (m_spellInfo->Id)
            {
                // Glyph of Scourge Strike
                case 69961:
                {
                    Unit::AuraEffectList const& mPeriodic = unitTarget->GetAuraEffectsByType(SPELL_AURA_PERIODIC_DAMAGE);
                    for (Unit::AuraEffectList::const_iterator i = mPeriodic.begin(); i != mPeriodic.end(); ++i)
                    {
                        AuraEffect const* aurEff = *i;
                        SpellInfo const* spellInfo = aurEff->GetSpellInfo();
                        // search our Blood Plague and Frost Fever on target
                        if (spellInfo->SpellFamilyName == SPELLFAMILY_DEATHKNIGHT && spellInfo->SpellFamilyFlags[2] & 0x2 &&
                            aurEff->GetCasterGUID() == m_caster->GetGUID())
                        {
                            uint32 countMin = aurEff->GetBase()->GetMaxDuration();
                            uint32 countMax = spellInfo->GetMaxDuration();

                            // this Glyph
                            countMax += 9000;
                            // talent Epidemic
                            if (AuraEffect const* epidemic = m_caster->GetAuraEffect(SPELL_AURA_ADD_FLAT_MODIFIER, SPELLFAMILY_DEATHKNIGHT, 234, EFFECT_0))
                                countMax += epidemic->GetAmount();

                            if (countMin < countMax)
                            {
                                aurEff->GetBase()->SetDuration(aurEff->GetBase()->GetDuration() + 3000);
                                aurEff->GetBase()->SetMaxDuration(countMin + 3000);
                            }
                        }
                    }
                    return;
                }
                case 45204: // Clone Me!
                    m_caster->CastSpell(unitTarget, damage, true);
                    break;
                case 55693:                                 // Remove Collapsing Cave Aura
                    if (!unitTarget)
                        return;
                    unitTarget->RemoveAurasDueToSpell(m_spellInfo->Effects[effIndex].CalcValue());
                    break;
                    // Bending Shinbone
                case 8856:
                {
                    if (!itemTarget && m_caster->GetTypeId() != TypeID::TYPEID_PLAYER)
                        return;

                    uint32 spell_id = Skyfire::Spells::GetBendingShinboneTriggerSpellId(roll_chance_i(20));

                    m_caster->CastSpell(m_caster, spell_id, true, NULL);
                    return;
                }
                // Brittle Armor - need remove one 24575 Brittle Armor aura
                case 24590:
                    unitTarget->RemoveAuraFromStack(Skyfire::Spells::GetScriptEffectStackRemovalAuraSpellId(m_spellInfo->Id));
                    return;
                    // Mercurial Shield - need remove one 26464 Mercurial Shield aura
                case 26465:
                    unitTarget->RemoveAuraFromStack(Skyfire::Spells::GetScriptEffectStackRemovalAuraSpellId(m_spellInfo->Id));
                    return;
                    // Shadow Flame (All script effects, not just end ones to prevent player from dodging the last triggered spell)
                case 22539:
                case 22972:
                case 22975:
                case 22976:
                case 22977:
                case 22978:
                case 22979:
                case 22980:
                case 22981:
                case 22982:
                case 22983:
                case 22984:
                case 22985:
                {
                    if (!unitTarget || !unitTarget->IsAlive())
                        return;

                    Skyfire::Spells::ShadowFlameScriptRule const* shadowFlameRule =
                        Skyfire::Spells::GetShadowFlameScriptRule(m_spellInfo->Id);
                    if (!shadowFlameRule)
                        return;

                    // Onyxia Scale Cloak
                    if (unitTarget->HasAura(shadowFlameRule->ProtectionAuraSpellId))
                        return;

                    // Shadow Flame
                    m_caster->CastSpell(unitTarget, shadowFlameRule->TriggerSpellId, true);
                    return;
                }
                // Decimate
                case 28374:
                case 54426:
                    if (unitTarget)
                    {
                        int32 damage = int32(unitTarget->GetHealth()) - int32(unitTarget->CountPctFromMaxHealth(5));
                        if (damage > 0)
                        {
                            uint32 decimateTriggerSpellId =
                                Skyfire::Spells::GetDecimateScriptTriggerSpellId(m_spellInfo->Id);
                            m_caster->CastCustomSpell(decimateTriggerSpellId, SPELLVALUE_BASE_POINT0, damage, unitTarget);
                        }
                    }
                    return;
                    // Mirren's Drinking Hat
                case 29830:
                {
                    uint32 item = 0;
                    item = Skyfire::Spells::GetMirrensDrinkingHatItemId(std::rand() % 6 + 1);
                    if (item)
                        DoCreateItem(effIndex, item);
                    break;
                }
                case 20589: // Escape artist
                {
                    // Removes snares and roots.
                    unitTarget->RemoveMovementImpairingAuras();
                    break;
                }
                // Plant Warmaul Ogre Banner
                case 32307:
                    if (Player* caster = m_caster->ToPlayer())
                    {
                        caster->RewardPlayerAndGroupAtEvent(Skyfire::Spells::GetPlantWarmaulOgreBannerEventId(), unitTarget);
                        if (Creature* target = unitTarget->ToCreature())
                        {
                            target->setDeathState(DeathState::CORPSE);
                            target->RemoveCorpse();
                        }
                    }
                    break;
                    // Mug Transformation
                case 41931:
                {
                    if (m_caster->GetTypeId() != TypeID::TYPEID_PLAYER)
                        return;

                    uint8 bag = 19;
                    uint8 slot = 0;
                    Item* item = NULL;

                    while (bag) // 256 = 0 due to var type
                    {
                        item = m_caster->ToPlayer()->GetItemByPos(bag, slot);
                        if (item && item->GetEntry() == Skyfire::Spells::GetMugTransformationItemId())
                            break;

                        ++slot;
                        if (slot == 39)
                        {
                            slot = 0;
                            ++bag;
                        }
                    }
                    if (bag)
                    {
                        if (m_caster->ToPlayer()->GetItemByPos(bag, slot)->GetCount() == 1) m_caster->ToPlayer()->RemoveItem(bag, slot, true);
                        else m_caster->ToPlayer()->GetItemByPos(bag, slot)->SetCount(m_caster->ToPlayer()->GetItemByPos(bag, slot)->GetCount() - 1);
                        // Spell 42518 (Braufest - Gratisprobe des Braufest herstellen)
                        m_caster->CastSpell(m_caster, Skyfire::Spells::GetBrewfestSampleSpellId(), true);
                        return;
                    }
                    break;
                }
                // Brutallus - Burn
                case 45141:
                case 45151:
                {
                    //Workaround for Range ... should be global for every ScriptEffect
                    float radius = m_spellInfo->Effects[effIndex].CalcRadius();
                    uint32 rangeAuraSpellId = Skyfire::Spells::GetBrutallusBurnRangeAuraSpellId();
                    if (unitTarget && unitTarget->GetTypeId() == TypeID::TYPEID_PLAYER &&
                        unitTarget->GetDistance(m_caster) >= radius && !unitTarget->HasAura(rangeAuraSpellId) &&
                        unitTarget != m_caster)
                        unitTarget->CastSpell(unitTarget, rangeAuraSpellId, true);

                    break;
                }
                // Goblin Weather Machine
                case 46203:
                {
                    if (!unitTarget)
                        return;

                    uint32 spellId = Skyfire::Spells::GetGoblinWeatherMachineSpellId(rand() % 4);
                    unitTarget->CastSpell(unitTarget, spellId, true);
                    break;
                }
                // 5, 000 Gold
                case 46642:
                {
                    if (!unitTarget || unitTarget->GetTypeId() != TypeID::TYPEID_PLAYER)
                        return;

                    unitTarget->ToPlayer()->ModifyMoney(Skyfire::Spells::GetGoldScriptAmount() * GOLD);

                    break;
                }
                // Roll Dice - Decahedral Dwarven Dice
                case 47770:
                {
                    char buf[128];
                    const char* gender = "his";
                    if (m_caster->getGender() > 0)
                        gender = "her";
                    snprintf(buf, sizeof(buf), "%s rubs %s [Decahedral Dwarven Dice] between %s hands and rolls. One %u and one %u.", m_caster->GetName().c_str(), gender, gender, urand(1, 10), urand(1, 10));
                    m_caster->MonsterTextEmote(buf, 0);
                    break;
                }
                // Roll 'dem Bones - Worn Troll Dice
                case 47776:
                {
                    char buf[128];
                    const char* gender = "his";
                    if (m_caster->getGender() > 0)
                        gender = "her";
                    snprintf(buf, sizeof(buf), "%s causually tosses %s [Worn Troll Dice]. One %u and one %u.", m_caster->GetName().c_str(), gender, urand(1, 6), urand(1, 6));
                    m_caster->MonsterTextEmote(buf, 0);
                    break;
                }
                // Emblazon Runeblade
                case 51770:
                {
                    m_caster->CastSpell(unitTarget, Skyfire::Spells::GetEmblazonRunebladeTriggerSpellId(), false);
                    return;
                }

                // Death Knight Initiate Visual
                case 51519:
                {
                    if (!unitTarget || unitTarget->GetTypeId() != TypeID::TYPEID_UNIT)
                        return;

                    uint32 iTmpSpellId = Skyfire::Spells::GetDeathKnightInitiateVisualSpellId(unitTarget->GetDisplayId());
                    if (!iTmpSpellId)
                        return;

                    unitTarget->CastSpell(unitTarget, iTmpSpellId, true);
                    Creature* npc = unitTarget->ToCreature();
                    npc->LoadEquipment();
                    return;
                }
                // Deathbolt from Thalgran Blightbringer
                // reflected by Freya's Ward
                // Retribution by Sevenfold Retribution
                case 51854:
                {
                    if (!unitTarget)
                        return;
                    if (unitTarget->HasAura(Skyfire::Spells::GetDeathboltWardAuraSpellId()))
                        unitTarget->CastSpell(m_caster, Skyfire::Spells::GetDeathboltReflectedSpellId(), true);
                    else
                        m_caster->CastSpell(unitTarget, Skyfire::Spells::GetDeathboltDirectSpellId(), true);
                    break;
                }
                // Summon Ghouls On Scarlet Crusade
                case 51904:
                {
                    if (!m_targets.HasDst())
                        return;

                    float x, y, z;
                    float radius = m_spellInfo->Effects[effIndex].CalcRadius();
                    for (uint8 i = 0; i < 15; ++i)
                    {
                        m_caster->GetRandomPoint(*destTarget, radius, x, y, z);
                        m_caster->CastSpell(x, y, z, Skyfire::Spells::GetSummonGhoulsOnScarletCrusadeSpellId(), true);
                    }
                    break;
                }
                case 52173: // Coyote Spirit Despawn
                case 60243: // Blood Parrot Despawn
                    if (Skyfire::Spells::IsScriptDespawnSpell(m_spellInfo->Id) &&
                        unitTarget->GetTypeId() == TypeID::TYPEID_UNIT && unitTarget->ToCreature()->IsSummon())
                        unitTarget->ToTempSummon()->UnSummon();
                    return;
                case 52479: // Gift of the Harvester
                    if (unitTarget && m_originalCaster)
                    {
                        uint32 spellId = urand(0, 1) ? damage : Skyfire::Spells::GetGiftOfTheHarvesterFallbackSpellId();
                        m_originalCaster->CastSpell(unitTarget, spellId, true);
                    }
                    return;
                case 53110: // Devour Humanoid
                    if (unitTarget)
                        unitTarget->CastSpell(m_caster, damage, true);
                    return;
                case 57347: // Retrieving (Wintergrasp RP-GG pickup spell)
                {
                    if (!unitTarget || unitTarget->GetTypeId() != TypeID::TYPEID_UNIT || m_caster->GetTypeId() != TypeID::TYPEID_PLAYER)
                        return;

                    unitTarget->ToCreature()->DespawnOrUnsummon();

                    return;
                }
                case 57349: // Drop RP-GG (Wintergrasp RP-GG at death drop spell)
                {
                    if (m_caster->GetTypeId() != TypeID::TYPEID_PLAYER)
                        return;

                    // Delete item from inventory at death
                    m_caster->ToPlayer()->DestroyItemCount(damage, Skyfire::Spells::GetWintergraspRpggDropItemCount(), true);

                    return;
                }
                case 58418:                                 // Portal to Orgrimmar
                case 58420:                                 // Portal to Stormwind
                {
                    if (!unitTarget || unitTarget->GetTypeId() != TypeID::TYPEID_PLAYER || effIndex != 0)
                        return;

                    uint32 spellID = m_spellInfo->Effects[EFFECT_0].CalcValue();
                    uint32 questID = m_spellInfo->Effects[EFFECT_1].CalcValue();

                    if (unitTarget->ToPlayer()->GetQuestStatus(questID) == QUEST_STATUS_COMPLETE)
                        unitTarget->CastSpell(unitTarget, spellID, true);

                    return;
                }
                case 58983: // Big Blizzard Bear
                {
                    if (!unitTarget || unitTarget->GetTypeId() != TypeID::TYPEID_PLAYER)
                        return;

                    // Prevent stacking of mounts and client crashes upon dismounting
                    unitTarget->RemoveAurasByType(SPELL_AURA_MOUNTED);

                    // Triggered spell id dependent on riding skill
                    if (uint16 skillval = unitTarget->ToPlayer()->GetSkillValue(SKILL_RIDING))
                    {
                        if (skillval >= 150)
                            unitTarget->CastSpell(unitTarget, Skyfire::Spells::GetBigBlizzardBearMountSpellId(skillval), true);
                        else
                            unitTarget->CastSpell(unitTarget, Skyfire::Spells::GetBigBlizzardBearMountSpellId(skillval), true);
                    }
                    return;
                }
                case 59317:                                 // Teleporting
                {
                    if (!unitTarget || unitTarget->GetTypeId() != TypeID::TYPEID_PLAYER)
                        return;

                    // return from top
                    unitTarget->CastSpell(unitTarget,
                        Skyfire::Spells::GetTeleportingScriptSpellId(unitTarget->ToPlayer()->GetAreaId()), true);

                    return;
                }
                case 62482: // Grab Crate
                {
                    if (unitTarget)
                    {
                        if (Unit* seat = m_caster->GetVehicleBase())
                        {
                            if (Unit* parent = seat->GetVehicleBase())
                            {
                                /// @todo a hack, range = 11, should after some time cast, otherwise too far
                                m_caster->CastSpell(parent, Skyfire::Spells::GetGrabCrateParentSpellId(), true);
                                unitTarget->CastSpell(parent, m_spellInfo->Effects[EFFECT_0].CalcValue());
                            }
                        }
                    }
                    return;
                }
                case 60123: // Lightwell
                {
                    if (m_caster->GetTypeId() != TypeID::TYPEID_UNIT || !m_caster->ToCreature()->IsSummon())
                        return;

                    uint32 spell_heal;

                    spell_heal = Skyfire::Spells::GetLightwellHealSpellId(m_caster->GetEntry());
                    if (!spell_heal)
                    {
                        SF_LOG_ERROR("spells", "Unknown Lightwell spell caster %u", m_caster->GetEntry());
                        return;
                    }

                    // proc a spellcast
                    if (Aura* chargesAura = m_caster->GetAura(Skyfire::Spells::GetLightwellChargesAuraSpellId()))
                    {
                        m_caster->CastSpell(unitTarget, spell_heal, true, NULL, NULL, m_caster->ToTempSummon()->GetSummonerGUID());
                        if (chargesAura->ModCharges(-1))
                            m_caster->ToTempSummon()->UnSummon();
                    }

                    return;
                }
                case 45668:                                 // Ultra-Advanced Proto-Typical Shortening Blaster
                {
                    if (!unitTarget || unitTarget->GetTypeId() != TypeID::TYPEID_UNIT)
                        return;

                    if (roll_chance_i(50))                  // chance unknown, using 50
                        return;

                    Skyfire::Spells::ProtoTypicalShorteningBlasterSpells const* casterBlasterSpells =
                        Skyfire::Spells::GetProtoTypicalShorteningBlasterSpells(std::rand() % 4);
                    Skyfire::Spells::ProtoTypicalShorteningBlasterSpells const* targetBlasterSpells =
                        Skyfire::Spells::GetProtoTypicalShorteningBlasterSpells(std::rand() % 4);
                    if (casterBlasterSpells && targetBlasterSpells)
                    {
                        m_caster->CastSpell(m_caster, casterBlasterSpells->CasterSpellId, true);
                        unitTarget->CastSpell(unitTarget, targetBlasterSpells->TargetSpellId, true);
                    }
                    break;
                }
            }
            break;
        }
        case SPELLFAMILY_PALADIN:
        {
            // Judgement (seal trigger)
            if (m_spellInfo->GetCategory() == SPELLCATEGORY_JUDGEMENT)
            {
                if (!unitTarget || !unitTarget->IsAlive())
                    return;
                uint32 spellId1 = 0;
                uint32 spellId2 = 0;

                spellId1 = Skyfire::Spells::GetJudgementSealTriggerSpellId(m_spellInfo->Id);
                if (!spellId1)
                {
                    SF_LOG_ERROR("spells", "Unsupported Judgement (seal trigger) spell (Id: %u) in Spell::EffectScriptEffect", m_spellInfo->Id);
                    return;
                }
                // all seals have aura dummy in 2 effect
                Unit::AuraApplicationMap& sealAuras = m_caster->GetAppliedAuras();
                for (Unit::AuraApplicationMap::iterator iter = sealAuras.begin(); iter != sealAuras.end();)
                {
                    Aura* aura = iter->second->GetBase();
                    if (aura->GetSpellInfo()->GetSpellSpecific() == SPELL_SPECIFIC_SEAL)
                    {
                        if (AuraEffect* aureff = aura->GetEffect(2))
                            if (aureff->GetAuraType() == SPELL_AURA_DUMMY)
                            {
                                if (sSpellMgr->GetSpellInfo(aureff->GetAmount()))
                                    spellId2 = aureff->GetAmount();
                                break;
                            }
                        if (!spellId2)
                        {
                            spellId2 = Skyfire::Spells::GetJudgementFallbackSealTriggerSpellId(iter->first);
                        }
                        break;
                    }
                    else
                        ++iter;
                }
                if (spellId1)
                    m_caster->CastSpell(unitTarget, spellId1, true);
                if (spellId2)
                    m_caster->CastSpell(unitTarget, spellId2, true);
                return;
            }
            break;
        }
        case SPELLFAMILY_DEATHKNIGHT:
        {
            // Pestilence
            if (m_spellInfo->SpellFamilyFlags[1] & 0x10000)
            {
                // Get diseases on target of spell
                if (m_targets.GetUnitTarget() && (m_targets.GetUnitTarget() != unitTarget))
                {
                    // And spread them on target
                    // Blood Plague
                    if (m_targets.GetUnitTarget()->GetAura(Skyfire::Spells::GetPestilenceBloodPlagueSpellId()))
                        m_caster->CastSpell(unitTarget, Skyfire::Spells::GetPestilenceBloodPlagueSpellId(), true);
                    // Frost Fever
                    if (m_targets.GetUnitTarget()->GetAura(Skyfire::Spells::GetPestilenceFrostFeverSpellId()))
                        m_caster->CastSpell(unitTarget, Skyfire::Spells::GetPestilenceFrostFeverSpellId(), true);
                }
            }
            break;
        }
        case SPELLFAMILY_WARRIOR:
        {
            switch (m_spellInfo->Id)
            {
                case 107574: // Avatar
                {
                    // Removes snares and roots.
                    unitTarget->RemoveMovementImpairingAuras();
                    break;
                }
                break;
            }
            break;
        }
        case SPELLFAMILY_WARLOCK:
        {
            switch (m_spellInfo->Id)
            {
                case 6201: //Create Healthstone
                case 34130: //Create Healthstone
                {
                    m_caster->CastSpell(unitTarget,
                        Skyfire::Spells::GetCreateHealthstoneTriggeredSpellId(m_spellInfo->Id), true);
                    break;
                }
                break;
            }
            break;
        }
    }

    // normal DB scripted effect
    SF_LOG_DEBUG("spells", "Spell ScriptStart spellid %u in EffectScriptEffect(%u)", m_spellInfo->Id, effIndex);
    m_caster->GetMap()->ScriptsStart(sSpellScripts, uint32(m_spellInfo->Id | (effIndex << 24)), m_caster, unitTarget);
}
