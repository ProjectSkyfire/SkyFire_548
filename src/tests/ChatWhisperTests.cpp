/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "Cluster/ChatWhisper.h"
#include <stdexcept>
int main()
{
    using namespace Skyfire::Chat;
    auto check = [](bool ok) { if (!ok) throw std::runtime_error("Chat whisper invariant failed"); };
    PresenceSnapshot snapshot{std::string(64, 'a'), 1, {{12, 42, 1, "Sender"}, {13, 43, 2, "Receiver"}}};
    PresenceDirectory directory;
    check(directory.Replace(1, "world-a", snapshot, 100));
    Whisper message; message.Generation = snapshot.Generation; message.Account = 12;
    message.Sender = 42; message.SenderIncarnation = 1; message.Receiver = 43; message.ReceiverIncarnation = 2;
    message.Text = "Hello";
    auto bytes = EncodeWhisper(message).Bytes;
    Whisper decoded;
    check(DecodeWhisper(bytes, decoded) && decoded.Text == "Hello");
    for (std::size_t size = 0; size < bytes.size(); ++size)
        check(!DecodeWhisper(std::vector<std::uint8_t>(bytes.begin(), bytes.begin() + size), decoded));
    bytes.push_back(0); check(!DecodeWhisper(bytes, decoded));
    auto changed = message; changed.Text = std::string(256, 'x');
    check(!DecodeWhisper(EncodeWhisper(changed).Bytes, decoded));
    changed.Text = ""; check(!DecodeWhisper(EncodeWhisper(changed).Bytes, decoded));
    changed.Text = "bad\nmessage"; check(!DecodeWhisper(EncodeWhisper(changed).Bytes, decoded));
    check(AuthorizeWhisper(directory, 1, "world-a", message, 101));
    check(!AuthorizeWhisper(directory, 2, "world-a", message, 101));
    check(!AuthorizeWhisper(directory, 1, "world-b", message, 101));
    check(!AuthorizeWhisper(directory, 1, "world-a", message, 15100));
    changed = message; changed.Account = 13;
    check(!AuthorizeWhisper(directory, 1, "world-a", changed, 101));
    changed = message; changed.Generation = std::string(64, 'b');
    check(!AuthorizeWhisper(directory, 1, "world-a", changed, 101));
    changed = message; changed.SenderIncarnation = 3;
    check(!AuthorizeWhisper(directory, 1, "world-a", changed, 101));
    changed = message; changed.ReceiverIncarnation = 3;
    check(!AuthorizeWhisper(directory, 1, "world-a", changed, 101));
    snapshot.Sequence = 2; snapshot.Players[1].Incarnation = 3;
    check(directory.Replace(1, "world-a", snapshot, 102));
    check(!AuthorizeWhisper(directory, 1, "world-a", message, 103));
    check(AuthorizeWhisper(directory, 1, "world-a", changed, 103));
    // Same GUIDs on another realm never satisfy the original realm's lookup.
    check(directory.Replace(2, "world-b", snapshot, 104));
    snapshot.Sequence = 3; snapshot.Players.pop_back();
    check(directory.Replace(1, "world-a", snapshot, 105));
    check(!AuthorizeWhisper(directory, 1, "world-a", changed, 106));
}
