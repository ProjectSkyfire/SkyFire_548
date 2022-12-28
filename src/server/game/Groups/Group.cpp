/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "Common.h"
#include "Opcodes.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Player.h"
#include "World.h"
#include "ObjectMgr.h"
#include "GroupMgr.h"
#include "Group.h"
#include "Formulas.h"
#include "ObjectAccessor.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "MapManager.h"
#include "InstanceSaveMgr.h"
#include "MapInstanced.h"
#include "Util.h"
#include "LFGMgr.h"
#include "UpdateFieldFlags.h"

Roll::Roll(uint64 _guid, LootItem const& li) : itemGUID(_guid), itemid(li.itemid),
itemRandomPropId(li.randomPropertyId), itemRandomSuffix(li.randomSuffix), itemCount(li.count),
totalPlayersRolling(0), totalNeed(0), totalGreed(0), totalPass(0), itemSlot(0),
rollVoteMask(uint8(RollMask::ROLL_ALL_TYPE_NO_DISENCHANT)) { }

Roll::~Roll() { }

void Roll::setLoot(Loot* pLoot)
{
    link(pLoot, this);
}

Loot* Roll::getLoot()
{
    return getTarget();
}

Group::Group() : m_leaderGuid(0), m_leaderName(""), m_groupType(GROUPTYPE_NORMAL),
m_dungeonDifficulty(DIFFICULTY_NORMAL), m_raidDifficulty(DIFFICULTY_10MAN_NORMAL),
m_bgGroup(NULL), m_bfGroup(NULL), m_lootMethod(LootMethod::FREE_FOR_ALL), m_lootThreshold(ITEM_QUALITY_UNCOMMON), m_looterGuid(0),
m_subGroupsCounts(NULL), m_guid(0), m_counter(0), m_maxEnchantingLevel(0), m_dbStoreId(0), _readyCheckInProgress(false)
{
    for (uint8 i = 0; i < TARGETICONCOUNT; ++i)
        m_targetIcons[i] = 0;
}

Group::~Group()
{
    if (m_bgGroup)
    {
        SF_LOG_DEBUG("bg.battleground", "Group::~Group: battleground group being deleted.");
        if (m_bgGroup->GetBgRaid(ALLIANCE) == this) m_bgGroup->SetBgRaid(ALLIANCE, NULL);
        else if (m_bgGroup->GetBgRaid(HORDE) == this) m_bgGroup->SetBgRaid(HORDE, NULL);
        else SF_LOG_ERROR("misc", "Group::~Group: battleground group is not linked to the correct battleground.");
    }
    Rolls::iterator itr;
    while (!RollId.empty())
    {
        itr = RollId.begin();
        Roll *r = *itr;
        RollId.erase(itr);
        delete (r);
    }

    // it is undefined whether objectmgr (which stores the groups) or instancesavemgr
    // will be unloaded first so we must be prepared for both cases
    // this may unload some instance saves
    for (uint8 i = 0; i < 15; ++i)
        for (BoundInstancesMap::iterator itr2 = m_boundInstances[i].begin(); itr2 != m_boundInstances[i].end(); ++itr2)
            itr2->second.save->RemoveGroup(this);

    // Sub group counters clean up
    delete [] m_subGroupsCounts;
}

bool Group::Create(Player* leader)
{
    uint64 leaderGuid = leader->GetGUID();
    uint32 lowguid = sGroupMgr->GenerateGroupId();

    m_guid = MAKE_NEW_GUID(lowguid, 0, HIGHGUID_GROUP);
    m_leaderGuid = leaderGuid;
    m_leaderName = leader->GetName();

    if (isBGGroup() || isBFGroup())
        m_groupType = GROUPTYPE_BGRAID;

    if (m_groupType & GROUPTYPE_RAID)
        _initRaidSubGroupsCounter();

    if (!isLFGGroup())
        m_lootMethod = LootMethod::GROUP_LOOT;

    m_lootThreshold = ITEM_QUALITY_UNCOMMON;
    m_looterGuid = leaderGuid;

    m_dungeonDifficulty = DIFFICULTY_NORMAL;
    m_raidDifficulty = DIFFICULTY_10MAN_NORMAL;

    if (!isBGGroup() && !isBFGroup())
    {
        m_dungeonDifficulty = leader->GetDungeonDifficulty();
        m_raidDifficulty = leader->GetRaidDifficulty();

        m_dbStoreId = sGroupMgr->GenerateNewGroupDbStoreId();

        sGroupMgr->RegisterGroupDbStoreId(m_dbStoreId, this);

        // Store group in database
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GROUP);

        uint8 index = 0;

        stmt->setUInt32(index++, m_dbStoreId);
        stmt->setUInt32(index++, GUID_LOPART(m_leaderGuid));
        stmt->setUInt8(index++, uint8(m_lootMethod));
        stmt->setUInt32(index++, GUID_LOPART(m_looterGuid));
        stmt->setUInt8(index++, uint8(m_lootThreshold));
        stmt->setUInt32(index++, uint32(m_targetIcons[0]));
        stmt->setUInt32(index++, uint32(m_targetIcons[1]));
        stmt->setUInt32(index++, uint32(m_targetIcons[2]));
        stmt->setUInt32(index++, uint32(m_targetIcons[3]));
        stmt->setUInt32(index++, uint32(m_targetIcons[4]));
        stmt->setUInt32(index++, uint32(m_targetIcons[5]));
        stmt->setUInt32(index++, uint32(m_targetIcons[6]));
        stmt->setUInt32(index++, uint32(m_targetIcons[7]));
        stmt->setUInt8(index++, uint8(m_groupType));
        stmt->setUInt32(index++, uint8(m_dungeonDifficulty));
        stmt->setUInt32(index++, uint8(m_raidDifficulty));

        CharacterDatabase.Execute(stmt);


        ASSERT(AddMember(leader)); // If the leader can't be added to a new group because it appears full, something is clearly wrong.

        Player::ConvertInstancesToGroup(leader, this, false);
    }
    else if (!AddMember(leader))
        return false;

    return true;
}

void Group::LoadGroupFromDB(Field* fields)
{
    m_dbStoreId = fields[15].GetUInt32();
    m_guid = MAKE_NEW_GUID(sGroupMgr->GenerateGroupId(), 0, HIGHGUID_GROUP);
    m_leaderGuid = MAKE_NEW_GUID(fields[0].GetUInt32(), 0, HIGHGUID_PLAYER);

    // group leader not exist
    if (!sObjectMgr->GetPlayerNameByGUID(fields[0].GetUInt32(), m_leaderName))
        return;

    m_lootMethod = LootMethod(fields[1].GetUInt8());
    m_looterGuid = MAKE_NEW_GUID(fields[2].GetUInt32(), 0, HIGHGUID_PLAYER);
    m_lootThreshold = ItemQualities(fields[3].GetUInt8());

    for (uint8 i = 0; i < TARGETICONCOUNT; ++i)
        m_targetIcons[i] = fields[4+i].GetUInt32();

    m_groupType  = GroupType(fields[12].GetUInt8());
    if (m_groupType & GROUPTYPE_RAID)
        _initRaidSubGroupsCounter();

    m_dungeonDifficulty = Player::CheckLoadedDungeonDifficultyID(DifficultyID(fields[13].GetUInt8()));
    m_raidDifficulty = Player::CheckLoadedRaidDifficultyID(DifficultyID(fields[14].GetUInt8()));

    if (m_groupType & GROUPTYPE_LFG)
        sLFGMgr->_LoadFromDB(fields, GetGUID());
}

void Group::LoadMemberFromDB(uint32 guidLow, uint8 memberFlags, uint8 subgroup, uint8 roles)
{
    MemberSlot member;
    member.guid = MAKE_NEW_GUID(guidLow, 0, HIGHGUID_PLAYER);

    // skip non-existed member
    if (!sObjectMgr->GetPlayerNameByGUID(member.guid, member.name))
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP_MEMBER);
        stmt->setUInt32(0, guidLow);
        CharacterDatabase.Execute(stmt);
        return;
    }

    member.group = subgroup;
    member.flags = memberFlags;
    member.roles = roles;

    m_memberSlots.push_back(member);

    SubGroupCounterIncrease(subgroup);

    sLFGMgr->SetupGroupMember(member.guid, GetGUID());
}

void Group::ChangeFlagEveryoneAssistant(bool apply)
{
    if (apply)
        m_groupType = GroupType(m_groupType | GROUPTYPE_EVERYONE_IS_ASSISTANT);
    else
        m_groupType = GroupType(m_groupType &~GROUPTYPE_EVERYONE_IS_ASSISTANT);

    this->SendUpdate();
}

void Group::ConvertToLFG()
{
    m_groupType = GroupType(m_groupType | GROUPTYPE_LFG | GROUPTYPE_UNK1);
    m_lootMethod = LootMethod::NEED_BEFORE_GREED;
    if (!isBGGroup() && !isBFGroup())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_TYPE);

        stmt->setUInt8(0, uint8(m_groupType));
        stmt->setUInt32(1, m_dbStoreId);

        CharacterDatabase.Execute(stmt);
    }

    SendUpdate();
}

void Group::ConvertToRaid()
{
    m_groupType = GroupType(m_groupType | GROUPTYPE_RAID);

    _initRaidSubGroupsCounter();

    if (!isBGGroup() && !isBFGroup())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_TYPE);

        stmt->setUInt8(0, uint8(m_groupType));
        stmt->setUInt32(1, m_dbStoreId);

        CharacterDatabase.Execute(stmt);
    }

    SendUpdate();

    // update quest related GO states (quest activity dependent from raid membership)
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
        if (Player* player = ObjectAccessor::FindPlayer(citr->guid))
            player->UpdateForQuestWorldObjects();
}

void Group::ConvertToGroup()
{
    if (m_memberSlots.size() > 5)
        return; // What message error should we send?

    m_groupType = GroupType(GROUPTYPE_NORMAL);

    if (m_subGroupsCounts)
    {
        delete [] m_subGroupsCounts;
        m_subGroupsCounts = NULL;
    }

    if (!isBGGroup() && !isBFGroup())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_TYPE);

        stmt->setUInt8(0, uint8(m_groupType));
        stmt->setUInt32(1, m_dbStoreId);

        CharacterDatabase.Execute(stmt);
    }

    SendUpdate();

    // update quest related GO states (quest activity dependent from raid membership)
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
        if (Player* player = ObjectAccessor::FindPlayer(citr->guid))
            player->UpdateForQuestWorldObjects();
}

bool Group::AddInvite(Player* player)
{
    if (!player || player->GetGroupInvite())
        return false;
    Group* group = player->GetGroup();
    if (group && (group->isBGGroup() || group->isBFGroup()))
        group = player->GetOriginalGroup();
    if (group)
        return false;

    RemoveInvite(player);

    m_invitees.insert(player);

    player->SetGroupInvite(this);

    sScriptMgr->OnGroupInviteMember(this, player->GetGUID());

    return true;
}

bool Group::AddLeaderInvite(Player* player)
{
    if (!AddInvite(player))
        return false;

    m_leaderGuid = player->GetGUID();
    m_leaderName = player->GetName();
    return true;
}

void Group::RemoveInvite(Player* player)
{
    if (player)
    {
        m_invitees.erase(player);
        player->SetGroupInvite(NULL);
    }
}

void Group::RemoveAllInvites()
{
    for (InvitesList::iterator itr=m_invitees.begin(); itr != m_invitees.end(); ++itr)
        if (*itr)
            (*itr)->SetGroupInvite(NULL);

    m_invitees.clear();
}

Player* Group::GetInvited(uint64 guid) const
{
    for (InvitesList::const_iterator itr = m_invitees.begin(); itr != m_invitees.end(); ++itr)
    {
        if ((*itr) && (*itr)->GetGUID() == guid)
            return (*itr);
    }
    return NULL;
}

Player* Group::GetInvited(const std::string& name) const
{
    for (InvitesList::const_iterator itr = m_invitees.begin(); itr != m_invitees.end(); ++itr)
    {
        if ((*itr) && (*itr)->GetName() == name)
            return (*itr);
    }
    return NULL;
}

