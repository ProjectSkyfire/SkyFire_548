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
    std::string declinedNames[BATTLE_PET_MAX_DECLINED_NAMES];

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
        for (uint8 i = 0; i < BATTLE_PET_MAX_DECLINED_NAMES; i++)
            declinedNames[i] = recvData.ReadString(declinedNameLen[i]);

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
