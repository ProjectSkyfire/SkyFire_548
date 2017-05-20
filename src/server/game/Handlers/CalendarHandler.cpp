/*
 * Copyright (C) 2011-2017 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2017 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2017 MaNGOS <https://www.getmangos.eu/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/*
----- Opcodes Not Used yet -----

SMSG_CALENDAR_EVENT_INVITE_NOTES        [ packguid(Invitee), uint64(inviteId), string(Text), Boolean(Unk) ]
?CMSG_CALENDAR_EVENT_INVITE_NOTES       [ uint32(unk1), uint32(unk2), uint32(unk3), uint32(unk4), uint32(unk5) ]
SMSG_CALENDAR_EVENT_INVITE_NOTES_ALERT  [ uint64(inviteId), string(Text) ]
SMSG_CALENDAR_RAID_LOCKOUT_UPDATED      SendCalendarRaidLockoutUpdated(InstanceSave const* save)

@todo

Finish complains' handling - what to do with received complains and how to respond?
Find out what to do with all "not used yet" opcodes
Correct errors sending (event/invite not found, invites exceeded, event already passed, permissions etc.)
Fix locked events to be displayed properly and response time shouldn't be shown for people that haven't respond yet
Copied events should probably have a new owner

*/

#include "InstanceSaveMgr.h"
#include "Log.h"
#include "Opcodes.h"
#include "Player.h"
#include "SocialMgr.h"
#include "CalendarMgr.h"
#include "ObjectMgr.h"
#include "ObjectAccessor.h"
#include "DatabaseEnv.h"
#include "GuildMgr.h"
#include "ArenaTeamMgr.h"
#include "WorldSession.h"

