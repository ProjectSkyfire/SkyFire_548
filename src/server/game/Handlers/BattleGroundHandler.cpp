/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "Common.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "ArenaTeamMgr.h"
#include "WorldPacket.h"
#include "WorldSession.h"

#include "ArenaTeam.h"
#include "BattlegroundMgr.h"
#include "Battleground.h"
#include "Chat.h"
#include "Language.h"
#include "Log.h"
#include "Player.h"
#include "Object.h"
#include "Opcodes.h"
#include "DisableMgr.h"
#include "Group.h"

void WorldSession::HandleBattlemasterHelloOpcode(WorldPacket& recvData)
{
    uint64 guid;
    recvData >> guid;
    SF_LOG_DEBUG("network", "WORLD: Recvd CMSG_BATTLEMASTER_HELLO Message from (GUID: %u TypeId:%u)", GUID_LOPART(guid), uint8(GuidHigh2TypeId(GUID_HIPART(guid))));

    Creature* unit = GetPlayer()->GetMap()->GetCreature(guid);
    if (!unit)
        return;

    if (!unit->IsBattleMaster())                             // it's not battlemaster
        return;

    // Stop the npc if moving
    unit->StopMoving();

    BattlegroundTypeId bgTypeId = sBattlegroundMgr->GetBattleMasterBG(unit->GetEntry());

    if (!_player->GetBGAccessByLevel(bgTypeId))
    {
                                                            // temp, must be gossip message...
        SendNotification(LANG_YOUR_BG_LEVEL_REQ_ERROR);
        return;
    }

    SendBattleGroundList(guid, bgTypeId);
}

void WorldSession::SendBattleGroundList(uint64 guid, BattlegroundTypeId bgTypeId)
{
    WorldPacket data;
    sBattlegroundMgr->BuildBattlegroundListPacket(&data, guid, _player, bgTypeId);
    SendPacket(&data);
}