bool Group::AddMember(Player* player)
{
    // Get first not-full group
    uint8 subGroup = 0;
    if (m_subGroupsCounts)
    {
        bool groupFound = false;
        for (; subGroup < MAX_RAID_SUBGROUPS; ++subGroup)
        {
            if (m_subGroupsCounts[subGroup] < MAXGROUPSIZE)
            {
                groupFound = true;
                break;
            }
        }
        // We are raid group and no one slot is free
        if (!groupFound)
            return false;
    }

    MemberSlot member;
    member.guid      = player->GetGUID();
    member.name      = player->GetName();
    member.group     = subGroup;
    member.flags     = 0;
    member.roles     = 0;
    m_memberSlots.push_back(member);

    SubGroupCounterIncrease(subGroup);

    player->SetGroupInvite(NULL);
    if (player->GetGroup())
    {
        if (isBGGroup() || isBFGroup()) // if player is in group and he is being added to BG raid group, then call SetBattlegroundRaid()
            player->SetBattlegroundOrBattlefieldRaid(this, subGroup);
        else //if player is in bg raid and we are adding him to normal group, then call SetOriginalGroup()
            player->SetOriginalGroup(this, subGroup);
    }
    else //if player is not in group, then call set group
        player->SetGroup(this, subGroup);

    // if the same group invites the player back, cancel the homebind timer
    InstanceGroupBind* bind = GetBoundInstance(player);
    if (bind && bind->save->GetInstanceId() == player->GetInstanceId())
        player->m_InstanceValid = true;

    if (!isRaidGroup())                                      // reset targetIcons for non-raid-groups
    {
        for (uint8 i = 0; i < TARGETICONCOUNT; ++i)
            m_targetIcons[i] = 0;
    }

    // insert into the table if we're not a battleground group
    if (!isBGGroup() && !isBFGroup())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GROUP_MEMBER);

        stmt->setUInt32(0, m_dbStoreId);
        stmt->setUInt32(1, GUID_LOPART(member.guid));
        stmt->setUInt8(2, member.flags);
        stmt->setUInt8(3, member.group);
        stmt->setUInt8(4, member.roles);

        CharacterDatabase.Execute(stmt);
    }

    SendUpdate();
    sScriptMgr->OnGroupAddMember(this, player->GetGUID());

    if (!IsLeader(player->GetGUID()) && !isBGGroup() && !isBFGroup())
    {
        // reset the new member's instances, unless he is currently in one of them
        // including raid/heroic instances that they are not permanently bound to!
        player->ResetInstances(InstanceResetMethod::INSTANCE_RESET_GROUP_JOIN, false);
        player->ResetInstances(InstanceResetMethod::INSTANCE_RESET_GROUP_JOIN, true);

        if (player->getLevel() >= LEVELREQUIREMENT_HEROIC)
        {
            if (player->GetDungeonDifficulty() != GetDungeonDifficulty())
            {
                player->SetDungeonDifficulty(GetDungeonDifficulty());
                player->SendDungeonDifficulty();
            }
            if (player->GetRaidDifficulty() != GetRaidDifficulty())
            {
                player->SetRaidDifficulty(GetRaidDifficulty());
                player->SendRaidDifficulty();
            }
        }
    }
    player->SetGroupUpdateFlag(GROUP_UPDATE_FULL);
    UpdatePlayerOutOfRange(player);

    // quest related GO state dependent from raid membership
    if (isRaidGroup())
        player->UpdateForQuestWorldObjects();

    {
        // Broadcast new player group member fields to rest of the group
        player->SetFieldNotifyFlag(UF_FLAG_PARTY_MEMBER);

        UpdateData groupData(player->GetMapId());
        WorldPacket groupDataPacket;

        // Broadcast group members' fields to player
        for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
        {
            if (itr->GetSource() == player)
                continue;

            if (Player* member = itr->GetSource())
            {
                if (player->HaveAtClient(member))
                {
                    member->SetFieldNotifyFlag(UF_FLAG_PARTY_MEMBER);
                    member->BuildValuesUpdateBlockForPlayer(&groupData, player);
                    member->RemoveFieldNotifyFlag(UF_FLAG_PARTY_MEMBER);
                }

                if (member->HaveAtClient(player))
                {
                    UpdateData newData(player->GetMapId());
                    WorldPacket newDataPacket;
                    player->BuildValuesUpdateBlockForPlayer(&newData, member);
                    if (newData.HasData())
                    {
                        newData.BuildPacket(&newDataPacket);
                        member->SendDirectMessage(&newDataPacket);
                    }
                }
            }
        }

        if (groupData.HasData())
        {
            groupData.BuildPacket(&groupDataPacket);
            player->SendDirectMessage(&groupDataPacket);
        }

        player->RemoveFieldNotifyFlag(UF_FLAG_PARTY_MEMBER);
    }

    if (m_maxEnchantingLevel < player->GetSkillValue(SKILL_ENCHANTING))
        m_maxEnchantingLevel = player->GetSkillValue(SKILL_ENCHANTING);

    return true;
}

bool Group::RemoveMember(uint64 guid, const RemoveMethod& method /*= GROUP_REMOVEMETHOD_DEFAULT*/, uint64 kicker /*= 0*/, const char* reason /*= NULL*/)
{
    BroadcastGroupUpdate();

    sScriptMgr->OnGroupRemoveMember(this, guid, method, kicker, reason);

    // LFG group vote kick handled in scripts
    if (isLFGGroup() && method == GROUP_REMOVEMETHOD_KICK)
        return m_memberSlots.size();

    // remove member and change leader (if need) only if strong more 2 members _before_ member remove (BG/BF allow 1 member group)
    if (GetMembersCount() > ((isBGGroup() || isLFGGroup() || isBFGroup()) ? 1u : 2u))
    {
        Player* player = ObjectAccessor::FindPlayer(guid);
        if (player)
        {
            // Battleground group handling
            if (isBGGroup() || isBFGroup())
                player->RemoveFromBattlegroundOrBattlefieldRaid();
            else
            // Regular group
            {
                if (player->GetOriginalGroup() == this)
                    player->SetOriginalGroup(NULL);
                else
                    player->SetGroup(NULL);

                // quest related GO state dependent from raid membership
                player->UpdateForQuestWorldObjects();
            }

            WorldPacket data;

            if (method == GROUP_REMOVEMETHOD_KICK || method == GROUP_REMOVEMETHOD_KICK_LFG)
            {
                data.Initialize(SMSG_GROUP_UNINVITE, 0);
                player->GetSession()->SendPacket(&data);
            }

            SendUpdateToPlayer(guid, NULL);

            _homebindIfInstance(player);
        }

        // Remove player from group in DB
        if (!isBGGroup() && !isBFGroup())
        {
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP_MEMBER);
            stmt->setUInt32(0, GUID_LOPART(guid));
            CharacterDatabase.Execute(stmt);
            DelinkMember(guid);
        }

        // Reevaluate group enchanter if the leaving player had enchanting skill or the player is offline
        if ((player && player->GetSkillValue(SKILL_ENCHANTING)) || !player)
            ResetMaxEnchantingLevel();

        // Remove player from loot rolls
        for (Rolls::iterator it = RollId.begin(); it != RollId.end(); ++it)
        {
            Roll* roll = *it;
            Roll::PlayerVote::iterator itr2 = roll->playerVote.find(guid);
            if (itr2 == roll->playerVote.end())
                continue;

            if (itr2->second == RollType::ROLL_GREED || itr2->second == RollType::ROLL_DISENCHANT)
                --roll->totalGreed;
            else if (itr2->second == RollType::ROLL_NEED)
                --roll->totalNeed;
            else if (itr2->second == RollType::ROLL_PASS)
                --roll->totalPass;

            if (itr2->second != RollType::ROLL_INVALID)
                --roll->totalPlayersRolling;

            roll->playerVote.erase(itr2);

            CountRollVote(guid, roll->itemGUID, RollType::MAX_ROLL_TYPE);
        }

        // Update subgroups
        member_witerator slot = _getMemberWSlot(guid);
        if (slot != m_memberSlots.end())
        {
            SubGroupCounterDecrease(slot->group);
            m_memberSlots.erase(slot);
        }

        // Pick new leader if necessary
        if (m_leaderGuid == guid)
        {
            for (member_witerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
            {
                if (ObjectAccessor::FindPlayer(itr->guid))
                {
                    ChangeLeader(itr->guid);
                    break;
                }
            }
        }

        SendUpdate();

        if (isLFGGroup() && GetMembersCount() == 1)
        {
            Player* leader = ObjectAccessor::FindPlayer(GetLeaderGUID());
            uint32 mapId = sLFGMgr->GetDungeonMapId(GetGUID());
            if (!mapId || !leader || (leader->IsAlive() && leader->GetMapId() != mapId))
            {
                Disband();
                return false;
            }
        }

        if (m_memberMgr.getSize() < ((isLFGGroup() || isBGGroup()) ? 1u : 2u))
            Disband();

        return true;
    }
    // If group size before player removal <= 2 then disband it
    else
    {
        Disband();
        return false;
    }
}

void Group::ChangeLeader(uint64 newLeaderGuid)
{
    member_witerator slot = _getMemberWSlot(newLeaderGuid);

    if (slot == m_memberSlots.end())
        return;

    Player* newLeader = ObjectAccessor::FindPlayer(slot->guid);

    // Don't allow switching leader to offline players
    if (!newLeader)
        return;

    sScriptMgr->OnGroupChangeLeader(this, newLeaderGuid, m_leaderGuid);

    if (!isBGGroup() && !isBFGroup())
    {
        SQLTransaction trans = CharacterDatabase.BeginTransaction();

        // Remove the groups permanent instance bindings
        for (uint8 i = 0; i < 15; ++i)
        {
            for (BoundInstancesMap::iterator itr = m_boundInstances[i].begin(); itr != m_boundInstances[i].end();)
            {
                // Do not unbind saves of instances that already had map created (a newLeader entered)
                // forcing a new instance with another leader requires group disbanding (confirmed on retail)
                if (itr->second.perm && !sMapMgr->FindMap(itr->first, itr->second.save->GetInstanceId()))
                {
                    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP_INSTANCE_PERM_BINDING);
                    stmt->setUInt32(0, m_dbStoreId);
                    stmt->setUInt32(1, itr->second.save->GetInstanceId());
                    trans->Append(stmt);

                    itr->second.save->RemoveGroup(this);
                    m_boundInstances[i].erase(itr++);
                }
                else
                    ++itr;
            }
        }

        // Copy the permanent binds from the new leader to the group
        Player::ConvertInstancesToGroup(newLeader, this, true);

        // Update the group leader
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_LEADER);

        stmt->setUInt32(0, newLeader->GetGUIDLow());
        stmt->setUInt32(1, m_dbStoreId);

        trans->Append(stmt);

        CharacterDatabase.CommitTransaction(trans);
    }

    m_leaderGuid = newLeader->GetGUID();
    m_leaderName = newLeader->GetName();
    ToggleGroupMemberFlag(slot, GroupMemberFlags::MEMBER_FLAG_ASSISTANT, false);

    uint8 leaderNameLen = m_leaderName.size();

    WorldPacket data(SMSG_GROUP_SET_LEADER, 1 + 1 + leaderNameLen);
    data << uint8(0);

    data.WriteBits(leaderNameLen, 6);
    data.FlushBits();

    data.WriteString(m_leaderName);

    BroadcastPacket(&data, true);
}

void Group::Disband(bool hideDestroy /* = false */)
{
    sScriptMgr->OnGroupDisband(this);

    Player* player;
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
    {
        player = ObjectAccessor::FindPlayer(citr->guid);
        if (!player)
            continue;

        //we cannot call _removeMember because it would invalidate member iterator
        //if we are removing player from battleground raid
        if (isBGGroup() || isBFGroup())
            player->RemoveFromBattlegroundOrBattlefieldRaid();
        else
        {
            //we can remove player who is in battleground from his original group
            if (player->GetOriginalGroup() == this)
                player->SetOriginalGroup(NULL);
            else
                player->SetGroup(NULL);
        }

        // quest related GO state dependent from raid membership
        if (isRaidGroup())
            player->UpdateForQuestWorldObjects();

        if (!player->GetSession())
            continue;

        WorldPacket data;
        if (!hideDestroy)
        {
            data.Initialize(SMSG_GROUP_DESTROYED, 0);
            player->GetSession()->SendPacket(&data);
        }

        //we already removed player from group and in player->GetGroup() is his original group, send update
        if (Group* group = player->GetGroup())
            group->SendUpdate();
        else
            SendUpdateToPlayer(player->GetGUID(), NULL);

        _homebindIfInstance(player);
    }
    RollId.clear();
    m_memberSlots.clear();

    RemoveAllInvites();

    if (!isBGGroup() && !isBFGroup())
    {
        SQLTransaction trans = CharacterDatabase.BeginTransaction();

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP);
        stmt->setUInt32(0, m_dbStoreId);
        trans->Append(stmt);

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP_MEMBER_ALL);
        stmt->setUInt32(0, m_dbStoreId);
        trans->Append(stmt);

        CharacterDatabase.CommitTransaction(trans);

        ResetInstances(InstanceResetMethod::INSTANCE_RESET_GROUP_DISBAND, false, NULL);
        ResetInstances(InstanceResetMethod::INSTANCE_RESET_GROUP_DISBAND, true, NULL);

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_LFG_DATA);
        stmt->setUInt32(0, m_dbStoreId);
        CharacterDatabase.Execute(stmt);

        sGroupMgr->FreeGroupDbStoreId(this);
    }

    sGroupMgr->RemoveGroup(this);
    delete this;
}

