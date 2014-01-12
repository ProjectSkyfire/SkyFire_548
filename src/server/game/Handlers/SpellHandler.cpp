/*
 * Copyright (C) 2011-2014 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2014 MaNGOS <http://getmangos.com/>
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
#include "DBCStores.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "ObjectMgr.h"
#include "GuildMgr.h"
#include "SpellMgr.h"
#include "Log.h"
#include "Opcodes.h"
#include "Spell.h"
#include "Totem.h"
#include "TemporarySummon.h"
#include "SpellAuras.h"
#include "CreatureAI.h"
#include "ScriptMgr.h"
#include "GameObjectAI.h"
#include "SpellAuraEffects.h"
#include "Player.h"

void WorldSession::HandleClientCastFlags(WorldPacket& recvPacket, uint8 castFlags, SpellCastTargets& targets)
{
    // some spell cast packet including more data (for projectiles?)
    if (castFlags & 0x02)
    {
        // not sure about these two
        float elevation, speed;
        recvPacket >> elevation;
        recvPacket >> speed;

        targets.SetElevation(elevation);
        targets.SetSpeed(speed);

        uint8 hasMovementData;
        recvPacket >> hasMovementData;
        if (hasMovementData)
            HandleMovementOpcodes(recvPacket);
    }
    else if (castFlags & 0x8)   // Archaeology
    {
        uint32 count, entry, usedCount;
        uint8 type;
        recvPacket >> count;
        for (uint32 i = 0; i < count; ++i)
        {
            recvPacket >> type;
            switch (type)
            {
                case 2: // Keystones
                    recvPacket >> entry;        // Item id
                    recvPacket >> usedCount;    // Item count
                    break;
                case 1: // Fragments
                    recvPacket >> entry;        // Currency id
                    recvPacket >> usedCount;    // Currency count
                    break;
            }
        }
    }
}

void WorldSession::HandleUseItemOpcode(WorldPacket& recvPacket)
{
    /// @todo add targets.read() check
    Player* pUser = _player;

    // ignore for remote control state
    if (pUser->m_mover != pUser)
        return;

    uint8 bagIndex, slot, castFlags;
    uint8 castCount;                                       // next cast if exists (single or not)
    uint64 itemGUID;
    uint32 glyphIndex;                                      // something to do with glyphs?
    uint32 spellId;                                         // casted spell id

    recvPacket >> bagIndex >> slot >> castCount >> spellId >> itemGUID >> glyphIndex >> castFlags;

    if (glyphIndex >= MAX_GLYPH_SLOT_INDEX)
    {
        pUser->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, NULL, NULL);
        return;
    }

    Item* pItem = pUser->GetUseableItemByPos(bagIndex, slot);
    if (!pItem)
    {
        pUser->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, NULL, NULL);
        return;
    }

    if (pItem->GetGUID() != itemGUID)
    {
        pUser->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, NULL, NULL);
        return;
    }

    TC_LOG_DEBUG("network", "WORLD: CMSG_USE_ITEM packet, bagIndex: %u, slot: %u, castCount: %u, spellId: %u, Item: %u, glyphIndex: %u, data length = %i", bagIndex, slot, castCount, spellId, pItem->GetEntry(), glyphIndex, (uint32)recvPacket.size());

    ItemTemplate const* proto = pItem->GetTemplate();
    if (!proto)
    {
        pUser->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, pItem, NULL);
        return;
    }

    // some item classes can be used only in equipped state
    if (proto->InventoryType != INVTYPE_NON_EQUIP && !pItem->IsEquipped())
    {
        pUser->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, pItem, NULL);
        return;
    }

    InventoryResult msg = pUser->CanUseItem(pItem);
    if (msg != EQUIP_ERR_OK)
    {
        pUser->SendEquipError(msg, pItem, NULL);
        return;
    }

    // only allow conjured consumable, bandage, poisons (all should have the 2^21 item flag set in DB)
    if (proto->Class == ITEM_CLASS_CONSUMABLE && !(proto->Flags & ITEM_PROTO_FLAG_USEABLE_IN_ARENA) && pUser->InArena())
    {
        pUser->SendEquipError(EQUIP_ERR_NOT_DURING_ARENA_MATCH, pItem, NULL);
        return;
    }

    // don't allow items banned in arena
    if (proto->Flags & ITEM_PROTO_FLAG_NOT_USEABLE_IN_ARENA && pUser->InArena())
    {
        pUser->SendEquipError(EQUIP_ERR_NOT_DURING_ARENA_MATCH, pItem, NULL);
        return;
    }

    if (pUser->IsInCombat())
    {
        for (int i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
        {
            if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(proto->Spells[i].SpellId))
            {
                if (!spellInfo->CanBeUsedInCombat())
                {
                    pUser->SendEquipError(EQUIP_ERR_NOT_IN_COMBAT, pItem, NULL);
                    return;
                }
            }
        }
    }

    // check also  BIND_WHEN_PICKED_UP and BIND_QUEST_ITEM for .additem or .additemset case by GM (not binded at adding to inventory)
    if (pItem->GetTemplate()->Bonding == BIND_WHEN_USE || pItem->GetTemplate()->Bonding == BIND_WHEN_PICKED_UP || pItem->GetTemplate()->Bonding == BIND_QUEST_ITEM)
    {
        if (!pItem->IsSoulBound())
        {
            pItem->SetState(ITEM_CHANGED, pUser);
            pItem->SetBinding(true);
        }
    }

    SpellCastTargets targets;
    targets.Read(recvPacket, pUser);
    HandleClientCastFlags(recvPacket, castFlags, targets);

    // Note: If script stop casting it must send appropriate data to client to prevent stuck item in gray state.
    if (!sScriptMgr->OnItemUse(pUser, pItem, targets))
    {
        // no script or script not process request by self
        pUser->CastItemUseSpell(pItem, targets, castCount, glyphIndex);
    }
}

void WorldSession::HandleOpenItemOpcode(WorldPacket& recvPacket)
{
    TC_LOG_DEBUG("network", "WORLD: CMSG_OPEN_ITEM packet, data length = %i", (uint32)recvPacket.size());

    Player* pUser = _player;

    // ignore for remote control state
    if (pUser->m_mover != pUser)
        return;

    uint8 bagIndex, slot;

    recvPacket >> bagIndex >> slot;

    TC_LOG_INFO("network", "bagIndex: %u, slot: %u", bagIndex, slot);

    Item* item = pUser->GetItemByPos(bagIndex, slot);
    if (!item)
    {
        pUser->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, NULL, NULL);
        return;
    }

    ItemTemplate const* proto = item->GetTemplate();
    if (!proto)
    {
        pUser->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, item, NULL);
        return;
    }

    // Verify that the bag is an actual bag or wrapped item that can be used "normally"
    if (!(proto->Flags & ITEM_PROTO_FLAG_OPENABLE) && !item->HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_WRAPPED))
    {
        pUser->SendEquipError(EQUIP_ERR_CLIENT_LOCKED_OUT, item, NULL);
        TC_LOG_ERROR("network", "Possible hacking attempt: Player %s [guid: %u] tried to open item [guid: %u, entry: %u] which is not openable!",
                pUser->GetName().c_str(), pUser->GetGUIDLow(), item->GetGUIDLow(), proto->ItemId);
        return;
    }

    // locked item
    uint32 lockId = proto->LockID;
    if (lockId)
    {
        LockEntry const* lockInfo = sLockStore.LookupEntry(lockId);

        if (!lockInfo)
        {
            pUser->SendEquipError(EQUIP_ERR_ITEM_LOCKED, item, NULL);
            TC_LOG_ERROR("network", "WORLD::OpenItem: item [guid = %u] has an unknown lockId: %u!", item->GetGUIDLow(), lockId);
            return;
        }

        // was not unlocked yet
        if (item->IsLocked())
        {
            pUser->SendEquipError(EQUIP_ERR_ITEM_LOCKED, item, NULL);
            return;
        }
    }

    if (item->HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_WRAPPED))// wrapped?
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_GIFT_BY_ITEM);

        stmt->setUInt32(0, item->GetGUIDLow());

        PreparedQueryResult result = CharacterDatabase.Query(stmt);

        if (result)
        {
            Field* fields = result->Fetch();
            uint32 entry = fields[0].GetUInt32();
            uint32 flags = fields[1].GetUInt32();

            item->SetUInt64Value(ITEM_FIELD_GIFT_CREATOR, 0);
            item->SetEntry(entry);
            item->SetUInt32Value(ITEM_FIELD_DYNAMIC_FLAGS, flags);
            item->SetState(ITEM_CHANGED, pUser);
        }
        else
        {
            TC_LOG_ERROR("network", "Wrapped item %u don't have record in character_gifts table and will deleted", item->GetGUIDLow());
            pUser->DestroyItem(item->GetBagSlot(), item->GetSlot(), true);
            return;
        }

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GIFT);

        stmt->setUInt32(0, item->GetGUIDLow());

        CharacterDatabase.Execute(stmt);
    }
    else
        pUser->SendLoot(item->GetGUID(), LOOT_CORPSE);
}

void WorldSession::HandleGameObjectUseOpcode(WorldPacket& recvData)
{
    uint64 guid;

    recvData >> guid;

    TC_LOG_DEBUG("network", "WORLD: Recvd CMSG_GAMEOBJ_USE Message [guid=%u]", GUID_LOPART(guid));

    if (GameObject* obj = GetPlayer()->GetMap()->GetGameObject(guid))
    {
        // ignore for remote control state
        if (_player->m_mover != _player)
            if (!(_player->IsOnVehicle(_player->m_mover) || _player->IsMounted()) && !obj->GetGOInfo()->IsUsableMounted())
                return;

        obj->Use(_player);
    }
}

void WorldSession::HandleGameobjectReportUse(WorldPacket& recvPacket)
{
    uint64 guid;
    recvPacket >> guid;

    TC_LOG_DEBUG("network", "WORLD: Recvd CMSG_GAMEOBJ_REPORT_USE Message [in game guid: %u]", GUID_LOPART(guid));

    // ignore for remote control state
    if (_player->m_mover != _player)
        return;

    GameObject* go = GetPlayer()->GetMap()->GetGameObject(guid);
    if (!go)
        return;

    if (!go->IsWithinDistInMap(_player, INTERACTION_DISTANCE))
        return;

    if (go->AI()->GossipHello(_player))
        return;

    _player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT, go->GetEntry());
}

void WorldSession::HandleCastSpellOpcode(WorldPacket& recvPacket)
{
    // ignore for remote control state (for player case)
    Unit* mover = _player->m_mover;
    if (mover != _player && mover->GetTypeId() == TYPEID_PLAYER)
    {
        recvPacket.rfinish(); // prevent spam at ignore packet
        return;
    }

    uint8 castCount = 0;
    uint8 castFlags = 0;
    uint32 spellId = 0;
    uint32 glyphIndex = 0;
    uint32 targetMask = 0;
    uint32 targetStringLength = 0;
    float elevation = 0.0f;
    float missileSpeed = 0.0f;
    ObjectGuid targetGuid = 0;
    ObjectGuid itemTargetGuid = 0;
    ObjectGuid destTransportGuid = 0;
    ObjectGuid srcTransportGuid = 0;
    Position srcPos;
    Position destPos;
    std::string targetString;
    
    // Movement data
    MovementInfo movementInfo;
    ObjectGuid movementTransportGuid = 0;
    ObjectGuid movementGuid = 0;
    bool hasTransport = false;
    bool hasTransportTime2 = false;
    bool hasTransportTime3 = false;
    bool hasFallData = false;
    bool hasFallDirection = false;
    bool hasTimestamp = false;
    bool hasSplineElevation = false;
    bool hasPitch = false;
    bool hasOrientation = false;
    bool hasUnkMovementField = false;
    uint32 unkMovementLoopCounter = 0;
    Unit* caster = mover;

    bool hasMovement = recvPacket.ReadBit();                                        // 416
    bool hasElevation = !recvPacket.ReadBit();                                      // 248
    bool hasMissileSpeed = !recvPacket.ReadBit();                                   // 252
    bool hasDestLocation = recvPacket.ReadBit();                                    // 112
    bool hasTargetMask = !recvPacket.ReadBit();                                     // 32
    bool hasTargetString = !recvPacket.ReadBit();                                   // 120
    bool hasCastCount = !recvPacket.ReadBit();                                      // 16
    bool hasSpellId = !recvPacket.ReadBit();                                        // 20
    recvPacket.ReadBit();
    recvPacket.ReadBit();
    uint8 researchDataCount = recvPacket.ReadBits(2);                               // 424

    for (uint8 i = 0; i < researchDataCount; ++i)
        recvPacket.ReadBits(2); // Unk bits related to archaeology research

    bool hasSrcLocation = recvPacket.ReadBit();                                     // 80
    bool hasGlyphIndex = !recvPacket.ReadBit();                                     // 24
    bool hasCastFlags = !recvPacket.ReadBit();                                      // 28

    itemTargetGuid[7] = recvPacket.ReadBit();                                       // 55
    itemTargetGuid[3] = recvPacket.ReadBit();                                       // 51
    itemTargetGuid[1] = recvPacket.ReadBit();                                       // 49
    itemTargetGuid[0] = recvPacket.ReadBit();                                       // 48
    itemTargetGuid[5] = recvPacket.ReadBit();                                       // 53
    itemTargetGuid[4] = recvPacket.ReadBit();                                       // 52
    itemTargetGuid[6] = recvPacket.ReadBit();                                       // 54
    itemTargetGuid[2] = recvPacket.ReadBit();                                       // 50

    if (hasSrcLocation)
    {
        srcTransportGuid[7] = recvPacket.ReadBit();                                 // 63
        srcTransportGuid[2] = recvPacket.ReadBit();                                 // 58
        srcTransportGuid[0] = recvPacket.ReadBit();                                 // 56
        srcTransportGuid[6] = recvPacket.ReadBit();                                 // 62
        srcTransportGuid[4] = recvPacket.ReadBit();                                 // 60
        srcTransportGuid[5] = recvPacket.ReadBit();                                 // 61
        srcTransportGuid[1] = recvPacket.ReadBit();                                 // 57
        srcTransportGuid[3] = recvPacket.ReadBit();                                 // 59
    }

    if (hasMovement)
    {
        movementGuid[0] = recvPacket.ReadBit();                                     // 256
        unkMovementLoopCounter = recvPacket.ReadBits(22);                           // 392
        bool hasMovementFlags2 = !recvPacket.ReadBit();                             // 268
        hasTransport = recvPacket.ReadBit();                                        // 344
        movementGuid[6] = recvPacket.ReadBit();                                     // 262

        if (hasTransport)
        {
            movementTransportGuid[0] = recvPacket.ReadBit();                        // 296
            movementTransportGuid[3] = recvPacket.ReadBit();                        // 299
            movementTransportGuid[4] = recvPacket.ReadBit();                        // 300
            movementTransportGuid[5] = recvPacket.ReadBit();                        // 301
            movementTransportGuid[7] = recvPacket.ReadBit();                        // 303
            movementTransportGuid[6] = recvPacket.ReadBit();                        // 302
            movementTransportGuid[2] = recvPacket.ReadBit();                        // 298
            hasTransportTime3 = recvPacket.ReadBit();                               // 340
            movementTransportGuid[1] = recvPacket.ReadBit();                        // 297
            hasTransportTime2 = recvPacket.ReadBit();                               // 332
        }

        if (hasMovementFlags2)
            movementInfo.flags2 = recvPacket.ReadBits(13);                          // 268

        hasFallData = recvPacket.ReadBit();                                         // 380
        movementGuid[5] = recvPacket.ReadBit();                                     // 261

        if (hasFallData)
            hasFallDirection = recvPacket.ReadBit();                                // 376

        hasOrientation = !recvPacket.ReadBit();                                     // 288
        recvPacket.ReadBit();                                                       // 388
        movementGuid[7] = recvPacket.ReadBit();                                     // 263
        movementGuid[1] = recvPacket.ReadBit();                                     // 257
        movementGuid[3] = recvPacket.ReadBit();                                     // 259
        hasSplineElevation = !recvPacket.ReadBit();                                 // 384
        recvPacket.ReadBit();                                                       // 389
        bool hasMovementFlags = !recvPacket.ReadBit();                              // 264
        hasPitch = !recvPacket.ReadBit();                                           // 352
        hasUnkMovementField = !recvPacket.ReadBit();                                // 408
        movementGuid[2] = recvPacket.ReadBit();                                     // 258
        movementGuid[4] = recvPacket.ReadBit();                                     // 260
        recvPacket.ReadBit();                                                       // 412
        hasTimestamp = !recvPacket.ReadBit();                                       // 272

        if (hasMovementFlags)
            movementInfo.flags = recvPacket.ReadBits(30);                           // 264
    }

    targetGuid[0] = recvPacket.ReadBit();                                           // 40
    targetGuid[2] = recvPacket.ReadBit();                                           // 42
    targetGuid[7] = recvPacket.ReadBit();                                           // 47
    targetGuid[4] = recvPacket.ReadBit();                                           // 44
    targetGuid[5] = recvPacket.ReadBit();                                           // 45
    targetGuid[6] = recvPacket.ReadBit();                                           // 46
    targetGuid[1] = recvPacket.ReadBit();                                           // 41
    targetGuid[3] = recvPacket.ReadBit();                                           // 43

    if (hasDestLocation)
    {
        destTransportGuid[2] = recvPacket.ReadBit();                                // 90
        destTransportGuid[6] = recvPacket.ReadBit();                                // 94
        destTransportGuid[1] = recvPacket.ReadBit();                                // 89
        destTransportGuid[4] = recvPacket.ReadBit();                                // 92
        destTransportGuid[5] = recvPacket.ReadBit();                                // 93
        destTransportGuid[3] = recvPacket.ReadBit();                                // 91
        destTransportGuid[7] = recvPacket.ReadBit();                                // 95
        destTransportGuid[0] = recvPacket.ReadBit();                                // 88
    }


    if (hasTargetMask)
        targetMask = recvPacket.ReadBits(20);                                       // 32

    if (hasTargetString)
        targetStringLength = recvPacket.ReadBits(7);                                // 120

    if (hasCastFlags)
        castFlags = recvPacket.ReadBits(5);                                         // 28

    for (uint8 i = 0; i < researchDataCount; ++i)
    {
        recvPacket.read_skip<uint32>(); // Archaeology research keystone/fragment id
        recvPacket.read_skip<uint32>(); // Archaeology research keystone/fragment count
    }

    if (hasDestLocation)
    {
        float x, y, z;
        recvPacket.ReadByteSeq(destTransportGuid[2]);                               // 90
        recvPacket.ReadByteSeq(destTransportGuid[1]);                               // 89
        recvPacket.ReadByteSeq(destTransportGuid[6]);                               // 94
        recvPacket >> z;                                                            // 104
        recvPacket.ReadByteSeq(destTransportGuid[3]);                               // 91
        recvPacket >> y;                                                            // 100
        recvPacket.ReadByteSeq(destTransportGuid[5]);                               // 93
        recvPacket >> x;                                                            // 96
        recvPacket.ReadByteSeq(destTransportGuid[0]);                               // 88
        recvPacket.ReadByteSeq(destTransportGuid[7]);                               // 95
        recvPacket.ReadByteSeq(destTransportGuid[4]);                               // 92
        destPos.Relocate(x, y, z);
    }
    else
    {
        destTransportGuid = caster->GetTransGUID();
        if (destTransportGuid)
            destPos.Relocate(caster->GetTransOffsetX(), caster->GetTransOffsetY(), caster->GetTransOffsetZ(), caster->GetTransOffsetO());
        else
            destPos.Relocate(caster);
    }

    recvPacket.ReadByteSeq(targetGuid[4]);                                          // 44
    recvPacket.ReadByteSeq(targetGuid[2]);                                          // 42
    recvPacket.ReadByteSeq(targetGuid[1]);                                          // 41
    recvPacket.ReadByteSeq(targetGuid[0]);                                          // 40
    recvPacket.ReadByteSeq(targetGuid[5]);                                          // 45
    recvPacket.ReadByteSeq(targetGuid[3]);                                          // 43
    recvPacket.ReadByteSeq(targetGuid[6]);                                          // 46
    recvPacket.ReadByteSeq(targetGuid[7]);                                          // 47

    if (hasSpellId)
        recvPacket >> spellId;                                                      // 20

    if (hasMovement)
    {
        recvPacket.ReadByteSeq(movementGuid[4]);                                    // 260
        recvPacket.ReadByteSeq(movementGuid[7]);                                    // 263

        for (uint8 i = 0; i != unkMovementLoopCounter; i++)
            recvPacket.read_skip<uint32>();                                         // 396

        if (hasFallData)
        {
            recvPacket >> movementInfo.jump.zspeed;                                 // 360
            recvPacket >> movementInfo.jump.fallTime;                               // 356

            if (hasFallDirection)
            {
                recvPacket >> movementInfo.jump.cosAngle;                           // 368
                recvPacket >> movementInfo.jump.xyspeed;                            // 372
                recvPacket >> movementInfo.jump.sinAngle;                           // 364
            }
        }

        if (hasOrientation)
            movementInfo.pos.SetOrientation(recvPacket.read<float>());              // 288

        if (hasTransport)
        {
            recvPacket >> movementInfo.transport.pos.m_positionX;                   // 304
            recvPacket.ReadByteSeq(movementTransportGuid[3]);                       // 299
            movementInfo.transport.pos.SetOrientation(recvPacket.read<float>());    // 316

            if (hasTransportTime3)
                recvPacket >> movementInfo.transport.time3;                         // 336

            recvPacket >> movementInfo.transport.time;                              // 324
            recvPacket >> movementInfo.transport.seat;                              // 320
            recvPacket.ReadByteSeq(movementTransportGuid[1]);                       // 297
            recvPacket.ReadByteSeq(movementTransportGuid[0]);                       // 296
            recvPacket.ReadByteSeq(movementTransportGuid[6]);                       // 302
            recvPacket.ReadByteSeq(movementTransportGuid[2]);                       // 298
            recvPacket >> movementInfo.transport.pos.m_positionZ;                   // 312
            recvPacket >> movementInfo.transport.pos.m_positionY;                   // 308
            recvPacket.ReadByteSeq(movementTransportGuid[4]);                       // 300
            recvPacket.ReadByteSeq(movementTransportGuid[5]);                       // 301
            recvPacket.ReadByteSeq(movementTransportGuid[7]);                       // 303

            if (hasTransportTime2)
                recvPacket >> movementInfo.transport.time2;                         // 328
        }

        if (hasSplineElevation)
            recvPacket >> movementInfo.splineElevation;                             // 384

        recvPacket.ReadByteSeq(movementGuid[2]);                                    // 258
        recvPacket >> movementInfo.pos.m_positionZ;                                 // 284
        recvPacket.ReadByteSeq(movementGuid[1]);                                    // 257
        recvPacket.ReadByteSeq(movementGuid[3]);                                    // 259
        recvPacket >> movementInfo.pos.m_positionX;                                 // 276

        if (hasUnkMovementField)
            recvPacket.read_skip<uint32>();                                         // 408

        recvPacket.ReadByteSeq(movementGuid[0]);                                    // 256
        recvPacket >> movementInfo.pos.m_positionY;                                 // 280
        recvPacket.ReadByteSeq(movementGuid[5]);                                    // 261

        if (hasPitch)
            movementInfo.pitch = G3D::wrap
                (recvPacket.read<float>(), float(-M_PI), float(M_PI));              // 352

        recvPacket.ReadByteSeq(movementGuid[6]);                                    // 262

        if (hasTimestamp)
            recvPacket >> movementInfo.time;                                        // 272
    }

    recvPacket.ReadByteSeq(itemTargetGuid[7]);                                      // 55
    recvPacket.ReadByteSeq(itemTargetGuid[2]);                                      // 50
    recvPacket.ReadByteSeq(itemTargetGuid[6]);                                      // 54
    recvPacket.ReadByteSeq(itemTargetGuid[3]);                                      // 51
    recvPacket.ReadByteSeq(itemTargetGuid[5]);                                      // 53
    recvPacket.ReadByteSeq(itemTargetGuid[0]);                                      // 48
    recvPacket.ReadByteSeq(itemTargetGuid[1]);                                      // 49
    recvPacket.ReadByteSeq(itemTargetGuid[4]);                                      // 52

    if (hasSrcLocation)
    {
        float x, y, z;
        recvPacket.ReadByteSeq(srcTransportGuid[0]);                                // 56
        recvPacket.ReadByteSeq(srcTransportGuid[1]);                                // 57
        recvPacket >> z;                                                            // 72
        recvPacket >> y;                                                            // 68
        recvPacket >> x;                                                            // 64
        recvPacket.ReadByteSeq(srcTransportGuid[4]);                                // 60
        recvPacket.ReadByteSeq(srcTransportGuid[2]);                                // 58
        recvPacket.ReadByteSeq(srcTransportGuid[6]);                                // 62
        recvPacket.ReadByteSeq(srcTransportGuid[3]);                                // 59
        recvPacket.ReadByteSeq(srcTransportGuid[7]);                                // 63
        recvPacket.ReadByteSeq(srcTransportGuid[5]);                                // 61
        srcPos.Relocate(x, y, z);
    }
    else
    {
        srcTransportGuid = caster->GetTransGUID();
        if (srcTransportGuid)
            srcPos.Relocate(caster->GetTransOffsetX(), caster->GetTransOffsetY(), caster->GetTransOffsetZ(), caster->GetTransOffsetO());
        else
            srcPos.Relocate(caster);
    }

    if (hasElevation)
        recvPacket >> elevation;                                                    // 248

    if (hasTargetString)
        targetString = recvPacket.ReadString(targetStringLength);                   // 120

    if (hasGlyphIndex)
        recvPacket >> glyphIndex;                                                   // 24

    if (hasCastCount)
        recvPacket >> castCount;                                                    // 16

    if (hasMissileSpeed)
        recvPacket >> missileSpeed;                                                 // 252

    TC_LOG_DEBUG("network", "WORLD: got cast spell packet, castCount: %u, spellId: %u, castFlags: %u, data length = %u", castCount, spellId, castFlags, (uint32)recvPacket.size());

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo)
    {
        TC_LOG_ERROR("network", "WORLD: unknown spell id %u", spellId);
        recvPacket.rfinish(); // prevent spam at ignore packet
        return;
    }

    if (spellInfo->IsPassive())
    {
        recvPacket.rfinish(); // prevent spam at ignore packet
        return;
    }

    if (caster->GetTypeId() == TYPEID_UNIT && !caster->ToCreature()->HasSpell(spellId))
    {
        // If the vehicle creature does not have the spell but it allows the passenger to cast own spells
        // change caster to player and let him cast
        if (!_player->IsOnVehicle(caster) || spellInfo->CheckVehicle(_player) != SPELL_CAST_OK)
        {
            recvPacket.rfinish(); // prevent spam at ignore packet
            return;
        }

        caster = _player;
    }

    if (caster->GetTypeId() == TYPEID_PLAYER && !caster->ToPlayer()->HasActiveSpell(spellId))
    {
        // not have spell in spellbook
        recvPacket.rfinish(); // prevent spam at ignore packet
        return;
    }

    Unit::AuraEffectList swaps = mover->GetAuraEffectsByType(SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS);
    Unit::AuraEffectList const& swaps2 = mover->GetAuraEffectsByType(SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS_2);
    if (!swaps2.empty())
        swaps.insert(swaps.end(), swaps2.begin(), swaps2.end());

    if (!swaps.empty())
    {
        for (Unit::AuraEffectList::const_iterator itr = swaps.begin(); itr != swaps.end(); ++itr)
        {
            if ((*itr)->IsAffectingSpell(spellInfo))
            {
                if (SpellInfo const* newInfo = sSpellMgr->GetSpellInfo((*itr)->GetAmount()))
                {
                    spellInfo = newInfo;
                    spellId = newInfo->Id;
                }
                break;
            }
        }
    }

    // Client is resending autoshot cast opcode when other spell is casted during shoot rotation
    // Skip it to prevent "interrupt" message
    if (spellInfo->IsAutoRepeatRangedSpell() && caster->GetCurrentSpell(CURRENT_AUTOREPEAT_SPELL)
        && caster->GetCurrentSpell(CURRENT_AUTOREPEAT_SPELL)->m_spellInfo == spellInfo)
    {
        recvPacket.rfinish();
        return;
    }

    // can't use our own spells when we're in possession of another unit,
    if (_player->isPossessing())
    {
        recvPacket.rfinish(); // prevent spam at ignore packet
        return;
    }

    // client provided targets
    SpellCastTargets targets(caster, targetMask, targetGuid, itemTargetGuid, srcTransportGuid, destTransportGuid, srcPos, destPos, elevation, missileSpeed, targetString);

    // auto-selection buff level base at target level (in spellInfo)
    if (targets.GetUnitTarget())
    {
        SpellInfo const* actualSpellInfo = spellInfo->GetAuraRankForLevel(targets.GetUnitTarget()->getLevel());

        // if rank not found then function return NULL but in explicit cast case original spell can be casted and later failed with appropriate error message
        if (actualSpellInfo)
            spellInfo = actualSpellInfo;
    }

    Spell* spell = new Spell(caster, spellInfo, TRIGGERED_NONE, 0, false);
    spell->m_cast_count = castCount;                       // set count of casts
    spell->m_glyphIndex = glyphIndex;
    spell->prepare(&targets);
}

void WorldSession::HandleCancelCastOpcode(WorldPacket& recvPacket)
{
    uint32 spellId = 0;
    uint8 counter = 0;

    bool hasSpellId = !recvPacket.ReadBit();
    bool hasCounter = !recvPacket.ReadBit();
    recvPacket.FlushBits();

    if (hasSpellId)
        recvPacket >> spellId;

    if (hasCounter)
        recvPacket >> counter;

    if (_player->IsNonMeleeSpellCasted(false))
        _player->InterruptNonMeleeSpells(false, spellId, false);
}

void WorldSession::HandleCancelAuraOpcode(WorldPacket& recvPacket)
{
    uint32 spellId;
    recvPacket >> spellId;
    
    ObjectGuid guid;

	recvPacket.ReadBit();
    
    guid[2] = recvPacket.ReadBit();
    guid[5] = recvPacket.ReadBit();
    guid[6] = recvPacket.ReadBit();
    guid[7] = recvPacket.ReadBit();
    guid[3] = recvPacket.ReadBit();
    guid[0] = recvPacket.ReadBit();
    guid[4] = recvPacket.ReadBit();
    guid[1] = recvPacket.ReadBit();

	recvPacket.FlushBits();

    recvPacket.ReadByteSeq(guid[3]);
    recvPacket.ReadByteSeq(guid[1]);
    recvPacket.ReadByteSeq(guid[6]);
    recvPacket.ReadByteSeq(guid[7]);
    recvPacket.ReadByteSeq(guid[5]);
    recvPacket.ReadByteSeq(guid[0]);
    recvPacket.ReadByteSeq(guid[4]);
    recvPacket.ReadByteSeq(guid[2]);

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo)
        return;

    // not allow remove spells with attr SPELL_ATTR0_CANT_CANCEL
    if (spellInfo->Attributes & SPELL_ATTR0_CANT_CANCEL)
        return;

    // channeled spell case (it currently casted then)
    if (spellInfo->IsChanneled())
    {
        if (Spell* curSpell = _player->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
            if (curSpell->m_spellInfo->Id == spellId)
                _player->InterruptSpell(CURRENT_CHANNELED_SPELL);
        return;
    }

    // non channeled case:
    // don't allow remove non positive spells
    // don't allow cancelling passive auras (some of them are visible)
    if (!spellInfo->IsPositive() || spellInfo->IsPassive())
        return;

    // maybe should only remove one buff when there are multiple?
    _player->RemoveOwnedAura(spellId, 0, 0, AURA_REMOVE_BY_CANCEL);
}

void WorldSession::HandlePetCancelAuraOpcode(WorldPacket& recvPacket)
{
    uint64 guid;
    uint32 spellId;

    recvPacket >> guid;
    recvPacket >> spellId;

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo)
    {
        TC_LOG_ERROR("network", "WORLD: unknown PET spell id %u", spellId);
        return;
    }

    Creature* pet=ObjectAccessor::GetCreatureOrPetOrVehicle(*_player, guid);

    if (!pet)
    {
        TC_LOG_ERROR("network", "HandlePetCancelAura: Attempt to cancel an aura for non-existant pet %u by player '%s'", uint32(GUID_LOPART(guid)), GetPlayer()->GetName().c_str());
        return;
    }

    if (pet != GetPlayer()->GetGuardianPet() && pet != GetPlayer()->GetCharm())
    {
        TC_LOG_ERROR("network", "HandlePetCancelAura: Pet %u is not a pet of player '%s'", uint32(GUID_LOPART(guid)), GetPlayer()->GetName().c_str());
        return;
    }

    if (!pet->IsAlive())
    {
        pet->SendPetActionFeedback(FEEDBACK_PET_DEAD);
        return;
    }

    pet->RemoveOwnedAura(spellId, 0, 0, AURA_REMOVE_BY_CANCEL);

    pet->AddCreatureSpellCooldown(spellId);
}

void WorldSession::HandleCancelGrowthAuraOpcode(WorldPacket& /*recvPacket*/) { }

