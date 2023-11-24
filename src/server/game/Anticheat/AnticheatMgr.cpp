/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "AnticheatMgr.h"
#include "Battleground.h"
#include "Chat.h"
#include "Language.h"
#include "MapManager.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuras.h"
#include "World.h"
#include "WorldSession.h"

enum Spells
{
    FREEZE = 9454,
    LFG_SPELL_DUNGEON_DESERTER = 71041,
    BG_SPELL_DESERTER = 26013,
    SILENCED = 23207,
    RESURRECTION_SICKNESS = 15007,
    SLOWDOWN = 61458
};

AnticheatData::AnticheatData()
{
    lastOpcode = 0;
    totalReports = 0;
    for (uint8 i = 0; i < MAX_REPORT_TYPES; i++)
    {
        typeReports[i] = 0;
        tempReports[i] = 0;
        tempReportsTimer[i] = 0;
        reportAnnounceCount[i] = 0;
        reportAnnounceCooldown[i] = 0;
    }
    average = 0;
    creationTime = 0;
    hasDailyReport = false;
}

float AnticheatData::GetAverage() const
{
    return average;
}

void AnticheatData::SetAverage(float _average)
{
    average = _average;
}

uint32 AnticheatData::GetCreationTime() const
{
    return creationTime;
}

void AnticheatData::SetCreationTime(uint32 _creationTime)
{
    creationTime = _creationTime;
}
void AnticheatData::SetDailyReportState(bool b)
{
    hasDailyReport = b;
}

bool AnticheatData::GetDailyReportState()
{
    return hasDailyReport;
}

void AnticheatData::Reset()
{
    totalReports = 0;
    average = 0;
    creationTime = 0;
    for (uint8 i = 0; i < MAX_REPORT_TYPES; i++)
    {
        tempReports[i] = 0;
        tempReportsTimer[i] = 0;
        typeReports[i] = 0;
    }
}

void AnticheatData::SetPosition(float x, float y, float z, float o)
{
    lastMovementInfo.pos.m_positionX = x;
    lastMovementInfo.pos.m_positionY = y;
    lastMovementInfo.pos.m_positionZ = z;
    lastMovementInfo.pos.m_orientation = o;
}

class AnticheatScript : public PlayerScript
{
public:
    AnticheatScript() : PlayerScript("AnticheatScripts") { }

    void OnLogout(Player* player) override
    {
        sAnticheatMgr->HandlePlayerLogout(player);
    }

    void OnLogin(Player* player, bool /*loginFirst*/ ) override
    {
        sAnticheatMgr->HandlePlayerLogin(player);
    }
};

void AddSC_Anticheat()
{
    new AnticheatScript();
}

AnticheatMgr::AnticheatMgr()
{
}

AnticheatMgr::~AnticheatMgr()
{
}

void AnticheatMgr::StartHackDetection(Player* player, MovementInfo& movementInfo, uint32 opcode)
{
    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_ENABLE))
        return;

    if (player->IsGameMaster())
        return;

    AnticheatData* data = GetDataFor(player);
    if (!data)
        return;

    if (player->IsInFlight() || player->GetTransport() || player->GetVehicle())
    {
        data->lastMovementInfo = movementInfo;
        data->lastOpcode = opcode;
        return;
    }

    SpeedHackDetection(player, movementInfo, *data);
    FlyHackDetection(player, movementInfo, *data);
    TeleportHackDetection(player, movementInfo, *data);
    if (movementInfo.HasMovementFlag(MOVEMENTFLAG_WATERWALKING))
    {
        WalkOnWaterHackDetection(player, movementInfo, *data);
    }
    JumpHackDetection(player, movementInfo, *data, opcode);
    TeleportPlaneHackDetection(player, movementInfo, *data, opcode);
    ClimbHackDetection(player, movementInfo, *data, opcode);
    GravityHackDetection(player, movementInfo, *data, opcode);
    if (Battleground* bg = player->GetBattleground())
    {
        if (bg->GetStatus() == STATUS_WAIT_JOIN)
        {
            BGStartExploit(player, movementInfo, *data, opcode);
        }
    }
    data->lastMovementInfo = movementInfo;
    data->lastOpcode = opcode;
}

void AnticheatMgr::SpeedHackDetection(Player* player, MovementInfo& movementInfo, AnticheatData& data)
{
    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_SPEEDHACK_ENABLE))
        return;

    // We exempt all transports found in 548 to prevent false tele hack hits
    if (player->GetMapId())
    {
        switch (player->GetMapId())
        {
            case 369: //Transport: Deeprun Tram
            case 607: //Transport: Strands of the Ancients
            case 582: //Transport: Rut'theran to Auberdine
            case 584: //Transport: Menethil to Theramore
            case 586: //Transport: Exodar to Auberdine
            case 587: //Transport: Feathermoon Ferry
            case 588: //Transport: Menethil to Auberdine
            case 589: //Transport: Orgrimmar to Grom'Gol
            case 590: //Transport: Grom'Gol to Undercity
            case 591: //Transport: Undercity to Orgrimmar
            case 592: //Transport: Borean Tundra Test
            case 593: //Transport: Booty Bay to Ratchet
            case 594: //Transport: Howling Fjord Sister Mercy (Quest)
            case 596: //Transport: Naglfar
            case 610: //Transport: Tirisfal to Vengeance Landing
            case 612: //Transport: Menethil to Valgarde
            case 613: //Transport: Orgrimmar to Warsong Hold
            case 614: //Transport: Stormwind to Valiance Keep
            case 620: //Transport: Moa'ki to Unu'pe
            case 621: //Transport: Moa'ki to Kamagua
            case 622: //Transport: Orgrim's Hammer
            case 623: //Transport: The Skybreaker
            case 641: //Transport: Alliance Airship BG
            case 642: //Transport: Horde Airship BG
            case 647: //Transport: Orgrimmar to Thunder Bluff
            case 672: //Transport: The Skybreaker (Icecrown Citadel Raid)
            case 673: //Transport: Orgrim's Hammer (Icecrown Citadel Raid)
            case 712: //Transport: The Skybreaker (IC Dungeon)
            case 713: //Transport: Orgrim's Hammer (IC Dungeon)
            case 718: //Transport: The Mighty Wind (Icecrown Citadel Raid)
            case 766: // Transport 2033864
            case 767: // Transport 2033865
            case 747: // Transport 203732
            case 762: // Transport 203861
            case 763: // Transport 203862
            case 1172: // Transport_Siege_of_Orgrimmar_Alliance
            case 1173: // Transport_Siege_of_Orgrimmar_Horde
            case 662: // Transport197195
            case 674: // Transport197349-2
            case 738: // Transport200100
            case 739: // Transport200101
            case 740: // Transport200102
            case 741: // Transport200103
            case 742: // Transport203729
            case 743: // Transport203730
            case 748: // Transport203858
            case 749: // Transport203859
            case 750: // Transport203860
            case 765: // Transport203863
            case 1132: // Transport218599
            case 1133: // Transport218600
                return;
        }
    }
    if (player->HasAura(1850) /*Dash*/ || player->HasAuraType(SPELL_AURA_FEATHER_FALL) || player->HasAuraType(SPELL_AURA_SAFE_FALL))
    {
        return;
    }

    if (player->IsInWater() && player->HasAura(57856) /*Glyph of Aquatic Form*/)
    {
        return;
    }

    float distance2D = movementInfo.pos.GetExactDist2d(&data.lastMovementInfo.pos);
    uint8 moveType = 0;

    // we need to know HOW is the player moving
    // TO-DO: Should we check the incoming movement flags?
    if (player->HasUnitMovementFlag(MOVEMENTFLAG_SWIMMING))
        moveType = MOVE_SWIM;
    else if (player->IsFlying())
        moveType = MOVE_FLIGHT;
    else if (player->HasUnitMovementFlag(MOVEMENTFLAG_WALKING))
        moveType = MOVE_WALK;
    else
        moveType = MOVE_RUN;

    // how many yards the player can do in one sec.
    float speedRate = player->GetSpeed(UnitMoveType(moveType)) + movementInfo.jump.xyspeed;

    // how long the player took to move to here.
    uint32 timeDiff = getMSTimeDiff(data.lastMovementInfo.time, movementInfo.time);

    if (int32(timeDiff) < 0)
    {
        uint32 latency = player->GetSession()->GetLatency();
        std::string goXYZ = ".go xyz " + std::to_string(player->GetPositionX()) + " " + std::to_string(player->GetPositionY()) + " " + std::to_string(player->GetPositionZ() + 1.0f) + " " + std::to_string(player->GetMap()->GetId()) + " " + std::to_string(player->GetOrientation());
        SF_LOG_INFO("anticheat", "Anticheat Manager:: Time Manipulation- Hack detected player %s - (GUID %u) - Latency: %u ms - IP: %s - Cheat Flagged at: %s", player->GetName().c_str(), player->GetGUID(), latency, player->GetSession()->GetRemoteAddress(), goXYZ.c_str());
        BuildReport(player, data, TIME_MANIPULATION_REPORT);
        timeDiff = 1;
    }

    if (!timeDiff)
        timeDiff = 1;

    // this is the distance doable by the player in 1 sec, using the time done to move to this point.
    float clientSpeedRate = distance2D * 1000 / timeDiff;

    // we create a diff speed in uint32 for further precision checking to avoid legit fall and slide
    uint32 diffspeed = clientSpeedRate - speedRate;

    // create a conf to establish a speed limit tolerance over server rate set speed
    // this is done so we can ignore minor violations that are not false positives such as going 1 or 2 over the speed limit
    uint32 _assignedspeeddiff = sWorld->getIntConfig(WorldIntConfigs::CONFIG_ANTICHEAT_SPEED_LIMIT_TOLERANCE);

        // We did the (uint32) cast to accept a margin of tolerance for seasonal spells and buffs such as sugar rush
    // We check the last MovementInfo for the falling flag since falling down a hill and sliding a bit triggered a false positive
    if (!movementInfo.HasMovementFlag(MOVEMENTFLAG_FALLING))
    {
        if (diffspeed >= _assignedspeeddiff)
        {
            // we did the (uint32) cast to accept a margin of tolerance
            if (clientSpeedRate >= _assignedspeeddiff + speedRate)
            {
                uint32 _combined = _assignedspeeddiff + speedRate;
                BuildReport(player, data, SPEED_HACK_REPORT);
                uint32 latency = player->GetSession()->GetLatency();
                std::string goXYZ = ".go xyz " + std::to_string(player->GetPositionX()) + " " + std::to_string(player->GetPositionY()) + " " + std::to_string(player->GetPositionZ() + 1.0f) + " " + std::to_string(player->GetMap()->GetId()) + " " + std::to_string(player->GetOrientation());
                SF_LOG_INFO("anticheat", "Anticheat Manager:: Speed - Hack detected player %s - (GUID %u) - (Speed Movement at %u above allowed Server Set rate %u.)- Latency: %u ms - IP: %s - Cheat Flagged at: %s", player->GetName().c_str(), player->GetGUID(), diffspeed, _combined, latency, player->GetSession()->GetRemoteAddress(), goXYZ.c_str());
                if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_CM_SPEEDHACK))
                {
                    SF_LOG_INFO("anticheat", "ANTICHEAT COUNTER MEASURE:: %s SPEED HACK REVERTED", player->GetName().c_str());
                    if (Aura* slowcheater = player->AddAura(SLOWDOWN, player))// SLOWDOWN
                    {
                        slowcheater->SetDuration(1000);
                    }
                }
            }
        }
    }
}

