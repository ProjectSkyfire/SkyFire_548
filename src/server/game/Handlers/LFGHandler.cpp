/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "LFGMgr.h"
#include "ObjectMgr.h"
#include "Group.h"
#include "Player.h"
#include "Opcodes.h"
#include "WorldPacket.h"
#include "WorldSession.h"

void BuildPlayerLockDungeonBlock(WorldPacket& data, lfg::LfgLockMap const& lock)
{
    for (lfg::LfgLockMap::const_iterator it = lock.begin(); it != lock.end(); ++it)
    {
        data << uint32(0);                                 // Current itemLevel
        data << uint32(it->first);                         // Dungeon entry (id + type)
        data << uint32(0);                                 // Required itemLevel
        data << uint32(it->second);                        // Lock status
    }
}

void BuildPartyLockDungeonBlock(WorldPacket& data, lfg::LfgLockPartyMap const& lockMap)
{
    data.WriteBits(lockMap.size(), 22);
    for (lfg::LfgLockPartyMap::const_iterator it = lockMap.begin(); it != lockMap.end(); ++it)
    {
        bool hasGuid = true;
        ObjectGuid PlayerGUID = it->first;

        data.WriteBit(hasGuid);
        data.WriteGuidMask(PlayerGUID, 3, 6, 0, 5, 2, 7, 4, 1);
        data.WriteBits(it->second.size(), 20);                            // Size of lock dungeons

        data.FlushBits();

        BuildPlayerLockDungeonBlock(data, it->second);
        data.WriteGuidBytes(PlayerGUID, 0, 3, 1, 4, 6, 2, 5, 7);
    }
}