void WorldSession::HandleBattlemasterJoinOpcode(WorldPacket& recvData)
{
    uint32 bgTypeId_;
    uint8 asGroup;
    bool isPremade = false;
    Group* grp = NULL;
    ObjectGuid guid;
    bool hasRoleMask;
    uint8 roleMask = 0;

    for (int i = 0; i < 2; i++) // blacklistedMapIds
        recvData.read_skip<uint32>();

    guid[1] = recvData.ReadBit();
    guid[7] = recvData.ReadBit();
    guid[0] = recvData.ReadBit();
    guid[3] = recvData.ReadBit();
    asGroup = recvData.ReadBit();           // As Group
    guid[4] = recvData.ReadBit();
    hasRoleMask = !recvData.ReadBit();
    guid[6] = recvData.ReadBit();
    guid[2] = recvData.ReadBit();
    guid[5] = recvData.ReadBit();

    recvData.ReadByteSeq(guid[7]);
    recvData.ReadByteSeq(guid[2]);
    recvData.ReadByteSeq(guid[4]);
    recvData.ReadByteSeq(guid[5]);
    recvData.ReadByteSeq(guid[0]);
    recvData.ReadByteSeq(guid[6]);
    recvData.ReadByteSeq(guid[3]);
    recvData.ReadByteSeq(guid[1]);

    if (hasRoleMask)
        recvData >> roleMask; // Need to set this as group role later

    //extract from guid
    bgTypeId_ = GUID_LOPART(guid);

    if (!sBattlemasterListStore.LookupEntry(bgTypeId_))
    {
        SF_LOG_ERROR("network", "Battleground: invalid bgtype (%u) received. possible cheater? player guid %u", bgTypeId_, _player->GetGUIDLow());
        return;
    }

    if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, bgTypeId_, NULL))
    {
        ChatHandler(this).PSendSysMessage(LANG_BG_DISABLED);
        return;
    }
    BattlegroundTypeId bgTypeId = BattlegroundTypeId(bgTypeId_);

    //SF_LOG_DEBUG("network", "WORLD: Recvd CMSG_BATTLEMASTER_JOIN Message from (GUID:"UI64FMTD" TypeId:%u)", guid, bgTypeId_);

    // can do this, since it's battleground, not arena
    BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, 0);
    BattlegroundQueueTypeId bgQueueTypeIdRandom = BattlegroundMgr::BGQueueTypeId(BattlegroundTypeId::BATTLEGROUND_RB, 0);

    // ignore if player is already in BG
    if (_player->InBattleground())
        return;

    // get bg instance or bg template if instance not found
    Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(bgTypeId);
    if (!bg)
        return;

    // expected bracket entry
    PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), _player->getLevel());
    if (!bracketEntry)
        return;

    GroupJoinBattlegroundResult err = GroupJoinBattlegroundResult::ERR_BATTLEGROUND_NONE;

    // check queue conditions
    if (!asGroup)
    {
        if (GetPlayer()->isUsingLfg())
        {
            WorldPacket data;
            sBattlegroundMgr->BuildStatusFailedPacket(&data, bg, _player, 0, GroupJoinBattlegroundResult::ERR_LFG_CANT_USE_BATTLEGROUND);
            GetPlayer()->GetSession()->SendPacket(&data);
            return;
        }

        // check Deserter debuff
        if (!_player->CanJoinToBattleground(bg))
        {
            WorldPacket data;
            sBattlegroundMgr->BuildStatusFailedPacket(&data, bg, _player, 0, GroupJoinBattlegroundResult::ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS);
            _player->GetSession()->SendPacket(&data);
            return;
        }

        if (_player->GetBattlegroundQueueIndex(bgQueueTypeIdRandom) < PLAYER_MAX_BATTLEGROUND_QUEUES)
        {
            // player is already in random queue
            WorldPacket data;
            sBattlegroundMgr->BuildStatusFailedPacket(&data, bg, _player, 0, GroupJoinBattlegroundResult::ERR_IN_RANDOM_BG);
            _player->GetSession()->SendPacket(&data);
            return;
        }

        if (_player->InBattlegroundQueue() && bgTypeId == BattlegroundTypeId::BATTLEGROUND_RB)
        {
            // player is already in queue, can't start random queue
            WorldPacket data;
            sBattlegroundMgr->BuildStatusFailedPacket(&data, bg, _player, 0, GroupJoinBattlegroundResult::ERR_IN_NON_RANDOM_BG);
            _player->GetSession()->SendPacket(&data);
            return;
        }

        // check if already in queue
        if (_player->GetBattlegroundQueueIndex(bgQueueTypeId) < PLAYER_MAX_BATTLEGROUND_QUEUES)
            // player is already in this queue
            return;

        // check if has free queue slots
        if (!_player->HasFreeBattlegroundQueueId())
        {
            WorldPacket data;
            sBattlegroundMgr->BuildStatusFailedPacket(&data, bg, _player, 0, GroupJoinBattlegroundResult::ERR_BATTLEGROUND_TOO_MANY_QUEUES);
            _player->GetSession()->SendPacket(&data);
            return;
        }

        BattlegroundQueue& bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);
        GroupQueueInfo* ginfo = bgQueue.AddGroup(_player, NULL, bgTypeId, bracketEntry, 0, false, isPremade, 0, 0);

        uint32 avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());
        uint32 queueSlot = _player->AddBattlegroundQueueId(bgQueueTypeId);

        // add joined time data
        _player->AddBattlegroundQueueJoinTime(bgTypeId, ginfo->JoinTime);

        WorldPacket data; // send status packet (in queue)
        sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, _player, queueSlot, STATUS_WAIT_QUEUE, avgTime, ginfo->JoinTime, ginfo->ArenaType);
        SendPacket(&data);

        SF_LOG_DEBUG("bg.battleground", "Battleground: player joined queue for bg queue type %u bg type %u: GUID %u, NAME %s",
                       bgQueueTypeId, uint32(bgTypeId), _player->GetGUIDLow(), _player->GetName().c_str());
    }
    else
    {
        grp = _player->GetGroup();

        if (!grp)
            return;

        if (grp->GetLeaderGUID() != _player->GetGUID())
            return;

        err = grp->CanJoinBattlegroundQueue(bg, bgQueueTypeId, 0, bg->GetMaxPlayersPerTeam(), false, 0);
        isPremade = (grp->GetMembersCount() >= bg->GetMinPlayersPerTeam());

        BattlegroundQueue& bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);
        GroupQueueInfo* ginfo = NULL;
        uint32 avgTime = 0;

        if (err == GroupJoinBattlegroundResult::ERR_BATTLEGROUND_NONE)
        {
            SF_LOG_DEBUG("bg.battleground", "Battleground: the following players are joining as group:");
            ginfo = bgQueue.AddGroup(_player, grp, bgTypeId, bracketEntry, 0, false, isPremade, 0, 0);
            avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());
        }

        for (GroupReference* itr = grp->GetFirstMember(); itr != NULL; itr = itr->next())
        {
            Player* member = itr->GetSource();
            if (!member)
                continue;   // this should never happen

            if (err != GroupJoinBattlegroundResult::ERR_BATTLEGROUND_NONE)
            {
                WorldPacket data;
                sBattlegroundMgr->BuildStatusFailedPacket(&data, bg, _player, 0, err);
                member->GetSession()->SendPacket(&data);
                continue;
            }

            // add to queue
            uint32 queueSlot = member->AddBattlegroundQueueId(bgQueueTypeId);

            // add joined time data
            member->AddBattlegroundQueueJoinTime(bgTypeId, ginfo->JoinTime);

            WorldPacket data; // send status packet (in queue)
            sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, member, queueSlot, STATUS_WAIT_QUEUE, avgTime, ginfo->JoinTime, ginfo->ArenaType);
            member->GetSession()->SendPacket(&data);
            SF_LOG_DEBUG("bg.battleground", "Battleground: player joined queue for bg queue type %u bg type %u: GUID %u, NAME %s",
                bgQueueTypeId, uint32(bgTypeId), member->GetGUIDLow(), member->GetName().c_str());
        }
        SF_LOG_DEBUG("bg.battleground", "Battleground: group end");
    }

    sBattlegroundMgr->ScheduleQueueUpdate(0, 0, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());
}

