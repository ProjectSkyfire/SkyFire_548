/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_WORLD_OPCODE_DISPATCH_H
#define SKYFIRE_WORLD_OPCODE_DISPATCH_H
#include "../../../shared/Protocol/Opcodes.h"

/// Player state
enum SessionStatus
{
    STATUS_AUTHED = 0,                                      // Player authenticated (_player == NULL, m_playerRecentlyLogout = false or will be reset before handler call, m_GUID have garbage)
    STATUS_LOGGEDIN,                                        // Player in game (_player != NULL, m_GUID == _player->GetGUID(), inWorld())
    STATUS_TRANSFER,                                        // Player transferring to another map (_player != NULL, m_GUID == _player->GetGUID(), !inWorld())
    STATUS_LOGGEDIN_OR_RECENTLY_LOGGOUT,                    // _player != NULL or _player == NULL && m_playerRecentlyLogout && m_playerLogout, m_GUID store last _player guid)
    STATUS_NEVER,                                           // Opcode not accepted from client (deprecated or server side only)
    STATUS_UNHANDLED                                        // Opcode not handled yet
};

enum PacketProcessing
{
    PROCESS_INPLACE = 0,                                    // Process packet whenever we receive it - mostly for non-handled or non-implemented packets
    PROCESS_THREADUNSAFE,                                   // Packet is not thread-safe - process it in World::UpdateSessions()
    PROCESS_THREADSAFE                                      // Packet is thread-safe - process it in Map::Update()
};

class WorldPacket;
class WorldSession;

typedef void(WorldSession::* pOpcodeHandler)(WorldPacket& recvPacket);

struct OpcodeHandler
{
    OpcodeHandler(uint16 opcodeNumber, char const* _name, SessionStatus _status, PacketProcessing _processing, pOpcodeHandler _handler)
        : Handler(_handler), Name(_name), Status(_status), ProcessingPlace(_processing), OpcodeNumber(opcodeNumber) { }

    pOpcodeHandler Handler;
    char const* Name;
    SessionStatus Status;
    PacketProcessing ProcessingPlace;
    uint16 OpcodeNumber;
};

class OpcodeTable
{
public:
    OpcodeTable()
    {
        memset(_internalTable, 0, sizeof(_internalTable));
        memset(_opcodeTable, 0, sizeof(_opcodeTable));
    }

    ~OpcodeTable()
    {
        for (uint16 i = 0; i < NUM_OPCODES; ++i)
            delete _internalTable[i];
    }

    void InitializeClientTable();
    void InitializeServerTable();

    inline Opcodes GetOpcodeByNumber(uint16 number) const { return _opcodeTable[number]; }

    OpcodeHandler const* operator[](uint32 index) const
    {
        return _internalTable[index];
    }

private:
    template<bool isInValidRange, bool isNonZero>
    void ValidateAndSetOpcode(uint16 opcode, uint16 opcodeNumber, char const* name, SessionStatus status, PacketProcessing processing, pOpcodeHandler handler = 0);

    // Prevent copying this structure
    OpcodeTable(OpcodeTable const&);
    OpcodeTable& operator=(OpcodeTable const&);

    OpcodeHandler* _internalTable[NUM_OPCODES];

    // Store opcode / number list - for speed
    Opcodes _opcodeTable[NUM_OPCODE_HANDLERS];
};

extern OpcodeTable serverOpcodeTable;
extern OpcodeTable clientOpcodeTable;

void InitializeClientTable();
void InitializeServerTable();

#endif
