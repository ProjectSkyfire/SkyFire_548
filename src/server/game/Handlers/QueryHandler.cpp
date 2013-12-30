/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
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
#include "Language.h"
#include "DatabaseEnv.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "UpdateMask.h"
#include "NPCHandler.h"
#include "Pet.h"
#include "MapManager.h"
#include "Config.h"

/*enum DBQueryType
{
    BattlePetAbility           = 0xCBA43BD7,
    BattlePetAbilityEffect     = 0xDD8B690E,
    BattlePetAbilityState      = 0x3C556E43,
    BattlePetAbilityTurn       = 0xECD8ECDC,
    BattlePetBreedQuality      = 0x1B5A4EA6,
    BattlePetBreedState        = 0x6AFB3206,
    DB_QUERY_BattlePetEffectProperties  = 0x63B4C4BA,
    BattlePetNPCTeamMember     = 0xF2059DFA,
    BattlePetSpecies           = 0x6C93F9B1,
    BattlePetSpeciesState      = 0x15D87DD0,
    BattlePetSpeciesXAbility   = 0x44237314,
    BattlePetState             = 0x8F447330,
    BattlePetVisual            = 0xC3ADEB43,
    DB_QUERY_NPC_TEXT          = 0x021826BB,
    Creature                   = 0xC9D6B6B3,
    GameObjects                = 0x13C403A5,
    DB_QUERY_ITEM_SPARSE       = 0x919BE54E,
    DB_QUERY_ITEM              = 0x50238EC2,
    ItemCurrencyCost           = 0x6FE05AE9,
    ItemExtendedCost           = 0xBB858355,
    ItemUpgrade                = 0x7006463B,
    KeyChain                   = 0x6D8A2694,
    Locale                     = 0x3F85ABB7,
    Location                   = 0x394C3727,
    MapChallengeMode           = 0x383B4C27,
    MarketingPromotionsXLocale = 0xA1D3F1AD,
    Path                       = 0x94F46395,
    PathNode                   = 0x3B9E4CA2,
    PathNodeProperty           = 0xFE21C024,
    PathProperty               = 0x08E54F60,
    QuestPackageItem           = 0xCC2F84F0,
    RulesetItemUpgrade         = 0x6DB7086C,
    RulesetRaidLootUpgrade     = 0xED1FBB4D,
    SceneScript                = 0xD4B163CC,
    SceneScriptPackage         = 0xE8CB5E09,
    SceneScriptPackageMember   = 0xE44DB71C,
    SpellReagents              = 0xAB66C99F
};*/

void WorldSession::SendNameQueryOpcode(ObjectGuid guid)
{
    Player* player = ObjectAccessor::FindPlayer(guid);
    CharacterNameData const* nameData = sWorld->GetCharacterNameData(GUID_LOPART(guid));

    WorldPacket data(SMSG_NAME_QUERY_RESPONSE, 500);

    data.WriteBit(guid[4]);
    data.WriteBit(guid[7]);
    data.WriteBit(guid[6]);
    data.WriteBit(guid[2]);
    data.WriteBit(guid[5]);
    data.WriteBit(guid[1]);
    data.WriteBit(guid[3]);
    data.WriteBit(guid[0]);

    data.WriteByteSeq(guid[0]);
    data.WriteByteSeq(guid[6]);
    data.WriteByteSeq(guid[3]);
    data.WriteByteSeq(guid[7]);
    data.WriteByteSeq(guid[2]);
    data.WriteByteSeq(guid[4]);
    data.WriteByteSeq(guid[1]);
    
    data << uint8(!nameData);

    if (nameData)
    {
        data << uint8(nameData->m_class);
        data << uint8(nameData->m_gender);
        data << uint32(sConfigMgr->GetIntDefault("RealmID", 0)); // RealmID
        data << uint8(nameData->m_race);
        data << uint32(50397209); // const player time
        data << uint8(nameData->m_level);
    }

    data.WriteByteSeq(guid[5]);

    if (!nameData)
    {
        SendPacket(&data);
        return;
    }

    data.WriteBit(guid[3]);
    data.WriteBit(0);

    DeclinedName const* names = (player ? player->GetDeclinedNames() : NULL);
    for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
        data.WriteBits(names ? names->name[i].size() : 0, 7);

    data.WriteBit(0);
    data.WriteBit(guid[0]);
    data.WriteBit(0);
    data.WriteBit(guid[4]);
    data.WriteBit(0);
    data.WriteBit(guid[6]);
    data.WriteBit(guid[7]);
    data.WriteBit(0);
    data.WriteBit(0);
    data.WriteBit(0);
    data.WriteBit(guid[1]);
    data.WriteBits(nameData->m_name.size(), 6);
    data.WriteBit(guid[2]);
    data.WriteBit(0);
    data.WriteBit(guid[5]);
    data.WriteBit(0);
    data.FlushBits();

    data.WriteByteSeq(guid[4]);
    data.WriteByteSeq(guid[1]);
    data.WriteByteSeq(guid[5]);
    
    if (names)
        for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
            data.WriteString(names->name[i]);

    data.WriteString(nameData->m_name);

    data.WriteByteSeq(guid[0]);
    data.WriteByteSeq(guid[3]);
    data.WriteByteSeq(guid[7]);
    data.WriteByteSeq(guid[6]);
    data.WriteByteSeq(guid[2]);

    SendPacket(&data);
}

