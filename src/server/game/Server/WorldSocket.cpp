/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include <ace/Message_Block.h>
#include <ace/OS_NS_string.h>
#include <ace/OS_NS_unistd.h>
#include <ace/os_include/arpa/os_inet.h>
#include <ace/os_include/netinet/os_tcp.h>
#include <ace/os_include/sys/os_types.h>
#include <ace/os_include/sys/os_socket.h>
#include <ace/OS_NS_string.h>
#include <ace/Reactor.h>
#include <ace/Auto_Ptr.h>

#include "WorldSocket.h"
#include "Common.h"
#include "Player.h"
#include "Util.h"
#include "World.h"
#include "WorldPacket.h"
#include "SharedDefines.h"
#include "ByteBuffer.h"
#include "Opcodes.h"
#include "DatabaseEnv.h"
#include "BigNumber.h"
#include "SHA1.h"
#include "WorldSession.h"
#include "WorldSocketMgr.h"
#include "Log.h"
#include "PacketLog.h"
#include "ScriptMgr.h"
#include "AccountMgr.h"

#if defined(__GNUC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

struct ServerPktHeader
{
    ServerPktHeader(uint32 size, uint32 cmd, AuthCrypt* _authCrypt) : size(size)
    {
        if (_authCrypt->IsInitialized())
        {
            uint32 data = (size << 13) | (cmd & MAX_OPCODE);
            memcpy(&header[0], &data, 4);
            _authCrypt->EncryptSend((uint8*)&header[0], getHeaderLength());
        }
        else
        {
            // Dynamic header size is not needed anymore, we are using not encrypted part for only the first few packets
            memcpy(&header[0], &size, 2);
            memcpy(&header[2], &cmd, 2);
        }
    }

    uint8 getHeaderLength()
    {
        return 4;
    }

    const uint32 size;
    uint8 header[4];
};

struct AuthClientPktHeader
{
    uint16 size;
    uint32 cmd;
};

struct WorldClientPktHeader
{
    uint16 size;
    uint16 cmd;
};

#if defined(__GNUC__)
#pragma pack()
#else
#pragma pack(pop)
#endif

WorldSocket::WorldSocket (void): WorldHandler(),
m_LastPingTime(ACE_Time_Value::zero), m_OverSpeedPings(0), m_Session(0),
m_RecvWPct(0), m_RecvPct(), m_Header(sizeof(AuthClientPktHeader)),
m_WorldHeader(sizeof(WorldClientPktHeader)), m_OutBuffer(0),
m_OutBufferSize(65536), m_OutActive(false),

m_Seed(static_cast<uint32> (rand32()))
{
    reference_counting_policy().value (ACE_Event_Handler::Reference_Counting_Policy::ENABLED);

    msg_queue()->high_water_mark(8 * 1024 * 1024);
    msg_queue()->low_water_mark(8 * 1024 * 1024);
}

WorldSocket::~WorldSocket (void)
{
    delete m_RecvWPct;

    if (m_OutBuffer)
        m_OutBuffer->release();

    closing_ = true;

    peer().close();
}

bool WorldSocket::IsClosed (void) const
{
    return closing_;
}

void WorldSocket::CloseSocket (void)
{
    {
        ACE_GUARD (LockType, Guard, m_OutBufferLock);

        if (closing_)
            return;

        closing_ = true;
        peer().close_writer();
    }

    {
        ACE_GUARD (LockType, Guard, m_SessionLock);

        m_Session = NULL;
    }
}

const std::string& WorldSocket::GetRemoteAddress (void) const
{
    return m_Address;
}