void WorldSession::HandleCalendarGetCalendar(WorldPacket& /*recvData*/)
{
    uint64 guid = _player->GetGUID();
    SF_LOG_DEBUG("network", "CMSG_CALENDAR_GET_CALENDAR [" UI64FMTD "]", guid);

    time_t currTime = time(NULL);
    uint32 counter = 0;
    CalendarInviteStore invites = sCalendarMgr->GetPlayerInvites(guid);
    CalendarEventStore playerEvents = sCalendarMgr->GetPlayerEvents(guid);
    ResetTimeByMapDifficultyMap const& resets = sInstanceSaveMgr->GetResetTimeMap();

    ByteBuffer lockoutInfoBuffer;
    ByteBuffer invitesInfoBuffer;
    ByteBuffer eventsInfoBuffer;
    WorldPacket data(SMSG_CALENDAR_SEND_CALENDAR, 1000); // Average size if no instance
    size_t resetPos = data.bitwpos();
    data.WriteBits(0, 20);  // Reset placeholder
    data.WriteBits(0, 16);  // Holidays -> fuck that shit... Necessary to find out when this should be sent
    size_t lockoutPos = data.bitwpos();
    data.WriteBits(0, 20);  // Lockout placeholder

    for (uint8 i = 0; i < MAX_DIFFICULTY; ++i)
    {
        Player::BoundInstancesMap boundInstances = _player->GetBoundInstances(Difficulty(i));
        for (Player::BoundInstancesMap::const_iterator itr = boundInstances.begin(); itr != boundInstances.end(); ++itr)
        {
            if (itr->second.perm)
            {
                InstanceSave const* save = itr->second.save;
                ObjectGuid guid = save->GetInstanceId();
                data.WriteBit(guid[6]);
                data.WriteBit(guid[7]);
                data.WriteBit(guid[2]);
                data.WriteBit(guid[1]);
                data.WriteBit(guid[5]);
                data.WriteBit(guid[4]);
                data.WriteBit(guid[0]);
                data.WriteBit(guid[3]);

                lockoutInfoBuffer << uint32(save->GetDifficulty());
                lockoutInfoBuffer.WriteByteSeq(guid[3]);
                lockoutInfoBuffer.WriteByteSeq(guid[0]);
                lockoutInfoBuffer.WriteByteSeq(guid[1]);
                lockoutInfoBuffer.WriteByteSeq(guid[5]);
                lockoutInfoBuffer << uint32(save->GetResetTime() - currTime);
                lockoutInfoBuffer << uint32(save->GetMapId());
                lockoutInfoBuffer.WriteByteSeq(guid[2]);
                lockoutInfoBuffer.WriteByteSeq(guid[7]);
                lockoutInfoBuffer.WriteByteSeq(guid[6]);
                lockoutInfoBuffer.WriteByteSeq(guid[4]);

                ++counter;
            }
        }
    }

    data.WriteBits(invites.size(), 19);
    for (CalendarInviteStore::const_iterator itr = invites.begin(); itr != invites.end(); ++itr)
    {
        CalendarEvent* calendarEvent = sCalendarMgr->GetEvent((*itr)->GetEventId());
        ObjectGuid guid = (*itr)->GetSenderGUID();
        data.WriteBit(guid[1]);
        data.WriteBit(guid[2]);
        data.WriteBit(guid[6]);
        data.WriteBit(guid[7]);
        data.WriteBit(guid[3]);
        data.WriteBit(guid[0]);
        data.WriteBit(guid[4]);
        data.WriteBit(guid[5]);

        invitesInfoBuffer.WriteByteSeq(guid[2]);
        invitesInfoBuffer << uint64((*itr)->GetInviteId());
        invitesInfoBuffer << uint8((*itr)->GetStatus());
        invitesInfoBuffer.WriteByteSeq(guid[6]);
        invitesInfoBuffer.WriteByteSeq(guid[3]);
        invitesInfoBuffer.WriteByteSeq(guid[4]);
        invitesInfoBuffer.WriteByteSeq(guid[1]);
        invitesInfoBuffer.WriteByteSeq(guid[0]);
        invitesInfoBuffer << uint64((*itr)->GetEventId());
        invitesInfoBuffer.WriteByteSeq(guid[7]);
        invitesInfoBuffer.WriteByteSeq(guid[5]);

        invitesInfoBuffer << uint8((*itr)->GetRank());
        invitesInfoBuffer << uint8((calendarEvent && calendarEvent->IsGuildEvent() && calendarEvent->GetGuildId() == _player->GetGuildId()) ? 1 : 0);
    }

    data.WriteBits(playerEvents.size(), 19);
    for (CalendarEventStore::const_iterator itr = playerEvents.begin(); itr != playerEvents.end(); ++itr)
    {
        CalendarEvent* calendarEvent = *itr;
        Guild* guild = sGuildMgr->GetGuildById(calendarEvent->GetGuildId());
        ObjectGuid guildGuid = guild ? guild->GetGUID() : 0;
        ObjectGuid creatorGuid = calendarEvent->GetCreatorGUID();

        data.WriteBit(creatorGuid[2]);
        data.WriteBit(guildGuid[1]);
        data.WriteBit(guildGuid[7]);
        data.WriteBit(creatorGuid[4]);
        data.WriteBit(guildGuid[5]);
        data.WriteBit(guildGuid[6]);
        data.WriteBit(guildGuid[3]);
        data.WriteBit(guildGuid[4]);
        data.WriteBit(creatorGuid[7]);
        data.WriteBits(calendarEvent->GetTitle().size(), 8);
        data.WriteBit(creatorGuid[1]);
        data.WriteBit(guildGuid[2]);
        data.WriteBit(guildGuid[0]);
        data.WriteBit(creatorGuid[0]);
        data.WriteBit(creatorGuid[3]);
        data.WriteBit(creatorGuid[6]);
        data.WriteBit(creatorGuid[5]);

        eventsInfoBuffer.WriteByteSeq(creatorGuid[5]);
        eventsInfoBuffer.WriteByteSeq(guildGuid[3]);
        eventsInfoBuffer.WriteString(calendarEvent->GetTitle());
        eventsInfoBuffer.WriteByteSeq(guildGuid[7]);
        eventsInfoBuffer << int32(calendarEvent->GetDungeonId());
        eventsInfoBuffer.WriteByteSeq(creatorGuid[0]);
        eventsInfoBuffer.WriteByteSeq(creatorGuid[4]);
        eventsInfoBuffer.WriteByteSeq(guildGuid[2]);
        eventsInfoBuffer.WriteByteSeq(creatorGuid[7]);
        eventsInfoBuffer.WriteByteSeq(creatorGuid[2]);
        eventsInfoBuffer.AppendPackedTime(calendarEvent->GetEventTime());
        eventsInfoBuffer.WriteByteSeq(creatorGuid[3]);
        eventsInfoBuffer.WriteByteSeq(creatorGuid[1]);
        eventsInfoBuffer.WriteByteSeq(guildGuid[6]);
        eventsInfoBuffer.WriteByteSeq(guildGuid[1]);
        eventsInfoBuffer.WriteByteSeq(creatorGuid[6]);
        eventsInfoBuffer << uint32(calendarEvent->GetFlags());
        eventsInfoBuffer.WriteByteSeq(guildGuid[4]);
        eventsInfoBuffer.WriteByteSeq(guildGuid[5]);
        eventsInfoBuffer.WriteByteSeq(guildGuid[0]);
        eventsInfoBuffer << uint64(calendarEvent->GetEventId());
        eventsInfoBuffer << uint8(calendarEvent->GetType());
    }

    data.FlushBits();
    data.PutBits(lockoutPos, counter, 20);
    data.append(eventsInfoBuffer);
    data.append(lockoutInfoBuffer);
    data.append(invitesInfoBuffer);
    data.AppendPackedTime(currTime);                       // zone time

    counter = 0;
    std::set<uint32> sentMaps;
    for (ResetTimeByMapDifficultyMap::const_iterator itr = resets.begin(); itr != resets.end(); ++itr)
    {
        uint32 mapId = PAIR32_LOPART(itr->first);
        if (sentMaps.find(mapId) != sentMaps.end())
            continue;

        MapEntry const* mapEntry = sMapStore.LookupEntry(mapId);
        if (!mapEntry || !mapEntry->IsRaid())
            continue;

        sentMaps.insert(mapId);

        data << int32(mapId);
        data << int32(itr->second - currTime);
        data << int32(0); // offset => found it different only once
        counter++;
    }

    data.PutBits(resetPos, counter, 20);
    data << uint32(1135753200); // Constant date, unk (28.12.2005 07:00)
    data << uint32(currTime);   // server time

    SendPacket(&data);
}

