/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#ifndef SKYFIRE_CHAT_ROUTING_H
#define SKYFIRE_CHAT_ROUTING_H
#include "ChatPresence.h"
#include <deque>

namespace Skyfire::Chat
{
    // Membership projections come from the authenticated gameplay authority, never
    // from a client-supplied group/guild identifier. Separate audiences preserve
    // subgroup, officer, addon-prefix and channel-moderator restrictions.
    enum class AudienceKind : std::uint8_t
    {
        Party = 1, Raid, RaidWarning, Instance, Guild, Officer, Channel, Spatial, Whisper,
        ChannelControl, GuildControl, Command
    };
    struct RouteMember
    {
        std::uint64_t Guid = 0, Incarnation = 0;
        bool Speak = false, Listen = false;
    };
    struct AudienceProjection
    {
        std::string Generation, Key;
        AudienceKind Kind = AudienceKind::Party;
        std::uint64_t Revision = 0;
        std::vector<RouteMember> Members;
    };
    struct RoutedMessage
    {
        std::string Generation, Audience, Text, Prefix;
        std::uint64_t Sequence = 0, Revision = 0, Sender = 0, Incarnation = 0;
        std::uint32_t Account = 0, Language = 0;
    };
    struct RoutedDelivery
    {
        RoutedMessage Message;
        std::vector<RouteMember> Recipients;
        std::uint64_t Expires = 0;
    };
    constexpr std::size_t MaxRoutingBytes = 80 * 1024;
    // Addon payloads are bytes, not necessarily printable UTF-8. Do not pass them
    // through the cluster identity-string decoder or silently rewrite them.
    inline bool ReadRoutingBytes(Cluster::Reader& in, std::string& text, std::size_t maximum)
    {
        std::uint16_t length;
        if (!in.U16(length) || length > maximum) return false;
        text.clear(); text.reserve(length);
        for (unsigned i = 0; i < length; ++i)
        { std::uint8_t byte; if (!in.U8(byte)) return false; text.push_back(char(byte)); }
        return true;
    }
    inline Cluster::Writer EncodeRoute(AudienceProjection const& projection, RoutedMessage const& message)
    {
        Cluster::Writer out; out.String(projection.Generation); out.String(projection.Key);
        out.U8(std::uint8_t(projection.Kind)); Write64(out, projection.Revision);
        out.U16(std::uint16_t(projection.Members.size()));
        for (auto const& member : projection.Members)
        {
            Write64(out, member.Guid); Write64(out, member.Incarnation);
            out.U8((member.Speak ? 1 : 0) | (member.Listen ? 2 : 0));
        }
        Write64(out, message.Sequence); Write64(out, message.Sender); Write64(out, message.Incarnation);
        out.U32(message.Account); out.U32(message.Language); out.String(message.Text);
        out.U8(message.Prefix.empty() ? 0 : 1); if (!message.Prefix.empty()) out.String(message.Prefix);
        return out;
    }
    inline bool DecodeRoute(std::vector<std::uint8_t> const& bytes, AudienceProjection& projection, RoutedMessage& message)
    {
        if (bytes.size() > MaxRoutingBytes) return false;
        Cluster::Reader in(bytes); AudienceProjection p; RoutedMessage m;
        std::uint8_t kind, prefix; std::uint16_t count;
        if (!in.String(p.Generation, 64) || !in.String(p.Key, 64) || !in.U8(kind) ||
            kind < 1 || kind > 12 || !Read64(in, p.Revision) || !in.U16(count) || count > 4096) return false;
        p.Kind = AudienceKind(kind);
        for (unsigned i = 0; i < count; ++i)
        {
            RouteMember member; std::uint8_t flags;
            if (!Read64(in, member.Guid) || !Read64(in, member.Incarnation) || !in.U8(flags) || flags > 3) return false;
            member.Speak = (flags & 1) != 0; member.Listen = (flags & 2) != 0; p.Members.push_back(member);
        }
        if (!Read64(in, m.Sequence) || !Read64(in, m.Sender) || !Read64(in, m.Incarnation) ||
            !in.U32(m.Account) || !in.U32(m.Language) || !ReadRoutingBytes(in, m.Text, 511) ||
            !in.U8(prefix) || prefix > 1 || (prefix && (!ReadRoutingBytes(in, m.Prefix, 16) || m.Prefix.empty())) || !in.End()) return false;
        m.Generation = p.Generation; m.Audience = p.Key; m.Revision = p.Revision;
        projection = std::move(p); message = std::move(m); return true;
    }
    inline Cluster::Writer EncodeRecipients(std::vector<RouteMember> const& recipients)
    {
        Cluster::Writer out; out.U16(std::uint16_t(recipients.size()));
        for (auto const& member : recipients) { Write64(out, member.Guid); Write64(out, member.Incarnation); }
        return out;
    }
    inline bool DecodeRecipients(std::vector<std::uint8_t> const& bytes, std::vector<RouteMember>& recipients)
    {
        Cluster::Reader in(bytes); std::uint16_t count; std::set<std::uint64_t> unique;
        if (!in.U16(count) || count > 4096) return false;
        std::vector<RouteMember> decoded;
        for (unsigned i = 0; i < count; ++i)
        {
            RouteMember member;
            if (!Read64(in, member.Guid) || !member.Guid || !Read64(in, member.Incarnation) || !member.Incarnation ||
                !unique.insert(member.Guid).second) return false;
            decoded.push_back(member);
        }
        if (!in.End()) return false;
        recipients = std::move(decoded); return true;
    }

