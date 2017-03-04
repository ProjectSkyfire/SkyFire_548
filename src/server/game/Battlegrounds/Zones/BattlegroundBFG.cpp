/*
* Copyright (C) 2011-2017 Project SkyFire <http://www.projectskyfire.org/>
* Copyright (C) 2008-2017 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2017 MaNGOS <https://www.getmangos.eu/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 3 of the License, or (at your
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

#include "AchievementMgr.h"
#include "Battleground.h"
#include "BattlegroundBFG.h"
#include "World.h"
#include "WorldPacket.h"
#include "ObjectMgr.h"
#include "BattlegroundMgr.h"
#include "Creature.h"
#include "Language.h"
#include "Object.h"
#include "Player.h"
#include "Util.h"
#include "WorldSession.h"

BattlegroundBFG::BattlegroundBFG()
{
    m_IsInformedNearVictory = false;
    m_BuffChange = true;
    BgObjects.resize(GILNEAS_BG_OBJECT_MAX);
    BgCreatures.resize(GILNEAS_BG_ALL_NODES_COUNT + 3);

    for (uint8 i = 0; i < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++i)
    {
        m_Nodes[i] = 0;
        m_prevNodes[i] = 0;
        m_NodeTimers[i] = 0;
        m_BannerTimers[i].timer = 0;
        m_BannerTimers[i].type = 0;
        m_BannerTimers[i].teamIndex = 0;
    }

    for (uint8 i = 0; i < BG_TEAMS_COUNT; ++i)
    {
        m_lastTick[i] = 0;
        m_HonorScoreTics[i] = 0;
        m_ReputationScoreTics[i] = 0;
        m_TeamScores500Disadvantage[i] = false;
    }

    m_HonorTics = 0;
    m_ReputationTics = 0;

    StartMessageIds[BG_STARTING_EVENT_FIRST] = LANG_BG_BFG_START_TWO_MINUTES;
    StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_BG_BFG_START_ONE_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_THIRD] = LANG_BG_BFG_START_HALF_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_BG_BFG_HAS_BEGUN;
}

BattlegroundBFG::~BattlegroundBFG() {}

void BattlegroundBFG::PostUpdateImpl(uint32 diff)
{
    if (GetStatus() == STATUS_IN_PROGRESS) {
        int team_points[BG_TEAMS_COUNT] = { 0, 0 };

        for (int node = 0; node < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++node)
        {
            // 3 sec delay to spawn new banner instead previous despawned one
            if (m_BannerTimers[node].timer) {
                if (m_BannerTimers[node].timer > diff)
                    m_BannerTimers[node].timer -= diff;
                else
                {
                    m_BannerTimers[node].timer = 0;
                    _CreateBanner(node, m_BannerTimers[node].type, m_BannerTimers[node].teamIndex, false);
                }
            }

            // 1-minute to occupy a node from contested state
            if (m_NodeTimers[node])
            {
                if (m_NodeTimers[node] > diff)
                    m_NodeTimers[node] -= diff;
                else
                {
                    m_NodeTimers[node] = 0;
                    // Change from contested to occupied !
                    uint8 teamIndex = m_Nodes[node] - 1;
                    m_prevNodes[node] = m_Nodes[node];
                    m_Nodes[node] += 2;
                    // burn current contested banner
                    _DelBanner(node, GILNEAS_BG_NODE_TYPE_CONTESTED, teamIndex);
                    // create new occupied banner
                    _CreateBanner(node, GILNEAS_BG_NODE_TYPE_OCCUPIED, teamIndex, true);
                    _SendNodeUpdate(node);
                    _NodeOccupied(node, (teamIndex == 0) ? ALLIANCE : HORDE);
                    // Message to chatlog

                    if (teamIndex == 0)
                    {
                        // FIXME: team and node names not localized
                        SendMessage2ToAll(LANG_BG_BFG_NODE_TAKEN, CHAT_MSG_BG_SYSTEM_ALLIANCE, NULL, LANG_BG_BFG_ALLY, _GetNodeNameId(node));
                        PlaySoundToAll(GILNEAS_BG_SOUND_NODE_CAPTURED_ALLIANCE);
                    }
                    else
                    {
                        // FIXME: team and node names not localized
                        SendMessage2ToAll(LANG_BG_BFG_NODE_TAKEN, CHAT_MSG_BG_SYSTEM_HORDE, NULL, LANG_BG_BFG_HORDE, _GetNodeNameId(node));
                        PlaySoundToAll(GILNEAS_BG_SOUND_NODE_CAPTURED_HORDE);
                    }
                }
            }

            for (int team = 0; team < BG_TEAMS_COUNT; ++team)
                if (m_Nodes[node] == team + GILNEAS_BG_NODE_TYPE_OCCUPIED)
                    ++team_points[team];
        }

        // Accumulate points
        for (int team = 0; team < BG_TEAMS_COUNT; ++team)
        {
            int points = team_points[team];
            if (!points)
                continue;

            m_lastTick[team] += diff;

            if (m_lastTick[team] > GILNEAS_BG_TickIntervals[points])
            {
                m_lastTick[team] -= GILNEAS_BG_TickIntervals[points];
                m_TeamScores[team] += GILNEAS_BG_TickPoints[points];
                m_HonorScoreTics[team] += GILNEAS_BG_TickPoints[points];
                m_ReputationScoreTics[team] += GILNEAS_BG_TickPoints[points];

                if (m_ReputationScoreTics[team] >= m_ReputationTics)
                {
                    (team == TEAM_ALLIANCE) ? RewardReputationToTeam(509, 10, ALLIANCE) : RewardReputationToTeam(510, 10, HORDE);
                    m_ReputationScoreTics[team] -= m_ReputationTics;
                }

                if (m_HonorScoreTics[team] >= m_HonorTics)
                {
                    RewardHonorToTeam(GetBonusHonorFromKill(1), (team == TEAM_ALLIANCE) ? ALLIANCE : HORDE);
                    m_HonorScoreTics[team] -= m_HonorTics;
                }

                if (!m_IsInformedNearVictory && m_TeamScores[team] > GILNEAS_BG_WARNING_NEAR_VICTORY_SCORE)
                {
                    if (team == TEAM_ALLIANCE)
                        SendMessageToAll(LANG_BG_BFG_A_NEAR_VICTORY, CHAT_MSG_BG_SYSTEM_NEUTRAL);
                    else
                        SendMessageToAll(LANG_BG_BFG_H_NEAR_VICTORY, CHAT_MSG_BG_SYSTEM_NEUTRAL);
                    PlaySoundToAll(GILNEAS_BG_SOUND_NEAR_VICTORY);
                    m_IsInformedNearVictory = true;
                }

                if (m_TeamScores[team] > GILNEAS_BG_MAX_TEAM_SCORE)
                    m_TeamScores[team] = GILNEAS_BG_MAX_TEAM_SCORE;

                if (team == TEAM_ALLIANCE)
                    UpdateWorldState(GILNEAS_BG_OP_RESOURCES_ALLY, m_TeamScores[team]);
                else if (team == TEAM_HORDE)
                    UpdateWorldState(GILNEAS_BG_OP_RESOURCES_HORDE, m_TeamScores[team]);
                // update achievement flags
                // we increased m_TeamScores[team] so we just need to check if it is 500 more than other teams resources
                uint8 otherTeam = (team + 1) % BG_TEAMS_COUNT;
                if (m_TeamScores[team] > m_TeamScores[otherTeam] + 500)
                    m_TeamScores500Disadvantage[otherTeam] = true;
            }
        }

        // Test win condition
        if (m_TeamScores[TEAM_ALLIANCE] >= GILNEAS_BG_MAX_TEAM_SCORE)
            EndBattleground(ALLIANCE);
        else if (m_TeamScores[TEAM_HORDE] >= GILNEAS_BG_MAX_TEAM_SCORE)
            EndBattleground(HORDE);
    }
}

void BattlegroundBFG::StartingEventCloseDoors()
{
    // despawn banners, auras and buffs
    for (int obj = GILNEAS_BG_OBJECT_BANNER_NEUTRAL; obj < GILNEAS_BG_DYNAMIC_NODES_COUNT * 8; ++obj)
        SpawnBGObject(obj, RESPAWN_ONE_DAY);
    for (int i = 0; i < GILNEAS_BG_DYNAMIC_NODES_COUNT * 3; ++i)
        SpawnBGObject(GILNEAS_BG_OBJECT_SPEEDBUFF_LIGHTHOUSE + i, RESPAWN_ONE_DAY);

    // Starting doors
    DoorClose(GILNEAS_BG_OBJECT_GATE_A_1);
    DoorClose(GILNEAS_BG_OBJECT_GATE_H_1);
    SpawnBGObject(GILNEAS_BG_OBJECT_GATE_A_1, RESPAWN_IMMEDIATELY);
    SpawnBGObject(GILNEAS_BG_OBJECT_GATE_H_1, RESPAWN_IMMEDIATELY);

    // Starting base spirit guides
    _NodeOccupied(GILNEAS_BG_SPIRIT_ALIANCE, ALLIANCE);
    _NodeOccupied(GILNEAS_BG_SPIRIT_HORDE, HORDE);
}

void BattlegroundBFG::StartingEventOpenDoors()
{
    // spawn neutral banners
    for (int banner = GILNEAS_BG_OBJECT_BANNER_NEUTRAL, i = 0; i < 3; banner += 8, ++i)
        SpawnBGObject(banner, RESPAWN_IMMEDIATELY);
    for (int i = 0; i < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++i)
    {
        //randomly select buff to spawn
        uint8 buff = urand(0, 2);
        SpawnBGObject(GILNEAS_BG_OBJECT_SPEEDBUFF_LIGHTHOUSE + buff + i * 3, RESPAWN_IMMEDIATELY);
    }
    DoorOpen(GILNEAS_BG_OBJECT_GATE_A_1);
    DoorOpen(GILNEAS_BG_OBJECT_GATE_H_1);

    // Achievement: Let's Get This Done
    StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, BG_EVENT_START_BATTLE);
}

void BattlegroundBFG::AddPlayer(Player* player)
{
    Battleground::AddPlayer(player);
    BattlegroundBFGScore* sc = new BattlegroundBFGScore;
    PlayerScores[player->GetGUID()] = sc;
}

void BattlegroundBFG::RemovePlayer(Player* /*player*/, uint64 /*guid*/, uint32 /*team*/) { }

