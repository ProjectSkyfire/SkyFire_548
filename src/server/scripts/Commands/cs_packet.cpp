/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Chat.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "PacketLog.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "WorldSession.h"

#include <string>
#include <vector>

namespace
{
    std::string TrimCommandText(std::string text)
    {
        std::string::size_type first = text.find_first_not_of(" \t\r\n");
        if (first == std::string::npos)
            return "";

        std::string::size_type last = text.find_last_not_of(" \t\r\n");
        text = text.substr(first, last - first + 1);

        if (text.size() >= 2 && text[0] == '"' && text[text.size() - 1] == '"')
            text = text.substr(1, text.size() - 2);

        first = text.find_first_not_of(" \t\r\n");
        if (first == std::string::npos)
            return "";

        last = text.find_last_not_of(" \t\r\n");
        return text.substr(first, last - first + 1);
    }

    std::string GetPacketCommandName(char const* args)
    {
        return TrimCommandText(args ? args : "");
    }
}

class packet_commandscript : public CommandScript
{
public:
    packet_commandscript() : CommandScript("packet_commandscript") { }

    std::vector<ChatCommand> GetCommands() const OVERRIDE
    {
        static std::vector<ChatCommand> packetCharCommandTable =
        {
            { "start", rbac::RBAC_PERM_COMMAND_PACKET_CHAR_START, false, &HandlePacketCharStartCommand, "", },
            { "stop",  rbac::RBAC_PERM_COMMAND_PACKET_CHAR_STOP,  false, &HandlePacketCharStopCommand,  "", },
        };

        static std::vector<ChatCommand> packetCommandTable =
        {
            { "start", rbac::RBAC_PERM_COMMAND_PACKET_START, false, &HandlePacketStartCommand, "", },
            { "stop",  rbac::RBAC_PERM_COMMAND_PACKET_STOP,  false, &HandlePacketStopCommand,  "", },
            { "char",  rbac::RBAC_PERM_COMMAND_PACKET_CHAR,  false, NULL,                     "", packetCharCommandTable },
        };

        static std::vector<ChatCommand> commandTable =
        {
            { "packet", rbac::RBAC_PERM_COMMAND_PACKET, false, NULL, "", packetCommandTable },
        };

        return commandTable;
    }

    static bool HandlePacketStartCommand(ChatHandler* handler, char const* /*args*/)
    {
        sPacketLog->EnableGlobalLogging();
        handler->PSendSysMessage("Packet logging enabled for all sessions.");
        return true;
    }

    static bool HandlePacketStopCommand(ChatHandler* handler, char const* /*args*/)
    {
        sPacketLog->DisableAllLogging();
        handler->PSendSysMessage("Packet logging disabled.");
        return true;
    }

    static bool HandlePacketCharStartCommand(ChatHandler* handler, char const* args)
    {
        std::string characterName = GetPacketCommandName(args);
        if (characterName.empty() || !normalizePlayerName(characterName))
        {
            handler->PSendSysMessage("Usage: .packet char start \"name\"");
            handler->SetSentErrorMessage(true);
            return false;
        }

        sPacketLog->EnableCharacterLogging(characterName);

        if (Player* target = sObjectAccessor->FindPlayerByName(characterName))
            if (WorldSession* targetSession = target->GetSession())
                targetSession->LogPacketMarker("Packet logging enabled for this character.");

        handler->PSendSysMessage("Packet logging enabled for character %s.", characterName.c_str());
        return true;
    }

    static bool HandlePacketCharStopCommand(ChatHandler* handler, char const* args)
    {
        std::string characterName = GetPacketCommandName(args);
        if (characterName.empty() || !normalizePlayerName(characterName))
        {
            handler->PSendSysMessage("Usage: .packet char stop \"name\"");
            handler->SetSentErrorMessage(true);
            return false;
        }

        sPacketLog->DisableCharacterLogging(characterName);
        handler->PSendSysMessage("Packet logging disabled for character %s.", characterName.c_str());
        return true;
    }
};

void AddSC_packet_commandscript()
{
    new packet_commandscript();
}