    // All methods run on the daemon's event loop. A route is fenced by realm,
    // certificate identity, world generation, audience revision and player session.
    // The world must recheck current recipient permissions before client delivery.
    class MessageRouter
    {
        using Owner = std::pair<std::uint32_t, std::string>;
        struct Audience { AudienceProjection Projection; std::uint64_t Expires; };
        struct Node
        {
            std::string Generation;
            std::map<std::string, Audience> Audiences;
            std::uint64_t LastSequence = 0, Expires = 0;
            std::deque<RoutedDelivery> Deliveries;
        };
        std::map<Owner, Node> _nodes;
    public:
        static constexpr std::size_t MaxAudiences = 1024, MaxMembers = 4096, MaxQueued = 128;
        static constexpr std::uint64_t DeliveryLifetime = 5000;

        void Expire(std::uint64_t now)
        {
            for (auto it = _nodes.begin(); it != _nodes.end();)
            {
                if (now >= it->second.Expires) { it = _nodes.erase(it); continue; }
                auto& node = it->second;
                for (auto audience = node.Audiences.begin(); audience != node.Audiences.end();)
                    if (now >= audience->second.Expires) audience = node.Audiences.erase(audience); else ++audience;
                while (!node.Deliveries.empty() && now >= node.Deliveries.front().Expires) node.Deliveries.pop_front();
                ++it;
            }
        }