void BattlegroundBFG::HandleAreaTrigger(Player* player, uint32 trigger) 
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    switch (trigger) {
    case 6447:                                          // Alliance start
        if (player->GetTeam() != ALLIANCE)
            player->GetSession()->SendAreaTriggerMessage("Only The Alliance can use that portal");
        else
            player->LeaveBattleground();
        break;
    case 6448:                                          // Horde start
        if (player->GetTeam() != HORDE)
            player->GetSession()->SendAreaTriggerMessage("Only The Horde can use that portal");
        else
            player->LeaveBattleground();
        break;
    case 6265:                                          // Waterworks heal
    case 6266:                                          // Mine speed
    case 6267:                                          // Waterworks speed
    case 6268:                                          // Mine berserk
    case 6269:                                          // Lighthouse berserk
        //break;
    default:
        Battleground::HandleAreaTrigger(player, trigger);
        break;
    }
}

void BattlegroundBFG::_CreateBanner(uint8 node, uint8 type, uint8 teamIndex, bool delay)
{
    // Just put it into the queue
    if (delay)
    {
        m_BannerTimers[node].timer = 2000;
        m_BannerTimers[node].type = type;
        m_BannerTimers[node].teamIndex = teamIndex;
        return;
    }

    uint8 obj = node * 8 + type + teamIndex;

    SpawnBGObject(obj, RESPAWN_IMMEDIATELY);

    // handle aura with banner
    if (!type)
        return;
    obj = node * 8 + ((type == GILNEAS_BG_NODE_TYPE_OCCUPIED) ? (3 + teamIndex) : 7);
    SpawnBGObject(obj, RESPAWN_IMMEDIATELY);
}