void WorldSession::HandleCalendarGetEvent(WorldPacket& recvData)
{
    uint64 eventId;
    recvData >> eventId;

    SF_LOG_DEBUG("network", "CMSG_CALENDAR_GET_EVENT. Player ["
        UI64FMTD "] Event [" UI64FMTD "]", _player->GetGUID(), eventId);

    if (CalendarEvent* calendarEvent = sCalendarMgr->GetEvent(eventId))
        sCalendarMgr->SendCalendarEvent(_player->GetGUID(), *calendarEvent, CALENDAR_SENDTYPE_GET);
    else
        sCalendarMgr->SendCalendarCommandResult(_player->GetGUID(), CALENDAR_ERROR_EVENT_INVALID);
}

void WorldSession::HandleCalendarGuildFilter(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "CMSG_CALENDAR_GUILD_FILTER [" UI64FMTD "]", _player->GetGUID());

    uint32 minLevel;
    uint32 maxLevel;
    uint32 minRank;

    recvData >> minLevel >> maxLevel >> minRank;

    if (Guild* guild = sGuildMgr->GetGuildById(_player->GetGuildId()))
        guild->MassInviteToEvent(this, minLevel, maxLevel, minRank);

    SF_LOG_DEBUG("network", "CMSG_CALENDAR_GUILD_FILTER: Min level [%d], Max level [%d], Min rank [%d]", minLevel, maxLevel, minRank);
}

void WorldSession::HandleCalendarArenaTeam(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "CMSG_CALENDAR_ARENA_TEAM [" UI64FMTD "]", _player->GetGUID());

    uint32 arenaTeamId;
    recvData >> arenaTeamId;

    if (ArenaTeam* team = sArenaTeamMgr->GetArenaTeamById(arenaTeamId))
        team->MassInviteToEvent(this);
}

void WorldSession::HandleCalendarAddEvent(WorldPacket& recvData)
{
    uint64 guid = _player->GetGUID();

    std::string title;
    std::string description;
    uint8 type;
    int32 dungeonId;
    uint32 eventPackedTime;
    uint32 maxInvites;  // always 100, necesary? Not find the way how to change it
    uint32 flags;
    uint32 inviteeCount;
    uint16 descriptionLength, titleLength;

    recvData >> maxInvites >> flags >> dungeonId;
    recvData.ReadPackedTime(eventPackedTime);
    recvData >> type;
    inviteeCount = recvData.ReadBits(22);
    descriptionLength = recvData.ReadBits(11);

    std::list<CalendarInvitePacketInfo> calendarInviteList;
    for (uint32 i = 0; i < inviteeCount; i++)
    {
        CalendarInvitePacketInfo info;
        info.Guid[7] = recvData.ReadBit();
        info.Guid[2] = recvData.ReadBit();
        info.Guid[6] = recvData.ReadBit();
        info.Guid[3] = recvData.ReadBit();
        info.Guid[5] = recvData.ReadBit();
        info.Guid[1] = recvData.ReadBit();
        info.Guid[0] = recvData.ReadBit();
        info.Guid[4] = recvData.ReadBit();
        calendarInviteList.push_back(info);
    }

    titleLength = recvData.ReadBits(8);

    for (std::list<CalendarInvitePacketInfo>::iterator iter = calendarInviteList.begin(); iter != calendarInviteList.end(); ++iter)
    {
        recvData.ReadByteSeq(iter->Guid[4]);
        recvData.ReadByteSeq(iter->Guid[2]);
        recvData.ReadByteSeq(iter->Guid[3]);
        recvData.ReadByteSeq(iter->Guid[1]);
        recvData.ReadByteSeq(iter->Guid[0]);
        recvData.ReadByteSeq(iter->Guid[6]);
        recvData.ReadByteSeq(iter->Guid[7]);
        recvData >> iter->Status;
        recvData.ReadByteSeq(iter->Guid[5]);
        recvData >> iter->ModerationRank;
    }

    title = recvData.ReadString(titleLength);
    description = recvData.ReadString(descriptionLength);

    CalendarEvent* calendarEvent = new CalendarEvent(sCalendarMgr->GetFreeEventId(), guid, 0, CalendarEventType(type), dungeonId,
        time_t(eventPackedTime), flags, title, description);

    if (calendarEvent->IsGuildEvent() || calendarEvent->IsGuildAnnouncement())
        if (Player* creator = ObjectAccessor::FindPlayer(guid))
            calendarEvent->SetGuildId(creator->GetGuildId());

    if (calendarEvent->IsGuildAnnouncement())
    {
        CalendarInvite* invite = new CalendarInvite(0, calendarEvent->GetEventId(), 0, guid, 946684800, CALENDAR_STATUS_NOT_SIGNED_UP, CALENDAR_RANK_PLAYER, "");
        sCalendarMgr->AddInvite(calendarEvent, invite);
    }
    else
    {
        uint32 inviteCount;
        recvData >> inviteCount;

        for (uint32 i = 0; i < inviteCount; ++i)
        {
            uint64 invitee = 0;
            uint8 status = 0;
            uint8 rank = 0;
            recvData.readPackGUID(invitee);
            recvData >> status >> rank;

            CalendarInvite* invite = new CalendarInvite(sCalendarMgr->GetFreeInviteId(), calendarEvent->GetEventId(), invitee, guid, 946684800, CalendarInviteStatus(status), CalendarModerationRank(rank), "");
            sCalendarMgr->AddInvite(calendarEvent, invite);
        }
    }

    sCalendarMgr->AddEvent(calendarEvent, CALENDAR_SENDTYPE_ADD);
}

