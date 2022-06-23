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

#include "AnticheatMgr.h"
#include "AnticheatScripts.h"
#include "AccountMgr.h"
#include "DatabaseEnv.h"
#include "DBCStores.h"
#include "Log.h"
#include "Map.h"
#include "MapManager.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "SharedDefines.h"
#include "SpellAuras.h"
#include "World.h"
#include "WorldSession.h"

constexpr auto LANG_ANTICHEAT_ALERT = 30087;
constexpr auto LANG_ANTICHEAT_TELEPORT = 30088;
constexpr auto LANG_ANTICHEAT_IGNORECONTROL = 30089;
constexpr auto LANG_ANTICHEAT_DUEL = 30090;

enum Spells
{
    SHACKLES = 38505,
    LFG_SPELL_DUNGEON_DESERTER = 71041,
    BG_SPELL_DESERTER = 26013,
    SILENCED = 23207,
    RESURRECTION_SICKNESS = 15007
};

AnticheatMgr::AnticheatMgr()
{
    /*
    **    May you do good and not evil.
    **    May you find forgiveness for yourself and forgive others.
    **    May you share freely, never taking more than you give.
    **    CTHULHU is watching ^(;,;)^
    */
}

AnticheatMgr::~AnticheatMgr()
{
    m_Players.clear();
}

void AnticheatMgr::JumpHackDetection(Player* player, MovementInfo /* movementInfo */, uint32 opcode)
{
    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_JUMPHACK_ENABLE))
        return;

    // we pull the player's individual guid
    ObjectGuid key = player->GetGUID();

    // Chain or double multi jumping is not a thing in 335
    if (m_Players[key].GetLastOpcode() == MSG_MOVE_JUMP && opcode == MSG_MOVE_JUMP)
    {
        if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_WRITELOG_ENABLE))
        {
            uint32 latency = 0;
            latency = player->GetSession()->GetLatency();
            SF_LOG_INFO("anticheat", "AnticheatMgr:: Jump-Hack detected player %s (%s) - Latency: %u ms", player->GetName().c_str(), player->GetGUID(), latency);
        }
        BuildReport(player, JUMP_HACK_REPORT);
    }
}

/*
void AnticheatMgr::WalkOnWaterHackDetection(Player* player, MovementInfo movementInfo)
{
    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_WATERWALKHACK_ENABLE))
        return;

    // we pull the player's individual guid
    ObjectGuid key = player->GetGUID();
    uint32 distance2D = (uint32)movementInfo.pos.GetExactDist2d(&m_Players[key].GetLastMovementInfo().pos);

    // We don't need to check for a waterwalk hack if the player hasn't moved
    // This is necessary since MovementHandler fires if you rotate the camera in place
    if (!distance2D)
        return;

    // if the player is water walking on water then we are good.
    if (player->GetLiquidStatus() == LIQUID_MAP_WATER_WALK && !player->IsFlying())
    {
        if (!m_Players[key].GetLastMovementInfo().HasMovementFlag(MOVEMENTFLAG_WATERWALKING) && !movementInfo.HasMovementFlag(MOVEMENTFLAG_WATERWALKING))
        {
            if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_WRITELOG_ENABLE))
            {
                uint32 latency = 0;
                latency = player->GetSession()->GetLatency();
                SF_LOG_INFO("anticheat", "AnticheatMgr:: Walk on Water - Hack detected player %s (%s) - Latency: %u ms", player->GetName().c_str(), player->GetGUID(), latency);
            }
            BuildReport(player, WALK_WATER_HACK_REPORT);
        }
    }

    // if we are a ghost we can walk on water
    if (!player->IsAlive())
        return;

    // Prevents the False Positive for water walking when you ressurrect.
    if (m_Players[key].GetLastOpcode() == SMSG_CORPSE_RECLAIM_DELAY)
        return;

    // if the player previous movement and current movement is water walking then we do a follow up check
    if (m_Players[key].GetLastMovementInfo().HasMovementFlag(MOVEMENTFLAG_WATERWALKING) && movementInfo.HasMovementFlag(MOVEMENTFLAG_WATERWALKING))
    { // if player has the following auras then we return
        if (player->HasAuraType(SPELL_AURA_WATER_WALK) || player->HasAuraType(SPELL_AURA_FEATHER_FALL) ||
            player->HasAuraType(SPELL_AURA_SAFE_FALL))
        {
            return;
        }

    }
    else if (!m_Players[key].GetLastMovementInfo().HasMovementFlag(MOVEMENTFLAG_WATERWALKING) && !movementInfo.HasMovementFlag(MOVEMENTFLAG_WATERWALKING))
    {
        //Boomer Review Time:
        //Return stops code execution of the entire function
        return;
    }

    if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_WRITELOG_ENABLE))
    {
        uint32 latency = 0;
        latency = player->GetSession()->GetLatency();
        SF_LOG_INFO("anticheat", "AnticheatMgr:: Walk on Water - Hack detected player %s (%s) - Latency: %u ms", player->GetName().c_str(), player->GetGUID(), latency);
    }
    BuildReport(player, WALK_WATER_HACK_REPORT);

}
*/

void AnticheatMgr::FlyHackDetection(Player* player, MovementInfo movementInfo)
{
    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_FLYHACK_ENABLE))
        return;

    ObjectGuid key = player->GetGUID();

    //we check to ensure they are not flying
    if (!m_Players[key].GetLastMovementInfo().HasMovementFlag(MOVEMENTFLAG_FLYING))
        return;

    //we check to see if they have legal flight auras
    if (player->HasAuraType(SPELL_AURA_FLY) ||
        player->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED) ||
        player->HasAuraType(SPELL_AURA_MOD_INCREASE_FLIGHT_SPEED))
        return;

    /*Thanks to @LilleCarl for info to check extra flag*/
    bool stricterChecks = true;
    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_FLYHACKSTRICT_ENABLE))
    {// super strict way to check, you can only ascend\descend in water and air, we check u are ascending\descending and not in water.
     // we are not checking for legal flight here because those checks were dont earlier.
        stricterChecks = !(movementInfo.HasMovementFlag(MOVEMENTFLAG_ASCENDING | MOVEMENTFLAG_DESCENDING) && !player->IsInWater());
    }

    // if you are not flying and not ascending then we do a return, you are then not guilty.
    if (!movementInfo.HasMovementFlag(MOVEMENTFLAG_CAN_FLY) && !movementInfo.HasMovementFlag(MOVEMENTFLAG_FLYING) && stricterChecks)
    {
        return;
    }

    if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_WRITELOG_ENABLE))
    {
        uint32 latency = 0;
        latency = player->GetSession()->GetLatency();
        SF_LOG_INFO("anticheat", "AnticheatMgr:: Fly-Hack detected player %s (%s) - Latency: %u ms", player->GetName().c_str(), player->GetGUID(), latency);
    }
    BuildReport(player, FLY_HACK_REPORT);
}

