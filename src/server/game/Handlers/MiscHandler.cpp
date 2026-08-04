/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "AccountMgr.h"
#include "AccountMgr.h"
#include "AccountDataUtils.h"
#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "BigNumber.h"
#include "CellImpl.h"
#include "CharacterBoost.h"
#include "Chat.h"
#include "Common.h"
#include "CreatureAI.h"
#include "DatabaseEnv.h"
#include "DB2Stores.h"
#include "DBCEnums.h"
#include "GameObjectAI.h"
#include "GossipDef.h"
#include "Group.h"
#include "GuildMgr.h"
#include "InstanceScript.h"
#include "Language.h"
#include "Log.h"
#include "LootMgr.h"
#include "MapManager.h"
#include "Object.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "OutdoorPvP.h"
#include "Pet.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SocialMgr.h"
#include "Spell.h"
#include "UpdateData.h"
#include "Vehicle.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "zlib.h"

#include <cstddef>

namespace
{
struct GuidRequest
{
    ObjectGuid guid;
};

struct UInt8Request
{
    uint8 value;
};

struct UInt32Request
{
    uint32 value;
};

struct Int32Request
{
    int32 value;
};

struct UInt64Request
{
    uint64 value;
};

struct BoolRequest
{
    bool value;
};

struct StringRequest
{
    std::string value;
};

struct GossipSelectOptionRequest
{
    ObjectGuid guid;
    std::string code;
    uint32 gossipListId;
    uint32 menuId;
};

struct SetPvPRequest
{
    bool hasStatus;
    bool newPvPStatus;
};

struct AddFriendRequest
{
    std::string name;
    std::string note;
};

struct ContactNotesRequest
{
    uint64 guid;
    std::string note;
};

struct BugReportRequest
{
    std::string content;
    std::string type;
    uint32 suggestion;
};

struct ResurrectResponseRequest
{
    ObjectGuid guid;
    uint32 status;
};

struct AreaTriggerRequest
{
    uint32 triggerId;
    uint8 unk1;
    uint8 unk2;
};

struct ActionButtonRequest
{
    ObjectGuid buttonStream;
    uint8 slotId;
};

struct WorldTeleportRequest
{
    uint32 time;
    uint32 mapId;
    float positionX;
    float positionY;
    float positionZ;
    float orientation;
};

struct ComplainRequest
{
    uint64 spammerGuid;
    uint32 unk1;
    uint32 unk2;
    uint32 unk3;
    uint32 unk4;
    std::string description;
    uint8 spamType;
};

struct TimeSyncRequest
{
    uint32 counter;
    uint32 clientTicks;
};

struct DiscardedTimeSyncAcksRequest
{
    bool hasCounter;
    uint32 counter;
};

struct UpdateMissileTrajectoryRequest
{
    uint64 guid;
    uint32 spellId;
    float elevation;
    float speed;
    float curX;
    float curY;
    float curZ;
    float targetX;
    float targetY;
    float targetZ;
    uint8 moveStop;
};

struct SceneCompletedRequest
{
    uint32 unk;
    uint8 hasData;
};

void ReadRepopRequest(WorldPacket& recvData)
{
    recvData.read_skip<uint8>();
}

GossipSelectOptionRequest ReadGossipSelectOptionRequest(WorldPacket& recvData, Player* player)
{
    GossipSelectOptionRequest request;
    uint8 boxTextLength = 0;

    recvData >> request.gossipListId >> request.menuId;

    request.guid[3] = recvData.ReadBit();
    request.guid[0] = recvData.ReadBit();
    request.guid[1] = recvData.ReadBit();
    request.guid[4] = recvData.ReadBit();
    request.guid[7] = recvData.ReadBit();
    request.guid[5] = recvData.ReadBit();
    request.guid[6] = recvData.ReadBit();
    boxTextLength = recvData.ReadBits(8);
    request.guid[2] = recvData.ReadBit();

    recvData.ReadByteSeq(request.guid[7]);
    recvData.ReadByteSeq(request.guid[3]);
    recvData.ReadByteSeq(request.guid[4]);
    recvData.ReadByteSeq(request.guid[6]);
    recvData.ReadByteSeq(request.guid[0]);
    recvData.ReadByteSeq(request.guid[5]);

    if (player->PlayerTalkClass->IsGossipOptionCoded(request.gossipListId))
        request.code = recvData.ReadString(boxTextLength);

    recvData.ReadByteSeq(request.guid[2]);
    recvData.ReadByteSeq(request.guid[1]);

    return request;
}

SetPvPRequest ReadSetPvPRequest(WorldPacket& recvData)
{
    SetPvPRequest request;
    request.hasStatus = recvData.size() == 1;
    request.newPvPStatus = false;

    if (request.hasStatus)
        request.newPvPStatus = recvData.ReadBit();

    return request;
}

UInt32Request ReadUInt32Request(WorldPacket& recvData)
{
    UInt32Request request;
    recvData >> request.value;
    return request;
}

Int32Request ReadInt32Request(WorldPacket& recvData)
{
    Int32Request request;
    recvData >> request.value;
    return request;
}

UInt8Request ReadUInt8Request(WorldPacket& recvData)
{
    UInt8Request request;
    recvData >> request.value;
    return request;
}

UInt64Request ReadUInt64Request(WorldPacket& recvData)
{
    UInt64Request request;
    recvData >> request.value;
    return request;
}

StringRequest ReadStringRequest(WorldPacket& recvData)
{
    StringRequest request;
    recvData >> request.value;
    return request;
}

GuidRequest ReadSetSelectionRequest(WorldPacket& recvData)
{
    GuidRequest request;

    request.guid[7] = recvData.ReadBit();
    request.guid[6] = recvData.ReadBit();
    request.guid[5] = recvData.ReadBit();
    request.guid[4] = recvData.ReadBit();
    request.guid[3] = recvData.ReadBit();
    request.guid[2] = recvData.ReadBit();
    request.guid[1] = recvData.ReadBit();
    request.guid[0] = recvData.ReadBit();

    recvData.ReadByteSeq(request.guid[0]);
    recvData.ReadByteSeq(request.guid[7]);
    recvData.ReadByteSeq(request.guid[3]);
    recvData.ReadByteSeq(request.guid[5]);
    recvData.ReadByteSeq(request.guid[1]);
    recvData.ReadByteSeq(request.guid[4]);
    recvData.ReadByteSeq(request.guid[6]);
    recvData.ReadByteSeq(request.guid[2]);

    return request;
}

void ReadContactListRequest(WorldPacket& recvData)
{
    recvData.read_skip<uint32>(); // always 1
}

AddFriendRequest ReadAddFriendRequest(WorldPacket& recvData)
{
    AddFriendRequest request;
    recvData >> request.name;
    recvData >> request.note;
    return request;
}

StringRequest ReadAddIgnoreRequest(WorldPacket& recvData)
{
    return ReadStringRequest(recvData);
}

ContactNotesRequest ReadSetContactNotesRequest(WorldPacket& recvData)
{
    ContactNotesRequest request;
    recvData >> request.guid >> request.note;
    return request;
}

BugReportRequest ReadBugReportRequest(WorldPacket& recvData)
{
    BugReportRequest request;
    uint32 contentLength;
    uint32 typeLength;

    recvData >> request.suggestion >> contentLength;
    request.content = recvData.ReadString(contentLength);

    recvData >> typeLength;
    request.type = recvData.ReadString(typeLength);

    return request;
}

void ReadReclaimCorpseRequest(WorldPacket& recvData)
{
    ObjectGuid guid;

    recvData.ReadGuidMask(guid, 1, 5, 7, 2, 6, 3, 0, 4);
    recvData.ReadGuidBytes(guid, 2, 5, 4, 6, 1, 0, 7, 3);
}

ResurrectResponseRequest ReadResurrectResponseRequest(WorldPacket& recvData)
{
    ResurrectResponseRequest request;

    recvData >> request.status;

    request.guid[3] = recvData.ReadBit();
    request.guid[0] = recvData.ReadBit();
    request.guid[6] = recvData.ReadBit();
    request.guid[4] = recvData.ReadBit();
    request.guid[5] = recvData.ReadBit();
    request.guid[2] = recvData.ReadBit();
    request.guid[1] = recvData.ReadBit();
    request.guid[7] = recvData.ReadBit();

    recvData.ReadByteSeq(request.guid[7]);
    recvData.ReadByteSeq(request.guid[0]);
    recvData.ReadByteSeq(request.guid[1]);
    recvData.ReadByteSeq(request.guid[3]);
    recvData.ReadByteSeq(request.guid[4]);
    recvData.ReadByteSeq(request.guid[6]);
    recvData.ReadByteSeq(request.guid[2]);
    recvData.ReadByteSeq(request.guid[5]);

    return request;
}

AreaTriggerRequest ReadAreaTriggerRequest(WorldPacket& recvData)
{
    AreaTriggerRequest request;
    recvData >> request.triggerId;
    request.unk1 = recvData.ReadBit();
    request.unk2 = recvData.ReadBit();
    return request;
}

UInt32Request ReadRequestAccountDataRequest(WorldPacket& recvData)
{
    UInt32Request request;
    request.value = recvData.ReadBits(3);
    return request;
}

ActionButtonRequest ReadSetActionButtonRequest(WorldPacket& recvData)
{
    ActionButtonRequest request;
    recvData >> request.slotId;
    recvData.ReadGuidMask(request.buttonStream, 7, 0, 5, 2, 1, 6, 3, 4);
    recvData.ReadGuidBytes(request.buttonStream, 6, 7, 3, 5, 2, 1, 4, 0);
    return request;
}

void ReadMoveTimeSkippedRequest(WorldPacket& recvData)
{
    ObjectGuid guid;
    recvData.read_skip<uint32>(); // time skipped

    recvData.ReadGuidMask(guid, 5, 0, 7, 4, 1, 2, 6, 3);
    recvData.ReadGuidBytes(guid, 7, 2, 0, 6, 1, 5, 3, 4);
}

GuidRequest ReadInspectRequest(WorldPacket& recvData)
{
    GuidRequest request;

    request.guid[0] = recvData.ReadBit();
    request.guid[3] = recvData.ReadBit();
    request.guid[7] = recvData.ReadBit();
    request.guid[2] = recvData.ReadBit();
    request.guid[5] = recvData.ReadBit();
    request.guid[1] = recvData.ReadBit();
    request.guid[4] = recvData.ReadBit();
    request.guid[6] = recvData.ReadBit();

    recvData.ReadByteSeq(request.guid[3]);
    recvData.ReadByteSeq(request.guid[5]);
    recvData.ReadByteSeq(request.guid[2]);
    recvData.ReadByteSeq(request.guid[4]);
    recvData.ReadByteSeq(request.guid[1]);
    recvData.ReadByteSeq(request.guid[6]);
    recvData.ReadByteSeq(request.guid[0]);
    recvData.ReadByteSeq(request.guid[7]);

    return request;
}

GuidRequest ReadInspectHonorStatsRequest(WorldPacket& recvData)
{
    GuidRequest request;
    recvData.ReadGuidMask(request.guid, 4, 3, 6, 1, 0, 2, 5, 7);
    recvData.ReadGuidBytes(request.guid, 0, 5, 1, 4, 2, 6, 7, 3);
    return request;
}

WorldTeleportRequest ReadWorldTeleportRequest(WorldPacket& recvData)
{
    WorldTeleportRequest request;
    recvData >> request.time;
    recvData >> request.mapId;
    recvData >> request.positionX;
    recvData >> request.positionY;
    recvData >> request.positionZ;
    recvData >> request.orientation;
    return request;
}

ComplainRequest ReadComplainRequest(WorldPacket& recvData)
{
    ComplainRequest request = ComplainRequest();
    recvData >> request.spamType;
    recvData >> request.spammerGuid;

    switch (request.spamType)
    {
        case 0:
            recvData >> request.unk1;
            recvData >> request.unk2;
            recvData >> request.unk3;
            break;
        case 1:
            recvData >> request.unk1;
            recvData >> request.unk2;
            recvData >> request.unk3;
            recvData >> request.unk4;
            recvData >> request.description;
            break;
    }

    return request;
}

BoolRequest ReadFarSightRequest(WorldPacket& recvData)
{
    BoolRequest request;
    request.value = recvData.ReadBit();
    return request;
}

TimeSyncRequest ReadTimeSyncRequest(WorldPacket& recvData)
{
    TimeSyncRequest request;
    recvData >> request.counter >> request.clientTicks;
    return request;
}

GuidRequest ReadQueryInspectAchievementsRequest(WorldPacket& recvData)
{
    GuidRequest request;

    request.guid[2] = recvData.ReadBit();
    request.guid[7] = recvData.ReadBit();
    request.guid[1] = recvData.ReadBit();
    request.guid[5] = recvData.ReadBit();
    request.guid[4] = recvData.ReadBit();
    request.guid[0] = recvData.ReadBit();
    request.guid[3] = recvData.ReadBit();
    request.guid[6] = recvData.ReadBit();

    recvData.ReadByteSeq(request.guid[7]);
    recvData.ReadByteSeq(request.guid[2]);
    recvData.ReadByteSeq(request.guid[0]);
    recvData.ReadByteSeq(request.guid[4]);
    recvData.ReadByteSeq(request.guid[1]);
    recvData.ReadByteSeq(request.guid[5]);
    recvData.ReadByteSeq(request.guid[6]);
    recvData.ReadByteSeq(request.guid[3]);

    return request;
}

GuidRequest ReadAreaSpiritHealerQueryRequest(WorldPacket& recvData)
{
    GuidRequest request;
    recvData.ReadGuidMask(request.guid, 5, 6, 0, 4, 1, 2, 7, 3);
    recvData.ReadGuidBytes(request.guid, 0, 2, 6, 7, 1, 5, 3, 4);
    return request;
}

GuidRequest ReadAreaSpiritHealerQueueRequest(WorldPacket& recvData)
{
    GuidRequest request;

    request.guid[5] = recvData.ReadBit();
    request.guid[4] = recvData.ReadBit();
    request.guid[0] = recvData.ReadBit();
    request.guid[2] = recvData.ReadBit();
    request.guid[7] = recvData.ReadBit();
    request.guid[1] = recvData.ReadBit();
    request.guid[6] = recvData.ReadBit();
    request.guid[3] = recvData.ReadBit();

    recvData.ReadByteSeq(request.guid[1]);
    recvData.ReadByteSeq(request.guid[7]);
    recvData.ReadByteSeq(request.guid[6]);
    recvData.ReadByteSeq(request.guid[2]);
    recvData.ReadByteSeq(request.guid[4]);
    recvData.ReadByteSeq(request.guid[3]);
    recvData.ReadByteSeq(request.guid[0]);
    recvData.ReadByteSeq(request.guid[5]);

    return request;
}

BoolRequest ReadInstanceLockResponseRequest(WorldPacket& recvPacket)
{
    BoolRequest request;
    request.value = recvPacket.ReadBit();
    return request;
}

UpdateMissileTrajectoryRequest ReadUpdateMissileTrajectoryRequest(WorldPacket& recvPacket)
{
    UpdateMissileTrajectoryRequest request;
    recvPacket >> request.guid >> request.spellId >> request.elevation >> request.speed;
    recvPacket >> request.curX >> request.curY >> request.curZ;
    recvPacket >> request.targetX >> request.targetY >> request.targetZ;
    recvPacket >> request.moveStop;
    return request;
}

GuidRequest ReadObjectUpdateFailedRequest(WorldPacket& recvPacket)
{
    GuidRequest request;

    request.guid[3] = recvPacket.ReadBit();
    request.guid[5] = recvPacket.ReadBit();
    request.guid[6] = recvPacket.ReadBit();
    request.guid[0] = recvPacket.ReadBit();
    request.guid[1] = recvPacket.ReadBit();
    request.guid[2] = recvPacket.ReadBit();
    request.guid[7] = recvPacket.ReadBit();
    request.guid[4] = recvPacket.ReadBit();

    recvPacket.ReadByteSeq(request.guid[0]);
    recvPacket.ReadByteSeq(request.guid[6]);
    recvPacket.ReadByteSeq(request.guid[5]);
    recvPacket.ReadByteSeq(request.guid[7]);
    recvPacket.ReadByteSeq(request.guid[2]);
    recvPacket.ReadByteSeq(request.guid[1]);
    recvPacket.ReadByteSeq(request.guid[3]);
    recvPacket.ReadByteSeq(request.guid[4]);

    return request;
}

DiscardedTimeSyncAcksRequest ReadDiscardedTimeSyncAcksRequest(WorldPacket& recvData)
{
    DiscardedTimeSyncAcksRequest request;
    request.hasCounter = false;
    request.counter = 0;

    bool hasInfo = !recvData.ReadBit();
    if (hasInfo)
    {
        request.hasCounter = true;
        recvData >> request.counter;
    }

    return request;
}

SceneCompletedRequest ReadSceneCompletedRequest(WorldPacket& recvPacket)
{
    SceneCompletedRequest request;
    request.unk = 0;
    request.hasData = recvPacket.ReadBit();
    if (request.hasData)
        request.unk = recvPacket.read<uint32>();
    return request;
}
}

