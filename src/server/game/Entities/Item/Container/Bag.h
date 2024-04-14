/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_BAG_H
#define SKYFIRE_BAG_H

// Maximum 36 Slots ((CONTAINER_END - CONTAINER_FIELD_SLOTS)/2
#define MAX_BAG_SIZE 36                                     // 2.0.12

#include "Item.h"
#include "ItemPrototype.h"

class Bag : public Item
{
public:
    Bag();
    ~Bag();

    void AddToWorld() OVERRIDE;
    void RemoveFromWorld() OVERRIDE;

    bool Create(uint32 guidlow, uint32 itemid, Player const* owner) OVERRIDE;

    void StoreItem(uint8 slot, Item* pItem, bool update);
    void RemoveItem(uint8 slot, bool update);

    Item* GetItemByPos(uint8 slot) const;
    uint32 GetItemCount(uint32 item, Item* eItem = NULL) const;
    uint32 GetItemCountWithLimitCategory(uint32 limitCategory, Item* skipItem = NULL) const;

    uint8 GetSlotByItemGUID(uint64 guid) const;
    bool IsEmpty() const;
    uint32 GetFreeSlots() const;
    uint32 GetBagSize() const { return GetUInt32Value(CONTAINER_FIELD_NUM_SLOTS); }

    // DB operations
    // overwrite virtual Item::SaveToDB
    void SaveToDB(SQLTransaction& trans) OVERRIDE;
    // overwrite virtual Item::LoadFromDB
    bool LoadFromDB(uint32 guid, uint64 owner_guid, Field* fields, uint32 entry) OVERRIDE;
    // overwrite virtual Item::DeleteFromDB
    void DeleteFromDB(SQLTransaction& trans) OVERRIDE;

    void BuildCreateUpdateBlockForPlayer(UpdateData* data, Player* target) const OVERRIDE;

protected:
    // Bag Storage space
    Item* m_bagslot[MAX_BAG_SIZE];
};

inline Item* NewItemOrBag(ItemTemplate const* proto)
{
    return (proto->InventoryType == INVTYPE_BAG) ? new Bag : new Item;
}
#endif
