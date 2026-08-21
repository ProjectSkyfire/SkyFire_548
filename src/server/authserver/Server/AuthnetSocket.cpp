/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "AuthnetSocket.h"
#include "Log.h"
#include "Util.h"

#include <vector>

namespace
{
    // Generous safety cap against a runaway/hostile sender - not a protocol
    // limit. A real first message has been observed well under this.
    constexpr size_t MaxCapturedBytes = 8192;
}

AuthnetSocket::AuthnetSocket(RealmSocket& socket) :
    socket_(socket)
{
}

AuthnetSocket::~AuthnetSocket(void) { }

void AuthnetSocket::OnAccept(void)
{
    SF_LOG_INFO("server.authserver", "'%s:%d' authnet passive probe: connection accepted",
        socket().getRemoteAddress().c_str(), socket().getRemotePort());
}

void AuthnetSocket::OnClose(void)
{
    SF_LOG_DEBUG("server.authserver", "'%s:%d' authnet passive probe: connection closed, %zu byte(s) captured total",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(), _captured.size());
}

void AuthnetSocket::OnRead(void)
{
    // Deliberately does not try to interpret any framing (length prefix,
    // header/body boundary) - an earlier version guessed at that and
    // truncated a real message as a result. Instead: drain whatever is
    // available on this read, append it to what's been captured so far
    // across any prior reads on this same connection, and log the
    // cumulative total every time. Never responds, so the client can't
    // progress past this first message into the later, credential-bearing
    // RPC call.
    size_t available = socket().GetAvailableBytes();
    if (available == 0)
        return;

    if (_captured.size() + available > MaxCapturedBytes)
        available = MaxCapturedBytes - _captured.size();

    if (available == 0)
    {
        SF_LOG_ERROR("server.authserver", "'%s:%d' authnet passive probe: hit the %zu byte capture cap, closing",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), MaxCapturedBytes);
        socket().Close();
        return;
    }

    size_t offset = _captured.size();
    _captured.resize(offset + available);
    socket().ReadBytes(_captured.data() + offset, available);

    SF_LOG_INFO("server.authserver", "'%s:%d' authnet passive probe: %zu new byte(s), %zu total so far: %s",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(), available, _captured.size(),
        ByteArrayToHexStr(_captured).c_str());
}