void WorldSession::HandleNameQueryOpcode(WorldPacket& recvData)
{
    ObjectGuid guid;

    uint8 bit20, bit28;
    uint32 unk, unk1;

    bit20 = recvData.ReadBit();
    guid[0] = recvData.ReadBit();
    guid[6] = recvData.ReadBit();
    guid[2] = recvData.ReadBit();
    guid[1] = recvData.ReadBit();
    guid[7] = recvData.ReadBit();
    bit28 = recvData.ReadBit();
    guid[3] = recvData.ReadBit();
    guid[5] = recvData.ReadBit();
    guid[4] = recvData.ReadBit();

    recvData.ReadByteSeq(guid[0]);
    recvData.ReadByteSeq(guid[5]);
    recvData.ReadByteSeq(guid[2]);
    recvData.ReadByteSeq(guid[4]);
    recvData.ReadByteSeq(guid[7]);
    recvData.ReadByteSeq(guid[6]);
    recvData.ReadByteSeq(guid[1]);
    recvData.ReadByteSeq(guid[3]);

    if (bit28)
        recvData >> unk;

    if (bit20)
        recvData >> unk1;

    // This is disable by default to prevent lots of console spam
    // TC_LOG_INFO("network", "HandleNameQueryOpcode %u", guid);

    SendNameQueryOpcode(guid);
}

void WorldSession::SendRealmNameQueryOpcode(uint32 realmId)
{
    std::string realmName = "";
    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_REALMNAME_BY_ID);
    stmt->setInt32(0, realmId);
    PreparedQueryResult result = LoginDatabase.Query(stmt);

    WorldPacket data(SMSG_REALM_NAME_QUERY_RESPONSE);
    data << uint8(!result);
    data << uint32(realmId);
    
    if (result)
    {
        Field* fields = result->Fetch();
        realmName = fields[0].GetString();

        data.WriteBit(1);
        data.WriteBits(realmName.length(), 8);
        data.WriteBits(realmName.length(), 8);
        data.FlushBits();

        data.WriteString(realmName);
        data.WriteString(realmName);
    }

    SendPacket(&data);
}

void WorldSession::HandleRealmNameQueryOpcode(WorldPacket& recvPacket)
{
    uint32 realmId;
    recvPacket >> realmId;
    SendRealmNameQueryOpcode(realmId);
}

void WorldSession::HandleQueryTimeOpcode(WorldPacket & /*recvData*/)
{
    SendQueryTimeResponse();
}

void WorldSession::SendQueryTimeResponse()
{
    WorldPacket data(SMSG_QUERY_TIME_RESPONSE, 4+4);
    data << uint32(time(NULL));
    data << uint32(sWorld->GetNextDailyQuestsResetTime() - time(NULL));
    SendPacket(&data);
}

