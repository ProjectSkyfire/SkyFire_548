/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ArenaTeam.h"
#include "ArenaTeamMgr.h"
#include "Common.h"
#include "GossipDef.h"
#include "Guild.h"
#include "GuildMgr.h"
#include "Language.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "SocialMgr.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"

#define CHARTER_DISPLAY_ID 16161

constexpr uint32 GUILD_CHARTER = 5863;
constexpr uint64 GUILD_CHARTER_COST = 1000;
constexpr uint8 GUILD_CHARTER_TYPE = 4;

void WorldSession::HandlePetitionBuyOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "Received opcode CMSG_PETITION_BUY");

    ObjectGuid npcGuid;
    uint8 nameLen;
    std::string name;

    recvData.ReadGuidMask(npcGuid, 5, 2, 3);
    nameLen = recvData.ReadBits(7);
    recvData.ReadGuidMask(npcGuid, 4, 1, 7, 0, 6);

    name = recvData.ReadString(nameLen);
    recvData.ReadGuidBytes(npcGuid, 1, 7, 4, 6, 0, 5, 2, 3);

    SF_LOG_DEBUG("network", "Petitioner with GUID %u tried sell petition: name %s", GUID_LOPART(npcGuid), name.c_str());

    // prevent cheating
    Creature* creature = GetPlayer()->GetNPCIfCanInteractWith(npcGuid, UNIT_NPC_FLAG_PETITIONER);
    if (!creature)
    {
        SF_LOG_DEBUG("network", "WORLD: HandlePetitionBuyOpcode - Unit (GUID: %u) not found or you can't interact with him.", GUID_LOPART(npcGuid));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    // if tabard designer, then trying to buy a guild charter.
    // do not let if already in guild.
    if (_player->GetGuildId())
        return;

    if (sGuildMgr->GetGuildByName(name))
    {
        Guild::SendCommandResult(this, GUILD_COMMAND_CREATE, ERR_GUILD_NAME_EXISTS_S, name);
        return;
    }

    if (sObjectMgr->IsReservedName(name) || !ObjectMgr::IsValidCharterName(name))
    {
        Guild::SendCommandResult(this, GUILD_COMMAND_CREATE, ERR_GUILD_NAME_INVALID, name);
        return;
    }

    ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(GUILD_CHARTER);
    if (!pProto)
    {
        _player->SendBuyFailed(BuyResult::BUY_ERR_CANT_FIND_ITEM, 0, GUILD_CHARTER);
        return;
    }

    if (!_player->HasEnoughMoney(uint64(GUILD_CHARTER_COST)))
    {                                                       //player hasn't got enough money
        _player->SendBuyFailed(BuyResult::BUY_ERR_NOT_ENOUGHT_MONEY, creature->GetObjectGUID(), GUILD_CHARTER);
        return;
    }

    ItemPosCountVec dest;
    InventoryResult msg = _player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, GUILD_CHARTER, pProto->BuyCount);
    if (msg != EQUIP_ERR_OK)
    {
        _player->SendEquipError(msg, NULL, NULL, GUILD_CHARTER);
        return;
    }

    _player->ModifyMoney(-(int32)GUILD_CHARTER_COST);
    Item* charter = _player->StoreNewItem(dest, GUILD_CHARTER, true);
    if (!charter)
        return;

    charter->SetUInt32Value(ITEM_FIELD_ENCHANTMENT, charter->GetGUIDLow());
    // ITEM_FIELD_ENCHANTMENT is guild/arenateam id
    // ITEM_FIELD_ENCHANTMENT+1 is current signatures count (showed on item)
    charter->SetState(ITEM_CHANGED, _player);
    _player->SendNewItem(charter, 1, true, false);

    // a petition is invalid, if both the owner and the type matches
    // we checked above, if this player is in an arenateam, so this must be
    // datacorruption
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION_BY_OWNER);
    stmt->setUInt32(0, _player->GetGUIDLow());
    stmt->setUInt8(1, GUILD_CHARTER_TYPE);
    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    std::ostringstream ssInvalidPetitionGUIDs;

    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            ssInvalidPetitionGUIDs << '\'' << fields[0].GetUInt32() << "', ";
        } while (result->NextRow());
    }

    // delete petitions with the same guid as this one
    ssInvalidPetitionGUIDs << '\'' << charter->GetGUIDLow() << '\'';

    SF_LOG_DEBUG("network", "Invalid petition GUIDs: %s", ssInvalidPetitionGUIDs.str().c_str());
    CharacterDatabase.EscapeString(name);
    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    trans->PAppend("DELETE FROM petition WHERE petitionguid IN (%s)", ssInvalidPetitionGUIDs.str().c_str());
    trans->PAppend("DELETE FROM petition_sign WHERE petitionguid IN (%s)", ssInvalidPetitionGUIDs.str().c_str());

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_PETITION);
    stmt->setUInt32(0, _player->GetGUIDLow());
    stmt->setUInt32(1, charter->GetGUIDLow());
    stmt->setString(2, name);
    stmt->setUInt8(3, GUILD_CHARTER_TYPE);
    trans->Append(stmt);

    CharacterDatabase.CommitTransaction(trans);
}

