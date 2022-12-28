/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "Common.h"
#include "DatabaseEnv.h"
#include "Group.h"
#include "GroupMgr.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Pet.h"
#include "Player.h"
#include "SocialMgr.h"
#include "SpellAuras.h"
#include "Util.h"
#include "Vehicle.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"

class Aura;

/* differeces from off:
    -you can uninvite yourself - is is useful
    -you can accept invitation even if leader went offline
*/
/* todo:
    -group_destroyed msg is sent but not shown
    -reduce xp gaining when in raid group
    -quest sharing has to be corrected
    -FIX sending PartyMemberStats
*/

void WorldSession::SendPartyResult(PartyOperation operation, const std::string& member, PartyResult res, uint32 val /* = 0 */)
{
    WorldPacket data(SMSG_PARTY_COMMAND_RESULT, 4 + member.size() + 1 + 4 + 4 + 8);
    data << uint32(operation);
    data << member;
    data << uint32(res);
    data << uint32(val);                                // LFD cooldown related (used with ERR_PARTY_LFG_BOOT_COOLDOWN_S and ERR_PARTY_LFG_BOOT_NOT_ELIGIBLE_S)
    data << uint64(0);                                  // player who caused error (in some cases).

    SendPacket(&data);
}

void WorldSession::SendGroupInviteNotification(ObjectGuid InviterGUID, const std::string& inviterName, bool inGroup)
{
    SF_LOG_DEBUG("network", "WORLD: sending SMSG_GROUP_INVITE");
    
    WorldPacket data(SMSG_GROUP_INVITE, 6 + 1 + 8 + 8 + 4 + 4 + 4 + inviterName.size());
    data.WriteBits(0, 8);
    data.WriteBits(0, 8);
    data.WriteGuidMask(InviterGUID, 2);
    data.WriteBit(0);
    data.WriteBits(inviterName.size(), 6);              // inviter name length
    data.WriteGuidMask(InviterGUID, 7, 5);
    data.WriteBit(!inGroup);                            // inverse already in group
    data.WriteBit(0);                                   // auto decline
    data.WriteGuidMask(InviterGUID, 1);
    data.WriteBit(0);                                   // cross realm invite (includes hyphen between inviter and server name)
    data.WriteBit(0);                                   // realm transfer warning("Accepting this invitation may transfer you to another realm")
    data.WriteBits(0, 22);                              // counter
    data.WriteGuidMask(InviterGUID, 3, 0, 4, 6);
    data.FlushBits();

    data.WriteGuidBytes(InviterGUID, 6, 7, 2, 0);
    data << uint64(0);
    data << uint32(0);
    data << uint32(0);
    data.WriteGuidBytes(InviterGUID, 1, 5, 4);
    data << int32(0);
    data.WriteString(inviterName);
    data.WriteGuidBytes(InviterGUID, 3);
    data << uint32(0);

    /*for (int i = 0; i < counter; i++)
        data << int32(0);*/

    SendPacket(&data);
}

void WorldSession::HandleGroupInviteOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_GROUP_INVITE");

    ObjectGuid crossRealmGuid;                                      // unused

    recvData.read_skip<uint32>();                                   // Non-zero in cross realm invites
    recvData.read_skip<uint8>();                                    // Unknown
    recvData.read_skip<uint32>();                                   // Always 0

    crossRealmGuid[7] = recvData.ReadBit();
    uint8 realmLen = recvData.ReadBits(9);
    crossRealmGuid[3] = recvData.ReadBit();
    uint8 nameLen = recvData.ReadBits(9);
    crossRealmGuid[2] = recvData.ReadBit();
    crossRealmGuid[5] = recvData.ReadBit();
    crossRealmGuid[4] = recvData.ReadBit();
    crossRealmGuid[0] = recvData.ReadBit();
    crossRealmGuid[1] = recvData.ReadBit();
    crossRealmGuid[6] = recvData.ReadBit();

    recvData.ReadByteSeq(crossRealmGuid[7]);
    recvData.ReadByteSeq(crossRealmGuid[6]);
    recvData.ReadByteSeq(crossRealmGuid[0]);
    recvData.ReadByteSeq(crossRealmGuid[4]);
    std::string realmName = recvData.ReadString(realmLen);          // unused
    recvData.ReadByteSeq(crossRealmGuid[1]);
    recvData.ReadByteSeq(crossRealmGuid[2]);
    recvData.ReadByteSeq(crossRealmGuid[3]);
    std::string memberName = recvData.ReadString(nameLen);
    recvData.ReadByteSeq(crossRealmGuid[5]);

    // attempt add selected player

    // cheating
    if (!normalizePlayerName(memberName))
    {
        SendPartyResult(PartyOperation::PARTY_OP_INVITE, memberName, PartyResult::ERR_BAD_PLAYER_NAME_S);
        return;
    }

    Player* player = sObjectAccessor->FindPlayerByName(memberName);

    // no player
    if (!player)
    {
        SendPartyResult(PartyOperation::PARTY_OP_INVITE, memberName, PartyResult::ERR_BAD_PLAYER_NAME_S);
        return;
    }

    // restrict invite to GMs
    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ALLOW_GM_GROUP) && !GetPlayer()->IsGameMaster() && player->IsGameMaster())
    {
        SendPartyResult(PartyOperation::PARTY_OP_INVITE, memberName, PartyResult::ERR_BAD_PLAYER_NAME_S);
        return;
    }

    // can't group with
    if (!GetPlayer()->IsGameMaster() && !sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ALLOW_TWO_SIDE_INTERACTION_GROUP) && GetPlayer()->GetTeam() != player->GetTeam())
    {
        SendPartyResult(PartyOperation::PARTY_OP_INVITE, memberName, PartyResult::ERR_PLAYER_WRONG_FACTION);
        return;
    }

    if (GetPlayer()->GetInstanceId() != 0 && player->GetInstanceId() != 0 && GetPlayer()->GetInstanceId() != player->GetInstanceId() && GetPlayer()->GetMapId() == player->GetMapId())
    {
        SendPartyResult(PartyOperation::PARTY_OP_INVITE, memberName, PartyResult::ERR_TARGET_NOT_IN_INSTANCE_S);
        return;
    }

    // just ignore us
    if (player->GetInstanceId() != 0 && player->GetDungeonDifficulty() != GetPlayer()->GetDungeonDifficulty())
    {
        SendPartyResult(PartyOperation::PARTY_OP_INVITE, memberName, PartyResult::ERR_IGNORING_YOU_S);
        return;
    }

    if (player->GetSocial()->HasIgnore(GetPlayer()->GetGUIDLow()))
    {
        SendPartyResult(PartyOperation::PARTY_OP_INVITE, memberName, PartyResult::ERR_IGNORING_YOU_S);
        return;
    }

    Group* group = GetPlayer()->GetGroup();
    if (group && group->isBGGroup())
        group = GetPlayer()->GetOriginalGroup();

    Group* group2 = player->GetGroup();
    if (group2 && group2->isBGGroup())
        group2 = player->GetOriginalGroup();
    // player already in another group or invited
    if (group2 || player->GetGroupInvite())
    {
        SendPartyResult(PartyOperation::PARTY_OP_INVITE, memberName, PartyResult::ERR_ALREADY_IN_GROUP_S);

        if (group2)
        {
            // tell the player that they were invited but it failed as they were already in a group
            player->GetSession()->SendGroupInviteNotification(GetPlayer()->GetObjectGUID(), GetPlayer()->GetName(), true);
        }

        return;
    }

    if (group)
    {
        // not have permissions for invite
        if (!group->IsLeader(GetPlayer()->GetGUID()) && !group->IsAssistant(GetPlayer()->GetGUID()))
        {
            SendPartyResult(PartyOperation::PARTY_OP_INVITE, "", PartyResult::ERR_NOT_LEADER);
            return;
        }
        // not have place
        if (group->IsFull())
        {
            SendPartyResult(PartyOperation::PARTY_OP_INVITE, "", PartyResult::ERR_GROUP_FULL);
            return;
        }
    }

    // ok, but group not exist, start a new group
    // but don't create and save the group to the DB until
    // at least one person joins
    if (!group)
    {
        group = new Group;
        // new group: if can't add then delete
        if (!group->AddLeaderInvite(GetPlayer()))
        {
            delete group;
            return;
        }
        if (!group->AddInvite(player))
        {
            delete group;
            return;
        }
    }
    else
    {
        // already existed group: if can't add then just leave
        if (!group->AddInvite(player))
        {
            return;
        }
    }

    // ok, we do it
    player->GetSession()->SendGroupInviteNotification(GetPlayer()->GetObjectGUID(), GetPlayer()->GetName(), false);

    SendPartyResult(PartyOperation::PARTY_OP_INVITE, memberName, PartyResult::ERR_PARTY_RESULT_OK);
}

void WorldSession::HandleGroupInviteResponseOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_GROUP_INVITE_RESPONSE");

    recvData.read_skip<uint8>();                // Unknown
    bool unknown = recvData.ReadBit();          // Unknown
    bool accept = recvData.ReadBit();

    /*if (unknown)
        recvData.read_skip<uint32>();*/

    Group* group = GetPlayer()->GetGroupInvite();

    if (!group)
        return;

    if (accept)
    {
        // Remove player from invitees in any case
        group->RemoveInvite(GetPlayer());

        if (group->GetLeaderGUID() == GetPlayer()->GetGUID())
        {
            SF_LOG_ERROR("network", "HandleGroupAcceptOpcode: player %s(%d) tried to accept an invite to his own group", GetPlayer()->GetName().c_str(), GetPlayer()->GetGUIDLow());
            return;
        }

        // Group is full
        if (group->IsFull())
        {
            SendPartyResult(PartyOperation::PARTY_OP_INVITE, "", PartyResult::ERR_GROUP_FULL);
            return;
        }

        Player* leader = ObjectAccessor::FindPlayer(group->GetLeaderGUID());

        // Forming a new group, create it
        if (!group->IsCreated())
        {
            // This can happen if the leader is zoning. To be removed once delayed actions for zoning are implemented
            if (!leader)
            {
                group->RemoveAllInvites();
                return;
            }

            // If we're about to create a group there really should be a leader present
            ASSERT(leader);
            group->RemoveInvite(leader);
            group->Create(leader);
            sGroupMgr->AddGroup(group);
        }

        // Everything is fine, do it, PLAYER'S GROUP IS SET IN ADDMEMBER!!!
        if (!group->AddMember(GetPlayer()))
            return;

        group->BroadcastGroupUpdate();
    }
    else
    {
        // Remember leader if online (group pointer will be invalid if group gets disbanded)
        Player* leader = ObjectAccessor::FindPlayer(group->GetLeaderGUID());

        // uninvite, group can be deleted
        GetPlayer()->UninviteFromGroup();

        if (!leader || !leader->GetSession())
            return;

        // report
        WorldPacket data(SMSG_GROUP_DECLINE, GetPlayer()->GetName().size());
        data << GetPlayer()->GetName();
        leader->GetSession()->SendPacket(&data);
    }
}

void WorldSession::HandleGroupUninviteGuidOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_GROUP_UNINVITE_GUID");

    ObjectGuid guid;

    recvData.read_skip<uint8>();

    guid[6] = recvData.ReadBit();
    guid[4] = recvData.ReadBit();
    guid[3] = recvData.ReadBit();
    guid[2] = recvData.ReadBit();
    guid[0] = recvData.ReadBit();
    guid[1] = recvData.ReadBit();
    guid[7] = recvData.ReadBit();
    guid[5] = recvData.ReadBit();

    uint8 reasonLen = recvData.ReadBits(8);
    std::string reason = recvData.ReadString(reasonLen);
    recvData.ReadByteSeq(guid[5]);
    recvData.ReadByteSeq(guid[6]);
    recvData.ReadByteSeq(guid[1]);
    recvData.ReadByteSeq(guid[4]);
    recvData.ReadByteSeq(guid[3]);
    recvData.ReadByteSeq(guid[2]);
    recvData.ReadByteSeq(guid[7]);
    recvData.ReadByteSeq(guid[0]);

    //can't uninvite yourself
    if (guid == GetPlayer()->GetGUID())
    {
        SF_LOG_ERROR("network", "WorldSession::HandleGroupUninviteGuidOpcode: leader %s(%d) tried to uninvite himself from the group.",
            GetPlayer()->GetName().c_str(), GetPlayer()->GetGUIDLow());
        return;
    }

    PartyResult res = GetPlayer()->CanUninviteFromGroup();
    if (res != PartyResult::ERR_PARTY_RESULT_OK)
    {
        SendPartyResult(PartyOperation::PARTY_OP_UNINVITE, "", res);
        return;
    }

    Group* grp = GetPlayer()->GetGroup();
    if (!grp)
        return;

    if (grp->IsLeader(guid))
    {
        SendPartyResult(PartyOperation::PARTY_OP_UNINVITE, "", PartyResult::ERR_NOT_LEADER);
        return;
    }

    if (grp->IsMember(guid))
    {
        Player::RemoveFromGroup(grp, guid, GROUP_REMOVEMETHOD_KICK, GetPlayer()->GetGUID(), reason.c_str());
        return;
    }

    if (Player* player = grp->GetInvited(guid))
    {
        player->UninviteFromGroup();
        return;
    }

    SendPartyResult(PartyOperation::PARTY_OP_UNINVITE, "", PartyResult::ERR_TARGET_NOT_IN_GROUP_S);
}

void WorldSession::HandleGroupSetLeaderOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_GROUP_SET_LEADER");

    ObjectGuid guid;

    recvData.read_skip<uint8>();

    guid[1] = recvData.ReadBit();
    guid[7] = recvData.ReadBit();
    guid[0] = recvData.ReadBit();
    guid[2] = recvData.ReadBit();
    guid[5] = recvData.ReadBit();
    guid[3] = recvData.ReadBit();
    guid[4] = recvData.ReadBit();
    guid[6] = recvData.ReadBit();

    recvData.ReadByteSeq(guid[1]);
    recvData.ReadByteSeq(guid[5]);
    recvData.ReadByteSeq(guid[7]);
    recvData.ReadByteSeq(guid[6]);
    recvData.ReadByteSeq(guid[0]);
    recvData.ReadByteSeq(guid[2]);
    recvData.ReadByteSeq(guid[4]);
    recvData.ReadByteSeq(guid[3]);

    Player* player = ObjectAccessor::FindPlayer(guid);
    Group* group = GetPlayer()->GetGroup();

    if (!group || !player)
        return;

    if (!group->IsLeader(GetPlayer()->GetGUID()) || player->GetGroup() != group)
        return;

    // Everything's fine, accepted.
    group->ChangeLeader(guid);
    group->SendUpdate();
}

void WorldSession::HandleGroupSetRolesOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_GROUP_SET_ROLES");

    uint32 newRole;
    ObjectGuid targetGuid;

    recvData.read_skip<uint8>();
    recvData >> newRole;

    targetGuid[2] = recvData.ReadBit();
    targetGuid[0] = recvData.ReadBit();
    targetGuid[7] = recvData.ReadBit();
    targetGuid[4] = recvData.ReadBit();
    targetGuid[1] = recvData.ReadBit();
    targetGuid[3] = recvData.ReadBit();
    targetGuid[6] = recvData.ReadBit();
    targetGuid[5] = recvData.ReadBit();

    recvData.ReadByteSeq(targetGuid[1]);
    recvData.ReadByteSeq(targetGuid[5]);
    recvData.ReadByteSeq(targetGuid[2]);
    recvData.ReadByteSeq(targetGuid[6]);
    recvData.ReadByteSeq(targetGuid[7]);
    recvData.ReadByteSeq(targetGuid[0]);
    recvData.ReadByteSeq(targetGuid[4]);
    recvData.ReadByteSeq(targetGuid[3]);

    Player* tPlayer = ObjectAccessor::FindPlayer(targetGuid);
    Group* group = GetPlayer()->GetGroup();

    if (!tPlayer || !group)
        return;

    if (group != tPlayer->GetGroup())
        return;

    ObjectGuid assignerGuid = GetPlayer()->GetGUID();

    WorldPacket data(SMSG_GROUP_SET_ROLE, 1 + 8 + 1 + 8 + 4 + 1 + 4);
    data.WriteBit(assignerGuid[1]);
    data.WriteBit(targetGuid[7]);
    data.WriteBit(targetGuid[6]);
    data.WriteBit(targetGuid[4]);
    data.WriteBit(targetGuid[1]);
    data.WriteBit(targetGuid[0]);
    data.WriteBit(assignerGuid[0]);
    data.WriteBit(assignerGuid[7]);
    data.WriteBit(targetGuid[3]);
    data.WriteBit(assignerGuid[6]);
    data.WriteBit(targetGuid[2]);
    data.WriteBit(assignerGuid[4]);
    data.WriteBit(assignerGuid[5]);
    data.WriteBit(assignerGuid[2]);
    data.WriteBit(targetGuid[5]);
    data.WriteBit(assignerGuid[3]);

    data.WriteByteSeq(assignerGuid[1]);
    data.WriteByteSeq(assignerGuid[6]);
    data.WriteByteSeq(assignerGuid[2]);
    data.WriteByteSeq(targetGuid[3]);
    data << uint32(group->GetMemberRole(targetGuid));
    data.WriteByteSeq(assignerGuid[7]);
    data.WriteByteSeq(targetGuid[5]);
    data.WriteByteSeq(assignerGuid[3]);
    data.WriteByteSeq(targetGuid[4]);
    data.WriteByteSeq(targetGuid[7]);
    data.WriteByteSeq(assignerGuid[5]);
    data.WriteByteSeq(targetGuid[6]);
    data.WriteByteSeq(targetGuid[2]);
    data.WriteByteSeq(targetGuid[1]);
    data.WriteByteSeq(targetGuid[0]);
    data.WriteByteSeq(assignerGuid[4]);
    data << uint8(0);                           // unknown
    data.WriteByteSeq(assignerGuid[0]);
    data << uint32(newRole);

    group->BroadcastPacket(&data, false);
    group->SetMemberRole(targetGuid, newRole);
    group->SendUpdate();
}

