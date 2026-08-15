/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_LOOT_PACKET_ENCODING_H
#define SKYFIRE_LOOT_PACKET_ENCODING_H

#include "ByteBuffer.h"

namespace Skyfire
{
namespace Looting
{
    // Gated by the flag written just before the owner GUID's third byte. All 703 sniffed
    // 18414 responses set that flag and follow it with these ten bits.
    //
    // The client caps a stack at a single item when the loot method is master, group or
    // need-before-greed and the item quality reaches the threshold. A missing threshold
    // reads as 0, which stops poor and common drops from ever merging in the loot window.
    inline void WriteLootResponseMethodThresholdBits(ByteBuffer* buffer)
    {
        buffer->WriteBits(0, 8); // LootMethod: FREE_FOR_ALL
        buffer->WriteBits(2, 2); // Threshold: ITEM_QUALITY_UNCOMMON
    }

    inline void WriteLootItemBitData(uint8 slotType, ByteBuffer* buffer)
    {
        buffer->WriteBits(slotType, 3);
        buffer->WriteBit(0); // canTradeToTapList
        buffer->WriteBit(1); // no optional slot type byte
        buffer->WriteBit(0); // slot is always present
        buffer->WriteBits(3, 2); // every sniffed 18414 loot item sets both bits
    }
}
}

#endif