void WorldSession::HandlePetitionShowSignOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "Received opcode CMSG_PETITION_SHOW_SIGNATURES");

    uint8 playerCount = 0;
    ObjectGuid petitionGuid;

    recvData.ReadGuidMask(petitionGuid, 3, 7, 2, 4, 5, 6, 0, 1);
    recvData.ReadGuidBytes(petitionGuid, 2, 4, 5, 7, 1, 0, 3, 6);

    // solve (possible) some strange compile problems with explicit use GUID_LOPART(petitionguid) at some GCC versions (wrong code optimization in compiler?)
    uint32 petitionGuidLow = GUID_LOPART(petitionGuid);

    // if guild petition and has guild => error, return;
    if (_player->GetGuildId())
        return;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION_SIGNATURE);

    stmt->setUInt32(0, petitionGuidLow);

    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    // result == NULL also correct in case no sign yet
    if (result)
        playerCount = uint8(result->GetRowCount());

    SF_LOG_DEBUG("network", "CMSG_PETITION_SHOW_SIGNATURES petition entry: '%u'", petitionGuidLow);

    ObjectGuid playerGuid = _player->GetGUID();
    ObjectGuid* playerGuids = new ObjectGuid[playerCount];

    for (uint8 i = 0; i < playerCount; ++i)
    {
        Field* fields2 = result->Fetch();
        uint32 lowGuid = fields2[0].GetUInt32();
        playerGuids[i] = MAKE_NEW_GUID(lowGuid, 0, HIGHGUID_PLAYER);
        result->NextRow();
    }

    WorldPacket data(SMSG_PETITION_SHOW_SIGNATURES, (8 + 8 + 3 + (playerCount * (8 + 4)) + 4));
    data.WriteGuidMask(playerGuid, 1);
    data.WriteGuidMask(petitionGuid, 3);
    data.WriteGuidMask(playerGuid, 3);
    data.WriteGuidMask(petitionGuid, 4, 0);
    data.WriteGuidMask(playerGuid, 7, 5);
    data.WriteGuidMask(petitionGuid, 1, 5, 7);
    data.WriteGuidMask(playerGuid, 0, 6);
    data.WriteGuidMask(petitionGuid, 6);
    data.WriteGuidMask(playerGuid, 2, 4);

    data.WriteBits(playerCount, 21);

    for (int i = 0; i < playerCount; i++)
    {
        data.WriteGuidMask(playerGuids[i], 2, 0, 4, 7, 5, 1, 6, 3);
    }
    data.WriteGuidMask(petitionGuid, 2);
    data.FlushBits();

    for (int i = 0; i < playerCount; i++)
    {
        data.WriteGuidBytes(playerGuids[i], 6, 0, 1, 3, 2, 5, 7, 4);
        data << uint32(1); // Choice ??? Blizzard also stores declined players ???
    }

    data.WriteGuidBytes(petitionGuid, 6, 5, 4);
    data.WriteGuidBytes(playerGuid, 4);
    data.WriteGuidBytes(petitionGuid, 1);
    data << uint32(petitionGuidLow); // guildID
    data.WriteGuidBytes(petitionGuid, 2, 3, 7);
    data.WriteGuidBytes(playerGuid, 5, 6, 3, 7, 1, 0);
    data.WriteGuidBytes(petitionGuid, 0);
    data.WriteGuidBytes(playerGuid, 2);
    delete[] playerGuids;
    SendPacket(&data);
}