void BuildQuestReward(WorldPacket& data, Quest const* quest, Player* player)
{
    uint8 rewCount = quest->GetRewItemsCount() + quest->GetRewCurrencyCount();

    data << uint32(quest->GetRewMoney());
    data << uint32(quest->XPValue(player));
    data << uint8(rewCount);
    if (rewCount)
    {
        for (uint8 i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
        {
            if (uint32 currencyId = quest->RewardCurrencyId[i])
            {
                data << uint32(currencyId);
                data << uint32(0);
                data << uint32(quest->RewardCurrencyCount[i]);
                data << uint8(1);                                           // Is currency
            }
        }

        for (uint8 i = 0; i < QUEST_REWARDS_COUNT; ++i)
        {
            if (uint32 itemId = quest->RewardItemId[i])
            {
                ItemTemplate const* item = sObjectMgr->GetItemTemplate(itemId);
                data << uint32(itemId);
                data << uint32(item ? item->DisplayInfoID : 0);
                data << uint32(quest->RewardItemIdCount[i]);
                data << uint8(0);                                           // Is currency
            }
        }
    }
}

void WorldSession::HandleLfgJoinOpcode(WorldPacket& recvData)
{
    if (!sLFGMgr->isOptionEnabled(lfg::LFG_OPTION_ENABLE_DUNGEON_FINDER | lfg::LFG_OPTION_ENABLE_RAID_BROWSER) ||
        (GetPlayer()->GetGroup() && GetPlayer()->GetGroup()->GetLeaderGUID() != GetPlayer()->GetGUID() &&
        (GetPlayer()->GetGroup()->GetMembersCount() == MAXGROUPSIZE || !GetPlayer()->GetGroup()->isLFGGroup())))
    {
        recvData.rfinish();
        return;
    }
    uint32 roles;
    bool QueueAsGroup;
    uint8 PartyIndex;

    recvData >> PartyIndex;           // PartyIndex
    for (int32 i = 0; i < 3; ++i)
        recvData.read_skip<uint32>(); // Needs
    recvData >> roles;                // Roles
    uint32 numDungeons = recvData.ReadBits(22);
    uint32 commentLen = recvData.ReadBits(8);
    QueueAsGroup = recvData.ReadBit();        // QueueAsGroup

    if (!numDungeons)
    {
        SF_LOG_DEBUG("lfg", "CMSG_LFD_JOIN %s no dungeons selected", GetPlayerInfo().c_str());
        recvData.rfinish();
        return;
    }

    lfg::LfgDungeonSet newDungeons;
    for (uint32 i = 0; i < numDungeons; ++i)
    {
        uint32 dungeon;
        recvData >> dungeon;
        newDungeons.insert((dungeon & 0x00FFFFFF));        // remove the type from the dungeon entry
    }

    std::string comment = recvData.ReadString(commentLen);

    SF_LOG_DEBUG("lfg", "CMSG_LFD_JOIN %s roles: %u, Dungeons: %u, Comment: %s",
        GetPlayerInfo().c_str(), roles, uint8(newDungeons.size()), comment.c_str());

    if (GetPlayer()->GetGroup())
    {
        if (!GetPlayer()->GetGroup()->RoleCheckAllResponded())
        {
            SendRolePollInform(GetPlayer()->GetObjectGUID(), PartyIndex);
            return;
        }
    }

    sLFGMgr->JoinLfg(GetPlayer(), uint8(roles), newDungeons, comment);
}

void WorldSession::HandleLfgLeaveOpcode(WorldPacket& recvData)
{
    ObjectGuid RequesterGUID;
    Group* group = GetPlayer()->GetGroup();
    recvData.read_skip<uint32>();                          // Always 8
    recvData.read_skip<uint32>();                          // Join date
    recvData.read_skip<uint32>();                          // Always 3
    recvData.read_skip<uint32>();                          // Queue Id


    recvData.ReadGuidMask(RequesterGUID, 6, 0, 2, 3, 1, 5, 4, 7);
    recvData.ReadGuidBytes(RequesterGUID, 2, 0, 4, 6, 3, 1, 5, 7);

    // Check cheating - only leader can leave the queue
    if (!group || group->GetLeaderGUID() == uint64(RequesterGUID))
        sLFGMgr->LeaveLfg(uint64(RequesterGUID));
}

void WorldSession::HandleLfgProposalResultOpcode(WorldPacket& recvData)
{
    uint32 ProposalID;
    bool Accepted;

    ObjectGuid guid1, guid2;

    recvData >> ProposalID;         // ProposalID
    recvData.read_skip<uint32>();   // Id
    recvData.read_skip<uint32>();   // Type
    recvData.read_skip<uint32>();   // UnixTime

    Accepted = recvData.ReadBit();
    recvData.ReadGuidMask(guid1, 6, 0, 2, 4);
    recvData.ReadGuidMask(guid2, 6, 7);
    recvData.ReadGuidMask(guid1, 3);
    recvData.ReadGuidMask(guid2, 4);
    recvData.ReadGuidMask(guid1, 7);
    recvData.ReadGuidMask(guid2, 1);
    recvData.ReadGuidMask(guid1, 5);
    recvData.ReadGuidMask(guid2, 0);
    recvData.ReadGuidMask(guid1, 1);
    recvData.ReadGuidMask(guid2, 2, 3, 5);

    recvData.ReadGuidBytes(guid1, 3, 6, 4, 1);
    recvData.ReadGuidBytes(guid2, 7, 0);
    recvData.ReadGuidBytes(guid1, 7);
    recvData.ReadGuidBytes(guid2, 6);
    recvData.ReadGuidBytes(guid1, 5);
    recvData.ReadGuidBytes(guid2, 3, 1, 5, 4);
    recvData.ReadGuidBytes(guid1, 0, 2);
    recvData.ReadGuidBytes(guid2, 2);

    SF_LOG_DEBUG("lfg", "CMSG_LFD_PROPOSAL_RESULT %s proposal: %u accept: %u",
        GetPlayerInfo().c_str(), ProposalID, Accepted ? 1 : 0);
    sLFGMgr->UpdateProposal(ProposalID, GetPlayer()->GetGUID(), Accepted);
}

void WorldSession::HandleLfgSetRolesOpcode(WorldPacket& recvData)
{
    uint32 roles = recvData.read<uint32>();
    recvData.read_skip<uint8>();
    uint64 guid = GetPlayer()->GetGUID();
    Group* group = GetPlayer()->GetGroup();
    if (!group)
    {
        SF_LOG_DEBUG("lfg", "CMSG_LFG_SET_ROLES %s Not in group",
            GetPlayerInfo().c_str());
        return;
    }
    uint64 gguid = group->GetGUID();
    SF_LOG_DEBUG("lfg", "CMSG_LFG_SET_ROLES: Group %u, Player %s, Roles: %u",
        GUID_LOPART(gguid), GetPlayerInfo().c_str(), roles);
    sLFGMgr->UpdateRoleCheck(gguid, guid, roles);
}

void WorldSession::HandleLfgSetCommentOpcode(WorldPacket&  recvData)
{
    std::string comment;
    recvData >> comment;

    SF_LOG_DEBUG("lfg", "CMSG_LFG_SET_COMMENT %s comment: %s",
        GetPlayerInfo().c_str(), comment.c_str());

    sLFGMgr->SetComment(GetPlayer()->GetGUID(), comment);
}

void WorldSession::HandleLFDSetBootVoteOpcode(WorldPacket& recvData)
{
    bool agree;                                            // Agree to kick player
    recvData >> agree;

    uint64 guid = GetPlayer()->GetGUID();
    SF_LOG_DEBUG("lfg", "CMSG_LFD_SET_BOOT_VOTE %s agree: %u",
        GetPlayerInfo().c_str(), agree ? 1 : 0);
    sLFGMgr->UpdateBoot(guid, agree);
}

void WorldSession::HandleLFDTeleportOpcode(WorldPacket& recvData)
{
    bool out;
    recvData >> out;

    SF_LOG_DEBUG("lfg", "CMSG_LFD_TELEPORT %s out: %u",
        GetPlayerInfo().c_str(), out ? 1 : 0);
    sLFGMgr->TeleportPlayer(GetPlayer(), out, true);
}

void WorldSession::HandleLFDGetLockInfoOpcode(WorldPacket& recvData)
{
    uint8 partyIndex = 0;
    recvData >> partyIndex; // partyIndex NYI
    bool forPlayer = recvData.ReadBit();
    SF_LOG_DEBUG("lfg", "CMSG_LFD_LOCK_INFO_REQUEST %s for %s", GetPlayerInfo().c_str(), (forPlayer ? "player" : "party"));

    if (forPlayer)
        SendLfgPlayerLockInfo();
    else
        SendLfgPartyLockInfo();
}

void WorldSession::SendLfgPlayerLockInfo()
{
    uint64 guid = GetPlayer()->GetGUID();

    // Get Random dungeons that can be done at a certain level and expansion
    uint8 level = GetPlayer()->getLevel();
    lfg::LfgDungeonSet const& randomDungeons =
        sLFGMgr->GetRandomAndSeasonalDungeons(level, GetPlayer()->GetSession()->Expansion());

    // Get player locked Dungeons
    lfg::LfgLockMap const& lock = sLFGMgr->GetLockedDungeons(guid);
    uint32 rsize = uint32(randomDungeons.size());
    uint32 lsize = uint32(lock.size());

    SF_LOG_DEBUG("lfg", "SMSG_LFD_PLAYER_INFO %s", GetPlayerInfo().c_str());
    WorldPacket data(SMSG_LFD_PLAYER_INFO, 1 + rsize * (4 + 1 + 4 + 4 + 4 + 4 + 1 + 4 + 4 + 4) + 4 + lsize * (1 + 4 + 4 + 4 + 4 + 1 + 4 + 4 + 4));

    data.WriteBits(lock.size(), 20); // locksize count
    data.WriteBit(0);                // hasPlayerGuid
    data.WriteBits(randomDungeons.size(), 17);


    for (lfg::LfgDungeonSet::const_iterator it = randomDungeons.begin(); it != randomDungeons.end(); ++it)
    {
        data.WriteBit(0); // ShortageEligible
        data.WriteBit(0); // FirstReward
        data.WriteBits(0, 21);
        data.WriteBits(0, 19);
        data.WriteBits(0, 20);
        //forloop {} // 64
        
        data.WriteBits(0, 21);
    }
    // if (hasPlayerGuid) {}
    data.FlushBits();
    // if (hasPlayerGuid) {}

    for (lfg::LfgDungeonSet::const_iterator it = randomDungeons.begin(); it != randomDungeons.end(); ++it)
    {
        lfg::LfgReward const* reward = sLFGMgr->GetRandomDungeonReward(*it, level);
        Quest const* quest = NULL;
        bool done = false;
        if (reward)
        {
            quest = sObjectMgr->GetQuestTemplate(reward->firstQuest);
            if (quest)
            {
                done = !GetPlayer()->CanRewardQuest(quest, false);
                if (done)
                    quest = sObjectMgr->GetQuestTemplate(reward->otherQuest);
            }
        }

        data << uint32(0); // RewardXP
        //forloop {} // ShortageReward
        data << uint32(0); // SpecificQuantity
        //forloop {} // BonusCurrency
        data << uint32(0); // PurseLimit
        data << uint32(0); // RewardMoney
        //forloop {} // Item
        data << uint32(0); // OverallQuantity
        data << uint32(0); // PurseWeeklyQuantity
        data << uint32(0); // OverallLimit
        data << uint32(0); // Quantity
        data << uint32(0); // CompletionCurrencyID
        data << uint32(*it); // Dungeon Entry (id + type)
        //forloop {} // Currency
        data << uint32(0); // PurseWeeklyLimit
        data << uint32(0); // Mask
        data << uint32(0); // PurseQuantity
        data << uint32(0); // CompletionLimit
        data << uint32(0); // SpecificLimit
        data << uint32(0); // CompletedMask
        data << uint32(0); // CompletionQuantity
    }

    for (lfg::LfgLockMap::const_iterator it = lock.begin(); it != lock.end(); ++it)
    {
        data << uint32(it->first);                         // Dungeon entry (id + type)
        data << uint32(it->second);                        // Lock status
        data << uint32(0);                                 // Current itemLevel
        data << uint32(0);                                 // Required itemLevel
    }
    SendPacket(&data);
}

void WorldSession::SendLfgPartyLockInfo()
{
    uint64 guid = GetPlayer()->GetGUID();
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    // Get the locked dungeons of the other party members
    lfg::LfgLockPartyMap lockMap;
    for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* plrg = itr->GetSource();
        if (!plrg)
            continue;

        uint64 pguid = plrg->GetGUID();
        if (pguid == guid)
            continue;

        lockMap[pguid] = sLFGMgr->GetLockedDungeons(pguid);
    }

    uint32 size = 0;
    for (lfg::LfgLockPartyMap::const_iterator it = lockMap.begin(); it != lockMap.end(); ++it)
        size += 8 + 4 + uint32(it->second.size()) * (4 + 4 + 4 + 4);

    SF_LOG_DEBUG("lfg", "SMSG_LFD_PARTY_INFO %s", GetPlayerInfo().c_str());
    WorldPacket data(SMSG_LFD_PARTY_INFO, 1 + size);
    BuildPartyLockDungeonBlock(data, lockMap);
    SendPacket(&data);
}