void WorldSession::HandleRepopRequestOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Recvd CMSG_REPOP_REQUEST Message");

    ReadRepopRequest(recvData);

    if (GetPlayer()->IsAlive() || GetPlayer()->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_GHOST))
        return;

    if (GetPlayer()->HasAuraType(SPELL_AURA_PREVENT_RESURRECTION))
        return; // silently return, client should display the error by itself

    // the world update order is sessions, players, creatures
    // the netcode runs in parallel with all of these
    // creatures can kill players
    // so if the server is lagging enough the player can
    // release spirit after he's killed but before he is updated
    if (GetPlayer()->getDeathState() == DeathState::JUST_DIED)
    {
        SF_LOG_DEBUG("network", "HandleRepopRequestOpcode: got request after player %s(%d) was killed and before he was updated",
            GetPlayer()->GetName().c_str(), GetPlayer()->GetGUIDLow());
        GetPlayer()->KillPlayer();
    }

    //this is spirit release confirm?
    GetPlayer()->RemovePet(NULL, PET_SAVE_NOT_IN_SLOT, true);
    GetPlayer()->BuildPlayerRepop();
    GetPlayer()->RepopAtGraveyard();
}

void WorldSession::HandleGossipSelectOptionOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_GOSSIP_SELECT_OPTION");

    GossipSelectOptionRequest request = ReadGossipSelectOptionRequest(recvData, _player);

    Creature* unit = NULL;
    GameObject* go = NULL;
    if (IS_CRE_OR_VEH_GUID(request.guid))
    {
        unit = GetPlayer()->GetNPCIfCanInteractWith(request.guid, UNIT_NPC_FLAG_NONE);
        if (!unit)
        {
            SF_LOG_DEBUG("network", "WORLD: HandleGossipSelectOptionOpcode - Unit (GUID: %u) not found or you can't interact with him.", uint32(GUID_LOPART(request.guid)));
            return;
        }
    }
    else if (IS_GAMEOBJECT_GUID(request.guid))
    {
        go = _player->GetMap()->GetGameObject(request.guid);
        if (!go)
        {
            SF_LOG_DEBUG("network", "WORLD: HandleGossipSelectOptionOpcode - GameObject (GUID: %u) not found.", uint32(GUID_LOPART(request.guid)));
            return;
        }
    }
    else
    {
        SF_LOG_DEBUG("network", "WORLD: HandleGossipSelectOptionOpcode - unsupported GUID type for highguid %u. lowpart %u.", uint32(GUID_HIPART(request.guid)), uint32(GUID_LOPART(request.guid)));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    if ((unit && unit->GetCreatureTemplate()->ScriptID != unit->LastUsedScriptID) || (go && go->GetGOInfo()->ScriptId != go->LastUsedScriptID))
    {
        SF_LOG_DEBUG("network", "WORLD: HandleGossipSelectOptionOpcode - Script reloaded while in use, ignoring and set new scipt id");
        if (unit)
            unit->LastUsedScriptID = unit->GetCreatureTemplate()->ScriptID;
        if (go)
            go->LastUsedScriptID = go->GetGOInfo()->ScriptId;
        _player->PlayerTalkClass->SendCloseGossip();
        return;
    }
    if (!request.code.empty())
    {
        if (unit)
        {
            unit->AI()->sGossipSelectCode(_player, request.menuId, request.gossipListId, request.code.c_str());
            if (!sScriptMgr->OnGossipSelectCode(_player, unit, _player->PlayerTalkClass->GetGossipOptionSender(request.gossipListId), _player->PlayerTalkClass->GetGossipOptionAction(request.gossipListId), request.code.c_str()))
                _player->OnGossipSelect(unit, request.gossipListId, request.menuId);
        }
        else
        {
            go->AI()->GossipSelectCode(_player, request.menuId, request.gossipListId, request.code.c_str());
            sScriptMgr->OnGossipSelectCode(_player, go, _player->PlayerTalkClass->GetGossipOptionSender(request.gossipListId), _player->PlayerTalkClass->GetGossipOptionAction(request.gossipListId), request.code.c_str());
        }
    }
    else
    {
        if (unit)
        {
            unit->AI()->sGossipSelect(_player, request.menuId, request.gossipListId);
            if (!sScriptMgr->OnGossipSelect(_player, unit, _player->PlayerTalkClass->GetGossipOptionSender(request.gossipListId), _player->PlayerTalkClass->GetGossipOptionAction(request.gossipListId)))
                _player->OnGossipSelect(unit, request.gossipListId, request.menuId);
        }
        else
        {
            go->AI()->GossipSelect(_player, request.menuId, request.gossipListId);
            if (!sScriptMgr->OnGossipSelect(_player, go, _player->PlayerTalkClass->GetGossipOptionSender(request.gossipListId), _player->PlayerTalkClass->GetGossipOptionAction(request.gossipListId)))
                _player->OnGossipSelect(go, request.gossipListId, request.menuId);
        }
    }
}

void WorldSession::HandleWhoOpcode(WorldPacket& recvData)
{
    //CMSG_WHO
    SF_LOG_DEBUG("network", "WORLD: Recvd CMSG_WHO Message");

    time_t now = time(NULL);
    if (now - timeLastWhoCommand < 5)
        return;
    else timeLastWhoCommand = now;

    uint32 levelMin, levelMax, raceMask, classMask, zonesCount, wordCount;
    uint32 zoneIds[10];                                     // 10 is client limit
    bool requestServerInfo, showEnemies, showArenaPlayers, exactName;
    uint8 playerLen, guildLen, realmNameLen, guildRealmNameLen;
    std::string playerName, guildName, realmName, guildRealmName;

    recvData >> classMask;                                  // race mask
    recvData >> raceMask;                                   // minimal player level, default 100 (MAX_LEVEL)
    recvData >> levelMax;                                   // maximal player level, default 0
    recvData >> levelMin;                                   // class mask

    showEnemies = recvData.ReadBit();
    exactName = recvData.ReadBit();
    requestServerInfo = recvData.ReadBit();
    guildRealmNameLen = recvData.ReadBits(9);
    showArenaPlayers = recvData.ReadBit();
    playerLen = recvData.ReadBits(6);

    zonesCount = recvData.ReadBits(4);                     // zones count, client limit = 10 (2.0.10)
    if (zonesCount > 10)                                   // can't be received from real client or broken packet
        return;

    realmNameLen = recvData.ReadBits(9);
    guildLen = recvData.ReadBits(7);

    wordCount = recvData.ReadBits(3);
    if (wordCount > 4)
        return;                                            // can't be received from real client or broken packet

    uint8* wordLens = new uint8[wordCount];
    std::string* words = new std::string[wordCount];

    for (uint8 i = 0; i < wordCount; i++)
        wordLens[i] = recvData.ReadBits(7);

    std::wstring wWords[4];
    for (uint32 i = 0; i < wordCount; ++i)
    {
        std::string temp;
        recvData >> temp;                                  // user entered string, it used as universal search pattern(guild+player name)?

        if (!Utf8toWStr(temp, wWords[i]))
            continue;

        wstrToLower(wWords[i]);

        SF_LOG_DEBUG("network", "String %u: %s", i, temp.c_str());
    }

    guildRealmName = recvData.ReadString(guildRealmNameLen);

    for (uint32 i = 0; i < zonesCount; ++i)
    {
        recvData >> zoneIds[i];                            // zone id, 0 if zone is unknown...
        SF_LOG_DEBUG("network", "Zone %u: %u", i, zoneIds[i]);
    }

    // NYI
    playerName = recvData.ReadString(playerLen);
    realmName = recvData.ReadString(realmNameLen);
    guildName = recvData.ReadString(guildLen);

    uint32 virtualRealmAddress = 0;
    int32 faction = 0, locale = 0;

    if (requestServerInfo)
    {
        recvData >> locale;
        recvData >> virtualRealmAddress;
        recvData >> faction;
    }

    SF_LOG_DEBUG("network", "Minlvl %u, maxlvl %u, name %s, guild %s, racemask %u, classmask %u, zones %u, strings %u", levelMin, levelMax, playerName.c_str(), guildName.c_str(), raceMask, classMask, zonesCount, wordCount);

    std::wstring wPlayerName, wGuildName;
    if (!(Utf8toWStr(playerName, wPlayerName) && Utf8toWStr(guildName, wGuildName)))
        return;

    wstrToLower(wPlayerName);
    wstrToLower(wGuildName);

    // SMSG_WHO
    // client send in case not set max level value 100 but Skyfire supports 255 max level,
    // update it to show GMs with characters after 100 level
    if (levelMax >= MAX_LEVEL)
        levelMax = STRONG_MAX_LEVEL;

    uint32 team = _player->GetTeam();

    //bool allowTwoSideWhoList = sWorld->GetBoolConfig(CONFIG_ALLOW_TWO_SIDE_WHO_LIST);
    uint32 gmLevelInWhoList = sWorld->getIntConfig(WorldIntConfigs::CONFIG_GM_LEVEL_IN_WHO_LIST);
    uint8 displaycount = 0, matchcount = 0;

    ByteBuffer bytesData;
    WorldPacket data(SMSG_WHO);

    size_t pos = data.bitwpos();
    data.WriteBits(displaycount, 6);

    SF_SHARED_GUARD readGuard(*HashMapHolder<Player>::GetLock());
    HashMapHolder<Player>::MapType const& m = sObjectAccessor->GetPlayers();
    for (HashMapHolder<Player>::MapType::const_iterator itr = m.begin(); itr != m.end(); ++itr)
    {
        Player* target = itr->second;
        // player can see member of other team only if CONFIG_ALLOW_TWO_SIDE_WHO_LIST
        if (target->GetTeam() != team && !HasPermission(rbac::RBAC_PERM_TWO_SIDE_WHO_LIST))
            continue;

        // player can see MODERATOR, GAME MASTER, ADMINISTRATOR only if CONFIG_GM_IN_WHO_LIST
        if (!HasPermission(rbac::RBAC_PERM_WHO_SEE_ALL_SEC_LEVELS) && target->GetSession()->GetSecurity() > AccountTypes(gmLevelInWhoList))
            continue;

        // do not process players which are not in world
        if (!target->IsInWorld())
            continue;

        // check if target is globally visible for player
        if (!target->IsVisibleGloballyFor(_player))
            continue;

        // check if target's level is in level range
        uint8 level = target->getLevel();
        if (level < levelMin || level > levelMax)
            continue;

        // check if class matches classmask
        uint8 class_ = target->getClass();
        if (!(classMask & (1 << class_)))
            continue;

        // check if race matches racemask
        uint32 race = target->getRace();
        if (!(raceMask & (1 << race)))
            continue;

        uint32 zoneId = target->GetZoneId();
        uint8 gender = target->getGender();

        bool z_show = true;
        for (uint32 i = 0; i < zonesCount; ++i)
        {
            if (zoneIds[i] == zoneId)
            {
                z_show = true;
                break;
            }

            z_show = false;
        }
        if (!z_show)
            continue;

        std::string pname = target->GetName();
        std::wstring wpname;
        if (!Utf8toWStr(pname, wpname))
            continue;
        wstrToLower(wpname);

        if (!(wPlayerName.empty() || wpname.find(wPlayerName) != std::wstring::npos))
            continue;

        std::string gname = sGuildMgr->GetGuildNameById(target->GetGuildId());
        std::wstring wgname;
        if (!Utf8toWStr(gname, wgname))
            continue;
        wstrToLower(wgname);

        if (!(wGuildName.empty() || wgname.find(wGuildName) != std::wstring::npos))
            continue;

        std::string aname;
        if (AreaTableEntry const* areaEntry = GetAreaEntryByAreaID(zoneId))
            aname = areaEntry->m_AreaName[GetSessionDbcLocale()];

        bool s_show = true;
        for (uint32 i = 0; i < wordCount; ++i)
        {
            if (!wWords[i].empty())
            {
                if (wgname.find(wWords[i]) != std::wstring::npos ||
                    wpname.find(wWords[i]) != std::wstring::npos ||
                    Utf8FitTo(aname, wWords[i]))
                {
                    s_show = true;
                    break;
                }
                s_show = false;
            }
        }
        if (!s_show)
            continue;

        // 49 is maximum player count sent to client - can be overridden
        // through config, but is unstable
        if ((matchcount++) >= sWorld->getIntConfig(WorldIntConfigs::CONFIG_MAX_WHO))
            continue;

        ObjectGuid playerGuid = target->GetGUID();
        ObjectGuid accountId = target->GetSession()->GetAccountId();
        ObjectGuid guildGuid = target->GetGuild() ? target->GetGuild()->GetGUID() : 0;

        data.WriteBit(accountId[2]);
        data.WriteBit(playerGuid[2]);
        data.WriteBit(accountId[7]);
        data.WriteBit(guildGuid[5]);
        data.WriteBits(gname.size(), 7);
        data.WriteBit(accountId[1]);
        data.WriteBit(accountId[5]);
        data.WriteBit(guildGuid[7]);
        data.WriteBit(playerGuid[5]);
        data.WriteBit(false);
        data.WriteBit(guildGuid[1]);
        data.WriteBit(playerGuid[6]);
        data.WriteBit(guildGuid[2]);
        data.WriteBit(playerGuid[4]);
        data.WriteBit(guildGuid[0]);
        data.WriteBit(guildGuid[3]);
        data.WriteBit(accountId[6]);
        data.WriteBit(false);
        data.WriteBit(playerGuid[1]);
        data.WriteBit(guildGuid[4]);
        data.WriteBit(accountId[0]);

        if (DeclinedName const* names = itr->second->GetDeclinedNames())
        {
            for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
                data.WriteBits(names->name[i].size(), 14);
        }
        else
        {
            for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
                data.WriteBits(0, 7);
        }

        data.WriteBit(playerGuid[3]);
        data.WriteBit(guildGuid[6]);
        data.WriteBit(playerGuid[0]);
        data.WriteBit(accountId[4]);
        data.WriteBit(accountId[3]);
        data.WriteBit(playerGuid[7]);
        data.WriteBits(pname.size(), 6);

        bytesData.WriteByteSeq(playerGuid[1]);
        bytesData << uint32(target->GetSession()->GetVirtualRealmID());
        bytesData.WriteByteSeq(playerGuid[7]);
        bytesData << uint32(realmID); // NYI. guild creation virtual realm id.
        bytesData.WriteByteSeq(playerGuid[4]);
        bytesData.WriteString(pname);
        bytesData.WriteByteSeq(guildGuid[1]);
        bytesData.WriteByteSeq(playerGuid[0]);
        bytesData.WriteByteSeq(guildGuid[2]);
        bytesData.WriteByteSeq(guildGuid[0]);
        bytesData.WriteByteSeq(guildGuid[4]);
        bytesData.WriteByteSeq(playerGuid[3]);
        bytesData.WriteByteSeq(guildGuid[6]);
        bytesData << uint32(target->GetSession()->GetAccountId());
        bytesData.WriteString(gname);
        bytesData.WriteByteSeq(guildGuid[3]);
        bytesData.WriteByteSeq(accountId[4]);
        bytesData << uint8(class_);
        bytesData.WriteByteSeq(accountId[7]);
        bytesData.WriteByteSeq(playerGuid[6]);
        bytesData.WriteByteSeq(playerGuid[2]);

        if (DeclinedName const* names = itr->second->GetDeclinedNames())
            for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
                bytesData.WriteString(names->name[i]);

        bytesData.WriteByteSeq(accountId[2]);
        bytesData.WriteByteSeq(accountId[3]);
        bytesData << uint8(race);
        bytesData.WriteByteSeq(guildGuid[7]);
        bytesData.WriteByteSeq(accountId[1]);
        bytesData.WriteByteSeq(accountId[5]);
        bytesData.WriteByteSeq(accountId[6]);
        bytesData.WriteByteSeq(playerGuid[5]);
        bytesData.WriteByteSeq(accountId[0]);
        bytesData << uint8(gender);
        bytesData.WriteByteSeq(guildGuid[5]);
        bytesData << uint8(level);
        bytesData << int32(zoneId);

        ++displaycount;
    }

    data.FlushBits();
    data.PutBits(pos, displaycount, 6);
    data.append(bytesData);

    SendPacket(&data);

    delete[] words;
    delete[] wordLens;

    SF_LOG_DEBUG("network", "WORLD: Send SMSG_WHO Message");
}

