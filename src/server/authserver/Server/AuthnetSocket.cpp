/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "AuthnetSocket.h"
#include "Authentication/BsnBitStream.h"
#include "Log.h"
#include "Util.h"

#include <algorithm>
#include <cstring>
#include <string>
#include <vector>

namespace
{
    constexpr size_t MaxCapturedBytes = 8192;

    constexpr uint32 InitialLoginCommand = 9;
    constexpr size_t MaxInitialIdentityBytes = 0x140;
    constexpr size_t ClientModeSwitchRequestLen = 2;
    constexpr size_t FirstEncryptedFollowupLen = 57;
    constexpr size_t EncryptedNewsRequestLen = 27;
    constexpr size_t NewsRequestBurstCount = 19;
    char constexpr EmptyHttpResponse[] =
        "HTTP/1.1 204 No Content\r\n"
        "Content-Length: 0\r\n"
        "Connection: close\r\n"
        "\r\n";

    constexpr uint8 InitialClientToServerCryptState[256] =
    {
        0x08, 0x27, 0x53, 0x31, 0x84, 0x32, 0xA8, 0x50, 0x22, 0x95, 0x41, 0xA4, 0x64, 0xB7, 0xA5, 0x46,
        0x6F, 0x28, 0x80, 0xFC, 0x62, 0x16, 0x60, 0x1F, 0xED, 0x12, 0xA7, 0xD6, 0x35, 0xD8, 0x63, 0xF8,
        0xEA, 0xA0, 0x1A, 0xAE, 0xE2, 0xE0, 0x3D, 0x43, 0x2F, 0x20, 0xBE, 0xCA, 0xF6, 0x76, 0x7D, 0x98,
        0xF9, 0x2D, 0x38, 0xDA, 0xA9, 0x6A, 0x07, 0xC7, 0x9C, 0x9F, 0x6B, 0xA3, 0x0D, 0xE9, 0x9D, 0x2C,
        0x7E, 0x5F, 0x04, 0x77, 0x1D, 0xEB, 0x5E, 0xDE, 0x19, 0x8A, 0xC0, 0x52, 0x67, 0xE5, 0xBA, 0xD7,
        0x21, 0xE1, 0x56, 0x59, 0xC3, 0x61, 0x69, 0x71, 0x2E, 0xB3, 0x06, 0x1E, 0x85, 0xB1, 0x93, 0x3C,
        0x5A, 0xAD, 0x86, 0xFA, 0x55, 0xC4, 0xAC, 0x8D, 0xE6, 0x3B, 0x0C, 0x88, 0x0F, 0x74, 0x13, 0x45,
        0xDD, 0xD3, 0xBF, 0x25, 0xD1, 0x40, 0x97, 0xBB, 0x4B, 0x4A, 0x83, 0x34, 0xA6, 0x78, 0x7F, 0xD9,
        0x0E, 0xAA, 0x8F, 0xE7, 0x44, 0x18, 0x51, 0x05, 0x23, 0xF4, 0x81, 0x02, 0x3A, 0x00, 0x39, 0x96,
        0xF2, 0x57, 0x26, 0xC5, 0x15, 0x5C, 0xC9, 0x5B, 0x70, 0x48, 0x0B, 0xCB, 0x17, 0x30, 0x37, 0x75,
        0xE8, 0xBC, 0xF1, 0xE4, 0xCC, 0xC8, 0xCF, 0x9A, 0x47, 0x99, 0xB6, 0x66, 0xC1, 0x5D, 0x36, 0x7C,
        0x73, 0xAB, 0xCD, 0x92, 0x6D, 0x90, 0xB8, 0x65, 0xA1, 0x3E, 0x8C, 0x8E, 0xD2, 0xAF, 0x14, 0x24,
        0xB2, 0xF7, 0x54, 0xDF, 0xCE, 0x68, 0x01, 0xC6, 0x9E, 0x3F, 0x89, 0xFE, 0xDC, 0x0A, 0x03, 0xEF,
        0xDB, 0x33, 0x87, 0xE3, 0x91, 0x4E, 0x8B, 0x1C, 0xEC, 0x7A, 0xB9, 0x7B, 0xD0, 0x79, 0x49, 0xEE,
        0xF0, 0x09, 0x2A, 0xC2, 0xFF, 0x94, 0x4F, 0x4D, 0x1B, 0x42, 0x6E, 0x4C, 0xF5, 0x58, 0xFD, 0xA2,
        0x6C, 0x9B, 0x72, 0xB5, 0xD4, 0xF3, 0xD5, 0x82, 0x11, 0x2B, 0xB4, 0x29, 0xBD, 0xFB, 0x10, 0xB0
    };

