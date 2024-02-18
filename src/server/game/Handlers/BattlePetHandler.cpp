/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "BattlePet.h"
#include "BattlePetMgr.h"
#include "Common.h"
#include "DB2Enums.h"
#include "DB2Stores.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "TemporarySummon.h"
#include "WorldSession.h"
#include "WorldPacket.h"

void WorldSession::HandlePetBattleStartPvpMatchmaking(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_PET_BATTLE_START_PVP_MATCHMAKING");

    WorldPacket data(SMSG_PET_BATTLE_QUEUE_STATUS, 4+4+4+4+4+4+4+4+4+8+1);

    //CliRideTicket.RequesterGuid
    ObjectGuid guid = GetPlayer()->GetGUID();

    data.WriteGuidMask(guid, 7, 2, 6, 1);
    data.WriteBit(0); // 20 hasAverageWaitTime
    data.WriteGuidMask(guid, 4);
    data.WriteBits(0, 22);
    data.WriteGuidMask(guid, 0);
    data.WriteBit(0); // 48 hasClientWaitTime
    data.WriteGuidMask(guid, 3, 5);

    data.FlushBits();
    data.WriteGuidBytes(guid, 2, 4);
    data << uint32(0); // 72 CliRideTicket.Time
    data.WriteGuidBytes(guid, 3);
    data << uint32(1); // 24 Status // ERR_PETBATTLE_QUEUE_ status, 1 = ERR_PETBATTLE_QUEUE_QUEUED
    data.WriteGuidBytes(guid, 6);
    //if (hasClientWaitTime)
    //data << uint32(0) // 44
    data.WriteGuidBytes(guid, 1);
    data << uint32(0); // 68 CliRideTicket.Type
    data.WriteGuidBytes(guid, 5, 7);
    data << uint32(0); // 64 CliRideTicket.Id
    data.WriteGuidBytes(guid, 0);
    
    for (uint8 i = 0; i < 3; i++)
        data << uint32(1); // SlotResult

    //if (hasAverageWaitTime)
    // data << uint32(0); // 16

    SendPacket(&data);
}

void WorldSession::HandlePetBattleStopPvpMatchmaking(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_PET_BATTLE_STOP_PVP_MATCHMAKING");
    //CliRideTicket
}

void WorldSession::HandleBattlePetDelete(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_BATTLE_PET_DELETE");

    ObjectGuid petEntry;

    petEntry[3] = recvData.ReadBit();
    petEntry[5] = recvData.ReadBit();
    petEntry[6] = recvData.ReadBit();
    petEntry[2] = recvData.ReadBit();
    petEntry[4] = recvData.ReadBit();
    petEntry[0] = recvData.ReadBit();
    petEntry[7] = recvData.ReadBit();
    petEntry[1] = recvData.ReadBit();

    recvData.ReadByteSeq(petEntry[4]);
    recvData.ReadByteSeq(petEntry[1]);
    recvData.ReadByteSeq(petEntry[5]);
    recvData.ReadByteSeq(petEntry[0]);
    recvData.ReadByteSeq(petEntry[7]);
    recvData.ReadByteSeq(petEntry[2]);
    recvData.ReadByteSeq(petEntry[3]);
    recvData.ReadByteSeq(petEntry[6]);

    BattlePetMgr* battlePetMgr = GetPlayer()->GetBattlePetMgr();

    BattlePet* battlePet = battlePetMgr->GetBattlePet(petEntry);
    if (!battlePet)
    {
        SF_LOG_DEBUG("network", "CMSG_BATTLE_PET_DELETE - Player %u tryed to release Battle Pet %lu which it doesn't own!",
            GetPlayer()->GetGUIDLow(), (uint64)petEntry);
        return;
    }

    if (!BattlePetSpeciesHasFlag(battlePet->GetSpecies(), BATTLE_PET_FLAG_RELEASABLE))
    {
        SF_LOG_DEBUG("network", "CMSG_BATTLE_PET_DELETE - Player %u tryed to release Battle Pet %lu which isn't releasable!",
            GetPlayer()->GetGUIDLow(), (uint64)petEntry);
        return;
    }

    battlePetMgr->Delete(battlePet);
}

#define BATTLE_PET_MAX_DECLINED_NAMES 5

