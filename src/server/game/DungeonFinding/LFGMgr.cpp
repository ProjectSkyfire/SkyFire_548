/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Common.h"
#include "DBCStores.h"
#include "DisableMgr.h"
#include "GameEventMgr.h"
#include "Group.h"
#include "GroupMgr.h"
#include "GridDefines.h"
#include "InstanceSaveMgr.h"
#include "LFGGroupData.h"
#include "LFGMgr.h"
#include "LFGPlayerData.h"
#include "LFGQueue.h"
#include "LFGScripts.h"
#include "MapManager.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "RBAC.h"
#include "SharedDefines.h"
#include "SocialMgr.h"
#include "WorldSession.h"

#include <algorithm>
#include <vector>

namespace lfg
{
    namespace
    {
        uint8 const LFG_COMBAT_ROLE_MASK = PLAYER_ROLE_TANK | PLAYER_ROLE_HEALER | PLAYER_ROLE_DAMAGE;

        bool IsScenarioDifficulty(uint32 difficulty)
        {
            return difficulty == DIFFICULTY_SCE_NORMAL || difficulty == DIFFICULTY_SCE_HEROIC;
        }

        bool IsScenarioDungeon(LFGDungeonEntry const* dungeon)
        {
            if (!dungeon)
                return false;

            if (IsScenarioDifficulty(dungeon->m_DifficultyID))
                return true;

            MapEntry const* map = sMapStore.LookupEntry(dungeon->m_ContinentID);
            return map && map->IsScenario();
        }

        bool IsScenarioDungeon(LFGDungeonData const& dungeon)
        {
            if (IsScenarioDifficulty(dungeon.difficulty))
                return true;

            MapEntry const* map = sMapStore.LookupEntry(dungeon.map);
            return map && map->IsScenario();
        }

        bool IsRaidDungeon(LFGDungeonData const& dungeon)
        {
            if (dungeon.type == LFG_TYPE_RAID)
                return true;

            MapEntry const* map = sMapStore.LookupEntry(dungeon.map);
            return map && map->IsRaid();
        }

        bool IsFlexibleRaidData(LFGDungeonData const& dungeon)
        {
            return dungeon.difficulty == DIFFICULTY_FLEX && IsRaidDungeon(dungeon);
        }

        bool HasValidLfgTeleportLocation(LFGDungeonData const& dungeon)
        {
            if (!dungeon.map || (dungeon.x == 0.0f && dungeon.y == 0.0f && dungeon.z == 0.0f))
                return false;

            return Skyfire::IsValidMapCoord(dungeon.x, dungeon.y, dungeon.z, dungeon.o);
        }

        bool BindLfgGroupToDungeonInstance(Group* group, LFGDungeonData const& dungeon)
        {
            if (!group)
                return false;

            MapEntry const* map = sMapStore.LookupEntry(dungeon.map);
            if (!map || (!map->IsInstance() && !map->IsScenario()))
                return true;

            DifficultyID difficulty = DifficultyID(dungeon.difficulty);
            if (group->GetBoundInstance(difficulty, dungeon.map))
                return true;

            InstanceSave* save = sInstanceSaveMgr->AddInstanceSave(dungeon.map, sMapMgr->GenerateInstanceId(), difficulty, 0, true);
            if (!save)
                return false;

            return group->BindToInstance(save, false) != NULL;
        }

        struct LfgRoleAssignment
        {
            uint64 guid;
            uint8 leader;
            uint8 availableRoles;
            uint8 assignedRole;
        };

        uint8 CountAvailableRoles(uint8 roles)
        {
            uint8 count = 0;
            if (roles & PLAYER_ROLE_TANK)
                ++count;
            if (roles & PLAYER_ROLE_HEALER)
                ++count;
            if (roles & PLAYER_ROLE_DAMAGE)
                ++count;
            return count;
        }

        bool TryAssignLfgRoles(std::vector<LfgRoleAssignment>& assignments, size_t index, uint8 tanks, uint8 healers, uint8 damage)
        {
            if (index == assignments.size())
                return true;

            LfgRoleAssignment& assignment = assignments[index];
            uint8 const rolePreference[] = { PLAYER_ROLE_TANK, PLAYER_ROLE_HEALER, PLAYER_ROLE_DAMAGE };

            for (uint8 role : rolePreference)
            {
                if (!(assignment.availableRoles & role))
                    continue;

                if (role == PLAYER_ROLE_TANK && tanks >= LFG_TANKS_NEEDED)
                    continue;
                if (role == PLAYER_ROLE_HEALER && healers >= LFG_HEALERS_NEEDED)
                    continue;
                if (role == PLAYER_ROLE_DAMAGE && damage >= LFG_DPS_NEEDED)
                    continue;

                assignment.assignedRole = role | assignment.leader;

                if (TryAssignLfgRoles(assignments, index + 1,
                    tanks + (role == PLAYER_ROLE_TANK),
                    healers + (role == PLAYER_ROLE_HEALER),
                    damage + (role == PLAYER_ROLE_DAMAGE)))
                    return true;
            }

            assignment.assignedRole = PLAYER_ROLE_NONE;
            return false;
        }
    }

    LFGMgr::LFGMgr() : m_QueueTimer(0), m_lfgProposalId(1),
        m_options(sWorld->getIntConfig(WorldIntConfigs::CONFIG_LFG_OPTIONSMASK)),
        m_debugRequirementOverride(false)
    {
        new LFGPlayerScript();
        new LFGGroupScript();
    }

    LFGMgr::~LFGMgr()
    {
        for (LfgRewardContainer::iterator itr = RewardMapStore.begin(); itr != RewardMapStore.end(); ++itr)
            delete itr->second;
    }

    void LFGMgr::_LoadFromDB(Field* fields, uint64 guid)
    {
        if (!fields)
            return;

        if (!IS_GROUP_GUID(guid))
            return;

        SetLeader(guid, MAKE_NEW_GUID(fields[0].GetUInt32(), 0, HIGHGUID_PLAYER));

        uint32 dungeon = fields[16].GetUInt32();
        uint8 state = fields[17].GetUInt8();

        if (!dungeon || !state)
            return;

        SetDungeon(guid, dungeon);

        switch (state)
        {
            case LFG_STATE_DUNGEON:
            case LFG_STATE_FINISHED_DUNGEON:
                //case LFG_STATE_BOOT:
                SetState(guid, (LfgState)state);
                break;
            default:
                break;
        }
    }

    void LFGMgr::_SaveToDB(uint64 guid, uint32 db_guid)
    {
        if (!IS_GROUP_GUID(guid))
            return;
        SQLTransaction trans = CharacterDatabase.BeginTransaction();
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_LFG_DATA);

        stmt->setUInt32(0, db_guid);

