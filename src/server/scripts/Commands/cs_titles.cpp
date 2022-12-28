/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
Name: titles_commandscript
%Complete: 100
Comment: All titles related commands
Category: commandscripts
EndScriptData */

#include "Chat.h"
#include "Language.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ScriptMgr.h"

class titles_commandscript : public CommandScript
{
public:
    titles_commandscript() : CommandScript("titles_commandscript") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> titlesSetCommandTable =
        {
            { "mask", rbac::RBAC_PERM_COMMAND_TITLES_SET_MASK, false, &HandleTitlesSetMaskCommand, "", },
        };
        static std::vector<ChatCommand> titlesCommandTable =
        {
            { "add",     rbac::RBAC_PERM_COMMAND_TITLES_ADD,     false, &HandleTitlesAddCommand,     "", },
            { "current", rbac::RBAC_PERM_COMMAND_TITLES_CURRENT, false, &HandleTitlesCurrentCommand, "", },
            { "remove",  rbac::RBAC_PERM_COMMAND_TITLES_REMOVE,  false, &HandleTitlesRemoveCommand,  "", },
            { "set",     rbac::RBAC_PERM_COMMAND_TITLES_SET,     false, NULL,       "", titlesSetCommandTable },
        };
        static std::vector<ChatCommand> commandTable =
        {
            { "titles", rbac::RBAC_PERM_COMMAND_TITLES, false, NULL, "", titlesCommandTable },
        };
        return commandTable;
    }

    static bool HandleTitlesCurrentCommand(ChatHandler* handler, char const* args)
    {
        // number or [name] Shift-click form |color|Htitle:title_id|h[name]|h|r
        char* id_p = handler->extractKeyFromLink((char*)args, "Htitle");
        if (!id_p)
            return false;

        int32 id = atoi(id_p);
        if (id <= 0)
        {
            handler->PSendSysMessage(LANG_INVALID_TITLE_ID, id);
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player* target = handler->getSelectedPlayer();
        if (!target)
        {
            handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // check online security
        if (handler->HasLowerSecurity(target, 0))
            return false;

        CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(id);
        if (!titleInfo)
        {
            handler->PSendSysMessage(LANG_INVALID_TITLE_ID, id);
            handler->SetSentErrorMessage(true);
            return false;
        }

        std::string tNameLink = handler->GetNameLink(target);

        target->SetTitle(titleInfo);                            // to be sure that title now known
        target->SetUInt32Value(PLAYER_FIELD_PLAYER_TITLE, titleInfo->bit_index);

        handler->PSendSysMessage(LANG_TITLE_CURRENT_RES, id, titleInfo->name, tNameLink.c_str());

        return true;
    }

    static bool HandleTitlesAddCommand(ChatHandler* handler, char const* args)
    {
        // number or [name] Shift-click form |color|Htitle:title_id|h[name]|h|r
        char* id_p = handler->extractKeyFromLink((char*)args, "Htitle");
        if (!id_p)
            return false;

        int32 id = atoi(id_p);
        if (id <= 0)
        {
            handler->PSendSysMessage(LANG_INVALID_TITLE_ID, id);
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player* target = handler->getSelectedPlayer();
        if (!target)
        {
            handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // check online security
        if (handler->HasLowerSecurity(target, 0))
            return false;

        CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(id);
        if (!titleInfo)
        {
            handler->PSendSysMessage(LANG_INVALID_TITLE_ID, id);
            handler->SetSentErrorMessage(true);
            return false;
        }

        std::string tNameLink = handler->GetNameLink(target);

        char titleNameStr[80];
        snprintf(titleNameStr, 80, titleInfo->name, target->GetName().c_str());

        target->SetTitle(titleInfo);
        handler->PSendSysMessage(LANG_TITLE_ADD_RES, id, titleNameStr, tNameLink.c_str());

        return true;
    }

    static bool HandleTitlesRemoveCommand(ChatHandler* handler, char const* args)
    {
        // number or [name] Shift-click form |color|Htitle:title_id|h[name]|h|r
        char* id_p = handler->extractKeyFromLink((char*)args, "Htitle");
        if (!id_p)
            return false;

        int32 id = atoi(id_p);
        if (id <= 0)
        {
            handler->PSendSysMessage(LANG_INVALID_TITLE_ID, id);
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player* target = handler->getSelectedPlayer();
        if (!target)
        {
            handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // check online security
        if (handler->HasLowerSecurity(target, 0))
            return false;

        CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(id);
        if (!titleInfo)
        {
            handler->PSendSysMessage(LANG_INVALID_TITLE_ID, id);
            handler->SetSentErrorMessage(true);
            return false;
        }

        target->SetTitle(titleInfo, true);

        std::string tNameLink = handler->GetNameLink(target);

        char titleNameStr[80];
        snprintf(titleNameStr, 80, titleInfo->name, target->GetName().c_str());

        handler->PSendSysMessage(LANG_TITLE_REMOVE_RES, id, titleNameStr, tNameLink.c_str());

        if (!target->HasTitle(target->GetInt32Value(PLAYER_FIELD_PLAYER_TITLE)))
        {
            target->SetUInt32Value(PLAYER_FIELD_PLAYER_TITLE, 0);
            handler->PSendSysMessage(LANG_CURRENT_TITLE_RESET, tNameLink.c_str());
        }

        return true;
    }

    //Edit Player KnownTitles
    static bool HandleTitlesSetMaskCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        uint64 titles = 0;

        sscanf((char*)args, UI64FMTD, &titles);

        Player* target = handler->getSelectedPlayer();
        if (!target)
        {
            handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // check online security
        if (handler->HasLowerSecurity(target, 0))
            return false;

        uint64 titles2 = titles;

        for (uint32 i = 1; i < sCharTitlesStore.GetNumRows(); ++i)
            if (CharTitlesEntry const* tEntry = sCharTitlesStore.LookupEntry(i))
                titles2 &= ~(uint64(1) << tEntry->bit_index);

        titles &= ~titles2;                                     // remove not existed titles

        target->SetUInt64Value(PLAYER_FIELD_KNOWN_TITLES, titles);
        handler->SendSysMessage(LANG_DONE);

        if (!target->HasTitle(target->GetInt32Value(PLAYER_FIELD_PLAYER_TITLE)))
        {
            target->SetUInt32Value(PLAYER_FIELD_PLAYER_TITLE, 0);
            handler->PSendSysMessage(LANG_CURRENT_TITLE_RESET, handler->GetNameLink(target).c_str());
        }

        return true;
    }
};

void AddSC_titles_commandscript()
{
    new titles_commandscript();
}