void BattlegroundBFG::_DelBanner(uint8 node, uint8 type, uint8 teamIndex)
{
    uint8 obj = node * 8 + type + teamIndex;
    SpawnBGObject(obj, RESPAWN_ONE_DAY);

    // handle aura with banner
    if (!type)
        return;
    obj = node * 8 + ((type == GILNEAS_BG_NODE_TYPE_OCCUPIED) ? (3 + teamIndex) : 7);
    SpawnBGObject(obj, RESPAWN_ONE_DAY);
}

int32 BattlegroundBFG::_GetNodeNameId(uint8 node)
{
    switch (node)
    {
        case GILNEAS_BG_NODE_LIGHTHOUSE: return LANG_BG_BFG_NODE_LIGHTHOUSE;
        case GILNEAS_BG_NODE_WATERWORKS: return LANG_BG_BFG_NODE_WATERWORKS;
        case GILNEAS_BG_NODE_MINE:       return LANG_BG_BFG_NODE_MINE;
        default:
            ASSERT(false);
    }
    return 0;
}

void BattlegroundBFG::FillInitialWorldStates(WorldStateBuilder& builder)
{
    const uint8 plusArray[] = { 0, 2, 3, 0, 1 };

    // Node icons
    for (uint8 node = 0; node < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++node)
        builder.AppendState(GILNEAS_BG_OP_NODEICONS[node], (m_Nodes[node] == 0) ? 1 : 0);

    // Node occupied states
    for (uint8 node = 0; node < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++node)
        for (uint8 i = 1; i < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++i)
            builder.AppendState(GILNEAS_BG_OP_NODESTATES[node] + plusArray[i], (m_Nodes[node] == i) ? 1 : 0);

    // How many bases each team owns
    uint8 ally = 0, horde = 0;
    for (uint8 node = 0; node < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++node)
        if (m_Nodes[node] == GILNEAS_BG_NODE_STATUS_ALLY_OCCUPIED)
            ++ally;
        else if (m_Nodes[node] == GILNEAS_BG_NODE_STATUS_HORDE_OCCUPIED)
            ++horde;

    builder.AppendState(GILNEAS_BG_OP_OCCUPIED_BASES_ALLY, ally);
    builder.AppendState(GILNEAS_BG_OP_OCCUPIED_BASES_HORDE, horde);

    // Team scores
    builder.AppendState(GILNEAS_BG_OP_RESOURCES_MAX, GILNEAS_BG_MAX_TEAM_SCORE);
    builder.AppendState(GILNEAS_BG_OP_RESOURCES_WARNING, GILNEAS_BG_WARNING_NEAR_VICTORY_SCORE);
    builder.AppendState(GILNEAS_BG_OP_RESOURCES_ALLY, m_TeamScores[TEAM_ALLIANCE]);
    builder.AppendState(GILNEAS_BG_OP_RESOURCES_HORDE, m_TeamScores[TEAM_HORDE]);

    // other unknown
    builder.AppendState(0x745, 0x2);           // 37 1861 unk
}