    constexpr uint8 InitialServerToClientCryptState[256] =
    {
        0xCB, 0x33, 0xE3, 0x08, 0x1E, 0x3E, 0xA6, 0x1C, 0x97, 0x2A, 0x43, 0xC6, 0x12, 0x10, 0xB2, 0xB8,
        0xC9, 0x84, 0x83, 0xCE, 0xFC, 0x3D, 0x4B, 0x95, 0x49, 0xA2, 0x46, 0xD8, 0x37, 0xB0, 0x05, 0x48,
        0xF9, 0xDD, 0x88, 0x5F, 0x69, 0x2F, 0xCF, 0xE1, 0x6C, 0xA1, 0x29, 0x1B, 0x5D, 0x7B, 0x6D, 0x64,
        0xD6, 0x76, 0xA5, 0x7E, 0xF8, 0x57, 0xAC, 0x7D, 0xE0, 0x00, 0x9F, 0x2E, 0x17, 0xA4, 0x32, 0x56,
        0x80, 0xAF, 0x5E, 0x53, 0x52, 0x96, 0xF7, 0xB3, 0x30, 0xEF, 0x82, 0xDF, 0x42, 0x27, 0x73, 0xC1,
        0xF3, 0xA0, 0x31, 0x2B, 0xBC, 0x60, 0xFE, 0x67, 0x94, 0x13, 0x92, 0x81, 0x3F, 0x3C, 0x6A, 0x15,
        0x25, 0xB1, 0xE7, 0xC3, 0xA3, 0x9B, 0x59, 0xD9, 0x72, 0x14, 0x51, 0x50, 0x39, 0x71, 0xD5, 0xCA,
        0xC2, 0x41, 0xAE, 0x8A, 0x4A, 0x26, 0x91, 0x35, 0x78, 0x0E, 0xAB, 0x11, 0xBE, 0x89, 0x70, 0x06,
        0x20, 0xA9, 0x9D, 0xFB, 0xD3, 0x75, 0x8B, 0x02, 0x6B, 0x16, 0xAA, 0xB7, 0x66, 0x9C, 0x98, 0xDB,
        0xE6, 0xFD, 0x28, 0x5A, 0x65, 0xEA, 0x0D, 0x85, 0xF4, 0x18, 0x09, 0xC5, 0x07, 0xCC, 0x62, 0x2D,
        0xA7, 0xAD, 0x63, 0xC7, 0xB5, 0x90, 0xE5, 0x86, 0xEE, 0xED, 0x6F, 0x4C, 0x5B, 0xE8, 0xDA, 0x87,
        0x4D, 0x55, 0xD7, 0x04, 0xE9, 0xD4, 0x77, 0xF5, 0xF1, 0x1A, 0x54, 0x93, 0x4F, 0x9E, 0x8C, 0x58,
        0x36, 0xFF, 0x24, 0x7F, 0x34, 0x5C, 0x38, 0xBD, 0x47, 0x01, 0x2C, 0x3B, 0xDC, 0x68, 0xC4, 0x8E,
        0xF2, 0xCD, 0x99, 0xFA, 0x0F, 0xBF, 0x74, 0x7C, 0xB9, 0x40, 0xD0, 0xD1, 0xA8, 0x0A, 0x0C, 0x1D,
        0xEC, 0x03, 0xB4, 0xE2, 0xEB, 0x6E, 0xBA, 0x23, 0xC8, 0xD2, 0x8F, 0x8D, 0x9A, 0xBB, 0x21, 0x7A,
        0x45, 0xC0, 0x3A, 0xE4, 0xDE, 0x44, 0x0B, 0x4E, 0x19, 0x22, 0x1F, 0xB6, 0xF0, 0x61, 0xF6, 0x79
    };