        bool Project(PresenceDirectory const& presence, std::uint32_t realm,
            std::string const& owner, AudienceProjection projection, std::uint64_t now)
        {
            Expire(now);
            if (!realm || !Cluster::ValidKey(owner) || !Cluster::ValidKey(projection.Key) ||
                projection.Generation.size() != 64 || projection.Generation.find_first_not_of("0123456789abcdef") != std::string::npos ||
                !projection.Revision || projection.Members.size() > MaxMembers ||
                projection.Kind < AudienceKind::Party || projection.Kind > AudienceKind::Command) return false;
            std::set<std::uint64_t> members;
            for (auto const& member : projection.Members)
                if (!members.insert(member.Guid).second || !presence.Find(realm, owner, projection.Generation,
                    member.Guid, member.Incarnation, now)) return false;
            Owner key{realm, owner};
            auto existing = _nodes.find(key);
            std::size_t totalMembers = projection.Members.size();
            for (auto const& node : _nodes)
                for (auto const& audience : node.second.Audiences)
                    if (node.first != key || audience.first != projection.Key)
                        totalMembers += audience.second.Projection.Members.size();
            if (totalMembers > 65536) return false;
            if (existing == _nodes.end() && _nodes.size() >= 128) return false;
            if (existing != _nodes.end() && existing->second.Generation != projection.Generation) return false;
            if (existing != _nodes.end())
            {
                auto old = existing->second.Audiences.find(projection.Key);
                if (old != existing->second.Audiences.end() &&
                    (old->second.Projection.Kind != projection.Kind || old->second.Projection.Revision >= projection.Revision)) return false;
                if (old == existing->second.Audiences.end() && existing->second.Audiences.size() >= MaxAudiences) return false;
            }
            auto& node = _nodes[key];
            node.Generation = projection.Generation; node.Expires = now + PresenceLeaseMs;
            std::string audienceKey = projection.Key;
            node.Audiences[audienceKey] = {std::move(projection), now + PresenceLeaseMs};
            return true;
        }

        bool Route(PresenceDirectory const& presence, std::uint32_t realm,
            std::string const& owner, RoutedMessage message, std::uint64_t now)
        {
            Expire(now);
            auto found = _nodes.find({realm, owner});
            if (found == _nodes.end() || found->second.Generation != message.Generation || !message.Sequence ||
                message.Sequence <= found->second.LastSequence || message.Text.size() > 511 || message.Prefix.size() > 16 ||
                (message.Language != 0xffffffffu && (message.Text.empty() || !Cluster::ValidUtf8(message.Text) || !message.Prefix.empty()))) return false;
            auto& node = found->second;
            auto audience = node.Audiences.find(message.Audience);
            if (audience == node.Audiences.end() || audience->second.Projection.Revision != message.Revision ||
                node.Deliveries.size() >= MaxQueued) return false;
            auto const* sender = presence.Find(realm, owner, message.Generation, message.Sender, message.Incarnation, now);
            if (!sender || sender->Account != message.Account) return false;
            bool canSpeak = false;
            if (audience->second.Projection.Kind >= AudienceKind::ChannelControl &&
                (audience->second.Projection.Members.size() != 1 || !message.Prefix.empty() || message.Language)) return false;
            RoutedDelivery delivery;
            for (auto const& member : audience->second.Projection.Members)
            {
                if (member.Guid == message.Sender && member.Incarnation == message.Incarnation) canSpeak = member.Speak;
                if (member.Listen && presence.Find(realm, owner, message.Generation, member.Guid, member.Incarnation, now))
                    delivery.Recipients.push_back(member);
            }
            if (!canSpeak) return false;
            node.LastSequence = message.Sequence;
            delivery.Message = std::move(message); delivery.Expires = now + DeliveryLifetime;
            node.Deliveries.push_back(std::move(delivery)); return true;
        }

        // Destructive read is intentionally at-most-once. Transport must not retry
        // a lost batch as a new message; expired chat is never replayed after login.
        std::vector<RoutedDelivery> Take(std::uint32_t realm, std::string const& owner,
            std::string const& generation, std::uint64_t now)
        {
            Expire(now);
            std::vector<RoutedDelivery> result;
            auto found = _nodes.find({realm, owner});
            if (found == _nodes.end() || found->second.Generation != generation) return result;
            while (!found->second.Deliveries.empty() && result.size() < 32)
            {
                auto delivery = std::move(found->second.Deliveries.front()); found->second.Deliveries.pop_front();
                auto audience = found->second.Audiences.find(delivery.Message.Audience);
                if (audience == found->second.Audiences.end() ||
                    audience->second.Projection.Revision != delivery.Message.Revision) continue;
                result.push_back(std::move(delivery));
            }
            return result;
        }
    };
}
#endif