void WorldSession::HandleLfrJoinOpcode(WorldPacket& recvData)
{
    uint32 entry;                                          // Raid id to search
    recvData >> entry;
    SF_LOG_DEBUG("lfg", "CMSG_LFG_LFR_JOIN %s dungeon entry: %u",
        GetPlayerInfo().c_str(), entry);
    //SendLfrUpdateListOpcode(entry);
}

void WorldSession::HandleLfrLeaveOpcode(WorldPacket& recvData)
{
    uint32 dungeonId;                                      // Raid id queue to leave
    recvData >> dungeonId;
    SF_LOG_DEBUG("lfg", "CMSG_LFG_LFR_LEAVE %s dungeonId: %u",
        GetPlayerInfo().c_str(), dungeonId);
    //sLFGMgr->LeaveLfr(GetPlayer(), dungeonId);
}

void WorldSession::HandleLfgGetStatus(WorldPacket& /*recvData*/)
{
    SF_LOG_DEBUG("lfg", "CMSG_LFG_GET_STATUS %s", GetPlayerInfo().c_str());

    if (!GetPlayer()->isUsingLfg())
        return;

    uint64 guid = GetPlayer()->GetGUID();
    lfg::LfgUpdateData updateData = sLFGMgr->GetLfgStatus(guid);

    if (GetPlayer()->GetGroup())
    {
        SendLfgUpdateStatus(updateData, true);
        updateData.dungeons.clear();
        SendLfgUpdateStatus(updateData, false);
    }
    else
    {
        SendLfgUpdateStatus(updateData, false);
        updateData.dungeons.clear();
        SendLfgUpdateStatus(updateData, true);
    }
}