    struct ProbePacketHeader
    {
        bool decoded = false;
        uint32 command = 0;
        uint32 modeSwitch = 0;
        uint32 mode = 0;
        size_t bitPosition = 0;
    };

    struct InitialComponent
    {
        std::string program;
        std::string platform;
        uint32 build = 0;
    };

    struct InitialRequestInfo
    {
        ProbePacketHeader header;
        std::string program;
        std::string platform;
        std::string locale;
        std::vector<InitialComponent> components;
        bool hasIdentity = false;
        std::string identity;
        uint32 identityLength = 0;
        uint64 tailValue = 0;
        size_t bitLength = 0;
        size_t packetLength = 0;
    };

    std::vector<uint8> BuildStartupResponseProbe()
    {
        Skyfire::Authnet::BitWriter writer;

        writer.WriteBits(0x00, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x00, 4);

        writer.WriteBits(0, 1);
        writer.WriteBits(0, 3);
        writer.WriteUInt32(0x8000C350);
        writer.WriteBits(0, 1);
        writer.WriteBits(0, 8);
        writer.AlignToByte();
        writer.WriteBits(0, 8);
        writer.AlignToByte();
        writer.WriteUInt32(0);
        writer.WriteBits(0, 8);
        writer.WriteUInt32(0);
        writer.WriteUInt32(0);
        writer.WriteBits(0, 8);
        writer.WriteBits(0, 5);
        uint8 const tailString[] = { 'A' };
        writer.WriteBytes(tailString, sizeof(tailString));
        writer.WriteUInt32(0);
        writer.WriteUInt32(0);
        writer.WriteUInt32(0);
        writer.WriteBits(0, 1);

        return writer.Data();
    }

    std::vector<uint8> BuildLoginAuthStatusProbe(uint32 status)
    {
        Skyfire::Authnet::BitWriter writer;

        writer.WriteBits(0x00, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x02, 4);
        writer.WriteBits(1, 1);
        writer.WriteBits(status, 8);

        return writer.Data();
    }

    std::vector<uint8> BuildEmptyRequestResult(uint32 requestId)
    {
        Skyfire::Authnet::BitWriter writer;

        writer.WriteBits(0x09, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x0B, 4);
        writer.WriteBits(0, 16);
        writer.WriteBits(0, 16);
        writer.WriteUInt32(requestId);
        writer.WriteBits(0, 6);
        writer.AlignToByte();

        return writer.Data();
    }

    void ApplyStreamCipher(uint8 state[256], uint8& i, uint8& j, std::vector<uint8>& payload)
    {
        for (size_t pos = 0; pos < payload.size(); ++pos)
        {
            ++i;
            uint8 si = state[i];
            j = uint8(j + si);
            uint8 sj = state[j];
            state[i] = sj;
            state[j] = si;
            payload[pos] ^= state[uint8(state[i] + state[j])];
        }
    }

    ProbePacketHeader DecodeProbePacketHeader(std::vector<uint8> const& packet)
    {
        ProbePacketHeader header;
        Skyfire::Authnet::BitReader reader(packet.data(), packet.size());

        if (!reader.ReadBits(6, header.command) || !reader.ReadBits(1, header.modeSwitch))
            return header;

        if (header.modeSwitch && !reader.ReadBits(4, header.mode))
            return header;

        header.bitPosition = reader.BitPosition();
        header.decoded = true;
        return header;
    }

    bool TryDecodePostSuccessRequestId(std::vector<uint8> const& packet, uint32& requestId)
    {
        Skyfire::Authnet::BitReader reader(packet.data(), packet.size());

        ProbePacketHeader header;
        if (!reader.ReadBits(6, header.command) || !reader.ReadBits(1, header.modeSwitch))
            return false;

        if (!header.modeSwitch || !reader.ReadBits(4, header.mode))
            return false;

        if (header.command != 9 || header.mode != 11)
            return false;

        uint32 requestCheck = 0;
        if (!reader.ReadBits(31, requestCheck) || !reader.ReadUInt32(requestId))
            return false;

        return true;
    }