int WorldSocket::SendPacket(WorldPacket const& pct)
{
    ACE_GUARD_RETURN (LockType, Guard, m_OutBufferLock, -1);

    if (closing_)
        return -1;

    // Dump outgoing packet
    if (sPacketLog->CanLogPacket())
        sPacketLog->LogPacket(pct, SERVER_TO_CLIENT);

    WorldPacket const* pkt = &pct;

    // Empty buffer used in case packet should be compressed
    // Disable compression for now :)
   /* WorldPacket buff;
    if (m_Session && pkt->size() > 0x400)
    {
        buff.Compress(m_Session->GetCompressionStream(), pkt);
        pkt = &buff;
    }*/

    uint16 opcodeNumber = serverOpcodeTable[pkt->GetOpcode()]->OpcodeNumber;

    if (m_Session)
        SF_LOG_TRACE("network.opcode", "S->C: %s %s", m_Session->GetPlayerInfo().c_str(), GetOpcodeNameForLogging(pkt->GetOpcode(), true).c_str());

    sScriptMgr->OnPacketSend(this, *pkt);

    ServerPktHeader header(!m_Crypt.IsInitialized() ? pkt->size() + 2 : pct.size(), opcodeNumber, &m_Crypt);

    if (m_OutBuffer->space() >= pkt->size() + header.getHeaderLength() && msg_queue()->is_empty())
    {
        // Put the packet on the buffer.
        if (m_OutBuffer->copy((char*) header.header, header.getHeaderLength()) == -1)
            ACE_ASSERT (false);

        if (!pkt->empty())
            if (m_OutBuffer->copy((char*) pkt->contents(), pkt->size()) == -1)
                ACE_ASSERT (false);
    }
    else
    {
        // Enqueue the packet.
        ACE_Message_Block* mb;

        ACE_NEW_RETURN(mb, ACE_Message_Block(pkt->size() + header.getHeaderLength()), -1);

        mb->copy((char*) header.header, header.getHeaderLength());

        if (!pkt->empty())
            mb->copy((const char*)pkt->contents(), pkt->size());

        if (msg_queue()->enqueue_tail(mb, (ACE_Time_Value*)&ACE_Time_Value::zero) == -1)
        {
            SF_LOG_ERROR("network", "WorldSocket::SendPacket enqueue_tail failed");
            mb->release();
            return -1;
        }
    }

    return 0;
}

long WorldSocket::AddReference (void)
{
    return static_cast<long> (add_reference());
}

long WorldSocket::RemoveReference (void)
{
    return static_cast<long> (remove_reference());
}

int WorldSocket::open (void *a)
{
    ACE_UNUSED_ARG (a);

    // Prevent double call to this func.
    if (m_OutBuffer)
        return -1;

    // This will also prevent the socket from being Updated
    // while we are initializing it.
    m_OutActive = true;

    // Hook for the manager.
    if (sWorldSocketMgr->OnSocketOpen(this) == -1)
        return -1;

    // Allocate the buffer.
    ACE_NEW_RETURN (m_OutBuffer, ACE_Message_Block (m_OutBufferSize), -1);

    // Store peer address.
    ACE_INET_Addr remote_addr;

    if (peer().get_remote_addr(remote_addr) == -1)
    {
        SF_LOG_ERROR("network", "WorldSocket::open: peer().get_remote_addr errno = %s", ACE_OS::strerror (errno));
        return -1;
    }

    m_Address = remote_addr.get_host_addr();

    // not an opcode. this packet sends raw string WORLD OF WARCRAFT CONNECTION - SERVER TO CLIENT"
    // because of our implementation, bytes "WO" become the opcode
    WorldPacket packet(MSG_VERIFY_CONNECTIVITY);
    packet << std::string("RLD OF WARCRAFT CONNECTION - SERVER TO CLIENT");

    if (SendPacket(packet) == -1)
        return -1;

    // Register with ACE Reactor
    if (reactor()->register_handler(this, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::WRITE_MASK) == -1)
    {
        SF_LOG_ERROR("network", "WorldSocket::open: unable to register client handler errno = %s", ACE_OS::strerror (errno));
        return -1;
    }

    // reactor takes care of the socket from now on
    remove_reference();

    return 0;
}

int WorldSocket::close (u_long)
{
    shutdown();

    closing_ = true;

    remove_reference();

    return 0;
}

int WorldSocket::handle_input (ACE_HANDLE)
{
    if (closing_)
        return -1;

    switch (handle_input_missing_data())
    {
        case -1 :
        {
            if ((errno == EWOULDBLOCK) ||
                (errno == EAGAIN))
            {
                return Update();                           // interesting line, isn't it ?
            }

            SF_LOG_DEBUG("network", "WorldSocket::handle_input: Peer error closing connection errno = %s", ACE_OS::strerror (errno));

            errno = ECONNRESET;
            return -1;
        }
        case 0:
        {
            SF_LOG_DEBUG("network", "WorldSocket::handle_input: Peer has closed connection");

            errno = ECONNRESET;
            return -1;
        }
        case 1:
            return 1;
        default:
            return Update();                               // another interesting line ;)
    }

    ACE_NOTREACHED(return -1);
}

