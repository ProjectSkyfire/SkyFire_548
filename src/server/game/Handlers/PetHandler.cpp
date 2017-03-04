/*
 * Copyright (C) 2011-2017 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2017 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2017 MaNGOS <https://www.getmangos.eu/>
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

#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "ObjectMgr.h"
#include "SpellMgr.h"
#include "Log.h"
#include "Opcodes.h"
#include "Spell.h"
#include "ObjectAccessor.h"
#include "CreatureAI.h"
#include "Util.h"
#include "Pet.h"
#include "World.h"
#include "Group.h"
#include "SpellInfo.h"
#include "Player.h"

void WorldSession::HandleDismissCritter(WorldPacket& recvData)
{
    ObjectGuid guid;

    recvData.ReadGuidMask(guid, 4, 6, 7, 5, 1, 0, 2, 3);
    recvData.ReadGuidBytes(guid, 2, 4, 5, 0, 1, 7, 3, 6);

    SF_LOG_DEBUG("network", "WORLD: Received CMSG_DISMISS_CRITTER for (guid: %u)", uint32(GUID_LOPART(guid)));

    Unit* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*_player, guid);

    if (!pet)
    {
        SF_LOG_DEBUG("network", "Vanitypet (guid: %u) does not exist - player '%s' (guid: %u / account: %u) attempted to dismiss it (possibly lagged out)",
            uint32(GUID_LOPART(guid)), GetPlayer()->GetName().c_str(), GetPlayer()->GetGUIDLow(), GetAccountId());
        return;
    }

    if (_player->GetCritterGUID() == pet->GetGUID())
    {
         if (pet->GetTypeId() == TYPEID_UNIT && pet->ToCreature()->IsSummon())
             pet->ToTempSummon()->UnSummon();
    }
}

void WorldSession::HandlePetAction(WorldPacket& recvData) //  sub_68C8FD [5.4.8 18291]
{
    ObjectGuid PetGUID, TargetGUID;
    uint32 Action;
    float x, y, z;
    recvData >> Action;

    // Position
    recvData >> y >> z >> x;

    PetGUID[1] = recvData.ReadBit();
    PetGUID[0] = recvData.ReadBit();
    PetGUID[6] = recvData.ReadBit();
    PetGUID[7] = recvData.ReadBit();
    PetGUID[5] = recvData.ReadBit();
    TargetGUID[7] = recvData.ReadBit();
    PetGUID[2] = recvData.ReadBit();
    PetGUID[3] = recvData.ReadBit();
    TargetGUID[6] = recvData.ReadBit();
    TargetGUID[3] = recvData.ReadBit();
    TargetGUID[0] = recvData.ReadBit();
    TargetGUID[2] = recvData.ReadBit();
    TargetGUID[5] = recvData.ReadBit();
    PetGUID[4] = recvData.ReadBit();
    TargetGUID[4] = recvData.ReadBit();
    TargetGUID[1] = recvData.ReadBit();

    recvData.ReadByteSeq(PetGUID[7]);
    recvData.ReadByteSeq(PetGUID[6]);
    recvData.ReadByteSeq(PetGUID[1]);
    recvData.ReadByteSeq(PetGUID[2]);
    recvData.ReadByteSeq(PetGUID[5]);
    recvData.ReadByteSeq(PetGUID[4]);
    recvData.ReadByteSeq(TargetGUID[5]);
    recvData.ReadByteSeq(PetGUID[3]);
    recvData.ReadByteSeq(TargetGUID[0]);
    recvData.ReadByteSeq(TargetGUID[1]);
    recvData.ReadByteSeq(TargetGUID[7]);
    recvData.ReadByteSeq(TargetGUID[4]);
    recvData.ReadByteSeq(TargetGUID[6]);
    recvData.ReadByteSeq(TargetGUID[2]);
    recvData.ReadByteSeq(TargetGUID[3]);
    recvData.ReadByteSeq(PetGUID[0]);

    uint32 SpellID = UNIT_ACTION_BUTTON_ACTION(Action);
    uint8 Flag = UNIT_ACTION_BUTTON_TYPE(Action);             //delete = 0x07 CastSpell = C1

    // used also for charmed creature
    Unit* pet = ObjectAccessor::GetUnit(*_player, PetGUID);
    SF_LOG_DEBUG("network", "HandlePetAction: Pet (GUID: %u) - Flag: %u, SpellID: %u, Target: %u.", uint32(GUID_LOPART(PetGUID)), uint32(Flag), SpellID, uint32(GUID_LOPART(TargetGUID)));

    if (!pet)
    {
        SF_LOG_ERROR("network", "HandlePetAction: Pet (GUID: %u) doesn't exist for player %s (GUID: %u)", uint32(GUID_LOPART(PetGUID)), GetPlayer()->GetName().c_str(), GUID_LOPART(GetPlayer()->GetGUID()));
        return;
    }

    if (pet != GetPlayer()->GetFirstControlled())
    {
        SF_LOG_ERROR("network", "HandlePetAction: Pet (GUID: %u) does not belong to player %s (GUID: %u)", uint32(GUID_LOPART(PetGUID)), GetPlayer()->GetName().c_str(), GUID_LOPART(GetPlayer()->GetGUID()));
        return;
    }

    if (!pet->IsAlive())
    {
        SpellInfo const* spell = (Flag == ACT_ENABLED || Flag == ACT_PASSIVE) ? sSpellMgr->GetSpellInfo(SpellID) : NULL;
        if (!spell)
            return;
        if (!(spell->Attributes & SPELL_ATTR0_CASTABLE_WHILE_DEAD))
            return;
    }

    /// @todo allow control charmed player?
    if (pet->GetTypeId() == TYPEID_PLAYER && !(Flag == ACT_COMMAND && SpellID == COMMAND_ATTACK))
        return;

    if (GetPlayer()->m_Controlled.size() == 1)
        HandlePetActionHelper(pet, PetGUID, SpellID, Flag, TargetGUID, x, y, z);
    else
    {
        //If a pet is dismissed, m_Controlled will change
        std::vector<Unit*> controlled;
        for (Unit::ControlList::iterator itr = GetPlayer()->m_Controlled.begin(); itr != GetPlayer()->m_Controlled.end(); ++itr)
            if ((*itr)->GetEntry() == pet->GetEntry() && (*itr)->IsAlive())
                controlled.push_back(*itr);
        for (std::vector<Unit*>::iterator itr = controlled.begin(); itr != controlled.end(); ++itr)
            HandlePetActionHelper(*itr, PetGUID, SpellID, Flag, TargetGUID, x, y, z);
    }
}

void WorldSession::HandlePetStopAttack(WorldPacket &recvData)
{
    ObjectGuid guid;

    guid[7] = recvData.ReadBit();
    guid[5] = recvData.ReadBit();
    guid[1] = recvData.ReadBit();
    guid[6] = recvData.ReadBit();
    guid[0] = recvData.ReadBit();
    guid[2] = recvData.ReadBit();
    guid[4] = recvData.ReadBit();
    guid[3] = recvData.ReadBit();

    recvData.ReadByteSeq(guid[2]);
    recvData.ReadByteSeq(guid[5]);
    recvData.ReadByteSeq(guid[0]);
    recvData.ReadByteSeq(guid[4]);
    recvData.ReadByteSeq(guid[1]);
    recvData.ReadByteSeq(guid[7]);
    recvData.ReadByteSeq(guid[6]);
    recvData.ReadByteSeq(guid[3]);

    SF_LOG_DEBUG("network", "WORLD: Received CMSG_PET_STOP_ATTACK for GUID " UI64FMTD "", (uint64)guid);

    Unit* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*_player, guid);

    if (!pet)
    {
        SF_LOG_ERROR("network", "HandlePetStopAttack: Pet %u does not exist", uint32(GUID_LOPART(guid)));
        return;
    }

    if (pet != GetPlayer()->GetPet() && pet != GetPlayer()->GetCharm())
    {
        SF_LOG_ERROR("network", "HandlePetStopAttack: Pet GUID %u isn't a pet or charmed creature of player %s",
            uint32(GUID_LOPART(guid)), GetPlayer()->GetName().c_str());
        return;
    }

    if (!pet->IsAlive())
        return;

    pet->AttackStop();
}

void WorldSession::HandlePetActionHelper(Unit* pet, uint64 guid1, uint32 spellid, uint16 flag, uint64 guid2, float x, float y, float z)
{
    CharmInfo* charmInfo = pet->GetCharmInfo();
    if (!charmInfo)
    {
        SF_LOG_ERROR("network", "WorldSession::HandlePetAction(petGuid: " UI64FMTD ", tagGuid: " UI64FMTD ", spellId: %u, flag: %u): object (entry: %u TypeId: %u) is considered pet-like but doesn't have a charminfo!",
            guid1, guid2, spellid, flag, pet->GetGUIDLow(), pet->GetTypeId());
        return;
    }

    switch (flag)
    {
        case ACT_COMMAND:                                   //0x07
            switch (spellid)
            {
                case COMMAND_STAY:                          //flat=1792  //STAY
                    pet->StopMoving();
                    pet->GetMotionMaster()->Clear(false);
                    pet->GetMotionMaster()->MoveIdle();
                    charmInfo->SetCommandState(COMMAND_STAY);

                    charmInfo->SetIsCommandAttack(false);
                    charmInfo->SetIsAtStay(true);
                    charmInfo->SetIsCommandFollow(false);
                    charmInfo->SetIsFollowing(false);
                    charmInfo->SetIsReturning(false);
                    charmInfo->SaveStayPosition();
                    break;
                case COMMAND_FOLLOW:                        //spellid=1792  //FOLLOW
                    pet->AttackStop();
                    pet->InterruptNonMeleeSpells(false);
                    pet->GetMotionMaster()->MoveFollow(_player, PET_FOLLOW_DIST, pet->GetFollowAngle());
                    charmInfo->SetCommandState(COMMAND_FOLLOW);

                    charmInfo->SetIsCommandAttack(false);
                    charmInfo->SetIsAtStay(false);
                    charmInfo->SetIsReturning(true);
                    charmInfo->SetIsCommandFollow(true);
                    charmInfo->SetIsFollowing(false);
                    break;
                case COMMAND_ATTACK:                        //spellid=1792  //ATTACK
                {
                    // Can't attack if owner is pacified
                    if (_player->HasAuraType(SPELL_AURA_MOD_PACIFY))
                    {
                        //pet->SendPetCastFail(spellid, SPELL_FAILED_PACIFIED);
                        /// @todo Send proper error message to client
                        return;
                    }

                    // only place where pet can be player
                    Unit* TargetUnit = ObjectAccessor::GetUnit(*_player, guid2);
                    if (!TargetUnit)
                        return;

                    if (Unit* owner = pet->GetOwner())
                        if (!owner->IsValidAttackTarget(TargetUnit))
                            return;

                    pet->ClearUnitState(UNIT_STATE_FOLLOW);
                    // This is true if pet has no target or has target but targets differs.
                    if (pet->GetVictim() != TargetUnit || (pet->GetVictim() == TargetUnit && !pet->GetCharmInfo()->IsCommandAttack()))
                    {
                        if (pet->GetVictim())
                            pet->AttackStop();

                        if (pet->GetTypeId() != TYPEID_PLAYER && pet->ToCreature()->IsAIEnabled)
                        {
                            charmInfo->SetIsCommandAttack(true);
                            charmInfo->SetIsAtStay(false);
                            charmInfo->SetIsFollowing(false);
                            charmInfo->SetIsCommandFollow(false);
                            charmInfo->SetIsReturning(false);

                            pet->ToCreature()->AI()->AttackStart(TargetUnit);

                            //10% chance to play special pet attack talk, else growl
                            if (pet->ToCreature()->IsPet() && ((Pet*)pet)->getPetType() == SUMMON_PET && pet != TargetUnit && urand(0, 100) < 10)
                                pet->SendPetTalk((uint32)PET_TALK_ATTACK);
                            else
                            {
                                // 90% chance for pet and 100% chance for charmed creature
                                pet->SendPetAIReaction(guid1);
                            }
                        }
                        else                                // charmed player
                        {
                            if (pet->GetVictim() && pet->GetVictim() != TargetUnit)
                                pet->AttackStop();

                            charmInfo->SetIsCommandAttack(true);
                            charmInfo->SetIsAtStay(false);
                            charmInfo->SetIsFollowing(false);
                            charmInfo->SetIsCommandFollow(false);
                            charmInfo->SetIsReturning(false);

                            pet->Attack(TargetUnit, true);
                            pet->SendPetAIReaction(guid1);
                        }
                    }
                    break;
                }
                case COMMAND_ABANDON:                       // abandon (hunter pet) or dismiss (summoned pet)
                    if (pet->GetCharmerGUID() == GetPlayer()->GetGUID())
                        _player->StopCastingCharm();
                    else if (pet->GetOwnerGUID() == GetPlayer()->GetGUID())
                    {
                        ASSERT(pet->GetTypeId() == TYPEID_UNIT);
                        if (pet->IsPet())
                        {
                            if (((Pet*)pet)->getPetType() == HUNTER_PET)
                                GetPlayer()->RemovePet((Pet*)pet, PET_SAVE_AS_DELETED);
                            else
                                //dismissing a summoned pet is like killing them (this prevents returning a soulshard...)
                                pet->setDeathState(CORPSE);
                        }
                        else if (pet->HasUnitTypeMask(UNIT_MASK_MINION))
                        {
                            ((Minion*)pet)->UnSummon();
                        }
                    }
                    break;
                case COMMAND_MOVE_TO:
                    pet->StopMoving();
                    pet->GetMotionMaster()->Clear(false);
                    pet->GetMotionMaster()->MovePoint(0, x, y, z);
                    charmInfo->SetCommandState(COMMAND_MOVE_TO);

                    charmInfo->SetIsCommandAttack(false);
                    charmInfo->SetIsAtStay(true);
                    charmInfo->SetIsFollowing(false);
                    charmInfo->SetIsReturning(false);
                    charmInfo->SaveStayPosition();
                    break;
                default:
                    SF_LOG_ERROR("network", "WORLD: unknown PET flag Action %i and spellid %i.", uint32(flag), spellid);
            }
            break;
        case ACT_REACTION:                                  // 0x6
            switch (spellid)
            {
                case REACT_PASSIVE:                         //passive
                    pet->AttackStop();

                case REACT_DEFENSIVE:                       //recovery
                case REACT_AGGRESSIVE:                      //activete
                    if (pet->GetTypeId() == TYPEID_UNIT)
                        pet->ToCreature()->SetReactState(ReactStates(spellid));
                    break;
            }
            break;
        case ACT_DISABLED:                                  // 0x81    spell (disabled), ignore
        case ACT_PASSIVE:                                   // 0x01
        case ACT_ENABLED:                                   // 0xC1    spell
        {
            Unit* unit_target = NULL;

            if (guid2)
                unit_target = ObjectAccessor::GetUnit(*_player, guid2);

            // do not cast unknown spells
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellid);
            if (!spellInfo)
            {
                SF_LOG_ERROR("network", "WORLD: unknown PET spell id %i", spellid);
                return;
            }

            for (uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
            {
                if (spellInfo->Effects[i].TargetA.GetTarget() == TARGET_UNIT_SRC_AREA_ENEMY || spellInfo->Effects[i].TargetA.GetTarget() == TARGET_UNIT_DEST_AREA_ENEMY || spellInfo->Effects[i].TargetA.GetTarget() == TARGET_DEST_DYNOBJ_ENEMY)
                    return;
            }

            // do not cast not learned spells
            if (!pet->HasSpell(spellid) || spellInfo->IsPassive())
                return;

            //  Clear the flags as if owner clicked 'attack'. AI will reset them
            //  after AttackStart, even if spell failed
            if (pet->GetCharmInfo())
            {
                pet->GetCharmInfo()->SetIsAtStay(false);
                pet->GetCharmInfo()->SetIsCommandAttack(true);
                pet->GetCharmInfo()->SetIsReturning(false);
                pet->GetCharmInfo()->SetIsFollowing(false);
            }

            Spell* spell = new Spell(pet, spellInfo, TRIGGERED_NONE);

            SpellCastResult result = spell->CheckPetCast(unit_target);

            //auto turn to target unless possessed
            if (result == SPELL_FAILED_UNIT_NOT_INFRONT && !pet->isPossessed() && !pet->IsVehicle())
            {
                if (unit_target)
                {
                    pet->SetInFront(unit_target);
                    if (Player* player = unit_target->ToPlayer())
                        pet->SendUpdateToPlayer(player);
                }
                else if (Unit* unit_target2 = spell->m_targets.GetUnitTarget())
                {
                    pet->SetInFront(unit_target2);
                    if (Player* player = unit_target2->ToPlayer())
                        pet->SendUpdateToPlayer(player);
                }

                if (Unit* powner = pet->GetCharmerOrOwner())
                    if (Player* player = powner->ToPlayer())
                        pet->SendUpdateToPlayer(player);

                result = SPELL_CAST_OK;
            }

            if (result == SPELL_CAST_OK)
            {
                pet->ToCreature()->AddCreatureSpellCooldown(spellid);

                unit_target = spell->m_targets.GetUnitTarget();

                //10% chance to play special pet attack talk, else growl
                //actually this only seems to happen on special spells, fire shield for imp, torment for voidwalker, but it's stupid to check every spell
                if (pet->ToCreature()->IsPet() && (((Pet*)pet)->getPetType() == SUMMON_PET) && (pet != unit_target) && (urand(0, 100) < 10))
                    pet->SendPetTalk((uint32)PET_TALK_SPECIAL_SPELL);
                else
                {
                    pet->SendPetAIReaction(guid1);
                }

                if (unit_target && !GetPlayer()->IsFriendlyTo(unit_target) && !pet->isPossessed() && !pet->IsVehicle())
                {
                    // This is true if pet has no target or has target but targets differs.
                    if (pet->GetVictim() != unit_target)
                    {
                        if (pet->GetVictim())
                            pet->AttackStop();
                        pet->GetMotionMaster()->Clear();
                        if (pet->ToCreature()->IsAIEnabled)
                            pet->ToCreature()->AI()->AttackStart(unit_target);
                    }
                }

                spell->prepare(&(spell->m_targets));
            }
            else
            {
                if (pet->isPossessed() || pet->IsVehicle()) /// @todo: confirm this check
                    Spell::SendCastResult(GetPlayer(), spellInfo, 0, result);
                else
                    spell->SendPetCastResult(result);

                if (!pet->ToCreature()->HasSpellCooldown(spellid))
                    GetPlayer()->SendClearCooldown(spellid, pet);

                spell->finish(false);
                delete spell;

                // reset specific flags in case of spell fail. AI will reset other flags
                if (pet->GetCharmInfo())
                    pet->GetCharmInfo()->SetIsCommandAttack(false);
            }
            break;
        }
        default:
            SF_LOG_ERROR("network", "WORLD: unknown PET flag Action %i and spellid %i.", uint32(flag), spellid);
    }
}

void WorldSession::HandlePetNameQuery(WorldPacket& recvData)
{
    SF_LOG_INFO("network", "HandlePetNameQuery. CMSG_PET_NAME_QUERY");

    ObjectGuid petGuid;
    ObjectGuid petNumber;

    petNumber[0] = recvData.ReadBit();
    petNumber[5] = recvData.ReadBit();
    petGuid[1] = recvData.ReadBit();
    petGuid[7] = recvData.ReadBit();
    petNumber[7] = recvData.ReadBit();
    petGuid[6] = recvData.ReadBit();
    petGuid[4] = recvData.ReadBit();
    petGuid[5] = recvData.ReadBit();
    petGuid[0] = recvData.ReadBit();
    petNumber[3] = recvData.ReadBit();
    petNumber[6] = recvData.ReadBit();
    petNumber[2] = recvData.ReadBit();
    petGuid[3] = recvData.ReadBit();
    petGuid[2] = recvData.ReadBit();
    petNumber[1] = recvData.ReadBit();
    petNumber[4] = recvData.ReadBit();

    recvData.ReadByteSeq(petNumber[2]);
    recvData.ReadByteSeq(petNumber[1]);
    recvData.ReadByteSeq(petNumber[0]);
    recvData.ReadByteSeq(petNumber[7]);
    recvData.ReadByteSeq(petGuid[5]);
    recvData.ReadByteSeq(petGuid[0]);
    recvData.ReadByteSeq(petNumber[6]);
    recvData.ReadByteSeq(petGuid[4]);
    recvData.ReadByteSeq(petNumber[5]);
    recvData.ReadByteSeq(petGuid[2]);
    recvData.ReadByteSeq(petGuid[6]);
    recvData.ReadByteSeq(petNumber[3]);
    recvData.ReadByteSeq(petGuid[3]);
    recvData.ReadByteSeq(petNumber[4]);
    recvData.ReadByteSeq(petGuid[1]);
    recvData.ReadByteSeq(petGuid[7]);

    SendPetNameQuery(petGuid, petNumber);
}

void WorldSession::SendPetNameQuery(ObjectGuid petGuid, uint64 petNumber)
{
    Creature* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*_player, petGuid);
    if (!pet)
    {
        WorldPacket data(SMSG_PET_NAME_QUERY_RESPONSE, (8 + 1));
        data.WriteBit(0);
        data.FlushBits();
        data << uint64(petNumber);

        _player->GetSession()->SendPacket(&data);
        return;
    }

    WorldPacket data(SMSG_PET_NAME_QUERY_RESPONSE, (8 + 1 + 1 + 5 + pet->GetName().size() + 4));
    data.WriteBit(1);                               // has data

    bool declinedNames = pet->IsPet() && ((Pet*)pet)->GetDeclinedNames();

    for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
    {
        if (declinedNames)
            data.WriteBits(((Pet*)pet)->GetDeclinedNames()->name[i].size(), 7);
        else
            data.WriteBits(0, 7);
    }

    data.WriteBit(0);                               // unknown
    data.WriteBits(pet->GetName().size(), 8);
    data.FlushBits();

    if (declinedNames)
        for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
            data.WriteString(((Pet*)pet)->GetDeclinedNames()->name[i]);

    data.WriteString(pet->GetName());
    data << uint32(pet->GetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP));
    data << uint64(petNumber);

    _player->GetSession()->SendPacket(&data);
}

bool WorldSession::CheckStableMaster(uint64 guid)
{
    // spell case or GM
    if (guid == GetPlayer()->GetGUID())
    {
        if (!GetPlayer()->IsGameMaster() && !GetPlayer()->HasAuraType(SPELL_AURA_OPEN_STABLE))
        {
            SF_LOG_DEBUG("network", "Player (GUID:%u) attempt open stable in cheating way.", GUID_LOPART(guid));
            return false;
        }
    }
    // stable master case
    else
    {
        if (!GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_STABLEMASTER))
        {
            SF_LOG_DEBUG("network", "Stablemaster (GUID:%u) not found or you can't interact with him.", GUID_LOPART(guid));
            return false;
        }
    }
    return true;
}

void WorldSession::HandlePetSetAction(WorldPacket& recvData)
{
    SF_LOG_INFO("network", "HandlePetSetAction. CMSG_PET_SET_ACTION");

    uint64 petguid;
    uint8  count;

    recvData >> petguid;

    Unit* pet = ObjectAccessor::GetUnit(*_player, petguid);

    if (!pet || pet != _player->GetFirstControlled())
    {
        SF_LOG_ERROR("network", "HandlePetSetAction: Unknown pet (GUID: %u) or pet owner (GUID: %u)", GUID_LOPART(petguid), _player->GetGUIDLow());
        return;
    }

    CharmInfo* charmInfo = pet->GetCharmInfo();
    if (!charmInfo)
    {
        SF_LOG_ERROR("network", "WorldSession::HandlePetSetAction: object (GUID: %u TypeId: %u) is considered pet-like but doesn't have a charminfo!", pet->GetGUIDLow(), pet->GetTypeId());
        return;
    }

    count = (recvData.size() == 24) ? 2 : 1;

    uint32 position[2];
    uint32 data[2];
    bool move_command = false;

    for (uint8 i = 0; i < count; ++i)
    {
        recvData >> position[i];
        recvData >> data[i];

        uint8 act_state = UNIT_ACTION_BUTTON_TYPE(data[i]);

        //ignore invalid position
        if (position[i] >= MAX_UNIT_ACTION_BAR_INDEX)
            return;

        // in the normal case, command and reaction buttons can only be moved, not removed
        // at moving count == 2, at removing count == 1
        // ignore attempt to remove command|reaction buttons (not possible at normal case)
        if (act_state == ACT_COMMAND || act_state == ACT_REACTION)
        {
            if (count == 1)
                return;

            move_command = true;
        }
    }

    // check swap (at command->spell swap client remove spell first in another packet, so check only command move correctness)
    if (move_command)
    {
        uint8 act_state_0 = UNIT_ACTION_BUTTON_TYPE(data[0]);
        if (act_state_0 == ACT_COMMAND || act_state_0 == ACT_REACTION)
        {
            uint32 spell_id_0 = UNIT_ACTION_BUTTON_ACTION(data[0]);
            UnitActionBarEntry const* actionEntry_1 = charmInfo->GetActionBarEntry(position[1]);
            if (!actionEntry_1 || spell_id_0 != actionEntry_1->GetAction() ||
                act_state_0 != actionEntry_1->GetType())
                return;
        }

        uint8 act_state_1 = UNIT_ACTION_BUTTON_TYPE(data[1]);
        if (act_state_1 == ACT_COMMAND || act_state_1 == ACT_REACTION)
        {
            uint32 spell_id_1 = UNIT_ACTION_BUTTON_ACTION(data[1]);
            UnitActionBarEntry const* actionEntry_0 = charmInfo->GetActionBarEntry(position[0]);
            if (!actionEntry_0 || spell_id_1 != actionEntry_0->GetAction() ||
                act_state_1 != actionEntry_0->GetType())
                return;
        }
    }

    for (uint8 i = 0; i < count; ++i)
    {
        uint32 spell_id = UNIT_ACTION_BUTTON_ACTION(data[i]);
        uint8 act_state = UNIT_ACTION_BUTTON_TYPE(data[i]);

        SF_LOG_INFO("network", "Player %s has changed pet spell action. Position: %u, Spell: %u, State: 0x%X",
            _player->GetName().c_str(), position[i], spell_id, uint32(act_state));

        //if it's act for spell (en/disable/cast) and there is a spell given (0 = remove spell) which pet doesn't know, don't add
        if (!((act_state == ACT_ENABLED || act_state == ACT_DISABLED || act_state == ACT_PASSIVE) && spell_id && !pet->HasSpell(spell_id)))
        {
            if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spell_id))
            {
                //sign for autocast
                if (act_state == ACT_ENABLED)
                {
                    if (pet->GetTypeId() == TYPEID_UNIT && pet->ToCreature()->IsPet())
                        ((Pet*)pet)->ToggleAutocast(spellInfo, true);
                    else
                        for (Unit::ControlList::iterator itr = GetPlayer()->m_Controlled.begin(); itr != GetPlayer()->m_Controlled.end(); ++itr)
                            if ((*itr)->GetEntry() == pet->GetEntry())
                                (*itr)->GetCharmInfo()->ToggleCreatureAutocast(spellInfo, true);
                }
                //sign for no/turn off autocast
                else if (act_state == ACT_DISABLED)
                {
                    if (pet->GetTypeId() == TYPEID_UNIT && pet->ToCreature()->IsPet())
                        ((Pet*)pet)->ToggleAutocast(spellInfo, false);
                    else
                        for (Unit::ControlList::iterator itr = GetPlayer()->m_Controlled.begin(); itr != GetPlayer()->m_Controlled.end(); ++itr)
                            if ((*itr)->GetEntry() == pet->GetEntry())
                                (*itr)->GetCharmInfo()->ToggleCreatureAutocast(spellInfo, false);
                }
            }

            charmInfo->SetActionBar(position[i], spell_id, ActiveStates(act_state));
        }
    }
}

void WorldSession::HandlePetRename(WorldPacket& recvData)
{
    SF_LOG_INFO("network", "HandlePetRename. CMSG_PET_RENAME");

    uint8 declinedNamesLength[MAX_DECLINED_NAME_CASES];
    uint32 petNumber;
    std::string name;
    DeclinedName declinedName;

    recvData >> petNumber;
    bool hasName = !recvData.ReadBit();
    bool hasDeclinedNames = recvData.ReadBit();
    if (hasDeclinedNames)
        for (int i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
            declinedNamesLength[i] = recvData.ReadBits(7);
    if (hasName)
        name = recvData.ReadString(recvData.ReadBits(8));
    if (hasDeclinedNames)
        for (int i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
            declinedName.name[i] = recvData.ReadBits(declinedNamesLength[i]);

    Pet* pet = _player->GetPet();

    if (!pet || pet->getPetType() != HUNTER_PET ||
        !pet->HasByteFlag(UNIT_FIELD_SHAPESHIFT_FORM, 2, UNIT_CAN_BE_RENAMED) ||
        !pet->GetCharmInfo())
        return;

    PetNameInvalidReason res = ObjectMgr::CheckPetName(name);
    if (res != PET_NAME_SUCCESS)
    {
        SendPetNameInvalid(res, name, NULL, petNumber);
        return;
    }

    if (sObjectMgr->IsReservedName(name))
    {
        SendPetNameInvalid(PET_NAME_RESERVED, name, NULL, petNumber);
        return;
    }

    pet->SetName(name);

    if (_player->GetGroup())
        _player->SetGroupUpdateFlag(GROUP_UPDATE_FLAG_PET_NAME);

    pet->RemoveByteFlag(UNIT_FIELD_SHAPESHIFT_FORM, 2, UNIT_CAN_BE_RENAMED);

    if (hasDeclinedNames)
    {
        std::wstring wname;
        if (!Utf8toWStr(name, wname))
            return;

        if (!ObjectMgr::CheckDeclinedNames(wname, declinedName))
        {
            SendPetNameInvalid(PET_NAME_DECLENSION_DOESNT_MATCH_BASE_NAME, name, &declinedName, petNumber);
            return;
        }
    }

    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    if (hasDeclinedNames)
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_PET_DECLINEDNAME);
        stmt->setUInt32(0, pet->GetCharmInfo()->GetPetNumber());
        trans->Append(stmt);

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_ADD_CHAR_PET_DECLINEDNAME);
        stmt->setUInt32(0, _player->GetGUIDLow());

        for (uint8 i = 0; i < 5; i++)
            stmt->setString(i+1, declinedName.name[i]);

        trans->Append(stmt);
    }

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_NAME);
    stmt->setString(0, name);
    stmt->setUInt32(1, _player->GetGUIDLow());
    stmt->setUInt32(2, pet->GetCharmInfo()->GetPetNumber());
    trans->Append(stmt);

    CharacterDatabase.CommitTransaction(trans);

    pet->SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP, uint32(time(NULL))); // cast can't be helped
}

void WorldSession::HandlePetAbandon(WorldPacket& recvData)
{
    ObjectGuid guid;

    guid[7] = recvData.ReadBit();
    guid[3] = recvData.ReadBit();
    guid[4] = recvData.ReadBit();
    guid[2] = recvData.ReadBit();
    guid[5] = recvData.ReadBit();
    guid[6] = recvData.ReadBit();
    guid[1] = recvData.ReadBit();
    guid[0] = recvData.ReadBit();

    recvData.ReadByteSeq(guid[0]);
    recvData.ReadByteSeq(guid[2]);
    recvData.ReadByteSeq(guid[5]);
    recvData.ReadByteSeq(guid[6]);
    recvData.ReadByteSeq(guid[7]);
    recvData.ReadByteSeq(guid[1]);
    recvData.ReadByteSeq(guid[4]);
    recvData.ReadByteSeq(guid[3]);

    SF_LOG_INFO("network", "HandlePetAbandon. CMSG_PET_ABANDON pet guid is %u", GUID_LOPART(guid));

    if (!_player->IsInWorld())
        return;

    // pet/charmed
    Creature* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*_player, guid);
    if (pet)
    {
        if (pet->IsPet())
            _player->RemovePet((Pet*)pet, PET_SAVE_AS_DELETED);
        else if (pet->GetGUID() == _player->GetCharmGUID())
            _player->StopCastingCharm();
    }
}

void WorldSession::HandlePetSpellAutocastOpcode(WorldPacket& recvPacket)
{
    SF_LOG_INFO("network", "CMSG_PET_SPELL_AUTOCAST");
    ObjectGuid petGUID;
    uint32 spellId;
    bool state;

    recvPacket >> spellId;
    uint8 bitOrder[8] = { 0, 4, 2, 6, 1, 5, 3, 7 };
    recvPacket.ReadBitInOrder(petGUID, bitOrder);
    state = recvPacket.ReadBit();

    recvPacket.ReadGuidBytes(petGUID, 5, 0, 4, 1, 7, 2, 3, 6);

    if (!_player->GetGuardianPet() && !_player->GetCharm())
        return;

    if (ObjectAccessor::FindPlayer(petGUID))
        return;

    Creature* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*_player, petGUID);

    if (!pet || (pet != _player->GetGuardianPet() && pet != _player->GetCharm()))
    {
        SF_LOG_ERROR("network", "HandlePetSpellAutocastOpcode.Pet %u isn't pet of player %s (GUID: %u).", uint32(GUID_LOPART(petGUID)), GetPlayer()->GetName().c_str(), GUID_LOPART(GetPlayer()->GetGUID()));
        return;
    }

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    // do not add not learned spells/ passive spells
    if (!pet->HasSpell(spellId) || !spellInfo->IsAutocastable())
        return;

    CharmInfo* charmInfo = pet->GetCharmInfo();
    if (!charmInfo)
    {
        SF_LOG_ERROR("network", "WorldSession::HandlePetSpellAutocastOpcod: object (GUID: %u TypeId: %u) is considered pet-like but doesn't have a charminfo!", pet->GetGUIDLow(), pet->GetTypeId());
        return;
    }

    if (pet->IsPet())
        ((Pet*)pet)->ToggleAutocast(spellInfo, state);
    else
        pet->GetCharmInfo()->ToggleCreatureAutocast(spellInfo, state);

    charmInfo->SetSpellAutocast(spellInfo, state);
}

void WorldSession::HandlePetCastSpellOpcode(WorldPacket& recvPacket)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_PET_CAST_SPELL");

    uint64 guid;
    uint8  castCount;
    uint32 spellId;
    uint8  castFlags;

    recvPacket >> guid >> castCount >> spellId >> castFlags;

    SF_LOG_DEBUG("network", "WORLD: CMSG_PET_CAST_SPELL, guid: " UI64FMTD ", castCount: %u, spellId %u, castFlags %u", guid, castCount, spellId, castFlags);

    // This opcode is also sent from charmed and possessed units (players and creatures)
    if (!_player->GetGuardianPet() && !_player->GetCharm())
        return;

    Unit* caster = ObjectAccessor::GetUnit(*_player, guid);

    if (!caster || (caster != _player->GetGuardianPet() && caster != _player->GetCharm()))
    {
        SF_LOG_ERROR("network", "HandlePetCastSpellOpcode: Pet %u isn't pet of player %s (GUID: %u).", uint32(GUID_LOPART(guid)), GetPlayer()->GetName().c_str(), GUID_LOPART(GetPlayer()->GetGUID()));
        return;
    }

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo)
    {
        SF_LOG_ERROR("network", "WORLD: unknown PET spell id %i", spellId);
        return;
    }

    // do not cast not learned spells
    if (!caster->HasSpell(spellId) || spellInfo->IsPassive())
        return;

    SpellCastTargets targets;
    targets.Read(recvPacket, caster);
    HandleClientCastFlags(recvPacket, castFlags, targets);

    caster->ClearUnitState(UNIT_STATE_FOLLOW);

    Spell* spell = new Spell(caster, spellInfo, TRIGGERED_NONE);
    spell->m_cast_count = castCount;                    // probably pending spell cast
    spell->m_targets = targets;

    SpellCastResult result = spell->CheckPetCast(NULL);

    if (result == SPELL_CAST_OK)
    {
        if (Creature* creature = caster->ToCreature())
        {
            creature->AddCreatureSpellCooldown(spellId);
            if (Pet* pet = creature->ToPet())
            {
                // 10% chance to play special pet attack talk, else growl
                // actually this only seems to happen on special spells, fire shield for imp, torment for voidwalker, but it's stupid to check every spell
                if (pet->getPetType() == SUMMON_PET && (urand(0, 100) < 10))
                    pet->SendPetTalk(PET_TALK_SPECIAL_SPELL);
                else
                    pet->SendPetAIReaction(guid);
            }
        }

        spell->prepare(&(spell->m_targets));
    }
    else
    {
        spell->SendPetCastResult(result);

        if (caster->GetTypeId() == TYPEID_PLAYER)
        {
            if (!caster->ToPlayer()->HasSpellCooldown(spellId))
                GetPlayer()->SendClearCooldown(spellId, caster);
        }
        else
        {
            if (!caster->ToCreature()->HasSpellCooldown(spellId))
                GetPlayer()->SendClearCooldown(spellId, caster);
        }

        spell->finish(false);
        delete spell;
    }
}

void WorldSession::SendPetNameInvalid(uint32 error, const std::string& name, DeclinedName *declinedName, uint32 petNumber)
{
    WorldPacket data(SMSG_PET_NAME_INVALID, 4 + name.size() + 1 + 1 + 1 + 4);
    data.WriteBit(1);
    data.WriteBits(name.size(), 8);
    data.WriteBit(declinedName ? 1 : 0);
    data.FlushBits();

    if (declinedName)
    {
        for (uint32 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
            data.WriteBits(declinedName->name[i].size(), 7);

        for (uint32 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
            data.WriteString(declinedName->name[i]);
    }

    data.WriteString(name);
    data << uint8(error);
    data << uint32(petNumber);

    SendPacket(&data);
}

void WorldSession::HandlePetLearnTalent(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_PET_LEARN_TALENT");
}

void WorldSession::HandleLearnPreviewTalentsPet(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "CMSG_LEARN_PREVIEW_TALENTS_PET");
}