void WorldSession::HandleBattlegroundPlayerPositionsOpcode(WorldPacket& /*recvData*/)
{
    SF_LOG_DEBUG("network", "WORLD: Recvd CMSG_BATTLEGROUND_PLAYER_POSITIONS Message");

    Battleground* bg = _player->GetBattleground();
    if (!bg)                                                 // can't be received if player not in battleground
        return;

    uint32 acount = 0;
    uint32 hcount = 0;
    Player* aplr = NULL;
    Player* hplr = NULL;

    if (uint64 guid = bg->GetFlagPickerGUID(TEAM_ALLIANCE))
    {
        aplr = ObjectAccessor::FindPlayer(guid);
        if (aplr)
            ++acount;
    }

    if (uint64 guid = bg->GetFlagPickerGUID(TEAM_HORDE))
    {
        hplr = ObjectAccessor::FindPlayer(guid);
        if (hplr)
            ++hcount;
    }

    ObjectGuid aguid = aplr ? aplr->GetGUID() : 0;
    ObjectGuid hguid = hplr ? hplr->GetGUID() : 0;

    WorldPacket data(SMSG_BATTLEFIELD_PLAYER_POSITIONS);

    data.WriteBits(acount, 22);
    for (uint8 i = 0; i < acount; i++)
    {
        data.WriteBit(aguid[3]);
        data.WriteBit(aguid[5]);
        data.WriteBit(aguid[1]);
        data.WriteBit(aguid[6]);
        data.WriteBit(aguid[7]);
        data.WriteBit(aguid[0]);
        data.WriteBit(aguid[2]);
        data.WriteBit(aguid[4]);
    }

    data.WriteBits(hcount, 22);
    for (uint8 i = 0; i < hcount; i++)
    {
        data.WriteBit(hguid[6]);
        data.WriteBit(hguid[5]);
        data.WriteBit(hguid[4]);
        data.WriteBit(hguid[7]);
        data.WriteBit(hguid[2]);
        data.WriteBit(hguid[1]);
        data.WriteBit(hguid[0]);
        data.WriteBit(hguid[3]);
    }

    data.FlushBits();

    for (uint8 i = 0; i < hcount; i++)
    {
        data.WriteByteSeq(hguid[2]);
        data.WriteByteSeq(hguid[1]);
        data << float(hplr->GetPositionY());
        data.WriteByteSeq(hguid[5]);
        data.WriteByteSeq(hguid[4]);
        data.WriteByteSeq(hguid[7]);
        data.WriteByteSeq(hguid[0]);
        data.WriteByteSeq(hguid[6]);
        data.WriteByteSeq(hguid[3]);
        data << float(hplr->GetPositionX());
    }

    for (uint8 i = 0; i < acount; i++)
    {
        data.WriteByteSeq(aguid[6]);
        data << float(aplr->GetPositionX());
        data.WriteByteSeq(aguid[5]);
        data.WriteByteSeq(aguid[3]);
        data << float(aplr->GetPositionY());
        data.WriteByteSeq(aguid[1]);
        data.WriteByteSeq(aguid[7]);
        data.WriteByteSeq(aguid[0]);
        data.WriteByteSeq(aguid[2]);
        data.WriteByteSeq(aguid[4]);
    }

    SendPacket(&data);
}

