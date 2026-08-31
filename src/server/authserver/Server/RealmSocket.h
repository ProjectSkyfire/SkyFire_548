/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_REALMSOCKET_H
#define SF_REALMSOCKET_H

#include "Common.h"
#include "PacketLogServer.h"
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>
#include <array>
#include <atomic>
#include <deque>
#include <memory>
#include <string>
#include <vector>

typedef boost::asio::ip::tcp::socket RealmSocketHandle;

class RealmSocket : public std::enable_shared_from_this<RealmSocket>
{
public:
    class Session
    {
    public:
        Session(void);
        virtual ~Session(void);

        virtual void OnRead(void) = 0;
        virtual void OnAccept(void) = 0;
        virtual void OnClose(void) = 0;
    };

    RealmSocket(std::unique_ptr<RealmSocketHandle> socket, std::string remoteAddress, uint16 remotePort);
    ~RealmSocket(void);

    void Start();
    void Close();

    size_t GetAvailableBytes(void) const;
    bool PeekBytes(void* buf, size_t len, size_t offset = 0) const;
    bool ReadBytes(void* buf, size_t len);
    void DiscardBytes(size_t len);

    bool QueueSend(void const* buf, size_t len, bool closeWhenSent = false);

    boost::asio::any_io_executor GetExecutor() const;
    void SetTcpNoDelay(bool enable);

    const std::string& getRemoteAddress(void) const;
    uint16 getRemotePort(void) const;
    void SetPacketLogAccountName(std::string accountName);

    void set_session(std::unique_ptr<Session> session);

private:
    void Run();
    void AsyncRead();
    void HandleRead(boost::system::error_code const& error, size_t bytesTransferred);
    void QueueWrite(std::vector<char> data);
    void StartAsyncWrite();
    void HandleWrite(boost::system::error_code const& error);
    void CloseSocket();
    void NotifyClose();
    void CompactInputBuffer();
    Skyfire::PacketLogServerSessionInfo BuildPacketLogSessionInfo() const;
    void LogAuthPacket(void const* data, size_t len, Skyfire::PacketLogServerDirection direction);

    bool IsOpen(void) const;

    std::unique_ptr<RealmSocketHandle> _socket;
    std::array<char, 4096> _readBuffer;
    std::vector<char> _inputBuffer;
    size_t _inputReadPos;
    std::unique_ptr<Session> _session;
    std::string _remoteAddress;
    std::string _packetLogAccountName;
    uint16 _remotePort;
    std::deque<std::vector<char>> _writeQueue;
    bool _writeInProgress;
    bool _closeWhenWritesFlush;
    std::atomic<bool> _closed;
    std::atomic<bool> _closeNotified;
};

#endif
