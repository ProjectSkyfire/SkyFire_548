/*
 * Copyright (C) 2011-2014 Project SkyFire <http://www.projectskyfire.org/>
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

#ifndef _WORLDSTATE_BUILDER_
#define _WORLDSTATE_BUILDER_

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

            for (std::map<uint32, uint32>::const_iterator iter = _wstateList.begin(); iter != _wstateList.end(); iter++)
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