void WorldSession::HandleCalendarUpdateEvent(WorldPacket& recvData)
{
    uint64 guid = _player->GetGUID();
    time_t oldEventTime;

    ObjectGuid eventId;
    ObjectGuid inviteId;
    std::string title;
    std::string description;
    uint8 type;
    uint32 maxInvites;
    int32 dungeonId;
    uint32 eventPackedTime;
    uint32 flags;
    uint16 descriptionLength;
    uint16 titleLength;

    recvData >> maxInvites >> dungeonId;
    recvData.ReadPackedTime(eventPackedTime);
    recvData >> flags >> type;
    eventId[4] = recvData.ReadBit();
    eventId[5] = recvData.ReadBit();
    eventId[2] = recvData.ReadBit();
    inviteId[4] = recvData.ReadBit();
    eventId[7] = recvData.ReadBit();
    eventId[0] = recvData.ReadBit();
    inviteId[5] = recvData.ReadBit();
    inviteId[3] = recvData.ReadBit();
    eventId[6] = recvData.ReadBit();
    eventId[1] = recvData.ReadBit();
    inviteId[6] = recvData.ReadBit();
    inviteId[2] = recvData.ReadBit();
    inviteId[7] = recvData.ReadBit();
    inviteId[1] = recvData.ReadBit();
    inviteId[0] = recvData.ReadBit();
    descriptionLength = recvData.ReadBits(11);
    titleLength = recvData.ReadBits(8);
    eventId[3] = recvData.ReadBit();

    recvData.ReadByteSeq(inviteId[6]);
    recvData.ReadByteSeq(eventId[0]);
    recvData.ReadByteSeq(inviteId[7]);
    recvData.ReadByteSeq(inviteId[3]);
    recvData.ReadByteSeq(eventId[6]);
    recvData.ReadByteSeq(inviteId[1]);
    recvData.ReadByteSeq(eventId[2]);
    title = recvData.ReadString(titleLength);
    recvData.ReadByteSeq(inviteId[5]);
    recvData.ReadByteSeq(inviteId[4]);
    recvData.ReadByteSeq(eventId[5]);
    recvData.ReadByteSeq(eventId[3]);
    recvData.ReadByteSeq(inviteId[0]);
    recvData.ReadByteSeq(eventId[4]);
    description = recvData.ReadString(descriptionLength);
    recvData.ReadByteSeq(eventId[1]);
    recvData.ReadByteSeq(inviteId[2]);
    recvData.ReadByteSeq(eventId[7]);

    SF_LOG_DEBUG("network", "CMSG_CALENDAR_UPDATE_EVENT [" UI64FMTD "] EventId [" UI64FMTD
        "], InviteId [" UI64FMTD "] Title %s, Description %s, type %u "
        "MaxInvites %u, Dungeon ID %d, Time %u, Flags %u",
        guid, (uint64)eventId, (uint64)inviteId, title.c_str(),
        description.c_str(), type, maxInvites, dungeonId,
        eventPackedTime, flags);

    if (CalendarEvent* calendarEvent = sCalendarMgr->GetEvent(eventId))
    {
        oldEventTime = calendarEvent->GetEventTime();

        calendarEvent->SetType(CalendarEventType(type));
        calendarEvent->SetFlags(flags);
        calendarEvent->SetEventTime(time_t(eventPackedTime));
        calendarEvent->SetDungeonId(dungeonId);
        calendarEvent->SetTitle(title);
        calendarEvent->SetDescription(description);

        sCalendarMgr->UpdateEvent(calendarEvent);
        sCalendarMgr->SendCalendarEventUpdateAlert(*calendarEvent, oldEventTime);
    }
    else
        sCalendarMgr->SendCalendarCommandResult(guid, CALENDAR_ERROR_EVENT_INVALID);
}

void WorldSession::HandleCalendarRemoveEvent(WorldPacket& recvData)
{
    uint64 guid = _player->GetGUID();
    recvData.read_skip<uint64>(); // Skip InviteId
    uint64 eventId;

    recvData >> eventId;
    recvData.rfinish(); // Skip flags

    sCalendarMgr->RemoveEvent(eventId, guid);
}

void WorldSession::HandleCalendarCopyEvent(WorldPacket& recvData)
{
    uint64 guid = _player->GetGUID();
    uint64 eventId;
    uint64 inviteId;
    uint32 time;

    recvData >> eventId >> inviteId;
    recvData.ReadPackedTime(time);
    SF_LOG_DEBUG("network", "CMSG_CALENDAR_COPY_EVENT [" UI64FMTD "], EventId [" UI64FMTD
        "] inviteId [" UI64FMTD "] Time: %u", guid, eventId, inviteId, time);

    if (CalendarEvent* oldEvent = sCalendarMgr->GetEvent(eventId))
    {
        CalendarEvent* newEvent = new CalendarEvent(*oldEvent, sCalendarMgr->GetFreeEventId(), guid);
        newEvent->SetEventTime(time_t(time));
        CalendarInviteStore invites = sCalendarMgr->GetEventInvites(eventId);

        for (CalendarInviteStore::const_iterator itr = invites.begin(); itr != invites.end(); ++itr)
            sCalendarMgr->AddInvite(newEvent, new CalendarInvite(**itr, sCalendarMgr->GetFreeInviteId(), newEvent->GetEventId(), guid), false);

        sCalendarMgr->AddEvent(newEvent, CALENDAR_SENDTYPE_COPY);
    }
    else
        sCalendarMgr->SendCalendarCommandResult(guid, CALENDAR_ERROR_EVENT_INVALID);
}

