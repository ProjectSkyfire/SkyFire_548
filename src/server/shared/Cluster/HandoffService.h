/* Part of Project SkyFire. See LICENSE.md for copyright information. */
#ifndef SKYFIRE_HANDOFF_SERVICE_H
#define SKYFIRE_HANDOFF_SERVICE_H
#include "HandoffProtocol.h"
#include <algorithm>
namespace Skyfire::Cluster::Handoff
{
    // Certificate identity must also have a current, ready registration. Destination ownership
    // is assigned here, never trusted from the caller. A restarted world gets a new owner id.
    inline Result Authorize(std::string const& identity, std::vector<Node> const& nodes, std::uint64_t now, Request& request)
    {
        auto live = [now](Node const& n) { return n.Live && n.Ready && (n.Capabilities & 64) && now < n.ExpiresAt; };
        auto caller = std::find_if(nodes.begin(),nodes.end(),[&](Node const& n) { return n.Key == identity && live(n); });
        if (caller == nodes.end()) return Result::Denied;
        if (request.Bind.Use == Purpose::Login)
        {
            if (caller->Type != Service::Auth) return Result::Denied;
            request.Bind.Destination = "authnet"; // May be consumed by a different registered auth node.
            return Result::Ok;
        }
        auto serves = [&](Node const& n)
        {
            return n.Type == Service::World && live(n) && n.Admin == Administration::Enabled &&
                std::find(n.Realms.begin(),n.Realms.end(),request.Bind.Realm) != n.Realms.end();
        };
        Node const* destination = nullptr;
        if (request.Action == Operation::Consume)
        {
            if (!serves(*caller)) return Result::Denied;
            destination = &*caller;
        }
        else
        {
            if (caller->Type != Service::Auth) return Result::Denied;
            for (auto const& node : nodes)
            {
                if (!serves(node)) continue;
                // Until the realm-directory phase chooses endpoints, ambiguity fails closed.
                if (destination) return Result::Unavailable;
                destination = &node;
            }
        }
        if (!destination) return Result::Unavailable;
        request.Bind.Destination = destination->Key + ":" + std::to_string(destination->Owner);
        return Result::Ok;
    }
    inline Result Execute(Store& store, Request const& request, std::uint64_t now, std::string& token)
    {
        token.clear();
        if (request.Action == Operation::Issue) return store.Issue(request.Bind,request.Ttl,now,token);
        if (request.Action == Operation::Inspect) return store.Inspect(request.Bind,now,token);
        auto selected = request.Token;
        if (selected == "-")
        {
            auto result = store.Inspect(request.Bind,now,selected);
            if (result != Result::Ok) return result;
        }
        return request.Action == Operation::Consume ? store.Consume(selected,request.Bind,now) : store.Revoke(selected,request.Bind,now);
    }
}
#endif
