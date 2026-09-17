/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Player.h"
#include "Item.h"
#include "WorldSession.h"

void Player::TradeCancel(bool sendback, TradeStatus status /*= TRADE_STATUS_CANCELLED*/)
{
    if (m_trade)
    {
        Player* trader = m_trade->GetTrader();

        // send yellow "Trade canceled" message to both traders
        if (sendback)
            GetSession()->SendCancelTrade(status);

        trader->GetSession()->SendCancelTrade(status);

        // cleanup
        delete m_trade;
        m_trade = NULL;
        delete trader->m_trade;
        trader->m_trade = NULL;
    }
}

void Player::UpdateSoulboundTradeItems()
{
    if (m_itemSoulboundTradeable.empty())
        return;

    // The list is keyed on GUID rather than Item*, because an item can leave the player's
    // possession through a path that never calls RemoveTradeableItem -- and the resulting stale
    // pointer used to crash here, in Player::Update, reading ITEM_FIELD_OWNER off freed memory.
    // Resolving the GUID against the current inventory turns that into a miss we can drop.
    for (ItemGuidSet::iterator itr = m_itemSoulboundTradeable.begin(); itr != m_itemSoulboundTradeable.end();)
    {
        Item* item = GetItemByGuid(*itr);
        if (!item || item->GetOwnerGUID() != GetGUID() || item->CheckSoulboundTradeExpire())
        {
            m_itemSoulboundTradeable.erase(itr++);
            continue;
        }
        ++itr;
    }
}

void Player::AddTradeableItem(Item* item)
{
    if (item)
        m_itemSoulboundTradeable.insert(item->GetGUID());
}

void Player::RemoveTradeableItem(Item* item)
{
    if (item)
        m_itemSoulboundTradeable.erase(item->GetGUID());
}