void WorldSession::HandleGroupDisbandOpcode(WorldPacket& /*recvData*/)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_GROUP_DISBAND");

    Group* grp = GetPlayer()->GetGroup();
    if (!grp)
        return;

    if (_player->InBattleground())
    {
        SendPartyResult(PartyOperation::PARTY_OP_INVITE, "", PartyResult::ERR_INVITE_RESTRICTED);
        return;
    }

    /** error handling **/
    /********************/

    // everything's fine, do it
    SendPartyResult(PartyOperation::PARTY_OP_LEAVE, GetPlayer()->GetName(), PartyResult::ERR_PARTY_RESULT_OK);

    GetPlayer()->RemoveFromGroup(GROUP_REMOVEMETHOD_LEAVE);
}

void WorldSession::HandleLootMethodOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_LOOT_METHOD");

    ObjectGuid lootMaster;
    uint8 lootMethod;
    uint32 lootThreshold;

    recvData.read_skip<uint8>(); // PartyIndex
    recvData >> lootMethod;      // Method
    recvData >> lootThreshold;   // Threshold
    recvData.ReadGuidMask(lootMaster, 7, 1, 2, 0, 4, 5, 6, 3);
    recvData.ReadGuidBytes(lootMaster, 7, 1, 3, 4, 6, 5, 0, 2);

    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    /** error handling **/
    if (!group->IsLeader(GetPlayer()->GetGUID()))
        return;
    /********************/

    // everything's fine, do it
    group->SetLootMethod((LootMethod)lootMethod);
    group->SetLooterGuid(lootMaster);
    group->SetLootThreshold((ItemQualities)lootThreshold);
    group->SendUpdate();
}

void WorldSession::HandleLootRoll(WorldPacket& recvData)
{
    ObjectGuid guid;
    uint8 rollType, itemSlot;

    recvData >> itemSlot;
    recvData >> rollType;              // 0: pass, 1: need, 2: greed

    recvData.ReadGuidMask(guid, 7, 1, 2, 0, 6, 3, 4, 5);
    recvData.ReadGuidBytes(guid, 0, 2, 7, 3, 1, 5, 4, 6);

    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    RollType type = RollType(rollType);
    group->CountRollVote(GetPlayer()->GetGUID(), guid, type);

    switch (type)
    {
        case RollType::ROLL_NEED:
            GetPlayer()->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED, 1);
            break;
        case RollType::ROLL_GREED:
            GetPlayer()->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED, 1);
            break;
        default:
            break;
    }
}

void WorldSession::HandleMinimapPingOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_MINIMAP_PING");

    if (!GetPlayer()->GetGroup())
        return;

    float x, y;
    recvData >> y;
    recvData >> x;
    recvData.read_skip<uint8>();

    //SF_LOG_DEBUG("misc", "Received opcode MSG_MINIMAP_PING X: %f, Y: %f", x, y);

    /** error handling **/
    /********************/

    ObjectGuid guid = GetPlayer()->GetGUID();

    // everything's fine, do it
    WorldPacket data(SMSG_MINIMAP_PING, 1 + 8 + 4 + 4);
    data << float(y);
    data << float(x);

    data.WriteBit(guid[0]);
    data.WriteBit(guid[5]);
    data.WriteBit(guid[2]);
    data.WriteBit(guid[7]);
    data.WriteBit(guid[1]);
    data.WriteBit(guid[3]);
    data.WriteBit(guid[6]);
    data.WriteBit(guid[4]);

    data.WriteByteSeq(guid[6]);
    data.WriteByteSeq(guid[5]);
    data.WriteByteSeq(guid[7]);
    data.WriteByteSeq(guid[2]);
    data.WriteByteSeq(guid[0]);
    data.WriteByteSeq(guid[3]);
    data.WriteByteSeq(guid[1]);
    data.WriteByteSeq(guid[4]);

    GetPlayer()->GetGroup()->BroadcastPacket(&data, true, -1, GetPlayer()->GetGUID());
}

void WorldSession::HandleRandomRollOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_RANDOM_ROLL");

    uint32 minimum, maximum, roll;
    recvData >> maximum;
    recvData >> minimum;
    recvData.read_skip<uint8>();

    /** error handling **/
    if (minimum > maximum || maximum > 10000)                // < 32768 for urand call
        return;
    /********************/

    // everything's fine, do it
    roll = urand(minimum, maximum);

    //SF_LOG_DEBUG("misc", "ROLL: MIN: %u, MAX: %u, ROLL: %u", minimum, maximum, roll);

    ObjectGuid guid = GetPlayer()->GetGUID();

    WorldPacket data(SMSG_RANDOM_ROLL, 4 + 4 + 4 + 1 + 8);
    data << uint32(roll);
    data << uint32(minimum);
    data << uint32(maximum);

    data.WriteBit(guid[0]);
    data.WriteBit(guid[6]);
    data.WriteBit(guid[7]);
    data.WriteBit(guid[1]);
    data.WriteBit(guid[4]);
    data.WriteBit(guid[5]);
    data.WriteBit(guid[2]);
    data.WriteBit(guid[3]);

    data.WriteByteSeq(guid[5]);
    data.WriteByteSeq(guid[4]);
    data.WriteByteSeq(guid[2]);
    data.WriteByteSeq(guid[0]);
    data.WriteByteSeq(guid[3]);
    data.WriteByteSeq(guid[1]);
    data.WriteByteSeq(guid[6]);
    data.WriteByteSeq(guid[7]);

    if (GetPlayer()->GetGroup())
        GetPlayer()->GetGroup()->BroadcastPacket(&data, false);
    else
        SendPacket(&data);
}

void WorldSession::HandleRaidTargetUpdateOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received MSG_RAID_TARGET_UPDATE");

    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    uint8 Symbol, Index;
    recvData >> Symbol >> Index;

    /** error handling **/
    /********************/

    // everything's fine, do it
    if (Symbol == 0xFF)                                     // target icon request
        group->SendTargetIconList(this);
    else                                                    // target icon update
    {
        if (!group->IsLeader(GetPlayer()->GetGUID()) && !group->IsAssistant(GetPlayer()->GetGUID()))
            return;

        ObjectGuid targetGuid;

        targetGuid[3] = recvData.ReadBit();
        targetGuid[2] = recvData.ReadBit();
        targetGuid[1] = recvData.ReadBit();
        targetGuid[5] = recvData.ReadBit();
        targetGuid[0] = recvData.ReadBit();
        targetGuid[6] = recvData.ReadBit();
        targetGuid[7] = recvData.ReadBit();
        targetGuid[4] = recvData.ReadBit();

        recvData.ReadByteSeq(targetGuid[2]);
        recvData.ReadByteSeq(targetGuid[3]);
        recvData.ReadByteSeq(targetGuid[0]);
        recvData.ReadByteSeq(targetGuid[7]);
        recvData.ReadByteSeq(targetGuid[5]);
        recvData.ReadByteSeq(targetGuid[1]);
        recvData.ReadByteSeq(targetGuid[6]);
        recvData.ReadByteSeq(targetGuid[4]);

        group->SetTargetIcon(Symbol, _player->GetGUID(), targetGuid, Index);
    }
}

void WorldSession::HandleGroupRaidConvertOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_GROUP_RAID_CONVERT");

    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    if (_player->InBattleground())
        return;

    // error handling
    if (!group->IsLeader(GetPlayer()->GetGUID()) || group->GetMembersCount() < 2)
        return;

    // everything's fine, do it (is it 0 (PARTY_OP_INVITE) correct code)
    SendPartyResult(PartyOperation::PARTY_OP_INVITE, "", PartyResult::ERR_PARTY_RESULT_OK);

    // New 4.x: it is now possible to convert a raid to a group if member count is 5 or less

    bool toRaid;
    toRaid = recvData.ReadBit();

    if (toRaid)
        group->ConvertToRaid();
    else
        group->ConvertToGroup();
}

void WorldSession::HandleGroupChangeSubGroupOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_GROUP_CHANGE_SUB_GROUP");

    // we will get correct pointer for group here, so we don't have to check if group is BG raid
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    ObjectGuid TargetGUID;

    uint8 PartyIndex;
    uint8 groupNr;

    recvData >> PartyIndex;
    recvData >> groupNr;

    recvData.ReadGuidMask(TargetGUID, 1, 4, 6, 3, 7, 2, 0, 5); //17, 20, 22, 19, 23, 18, 16, 21
    recvData.ReadGuidBytes(TargetGUID, 2, 6, 1, 5, 3, 4, 0, 7); //18, 22, 17, 21, 19, 20, 16, 23

    if (groupNr >= MAX_RAID_SUBGROUPS)
        return;

    uint64 senderGuid = GetPlayer()->GetGUID();
    if (!group->IsLeader(senderGuid) && !group->IsAssistant(senderGuid))
        return;

    if (!group->HasFreeSlotSubGroup(groupNr))
        return;

    if(Player* movedPlayer = sObjectAccessor->FindPlayer(TargetGUID))
        group->ChangeMembersGroup(movedPlayer, groupNr);
}

void WorldSession::HandleGroupSwapSubGroupOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_GROUP_SWAP_SUB_GROUP");
    std::string unk1;
    std::string unk2;

    recvData >> unk1;
    recvData >> unk2;
}

void WorldSession::HandleGroupAssistantLeaderOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_GROUP_ASSISTANT_LEADER");

    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    if (!group->IsLeader(GetPlayer()->GetGUID()))
        return;

    ObjectGuid guid;

    uint8 unk = 0;
    recvData >> unk;

    guid[2] = recvData.ReadBit();
    guid[0] = recvData.ReadBit();
    guid[6] = recvData.ReadBit();
    guid[3] = recvData.ReadBit();
    guid[1] = recvData.ReadBit();
    bool apply = recvData.ReadBit();
    guid[4] = recvData.ReadBit();
    guid[5] = recvData.ReadBit();
    guid[7] = recvData.ReadBit();

    recvData.ReadByteSeq(guid[5]);
    recvData.ReadByteSeq(guid[1]);
    recvData.ReadByteSeq(guid[0]);
    recvData.ReadByteSeq(guid[7]);
    recvData.ReadByteSeq(guid[3]);
    recvData.ReadByteSeq(guid[6]);
    recvData.ReadByteSeq(guid[2]);
    recvData.ReadByteSeq(guid[4]);

    group->SetGroupMemberFlag(guid, apply, GroupMemberFlags::MEMBER_FLAG_ASSISTANT);

    group->SendUpdate();
}

void WorldSession::HandleGroupEveryoneIsAssistantOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_SET_EVERYONE_IS_ASSISTANT");

    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    if (!group->IsLeader(GetPlayer()->GetGUID()))
        return;

    recvData.read_skip<uint8>();
    bool apply = recvData.ReadBit();
    recvData.FlushBits();

    group->ChangeFlagEveryoneAssistant(apply);
}

void WorldSession::HandleSetPartyAssignmentOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_SET_PARTY_ASSIGNMENT");

    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    uint64 senderGuid = GetPlayer()->GetGUID();
    if (!group->IsLeader(senderGuid) && !group->IsAssistant(senderGuid))
        return;

    uint8 Assignment, partyindex;
    ObjectGuid guid;
    recvData >> Assignment;
    recvData >> partyindex;
    recvData.ReadGuidMask(guid, 5, 6, 2, 3, 1, 0, 4, 7);
    bool apply = recvData.ReadBit();
    recvData.FlushBits();
    recvData.ReadGuidBytes(guid, 2, 5, 1, 0, 6, 3, 4, 7);

    GroupMemberAssignment assignment = GroupMemberAssignment(Assignment);
    switch (assignment)
    {
        case GroupMemberAssignment::GROUP_ASSIGN_MAINASSIST:
            group->RemoveUniqueGroupMemberFlag(GroupMemberFlags::MEMBER_FLAG_MAINASSIST);
            group->SetGroupMemberFlag(guid, apply, GroupMemberFlags::MEMBER_FLAG_MAINASSIST);
            break;
        case GroupMemberAssignment::GROUP_ASSIGN_MAINTANK:
            group->RemoveUniqueGroupMemberFlag(GroupMemberFlags::MEMBER_FLAG_MAINTANK);
            group->SetGroupMemberFlag(guid, apply, GroupMemberFlags::MEMBER_FLAG_MAINTANK);
        default:
            break;
    }

    group->SendUpdate();
}

