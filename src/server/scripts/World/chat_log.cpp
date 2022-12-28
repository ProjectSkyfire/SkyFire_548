/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "Channel.h"
#include "Guild.h"
#include "Group.h"

class ChatLogScript : public PlayerScript
{
public:
    ChatLogScript() : PlayerScript("ChatLogScript") { }

    void OnChat(Player* player, ChatMsg type, Language lang, std::string& msg)
    {
        switch (type)
        {
            case ChatMsg::CHAT_MSG_ADDON:
                if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_ADDON))
                    SF_LOG_DEBUG("chat.log", "[ADDON] Player %s sends: %s",
                        player->GetName().c_str(), msg.c_str());
                break;

            case ChatMsg::CHAT_MSG_SAY:
                if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_PUBLIC))
                    SF_LOG_DEBUG("chat.log", "[SAY] Player %s says (language %u): %s",
                        player->GetName().c_str(), uint8(lang), msg.c_str());
                break;

            case ChatMsg::CHAT_MSG_EMOTE:
                if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_PUBLIC))
                    SF_LOG_DEBUG("chat.log", "[TEXTEMOTE] Player %s emotes: %s",
                        player->GetName().c_str(), msg.c_str());
                break;

            case ChatMsg::CHAT_MSG_YELL:
                if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_PUBLIC))
                    SF_LOG_DEBUG("chat.log", "[YELL] Player %s yells (language %u): %s",
                        player->GetName().c_str(), uint8(lang), msg.c_str());
                break;
            default:
                break;
        }
    }

    void OnChat(Player* player, ChatMsg /*type*/, Language lang, std::string& msg, Player* receiver)
    {
        if (lang != Language::LANG_ADDON && sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_WHISPER))
            SF_LOG_DEBUG("chat.log", "[WHISPER] Player %s tells %s: %s",
                player->GetName().c_str(), receiver ? receiver->GetName().c_str() : "<unknown>", msg.c_str());
        else if (lang == Language::LANG_ADDON && sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_ADDON))
            SF_LOG_DEBUG("chat.log", "[ADDON] Player %s tells %s: %s",
                player->GetName().c_str(), receiver ? receiver->GetName().c_str() : "<unknown>", msg.c_str());
    }

    void OnChat(Player* player, ChatMsg type, Language lang, std::string& msg, Group* group)
    {
        //! NOTE:
        //! LANG_ADDON can only be sent by client in "PARTY", "RAID", "GUILD", "BATTLEGROUND", "WHISPER"
        switch (type)
        {
            case ChatMsg::CHAT_MSG_PARTY:
                if (lang != Language::LANG_ADDON && sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_PARTY))
                    SF_LOG_DEBUG("chat.log", "[PARTY] Player %s tells group with leader %s: %s",
                        player->GetName().c_str(), group ? group->GetLeaderName() : "<unknown>", msg.c_str());
                else if (lang == Language::LANG_ADDON && sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_ADDON))
                    SF_LOG_DEBUG("chat.log", "[ADDON] Player %s tells group with leader %s: %s",
                        player->GetName().c_str(), group ? group->GetLeaderName() : "<unknown>", msg.c_str());
                break;

            case ChatMsg::CHAT_MSG_PARTY_LEADER:
                if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_PARTY))
                    SF_LOG_DEBUG("chat.log", "[PARTY] Leader %s tells group: %s",
                        player->GetName().c_str(), msg.c_str());
                break;

            case ChatMsg::CHAT_MSG_RAID:
                if (lang != Language::LANG_ADDON && sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_RAID))
                    SF_LOG_DEBUG("chat.log", "[RAID] Player %s tells raid with leader %s: %s",
                        player->GetName().c_str(), group ? group->GetLeaderName() : "<unknown>", msg.c_str());
                else if (lang == Language::LANG_ADDON && sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_ADDON))
                    SF_LOG_DEBUG("chat.log", "[ADDON] Player %s tells raid with leader %s: %s",
                        player->GetName().c_str(), group ? group->GetLeaderName() : "<unknown>", msg.c_str());
                break;

            case ChatMsg::CHAT_MSG_RAID_LEADER:
                if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_RAID))
                    SF_LOG_DEBUG("chat.log", "[RAID] Leader player %s tells raid: %s",
                        player->GetName().c_str(), msg.c_str());
                break;

            case ChatMsg::CHAT_MSG_RAID_WARNING:
                if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_RAID))
                    SF_LOG_DEBUG("chat.log", "[RAID] Leader player %s warns raid with: %s",
                        player->GetName().c_str(), msg.c_str());
                break;

            case ChatMsg::CHAT_MSG_INSTANCE:
                if (lang != Language::LANG_ADDON && sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_BGROUND))
                    SF_LOG_DEBUG("chat.log", "[BATTLEGROUND] Player %s tells battleground with leader %s: %s",
                        player->GetName().c_str(), group ? group->GetLeaderName() : "<unknown>", msg.c_str());
                else if (lang == Language::LANG_ADDON && sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_ADDON))
                    SF_LOG_DEBUG("chat.log", "[ADDON] Player %s tells battleground with leader %s: %s",
                        player->GetName().c_str(), group ? group->GetLeaderName() : "<unknown>", msg.c_str());
                break;

            case ChatMsg::CHAT_MSG_INSTANCE_LEADER:
                if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_BGROUND))
                    SF_LOG_DEBUG("chat.log", "[BATTLEGROUND] Leader player %s tells battleground: %s",
                        player->GetName().c_str(), msg.c_str());
                break;
            default:
                break;
        }
    }

    void OnChat(Player* player, ChatMsg type, Language lang, std::string& msg, Guild* guild)
    {
        switch (type)
        {
            case ChatMsg::CHAT_MSG_GUILD:
                if (lang != Language::LANG_ADDON && sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_GUILD))
                    SF_LOG_DEBUG("chat.log", "[GUILD] Player %s tells guild %s: %s",
                        player->GetName().c_str(), guild ? guild->GetName().c_str() : "<unknown>", msg.c_str());
                else if (lang == Language::LANG_ADDON && sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_ADDON))
                    SF_LOG_DEBUG("chat.log", "[ADDON] Player %s sends to guild %s: %s",
                        player->GetName().c_str(), guild ? guild->GetName().c_str() : "<unknown>", msg.c_str());
                break;

            case ChatMsg::CHAT_MSG_OFFICER:
                if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_GUILD))
                    SF_LOG_DEBUG("chat.log", "[OFFICER] Player %s tells guild %s officers: %s",
                        player->GetName().c_str(), guild ? guild->GetName().c_str() : "<unknown>", msg.c_str());
                break;
            default:
                break;
        }
    }

    void OnChat(Player* player, ChatMsg /*type*/, Language /*lang*/, std::string& msg, Channel* channel)
    {
        bool isSystem = channel &&
                        (channel->HasFlag(CHANNEL_FLAG_TRADE) ||
                         channel->HasFlag(CHANNEL_FLAG_GENERAL) ||
                         channel->HasFlag(CHANNEL_FLAG_CITY) ||
                         channel->HasFlag(CHANNEL_FLAG_LFG));

        if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_SYSCHAN) && isSystem)
            SF_LOG_DEBUG("chat.log", "[SYSCHAN] Player %s tells channel %s: %s",
                player->GetName().c_str(), channel->GetName().c_str(), msg.c_str());
        else if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_CHANNEL))
            SF_LOG_DEBUG("chat.log", "[CHANNEL] Player %s tells channel %s: %s",
                player->GetName().c_str(), channel ? channel->GetName().c_str() : "<unknown>", msg.c_str());
    }
};

void AddSC_chat_log()
{
    new ChatLogScript();
}