void WorldSession::HandlePVPLogDataOpcode(WorldPacket & /*recvData*/)
{
    SF_LOG_DEBUG("network", "WORLD: Recvd CMSG_PVP_LOG_DATA Message");

    Battleground* bg = _player->GetBattleground();
    if (!bg)
        return;

    // Prevent players from sending BuildPvpLogDataPacket in an arena except for when sent in BattleGround::EndBattleGround.
    if (bg->isArena())
        return;

    WorldPacket data;
    sBattlegroundMgr->BuildPvpLogDataPacket(&data, bg);
    SendPacket(&data);

    SF_LOG_DEBUG("network", "WORLD: Sent SMSG_PVP_LOG_DATA Message");
}

void WorldSession::HandleBattlefieldListOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Recvd CMSG_BATTLEFIELD_LIST Message");

    uint32 bgTypeId;
    recvData >> bgTypeId;                                  // id from DBC

    BattlemasterListEntry const* bl = sBattlemasterListStore.LookupEntry(bgTypeId);
    if (!bl)
    {
        SF_LOG_DEBUG("bg.battleground", "BattlegroundHandler: invalid bgtype (%u) with player (Name: %s, GUID: %u) received.", bgTypeId, _player->GetName().c_str(), _player->GetGUIDLow());
        return;
    }

    WorldPacket data;
    sBattlegroundMgr->BuildBattlegroundListPacket(&data, 0, _player, BattlegroundTypeId(bgTypeId));
    SendPacket(&data);
}

