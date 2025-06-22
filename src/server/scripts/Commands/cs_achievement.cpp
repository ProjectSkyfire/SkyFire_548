/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/* ScriptData
Name: achievement_commandscript
%Complete: 100
Comment: All achievement related commands
Category: commandscripts
EndScriptData */

#include "AchievementMgr.h"
#include "Chat.h"
#include "Guild.h"
#include "Language.h"
#include "Player.h"
#include "ScriptMgr.h"

class achievement_commandscript : public CommandScript
{
public:
    achievement_commandscript() : CommandScript("achievement_commandscript") { }

    std::vector<ChatCommand> GetCommands() const OVERRIDE
    {
        static std::vector<ChatCommand> achievementCommandTable =
        {
            { "add", rbac::RBAC_PERM_COMMAND_ACHIEVEMENT_ADD, false, &HandleAchievementAddCommand, "", },
        };
        static std::vector<ChatCommand> commandTable =
        {
            { "achievement", rbac::RBAC_PERM_COMMAND_ACHIEVEMENT,  false, NULL, "", achievementCommandTable },
        };
        return commandTable;
    }

    static bool HandleAchievementAddCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        uint32 achievementId = atoi((char*)args);
        if (!achievementId)
        {
            if (char* id = handler->extractKeyFromLink((char*)args, "Hachievement"))
                achievementId = atoi(id);
            if (!achievementId)
                return false;
        }

        Player* target = handler->getSelectedPlayer();
        if (!target)
        {
            handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (AchievementEntry const* achievementEntry = sAchievementMgr->GetAchievement(achievementId))
        {
            if (achievementEntry->flags & ACHIEVEMENT_FLAG_GUILD)
            {
                if (Guild* guild = target->GetGuild())
                    guild->GetAchievementMgr().CompletedAchievement(achievementEntry, target);
            }
            else
                target->CompletedAchievement(achievementEntry);
        }

        return true;
    }
};

void AddSC_achievement_commandscript()
{
    new achievement_commandscript();
}
