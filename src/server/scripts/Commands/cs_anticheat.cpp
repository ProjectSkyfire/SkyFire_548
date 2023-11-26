/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Chat.h"
#include "Language.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuras.h"
#include "AnticheatMgr.h"

enum Spells
{
    FREEZE = 9454,
    LFG_SPELL_DUNGEON_DESERTER = 71041,
    BG_SPELL_DESERTER = 26013,
    SILENCED = 23207
};

class anticheat_commandscript : public CommandScript
{
public:
    anticheat_commandscript() : CommandScript("anticheat_commandscript") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> anticheatCommandTable =
        {
            { "global",          rbac::RBAC_PERM_COMMAND_GM, true,  &HandleAntiCheatGlobalCommand,  "",      },
            { "player",          rbac::RBAC_PERM_COMMAND_GM, true,  &HandleAntiCheatPlayerCommand,  "",      },
            { "delete",          rbac::RBAC_PERM_COMMAND_GM, true,  &HandleAntiCheatDeleteCommand,  "",   },
            { "purge",           rbac::RBAC_PERM_COMMAND_GM, true,  &HandleAntiCheatPurgeCommand,  "",    },
            { "handle",          rbac::RBAC_PERM_COMMAND_GM, true,  &HandleAntiCheatHandleCommand,  "",   },
            { "jail",            rbac::RBAC_PERM_COMMAND_GM, true,  &HandleAnticheatJailCommand,  "",        },
            { "parole",          rbac::RBAC_PERM_COMMAND_GM, true,  &HandleAnticheatParoleCommand,  "",      },
        };

