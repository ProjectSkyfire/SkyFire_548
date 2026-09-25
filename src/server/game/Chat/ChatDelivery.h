/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#ifndef SKYFIRE_CHAT_DELIVERY_H
#define SKYFIRE_CHAT_DELIVERY_H
#include "Cluster/ChatClient.h"
#include <functional>
class Player;
class Group;
class WorldPacket;
class WorldSession;
namespace Skyfire::Chat::Delivery
{
    using Check = std::function<bool(Player*, Player*)>;
    // Returns true when service routing owns the request, including an explicit
    // failure. Never silently broadcast locally after an ambiguous service reply.
    bool Submit(Player* sender, AudienceKind kind, std::string const& text, std::uint32_t language,
        std::string const& prefix, WorldPacket const& packet, std::vector<Player*> const& recipients, Check check,
        std::function<void(Player*)> prepare = {});
    bool GroupMessage(Player* sender, Group* group, WorldPacket const& packet, std::string const& text,
        std::uint32_t language, std::string const& prefix, AudienceKind kind, int subgroup = -1,
        bool ignoreOtherGroups = false, std::uint64_t ignore = 0);
    void Update();
    bool SpatialMessage(Player* sender, WorldPacket const& packet, std::string const& text,
        std::uint32_t language, float range, bool ownTeamOnly = false);
    bool Control(WorldSession* session, WorldPacket& packet, void (WorldSession::*handler)(WorldPacket&),
        AudienceKind kind, char const* action);
    bool Command(WorldSession* session, std::string const& command);
}
#endif
