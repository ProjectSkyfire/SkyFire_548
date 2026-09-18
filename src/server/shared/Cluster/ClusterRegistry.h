/*
 * This file is part of Project SkyFire https://www.projectskyfire.org.
 * See LICENSE.md file for Copyright information.
 */
#ifndef SKYFIRE_CLUSTER_REGISTRY_H
#define SKYFIRE_CLUSTER_REGISTRY_H
#include "ClusterProtocol.h"
#include <map>
#include <utility>

namespace Skyfire::Cluster
{
    // Owned by the hub main thread. Times are monotonic milliseconds, never wall clock.
    // Database configuration is separate from live registrations; restart starts empty.
    class Registry
    {
    public:
        explicit Registry(std::size_t capacity = 128) : _capacity(capacity) { }
        bool Register(Node node, std::uint64_t owner, std::uint64_t now, std::uint64_t lease)
        {
            Expire(now);
            if (!owner || !lease || !ValidKey(node.Key) || _nodes.count(node.Key) || _nodes.size() >= _capacity) return false;
            for (auto const& entry : _nodes) if (entry.second.Owner == owner) return false;
            node.Owner = owner; node.Ready = false; node.Load = 0; node.ExpiresAt = now + lease;
            node.Live = true;
            std::string const key = node.Key;
            _nodes.emplace(key, std::move(node));
            return true;
        }
        bool Renew(std::string const& key, std::uint64_t owner, std::uint64_t now, std::uint64_t lease,
            std::uint32_t load, int ready = -1)
        {
            auto it = _nodes.find(key);
            if (it == _nodes.end() || it->second.Owner != owner || now >= it->second.ExpiresAt || !lease) return false;
            it->second.ExpiresAt = now + lease; it->second.Load = load;
            if (ready >= 0) it->second.Ready = ready != 0;
            return true;
        }
        bool Remove(std::string const& key, std::uint64_t owner)
        {
            auto it = _nodes.find(key);
            if (it == _nodes.end() || it->second.Owner != owner) return false;
            _nodes.erase(it); return true;
        }
        bool SetRealms(std::string const& key, std::uint64_t owner, std::uint64_t now,
            std::uint64_t lease, std::vector<std::uint32_t> const& realms)
        {
            auto it = _nodes.find(key);
            if (it == _nodes.end() || it->second.Owner != owner || now >= it->second.ExpiresAt || !lease ||
                it->second.Type != Service::World || !(it->second.Capabilities & 8)) return false;
            Writer payload; payload.U16(std::uint16_t(realms.size()));
            if (realms.empty() || realms.size() > 64) return false;
            bool primary = false;
            for (auto id : realms) { payload.U32(id); if (id == it->second.Realm) primary = true; }
            std::vector<std::uint32_t> validated;
            if (!primary || !DecodeRealms(payload.Bytes, validated)) return false;
            it->second.Realms = std::move(validated);
            it->second.ExpiresAt = now + lease;
            return true;
        }
        void Expire(std::uint64_t now)
        {
            for (auto it = _nodes.begin(); it != _nodes.end();)
                if (now >= it->second.ExpiresAt) it = _nodes.erase(it); else ++it;
        }
        std::vector<Node> Snapshot() const
        {
            std::vector<Node> nodes;
            for (auto const& entry : _nodes)
            {
                nodes.push_back(entry.second);
                auto policy = _administration.find(entry.first);
                nodes.back().Admin = policy == _administration.end() ? Administration::Enabled : policy->second;
            }
            return nodes;
        }
        bool SetAdministration(std::string const& key, Administration state)
        {
            if (!ValidKey(key) || unsigned(state) > unsigned(Administration::Disabled)) return false;
            _administration[key] = state; return true;
        }
        void Clear() { _nodes.clear(); }
    private:
        std::size_t _capacity;
        std::map<std::string, Node> _nodes;
        std::map<std::string, Administration> _administration;
    };
}
#endif
