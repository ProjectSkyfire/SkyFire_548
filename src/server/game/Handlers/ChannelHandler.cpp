/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ChannelMgr.h"
#include "ObjectMgr.h"                                      // for normalizePlayerName
#include "Player.h"
#include "WorldSession.h"
#include "ChatDelivery.h"

void WorldSession::HandleJoinChannel(WorldPacket& recvPacket)
{
    if (Skyfire::Chat::Delivery::Control(this, recvPacket, &WorldSession::HandleJoinChannel,
        Skyfire::Chat::AudienceKind::ChannelControl, "HandleJoinChannel")) return;
    uint32 channelId;
    uint32 channelLength, passLength;
    std::string channelName, password;

    recvPacket >> channelId;

    uint8 unknown1 = recvPacket.ReadBit();                  // unknown bit
    channelLength = recvPacket.ReadBits(7);
    passLength = recvPacket.ReadBits(7);
    uint8 unknown2 = recvPacket.ReadBit();                  // unknown bit

    password = recvPacket.ReadString(passLength);
    channelName = recvPacket.ReadString(channelLength);

    SF_LOG_DEBUG("chat.system", "CMSG_CHAT_JOIN_CHANNEL %s Channel: %u, unk1: %u, unk2: %u, channel: %s",
        GetPlayerInfo().c_str(), channelId, unknown1, unknown2, channelName.c_str());

    if (channelId)
    {
        ChatChannelsEntry const* channel = sChatChannelsStore.LookupEntry(channelId);
        if (!channel)
            return;

        AreaTableEntry const* zone = GetAreaEntryByAreaID(GetPlayer()->GetZoneId());
        if (!zone || !GetPlayer()->CanJoinConstantChannelInZone(channel, zone))
            return;
    }

    if (channelName.empty())
        return;

    if (ChannelMgr* cMgr = ChannelMgr::forTeam(GetPlayer()->GetTeam()))
    {
        cMgr->setTeam(GetPlayer()->GetTeam());
        if (Channel* channel = cMgr->GetJoinChannel(channelName, channelId))
            channel->JoinChannel(GetPlayer(), password);
    }
}

void WorldSession::HandleLeaveChannel(WorldPacket& recvPacket)
{
    if (Skyfire::Chat::Delivery::Control(this, recvPacket, &WorldSession::HandleLeaveChannel,
        Skyfire::Chat::AudienceKind::ChannelControl, "HandleLeaveChannel")) return;
    uint32 unk;
    std::string channelName;
    recvPacket >> unk;                                      // channel id?
    uint32 length = recvPacket.ReadBits(7);
    channelName = recvPacket.ReadString(length);

    SF_LOG_DEBUG("chat.system", "CMSG_LEAVE_CHANNEL %s Channel: %s, unk1: %u",
        GetPlayerInfo().c_str(), channelName.c_str(), unk);

    if (channelName.empty())
        return;

    if (ChannelMgr* cMgr = ChannelMgr::forTeam(GetPlayer()->GetTeam()))
    {
        if (Channel* channel = cMgr->GetChannel(channelName, GetPlayer()))
            channel->LeaveChannel(GetPlayer(), true);
        cMgr->LeftChannel(channelName);
    }
}

void WorldSession::HandleChannelList(WorldPacket& recvPacket)
{
    if (Skyfire::Chat::Delivery::Control(this, recvPacket, &WorldSession::HandleChannelList,
        Skyfire::Chat::AudienceKind::ChannelControl, "HandleChannelList")) return;
    uint32 length = recvPacket.ReadBits(7);
    std::string channelName = recvPacket.ReadString(length);

    SF_LOG_DEBUG("chat.system", "%s %s Channel: %s",
        recvPacket.GetOpcode() == CMSG_CHANNEL_DISPLAY_LIST ? "CMSG_CHANNEL_DISPLAY_LIST" : "CMSG_CHANNEL_LIST",
        GetPlayerInfo().c_str(), channelName.c_str());

    if (ChannelMgr* cMgr = ChannelMgr::forTeam(GetPlayer()->GetTeam()))
        if (Channel* channel = cMgr->GetChannel(channelName, GetPlayer()))
            channel->List(GetPlayer());
}