void WorldSession::SendLfgUpdateStatus(lfg::LfgUpdateData const& updateData, bool party)
{
    bool join = false;
    bool queued = false;
    uint8 size = uint8(updateData.dungeons.size());
    ObjectGuid guid = _player->GetGUID();
    time_t joinTime = sLFGMgr->GetQueueJoinTime(_player->GetGUID());
    uint32 queueId = sLFGMgr->GetQueueId(_player->GetGUID());
    bool lfgjoined = updateData.updateType != lfg::LFG_UPDATETYPE_REMOVED_FROM_QUEUE;

    switch (updateData.updateType)
    {
        case lfg::LFG_UPDATETYPE_JOIN_QUEUE_INITIAL:            // Joined queue outside the dungeon
            join = true;
            break;
        case lfg::LFG_UPDATETYPE_JOIN_QUEUE:
        case lfg::LFG_UPDATETYPE_ADDED_TO_QUEUE:                // Rolecheck Success
            join = true;
            queued = true;
            break;
        case lfg::LFG_UPDATETYPE_PROPOSAL_BEGIN:
            join = true;
            break;
        case lfg::LFG_UPDATETYPE_UPDATE_STATUS:
            join = updateData.state != lfg::LFG_STATE_NONE;
            queued = updateData.state == lfg::LFG_STATE_QUEUED;
            break;
        default:
            break;
    }

    SF_LOG_DEBUG("lfg", "SMSG_LFD_UPDATE_STATUS %s updatetype: %u, party %s",
        GetPlayerInfo().c_str(), updateData.updateType, party ? "true" : "false");

    WorldPacket data(SMSG_LFD_UPDATE_STATUS, 1 + 8 + 3 + 2 + 1 + updateData.comment.length() + 4 + 4 + 1 + 1 + 1 + 4 + size);

    data.WriteBits(updateData.comment.length(), 8);       // CommentLen
    data.WriteBit(party);                                 // IsParty
    data.WriteBit(join);                                  // Joined
    data.WriteBits(size, 22);                             // Slots
    data.WriteGuidMask(guid, 2, 3, 1);
    data.WriteBit(true);                                  // NotifyUI
    data.WriteGuidMask(guid, 7, 6, 0);
    data.WriteBit(lfgjoined); // LfgJoined
    data.WriteBit(queued);                                // Queued
    data.WriteBits(0, 24);                                // SuspendedPlayers
    data.WriteGuidMask(guid, 5);
    //forloop 75 SuspendedPlayers
    data.WriteGuidMask(guid, 4);

    data.FlushBits();

    data.WriteGuidBytes(guid, 3);
    for (uint8 i = 0; i < 3; ++i)
        data << uint8(0);                                 // Needs

    data.WriteGuidBytes(guid, 4);
    //forloop 75 SuspendedPlayers
    data.WriteGuidBytes(guid, 6);
    data << uint8(0);                                      // SubType
    data << uint32(sLFGMgr->GetRoles(_player->GetGUID())); // RequestedRoles
    data << uint32(queueId);                              // Id
    data.WriteGuidBytes(guid, 5);
    data.WriteString(updateData.comment);                 // Comment
    data.WriteGuidBytes(guid, 2);
    for (lfg::LfgDungeonSet::const_iterator it = updateData.dungeons.begin(); it != updateData.dungeons.end(); ++it)
        data << uint32(*it);
    data.WriteGuidBytes(guid, 0, 1);
    data << uint32(joinTime);                             // UnixTime
    data << uint8(updateData.updateType);                 // Reason
    data << uint32(3);                                    // Type
    data.WriteGuidBytes(guid, 7);
    SendPacket(&data);
}