void AnticheatMgr::FlyHackDetection(Player* player, MovementInfo& movementInfo, AnticheatData& data)
{
    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_FLYHACK_ENABLE))
        return;

    if (!data.lastMovementInfo.HasMovementFlag(MOVEMENTFLAG_FLYING))
        return;

    if (player->HasAuraType(SPELL_AURA_FLY) ||
        player->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED) ||
        player->HasAuraType(SPELL_AURA_MOD_INCREASE_FLIGHT_SPEED))
        return;

    /*Thanks to @LilleCarl for info to check extra flag*/
    bool stricterChecks = true;
    stricterChecks = !(movementInfo.HasMovementFlag(MOVEMENTFLAG_ASCENDING | MOVEMENTFLAG_DESCENDING) && !player->IsInWater());

    if (!movementInfo.HasMovementFlag(MOVEMENTFLAG_CAN_FLY) && !movementInfo.HasMovementFlag(MOVEMENTFLAG_FLYING) && stricterChecks)
    {
        return;
    }

    uint32 latency = player->GetSession()->GetLatency();
    std::string goXYZ = ".go xyz " + std::to_string(player->GetPositionX()) + " " + std::to_string(player->GetPositionY()) + " " + std::to_string(player->GetPositionZ() + 1.0f) + " " + std::to_string(player->GetMap()->GetId()) + " " + std::to_string(player->GetOrientation());
    SF_LOG_INFO("anticheat", "Anticheat Manager:: Fly - Hack detected player %s - (GUID %u) - Latency: %u ms - IP: %s - Cheat Flagged at: %s", player->GetName().c_str(), player->GetGUID(), latency, player->GetSession()->GetRemoteAddress().c_str(), goXYZ.c_str());
    BuildReport(player, data, FLY_HACK_REPORT);

    if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_CM_FLYHACK))
    {
        SF_LOG_INFO("anticheat", "ANTICHEAT COUNTER MEASURE:: %s FLY HACK REVERTED", player->GetName().c_str());
        // Drop them with a op code set if they use a exploit or hack app
        WorldPacket cheater(SMSG_MOVE_UNSET_CAN_FLY, 8 + 4);
        cheater.SetOpcode(SMSG_MOVE_UNSET_CAN_FLY);

        player->WriteMovementInfo(cheater);
        player->SendMessageToSet(&cheater, true);
    }
}

