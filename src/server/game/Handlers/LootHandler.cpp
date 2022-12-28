/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "Common.h"
#include "Log.h"
#include "Corpse.h"
#include "Creature.h"
#include "GameObject.h"
#include "Group.h"
#include "GuildMgr.h"
#include "LootMgr.h"
#include "ObjectAccessor.h"
#include "Object.h"
#include "Opcodes.h"
#include "Player.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"

void WorldSession::HandleAutostoreLootItemOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_AUTOSTORE_LOOT_ITEM");
    Player* player = GetPlayer();
    Loot* loot = NULL;

    int32 lootCount = recvData.ReadBits(23);
    ObjectGuid* guids = new ObjectGuid[lootCount];

    for (int i = 0; i < lootCount; i++)
    {
        (guids[i])[2] = recvData.ReadBit();
        (guids[i])[7] = recvData.ReadBit();
        (guids[i])[0] = recvData.ReadBit();
        (guids[i])[6] = recvData.ReadBit();
        (guids[i])[5] = recvData.ReadBit();
        (guids[i])[3] = recvData.ReadBit();
        (guids[i])[1] = recvData.ReadBit();
        (guids[i])[4] = recvData.ReadBit();
    }

    for (int i = 0; i < lootCount; i++)
    {
        recvData.ReadByteSeq((guids[i])[0]);
        recvData.ReadByteSeq((guids[i])[4]);
        recvData.ReadByteSeq((guids[i])[1]);
        recvData.ReadByteSeq((guids[i])[7]);
        recvData.ReadByteSeq((guids[i])[6]);
        recvData.ReadByteSeq((guids[i])[5]);
        recvData.ReadByteSeq((guids[i])[3]);
        recvData.ReadByteSeq((guids[i])[2]);
        uint8 lootSlot;
        recvData >> lootSlot;

        uint64 guid = guids[i];

        if (IS_GAMEOBJECT_GUID(guid))
        {
            GameObject* go = player->GetMap()->GetGameObject(guid);

            // not check distance for GO in case owned GO (fishing bobber case, for example) or Fishing hole GO
            if (!go || ((go->GetOwnerGUID() != _player->GetGUID() && go->GetGoType() != GAMEOBJECT_TYPE_FISHINGHOLE) && !go->IsWithinDistInMap(_player, INTERACTION_DISTANCE)))
            {
                player->SendLootRelease(guid);
                break;
            }

            loot = &go->loot;
        }
        else if (IS_ITEM_GUID(guid))
        {
            Item* pItem = player->GetItemByGuid(guid);

            if (!pItem)
            {
                player->SendLootRelease(guid);
                break;
            }

            loot = &pItem->loot;
        }
        else if (IS_CORPSE_GUID(guid))
        {
            Corpse* bones = ObjectAccessor::GetCorpse(*player, guid);
            if (!bones)
            {
                player->SendLootRelease(guid);
                break;
            }

            loot = &bones->loot;
        }
        else
        {
            Creature* creature = GetPlayer()->GetMap()->GetCreature(guid);

            bool lootAllowed = creature && creature->IsAlive() == (player->getClass() == CLASS_ROGUE && creature->lootForPickPocketed);

            if (!lootAllowed || !creature->IsWithinDistInMap(_player, INTERACTION_DISTANCE))
            {
                player->SendLootRelease(guid);
                break;
            }

            loot = &creature->loot;
        }

        if (!loot->HasLooter(_player->GetGUID()))
            break;

        player->StoreLootItem(lootSlot, loot);

        // If player is removing the last LootItem, delete the empty container.
        if (loot->isLooted() && IS_ITEM_GUID(guid))
            player->GetSession()->DoLootRelease(guid);
    }
    delete [] guids;
}