void WorldSession::SendLfgRoleChosen(uint64 guid, uint8 roles)
{
    SF_LOG_DEBUG("lfg", "SMSG_LFG_ROLE_CHOSEN %s guid: %u roles: %u",
        GetPlayerInfo().c_str(), GUID_LOPART(guid), roles);

    ObjectGuid objGuid = guid;
    WorldPacket data(SMSG_LFG_ROLE_CHOSEN, 8 + 1 + 4);
    data.WriteGuidMask(objGuid, 6, 2, 1, 7, 0);
    data.WriteBit(roles > 0);   // Accepted
    data.WriteGuidMask(objGuid, 3, 5, 4);
    data.FlushBits();

    data.WriteGuidBytes(objGuid, 0, 3, 6);
    data << uint32(roles);      // RoleMask
    data.WriteGuidBytes(objGuid, 5, 1, 4, 2, 7);
    SendPacket(&data);
}

void WorldSession::SendLfgRoleCheckUpdate(lfg::LfgRoleCheck const& roleCheck)
{
    lfg::LfgDungeonSet dungeons;
    if (roleCheck.rDungeonId)
        dungeons.insert(roleCheck.rDungeonId);
    else
        dungeons = roleCheck.dungeons;

    ObjectGuid guid = roleCheck.leader;
    uint8 roles = roleCheck.roles.find(guid)->second;
    Player* player = ObjectAccessor::FindPlayer(guid);

    SF_LOG_DEBUG("lfg", "SMSG_LFD_ROLE_CHECK_UPDATE %s", GetPlayerInfo().c_str());
    WorldPacket data(SMSG_LFD_ROLE_CHECK_UPDATE, 4 + 1 + 1 + dungeons.size() * 4 + 1 + roleCheck.roles.size() * (8 + 1 + 4 + 1));

    data << uint8(roleCheck.state); // RoleCheckStatus
    data << uint8(0); // PartyIndex
    data.WriteBits(roleCheck.roles.size(), 21); // Members
    if (!roleCheck.roles.empty())
    {
        // Leader info MUST be sent 1st :S        
        data.WriteBit(roleCheck.state == lfg::LFG_ROLECHECK_FINISHED); // RoleCheckComplete
        data.WriteGuidMask(guid, 3, 0, 5, 2, 7, 1, 4, 6);

        for (lfg::LfgRolesMap::const_iterator it = roleCheck.roles.begin(); it != roleCheck.roles.end(); ++it)
        {
            if (it->first == roleCheck.leader)
                continue;

            guid = it->first;
            data.WriteBit(roleCheck.state == lfg::LFG_ROLECHECK_FINISHED); // RoleCheckComplete
            data.WriteGuidMask(guid, 3, 0, 5, 2, 7, 1, 4, 6);
        }
    }
    data.WriteGuidMask(guid, 3, 5);
    data.WriteBits(dungeons.size(), 22); // JoinSlots
    data.WriteGuidMask(guid, 0, 7, 6, 1, 4, 2);
    data.WriteBit(roleCheck.state == lfg::LFG_ROLECHECK_INITIALITING); // IsBeginning

    data.FlushBits();

    data.WriteGuidBytes(guid, 0);
    if (!roleCheck.roles.empty())
    {
        // Leader info MUST be sent 1st :S        
        data << uint8(player ? player->getLevel() : 0); // Level
        data.WriteGuidBytes(guid, 3, 6);
        data << uint32(roles); // RolesDesired
        data.WriteGuidBytes(guid, 2, 4, 0, 1, 5, 7);

        for (lfg::LfgRolesMap::const_iterator it = roleCheck.roles.begin(); it != roleCheck.roles.end(); ++it)
        {
            if (it->first == roleCheck.leader)
                continue;

            guid = it->first;
            data << uint8(player ? player->getLevel() : 0); // Level
            data.WriteGuidBytes(guid, 3, 6);
            data << uint32(roles);                          // RolesDesired
            data.WriteGuidBytes(guid, 2, 4, 0, 1, 5, 7);
        }
    }
    data.WriteGuidBytes(guid, 1, 7, 6, 4, 3, 2, 5);
    if (!dungeons.empty())
        for (lfg::LfgDungeonSet::iterator it = dungeons.begin(); it != dungeons.end(); ++it)
            data << uint32(sLFGMgr->GetLFGDungeonEntry(*it)); // Dungeon

    SendPacket(&data);
}