        static std::vector<ChatCommand> commandTable =
        {
            { "anticheat",       rbac::RBAC_PERM_COMMAND_GM,  true, NULL,              "",      anticheatCommandTable      },
        };
        return commandTable;
    }
    static bool HandleAntiCheatGlobalCommand(ChatHandler* handler, const char* args)
    {
        if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_ENABLE))
        {
            handler->PSendSysMessage("The Anticheat System is disabled.");
            return true;
        }

        sAnticheatMgr->AnticheatGlobalCommand(handler);

        return true;
    }
    static bool HandleAntiCheatPlayerCommand(ChatHandler* handler, char const* args)
    {
        if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_ENABLE))
            return false;

        Player* target = NULL;
        std::string playerName;
        if (!handler->extractPlayerTarget((char*)args, &target, NULL, &playerName))
            return false;

        uint32 guid = target->GetGUIDLow();

        float average = sAnticheatMgr->GetAverage(guid);
        uint32 total_reports = sAnticheatMgr->GetTotalReports(guid);
        uint32 speed_reports = sAnticheatMgr->GetTypeReports(guid, 0);
        uint32 fly_reports = sAnticheatMgr->GetTypeReports(guid, 1);
        uint32 waterwalk_reports = sAnticheatMgr->GetTypeReports(guid, 2);
        uint32 jump_reports = sAnticheatMgr->GetTypeReports(guid, 3);
        uint32 teleportplane_reports = sAnticheatMgr->GetTypeReports(guid, 4);
        uint32 climb_reports = sAnticheatMgr->GetTypeReports(guid, 5);
        uint32 time_reports = sAnticheatMgr->GetTypeReports(guid, 6);
        uint32 gravity_reports = sAnticheatMgr->GetTypeReports(guid, 7);
        uint32 teleport_reports = sAnticheatMgr->GetTypeReports(guid, 8);
        uint32 bgstart_reports = sAnticheatMgr->GetTypeReports(guid, 9);

        uint32 latency = 0;
        latency = target->GetSession()->GetLatency();

        // account ban info
        QueryResult resultADB = LoginDatabase.PQuery("SELECT FROM_UNIXTIME(bandate), unbandate-bandate, active, unbandate, banreason, bannedby FROM account_banned WHERE id = '%u' ORDER BY bandate ASC", target->GetSession()->GetAccountId());
        // character ban info
        QueryResult resultCDB = CharacterDatabase.PQuery("SELECT FROM_UNIXTIME(bandate), unbandate-bandate, active, unbandate, banreason, bannedby FROM character_banned WHERE guid = '%u' ORDER BY bandate ASC", target->GetGUID());
        //                                                           0      1      2     3
        QueryResult resultLDB = CharacterDatabase.PQuery("SELECT accountId, type, time, data FROM account_data WHERE `data` LIKE '%%CastSpellByName%%' AND accountId = '%u'", target->GetSession()->GetAccountId());

        handler->PSendSysMessage("-----------------------------------------------------------------");
        handler->PSendSysMessage("Information about player %s", target->GetName().c_str());
        handler->PSendSysMessage("IP Address: %s || Latency %u ms", target->GetSession()->GetRemoteAddress().c_str(), latency);
        if (resultADB)
        {
            do
            {
                Field* fields = resultADB->Fetch();
                std::string startbanEnd = TimeToTimestampStr(fields[3].GetUInt64());
                std::string bannedReason = fields[4].GetString();
                std::string bannedBy = fields[5].GetString();
                handler->PSendSysMessage("Account Previously Banned: Yes");
                handler->PSendSysMessage("Ban Ended: %s", startbanEnd.c_str());
                handler->PSendSysMessage("Ban by: %s || Ban Reason: %s", bannedBy.c_str(), bannedReason.c_str());
            } while (resultADB->NextRow());
        }
        if (!resultADB)
        {
            handler->PSendSysMessage("Account Previously Banned: No");
        }
        if (resultCDB)
        {
            do
            {
                Field* fields = resultCDB->Fetch();
                std::string startbanEnd = TimeToTimestampStr(fields[3].GetUInt64());
                std::string bannedReason = fields[4].GetString();
                std::string bannedBy = fields[5].GetString();
                handler->PSendSysMessage("Character Previously Banned: Yes");
                handler->PSendSysMessage("Ban Ended: %s", startbanEnd.c_str());
                handler->PSendSysMessage("Ban by: %s || Ban Reason: %s", bannedBy.c_str(), bannedReason.c_str());
            } while (resultCDB->NextRow());
        }
        if (!resultCDB)
        {
            handler->PSendSysMessage("Character Previously Banned: No");
        }
        if (resultLDB)
        {
            do
            {
                handler->PSendSysMessage("Macro Requiring Lua unlock Detected: Yes");
            } while (resultLDB->NextRow());
        }
        if (!resultLDB)
        {
            handler->PSendSysMessage("Macro Requiring Lua unlock Detected: No");
        }
        handler->PSendSysMessage("Average: %f || Total Reports: %u ", average, total_reports);
        handler->PSendSysMessage("Speed Reports: %u || Fly Reports: %u || Jump Reports: %u ", speed_reports, fly_reports, jump_reports);
        handler->PSendSysMessage("Walk On Water Reports: %u  || Teleport To Plane Reports: %u", waterwalk_reports, teleportplane_reports);
        handler->PSendSysMessage("Teleport Reports: %u || Climb Reports: %u", teleport_reports, climb_reports);
        handler->PSendSysMessage("Time Manipuilation Reports: %u || Gravity Reports: %u", time_reports, gravity_reports);
        handler->PSendSysMessage("Battle Ground Start Reports: %u", bgstart_reports);
        return true;
    }
    static bool HandleAntiCheatDeleteCommand(ChatHandler* handler, char const* args)
    {
        if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_ENABLE))
            return false;

        Player* target = NULL;
        std::string playerName;
        if (!handler->extractPlayerTarget((char*)args, &target, NULL, &playerName))
            return false;

        sAnticheatMgr->AnticheatDeleteCommand(target->GetGUID());
        handler->PSendSysMessage("Anticheat players_reports_status deleted for player %s", target->GetName().c_str());
        return true;
    }
    static bool HandleAntiCheatPurgeCommand(ChatHandler* handler, const char* args)
    {
        // For the sins I am about to commit, may CTHULHU forgive me
        // this will purge the daily_player_reports which is the cumlative statistics of auto reports
        sAnticheatMgr->AnticheatPurgeCommand(handler);
        handler->PSendSysMessage("The Anticheat daily_player_reports has been purged.");
        return true;
    }
    static bool HandleAntiCheatHandleCommand(ChatHandler* handler, const char* args)
    {
        if (!handler->GetSession() && !handler->GetSession()->GetPlayer())
            return false;

        std::string argstr = (char*)args;

        if (argstr == "on")
        {
            sWorld->SetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_ENABLE, true);
            handler->SendSysMessage("The Anticheat System is now: Enabled!");
            return true;
        }
        if (argstr == "off")
        {
            sWorld->SetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_ENABLE, false);
            handler->SendSysMessage("The Anticheat System is now: Disabled!");
            return true;
        }

        std::string strACModIs = sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_ENABLE) ? "enabled" : "disabled";
        handler->PSendSysMessage("Anticheat Manager is now %s.", strACModIs.c_str());

        return true;
    }
    static bool HandleAnticheatJailCommand(ChatHandler* handler, char const* args)
    {
        if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_ENABLE))
            return false;

        Player* target = NULL;
        std::string playerName;
        if (!handler->extractPlayerTarget((char*)args, &target, NULL, &playerName))
            return false;

        WorldLocation jailLoc = WorldLocation(1, 16226.5f, 16403.6f, -64.5f, 3.2f);// GM Jail Location
        target->TeleportTo(jailLoc);
        target->SetHomebind(jailLoc, 876);// GM Jail Homebind location
        target->CastSpell(target, FREEZE);// Freeze him in place to ensure no exploit happens for jail break attempt

        if (Aura* dungdesert = target->AddAura(LFG_SPELL_DUNGEON_DESERTER, target))// LFG_SPELL_DUNGEON_DESERTER
        {
            dungdesert->SetDuration(-1);
        }
        if (Aura* bgdesert = target->AddAura(BG_SPELL_DESERTER, target))// BG_SPELL_DESERTER
        {
            bgdesert->SetDuration(-1);
        }
        if (Aura* silent = target->AddAura(SILENCED, target))// SILENCED
        {
            silent->SetDuration(-1);
        }

        return true;
    }
    static bool HandleAnticheatParoleCommand(ChatHandler* handler, char const* args)
    {
        if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ANTICHEAT_ENABLE))
            return false;

        Player* target = NULL;
        std::string playerName;
        if (!handler->extractPlayerTarget((char*)args, &target, NULL, &playerName))
            return false;

        WorldLocation Aloc = WorldLocation(0, -8833.37f, 628.62f, 94.00f, 1.06f);// Stormwind
        WorldLocation Hloc = WorldLocation(1, 1569.59f, -4397.63f, 16.06f, 0.54f);// Orgrimmar
        WorldLocation Nloc = WorldLocation(860, 1470.96f, 3466.06f, 181.63f, 2.78f);//Shang Xi Training Grounds

        if (target->getRace() == RACE_PANDAREN_NEUTRAL)
        {
            target->TeleportTo(860, 1470.96f, 3466.06f, 181.63f, 2.78f);//Shang Xi Training Grounds
            target->SetHomebind(Nloc, 5834);// Shang Xi Training Grounds Homebind location
        }
        if (target->GetTeamId() == TEAM_ALLIANCE && target->getRace() != RACE_PANDAREN_NEUTRAL)
        {
            target->TeleportTo(0, -8833.37f, 628.62f, 94.00f, 1.06f);//Stormwind
            target->SetHomebind(Aloc, 1519);// Stormwind Homebind location
        }
        if (target->GetTeamId() == TEAM_HORDE && target->getRace() != RACE_PANDAREN_NEUTRAL)
        {
            target->TeleportTo(1, 1569.59f, -4397.63f, 16.06f, 0.54f);//Orgrimmar
            target->SetHomebind(Hloc, 1653);// Orgrimmar Homebind location
        }
        target->RemoveAura(FREEZE);// remove shackles
        target->RemoveAura(LFG_SPELL_DUNGEON_DESERTER);// LFG_SPELL_DUNGEON_DESERTER
        target->RemoveAura(BG_SPELL_DESERTER);// BG_SPELL_DESERTER
        target->RemoveAura(SILENCED);// SILENCED
        sAnticheatMgr->AnticheatDeleteCommand(target->GetGUID());// deletes auto reports on player
        return true;
    }
};

void AddSC_anticheat_commandscript()
{
    new anticheat_commandscript();
}
