/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*
* mod-prabowow: auto-sell grey (poor quality) loot.
*
* Uses the PlayerScript::OnLootItem hook (Player::StoreLootItem). Only the
* count that was just looted is sold, so grey items the player deliberately
* keeps in the bags are untouched. Greys with no vendor value stay in the bag.
*/

#include "PraboWoWConfig.h"

#include "Chat.h"
#include "Item.h"
#include "ItemPrototype.h"
#include "Log.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SharedDefines.h"

#include <algorithm>
#include <string>

class prabowow_auto_sell_grey : public PlayerScript
{
public:
    prabowow_auto_sell_grey() : PlayerScript("prabowow_auto_sell_grey") { }

    void OnLootItem(Player* player, Item* item, uint32 count, uint64 /*lootGuid*/) override
    {
        if (!item || !count)
            return;

        if (!PraboWoW::GetBool("PraboWoW.AutoSellGrey.Enable", true))
            return;

        ItemTemplate const* proto = item->GetTemplate();
        if (!proto || proto->Quality != ITEM_QUALITY_POOR || !proto->SellPrice)
            return;

        uint32 sellCount = std::min(count, item->GetCount());
        if (!sellCount)
            return;

        uint64 money = uint64(proto->SellPrice) * sellCount;
        if (player->GetMoney() + money > MAX_MONEY_AMOUNT)
            return; // keep the item rather than lose it to the gold cap

        std::string name = proto->Name1;

        // DestroyItemCount may free the Item once the stack hits zero, so nothing
        // below touches `item` again.
        uint32 remaining = sellCount;
        player->DestroyItemCount(item, remaining, true);
        player->ModifyMoney(int64(money));

        ChatHandler(player->GetSession()).PSendSysMessage("|cff9d9d9d[%s]|r x%u sold automatically for %s.",
            name.c_str(), sellCount, PraboWoW::FormatMoney(money).c_str());
    }
};

void AddSC_prabowow_auto_sell()
{
    new prabowow_auto_sell_grey();
}