void BattlegroundBFG::_SendNodeUpdate(uint8 node)
{
    // Send node owner state update to refresh map icons on client
    const uint8 plusArray[] = { 0, 2, 3, 0, 1 };

    if (m_prevNodes[node])
        UpdateWorldState(GILNEAS_BG_OP_NODESTATES[node] + plusArray[m_prevNodes[node]], 0);
    else
        UpdateWorldState(GILNEAS_BG_OP_NODEICONS[node], 0);

    UpdateWorldState(GILNEAS_BG_OP_NODESTATES[node] + plusArray[m_Nodes[node]], 1);

    // How many bases each team owns
    uint8 ally = 0, horde = 0;
    for (uint8 i = 0; i < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++i)
        if (m_Nodes[i] == GILNEAS_BG_NODE_STATUS_ALLY_OCCUPIED)
            ++ally;
        else if (m_Nodes[i] == GILNEAS_BG_NODE_STATUS_HORDE_OCCUPIED)
            ++horde;

    UpdateWorldState(GILNEAS_BG_OP_OCCUPIED_BASES_ALLY, ally);
    UpdateWorldState(GILNEAS_BG_OP_OCCUPIED_BASES_HORDE, horde);
}

void BattlegroundBFG::_NodeOccupied(uint8 node, Team team)
{
    if (!AddSpiritGuide(node, GILNEAS_BG_SpiritGuidePos[node][0], GILNEAS_BG_SpiritGuidePos[node][1], GILNEAS_BG_SpiritGuidePos[node][2], GILNEAS_BG_SpiritGuidePos[node][3], team))
        SF_LOG_ERROR("bg.battleground", "Failed to spawn spirit guide! point: %u, team: %u, ", node, team);

    if (node >= GILNEAS_BG_DYNAMIC_NODES_COUNT)//only dynamic nodes, no start points
        return;

    uint8 capturedNodes = 0;
    for (uint8 i = 0; i < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++i)
        if (m_Nodes[i] == GetTeamIndexByTeamId(team) + GILNEAS_BG_NODE_TYPE_OCCUPIED && !m_NodeTimers[i])
            ++capturedNodes;

    Creature* trigger = BgCreatures[node + 5] ? GetBGCreature(node + 5) : NULL; // 0-5 spirit guides
    if (!trigger)
        trigger = AddCreature(WORLD_TRIGGER, node + 5, team, GILNEAS_BG_NodePositions[node][0], GILNEAS_BG_NodePositions[node][1], GILNEAS_BG_NodePositions[node][2], GILNEAS_BG_NodePositions[node][3]);

    //add bonus honor aura trigger creature when node is accupied
    //cast bonus aura (+50% honor in 25yards)
    //aura should only apply to players who have accupied the node, set correct faction for trigger
    if (trigger)
    {
        trigger->setFaction(team == ALLIANCE ? 84 : 83);
        trigger->CastSpell(trigger, SPELL_HONORABLE_DEFENDER_25Y, false);
    }
}

void BattlegroundBFG::_NodeDeOccupied(uint8 node)
{
    if (node >= GILNEAS_BG_DYNAMIC_NODES_COUNT)
        return;

    //remove bonus honor aura trigger creature when node is lost
    if (node < GILNEAS_BG_DYNAMIC_NODES_COUNT)//only dynamic nodes, no start points
        DelCreature(node + 5);//NULL checks are in DelCreature! 0-5 spirit guides

    RelocateDeadPlayers(BgCreatures[node]);

    DelCreature(node);

    // buff object isn't despawned
}