void WorldSession::HandlePetitionQueryOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "Received opcode CMSG_PETITION_QUERY");   // ok

    uint32 guildguid;
    ObjectGuid petitionGuid;

    recvData >> guildguid;                                 // in Skyfire always same as GUID_LOPART(petitionguid)

    recvData.ReadGuidMask(petitionGuid, 2, 3, 1, 0, 4, 7, 6, 5);
    recvData.ReadGuidBytes(petitionGuid, 0, 4, 7, 5, 1, 6, 3, 2);

    SF_LOG_DEBUG("network", "CMSG_PETITION_QUERY Petition GUID %u Guild GUID %u", GUID_LOPART(petitionGuid), guildguid);

    SendPetitionQueryOpcode(petitionGuid);
}

void WorldSession::SendPetitionQueryOpcode(uint64 petitionGuid)
{
    ObjectGuid ownerGuid = 0;
    uint32 type;
    std::string name = "NO_NAME_FOR_GUID";

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION);

    stmt->setUInt32(0, GUID_LOPART(petitionGuid));

    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (result)
    {
        Field* fields = result->Fetch();
        ownerGuid = MAKE_NEW_GUID(fields[0].GetUInt32(), 0, HIGHGUID_PLAYER);
        name = fields[1].GetString();
        type = fields[2].GetUInt8();
    }
    else
    {
        SF_LOG_DEBUG("network", "CMSG_PETITION_QUERY failed for petition (GUID: %u)", GUID_LOPART(petitionGuid));
        return;
    }

    WorldPacket data(SMSG_PETITION_QUERY_RESPONSE, (8 + 2 + 1 + name.size() + (13 * 4) + 2));
    data << uint32(GUID_LOPART(petitionGuid));              // guild/team guid (in Skyfire always same as GUID_LOPART(petition guid)
    data.WriteBit(1); // hasData;

    for (int i = 0; i < 10; i++)
        data.WriteBits(0, 6); // Unk strings;

    data.WriteGuidMask(ownerGuid, 2, 4);
    data.WriteBits(0, 12);
    data.WriteGuidMask(ownerGuid, 0, 7, 3, 6, 5);
    data.WriteBits(name.size(), 7); //NameLen
    data.WriteGuidMask(ownerGuid, 1);

    data.FlushBits();

    data.WriteGuidBytes(ownerGuid, 5);
    data << uint32(0);
    data.WriteString(name);
    data << uint32(0);
    data.WriteGuidBytes(ownerGuid, 4);
    data << uint32(type); // Type - 4 guild
    data.WriteGuidBytes(ownerGuid, 6);
    data << uint32(0);
    data << uint32(sWorld->getIntConfig(WorldIntConfigs::CONFIG_MIN_PETITION_SIGNS)); // Required sign count
    data.WriteGuidBytes(ownerGuid, 1, 7, 0);
    data << uint32(0);
    data << uint32(0);
    data.WriteGuidBytes(ownerGuid, 2);
    data << uint32(time(NULL) + YEAR); // Deadline
    data << uint16(0);
    data << uint32(0);
    data.WriteGuidBytes(ownerGuid, 3);
    data << uint32(0);
    data << uint32(0);
    data << uint32(0);
    data << uint32(0);

    SendPacket(&data);
}

