/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_SKYFIRE_CHANNELMGR_H
#define SF_SKYFIRE_CHANNELMGR_H

#include "Channel.h"
#include "Common.h"
#include <ace/Singleton.h>

#include <map>
#include <string>

#include "World.h"

class ChannelMgr
{
    typedef std::map<std::wstring, Channel*> ChannelMap;

public:
    ChannelMgr() : team(0)
    { }

    ~ChannelMgr();

    static ChannelMgr* forTeam(uint32 team);
    void setTeam(uint32 newTeam) { team = newTeam; }

    Channel* GetJoinChannel(std::string const& name, uint32 channel_id);
    Channel* GetChannel(std::string const& name, Player* p, bool pkt = true);
    void LeftChannel(std::string const& name);

private:
    ChannelMap channels;
    uint32 team;

    void MakeNotOnPacket(WorldPacket* data, std::string const& name);
};

class AllianceChannelMgr : public ChannelMgr { };
class HordeChannelMgr : public ChannelMgr { };

#endif