void WorldSession::HandleCalendarEventInvite(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "CMSG_CALENDAR_EVENT_INVITE");

    uint64 playerGuid = _player->GetGUID();

    uint64 eventId;
    uint64 inviteId;
    std::string name;
    uint16 length;
    bool isPreInvite;
    bool isGuildEvent;

    uint64 inviteeGuid = 0;
    uint32 inviteeTeam = 0;
    uint32 inviteeGuildId = 0;

    recvData >> eventId >> inviteId;
    isPreInvite = recvData.ReadBit();
    length = recvData.ReadBits(9);
    isGuildEvent = recvData.ReadBit();
    name = recvData.ReadString(length);

    Player* player = sObjectAccessor->FindPlayerByName(name.c_str());
    if (player)
    {
        // Invitee is online
        inviteeGuid = player->GetGUID();
        inviteeTeam = player->GetTeam();
        inviteeGuildId = player->GetGuildId();
    }
    else
    {
        // Invitee offline, get data from database
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUID_RACE_ACC_BY_NAME);
        stmt->setString(0, name);
        if (PreparedQueryResult result = CharacterDatabase.Query(stmt))
        {
            Field* fields = result->Fetch();
            inviteeGuid = MAKE_NEW_GUID(fields[0].GetUInt32(), 0, HIGHGUID_PLAYER);
            inviteeTeam = Player::TeamForRace(fields[1].GetUInt8());
            inviteeGuildId = Player::GetGuildIdFromDB(inviteeGuid);
        }
    }

    if (!inviteeGuid)
    {
        sCalendarMgr->SendCalendarCommandResult(playerGuid, CALENDAR_ERROR_PLAYER_NOT_FOUND);
        return;
    }

    if (_player->GetTeam() != inviteeTeam && !sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_CALENDAR))
    {
        sCalendarMgr->SendCalendarCommandResult(playerGuid, CALENDAR_ERROR_NOT_ALLIED);
        return;
    }

    if (QueryResult result = CharacterDatabase.PQuery("SELECT flags FROM character_social WHERE guid = " UI64FMTD " AND friend = " UI64FMTD, inviteeGuid, playerGuid))
    {
        Field* fields = result->Fetch();
        if (fields[0].GetUInt8() & SOCIAL_FLAG_IGNORED)
        {
            sCalendarMgr->SendCalendarCommandResult(playerGuid, CALENDAR_ERROR_IGNORING_YOU_S, name.c_str());
            return;
        }
    }

    if (!isPreInvite)
    {
        if (CalendarEvent* calendarEvent = sCalendarMgr->GetEvent(eventId))
        {
            if (calendarEvent->GetEventTime() < time(NULL))
            {
                sCalendarMgr->SendCalendarCommandResult(playerGuid, CALENDAR_ERROR_EVENT_PASSED);
                return;
            }

            // we probably can invite them, but we have to send them some different info about that invite
            if (calendarEvent->IsGuildEvent() && calendarEvent->GetGuildId() == inviteeGuildId)
            {
                sCalendarMgr->SendCalendarCommandResult(playerGuid, CALENDAR_ERROR_NO_GUILD_INVITES);
                return;
            }

            CalendarInvite* invite = new CalendarInvite(sCalendarMgr->GetFreeInviteId(), eventId, inviteeGuid, playerGuid, 0, CALENDAR_STATUS_INVITED, CALENDAR_RANK_PLAYER, "");
            sCalendarMgr->AddInvite(calendarEvent, invite, true, player && player->GetGuildId() != calendarEvent->GetGuildId());
        }
        else
            sCalendarMgr->SendCalendarCommandResult(playerGuid, CALENDAR_ERROR_EVENT_INVALID);
    }
    else
    {
        // we probably can invite them, but we have to send them some different info about that invite
        if (isGuildEvent && _player->GetGuildId() == inviteeGuildId)
        {
            sCalendarMgr->SendCalendarCommandResult(playerGuid, CALENDAR_ERROR_NO_GUILD_INVITES);
            return;
        }

        CalendarInvite invite(0, 0, inviteeGuid, playerGuid, 0, CALENDAR_STATUS_INVITED, CALENDAR_RANK_PLAYER, "");
        sCalendarMgr->SendCalendarEventInvite(invite);
    }
}

