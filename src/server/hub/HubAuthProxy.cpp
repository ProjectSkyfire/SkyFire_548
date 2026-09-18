/* Part of Project SkyFire. See LICENSE.md for copyright information. */
#include "HubAuthProxy.h"
#include <boost/asio/write.hpp>
#include <boost/asio/steady_timer.hpp>
#include <array>
#include <chrono>

using boost::asio::ip::tcp;
namespace
{
    std::uint64_t Now()
    {
        return std::uint64_t(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count());
    }
}
class HubAuthProxySession : public std::enable_shared_from_this<HubAuthProxySession>
{
public:
    HubAuthProxySession(HubAuthProxy& owner, tcp::socket client, std::uint64_t id)
        : _owner(owner), _client(std::move(client)), _backend(owner._io), _timer(owner._io), _id(id) { }
    void Start() { TryBackend(); }
    std::string NodeKey() const { return _reserved ? _node.Key : ""; }
    bool StillEligible() const
    {
        for (auto const& node : _owner._nodes)
            if (node.Key == _node.Key && node.Owner == _node.Owner)
                return node.Live && node.Ready && node.Admin == Skyfire::Cluster::Administration::Enabled && Now() < node.ExpiresAt;
        return false;
    }
    void WithdrawPending()
    {
        if (!_closed && _connecting && !StillEligible())
        { _withdrawn = true; boost::system::error_code ec; _backend.close(ec); }
    }
    void Stop()
    {
        if (_closed) return;
        _closed = true; _timer.cancel();
        boost::system::error_code ec; _client.close(ec); _backend.close(ec);
        Release(); _owner._sessions.erase(_id);
    }
private:
    void Release() { if (_reserved) { _owner._routing.Release(_node); _reserved = false; } }
    void ConnectFailed()
    {
        ++_owner._status.ConnectFailures;
        _owner._backoff.Failed(_node,Now());
        TryBackend();
    }
    void TryBackend()
    {
        Release();
        if (_closed) return;
        _connecting = false; _withdrawn = false;
        auto excluded = _tried;
        for (auto const& node : _owner._nodes) if (_owner._backoff.Cooling(node,Now())) excluded.insert(node.Key);
        if (_tried.size() >= _owner._options.MaxAttempts || !_owner._routing.Select(_owner._nodes, Now(), excluded, _node))
        {
            ++_owner._status.Rejected; ++_owner._status.NoBackend;
            _owner._status.LastRejection = "No eligible backend: readiness, lease, maintenance, capacity, retry limit or connection backoff. Client connection closed without replay.";
            Stop(); return;
        }
        _reserved = true;
        if (!_tried.empty()) ++_owner._status.Retries;
        _tried.insert(_node.Key); ++_owner._status.Attempts;
        boost::system::error_code ec;
        auto address = boost::asio::ip::make_address(_node.Address, ec);
        auto const listen = _owner._acceptor.local_endpoint();
        if (ec || !_node.Port || _owner._loopEndpoints.count({_node.Address, _node.Port}) ||
            (_node.Port == listen.port() && (address == listen.address() || (listen.address().is_unspecified() && address.is_loopback()))))
        { ConnectFailed(); return; }
        _backend.close(ec);
        _backend = tcp::socket(_owner._io);
        auto self = shared_from_this();
        auto const attempt = ++_attempt;
        _connecting = true;
        _timer.expires_after(std::chrono::seconds(_owner._options.ConnectTimeoutSeconds));
        _timer.async_wait([self, attempt](boost::system::error_code error)
        {
            if (!error && !self->_closed && attempt == self->_attempt)
            {
                // The connect callback owns retry accounting; cancellation never replays data.
                boost::system::error_code ignored; self->_backend.close(ignored);
            }
        });
        _backend.async_connect(tcp::endpoint(address, _node.Port), [self, attempt](boost::system::error_code error)
        {
            if (self->_closed || attempt != self->_attempt) return;
            self->_timer.cancel();
            ++self->_attempt; // Invalidate a connect deadline already queued before cancellation.
            self->_connecting = false;
            if (self->_withdrawn || !self->StillEligible())
            { ++self->_owner._status.Withdrawn; self->TryBackend(); return; }
            if (error) { self->ConnectFailed(); return; }
            boost::system::error_code endpointError;
            auto local = self->_backend.local_endpoint(endpointError);
            if (endpointError) { self->ConnectFailed(); return; }
            auto remote = self->_backend.remote_endpoint(endpointError);
            if (endpointError) { self->ConnectFailed(); return; }
            if (remote.port() == self->_owner._options.Port && remote.address() == local.address() &&
                self->_owner._acceptor.local_endpoint().address().is_unspecified())
            {
                // Cache local-interface loops before queued accepts can perpetuate the same route.
                self->_owner._loopEndpoints.insert({self->_node.Address, self->_node.Port});
                self->ConnectFailed(); return;
            }
            ++self->_owner._status.Routed;
            self->_owner._backoff.Succeeded(self->_node);
            self->Touch();
            // After TCP connect, never select another backend, even if the first write fails.
            if (self->_owner._options.PreserveClientIP) self->SendIdentity();
            else { self->Read(true); self->Read(false); }
        });
    }
    void SendIdentity()
    {
        boost::system::error_code ec;
        auto source = _client.remote_endpoint(ec);
        if (ec) { StreamFailed(); return; }
        auto destination = _client.local_endpoint(ec);
        if (ec || source.address().is_v4() != destination.address().is_v4()) { StreamFailed(); return; }
        _identity = std::string("PROXY ") + (source.address().is_v4() ? "TCP4 " : "TCP6 ") + source.address().to_string() +
            " " + destination.address().to_string() + " " + std::to_string(source.port()) + " " + std::to_string(destination.port()) + "\r\n";
        auto self = shared_from_this();
        boost::asio::async_write(_backend, boost::asio::buffer(_identity), [self](boost::system::error_code error, std::size_t)
        {
            if (self->_closed) return;
            if (error) { self->StreamFailed(); return; }
            self->Read(true); self->Read(false);
        });
    }
    void Touch()
    {
        _timer.expires_after(std::chrono::seconds(_owner._options.IdleTimeoutSeconds));
        auto self = shared_from_this();
        _timer.async_wait([self](boost::system::error_code ec) { if (!ec && !self->_closed) self->StreamFailed(); });
    }
    void StreamFailed() { ++_owner._status.StreamFailures; Stop(); }
    void Read(bool fromClient)
    {
        auto self = shared_from_this();
        auto& input = fromClient ? _client : _backend;
        auto& buffer = fromClient ? _up : _down;
        input.async_read_some(boost::asio::buffer(buffer), [self, fromClient](boost::system::error_code ec, std::size_t bytes)
        {
            if (self->_closed) return;
            // Deliver any bytes accompanying EOF before half-closing the other write direction.
            if (bytes)
            {
                self->Touch();
                auto& output = fromClient ? self->_backend : self->_client;
                auto& data = fromClient ? self->_up : self->_down;
                boost::asio::async_write(output, boost::asio::buffer(data.data(), bytes),
                    [self, fromClient, ec](boost::system::error_code writeError, std::size_t written)
                {
                    if (self->_closed) return;
                    (fromClient ? self->_owner._status.ClientBytes : self->_owner._status.BackendBytes) += written;
                    if (writeError) { self->StreamFailed(); return; }
                    self->Touch();
                    if (ec) self->ReadEnded(fromClient, ec); else self->Read(fromClient);
                });
            }
            else if (ec) self->ReadEnded(fromClient, ec);
            else self->Read(fromClient);
        });
    }
    void ReadEnded(bool fromClient, boost::system::error_code ec)
    {
        if (ec != boost::asio::error::eof) { StreamFailed(); return; }
        (fromClient ? _clientEnded : _backendEnded) = true;
        boost::system::error_code ignored;
        (fromClient ? _backend : _client).shutdown(tcp::socket::shutdown_send, ignored);
        if (_clientEnded && _backendEnded) Stop();
    }
    HubAuthProxy& _owner;
    tcp::socket _client, _backend;
    boost::asio::steady_timer _timer;
    std::uint64_t _id, _attempt = 0;
    Skyfire::Cluster::Node _node;
    std::set<std::string> _tried;
    std::array<char, 16384> _up{}, _down{};
    std::string _identity;
    bool _closed = false, _reserved = false, _clientEnded = false, _backendEnded = false;
    bool _connecting = false, _withdrawn = false;
};
HubAuthProxy::HubAuthProxy() : _acceptor(_io), _acceptRetry(_io) { }
HubAuthProxy::~HubAuthProxy() { Close(); }
bool HubAuthProxy::Open(HubAuthProxyOptions options, std::string& error)
{
    if (_status.Enabled) { error = "Listener already open."; return false; }
    // Port zero is allowed for isolated transport tests; production config validates 1..65535.
    if (!options.MaxConnections || options.MaxConnections > 65536 ||
        !options.ConnectTimeoutSeconds || options.ConnectTimeoutSeconds > 30 || options.IdleTimeoutSeconds < 5 ||
        options.IdleTimeoutSeconds > 86400 || !options.MaxAttempts || options.MaxAttempts > 16)
    { error = "Invalid listener limits or timeouts."; return false; }
    for (auto const& weight : options.Weights)
        if (!Skyfire::Cluster::ValidKey(weight.first) || !weight.second || weight.second > 1000)
        { error = "Invalid node weight (allowed 1..1000)."; return false; }
    boost::system::error_code ec;
    auto address = boost::asio::ip::make_address(options.Address, ec);
    if (ec) { error = "Listener address must be numeric."; return false; }
    tcp::endpoint endpoint(address, options.Port);
    _acceptor.open(endpoint.protocol(), ec);
    if (!ec) _acceptor.bind(endpoint, ec);
    if (!ec) _acceptor.listen(boost::asio::socket_base::max_listen_connections, ec);
    if (ec) { error = ec.message(); boost::system::error_code ignored; _acceptor.close(ignored); return false; }
    options.Port = _acceptor.local_endpoint().port();
    _options = std::move(options);
    _routing.Clear(); _routing.Authnet = _options.Authnet; _routing.PreserveClientIP = _options.PreserveClientIP; _routing.Weights = _options.Weights;
    _status = {}; _status.Enabled = true; _status.Address = _options.Address; _status.Port = _options.Port;
    _status.Name = _options.Authnet ? "Authnet ingress" : "Legacy auth ingress";
    _io.restart(); Accept(); return true;
}
void HubAuthProxy::Accept()
{
    _acceptor.async_accept([this](boost::system::error_code ec, tcp::socket socket)
    {
        if (!_status.Enabled) return;
        if (!ec)
        {
            ++_status.Accepted;
            if (_sessions.size() >= _options.MaxConnections)
            {
                ++_status.Rejected; ++_status.LimitRejected;
                _status.LastRejection = "Ingress connection limit reached. Client connection closed.";
                socket.close(ec);
            }
            else
            {
                auto session = std::make_shared<HubAuthProxySession>(*this, std::move(socket), ++_nextId);
                _sessions.emplace(_nextId, session); session->Start();
            }
        }
        if (!_status.Enabled) return;
        if (ec)
        {
            _acceptRetry.expires_after(std::chrono::seconds(1));
            _acceptRetry.async_wait([this](boost::system::error_code error) { if (!error && _status.Enabled) Accept(); });
        }
        else Accept();
    });
}
void HubAuthProxy::Update(std::vector<Skyfire::Cluster::Node> nodes)
{
    if (!_status.Enabled) return;
    _nodes = std::move(nodes);
    _backoff.Prune(_nodes);
    for (auto const& session : _sessions) session.second->WithdrawPending();
    for (unsigned i = 0; i < 512 && _io.poll_one(); ++i) { }
}
HubAuthProxyStatus HubAuthProxy::Status() const
{
    auto status = _status; status.Active = _sessions.size();
    for (auto const& session : _sessions)
    {
        auto const key = session.second->NodeKey();
        if (!key.empty()) ++status.ConnectionsByNode[key];
    }
    auto const now = Now();
    for (auto const& node : _nodes)
        if (_backoff.Cooling(node,now)) ++status.BackoffNodes;
        else if (_routing.Eligible(node,now)) ++status.AvailableNodes;
    return status;
}
void HubAuthProxy::Close()
{
    if (!_status.Enabled) return;
    _status.Enabled = false;
    _acceptRetry.cancel();
    boost::system::error_code ec; _acceptor.close(ec);
    auto sessions = _sessions;
    for (auto const& entry : sessions) entry.second->Stop();
    _nodes.clear(); _loopEndpoints.clear(); _routing.Clear(); _backoff.Clear();
    _io.restart(); while (_io.poll_one()) { }
    _io.stop();
}