/*
void AnticheatMgr::TeleportPlaneHackDetection(Player* player, MovementInfo movementInfo, uint32 opcode)
{
    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_TELEPANEHACK_ENABLE))
        return;

    if (player->HasAuraType(SPELL_AURA_WATER_WALK) || player->HasAuraType(SPELL_AURA_WATER_BREATHING) || player->HasAuraType(SPELL_AURA_GHOST))
        return;

    ObjectGuid key = player->GetGUID();

    uint32 distance2D = (uint32)movementInfo.pos.GetExactDist2d(&m_Players[key].GetLastMovementInfo().pos);

    // We don't need to check for a water walking hack if the player hasn't moved
    // This is necessary since MovementHandler fires if you rotate the camera in place
    if (!distance2D)
        return;

    //Celestial Planetarium Observer Battle has a narrow path that false flags
    if (player && GetWMOAreaTableEntryByTripple(5202, 0, 24083))
        return;

    if (m_Players[key].GetLastOpcode() == MSG_MOVE_JUMP)
        return;

    if (opcode == (MSG_MOVE_FALL_LAND))
        return;

    if (player->GetLiquidStatus() == LIQUID_MAP_ABOVE_WATER)
        return;

    if (movementInfo.HasMovementFlag(MOVEMENTFLAG_FALLING | MOVEMENTFLAG_SWIMMING))
        return;

    // If he is flying we dont need to check
    if (movementInfo.HasMovementFlag(MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING))
        return;

    float pos_z = player->GetPositionZ();
    float ground_Z = player->GetFloorZ();
    float groundZ = player->GetMapHeight(player->GetPositionX(), player->GetPositionY(), MAX_HEIGHT);
    float floorZ = player->GetMapHeight(player->GetPositionX(), player->GetPositionY(), player->GetPositionZ());

    // we are not really walking there
    if (groundZ == floorZ && (fabs(ground_Z - pos_z) > 2.0f || fabs(ground_Z - pos_z) < -1.0f))
    {
        if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_WRITELOG_ENABLE))
        {
            uint32 latency = 0;
            latency = player->GetSession()->GetLatency();
            SF_LOG_INFO("anticheat", "AnticheatMgr:: Teleport To Plane - Hack detected player %s (%s) - Latency: %u ms", player->GetName().c_str(), player->GetGUID(), latency);
        }
        BuildReport(player, TELEPORT_PLANE_HACK_REPORT);
    }
}
*/

void AnticheatMgr::IgnoreControlHackDetection(Player* player, MovementInfo movementInfo, uint32 opcode)
{
    float x, y;
    player->GetPosition(x, y);
    ObjectGuid key = player->GetGUID();

    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_IGNORECONTROLHACK_ENABLE))
        return;

    if (m_Players[key].GetLastOpcode() == MSG_MOVE_JUMP)
        return;

    if (opcode == (MSG_MOVE_FALL_LAND))
        return;

    if (movementInfo.HasMovementFlag(MOVEMENTFLAG_FALLING | MOVEMENTFLAG_SWIMMING))
        return;

    uint32 latency = 0;
    latency = player->GetSession()->GetLatency() >= 400;
    //So here we check if hte player has a root state and not in a vehicle
    // except for lag, we can legitimately blame lag for false hits, so we see if they are above 400 then we exempt the check
    if (player->HasUnitState(UNIT_STATE_ROOT) && !player->GetVehicle() && !latency)
    {// Here we check if the x and y position changes while rooted, Nothing moves when rooted, no exception
        bool unrestricted = movementInfo.pos.GetPositionX() != x || movementInfo.pos.GetPositionY() != y;
        if (unrestricted)
        {// we do this because we can not get the collumn count being propper when we add more collumns for the report, so we make a indvidual warning for Ignore Control
            if (m_Players[key].GetTotalReports() > sWorld->getIntConfig(WorldIntConfigs::CONFIG_ANTICHEAT_REPORTS_INGAME_NOTIFICATION))
            {
                _alertFrequency = sWorld->getIntConfig(WorldIntConfigs::CONFIG_ANTICHEAT_ALERT_FREQUENCY);
                // So we dont divide by 0 by accident
                if (_alertFrequency < 1)
                    _alertFrequency = 1;
                if (++_counter % _alertFrequency == 0)
                {
                    // display warning at the center of the screen, hacky way?
                    std::string str = "|cFFFFFC00[Playername:|cFF00FFFF[|cFF60FF00" + std::string(player->GetName().c_str()) + "|cFF00FFFF] Possible Ignore Control Hack Detected!";
                    WorldPacket data(SMSG_NOTIFICATION, (str.size() + 1));
                    data << str;
                    sWorld->SendGlobalGMMessage(&data);
                    uint32 latency = 0;
                    latency = player->GetSession()->GetLatency();
                    // need better way to limit chat spam
                    if (m_Players[key].GetTotalReports() >= sWorld->getIntConfig(WorldIntConfigs::CONFIG_ANTICHEAT_REPORT_IN_CHAT_MIN) && m_Players[key].GetTotalReports() <= sWorld->getIntConfig(WorldIntConfigs::CONFIG_ANTICHEAT_REPORT_IN_CHAT_MAX))
                    {
                        sWorld->SendGMText(LANG_ANTICHEAT_IGNORECONTROL, player->GetName().c_str(), latency);
                    }
                    _counter = 0;
                }
            }
            if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_WRITELOG_ENABLE))
            {
                uint32 latency = 0;
                latency = player->GetSession()->GetLatency();
                SF_LOG_INFO("anticheat", "AnticheatMgr:: Ignore Control - Hack detected player %s (%s) - Latency: %u ms", player->GetName().c_str(), player->GetGUID(), latency);
            }
            BuildReport(player, IGNORE_CONTROL_REPORT);
        }
    }
}

