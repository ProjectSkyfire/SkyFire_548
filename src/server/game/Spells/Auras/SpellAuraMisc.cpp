/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include "Battleground.h"
#include "CellImpl.h"
#include "Common.h"
#include "Formulas.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "OutdoorPvPMgr.h"
#include "Pet.h"
#include "Player.h"
#include "ReputationMgr.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellAuraMetadata.h"
#include "SpellMgr.h"
#include "SpellValidation.h"
#include "Unit.h"
#include "Util.h"
#include "Vehicle.h"
#include "WeatherMgr.h"
#include "WorldPacket.h"

class Aura;

void AuraEffect::HandleArenaPreparation(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
        target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PREPARATION);
    else
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (target->HasAuraType(GetAuraType()))
            return;
        target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PREPARATION);
    }
}

void AuraEffect::HandleAuraRetainComboPoints(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (target->GetTypeId() != TypeID::TYPEID_PLAYER)
        return;

    // combo points was added in SPELL_EFFECT_ADD_COMBO_POINTS handler
    // remove only if aura expire by time (in case combo points amount change aura removed without combo points lost)
    if (!(apply) && GetBase()->GetDuration() == 0 && target->ToPlayer()->GetComboTarget())
        if (Unit* unit = ObjectAccessor::GetUnit(*target, target->ToPlayer()->GetComboTarget()))
            target->ToPlayer()->AddComboPoints(unit, -GetAmount());
}

/*********************************************************/
/***                    OTHERS                         ***/
/*********************************************************/

