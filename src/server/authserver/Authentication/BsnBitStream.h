/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_BSNBITSTREAM_H
#define SF_BSNBITSTREAM_H

#include "AuthnetCodes.h"
#include "Define.h"

#include <algorithm>
#include <cstring>
#include <string>
#include <vector>

// Bit packing used by the authnet RPC transport. Values are consumed
// most-significant bits first, but each chunk is deposited starting at
// the least-significant end of the target byte. Neither plain MSB-first
// nor plain LSB-first reproduces it - see the authnet client login probe
// research note, where this was recovered by hooking the client's own
// write primitive and verified against captured traffic.
namespace Skyfire::Authnet
{
    class BitWriter
    {
    public:
        BitWriter() : _bitPos(0) { }

        void WriteBits(uint32 value, uint32 bitCount)
        {
            uint32 remaining = bitCount;
            while (remaining > 0)
            {
                uint32 offset = _bitPos & 7;
                uint32 chunk = std::min<uint32>(8 - offset, remaining);
                remaining -= chunk;

                size_t index = _bitPos >> 3;
                if (index >= _buffer.size())
                    _buffer.resize(index + 1, 0);

                uint8 mask = uint8((1u << chunk) - 1);
                uint8 piece = uint8((value >> remaining) & mask);
                _buffer[index] = uint8((_buffer[index] & ~(mask << offset)) | (piece << offset));
                _bitPos += chunk;
            }
        }

        void WriteUInt32(uint32 value) { WriteBits(value, 32); }

        void WriteFourCC(char const* text)
        {
            uint32 value = 0;
            for (size_t i = 0; i < 4; ++i)
                value = (value << 8) | uint8(text[i] != '\0' ? text[i] : '\0');
            WriteUInt32(value);
        }

        void AlignToByte()
        {
            if (_bitPos & 7)
                _bitPos = (_bitPos + 7) & ~size_t(7);
            size_t needed = (_bitPos + 7) >> 3;
            if (needed > _buffer.size())
                _buffer.resize(needed, 0);
        }

        void WriteBytes(void const* data, size_t length)
        {
            AlignToByte();
            size_t index = _bitPos >> 3;
            if (index + length > _buffer.size())
                _buffer.resize(index + length, 0);
            std::memcpy(_buffer.data() + index, data, length);
            _bitPos += length * 8;
        }

        std::vector<uint8> const& Data() const { return _buffer; }
        size_t BitPosition() const { return _bitPos; }

    private:
        std::vector<uint8> _buffer;
        size_t _bitPos;
    };

    inline size_t ByteLengthForBits(size_t bitLength)
    {
        return (bitLength + 7) / 8;
    }

    class BitReader
    {
    public:
        BitReader(uint8 const* data, size_t length) : _data(data), _sizeBits(length * 8), _bitPos(0) { }
        BitReader(uint8 const* data, size_t length, size_t startBitPos) :
            _data(data), _sizeBits(length * 8), _bitPos(startBitPos) { }

        bool ReadBits(uint32 bitCount, uint32& out)
        {
            if (_bitPos + bitCount > _sizeBits)
                return false;

            uint32 remaining = bitCount;
            uint32 value = 0;
            while (remaining > 0)
            {
                uint32 offset = _bitPos & 7;
                uint32 chunk = std::min<uint32>(8 - offset, remaining);
                remaining -= chunk;

                uint8 mask = uint8((1u << chunk) - 1);
                value |= uint32((_data[_bitPos >> 3] >> offset) & mask) << remaining;
                _bitPos += chunk;
            }

            out = value;
            return true;
        }

        bool ReadUInt32(uint32& out) { return ReadBits(32, out); }

        bool ReadUInt64(uint64& out)
        {
            uint32 high = 0;
            uint32 low = 0;
            if (!ReadUInt32(high) || !ReadUInt32(low))
                return false;

            out = (uint64(high) << 32) | uint64(low);
            return true;
        }

        bool ReadFourCC(std::string& out)
        {
            uint32 value = 0;
            if (!ReadUInt32(value))
                return false;

            out.clear();
            for (int shift = 24; shift >= 0; shift -= 8)
            {
                char c = char((value >> shift) & 0xFF);
                if (c != '\0')
                    out.push_back(c);
            }
            return true;
        }