/*********************************************************/
/***                   LOOT SYSTEM                     ***/
/*********************************************************/

void Group::SendLootStartRoll(uint32 countDown, uint32 mapid, const Roll &r)
{
    WorldPacket data(SMSG_LOOT_START_ROLL, (8+4+4+4+4+4+4+1));
    data << uint64(r.itemGUID);                             // guid of rolled item
    data << uint32(mapid);                                  // 3.3.3 mapid
    data << uint32(r.itemSlot);                             // itemslot
    data << uint32(r.itemid);                               // the itemEntryId for the item that shall be rolled for
    data << uint32(r.itemRandomSuffix);                     // randomSuffix
    data << uint32(r.itemRandomPropId);                     // item random property ID
    data << uint32(r.itemCount);                            // items in stack
    data << uint32(countDown);                              // the countdown time to choose "need" or "greed"
    data << uint8(r.rollVoteMask);                          // roll type mask
    data << uint8(r.totalPlayersRolling);                   // maybe the number of players rolling for it???

    for (Roll::PlayerVote::const_iterator itr=r.playerVote.begin(); itr != r.playerVote.end(); ++itr)
    {
        Player* p = ObjectAccessor::FindPlayer(itr->first);
        if (!p || !p->GetSession())
            continue;

        if (itr->second == RollType::MAX_ROLL_TYPE)
            p->GetSession()->SendPacket(&data);
    }
}

void Group::SendLootStartRollToPlayer(uint32 countDown, uint32 mapId, Player* p, bool canNeed, Roll const& r)
{
    if (!p || !p->GetSession())
        return;

    WorldPacket data(SMSG_LOOT_START_ROLL, (8 + 4 + 4 + 4 + 4 + 4 + 4 + 1));
    data << uint64(r.itemGUID);                             // guid of rolled item
    data << uint32(mapId);                                  // 3.3.3 mapid
    data << uint32(r.itemSlot);                             // itemslot
    data << uint32(r.itemid);                               // the itemEntryId for the item that shall be rolled for
    data << uint32(r.itemRandomSuffix);                     // randomSuffix
    data << uint32(r.itemRandomPropId);                     // item random property ID
    data << uint32(r.itemCount);                            // items in stack
    data << uint32(countDown);                              // the countdown time to choose "need" or "greed"
    uint8 voteMask = uint8(r.rollVoteMask);
    if (!canNeed)
        voteMask &= ~uint8(RollMask::ROLL_FLAG_TYPE_NEED);
    data << uint8(voteMask);                                // roll type mask
    data << uint8(r.totalPlayersRolling);                   // maybe the number of players rolling for it???

    p->GetSession()->SendPacket(&data);
}

void Group::SendLootRoll(uint64 sourceGuid, uint64 targetGuid, uint8 rollNumber, RollType rollType, Roll const& roll)
{
    WorldPacket data(SMSG_LOOT_ROLL, (8+4+8+4+4+4+1+1+1));
    data << uint64(sourceGuid);                             // guid of the item rolled
    data << uint32(roll.itemSlot);                          // slot
    data << uint64(targetGuid);
    data << uint32(roll.itemid);                            // the itemEntryId for the item that shall be rolled for
    data << uint32(roll.itemRandomSuffix);                  // randomSuffix
    data << uint32(roll.itemRandomPropId);                  // Item random property ID
    data << uint32(rollNumber);                             // 0: "Need for: [item name]" > 127: "you passed on: [item name]"      Roll number
    data << uint8(rollType);                                // 0: "Need for: [item name]" 0: "You have selected need for [item name] 1: need roll 2: greed roll
    data << uint8(0);                                       // 1: "You automatically passed on: %s because you cannot loot that item." - Possibly used in need befor greed

    for (Roll::PlayerVote::const_iterator itr = roll.playerVote.begin(); itr != roll.playerVote.end(); ++itr)
    {
        Player* p = ObjectAccessor::FindPlayer(itr->first);
        if (!p || !p->GetSession())
            continue;

        if (itr->second != RollType::ROLL_INVALID)
            p->GetSession()->SendPacket(&data);
    }
}

void Group::SendLootRollWon(uint64 sourceGuid, uint64 targetGuid, uint8 rollNumber, RollType rollType, Roll const& roll)
{
    WorldPacket data(SMSG_LOOT_ROLL_WON, (8+4+4+4+4+8+1+1));
    data << uint64(sourceGuid);                             // guid of the item rolled
    data << uint32(roll.itemSlot);                          // slot
    data << uint32(roll.itemid);                            // the itemEntryId for the item that shall be rolled for
    data << uint32(roll.itemRandomSuffix);                  // randomSuffix
    data << uint32(roll.itemRandomPropId);                  // Item random property
    data << uint64(targetGuid);                             // guid of the player who won.
    data << uint32(rollNumber);                             // rollnumber realted to SMSG_LOOT_ROLL
    data << uint8(rollType);                                // rollType related to SMSG_LOOT_ROLL

    for (Roll::PlayerVote::const_iterator itr = roll.playerVote.begin(); itr != roll.playerVote.end(); ++itr)
    {
        Player* p = ObjectAccessor::FindPlayer(itr->first);
        if (!p || !p->GetSession())
            continue;

        if (itr->second != RollType::ROLL_INVALID)
            p->GetSession()->SendPacket(&data);
    }
}

void Group::SendLootAllPassed(Roll const& roll)
{
    WorldPacket data(SMSG_LOOT_ALL_PASSED, (8+4+4+4+4));
    data << uint64(roll.itemGUID);                             // Guid of the item rolled
    data << uint32(roll.itemSlot);                             // Item loot slot
    data << uint32(roll.itemid);                               // The itemEntryId for the item that shall be rolled for
    data << uint32(roll.itemRandomPropId);                     // Item random property ID
    data << uint32(roll.itemRandomSuffix);                     // Item random suffix ID

    for (Roll::PlayerVote::const_iterator itr = roll.playerVote.begin(); itr != roll.playerVote.end(); ++itr)
    {
        Player* player = ObjectAccessor::FindPlayer(itr->first);
        if (!player || !player->GetSession())
            continue;

        if (itr->second != RollType::ROLL_INVALID)
            player->GetSession()->SendPacket(&data);
    }
}

// notify group members which player is the allowed looter for the given creature
void Group::SendLooter(Creature* creature, Player* pLooter)
{
    ASSERT(creature);

    ObjectGuid creatureGuid = creature->GetGUID();
    ObjectGuid looterGuid = pLooter ? pLooter->GetGUID() : 0;
    ObjectGuid masterLooterGuid = GetLootMethod() == LootMethod::MASTER_LOOT ? GetLooterGuid() : 0;
    WorldPacket data(SMSG_LOOT_LIST);

    data.WriteBit(creatureGuid[5]);
    data.WriteBit(masterLooterGuid != (uint64)0);
    if (masterLooterGuid)
    {
        data.WriteBit(masterLooterGuid[4]);
        data.WriteBit(masterLooterGuid[6]);
        data.WriteBit(masterLooterGuid[0]);
        data.WriteBit(masterLooterGuid[7]);
        data.WriteBit(masterLooterGuid[5]);
        data.WriteBit(masterLooterGuid[2]);
        data.WriteBit(masterLooterGuid[3]);
        data.WriteBit(masterLooterGuid[1]);
    }
    data.WriteBit(creatureGuid[1]);
    data.WriteBit(looterGuid != (uint64)0);
    data.WriteBit(creatureGuid[4]);
    data.WriteBit(creatureGuid[3]);
    data.WriteBit(creatureGuid[2]);
    if (looterGuid)
    {
        data.WriteBit(looterGuid[2]);
        data.WriteBit(looterGuid[3]);
        data.WriteBit(looterGuid[4]);
        data.WriteBit(looterGuid[5]);
        data.WriteBit(looterGuid[6]);
        data.WriteBit(looterGuid[0]);
        data.WriteBit(looterGuid[1]);
        data.WriteBit(looterGuid[7]);
    }
    data.WriteBit(creatureGuid[7]);
    data.WriteBit(creatureGuid[0]);
    data.WriteBit(creatureGuid[6]);
    data.FlushBits();

    if (looterGuid)
    {
        data.WriteByteSeq(looterGuid[7]);
        data.WriteByteSeq(looterGuid[1]);
        data.WriteByteSeq(looterGuid[0]);
        data.WriteByteSeq(looterGuid[6]);
        data.WriteByteSeq(looterGuid[5]);
        data.WriteByteSeq(looterGuid[3]);
        data.WriteByteSeq(looterGuid[4]);
        data.WriteByteSeq(looterGuid[2]);
    }

    if (masterLooterGuid)
    {
        data.WriteByteSeq(masterLooterGuid[4]);
        data.WriteByteSeq(masterLooterGuid[5]);
        data.WriteByteSeq(masterLooterGuid[6]);
        data.WriteByteSeq(masterLooterGuid[3]);
        data.WriteByteSeq(masterLooterGuid[2]);
        data.WriteByteSeq(masterLooterGuid[7]);
        data.WriteByteSeq(masterLooterGuid[0]);
        data.WriteByteSeq(masterLooterGuid[1]);
    }

    data.WriteByteSeq(creatureGuid[5]);
    data.WriteByteSeq(creatureGuid[1]);
    data.WriteByteSeq(creatureGuid[6]);
    data.WriteByteSeq(creatureGuid[2]);
    data.WriteByteSeq(creatureGuid[3]);
    data.WriteByteSeq(creatureGuid[0]);
    data.WriteByteSeq(creatureGuid[7]);
    data.WriteByteSeq(creatureGuid[4]);
    
    BroadcastPacket(&data, false);
}

void Group::GroupLoot(Loot* loot, WorldObject* pLootedObject)
{
    std::vector<LootItem>::iterator i;
    ItemTemplate const* item;
    uint8 itemSlot = 0;

    for (i = loot->items.begin(); i != loot->items.end(); ++i, ++itemSlot)
    {
        if (i->freeforall)
            continue;

        item = sObjectMgr->GetItemTemplate(i->itemid);
        if (!item)
        {
            //SF_LOG_DEBUG("misc", "Group::GroupLoot: missing item prototype for item with id: %d", i->itemid);
            continue;
        }

        //roll for over-threshold item if it's one-player loot
        if (item->Quality >= uint32(m_lootThreshold))
        {
            uint64 newitemGUID = MAKE_NEW_GUID(sObjectMgr->GenerateLowGuid(HIGHGUID_ITEM), 0, HIGHGUID_ITEM);
            Roll* r = new Roll(newitemGUID, *i);

            //a vector is filled with only near party members
            for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* member = itr->GetSource();
                if (!member || !member->GetSession())
                    continue;
                if (i->AllowedForPlayer(member))
                {
                    if (member->IsWithinDistInMap(pLootedObject, sWorld->GetFloatConfig(WorldFloatConfigs::CONFIG_GROUP_XP_DISTANCE), false))
                    {
                        r->totalPlayersRolling++;

                        if (member->GetPassOnGroupLoot())
                        {
                            r->playerVote[member->GetGUID()] = RollType::ROLL_PASS;
                            r->totalPass++;
                            // can't broadcast the pass now. need to wait until all rolling players are known.
                        }
                        else
                            r->playerVote[member->GetGUID()] = RollType::MAX_ROLL_TYPE;
                    }
                }
            }

            if (r->totalPlayersRolling > 0)
            {
                r->setLoot(loot);
                r->itemSlot = itemSlot;
                if (item->DisenchantID && m_maxEnchantingLevel >= item->RequiredDisenchantSkill)
                    r->rollVoteMask |= uint8(RollMask::ROLL_FLAG_TYPE_DISENCHANT);

                loot->items[itemSlot].is_blocked = true;

                // If there is any "auto pass", broadcast the pass now.
                if (r->totalPass)
                {
                    for (Roll::PlayerVote::const_iterator itr=r->playerVote.begin(); itr != r->playerVote.end(); ++itr)
                    {
                        Player* p = ObjectAccessor::FindPlayer(itr->first);
                        if (!p || !p->GetSession())
                            continue;

                        if (itr->second == RollType::ROLL_PASS)
                            SendLootRoll(newitemGUID, p->GetGUID(), 128, RollType::ROLL_PASS, *r);
                    }
                }

                SendLootStartRoll(60000, pLootedObject->GetMapId(), *r);

                RollId.push_back(r);

                if (Creature* creature = pLootedObject->ToCreature())
                {
                    creature->m_groupLootTimer = 60000;
                    creature->lootingGroupLowGUID = GetLowGUID();
                }
                else if (GameObject* go = pLootedObject->ToGameObject())
                {
                    go->m_groupLootTimer = 60000;
                    go->lootingGroupLowGUID = GetLowGUID();
                }
            }
            else
                delete r;
        }
        else
            i->is_underthreshold = true;
    }

    for (i = loot->quest_items.begin(); i != loot->quest_items.end(); ++i, ++itemSlot)
    {
        if (!i->follow_loot_rules)
            continue;

        item = sObjectMgr->GetItemTemplate(i->itemid);
        if (!item)
        {
            //SF_LOG_DEBUG("misc", "Group::GroupLoot: missing item prototype for item with id: %d", i->itemid);
            continue;
        }

        uint64 newitemGUID = MAKE_NEW_GUID(sObjectMgr->GenerateLowGuid(HIGHGUID_ITEM), 0, HIGHGUID_ITEM);
        Roll* r = new Roll(newitemGUID, *i);

        //a vector is filled with only near party members
        for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
        {
            Player* member = itr->GetSource();
            if (!member || !member->GetSession())
                continue;

            if (i->AllowedForPlayer(member))
            {
                if (member->IsWithinDistInMap(pLootedObject, sWorld->GetFloatConfig(WorldFloatConfigs::CONFIG_GROUP_XP_DISTANCE), false))
                {
                    r->totalPlayersRolling++;
                    r->playerVote[member->GetGUID()] = RollType::MAX_ROLL_TYPE;
                }
            }
        }

        if (r->totalPlayersRolling > 0)
        {
            r->setLoot(loot);
            r->itemSlot = itemSlot;

            loot->quest_items[itemSlot - loot->items.size()].is_blocked = true;

            SendLootStartRoll(60000, pLootedObject->GetMapId(), *r);

            RollId.push_back(r);

            if (Creature* creature = pLootedObject->ToCreature())
            {
                creature->m_groupLootTimer = 60000;
                creature->lootingGroupLowGUID = GetLowGUID();
            }
            else if (GameObject* go = pLootedObject->ToGameObject())
            {
                go->m_groupLootTimer = 60000;
                go->lootingGroupLowGUID = GetLowGUID();
            }
        }
        else
            delete r;
    }
}