void WorldSession::HandleCalendarEventSignup(WorldPacket& recvData)
{
    uint64 guid = _player->GetGUID();
    uint64 eventId;
    bool tentative;

    recvData >> eventId;
    tentative = recvData.ReadBit();

    SF_LOG_DEBUG("network", "CMSG_CALENDAR_EVENT_SIGNUP [" UI64FMTD "] EventId [" UI64FMTD "] Tentative %u", guid, eventId, tentative);

    if (CalendarEvent* calendarEvent = sCalendarMgr->GetEvent(eventId))
    {
        if (calendarEvent->IsGuildEvent() && calendarEvent->GetGuildId() != _player->GetGuildId())
        {
            sCalendarMgr->SendCalendarCommandResult(guid, CALENDAR_ERROR_GUILD_PLAYER_NOT_IN_GUILD);
            return;
        }

        CalendarInviteStatus status = tentative ? CALENDAR_STATUS_TENTATIVE : CALENDAR_STATUS_SIGNED_UP;
        CalendarInvite* invite = new CalendarInvite(sCalendarMgr->GetFreeInviteId(), eventId, guid, guid, time(NULL), status, CALENDAR_RANK_PLAYER, "");
        sCalendarMgr->AddInvite(calendarEvent, invite);
        sCalendarMgr->SendCalendarClearPendingAction(guid);
    }
    else
        sCalendarMgr->SendCalendarCommandResult(guid, CALENDAR_ERROR_EVENT_INVALID);
}

void WorldSession::HandleCalendarEventRsvp(WorldPacket& recvData)
{
    uint64 guid = _player->GetGUID();
    uint64 eventId;
    uint64 inviteId;
    uint8 status;

    recvData >> eventId >> inviteId >> status;
    SF_LOG_DEBUG("network", "CMSG_CALENDAR_EVENT_RSVP [" UI64FMTD "] EventId ["
        UI64FMTD "], InviteId [" UI64FMTD "], status %u", guid, eventId,
        inviteId, status);

    if (CalendarEvent* calendarEvent = sCalendarMgr->GetEvent(eventId))
    {
        // i think we still should be able to remove self from locked events
        if (status != CALENDAR_STATUS_REMOVED && calendarEvent->GetFlags() & CALENDAR_FLAG_INVITES_LOCKED)
        {
            sCalendarMgr->SendCalendarCommandResult(guid, CALENDAR_ERROR_EVENT_LOCKED);
            return;
        }

        if (CalendarInvite* invite = sCalendarMgr->GetInvite(inviteId))
        {
            invite->SetStatus(CalendarInviteStatus(status));
            invite->SetStatusTime(time(NULL));

            sCalendarMgr->UpdateInvite(invite);
            sCalendarMgr->SendCalendarEventStatus(*calendarEvent, *invite);
            sCalendarMgr->SendCalendarEventStatusAlert(*calendarEvent, *invite);
            sCalendarMgr->SendCalendarClearPendingAction(guid);
        }
        else
            sCalendarMgr->SendCalendarCommandResult(guid, CALENDAR_ERROR_NO_INVITE); // correct?
    }
    else
        sCalendarMgr->SendCalendarCommandResult(guid, CALENDAR_ERROR_EVENT_INVALID);
}

void WorldSession::HandleCalendarEventRemoveInvite(WorldPacket& recvData)
{
    uint64 guid = _player->GetGUID();
    ObjectGuid invitee;
    uint64 eventId;
    uint64 senderId;
    uint64 inviteId;

    recvData >> inviteId >> senderId >> eventId;

    invitee[6] = recvData.ReadBit();
    invitee[3] = recvData.ReadBit();
    invitee[2] = recvData.ReadBit();
    invitee[4] = recvData.ReadBit();
    invitee[5] = recvData.ReadBit();
    invitee[7] = recvData.ReadBit();
    invitee[0] = recvData.ReadBit();
    invitee[1] = recvData.ReadBit();

    recvData.ReadByteSeq(invitee[0]);
    recvData.ReadByteSeq(invitee[4]);
    recvData.ReadByteSeq(invitee[7]);
    recvData.ReadByteSeq(invitee[3]);
    recvData.ReadByteSeq(invitee[5]);
    recvData.ReadByteSeq(invitee[1]);
    recvData.ReadByteSeq(invitee[2]);
    recvData.ReadByteSeq(invitee[6]);

    SF_LOG_DEBUG("network", "CMSG_CALENDAR_EVENT_REMOVE_INVITE ["
        UI64FMTD "] EventId [" UI64FMTD "], senderId ["
        UI64FMTD "], Invitee ([" UI64FMTD "] id: [" UI64FMTD "])",
        guid, eventId, senderId, (uint64)invitee, inviteId);

    if (CalendarEvent* calendarEvent = sCalendarMgr->GetEvent(eventId))
    {
        if (calendarEvent->GetCreatorGUID() == invitee)
        {
            sCalendarMgr->SendCalendarCommandResult(guid, CALENDAR_ERROR_DELETE_CREATOR_FAILED);
            return;
        }

        sCalendarMgr->RemoveInvite(inviteId, eventId, guid);
    }
    else
        sCalendarMgr->SendCalendarCommandResult(guid, CALENDAR_ERROR_NO_INVITE);
}