void WorldSession::HandleLogoutRequestOpcode(WorldPacket& /*recvData*/)
{
    SF_LOG_DEBUG("network", "WORLD: Recvd CMSG_LOGOUT_REQUEST Message, security - %u", uint8(GetSecurity()));

    if (uint64 lguid = GetPlayer()->GetLootGUID())
        DoLootRelease(lguid);

    bool instantLogout = (GetPlayer()->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_RESTING) && !GetPlayer()->IsInCombat()) ||
        GetPlayer()->IsInFlight() || HasPermission(rbac::RBAC_PERM_INSTANT_LOGOUT);

    /// TODO: Possibly add RBAC permission to log out in combat
    bool canLogoutInCombat = GetPlayer()->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_RESTING);

    uint32 reason = 0;
    if (GetPlayer()->IsInCombat() && !canLogoutInCombat)
        reason = 1;
    else if (GetPlayer()->IsFalling())
        reason = 3;                                         // is jumping or falling
    else if (GetPlayer()->duel || GetPlayer()->HasAura(9454)) // is dueling or frozen by GM via freeze command
        reason = 2;                                         // FIXME - Need the correct value

    WorldPacket data(SMSG_LOGOUT_RESPONSE, 1 + 4);
    data << uint32(reason);
    data.WriteBit(instantLogout);
    data.FlushBits();
    SendPacket(&data);

    if (reason)
    {
        LogoutRequest(0);
        return;
    }

    // instant logout in taverns/cities or on taxi or for admins, gm's, mod's if its enabled in worldserver.conf
    if (instantLogout)
    {
        LogoutPlayer(true);
        return;
    }

    // not set flags if player can't free move to prevent lost state at logout cancel
    if (GetPlayer()->CanFreeMove())
    {
        GetPlayer()->SetStandState(UNIT_STAND_STATE_SIT);
        GetPlayer()->SetRooted(true);
        GetPlayer()->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
    }

    LogoutRequest(time(NULL));
}

void WorldSession::HandlePlayerLogoutOpcode(WorldPacket& /*recvData*/)
{
    SF_LOG_DEBUG("network", "WORLD: Recvd CMSG_PLAYER_LOGOUT Message");
}

void WorldSession::HandleLogoutCancelOpcode(WorldPacket& /*recvData*/)
{
    SF_LOG_DEBUG("network", "WORLD: Recvd CMSG_LOGOUT_CANCEL Message");

    // Player have already logged out serverside, too late to cancel
    if (!GetPlayer())
        return;

    LogoutRequest(0);

    WorldPacket data(SMSG_LOGOUT_CANCEL_ACK, 0);
    SendPacket(&data);

    // not remove flags if can't free move - its not set in Logout request code.
    if (GetPlayer()->CanFreeMove())
    {
        //!we can move again
        GetPlayer()->SetRooted(false);

        //! Stand Up
        GetPlayer()->SetStandState(UNIT_STAND_STATE_STAND);

        //! DISABLE_ROTATE
        GetPlayer()->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
    }

    SF_LOG_DEBUG("network", "WORLD: Sent SMSG_LOGOUT_CANCEL_ACK Message");
}

void WorldSession::HandleSetPvP(WorldPacket& recvData)
{
    SetPvPRequest request = ReadSetPvPRequest(recvData);
    if (request.hasStatus)
    {
        GetPlayer()->ApplyModFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_IN_PVP, request.newPvPStatus);
        GetPlayer()->ApplyModFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_PVP_TIMER, !request.newPvPStatus);
    }
}
void WorldSession::HandleTogglePvP(WorldPacket& /*recvData*/)
{
    GetPlayer()->ToggleFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_IN_PVP);
    GetPlayer()->ToggleFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_PVP_TIMER);

    if (GetPlayer()->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_IN_PVP))
    {
        if (!GetPlayer()->IsPvP() || GetPlayer()->pvpInfo.EndTimer)
            GetPlayer()->UpdatePvP(true, true);
    }
    else
    {
        if (!GetPlayer()->pvpInfo.IsHostile && GetPlayer()->IsPvP())
            GetPlayer()->pvpInfo.EndTimer = time(NULL);     // start toggle-off
    }

    //if (OutdoorPvP* pvp = _player->GetOutdoorPvP())
    //    pvp->HandlePlayerActivityChanged(_player);
}

void WorldSession::HandleZoneUpdateOpcode(WorldPacket& recvData)
{
    UInt32Request request = ReadUInt32Request(recvData);

    SF_LOG_DEBUG("network", "WORLD: Recvd ZONE_UPDATE: %u", request.value);

    // use server size data
    uint32 newzone, newarea;
    GetPlayer()->GetZoneAndAreaId(newzone, newarea);
    GetPlayer()->UpdateZone(newzone, newarea);
    //GetPlayer()->SendInitWorldStates(true, newZone);
}

void WorldSession::HandleReturnToGraveyard(WorldPacket& /*recvPacket*/)
{
    if (GetPlayer()->IsAlive() || !GetPlayer()->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_GHOST))
        return;

    GetPlayer()->RepopAtGraveyard();
}

void WorldSession::HandleRequestCemeteryList(WorldPacket& /*recvPacket*/)
{
    uint32 zoneId = _player->GetZoneId();
    uint32 team = _player->GetTeam();

    std::vector<uint32> GraveyardIds;

    GraveYardContainer::const_iterator it;
    std::pair<GraveYardContainer::const_iterator, GraveYardContainer::const_iterator> range;
    range = sObjectMgr->GraveYardStore.equal_range(zoneId);

    for (it = range.first; it != range.second && GraveyardIds.size() < 16; ++it) // client max
    {
        if (it->second.team == 0 || it->second.team == team)
            GraveyardIds.push_back(it->first);
    }

    if (GraveyardIds.empty())
    {
        SF_LOG_DEBUG("network", "No graveyards found for zone %u for %u (team %u) in CMSG_REQUEST_CEMETERY_LIST", zoneId, m_GUIDLow, team);
        return;
    }

    bool IsGossipTriggered = false;
    WorldPacket data(SMSG_REQUEST_CEMETERY_LIST_RESPONSE, 4 + 4 * GraveyardIds.size());

    data.WriteBits(GraveyardIds.size(), 22);
    data << IsGossipTriggered;

    for (uint32 i = 0; i < GraveyardIds.size(); ++i)
        data << uint32(GraveyardIds[i]);

    SendPacket(&data);
}

void WorldSession::HandleSetSelectionOpcode(WorldPacket& recvData)
{
    GuidRequest request = ReadSetSelectionRequest(recvData);
    _player->SetSelection(request.guid);
}

void WorldSession::HandleStandStateChangeOpcode(WorldPacket& recvData)
{
    // SF_LOG_DEBUG("network", "WORLD: Received CMSG_STAND_STATE_CHANGE"); -- too many spam in log at lags/debug stop
    UInt32Request request = ReadUInt32Request(recvData);

    _player->SetStandState(request.value);
}

void WorldSession::HandleContactListOpcode(WorldPacket& recvData)
{
    ReadContactListRequest(recvData);
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_CONTACT_LIST");
    _player->GetSocial()->SendSocialList(_player);
}

void WorldSession::HandleAddFriendOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_ADD_FRIEND");

    AddFriendRequest request = ReadAddFriendRequest(recvData);

    request.name = request.name.substr(0, request.name.find("-"));
    if (!normalizePlayerName(request.name))
        return;

    SF_LOG_DEBUG("network", "WORLD: %s asked to add friend : '%s'",
        GetPlayer()->GetName().c_str(), request.name.c_str());

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUID_RACE_ACC_BY_NAME);

    stmt->setString(0, request.name);

    _addFriendCallback.SetParam(request.note);
    _addFriendCallback.SetFutureResult(CharacterDatabase.AsyncQuery(stmt));
}

