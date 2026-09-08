/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_AUTHNETCONNECTTO_H
#define SF_AUTHNETCONNECTTO_H

#include "AuthDefines.h"
#include "Common.h"
#include <array>
#include <string>

class WorldPacket;

namespace AuthnetConnectTo
{
    struct PendingConnection
    {
        uint32 AccountId = 0;
        std::string Login;
        SessionKey WorldSessionKey = {};
    };

    uint64 GenerateKey(uint32 accountId);
    void Register(uint64 key, PendingConnection connection);
    bool Consume(uint64 key, PendingConnection& connection);
    void Remove(uint64 key);

    WorldPacket BuildPacket(uint64 key, std::array<uint8, 16> const& address,
        uint32 addressType, uint16 port);
}

#endif