void AnticheatMgr::TeleportHackDetection(Player* player, MovementInfo movementInfo, AnticheatData& data)
{
    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_TELEPORTHACK_ENABLE))
        return;

    float lastX = data.lastMovementInfo.pos.GetPositionX();
    float newX = movementInfo.pos.GetPositionX();

    float lastY = data.lastMovementInfo.pos.GetPositionY();
    float newY = movementInfo.pos.GetPositionY();

    float lastZ = data.lastMovementInfo.pos.GetPositionZ();
    float newZ = movementInfo.pos.GetPositionZ();

    float xDiff = fabs(lastX - newX);
    float yDiff = fabs(lastY - newY);

    if (player->IsFalling() || (player->IsFalling() && player->IsMounted()))
        return;

    // We exempt all transports found in 548 to prevent false tele hack hits
    if (player->GetMapId())
    {
        switch (player->GetMapId())
        {
            case 369: //Transport: Deeprun Tram
            case 607: //Transport: Strands of the Ancients
            case 582: //Transport: Rut'theran to Auberdine
            case 584: //Transport: Menethil to Theramore
            case 586: //Transport: Exodar to Auberdine
            case 587: //Transport: Feathermoon Ferry
            case 588: //Transport: Menethil to Auberdine
            case 589: //Transport: Orgrimmar to Grom'Gol
            case 590: //Transport: Grom'Gol to Undercity
            case 591: //Transport: Undercity to Orgrimmar
            case 592: //Transport: Borean Tundra Test
            case 593: //Transport: Booty Bay to Ratchet
            case 594: //Transport: Howling Fjord Sister Mercy (Quest)
            case 596: //Transport: Naglfar
            case 610: //Transport: Tirisfal to Vengeance Landing
            case 612: //Transport: Menethil to Valgarde
            case 613: //Transport: Orgrimmar to Warsong Hold
            case 614: //Transport: Stormwind to Valiance Keep
            case 620: //Transport: Moa'ki to Unu'pe
            case 621: //Transport: Moa'ki to Kamagua
            case 622: //Transport: Orgrim's Hammer
            case 623: //Transport: The Skybreaker
            case 641: //Transport: Alliance Airship BG
            case 642: //Transport: Horde Airship BG
            case 647: //Transport: Orgrimmar to Thunder Bluff
            case 672: //Transport: The Skybreaker (Icecrown Citadel Raid)
            case 673: //Transport: Orgrim's Hammer (Icecrown Citadel Raid)
            case 712: //Transport: The Skybreaker (IC Dungeon)
            case 713: //Transport: Orgrim's Hammer (IC Dungeon)
            case 718: //Transport: The Mighty Wind (Icecrown Citadel Raid)
            case 766: // Transport 2033864
            case 767: // Transport 2033865
            case 747: // Transport 203732
            case 762: // Transport 203861
            case 763: // Transport 203862
            case 1172: // Transport_Siege_of_Orgrimmar_Alliance
            case 1173: // Transport_Siege_of_Orgrimmar_Horde
            case 662: // Transport197195
            case 674: // Transport197349-2
            case 738: // Transport200100
            case 739: // Transport200101
            case 740: // Transport200102
            case 741: // Transport200103
            case 742: // Transport203729
            case 743: // Transport203730
            case 748: // Transport203858
            case 749: // Transport203859
            case 750: // Transport203860
            case 765: // Transport203863
            case 1132: // Transport218599
            case 1133: // Transport218600
                return;
        }
    }

    /* Please work */
    if ((xDiff >= 50.0f || yDiff >= 50.0f) && !player->CanTeleport() && !player->IsBeingTeleported())// teleport helpers in play
    {
        uint32 latency = player->GetSession()->GetLatency();
        std::string goXYZ = ".go xyz " + std::to_string(player->GetPositionX()) + " " + std::to_string(player->GetPositionY()) + " " + std::to_string(player->GetPositionZ() + 1.0f) + " " + std::to_string(player->GetMap()->GetId()) + " " + std::to_string(player->GetOrientation());
        SF_LOG_INFO("anticheat", "Anticheat Manager:: Teleport - Hack detected player %s - (GUID %u) - X-Diff: %f - Y-Diff: %f - Latency: %u ms - IP: %s - Cheat Flagged at: %s", player->GetName().c_str(), player->GetGUID(), xDiff, yDiff, latency, player->GetSession()->GetRemoteAddress().c_str(), goXYZ.c_str());
        BuildReport(player, data, TELEPORT_HACK_REPORT);
        if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_CM_TELEPORT))
        {
            player->TeleportTo(player->GetMapId(), lastX, lastY, lastZ, player->GetOrientation());
            SF_LOG_INFO("anticheat", "ANTICHEAT COUNTER MEASURE:: %s TELEPORT HACK REVERTED PLAYER BACK", player->GetName().c_str());
        }
    }
    else if (player->CanTeleport())// if we hit the teleport helpers in the source then we return it to false
        player->SetCanTeleport(false);
}

void AnticheatMgr::WalkOnWaterHackDetection(Player* player, MovementInfo& movementInfo, AnticheatData& data)
{
    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_WATERWALKHACK_ENABLE))
        return;

    if (!data.lastMovementInfo.HasMovementFlag(MOVEMENTFLAG_WATERWALKING))
        return;

    if (player->IsFlying())
        return;

    // ghost can water walk
    if (player->HasAuraType(SPELL_AURA_GHOST))
        return;

    // if we are a ghost we can walk on water
    if (!player->IsAlive())
        return;

    // if the player previous movement and current movement is water walking then we do a follow up check
    if (data.lastMovementInfo.HasMovementFlag(MOVEMENTFLAG_WATERWALKING) && movementInfo.HasMovementFlag(MOVEMENTFLAG_WATERWALKING))
    { // if player has the following auras then we return
        if (player->HasAuraType(SPELL_AURA_WATER_WALK) || player->HasAuraType(SPELL_AURA_FEATHER_FALL) ||
            player->HasAuraType(SPELL_AURA_SAFE_FALL))
        {
            return;
        }
    }
    else if (!data.lastMovementInfo.HasMovementFlag(MOVEMENTFLAG_WATERWALKING) && !movementInfo.HasMovementFlag(MOVEMENTFLAG_WATERWALKING))
    {
        //Boomer Review Time:
        //Return stops code execution of the entire function
        return;
    }

    uint32 latency = 0;
    latency = player->GetSession()->GetLatency();
    std::string goXYZ = ".go xyz " + std::to_string(player->GetPositionX()) + " " + std::to_string(player->GetPositionY()) + " " + std::to_string(player->GetPositionZ() + 1.0f) + " " + std::to_string(player->GetMap()->GetId()) + " " + std::to_string(player->GetOrientation());
    SF_LOG_INFO("anticheat", "Anticheat Manager:: Walk on Water - Hack detected player %s - (GUID %u) - Latency: %u ms - IP: %s - Cheat Flagged at: %s", player->GetName().c_str(), player->GetGUID(), latency, player->GetSession()->GetRemoteAddress().c_str(), goXYZ.c_str());
    BuildReport(player, data, WALK_WATER_HACK_REPORT);
    if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_CM_WATERHACK))
    {
        SF_LOG_INFO("anticheat", "ANTICHEAT COUNTER MEASURE:: %s WATER WALKING HACK REVERTED", player->GetName().c_str());
        player->GetMotionMaster()->MoveFallPlayer();
    }
}

void AnticheatMgr::JumpHackDetection(Player* player, MovementInfo& movementInfo, AnticheatData& data, uint32 opcode)
{
    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_JUMPHACK_ENABLE))
        return;

    const float ground_Z = movementInfo.pos.GetPositionZ() - player->GetMap()->GetHeight(movementInfo.pos.GetPositionX(), movementInfo.pos.GetPositionY(), movementInfo.pos.GetPositionZ());

    const bool no_fly_auras = !(player->HasAuraType(SPELL_AURA_FLY) || player->HasAuraType(SPELL_AURA_MOD_INCREASE_VEHICLE_FLIGHT_SPEED)
        || player->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED) || player->HasAuraType(SPELL_AURA_MOD_INCREASE_FLIGHT_SPEED)
        || player->HasAuraType(SPELL_AURA_MOD_MOUNTED_FLIGHT_SPEED_ALWAYS));
    const bool no_fly_flags = ((movementInfo.flags & (MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING)) == 0);
    const bool no_swim_in_water = !player->IsInWater();
    const bool no_swim_above_water = movementInfo.pos.GetPositionZ() - 7.0f >= player->GetMap()->GetWaterLevel(movementInfo.pos.GetPositionX(), movementInfo.pos.GetPositionY());
    const bool no_swim_water = no_swim_in_water && no_swim_above_water;

    if (data.lastOpcode == MSG_MOVE_JUMP && opcode == MSG_MOVE_JUMP)
    {
        BuildReport(player, data, JUMP_HACK_REPORT);
        uint32 latency = 0;
        latency = player->GetSession()->GetLatency();
        std::string goXYZ = ".go xyz " + std::to_string(player->GetPositionX()) + " " + std::to_string(player->GetPositionY()) + " " + std::to_string(player->GetPositionZ() + 1.0f) + " " + std::to_string(player->GetMap()->GetId()) + " " + std::to_string(player->GetOrientation());
        SF_LOG_INFO("anticheat", "Anticheat Manager:: Jump-Hack detected player %s - Latency: %u ms - IP: %u - Cheat Flagged at: %s", player->GetName().c_str(), latency, player->GetSession()->GetRemoteAddress().c_str(), goXYZ.c_str());
        if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_CM_JUMPHACK))
        {
            SF_LOG_INFO("anticheat", "ANTICHEAT COUNTER MEASURE:: %s JUMP HACK REVERTED", player->GetName().c_str());
            player->GetMotionMaster()->MoveFallPlayer();
        }
    }
    else if (no_fly_auras && no_fly_flags && no_swim_water)
    {
        if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_ADV_JUMPHACK_ENABLE))
            return;

        if (data.lastOpcode == MSG_MOVE_JUMP && !player->IsFalling())
            return;

        float distance2D = movementInfo.pos.GetExactDist2d(&data.lastMovementInfo.pos);

        // This is necessary since MovementHandler fires if you rotate the camera in place
        if (!distance2D)
            return;

        if (!player->HasUnitMovementFlag(MOVEMENTFLAG_DISABLE_GRAVITY) && movementInfo.jump.zspeed < -10.0f)
            return;

        if (player->HasAuraType(SPELL_AURA_WATER_WALK) || player->HasAuraType(SPELL_AURA_FEATHER_FALL) ||
            player->HasAuraType(SPELL_AURA_SAFE_FALL))
        {
            return;
        }

        // We exempt select areas found in 335 to prevent false hack hits
        if (player->GetAreaId())
        {
            switch (player->GetAreaId())
            {
                case 4273: //Celestial Planetarium Observer Battle has a narrow path that false flags
                case 6724: //Backbreaker Bay
                case 6725: //Greymist Firth
                    return;
            }
        }

        if (ground_Z > 5.0f && movementInfo.pos.GetPositionZ() >= player->GetPositionZ())
        {
            uint32 latency = 0;
            latency = player->GetSession()->GetLatency();
            std::string goXYZ = ".go xyz " + std::to_string(player->GetPositionX()) + " " + std::to_string(player->GetPositionY()) + " " + std::to_string(player->GetPositionZ() + 1.0f) + " " + std::to_string(player->GetMap()->GetId()) + " " + std::to_string(player->GetOrientation());
            SF_LOG_INFO("anticheat", "Anticheat Manager:: Advanced Jump-Hack detected player %s - Latency: %u ms - IP: %u - Cheat Flagged at: %s", player->GetName().c_str(), latency, player->GetSession()->GetRemoteAddress().c_str(), goXYZ.c_str());
            BuildReport(player, data, JUMP_HACK_REPORT);
            if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_CM_ADVJUMPHACK))
            {
                SF_LOG_INFO("anticheat", "ANTICHEAT COUNTER MEASURE:: %s ADVANCE JUMP HACK REVERTED", player->GetName().c_str());
                player->GetMotionMaster()->MoveFallPlayer();
            }
        }
    }
}