void WorldSession::HandleCancelAutoRepeatSpellOpcode(WorldPacket& /*recvPacket*/)
{
    // may be better send SMSG_CANCEL_AUTO_REPEAT?
    // cancel and prepare for deleting
    _player->InterruptSpell(CURRENT_AUTOREPEAT_SPELL);
}

void WorldSession::HandleCancelChanneling(WorldPacket& recvData)
{
    recvData.read_skip<uint32>();                          // spellid, not used

    // ignore for remote control state (for player case)
    Unit* mover = _player->m_mover;
    if (mover != _player && mover->GetTypeId() == TYPEID_PLAYER)
        return;

    mover->InterruptSpell(CURRENT_CHANNELED_SPELL);
}

void WorldSession::HandleTotemDestroyed(WorldPacket& recvPacket)
{
    // ignore for remote control state
    if (_player->m_mover != _player)
        return;

    uint8 slotId;
    uint64 guid;
    recvPacket >> slotId;
    recvPacket >> guid;

    ++slotId;
    if (slotId >= MAX_TOTEM_SLOT)
        return;

    if (!_player->m_SummonSlot[slotId])
        return;

    Creature* totem = GetPlayer()->GetMap()->GetCreature(_player->m_SummonSlot[slotId]);
    if (totem && totem->IsTotem() && totem->GetGUID() == guid)
        totem->ToTotem()->UnSummon();
}

