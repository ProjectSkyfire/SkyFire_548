/*
 * Copyright (C) 2011-2017 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2017 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2017 MaNGOS <https://www.getmangos.eu/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _BYTEBUFFER_H
#define _BYTEBUFFER_H

#ifndef SF_TIMEZONE
#if _MSC_VER >= 1900
#define SF_TIMEZONE _timezone
#else
#define SF_TIMEZONE timezone
#endif
#endif

#include "Define.h"
#include "Errors.h"
#include "ByteConverter.h"

#include <ace/OS_NS_time.h>
#include <exception>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <cstring>
#include <time.h>

// Root of ByteBuffer exception hierarchy
class ByteBufferException : public std::exception
{
public:
    ~ByteBufferException() throw() { }

    char const* what() const throw() { return msg_.c_str(); }

protected:
    std::string & message() throw() { return msg_; }

private:
    std::string msg_;
};

class ByteBufferPositionException : public ByteBufferException
{
public:
    ByteBufferPositionException(bool add, size_t pos, size_t size, size_t valueSize);

    ~ByteBufferPositionException() throw() { }
};

class ByteBufferSourceException : public ByteBufferException
{
public:
    ByteBufferSourceException(size_t pos, size_t size, size_t valueSize);

    ~ByteBufferSourceException() throw() { }
};

//! Structure to ease conversions from single 64 bit integer guid into individual bytes, for packet sending purposes
//! Nuke this out when porting ObjectGuid from MaNGOS, but preserve the per-byte storage
struct ObjectGuid
{
public:
    ObjectGuid() { _data.u64 = UI64LIT(0); }
    ObjectGuid(uint64 guid) { _data.u64 = guid; }
    ObjectGuid(ObjectGuid const& other) { _data.u64 = other._data.u64; }

    uint8& operator[](uint32 index)
    {
        ASSERT(index < sizeof(uint64));

#if SKYFIRE_ENDIAN == SKYFIRE_LITTLEENDIAN
        return _data.byte[index];
#else
        return _data.byte[7 - index];
#endif
    }

    uint8 const& operator[](uint32 index) const
    {
        ASSERT(index < sizeof(uint64));

#if SKYFIRE_ENDIAN == SKYFIRE_LITTLEENDIAN
        return _data.byte[index];
#else
        return _data.byte[7 - index];
#endif
    }

    operator uint64()
    {
        return _data.u64;
    }

    ObjectGuid& operator=(uint64 guid)
    {
        _data.u64 = guid;
        return *this;
    }

    ObjectGuid& operator=(ObjectGuid const& other)
    {
        _data.u64 = other._data.u64;
        return *this;
    }

private:
    union
    {
        uint64 u64;
        uint8 byte[8];
    } _data;

};
class ByteBuffer
{
public:
    const static size_t DEFAULT_SIZE = 0x1000;

    // constructor
    ByteBuffer() : _rpos(0), _wpos(0), _bitpos(8), _curbitval(0)
    {
        _storage.reserve(DEFAULT_SIZE);
    }

    ByteBuffer(size_t reserve) : _rpos(0), _wpos(0), _bitpos(8), _curbitval(0)
    {
        _storage.reserve(reserve);
    }

    // copy constructor
    ByteBuffer(const ByteBuffer &buf) : _rpos(buf._rpos), _wpos(buf._wpos),
        _bitpos(buf._bitpos), _curbitval(buf._curbitval), _storage(buf._storage)
    {
    }

    void clear()
    {
        _storage.clear();
        _rpos = _wpos = 0;
    }

    template <typename T> void append(T value)
    {
        FlushBits();
        EndianConvert(value);
        append((uint8 *)&value, sizeof(value));
    }

    void FlushBits()
    {
        if (_bitpos == 8)
            return;

        append((uint8 *)&_curbitval, sizeof(uint8));
        _curbitval = 0;
        _bitpos = 8;
    }

    void WriteBitInOrder(ObjectGuid guid, uint8 order[8])
    {
        for (uint8 i = 0; i < 8; ++i)
            WriteBit(guid[order[i]]);
    }

    bool WriteBit(uint32 bit)
    {
        --_bitpos;
        if (bit)
            _curbitval |= (1 << (_bitpos));

        if (_bitpos == 0)
        {
            _bitpos = 8;
            append((uint8 *)&_curbitval, sizeof(_curbitval));
            _curbitval = 0;
        }

        return (bit != 0);
    }

    void ReadBitInOrder(ObjectGuid& guid, uint8 order[8])
    {
        for (uint8 i = 0; i < 8; ++i)
            guid[order[i]] = ReadBit();
    }

    bool ReadBit()
    {
        ++_bitpos;
        if (_bitpos > 7)
        {
            _bitpos = 0;
            _curbitval = read<uint8>();
        }

        return ((_curbitval >> (7 - _bitpos)) & 1) != 0;
    }

    template <typename T> void WriteBits(T value, size_t bits)
    {
        for (int32 i = bits - 1; i >= 0; --i)
            WriteBit((value >> i) & 1);
    }

    uint32 ReadBits(size_t bits)
    {
        uint32 value = 0;
        for (int32 i = bits - 1; i >= 0; --i)
            if (ReadBit())
                value |= (1 << (i));

        return value;
    }

    void ReadBytesSeq(ObjectGuid& guid, uint8 order[8])
    {
        for (uint8 i = 0; i < 8; ++i)
            ReadByteSeq(guid[order[i]]);
    }

    // Reads a byte (if needed) in-place
    void ReadByteSeq(uint8& b)
    {
        if (b != 0)
            b ^= read<uint8>();
    }

    void WriteBytesSeq(ObjectGuid guid, uint8 order[8])
    {
        for (uint8 i = 0; i < 8; ++i)
            WriteByteSeq(guid[order[i]]);
    }

    void WriteByteSeq(uint8 b)
    {
        if (b != 0)
            append<uint8>(b ^ 1);
    }

    template <typename T> void put(size_t pos, T value)
    {
        EndianConvert(value);
        put(pos, (uint8 *)&value, sizeof(value));
    }

    void ReadGuidMaskList(ObjectGuid& guid, int count, ...)
    {
        va_list ap;
        va_start(ap, count);
        for (uint8 i = 0; i<count; ++i)
        {
            uint8 offset = va_arg(ap, uint32);
            guid[offset] = ReadBit();
        }
        va_end(ap);
    }
    void ReadGuidMask(ObjectGuid& guid, uint8 v1)                                                                           { ReadGuidMaskList(guid, 1, v1); }
    void ReadGuidMask(ObjectGuid& guid, uint8 v1, uint8 v2)                                                                 { ReadGuidMaskList(guid, 2, v1, v2); }
    void ReadGuidMask(ObjectGuid& guid, uint8 v1, uint8 v2, uint8 v3)                                                       { ReadGuidMaskList(guid, 3, v1, v2, v3); }
    void ReadGuidMask(ObjectGuid& guid, uint8 v1, uint8 v2, uint8 v3, uint8 v4)                                             { ReadGuidMaskList(guid, 4, v1, v2, v3, v4); }
    void ReadGuidMask(ObjectGuid& guid, uint8 v1, uint8 v2, uint8 v3, uint8 v4, uint8 v5)                                   { ReadGuidMaskList(guid, 5, v1, v2, v3, v4, v5); }
    void ReadGuidMask(ObjectGuid& guid, uint8 v1, uint8 v2, uint8 v3, uint8 v4, uint8 v5, uint8 v6)                         { ReadGuidMaskList(guid, 6, v1, v2, v3, v4, v5, v6); }
    void ReadGuidMask(ObjectGuid& guid, uint8 v1, uint8 v2, uint8 v3, uint8 v4, uint8 v5, uint8 v6, uint8 v7)               { ReadGuidMaskList(guid, 7, v1, v2, v3, v4, v5, v6, v7); }
    void ReadGuidMask(ObjectGuid& guid, uint8 v1, uint8 v2, uint8 v3, uint8 v4, uint8 v5, uint8 v6, uint8 v7, uint8 v8)     { ReadGuidMaskList(guid, 8, v1, v2, v3, v4, v5, v6, v7, v8); }

    void WriteGuidMaskList(const ObjectGuid& guid, int count, ...)
    {
        va_list ap;
        va_start(ap, count);
        for (uint8 i = 0; i<count; ++i)
        {
            uint8 offset = va_arg(ap, uint32);
            WriteBit(guid[offset]);
        }
        va_end(ap);
    }

    void WriteGuidMask(const ObjectGuid& guid, uint8 v1)                                                                           { WriteGuidMaskList(guid, 1, v1); }
    void WriteGuidMask(const ObjectGuid& guid, uint8 v1, uint8 v2)                                                                 { WriteGuidMaskList(guid, 2, v1, v2); }
    void WriteGuidMask(const ObjectGuid& guid, uint8 v1, uint8 v2, uint8 v3)                                                       { WriteGuidMaskList(guid, 3, v1, v2, v3); }
    void WriteGuidMask(const ObjectGuid& guid, uint8 v1, uint8 v2, uint8 v3, uint8 v4)                                             { WriteGuidMaskList(guid, 4, v1, v2, v3, v4); }
    void WriteGuidMask(const ObjectGuid& guid, uint8 v1, uint8 v2, uint8 v3, uint8 v4, uint8 v5)                                   { WriteGuidMaskList(guid, 5, v1, v2, v3, v4, v5); }
    void WriteGuidMask(const ObjectGuid& guid, uint8 v1, uint8 v2, uint8 v3, uint8 v4, uint8 v5, uint8 v6)                         { WriteGuidMaskList(guid, 6, v1, v2, v3, v4, v5, v6); }
    void WriteGuidMask(const ObjectGuid& guid, uint8 v1, uint8 v2, uint8 v3, uint8 v4, uint8 v5, uint8 v6, uint8 v7)               { WriteGuidMaskList(guid, 7, v1, v2, v3, v4, v5, v6, v7); }
    void WriteGuidMask(const ObjectGuid& guid, uint8 v1, uint8 v2, uint8 v3, uint8 v4, uint8 v5, uint8 v6, uint8 v7, uint8 v8)     { WriteGuidMaskList(guid, 8, v1, v2, v3, v4, v5, v6, v7, v8); }

    void ReadGuidBytesList(ObjectGuid& guid, int count, ...)
    {
        va_list ap;
        va_start(ap, count);
        for (uint8 i = 0; i<count; ++i)
        {
            uint8 offset = va_arg(ap, uint32);
            ReadByteSeq(guid[offset]);
        }
        va_end(ap);
    }
    void ReadGuidBytes(ObjectGuid& guid, uint8 v1)                                                                          { ReadGuidBytesList(guid, 1, v1); }
    void ReadGuidBytes(ObjectGuid& guid, uint8 v1, uint8 v2)                                                                { ReadGuidBytesList(guid, 2, v1, v2); }
    void ReadGuidBytes(ObjectGuid& guid, uint8 v1, uint8 v2, uint8 v3)                                                      { ReadGuidBytesList(guid, 3, v1, v2, v3); }
    void ReadGuidBytes(ObjectGuid& guid, uint8 v1, uint8 v2, uint8 v3, uint8 v4)                                            { ReadGuidBytesList(guid, 4, v1, v2, v3, v4); }
    void ReadGuidBytes(ObjectGuid& guid, uint8 v1, uint8 v2, uint8 v3, uint8 v4, uint8 v5)                                  { ReadGuidBytesList(guid, 5, v1, v2, v3, v4, v5); }
    void ReadGuidBytes(ObjectGuid& guid, uint8 v1, uint8 v2, uint8 v3, uint8 v4, uint8 v5, uint8 v6)                        { ReadGuidBytesList(guid, 6, v1, v2, v3, v4, v5, v6); }
    void ReadGuidBytes(ObjectGuid& guid, uint8 v1, uint8 v2, uint8 v3, uint8 v4, uint8 v5, uint8 v6, uint8 v7)              { ReadGuidBytesList(guid, 7, v1, v2, v3, v4, v5, v6, v7); }
    void ReadGuidBytes(ObjectGuid& guid, uint8 v1, uint8 v2, uint8 v3, uint8 v4, uint8 v5, uint8 v6, uint8 v7, uint8 v8)    { ReadGuidBytesList(guid, 8, v1, v2, v3, v4, v5, v6, v7, v8); }

    void WriteGuidBytesList(const ObjectGuid& guid, int count, ...)
    {
        va_list ap;
        va_start(ap, count);
        for (uint8 i = 0; i<count; ++i)
        {
            uint8 offset = va_arg(ap, uint32);
            WriteByteSeq(guid[offset]);
        }
        va_end(ap);
    }
    void WriteGuidBytes(const ObjectGuid& guid, uint8 v1)                                                                           { WriteGuidBytesList(guid, 1, v1); }
    void WriteGuidBytes(const ObjectGuid& guid, uint8 v1, uint8 v2)                                                                 { WriteGuidBytesList(guid, 2, v1, v2); }
    void WriteGuidBytes(const ObjectGuid& guid, uint8 v1, uint8 v2, uint8 v3)                                                       { WriteGuidBytesList(guid, 3, v1, v2, v3); }
    void WriteGuidBytes(const ObjectGuid& guid, uint8 v1, uint8 v2, uint8 v3, uint8 v4)                                             { WriteGuidBytesList(guid, 4, v1, v2, v3, v4); }
    void WriteGuidBytes(const ObjectGuid& guid, uint8 v1, uint8 v2, uint8 v3, uint8 v4, uint8 v5)                                   { WriteGuidBytesList(guid, 5, v1, v2, v3, v4, v5); }
    void WriteGuidBytes(const ObjectGuid& guid, uint8 v1, uint8 v2, uint8 v3, uint8 v4, uint8 v5, uint8 v6)                         { WriteGuidBytesList(guid, 6, v1, v2, v3, v4, v5, v6); }
    void WriteGuidBytes(const ObjectGuid& guid, uint8 v1, uint8 v2, uint8 v3, uint8 v4, uint8 v5, uint8 v6, uint8 v7)               { WriteGuidBytesList(guid, 7, v1, v2, v3, v4, v5, v6, v7); }
    void WriteGuidBytes(const ObjectGuid& guid, uint8 v1, uint8 v2, uint8 v3, uint8 v4, uint8 v5, uint8 v6, uint8 v7, uint8 v8)     { WriteGuidBytesList(guid, 8, v1, v2, v3, v4, v5, v6, v7, v8); }

    /**
    * @name   PutBits
    * @brief  Places specified amount of bits of value at specified position in packet.
    *         To ensure all bits are correctly written, only call this method after
    *         bit flush has been performed

    * @param  pos Position to place the value at, in bits. The entire value must fit in the packet
    *             It is advised to obtain the position using bitwpos() function.

    * @param  value Data to write.
    * @param  bitCount Number of bits to store the value on.
    */
    template <typename T> void PutBits(size_t pos, T value, uint32 bitCount)
    {
        if (!bitCount)
            throw ByteBufferSourceException((pos + bitCount) / 8, size(), 0);

        if (pos + bitCount > size() * 8)
            throw ByteBufferPositionException(false, (pos + bitCount) / 8, size(), (bitCount - 1) / 8 + 1);

        for (uint32 i = 0; i < bitCount; ++i)
        {
            size_t wp = (pos + i) / 8;
            size_t bit = (pos + i) % 8;
            if ((value >> (bitCount - i - 1)) & 1)
                _storage[wp] |= 1 << (7 - bit);
            else
                _storage[wp] &= ~(1 << (7 - bit));
        }
    }

    ByteBuffer &operator<<(uint8 value)
    {
        append<uint8>(value);
        return *this;
    }

    ByteBuffer &operator<<(uint16 value)
    {
        append<uint16>(value);
        return *this;
    }

    ByteBuffer &operator<<(uint32 value)
    {
        append<uint32>(value);
        return *this;
    }

    ByteBuffer &operator<<(uint64 value)
    {
        append<uint64>(value);
        return *this;
    }

    // signed as in 2e complement
    ByteBuffer &operator<<(int8 value)
    {
        append<int8>(value);
        return *this;
    }

    ByteBuffer &operator<<(int16 value)
    {
        append<int16>(value);
        return *this;
    }

    ByteBuffer &operator<<(int32 value)
    {
        append<int32>(value);
        return *this;
    }

    ByteBuffer &operator<<(int64 value)
    {
        append<int64>(value);
        return *this;
    }

    // floating points
    ByteBuffer &operator<<(float value)
    {
        append<float>(value);
        return *this;
    }

    ByteBuffer &operator<<(double value)
    {
        append<double>(value);
        return *this;
    }

    ByteBuffer &operator<<(const std::string &value)
    {
        if (size_t len = value.length())
            append((uint8 const*)value.c_str(), len);
        append((uint8)0);
        return *this;
    }

    ByteBuffer &operator<<(const char *str)
    {
        if (size_t len = (str ? strlen(str) : 0))
            append((uint8 const*)str, len);
        append((uint8)0);
        return *this;
    }

    ByteBuffer &operator>>(bool &value)
    {
        value = read<char>() > 0 ? true : false;
        return *this;
    }

    ByteBuffer &operator>>(uint8 &value)
    {
        value = read<uint8>();
        return *this;
    }

    ByteBuffer &operator>>(uint16 &value)
    {
        value = read<uint16>();
        return *this;
    }

    ByteBuffer &operator>>(uint32 &value)
    {
        value = read<uint32>();
        return *this;
    }

    ByteBuffer &operator>>(uint64 &value)
    {
        value = read<uint64>();
        return *this;
    }

    //signed as in 2e complement
    ByteBuffer &operator>>(int8 &value)
    {
        value = read<int8>();
        return *this;
    }

    ByteBuffer &operator>>(int16 &value)
    {
        value = read<int16>();
        return *this;
    }

    ByteBuffer &operator>>(int32 &value)
    {
        value = read<int32>();
        return *this;
    }

    ByteBuffer &operator>>(int64 &value)
    {
        value = read<int64>();
        return *this;
    }

    ByteBuffer &operator>>(float &value)
    {
        value = read<float>();
        return *this;
    }

    ByteBuffer &operator>>(double &value)
    {
        value = read<double>();
        return *this;
    }

    ByteBuffer &operator>>(std::string& value)
    {
        value.clear();
        while (rpos() < size())                         // prevent crash at wrong string format in packet
        {
            char c = read<char>();
            if (c == 0)
                break;
            value += c;
        }
        return *this;
    }

    uint8& operator[](size_t const pos)
    {
        if (pos >= size())
            throw ByteBufferPositionException(false, pos, 1, size());
        return _storage[pos];
    }

    uint8 const& operator[](size_t const pos) const
    {
        if (pos >= size())
            throw ByteBufferPositionException(false, pos, 1, size());
        return _storage[pos];
    }

    size_t rpos() const { return _rpos; }

    size_t rpos(size_t rpos_)
    {
        _rpos = rpos_;
        return _rpos;
    }

    void rfinish()
    {
        _rpos = wpos();
    }

    size_t wpos() const { return _wpos; }

    size_t wpos(size_t wpos_)
    {
        _wpos = wpos_;
        return _wpos;
    }

    /// Returns position of last written bit
    size_t bitwpos() const { return _wpos * 8 + 8 - _bitpos; }

    size_t bitwpos(size_t newPos)
    {
        _wpos = newPos / 8;
        _bitpos = 8 - (newPos % 8);
        return _wpos * 8 + 8 - _bitpos;
    }

    template<typename T>
    void read_skip() { read_skip(sizeof(T)); }

    void read_skip(size_t skip)
    {
        if (_rpos + skip > size())
            throw ByteBufferPositionException(false, _rpos, skip, size());
        _rpos += skip;
    }

    template <typename T> T read()
    {
        T r = read<T>(_rpos);
        _rpos += sizeof(T);
        return r;
    }

    template <typename T> T read(size_t pos) const
    {
        if (pos + sizeof(T) > size())
            throw ByteBufferPositionException(false, pos, sizeof(T), size());
        T val = *((T const*)&_storage[pos]);
        EndianConvert(val);
        return val;
    }

    void read(uint8 *dest, size_t len)
    {
        if (_rpos + len > size())
            throw ByteBufferPositionException(false, _rpos, len, size());
        std::memcpy(dest, &_storage[_rpos], len);
        _rpos += len;
    }

    void readPackGUID(uint64& guid)
    {
        if (rpos() + 1 > size())
            throw ByteBufferPositionException(false, _rpos, 1, size());

        guid = 0;

        uint8 guidmark = 0;
        (*this) >> guidmark;

        for (int i = 0; i < 8; ++i)
        {
            if (guidmark & (uint8(1) << i))
            {
                if (rpos() + 1 > size())
                    throw ByteBufferPositionException(false, _rpos, 1, size());

                uint8 bit;
                (*this) >> bit;
                guid |= (uint64(bit) << (i * 8));
            }
        }
    }

    std::string ReadString(uint32 length)
    {
        if (!length)
            return std::string();
        char* buffer = new char[length + 1]();
        read((uint8*)buffer, length);
        std::string retval = buffer;
        delete[] buffer;
        return retval;
    }

    //! Method for writing strings that have their length sent separately in packet
    //! without null-terminating the string
    void WriteString(std::string const& str)
    {
        if (size_t len = str.length())
            append(str.c_str(), len);
    }

    uint32 ReadPackedTime()
    {
        uint32 packedDate = read<uint32>();
        tm lt = tm();

        lt.tm_min = packedDate & 0x3F;
        lt.tm_hour = (packedDate >> 6) & 0x1F;
        //lt.tm_wday = (packedDate >> 11) & 7;
        lt.tm_mday = ((packedDate >> 14) & 0x3F) + 1;
        lt.tm_mon = (packedDate >> 20) & 0xF;
        lt.tm_year = ((packedDate >> 24) & 0x1F) + 100;

        return uint32(mktime(&lt) + SF_TIMEZONE);
    }

    ByteBuffer& ReadPackedTime(uint32& time)
    {
        time = ReadPackedTime();
        return *this;
    }

    uint8 * contents() { return &_storage[0]; }

    const uint8 *contents() const { return &_storage[0]; }

    size_t size() const { return _storage.size(); }
    bool empty() const { return _storage.empty(); }

    void resize(size_t newsize)
    {
        _storage.resize(newsize, 0);
        _rpos = 0;
        _wpos = size();
    }

    void reserve(size_t ressize)
    {
        if (ressize > size())
            _storage.reserve(ressize);
    }

    void append(const char *src, size_t cnt)
    {
        return append((const uint8 *)src, cnt);
    }

    template<class T> void append(const T *src, size_t cnt)
    {
        return append((const uint8 *)src, cnt * sizeof(T));
    }

    void append(const uint8 *src, size_t cnt)
    {
        if (!cnt)
            throw ByteBufferSourceException(_wpos, size(), cnt);

        if (!src)
            throw ByteBufferSourceException(_wpos, size(), cnt);

        ASSERT(size() < 10000000);

        if (_storage.size() < _wpos + cnt)
            _storage.resize(_wpos + cnt);
        std::memcpy(&_storage[_wpos], src, cnt);
        _wpos += cnt;
    }

    void append(const ByteBuffer& buffer)
    {
        if (buffer.wpos())
            append(buffer.contents(), buffer.wpos());
    }

    void AppendBits(const ByteBuffer& buffer)
    {
        for (uint32 i = 0; i < buffer.wpos(); ++i)
            WriteBits(*(buffer.contents() + i), 8);

        WriteBits(buffer._curbitval >> buffer._bitpos, 8 - buffer._bitpos);
    }

    // can be used in SMSG_ON_MONSTER_MOVE opcode
    void appendPackXYZ(float x, float y, float z)
    {
        uint32 packed = 0;
        packed |= ((int)(x / 0.25f) & 0x7FF);
        packed |= ((int)(y / 0.25f) & 0x7FF) << 11;
        packed |= ((int)(z / 0.25f) & 0x3FF) << 22;
        *this << packed;
    }

    void appendPackGUID(uint64 guid)
    {
        uint8 packGUID[8 + 1];
        packGUID[0] = 0;
        size_t size = 1;
        for (uint8 i = 0; guid != 0; ++i)
        {
            if (guid & 0xFF)
            {
                packGUID[0] |= uint8(1 << i);
                packGUID[size] = uint8(guid & 0xFF);
                ++size;
            }

            guid >>= 8;
        }
        append(packGUID, size);
    }

    void AppendPackedTime(time_t time)
    {
        tm lt;
        ACE_OS::localtime_r(&time, &lt);
        append<uint32>((lt.tm_year - 100) << 24 | lt.tm_mon << 20 | (lt.tm_mday - 1) << 14 | lt.tm_wday << 11 | lt.tm_hour << 6 | lt.tm_min);
    }

    void put(size_t pos, const uint8 *src, size_t cnt)
    {
        if (pos + cnt > size())
            throw ByteBufferPositionException(true, pos, cnt, size());

        if (!src)
            throw ByteBufferSourceException(_wpos, size(), cnt);

        std::memcpy(&_storage[pos], src, cnt);
    }

    void print_storage() const;

    void textlike() const;

    void hexlike() const;

