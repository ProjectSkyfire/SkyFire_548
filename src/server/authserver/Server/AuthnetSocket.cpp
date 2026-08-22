/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "AuthnetSocket.h"
#include "Log.h"
#include "Util.h"

#include <algorithm>
#include <vector>

namespace
{
    constexpr size_t MaxCapturedBytes = 8192;

    constexpr uint8 FixedTrailer[] = { 0x40, 0x01, 0x49, 0x01, 0x18, 0x00 };
    constexpr size_t FixedEnvelopePrefixLen = 62;
    constexpr size_t TokenLen = 5;
}

AuthnetSocket::AuthnetSocket(RealmSocket& socket) :
    socket_(socket), _responded(false)
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

    TrySendProbeResponse();
}

void AuthnetSocket::TrySendProbeResponse(void)
{
    if (_responded)
        return;

    if (_captured.size() < FixedEnvelopePrefixLen)
        return;

    auto trailerIt = std::search(_captured.begin(), _captured.end(),
        std::begin(FixedTrailer), std::end(FixedTrailer));
    if (trailerIt == _captured.end())
        return;

    size_t trailerOffset = size_t(trailerIt - _captured.begin());
    size_t tokenOffset = trailerOffset + sizeof(FixedTrailer);
    if (_captured.size() < tokenOffset + TokenLen)
        return;

    std::vector<uint8> response;
    response.reserve(4 + 2 + TokenLen);
    response.push_back(0x00);
    response.push_back(0x00);
    response.push_back(0x00);
    response.push_back(0x00);
    response.push_back(0x00);
    response.push_back(0x00);
    response.insert(response.end(), _captured.begin() + tokenOffset, _captured.begin() + tokenOffset + TokenLen);

    SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: sending experimental %zu-byte response: %s",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(), response.size(),
        ByteArrayToHexStr(response).c_str());

    socket().QueueSend(reinterpret_cast<char const*>(response.data()), response.size(), true);
    _responded = true;
}
