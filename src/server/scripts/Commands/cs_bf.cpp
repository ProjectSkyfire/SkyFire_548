/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/* ScriptData
Name: bf_commandscript
%Complete: 100
Comment: All bf related commands
Category: commandscripts
EndScriptData */

#include "BattlefieldMgr.h"
#include "Chat.h"
#include "ScriptMgr.h"

class bf_commandscript : public CommandScript
{
public:
    bf_commandscript() : CommandScript("bf_commandscript") { }

    std::vector<ChatCommand> GetCommands() const OVERRIDE
    {
        static std::vector<ChatCommand> battlefieldcommandTable =
        {
            { "start",  rbac::RBAC_PERM_COMMAND_BF_START,  false, &HandleBattlefieldStart,  "", },
            { "stop",   rbac::RBAC_PERM_COMMAND_BF_STOP,   false, &HandleBattlefieldEnd,    "", },
            { "switch", rbac::RBAC_PERM_COMMAND_BF_SWITCH, false, &HandleBattlefieldSwitch, "", },
            { "timer",  rbac::RBAC_PERM_COMMAND_BF_TIMER,  false, &HandleBattlefieldTimer,  "", },
            { "enable", rbac::RBAC_PERM_COMMAND_BF_ENABLE, false, &HandleBattlefieldEnable, "", },
        };
        static std::vector<ChatCommand> commandTable =
        {
            { "bf", rbac::RBAC_PERM_COMMAND_BF, false, NULL, "", battlefieldcommandTable },
        };
        return commandTable;
    }

    static bool HandleBattlefieldStart(ChatHandler* handler, const char* args)
    {
        uint32 battleid = 0;
        char* battleid_str = strtok((char*)args, " ");
        if (!battleid_str)
            return false;

        battleid = atoi(battleid_str);

        Battlefield* bf = sBattlefieldMgr->GetBattlefieldByBattleId(battleid);

        if (!bf)
            return false;

        bf->StartBattle();

        if (battleid == 1)
            handler->SendGlobalGMSysMessage("Wintergrasp (Command start used)");

        return true;
    }

    static bool HandleBattlefieldEnd(ChatHandler* handler, const char* args)
    {
        uint32 battleid = 0;
        char* battleid_str = strtok((char*)args, " ");
        if (!battleid_str)
            return false;

        battleid = atoi(battleid_str);

        Battlefield* bf = sBattlefieldMgr->GetBattlefieldByBattleId(battleid);

        if (!bf)
            return false;

        bf->EndBattle(true);

        if (battleid == 1)
            handler->SendGlobalGMSysMessage("Wintergrasp (Command stop used)");

        return true;
    }

    static bool HandleBattlefieldEnable(ChatHandler* handler, const char* args)
    {
        uint32 battleid = 0;
        char* battleid_str = strtok((char*)args, " ");
        if (!battleid_str)
            return false;

        battleid = atoi(battleid_str);

        Battlefield* bf = sBattlefieldMgr->GetBattlefieldByBattleId(battleid);

        if (!bf)
            return false;

        if (bf->IsEnabled())
        {
            bf->ToggleBattlefield(false);
            if (battleid == 1)
                handler->SendGlobalGMSysMessage("Wintergrasp is disabled");
        }
        else
        {
            bf->ToggleBattlefield(true);
            if (battleid == 1)
                handler->SendGlobalGMSysMessage("Wintergrasp is enabled");
        }

        return true;
    }

    static bool HandleBattlefieldSwitch(ChatHandler* handler, const char* args)
    {
        uint32 battleid = 0;
        char* battleid_str = strtok((char*)args, " ");
        if (!battleid_str)
            return false;

        battleid = atoi(battleid_str);

        Battlefield* bf = sBattlefieldMgr->GetBattlefieldByBattleId(battleid);

        if (!bf)
            return false;

        bf->EndBattle(false);
        if (battleid == 1)
            handler->SendGlobalGMSysMessage("Wintergrasp (Command switch used)");

        return true;
    }

    static bool HandleBattlefieldTimer(ChatHandler* handler, const char* args)
    {
        uint32 battleid = 0;
        uint32 time = 0;
        char* battleid_str = strtok((char*)args, " ");
        if (!battleid_str)
            return false;
        char* time_str = strtok(NULL, " ");
        if (!time_str)
            return false;

        battleid = atoi(battleid_str);

        time = atoi(time_str);

        Battlefield* bf = sBattlefieldMgr->GetBattlefieldByBattleId(battleid);

        if (!bf)
            return false;

        bf->SetTimer(time * IN_MILLISECONDS);
        bf->SendInitWorldStatesToAll();
        if (battleid == 1)
            handler->SendGlobalGMSysMessage("Wintergrasp (Command timer used)");

        return true;
    }
};

void AddSC_bf_commandscript()
{
    new bf_commandscript();
}