void WorldSession::HandleLootMoneyOpcode(WorldPacket& /*recvData*/)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_LOOT_MONEY");

    Player* player = GetPlayer();
    uint64 guid = player->GetLootGUID();
    if (!guid)
        return;

    Loot* loot = NULL;
    bool shareMoney = true;

    switch (GUID_HIPART(guid))
    {
        case HIGHGUID_GAMEOBJECT:
        {
            GameObject* go = GetPlayer()->GetMap()->GetGameObject(guid);

            // do not check distance for GO if player is the owner of it (ex. fishing bobber)
            if (go && ((go->GetOwnerGUID() == player->GetGUID() || go->IsWithinDistInMap(player, INTERACTION_DISTANCE))))
                loot = &go->loot;

            break;
        }
        case HIGHGUID_CORPSE:                               // remove insignia ONLY in BG
        {
            Corpse* bones = ObjectAccessor::GetCorpse(*player, guid);

            if (bones && bones->IsWithinDistInMap(player, INTERACTION_DISTANCE))
            {
                loot = &bones->loot;
                shareMoney = false;
            }

            break;
        }
        case HIGHGUID_ITEM:
        {
            if (Item* item = player->GetItemByGuid(guid))
            {
                loot = &item->loot;
                shareMoney = false;
            }
            break;
        }
        case HIGHGUID_UNIT:
        case HIGHGUID_VEHICLE:
        {
            Creature* creature = player->GetMap()->GetCreature(guid);
            bool lootAllowed = creature && creature->IsAlive() == (player->getClass() == CLASS_ROGUE && creature->lootForPickPocketed);
            if (lootAllowed && creature->IsWithinDistInMap(player, INTERACTION_DISTANCE))
            {
                loot = &creature->loot;
                if (creature->IsAlive())
                    shareMoney = false;
            }
            break;
        }
        default:
            return;                                         // unlootable type
    }

    if (loot)
    {
        loot->NotifyMoneyRemoved();
        if (shareMoney && player->GetGroup())      //item, pickpocket and players can be looted only single player
        {
            Group* group = player->GetGroup();

            std::vector<Player*> playersNear;
            for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* member = itr->GetSource();
                if (!member)
                    continue;

                if (player->IsWithinDistInMap(member, sWorld->GetFloatConfig(WorldFloatConfigs::CONFIG_GROUP_XP_DISTANCE), false))
                    playersNear.push_back(member);
            }

            uint32 goldPerPlayer = uint32((loot->gold) / (playersNear.size()));

            for (std::vector<Player*>::const_iterator i = playersNear.begin(); i != playersNear.end(); ++i)
            {
                (*i)->ModifyMoney(goldPerPlayer);
                (*i)->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY, goldPerPlayer);

                if (Guild* guild = sGuildMgr->GetGuildById((*i)->GetGuildId()))
                    if (uint32 guildGold = CalculatePct(goldPerPlayer, (*i)->GetTotalAuraModifier(SPELL_AURA_DEPOSIT_BONUS_MONEY_IN_GUILD_BANK_ON_LOOT)))
                        guild->HandleMemberDepositMoney(this, guildGold, true);

                WorldPacket data(SMSG_LOOT_MONEY_NOTIFY, 4 + 1);
                data.WriteBit(playersNear.size() <= 1); // Controls the text displayed in chat. 0 is "Your share is..." and 1 is "You loot..."
                data.FlushBits();
                data << uint32(goldPerPlayer);
                (*i)->GetSession()->SendPacket(&data);
            }
        }
        else
        {
            player->ModifyMoney(loot->gold);
            player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY, loot->gold);

            if (Guild* guild = sGuildMgr->GetGuildById(player->GetGuildId()))
                if (uint32 guildGold = CalculatePct(loot->gold, player->GetTotalAuraModifier(SPELL_AURA_DEPOSIT_BONUS_MONEY_IN_GUILD_BANK_ON_LOOT)))
                    guild->HandleMemberDepositMoney(this, guildGold, true);

            WorldPacket data(SMSG_LOOT_MONEY_NOTIFY, 4 + 1);
            data.WriteBit(1);   // "You loot..."
            data.FlushBits();
            data << uint32(loot->gold);
            SendPacket(&data);
        }

        loot->gold = 0;

        // Delete the money loot record from the DB
        if (loot->containerID > 0)
            loot->DeleteLootMoneyFromContainerItemDB();

        // Delete container if empty
        if (loot->isLooted() && IS_ITEM_GUID(guid))
            player->GetSession()->DoLootRelease(guid);
    }
}

void WorldSession::HandleLootOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_LOOT");

    ObjectGuid guid;

    guid[4] = recvData.ReadBit();
    guid[5] = recvData.ReadBit();
    guid[2] = recvData.ReadBit();
    guid[7] = recvData.ReadBit();
    guid[0] = recvData.ReadBit();
    guid[1] = recvData.ReadBit();
    guid[3] = recvData.ReadBit();
    guid[6] = recvData.ReadBit();

    recvData.ReadByteSeq(guid[3]);
    recvData.ReadByteSeq(guid[5]);
    recvData.ReadByteSeq(guid[0]);
    recvData.ReadByteSeq(guid[6]);
    recvData.ReadByteSeq(guid[4]);
    recvData.ReadByteSeq(guid[1]);
    recvData.ReadByteSeq(guid[7]);
    recvData.ReadByteSeq(guid[2]);

    // Check possible cheat
    if (!_player->IsAlive())
        return;

    GetPlayer()->SendLoot(guid, LootType::LOOT_CORPSE);

    // interrupt cast
    if (GetPlayer()->IsNonMeleeSpellCasted(false))
        GetPlayer()->InterruptNonMeleeSpells(false);
}

