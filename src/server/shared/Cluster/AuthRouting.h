/* Part of Project SkyFire. See LICENSE.md for copyright information. */
#ifndef SKYFIRE_AUTH_ROUTING_H
#define SKYFIRE_AUTH_ROUTING_H
#include "ClusterProtocol.h"
#include <algorithm>
#include <map>
#include <set>

namespace Skyfire::Cluster
{
    // Main-thread owned. Smooth weighted round robin; reservations include connecting clients.
    class AuthRouting
    {
    public:
        using Identity = std::pair<std::string, std::uint64_t>;
        std::map<std::string, unsigned> Weights;
        bool Authnet = true;
        bool PreserveClientIP = true;
        bool Select(std::vector<Node> const& nodes, std::uint64_t now,
            std::set<std::string> const& excluded, Node& selected)
        {
            std::set<Identity> live;
            std::int64_t total = 0, best = 0;
            bool found = false;
            for (auto const& node : nodes)
            {
                Identity id{node.Key, node.Owner};
                live.insert(id);
                unsigned const weight = Weights.count(node.Key) ? Weights.at(node.Key) : 1;
                auto const count = _reserved.find(id);
                auto const reserved = count == _reserved.end() ? 0 : count->second;
                if (!weight || weight > 1000 || node.Type != Service::Auth || !node.Ready ||
                    now >= node.ExpiresAt || node.Build != 18414 || bool(node.Capabilities & 16) != Authnet ||
                    bool(node.Capabilities & 32) != PreserveClientIP || excluded.count(node.Key) ||
                    (node.Capacity && (std::max)(node.Load, reserved) >= node.Capacity))
                { _score.erase(id); continue; }
                auto& score = _score[id];
                score += weight; total += weight;
                if (!found || score > best) { found = true; best = score; selected = node; }
            }
            for (auto it = _score.begin(); it != _score.end();)
                if (!live.count(it->first)) it = _score.erase(it); else ++it;
            if (!found) return false;
            Identity id{selected.Key, selected.Owner};
            _score[id] -= total;
            ++_reserved[id];
            return true;
        }
        void Release(Node const& node)
        {
            auto it = _reserved.find({node.Key, node.Owner});
            if (it != _reserved.end() && --it->second == 0) _reserved.erase(it);
        }
        void Clear() { _score.clear(); _reserved.clear(); }
    private:
        std::map<Identity, std::int64_t> _score;
        std::map<Identity, std::uint32_t> _reserved;
    };
}
#endif