void Group::NeedBeforeGreed(Loot* loot, WorldObject* lootedObject)
{
    ItemTemplate const* item;
    uint8 itemSlot = 0;
    for (std::vector<LootItem>::iterator i = loot->items.begin(); i != loot->items.end(); ++i, ++itemSlot)
    {
        if (i->freeforall)
            continue;

        item = sObjectMgr->GetItemTemplate(i->itemid);

        //roll for over-threshold item if it's one-player loot
        if (item->Quality >= uint32(m_lootThreshold))
        {
            uint64 newitemGUID = MAKE_NEW_GUID(sObjectMgr->GenerateLowGuid(HIGHGUID_ITEM), 0, HIGHGUID_ITEM);
            Roll* r = new Roll(newitemGUID, *i);

            for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* playerToRoll = itr->GetSource();
                if (!playerToRoll || !playerToRoll->GetSession())
                    continue;

                bool allowedForPlayer = i->AllowedForPlayer(playerToRoll);
                if (allowedForPlayer && playerToRoll->IsWithinDistInMap(lootedObject, sWorld->GetFloatConfig(WorldFloatConfigs::CONFIG_GROUP_XP_DISTANCE), false))
                {
                    r->totalPlayersRolling++;
                    if (playerToRoll->GetPassOnGroupLoot())
                    {
                        r->playerVote[playerToRoll->GetGUID()] = RollType::ROLL_PASS;
                        r->totalPass++;
                        // can't broadcast the pass now. need to wait until all rolling players are known.
                    }
                    else
                        r->playerVote[playerToRoll->GetGUID()] = RollType::MAX_ROLL_TYPE;
                }
            }

            if (r->totalPlayersRolling > 0)
            {
                r->setLoot(loot);
                r->itemSlot = itemSlot;
                if (item->DisenchantID && m_maxEnchantingLevel >= item->RequiredDisenchantSkill)
                    r->rollVoteMask |= uint8(RollMask::ROLL_FLAG_TYPE_DISENCHANT);

                if (item->Flags2 & ITEM_FLAGS_EXTRA_NEED_ROLL_DISABLED)
                    r->rollVoteMask &= ~uint8(RollMask::ROLL_FLAG_TYPE_NEED);

                loot->items[itemSlot].is_blocked = true;

                //Broadcast Pass and Send Rollstart
                for (Roll::PlayerVote::const_iterator itr = r->playerVote.begin(); itr != r->playerVote.end(); ++itr)
                {
                    Player* p = ObjectAccessor::FindPlayer(itr->first);
                    if (!p || !p->GetSession())
                        continue;

                    if (itr->second == RollType::ROLL_PASS)
                        SendLootRoll(newitemGUID, p->GetGUID(), 128, RollType::ROLL_PASS, *r);
                    else
                        SendLootStartRollToPlayer(60000, lootedObject->GetMapId(), p, p->CanRollForItemInLFG(item, lootedObject) == EQUIP_ERR_OK, *r);
                }

                RollId.push_back(r);

                if (Creature* creature = lootedObject->ToCreature())
                {
                    creature->m_groupLootTimer = 60000;
                    creature->lootingGroupLowGUID = GetLowGUID();
                }
                else if (GameObject* go = lootedObject->ToGameObject())
                {
                    go->m_groupLootTimer = 60000;
                    go->lootingGroupLowGUID = GetLowGUID();
                }
            }
            else
                delete r;
        }
        else
            i->is_underthreshold = true;
    }

    for (std::vector<LootItem>::iterator i = loot->quest_items.begin(); i != loot->quest_items.end(); ++i, ++itemSlot)
    {
        if (!i->follow_loot_rules)
            continue;

        item = sObjectMgr->GetItemTemplate(i->itemid);
        uint64 newitemGUID = MAKE_NEW_GUID(sObjectMgr->GenerateLowGuid(HIGHGUID_ITEM), 0, HIGHGUID_ITEM);
        Roll* r = new Roll(newitemGUID, *i);

        for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
        {
            Player* playerToRoll = itr->GetSource();
            if (!playerToRoll || !playerToRoll->GetSession())
                continue;

            bool allowedForPlayer = i->AllowedForPlayer(playerToRoll);
            if (allowedForPlayer && playerToRoll->IsWithinDistInMap(lootedObject, sWorld->GetFloatConfig(WorldFloatConfigs::CONFIG_GROUP_XP_DISTANCE), false))
            {
                r->totalPlayersRolling++;
                r->playerVote[playerToRoll->GetGUID()] = RollType::MAX_ROLL_TYPE;
            }
        }

        if (r->totalPlayersRolling > 0)
        {
            r->setLoot(loot);
            r->itemSlot = itemSlot;

            loot->quest_items[itemSlot - loot->items.size()].is_blocked = true;

            //Broadcast Pass and Send Rollstart
            for (Roll::PlayerVote::const_iterator itr = r->playerVote.begin(); itr != r->playerVote.end(); ++itr)
            {
                Player* p = ObjectAccessor::FindPlayer(itr->first);
                if (!p || !p->GetSession())
                    continue;

                if (itr->second == RollType::ROLL_PASS)
                    SendLootRoll(newitemGUID, p->GetGUID(), 128, RollType::ROLL_PASS, *r);
                else
                    SendLootStartRollToPlayer(60000, lootedObject->GetMapId(), p, p->CanRollForItemInLFG(item, lootedObject) == EQUIP_ERR_OK, *r);
            }

            RollId.push_back(r);

            if (Creature* creature = lootedObject->ToCreature())
            {
                creature->m_groupLootTimer = 60000;
                creature->lootingGroupLowGUID = GetLowGUID();
            }
            else if (GameObject* go = lootedObject->ToGameObject())
            {
                go->m_groupLootTimer = 60000;
                go->lootingGroupLowGUID = GetLowGUID();
            }
        }
        else
            delete r;
    }
}

void Group::MasterLoot(Loot* /*loot*/, WorldObject* pLootedObject)
{
    SF_LOG_DEBUG("network", "Group::MasterLoot (SMSG_LOOT_MASTER_LIST)");

    uint32 realCount = 0;
    ObjectGuid lootedGuid = pLootedObject->GetGUID();
    WorldPacket data(SMSG_LOOT_MASTER_LIST);

    data.WriteBit(lootedGuid[1]);
    data.WriteBits(0, 24);

    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* looter = itr->GetSource();
        if (!looter->IsInWorld())
            continue;

        if (looter->IsWithinDistInMap(pLootedObject, sWorld->GetFloatConfig(WorldFloatConfigs::CONFIG_GROUP_XP_DISTANCE), false))
        {
            ObjectGuid looterGuid = looter->GetGUID();
            data.WriteBit(looterGuid[4]);
            data.WriteBit(looterGuid[1]);
            data.WriteBit(looterGuid[2]);
            data.WriteBit(looterGuid[5]);
            data.WriteBit(looterGuid[3]);
            data.WriteBit(looterGuid[7]);
            data.WriteBit(looterGuid[0]);
            data.WriteBit(looterGuid[6]);
            ++realCount;
        }
    }

    data.WriteBit(lootedGuid[2]);
    data.WriteBit(lootedGuid[0]);
    data.WriteBit(lootedGuid[6]);
    data.WriteBit(lootedGuid[7]);
    data.WriteBit(lootedGuid[3]);
    data.WriteBit(lootedGuid[4]);

    data.WriteByteSeq(lootedGuid[3]);
    data.WriteByteSeq(lootedGuid[0]);
    data.WriteByteSeq(lootedGuid[2]);

    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* looter = itr->GetSource();
        if (!looter->IsInWorld())
            continue;

        if (looter->IsWithinDistInMap(pLootedObject, sWorld->GetFloatConfig(WorldFloatConfigs::CONFIG_GROUP_XP_DISTANCE), false))
        {
            ObjectGuid looterGuid = looter->GetGUID();
            data.WriteByteSeq(looterGuid[1]);
            data.WriteByteSeq(looterGuid[3]);
            data.WriteByteSeq(looterGuid[7]);
            data.WriteByteSeq(looterGuid[2]);
            data.WriteByteSeq(looterGuid[0]);
            data.WriteByteSeq(looterGuid[6]);
            data.WriteByteSeq(looterGuid[4]);
            data.WriteByteSeq(looterGuid[5]);
        }
    }

    data.WriteByteSeq(lootedGuid[7]);
    data.WriteByteSeq(lootedGuid[1]);
    data.WriteByteSeq(lootedGuid[6]);
    data.WriteByteSeq(lootedGuid[4]);
    data.WriteByteSeq(lootedGuid[5]);

    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* looter = itr->GetSource();
        if (looter->IsWithinDistInMap(pLootedObject, sWorld->GetFloatConfig(WorldFloatConfigs::CONFIG_GROUP_XP_DISTANCE), false))
            looter->GetSession()->SendPacket(&data);
    }
}

void Group::CountRollVote(uint64 playerGUID, uint64 Guid, RollType Choice)
{
    Rolls::iterator rollI = GetRoll(Guid);
    if (rollI == RollId.end())
        return;
    Roll* roll = *rollI;

    Roll::PlayerVote::iterator itr = roll->playerVote.find(playerGUID);
    // this condition means that player joins to the party after roll begins
    if (itr == roll->playerVote.end())
        return;

    if (roll->getLoot())
        if (roll->getLoot()->items.empty())
            return;

    switch (Choice)
    {
        case RollType::ROLL_PASS:                                     // Player choose pass
            SendLootRoll(0, playerGUID, 128, RollType::ROLL_PASS, *roll);
            ++roll->totalPass;
            itr->second = RollType::ROLL_PASS;
            break;
        case RollType::ROLL_NEED:                                     // player choose Need
            SendLootRoll(0, playerGUID, 0, RollType::ROLL_NEED, *roll);
            ++roll->totalNeed;
            itr->second = RollType::ROLL_NEED;
            break;
        case RollType::ROLL_GREED:                                    // player choose Greed
            SendLootRoll(0, playerGUID, 128, RollType::ROLL_GREED, *roll);
            ++roll->totalGreed;
            itr->second = RollType::ROLL_GREED;
            break;
        case RollType::ROLL_DISENCHANT:                               // player choose Disenchant
            SendLootRoll(0, playerGUID, 128, RollType::ROLL_DISENCHANT, *roll);
            ++roll->totalGreed;
            itr->second = RollType::ROLL_DISENCHANT;
            break;
    }

    if (roll->totalPass + roll->totalNeed + roll->totalGreed >= roll->totalPlayersRolling)
        CountTheRoll(rollI);
}