void AnticheatMgr::TeleportPlaneHackDetection(Player* player, MovementInfo& movementInfo, AnticheatData& data, uint32 opcode)
{
    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_TELEPANEHACK_ENABLE))
        return;


    if (player->HasAuraType(SPELL_AURA_WATER_WALK) || player->HasAuraType(SPELL_AURA_WATER_BREATHING) || player->HasAuraType(SPELL_AURA_GHOST))
        return;

    ZLiquidStatus result = LIQUID_MAP_ABOVE_WATER;
    float distance2D = movementInfo.pos.GetExactDist2d(&data.lastMovementInfo.pos);

    // We don't need to check for a water walking hack if the player hasn't moved
    // This is necessary since MovementHandler fires if you rotate the camera in place
    if (!distance2D)
        return;

    if (data.lastOpcode == MSG_MOVE_JUMP)
        return;

    if (opcode == MSG_MOVE_FALL_LAND)
        return;

    if (player && result == LIQUID_MAP_ABOVE_WATER)
        return;

    if (movementInfo.HasMovementFlag(MOVEMENTFLAG_FALLING | MOVEMENTFLAG_SWIMMING))
        return;

    // If he is flying we dont need to check
    if (movementInfo.HasMovementFlag(MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING))
        return;

    // We exempt select areas found in 335 to prevent false hack hits
    if (player->GetAreaId())
    {
        switch (player->GetAreaId())
        {
        case 4273: //Celestial Planetarium Observer Battle has a narrow path that false flags
            return;
        }
    }

    float pos_z = player->GetPositionZ();
    float groundZ = player->GetMap()->GetHeight(player->GetPositionX(), player->GetPositionY(), MAX_HEIGHT);
    float floorZ = player->GetMap()->GetHeight(player->GetPositionX(), player->GetPositionY(), player->GetPositionZ());

    // we are not really walking there
    if (groundZ == floorZ && (fabs(groundZ - pos_z) > 2.0f || fabs(groundZ - pos_z) < -1.0f))
    {
        uint32 latency = 0;
        latency = player->GetSession()->GetLatency();
        std::string goXYZ = ".go xyz " + std::to_string(player->GetPositionX()) + " " + std::to_string(player->GetPositionY()) + " " + std::to_string(player->GetPositionZ() + 1.0f) + " " + std::to_string(player->GetMap()->GetId()) + " " + std::to_string(player->GetOrientation());
        SF_LOG_INFO("anticheat", "Anticheat Manager:: Teleport To Plane - Hack detected player %s - (GUID %u) - Latency: %u ms - IP: %s - Cheat Flagged at: %s", player->GetName().c_str(), player->GetGUID(), latency, player->GetSession()->GetRemoteAddress().c_str(), goXYZ.c_str());
        BuildReport(player, data, TELEPORT_PLANE_HACK_REPORT);
    }
}

// basic detection
void AnticheatMgr::ClimbHackDetection(Player *player, MovementInfo &movementInfo, AnticheatData& data, uint32 opcode)
{
    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_CLIMBHACK_ENABLE))
        return;

    // in this case we don't care if they are "legal" flags, they are handled in another parts of the Anticheat Manager.
    if (player->IsInWater() ||
        player->IsFlying() ||
        player->IsFalling())
        return;

    // If the player jumped, we dont want to check for climb hack
    // This can lead to false positives for climbing game objects legit
    if (opcode == MSG_MOVE_JUMP)
        return;

    if (player->HasUnitMovementFlag(MOVEMENTFLAG_FALLING))
        return;

    Position playerPos;
    player->GetPosition(&playerPos);

    float deltaZ = fabs(playerPos.GetPositionZ() - movementInfo.pos.GetPositionZ());
    float tanangle = movementInfo.pos.GetExactDist2d(&playerPos) / deltaZ;

    if (!player->HasUnitMovementFlag(MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING | MOVEMENTFLAG_SWIMMING))
    {
        if (movementInfo.pos.GetPositionZ() > playerPos.GetPositionZ() &&
            deltaZ > 1.87f && tanangle < 0.57735026919f) // 30 degrees
        {
            uint32 latency = 0;
            latency = player->GetSession()->GetLatency();
            std::string goXYZ = ".go xyz " + std::to_string(player->GetPositionX()) + " " + std::to_string(player->GetPositionY()) + " " + std::to_string(player->GetPositionZ() + 1.0f) + " " + std::to_string(player->GetMap()->GetId()) + " " + std::to_string(player->GetOrientation());
            SF_LOG_INFO("anticheat", "Anticheat Manager:: Climb - Hack detected player %s - (GUID %u) - Latency: %u ms - IP: %s - Cheat Flagged at: %s", player->GetName().c_str(), player->GetGUID(), latency, player->GetSession()->GetRemoteAddress().c_str(), goXYZ.c_str());
            BuildReport(player, data, CLIMB_HACK_REPORT);
        }
    }
}

void AnticheatMgr::GravityHackDetection(Player* player, MovementInfo& movementInfo, AnticheatData& data, uint32 opcode)
{
    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_GRAVITY_ENABLE))
        return;

    if (player->HasAuraType(SPELL_AURA_FEATHER_FALL))
    {
        return;
    }

    if (data.lastOpcode == MSG_MOVE_JUMP)
    {
        if (!player->HasUnitMovementFlag(MOVEMENTFLAG_DISABLE_GRAVITY) && movementInfo.jump.zspeed < -10.0f)
        {
            uint32 latency = 0;
            latency = player->GetSession()->GetLatency();
            std::string goXYZ = ".go xyz " + std::to_string(player->GetPositionX()) + " " + std::to_string(player->GetPositionY()) + " " + std::to_string(player->GetPositionZ() + 1.0f) + " " + std::to_string(player->GetMap()->GetId()) + " " + std::to_string(player->GetOrientation());
            SF_LOG_INFO("anticheat", "Anticheat Manager:: Gravity - Hack detected player %s - (GUID %u) - Latency: %u ms - IP: %s - Cheat Flagged at: %s", player->GetName().c_str(), player->GetGUID(), latency, player->GetSession()->GetRemoteAddress().c_str(), goXYZ.c_str());
            BuildReport(player, data, GRAVITY_HACK_REPORT);
        }
    }
}

