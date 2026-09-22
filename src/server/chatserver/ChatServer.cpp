/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "ChatServer.h"
#include "Cluster/ChatProtocol.h"
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
        std::vector<std::uint8_t> Output;
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
                }
            }
            X509_free(cert);
            return valid;
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
                    if (readError || !DecodeProbe(self->Input, id, realm) || !self->Owner.Config.Realms.count(realm)) { self->Close(true); return; }
                    ++self->Owner.Counters.Requests;
                    Cluster::Writer out;
                    out.U8('S'); out.U8('F'); out.U8('C'); out.U8('H'); out.U16(1); out.U16(0x8001); out.U32(id); out.U32(realm);
                    self->Output = std::move(out.Bytes);
                    boost::asio::async_write(self->Stream, boost::asio::buffer(self->Output),
                        [self](boost::system::error_code writeError, std::size_t) { self->Close(bool(writeError)); });
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
        result.Connections = std::uint32_t(_state->Sessions.size());
        result.Uptime = std::uint32_t(std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - _state->Started).count());
        return result;
    }
}