//called when roll timer expires
void Group::EndRoll(Loot* pLoot)
{
    for (Rolls::iterator itr = RollId.begin(); itr != RollId.end();)
    {
        if ((*itr)->getLoot() == pLoot)
        {
            CountTheRoll(itr);           //i don't have to edit player votes, who didn't vote ... he will pass
            itr = RollId.begin();
        }
        else
            ++itr;
    }
}

void Group::CountTheRoll(Rolls::iterator rollI)
{
    Roll* roll = *rollI;
    if (!roll->isValid())                                   // is loot already deleted ?
    {
        RollId.erase(rollI);
        delete roll;
        return;
    }

    //end of the roll
    if (roll->totalNeed > 0)
    {
        if (!roll->playerVote.empty())
        {
            uint8 maxresul = 0;
            uint64 maxguid  = (*roll->playerVote.begin()).first;
            Player* player;

            for (Roll::PlayerVote::const_iterator itr=roll->playerVote.begin(); itr != roll->playerVote.end(); ++itr)
            {
                if (itr->second != RollType::ROLL_NEED)
                    continue;

                uint8 randomN = urand(1, 100);
                SendLootRoll(0, itr->first, randomN, RollType::ROLL_NEED, *roll);
                if (maxresul < randomN)
                {
                    maxguid  = itr->first;
                    maxresul = randomN;
                }
            }
            SendLootRollWon(0, maxguid, maxresul, RollType::ROLL_NEED, *roll);
            player = ObjectAccessor::FindPlayer(maxguid);

            if (player && player->GetSession())
            {
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT, roll->itemid, maxresul);

                ItemPosCountVec dest;
                LootItem* item = &(roll->itemSlot >= roll->getLoot()->items.size() ? roll->getLoot()->quest_items[roll->itemSlot - roll->getLoot()->items.size()] : roll->getLoot()->items[roll->itemSlot]);
                InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, roll->itemid, item->count);
                if (msg == EQUIP_ERR_OK)
                {
                    item->is_looted = true;
                    roll->getLoot()->NotifyItemRemoved(roll->itemSlot);
                    roll->getLoot()->unlootedCount--;
                    player->StoreNewItem(dest, roll->itemid, true, item->randomPropertyId, item->GetAllowedLooters());
                }
                else
                {
                    item->is_blocked = false;
                    player->SendEquipError(msg, NULL, NULL, roll->itemid);
                }
            }
        }
    }
    else if (roll->totalGreed > 0)
    {
        if (!roll->playerVote.empty())
        {
            uint8 maxresul = 0;
            uint64 maxguid = (*roll->playerVote.begin()).first;
            Player* player;
            RollType rollvote = RollType::ROLL_INVALID;

            Roll::PlayerVote::iterator itr;
            for (itr = roll->playerVote.begin(); itr != roll->playerVote.end(); ++itr)
            {
                if (itr->second != RollType::ROLL_GREED && itr->second != RollType::ROLL_DISENCHANT)
                    continue;

                uint8 randomN = urand(1, 100);
                SendLootRoll(0, itr->first, randomN, RollType(itr->second), *roll);
                if (maxresul < randomN)
                {
                    maxguid  = itr->first;
                    maxresul = randomN;
                    rollvote = RollType(itr->second);
                }
            }
            SendLootRollWon(0, maxguid, maxresul, rollvote, *roll);
            player = ObjectAccessor::FindPlayer(maxguid);

            if (player && player->GetSession())
            {
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED_ON_LOOT, roll->itemid, maxresul);

                LootItem* item = &(roll->itemSlot >= roll->getLoot()->items.size() ? roll->getLoot()->quest_items[roll->itemSlot - roll->getLoot()->items.size()] : roll->getLoot()->items[roll->itemSlot]);

                if (rollvote == RollType::ROLL_GREED)
                {
                    ItemPosCountVec dest;
                    InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, roll->itemid, item->count);
                    if (msg == EQUIP_ERR_OK)
                    {
                        item->is_looted = true;
                        roll->getLoot()->NotifyItemRemoved(roll->itemSlot);
                        roll->getLoot()->unlootedCount--;
                        player->StoreNewItem(dest, roll->itemid, true, item->randomPropertyId, item->GetAllowedLooters());
                    }
                    else
                    {
                        item->is_blocked = false;
                        player->SendEquipError(msg, NULL, NULL, roll->itemid);
                    }
                }
                else if (rollvote == RollType::ROLL_DISENCHANT)
                {
                    item->is_looted = true;
                    roll->getLoot()->NotifyItemRemoved(roll->itemSlot);
                    roll->getLoot()->unlootedCount--;
                    ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(roll->itemid);
                    player->AutoStoreLoot(pProto->DisenchantID, LootTemplates_Disenchant, true);
                    player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL, 13262); // Disenchant
                }
            }
        }
    }
    else
    {
        SendLootAllPassed(*roll);

        // remove is_blocked so that the item is lootable by all players
        LootItem* item = &(roll->itemSlot >= roll->getLoot()->items.size() ? roll->getLoot()->quest_items[roll->itemSlot - roll->getLoot()->items.size()] : roll->getLoot()->items[roll->itemSlot]);
        if (item)
            item->is_blocked = false;
    }

    RollId.erase(rollI);
    delete roll;
}

void Group::SetTargetIcon(uint8 id, ObjectGuid whoGuid, ObjectGuid targetGuid, uint8 Index)
{
    if (id >= TARGETICONCOUNT)
        return;

    // clean other icons
    if (targetGuid != 0)
        for (int i=0; i<TARGETICONCOUNT; ++i)
            if (m_targetIcons[i] == targetGuid)
                SetTargetIcon(i, 0, 0, 0);

    m_targetIcons[id] = targetGuid;

    WorldPacket data(SMSG_RAID_TARGET_UPDATE_SINGLE, 8 + 1 + 8 + 1);

    data.WriteBit(whoGuid[6]);
    data.WriteBit(targetGuid[4]);
    data.WriteBit(whoGuid[0]);
    data.WriteBit(whoGuid[7]);
    data.WriteBit(targetGuid[6]);
    data.WriteBit(whoGuid[5]);
    data.WriteBit(whoGuid[3]);
    data.WriteBit(whoGuid[4]);
    data.WriteBit(targetGuid[7]);
    data.WriteBit(targetGuid[2]);
    data.WriteBit(targetGuid[5]);
    data.WriteBit(targetGuid[1]);
    data.WriteBit(whoGuid[2]);
    data.WriteBit(whoGuid[1]);
    data.WriteBit(targetGuid[0]);
    data.WriteBit(targetGuid[3]);

    data.WriteByteSeq(targetGuid[1]);

    data << uint8(id);

    data.WriteByteSeq(whoGuid[0]);
    data.WriteByteSeq(whoGuid[5]);
    data.WriteByteSeq(whoGuid[3]);
    data.WriteByteSeq(targetGuid[7]);
    data.WriteByteSeq(targetGuid[6]);
    data.WriteByteSeq(whoGuid[1]);
    data.WriteByteSeq(targetGuid[2]);
    data.WriteByteSeq(targetGuid[4]);
    data.WriteByteSeq(targetGuid[0]);
    data.WriteByteSeq(targetGuid[3]);
    data.WriteByteSeq(targetGuid[5]);
    data.WriteByteSeq(whoGuid[6]);

    data << uint8(Index);

    data.WriteByteSeq(whoGuid[4]);
    data.WriteByteSeq(whoGuid[2]);
    data.WriteByteSeq(whoGuid[7]);

    BroadcastPacket(&data, true);
}

void Group::SendTargetIconList(WorldSession* session)
{
    if (!session)
        return;

    uint8 Index = 0;
    WorldPacket data(SMSG_RAID_TARGET_UPDATE_ALL, (1 + TARGETICONCOUNT * 9));
    data.WriteBits(0, 25);

    for (uint8 i = 0; i < TARGETICONCOUNT; ++i)
    {
        if (m_targetIcons[i] == 0)
            continue;

        ObjectGuid guid = m_targetIcons[i];

        data.WriteBit(guid[2]);
        data.WriteBit(guid[1]);
        data.WriteBit(guid[3]);
        data.WriteBit(guid[7]);
        data.WriteBit(guid[6]);
        data.WriteBit(guid[4]);
        data.WriteBit(guid[0]);
        data.WriteBit(guid[5]);

        data.WriteByteSeq(guid[4]);
        data.WriteByteSeq(guid[7]);
        data.WriteByteSeq(guid[1]);
        data.WriteByteSeq(guid[0]);
        data.WriteByteSeq(guid[6]);
        data.WriteByteSeq(guid[5]);
        data.WriteByteSeq(guid[3]);
        data << uint8(i);
        data.WriteByteSeq(guid[2]);
    }

    data << uint8(Index);

    session->SendPacket(&data);
}

void Group::SendUpdate()
{
    for (member_witerator witr = m_memberSlots.begin(); witr != m_memberSlots.end(); ++witr)
        SendUpdateToPlayer(witr->guid, &(*witr));
}