void AuraEffect::HandleAuraDummy(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_REAPPLY)))
        return;

    Unit* target = aurApp->GetTarget();

    Unit* caster = GetCaster();

    if (mode & AURA_EFFECT_HANDLE_REAL)
    {
        // pet auras
        if (PetAura const* petSpell = sSpellMgr->GetPetAura(GetId(), m_effIndex))
        {
            if (apply)
                target->AddPetAura(petSpell);
            else
                target->RemovePetAura(petSpell);
        }
    }

    if (mode & (AURA_EFFECT_HANDLE_REAL | AURA_EFFECT_HANDLE_REAPPLY))
    {
        // AT APPLY
        if (apply)
        {
            switch (GetId())
            {
                case 1515:                                      // Tame beast
                    // FIX_ME: this is 2.0.12 threat effect replaced in 2.1.x by dummy aura, must be checked for correctness
                    if (caster && target->CanHaveThreatList())
                        target->AddThreat(caster, 10.0f);
                    break;
                case 13139:                                     // net-o-matic
                    // root to self part of (root_target->charge->root_self sequence
                    if (caster)
                        caster->CastSpell(caster, Skyfire::Spells::GetNetOMaticRootSelfSpellId(), true, NULL, this);
                    break;
                case 37096:                                     // Blood Elf Illusion
                {
                    if (caster)
                    {
                        uint32 disguiseSpellId =
                            Skyfire::Spells::GetBloodElfIllusionDisguiseSpellId(GetId(), caster->getGender());
                        if (disguiseSpellId)
                            caster->CastSpell(target, disguiseSpellId, true, NULL, this);
                    }
                    break;
                }
                case 39850:                                     // Rocket Blast
                    if (roll_chance_i(20))                       // backfire stun
                        target->CastSpell(target, Skyfire::Spells::GetAuraDummyTriggeredSpellId(GetId()), true, NULL, this);
                    break;
                case 43873:                                     // Headless Horseman Laugh
                    target->PlayDistanceSound(11965, target->ToPlayer());
                    break;
                case 46354:                                     // Blood Elf Illusion
                    if (caster)
                    {
                        uint32 disguiseSpellId =
                            Skyfire::Spells::GetBloodElfIllusionDisguiseSpellId(GetId(), caster->getGender());
                        if (disguiseSpellId)
                            caster->CastSpell(target, disguiseSpellId, true, NULL, this);
                    }
                    break;
                case 46361:                                     // Reinforced Net
                    if (caster)
                        target->GetMotionMaster()->MoveFall();
                    break;
                case 51701: // Honor Among Thieves
                    if (target->GetTypeId() == TypeID::TYPEID_PLAYER)
                        if (Unit* spellTarget = ObjectAccessor::GetUnit(*target, target->ToPlayer()->GetComboTarget()))
                            target->CastSpell(spellTarget, Skyfire::Spells::GetAuraDummyTriggeredSpellId(GetId()), true);
                    break;
                case 71563:
                    if (Aura* newAura = target->AddAura(Skyfire::Spells::GetAuraDummyTriggeredSpellId(GetId()), target))
                        newAura->SetStackAmount(newAura->GetSpellInfo()->StackAmount);
                    break;
            }
        }
        // AT REMOVE
        else
        {
            if ((GetSpellInfo()->IsQuestTame()) && caster && caster->IsAlive() && target->IsAlive())
            {
                uint32 finalSpellId = Skyfire::Spells::GetQuestTameFinalSpellId(GetId());

                if (finalSpellId)
                    caster->CastSpell(target, finalSpellId, true, NULL, this);
            }

            switch (m_spellInfo->SpellFamilyName)
            {
                case SPELLFAMILY_GENERIC:
                    switch (GetId())
                    {
                        case 2584: // Waiting to Resurrect
                            // Waiting to resurrect spell cancel, we must remove player from resurrect queue
                            if (target->GetTypeId() == TypeID::TYPEID_PLAYER)
                            {
                                if (Battleground* bg = target->ToPlayer()->GetBattleground())
                                    bg->RemovePlayerFromResurrectQueue(target->GetGUID());
                                if (Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(target->GetZoneId()))
                                    bf->RemovePlayerFromResurrectQueue(target->GetGUID());
                            }
                            break;
                        case 36730:                                     // Flame Strike
                        {
                            target->CastSpell(target,
                                Skyfire::Spells::GetAuraRemoveTriggeredSpellId(GetId(), target->GetMap()->IsHeroic()),
                                true, NULL, this);
                            break;
                        }
                        case 44191:                                     // Flame Strike
                        {
                            if (target->GetMap()->IsInstance())
                            {
                                uint32 spellId = Skyfire::Spells::GetAuraRemoveTriggeredSpellId(GetId(),
                                    target->GetMap()->IsHeroic());

                                target->CastSpell(target, spellId, true, NULL, this);
                            }
                            break;
                        }
                        case 43681: // Inactive
                        {
                            if (target->GetTypeId() != TypeID::TYPEID_PLAYER || aurApp->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
                                return;

                            if (target->GetMap()->IsBattleground())
                                target->ToPlayer()->LeaveBattleground();
                            break;
                        }
                        case 42783: // Wrath of the Astromancer
                            target->CastSpell(target, GetAmount(), true, NULL, this);
                            break;
                        case 46308: // Burning Winds casted only at creatures at spawn
                            target->CastSpell(target,
                                Skyfire::Spells::GetAuraRemoveTriggeredSpellId(GetId(), target->GetMap()->IsHeroic()),
                                true, NULL, this);
                            break;
                        case 52172:  // Coyote Spirit Despawn Aura
                        case 60244:  // Blood Parrot Despawn Aura
                        case 130067: // Terracotta Warrior Despawn Aura
                        case 130070: // Whirlwind of Blades Despawn Aura
                        case 130108: // Martar Despawn Aura
                        case 130112: // Feverbite Despawn Aura
                        case 130119: // Kidnapped Puppies Despawn Aura
                        case 130156: // Tranquil Sprout Despawn Aura
                        case 130484: // Quilen Statuette Despawn Aura
                        case 148596: // Barnacle Crew Despawn Aura
                            if (Skyfire::Spells::IsDespawnOnRemoveAura(GetId()))
                                target->CastSpell((Unit*)NULL, GetAmount(), true, NULL, this);
                            break;
                        case 91604: // Restricted Flight Area
                            if (aurApp->GetRemoveMode() == AURA_REMOVE_BY_EXPIRE)
                                target->CastSpell(target,
                                    Skyfire::Spells::GetAuraRemoveTriggeredSpellId(GetId(), target->GetMap()->IsHeroic()),
                                    true);
                            break;
                    }
                    break;
                case SPELLFAMILY_DEATHKNIGHT:
                    // Summon Gargoyle (Dismiss Gargoyle at remove)
                    if (GetId() == 61777)
                        target->CastSpell(target, GetAmount(), true);
                    break;
                default:
                    break;
            }
        }
    }

    // AT APPLY & REMOVE

    switch (m_spellInfo->SpellFamilyName)
    {
        case SPELLFAMILY_GENERIC:
        {
            if (!(mode & AURA_EFFECT_HANDLE_REAL))
                break;
            switch (GetId())
            {
                // Recently Bandaged
                case 11196:
                    target->ApplySpellImmune(GetId(), IMMUNITY_MECHANIC, GetMiscValue(), apply);
                    break;
                    // Unstable Power
                case 24658:
                {
                    uint32 spellId = Skyfire::Spells::GetStackingAuraTriggerSpellId(GetId());
                    if (apply && caster)
                    {
                        SpellInfo const* spell = sSpellMgr->GetSpellInfo(spellId);

                        for (uint32 i = 0; i < spell->StackAmount; ++i)
                            caster->CastSpell(target, spell->Id, true, NULL, NULL, GetCasterGUID());
                        break;
                    }
                    target->RemoveAurasDueToSpell(spellId);
                    break;
                }
                // Restless Strength
                case 24661:
                {
                    uint32 spellId = Skyfire::Spells::GetStackingAuraTriggerSpellId(GetId());
                    if (apply && caster)
                    {
                        SpellInfo const* spell = sSpellMgr->GetSpellInfo(spellId);
                        for (uint32 i = 0; i < spell->StackAmount; ++i)
                            caster->CastSpell(target, spell->Id, true, NULL, NULL, GetCasterGUID());
                        break;
                    }
                    target->RemoveAurasDueToSpell(spellId);
                    break;
                }
                // Tag Murloc
                case 30877:
                {
                    // Tag/untag Blacksilt Scout
                    target->SetEntry(Skyfire::Spells::GetTaggedMurlocEntry(apply));
                    break;
                }
                // LK Intro VO (1)
                case 58204:
                    if (target->GetTypeId() == TypeID::TYPEID_PLAYER)
                    {
                        // Play part 1
                        if (apply)
                            target->PlayDirectSound(Skyfire::Spells::GetLichKingIntroApplySoundId(GetId()), target->ToPlayer());
                        // continue in 58205
                        else
                            target->CastSpell(target, Skyfire::Spells::GetLichKingIntroContinueSpellId(GetId()), true);
                    }
                    break;
                    // LK Intro VO (2)
                case 58205:
                    if (target->GetTypeId() == TypeID::TYPEID_PLAYER)
                    {
                        // Play part 2
                        if (apply)
                            target->PlayDirectSound(Skyfire::Spells::GetLichKingIntroApplySoundId(GetId()), target->ToPlayer());
                        // Play part 3
                        else
                            target->PlayDirectSound(Skyfire::Spells::GetLichKingIntroRemoveSoundId(GetId()), target->ToPlayer());
                    }
                    break;
                case 62061: // Festive Holiday Mount
                    if (target->HasAuraType(SPELL_AURA_MOUNTED))
                    {
                        uint32 creatureEntry = 0;
                        if (apply)
                        {
                            if (target->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED))
                                creatureEntry = Skyfire::Spells::GetFestiveHolidayMountCreatureEntry(true);
                            else
                                creatureEntry = Skyfire::Spells::GetFestiveHolidayMountCreatureEntry(false);
                        }
                        else
                            creatureEntry = target->GetAuraEffectsByType(SPELL_AURA_MOUNTED).front()->GetMiscValue();

                        if (CreatureTemplate const* creatureInfo = sObjectMgr->GetCreatureTemplate(creatureEntry))
                        {
                            uint32 displayID = ObjectMgr::ChooseDisplayId(creatureInfo);
                            sObjectMgr->GetCreatureModelRandomGender(&displayID);

                            target->SetUInt32Value(UNIT_FIELD_MOUNT_DISPLAY_ID, displayID);
                        }
                    }
                    break;
                default:
                {
                    Skyfire::Spells::ChampioningAuraInfo const* championingAura =
                        Skyfire::Spells::GetChampioningAuraInfo(GetId());
                    if (!championingAura || !caster || caster->GetTypeId() != TypeID::TYPEID_PLAYER)
                        break;

                    uint32 factionId = apply ? championingAura->FactionId : 0;
                    uint8 championingType = apply ? championingAura->ChampioningType : 0;
                    caster->ToPlayer()->SetChampioningFaction(factionId);
                    caster->ToPlayer()->SetChampioningType(championingType);
                    break;
                }
            }
            break;
        }
        default:
            break;
    }
}

