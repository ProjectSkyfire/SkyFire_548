/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
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

#include "Opcodes.h"
#include "WorldSession.h"
#include "WorldPacket.h"

void WorldSession::SendAuthResponse(uint8 code, bool queued, uint32 queuePos)
{   
    QueryResult result = LoginDatabase.PQuery("SELECT class, expansion FROM realm_classes WHERE realmId = %u", realmID);
    QueryResult result2 = LoginDatabase.PQuery("SELECT race, expansion FROM realm_races WHERE realmId = %u", realmID);

    if (!result || !result2)
    {
        TC_LOG_ERROR("network", "Unable to retrieve class or race data.");
        return;
    }

    WorldPacket packet(SMSG_AUTH_RESPONSE, 80);
    
    packet << uint8(code);                             // Auth response ?

    packet.WriteBit(queued);
    if (queued)
        packet.WriteBit(1);                             // Unknown

    packet.WriteBit(code == AUTH_OK);

    if (code == AUTH_OK)
    {
        packet.WriteBit(0);
        packet.WriteBits(0, 21);
        packet.WriteBits(0, 21);
        packet.WriteBits(result2->GetRowCount(), 23);
        packet.WriteBit(0);
        packet.WriteBit(0);
        packet.WriteBit(0);
        packet.WriteBits(result->GetRowCount(), 23);
    }
    TC_LOG_ERROR("network", "SMSG_AUTH_RESPONSE");
    packet.FlushBits();

    if (code == AUTH_OK)
    {
        packet << uint32(0);
        packet << uint32(Expansion());
        packet << uint8(Expansion());
        
        do
        {
            Field* fields = result2->Fetch();
            
            packet << fields[1].GetUInt8();
            packet << fields[0].GetUInt8();
        } 
        while (result2->NextRow());   
        
        packet << uint8(Expansion());
        packet << uint32(0);

        do
        {
            Field* fields = result->Fetch();
            
            packet << fields[0].GetUInt8();
            packet << fields[1].GetUInt8();
        } 
        while (result->NextRow());     

        packet << uint32(0);
        packet << uint32(0);
    }

    SendPacket(&packet);
}

void WorldSession::SendClientCacheVersion(uint32 version)
{
    WorldPacket data(SMSG_CLIENTCACHE_VERSION, 4);
    data << uint32(version);
    SendPacket(&data);
}
