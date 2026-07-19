/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/** \addtogroup u2w User to World Communication
 * @{
 * \file WorldSocket.h
 * \author Derex <derex101@gmail.com>
 */

#ifndef SF_WORLDSOCKET_H
#define SF_WORLDSOCKET_H

#include "AuthCrypt.h"
#include "Common.h"
#include "Platform/Threading.h"
#include "SharedDefines.h"
#include "WorldSocketSessionState.h"
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>
#include <array>
#include <atomic>
#include <chrono>
#include <deque>
#include <functional>
#include <memory>
#include <mutex>
#include <vector>

typedef boost::asio::ip::tcp::socket WorldSocketHandle;

namespace Skyfire
{
namespace Net
{
    template<class AsyncWriteStream>
    class BoostAsioWriteQueue;
}
}

class WorldPacket;
class WorldSession;
struct PacketLogSessionInfo;

/// Handler that can communicate over stream sockets.
class WorldSocket
{
public:
    WorldSocket(std::unique_ptr<WorldSocketHandle> socket, std::string remoteAddress);
    ~WorldSocket(void);

    friend class WorldSocketMgr;
    friend class ReactorRunnable;

    /// Mutex type used for various synchronizations.
    typedef Skyfire::Mutex LockType;
    typedef std::unique_lock<LockType> GuardType;

    /// Check if socket is closed.
    bool IsClosed(void) const;

    /// Close the socket.
    void CloseSocket(void);

    /// Get address of connected peer.
    const std::string& GetRemoteAddress(void) const;

    /// Send A packet on the socket, this function is reentrant.
    /// @param pct packet to send
    /// @return -1 of failure
    int SendPacket(const WorldPacket& pct);

    /// Writes a readable marker into the per-session packet log.
    void LogPacketMarker(std::string const& marker);

    /// Refreshes packet log metadata after a session gains character context.
    void RefreshPacketLogSessionInfo();

    /// Add reference to this object.
    long AddReference(void);

    /// Remove reference to this object.
    long RemoveReference(void);

    /// Called after socket accept and manager setup.
    int Initialize(void);

    /// Starts asynchronous socket processing.
    void Start(std::function<void(WorldSocket*)> closeHandler);

    /// Detaches the owning world session when the session is being removed.
    void DetachSession(WorldSession* session);

    /// Returns true when outgoing data is waiting to be flushed.
    bool HasPendingOutput(void) const;

private:
    /// Helper functions for processing incoming data.
    int handle_input_header(void);
    int handle_input_payload(void);
    int handle_input_missing_data(char const* data, size_t length);

    /// process one incoming packet.
    /// @param new_pct received packet, note that you need to delete it.
    int ProcessIncoming(WorldPacket* new_pct);

    /// Called by ProcessIncoming() on CMSG_AUTH_SESSION.
    int HandleAuthSession(WorldPacket& recvPacket);

    /// Called by ProcessIncoming() on CMSG_PING.
    int HandlePing(WorldPacket& recvPacket);

    /// Called by MSG_VERIFY_CONNECTIVITY_RESPONSE
    int HandleSendAuthSession();

private:
    void SendAuthResponseError(ResponseCodes code);
    bool IsValidSocket(void) const;
    void StartAsyncRead();
    void QueueSerializedPacket(std::vector<char> data);
    void HandleAsyncRead(boost::system::error_code const& error, size_t transferredBytes);
    void HandleAsyncWrite(boost::system::error_code const& error, size_t transferredBytes);
    void NotifyClosed();
    PacketLogSessionInfo BuildPacketLogSessionInfo() const;

    /// Time in which the last ping was received
    std::chrono::steady_clock::time_point m_LastPingTime;
    bool m_HasLastPingTime;

    /// Keep track of over-speed pings, to prevent ping flood.
    uint32 m_OverSpeedPings;

    /// Address of the remote peer
    std::string m_Address;

    /// Class used for managing encryption of the headers
    AuthCrypt m_Crypt;

    /// Session to which received packets are routed
    WorldSocketSessionState m_SessionState;

    /// here are stored the fragments of the received data
    WorldPacket* m_RecvWPct;

    size_t m_RecvPctRead;
    std::vector<uint8> m_Header;
    size_t m_HeaderRead;
    std::vector<uint8> m_WorldHeader;
    size_t m_WorldHeaderRead;

    /// Mutex for protecting packet serialization and pre-start output.
    mutable LockType m_SendLock;

    /// Queue used before the socket manager starts asynchronous processing.
    std::deque<std::vector<char>> m_PendingOutput;

    /// Configured output buffer size retained for network option compatibility.
    size_t m_OutBufferSize;
    bool m_Started;

    std::array<uint8, 4> m_Seed;
    std::array<char, 4096> m_ReadBuffer;

    std::unique_ptr<WorldSocketHandle> m_Socket;
    std::unique_ptr<Skyfire::Net::BoostAsioWriteQueue<WorldSocketHandle>> m_WriteQueue;
    std::function<void(WorldSocket*)> m_CloseHandler;
    std::atomic<long> m_ReferenceCount;
    std::atomic<bool> m_Closed;
    std::atomic<bool> m_CloseNotified;

    WorldSocket(WorldSocket const& right) = delete;
    WorldSocket& operator=(WorldSocket const& right) = delete;
};

#endif  /* _WORLDSOCKET_H */

/// @}
