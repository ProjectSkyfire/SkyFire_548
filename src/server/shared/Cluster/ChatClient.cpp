/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "ChatClient.h"
#include "CertificateTools.h"
#include "HandoffClient.h"
#include "Config.h"
#include "Log.h"
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <condition_variable>
#include <mutex>
#include <optional>
#include <deque>

namespace Skyfire::Chat
{
    namespace
    {
        bool RouteWhispers = false;
        struct Client
        {
            Cluster::AgentOptions Options;
            std::uint32_t Realm = 0;
            std::string Generation, ChatKey;
            std::mutex Lock;
            std::condition_variable Wake;
            std::optional<std::vector<PlayerPresence>> Pending;
            struct Queued { Whisper Message; std::chrono::steady_clock::time_point Deadline; };
            std::deque<Queued> Whispers;
            std::deque<WhisperResult> Results;
            std::map<std::uint32_t, unsigned> AccountPending;
            unsigned TotalPending = 0;
            bool Stopping = false;
            std::thread Worker;
            ~Client() { Stop(); }
            void Stop()
            {
                { std::lock_guard<std::mutex> lock(Lock); Stopping = true; Pending.reset(); }
                Wake.notify_one(); if (Worker.joinable()) Worker.join();
            }
            bool VerifyIdentity(SSL* stream)
            {
                X509* certificate = SSL_get_peer_certificate(stream);
                if (!certificate) return false;
                X509_NAME const* subject = X509_get_subject_name(certificate);
                int index = X509_NAME_get_index_by_NID(subject, NID_commonName, -1);
                bool valid = false;
                if (index >= 0 && X509_NAME_get_index_by_NID(subject, NID_commonName, index) < 0)
                {
                    ASN1_STRING const* value = X509_NAME_ENTRY_get_data(X509_NAME_get_entry(subject, index));
                    int length = ASN1_STRING_length(value);
                    if (length > 0 && length <= 64)
                        valid = std::string(reinterpret_cast<char const*>(ASN1_STRING_get0_data(value)), std::size_t(length)) == ChatKey;
                }
                X509_free(certificate); return valid;
            }
            bool Send(std::uint16_t operation, Cluster::Writer const& payload)
            {
                try
                {
                    if (payload.Bytes.size() > MaxPresenceBytes) return false;
                    boost::asio::io_context io;
                    boost::asio::ssl::context tls(boost::asio::ssl::context::tls_client);
                    if (!SSL_CTX_set_min_proto_version(tls.native_handle(), TLS1_2_VERSION)) return false;
                    tls.load_verify_file(Options.CA); tls.use_certificate_chain_file(Options.Certificate);
                    tls.use_private_key_file(Options.PrivateKey, boost::asio::ssl::context::pem);
                    if (!SSL_CTX_check_private_key(tls.native_handle())) return false;
                    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> stream(io, tls);
                    boost::asio::ip::tcp::resolver resolver(io);
                    stream.set_verify_mode(boost::asio::ssl::verify_peer);
                    stream.set_verify_callback(boost::asio::ssl::host_name_verification(Options.Host));
                    if (!SSL_set_tlsext_host_name(stream.native_handle(), Options.Host.c_str())) return false;
                    Cluster::Writer request;
                    request.U8('S'); request.U8('F'); request.U8('C'); request.U8('H');
                    request.U16(1); request.U16(operation); request.U32(1); request.U32(Realm);
                    request.U32(std::uint32_t(payload.Bytes.size()));
                    request.Bytes.insert(request.Bytes.end(), payload.Bytes.begin(), payload.Bytes.end());
                    std::vector<std::uint8_t> response(ProbeSize + (operation == 3 ? payload.Bytes.size() : 0));
                    bool success = false;
                    resolver.async_resolve(Options.Host, std::to_string(Options.Port),
                        [&](boost::system::error_code ec, auto endpoints)
                    {
                        if (ec) { io.stop(); return; }
                        boost::asio::async_connect(stream.next_layer(), endpoints, [&](boost::system::error_code connected, auto const&)
                        {
                            if (connected) { io.stop(); return; }
                            stream.async_handshake(boost::asio::ssl::stream_base::client, [&](boost::system::error_code handshake)
                            {
                                if (handshake || !VerifyIdentity(stream.native_handle()) || !Skyfire::Certificates::PeerAllowed(stream.native_handle())) { io.stop(); return; }
                                boost::asio::async_write(stream, boost::asio::buffer(request.Bytes), [&](boost::system::error_code sent, std::size_t)
                                {
                                    if (sent) { io.stop(); return; }
                                    boost::asio::async_read(stream, boost::asio::buffer(response), [&](boost::system::error_code received, std::size_t)
                                    {
                                        auto expected = request.Bytes; expected.resize(ProbeSize); expected[6] = 0x80;
                                        if (operation == 3) expected.insert(expected.end(), payload.Bytes.begin(), payload.Bytes.end());
                                        success = !received && std::equal(response.begin(), response.end(), expected.begin()); io.stop();
                                    });
                                });
                            });
                        });
                    });
                    // All network work runs on this worker; no DNS/TLS/socket waits on world ticks.
                    io.run_for(std::chrono::seconds(5));
                    resolver.cancel(); boost::system::error_code ignored; stream.next_layer().close(ignored);
                    return success;
                }
                catch (std::exception const&) { return false; }
            }
            void Run()
            {
                std::uint64_t sequence = 0;
                bool available = false, first = true;
                for (;;)
                {
                    PresenceSnapshot snapshot;
                    std::optional<Queued> whisper;
                    {
                        std::unique_lock<std::mutex> lock(Lock);
                        Wake.wait(lock, [&] { return Stopping || Pending.has_value() || !Whispers.empty(); });
                        if (Stopping) return;
                        auto now = std::chrono::steady_clock::now();
                        while (!Whispers.empty() && now >= Whispers.front().Deadline)
                        { Results.push_back({std::move(Whispers.front().Message), false, Whispers.front().Deadline}); Whispers.pop_front(); }
                        if (Pending) { snapshot.Players = std::move(*Pending); Pending.reset(); }
                        else if (!Whispers.empty()) { whisper = std::move(Whispers.front()); Whispers.pop_front(); }
                        else continue;
                    }
                    if (whisper)
                    {
                        bool success = Send(3, EncodeWhisper(whisper->Message)) && std::chrono::steady_clock::now() < whisper->Deadline;
                        std::lock_guard<std::mutex> lock(Lock);
                        Results.push_back({std::move(whisper->Message), success, whisper->Deadline});
                        continue;
                    }
                    snapshot.Generation = Generation; snapshot.Sequence = ++sequence;
                    bool current = Send(2, EncodePresence(snapshot));
                    if (first || current != available)
                        SF_LOG_INFO("server.chat", "Chat presence publication %s.", current ? "available" : "unavailable");
                    first = false; available = current;
                }
            }
        };
        std::unique_ptr<Client> Active;
    }
    bool StartClient(Cluster::AgentOptions options, std::string& error)
    {
        RouteWhispers = sConfigMgr->GetBoolDefault("ChatService.Whispers", false);
        if (!sConfigMgr->GetBoolDefault("ChatService.Enable", false))
        {
            if (RouteWhispers) { error = "ChatService.Whispers requires ChatService.Enable; routed whispers will be unavailable."; return false; }
            return true;
        }
        int const port = sConfigMgr->GetIntDefault("ChatService.Port", 54940);
        int const realm = sConfigMgr->GetIntDefault("RealmID", 0);
        if (Active || !options.Enabled || realm <= 0 || port < 1 || port > 65535)
        { error = "Chat presence requires cluster TLS, a positive explicit RealmID and a valid endpoint."; return false; }
        options.Host = sConfigMgr->GetStringDefault("ChatService.Host", "localhost"); options.Port = std::uint16_t(port);
        if (options.Host.empty() || options.Host.size() > 255 || !Cluster::ValidUtf8(options.Host))
        { error = "Invalid ChatService.Host."; return false; }
        auto client = std::make_unique<Client>(); client->Options = std::move(options); client->Realm = std::uint32_t(realm);
        client->ChatKey = sConfigMgr->GetStringDefault("ChatService.NodeKey", "skyfire-chat-primary");
        if (!Cluster::ValidKey(client->ChatKey)) { error = "Invalid ChatService.NodeKey."; return false; }
        client->Generation = Cluster::Handoff::RandomToken();
        if (client->Generation.empty()) { error = "Cannot generate chat presence incarnation."; return false; }
        try { client->Worker = std::thread([ptr = client.get()] { ptr->Run(); }); }
        catch (std::exception const&) { error = "Cannot start chat presence worker."; return false; }
        Active = std::move(client); return true;
    }
    bool ClientEnabled() { return bool(Active); }
    void PublishPresence(std::vector<PlayerPresence> players)
    {
        if (!Active) return;
        if (players.size() > MaxPresencePlayers) return; // Never publish a partial presence snapshot.
        { std::lock_guard<std::mutex> lock(Active->Lock); Active->Pending = std::move(players); }
        Active->Wake.notify_one(); // Latest-only pending slot bounds memory during outages.
    }
    bool WhispersEnabled() { return RouteWhispers; }
    bool QueueWhisper(Whisper message)
    {
        if (!Active || !RouteWhispers) return false;
        message.Generation = Active->Generation;
        Whisper decoded;
        if (!DecodeWhisper(EncodeWhisper(message).Bytes, decoded)) return false;
        std::lock_guard<std::mutex> lock(Active->Lock);
        if (Active->Stopping || Active->TotalPending >= 128) return false;
        auto count = Active->AccountPending.find(message.Account);
        if (count != Active->AccountPending.end() && count->second >= 4) return false;
        ++Active->AccountPending[message.Account]; ++Active->TotalPending;
        Active->Whispers.push_back({std::move(message), std::chrono::steady_clock::now() + std::chrono::seconds(5)});
        Active->Wake.notify_one(); return true;
    }
    std::vector<WhisperResult> TakeWhisperResults()
    {
        std::vector<WhisperResult> results;
        if (!Active) return results;
        std::lock_guard<std::mutex> lock(Active->Lock);
        while (!Active->Results.empty() && results.size() < 32)
        {
            if (std::chrono::steady_clock::now() >= Active->Results.front().Deadline) Active->Results.front().Success = false;
            auto account = Active->Results.front().Message.Account;
            auto found = Active->AccountPending.find(account);
            if (found != Active->AccountPending.end() && !--found->second) Active->AccountPending.erase(found);
            --Active->TotalPending;
            results.push_back(std::move(Active->Results.front())); Active->Results.pop_front();
        }
        return results;
    }
    void StopClient() { Active.reset(); RouteWhispers = false; }
}