/// Only _static_ data is sent in this packet !!!
void WorldSession::HandleCreatureQueryOpcode(WorldPacket& recvData)
{
    uint32 entry;
    recvData >> entry;

    WorldPacket data(SMSG_CREATURE_QUERY_RESPONSE, 500);

    CreatureTemplate const* ci = sObjectMgr->GetCreatureTemplate(entry);
    if (ci)
    {
        std::string Name, SubName;
        Name = ci->Name;
        SubName = ci->SubName;

        int loc_idx = GetSessionDbLocaleIndex();
        if (loc_idx >= 0)
        {
            if (CreatureLocale const* cl = sObjectMgr->GetCreatureLocale(entry))
            {
                ObjectMgr::GetLocaleString(cl->Name, loc_idx, Name);
                ObjectMgr::GetLocaleString(cl->SubName, loc_idx, SubName);
            }
        }
        TC_LOG_DEBUG("network", "WORLD: CMSG_CREATURE_QUERY '%s' - Entry: %u.", ci->Name.c_str(), entry);

        data.WriteBit(1);                                    // Has data
        data.WriteBit(ci->RacialLeader);
        data.WriteBits(ci->IconName.length() + 1, 6);
        data.WriteBits(0, 11);
        data.WriteBits(MAX_CREATURE_QUEST_ITEMS, 22);        // Quest items

        for (int i = 0; i < 8; i++)
        {
            if (i == 1)
                data.WriteBits(Name.length() + 1, 11);
            else
                data.WriteBits(0, 11);                       // Name2, ..., name8
        }

        data.WriteBits(SubName.length() ? SubName.length() + 1 : 0, 11);
        data.FlushBits();

        data << uint32(ci->Modelid3);                       // Modelid3
        data << uint32(ci->KillCredit[1]);                  // New in 3.1, kill credit
        data << uint32(ci->type);                           // CreatureType.dbc
        data << Name;
        data << float(ci->ModMana);                         // Mana modifier
        data << uint32(ci->type_flags2);                    // Flags2
        data << uint32(ci->type_flags);                     // Flags
        data << uint32(ci->family);                         // CreatureFamily.dbc
        data << uint32(ci->KillCredit[0]);                  // New in 3.1, kill credit
        data << uint32(ci->Modelid4);                       // Modelid4

        for (uint32 i = 0; i < MAX_CREATURE_QUEST_ITEMS; ++i)
            data << uint32(ci->questItems[i]);              // ItemId[6], quest drop

        data << float(ci->ModHealth);                       // Hp modifier
        data << uint32(ci->movementId);                     // CreatureMovementInfo.dbc
        data << uint32(ci->expansion);                      // Expansion Required

        if (ci->IconName != "")
            data << ci->IconName;                           // "Directions" for guard, string for Icons 2.3.0

        data << uint32(ci->Modelid2);                       // Modelid2
        data << uint32(ci->Modelid1);                       // Modelid1
        data << uint32(ci->rank);                           // Creature Rank (elite, boss, etc)    

        if (SubName != "")
            data << SubName;                                // Subname

        data << uint32(entry);                              // creature entry

        SendPacket(&data);
        TC_LOG_DEBUG("network", "WORLD: Sent SMSG_CREATURE_QUERY_RESPONSE");
    }
    else
    {
        TC_LOG_DEBUG("network", "WORLD: CMSG_CREATURE_QUERY - NO CREATURE INFO! (ENTRY: %u)", entry);
        //data.WriteBit(0);
    }
}

