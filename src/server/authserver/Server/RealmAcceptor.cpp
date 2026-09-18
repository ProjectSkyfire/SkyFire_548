/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "AuthSocket.h"
#include "Log.h"
#include "Network/BoostAsioUtils.h"
#include "RealmAcceptor.h"
#include "Configuration/Config.h"
#include "Network/ProxyProtocol.h"
#include <algorithm>
#include <boost/system/error_code.hpp>
#include <memory>

RealmAcceptor::RealmAcceptor() :
    _threadGroup(),
    _acceptor(_threadGroup.GetIoContext()),
    _closed(true)
{
}

RealmAcceptor::~RealmAcceptor()
{
    Close();
}

bool RealmAcceptor::Open(uint16 port, std::string const& bindIp)
{
    _proxyEnabled = sConfigMgr->GetBoolDefault("Realm.ProxyProtocol.Enable", false);
    _proxyPeers.clear();
    if (_proxyEnabled && !Skyfire::Net::ParseProxyPeers(sConfigMgr->GetStringDefault("Realm.ProxyProtocol.TrustedPeers", ""), _proxyPeers))
    {
        SF_LOG_ERROR("server.authserver", "Realm.ProxyProtocol.TrustedPeers requires 1..16 numeric hub addresses.");
        return false;
    }
    if (!Skyfire::Net::OpenTcpAcceptor(_threadGroup.GetIoContext(), _acceptor, port, bindIp, "server.authserver", "auth"))
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

void RealmAcceptor::Close()
{
    bool expected = false;
    if (!_closed.compare_exchange_strong(expected, true))
        return;

    Skyfire::Net::CloseTcpAcceptor(_acceptor);
    _threadGroup.StopAndJoin();
}

void RealmAcceptor::Update()
{
}

void RealmAcceptor::AsyncAccept()
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

void RealmAcceptor::HandleAccept(std::shared_ptr<RealmSocketHandle> clientSocket, boost::system::error_code const& error)
{
    if (_closed)
        return;

    if (error)
    {
        if (error != boost::asio::error::operation_aborted)
            SF_LOG_ERROR("server.authserver", "Failed to accept auth socket, error %d", error.value());
    }
    else
    {
        boost::system::error_code endpointError;
        boost::asio::ip::tcp::endpoint remoteEndpoint = clientSocket->remote_endpoint(endpointError);
        std::string remoteAddress = endpointError ? std::string("<unknown>") : remoteEndpoint.address().to_string();
        uint16 remotePort = endpointError ? 0 : remoteEndpoint.port();

        auto start = [clientSocket](std::string const& address, uint16 port)
        {
            std::unique_ptr<RealmSocketHandle> socketHandle(new RealmSocketHandle(std::move(*clientSocket)));
            std::shared_ptr<RealmSocket> socket(new RealmSocket(std::move(socketHandle), address, port));
            socket->set_session(std::unique_ptr<RealmSocket::Session>(new AuthSocket(*socket)));
            socket->Start();
        };
        if (!_proxyEnabled) start(remoteAddress, remotePort);
        else if (!endpointError && _pendingProxy < 128 && std::find(_proxyPeers.begin(), _proxyPeers.end(), remoteEndpoint.address()) != _proxyPeers.end())
        {
            ++_pendingProxy;
            Skyfire::Net::ProxyHeaderReader::Start(clientSocket, [this, start](bool success, boost::asio::ip::tcp::endpoint source)
            {
                --_pendingProxy;
                if (success && !_closed) start(source.address().to_string(), source.port());
            });
        }
        else Skyfire::Net::CloseTcpSocket(*clientSocket);
    }

    AsyncAccept();
}
