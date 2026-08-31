/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "AuthnetAcceptor.h"
#include "AuthnetSocket.h"
#include "Log.h"
#include "Network/BoostAsioUtils.h"
#include <boost/system/error_code.hpp>
#include <memory>

AuthnetAcceptor::AuthnetAcceptor() :
    _threadGroup(),
    _acceptor(_threadGroup.GetIoContext()),
    _closed(true)
{
}

AuthnetAcceptor::~AuthnetAcceptor()
{
    Close();
}

bool AuthnetAcceptor::Open(uint16 port, std::string const& bindIp)
{
    if (!Skyfire::Net::OpenTcpAcceptor(_threadGroup.GetIoContext(), _acceptor, port, bindIp, "server.authserver", "authnet"))
        return false;

    _closed = false;
    AsyncAccept();

    if (_threadGroup.Start(1) == -1)
    {
        Close();
        return false;
    }

    return true;
}

void AuthnetAcceptor::Close()
{
    bool expected = false;
    if (!_closed.compare_exchange_strong(expected, true))
        return;

    Skyfire::Net::CloseTcpAcceptor(_acceptor);
    _threadGroup.StopAndJoin();
}

void AuthnetAcceptor::Update()
{
}

void AuthnetAcceptor::AsyncAccept()
{
    if (!_acceptor.is_open())
        return;

    std::shared_ptr<RealmSocketHandle> clientSocket(new RealmSocketHandle(_threadGroup.GetIoContext()));
    _acceptor.async_accept(*clientSocket,
        [this, clientSocket](boost::system::error_code const& error)
        {
            HandleAccept(clientSocket, error);
        });
}

void AuthnetAcceptor::HandleAccept(std::shared_ptr<RealmSocketHandle> clientSocket, boost::system::error_code const& error)
{
    if (_closed)
        return;

    if (error)
    {
        if (error != boost::asio::error::operation_aborted)
            SF_LOG_ERROR("server.authserver", "Failed to accept authnet socket, error %d", error.value());
    }
    else
    {
        boost::system::error_code endpointError;
        boost::asio::ip::tcp::endpoint remoteEndpoint = clientSocket->remote_endpoint(endpointError);
        std::string remoteAddress = endpointError ? std::string("<unknown>") : remoteEndpoint.address().to_string();
        uint16 remotePort = endpointError ? 0 : remoteEndpoint.port();

        std::unique_ptr<RealmSocketHandle> socketHandle(new RealmSocketHandle(std::move(*clientSocket)));
        std::shared_ptr<RealmSocket> socket(new RealmSocket(std::move(socketHandle), remoteAddress, remotePort));
        socket->SetTcpNoDelay(true);
        socket->set_session(std::unique_ptr<RealmSocket::Session>(new AuthnetSocket(*socket)));
        socket->Start();
    }

    AsyncAccept();
}
