/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#ifndef SKYFIRE_REALM_DIRECTORY_H
#define SKYFIRE_REALM_DIRECTORY_H
#include "ClusterProtocol.h"
#include <algorithm>
#include <chrono>
#include <map>
#include <mutex>

namespace Skyfire::Cluster::Realms
{
    constexpr Message RequestType = Message(7), ReplyType = Message(0x8002);
    constexpr std::size_t BatchSize = 16, MaximumRealms = 1024;
    constexpr std::uint32_t CacheMilliseconds = 3000;
    enum class Status : std::uint8_t { Offline, Ready, Draining, Maintenance, Incompatible, Conflict };
    inline char const* Name(Status status)
    {
        switch (status)
        {
            case Status::Ready: return "ready";
            case Status::Draining: return "draining";
            case Status::Maintenance: return "maintenance";
            case Status::Incompatible: return "incompatible";
            case Status::Conflict: return "conflict";
            default: return "offline";
        }
    }
    struct Query { std::uint32_t Realm = 0, Build = 0; };
    struct Route
    {
        Query Identity;
        Status State = Status::Offline;
        std::string Address = "-", Destination = "-";
        std::uint16_t Port = 0;
        std::uint32_t ValidFor = 0;
    };
    // A realm has one world owner. Independent game processes are not interchangeable replicas.
    inline Route Resolve(Query query, std::vector<Node> const& nodes, std::uint64_t now, bool requireHandoff)
    {
        Route route; route.Identity = query;
        Node const* owner = nullptr;
        for (auto const& node : nodes)
        {
            if (node.Type != Service::World || !node.Live || now >= node.ExpiresAt ||
                std::find(node.Realms.begin(),node.Realms.end(),query.Realm) == node.Realms.end()) continue;
            if (owner) { route.State = Status::Conflict; return route; }
            owner = &node;
        }
        if (!owner) return route;
        if (owner->Build != query.Build || (requireHandoff && !(owner->Capabilities & 64))) route.State = Status::Incompatible;
        else if (owner->Admin == Administration::Disabled) route.State = Status::Maintenance;
        else if (owner->Admin == Administration::Draining) route.State = Status::Draining;
        else if (owner->Ready)
        {
            route.Destination = owner->Key + ":" + std::to_string(owner->Owner);
            route.State = Status::Ready; route.Address = owner->Address; route.Port = owner->Port;
            route.ValidFor = std::uint32_t((std::min)(std::uint64_t(CacheMilliseconds),owner->ExpiresAt - now));
        }
        return route;
    }
    inline Writer EncodeQuery(std::vector<Query> const& queries)
    {
        Writer out; out.U8(1); out.U16(std::uint16_t(queries.size()));
        for (auto const& query : queries) { out.U32(query.Realm); out.U32(query.Build); }
        return out;
    }
    inline bool DecodeQuery(std::vector<std::uint8_t> const& bytes, std::vector<Query>& queries)
    {
        Reader reader(bytes); std::uint8_t version; std::uint16_t count;
        if (!reader.U8(version) || version != 1 || !reader.U16(count) || !count || count > BatchSize) return false;
        std::vector<Query> result;
        for (unsigned i = 0; i < count; ++i)
        {
            Query query;
            if (!reader.U32(query.Realm) || !query.Realm || !reader.U32(query.Build) || !query.Build ||
                std::any_of(result.begin(),result.end(),[&](Query const& q) { return q.Realm == query.Realm; })) return false;
            result.push_back(query);
        }
        if (!reader.End()) return false;
        queries = std::move(result); return true;
    }
    inline Writer EncodeReply(std::vector<Route> const& routes)
    {
        Writer out; out.U8(1); out.U16(std::uint16_t(routes.size()));
        for (auto const& route : routes)
        {
            out.U32(route.Identity.Realm); out.U32(route.Identity.Build); out.U8(std::uint8_t(route.State));
            out.String(route.Address); out.U16(route.Port); out.U32(route.ValidFor); out.String(route.Destination);
        }
        return out;
    }
    inline bool DecodeReply(std::vector<std::uint8_t> const& bytes, std::vector<Query> const& queries, std::vector<Route>& routes)
    {
        Reader reader(bytes); std::uint8_t version; std::uint16_t count;
        if (!reader.U8(version) || version != 1 || !reader.U16(count) || count != queries.size() || !count || count > BatchSize) return false;
        std::vector<Route> result;
        for (unsigned i = 0; i < count; ++i)
        {
            Route route; std::uint8_t state;
            if (!reader.U32(route.Identity.Realm) || route.Identity.Realm != queries[i].Realm ||
                !reader.U32(route.Identity.Build) || route.Identity.Build != queries[i].Build ||
                !reader.U8(state) || state > std::uint8_t(Status::Conflict) || !reader.String(route.Address,64) ||
                !reader.U16(route.Port) || !reader.U32(route.ValidFor) || !reader.String(route.Destination,96)) return false;
            route.State = Status(state);
            if (route.State == Status::Ready ? (route.Address == "-" || route.Destination == "-" || !route.Port || !route.ValidFor || route.ValidFor > CacheMilliseconds) :
                (route.Address != "-" || route.Destination != "-" || route.Port || route.ValidFor)) return false;
            result.push_back(std::move(route));
        }
        if (!reader.End()) return false;
        routes = std::move(result); return true;
    }
    inline std::uint64_t Now()
    {
        return std::uint64_t(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count());
    }
    // The agent writes; authentication packet builders read copies without network I/O.
    class Cache
    {
    public:
        void Configure(bool enabled) { std::lock_guard<std::mutex> lock(_mutex); _enabled = enabled; _routes.clear(); }
        bool Enabled() const { std::lock_guard<std::mutex> lock(_mutex); return _enabled; }
        void SetQueries(std::vector<Query> queries)
        {
            std::lock_guard<std::mutex> lock(_mutex);
            std::vector<Query> valid;
            for (auto query : queries)
                if (query.Realm && query.Build && valid.size() < MaximumRealms &&
                    std::none_of(valid.begin(),valid.end(),[&](Query const& q) { return q.Realm == query.Realm; })) valid.push_back(query);
            _queries = std::move(valid);
            for (auto it = _routes.begin(); it != _routes.end();)
                if (std::none_of(_queries.begin(),_queries.end(),[&](Query const& q) { return q.Realm == it->first && q.Build == it->second.Value.Identity.Build; }))
                    it = _routes.erase(it); else ++it;
        }
        std::vector<Query> Queries() const { std::lock_guard<std::mutex> lock(_mutex); return _queries; }
        void Clear() { std::lock_guard<std::mutex> lock(_mutex); _routes.clear(); }
        void Store(std::vector<Route> const& routes, std::uint64_t requestStarted)
        {
            std::lock_guard<std::mutex> lock(_mutex);
            for (auto const& route : routes)
                if (std::any_of(_queries.begin(),_queries.end(),[&](Query const& q) { return q.Realm == route.Identity.Realm && q.Build == route.Identity.Build; }))
                    _routes[route.Identity.Realm] = {route,requestStarted + (route.State == Status::Ready ? route.ValidFor : CacheMilliseconds)};
        }
        Route Read(Query query, std::uint64_t now) const
        {
            std::lock_guard<std::mutex> lock(_mutex);
            auto found = _routes.find(query.Realm);
            if (found != _routes.end() && found->second.Value.Identity.Build == query.Build && now < found->second.ExpiresAt) return found->second.Value;
            Route offline; offline.Identity = query; return offline;
        }
    private:
        struct Entry { Route Value; std::uint64_t ExpiresAt; };
        mutable std::mutex _mutex;
        bool _enabled = false;
        std::vector<Query> _queries;
        std::map<std::uint32_t,Entry> _routes;
    };
    inline Cache Client;
}
#endif