/// Only _static_ data is sent in this packet !!!
void WorldSession::HandleGameObjectQueryOpcode(WorldPacket& recvData)
{
    uint32 entry;
    ObjectGuid guid;

    recvData >> entry;

    guid[6] = recvData.ReadBit();
    guid[3] = recvData.ReadBit();
    guid[1] = recvData.ReadBit();
    guid[2] = recvData.ReadBit();
    guid[0] = recvData.ReadBit();
    guid[7] = recvData.ReadBit();
    guid[5] = recvData.ReadBit();
    guid[4] = recvData.ReadBit();

    recvData.ReadByteSeq(guid[5]);
    recvData.ReadByteSeq(guid[0]);
    recvData.ReadByteSeq(guid[6]);
    recvData.ReadByteSeq(guid[7]);
    recvData.ReadByteSeq(guid[3]);
    recvData.ReadByteSeq(guid[4]);
    recvData.ReadByteSeq(guid[2]);
    recvData.ReadByteSeq(guid[1]);

    const GameObjectTemplate* info = sObjectMgr->GetGameObjectTemplate(entry);
    if (info)
    {
        std::string Name;
        std::string IconName;
        std::string CastBarCaption;

        Name = info->name;
        IconName = info->IconName;
        CastBarCaption = info->castBarCaption;

        int loc_idx = GetSessionDbLocaleIndex();
        if (loc_idx >= 0)
        {
            if (GameObjectLocale const* gl = sObjectMgr->GetGameObjectLocale(entry))
            {
                ObjectMgr::GetLocaleString(gl->Name, loc_idx, Name);
                ObjectMgr::GetLocaleString(gl->CastBarCaption, loc_idx, CastBarCaption);
            }
        }
        TC_LOG_DEBUG("network", "WORLD: CMSG_GAMEOBJECT_QUERY '%s' - Entry: %u. ", info->name.c_str(), entry);
        WorldPacket data (SMSG_GAMEOBJECT_QUERY_RESPONSE, 150);
        data << uint32(entry);
        size_t pos = data.wpos();
        data << uint32(0);

        data << uint32(info->type);
        data << uint32(info->displayId);
        data << Name;
        data << uint8(0) << uint8(0) << uint8(0);           // name2, name3, name4
        data << IconName;                                   // 2.0.3, string. Icon name to use instead of default icon for go's (ex: "Attack" makes sword)
        data << CastBarCaption;                             // 2.0.3, string. Text will appear in Cast Bar when using GO (ex: "Collecting")
        data << info->unk1;                                 // 2.0.3, string

        data.append(info->raw.data, MAX_GAMEOBJECT_DATA);
        data << float(info->size);                          // go size

        data << uint8(MAX_GAMEOBJECT_QUEST_ITEMS);

        for (uint32 i = 0; i < MAX_GAMEOBJECT_QUEST_ITEMS; ++i)
            data << uint32(info->questItems[i]);            // itemId[6], quest drop

        data << int32(info->unkInt32);                      // 4.x, unknown

        data.put(pos, uint32(data.wpos() - pos));

        data.WriteBit(1);
        data.FlushBits();

        SendPacket(&data);

        TC_LOG_DEBUG("network", "WORLD: Sent SMSG_GAMEOBJECT_QUERY_RESPONSE");
    }
    else
    {
        TC_LOG_DEBUG("network", "WORLD: CMSG_GAMEOBJECT_QUERY - Missing gameobject info for (GUID: %u, ENTRY: %u)",
            GUID_LOPART((uint64)guid), entry);
        WorldPacket data (SMSG_GAMEOBJECT_QUERY_RESPONSE, 4);
        data << uint32(entry | 0x80000000);
        data << uint32(0);
        data.WriteBit(0);
        data.FlushBits();
        SendPacket(&data);
        TC_LOG_DEBUG("network", "WORLD: Sent SMSG_GAMEOBJECT_QUERY_RESPONSE");
    }
}

