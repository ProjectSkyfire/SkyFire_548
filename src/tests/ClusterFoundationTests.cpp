/*
 * This file is part of Project SkyFire https://www.projectskyfire.org.
 * See LICENSE.md file for Copyright information.
 */
#include "Cluster/ClusterRegistry.h"
#include <algorithm>
#include <iostream>
using namespace Skyfire::Cluster;
namespace
{
    bool Check(bool value, char const* message)
    {
        if (!value) std::cerr << message << '\n';
        return value;
    }
    Writer Registration()
    {
        Writer out;
        out.String("world-a"); out.String("World A"); out.U8(2); out.String("127.0.0.1");
        out.U16(8085); out.U32(1); out.U32(18414); out.U32(100); out.U32(7);
        return out;
    }
}
int main()
{
    bool ok = true;
    for (std::uint32_t realm : {0u, 1u})
    {
        Writer character;
        character.String("characters-1"); character.String("Characters"); character.U8(4);
        character.String("127.0.0.1"); character.U16(54930); character.U32(realm);
        character.U32(18414); character.U32(32); character.U32(1024);
        Node decodedCharacter;
        bool accepted = DecodeRegistration(character.Bytes, decodedCharacter);
        ok &= Check(accepted == (realm == 0), "Character directory registration realm validation failed");
        if (accepted)
            ok &= Check(decodedCharacter.Type == Service::Character && decodedCharacter.Capabilities == 1024,
                "Character service registration lost its role or capability");
    }
    auto payload = Registration();
    Node node;
    ok &= Check(DecodeRegistration(payload.Bytes, node), "Valid registration rejected");
    ok &= Check(node.Key == "world-a" && node.Port == 8085 && node.Realm == 1 && node.Capacity == 100,
        "Registration fields or byte order are incorrect");
    for (std::size_t length = 0; length < payload.Bytes.size(); ++length)
    {
        auto truncated = payload.Bytes; truncated.resize(length);
        Node rejected;
        ok &= Check(!DecodeRegistration(truncated, rejected), "Partial registration accepted");
    }
    auto trailing = payload.Bytes; trailing.push_back(0);
    ok &= Check(!DecodeRegistration(trailing, node), "Trailing registration bytes accepted");
    auto excessiveString = payload.Bytes; excessiveString[0] = 0xff; excessiveString[1] = 0xff;
    ok &= Check(!DecodeRegistration(excessiveString, node), "Oversized string accepted");
    auto embeddedNull = payload.Bytes; embeddedNull[2] = 0;
    ok &= Check(!DecodeRegistration(embeddedNull, node), "Embedded NUL accepted");
    ok &= Check(!ValidKey("../world") && !ValidKey("") && !ValidKey(std::string(65, 'a')),
        "Invalid node identity accepted");
    ok &= Check(!ValidUtf8(std::string("\xc0\x80", 2)) && !ValidUtf8(std::string("\xed\xa0\x80", 3)) &&
        ValidUtf8(std::string("\xc3\xa9", 2)), "UTF-8 validation failed");
    auto frame = Frame(Message::Register, payload);
    std::array<std::uint8_t, HeaderSize> headerBytes{};
    std::copy_n(frame.begin(), HeaderSize, headerBytes.begin());
    Header header;
    ok &= Check(DecodeHeader(headerBytes, header) && header.Version == 1 && header.Type == Message::Register &&
        header.Length == payload.Bytes.size(), "Header decoding failed");
    headerBytes[0] = 'X';
    ok &= Check(!DecodeHeader(headerBytes, header), "Bad magic accepted");
    headerBytes[0] = 'S'; headerBytes[5] = 2;
    ok &= Check(DecodeHeader(headerBytes, header) && header.Version != ProtocolVersion,
        "Incompatible version cannot be diagnosed");
    headerBytes[8] = 1;
    ok &= Check(!DecodeHeader(headerBytes, header), "Oversized frame accepted");
    Writer huge; huge.Bytes.resize(MaximumPayload + 1);
    ok &= Check(Frame(Message::Register, huge).empty(), "Oversized outgoing frame accepted");
    Node valid;
    DecodeRegistration(payload.Bytes, valid);
    Registry registry(2);
    ok &= Check(registry.Register(valid, 1, 100, 15000), "Registration failed");
    ok &= Check(!registry.Snapshot()[0].Ready, "Registration incorrectly implies readiness");
    ok &= Check(!registry.Register(valid, 2, 100, 15000), "Duplicate live identity accepted");
    ok &= Check(!registry.Renew(valid.Key, 2, 200, 15000, 3, 1), "Other connection renewed lease");
    ok &= Check(!registry.Remove(valid.Key, 2), "Other connection removed lease");
    ok &= Check(registry.Renew(valid.Key, 1, 200, 15000, 3, 1) && registry.Snapshot()[0].Ready,
        "Readiness transition failed");
    ok &= Check(registry.Renew(valid.Key, 1, 300, 15000, 4) && registry.Snapshot()[0].Ready &&
        registry.Snapshot()[0].Load == 4, "Heartbeat failed to preserve readiness and update load");
    ok &= Check(registry.Renew(valid.Key, 1, 400, 15000, 4, 0) && !registry.Snapshot()[0].Ready,
        "Not-ready transition failed");
    auto second = valid; second.Key = "world-b";
    ok &= Check(!registry.Register(second, 1, 500, 15000), "One connection registered multiple identities");
    ok &= Check(registry.Register(second, 2, 500, 15000), "Second node failed");
    auto third = valid; third.Key = "world-c";
    ok &= Check(!registry.Register(third, 3, 500, 15000), "Registry capacity exceeded");
    registry.Expire(15400);
    ok &= Check(registry.Snapshot().size() == 1, "Lease did not expire at its deadline");
    ok &= Check(!registry.Renew(second.Key, 2, 15500, 15000, 0), "Expired lease revived by heartbeat");
    ok &= Check(registry.Register(valid, 3, 15500, 15000), "Fresh registration after expiry failed");
    ok &= Check(!registry.Remove(valid.Key, 1) && registry.Snapshot().size() == 1,
        "Stale disconnect deleted replacement registration");
    ok &= Check(registry.Remove(valid.Key, 3) && registry.Snapshot().empty(), "Deregistration failed");
    registry.Register(valid, 4, 16000, 15000);
    registry.Clear();
    ok &= Check(registry.Snapshot().empty(), "Shutdown failed to clear registry");
    valid.Capabilities = 8;
    registry.Register(valid, 5, 0, 100);
    ok &= Check(!registry.SetRealms(valid.Key, 6, 1, 100, {1, 2}), "Another owner changed realms");
    ok &= Check(!registry.SetRealms(valid.Key, 5, 1, 100, {2}), "Missing primary realm accepted");
    ok &= Check(!registry.SetRealms(valid.Key, 5, 1, 100, {1, 1}), "Duplicate realms accepted");
    ok &= Check(!registry.SetRealms(valid.Key, 5, 1, 100, {1, 0}), "Zero realm accepted");
    ok &= Check(!registry.SetRealms(valid.Key, 5, 1, 100, {}), "Empty realm list accepted");
    ok &= Check(registry.SetRealms(valid.Key, 5, 1, 100, {1, 2}) && registry.Snapshot()[0].Realms.size() == 2 &&
        !registry.Snapshot()[0].Ready, "Realm list failed or implied readiness");
    ok &= Check(!registry.SetRealms(valid.Key, 5, 101, 100, {1, 2}), "Expired lease renewed by realm list");
    Writer realms; realms.U16(2); realms.U32(1); realms.U32(2);
    std::vector<std::uint32_t> decoded;
    ok &= Check(DecodeRealms(realms.Bytes, decoded) && decoded == std::vector<std::uint32_t>({1, 2}), "Realm byte order incorrect");
    for (std::size_t size = 0; size < realms.Bytes.size(); ++size)
    {
        auto truncated = realms.Bytes; truncated.resize(size);
        ok &= Check(!DecodeRealms(truncated, decoded), "Partial realm list accepted");
    }
    realms.Bytes.push_back(0);
    ok &= Check(!DecodeRealms(realms.Bytes, decoded), "Trailing realm bytes accepted");
    realms.Bytes[1] = 65;
    ok &= Check(!DecodeRealms(realms.Bytes, decoded), "Excessive realm count accepted");
    Registry maintenance;
    auto auth = valid; auth.Type = Service::Auth; auth.Realm = 0; auth.Realms.clear(); auth.Capabilities = 16;
    maintenance.Register(auth,10,0,100);
    ok &= Check(maintenance.SetAdministration(auth.Key,Administration::Draining),"Drain policy rejected");
    maintenance.Renew(auth.Key,10,1,100,7,1);
    ok &= Check(maintenance.Snapshot()[0].Ready && maintenance.Snapshot()[0].Admin == Administration::Draining,
        "Heartbeat overwrote maintenance policy or readiness was conflated with policy");
    maintenance.Remove(auth.Key,10);
    auth.Admin = Administration::Enabled;
    maintenance.Register(auth,11,2,100);
    ok &= Check(maintenance.Snapshot()[0].Admin == Administration::Draining && !maintenance.Snapshot()[0].Ready,
        "Reconnect bypassed drain or inherited readiness");
    maintenance.Clear();
    maintenance.Register(auth,12,3,100);
    ok &= Check(maintenance.Snapshot()[0].Admin == Administration::Draining,"Clearing live leases discarded policy");
    maintenance.SetAdministration(auth.Key,Administration::Disabled);
    Registry restarted;
    restarted.SetAdministration(auth.Key,maintenance.Snapshot()[0].Admin); // Persisted-policy replay on hub startup.
    restarted.Register(auth,1,4,100);
    ok &= Check(restarted.Snapshot()[0].Admin == Administration::Disabled,"Policy replay failed after hub restart");
    restarted.SetAdministration(auth.Key,Administration::Enabled);
    ok &= Check(restarted.Snapshot()[0].Admin == Administration::Enabled,"Re-enable did not clear maintenance");
    ok &= Check(!restarted.SetAdministration("../bad",Administration::Draining) &&
        !restarted.SetAdministration(auth.Key,Administration(3)),"Invalid policy accepted");
    Registry worlds;
    worlds.Register(valid,1,0,100); worlds.Renew(valid.Key,1,1,100,12,1);
    worlds.Expire(101);
    ok &= Check(worlds.Snapshot().empty(),"Expired world stayed in live directory");
    worlds.Register(valid,2,102,100);
    ok &= Check(worlds.Snapshot().size() == 1 && !worlds.Snapshot()[0].Ready && worlds.Snapshot()[0].Load == 0,
        "Recovered world inherited stale readiness/load");
    worlds.SetRealms(valid.Key,2,103,100,{1,3}); worlds.Renew(valid.Key,2,104,100,1,1);
    ok &= Check(worlds.Snapshot()[0].Ready && worlds.Snapshot()[0].Realms == std::vector<std::uint32_t>({1,3}),
        "Recovered world failed to republish its live realm list");
    return ok ? 0 : 1;
}
