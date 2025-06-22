/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "AccountMgr.h"
#include "CellImpl.h"
#include "ChannelMgr.h"
#include "Chat.h"
#include "Common.h"
#include "DatabaseEnv.h"
#include "GridNotifiersImpl.h"
#include "Group.h"
#include "Guild.h"
#include "GuildMgr.h"
#include "Language.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "Util.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"

void WorldSession::HandleMessagechatOpcode(WorldPacket& recvData)
{
    ChatMsg type;
    Language lang;

    switch (recvData.GetOpcode())
    {
        case CMSG_MESSAGECHAT_SAY:
            type = ChatMsg::CHAT_MSG_SAY;
            break;
        case CMSG_MESSAGECHAT_YELL:
            type = ChatMsg::CHAT_MSG_YELL;
            break;
        case CMSG_MESSAGECHAT_PARTY:
            type = ChatMsg::CHAT_MSG_PARTY;
            break;
        case CMSG_MESSAGECHAT_RAID:
            type = ChatMsg::CHAT_MSG_RAID;
            break;
        case CMSG_MESSAGECHAT_RAID_WARNING:
            type = ChatMsg::CHAT_MSG_RAID_WARNING;
            break;
        case CMSG_MESSAGECHAT_GUILD:
            type = ChatMsg::CHAT_MSG_GUILD;
            break;
        case CMSG_MESSAGECHAT_OFFICER:
            type = ChatMsg::CHAT_MSG_OFFICER;
            break;
        case CMSG_MESSAGECHAT_WHISPER:
            type = ChatMsg::CHAT_MSG_WHISPER;
            break;
        case CMSG_MESSAGECHAT_EMOTE:
            type = ChatMsg::CHAT_MSG_EMOTE;
            break;
        case CMSG_MESSAGECHAT_CHANNEL:
            type = ChatMsg::CHAT_MSG_CHANNEL;
            break;
        case CMSG_CHAT_MESSAGE_AFK:
            type = ChatMsg::CHAT_MSG_AFK;
            break;
        case CMSG_MESSAGECHAT_DND:
            type = ChatMsg::CHAT_MSG_DND;
            break;
            /*
                case CMSG_MESSAGECHAT_BATTLEGROUND:
                type = CHAT_MSG_BATTLEGROUND;
                break;
            */
        default:
            SF_LOG_ERROR("network", "HandleMessagechatOpcode : Unknown chat opcode (%u)", recvData.GetOpcode());
            recvData.hexlike();
            return;
    }

    if (type >= ChatMsg::MSG_NULL_ACTION)
    {
        SF_LOG_ERROR("network", "CHAT: Wrong message type received: %u", uint8(type));
        recvData.rfinish();
        return;
    }

    Player* sender = GetPlayer();

    //SF_LOG_DEBUG("misc", "CHAT: packet received. type %u, lang %u", type, lang);

    // no language sent with emote packet.
    if (type != ChatMsg::CHAT_MSG_EMOTE && type != ChatMsg::CHAT_MSG_AFK && type != ChatMsg::CHAT_MSG_DND)
    {
        uint32 plang = 0;
        recvData >> plang;
        lang = Language(plang);

        // prevent talking at unknown language (cheating)
        LanguageDesc const* langDesc = GetLanguageDescByID(lang);
        if (!langDesc)
        {
            SendNotification(LANG_UNKNOWN_LANGUAGE);
            recvData.rfinish();
            return;
        }
        if (langDesc->skill_id != 0 && !sender->HasSkill(langDesc->skill_id))
        {
            // also check SPELL_AURA_COMPREHEND_LANGUAGE (client offers option to speak in that language)
            Unit::AuraEffectList const& langAuras = sender->GetAuraEffectsByType(SPELL_AURA_COMPREHEND_LANGUAGE);
            bool foundAura = false;
            for (Unit::AuraEffectList::const_iterator i = langAuras.begin(); i != langAuras.end(); ++i)
            {
                if ((*i)->GetMiscValue() == int32(lang))
                {
                    foundAura = true;
                    break;
                }
            }
            if (!foundAura)
            {
                SendNotification(LANG_NOT_LEARNED_LANGUAGE);
                recvData.rfinish();
                return;
            }
        }

        if (lang == Language::LANG_ADDON)
        {
            // LANG_ADDON is only valid for the following message types
            switch (type)
            {
                case ChatMsg::CHAT_MSG_PARTY:
                case ChatMsg::CHAT_MSG_RAID:
                case ChatMsg::CHAT_MSG_GUILD:
                case ChatMsg::CHAT_MSG_INSTANCE:
                case ChatMsg::CHAT_MSG_WHISPER:
                    if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_ADDON))
                    {
                        std::string msg = "";
                        recvData >> msg;

                        if (msg.empty())
                            return;

                        sScriptMgr->OnPlayerChat(sender, ChatMsg::CHAT_MSG_ADDON, lang, msg);
                    }

                    // Disabled addon channel?
                    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ADDON_CHANNEL))
                        return;
                    break;
                default:
                    SF_LOG_ERROR("network", "Player %s (GUID: %u) sent a chatmessage with an invalid language/message type combination",
                        GetPlayer()->GetName().c_str(), GetPlayer()->GetGUIDLow());

                    recvData.rfinish();
                    return;
            }
        }
        // LANG_ADDON should not be changed nor be affected by flood control
        else
        {
            // send in universal language if player in .gm on mode (ignore spell effects)
            if (sender->IsGameMaster())
                lang = Language::LANG_UNIVERSAL;
            else
            {
                // send in universal language in two side iteration allowed mode
                if (HasPermission(rbac::RBAC_PERM_TWO_SIDE_INTERACTION_CHAT))
                    lang = Language::LANG_UNIVERSAL;
                else
                {
                    switch (type)
                    {
                        case ChatMsg::CHAT_MSG_PARTY:
                        case ChatMsg::CHAT_MSG_PARTY_LEADER:
                        case ChatMsg::CHAT_MSG_RAID:
                        case ChatMsg::CHAT_MSG_RAID_LEADER:
                        case ChatMsg::CHAT_MSG_RAID_WARNING:
                            // allow two side chat at group channel if two side group allowed
                            if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ALLOW_TWO_SIDE_INTERACTION_GROUP))
                                lang = Language::LANG_UNIVERSAL;
                            break;
                        case ChatMsg::CHAT_MSG_GUILD:
                        case ChatMsg::CHAT_MSG_OFFICER:
                            // allow two side chat at guild channel if two side guild allowed
                            if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ALLOW_TWO_SIDE_INTERACTION_GUILD))
                                lang = Language::LANG_UNIVERSAL;
                            break;
                        default:
                            break;
                    }
                }

                // but overwrite it by SPELL_AURA_MOD_LANGUAGE auras (only single case used)
                Unit::AuraEffectList const& ModLangAuras = sender->GetAuraEffectsByType(SPELL_AURA_MOD_LANGUAGE);
                if (!ModLangAuras.empty())
                    lang = Language(ModLangAuras.front()->GetMiscValue());
            }

            if (!sender->CanSpeak())
            {
                std::string timeStr = secsToTimeString(m_muteTime - time(NULL));
                SendNotification(GetSkyFireString(LANG_WAIT_BEFORE_SPEAKING), timeStr.c_str());
                recvData.rfinish(); // Prevent warnings
                return;
            }
        }
    }
    else
        lang = Language::LANG_UNIVERSAL;

    if (sender->HasAura(1852) && type != ChatMsg::CHAT_MSG_WHISPER)
    {
        SendNotification(GetSkyFireString(LANG_GM_SILENCE), sender->GetName().c_str());
        recvData.rfinish();
        return;
    }

    uint32 textLength = 0;
    uint32 receiverLength = 0;
    std::string to, channel, msg;
    bool ignoreChecks = false;
    switch (type)
    {
        case ChatMsg::CHAT_MSG_SAY:
        case ChatMsg::CHAT_MSG_EMOTE:
        case ChatMsg::CHAT_MSG_YELL:
        case ChatMsg::CHAT_MSG_PARTY:
        case ChatMsg::CHAT_MSG_GUILD:
        case ChatMsg::CHAT_MSG_OFFICER:
        case ChatMsg::CHAT_MSG_RAID:
        case ChatMsg::CHAT_MSG_RAID_WARNING:
        case ChatMsg::CHAT_MSG_INSTANCE:
            textLength = recvData.ReadBits(8);
            msg = recvData.ReadString(textLength);
            break;
        case ChatMsg::CHAT_MSG_WHISPER:
            textLength = recvData.ReadBits(8);
            receiverLength = recvData.ReadBits(9);
            msg = recvData.ReadString(textLength);
            to = recvData.ReadString(receiverLength);
            break;
        case ChatMsg::CHAT_MSG_CHANNEL:
            textLength = recvData.ReadBits(9);
            receiverLength = recvData.ReadBits(8);
            msg = recvData.ReadString(receiverLength);
            channel = recvData.ReadString(textLength);
            break;
        case ChatMsg::CHAT_MSG_AFK:
        case ChatMsg::CHAT_MSG_DND:
            textLength = recvData.ReadBits(8);
            msg = recvData.ReadString(textLength);
            ignoreChecks = true;
            break;
        default:
            break;
    }

    if (!ignoreChecks)
    {
        if (msg.empty())
            return;

        if (ChatHandler(this).ParseCommands(msg.c_str()))
            return;

        if (lang != Language::LANG_ADDON)
        {
            // Strip invisible characters for non-addon messages
            if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_CHAT_FAKE_MESSAGE_PREVENTING))
                stripLineInvisibleChars(msg);

            if (sWorld->getIntConfig(WorldIntConfigs::CONFIG_CHAT_STRICT_LINK_CHECKING_SEVERITY) && !ChatHandler(this).isValidChatMessage(msg.c_str()))
            {
                SF_LOG_ERROR("network", "Player %s (GUID: %u) sent a chatmessage with an invalid link: %s", GetPlayer()->GetName().c_str(),
                    GetPlayer()->GetGUIDLow(), msg.c_str());

                if (sWorld->getIntConfig(WorldIntConfigs::CONFIG_CHAT_STRICT_LINK_CHECKING_KICK))
                    KickPlayer();

                return;
            }
        }
    }

    switch (type)
    {
        case ChatMsg::CHAT_MSG_SAY:
        case ChatMsg::CHAT_MSG_EMOTE:
        case ChatMsg::CHAT_MSG_YELL:
        {
            if (sender->getLevel() < sWorld->getIntConfig(WorldIntConfigs::CONFIG_CHAT_SAY_LEVEL_REQ))
            {
                SendNotification(GetSkyFireString(LANG_SAY_REQ), sWorld->getIntConfig(WorldIntConfigs::CONFIG_CHAT_SAY_LEVEL_REQ));
                return;
            }

            if (type == ChatMsg::CHAT_MSG_SAY)
                sender->Say(msg, lang);
            else if (type == ChatMsg::CHAT_MSG_EMOTE)
                sender->TextEmote(msg);
            else if (type == ChatMsg::CHAT_MSG_YELL)
                sender->Yell(msg, lang);
        } break;
        case ChatMsg::CHAT_MSG_WHISPER:
        {
            if (!normalizePlayerName(to))
            {
                SendPlayerNotFoundNotice(to);
                break;
            }

            Player* receiver = sObjectAccessor->FindPlayerByName(to);
            if (!receiver || (!receiver->isAcceptWhispers() && receiver->GetSession()->HasPermission(rbac::RBAC_PERM_CAN_FILTER_WHISPERS) && !receiver->IsInWhisperWhiteList(sender->GetGUID())))
            {
                SendPlayerNotFoundNotice(to);
                return;
            }
            if (!sender->IsGameMaster() && sender->getLevel() < sWorld->getIntConfig(WorldIntConfigs::CONFIG_CHAT_WHISPER_LEVEL_REQ) && !receiver->IsInWhisperWhiteList(sender->GetGUID()))
            {
                SendNotification(GetSkyFireString(LANG_WHISPER_REQ), sWorld->getIntConfig(WorldIntConfigs::CONFIG_CHAT_WHISPER_LEVEL_REQ));
                return;
            }

            if (GetPlayer()->GetTeam() != receiver->GetTeam() && !HasPermission(rbac::RBAC_PERM_TWO_SIDE_INTERACTION_CHAT) && !receiver->IsInWhisperWhiteList(sender->GetGUID()))
            {
                SendWrongFactionNotice();
                return;
            }

            if (GetPlayer()->HasAura(1852) && !receiver->IsGameMaster())
            {
                SendNotification(GetSkyFireString(LANG_GM_SILENCE), GetPlayer()->GetName().c_str());
                return;
            }

            // If player is a Gamemaster and doesn't accept whisper, we auto-whitelist every player that the Gamemaster is talking to
            // We also do that if a player is under the required level for whispers.
            if (receiver->getLevel() < sWorld->getIntConfig(WorldIntConfigs::CONFIG_CHAT_WHISPER_LEVEL_REQ) ||
                (HasPermission(rbac::RBAC_PERM_CAN_FILTER_WHISPERS) && !sender->isAcceptWhispers() && !sender->IsInWhisperWhiteList(receiver->GetGUID())))
                sender->AddWhisperWhiteList(receiver->GetGUID());

            GetPlayer()->Whisper(msg, lang, receiver->GetGUID());
        } break;
        case ChatMsg::CHAT_MSG_PARTY:
        case ChatMsg::CHAT_MSG_PARTY_LEADER:
        {
            // if player is in battleground, he cannot say to battleground members by /p
            Group* group = GetPlayer()->GetOriginalGroup();
            if (!group)
            {
                group = _player->GetGroup();
                if (!group || group->isBGGroup())
                    return;
            }

            if (group->IsLeader(GetPlayer()->GetGUID()))
                type = ChatMsg::CHAT_MSG_PARTY_LEADER;

            sScriptMgr->OnPlayerChat(GetPlayer(), type, lang, msg, group);

            WorldPacket data;
            ChatHandler::BuildChatPacket(data, ChatMsg(type), Language(lang), _player, NULL, msg);
            group->BroadcastPacket(&data, false, group->GetMemberGroup(GetPlayer()->GetGUID()));
        } break;
        case ChatMsg::CHAT_MSG_GUILD:
        {
            if (GetPlayer()->GetGuildId())
            {
                if (Guild* guild = sGuildMgr->GetGuildById(GetPlayer()->GetGuildId()))
                {
                    sScriptMgr->OnPlayerChat(GetPlayer(), type, lang, msg, guild);

                    guild->BroadcastToGuild(this, false, msg, lang == Language::LANG_ADDON ? Language::LANG_ADDON : Language::LANG_UNIVERSAL);
                }
            }
        } break;
        case ChatMsg::CHAT_MSG_OFFICER:
        {
            if (GetPlayer()->GetGuildId())
            {
                if (Guild* guild = sGuildMgr->GetGuildById(GetPlayer()->GetGuildId()))
                {
                    sScriptMgr->OnPlayerChat(GetPlayer(), type, lang, msg, guild);

                    guild->BroadcastToGuild(this, true, msg, lang == Language::LANG_ADDON ? Language::LANG_ADDON : Language::LANG_UNIVERSAL);
                }
            }
        } break;
        case ChatMsg::CHAT_MSG_RAID:
        case ChatMsg::CHAT_MSG_RAID_LEADER:
        {
            // if player is in battleground, he cannot say to battleground members by /ra
            Group* group = GetPlayer()->GetOriginalGroup();
            if (!group)
            {
                group = GetPlayer()->GetGroup();
                if (!group || group->isBGGroup() || !group->isRaidGroup())
                    return;
            }

            if (group->IsLeader(GetPlayer()->GetGUID()))
                type = ChatMsg::CHAT_MSG_RAID_LEADER;

            sScriptMgr->OnPlayerChat(GetPlayer(), type, lang, msg, group);

            WorldPacket data;
            ChatHandler::BuildChatPacket(data, ChatMsg(type), lang, _player, NULL, msg);
            group->BroadcastPacket(&data, false);
        } break;
        case ChatMsg::CHAT_MSG_RAID_WARNING:
        {
            Group* group = GetPlayer()->GetGroup();
            if (!group || !group->isRaidGroup() || !(group->IsLeader(GetPlayer()->GetGUID()) || group->IsAssistant(GetPlayer()->GetGUID())) || group->isBGGroup())
                return;

            sScriptMgr->OnPlayerChat(GetPlayer(), type, lang, msg, group);

            WorldPacket data;
            //in battleground, raid warning is sent only to players in battleground - code is ok
            ChatHandler::BuildChatPacket(data, ChatMsg::CHAT_MSG_RAID_WARNING, lang, _player, NULL, msg);
            group->BroadcastPacket(&data, false);
        } break;
        case ChatMsg::CHAT_MSG_INSTANCE:
        case ChatMsg::CHAT_MSG_INSTANCE_LEADER:
        {
            // battleground raid is always in Player->GetGroup(), never in GetOriginalGroup()
            Group* group = GetPlayer()->GetGroup();
            if (!group || !group->isBGGroup())
                return;

            if (group->IsLeader(GetPlayer()->GetGUID()))
                type = ChatMsg::CHAT_MSG_INSTANCE_LEADER;

            sScriptMgr->OnPlayerChat(GetPlayer(), type, lang, msg, group);

            WorldPacket data;
            ChatHandler::BuildChatPacket(data, (ChatMsg)type, Language(lang), _player, NULL, msg);
            group->BroadcastPacket(&data, false);
        } break;
        case ChatMsg::CHAT_MSG_CHANNEL:
        {
            if (!HasPermission(rbac::RBAC_PERM_SKIP_CHECK_CHAT_CHANNEL_REQ))
            {
                if (_player->getLevel() < sWorld->getIntConfig(WorldIntConfigs::CONFIG_CHAT_CHANNEL_LEVEL_REQ))
                {
                    SendNotification(GetSkyFireString(LANG_CHANNEL_REQ), sWorld->getIntConfig(WorldIntConfigs::CONFIG_CHAT_CHANNEL_LEVEL_REQ));
                    return;
                }
            }

            if (ChannelMgr* cMgr = ChannelMgr::forTeam(_player->GetTeam()))
            {
                if (Channel* chn = cMgr->GetChannel(channel, _player))
                {
                    sScriptMgr->OnPlayerChat(_player, type, lang, msg, chn);
                    chn->Say(_player->GetGUID(), msg.c_str(), lang);
                }
            }
        } break;
        case ChatMsg::CHAT_MSG_AFK:
        {
            if (!_player->IsInCombat())
            {
                if (_player->isAFK())                       // Already AFK
                {
                    if (msg.empty())
                        _player->ToggleAFK();               // Remove AFK
                    else
                        _player->autoReplyMsg = msg;        // Update message
                }
                else                                        // New AFK mode
                {
                    _player->autoReplyMsg = msg.empty() ? GetSkyFireString(LANG_PLAYER_AFK_DEFAULT) : msg;

                    if (_player->isDND())
                        _player->ToggleDND();

                    _player->ToggleAFK();
                }

                sScriptMgr->OnPlayerChat(_player, type, lang, msg);
            }
            break;
        }
        case ChatMsg::CHAT_MSG_DND:
        {
            if (_player->isDND())                           // Already DND
            {
                if (msg.empty())
                    _player->ToggleDND();                   // Remove DND
                else
                    _player->autoReplyMsg = msg;            // Update message
            }
            else                                            // New DND mode
            {
                _player->autoReplyMsg = msg.empty() ? GetSkyFireString(LANG_PLAYER_DND_DEFAULT) : msg;

                if (_player->isAFK())
                    _player->ToggleAFK();

                _player->ToggleDND();
            }

            sScriptMgr->OnPlayerChat(_player, type, lang, msg);
            break;
        }
        default:
            SF_LOG_ERROR("network", "CHAT: unknown message type %u, lang: %u", uint8(type), uint8(lang));
            break;
    }
}

