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
    
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    SendBlackMarketHello(npcGuid);
}

void WorldSession::SendBlackMarketHello(uint64 npcGuid)
{
    bool Open = sWorld->getBoolConfig(CONFIG_BLACK_MARKET_OPEN);
    WorldPacket data(SMSG_BLACKMARKET_HELLO, 9);

    ObjectGuid UnitGUID = npcGuid;

    data.WriteBit(UnitGUID[2]);
    data.WriteBit(UnitGUID[0]);
    data.WriteBit(UnitGUID[4]);
    data.WriteBit(UnitGUID[1]);
    data.WriteBit(UnitGUID[3]);
    data.WriteBit(UnitGUID[6]);
    data.WriteBit(UnitGUID[5]);
    data.WriteBit(UnitGUID[7]);
    data.WriteBit(Open);      // 

    data.WriteByteSeq(UnitGUID[6]);
    data.WriteByteSeq(UnitGUID[1]);
    data.WriteByteSeq(UnitGUID[2]);
    data.WriteByteSeq(UnitGUID[5]);
    data.WriteByteSeq(UnitGUID[0]);
    data.WriteByteSeq(UnitGUID[7]);
    data.WriteByteSeq(UnitGUID[4]);
    data.WriteByteSeq(UnitGUID[3]);

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

    SendBlackMarketRequestItemsResult();
}

void WorldSession::SendBlackMarketRequestItemsResult()
{
    WorldPacket data(SMSG_BLACKMARKET_REQUEST_ITEMS_RESULT);
    
    sBlackMarketMgr->BuildBlackMarketRequestItemsResult(data, GetPlayer()->GetGUIDLow());

    SendPacket(&data);
}

void WorldSession::HandleBlackMarketBidOnItem(WorldPacket& recvData)
{
    ObjectGuid UnitGUID;
    uint32 ItemID, MarketID;
    uint64 BidAmount;

    recvData >> ItemID >> MarketID >> BidAmount;

    UnitGUID[0] = recvData.ReadBit();
    UnitGUID[5] = recvData.ReadBit();
    UnitGUID[4] = recvData.ReadBit();
    UnitGUID[3] = recvData.ReadBit();
    UnitGUID[7] = recvData.ReadBit();
    UnitGUID[6] = recvData.ReadBit();
    UnitGUID[1] = recvData.ReadBit();
    UnitGUID[2] = recvData.ReadBit();

    recvData.ReadByteSeq(UnitGUID[4]);
    recvData.ReadByteSeq(UnitGUID[3]);
    recvData.ReadByteSeq(UnitGUID[6]);
    recvData.ReadByteSeq(UnitGUID[5]);
    recvData.ReadByteSeq(UnitGUID[7]);
    recvData.ReadByteSeq(UnitGUID[1]);
    recvData.ReadByteSeq(UnitGUID[0]);
    recvData.ReadByteSeq(UnitGUID[2]);

    TC_LOG_DEBUG("blackMarket", ">> HandleBlackMarketBid >> MarketID : %u, BidAmount : " UI64FMTD ", ItemID : %u", MarketID, BidAmount, ItemID);

    uint64 npcGuid = uint64(UnitGUID);

    if (!BidAmount)
        return;

    BlackMarketAuction *auction = sBlackMarketMgr->GetAuction(MarketID);
    if (!auction)
    {
        TC_LOG_DEBUG("blackMarket", "HandleBlackMarketBid - Auction (MarketID: %u) not found.", MarketID);
        return;
    }

    if (auction->GetCurrentBidder() == GetPlayer()->GetGUIDLow())
    {
        TC_LOG_DEBUG("blackMarket", "HandleBlackMarketBid - Player (GUID: %u) is already the highest bidder.", GetPlayer()->GetGUIDLow());
        return;
    }

    if (auction->GetCurrentBid() > BidAmount && BidAmount != auction->GetTemplate()->MinBid)
    {
        TC_LOG_DEBUG("blackMarket", "HandleBlackMarketBid - Player (GUID: %u) could not bid. The current bid (%u) is higher than the given amount (" UI64FMTD ").", GetPlayer()->GetGUIDLow(), auction->GetCurrentBid(), BidAmount);
        return;
    }

    uint64 currentRequiredIncrement = auction->GetCurrentBid() + auction->GetMinIncrement();
    if (currentRequiredIncrement > BidAmount)
    {
        TC_LOG_DEBUG("blackMarket", "HandleBlackMarketBid - Player (GUID: %u) could not bid. The BidAmount (" UI64FMTD ") is lower than the current requiredIncrement (" UI64FMTD ").", GetPlayer()->GetGUIDLow(), BidAmount, currentRequiredIncrement);
        return;
    }

    uint64 newIncrement = BidAmount - currentRequiredIncrement;

    if (!GetPlayer()->ModifyMoney(-int64(BidAmount)))
    {
        TC_LOG_DEBUG("blackMarket", "HandleBlackMarketBid - Player (GUID: %u) has not enough money to bid.", GetPlayer()->GetGUIDLow());
        return;
    }

    sBlackMarketMgr->UpdateAuction(auction, currentRequiredIncrement, newIncrement, GetPlayer());


    SendBlackMarketBidOnItemResult(ItemID);
    SendBlackMarketRequestItemsResult();
}

void WorldSession::SendBlackMarketBidOnItemResult(uint32 ItemID)
{
    WorldPacket data(SMSG_BLACKMARKET_BID_RESULT, 12);
    data << uint32(2);              // MarketID might be Result
    data << uint32(ItemID);         // ItemID
    data << uint32(2);              // Result might be MarketID
    SendPacket(&data);
}