void WorldSession::HandleCalendarEventStatus(WorldPacket& recvData)
{
    uint64 guid = _player->GetGUID();
    ObjectGuid invitee;
    uint64 eventId;
    uint64 inviteId;
    uint64 senderId;
    uint8 status;

    recvData >> senderId >> eventId >> inviteId >> status;

    invitee[4] = recvData.ReadBit();
    invitee[3] = recvData.ReadBit();
    invitee[7] = recvData.ReadBit();
    invitee[6] = recvData.ReadBit();
    invitee[2] = recvData.ReadBit();
    invitee[0] = recvData.ReadBit();
    invitee[5] = recvData.ReadBit();
    invitee[1] = recvData.ReadBit();

    recvData.ReadByteSeq(invitee[7]);
    recvData.ReadByteSeq(invitee[5]);
    recvData.ReadByteSeq(invitee[2]);
    recvData.ReadByteSeq(invitee[1]);
    recvData.ReadByteSeq(invitee[4]);
    recvData.ReadByteSeq(invitee[6]);
    recvData.ReadByteSeq(invitee[0]);
    recvData.ReadByteSeq(invitee[3]);

    SF_LOG_DEBUG("network", "CMSG_CALENDAR_EVENT_STATUS [" UI64FMTD"] EventId ["
        UI64FMTD "] senderId [" UI64FMTD "], Invitee ([" UI64FMTD "] id: ["
        UI64FMTD "], status %u", guid, eventId, senderId, (uint64)invitee, inviteId, status);

    if (CalendarEvent* calendarEvent = sCalendarMgr->GetEvent(eventId))
    {
        if (CalendarInvite* invite = sCalendarMgr->GetInvite(inviteId))
        {
            invite->SetStatus((CalendarInviteStatus)status);

            sCalendarMgr->UpdateInvite(invite);
            sCalendarMgr->SendCalendarEventStatus(*calendarEvent, *invite);
            sCalendarMgr->SendCalendarEventStatusAlert(*calendarEvent, *invite);
            sCalendarMgr->SendCalendarClearPendingAction(invitee);
        }
        else
            sCalendarMgr->SendCalendarCommandResult(guid, CALENDAR_ERROR_NO_INVITE); // correct?
    }
    else
        sCalendarMgr->SendCalendarCommandResult(guid, CALENDAR_ERROR_EVENT_INVALID);
}

void WorldSession::HandleCalendarEventModeratorStatus(WorldPacket& recvData)
{
    uint64 guid = _player->GetGUID();
    ObjectGuid invitee;
    uint64 eventId;
    uint64 inviteId;
    uint64 senderId;
    uint8 rank;

    recvData >> rank >> eventId >> senderId >> inviteId;

    invitee[6] = recvData.ReadBit();
    invitee[5] = recvData.ReadBit();
    invitee[1] = recvData.ReadBit();
    invitee[3] = recvData.ReadBit();
    invitee[4] = recvData.ReadBit();
    invitee[7] = recvData.ReadBit();
    invitee[0] = recvData.ReadBit();
    invitee[2] = recvData.ReadBit();

    recvData.ReadByteSeq(invitee[7]);
    recvData.ReadByteSeq(invitee[5]);
    recvData.ReadByteSeq(invitee[0]);
    recvData.ReadByteSeq(invitee[4]);
    recvData.ReadByteSeq(invitee[1]);
    recvData.ReadByteSeq(invitee[3]);
    recvData.ReadByteSeq(invitee[2]);
    recvData.ReadByteSeq(invitee[6]);

    SF_LOG_DEBUG("network", "CMSG_CALENDAR_EVENT_MODERATOR_STATUS [" UI64FMTD "] EventId ["
        UI64FMTD "] senderId [" UI64FMTD "], Invitee ([" UI64FMTD "] id: ["
        UI64FMTD "], rank %u", guid, eventId, senderId, (uint64)invitee, inviteId, rank);

    if (CalendarEvent* calendarEvent = sCalendarMgr->GetEvent(eventId))
    {
        if (calendarEvent->IsGuildEvent())
        {
            sCalendarMgr->SendCalendarCommandResult(guid, CALENDAR_ERROR_NO_MODERATOR);
            return;
        }

        if (CalendarInvite* invite = sCalendarMgr->GetInvite(inviteId))
        {
            invite->SetRank(CalendarModerationRank(rank));
            sCalendarMgr->UpdateInvite(invite);
            sCalendarMgr->SendCalendarEventModeratorStatus(*calendarEvent, *invite);
        }
        else
            sCalendarMgr->SendCalendarCommandResult(guid, CALENDAR_ERROR_NO_INVITE); // correct?
    }
    else
        sCalendarMgr->SendCalendarCommandResult(guid, CALENDAR_ERROR_EVENT_INVALID);
}

void WorldSession::HandleCalendarComplain(WorldPacket& recvData)
{
    uint64 guid = _player->GetGUID();
    uint64 eventId;
    ObjectGuid complainGUID;
    uint64 inviteId;

    recvData >> inviteId >> eventId;
    complainGUID[4] = recvData.ReadBit();
    complainGUID[6] = recvData.ReadBit();
    complainGUID[2] = recvData.ReadBit();
    complainGUID[7] = recvData.ReadBit();
    complainGUID[1] = recvData.ReadBit();
    complainGUID[5] = recvData.ReadBit();
    complainGUID[3] = recvData.ReadBit();
    complainGUID[0] = recvData.ReadBit();

    recvData.ReadByteSeq(complainGUID[6]);
    recvData.ReadByteSeq(complainGUID[7]);
    recvData.ReadByteSeq(complainGUID[1]);
    recvData.ReadByteSeq(complainGUID[0]);
    recvData.ReadByteSeq(complainGUID[4]);
    recvData.ReadByteSeq(complainGUID[2]);
    recvData.ReadByteSeq(complainGUID[3]);
    recvData.ReadByteSeq(complainGUID[5]);

    SF_LOG_DEBUG("network", "CMSG_CALENDAR_COMPLAIN [" UI64FMTD "] EventId ["
        UI64FMTD "] guid [" UI64FMTD "] InviteId [" UI64FMTD "]", guid, eventId, (uint64)complainGUID, inviteId);

    // what to do with complains?
}