void WorldSession::HandleAddFriendOpcodeCallBack(PreparedQueryResult result, std::string const& friendNote)
{
    if (!GetPlayer())
        return;

    uint64 friendGuid;
    uint32 friendAccountId;
    uint32 team;
    FriendsResult friendResult;

    friendResult = FRIEND_NOT_FOUND;
    friendGuid = 0;

    if (result)
    {
        Field* fields = result->Fetch();

        friendGuid = MAKE_NEW_GUID(fields[0].GetUInt32(), 0, HIGHGUID_PLAYER);
        team = Player::TeamForRace(fields[1].GetUInt8());
        friendAccountId = fields[2].GetUInt32();

        if (HasPermission(rbac::RBAC_PERM_ALLOW_GM_FRIEND) || AccountMgr::IsPlayerAccount(AccountMgr::GetSecurity(friendAccountId, GetVirtualRealmID())))
        {
            if (friendGuid)
            {
                if (friendGuid == GetPlayer()->GetGUID())
                    friendResult = FRIEND_SELF;
                else if (GetPlayer()->GetTeam() != team && !HasPermission(rbac::RBAC_PERM_TWO_SIDE_ADD_FRIEND))
                    friendResult = FRIEND_ENEMY;
                else if (GetPlayer()->GetSocial()->HasFriend(GUID_LOPART(friendGuid)))
                    friendResult = FRIEND_ALREADY;
                else
                {
                    Player* pFriend = ObjectAccessor::FindPlayer(friendGuid);
                    if (pFriend && pFriend->IsInWorld() && pFriend->IsVisibleGloballyFor(GetPlayer()))
                        friendResult = FRIEND_ADDED_ONLINE;
                    else
                        friendResult = FRIEND_ADDED_OFFLINE;
                    if (!GetPlayer()->GetSocial()->AddToSocialList(GUID_LOPART(friendGuid), false))
                    {
                        friendResult = FRIEND_LIST_FULL;
                        SF_LOG_DEBUG("network", "WORLD: %s's friend list is full.", GetPlayer()->GetName().c_str());
                    }
                }
                GetPlayer()->GetSocial()->SetFriendNote(GUID_LOPART(friendGuid), friendNote);
            }
        }
    }

    sSocialMgr->SendFriendStatus(GetPlayer(), friendResult, GUID_LOPART(friendGuid), false);

    SF_LOG_DEBUG("network", "WORLD: Sent (SMSG_FRIEND_STATUS)");
}

void WorldSession::HandleDelFriendOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_DEL_FRIEND");

    UInt64Request request = ReadUInt64Request(recvData);

    _player->GetSocial()->RemoveFromSocialList(GUID_LOPART(request.value), false);

    sSocialMgr->SendFriendStatus(GetPlayer(), FRIEND_REMOVED, GUID_LOPART(request.value), false);

    SF_LOG_DEBUG("network", "WORLD: Sent motd (SMSG_FRIEND_STATUS)");
}

void WorldSession::HandleAddIgnoreOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_ADD_IGNORE");

    StringRequest request = ReadAddIgnoreRequest(recvData);

    if (!normalizePlayerName(request.value))
        return;

    SF_LOG_DEBUG("network", "WORLD: %s asked to Ignore: '%s'",
        GetPlayer()->GetName().c_str(), request.value.c_str());

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUID_BY_NAME);

    stmt->setString(0, request.value);

    _addIgnoreCallback = CharacterDatabase.AsyncQuery(stmt);
}

void WorldSession::HandleAddIgnoreOpcodeCallBack(PreparedQueryResult result)
{
    if (!GetPlayer())
        return;

    uint64 IgnoreGuid;
    FriendsResult ignoreResult;

    ignoreResult = FRIEND_IGNORE_NOT_FOUND;
    IgnoreGuid = 0;

    if (result)
    {
        IgnoreGuid = MAKE_NEW_GUID((*result)[0].GetUInt32(), 0, HIGHGUID_PLAYER);

        if (IgnoreGuid)
        {
            if (IgnoreGuid == GetPlayer()->GetGUID())              //not add yourself
                ignoreResult = FRIEND_IGNORE_SELF;
            else if (GetPlayer()->GetSocial()->HasIgnore(GUID_LOPART(IgnoreGuid)))
                ignoreResult = FRIEND_IGNORE_ALREADY;
            else
            {
                ignoreResult = FRIEND_IGNORE_ADDED;

                // ignore list full
                if (!GetPlayer()->GetSocial()->AddToSocialList(GUID_LOPART(IgnoreGuid), true))
                    ignoreResult = FRIEND_IGNORE_FULL;
            }
        }
    }

    sSocialMgr->SendFriendStatus(GetPlayer(), ignoreResult, GUID_LOPART(IgnoreGuid), false);

    SF_LOG_DEBUG("network", "WORLD: Sent (SMSG_FRIEND_STATUS)");
}

void WorldSession::HandleDelIgnoreOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_DEL_IGNORE");

    UInt64Request request = ReadUInt64Request(recvData);

    _player->GetSocial()->RemoveFromSocialList(GUID_LOPART(request.value), true);

    sSocialMgr->SendFriendStatus(GetPlayer(), FRIEND_IGNORE_REMOVED, GUID_LOPART(request.value), false);

    SF_LOG_DEBUG("network", "WORLD: Sent motd (SMSG_FRIEND_STATUS)");
}

void WorldSession::HandleSetContactNotesOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "CMSG_SET_CONTACT_NOTES");
    ContactNotesRequest request = ReadSetContactNotesRequest(recvData);
    _player->GetSocial()->SetFriendNote(GUID_LOPART(request.guid), request.note);
}

void WorldSession::HandleBugOpcode(WorldPacket& recvData)
{
    BugReportRequest request = ReadBugReportRequest(recvData);

    if (request.suggestion == 0)
        SF_LOG_DEBUG("network", "WORLD: Received CMSG_BUG [Bug Report]");
    else
        SF_LOG_DEBUG("network", "WORLD: Received CMSG_BUG [Suggestion]");

    SF_LOG_DEBUG("network", "%s", request.type.c_str());
    SF_LOG_DEBUG("network", "%s", request.content.c_str());

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_BUG_REPORT);

    stmt->setString(0, request.type);
    stmt->setString(1, request.content);

    CharacterDatabase.Execute(stmt);
}

void WorldSession::HandleReclaimCorpseOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_RECLAIM_CORPSE");

    ReadReclaimCorpseRequest(recvData);

    if (GetPlayer()->IsAlive())
        return;

    // do not allow corpse reclaim in arena
    if (GetPlayer()->InArena())
        return;

    // body not released yet
    if (!GetPlayer()->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_GHOST))
        return;

    Corpse* corpse = GetPlayer()->GetCorpse();

    if (!corpse)
        return;

    // prevent resurrect before 30-sec delay after body release not finished
    if (time_t(corpse->GetGhostTime() + GetPlayer()->GetCorpseReclaimDelay(corpse->GetType() == CorpseType::CORPSE_RESURRECTABLE_PVP)) > time_t(time(NULL)))
        return;

    if (!corpse->IsWithinDistInMap(GetPlayer(), CORPSE_RECLAIM_RADIUS, true))
        return;

    // resurrect
    GetPlayer()->ResurrectPlayer(GetPlayer()->InBattleground() ? 1.0f : 0.5f);

    // spawn bones
    GetPlayer()->SpawnCorpseBones();
}

void WorldSession::HandleResurrectResponseOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_RESURRECT_RESPONSE");

    ResurrectResponseRequest request = ReadResurrectResponseRequest(recvData);

    if (GetPlayer()->IsAlive())
        return;

    if (request.status == 1)
    {
        GetPlayer()->ClearResurrectRequestData();           // reject
        return;
    }

    if (!GetPlayer()->IsRessurectRequestedBy(request.guid))
        return;

    GetPlayer()->ResurectUsingRequestData();
}

void WorldSession::HandleAreaTriggerOpcode(WorldPacket& recvData)
{
    AreaTriggerRequest request = ReadAreaTriggerRequest(recvData);

    SF_LOG_DEBUG("network", "CMSG_AREATRIGGER. Trigger ID: %u, Unk1: %u, Unk2: %u", request.triggerId, request.unk1, request.unk2);

    Player* player = GetPlayer();
    if (player->IsInFlight())
    {
        SF_LOG_DEBUG("network", "HandleAreaTriggerOpcode: Player '%s' (GUID: %u) in flight, ignore Area Trigger ID:%u, unk1: %u, unk2: %u",
            player->GetName().c_str(), player->GetGUIDLow(), request.triggerId, request.unk1, request.unk2);
        return;
    }

    AreaTriggerEntry const* atEntry = sAreaTriggerStore.LookupEntry(request.triggerId);
    if (!atEntry)
    {
        SF_LOG_DEBUG("network", "HandleAreaTriggerOpcode: Player '%s' (GUID: %u) send unknown (by DBC) Area Trigger ID:%u, unk1: %u, unk2: %u",
            player->GetName().c_str(), player->GetGUIDLow(), request.triggerId, request.unk1, request.unk2);
        return;
    }

    if (player->GetMapId() != atEntry->mapid)
    {
        SF_LOG_DEBUG("network", "HandleAreaTriggerOpcode: Player '%s' (GUID: %u) too far (trigger map: %u player map: %u), ignore Area Trigger ID: %u, unk1: %u, unk2: %u",
            player->GetName().c_str(), atEntry->mapid, player->GetMapId(), player->GetGUIDLow(), request.triggerId, request.unk1, request.unk2);
        return;
    }

    // delta is safe radius
    const float delta = 5.0f;

    if (atEntry->radius > 0)
    {
        // if we have radius check it
        float dist = player->GetDistance(atEntry->x, atEntry->y, atEntry->z);
        if (dist > atEntry->radius + delta)
        {
            SF_LOG_DEBUG("network", "HandleAreaTriggerOpcode: Player '%s' (GUID: %u) too far (radius: %f distance: %f), ignore Area Trigger ID: %u, unk1: %u, unk2: %u",
                player->GetName().c_str(), player->GetGUIDLow(), atEntry->radius, dist, request.triggerId, request.unk1, request.unk2);
            return;
        }
    }
    else
    {
        // we have only extent

        // rotate the players position instead of rotating the whole cube, that way we can make a simplified
        // is-in-cube check and we have to calculate only one point instead of 4

        // 2PI = 360°, keep in mind that ingame orientation is counter-clockwise
        double rotation = 2 * M_PI - atEntry->box_orientation;
        double sinVal = std::sin(rotation);
        double cosVal = std::cos(rotation);

        float playerBoxDistX = player->GetPositionX() - atEntry->x;
        float playerBoxDistY = player->GetPositionY() - atEntry->y;

        float rotPlayerX = float(atEntry->x + playerBoxDistX * cosVal - playerBoxDistY * sinVal);
        float rotPlayerY = float(atEntry->y + playerBoxDistY * cosVal + playerBoxDistX * sinVal);

        // box edges are parallel to coordiante axis, so we can treat every dimension independently :D
        float dz = player->GetPositionZ() - atEntry->z;
        float dx = rotPlayerX - atEntry->x;
        float dy = rotPlayerY - atEntry->y;
        if ((fabs(dx) > atEntry->box_x / 2 + delta) ||
            (fabs(dy) > atEntry->box_y / 2 + delta) ||
            (fabs(dz) > atEntry->box_z / 2 + delta))
        {
            SF_LOG_DEBUG("network", "HandleAreaTriggerOpcode: Player '%s' (GUID: %u) too far (1/2 box X: %f 1/2 box Y: %f 1/2 box Z: %f rotatedPlayerX: %f rotatedPlayerY: %f dZ:%f), ignore Area Trigger ID: %u, unk1: %u, unk2: %u",
                player->GetName().c_str(), player->GetGUIDLow(), atEntry->box_x / 2, atEntry->box_y / 2, atEntry->box_z / 2, rotPlayerX, rotPlayerY, dz, request.triggerId, request.unk1, request.unk2);
            return;
        }
    }

    // unk2 is enter/leave (set on enter). Debug + exploration credit only on enter,
    // otherwise .debug areatriggers prints twice (enter and leave).
    if (request.unk2 && player->isDebugAreaTriggers)
        ChatHandler(player->GetSession()).PSendSysMessage(LANG_DEBUG_AREATRIGGER_REACHED, request.triggerId);

    if (sScriptMgr->OnAreaTrigger(player, atEntry))
        return;

    if (request.unk2 && player->IsAlive())
        if (uint32 questId = sObjectMgr->GetQuestForAreaTrigger(request.triggerId))
            if (player->GetQuestStatus(questId) == QUEST_STATUS_INCOMPLETE)
                player->AreaExploredOrEventHappens(questId);

    if (sObjectMgr->IsTavernAreaTrigger(request.triggerId))
    {
        TavernRestArea const* restArea = sObjectMgr->GetTavernRestArea(request.triggerId);

        // The first bit is set in known 5.4.8 captures; the second bit carries the
        // enter/leave state for the trigger.
        if (!request.unk2)
        {
            if (player->GetRestType() == REST_TYPE_IN_TAVERN)
            {
                player->RemoveFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_RESTING);
                player->SetRestType(REST_TYPE_NO);
            }

            return;
        }

        if (restArea && !sObjectMgr->IsInTavernRestArea(*restArea, player->GetMapId(), player->GetPositionX(), player->GetPositionY(), player->GetPositionZ()))
            return;

        // set resting flag we are in the inn
        player->SetFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_RESTING);

        if (restArea)
            player->InnEnter(time(NULL), restArea->MapId, restArea->X, restArea->Y, restArea->Z,
                restArea->Radius, restArea->BoxX, restArea->BoxY, restArea->BoxZ, restArea->BoxOrientation);
        else
            player->InnEnter(time(NULL), atEntry->mapid, atEntry->x, atEntry->y, atEntry->z);

        player->SetRestType(REST_TYPE_IN_TAVERN);

        if (sWorld->IsFFAPvPRealm())
            player->RemoveByteFlag(UNIT_FIELD_SHAPESHIFT_FORM, 1, UNIT_BYTE2_FLAG_FFA_PVP);

        return;
    }

    if (Battleground* bg = player->GetBattleground())
        if (bg->GetStatus() == STATUS_IN_PROGRESS)
        {
            bg->HandleAreaTrigger(player, request.triggerId);
            return;
        }

    if (OutdoorPvP* pvp = player->GetOutdoorPvP())
        if (pvp->HandleAreaTrigger(_player, request.triggerId))
            return;

    AreaTriggerStruct const* at = sObjectMgr->GetAreaTrigger(request.triggerId);
    if (!at)
        return;

    bool teleported = false;
    if (player->GetMapId() != at->target_mapId)
    {
        if (!sMapMgr->CanPlayerEnter(at->target_mapId, player, false))
            return;

        if (Group* group = player->GetGroup())
            if (group->isLFGGroup() && player->GetMap()->IsInstance())
                teleported = player->TeleportToBGEntryPoint();
    }

    if (!teleported)
        player->TeleportTo(at->target_mapId, at->target_X, at->target_Y, at->target_Z, at->target_Orientation, TELE_TO_NOT_LEAVE_TRANSPORT);
}