void AuraEffect::HandleChannelDeathItem(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    if (apply || aurApp->GetRemoveMode() != AURA_REMOVE_BY_DEATH)
        return;

    Unit* caster = GetCaster();

    if (!caster || caster->GetTypeId() != TypeID::TYPEID_PLAYER)
        return;

    Player* plCaster = caster->ToPlayer();

    // Item amount
    if (GetAmount() <= 0)
        return;

    if (GetSpellInfo()->Effects[m_effIndex].ItemType == 0)
        return;

    //Adding items
    uint32 noSpaceForCount = 0;
    uint32 count = m_amount;

    ItemPosCountVec dest;
    InventoryResult msg = plCaster->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, GetSpellInfo()->Effects[m_effIndex].ItemType, count, &noSpaceForCount);
    if (msg != EQUIP_ERR_OK)
    {
        count -= noSpaceForCount;
        plCaster->SendEquipError(msg, NULL, NULL, GetSpellInfo()->Effects[m_effIndex].ItemType);
        if (count == 0)
            return;
    }

    Item* newitem = plCaster->StoreNewItem(dest, GetSpellInfo()->Effects[m_effIndex].ItemType, true);
    if (!newitem)
    {
        plCaster->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, NULL, NULL);
        return;
    }
    plCaster->SendNewItem(newitem, count, true, true);
}