void WorldSession::HandleBattleFieldPortOpcode(WorldPacket &recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Recvd CMSG_BATTLEFIELD_PORT Message");

    uint32 time;
    uint32 queueSlot;
    uint32 id;
    uint8 action;                       // enter battle 0x1, leave queue 0x0
    ObjectGuid guid;

    action = recvData.ReadBit();

    recvData >> queueSlot;
    recvData >> id;
    recvData >> time;

    guid[6] = recvData.ReadBit();
    guid[4] = recvData.ReadBit();
    guid[2] = recvData.ReadBit();
    guid[5] = recvData.ReadBit();
    guid[0] = recvData.ReadBit();
    guid[1] = recvData.ReadBit();
    guid[7] = recvData.ReadBit();
    guid[3] = recvData.ReadBit();

    recvData.ReadByteSeq(guid[2]);
    recvData.ReadByteSeq(guid[5]);
    recvData.ReadByteSeq(guid[3]);
    recvData.ReadByteSeq(guid[0]);
    recvData.ReadByteSeq(guid[7]);
    recvData.ReadByteSeq(guid[4]);
    recvData.ReadByteSeq(guid[6]);
    recvData.ReadByteSeq(guid[1]);

    if (!_player->InBattlegroundQueue())
    {
        SF_LOG_DEBUG("bg.battleground", "CMSG_BATTLEFIELD_PORT %s Slot: %u, Unk: %u, Time: %u, Action: %u. Player not in queue!",
            GetPlayerInfo().c_str(), queueSlot, id, time, action);
        return;
    }

    BattlegroundQueueTypeId bgQueueTypeId = _player->GetBattlegroundQueueTypeId(queueSlot);
    if (bgQueueTypeId == BATTLEGROUND_QUEUE_NONE)
    {
        SF_LOG_DEBUG("bg.battleground", "CMSG_BATTLEFIELD_PORT %s Slot: %u, Unk: %u, Time: %u, Action: %u. Invalid queueSlot!",
            GetPlayerInfo().c_str(), queueSlot, id, time, action);
        return;
    }

    BattlegroundQueue& bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);

    //we must use temporary variable, because GroupQueueInfo pointer can be deleted in BattlegroundQueue::RemovePlayer() function
    GroupQueueInfo ginfo;
    if (!bgQueue.GetPlayerGroupInfoData(_player->GetGUID(), &ginfo))
    {
        SF_LOG_DEBUG("bg.battleground", "CMSG_BATTLEFIELD_PORT %s Slot: %u, Unk: %u, Time: %u, Action: %u. Player not in queue (No player Group Info)!",
            GetPlayerInfo().c_str(), queueSlot, id, time, action);
        return;
    }
    // if action == 1, then instanceId is required
    if (!ginfo.IsInvitedToBGInstanceGUID && action == 1)
    {
        SF_LOG_DEBUG("bg.battleground", "CMSG_BATTLEFIELD_PORT %s Slot: %u, Unk: %u, Time: %u, Action: %u. Player is not invited to any bg!",
            GetPlayerInfo().c_str(), queueSlot, id, time, action);
        return;
    }

    BattlegroundTypeId bgTypeId = BattlegroundMgr::BGTemplateId(bgQueueTypeId);
    // BGTemplateId returns BATTLEGROUND_AA when it is arena queue.
    // Do instance id search as there is no AA bg instances.
    Battleground* bg = sBattlegroundMgr->GetBattleground(ginfo.IsInvitedToBGInstanceGUID, bgTypeId == BattlegroundTypeId::BATTLEGROUND_AA ? BattlegroundTypeId::BATTLEGROUND_TYPE_NONE : bgTypeId);
    if (!bg)
    {
        if (action)
        {
            SF_LOG_DEBUG("bg.battleground", "CMSG_BATTLEFIELD_PORT %s Slot: %u, Unk: %u, Time: %u, Action: %u. Cant find BG with id %u!",
                GetPlayerInfo().c_str(), queueSlot, id, time, action, ginfo.IsInvitedToBGInstanceGUID);
            return;
        }

        bg = sBattlegroundMgr->GetBattlegroundTemplate(bgTypeId);
        if (!bg)
        {
            SF_LOG_ERROR("network", "BattlegroundHandler: bg_template not found for type id %u.", uint32(bgTypeId));
            return;
        }
    }

    SF_LOG_DEBUG("bg.battleground", "CMSG_BATTLEFIELD_PORT %s Slot: %u, Unk: %u, Time: %u, Action: %u.",
        GetPlayerInfo().c_str(), queueSlot, id, time, action);

    // get real bg type
    bgTypeId = bg->GetTypeID();

    // expected bracket entry
    PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), _player->getLevel());
    if (!bracketEntry)
        return;

    //some checks if player isn't cheating - it is not exactly cheating, but we cannot allow it
    if (action == 1 && ginfo.ArenaType == 0)
    {
        //if player is trying to enter battleground (not arena!) and he has deserter debuff, we must just remove him from queue
        if (!_player->CanJoinToBattleground(bg))
        {
            //send bg command result to show nice message
            WorldPacket data2;
            sBattlegroundMgr->BuildStatusFailedPacket(&data2, bg, _player, 0, GroupJoinBattlegroundResult::ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS);
            _player->GetSession()->SendPacket(&data2);
            action = 0;
            SF_LOG_DEBUG("bg.battleground", "Player %s (%u) has a deserter debuff, do not port him to battleground!", _player->GetName().c_str(), _player->GetGUIDLow());
        }
        //if player don't match battleground max level, then do not allow him to enter! (this might happen when player leveled up during his waiting in queue
        if (_player->getLevel() > bg->GetMaxLevel())
        {
            SF_LOG_DEBUG("network", "Player %s (%u) has level (%u) higher than maxlevel (%u) of battleground (%u)! Do not port him to battleground!",
                _player->GetName().c_str(), _player->GetGUIDLow(), _player->getLevel(), bg->GetMaxLevel(), uint32(bg->GetTypeID()));
            action = 0;
        }
    }

    WorldPacket data;
    if (action)
    {
        if (!_player->IsInvitedForBattlegroundQueueType(bgQueueTypeId))
            return;                                 // cheating?

        if (!_player->InBattleground())
            _player->SetBattlegroundEntryPoint();

        // resurrect the player
        if (!_player->IsAlive())
        {
            _player->ResurrectPlayer(1.0f);
            _player->SpawnCorpseBones();
        }
        // stop taxi flight at port
        if (_player->IsInFlight())
        {
            _player->GetMotionMaster()->MovementExpired();
            _player->CleanupAfterTaxiFlight();
        }

        sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, _player, queueSlot, STATUS_IN_PROGRESS, _player->GetBattlegroundQueueJoinTime(bgTypeId), bg->GetElapsedTime(), bg->GetArenaType());
        _player->GetSession()->SendPacket(&data);

        // remove battleground queue status from BGmgr
        bgQueue.RemovePlayer(_player->GetGUID(), false);
        // this is still needed here if battleground "jumping" shouldn't add deserter debuff
        // also this is required to prevent stuck at old battleground after SetBattlegroundId set to new
        if (Battleground* currentBg = _player->GetBattleground())
            currentBg->RemovePlayerAtLeave(_player->GetGUID(), false, true);

        // set the destination instance id
        _player->SetBattlegroundId(bg->GetInstanceID(), bgTypeId);
        // set the destination team
        _player->SetBGTeam(ginfo.Team);

        // bg->HandleBeforeTeleportToBattleground(_player);
        sBattlegroundMgr->SendToBattleground(_player, ginfo.IsInvitedToBGInstanceGUID, bgTypeId);
        // add only in HandleMoveWorldPortAck()
        // bg->AddPlayer(_player, team);
        SF_LOG_DEBUG("bg.battleground", "Battleground: player %s (%u) joined battle for bg %u, bgtype %u, queue type %u.", _player->GetName().c_str(), _player->GetGUIDLow(), bg->GetInstanceID(), uint32(bg->GetTypeID()), bgQueueTypeId);
    }
    else // leave queue
    {
        // if player leaves rated arena match before match start, it is counted as he played but he lost
        if (ginfo.IsRated && ginfo.IsInvitedToBGInstanceGUID)
        {
            /*
            ArenaTeam* at = sArenaTeamMgr->GetArenaTeamById(ginfo.Team);
            if (at)
            {
                SF_LOG_DEBUG("bg.battleground", "UPDATING memberLost's personal arena rating for %u by opponents rating: %u, because he has left queue!", GUID_LOPART(_player->GetGUID()), ginfo.OpponentsTeamRating);
                at->MemberLost(_player, ginfo.OpponentsMatchmakerRating);
                at->SaveToDB();
            }*/
        }
        sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, _player, queueSlot, STATUS_NONE, _player->GetBattlegroundQueueJoinTime(bgTypeId), 0, 0);
        SendPacket(&data);

        _player->RemoveBattlegroundQueueId(bgQueueTypeId);  // must be called this way, because if you move this call to queue->removeplayer, it causes bugs
        bgQueue.RemovePlayer(_player->GetGUID(), true);
        // player left queue, we should update it - do not update Arena Queue
        if (!ginfo.ArenaType)
            sBattlegroundMgr->ScheduleQueueUpdate(ginfo.ArenaMatchmakerRating, ginfo.ArenaType, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());

        SF_LOG_DEBUG("bg.battleground", "Battleground: player %s (%u) left queue for bgtype %u, queue type %u.", _player->GetName().c_str(), _player->GetGUIDLow(), uint32(bg->GetTypeID()), bgQueueTypeId);
    }
}