void WorldSession::HandlePetitionRenameOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "Received opcode CMSG_PETITION_RENAME");

    ObjectGuid petitionGuid;
    uint32 type;
    uint8 nameLen;
    std::string newName;

    nameLen = recvData.ReadBits(7);
    recvData.ReadGuidMask(petitionGuid, 7, 4, 6, 2, 0, 5, 3, 1);
    recvData.ReadGuidBytes(petitionGuid, 4, 1, 7);
    newName = recvData.ReadString(nameLen);
    recvData.ReadGuidBytes(petitionGuid, 0, 3, 2, 6, 5);

    Item* item = _player->GetItemByGuid(petitionGuid);
    if (!item)
        return;

    if (sGuildMgr->GetGuildByName(newName))
    {
        Guild::SendCommandResult(this, GUILD_COMMAND_CREATE, ERR_GUILD_NAME_EXISTS_S, newName);
        return;
    }
    if (sObjectMgr->IsReservedName(newName) || !ObjectMgr::IsValidCharterName(newName))
    {
        Guild::SendCommandResult(this, GUILD_COMMAND_CREATE, ERR_GUILD_NAME_INVALID, newName);
        return;
    }

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_PETITION_NAME);

    stmt->setString(0, newName);
    stmt->setUInt32(1, GUID_LOPART(petitionGuid));

    CharacterDatabase.Execute(stmt);

    SF_LOG_DEBUG("network", "Petition (GUID: %u) renamed to '%s'", GUID_LOPART(petitionGuid), newName.c_str());

    WorldPacket data(SMSG_PETITION_RENAME_RESULT, (9 + 1 + newName.size()));
    data.WriteBits(newName.length(), 7);
    data.WriteGuidMask(petitionGuid, 0, 3, 4, 2, 6, 5, 7, 1);
    data.FlushBits();
    data.WriteGuidBytes(petitionGuid, 4, 3, 6, 0, 5, 2, 1, 7);
    data.WriteString(newName);
    SendPacket(&data);
}

