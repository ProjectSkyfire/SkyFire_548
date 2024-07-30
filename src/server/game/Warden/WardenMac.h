/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_WARDEN_MAC_H
#define SF_WARDEN_MAC_H

#include "ByteBuffer.h"
#include "Cryptography/ARC4.h"
#include "Warden.h"
#include <map>

class WorldSession;
class Warden;

class WardenMac : public Warden
{
public:
    WardenMac();
    ~WardenMac();

    void Init(WorldSession* session, SessionKey const& k);
    ClientWardenModule* GetModuleForClient();
    void InitializeModule();
    void RequestHash();
    void HandleHashResult(ByteBuffer& buff);
    void RequestData();
    void HandleData(ByteBuffer& buff);
};

#endif
