/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SF_UPDATEDATA_H
#define SF_UPDATEDATA_H

#include "ByteBuffer.h"
#include <set>

class WorldPacket;

enum OBJECT_UPDATE_TYPE
{
    UPDATETYPE_VALUES               = 0,
    UPDATETYPE_CREATE_OBJECT        = 1,
    UPDATETYPE_CREATE_OBJECT2       = 2,
    UPDATETYPE_OUT_OF_RANGE_OBJECTS = 3,
};

enum OBJECT_UPDATE_FLAGS
{
    UPDATEFLAG_NONE                  = 0x0000,
    UPDATEFLAG_SELF                  = 0x0001,
    UPDATEFLAG_TRANSPORT             = 0x0002,
    UPDATEFLAG_HAS_TARGET            = 0x0004,
    UPDATEFLAG_LIVING                = 0x0008,
    UPDATEFLAG_STATIONARY_POSITION   = 0x0010,
    UPDATEFLAG_VEHICLE               = 0x0020,
    UPDATEFLAG_GO_TRANSPORT_POSITION = 0x0040,
    UPDATEFLAG_ROTATION              = 0x0080,
    UPDATEFLAG_ANIMKITS              = 0x0100,
};

class UpdateData
{
    public:
        UpdateData(uint16 map);

        void AddOutOfRangeGUID(std::set<uint64>& guids);
        void AddOutOfRangeGUID(uint64 guid);
        void AddUpdateBlock(const ByteBuffer &block);
        bool BuildPacket(WorldPacket* packet);
        bool HasData() const { return m_blockCount > 0 || !m_outOfRangeGUIDs.empty(); }
        void Clear();

        std::set<uint64> const& GetOutOfRangeGUIDs() const { return m_outOfRangeGUIDs; }

    protected:
        uint16 m_map;
        uint32 m_blockCount;
        std::set<uint64> m_outOfRangeGUIDs;
        ByteBuffer m_data;
};
#endif
