/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#ifndef SKYFIRE_HUB_AUTH_PROXY_H
#define SKYFIRE_HUB_AUTH_PROXY_H
#include "Cluster/AuthRouting.h"
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/steady_timer.hpp>
#include <memory>

struct HubAuthProxyOptions
{
    std::string Address = "127.0.0.1";
    std::uint16_t Port = 1118;
    bool Authnet = true, PreserveClientIP = true;
    unsigned MaxConnections = 4096, ConnectTimeoutSeconds = 3, IdleTimeoutSeconds = 300, MaxAttempts = 3;
    std::map<std::string, unsigned> Weights;
};
struct HubAuthProxyStatus
{
    std::string Name, Address;
    std::uint16_t Port = 0;
    bool Enabled = false;
    std::uint64_t Active = 0, Accepted = 0, Routed = 0, Rejected = 0, Attempts = 0,
        ConnectFailures = 0, Retries = 0, StreamFailures = 0, ClientBytes = 0, BackendBytes = 0;
    std::uint64_t NoBackend = 0, LimitRejected = 0, Withdrawn = 0, BackoffNodes = 0, AvailableNodes = 0;
    std::string LastRejection;
    std::map<std::string,std::uint64_t> ConnectionsByNode;
};
class HubAuthProxySession;
class HubAuthProxy
{
public:
    HubAuthProxy();
    ~HubAuthProxy();
    bool Open(HubAuthProxyOptions options, std::string& error);
    void Update(std::vector<Skyfire::Cluster::Node> nodes);
    void Close();
    HubAuthProxyStatus Status() const;
private:
    friend class HubAuthProxySession;
    void Accept();
    boost::asio::io_context _io;
    boost::asio::ip::tcp::acceptor _acceptor;
    boost::asio::steady_timer _acceptRetry;
    HubAuthProxyOptions _options;
    HubAuthProxyStatus _status;
    Skyfire::Cluster::AuthRouting _routing;
    Skyfire::Cluster::AuthBackoff _backoff;
    std::vector<Skyfire::Cluster::Node> _nodes;
    std::set<std::pair<std::string, std::uint16_t>> _loopEndpoints;
    std::map<std::uint64_t, std::shared_ptr<HubAuthProxySession>> _sessions;
    std::uint64_t _nextId = 0;
};
#endif
