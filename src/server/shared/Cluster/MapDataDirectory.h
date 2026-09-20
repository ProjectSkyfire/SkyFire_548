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
    constexpr Message MetricsType = Message(9);
    constexpr std::uint32_t Capability = 256, RestartCapability = 512;
    constexpr Message RestartReply = Message(0x8004);
    inline bool DecodeQuery(std::vector<std::uint8_t> const& bytes, std::string& key)
    {
        Reader input(bytes); std::uint8_t version;
        return input.U8(version) && version == 1 && input.String(key, 64) && ValidKey(key) && input.End();
    }
    inline bool DecodeMetrics(std::vector<std::uint8_t> const& bytes, MapMetrics& metrics)
    {
        Reader input(bytes); std::uint8_t version; std::uint16_t count;
        if (!input.U8(version) || (version != 1 && version != 2) || !input.U32(metrics.Uptime) ||
            !input.U32(metrics.CpuBasisPoints) || metrics.CpuBasisPoints > 10000 ||
            !input.U32(metrics.MemoryMiB) || !input.U32(metrics.Requests) || !input.U32(metrics.Failures) ||
            !input.U32(metrics.SentKiB) || !input.U32(metrics.Assets) || !input.U32(metrics.Active) ||
            metrics.Active > 32 || !input.U16(count) || !count || count > 64) return false;
        metrics.Maps.clear();
        for (unsigned i = 0; i < count; ++i)
        {
            std::uint32_t id;
            if (!input.U32(id) || id > 9999 || std::find(metrics.Maps.begin(), metrics.Maps.end(), id) != metrics.Maps.end()) return false;
            metrics.Maps.push_back(id);
        }
        metrics.Generation.clear();
        if (version == 2 && (!input.String(metrics.Generation,32) || metrics.Generation.size() != 32 ||
            metrics.Generation.find_first_not_of("0123456789abcdef") != std::string::npos)) return false;
        return input.End();
    }
    inline bool RestartReady(Node const& node, std::string const& previous, std::uint64_t now)
    {
        return node.Type == Service::Map && node.Live && node.Ready && node.ExpiresAt > now &&
            (node.Capabilities & RestartCapability) && !node.Metrics.Generation.empty() &&
            node.Metrics.Generation != previous && node.Metrics.ReceivedAt && now >= node.Metrics.ReceivedAt &&
            now - node.Metrics.ReceivedAt <= 15000;
    }
    inline Writer Resolve(std::string const& key, std::vector<Node> const& nodes, std::uint64_t now)
    {
        Node const* selected = nullptr;
        for (auto const& node : nodes)
            if (node.Key == key && node.Type == Service::Map && (node.Capabilities & Capability) != 0 &&
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
