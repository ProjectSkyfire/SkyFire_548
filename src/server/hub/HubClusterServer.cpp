/*
 * This file is part of Project SkyFire https://www.projectskyfire.org.
 * See LICENSE.md file for Copyright information.
 */
#include "HubClusterServer.h"
#include "Cluster/HandoffService.h"
#include "Cluster/HandoffClient.h"
#include "Cluster/RealmDirectory.h"
#include "Cluster/MapDataDirectory.h"
#include "Cluster/CharacterMetrics.h"
#include <sstream>
#include "Log.h"
#include "Configuration/Config.h"
#include "Database/DatabaseEnv.h"
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
#include <set>

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
            auto pending = _server._mapRestarts.find(_key);
            if (pending != _server._mapRestarts.end() && pending->second == _owner) _server._mapRestarts.erase(pending);
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
                self->_header.Type != Message::Heartbeat && self->_header.Type != Message::Deregister && self->_header.Type != Message::Realms &&
                self->_header.Type != Handoff::RequestType && self->_header.Type != Realms::RequestType &&
                self->_header.Type != MapData::RequestType && self->_header.Type != MapData::MetricsType &&
                self->_header.Type != CharacterMetricsType)
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
        if (_header.Type == CharacterMetricsType)
        {
            CharacterMetrics metrics;
            if (!DecodeCharacterMetrics(_body, metrics) ||
                !_server._registry.SetCharacterMetrics(_key, _owner, HubClusterServer::Now(), metrics))
            { Reject(Error::Malformed, "Character metrics require a live character service and valid payload."); return; }
            Acknowledge(false); return;
        }
        if (_header.Type == MapData::MetricsType)
        {
            MapMetrics metrics;
            if (!MapData::DecodeMetrics(_body, metrics) ||
                !_server._registry.SetMapMetrics(_key, _owner, HubClusterServer::Now(), std::move(metrics)))
            { Reject(Error::Malformed, "Map metrics require a live mapserver registration and valid payload."); return; }
            Acknowledge(false); return;
        }
        if (_header.Type == MapData::RequestType)
        {
            std::string key;
            // Bootstrap precedes world readiness; use an explicit certificate-key allowlist.
            std::istringstream readers(sConfigMgr->GetStringDefault("Hub.MapData.Readers", ""));
            std::string reader; bool allowed = false;
            while (readers >> reader) if (reader == _identity) allowed = true;
            if (!_key.empty() || !allowed || !MapData::DecodeQuery(_body, key))
            { Reject(Error::Identity, "Map discovery requires an allowed certificate identity and a separate connection."); return; }
            Write(Frame(MapData::ReplyType, MapData::Resolve(key, _server._registry.Snapshot(), HubClusterServer::Now())), true);
            return;
        }
        if (_header.Type == Realms::RequestType)
        {
            std::vector<Realms::Query> queries;
            if (_key.empty() || !Realms::DecodeQuery(_body,queries))
            { Reject(Error::Malformed,"Realm queries require a registered auth connection and 1..16 realm/build pairs."); return; }
            auto const now = HubClusterServer::Now(); auto nodes = _server._registry.Snapshot();
            auto caller = std::find_if(nodes.begin(),nodes.end(),[&](Node const& node) { return node.Key == _key && node.Owner == _owner; });
            if (caller == nodes.end() || caller->Type != Service::Auth || !caller->Ready || now >= caller->ExpiresAt || !(caller->Capabilities & 128))
            { Reject(Error::NotRegistered,"Realm queries require a ready, leased auth node with directory capability."); return; }
            std::vector<Realms::Route> routes;
            for (auto query : queries) routes.push_back(Realms::Resolve(query,nodes,now,(caller->Capabilities & 64) != 0));
            Write(Frame(Realms::ReplyType,Realms::EncodeReply(routes)),false); return;
        }
        if (_header.Type == Handoff::RequestType)
        {
            Handoff::Request request;
            if (!_key.empty() || !Handoff::Decode(_body,request))
            { Reject(Error::Malformed,"Invalid handoff request; use a separate TLS connection."); return; }
            boost::system::error_code addressError;
            auto address = boost::asio::ip::make_address(request.Bind.Address,addressError);
            if (addressError || address.is_unspecified() || address.is_multicast())
            { Reject(Error::Malformed,"Handoff requires a concrete client address."); return; }
            request.Bind.Address = address.to_string();
            auto const now = HubClusterServer::Now();
            auto result = Handoff::Authorize(_identity,_server._registry.Snapshot(),now,request);
            std::string token;
            if (result == Handoff::Result::Ok) result = Handoff::Execute(_server._databaseHandoffs ? static_cast<Handoff::Store&>(*_server._databaseHandoffs) :
                static_cast<Handoff::Store&>(_server._handoffs),request,now,token);
            // Never include token, evidence, session key or client payload in diagnostics.
            SF_LOG_INFO("server.handoff", "Handoff node '%s' operation %u account %u realm %u result %u.",
                _identity.c_str(),unsigned(request.Action),request.Bind.Account,request.Bind.Realm,unsigned(result));
            Writer reply; reply.U8(std::uint8_t(result)); reply.String(token.empty() ? "-" : token);
            Write(Frame(Handoff::ReplyType,reply),true); return;
        }
        if (_header.Type == Message::Register)
        {
            Node node;
            if (!_key.empty()) { Reject(Error::Conflict, "This connection is already registered."); return; }
            if (!DecodeRegistration(_body, node)) { Reject(Error::Malformed, "Invalid node registration."); return; }
            if (node.Key != _identity) { Reject(Error::Identity, "Node key does not match certificate identity."); return; }
            boost::system::error_code addressError;
            auto address = boost::asio::ip::make_address(node.Address, addressError);
            if (addressError || address.is_unspecified() || address.is_multicast()) { Reject(Error::Malformed, "Advertise a concrete numeric endpoint address."); return; }
            if (node.Build != 18414 || (node.Capabilities & ~std::uint32_t(2047)) ||
                (node.Type == Service::Auth && (node.Capabilities & (776 | 1024))) ||
                (node.Type == Service::World && (node.Capabilities & (944 | 1024))) ||
                (node.Type == Service::Character && node.Capabilities != 1024) ||
                (node.Type == Service::Map && (node.Capabilities != MapData::Capability && node.Capabilities != (MapData::Capability | MapData::RestartCapability))))
            { Reject(Error::Version, "Requires client build 18414 and supported service capabilities."); return; }
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
        auto restart = _server._mapRestarts.find(_key);
        if (!close && _header.Type == Message::Heartbeat && restart != _server._mapRestarts.end() && restart->second == _owner)
        {
            _server._mapRestarts.erase(restart);
            Write(Frame(MapData::RestartReply, payload), true); return;
        }
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

HubClusterServer::HubClusterServer() : _tls(boost::asio::ssl::context::tls_server), _acceptor(_io), _handoffs(Handoff::RandomToken) { }
HubClusterServer::~HubClusterServer() { Close(); }
bool HubClusterServer::RestartMap(std::string const& key, std::string& error)
{
    for (auto const& node : _registry.Snapshot())
        if (node.Key == key && node.Type == Service::Map && node.Live && node.Ready && node.ExpiresAt > Now())
        {
            if (!(node.Capabilities & MapData::RestartCapability))
            { error = "Mapserver must be restarted once with the updated Python script to enable hub restarts."; return false; }
            if (!_mapRestarts.emplace(key,node.Owner).second)
            { error = "Mapserver restart is already queued."; return false; }
            SF_LOG_INFO("server.hub", "Restart queued for mapserver '%s'.",key.c_str()); return true;
        }
    error = "Mapserver is offline or not ready."; return false;
}

bool HubClusterServer::LoadAdministration(std::string& error)
{
    auto count = HubDatabase.Query("SELECT COUNT(*) FROM hub_cluster_policy");
    if (!count || count->Fetch()[0].GetUInt64() > 4096)
    { error = "Cannot load cluster policy (maximum 4096 records). Apply sql/updates/hub/2026_09_18_hub_00.sql and 2026_09_18_hub_01.sql."; return false; }
    auto result = HubDatabase.Query(HubDatabase.GetPreparedStatement(HUB_SEL_CLUSTER_POLICY));
    if (!result && count->Fetch()[0].GetUInt64()) { error = "Cannot read persisted cluster policy."; return false; }
    _policies.clear();
    if (result) do
    {
        auto fields = result->Fetch();
        Node node; node.Key = fields[0].GetString(); node.Name = fields[1].GetString();
        node.Capabilities = fields[2].GetUInt32(); node.Admin = Administration(fields[3].GetUInt8());
        node.Type = Service(fields[4].GetUInt8());
        node.Live = false; node.Ready = false;
        if ((node.Type != Service::Auth && node.Type != Service::World && node.Type != Service::Map && node.Type != Service::Character) || !ValidKey(node.Key) || unsigned(node.Admin) > 2 || node.Name.empty() || !ValidUtf8(node.Name))
        { error = "Invalid persisted cluster policy; correct the hub_cluster_policy row before startup."; return false; }
        _policies[node.Key] = node;
        _registry.SetAdministration(node.Key,node.Admin);
    } while (result->NextRow());
    return true;
}
bool HubClusterServer::SetAdministration(std::string const& key, std::string const& action, std::string const& actor, std::string& error)
{
    if (_closed) { error = "Cluster listener is disabled."; return false; }
    if (!ValidKey(key) || (action != "enable" && action != "disable" && action != "drain"))
    { error = "Use a valid node key and enable, disable or drain."; return false; }
    Node node; bool found = false;
    for (auto const& current : _registry.Snapshot()) if (current.Key == key) { node = current; found = true; break; }
    if (!found)
    {
        auto policy = _policies.find(key);
        if (policy == _policies.end()) { error = "Node is not registered and has no saved policy."; return false; }
        node = policy->second;
    }
    if (!_policies.count(key) && _policies.size() >= 4096) { error = "Cluster policy limit reached."; return false; }
    if (node.Type == Service::Character) { error = "Stop worlds before operating the character service; routing drain is not database fencing."; return false; }
    auto const state = action == "drain" ? Administration::Draining : action == "disable" ? Administration::Disabled : Administration::Enabled;
    auto stmt = HubDatabase.GetPreparedStatement(HUB_UPSERT_CLUSTER_POLICY);
    stmt->setString(0,key); stmt->setString(1,node.Name); stmt->setUInt32(2,node.Capabilities);
    stmt->setUInt8(3,uint8(state)); stmt->setString(4,actor); stmt->setUInt8(5,uint8(node.Type));
    HubDatabase.DirectExecute(stmt);
    auto check = HubDatabase.GetPreparedStatement(HUB_SEL_CLUSTER_POLICY_BY_KEY); check->setString(0,key);
    auto saved = HubDatabase.Query(check);
    if (!saved || saved->Fetch()[0].GetUInt8() != uint8(state))
    { error = "Policy persistence could not be verified; no in-memory change made. Check database availability and state before retrying."; return false; }
    node.Admin = state; node.Ready = false; node.Live = false;
    _policies[key] = node; _registry.SetAdministration(key,state);
    SF_LOG_INFO("server.hub", "Cluster routing policy: actor '%s', node '%s', state '%s'. Existing streams remain pinned.",
        actor.c_str(),key.c_str(),AdministrationName(state));
    return true;
}
std::vector<Node> HubClusterServer::Directory() const
{
    auto nodes = _registry.Snapshot();
    std::set<std::string> live;
    for (auto const& node : nodes) live.insert(node.Key);
    for (auto const& policy : _policies)
    {
        if (!live.count(policy.first)) nodes.push_back(policy.second);
    }
    return nodes;
}
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
        auto const store = sConfigMgr->GetStringDefault("Hub.Handoff.Store", "memory");
        if (store != "memory" && store != "database")
        { SF_LOG_ERROR("server.hub", "Hub.Handoff.Store must be memory or database."); return false; }
        std::unique_ptr<HubHandoffStore> databaseHandoffs;
        if (store == "database")
        {
            databaseHandoffs = std::make_unique<HubHandoffStore>();
            if (!databaseHandoffs->Open(sConfigMgr->GetStringDefault("HubDatabaseInfo", "")))
            {
                SF_LOG_ERROR("server.hub", "Database handoff store unavailable; apply the durable handoff migration before enabling it.");
                return false;
            }
        }
        // Registration owners must not repeat across boots when grants survive a restart.
        auto ownerSeed = Handoff::RandomToken();
        if (!Handoff::IsToken(ownerSeed)) return false;
        _nextOwner = std::stoull(ownerSeed.substr(0, 16), nullptr, 16);
        if (!SSL_CTX_set_min_proto_version(_tls.native_handle(), TLS1_2_VERSION)) return false;
        _tls.use_certificate_chain_file(certificate);
        _tls.use_private_key_file(key, boost::asio::ssl::context::pem);
        if (!SSL_CTX_check_private_key(_tls.native_handle())) return false;
        _tls.load_verify_file(ca);
        _tls.set_verify_mode(boost::asio::ssl::verify_peer | boost::asio::ssl::verify_fail_if_no_peer_cert);
        if (!Skyfire::Net::OpenTcpAcceptor(_io, _acceptor, port, address, "server.hub", "cluster listener")) return false;
        _databaseHandoffs = std::move(databaseHandoffs);
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
        _databaseHandoffs.reset();
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
            if (_sessions.size() >= _maxConnections + 32) Skyfire::Net::CloseTcpSocket(socket);
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
    if (Now() >= _handoffCleanupAt)
    {
        if (_databaseHandoffs) _databaseHandoffs->Cleanup(Now()); else _handoffs.Cleanup(Now());
        _handoffCleanupAt = Now() + 1000;
    }
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
    _handoffs.Clear();
    _databaseHandoffs.reset();
    // Drain cancelled callbacks while the server is still closed, including the
    // accept callback, before a possible later Open() restarts this context.
    _io.restart();
    while (_io.poll_one()) { }
    _io.stop();
}
