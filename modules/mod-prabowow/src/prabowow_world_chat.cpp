/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*
* mod-prabowow: realm-wide chat.
*
*   .chat <message>   broadcast to everyone online, both factions
*
* Delivered as a system message, so it reaches every map and ignores the
* faction language barrier. Mutes, a minimum level and a per-player cooldown
* keep it from becoming a spam channel.
*/

#include "PraboWoWConfig.h"

#include "Chat.h"
#include "Log.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SharedDefines.h"
#include "World.h"
#include "WorldSession.h"

#include <ctime>
#include <mutex>
#include <string>
#include <unordered_map>

namespace
{
    std::mutex g_lock;
    std::unordered_map<uint32, time_t> g_lastMessage; // character low guid -> last .chat time

    char const* ClassColor(uint8 playerClass)
    {
        switch (playerClass)
        {
            case CLASS_WARRIOR:      return "C79C6E";
            case CLASS_PALADIN:      return "F58CBA";
            case CLASS_HUNTER:       return "ABD473";
            case CLASS_ROGUE:        return "FFF569";
            case CLASS_PRIEST:       return "FFFFFF";
            case CLASS_DEATH_KNIGHT: return "C41F3B";
            case CLASS_SHAMAN:       return "0070DE";
            case CLASS_MAGE:         return "69CCF0";
            case CLASS_WARLOCK:      return "9482C9";
            case CLASS_MONK:         return "00FF96";
            case CLASS_DRUID:        return "FF7D0A";
            default:                 return "FFFFFF";
        }
    }

    // Players must not be able to inject fake item/quest links or colour codes.
    std::string Sanitize(char const* raw)
    {
        std::string text;
        for (char const* p = raw; *p; ++p)
        {
            if (*p == '|')
                text += "||";
            else if (*p == '\r' || *p == '\n')
                text += ' ';
            else
                text += *p;
        }

        size_t start = text.find_first_not_of(' ');
        size_t end = text.find_last_not_of(' ');
        if (start == std::string::npos)
            return std::string();
        text = text.substr(start, end - start + 1);

        if (text.size() > 200)
            text.resize(200);
        return text;
    }
}

class prabowow_chat_commandscript : public CommandScript
{
public:
    prabowow_chat_commandscript() : CommandScript("prabowow_chat_commandscript") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> commandTable =
        {
            { "chat", PraboWoW::RBAC_PERM_COMMAND_CHAT, false, &HandleChatCommand, "" },
        };
        return commandTable;
    }

    static bool HandleChatCommand(ChatHandler* handler, char const* args)
    {
        WorldSession* session = handler->GetSession();
        Player* player = session ? session->GetPlayer() : NULL;
        if (!player)
            return false;

        if (!PraboWoW::GetBool("PraboWoW.WorldChat.Enable", true))
        {
            handler->SendSysMessage("|cff00ff00[PraboWoW]|r World chat is disabled on this realm.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!*args)
        {
            handler->SendSysMessage("|cff00ff00[PraboWoW]|r Usage: .chat <message>");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (session->m_muteTime > time(NULL))
        {
            handler->SendSysMessage("|cff00ff00[PraboWoW]|r You are muted and cannot use world chat.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        uint32 minLevel = uint32(std::max<int32>(PraboWoW::GetInt("PraboWoW.WorldChat.MinLevel", 1), 1));
        if (player->getLevel() < minLevel)
        {
            handler->PSendSysMessage("|cff00ff00[PraboWoW]|r World chat unlocks at level %u.", minLevel);
            handler->SetSentErrorMessage(true);
            return false;
        }

        std::string text = Sanitize(args);
        if (text.empty())
        {
            handler->SendSysMessage("|cff00ff00[PraboWoW]|r Usage: .chat <message>");
            handler->SetSentErrorMessage(true);
            return false;
        }

        int32 cooldown = PraboWoW::GetInt("PraboWoW.WorldChat.Cooldown", 3);
        time_t now = time(NULL);
        if (cooldown > 0 && !player->IsGameMaster())
        {
            std::lock_guard<std::mutex> lock(g_lock);
            time_t& last = g_lastMessage[player->GetGUIDLow()];
            if (last + cooldown > now)
            {
                handler->PSendSysMessage("|cff00ff00[PraboWoW]|r Please wait %u second(s) before using world chat again.", uint32(last + cooldown - now));
                handler->SetSentErrorMessage(true);
                return false;
            }
            last = now;
        }

        std::string line = "|cffFF8000[World]|r ";
        if (player->IsGameMaster())
            line += "|cffFF0000<GM>|r ";
        line += "|cff";
        line += ClassColor(player->getClass());
        line += "[";
        line += player->GetName();
        line += "]|r: ";
        line += text;

        sWorld->SendGlobalText(line.c_str(), NULL);
        SF_LOG_INFO(PraboWoW::LOG, "[mod-prabowow] [World] %s: %s", player->GetName().c_str(), text.c_str());
        return true;
    }
};

void AddSC_prabowow_world_chat()
{
    new prabowow_chat_commandscript();
}