void AnticheatMgr::ZAxisHackDetection(Player* player, MovementInfo movementInfo)
{
    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_ZAXISHACK_ENABLE))
        return;

   ObjectGuid key = player->GetGUID();

   // If he is flying we dont need to check
   if (movementInfo.HasMovementFlag(MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING))
       return;

   // If the player is allowed to waterwalk (or he is dead because he automatically waterwalks then) we dont need to check any further
   // We also stop if the player is in water, because otherwise you get a false positive for swimming
   if (movementInfo.HasMovementFlag(MOVEMENTFLAG_WATERWALKING) || player->IsInWater() || !player->IsAlive())
       return;

   //Celestial Planetarium Observer Battle has a narrow path that false flags
   if (player && GetWMOAreaTableEntryByTripple(5202, 0, 24083))
       return;

   // We want to exclude this LiquidStatus from detection because it leads to false positives.
   // Basically everytime you stand on a wmo game object in water
 //  if (player->GetLiquidStatus() == LIQUID_MAP_ABOVE_WATER)
 //      return;

   // This is Black Magic. Check only for x and y difference but no z difference that is greater then or equal to z +2.5 of the ground
   if (m_Players[key].GetLastMovementInfo().pos.GetPositionZ() == movementInfo.pos.GetPositionZ()
       && player->GetPositionZ() >= player->GetFloorZ() + 2.5f)
   {
       if (m_Players[key].GetTotalReports() > sWorld->getIntConfig(WorldIntConfigs::CONFIG_ANTICHEAT_REPORTS_INGAME_NOTIFICATION))
       {// we do this because we can not get the collumn count being propper when we add more collumns for the report, so we make a indvidual warning for Ignore Zaxis Hack
           _alertFrequency = sWorld->getIntConfig(WorldIntConfigs::CONFIG_ANTICHEAT_ALERT_FREQUENCY);
           // So we dont divide by 0 by accident
           if (_alertFrequency < 1)
               _alertFrequency = 1;
           if (++_counter % _alertFrequency == 0)
           {
                // display warning at the center of the screen, hacky way?
                std::string str = "|cFFFFFC00[Playername:|cFF00FFFF[|cFF60FF00" + std::string(player->GetName().c_str()) + "|cFF00FFFF] Possible Ignore Zaxis Hack Detected!";
                WorldPacket data(SMSG_NOTIFICATION, (str.size() + 1));
                data << str;
                sWorld->SendGlobalGMMessage(&data);
                uint32 latency = 0;
                latency = player->GetSession()->GetLatency();
                // need better way to limit chat spam
                if (m_Players[key].GetTotalReports() >= sWorld->getIntConfig(WorldIntConfigs::CONFIG_ANTICHEAT_REPORT_IN_CHAT_MIN) && m_Players[key].GetTotalReports() <= sWorld->getIntConfig(WorldIntConfigs::CONFIG_ANTICHEAT_REPORT_IN_CHAT_MAX))
                {
                    sWorld->SendGMText(LANG_ANTICHEAT_ALERT, player->GetName().c_str(), player->GetName().c_str(), latency);
                }
                _counter = 0;
           }
       }
       if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_WRITELOG_ENABLE))
       {
           uint32 latency = 0;
           latency = player->GetSession()->GetLatency();
           SF_LOG_INFO("anticheat", "AnticheatMgr:: Ignore Zaxis Hack detected player %s (%s) - Latency: %u ms", player->GetName().c_str(), player->GetGUID(), latency);
       }

       BuildReport(player, ZAXIS_HACK_REPORT);
   }

}