void WorldSession::HandleAddonMessagechatOpcode(WorldPacket& recvData)
{
    Player* sender = GetPlayer();
    ChatMsg type;

    switch (recvData.GetOpcode())
    {
        case CMSG_MESSAGECHAT_ADDON_BATTLEGROUND:
            type = ChatMsg::CHAT_MSG_INSTANCE;
            break;
        case CMSG_MESSAGECHAT_ADDON_GUILD:
            type = ChatMsg::CHAT_MSG_GUILD;
            break;
        case CMSG_MESSAGECHAT_ADDON_OFFICER:
            type = ChatMsg::CHAT_MSG_OFFICER;
            break;
        case CMSG_MESSAGECHAT_ADDON_PARTY:
            type = ChatMsg::CHAT_MSG_PARTY;
            break;
        case CMSG_MESSAGECHAT_ADDON_RAID:
            type = ChatMsg::CHAT_MSG_RAID;
            break;
        case CMSG_MESSAGECHAT_ADDON_WHISPER:
            type = ChatMsg::CHAT_MSG_WHISPER;
            break;
        default:
            SF_LOG_ERROR("network", "HandleAddonMessagechatOpcode: Unknown addon chat opcode (%u)", recvData.GetOpcode());
            recvData.hexlike();
            return;
    }

    std::string message;
    std::string prefix;
    std::string targetName;

    switch (type)
    {
        case ChatMsg::CHAT_MSG_WHISPER:
        {
            uint32 msgLen = recvData.ReadBits(9);
            uint32 prefixLen = recvData.ReadBits(5);
            uint32 targetLen = recvData.ReadBits(10);
            message = recvData.ReadString(msgLen);
            prefix = recvData.ReadString(prefixLen);
            targetName = recvData.ReadString(targetLen);
            break;
        }
        case ChatMsg::CHAT_MSG_PARTY:
        case ChatMsg::CHAT_MSG_RAID:
        case ChatMsg::CHAT_MSG_OFFICER:
        {
            uint32 prefixLen = recvData.ReadBits(5);
            uint32 msgLen = recvData.ReadBits(9);
            prefix = recvData.ReadString(prefixLen);
            message = recvData.ReadString(msgLen);
            break;
        }
        case ChatMsg::CHAT_MSG_GUILD:
        case ChatMsg::CHAT_MSG_INSTANCE:
        {
            uint32 msgLen = recvData.ReadBits(9);
            uint32 prefixLen = recvData.ReadBits(5);
            message = recvData.ReadString(msgLen);
            prefix = recvData.ReadString(prefixLen);
            break;
        }
        default:
            break;
    }

    if (prefix.empty() || prefix.length() > 16)
        return;

    // Logging enabled?
    if (sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_CHATLOG_ADDON))
    {
        if (message.empty())
            return;

        // Weird way to log stuff...
        sScriptMgr->OnPlayerChat(sender, ChatMsg::CHAT_MSG_ADDON, Language::LANG_ADDON, message);
    }

    // Disabled addon channel?
    if (!sWorld->GetBoolConfig(WorldBoolConfigs::CONFIG_ADDON_CHANNEL))
        return;

    switch (type)
    {
        case ChatMsg::CHAT_MSG_INSTANCE:
        {
            Group* group = sender->GetGroup();
            if (!group || !group->isBGGroup())
                return;

            WorldPacket data;
            ChatHandler::BuildChatPacket(data, type, Language::LANG_ADDON, sender, NULL, message, 0U, "", DEFAULT_LOCALE, prefix);
            group->BroadcastAddonMessagePacket(&data, prefix, false);
            break;
        }
        case ChatMsg::CHAT_MSG_GUILD:
        case ChatMsg::CHAT_MSG_OFFICER:
        {
            if (sender->GetGuildId())
                if (Guild* guild = sGuildMgr->GetGuildById(sender->GetGuildId()))
                    guild->BroadcastAddonToGuild(this, type == ChatMsg::CHAT_MSG_OFFICER, message, prefix);
            break;
        }
        case ChatMsg::CHAT_MSG_WHISPER:
        {
            if (!normalizePlayerName(targetName))
                break;
            Player* receiver = sObjectAccessor->FindPlayerByName(targetName.c_str());
            if (!receiver)
                break;

            sender->WhisperAddon(message, prefix, receiver);
            break;
        }
        // Messages sent to "RAID" while in a party will get delivered to "PARTY"
        case ChatMsg::CHAT_MSG_PARTY:
        case ChatMsg::CHAT_MSG_RAID:
        {
            Group* group = sender->GetGroup();
            if (!group || group->isBGGroup())
                break;

            WorldPacket data;
            ChatHandler::BuildChatPacket(data, type, Language::LANG_ADDON, sender, NULL, message, 0U, "", DEFAULT_LOCALE, prefix);
            group->BroadcastAddonMessagePacket(&data, prefix, true, -1, group->GetMemberGroup(sender->GetGUID()));
            break;
        }
        default:
        {
            SF_LOG_ERROR("misc", "HandleAddonMessagechatOpcode: unknown addon message type %u", uint8(type));
            break;
        }
    }
}

