/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SC_ACMGR_H
#define SC_ACMGR_H

#include "Common.h"
#include "SharedDefines.h"
#include <unordered_map>
#include "WorldSession.h"

class Player;
class ChatHandler;

enum ReportTypes
{
    SPEED_HACK_REPORT = 0,
    FLY_HACK_REPORT = 1,
    WALK_WATER_HACK_REPORT = 2,
    JUMP_HACK_REPORT = 3,
    TELEPORT_PLANE_HACK_REPORT = 4,
    CLIMB_HACK_REPORT = 5,
    TIME_MANIPULATION_REPORT = 6,
    GRAVITY_HACK_REPORT = 7,
    TELEPORT_HACK_REPORT = 8,
    BG_START_HACK_REPORT = 9,
    MAX_REPORT_TYPES
};

struct AnticheatData
{
    AnticheatData();

    void SetPosition(float x, float y, float z, float o);
    void Reset();

    uint32 lastOpcode;
    MovementInfo lastMovementInfo;
    uint32 totalReports;
    float average;
    uint32 creationTime;
    bool hasDailyReport;

    uint32 GetTotalReports() const;
    void SetTotalReports(uint32 _totalReports);

    uint32 GetTypeReports(uint32 type) const;
    void SetTypeReports(uint32 type, uint32 amount);

    float GetAverage() const;
    void SetAverage(float _average);

    uint32 GetCreationTime() const;
    void SetCreationTime(uint32 creationTime);

    uint32 typeReports[MAX_REPORT_TYPES];
    uint32 tempReports[MAX_REPORT_TYPES];
    uint32 tempReportsTimer[MAX_REPORT_TYPES];

    uint32 reportAnnounceCount[MAX_REPORT_TYPES];
    time_t reportAnnounceCooldown[MAX_REPORT_TYPES];
    void SetDailyReportState(bool b);
    bool GetDailyReportState();
};

// GUIDLow is the key.
typedef std::unordered_map<uint32, AnticheatData> AnticheatPlayersDataMap;

class AnticheatMgr
{
    AnticheatMgr();
    ~AnticheatMgr();

    public:
        friend class ACE_Singleton<AnticheatMgr, ACE_Null_Mutex>;

        void SetAllowedMovement(Player* player, bool);
        void StartHackDetection(Player* player, MovementInfo &movementInfo, uint32 opcode);
        void SavePlayerData(Player* player);
        void SavePlayerDataDaily(Player* player);
        void HandlePlayerLogin(Player* player);
        void HandlePlayerLogout(Player* player);

        void OnPlayerMove(Player* player, MovementInfo &mi, uint32 opcode);
        uint32 GetTotalReports(uint32 lowGUID);
        float GetAverage(uint32 lowGUID);
        uint32 GetTypeReports(uint32 lowGUID, uint8 type);
        AnticheatData* GetDataFor(Player* player);

        void AnticheatGlobalCommand(ChatHandler* handler);
        void AnticheatDeleteCommand(uint32 guid);
        void AnticheatPurgeCommand(ChatHandler* handler);
        void ResetDailyReportStates();

    private:
        void MapSpeedTeleExemption(Player* player, MovementInfo& movementInfo);
        void BGLogger(Player* player, AnticheatData& data);
        void SpeedHackDetection(Player* player, MovementInfo &movementInfo, AnticheatData& data);
        void FlyHackDetection(Player* player, MovementInfo &movementInfo, AnticheatData& data);
        void TeleportHackDetection(Player* player, MovementInfo movementInfo, AnticheatData& data);
        void WalkOnWaterHackDetection(Player* player, MovementInfo &movementInfo, AnticheatData& data);
        void JumpHackDetection(Player* player, MovementInfo &movementInfo, AnticheatData& data, uint32 opcode);
        void TeleportPlaneHackDetection(Player* player, MovementInfo &movementInfo, AnticheatData& data, uint32 opcode);
        void ClimbHackDetection(Player* player, MovementInfo &movementInfo, AnticheatData& data, uint32 opcode);
        void GravityHackDetection(Player* player, MovementInfo& movementInfo, AnticheatData& data, uint32 opcode);
        void BGStartExploit(Player* player, MovementInfo movementInfo, AnticheatData& data, uint32 opcode);
        void BuildReport(Player* player, AnticheatData& data, uint8 reportType);

        bool MustCheckTempReports(uint8 type);
        AnticheatPlayersDataMap _players;
};

#define sAnticheatMgr ACE_Singleton<AnticheatMgr, ACE_Null_Mutex>::instance()

#endif