void WorldSession::HandleBattlePetModifyName(WorldPacket& recvData)
{
    // TODO: finish this...
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_BATTLE_PET_MODIFY_NAME");

    ObjectGuid petEntry;
    uint8 nicknameLen;
    std::string nickname;
    bool hasDeclinedNames;
    uint8 declinedNameLen[BATTLE_PET_MAX_DECLINED_NAMES];

    petEntry[5] = recvData.ReadBit();
    petEntry[7] = recvData.ReadBit();
    petEntry[3] = recvData.ReadBit();
    petEntry[0] = recvData.ReadBit();
    petEntry[6] = recvData.ReadBit();
    nicknameLen = recvData.ReadBits(7);
    petEntry[2] = recvData.ReadBit();
    petEntry[1] = recvData.ReadBit();
    hasDeclinedNames = recvData.ReadBit();
    petEntry[4] = recvData.ReadBit();

    if (hasDeclinedNames)
        for (uint8 i = 0; i < BATTLE_PET_MAX_DECLINED_NAMES; i++)
            declinedNameLen[i] = recvData.ReadBits(7);

    recvData.ReadByteSeq(petEntry[3]);
    recvData.ReadByteSeq(petEntry[0]);
    recvData.ReadByteSeq(petEntry[6]);
    recvData.ReadByteSeq(petEntry[1]);
    recvData.ReadByteSeq(petEntry[5]);
    recvData.ReadByteSeq(petEntry[2]);
    recvData.ReadByteSeq(petEntry[4]);
    recvData.ReadByteSeq(petEntry[7]);
    nickname = recvData.ReadString(nicknameLen);

    if (hasDeclinedNames)
    {
        std::string declinedNames[BATTLE_PET_MAX_DECLINED_NAMES];
        for (uint8 i = 0; i < BATTLE_PET_MAX_DECLINED_NAMES; i++)
            declinedNames[i] = recvData.ReadString(declinedNameLen[i]);
    }

    BattlePetMgr* battlePetMgr = GetPlayer()->GetBattlePetMgr();

    BattlePet* battlePet = battlePetMgr->GetBattlePet(petEntry);
    if (!battlePet)
    {
        SF_LOG_DEBUG("network", "CMSG_BATTLE_PET_MODIFY_NAME - Player %u tryed to set the name for Battle Pet %lu which it doesn't own!",
            GetPlayer()->GetGUIDLow(), (uint64)petEntry);
        return;
    }

    if (nickname.size() > BATTLE_PET_MAX_NAME_LENGTH)
    {
        SF_LOG_DEBUG("network", "CMSG_BATTLE_PET_MODIFY_NAME - Player %u tryed to set the name for Battle Pet %lu with an invalid length!",
            GetPlayer()->GetGUIDLow(), (uint64)petEntry);
        return;
    }

    // TODO: check for invalid characters, ect...

    battlePet->SetNickname(nickname);
    battlePet->SetTimestamp((uint32)time(NULL));

    if (battlePetMgr->GetCurrentSummonId())
        battlePetMgr->GetCurrentSummon()->SetUInt32Value(UNIT_FIELD_BATTLE_PET_COMPANION_NAME_TIMESTAMP, battlePet->GetTimestamp());
}

