/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*
* mod-prabowow: heirloom vendor.
*
* The creature template and its spawns (one per distinct playercreateinfo
* start position) come from sql/updates/world/*prabowow_heirloom_vendor*.sql.
* The vendor list is built here at startup from every ITEM_QUALITY_HEIRLOOM
* item in Item-sparse.db2, so nothing has to be hand-maintained per patch.
*
* Heirlooms are normally bought with Justice Points and carry no gold price;
* PraboWoW.HeirloomVendor.PriceGold overrides BuyPrice so they sell for gold.
*/

#include "PraboWoWConfig.h"

#include "Creature.h"
#include "ItemPrototype.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "SharedDefines.h"
#include "Unit.h"

#include <algorithm>
#include <limits>
#include <vector>

class prabowow_heirloom_vendor : public WorldScript
{
public:
    prabowow_heirloom_vendor() : WorldScript("prabowow_heirloom_vendor") { }

    void OnStartup() override
    {
        if (!PraboWoW::GetBool("PraboWoW.HeirloomVendor.Enable", true))
            return;

        uint32 entry = uint32(std::max<int32>(PraboWoW::GetInt("PraboWoW.HeirloomVendor.Entry", 900001), 1));
        int32 priceGold = PraboWoW::GetInt("PraboWoW.HeirloomVendor.PriceGold", 500);

        CreatureTemplate const* cInfo = sObjectMgr->GetCreatureTemplate(entry);
        if (!cInfo)
        {
            SF_LOG_ERROR(PraboWoW::LOG, "[mod-prabowow] Heirloom vendor creature_template %u does not exist. Apply sql/updates/world/*prabowow_heirloom_vendor*.sql.", entry);
            return;
        }
        if (!(cInfo->npcflag & UNIT_NPC_FLAG_VENDOR))
        {
            SF_LOG_ERROR(PraboWoW::LOG, "[mod-prabowow] Heirloom vendor creature_template %u has no UNIT_NPC_FLAG_VENDOR.", entry);
            return;
        }

        std::vector<uint32> heirlooms;
        for (auto const& pair : *sObjectMgr->GetItemTemplateStore())
        {
            ItemTemplate const& proto = pair.second;
            if (proto.Quality != ITEM_QUALITY_HEIRLOOM || proto.Name1.empty())
                continue;
            heirlooms.push_back(pair.first);
        }
        std::sort(heirlooms.begin(), heirlooms.end());

        uint32 price = 0;
        if (priceGold > 0)
            price = uint32(std::min<uint64>(uint64(priceGold) * GOLD, uint64(std::numeric_limits<int32>::max())));

        uint32 added = 0;
        uint32 skipped = 0;
        for (uint32 itemId : heirlooms)
        {
            if (price)
            {
                // ItemTemplates live in ObjectMgr's own (non-const) container; the
                // accessor is const-only, hence the cast. Done before players log in.
                if (ItemTemplate const* proto = sObjectMgr->GetItemTemplate(itemId))
                    const_cast<ItemTemplate*>(proto)->BuyPrice = int32(price);
            }

            VendorItemData const* list = sObjectMgr->GetNpcVendorItemList(entry);
            if (list && list->FindItemCostPair(itemId, 0, ITEM_VENDOR_TYPE_ITEM))
                continue; // already listed through npc_vendor

            if (list && list->GetItemCount() >= MAX_VENDOR_ITEMS)
            {
                ++skipped;
                continue;
            }

            if (!sObjectMgr->IsVendorItemValid(entry, itemId, 0, 0, 0, ITEM_VENDOR_TYPE_ITEM))
            {
                ++skipped;
                continue;
            }

            sObjectMgr->AddVendorItem(entry, itemId, 0, 0, 0, ITEM_VENDOR_TYPE_ITEM, false);
            ++added;
        }

        SF_LOG_INFO(PraboWoW::LOG, "[mod-prabowow] Heirloom vendor %u: %u heirloom(s) found in Item-sparse.db2, %u registered, %u skipped, price %s each.",
            entry, uint32(heirlooms.size()), added, skipped, price ? PraboWoW::FormatMoney(price).c_str() : "from DB2 (unchanged)");
    }
};

void AddSC_prabowow_heirloom_vendor()
{
    new prabowow_heirloom_vendor();
}
