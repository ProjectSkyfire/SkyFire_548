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

#include "AccountMgr.h"
#include "Language.h"
#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "ObjectAccessor.h"
#include "Chat.h"
#include "Player.h"
#include "World.h"
#include "WorldSession.h"
#include "SpellAuras.h"
#include "../../game/Anticheat/AnticheatMgr.h"

enum Spells
{
    SHACKLES = 38505,
    LFG_SPELL_DUNGEON_DESERTER = 71041,
    BG_SPELL_DESERTER = 26013,
    SILENCED = 23207
};

class anticheat_commandscript : public CommandScript
{
public:
    anticheat_commandscript() : CommandScript("anticheat_commandscript") { }

    std::vector<ChatCommand> GetCommands() const
    {
        static std::vector<ChatCommand> anticheatCommandTable =
        {
            { "global",      rbac::RBAC_ROLE_GAMEMASTER,    true, &HandleAntiCheatGlobalCommand,              "", },
            { "player",      rbac::RBAC_ROLE_GAMEMASTER,    true, &HandleAntiCheatPlayerCommand,              "", },
            { "delete",      rbac::RBAC_ROLE_ADMINISTRATOR, true, &HandleAntiCheatDeleteCommand,              "", },
            { "purge",       rbac::RBAC_ROLE_ADMINISTRATOR, true, &HandleAntiCheatPurgeCommand,               "", },
            { "handle",      rbac::RBAC_ROLE_ADMINISTRATOR, true, &HandleAntiCheatHandleCommand,              "", },
            { "jail",        rbac::RBAC_ROLE_GAMEMASTER,    true, &HandleAnticheatJailCommand,                "", },
            { "parole",      rbac::RBAC_ROLE_ADMINISTRATOR, true, &HandleAnticheatParoleCommand,              "", },
            { "warn",        rbac::RBAC_ROLE_GAMEMASTER,    true, &HandleAnticheatWarnCommand,                "", },
        };

        static std::vector<ChatCommand> commandTable =
        {
            { "anticheat",   rbac::RBAC_ROLE_ADMINISTRATOR, true,   NULL, "",  anticheatCommandTable},
        };

        return commandTable;
    }