void WorldSession::SendLfgJoinResult(lfg::LfgJoinResultData const& joinData)
{
    uint32 size = 0;
    ObjectGuid guid = GetPlayer()->GetGUID();
    uint32 queueId = sLFGMgr->GetQueueId(_player->GetGUID());
    for (lfg::LfgLockPartyMap::const_iterator it = joinData.lockmap.begin(); it != joinData.lockmap.end(); ++it)
        size += 8 + 4 + uint32(it->second.size()) * (4 + 4 + 4 + 4);

    SF_LOG_DEBUG("lfg", "SMSG_LFD_JOIN_RESULT %s checkResult: %u checkValue: %u",
        GetPlayerInfo().c_str(), joinData.result, joinData.state);

    WorldPacket data(SMSG_LFD_JOIN_RESULT, 4 + 4 + size);

    data.WriteGuidMask(guid, 7, 6, 3, 0);
    data.WriteBits(joinData.lockmap.size(), 22); // BlackList
    
    for (lfg::LfgLockPartyMap::const_iterator it = joinData.lockmap.begin(); it != joinData.lockmap.end(); ++it)
    {
        ObjectGuid playerGuid = it->first;
        data.WriteGuidMask(playerGuid, 3);
        data.WriteBits(it->second.size(), 20); // Slots
        data.WriteGuidMask(playerGuid, 6, 1, 4, 7, 2, 0, 5);
    }

    data.WriteGuidMask(guid, 5, 1, 4, 2);
    data.FlushBits();
    data << uint8(joinData.result); // Result

    for (lfg::LfgLockPartyMap::const_iterator it = joinData.lockmap.begin(); it != joinData.lockmap.end(); ++it)
    {
        ObjectGuid playerGuid = it->first;
        data.WriteGuidBytes(playerGuid, 4);

        for (lfg::LfgLockMap::const_iterator itr = it->second.begin(); itr != it->second.end(); ++itr)
        {
            data << uint32(0);                                 // SubReason2
            data << uint32(0);                                 // SubReason1
            data << uint32(itr->second);                       // Reason
            data << uint32(itr->first);                        // Slot
        }
        data.WriteGuidBytes(playerGuid, 1, 0, 5, 7, 3, 6, 2);
    }

    data << uint8(joinData.state);  // ResultDetail
    data.WriteGuidBytes(guid, 2);
    data << uint32(time(NULL));     // UnixTime
    data << uint32(queueId);        // Id
    data << uint32(3);              // Type
    data.WriteGuidBytes(guid, 6, 4, 1, 0, 5, 7, 3);
    SendPacket(&data);
}

void WorldSession::SendLfgQueueStatus(lfg::LfgQueueStatusData const& queueData)
{
    SF_LOG_DEBUG("lfg", "SMSG_LFD_QUEUE_STATUS %s dungeon: %u, waitTime: %d, "
        "avgWaitTime: %d, waitTimeTanks: %d, waitTimeHealer: %d, waitTimeDps: %d, "
        "queuedTime: %u, tanks: %u, healers: %u, dps: %u",
        GetPlayerInfo().c_str(), queueData.dungeonId, queueData.waitTime, queueData.waitTimeAvg,
        queueData.waitTimeTank, queueData.waitTimeHealer, queueData.waitTimeDps,
        queueData.queuedTime, queueData.tanks, queueData.healers, queueData.dps);

    ObjectGuid guid = _player->GetGUID();
    WorldPacket data(SMSG_LFD_QUEUE_STATUS, 8+(10*4)+(3*1));
    data.WriteGuidMask(guid, 4, 3, 5, 1, 2, 0, 6, 7);
    data << uint32(queueData.dungeonId);                   // Dungeon
    data.WriteGuidBytes(guid, 0);
    data << uint32(queueData.queuedTime);                  // Player wait time in queue
    data.WriteGuidBytes(guid, 4);
    data << uint32(queueData.joinTime);                    // Join time
    data << int32(queueData.waitTimeTank);                 // Wait Tanks
    data << uint8(queueData.tanks);                        // Tanks needed
    data << int32(queueData.waitTimeHealer);               // Wait Healers
    data << uint8(queueData.healers);                      // Healers needed
    data << int32(queueData.waitTimeDps);                  // Wait Dps
    data << uint8(queueData.dps);                          // Dps needed
    data << int32(queueData.waitTime);                     // Wait Time
    data << int32(queueData.waitTimeAvg);                  // Average Wait time
    data.WriteGuidBytes(guid, 1);
    data << uint32(queueData.queueId);                     // Queue Id
    data.WriteGuidBytes(guid, 7, 2);
    data << uint32(3);
    data.WriteGuidBytes(guid, 5, 3, 6);
    SendPacket(&data);
}