void AnticheatMgr::BGStartExploit(Player* player, MovementInfo movementInfo, AnticheatData& data, uint32 opcode)
{
    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_BG_START_HACK_ENABLE))
        return;

    switch (player->GetMapId())
    {
        case 30: // Alterac Valley
        {
            if (Battleground* bg = player->GetBattleground())
            {
                if (bg->GetStatus() == STATUS_WAIT_JOIN)
                {
                    // Outside of starting area before BG has started.
                    if ((player->GetTeamId() == TEAM_ALLIANCE && movementInfo.pos.GetPositionX() < 770.0f) ||
                        (player->GetTeamId() == TEAM_ALLIANCE && movementInfo.pos.GetPositionX() > 940.31f) ||
                        (player->GetTeamId() == TEAM_ALLIANCE && movementInfo.pos.GetPositionY() < -525.0f))
                    {
                        uint32 latency = 0;
                        latency = player->GetSession()->GetLatency();
                        std::string goXYZ = ".go xyz " + std::to_string(player->GetPositionX()) + " " + std::to_string(player->GetPositionY()) + " " + std::to_string(player->GetPositionZ() + 1.0f) + " " + std::to_string(player->GetMap()->GetId()) + " " + std::to_string(player->GetOrientation());
                        SF_LOG_INFO("anticheat", "Anticheat Manager:: BG START - Hack detected player %s - (GUID %u) - Latency: %u ms - IP: %s - Cheat Flagged at: %s", player->GetName().c_str(), player->GetGUID(), latency, player->GetSession()->GetRemoteAddress().c_str(), goXYZ.c_str());
                        BuildReport(player, data, BG_START_HACK_REPORT);
                    }
                    if ((player->GetTeamId() == TEAM_HORDE && movementInfo.pos.GetPositionY() > -535.0f) ||
                        (player->GetTeamId() == TEAM_HORDE && movementInfo.pos.GetPositionX() > -1283.33f) ||
                        (player->GetTeamId() == TEAM_HORDE && movementInfo.pos.GetPositionY() < -716.0f))
                    {
                        uint32 latency = 0;
                        latency = player->GetSession()->GetLatency();
                        std::string goXYZ = ".go xyz " + std::to_string(player->GetPositionX()) + " " + std::to_string(player->GetPositionY()) + " " + std::to_string(player->GetPositionZ() + 1.0f) + " " + std::to_string(player->GetMap()->GetId()) + " " + std::to_string(player->GetOrientation());
                        SF_LOG_INFO("anticheat", "Anticheat Manager:: BG START - Hack detected player %s - (GUID %u) - Latency: %u ms - IP: %s - Cheat Flagged at: %s", player->GetName().c_str(), player->GetGUID(), latency, player->GetSession()->GetRemoteAddress().c_str(), goXYZ.c_str());
                        BuildReport(player, data, BG_START_HACK_REPORT);
                    }
                }
            }
            break;
        }
        case 489: // Warsong Gulch
        {
            // Only way to get this high is with engineering items malfunction.
            if (!(movementInfo.HasMovementFlag(MOVEMENTFLAG_FALLING_FAR) || data.lastOpcode == MSG_MOVE_JUMP) && movementInfo.pos.GetPositionZ() > 380.0f)
            {
                uint32 latency = 0;
                latency = player->GetSession()->GetLatency();
                std::string goXYZ = ".go xyz " + std::to_string(player->GetPositionX()) + " " + std::to_string(player->GetPositionY()) + " " + std::to_string(player->GetPositionZ() + 1.0f) + " " + std::to_string(player->GetMap()->GetId()) + " " + std::to_string(player->GetOrientation());
                SF_LOG_INFO("anticheat", "Anticheat Manager:: BG START - Hack detected player %s - (GUID %u) - Latency: %u ms - IP: %s - Cheat Flagged at: %s", player->GetName().c_str(), player->GetGUID(), latency, player->GetSession()->GetRemoteAddress().c_str(), goXYZ.c_str());
                BuildReport(player, data, BG_START_HACK_REPORT);
            }

            if (Battleground* bg = player->GetBattleground())
            {
                if (bg->GetStatus() == STATUS_WAIT_JOIN)
                {
                    // Outside of starting area before BG has started.
                    if ((player->GetTeamId() == TEAM_ALLIANCE && movementInfo.pos.GetPositionX() < 1490.0f) ||
                        (player->GetTeamId() == TEAM_ALLIANCE && movementInfo.pos.GetPositionY() > 1500.0f) ||
                        (player->GetTeamId() == TEAM_ALLIANCE && movementInfo.pos.GetPositionY() < 1450.0f))
                    {
                        uint32 latency = 0;
                        latency = player->GetSession()->GetLatency();
                        std::string goXYZ = ".go xyz " + std::to_string(player->GetPositionX()) + " " + std::to_string(player->GetPositionY()) + " " + std::to_string(player->GetPositionZ() + 1.0f) + " " + std::to_string(player->GetMap()->GetId()) + " " + std::to_string(player->GetOrientation());
                        SF_LOG_INFO("anticheat", "Anticheat Manager:: BG START - Hack detected player %s - (GUID %u) - Latency: %u ms - IP: %s - Cheat Flagged at: %s", player->GetName().c_str(), player->GetGUID(), latency, player->GetSession()->GetRemoteAddress().c_str(), goXYZ.c_str());
                        BuildReport(player, data, BG_START_HACK_REPORT);
                    }
                    if ((player->GetTeamId() == TEAM_HORDE && movementInfo.pos.GetPositionX() > 957.0f) ||
                        (player->GetTeamId() == TEAM_HORDE && movementInfo.pos.GetPositionY() < 1416.0f) ||
                        (player->GetTeamId() == TEAM_HORDE && movementInfo.pos.GetPositionY() > 1466.0f))
                    {
                        uint32 latency = 0;
                        latency = player->GetSession()->GetLatency();
                        std::string goXYZ = ".go xyz " + std::to_string(player->GetPositionX()) + " " + std::to_string(player->GetPositionY()) + " " + std::to_string(player->GetPositionZ() + 1.0f) + " " + std::to_string(player->GetMap()->GetId()) + " " + std::to_string(player->GetOrientation());
                        SF_LOG_INFO("anticheat", "Anticheat Manager:: BG START - Hack detected player %s - (GUID %u) - Latency: %u ms - IP: %s - Cheat Flagged at: %s", player->GetName().c_str(), player->GetGUID(), latency, player->GetSession()->GetRemoteAddress().c_str(), goXYZ.c_str());
                        BuildReport(player, data, BG_START_HACK_REPORT);
                    }
                }
            }
            break;
        }
        case 529: // Arathi Basin
        {
            if (Battleground* bg = player->GetBattleground())
            {
                if (bg->GetStatus() == STATUS_WAIT_JOIN)
                {
                    // Outside of starting area before BG has started.
                    if ((player->GetTeamId() == TEAM_ALLIANCE && movementInfo.pos.GetPositionX() < 1270.0f) ||
                        (player->GetTeamId() == TEAM_ALLIANCE && movementInfo.pos.GetPositionY() < 1258.0f) ||
                        (player->GetTeamId() == TEAM_ALLIANCE && movementInfo.pos.GetPositionY() > 1361.0f))
                    {
                        uint32 latency = 0;
                        latency = player->GetSession()->GetLatency();
                        std::string goXYZ = ".go xyz " + std::to_string(player->GetPositionX()) + " " + std::to_string(player->GetPositionY()) + " " + std::to_string(player->GetPositionZ() + 1.0f) + " " + std::to_string(player->GetMap()->GetId()) + " " + std::to_string(player->GetOrientation());
                        SF_LOG_INFO("anticheat", "Anticheat Manager:: BG START - Hack detected player %s - (GUID %u) - Latency: %u ms - IP: %s - Cheat Flagged at: %s", player->GetName().c_str(), player->GetGUID(), latency, player->GetSession()->GetRemoteAddress().c_str(), goXYZ.c_str());
                        BuildReport(player, data, BG_START_HACK_REPORT);
                    }
                    if ((player->GetTeamId() == TEAM_HORDE && movementInfo.pos.GetPositionX() > 730.0f) ||
                        (player->GetTeamId() == TEAM_HORDE && movementInfo.pos.GetPositionY() > 724.8f))
                    {
                        uint32 latency = 0;
                        latency = player->GetSession()->GetLatency();
                        std::string goXYZ = ".go xyz " + std::to_string(player->GetPositionX()) + " " + std::to_string(player->GetPositionY()) + " " + std::to_string(player->GetPositionZ() + 1.0f) + " " + std::to_string(player->GetMap()->GetId()) + " " + std::to_string(player->GetOrientation());
                        SF_LOG_INFO("anticheat", "Anticheat Manager:: BG START - Hack detected player %s - (GUID %u) - Latency: %u ms - IP: %s - Cheat Flagged at: %s", player->GetName().c_str(), player->GetGUID(), latency, player->GetSession()->GetRemoteAddress().c_str(), goXYZ.c_str());
                        BuildReport(player, data, BG_START_HACK_REPORT);
                    }
                }
            }
            break;
        }
        case 566: // Eye of the Storm
        {
            if (Battleground* bg = player->GetBattleground())
            {
                if (bg->GetStatus() == STATUS_WAIT_JOIN)
                {
                    // Outside of starting area before BG has started.
                    if ((player->GetTeamId() == TEAM_ALLIANCE && movementInfo.pos.GetPositionX() < 2512.0f) ||
                        (player->GetTeamId() == TEAM_ALLIANCE && movementInfo.pos.GetPositionY() > 1610.0f) ||
                        (player->GetTeamId() == TEAM_ALLIANCE && movementInfo.pos.GetPositionY() < 1584.0f))
                    {
                        uint32 latency = 0;
                        latency = player->GetSession()->GetLatency();
                        std::string goXYZ = ".go xyz " + std::to_string(player->GetPositionX()) + " " + std::to_string(player->GetPositionY()) + " " + std::to_string(player->GetPositionZ() + 1.0f) + " " + std::to_string(player->GetMap()->GetId()) + " " + std::to_string(player->GetOrientation());
                        SF_LOG_INFO("anticheat", "Anticheat Manager:: BG START - Hack detected player %s - (GUID %u) - Latency: %u ms - IP: %s - Cheat Flagged at: %s", player->GetName().c_str(), player->GetGUID(), latency, player->GetSession()->GetRemoteAddress().c_str(), goXYZ.c_str());
                        BuildReport(player, data, BG_START_HACK_REPORT);
                    }
                    if ((player->GetTeamId() == TEAM_HORDE && movementInfo.pos.GetPositionX() > 1816.0f) ||
                        (player->GetTeamId() == TEAM_HORDE && movementInfo.pos.GetPositionY() > 1554.0f) ||
                        (player->GetTeamId() == TEAM_HORDE && movementInfo.pos.GetPositionY() < 1526.0f))
                    {
                        uint32 latency = 0;
                        latency = player->GetSession()->GetLatency();
                        std::string goXYZ = ".go xyz " + std::to_string(player->GetPositionX()) + " " + std::to_string(player->GetPositionY()) + " " + std::to_string(player->GetPositionZ() + 1.0f) + " " + std::to_string(player->GetMap()->GetId()) + " " + std::to_string(player->GetOrientation());
                        SF_LOG_INFO("anticheat", "Anticheat Manager:: BG START - Hack detected player %s - (GUID %u) - Latency: %u ms - IP: %s - Cheat Flagged at: %s", player->GetName().c_str(), player->GetGUID(), latency, player->GetSession()->GetRemoteAddress().c_str(), goXYZ.c_str());
                        BuildReport(player, data, BG_START_HACK_REPORT);
                    }
                }
            }
            break;
        }
        case 628: // Island Of Conquest
        {
            if (Battleground* bg = player->GetBattleground())
            {
                if (bg->GetStatus() == STATUS_WAIT_JOIN)
                {
                    // Outside of starting area before BG has started.
                    if ((player->GetTeamId() == TEAM_ALLIANCE && movementInfo.pos.GetPositionX() > 412.0f) ||
                        (player->GetTeamId() == TEAM_ALLIANCE && movementInfo.pos.GetPositionY() < -911.0f) ||
                        (player->GetTeamId() == TEAM_ALLIANCE && movementInfo.pos.GetPositionY() > -760.0f))
                    {
                        uint32 latency = 0;
                        latency = player->GetSession()->GetLatency();
                        std::string goXYZ = ".go xyz " + std::to_string(player->GetPositionX()) + " " + std::to_string(player->GetPositionY()) + " " + std::to_string(player->GetPositionZ() + 1.0f) + " " + std::to_string(player->GetMap()->GetId()) + " " + std::to_string(player->GetOrientation());
                        SF_LOG_INFO("anticheat", "Anticheat Manager:: BG START - Hack detected player %s - (GUID %u) - Latency: %u ms - IP: %s - Cheat Flagged at: %s", player->GetName().c_str(), player->GetGUID(), latency, player->GetSession()->GetRemoteAddress().c_str(), goXYZ.c_str());
                        BuildReport(player, data, BG_START_HACK_REPORT);
                    }
                    if ((player->GetTeamId() == TEAM_HORDE && movementInfo.pos.GetPositionX() < 1147.8f) ||
                        (player->GetTeamId() == TEAM_HORDE && movementInfo.pos.GetPositionY() < -855.0f) ||
                        (player->GetTeamId() == TEAM_HORDE && movementInfo.pos.GetPositionY() > -676.0f))
                    {
                        uint32 latency = 0;
                        latency = player->GetSession()->GetLatency();
                        std::string goXYZ = ".go xyz " + std::to_string(player->GetPositionX()) + " " + std::to_string(player->GetPositionY()) + " " + std::to_string(player->GetPositionZ() + 1.0f) + " " + std::to_string(player->GetMap()->GetId()) + " " + std::to_string(player->GetOrientation());
                        SF_LOG_INFO("anticheat", "Anticheat Manager:: BG START - Hack detected player %s - (GUID %u) - Latency: %u ms - IP: %s - Cheat Flagged at: %s", player->GetName().c_str(), player->GetGUID(), latency, player->GetSession()->GetRemoteAddress().c_str(), goXYZ.c_str());
                        BuildReport(player, data, BG_START_HACK_REPORT);
                    }
                }
            }
            break;
        }
        return;
    }
}

