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
#include "ObjectMgr.h"
#include "Player.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "DatabaseEnv.h"
#include "DBCStores.h"
#include "ScriptMgr.h"
#include "AccountMgr.h"
#include "BlackMarketMgr.h"
#include "Item.h"
#include "Language.h"
#include "Log.h"
#include <vector>

BlackMarketMgr::~BlackMarketMgr()
{
    for (BMAuctionStore::const_iterator itr = GetAuctionsBegin(); itr != GetAuctionsEnd(); ++itr)
        delete itr->second;

    for (BMAuctionTemplateStore::const_iterator itr = GetTemplatesBegin(); itr != GetTemplatesEnd(); ++itr)
        delete itr->second;
}

BlackMarketMgr::BlackMarketMgr() { }

BlackMarketAuctionTemplate* BlackMarketAuction::GetTemplate() const
{
    BlackMarketAuctionTemplate* bmTemplate = sBlackMarketMgr->GetTemplate(GetTemplateId());
    if (bmTemplate)
        return bmTemplate;

    return NULL;
}

void BlackMarketAuction::SaveToDB(SQLTransaction& trans)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_BLACKMARKET_AUCTION);
    stmt->setUInt32(0, GetAuctionId());
    stmt->setUInt32(1, GetTemplateId());
    stmt->setUInt32(2, GetStartTime());
    stmt->setUInt32(3, GetCurrentBidder());
    stmt->setUInt32(4, GetCurrentBid());
    stmt->setUInt32(5, GetMinIncrement());
    stmt->setUInt32(6, GetNumBids());
    trans->Append(stmt);
}

void BlackMarketAuction::DeleteFromDB(SQLTransaction& trans)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_BLACKMARKET_AUCTION);
    stmt->setUInt32(0, GetAuctionId());
    trans->Append(stmt);
}

void BlackMarketAuction::UpdateToDB(SQLTransaction& trans)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_BLACKMARKET_AUCTION);
    stmt->setUInt32(0, GetCurrentBidder());
    stmt->setUInt32(1, GetCurrentBid());
    stmt->setUInt32(2, GetMinIncrement());
    stmt->setUInt32(3, GetNumBids());
    stmt->setUInt32(4, GetAuctionId());
    trans->Append(stmt);
}

uint32 BlackMarketAuction::TimeLeft()
{
    uint32 endTime = GetStartTime() + GetTemplate()->Duration;
    uint32 curTime = time(NULL);
    return (endTime >= curTime) ? endTime - curTime : 0;
}