    size_t ByteLengthForBits(size_t bitLength)
    {
        return (bitLength + 7) / 8;
    }

    void TrimTrailingNulls(std::string& value)
    {
        while (!value.empty() && value[value.size() - 1] == '\0')
            value.erase(value.size() - 1);
    }

    bool TryDecodeInitialRequest(std::vector<uint8> const& captured, InitialRequestInfo& info)
    {
        using namespace Skyfire::Authnet;

        BitReader reader(captured.data(), captured.size());

        if (!reader.ReadBits(6, info.header.command) || !reader.ReadBits(1, info.header.modeSwitch))
            return false;

        if (info.header.modeSwitch && !reader.ReadBits(4, info.header.mode))
            return false;

        info.header.bitPosition = reader.BitPosition();
        info.header.decoded = true;

        if (info.header.command != InitialLoginCommand || !info.header.modeSwitch)
            return false;

        uint32 componentCount = 0;
        if (!reader.ReadFourCC(info.program) || !reader.ReadFourCC(info.platform) ||
            !reader.ReadFourCC(info.locale) || !reader.ReadBits(6, componentCount))
            return false;

        info.components.clear();
        info.components.reserve(componentCount);

        for (uint32 i = 0; i < componentCount; ++i)
        {
            InitialComponent component;
            if (!reader.ReadFourCC(component.program) || !reader.ReadFourCC(component.platform) ||
                !reader.ReadUInt32(component.build))
                return false;

            info.components.push_back(component);
        }

        uint32 hasIdentity = 0;
        if (!reader.ReadBits(1, hasIdentity))
            return false;

        info.hasIdentity = hasIdentity != 0;
        if (info.hasIdentity)
        {
            uint32 identityLengthMinus3 = 0;
            if (!reader.ReadBits(9, identityLengthMinus3))
                return false;

            info.identityLength = identityLengthMinus3 + 3;
            if (info.identityLength > MaxInitialIdentityBytes)
                return false;

            if (!reader.ReadString(info.identityLength, info.identity))
                return false;

            TrimTrailingNulls(info.identity);
        }

        if (!reader.ReadUInt64(info.tailValue))
            return false;

        info.bitLength = reader.BitPosition();
        info.packetLength = ByteLengthForBits(info.bitLength);
        return true;
    }

    bool CapturedStartsWith(std::vector<uint8> const& bytes, char const* prefix)
    {
        size_t prefixLen = std::strlen(prefix);
        return bytes.size() >= prefixLen && std::memcmp(bytes.data(), prefix, prefixLen) == 0;
    }

    bool LooksLikeHttpRequest(std::vector<uint8> const& bytes)
    {
        return CapturedStartsWith(bytes, "GET ") ||
            CapturedStartsWith(bytes, "HEAD ") ||
            CapturedStartsWith(bytes, "POST ") ||
            CapturedStartsWith(bytes, "OPTIONS ");
    }
}

AuthnetSocket::AuthnetSocket(RealmSocket& socket) :
    socket_(socket), _encryptedBytesProcessed(0), _initialRequestLen(0), _clientCryptI(0), _clientCryptJ(0),
    _clientCryptInitialized(false), _serverCryptI(0), _serverCryptJ(0), _serverCryptInitialized(false),
    _responded(false), _httpResponded(false), _clientModeSwitchSeen(false), _followupLogged(false),
    _postSuccessBurstSeen(false)
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
    SF_LOG_INFO("server.authserver", "'%s:%d' authnet passive probe: connection closed, %zu byte(s) captured total, responded=%s",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(), _captured.size(),
        _responded ? "yes" : "no");
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

    std::vector<uint8> newBytes(_captured.begin() + offset, _captured.end());

    SF_LOG_INFO("server.authserver", "'%s:%d' authnet passive probe: %zu new byte(s), %zu total so far, new=%s",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(), available, _captured.size(),
        ByteArrayToHexStr(newBytes).c_str());

    TrySendProbeResponse(offset, available);
}