int WorldSocket::handle_output (ACE_HANDLE)
{
    ACE_GUARD_RETURN (LockType, Guard, m_OutBufferLock, -1);

    if (closing_)
        return -1;

    size_t send_len = m_OutBuffer->length();

    if (send_len == 0)
        return handle_output_queue(Guard);

#ifdef MSG_NOSIGNAL
    ssize_t n = peer().send (m_OutBuffer->rd_ptr(), send_len, MSG_NOSIGNAL);
#else
    ssize_t n = peer().send (m_OutBuffer->rd_ptr(), send_len);
#endif // MSG_NOSIGNAL

    if (n == 0)
        return -1;
    else if (n == -1)
    {
        if (errno == EWOULDBLOCK || errno == EAGAIN)
            return schedule_wakeup_output (Guard);

        return -1;
    }
    else if (n < (ssize_t)send_len) //now n > 0
    {
        m_OutBuffer->rd_ptr (static_cast<size_t> (n));

        // move the data to the base of the buffer
        m_OutBuffer->crunch();

        return schedule_wakeup_output (Guard);
    }
    else //now n == send_len
    {
        m_OutBuffer->reset();

        return handle_output_queue (Guard);
    }

    ACE_NOTREACHED (return 0);
}

int WorldSocket::handle_output_queue (GuardType& g)
{
    if (msg_queue()->is_empty())
        return cancel_wakeup_output(g);

    ACE_Message_Block* mblk;

    if (msg_queue()->dequeue_head(mblk, (ACE_Time_Value*)&ACE_Time_Value::zero) == -1)
    {
        SF_LOG_ERROR("network", "WorldSocket::handle_output_queue dequeue_head");
        return -1;
    }

    const size_t send_len = mblk->length();

#ifdef MSG_NOSIGNAL
    ssize_t n = peer().send(mblk->rd_ptr(), send_len, MSG_NOSIGNAL);
#else
    ssize_t n = peer().send(mblk->rd_ptr(), send_len);
#endif // MSG_NOSIGNAL

    if (n == 0)
    {
        mblk->release();

        return -1;
    }
    else if (n == -1)
    {
        if (errno == EWOULDBLOCK || errno == EAGAIN)
        {
            msg_queue()->enqueue_head(mblk, (ACE_Time_Value*) &ACE_Time_Value::zero);
            return schedule_wakeup_output (g);
        }

        mblk->release();
        return -1;
    }
    else if (n < (ssize_t)send_len) //now n > 0
    {
        mblk->rd_ptr(static_cast<size_t> (n));

        if (msg_queue()->enqueue_head(mblk, (ACE_Time_Value*) &ACE_Time_Value::zero) == -1)
        {
            SF_LOG_ERROR("network", "WorldSocket::handle_output_queue enqueue_head");
            mblk->release();
            return -1;
        }

        return schedule_wakeup_output (g);
    }
    else //now n == send_len
    {
        mblk->release();

        return msg_queue()->is_empty() ? cancel_wakeup_output(g) : ACE_Event_Handler::WRITE_MASK;
    }

    ACE_NOTREACHED(return -1);
}

int WorldSocket::handle_close (ACE_HANDLE h, ACE_Reactor_Mask)
{
    // Critical section
    {
        ACE_GUARD_RETURN (LockType, Guard, m_OutBufferLock, -1);

        closing_ = true;

        if (h == ACE_INVALID_HANDLE)
            peer().close_writer();
    }

    // Critical section
    {
        ACE_GUARD_RETURN (LockType, Guard, m_SessionLock, -1);

        m_Session = NULL;
    }

    reactor()->remove_handler(this, ACE_Event_Handler::DONT_CALL | ACE_Event_Handler::ALL_EVENTS_MASK);
    return 0;
}

int WorldSocket::Update (void)
{
    if (closing_)
        return -1;

    if (m_OutActive)
        return 0;

    {
        ACE_GUARD_RETURN (LockType, Guard, m_OutBufferLock, 0);
        if (m_OutBuffer->length() == 0 && msg_queue()->is_empty())
            return 0;
    }

    int ret;
    do
        ret = handle_output(get_handle());
    while (ret > 0);

    return ret;
}

