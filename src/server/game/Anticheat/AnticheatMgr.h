/*
 * Copyright (C) 2011-2021 Project SkyFire <https://www.projectskyfire.org/>
 * Copyright (C) 2008-2021 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2021 MaNGOS <https://www.getmangos.eu/>
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

#ifndef SC_ACMGR_H
#define SC_ACMGR_H

#include "Common.h"
#include "SharedDefines.h"
#include "ScriptMgr.h"
#include "AnticheatData.h"
#include "Chat.h"
#include "Player.h"

class Player;
class AnticheatData;

enum ReportTypes
{
    SPEED_HACK_REPORT = 0,
    FLY_HACK_REPORT = 1,
    WALK_WATER_HACK_REPORT = 2,
    JUMP_HACK_REPORT = 3,
    TELEPORT_PLANE_HACK_REPORT = 4,
    CLIMB_HACK_REPORT = 5,
    TELEPORT_HACK_REPORT = 6,
    IGNORE_CONTROL_REPORT = 7,
    ZAXIS_HACK_REPORT = 8,
    ANTISWIM_HACK_REPORT = 9,
    GRAVITY_HACK_REPORT = 10

   // MAX_REPORT_TYPES
};

// GUIDLow is the key.
typedef std::map<uint32, AnticheatData> AnticheatPlayersDataMap;

class AnticheatMgr
{
    AnticheatMgr();
    ~AnticheatMgr();

    public:
        static AnticheatMgr* instance()
        {
           static AnticheatMgr* instance = new AnticheatMgr();
           return instance;
        }
        void SetAllowedMovement(Player* player, bool);
        void StartHackDetection(Player* player, MovementInfo movementInfo, uint32 opcode);
        void SavePlayerData(Player* player);
        void SavePlayerDataDaily(Player* player);
        void OnPlayerMove(Player* player, MovementInfo mi, uint32 opcode);
        void StartScripts();

        void HandlePlayerLogin(Player* player);
        void HandlePlayerLogout(Player* player);

        uint32 GetTotalReports(uint32 lowGUID);
        float GetAverage(uint32 lowGUID);
        uint32 GetTypeReports(uint32 lowGUID, uint8 type);

        void AnticheatGlobalCommand(ChatHandler* handler);
        void AnticheatDeleteCommand(uint32 guid);
        void AnticheatPurgeCommand(ChatHandler* handler);
        void ResetDailyReportStates();
    private:
        void SpeedHackDetection(Player* player, MovementInfo movementInfo);
        void FlyHackDetection(Player* player, MovementInfo movementInfo);
        //void WalkOnWaterHackDetection(Player* player, MovementInfo movementInfo);
        void JumpHackDetection(Player* player, MovementInfo movementInfo,uint32 opcode);
        //void TeleportPlaneHackDetection(Player* player, MovementInfo movementInfo, uint32 opcode);
        void ClimbHackDetection(Player* player, MovementInfo movementInfo, uint32 opcode);
        void IgnoreControlHackDetection(Player* player, MovementInfo movementInfo, uint32 opcode);
        void TeleportHackDetection(Player* player, MovementInfo movementInfo);
        void ZAxisHackDetection(Player* player, MovementInfo movementInfo);
        //void AntiSwimHackDetection(Player* player, MovementInfo movementInfo, uint32 opcode);
        void GravityHackDetection(Player* player, MovementInfo movementInfo);
        void BuildReport(Player* player,uint8 reportType);

        bool MustCheckTempReports(uint8 type);
        uint32 _counter = 0;
        uint32 _alertFrequency;
        AnticheatPlayersDataMap m_Players;                        ///< Player data
};

#define sAnticheatMgr AnticheatMgr::instance()

#endif