void WorldSession::HandleChannelPassword(WorldPacket& recvPacket)
{
    if (Skyfire::Chat::Delivery::Control(this, recvPacket, &WorldSession::HandleChannelPassword,
        Skyfire::Chat::AudienceKind::ChannelControl, "HandleChannelPassword")) return;
    uint32 nameLength = recvPacket.ReadBits(8);
    uint32 passLength = recvPacket.ReadBits(7);

    std::string channelName = recvPacket.ReadString(nameLength);
    std::string password = recvPacket.ReadString(passLength);

    SF_LOG_DEBUG("chat.system", "CMSG_CHANNEL_PASSWORD %s Channel: %s",
        GetPlayerInfo().c_str(), channelName.c_str());

    if (ChannelMgr* cMgr = ChannelMgr::forTeam(GetPlayer()->GetTeam()))
        if (Channel* channel = cMgr->GetChannel(channelName, GetPlayer()))
            channel->Password(GetPlayer(), password);
}

void WorldSession::HandleChannelSetOwner(WorldPacket& recvPacket)
{
    if (Skyfire::Chat::Delivery::Control(this, recvPacket, &WorldSession::HandleChannelSetOwner,
        Skyfire::Chat::AudienceKind::ChannelControl, "HandleChannelSetOwner")) return;
    uint32 nameLength = recvPacket.ReadBits(8) << 1;
    nameLength += recvPacket.ReadBit();
    uint32 channelLength = recvPacket.ReadBits(7);

    std::string channelName = recvPacket.ReadString(channelLength);
    std::string targetName = recvPacket.ReadString(nameLength);

    SF_LOG_DEBUG("chat.system", "CMSG_CHANNEL_SET_OWNER %s Channel: %s, Target: %s",
        GetPlayerInfo().c_str(), channelName.c_str(), targetName.c_str());

    if (!normalizePlayerName(targetName))
        return;

    if (ChannelMgr* cMgr = ChannelMgr::forTeam(GetPlayer()->GetTeam()))
        if (Channel* channel = cMgr->GetChannel(channelName, GetPlayer()))
            channel->SetOwner(GetPlayer(), targetName);
}

void WorldSession::HandleChannelOwner(WorldPacket& recvPacket)
{
    if (Skyfire::Chat::Delivery::Control(this, recvPacket, &WorldSession::HandleChannelOwner,
        Skyfire::Chat::AudienceKind::ChannelControl, "HandleChannelOwner")) return;
    uint32 length = recvPacket.ReadBits(8);
    std::string channelName = recvPacket.ReadString(length);

    SF_LOG_DEBUG("chat.system", "CMSG_CHANNEL_OWNER %s Channel: %s",
        GetPlayerInfo().c_str(), channelName.c_str());

    if (ChannelMgr* cMgr = ChannelMgr::forTeam(GetPlayer()->GetTeam()))
        if (Channel* channel = cMgr->GetChannel(channelName, GetPlayer()))
            channel->SendWhoOwner(GetPlayer()->GetGUID());
}

void WorldSession::HandleChannelModerator(WorldPacket& recvPacket)
{
    if (Skyfire::Chat::Delivery::Control(this, recvPacket, &WorldSession::HandleChannelModerator,
        Skyfire::Chat::AudienceKind::ChannelControl, "HandleChannelModerator")) return;
    uint32 channelLength = recvPacket.ReadBits(8);
    uint32 nameLength = recvPacket.ReadBits(7);

    std::string targetName = recvPacket.ReadString(nameLength);
    std::string channelName = recvPacket.ReadString(channelLength);

    SF_LOG_DEBUG("chat.system", "CMSG_CHANNEL_MODERATOR %s Channel: %s, Target: %s",
        GetPlayerInfo().c_str(), channelName.c_str(), targetName.c_str());

    if (!normalizePlayerName(targetName))
        return;

    if (ChannelMgr* cMgr = ChannelMgr::forTeam(GetPlayer()->GetTeam()))
        if (Channel* channel = cMgr->GetChannel(channelName, GetPlayer()))
            channel->SetModerator(GetPlayer(), targetName);
}

