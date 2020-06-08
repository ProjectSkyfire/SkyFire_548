/*
 * Copyright (C) 2011-2020 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2020 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2020 MaNGOS <https://www.getmangos.eu/>
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
#include "WorldPacket.h"
#include "WorldSession.h"
#include "World.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Log.h"
#include "Opcodes.h"
#include "Player.h"
#include <list>
#include <vector>
#include <utility>

void WorldSession::SendVoidStorageTransferResult(VoidTransferError result)
{
    WorldPacket data(SMSG_VOID_TRANSFER_RESULT, 4);
    data << uint32(result);
    SendPacket(&data);
}

void WorldSession::HandleVoidStorageUnlock(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_VOID_STORAGE_UNLOCK");
    Player* player = GetPlayer();

    ObjectGuid npcGuid;

    recvData.ReadGuidMask(npcGuid, 3, 1, 5, 6, 4, 0, 7, 2);
    recvData.ReadGuidBytes(npcGuid, 4, 3, 6, 2, 1, 5, 7, 0);

    Creature* unit = player->GetNPCIfCanInteractWith(npcGuid, UNIT_NPC_FLAG_VAULTKEEPER);
    if (!unit)
    {
        SF_LOG_DEBUG("network", "WORLD: HandleVoidStorageUnlock - Unit (GUID: %u) not found or player can't interact with it.", GUID_LOPART(npcGuid));
        return;
    }

    if (player->IsVoidStorageUnlocked())
    {
        SF_LOG_DEBUG("network", "WORLD: HandleVoidStorageUnlock - Player (GUID: %u, name: %s) tried to unlock void storage a 2nd time.", player->GetGUIDLow(), player->GetName().c_str());
        return;
    }

    player->ModifyMoney(-int64(VOID_STORAGE_UNLOCK));
    player->UnlockVoidStorage();
}

void WorldSession::HandleVoidStorageQuery(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_VOID_STORAGE_QUERY");
    Player* player = GetPlayer();

    ObjectGuid npcGuid;
    recvData.ReadGuidMask(npcGuid, 1, 5, 6, 0, 7, 2, 3, 4);
    recvData.ReadGuidBytes(npcGuid, 1, 6, 4, 3, 7, 0, 2, 5);

    Creature* unit = player->GetNPCIfCanInteractWith(npcGuid, UNIT_NPC_FLAG_VAULTKEEPER);
    if (!unit)
    {
        SF_LOG_DEBUG("network", "WORLD: HandleVoidStorageQuery - Unit (GUID: %u) not found or player can't interact with it.", GUID_LOPART(npcGuid));
        return;
    }

    if (!player->IsVoidStorageUnlocked())
    {
        SF_LOG_DEBUG("network", "WORLD: HandleVoidStorageQuery - Player (GUID: %u, name: %s) queried void storage without unlocking it.", player->GetGUIDLow(), player->GetName().c_str());
        return;
    }

    uint8 count = 0;
    for (uint8 i = 0; i < VOID_STORAGE_MAX_SLOT; ++i)
        if (player->GetVoidStorageItem(i))
            ++count;

    WorldPacket data(SMSG_VOID_STORAGE_CONTENTS, 2 * count + (14 + 4 + 4 + 4 + 4) * count);

    data.WriteBits(count, 7);

    ByteBuffer itemData((14 + 4 + 4 + 4 + 4) * count);

    for (uint8 i = 0; i < VOID_STORAGE_MAX_SLOT; ++i)
    {
        VoidStorageItem* item = player->GetVoidStorageItem(i);
        if (!item)
            continue;

        ObjectGuid itemId = item->ItemId;
        ObjectGuid creatorGuid = item->CreatorGuid;

        data.WriteGuidMask(creatorGuid, 1, 3);
        data.WriteGuidMask(itemId, 1);
        data.WriteGuidMask(creatorGuid, 2);
        data.WriteGuidMask(itemId, 2);
        data.WriteGuidMask(creatorGuid, 2, 0);
        data.WriteGuidMask(itemId, 6, 5);
        data.WriteGuidMask(creatorGuid, 2);
        data.WriteGuidMask(itemId, 7, 3, 4, 0);
        data.WriteGuidMask(creatorGuid, 6, 7);

        itemData.WriteGuidBytes(creatorGuid, 4, 7);
        itemData.WriteGuidBytes(itemId, 6);
        itemData.WriteGuidBytes(creatorGuid, 6);
        itemData.WriteGuidBytes(itemId, 2);

        itemData << uint32(item->ItemSuffixFactor); //= 32

        itemData.WriteGuidBytes(itemId, 7, 3);
        itemData.WriteGuidBytes(creatorGuid, 0);

        itemData << uint32(0); //= 16 - UpgradeID

        itemData.WriteGuidBytes(itemId, 0);

        itemData << uint32(item->ItemRandomPropertyId); //= 24

        itemData.WriteGuidBytes(creatorGuid, 2, 5, 3);

        itemData << uint32(item->ItemEntry); //= 28

        itemData.WriteGuidBytes(itemId, 5, 1);

        itemData << uint32(i); //= 20

        itemData.WriteGuidBytes(itemId, 4);
        itemData.WriteGuidBytes(creatorGuid, 1);
    }

    data.FlushBits();
    data.append(itemData);

    SendPacket(&data);
}

void WorldSession::HandleVoidStorageTransfer(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_VOID_STORAGE_TRANSFER");
    Player* player = GetPlayer();

    ObjectGuid npcGuid;
    recvData.ReadGuidMask(npcGuid, 7, 4);

    uint32 countDeposit = recvData.ReadBits(24); //40

    if (countDeposit > 9)
    {
        SF_LOG_DEBUG("network", "WORLD: HandleVoidStorageTransfer - Player (GUID: %u, name: %s) wants to deposit more than 9 items (%u).", player->GetGUIDLow(), player->GetName().c_str(), countDeposit);
        recvData.rfinish();
        return;
    }

    std::vector<ObjectGuid> itemGuids(countDeposit);
    for (uint32 i = 0; i < countDeposit; ++i)
    {
        recvData.ReadGuidMask(itemGuids[i], 0, 3, 6, 5, 4, 2, 1, 7);
    }

    uint32 countWithdraw = recvData.ReadBits(24);

    if (countWithdraw > 9)
    {
        SF_LOG_DEBUG("network", "WORLD: HandleVoidStorageTransfer - Player (GUID: %u, name: %s) wants to withdraw more than 9 items (%u).", player->GetGUIDLow(), player->GetName().c_str(), countWithdraw);
        recvData.rfinish();
        return;
    }

    std::vector<ObjectGuid> itemIds(countWithdraw);
    for (uint32 i = 0; i < countWithdraw; ++i)
    {
        recvData.ReadGuidMask(itemIds[i], 4, 0, 5, 7, 6, 1, 2, 3);
    }

    recvData.ReadGuidMask(npcGuid, 6, 0, 3, 1, 2, 5);

    for (uint32 i = 0; i < countDeposit; ++i)
    {
        recvData.ReadGuidBytes(itemGuids[i], 5, 6, 3, 4, 1, 7, 2, 0);
    }

    recvData.ReadGuidBytes(npcGuid, 5);

    for (uint32 i = 0; i < countWithdraw; ++i)
    {
        recvData.ReadGuidBytes(itemIds[i], 0, 4, 1, 2, 6, 3, 7, 5);
    }

    recvData.ReadGuidBytes(npcGuid, 1, 7, 4, 3, 2, 0, 6);

    Creature* unit = player->GetNPCIfCanInteractWith(npcGuid, UNIT_NPC_FLAG_VAULTKEEPER);
    if (!unit)
    {
        SF_LOG_DEBUG("network", "WORLD: HandleVoidStorageTransfer - Unit (GUID: %u) not found or player can't interact with it.", GUID_LOPART(npcGuid));
        return;
    }

    if (!player->IsVoidStorageUnlocked())
    {
        SF_LOG_DEBUG("network", "WORLD: HandleVoidStorageTransfer - Player (GUID: %u, name: %s) queried void storage without unlocking it.", player->GetGUIDLow(), player->GetName().c_str());
        return;
    }

    if (itemGuids.size() > player->GetNumOfVoidStorageFreeSlots())
    {
        SendVoidStorageTransferResult(VoidTransferError::VOID_TRANSFER_ERROR_FULL);
        return;
    }

    uint32 freeBagSlots = 0;
    if (itemIds.size() != 0)
    {
        // make this a Player function
        for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)
            if (Bag* bag = player->GetBagByPos(i))
                freeBagSlots += bag->GetFreeSlots();
        for (uint8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; i++)
            if (!player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                ++freeBagSlots;
    }

    if (itemIds.size() > freeBagSlots)
    {
        SendVoidStorageTransferResult(VoidTransferError::VOID_TRANSFER_ERROR_INVENTORY_FULL);
        return;
    }

    if (!player->HasEnoughMoney(uint64(itemGuids.size() * VOID_STORAGE_STORE_ITEM)))
    {
        SendVoidStorageTransferResult(VoidTransferError::VOID_TRANSFER_ERROR_NOT_ENOUGH_MONEY);
        return;
    }

    std::pair<VoidStorageItem, uint8> depositItems[VOID_STORAGE_MAX_DEPOSIT];
    uint8 depositCount = 0;
    for (std::vector<ObjectGuid>::iterator itr = itemGuids.begin(); itr != itemGuids.end(); ++itr)
    {
        Item* item = player->GetItemByGuid(*itr);
        if (!item)
        {
            SF_LOG_DEBUG("network", "WORLD: HandleVoidStorageTransfer - Player (GUID: %u, name: %s) wants to deposit an invalid item (item guid: " UI64FMTD ").", player->GetGUIDLow(), player->GetName().c_str(), uint64(*itr));
            continue;
        }

        VoidStorageItem itemVS(sObjectMgr->GenerateVoidStorageItemId(), item->GetEntry(), item->GetUInt64Value(ITEM_FIELD_CREATOR), item->GetItemRandomPropertyId(), item->GetItemSuffixFactor());

        uint8 slot = player->AddVoidStorageItem(itemVS);

        depositItems[depositCount++] = std::make_pair(itemVS, slot);

        player->DestroyItem(item->GetBagSlot(), item->GetSlot(), true);
    }

    int64 cost = depositCount * VOID_STORAGE_STORE_ITEM;

    player->ModifyMoney(-cost);

    VoidStorageItem withdrawItems[VOID_STORAGE_MAX_WITHDRAW];
    uint8 withdrawCount = 0;
    for (std::vector<ObjectGuid>::iterator itr = itemIds.begin(); itr != itemIds.end(); ++itr)
    {
        uint8 slot;
        VoidStorageItem* itemVS = player->GetVoidStorageItem(*itr, slot);
        if (!itemVS)
        {
            SF_LOG_DEBUG("network", "WORLD: HandleVoidStorageTransfer - Player (GUID: %u, name: %s) tried to withdraw an invalid item (id: " UI64FMTD ")", player->GetGUIDLow(), player->GetName().c_str(), uint64(*itr));
            continue;
        }

        ItemPosCountVec dest;
        InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemVS->ItemEntry, 1);
        if (msg != EQUIP_ERR_OK)
        {
            SendVoidStorageTransferResult(VoidTransferError::VOID_TRANSFER_ERROR_INVENTORY_FULL);
            SF_LOG_DEBUG("network", "WORLD: HandleVoidStorageTransfer - Player (GUID: %u, name: %s) couldn't withdraw item id " UI64FMTD " because inventory was full.", player->GetGUIDLow(), player->GetName().c_str(), uint64(*itr));
            return;
        }

        Item* item = player->StoreNewItem(dest, itemVS->ItemEntry, true, itemVS->ItemRandomPropertyId);
        item->SetUInt64Value(ITEM_FIELD_CREATOR, uint64(itemVS->CreatorGuid));
        item->SetBinding(true);
        player->SendNewItem(item, 1, false, false, false);

        withdrawItems[withdrawCount++] = *itemVS;

        player->DeleteVoidStorageItem(slot);
    }

    WorldPacket data(SMSG_VOID_STORAGE_TRANSFER_CHANGES, ((5 + 5 + (7 + 7) * depositCount +
        7 * withdrawCount) / 8) + 7 * withdrawCount + (7 + 7 + 4 * 4) * depositCount);

    data.WriteBits(withdrawCount, 4);

    for (uint8 i = 0; i < withdrawCount; ++i)
    {
        ObjectGuid itemId = withdrawItems[i].ItemId;
        data.WriteGuidMask(itemId, 1, 6, 7, 3, 2, 0, 4, 5);
    }

    data.WriteBits(depositCount, 4);

    for (uint8 i = 0; i < depositCount; ++i)
    {
        ObjectGuid itemId = depositItems[i].first.ItemId;
        ObjectGuid creatorGuid = depositItems[i].first.CreatorGuid;
        data.WriteGuidMask(itemId, 0);
        data.WriteGuidMask(creatorGuid, 6, 4);
        data.WriteGuidMask(itemId, 3);
        data.WriteGuidMask(creatorGuid, 3);
        data.WriteGuidMask(itemId, 5, 7);
        data.WriteGuidMask(creatorGuid, 0, 5, 7);
        data.WriteGuidMask(itemId, 6, 4);
        data.WriteGuidMask(creatorGuid, 1);
        data.WriteGuidMask(itemId, 1);
        data.WriteGuidMask(creatorGuid, 2);
    }

    data.FlushBits();

    for (uint8 i = 0; i < depositCount; ++i)
    {
        ObjectGuid itemId = depositItems[i].first.ItemId;
        ObjectGuid creatorGuid = depositItems[i].first.CreatorGuid;
        data << uint32(depositItems[i].second); // slot
        data.WriteGuidBytes(creatorGuid, 5);
        data << uint32(depositItems[i].first.ItemEntry);
        data.WriteGuidBytes(creatorGuid, 6, 3);
        data << uint32(depositItems[i].first.ItemSuffixFactor);
        data.WriteGuidBytes(creatorGuid, 2);
        data.WriteGuidBytes(itemId, 5);
        data << uint32(depositItems[i].first.ItemRandomPropertyId);
        data.WriteGuidBytes(itemId, 3);
        data.WriteGuidBytes(creatorGuid, 7, 4, 1);
        data.WriteGuidBytes(itemId, 0, 4, 6);
        data << uint32(0); // Item Upgrade Level
        data.WriteGuidBytes(itemId, 1, 2);
        data.WriteGuidBytes(creatorGuid, 0);
        data.WriteGuidBytes(itemId, 7);
    }

    for (uint8 i = 0; i < withdrawCount; ++i)
    {
        ObjectGuid itemId = withdrawItems[i].ItemId;

        data.WriteGuidBytes(itemId, 7, 3, 1, 5, 4, 0, 6, 2);
    }

    SendPacket(&data);

    SendVoidStorageTransferResult(VoidTransferError::VOID_TRANSFER_ERROR_NO_ERROR);
}

void WorldSession::HandleVoidSwapItem(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_VOID_SWAP_ITEM");

    Player* player = GetPlayer();
    uint32 newSlot;
    ObjectGuid npcGuid;
    ObjectGuid itemId;

    recvData >> newSlot;

    recvData.ReadGuidMask(npcGuid, 6);
    recvData.ReadGuidMask(itemId, 4, 7, 3, 2);
    recvData.ReadGuidMask(npcGuid, 4, 2);
    recvData.ReadGuidMask(itemId, 0, 1);
    recvData.ReadGuidMask(npcGuid, 7, 1);
    recvData.ReadGuidMask(itemId, 6);
    recvData.ReadGuidMask(npcGuid, 3, 5);
    recvData.ReadGuidMask(itemId, 5);
    recvData.ReadGuidMask(npcGuid, 0);

    recvData.ReadGuidBytes(npcGuid, 3, 5);
    recvData.ReadGuidBytes(itemId, 6);
    recvData.ReadGuidBytes(npcGuid, 4);
    recvData.ReadGuidBytes(itemId, 4);
    recvData.ReadGuidBytes(npcGuid, 0);
    recvData.ReadGuidBytes(itemId, 5, 7);
    recvData.ReadGuidBytes(npcGuid, 7, 2, 1);
    recvData.ReadGuidBytes(itemId, 1, 3);
    recvData.ReadGuidBytes(npcGuid, 6);
    recvData.ReadGuidBytes(itemId, 0, 2);

    Creature* unit = player->GetNPCIfCanInteractWith(npcGuid, UNIT_NPC_FLAG_VAULTKEEPER);
    if (!unit)
    {
        SF_LOG_DEBUG("network", "WORLD: HandleVoidSwapItem - Unit (GUID: %u) not found or player can't interact with it.", GUID_LOPART(npcGuid));
        return;
    }

    if (!player->IsVoidStorageUnlocked())
    {
        SF_LOG_DEBUG("network", "WORLD: HandleVoidSwapItem - Player (GUID: %u, name: %s) queried void storage without unlocking it.", player->GetGUIDLow(), player->GetName().c_str());
        return;
    }

    uint8 oldSlot;
    if (!player->GetVoidStorageItem(itemId, oldSlot))
    {
        SF_LOG_DEBUG("network", "WORLD: HandleVoidSwapItem - Player (GUID: %u, name: %s) requested swapping an invalid item (slot: %u, itemid: " UI64FMTD ").", player->GetGUIDLow(), player->GetName().c_str(), newSlot, uint64(itemId));
        return;
    }

    bool usedSrcSlot = player->GetVoidStorageItem(oldSlot) != NULL; // should be always true
    bool usedDestSlot = player->GetVoidStorageItem(newSlot) != NULL;

    ObjectGuid itemIdDest;
    if (usedDestSlot)
        itemIdDest = player->GetVoidStorageItem(newSlot)->ItemId;

    if (!player->SwapVoidStorageItem(oldSlot, newSlot))
    {
        SendVoidStorageTransferResult(VoidTransferError::VOID_TRANSFER_ERROR_INTERNAL_ERROR_1);
        return;
    }

    WorldPacket data(SMSG_VOID_ITEM_SWAP_RESPONSE, 1 + (usedSrcSlot + usedDestSlot) * (1 + 7 + 4));

    data.WriteBit(!usedSrcSlot);

    data.WriteGuidMask(itemId, 4, 1, 6, 0, 3, 7, 2, 5);

    data.WriteBit(!usedDestSlot);

    data.WriteGuidMask(itemIdDest, 6, 0, 3, 2, 1, 5, 7, 4);

    data.WriteBit(!false); //VoidItemSlotA
    data.WriteBit(!usedDestSlot);

    data.FlushBits();

    data.WriteGuidBytes(itemIdDest, 3, 7, 2, 5, 0, 1, 4, 6);
    data.WriteGuidBytes(itemId, 0, 2, 7, 5, 6, 4, 3, 1);

    //if (VoidItemSlotA)
    //    data << uint32(VoidItemSlotA);

    if (usedDestSlot)
        data << uint32(newSlot);

    SendPacket(&data);
}