int WorldSocket::handle_input_header (void)
{
    ACE_ASSERT(m_RecvWPct == NULL);


    if (m_Crypt.IsInitialized())
    {
        ACE_ASSERT(m_WorldHeader.length() == sizeof(WorldClientPktHeader));
        uint8* uintHeader = (uint8*)m_WorldHeader.rd_ptr();
        m_Crypt.DecryptRecv(uintHeader, sizeof(WorldClientPktHeader));
        WorldClientPktHeader& header = *(WorldClientPktHeader*)uintHeader;

        uint32 value = *(uint32*)uintHeader;
        header.cmd = value & 0x1FFF;
        header.size = ((value & ~(uint32)0x1FFF) >> 13);

        if (header.size > 10236)
        {
            Player* _player = m_Session ? m_Session->GetPlayer() : NULL;
            SF_LOG_ERROR("network", "WorldSocket::handle_input_header(): client (account: %u, char [GUID: %u, name: %s]) sent malformed packet (size: %d, cmd: %d)",
                m_Session ? m_Session->GetAccountId() : 0,
                _player ? _player->GetGUIDLow() : 0,
                _player ? _player->GetName().c_str() : "<none>",
                header.size, header.cmd);

            errno = EINVAL;
            return -1;
        }

        uint16 opcodeNumber = PacketFilter::DropHighBytes(header.cmd);
        ACE_NEW_RETURN(m_RecvWPct, WorldPacket(clientOpcodeTable.GetOpcodeByNumber(opcodeNumber), header.size), -1);
        m_RecvWPct->SetReceivedOpcode(opcodeNumber);

        if (header.size > 0)
        {
            m_RecvWPct->resize(header.size);
            m_RecvPct.base ((char*) m_RecvWPct->contents(), m_RecvWPct->size());
        }
        else
            ACE_ASSERT(m_RecvPct.space() == 0);
    }
    else
    {
        ACE_ASSERT(m_Header.length() == sizeof(AuthClientPktHeader));
        uint8* uintHeader = (uint8*)m_Header.rd_ptr();
        AuthClientPktHeader& header = *((AuthClientPktHeader*)uintHeader);

        if ((header.size < 4) || (header.size > 10240))
        {
            Player* _player = m_Session ? m_Session->GetPlayer() : NULL;
            SF_LOG_ERROR("network", "WorldSocket::handle_input_header(): client (account: %u, char [GUID: %u, name: %s]) sent malformed packet (size: %d, cmd: %d)",
                m_Session ? m_Session->GetAccountId() : 0,
                _player ? _player->GetGUIDLow() : 0,
                _player ? _player->GetName().c_str() : "<none>",
                header.size, header.cmd);

            errno = EINVAL;
            return -1;
        }

        header.size -= 4;

        uint16 opcodeNumber = PacketFilter::DropHighBytes(header.cmd);
        ACE_NEW_RETURN(m_RecvWPct, WorldPacket(clientOpcodeTable.GetOpcodeByNumber(opcodeNumber), header.size), -1);
        m_RecvWPct->SetReceivedOpcode(opcodeNumber);

        if (header.size > 0)
        {
            m_RecvWPct->resize(header.size);
            m_RecvPct.base ((char*) m_RecvWPct->contents(), m_RecvWPct->size());
        }
        else
            ACE_ASSERT(m_RecvPct.space() == 0);
    }

    return 0;
}
int WorldSocket::handle_input_payload (void)
{
    // set errno properly here on error !!!
    // now have a header and payload

    if (m_Crypt.IsInitialized())
    {
        ACE_ASSERT (m_RecvPct.space() == 0);
        ACE_ASSERT (m_WorldHeader.space() == 0);
        ACE_ASSERT (m_RecvWPct != NULL);

        const int ret = ProcessIncoming (m_RecvWPct);

        m_RecvPct.base (NULL, 0);
        m_RecvPct.reset();
        m_RecvWPct = NULL;

        m_WorldHeader.reset();

        if (ret == -1)
            errno = EINVAL;

        return ret;
    }
    else
    {
        ACE_ASSERT(m_RecvPct.space() == 0);
        ACE_ASSERT(m_Header.space() == 0);
        ACE_ASSERT(m_RecvWPct != NULL);

        const int ret = ProcessIncoming(m_RecvWPct);

        m_RecvPct.base(NULL, 0);
        m_RecvPct.reset();
        m_RecvWPct = NULL;

        m_Header.reset();

        if (ret == -1)
            errno = EINVAL;

        return ret;
    }
}