void WorldSession::HandleChannelUnmoderator(WorldPacket& recvPacket)
{
    if (Skyfire::Chat::Delivery::Control(this, recvPacket, &WorldSession::HandleChannelUnmoderator,
        Skyfire::Chat::AudienceKind::ChannelControl, "HandleChannelUnmoderator")) return;
    uint32 nameLength = recvPacket.ReadBits(7);
    uint32 channelLength = recvPacket.ReadBits(8);

    std::string channelName = recvPacket.ReadString(channelLength);
    std::string targetName = recvPacket.ReadString(nameLength);

    SF_LOG_DEBUG("chat.system", "CMSG_CHANNEL_UNMODERATOR %s Channel: %s, Target: %s",
        GetPlayerInfo().c_str(), channelName.c_str(), targetName.c_str());

    if (!normalizePlayerName(targetName))
        return;

    if (ChannelMgr* cMgr = ChannelMgr::forTeam(GetPlayer()->GetTeam()))
        if (Channel* channel = cMgr->GetChannel(channelName, GetPlayer()))
            channel->UnsetModerator(GetPlayer(), targetName);
}

void WorldSession::HandleChannelMute(WorldPacket& recvPacket)
{
    if (Skyfire::Chat::Delivery::Control(this, recvPacket, &WorldSession::HandleChannelMute,
        Skyfire::Chat::AudienceKind::ChannelControl, "HandleChannelMute")) return;
    uint32 channelLength = recvPacket.ReadBits(8);
    uint32 nameLength = recvPacket.ReadBits(7);

    std::string channelName = recvPacket.ReadString(channelLength);
    std::string targetName = recvPacket.ReadString(nameLength);

    SF_LOG_DEBUG("chat.system", "CMSG_CHANNEL_MUTE %s Channel: %s, Target: %s",
        GetPlayerInfo().c_str(), channelName.c_str(), targetName.c_str());

    if (!normalizePlayerName(targetName))
        return;

    if (ChannelMgr* cMgr = ChannelMgr::forTeam(GetPlayer()->GetTeam()))
        if (Channel* channel = cMgr->GetChannel(channelName, GetPlayer()))
            channel->SetMute(GetPlayer(), targetName);
}

void WorldSession::HandleChannelUnmute(WorldPacket& recvPacket)
{
    if (Skyfire::Chat::Delivery::Control(this, recvPacket, &WorldSession::HandleChannelUnmute,
        Skyfire::Chat::AudienceKind::ChannelControl, "HandleChannelUnmute")) return;
    uint32 nameLength = recvPacket.ReadBits(8);
    uint32 channelLength = recvPacket.ReadBits(7);

    std::string targetName = recvPacket.ReadString(nameLength);
    std::string channelName = recvPacket.ReadString(channelLength);

    SF_LOG_DEBUG("chat.system", "CMSG_CHANNEL_UNMUTE %s Channel: %s, Target: %s",
        GetPlayerInfo().c_str(), channelName.c_str(), targetName.c_str());

    if (!normalizePlayerName(targetName))
        return;

    if (ChannelMgr* cMgr = ChannelMgr::forTeam(GetPlayer()->GetTeam()))
        if (Channel* channel = cMgr->GetChannel(channelName, GetPlayer()))
            channel->UnsetMute(GetPlayer(), targetName);
}