void WorldSession::HandleCorpseQueryOpcode(WorldPacket& /*recvData*/)
{
    TC_LOG_DEBUG("network", "WORLD: Received MSG_CORPSE_QUERY");

    Corpse* corpse = GetPlayer()->GetCorpse();

    if (!corpse)
    {
        WorldPacket data(MSG_CORPSE_QUERY, 1);
        data << uint8(0);                                   // corpse not found
        SendPacket(&data);
        return;
    }

    uint32 mapid = corpse->GetMapId();
    float x = corpse->GetPositionX();
    float y = corpse->GetPositionY();
    float z = corpse->GetPositionZ();
    uint32 corpsemapid = mapid;

    // if corpse at different map
    if (mapid != _player->GetMapId())
    {
        // search entrance map for proper show entrance
        if (MapEntry const* corpseMapEntry = sMapStore.LookupEntry(mapid))
        {
            if (corpseMapEntry->IsDungeon() && corpseMapEntry->entrance_map >= 0)
            {
                // if corpse map have entrance
                if (Map const* entranceMap = sMapMgr->CreateBaseMap(corpseMapEntry->entrance_map))
                {
                    mapid = corpseMapEntry->entrance_map;
                    x = corpseMapEntry->entrance_x;
                    y = corpseMapEntry->entrance_y;
                    z = entranceMap->GetHeight(GetPlayer()->GetPhaseMask(), x, y, MAX_HEIGHT);
                }
            }
        }
    }

    WorldPacket data(MSG_CORPSE_QUERY, 1+(6*4));
    data << uint8(1);                                       // corpse found
    data << int32(mapid);
    data << float(x);
    data << float(y);
    data << float(z);
    data << int32(corpsemapid);
    data << uint32(0);                                      // unknown
    SendPacket(&data);
}

void WorldSession::HandleNpcTextQueryOpcode(WorldPacket& recvData)
{
    uint32 textID;
    ObjectGuid guid;

    recvData >> textID;

    TC_LOG_DEBUG("network", "WORLD: CMSG_NPC_TEXT_QUERY ID '%u'", textID);

    guid[5] = recvData.ReadBit();
    guid[6] = recvData.ReadBit();
    guid[7] = recvData.ReadBit();
    guid[4] = recvData.ReadBit();
    guid[3] = recvData.ReadBit();
    guid[0] = recvData.ReadBit();
    guid[2] = recvData.ReadBit();
    guid[1] = recvData.ReadBit();

    recvData.ReadByteSeq(guid[0]);
    recvData.ReadByteSeq(guid[7]);
    recvData.ReadByteSeq(guid[1]);
    recvData.ReadByteSeq(guid[4]);
    recvData.ReadByteSeq(guid[3]);
    recvData.ReadByteSeq(guid[5]);
    recvData.ReadByteSeq(guid[2]);
    recvData.ReadByteSeq(guid[6]);

    GossipText const* pGossip = sObjectMgr->GetGossipText(textID);

    WorldPacket data(SMSG_NPC_TEXT_UPDATE, 100);          // guess size

    data << textID;

    if (!pGossip)
    {
        for (uint32 i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; ++i)
        {
            data << float(0);
            data << "Greetings $N";
            data << "Greetings $N";
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
        }
    }
    else
    {
        std::string Text_0[MAX_LOCALES], Text_1[MAX_LOCALES];
        for (int i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; ++i)
        {
            Text_0[i]=pGossip->Options[i].Text_0;
            Text_1[i]=pGossip->Options[i].Text_1;
        }

        int loc_idx = GetSessionDbLocaleIndex();
        if (loc_idx >= 0)
        {
            if (NpcTextLocale const* nl = sObjectMgr->GetNpcTextLocale(textID))
            {
                for (int i = 0; i < MAX_LOCALES; ++i)
                {
                    ObjectMgr::GetLocaleString(nl->Text_0[i], loc_idx, Text_0[i]);
                    ObjectMgr::GetLocaleString(nl->Text_1[i], loc_idx, Text_1[i]);
                }
            }
        }

        for (int i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; ++i)
        {
            data << pGossip->Options[i].Probability;

            if (Text_0[i].empty())
                data << Text_1[i];
            else
                data << Text_0[i];

            if (Text_1[i].empty())
                data << Text_0[i];
            else
                data << Text_1[i];

            data << pGossip->Options[i].Language;

            for (int j = 0; j < MAX_GOSSIP_TEXT_EMOTES; ++j)
            {
                data << pGossip->Options[i].Emotes[j]._Delay;
                data << pGossip->Options[i].Emotes[j]._Emote;
            }
        }
    }

    SendPacket(&data);

    TC_LOG_DEBUG("network", "WORLD: Sent SMSG_NPC_TEXT_UPDATE");
}