void Group::SendUpdateToPlayer(uint64 playerGUID, MemberSlot* slot)
{
    Player* player = ObjectAccessor::FindPlayer(playerGUID);

    if (!player || !player->GetSession())
        return;

    // if MemberSlot wasn't provided
    if (!slot)
    {
        ObjectGuid groupGuid = m_guid;
        ObjectGuid leaderGuid = uint64(0);

        WorldPacket data(SMSG_GROUP_LIST, 4 + 1 + 1 + 1 + 4 + 1 + 8 + 1 + 8 + 3);
        data.WriteBit(groupGuid[0]);
        data.WriteBit(leaderGuid[7]);
        data.WriteBit(leaderGuid[1]);
        data.WriteBit(0);
        data.WriteBit(groupGuid[7]);
        data.WriteBit(leaderGuid[6]);
        data.WriteBit(leaderGuid[5]);
        data.WriteBits(0, 21);
        data.WriteBit(leaderGuid[3]);
        data.WriteBit(leaderGuid[0]);
        data.WriteBit(0);
        data.WriteBit(groupGuid[5]);
        data.WriteBit(groupGuid[2]);
        data.WriteBit(groupGuid[4]);
        data.WriteBit(groupGuid[1]);
        data.WriteBit(0);
        data.WriteBit(leaderGuid[2]);
        data.WriteBit(groupGuid[6]);
        data.WriteBit(leaderGuid[4]);
        data.WriteBit(groupGuid[3]);
        data.FlushBits();

        data.WriteByteSeq(leaderGuid[0]);
        data.WriteByteSeq(groupGuid[1]);
        data.WriteByteSeq(leaderGuid[4]);
        data.WriteByteSeq(leaderGuid[2]);
        data.WriteByteSeq(groupGuid[6]);
        data.WriteByteSeq(groupGuid[4]);
        data << uint8(0x10);
        data << uint8(0);
        data << int32(-1);
        data.WriteByteSeq(groupGuid[7]);
        data.WriteByteSeq(leaderGuid[3]);
        data.WriteByteSeq(leaderGuid[1]);
        data << uint32(m_counter++);
        data.WriteByteSeq(groupGuid[0]);
        data.WriteByteSeq(groupGuid[2]);
        data.WriteByteSeq(groupGuid[5]);
        data.WriteByteSeq(groupGuid[3]);
        data.WriteByteSeq(leaderGuid[7]);
        data << uint8(0);
        data.WriteByteSeq(leaderGuid[5]);
        data.WriteByteSeq(leaderGuid[6]);

        player->GetSession()->SendPacket(&data);
        return;
    }

    if (player->GetGroup() != this)
        return;

    uint8 groupPosition = 0;
    uint8 i = 0;

    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
    {
        if (citr->group != slot->group)
            continue;

        if (citr->guid == slot->guid)
        {
            groupPosition = i;
            break;
        }

        i++;
    }

    ByteBuffer memberData;
    ObjectGuid groupGuid = m_guid;
    ObjectGuid leaderGuid = m_leaderGuid;
    ObjectGuid looterGuid = m_looterGuid;

    WorldPacket data(SMSG_GROUP_LIST, 4 + 3 + 4 + 1 + 8 + 1 + 8 + 3 + (GetMembersCount() * (4 + 2 + 8)) + 1 + 8 + 2 + 4 + 4);
    data.WriteBit(groupGuid[0]);
    data.WriteBit(leaderGuid[7]);
    data.WriteBit(leaderGuid[1]);
    data.WriteBit(1);                                   // has dungeon and raid difficulty
    data.WriteBit(groupGuid[7]);
    data.WriteBit(leaderGuid[6]);
    data.WriteBit(leaderGuid[5]);
    data.WriteBits(GetMembersCount(), 21);

    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
    {
        ObjectGuid memberGuid = citr->guid;
        std::string memberName = citr->name;

        Player* member = ObjectAccessor::FindPlayer(memberGuid);

        uint8 onlineState = member ? MEMBER_STATUS_ONLINE : MEMBER_STATUS_OFFLINE;
        onlineState = onlineState | ((isBGGroup() || isBFGroup()) ? MEMBER_STATUS_PVP : 0);

        data.WriteBit(memberGuid[1]);
        data.WriteBit(memberGuid[2]);
        data.WriteBit(memberGuid[5]);
        data.WriteBit(memberGuid[6]);
        data.WriteBits(memberName.size(), 6);
        data.WriteBit(memberGuid[7]);
        data.WriteBit(memberGuid[3]);
        data.WriteBit(memberGuid[0]);
        data.WriteBit(memberGuid[4]);

        memberData.WriteByteSeq(memberGuid[6]);
        memberData.WriteByteSeq(memberGuid[3]);
        memberData << uint8(citr->roles); // 67
        memberData << uint8(onlineState); // 64
        memberData.WriteByteSeq(memberGuid[7]);
        memberData.WriteByteSeq(memberGuid[4]);
        memberData.WriteByteSeq(memberGuid[1]);
        memberData.WriteString(memberName);
        memberData.WriteByteSeq(memberGuid[5]);
        memberData.WriteByteSeq(memberGuid[2]);
        memberData << uint8(citr->group); // 65
        memberData.WriteByteSeq(memberGuid[0]);
        memberData << uint8(citr->flags); // 66
    }

    data.WriteBit(leaderGuid[3]);
    data.WriteBit(leaderGuid[0]);
    data.WriteBit(1);                                   // has loot mode
    data.WriteBit(groupGuid[5]);

    //if (hasLootMode)
    {
        data.WriteBit(looterGuid[6]);
        data.WriteBit(looterGuid[4]);
        data.WriteBit(looterGuid[5]);
        data.WriteBit(looterGuid[2]);
        data.WriteBit(looterGuid[1]);
        data.WriteBit(looterGuid[0]);
        data.WriteBit(looterGuid[7]);
        data.WriteBit(looterGuid[3]);
    }

    data.WriteBit(groupGuid[2]);
    data.WriteBit(groupGuid[4]);
    data.WriteBit(groupGuid[1]);
    data.WriteBit(0);                                   // is LFG
    data.WriteBit(leaderGuid[2]);
    data.WriteBit(groupGuid[6]);

    /*if (isLFGGroup())
    {
    }*/

    data.WriteBit(leaderGuid[4]);
    data.WriteBit(groupGuid[3]);
    data.FlushBits();

    data.WriteByteSeq(leaderGuid[0]);

    //if (hasInstanceDifficulty)
    {
        data << uint32(m_raidDifficulty);               // raid Difficulty
        data << uint32(m_dungeonDifficulty);            // dungeon Difficulty
    }

    data.append(memberData);
    data.WriteByteSeq(groupGuid[1]);

    /*if (isLFGGroup())
    {
    }*/

    data.WriteByteSeq(leaderGuid[4]);
    data.WriteByteSeq(leaderGuid[2]);

    //if (hasLootMode)
    {
        data << uint8(m_lootMethod);
        data.WriteByteSeq(looterGuid[0]);
        data.WriteByteSeq(looterGuid[5]);
        data.WriteByteSeq(looterGuid[4]);
        data.WriteByteSeq(looterGuid[3]);
        data.WriteByteSeq(looterGuid[2]);
        data << uint8(m_lootThreshold);
        data.WriteByteSeq(looterGuid[7]);
        data.WriteByteSeq(looterGuid[1]);
        data.WriteByteSeq(looterGuid[6]);
    }

    data.WriteByteSeq(groupGuid[6]);
    data.WriteByteSeq(groupGuid[4]);
    data << uint8(m_groupType);
    data << uint8(0);
    data << uint32(groupPosition);
    data.WriteByteSeq(groupGuid[7]);
    data.WriteByteSeq(leaderGuid[3]);
    data.WriteByteSeq(leaderGuid[1]);
    data << uint32(m_counter++);
    data.WriteByteSeq(groupGuid[0]);
    data.WriteByteSeq(groupGuid[2]);
    data.WriteByteSeq(groupGuid[5]);
    data.WriteByteSeq(groupGuid[3]);
    data.WriteByteSeq(leaderGuid[7]);
    data << uint8(0);
    data.WriteByteSeq(leaderGuid[5]);
    data.WriteByteSeq(leaderGuid[6]);

    player->GetSession()->SendPacket(&data);

    /*data << uint8(m_groupType);                         // group type (flags in 3.3)
    data << uint8(slot->group);
    data << uint8(slot->flags);
    data << uint8(slot->roles);
    if (isLFGGroup())
    {
        data << uint8(sLFGMgr->GetState(m_guid) == lfg::LFG_STATE_FINISHED_DUNGEON ? 2 : 0); // FIXME - Dungeon save status? 2 = done
        data << uint32(sLFGMgr->GetDungeon(m_guid));
        data << uint8(0); // 4.x new
    }

    data << uint64(m_guid);
    data << uint32(m_counter++);                        // 3.3, value increases every time this packet gets sent
    data << uint32(GetMembersCount()-1);
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
    {
        if (slot->guid == citr->guid)
            continue;

        Player* member = ObjectAccessor::FindPlayer(citr->guid);

        uint8 onlineState = member ? MEMBER_STATUS_ONLINE : MEMBER_STATUS_OFFLINE;
        onlineState = onlineState | ((isBGGroup() || isBFGroup()) ? MEMBER_STATUS_PVP : 0);

        data << citr->name;
        data << uint64(citr->guid);                     // guid
        data << uint8(onlineState);                     // online-state
        data << uint8(citr->group);                     // groupid
        data << uint8(citr->flags);                     // See enum GroupMemberFlags
        data << uint8(citr->roles);                     // Lfg Roles
    }

    data << uint64(m_leaderGuid);                       // leader guid

    if (GetMembersCount() - 1)
    {
        data << uint8(m_lootMethod);                    // loot method
        data << uint64(m_looterGuid);                   // looter guid
        data << uint8(m_lootThreshold);                 // loot threshold
        data << uint8(m_dungeonDifficulty);             // Dungeon Difficulty
        data << uint8(m_raidDifficulty);                // Raid Difficulty
    }*/
}

void Group::SendReadyCheckCompleted()
{
    ObjectGuid guid = m_guid;

    WorldPacket data(SMSG_RAID_READY_CHECK_COMPLETED, 1 + 8 + 1);
    data.WriteBit(guid[4]);
    data.WriteBit(guid[2]);
    data.WriteBit(guid[5]);
    data.WriteBit(guid[7]);
    data.WriteBit(guid[1]);
    data.WriteBit(guid[0]);
    data.WriteBit(guid[3]);
    data.WriteBit(guid[6]);

    data.WriteByteSeq(guid[6]);
    data.WriteByteSeq(guid[0]);
    data.WriteByteSeq(guid[3]);
    data.WriteByteSeq(guid[1]);
    data.WriteByteSeq(guid[5]);
    data << uint8(0);
    data.WriteByteSeq(guid[7]);
    data.WriteByteSeq(guid[2]);
    data.WriteByteSeq(guid[4]);

    BroadcastPacket(&data, false);
}

void Group::UpdatePlayerOutOfRange(Player* player)
{
    if (!player || !player->IsInWorld())
        return;

    WorldPacket data;
    player->GetSession()->BuildPartyMemberStatsChangedPacket(player, &data);

    Player* member;
    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        member = itr->GetSource();
        if (member && !member->IsWithinDist(player, member->GetSightRange(), false))
            member->GetSession()->SendPacket(&data);
    }
}

void Group::BroadcastAddonMessagePacket(WorldPacket* packet, const std::string& prefix, bool ignorePlayersInBGRaid, int group, uint64 ignore)
{
    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* player = itr->GetSource();
        if (!player || (ignore != 0 && player->GetGUID() == ignore) || (ignorePlayersInBGRaid && player->GetGroup() != this))
            continue;

        if (WorldSession* session = player->GetSession())
            if (session && (group == -1 || itr->getSubGroup() == group))
                if (session->IsAddonRegistered(prefix))
                    session->SendPacket(packet);
    }
}

void Group::BroadcastPacket(WorldPacket* packet, bool ignorePlayersInBGRaid, int group, uint64 ignore)
{
    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* player = itr->GetSource();
        if (!player || (ignore != 0 && player->GetGUID() == ignore) || (ignorePlayersInBGRaid && player->GetGroup() != this))
            continue;

        if (player->GetSession() && (group == -1 || itr->getSubGroup() == group))
            player->GetSession()->SendPacket(packet);
    }
}

void Group::BroadcastReadyCheck(WorldPacket* packet)
{
    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* player = itr->GetSource();
        if (player && player->GetSession())
            if (IsLeader(player->GetGUID()) || IsAssistant(player->GetGUID()))
                player->GetSession()->SendPacket(packet);
    }
}

void Group::OfflineReadyCheck()
{
    ObjectGuid groupGuid = GetGUID();

    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
    {
        Player* player = ObjectAccessor::FindPlayer(citr->guid);
        if (!player || !player->GetSession())
        {
            ObjectGuid playerGuid = citr->guid;

            WorldPacket data(SMSG_RAID_READY_CHECK_CONFIRM, 1 + 1 + 8 + 1 + 8);
            data.WriteBit(groupGuid[4]);
            data.WriteBit(playerGuid[5]);
            data.WriteBit(playerGuid[3]);
            data.WriteBit(0);
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

            BroadcastPacket(&data, false);

            ReadyCheckMemberHasResponded(playerGuid);
        }
    }
}

bool Group::_setMembersGroup(uint64 guid, uint8 group)
{
    member_witerator slot = _getMemberWSlot(guid);
    if (slot == m_memberSlots.end())
        return false;

    slot->group = group;

    SubGroupCounterIncrease(group);

    if (!isBGGroup() && !isBFGroup())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_MEMBER_SUBGROUP);

        stmt->setUInt8(0, group);
        stmt->setUInt32(1, GUID_LOPART(guid));

        CharacterDatabase.Execute(stmt);
    }

    return true;
}

bool Group::SameSubGroup(Player const* member1, Player const* member2) const
{
    if (!member1 || !member2)
        return false;

    if (member1->GetGroup() != this || member2->GetGroup() != this)
        return false;
    else
        return member1->GetSubGroup() == member2->GetSubGroup();
}

// Allows setting sub groups both for online or offline members
void Group::ChangeMembersGroup(uint64 guid, uint8 group)
{
    // Only raid groups have sub groups
    if (!isRaidGroup())
        return;

    // Check if player is really in the raid
    member_witerator slot = _getMemberWSlot(guid);
    if (slot == m_memberSlots.end())
        return;

    // Abort if the player is already in the target sub group
    uint8 prevSubGroup = GetMemberGroup(guid);
    if (prevSubGroup == group)
        return;

    // Update the player slot with the new sub group setting
    slot->group = group;

    // Increase the counter of the new sub group..
    SubGroupCounterIncrease(group);

    // ..and decrease the counter of the previous one
    SubGroupCounterDecrease(prevSubGroup);

    // Preserve new sub group in database for non-raid groups
    if (!isBGGroup() && !isBFGroup())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_MEMBER_SUBGROUP);

        stmt->setUInt8(0, group);
        stmt->setUInt32(1, GUID_LOPART(guid));

        CharacterDatabase.Execute(stmt);
    }

    // In case the moved player is online, update the player object with the new sub group references
    if (Player* player = ObjectAccessor::FindPlayer(guid))
    {
        if (player->GetGroup() == this)
            player->GetGroupRef().setSubGroup(group);
        else
        {
            // If player is in BG raid, it is possible that he is also in normal raid - and that normal raid is stored in m_originalGroup reference
            prevSubGroup = player->GetOriginalSubGroup();
            player->GetOriginalGroupRef().setSubGroup(group);
        }
    }

    // Broadcast the changes to the group
    SendUpdate();
}

void Group::ChangeMembersGroup(Player* player, uint8 group)
{
    ChangeMembersGroup(player->GetGUID(), group);
}