void AnticheatMgr::SetAllowedMovement(Player* player, bool)
{
    player->SetCanTeleport(true);
}

void AnticheatMgr::HandlePlayerLogin(Player* player)
{
    // we must delete this to prevent errors in case of crash
    CharacterDatabase.PExecute("DELETE FROM players_reports_status WHERE guid=%u",player->GetGUIDLow());
    // we initialize the pos of lastMovementPosition var.
    _players[player->GetGUIDLow()].SetPosition(player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation());
    QueryResult resultDB = CharacterDatabase.PQuery("SELECT * FROM daily_players_reports WHERE guid=%u;",player->GetGUIDLow());
    
    if (resultDB)
        _players[player->GetGUIDLow()].SetDailyReportState(true);
}

void AnticheatMgr::HandlePlayerLogout(Player* player)
{
    // TO-DO Make a table that stores the cheaters of the day, with more detailed information.

    // We must also delete it at logout to prevent have data of offline players in the db when we query the database (IE: The GM Command)
    CharacterDatabase.PExecute("DELETE FROM players_reports_status WHERE guid=%u",player->GetGUIDLow());
    // Delete not needed data from the memory.
    _players.erase(player->GetGUIDLow());
}

void AnticheatMgr::SavePlayerData(Player* player)
{
    CharacterDatabase.PExecute("REPLACE INTO players_reports_status (guid,average,total_reports,speed_reports,fly_reports,jump_reports,waterwalk_reports,teleportplane_reports,climb_reports,time_maniputation_reports, gravity_reports, teleport_reports, bg_start_reports, creation_time) VALUES (%u,%f,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u);",player->GetGUIDLow(),_players[player->GetGUIDLow()].GetAverage(),_players[player->GetGUIDLow()].GetTotalReports(), _players[player->GetGUIDLow()].GetTypeReports(SPEED_HACK_REPORT),_players[player->GetGUIDLow()].GetTypeReports(FLY_HACK_REPORT),_players[player->GetGUIDLow()].GetTypeReports(JUMP_HACK_REPORT),_players[player->GetGUIDLow()].GetTypeReports(WALK_WATER_HACK_REPORT),_players[player->GetGUIDLow()].GetTypeReports(TELEPORT_PLANE_HACK_REPORT),_players[player->GetGUIDLow()].GetTypeReports(CLIMB_HACK_REPORT), _players[player->GetGUIDLow()].GetTypeReports(TIME_MANIPULATION_REPORT), _players[player->GetGUIDLow()].GetTypeReports(GRAVITY_HACK_REPORT), _players[player->GetGUIDLow()].GetTypeReports(TELEPORT_HACK_REPORT), _players[player->GetGUIDLow()].GetTypeReports(BG_START_HACK_REPORT), _players[player->GetGUIDLow()].GetCreationTime());
}

