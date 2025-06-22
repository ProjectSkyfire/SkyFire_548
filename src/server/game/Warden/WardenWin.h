/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_WARDEN_WIN_H
#define SF_WARDEN_WIN_H

#include "ByteBuffer.h"
#include "Cryptography/ARC4.h"
#include "Cryptography/BigNumber.h"
#include "Warden.h"
#include <map>

#if defined(__GNUC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

struct WardenInitModuleRequest
{
    uint8 Command1;
    uint16 Size1;
    uint32 CheckSumm1;
    uint8 Unk1;
    uint8 Unk2;
    uint8 Type;
    uint8 String_library1;
    uint32 Function1[4];

    uint8 Command2;
    uint16 Size2;
    uint32 CheckSumm2;
    uint8 Unk3;
    uint8 Unk4;
    uint8 String_library2;
    uint32 Function2;
    uint8 Function2_set;

    uint8 Command3;
    uint16 Size3;
    uint32 CheckSumm3;
    uint8 Unk5;
    uint8 Unk6;
    uint8 String_library3;
    uint32 Function3;
    uint8 Function3_set;
};

#if defined(__GNUC__)
#pragma pack()
#else
#pragma pack(pop)
#endif

class WorldSession;
class Warden;

class WardenWin : public Warden
{
public:
    WardenWin();
    ~WardenWin();

    void Init(WorldSession* session, SessionKey const& K);
    ClientWardenModule* GetModuleForClient();
    void InitializeModule();
    void RequestHash();
    void HandleHashResult(ByteBuffer& buff);
    void RequestData();
    void HandleData(ByteBuffer& buff);

private:
    uint32 _serverTicks;
    std::list<uint16> _otherChecksTodo;
    std::list<uint16> _memChecksTodo;
    std::list<uint16> _currentChecks;
};

#endif