/* Invoked if a player used a banner as a gameobject */
void BattlegroundBFG::EventPlayerClickedOnFlag(Player* source, GameObject* /*target_obj*/)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    uint8 node = GILNEAS_BG_NODE_LIGHTHOUSE;
    GameObject* obj = GetBgMap()->GetGameObject(BgObjects[node * 8 + 5]);
    while ((node < GILNEAS_BG_DYNAMIC_NODES_COUNT) && ((!obj) || (!source->IsWithinDistInMap(obj, 10))))
    {
        ++node;
        obj = GetBgMap()->GetGameObject(BgObjects[node * 8 + GILNEAS_BG_OBJECT_AURA_CONTESTED]);
    }

    // this means our player isn't close to any of banners - maybe cheater ??
    if (node == GILNEAS_BG_DYNAMIC_NODES_COUNT)
        return;

    TeamId teamIndex = GetTeamIndexByTeamId(source->GetTeam());

    // Check if player really could use this banner, not cheated
    if (!(m_Nodes[node] == 0 || teamIndex == m_Nodes[node] % 2))
        return;

    source->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_ENTER_PVP_COMBAT);
    uint32 sound = 0;
    // If node is neutral, change to contested
    if (m_Nodes[node] == GILNEAS_BG_NODE_TYPE_NEUTRAL)
    {
        UpdatePlayerScore(source, SCORE_BASES_ASSAULTED, 1);
        m_prevNodes[node] = m_Nodes[node];
        m_Nodes[node] = teamIndex + 1;
        // burn current neutral banner
        _DelBanner(node, GILNEAS_BG_NODE_TYPE_NEUTRAL, 0);
        // create new contested banner
        _CreateBanner(node, GILNEAS_BG_NODE_TYPE_CONTESTED, teamIndex, true);
        _SendNodeUpdate(node);
        m_NodeTimers[node] = GILNEAS_BG_FLAG_CAPTURING_TIME;

        // FIXME: team and node names not localized
        if (teamIndex == 0)
            SendMessage2ToAll(LANG_BG_BFG_NODE_CLAIMED, CHAT_MSG_BG_SYSTEM_ALLIANCE, source, _GetNodeNameId(node), LANG_BG_BFG_ALLY);
        else
            SendMessage2ToAll(LANG_BG_BFG_NODE_CLAIMED, CHAT_MSG_BG_SYSTEM_HORDE, source, _GetNodeNameId(node), LANG_BG_BFG_HORDE);

        sound = GILNEAS_BG_SOUND_NODE_CLAIMED;
    }
    // If node is contested
    else if ((m_Nodes[node] == GILNEAS_BG_NODE_STATUS_ALLY_CONTESTED) || (m_Nodes[node] == GILNEAS_BG_NODE_STATUS_HORDE_CONTESTED))
    {
        // If last state is NOT occupied, change node to enemy-contested
        if (m_prevNodes[node] < GILNEAS_BG_NODE_TYPE_OCCUPIED) {
            UpdatePlayerScore(source, SCORE_BASES_ASSAULTED, 1);
            m_prevNodes[node] = m_Nodes[node];
            m_Nodes[node] = teamIndex + GILNEAS_BG_NODE_TYPE_CONTESTED;
            // burn current contested banner
            _DelBanner(node, GILNEAS_BG_NODE_TYPE_CONTESTED, !teamIndex);
            // create new contested banner
            _CreateBanner(node, GILNEAS_BG_NODE_TYPE_CONTESTED, teamIndex, true);
            _SendNodeUpdate(node);
            m_NodeTimers[node] = GILNEAS_BG_FLAG_CAPTURING_TIME;

            // FIXME: node names not localized
            if (teamIndex == TEAM_ALLIANCE)
                SendMessage2ToAll(LANG_BG_BFG_NODE_ASSAULTED, CHAT_MSG_BG_SYSTEM_ALLIANCE, source, _GetNodeNameId(node));
            else
                SendMessage2ToAll(LANG_BG_BFG_NODE_ASSAULTED, CHAT_MSG_BG_SYSTEM_HORDE, source, _GetNodeNameId(node));
        }
        // If contested, change back to occupied
        else
        {
            UpdatePlayerScore(source, SCORE_BASES_DEFENDED, 1);
            m_prevNodes[node] = m_Nodes[node];
            m_Nodes[node] = teamIndex + GILNEAS_BG_NODE_TYPE_OCCUPIED;
            // burn current contested banner
            _DelBanner(node, GILNEAS_BG_NODE_TYPE_CONTESTED, !teamIndex);
            // create new occupied banner
            _CreateBanner(node, GILNEAS_BG_NODE_TYPE_OCCUPIED, teamIndex, true);
            _SendNodeUpdate(node);
            m_NodeTimers[node] = 0;
            _NodeOccupied(node, (teamIndex == TEAM_ALLIANCE) ? ALLIANCE : HORDE);

            // FIXME: node names not localized
            if (teamIndex == TEAM_ALLIANCE)
                SendMessage2ToAll(LANG_BG_BFG_NODE_DEFENDED, CHAT_MSG_BG_SYSTEM_ALLIANCE, source, _GetNodeNameId(node));
            else
                SendMessage2ToAll(LANG_BG_BFG_NODE_DEFENDED, CHAT_MSG_BG_SYSTEM_HORDE, source, _GetNodeNameId(node));
        }
        sound = (teamIndex == TEAM_ALLIANCE) ? GILNEAS_BG_SOUND_NODE_ASSAULTED_ALLIANCE : GILNEAS_BG_SOUND_NODE_ASSAULTED_HORDE;
    }
    // If node is occupied, change to enemy-contested
    else
    {
        UpdatePlayerScore(source, SCORE_BASES_ASSAULTED, 1);
        m_prevNodes[node] = m_Nodes[node];
        m_Nodes[node] = teamIndex + GILNEAS_BG_NODE_TYPE_CONTESTED;
        // burn current occupied banner
        _DelBanner(node, GILNEAS_BG_NODE_TYPE_OCCUPIED, !teamIndex);
        // create new contested banner
        _CreateBanner(node, GILNEAS_BG_NODE_TYPE_CONTESTED, teamIndex, true);
        _SendNodeUpdate(node);
        _NodeDeOccupied(node);
        m_NodeTimers[node] = GILNEAS_BG_FLAG_CAPTURING_TIME;

        // FIXME: node names not localized
        if (teamIndex == TEAM_ALLIANCE)
            SendMessage2ToAll(LANG_BG_BFG_NODE_ASSAULTED, CHAT_MSG_BG_SYSTEM_ALLIANCE, source, _GetNodeNameId(node));
        else
            SendMessage2ToAll(LANG_BG_BFG_NODE_ASSAULTED, CHAT_MSG_BG_SYSTEM_HORDE, source, _GetNodeNameId(node));

        sound = (teamIndex == TEAM_ALLIANCE) ? GILNEAS_BG_SOUND_NODE_ASSAULTED_ALLIANCE : GILNEAS_BG_SOUND_NODE_ASSAULTED_HORDE;
    }

    // If node is occupied again, send "X has taken the Y" msg.
    if (m_Nodes[node] >= GILNEAS_BG_NODE_TYPE_OCCUPIED)
    {
        // FIXME: team and node names not localized
        if (teamIndex == TEAM_ALLIANCE)
            SendMessage2ToAll(LANG_BG_BFG_NODE_TAKEN, CHAT_MSG_BG_SYSTEM_ALLIANCE, NULL, LANG_BG_BFG_ALLY, _GetNodeNameId(node));
        else
            SendMessage2ToAll(LANG_BG_BFG_NODE_TAKEN, CHAT_MSG_BG_SYSTEM_HORDE, NULL, LANG_BG_BFG_HORDE, _GetNodeNameId(node));
    }
    PlaySoundToAll(sound);
}