protected:
    size_t _rpos, _wpos, _bitpos;
    uint8 _curbitval;
    std::vector<uint8> _storage;
};

template <typename T>
inline ByteBuffer &operator<<(ByteBuffer &b, std::vector<T> v)
{
    b << (uint32)v.size();
    for (typename std::vector<T>::iterator i = v.begin(); i != v.end(); ++i)
    {
        b << *i;
    }
    return b;
}

template <typename T>
inline ByteBuffer &operator>>(ByteBuffer &b, std::vector<T> &v)
{
    uint32 vsize;
    b >> vsize;
    v.clear();
    while (vsize--)
    {
        T t;
        b >> t;
        v.push_back(t);
    }
    return b;
}

template <typename T>
inline ByteBuffer &operator<<(ByteBuffer &b, std::list<T> v)
{
    b << (uint32)v.size();
    for (typename std::list<T>::iterator i = v.begin(); i != v.end(); ++i)
    {
        b << *i;
    }
    return b;
}

template <typename T>
inline ByteBuffer &operator>>(ByteBuffer &b, std::list<T> &v)
{
    uint32 vsize;
    b >> vsize;
    v.clear();
    while (vsize--)
    {
        T t;
        b >> t;
        v.push_back(t);
    }
    return b;
}

template <typename K, typename V>
inline ByteBuffer &operator<<(ByteBuffer &b, std::map<K, V> &m)
{
    b << (uint32)m.size();
    for (typename std::map<K, V>::iterator i = m.begin(); i != m.end(); ++i)
    {
        b << i->first << i->second;
    }
    return b;
}

template <typename K, typename V>
inline ByteBuffer &operator>>(ByteBuffer &b, std::map<K, V> &m)
{
    uint32 msize;
    b >> msize;
    m.clear();
    while (msize--)
    {
        K k;
        V v;
        b >> k >> v;
        m.insert(make_pair(k, v));
    }
    return b;
}

/// @todo Make a ByteBuffer.cpp and move all this inlining to it.
template<> inline std::string ByteBuffer::read<std::string>()
{
    std::string tmp;
    *this >> tmp;
    return tmp;
}

template<>
inline void ByteBuffer::read_skip<char*>()
{
    std::string temp;
    *this >> temp;
}

template<>
inline void ByteBuffer::read_skip<char const*>()
{
    read_skip<char*>();
}

template<>
inline void ByteBuffer::read_skip<std::string>()
{
    read_skip<char*>();
}

#endif
