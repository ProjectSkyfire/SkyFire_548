/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "Cluster/ChatPresence.h"
#include <stdexcept>
int main()
{
    using namespace Skyfire::Chat;
    auto check = [](bool ok) { if (!ok) throw std::runtime_error("Chat presence invariant failed"); };
    PresenceSnapshot snapshot{std::string(64, 'a'), 1, {{12, 42, 1, "Player"}}};
    auto payload = EncodePresence(snapshot);
    PresenceSnapshot decoded;
    check(DecodePresence(payload.Bytes, decoded) && decoded.Players[0].Guid == 42);
    for (std::size_t size = 0; size < payload.Bytes.size(); ++size)
        check(!DecodePresence(std::vector<std::uint8_t>(payload.Bytes.begin(), payload.Bytes.begin() + size), decoded));
    payload.Bytes.push_back(0); check(!DecodePresence(payload.Bytes, decoded));
    auto invalid = snapshot; invalid.Players.push_back(invalid.Players.front());
    check(!DecodePresence(EncodePresence(invalid).Bytes, decoded));
    invalid = snapshot; invalid.Players[0].Incarnation = 0;
    check(!DecodePresence(EncodePresence(invalid).Bytes, decoded));
    invalid = snapshot; invalid.Generation = "not-a-generation";
    check(!DecodePresence(EncodePresence(invalid).Bytes, decoded));
    PresenceDirectory directory;
    check(directory.Replace(1, "world-a", snapshot, 100));
    check(!directory.Replace(1, "world-a", snapshot, 101)); // No replay refreshes a lease.
    check(directory.Replace(2, "world-b", snapshot, 100)); // Same GUID/name in another realm.
    check(!directory.Replace(1, "world-b", snapshot, 102)); // No same-realm duplicate owner.
    auto replacement = snapshot; replacement.Generation = std::string(64, 'b');
    check(!directory.Replace(1, "world-a", replacement, 103)); // Old generation is still live.
    snapshot.Sequence = 2; snapshot.Players[0].Incarnation = 2;
    check(directory.Replace(1, "world-a", snapshot, 104)); // Relogged session replaces old incarnation.
    directory.Expire(15100);
    check(directory.Players() == 1); // Realm 2 expired independently.
    check(!directory.Replace(1, "world-a", replacement, 15103));
    check(directory.Replace(1, "world-a", replacement, 15104));
    replacement.Sequence = 2; replacement.Players.clear();
    check(directory.Replace(1, "world-a", replacement, 15105)); // Logout snapshot.
    check(directory.Players() == 0);
    directory.Expire(30105); // Crash/transport loss leaves no live presence.
    check(directory.Players() == 0);
}
