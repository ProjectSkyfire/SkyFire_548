/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "Cluster/RealmDirectory.h"
#include "Cluster/ClusterRegistry.h"
#include "Cluster/HandoffService.h"
#include <iostream>
#include <stdexcept>
using namespace Skyfire::Cluster;
namespace
{
    void Check(bool ok, char const* message) { if (!ok) throw std::runtime_error(message); }
    Node World(std::string key, unsigned realm, unsigned owner)
    {
        Node node; node.Key = key; node.Type = Service::World; node.Realms = {realm}; node.Realm = realm;
        node.Build = 18414; node.Capabilities = 72; node.Ready = true; node.Owner = owner; node.ExpiresAt = 10000;
        node.Address = "192.0.2." + std::to_string(owner); node.Port = 8085; return node;
    }
    void Routing()
    {
        using Realms::Status;
        std::vector<Node> nodes = {World("world-a",1,1),World("world-b",2,2)};
        auto route = Realms::Resolve({1,18414},nodes,100,true);
        Check(route.State == Status::Ready && route.Address == "192.0.2.1" && route.Destination == "world-a:1","Wrong realm owner");
        Check(Realms::Resolve({2,18414},nodes,100,true).Address == "192.0.2.2","Second realm not isolated");
        Check(Realms::Resolve({3,18414},nodes,100,true).State == Status::Offline,"Unknown realm available");
        Check(Realms::Resolve({1,12345},nodes,100,true).State == Status::Incompatible,"Wrong build available");
        nodes[0].Admin = Administration::Draining;
        route = Realms::Resolve({1,18414},nodes,100,true);
        Check(route.State == Status::Draining && route.Address == "-" && !route.Port,"Drain published endpoint");
        nodes[0].Admin = Administration::Disabled;
        Check(Realms::Resolve({1,18414},nodes,100,true).State == Status::Maintenance,"Maintenance not represented");
        nodes[0].Admin = Administration::Enabled; nodes[0].Ready = false;
        Check(Realms::Resolve({1,18414},nodes,100,true).State == Status::Offline,"Unready realm available");
        nodes[0].Ready = true; nodes[0].Capabilities = 8;
        Check(Realms::Resolve({1,18414},nodes,100,true).State == Status::Incompatible,"Missing handoff capability accepted");
        nodes[0].Capabilities = 72; nodes.push_back(World("duplicate",1,3)); nodes.back().Ready = false;
        Check(Realms::Resolve({1,18414},nodes,100,true).State == Status::Conflict,"Duplicate ownership accepted");
        nodes.pop_back(); nodes[0].ExpiresAt = 101;
        Check(Realms::Resolve({1,18414},nodes,100,true).ValidFor == 1,"Cache can outlive lease");
        Check(Realms::Resolve({1,18414},nodes,101,true).State == Status::Offline,"Expired lease available");
        Registry registry; Check(registry.Register(World("world",1,1),1,100,100),"Registration failed");
        registry.Renew("world",1,100,100,0,1); registry.SetAdministration("world",Administration::Disabled);
        registry.Expire(200); Check(registry.Register(World("world",1,2),2,201,100),"Restart failed");
        registry.Renew("world",2,201,100,0,1);
        Check(Realms::Resolve({1,18414},registry.Snapshot(),201,true).State == Status::Maintenance,"Restart lost policy");
    }
    void WireAndCache()
    {
        std::vector<Realms::Query> queries = {{1,18414},{2,18414}}, decoded;
        auto bytes = Realms::EncodeQuery(queries).Bytes;
        Check(Realms::DecodeQuery(bytes,decoded),"Query roundtrip failed");
        for (std::size_t i = 0; i < bytes.size(); ++i)
            Check(!Realms::DecodeQuery({bytes.begin(),bytes.begin()+i},decoded),"Truncated query accepted");
        bytes.push_back(0); Check(!Realms::DecodeQuery(bytes,decoded),"Trailing query data accepted");
        queries[1] = queries[0]; Check(!Realms::DecodeQuery(Realms::EncodeQuery(queries).Bytes,decoded),"Duplicate IDs accepted");
        queries = {{1,18414}};
        auto route = Realms::Resolve(queries[0],{World("world",1,1)},100,true);
        std::vector<Realms::Route> routes;
        bytes = Realms::EncodeReply({route}).Bytes;
        Check(Realms::DecodeReply(bytes,queries,routes),"Reply roundtrip failed");
        for (std::size_t i = 0; i < bytes.size(); ++i)
            Check(!Realms::DecodeReply({bytes.begin(),bytes.begin()+i},queries,routes),"Truncated reply accepted");
        auto wrong = queries; wrong[0].Realm = 2;
        Check(!Realms::DecodeReply(bytes,wrong,routes),"Mismatched reply accepted");
        route.ValidFor = 3001; Check(!Realms::DecodeReply(Realms::EncodeReply({route}).Bytes,queries,routes),"Oversized TTL accepted");
        route.ValidFor = 3000;
        Realms::Cache cache; cache.Configure(true); cache.SetQueries(queries); cache.Store({route},100);
        Check(cache.Read(queries[0],3099).State == Realms::Status::Ready,"Fresh route missing");
        Check(cache.Read(queries[0],3100).State == Realms::Status::Offline,"Stale route survived deadline");
        cache.Store({route},100); cache.Clear();
        Check(cache.Read(queries[0],101).State == Realms::Status::Offline,"Disconnect retained endpoint");
        cache.Store({route},100); cache.SetQueries({{1,12345}});
        Check(cache.Read(queries[0],101).State == Realms::Status::Offline,"Changed identity retained endpoint");
    }
    void SelectionRace()
    {
        using namespace Handoff;
        auto auth = World("auth",0,2); auth.Type = Service::Auth; auth.Capabilities = 64 | 128;
        std::vector<Node> nodes = {auth,World("world",1,1)};
        Request request; request.Action = Operation::Issue; request.Bind.Use = Purpose::World; request.Bind.Account = 1;
        request.Bind.Realm = 1; request.Bind.Address = "127.0.0.1"; request.Bind.Evidence = std::string(64,'a'); request.Ttl = 60;
        Check(Authorize("auth",nodes,100,request) == Result::Unavailable,"Unbound selection accepted");
        request.ExpectedDestination = "world:1";
        Request decoded; Check(Decode(Encode(request).Bytes,decoded) && decoded.ExpectedDestination == "world:1","v2 binding lost");
        Check(Authorize("auth",nodes,100,request) == Result::Ok,"Current owner rejected");
        nodes[1].Owner = 3;
        Check(Authorize("auth",nodes,100,request) == Result::Unavailable,"Stale selected owner accepted after restart");
    }
}
int main()
{
    try { Routing(); WireAndCache(); SelectionRace(); std::cout << "Realm directory routing, maintenance, expiry, wire and selected-owner checks passed.\n"; }
    catch (std::exception const& e) { std::cerr << e.what() << '\n'; return 1; }
}