uint32 BattlegroundBFG::GetPrematureWinner()
{
    // How many bases each team owns
    uint8 ally = 0, horde = 0;
    for (uint8 i = 0; i < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++i)
        if (m_Nodes[i] == GILNEAS_BG_NODE_STATUS_ALLY_OCCUPIED)
            ++ally;
        else if (m_Nodes[i] == GILNEAS_BG_NODE_STATUS_HORDE_OCCUPIED)
            ++horde;

    if (ally > horde)
        return ALLIANCE;
    else if (horde > ally)
        return HORDE;

    // If the values are equal, fall back to the original result (based on number of players on each team)
    return Battleground::GetPrematureWinner();
}

bool BattlegroundBFG::SetupBattleground() {
    for (int i = 0; i < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++i)
    {
        if (!AddObject(GILNEAS_BG_OBJECT_BANNER_NEUTRAL + 8 * i, GILNEAS_BG_OBJECTID_NODE_BANNER_0 + i, GILNEAS_BG_NodePositions[i][0], GILNEAS_BG_NodePositions[i][1], GILNEAS_BG_NodePositions[i][2], GILNEAS_BG_NodePositions[i][3], 0, 0, std::sin(GILNEAS_BG_NodePositions[i][3] / 2), std::cos(GILNEAS_BG_NodePositions[i][3] / 2), RESPAWN_ONE_DAY)
            || !AddObject(GILNEAS_BG_OBJECT_BANNER_CONT_A + 8 * i, GILNEAS_BG_OBJECTID_BANNER_CONT_A, GILNEAS_BG_NodePositions[i][0], GILNEAS_BG_NodePositions[i][1], GILNEAS_BG_NodePositions[i][2], GILNEAS_BG_NodePositions[i][3], 0, 0, std::sin(GILNEAS_BG_NodePositions[i][3] / 2), std::cos(GILNEAS_BG_NodePositions[i][3] / 2), RESPAWN_ONE_DAY)
            || !AddObject(GILNEAS_BG_OBJECT_BANNER_CONT_H + 8 * i, GILNEAS_BG_OBJECTID_BANNER_CONT_H, GILNEAS_BG_NodePositions[i][0], GILNEAS_BG_NodePositions[i][1], GILNEAS_BG_NodePositions[i][2], GILNEAS_BG_NodePositions[i][3], 0, 0, std::sin(GILNEAS_BG_NodePositions[i][3] / 2), std::cos(GILNEAS_BG_NodePositions[i][3] / 2), RESPAWN_ONE_DAY)
            || !AddObject(GILNEAS_BG_OBJECT_BANNER_ALLY + 8 * i, GILNEAS_BG_OBJECTID_BANNER_A, GILNEAS_BG_NodePositions[i][0], GILNEAS_BG_NodePositions[i][1], GILNEAS_BG_NodePositions[i][2], GILNEAS_BG_NodePositions[i][3], 0, 0, std::sin(GILNEAS_BG_NodePositions[i][3] / 2), std::cos(GILNEAS_BG_NodePositions[i][3] / 2), RESPAWN_ONE_DAY)
            || !AddObject(GILNEAS_BG_OBJECT_BANNER_HORDE + 8 * i, GILNEAS_BG_OBJECTID_BANNER_H, GILNEAS_BG_NodePositions[i][0], GILNEAS_BG_NodePositions[i][1], GILNEAS_BG_NodePositions[i][2], GILNEAS_BG_NodePositions[i][3], 0, 0, std::sin(GILNEAS_BG_NodePositions[i][3] / 2), std::cos(GILNEAS_BG_NodePositions[i][3] / 2), RESPAWN_ONE_DAY)
            || !AddObject(GILNEAS_BG_OBJECT_AURA_ALLY + 8 * i, GILNEAS_BG_OBJECTID_AURA_A, GILNEAS_BG_NodePositions[i][0], GILNEAS_BG_NodePositions[i][1], GILNEAS_BG_NodePositions[i][2], GILNEAS_BG_NodePositions[i][3], 0, 0, std::sin(GILNEAS_BG_NodePositions[i][3] / 2), std::cos(GILNEAS_BG_NodePositions[i][3] / 2), RESPAWN_ONE_DAY)
            || !AddObject(GILNEAS_BG_OBJECT_AURA_HORDE + 8 * i, GILNEAS_BG_OBJECTID_AURA_H, GILNEAS_BG_NodePositions[i][0], GILNEAS_BG_NodePositions[i][1], GILNEAS_BG_NodePositions[i][2], GILNEAS_BG_NodePositions[i][3], 0, 0, std::sin(GILNEAS_BG_NodePositions[i][3] / 2), std::cos(GILNEAS_BG_NodePositions[i][3] / 2), RESPAWN_ONE_DAY)
            || !AddObject(GILNEAS_BG_OBJECT_AURA_CONTESTED + 8 * i, GILNEAS_BG_OBJECTID_AURA_C, GILNEAS_BG_NodePositions[i][0], GILNEAS_BG_NodePositions[i][1], GILNEAS_BG_NodePositions[i][2], GILNEAS_BG_NodePositions[i][3], 0, 0, std::sin(GILNEAS_BG_NodePositions[i][3] / 2), std::cos(GILNEAS_BG_NodePositions[i][3] / 2), RESPAWN_ONE_DAY))
        {
            SF_LOG_ERROR("sql.sql", "BatteGroundAB: Failed to spawn some object Battleground not created!");
            return false;
        }
    }

    if (!AddObject(GILNEAS_BG_OBJECT_GATE_A_1, GILNEAS_BG_OBJECTID_GATE_A_1, GILNEAS_BG_DoorPositions[0][0], GILNEAS_BG_DoorPositions[0][1], GILNEAS_BG_DoorPositions[0][2], GILNEAS_BG_DoorPositions[0][3], GILNEAS_BG_DoorPositions[0][4], GILNEAS_BG_DoorPositions[0][5], GILNEAS_BG_DoorPositions[0][6], GILNEAS_BG_DoorPositions[0][7], RESPAWN_IMMEDIATELY)
        || !AddObject(GILNEAS_BG_OBJECT_GATE_H_1, GILNEAS_BG_OBJECTID_GATE_H_1, GILNEAS_BG_DoorPositions[2][0], GILNEAS_BG_DoorPositions[2][1], GILNEAS_BG_DoorPositions[2][2], GILNEAS_BG_DoorPositions[2][3], GILNEAS_BG_DoorPositions[2][4], GILNEAS_BG_DoorPositions[2][5], GILNEAS_BG_DoorPositions[2][6], GILNEAS_BG_DoorPositions[2][7], RESPAWN_IMMEDIATELY))
    {
        SF_LOG_ERROR("sql.sql", "BatteGroundBGF: Failed to spawn door object Battleground not created!");
        return false;
    }

    // Buffs
    for (int i = 0; i < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++i)
    {
        if (!AddObject(GILNEAS_BG_OBJECT_SPEEDBUFF_LIGHTHOUSE + 3 * i, Buff_Entries[0], GILNEAS_BG_BuffPositions[i][0], GILNEAS_BG_BuffPositions[i][1], GILNEAS_BG_BuffPositions[i][2], GILNEAS_BG_BuffPositions[i][3], 0, 0, sin(GILNEAS_BG_BuffPositions[i][3] / 2), cos(GILNEAS_BG_BuffPositions[i][3] / 2), RESPAWN_ONE_DAY)
            || !AddObject(GILNEAS_BG_OBJECT_SPEEDBUFF_LIGHTHOUSE + 3 * i + 1, Buff_Entries[1], GILNEAS_BG_BuffPositions[i][0], GILNEAS_BG_BuffPositions[i][1], GILNEAS_BG_BuffPositions[i][2], GILNEAS_BG_BuffPositions[i][3], 0, 0, sin(GILNEAS_BG_BuffPositions[i][3] / 2), cos(GILNEAS_BG_BuffPositions[i][3] / 2), RESPAWN_ONE_DAY)
            || !AddObject(GILNEAS_BG_OBJECT_SPEEDBUFF_LIGHTHOUSE + 3 * i + 2, Buff_Entries[2], GILNEAS_BG_BuffPositions[i][0], GILNEAS_BG_BuffPositions[i][1], GILNEAS_BG_BuffPositions[i][2], GILNEAS_BG_BuffPositions[i][3], 0, 0, sin(GILNEAS_BG_BuffPositions[i][3] / 2), cos(GILNEAS_BG_BuffPositions[i][3] / 2), RESPAWN_ONE_DAY))
            SF_LOG_ERROR("sql.sql", "BatteGroundBGF: Failed to spawn buff object!");
    }

    return true;
}

