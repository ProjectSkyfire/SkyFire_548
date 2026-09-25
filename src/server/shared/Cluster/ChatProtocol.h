/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#ifndef SKYFIRE_CHAT_PROTOCOL_H
#define SKYFIRE_CHAT_PROTOCOL_H
#include "ClusterProtocol.h"
#include <algorithm>

namespace Skyfire::Chat
{
    constexpr std::uint32_t Capability = 2048;
    constexpr Cluster::Message MetricsType = Cluster::Message(11);
    // A separate TLS endpoint. Operation 1 is a fixed-size health probe.
    // Operation 2 carries bounded presence snapshots; see ChatPresence.h.
    // Operation 3 relays bounded whispers; see ChatWhisper.h.
    // Operation 4 routes an authority-projected audience; see ChatRouting.h.
    // Client opcodes, commands and database mutations are not accepted.
    constexpr std::size_t ProbeSize = 16;
    inline bool DecodeProbe(std::array<std::uint8_t, ProbeSize> const& bytes, std::uint32_t& id, std::uint32_t& realm)
    {
        if (bytes[0] != 'S' || bytes[1] != 'F' || bytes[2] != 'C' || bytes[3] != 'H' ||
            bytes[4] != 0 || bytes[5] != 1 || bytes[6] != 0 || bytes[7] != 1) return false;
        id = (std::uint32_t(bytes[8]) << 24) | (std::uint32_t(bytes[9]) << 16) |
            (std::uint32_t(bytes[10]) << 8) | bytes[11];
        realm = (std::uint32_t(bytes[12]) << 24) | (std::uint32_t(bytes[13]) << 16) |
            (std::uint32_t(bytes[14]) << 8) | bytes[15];
        return id != 0 && realm != 0;
    }
    inline Cluster::Writer EncodeMetrics(Cluster::ChatMetrics const& m)
    {
        Cluster::Writer out; out.U8(4); out.U32(m.Uptime); out.U32(m.Connections);
        out.U32(m.Requests); out.U32(m.Failures); out.U16(std::uint16_t(m.Realms.size()));
        for (auto realm : m.Realms) out.U32(realm);
        out.U32(m.PresencePlayers); out.U32(m.WhisperRelays);
        out.U32(m.RoutedMessages); out.U32(m.RoutedRecipients); out.U32(m.RoutedControls);
        return out;
    }
    inline bool DecodeMetrics(std::vector<std::uint8_t> const& bytes, Cluster::ChatMetrics& m)
    {
        Cluster::Reader in(bytes); std::uint8_t version;
        std::uint16_t count;
        if (!in.U8(version) || (version < 1 || version > 4) || !in.U32(m.Uptime) || !in.U32(m.Connections) ||
            m.Connections > 128 || !in.U32(m.Requests) || !in.U32(m.Failures) ||
            !in.U16(count) || count == 0 || count > 64) return false;
        m.Realms.clear();
        for (unsigned i = 0; i < count; ++i)
        {
            std::uint32_t realm;
            if (!in.U32(realm) || !realm || std::find(m.Realms.begin(), m.Realms.end(), realm) != m.Realms.end()) return false;
            m.Realms.push_back(realm);
        }
        m.PresencePlayers = 0;
        if (version >= 2 && (!in.U32(m.PresencePlayers) || m.PresencePlayers > 16384)) return false;
        m.WhisperRelays = 0;
        if (version >= 3 && !in.U32(m.WhisperRelays)) return false;
        m.RoutedMessages = m.RoutedRecipients = m.RoutedControls = 0;
        if (version >= 4 && (!in.U32(m.RoutedMessages) || !in.U32(m.RoutedRecipients) || !in.U32(m.RoutedControls))) return false;
        return in.End();
    }
}
#endif
