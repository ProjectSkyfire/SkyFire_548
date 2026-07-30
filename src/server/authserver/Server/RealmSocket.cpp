/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Log.h"
#include "Network/BoostAsioUtils.h"
#include "RealmSocket.h"
#include <boost/asio/buffer.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/write.hpp>
#include <algorithm>
#include <cstring>
#include <string>
#include <utility>

namespace
{
std::string GetAuthOpcodeNameForLogging(uint8 opcode)
{
    switch (opcode)
    {
        case 0x00:
            return "AUTH_LOGON_CHALLENGE";
        case 0x01:
            return "AUTH_LOGON_PROOF";
        case 0x02:
            return "AUTH_RECONNECT_CHALLENGE";
        case 0x03:
            return "AUTH_RECONNECT_PROOF";
        case 0x10:
            return "REALM_LIST";
        case 0x32:
            return "XFER_ACCEPT";
        case 0x33:
            return "XFER_RESUME";
        case 0x34:
            return "XFER_CANCEL";
        default:
            return "UNKNOWN_AUTH_OPCODE";
    }
}
}

RealmSocket::Session::Session(void) { }

RealmSocket::Session::~Session(void) { }

RealmSocket::RealmSocket(std::unique_ptr<RealmSocketHandle> socket, std::string remoteAddress, uint16 remotePort) :
    _socket(std::move(socket)), _readBuffer(), _inputBuffer(), _inputReadPos(0), _session(),
    _remoteAddress(std::move(remoteAddress)), _packetLogAccountName(), _remotePort(remotePort), _writeQueue(),
    _writeInProgress(false), _closed(false), _closeNotified(false)
{
    _inputBuffer.reserve(4096);
}

RealmSocket::~RealmSocket(void)
{
    CloseSocket();
}

void RealmSocket::Start()
{
    if (_session)
        _session->OnAccept();

    AsyncRead();
}

void RealmSocket::Close()
{
    CloseSocket();
}

const std::string& RealmSocket::getRemoteAddress(void) const
{
    return _remoteAddress;
}

uint16 RealmSocket::getRemotePort(void) const
{
    return _remotePort;
}

void RealmSocket::SetPacketLogAccountName(std::string accountName)
{
    _packetLogAccountName = std::move(accountName);
    sPacketLogServer->RefreshSessionInfo(this, BuildPacketLogSessionInfo());
}

size_t RealmSocket::GetAvailableBytes(void) const
{
    return _inputBuffer.size() - _inputReadPos;
}

bool RealmSocket::PeekBytes(void* buf, size_t len, size_t offset) const
{
    if (len == 0)
        return true;

    if (buf == NULL)
        return false;

    size_t availableBytes = GetAvailableBytes();
    if (offset > availableBytes || len > availableBytes - offset)
        return false;

    memcpy(buf, _inputBuffer.data() + _inputReadPos + offset, len);
    return true;
}

bool RealmSocket::ReadBytes(void* buf, size_t len)
{
    bool ret = PeekBytes(buf, len);

    if (ret)
        DiscardBytes(len);

    return ret;
}

void RealmSocket::DiscardBytes(size_t len)
{
    _inputReadPos = std::min(_inputReadPos + len, _inputBuffer.size());
}

bool RealmSocket::QueueSend(void const* buf, size_t len)
{
    if (buf == NULL || len == 0)
        return true;

    if (_closed)
        return false;

    char const* bytes = static_cast<char const*>(buf);
    std::vector<char> data(bytes, bytes + len);
    LogAuthPacket(data.data(), data.size(), Skyfire::PACKET_LOG_SERVER_TO_CLIENT);

    std::shared_ptr<RealmSocket> self = shared_from_this();
    boost::asio::post(_socket->get_executor(),
        [self, data = std::move(data)]() mutable
        {
            self->QueueWrite(std::move(data));
        });

    return true;
}

void RealmSocket::set_session(std::unique_ptr<Session> session)
{
    _session = std::move(session);
}

void RealmSocket::Run()
{
    AsyncRead();
}

