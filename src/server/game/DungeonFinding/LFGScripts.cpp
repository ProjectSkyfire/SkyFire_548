/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/*
 * Interaction between core and LFGScripts
 */

#include "Common.h"
#include "SharedDefines.h"
#include "Player.h"
#include "Group.h"
#include "LFGScripts.h"
#include "LFGMgr.h"
#include "ScriptMgr.h"
#include "ObjectAccessor.h"
#include "WorldSession.h"

namespace lfg
{

LFGPlayerScript::LFGPlayerScript() : PlayerScript("LFGPlayerScript") { }

void LFGPlayerScript::OnLogout(Player* player)
{
    if (!sLFGMgr->isOptionEnabled(LFG_OPTION_ENABLE_DUNGEON_FINDER | LFG_OPTION_ENABLE_RAID_BROWSER))
        return;

    if (!player->GetGroup())
    {
        player->GetSession()->SendLfgLfrList(false);
        sLFGMgr->LeaveLfg(player->GetGUID());
    }
    else if (player->GetSession()->PlayerDisconnected())
        sLFGMgr->LeaveLfg(player->GetGUID(), true);
}

void LFGPlayerScript::OnLogin(Player* player, bool /*loginFirst*/)
{
    if (!sLFGMgr->isOptionEnabled(LFG_OPTION_ENABLE_DUNGEON_FINDER | LFG_OPTION_ENABLE_RAID_BROWSER))
        return;

    // Temporal: Trying to determine when group data and LFG data gets desynched
    uint64 guid = player->GetGUID();
    uint64 gguid = sLFGMgr->GetGroup(guid);

    if (Group const* group = player->GetGroup())
    {
        uint64 gguid2 = group->GetGUID();
        if (gguid != gguid2)
        {
            SF_LOG_ERROR("lfg", "%s on group %u but LFG has group %u saved... Fixing.",
                player->GetSession()->GetPlayerInfo().c_str(), GUID_LOPART(gguid2), GUID_LOPART(gguid));
            sLFGMgr->SetupGroupMember(guid, group->GetGUID());
        }
    }
    sLFGMgr->SetTeam(player->GetGUID(), player->GetTeam());
    /// @todo - Restore LfgPlayerData and send proper status to player if it was in a group
}

void LFGPlayerScript::OnMapChanged(Player* player)
{
    Map const* map = player->GetMap();

    if (sLFGMgr->inLfgDungeonMap(player->GetGUID(), map->GetId(), map->GetDifficulty()))
    {
        Group* group = player->GetGroup();
        // This function is also called when players log in
        // if for some reason the LFG system recognises the player as being in a LFG dungeon,
        // but the player was loaded without a valid group, we'll teleport to homebind to prevent
        // crashes or other undefined behaviour
        if (!group)
        {
            sLFGMgr->LeaveLfg(player->GetGUID());
            player->RemoveAurasDueToSpell(LFG_SPELL_LUCK_OF_THE_DRAW);
            player->TeleportTo(player->m_homebindMapId, player->m_homebindX, player->m_homebindY, player->m_homebindZ, 0.0f);
            SF_LOG_ERROR("lfg", "LFGPlayerScript::OnMapChanged, Player %s (%u) is in LFG dungeon map but does not have a valid group! "
                "Teleporting to homebind.", player->GetName().c_str(), player->GetGUIDLow());
            return;
        }

        for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
            if (Player* member = itr->GetSource())
                player->GetSession()->SendNameQueryOpcode(member->GetGUID());

        if (sLFGMgr->selectedRandomLfgDungeon(player->GetGUID()))
            player->CastSpell(player, LFG_SPELL_LUCK_OF_THE_DRAW, true);
    }
    else
    {
        Group* group = player->GetGroup();
        if (group && group->GetMembersCount() == 1)
        {
            sLFGMgr->LeaveLfg(group->GetGUID());
            group->Disband();
            SF_LOG_DEBUG("lfg", "LFGPlayerScript::OnMapChanged, Player %s(%u) is last in the lfggroup so we disband the group.",
                player->GetName().c_str(), GUID_LOPART(player->GetGUID()));
        }
        player->RemoveAurasDueToSpell(LFG_SPELL_LUCK_OF_THE_DRAW);
    }
}

LFGGroupScript::LFGGroupScript() : GroupScript("LFGGroupScript") { }

void LFGGroupScript::OnAddMember(Group* group, uint64 guid)
{
    if (!sLFGMgr->isOptionEnabled(LFG_OPTION_ENABLE_DUNGEON_FINDER | LFG_OPTION_ENABLE_RAID_BROWSER))
        return;

    uint64 gguid = group->GetGUID();
    uint64 leader = group->GetLeaderGUID();

    if (leader == guid)
    {
        SF_LOG_DEBUG("lfg", "LFGScripts::OnAddMember [" UI64FMTD "]: added [" UI64FMTD "] leader " UI64FMTD "]", gguid, guid, leader);
        sLFGMgr->SetLeader(gguid, guid);
    }
    else
    {
        LfgState gstate = sLFGMgr->GetState(gguid);
        LfgState state = sLFGMgr->GetState(guid);
        SF_LOG_DEBUG("lfg", "LFGScripts::OnAddMember [" UI64FMTD "]: added [" UI64FMTD "] leader " UI64FMTD "] gstate: %u, state: %u", gguid, guid, leader, gstate, state);

        if (state == LFG_STATE_QUEUED)
            sLFGMgr->LeaveLfg(guid);

        if (gstate == LFG_STATE_QUEUED)
            sLFGMgr->LeaveLfg(gguid);
    }

    sLFGMgr->SetGroup(guid, gguid);
    sLFGMgr->AddPlayerToGroup(gguid, guid);
}

void LFGGroupScript::OnRemoveMember(Group* group, uint64 guid, RemoveMethod method, uint64 kicker, char const* reason)
{
    if (!sLFGMgr->isOptionEnabled(LFG_OPTION_ENABLE_DUNGEON_FINDER | LFG_OPTION_ENABLE_RAID_BROWSER))
        return;

    uint64 gguid = group->GetGUID();
    SF_LOG_DEBUG("lfg", "LFGScripts::OnRemoveMember [" UI64FMTD "]: remove [" UI64FMTD "] Method: %d Kicker: [" UI64FMTD "] Reason: %s", gguid, guid, method, kicker, (reason ? reason : ""));

    bool isLFG = group->isLFGGroup();

    if (isLFG && method == GROUP_REMOVEMETHOD_KICK)        // Player have been kicked
    {
        /// @todo - Update internal kick cooldown of kicker
        std::string str_reason = "";
        if (reason)
            str_reason = std::string(reason);
        sLFGMgr->InitBoot(gguid, kicker, guid, str_reason);
        return;
    }

    LfgState state = sLFGMgr->GetState(gguid);

    // If group is being formed after proposal success do nothing more
    if (state == LFG_STATE_PROPOSAL && method == GROUP_REMOVEMETHOD_DEFAULT)
    {
        // LfgData: Remove player from group
        sLFGMgr->SetGroup(guid, 0);
        sLFGMgr->RemovePlayerFromGroup(gguid, guid);
        return;
    }

    sLFGMgr->LeaveLfg(guid);
    sLFGMgr->SetGroup(guid, 0);
    uint8 players = sLFGMgr->RemovePlayerFromGroup(gguid, guid);

    if (Player* player = ObjectAccessor::FindPlayer(guid))
    {
        if (method == GROUP_REMOVEMETHOD_LEAVE && state == LFG_STATE_DUNGEON &&
            players >= LFG_GROUP_KICK_VOTES_NEEDED)
            player->CastSpell(player, LFG_SPELL_DUNGEON_DESERTER, true);
        //else if (state == LFG_STATE_BOOT)
            // Update internal kick cooldown of kicked

        player->GetSession()->SendLfgUpdateStatus(LfgUpdateData(LFG_UPDATETYPE_LEADER_UNK1), true);
        if (isLFG && player->GetMap()->IsInstance())            // Teleport player out the dungeon
            sLFGMgr->TeleportPlayer(player, true);
    }

    if (isLFG && state != LFG_STATE_FINISHED_DUNGEON) // Need more players to finish the dungeon
        if (Player* leader = ObjectAccessor::FindPlayer(sLFGMgr->GetLeader(gguid)))
            leader->GetSession()->SendLfgOfferContinue(sLFGMgr->GetDungeon(gguid, false));
}

void LFGGroupScript::OnDisband(Group* group)
{
    if (!sLFGMgr->isOptionEnabled(LFG_OPTION_ENABLE_DUNGEON_FINDER | LFG_OPTION_ENABLE_RAID_BROWSER))
        return;

    uint64 gguid = group->GetGUID();
    SF_LOG_DEBUG("lfg", "LFGScripts::OnDisband [" UI64FMTD "]", gguid);

    sLFGMgr->RemoveGroupData(gguid);
}

void LFGGroupScript::OnChangeLeader(Group* group, uint64 newLeaderGuid, uint64 oldLeaderGuid)
{
    if (!sLFGMgr->isOptionEnabled(LFG_OPTION_ENABLE_DUNGEON_FINDER | LFG_OPTION_ENABLE_RAID_BROWSER))
        return;

    uint64 gguid = group->GetGUID();

    SF_LOG_DEBUG("lfg", "LFGScripts::OnChangeLeader [" UI64FMTD "]: old [" UI64FMTD "] new [" UI64FMTD "]", gguid, newLeaderGuid, oldLeaderGuid);
    sLFGMgr->SetLeader(gguid, newLeaderGuid);
}

void LFGGroupScript::OnInviteMember(Group* group, uint64 guid)
{
    if (!sLFGMgr->isOptionEnabled(LFG_OPTION_ENABLE_DUNGEON_FINDER | LFG_OPTION_ENABLE_RAID_BROWSER))
        return;

    uint64 gguid = group->GetGUID();
    uint64 leader = group->GetLeaderGUID();
    SF_LOG_DEBUG("lfg", "LFGScripts::OnInviteMember [" UI64FMTD "]: invite [" UI64FMTD "] leader [" UI64FMTD "]", gguid, guid, leader);
    // No gguid ==  new group being formed
    // No leader == after group creation first invite is new leader
    // leader and no gguid == first invite after leader is added to new group (this is the real invite)
    if (leader && !gguid)
        sLFGMgr->LeaveLfg(leader);
}

} // namespace lfg
