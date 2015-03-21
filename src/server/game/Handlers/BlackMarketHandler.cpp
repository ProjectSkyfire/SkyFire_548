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

#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Player.h"
#include "Unit.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "BlackMarketMgr.h"

void WorldSession::HandleBlackMarketOpen(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "WORLD: Received CMSG_BLACK_MARKET_OPEN");

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

    SendBlackMarketOpenResult(npcGuid);
}

void WorldSession::SendBlackMarketOpenResult(uint64 npcGuid)
{
    ObjectGuid guid = npcGuid;
    bool open = sWorld->getIntConfig(CONFIG_BLACK_MARKET_OPEN);

    WorldPacket data(SMSG_BLACK_MARKET_OPEN_RESULT, 9);

    data.WriteBit(guid[2]);
    data.WriteBit(guid[0]);
    data.WriteBit(guid[4]);
    data.WriteBit(guid[1]);
    data.WriteBit(guid[3]);
    data.WriteBit(guid[6]);
    data.WriteBit(guid[5]);
    data.WriteBit(guid[7]);
    data.WriteBit(open);               // Open / Closed

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

void WorldSession::HandleBlackMarketRequestItems(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "WORLD: CMSG_BLACK_MARKET_REQUEST_ITEMS");

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
    WorldPacket data(SMSG_BLACK_MARKET_REQUEST_ITEMS_RESULT);
    
    sBlackMarketMgr->BuildBlackMarketRequestItemsResult(data, GetPlayer()->GetGUIDLow());
    
    SendPacket(&data);
}

void WorldSession::HandleBlackMarketBid(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "WORLD: CMSG_BLACK_MARKET_BID");

    ObjectGuid guid;
    uint32 itemEntry, id;
    uint64 bidAmount;

    recvData >> itemEntry >> id >> bidAmount;

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

    TC_LOG_DEBUG("blackMarket", ">> HandleBlackMarketBid >> Id : %u, BidAmount : %u, ItemEntry : %u", id, bidAmount, itemEntry);

    uint64 npcGuid = uint64(guid);

    if (!bidAmount)
        return;

    Creature* creature = GetPlayer()->GetNPCIfCanInteractWith(npcGuid, UNIT_NPC_FLAG_BLACKMARKET);
    if (!creature)
    {
        TC_LOG_DEBUG("blackMarket", "HandleBlackMarketBid - Unit (GUID: %u) not found or you can't interact with him.", uint32(GUID_LOPART(npcGuid)));
        return;
    }

    BlackMarketAuction *auction = sBlackMarketMgr->GetAuction(id);
    if (!auction)
    {
        TC_LOG_DEBUG("blackMarket", "HandleBlackMarketBid - Auction (Id: %u) not found.", id);
        return;
    }

    if (auction->GetCurrentBidder() == GetPlayer()->GetGUIDLow())
    {
        TC_LOG_DEBUG("blackMarket", "HandleBlackMarketBid - Player (GUID: %u) is already the highest bidder.", GetPlayer()->GetGUIDLow());
        return;
    }

    if (auction->GetCurrentBid() >= bidAmount && bidAmount != auction->GetTemplate()->MinBid)
    {
        TC_LOG_DEBUG("blackMarket", "HandleBlackMarketBid - Player (GUID: %u) could not bid. The current bid (%u) is higher than the given amount (%u).", GetPlayer()->GetGUIDLow(), auction->GetCurrentBid(), bidAmount);
        return;
    }

    uint64 currentRequiredIncrement = auction->GetCurrentBid() + auction->GetMinIncrement();
    if (currentRequiredIncrement >= bidAmount)
    {
        TC_LOG_DEBUG("blackMarket", "HandleBlackMarketBid - Player (GUID: %u) could not bid. The BidAmount (%u) is lower than the current requiredIncrement (%u).", GetPlayer()->GetGUIDLow(), bidAmount, currentRequiredIncrement);
        return;
    }

    uint64 newIncrement = bidAmount - currentRequiredIncrement;

    if (!GetPlayer()->ModifyMoney(-int64(bidAmount)))
    {
        TC_LOG_DEBUG("blackMarket", "HandleBlackMarketBid - Player (GUID: %u) has not enough money to bid.", GetPlayer()->GetGUIDLow());
        return;
    }

    sBlackMarketMgr->UpdateAuction(auction, currentRequiredIncrement, newIncrement, GetPlayer());

    SendBlackMarketBidOnItemResult(itemEntry);
    SendBlackMarketRequestItemsResult();
}

void WorldSession::SendBlackMarketBidOnItemResult(uint32 itemEntry)
{
    TC_LOG_DEBUG("network", "WORLD: SMSG_BLACK_MARKET_BID_ON_ITEM_RESULT");

    WorldPacket data(SMSG_BLACK_MARKET_BID_ON_ITEM_RESULT, 12);
    
    data << uint32(2);              // MarketID might be Result
    data << uint32(itemEntry);      // ItemEntry
    data << uint32(2);              // Result might be MarketID
    
    SendPacket(&data);
}