void WorldSession::HandleBattlefieldLeaveOpcode(WorldPacket& /*recvData*/)
{
    SF_LOG_DEBUG("network", "WORLD: Recvd CMSG_BATTLEFIELD_LEAVE Message");

    // not allow leave battleground in combat
    if (_player->IsInCombat())
        if (Battleground* bg = _player->GetBattleground())
            if (bg->GetStatus() != STATUS_WAIT_LEAVE)
                return;

    _player->LeaveBattleground();
}

void WorldSession::HandleBattlefieldStatusOpcode(WorldPacket & /*recvData*/)
{
    // empty opcode
    SF_LOG_DEBUG("network", "WORLD: Recvd CMSG_BATTLEFIELD_STATUS Message");

    WorldPacket data;
    // we must update all queues here
    Battleground* bg = NULL;
    for (uint8 i = 0; i < PLAYER_MAX_BATTLEGROUND_QUEUES; ++i)
    {
        BattlegroundQueueTypeId bgQueueTypeId = _player->GetBattlegroundQueueTypeId(i);
        if (!bgQueueTypeId)
            continue;
        BattlegroundTypeId bgTypeId = BattlegroundMgr::BGTemplateId(bgQueueTypeId);
        uint8 arenaType = BattlegroundMgr::BGArenaType(bgQueueTypeId);
        if (bgTypeId == _player->GetBattlegroundTypeId())
        {
            bg = _player->GetBattleground();
            //i cannot check any variable from player class because player class doesn't know if player is in 2v2 / 3v3 or 5v5 arena
            //so i must use bg pointer to get that information
            if (bg && bg->GetArenaType() == arenaType)
            {
                // this line is checked, i only don't know if GetElapsedTime() is changing itself after bg end!
                // send status in Battleground
                sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, GetPlayer(), i, STATUS_IN_PROGRESS, _player->GetBattlegroundQueueJoinTime(bgTypeId), bg->GetElapsedTime(), arenaType);
                SendPacket(&data);
                continue;
            }
        }

        //we are sending update to player about queue - he can be invited there!
        //get GroupQueueInfo for queue status
        BattlegroundQueue& bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);
        GroupQueueInfo ginfo;
        if (!bgQueue.GetPlayerGroupInfoData(_player->GetGUID(), &ginfo))
            continue;
        if (ginfo.IsInvitedToBGInstanceGUID)
        {
            bg = sBattlegroundMgr->GetBattleground(ginfo.IsInvitedToBGInstanceGUID, bgTypeId);
            if (!bg)
                continue;

            // send status invited to Battleground
            sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, GetPlayer(), i, STATUS_WAIT_JOIN, getMSTimeDiff(getMSTime(), ginfo.RemoveInviteTime), _player->GetBattlegroundQueueJoinTime(bgTypeId), arenaType);
            SendPacket(&data);
        }
        else
        {
            bg = sBattlegroundMgr->GetBattlegroundTemplate(bgTypeId);
            if (!bg)
                continue;

            // expected bracket entry
            PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), _player->getLevel());
            if (!bracketEntry)
                continue;

            uint32 avgTime = bgQueue.GetAverageQueueWaitTime(&ginfo, bracketEntry->GetBracketId());
            // send status in Battleground Queue
            sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, GetPlayer(), i, STATUS_WAIT_QUEUE, avgTime, _player->GetBattlegroundQueueJoinTime(bgTypeId), arenaType);
            SendPacket(&data);
        }
    }
}