        bool ReadBytes(void* out, size_t length)
        {
            AlignToByte();
            if (length > RemainingBits() / 8)
                return false;

            std::memcpy(out, _data + (_bitPos >> 3), length);
            _bitPos += length * 8;
            return true;
        }

        bool ReadString(size_t length, std::string& out)
        {
            out.clear();
            out.resize(length);

            if (length == 0)
                return true;

            if (!ReadBytes(&out[0], length))
            {
                out.clear();
                return false;
            }

            return true;
        }

        void AlignToByte()
        {
            if (_bitPos & 7)
                _bitPos = (_bitPos + 7) & ~size_t(7);
        }

        size_t BitPosition() const { return _bitPos; }
        size_t RemainingBits() const { return _bitPos >= _sizeBits ? 0 : _sizeBits - _bitPos; }
        void SetBitPosition(size_t bitPos) { _bitPos = bitPos; }

    private:
        uint8 const* _data;
        size_t _sizeBits;
        size_t _bitPos;
    };

    struct PacketHeader
    {
        bool decoded = false;
        uint32 packetId = 0;
        uint32 inChannel = 0;
        uint32 channelId = 0;
        size_t bitPosition = 0;
    };

    inline bool ReadPacketHeader(BitReader& reader, PacketHeader& header)
    {
        header = PacketHeader();
        if (!reader.ReadBits(AUTHNET_PACKET_ID_BITS, header.packetId) ||
            !reader.ReadBits(AUTHNET_IN_CHANNEL_BITS, header.inChannel))
            return false;

        if (header.inChannel && !reader.ReadBits(AUTHNET_CHANNEL_ID_BITS, header.channelId))
            return false;

        header.bitPosition = reader.BitPosition();
        header.decoded = true;
        return true;
    }

    inline void WritePacketHeader(BitWriter& writer, uint32 packetId, uint32 inChannel, uint32 channelId = 0)
    {
        writer.WriteBits(packetId, AUTHNET_PACKET_ID_BITS);
        writer.WriteBits(inChannel ? 1 : 0, AUTHNET_IN_CHANNEL_BITS);
        if (inChannel)
            writer.WriteBits(channelId, AUTHNET_CHANNEL_ID_BITS);
    }

    struct EventHeader
    {
        uint32 eventId = 0;
        uint32 optionalPresent = 0;
        uint32 optionalVariant = 0;
        uint32 requestToken = 0;
    };

    inline bool ReadEventHeader(BitReader& reader, EventHeader& header)
    {
        header = EventHeader();
        return reader.ReadBits(AUTHNET_EVENT_ID_BITS, header.eventId) &&
            reader.ReadBits(AUTHNET_EVENT_OPTIONAL_BITS, header.optionalPresent);
    }

    // Type 4008 Header with m_sourceAddress optional absent.
    // Destination::Class stays -1; B236E0 will not call A7E890.
    // C->S logon does not use this: FourCC WoW starts at bit 11.
    inline void WriteEventHeader(BitWriter& writer, uint32 eventId)
    {
        writer.WriteBits(eventId, AUTHNET_EVENT_ID_BITS);
        writer.WriteBits(0, AUTHNET_EVENT_OPTIONAL_BITS);
    }

    // 164556/165412: FourCC, Destination, and presence bits after
    // 4920 were copied as body. 173145 sent optional-present plus
    // 32-bit Address 0 then Failure (49A000000000000200C900). Copied
    // 11 bytes (token 0x120B), no crash, no failure log. Kind-11
    // optional is consumed; it does not set Destination::Class.
    // 174840 no-Header Failure is event 0; do not send it again.
    inline void WriteEventHeaderSourceAddress(BitWriter& writer, uint32 eventId, uint32 address)
    {
        writer.WriteBits(eventId, AUTHNET_EVENT_ID_BITS);
        writer.WriteBits(1, AUTHNET_EVENT_OPTIONAL_BITS);
        writer.WriteUInt32(address);
    }

