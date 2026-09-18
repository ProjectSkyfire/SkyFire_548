/*
 * This file is part of Project SkyFire https://www.projectskyfire.org.
 * See LICENSE.md file for Copyright information.
 */
#include "HubClusterServer.h"
#include "Log.h"
#include "Network/BoostAsioUtils.h"
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <openssl/ssl.h>
#include <openssl/x509.h>
#include <chrono>
#include <exception>
#include <utility>

using namespace Skyfire::Cluster;

class HubClusterSession : public std::enable_shared_from_this<HubClusterSession>
{
public:
    HubClusterSession(HubClusterServer& server, boost::asio::ip::tcp::socket socket, std::uint64_t owner)
        : _server(server), _stream(std::move(socket), server._tls), _timer(server._io), _owner(owner) { }
    void Start()
    {
        Deadline(5);
        auto self = shared_from_this();
        _stream.async_handshake(boost::asio::ssl::stream_base::server, [self](boost::system::error_code error)
        {
            if (self->_closed) return;
            if (error) { self->Stop(); return; }
            X509* certificate = SSL_get_peer_certificate(self->_stream.native_handle());
            if (!certificate) { self->Stop(); return; }
            auto* const subject = X509_get_subject_name(certificate);
            int const index = X509_NAME_get_index_by_NID(subject, NID_commonName, -1);
            bool const single = index >= 0 && X509_NAME_get_index_by_NID(subject, NID_commonName, index) < 0;
            if (single)
            {
                auto* const name = X509_NAME_ENTRY_get_data(X509_NAME_get_entry(subject, index));
                auto bytes = ASN1_STRING_get0_data(name);
                self->_identity.assign(reinterpret_cast<char const*>(bytes), ASN1_STRING_length(name));
            }
            X509_free(certificate);
            if (!ValidKey(self->_identity)) { self->Reject(Error::Identity, "Certificate must have one node-key common name."); return; }
            self->ReadHeader();
        });
    }
    void Stop()
    {
        if (_closed) return;
        _closed = true;
        _timer.cancel();
        Skyfire::Net::CloseTcpSocket(_stream.next_layer());
        if (!_key.empty())
        {
            _server._registry.Remove(_key, _owner);
            SF_LOG_INFO("server.hub", "Cluster node '%s' disconnected or its lease expired.", _key.c_str());
        }
        _server._sessions.erase(_owner);
    }
private:
    void Deadline(std::uint32_t seconds)
    {
        _timer.expires_after(std::chrono::seconds(seconds));
        auto self = shared_from_this();
        _timer.async_wait([self](boost::system::error_code error) { if (!error) self->Stop(); });
    }
    void ReadHeader()
    {
        if (_closed) return;
        Deadline(_key.empty() ? 5 : _server._leaseSeconds);
        auto self = shared_from_this();
        boost::asio::async_read(_stream, boost::asio::buffer(_headerBytes), [self](boost::system::error_code error, std::size_t)
        {
            if (self->_closed) return;
            if (error) { self->Stop(); return; }
            if (!DecodeHeader(self->_headerBytes, self->_header)) { self->Reject(Error::Malformed, "Invalid frame or payload exceeds 4096 bytes."); return; }
            if (self->_header.Version != ProtocolVersion) { self->Reject(Error::Version, "Incompatible cluster protocol; hub requires version 1."); return; }
            if (self->_header.Type != Message::Register && self->_header.Type != Message::Ready &&
                self->_header.Type != Message::Heartbeat && self->_header.Type != Message::Deregister && self->_header.Type != Message::Realms)
            { self->Reject(Error::Malformed, "Unsupported request type."); return; }
            self->_body.resize(self->_header.Length);
            if (self->_body.empty()) { self->Handle(); return; }
            boost::asio::async_read(self->_stream, boost::asio::buffer(self->_body), [self](boost::system::error_code bodyError, std::size_t)
            {
                if (self->_closed) return;
                if (bodyError) self->Stop(); else self->Handle();
            });
        });
    }
    void Handle()
    {
        if (_header.Type == Message::Register)
        {
            Node node;
            if (!_key.empty()) { Reject(Error::Conflict, "This connection is already registered."); return; }
            if (!DecodeRegistration(_body, node)) { Reject(Error::Malformed, "Invalid node registration."); return; }
            if (node.Key != _identity) { Reject(Error::Identity, "Node key does not match certificate identity."); return; }
            boost::system::error_code addressError;
            auto address = boost::asio::ip::make_address(node.Address, addressError);
            if (addressError || address.is_unspecified() || address.is_multicast()) { Reject(Error::Malformed, "Advertise a concrete numeric endpoint address."); return; }
            if (node.Build != 18414 || (node.Capabilities & ~std::uint32_t(31)) ||
                (node.Type == Service::Auth && (node.Capabilities & 8)) ||
                (node.Type == Service::World && (node.Capabilities & 16)))
            { Reject(Error::Version, "Requires client build 18414 and supported service capabilities (mask 0..31)."); return; }
            if (!_server._registry.Register(node, _owner, HubClusterServer::Now(), _server._leaseSeconds * 1000ULL))
            { Reject(Error::Conflict, "Node key is already leased or registry capacity is exhausted."); return; }
            _key = node.Key;
            _needsRealms = (node.Capabilities & 8) != 0;
            _primaryRealm = node.Realm;
            SF_LOG_INFO("server.hub", "Cluster node '%s' registered; awaiting readiness.", _key.c_str());
        }
        else
        {
            if (_key.empty()) { Reject(Error::NotRegistered, "Register before sending lifecycle messages."); return; }
            if (_header.Type == Message::Deregister)
            {
                if (!_body.empty()) { Reject(Error::Malformed, "Deregister requires an empty payload."); return; }
                if (!_server._registry.Remove(_key, _owner)) { Reject(Error::NotRegistered, "Node lease expired; reconnect and register."); return; }
                Acknowledge(true); return;
            }
            if (_header.Type == Message::Realms)
            {
                std::vector<std::uint32_t> realms;
                if (!_needsRealms || !DecodeRealms(_body, realms))
                { Reject(Error::Malformed, "Invalid or unsupported realm list."); return; }
                bool primary = false;
                for (auto id : realms) if (id == _primaryRealm) primary = true;
                if (!primary) { Reject(Error::Malformed, "Realm list must include the primary realm."); return; }
                if (!_server._registry.SetRealms(_key, _owner, HubClusterServer::Now(), _server._leaseSeconds * 1000ULL, realms))
                { Reject(Error::NotRegistered, "Node lease expired; reconnect and register."); return; }
                _realmsReceived = true;
                Acknowledge(false); return;
            }
            if (_needsRealms && !_realmsReceived)
            { Reject(Error::Malformed, "Publish realms before readiness or heartbeats."); return; }
            Reader reader(_body);
            std::uint8_t ready = 0;
            std::uint32_t load = 0;
            if ((_header.Type == Message::Ready && (!reader.U8(ready) || ready > 1)) || !reader.U32(load) || !reader.End())
            { Reject(Error::Malformed, "Invalid readiness or heartbeat payload."); return; }
            if (!_server._registry.Renew(_key, _owner, HubClusterServer::Now(), _server._leaseSeconds * 1000ULL,
                load, _header.Type == Message::Ready ? int(ready) : -1))
            { Reject(Error::NotRegistered, "Node lease expired; reconnect and register."); return; }
        }
        Acknowledge(false);
    }
    void Acknowledge(bool close)
    {
        Writer payload; payload.U16(std::uint16_t(_header.Type)); payload.U32(_server._leaseSeconds);
        Write(Frame(Message::Ack, payload), close);
    }
    void Reject(Error code, char const* message)
    {
        Writer payload; payload.U16(std::uint16_t(code)); payload.String(message);
        Write(Frame(Message::Error, payload), true);
    }
    void Write(std::vector<std::uint8_t> bytes, bool close)
    {
        _response = std::move(bytes);
        // A bounded response cannot keep a dead or slow peer alive indefinitely.
        Deadline(5);
        auto self = shared_from_this();
        boost::asio::async_write(_stream, boost::asio::buffer(_response), [self, close](boost::system::error_code error, std::size_t)
        {
            if (self->_closed) return;
            if (error || close) self->Stop(); else self->ReadHeader();
        });
    }
    HubClusterServer& _server;
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> _stream;
    boost::asio::steady_timer _timer;
    std::uint64_t _owner;
    bool _closed = false;
    bool _needsRealms = false, _realmsReceived = false;
    std::uint32_t _primaryRealm = 0;
    std::string _identity, _key;
    std::array<std::uint8_t, HeaderSize> _headerBytes{};
    Header _header;
    std::vector<std::uint8_t> _body, _response;
};

