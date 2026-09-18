/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "RealmSocket.h"

#include <boost/asio/buffer.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>
#include <boost/system/error_code.hpp>

#include <array>
#include <chrono>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#ifdef _WIN32
#include <winsock2.h>
#endif

namespace
{
    using boost::asio::ip::tcp;

#ifdef _WIN32
    class WinsockScope
    {
    public:
        WinsockScope()
        {
            WSAStartup(MAKEWORD(2, 2), &_data);
        }

        ~WinsockScope()
        {
            WSACleanup();
        }

    private:
        WSADATA _data;
    };
#endif

    std::shared_ptr<RealmSocket> CreateConnectedRealmSocket(boost::asio::io_context& ioContext, tcp::socket& peer, bool authnet = false)
    {
        tcp::acceptor acceptor(ioContext, tcp::endpoint(tcp::v4(), 0));

        peer = tcp::socket(ioContext);
        peer.connect(tcp::endpoint(boost::asio::ip::address_v4::loopback(), acceptor.local_endpoint().port()));
        peer.non_blocking(true);

        std::unique_ptr<RealmSocketHandle> serverSocket(new RealmSocketHandle(ioContext));
        acceptor.accept(*serverSocket);

        return std::make_shared<RealmSocket>(std::move(serverSocket), "127.0.0.1", peer.local_endpoint().port(), authnet);
    }

    bool IsWouldBlock(boost::system::error_code const& error)
    {
        return error == boost::asio::error::would_block || error == boost::asio::error::try_again;
    }

    bool TryRead(tcp::socket& peer, std::string& received, std::string& errorText)
    {
        std::array<char, 64> buffer = {};
        boost::system::error_code error;
        size_t bytes = peer.read_some(boost::asio::buffer(buffer), error);

        if (!error)
        {
            received.append(buffer.data(), bytes);
            return true;
        }

        if (IsWouldBlock(error))
            return false;

        errorText = error.message();
        return false;
    }

    class CountingSession : public RealmSocket::Session
    {
    public:
        explicit CountingSession(uint32& closeCount) : _closeCount(closeCount)
        {
        }

        void OnRead() override
        {
        }

        void OnAccept() override
        {
        }

        void OnClose() override
        {
            ++_closeCount;
        }

    private:
        uint32& _closeCount;
    };

    class BufferSession : public RealmSocket::Session
    {
    public:
        explicit BufferSession(bool& passed) : _passed(passed)
        {
        }

        void OnRead() override
        {
            if (_socket == nullptr)
                return;

            if (_socket->GetAvailableBytes() != 5)
                return;

            char peeked[3] = {};
            if (!_socket->PeekBytes(peeked, sizeof(peeked)) || std::string(peeked, sizeof(peeked)) != "abc")
                return;

            if (_socket->GetAvailableBytes() != 5)
                return;

            char offsetPeek[2] = {};
            if (!_socket->PeekBytes(offsetPeek, sizeof(offsetPeek), 3) || std::string(offsetPeek, sizeof(offsetPeek)) != "de")
                return;

            char firstRead[2] = {};
            if (!_socket->ReadBytes(firstRead, sizeof(firstRead)) || std::string(firstRead, sizeof(firstRead)) != "ab")
                return;

            if (_socket->GetAvailableBytes() != 3)
                return;

            _socket->DiscardBytes(1);

            char secondRead[2] = {};
            if (!_socket->ReadBytes(secondRead, sizeof(secondRead)) || std::string(secondRead, sizeof(secondRead)) != "de")
                return;

            _passed = _socket->GetAvailableBytes() == 0;
        }

        void OnAccept() override
        {
        }

        void OnClose() override
        {
        }

        void SetSocket(RealmSocket& socket)
        {
            _socket = &socket;
        }

    private:
        bool& _passed;
        RealmSocket* _socket = nullptr;
    };
}

int main()
{
#ifdef _WIN32
    WinsockScope winsock;
#endif

    boost::asio::io_context ioContext;
    tcp::socket peer(ioContext);
    std::shared_ptr<RealmSocket> socket = CreateConnectedRealmSocket(ioContext, peer);

    tcp::socket authnetPeer(ioContext);
    auto authnetSocket = CreateConnectedRealmSocket(ioContext, authnetPeer, true);
    if (RealmSocket::GetActiveConnections(false) != 1 || RealmSocket::GetActiveConnections(true) != 1)
    {
        std::cerr << "Authnet and legacy connection counts are not independent\n";
        return 1;
    }
    authnetSocket->Close();
    authnetSocket->Close();
    authnetSocket.reset();
    if (RealmSocket::GetActiveConnections(false) != 1 || RealmSocket::GetActiveConnections(true) != 0)
    {
        std::cerr << "Repeated close/destruction corrupted active connection counts\n";
        return 1;
    }

    char const payload[] = "queued-auth-write";
    if (!socket->QueueSend(payload, std::strlen(payload)))
    {
        std::cerr << "RealmSocket::QueueSend rejected an open socket\n";
        return 1;
    }

    std::string received;
    std::string errorText;
    if (TryRead(peer, received, errorText))
    {
        std::cerr << "RealmSocket::QueueSend wrote before the io_context was polled\n";
        return 1;
    }

    if (!errorText.empty())
    {
        std::cerr << "Unexpected peer read error before io_context poll: " << errorText << '\n';
        return 1;
    }

    for (uint32 i = 0; i < 200 && received.size() < std::strlen(payload); ++i)
    {
        ioContext.restart();
        ioContext.poll();

        errorText.clear();
        TryRead(peer, received, errorText);
        if (!errorText.empty())
        {
            std::cerr << "Unexpected peer read error after io_context poll: " << errorText << '\n';
            return 1;
        }

        if (received.size() < std::strlen(payload))
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    if (received != payload)
    {
        std::cerr << "Queued write payload mismatch. Expected '" << payload << "', got '" << received << "'\n";
        return 1;
    }

    socket->Close();

    tcp::socket closePeer(ioContext);
    std::shared_ptr<RealmSocket> closeSocket = CreateConnectedRealmSocket(ioContext, closePeer);
    uint32 closeCount = 0;
    closeSocket->set_session(std::unique_ptr<RealmSocket::Session>(new CountingSession(closeCount)));

    closeSocket->Close();
    closeSocket->Close();

    if (closeCount != 1)
    {
        std::cerr << "RealmSocket notified close " << closeCount << " times\n";
        return 1;
    }

    tcp::socket bufferPeer(ioContext);
    std::shared_ptr<RealmSocket> bufferSocket = CreateConnectedRealmSocket(ioContext, bufferPeer);
    bool bufferApiPassed = false;
    std::unique_ptr<BufferSession> bufferSession(new BufferSession(bufferApiPassed));
    bufferSession->SetSocket(*bufferSocket);
    bufferSocket->set_session(std::move(bufferSession));
    bufferSocket->Start();

    boost::asio::write(bufferPeer, boost::asio::buffer("abcde", 5));

    for (uint32 i = 0; i < 200 && !bufferApiPassed; ++i)
    {
        ioContext.restart();
        ioContext.poll();

        if (!bufferApiPassed)
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    if (!bufferApiPassed)
    {
        std::cerr << "RealmSocket explicit buffer API did not preserve peek/read/discard semantics\n";
        return 1;
    }

    return 0;
}
