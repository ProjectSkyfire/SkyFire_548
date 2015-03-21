/*
 * Copyright (C) 2011-2015 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2015 MaNGOS <http://getmangos.com/>
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

    guid[4] = recvData.ReadBit();  // 20
    guid[6] = recvData.ReadBit();  // 22
    guid[7] = recvData.ReadBit();  // 23
    guid[5] = recvData.ReadBit();  // 21
    guid[1] = recvData.ReadBit();  // 17
    guid[0] = recvData.ReadBit();  // 16
    guid[2] = recvData.ReadBit();  // 18
    guid[3] = recvData.ReadBit();  // 19

    recvData.ReadByteSeq(guid[2]);  // 18
    recvData.ReadByteSeq(guid[4]);  // 20
    recvData.ReadByteSeq(guid[5]);  // 21
    recvData.ReadByteSeq(guid[0]);  // 16
    recvData.ReadByteSeq(guid[1]);  // 17
    recvData.ReadByteSeq(guid[7]);  // 23
    recvData.ReadByteSeq(guid[3]);  // 19
    recvData.ReadByteSeq(guid[6]);  // 22
    
    TC_LOG_DEBUG("network", "WORLD: Received CMSG_DISMISS_CRITTER for GUID " UI64FMTD, uint64(guid));

    Unit* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*_player, uint64(guid));

    if (!pet)
    {
        TC_LOG_DEBUG("network", "Vanitypet (guid: %u) does not exist - player '%s' (guid: %u / account: %u) attempted to dismiss it (possibly lagged out)",
            uint32(GUID_LOPART(uint64(guid))), GetPlayer()->GetName().c_str(), GetPlayer()->GetGUIDLow(), GetAccountId());
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
    ObjectGuid guid1;
    ObjectGuid guid2;
    uint32 data;
    float x, y, z;

    // Data Comes Single
    recvData >> data;

    // Position
    recvData >> y;
    recvData >> z;
    recvData >> x;

    guid1[1] = recvData.ReadBit();
    guid1[0] = recvData.ReadBit();
    guid1[6] = recvData.ReadBit();
    guid1[7] = recvData.ReadBit();
    guid1[5] = recvData.ReadBit();
    guid2[7] = recvData.ReadBit();
    guid1[2] = recvData.ReadBit();
    guid1[3] = recvData.ReadBit();
    guid2[6] = recvData.ReadBit();
    guid2[3] = recvData.ReadBit();
    guid2[0] = recvData.ReadBit();
    guid2[2] = recvData.ReadBit();
    guid2[5] = recvData.ReadBit();
    guid1[4] = recvData.ReadBit();
    guid2[4] = recvData.ReadBit();
    guid2[1] = recvData.ReadBit();

    recvData.ReadByteSeq(guid1[7]);
    recvData.ReadByteSeq(guid1[6]);
    recvData.ReadByteSeq(guid1[1]);
    recvData.ReadByteSeq(guid1[2]);
    recvData.ReadByteSeq(guid1[5]);
    recvData.ReadByteSeq(guid1[4]);
    recvData.ReadByteSeq(guid2[5]);
    recvData.ReadByteSeq(guid1[3]);
    recvData.ReadByteSeq(guid2[0]);
    recvData.ReadByteSeq(guid2[1]);
    recvData.ReadByteSeq(guid2[7]);
    recvData.ReadByteSeq(guid2[4]);
    recvData.ReadByteSeq(guid2[6]);
    recvData.ReadByteSeq(guid2[2]);
    recvData.ReadByteSeq(guid2[3]);
    recvData.ReadByteSeq(guid1[0]);

    uint32 spellid = UNIT_ACTION_BUTTON_ACTION(data);
    uint8 flag = UNIT_ACTION_BUTTON_TYPE(data);             //delete = 0x07 CastSpell = C1

    // used also for charmed creature
    Unit* pet= ObjectAccessor::GetUnit(*_player, guid1);
    TC_LOG_INFO("network", "HandlePetAction: Pet %u - flag: %u, spellid: %u, target: %u.", uint32(GUID_LOPART(guid1)), uint32(flag), spellid, uint32(GUID_LOPART(guid2)));

    if (!pet)
    {
        TC_LOG_ERROR("network", "HandlePetAction: Pet (GUID: %u) doesn't exist for player %s (GUID: %u)", uint32(GUID_LOPART(guid1)), GetPlayer()->GetName().c_str(), GUID_LOPART(GetPlayer()->GetGUID()));
        return;
    }

    if (pet != GetPlayer()->GetFirstControlled())
    {
        TC_LOG_ERROR("network", "HandlePetAction: Pet (GUID: %u) does not belong to player %s (GUID: %u)", uint32(GUID_LOPART(guid1)), GetPlayer()->GetName().c_str(), GUID_LOPART(GetPlayer()->GetGUID()));
        return;
    }

    if (!pet->IsAlive())
    {
        SpellInfo const* spell = (flag == ACT_ENABLED || flag == ACT_PASSIVE) ? sSpellMgr->GetSpellInfo(spellid) : NULL;
        if (!spell)
            return;
        if (!(spell->Attributes & SPELL_ATTR0_CASTABLE_WHILE_DEAD))
            return;
    }

    /// @todo allow control charmed player?
    if (pet->GetTypeId() == TYPEID_PLAYER && !(flag == ACT_COMMAND && spellid == COMMAND_ATTACK))
        return;

    if (GetPlayer()->m_Controlled.size() == 1)
        HandlePetActionHelper(pet, guid1, spellid, flag, guid2, x, y, z);
    else
    {
        //If a pet is dismissed, m_Controlled will change
        std::vector<Unit*> controlled;
        for (Unit::ControlList::iterator itr = GetPlayer()->m_Controlled.begin(); itr != GetPlayer()->m_Controlled.end(); ++itr)
            if ((*itr)->GetEntry() == pet->GetEntry() && (*itr)->IsAlive())
                controlled.push_back(*itr);
        for (std::vector<Unit*>::iterator itr = controlled.begin(); itr != controlled.end(); ++itr)
            HandlePetActionHelper(*itr, guid1, spellid, flag, guid2, x, y, z);
    }
}

void WorldSession::HandlePetStopAttack(WorldPacket &recvData)
{
    ObjectGuid guid;

    guid[7] = recvData.ReadBit();  // 23
    guid[5] = recvData.ReadBit();  // 21
    guid[1] = recvData.ReadBit();  // 17
    guid[6] = recvData.ReadBit();  // 22
    guid[0] = recvData.ReadBit();  // 16
    guid[2] = recvData.ReadBit();  // 18
    guid[4] = recvData.ReadBit();  // 20
    guid[3] = recvData.ReadBit();  // 19

    recvData.ReadByteSeq(guid[2]);  // 18
    recvData.ReadByteSeq(guid[5]);  // 21
    recvData.ReadByteSeq(guid[0]);  // 16
    recvData.ReadByteSeq(guid[4]);  // 20
    recvData.ReadByteSeq(guid[1]);  // 17
    recvData.ReadByteSeq(guid[7]);  // 23
    recvData.ReadByteSeq(guid[6]);  // 22
    recvData.ReadByteSeq(guid[3]);  // 19

    TC_LOG_DEBUG("network", "WORLD: Received CMSG_PET_STOP_ATTACK for GUID " UI64FMTD "", uint64(guid));

    Unit* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*_player, uint64(guid));

    if (!pet)
    {
        TC_LOG_ERROR("network", "HandlePetStopAttack: Pet %u does not exist", uint32(GUID_LOPART(uint64(guid))));
        return;
    }

    if (pet != GetPlayer()->GetPet() && pet != GetPlayer()->GetCharm())
    {
        TC_LOG_ERROR("network", "HandlePetStopAttack: Pet GUID %u isn't a pet or charmed creature of player %s",
            uint32(GUID_LOPART(uint64(guid))), GetPlayer()->GetName().c_str());
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
        TC_LOG_ERROR("network", "WorldSession::HandlePetAction(petGuid: " UI64FMTD ", tagGuid: " UI64FMTD ", spellId: %u, flag: %u): object (entry: %u TypeId: %u) is considered pet-like but doesn't have a charminfo!",
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
                    TC_LOG_ERROR("network", "WORLD: unknown PET flag Action %i and spellid %i.", uint32(flag), spellid);
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
                TC_LOG_ERROR("network", "WORLD: unknown PET spell id %i", spellid);
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
            TC_LOG_ERROR("network", "WORLD: unknown PET flag Action %i and spellid %i.", uint32(flag), spellid);
    }
}

void WorldSession::HandlePetNameQuery(WorldPacket& recvData)
{
    TC_LOG_INFO("network", "HandlePetNameQuery. CMSG_PET_NAME_QUERY");

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
            TC_LOG_DEBUG("network", "Player (GUID:%u) attempt open stable in cheating way.", GUID_LOPART(guid));
            return false;
        }
    }
    // stable master case
    else
    {
        if (!GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_STABLEMASTER))
        {
            TC_LOG_DEBUG("network", "Stablemaster (GUID:%u) not found or you can't interact with him.", GUID_LOPART(guid));
            return false;
        }
    }
    return true;
}

void WorldSession::HandlePetSetAction(WorldPacket& recvData)
{
    TC_LOG_INFO("network", "HandlePetSetAction. CMSG_PET_SET_ACTION");

    uint64 petguid;
    uint8  count;

    recvData >> petguid;

    Unit* pet = ObjectAccessor::GetUnit(*_player, petguid);

    if (!pet || pet != _player->GetFirstControlled())
    {
        TC_LOG_ERROR("network", "HandlePetSetAction: Unknown pet (GUID: %u) or pet owner (GUID: %u)", GUID_LOPART(petguid), _player->GetGUIDLow());
        return;
    }

    CharmInfo* charmInfo = pet->GetCharmInfo();
    if (!charmInfo)
    {
        TC_LOG_ERROR("network", "WorldSession::HandlePetSetAction: object (GUID: %u TypeId: %u) is considered pet-like but doesn't have a charminfo!", pet->GetGUIDLow(), pet->GetTypeId());
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

        TC_LOG_INFO("network", "Player %s has changed pet spell action. Position: %u, Spell: %u, State: 0x%X",
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
    TC_LOG_INFO("network", "HandlePetRename. CMSG_PET_RENAME");

    uint64 petguid;
    uint8 isdeclined;

    std::string name;
    DeclinedName declinedname;

    recvData >> petguid;
    recvData >> name;
    recvData >> isdeclined;

    Pet* pet = ObjectAccessor::FindPet(petguid);
                                                            // check it!
    if (!pet || !pet->IsPet() || ((Pet*)pet)->getPetType()!= HUNTER_PET ||
        !pet->HasByteFlag(UNIT_FIELD_SHAPESHIFT_FORM, 2, UNIT_CAN_BE_RENAMED) ||
        pet->GetOwnerGUID() != _player->GetGUID() || !pet->GetCharmInfo())
        return;

    PetNameInvalidReason res = ObjectMgr::CheckPetName(name);
    if (res != PET_NAME_SUCCESS)
    {
        SendPetNameInvalid(res, name, NULL);
        return;
    }

    if (sObjectMgr->IsReservedName(name))
    {
        SendPetNameInvalid(PET_NAME_RESERVED, name, NULL);
        return;
    }

    pet->SetName(name);

    Player* owner = pet->GetOwner();
    if (owner && owner->GetGroup())
        owner->SetGroupUpdateFlag(GROUP_UPDATE_FLAG_PET_NAME);

    pet->RemoveByteFlag(UNIT_FIELD_SHAPESHIFT_FORM, 2, UNIT_CAN_BE_RENAMED);

    if (isdeclined)
    {
        for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
        {
            recvData >> declinedname.name[i];
        }

        std::wstring wname;
        if (!Utf8toWStr(name, wname))
            return;

        if (!ObjectMgr::CheckDeclinedNames(wname, declinedname))
        {
            SendPetNameInvalid(PET_NAME_DECLENSION_DOESNT_MATCH_BASE_NAME, name, &declinedname);
            return;
        }
    }

    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    if (isdeclined)
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_PET_DECLINEDNAME);
        stmt->setUInt32(0, pet->GetCharmInfo()->GetPetNumber());
        trans->Append(stmt);

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_ADD_CHAR_PET_DECLINEDNAME);
        stmt->setUInt32(0, _player->GetGUIDLow());

        for (uint8 i = 0; i < 5; i++)
            stmt->setString(i+1, declinedname.name[i]);

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
    uint64 guid;
    recvData >> guid;                                      //pet guid
    TC_LOG_INFO("network", "HandlePetAbandon. CMSG_PET_ABANDON pet guid is %u", GUID_LOPART(guid));

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
    TC_LOG_INFO("network", "CMSG_PET_SPELL_AUTOCAST");

    ObjectGuid guid;
    uint32 spellId;
    bool enabled;

    recvPacket >> spellId;

    guid[0] = recvPacket.ReadBit();  // 16
    guid[4] = recvPacket.ReadBit();  // 20
    guid[2] = recvPacket.ReadBit();  // 18
    guid[6] = recvPacket.ReadBit();  // 22
    guid[1] = recvPacket.ReadBit();  // 17
    guid[5] = recvPacket.ReadBit();  // 21
    guid[3] = recvPacket.ReadBit();  // 19
    guid[7] = recvPacket.ReadBit();  // 23
    enabled = recvPacket.ReadBit();  // 28 - AutocastEnabled

    recvPacket.ReadByteSeq(guid[5]);  // 21
    recvPacket.ReadByteSeq(guid[0]);  // 16
    recvPacket.ReadByteSeq(guid[4]);  // 20
    recvPacket.ReadByteSeq(guid[1]);  // 17
    recvPacket.ReadByteSeq(guid[7]);  // 23
    recvPacket.ReadByteSeq(guid[2]);  // 18
    recvPacket.ReadByteSeq(guid[3]);  // 19
    recvPacket.ReadByteSeq(guid[6]);  // 22

    if (!_player->GetGuardianPet() && !_player->GetCharm())
        return;

    if (ObjectAccessor::FindPlayer(guid))
        return;

    Creature* pet=ObjectAccessor::GetCreatureOrPetOrVehicle(*_player, guid);

    if (!pet || (pet != _player->GetGuardianPet() && pet != _player->GetCharm()))
    {
        TC_LOG_ERROR("network", "HandlePetSpellAutocastOpcode.Pet %u isn't pet of player %s (GUID: %u).", uint32(GUID_LOPART(guid)), GetPlayer()->GetName().c_str(), GUID_LOPART(GetPlayer()->GetGUID()));
        return;
    }

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    // do not add not learned spells/ passive spells
    if (!pet->HasSpell(spellId) || !spellInfo->IsAutocastable())
        return;

    CharmInfo* charmInfo = pet->GetCharmInfo();
    if (!charmInfo)
    {
        TC_LOG_ERROR("network", "WorldSession::HandlePetSpellAutocastOpcod: object (GUID: %u TypeId: %u) is considered pet-like but doesn't have a charminfo!", pet->GetGUIDLow(), pet->GetTypeId());
        return;
    }

    if (pet->IsPet())
        ((Pet*)pet)->ToggleAutocast(spellInfo, enabled);
    else
        pet->GetCharmInfo()->ToggleCreatureAutocast(spellInfo, enabled);

    charmInfo->SetSpellAutocast(spellInfo, enabled);
}

void WorldSession::HandlePetCastSpellOpcode(WorldPacket& recvPacket)
{
    TC_LOG_DEBUG("network", "WORLD: CMSG_PET_CAST_SPELL");

    ObjectGuid petGuid = 0;

    uint8 castCount = 0;
    uint8 castFlags = 0;
    uint32 spellId = 0;
    uint32 glyphIndex = 0;
    uint32 targetMask = 0;
    uint32 targetStringLength = 0;
    float elevation = 0.0f;
    float missileSpeed = 0.0f;
    ObjectGuid targetGuid = 0;
    ObjectGuid itemTargetGuid = 0;
    ObjectGuid destTransportGuid = 0;
    ObjectGuid srcTransportGuid = 0;
    Position srcPos;
    Position destPos;
    std::string targetString;

    // Movement data
    MovementInfo movementInfo;
    ObjectGuid movementTransportGuid = 0;
    ObjectGuid movementGuid = 0;
    bool hasTransport = false;
    bool hasTransportTime2 = false;
    bool hasTransportTime3 = false;
    bool hasFallData = false;
    bool hasFallDirection = false;
    bool hasTimestamp = false;
    bool hasSplineElevation = false;
    bool hasPitch = false;
    bool hasOrientation = false;
    bool hasUnkMovementField = false;
    uint32 unkMovementLoopCounter = 0;
    Unit* caster = _player->GetPet();

    bool hasDestLocation = recvPacket.ReadBit();
    petGuid[7] = recvPacket.ReadBit();
    bool hasMissileSpeed = !recvPacket.ReadBit();
    bool hasSrcLocation = recvPacket.ReadBit();
    petGuid[1] = recvPacket.ReadBit();
    uint8 researchDataCount = recvPacket.ReadBits(2);
    bool hasTargetMask = !recvPacket.ReadBit();
    petGuid[4] = recvPacket.ReadBit();
    recvPacket.ReadBit(); // Fake bit
    petGuid[6] = recvPacket.ReadBit();
    bool hasTargetString = !recvPacket.ReadBit();
    recvPacket.ReadBit(); // Fake Bit
    bool hasMovement = recvPacket.ReadBit();
    bool hasCastFlags = !recvPacket.ReadBit();
    bool hasSpellId = !recvPacket.ReadBit();
    petGuid[0] = recvPacket.ReadBit();
    petGuid[5] = recvPacket.ReadBit();
    petGuid[2] = recvPacket.ReadBit();

    for (uint8 i = 0; i < researchDataCount; ++i)
        recvPacket.ReadBits(2);

    petGuid[3] = recvPacket.ReadBit();
    bool hasGlyphIndex = !recvPacket.ReadBit();
    bool hasCastCount = !recvPacket.ReadBit();
    bool hasElevation = !recvPacket.ReadBit();

    if (hasMovement)
    {
        hasOrientation = !recvPacket.ReadBit();
        hasSplineElevation = !recvPacket.ReadBit();
        recvPacket.ReadBit();
        movementGuid[5] = recvPacket.ReadBit();
        movementGuid[7] = recvPacket.ReadBit();
        bool hasMovementFlags2 = !recvPacket.ReadBit();
        hasTimestamp = !recvPacket.ReadBit();
        hasFallData = recvPacket.ReadBit();
        bool hasMovementFlags = !recvPacket.ReadBit();
        hasUnkMovementField = !recvPacket.ReadBit();

        if (hasMovementFlags)
            movementInfo.flags = recvPacket.ReadBits(30);

        recvPacket.ReadBit();
        movementGuid[6] = recvPacket.ReadBit();
        hasTransport = recvPacket.ReadBit();
        movementGuid[0] = recvPacket.ReadBit();
        unkMovementLoopCounter = recvPacket.ReadBits(22);

        if (hasTransport)
        {
            hasTransportTime2 = recvPacket.ReadBit();
            hasTransportTime3 = recvPacket.ReadBit();
            movementTransportGuid[5] = recvPacket.ReadBit();
            movementTransportGuid[6] = recvPacket.ReadBit();
            movementTransportGuid[4] = recvPacket.ReadBit();
            movementTransportGuid[0] = recvPacket.ReadBit();
            movementTransportGuid[1] = recvPacket.ReadBit();
            movementTransportGuid[2] = recvPacket.ReadBit();
            movementTransportGuid[7] = recvPacket.ReadBit();
            movementTransportGuid[3] = recvPacket.ReadBit();
        }

        movementGuid[1] = recvPacket.ReadBit();

        if (hasMovementFlags2)
            movementInfo.flags2 = recvPacket.ReadBits(13);

        movementGuid[3] = recvPacket.ReadBit();
        movementGuid[2] = recvPacket.ReadBit();
        recvPacket.ReadBit();
        hasPitch = !recvPacket.ReadBit();
        movementGuid[4] = recvPacket.ReadBit();

        if (hasFallData)
            hasFallDirection = recvPacket.ReadBit();
    }

    if (hasDestLocation)
    {
        destTransportGuid[2] = recvPacket.ReadBit();
        destTransportGuid[0] = recvPacket.ReadBit();
        destTransportGuid[1] = recvPacket.ReadBit();
        destTransportGuid[4] = recvPacket.ReadBit();
        destTransportGuid[5] = recvPacket.ReadBit();
        destTransportGuid[6] = recvPacket.ReadBit();
        destTransportGuid[3] = recvPacket.ReadBit();
        destTransportGuid[7] = recvPacket.ReadBit();
    }

    if (hasCastFlags)
        castFlags = recvPacket.ReadBits(5);

    targetGuid[2] = recvPacket.ReadBit();
    targetGuid[4] = recvPacket.ReadBit();
    targetGuid[7] = recvPacket.ReadBit();
    targetGuid[0] = recvPacket.ReadBit();
    targetGuid[6] = recvPacket.ReadBit();
    targetGuid[1] = recvPacket.ReadBit();
    targetGuid[5] = recvPacket.ReadBit();
    targetGuid[3] = recvPacket.ReadBit();

    if (hasTargetMask)
        targetMask = recvPacket.ReadBits(20);

    if (hasTargetString)
        targetStringLength = recvPacket.ReadBits(7);

    if (hasSrcLocation)
    {
        srcTransportGuid[2] = recvPacket.ReadBit();
        srcTransportGuid[0] = recvPacket.ReadBit();
        srcTransportGuid[3] = recvPacket.ReadBit();
        srcTransportGuid[1] = recvPacket.ReadBit();
        srcTransportGuid[6] = recvPacket.ReadBit();
        srcTransportGuid[7] = recvPacket.ReadBit();
        srcTransportGuid[4] = recvPacket.ReadBit();
        srcTransportGuid[5] = recvPacket.ReadBit();
    }

    itemTargetGuid[6] = recvPacket.ReadBit();
    itemTargetGuid[0] = recvPacket.ReadBit();
    itemTargetGuid[3] = recvPacket.ReadBit();
    itemTargetGuid[4] = recvPacket.ReadBit();
    itemTargetGuid[2] = recvPacket.ReadBit();
    itemTargetGuid[1] = recvPacket.ReadBit();
    itemTargetGuid[5] = recvPacket.ReadBit();
    itemTargetGuid[7] = recvPacket.ReadBit();


    recvPacket.ReadByteSeq(petGuid[2]);
    recvPacket.ReadByteSeq(petGuid[6]);
    recvPacket.ReadByteSeq(petGuid[3]);

    for (uint8 i = 0; i < researchDataCount; ++i)
    {
        recvPacket.read_skip<uint32>();
        recvPacket.read_skip<uint32>();
    }

    recvPacket.ReadByteSeq(petGuid[1]);
    recvPacket.ReadByteSeq(petGuid[7]);
    recvPacket.ReadByteSeq(petGuid[0]);
    recvPacket.ReadByteSeq(petGuid[4]);
    recvPacket.ReadByteSeq(petGuid[5]);
    
    if (hasDestLocation)
    {
        float x, y, z;
        recvPacket.ReadByteSeq(destTransportGuid[4]);
        recvPacket.ReadByteSeq(destTransportGuid[1]);
        recvPacket.ReadByteSeq(destTransportGuid[7]);
        recvPacket >> z;
        recvPacket >> y;
        recvPacket.ReadByteSeq(destTransportGuid[6]);
        recvPacket.ReadByteSeq(destTransportGuid[3]);
        recvPacket >> x;
        recvPacket.ReadByteSeq(destTransportGuid[2]);
        recvPacket.ReadByteSeq(destTransportGuid[5]);
        recvPacket.ReadByteSeq(destTransportGuid[0]);
        destPos.Relocate(x, y, z);
    }
    else
    {
        destTransportGuid = caster->GetTransGUID();
        if (destTransportGuid)
            destPos.Relocate(caster->GetTransOffsetX(), caster->GetTransOffsetY(), caster->GetTransOffsetZ(), caster->GetTransOffsetO());
        else
            destPos.Relocate(caster);
    }

    if (hasMovement)
    {
        if (hasPitch)
            movementInfo.pitch = G3D::wrap(recvPacket.read<float>(), float(-M_PI), float(M_PI));

        if (hasTransport)
        {
            if (hasTransportTime3)
                recvPacket >> movementInfo.transport.time3;
            
            if (hasTransportTime2)
                recvPacket >> movementInfo.transport.time2;

            recvPacket >> movementInfo.transport.seat;
            movementInfo.transport.pos.SetOrientation(recvPacket.read<float>());
            recvPacket >> movementInfo.transport.pos.m_positionZ;
            recvPacket.ReadByteSeq(movementTransportGuid[2]);
            recvPacket >> movementInfo.transport.time;
            recvPacket.ReadByteSeq(movementTransportGuid[3]);
            recvPacket >> movementInfo.transport.pos.m_positionX;
            recvPacket.ReadByteSeq(movementTransportGuid[6]);
            recvPacket.ReadByteSeq(movementTransportGuid[5]);
            recvPacket.ReadByteSeq(movementTransportGuid[7]);
            recvPacket.ReadByteSeq(movementTransportGuid[0]);
            recvPacket >> movementInfo.transport.pos.m_positionY;
            recvPacket.ReadByteSeq(movementTransportGuid[4]);
            recvPacket.ReadByteSeq(movementTransportGuid[1]);
        }

        if (hasUnkMovementField)
            recvPacket.read_skip<uint32>();

        for (uint8 i = 0; i != unkMovementLoopCounter; i++)
            recvPacket.read_skip<uint32>();

        recvPacket.ReadByteSeq(movementGuid[3]);

        if (hasOrientation)
            movementInfo.pos.SetOrientation(recvPacket.read<float>());

        recvPacket.ReadByteSeq(movementGuid[5]);

        if (hasFallData)
        {
            recvPacket >> movementInfo.jump.zspeed;

            if (hasFallDirection)
            {
                recvPacket >> movementInfo.jump.cosAngle;
                recvPacket >> movementInfo.jump.xyspeed;
                recvPacket >> movementInfo.jump.sinAngle;
            }

            recvPacket >> movementInfo.jump.fallTime;
        }

        if (hasTimestamp)
            recvPacket >> movementInfo.time;

        recvPacket.ReadByteSeq(movementGuid[6]);
        recvPacket >> movementInfo.pos.m_positionX;
        recvPacket.ReadByteSeq(movementGuid[1]);
        recvPacket >> movementInfo.pos.m_positionZ;
        recvPacket.ReadByteSeq(movementGuid[2]);
        recvPacket.ReadByteSeq(movementGuid[7]);
        recvPacket.ReadByteSeq(movementGuid[0]);
        recvPacket >> movementInfo.pos.m_positionY;
        recvPacket.ReadByteSeq(movementGuid[4]);
        
        if (hasSplineElevation)
            recvPacket >> movementInfo.splineElevation;
    }

    if (hasSrcLocation)
    {
        float x, y, z;
        recvPacket.ReadByteSeq(srcTransportGuid[3]);
        recvPacket.ReadByteSeq(srcTransportGuid[4]);
        recvPacket.ReadByteSeq(srcTransportGuid[2]);
        recvPacket.ReadByteSeq(srcTransportGuid[1]);
        recvPacket.ReadByteSeq(srcTransportGuid[0]);
        recvPacket.ReadByteSeq(srcTransportGuid[7]);
        recvPacket >> z;
        recvPacket.ReadByteSeq(srcTransportGuid[6]);
        recvPacket.ReadByteSeq(srcTransportGuid[5]);
        recvPacket >> x;
        recvPacket >> y;
        srcPos.Relocate(x, y, z);
    }
    else
    {
        srcTransportGuid = caster->GetTransGUID();
        if (srcTransportGuid)
            srcPos.Relocate(caster->GetTransOffsetX(), caster->GetTransOffsetY(), caster->GetTransOffsetZ(), caster->GetTransOffsetO());
        else
            srcPos.Relocate(caster);
    }

    if (hasMissileSpeed)
        recvPacket >> missileSpeed;

    recvPacket.ReadByteSeq(targetGuid[1]);
    recvPacket.ReadByteSeq(targetGuid[2]);
    recvPacket.ReadByteSeq(targetGuid[5]);
    recvPacket.ReadByteSeq(targetGuid[7]);
    recvPacket.ReadByteSeq(targetGuid[4]);
    recvPacket.ReadByteSeq(targetGuid[6]);
    recvPacket.ReadByteSeq(targetGuid[3]);
    recvPacket.ReadByteSeq(targetGuid[0]);

    recvPacket.ReadByteSeq(itemTargetGuid[1]);
    recvPacket.ReadByteSeq(itemTargetGuid[5]);
    recvPacket.ReadByteSeq(itemTargetGuid[7]);
    recvPacket.ReadByteSeq(itemTargetGuid[3]);
    recvPacket.ReadByteSeq(itemTargetGuid[0]);
    recvPacket.ReadByteSeq(itemTargetGuid[2]);
    recvPacket.ReadByteSeq(itemTargetGuid[4]);
    recvPacket.ReadByteSeq(itemTargetGuid[6]);

    if (hasElevation)
        recvPacket >> elevation;

    if (hasCastCount)
        recvPacket >> castCount;
    
    if (hasTargetString)
        targetString = recvPacket.ReadString(targetStringLength);

    if (hasGlyphIndex)
        recvPacket >> glyphIndex;

    if (hasSpellId)
        recvPacket >> spellId;
    
    TC_LOG_DEBUG("network", "WORLD: CMSG_PET_CAST_SPELL, guid: " UI64FMTD ", castCount: %u, spellId %u, castFlags %u", uint64(petGuid), castCount, spellId, castFlags);

    // This opcode is also sent from charmed and possessed units (players and creatures)
    if (!_player->GetGuardianPet() && !_player->GetCharm())
    {
        recvPacket.rfinish(); // prevent spam at ignore packet
        return;
    }

    caster = ObjectAccessor::GetUnit(*_player, petGuid);

    if (!caster || (caster != _player->GetGuardianPet() && caster != _player->GetCharm()))
    {
        TC_LOG_ERROR("network", "HandlePetCastSpellOpcode: Pet %u isn't pet of player %s (GUID: %u).", uint32(GUID_LOPART(petGuid)), GetPlayer()->GetName().c_str(), GUID_LOPART(GetPlayer()->GetGUID()));
        recvPacket.rfinish(); // prevent spam at ignore packet
        return;
    }

    TC_LOG_DEBUG("network", "WORLD: got cast pet spell packet, castCount: %u, spellId: %u, castFlags: %u, data length = %u", castCount, spellId, castFlags, (uint32)recvPacket.size());

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo)
    {
        TC_LOG_ERROR("network", "WORLD: unknown PET spell id %u", spellId);
        recvPacket.rfinish(); // prevent spam at ignore packet
        return;
    }

    // do not cast not learned spells
    if (!caster->HasSpell(spellId) || spellInfo->IsPassive())
    {
        recvPacket.rfinish(); // prevent spam at ignore packet
        return;
    }

    // client provided targets
    SpellCastTargets targets(caster, targetMask, targetGuid, itemTargetGuid, srcTransportGuid, destTransportGuid, srcPos, destPos, elevation, missileSpeed, targetString);

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
                    pet->SendPetAIReaction(petGuid);
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

void WorldSession::SendPetNameInvalid(uint32 error, const std::string& name, DeclinedName *declinedName)
{
    WorldPacket data(SMSG_PET_NAME_INVALID, 4 + name.size() + 1 + 1);
    data << uint32(error);
    data << name;
    data << uint8(declinedName ? 1 : 0);
    if (declinedName)
        for (uint32 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
            data << declinedName->name[i];

    SendPacket(&data);
}

void WorldSession::HandeLearnPetSpecializationGroup(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "CMSG_LEARN_PET_SPECIALIZATION_GROUP");

    uint32 specGroupIndex;
    ObjectGuid petGUID;

    recvData >> specGroupIndex;

    petGUID[5] = recvData.ReadBit();  // 21
    petGUID[7] = recvData.ReadBit();  // 23
    petGUID[3] = recvData.ReadBit();  // 19
    petGUID[0] = recvData.ReadBit();  // 16
    petGUID[6] = recvData.ReadBit();  // 22
    petGUID[4] = recvData.ReadBit();  // 20
    petGUID[1] = recvData.ReadBit();  // 17
    petGUID[2] = recvData.ReadBit();  // 18

    recvData.ReadByteSeq(petGUID[7]);  // 23
    recvData.ReadByteSeq(petGUID[5]);  // 21
    recvData.ReadByteSeq(petGUID[4]);  // 20
    recvData.ReadByteSeq(petGUID[3]);  // 19
    recvData.ReadByteSeq(petGUID[0]);  // 16
    recvData.ReadByteSeq(petGUID[2]);  // 18
    recvData.ReadByteSeq(petGUID[6]);  // 22
    recvData.ReadByteSeq(petGUID[1]);  // 17

    if (_player->GetPet()->GetGUID() != uint64(petGUID))
    {
        TC_LOG_DEBUG("network", "HandeLearnPetSpecializationGroup: Received wrong Pet GUID (%u) from Player %s (GUID: %u)", GUID_LOPART(petGUID), _player->GetName().c_str(), _player->GetGUID());
        return;
    }

    Pet* pet = _player->GetPet();
    if (!pet)
        return;

    if (_player->IsInCombat() || pet->getLevel() < 15)
        return;

    if (specGroupIndex > MAX_TALENT_TABS)
        return;
    
    uint32 specId;
    switch (specGroupIndex)
    {
        case 0: specId = SPEC_PET_FEROCITY; break;
        case 1: specId = SPEC_PET_TENACITY; break;
        case 2: specId = SPEC_PET_CUNNING;  break;
        default: break;
    }

    if (pet->GetSpecialization() != SPEC_NONE)
        pet->UnlearnSpecializationSpells();

    TC_LOG_DEBUG("network", "HandeLearnPetSpecializationGroup: Pet GUID (%u) from Player %s (GUID: %u) wants to learn specialization %u", GUID_LOPART(petGUID), _player->GetName().c_str(), _player->GetGUID(), specId);

    pet->SetSpecialization(specId);
    pet->SendPetSpecialization();
    pet->LearnSpecializationSpells();

    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_SPECIALIZATION);

    std::ostringstream ss;
    for (uint8 i = 0; i < MAX_TALENT_SPECS; ++i)
        ss << pet->GetSpecialization(i) << " ";
    stmt->setString(0, ss.str());

    stmt->setUInt8(1, pet->GetSpecializationCount());
    stmt->setUInt8(2, pet->GetActiveSpecialization());

    stmt->setUInt32(3, _player->GetGUIDLow());
    stmt->setUInt32(4, pet->GetCharmInfo()->GetPetNumber());
    trans->Append(stmt);

    CharacterDatabase.CommitTransaction(trans);
}