void WorldSession::HandleRaidReadyCheckOpcode(WorldPacket& /*recvData*/)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_RAID_READY_CHECK");

    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    ObjectGuid playerGuid = GetPlayer()->GetGUID();

    /** error handling **/
    if (!group->IsLeader(playerGuid) && !group->IsAssistant(playerGuid))
        return;

    // check is also done client side
    if (group->ReadyCheckInProgress())
        return;
    /********************/

    uint32 readyCheckDuration = 35000;
    ObjectGuid groupGuid = group->GetGUID();

    // everything's fine, do it
    WorldPacket data(SMSG_RAID_READY_CHECK, 1 + 8 + 1 + 8 + 1 + 4);
    data.WriteBit(groupGuid[4]);
    data.WriteBit(groupGuid[2]);
    data.WriteBit(playerGuid[4]);
    data.WriteBit(groupGuid[3]);
    data.WriteBit(groupGuid[7]);
    data.WriteBit(groupGuid[1]);
    data.WriteBit(groupGuid[0]);
    data.WriteBit(playerGuid[6]);
    data.WriteBit(playerGuid[5]);
    data.WriteBit(groupGuid[6]);
    data.WriteBit(groupGuid[5]);
    data.WriteBit(playerGuid[0]);
    data.WriteBit(playerGuid[1]);
    data.WriteBit(playerGuid[2]);
    data.WriteBit(playerGuid[7]);
    data.WriteBit(playerGuid[3]);

    data << uint32(readyCheckDuration);
    data.WriteByteSeq(groupGuid[2]);
    data.WriteByteSeq(groupGuid[7]);
    data.WriteByteSeq(groupGuid[3]);
    data.WriteByteSeq(playerGuid[4]);
    data.WriteByteSeq(groupGuid[1]);
    data.WriteByteSeq(groupGuid[0]);
    data.WriteByteSeq(playerGuid[1]);
    data.WriteByteSeq(playerGuid[2]);
    data.WriteByteSeq(playerGuid[6]);
    data.WriteByteSeq(playerGuid[5]);
    data.WriteByteSeq(groupGuid[6]);
    data.WriteByteSeq(playerGuid[0]);
    data << uint8(0);                       // unknown
    data.WriteByteSeq(playerGuid[7]);
    data.WriteByteSeq(groupGuid[4]);
    data.WriteByteSeq(playerGuid[3]);
    data.WriteByteSeq(groupGuid[5]);

    group->BroadcastPacket(&data, false);

    group->ReadyCheck(true);
    group->ReadyCheckMemberHasResponded(playerGuid);
    group->OfflineReadyCheck();

    // leader keeps track of ready check timer
    GetPlayer()->SetReadyCheckTimer(readyCheckDuration);
}

void WorldSession::HandleRaidReadyCheckConfirmOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_RAID_READY_CHECK_CONFIRM");

    ObjectGuid guid;    // currently unused

    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    if (!group->ReadyCheckInProgress())
        return;

    recvData.read_skip<uint8>();

    guid[2] = recvData.ReadBit();
    guid[1] = recvData.ReadBit();
    guid[0] = recvData.ReadBit();
    guid[3] = recvData.ReadBit();
    guid[6] = recvData.ReadBit();
    bool status = recvData.ReadBit();
    guid[7] = recvData.ReadBit();
    guid[4] = recvData.ReadBit();
    guid[5] = recvData.ReadBit();

    recvData.ReadByteSeq(guid[1]);
    recvData.ReadByteSeq(guid[0]);
    recvData.ReadByteSeq(guid[3]);
    recvData.ReadByteSeq(guid[2]);
    recvData.ReadByteSeq(guid[4]);
    recvData.ReadByteSeq(guid[5]);
    recvData.ReadByteSeq(guid[7]);
    recvData.ReadByteSeq(guid[6]);

    ObjectGuid groupGuid = group->GetGUID();
    ObjectGuid playerGuid = GetPlayer()->GetGUID();

    WorldPacket data(SMSG_RAID_READY_CHECK_CONFIRM, 1 + 1 + 8 + 1 + 8);
    data.WriteBit(groupGuid[4]);
    data.WriteBit(playerGuid[5]);
    data.WriteBit(playerGuid[3]);
    data.WriteBit(status);
    data.WriteBit(groupGuid[2]);
    data.WriteBit(playerGuid[6]);
    data.WriteBit(groupGuid[3]);
    data.WriteBit(playerGuid[0]);
    data.WriteBit(playerGuid[1]);
    data.WriteBit(groupGuid[1]);
    data.WriteBit(groupGuid[5]);
    data.WriteBit(playerGuid[7]);
    data.WriteBit(playerGuid[4]);
    data.WriteBit(groupGuid[6]);
    data.WriteBit(playerGuid[2]);
    data.WriteBit(groupGuid[0]);
    data.WriteBit(groupGuid[7]);
    data.FlushBits();

    data.WriteByteSeq(playerGuid[4]);
    data.WriteByteSeq(playerGuid[2]);
    data.WriteByteSeq(playerGuid[1]);
    data.WriteByteSeq(groupGuid[4]);
    data.WriteByteSeq(groupGuid[2]);
    data.WriteByteSeq(playerGuid[0]);
    data.WriteByteSeq(groupGuid[5]);
    data.WriteByteSeq(groupGuid[3]);
    data.WriteByteSeq(playerGuid[7]);
    data.WriteByteSeq(groupGuid[6]);
    data.WriteByteSeq(groupGuid[1]);
    data.WriteByteSeq(playerGuid[6]);
    data.WriteByteSeq(playerGuid[3]);
    data.WriteByteSeq(playerGuid[5]);
    data.WriteByteSeq(groupGuid[0]);
    data.WriteByteSeq(groupGuid[7]);

    group->BroadcastPacket(&data, false);
    group->ReadyCheckMemberHasResponded(playerGuid);

    if (group->ReadyCheckAllResponded())
    {
        Player* leader = ObjectAccessor::FindPlayer(group->GetLeaderGUID());
        if (leader)
            leader->SetReadyCheckTimer(0);

        group->ReadyCheck(false);
        group->ReadyCheckResetResponded();
        group->SendReadyCheckCompleted();
    }
}