        trans->Append(stmt);

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_LFG_DATA);
        stmt->setUInt32(0, db_guid);

        stmt->setUInt32(1, GetDungeon(guid));
        stmt->setUInt32(2, GetState(guid));
        trans->Append(stmt);
        CharacterDatabase.CommitTransaction(trans);
    }

    /// Load rewards for completing dungeons
    void LFGMgr::LoadRewards()
    {
        uint32 oldMSTime = getMSTime();

        for (LfgRewardContainer::iterator itr = RewardMapStore.begin(); itr != RewardMapStore.end(); ++itr)
            delete itr->second;
        RewardMapStore.clear();

        // ORDER BY is very important for GetRandomDungeonReward!
        QueryResult result = WorldDatabase.Query("SELECT dungeonId, maxLevel, firstQuestId, otherQuestId FROM lfg_dungeon_rewards ORDER BY dungeonId, maxLevel ASC");

        if (!result)
        {
            SF_LOG_ERROR("server.loading", ">> Loaded 0 lfg dungeon rewards. DB table `lfg_dungeon_rewards` is empty!");
            return;
        }

        uint32 count = 0;

        Field* fields = NULL;
        do
        {
            fields = result->Fetch();
            uint32 dungeonId = fields[0].GetUInt32();
            uint32 maxLevel = fields[1].GetUInt8();
            uint32 firstQuestId = fields[2].GetUInt32();
            uint32 otherQuestId = fields[3].GetUInt32();

            if (!GetLFGDungeonEntry(dungeonId))
            {
                SF_LOG_ERROR("sql.sql", "Dungeon %u specified in table `lfg_dungeon_rewards` does not exist!", dungeonId);
                continue;
            }

            if (!maxLevel || maxLevel > sWorld->getIntConfig(WorldIntConfigs::CONFIG_MAX_PLAYER_LEVEL))
            {
                SF_LOG_ERROR("sql.sql", "Level %u specified for dungeon %u in table `lfg_dungeon_rewards` can never be reached!", maxLevel, dungeonId);
                maxLevel = sWorld->getIntConfig(WorldIntConfigs::CONFIG_MAX_PLAYER_LEVEL);
            }

            if (!firstQuestId || !sObjectMgr->GetQuestTemplate(firstQuestId))
            {
                SF_LOG_ERROR("sql.sql", "First quest %u specified for dungeon %u in table `lfg_dungeon_rewards` does not exist!", firstQuestId, dungeonId);
                continue;
            }

            if (otherQuestId && !sObjectMgr->GetQuestTemplate(otherQuestId))
            {
                SF_LOG_ERROR("sql.sql", "Other quest %u specified for dungeon %u in table `lfg_dungeon_rewards` does not exist!", otherQuestId, dungeonId);
                otherQuestId = 0;
            }

            RewardMapStore.insert(LfgRewardContainer::value_type(dungeonId, new LfgReward(maxLevel, firstQuestId, otherQuestId)));
            ++count;
        } while (result->NextRow());

        SF_LOG_INFO("server.loading", ">> Loaded %u lfg dungeon rewards in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    }

    LFGDungeonData const* LFGMgr::GetLFGDungeon(uint32 id)
    {
        LFGDungeonContainer::const_iterator itr = LfgDungeonStore.find(id);
        if (itr != LfgDungeonStore.end())
            return &(itr->second);

        return NULL;
    }

    void LFGMgr::LoadLFGDungeons(bool reload /* = false */)
    {
        uint32 oldMSTime = getMSTime();

        LfgDungeonStore.clear();

        // Initialize Dungeon map with data from dbcs
        for (uint32 i = 0; i < sLFGDungeonStore.GetNumRows(); ++i)
        {
            LFGDungeonEntry const* dungeon = sLFGDungeonStore.LookupEntry(i);
            if (!dungeon)
                continue;

            switch (dungeon->m_Type)
            {
                case LFG_TYPE_DUNGEON:
                case LFG_TYPE_RAID:
                case LFG_TYPE_RANDOM:
                    LfgDungeonStore[dungeon->m_ID] = LFGDungeonData(dungeon);
                    break;
                default:
                    if (IsScenarioDungeon(dungeon))
                        LfgDungeonStore[dungeon->m_ID] = LFGDungeonData(dungeon);
                    break;
            }
        }

        // Fill teleport locations and lock metadata from DB.
        QueryResult result;
        bool usingDungeonTemplate = false;
        QueryResult dungeonTemplateTable = WorldDatabase.Query("SELECT 1 FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = 'lfg_dungeon_template' LIMIT 1");
        if (dungeonTemplateTable)
        {
            result = WorldDatabase.Query("SELECT dungeonId, position_x, position_y, position_z, orientation, requiredItemLevel FROM lfg_dungeon_template");
            if (result)
                usingDungeonTemplate = true;
        }

        if (!result)
            result = WorldDatabase.Query("SELECT dungeonId, position_x, position_y, position_z, orientation FROM lfg_entrances");

        uint32 count = 0;

        if (result)
        {
            do
            {
                Field* fields = result->Fetch();
                uint32 dungeonId = fields[0].GetUInt32();
                LFGDungeonContainer::iterator dungeonItr = LfgDungeonStore.find(dungeonId);
                if (dungeonItr == LfgDungeonStore.end())
                {
                    SF_LOG_ERROR("sql.sql", "table `%s` contains data for wrong dungeon %u", usingDungeonTemplate ? "lfg_dungeon_template" : "lfg_entrances", dungeonId);
                    continue;
                }

                LFGDungeonData& data = dungeonItr->second;
                data.x = fields[1].GetFloat();
                data.y = fields[2].GetFloat();
                data.z = fields[3].GetFloat();
                data.o = fields[4].GetFloat();

                if (usingDungeonTemplate)
                    data.requiredItemLevel = fields[5].GetUInt32();

                ++count;
            } while (result->NextRow());
        }

        if (usingDungeonTemplate)
            SF_LOG_INFO("server.loading", ">> Loaded %u lfg dungeon templates in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
        else if (result)
            SF_LOG_INFO("server.loading", ">> Loaded %u lfg entrance positions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
        else
            SF_LOG_ERROR("server.loading", ">> Loaded 0 lfg entrance positions. DB tables `lfg_dungeon_template` and `lfg_entrances` are empty or missing!");

        // Fill all other teleport coords from areatriggers
        for (LFGDungeonContainer::iterator itr = LfgDungeonStore.begin(); itr != LfgDungeonStore.end(); ++itr)
        {
            LFGDungeonData& dungeon = itr->second;

            // No teleport coords in database, load from areatriggers
            if (dungeon.type != LFG_TYPE_RANDOM && dungeon.x == 0.0f && dungeon.y == 0.0f && dungeon.z == 0.0f)
            {
                AreaTriggerStruct const* at = sObjectMgr->GetMapEntranceTrigger(dungeon.map);
                if (!at)
                {
                    SF_LOG_ERROR("sql.sql", "Failed to load dungeon %s, cant find areatrigger for map %u", dungeon.name.c_str(), dungeon.map);
                    continue;
                }

                dungeon.map = at->target_mapId;
                dungeon.x = at->target_X;
                dungeon.y = at->target_Y;
                dungeon.z = at->target_Z;
                dungeon.o = at->target_Orientation;
            }

            if (dungeon.type != LFG_TYPE_RANDOM && !HasValidLfgTeleportLocation(dungeon))
            {
                SF_LOG_ERROR("sql.sql", "LFG dungeon %u (%s) has no valid teleport location for map %u. Add lfg_dungeon_template data.",
                    dungeon.id, dungeon.name.c_str(), uint32(dungeon.map));
                continue;
            }

            if (dungeon.type != LFG_TYPE_RANDOM)
                CachedDungeonMapStore[dungeon.group].insert(dungeon.id);
            CachedDungeonMapStore[0].insert(dungeon.id);
        }

        if (reload)
            CachedDungeonMapStore.clear();
    }

    void LFGMgr::Update(uint32 diff)
    {
        if (!isOptionEnabled(LFG_OPTION_ENABLE_DUNGEON_FINDER | LFG_OPTION_ENABLE_RAID_BROWSER))
            return;

        time_t currTime = time(NULL);

        // Remove obsolete role checks
        for (LfgRoleCheckContainer::iterator it = RoleChecksStore.begin(); it != RoleChecksStore.end();)
        {
            LfgRoleCheckContainer::iterator itRoleCheck = it++;
            LfgRoleCheck& roleCheck = itRoleCheck->second;
            if (currTime < roleCheck.cancelTime)
                continue;

            if (IS_GROUP_GUID(itRoleCheck->first) && GroupsStore.find(itRoleCheck->first) == GroupsStore.end())
            {
                RoleChecksStore.erase(itRoleCheck);
                continue;
            }

            roleCheck.state = LFG_ROLECHECK_MISSING_ROLE;

            for (LfgRolesMap::const_iterator itRoles = roleCheck.roles.begin(); itRoles != roleCheck.roles.end(); ++itRoles)
            {
                uint64 guid = itRoles->first;
                SendLfgRoleCheckUpdate(guid, roleCheck);
                if (guid == roleCheck.leader)
                    SendLfgJoinResult(guid, LfgJoinResultData(LFG_JOIN_ROLE_CHECK_FAILED, LFG_ROLECHECK_MISSING_ROLE));
                SendLfgUpdateStatus(guid, LfgUpdateData(LFG_UPDATETYPE_ROLECHECK_FAILED), true);
                RestoreOrClearState(guid, "Remove Obsolete RoleCheck");
            }

            RestoreOrClearState(itRoleCheck->first, "Remove Obsolete RoleCheck");
            RoleChecksStore.erase(itRoleCheck);
        }

        // Remove obsolete proposals
        for (LfgProposalContainer::iterator it = ProposalsStore.begin(); it != ProposalsStore.end();)
        {
            LfgProposalContainer::iterator itRemove = it++;
            if (itRemove->second.cancelTime < currTime)
                RemoveProposal(itRemove, LFG_UPDATETYPE_PROPOSAL_FAILED);
        }

        // Remove obsolete kicks
        for (LfgPlayerBootContainer::iterator it = BootsStore.begin(); it != BootsStore.end();)
        {
            LfgPlayerBootContainer::iterator itBoot = it++;
            LfgPlayerBoot& boot = itBoot->second;
            if (boot.cancelTime < currTime)
            {
                LfgGroupDataContainer::const_iterator groupData = GroupsStore.find(itBoot->first);
                if (groupData == GroupsStore.end())
                {
                    BootsStore.erase(itBoot);
                    continue;
                }

                boot.inProgress = false;
                for (LfgAnswerContainer::const_iterator itVotes = boot.votes.begin(); itVotes != boot.votes.end(); ++itVotes)
                {
                    uint64 pguid = itVotes->first;
                    if (pguid != boot.victim)
                        SendLfgBootProposalUpdate(pguid, boot);
                    SetState(pguid, LFG_STATE_DUNGEON);
                }
                SetState(itBoot->first, LFG_STATE_DUNGEON);
                SetVoteKick(itBoot->first, false);
                BootsStore.erase(itBoot);
            }
        }

        uint32 lastProposalId = m_lfgProposalId;
        // Check if a proposal can be formed with the new groups being added
        for (LfgQueueContainer::iterator it = QueuesStore.begin(); it != QueuesStore.end(); ++it)
            if (uint8 newProposals = it->second.FindGroups())
                SF_LOG_DEBUG("lfg.update", "Found %u new groups in queue %u", newProposals, it->first);

        if (lastProposalId != m_lfgProposalId)
        {
            for (LfgProposalContainer::const_iterator itProposal = ProposalsStore.upper_bound(lastProposalId); itProposal != ProposalsStore.end(); ++itProposal)
            {
                uint32 proposalId = itProposal->first;
                LfgProposal& proposal = ProposalsStore[proposalId];

                uint64 guid = 0;
                for (LfgProposalPlayerContainer::const_iterator itPlayers = proposal.players.begin(); itPlayers != proposal.players.end(); ++itPlayers)
                {
                    guid = itPlayers->first;
                    SetState(guid, LFG_STATE_PROPOSAL);
                    if (uint64 gguid = GetGroup(guid))
                    {
                        SetState(gguid, LFG_STATE_PROPOSAL);
                        SendLfgUpdateStatus(guid, LfgUpdateData(LFG_UPDATETYPE_PROPOSAL_BEGIN, GetSelectedDungeons(guid), GetComment(guid)), true);
                    }
                    else
                        SendLfgUpdateStatus(guid, LfgUpdateData(LFG_UPDATETYPE_PROPOSAL_BEGIN, GetSelectedDungeons(guid), GetComment(guid)), false);
                    SendLfgUpdateProposal(guid, proposal);
                }

                if (proposal.state == LFG_PROPOSAL_SUCCESS)
                    UpdateProposal(proposalId, guid, true);
            }
        }

        // Update all players status queue info
        if (m_QueueTimer > LFG_QUEUEUPDATE_INTERVAL)
        {
            m_QueueTimer = 0;
            time_t currTime = time(NULL);
            for (LfgQueueContainer::iterator it = QueuesStore.begin(); it != QueuesStore.end(); ++it)
                it->second.UpdateQueueTimers(it->first, currTime);
        }
        else
            m_QueueTimer += diff;
    }

    /**
        Adds the player/group to lfg queue. If player is in a group then it is the leader
        of the group tying to join the group. Join conditions are checked before adding
        to the new queue.

       @param[in]     player Player trying to join (or leader of group trying to join)
       @param[in]     roles Player selected roles
       @param[in]     dungeons Dungeons the player/group is applying for
       @param[in]     comment Player selected comment
    */
    void LFGMgr::JoinLfg(Player* player, uint8 roles, LfgDungeonSet& dungeons, const std::string& comment)
    {
        if (!player || !player->GetSession() || dungeons.empty())
            return;

        Group* grp = player->GetGroup();
        uint64 guid = player->GetGUID();
        uint64 gguid = grp ? grp->GetGUID() : guid;
        uint8 queueId = GetTeam(guid);
        LfgJoinResultData joinData;
        LfgGuidSet players;
        uint32 rDungeonId = 0;
        bool isContinue = grp && grp->isLFGGroup() && GetState(gguid) == LFG_STATE_DUNGEON;
        bool hasFlexibleRaid = false;

        // Do not allow to change dungeon in the middle of a current dungeon
        if (isContinue)
        {
            dungeons.clear();
            dungeons.insert(GetDungeon(gguid));
        }

        for (LfgDungeonSet::const_iterator it = dungeons.begin(); it != dungeons.end(); ++it)
            if (IsFlexibleRaidDungeon(*it))
            {
                hasFlexibleRaid = true;
                break;
            }

        // Already in queue?
        LfgState state = GetState(gguid);
        bool hasActiveQueueState = state == LFG_STATE_ROLECHECK || state == LFG_STATE_PROPOSAL ||
            state == LFG_STATE_DUNGEON || state == LFG_STATE_BOOT;
        if (state == LFG_STATE_QUEUED)
        {
            LFGQueue& queue = GetQueue(gguid);
            queue.RemoveFromQueue(gguid);
        }

        // Check player or group member restrictions
        if (hasActiveQueueState && !isContinue)
            joinData.result = LFG_JOIN_INTERNAL_ERROR;
        else if (!IsValidPlayerRoles(roles))
        {
            joinData.result = LFG_JOIN_ROLE_CHECK_FAILED;
            joinData.state = LFG_ROLECHECK_NO_ROLE;
        }
        else if (grp && !grp->isLFGGroup() && !grp->IsLeader(guid))
            joinData.result = LFG_JOIN_INTERNAL_ERROR;
        else if (!player->GetSession()->HasPermission(rbac::RBAC_PERM_JOIN_DUNGEON_FINDER))
            joinData.result = LFG_JOIN_NOT_MEET_REQS;
        else if (player->InBattleground() || player->InArena() || player->InBattlegroundQueue())
            joinData.result = LFG_JOIN_USING_BG_SYSTEM;
        else if (player->HasAura(LFG_SPELL_DUNGEON_DESERTER))
            joinData.result = LFG_JOIN_DESERTER;
        else if (player->HasAura(LFG_SPELL_DUNGEON_COOLDOWN))
            joinData.result = LFG_JOIN_RANDOM_COOLDOWN;
        else if (dungeons.empty())
            joinData.result = LFG_JOIN_NOT_MEET_REQS;
        else if (grp)
        {
            uint8 groupMemberLimit = hasFlexibleRaid ? MAXRAIDSIZE : MAXGROUPSIZE;
            if (grp->GetMembersCount() > groupMemberLimit)
                joinData.result = LFG_JOIN_TOO_MUCH_MEMBERS;
            else
            {
                uint8 memberCount = 0;
                for (GroupReference* itr = grp->GetFirstMember(); itr != NULL && joinData.result == LFG_JOIN_OK; itr = itr->next())
                {
                    if (Player* plrg = itr->GetSource())
                    {
                        if (!plrg->GetSession()->HasPermission(rbac::RBAC_PERM_JOIN_DUNGEON_FINDER))
                            joinData.result = LFG_JOIN_INTERNAL_ERROR;
                        if (plrg->HasAura(LFG_SPELL_DUNGEON_DESERTER))
                            joinData.result = LFG_JOIN_PARTY_DESERTER;
                        else if (plrg->HasAura(LFG_SPELL_DUNGEON_COOLDOWN))
                            joinData.result = LFG_JOIN_PARTY_RANDOM_COOLDOWN;
                        else if (plrg->InBattleground() || plrg->InArena() || plrg->InBattlegroundQueue())
                            joinData.result = LFG_JOIN_USING_BG_SYSTEM;
                        ++memberCount;
                        players.insert(plrg->GetGUID());
                    }
                }

                if (joinData.result == LFG_JOIN_OK && memberCount != grp->GetMembersCount())
                    joinData.result = LFG_JOIN_DISCONNECTED;
            }
        }
        else
            players.insert(player->GetGUID());

        // Check if all dungeons are valid
        bool isRaid = false;
        if (joinData.result == LFG_JOIN_OK)
        {
            bool isDungeon = false;
            bool hasNonFlexibleRaid = false;
            for (LfgDungeonSet::const_iterator it = dungeons.begin(); it != dungeons.end() && joinData.result == LFG_JOIN_OK; ++it)
            {
                LfgType type = GetDungeonType(*it);
                switch (type)
                {
                    case LFG_TYPE_RANDOM:
                        if (dungeons.size() > 1)               // Only allow 1 random dungeon
                            joinData.result = LFG_JOIN_DUNGEON_INVALID;
                        else
                            rDungeonId = (*dungeons.begin());
                        // No break on purpose (Random can only be dungeon or heroic dungeon)
                    case LFG_TYPE_DUNGEON:
                        if (isRaid)
                            joinData.result = LFG_JOIN_MIXED_RAID_DUNGEON;
                        isDungeon = true;
                        break;
                    case LFG_TYPE_RAID:
                        if (isDungeon)
                            joinData.result = LFG_JOIN_MIXED_RAID_DUNGEON;
                        isRaid = true;
                        if (IsFlexibleRaidDungeon(*it))
                            hasFlexibleRaid = true;
                        else
                            hasNonFlexibleRaid = true;
                        if (hasFlexibleRaid && hasNonFlexibleRaid)
                            joinData.result = LFG_JOIN_MIXED_RAID_DUNGEON;
                        break;
                    default:
                        joinData.result = LFG_JOIN_DUNGEON_INVALID;
                        break;
                }
            }

            // it could be changed
            if (joinData.result == LFG_JOIN_OK)
            {
                // Expand random dungeons and check restrictions
                if (rDungeonId)
                    dungeons = GetDungeonsByRandom(rDungeonId);

                // if we have lockmap then there are no compatible dungeons
                GetCompatibleDungeons(dungeons, players, joinData.lockmap, isContinue);
                if (dungeons.empty())
                    joinData.result = grp ? LFG_JOIN_INTERNAL_ERROR : LFG_JOIN_NOT_MEET_REQS;
            }
        }

        // Can't join. Send result
        if (joinData.result != LFG_JOIN_OK)
        {
            SF_LOG_DEBUG("lfg.join", "%u joining with %u members. Result: %u, Dungeons: %s",
                GUID_LOPART(guid), grp ? grp->GetMembersCount() : 1, joinData.result, ConcatenateDungeons(dungeons).c_str());
            if (!dungeons.empty())                             // Only should show lockmap when have no dungeons available
                joinData.lockmap.clear();
            player->GetSession()->SendLfgJoinResult(joinData);
            return;
        }

        if (isRaid && !hasFlexibleRaid)
        {
            SF_LOG_DEBUG("lfg.join", "%u trying to join raid browser and it's disabled.", GUID_LOPART(guid));
            return;
        }

        if (grp)
            SetActiveQueueId(gguid, queueId);

        SetActiveQueueId(guid, queueId);
        SetComment(guid, comment);

        std::string debugNames = "";
        if (grp)                                               // Begin rolecheck
        {
            // Create new rolecheck
            LfgRoleCheck& roleCheck = RoleChecksStore[gguid];
            roleCheck.cancelTime = time_t(time(NULL)) + LFG_TIME_ROLECHECK;
            roleCheck.state = grp->RoleCheckAllResponded() ? LFG_ROLECHECK_FINISHED : LFG_ROLECHECK_INITIALITING;
            roleCheck.leader = guid;
            roleCheck.dungeons = dungeons;
            roleCheck.rDungeonId = rDungeonId;

            if (rDungeonId)
            {
                dungeons.clear();
                dungeons.insert(rDungeonId);
            }

            SetState(gguid, LFG_STATE_ROLECHECK);
            // Send update to player
            LfgUpdateData updateData = LfgUpdateData(LFG_UPDATETYPE_JOIN_QUEUE, dungeons, comment);
            for (GroupReference* itr = grp->GetFirstMember(); itr != NULL; itr = itr->next())
            {
                if (Player* plrg = itr->GetSource())
                {
                    uint64 pguid = plrg->GetGUID();
                    plrg->GetSession()->SendLfgUpdateStatus(updateData, false);
                    SetActiveQueueId(pguid, queueId);
                    SetState(pguid, LFG_STATE_ROLECHECK);
                    if (!isContinue)
                        SetSelectedDungeons(pguid, dungeons);
                    roleCheck.roles[pguid] = grp->GetMemberRole(pguid);
                    if (!debugNames.empty())
                        debugNames.append(", ");
                    debugNames.append(plrg->GetName());
                }
            }
            // Update leader role
            UpdateRoleCheck(gguid, guid, roles);
        }
        else                                                   // Add player to queue
        {
            LfgRolesMap rolesMap;
            rolesMap[guid] = roles;
            LFGQueue& queue = GetQueue(guid);
            queue.AddQueueData(guid, time(NULL), dungeons, rolesMap);

            if (!isContinue)
            {
                if (rDungeonId)
                {
                    dungeons.clear();
                    dungeons.insert(rDungeonId);
                }
                SetSelectedDungeons(guid, dungeons);
            }
            // Send update to player
            player->GetSession()->SendLfgJoinResult(joinData);
            player->GetSession()->SendLfgUpdateStatus(LfgUpdateData(LFG_UPDATETYPE_JOIN_QUEUE, dungeons, comment), false);
            SetState(gguid, LFG_STATE_QUEUED);
            SetRoles(guid, roles);
            debugNames.append(player->GetName());
        }

        SF_LOG_DEBUG("lfg.join", "%u joined (%s), Members: %s. Dungeons (%u): %s", GUID_LOPART(guid),
            grp ? "group" : "player", debugNames.c_str(), uint32(dungeons.size()), ConcatenateDungeons(dungeons).c_str());
    }

    /**
        Leaves Dungeon System. Player/Group is removed from queue, rolechecks, proposals
        or votekicks. Player or group needs to be not NULL and using Dungeon System

       @param[in]     guid Player or group guid
    */
    void LFGMgr::LeaveLfg(uint64 guid, bool disconnected)
    {
        uint64 gguid = IS_GROUP_GUID(guid) ? guid : GetGroup(guid);

        SF_LOG_DEBUG("lfg.leave", "%u left (%s)", GUID_LOPART(guid), guid == gguid ? "group" : "player");

        LfgState state = GetState(guid);
        switch (state)
        {
            case LFG_STATE_QUEUED:
                if (gguid)
                    ClearGroupQueueState(gguid, "Leave queued group", true);
                else
                {
                    LFGQueue& queue = GetQueue(guid);
                    queue.RemoveFromQueue(guid);
                    SendLfgUpdateStatus(guid, LfgUpdateData(LFG_UPDATETYPE_REMOVED_FROM_QUEUE), false);
                    ClearQueueState(guid, "Leave queued player");
                }
                break;
            case LFG_STATE_ROLECHECK:
                if (gguid)
                    UpdateRoleCheck(gguid);                    // No player to update role = LFG_ROLECHECK_ABORTED
                break;
            case LFG_STATE_PROPOSAL:
            {
                // Remove from Proposals
                LfgProposalContainer::iterator it = ProposalsStore.begin();
                uint64 pguid = gguid == guid ? GetLeader(gguid) : guid;
                while (it != ProposalsStore.end())
                {
                    LfgProposalPlayerContainer::iterator itPlayer = it->second.players.find(pguid);
                    if (itPlayer != it->second.players.end())
                    {
                        // Mark the player/leader of group who left as didn't accept the proposal
                        itPlayer->second.accept = LFG_ANSWER_DENY;
                        break;
                    }
                    ++it;
                }

                // Remove from queue - if proposal is found, RemoveProposal will call RemoveFromQueue
                if (it != ProposalsStore.end())
                    RemoveProposal(it, LFG_UPDATETYPE_PROPOSAL_DECLINED);
                break;
            }
            case LFG_STATE_NONE:
            case LFG_STATE_RAIDBROWSER:
                break;
            case LFG_STATE_DUNGEON:
            case LFG_STATE_FINISHED_DUNGEON:
                //case LFG_STATE_BOOT:
                if (guid == gguid && gguid && !disconnected)
                    ClearGroupQueueState(gguid, "Leave dungeon group", true);
                else if (guid != gguid && !disconnected) // Player
                    ClearQueueState(guid, "Leave dungeon member");
                break;
        }
    }

    void LFGMgr::LeaveSoloLfg(uint64 guid, uint32 queueId, bool disconnected)
    {
        SF_LOG_DEBUG("lfg.leave", "Player: %u left queue.", GUID_LOPART(guid));

        LfgState state = GetState(guid);
        switch (state)
        {
            case LFG_STATE_QUEUED:
            {
                LFGQueue& queue = GetQueue(queueId);
                queue.RemoveFromQueue(guid);
                SendLfgUpdateStatus(guid, LfgUpdateData(LFG_UPDATETYPE_REMOVED_FROM_QUEUE), false);
                ClearQueueState(guid, "Leave queued solo player");
                break;
            }
            case LFG_STATE_PROPOSAL:
            {
                // Remove from Proposals
                LfgProposalContainer::iterator it = ProposalsStore.begin();
                while (it != ProposalsStore.end())
                {
                    LfgProposalPlayerContainer::iterator itPlayer = it->second.players.find(guid);
                    if (itPlayer != it->second.players.end())
                    {
                        // Mark the player/leader of group who left as didn't accept the proposal
                        itPlayer->second.accept = LFG_ANSWER_DENY;
                        break;
                    }
                    ++it;
                }

                // Remove from queue - if proposal is found, RemoveProposal will call RemoveFromQueue
                if (it != ProposalsStore.end())
                    RemoveProposal(it, LFG_UPDATETYPE_PROPOSAL_DECLINED);
                break;
            }
            case LFG_STATE_NONE:
            case LFG_STATE_RAIDBROWSER:
                break;
            case LFG_STATE_DUNGEON:
            case LFG_STATE_FINISHED_DUNGEON:
            case LFG_STATE_BOOT:
            {
                ClearQueueState(guid, "Leave solo dungeon player");
                break;
            }
        }
    }

    /**
       Update the Role check info with the player selected role.

       @param[in]     grp Group guid to update rolecheck
       @param[in]     guid Player guid (0 = rolecheck failed)
       @param[in]     roles Player selected roles
    */
    void LFGMgr::UpdateRoleCheck(uint64 gguid, uint64 guid /* = 0 */, uint8 roles /* = PLAYER_ROLE_NONE */)
    {
        if (!gguid)
            return;

        LfgRolesMap check_roles;
        LfgRoleCheckContainer::iterator itRoleCheck = RoleChecksStore.find(gguid);
        if (itRoleCheck == RoleChecksStore.end())
            return;

        LfgRoleCheck& roleCheck = itRoleCheck->second;
        bool sendRoleChosen = roleCheck.state != LFG_ROLECHECK_DEFAULT && guid;

        if (!guid)
            roleCheck.state = LFG_ROLECHECK_ABORTED;
        else if (!IsValidPlayerRoles(roles))                          // Player selected no role or an invalid role mask.
            roleCheck.state = LFG_ROLECHECK_NO_ROLE;
        else
        {
            roleCheck.roles[guid] = roles;

            // Check if all players have selected a role
            LfgRolesMap::const_iterator itRoles = roleCheck.roles.begin();
            while (itRoles != roleCheck.roles.end() && itRoles->second != PLAYER_ROLE_NONE)
                ++itRoles;

            if (itRoles == roleCheck.roles.end())
            {
                // use temporal var to check roles, CheckGroupRoles modifies the roles
                check_roles = roleCheck.roles;
                bool scenario = false;
                bool flexibleRaid = false;
                for (LfgDungeonSet::const_iterator it = roleCheck.dungeons.begin(); it != roleCheck.dungeons.end(); ++it)
                {
                    LFGDungeonData const* dungeon = GetLFGDungeon(*it);
                    if (!dungeon)
                        continue;

                    if (IsScenarioDungeon(*dungeon))
                    {
                        scenario = true;
                        break;
                    }

                    if (IsFlexibleRaidData(*dungeon))
                        flexibleRaid = true;
                }

                bool rolesOk = false;
                if (scenario)
                    rolesOk = CheckDpsOnlyRoles(check_roles, uint8(check_roles.size()));
                else if (flexibleRaid)
                    rolesOk = CheckFlexibleRaidRoles(check_roles, MAXRAIDSIZE);
                else
                    rolesOk = CheckGroupRoles(check_roles);

                roleCheck.state = rolesOk ? LFG_ROLECHECK_FINISHED : LFG_ROLECHECK_WRONG_ROLES;
            }
        }

        LfgDungeonSet dungeons;
        if (roleCheck.rDungeonId)
            dungeons.insert(roleCheck.rDungeonId);
        else
            dungeons = roleCheck.dungeons;

        LfgJoinResult joinResult = LFG_JOIN_FAILED;
        switch (roleCheck.state)
        {
            case LFG_ROLECHECK_MISSING_ROLE:
            case LFG_ROLECHECK_WRONG_ROLES:
            case LFG_ROLECHECK_ABORTED:
            case LFG_ROLECHECK_NO_ROLE:
                joinResult = LFG_JOIN_ROLE_CHECK_FAILED;
                break;
            default:
                break;
        }

        LfgJoinResultData joinData = LfgJoinResultData(joinResult, roleCheck.state);
        for (LfgRolesMap::const_iterator it = roleCheck.roles.begin(); it != roleCheck.roles.end(); ++it)
        {
            uint64 pguid = it->first;

            if (sendRoleChosen)
                SendLfgRoleChosen(pguid, guid, roles);

            SendLfgRoleCheckUpdate(pguid, roleCheck);
            switch (roleCheck.state)
            {
                case LFG_ROLECHECK_INITIALITING:
                    continue;
                case LFG_ROLECHECK_FINISHED:
                    SetState(pguid, LFG_STATE_QUEUED);
                    SetRoles(pguid, it->second);
                    SendLfgUpdateStatus(pguid, LfgUpdateData(LFG_UPDATETYPE_ADDED_TO_QUEUE, dungeons, GetComment(pguid)), true);
                    break;
                default:
                    if (roleCheck.leader == pguid)
                        SendLfgJoinResult(pguid, joinData);
                    SendLfgUpdateStatus(pguid, LfgUpdateData(LFG_UPDATETYPE_ROLECHECK_FAILED), true);
                    RestoreOrClearState(pguid, "Rolecheck Failed");
                    break;
            }
        }

        if (roleCheck.state == LFG_ROLECHECK_FINISHED)
        {
            SetState(gguid, LFG_STATE_QUEUED);
            LFGQueue& queue = GetQueue(gguid);
            queue.AddQueueData(gguid, time_t(time(NULL)), roleCheck.dungeons, roleCheck.roles);
            RoleChecksStore.erase(itRoleCheck);
        }
        else if (roleCheck.state != LFG_ROLECHECK_INITIALITING)
        {
            RestoreOrClearState(gguid, "Rolecheck Failed");
            RoleChecksStore.erase(itRoleCheck);
        }
    }

    /**
       Given a list of dungeons remove the dungeons players have restrictions.

       @param[in, out] dungeons Dungeons to check restrictions
       @param[in]     players Set of players to check their dungeon restrictions
       @param[out]    lockMap Map of players Lock status info of given dungeons (Empty if dungeons is not empty)
    */
    void LFGMgr::GetCompatibleDungeons(LfgDungeonSet& dungeons, LfgGuidSet const& players, LfgLockPartyMap& lockMap, bool isContinue)
    {
        lockMap.clear();
        std::map<uint32, uint32> lockedDungeons;
        for (LfgGuidSet::const_iterator it = players.begin(); it != players.end() && !dungeons.empty(); ++it)
        {
            uint64 guid = (*it);
            LfgLockMap const& cachedLockMap = GetLockedDungeons(guid);
            Player* player = ObjectAccessor::FindPlayer(guid);
            for (LfgLockMap::const_iterator it2 = cachedLockMap.begin(); it2 != cachedLockMap.end() && !dungeons.empty(); ++it2)
            {
                uint32 dungeonId = (it2->first & 0x00FFFFFF); // Compare dungeon ids
                LfgDungeonSet::iterator itDungeon = dungeons.find(dungeonId);
                if (itDungeon != dungeons.end())
                {
                    bool eraseDungeon = true;
                    // Don't remove the dungeon if team members are trying to continue a locked instance
                    if (it2->second.lockStatus == LFG_LOCKSTATUS_RAID_LOCKED && isContinue)
                    {
                        LFGDungeonData const* dungeon = GetLFGDungeon(dungeonId);
                        ASSERT(dungeon);
                        ASSERT(player);
                        if (InstancePlayerBind* playerBind = player->GetBoundInstance(dungeon->map, DifficultyID(dungeon->difficulty)))
                        {
                            if (InstanceSave* playerSave = playerBind->save)
                            {
                                uint32 dungeonInstanceId = playerSave->GetInstanceId();
                                auto itLockedDungeon = lockedDungeons.find(dungeonId);
                                if (itLockedDungeon == lockedDungeons.end() || itLockedDungeon->second == dungeonInstanceId)
                                    eraseDungeon = false;
                                lockedDungeons[dungeonId] = dungeonInstanceId;
                            }
                        }
                    }

                    if (eraseDungeon)
                        dungeons.erase(itDungeon);

                    lockMap[guid][dungeonId] = it2->second;
                }
            }
        }
        if (!dungeons.empty())
            lockMap.clear();
    }

    /**
       Check if a group can be formed with the given group roles

       @param[in]     groles Map of roles to check
       @return True if roles are compatible
    */
    bool LFGMgr::CheckGroupRoles(LfgRolesMap& groles)
    {
        if (groles.empty() || groles.size() > MAXGROUPSIZE)
            return false;

        std::vector<LfgRoleAssignment> assignments;
        assignments.reserve(groles.size());

        for (LfgRolesMap::iterator it = groles.begin(); it != groles.end(); ++it)
        {
            uint8 roles = it->second & LFG_COMBAT_ROLE_MASK;
            if (!roles)
                return false;

            LfgRoleAssignment assignment;
            assignment.guid = it->first;
            assignment.leader = it->second & PLAYER_ROLE_LEADER;
            assignment.availableRoles = roles;
            assignment.assignedRole = PLAYER_ROLE_NONE;
            assignments.push_back(assignment);
        }

        std::sort(assignments.begin(), assignments.end(), [](LfgRoleAssignment const& left, LfgRoleAssignment const& right)
        {
            uint8 leftCount = CountAvailableRoles(left.availableRoles);
            uint8 rightCount = CountAvailableRoles(right.availableRoles);
            if (leftCount != rightCount)
                return leftCount < rightCount;

            return left.guid < right.guid;
        });

        if (!TryAssignLfgRoles(assignments, 0, 0, 0, 0))
            return false;

        for (LfgRoleAssignment const& assignment : assignments)
            groles[assignment.guid] = assignment.assignedRole;

        return true;
    }

    bool LFGMgr::CheckDpsOnlyRoles(LfgRolesMap& groles, uint8 neededDamage)
    {
        if (groles.empty() || groles.size() > neededDamage)
            return false;

        for (LfgRolesMap::iterator it = groles.begin(); it != groles.end(); ++it)
            it->second = PLAYER_ROLE_DAMAGE | (it->second & PLAYER_ROLE_LEADER);

        return true;
    }

    bool LFGMgr::CheckFlexibleRaidRoles(LfgRolesMap& groles, uint8 maxPlayers)
    {
        if (groles.empty() || !maxPlayers || groles.size() > maxPlayers)
            return false;

        for (LfgRolesMap::iterator it = groles.begin(); it != groles.end(); ++it)
        {
            uint8 leader = it->second & PLAYER_ROLE_LEADER;
            uint8 roles = it->second & LFG_COMBAT_ROLE_MASK;
            if (!roles)
                return false;

            if (roles & PLAYER_ROLE_TANK)
                it->second = PLAYER_ROLE_TANK | leader;
            else if (roles & PLAYER_ROLE_HEALER)
                it->second = PLAYER_ROLE_HEALER | leader;
            else
                it->second = PLAYER_ROLE_DAMAGE | leader;
        }

        return true;
    }

    /**
       Makes a new group given a proposal
       @param[in]     proposal Proposal to get info from
    */
    bool LFGMgr::MakeNewGroup(LfgProposal const& proposal)
    {
        LfgGuidList players;
        LfgGuidList playersToTeleport;
        LfgGuidSet expectedPlayers;

        for (LfgProposalPlayerContainer::const_iterator it = proposal.players.begin(); it != proposal.players.end(); ++it)
        {
            uint64 guid = it->first;
            expectedPlayers.insert(guid);
            if (guid == proposal.leader)
                players.push_front(guid);
            else
                players.push_back(guid);

            if (proposal.isNew || proposal.group || GetGroup(guid) != proposal.group)
                playersToTeleport.push_back(guid);
        }

        // Set the dungeon difficulty
        LFGDungeonData const* dungeon = GetLFGDungeon(proposal.dungeonId);
        if (!dungeon)
        {
            SF_LOG_ERROR("lfg.proposal.group.make", "Proposal %u cannot create group for missing dungeon %u.",
                proposal.id, proposal.dungeonId);
            return false;
        }

        Group* grp = proposal.group ? sGroupMgr->GetGroupByGUID(GUID_LOPART(proposal.group)) : NULL;
        bool const groupAlreadyExisted = grp != NULL;
        for (LfgGuidList::const_iterator it = players.begin(); it != players.end(); ++it)
        {
            uint64 pguid = (*it);
            Player* player = ObjectAccessor::FindPlayer(pguid);
            if (!player)
            {
                SF_LOG_DEBUG("lfg.proposal.group.make", "Proposal %u cannot create group, player %u is offline.",
                    proposal.id, GUID_LOPART(pguid));
                return false;
            }

            Group* group = player->GetGroup();
            if (group && group != grp)
                group->RemoveMember(player->GetGUID());

            if (!grp)
            {
                grp = new Group();
                grp->ConvertToLFG();
                if (!grp->Create(player))
                {
                    delete grp;
                    SF_LOG_ERROR("lfg.proposal.group.make", "Proposal %u failed to create LFG group with leader %u.",
                        proposal.id, GUID_LOPART(pguid));
                    return false;
                }

                uint64 gguid = grp->GetGUID();
                SetActiveQueueId(gguid, GetActiveQueueId(proposal.leader));
                SetState(gguid, LFG_STATE_PROPOSAL);
                sGroupMgr->AddGroup(grp);
            }
            else if (group != grp)
            {
                if (!grp->AddMember(player))
                {
                    SF_LOG_ERROR("lfg.proposal.group.make", "Proposal %u failed to add player %u to group %u.",
                        proposal.id, GUID_LOPART(pguid), GUID_LOPART(grp->GetGUID()));
                    return false;
                }
            }

            grp->SetLfgRoles(pguid, proposal.players.find(pguid)->second.role);

            // Add the cooldown spell if queued for a random dungeon
            if (dungeon->type == LFG_TYPE_RANDOM)
                player->CastSpell(player, LFG_SPELL_DUNGEON_COOLDOWN, false);
        }

        if (!grp)
        {
            SF_LOG_ERROR("lfg.proposal.group.make", "Proposal %u did not create or find a group.", proposal.id);
            return false;
        }

        for (LfgGuidSet::const_iterator it = expectedPlayers.begin(); it != expectedPlayers.end(); ++it)
        {
            if (!grp->IsMember(*it))
            {
                SF_LOG_ERROR("lfg.proposal.group.make", "Proposal %u created incomplete group %u, missing player %u.",
                    proposal.id, GUID_LOPART(grp->GetGUID()), GUID_LOPART(*it));
                return false;
            }
        }

        bool const isRaidDungeon = IsRaidDungeon(*dungeon);
        if (isRaidDungeon && !grp->isRaidGroup())
            grp->ConvertToRaid();

        DifficultyID const difficulty = DifficultyID(dungeon->difficulty);
        if (isRaidDungeon)
            grp->SetRaidDifficulty(difficulty);
        else
            grp->SetDungeonDifficulty(difficulty);

        uint64 gguid = grp->GetGUID();
        SetActiveQueueId(gguid, GetActiveQueueId(proposal.leader));
        SetDungeon(gguid, dungeon->Entry());
        SetState(gguid, LFG_STATE_DUNGEON);

        uint64 leader = proposal.leader && grp->IsMember(proposal.leader) ? proposal.leader : grp->GetLeaderGUID();
        if (leader && grp->GetLeaderGUID() != leader)
            grp->ChangeLeader(leader);

        SetLeader(gguid, leader);

        for (LfgGuidList::const_iterator it = players.begin(); it != players.end(); ++it)
            if (grp->IsMember(*it))
                SetupGroupMember(*it, gguid);

        _SaveToDB(gguid, grp->GetDbStoreId());

        if (!BindLfgGroupToDungeonInstance(grp, *dungeon))
        {
            SF_LOG_ERROR("lfg.proposal.group.make", "Proposal %u failed to bind group %u to dungeon %u map %u.",
                proposal.id, GUID_LOPART(gguid), dungeon->id, uint32(dungeon->map));
            return false;
        }

        bool const forceChangeInstance = !proposal.isNew && groupAlreadyExisted;

        // Teleport Player
        for (LfgGuidList::const_iterator it = playersToTeleport.begin(); it != playersToTeleport.end(); ++it)
            if (Player* player = ObjectAccessor::FindPlayer(*it))
                if (player->GetMapId() != uint32(dungeon->map) || forceChangeInstance)
                    TeleportPlayer(player, false, false, forceChangeInstance);

        // Update group info
        grp->SendUpdate();
        return true;
    }

    uint32 LFGMgr::AddProposal(LfgProposal& proposal)
    {
        proposal.id = ++m_lfgProposalId;
        ProposalsStore[m_lfgProposalId] = proposal;
        return m_lfgProposalId;
    }

    /**
       Update Proposal info with player answer

       @param[in]     proposalId Proposal id to be updated
       @param[in]     guid Player guid to update answer
       @param[in]     accept Player answer
    */
    void LFGMgr::UpdateProposal(uint32 proposalId, uint64 guid, bool accept)
    {
        // Check if the proposal exists
        LfgProposalContainer::iterator itProposal = ProposalsStore.find(proposalId);
        if (itProposal == ProposalsStore.end())
            return;

        LfgProposal& proposal = itProposal->second;

        // Check if proposal have the current player
        LfgProposalPlayerContainer::iterator itProposalPlayer = proposal.players.find(guid);
        if (itProposalPlayer == proposal.players.end())
            return;

        LfgProposalPlayer& player = itProposalPlayer->second;
        if (proposal.state != LFG_PROPOSAL_INITIATING || player.accept != LFG_ANSWER_PENDING)
        {
            SF_LOG_DEBUG("lfg.proposal.update", "Ignoring stale proposal response. Player %u, Proposal %u, Selection: %u, State: %u, Previous: %d",
                GUID_LOPART(guid), proposalId, accept, proposal.state, player.accept);
            return;
        }

        if (player.group && GetGroup(guid) != player.group)
        {
            SF_LOG_DEBUG("lfg.proposal.update", "Player %u is no longer in proposal group %u. Removing stale proposal %u.",
                GUID_LOPART(guid), GUID_LOPART(player.group), proposalId);
            player.accept = LFG_ANSWER_DENY;
            RemoveProposal(itProposal, LFG_UPDATETYPE_PROPOSAL_DECLINED);
            return;
        }

        for (LfgGuidList::const_iterator itQueue = proposal.queues.begin(); itQueue != proposal.queues.end(); ++itQueue)
        {
            if (GetQueue(*itQueue).HasQueueData(*itQueue))
                continue;

            SF_LOG_DEBUG("lfg.proposal.update", "Proposal %u has stale queue owner %u. Removing proposal.",
                proposalId, GUID_LOPART(*itQueue));
            player.accept = LFG_ANSWER_DENY;
            RemoveProposal(itProposal, LFG_UPDATETYPE_PROPOSAL_DECLINED);
            return;
        }

        player.accept = LfgAnswer(accept);

        SF_LOG_DEBUG("lfg.proposal.update", "Player %u, Proposal %u, Selection: %u", GUID_LOPART(guid), proposalId, accept);
        if (!accept)
        {
            RemoveProposal(itProposal, LFG_UPDATETYPE_PROPOSAL_DECLINED);
            return;
        }

        // check if all have answered and reorder players (leader first)
        bool allAnswered = true;
        for (LfgProposalPlayerContainer::const_iterator itPlayers = proposal.players.begin(); itPlayers != proposal.players.end(); ++itPlayers)
            if (itPlayers->second.accept != LFG_ANSWER_AGREE)   // No answer (-1) or not accepted (0)
                allAnswered = false;

        if (!allAnswered)
        {
            for (LfgProposalPlayerContainer::const_iterator it = proposal.players.begin(); it != proposal.players.end(); ++it)
                SendLfgUpdateProposal(it->first, proposal);

            return;
        }

        if (!GetLFGDungeon(proposal.dungeonId))
        {
            SF_LOG_ERROR("lfg.proposal.update", "Proposal %u accepted but dungeon %u no longer exists.",
                proposalId, proposal.dungeonId);
            RemoveProposal(itProposal, LFG_UPDATETYPE_PROPOSAL_FAILED);
            return;
        }

        for (LfgProposalPlayerContainer::iterator it = proposal.players.begin(); it != proposal.players.end(); ++it)
        {
            uint64 pguid = it->first;
            if (!ObjectAccessor::FindPlayer(pguid))
            {
                SF_LOG_DEBUG("lfg.proposal.update", "Proposal %u accepted but player %u is offline.",
                    proposalId, GUID_LOPART(pguid));
                it->second.accept = LFG_ANSWER_DENY;
                RemoveProposal(itProposal, LFG_UPDATETYPE_GROUP_MEMBER_OFFLINE);
                return;
            }

            if (it->second.group && GetGroup(pguid) != it->second.group)
            {
                SF_LOG_DEBUG("lfg.proposal.update", "Proposal %u accepted but player %u left proposal group %u.",
                    proposalId, GUID_LOPART(pguid), GUID_LOPART(it->second.group));
                it->second.accept = LFG_ANSWER_DENY;
                RemoveProposal(itProposal, LFG_UPDATETYPE_PROPOSAL_DECLINED);
                return;
            }
        }

        bool sendUpdate = proposal.state != LFG_PROPOSAL_SUCCESS;
        proposal.state = LFG_PROPOSAL_SUCCESS;
        time_t joinTime = time(NULL);

        uint64 queueOwner = proposal.queues.empty() ? guid : proposal.queues.front();
        LFGQueue& queue = GetQueue(queueOwner);
        LfgUpdateData groupFoundData = LfgUpdateData(LFG_UPDATETYPE_GROUP_FOUND);
        LfgUpdateData removedFromQueueData = LfgUpdateData(LFG_UPDATETYPE_REMOVED_FROM_QUEUE);
        for (LfgProposalPlayerContainer::const_iterator it = proposal.players.begin(); it != proposal.players.end(); ++it)
        {
            uint64 pguid = it->first;
            uint64 gguid = it->second.group;
            LfgDungeonSet const& selectedDungeons = GetSelectedDungeons(pguid);
            uint32 dungeonId = selectedDungeons.empty() ? proposal.dungeonId : (*selectedDungeons.begin());
            int32 waitTime = -1;
            uint64 queuedGuid = gguid ? gguid : pguid;
            time_t queueJoinTime = queue.GetJoinTime(queuedGuid);
            if (!queueJoinTime && queuedGuid != pguid)
                queueJoinTime = queue.GetJoinTime(pguid);

            if (queueJoinTime)
                waitTime = int32(joinTime - queueJoinTime);
            else
                SF_LOG_DEBUG("lfg.proposal.update", "Proposal %u missing queue join time for player %u queue owner %u",
                    proposalId, GUID_LOPART(pguid), GUID_LOPART(queuedGuid));

            if (waitTime >= 0 && dungeonId)
            {
                // Update timers
                uint8 role = GetRoles(pguid);
                role &= ~PLAYER_ROLE_LEADER;
                switch (role)
                {
                    case PLAYER_ROLE_DAMAGE:
                        queue.UpdateWaitTimeDps(waitTime, dungeonId);
                        break;
                    case PLAYER_ROLE_HEALER:
                        queue.UpdateWaitTimeHealer(waitTime, dungeonId);
                        break;
                    case PLAYER_ROLE_TANK:
                        queue.UpdateWaitTimeTank(waitTime, dungeonId);
                        break;
                    default:
                        queue.UpdateWaitTimeAvg(waitTime, dungeonId);
                        break;
                }
            }
        }

        if (sendUpdate)
            for (LfgProposalPlayerContainer::const_iterator it = proposal.players.begin(); it != proposal.players.end(); ++it)
                SendLfgUpdateProposal(it->first, proposal);

        if (!MakeNewGroup(proposal))
        {
            for (LfgProposalPlayerContainer::iterator it = proposal.players.begin(); it != proposal.players.end(); ++it)
                it->second.accept = LFG_ANSWER_DENY;

            RemoveProposal(itProposal, LFG_UPDATETYPE_PROPOSAL_FAILED);
            return;
        }

        for (LfgProposalPlayerContainer::const_iterator it = proposal.players.begin(); it != proposal.players.end(); ++it)
        {
            uint64 pguid = it->first;
            uint64 gguid = it->second.group;

            SendLfgUpdateStatus(pguid, groupFoundData, gguid != 0);
            SendLfgUpdateStatus(pguid, removedFromQueueData, true);
            SendLfgUpdateStatus(pguid, removedFromQueueData, false);
            SetState(pguid, LFG_STATE_DUNGEON);
        }

        // Remove players/groups from Queue
        for (LfgGuidList::const_iterator it = proposal.queues.begin(); it != proposal.queues.end(); ++it)
            GetQueue(*it).RemoveFromQueue(*it);

        ProposalsStore.erase(itProposal);
    }

    /**
       Remove a proposal from the pool, remove the group that didn't accept (if needed) and readd the other members to the queue

       @param[in]     itProposal Iterator to the proposal to remove
       @param[in]     type Type of removal (LFG_UPDATETYPE_PROPOSAL_FAILED, LFG_UPDATETYPE_PROPOSAL_DECLINED)
    */
    void LFGMgr::RemoveProposal(LfgProposalContainer::iterator itProposal, LfgUpdateType type)
    {
        LfgProposal& proposal = itProposal->second;
        proposal.state = LFG_PROPOSAL_FAILED;

        SF_LOG_DEBUG("lfg.proposal.remove", "Proposal %u, state FAILED, UpdateType %u", itProposal->first, type);
        if (proposal.players.empty())
        {
            ProposalsStore.erase(itProposal);
            return;
        }

        // Mark all people that didn't answered as no accept
        if (type == LFG_UPDATETYPE_PROPOSAL_FAILED)
            for (LfgProposalPlayerContainer::iterator it = proposal.players.begin(); it != proposal.players.end(); ++it)
                if (it->second.accept == LFG_ANSWER_PENDING)
                    it->second.accept = LFG_ANSWER_DENY;

        // Mark players/groups to be removed
        LfgGuidSet toRemove;
        for (LfgProposalPlayerContainer::iterator it = proposal.players.begin(); it != proposal.players.end(); ++it)
        {
            if (it->second.accept == LFG_ANSWER_AGREE)
                continue;

            uint64 guid = it->second.group ? it->second.group : it->first;
            // Player didn't accept or still pending when no secs left
            if (it->second.accept == LFG_ANSWER_DENY || type == LFG_UPDATETYPE_PROPOSAL_FAILED)
            {
                it->second.accept = LFG_ANSWER_DENY;
                toRemove.insert(guid);
            }
        }

        LfgGuidSet missingQueueData;
        for (LfgGuidList::const_iterator it = proposal.queues.begin(); it != proposal.queues.end(); ++it)
        {
            if (!GetQueue(*it).HasQueueData(*it))
            {
                missingQueueData.insert(*it);
                toRemove.insert(*it);
                SF_LOG_DEBUG("lfg.proposal.remove", "Proposal %u missing queue data for %u while removing proposal",
                    proposal.id, GUID_LOPART(*it));
            }
        }

        // Notify players
        for (LfgProposalPlayerContainer::const_iterator it = proposal.players.begin(); it != proposal.players.end(); ++it)
        {
            uint64 guid = it->first;
            uint64 gguid = it->second.group ? it->second.group : guid;
            bool canRequeue = toRemove.find(gguid) == toRemove.end() && GetQueue(gguid).HasQueueData(gguid);

            SendLfgUpdateProposal(guid, proposal);

            if (!canRequeue)                                    // Didn't accept, stale queue data, or same group as someone that didn't accept
            {
                LfgUpdateData updateData;
                if (it->second.accept == LFG_ANSWER_DENY)
                {
                    updateData.updateType = type;
                    SF_LOG_DEBUG("lfg.proposal.remove", "%u didn't accept. Removing from queue and compatible cache", GUID_LOPART(guid));
                }
                else if (missingQueueData.find(gguid) != missingQueueData.end())
                {
                    updateData.updateType = LFG_UPDATETYPE_REMOVED_FROM_QUEUE;
                    SF_LOG_DEBUG("lfg.proposal.remove", "%u no longer has queue data. Removing stale proposal state", GUID_LOPART(guid));
                }
                else
                {
                    updateData.updateType = LFG_UPDATETYPE_REMOVED_FROM_QUEUE;
                    SF_LOG_DEBUG("lfg.proposal.remove", "%u cannot be requeued. Removing from queue and compatible cache", GUID_LOPART(guid));
                }

                RestoreOrClearState(guid, "Proposal Fail (didn't accept or in group with someone that didn't accept)");
                if (gguid != guid)
                {
                    RestoreOrClearState(it->second.group, "Proposal Fail (someone in group didn't accept)");
                    SendLfgUpdateStatus(guid, updateData, true);
                }
                else
                    SendLfgUpdateStatus(guid, updateData, false);
            }
            else
            {
                SF_LOG_DEBUG("lfg.proposal.remove", "Readding %u to queue.", GUID_LOPART(guid));
                SetState(guid, LFG_STATE_QUEUED);
                if (gguid != guid)
                {
                    SetState(gguid, LFG_STATE_QUEUED);
                    SendLfgUpdateStatus(guid, LfgUpdateData(LFG_UPDATETYPE_ADDED_TO_QUEUE, GetSelectedDungeons(guid), GetComment(guid)), true);
                }
                else
                    SendLfgUpdateStatus(guid, LfgUpdateData(LFG_UPDATETYPE_ADDED_TO_QUEUE, GetSelectedDungeons(guid), GetComment(guid)), false);
            }
        }

        // Remove players/groups from queue
        for (LfgGuidSet::const_iterator it = toRemove.begin(); it != toRemove.end(); ++it)
        {
            uint64 guid = *it;
            GetQueue(guid).RemoveFromQueue(guid);
            proposal.queues.remove(guid);
        }

        // Readd to queue
        for (LfgGuidList::const_iterator it = proposal.queues.begin(); it != proposal.queues.end(); ++it)
        {
            uint64 guid = *it;
            LFGQueue& queue = GetQueue(guid);
            if (queue.HasQueueData(guid))
                queue.AddToQueue(guid, true);
        }

        ProposalsStore.erase(itProposal);
    }

    /**
       Initialize a boot kick vote

       @param[in]     gguid Group the vote kicks belongs to
       @param[in]     kicker Kicker guid
       @param[in]     victim Victim guid
       @param[in]     reason Kick reason
    */
    void LFGMgr::InitBoot(uint64 gguid, uint64 kicker, uint64 victim, std::string const& reason)
    {
        LfgGroupDataContainer::const_iterator groupData = GroupsStore.find(gguid);
        if (groupData == GroupsStore.end())
            return;

        LfgGuidSet const& players = groupData->second.GetPlayers();
        if (players.find(kicker) == players.end() || players.find(victim) == players.end())
        {
            SF_LOG_DEBUG("lfg.boot", "Group %u rejected boot init for invalid kicker %u or victim %u",
                GUID_LOPART(gguid), GUID_LOPART(kicker), GUID_LOPART(victim));
            return;
        }

        SetVoteKick(gguid, true);
        SetState(gguid, LFG_STATE_BOOT);

        LfgPlayerBoot& boot = BootsStore[gguid];
        boot.inProgress = true;
        boot.cancelTime = time_t(time(NULL)) + LFG_TIME_BOOT;
        boot.reason = reason;
        boot.victim = victim;
        boot.votes.clear();

        // Set votes
        for (LfgGuidSet::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            uint64 guid = (*itr);
            boot.votes[guid] = LFG_ANSWER_PENDING;
            SetState(guid, LFG_STATE_BOOT);
        }

        LfgAnswerContainer::iterator victimVote = boot.votes.find(victim);
        if (victimVote != boot.votes.end())
            victimVote->second = LFG_ANSWER_DENY;              // Victim auto vote NO

        LfgAnswerContainer::iterator kickerVote = boot.votes.find(kicker);
        if (kickerVote != boot.votes.end())
            kickerVote->second = LFG_ANSWER_AGREE;             // Kicker auto vote YES

        // Notify players
        for (LfgGuidSet::const_iterator it = players.begin(); it != players.end(); ++it)
            SendLfgBootProposalUpdate(*it, boot);
    }

    /**
       Update Boot info with player answer

       @param[in]     guid Player who has answered
       @param[in]     player answer
    */
    void LFGMgr::UpdateBoot(uint64 guid, bool accept)
    {
        uint64 gguid = GetGroup(guid);
        if (!gguid)
            return;

        LfgPlayerBootContainer::iterator itBoot = BootsStore.find(gguid);
        if (itBoot == BootsStore.end())
            return;

        LfgGroupDataContainer::const_iterator groupData = GroupsStore.find(gguid);
        if (groupData == GroupsStore.end())
        {
            BootsStore.erase(itBoot);
            return;
        }

        LfgPlayerBoot& boot = itBoot->second;

        LfgAnswerContainer::iterator itVote = boot.votes.find(guid);
        if (itVote == boot.votes.end())
            return;

        if (itVote->second != LFG_ANSWER_PENDING)    // Cheat check: Player can't vote twice
            return;

        itVote->second = LfgAnswer(accept);

        uint8 votesNum = 0;
        uint8 agreeNum = 0;
        for (LfgAnswerContainer::const_iterator itVotes = boot.votes.begin(); itVotes != boot.votes.end(); ++itVotes)
        {
            if (itVotes->second != LFG_ANSWER_PENDING)
            {
                ++votesNum;
                if (itVotes->second == LFG_ANSWER_AGREE)
                    ++agreeNum;
            }
        }

        // if we don't have enough votes (agree or deny) do nothing
        if (agreeNum < LFG_GROUP_KICK_VOTES_NEEDED && (votesNum - agreeNum) < LFG_GROUP_KICK_VOTES_NEEDED)
            return;

        // Send update info to all players
        boot.inProgress = false;
        for (LfgAnswerContainer::const_iterator itVotes = boot.votes.begin(); itVotes != boot.votes.end(); ++itVotes)
        {
            uint64 pguid = itVotes->first;
            if (pguid != boot.victim)
                SendLfgBootProposalUpdate(pguid, boot);
            SetState(pguid, LFG_STATE_DUNGEON);
        }

        SetState(gguid, LFG_STATE_DUNGEON);
        SetVoteKick(gguid, false);
        if (agreeNum == LFG_GROUP_KICK_VOTES_NEEDED)           // Vote passed - Kick player
        {
            if (Group* group = sGroupMgr->GetGroupByGUID(GUID_LOPART(gguid)))
                Player::RemoveFromGroup(group, boot.victim, GROUP_REMOVEMETHOD_KICK_LFG);
            DecreaseKicksLeft(gguid);
        }
        BootsStore.erase(itBoot);
    }

    /**
       Teleports the player in or out the dungeon

       @param[in]     player Player to teleport
       @param[in]     out Teleport out (true) or in (false)
       @param[in]     fromOpcode Function called from opcode handlers? (Default false)
    */
    void LFGMgr::TeleportPlayer(Player* player, bool out, bool fromOpcode /*= false*/, bool forceChangeInstance /*= false*/)
    {
        LFGDungeonData const* dungeon = NULL;
        Group* group = player->GetGroup();

        if (group && group->isLFGGroup())
            dungeon = GetLFGDungeon(GetDungeon(group->GetGUID()));

        if (!dungeon)
        {
            SF_LOG_DEBUG("lfg.teleport", "Player %s not in group/lfggroup or dungeon not found!",
                player->GetName().c_str());
            player->GetSession()->SendLfgTeleportError(uint8(LFG_TELEPORTERROR_INVALID_LOCATION));
            return;
        }

        if (player->IsBeingTeleported())
        {
            SF_LOG_DEBUG("lfg.teleport", "Player %s already has a pending teleport, skipping LFG teleport %s.",
                player->GetName().c_str(), out ? "out" : "in");
            return;
        }

        if (out)
        {
            SF_LOG_DEBUG("lfg.teleport", "Player %s is being teleported out. Current Map %u - Expected Map %u",
                player->GetName().c_str(), player->GetMapId(), uint32(dungeon->map));
            if (player->GetMapId() == uint32(dungeon->map))
            {
                uint64 const guid = player->GetGUID();
                LfgPlayerData& playerData = PlayersStore[guid];
                LfgReturnLocation const& returnLocation = playerData.GetReturnLocation();
                if (returnLocation.IsSet && MapManager::IsValidMapCoord(returnLocation.MapId, returnLocation.X, returnLocation.Y, returnLocation.Z, returnLocation.O))
                {
                    if (player->TeleportTo(returnLocation.MapId, returnLocation.X, returnLocation.Y, returnLocation.Z, returnLocation.O))
                    {
                        playerData.ClearReturnLocation();
                        return;
                    }

                    SF_LOG_DEBUG("lfg.teleport", "Player %s failed LFG return teleport to map %u (x: %f, y: %f, z: %f), falling back to battleground entry point",
                        player->GetName().c_str(), returnLocation.MapId, returnLocation.X, returnLocation.Y, returnLocation.Z);
                }

                playerData.ClearReturnLocation();
                player->TeleportToBGEntryPoint();
            }

            return;
        }

        LfgTeleportError error = LFG_TELEPORTERROR_OK;

        if (!player->IsAlive())
            error = LFG_TELEPORTERROR_PLAYER_DEAD;
        else if (player->IsInCombat())
            error = LFG_TELEPORTERROR_IN_COMBAT;
        else if (player->IsFalling() || player->HasUnitState(UNIT_STATE_JUMPING))
            error = LFG_TELEPORTERROR_FALLING;
        else if (player->IsMirrorTimerActive(FATIGUE_TIMER))
            error = LFG_TELEPORTERROR_FATIGUE;
        else if (player->GetVehicle())
            error = LFG_TELEPORTERROR_IN_VEHICLE;
        else if (player->GetCharmGUID())
            error = LFG_TELEPORTERROR_CHARMING;
        else if (player->GetMapId() != uint32(dungeon->map) || forceChangeInstance)  // Do not teleport players in dungeon to the entrance
        {
            uint32 mapid = dungeon->map;
            float x = dungeon->x;
            float y = dungeon->y;
            float z = dungeon->z;
            float orientation = dungeon->o;

            if (!HasValidLfgTeleportLocation(*dungeon))
            {
                SF_LOG_ERROR("lfg.teleport", "Player %s cannot teleport to LFG dungeon %u (%s): invalid entrance map %u position %f %f %f %f",
                    player->GetName().c_str(), dungeon->id, dungeon->name.c_str(), uint32(dungeon->map), dungeon->x, dungeon->y, dungeon->z, dungeon->o);
                error = LFG_TELEPORTERROR_INVALID_LOCATION;
            }

            if (error == LFG_TELEPORTERROR_OK && !fromOpcode && !forceChangeInstance)
            {
                // Select a player inside to be teleported to
                for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
                {
                    Player* plrg = itr->GetSource();
                    if (plrg && plrg != player && plrg->GetMapId() == uint32(dungeon->map))
                    {
                        mapid = plrg->GetMapId();
                        x = plrg->GetPositionX();
                        y = plrg->GetPositionY();
                        z = plrg->GetPositionZ();
                        orientation = plrg->GetOrientation();
                        break;
                    }
                }
            }

            if (error == LFG_TELEPORTERROR_OK && !player->GetMap()->IsInstance())
            {
                if (MapManager::IsValidMapCoord(player->GetMapId(), player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation()))
                    PlayersStore[uint64(player->GetGUID())].SetReturnLocation(player->GetMapId(), player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation());

                player->SetBattlegroundEntryPoint();
            }

            if (error == LFG_TELEPORTERROR_OK && player->IsInFlight())
            {
                player->GetMotionMaster()->MovementExpired();
                player->CleanupAfterTaxiFlight();
            }

            if (error == LFG_TELEPORTERROR_OK)
            {
                player->SetForcedTeleportFar(forceChangeInstance);
                if (!player->TeleportTo(mapid, x, y, z, orientation))
                {
                    error = LFG_TELEPORTERROR_INVALID_LOCATION;
                    if (forceChangeInstance)
                        player->SetSemaphoreTeleportForcedFar(false);
                }
                player->SetForcedTeleportFar(false);
            }
        }
        else
            error = LFG_TELEPORTERROR_INVALID_LOCATION;

        if (error != LFG_TELEPORTERROR_OK)
            player->GetSession()->SendLfgTeleportError(uint8(error));

        SF_LOG_DEBUG("lfg.teleport", "Player %s is being teleported in to map %u "
            "(x: %f, y: %f, z: %f) Result: %u", player->GetName().c_str(), dungeon->map,
            dungeon->x, dungeon->y, dungeon->z, error);
    }

    void LFGMgr::TeleportDungeonGroupOut(Group* group)
    {
        if (!group || !group->isLFGGroup())
            return;

        uint64 const groupGuid = group->GetGUID();
        LfgState const groupState = GetState(groupGuid);
        if (groupState != LFG_STATE_DUNGEON && groupState != LFG_STATE_FINISHED_DUNGEON)
            return;

        LFGDungeonData const* dungeon = GetLFGDungeon(GetDungeon(groupGuid));
        if (!dungeon)
            return;

        for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
        {
            Player* member = itr->GetSource();
            if (!member || member->GetMapId() != uint32(dungeon->map))
                continue;

            TeleportPlayer(member, true);
        }
    }

    /**
       Finish a dungeon and give reward, if any.

       @param[in]     guid Group guid
       @param[in]     dungeonId Dungeonid
    */
    void LFGMgr::FinishDungeon(uint64 gguid, const uint32 dungeonId)
    {
        uint32 gDungeonId = GetDungeon(gguid);
        if (gDungeonId != dungeonId)
        {
            SF_LOG_DEBUG("lfg.dungeon.finish", "Group %u finished dungeon %u but queued for %u", GUID_LOPART(gguid), dungeonId, gDungeonId);
            return;
        }

        if (GetState(gguid) == LFG_STATE_FINISHED_DUNGEON) // Shouldn't happen. Do not reward multiple times
        {
            SF_LOG_DEBUG("lfg.dungeon.finish", "Group: %u already rewarded", GUID_LOPART(gguid));
            return;
        }

        SetState(gguid, LFG_STATE_FINISHED_DUNGEON);

        const LfgGuidSet& players = GetPlayers(gguid);
        for (LfgGuidSet::const_iterator it = players.begin(); it != players.end(); ++it)
        {
            uint64 guid = (*it);
            if (GetState(guid) == LFG_STATE_FINISHED_DUNGEON)
            {
                SF_LOG_DEBUG("lfg.dungeon.finish", "Group: %u, Player: %u already rewarded", GUID_LOPART(gguid), GUID_LOPART(guid));
                continue;
            }

            uint32 rDungeonId = 0;
            const LfgDungeonSet& dungeons = GetSelectedDungeons(guid);
            if (!dungeons.empty())
                rDungeonId = (*dungeons.begin());

            SetState(guid, LFG_STATE_FINISHED_DUNGEON);

            // Give rewards only if its a random dungeon
            LFGDungeonData const* dungeon = GetLFGDungeon(rDungeonId);

            if (!dungeon || (dungeon->type != LFG_TYPE_RANDOM && !dungeon->seasonal))
            {
                SF_LOG_DEBUG("lfg.dungeon.finish", "Group: %u, Player: %u dungeon %u is not random or seasonal", GUID_LOPART(gguid), GUID_LOPART(guid), rDungeonId);
                continue;
            }

            Player* player = ObjectAccessor::FindPlayer(guid);
            if (!player || !player->IsInWorld())
            {
                SF_LOG_DEBUG("lfg.dungeon.finish", "Group: %u, Player: %u not found in world", GUID_LOPART(gguid), GUID_LOPART(guid));
                continue;
            }

            LFGDungeonData const* dungeonDone = GetLFGDungeon(dungeonId);
            uint32 mapId = dungeonDone ? uint32(dungeonDone->map) : 0;

            if (player->GetMapId() != mapId)
            {
                SF_LOG_DEBUG("lfg.dungeon.finish", "Group: %u, Player: %u is in map %u and should be in %u to get reward", GUID_LOPART(gguid), GUID_LOPART(guid), player->GetMapId(), mapId);
                continue;
            }

            // Update achievements
            if (dungeon->difficulty == DIFFICULTY_HEROIC)
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_USE_LFD_TO_GROUP_WITH_PLAYERS, 1);

            LfgReward const* reward = GetRandomDungeonReward(rDungeonId, player->getLevel());
            if (!reward)
                continue;

            bool done = false;
            Quest const* quest = sObjectMgr->GetQuestTemplate(reward->firstQuest);
            if (!quest)
                continue;

            // if we can take the quest, means that we haven't done this kind of "run", IE: First Heroic Random of Day.
            if (player->CanRewardQuest(quest, false))
                player->RewardQuest(quest, 0, NULL, false);
            else
            {
                done = true;
                quest = sObjectMgr->GetQuestTemplate(reward->otherQuest);
                if (!quest)
                    continue;
                // we give reward without informing client (retail does this)
                player->RewardQuest(quest, 0, NULL, false);
            }

            // Give rewards
            SF_LOG_DEBUG("lfg.dungeon.finish", "Group: %u, Player: %u done dungeon %u, %s previously done.", GUID_LOPART(gguid), GUID_LOPART(guid), GetDungeon(gguid), done ? " " : " not");
            LfgPlayerRewardData data = LfgPlayerRewardData(dungeon->Entry(), GetDungeon(gguid, false), done, quest);
            player->GetSession()->SendLfgPlayerReward(data);
        }
    }

    // --------------------------------------------------------------------------//
    // Auxiliar Functions
    // --------------------------------------------------------------------------//

    /**
       Get the dungeon list that can be done given a random dungeon entry.

       @param[in]     randomdungeon Random dungeon id (if value = 0 will return all dungeons)
       @returns Set of dungeons that can be done.
    */
    LfgDungeonSet const& LFGMgr::GetDungeonsByRandom(uint32 randomdungeon)
    {
        LFGDungeonData const* dungeon = GetLFGDungeon(randomdungeon);
        uint32 group = dungeon ? dungeon->group : 0;
        return CachedDungeonMapStore[group];
    }

    /**
       Get the reward of a given random dungeon at a certain level

       @param[in]     dungeon dungeon id
       @param[in]     level Player level
       @returns Reward
    */
    LfgReward const* LFGMgr::GetRandomDungeonReward(uint32 dungeon, uint8 level)
    {
        LfgReward const* rew = NULL;
        LfgRewardContainerBounds bounds = RewardMapStore.equal_range(dungeon & 0x00FFFFFF);
        for (LfgRewardContainer::const_iterator itr = bounds.first; itr != bounds.second; ++itr)
        {
            rew = itr->second;
            // ordered properly at loading
            if (itr->second->maxLevel >= level)
                break;
        }

        return rew;
    }

    /**
       Given a Dungeon id returns the dungeon Type

       @param[in]     dungeon dungeon id
       @returns Dungeon type
    */
    LfgType LFGMgr::GetDungeonType(uint32 dungeonId)
    {
        LFGDungeonData const* dungeon = GetLFGDungeon(dungeonId);
        if (!dungeon)
            return LFG_TYPE_NONE;

        return LfgType(dungeon->type);
    }

    bool LFGMgr::IsRaidFinderDungeon(uint32 dungeonId)
    {
        LFGDungeonData const* dungeon = GetLFGDungeon(dungeonId);
        return dungeon && dungeon->difficulty == DIFFICULTY_25MAN_LFR && IsRaidDungeon(*dungeon);
    }

    bool LFGMgr::IsFlexibleRaidDungeon(uint32 dungeonId)
    {
        LFGDungeonData const* dungeon = GetLFGDungeon(dungeonId);
        return dungeon && IsFlexibleRaidData(*dungeon);
    }

    LfgState LFGMgr::GetState(uint64 guid)
    {
        LfgState state;
        if (IS_GROUP_GUID(guid))
        {
            state = GroupsStore[guid].GetState();
            SF_LOG_TRACE("lfg.data.group.state.get", "Group: %u, State: %u", GUID_LOPART(guid), state);
        }
        else
        {
            state = PlayersStore[guid].GetState();
            SF_LOG_TRACE("lfg.data.player.state.get", "Player: %u, State: %u", GUID_LOPART(guid), state);
        }
        return state;
    }

    LfgState LFGMgr::GetOldState(uint64 guid)
    {
        LfgState state;
        if (IS_GROUP_GUID(guid))
        {
            state = GroupsStore[guid].GetOldState();
            SF_LOG_TRACE("lfg.data.group.oldstate.get", "Group: %u, Old state: %u", GUID_LOPART(guid), state);
        }
        else
        {
            state = PlayersStore[guid].GetOldState();
            SF_LOG_TRACE("lfg.data.player.oldstate.get", "Player: %u, Old state: %u", GUID_LOPART(guid), state);
        }
        return state;
    }

    uint32 LFGMgr::GetDungeon(uint64 guid, bool asId /*= true */)
    {
        uint32 dungeon = GroupsStore[guid].GetDungeon(asId);
        SF_LOG_TRACE("lfg.data.group.dungeon.get", "Group: %u, asId: %u, Dungeon: %u", GUID_LOPART(guid), asId, dungeon);
        return dungeon;
    }

    uint32 LFGMgr::GetDungeonMapId(uint64 guid)
    {
        uint32 dungeonId = GroupsStore[guid].GetDungeon(true);
        uint32 mapId = 0;
        if (dungeonId)
            if (LFGDungeonData const* dungeon = GetLFGDungeon(dungeonId))
                mapId = dungeon->map;

        SF_LOG_TRACE("lfg.data.group.dungeon.map", "Group: %u, MapId: %u (DungeonId: %u)", GUID_LOPART(guid), mapId, dungeonId);
        return mapId;
    }

    uint8 LFGMgr::GetRoles(uint64 guid)
    {
        uint8 roles = PlayersStore[guid].GetRoles();
        SF_LOG_TRACE("lfg.data.player.role.get", "Player: %u, Role: %u", GUID_LOPART(guid), roles);
        return roles;
    }

    uint8 LFGMgr::GetActiveQueueId(uint64 guid)
    {
        uint8 queueId = 0;
        if (IS_GROUP_GUID(guid))
        {
            queueId = GroupsStore[guid].GetActiveQueueId();
            SF_LOG_TRACE("lfg.data.group.queue.active.get", "Group: %u, QueueId: %u", GUID_LOPART(guid), queueId);
        }
        else
        {
            queueId = PlayersStore[guid].GetActiveQueueId();
            SF_LOG_TRACE("lfg.data.player.queue.active.get", "Player: %u, QueueId: %u", GUID_LOPART(guid), queueId);
        }

        return queueId;
    }

    bool LFGMgr::IsVoteKickActive(uint64 guid)
    {
        bool active = GroupsStore[guid].IsVoteKickActive();
        SF_LOG_TRACE("lfg.data.group.votekick.get", "Group: %u, Active: %d", GUID_LOPART(guid), active);
        return active;
    }

    void LFGMgr::SetVoteKick(uint64 guid, bool active)
    {
        LfgGroupData& data = GroupsStore[guid];
        SF_LOG_TRACE("lfg.data.group.votekick.set", "Group: %u, New state: %d, Previous: %d", GUID_LOPART(guid), active, data.IsVoteKickActive());
        data.SetVoteKick(active);
    }

    const std::string& LFGMgr::GetComment(uint64 guid)
    {
        SF_LOG_TRACE("lfg.data.player.comment.get", "Player: %u, Comment: %s", GUID_LOPART(guid), PlayersStore[guid].GetComment().c_str());
        return PlayersStore[guid].GetComment();
    }

    LfgDungeonSet const& LFGMgr::GetSelectedDungeons(uint64 guid)
    {
        SF_LOG_TRACE("lfg.data.player.dungeons.selected.get", "Player: %u, Selected Dungeons: %s", GUID_LOPART(guid), ConcatenateDungeons(PlayersStore[guid].GetSelectedDungeons()).c_str());
        return PlayersStore[guid].GetSelectedDungeons();
    }

    LfgLockMap const LFGMgr::GetLockedDungeons(uint64 guid)
    {
        SF_LOG_TRACE("lfg.data.player.dungeons.locked.get", "Player: %u, LockedDungeons.", GUID_LOPART(guid));
        LfgLockMap lock;
        Player* player = ObjectAccessor::FindPlayer(guid);
        if (!player)
        {
            SF_LOG_WARN("lfg.data.player.dungeons.locked.get", "Player: %u not ingame while retrieving his LockedDungeons.", GUID_LOPART(guid));
            return lock;
        }

        uint8 level = player->getLevel();
        uint8 expansion = player->GetSession()->Expansion();
        float playerItemLevel = player->GetAverageItemLevel();
        uint32 currentItemLevel = uint32(playerItemLevel);
        LfgDungeonSet const& dungeons = GetDungeonsByRandom(0);
        bool denyJoin = !player->GetSession()->HasPermission(rbac::RBAC_PERM_JOIN_DUNGEON_FINDER);

        for (LfgDungeonSet::const_iterator it = dungeons.begin(); it != dungeons.end(); ++it)
        {
            LFGDungeonData const* dungeon = GetLFGDungeon(*it);
            if (!dungeon) // should never happen - We provide a list from sLFGDungeonStore
                continue;

            uint32 lockStatus = 0;
            uint32 requiredItemLevel = dungeon->requiredItemLevel;
            bool bypassLfgRequirements = IsDebugRequirementOverrideEnabled();

            if (denyJoin)
                lockStatus = LFG_LOCKSTATUS_RAID_LOCKED;
            else if (!bypassLfgRequirements)
            {
                if (dungeon->expansion > expansion)
                    lockStatus = LFG_LOCKSTATUS_INSUFFICIENT_EXPANSION;
                else if (DisableMgr::IsDisabledFor(DISABLE_TYPE_MAP, dungeon->map, player))
                    lockStatus = LFG_LOCKSTATUS_RAID_LOCKED;
                else if (dungeon->difficulty > DIFFICULTY_NORMAL && player->GetBoundInstance(dungeon->map, DifficultyID(dungeon->difficulty)))
                    lockStatus = LFG_LOCKSTATUS_RAID_LOCKED;
                else if (dungeon->minlevel > level)
                    lockStatus = LFG_LOCKSTATUS_TOO_LOW_LEVEL;
                else if (dungeon->maxlevel < level)
                    lockStatus = LFG_LOCKSTATUS_TOO_HIGH_LEVEL;
                else if (dungeon->seasonal && !IsSeasonActive(dungeon->id))
                    lockStatus = LFG_LOCKSTATUS_NOT_IN_SEASON;
                else if (AccessRequirement const* ar = sObjectMgr->GetAccessRequirement(dungeon->map, DifficultyID(dungeon->difficulty)))
                {
                    if (!requiredItemLevel)
                        requiredItemLevel = ar->iLvl;

                    if (requiredItemLevel && playerItemLevel < requiredItemLevel)
                        lockStatus = LFG_LOCKSTATUS_TOO_LOW_GEAR_SCORE;
                    else if (ar->achievement && !player->HasAchieved(ar->achievement))
                        lockStatus = LFG_LOCKSTATUS_MISSING_ACHIEVEMENT;
                    else if (player->GetTeam() == ALLIANCE && ar->quest_A && !player->GetQuestRewardStatus(ar->quest_A))
                        lockStatus = LFG_LOCKSTATUS_QUEST_NOT_COMPLETED;
                    else if (player->GetTeam() == HORDE && ar->quest_H && !player->GetQuestRewardStatus(ar->quest_H))
                        lockStatus = LFG_LOCKSTATUS_QUEST_NOT_COMPLETED;
                    else if (ar->item)
                    {
                        if (!player->HasItemCount(ar->item) && (!ar->item2 || !player->HasItemCount(ar->item2)))
                            lockStatus = LFG_LOCKSTATUS_MISSING_ITEM;
                    }
                    else if (ar->item2 && !player->HasItemCount(ar->item2))
                        lockStatus = LFG_LOCKSTATUS_MISSING_ITEM;
                }
                else if (requiredItemLevel && playerItemLevel < requiredItemLevel)
                    lockStatus = LFG_LOCKSTATUS_TOO_LOW_GEAR_SCORE;

                /* @todo VoA closed if WG is not under team control (LFG_LOCKSTATUS_RAID_LOCKED)
                lockStatus = LFG_LOCKSTATUS_TOO_HIGH_GEAR_SCORE;
                lockStatus = LFG_LOCKSTATUS_ATTUNEMENT_TOO_LOW_LEVEL;
                lockStatus = LFG_LOCKSTATUS_ATTUNEMENT_TOO_HIGH_LEVEL;
                */
            }

            if (lockStatus)
                lock[dungeon->Entry()] = LfgLockData(lockStatus, currentItemLevel, requiredItemLevel);
        }

        return lock;
    }

    uint8 LFGMgr::GetKicksLeft(uint64 guid)
    {
        uint8 kicks = GroupsStore[guid].GetKicksLeft();
        SF_LOG_TRACE("lfg.data.group.kickleft.get", "Group: %u, Kicks left: %u", GUID_LOPART(guid), kicks);
        return kicks;
    }

    void LFGMgr::RestoreState(uint64 guid, char const* debugMsg)
    {
        if (IS_GROUP_GUID(guid))
        {
            LfgGroupData& data = GroupsStore[guid];
            SF_LOG_TRACE("lfg.data.group.state.restore", "Group: %u (%s), State: %s, Old state: %s",
                GUID_LOPART(guid), debugMsg, GetStateString(data.GetState()).c_str(),
                GetStateString(data.GetOldState()).c_str());

            data.RestoreState();
        }
        else
        {
            LfgPlayerData& data = PlayersStore[guid];
            SF_LOG_TRACE("lfg.data.player.state.restore", "Player: %u (%s), State: %s, Old state: %s",
                GUID_LOPART(guid), debugMsg, GetStateString(data.GetState()).c_str(),
                GetStateString(data.GetOldState()).c_str());
            data.RestoreState();
        }
    }

    void LFGMgr::RestoreOrClearState(uint64 guid, char const* debugMsg)
    {
        if (GetOldState(guid) == LFG_STATE_NONE)
            ClearQueueState(guid, debugMsg);
        else
            RestoreState(guid, debugMsg);
    }

    void LFGMgr::SetState(uint64 guid, LfgState state)
    {
        if (IS_GROUP_GUID(guid))
        {
            LfgGroupData& data = GroupsStore[guid];
            SF_LOG_TRACE("lfg.data.group.state.set", "Group: %u, New state: %s, Previous: %s, Old state: %s",
                GUID_LOPART(guid), GetStateString(state).c_str(), GetStateString(data.GetState()).c_str(),
                GetStateString(data.GetOldState()).c_str());
            data.SetState(state);
        }
        else
        {
            LfgPlayerData& data = PlayersStore[guid];
            SF_LOG_TRACE("lfg.data.player.state.set", "Player: %u, New state: %s, Previous: %s, OldState: %s",
                GUID_LOPART(guid), GetStateString(state).c_str(), GetStateString(data.GetState()).c_str(),
                GetStateString(data.GetOldState()).c_str());
            data.SetState(state);
        }
    }

    void LFGMgr::ClearState(uint64 guid, char const* debugMsg)
    {
        ClearQueueState(guid, debugMsg);
    }

    void LFGMgr::ClearQueueState(uint64 guid, char const* debugMsg)
    {
        SF_LOG_TRACE("lfg.data.queue.clear", "%s: %u", debugMsg ? debugMsg : "Clear queue state", GUID_LOPART(guid));

        for (LfgQueueContainer::iterator itr = QueuesStore.begin(); itr != QueuesStore.end(); ++itr)
            itr->second.RemoveFromQueue(guid);

        while (RestoreActiveQueue(guid))
            SetState(guid, LFG_STATE_NONE);

        SetState(guid, LFG_STATE_NONE);
    }

    void LFGMgr::ClearGroupQueueState(uint64 guid, char const* debugMsg, bool sendUpdate)
    {
        if (!guid || !IS_GROUP_GUID(guid))
            return;

        LfgGroupDataContainer::const_iterator itr = GroupsStore.find(guid);
        if (itr == GroupsStore.end())
        {
            ClearQueueState(guid, debugMsg);
            return;
        }

        LfgGuidSet const players = itr->second.GetPlayers();
        LfgUpdateData removedFromQueueData(LFG_UPDATETYPE_REMOVED_FROM_QUEUE);

        RoleChecksStore.erase(guid);
        BootsStore.erase(guid);

        ClearQueueState(guid, debugMsg);
        for (LfgGuidSet::const_iterator it = players.begin(); it != players.end(); ++it)
        {
            SetGroup(*it, 0);
            ClearQueueState(*it, debugMsg);
            if (sendUpdate)
            {
                if (Player* player = ObjectAccessor::FindPlayer(*it))
                    player->GetSession()->SendLfgClearStatus();
                else
                {
                    SendLfgUpdateStatus(*it, removedFromQueueData, true);
                    SendLfgUpdateStatus(*it, removedFromQueueData, false);
                }
            }
        }
    }

    void LFGMgr::ClearDungeonGroupState(uint64 guid, uint32 dbGuid, char const* debugMsg, bool sendUpdate)
    {
        ClearGroupQueueState(guid, debugMsg, sendUpdate);

        if (!dbGuid)
            return;

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_LFG_DATA);
        stmt->setUInt32(0, dbGuid);
        CharacterDatabase.Execute(stmt);
    }

    void LFGMgr::SetActiveQueueId(uint64 guid, uint8 queueId)
    {
        if (IS_GROUP_GUID(guid))
        {
            SF_LOG_TRACE("lfg.data.group.queue.active.set", "Group: %u, QueueId: %u", GUID_LOPART(guid), queueId);
            GroupsStore[guid].SetActiveQueueId(queueId);
        }
        else
        {
            SF_LOG_TRACE("lfg.data.player.queue.active.set", "Player: %u, QueueId: %u", GUID_LOPART(guid), queueId);
            PlayersStore[guid].SetActiveQueueId(queueId);
        }
    }

    void LFGMgr::SetDungeon(uint64 guid, uint32 dungeon)
    {
        SF_LOG_TRACE("lfg.data.group.dungeon.set", "Group: %u, Dungeon: %u", GUID_LOPART(guid), dungeon);
        GroupsStore[guid].SetDungeon(dungeon);
    }

    void LFGMgr::SetRoles(uint64 guid, uint8 roles)
    {
        SF_LOG_TRACE("lfg.data.player.role.set", "Player: %u, Roles: %u", GUID_LOPART(guid), roles);
        PlayersStore[guid].SetRoles(roles);
    }

    void LFGMgr::SetComment(uint64 guid, std::string const& comment)
    {
        SF_LOG_TRACE("lfg.data.player.comment.set", "Player: %u, Comment: %s", GUID_LOPART(guid), comment.c_str());
        PlayersStore[guid].SetComment(comment);
    }

    void LFGMgr::SetSelectedDungeons(uint64 guid, LfgDungeonSet const& dungeons)
    {
        SF_LOG_TRACE("lfg.data.player.dungeon.selected.set", "Player: %u, Dungeons: %s", GUID_LOPART(guid), ConcatenateDungeons(dungeons).c_str());
        PlayersStore[guid].SetSelectedDungeons(dungeons);
    }

    void LFGMgr::DecreaseKicksLeft(uint64 guid)
    {
        GroupsStore[guid].DecreaseKicksLeft();
        SF_LOG_TRACE("lfg.data.group.kicksleft.decrease", "Group: %u, Kicks: %u", GUID_LOPART(guid), GroupsStore[guid].GetKicksLeft());
    }

    void LFGMgr::RemovePlayerData(uint64 guid)
    {
        SF_LOG_TRACE("lfg.data.player.remove", "Player: %u", GUID_LOPART(guid));
        LfgPlayerDataContainer::iterator it = PlayersStore.find(guid);
        if (it != PlayersStore.end())
            PlayersStore.erase(it);
    }

    void LFGMgr::RemoveGroupData(uint64 guid)
    {
        SF_LOG_TRACE("lfg.data.group.remove", "Group: %u", GUID_LOPART(guid));
        LfgGroupDataContainer::iterator it = GroupsStore.find(guid);
        if (it == GroupsStore.end())
            return;

        LfgState state = GetState(guid);

        for (LfgProposalContainer::iterator itProposal = ProposalsStore.begin(); itProposal != ProposalsStore.end();)
        {
            LfgProposalContainer::iterator itProposalRemove = itProposal++;
            bool removeProposal = false;

            for (LfgProposalPlayerContainer::iterator itPlayer = itProposalRemove->second.players.begin();
                itPlayer != itProposalRemove->second.players.end(); ++itPlayer)
            {
                if (itPlayer->second.group != guid)
                    continue;

                itPlayer->second.accept = LFG_ANSWER_DENY;
                removeProposal = true;
            }

            if (removeProposal)
                RemoveProposal(itProposalRemove, LFG_UPDATETYPE_PROPOSAL_DECLINED);
        }

        for (LfgQueueContainer::iterator itQueue = QueuesStore.begin(); itQueue != QueuesStore.end(); ++itQueue)
            itQueue->second.RemoveFromQueue(guid);

        // If group is being formed after proposal success do nothing more
        LfgGuidSet players = it->second.GetPlayers();
        for (LfgGuidSet::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            uint64 guid = (*itr);
            SetGroup(guid, 0);
            if (state != LFG_STATE_PROPOSAL)
            {
                ClearQueueState(guid, "Remove group data");
                if (state != LFG_STATE_NONE)
                {
                    SendLfgUpdateStatus(guid, LfgUpdateData(LFG_UPDATETYPE_REMOVED_FROM_QUEUE), true);
                    SendLfgUpdateStatus(guid, LfgUpdateData(LFG_UPDATETYPE_REMOVED_FROM_QUEUE), false);
                }
            }
            else
                ClearQueueState(guid, "Remove proposal group data");
        }

        RoleChecksStore.erase(guid);
        BootsStore.erase(guid);
        GroupsStore.erase(it);
    }

    uint8 LFGMgr::GetTeam(uint64 guid)
    {
        uint8 team = PlayersStore[guid].GetTeam();
        SF_LOG_TRACE("lfg.data.player.team.get", "Player: %u, Team: %u", GUID_LOPART(guid), team);
        return team;
    }

    uint8 LFGMgr::RemovePlayerFromGroup(uint64 gguid, uint64 guid)
    {
        return GroupsStore[gguid].RemovePlayer(guid);
    }

    void LFGMgr::AddPlayerToGroup(uint64 gguid, uint64 guid)
    {
        GroupsStore[gguid].AddPlayer(guid);
    }

    void LFGMgr::SetLeader(uint64 gguid, uint64 leader)
    {
        GroupsStore[gguid].SetLeader(leader);
    }

    void LFGMgr::SetTeam(uint64 guid, uint8 team)
    {
        if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ALLOW_TWO_SIDE_INTERACTION_GROUP))
            team = 0;

        PlayersStore[guid].SetTeam(team);
    }

    uint64 LFGMgr::GetGroup(uint64 guid)
    {
        return PlayersStore[guid].GetGroup();
    }

    void LFGMgr::SetGroup(uint64 guid, uint64 group)
    {
        PlayersStore[guid].SetGroup(group);
    }

    LfgGuidSet const& LFGMgr::GetPlayers(uint64 guid)
    {
        return GroupsStore[guid].GetPlayers();
    }

    uint8 LFGMgr::GetPlayerCount(uint64 guid)
    {
        return GroupsStore[guid].GetPlayerCount();
    }

    uint64 LFGMgr::GetLeader(uint64 guid)
    {
        return GroupsStore[guid].GetLeader();
    }

    bool LFGMgr::HasIgnore(uint64 guid1, uint64 guid2)
    {
        Player* plr1 = ObjectAccessor::FindPlayer(guid1);
        Player* plr2 = ObjectAccessor::FindPlayer(guid2);
        uint32 low1 = GUID_LOPART(guid1);
        uint32 low2 = GUID_LOPART(guid2);
        return plr1 && plr2 && (plr1->GetSocial()->HasIgnore(low2) || plr2->GetSocial()->HasIgnore(low1));
    }

    void LFGMgr::SendLfgRoleChosen(uint64 guid, uint64 pguid, uint8 roles)
    {
        if (Player* player = ObjectAccessor::FindPlayer(guid))
            player->GetSession()->SendLfgRoleChosen(pguid, roles);
    }

    void LFGMgr::SendLfgRoleCheckUpdate(uint64 guid, LfgRoleCheck const& roleCheck)
    {
        if (Player* player = ObjectAccessor::FindPlayer(guid))
            player->GetSession()->SendLfgRoleCheckUpdate(roleCheck);
    }

    void LFGMgr::SendLfgUpdateStatus(uint64 guid, LfgUpdateData const& data, bool party)
    {
        if (Player* player = ObjectAccessor::FindPlayer(guid))
            player->GetSession()->SendLfgUpdateStatus(data, party);
    }

    void LFGMgr::SendLfgJoinResult(uint64 guid, LfgJoinResultData const& data)
    {
        if (Player* player = ObjectAccessor::FindPlayer(guid))
            player->GetSession()->SendLfgJoinResult(data);
    }

    void LFGMgr::SendLfgBootProposalUpdate(uint64 guid, LfgPlayerBoot const& boot)
    {
        if (Player* player = ObjectAccessor::FindPlayer(guid))
            player->GetSession()->SendLfgBootProposalUpdate(boot);
    }

    void LFGMgr::SendLfgUpdateProposal(uint64 guid, LfgProposal const& proposal)
    {
        if (Player* player = ObjectAccessor::FindPlayer(guid))
            player->GetSession()->SendLfgUpdateProposal(proposal);
    }

    void LFGMgr::SendLfgQueueStatus(uint64 guid, LfgQueueStatusData const& data)
    {
        if (Player* player = ObjectAccessor::FindPlayer(guid))
            player->GetSession()->SendLfgQueueStatus(data);
    }

    bool LFGMgr::IsLfgGroup(uint64 guid)
    {
        return guid && IS_GROUP_GUID(guid) && GroupsStore[guid].IsLfgGroup();
    }

    uint8 LFGMgr::GetQueueId(uint64 guid)
    {
        if (IS_GROUP_GUID(guid))
        {
            LfgGroupData const& groupData = GroupsStore[guid];
            if (groupData.GetQueues().find(groupData.GetActiveQueueId()) != groupData.GetQueues().end())
                return groupData.GetActiveQueueId();

            LfgGuidSet const& players = GetPlayers(guid);
            uint64 pguid = players.empty() ? 0 : (*players.begin());
            if (pguid)
                return GetQueueId(pguid);
        }

        LfgPlayerData const& playerData = PlayersStore[guid];
        if (playerData.GetQueues().find(playerData.GetActiveQueueId()) != playerData.GetQueues().end())
            return playerData.GetActiveQueueId();

        return GetTeam(guid);
    }

    LFGQueue& LFGMgr::GetQueue(uint64 guid)
    {
        uint8 queueId = GetQueueId(guid);
        return QueuesStore[queueId];
    }

    bool LFGMgr::AllQueued(LfgGuidList const& check)
    {
        if (check.empty())
            return false;

        for (LfgGuidList::const_iterator it = check.begin(); it != check.end(); ++it)
        {
            LfgState state = GetState(*it);
            if (state != LFG_STATE_QUEUED)
            {
                if (state != LFG_STATE_PROPOSAL)
                    SF_LOG_DEBUG("lfg.allqueued", "Unexpected state found while trying to form new group. Guid: %u, State: %s", GUID_LOPART((*it)), GetStateString(state).c_str());
                return false;
            }
        }
        return true;
    }

    time_t LFGMgr::GetQueueJoinTime(uint64 guid)
    {
        uint8 queueId = GetQueueId(guid);
        LfgQueueContainer::const_iterator itr = QueuesStore.find(queueId);
        uint64 queueGuid = guid;
        if (!IS_GROUP_GUID(guid))
            if (uint64 gguid = GetGroup(guid))
            {
                LfgState groupState = GetState(gguid);
                if (groupState == LFG_STATE_QUEUED || groupState == LFG_STATE_PROPOSAL)
                    queueGuid = gguid;
            }

        if (itr != QueuesStore.end())
            return itr->second.GetJoinTime(queueGuid);

        return 0;
    }

    // Only for debugging purposes
    void LFGMgr::Clean()
    {
        SF_LOG_INFO("lfg", "Clearing all Dungeon Finder runtime state: queues=%u players=%u groups=%u roleChecks=%u proposals=%u boots=%u",
            uint32(QueuesStore.size()), uint32(PlayersStore.size()), uint32(GroupsStore.size()),
            uint32(RoleChecksStore.size()), uint32(ProposalsStore.size()), uint32(BootsStore.size()));

        QueuesStore.clear();
        PlayersStore.clear();
        GroupsStore.clear();
        RoleChecksStore.clear();
        ProposalsStore.clear();
        BootsStore.clear();
    }

    bool LFGMgr::isOptionEnabled(uint32 option)
    {
        return m_options & option;
    }

    uint32 LFGMgr::GetOptions()
    {
        return m_options;
    }

    void LFGMgr::SetOptions(uint32 options)
    {
        m_options = options;
    }

    LfgUpdateData LFGMgr::GetLfgStatus(uint64 guid)
    {
        RestoreActiveQueue(guid);

        LfgPlayerData& playerData = PlayersStore[guid];
        if (uint64 gguid = GetGroup(guid))
        {
            if (!sGroupMgr->GetGroupByGUID(GUID_LOPART(gguid)))
            {
                SF_LOG_DEBUG("lfg.status", "Player %u had stale LFG group %u while requesting status; clearing finder state.",
                    GUID_LOPART(guid), GUID_LOPART(gguid));
                SetGroup(guid, 0);
                ClearQueueState(guid, "Stale group status request");
                return LfgUpdateData(LFG_UPDATETYPE_REMOVED_FROM_QUEUE);
            }

            RestoreActiveQueue(gguid);
            LfgState groupState = GetState(gguid);
            if (groupState != LFG_STATE_NONE)
            {
                LfgDungeonSet statusDungeons = playerData.GetSelectedDungeons();
                if (statusDungeons.empty())
                    if (uint32 dungeon = GetDungeon(gguid, false))
                        statusDungeons.insert(dungeon);

                return LfgUpdateData(LFG_UPDATETYPE_UPDATE_STATUS, groupState, statusDungeons);
            }
        }

        return LfgUpdateData(LFG_UPDATETYPE_UPDATE_STATUS, playerData.GetState(), playerData.GetSelectedDungeons());
    }

    bool LFGMgr::SendActiveProposal(uint64 guid)
    {
        if (!guid)
            return false;

        for (LfgProposalContainer::const_iterator itr = ProposalsStore.begin(); itr != ProposalsStore.end(); ++itr)
        {
            LfgProposalPlayerContainer::const_iterator itPlayer = itr->second.players.find(guid);
            if (itPlayer == itr->second.players.end())
                continue;

            SendLfgUpdateProposal(guid, itr->second);
            return true;
        }

        return false;
    }

    bool LFGMgr::RestoreActiveQueue(uint64 guid)
    {
        if (!guid)
            return false;

        if (IS_GROUP_GUID(guid))
        {
            LfgGroupData& groupData = GroupsStore[guid];
            uint8 activeQueueId = groupData.GetActiveQueueId();
            LfgGroupQueueDataContainer const& queues = groupData.GetQueues();
            LfgGroupQueueDataContainer::const_iterator activeItr = queues.find(activeQueueId);
            if (activeItr != queues.end() && (activeItr->second.State != LFG_STATE_NONE || activeItr->second.OldState != LFG_STATE_NONE))
                return true;

            for (LfgGroupQueueDataContainer::const_iterator itr = queues.begin(); itr != queues.end(); ++itr)
            {
                if (itr->second.State == LFG_STATE_NONE && itr->second.OldState == LFG_STATE_NONE)
                    continue;

                groupData.SetActiveQueueId(itr->first);
                return true;
            }

            return false;
        }

        LfgPlayerData& playerData = PlayersStore[guid];
        uint8 activeQueueId = playerData.GetActiveQueueId();
        LfgPlayerQueueDataContainer const& queues = playerData.GetQueues();
        LfgPlayerQueueDataContainer::const_iterator activeItr = queues.find(activeQueueId);
        if (activeItr != queues.end() && (activeItr->second.State != LFG_STATE_NONE || activeItr->second.OldState != LFG_STATE_NONE))
            return true;

        for (LfgPlayerQueueDataContainer::const_iterator itr = queues.begin(); itr != queues.end(); ++itr)
        {
            if (itr->second.State == LFG_STATE_NONE && itr->second.OldState == LFG_STATE_NONE)
                continue;

            playerData.SetActiveQueueId(itr->first);
            return true;
        }

        return false;
    }

    bool LFGMgr::IsSeasonActive(uint32 dungeonId)
    {
        switch (dungeonId)
        {
            case 285: // The Headless Horseman
                return IsHolidayActive(HolidayIds::HOLIDAY_HALLOWS_END);
            case 286: // The Frost Lord Ahune
                return IsHolidayActive(HolidayIds::HOLIDAY_FIRE_FESTIVAL);
            case 287: // Coren Direbrew
                return IsHolidayActive(HolidayIds::HOLIDAY_BREWFEST);
            case 288: // The Crown Chemical Co.
                return IsHolidayActive(HolidayIds::HOLIDAY_LOVE_IS_IN_THE_AIR);
        }
        return false;
    }

    std::string LFGMgr::DumpQueueInfo(bool full)
    {
        uint32 size = uint32(QueuesStore.size());
        std::ostringstream o;

        o << "Number of Queues: " << size << "\n";
        for (LfgQueueContainer::const_iterator itr = QueuesStore.begin(); itr != QueuesStore.end(); ++itr)
        {
            o << "Queue Id: " << uint32(itr->first) << "\n";
            std::string const& queued = itr->second.DumpQueueInfo(full);
            std::string const& compatibles = itr->second.DumpCompatibleInfo(full);
            o << queued << compatibles;
        }

        if (full)
        {
            time_t const currTime = time(NULL);

            o << "Role Checks: " << RoleChecksStore.size() << "\n";
            for (LfgRoleCheckContainer::const_iterator itr = RoleChecksStore.begin(); itr != RoleChecksStore.end(); ++itr)
            {
                LfgRoleCheck const& roleCheck = itr->second;
                o << "  Group " << itr->first
                    << " state: " << uint32(roleCheck.state)
                    << " leader: " << roleCheck.leader
                    << " expires: " << uint32(roleCheck.cancelTime > currTime ? roleCheck.cancelTime - currTime : 0) << "s"
                    << " random: " << roleCheck.rDungeonId
                    << " dungeons: " << ConcatenateDungeons(roleCheck.dungeons) << "\n";

                for (LfgRolesMap::const_iterator itRoles = roleCheck.roles.begin(); itRoles != roleCheck.roles.end(); ++itRoles)
                    o << "    role " << itRoles->first << ": " << GetRolesString(itRoles->second) << "\n";
            }

            o << "Proposals: " << ProposalsStore.size() << "\n";
            for (LfgProposalContainer::const_iterator itr = ProposalsStore.begin(); itr != ProposalsStore.end(); ++itr)
            {
                LfgProposal const& proposal = itr->second;
                o << "  Proposal " << itr->first
                    << " state: " << uint32(proposal.state)
                    << " dungeon: " << proposal.dungeonId
                    << " group: " << proposal.group
                    << " leader: " << proposal.leader
                    << " expires: " << uint32(proposal.cancelTime > currTime ? proposal.cancelTime - currTime : 0) << "s"
                    << " queues: " << ConcatenateGuids(proposal.queues) << "\n";

                for (LfgProposalPlayerContainer::const_iterator itPlayer = proposal.players.begin(); itPlayer != proposal.players.end(); ++itPlayer)
                {
                    LfgProposalPlayer const& player = itPlayer->second;
                    o << "    player " << itPlayer->first
                        << " role: " << GetRolesString(player.role)
                        << " accept: " << int32(player.accept)
                        << " group: " << player.group << "\n";
                }
            }

            o << "Boot Votes: " << BootsStore.size() << "\n";
        }

        return o.str();
    }

    std::string LFGMgr::DumpPlayerInfo(uint64 guid)
    {
        std::ostringstream o;
        LfgPlayerDataContainer::const_iterator itr = PlayersStore.find(guid);
        if (itr == PlayersStore.end())
        {
            o << "LFG player data missing for guid " << guid << "\n";
            return o.str();
        }

        LfgPlayerData const& playerData = itr->second;
        time_t const currTime = time(NULL);
        uint64 const group = playerData.GetGroup();

        o << "LFG Player: " << guid << "\n";
        o << "  Active Queue: " << uint32(playerData.GetActiveQueueId()) << "\n";
        o << "  Original Group: " << group << "\n";
        if (group)
            o << "  Original Group Exists: " << (sGroupMgr->GetGroupByGUID(GUID_LOPART(group)) ? "yes" : "no") << "\n";
        o << "  State: " << GetStateString(playerData.GetState()) << " old: " << GetStateString(playerData.GetOldState()) << "\n";
        o << "  Roles: " << GetRolesString(playerData.GetRoles()) << "\n";
        o << "  Dungeons: " << ConcatenateDungeons(playerData.GetSelectedDungeons()) << "\n";
        o << "  Locked Dungeons: " << GetLockedDungeons(guid).size() << "\n";
        if (!playerData.GetComment().empty())
            o << "  Comment: " << playerData.GetComment() << "\n";

        LfgPlayerQueueDataContainer const& queues = playerData.GetQueues();
        o << "  Saved Queues: " << queues.size() << "\n";
        for (LfgPlayerQueueDataContainer::const_iterator itQueue = queues.begin(); itQueue != queues.end(); ++itQueue)
        {
            time_t joinTime = 0;
            LfgQueueContainer::const_iterator itLfgQueue = QueuesStore.find(itQueue->first);
            if (itLfgQueue != QueuesStore.end())
                joinTime = itLfgQueue->second.GetJoinTime(guid);

            o << "    Queue " << uint32(itQueue->first)
                << " state: " << GetStateString(itQueue->second.State)
                << " old: " << GetStateString(itQueue->second.OldState)
                << " roles: " << GetRolesString(itQueue->second.Roles)
                << " dungeons: " << ConcatenateDungeons(itQueue->second.SelectedDungeons);

            if (joinTime)
                o << " queued: " << uint32(currTime > joinTime ? currTime - joinTime : 0) << "s";

            if (!itQueue->second.Comment.empty())
                o << " comment: " << itQueue->second.Comment;

            o << "\n";
        }

        return o.str();
    }

    std::string LFGMgr::DumpGroupInfo(uint64 guid)
    {
        std::ostringstream o;
        LfgGroupDataContainer::const_iterator itr = GroupsStore.find(guid);
        if (itr == GroupsStore.end())
        {
            o << "LFG group data missing for guid " << guid << "\n";
            return o.str();
        }

        LfgGroupData const& groupData = itr->second;
        time_t const currTime = time(NULL);

        o << "LFG Group: " << guid << "\n";
        o << "  Is LFG Group: " << (groupData.IsLfgGroup() ? "yes" : "no") << "\n";
        o << "  Active Queue: " << uint32(groupData.GetActiveQueueId()) << "\n";
        o << "  State: " << GetStateString(groupData.GetState()) << " old: " << GetStateString(groupData.GetOldState()) << "\n";
        o << "  Dungeon: " << groupData.GetDungeon(true) << "\n";
        o << "  Leader: " << groupData.GetLeader() << "\n";
        o << "  Players: " << groupData.GetPlayers().size() << "\n";
        o << "  Kicks Left: " << uint32(groupData.GetKicksLeft()) << "\n";
        o << "  Vote Kick: " << (groupData.IsVoteKickActive() ? "active" : "inactive") << "\n";

        LfgGroupQueueDataContainer const& queues = groupData.GetQueues();
        o << "  Saved Queues: " << queues.size() << "\n";
        for (LfgGroupQueueDataContainer::const_iterator itQueue = queues.begin(); itQueue != queues.end(); ++itQueue)
        {
            time_t joinTime = 0;
            LfgQueueContainer::const_iterator itLfgQueue = QueuesStore.find(itQueue->first);
            if (itLfgQueue != QueuesStore.end())
                joinTime = itLfgQueue->second.GetJoinTime(guid);

            o << "    Queue " << uint32(itQueue->first)
                << " state: " << GetStateString(itQueue->second.State)
                << " old: " << GetStateString(itQueue->second.OldState)
                << " dungeon: " << itQueue->second.Dungeon;

            if (joinTime)
                o << " queued: " << uint32(currTime > joinTime ? currTime - joinTime : 0) << "s";

            o << "\n";
        }

        for (LfgGuidSet::const_iterator itPlayer = groupData.GetPlayers().begin(); itPlayer != groupData.GetPlayers().end(); ++itPlayer)
            o << "    Member: " << *itPlayer << "\n";

        return o.str();
    }

    void LFGMgr::SetupGroupMember(uint64 guid, uint64 gguid)
    {
        LfgDungeonSet dungeons;
        dungeons.insert(GetDungeon(gguid));
        SetActiveQueueId(guid, GetActiveQueueId(gguid));
        SetSelectedDungeons(guid, dungeons);
        SetState(guid, GetState(gguid));
        SetGroup(guid, gguid);
        AddPlayerToGroup(gguid, guid);
    }

    bool LFGMgr::selectedRandomLfgDungeon(uint64 guid)
    {
        if (GetState(guid) != LFG_STATE_NONE)
        {
            LfgDungeonSet const& dungeons = GetSelectedDungeons(guid);
            if (!dungeons.empty())
            {
                LFGDungeonData const* dungeon = GetLFGDungeon(*dungeons.begin());
                if (dungeon && (dungeon->type == LFG_TYPE_RANDOM || dungeon->seasonal))
                    return true;
            }
        }

        return false;
    }

    bool LFGMgr::inLfgDungeonMap(uint64 guid, uint32 map, DifficultyID difficulty)
    {
        if (IS_GROUP_GUID(guid))
        {
            if (uint32 dungeonId = GetDungeon(guid, true))
                if (LFGDungeonData const* dungeon = GetLFGDungeon(dungeonId))
                    if (uint32(dungeon->map) == map && dungeon->difficulty == difficulty)
                        return true;

            return false;
        }

        if (uint64 gguid = GetGroup(guid))
            if (uint32 dungeonId = GetDungeon(gguid, true))
                if (LFGDungeonData const* dungeon = GetLFGDungeon(dungeonId))
                    if (uint32(dungeon->map) == map && dungeon->difficulty == difficulty)
                        return true;

        LfgDungeonSet const& selectedDungeons = GetSelectedDungeons(guid);
        for (LfgDungeonSet::const_iterator itr = selectedDungeons.begin(); itr != selectedDungeons.end(); ++itr)
            if (LFGDungeonData const* dungeon = GetLFGDungeon(*itr))
                if (uint32(dungeon->map) == map && dungeon->difficulty == difficulty)
                    return true;

        return false;
    }

    uint32 LFGMgr::GetLFGDungeonEntry(uint32 id)
    {
        if (id)
            if (LFGDungeonData const* dungeon = GetLFGDungeon(id))
                return dungeon->Entry();

        return 0;
    }

    uint8 LFGMgr::GetLFGDungeonCategory(uint32 id)
    {
        if (id)
            if (LFGDungeonData const* dungeon = GetLFGDungeon(id))
                return dungeon->category;

        return 0;
    }

    LfgDungeonSet LFGMgr::GetRandomAndSeasonalDungeons(uint8 level, uint8 expansion)
    {
        LfgDungeonSet randomDungeons;
        for (lfg::LFGDungeonContainer::const_iterator itr = LfgDungeonStore.begin(); itr != LfgDungeonStore.end(); ++itr)
        {
            lfg::LFGDungeonData const& dungeon = itr->second;
            if ((dungeon.type == lfg::LFG_TYPE_RANDOM || (dungeon.seasonal && sLFGMgr->IsSeasonActive(dungeon.id)))
                && dungeon.expansion <= expansion && dungeon.minlevel <= level && level <= dungeon.maxlevel)
                randomDungeons.insert(dungeon.Entry());
        }
        return randomDungeons;
    }

} // namespace lfg
