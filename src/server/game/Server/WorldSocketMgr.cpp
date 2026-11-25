/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/** \file WorldSocketMgr.cpp
*  \ingroup u2w
*  \author Derex <derex101@gmail.com>
*/

#include "WorldSocketMgr.h"

#include <atomic>
#include <memory>
#include <set>

#include "Common.h"
#include "Config.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "ScriptMgr.h"
#include "Threading/BoostAsioThreadGroup.h"
#include "WorldSocket.h"
#include "WorldSocketAcceptor.h"
#include <boost/asio/socket_base.hpp>
#include <boost/system/error_code.hpp>
#include <vector>

/**
* This is a helper class to WorldSocketMgr, that manages
* network threads, and assigning connections from acceptor thread
* to other network threads
*/
class ReactorRunnable
{
public:
    ReactorRunnable() :
        m_ThreadGroup(),
        m_Connections(0),
        m_Stopped(false)
    {
    }

    virtual ~ReactorRunnable()
    {
        Stop();
        Wait();
    }

    void Stop()
    {
        if (m_Stopped.exchange(true))
            return;

        std::vector<WorldSocket*> sockets;

        {
            std::lock_guard<std::mutex> guard(m_SocketsLock);
            sockets.assign(m_Sockets.begin(), m_Sockets.end());
            // Close callbacks can remove the reactor's last reference after
            // this lock is released. Keep the snapshot alive until it is used.
            for (WorldSocket* socket : sockets)
                socket->AddReference();
        }

        for (WorldSocket* socket : sockets)
        {
            socket->CloseSocket();
            socket->RemoveReference();
        }

        m_ThreadGroup.Drain();
    }

    int Start()
    {
        return m_ThreadGroup.Start(1,
            []
            {
                SF_LOG_DEBUG("misc", "Network Thread Starting");
            },
            []
            {
                SF_LOG_DEBUG("misc", "Network Thread exits");
            });
    }

    void Wait()
    {
        m_ThreadGroup.Join();
    }

    long Connections()
    {
        return m_Connections.load();
    }

    boost::asio::io_context& GetIoContext()
    {
        return m_ThreadGroup.GetIoContext();
    }

    int AddSocket(WorldSocket* sock)
    {
        {
            std::lock_guard<std::mutex> guard(m_SocketsLock);

            if (m_Stopped)
                return -1;

            ++m_Connections;
            sock->AddReference();
            m_Sockets.insert(sock);
        }

        sScriptMgr->OnSocketOpen(sock);

        sock->AddReference();
        sock->Start([this](WorldSocket* socket)
        {
            SocketClosed(socket);
        });
        sock->RemoveReference();

        return 0;
    }

    void SocketClosed(WorldSocket* sock)
    {
        bool owned = false;

        {
            std::lock_guard<std::mutex> guard(m_SocketsLock);
            SocketSet::iterator itr = m_Sockets.find(sock);
            if (itr != m_Sockets.end())
            {
                m_Sockets.erase(itr);
                --m_Connections;
                owned = true;
            }
        }

        if (!owned)
            return;

        sScriptMgr->OnSocketClose(sock, false);
        sock->RemoveReference();
    }

private:
    typedef std::atomic<long> AtomicInt;
    typedef std::set<WorldSocket*> SocketSet;

    Skyfire::Asio::IoContextThreadGroup m_ThreadGroup;
    AtomicInt m_Connections;
    std::atomic<bool> m_Stopped;

    SocketSet m_Sockets;
    std::mutex m_SocketsLock;
};

WorldSocketMgr::WorldSocketMgr() :
    m_NetThreads(0),
    m_NetThreadsCount(0),
    m_SockOutKBuff(-1),
    m_SockOutUBuff(65536),
    m_UseNoDelay(true),
    m_Acceptor(0) { }

WorldSocketMgr::~WorldSocketMgr()
{
    delete[] m_NetThreads;
    delete m_Acceptor;
}

