/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "BlackMarketMgr.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Player.h"
#include "Unit.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"

void WorldSession::HandleBlackMarketHelloOpcode(WorldPacket& recvData)
{
    ObjectGuid NpcGUID;
    recvData.ReadGuidMask(NpcGUID, 4, 5, 2, 7, 0, 1, 3, 6);
    recvData.ReadGuidBytes(NpcGUID, 4, 3, 0, 6, 2, 7, 5, 1);

    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    SendBlackMarketHello(NpcGUID, sBlackMarketMgr->isBlackMarketOpen());
}

void WorldSession::SendBlackMarketHello(ObjectGuid NpcGUID, bool Open)
{
    WorldPacket data(SMSG_BLACKMARKET_HELLO, 9);
    data.WriteGuidMask(NpcGUID, 2, 0, 4, 1, 3, 6, 5, 7);
    data.WriteBit(Open);
    data.WriteGuidBytes(NpcGUID, 6, 1, 2, 5, 0, 7, 4, 3);
    SendPacket(&data);
}

void WorldSession::HandleBlackMarketRequestItemOpcode(WorldPacket& recvData)
{
    ObjectGuid NpcGUID;
    uint32 Timestamp;
    recvData >> Timestamp;
    recvData.ReadGuidMask(NpcGUID, 2, 6, 0, 3, 4, 5, 1, 7);
    recvData.ReadGuidBytes(NpcGUID, 6, 2, 3, 5, 7, 4, 1, 0);
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
    ObjectGuid NpcGUID;
    uint32 ItemID, MarketID;
    uint64 BidAmount;

    recvData >> ItemID >> MarketID >> BidAmount;
    recvData.ReadGuidMask(NpcGUID, 0, 5, 4, 3, 7, 6, 1, 2);
    recvData.ReadGuidBytes(NpcGUID, 4, 3, 6, 5, 7, 1, 0, 2);

    SF_LOG_DEBUG("blackMarket", ">> HandleBlackMarketBid >> MarketID : %u, BidAmount : " UI64FMTD ", ItemID : %u", MarketID, BidAmount, ItemID);

    if (!BidAmount)
        return;

    BlackMarketAuction* auction = sBlackMarketMgr->GetAuction(MarketID);
    if (!auction)
    {
        SF_LOG_DEBUG("blackMarket", "HandleBlackMarketBid - Auction (MarketID: %u) not found.", MarketID);
        return;
    }

    if (auction->GetCurrentBidder() == GetPlayer()->GetGUIDLow())
    {
        SF_LOG_DEBUG("blackMarket", "HandleBlackMarketBid - Player (GUID: %u) is already the highest bidder.", GetPlayer()->GetGUIDLow());
        return;
    }

    if (auction->GetCurrentBid() > BidAmount && BidAmount != auction->GetTemplate()->MinBid)
    {
        SF_LOG_DEBUG("blackMarket", "HandleBlackMarketBid - Player (GUID: %u) could not bid. The current bid (" UI64FMTD ") is higher than the given amount (" UI64FMTD ").", GetPlayer()->GetGUIDLow(), auction->GetCurrentBid(), BidAmount);
        return;
    }

    uint64 currentRequiredIncrement = auction->GetCurrentBid() + auction->GetMinIncrement();
    if (currentRequiredIncrement > BidAmount)
    {
        SF_LOG_DEBUG("blackMarket", "HandleBlackMarketBid - Player (GUID: %u) could not bid. The BidAmount (" UI64FMTD ") is lower than the current requiredIncrement (" UI64FMTD ").", GetPlayer()->GetGUIDLow(), BidAmount, currentRequiredIncrement);
        return;
    }

    uint64 newIncrement = BidAmount - currentRequiredIncrement;

    if (!GetPlayer()->ModifyMoney(-int64(BidAmount)))
    {
        SF_LOG_DEBUG("blackMarket", "HandleBlackMarketBid - Player (GUID: %u) has not enough money to bid.", GetPlayer()->GetGUIDLow());
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