void WorldSession::SendLfgPlayerReward(lfg::LfgPlayerRewardData const& rewardData)
{
    if (!rewardData.rdungeonEntry || !rewardData.sdungeonEntry || !rewardData.quest)
        return;

    SF_LOG_DEBUG("lfg", "SMSG_LFG_PLAYER_REWARD %s rdungeonEntry: %u, sdungeonEntry: %u, done: %u",
        GetPlayerInfo().c_str(), rewardData.rdungeonEntry, rewardData.sdungeonEntry, rewardData.done);

    uint8 itemNum = rewardData.quest->GetRewItemsCount() + rewardData.quest->GetRewCurrencyCount();

    WorldPacket data(SMSG_LFG_PLAYER_REWARD, 4 + 4 + 1 + 4 + 4 + 4 + 4 + 4 + 1 + itemNum * (4 + 4 + 4));
    data << uint32(rewardData.rdungeonEntry);                               // Random Dungeon Finished
    data << uint32(rewardData.sdungeonEntry);                               // Dungeon Finished
    BuildQuestReward(data, rewardData.quest, GetPlayer());
    SendPacket(&data);
}

void WorldSession::SendLfgBootProposalUpdate(lfg::LfgPlayerBoot const& boot)
{
    uint64 guid = GetPlayer()->GetGUID();
    lfg::LfgAnswer playerVote = boot.votes.find(guid)->second;
    uint8 votesNum = 0;
    uint8 agreeNum = 0;
    uint32 secsleft = uint8((boot.cancelTime - time(NULL)) / 1000);
    for (lfg::LfgAnswerContainer::const_iterator it = boot.votes.begin(); it != boot.votes.end(); ++it)
    {
        if (it->second != lfg::LFG_ANSWER_PENDING)
        {
            ++votesNum;
            if (it->second == lfg::LFG_ANSWER_AGREE)
                ++agreeNum;
        }
    }
    SF_LOG_DEBUG("lfg", "SMSG_LFG_BOOT_PROPOSAL_UPDATE %s inProgress: %u - "
        "didVote: %u - agree: %u - victim: %u votes: %u - agrees: %u - left: %u - "
        "needed: %u - reason %s",
        GetPlayerInfo().c_str(), uint8(boot.inProgress), uint8(playerVote != lfg::LFG_ANSWER_PENDING),
        uint8(playerVote == lfg::LFG_ANSWER_AGREE), GUID_LOPART(boot.victim), votesNum, agreeNum,
        secsleft, lfg::LFG_GROUP_KICK_VOTES_NEEDED, boot.reason.c_str());
    WorldPacket data(SMSG_LFD_BOOT_PROPOSAL_UPDATE, 1 + 1 + 1 + 1 + 8 + 4 + 4 + 4 + 4 + boot.reason.length());

    ObjectGuid TargetGUID = boot.victim;

    data.WriteBit(boot.reason.size() > 0);
    data.WriteGuidMask(TargetGUID, 3);
    data.WriteBit(playerVote != lfg::LFG_ANSWER_PENDING);        // MyVoteCompleted
    data.WriteBit(agreeNum >= lfg::LFG_GROUP_KICK_VOTES_NEEDED); // VotePassed
    data.WriteBit(playerVote == lfg::LFG_ANSWER_AGREE);          // MyVote
    data.WriteGuidMask(TargetGUID, 6);
    if (boot.reason.size() > 0)
    {
        data.WriteBits(boot.reason.size(), 8);
    }
    data.WriteBit(boot.inProgress);                              // VoteInProgress
    data.WriteGuidMask(TargetGUID, 1, 7, 5, 2, 0, 4);

    data.FlushBits();

    data.WriteGuidBytes(TargetGUID, 2, 4, 3, 6);
    data << uint32(lfg::LFG_GROUP_KICK_VOTES_NEEDED);            // VotesNeeded
    data << uint32(secsleft);                                    // TimeLeft
    if (boot.reason.size() > 0)
    {
        data.WriteString(boot.reason.c_str());
    }
    data.WriteGuidBytes(TargetGUID, 5, 0);
    data << uint32(agreeNum);                                   // BootVotes
    data.WriteGuidBytes(TargetGUID, 7);
    data << uint32(votesNum);                                   // TotalVotes
    data.WriteGuidBytes(TargetGUID, 1);
    SendPacket(&data);
}

