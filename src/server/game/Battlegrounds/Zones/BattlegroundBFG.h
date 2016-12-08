/*
 * Copyright (C) 2011-2016 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2016 MaNGOS <http://getmangos.com/>
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

#ifndef SF_BATTLEGROUNDBFG_H
#define SF_BATTLEGROUNDBFG_H

#include "Battleground.h"

enum BG_BFG_WORLDSTATES
{
    BG_BFG_OP_OCCUPIED_BASES_HORDE = 1778,
    BG_BFG_OP_OCCUPIED_BASES_ALLY = 1779,
    BG_BFG_OP_RESOURCES_ALLY = 1776,
    BG_BFG_OP_RESOURCES_HORDE = 1777,
    BG_BFG_OP_RESOURCES_MAX = 1780,
    BG_BFG_OP_RESOURCES_WARNING = 1955
};

const uint32 BG_BFG_OP_NODESTATES[3] = { 1767, 1782, 1772 }; // from AB as PH

const uint32 BG_BFG_OP_NODEICONS[3] = { 1842, 1846, 1845 }; // from AB as PH

enum BG_BFG_NodeObjectId
{
    BG_BFG_OBJECTID_NODE_BANNER_0 = 180087,       // Mines
    BG_BFG_OBJECTID_NODE_BANNER_1 = 180088,       // Lighthouse
    BG_BFG_OBJECTID_NODE_BANNER_2 = 180089,       // Waterworks
};

enum BG_BFG_ObjectType
{
    // for all 3 node points 8*3=24 objects
    BG_BFG_OBJECT_BANNER_NEUTRAL = 0,
    BG_BFG_OBJECT_BANNER_CONT_A = 1,
    BG_BFG_OBJECT_BANNER_CONT_H = 2,
    BG_BFG_OBJECT_BANNER_ALLY = 3,
    BG_BFG_OBJECT_BANNER_HORDE = 4,
    BG_BFG_OBJECT_AURA_ALLY = 5,
    BG_BFG_OBJECT_AURA_HORDE = 6,
    BG_BFG_OBJECT_AURA_CONTESTED = 7,
    //gates
    BG_BFG_OBJECT_GATE_A = 40,
    BG_BFG_OBJECT_GATE_H = 41,
    //buffs
    BG_BFG_OBJECT_SPEEDBUFF_LIGHTHOUSE = 42,
    BG_BFG_OBJECT_REGENBUFF_LIGHTHOUSE = 43,
    BG_BFG_OBJECT_BERSERKBUFF_LIGHTHOUSE = 44,
    BG_BFG_OBJECT_SPEEDBUFF_MINES = 45,
    BG_BFG_OBJECT_REGENBUFF_MINES = 46,
    BG_BFG_OBJECT_BERSERKBUFF_MINES = 47,
    BG_BFG_OBJECT_SPEEDBUFF_WATERWORKS = 48,
    BG_BFG_OBJECT_REGENBUFF_WATERWORKS = 49,
    BG_BFG_OBJECT_BERSERKBUFF_WATERWORKS = 50,
    BG_BFG_OBJECT_MAX = 51
};

/* Object id templates from DB */
enum BG_BFG_ObjectTypes
{
    BG_BFG_OBJECTID_BANNER_A = 180058,
    BG_BFG_OBJECTID_BANNER_CONT_A = 180059,
    BG_BFG_OBJECTID_BANNER_H = 180060,
    BG_BFG_OBJECTID_BANNER_CONT_H = 180061,

    BG_BFG_OBJECTID_AURA_A = 180100,
    BG_BFG_OBJECTID_AURA_H = 180101,
    BG_BFG_OBJECTID_AURA_C = 180102,

    BG_BFG_OBJECTID_GATE_A = 180255,
    BG_BFG_OBJECTID_GATE_H = 180256
};

enum BG_BFG_Timers
{
    BG_BFG_FLAG_CAPTURING_TIME = 60000
};

enum BG_BFG_Score
{
    BG_BFG_WARNING_NEAR_VICTORY_SCORE = 1800,
    BG_BFG_MAX_TEAM_SCORE = 2000
};

