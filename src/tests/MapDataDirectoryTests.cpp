/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "Cluster/MapDataDirectory.h"
#include "Cluster/MapDataStartCheck.h"
#include <stdexcept>
using namespace Skyfire::Cluster;
int main()
{
    auto check = [](bool ok) { if (!ok) throw std::runtime_error("Map directory invariant failed"); };
    Node east; east.Key = "east"; east.Type = Service::Map; east.Capabilities = 256;
    east.Ready = true; east.Build = 18414; east.ExpiresAt = 5000; east.Address = "127.0.0.1"; east.Port = 54910;
    Node west = east; west.Key = "west"; west.Port = 54911;
    auto available = [](std::string const& key, std::vector<Node> const& nodes, std::uint64_t now)
    { return MapData::Resolve(key, nodes, now).Bytes.at(1) == 1; };
    check(available("east", {east,west}, 1)); check(available("west", {east,west}, 1));
    check(!available("absent", {east,west}, 1)); check(!available("east", {east}, 5000));
    east.Admin = Administration::Draining; check(!available("east", {east}, 1));
    east.Admin = Administration::Enabled; east.Ready = false; check(!available("east", {east}, 1));
    east.Ready = true; east.Live = false; check(!available("east", {east}, 1));
    east.Live = true; east.Capacity = 1; east.Load = 1; check(!available("east", {east}, 1));
    east.Load = 0; east.Type = Service::World; check(!available("east", {east}, 1));
    auto start = [&](std::string const& config, std::vector<Node> const& nodes)
    { std::istringstream input(config); std::string error; return MapData::CheckWorldStart(input,nodes,1,error); };
    check(start("MapData.Enable=0", {}));
    check(!start("MapData.Enable=1\nMapData.Sources=west=1", {}));
    check(start("MapData.Enable=true # enabled\nMapData.Sources='west=1'", {west}));
    check(!start("MapData.Enable=1\nMapData.Sources=west=1 missing=0", {west}));
    check(!start("MapData.Enable=1\nMapData.Sources=west=1 west=0", {west}));
    check(!start("MapData.Enable=1\nMapData.Sources=west=1 east=1", {east,west}));
    check(!start("MapData.Enable=1\nMapData.Sources=", {west}));
    Writer query; query.U8(1); query.String("east"); std::string key;
    check(MapData::DecodeQuery(query.Bytes,key) && key == "east");
    query.U8(0); check(!MapData::DecodeQuery(query.Bytes,key));
    Writer registration; registration.String("east"); registration.String("Eastern maps"); registration.U8(3);
    registration.String("127.0.0.1"); registration.U16(54910); registration.U32(0);
    registration.U32(18414); registration.U32(4); registration.U32(256); Node decoded;
    check(DecodeRegistration(registration.Bytes,decoded) && decoded.Type == Service::Map && decoded.Realms.empty());
}
