/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/* ScriptData
Name: message_commandscript
%Complete: 100
Comment: All message related commands
Category: commandscripts
EndScriptData */

#include "ChannelMgr.h"
#include "Chat.h"
#include "Language.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ScriptMgr.h"

class message_commandscript : public CommandScript
{
public:
    message_commandscript() : CommandScript("message_commandscript") { }

    std::vector<ChatCommand> GetCommands() const
    {
        static std::vector<ChatCommand> channelSetCommandTable =
        {
            { "ownership", rbac::RBAC_PERM_COMMAND_CHANNEL_SET_OWNERSHIP, false, &HandleChannelSetOwnership, "", },
        };
        static std::vector<ChatCommand> channelCommandTable =
        {
            { "set", rbac::RBAC_PERM_COMMAND_CHANNEL_SET, true, NULL, "", channelSetCommandTable },
        };
        static std::vector<ChatCommand> commandTable =
        {
            { "channel",        rbac::RBAC_PERM_COMMAND_CHANNEL,        true, NULL,                         "", channelCommandTable  },
            { "nameannounce",   rbac::RBAC_PERM_COMMAND_NAMEANNOUNCE,   true, &HandleNameAnnounceCommand,   "", },
            { "gmnameannounce", rbac::RBAC_PERM_COMMAND_GMNAMEANNOUNCE, true, &HandleGMNameAnnounceCommand, "", },
            { "announce",       rbac::RBAC_PERM_COMMAND_ANNOUNCE,       true, &HandleAnnounceCommand,       "", },
            { "gmannounce",     rbac::RBAC_PERM_COMMAND_GMANNOUNCE,     true, &HandleGMAnnounceCommand,     "", },
            { "notify",         rbac::RBAC_PERM_COMMAND_NOTIFY,         true, &HandleNotifyCommand,         "", },
            { "gmnotify",       rbac::RBAC_PERM_COMMAND_GMNOTIFY,       true, &HandleGMNotifyCommand,       "", },
            { "whispers",       rbac::RBAC_PERM_COMMAND_WHISPERS,      false, &HandleWhispersCommand,       "", },
        };
        return commandTable;
    }

    static bool HandleChannelSetOwnership(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;
        char const* channelStr = strtok((char*)args, " ");
        char const* argStr = strtok(NULL, "");

        if (!channelStr || !argStr)
            return false;

        Player* player = handler->GetSession()->GetPlayer();
        Channel* channcel = NULL;

        if (ChannelMgr* cMgr = ChannelMgr::forTeam(player->GetTeam()))
            channcel = cMgr->GetChannel(channelStr, player);

        if (strcmp(argStr, "on") == 0)
        {
            if (channcel)
                channcel->SetOwnership(true);
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHANNEL_OWNERSHIP);
            stmt->setUInt8(0, 1);
            stmt->setString(1, channelStr);
            CharacterDatabase.Execute(stmt);
            handler->PSendSysMessage(LANG_CHANNEL_ENABLE_OWNERSHIP, channelStr);
        }
        else if (strcmp(argStr, "off") == 0)
        {
            if (channcel)
                channcel->SetOwnership(false);
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHANNEL_OWNERSHIP);
            stmt->setUInt8(0, 0);
            stmt->setString(1, channelStr);
            CharacterDatabase.Execute(stmt);
            handler->PSendSysMessage(LANG_CHANNEL_DISABLE_OWNERSHIP, channelStr);
        }
        else
            return false;

        return true;
    }

    static bool HandleNameAnnounceCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        std::string name("Console");
        if (WorldSession* session = handler->GetSession())
            name = session->GetPlayer()->GetName();

        sWorld->SendWorldText(LANG_ANNOUNCE_COLOR, name.c_str(), args);
        return true;
    }

    static bool HandleGMNameAnnounceCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        std::string name("Console");
        if (WorldSession* session = handler->GetSession())
            name = session->GetPlayer()->GetName();

        sWorld->SendGMText(LANG_GM_ANNOUNCE_COLOR, name.c_str(), args);
        return true;
    }
    // global announce
    static bool HandleAnnounceCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        char buff[2048];
        snprintf(buff, sizeof(buff), handler->GetSkyFireString(LANG_SYSTEMMESSAGE), args);
        sWorld->SendServerMessage(SERVER_MSG_STRING, buff);
        return true;
    }
    // announce to logged in GMs
    static bool HandleGMAnnounceCommand(ChatHandler* /*handler*/, char const* args)
    {
        if (!*args)
            return false;

        sWorld->SendGMText(LANG_GM_BROADCAST, args);
        return true;
    }
    // notification player at the screen
    static bool HandleNotifyCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        std::string msg = handler->GetSkyFireString(LANG_GLOBAL_NOTIFY);
        msg += args;

        WorldPacket data(SMSG_NOTIFICATION, 2 + msg.length());
        data.WriteBits(msg.length(), 12);
        data.FlushBits();
        data.WriteString(msg);
        sWorld->SendGlobalMessage(&data);

        return true;
    }
    // notification GM at the screen
    static bool HandleGMNotifyCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        std::string str = handler->GetSkyFireString(LANG_GM_NOTIFY);
        str += args;

        WorldPacket data(SMSG_NOTIFICATION, (str.size() + 1));
        data << str;
        sWorld->SendGlobalGMMessage(&data);

        return true;
    }
    // Enable\Dissable accept whispers (for GM)
    static bool HandleWhispersCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
        {
            handler->PSendSysMessage(LANG_COMMAND_WHISPERACCEPTING, handler->GetSession()->GetPlayer()->isAcceptWhispers() ? handler->GetSkyFireString(LANG_ON) : handler->GetSkyFireString(LANG_OFF));
            return true;
        }

        std::string argStr = strtok((char*)args, " ");
        // whisper on
        if (argStr == "on")
        {
            handler->GetSession()->GetPlayer()->SetAcceptWhispers(true);
            handler->SendSysMessage(LANG_COMMAND_WHISPERON);
            return true;
        }

        // whisper off
        if (argStr == "off")
        {
            // Remove all players from the Gamemaster's whisper whitelist
            handler->GetSession()->GetPlayer()->ClearWhisperWhiteList();
            handler->GetSession()->GetPlayer()->SetAcceptWhispers(false);
            handler->SendSysMessage(LANG_COMMAND_WHISPEROFF);
            return true;
        }

        if (argStr == "remove")
        {
            std::string name = strtok(NULL, " ");
            if (normalizePlayerName(name))
            {
                if (Player* player = sObjectAccessor->FindPlayerByName(name))
                {
                    handler->GetSession()->GetPlayer()->RemoveFromWhisperWhiteList(player->GetGUID());
                    handler->PSendSysMessage(LANG_COMMAND_WHISPEROFFPLAYER, name.c_str());
                    return true;
                }
                else
                {
                    handler->PSendSysMessage(LANG_PLAYER_NOT_FOUND, name.c_str());
                    handler->SetSentErrorMessage(true);
                    return false;
                }
            }
        }
        handler->SendSysMessage(LANG_USE_BOL);
        handler->SetSentErrorMessage(true);
        return false;
    }
};

void AddSC_message_commandscript()
{
    new message_commandscript();
}