void WorldSession::HandlePetitionSignOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "Received opcode CMSG_PETITION_SIGN");    // ok

    Field* fields;
    ObjectGuid petitionGuid;

    recvData.read_skip<uint8>();
    recvData.ReadGuidMask(petitionGuid, 4, 2, 0, 1, 5, 3, 6, 7);
    recvData.ReadGuidBytes(petitionGuid, 6, 1, 7, 2, 5, 3, 0, 4);

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION_SIGNATURES);

    stmt->setUInt32(0, GUID_LOPART(petitionGuid));
    stmt->setUInt32(1, GUID_LOPART(petitionGuid));

    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (!result)
    {
        SF_LOG_ERROR("network", "Petition %u is not found for player %u %s", GUID_LOPART(petitionGuid), GetPlayer()->GetGUIDLow(), GetPlayer()->GetName().c_str());
        return;
    }

    fields = result->Fetch();
    uint64 ownerGuid = MAKE_NEW_GUID(fields[0].GetUInt32(), 0, HIGHGUID_PLAYER);
    uint64 signs = fields[1].GetUInt64();
    uint8 type = fields[2].GetUInt8();

    uint32 playerGuid = _player->GetGUIDLow();
    if (GUID_LOPART(ownerGuid) == playerGuid)
        return;

    // not let enemies sign guild charter
    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ALLOW_TWO_SIDE_INTERACTION_GUILD) && GetPlayer()->GetTeam() != sObjectMgr->GetPlayerTeamByGUID(ownerGuid))
    {
        Guild::SendCommandResult(this, GUILD_COMMAND_CREATE, ERR_GUILD_NOT_ALLIED);
        return;
    }

    if (_player->GetGuildId())
    {
        Guild::SendCommandResult(this, GUILD_COMMAND_INVITE, ERR_ALREADY_IN_GUILD_S, _player->GetName());
        return;
    }
    if (_player->GetGuildIdInvited())
    {
        Guild::SendCommandResult(this, GUILD_COMMAND_INVITE, ERR_ALREADY_INVITED_TO_GUILD_S, _player->GetName());
        return;
    }

    if (++signs > type)                                        // client signs maximum
        return;

    // Client doesn't allow to sign petition two times by one character, but not check sign by another character from same account
    // not allow sign another player from already sign player account
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION_SIG_BY_ACCOUNT);

    stmt->setUInt32(0, GetAccountId());
    stmt->setUInt32(1, GUID_LOPART(petitionGuid));

    result = CharacterDatabase.Query(stmt);

    if (result)
    {
        // close at signer side
        SendPetitionSignResults(petitionGuid, _player->GetGUID(), PETITION_SIGN_ALREADY_SIGNED);
        return;
    }

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_PETITION_SIGNATURE);

    stmt->setUInt32(0, GUID_LOPART(ownerGuid));
    stmt->setUInt32(1, GUID_LOPART(petitionGuid));
    stmt->setUInt32(2, playerGuid);
    stmt->setUInt32(3, GetAccountId());

    CharacterDatabase.Execute(stmt);

    SF_LOG_DEBUG("network", "PETITION SIGN: GUID %u by player: %s (GUID: %u Account: %u)", GUID_LOPART(petitionGuid), _player->GetName().c_str(), playerGuid, GetAccountId());

    // close at signer side
    SendPetitionSignResults(petitionGuid, _player->GetGUID(), PETITION_SIGN_OK);

    // update signs count on charter, required testing...
    //Item* item = _player->GetItemByGuid(petitionguid));
    //if (item)
    //    item->SetUInt32Value(ITEM_FIELD_ENCHANTMENT+1, signs);

    // update for owner if online
    if (Player* owner = ObjectAccessor::FindPlayer(ownerGuid))
        owner->GetSession()->SendPetitionSignResults(petitionGuid, _player->GetGUID(), PETITION_SIGN_OK);
}

void WorldSession::HandlePetitionDeclineOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "Received opcode MSG_PETITION_DECLINE");  // ok

    ObjectGuid petitionGuid;
    uint64 ownerGuid;

    recvData.ReadGuidMask(petitionGuid, 5, 6, 4, 3, 1, 7, 0, 2);
    recvData.ReadGuidBytes(petitionGuid, 6, 2, 1, 5, 0, 7, 4, 3);

    SF_LOG_DEBUG("network", "Petition %u declined by %u", GUID_LOPART(petitionGuid), _player->GetGUIDLow());

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION_OWNER_BY_GUID);

    stmt->setUInt32(0, GUID_LOPART(petitionGuid));

    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (!result)
        return;

    Field* fields = result->Fetch();
    ownerGuid = MAKE_NEW_GUID(fields[0].GetUInt32(), 0, HIGHGUID_PLAYER);

    Player* owner = ObjectAccessor::FindPlayer(ownerGuid);
    if (owner)                                               // petition owner online
        owner->GetSession()->SendPetitionSignResults(petitionGuid, _player->GetGUID(), PETITION_SIGN_OK);
}