// Retrieve the next Round-Roubin player for the group
//
// No update done if loot method is Master or FFA.
//
// If the RR player is not yet set for the group, the first group member becomes the round-robin player.
// If the RR player is set, the next player in group becomes the round-robin player.
//
// If ifneed is true,
//      the current RR player is checked to be near the looted object.
//      if yes, no update done.
//      if not, he loses his turn.
void Group::UpdateLooterGuid(WorldObject* pLootedObject, bool ifneed)
{
    switch (GetLootMethod())
    {
        case LootMethod::MASTER_LOOT:
        case LootMethod::FREE_FOR_ALL:
            return;
        default:
            // round robin style looting applies for all low
            // quality items in each loot method except free for all and master loot
            break;
    }

    uint64 oldLooterGUID = GetLooterGuid();
    member_citerator guid_itr = _getMemberCSlot(oldLooterGUID);
    if (guid_itr != m_memberSlots.end())
    {
        if (ifneed)
        {
            // not update if only update if need and ok
            Player* looter = ObjectAccessor::FindPlayer(guid_itr->guid);
            if (looter && looter->IsWithinDistInMap(pLootedObject, sWorld->GetFloatConfig(WorldFloatConfigs::CONFIG_GROUP_XP_DISTANCE), false))
                return;
        }
        ++guid_itr;
    }

    // search next after current
    Player* pNewLooter = NULL;
    for (member_citerator itr = guid_itr; itr != m_memberSlots.end(); ++itr)
    {
        if (Player* player = ObjectAccessor::FindPlayer(itr->guid))
            if (player->IsWithinDistInMap(pLootedObject, sWorld->GetFloatConfig(WorldFloatConfigs::CONFIG_GROUP_XP_DISTANCE), false))
            {
                pNewLooter = player;
                break;
            }
    }

    if (!pNewLooter)
    {
        // search from start
        for (member_citerator itr = m_memberSlots.begin(); itr != guid_itr; ++itr)
        {
            if (Player* player = ObjectAccessor::FindPlayer(itr->guid))
                if (player->IsWithinDistInMap(pLootedObject, sWorld->GetFloatConfig(WorldFloatConfigs::CONFIG_GROUP_XP_DISTANCE), false))
                {
                    pNewLooter = player;
                    break;
                }
        }
    }

    if (pNewLooter)
    {
        if (oldLooterGUID != pNewLooter->GetGUID())
        {
            SetLooterGuid(pNewLooter->GetGUID());
            SendUpdate();
        }
    }
    else
    {
        SetLooterGuid(0);
        SendUpdate();
    }
}

GroupJoinBattlegroundResult Group::CanJoinBattlegroundQueue(Battleground const* bgOrTemplate, BattlegroundQueueTypeId bgQueueTypeId, uint32 MinPlayerCount, uint32 /*MaxPlayerCount*/, bool isRated, uint32 arenaSlot)
{
    // check if this group is LFG group
    if (isLFGGroup())
        return GroupJoinBattlegroundResult::ERR_LFG_CANT_USE_BATTLEGROUND;

    BattlemasterListEntry const* bgEntry = sBattlemasterListStore.LookupEntry(uint32(bgOrTemplate->GetTypeID()));
    if (!bgEntry)
        return GroupJoinBattlegroundResult::ERR_BATTLEGROUND_JOIN_FAILED;            // shouldn't happen

    // check for min / max count
    uint32 memberscount = GetMembersCount();

    if (memberscount > bgEntry->maxGroupSize)                // no MinPlayerCount for battlegrounds
        return GroupJoinBattlegroundResult::ERR_BATTLEGROUND_NONE;                        // ERR_GROUP_JOIN_BATTLEGROUND_TOO_MANY handled on client side

    // get a player as reference, to compare other players' stats to (arena team id, queue id based on level, etc.)
    Player* reference = GetFirstMember()->GetSource();
    // no reference found, can't join this way
    if (!reference)
        return GroupJoinBattlegroundResult::ERR_BATTLEGROUND_JOIN_FAILED;

    PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bgOrTemplate->GetMapId(), reference->getLevel());
    if (!bracketEntry)
        return GroupJoinBattlegroundResult::ERR_BATTLEGROUND_JOIN_FAILED;

    uint32 team = reference->GetTeam();

    BattlegroundQueueTypeId bgQueueTypeIdRandom = BattlegroundMgr::BGQueueTypeId(BattlegroundTypeId::BATTLEGROUND_RB, 0);

    // check every member of the group to be able to join
    memberscount = 0;
    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next(), ++memberscount)
    {
        Player* member = itr->GetSource();
        // offline member? don't let join
        if (!member)
            return GroupJoinBattlegroundResult::ERR_BATTLEGROUND_JOIN_FAILED;
        // don't allow cross-faction join as group
        if (member->GetTeam() != team)
            return GroupJoinBattlegroundResult::ERR_BATTLEGROUND_JOIN_TIMED_OUT;
        // not in the same battleground level braket, don't let join
        PvPDifficultyEntry const* memberBracketEntry = GetBattlegroundBracketByLevel(bracketEntry->mapId, member->getLevel());
        if (memberBracketEntry != bracketEntry)
            return GroupJoinBattlegroundResult::ERR_BATTLEGROUND_JOIN_RANGE_INDEX;
        // don't let join if someone from the group is already in that bg queue
        if (member->InBattlegroundQueueForBattlegroundQueueType(bgQueueTypeId))
            return GroupJoinBattlegroundResult::ERR_BATTLEGROUND_JOIN_FAILED;            // not blizz-like
        // don't let join if someone from the group is in bg queue random
        if (member->InBattlegroundQueueForBattlegroundQueueType(bgQueueTypeIdRandom))
            return GroupJoinBattlegroundResult::ERR_IN_RANDOM_BG;
        // don't let join to bg queue random if someone from the group is already in bg queue
        if (bgOrTemplate->GetTypeID() == BattlegroundTypeId::BATTLEGROUND_RB && member->InBattlegroundQueue())
            return GroupJoinBattlegroundResult::ERR_IN_NON_RANDOM_BG;
        // check for deserter debuff in case not arena queue
        if (bgOrTemplate->GetTypeID() != BattlegroundTypeId::BATTLEGROUND_AA && !member->CanJoinToBattleground(bgOrTemplate))
            return GroupJoinBattlegroundResult::ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS;
        // check if member can join any more battleground queues
        if (!member->HasFreeBattlegroundQueueId())
            return GroupJoinBattlegroundResult::ERR_BATTLEGROUND_TOO_MANY_QUEUES;        // not blizz-like
        // check if someone in party is using dungeon system
        if (member->isUsingLfg())
            return GroupJoinBattlegroundResult::ERR_LFG_CANT_USE_BATTLEGROUND;
    }

    // only check for MinPlayerCount since MinPlayerCount == MaxPlayerCount for arenas...
    if (bgOrTemplate->isArena() && memberscount != MinPlayerCount)
        return GroupJoinBattlegroundResult::ERR_ARENA_TEAM_PARTY_SIZE;

    return GroupJoinBattlegroundResult::ERR_BATTLEGROUND_NONE;
}

//===================================================
//============== Roll ===============================
//===================================================

void Roll::targetObjectBuildLink()
{
    // called from link()
    getTarget()->addLootValidatorRef(this);
}

void Group::SetDungeonDifficulty(DifficultyID difficulty)
{
    m_dungeonDifficulty = difficulty;
    if (!isBGGroup() && !isBFGroup())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_DIFFICULTY);

        stmt->setUInt8(0, uint8(m_dungeonDifficulty));
        stmt->setUInt32(1, m_dbStoreId);

        CharacterDatabase.Execute(stmt);
    }

    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* player = itr->GetSource();
        if (!player->GetSession())
            continue;

        player->SetDungeonDifficulty(difficulty);
        player->SendDungeonDifficulty();
    }
}

void Group::SetRaidDifficulty(DifficultyID difficulty)
{
    m_raidDifficulty = difficulty;
    if (!isBGGroup() && !isBFGroup())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_RAID_DIFFICULTY);

        stmt->setUInt8(0, uint8(m_raidDifficulty));
        stmt->setUInt32(1, m_dbStoreId);

        CharacterDatabase.Execute(stmt);
    }

    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* player = itr->GetSource();
        if (!player->GetSession())
            continue;

        player->SetRaidDifficulty(difficulty);
        player->SendRaidDifficulty();
    }
}

bool Group::InCombatToInstance(uint32 instanceId)
{
    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* player = itr->GetSource();
        if (player && !player->getAttackers().empty() && player->GetInstanceId() == instanceId && (player->GetMap()->IsRaidOrHeroicDungeon()))
            for (std::set<Unit*>::const_iterator i = player->getAttackers().begin(); i != player->getAttackers().end(); ++i)
                if ((*i) && (*i)->GetTypeId() == TypeID::TYPEID_UNIT && (*i)->ToCreature()->GetCreatureTemplate()->flags_extra & CREATURE_FLAG_EXTRA_INSTANCE_BIND)
                    return true;
    }
    return false;
}

void Group::ResetInstances(InstanceResetMethod method, bool isRaid, Player* SendMsgTo)
{
    if (isBGGroup() || isBFGroup())
        return;

    // method can be INSTANCE_RESET_ALL, INSTANCE_RESET_CHANGE_DIFFICULTY, INSTANCE_RESET_GROUP_DISBAND

    // we assume that when the difficulty changes, all instances that can be reset will be
    DifficultyID diff = GetDungeonDifficulty();
    if (isRaid)
    {
        diff = GetRaidDifficulty();
    }

    for (BoundInstancesMap::iterator itr = m_boundInstances[diff].begin(); itr != m_boundInstances[diff].end();)
    {
        InstanceSave* instanceSave = itr->second.save;
        const MapEntry* entry = sMapStore.LookupEntry(itr->first);
        if (!entry || entry->IsRaid() != isRaid || (!instanceSave->CanReset() && method != InstanceResetMethod::INSTANCE_RESET_GROUP_DISBAND))
        {
            ++itr;
            continue;
        }

        if (method == InstanceResetMethod::INSTANCE_RESET_ALL)
        {
            // the "reset all instances" method can only reset normal maps
            if (entry->map_type == MAP_RAID || diff == DIFFICULTY_HEROIC)
            {
                ++itr;
                continue;
            }
        }

        bool isEmpty = true;
        // if the map is loaded, reset it
        Map* map = sMapMgr->FindMap(instanceSave->GetMapId(), instanceSave->GetInstanceId());
        if (map && map->IsInstance() && !(method == InstanceResetMethod::INSTANCE_RESET_GROUP_DISBAND && !instanceSave->CanReset()))
        {
            if (instanceSave->CanReset())
                isEmpty = ((InstanceMap*)map)->Reset(method);
            else
                isEmpty = !map->HavePlayers();
        }

        if (SendMsgTo)
        {
            if (!isEmpty)
                SendMsgTo->SendResetInstanceFailed(2, instanceSave->GetMapId());
            else if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_INSTANCES_RESET_ANNOUNCE))
            {
                if (Group* group = SendMsgTo->GetGroup())
                {
                    for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
                        if (Player* player = itr->GetSource())
                            player->SendResetInstanceSuccess(instanceSave->GetMapId());
                }

                else
                    SendMsgTo->SendResetInstanceSuccess(instanceSave->GetMapId());
            }
            else
                SendMsgTo->SendResetInstanceSuccess(instanceSave->GetMapId());
        }

        if (isEmpty || method == InstanceResetMethod::INSTANCE_RESET_GROUP_DISBAND || method == InstanceResetMethod::INSTANCE_RESET_CHANGE_DIFFICULTY)
        {
            // do not reset the instance, just unbind if others are permanently bound to it
            if (instanceSave->CanReset())
                instanceSave->DeleteFromDB();
            else
            {
                PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP_INSTANCE_BY_INSTANCE);

                stmt->setUInt32(0, instanceSave->GetInstanceId());

                CharacterDatabase.Execute(stmt);
            }


            // i don't know for sure if hash_map iterators
            m_boundInstances[diff].erase(itr);
            itr = m_boundInstances[diff].begin();
            // this unloads the instance save unless online players are bound to it
            // (eg. permanent binds or GM solo binds)
            instanceSave->RemoveGroup(this);
        }
        else
            ++itr;
    }
}

InstanceGroupBind* Group::GetBoundInstance(Player* player)
{
    uint32 mapid = player->GetMapId();
    MapEntry const* mapEntry = sMapStore.LookupEntry(mapid);
    return GetBoundInstance(mapEntry);
}

InstanceGroupBind* Group::GetBoundInstance(Map* aMap)
{
    return GetBoundInstance(aMap->GetEntry());
}

InstanceGroupBind* Group::GetBoundInstance(MapEntry const* mapEntry)
{
    if (!mapEntry || !mapEntry->IsInstance())
        return NULL;

    DifficultyID difficulty = GetDifficulty(mapEntry);
    return GetBoundInstance(difficulty, mapEntry->MapID);
}