void WorldSession::HandleRaidReadyCheckFinishedOpcode(WorldPacket& /*recvData*/)
{
    //Group* group = GetPlayer()->GetGroup();
    //if (!group)
    //    return;

    //if (!group->IsLeader(GetPlayer()->GetGUID()) && !group->IsAssistant(GetPlayer()->GetGUID()))
    //    return;

    // Is any reaction need?
}

void WorldSession::BuildPartyMemberStatsChangedPacket(Player* player, WorldPacket* data)
{
    uint32 mask = player->GetGroupUpdateFlag();
    ObjectGuid guid = player->GetGUID();
    ByteBuffer databuff;

    if (mask == GROUP_UPDATE_FLAG_NONE)
        return;

    std::set<uint32> const& phases = player->GetPhases();

    if (mask & GROUP_UPDATE_FLAG_POWER_TYPE)                // if update power type, update current/max power also
        mask |= (GROUP_UPDATE_FLAG_CUR_POWER | GROUP_UPDATE_FLAG_MAX_POWER);

    if (mask & GROUP_UPDATE_FLAG_PET_POWER_TYPE)            // same for pets
        mask |= (GROUP_UPDATE_FLAG_PET_CUR_POWER | GROUP_UPDATE_FLAG_PET_MAX_POWER);

    data->Initialize(SMSG_PARTY_MEMBER_STATE, 80);          // average value
    data->WriteBit(guid[0]);
    data->WriteBit(guid[5]);
    data->WriteBit(0); // ForEnemy
    data->WriteBit(guid[1]);
    data->WriteBit(guid[4]);
    data->WriteBit(1); // FullUpdate
    data->WriteBit(guid[6]);
    data->WriteBit(guid[2]);
    data->WriteBit(guid[7]);
    data->WriteBit(guid[3]);
    data->FlushBits();
    data->WriteByteSeq(guid[3]);
    data->WriteByteSeq(guid[2]);
    data->WriteByteSeq(guid[6]);
    data->WriteByteSeq(guid[7]);
    data->WriteByteSeq(guid[5]);
    *data << uint32(mask); // 6
    data->WriteByteSeq(guid[1]);
    data->WriteByteSeq(guid[4]);
    data->WriteByteSeq(guid[0]);

    if (mask & GROUP_UPDATE_FLAG_STATUS)
    {
        uint16 playerStatus = MEMBER_STATUS_ONLINE;
        if (player->IsPvP())
            playerStatus |= MEMBER_STATUS_PVP;

        if (!player->IsAlive())
        {
            if (player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_GHOST))
                playerStatus |= MEMBER_STATUS_GHOST;
            else
                playerStatus |= MEMBER_STATUS_DEAD;
        }

        if (player->HasByteFlag(UNIT_FIELD_SHAPESHIFT_FORM, 1, UNIT_BYTE2_FLAG_FFA_PVP))
            playerStatus |= MEMBER_STATUS_PVP_FFA;

        if (player->isAFK())
            playerStatus |= MEMBER_STATUS_AFK;

        if (player->isDND())
            playerStatus |= MEMBER_STATUS_DND;

        databuff << uint16(playerStatus);
    }

    /*
    if (mask & GROUP_UPDATE_FLAG_UNK)
    {
        databuff << uint8(1); // ?
        databuff << uint8(0); // ?
    }*/

    if (mask & GROUP_UPDATE_FLAG_CUR_HP)
        databuff << uint32(player->GetHealth());

    if (mask & GROUP_UPDATE_FLAG_MAX_HP)
        databuff << uint32(player->GetMaxHealth());

    if (mask & GROUP_UPDATE_FLAG_POWER_TYPE)
        databuff << uint8(player->getPowerType());

    //if (mask & GROUP_UPDATE_FLAG_UNK2)
    //    databuff << uint16(0);

    if (mask & GROUP_UPDATE_FLAG_CUR_POWER)
        databuff << uint16(player->GetPower(player->getPowerType()));

    if (mask & GROUP_UPDATE_FLAG_MAX_POWER)
        databuff << uint16(player->GetMaxPower(player->getPowerType()));

    if (mask & GROUP_UPDATE_FLAG_LEVEL)
        databuff << uint16(player->getLevel());

    if (mask & GROUP_UPDATE_FLAG_ZONE)
        databuff << uint16(player->GetZoneId());

    //if (mask & GROUP_UPDATE_FLAG_UNK400)
    //    databuff << uint16(0);

    if (mask & GROUP_UPDATE_FLAG_POSITION)
    {
        databuff << uint16(player->GetPositionX());
        databuff << uint16(player->GetPositionY());
        databuff << uint16(player->GetPositionZ());
    }
    /*
    if (mask & GROUP_UPDATE_FLAG_AURAS)
    {
        databuff << uint8(0);
        uint64 auramask = player->GetAuraUpdateMaskForRaid();
        databuff << uint64(auramask);
        databuff << uint32(MAX_AURAS); // count
        for (uint32 i = 0; i < MAX_AURAS; ++i)
        {
            if (auramask & (uint64(1) << i))
            {
                AuraApplication const* aurApp = player->GetVisibleAura(i);
                if (!aurApp)
                {
                    databuff << uint32(0);
                    databuff << uint16(0);
                    continue;
                }

                databuff << uint32(aurApp->GetBase()->GetId());
                databuff << uint16(aurApp->GetFlags());

                if (aurApp->GetFlags() & AFLAG_ANY_EFFECT_AMOUNT_SENT)
                {
                    for (uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                    {
                        if (AuraEffect const* eff = aurApp->GetBase()->GetEffect(i))
                            databuff << int32(eff->GetAmount());
                        else
                            databuff << int32(0);
                    }
                }
            }
        }
    }
    */
    Pet* pet = player->GetPet();
    if (mask & GROUP_UPDATE_FLAG_PET_GUID)
    {
        if (pet)
            databuff << uint64(pet->GetGUID());
        else
            databuff << uint64(0);
    }

    if (mask & GROUP_UPDATE_FLAG_PET_NAME)
    {
        if (pet)
            databuff << pet->GetName();
        else
            databuff << uint8(0);
    }

    if (mask & GROUP_UPDATE_FLAG_PET_MODEL_ID)
    {
        if (pet)
            databuff << uint16(pet->GetDisplayId());
        else
            databuff << uint16(0);
    }

    if (mask & GROUP_UPDATE_FLAG_PET_CUR_HP)
    {
        if (pet)
            databuff << uint32(pet->GetHealth());
        else
            databuff << uint32(0);
    }

    if (mask & GROUP_UPDATE_FLAG_PET_MAX_HP)
    {
        if (pet)
            databuff << uint32(pet->GetMaxHealth());
        else
            databuff << uint32(0);
    }

    if (mask & GROUP_UPDATE_FLAG_PET_POWER_TYPE)
    {
        if (pet)
            databuff << uint8(pet->getPowerType());
        else
            databuff << uint8(0);
    }

    //if (mask & GROUP_UPDATE_FLAG_UNK2)
    //    databuff << uint16(0);

    if (mask & GROUP_UPDATE_FLAG_PET_CUR_POWER)
    {
        if (pet)
            databuff << uint16(pet->GetPower(pet->getPowerType()));
        else
            databuff << uint16(0);
    }

    if (mask & GROUP_UPDATE_FLAG_PET_MAX_POWER)
    {
        if (pet)
            databuff << uint16(pet->GetMaxPower(pet->getPowerType()));
        else
            databuff << uint16(0);
    }

    /*
    if (mask & GROUP_UPDATE_FLAG_PET_AURAS)
    {
        if (pet)
        {
            *data << uint8(0);
            uint64 auramask = pet->GetAuraUpdateMaskForRaid();
            *data << uint64(auramask);
            *data << uint32(MAX_AURAS); // count
            for (uint32 i = 0; i < MAX_AURAS; ++i)
            {
                if (auramask & (uint64(1) << i))
                {
                    AuraApplication const* aurApp = pet->GetVisibleAura(i);
                    if (!aurApp)
                    {
                        *data << uint32(0);
                        *data << uint16(0);
                        continue;
                    }

                    *data << uint32(aurApp->GetBase()->GetId());
                    *data << uint16(aurApp->GetFlags());

                    if (aurApp->GetFlags() & AFLAG_ANY_EFFECT_AMOUNT_SENT)
                    {
                        for (uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                        {
                            if (AuraEffect const* eff = aurApp->GetBase()->GetEffect(i))
                                *data << int32(eff->GetAmount());
                            else
                                *data << int32(0);
                        }
                    }
                }
            }
        }
        else
        {
            *data << uint8(0);
            *data << uint64(0);
        }
    }
    */

    if (mask & GROUP_UPDATE_FLAG_VEHICLE_SEAT)
    {
        if (Vehicle* veh = player->GetVehicle())
            databuff << uint32(veh->GetVehicleInfo()->m_seatID[player->m_movementInfo.transport.seat]);
        else
            databuff << uint32(0);
    }

    if (mask & GROUP_UPDATE_FLAG_PHASE)
    {
        databuff << uint32(phases.empty() ? 8 : 0);
        databuff << uint32(phases.size());
        for (std::set<uint32>::const_iterator itr = phases.begin(); itr != phases.end(); ++itr)
            *data << uint16(*itr);
    }

    *data << uint32(databuff.size());
    data->append(databuff);
}