/*void WorldSession::HandleDbQueryOpcode(WorldPacket& p_ReceivedPacket)
{
    uint32 l_QueryType;
    uint32 l_Count;

    p_ReceivedPacket >> l_QueryType;

    l_Count = p_ReceivedPacket.ReadBits(23);

    std::vector<ObjectGuid> l_Guids;
    std::vector<uint32>		l_requestedEntries;

    for (uint32 l_I = 0 ; l_I < l_Count ; l_I++)
    {
        ObjectGuid l_Guid;

        l_Guid[7] = p_ReceivedPacket.ReadBit();
        l_Guid[5] = p_ReceivedPacket.ReadBit();
        l_Guid[6] = p_ReceivedPacket.ReadBit();
        l_Guid[2] = p_ReceivedPacket.ReadBit();
        l_Guid[1] = p_ReceivedPacket.ReadBit();
        l_Guid[3] = p_ReceivedPacket.ReadBit();
        l_Guid[0] = p_ReceivedPacket.ReadBit();
        l_Guid[4] = p_ReceivedPacket.ReadBit();

        l_Guids.push_back(l_Guid);
    }

    for (uint32 l_I = 0 ; l_I < l_Count ; l_I++)
    {
        uint32	l_requestedEntry;

        p_ReceivedPacket.ReadByteSeq(l_Guids[l_I][7]);
        p_ReceivedPacket.ReadByteSeq(l_Guids[l_I][6]);
        p_ReceivedPacket.ReadByteSeq(l_Guids[l_I][1]);
        p_ReceivedPacket.ReadByteSeq(l_Guids[l_I][2]);
        p_ReceivedPacket >> l_requestedEntry;
        p_ReceivedPacket.ReadByteSeq(l_Guids[l_I][5]);
        p_ReceivedPacket.ReadByteSeq(l_Guids[l_I][3]);
        p_ReceivedPacket.ReadByteSeq(l_Guids[l_I][0]);
        p_ReceivedPacket.ReadByteSeq(l_Guids[l_I][4]);

        l_requestedEntries.push_back(l_requestedEntry);
    }

    if (!l_Count)
        return;

    for(uint32 l_I = 0 ; l_I < l_Count ; l_I++)
    {
        WorldPacket l_Data(SMSG_DB_REPLY);
        l_Data << uint32(0x00);				/// Data size placeholder

        switch (l_QueryType)
        {
        case DB_QUERY_NPC_TEXT:
        {
            SendNpcTextDBQueryResponse(this, l_Data, l_requestedEntries[l_I]);
            break;
        }
        case DB_QUERY_ITEM_SPARSE:
        {
            if(!SendItemSparseDBQueryResponse(this, l_Data, l_requestedEntries[l_I])) continue; //dont send if no item
            break; //to disable the sent of the opcode
        }
        case DB_QUERY_ITEM:
        {
            if(!SendItemDBQueryResponse(this, l_Data, l_requestedEntries[l_I])) continue; //dont send if no item
            break; //to disable the sent of the opcode
        }
        case DB_QUERY_BattlePetEffectProperties:
        {
            l_Data << uint32(l_requestedEntries[l_I]);
            l_Data << uint32(38);
            l_Data << "Points";
            l_Data << "Accuracy";
            l_Data << uint32(43);
            l_Data << uint32(0);
            l_Data << uint32(0);
            l_Data << uint32(0);
            break; //to disable the sent of the opcode
        }
        default:
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "Received non handled db query type 0x%08.8X", l_QueryType);
            continue; //to disable the sent of the opcode
        }
        }

        l_Data << uint32(sObjectMgr->GetHotfixDate(l_requestedEntries[l_I], l_QueryType));
        l_Data << uint32(l_QueryType);
        l_Data << uint32(l_requestedEntries[l_I]);

        l_Data.wpos(0);
        l_Data << uint32(l_Data.size() - 16);

        SendPacket(&l_Data);
    }
}*/