void WorldSession::HandleSelfResOpcode(WorldPacket& /*recvData*/)
{
    TC_LOG_DEBUG("network", "WORLD: CMSG_SELF_RES");                  // empty opcode

    if (_player->HasAuraType(SPELL_AURA_PREVENT_RESURRECTION))
        return; // silent return, client should display error by itself and not send this opcode

    if (_player->GetUInt32Value(PLAYER_FIELD_SELF_RES_SPELL))
    {
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(_player->GetUInt32Value(PLAYER_FIELD_SELF_RES_SPELL));
        if (spellInfo)
            _player->CastSpell(_player, spellInfo, false, 0);

        _player->SetUInt32Value(PLAYER_FIELD_SELF_RES_SPELL, 0);
    }
}

void WorldSession::HandleSpellClick(WorldPacket& recvData)
{
    uint64 guid;
    recvData >> guid;

    // this will get something not in world. crash
    Creature* unit = ObjectAccessor::GetCreatureOrPetOrVehicle(*_player, guid);

    if (!unit)
        return;

    /// @todo Unit::SetCharmedBy: 28782 is not in world but 0 is trying to charm it! -> crash
    if (!unit->IsInWorld())
        return;

    unit->HandleSpellClick(_player);
}

void WorldSession::HandleMirrorImageDataRequest(WorldPacket& recvData)
{
    TC_LOG_DEBUG("network", "WORLD: CMSG_GET_MIRRORIMAGE_DATA");
    uint64 guid;
    recvData >> guid;
    recvData.read_skip<uint32>(); // DisplayId ?

    // Get unit for which data is needed by client
    Unit* unit = ObjectAccessor::GetObjectInWorld(guid, (Unit*)NULL);
    if (!unit)
        return;

    if (!unit->HasAuraType(SPELL_AURA_CLONE_CASTER))
        return;

    // Get creator of the unit (SPELL_AURA_CLONE_CASTER does not stack)
    Unit* creator = unit->GetAuraEffectsByType(SPELL_AURA_CLONE_CASTER).front()->GetCaster();
    if (!creator)
        return;

    WorldPacket data(SMSG_MIRRORIMAGE_DATA, 68);
    data << uint64(guid);
    data << uint32(creator->GetDisplayId());
    data << uint8(creator->getRace());
    data << uint8(creator->getGender());
    data << uint8(creator->getClass());

    if (creator->GetTypeId() == TYPEID_PLAYER)
    {
        Player* player = creator->ToPlayer();
        Guild* guild = NULL;

        if (uint32 guildId = player->GetGuildId())
            guild = sGuildMgr->GetGuildById(guildId);

        data << uint8(player->GetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, 0));   // skin
        data << uint8(player->GetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, 1));   // face
        data << uint8(player->GetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, 2));   // hair
        data << uint8(player->GetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, 3));   // haircolor
        data << uint8(player->GetByteValue(PLAYER_FIELD_REST_STATE, 0)); // facialhair
        data << uint64(guild ? guild->GetGUID() : 0);

        static EquipmentSlots const itemSlots[] =
        {
            EQUIPMENT_SLOT_HEAD,
            EQUIPMENT_SLOT_SHOULDERS,
            EQUIPMENT_SLOT_BODY,
            EQUIPMENT_SLOT_CHEST,
            EQUIPMENT_SLOT_WAIST,
            EQUIPMENT_SLOT_LEGS,
            EQUIPMENT_SLOT_FEET,
            EQUIPMENT_SLOT_WRISTS,
            EQUIPMENT_SLOT_HANDS,
            EQUIPMENT_SLOT_BACK,
            EQUIPMENT_SLOT_TABARD,
            EQUIPMENT_SLOT_END
        };

        // Display items in visible slots
        for (EquipmentSlots const* itr = &itemSlots[0]; *itr != EQUIPMENT_SLOT_END; ++itr)
        {
            if (*itr == EQUIPMENT_SLOT_HEAD && player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_HIDE_HELM))
                data << uint32(0);
            else if (*itr == EQUIPMENT_SLOT_BACK && player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_HIDE_CLOAK))
                data << uint32(0);
            else if (Item const* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, *itr))
                data << uint32(item->GetTemplate()->DisplayInfoID);
            else
                data << uint32(0);
        }
    }
    else
    {
        // Skip player data for creatures
        data << uint8(0);
        data << uint32(0);
        data << uint32(0);
        data << uint32(0);
        data << uint32(0);
        data << uint32(0);
        data << uint32(0);
        data << uint32(0);
        data << uint32(0);
        data << uint32(0);
        data << uint32(0);
        data << uint32(0);
        data << uint32(0);
        data << uint32(0);
    }

    SendPacket(&data);
}