bool AuthnetSocket::DecodeInitialRequest(void)
{
    if (_initialRequestLen != 0)
        return true;

    InitialRequestInfo request;
    if (!TryDecodeInitialRequest(_captured, request))
        return false;

    _initialRequestLen = request.packetLength;

    SF_LOG_INFO("server.authserver",
        "'%s:%d' authnet decode: initial_header command=%u mode_switch=%u mode=%u header_bits=%zu program=%s platform=%s locale=%s components=%zu",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(),
        request.header.command, request.header.modeSwitch, request.header.mode, request.header.bitPosition,
        request.program.c_str(), request.platform.c_str(), request.locale.c_str(), request.components.size());

    for (size_t i = 0; i < request.components.size(); ++i)
    {
        SF_LOG_INFO("server.authserver", "'%s:%d' authnet decode: component[%u] = %s.%s.%u",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(),
            uint32(i), request.components[i].program.c_str(), request.components[i].platform.c_str(),
            request.components[i].build);
    }

    SF_LOG_INFO("server.authserver", "'%s:%d' authnet decode: has_identity=%s identity_len=%u identity=%s tail_value=" UI64FMTD " initial_request_len=%zu initial_bits=%zu",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(),
        request.hasIdentity ? "yes" : "no", request.identityLength, request.identity.c_str(),
        request.tailValue, _initialRequestLen, request.bitLength);

    return true;
}

void AuthnetSocket::CryptClientPayload(std::vector<uint8>& payload)
{
    if (!_clientCryptInitialized)
    {
        std::memcpy(_clientCryptState, InitialClientToServerCryptState, sizeof(_clientCryptState));
        _clientCryptI = 0;
        _clientCryptJ = 0;
        _clientCryptInitialized = true;
    }

    ApplyStreamCipher(_clientCryptState, _clientCryptI, _clientCryptJ, payload);
}

void AuthnetSocket::CryptServerPayload(std::vector<uint8>& payload)
{
    if (!_serverCryptInitialized)
    {
        std::memcpy(_serverCryptState, InitialServerToClientCryptState, sizeof(_serverCryptState));
        _serverCryptI = 0;
        _serverCryptJ = 0;
        _serverCryptInitialized = true;
    }

    ApplyStreamCipher(_serverCryptState, _serverCryptI, _serverCryptJ, payload);
}

void AuthnetSocket::SendEncryptedRequestResult(uint32 requestId)
{
    if (std::find(_answeredRequestIds.begin(), _answeredRequestIds.end(), requestId) != _answeredRequestIds.end())
        return;

    _answeredRequestIds.push_back(requestId);

    std::vector<uint8> response = BuildEmptyRequestResult(requestId);
    std::vector<uint8> plainResponse = response;
    CryptServerPayload(response);

    SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: sending encrypted empty request result id=%u plain=%s encrypted=%s",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(), requestId,
        ByteArrayToHexStr(plainResponse).c_str(), ByteArrayToHexStr(response).c_str());

    socket().QueueSend(reinterpret_cast<char const*>(response.data()), response.size());
}

void AuthnetSocket::ProcessEncryptedClientBytes(size_t encryptedFollowupOffset)
{
    if (_captured.size() <= encryptedFollowupOffset + _encryptedBytesProcessed)
        return;

    size_t offset = encryptedFollowupOffset + _encryptedBytesProcessed;
    std::vector<uint8> plain(_captured.begin() + offset, _captured.end());
    _encryptedBytesProcessed += plain.size();

    CryptClientPayload(plain);

    SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: decrypted post-success byte(s) offset=%zu len=%zu plain=%s",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(), offset, plain.size(),
        ByteArrayToHexStr(plain).c_str());

    if (plain.size() % EncryptedNewsRequestLen == 0)
    {
        for (size_t pos = 0; pos < plain.size(); pos += EncryptedNewsRequestLen)
        {
            std::vector<uint8> packet(plain.begin() + pos, plain.begin() + pos + EncryptedNewsRequestLen);
            uint32 requestId = 0;
            if (TryDecodePostSuccessRequestId(packet, requestId))
            {
                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: decoded post-success request id=%u",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(), requestId);
                SendEncryptedRequestResult(requestId);
            }
        }

        return;
    }

    ProbePacketHeader header = DecodeProbePacketHeader(plain);
    if (header.decoded)
    {
        SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: decrypted post-success header command=%u mode_switch=%u mode=%u header_bits=%zu",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(),
            header.command, header.modeSwitch, header.mode, header.bitPosition);
    }
}