void WorldSession::HandleBattlemasterJoinArena(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_BATTLEMASTER_JOIN_ARENA");

    uint8 arenaslot;                                        // 2v2, 3v3 or 5v5

    recvData >> arenaslot;

    // ignore if we already in BG or BG queue
    if (_player->InBattleground())
        return;

    uint32 arenaRating = 0;
    uint32 matchmakerRating = 0;

    uint8 arenatype = ArenaTeam::GetTypeBySlot(arenaslot);

    //check existance
    Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(BattlegroundTypeId::BATTLEGROUND_AA);
    if (!bg)
    {
        SF_LOG_ERROR("network", "Battleground: template bg (all arenas) not found");
        return;
    }

    if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, uint32(BattlegroundTypeId::BATTLEGROUND_AA), NULL))
    {
        ChatHandler(this).PSendSysMessage(LANG_ARENA_DISABLED);
        return;
    }

    BattlegroundTypeId bgTypeId = bg->GetTypeID();
    BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, arenatype);
    PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), _player->getLevel());
    if (!bracketEntry)
        return;

    GroupJoinBattlegroundResult err = GroupJoinBattlegroundResult::ERR_BATTLEGROUND_NONE;

    Group* grp = _player->GetGroup();
    // no group found, error
    if (!grp)
        return;
    if (grp->GetLeaderGUID() != _player->GetGUID())
        return;

    /*
    uint32 ateamId = _player->GetArenaTeamId(arenaslot);
    // check real arenateam existence only here (if it was moved to group->CanJoin .. () then we would ahve to get it twice)
    ArenaTeam* at = sArenaTeamMgr->GetArenaTeamById(ateamId);
    if (!at)
    {
        _player->GetSession()->SendNotInArenaTeamPacket(arenatype);
        return;
    }
    */

    arenaRating = 1;
    matchmakerRating = 1;

    BattlegroundQueue &bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);

    uint32 avgTime = 0;
    GroupQueueInfo* ginfo = NULL;

    err = grp->CanJoinBattlegroundQueue(bg, bgQueueTypeId, arenatype, arenatype, true, arenaslot);
    if (err == GroupJoinBattlegroundResult::ERR_BATTLEGROUND_NONE)
    {
        SF_LOG_DEBUG("bg.battleground", "Battleground: leader %s queued with matchmaker rating %u for type %u", _player->GetName().c_str(), matchmakerRating, arenatype);

        ginfo = bgQueue.AddGroup(_player, grp, bgTypeId, bracketEntry, arenatype, true, false, arenaRating, matchmakerRating);
        avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());
    }

    for (GroupReference* itr = grp->GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* member = itr->GetSource();
        if (!member)
            continue;

        if (err != GroupJoinBattlegroundResult::ERR_BATTLEGROUND_NONE)
        {
            WorldPacket data;
            sBattlegroundMgr->BuildStatusFailedPacket(&data, bg, _player, 0, err);
            member->GetSession()->SendPacket(&data);
            continue;
        }

        // add to queue
        uint32 queueSlot = member->AddBattlegroundQueueId(bgQueueTypeId);

        // add joined time data
        member->AddBattlegroundQueueJoinTime(bgTypeId, ginfo->JoinTime);

        WorldPacket data; // send status packet (in queue)
        sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, member, queueSlot, STATUS_WAIT_QUEUE, avgTime, ginfo->JoinTime, arenatype);
        member->GetSession()->SendPacket(&data);

        SF_LOG_DEBUG("bg.battleground", "Battleground: player joined queue for arena as group bg queue type %u bg type %u: GUID %u, NAME %s", bgQueueTypeId, uint32(bgTypeId), member->GetGUIDLow(), member->GetName().c_str());
    }

    sBattlegroundMgr->ScheduleQueueUpdate(matchmakerRating, arenatype, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());
}

