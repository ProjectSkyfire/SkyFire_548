/*
 * This file is part of Project SkyFire https://www.projectskyfire.org.
 * See LICENSE.md file for Copyright information.
 */
#ifndef SKYFIRE_HUB_CLUSTER_SERVER_H
#define SKYFIRE_HUB_CLUSTER_SERVER_H
#include "Cluster/ClusterRegistry.h"
#include "Cluster/HandoffStore.h"
#include "HubHandoffStore.h"
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/context.hpp>
#include <map>
#include <memory>

class HubClusterSession;
class HubClusterServer
{
public:
    HubClusterServer();
    ~HubClusterServer();
    bool Open(std::string const& address, std::uint16_t port, std::string const& certificate,
        std::string const& key, std::string const& ca, std::uint32_t leaseSeconds, std::size_t maxConnections);
    void Update();
    void Close();
    bool IsOpen() const { return !_closed; }
    std::vector<Skyfire::Cluster::Node> Snapshot() const { return _registry.Snapshot(); }
    std::vector<Skyfire::Cluster::Node> Directory() const;
    bool LoadAdministration(std::string& error);
    bool SetAdministration(std::string const& key, std::string const& action, std::string const& actor, std::string& error);
    Skyfire::Cluster::Handoff::Counters HandoffStatus() const { return _databaseHandoffs ? _databaseHandoffs->Status() : _handoffs.Status(); }
private:
    friend class HubClusterSession;
    void Accept();
    static std::uint64_t Now();
    boost::asio::io_context _io;
    boost::asio::ssl::context _tls;
    boost::asio::ip::tcp::acceptor _acceptor;
    Skyfire::Cluster::Registry _registry;
    Skyfire::Cluster::Handoff::MemoryStore _handoffs;
    std::unique_ptr<HubHandoffStore> _databaseHandoffs;
    std::uint64_t _handoffCleanupAt = 0;
    std::map<std::string,Skyfire::Cluster::Node> _policies;
    std::map<std::uint64_t, std::shared_ptr<HubClusterSession>> _sessions;
    std::uint64_t _nextOwner = 0;
    std::uint32_t _leaseSeconds = 15;
    std::size_t _maxConnections = 128;
    bool _closed = true;
};
#endif
