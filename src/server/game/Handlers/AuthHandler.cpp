/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "Opcodes.h"
#include "WorldSession.h"
#include "WorldPacket.h"
#include "Config.h"

void WorldSession::SendAuthResponse(ResponseCodes code, bool queued, uint32 queuePos)
{
    std::map<uint32, std::string> realmNamesToSend;

    QueryResult classResult = LoginDatabase.PQuery("SELECT class, expansion FROM realm_classes WHERE realmId = %u", realmID);
    QueryResult raceResult = LoginDatabase.PQuery("SELECT race, expansion FROM realm_races WHERE realmId = %u", realmID);

    if (!classResult || !raceResult)
    {
        SF_LOG_ERROR("network", "Unable to retrieve class or race data.");
        return;
    }

    RealmNameMap::const_iterator iter = realmNameStore.find(realmID);
    if (iter != realmNameStore.end()) // Add local realm
        realmNamesToSend[realmID] = iter->second;

    SF_LOG_DEBUG("network", "SMSG_AUTH_RESPONSE");
    WorldPacket packet(SMSG_AUTH_RESPONSE, 80);

    packet.WriteBit(code == ResponseCodes::AUTH_OK);

    if (code == ResponseCodes::AUTH_OK)
    {
        packet.WriteBits(realmNamesToSend.size(), 21); // Send current realmId

        for (std::map<uint32, std::string>::const_iterator itr = realmNamesToSend.begin(); itr != realmNamesToSend.end(); ++itr)
        {
            packet.WriteBits(itr->second.size(), 8);
            packet.WriteBits(itr->second.size(), 8);
            packet.WriteBit(itr->first == realmID); // Home realm
        }

        packet.WriteBits(classResult->GetRowCount(), 23);
        packet.WriteBits(0, 21);
        packet.WriteBit(0);
        packet.WriteBit(0);
        packet.WriteBit(0);
        packet.WriteBit(0);
        packet.WriteBits(raceResult->GetRowCount(), 23);
        packet.WriteBit(0);
    }

    packet.WriteBit(queued);

    if (queued)
        packet.WriteBit(1);                             // Unknown

    packet.FlushBits();

    if (queued)
        packet << uint32(0);                            // Unknown

    if (code == ResponseCodes::AUTH_OK)
    {
        for (std::map<uint32, std::string>::const_iterator itr = realmNamesToSend.begin(); itr != realmNamesToSend.end(); ++itr)
        {
            packet << uint32(itr->first);
            packet.WriteString(itr->second);
            packet.WriteString(itr->second);
        }

        do
        {
            Field* fields = raceResult->Fetch();

            packet << fields[1].GetUInt8();
            packet << fields[0].GetUInt8();
        }
        while (raceResult->NextRow());

        do
        {
            Field* fields = classResult->Fetch();

            packet << fields[1].GetUInt8();
            packet << fields[0].GetUInt8();
        }
        while (classResult->NextRow());

        packet << uint32(0);
        packet << uint8(Expansion());
        packet << uint32(Expansion());
        packet << uint32(0);
        packet << uint8(Expansion());
        packet << uint32(0);
        packet << uint32(0);
        packet << uint32(0);
    }

    packet << uint8(code);                             // Auth response ?

    SendPacket(&packet);
}

void WorldSession::SendClientCacheVersion(uint32 version)
{
    WorldPacket data(SMSG_CLIENTCACHE_VERSION, 4);
    data << uint32(version);
    SendPacket(&data);
}