void WorldSession::HandleLootReleaseOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_LOOT_RELEASE");

    // cheaters can modify lguid to prevent correct apply loot release code and re-loot
    // use internal stored guid

    ObjectGuid guid;
    guid[7] = recvData.ReadBit();
    guid[4] = recvData.ReadBit();
    guid[2] = recvData.ReadBit();
    guid[3] = recvData.ReadBit();
    guid[0] = recvData.ReadBit();
    guid[5] = recvData.ReadBit();
    guid[6] = recvData.ReadBit();
    guid[1] = recvData.ReadBit();

    recvData.ReadByteSeq(guid[0]);
    recvData.ReadByteSeq(guid[6]);
    recvData.ReadByteSeq(guid[4]);
    recvData.ReadByteSeq(guid[2]);
    recvData.ReadByteSeq(guid[5]);
    recvData.ReadByteSeq(guid[3]);
    recvData.ReadByteSeq(guid[7]);
    recvData.ReadByteSeq(guid[1]);

    if (uint64 lootGuid = GetPlayer()->GetLootGUID())
        if (lootGuid == guid)
            DoLootRelease(lootGuid);
}

void WorldSession::DoLootRelease(uint64 lguid)
{
    Player  *player = GetPlayer();
    Loot    *loot;

    player->SetLootGUID(0);
    player->SendLootRelease(lguid);

    player->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOOTING);

    if (!player->IsInWorld())
        return;

    if (IS_GAMEOBJECT_GUID(lguid))
    {
        GameObject* go = GetPlayer()->GetMap()->GetGameObject(lguid);

        // not check distance for GO in case owned GO (fishing bobber case, for example) or Fishing hole GO
        if (!go || ((go->GetOwnerGUID() != _player->GetGUID() && go->GetGoType() != GAMEOBJECT_TYPE_FISHINGHOLE) && !go->IsWithinDistInMap(_player, INTERACTION_DISTANCE)))
            return;

        loot = &go->loot;

        if (go->GetGoType() == GAMEOBJECT_TYPE_DOOR)
        {
            // locked doors are opened with spelleffect openlock, prevent remove its as looted
            go->UseDoorOrButton();
        }
        else if (loot->isLooted() || go->GetGoType() == GAMEOBJECT_TYPE_FISHINGNODE)
        {
            if (go->GetGoType() == GAMEOBJECT_TYPE_FISHINGHOLE)
            {                                               // The fishing hole used once more
                go->AddUse();                               // if the max usage is reached, will be despawned in next tick
                if (go->GetUseCount() >= go->GetGOValue()->FishingHole.MaxOpens)
                    go->SetLootState(LootState::GO_JUST_DEACTIVATED);
                else
                    go->SetLootState(LootState::GO_READY);
            }
            else
                go->SetLootState(LootState::GO_JUST_DEACTIVATED);

            loot->clear();
        }
        else
        {
            // not fully looted object
            go->SetLootState(LootState::GO_ACTIVATED, player);

            // if the round robin player release, reset it.
            if (player->GetGUID() == loot->roundRobinPlayer)
            {
                if (Group* group = player->GetGroup())
                {
                    if (group->GetLootMethod() != LootMethod::MASTER_LOOT)
                    {
                        loot->roundRobinPlayer = 0;
                    }
                }
                else
                    loot->roundRobinPlayer = 0;
            }
        }
    }
    else if (IS_CORPSE_GUID(lguid))        // ONLY remove insignia at BG
    {
        Corpse* corpse = ObjectAccessor::GetCorpse(*player, lguid);
        if (!corpse || !corpse->IsWithinDistInMap(_player, INTERACTION_DISTANCE))
            return;

        loot = &corpse->loot;

        if (loot->isLooted())
        {
            loot->clear();
            corpse->RemoveFlag(CORPSE_FIELD_DYNAMIC_FLAGS, CORPSE_DYNFLAG_LOOTABLE);
        }
    }
    else if (IS_ITEM_GUID(lguid))
    {
        Item* pItem = player->GetItemByGuid(lguid);
        if (!pItem)
            return;

        ItemTemplate const* proto = pItem->GetTemplate();

        // destroy only 5 items from stack in case prospecting and milling
        if (proto->Flags & (ITEM_PROTO_FLAG_PROSPECTABLE | ITEM_PROTO_FLAG_MILLABLE))
        {
            pItem->m_lootGenerated = false;
            pItem->loot.clear();

            uint32 count = pItem->GetCount();

            // >=5 checked in spell code, but will work for cheating cases also with removing from another stacks.
            if (count > 5)
                count = 5;

            player->DestroyItemCount(pItem, count, true);
        }
        else
        {
            if (pItem->loot.isLooted()) // Only delete item if no loot or money (unlooted loot is saved to db)
                player->DestroyItem(pItem->GetBagSlot(), pItem->GetSlot(), true);
        }
        return;                                             // item can be looted only single player
    }
    else
    {
        Creature* creature = GetPlayer()->GetMap()->GetCreature(lguid);

        bool lootAllowed = creature && creature->IsAlive() == (player->getClass() == CLASS_ROGUE && creature->lootForPickPocketed);
        if (!lootAllowed || !creature->IsWithinDistInMap(_player, INTERACTION_DISTANCE))
            return;

        loot = &creature->loot;
        if (loot->isLooted())
        {
            // skip pickpocketing loot for speed, skinning timer reduction is no-op in fact
            if (!creature->IsAlive())
                creature->AllLootRemovedFromCorpse();

            creature->RemoveFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
            loot->clear();
        }
        else
        {
            // if the round robin player release, reset it.
            if (player->GetGUID() == loot->roundRobinPlayer)
            {
                if (Group* group = player->GetGroup())
                {
                    if (group->GetLootMethod() != LootMethod::MASTER_LOOT)
                    {
                        loot->roundRobinPlayer = 0;
                        group->SendLooter(creature, NULL);

                        // force update of dynamic flags, otherwise other group's players still not able to loot.
                        creature->ForceValuesUpdateAtIndex(OBJECT_FIELD_DYNAMIC_FLAGS);
                    }
                }
                else
                    loot->roundRobinPlayer = 0;
            }
        }
    }

    //Player is not looking at loot list, he doesn't need to see updates on the loot list
    loot->RemoveLooter(player->GetGUID());
}