void WorldSession::HandleUpdateAccountData(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_UPDATE_ACCOUNT_DATA");

    uint32 timestamp = 0, decompressedSize = 0, compCount = 0;
    uint8 type = 0;
    recvData >> decompressedSize >> timestamp >> compCount;

    std::size_t const typePosition = recvData.rpos() + compCount;
    if (typePosition >= recvData.size())
    {
        recvData.rfinish();
        SF_LOG_DEBUG("network", "UAD: Account data packet missing type bits");
        return;
    }

    type = recvData[typePosition] >> 5;
    AccountDataType UADType = AccountDataType(type);
    if (UADType >= AccountDataType::NUM_ACCOUNT_DATA_TYPES)
    {
        recvData.rfinish();
        SF_LOG_DEBUG("network", "UAD: Unknown account data type: %u", type);
        return;
    }

    if (decompressedSize == 0)                               // erase
    {
        SetAccountData(UADType, 0, "");

        WorldPacket data(SMSG_UPDATE_ACCOUNT_DATA_COMPLETE, 4 + 4);
        data << uint32(type);
        data << uint32(0);
        SendPacket(&data);

        return;
    }

    if (decompressedSize > 0xFFFF)
    {
        recvData.rfinish();                   // unneeded warning spam in this case
        SF_LOG_DEBUG("network", "UAD: Account data packet too big, size %u", decompressedSize);
        return;
    }

    ByteBuffer dest;
    dest.resize(decompressedSize);

    uLongf realSize = decompressedSize;
    if (uncompress(const_cast<uint8*>(dest.contents()), &realSize, const_cast<uint8*>(recvData.contents() + recvData.rpos()), compCount) != Z_OK)
    {
        recvData.rfinish();                   // unneeded warning spam in this case
        SF_LOG_DEBUG("network", "UAD: Failed to decompress account data");
        return;
    }

    recvData.rpos(recvData.rpos() + compCount);

    recvData.ReadBits(3);

    std::string adata;
    dest >> adata;

    SetAccountData(UADType, timestamp, adata);

    WorldPacket data(SMSG_UPDATE_ACCOUNT_DATA_COMPLETE, 4 + 4);
    data << uint32(UADType);
    data << uint32(0);
    SendPacket(&data);
}

void WorldSession::HandleRequestAccountData(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_REQUEST_ACCOUNT_DATA");

    UInt32Request request = ReadRequestAccountDataRequest(recvData);

    SF_LOG_DEBUG("network", "RAD: type %u", request.value);

    AccountDataType RADType = AccountDataType(request.value);
    if (RADType >= AccountDataType::NUM_ACCOUNT_DATA_TYPES)
    {
        SF_LOG_DEBUG("network", "RAD: Unknown account data type: %u", request.value);
        return;
    }

    AccountData* adata = GetAccountData(RADType);
    if (RADType == AccountDataType::PER_CHARACTER_CHAT_CACHE && Skyfire::AccountData::HasEmptyCharacterChatChannels(adata->Data))
    {
        SF_LOG_DEBUG("network", "RAD: Clearing empty per-character chat cache for account %u player %u", GetAccountId(), GetGuidLow());
        SetAccountData(RADType, 0, "");
        adata = GetAccountData(RADType);
    }

    uint32 size = adata->Data.size();

    uLongf destSize = size ? compressBound(size) : 0;

    ByteBuffer dest;
    dest.resize(destSize);

    if (size && compress(dest.contents(), &destSize, (uint8 const*)adata->Data.c_str(), size) != Z_OK)
    {
        SF_LOG_DEBUG("network", "RAD: Failed to compress account data");
        return;
    }

    dest.resize(destSize);

    WorldPacket data(SMSG_UPDATE_ACCOUNT_DATA, 8 + 4 + 4 + 4 + destSize);

    ObjectGuid guid = GetPlayer() ? GetPlayer()->GetGUID() : 0;

    data.WriteBits(request.value, 3); // type (0-7)
    data.WriteGuidMask(guid, 5, 1, 3, 7, 0, 4, 2, 6);

    data.WriteGuidBytes(guid, 3, 1, 5);
    data << uint32(size);         // decompressed length
    data << uint32(destSize);
    data.append(dest);
    data.WriteGuidBytes(guid, 7, 4, 0, 6, 2);
    data << uint32(adata->Time); // unix time

    SendPacket(&data);
}

int32 WorldSession::HandleEnableNagleAlgorithm()
{
    // Instructs the server we wish to receive few amounts of large packets (SMSG_MULTIPLE_PACKETS?)
    // instead of large amount of small packets
    return 0;
}

void WorldSession::HandleSetActionButtonOpcode(WorldPacket& recvData)
{
    ActionButtonRequest request = ReadSetActionButtonRequest(recvData);

    ActionButtonPACKET* button = reinterpret_cast<ActionButtonPACKET*>(&request.buttonStream);
    button->id = ACTION_BUTTON_ACTION(request.buttonStream);
    button->unk = ACTION_BUTTON_TYPE(request.buttonStream);

    SF_LOG_DEBUG("network", "CMSG_SET_ACTION_BUTTON slotId: %u actionId: %u buttontype: %u", request.slotId, button->id, button->unk);

    if (!button->id)
        GetPlayer()->removeActionButton(request.slotId);
    else
        GetPlayer()->addActionButton(request.slotId, button->id, ActionButtonType(button->unk));
}

void WorldSession::HandleCompleteCinematic(WorldPacket& /*recvData*/)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_COMPLETE_CINEMATIC");
    // If player has sight bound to visual waypoint NPC we should remove it
    GetPlayer()->StopCinematic();
}

void WorldSession::HandleNextCinematicCamera(WorldPacket& /*recvData*/)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_NEXT_CINEMATIC_CAMERA");
}

void WorldSession::HandleMoveTimeSkippedOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_MOVE_TIME_SKIPPED");

    ReadMoveTimeSkippedRequest(recvData);

    //TODO!

    /*
        GetPlayer()->ModifyLastMoveTime(-int32(time_skipped));
    */
}

void WorldSession::HandleFeatherFallAck(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_MOVE_FEATHER_FALL_ACK");

    // no used
    recvData.rfinish();                       // prevent warnings spam
}

void WorldSession::HandleMoveUnRootAck(WorldPacket& recvData)
{
    // no used
    recvData.rfinish();                       // prevent warnings spam
    /*
        uint64 guid;
        recvData >> guid;

        // now can skip not our packet
        if (_player->GetGUID() != guid)
        {
            recvData.rfinish();                   // prevent warnings spam
            return;
        }

        SF_LOG_DEBUG("network", "WORLD: CMSG_FORCE_MOVE_UNROOT_ACK");

        recvData.read_skip<uint32>();                          // unk

        MovementInfo movementInfo;
        movementInfo.guid = guid;
        ReadMovementInfo(recvData, &movementInfo);
        recvData.read_skip<float>();                           // unk2
    */
}

void WorldSession::HandleMoveRootAck(WorldPacket& recvData)
{
    // no used
    recvData.rfinish();                       // prevent warnings spam
    /*
        uint64 guid;
        recvData >> guid;

        // now can skip not our packet
        if (_player->GetGUID() != guid)
        {
            recvData.rfinish();                   // prevent warnings spam
            return;
        }

        SF_LOG_DEBUG("network", "WORLD: CMSG_FORCE_MOVE_ROOT_ACK");

        recvData.read_skip<uint32>();                          // unk

        MovementInfo movementInfo;
        ReadMovementInfo(recvData, &movementInfo);
    */
}

void WorldSession::HandleMoveGravityAck(WorldPacket& recvData)
{
    // not used
    recvData.rfinish();                       // prevent warnings spam
}

void WorldSession::HandleSetActionBarToggles(WorldPacket& recvData)
{
    UInt8Request request = ReadUInt8Request(recvData);

    if (!GetPlayer())                                        // ignore until not logged (check needed because STATUS_AUTHED)
    {
        if (request.value != 0)
            SF_LOG_ERROR("network", "WorldSession::HandleSetActionBarToggles in not logged state with value: %u, ignored", uint32(request.value));
        return;
    }

    GetPlayer()->SetByteValue(PLAYER_FIELD_LIFETIME_MAX_RANK, 2, request.value);
}

void WorldSession::HandlePlayedTime(WorldPacket& recvData)
{
    UInt8Request request = ReadUInt8Request(recvData);     // 0 or 1 expected

    WorldPacket data(SMSG_PLAYED_TIME, 4 + 4 + 1);
    data << uint32(_player->GetTotalPlayedTime());
    data << uint32(_player->GetLevelPlayedTime());
    data << uint8(request.value);                           // 0 - will not show in chat frame
    SendPacket(&data);
}

void WorldSession::HandleInspectOpcode(WorldPacket& recvData)
{
    GuidRequest request = ReadInspectRequest(recvData);

    SF_LOG_DEBUG("network", "WORLD: Received CMSG_INSPECT");

    Player* player = ObjectAccessor::FindPlayer(request.guid);
    if (!player)
    {
        SF_LOG_DEBUG("network", "CMSG_INSPECT: No player found from GUID: " UI64FMTD, (uint64)request.guid);
        return;
    }

    _player->SendInspectResult(player);
}

void WorldSession::HandleInspectHonorStatsOpcode(WorldPacket& recvData)
{
    GuidRequest request = ReadInspectHonorStatsRequest(recvData);

    Player* player = ObjectAccessor::FindPlayer(request.guid);

    if (!player)
    {
        SF_LOG_DEBUG("network", "CMSG_INSPECT_HONOR_STATS: No player found from GUID: " UI64FMTD, (uint64)request.guid);
        return;
    }

    ObjectGuid playerGuid = player->GetGUID();
    WorldPacket data(SMSG_INSPECT_HONOR_STATS, 8 + 1 + 4 + 4);

    data << uint32(player->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS));
    data << uint16(player->GetUInt16Value(PLAYER_FIELD_YESTERDAY_HONORABLE_KILLS, 1));  // yesterday kills
    data << uint16(player->GetUInt16Value(PLAYER_FIELD_YESTERDAY_HONORABLE_KILLS, 0));  // today kills
    data << uint8(0);                                               // rank

    data.WriteGuidMask(playerGuid, 2, 1, 6, 4, 5, 3, 7, 0);
    data.WriteGuidBytes(playerGuid, 1, 3, 6, 7, 2, 4, 5, 0);

    SendPacket(&data);
}

void WorldSession::HandleWorldTeleportOpcode(WorldPacket& recvData)
{
    WorldTeleportRequest request = ReadWorldTeleportRequest(recvData);

    SF_LOG_DEBUG("network", "WORLD: Received CMSG_WORLD_TELEPORT");

    if (GetPlayer()->IsInFlight())
    {
        SF_LOG_DEBUG("network", "Player '%s' (GUID: %u) in flight, ignore worldport command.",
            GetPlayer()->GetName().c_str(), GetPlayer()->GetGUIDLow());
        return;
    }

    SF_LOG_DEBUG("network", "CMSG_WORLD_TELEPORT: Player = %s, Time = %u, map = %u, x = %f, y = %f, z = %f, o = %f",
        GetPlayer()->GetName().c_str(), request.time, request.mapId, request.positionX, request.positionY, request.positionZ, request.orientation);

    if (HasPermission(rbac::RBAC_PERM_OPCODE_WORLD_TELEPORT))
        GetPlayer()->TeleportTo(request.mapId, request.positionX, request.positionY, request.positionZ, request.orientation);
    else
        SendNotification(LANG_YOU_NOT_HAVE_PERMISSION);
}

void WorldSession::HandleWhoisOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "Received opcode CMSG_WHOIS");
    StringRequest request = ReadStringRequest(recvData);

    if (!HasPermission(rbac::RBAC_PERM_OPCODE_WHOIS))
    {
        SendNotification(LANG_YOU_NOT_HAVE_PERMISSION);
        return;
    }

    if (request.value.empty() || !normalizePlayerName(request.value))
    {
        SendNotification(LANG_NEED_CHARACTER_NAME);
        return;
    }

    Player* player = sObjectAccessor->FindPlayerByName(request.value);

    if (!player)
    {
        SendNotification(LANG_PLAYER_NOT_EXIST_OR_OFFLINE, request.value.c_str());
        return;
    }

    uint32 accid = player->GetSession()->GetAccountId();

    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_WHOIS);

    stmt->setUInt32(0, accid);

    PreparedQueryResult result = LoginDatabase.Query(stmt);

    if (!result)
    {
        SendNotification(LANG_ACCOUNT_FOR_PLAYER_NOT_FOUND, request.value.c_str());
        return;
    }

    Field* fields = result->Fetch();
    std::string acc = fields[0].GetString();
    if (acc.empty())
        acc = "Unknown";
    std::string email = fields[1].GetString();
    if (email.empty())
        email = "Unknown";
    std::string lastip = fields[2].GetString();
    if (lastip.empty())
        lastip = "Unknown";

    std::string msg = request.value + "'s " + "account is " + acc + ", e-mail: " + email + ", last ip: " + lastip;

    WorldPacket data(SMSG_WHOIS, msg.size() + 1);
    data.WriteBits(msg.size(), 11);
    data.FlushBits();
    if (msg.size())
        data.WriteString(msg);
    SendPacket(&data);

    SF_LOG_DEBUG("network", "Received whois command from player %s for character %s",
        GetPlayer()->GetName().c_str(), request.value.c_str());
}

