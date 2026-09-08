/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_AUTHNETACCEPTOR_H
#define SF_AUTHNETACCEPTOR_H

#include "Common.h"
#include "RealmSocket.h"
#include "Threading/BoostAsioThreadGroup.h"
#include <boost/asio/ip/tcp.hpp>
#include <atomic>
#include <string>

// Launcher-facing listener for authnet (port 1119 by default), separate
// from the classic GRUNT listener (RealmAcceptor). Disabled by default;
// gated by Authnet.Enabled in authserver.conf.
class AuthnetAcceptor
{
public:
    AuthnetAcceptor();
    ~AuthnetAcceptor();

    bool Open(uint16 port, std::string const& bindIp);
    void Close();
    void Update();

private:
    void AsyncAccept();
    void HandleAccept(std::shared_ptr<RealmSocketHandle> clientSocket, boost::system::error_code const& error);

    Skyfire::Asio::IoContextThreadGroup _threadGroup;
    boost::asio::ip::tcp::acceptor _acceptor;
    std::atomic<bool> _closed;
};

#endif
