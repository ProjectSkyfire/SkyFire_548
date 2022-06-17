/*
 * Copyright (C) 2011-2021 Project SkyFire <https://www.projectskyfire.org/>
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

#ifndef SF_BATTLEGROUND_VOP_H
#define SF_BATTLEGROUND_VOP_H

#include "Battleground.h"

#define BG_VOP_MAX_TEAM_SCORE 1600

enum BG_VOP_Spells
{
    BG_VOP_SPELL_ORB_PICKED_UP_1 = 121164,
    BG_VOP_SPELL_ORB_PICKED_UP_2 = 121175,
    BG_VOP_SPELL_ORB_PICKED_UP_3 = 121176,
    BG_VOP_SPELL_ORB_PICKED_UP_4 = 121177,

    BG_VOP_SPELL_ORB_AURA_1 = 121217,
    BG_VOP_SPELL_ORB_AURA_2 = 121219,
    BG_VOP_SPELL_ORB_AURA_3 = 121220,
    BG_VOP_SPELL_ORB_AURA_4 = 121221
};

enum BG_VOP_Objects
{
    BG_VOP_OBJECT_DOOR_A = 0,
    BG_VOP_OBJECT_DOOR_H = 1,
    BG_VOP_OBJECT_ORB_1 = 2,
    BG_VOP_OBJECT_ORB_2 = 3,
    BG_VOP_OBJECT_ORB_3 = 4,
    BG_VOP_OBJECT_ORB_4 = 5,
    BG_VOP_OBJECT_BUFF_NORTH = 6,
    BG_VOP_OBJECT_BUFF_SOUTH = 7,
    BG_VOP_OBJECT_MAX = 8
};

enum BG_VOP_Creatures
{
    BG_VOP_CREATURE_ORB_AURA_1 = 0,
    BG_VOP_CREATURE_ORB_AURA_2 = 1,
    BG_VOP_CREATURE_ORB_AURA_3 = 2,
    BG_VOP_CREATURE_ORB_AURA_4 = 3,
    BG_VOP_CREATURE_SPIRIT_1 = 4,
    BG_VOP_CREATURE_SPIRIT_2 = 5,
    BG_VOP_CREATURE_MAX = 6
};

enum BG_VOP_Objets_Entry
{
    BG_VOP_OBJECT_DOOR_ENTRY = 213172,
    BG_VOP_OBJECT_ORB_1_ENTRY = 212091,
    BG_VOP_OBJECT_ORB_2_ENTRY = 212092,
    BG_VOP_OBJECT_ORB_3_ENTRY = 212093,
    BG_VOP_OBJECT_ORB_4_ENTRY = 212094,
};
enum BG_VOP_WorldStates
{
    BG_VOP_OP_RESOURCES_MAX = 1780,
    BG_VOP_OP_RESOURCES_A = 6303,
    BG_VOP_OP_RESOURCES_H = 6304
};

// Tick intervals and given points: case 0, 1, 2, 3, 4
const uint32 BG_VOP_TickIntervals[5] = { 15000, 12000, 9000, 6000, 3000 };
const uint32 BG_VOP_TickPoints[5] = { 0, 10, 10, 10, 10 };

const uint32 BG_VOP_PK_VP = 10;

struct BattlegroundVOPScore : public BattlegroundScore
{
    BattlegroundVOPScore() : OrbControl(0), OrbScore(0) { }
    ~BattlegroundVOPScore() { }
    uint32 OrbControl;
    uint32 OrbScore;
};

class BattlegroundVOP : public Battleground
{
public:
    BattlegroundVOP();
    ~BattlegroundVOP() { }

    /* inherited from BattlegroundClass */
    void AddPlayer(Player* player) override;
    void StartingEventCloseDoors() override;
    void StartingEventOpenDoors() override;
    /* Battleground Events */
    void EventPlayerClickedOnFlag(Player* Source, GameObject* target_obj) override;
    void RemovePlayer(Player* player, uint64 guid, uint32 team) override;
    void HandleKillPlayer(Player* player, Player* killer) override;
    WorldSafeLocsEntry const* GetClosestGraveYard(Player* player) override;
    bool SetupBattleground() override;
    void Reset() override;
    void UpdateTeamScore(uint32 Team);
    void EndBattleground(uint32 winner);
    void UpdatePlayerScore(Player* Source, uint32 type, uint32 value, bool doAddHonor = true) override;
    void FillInitialWorldStates(WorldStateBuilder& builder) override;
    uint32 GetPrematureWinner() override;
    void PostUpdateImpl(uint32 diff) override;

private:
    uint64 m_FlagKeeper[4] = { };
    uint32 m_HonorScoreTics[BG_TEAMS_COUNT] = { };
    uint32 m_HonorTics; 
    uint32 m_lastTick[BG_TEAMS_COUNT] = { };
    bool   m_TeamScores500Disadvantage[BG_TEAMS_COUNT] = { };
};

#endif