void WorldSession::HandleComplainOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_COMPLAIN");

    ComplainRequest request = ReadComplainRequest(recvData);

    // NOTE: all chat messages from this spammer automatically ignored by spam reporter until logout in case chat spam.
    // if it's mail spam - ALL mails from this spammer automatically removed by client

    // Complaint Received message
    WorldPacket data(SMSG_COMPLAIN_RESULT, 2);
    data << uint8(0); // value 1 resets CGChat::m_complaintsSystemStatus in client. (unused?)
    data << uint8(0); // value 0xC generates a "CalendarError" in client.
    SendPacket(&data);

    SF_LOG_DEBUG("network", "REPORT SPAM: type %u, guid %u, unk1 %u, unk2 %u, unk3 %u, unk4 %u, message %s", request.spamType, GUID_LOPART(request.spammerGuid), request.unk1, request.unk2, request.unk3, request.unk4, request.description.c_str());
}

void WorldSession::HandleRealmSplitOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "CMSG_REALM_SPLIT");

    UInt32Request request = ReadUInt32Request(recvData);
    std::string const splitDate = "01/01/01";
    uint32 const splitState = 0; // 0 normal, 1 split, 2 split pending

    WorldPacket data(SMSG_REALM_SPLIT, 4 + 4 + splitDate.size());
    data.WriteBits(splitDate.size(), 7);
    data << splitState;
    data << request.value;
    data.WriteString(splitDate);
    SendPacket(&data);
    //SF_LOG_DEBUG("response sent %u", unk);
}

void WorldSession::HandleFarSightOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_FAR_SIGHT");

    BoolRequest request = ReadFarSightRequest(recvData);
    if (request.value)
    {
        SF_LOG_DEBUG("network", "Added FarSight " UI64FMTD " to player %u", _player->GetUInt64Value(PLAYER_FIELD_FARSIGHT_OBJECT), _player->GetGUIDLow());
        if (WorldObject* target = _player->GetViewpoint())
            _player->SetSeer(target);
        else
            SF_LOG_ERROR("network", "Player %s (GUID: %u) requests non-existing seer " UI64FMTD, _player->GetName().c_str(), GUID_LOPART(_player->GetGUID()), _player->GetUInt64Value(PLAYER_FIELD_FARSIGHT_OBJECT));
    }
    else
    {
        SF_LOG_DEBUG("network", "Player %u set vision to self", _player->GetGUIDLow());
        _player->SetSeer(_player);
    }

    GetPlayer()->UpdateVisibilityForPlayer();
}

void WorldSession::HandleSetTitleOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "CMSG_SET_TITLE");

    Int32Request request = ReadInt32Request(recvData);
    int32 title = request.value;

    // -1 at none
    if (title > 0 && title < MAX_TITLE_INDEX)
    {
        if (!GetPlayer()->HasTitle(title))
            return;
    }
    else
        title = 0;

    GetPlayer()->SetUInt32Value(PLAYER_FIELD_PLAYER_TITLE, title);
}

void WorldSession::SendTitleEarned(uint32 TitleIndex)
{
    WorldPacket data(SMSG_TITLE_EARNED, 4);
    data << uint32(TitleIndex);
    SendPacket(&data);
}

void WorldSession::SendTitleLost(uint32 TitleIndex)
{
    WorldPacket data(SMSG_TITLE_LOST, 4);
    data << uint32(TitleIndex);
    SendPacket(&data);
}

void WorldSession::HandleTimeSyncResp(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "CMSG_TIME_SYNC_RESPONSE");

    TimeSyncRequest request = ReadTimeSyncRequest(recvData);

    while (!_player->m_timeSyncQueue.empty() && _player->m_timeSyncQueue.front() < request.counter)
    {
        SF_LOG_DEBUG("network", "Discarding stale time sync counter %u for player %s before response %u",
            _player->m_timeSyncQueue.front(), _player->GetName().c_str(), request.counter);
        _player->m_timeSyncQueue.pop();
    }

    if (_player->m_timeSyncQueue.empty())
    {
        SF_LOG_DEBUG("network", "Ignoring time sync response %u from player %s with no pending request",
            request.counter, _player->GetName().c_str());
        return;
    }

    if (request.counter != _player->m_timeSyncQueue.front())
    {
        SF_LOG_DEBUG("network", "Ignoring out of order time sync response %u from player %s, expected %u",
            request.counter, _player->GetName().c_str(), _player->m_timeSyncQueue.front());
        return;
    }

    SF_LOG_DEBUG("network", "Time sync received: counter %u, client ticks %u, time since last sync %u", request.counter, request.clientTicks, request.clientTicks - _player->m_timeSyncClient);

    uint32 ourTicks = request.clientTicks + (getMSTime() - _player->m_timeSyncServer);

    // diff should be small
    SF_LOG_DEBUG("network", "Our ticks: %u, diff %u, latency %u", ourTicks, ourTicks - request.clientTicks, GetLatency());

    _player->m_timeSyncClient = request.clientTicks;
    _player->m_timeSyncQueue.pop();
}

void WorldSession::HandleResetInstancesOpcode(WorldPacket& /*recvData*/)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_RESET_INSTANCES");

    if (Group* group = _player->GetGroup())
    {
        if (group->IsLeader(_player->GetGUID()))
            group->ResetInstances(InstanceResetMethod::INSTANCE_RESET_ALL, false, _player);
    }
    else
        _player->ResetInstances(InstanceResetMethod::INSTANCE_RESET_ALL, false);
}

void WorldSession::HandleSetDungeonDifficultyOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "Received: CMSG_SET_DUNGEON_DIFFICULTY");

    UInt32Request request = ReadUInt32Request(recvData);

    DifficultyEntry const* difficultyEntry = sDifficultyStore.LookupEntry(request.value);
    if (!difficultyEntry)
    {
        SF_LOG_DEBUG("network", "%d sent an invalid instance mode %u!",
            _player->GetGUIDLow(), request.value);
        return;
    }

    if (difficultyEntry->maptype != MAP_DUNGEON)
    {
        SF_LOG_DEBUG("network", "%d sent an non-dungeon instance mode %d!",
            _player->GetGUIDLow(), difficultyEntry->DiffID);
        return;
    }

    DifficultyID difficulty = DifficultyID(difficultyEntry->DiffID);
    if (difficulty == _player->GetDungeonDifficulty())
        return;

    // cannot reset while in an instance
    Map* map = _player->FindMap();
    if (map && map->IsNonRaidInstance())
    {
        SF_LOG_DEBUG("network", "player (Name: %s, GUID: %u) tried to reset the instance while player is inside!",
            _player->GetName().c_str(), _player->GetGUIDLow());
        return;
    }

    Group* group = _player->GetGroup();
    if (group)
    {
        if (group->IsLeader(_player->GetGUID()))
        {
            for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* groupGuy = itr->GetSource();
                if (!groupGuy)
                    continue;

                if (!groupGuy->IsInMap(groupGuy))
                    return;

                if (groupGuy->GetMap()->IsNonRaidInstance())
                {
                    SF_LOG_DEBUG("network", "player %d tried to reset the instance while group member (Name: %s, GUID: %u) is inside!",
                        _player->GetGUIDLow(), groupGuy->GetName().c_str(), groupGuy->GetGUIDLow());
                    return;
                }
            }
            // the difficulty is set even if the instances can't be reset
            //_player->SendDungeonDifficulty(true);
            group->ResetInstances(InstanceResetMethod::INSTANCE_RESET_CHANGE_DIFFICULTY, false, _player);
            group->SetDungeonDifficulty(DifficultyID(difficulty));
        }
    }
    else
    {
        _player->ResetInstances(InstanceResetMethod::INSTANCE_RESET_CHANGE_DIFFICULTY, false);
        _player->SetDungeonDifficulty(DifficultyID(difficulty));
        _player->SendDungeonDifficulty();
    }
}

void WorldSession::HandleSetRaidDifficultyOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "Received: CMSG_SET_RAID_DIFFICULTY");

    UInt32Request request = ReadUInt32Request(recvData);

    DifficultyEntry const* difficultyEntry = sDifficultyStore.LookupEntry(request.value);
    if (!difficultyEntry)
    {
        SF_LOG_DEBUG("network", "%d sent an invalid instance mode %u!",
            _player->GetGUIDLow(), request.value);
        return;
    }
    if (difficultyEntry->maptype != MAP_RAID)
    {
        SF_LOG_DEBUG("network", "%d sent an non-raid instance mode %u!",
            _player->GetGUIDLow(), difficultyEntry->DiffID);
        return;
    }

    DifficultyID difficulty = DifficultyID(difficultyEntry->DiffID);
    if (difficulty == _player->GetRaidDifficulty())
        return;

    // cannot reset while in an instance
    Map* map = _player->FindMap();
    if (map && map->IsRaid())
    {
        SF_LOG_DEBUG("network", "player %d tried to reset the raid while inside!", _player->GetGUIDLow());
        return;
    }

    Group* group = _player->GetGroup();
    if (group)
    {
        if (group->IsLeader(_player->GetGUID()))
        {
            for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* groupGuy = itr->GetSource();
                if (!groupGuy)
                    continue;

                if (!groupGuy->IsInMap(groupGuy))
                    return;

                if (groupGuy->GetMap()->IsRaid())
                {
                    SF_LOG_DEBUG("network", "player %d tried to reset the raid while inside!", _player->GetGUIDLow());
                    return;
                }
            }
            // the difficulty is set even if the instances can't be reset
            //_player->SendDungeonDifficulty(true);
            group->ResetInstances(InstanceResetMethod::INSTANCE_RESET_CHANGE_DIFFICULTY, true, _player);
            group->SetRaidDifficulty(DifficultyID(difficulty));
        }
    }
    else
    {
        _player->ResetInstances(InstanceResetMethod::INSTANCE_RESET_CHANGE_DIFFICULTY, true);
        _player->SetRaidDifficulty(DifficultyID(difficulty));
        _player->SendRaidDifficulty();
    }
}

void WorldSession::HandleCancelMountAuraOpcode(WorldPacket& /*recvData*/)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_CANCEL_MOUNT_AURA");

    //If player is not mounted, so go out :)
    if (!_player->IsMounted())                              // not blizz like; no any messages on blizz
    {
        ChatHandler(this).SendSysMessage(LANG_CHAR_NON_MOUNTED);
        return;
    }

    if (_player->IsInFlight())                               // not blizz like; no any messages on blizz
    {
        ChatHandler(this).SendSysMessage(LANG_YOU_IN_FLIGHT);
        return;
    }

    _player->RemoveAurasByType(SPELL_AURA_MOUNTED); // Calls Dismount()
}

void WorldSession::HandleRequestPetInfoOpcode(WorldPacket& /*recvData */)
{
    if (_player->GetVehicle())
        _player->VehicleSpellInitialize();
    else if (Unit* minion = _player->GetFirstControlled())
    {
        if (minion->IsPet())
            _player->PetSpellInitialize();
        else if (minion->HasUnitTypeMask(UNIT_MASK_CONTROLABLE_GUARDIAN))
            _player->CharmSpellInitialize();
    }
}

void WorldSession::HandleSetTaxiBenchmarkOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_SET_TAXI_BENCHMARK_MODE");

    UInt8Request request = ReadUInt8Request(recvData);

    request.value ? _player->SetFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_TAXI_BENCHMARK) : _player->RemoveFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_TAXI_BENCHMARK);

    SF_LOG_DEBUG("network", "Client used \"/timetest %d\" command", request.value);
}

void WorldSession::HandleQueryInspectAchievements(WorldPacket& recvData)
{
    GuidRequest request = ReadQueryInspectAchievementsRequest(recvData);

    Player* player = ObjectAccessor::FindPlayer(request.guid);
    if (!player)
    {
        SF_LOG_DEBUG("network", "CMSG_QUERY_INSPECT_ACHIEVEMENTS: Inspected Player " UI64FMTD, (uint64)request.guid);
        return;
    }

    player->SendRespondInspectAchievements(_player);
}

void WorldSession::HandleGuildAchievementProgressQuery(WorldPacket& recvData)
{
    UInt32Request request = ReadUInt32Request(recvData);

    if (Guild* guild = sGuildMgr->GetGuildById(_player->GetGuildId()))
        guild->GetAchievementMgr().SendAchievementInfo(_player, request.value);
}

void WorldSession::HandleWorldStateUITimerUpdate(WorldPacket& /*recvData*/)
{
    // empty opcode
    SF_LOG_DEBUG("network", "WORLD: CMSG_WORLD_STATE_UI_TIMER_UPDATE");

    WorldPacket data(SMSG_UI_TIME, 4);
    data << uint32(time(NULL));
    SendPacket(&data);
}

void WorldSession::HandleReadyForAccountDataTimes(WorldPacket& /*recvData*/)
{
    // empty opcode
    SF_LOG_DEBUG("network", "WORLD: CMSG_READY_FOR_ACCOUNT_DATA_TIMES");

    SendAccountDataTimes(GLOBAL_CACHE_MASK);
}

void WorldSession::SendSetPhaseShift(std::set<uint32> const& phaseIds, std::set<uint32> const& terrainswaps, std::set<uint32> const& worldMapAreas)
{
    ObjectGuid guid = _player->GetGUID();

    WorldPacket data(SMSG_PHASE_SHIFT_CHANGE, 1 + 8 + 2 * phaseIds.size() + 4 + 2 * worldMapAreas.size() + 2 * terrainswaps.size() + 4);
    data.WriteGuidMask(guid, 0, 3, 1, 4, 6, 2, 7, 5);

    data.WriteGuidBytes(guid, 4, 3, 2);

    data << uint32(phaseIds.size()) * 2;        // Phase.dbc ids
    for (std::set<uint32>::const_iterator itr = phaseIds.begin(); itr != phaseIds.end(); ++itr)
        data << uint16(*itr);

    data.WriteGuidBytes(guid, 0, 6);

    data << uint32(0);                          // Inactive terrain swaps
    //for (uint8 i = 0; i < inactiveSwapsCount; ++i)
    //    data << uint16(0);

    data.WriteGuidBytes(guid, 1, 7);

    data << uint32(worldMapAreas.size()) * 2;     // WorldMapArea.dbc id (controls map display)
    for (std::set<uint32>::const_iterator itr = worldMapAreas.begin(); itr != worldMapAreas.end(); ++itr)
        data << uint16(*itr);

    data << uint32(terrainswaps.size()) * 2;    // Active terrain swaps
    for (std::set<uint32>::const_iterator itr = terrainswaps.begin(); itr != terrainswaps.end(); ++itr)
        data << uint16(*itr);

    data.WriteGuidBytes(guid, 5);

    data << uint32(phaseIds.size() ? 0 : 8);  // flags (not phasemask)

    SendPacket(&data);
}

