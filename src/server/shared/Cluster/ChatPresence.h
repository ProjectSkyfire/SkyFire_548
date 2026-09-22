/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#ifndef SKYFIRE_CHAT_PRESENCE_H
#define SKYFIRE_CHAT_PRESENCE_H
#include "ChatProtocol.h"
#include <map>
#include <set>

namespace Skyfire::Chat
{
    constexpr std::size_t MaxPresencePlayers = 4096;
    constexpr std::size_t MaxPresenceBytes = 512 * 1024;
    constexpr std::uint64_t PresenceLeaseMs = 15000;
    struct PlayerPresence
    {
        std::uint32_t Account = 0;
        std::uint64_t Guid = 0, Incarnation = 0;
        std::string Name;
    };
    struct PresenceSnapshot
    {
        std::string Generation;
        std::uint64_t Sequence = 0;
        std::vector<PlayerPresence> Players;
    };
    inline void Write64(Cluster::Writer& out, std::uint64_t value)
    { out.U32(std::uint32_t(value >> 32)); out.U32(std::uint32_t(value)); }
    inline bool Read64(Cluster::Reader& in, std::uint64_t& value)
    {
        std::uint32_t hi, lo;
        if (!in.U32(hi) || !in.U32(lo)) return false;
        value = (std::uint64_t(hi) << 32) | lo; return true;
    }
    inline Cluster::Writer EncodePresence(PresenceSnapshot const& snapshot)
    {
        Cluster::Writer out; out.String(snapshot.Generation); Write64(out, snapshot.Sequence);
        out.U16(std::uint16_t(snapshot.Players.size()));
        for (auto const& player : snapshot.Players)
        { out.U32(player.Account); Write64(out, player.Guid); Write64(out, player.Incarnation); out.String(player.Name); }
        return out;
    }
    inline bool DecodePresence(std::vector<std::uint8_t> const& bytes, PresenceSnapshot& snapshot)
    {
        if (bytes.size() > MaxPresenceBytes) return false;
        Cluster::Reader in(bytes); std::uint16_t count;
        PresenceSnapshot decoded;
        if (!in.String(decoded.Generation, 64) || decoded.Generation.size() != 64 ||
            decoded.Generation.find_first_not_of("0123456789abcdef") != std::string::npos ||
            !Read64(in, decoded.Sequence) || !decoded.Sequence || !in.U16(count) || count > MaxPresencePlayers) return false;
        std::set<std::uint64_t> guids;
        std::set<std::string> names;
        for (unsigned i = 0; i < count; ++i)
        {
            PlayerPresence player;
            if (!in.U32(player.Account) || !player.Account || !Read64(in, player.Guid) || !player.Guid ||
                !Read64(in, player.Incarnation) || !player.Incarnation || !in.String(player.Name, 48) ||
                !guids.insert(player.Guid).second || !names.insert(player.Name).second) return false;
            decoded.Players.push_back(std::move(player));
        }
        if (!in.End()) return false;
        snapshot = std::move(decoded); return true;
    }
    // Main-thread owned. A reconnecting world cannot replace an unexpired generation.
    // This is a presence lease, not authorization to write characters or execute commands.
    class PresenceDirectory
    {
        struct Entry { PresenceSnapshot Snapshot; std::uint64_t Expires; };
        std::map<std::pair<std::uint32_t, std::string>, Entry> _entries;
    public:
        void Expire(std::uint64_t now)
        {
            for (auto it = _entries.begin(); it != _entries.end();)
                if (now >= it->second.Expires) it = _entries.erase(it); else ++it;
        }
        bool Replace(std::uint32_t realm, std::string const& node, PresenceSnapshot snapshot, std::uint64_t now)
        {
            Expire(now);
            if (!realm || !Cluster::ValidKey(node) || snapshot.Players.size() > MaxPresencePlayers) return false;
            auto key = std::make_pair(realm, node);
            auto old = _entries.find(key);
            if (old != _entries.end() && (old->second.Snapshot.Generation != snapshot.Generation ||
                old->second.Snapshot.Sequence >= snapshot.Sequence)) return false;
            if (old == _entries.end() && _entries.size() >= 128) return false;
            std::size_t total = snapshot.Players.size();
            std::set<std::uint64_t> guids;
            std::set<std::string> names;
            for (auto const& entry : _entries)
            {
                if (entry.first == key) continue;
                total += entry.second.Snapshot.Players.size();
                if (entry.first.first == realm)
                    for (auto const& player : entry.second.Snapshot.Players)
                    { guids.insert(player.Guid); names.insert(player.Name); }
            }
            if (total > 16384) return false;
            for (auto const& player : snapshot.Players)
                if (!guids.insert(player.Guid).second || !names.insert(player.Name).second) return false;
            _entries[key] = {std::move(snapshot), now + PresenceLeaseMs}; return true;
        }
        PlayerPresence const* Find(std::uint32_t realm, std::string const& node, std::string const& generation,
            std::uint64_t guid, std::uint64_t incarnation, std::uint64_t now) const
        {
            auto entry = _entries.find({realm, node});
            if (entry == _entries.end() || now >= entry->second.Expires || entry->second.Snapshot.Generation != generation) return nullptr;
            for (auto const& player : entry->second.Snapshot.Players)
                if (player.Guid == guid && player.Incarnation == incarnation) return &player;
            return nullptr;
        }
        std::uint32_t Players() const
        {
            std::uint32_t count = 0;
            for (auto const& entry : _entries) count += std::uint32_t(entry.second.Snapshot.Players.size());
            return count;
        }
    };
}
#endif