/// Only _static_ data is sent in this packet !!!
void WorldSession::HandlePageTextQueryOpcode(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "WORLD: Received CMSG_PAGE_TEXT_QUERY");

    uint32 pageID;
    recvData >> pageID;
    recvData.read_skip<uint64>();                          // guid

    while (pageID)
    {
        PageText const* pageText = sObjectMgr->GetPageText(pageID);
                                                            // guess size
        WorldPacket data(SMSG_PAGE_TEXT_QUERY_RESPONSE, 50);
        data << pageID;

        if (!pageText)
        {
            data << "Item page missing.";
            data << uint32(0);
            pageID = 0;
        }
        else
        {
            std::string Text = pageText->Text;

            int loc_idx = GetSessionDbLocaleIndex();
            if (loc_idx >= 0)
                if (PageTextLocale const* player = sObjectMgr->GetPageTextLocale(pageID))
                    ObjectMgr::GetLocaleString(player->Text, loc_idx, Text);

            data << Text;
            data << uint32(pageText->NextPage);
            pageID = pageText->NextPage;
        }
        SendPacket(&data);

        TC_LOG_DEBUG("network", "WORLD: Sent SMSG_PAGE_TEXT_QUERY_RESPONSE");
    }
}

void WorldSession::HandleCorpseMapPositionQuery(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "WORLD: Recv CMSG_CORPSE_MAP_POSITION_QUERY");

    uint32 transportGuidLow;
    recvData >> transportGuidLow;

    WorldPacket data(SMSG_CORPSE_MAP_POSITION_QUERY_RESPONSE, 4+4+4+4);
    data << float(0);
    data << float(0);
    data << float(0);
    data << float(0);
    SendPacket(&data);
}

void WorldSession::HandleQuestPOIQuery(WorldPacket& recvData)
{
    uint32 count;
    recvData >> count; // quest count, max=25

    if (count >= MAX_QUEST_LOG_SIZE)
    {
        recvData.rfinish();
        return;
    }

    WorldPacket data(SMSG_QUEST_POI_QUERY_RESPONSE, 4+(4+4)*count);
    data << uint32(count); // count

    for (uint32 i = 0; i < count; ++i)
    {
        uint32 questId;
        recvData >> questId; // quest id

        bool questOk = false;

        uint16 questSlot = _player->FindQuestSlot(questId);

        if (questSlot != MAX_QUEST_LOG_SIZE)
            questOk =_player->GetQuestSlotQuestId(questSlot) == questId;

        if (questOk)
        {
            QuestPOIVector const* POI = sObjectMgr->GetQuestPOIVector(questId);

            if (POI)
            {
                data << uint32(questId); // quest ID
                data << uint32(POI->size()); // POI count

                for (QuestPOIVector::const_iterator itr = POI->begin(); itr != POI->end(); ++itr)
                {
                    data << uint32(itr->Id);                // POI index
                    data << int32(itr->ObjectiveIndex);     // objective index
                    data << uint32(itr->MapId);             // mapid
                    data << uint32(itr->AreaId);            // areaid
                    data << uint32(itr->Unk2);              // unknown
                    data << uint32(itr->Unk3);              // unknown
                    data << uint32(itr->Unk4);              // unknown
                    data << uint32(itr->points.size());     // POI points count

                    for (std::vector<QuestPOIPoint>::const_iterator itr2 = itr->points.begin(); itr2 != itr->points.end(); ++itr2)
                    {
                        data << int32(itr2->x); // POI point x
                        data << int32(itr2->y); // POI point y
                    }
                }
            }
            else
            {
                data << uint32(questId); // quest ID
                data << uint32(0); // POI count
            }
        }
        else
        {
            data << uint32(questId); // quest ID
            data << uint32(0); // POI count
        }
    }

    SendPacket(&data);
}