    // Type 4008: m_id, optional Address, sourceName FourCC, dest.
    // Type 4005 dest is 1-bit (0 Class, 1 Address). AF5880 binder+24
    // sets Header+16 Class and returns &Header+20 for the nested
    // FourCC/Address. Soft B2ACE0 reads the 1-bit integer; Primitive
    // B22170 wants stream+136 choice names then AEDCA0→AF5880.
    // 192222 Class0+WoW + 8-bit Failure hung (0x120F). 203715 same
    // Header AlignToByte + Failure hung (0x1210) without failure log —
    // retesting under IDA: Class=0 and A7E890 run; unaligned body was
    // Service(1). Align Soft Failure is the controlled body retest.
    // m_sourceName is a kind-8 FourCC, the same type as the m_destination alt 0
    // that B1B310 is known to crash on when it is 0. It defaulted to 0 on every
    // event we ever sent, which is a zero sender identity - a good reason for
    // the client to treat the event as unroutable and bounce it. Pass a real
    // FourCC to test that; the default stays 0 so older probes are unchanged.
    inline void WriteCompleteEventHeader(BitWriter& writer, uint32 eventId, uint32 destClass, uint32 destValue,
        uint32 sourceName = 0)
    {
        writer.WriteBits(eventId, AUTHNET_EVENT_ID_BITS);
        writer.WriteBits(0, AUTHNET_EVENT_OPTIONAL_BITS);
        writer.WriteUInt32(sourceName);
        writer.WriteBits(destClass, AUTHNET_DESTINATION_CLASS_BITS);
        writer.WriteUInt32(destValue);
    }

    // Header event with optional present, Destination::Class 1 (4 bits),
    // and the 16-bit Address. Matches B236E0 disc==1. Do not use this for
    // the initial 4920 probe. Bare optional-present tokens (49A0130122,
    // 49A0120211, 49A00000120211) all opened a new invoke.
    inline void WriteEventHeaderRequestToken(BitWriter& writer, uint32 eventId, uint32 token)
    {
        writer.WriteBits(eventId, AUTHNET_EVENT_ID_BITS);
        writer.WriteBits(1, AUTHNET_EVENT_OPTIONAL_BITS);
        writer.WriteBits(AUTHNET_EVENT_CLASS_CORRELATED, AUTHNET_EVENT_CLASS_BITS);
        writer.WriteBits(token, AUTHNET_REQUEST_TOKEN_BITS);
    }

    inline bool ReadRequestToken(BitReader& reader, uint32& token)
    {
        return reader.ReadBits(AUTHNET_REQUEST_TOKEN_BITS, token);
    }

    inline void WriteRequestToken(BitWriter& writer, uint32 token)
    {
        writer.WriteBits(token, AUTHNET_REQUEST_TOKEN_BITS);
    }

    // Packet 9 channel 1, Header event 0, 16-bit token. Same layout as
    // the captured C->S request correlator (49011202). Do not send this
    // back as an ack: the 112304 playtest treated it as a new request.
    inline void WriteRequestTokenPacket(BitWriter& writer, uint32 token)
    {
        WritePacketHeader(writer, AuthnetWire::PacketLogin, 1, AuthnetWire::ChannelRequestToken);
        WriteEventHeader(writer, AuthnetCreepEvent::Patch);
        WriteRequestToken(writer, token);
    }

    // Packet 0 channel 1, Header event 0. Matches the captured C->S
    // keepalive 4001.
    inline void WriteStartupPingPacket(BitWriter& writer)
    {
        WritePacketHeader(writer, AuthnetWire::PacketStartup, 1, AuthnetWire::ChannelRequestToken);
        WriteEventHeader(writer, AuthnetCreepEvent::Patch);
    }

    struct ChoiceTag
    {
        uint32 present = 0;
        uint32 tag = 0;
    };

    // Type 1621/2293 (flag=0 mapped choice on C->S): 1-bit present plus
    // 4-bit tag. C->S tag 9 is SetLaunchOptionRequest.
    inline bool ReadChoiceTag(BitReader& reader, ChoiceTag& choice)
    {
        choice = ChoiceTag();
        if (!reader.ReadBits(AUTHNET_CHOICE_PRESENT_BITS, choice.present))
            return false;
        if (!choice.present)
            return true;
        return reader.ReadBits(AUTHNET_CHOICE_TAG_BITS, choice.tag);
    }