void WorldSession::HandleChannelInvite(WorldPacket& recvPacket)
{
    if (Skyfire::Chat::Delivery::Control(this, recvPacket, &WorldSession::HandleChannelInvite,
        Skyfire::Chat::AudienceKind::ChannelControl, "HandleChannelInvite")) return;
    uint32 nameLength = recvPacket.ReadBits(7);
    uint32 channelLength = recvPacket.ReadBits(8);

    std::string targetName = recvPacket.ReadString(nameLength);
    std::string channelName = recvPacket.ReadString(channelLength);

    SF_LOG_DEBUG("chat.system", "CMSG_CHANNEL_INVITE %s Channel: %s, Target: %s",
        GetPlayerInfo().c_str(), channelName.c_str(), targetName.c_str());

    if (!normalizePlayerName(targetName))
        return;

    if (ChannelMgr* cMgr = ChannelMgr::forTeam(GetPlayer()->GetTeam()))
        if (Channel* channel = cMgr->GetChannel(channelName, GetPlayer()))
            channel->Invite(GetPlayer(), targetName);
}

void WorldSession::HandleChannelKick(WorldPacket& recvPacket)
{
    if (Skyfire::Chat::Delivery::Control(this, recvPacket, &WorldSession::HandleChannelKick,
        Skyfire::Chat::AudienceKind::ChannelControl, "HandleChannelKick")) return;
    uint32 channelLength = recvPacket.ReadBits(8);
    uint32 nameLength = recvPacket.ReadBits(7);

    std::string channelName = recvPacket.ReadString(channelLength);
    std::string targetName = recvPacket.ReadString(nameLength);

    SF_LOG_DEBUG("chat.system", "CMSG_CHANNEL_KICK %s Channel: %s, Target: %s",
        GetPlayerInfo().c_str(), channelName.c_str(), targetName.c_str());

    if (!normalizePlayerName(targetName))
        return;

    if (ChannelMgr* cMgr = ChannelMgr::forTeam(GetPlayer()->GetTeam()))
        if (Channel* channel = cMgr->GetChannel(channelName, GetPlayer()))
            channel->Kick(GetPlayer(), targetName);
}

void WorldSession::HandleChannelBan(WorldPacket& recvPacket)
{
    if (Skyfire::Chat::Delivery::Control(this, recvPacket, &WorldSession::HandleChannelBan,
        Skyfire::Chat::AudienceKind::ChannelControl, "HandleChannelBan")) return;
    uint32 channelLength, nameLength;
    std::string channelName, targetName;

    channelLength = recvPacket.ReadBits(8);
    nameLength = recvPacket.ReadBits(7);

    targetName = recvPacket.ReadString(nameLength);
    channelName = recvPacket.ReadString(channelLength);

    SF_LOG_DEBUG("chat.system", "CMSG_CHANNEL_BAN %s Channel: %s, Target: %s",
        GetPlayerInfo().c_str(), channelName.c_str(), targetName.c_str());

    if (!normalizePlayerName(targetName))
        return;

    if (ChannelMgr* cMgr = ChannelMgr::forTeam(GetPlayer()->GetTeam()))
        if (Channel* channel = cMgr->GetChannel(channelName, GetPlayer()))
            channel->Ban(GetPlayer(), targetName);
}

void WorldSession::HandleChannelUnban(WorldPacket& recvPacket)
{
    if (Skyfire::Chat::Delivery::Control(this, recvPacket, &WorldSession::HandleChannelUnban,
        Skyfire::Chat::AudienceKind::ChannelControl, "HandleChannelUnban")) return;
    uint32 channelLength = recvPacket.ReadBits(7);
    uint32 nameLength = recvPacket.ReadBits(8);

    std::string targetName = recvPacket.ReadString(nameLength);
    std::string channelName = recvPacket.ReadString(channelLength);

    SF_LOG_DEBUG("chat.system", "CMSG_CHANNEL_UNBAN %s Channel: %s, Target: %s",
        GetPlayerInfo().c_str(), channelName.c_str(), targetName.c_str());

    if (!normalizePlayerName(targetName))
        return;

    if (ChannelMgr* cMgr = ChannelMgr::forTeam(GetPlayer()->GetTeam()))
        if (Channel* channel = cMgr->GetChannel(channelName, GetPlayer()))
            channel->UnBan(GetPlayer(), targetName);
}