void AuthnetSocket::TrySendProbeResponse(size_t readOffset, size_t readSize)
{
    if (_httpResponded)
        return;

    if (LooksLikeHttpRequest(_captured))
    {
        SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: HTTP request received on authnet port, sending empty HTTP response and closing",
            socket().getRemoteAddress().c_str(), socket().getRemotePort());

        socket().QueueSend(EmptyHttpResponse, sizeof(EmptyHttpResponse) - 1, true);
        _httpResponded = true;
        _responded = true;
        return;
    }

    if (!DecodeInitialRequest())
        return;

    if (!_responded)
    {
        std::vector<uint8> response = BuildStartupResponseProbe();

        SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: initial request detected, sending startup response candidate %zu-byte response: %s",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), response.size(),
            ByteArrayToHexStr(response).c_str());

        socket().QueueSend(reinterpret_cast<char const*>(response.data()), response.size());
        _responded = true;
    }

    if (!_clientModeSwitchSeen && _captured.size() >= _initialRequestLen + ClientModeSwitchRequestLen)
    {
        std::vector<uint8> followup(_captured.begin() + _initialRequestLen,
            _captured.begin() + _initialRequestLen + ClientModeSwitchRequestLen);
        ProbePacketHeader header = DecodeProbePacketHeader(followup);

        if (header.decoded)
        {
            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: client mode switch header command=%u mode_switch=%u mode=%u header_bits=%zu",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                header.command, header.modeSwitch, header.mode, header.bitPosition);
        }

        if (header.decoded && header.command == 5 && header.modeSwitch && header.mode == 1)
        {
            _clientModeSwitchSeen = true;

            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: client mode switch detected, waiting for encrypted follow-up",
                socket().getRemoteAddress().c_str(), socket().getRemotePort());
        }
    }

    size_t encryptedFollowupOffset = _initialRequestLen + (_clientModeSwitchSeen ? ClientModeSwitchRequestLen : 0);
    if (_clientModeSwitchSeen)
        ProcessEncryptedClientBytes(encryptedFollowupOffset);

    if (!_followupLogged && _captured.size() >= encryptedFollowupOffset + FirstEncryptedFollowupLen)
    {
        std::vector<uint8> followup(_captured.begin() + encryptedFollowupOffset,
            _captured.begin() + encryptedFollowupOffset + FirstEncryptedFollowupLen);

        SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: first encrypted post-success bytes detected: %s",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), ByteArrayToHexStr(followup).c_str());

        _followupLogged = true;
    }

    size_t newsBurstBytes = EncryptedNewsRequestLen * NewsRequestBurstCount;
    size_t postNewsOffset = encryptedFollowupOffset + newsBurstBytes;

    if (_clientModeSwitchSeen && !_postSuccessBurstSeen)
    {
        size_t encryptedFollowupBytes = _captured.size() > encryptedFollowupOffset ? _captured.size() - encryptedFollowupOffset : 0;
        if (encryptedFollowupBytes >= newsBurstBytes)
        {
            std::vector<uint8> response = BuildLoginAuthStatusProbe(0);
            ProbePacketHeader header = DecodeProbePacketHeader(response);

            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: post-success request burst reached %zu byte(s), holding %zu-byte mode2 status candidate until framing is known: %s, header_ok=%s command=%u mode_switch=%u mode=%u",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(), encryptedFollowupBytes,
                response.size(), ByteArrayToHexStr(response).c_str(), header.decoded ? "yes" : "no",
                header.command, header.modeSwitch, header.mode);

            _postSuccessBurstSeen = true;
        }
    }

    if (_postSuccessBurstSeen && readOffset >= postNewsOffset && readSize != ClientModeSwitchRequestLen)
    {
        SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: post-success request length %zu observed, no status probe sent",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), readSize);
    }
}