void BlackMarketMgr::LoadBlackMarketTemplates()
{
    uint32 count = 0;
    uint32 oldMSTime = getMSTime();

    if (PreparedQueryResult result = WorldDatabase.Query(WorldDatabase.GetPreparedStatement(WORLD_SEL_BLACKMARKET_TEMPLATE)))
    {
        do
        {
            Field* fields = result->Fetch();

            BlackMarketAuctionTemplate* blackmarket_template = new BlackMarketAuctionTemplate();

            blackmarket_template->Id = fields[0].GetUInt32();
            blackmarket_template->MarketId = fields[1].GetUInt32();
            blackmarket_template->SellerNPCEntry = fields[2].GetUInt32();
            if (!sObjectMgr->GetCreatureTemplate(blackmarket_template->SellerNPCEntry)) 
            {
                SF_LOG_ERROR("sql.sql", "Table `blackmarket_template` (MarketId: %u) have data for not existing creature template (Entry: %u), ignoring", blackmarket_template->MarketId, blackmarket_template->SellerNPCEntry); 
                continue; 
            }
            blackmarket_template->ItemEntry = fields[3].GetUInt32();
            if (!sObjectMgr->GetItemTemplate(blackmarket_template->ItemEntry))
            { 
                SF_LOG_ERROR("sql.sql", "Table `blackmarket_template` (MarketId: %u) have data for not existing item template (Entry: %u), ignoring.", blackmarket_template->MarketId, blackmarket_template->ItemEntry); 
                continue; 
            }
            blackmarket_template->Quantity = fields[4].GetUInt32();
            if (!blackmarket_template->Quantity)
            { 
                SF_LOG_ERROR("sql.sql", "Table `blackmarket_template` (MarketId: %u) have amount == 0 for (ItemEntry : %u) in `blackmarket_template` table, ignoring.", blackmarket_template->MarketId, blackmarket_template->ItemEntry); 
                continue; 
            } 
            blackmarket_template->MinBid = fields[5].GetUInt32();
            blackmarket_template->Duration = fields[6].GetUInt32();
            blackmarket_template->Chance = fields[7].GetFloat();

            _templates[blackmarket_template->Id] = blackmarket_template;

            ++count;
        } while (result->NextRow());
    }

    SF_LOG_INFO("server.loading", ">> Loaded %u BlackMarket templates in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}
void BlackMarketMgr::LoadBlackMarketAuctions()
{
    uint32 count = 0;
    uint32 oldMSTime = getMSTime();

    if (PreparedQueryResult result = CharacterDatabase.Query(CharacterDatabase.GetPreparedStatement(CHAR_SEL_BLACKMARKET_AUCTIONS)))
    {
        SQLTransaction trans = CharacterDatabase.BeginTransaction();
        do
        {
            Field* fields = result->Fetch();

            uint32 auctionId = fields[0].GetUInt32();
            uint32 templateId = fields[1].GetUInt32();
            uint32 startTime = fields[2].GetUInt32();
            uint32 currentBidder = fields[3].GetUInt32();
            uint32 currentBid = fields[4].GetUInt32();
            uint32 minIncrement = fields[5].GetUInt32();
            uint32 numBids = fields[6].GetUInt32();

            BlackMarketAuction* auction = new BlackMarketAuction(auctionId, templateId, startTime, currentBidder, currentBid, minIncrement, numBids);

            if (auction->GetTemplate() == NULL)
            {
                auction->DeleteFromDB(trans);
                delete auction;
                continue;
            }

            _auctions[auction->GetAuctionId()] = auction;

            ++count;

        } while (result->NextRow());

        CharacterDatabase.CommitTransaction(trans);
    }

    SF_LOG_INFO("server.loading", ">> Loaded %u BlackMarket Auctions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void BlackMarketMgr::Update()
{
    uint32 curTime = time(NULL);

    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    // Delete expired auctions
    for (BMAuctionStore::const_iterator itr = GetAuctionsBegin(); itr != GetAuctionsEnd();)
    {
        BlackMarketAuction* auction = itr->second;
        if (auction->IsExpired())
        {
            if (auction->GetCurrentBidder())
                SendAuctionWon(auction, trans);

            auction->DeleteFromDB(trans);
            _auctions.erase((itr++)->first);
        }
        else
            ++itr;
    }

    // Add New Auctions
    int32 add = sWorld->getIntConfig(CONFIG_BLACK_MARKET_MAX_AUCTIONS) - _auctions.size();
    if (add > 0)
        CreateAuctions(add, trans);

    CharacterDatabase.CommitTransaction(trans);
}

uint32 BlackMarketMgr::GetFreeAuctionId()
{
    uint32 newId = 1;
    while (GetAuction(newId)) { ++newId; }
    return newId;
}

void BlackMarketMgr::CreateAuctions(uint32 number, SQLTransaction& trans)
{
    if (_templates.empty())
        return;

    for (uint32 i = 0; i < number; ++i)
    {
        // Select a template
        std::vector<uint32> templateList;
        uint32 rand = urand(1, 100);

        for (BMAuctionTemplateStore::const_iterator itr = GetTemplatesBegin(); itr != GetTemplatesEnd(); ++itr)
        {
            if (itr->second->Chance >= rand)
                templateList.push_back(itr->first);
        }

        for (BMAuctionStore::const_iterator itr = GetAuctionsBegin(); itr != GetAuctionsEnd(); ++itr)
            templateList.erase(std::remove(templateList.begin(), templateList.end(), itr->second->GetTemplateId()), templateList.end());

        if (templateList.empty())
            continue;

        BlackMarketAuctionTemplate *selTemplate = GetTemplate(templateList[urand(0, templateList.size() - 1)]);

        if (!selTemplate)
            continue;

        uint32 startTime = time(NULL) + sWorld->getIntConfig(CONFIG_BLACK_MARKET_AUCTION_DELAY) + urand(0, sWorld->getIntConfig(CONFIG_BLACK_MARKET_AUCTION_DELAY_MOD) * 2) - sWorld->getIntConfig(CONFIG_BLACK_MARKET_AUCTION_DELAY_MOD) / 2;

        uint32 minIncrement = selTemplate->MinBid * 0.05f;

        BlackMarketAuction* auction = new BlackMarketAuction;
        auction->SetAuctionId(GetFreeAuctionId());
        auction->SetCurrentBid(selTemplate->MinBid);
        auction->SetCurrentBidder(0);
        auction->SetNumBids(0);
        auction->SetMinIncrement(minIncrement);
        auction->SetStartTime(startTime);
        auction->SetTemplateId(selTemplate->Id);

        _auctions[auction->GetAuctionId()] = auction;

        auction->SaveToDB(trans);
    }
}

void BlackMarketMgr::BuildBlackMarketRequestItemsResult(WorldPacket& data, uint32 guidLow)
{
    uint32 count = 0;

    data << uint32(1);                                      // LastUpdateID
    data.WriteBits(count, 18);                              // ItemCount Placeholder

    for (BMAuctionStore::const_iterator itr = GetAuctionsBegin(); itr != GetAuctionsEnd(); ++itr)
    {
        BlackMarketAuction* auction = itr->second;
        if (!auction->IsActive())
            continue;

        data.WriteBit((guidLow == auction->GetCurrentBidder()));

        ++count;
    }

    data.FlushBits();

    for (BMAuctionStore::const_iterator itr = GetAuctionsBegin(); itr != GetAuctionsEnd(); ++itr)
    {
        BlackMarketAuction* auction = itr->second;
        if (!auction->IsActive())
            continue;

        data << uint32(auction->GetTemplate()->MarketId);       // MarketId
        data << uint32(auction->GetTemplate()->Quantity);       // Quantity
        data << uint32(auction->GetAuctionId());                // AuctionId
        data << uint32(auction->GetNumBids());                  // NumBids
        data << uint32(auction->GetTemplate()->ItemEntry);      // ItemEntry
        data << uint64(auction->GetTemplate()->MinBid);         // MinBid
        data << uint64(auction->GetCurrentBid());               // CurrentBid
        data << uint64(auction->GetMinIncrement());             // MinIncrement
        data << uint32(auction->GetTemplate()->SellerNPCEntry); // SellerNPC
        data << uint32(auction->TimeLeft());                    // SecondsRemaining
    }

    data.PutBits<uint32>(32, count, 18);

    SF_LOG_DEBUG("network", ">> Sent %u Black Market Auctions", count);
}

void BlackMarketMgr::UpdateAuction(BlackMarketAuction* auction, uint64 newPrice, uint64 requiredIncrement, Player* newBidder)
{
    uint64 currentBid = newPrice + requiredIncrement;
    uint64 minIncrement = currentBid * 0.05f;
    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    if (auction->GetCurrentBidder())
        SendAuctionOutbidded(auction, newPrice, newBidder, trans);

    auction->SetCurrentBid(currentBid);
    auction->SetCurrentBidder(newBidder->GetGUIDLow());
    auction->SetMinIncrement(minIncrement);
    auction->SetNumBids(auction->GetNumBids() + 1);

    auction->UpdateToDB(trans);

    CharacterDatabase.CommitTransaction(trans);
}

BlackMarketAuctionTemplate* BlackMarketMgr::GetTemplate(uint32 templateId) const
{
    for (BMAuctionTemplateStore::const_iterator itr = _templates.begin(); itr != _templates.end(); ++itr)
        if (itr->second->Id == templateId)
            return itr->second;

    SF_LOG_DEBUG("blackMarket", "BlackMarketMgr::GetTemplate: [%u] not found!", templateId);
    return NULL;
}

BlackMarketAuction* BlackMarketMgr::GetAuction(uint32 auctionId) const
{
    for (BMAuctionStore::const_iterator itr = _auctions.begin(); itr != _auctions.end(); ++itr)
        if (itr->second->GetAuctionId() == auctionId)
            return itr->second;

    SF_LOG_DEBUG("blackMarket", "BlackMarketMgr::GetAuction: [%u] not found!", auctionId);
    return NULL;
}

std::string BlackMarketAuction::BuildAuctionMailSubject(BMMailAuctionAnswers response)
{
    std::ostringstream strm;
    strm << GetTemplate()->ItemEntry << ":0:" << response << ':' << GetAuctionId() << ':' << GetTemplate()->Quantity;
    return strm.str();
}

std::string BlackMarketAuction::BuildAuctionMailBody(uint32 lowGuid)
{
    std::ostringstream strm;
    strm.width(16);
    strm << std::right << std::hex << MAKE_NEW_GUID(lowGuid, 0, HIGHGUID_PLAYER);   // HIGHGUID_PLAYER always present, even for empty guids
    strm << std::dec << ':' << GetCurrentBid() << ':' << 0;
    strm << ':' << 0 << ':' << 0;
    return strm.str();
}

void BlackMarketMgr::SendAuctionOutbidded(BlackMarketAuction* auction, uint32 newPrice, Player* newBidder, SQLTransaction& trans)
{
    uint64 bidder_guid = MAKE_NEW_GUID(auction->GetCurrentBidder(), 0, HIGHGUID_PLAYER);
    Player* bidder = ObjectAccessor::FindPlayer(bidder_guid);
    uint32 bidder_accId = sObjectMgr->GetPlayerAccountIdByGUID(bidder_guid);

    if (bidder || bidder_accId)
    {
        if (bidder)
        {
            WorldPacket data(SMSG_BLACKMARKET_OUT_BID, 12);

            data << uint32(auction->GetTemplate()->ItemEntry);
            data << uint32(1);
            data << uint32(1);
            bidder->GetSession()->SendPacket(&data);
        }
    
        MailDraft(auction->BuildAuctionMailSubject(BM_AUCTION_OUTBIDDED), auction->BuildAuctionMailBody(auction->GetTemplate()->SellerNPCEntry))
            .AddMoney(auction->GetCurrentBid())
            .SendMailTo(trans, MailReceiver(bidder, auction->GetCurrentBidder()), auction, MAIL_CHECK_MASK_COPIED);
    }
}

void BlackMarketMgr::SendAuctionWon(BlackMarketAuction* auction, SQLTransaction& trans)
{
    uint64 bidder_guid = MAKE_NEW_GUID(auction->GetCurrentBidder(), 0, HIGHGUID_PLAYER);
    Player* bidder = ObjectAccessor::FindPlayer(bidder_guid);
    uint32 bidder_accId = sObjectMgr->GetPlayerAccountIdByGUID(bidder_guid);

    if (bidder || bidder_accId)
    {
        if (bidder)
        {
            WorldPacket data(SMSG_BLACKMARKET_BID_WON, 12);
            data << uint32(1);                                  // 6 - might be OK - 6 (win msg recvd)
            data << uint32(auction->GetTemplate()->ItemEntry);  // 4 - ItemEntry
            data << uint32(1);                                  // 5 - might be OK - 5 (win msg recvd)

            bidder->GetSession()->SendPacket(&data);
        }

        ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(auction->GetTemplate()->ItemEntry);
        if (!itemTemplate)
            return;

        Item* pItem = Item::CreateItem(auction->GetTemplate()->ItemEntry, auction->GetTemplate()->Quantity, bidder);

        MailDraft draft(auction->BuildAuctionMailSubject(BM_AUCTION_WON), auction->BuildAuctionMailBody(auction->GetCurrentBidder()));
        
        SQLTransaction trans = CharacterDatabase.BeginTransaction();
        if (pItem)
        {
            pItem->SaveToDB(trans);
            draft.AddItem(pItem);
        }
        draft.SendMailTo(trans, MailReceiver(bidder, auction->GetCurrentBidder()), MailSender(auction), MAIL_CHECK_MASK_COPIED);
        CharacterDatabase.CommitTransaction(trans);
    }
}