void WorldSession::HandleOfferPetitionOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "Received opcode CMSG_OFFER_PETITION");   // ok

    uint8 playerCount = 0;
    ObjectGuid petitionGuid, playerGuid;
    uint32 type;
    Player* player;

    recvData.read_skip<uint32>();
    recvData.ReadGuidMask(playerGuid, 4, 1);
    recvData.ReadGuidMask(petitionGuid, 2);
    recvData.ReadGuidMask(playerGuid, 6);
    recvData.ReadGuidMask(petitionGuid, 1);
    recvData.ReadGuidMask(playerGuid, 2);
    recvData.ReadGuidMask(petitionGuid, 4);
    recvData.ReadGuidMask(playerGuid, 3, 7);
    recvData.ReadGuidMask(petitionGuid, 0, 6);
    recvData.ReadGuidMask(playerGuid, 5, 0);
    recvData.ReadGuidMask(petitionGuid, 3, 5, 7);

    recvData.ReadGuidBytes(playerGuid, 7);
    recvData.ReadGuidBytes(petitionGuid, 1, 4, 2);
    recvData.ReadGuidBytes(playerGuid, 6);
    recvData.ReadGuidBytes(petitionGuid, 3, 0, 5);
    recvData.ReadGuidBytes(playerGuid, 0, 2, 5, 3, 4);
    recvData.ReadGuidBytes(petitionGuid, 7);
    recvData.ReadGuidBytes(playerGuid, 1);
    recvData.ReadGuidBytes(petitionGuid, 6);

    player = ObjectAccessor::FindPlayer(playerGuid);
    if (!player)
        return;

    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ALLOW_TWO_SIDE_INTERACTION_GUILD) && GetPlayer()->GetTeam() != player->GetTeam())
    {
        Guild::SendCommandResult(this, GUILD_COMMAND_CREATE, ERR_GUILD_NOT_ALLIED);
        return;
    }

    if (player->GetGuildId())
    {
        Guild::SendCommandResult(this, GUILD_COMMAND_INVITE, ERR_ALREADY_IN_GUILD_S, _player->GetName());
        return;
    }

    if (player->GetGuildIdInvited())
    {
        Guild::SendCommandResult(this, GUILD_COMMAND_INVITE, ERR_ALREADY_INVITED_TO_GUILD_S, _player->GetName());
        return;
    }

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION_SIGNATURE);

    stmt->setUInt32(0, GUID_LOPART(petitionGuid));

    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    // result == NULL also correct charter without signs
    if (result)
        playerCount = uint8(result->GetRowCount());

    ObjectGuid* playerGuids = new ObjectGuid[playerCount];

    for (uint8 i = 0; i < playerCount; ++i)
    {
        Field* fields2 = result->Fetch();
        uint32 lowGuid = fields2[0].GetUInt32();
        playerGuids[i] = MAKE_NEW_GUID(lowGuid, 0, HIGHGUID_PLAYER);
        result->NextRow();
    }

    WorldPacket data(SMSG_PETITION_SHOW_SIGNATURES, (8 + 8 + 3 + (playerCount * (8 + 4)) + 4));
    data.WriteGuidMask(playerGuid, 1);
    data.WriteGuidMask(petitionGuid, 3);
    data.WriteGuidMask(playerGuid, 3);
    data.WriteGuidMask(petitionGuid, 4, 0);
    data.WriteGuidMask(playerGuid, 7, 5);
    data.WriteGuidMask(petitionGuid, 1, 5, 7);
    data.WriteGuidMask(playerGuid, 0, 6);
    data.WriteGuidMask(petitionGuid, 6);
    data.WriteGuidMask(playerGuid, 2, 4);

    data.WriteBits(playerCount, 21);

    for (int i = 0; i < playerCount; i++)
    {
        data.WriteGuidMask(playerGuids[i], 2, 0, 4, 7, 5, 1, 6, 3);
    }
    data.WriteGuidMask(petitionGuid, 2);
    data.FlushBits();

    for (int i = 0; i < playerCount; i++)
    {
        data.WriteGuidBytes(playerGuids[i], 6, 0, 1, 3, 2, 5, 7, 4);
        data << uint32(1); // Choice ??? Blizzard also stores declined players ???
    }

    data.WriteGuidBytes(petitionGuid, 6, 5, 4);
    data.WriteGuidBytes(playerGuid, 4);
    data.WriteGuidBytes(petitionGuid, 1);
    data << uint32(GUID_LOPART(petitionGuid)); // guildID
    data.WriteGuidBytes(petitionGuid, 2, 3, 7);
    data.WriteGuidBytes(playerGuid, 5, 6, 3, 7, 1, 0);
    data.WriteGuidBytes(petitionGuid, 0);
    data.WriteGuidBytes(playerGuid, 2);
    delete[] playerGuids;
    SendPacket(&data);
    player->GetSession()->SendPacket(&data);
}