void RealmSocket::AsyncRead()
{
    if (_closed || !IsOpen())
        return;

    std::shared_ptr<RealmSocket> self = shared_from_this();
    _socket->async_read_some(boost::asio::buffer(_readBuffer),
        [self](boost::system::error_code const& error, size_t bytesTransferred)
        {
            self->HandleRead(error, bytesTransferred);
        });
}

void RealmSocket::HandleRead(boost::system::error_code const& error, size_t bytesTransferred)
{
    if (error || bytesTransferred == 0)
    {
        CloseSocket();
        return;
    }

    _inputBuffer.insert(_inputBuffer.end(), _readBuffer.data(), _readBuffer.data() + bytesTransferred);
    LogAuthPacket(_readBuffer.data(), bytesTransferred, Skyfire::PACKET_LOG_CLIENT_TO_SERVER);

    if (_session)
    {
        _session->OnRead();
        CompactInputBuffer();
    }

    if (!_closed)
        AsyncRead();
}

void RealmSocket::QueueWrite(std::vector<char> data)
{
    if (_closed || !IsOpen())
        return;

    bool startWrite = !_writeInProgress && _writeQueue.empty();
    _writeQueue.push_back(std::move(data));

    if (startWrite)
        StartAsyncWrite();
}

void RealmSocket::StartAsyncWrite()
{
    if (_closed || _writeInProgress || _writeQueue.empty() || !IsOpen())
        return;

    _writeInProgress = true;

    std::shared_ptr<RealmSocket> self = shared_from_this();
    boost::asio::async_write(*_socket, boost::asio::buffer(_writeQueue.front()),
        [self](boost::system::error_code const& error, size_t)
        {
            self->HandleWrite(error);
        });
}

void RealmSocket::HandleWrite(boost::system::error_code const& error)
{
    if (!_writeQueue.empty())
        _writeQueue.pop_front();

    _writeInProgress = false;

    if (error)
    {
        if (error != boost::asio::error::operation_aborted)
        {
            SF_LOG_DEBUG("server.authserver", "Socket send failed for %s:%u with error %d",
                _remoteAddress.c_str(), _remotePort, error.value());
        }

        CloseSocket();
        return;
    }

    if (!_writeQueue.empty())
        StartAsyncWrite();
}

bool RealmSocket::IsOpen(void) const
{
    return _socket && _socket->is_open();
}

void RealmSocket::CloseSocket()
{
    bool expected = false;
    if (!_closed.compare_exchange_strong(expected, true))
        return;

    if (IsOpen())
        Skyfire::Net::CloseTcpSocket(*_socket);

    NotifyClose();
}

void RealmSocket::NotifyClose()
{
    bool expected = false;
    if (!_closeNotified.compare_exchange_strong(expected, true))
        return;

    if (_session)
        _session->OnClose();

    sPacketLogServer->CloseSession(this);
}

void RealmSocket::CompactInputBuffer()
{
    if (_inputReadPos == 0)
        return;

    if (_inputReadPos >= _inputBuffer.size())
        _inputBuffer.clear();
    else
        _inputBuffer.erase(_inputBuffer.begin(), _inputBuffer.begin() + ptrdiff_t(_inputReadPos));

    _inputReadPos = 0;
}

Skyfire::PacketLogServerSessionInfo RealmSocket::BuildPacketLogSessionInfo() const
{
    Skyfire::PacketLogServerSessionInfo sessionInfo;
    sessionInfo.RemoteAddress = _remoteAddress;
    sessionInfo.AccountName = _packetLogAccountName;
    sessionInfo.SessionName = "auth";
    sessionInfo.FilePrefix = "authsession";
    return sessionInfo;
}

void RealmSocket::LogAuthPacket(void const* data, size_t len, Skyfire::PacketLogServerDirection direction)
{
    if (!sPacketLogServer->CanLogPacket())
        return;

    uint32 opcode = 0;
    if (data && len > 0)
        opcode = *static_cast<uint8 const*>(data);

    sPacketLogServer->LogPacket(this, BuildPacketLogSessionInfo(), direction, opcode,
        GetAuthOpcodeNameForLogging(uint8(opcode)), data, len);
}