void AnticheatMgr::SavePlayerDataDaily(Player* player)
{
    CharacterDatabase.PExecute("REPLACE INTO players_reports_status (guid,average,total_reports,speed_reports,fly_reports,jump_reports,waterwalk_reports,teleportplane_reports,climb_reports,time_maniputation_reports, gravity_reports, teleport_reports, bg_start_reports, creation_time) VALUES (%u,%f,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u);", player->GetGUIDLow(), _players[player->GetGUIDLow()].GetAverage(), _players[player->GetGUIDLow()].GetTotalReports(), _players[player->GetGUIDLow()].GetTypeReports(SPEED_HACK_REPORT), _players[player->GetGUIDLow()].GetTypeReports(FLY_HACK_REPORT), _players[player->GetGUIDLow()].GetTypeReports(JUMP_HACK_REPORT), _players[player->GetGUIDLow()].GetTypeReports(WALK_WATER_HACK_REPORT), _players[player->GetGUIDLow()].GetTypeReports(TELEPORT_PLANE_HACK_REPORT), _players[player->GetGUIDLow()].GetTypeReports(CLIMB_HACK_REPORT), _players[player->GetGUIDLow()].GetTypeReports(TIME_MANIPULATION_REPORT), _players[player->GetGUIDLow()].GetTypeReports(GRAVITY_HACK_REPORT), _players[player->GetGUIDLow()].GetTypeReports(TELEPORT_HACK_REPORT), _players[player->GetGUIDLow()].GetTypeReports(BG_START_HACK_REPORT), _players[player->GetGUIDLow()].GetCreationTime());
}

void AnticheatMgr::OnPlayerMove(Player* player, MovementInfo &mi, uint32 opcode)
{
    if (!AccountMgr::IsAdminAccount(player->GetSession()->GetSecurity()) || sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_ENABLE_ON_GM))
        sAnticheatMgr->StartHackDetection(player, mi, opcode);
}

uint32 AnticheatMgr::GetTotalReports(uint32 lowGUID)
{
    AnticheatPlayersDataMap::iterator iter = _players.find(lowGUID);
    if (iter == _players.end())
        return 0;

    return iter->second.totalReports;
}

float AnticheatMgr::GetAverage(uint32 lowGUID)
{
    AnticheatPlayersDataMap::iterator iter = _players.find(lowGUID);
    if (iter == _players.end())
        return 0.0f;

    return iter->second.average;
}

uint32 AnticheatMgr::GetTypeReports(uint32 lowGUID, uint8 type)
{
    AnticheatPlayersDataMap::iterator iter = _players.find(lowGUID);
    if (iter == _players.end())
        return 0;

    return iter->second.typeReports[type];
}

bool AnticheatMgr::MustCheckTempReports(uint8 type)
{
    if (type == JUMP_HACK_REPORT)
        return false;

    if (type == GRAVITY_HACK_REPORT)
        return false;

    if (type == TELEPORT_HACK_REPORT)
        return false;

    if (type == BG_START_HACK_REPORT)
        return false;

    return true;
}