void WorldSession::HandleTurnInPetitionOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "Received opcode CMSG_TURN_IN_PETITION");

    // Get petition guid from packet
    WorldPacket data;
    ObjectGuid petitionGuid;

    recvData.ReadGuidMask(petitionGuid, 1, 2, 3, 0, 5, 7, 4, 6);
    recvData.ReadGuidBytes(petitionGuid, 2, 1, 4, 6, 0, 7, 5, 3);

    // Check if player really has the required petition charter
    Item* item = _player->GetItemByGuid(petitionGuid);
    if (!item)
        return;

    SF_LOG_DEBUG("network", "Petition %u turned in by %u", GUID_LOPART(petitionGuid), _player->GetGUIDLow());

    // Get petition data from db
    uint32 ownerguidlo;
    uint32 type;
    std::string name;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION);
    stmt->setUInt32(0, GUID_LOPART(petitionGuid));
    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (result)
    {
        Field* fields = result->Fetch();
        ownerguidlo = fields[0].GetUInt32();
        name = fields[1].GetString();
        type = fields[2].GetUInt8();
    }
    else
    {
        SF_LOG_ERROR("network", "Player %s (guid: %u) tried to turn in petition (guid: %u) that is not present in the database", _player->GetName().c_str(), _player->GetGUIDLow(), GUID_LOPART(petitionGuid));
        return;
    }

    // Only the petition owner can turn in the petition
    if (_player->GetGUIDLow() != ownerguidlo)
        return;

    // Petition type (guild/arena) specific checks
    // Check if player is already in a guild
    if (_player->GetGuildId())
    {
        data.Initialize(SMSG_TURN_IN_PETITION_RESULTS, 1);
        data.WriteBits(PETITION_TURN_ALREADY_IN_GUILD, 4);
        data.FlushBits();
        _player->GetSession()->SendPacket(&data);
        return;
    }

    // Check if guild name is already taken
    if (sGuildMgr->GetGuildByName(name))
    {
        Guild::SendCommandResult(this, GUILD_COMMAND_CREATE, ERR_GUILD_NAME_EXISTS_S, name);
        return;
    }

    // Get petition signatures from db
    uint8 signatures;

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION_SIGNATURE);
    stmt->setUInt32(0, GUID_LOPART(petitionGuid));
    result = CharacterDatabase.Query(stmt);

    if (result)
        signatures = uint8(result->GetRowCount());
    else
        signatures = 0;

    uint32 requiredSignatures = sWorld->getIntConfig(WorldIntConfigs::CONFIG_MIN_PETITION_SIGNS);

    // Notify player if signatures are missing
    if (signatures < requiredSignatures)
    {
        data.Initialize(SMSG_TURN_IN_PETITION_RESULTS, 1);
        data.WriteBits(PETITION_TURN_NEED_MORE_SIGNATURES, 4);
        data.FlushBits();
        SendPacket(&data);
        return;
    }

    // Proceed with guild/arena team creation

    // Delete charter item
    _player->DestroyItem(item->GetBagSlot(), item->GetSlot(), true);

    // Create guild
    Guild* guild = new Guild;

    if (!guild->Create(_player, name))
    {
        delete guild;
        return;
    }

    // Register guild and add guild master
    sGuildMgr->AddGuild(guild);

    Guild::SendCommandResult(this, GUILD_COMMAND_CREATE, ERR_GUILD_COMMAND_SUCCESS, name);

    // Add members from signatures
    for (uint8 i = 0; i < signatures; ++i)
    {
        Field* fields = result->Fetch();
        guild->AddMember(MAKE_NEW_GUID(fields[0].GetUInt32(), 0, HIGHGUID_PLAYER));
        result->NextRow();
    }

    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PETITION_BY_GUID);
    stmt->setUInt32(0, GUID_LOPART(petitionGuid));
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PETITION_SIGNATURE_BY_GUID);
    stmt->setUInt32(0, GUID_LOPART(petitionGuid));
    trans->Append(stmt);

    CharacterDatabase.CommitTransaction(trans);

    // created
    SF_LOG_DEBUG("network", "TURN IN PETITION GUID %u", GUID_LOPART(petitionGuid));

    data.Initialize(SMSG_TURN_IN_PETITION_RESULTS, 1);
    data.WriteBits(PETITION_TURN_OK, 4);
    data.FlushBits();
    SendPacket(&data);
}