void AuraEffect::HandleBindSight(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    Unit* caster = GetCaster();

    if (!caster || caster->GetTypeId() != TypeID::TYPEID_PLAYER)
        return;

    caster->ToPlayer()->SetViewpoint(target, apply);
}

void AuraEffect::HandleForceReaction(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    Player* player = target->ToPlayer();
    if (!player)
        return;

    uint32 factionId = GetMiscValue();
    ReputationRank factionRank = ReputationRank(m_amount);

    player->GetReputationMgr().ApplyForceReaction(factionId, factionRank, apply);
    player->GetReputationMgr().SendForceReactions();

    // stop fighting if at apply forced rank friendly or at remove real rank friendly
    if ((apply && factionRank >= REP_FRIENDLY) || (!apply && player->GetReputationRank(factionId) >= REP_FRIENDLY))
        player->StopAttackFaction(factionId);
}

void AuraEffect::HandleAuraEmpathy(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();
    if (!apply)
    {
        // do not remove unit flag if there are more than this auraEffect of that kind on unit on unit
        if (target->HasAuraType(GetAuraType()))
            return;
    }

    if (target->GetCreatureType() == CREATURE_TYPE_BEAST)
        target->ApplyModUInt32Value(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_SPECIALINFO, apply);
}

void AuraEffect::HandleAuraModFaction(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
    {
        target->setFaction(GetMiscValue());
        if (target->GetTypeId() == TypeID::TYPEID_PLAYER)
            target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);
    }
    else
    {
        target->RestoreFaction();
        if (target->GetTypeId() == TypeID::TYPEID_PLAYER)
            target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);
    }
}

void AuraEffect::HandleComprehendLanguage(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_SEND_FOR_CLIENT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
        target->SetFlag(UNIT_FIELD_FLAGS2, UNIT_FLAG2_COMPREHEND_LANG);
    else
    {
        if (target->HasAuraType(GetAuraType()))
            return;

        target->RemoveFlag(UNIT_FIELD_FLAGS2, UNIT_FLAG2_COMPREHEND_LANG);
    }
}