HubClusterServer::HubClusterServer() : _tls(boost::asio::ssl::context::tls_server), _acceptor(_io) { }
HubClusterServer::~HubClusterServer() { Close(); }
std::uint64_t HubClusterServer::Now()
{
    return std::uint64_t(std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()).count());
}
bool HubClusterServer::Open(std::string const& address, std::uint16_t port, std::string const& certificate,
    std::string const& key, std::string const& ca, std::uint32_t leaseSeconds, std::size_t maxConnections)
{
    if (!_closed || certificate.empty() || key.empty() || ca.empty() || leaseSeconds < 5 || leaseSeconds > 300 ||
        maxConnections < 1 || maxConnections > 1024) return false;
    try
    {
        if (!SSL_CTX_set_min_proto_version(_tls.native_handle(), TLS1_2_VERSION)) return false;
        _tls.use_certificate_chain_file(certificate);
        _tls.use_private_key_file(key, boost::asio::ssl::context::pem);
        if (!SSL_CTX_check_private_key(_tls.native_handle())) return false;
        _tls.load_verify_file(ca);
        _tls.set_verify_mode(boost::asio::ssl::verify_peer | boost::asio::ssl::verify_fail_if_no_peer_cert);
        if (!Skyfire::Net::OpenTcpAcceptor(_io, _acceptor, port, address, "server.hub", "cluster listener")) return false;
        _registry = Registry(maxConnections);
        _leaseSeconds = leaseSeconds; _maxConnections = maxConnections;
        _closed = false;
        _io.restart();
        Accept();
        SF_LOG_INFO("server.hub", "Cluster protocol v1 listening on %s:%u with mutual TLS; lease %u seconds.", address.c_str(), port, leaseSeconds);
        return true;
    }
    catch (std::exception const&)
    {
        SF_LOG_ERROR("server.hub", "Unable to initialize cluster TLS listener; check certificate, key and CA settings.");
        Skyfire::Net::CloseTcpAcceptor(_acceptor);
        return false;
    }
}
void HubClusterServer::Accept()
{
    _acceptor.async_accept([this](boost::system::error_code error, boost::asio::ip::tcp::socket socket)
    {
        if (_closed) return;
        if (!error)
        {
            if (_sessions.size() >= _maxConnections) Skyfire::Net::CloseTcpSocket(socket);
            else
            {
                auto session = std::make_shared<HubClusterSession>(*this, std::move(socket), ++_nextOwner);
                _sessions.emplace(_nextOwner, session);
                session->Start();
            }
        }
        if (!_closed) Accept();
    });
}
void HubClusterServer::Update()
{
    if (_closed) return;
    _registry.Expire(Now());
    // Bound work per main-loop iteration so network floods cannot starve supervision.
    for (unsigned i = 0; i < 128 && _io.poll_one(); ++i) { }
}
void HubClusterServer::Close()
{
    if (_closed) return;
    _closed = true;
    Skyfire::Net::CloseTcpAcceptor(_acceptor);
    auto sessions = _sessions;
    for (auto const& entry : sessions) entry.second->Stop();
    _registry.Clear();
    // Drain cancelled callbacks while the server is still closed, including the
    // accept callback, before a possible later Open() restarts this context.
    _io.restart();
    while (_io.poll_one()) { }
    _io.stop();
}
