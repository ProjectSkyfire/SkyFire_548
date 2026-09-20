/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_MAP_DATA_DIRECTORY_H
#define SKYFIRE_MAP_DATA_DIRECTORY_H
#include "ClusterProtocol.h"
#include <algorithm>

namespace Skyfire::Cluster::MapData
{
    constexpr Message RequestType = Message(8), ReplyType = Message(0x8003);
    constexpr std::uint32_t Capability = 256;
    inline bool DecodeQuery(std::vector<std::uint8_t> const& bytes, std::string& key)
    {
        Reader input(bytes); std::uint8_t version;
        return input.U8(version) && version == 1 && input.String(key, 64) && ValidKey(key) && input.End();
    }
    inline Writer Resolve(std::string const& key, std::vector<Node> const& nodes, std::uint64_t now)
    {
        Node const* selected = nullptr;
        for (auto const& node : nodes)
            if (node.Key == key && node.Type == Service::Map && node.Capabilities == Capability &&
                node.Live && node.Ready && node.ExpiresAt > now && node.Build == 18414 &&
                node.Admin == Administration::Enabled && (!node.Capacity || node.Load < node.Capacity))
                selected = &node;
        Writer reply; reply.U8(1); reply.U8(selected ? 1 : 0);
        reply.String(selected ? selected->Address : "-"); reply.U16(selected ? selected->Port : 0);
        reply.String(selected ? selected->Key : "-");
        reply.U32(selected ? std::uint32_t((std::min)(std::uint64_t(3000), selected->ExpiresAt - now)) : 0);
        return reply;
    }
}
#endif