    static bool HandleAnticheatWarnCommand(ChatHandler* handler, char const* args)
    {
        if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_ENABLE))
            return false;

        if (!*args)
            return false;

        Player* target;
        if (!handler->extractPlayerTarget((char*)args, &target))
            return false;

        ChatHandler(GetSession()).PSendSysMessage("The anticheat system has reported several times that you may be cheating. You will be monitored to confirm if this is accurate.");
        return true;
    }

    static bool HandleAnticheatJailCommand(ChatHandler* handler, char const* args)
    {
        if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_ENABLE))
            return false;

        if (!*args)
            return false;

        Player* target;
        if (!handler->extractPlayerTarget((char*)args, &target))
            return false;

        Player* pTarget = player->GetConnectedPlayer();

        AccountTypes playerSecurity;
        if (handler->GetSession())
            playerSecurity = AccountMgr::GetSecurity(handler->GetSession()->GetAccountId(), gmRealmID);
        else
            playerSecurity = AccountTypes::SEC_CONSOLE;

        // teleport both to jail.
        if (!AccountMgr::IsConsoleAccount(playerSecurity))
        {
            handler->GetSession()->GetPlayer()->TeleportTo(1, 16226.5f, 16403.6f, -64.5f, 3.2f);
        }

        WorldLocation loc = WorldLocation(1, 16226.5f, 16403.6f, -64.5f, 3.2f);// GM Jail Location
        pTarget->TeleportTo(loc);
        pTarget->SetHomebind(loc, 876);// GM Jail Homebind location
        pTarget->CastSpell(pTarget, SHACKLES);// shackle him in place to ensure no exploit happens for jail break attempt
        if (Aura* dungdesert = pTarget->AddAura(LFG_SPELL_DUNGEON_DESERTER, pTarget))// LFG_SPELL_DUNGEON_DESERTER
        {
            dungdesert->SetDuration(-1);
        }
        if (Aura* bgdesert = pTarget->AddAura(BG_SPELL_DESERTER, pTarget))// BG_SPELL_DESERTER
        {
            bgdesert->SetDuration(-1);
        }
        if (Aura* silent = pTarget->AddAura(SILENCED, pTarget))// SILENCED
        {
            silent->SetDuration(-1);
        }

        return true;
    }

    static bool HandleAnticheatParoleCommand(ChatHandler* handler, char const* args)
    {
        if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_ENABLE))
            return false;

        if (!*args)
            return false;

        Player* target;
        if (!handler->extractPlayerTarget((char*)args, &target))
            return false;

        Player* pTarget = target->GetSession();

        WorldLocation Aloc = WorldLocation(0, -8833.37f, 628.62f, 94.00f, 1.06f);// Stormwind
        WorldLocation Hloc = WorldLocation(1, 1569.59f, -4397.63f, 16.06f, 0.54f);// Orgrimmar

        if (pTarget->GetTeamId() == TEAM_ALLIANCE)
        {
            pTarget->TeleportTo(0, -8833.37f, 628.62f, 94.00f, 1.06f);//Stormwind
            pTarget->SetHomebind(Aloc, 1519);// Stormwind Homebind location
        }
        else
        {
            pTarget->TeleportTo(1, 1569.59f, -4397.63f, 7.7f, 0.54f);//Orgrimmar
            pTarget->SetHomebind(Hloc, 1653);// Orgrimmar Homebind location
        }
        pTarget->RemoveAura(SHACKLES);// remove shackles
        pTarget->RemoveAura(LFG_SPELL_DUNGEON_DESERTER);// LFG_SPELL_DUNGEON_DESERTER
        pTarget->RemoveAura(BG_SPELL_DESERTER);// BG_SPELL_DESERTER
        pTarget->RemoveAura(SILENCED);// SILENCED
        sAnticheatMgr->AnticheatDeleteCommand(pTarget->GetGUID());// deletes auto reports on player
        return true;
    }

    static bool HandleAntiCheatDeleteCommand(ChatHandler* handler, char const* args)
    {
        if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_ENABLE))
            return false;

        if (!*args)
            return false;

        Player* target;
        if (!handler->extractPlayerTarget((char*)args, &target))
            return false;

        sAnticheatMgr->AnticheatDeleteCommand(target->GetGUID());
        handler->PSendSysMessage("Anticheat players_reports_status deleted for player %s", target->GetName().c_str());
        return true;
    }
    static bool HandleAntiCheatPurgeCommand(ChatHandler* handler)
    {
        // For the sins I am about to commit, may CTHULHU forgive me
        // this will purge the daily_player_reports which is the cumlative statistics of auto reports
        sAnticheatMgr->AnticheatPurgeCommand(handler);
        handler->PSendSysMessage("The Anticheat daily_player_reports has been purged.");
        return true;
    }

    static bool HandleAntiCheatPlayerCommand(ChatHandler* handler, char const* args)
    {
        if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_ENABLE))
            return false;

        if (!*args)
            return false;

        Player* target;
        if (!handler->extractPlayerTarget((char*)args, &target))
            return false;

        uint32 guid = player->GetGUID().GetCounter();

        float average = sAnticheatMgr->GetAverage(guid);
        uint32 total_reports = sAnticheatMgr->GetTotalReports(guid);
        uint32 speed_reports = sAnticheatMgr->GetTypeReports(guid,0);
        uint32 fly_reports = sAnticheatMgr->GetTypeReports(guid,1);
        uint32 jump_reports = sAnticheatMgr->GetTypeReports(guid,3);
        uint32 waterwalk_reports = sAnticheatMgr->GetTypeReports(guid,2);
        uint32 teleportplane_reports = sAnticheatMgr->GetTypeReports(guid,4);
        uint32 climb_reports = sAnticheatMgr->GetTypeReports(guid,5);
        uint32 teleport_reports = sAnticheatMgr->GetTypeReports(guid, 6);
        uint32 ignorecontrol_reports = sAnticheatMgr->GetTypeReports(guid, 7);
        uint32 zaxis_reports = sAnticheatMgr->GetTypeReports(guid, 8);
        uint32 antiswim_reports = sAnticheatMgr->GetTypeReports(guid, 9);
        uint32 gravity_reports = sAnticheatMgr->GetTypeReports(guid, 10);

        uint32 latency = 0;
        latency = player->GetConnectedPlayer()->GetSession()->GetLatency();

        handler->PSendSysMessage("Information about player %s || Latency %u ms", player->GetName().c_str(), latency);
        handler->PSendSysMessage("Average: %f || Total Reports: %u ", average, total_reports);
        handler->PSendSysMessage("Speed Reports: %u || Fly Reports: %u || Jump Reports: %u ", speed_reports, fly_reports, jump_reports);
        handler->PSendSysMessage("Walk On Water Reports: %u  || Teleport To Plane Reports: %u", waterwalk_reports, teleportplane_reports);
        handler->PSendSysMessage("Teleport Reports: %u || Climb Reports: %u", teleport_reports, climb_reports);
        handler->PSendSysMessage("Ignore Control Reports: %u || Ignore Z-Axis Reports: %u", ignorecontrol_reports, zaxis_reports);
        handler->PSendSysMessage("Ignore Anti-Swim Reports: %u || Gravity Reports: %u", antiswim_reports, gravity_reports);
        return true;
    }

    static bool HandleAntiCheatHandleCommand(ChatHandler* handler, bool enable)
    {
        if (enable)
        {
            sWorld->SetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_ENABLE,true);
            handler->SendSysMessage("The Anticheat System is now: Enabled!");
        }
        else
        {
            sWorld->SetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_ENABLE,false);
            handler->SendSysMessage("The Anticheat System is now: Disabled!");
        }

        return true;
    }

    static bool HandleAntiCheatGlobalCommand(ChatHandler* handler)
    {
        if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_ENABLE))
        {
            handler->PSendSysMessage("The Anticheat System is disabled.");
            return true;
        }

        sAnticheatMgr->AnticheatGlobalCommand(handler);

        return true;
    }
};

void AddSC_anticheat_commandscript()
{
    new anticheat_commandscript();
}