void AnticheatMgr::TeleportHackDetection(Player* player, MovementInfo movementInfo)
{
    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_TELEPORTHACK_ENABLE))
        return;

    ObjectGuid key = player->GetGUID();

    if (m_Players[key].GetLastMovementInfo().pos.GetPositionX() == movementInfo.pos.GetPositionX())
        return;

    float lastX = m_Players[key].GetLastMovementInfo().pos.GetPositionX();
    float newX = movementInfo.pos.GetPositionX();

    float lastY = m_Players[key].GetLastMovementInfo().pos.GetPositionY();
    float newY = movementInfo.pos.GetPositionY();

    float xDiff = fabs(lastX - newX);
    float yDiff = fabs(lastY - newY);
    /* Dueling exploit detection*/
    if (player->duel)
    {
        if ((xDiff >= 50.0f || yDiff >= 50.0f) && !player->CanTeleport())
        {
            Player* opponent = player->duel->opponent;

            std::string str = "|cFFFFFC00[DUEL ALERT Playername:|cFF00FFFF[|cFF60FF00" + std::string(player->GetName().c_str()) + "|cFF00FFFF] Possible Teleport Hack Detected! While Dueling [|cFF60FF00" + std::string(opponent->GetName().c_str()) + "|cFF00FFFF]";
            WorldPacket data(SMSG_NOTIFICATION, (str.size() + 1));
            data << str;
            sWorld->SendGlobalGMMessage(&data);
            uint32 latency = 0;
            latency = player->GetSession()->GetLatency();
            uint32 latency2 = 0;
            latency2 = opponent->GetSession()->GetLatency();
            sWorld->SendGMText(LANG_ANTICHEAT_DUEL, player->GetName().c_str(), latency, opponent->GetName().c_str(), latency2);

            if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_WRITELOG_ENABLE))
            {
                SF_LOG_INFO("anticheat", "AnticheatMgr:: DUEL ALERT Teleport-Hack detected player %s (%s) while dueling %s - Latency: %u ms", player->GetName().c_str(), player->GetGUID(), opponent->GetName().c_str(), latency);
                SF_LOG_INFO("anticheat", "AnticheatMgr:: DUEL ALERT Teleport-Hack detected player %s (%s) while dueling %s - Latency: %u ms", opponent->GetName().c_str(), opponent->GetGUID(), player->GetName().c_str(), latency2);
            }
            BuildReport(player, TELEPORT_HACK_REPORT);
            BuildReport(opponent, TELEPORT_HACK_REPORT);
        }
        else if (player->CanTeleport())
            player->SetCanTeleport(false);
    }
    /* Please work */
    if ((xDiff >= 50.0f || yDiff >= 50.0f) && !player->CanTeleport())// teleport helpers in play
    {
        if (m_Players[key].GetTotalReports() > sWorld->getIntConfig(WorldIntConfigs::CONFIG_ANTICHEAT_REPORTS_INGAME_NOTIFICATION))
        {// we do this because we can not get the collumn count being propper when we add more collumns for the report, so we make a indvidual warning for Teleport Hack
            _alertFrequency = sWorld->getIntConfig(WorldIntConfigs::CONFIG_ANTICHEAT_ALERT_FREQUENCY);
            // So we dont divide by 0 by accident
            if (_alertFrequency < 1)
                _alertFrequency = 1;
            if (++_counter % _alertFrequency == 0)
            {
                // display warning at the center of the screen, hacky way?
                std::string str = "|cFFFFFC00[Playername:|cFF00FFFF[|cFF60FF00" + std::string(player->GetName().c_str()) + "|cFF00FFFF] Possible Teleport Hack Detected!";
                WorldPacket data(SMSG_NOTIFICATION, (str.size() + 1));
                data << str;
                sWorld->SendGlobalGMMessage(&data);
                uint32 latency = 0;
                latency = player->GetSession()->GetLatency();
                // need better way to limit chat spam
                if (m_Players[key].GetTotalReports() >= sWorld->getIntConfig(WorldIntConfigs::CONFIG_ANTICHEAT_REPORT_IN_CHAT_MIN) && m_Players[key].GetTotalReports() <= sWorld->getIntConfig(WorldIntConfigs::CONFIG_ANTICHEAT_REPORT_IN_CHAT_MAX))
                {
                    sWorld->SendGMText(LANG_ANTICHEAT_TELEPORT, player->GetName().c_str(), latency);
                }
                _counter = 0;
            }
        }
        if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_WRITELOG_ENABLE))
        {
            uint32 latency = 0;
            latency = player->GetSession()->GetLatency();
            SF_LOG_INFO("anticheat", "AnticheatMgr:: Teleport-Hack detected player %s (%s) - Latency: %u ms", player->GetName().c_str(), player->GetGUID(), latency);
        }
        BuildReport(player, TELEPORT_HACK_REPORT);
    }
    else if (player->CanTeleport())// if we hit the teleport helpers in the source then we return it to false
        player->SetCanTeleport(false);
}

void AnticheatMgr::SetAllowedMovement(Player* player, bool)
{
    player->SetCanTeleport(true);
}

void AnticheatMgr::StartHackDetection(Player* player, MovementInfo movementInfo, uint32 opcode)
{
    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_ENABLE))
        return;

    // GMs are the enforcer of the server, they should be exempt.
    if (player->IsGameMaster())
        return;

    ObjectGuid key = player->GetGUID();

    if (player->IsInFlight() || player->GetTransport() || player->GetVehicle())
    {
        m_Players[key].SetLastMovementInfo(movementInfo);
        m_Players[key].SetLastOpcode(opcode);
        return;
    }

    // Dear future me. Please forgive me.
    // I can't even begin to express how sorry I am for this order
    SpeedHackDetection(player, movementInfo);
    FlyHackDetection(player, movementInfo);
    JumpHackDetection(player, movementInfo, opcode);
//    TeleportPlaneHackDetection(player, movementInfo, opcode);
    ClimbHackDetection(player, movementInfo, opcode);
    TeleportHackDetection(player, movementInfo);
    IgnoreControlHackDetection(player, movementInfo, opcode);
    GravityHackDetection(player, movementInfo);
//    if (player->GetLiquidStatus() == LIQUID_MAP_WATER_WALK)
//    {
//        WalkOnWaterHackDetection(player, movementInfo);
//    }
//    else
//    {
        ZAxisHackDetection(player, movementInfo);
//    }
//    if (player->GetLiquidStatus() == LIQUID_MAP_UNDER_WATER)
//    {
//        AntiSwimHackDetection(player, movementInfo, opcode);
//    }
    m_Players[key].SetLastMovementInfo(movementInfo);
    m_Players[key].SetLastOpcode(opcode);
}

// basic detection
void AnticheatMgr::ClimbHackDetection(Player* player, MovementInfo movementInfo, uint32 opcode)
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

    float diffz = fabs(movementInfo.pos.GetPositionZ() - playerPos.GetPositionZ());
    float tanangle = movementInfo.pos.GetExactDist2d(&playerPos) / diffz;

    if (!player->HasUnitMovementFlag(MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING | MOVEMENTFLAG_SWIMMING))
    {
        if (movementInfo.pos.GetPositionZ() > playerPos.GetPositionZ() &&
            diffz > 1.87f && tanangle < 0.57735026919f) // 30 degrees
        {
            if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_WRITELOG_ENABLE))
            {
                uint32 latency = 0;
                latency = player->GetSession()->GetLatency();
                SF_LOG_INFO("anticheat", "AnticheatMgr:: Climb-Hack detected player %s (%s) - Latency: %u ms", player->GetName().c_str(), player->GetGUID(), latency);
            }

            BuildReport(player, CLIMB_HACK_REPORT);
        }
    }

}