    inline void WriteChoiceTag(BitWriter& writer, uint32 tag)
    {
        writer.WriteBits(1, AUTHNET_CHOICE_PRESENT_BITS);
        writer.WriteBits(tag, AUTHNET_CHOICE_TAG_BITS);
    }

    // GenericResult (type 1229): 1-bit tag, success=0 with empty body.
    inline void WriteGenericResultSuccess(BitWriter& writer)
    {
        writer.WriteBits(AuthnetGenericResult::Success, AUTHNET_GENERIC_RESULT_BITS);
    }

    inline bool ReadInvokeTag(BitReader& reader, uint32& tag)
    {
        return reader.ReadBits(AUTHNET_INVOKE_CHOICE_BITS, tag);
    }

    inline void WriteLarvaAuthentication(BitWriter& writer, uint32 tag)
    {
        writer.WriteBits(AuthnetLarvaInvoke::Authentication, AUTHNET_INVOKE_CHOICE_BITS);
        writer.WriteBits(tag, AUTHNET_INVOKE_CHOICE_BITS);
    }

    inline void WriteBsnCountedBytes(BitWriter& writer, char const* text, uint32 lengthBits)
    {
        size_t length = 0;
        while (text[length] != '\0')
            ++length;
        writer.WriteBits(uint32(length), lengthBits);
        writer.WriteBytes(text, length);
    }

    // Same length prefix as the C->S identity string (9-bit length
    // minus 3, then AlignToByte and the characters).
    inline void WriteIdentityCountedString(BitWriter& writer, char const* text)
    {
        size_t length = 0;
        while (text[length] != '\0')
            ++length;
        writer.WriteBits(uint32(length - AUTHNET_IDENTITY_LENGTH_BIAS), AUTHNET_IDENTITY_LENGTH_BITS);
        writer.WriteBytes(text, length);
    }

    // 154819 sent counted variant names Authentication/Module
    // (49200E41...); 0x0E is integer tag 14, not a name length.
    // 160034 sent counted field names m_function/m_parameters; Walker
    // kind-14 already has those names from the type DB. Sequential
    // Module is 8-bit tags 0,0, aligned 6-bit RequestPassword, 32-bit
    // 0, 3-bit 0 (152406).
    inline void WriteModule(BitWriter& writer)
    {
        WriteLarvaAuthentication(writer, AuthnetLarvaAuth::Module);
        WriteBsnCountedBytes(writer, "RequestPassword", AUTHNET_MODULE_FUNCTION_BYTES_BITS);
        writer.WriteBits(0, AUTHNET_MODULE_PARAMETERS_BITS);
        writer.WriteBits(0, AUTHNET_MODULE_EXTRA_BITS);
    }

    // Type 2313 Failure: 16-bit code then empty type-1317 array.
    // 161118 sent this after 8-bit tags 0,2; A78ED0 never logged the
    // code. Do not send 103 (LOGIN_BAD_SERVER_PROOF) or 138 (ignored).
    inline void WriteFailure(BitWriter& writer, uint32 code)
    {
        WriteLarvaAuthentication(writer, AuthnetLarvaAuth::Failure);
        writer.WriteBits(code, AUTHNET_FAILURE_CODE_BITS);
        writer.WriteBits(0, AUTHNET_MODULE_EXTRA_BITS);
    }

    inline void WriteCStringBytes(BitWriter& writer, char const* text)
    {
        size_t length = 0;
        while (text[length] != '\0')
            ++length;
        writer.WriteBytes(text, length + 1);
    }

    // 195827: identity-style Class after 4920 (token 0x1216). Exhausted.
    // AED820 (+136) maps binder integer -> schema name; it is not a
    // wire string reader. Keep this helper only for the unit test.
    inline void WriteEventHeaderDestClass(BitWriter& writer, uint32 eventId)
    {
        WriteEventHeader(writer, eventId);
        writer.WriteUInt32(0);
        WriteIdentityCountedString(writer, "Class");
        writer.WriteUInt32(AUTHNET_SUBSCRIBER_FOURCC_WOW);
    }

