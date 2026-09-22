/*
 * This file is part of Project SkyFire https://www.projectskyfire.org.
 * See LICENSE.md file for Copyright information.
 */
#ifndef SKYFIRE_CLUSTER_PROTOCOL_H
#define SKYFIRE_CLUSTER_PROTOCOL_H

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace Skyfire::Cluster
{
    // TLS transport. Integers are big endian; strings are uint16 byte length + UTF-8.
    // Header: SFHC (4), protocol version (2), message type (2), payload bytes (4).
    // Register: key, name, type(u8), numeric address, port(u16), realm/build/capacity(u32), capabilities(u32).
    // Capability bits: 1 world commands, 2 account administration, 4 world metrics, 8 realm list.
    // Bit 16 identifies an Authnet endpoint; auth nodes without it advertise legacy authentication.
    // Bit 32 identifies an auth endpoint requiring trusted PROXY v1 client-address metadata.
    // Bit 64 opts into hub-owned shared handoff enforcement (subprotocol v1, message 6).
    // Bit 256 identifies read-only map data providers (service type 3, realm 0).
    // Bit 128 opts auth nodes into live realm queries (subprotocol v1, message 7).
    // Realms: count(u16, 1..64), unique nonzero realm IDs(u32), including the primary realm.
    // Nodes advertising bit 8 must send Realms before readiness; older peers use the primary realm.
    // Ready: ready(u8), load(u32). Heartbeat: load(u32). Deregister: empty.
    // Map metrics (message 9, registered map providers only): v1(u8), uptime/CPU basis points/
    // resident MiB/requests/failures/sent KiB/assets/active transfers(u32), map count(u16), IDs(u32).
    // Version 2 appends a process-generation string (32 hex characters); restart capability is bit 512.
    // Ack: request type(u16), lease seconds(u32). Error: code(u16), description(string).
    constexpr std::uint16_t ProtocolVersion = 1;
    constexpr std::size_t HeaderSize = 12;
    constexpr std::uint32_t MaximumPayload = 4096;
    enum class Message : std::uint16_t { Register = 1, Ready = 2, Heartbeat = 3, Deregister = 4, Realms = 5, Ack = 0x8000, Error = 0xffff };
    // Character: capability 1024. Chat: capability 2048, realm coverage in metrics.
    // Both register with realm 0; only world nodes publish authentication realm routes.
    enum class Service : std::uint8_t { Auth = 1, World = 2, Map = 3, Character = 4, Chat = 5 };
    // Hub-owned policy, never accepted from a node registration or heartbeat.
    enum class Administration : std::uint8_t { Enabled = 0, Draining = 1, Disabled = 2 };
    inline char const* AdministrationName(Administration state)
    {
        return state == Administration::Draining ? "draining" : state == Administration::Disabled ? "disabled" : "enabled";
    }
    enum class Error : std::uint16_t { Malformed = 1, Version = 2, Identity = 3, Conflict = 4, NotRegistered = 5, Capacity = 6 };
    struct Header { std::uint16_t Version = 0; Message Type = Message::Error; std::uint32_t Length = 0; };
    struct MapMetrics
    {
        std::string Generation;
        std::uint32_t Uptime = 0, CpuBasisPoints = 0, MemoryMiB = 0, Requests = 0, Failures = 0,
            SentKiB = 0, Assets = 0, Active = 0;
        std::vector<std::uint32_t> Maps;
        std::uint64_t ReceivedAt = 0;
    };
    struct CharacterMetrics
    {
        std::uint32_t Uptime = 0, CpuBasisPoints = 0, MemoryMiB = 0, Requests = 0, Failures = 0,
            Reads = 0, Writes = 0, Transactions = 0, Connections = 0, Pending = 0,
            LatencyUs = 0, LastCommitAge = 0xffffffffu, DatabaseReady = 0;
        std::uint64_t ReceivedAt = 0;
    };
    struct ChatMetrics
    {
        std::uint32_t Uptime = 0, Connections = 0, Requests = 0, Failures = 0;
        std::vector<std::uint32_t> Realms;
        std::uint32_t PresencePlayers = 0;
        std::uint64_t ReceivedAt = 0;
    };
    struct Node
    {
        std::string Key, Name, Address;
        std::vector<std::uint32_t> Realms;
        MapMetrics Metrics;
        CharacterMetrics Character;
        ChatMetrics Chat;
        Service Type = Service::Auth;
        std::uint16_t Port = 0;
        std::uint32_t Realm = 0, Build = 0, Capacity = 0, Capabilities = 0, Load = 0;
        bool Ready = false;
        bool Live = true;
        Administration Admin = Administration::Enabled;
        std::uint64_t Owner = 0, ExpiresAt = 0;
    };
    inline bool ValidKey(std::string const& key)
    {
        return !key.empty() && key.size() <= 64 && key.find_first_not_of(
            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_.") == std::string::npos;
    }
    inline bool ValidUtf8(std::string const& value)
    {
        for (std::size_t i = 0; i < value.size();)
        {
            unsigned char first = static_cast<unsigned char>(value[i++]);
            if (first < 0x80) { if (first < 32 || first == 127) return false; continue; }
            unsigned count; std::uint32_t code, minimum;
            if (first >= 0xc2 && first <= 0xdf) { count = 1; code = first & 0x1f; minimum = 0x80; }
            else if (first >= 0xe0 && first <= 0xef) { count = 2; code = first & 0xf; minimum = 0x800; }
            else if (first >= 0xf0 && first <= 0xf4) { count = 3; code = first & 7; minimum = 0x10000; }
            else return false;
            if (value.size() - i < count) return false;
            while (count--)
            {
                unsigned char next = static_cast<unsigned char>(value[i++]);
                if ((next & 0xc0) != 0x80) return false;
                code = (code << 6) | (next & 0x3f);
            }
            if (code < minimum || code > 0x10ffff || (code >= 0xd800 && code <= 0xdfff)) return false;
        }
        return true;
    }
    class Writer
    {
    public:
        std::vector<std::uint8_t> Bytes;
        void U8(std::uint8_t value) { Bytes.push_back(value); }
        void U16(std::uint16_t value) { U8(std::uint8_t(value >> 8)); U8(std::uint8_t(value)); }
        void U32(std::uint32_t value) { U16(std::uint16_t(value >> 16)); U16(std::uint16_t(value)); }
        void String(std::string const& value) { U16(std::uint16_t(value.size())); Bytes.insert(Bytes.end(), value.begin(), value.end()); }
    };
    class Reader
    {
    public:
        explicit Reader(std::vector<std::uint8_t> const& bytes) : _bytes(bytes) { }
        bool U8(std::uint8_t& value) { if (_pos == _bytes.size()) return false; value = _bytes[_pos++]; return true; }
        bool U16(std::uint16_t& value) { std::uint8_t a, b; if (!U8(a) || !U8(b)) return false; value = (std::uint16_t(a) << 8) | b; return true; }
        bool U32(std::uint32_t& value) { std::uint16_t a, b; if (!U16(a) || !U16(b)) return false; value = (std::uint32_t(a) << 16) | b; return true; }
        bool String(std::string& value, std::size_t max)
        {
            std::uint16_t length;
            if (!U16(length) || !length || length > max || length > _bytes.size() - _pos) return false;
            value.assign(reinterpret_cast<char const*>(_bytes.data() + _pos), length);
            _pos += length;
            return ValidUtf8(value);
        }
        bool End() const { return _pos == _bytes.size(); }
    private:
        std::vector<std::uint8_t> const& _bytes;
        std::size_t _pos = 0;
    };
    inline bool DecodeHeader(std::array<std::uint8_t, HeaderSize> const& bytes, Header& header)
    {
        if (bytes[0] != 'S' || bytes[1] != 'F' || bytes[2] != 'H' || bytes[3] != 'C') return false;
        header.Version = (std::uint16_t(bytes[4]) << 8) | bytes[5];
        header.Type = Message((std::uint16_t(bytes[6]) << 8) | bytes[7]);
        header.Length = (std::uint32_t(bytes[8]) << 24) | (std::uint32_t(bytes[9]) << 16) |
            (std::uint32_t(bytes[10]) << 8) | bytes[11];
        return header.Length <= MaximumPayload;
    }
    inline std::vector<std::uint8_t> Frame(Message type, Writer const& payload)
    {
        if (payload.Bytes.size() > MaximumPayload) return {};
        Writer frame;
        frame.Bytes = { 'S', 'F', 'H', 'C' };
        frame.U16(ProtocolVersion); frame.U16(std::uint16_t(type)); frame.U32(std::uint32_t(payload.Bytes.size()));
        frame.Bytes.insert(frame.Bytes.end(), payload.Bytes.begin(), payload.Bytes.end());
        return frame.Bytes;
    }
    inline bool DecodeRegistration(std::vector<std::uint8_t> const& bytes, Node& node)
    {
        Reader reader(bytes);
        std::uint8_t type;
        if (!reader.String(node.Key, 64) || !ValidKey(node.Key) || !reader.String(node.Name, 100) ||
            !reader.U8(type) || (type != 1 && type != 2 && type != 3 && type != 4 && type != 5) || !reader.String(node.Address, 64) ||
            !reader.U16(node.Port) || !node.Port || !reader.U32(node.Realm) || !reader.U32(node.Build) || !node.Build ||
            !reader.U32(node.Capacity) || !reader.U32(node.Capabilities) || !reader.End()) return false;
        node.Type = Service(type);
        node.Realms.clear();
        if (node.Type == Service::World) node.Realms.push_back(node.Realm);
        return ((node.Type == Service::Auth || node.Type == Service::Map || node.Type == Service::Character || node.Type == Service::Chat) && node.Realm == 0) || (node.Type == Service::World && node.Realm != 0);
    }
    inline bool DecodeRealms(std::vector<std::uint8_t> const& bytes, std::vector<std::uint32_t>& realms)
    {
        Reader reader(bytes);
        std::uint16_t count;
        if (!reader.U16(count) || count == 0 || count > 64) return false;
        std::vector<std::uint32_t> decoded;
        for (unsigned i = 0; i < count; ++i)
        {
            std::uint32_t id;
            if (!reader.U32(id) || !id) return false;
            for (auto existing : decoded) if (existing == id) return false;
            decoded.push_back(id);
        }
        if (!reader.End()) return false;
        realms = std::move(decoded);
        return true;
    }
}
#endif