void WorldSession::HandlePetitionShowListOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "Received CMSG_PETITION_SHOWLIST");

    ObjectGuid guid;
    
    recvData.ReadGuidMask(guid, 1, 7, 2, 5, 4, 0, 3, 6);
    recvData.ReadGuidBytes(guid, 6, 3, 2, 4, 1, 7, 5, 0);

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_PETITIONER);
    if (!unit)
    {
        SF_LOG_DEBUG("network", "WORLD: HandlePetitionerShowListOpcode - Unit (GUID: %u) not found or you can't interact with him.", uint32(GUID_LOPART(guid)));
        return;
    }

    SendPetitionShowList(guid);
}

void WorldSession::SendPetitionShowList(uint64 guid)
{
    Creature* creature = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_PETITIONER);
    if (!creature)
    {
        SF_LOG_DEBUG("network", "WORLD: HandlePetitionShowListOpcode - Unit (GUID: %u) not found or you can't interact with him.", uint32(GUID_LOPART(guid)));
        return;
    }

    ObjectGuid npcGuid = guid;

    WorldPacket data(SMSG_PETITION_SHOWLIST, 8 + 4);
    data.WriteGuidMask(npcGuid, 3, 5, 7, 6, 1, 0, 2, 4);
    data.WriteGuidMask(npcGuid, 6, 0, 1);

    data << uint32(GUILD_CHARTER_COST); // Petition Cost
    data.WriteGuidBytes(npcGuid, 4, 3, 5, 2, 7);

    SendPacket(&data);
    SF_LOG_DEBUG("network", "Sent SMSG_PETITION_SHOWLIST");
}

void WorldSession::SendPetitionSignResults(ObjectGuid petitionGuid, ObjectGuid playerGuid, uint8 result)
{
    WorldPacket data(SMSG_PETITION_SIGN_RESULTS, 8 + 8 + 1);
    data.WriteGuidMask(playerGuid, 2, 0);
    data.WriteGuidMask(petitionGuid, 0);
    data.WriteGuidMask(playerGuid, 1);
    data.WriteGuidMask(petitionGuid, 5, 2, 4);
    data.WriteGuidMask(playerGuid, 6);
    data.WriteGuidMask(petitionGuid, 1, 6);
    data.WriteGuidMask(playerGuid, 4);
    data.WriteGuidMask(petitionGuid, 3);
    data.WriteGuidMask(playerGuid, 5, 3);
    data.WriteGuidMask(petitionGuid, 7);
    data.WriteGuidMask(playerGuid, 7);

    data.WriteBits(result, 4);
    data.FlushBits();

    data.WriteGuidBytes(petitionGuid, 0, 5);
    data.WriteGuidBytes(playerGuid, 3, 0);
    data.WriteGuidBytes(petitionGuid, 3);
    data.WriteGuidBytes(playerGuid, 2, 6, 4, 1);
    data.WriteGuidBytes(petitionGuid, 6, 7);
    data.WriteGuidBytes(playerGuid, 7);
    data.WriteGuidBytes(petitionGuid, 2, 1);
    data.WriteGuidBytes(playerGuid, 5);
    data.WriteGuidBytes(petitionGuid, 4);
    SendPacket(&data);
}