// Battlefield and Battleground
void WorldSession::HandleAreaSpiritHealerQueryOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_AREA_SPIRIT_HEALER_QUERY");

    Battleground* bg = _player->GetBattleground();

    GuidRequest request = ReadAreaSpiritHealerQueryRequest(recvData);

    Creature* unit = GetPlayer()->GetMap()->GetCreature(request.guid);
    if (!unit)
        return;

    if (!unit->IsSpiritService())                            // it's not spirit service
        return;

    if (bg)
        sBattlegroundMgr->SendAreaSpiritHealerQueryOpcode(_player, bg, request.guid);

    if (Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(_player->GetZoneId()))
        bf->SendAreaSpiritHealerQueryOpcode(_player, request.guid);
}

void WorldSession::HandleAreaSpiritHealerQueueOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_AREA_SPIRIT_HEALER_QUEUE");

    Battleground* bg = _player->GetBattleground();

    GuidRequest request = ReadAreaSpiritHealerQueueRequest(recvData);

    Creature* unit = GetPlayer()->GetMap()->GetCreature(request.guid);
    if (!unit)
        return;

    if (!unit->IsSpiritService())                            // it's not spirit service
        return;

    if (bg)
        bg->AddPlayerToResurrectQueue(request.guid, _player->GetGUID());

    if (Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(_player->GetZoneId()))
        bf->AddPlayerToResurrectQueue(request.guid, _player->GetGUID());
}

void WorldSession::HandleHearthAndResurrect(WorldPacket& /*recvData*/)
{
    if (_player->IsInFlight())
        return;

    if (/*Battlefield* bf = */sBattlefieldMgr->GetBattlefieldToZoneId(_player->GetZoneId()))
    {
        // bf->PlayerAskToLeave(_player); FIXME
        return;
    }

    AreaTableEntry const* atEntry = sAreaStore.LookupEntry(_player->GetAreaId());
    if (!atEntry || !(atEntry->m_flags & AREA_FLAG_WINTERGRASP_2))
        return;

    _player->BuildPlayerRepop();
    _player->ResurrectPlayer(100);
    _player->TeleportTo(_player->m_homebindMapId, _player->m_homebindX, _player->m_homebindY, _player->m_homebindZ, _player->GetOrientation());
}

void WorldSession::HandleInstanceLockResponse(WorldPacket& recvPacket)
{
    BoolRequest request = ReadInstanceLockResponseRequest(recvPacket);

    if (!_player->HasPendingBind())
    {
        SF_LOG_INFO("network", "InstanceLockResponse: Player %s (guid %u) tried to bind himself/teleport to graveyard without a pending bind!",
            _player->GetName().c_str(), _player->GetGUIDLow());
        return;
    }

    if (request.value)
        _player->BindToInstance();
    else
        _player->RepopAtGraveyard();

    _player->SetPendingBind(0, 0);
}

namespace
{
    // No server DB2 loaded — replying (even with size 0) can crash the 5.4.8 client.
    bool AlwaysSkipHotfixReply(uint32 type)
    {
        switch (type)
        {
            case DB2_REPLY_SPELL_VISUAL:
            case DB2_REPLY_SPELL_VISUAL_EFFECT_NAME:
            case DB2_REPLY_SPELL_VISUAL_KIT:
            case DB2_REPLY_SPELL_VISUAL_KIT_MODEL_ATTACH:
            case DB2_REPLY_SPELL_VISUAL_MISSILE:
            case DB2_REPLY_SPELL_EFFECT_CAMERA_SHAKES:
            case DB2_REPLY_SPELL_MISSILE:
            case DB2_REPLY_SPELL_MISSILE_MOTION:
                return true;
            default:
                return false;
        }
    }

    bool IsPlausibleHotfixEntry(uint32 entry)
    {
        return entry != 0 && entry <= 0x00FFFFFF;
    }

    bool ShouldSendHotfixReply(uint32 type, uint32 entry)
    {
        if (AlwaysSkipHotfixReply(type))
            return false;
        return IsPlausibleHotfixEntry(entry);
    }

    void SendEmptyDbReply(WorldSession* session, uint32 entry, uint32 type, char const* reason)
    {
        WorldPacket data(SMSG_DB_REPLY);
        data << uint32(entry);
        data << uint32(time(NULL));
        data << uint32(type);
        data << uint32(0);

        session->SendPacket(&data);
        SF_LOG_INFO("network", "SMSG_DB_REPLY: empty hotfix entry %u type %u (%s)", entry, type, reason);
    }

    bool IsKnownHotfixType(uint32 type)
    {
        switch (type)
        {
            case DB2_REPLY_BATTLE_PET_ABILITY:
            case DB2_REPLY_BATTLE_PET_ABILITY_EFFECT:
            case DB2_REPLY_BATTLE_PET_ABILITY_STATE:
            case DB2_REPLY_BATTLE_PET_ABILITY_TURN:
            case DB2_REPLY_BATTLE_PET_BREED_QUALITY:
            case DB2_REPLY_BATTLE_PET_BREED_STATE:
            case DB2_REPLY_BATTLE_PET_EFFECT_PROPERTIES:
            case DB2_REPLY_BATTLE_PET_NPC_TEAM_MEMBER:
            case DB2_REPLY_BATTLE_PET_SPECIES:
            case DB2_REPLY_BATTLE_PET_SPECIES_STATE:
            case DB2_REPLY_BATTLE_PET_SPECIESX_ABILITY:
            case DB2_REPLY_BATTLE_PET_STATE:
            case DB2_REPLY_BATTLE_PET_VISUAL:
            case DB2_REPLY_BROADCAST_TEXT:
            case DB2_REPLY_CREATURE:
            case DB2_REPLY_CREATURE_DIFFICULTY:
            case DB2_REPLY_CURVE:
            case DB2_REPLY_CURVE_POINT:
            case DB2_REPLY_DEVICE_BLACKLIST:
            case DB2_REPLY_DRIVER_BLACKLIST:
            case DB2_REPLY_GAMEOBJECTS:
            case DB2_REPLY_ITEM:
            case DB2_REPLY_ITEM_SPARSE:
            case DB2_REPLY_ITEM_CURRENCY_COST:
            case DB2_REPLY_ITEM_EXTENDED_COST:
            case DB2_REPLY_ITEM_TO_BATTLE_PET:
            case DB2_REPLY_ITEM_TO_MOUNT_SPELL:
            case DB2_REPLY_ITEM_UPGRADE:
            case DB2_REPLY_KEYCHAIN:
            case DB2_REPLY_LOCALE:
            case DB2_REPLY_LOCATION:
            case DB2_REPLY_MAP_CHALLENGE_MODE:
            case DB2_REPLY_MARKETING_PROMOTIONS_X_LOCALE:
            case DB2_REPLY_PATH:
            case DB2_REPLY_PATH_NODE:
            case DB2_REPLY_PATH_NODE_PROPERTY:
            case DB2_REPLY_PATH_PROPERTY:
            case DB2_REPLY_QUEST_PACKAGE_ITEM:
            case DB2_REPLY_RULE_SET_ITEM_UPGRADE:
            case DB2_REPLY_RULE_SET_RAID_LOOT_UPGRADE:
            case DB2_REPLY_SCENE_SCRIPT:
            case DB2_REPLY_SCENE_SCRIPT_PACKAGE:
            case DB2_REPLY_SCENE_SCRIPT_PACKAGE_MEMBER:
            case DB2_REPLY_SPELL_EFFECT_CAMERA_SHAKES:
            case DB2_REPLY_SPELL_MISSILE:
            case DB2_REPLY_SPELL_MISSILE_MOTION:
            case DB2_REPLY_SPELL_REAGENTS:
            case DB2_REPLY_SPELL_VISUAL:
            case DB2_REPLY_SPELL_VISUAL_EFFECT_NAME:
            case DB2_REPLY_SPELL_VISUAL_KIT:
            case DB2_REPLY_SPELL_VISUAL_KIT_MODEL_ATTACH:
            case DB2_REPLY_SPELL_VISUAL_MISSILE:
            case DB2_REPLY_VIGNETTE:
            case DB2_REPLY_WB_ACCESS_CONTROL_LIST:
            case DB2_REPLY_WB_CERT_BLACKLIST:
            case DB2_REPLY_WB_CERT_WHITELIST:
            case DB2_REPLY_WB_PERMISSIONS:
                return true;
            default:
                return false;
        }
    }
}

void WorldSession::HandleRequestHotfix(WorldPacket& recvPacket)
{
    uint32 type, count;
    recvPacket >> type;

    DB2StorageBase const* store = GetDB2Storage(type);
    bool const knownHotfixType = IsKnownHotfixType(type);
    bool const knownUnsupportedType = !store && knownHotfixType;
    if (!store && !knownUnsupportedType)
    {
        SF_LOG_ERROR("network", "CMSG_REQUEST_HOTFIX: Received unknown hotfix type: %u", type);
        recvPacket.rfinish();
        return;
    }

    count = recvPacket.ReadBits(21);

    std::vector<ObjectGuid> guids(count);
    for (uint32 i = 0; i < count; ++i)
    {
        recvPacket.ReadGuidMask(guids[i], 6, 3, 0, 1, 4, 5, 7, 2);
    }

    std::vector<uint32> entries(count);
    for (uint32 i = 0; i < count; ++i)
    {
        recvPacket.ReadByteSeq(guids[i][1]);
        recvPacket >> entries[i];
        recvPacket.ReadGuidBytes(guids[i], 0, 5, 6, 4, 7, 2, 3);
    }

    if (knownUnsupportedType)
    {
        for (uint32 i = 0; i < count; ++i)
        {
            if (!ShouldSendHotfixReply(type, entries[i]))
            {
                SF_LOG_INFO("network", "CMSG_REQUEST_HOTFIX: skipped entry %u type %u", entries[i], type);
                continue;
            }

            SendEmptyDbReply(this, entries[i], type, "unsupported store");
        }

        return;
    }

    for (uint32 i = 0; i < count; ++i)
    {
        if (!ShouldSendHotfixReply(type, entries[i]))
        {
            SF_LOG_INFO("network", "CMSG_REQUEST_HOTFIX: skipped entry %u type %u", entries[i], type);
            continue;
        }

        // temp: this should be moved once broadcast text is properly implemented
        if (type == DB2_REPLY_BROADCAST_TEXT)
        {
            SF_LOG_INFO("network", "SMSG_DB_REPLY: broadcast text hotfix entry %u (gossip hack)", entries[i]);
            SendBroadcastText(entries[i]);
            continue;
        }

        if (!store->HasRecord(entries[i]))
            continue;

        ByteBuffer record;
        store->WriteRecord(entries[i], (uint32)GetSessionDbcLocale(), record);

        WorldPacket data(SMSG_DB_REPLY);
        data << uint32(entries[i]);
        data << uint32(time(NULL));
        data << uint32(type);
        data << uint32(record.size());
        data.append(record);

        SendPacket(&data);

        SF_LOG_DEBUG("network", "SMSG_DB_REPLY: Sent hotfix entry: %u type: %u", entries[i], type);
    }
}

void WorldSession::SendBroadcastText(uint32 entry)
{
    /*
     *  This is a hack fix! Still uses Gossip Id's instead of Broadcast Id's.
     *  Major database changed required at some point.
     */

    ByteBuffer buffer;
    std::string defaultText = "Greetings, $n";

    GossipText const* pGossip = sObjectMgr->GetGossipText(entry);

    uint16 nrmTextLength = pGossip ? pGossip->Options[0].Text_0.length() : defaultText.length();
    uint16 altTextLength = pGossip ? pGossip->Options[0].Text_1.length() : defaultText.length();

    buffer << uint32(entry);
    buffer << uint32(pGossip ? pGossip->Options[0].Language : 0);
    buffer << uint16(nrmTextLength);

    if (nrmTextLength)
        buffer << std::string(pGossip ? pGossip->Options[0].Text_0 : defaultText);

    buffer << uint16(altTextLength);

    if (altTextLength)
        buffer << std::string(pGossip ? pGossip->Options[0].Text_1 : defaultText);

    for (int i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; i++)
        buffer << uint32(0);

    buffer << uint32(1);

    WorldPacket data(SMSG_DB_REPLY);
    data << uint32(entry);
    data << uint32(time(NULL));
    data << uint32(DB2_REPLY_BROADCAST_TEXT);
    data << uint32(buffer.size());
    data.append(buffer);

    SendPacket(&data);
}

void WorldSession::HandleUpdateMissileTrajectory(WorldPacket& recvPacket)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_UPDATE_MISSILE_TRAJECTORY");

    UpdateMissileTrajectoryRequest request = ReadUpdateMissileTrajectoryRequest(recvPacket);

    Unit* caster = ObjectAccessor::GetUnit(*_player, request.guid);
    Spell* spell = caster ? caster->GetCurrentSpell(CURRENT_GENERIC_SPELL) : NULL;
    if (!spell || spell->m_spellInfo->Id != request.spellId || !spell->m_targets.HasDst() || !spell->m_targets.HasSrc())
    {
        recvPacket.rfinish();
        return;
    }

    Position pos = *spell->m_targets.GetSrcPos();
    pos.Relocate(request.curX, request.curY, request.curZ);
    spell->m_targets.ModSrc(pos);

    pos = *spell->m_targets.GetDstPos();
    pos.Relocate(request.targetX, request.targetY, request.targetZ);
    spell->m_targets.ModDst(pos);

    spell->m_targets.SetElevation(request.elevation);
    spell->m_targets.SetSpeed(request.speed);

    if (request.moveStop)
    {
        uint32 opcode;
        recvPacket >> opcode;
        recvPacket.SetOpcode(MSG_MOVE_STOP); // always set to MSG_MOVE_STOP in client SetOpcode
        HandleMovementOpcodes(recvPacket);
    }
}