void AuraEffect::HandleAuraConvertRune(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    Player* player = target->ToPlayer();
    if (!player)
        return;

    if (player->getClass() != CLASS_DEATH_KNIGHT)
        return;

    uint32 runes = m_amount;
    // convert number of runes specified in aura amount of rune type in miscvalue to runetype in miscvalueb
    if (apply)
    {
        for (uint32 i = 0; i < MAX_RUNES && runes; ++i)
        {
            if (RuneType(GetMiscValue()) != player->GetCurrentRune(i))
                continue;
            if (!player->GetRuneCooldown(i))
            {
                player->AddRuneByAuraEffect(i, RuneType(GetMiscValueB()), this);
                --runes;
            }
        }
    }
    else
        player->RemoveRunesByAuraEffect(this);
}

void AuraEffect::HandleAuraLinked(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    Unit* target = aurApp->GetTarget();

    uint32 triggeredSpellId = sSpellMgr->GetSpellIdForDifficulty(m_spellInfo->Effects[m_effIndex].TriggerSpell, target);
    SpellInfo const* triggeredSpellInfo = sSpellMgr->GetSpellInfo(triggeredSpellId);
    if (!triggeredSpellInfo)
        return;

    if (mode & AURA_EFFECT_HANDLE_REAL)
    {
        if (apply)
        {
            Unit* caster = triggeredSpellInfo->NeedsToBeTriggeredByCaster(m_spellInfo) ? GetCaster() : target;

            if (!caster)
                return;
            // If amount avalible cast with basepoints (Crypt Fever for example)
            if (GetAmount())
                caster->CastCustomSpell(target, triggeredSpellId, &m_amount, NULL, NULL, true, NULL, this);
            else
                caster->CastSpell(target, triggeredSpellId, true, NULL, this);
        }
        else
        {
            uint64 casterGUID = triggeredSpellInfo->NeedsToBeTriggeredByCaster(m_spellInfo) ? GetCasterGUID() : target->GetGUID();
            target->RemoveAura(triggeredSpellId, casterGUID, 0, aurApp->GetRemoveMode());
        }
    }
    else if (mode & AURA_EFFECT_HANDLE_REAPPLY && apply)
    {
        uint64 casterGUID = triggeredSpellInfo->NeedsToBeTriggeredByCaster(m_spellInfo) ? GetCasterGUID() : target->GetGUID();
        // change the stack amount to be equal to stack amount of our aura
        if (Aura* triggeredAura = target->GetAura(triggeredSpellId, casterGUID))
            triggeredAura->ModStackAmount(GetBase()->GetStackAmount() - triggeredAura->GetStackAmount());
    }
}

void AuraEffect::HandleAuraModFakeInebriation(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
    {
        target->m_invisibilityDetect.AddFlag(INVISIBILITY_DRUNK);
        target->m_invisibilityDetect.AddValue(INVISIBILITY_DRUNK, GetAmount());

        if (target->GetTypeId() == TypeID::TYPEID_PLAYER)
        {
            int32 oldval = target->ToPlayer()->GetInt32Value(PLAYER_FIELD_FAKE_INEBRIATION);
            target->ToPlayer()->SetInt32Value(PLAYER_FIELD_FAKE_INEBRIATION, oldval + GetAmount());
        }
    }
    else
    {
        bool removeDetect = !target->HasAuraType(SPELL_AURA_MOD_FAKE_INEBRIATE);

        target->m_invisibilityDetect.AddValue(INVISIBILITY_DRUNK, -GetAmount());

        if (target->GetTypeId() == TypeID::TYPEID_PLAYER)
        {
            int32 oldval = target->ToPlayer()->GetInt32Value(PLAYER_FIELD_FAKE_INEBRIATION);
            target->ToPlayer()->SetInt32Value(PLAYER_FIELD_FAKE_INEBRIATION, oldval - GetAmount());

            if (removeDetect)
                removeDetect = !target->ToPlayer()->GetDrunkValue();
        }

        if (removeDetect)
            target->m_invisibilityDetect.DelFlag(INVISIBILITY_DRUNK);
    }

    // call functions which may have additional effects after chainging state of unit
    target->UpdateObjectVisibility();
}

