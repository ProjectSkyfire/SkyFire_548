/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_REALMACCEPTOR_H
#define SF_REALMACCEPTOR_H

#include "Common.h"
#include "RealmSocket.h"
#include "Threading/BoostAsioThreadGroup.h"
#include <boost/asio/ip/tcp.hpp>
#include <atomic>
#include <string>

class RealmAcceptor
{
public:
    RealmAcceptor();
    ~RealmAcceptor();

    bool Open(uint16 port, std::string const& bindIp);
    void Close();
    void Update();
    bool UsesProxyProtocol() const { return _proxyEnabled; }

private:
    void AsyncAccept();
    void HandleAccept(std::shared_ptr<RealmSocketHandle> clientSocket, boost::system::error_code const& error);

    Skyfire::Asio::IoContextThreadGroup _threadGroup;
    boost::asio::ip::tcp::acceptor _acceptor;
    std::atomic<bool> _closed;
    bool _proxyEnabled = false;
    unsigned _pendingProxy = 0;
    std::vector<boost::asio::ip::address> _proxyPeers;
};

#endif