    // Kind-6 receive: B22170 needs stream+136 choice names. Type 4005
    // Destination is kind 0x86 (integer, 1-bit). Type 2292 Invoke is
    // kind 0x06 (named, bounds.bits=8). C-string Authentication/Failure
    // after Class0+WoW hung (204335, token 0x3205). Keep helper for tests.
    inline void WriteNamedFailure(BitWriter& writer, uint32 code)
    {
        WriteCStringBytes(writer, "Authentication");
        WriteCStringBytes(writer, "Failure");
        writer.WriteBits(code, AUTHNET_FAILURE_CODE_BITS);
        writer.WriteBits(0, AUTHNET_MODULE_EXTRA_BITS);
    }

    // Same length prefix as C->S identity / 195827 dest Class.
    inline void WriteIdentityNamedFailure(BitWriter& writer, uint32 code)
    {
        WriteIdentityCountedString(writer, "Authentication");
        WriteIdentityCountedString(writer, "Failure");
        writer.WriteBits(code, AUTHNET_FAILURE_CODE_BITS);
        writer.WriteBits(0, AUTHNET_MODULE_EXTRA_BITS);
    }

    // Align + uint8 length + chars (no NUL). Distinct from C-string
    // (204335) and identity 9-bit-3 (205436) after Soft Class0+WoW.
    inline void WriteLengthPrefixedNamedFailure(BitWriter& writer, uint32 code)
    {
        writer.AlignToByte();
        auto writeName = [&writer](char const* text)
        {
            size_t length = 0;
            while (text[length] != '\0')
                ++length;
            writer.WriteBits(uint32(length), 8);
            writer.WriteBytes(text, length);
        };
        writeName("Authentication");
        writeName("Failure");
        writer.WriteBits(code, AUTHNET_FAILURE_CODE_BITS);
        writer.WriteBits(0, AUTHNET_MODULE_EXTRA_BITS);
    }

    // VersionedDecoder (BC92D0 / C38EE0): byte-oriented signed varint.
    // Low bit = sign, next 6 bits = payload, high bit = continue.
    inline void WriteVersionedVarint(BitWriter& writer, int64 value)
    {
        writer.AlignToByte();
        uint64 magnitude = value < 0 ? uint64(-value) : uint64(value);
        uint8 first = uint8((magnitude & 0x3F) << 1);
        if (value < 0)
            first |= 1;
        magnitude >>= 6;
        while (magnitude != 0)
        {
            writer.WriteBits(first | 0x80, 8);
            first = uint8(magnitude & 0x7F);
            magnitude >>= 7;
        }
        writer.WriteBits(first, 8);
    }

    // VersionedDecoder kind-6 (BCA9B0 / C39360): marker byte 3, then
    // signed varint choice. CreepTendril uses Hard::Encoder (bit Soft
    // path) for tendril state, but Versioned is the other BSN family
    // that reads kind-6 as integers without stream+136 names. 205436
    // identity names after Class0+WoW exhausted. 212019 Soft Header +
    // Versioned Failure exhausted (token 0x1212).
    inline void WriteVersionedChoice(BitWriter& writer, int64 value)
    {
        writer.AlignToByte();
        writer.WriteBits(3, 8);
        WriteVersionedVarint(writer, value);
    }

    // Versioned kind-9 (C39620): marker 9 + signed varint.
    inline void WriteVersionedInteger(BitWriter& writer, int64 value)
    {
        writer.AlignToByte();
        writer.WriteBits(9, 8);
        WriteVersionedVarint(writer, value);
    }

    // Versioned kind-11 absent (C396F0): marker 4 + present byte 0.
    inline void WriteVersionedOptionalAbsent(BitWriter& writer)
    {
        writer.AlignToByte();
        writer.WriteBits(4, 8);
        writer.WriteBits(0, 8);
    }

