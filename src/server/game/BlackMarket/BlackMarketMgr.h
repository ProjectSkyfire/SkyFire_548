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
 
#ifndef BLACK_MARKET_H
#define BLACK_MARKET_H


#include <ace/Singleton.h>

#include "Common.h"
#include "DatabaseEnv.h"
#include "DBCStructure.h"
#include "World.h"

class Item;
class Player;
class WorldPacket;

enum BMMailAuctionAnswers
{
    BM_AUCTION_OUTBIDDED = 0,
    BM_AUCTION_WON = 1,
};

#define BLACKMARKET_AUCTION_HOUSE 7

struct BlackMarketAuctionTemplate
{
    uint32 Id;
    uint32 MarketId;
    uint32 SellerNPCEntry;
    uint32 ItemEntry;
    uint32 Quantity;
    uint32 MinBid;
    uint32 Duration;
    float Chance;
};
typedef struct BlackMarketAuctionTemplate BlackMarketAuctionTemplate;

struct BlackMarketAuction
{


public:
    BlackMarketAuction(uint32 auctionId, uint32 templateId, uint32 startTime, uint32 currentBidder, uint32 currentBid,
        uint32 minIncrement, uint32 numBids) :
        _auctionId(auctionId), _templateId(templateId), _startTime(startTime), _currentBidder(currentBidder), _currentBid(currentBid),
        _minIncrement(minIncrement), _numBids(numBids) { }

    BlackMarketAuction() : _auctionId(1), _templateId(1), _startTime(0), _currentBidder(0), _currentBid(0),
        _minIncrement(0), _numBids(0) { }

    void SetAuctionId(uint32 auctionId) { _auctionId = auctionId; }
    uint32 GetAuctionId() const { return _auctionId; }

    void SetTemplateId(uint32 templateId) { _templateId = templateId; }
    uint32 GetTemplateId() const { return _templateId; }

    void SetStartTime(uint32 startTime) { _startTime = startTime; }
    uint32 GetStartTime() const { return _startTime; }

    void SetCurrentBidder(uint32 currentBidder) { _currentBidder = currentBidder; }
    uint32 GetCurrentBidder() const { return _currentBidder; }

    void SetCurrentBid(uint32 currentBid) { _currentBid = currentBid; }
    uint32 GetCurrentBid() const { return _currentBid; }

    void SetMinIncrement(uint32 minIncrement) { _minIncrement = minIncrement; }
    uint32 GetMinIncrement() const { return _minIncrement; }

    void SetNumBids(uint32 numBids) { _numBids = numBids; }
    uint32 GetNumBids() const { return _numBids; }
    
    BlackMarketAuctionTemplate* GetTemplate() const;

    void DeleteFromDB(SQLTransaction& trans);
    void SaveToDB(SQLTransaction& trans);
    bool LoadFromDB(Field* fields);
    void UpdateToDB(SQLTransaction& trans);

    uint32 GetEndTime() { return GetStartTime() + GetTemplate()->Duration; }
    uint32 TimeLeft();
    bool IsActive() { return (time(NULL) >= GetStartTime()); }
    bool IsExpired() { return GetEndTime() < time(NULL); }

    std::string BuildAuctionMailSubject(BMMailAuctionAnswers response);
    std::string BuildAuctionMailBody(uint32 lowGuid);

private:
    uint32 _auctionId;
    uint32 _templateId;
    uint32 _startTime;
    uint32 _currentBidder;
    uint32 _currentBid;
    uint32 _minIncrement;
    uint32 _numBids;
    BlackMarketAuctionTemplate* _template;
};
typedef struct BlackMarketAuction BlackMarketAuction;
typedef std::map<uint32, BlackMarketAuctionTemplate*> BMAuctionTemplateStore;
typedef std::map<uint32, BlackMarketAuction*> BMAuctionStore;

class BlackMarketMgr
{
    friend class ACE_Singleton<BlackMarketMgr, ACE_Null_Mutex>;

private:
    BlackMarketMgr();
    ~BlackMarketMgr();

    BMAuctionTemplateStore _templates;
    BMAuctionStore _auctions;

public:
    void Update();

    void LoadBlackMarketTemplates();
    void LoadBlackMarketAuctions();

    BlackMarketAuctionTemplate* GetTemplate(uint32 templateId) const;
    uint32 GetTemplatesCount() { return _templates.size(); }
    BMAuctionTemplateStore::iterator GetTemplatesBegin() { return _templates.begin(); }
    BMAuctionTemplateStore::iterator GetTemplatesEnd() { return _templates.end(); }

    BlackMarketAuction* GetAuction(uint32 auctionId) const;
    uint32 GetAuctionCount() { return _auctions.size(); }
    BMAuctionStore::iterator GetAuctionsBegin() { return _auctions.begin(); }
    BMAuctionStore::iterator GetAuctionsEnd() { return _auctions.end(); }

    uint32 GetFreeAuctionId();

    void CreateAuctions(uint32 number, SQLTransaction& trans);
    void UpdateAuction(BlackMarketAuction* auction, uint64 newPrice, uint64 requiredIncrement, Player* newBidder);

    void BuildBlackMarketRequestItemsResult(WorldPacket& data, uint32 guidLow);

    void SendAuctionWon(BlackMarketAuction* auction, SQLTransaction& trans);
    void SendAuctionOutbidded(BlackMarketAuction* auction, uint32 newPrice, Player* newBidder, SQLTransaction& trans);
    
    bool isBlackMarketOpen() { return sWorld->getBoolConfig(CONFIG_BLACK_MARKET_OPEN); }
};

#define sBlackMarketMgr ACE_Singleton<BlackMarketMgr, ACE_Null_Mutex>::instance()

#endif
