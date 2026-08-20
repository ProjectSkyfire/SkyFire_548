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
    // Real Header messages observed on this protocol family are small
    // (a handful of scalar fields); this is only a sanity bound against a
    // garbage/hostile length prefix, not a protocol limit.
    constexpr uint16 MaxPlausibleHeaderLength = 4096;
}

AuthnetSocket::AuthnetSocket(RealmSocket& socket) :
    socket_(socket), _loggedHeader(false)
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
    SF_LOG_DEBUG("server.authserver", "'%s:%d' authnet passive probe: connection closed",
        socket().getRemoteAddress().c_str(), socket().getRemotePort());
}

void AuthnetSocket::OnRead(void)
{
    // One frame per connection for now: log the header, then close. This
    // is deliberately conservative - the header's `size` field would tell
    // a full parser how many body bytes follow, but this build's exact
    // field-number layout isn't confirmed yet, so guessing at it risks
    // desyncing the stream (or worse, misreading unrelated bytes as a
    // body that happens to contain the password). Logging only the header
    // is safe: its confirmed fields are service_id/service_hash/method_id/
    // token/size/status - never credential-bearing.
    if (_loggedHeader)
        return;

    const size_t headerLengthSize = 2;
    if (socket().GetAvailableBytes() < headerLengthSize)
        return;

    uint16 headerLen = 0;
    socket().PeekBytes(&headerLen, headerLengthSize);

    if (headerLen == 0 || headerLen > MaxPlausibleHeaderLength)
    {
        SF_LOG_ERROR("server.authserver", "'%s:%d' authnet passive probe: implausible header length %u, closing",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), headerLen);
        socket().Close();
        return;
    }

    if (socket().GetAvailableBytes() < headerLengthSize + headerLen)
        return;

    socket().DiscardBytes(headerLengthSize);
    std::vector<uint8> headerBytes(headerLen);
    socket().ReadBytes(headerBytes.data(), headerBytes.size());

    SF_LOG_INFO("server.authserver", "'%s:%d' authnet passive probe: header frame, %u bytes: %s",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(), headerLen,
        ByteArrayToHexStr(headerBytes).c_str());

    _loggedHeader = true;

    SF_LOG_INFO("server.authserver", "'%s:%d' authnet passive probe: closing after first frame (no body/response parsing yet)",
        socket().getRemoteAddress().c_str(), socket().getRemotePort());
    socket().Close();
}