void WorldSession::SendLfgUpdateProposal(lfg::LfgProposal const& proposal)
{
    ObjectGuid guid = GetPlayer()->GetGUID();
    ObjectGuid gguid = proposal.players.find(guid)->second.group;
    bool silent = !proposal.isNew && gguid == proposal.group;
    uint32 dungeonEntry = proposal.dungeonId;
    uint32 queueId = sLFGMgr->GetQueueId(_player->GetGUID());
    time_t joinTime = sLFGMgr->GetQueueJoinTime(_player->GetGUID());

    SF_LOG_DEBUG("lfg", "SMSG_LFD_PROPOSAL_UPDATE %s state: %u",
        GetPlayerInfo().c_str(), proposal.state);

    // show random dungeon if player selected random dungeon and it's not lfg group
    if (!silent)
    {
        lfg::LfgDungeonSet const& playerDungeons = sLFGMgr->GetSelectedDungeons(guid);
        if (playerDungeons.find(proposal.dungeonId) == playerDungeons.end())
            dungeonEntry = (*playerDungeons.begin());
    }

    dungeonEntry = sLFGMgr->GetLFGDungeonEntry(dungeonEntry);

    WorldPacket data(SMSG_LFD_PROPOSAL_UPDATE, 4 + 1 + 4 + 4 + 1 + 1 + proposal.players.size() * (4 + 1 + 1 + 1 + 1 +1));
    data.WriteGuidMask(guid, 6, 0);
    data.WriteGuidMask(gguid, 1, 7, 5);
    data.WriteGuidMask(guid, 5);
    data.WriteGuidMask(gguid, 4);
    data.WriteBit(proposal.encounters > 0);             // ValidCompletedMask
    data.WriteGuidMask(guid, 2);
    data.WriteGuidMask(gguid, 6);
    data.WriteGuidMask(guid, 3, 7);
    data.WriteGuidMask(gguid, 3);
    data.WriteBits(proposal.players.size(), 21);        // Players
    for (lfg::LfgProposalPlayerContainer::const_iterator it = proposal.players.begin(); it != proposal.players.end(); ++it)
    {
        data.WriteBit(it->second.group && it->second.group == proposal.group); // MyParty
        data.WriteBit(it->first == guid);                                      // Me
        data.WriteBit(it->second.accept != lfg::LFG_ANSWER_PENDING);           // Responded
        data.WriteBit(it->second.accept == lfg::LFG_ANSWER_AGREE);             // Accepted
        data.WriteBit(it->second.group && it->second.group == gguid);          // SameParty
    }
    data.WriteGuidMask(gguid, 2);
    data.WriteGuidMask(guid, 4);
    data.WriteBit(silent);               // ProposalSilent
    data.WriteGuidMask(gguid, 0);
    data.WriteGuidMask(guid, 1);
    data.FlushBits();
    data.WriteGuidBytes(guid, 1);
    data.WriteGuidBytes(gguid, 4);
    data.WriteGuidBytes(guid, 4);
    data.WriteGuidBytes(gguid, 7);
    data.WriteGuidBytes(guid, 2);
    data.WriteGuidBytes(gguid, 0);
    data << uint32(dungeonEntry);        // Slot
    data << uint8(proposal.state);       // State
    data << uint32(queueId);             // Id
    data.WriteGuidBytes(guid, 6);
    data << uint32(proposal.id);         // ProposalID
    data.WriteGuidBytes(gguid, 5, 3);
    data << uint32(joinTime);            // UnixTime
    data.WriteGuidBytes(guid, 5);
    data.WriteGuidBytes(gguid, 6);
    for (lfg::LfgProposalPlayerContainer::const_iterator it = proposal.players.begin(); it != proposal.players.end(); ++it)
    {
        lfg::LfgProposalPlayer const& player = it->second;
        data << uint32(player.role);     // Role
    }
    data << uint32(proposal.encounters); // CompletedMask
    data.WriteGuidBytes(guid, 7);
    data.WriteGuidBytes(gguid, 1);
    data.WriteGuidBytes(guid, 0, 2);
    data << uint32(3);                   // Type
    data.WriteGuidBytes(guid, 3);
    SendPacket(&data);
}

void WorldSession::SendLfgLfrList(bool update)
{
    SF_LOG_DEBUG("lfg", "SMSG_LFG_LFR_LIST %s update: %u",
        GetPlayerInfo().c_str(), update ? 1 : 0);
    WorldPacket data(SMSG_LFG_UPDATE_SEARCH, 1);
    data << uint8(update);                                 // In Lfg Queue?
    SendPacket(&data);
}

void WorldSession::SendLfgDisabled()
{
    SF_LOG_DEBUG("lfg", "SMSG_LFD_DISABLED %s", GetPlayerInfo().c_str());
    WorldPacket data(SMSG_LFD_DISABLED, 0);
    SendPacket(&data);
}

void WorldSession::SendLfgOfferContinue(uint32 dungeonEntry)
{
    SF_LOG_DEBUG("lfg", "SMSG_LFD_OFFER_CONTINUE %s dungeon entry: %u",
        GetPlayerInfo().c_str(), dungeonEntry);
    WorldPacket data(SMSG_LFD_OFFER_CONTINUE, 4);
    data << uint32(dungeonEntry);
    SendPacket(&data);
}

void WorldSession::SendLfgTeleportError(uint8 err)
{
    SF_LOG_DEBUG("lfg", "SMSG_LFD_TELEPORT_DENIED %s reason: %u",
        GetPlayerInfo().c_str(), err);
    WorldPacket data(SMSG_LFD_TELEPORT_DENIED, 4);
    data.WriteBits(err, 4);                                   // Error
    SendPacket(&data);
}

/*
void WorldSession::SendLfrUpdateListOpcode(uint32 dungeonEntry)
{
    SF_LOG_DEBUG("network", "SMSG_LFG_UPDATE_LIST %s dungeon entry: %u",
        GetPlayerInfo().c_str(), dungeonEntry);
    WorldPacket data(SMSG_LFG_UPDATE_LIST);
    SendPacket(&data);
}
*/
