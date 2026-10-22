/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Group.h"
#include "LFGMgr.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Player.h"
#include "WorldPacket.h"
#include "WorldSession.h"

#include <vector>

namespace
{
    bool HasPacketBytes(WorldPacket const& packet, size_t bytes)
    {
        return packet.rpos() + bytes <= packet.size();
    }

    void SendLfrRemovedFromQueue(WorldSession* session)
    {
        session->SendLfgClearStatus();
    }
}

void BuildPlayerLockDungeonBlock(WorldPacket& data, lfg::LfgLockMap const& lock)
{
    for (lfg::LfgLockMap::const_iterator it = lock.begin(); it != lock.end(); ++it)
    {
        data << uint32(it->second.currentItemLevel);       // Current itemLevel
        data << uint32(it->first);                         // Dungeon entry (id + type)
        data << uint32(it->second.requiredItemLevel);      // Required itemLevel
        data << uint32(it->second.lockStatus);             // Lock status
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
    if (!sLFGMgr->isOptionEnabled(lfg::LFG_OPTION_ENABLE_DUNGEON_FINDER | lfg::LFG_OPTION_ENABLE_RAID_BROWSER))
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

    if (GetPlayer()->GetGroup() && GetPlayer()->GetGroup()->GetLeaderGUID() == GetPlayer()->GetGUID())
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
    Player* player = GetPlayer();
    if (!player)
    {
        recvData.rfinish();
        return;
    }

    Group* group = player->GetGroup();
    uint64 const guid = player->GetGUID();

    // 5.4.8 CMSG_LFD_LEAVE is roles, join time, ticket type (3), queue id, then
    // the requester guid. The eye and the dungeon-finder window both send it.
    // Treating the third field as the join time echoed "3" back at a queued
    // client, which does not match the ticket it stored, so the button looked dead.
    uint32 fields[4] = { 0, 0, 0, 0 };
    bool const haveTicket = HasPacketBytes(recvData, sizeof(fields));
    if (haveTicket)
    {
        for (uint32& field : fields)
            field = recvData.read<uint32>();
    }
    else
    {
        SF_LOG_DEBUG("lfg.leave", "CMSG_LFD_LEAVE %s short packet size: %u",
            GetPlayerInfo().c_str(), uint32(recvData.size()));
        recvData.rfinish();
    }

    uint32 packetJoinTime = 0;
    for (uint32 field : fields)
        if (field > 100000000u)
            packetJoinTime = field;

    if (haveTicket && recvData.rpos() < recvData.size())
    {
        ObjectGuid requester;
        recvData.ReadGuidMask(requester, 6, 0, 2, 3, 1, 5, 4, 7);
        size_t guidBytes = 0;
        for (uint8 i = 0; i < 8; ++i)
            if (requester[i])
                ++guidBytes;
        if (recvData.rpos() + guidBytes <= recvData.size())
            recvData.ReadGuidBytes(requester, 2, 0, 4, 6, 3, 1, 5, 7);
    }
    recvData.rfinish();

    uint64 const queueOwner = group ? group->GetGUID() : guid;
    time_t serverJoin = sLFGMgr->GetQueueJoinTime(queueOwner);
    if (!serverJoin)
        serverJoin = sLFGMgr->GetQueueJoinTime(guid);
    uint8 queueId = sLFGMgr->GetQueueId(queueOwner);
    if (!queueId)
        queueId = sLFGMgr->GetQueueId(guid);
    if (!queueId)
    {
        for (uint32 field : fields)
        {
            if (field && field <= 0xFF && field != 3 && field != packetJoinTime)
            {
                queueId = uint8(field);
                break;
            }
        }
    }

    uint32 const joinTime = packetJoinTime ? packetJoinTime : uint32(serverJoin);

    auto notify = [&](Player* member, uint64 lfgGroupGuid)
    {
        if (!member || !member->GetSession())
            return;
        member->GetSession()->SendLfgClearStatus(lfgGroupGuid, queueId, lfgGroupGuid != 0, joinTime);
    };

    auto clearPlayer = [&](uint64 playerGuid)
    {
        uint8 const playerQueueId = sLFGMgr->GetQueueId(playerGuid);
        sLFGMgr->LeaveSoloLfg(playerGuid, playerQueueId ? playerQueueId : queueId);
        if (sLFGMgr->GetState(playerGuid) != lfg::LFG_STATE_NONE)
            sLFGMgr->LeaveLfg(playerGuid);
    };

    if (!group)
    {
        SF_LOG_DEBUG("lfg.leave", "CMSG_LFD_LEAVE %s left solo queue %u at %u.",
            GetPlayerInfo().c_str(), queueId, joinTime);
        clearPlayer(guid);
        notify(player, 0);
        return;
    }

    uint64 const groupGuid = group->GetGUID();
    lfg::LfgState const groupState = sLFGMgr->GetState(groupGuid);
    bool const inLfgDungeon = group->isLFGGroup() &&
        (groupState == lfg::LFG_STATE_DUNGEON || groupState == lfg::LFG_STATE_FINISHED_DUNGEON ||
            groupState == lfg::LFG_STATE_BOOT || (player->GetMap() && player->GetMap()->IsDungeon()));

    std::vector<Player*> members;
    for (GroupReference* itr = group->GetFirstMember(); itr != nullptr; itr = itr->next())
        if (Player* member = itr->GetSource())
            members.push_back(member);

    if (inLfgDungeon)
    {
        // Leave Instance Group removes this player. The rest of the party stays,
        // and only someone still in the group is offered a replacement queue.
        SF_LOG_DEBUG("lfg.leave", "CMSG_LFD_LEAVE %s left dungeon group %u.",
            GetPlayerInfo().c_str(), GUID_LOPART(groupGuid));
        player->RemoveFromGroup(GROUP_REMOVEMETHOD_LEAVE);
        return;
    }

    // Premade or still forming: drop the whole party, not only the clicker.
    // A member's button used to be ignored, which is the same as a dead click.
    SF_LOG_DEBUG("lfg.leave", "CMSG_LFD_LEAVE %s left group queue %u (%u member(s)) at %u.",
        GetPlayerInfo().c_str(), GUID_LOPART(groupGuid), uint32(members.size()), joinTime);
    sLFGMgr->LeaveLfg(groupGuid);
    for (Player* member : members)
        clearPlayer(member->GetGUID());
    for (Player* member : members)
        notify(member, groupGuid);
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

void WorldSession::HandleLfgSetCommentOpcode(WorldPacket& recvData)
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
    // 5.4.8 sends the direction as one bit. Some clicks also prefix a party index.
    // Reading the first byte as the direction turned "teleport out" into "teleport in"
    // while already inside, which the client shows as nothing happening.
    bool out = false;
    if (recvData.size() >= 2)
    {
        recvData.read_skip<uint8>();
        if (recvData.rpos() < recvData.size())
            out = recvData.ReadBit();
    }
    else if (recvData.size() == 1)
    {
        uint8 raw = recvData.read<uint8>();
        out = raw != 0;
    }

    SF_LOG_DEBUG("lfg", "CMSG_LFD_TELEPORT %s out: %u size: %u",
        GetPlayerInfo().c_str(), out ? 1 : 0, uint32(recvData.size()));
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
    if (!GetPlayer())
        return;

    uint64 guid = GetPlayer()->GetGUID();
    ObjectGuid playerGuid = guid;
    bool const hasPlayerGuid = true;

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
    ByteBuffer rewardData;

    data.WriteBits(lock.size(), 20); // locksize count
    data.WriteBit(hasPlayerGuid);
    data.WriteBits(randomDungeons.size(), 17);

    for (lfg::LfgDungeonSet::const_iterator it = randomDungeons.begin(); it != randomDungeons.end(); ++it)
    {
        lfg::LfgReward const* reward = sLFGMgr->GetRandomDungeonReward(*it, level);
        Quest const* quest = NULL;
        bool firstReward = false;

        if (reward)
        {
            quest = sObjectMgr->GetQuestTemplate(reward->firstQuest);
            if (quest)
            {
                bool done = !GetPlayer()->CanRewardQuest(quest, false);
                if (done)
                    quest = sObjectMgr->GetQuestTemplate(reward->otherQuest);

                firstReward = !done;
            }
        }

        bool const shortageEligible = !GetPlayer()->GetGroup();
        uint8 const shortageCount = shortageEligible ? lfg::LFG_ROLE_SHORTAGE_MAX : 0;

        data.WriteBit(firstReward);
        data.WriteBit(shortageEligible);
        data.WriteBits(0, 21); // Additional currency count
        data.WriteBits(shortageCount, 19);
        data.WriteBits(quest ? quest->GetRewItemsCount() : 0, 20);

        for (uint8 i = 0; i < shortageCount; ++i)
        {
            data.WriteBits(0, 21); // Shortage currency count
            data.WriteBits(0, 20); // Shortage item count
            data.WriteBits(0, 21); // Extra shortage currency count
        }

        data.WriteBits(quest ? quest->GetRewCurrencyCount() : 0, 21);

        rewardData << uint32(quest ? quest->XPValue(GetPlayer()) : 0);

        for (uint8 i = 0; i < shortageCount; ++i)
        {
            rewardData << uint32(0); // Shortage reward money
            rewardData << uint32(lfg::PLAYER_ROLE_TANK | lfg::PLAYER_ROLE_HEALER | lfg::PLAYER_ROLE_DAMAGE);
            rewardData << uint32(0);
        }

        rewardData << uint32(0); // SpecificQuantity
        rewardData << uint32(0); // PurseLimit
        rewardData << uint32(quest ? quest->GetRewMoney() : 0);

        if (quest && quest->GetRewItemsCount())
        {
            for (uint8 i = 0; i < QUEST_REWARDS_COUNT; ++i)
            {
                if (!quest->RewardItemId[i])
                    continue;

                ItemTemplate const* item = sObjectMgr->GetItemTemplate(quest->RewardItemId[i]);
                rewardData << uint32(quest->RewardItemIdCount[i]);
                rewardData << uint32(quest->RewardItemId[i]);
                rewardData << uint32(item ? item->DisplayInfoID : 0);
            }
        }

        rewardData << uint32(0); // OverallQuantity
        rewardData << uint32(0); // PurseWeeklyQuantity
        rewardData << uint32(1); // OverallLimit
        rewardData << uint32(1); // Quantity
        rewardData << uint32(0); // CompletionCurrencyID
        rewardData << uint32(*it); // Dungeon Entry (id + type)

        if (quest && quest->GetRewCurrencyCount())
        {
            for (uint8 i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
            {
                if (!quest->RewardCurrencyId[i])
                    continue;

                rewardData << uint32(quest->RewardCurrencyId[i]);
                rewardData << uint32(quest->RewardCurrencyCount[i] * 100);
            }
        }

        rewardData << uint32(0); // PurseWeeklyLimit
        rewardData << uint32(0); // Mask
        rewardData << uint32(0); // PurseQuantity
        rewardData << uint32(1); // CompletionLimit
        rewardData << uint32(1); // SpecificLimit
        rewardData << uint32(0); // CompletedMask
        rewardData << uint32(1); // CompletionQuantity
    }

    if (hasPlayerGuid)
        data.WriteGuidMask(playerGuid, 5, 1, 2, 7, 3, 0, 6, 4);

    data.FlushBits();

    if (hasPlayerGuid)
        data.WriteGuidBytes(playerGuid, 7, 2, 3, 0, 4, 5, 6, 1);

    data.append(rewardData);

    for (lfg::LfgLockMap::const_iterator it = lock.begin(); it != lock.end(); ++it)
    {
        data << uint32(it->first);                         // Dungeon entry (id + type)
        data << uint32(it->second.lockStatus);             // Lock status
        data << uint32(it->second.currentItemLevel);       // Current itemLevel
        data << uint32(it->second.requiredItemLevel);      // Required itemLevel
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
    if (!HasPacketBytes(recvData, sizeof(uint32)))
    {
        SF_LOG_DEBUG("lfg.lfr", "CMSG_LFG_LFR_JOIN %s malformed packet size: %u",
            GetPlayerInfo().c_str(), uint32(recvData.size()));
        SendLfgJoinResult(lfg::LfgJoinResultData(lfg::LFG_JOIN_DUNGEON_INVALID));
        SendLfrRemovedFromQueue(this);
        return;
    }

    uint32 entry;                                          // Raid id to search
    recvData >> entry;
    bool const isRaidFinderDungeon = sLFGMgr->IsRaidFinderDungeon(entry);

    SF_LOG_DEBUG("lfg.lfr", "CMSG_LFG_LFR_JOIN %s dungeon entry: %u isRaid: %u size: %u",
        GetPlayerInfo().c_str(), entry, isRaidFinderDungeon ? 1 : 0, uint32(recvData.size()));

    if (!isRaidFinderDungeon)
    {
        SendLfgJoinResult(lfg::LfgJoinResultData(lfg::LFG_JOIN_DUNGEON_INVALID));
        SendLfrRemovedFromQueue(this);
        return;
    }

    // Matching is intentionally not enabled in the scaffolding slice.
    SendLfgJoinResult(lfg::LfgJoinResultData(lfg::LFG_JOIN_INTERNAL_ERROR));
    SendLfrRemovedFromQueue(this);
}

void WorldSession::HandleLfrLeaveOpcode(WorldPacket& recvData)
{
    if (!HasPacketBytes(recvData, sizeof(uint32)))
    {
        SF_LOG_DEBUG("lfg.lfr", "CMSG_LFG_LFR_LEAVE %s malformed packet size: %u",
            GetPlayerInfo().c_str(), uint32(recvData.size()));
        SendLfrRemovedFromQueue(this);
        return;
    }

    uint32 dungeonId;                                      // Raid id queue to leave
    recvData >> dungeonId;
    bool const isRaidFinderDungeon = sLFGMgr->IsRaidFinderDungeon(dungeonId);

    SF_LOG_DEBUG("lfg.lfr", "CMSG_LFG_LFR_LEAVE %s dungeonId: %u isRaid: %u size: %u",
        GetPlayerInfo().c_str(), dungeonId, isRaidFinderDungeon ? 1 : 0, uint32(recvData.size()));

    SendLfrRemovedFromQueue(this);
}

void WorldSession::HandleLfgGetStatus(WorldPacket& /*recvData*/)
{
    SF_LOG_DEBUG("lfg", "CMSG_LFG_GET_STATUS %s", GetPlayerInfo().c_str());

    uint64 guid = GetPlayer()->GetGUID();
    lfg::LfgUpdateData updateData = sLFGMgr->GetLfgStatus(guid);
    bool hasActiveProposal = updateData.state == lfg::LFG_STATE_PROPOSAL;

    if (Group* group = GetPlayer()->GetGroup())
    {
        if (group->isLFGGroup() && sLFGMgr->GetState(group->GetGUID()) == lfg::LFG_STATE_NONE)
        {
            uint64 const groupGuid = group->GetGUID();
            SF_LOG_DEBUG("lfg.status", "CMSG_LFG_GET_STATUS %s found stale LFG group %u with no finder state; disbanding group.",
                GetPlayerInfo().c_str(), GUID_LOPART(groupGuid));
            group->Disband();
            SendLfgLfrList(false);
            return;
        }
    }

    if (updateData.state == lfg::LFG_STATE_NONE)
    {
        SendLfgLfrList(false);
        return;
    }

    if (GetPlayer()->GetGroup())
    {
        SendLfgUpdateStatus(updateData, true);

        bool const inDungeonStatus = updateData.updateType == lfg::LFG_UPDATETYPE_UPDATE_STATUS &&
            (updateData.state == lfg::LFG_STATE_DUNGEON || updateData.state == lfg::LFG_STATE_FINISHED_DUNGEON);
        if (!inDungeonStatus)
        {
            updateData.dungeons.clear();
            SendLfgUpdateStatus(updateData, false);
        }
    }
    else
    {
        SendLfgUpdateStatus(updateData, false);
        updateData.dungeons.clear();
        SendLfgUpdateStatus(updateData, true);
    }

    if (hasActiveProposal)
        sLFGMgr->SendActiveProposal(guid);
}

void WorldSession::SendLfgUpdateStatus(lfg::LfgUpdateData const& updateData, bool party, uint64 queueGuidOverride, uint8 queueIdOverride, uint32 joinTimeOverride)
{
    bool join = false;
    bool queued = false;
    uint8 size = uint8(updateData.dungeons.size());
    ObjectGuid guid = _player->GetGUID();
    uint64 queueGuid = _player->GetGUID();
    if (queueGuidOverride)
    {
        guid = queueGuidOverride;
        queueGuid = queueGuidOverride;
    }
    else if (party)
        if (Group* group = _player->GetGroup())
        {
            guid = group->GetGUID();
            queueGuid = group->GetGUID();
        }

    time_t joinTime = sLFGMgr->GetQueueJoinTime(queueGuid);
    uint32 queueId = sLFGMgr->GetActiveQueueId(queueGuid);
    if (!queueId && (updateData.updateType != lfg::LFG_UPDATETYPE_REMOVED_FROM_QUEUE || !updateData.dungeons.empty()))
        queueId = sLFGMgr->GetQueueId(queueGuid);
    if (queueIdOverride)
        queueId = queueIdOverride;
    // The client matches an update against its queue by a three field ticket,
    // and the join time is one of those fields. Once the dungeon starts the
    // queue entry is already gone, so GetQueueJoinTime returns zero and every
    // update leaves with a ticket the client cannot recognize as its own. The
    // caller passes back the time the client itself sent, which is the only
    // copy still around.
    if (joinTimeOverride)
        joinTime = time_t(joinTimeOverride);
    uint8 dungeonCategory = 0;
    std::vector<uint32> dungeonEntries;
    dungeonEntries.reserve(updateData.dungeons.size());

    for (lfg::LfgDungeonSet::const_iterator it = updateData.dungeons.begin(); it != updateData.dungeons.end(); ++it)
    {
        uint32 const dungeonId = (*it & 0xFF000000) ? (*it & 0x00FFFFFF) : *it;
        uint32 const dungeonEntry = sLFGMgr->GetLFGDungeonEntry(dungeonId);
        if (dungeonEntry)
            dungeonEntries.push_back(dungeonEntry);
        else
            dungeonEntries.push_back(*it);

        if (!dungeonCategory)
            dungeonCategory = sLFGMgr->GetLFGDungeonCategory(dungeonId);
    }

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
        case lfg::LFG_UPDATETYPE_GROUP_FOUND:
            // Dungeon group is formed. Joined without queued is what switches the
            // minimap eye from "leave queue" to teleport and leave group.
            join = true;
            break;
        case lfg::LFG_UPDATETYPE_UPDATE_STATUS:
            queued = updateData.state == lfg::LFG_STATE_QUEUED;
            join = queued;
            break;
        default:
            break;
    }

    SF_LOG_DEBUG("lfg", "SMSG_LFD_UPDATE_STATUS %s updatetype: %u, party %s",
        GetPlayerInfo().c_str(), updateData.updateType, party ? "true" : "false");

    WorldPacket data(SMSG_LFD_UPDATE_STATUS, 1 + 8 + 3 + 2 + 1 + updateData.comment.length() + 4 + 4 + 1 + 1 + 1 + 4 + (4 * size));

    data.WriteBits(updateData.comment.length(), 8);       // CommentLen
    data.WriteBit(true);                                  // IsParty
    data.WriteBit(join);                                  // Joined
    data.WriteBits(size, 22);                             // Slots
    data.WriteGuidMask(guid, 2, 3, 1);
    data.WriteBit(join);                                  // Joined
    data.WriteGuidMask(guid, 7, 6, 0);
    data.WriteBit(true);                                  // NotifyUI
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
    data << uint8(updateData.updateType);                  // SubType
    data << uint32(sLFGMgr->GetRoles(_player->GetGUID())); // RequestedRoles
    data << uint32(queueId);                              // Id
    data.WriteGuidBytes(guid, 5);
    data.WriteString(updateData.comment);                 // Comment
    data.WriteGuidBytes(guid, 2);
    for (std::vector<uint32>::const_iterator it = dungeonEntries.begin(); it != dungeonEntries.end(); ++it)
        data << uint32(*it);
    data.WriteGuidBytes(guid, 0, 1);
    data << uint32(joinTime);                             // UnixTime
    data << uint8(dungeonCategory);                       // LFG category
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
    lfg::LfgRolesMap::const_iterator itLeaderRoles = roleCheck.roles.find(guid);
    uint8 roles = itLeaderRoles != roleCheck.roles.end() ? itLeaderRoles->second : 0;
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
            Player* member = ObjectAccessor::FindPlayer(guid);
            data << uint8(member ? member->getLevel() : 0); // Level
            data.WriteGuidBytes(guid, 3, 6);
            data << uint32(it->second);                     // RolesDesired
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
            data << uint32(itr->second.requiredItemLevel);      // SubReason2
            data << uint32(itr->second.currentItemLevel);       // SubReason1
            data << uint32(itr->second.lockStatus);             // Reason
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

    // The client only applies the status when the guid, id, type and join time
    // match the ticket the eye was opened with; otherwise it is dropped and the
    // tooltip stays on "< 1 minute" with no role icons.
    uint32 const rawDungeonId = (queueData.dungeonId & 0xFF000000) ? (queueData.dungeonId & 0x00FFFFFF) : queueData.dungeonId;
    uint32 dungeonEntry = sLFGMgr->GetLFGDungeonEntry(rawDungeonId);
    if (!dungeonEntry)
        dungeonEntry = queueData.dungeonId;

    ObjectGuid guid = queueData.requesterGuid ? queueData.requesterGuid : _player->GetGUID();
    uint8 const needs[3] = { queueData.tanks, queueData.healers, queueData.dps };
    int32 const waits[3] = { queueData.waitTimeTank, queueData.waitTimeHealer, queueData.waitTimeDps };

    WorldPacket data(SMSG_LFD_QUEUE_STATUS, 1 + 8 + 4 * 11 + 3);
    data.WriteGuidMask(guid, 4, 3, 5, 1, 2, 0, 6, 7);
    data.FlushBits();
    data << uint32(3);                                     // TicketType
    data.WriteGuidBytes(guid, 0);
    data << uint32(queueData.queuedTime);                  // QueuedTime, seconds
    data.WriteGuidBytes(guid, 4);
    data << int32(queueData.waitTime);                     // AvgWaitTimeMe
    for (uint8 i = 0; i < 3; ++i)
    {
        data << int32(waits[i]);                           // AvgWaitTimeByRole
        data << uint8(needs[i]);                           // LastNeeded
    }
    data << uint32(queueData.joinTime);                    // TicketTime
    data << uint32(queueData.queueId);                     // TicketId
    data.WriteGuidBytes(guid, 1);
    data << int32(queueData.waitTimeAvg);                  // AvgWaitTime
    data.WriteGuidBytes(guid, 7, 2);
    data << uint32(dungeonEntry);                          // Slot
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
    lfg::LfgAnswerContainer::const_iterator itVote = boot.votes.find(guid);
    lfg::LfgAnswer playerVote = itVote != boot.votes.end() ? itVote->second : lfg::LFG_ANSWER_PENDING;
    uint8 votesNum = 0;
    uint8 agreeNum = 0;
    time_t now = time(NULL);
    uint32 secsleft = boot.cancelTime > now ? uint32(boot.cancelTime - now) : 0;
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
    uint64 playerGuid = GetPlayer()->GetGUID();
    lfg::LfgProposalPlayerContainer::const_iterator itSelf = proposal.players.find(playerGuid);
    if (itSelf == proposal.players.end())
        return;

    uint64 originalGroup = itSelf->second.group;
    bool silent = !proposal.isNew && originalGroup && originalGroup == proposal.group;
    uint32 dungeonEntry = proposal.dungeonId;
    uint64 queueGuid = originalGroup ? originalGroup : playerGuid;
    uint32 queueId = sLFGMgr->GetQueueId(queueGuid);
    time_t joinTime = sLFGMgr->GetQueueJoinTime(queueGuid);
    if (!joinTime && queueGuid != playerGuid)
        joinTime = sLFGMgr->GetQueueJoinTime(playerGuid);

    SF_LOG_DEBUG("lfg", "SMSG_LFD_PROPOSAL_UPDATE %s state: %u",
        GetPlayerInfo().c_str(), proposal.state);

    // show random dungeon if player selected random dungeon and it's not lfg group
    if (!silent)
    {
        lfg::LfgDungeonSet const& playerDungeons = sLFGMgr->GetSelectedDungeons(playerGuid);
        if (!playerDungeons.empty() && playerDungeons.find(proposal.dungeonId) == playerDungeons.end())
            dungeonEntry = (*playerDungeons.begin());
    }

    dungeonEntry = sLFGMgr->GetLFGDungeonEntry(dungeonEntry);

    ObjectGuid queueGuidObject = queueGuid;
    ObjectGuid slotGuid = uint64(dungeonEntry) | (uint64(0x1F45) << 48);
    ByteBuffer roleData;

    WorldPacket data(SMSG_LFD_PROPOSAL_UPDATE, 4 + 1 + 4 + 4 + 1 + 1 + proposal.players.size() * (4 + 1 + 1 + 1 + 1 + 1));
    data.WriteGuidMask(slotGuid, 6, 0);
    data.WriteGuidMask(queueGuidObject, 1, 7, 5);
    data.WriteGuidMask(slotGuid, 5);
    data.WriteGuidMask(queueGuidObject, 4);
    data.WriteBit(silent);                              // ProposalSilent
    data.WriteGuidMask(slotGuid, 2);
    data.WriteGuidMask(queueGuidObject, 6);
    data.WriteGuidMask(slotGuid, 3, 7);
    data.WriteGuidMask(queueGuidObject, 3);
    data.WriteBits(proposal.players.size(), 21);        // Players
    for (lfg::LfgProposalPlayerContainer::const_iterator it = proposal.players.begin(); it != proposal.players.end(); ++it)
    {
        data.WriteBit(it->second.group && it->second.group == proposal.group); // MyParty
        data.WriteBit(it->first == playerGuid);                                // Me
        data.WriteBit(it->second.accept != lfg::LFG_ANSWER_PENDING);           // Responded
        data.WriteBit(it->second.accept == lfg::LFG_ANSWER_AGREE);             // Accepted
        data.WriteBit(it->second.group && it->second.group == originalGroup);   // SameParty

        uint8 role = it->second.role;
        if (proposal.leader != it->first)
            role &= ~lfg::PLAYER_ROLE_LEADER;

        roleData << uint32(role);
    }
    data.WriteGuidMask(queueGuidObject, 2);
    data.WriteGuidMask(slotGuid, 4);
    data.WriteBit(0);                                      // Unk
    data.WriteGuidMask(queueGuidObject, 0);
    data.WriteGuidMask(slotGuid, 1);
    data.FlushBits();

    data.WriteGuidBytes(slotGuid, 1);
    data.WriteGuidBytes(queueGuidObject, 4);
    data.WriteGuidBytes(slotGuid, 4);
    data.WriteGuidBytes(queueGuidObject, 7);
    data.WriteGuidBytes(queueGuidObject, 2);
    data.WriteGuidBytes(queueGuidObject, 0);
    data << uint32(dungeonEntry);        // Slot
    data << uint8(proposal.state);       // State
    data << uint32(queueId);             // Id
    data.WriteGuidBytes(slotGuid, 6);
    data << uint32(proposal.id);         // ProposalID
    data.WriteGuidBytes(queueGuidObject, 5, 3);
    data << uint32(joinTime);            // UnixTime
    data.WriteGuidBytes(slotGuid, 5);
    data.WriteGuidBytes(queueGuidObject, 6);
    data.append(roleData);
    data << uint32(proposal.encounters); // CompletedMask
    // slotGuid byte 7, not a second copy of queueGuidObject byte 7: both guids declare all eight
    // mask bits above, so the client expects eight bytes of each.
    data.WriteGuidBytes(slotGuid, 7);
    data.WriteGuidBytes(queueGuidObject, 1);
    data.WriteGuidBytes(slotGuid, 0, 2);
    data << uint32(3);                   // Type
    data.WriteGuidBytes(slotGuid, 3);
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

void WorldSession::SendLfgClearStatus(uint64 lfgGroupGuid, uint8 queueIdOverride, bool groupLeave, uint32 joinTimeOverride)
{
    if (groupLeave)
        SendLfgUpdateStatus(lfg::LfgUpdateData(lfg::LFG_UPDATETYPE_LEADER_UNK1), true, lfgGroupGuid, queueIdOverride, joinTimeOverride);

    lfg::LfgUpdateData removed(lfg::LFG_UPDATETYPE_REMOVED_FROM_QUEUE);
    SendLfgUpdateStatus(removed, false, 0, queueIdOverride, joinTimeOverride);
    SendLfgUpdateStatus(removed, true, lfgGroupGuid, queueIdOverride, joinTimeOverride);
    SendLfgLfrList(false);
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
    data.WriteBits(err, 4);                                   // Reason
    data.FlushBits();
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
