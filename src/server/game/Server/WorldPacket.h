/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRESERVER_WORLDPACKET_H
#define SKYFIRESERVER_WORLDPACKET_H

#include "ByteBuffer.h"
#include "Common.h"
#include "Opcodes.h"

struct z_stream_s;

class WorldPacket : public ByteBuffer
{
public:
    // just container for later use
    WorldPacket() : ByteBuffer(0), m_opcode(UNKNOWN_OPCODE), m_rcvdOpcodeNumber(0), _compressionStream(NULL)
    {
    }

    WorldPacket(Opcodes opcode, size_t res = 200) : ByteBuffer(res), m_opcode(opcode), m_rcvdOpcodeNumber(0), _compressionStream(NULL)
    {
    }
    // copy constructor
    WorldPacket(WorldPacket const& packet) : ByteBuffer(packet), m_opcode(packet.m_opcode), m_rcvdOpcodeNumber(0), _compressionStream(NULL)
    {
    }

    void Initialize(Opcodes opcode, size_t newres = 200)
    {
        clear();
        _storage.reserve(newres);
        m_opcode = opcode;
    }

    Opcodes GetOpcode() const { return m_opcode; }
    void SetOpcode(Opcodes opcode) { m_opcode = opcode; }
    void Compress(z_stream_s* compressionStream);
    void Compress(z_stream_s* compressionStream, WorldPacket const* source);
    void SetReceivedOpcode(uint16 opcode) { m_rcvdOpcodeNumber = opcode; }
    uint16 GetReceivedOpcode() { return m_rcvdOpcodeNumber; }

protected:
    Opcodes m_opcode;
    uint16 m_rcvdOpcodeNumber;
    void Compress(void* dst, uint32* dst_size, const void* src, int src_size);
    z_stream_s* _compressionStream;
};
#endif