void WorldSession::HandleUpdateProjectilePosition(WorldPacket& recvPacket)
{
    TC_LOG_DEBUG("network", "WORLD: CMSG_UPDATE_PROJECTILE_POSITION");

    uint64 casterGuid;
    uint32 spellId;
    uint8 castCount;
    float x, y, z;    // Position of missile hit

    recvPacket >> casterGuid;
    recvPacket >> spellId;
    recvPacket >> castCount;
    recvPacket >> x;
    recvPacket >> y;
    recvPacket >> z;

    Unit* caster = ObjectAccessor::GetUnit(*_player, casterGuid);
    if (!caster)
        return;

    Spell* spell = caster->FindCurrentSpellBySpellId(spellId);
    if (!spell || !spell->m_targets.HasDst())
        return;

    Position pos = *spell->m_targets.GetDstPos();
    pos.Relocate(x, y, z);
    spell->m_targets.ModDst(pos);

    WorldPacket data(SMSG_SET_PROJECTILE_POSITION, 21);
    data << uint64(casterGuid);
    data << uint8(castCount);
    data << float(x);
    data << float(y);
    data << float(z);
    caster->SendMessageToSet(&data, true);
}

void WorldSession::HandleRequestCategoryCooldowns(WorldPacket& /*recvPacket*/)
{
    std::map<uint32, int32> categoryMods;
    Unit::AuraEffectList const& categoryCooldownAuras = _player->GetAuraEffectsByType(SPELL_AURA_MOD_SPELL_CATEGORY_COOLDOWN);
    for (Unit::AuraEffectList::const_iterator itr = categoryCooldownAuras.begin(); itr != categoryCooldownAuras.end(); ++itr)
    {
        std::map<uint32, int32>::iterator cItr = categoryMods.find((*itr)->GetMiscValue());
        if (cItr == categoryMods.end())
            categoryMods[(*itr)->GetMiscValue()] = (*itr)->GetAmount();
        else
            cItr->second += (*itr)->GetAmount();
    }

    WorldPacket data(SMSG_SPELL_CATEGORY_COOLDOWN, 11);
    data.WriteBits(categoryMods.size(), 23);
    data.FlushBits();
    for (std::map<uint32, int32>::const_iterator itr = categoryMods.begin(); itr != categoryMods.end(); ++itr)
    {
        data << uint32(itr->first);
        data << int32(-itr->second);
    }

    SendPacket(&data);
}