void WorldSession::HandleBattlePetQueryName(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_BATTLE_PET_QUERY_NAME");

    ObjectGuid petEntry, petguid;

    petguid[2] = recvData.ReadBit();
    petEntry[6] = recvData.ReadBit();
    petEntry[3] = recvData.ReadBit();
    petguid[3] = recvData.ReadBit();
    petEntry[7] = recvData.ReadBit();
    petguid[4] = recvData.ReadBit();
    petguid[1] = recvData.ReadBit();
    petguid[0] = recvData.ReadBit();
    petEntry[0] = recvData.ReadBit();
    petguid[7] = recvData.ReadBit();
    petguid[5] = recvData.ReadBit();
    petguid[6] = recvData.ReadBit();
    petEntry[1] = recvData.ReadBit();
    petEntry[2] = recvData.ReadBit();
    petEntry[5] = recvData.ReadBit();
    petEntry[4] = recvData.ReadBit();

    recvData.ReadByteSeq(petguid[5]);
    recvData.ReadByteSeq(petEntry[1]);
    recvData.ReadByteSeq(petguid[0]);
    recvData.ReadByteSeq(petEntry[4]);
    recvData.ReadByteSeq(petguid[3]);
    recvData.ReadByteSeq(petEntry[3]);
    recvData.ReadByteSeq(petguid[1]);
    recvData.ReadByteSeq(petguid[6]);
    recvData.ReadByteSeq(petEntry[6]);
    recvData.ReadByteSeq(petEntry[0]);
    recvData.ReadByteSeq(petEntry[2]);
    recvData.ReadByteSeq(petguid[7]);
    recvData.ReadByteSeq(petguid[2]);
    recvData.ReadByteSeq(petEntry[7]);
    recvData.ReadByteSeq(petguid[4]);
    recvData.ReadByteSeq(petEntry[5]);

    Unit* tempUnit = sObjectAccessor->FindUnit(petguid);
    if (!tempUnit)
    {
        SF_LOG_DEBUG("network", "CMSG_BATTLE_PET_QUERY_NAME - Player %u queried the name of Battle Pet %lu which doesnt't exist in world!",
            GetPlayer()->GetGUIDLow(), (uint64)petEntry);
        return;
    }

    Unit* ownerUnit = tempUnit->ToTempSummon()->GetSummoner();
    if (!ownerUnit)
        return;

    BattlePetMgr* battlePetMgr = ownerUnit->ToPlayer()->GetBattlePetMgr();

    BattlePet* battlePet = battlePetMgr->GetBattlePet(battlePetMgr->GetCurrentSummonId());
    if (!battlePet)
        return;

    BattlePetSpeciesEntry const* speciesEntry = sBattlePetSpeciesStore.LookupEntry(battlePet->GetSpecies());

    WorldPacket data(SMSG_BATTLE_PET_QUERY_NAME_RESPONSE, 8 + 4 + 4 + 5 + battlePet->GetNickname().size());
    data << uint64(petEntry);
    data << uint32(battlePet->GetTimestamp());
    data << uint32(speciesEntry->NpcId);
    data.WriteBit(1);               // has names
    data.WriteBits(battlePet->GetNickname().size(), 8);

    // TODO: finish declined names
    for (uint8 i = 0; i < BATTLE_PET_MAX_DECLINED_NAMES; i++)
        data.WriteBits(0, 7);

    data.WriteBit(0);               // allowed?
    data.FlushBits();

    data.WriteString(battlePet->GetNickname());

    SendPacket(&data);
}

void WorldSession::HandleBattlePetSetBattleSlot(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_BATTLE_PET_SET_BATTLE_SLOT");

    uint8 slot;
    ObjectGuid petEntry;

    recvData >> slot;

    petEntry[4] = recvData.ReadBit();
    petEntry[6] = recvData.ReadBit();
    petEntry[5] = recvData.ReadBit();
    petEntry[7] = recvData.ReadBit();
    petEntry[3] = recvData.ReadBit();
    petEntry[1] = recvData.ReadBit();
    petEntry[0] = recvData.ReadBit();
    petEntry[2] = recvData.ReadBit();

    recvData.ReadByteSeq(petEntry[1]);
    recvData.ReadByteSeq(petEntry[3]);
    recvData.ReadByteSeq(petEntry[5]);
    recvData.ReadByteSeq(petEntry[0]);
    recvData.ReadByteSeq(petEntry[7]);
    recvData.ReadByteSeq(petEntry[6]);
    recvData.ReadByteSeq(petEntry[4]);
    recvData.ReadByteSeq(petEntry[2]);

    BattlePetMgr* battlePetMgr = GetPlayer()->GetBattlePetMgr();

    BattlePet* battlePet = battlePetMgr->GetBattlePet(petEntry);
    if (!battlePet)
    {
        SF_LOG_DEBUG("network", "CMSG_BATTLE_PET_SET_BATTLE_SLOT - Player %u tryed to add Battle Pet %lu to loadout which it doesn't own!",
            GetPlayer()->GetGUIDLow(), (uint64)petEntry);
        return;
    }

    if (!battlePetMgr->HasLoadoutSlot(slot))
    {
        SF_LOG_DEBUG("network", "CMSG_BATTLE_PET_SET_BATTLE_SLOT - Player %u tryed to add Battle Pet %lu into slot %u which is locked!",
            GetPlayer()->GetGUIDLow(), (uint64)petEntry, slot);
        return;
    }

    // this check is also done clientside
    if (BattlePetSpeciesHasFlag(battlePet->GetSpecies(), BATTLE_PET_FLAG_COMPANION))
    {
        SF_LOG_DEBUG("network", "CMSG_BATTLE_PET_SET_BATTLE_SLOT - Player %u tryed to add a compainion Battle Pet %lu into slot %u!",
            GetPlayer()->GetGUIDLow(), (uint64)petEntry, slot);
        return;
    }

    // sever handles slot swapping, find source slot and replace it with the destination slot
    uint8 srcSlot = battlePetMgr->GetLoadoutSlotForBattlePet(petEntry);
    if (srcSlot != BATTLE_PET_LOADOUT_SLOT_NONE)
        battlePetMgr->SetLoadoutSlot(srcSlot, battlePetMgr->GetLoadoutSlot(slot), true);

    battlePetMgr->SetLoadoutSlot(slot, petEntry, true);
}