int WorldSocket::handle_input_missing_data (void)
{
    char buf [4096];

    ACE_Data_Block db (sizeof (buf),
                        ACE_Message_Block::MB_DATA,
                        buf,
                        0,
                        0,
                        ACE_Message_Block::DONT_DELETE,
                        0);

    ACE_Message_Block message_block(&db,
                                    ACE_Message_Block::DONT_DELETE,
                                    0);

    const size_t recv_size = message_block.space();

    const ssize_t n = peer().recv (message_block.wr_ptr(),
                                          recv_size);

    if (n <= 0)
        return int(n);

    message_block.wr_ptr (n);

    while (message_block.length() > 0)
    {
        if (m_Crypt.IsInitialized())
        {
            if (m_WorldHeader.space() > 0)
            {
                //need to receive the header
                const size_t to_header = (message_block.length() > m_WorldHeader.space() ? m_WorldHeader.space() : message_block.length());
                m_WorldHeader.copy (message_block.rd_ptr(), to_header);
                message_block.rd_ptr (to_header);

                if (m_WorldHeader.space() > 0)
                {
                    // Couldn't receive the whole header this time.
                    ACE_ASSERT (message_block.length() == 0);
                    errno = EWOULDBLOCK;
                    return -1;
                }

                // We just received nice new header
                if (handle_input_header() == -1)
                {
                    ACE_ASSERT ((errno != EWOULDBLOCK) && (errno != EAGAIN));
                    return -1;
                }
            }
        }
        else
        {
            if (m_Header.space() > 0)
            {
                //need to receive the header
                const size_t to_header = (message_block.length() > m_Header.space() ? m_Header.space() : message_block.length());
                m_Header.copy (message_block.rd_ptr(), to_header);
                message_block.rd_ptr (to_header);

                if (m_Header.space() > 0)
                {
                    // Couldn't receive the whole header this time.
                    ACE_ASSERT (message_block.length() == 0);
                    errno = EWOULDBLOCK;
                    return -1;
                }

                // We just received nice new header
                if (handle_input_header() == -1)
                {
                    ACE_ASSERT ((errno != EWOULDBLOCK) && (errno != EAGAIN));
                    return -1;
                }
            }
        }

        // Its possible on some error situations that this happens
        // for example on closing when epoll receives more chunked data and stuff
        // hope this is not hack, as proper m_RecvWPct is asserted around
        if (!m_RecvWPct)
        {
            SF_LOG_ERROR("network", "Forcing close on input m_RecvWPct = NULL");
            errno = EINVAL;
            return -1;
        }

        // We have full read header, now check the data payload
        if (m_RecvPct.space() > 0)
        {
            //need more data in the payload
            const size_t to_data = (message_block.length() > m_RecvPct.space() ? m_RecvPct.space() : message_block.length());
            m_RecvPct.copy (message_block.rd_ptr(), to_data);
            message_block.rd_ptr (to_data);

            if (m_RecvPct.space() > 0)
            {
                // Couldn't receive the whole data this time.
                ACE_ASSERT (message_block.length() == 0);
                errno = EWOULDBLOCK;
                return -1;
            }
        }

        //just received fresh new payload
        if (handle_input_payload() == -1)
        {
            ACE_ASSERT ((errno != EWOULDBLOCK) && (errno != EAGAIN));
            return -1;
        }
    }

    return size_t(n) == recv_size ? 1 : 2;
}

int WorldSocket::cancel_wakeup_output (GuardType& g)
{
    if (!m_OutActive)
        return 0;

    m_OutActive = false;

    g.release();

    if (reactor()->cancel_wakeup
        (this, ACE_Event_Handler::WRITE_MASK) == -1)
    {
        // would be good to store errno from reactor with errno guard
        SF_LOG_ERROR("network", "WorldSocket::cancel_wakeup_output");
        return -1;
    }

    return 0;
}

int WorldSocket::schedule_wakeup_output (GuardType& g)
{
    if (m_OutActive)
        return 0;

    m_OutActive = true;

    g.release();

    if (reactor()->schedule_wakeup
        (this, ACE_Event_Handler::WRITE_MASK) == -1)
    {
        SF_LOG_ERROR("network", "WorldSocket::schedule_wakeup_output");
        return -1;
    }

    return 0;
}