void BattlegroundBFG::Reset()
{
    //call parent's class reset
    Battleground::Reset();

    m_TeamScores[TEAM_ALLIANCE] = 0;
    m_TeamScores[TEAM_HORDE] = 0;
    m_lastTick[TEAM_ALLIANCE] = 0;
    m_lastTick[TEAM_HORDE] = 0;
    m_HonorScoreTics[TEAM_ALLIANCE] = 0;
    m_HonorScoreTics[TEAM_HORDE] = 0;
    m_ReputationScoreTics[TEAM_ALLIANCE] = 0;
    m_ReputationScoreTics[TEAM_HORDE] = 0;
    m_IsInformedNearVictory = false;
    bool isBGWeekend = sBattlegroundMgr->IsBGWeekend(GetTypeID());
    m_HonorTics = (isBGWeekend) ? GILNEAS_BG_BGWeekendHonorTicks : GILNEAS_BG_NotBGWeekendHonorTicks;
    m_ReputationTics = (isBGWeekend) ? GILNEAS_BG_BGWeekendRepTicks : GILNEAS_BG_NotBGWeekendRepTicks;
    m_TeamScores500Disadvantage[TEAM_ALLIANCE] = false;
    m_TeamScores500Disadvantage[TEAM_HORDE] = false;

    for (uint8 i = 0; i < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++i)
    {
        m_Nodes[i] = 0;
        m_prevNodes[i] = 0;
        m_NodeTimers[i] = 0;
        m_BannerTimers[i].timer = 0;
    }

    for (uint8 i = 0; i < GILNEAS_BG_ALL_NODES_COUNT + 3; ++i)//+3 for aura triggers
        if (BgCreatures[i])
            DelCreature(i);
}