/* do NOT change the order, else wrong behaviour */
enum BG_BFG_BattlegroundNodes
{
    BG_BFG_NODE_LIGHTHOUSE = 0,
    BG_BFG_NODE_MINES = 1,
    BG_BFG_NODE_WATERWORKS = 2,

    BG_BFG_DYNAMIC_NODES_COUNT = 3,                        // dynamic nodes that can be captured

    BG_BFG_SPIRIT_ALIANCE = 4,
    BG_BFG_SPIRIT_HORDE = 5,

    BG_BFG_ALL_NODES_COUNT = 6                         // all nodes (dynamic and static)
};

enum BG_BFG_NodeStatus
{
    BG_BFG_NODE_TYPE_NEUTRAL = 0,
    BG_BFG_NODE_TYPE_CONTESTED = 1,
    BG_BFG_NODE_STATUS_ALLY_CONTESTED = 1,
    BG_BFG_NODE_STATUS_HORDE_CONTESTED = 2,
    BG_BFG_NODE_TYPE_OCCUPIED = 3,
    BG_BFG_NODE_STATUS_ALLY_OCCUPIED = 3,
    BG_BFG_NODE_STATUS_HORDE_OCCUPIED = 4
};

enum BG_BFG_Sounds
{
    BG_BFG_SOUND_NODE_CLAIMED = 8192,
    BG_BFG_SOUND_NODE_CAPTURED_ALLIANCE = 8173,
    BG_BFG_SOUND_NODE_CAPTURED_HORDE = 8213,
    BG_BFG_SOUND_NODE_ASSAULTED_ALLIANCE = 8212,
    BG_BFG_SOUND_NODE_ASSAULTED_HORDE = 8174,
    BG_BFG_SOUND_NEAR_VICTORY = 8456
};

enum BG_BFG_Objectives
{
    BFG_OBJECTIVE_ASSAULT_BASE = 122,
    BFG_OBJECTIVE_DEFEND_BASE = 123
};

#define BG_BFG_NotBFGBGWeekendHonorTicks      260
#define BG_BFG_BFGBGWeekendHonorTicks         160
#define BG_BFG_NotBFGBGWeekendReputationTicks 160
#define BG_BFG_BFGBGWeekendReputationTicks    120

 
////////////////////////////////////
//                                //
//  TAKEN FROM AB AS PLACEHOLDER  //
//          DO NOT USE            //
//      COORDS INCORRECT          //
////////////////////////////////////
// x, y, z, o
const float BG_BFG_NodePositions[BG_BFG_DYNAMIC_NODES_COUNT][4] =
{
    { 1166.785f, 1200.132f, -56.70859f, 0.9075713f },         // Lighthouse
    { 977.0156f, 1046.616f, -44.80923f, -2.600541f },         // Mines
    { 806.1821f, 874.2723f, -55.99371f, -2.303835f },         // Waterworks
};

// x, y, z, o, rot0, rot1, rot2, rot3
const float BG_BFG_DoorPositions[2][8] =
{
    { 1284.597f, 1281.167f, -15.97792f, 0.7068594f, 0.012957f, -0.060288f, 0.344959f, 0.93659f },
    { 708.0903f, 708.4479f, -17.8342f, -2.391099f, 0.050291f, 0.015127f, 0.929217f, -0.365784f }
};

// Tick intervals and given points: case 0, 1,     2,    3 captured nodes
const uint32 BG_BFG_TickIntervals[6] = { 0, 12000, 9000, 6000 };
const uint32 BG_BFG_TickPoints[6] =    { 0, 10,    10,   30 };

// WorldSafeLocs ids for 3 nodes, and for ally, and horde starting location
const uint32 BG_BFG_GraveyardIds[BG_BFG_ALL_NODES_COUNT] = { 893, 894, 895, 896, 897 };