int WorldSocket::ProcessIncoming(WorldPacket* new_pct)
{
    ACE_ASSERT (new_pct);

    // manage memory ;)
    ACE_Auto_Ptr<WorldPacket> aptr(new_pct);

    Opcodes opcode = new_pct->GetOpcode();

    if (closing_)
        return -1;

    // Dump received packet.
    if (sPacketLog->CanLogPacket())
        sPacketLog->LogPacket(*new_pct, CLIENT_TO_SERVER);

    std::string opcodeName = GetOpcodeNameForLogging(opcode, false);
    if (m_Session)
        SF_LOG_TRACE("network.opcode", "C->S: %s %s", m_Session->GetPlayerInfo().c_str(), opcodeName.c_str());

    try
    {
        switch (opcode)
        {
            case CMSG_PING:
                return HandlePing(*new_pct);
            case CMSG_AUTH_SESSION:
                if (m_Session)
                {
                    SF_LOG_ERROR("network", "WorldSocket::ProcessIncoming: received duplicate CMSG_AUTH_SESSION from %s", m_Session->GetPlayerInfo().c_str());
                    return -1;
                }

                sScriptMgr->OnPacketReceive(this, WorldPacket(*new_pct));
                return HandleAuthSession(*new_pct);
            case CMSG_KEEP_ALIVE:
                sScriptMgr->OnPacketReceive(this, WorldPacket(*new_pct));
                return 0;
            case CMSG_LOG_DISCONNECT:
                new_pct->rfinish(); // contains uint32 disconnectReason;
                sScriptMgr->OnPacketReceive(this, WorldPacket(*new_pct));
                return 0;
            // not an opcode, client sends string "WORLD OF WARCRAFT CONNECTION - CLIENT TO SERVER" without opcode
            // first 4 bytes become the opcode (2 dropped)
            case MSG_VERIFY_CONNECTIVITY:
            {
                sScriptMgr->OnPacketReceive(this, WorldPacket(*new_pct));
                std::string str;
                *new_pct >> str;
                if (str != "D OF WARCRAFT CONNECTION - CLIENT TO SERVER")
                    return -1;
                return HandleSendAuthSession();
            }
            /*case CMSG_ENABLE_NAGLE:
            {
                SF_LOG_DEBUG("network", "%s", opcodeName.c_str());
                sScriptMgr->OnPacketReceive(this, WorldPacket(*new_pct));
                return m_Session ? m_Session->HandleEnableNagleAlgorithm() : -1;
            }*/
            default:
            {
                ACE_GUARD_RETURN(LockType, Guard, m_SessionLock, -1);
                if (!m_Session)
                {
                    SF_LOG_ERROR("network.opcode", "ProcessIncoming: Client not authed opcode = %u", uint32(opcode));
                    return -1;
                }

                // prevent invalid memory access/crash with custom opcodes
                if (opcode >= NUM_OPCODES)
                    return 0;

                OpcodeHandler const* handler = clientOpcodeTable[opcode];
                if (!handler || handler->Status == STATUS_UNHANDLED)
                {
                    SF_LOG_ERROR("network.opcode", "No defined handler for opcode %s sent by %s", GetOpcodeNameForLogging(new_pct->GetOpcode(), false, new_pct->GetReceivedOpcode()).c_str(), m_Session->GetPlayerInfo().c_str());
                    return 0;
                }

                // Our Idle timer will reset on any non PING opcodes.
                // Catches people idling on the login screen and any lingering ingame connections.
                m_Session->ResetTimeOutTime();

                // OK, give the packet to WorldSession
                aptr.release();
                // WARNING here we call it with locks held.
                // Its possible to cause deadlock if QueuePacket calls back
                m_Session->QueuePacket(new_pct);
                return 0;
            }
        }
    }
    catch (ByteBufferException &)
    {
        SF_LOG_ERROR("network", "WorldSocket::ProcessIncoming ByteBufferException occured while parsing an instant handled packet %s from client %s, accountid=%i. Disconnected client.",
                       opcodeName.c_str(), GetRemoteAddress().c_str(), m_Session ? int32(m_Session->GetAccountId()) : -1);
        new_pct->hexlike();
        return -1;
    }

    ACE_NOTREACHED (return 0);
}

int WorldSocket::HandleSendAuthSession()
{
    WorldPacket packet(SMSG_AUTH_CHALLENGE, 37);
    packet << uint16(0);

    for (int i = 0; i < 8; i++)
        packet << uint32(0);

    packet << uint8(1);
    packet << uint32(m_Seed);

    return SendPacket(packet);
}

