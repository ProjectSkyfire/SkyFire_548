/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
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

enum class BMMailAuctionAnswers
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
    BlackMarketAuction(uint32 auctionId, uint32 templateId, uint32 startTime, uint32 currentBidder, uint64 currentBid,
        uint64 minIncrement, uint32 numBids) :
        _auctionId(auctionId), _templateId(templateId), _startTime(startTime), _currentBidder(currentBidder), _currentBid(currentBid),
        _minIncrement(minIncrement), _numBids(numBids), _template(NULL) { }

    BlackMarketAuction() : _auctionId(1), _templateId(1), _startTime(0), _currentBidder(0), _currentBid(0),
        _minIncrement(0), _numBids(0), _template(NULL) { }

    void SetAuctionId(uint32 auctionId) { _auctionId = auctionId; }
    uint32 GetAuctionId() const { return _auctionId; }

    void SetTemplateId(uint32 templateId) { _templateId = templateId; }
    uint32 GetTemplateId() const { return _templateId; }

    void SetStartTime(uint32 startTime) { _startTime = startTime; }
    uint32 GetStartTime() const { return _startTime; }

    void SetCurrentBidder(uint32 currentBidder) { _currentBidder = currentBidder; }
    uint32 GetCurrentBidder() const { return _currentBidder; }

    void SetCurrentBid(uint64 currentBid) { _currentBid = currentBid; }
    uint64 GetCurrentBid() const { return _currentBid; }

    void SetMinIncrement(uint64 minIncrement) { _minIncrement = minIncrement; }
    uint64 GetMinIncrement() const { return _minIncrement; }

    void SetNumBids(uint32 numBids) { _numBids = numBids; }
    uint32 GetNumBids() const { return _numBids; }
    
    BlackMarketAuctionTemplate* GetTemplate() const;

    void DeleteFromDB(SQLTransaction& trans);
    void SaveToDB(SQLTransaction& trans);
    bool LoadFromDB(Field* fields);
    void UpdateToDB(SQLTransaction& trans);

    uint32 GetEndTime() const { return GetStartTime() + GetTemplate()->Duration; }
    uint32 TimeLeft();
    bool IsActive() const { return (time(NULL) >= GetStartTime()); }
    bool IsExpired() const { return GetEndTime() < time(NULL); }

    std::string BuildAuctionMailSubject(BMMailAuctionAnswers response);
    std::string BuildAuctionMailBody(uint32 lowGuid);

private:
    uint32 _auctionId;
    uint32 _templateId;
    uint32 _startTime;
    uint32 _currentBidder;
    uint64 _currentBid;
    uint64 _minIncrement;
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
    void SendAuctionOutbidded(BlackMarketAuction* auction, SQLTransaction& trans);
    
    bool isBlackMarketOpen() { return sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_BLACK_MARKET_OPEN); }
};

#define sBlackMarketMgr ACE_Singleton<BlackMarketMgr, ACE_Null_Mutex>::instance()

#endif