// x, y, z, o
const float BG_BFG_BuffPositions[BG_BFG_DYNAMIC_NODES_COUNT][4] =
{
    { 1185.71f, 1185.24f, -56.36f, 2.56f },                   // Lighthouse
    { 990.75f, 1008.18f, -42.60f, 2.43f },                    // Mines
    { 817.66f, 843.34f, -56.54f, 3.01f },                     // Waterworks
};

// x, y, z, o
const float BG_BFG_SpiritGuidePos[BG_BFG_ALL_NODES_COUNT][4] =
{
    { 1200.03f, 1171.09f, -56.47f, 5.15f },                   // Lighthouse
    { 1017.43f, 960.61f, -42.95f, 4.88f },                    // Mines
    { 833.00f, 793.00f, -57.25f, 5.27f },                     // Waterworks
    { 1354.05f, 1275.48f, -11.30f, 4.77f },                   // alliance starting base
    { 714.61f, 646.15f, -10.87f, 4.34f }                      // horde starting base
};

struct BG_BFG_BannerTimer
{
    uint32      timer;
    uint8       type;
    uint8       teamIndex;
};

class BattlegroundBFGScore : public BattlegroundScore
{
    public:
        BattlegroundBFGScore(): BasesAssaulted(0), BasesDefended(0) {};
        virtual ~BattlegroundBFGScore() {};

        uint32 BasesAssaulted;
        uint32 BasesDefended;
};

class BattlegroundBFG : public Battleground
{
    public:
        BattlegroundBFG();
        ~BattlegroundBFG();

        
        void AddPlayer(Player* player);
        void StartingEventCloseDoors();
        void StartingEventOpenDoors();
        void RemovePlayer(Player* player, uint64 guid, uint32 team);
        void HandleAreaTrigger(Player* Source, uint32 Trigger);
        bool SetupBattleground();
        void Reset();
        void EndBattleground(uint32 winner);
        WorldSafeLocsEntry const* GetClosestGraveYard(Player* player);

        /* Scorekeeping */
        void UpdatePlayerScore(Player* Source, uint32 type, uint32 value, bool doAddHonor = true);

        void FillInitialWorldStates(WorldStateBuilder& builder);

        /* Nodes occupying */
        void EventPlayerClickedOnFlag(Player* source, GameObject* target_obj);

        /* achievement req. */
        bool IsAllNodesControlledByTeam(uint32 team) const;
        bool CheckAchievementCriteriaMeet(uint32 /*criteriaId*/, Player const* /*player*/, Unit const* /*target*/ = NULL, uint32 /*miscvalue1*/ = 0);

        uint32 GetPrematureWinner();
    private:
        void PostUpdateImpl(uint32 diff);
        /* Gameobject spawning/despawning */
        void _CreateBanner(uint8 node, uint8 type, uint8 teamIndex, bool delay);
        void _DelBanner(uint8 node, uint8 type, uint8 teamIndex);
        void _SendNodeUpdate(uint8 node);

        /* Creature spawning/despawning */
        /// @todo working, scripted peons spawning
        void _NodeOccupied(uint8 node, Team team);
        void _NodeDeOccupied(uint8 node);

        int32 _GetNodeNameId(uint8 node);

        /* Nodes info:
        0: neutral
        1: ally contested
        2: horde contested
        3: ally occupied
        4: horde occupied     */
        uint8               m_Nodes[BG_BFG_DYNAMIC_NODES_COUNT];
        uint8               m_prevNodes[BG_BFG_DYNAMIC_NODES_COUNT];
        BG_BFG_BannerTimer   m_BannerTimers[BG_BFG_DYNAMIC_NODES_COUNT];
        uint32              m_NodeTimers[BG_BFG_DYNAMIC_NODES_COUNT];
        uint32              m_lastTick[BG_TEAMS_COUNT];
        uint32              m_HonorScoreTics[BG_TEAMS_COUNT];
        uint32              m_ReputationScoreTics[BG_TEAMS_COUNT];
        bool                m_IsInformedNearVictory;
        uint32              m_HonorTics;
        uint32              m_ReputationTics;
        // need for achievements
        bool                m_TeamScores500Disadvantage[BG_TEAMS_COUNT];
};

#endif