void AnticheatMgr::BuildReport(Player* player, AnticheatData& data, uint8 reportType)
{
    uint32 key = player->GetGUIDLow();

    if (MustCheckTempReports(reportType))
    {
        uint32 actualTime = getMSTime();

        if (!data.tempReportsTimer[reportType])
            data.tempReportsTimer[reportType] = actualTime;

        if (getMSTimeDiff(data.tempReportsTimer[reportType], actualTime) < 3000)
        {
            data.tempReports[reportType] += 1;

            if (data.tempReports[reportType] < 3)
                return;
        }
        else
        {
            data.tempReportsTimer[reportType] = actualTime;
            data.tempReports[reportType] = 1;
            return;
        }
    }

    // generating creationTime for average calculation
    if (!data.totalReports)
        data.creationTime = getMSTime();

    // increasing total_reports
    data.totalReports += 1;
    // increasing specific cheat report
    data.typeReports[reportType] += 1;

    // diff time for average calculation
    uint32 diffTime = getMSTimeDiff(data.creationTime, getMSTime()) / IN_MILLISECONDS;

    if (diffTime > 0)
    {
        // Average == Reports per second
        float average = float(data.totalReports) / float(diffTime);
        data.average = average;
    }

    if (sWorld->getIntConfig(WorldIntConfigs::CONFIG_ANTICHEAT_MAX_REPORTS_FOR_DAILY_REPORT) < data.GetTotalReports())
    {
        if (!data.GetDailyReportState())
        {
            CharacterDatabase.PExecute("REPLACE INTO daily_players_reports (guid,average,total_reports,speed_reports,fly_reports,jump_reports,waterwalk_reports,teleportplane_reports,climb_reports,time_maniputation_reports, gravity_reports, teleport_reports, bg_start_reports, creation_time) VALUES (%u,%f,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u);", player->GetGUIDLow(), _players[player->GetGUIDLow()].GetAverage(), _players[player->GetGUIDLow()].GetTotalReports(), _players[player->GetGUIDLow()].GetTypeReports(SPEED_HACK_REPORT), _players[player->GetGUIDLow()].GetTypeReports(FLY_HACK_REPORT), _players[player->GetGUIDLow()].GetTypeReports(JUMP_HACK_REPORT), _players[player->GetGUIDLow()].GetTypeReports(WALK_WATER_HACK_REPORT), _players[player->GetGUIDLow()].GetTypeReports(TELEPORT_PLANE_HACK_REPORT), _players[player->GetGUIDLow()].GetTypeReports(CLIMB_HACK_REPORT), _players[player->GetGUIDLow()].GetTypeReports(TIME_MANIPULATION_REPORT), _players[player->GetGUIDLow()].GetTypeReports(GRAVITY_HACK_REPORT), _players[player->GetGUIDLow()].GetTypeReports(TELEPORT_HACK_REPORT), _players[player->GetGUIDLow()].GetTypeReports(BG_START_HACK_REPORT), _players[player->GetGUIDLow()].GetCreationTime());
            data.SetDailyReportState(true);
        }
    }

    // Send warning to ingame GMs
    if (data.totalReports > sWorld->getIntConfig(WorldIntConfigs::CONFIG_ANTICHEAT_REPORTS_INGAME_NOTIFICATION))
    {
        uint32& count = data.reportAnnounceCount[reportType];
        ++count;
        time_t now = time(NULL);
        if (data.reportAnnounceCooldown[reportType] < now)
            data.reportAnnounceCooldown[reportType] = now + 3;
        else
            return;

        const char* reportTypeName;
        switch (reportType)
        {
            case SPEED_HACK_REPORT:             reportTypeName = "SpeedHack";           break;
            case FLY_HACK_REPORT:               reportTypeName = "FlyHack";             break;
            case WALK_WATER_HACK_REPORT:        reportTypeName = "WaterWalkHack";       break;
            case JUMP_HACK_REPORT:              reportTypeName = "JumpHack";            break;
            case TELEPORT_PLANE_HACK_REPORT:    reportTypeName = "TeleportPlaneHack";   break;
            case CLIMB_HACK_REPORT:             reportTypeName = "ClimbHack";           break;
            case TIME_MANIPULATION_REPORT:      reportTypeName = "TimeManipulation";    break;
            case GRAVITY_HACK_REPORT:           reportTypeName = "GravityHack";         break;
            case TELEPORT_HACK_REPORT:          reportTypeName = "TeleportHack";        break;
            case BG_START_HACK_REPORT:          reportTypeName = "BGStartHack";         break;
            default:                            reportTypeName = "?";                   break;
        }
        SF_LOG_DEBUG("network", "Anticheat Alert: %s detected as possible cheater. HackType: %u.", player->GetName().c_str(), reportType);
        sWorld->SendGMText(LANG_CHEATER_CHATLOG, "Anticheat Alert", player->GetName().c_str(), player->GetName().c_str(), reportTypeName, count < 10 ? "|cFF00FF00" : count < 20 ? "|cFFFF8000" : "|cFFFF0000", count);
        count = 0;
    }
    // Automatic Moderation, not recommended but not hated
    // Auto Kick
    if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_AUTOKICK_ENABLE))
    {
        if (data.totalReports > sWorld->getIntConfig(WorldIntConfigs::CONFIG_ANTICHEAT_MAX_REPORTS_FOR_KICKS))
        {
            SF_LOG_INFO("anticheat", "AnticheatMgr:: Reports reached assigned threshhold and counteracted by kicking player %s (GUID %u)", player->GetName().c_str(), player->GetGUID());
            // kick offender when reports are reached for automatic moderation
            player->GetSession()->KickPlayer();
        }
    }
    // Auto Ban
    if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_AUTOBAN_ENABLE))
    {
        if (data.totalReports > sWorld->getIntConfig(WorldIntConfigs::CONFIG_ANTICHEAT_MAX_REPORTS_FOR_BANS))
        {
            SF_LOG_INFO("anticheat", "AnticheatMgr:: Reports reached assigned threshhold and counteracted by banning player %s (GUID %u)", player->GetName().c_str(), player->GetGUID());
            //Auto Ban the Offender Indefinately their whole account.
            std::string accountName;
            AccountMgr::GetName(player->GetSession()->GetAccountId(), accountName);
            sWorld->BanAccount(BAN_ACCOUNT, accountName, "0s", "Anticheat module Auto Banned Account for Reach Cheat Threshhold", "Server");
        }
    }
    //Auto Jail
    if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_AUTOJAIL_ENABLE))
    {
        if (data.totalReports > sWorld->getIntConfig(WorldIntConfigs::CONFIG_ANTICHEAT_MAX_REPORTS_FOR_JAILS))
        {
            SF_LOG_INFO("anticheat", "AnticheatMgr:: Reports reached assigned threshhold and counteracted by jailing player %s (GUID %u)", player->GetName().c_str(), player->GetGUID());
            // This is where they end up going shaw shank redemption style
            WorldLocation loc = WorldLocation(1, 16226.5f, 16403.6f, -64.5f, 3.2f);// GM Jail Location
            player->TeleportTo(loc);// we defined loc as the jail location so we tele them there
            player->SetHomebind(loc, 876);// GM Jail Homebind location
            player->CastSpell(player, FREEZE);// freeze him in place to ensure no exploit happens for jail break attempt

            if (Aura* dungdesert = player->AddAura(LFG_SPELL_DUNGEON_DESERTER,player))// LFG_SPELL_DUNGEON_DESERTER
            {
                dungdesert->SetDuration(-1);
            }
            if (Aura* bgdesert = player->AddAura(BG_SPELL_DESERTER, player))// BG_SPELL_DESERTER
            {
                bgdesert->SetDuration(-1);
            }
            if (Aura* silent = player->AddAura(SILENCED, player))// SILENCED
            {
                silent->SetDuration(-1);
            }
        }
    }
}

void AnticheatMgr::AnticheatGlobalCommand(ChatHandler* handler)
{
    // MySQL will sort all for us, anyway this is not the best way we must only save the anticheat data not whole player's data!.
    for (SessionMap::const_iterator itr = sWorld->GetAllSessions().begin(); itr != sWorld->GetAllSessions().end(); ++itr)
        if (Player* plr = itr->second->GetPlayer())
        {
            sAnticheatMgr->SavePlayerData(plr);
            sAnticheatMgr->SavePlayerDataDaily(plr);
        }

    QueryResult resultDB = CharacterDatabase.Query("SELECT guid,average,total_reports FROM players_reports_status WHERE total_reports != 0 ORDER BY average ASC LIMIT 3;");
    if (!resultDB)
    {
        handler->PSendSysMessage("No players found.");
        return;
    }
    else
    {
        handler->SendSysMessage("=============================");
        handler->PSendSysMessage("Players with the lowest averages:");
        do
        {
            Field *fieldsDB = resultDB->Fetch();

            uint32 guid = fieldsDB[0].GetUInt32();
            float average = fieldsDB[1].GetFloat();
            uint32 total_reports = fieldsDB[2].GetUInt32();

            if (Player* player = sObjectMgr->GetPlayerByLowGUID(guid))
                handler->PSendSysMessage("Player: %s Average: %f Total Reports: %u", player->GetName().c_str(),average,total_reports);
        } while (resultDB->NextRow());
    }

    resultDB = CharacterDatabase.Query("SELECT guid,average,total_reports FROM players_reports_status WHERE total_reports != 0 ORDER BY total_reports DESC LIMIT 3;");
    // this should never happen
    if (!resultDB)
    {
        handler->PSendSysMessage("No players found.");
        return;
    }
    else
    {
        handler->SendSysMessage("=============================");
        handler->PSendSysMessage("Players with the more reports:");
        do
        {
            Field *fieldsDB = resultDB->Fetch();

            uint32 guid = fieldsDB[0].GetUInt32();
            float average = fieldsDB[1].GetFloat();
            uint32 total_reports = fieldsDB[2].GetUInt32();

            if (Player* player = sObjectMgr->GetPlayerByLowGUID(guid))
                handler->PSendSysMessage("Player: %s Total Reports: %u Average: %f", player->GetName().c_str(),total_reports,average);
        } while (resultDB->NextRow());
    }
}

void AnticheatMgr::AnticheatDeleteCommand(uint32 guid)
{
    if (!guid)
    {
        for (AnticheatPlayersDataMap::iterator it = _players.begin(); it != _players.end(); ++it)
            (*it).second.Reset();

        CharacterDatabase.PExecute("DELETE FROM players_reports_status;");
    }
    else
    {
        AnticheatPlayersDataMap::iterator iter = _players.find(guid);
        if (iter == _players.end())
            return;

        iter->second.Reset();
        CharacterDatabase.PExecute("DELETE FROM players_reports_status WHERE guid=%u;", guid);
    }
}

void AnticheatMgr::AnticheatPurgeCommand(ChatHandler* /*handler*/)
{
    CharacterDatabase.Execute("TRUNCATE TABLE daily_players_reports;");
}

void AnticheatMgr::ResetDailyReportStates()
{
     for (AnticheatPlayersDataMap::iterator it = _players.begin(); it != _players.end(); ++it)
         it->second.hasDailyReport = false;
}

uint32 AnticheatData::GetTotalReports() const
{
    return totalReports;
}

void AnticheatData::SetTotalReports(uint32 _totalReports)
{
    totalReports = _totalReports;
}

void AnticheatData::SetTypeReports(uint32 type, uint32 amount)
{
    typeReports[type] = amount;
}

uint32 AnticheatData::GetTypeReports(uint32 type) const
{
    return typeReports[type];
}

AnticheatData* AnticheatMgr::GetDataFor(Player* player)
{
    AnticheatPlayersDataMap::iterator iter = _players.find(player->GetGUIDLow());
    if (iter == _players.end())
    {
        return NULL;
    }

    return &iter->second;
}