void WorldSession::HandleEmoteOpcode(WorldPacket& recvData)
{
    if (!GetPlayer()->IsAlive() || GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        return;

    uint32 emote;
    recvData >> emote;
    sScriptMgr->OnPlayerEmote(GetPlayer(), emote);
    GetPlayer()->HandleEmoteCommand(emote);
}

namespace Skyfire
{
    class EmoteChatBuilder
    {
    public:
        EmoteChatBuilder(Player const& player, uint32 text_emote, uint32 emote_num, Unit const* target)
            : i_player(player), i_text_emote(text_emote), i_emote_num(emote_num), i_target(target) { }

        void operator()(WorldPacket& data, LocaleConstant loc_idx)
        {
            ObjectGuid PlayerGuid = i_player.GetGUID();
            ObjectGuid TargetGuid = i_target ? i_target->GetGUID() : 0;

            data.Initialize(SMSG_TEXT_EMOTE, 2 * (8 + 1) + 4 + 4);

            data.WriteBit(PlayerGuid[1]);
            data.WriteBit(TargetGuid[7]);
            data.WriteBit(PlayerGuid[6]);
            data.WriteBit(TargetGuid[5]);
            data.WriteBit(PlayerGuid[3]);
            data.WriteBit(TargetGuid[6]);
            data.WriteBit(TargetGuid[2]);
            data.WriteBit(PlayerGuid[7]);
            data.WriteBit(TargetGuid[0]);
            data.WriteBit(TargetGuid[1]);
            data.WriteBit(PlayerGuid[4]);
            data.WriteBit(PlayerGuid[2]);
            data.WriteBit(TargetGuid[3]);
            data.WriteBit(TargetGuid[4]);
            data.WriteBit(PlayerGuid[0]);
            data.WriteBit(PlayerGuid[5]);

            data.WriteByteSeq(TargetGuid[2]);
            data.WriteByteSeq(TargetGuid[1]);
            data.WriteByteSeq(PlayerGuid[7]);
            data.WriteByteSeq(PlayerGuid[4]);
            data.WriteByteSeq(TargetGuid[7]);
            data.WriteByteSeq(PlayerGuid[5]);
            data.WriteByteSeq(PlayerGuid[2]);

            data << uint32(i_text_emote);

            data.WriteByteSeq(PlayerGuid[6]);
            data.WriteByteSeq(TargetGuid[0]);
            data.WriteByteSeq(PlayerGuid[3]);
            data.WriteByteSeq(PlayerGuid[1]);
            data.WriteByteSeq(TargetGuid[6]);
            data.WriteByteSeq(PlayerGuid[0]);
            data.WriteByteSeq(TargetGuid[3]);
            data.WriteByteSeq(TargetGuid[5]);
            data.WriteByteSeq(TargetGuid[4]);

            data << uint32(i_emote_num);
        }

    private:
        Player const& i_player;
        uint32        i_text_emote;
        uint32        i_emote_num;
        Unit const* i_target;
    };
}                                                           // namespace Skyfire

void WorldSession::HandleTextEmoteOpcode(WorldPacket& recvData)
{
    if (!GetPlayer()->IsAlive())
        return;

    if (!GetPlayer()->CanSpeak())
    {
        std::string timeStr = secsToTimeString(m_muteTime - time(NULL));
        SendNotification(GetSkyFireString(LANG_WAIT_BEFORE_SPEAKING), timeStr.c_str());
        return;
    }

    ObjectGuid guid;
    uint32 text_emote, emoteNum;

    recvData >> text_emote;
    recvData >> emoteNum;

    guid[6] = recvData.ReadBit();
    guid[7] = recvData.ReadBit();
    guid[3] = recvData.ReadBit();
    guid[2] = recvData.ReadBit();
    guid[0] = recvData.ReadBit();
    guid[5] = recvData.ReadBit();
    guid[1] = recvData.ReadBit();
    guid[4] = recvData.ReadBit();

    recvData.ReadByteSeq(guid[0]);
    recvData.ReadByteSeq(guid[5]);
    recvData.ReadByteSeq(guid[1]);
    recvData.ReadByteSeq(guid[4]);
    recvData.ReadByteSeq(guid[2]);
    recvData.ReadByteSeq(guid[3]);
    recvData.ReadByteSeq(guid[7]);
    recvData.ReadByteSeq(guid[6]);

    sScriptMgr->OnPlayerTextEmote(GetPlayer(), text_emote, emoteNum, guid);

    EmotesTextEntry const* em = sEmotesTextStore.LookupEntry(text_emote);
    if (!em)
        return;

    uint32 emote_anim = em->textid;

    switch (emote_anim)
    {
        case EMOTE_STATE_SLEEP:
        case EMOTE_STATE_SIT:
        case EMOTE_STATE_KNEEL:
        case EMOTE_ONESHOT_NONE:
            break;
        default:
            // Only allow text-emotes for "dead" entities (feign death included)
            if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
                break;
            GetPlayer()->HandleEmoteCommand(emote_anim);
            break;
    }

    Unit* unit = ObjectAccessor::GetUnit(*_player, guid);

    CellCoord p = Skyfire::ComputeCellCoord(GetPlayer()->GetPositionX(), GetPlayer()->GetPositionY());

    Cell cell(p);
    cell.SetNoCreate();

    Skyfire::EmoteChatBuilder emote_builder(*GetPlayer(), text_emote, emoteNum, unit);
    Skyfire::LocalizedPacketDo<Skyfire::EmoteChatBuilder > emote_do(emote_builder);
    Skyfire::PlayerDistWorker<Skyfire::LocalizedPacketDo<Skyfire::EmoteChatBuilder > > emote_worker(GetPlayer(), sWorld->GetFloatConfig(WorldFloatConfigs::CONFIG_LISTEN_RANGE_TEXTEMOTE), emote_do);
    TypeContainerVisitor<Skyfire::PlayerDistWorker<Skyfire::LocalizedPacketDo<Skyfire::EmoteChatBuilder> >, WorldTypeMapContainer> message(emote_worker);
    cell.Visit(p, message, *GetPlayer()->GetMap(), *GetPlayer(), sWorld->GetFloatConfig(WorldFloatConfigs::CONFIG_LISTEN_RANGE_TEXTEMOTE));

    GetPlayer()->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE, text_emote, 0, 0, unit);

    //Send scripted event call
    if (unit && unit->GetTypeId() == TypeID::TYPEID_UNIT && ((Creature*)unit)->AI())
        ((Creature*)unit)->AI()->ReceiveEmote(GetPlayer(), text_emote);
}