int WorldSocket::HandleAuthSession(WorldPacket& recvPacket)
{
    uint8 digest[20];
    uint32 clientSeed;
    uint8 security;
    uint16 clientBuild;
    uint32 id;
    uint32 addonSize;
    LocaleConstant locale;
    std::string account;
    SHA1Hash sha;
    BigNumber k;
    WorldPacket addonsData;

    recvPacket.read_skip<uint32>();
    recvPacket.read_skip<uint32>();
    recvPacket >> digest[18];
    recvPacket >> digest[14];
    recvPacket >> digest[3];
    recvPacket >> digest[4];
    recvPacket >> digest[0];
    recvPacket.read_skip<uint32>();
    recvPacket >> digest[11];
    recvPacket >> clientSeed;
    recvPacket >> digest[19];
    recvPacket.read_skip<uint8>();
    recvPacket.read_skip<uint8>();
    recvPacket >> digest[2];
    recvPacket >> digest[9];
    recvPacket >> digest[12];
    recvPacket.read_skip<uint64>();
    recvPacket.read_skip<uint32>();
    recvPacket >> digest[16];
    recvPacket >> digest[5];
    recvPacket >> digest[6];
    recvPacket >> digest[8];
    recvPacket >> clientBuild;
    recvPacket >> digest[17];
    recvPacket >> digest[7];
    recvPacket >> digest[13];
    recvPacket >> digest[15];
    recvPacket >> digest[1];
    recvPacket >> digest[10];
    recvPacket >> addonSize;

    addonsData.resize(addonSize);
    recvPacket.read((uint8*)addonsData.contents(), addonSize);

    recvPacket.ReadBit();
    uint32 accountNameLength = recvPacket.ReadBits(11);

    account = recvPacket.ReadString(accountNameLength);

    if (sWorld->IsClosed())
    {
        SendAuthResponseError(ResponseCodes::AUTH_REJECT);
        SF_LOG_ERROR("network", "WorldSocket::HandleAuthSession: World closed, denying client (%s).", GetRemoteAddress().c_str());
        return -1;
    }

    // Get the account information from the realmd database
    //         0           1        2       3          4         5       6          7   8
    // SELECT id, sessionkey, last_ip, locked, expansion, mutetime, locale, recruiter, os FROM account WHERE username = ?
    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_INFO_BY_NAME);

    stmt->setString(0, account);

    PreparedQueryResult result = LoginDatabase.Query(stmt);

    // Stop if the account is not found
    if (!result)
    {
        SendAuthResponseError(ResponseCodes::AUTH_UNKNOWN_ACCOUNT);
        SF_LOG_ERROR("network", "WorldSocket::HandleAuthSession: Sent Auth Response (unknown account).");
        return -1;
    }

    Field* fields = result->Fetch();

    uint8 expansion = fields[4].GetUInt8();
    uint32 world_expansion = sWorld->getIntConfig(WorldIntConfigs::CONFIG_EXPANSION);
    if (expansion > world_expansion)
        expansion = world_expansion;

    ///- Re-check ip locking (same check as in realmd).
    if (fields[3].GetUInt8() == 1) // if ip is locked
    {
        if (strcmp (fields[2].GetCString(), GetRemoteAddress().c_str()))
        {
            SendAuthResponseError(ResponseCodes::AUTH_FAILED);
            SF_LOG_DEBUG("network", "WorldSocket::HandleAuthSession: Sent Auth Response (Account IP differs).");
            return -1;
        }
    }

    id = fields[0].GetUInt32();

    k.SetHexStr(fields[1].GetCString());

    int64 mutetime = fields[5].GetInt64();
    //! Negative mutetime indicates amount of seconds to be muted effective on next login - which is now.
    if (mutetime < 0)
    {
        mutetime = time(NULL) + llabs(mutetime);

        stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_MUTE_TIME_LOGIN);

        stmt->setInt64(0, mutetime);
        stmt->setUInt32(1, id);

        LoginDatabase.Execute(stmt);
    }

    locale = LocaleConstant (fields[6].GetUInt8());
    if (locale >= TOTAL_LOCALES)
        locale = LOCALE_enUS;

    uint32 recruiter = fields[7].GetUInt32();
    std::string os = fields[8].GetString();
    bool hasBoost = fields[9].GetBool();

    // Must be done before WorldSession is created
    if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_WARDEN_ENABLED) && os != "Win" && os != "OSX")
    {
        SendAuthResponseError(ResponseCodes::AUTH_REJECT);
        SF_LOG_ERROR("network", "WorldSocket::HandleAuthSession: Client %s attempted to log in using invalid client OS (%s).", GetRemoteAddress().c_str(), os.c_str());
        return -1;
    }

    // Checks gmlevel per Realm
    stmt = LoginDatabase.GetPreparedStatement(LOGIN_GET_GMLEVEL_BY_REALMID);

    stmt->setUInt32(0, id);
    stmt->setInt32(1, int32(realmID));

    result = LoginDatabase.Query(stmt);

    if (!result)
        security = 0;
    else
    {
        fields = result->Fetch();
        security = fields[0].GetUInt8();
    }

    // Re-check account ban (same check as in realmd)
    stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_BANS);

    stmt->setUInt32(0, id);
    stmt->setString(1, GetRemoteAddress());

    PreparedQueryResult banresult = LoginDatabase.Query(stmt);

    if (banresult) // if account banned
    {
        SendAuthResponseError(ResponseCodes::AUTH_BANNED);
        SF_LOG_ERROR("network", "WorldSocket::HandleAuthSession: Sent Auth Response (Account banned).");
        return -1;
    }

    // Check locked state for server
    AccountTypes allowedAccountType = sWorld->GetPlayerSecurityLimit();
    SF_LOG_DEBUG("network", "Allowed Level: %u Player Level %u", uint8(allowedAccountType), security);
    if (allowedAccountType > AccountTypes::SEC_PLAYER && AccountTypes(security) < allowedAccountType)
    {
        SendAuthResponseError(ResponseCodes::AUTH_UNAVAILABLE);
        SF_LOG_INFO("network", "WorldSocket::HandleAuthSession: User tries to login but his security level is not enough");
        return -1;
    }

    // Check that Key and account name are the same on client and server
    uint32 t = 0;
    uint32 seed = m_Seed;

    sha.UpdateData(account);
    sha.UpdateData((uint8*)&t, 4);
    sha.UpdateData((uint8*)&clientSeed, 4);
    sha.UpdateData((uint8*)&seed, 4);
    sha.UpdateBigNumbers(&k, NULL);
    sha.Finalize();

    std::string address = GetRemoteAddress();

    if (memcmp(sha.GetDigest(), digest, 20))
    {
        SendAuthResponseError(ResponseCodes::AUTH_FAILED);
        SF_LOG_ERROR("network", "WorldSocket::HandleAuthSession: Authentication failed for account: %u ('%s') address: %s", id, account.c_str(), address.c_str());
        return -1;
    }

    SF_LOG_DEBUG("network", "WorldSocket::HandleAuthSession: Client '%s' authenticated successfully from %s.",
        account.c_str(),
        address.c_str());

    // Check if this user is by any chance a recruiter
    stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_RECRUITER);

    stmt->setUInt32(0, id);

    result = LoginDatabase.Query(stmt);

    bool isRecruiter = false;
    if (result)
        isRecruiter = true;

    // Update the last_ip in the database

    stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_LAST_IP);

    stmt->setString(0, address);
    stmt->setString(1, account);

    LoginDatabase.Execute(stmt);

    // NOTE ATM the socket is single-threaded, have this in mind ...
    ACE_NEW_RETURN(m_Session, WorldSession(id, this, AccountTypes(security), expansion, mutetime, locale, recruiter, isRecruiter, hasBoost), -1);

    m_Crypt.Init(&k);

    m_Session->LoadGlobalAccountData();
    m_Session->LoadTutorialsData();
    m_Session->ReadAddonsInfo(addonsData);
    m_Session->LoadPermissions();

    // Initialize Warden system only if it is enabled by config
    if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_WARDEN_ENABLED))
        m_Session->InitWarden(&k, os);

    // Sleep this Network thread for
    uint32 sleepTime = sWorld->getIntConfig(WorldIntConfigs::CONFIG_SESSION_ADD_DELAY);
    ACE_OS::sleep(ACE_Time_Value(0, sleepTime));

    sWorld->AddSession(m_Session);
    return 0;
}

