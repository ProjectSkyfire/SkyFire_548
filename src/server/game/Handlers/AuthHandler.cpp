/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Config.h"
#include "Opcodes.h"
#include "WorldPacket.h"
#include "WorldSession.h"

#define PLAYABLE_RACES_COUNT 15
#define PLAYABLE_CLASSES_COUNT 11

uint8 raceExpansion[PLAYABLE_RACES_COUNT][2] =
{
    { RACE_TAUREN,            EXPANSION_CLASSIC             },
    { RACE_UNDEAD_PLAYER,     EXPANSION_CLASSIC             },
    { RACE_ORC,               EXPANSION_CLASSIC             },
    { RACE_GNOME,             EXPANSION_CLASSIC             },
    { RACE_GOBLIN,            EXPANSION_CATACLYSM           },
    { RACE_HUMAN,             EXPANSION_CLASSIC             },
    { RACE_TROLL,             EXPANSION_CLASSIC             },
    { RACE_PANDAREN_NEUTRAL,  EXPANSION_MISTS_OF_PANDARIA   },
    { RACE_DRAENEI,           EXPANSION_THE_BURNING_CRUSADE },
    { RACE_WORGEN,            EXPANSION_CATACLYSM           },
    { RACE_BLOODELF,          EXPANSION_THE_BURNING_CRUSADE },
    { RACE_NIGHTELF,          EXPANSION_CLASSIC             },
    { RACE_DWARF,             EXPANSION_CLASSIC             },
    { RACE_PANDAREN_ALLIANCE, EXPANSION_MISTS_OF_PANDARIA   },
    { RACE_PANDAREN_HORDE,    EXPANSION_MISTS_OF_PANDARIA   },
};

uint8 classExpansion[PLAYABLE_CLASSES_COUNT][2] =
{
    { CLASS_MONK,         EXPANSION_MISTS_OF_PANDARIA      },
    { CLASS_WARRIOR,      EXPANSION_CLASSIC                },
    { CLASS_PALADIN,      EXPANSION_CLASSIC                },
    { CLASS_HUNTER,       EXPANSION_CLASSIC                },
    { CLASS_ROGUE,        EXPANSION_CLASSIC                },
    { CLASS_PRIEST,       EXPANSION_CLASSIC                },
    { CLASS_SHAMAN,       EXPANSION_CLASSIC                },
    { CLASS_MAGE,         EXPANSION_CLASSIC                },
    { CLASS_WARLOCK,      EXPANSION_CLASSIC                },
    { CLASS_DRUID,        EXPANSION_CLASSIC                },
    { CLASS_DEATH_KNIGHT, EXPANSION_WRATH_OF_THE_LICH_KING },
};

void WorldSession::SendAuthResponse(ResponseCodes code, bool queued, uint32 queuePos)
{
    SF_LOG_DEBUG("network", "SMSG_AUTH_RESPONSE");
    WorldPacket packet(SMSG_AUTH_RESPONSE, 113);

    packet.WriteBit(code == ResponseCodes::AUTH_OK);

    if (code == ResponseCodes::AUTH_OK)
    {
        packet.WriteBits(realmNameStore.size(), 21); // Send current realmId

        for (std::map<uint32, std::string>::const_iterator itr = realmNameStore.begin(); itr != realmNameStore.end(); ++itr)
        {
            packet.WriteBits(itr->second.size(), 8);
            std::string normalized = itr->second;
            normalized.erase(std::remove_if(normalized.begin(), normalized.end(), ::isspace), normalized.end());
            packet.WriteBits(normalized.size(), 8);
            packet.WriteBit(itr->first == GetVirtualRealmID()); // Home realm
        }

        packet.WriteBits(PLAYABLE_CLASSES_COUNT, 23);
        packet.WriteBits(0, 21);
        packet.WriteBit(0);
        packet.WriteBit(0);
        packet.WriteBit(0);
        packet.WriteBit(0);
        packet.WriteBits(PLAYABLE_RACES_COUNT, 23);
        packet.WriteBit(0);
    }

    packet.WriteBit(queued);

    if (queued)
        packet.WriteBit(1);

    packet.FlushBits();

    if (queued)
        packet << uint32(queuePos);

    if (code == ResponseCodes::AUTH_OK)
    {
        for (std::map<uint32, std::string>::const_iterator itr = realmNameStore.begin(); itr != realmNameStore.end(); ++itr)
        {
            packet << uint32(itr->first);
            packet.WriteString(itr->second);
            std::string normalized = itr->second;
            normalized.erase(std::remove_if(normalized.begin(), normalized.end(), ::isspace), normalized.end());
            packet.WriteString(normalized);
        }

        for (int i = 0; i < PLAYABLE_RACES_COUNT; i++)
        {
            packet << uint8(raceExpansion[i][1]);
            packet << uint8(raceExpansion[i][0]);
        }

        for (int i = 0; i < PLAYABLE_CLASSES_COUNT; i++)
        {
            packet << uint8(classExpansion[i][1]);
            packet << uint8(classExpansion[i][0]);
        }

        packet << uint32(0);
        packet << uint8(Expansion()); // Active Expansion
        packet << uint32(0);
        packet << uint32(0);          // "Your playtime expires in %u minutes." Gossip Code Box Warning, 1 = 1min
        packet << uint8(Expansion()); // Server Expansion
        packet << uint32(0);
        packet << uint32(0);
        packet << uint32(0);
    }

    packet << uint8(code);

    SendPacket(&packet);
}

void WorldSession::SendClientCacheVersion(uint32 version)
{
    WorldPacket data(SMSG_CLIENTCACHE_VERSION, 4);
    data << uint32(version);
    SendPacket(&data);
}