enum BattlePetFlagModes
{
    BATTLE_PET_FLAG_MODE_UNSET       = 0,
    BATTLE_PET_FLAG_MODE_SET         = 3
};

void WorldSession::HandleBattlePetSetFlags(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_BATTLE_PET_SET_FLAGS");

    ObjectGuid petEntry;
    uint32 flag;

    recvData >> flag;

    petEntry[5] = recvData.ReadBit();
    petEntry[4] = recvData.ReadBit();
    uint8 mode = recvData.ReadBits(2);
    petEntry[1] = recvData.ReadBit();
    petEntry[4] = recvData.ReadBit();
    petEntry[6] = recvData.ReadBit();
    petEntry[3] = recvData.ReadBit();
    petEntry[7] = recvData.ReadBit();
    petEntry[0] = recvData.ReadBit();

    recvData.ReadByteSeq(petEntry[4]);
    recvData.ReadByteSeq(petEntry[0]);
    recvData.ReadByteSeq(petEntry[7]);
    recvData.ReadByteSeq(petEntry[3]);
    recvData.ReadByteSeq(petEntry[1]);
    recvData.ReadByteSeq(petEntry[6]);
    recvData.ReadByteSeq(petEntry[2]);
    recvData.ReadByteSeq(petEntry[5]);

    BattlePet* battlePet = GetPlayer()->GetBattlePetMgr()->GetBattlePet(petEntry);
    if (!battlePet)
    {
        SF_LOG_DEBUG("network", "CMSG_BATTLE_PET_SET_FLAGS - Player %u tryed to set the flags for Battle Pet %lu which it doesn't own!",
            GetPlayer()->GetGUIDLow(), (uint64)petEntry);
        return;
    }

    // list of flags the client can currently change
    if (flag != BATTLE_PET_JOURNAL_FLAG_FAVORITES
        && flag != BATTLE_PET_JOURNAL_FLAG_ABILITY_1
        && flag != BATTLE_PET_JOURNAL_FLAG_ABILITY_2
        && flag != BATTLE_PET_JOURNAL_FLAG_ABILITY_3)
    {
        SF_LOG_DEBUG("network", "CMSG_BATTLE_PET_SET_FLAGS - Player %u tryed to set an invalid Battle Pet flag %u!",
            GetPlayer()->GetGUIDLow(), flag);
        return;
    }

    // TODO: check if Battle Pet is correct level for ability

    switch (mode)
    {
        case BATTLE_PET_FLAG_MODE_SET:
            battlePet->SetFlag(flag);
            break;
        case BATTLE_PET_FLAG_MODE_UNSET:
            battlePet->UnSetFlag(flag);
            break;
    }
}

void WorldSession::HandleBattlePetSummonCompanion(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_BATTLE_PET_SUMMON_COMPANION");

    ObjectGuid petEntry;

    petEntry[3] = recvData.ReadBit();
    petEntry[2] = recvData.ReadBit();
    petEntry[5] = recvData.ReadBit();
    petEntry[0] = recvData.ReadBit();
    petEntry[7] = recvData.ReadBit();
    petEntry[1] = recvData.ReadBit();
    petEntry[6] = recvData.ReadBit();
    petEntry[4] = recvData.ReadBit();

    recvData.ReadByteSeq(petEntry[6]);
    recvData.ReadByteSeq(petEntry[7]);
    recvData.ReadByteSeq(petEntry[3]);
    recvData.ReadByteSeq(petEntry[5]);
    recvData.ReadByteSeq(petEntry[0]);
    recvData.ReadByteSeq(petEntry[4]);
    recvData.ReadByteSeq(petEntry[1]);
    recvData.ReadByteSeq(petEntry[2]);

    Player* player = GetPlayer();
    BattlePetMgr* battlePetMgr = player->GetBattlePetMgr();

    BattlePet* battlePet = battlePetMgr->GetBattlePet(petEntry);
    if (!battlePet)
    {
        SF_LOG_DEBUG("network", "CMSG_SUMMON_BATTLE_PET_COMPANION - Player %u tryed to summon battle pet companion %lu which it doesn't own!",
            player->GetGUIDLow(), (uint64)petEntry);
        return;
    }

    if (!battlePet->GetCurrentHealth())
    {
        SF_LOG_DEBUG("network", "CMSG_SUMMON_BATTLE_PET_COMPANION - Player %u tryed to summon battle pet companion %lu which is dead!",
            player->GetGUIDLow(), (uint64)petEntry);
        return;
    }

    if (battlePetMgr->GetCurrentSummonId() == petEntry)
        battlePetMgr->UnSummonCurrentBattlePet(false);
    else
    {
        if (uint32 summonSpell = BattlePetGetSummonSpell(battlePet->GetSpecies()))
        {
            battlePetMgr->SetCurrentSummonId(petEntry);
            player->CastSpell(player, summonSpell, true);
        }
    }
}