void WorldSession::HandleChannelAnnouncements(WorldPacket& recvPacket)
{
    if (Skyfire::Chat::Delivery::Control(this, recvPacket, &WorldSession::HandleChannelAnnouncements,
        Skyfire::Chat::AudienceKind::ChannelControl, "HandleChannelAnnouncements")) return;
    uint32 length = recvPacket.ReadBits(8);
    std::string channelName = recvPacket.ReadString(length);

    SF_LOG_DEBUG("chat.system", "CMSG_CHANNEL_ANNOUNCEMENTS %s Channel: %s",
        GetPlayerInfo().c_str(), channelName.c_str());

    if (ChannelMgr* cMgr = ChannelMgr::forTeam(GetPlayer()->GetTeam()))
        if (Channel* channel = cMgr->GetChannel(channelName, GetPlayer()))
            channel->Announce(GetPlayer());
}

void WorldSession::HandleChannelDisplayListQuery(WorldPacket& recvPacket)
{
    if (Skyfire::Chat::Delivery::Control(this, recvPacket, &WorldSession::HandleChannelDisplayListQuery,
        Skyfire::Chat::AudienceKind::ChannelControl, "HandleChannelDisplayListQuery")) return;
    // this should be OK because the 2 function _were_ the same
    HandleChannelList(recvPacket);
}

void WorldSession::HandleGetChannelMemberCount(WorldPacket& recvPacket)
{
    if (Skyfire::Chat::Delivery::Control(this, recvPacket, &WorldSession::HandleGetChannelMemberCount,
        Skyfire::Chat::AudienceKind::ChannelControl, "HandleGetChannelMemberCount")) return;
    std::string channelName;
    recvPacket >> channelName;

    SF_LOG_DEBUG("chat.system", "CMSG_GET_CHANNEL_MEMBER_COUNT %s Channel: %s",
        GetPlayerInfo().c_str(), channelName.c_str());

    if (ChannelMgr* cMgr = ChannelMgr::forTeam(GetPlayer()->GetTeam()))
    {
        if (Channel* channel = cMgr->GetChannel(channelName, GetPlayer()))
        {
            SF_LOG_DEBUG("chat.system", "SMSG_CHANNEL_MEMBER_COUNT %s Channel: %s Count: %u",
                GetPlayerInfo().c_str(), channelName.c_str(), channel->GetNumPlayers());

            WorldPacket data(SMSG_CHANNEL_MEMBER_COUNT, channel->GetName().size() + 1 + 4);
            data << channel->GetName();
            data << uint8(channel->GetFlags());
            data << uint32(channel->GetNumPlayers());
            SendPacket(&data);
        }
    }
}

/*
void WorldSession::HandleSetChannelWatch(WorldPacket& recvPacket)
{
    if (Skyfire::Chat::Delivery::Control(this, recvPacket, &WorldSession::HandleSetChannelWatch,
        Skyfire::Chat::AudienceKind::ChannelControl, "HandleSetChannelWatch")) return;
    std::string channelName;
    recvPacket >> channelName;

    SF_LOG_DEBUG("chat.system", "CMSG_SET_CHANNEL_WATCH %s Channel: %s",
        GetPlayerInfo().c_str(), channelName.c_str());


    if (ChannelMgr* cMgr = channelMgr(GetPlayer()->GetTeam()))
        if (Channel* channel = cMgr->GetChannel(channelName, GetPlayer()))
            channel->JoinNotify(GetPlayer());

}
*/