int WorldSocket::HandlePing (WorldPacket& recvPacket)
{
    uint32 ping;
    uint32 latency;

    // Get the ping packet content
    recvPacket >> latency;
    recvPacket >> ping;

    if (m_LastPingTime == ACE_Time_Value::zero)
        m_LastPingTime = ACE_OS::gettimeofday(); // for 1st ping
    else
    {
        ACE_Time_Value cur_time = ACE_OS::gettimeofday();
        ACE_Time_Value diff_time (cur_time);
        diff_time -= m_LastPingTime;
        m_LastPingTime = cur_time;

        if (diff_time < ACE_Time_Value (27))
        {
            ++m_OverSpeedPings;

            uint32 max_count = sWorld->getIntConfig (WorldIntConfigs::CONFIG_MAX_OVERSPEED_PINGS);

            if (max_count && m_OverSpeedPings > max_count)
            {
                ACE_GUARD_RETURN (LockType, Guard, m_SessionLock, -1);

                if (m_Session && !m_Session->HasPermission(rbac::RBAC_PERM_SKIP_CHECK_OVERSPEED_PING))
                {
                    SF_LOG_ERROR("network", "WorldSocket::HandlePing: %s kicked for over-speed pings (address: %s)",
                        m_Session->GetPlayerInfo().c_str(), GetRemoteAddress().c_str());

                    return -1;
                }
            }
        }
        else
            m_OverSpeedPings = 0;
    }

    // critical section
    {
        ACE_GUARD_RETURN (LockType, Guard, m_SessionLock, -1);

        if (m_Session)
        {
            m_Session->SetLatency (latency);
            m_Session->ResetClientTimeDelay();
        }
        else
        {
            SF_LOG_ERROR("network", "WorldSocket::HandlePing: peer sent CMSG_PING, "
                            "but is not authenticated or got recently kicked, "
                            " address = %s",
                            GetRemoteAddress().c_str());
             return -1;
        }
    }

    WorldPacket packet(SMSG_PONG, 4);
    packet << ping;
    return SendPacket(packet);
}

void WorldSocket::SendAuthResponseError(ResponseCodes code)
{
        WorldPacket packet(SMSG_AUTH_RESPONSE, 1);
        packet.WriteBit(0); // has account info
        packet.WriteBit(0); // has queue info
        packet << uint8(code);
        SendPacket(packet);
}
