/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "Cluster/ChatProtocol.h"
#include "Cluster/ClusterRegistry.h"
#include <stdexcept>

int main()
{
    using namespace Skyfire;
    auto check = [](bool ok) { if (!ok) throw std::runtime_error("Chat protocol invariant failed"); };
    std::array<std::uint8_t, Chat::ProbeSize> request{'S','F','C','H',0,1,0,1,0,0,0,7,0,0,0,2};
    std::uint32_t id = 0, realm = 0;
    check(Chat::DecodeProbe(request, id, realm) && id == 7 && realm == 2);
    for (unsigned position : {0u, 5u, 7u, 11u, 15u})
    {
        auto invalid = request; invalid[position] = 0;
        check(!Chat::DecodeProbe(invalid, id, realm));
    }
    Cluster::ChatMetrics metrics; metrics.Realms = {1, 2}; metrics.Connections = 128;
    auto encoded = Chat::EncodeMetrics(metrics);
    Cluster::ChatMetrics decoded;
    check(Chat::DecodeMetrics(encoded.Bytes, decoded) && decoded.Realms == metrics.Realms);
    auto legacy = encoded.Bytes; legacy[0] = 3; legacy.resize(legacy.size() - 12);
    check(Chat::DecodeMetrics(legacy, decoded) && decoded.RoutedMessages == 0 && decoded.RoutedControls == 0);
    legacy[0] = 2; legacy.resize(legacy.size() - 4);
    check(Chat::DecodeMetrics(legacy, decoded) && decoded.WhisperRelays == 0);
    legacy[0] = 1; legacy.resize(legacy.size() - 4);
    check(Chat::DecodeMetrics(legacy, decoded) && decoded.PresencePlayers == 0);
    for (std::size_t size = 0; size < encoded.Bytes.size(); ++size)
        check(!Chat::DecodeMetrics(std::vector<std::uint8_t>(encoded.Bytes.begin(), encoded.Bytes.begin() + size), decoded));
    encoded.Bytes.push_back(0); check(!Chat::DecodeMetrics(encoded.Bytes, decoded));
    for (auto realms : std::vector<std::vector<std::uint32_t>>{{}, {0}, {1,1}, std::vector<std::uint32_t>(65,1)})
    {
        metrics.Realms = realms;
        check(!Chat::DecodeMetrics(Chat::EncodeMetrics(metrics).Bytes, decoded));
    }
    metrics.Realms = {1}; metrics.Connections = 129;
    check(!Chat::DecodeMetrics(Chat::EncodeMetrics(metrics).Bytes, decoded));
    Cluster::Registry registry;
    Cluster::Node node; node.Key = "chat-test"; node.Type = Cluster::Service::Chat;
    check(registry.Register(node, 1, 0, 100));
    check(!registry.SetChatMetrics(node.Key, 2, 1, metrics));
    check(registry.SetChatMetrics(node.Key, 1, 1, metrics));
    check(!registry.SetChatMetrics(node.Key, 1, 100, metrics));
    node.Key = "world-test"; node.Type = Cluster::Service::World;
    check(registry.Register(node, 2, 0, 100));
    check(!registry.SetChatMetrics(node.Key, 2, 1, metrics));
    for (unsigned primaryRealm : {0u, 1u})
    {
        Cluster::Writer registration;
        registration.String("chat-test"); registration.String("Chat"); registration.U8(5);
        registration.String("127.0.0.1"); registration.U16(54940); registration.U32(primaryRealm);
        registration.U32(18414); registration.U32(32); registration.U32(Chat::Capability);
        check(Cluster::DecodeRegistration(registration.Bytes, node) == (primaryRealm == 0));
    }
}
