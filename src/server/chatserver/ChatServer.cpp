/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "ChatServer.h"
#include "Cluster/ChatProtocol.h"
#include "Cluster/ChatPresence.h"
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <openssl/x509.h>
#include <chrono>

namespace Skyfire::Chat
{
    struct Server::State
    {
        struct Session;
        boost::asio::io_context Io;
        boost::asio::ssl::context Tls{boost::asio::ssl::context::tls_server};
        boost::asio::ip::tcp::acceptor Acceptor{Io};
        Options Config;
        std::set<std::shared_ptr<Session>> Sessions;
        Cluster::ChatMetrics Counters;
        PresenceDirectory Presence;
        std::chrono::steady_clock::time_point Started = std::chrono::steady_clock::now();
        bool Stopping = false;
        void Accept();
    };
    struct Server::State::Session : std::enable_shared_from_this<Session>
    {
        State& Owner;
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket> Stream;
        boost::asio::steady_timer Deadline;
        std::array<std::uint8_t, ProbeSize> Input{};
        std::vector<std::uint8_t> Output, Body;
        std::array<std::uint8_t, 4> Length{};
        std::string Identity;
        bool Closed = false;
        Session(State& owner, boost::asio::ip::tcp::socket socket) :
            Owner(owner), Stream(std::move(socket), owner.Tls), Deadline(owner.Io) { }
        void Close(bool failure = false)
        {
            if (Closed) return;
            Closed = true;
            if (failure) ++Owner.Counters.Failures;
            Deadline.cancel();
            boost::system::error_code ec;
            Stream.next_layer().close(ec);
            Owner.Sessions.erase(shared_from_this());
        }
        bool Authorized()
        {
            X509* cert = SSL_get_peer_certificate(Stream.native_handle());
            if (!cert) return false;
            X509_NAME const* name = X509_get_subject_name(cert);
            int const index = X509_NAME_get_index_by_NID(name, NID_commonName, -1);
            bool valid = false;
            if (index >= 0 && X509_NAME_get_index_by_NID(name, NID_commonName, index) < 0)
            {
                ASN1_STRING const* value = X509_NAME_ENTRY_get_data(X509_NAME_get_entry(name, index));
                int const length = ASN1_STRING_length(value);
                if (length > 0 && length <= 64)
                {
                    std::string key(reinterpret_cast<char const*>(ASN1_STRING_get0_data(value)), std::size_t(length));
                    valid = Cluster::ValidKey(key) && Owner.Config.WorldKeys.count(key) != 0;
                    if (valid) Identity = key;
                }
            }
            X509_free(cert);
            return valid;
        }
        void Reply()
        {
            ++Owner.Counters.Requests;
            Output.assign(Input.begin(), Input.end()); Output[6] = 0x80;
            auto self = shared_from_this();
            boost::asio::async_write(Stream, boost::asio::buffer(Output),
                [self](boost::system::error_code ec, std::size_t) { self->Close(bool(ec)); });
        }
        void Start()
        {
            auto self = shared_from_this();
            // One total deadline covers handshake, request and reply; slow clients cannot retain slots.
            Deadline.expires_after(std::chrono::seconds(Owner.Config.TimeoutSeconds));
            Deadline.async_wait([self](boost::system::error_code ec) { if (!ec) self->Close(true); });
            Stream.async_handshake(boost::asio::ssl::stream_base::server, [self](boost::system::error_code ec)
            {
                if (self->Closed) return;
                if (ec || !self->Authorized()) { self->Close(true); return; }
                boost::asio::async_read(self->Stream, boost::asio::buffer(self->Input),
                    [self](boost::system::error_code readError, std::size_t)
                {
                    if (self->Closed) return;
                    std::uint32_t id = 0, realm = 0;
                    auto probe = self->Input;
                    bool const presence = probe[6] == 0 && probe[7] == 2;
                    if (presence) probe[7] = 1;
                    if (readError || !DecodeProbe(probe, id, realm) || !self->Owner.Config.Realms.count(realm))
                    { self->Close(true); return; }
                    if (!presence) { self->Reply(); return; }
                    auto scope = self->Owner.Config.WorldRealms.find(self->Identity);
                    if (scope == self->Owner.Config.WorldRealms.end() || !scope->second.count(realm))
                    { self->Close(true); return; }
                    boost::asio::async_read(self->Stream, boost::asio::buffer(self->Length),
                        [self, realm](boost::system::error_code lengthError, std::size_t)
                    {
                        if (self->Closed) return;
                        auto const& b = self->Length;
                        std::uint32_t length = (std::uint32_t(b[0]) << 24) | (std::uint32_t(b[1]) << 16) |
                            (std::uint32_t(b[2]) << 8) | b[3];
                        if (lengthError || !length || length > MaxPresenceBytes) { self->Close(true); return; }
                        self->Body.resize(length);
                        boost::asio::async_read(self->Stream, boost::asio::buffer(self->Body),
                            [self, realm](boost::system::error_code bodyError, std::size_t)
                        {
                            if (self->Closed) return;
                            PresenceSnapshot snapshot;
                            auto now = std::uint64_t(std::chrono::duration_cast<std::chrono::milliseconds>(
                                std::chrono::steady_clock::now().time_since_epoch()).count());
                            if (bodyError || !DecodePresence(self->Body, snapshot) ||
                                !self->Owner.Presence.Replace(realm, self->Identity, std::move(snapshot), now))
                            { self->Close(true); return; }
                            self->Reply();
                        });
                    });
                });
            });
        }
    };
    void Server::State::Accept()
    {
        if (Stopping) return;
        Acceptor.async_accept([this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket)
        {
            if (Stopping) return;
            if (!ec)
            {
                if (Sessions.size() >= Config.MaxConnections) ++Counters.Failures;
                else
                {
                    auto session = std::make_shared<Session>(*this, std::move(socket));
                    Sessions.insert(session); session->Start();
                }
            }
            else ++Counters.Failures;
            Accept();
        });
    }
    Server::Server() = default;
    Server::~Server() { Stop(); }
    bool Server::Open(Options options, Cluster::AgentOptions const& tls, std::string& error)
    {
        if (_state || !options.Port || !options.MaxConnections || options.MaxConnections > 128 ||
            !options.TimeoutSeconds || options.TimeoutSeconds > 30 || options.WorldKeys.empty() || options.WorldKeys.size() > 128 ||
            options.Realms.empty() || options.Realms.size() > 64 || options.Realms.count(0))
        { error = "Invalid chat endpoint, connection limit, deadline or world allowlist."; return false; }
        for (auto const& key : options.WorldKeys)
            if (!Cluster::ValidKey(key)) { error = "Invalid chat world certificate identity."; return false; }
        for (auto const& scope : options.WorldRealms)
        {
            if (!options.WorldKeys.count(scope.first) || scope.second.empty())
            { error = "Presence scope requires an allowed world identity and realm list."; return false; }
            for (auto realm : scope.second)
                if (!options.Realms.count(realm)) { error = "Presence scope contains an unserved realm."; return false; }
        }
        try
        {
            auto state = std::make_unique<State>(); state->Config = std::move(options);
            state->Counters.Realms.assign(state->Config.Realms.begin(), state->Config.Realms.end());
            if (!SSL_CTX_set_min_proto_version(state->Tls.native_handle(), TLS1_2_VERSION))
            { error = "Cannot require TLS 1.2."; return false; }
            state->Tls.set_verify_mode(boost::asio::ssl::verify_peer | boost::asio::ssl::verify_fail_if_no_peer_cert);
            state->Tls.load_verify_file(tls.CA);
            state->Tls.use_certificate_chain_file(tls.Certificate);
            state->Tls.use_private_key_file(tls.PrivateKey, boost::asio::ssl::context::pem);
            if (!SSL_CTX_check_private_key(state->Tls.native_handle()))
            { error = "Chat certificate and private key do not match."; return false; }
            boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address(state->Config.Address), state->Config.Port);
            state->Acceptor.open(endpoint.protocol());
            state->Acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
            state->Acceptor.bind(endpoint); state->Acceptor.listen(128);
            _state = std::move(state); _state->Accept(); return true;
        }
        catch (std::exception const&) { Stop(); error = "Cannot open chat listener; check endpoint and TLS files."; return false; }
    }
    void Server::Update()
    {
        if (_state) _state->Presence.Expire(std::uint64_t(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count()));
        // Bound work per main-loop iteration so service-control heartbeats cannot starve.
        for (unsigned count = 0; _state && count < 64 && _state->Io.poll_one(); ++count) { }
    }
    void Server::Stop()
    {
        if (!_state) return;
        _state->Stopping = true;
        boost::system::error_code ec; _state->Acceptor.close(ec);
        while (!_state->Sessions.empty()) (*_state->Sessions.begin())->Close();
        _state->Io.poll(); _state.reset();
    }
    Cluster::ChatMetrics Server::Metrics() const
    {
        if (!_state) return {};
        auto result = _state->Counters;
        result.PresencePlayers = _state->Presence.Players();
        result.Connections = std::uint32_t(_state->Sessions.size());
        result.Uptime = std::uint32_t(std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - _state->Started).count());
        return result;
    }
}