void AuraEffect::HandleAuraOverrideSpells(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();

    if (!target || !target->IsInWorld())
        return;

    uint32 overrideId = uint32(GetMiscValue());

    if (apply)
    {
        target->SetUInt16Value(PLAYER_FIELD_OVERRIDE_SPELLS_ID, 1, overrideId);
        if (OverrideSpellDataEntry const* overrideSpells = sOverrideSpellDataStore.LookupEntry(overrideId))
            for (uint8 i = 0; i < MAX_OVERRIDE_SPELL; ++i)
                if (uint32 spellId = overrideSpells->spellId[i])
                    target->AddTemporarySpell(spellId);
    }
    else
    {
        if (target->GetUInt16Value(PLAYER_FIELD_OVERRIDE_SPELLS_ID, 1) == overrideId)
            target->SetUInt16Value(PLAYER_FIELD_OVERRIDE_SPELLS_ID, 1, 0);

        if (OverrideSpellDataEntry const* overrideSpells = sOverrideSpellDataStore.LookupEntry(overrideId))
            for (uint8 i = 0; i < MAX_OVERRIDE_SPELL; ++i)
                if (uint32 spellId = overrideSpells->spellId[i])
                    target->RemoveTemporarySpell(spellId);
    }
}

void AuraEffect::HandleAuraSetVehicle(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (!target->IsInWorld())
        return;

    uint32 vehicleId = GetMiscValue();

    if (apply)
    {
        if (!target->CreateVehicleKit(vehicleId, 0))
            return;
    }
    else if (target->GetVehicleKit())
        target->RemoveVehicleKit();

    if (target->GetTypeId() != TypeID::TYPEID_PLAYER)
        return;

    if (apply)
        target->ToPlayer()->SendOnCancelExpectedVehicleRideAura();
}

void AuraEffect::HandlePreventResurrection(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    if (aurApp->GetTarget()->GetTypeId() != TypeID::TYPEID_PLAYER)
        return;

    if (apply)
        aurApp->GetTarget()->RemoveByteFlag(PLAYER_FIELD_LIFETIME_MAX_RANK, 0, PLAYER_FIELD_BYTE_RELEASE_TIMER);
    else if (!aurApp->GetTarget()->GetBaseMap()->Instanceable())
        aurApp->GetTarget()->SetByteFlag(PLAYER_FIELD_LIFETIME_MAX_RANK, 0, PLAYER_FIELD_BYTE_RELEASE_TIMER);
}

void AuraEffect::HandleAuraMastery(AuraApplication const* aurApp, uint8 mode, bool /*apply*/) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();
    if (!target)
        return;

    target->UpdateMastery();
}

void AuraEffect::HandleAuraForceWeather(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();

    if (!target)
        return;

    if (apply)
    {
        WorldPacket data(SMSG_WEATHER, 4 + 4 + 1);
        data << uint32(GetMiscValue()); // WeatherID
        data << float(1.0f);            // Intensity
        data.WriteBit(false);           // Abrupt
        data.FlushBits();
        target->GetSession()->SendPacket(&data);
    }
    else
    {
        // send weather for current zone
        if (Weather* weather = WeatherMgr::FindWeather(target->GetZoneId()))
            weather->SendWeatherUpdateToPlayer(target);
        else
        {
            if (!WeatherMgr::AddWeather(target->GetZoneId()))
            {
                // send fine weather packet to remove old weather
                WeatherMgr::SendFineWeatherUpdateToPlayer(target);
            }
        }
    }
}

void AuraEffect::HandleEnableAltPower(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    uint32 altPowerId = GetMiscValue();
    UnitPowerBarEntry const* powerEntry = sUnitPowerBarStore.LookupEntry(altPowerId);
    if (!powerEntry)
        return;

    if (apply)
        aurApp->GetTarget()->SetMaxPower(POWER_ALTERNATE_POWER, powerEntry->MaxPower);
    else
        aurApp->GetTarget()->SetMaxPower(POWER_ALTERNATE_POWER, 0);
}

void AuraEffect::HandleOverrideSpellPowerByAttackPower(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();
    if (!target)
        return;

    target->ApplyModSignedFloatValue(PLAYER_FIELD_OVERRIDE_SPELL_POWER_BY_APPERCENT, float(m_amount), apply);
    target->UpdateSpellDamageAndHealingBonus();
}

