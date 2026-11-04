/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*
* Hooks a module installs to scale items per instance through the 5.4 item upgrade
* modifier (mod-destiny-weaver). All empty by default: items then use their own template
* and no extra ItemUpgrade rows are announced to clients.
*/

#ifndef SF_ITEM_SCALING_HOOKS_H
#define SF_ITEM_SCALING_HOOKS_H

#include "Define.h"

#include <atomic>
#include <vector>

class ByteBuffer;
class Item;
class Player;
struct ItemTemplate;

namespace ItemScalingHooks
{
    // The template an item's stats, armor and damage are applied from (its upgrade).
    using StatTemplateFn = ItemTemplate const*(*)(Item const* item);
    // Called at the end of Item::LoadFromDB: restore what the module keeps per item.
    using ItemLoadedFn = void(*)(Item* item);
    // ItemUpgrade.db2 rows to announce as hotfixes at login: (row id, timestamp).
    using UpgradeHotfixListFn = void(*)(std::vector<std::pair<uint32, uint32>>& rows);
    // Writes one requested ItemUpgrade row; false = not a module row (no reply).
    using UpgradeRowWriterFn = bool(*)(uint32 upgradeId, ByteBuffer& record, uint32& timestamp);
    // What an item would become if this player looted it now (for decisions such as bots'
    // loot rolls); nullptr = it would stay as it is.
    using LootPreviewFn = ItemTemplate const*(*)(Player* player, ItemTemplate const* proto);

    inline std::atomic<StatTemplateFn> StatTemplate{ nullptr };
    inline std::atomic<LootPreviewFn> LootPreview{ nullptr };
    inline std::atomic<ItemLoadedFn> ItemLoaded{ nullptr };
    inline std::atomic<UpgradeHotfixListFn> UpgradeHotfixList{ nullptr };
    inline std::atomic<UpgradeRowWriterFn> UpgradeRowWriter{ nullptr };
}

#endif
