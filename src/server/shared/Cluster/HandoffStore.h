/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#ifndef SKYFIRE_HANDOFF_STORE_H
#define SKYFIRE_HANDOFF_STORE_H
#include "ClusterProtocol.h"
#include <functional>
#include <map>
#include <tuple>

namespace Skyfire::Cluster::Handoff
{
    enum class Purpose : std::uint8_t { Login = 1, World = 2 };
    enum class Operation : std::uint8_t { Issue = 1, Inspect = 2, Consume = 3, Revoke = 4 };
    enum class Result : std::uint8_t { Ok, Invalid, Unavailable, Denied, Missing, Expired, Replay, Capacity };
    struct Binding
    {
        std::uint32_t Account = 0, Realm = 0;
        Purpose Use = Purpose::Login;
        std::string Address, Destination, Evidence;
        bool operator==(Binding const& rhs) const
        {
            return std::tie(Account,Realm,Use,Address,Destination,Evidence) ==
                std::tie(rhs.Account,rhs.Realm,rhs.Use,rhs.Address,rhs.Destination,rhs.Evidence);
        }
    };
    inline bool IsToken(std::string const& value)
    { return value.size() == 64 && value.find_first_not_of("0123456789abcdef") == std::string::npos; }
    inline bool Valid(Binding const& b)
    {
        return b.Account && !b.Address.empty() && b.Address.size() <= 64 && ValidUtf8(b.Address) &&
            !b.Destination.empty() && b.Destination.size() <= 96 && ValidUtf8(b.Destination) &&
            ((b.Use == Purpose::Login && !b.Realm && b.Evidence == "-") ||
             (b.Use == Purpose::World && b.Realm && IsToken(b.Evidence)));
    }
    struct Counters { std::uint64_t Issued = 0, Consumed = 0, Revoked = 0, Expired = 0, Rejected = 0, Replayed = 0; };
    // Process-neutral contract. Tokens are internal opaque capabilities, never client packet fields.
    // All times are monotonic milliseconds. The caller serializes operations.
    class Store
    {
    public:
        virtual ~Store() = default;
        virtual Result Issue(Binding const&, std::uint32_t ttl, std::uint64_t now, std::string& token) = 0;
        virtual Result Inspect(Binding const&, std::uint64_t now, std::string& token) = 0;
        virtual Result Consume(std::string const& token, Binding const&, std::uint64_t now) = 0;
        virtual Result Revoke(std::string const& token, Binding const&, std::uint64_t now) = 0;
        virtual void Cleanup(std::uint64_t now) = 0;
    };
    class MemoryStore final : public Store
    {
    public:
        explicit MemoryStore(std::function<std::string()> random, std::size_t capacity = 65536)
            : _random(std::move(random)), _capacity(capacity) { }
        Result Issue(Binding const& binding, std::uint32_t ttl, std::uint64_t now, std::string& token) override
        {
            token.clear(); Cleanup(now);
            if (!Valid(binding) || !ttl || ttl > 900) return Reject(Result::Invalid);
            if (_entries.size() >= _capacity) return Reject(Result::Capacity);
            auto value = _random();
            if (!IsToken(value) || _entries.count(value)) return Reject(Result::Unavailable);
            auto const slot = std::make_pair(binding.Account,binding.Use);
            auto old = _latest.find(slot);
            if (old != _latest.end())
            {
                auto& previous = _entries.at(old->second);
                if (previous.Status == State::Pending) { previous.Status = State::Revoked; ++_counts.Revoked; }
            }
            _entries.emplace(value,Entry{binding,now + std::uint64_t(ttl)*1000,State::Pending});
            _latest[slot] = value; token = std::move(value); ++_counts.Issued;
            return Result::Ok;
        }
        Result Inspect(Binding const& binding, std::uint64_t now, std::string& token) override
        {
            token.clear();
            auto it = _latest.find({binding.Account,binding.Use});
            if (it == _latest.end()) return Reject(Result::Missing);
            auto const result = Check(it->second,binding,now);
            if (result == Result::Ok) token = it->second;
            return result;
        }
        Result Consume(std::string const& token, Binding const& binding, std::uint64_t now) override
        {
            auto result = Check(token,binding,now);
            if (result != Result::Ok) return result;
            _entries.at(token).Status = State::Consumed; ++_counts.Consumed;
            return Result::Ok;
        }
        Result Revoke(std::string const& token, Binding const& binding, std::uint64_t now) override
        {
            auto result = Check(token,binding,now);
            if (result != Result::Ok) return result;
            _entries.at(token).Status = State::Revoked; ++_counts.Revoked;
            return Result::Ok;
        }
        void Cleanup(std::uint64_t now) override
        {
            for (auto it = _entries.begin(); it != _entries.end();)
            {
                if (it->second.Expires > now) { ++it; continue; }
                if (it->second.Status == State::Pending) ++_counts.Expired;
                auto slot = _latest.find({it->second.Bind.Account,it->second.Bind.Use});
                if (slot != _latest.end() && slot->second == it->first) _latest.erase(slot);
                it = _entries.erase(it);
            }
        }
        Counters const& Status() const { return _counts; }
        std::size_t Size() const { return _entries.size(); }
        void Clear() { _entries.clear(); _latest.clear(); }
    private:
        enum class State { Pending, Consumed, Revoked };
        struct Entry { Binding Bind; std::uint64_t Expires; State Status; };
        Result Reject(Result result)
        { ++_counts.Rejected; if (result == Result::Replay) ++_counts.Replayed; return result; }
        Result Check(std::string const& token, Binding const& binding, std::uint64_t now)
        {
            if (!IsToken(token) || !Valid(binding)) return Reject(Result::Invalid);
            auto it = _entries.find(token);
            if (it == _entries.end()) return Reject(Result::Missing);
            if (!(it->second.Bind == binding)) return Reject(Result::Denied);
            if (it->second.Expires <= now) return Reject(Result::Expired);
            if (it->second.Status != State::Pending) return Reject(Result::Replay);
            return Result::Ok;
        }
        std::function<std::string()> _random;
        std::size_t _capacity;
        std::map<std::string,Entry> _entries;
        std::map<std::pair<std::uint32_t,Purpose>,std::string> _latest;
        Counters _counts;
    };
}
#endif