void WorldSession::HandleLootMasterGiveOpcode(WorldPacket& recvData)
{
    uint8 slotid;
    uint64 lootguid, target_playerguid;

    recvData >> lootguid >> slotid >> target_playerguid;

    if (!_player->GetGroup() || _player->GetGroup()->GetLooterGuid() != _player->GetGUID())
    {
        _player->SendLootRelease(GetPlayer()->GetLootGUID());
        return;
    }

    Player* target = ObjectAccessor::FindPlayer(MAKE_NEW_GUID(target_playerguid, 0, HIGHGUID_PLAYER));
    if (!target)
        return;

    SF_LOG_DEBUG("network", "WorldSession::HandleLootMasterGiveOpcode (CMSG_LOOT_MASTER_GIVE, 0x02A3) Target = [%s].", target->GetName().c_str());

    if (_player->GetLootGUID() != lootguid)
        return;

    Loot* loot = NULL;

    if (IS_CRE_OR_VEH_GUID(GetPlayer()->GetLootGUID()))
    {
        Creature* creature = GetPlayer()->GetMap()->GetCreature(lootguid);
        if (!creature)
            return;

        loot = &creature->loot;
    }
    else if (IS_GAMEOBJECT_GUID(GetPlayer()->GetLootGUID()))
    {
        GameObject* pGO = GetPlayer()->GetMap()->GetGameObject(lootguid);
        if (!pGO)
            return;

        loot = &pGO->loot;
    }

    if (!loot)
        return;

    if (slotid >= loot->items.size() + loot->quest_items.size())
    {
        SF_LOG_DEBUG("loot", "MasterLootItem: Player %s might be using a hack! (slot %d, size %lu)",
            GetPlayer()->GetName().c_str(), slotid, (unsigned long)loot->items.size());
        return;
    }

    LootItem& item = slotid >= loot->items.size() ? loot->quest_items[slotid - loot->items.size()] : loot->items[slotid];

    ItemPosCountVec dest;
    InventoryResult msg = target->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, item.itemid, item.count);
    if (item.follow_loot_rules && !item.AllowedForPlayer(target))
        msg = EQUIP_ERR_CANT_EQUIP_EVER;
    if (msg != EQUIP_ERR_OK)
    {
        target->SendEquipError(msg, NULL, NULL, item.itemid);
        // send duplicate of error massage to master looter
        _player->SendEquipError(msg, NULL, NULL, item.itemid);
        return;
    }

    // list of players allowed to receive this item in trade
    AllowedLooterSet looters = item.GetAllowedLooters();

    // not move item from loot to target inventory
    Item* newitem = target->StoreNewItem(dest, item.itemid, true, item.randomPropertyId, looters);
    target->SendNewItem(newitem, uint32(item.count), false, false, true);
    target->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM, item.itemid, item.count);
    target->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE, item.itemid, item.count, uint64(loot->loot_type));
    target->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LOOT_EPIC_ITEM, item.itemid, item.count);

    // mark as looted
    item.count=0;
    item.is_looted=true;

    loot->NotifyItemRemoved(slotid);
    --loot->unlootedCount;
}
