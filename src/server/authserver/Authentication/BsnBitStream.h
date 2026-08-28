/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_BSNBITSTREAM_H
#define SF_BSNBITSTREAM_H

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

    class BitReader
    {
    public:
        BitReader(uint8 const* data, size_t length) : _data(data), _sizeBits(length * 8), _bitPos(0) { }

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

    private:
        uint8 const* _data;
        size_t _sizeBits;
        size_t _bitPos;
    };
}

#endif
