/*
 * Copyright (C) 2011-2014 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2014 MaNGOS <http://getmangos.com/>
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

#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Player.h"
#include "Unit.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"

void WorldSession::HandleBlackMarketHelloOpcode(WorldPacket& recvData)
{
    ObjectGuid guid;

    guid[4] = recvData.ReadBit();
    guid[5] = recvData.ReadBit();
    guid[2] = recvData.ReadBit();
    guid[7] = recvData.ReadBit();
    guid[0] = recvData.ReadBit();
    guid[1] = recvData.ReadBit();
    guid[3] = recvData.ReadBit();
    guid[6] = recvData.ReadBit();

    recvData.ReadByteSeq(guid[3]);
    recvData.ReadByteSeq(guid[5]);
    recvData.ReadByteSeq(guid[0]);
    recvData.ReadByteSeq(guid[6]);
    recvData.ReadByteSeq(guid[4]);
    recvData.ReadByteSeq(guid[1]);
    recvData.ReadByteSeq(guid[7]);
    recvData.ReadByteSeq(guid[2]);

    uint64 npcGuid = uint64(guid);

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(npcGuid, UNIT_NPC_FLAG_BLACKMARKET);

    if (!unit)
    {
        TC_LOG_DEBUG("network", "WORLD: Received CMSG_BLACKMARKET_HELLO - Unit (GUID: %u) not found or you can't interact with him.", uint32(GUID_LOPART(npcGuid)));
        return;
    }

    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    SendBlackMarketHello(npcGuid);
}

void WorldSession::SendBlackMarketHello(uint64 npcGuid)
{
    WorldPacket data(SMSG_BLACKMARKET_HELLO, 8);

    ObjectGuid guid = npcGuid;

    data.WriteBit(guid[2]);
    data.WriteBit(guid[0]);
    data.WriteBit(guid[4]);
    data.WriteBit(guid[1]);
    data.WriteBit(guid[3]);
    data.WriteBit(guid[6]);
    data.WriteBit(guid[5]);
    data.WriteBit(guid[7]);
    data.WriteBit(1);      //Unknow

    data.WriteByteSeq(guid[6]);
    data.WriteByteSeq(guid[1]);
    data.WriteByteSeq(guid[2]);
    data.WriteByteSeq(guid[5]);
    data.WriteByteSeq(guid[0]);
    data.WriteByteSeq(guid[7]);
    data.WriteByteSeq(guid[4]);
    data.WriteByteSeq(guid[3]);

    SendPacket(&data);
}

void WorldSession::HandleBlackMarketRequestItemOpcode(WorldPacket& recvData)
{
    ObjectGuid guid;
    uint32 Timestamp;

    recvData >> Timestamp;

    guid[2] = recvData.ReadBit();
    guid[6] = recvData.ReadBit();
    guid[0] = recvData.ReadBit();
    guid[3] = recvData.ReadBit();
    guid[4] = recvData.ReadBit();
    guid[5] = recvData.ReadBit();
    guid[1] = recvData.ReadBit();
    guid[7] = recvData.ReadBit();

    recvData.ReadByteSeq(guid[6]);
    recvData.ReadByteSeq(guid[2]);
    recvData.ReadByteSeq(guid[3]);
    recvData.ReadByteSeq(guid[5]);
    recvData.ReadByteSeq(guid[7]);
    recvData.ReadByteSeq(guid[4]);
    recvData.ReadByteSeq(guid[1]);
    recvData.ReadByteSeq(guid[0]);

    uint64 NpcGuid = uint64(guid);

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(NpcGuid, UNIT_NPC_FLAG_BLACKMARKET);
    if (!unit)
    {
        TC_LOG_DEBUG("network", "WORLD: Received CMSG_BLACKMARKET_REQUEST_ITEMS - Unit (GUID: %u) not found or you can't interact with him.", uint32(GUID_LOPART(NpcGuid)));
        return;
    }

    SendBlackMarketRequestItemsResult();
}

void WorldSession::SendBlackMarketRequestItemsResult()
{
    WorldPacket data(SMSG_BLACKMARKET_REQUEST_ITEMS_RESULT);
    //need structure
    SendPacket(&data);
}

void WorldSession::HandleBlackMarketBidOpcode(WorldPacket& recvData)
{
    ObjectGuid guid;
    uint32 itemid, id;
    uint64 price;

    recvData >> id >> itemid >> price;

    guid[0] = recvData.ReadBit();
    guid[5] = recvData.ReadBit();
    guid[4] = recvData.ReadBit();
    guid[3] = recvData.ReadBit();
    guid[7] = recvData.ReadBit();
    guid[6] = recvData.ReadBit();
    guid[1] = recvData.ReadBit();
    guid[2] = recvData.ReadBit();

    recvData.ReadByteSeq(guid[4]);
    recvData.ReadByteSeq(guid[3]);
    recvData.ReadByteSeq(guid[6]);
    recvData.ReadByteSeq(guid[5]);
    recvData.ReadByteSeq(guid[7]);
    recvData.ReadByteSeq(guid[1]);
    recvData.ReadByteSeq(guid[0]);
    recvData.ReadByteSeq(guid[2]);
    //structure ok need implement database data

    SendBlackMarketBidResult();
    SendBlackMarketRequestItemsResult();
}

void WorldSession::SendBlackMarketBidResult()
{
    WorldPacket data(SMSG_BLACKMARKET_BID_RESULT, 5);
    data << uint32(0); //unk
    SendPacket(&data);
}