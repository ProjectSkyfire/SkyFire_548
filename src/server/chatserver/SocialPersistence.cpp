/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "SocialPersistence.h"
#include "SocialSnapshot.h"
#include "Cluster/CertificateTools.h"
#include "Configuration/Config.h"
#include "Log.h"
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
// Build Boost.JSON from the installed Boost headers; no extra binary dependency.
#include <boost/json/src.hpp>
#include <condition_variable>
#include <chrono>
#include <deque>
#include <mutex>
#include <thread>

namespace Skyfire::Chat
{
    namespace
    {
        using Bytes = std::vector<std::uint8_t>;
        struct Rejected : std::runtime_error { Rejected() : std::runtime_error("Social request rejected") { } };
        void U32(Bytes& bytes, std::uint32_t value)
        { for (int shift = 24; shift >= 0; shift -= 8) bytes.push_back(std::uint8_t(value >> shift)); }
        void Text(Bytes& bytes, std::string const& value)
        { U32(bytes, std::uint32_t(value.size())); bytes.insert(bytes.end(), value.begin(), value.end()); }
        bool Domain(std::string const& value) { return value == "channels" || value == "guilds"; }
        struct Endpoint { std::uint32_t Realm; std::string Host, Key; std::uint16_t Port; };
        struct Connection
        {
            boost::asio::io_context Io;
            boost::asio::ssl::context Tls;
            boost::asio::ssl::stream<boost::asio::ip::tcp::socket> Socket;
            static boost::asio::ssl::context Context(Cluster::AgentOptions const& credentials)
            {
                boost::asio::ssl::context context(boost::asio::ssl::context::tls_client);
                if (!SSL_CTX_set_min_proto_version(context.native_handle(), TLS1_2_VERSION)) throw std::runtime_error("TLS unavailable");
                context.load_verify_file(credentials.CA);
                context.use_certificate_chain_file(credentials.Certificate);
                context.use_private_key_file(credentials.PrivateKey, boost::asio::ssl::context::pem);
                if (!SSL_CTX_check_private_key(context.native_handle())) throw std::runtime_error("Invalid social credentials");
                return context;
            }
            template<class Start> void Wait(Start start)
            {
                boost::asio::steady_timer timer(Io);
                timer.expires_after(std::chrono::seconds(5));
                boost::system::error_code result = boost::asio::error::timed_out;
                timer.async_wait([&](auto ec) { if (!ec) { boost::system::error_code ignored; Socket.next_layer().close(ignored); } });
                start([&](boost::system::error_code ec, auto...) { result = ec; timer.cancel(); });
                Io.run(); Io.restart();
                if (result) throw std::runtime_error("Social transport failed");
            }
            std::string Exchange(Bytes const& payload)
            {
                if (payload.size() > 256 * 1024 + 4096 || !Certificates::PeerAllowed(Socket.native_handle()))
                    throw std::runtime_error("Invalid social request or certificate");
                Bytes frame; U32(frame, std::uint32_t(payload.size())); frame.insert(frame.end(), payload.begin(), payload.end());
                Wait([&](auto done) { boost::asio::async_write(Socket, boost::asio::buffer(frame), done); });
                std::array<std::uint8_t, 4> header{};
                Wait([&](auto done) { boost::asio::async_read(Socket, boost::asio::buffer(header), done); });
                std::uint32_t size = 0; for (auto byte : header) size = (size << 8) | byte;
                if (!size || size > 9 * 1024 * 1024) throw std::runtime_error("Invalid social reply size");
                Bytes response(size);
                Wait([&](auto done) { boost::asio::async_read(Socket, boost::asio::buffer(response), done); });
                if (response.front() != 0) throw Rejected();
                return std::string(response.begin() + 1, response.end());
            }
            boost::json::value Call(std::uint8_t operation, boost::json::object const& body)
            {
                Bytes payload{operation}; Text(payload, boost::json::serialize(body));
                return boost::json::parse(Exchange(payload));
            }
            Connection(Endpoint const& endpoint, Cluster::AgentOptions const& credentials,
                std::string const& instance, std::string const& domain) : Tls(Context(credentials)), Socket(Io, Tls)
            {
                Socket.set_verify_mode(boost::asio::ssl::verify_peer);
                Socket.set_verify_callback(boost::asio::ssl::host_name_verification(endpoint.Host));
                auto address = boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(endpoint.Host), endpoint.Port);
                Wait([&](auto done) { Socket.next_layer().async_connect(address, done); });
                Wait([&](auto done) { Socket.async_handshake(boost::asio::ssl::stream_base::client, done); });
                X509* certificate = SSL_get1_peer_certificate(Socket.native_handle());
                if (!certificate) throw std::runtime_error("Missing social certificate");
                std::unique_ptr<X509, decltype(&X509_free)> guard(certificate, X509_free);
                if (Certificates::Identity(certificate) != endpoint.Key) throw std::runtime_error("Wrong character service identity");
                Bytes hello{16, 1}; U32(hello, endpoint.Realm); Text(hello, instance); Text(hello, domain);
                if (!Exchange(hello).empty()) throw std::runtime_error("Invalid social handshake");
            }
        };
    }
    struct SocialPersistence::State
    {
        struct Job
        {
            std::string Domain, Request;
            boost::json::object Body;
            std::chrono::steady_clock::time_point Deadline;
        };
        struct Realm
        {
            Endpoint Target;
            std::thread Worker;
            bool Ready = false;
            std::map<std::string, SocialSnapshot> Snapshots;
            std::deque<Job> Jobs;
            unsigned Pending = 0;
        };
        mutable std::mutex Lock;
        std::condition_variable Wake;
        bool Stopping = false;
        Cluster::AgentOptions Credentials;
        std::string Instance;
        std::map<std::uint32_t, std::unique_ptr<Realm>> Realms;
        std::deque<Result> Results;
        unsigned Pending = 0;
        void Run(Realm& realm)
        {
            for (;;)
            {
                { std::lock_guard<std::mutex> lock(Lock); if (Stopping) return; }
                try
                {
                    std::map<std::string, std::unique_ptr<Connection>> links;
                    std::map<std::string, SocialSnapshot> restored;
                    for (std::string const domain : {"channels", "guilds"})
                    {
                        { std::lock_guard<std::mutex> lock(Lock); if (Stopping) return; }
                        auto connection = std::make_unique<Connection>(realm.Target, Credentials, Instance, domain);
                        std::string after;
                        bool first = true;
                        auto& snapshot = restored[domain];
                        for (;;)
                        {
                            { std::lock_guard<std::mutex> lock(Lock); if (Stopping) return; }
                            auto page = connection->Call(17, {{"after", after}, {"limit", 32}});
                            if (first) { snapshot.Cursor = SocialSnapshot::Number(page.as_object().at("head")); first = false; }
                            auto count = snapshot.LoadPage(page, after);
                            if (count < 32) break;
                        }
                        // Replay from the first snapshot head. Per-record versions
                        // prevent older events from overwriting later page contents.
                        for (;;)
                        {
                            { std::lock_guard<std::mutex> lock(Lock); if (Stopping) return; }
                            auto page = connection->Call(19, {{"after", snapshot.Cursor}, {"limit", 32}});
                            if (snapshot.Events(page) < 32) break;
                        }
                        links.emplace(domain, std::move(connection));
                    }
                    { std::lock_guard<std::mutex> lock(Lock); realm.Snapshots = std::move(restored); realm.Ready = true; }
                    SF_LOG_INFO("server.chat", "Social persistence ready for realm %u.", realm.Target.Realm);
                    auto nextPoll = std::chrono::steady_clock::now();
                    for (;;)
                    {
                        Job job;
                        bool hasJob = false;
                        {
                            std::unique_lock<std::mutex> lock(Lock);
                            Wake.wait_until(lock, nextPoll, [&] { return Stopping || !realm.Jobs.empty(); });
                            if (Stopping) return;
                            if (!realm.Jobs.empty()) { job = std::move(realm.Jobs.front()); realm.Jobs.pop_front(); hasJob = true; }
                        }
                        if (hasJob)
                        {
                            if (std::chrono::steady_clock::now() >= job.Deadline)
                            {
                                std::lock_guard<std::mutex> lock(Lock);
                                Results.push_back({realm.Target.Realm, job.Domain, job.Request, Outcome::NotSent});
                                continue;
                            }
                            Result result{realm.Target.Realm, job.Domain, job.Request, Outcome::Unknown};
                            try
                            {
                                auto response = links.at(job.Domain)->Call(18, job.Body);
                                result.Revision = SocialSnapshot::Number(response.as_object().at("revision"));
                                result.Status = Outcome::Committed;
                            }
                            catch (Rejected const&)
                            {
                                result.Status = Outcome::Rejected;
                                std::lock_guard<std::mutex> lock(Lock); Results.push_back(std::move(result)); throw;
                            }
                            catch (...)
                            { std::lock_guard<std::mutex> lock(Lock); Results.push_back(std::move(result)); throw; }
                            { std::lock_guard<std::mutex> lock(Lock); Results.push_back(std::move(result)); }
                        }
                        if (std::chrono::steady_clock::now() >= nextPoll || hasJob)
                        {
                            for (auto const& link : links)
                            {
                                std::uint64_t cursor;
                                { std::lock_guard<std::mutex> lock(Lock); cursor = realm.Snapshots.at(link.first).Cursor; }
                                auto events = link.second->Call(19, {{"after", cursor}, {"limit", 32}});
                                std::lock_guard<std::mutex> lock(Lock);
                                realm.Snapshots.at(link.first).Events(events);
                            }
                            nextPoll = std::chrono::steady_clock::now() + std::chrono::seconds(2);
                        }
                    }
                }
                catch (std::exception const&)
                {
                    std::unique_lock<std::mutex> lock(Lock);
                    realm.Ready = false; realm.Snapshots.clear();
                    for (auto const& job : realm.Jobs) Results.push_back({realm.Target.Realm, job.Domain, job.Request, Outcome::NotSent});
                    realm.Jobs.clear();
                    if (Stopping) return;
                    SF_LOG_WARN("server.chat", "Social persistence unavailable for realm %u; rebuilding before accepting mutations.", realm.Target.Realm);
                    Wake.wait_for(lock, std::chrono::seconds(5), [&] { return Stopping; });
                }
            }
        }
    };
    SocialPersistence::SocialPersistence() = default;
    SocialPersistence::~SocialPersistence() { Stop(); }
    bool SocialPersistence::Start(std::set<std::uint32_t> const& realms, Cluster::AgentOptions const& credentials, std::string& error)
    {
        if (_state) { error = "Social persistence already started."; return false; }
        if (!sConfigMgr->GetBoolDefault("Chat.Persistence.Enable", false)) return true;
        try
        {
            auto state = std::make_unique<State>(); state->Credentials = credentials; state->Instance = Certificates::Random().substr(0, 32);
            if (realms.empty() || realms.size() > 64) throw std::runtime_error("Invalid persistence realms");
            for (auto id : realms)
            {
                auto prefix = "Chat.Persistence.Realm." + std::to_string(id);
                auto realm = std::make_unique<State::Realm>(); realm->Target.Realm = id;
                realm->Target.Host = sConfigMgr->GetStringDefault((prefix + ".Host").c_str(), "");
                realm->Target.Key = sConfigMgr->GetStringDefault((prefix + ".NodeKey").c_str(), "");
                auto port = sConfigMgr->GetIntDefault((prefix + ".Port").c_str(), 54930);
                auto address = boost::asio::ip::make_address(realm->Target.Host);
                if (!id || address.is_unspecified() || address.is_multicast() || port < 1 || port > 65535 || !Cluster::ValidKey(realm->Target.Key))
                    throw std::runtime_error("Invalid persistence endpoint");
                realm->Target.Port = std::uint16_t(port); state->Realms.emplace(id, std::move(realm));
            }
            _state = std::move(state);
            for (auto& item : _state->Realms) item.second->Worker = std::thread([this, realm = item.second.get()] { _state->Run(*realm); });
            return true;
        }
        catch (std::exception const&) { Stop(); error = "Cannot start social persistence; configure a numeric endpoint and certificate identity for every Chat.Realms entry."; return false; }
    }
    bool SocialPersistence::Enabled() const { return bool(_state); }
    bool SocialPersistence::Ready() const
    {
        if (!_state) return true;
        std::lock_guard<std::mutex> lock(_state->Lock);
        for (auto const& realm : _state->Realms) if (!realm.second->Ready) return false;
        return true;
    }
    bool SocialPersistence::Read(std::uint32_t realm, std::string const& domain, std::string const& key, SocialRecord& record) const
    {
        if (!_state) return false;
        std::lock_guard<std::mutex> lock(_state->Lock);
        auto found = _state->Realms.find(realm);
        if (found == _state->Realms.end() || !found->second->Ready || !Domain(domain)) return false;
        auto const& records = found->second->Snapshots.at(domain).Records;
        auto item = records.find(key); if (item == records.end()) return false;
        record = item->second; return true;
    }
    bool SocialPersistence::Ready(std::uint32_t realm) const
    {
        if (!_state) return true;
        std::lock_guard<std::mutex> lock(_state->Lock);
        auto found = _state->Realms.find(realm);
        return found != _state->Realms.end() && found->second->Ready;
    }
    bool SocialPersistence::Submit(std::uint32_t realm, std::string const& domain, std::string const& request,
        std::string const& key, std::uint64_t expected, std::uint64_t actor, boost::json::value document)
    {
        if (!_state || !Domain(domain) || request.size() != 32 || request.find_first_not_of("0123456789abcdef") != std::string::npos ||
            key.empty() || key.size() > 192 || !Cluster::ValidUtf8(key) || !actor || expected > 0x7fffffffffffffffULL ||
            boost::json::serialize(document).size() > 256 * 1024) return false;
        std::lock_guard<std::mutex> lock(_state->Lock);
        auto found = _state->Realms.find(realm);
        if (_state->Stopping || found == _state->Realms.end() || !found->second->Ready || found->second->Pending >= 32 || _state->Pending >= 128) return false;
        found->second->Jobs.push_back({domain, request, {{"id", request}, {"key", key}, {"expected", expected}, {"actor", actor}, {"document", std::move(document)}},
            std::chrono::steady_clock::now() + std::chrono::seconds(10)});
        ++found->second->Pending; ++_state->Pending; _state->Wake.notify_all(); return true;
    }
    std::vector<SocialPersistence::Result> SocialPersistence::TakeResults()
    {
        std::vector<Result> results;
        if (!_state) return results;
        std::lock_guard<std::mutex> lock(_state->Lock);
        while (!_state->Results.empty() && results.size() < 32)
        {
            auto result = std::move(_state->Results.front()); _state->Results.pop_front();
            --_state->Realms.at(result.Realm)->Pending; --_state->Pending; results.push_back(std::move(result));
        }
        return results;
    }
    void SocialPersistence::Stop()
    {
        if (!_state) return;
        { std::lock_guard<std::mutex> lock(_state->Lock); _state->Stopping = true; }
        _state->Wake.notify_all();
        for (auto& realm : _state->Realms) if (realm.second->Worker.joinable()) realm.second->Worker.join();
        _state.reset();
    }
}