int
WorldSocketMgr::StartReactiveIO(uint16 port, const char* address)
{
    m_UseNoDelay = sConfigMgr->GetBoolDefault("Network.TcpNodelay", true);

    int num_threads = sConfigMgr->GetIntDefault("Network.Threads", 1);

    if (num_threads <= 0)
    {
        SF_LOG_ERROR("misc", "Network.Threads is wrong in your config file");
        return -1;
    }

    m_NetThreadsCount = static_cast<size_t> (num_threads);

    m_NetThreads = new ReactorRunnable[m_NetThreadsCount];

    // -1 means use default
    m_SockOutKBuff = sConfigMgr->GetIntDefault("Network.OutKBuff", -1);

    m_SockOutUBuff = sConfigMgr->GetIntDefault("Network.OutUBuff", 65536);

    if (m_SockOutUBuff <= 0)
    {
        SF_LOG_ERROR("misc", "Network.OutUBuff is wrong in your config file");
        return -1;
    }

    for (size_t i = 0; i < m_NetThreadsCount; ++i)
    {
        if (m_NetThreads[i].Start() == -1)
        {
            SF_LOG_ERROR("misc", "Failed to start network thread");

            for (size_t j = 0; j < i; ++j)
                m_NetThreads[j].Stop();

            for (size_t j = 0; j < i; ++j)
                m_NetThreads[j].Wait();

            return -1;
        }
    }

    m_Acceptor = new WorldSocketAcceptor;

    if (!m_Acceptor->Open(port, address))
    {
        SF_LOG_ERROR("misc", "Failed to open acceptor, check if the port is free");

        for (size_t i = 0; i < m_NetThreadsCount; ++i)
            m_NetThreads[i].Stop();

        for (size_t i = 0; i < m_NetThreadsCount; ++i)
            m_NetThreads[i].Wait();

        return -1;
    }

    return 0;
}

int
WorldSocketMgr::StartNetwork(uint16 port, const char* address)
{
    if (StartReactiveIO(port, address) == -1)
        return -1;

    sScriptMgr->OnNetworkStart();

    return 0;
}

void
WorldSocketMgr::StopNetwork()
{
    if (m_Acceptor)
    {
        m_Acceptor->Close();
    }

    if (m_NetThreadsCount != 0)
    {
        for (size_t i = 0; i < m_NetThreadsCount; ++i)
            m_NetThreads[i].Stop();
    }

    Wait();

    sScriptMgr->OnNetworkStop();
}

void
WorldSocketMgr::Wait()
{
    if (m_NetThreadsCount != 0)
    {
        for (size_t i = 0; i < m_NetThreadsCount; ++i)
            m_NetThreads[i].Wait();
    }
}

int
WorldSocketMgr::OnSocketOpen(WorldSocket* sock, ReactorRunnable* reactor)
{
    // set some options here
    if (m_SockOutKBuff >= 0)
    {
        boost::system::error_code error;
        sock->m_Socket->set_option(boost::asio::socket_base::send_buffer_size(m_SockOutKBuff), error);
        if (error)
        {
            SF_LOG_ERROR("misc", "WorldSocketMgr::OnSocketOpen set_option SO_SNDBUF error = %d", error.value());
            return -1;
        }
    }

    // Set TCP_NODELAY.
    if (m_UseNoDelay)
    {
        boost::system::error_code error;
        sock->m_Socket->set_option(boost::asio::ip::tcp::no_delay(true), error);
        if (error)
        {
            SF_LOG_ERROR("misc", "WorldSocketMgr::OnSocketOpen: set_option TCP_NODELAY error = %d", error.value());
            return -1;
        }
    }

    sock->m_OutBufferSize = static_cast<size_t> (m_SockOutUBuff);

    if (sock->Initialize() == -1)
        return -1;

    return reactor->AddSocket(sock);
}

ReactorRunnable*
WorldSocketMgr::SelectNetworkThread()
{
    ASSERT(m_NetThreadsCount >= 1);

    size_t min = 0;

    for (size_t i = 1; i < m_NetThreadsCount; ++i)
        if (m_NetThreads[i].Connections() < m_NetThreads[min].Connections())
            min = i;

    return &m_NetThreads[min];
}

boost::asio::io_context&
WorldSocketMgr::GetNetworkIoContext(ReactorRunnable* reactor)
{
    return reactor->GetIoContext();
}