void AuraEffect::HandleAuraOverrideAutoattackWithSpell(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Unit* target = aurApp->GetTarget();

    if (apply)
    {
        SpellInfo const* overrideSpell = sSpellMgr->GetSpellInfo(m_spellInfo->Effects[GetEffIndex()].TriggerSpell);
        target->SetAutoattackOverrideRange(overrideSpell->GetMaxRange());
        target->SetAutoattackOverrideSpell(overrideSpell);
    }
    else
    {
        target->SetAutoattackOverrideSpell(0);
        target->SetAutoattackOverrideRange(0);
    }
}

void AuraEffect::HandleOverrideAttackPowerBySpellPower(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & (AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK | AURA_EFFECT_HANDLE_STAT)))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();
    if (!target)
        return;

    target->ApplyModSignedFloatValue(PLAYER_FIELD_OVERRIDE_APBY_SPELL_POWER_PERCENT, float(m_amount), apply);
    target->UpdateAttackPowerAndDamage();
    target->UpdateAttackPowerAndDamage(true);
}

void AuraEffect::HandlePlayScene(AuraApplication const* aurApp, uint8 mode, bool apply) const
{
    if (!(mode & AURA_EFFECT_HANDLE_REAL))
        return;

    Player* target = aurApp->GetTarget()->ToPlayer();
    if (!target)
        return;

    SceneTemplate const* sceneTemplate = sObjectMgr->GetSceneTemplate(GetMiscValue());
    if (!sceneTemplate)
    {
        SF_LOG_ERROR("spells", "SceneTemplate: %u does not exist in database.", GetMiscValue());
        return;
    }

    // check db2 if packageid exists
    SceneScriptPackageEntry const* entry = sSceneScriptPackageStore.LookupEntry(sceneTemplate->ScenePackageId);
    if (!entry)
    {
        SF_LOG_ERROR("spells", "ScenePackageID: %u does not exist in db2.", sceneTemplate->ScenePackageId);
        return;
    }

    ObjectGuid transportGUID = target->GetTransGUID();
    float PositionX = target->GetPositionX();
    float PositionY = target->GetPositionY();
    float PositionZ = target->GetPositionZ();
    float PositionO = target->GetOrientation();
    bool hasSceneID = sceneTemplate->SceneId;
    bool hasSceneFlags = sceneTemplate->PlaybackFlags;
    bool hasSceneScriptPackageID = sceneTemplate->ScenePackageId;
    uint32 sceneInstanceID = 0;
    if (!sceneInstanceID)
        ++sceneInstanceID;
    if (apply)
    {
        WorldPacket data(SMSG_PLAY_SCENE, 4 + 4 + 4 + 1 + 8 + 4 + 4 + 4 + 4 + 4);
        data << float(PositionY);
        data << float(PositionZ);
        data << float(PositionX);

        data.WriteBit(!hasSceneFlags);
        data.WriteBit(!sceneInstanceID);
        data.WriteBit(!PositionO);
        data.WriteBit(!hasSceneID);
        data.WriteBit(!hasSceneScriptPackageID);
        data.WriteBit(true);

        data.WriteGuidMask(transportGUID, 3, 2, 6, 4, 7, 1, 5, 0);
        data.FlushBits();
        data.WriteGuidBytes(transportGUID, 6, 3, 5, 4, 1, 2, 0, 7);

        if (hasSceneFlags)
            data << uint32(sceneTemplate->PlaybackFlags);
        if (PositionO)
            data << float(PositionO);
        if (hasSceneID)
            data << uint32(sceneTemplate->SceneId);
        if (sceneInstanceID)
            data << uint32(sceneInstanceID);
        if (hasSceneScriptPackageID)
            data << uint32(sceneTemplate->ScenePackageId);

        target->GetSession()->SendPacket(&data);
    }
    else
    {
        WorldPacket data(SMSG_CANCEL_SCENE, 4 + 1);
        data.WriteBit(!sceneInstanceID);
        data.FlushBits();
        data << uint32(sceneInstanceID);    // SceneInstanceID
        target->GetSession()->SendPacket(&data);
    }
}