void BattlegroundBFG::EndBattleground(uint32 winner)
{
    // Win reward
    if (winner == ALLIANCE)
        RewardHonorToTeam(GetBonusHonorFromKill(1), ALLIANCE);
    if (winner == HORDE)
        RewardHonorToTeam(GetBonusHonorFromKill(1), HORDE);
    // Complete map_end rewards (even if no team wins)
    RewardHonorToTeam(GetBonusHonorFromKill(1), HORDE);
    RewardHonorToTeam(GetBonusHonorFromKill(1), ALLIANCE);

    Battleground::EndBattleground(winner);
}

WorldSafeLocsEntry const* BattlegroundBFG::GetClosestGraveYard(Player* player)
{
    TeamId teamIndex = GetTeamIndexByTeamId(player->GetTeam());

    // Is there any occupied node for this team?
    std::vector<uint8> nodes;
    for (uint8 i = 0; i < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++i)
        if (m_Nodes[i] == teamIndex + 3)
            nodes.push_back(i);

    WorldSafeLocsEntry const* good_entry = NULL;
    // If so, select the closest node to place ghost on
    if (!nodes.empty()) {
        float plr_x = player->GetPositionX();
        float plr_y = player->GetPositionY();

        float mindist = 999999.0f;
        for (uint8 i = 0; i < nodes.size(); ++i) {
            WorldSafeLocsEntry const*entry = sWorldSafeLocsStore.LookupEntry(GILNEAS_BG_GraveyardIds[nodes[i]]);
            if (!entry)
                continue;
            float dist = (entry->x - plr_x)*(entry->x - plr_x) + (entry->y - plr_y)*(entry->y - plr_y);
            if (mindist > dist) {
                mindist = dist;
                good_entry = entry;
            }
        }
        nodes.clear();
    }
    // If not, place ghost on starting location
    if (!good_entry)
        good_entry = sWorldSafeLocsStore.LookupEntry(GILNEAS_BG_GraveyardIds[teamIndex + 5]);

    return good_entry;
}

/*WorldSafeLocsEntry const* BattlegroundBFG::GetExploitTeleportLocation(Team team)
{
    return sWorldSafeLocsStore.LookupEntry(team == ALLIANCE ? GILNEAS_EXPLOIT_TELEPORT_LOCATION_ALLIANCE : GILNEAS_EXPLOIT_TELEPORT_LOCATION_HORDE);
}*/

void BattlegroundBFG::UpdatePlayerScore(Player* Source, uint32 type, uint32 value, bool doAddHonor)
{
    BattlegroundScoreMap::iterator itr = PlayerScores.find(Source->GetGUID());
    if (itr == PlayerScores.end())                         // player not found...
        return;

    switch (type)
    {
        case SCORE_BASES_ASSAULTED:
            ((BattlegroundBFGScore*)itr->second)->BasesAssaulted += value;
            Source->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE, BG_OBJECTIVE_ASSAULT_BASE);
            break;
        case SCORE_BASES_DEFENDED:
            ((BattlegroundBFGScore*)itr->second)->BasesDefended += value;
            Source->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE, BG_OBJECTIVE_DEFEND_BASE);
            break;
        default:
            Battleground::UpdatePlayerScore(Source, type, value, doAddHonor);
            break;
    }
}

bool BattlegroundBFG::IsAllNodesControlledByTeam(uint32 team) const
{
    uint32 count = 0;
    for (int i = 0; i < GILNEAS_BG_DYNAMIC_NODES_COUNT; ++i)
        if ((team == ALLIANCE && m_Nodes[i] == GILNEAS_BG_NODE_STATUS_ALLY_OCCUPIED) ||
            (team == HORDE    && m_Nodes[i] == GILNEAS_BG_NODE_STATUS_HORDE_OCCUPIED))
            ++count;

    return count == GILNEAS_BG_DYNAMIC_NODES_COUNT;
}

bool BattlegroundBFG::CheckAchievementCriteriaMeet(uint32 criteriaId, Player const* player, Unit const* target, uint32 miscvalue)
{
    switch (criteriaId)
    {
        case BG_CRITERIA_CHECK_RESILIENT_VICTORY:
            return m_TeamScores500Disadvantage[GetTeamIndexByTeamId(player->GetTeam())];
    }

    return Battleground::CheckAchievementCriteriaMeet(criteriaId, player, target, miscvalue);
}
