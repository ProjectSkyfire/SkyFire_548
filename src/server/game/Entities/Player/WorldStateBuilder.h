/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SF_WORLDSTATE_BUILDER_H
#define SF_WORLDSTATE_BUILDER_H

#include "WorldPacket.h"
#include "WorldSession.h"

// This builder had to be created because blizzard is swaping worldstate with value therefore a big rewrite would be needed everytime
class WorldStateBuilder
{
    public:
        WorldStateBuilder(uint32 mapId, uint32 areaId, uint32 zoneId, std::map<uint32, uint32> wstateList = std::map<uint32, uint32>()) :
            _mapId(mapId), _zoneId(zoneId), _areaId(areaId), _wstateList(wstateList) {}

        void SendPacket(WorldSession* session)
        {
            WorldPacket data(SMSG_INIT_WORLD_STATES, 4 + 4 + 4 + 3 + (_wstateList.size() * (4 + 4)));
            data << uint32(_mapId);
            data << uint32(_zoneId);
            data << uint32(_areaId);
            data.WriteBits(_wstateList.size(), 21);
            data.FlushBits();

            for (std::map<uint32, uint32>::const_iterator iter = _wstateList.begin(); iter != _wstateList.end(); ++iter)
                data << uint32(iter->second) << uint32(iter->first);

            session->SendPacket(&data);
        }

        void AppendState(uint32 worldstate, uint32 value) { _wstateList.insert(std::make_pair(worldstate, value)); }

    private:
        uint32 _mapId;
        uint32 _zoneId;
        uint32 _areaId;
        std::map<uint32, uint32> _wstateList;
};
#endif