void WorldSession::HandleCalendarGetNumPending(WorldPacket& /*recvData*/)
{
    uint64 guid = _player->GetGUID();
    uint32 pending = sCalendarMgr->GetPlayerNumPending(guid);

    SF_LOG_DEBUG("network", "CMSG_CALENDAR_GET_NUM_PENDING: [" UI64FMTD
        "] Pending: %u", guid, pending);

    WorldPacket data(SMSG_CALENDAR_SEND_NUM_PENDING, 4);
    data << uint32(pending);
    SendPacket(&data);
}

void WorldSession::HandleSetSavedInstanceExtend(WorldPacket& recvData)
{
    uint32 mapId, difficulty;
    uint8 toggleExtend;
    recvData >> mapId >> difficulty>> toggleExtend;
    SF_LOG_DEBUG("network", "CMSG_SET_SAVED_INSTANCE_EXTEND - MapId: %u, Difficulty: %u, ToggleExtend: %s", mapId, difficulty, toggleExtend ? "On" : "Off");

    /*
    InstancePlayerBind* instanceBind = _player->GetBoundInstance(mapId, Difficulty(difficulty));
    if (!instanceBind || !instanceBind->save)
        return;

    InstanceSave* save = instanceBind->save;
    // http://www.wowwiki.com/Instance_Lock_Extension
    // SendCalendarRaidLockoutUpdated(save);
    */
}

// ----------------------------------- SEND ------------------------------------

void WorldSession::SendCalendarRaidLockout(InstanceSave const* save, bool add)
{
    SF_LOG_DEBUG("network", "%s", add ? "SMSG_CALENDAR_RAID_LOCKOUT_ADDED" : "SMSG_CALENDAR_RAID_LOCKOUT_REMOVED");
    time_t currTime = time(NULL);

    ObjectGuid guid = save->GetInstanceId();
    if (add)
    {
        WorldPacket data(SMSG_CALENDAR_RAID_LOCKOUT_ADDED, 4 + 4 + 4 + 4 + 8);
        data.WriteBit(guid[3]);
        data.WriteBit(guid[1]);
        data.WriteBit(guid[2]);
        data.WriteBit(guid[0]);
        data.WriteBit(guid[4]);
        data.WriteBit(guid[7]);
        data.WriteBit(guid[5]);
        data.WriteBit(guid[6]);
        data.FlushBits();

        data.WriteByteSeq(guid[6]);
        data << uint32(save->GetDifficulty());
        data.WriteByteSeq(guid[0]);
        data.WriteByteSeq(guid[4]);
        data.WriteByteSeq(guid[5]);
        data.WriteByteSeq(guid[3]);
        data.WriteByteSeq(guid[2]);
        data << uint32(save->GetMapId());
        data.WriteByteSeq(guid[1]);
        data.AppendPackedTime(currTime);
        data << uint32(save->GetResetTime() - currTime);
        data.WriteByteSeq(guid[7]);

        SendPacket(&data);
    }
    else
    {
        WorldPacket data(SMSG_CALENDAR_RAID_LOCKOUT_REMOVED, 4 + 4 + 8);
        data << uint32(save->GetDifficulty());
        data << uint32(save->GetMapId());

        data.WriteBit(guid[2]);
        data.WriteBit(guid[0]);
        data.WriteBit(guid[4]);
        data.WriteBit(guid[6]);
        data.WriteBit(guid[5]);
        data.WriteBit(guid[7]);
        data.WriteBit(guid[3]);
        data.WriteBit(guid[1]);
        data.FlushBits();

        data.WriteByteSeq(guid[6]);
        data.WriteByteSeq(guid[1]);
        data.WriteByteSeq(guid[7]);
        data.WriteByteSeq(guid[3]);
        data.WriteByteSeq(guid[4]);
        data.WriteByteSeq(guid[5]);
        data.WriteByteSeq(guid[0]);
        data.WriteByteSeq(guid[2]);

        SendPacket(&data);
    }
}

void WorldSession::SendCalendarRaidLockoutUpdated(InstanceSave const* save)
{
    if (!save)
        return;

    uint64 guid = _player->GetGUID();
    SF_LOG_DEBUG("network", "SMSG_CALENDAR_RAID_LOCKOUT_UPDATED [" UI64FMTD
        "] Map: %u, Difficulty %u", guid, save->GetMapId(), save->GetDifficulty());

    time_t currTime = time(NULL);

    WorldPacket data(SMSG_CALENDAR_RAID_LOCKOUT_UPDATED, 4 + 4 + 4 + 4 + 8);
    data << uint32(save->GetResetTime() - currTime); // New remaining time? => found only same values as the possible old time
    data << uint32(save->GetMapId());
    data << uint32(save->GetDifficulty());
    data.AppendPackedTime(currTime);
    data << uint32(save->GetResetTime() - currTime); // Old remaining time? => found only same values as the possible new time
    SendPacket(&data);
}