/*
void AnticheatMgr::AntiSwimHackDetection(Player* player, MovementInfo movementInfo, uint32 opcode)
{
    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_ANTISWIM_ENABLE))
        return;

    if (player->GetAreaId())
    {
        switch (player->GetAreaId())
        {
            case 2100: //Maraudon https://github.com/TrinityCore/TrinityCore/issues/27946
                return;
        }
    }

    if (player->GetLiquidStatus() == (LIQUID_MAP_ABOVE_WATER | LIQUID_MAP_WATER_WALK | LIQUID_MAP_IN_WATER))
        return;

    if (opcode == MSG_MOVE_JUMP)
        return;

    if (movementInfo.HasMovementFlag(MOVEMENTFLAG_FALLING | MOVEMENTFLAG_SWIMMING))
        return;

    if (player->GetLiquidStatus() == LIQUID_MAP_UNDER_WATER && !movementInfo.HasMovementFlag(MOVEMENTFLAG_SWIMMING))
    {
        if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_WRITELOG_ENABLE))
        {
            uint32 latency = 0;
            latency = player->GetSession()->GetLatency();
            SF_LOG_INFO("anticheat", "AnticheatMgr:: Anti-Swim-Hack detected player %s (%s) - Latency: %u ms", player->GetName().c_str(), player->GetGUID(), latency);
        }

        BuildReport(player, ANTISWIM_HACK_REPORT);

    }
}
*/

void AnticheatMgr::GravityHackDetection(Player* player, MovementInfo movementInfo)
{
    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_GRAVITY_ENABLE))
        return;

    if (player->HasAuraType(SPELL_AURA_FEATHER_FALL))
    {
        return;
    }

    ObjectGuid key = player->GetGUID();
    if (m_Players[key].GetLastOpcode() == MSG_MOVE_JUMP)
    {
        if (!player->HasUnitMovementFlag(MOVEMENTFLAG_DISABLE_GRAVITY) && movementInfo.jump.zspeed < -10.0f)
        {
            if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_WRITELOG_ENABLE))
            {
                uint32 latency = 0;
                latency = player->GetSession()->GetLatency();
                SF_LOG_INFO("anticheat", "AnticheatMgr:: Gravity-Hack detected player %s (%s) - Latency: %u ms", player->GetName().c_str(), player->GetGUID(), latency);
            }
            BuildReport(player, GRAVITY_HACK_REPORT);
        }
    }
}

void AnticheatMgr::SpeedHackDetection(Player* player, MovementInfo movementInfo)
{
    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_SPEEDHACK_ENABLE))
        return;

    ObjectGuid key = player->GetGUID();

    // We also must check the map because the movementFlag can be modified by the client.
    // If we just check the flag, they could always add that flag and always skip the speed hacking detection.
    // Abandon all hope ye who enter beyond this point
    // We exempt all transports found in 335 to prevent false speed hack hits
    if (m_Players[key].GetLastMovementInfo().HasMovementFlag(MOVEMENTFLAG_DISABLE_GRAVITY) && player->GetMapId())
    {
        switch (player->GetMapId())
        {
            case 369: //Transport: DEEPRUN TRAM
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
                return;
        }
    }

    // sometimes I believe the compiler ignores all my comments
    uint32 distance2D = (uint32)movementInfo.pos.GetExactDist2d(&m_Players[key].GetLastMovementInfo().pos);

    // We don't need to check for a speedhack if the player hasn't moved
    // This is necessary since MovementHandler fires if you rotate the camera in place
    if (!distance2D)
        return;

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
    // We remove the added speed for jumping because otherwise permanently jumping doubles your allowed speed
    uint32 speedRate = (uint32)(player->GetSpeed(UnitMoveType(moveType)));

    // how long the player took to move to here.
    uint32 timeDiff = getMSTimeDiff(m_Players[key].GetLastMovementInfo().time, movementInfo.time);

    // Ah ah ah! You'll never understand why this one works. Or will you?
    // This covers packet manipulation
    if (int32(timeDiff) < 0)
    {
        BuildReport(player, SPEED_HACK_REPORT);
        timeDiff = 1;
    }

    if (!timeDiff)
        timeDiff = 1;

    // this is the distance doable by the player in 1 sec, using the time done to move to this point.
    uint32 clientSpeedRate = distance2D * 1000 / timeDiff; // Only Chuck Norris can divide by zero so we divide by 1

    // We did the (uint32) cast to accept a margin of tolerance
    // We check the last MovementInfo for the falling flag since falling down a hill and sliding a bit triggered a false positive
    if ((clientSpeedRate > speedRate) && !m_Players[key].GetLastMovementInfo().HasMovementFlag(MOVEMENTFLAG_FALLING))
    {
        if (!player->CanTeleport())
        {
            if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_WRITELOG_ENABLE))
            {
                uint32 latency = 0;
                latency = player->GetSession()->GetLatency();
                SF_LOG_INFO("anticheat", "AnticheatMgr:: Speed-Hack detected player %s (%s) - Latency: %u ms", player->GetName().c_str(), player->GetGUID(), latency);
            }
            BuildReport(player, SPEED_HACK_REPORT);
        }
        return;
    }
}

void AnticheatMgr::StartScripts()
{
    new AnticheatScripts();
}

void AnticheatMgr::HandlePlayerLogin(Player* player)
{
    // we must delete this to prevent errors in case of crash
    CharacterDatabase.PExecute("DELETE FROM players_reports_status WHERE guid=%u", player->GetGUID());
    // we initialize the pos of lastMovementPosition var.
    m_Players[player->GetGUID()].SetPosition(player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation());
    QueryResult resultDB = CharacterDatabase.PQuery("SELECT * FROM daily_players_reports WHERE guid=%u;", player->GetGUID());

    if (resultDB)
        m_Players[player->GetGUID()].SetDailyReportState(true);
}

void AnticheatMgr::HandlePlayerLogout(Player* player)
{
    // TO-DO Make a table that stores the cheaters of the day, with more detailed information.

    // We must also delete it at logout to prevent have data of offline players in the db when we query the database (IE: The GM Command)
    CharacterDatabase.PExecute("DELETE FROM players_reports_status WHERE guid=%u", player->GetGUID());
    // Delete not needed data from the memory.
    m_Players.erase(player->GetGUID());
}