void WorldSession::HandleViolenceLevel(WorldPacket& recvPacket)
{
    ReadUInt8Request(recvPacket);

    // do something?
}

void WorldSession::HandleObjectUpdateFailedOpcode(WorldPacket& recvPacket)
{
    GuidRequest request = ReadObjectUpdateFailedRequest(recvPacket);

    WorldObject* obj = ObjectAccessor::GetWorldObject(*GetPlayer(), request.guid);
    if (request.guid == GetPlayer()->GetGUID())
        SF_LOG_ERROR("network", "Object update failed for object " UI64FMTD " (%s) for player %s (%u)", uint64(request.guid), obj ? obj->GetName().c_str() : "object-not-found", GetPlayerName().c_str(), GetGuidLow());
    else
        SF_LOG_DEBUG("network", "Object update failed for object " UI64FMTD " (%s) for player %s (%u)", uint64(request.guid), obj ? obj->GetName().c_str() : "object-not-found", GetPlayerName().c_str(), GetGuidLow());

    // If create object failed for current player then client will be stuck on loading screen
    //if (_player->GetGUID() == guid)
    //{
    //    LogoutPlayer(true);
    //    return;
    //}

    // Pretend we've never seen this object
    //_player->m_clientGUIDs.erase(guid);
}

void WorldSession::HandleSaveCUFProfiles(WorldPacket& recvPacket)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_SAVE_CUF_PROFILES");

    uint8 count = (uint8)recvPacket.ReadBits(19);

    if (count > MAX_CUF_PROFILES)
    {
        SF_LOG_ERROR("entities.player", "HandleSaveCUFProfiles - %s tried to save more than %i CUF profiles. Hacking attempt?", GetPlayerName().c_str(), MAX_CUF_PROFILES);
        recvPacket.rfinish();
        return;
    }

    CUFProfile* profiles[MAX_CUF_PROFILES];
    uint8 strlens[MAX_CUF_PROFILES];

    for (uint8 i = 0; i < count; ++i)
    {
        profiles[i] = new CUFProfile;
        profiles[i]->BoolOptions.set(CUF_AUTO_ACTIVATE_SPEC_2, recvPacket.ReadBit());
        profiles[i]->BoolOptions.set(CUF_DISPLAY_MAIN_TANK_AND_ASSIST, recvPacket.ReadBit());
        profiles[i]->BoolOptions.set(CUF_DISPLAY_POWER_BAR, recvPacket.ReadBit());
        profiles[i]->BoolOptions.set(CUF_AUTO_ACTIVATE_10_PLAYERS, recvPacket.ReadBit());
        profiles[i]->BoolOptions.set(CUF_AUTO_ACTIVATE_3_PLAYERS, recvPacket.ReadBit());
        profiles[i]->BoolOptions.set(CUF_UNK_156, recvPacket.ReadBit());
        profiles[i]->BoolOptions.set(CUF_AUTO_ACTIVATE_40_PLAYERS, recvPacket.ReadBit());
        profiles[i]->BoolOptions.set(CUF_AUTO_ACTIVATE_2_PLAYERS, recvPacket.ReadBit());
        profiles[i]->BoolOptions.set(CUF_KEEP_GROUPS_TOGETHER, recvPacket.ReadBit());
        profiles[i]->BoolOptions.set(CUF_USE_CLASS_COLORS, recvPacket.ReadBit());
        profiles[i]->BoolOptions.set(CUF_AUTO_ACTIVATE_25_PLAYERS, recvPacket.ReadBit());
        profiles[i]->BoolOptions.set(CUF_UNK_145, recvPacket.ReadBit());
        strlens[i] = (uint8)recvPacket.ReadBits(7);
        profiles[i]->BoolOptions.set(CUF_DISPLAY_PETS, recvPacket.ReadBit());
        profiles[i]->BoolOptions.set(CUF_AUTO_ACTIVATE_PVP, recvPacket.ReadBit());
        profiles[i]->BoolOptions.set(CUF_DISPLAY_ONLY_DISPELLABLE_DEBUFFS, recvPacket.ReadBit());
        profiles[i]->BoolOptions.set(CUF_DISPLAY_NON_BOSS_DEBUFFS, recvPacket.ReadBit());
        profiles[i]->BoolOptions.set(CUF_AUTO_ACTIVATE_15_PLAYERS, recvPacket.ReadBit());
        profiles[i]->BoolOptions.set(CUF_UNK_157, recvPacket.ReadBit());
        profiles[i]->BoolOptions.set(CUF_DISPLAY_BORDER, recvPacket.ReadBit());
        profiles[i]->BoolOptions.set(CUF_DISPLAY_HORIZONTAL_GROUPS, recvPacket.ReadBit());
        profiles[i]->BoolOptions.set(CUF_AUTO_ACTIVATE_SPEC_1, recvPacket.ReadBit());
        profiles[i]->BoolOptions.set(CUF_AUTO_ACTIVATE_5_PLAYERS, recvPacket.ReadBit());
        profiles[i]->BoolOptions.set(CUF_AUTO_ACTIVATE_PVE, recvPacket.ReadBit());
        profiles[i]->BoolOptions.set(CUF_DISPLAY_HEAL_PREDICTION, recvPacket.ReadBit());
        profiles[i]->BoolOptions.set(CUF_DISPLAY_AGGRO_HIGHLIGHT, recvPacket.ReadBit());
    }

    for (uint8 i = 0; i < count; ++i)
    {
        recvPacket >> profiles[i]->FrameHeight;
        recvPacket >> profiles[i]->Unk146;
        recvPacket >> profiles[i]->HealthText;
        recvPacket >> profiles[i]->FrameWidth;
        recvPacket >> profiles[i]->Unk148;
        recvPacket >> profiles[i]->SortBy;
        recvPacket >> profiles[i]->Unk150;
        profiles[i]->ProfileName = recvPacket.ReadString(strlens[i]);
        recvPacket >> profiles[i]->Unk147;
        recvPacket >> profiles[i]->Unk152;
        recvPacket >> profiles[i]->Unk154;

        GetPlayer()->SaveCUFProfile(i, profiles[i]);
    }

    for (uint8 i = count; i < MAX_CUF_PROFILES; ++i)
        GetPlayer()->SaveCUFProfile(i, NULL);
}

void WorldSession::SendLoadCUFProfiles()
{
    Player* player = GetPlayer();

    uint8 count = player->GetCUFProfilesCount();

    ByteBuffer byteBuffer(25 * count);
    WorldPacket data(SMSG_LOAD_CUF_PROFILES, 5 * count + 25 * count);

    data.WriteBits(count, 20);
    for (uint8 i = 0; i < MAX_CUF_PROFILES; ++i)
    {
        CUFProfile* profile = player->GetCUFProfile(i);
        if (!profile)
            continue;

        data.WriteBit(profile->BoolOptions[CUF_AUTO_ACTIVATE_SPEC_1]);
        data.WriteBit(profile->BoolOptions[CUF_AUTO_ACTIVATE_3_PLAYERS]);
        data.WriteBit(profile->BoolOptions[CUF_UNK_157]);
        data.WriteBit(profile->BoolOptions[CUF_AUTO_ACTIVATE_10_PLAYERS]);
        data.WriteBit(profile->BoolOptions[CUF_AUTO_ACTIVATE_40_PLAYERS]);
        data.WriteBit(profile->BoolOptions[CUF_DISPLAY_BORDER]);
        data.WriteBit(profile->BoolOptions[CUF_USE_CLASS_COLORS]);
        data.WriteBit(profile->BoolOptions[CUF_KEEP_GROUPS_TOGETHER]);
        data.WriteBit(profile->BoolOptions[CUF_DISPLAY_POWER_BAR]);
        data.WriteBits(profile->ProfileName.size(), 8);
        data.WriteBit(profile->BoolOptions[CUF_DISPLAY_PETS]);
        data.WriteBit(profile->BoolOptions[CUF_DISPLAY_AGGRO_HIGHLIGHT]);
        data.WriteBit(profile->BoolOptions[CUF_UNK_145]);
        data.WriteBit(profile->BoolOptions[CUF_AUTO_ACTIVATE_PVP]);
        data.WriteBit(profile->BoolOptions[CUF_UNK_156]);
        data.WriteBit(profile->BoolOptions[CUF_DISPLAY_MAIN_TANK_AND_ASSIST]);
        data.WriteBit(profile->BoolOptions[CUF_DISPLAY_NON_BOSS_DEBUFFS]);
        data.WriteBit(profile->BoolOptions[CUF_DISPLAY_HORIZONTAL_GROUPS]);
        data.WriteBit(profile->BoolOptions[CUF_AUTO_ACTIVATE_SPEC_2]);
        data.WriteBit(profile->BoolOptions[CUF_DISPLAY_HEAL_PREDICTION]);
        data.WriteBit(profile->BoolOptions[CUF_DISPLAY_ONLY_DISPELLABLE_DEBUFFS]);
        data.WriteBit(profile->BoolOptions[CUF_AUTO_ACTIVATE_25_PLAYERS]);
        data.WriteBit(profile->BoolOptions[CUF_AUTO_ACTIVATE_PVE]);
        data.WriteBit(profile->BoolOptions[CUF_AUTO_ACTIVATE_5_PLAYERS]);
        data.WriteBit(profile->BoolOptions[CUF_AUTO_ACTIVATE_15_PLAYERS]);
        data.WriteBit(profile->BoolOptions[CUF_AUTO_ACTIVATE_2_PLAYERS]);

        byteBuffer << uint16(profile->Unk152);
        byteBuffer << uint16(profile->Unk154);
        byteBuffer << uint8(profile->HealthText);
        byteBuffer.WriteString(profile->ProfileName);
        byteBuffer << uint8(profile->Unk147);
        byteBuffer << uint8(profile->Unk146);
        byteBuffer << uint16(profile->FrameHeight);
        byteBuffer << uint8(profile->Unk148);
        byteBuffer << uint8(profile->SortBy);
        byteBuffer << uint16(profile->FrameWidth);
        byteBuffer << uint16(profile->Unk150);
    }

    data.FlushBits();
    data.append(byteBuffer);
    SendPacket(&data);
}

#define JOIN_THE_ALLIANCE 1
#define JOIN_THE_HORDE    0

void WorldSession::HandleSelectFactionOpcode(WorldPacket& recvPacket)
{
    UInt32Request request = ReadUInt32Request(recvPacket);

    if (_player->getRace() != RACE_PANDAREN_NEUTRAL)
        return;

    if (request.value == JOIN_THE_HORDE)
    {
        _player->SetByteValue(UNIT_FIELD_SEX, 0, RACE_PANDAREN_HORDE);
        _player->setFactionForRace(RACE_PANDAREN_HORDE);
        _player->SaveToDB();
        WorldLocation location(1, 1357.62f, -4373.55f, 26.13f, 0.13f);
        _player->TeleportTo(location);
        _player->SetHomebind(location, 363);
        _player->learnSpell(669, false); // Language Orcish
        _player->learnSpell(108127, false); // Language Pandaren
    }
    else if (request.value == JOIN_THE_ALLIANCE)
    {
        _player->SetByteValue(UNIT_FIELD_SEX, 0, RACE_PANDAREN_ALLIANCE);
        _player->setFactionForRace(RACE_PANDAREN_ALLIANCE);
        _player->SaveToDB();
        WorldLocation location(0, -8960.02f, 516.10f, 96.36f, 0.67f);
        _player->TeleportTo(location);
        _player->SetHomebind(location, 9);
        _player->learnSpell(668, false); // Language Common
        _player->learnSpell(108127, false); // Language Pandaren
    }

    if (_player->GetQuestStatus(31450) == QUEST_STATUS_INCOMPLETE)
        _player->KilledMonsterCredit(64594);

    _player->SendMovieStart(116);
}

void WorldSession::HandleDiscardedTimeSyncAcks(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_DISCARDED_TIME_SYNC_ACKS");

    DiscardedTimeSyncAcksRequest request = ReadDiscardedTimeSyncAcksRequest(recvData);
    if (!_player)
        return;

    if (!request.hasCounter)
        return;

    while (!_player->m_timeSyncQueue.empty())
    {
        uint32 counter = _player->m_timeSyncQueue.front();
        _player->m_timeSyncQueue.pop();

        SF_LOG_DEBUG("network", "Discarded time sync ack %u from player %s, removed pending counter %u",
            request.counter, _player->GetName().c_str(), counter);

        if (counter >= request.counter)
            break;
    }
}

void WorldSession::SendPlayMusic(uint32 SoundKitID)
{
    WorldPacket data(SMSG_PLAY_MUSIC, 4);
    data << uint32(SoundKitID);
    SendPacket(&data);
}

void WorldSession::SendPageText(ObjectGuid GameObjectGUID)
{
    WorldPacket data(SMSG_PAGETEXT, 8);
    data.WriteGuidMask(GameObjectGUID, 0, 3, 2, 6, 5, 1, 7, 4);
    data.WriteGuidBytes(GameObjectGUID, 6, 2, 7, 0, 5, 3, 1, 4);
    _player->SendDirectMessage(&data);
}

void WorldSession::HandleSceneCompleted(WorldPacket& recvPacket)
{
    SF_LOG_ERROR("network", "recv CMSG_SCENE_COMPLETED");
    SceneCompletedRequest request = ReadSceneCompletedRequest(recvPacket);
    SF_LOG_ERROR("network", "hasData %u", request.hasData);
}

void WorldSession::SendCrossedInebriationThreshold(ObjectGuid guid, uint32 ItemID, DrunkenState drunkenState)
{
    WorldPacket data(SMSG_CROSSED_INEBRIATION_THRESHOLD, (8 + 4 + 4));
    data.WriteGuidMask(guid, 0, 4, 2, 6, 5, 1, 3, 7);

    data.WriteGuidBytes(guid, 3);
    data << ItemID;
    data << uint32(drunkenState);
    data.WriteGuidBytes(guid, 4, 6, 7, 0, 2, 5, 1);
    _player->SendMessageToSet(&data, true);
}