void WorldSession::HandleReportPvPAFK(WorldPacket& recvData)
{
    ObjectGuid playerGuid;

    playerGuid[3] = recvData.ReadBit();
    playerGuid[6] = recvData.ReadBit();
    playerGuid[1] = recvData.ReadBit();
    playerGuid[4] = recvData.ReadBit();
    playerGuid[5] = recvData.ReadBit();
    playerGuid[0] = recvData.ReadBit();
    playerGuid[7] = recvData.ReadBit();
    playerGuid[2] = recvData.ReadBit();

    recvData.ReadByteSeq(playerGuid[3]);
    recvData.ReadByteSeq(playerGuid[0]);
    recvData.ReadByteSeq(playerGuid[4]);
    recvData.ReadByteSeq(playerGuid[1]);
    recvData.ReadByteSeq(playerGuid[7]);
    recvData.ReadByteSeq(playerGuid[6]);
    recvData.ReadByteSeq(playerGuid[2]);
    recvData.ReadByteSeq(playerGuid[5]);

    Player* reportedPlayer = ObjectAccessor::FindPlayer(playerGuid);

    if (!reportedPlayer)
    {
        SF_LOG_DEBUG("bg.battleground", "WorldSession::HandleReportPvPAFK: player not found");
        return;
    }

    SF_LOG_DEBUG("bg.battleground", "WorldSession::HandleReportPvPAFK: %s reported %s", _player->GetName().c_str(), reportedPlayer->GetName().c_str());

    reportedPlayer->ReportedAfkBy(_player);
}

void WorldSession::HandleRequestPvpOptions(WorldPacket& /*recvData*/)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_REQUEST_PVP_OPTIONS_ENABLED");

    // triggers PVP_TYPES_ENABLED lua event
    WorldPacket data(SMSG_PVP_OPTIONS_ENABLED, 1);
    data.WriteBit(1);
    data.WriteBit(1);
    data.WriteBit(1);
    data.WriteBit(1);
    data.WriteBit(1);

    data.FlushBits();

    SendPacket(&data);
}

void WorldSession::HandleRequestPvpReward(WorldPacket& /*recvData*/)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_REQUEST_PVP_REWARDS");

    _player->SendPvpRewards();
}