void AnticheatMgr::SavePlayerData(Player* player)
{
    CharacterDatabase.PExecute("REPLACE INTO players_reports_status (guid,average,total_reports,speed_reports,fly_reports,jump_reports,waterwalk_reports,teleportplane_reports,climb_reports,teleport_reports,ignorecontrol_reports,zaxis_reports,antiswim_reports,gravity_reports,creation_time) VALUES (%u,%f,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u);", player->GetGUID(), m_Players[player->GetGUID()].GetAverage(), m_Players[player->GetGUID()].GetTotalReports(), m_Players[player->GetGUID()].GetTypeReports(SPEED_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(FLY_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(JUMP_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(WALK_WATER_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(TELEPORT_PLANE_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(CLIMB_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(TELEPORT_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(IGNORE_CONTROL_REPORT), m_Players[player->GetGUID()].GetTypeReports(ZAXIS_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(ANTISWIM_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(GRAVITY_HACK_REPORT), m_Players[player->GetGUID()].GetCreationTime());
}

void AnticheatMgr::SavePlayerDataDaily(Player* player)
{
    CharacterDatabase.PExecute("REPLACE INTO daily_players_reports (guid,average,total_reports,speed_reports,fly_reports,jump_reports,waterwalk_reports,teleportplane_reports,climb_reports,teleport_reports,ignorecontrol_reports,zaxis_reports,antiswim_reports,gravity_reports,creation_time) VALUES (%u,%f,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u);", player->GetGUID(), m_Players[player->GetGUID()].GetAverage(), m_Players[player->GetGUID()].GetTotalReports(), m_Players[player->GetGUID()].GetTypeReports(SPEED_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(FLY_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(JUMP_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(WALK_WATER_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(TELEPORT_PLANE_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(CLIMB_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(TELEPORT_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(IGNORE_CONTROL_REPORT), m_Players[player->GetGUID()].GetTypeReports(ZAXIS_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(ANTISWIM_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(GRAVITY_HACK_REPORT), m_Players[player->GetGUID()].GetCreationTime());
}

void AnticheatMgr::OnPlayerMove(Player* player, MovementInfo mi, uint32 opcode)
{
    if (!AccountMgr::IsAdminAccount(player->GetSession()->GetSecurity()) || sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_ENABLE_ON_GM))
        sAnticheatMgr->StartHackDetection(player, mi, opcode);
}

void AnticheatMgr::SavePlayerDataDaily(Player* player)
{
    CharacterDatabase.PExecute("REPLACE INTO daily_players_reports (guid,average,total_reports,speed_reports,fly_reports,jump_reports,waterwalk_reports,teleportplane_reports,climb_reports,teleport_reports,ignorecontrol_reports,zaxis_reports,antiswim_reports,gravity_reports,creation_time) VALUES (%u,%f,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u);", player->GetGUID(), m_Players[player->GetGUID()].GetAverage(), m_Players[player->GetGUID()].GetTotalReports(), m_Players[player->GetGUID()].GetTypeReports(SPEED_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(FLY_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(JUMP_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(WALK_WATER_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(TELEPORT_PLANE_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(CLIMB_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(TELEPORT_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(IGNORE_CONTROL_REPORT), m_Players[player->GetGUID()].GetTypeReports(ZAXIS_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(ANTISWIM_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(GRAVITY_HACK_REPORT), m_Players[player->GetGUID()].GetCreationTime());
}

void AnticheatMgr::OnPlayerMove(Player* player, MovementInfo mi, uint32 opcode)
{
    if (!AccountMgr::IsAdminAccount(player->GetSession()->GetSecurity()) || sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_ENABLE_ON_GM))
        sAnticheatMgr->StartHackDetection(player, mi, opcode);
}

uint32 AnticheatMgr::GetTotalReports(uint32 lowGUID)
{
    return m_Players[lowGUID].GetTotalReports();
}

float AnticheatMgr::GetAverage(uint32 lowGUID)
{
    return m_Players[lowGUID].GetAverage();
}

uint32 AnticheatMgr::GetTypeReports(uint32 lowGUID, uint8 type)
{
    return m_Players[lowGUID].GetTypeReports(type);
}

bool AnticheatMgr::MustCheckTempReports(uint8 type)
{
    if (type == JUMP_HACK_REPORT)
        return false;

    if (type == TELEPORT_HACK_REPORT)
        return false;

    if (type == IGNORE_CONTROL_REPORT)
        return false;

    if (type == GRAVITY_HACK_REPORT)
        return false;

    return true;
}

//
// Dear maintainer:
//
// Once you are done trying to 'optimize' this script,
// and have identify potentionally if there was a terrible
// mistake that was here or not, please increment the
// following counter as a warning to the next guy:
//
// total_hours_wasted_here = 42
//

void AnticheatMgr::BuildReport(Player* player, uint8 reportType)
{
    ObjectGuid key = player->GetGUID();

    if (MustCheckTempReports(reportType))
    {
        uint32 actualTime = getMSTime();

        if (!m_Players[key].GetTempReportsTimer(reportType))
            m_Players[key].SetTempReportsTimer(actualTime, reportType);

        if (getMSTimeDiff(m_Players[key].GetTempReportsTimer(reportType), actualTime) < 3000)
        {
            m_Players[key].SetTempReports(m_Players[key].GetTempReports(reportType) + 1, reportType);

            if (m_Players[key].GetTempReports(reportType) < 3)
                return;
        }
        else
        {
            m_Players[key].SetTempReportsTimer(actualTime, reportType);
            m_Players[key].SetTempReports(1, reportType);
            return;
        }
    }

    // generating creationTime for average calculation
    if (!m_Players[key].GetTotalReports())
        m_Players[key].SetCreationTime(getMSTime());

    // increasing total_reports
    m_Players[key].SetTotalReports(m_Players[key].GetTotalReports() + 1);
    // increasing specific cheat report
    m_Players[key].SetTypeReports(reportType, m_Players[key].GetTypeReports(reportType) + 1);

    // diff time for average calculation
    uint32 diffTime = getMSTimeDiff(m_Players[key].GetCreationTime(), getMSTime()) / IN_MILLISECONDS;

    if (diffTime > 0)
    {
        // Average == Reports per second
        float average = float(m_Players[key].GetTotalReports()) / float(diffTime);
        m_Players[key].SetAverage(average);
    }

    if (sWorld->getIntConfig(WorldIntConfigs::CONFIG_ANTICHEAT_MAX_REPORTS_FOR_DAILY_REPORT) < m_Players[key].GetTotalReports())
    {
        if (!m_Players[key].GetDailyReportState())
        {
            CharacterDatabase.PExecute("REPLACE INTO players_reports_status (guid,average,total_reports,speed_reports,fly_reports,jump_reports,waterwalk_reports,teleportplane_reports,climb_reports,teleport_reports,ignorecontrol_reports,zaxis_reports,antiswim_reports,gravity_reports,creation_time) VALUES (%u,%f,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u);", player->GetGUID(), m_Players[player->GetGUID()].GetAverage(), m_Players[player->GetGUID()].GetTotalReports(), m_Players[player->GetGUID()].GetTypeReports(SPEED_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(FLY_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(JUMP_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(WALK_WATER_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(TELEPORT_PLANE_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(CLIMB_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(TELEPORT_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(IGNORE_CONTROL_REPORT), m_Players[player->GetGUID()].GetTypeReports(ZAXIS_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(ANTISWIM_HACK_REPORT), m_Players[player->GetGUID()].GetTypeReports(GRAVITY_HACK_REPORT), m_Players[player->GetGUID()].GetCreationTime());
            m_Players[key].SetDailyReportState(true);
        }
    }

    if (m_Players[key].GetTotalReports() > sWorld->getIntConfig(WorldIntConfigs::CONFIG_ANTICHEAT_REPORTS_INGAME_NOTIFICATION))
    {
        _alertFrequency = sWorld->getIntConfig(WorldIntConfigs::CONFIG_ANTICHEAT_ALERT_FREQUENCY);
        // So we dont divide by 0 by accident
        if (_alertFrequency < 1)
            _alertFrequency = 1;
        if (++_counter % _alertFrequency == 0)
        {
            // display warning at the center of the screen, hacky way?
            std::string str = "|cFFFFFC00[Playername:]|cFF00FFFF[|cFF60FF00" + std::string(player->GetName().c_str()) + "|cFF00FFFF] Possible cheater!";
            WorldPacket data(SMSG_NOTIFICATION, (str.size() + 1));
            data << str;
            sWorld->SendGlobalGMMessage(&data);
        }
        // need better way to limit chat spam
        if (m_Players[key].GetTotalReports() >= sWorld->getIntConfig(WorldIntConfigs::CONFIG_ANTICHEAT_REPORT_IN_CHAT_MIN) && (m_Players[key].GetTotalReports() <= sWorld->getIntConfig(WorldIntConfigs::CONFIG_ANTICHEAT_REPORT_IN_CHAT_MAX)))
        {
            uint32 latency = 0;
            latency = player->GetSession()->GetLatency();
            sWorld->SendGMText(LANG_ANTICHEAT_ALERT, player->GetName().c_str(), player->GetName().c_str(), latency);
        }
        _counter = 0;
    }
    // Automatic Moderation, not recommended but not hated
    // Auto Kick
    if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_AUTOKICK_ENABLE))
    {
        if (m_Players[key].GetTotalReports() > sWorld->getIntConfig(WorldIntConfigs::CONFIG_ANTICHEAT_MAX_REPORTS_FOR_KICKS))
        {
            if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_WRITELOG_ENABLE))
            {
                SF_LOG_INFO("anticheat", "AnticheatMgr:: Reports reached assigned threshhold and counteracted by kicking player %s (%s)", player->GetName().c_str(), player->GetGUID());
            }
            // display warning at the center of the screen, hacky way?
            std::string str = "|cFFFFFC00[Playername:|cFF00FFFF[|cFF60FF00" + std::string(player->GetName().c_str()) + "|cFF00FFFF] Auto Kicked for Reaching Cheat Threshhold!";
            WorldPacket data(SMSG_NOTIFICATION, (str.size() + 1));
            data << str;
            sWorld->SendGlobalGMMessage(&data);
            // kick offender when reports are reached for automatic moderation
            player->GetSession()->KickPlayer();
            // publically shame them with a server message
            if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_ANNOUNCEKICK_ENABLE))
            {
                std::string plr = player->GetName();
                std::string tag_colour = "7bbef7";
                std::string plr_colour = "ff0000";
                std::ostringstream stream;
                stream << "|CFF" << plr_colour << "[AntiCheat]|r|CFF" << tag_colour <<
                    " Player |r|cff" << plr_colour << plr << "|r|cff" << tag_colour <<
                    " has been kicked by the Anticheat Module.|r";
                sWorld->SendServerMessage(SERVER_MSG_STRING, stream.str().c_str());
            }
        }
    }
    // Auto Ban
    if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_AUTOBAN_ENABLE))
    {
        if (m_Players[key].GetTotalReports() > sWorld->getIntConfig(WorldIntConfigs::CONFIG_ANTICHEAT_MAX_REPORTS_FOR_BANS))
        {
            if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_WRITELOG_ENABLE))
            {
                SF_LOG_INFO("anticheat", "AnticheatMgr:: Reports reached assigned threshhold and counteracted by banning player %s (%s)", player->GetName().c_str(), player->GetGUID());
            }
            // display warning at the center of the screen, hacky way?
            std::string str = "|cFFFFFC00[Playername:|cFF00FFFF[|cFF60FF00" + std::string(player->GetName().c_str()) + "|cFF00FFFF] Auto Banned Account for Reaching Cheat Threshhold!";
            WorldPacket data(SMSG_NOTIFICATION, (str.size() + 1));
            data << str;
            sWorld->SendGlobalGMMessage(&data);
            //Auto Ban the Offender Indefinately their whole account.
            std::string accountName;
            AccountMgr::GetName(player->GetSession()->GetAccountId(), accountName);
            sWorld->BanAccount(BAN_ACCOUNT, accountName, "0s", "Anticheat module Auto Banned Account for Reach Cheat Threshhold", "Server");
            // publically shame them with a server message
            if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_ANNOUNCEBAN_ENABLE))
            {
                std::string plr = player->GetName();
                std::string tag_colour = "7bbef7";
                std::string plr_colour = "ff0000";
                std::ostringstream stream;
                stream << "|CFF" << plr_colour << "[AntiCheat]|r|CFF" << tag_colour <<
                    " Player |r|cff" << plr_colour << plr << "|r|cff" << tag_colour <<
                    " has been Banned by the Anticheat Module.|r";
                sWorld->SendServerMessage(SERVER_MSG_STRING, stream.str().c_str());
            }
        }
    }
    //Auto Jail
    if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_AUTOJAIL_ENABLE))
    {
        if (m_Players[key].GetTotalReports() > sWorld->getIntConfig(WorldIntConfigs::CONFIG_ANTICHEAT_MAX_REPORTS_FOR_JAILS))
        {
            if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_WRITELOG_ENABLE))
            {
                SF_LOG_INFO("anticheat", "AnticheatMgr:: Reports reached assigned threshhold and counteracted by jailing player %s (%s)", player->GetName().c_str(), player->GetGUID());
            }
            // display warning at the center of the screen, hacky way?
            std::string str = "|cFFFFFC00[Playername:|cFF00FFFF[|cFF60FF00" + std::string(player->GetName().c_str()) + "|cFF00FFFF] Auto Jailed Account for Reaching Cheat Threshhold!";
            WorldPacket data(SMSG_NOTIFICATION, (str.size() + 1));
            data << str;
            sWorld->SendGlobalGMMessage(&data);
            // This is where they end up going shaw shank redemption style
            WorldLocation loc = WorldLocation(1, 16226.5f, 16403.6f, -64.5f, 3.2f);// GM Jail Location
            player->TeleportTo(loc);// we defined loc as the jail location so we tele them there
            player->SetHomebind(loc, 876);// GM Jail Homebind location
            player->CastSpell(player, SHACKLES);// shackle him in place to ensure no exploit happens for jail break attempt

            if (Aura* dungdesert = player->AddAura(LFG_SPELL_DUNGEON_DESERTER, player))// LFG_SPELL_DUNGEON_DESERTER
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

            // publically shame them with a server message
            if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_ANNOUNCEJAIL_ENABLE))
            {
                std::string plr = player->GetName();
                std::string tag_colour = "7bbef7";
                std::string plr_colour = "ff0000";
                std::ostringstream stream;
                stream << "|CFF" << plr_colour << "[AntiCheat]|r|CFF" << tag_colour <<
                    " Player |r|cff" << plr_colour << plr << "|r|cff" << tag_colour <<
                    " has been Jailed by the Anticheat Module.|r";
                sWorld->SendServerMessage(SERVER_MSG_STRING, stream.str().c_str());
            }
        }
    }
}
// these are the supporters for the gm commands in cs_anticheat.cpp
void AnticheatMgr::AnticheatGlobalCommand(ChatHandler* handler)
{   // .anticheat global gm command
    // save All Anticheat Player Data before displaying global stats
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
            Field* fieldsDB = resultDB->Fetch();

            uint32 guid = fieldsDB[0].GetUInt32();
            float average = fieldsDB[1].GetFloat();
            uint32 total_reports = fieldsDB[2].GetUInt32();

            if (Player* player = HashMapHolder<Player>::Find(guid))
                handler->PSendSysMessage("Player: %s Average: %f Total Reports: %u", player->GetName().c_str(), average, total_reports);

        } while (resultDB->NextRow());
    }

    resultDB = CharacterDatabase.Query("SELECT guid,average,total_reports FROM players_reports_status WHERE total_reports != 0 ORDER BY total_reports DESC LIMIT 3;");

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
            Field* fieldsDB = resultDB->Fetch();

            uint32 guid = fieldsDB[0].GetUInt32();
            float average = fieldsDB[1].GetFloat();
            uint32 total_reports = fieldsDB[2].GetUInt32();

            if (Player* player = HashMapHolder<Player>::Find(guid))
                handler->PSendSysMessage("Player: %s Total Reports: %u Average: %f", player->GetName().c_str(), total_reports, average);

        } while (resultDB->NextRow());
    }
}

// .anticheat delete gm cmd
void AnticheatMgr::AnticheatDeleteCommand(uint32 guid)
{
    if (!guid)
    {
        for (AnticheatPlayersDataMap::iterator it = m_Players.begin(); it != m_Players.end(); ++it)
        {
            (*it).second.SetTotalReports(0);
            (*it).second.SetAverage(0);
            (*it).second.SetCreationTime(0);
            for (uint8 i = 0; i < MAX_REPORT_TYPES; i++)
            {
                (*it).second.SetTempReports(0, i);
                (*it).second.SetTempReportsTimer(0, i);
                (*it).second.SetTypeReports(i, 0);
            }
        }
        CharacterDatabase.PExecute("DELETE FROM players_reports_status;");
    }
    else
    {
        m_Players[guid].SetTotalReports(0);
        m_Players[guid].SetAverage(0);
        m_Players[guid].SetCreationTime(0);
        for (uint8 i = 0; i < MAX_REPORT_TYPES; i++)
        {
            m_Players[guid].SetTempReports(0, i);
            m_Players[guid].SetTempReportsTimer(0, i);
            m_Players[guid].SetTypeReports(i, 0);
        }
        CharacterDatabase.PExecute("DELETE FROM players_reports_status WHERE guid=%u;", guid);
    }
}

// .anticheat purge gm cmd
void AnticheatMgr::AnticheatPurgeCommand(ChatHandler* /*handler*/)
{// we purge the whole daily_players_reports table in the character database
    CharacterDatabase.Execute("TRUNCATE TABLE daily_players_reports;");
}

void AnticheatMgr::ResetDailyReportStates()
{// this resets the daily reports to zero
    for (AnticheatPlayersDataMap::iterator it = m_Players.begin(); it != m_Players.end(); ++it)
        m_Players[(*it).first].SetDailyReportState(false);
}