void WorldSession::HandleBattlePetWildRequest(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_BATTLE_PET_WILD_REQUEST");
    ObjectGuid guid;
    bool hasOrientation;
    bool hasResult;

    PetBattleRequest petBattleRequest;

    for (uint8 i = 0; i < 2; i++) // team positions
    {
        recvData >> petBattleRequest.Positions[i].x;
        recvData >> petBattleRequest.Positions[i].z;
        recvData >> petBattleRequest.Positions[i].y;
    }

    // origin position
    recvData >> petBattleRequest.Origin.z;
    recvData >> petBattleRequest.Origin.y;
    recvData >> petBattleRequest.Origin.x; 

    recvData.ReadGuidMask(guid, 0);
    hasOrientation = recvData.ReadBit();
    recvData.ReadGuidMask(guid, 6, 3, 5, 2, 7, 1, 4);
    hasResult = recvData.ReadBit();

    recvData.ReadGuidBytes(guid, 3, 6, 5, 2, 7, 1, 0, 4);

    if (hasOrientation)
        recvData >> petBattleRequest.Orientation;

    if (hasResult)
        recvData >> petBattleRequest.LocationResult;

    Creature* wildBattlePet = ObjectAccessor::GetCreatureOrPetOrVehicle(*GetPlayer(), guid);
    petBattleRequest.Type = PetBattleRequest::PET_BATTLE_TYPE_PVE;
    petBattleRequest.Challenger = GetPlayer();
    petBattleRequest.Enemy = wildBattlePet;

    
    WorldPacket data(SMSG_BATTLE_PET_LOCATION_FINALIZE, 100);
    data << petBattleRequest.Origin.x;
    data << petBattleRequest.Origin.y;

    for (uint8 i = 0; i < 2; i++) // team positions
    {
        data << petBattleRequest.Positions[i].y;
        data << petBattleRequest.Positions[i].x;
        data << petBattleRequest.Positions[i].z;
    }

    data << petBattleRequest.Origin.z;

    data.WriteBit(petBattleRequest.Orientation);
    data.WriteBit(petBattleRequest.LocationResult);

    if (petBattleRequest.LocationResult)
        data << uint32(petBattleRequest.LocationResult);
    if (petBattleRequest.Orientation)
        data << float(petBattleRequest.Orientation);
    _player->SendDirectMessage(&data);
    

    //SMSG_BATTLE_PET_UPDATE_INIT starts the pet battle itself.
    WorldPacket data2(SMSG_BATTLE_PET_UPDATE_INIT, 1000);

    bool hasWatingForFrontPetsMaxSecs = true;
    bool hasPvPMaxRoundTime = true;
    bool hasForfietPenalty = true;
    bool hasCreatureId = false;
    bool hasDisplayId = false;

    ObjectGuid wildBattlePetGuid = petBattleRequest.EnemyGUID;

    // enviroment update
    for (uint8 i = 0; i < 3; i++)
    {
        data2.WriteBits(0, 21);                      // AuraCount
        data2.WriteBits(0, 21);                      // StateCount
    }

    for (uint8 i=0; i < 2; i++)
    {
        bool hasRoundTimeSec = false;
        bool hasFrontPet = false;
        bool trapStatus = true;

        ObjectGuid characterGuid = 0;

        data2.WriteBit(trapStatus);
        data2.WriteBits(1, 2); // BattlePet Count
        data2.WriteBit(characterGuid[2]);

        for (uint8 j=0; j < 1; j++)
        {
            ObjectGuid petEntry = 0;

            data2.WriteBits(0, 21);      // aura count
            data2.WriteBit(petEntry[3]);
            data2.WriteBits(0, 21);

            uint8 abilityCount = 0;
            for (uint8 k = 0; k < 3; k++)
                    abilityCount++;

            data2.WriteBit(petEntry[0]);
            data2.WriteBit(0); // flags
            data2.WriteGuidMask(petEntry, 5, 1);

            data2.WriteBits(abilityCount, 20);
            data2.WriteBit(0);
            data2.WriteBits(0, 7); // name size
            data2.WriteGuidMask(petEntry, 2, 4);

            for (uint8 l = 0; l < 3; l++)
                data2.WriteBit(0);

            data2.WriteGuidMask(petEntry, 6, 7);
        }

        data2.WriteBit(hasFrontPet);
        data2.WriteBit(hasRoundTimeSec);
        data2.WriteGuidMask(characterGuid, 5, 3, 4, 6, 7, 0, 1);
    }

    data2.WriteBit(hasForfietPenalty);

    data2.WriteBit(0); // CanAwardXP?
    data2.WriteBit(0); // IsPvP

    data2.WriteBit(hasDisplayId);
    data2.WriteBit(hasCreatureId);
    data2.WriteBit(hasWatingForFrontPetsMaxSecs);
    data2.WriteBit(wildBattlePetGuid);

    data2.WriteGuidMask(wildBattlePetGuid, 2, 4, 5, 1, 3, 6, 7, 0);

    data2.WriteBit(!hasPvPMaxRoundTime);
    data2.WriteBit(0); // battlepetstate

    data2.FlushBits();

    for (uint8 i = 0; i < 2; i++)
    {
        ObjectGuid characterGuid = 0;

        bool hasRoundTimeSec = false;
        bool hasFrontPet = true;
        bool trapStatus = true;

        uint8 battlePetTeamIndex = 0;
        for (uint8 j = 0; j < 1; j++)
        {
            data2 << battlePetTeamIndex;
            battlePetTeamIndex++;


            BattlePet* battlePet = GetPlayer()->GetBattlePetMgr()->GetBattlePet(GetPlayer()->GetBattlePetMgr()->GetLoadoutSlot(j));
            ObjectGuid petEntry = battlePet->GetId();

            for (uint8 k = 0; k < 3; k++)
            {
                data2 << uint32(0); //AbilityId
                data2 << uint8(k);
                data2 << uint8(0); // GlobalIndex
                data2 << uint16(0); // Cooldown
                data2 << uint16(0); // Lockdown
            }

            data2 << uint32(0);
            data2.WriteByteSeq(petEntry[4]);
            data2 << uint16(battlePet->GetLevel()); //Level
            data2.WriteByteSeq(petEntry[7]);
            data2 << uint16(battlePet->GetQuality()); // quality
            data2.WriteByteSeq(petEntry[6]);
            data2 << uint32(battlePet->GetPower()); // power
            data2.WriteByteSeq(petEntry[0]);
            data2 << uint32(battlePet->GetMaxHealth()); //maxhp
            data2.WriteByteSeq(petEntry[5]);
            data2.WriteByteSeq(petEntry[2]);
            data2 << uint32(battlePet->GetSpeed()); //speed
            data2 << uint32(battlePet->GetCurrentHealth()); // curhp
            data2.WriteByteSeq(petEntry[3]);
            data2 << uint32(0);
            data2.WriteByteSeq(petEntry[1]);
            data2 << uint32(0);
            data2 << uint16(battlePet->GetXp()); // xp
            data2 << battlePet->GetFlags(); // flags
            data2.WriteString(battlePet->GetNickname()); //name
            data2 << uint32(battlePet->GetSpecies()); //species
        }

        if (trapStatus)
            data2 << uint32(0);

        data2 << uint32(0);
        data2.WriteGuidBytes(characterGuid, 5, 7, 6, 1, 4, 0);

        if (hasFrontPet)
            data2 << uint8(0);

        data2 << uint8(6);

        if (hasRoundTimeSec)
            data2 << uint16(0);

        data2.WriteGuidBytes(characterGuid, 3, 2);
    }

    if (hasForfietPenalty)
        data2 << uint8(10);
    
    data2 << uint8(1);

    data2.WriteGuidBytes(wildBattlePetGuid, 5, 4, 3, 2, 7, 0, 1, 6);

    if (hasDisplayId)
        data2 << uint32(0);

    if (hasPvPMaxRoundTime)
        data2 << uint16(30);

    data2 << uint32(0);

    if (hasWatingForFrontPetsMaxSecs)
        data2 << uint16(30);

    if (hasCreatureId)
        data2 << uint32(0);

    _player->SendDirectMessage(&data2);
}