InstanceGroupBind* Group::GetBoundInstance(DifficultyID difficulty, uint32 mapId)
{
    // some instances only have one difficulty
    GetDownscaledMapDifficultyData(mapId, difficulty);

    BoundInstancesMap::iterator itr = m_boundInstances[difficulty].find(mapId);
    if (itr != m_boundInstances[difficulty].end())
        return &itr->second;
    else
        return NULL;
}

InstanceGroupBind* Group::BindToInstance(InstanceSave* save, bool permanent, bool load)
{
    if (!save || isBGGroup() || isBFGroup())
        return NULL;

    InstanceGroupBind& bind = m_boundInstances[save->GetDifficulty()][save->GetMapId()];
    if (!load && (!bind.save || permanent != bind.perm || save != bind.save))
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_GROUP_INSTANCE);

        stmt->setUInt32(0, m_dbStoreId);
        stmt->setUInt32(1, save->GetInstanceId());
        stmt->setBool(2, permanent);

        CharacterDatabase.Execute(stmt);
    }

    if (bind.save != save)
    {
        if (bind.save)
            bind.save->RemoveGroup(this);
        save->AddGroup(this);
    }

    bind.save = save;
    bind.perm = permanent;
    if (!load)
        SF_LOG_DEBUG("maps", "Group::BindToInstance: Group (guid: %u, storage id: %u) is now bound to map %d, instance %d, difficulty %d",
        GUID_LOPART(GetGUID()), m_dbStoreId, save->GetMapId(), save->GetInstanceId(), save->GetDifficulty());

    return &bind;
}

void Group::UnbindInstance(uint32 mapid, uint8 difficulty, bool unload)
{
    BoundInstancesMap::iterator itr = m_boundInstances[difficulty].find(mapid);
    if (itr != m_boundInstances[difficulty].end())
    {
        if (!unload)
        {
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP_INSTANCE_BY_GUID);

            stmt->setUInt32(0, m_dbStoreId);
            stmt->setUInt32(1, itr->second.save->GetInstanceId());

            CharacterDatabase.Execute(stmt);
        }

        itr->second.save->RemoveGroup(this);                // save can become invalid
        m_boundInstances[difficulty].erase(itr);
    }
}

void Group::_homebindIfInstance(Player* player)
{
    if (player && !player->IsGameMaster() && sMapStore.LookupEntry(player->GetMapId())->IsInstance())
        player->m_InstanceValid = false;
}

void Group::BroadcastGroupUpdate(void)
{
    // FG: HACK: force flags update on group leave - for values update hack
    // -- not very efficient but safe
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
    {
        Player* pp = ObjectAccessor::FindPlayer(citr->guid);
        if (pp && pp->IsInWorld())
        {
            pp->ForceValuesUpdateAtIndex(UNIT_FIELD_SHAPESHIFT_FORM);
            pp->ForceValuesUpdateAtIndex(UNIT_FIELD_FACTION_TEMPLATE);
            SF_LOG_DEBUG("misc", "-- Forced group value update for '%s'", pp->GetName().c_str());
        }
    }
}

void Group::ResetMaxEnchantingLevel()
{
    m_maxEnchantingLevel = 0;
    Player* member = NULL;
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
    {
        member = ObjectAccessor::FindPlayer(citr->guid);
        if (member && m_maxEnchantingLevel < member->GetSkillValue(SKILL_ENCHANTING))
            m_maxEnchantingLevel = member->GetSkillValue(SKILL_ENCHANTING);
    }
}

void Group::SetLootMethod(LootMethod method)
{
    m_lootMethod = method;
}

void Group::SetLooterGuid(uint64 guid)
{
    m_looterGuid = guid;
}

void Group::SetLootThreshold(ItemQualities threshold)
{
    m_lootThreshold = threshold;
}

void Group::SetLfgRoles(uint64 guid, const uint8 roles)
{
    member_witerator slot = _getMemberWSlot(guid);
    if (slot == m_memberSlots.end())
        return;

    slot->roles = roles;
    SendUpdate();
}

bool Group::IsFull() const
{
    return isRaidGroup() ? (m_memberSlots.size() >= MAXRAIDSIZE) : (m_memberSlots.size() >= MAXGROUPSIZE);
}

bool Group::isLFGGroup() const
{
    return m_groupType & GROUPTYPE_LFG;
}

bool Group::isRaidGroup() const
{
    return m_groupType & GROUPTYPE_RAID;
}

bool Group::isBGGroup() const
{
    return m_bgGroup != NULL;
}

bool Group::isBFGroup() const
{
    return m_bfGroup != NULL;
}

bool Group::IsCreated() const
{
    return GetMembersCount() > 0;
}

uint64 Group::GetLeaderGUID() const
{
    return m_leaderGuid;
}

uint64 Group::GetGUID() const
{
    return m_guid;
}

uint32 Group::GetLowGUID() const
{
    return GUID_LOPART(m_guid);
}

char const* Group::GetLeaderName() const
{
    return m_leaderName.c_str();
}

LootMethod Group::GetLootMethod() const
{
    return m_lootMethod;
}

uint64 Group::GetLooterGuid() const
{
    return m_looterGuid;
}

ItemQualities Group::GetLootThreshold() const
{
    return m_lootThreshold;
}

bool Group::IsMember(uint64 guid) const
{
    return _getMemberCSlot(guid) != m_memberSlots.end();
}

bool Group::IsLeader(uint64 guid) const
{
    return (GetLeaderGUID() == guid);
}

uint64 Group::GetMemberGUID(const std::string& name)
{
    for (member_citerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        if (itr->name == name)
            return itr->guid;
    return 0;
}

bool Group::IsAssistant(uint64 guid) const
{
    member_citerator mslot = _getMemberCSlot(guid);
    if (mslot == m_memberSlots.end())
        return false;
    return mslot->flags & uint8(GroupMemberFlags::MEMBER_FLAG_ASSISTANT);
}

bool Group::SameSubGroup(uint64 guid1, uint64 guid2) const
{
    member_citerator mslot2 = _getMemberCSlot(guid2);
    if (mslot2 == m_memberSlots.end())
       return false;
    return SameSubGroup(guid1, &*mslot2);
}

bool Group::SameSubGroup(uint64 guid1, MemberSlot const* slot2) const
{
    member_citerator mslot1 = _getMemberCSlot(guid1);
    if (mslot1 == m_memberSlots.end() || !slot2)
        return false;
    return (mslot1->group == slot2->group);
}

bool Group::HasFreeSlotSubGroup(uint8 subgroup) const
{
    return (m_subGroupsCounts && m_subGroupsCounts[subgroup] < MAXGROUPSIZE);
}


uint8 Group::GetMemberGroup(uint64 guid) const
{
    member_citerator mslot = _getMemberCSlot(guid);
    if (mslot == m_memberSlots.end())
       return (MAX_RAID_SUBGROUPS+1);
    return mslot->group;
}

void Group::SetBattlegroundGroup(Battleground* bg)
{
    m_bgGroup = bg;
}

void Group::SetBattlefieldGroup(Battlefield *bg)
{
    m_bfGroup = bg;
}

void Group::SetGroupMemberFlag(uint64 guid, bool apply, GroupMemberFlags flag)
{
    // Assistants, main assistants and main tanks are only available in raid groups
    if (!isRaidGroup())
       return;

    // Check if player is really in the raid
    member_witerator slot = _getMemberWSlot(guid);
    if (slot == m_memberSlots.end())
        return;

    // Do flag specific actions, e.g ensure uniqueness
    switch (flag)
    {
        case GroupMemberFlags::MEMBER_FLAG_MAINASSIST:
            RemoveUniqueGroupMemberFlag(GroupMemberFlags::MEMBER_FLAG_MAINASSIST);         // Remove main assist flag from current if any.
            break;
        case GroupMemberFlags::MEMBER_FLAG_MAINTANK:
            RemoveUniqueGroupMemberFlag(GroupMemberFlags::MEMBER_FLAG_MAINTANK);           // Remove main tank flag from current if any.
            break;
        case GroupMemberFlags::MEMBER_FLAG_ASSISTANT:
            break;
        default:
            return;                                                      // This should never happen
    }

    // Switch the actual flag
    ToggleGroupMemberFlag(slot, flag, apply);

    // Preserve the new setting in the db
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_MEMBER_FLAG);

    stmt->setUInt8(0, slot->flags);
    stmt->setUInt32(1, GUID_LOPART(guid));

    CharacterDatabase.Execute(stmt);

    // Broadcast the changes to the group
    SendUpdate();
}

DifficultyID Group::GetDifficulty(MapEntry const* mapEntry) const
{
    if (!mapEntry->IsRaid())
        return m_dungeonDifficulty;

    return m_raidDifficulty;
}
/*
DifficultyID Group::GetDungeonDifficulty() const
{
    return m_dungeonDifficulty;
}

DifficultyID Group::GetRaidDifficulty() const
{
    return m_raidDifficulty;
}
*/
bool Group::isRollLootActive() const
{
    return !RollId.empty();
}

Group::Rolls::iterator Group::GetRoll(uint64 Guid)
{
    Rolls::iterator iter;
    for (iter=RollId.begin(); iter != RollId.end(); ++iter)
        if ((*iter)->itemGUID == Guid && (*iter)->isValid())
            return iter;
    return RollId.end();
}

void Group::LinkMember(GroupReference* pRef)
{
    m_memberMgr.insertFirst(pRef);
}

void Group::DelinkMember(uint64 guid)
{
    GroupReference* ref = m_memberMgr.getFirst();
    while (ref)
    {
        GroupReference* nextRef = ref->next();
        if (ref->GetSource()->GetGUID() == guid)
        {
            ref->unlink();
            break;
        }
        ref = nextRef;
    }
}

Group::BoundInstancesMap& Group::GetBoundInstances(DifficultyID difficulty)
{
    return m_boundInstances[difficulty];
}

void Group::_initRaidSubGroupsCounter()
{
    // Sub group counters initialization
    if (!m_subGroupsCounts)
        m_subGroupsCounts = new uint8[MAX_RAID_SUBGROUPS];

    memset((void*)m_subGroupsCounts, 0, (MAX_RAID_SUBGROUPS)*sizeof(uint8));

    for (member_citerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        ++m_subGroupsCounts[itr->group];
}

Group::member_citerator Group::_getMemberCSlot(uint64 Guid) const
{
    for (member_citerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        if (itr->guid == Guid)
            return itr;
    return m_memberSlots.end();
}

Group::member_witerator Group::_getMemberWSlot(uint64 Guid)
{
    for (member_witerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        if (itr->guid == Guid)
            return itr;
    return m_memberSlots.end();
}

void Group::SubGroupCounterIncrease(uint8 subgroup)
{
    if (m_subGroupsCounts)
        ++m_subGroupsCounts[subgroup];
}

void Group::SubGroupCounterDecrease(uint8 subgroup)
{
    if (m_subGroupsCounts)
        --m_subGroupsCounts[subgroup];
}

void Group::RemoveUniqueGroupMemberFlag(GroupMemberFlags flag)
{
    for (member_witerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        if (itr->flags & uint8(flag))
            itr->flags &= ~uint8(flag);
}

void Group::ToggleGroupMemberFlag(member_witerator slot, GroupMemberFlags flag, bool apply)
{
    if (apply)
        slot->flags |= uint8(flag);
    else
        slot->flags &= ~uint8(flag);
}

void Group::SetMemberRole(uint64 guid, uint32 role)
{
    member_witerator itr = _getMemberWSlot(guid);
    if (itr == m_memberSlots.end())
        return;

    itr->roles = role;

    PreparedStatement* prepStatement = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_ROLE);
    prepStatement->setUInt8(0, role);
    prepStatement->setUInt32(1, GUID_LOPART(guid));

    CharacterDatabase.Execute(prepStatement);
}

uint32 Group::GetMemberRole(uint64 guid) const
{
    member_citerator itr = _getMemberCSlot(guid);
    if (itr == m_memberSlots.end())
        return 0;

    return itr->roles;
}

bool Group::RoleCheckAllResponded() const
{
    for (member_citerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        if (!itr->roles)
            return false;

    return true;
}

void Group::ReadyCheckMemberHasResponded(uint64 guid)
{
    member_witerator itr = _getMemberWSlot(guid);
    if (itr == m_memberSlots.end())
        return;

    itr->readyCheckHasResponded = true;
}

void Group::ReadyCheckResetResponded()
{
    for (member_witerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        itr->readyCheckHasResponded = false;
}

bool Group::ReadyCheckAllResponded() const
{
    for (member_citerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        if (!itr->readyCheckHasResponded)
            return false;

    return true;
}