void WorldSession::HandleChatIgnoredOpcode(WorldPacket& recvData)
{
    ObjectGuid guid;
    uint8 unk;
    //SF_LOG_DEBUG("network", "WORLD: Received CMSG_CHAT_IGNORED");

    guid[5] = recvData.ReadBit();
    recvData >> unk;                                       // probably related to spam reporting
    guid[0] = recvData.ReadBit();
    guid[1] = recvData.ReadBit();
    guid[3] = recvData.ReadBit();
    guid[6] = recvData.ReadBit();
    guid[7] = recvData.ReadBit();
    guid[4] = recvData.ReadBit();
    guid[2] = recvData.ReadBit();

    recvData.ReadByteSeq(guid[2]);
    recvData.ReadByteSeq(guid[0]);
    recvData.ReadByteSeq(guid[3]);
    recvData.ReadByteSeq(guid[4]);
    recvData.ReadByteSeq(guid[7]);
    recvData.ReadByteSeq(guid[6]);
    recvData.ReadByteSeq(guid[0]);
    recvData.ReadByteSeq(guid[5]);

    Player* player = ObjectAccessor::FindPlayer(guid);
    if (!player || !player->GetSession())
        return;

    WorldPacket data;
    ChatHandler::BuildChatPacket(data, ChatMsg::CHAT_MSG_IGNORED, Language::LANG_UNIVERSAL, _player, _player, GetPlayer()->GetName());
    player->GetSession()->SendPacket(&data);
}

void WorldSession::HandleChannelDeclineInvite(WorldPacket& recvPacket)
{
    SF_LOG_DEBUG("network", "Opcode %u", recvPacket.GetOpcode());
}

void WorldSession::SendPlayerNotFoundNotice(std::string const& name)
{
    WorldPacket data(SMSG_CHAT_PLAYER_NOT_FOUND, name.size() + 2);
    data.WriteBits(name.size(), 9);
    data.FlushBits();
    data << name;
    SendPacket(&data);
}

void WorldSession::SendPlayerAmbiguousNotice(std::string const& name)
{
    WorldPacket data(SMSG_CHAT_PLAYER_AMBIGUOUS, name.size() + 2);
    data.WriteBits(name.size(), 9);
    data.FlushBits();
    data << name;
    SendPacket(&data);
}

void WorldSession::SendWrongFactionNotice()
{
    WorldPacket data(SMSG_CHAT_WRONG_FACTION, 0);
    SendPacket(&data);
}

void WorldSession::SendChatRestrictedNotice(ChatRestrictionType restriction)
{
    WorldPacket data(SMSG_CHAT_RESTRICTED, 1);
    data << uint8(restriction);
    SendPacket(&data);
}