/*this procedure handles clients CMSG_REQUEST_PARTY_MEMBER_STATS request*/
void WorldSession::HandleRequestPartyMemberStatsOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_REQUEST_PARTY_MEMBER_STATS");
    ObjectGuid guid;

    recvData.read_skip<uint8>(); // flags

    guid[7] = recvData.ReadBit();
    guid[4] = recvData.ReadBit();
    guid[0] = recvData.ReadBit();
    guid[1] = recvData.ReadBit();
    guid[3] = recvData.ReadBit();
    guid[6] = recvData.ReadBit();
    guid[2] = recvData.ReadBit();
    guid[5] = recvData.ReadBit();

    recvData.ReadByteSeq(guid[3]);
    recvData.ReadByteSeq(guid[6]);
    recvData.ReadByteSeq(guid[5]);
    recvData.ReadByteSeq(guid[2]);
    recvData.ReadByteSeq(guid[1]);
    recvData.ReadByteSeq(guid[4]);
    recvData.ReadByteSeq(guid[0]);
    recvData.ReadByteSeq(guid[7]);

    Player* player = HashMapHolder<Player>::Find(guid);
    if (!player)
        return;

    WorldPacket data;
    BuildPartyMemberStatsChangedPacket(player, &data);
}

void WorldSession::HandleRequestRaidInfoOpcode(WorldPacket& /*recvData*/)
{
    // every time the player checks the character screen
    _player->SendRaidInfo();
}

void WorldSession::HandleOptOutOfLootOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_OPT_OUT_OF_LOOT");

    bool passOnLoot;
    recvData >> passOnLoot; // 1 always pass, 0 do not pass

    // ignore if player not loaded
    if (!GetPlayer())                                        // needed because STATUS_AUTHED
    {
        if (passOnLoot)
            SF_LOG_ERROR("network", "CMSG_OPT_OUT_OF_LOOT value<>0 for not-loaded character!");
        return;
    }

    GetPlayer()->SetPassOnGroupLoot(passOnLoot);
}

void WorldSession::HandleGroupInitiatePollRole(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_GROUP_INITIATE_ROLE_POLL");

    uint8 Index = 0;
    recvData >> Index;
}

void WorldSession::SendRolePollInform(ObjectGuid guid, uint8 Index)
{
    WorldPacket data(SMSG_GROUP_ROLE_POLL_INFORM, 8 + 1);
    data.WriteGuidMask(guid, 5, 7, 3, 1, 2, 0, 4, 6);
    data.WriteGuidBytes(guid, 7);
    data << uint8(Index);
    data.WriteGuidBytes(guid, 6, 5, 0, 1, 4, 2, 3);
    GetPlayer()->GetGroup()->BroadcastPacket(&data, false, -1);
}