    // Versioned kind-8 FourCC (C39580 / C39030): marker 7 + BE u32.
    inline void WriteVersionedFourCC(BitWriter& writer, uint32 fourcc)
    {
        writer.AlignToByte();
        writer.WriteBits(7, 8);
        writer.WriteBits((fourcc >> 24) & 0xFF, 8);
        writer.WriteBits((fourcc >> 16) & 0xFF, 8);
        writer.WriteBits((fourcc >> 8) & 0xFF, 8);
        writer.WriteBits(fourcc & 0xFF, 8);
    }

    // Versioned kind-14 Header (C39930): marker 5, varint field count,
    // then per-field index varint + typed payload. Type 4008 fields:
    // m_id, optional sourceAddress, sourceName FourCC, destination.
    inline void WriteVersionedEventHeader(BitWriter& writer, uint32 eventId, uint32 destFourCC)
    {
        writer.AlignToByte();
        writer.WriteBits(5, 8);
        WriteVersionedVarint(writer, 4);

        WriteVersionedVarint(writer, 0);
        WriteVersionedInteger(writer, eventId);

        WriteVersionedVarint(writer, 1);
        WriteVersionedOptionalAbsent(writer);

        WriteVersionedVarint(writer, 2);
        WriteVersionedFourCC(writer, 0);

        WriteVersionedVarint(writer, 3);
        WriteVersionedChoice(writer, AUTHNET_DESTINATION_CLASS_NONE);
        WriteVersionedFourCC(writer, destFourCC);
    }

    // Versioned Authentication(0) + Failure(2), then Soft Failure code.
    inline void WriteVersionedFailure(BitWriter& writer, uint32 code)
    {
        WriteVersionedChoice(writer, AuthnetLarvaInvoke::Authentication);
        WriteVersionedChoice(writer, AuthnetLarvaAuth::Failure);
        writer.WriteBits(code, AUTHNET_FAILURE_CODE_BITS);
        writer.WriteBits(0, AUTHNET_MODULE_EXTRA_BITS);
    }

    // Type 2293 ModuleBegin: empty struct2 after 2292 Authentication.
    inline void WriteModuleBegin(BitWriter& writer)
    {
        WriteLarvaAuthentication(writer, AuthnetLarvaAuth::ModuleBegin);
    }

    // Type 2293 SetLaunchOptionResponse (wire tag 10). 142448's 1+4
    // 492013 was copied into the next request. This is the side-RPC
    // response, not the first-invoke body.
    inline void WriteSetLaunchOptionResponse(BitWriter& writer)
    {
        WriteLarvaAuthentication(writer, AuthnetLarvaAuth::SetLaunchOptionResponse);
        WriteGenericResultSuccess(writer);
    }

    // C->S packet 9 channel 0 after Header event 4. The client copies
    // the last S->C packet 9 channel 0 (token low byte = that length)
    // in front of SetLaunchOptionRequest.
    inline bool ReadCreepFollowUpChoice(BitReader& reader, ChoiceTag& choice, uint32 copiedPacketBytes = 0)
    {
        if (copiedPacketBytes > 2)
        {
            size_t extraBits = (copiedPacketBytes - 2) * 8;
            if (reader.RemainingBits() < extraBits)
                return false;
            reader.SetBitPosition(reader.BitPosition() + extraBits);
            return ReadChoiceTag(reader, choice);
        }

        size_t afterHeader = reader.BitPosition();
        if (!ReadChoiceTag(reader, choice))
            return false;
        if (choice.present)
            return true;

        reader.SetBitPosition(afterHeader);
        uint32 invokeTag = 0;
        uint32 authTag = 0;
        if (!ReadInvokeTag(reader, invokeTag) || !ReadInvokeTag(reader, authTag))
        {
            reader.SetBitPosition(afterHeader);
            return ReadChoiceTag(reader, choice);
        }
        return ReadChoiceTag(reader, choice);
    }

    // Packet 9 channel 1, Header event 0, 16-bit token, then the Larva
    // body. Event 4 on this result (channel 0 or channel 1) is a new
    // invoke; 134011 sent 4921120211 and the client opened 0x1207.
    inline void WriteChannel1AuthResult(BitWriter& writer, uint32 token, uint32 tag)
    {
        WriteRequestTokenPacket(writer, token);
        WriteChoiceTag(writer, tag);
    }
}

#endif
