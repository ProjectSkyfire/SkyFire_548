/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef DB2PACKETWRITER_H
#define DB2PACKETWRITER_H

#include "Define.h"

template<class T>
class DB2Storage;
class ByteBuffer;
struct ItemEntry;
struct ItemSparseEntry;

namespace DB2Utilities
{
    //
    bool HasItemEntry(DB2Storage<ItemEntry> const& store, uint32 id);
    bool HasItemSparseEntry(DB2Storage<ItemSparseEntry> const& store, uint32 id);

    //
    void WriteItemDbReply(DB2Storage<ItemEntry> const& store, uint32 id, uint32 locale, ByteBuffer& buffer);
    void WriteItemSparseDbReply(DB2Storage<ItemSparseEntry> const& store, uint32 id, uint32 locale, ByteBuffer& buffer);
}

#endif // DB2PACKETWRITER_H
