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

#include "CalendarMgr.h"
#include "QueryResult.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "Player.h"
#include "GuildMgr.h"
#include "ObjectAccessor.h"
#include "Opcodes.h"

CalendarInvite::~CalendarInvite()
{
    if (_inviteId)
        sCalendarMgr->FreeInviteId(_inviteId);
}

CalendarEvent::~CalendarEvent()
{
    sCalendarMgr->FreeEventId(_eventId);
}

CalendarMgr::CalendarMgr() { }

CalendarMgr::~CalendarMgr()
{
    for (CalendarEventStore::iterator itr = _events.begin(); itr != _events.end(); ++itr)
        delete *itr;

    for (CalendarEventInviteStore::iterator itr = _invites.begin(); itr != _invites.end(); ++itr)
        for (CalendarInviteStore::iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
            delete *itr2;
}

void CalendarMgr::LoadFromDB()
{
    uint32 count = 0;
    _maxEventId = 0;
    _maxInviteId = 0;

    //                                                       0   1        2      3            4     5        6          7
    if (QueryResult result = CharacterDatabase.Query("SELECT id, creator, title, description, type, dungeon, eventtime, flags FROM calendar_events"))
        do
        {
            Field* fields = result->Fetch();

            uint64 eventId          = fields[0].GetUInt64();
            uint64 creatorGUID      = MAKE_NEW_GUID(fields[1].GetUInt32(), 0, HIGHGUID_PLAYER);
            std::string title       = fields[2].GetString();
            std::string description = fields[3].GetString();
            CalendarEventType type  = CalendarEventType(fields[4].GetUInt8());
            int32 dungeonId         = fields[5].GetInt32();
            uint32 eventTime        = fields[6].GetUInt32();
            uint32 flags            = fields[7].GetUInt32();
            uint32 guildId = 0;

            if (flags & CALENDAR_FLAG_GUILD_EVENT || flags & CALENDAR_FLAG_WITHOUT_INVITES)
                guildId = Player::GetGuildIdFromDB(creatorGUID);

            CalendarEvent* calendarEvent = new CalendarEvent(eventId, creatorGUID, guildId, type, dungeonId, time_t(eventTime), flags, title, description);
            _events.insert(calendarEvent);

            _maxEventId = std::max(_maxEventId, eventId);

            ++count;
        }
        while (result->NextRow());

    SF_LOG_INFO("server.loading", ">> Loaded %u calendar events", count);
    count = 0;

    //                                                       0   1      2        3       4       5           6     7
    if (QueryResult result = CharacterDatabase.Query("SELECT id, event, invitee, sender, status, statustime, rank, text FROM calendar_invites"))
        do
        {
            Field* fields = result->Fetch();

            uint64 inviteId             = fields[0].GetUInt64();
            uint64 eventId              = fields[1].GetUInt64();
            uint64 invitee              = MAKE_NEW_GUID(fields[2].GetUInt32(), 0, HIGHGUID_PLAYER);
            uint64 senderGUID           = MAKE_NEW_GUID(fields[3].GetUInt32(), 0, HIGHGUID_PLAYER);
            CalendarInviteStatus status = CalendarInviteStatus(fields[4].GetUInt8());
            uint32 statusTime           = fields[5].GetUInt32();
            CalendarModerationRank rank = CalendarModerationRank(fields[6].GetUInt8());
            std::string text            = fields[7].GetString();

            CalendarInvite* invite = new CalendarInvite(inviteId, eventId, invitee, senderGUID, time_t(statusTime), status, rank, text);
            _invites[eventId].push_back(invite);

            _maxInviteId = std::max(_maxInviteId, inviteId);

            ++count;
        }
        while (result->NextRow());

    SF_LOG_INFO("server.loading", ">> Loaded %u calendar invites", count);

    for (uint64 i = 1; i < _maxEventId; ++i)
        if (!GetEvent(i))
            _freeEventIds.push_back(i);

    for (uint64 i = 1; i < _maxInviteId; ++i)
        if (!GetInvite(i))
            _freeInviteIds.push_back(i);
}

void CalendarMgr::AddEvent(CalendarEvent* calendarEvent, CalendarSendEventType sendType)
{
    _events.insert(calendarEvent);
    UpdateEvent(calendarEvent);
    SendCalendarEvent(calendarEvent->GetCreatorGUID(), *calendarEvent, sendType);
}

void CalendarMgr::AddInvite(CalendarEvent* calendarEvent, CalendarInvite* invite, bool sendEventInvitePacket /*= true*/, bool forceEventInviteAlertPacket /*= false*/)
{
    if (sendEventInvitePacket)
        SendCalendarEventInvite(*invite);

    if (!calendarEvent->IsGuildEvent() || forceEventInviteAlertPacket)
        SendCalendarEventInviteAlert(*calendarEvent, *invite, (calendarEvent->IsGuildEvent() && calendarEvent->GetCreatorGUID() == invite->GetInviteeGUID()) || calendarEvent->IsGuildAnnouncement());

    if (!calendarEvent->IsGuildAnnouncement())
    {
        _invites[invite->GetEventId()].push_back(invite);
        UpdateInvite(invite);
    }
}

void CalendarMgr::RemoveEvent(uint64 eventId, uint64 remover)
{
    CalendarEvent* calendarEvent = GetEvent(eventId);

    if (!calendarEvent)
    {
        SendCalendarCommandResult(remover, CALENDAR_ERROR_EVENT_INVALID);
        return;
    }

    SendCalendarEventRemovedAlert(*calendarEvent);

    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    PreparedStatement* stmt;
    MailDraft mail(calendarEvent->BuildCalendarMailSubject(remover), calendarEvent->BuildCalendarMailBody());

    CalendarInviteStore& eventInvites = _invites[eventId];
    for (size_t i = 0; i < eventInvites.size(); ++i)
    {
        CalendarInvite* invite = eventInvites[i];
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CALENDAR_INVITE);
        stmt->setUInt64(0, invite->GetInviteId());
        trans->Append(stmt);

        // guild events only? check invite status here?
        // When an event is deleted, all invited (accepted/declined? - verify) guildies are notified via in-game mail. (wowwiki)
        if (remover && invite->GetInviteeGUID() != remover)
            mail.SendMailTo(trans, MailReceiver(invite->GetInviteeGUID()), calendarEvent, MAIL_CHECK_MASK_COPIED);

        delete invite;
    }

    _invites.erase(eventId);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CALENDAR_EVENT);
    stmt->setUInt64(0, eventId);
    trans->Append(stmt);
    CharacterDatabase.CommitTransaction(trans);

    delete calendarEvent;
    _events.erase(calendarEvent);
}

void CalendarMgr::RemoveInvite(uint64 inviteId, uint64 eventId, uint64 /*remover*/)
{
    CalendarEvent* calendarEvent = GetEvent(eventId);

    if (!calendarEvent)
        return;

    CalendarInviteStore::iterator itr = _invites[eventId].begin();
    for (; itr != _invites[eventId].end(); ++itr)
        if ((*itr)->GetInviteId() == inviteId)
            break;

    if (itr == _invites[eventId].end())
        return;

    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CALENDAR_INVITE);
    stmt->setUInt64(0, (*itr)->GetInviteId());
    trans->Append(stmt);
    CharacterDatabase.CommitTransaction(trans);

    if (!calendarEvent->IsGuildEvent())
        SendCalendarEventInviteRemoveAlert((*itr)->GetInviteeGUID(), *calendarEvent, CALENDAR_STATUS_REMOVED);

    SendCalendarEventInviteRemove(*calendarEvent, **itr, calendarEvent->GetFlags());

    // we need to find out how to use CALENDAR_INVITE_REMOVED_MAIL_SUBJECT to force client to display different mail
    //if ((*itr)->GetInviteeGUID() != remover)
    //    MailDraft(calendarEvent->BuildCalendarMailSubject(remover), calendarEvent->BuildCalendarMailBody())
    //        .SendMailTo(trans, MailReceiver((*itr)->GetInvitee()), calendarEvent, MAIL_CHECK_MASK_COPIED);

    delete *itr;
    _invites[eventId].erase(itr);
}

void CalendarMgr::UpdateEvent(CalendarEvent* calendarEvent)
{
    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_CALENDAR_EVENT);
    stmt->setUInt64(0, calendarEvent->GetEventId());
    stmt->setUInt32(1, GUID_LOPART(calendarEvent->GetCreatorGUID()));
    stmt->setString(2, calendarEvent->GetTitle());
    stmt->setString(3, calendarEvent->GetDescription());
    stmt->setUInt8(4, calendarEvent->GetType());
    stmt->setInt32(5, calendarEvent->GetDungeonId());
    stmt->setUInt32(6, uint32(calendarEvent->GetEventTime()));
    stmt->setUInt32(7, calendarEvent->GetFlags());
    trans->Append(stmt);
    CharacterDatabase.CommitTransaction(trans);
}

void CalendarMgr::UpdateInvite(CalendarInvite* invite)
{
    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_CALENDAR_INVITE);
    stmt->setUInt64(0, invite->GetInviteId());
    stmt->setUInt64(1, invite->GetEventId());
    stmt->setUInt32(2, GUID_LOPART(invite->GetInviteeGUID()));
    stmt->setUInt32(3, GUID_LOPART(invite->GetSenderGUID()));
    stmt->setUInt8(4, invite->GetStatus());
    stmt->setUInt32(5, uint32(invite->GetStatusTime()));
    stmt->setUInt8(6, invite->GetRank());
    stmt->setString(7, invite->GetText());
    trans->Append(stmt);
    CharacterDatabase.CommitTransaction(trans);
}

void CalendarMgr::RemoveAllPlayerEventsAndInvites(uint64 guid)
{
    for (CalendarEventStore::const_iterator itr = _events.begin(); itr != _events.end(); ++itr)
        if ((*itr)->GetCreatorGUID() == guid)
            RemoveEvent((*itr)->GetEventId(), 0); // don't send mail if removing a character

    CalendarInviteStore playerInvites = GetPlayerInvites(guid);
    for (CalendarInviteStore::const_iterator itr = playerInvites.begin(); itr != playerInvites.end(); ++itr)
        RemoveInvite((*itr)->GetInviteId(), (*itr)->GetEventId(), guid);
}

void CalendarMgr::RemovePlayerGuildEventsAndSignups(uint64 guid, uint32 guildId)
{
    for (CalendarEventStore::const_iterator itr = _events.begin(); itr != _events.end(); ++itr)
        if ((*itr)->GetCreatorGUID() == guid && ((*itr)->IsGuildEvent() || (*itr)->IsGuildAnnouncement()))
            RemoveEvent((*itr)->GetEventId(), guid);

    CalendarInviteStore playerInvites = GetPlayerInvites(guid);
    for (CalendarInviteStore::const_iterator itr = playerInvites.begin(); itr != playerInvites.end(); ++itr)
        if (CalendarEvent* calendarEvent = GetEvent((*itr)->GetEventId()))
            if (calendarEvent->IsGuildEvent() && calendarEvent->GetGuildId() == guildId)
                RemoveInvite((*itr)->GetInviteId(), (*itr)->GetEventId(), guid);
}

CalendarEvent* CalendarMgr::GetEvent(uint64 eventId) const
{
    for (CalendarEventStore::const_iterator itr = _events.begin(); itr != _events.end(); ++itr)
        if ((*itr)->GetEventId() == eventId)
            return *itr;

    SF_LOG_DEBUG("calendar", "CalendarMgr::GetEvent: [" UI64FMTD "] not found!", eventId);
    return NULL;
}

CalendarInvite* CalendarMgr::GetInvite(uint64 inviteId) const
{
    for (CalendarEventInviteStore::const_iterator itr = _invites.begin(); itr != _invites.end(); ++itr)
        for (CalendarInviteStore::const_iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
            if ((*itr2)->GetInviteId() == inviteId)
                return *itr2;

    SF_LOG_DEBUG("calendar", "CalendarMgr::GetInvite: [" UI64FMTD "] not found!", inviteId);
    return NULL;
}

void CalendarMgr::FreeEventId(uint64 id)
{
    if (id == _maxEventId)
        --_maxEventId;
    else
        _freeEventIds.push_back(id);
}

uint64 CalendarMgr::GetFreeEventId()
{
    if (_freeEventIds.empty())
        return ++_maxEventId;

    uint64 eventId = _freeEventIds.front();
    _freeEventIds.pop_front();
    return eventId;
}

void CalendarMgr::FreeInviteId(uint64 id)
{
    if (id == _maxInviteId)
        --_maxInviteId;
    else
        _freeInviteIds.push_back(id);
}

uint64 CalendarMgr::GetFreeInviteId()
{
    if (_freeInviteIds.empty())
        return ++_maxInviteId;

    uint64 inviteId = _freeInviteIds.front();
    _freeInviteIds.pop_front();
    return inviteId;
}

CalendarEventStore CalendarMgr::GetPlayerEvents(uint64 guid)
{
    CalendarEventStore events;

    for (CalendarEventInviteStore::const_iterator itr = _invites.begin(); itr != _invites.end(); ++itr)
        for (CalendarInviteStore::const_iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
            if ((*itr2)->GetInviteeGUID() == guid)
                events.insert(GetEvent(itr->first));

    if (Player* player = ObjectAccessor::FindPlayer(guid))
        for (CalendarEventStore::const_iterator itr = _events.begin(); itr != _events.end(); ++itr)
            if ((*itr)->GetGuildId() == player->GetGuildId())
                events.insert(*itr);

    return events;
}

CalendarInviteStore const& CalendarMgr::GetEventInvites(uint64 eventId)
{
    return _invites[eventId];
}

CalendarInviteStore CalendarMgr::GetPlayerInvites(uint64 guid)
{
    CalendarInviteStore invites;

    for (CalendarEventInviteStore::const_iterator itr = _invites.begin(); itr != _invites.end(); ++itr)
        for (CalendarInviteStore::const_iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
            if ((*itr2)->GetInviteeGUID() == guid)
                invites.push_back(*itr2);

    return invites;
}

uint32 CalendarMgr::GetPlayerNumPending(uint64 guid)
{
    CalendarInviteStore const& invites = GetPlayerInvites(guid);

    uint32 pendingNum = 0;
    for (CalendarInviteStore::const_iterator itr = invites.begin(); itr != invites.end(); ++itr)
    {
        switch ((*itr)->GetStatus())
        {
            case CALENDAR_STATUS_INVITED:
            case CALENDAR_STATUS_TENTATIVE:
            case CALENDAR_STATUS_NOT_SIGNED_UP:
                ++pendingNum;
                break;
            default:
                break;
        }
    }

    return pendingNum;
}

std::string CalendarEvent::BuildCalendarMailSubject(uint64 remover) const
{
    std::ostringstream strm;
    strm << remover << ':' << _title;
    return strm.str();
}

std::string CalendarEvent::BuildCalendarMailBody() const
{
    WorldPacket data;
    uint32 time;
    std::ostringstream strm;

    // we are supposed to send PackedTime so i used WorldPacket to pack it
    data.AppendPackedTime(_eventTime);
    data >> time;
    strm << time;
    return strm.str();
}

void CalendarMgr::SendCalendarEventInvite(CalendarInvite const& invite)
{
    CalendarEvent* calendarEvent = GetEvent(invite.GetEventId());
    time_t statusTime = invite.GetStatusTime();

    ObjectGuid invitee = invite.GetInviteeGUID();
    Player* player = ObjectAccessor::FindPlayer(invitee);

    uint8 level = player ? player->getLevel() : Player::GetLevelFromDB(invitee);

    WorldPacket data(SMSG_CALENDAR_EVENT_INVITE, 8 + 8 + 8 + 1 + 1 + 1 + (statusTime ? 4 : 0) + 1);
    data << uint8(invite.GetSenderGUID() == invite.GetInviteeGUID()); // true only if the invite is sign-up
    data << uint8(invite.GetStatus());
    data << uint64(invite.GetInviteId());
    data << uint8(level);
    data << uint64(invite.GetEventId());

    data.WriteBit(invitee[6]);
    data.WriteBit(invitee[4]);
    data.WriteBit(invitee[1]);
    data.WriteBit(invitee[3]);
    data.WriteBit(invitee[7]);
    data.WriteBit(invitee[0]);
    data.WriteBit(invitee[2]);
    data.WriteBit(invitee[5]);
    data.WriteBit(!statusTime);
    data.WriteBit(1);   // FIXME: Clear pendings
    data.FlushBits();

    data.WriteByteSeq(invitee[7]);
    data.WriteByteSeq(invitee[0]);
    data.WriteByteSeq(invitee[5]);
    if (statusTime)
        data.AppendPackedTime(statusTime);
    data.WriteByteSeq(invitee[2]);
    data.WriteByteSeq(invitee[3]);
    data.WriteByteSeq(invitee[4]);
    data.WriteByteSeq(invitee[1]);
    data.WriteByteSeq(invitee[6]);

    if (!calendarEvent) // Pre-invite
    {
        if (Player* player = ObjectAccessor::FindPlayer(invite.GetSenderGUID()))
            player->SendDirectMessage(&data);
    }
    else
    {
        if (calendarEvent->GetCreatorGUID() != invite.GetInviteeGUID()) // correct?
            SendPacketToAllEventRelatives(data, *calendarEvent);
    }
}

void CalendarMgr::SendCalendarEventUpdateAlert(CalendarEvent const& calendarEvent, time_t oldEventTime)
{
    WorldPacket data(SMSG_CALENDAR_EVENT_UPDATED_ALERT, 4 + 1 + 4 + 4 + 8 + 4 + 4 + 3 +
        calendarEvent.GetTitle().size() + calendarEvent.GetDescription().size());

    data << uint32(calendarEvent.GetFlags());
    data << uint8(calendarEvent.GetType());
    data << uint32(0);      // Lock Date?
    data.AppendPackedTime(oldEventTime);
    data << uint64(calendarEvent.GetEventId());
    data.AppendPackedTime(calendarEvent.GetEventTime());
    data << int32(calendarEvent.GetDungeonId());
    data.WriteBit(1);       // FIXME: Clear pendings
    data.WriteBits(calendarEvent.GetDescription().size(), 11);
    data.WriteBits(calendarEvent.GetTitle().size(), 8);
    data.FlushBits();

    data.WriteString(calendarEvent.GetTitle());
    data.WriteString(calendarEvent.GetDescription());

    SendPacketToAllEventRelatives(data, calendarEvent);
}

void CalendarMgr::SendCalendarEventStatus(CalendarEvent const& calendarEvent, CalendarInvite const& invite)
{
    ObjectGuid guid = invite.GetInviteeGUID();
    WorldPacket data(SMSG_CALENDAR_EVENT_INVITE_STATUS, 8 + 8 + 4 + 4 + 1 + 4);
    data.WriteBit(guid[5]);
    data.WriteBit(guid[0]);
    data.WriteBit(1);       // FIXME: Clear pendings
    data.WriteBit(guid[2]);
    data.WriteBit(guid[1]);
    data.WriteBit(guid[4]);
    data.WriteBit(guid[6]);
    data.WriteBit(guid[7]);
    data.WriteBit(guid[3]);
    data.FlushBits();

    data.WriteByteSeq(guid[1]);
    data.WriteByteSeq(guid[6]);
    data.WriteByteSeq(guid[7]);
    if (invite.GetStatusTime())
        data.AppendPackedTime(invite.GetStatusTime());
    else
        data << uint32(0);
    data.WriteByteSeq(guid[2]);
    data.WriteByteSeq(guid[4]);
    data << uint64(calendarEvent.GetEventId());
    data.WriteByteSeq(guid[0]);
    data.WriteByteSeq(guid[3]);
    data.WriteByteSeq(guid[5]);
    data << uint8(invite.GetStatus());
    data << uint32(calendarEvent.GetFlags());
    data.AppendPackedTime(calendarEvent.GetEventTime());

    SendPacketToAllEventRelatives(data, calendarEvent);
}

void CalendarMgr::SendCalendarEventStatusAlert(CalendarEvent const& calendarEvent, CalendarInvite const& invite)
{
    if (Player* player = ObjectAccessor::FindPlayer(invite.GetInviteeGUID()))
    {
        WorldPacket data(SMSG_CALENDAR_EVENT_INVITE_STATUS_ALERT, 8 + 4 + 4 + 1);
        data << uint64(calendarEvent.GetEventId());
        data << uint32(calendarEvent.GetFlags());
        data << uint8(invite.GetStatus());
        data.AppendPackedTime(calendarEvent.GetEventTime());

        player->SendDirectMessage(&data);
    }
}

void CalendarMgr::SendCalendarEventRemovedAlert(CalendarEvent const& calendarEvent)
{
    WorldPacket data(SMSG_CALENDAR_EVENT_REMOVED_ALERT, 1 + 8 + 1);
    data << uint64(calendarEvent.GetEventId());
    data.AppendPackedTime(calendarEvent.GetEventTime());
    data.WriteBit(1); // FIXME: Clear pendings
    data.FlushBits();

    SendPacketToAllEventRelatives(data, calendarEvent);
}

void CalendarMgr::SendCalendarEventInviteRemove(CalendarEvent const& calendarEvent, CalendarInvite const& invite, uint32 flags)
{
    ObjectGuid guid = invite.GetInviteeGUID();
    WorldPacket data(SMSG_CALENDAR_EVENT_INVITE_REMOVED, 8 + 4 + 4 + 1);
    data.WriteBit(guid[6]);
    data.WriteBit(guid[7]);
    data.WriteBit(guid[3]);
    data.WriteBit(guid[0]);
    data.WriteBit(guid[2]);
    data.WriteBit(guid[4]);
    data.WriteBit(guid[1]);
    data.WriteBit(guid[5]);
    data.WriteBit(1); // FIXME: Clear pendings
    data.FlushBits();

    data.WriteByteSeq(guid[0]);
    data.WriteByteSeq(guid[4]);
    data.WriteByteSeq(guid[3]);
    data.WriteByteSeq(guid[5]);
    data << uint64(invite.GetEventId());
    data.WriteByteSeq(guid[7]);
    data.WriteByteSeq(guid[1]);
    data.WriteByteSeq(guid[2]);
    data << uint32(flags);
    data.WriteByteSeq(guid[6]);

    SendPacketToAllEventRelatives(data, calendarEvent);
}

void CalendarMgr::SendCalendarEventModeratorStatus(CalendarEvent const& calendarEvent, CalendarInvite const& invite)
{
    ObjectGuid guid = invite.GetInviteeGUID();
    WorldPacket data(SMSG_CALENDAR_EVENT_MODERATOR_STATUS, 8 + 8 + 1 + 1);
    data.WriteBit(guid[3]);
    data.WriteBit(guid[7]);
    data.WriteBit(guid[2]);
    data.WriteBit(guid[4]);
    data.WriteBit(guid[1]);
    data.WriteBit(guid[6]);
    data.WriteBit(guid[5]);
    data.WriteBit(1); // FIXME: Clear pendings
    data.WriteBit(guid[0]);
    data.FlushBits();

    data.WriteByteSeq(guid[5]);
    data.WriteByteSeq(guid[4]);
    data.WriteByteSeq(guid[6]);
    data.WriteByteSeq(guid[1]);
    data.WriteByteSeq(guid[7]);
    data.WriteByteSeq(guid[2]);
    data.WriteByteSeq(guid[3]);
    data << uint8(invite.GetRank());
    data << uint64(invite.GetEventId());
    data.WriteByteSeq(guid[0]);

    SendPacketToAllEventRelatives(data, calendarEvent);
}

void CalendarMgr::SendCalendarEventInviteAlert(CalendarEvent const& calendarEvent, CalendarInvite const& invite, bool broadcast)
{
    Guild* guild = sGuildMgr->GetGuildById(calendarEvent.GetGuildId());
    ObjectGuid guildId = guild ? guild->GetGUID() : 0;
    ObjectGuid guid = calendarEvent.GetCreatorGUID();
    ObjectGuid guid2 = invite.GetSenderGUID();
    WorldPacket data(SMSG_CALENDAR_EVENT_INVITE_ALERT);
    data << uint64(calendarEvent.GetEventId());
    data << int32(calendarEvent.GetDungeonId());
    data << uint8(calendarEvent.GetType());
    data << uint64(invite.GetInviteId());
    data << uint32(calendarEvent.GetFlags());
    data << uint8(invite.GetStatus());
    data.AppendPackedTime(calendarEvent.GetEventTime());
    data << uint8(invite.GetRank());

    data.WriteBit(guildId[7]);
    data.WriteBit(guid[6]);
    data.WriteBit(guildId[4]);
    data.WriteBit(guildId[0]);
    data.WriteBit(guid[3]);
    data.WriteBit(guid2[1]);
    data.WriteBit(guildId[5]);
    data.WriteBit(guid[2]);
    data.WriteBit(guid[0]);
    data.WriteBit(guildId[1]);
    data.WriteBit(guid[5]);
    data.WriteBit(guildId[6]);
    data.WriteBit(guildId[3]);
    data.WriteBit(guid2[6]);
    data.WriteBit(guid2[2]);
    data.WriteBit(guid[4]);
    data.WriteBit(guid[7]);
    data.WriteBit(guid2[4]);
    data.WriteBit(guid[1]);
    data.WriteBit(guid2[3]);
    data.WriteBit(guildId[2]);
    data.WriteBit(guid2[0]);
    data.WriteBits(calendarEvent.GetTitle().size(), 8);
    data.WriteBit(guid2[5]);
    data.WriteBit(guid2[7]);
    data.FlushBits();

    data.WriteByteSeq(guid[5]);
    data.WriteByteSeq(guildId[6]);
    data.WriteByteSeq(guildId[0]);
    data.WriteByteSeq(guid[6]);
    data.WriteByteSeq(guid2[5]);
    data.WriteByteSeq(guid2[4]);
    data.WriteString(calendarEvent.GetTitle());
    data.WriteByteSeq(guid[0]);
    data.WriteByteSeq(guid[1]);
    data.WriteByteSeq(guildId[2]);
    data.WriteByteSeq(guildId[7]);
    data.WriteByteSeq(guid2[6]);
    data.WriteByteSeq(guildId[1]);
    data.WriteByteSeq(guid[2]);
    data.WriteByteSeq(guid2[3]);
    data.WriteByteSeq(guid2[7]);
    data.WriteByteSeq(guid2[0]);
    data.WriteByteSeq(guid[3]);
    data.WriteByteSeq(guid2[2]);
    data.WriteByteSeq(guid[7]);
    data.WriteByteSeq(guildId[5]);
    data.WriteByteSeq(guildId[4]);
    data.WriteByteSeq(guildId[3]);
    data.WriteByteSeq(guid2[1]);
    data.WriteByteSeq(guid[4]);

    if (broadcast)
    {
        if (guild)
            guild->BroadcastPacket(&data);
    } else if (Player* player = ObjectAccessor::FindPlayer(invite.GetInviteeGUID()))
        player->SendDirectMessage(&data);
}

void CalendarMgr::SendCalendarEvent(uint64 playerGuid, CalendarEvent const& calendarEvent, CalendarSendEventType sendType)
{
    Player* player = ObjectAccessor::FindPlayer(playerGuid);
    if (!player)
        return;

    CalendarInviteStore const& eventInviteeList = _invites[calendarEvent.GetEventId()];

    Guild* guild = sGuildMgr->GetGuildById(calendarEvent.GetGuildId());
    ObjectGuid creatorGuid = calendarEvent.GetCreatorGUID();
    ObjectGuid guildGuid = guild ? guild->GetGUID() : 0;
    ByteBuffer inviteeData;
    WorldPacket data(SMSG_CALENDAR_SEND_EVENT, 60 + eventInviteeList.size() * 32);
    data.WriteBits(eventInviteeList.size(), 20);

    for (CalendarInviteStore::const_iterator iter = eventInviteeList.begin(); iter != eventInviteeList.end(); ++iter)
    {
        CalendarInvite const* invitee = (*iter);
        ObjectGuid guid = invitee->GetInviteeGUID();
        Player* player = ObjectAccessor::FindPlayer(invitee->GetInviteeGUID());
        uint8 inviteeLevel = player ? player->getLevel() : Player::GetLevelFromDB(invitee->GetInviteeGUID());
        uint32 inviteeGuildId = player ? player->GetGuildId() : Player::GetGuildIdFromDB(invitee->GetInviteeGUID());

        data.WriteBit(guid[1]);
        data.WriteBit(guid[2]);
        data.WriteBit(guid[0]);
        data.WriteBit(guid[7]);
        data.WriteBit(guid[3]);
        data.WriteBit(guid[5]);
        data.WriteBit(guid[6]);
        data.WriteBits(invitee->GetText().size(), 8);
        data.WriteBit(guid[4]);

        if (invitee->GetStatusTime())
            inviteeData.AppendPackedTime(invitee->GetStatusTime());
        else
            inviteeData << uint32(0);
        inviteeData.WriteByteSeq(guid[5]);
        inviteeData << uint8(calendarEvent.IsGuildEvent() && calendarEvent.GetGuildId() == inviteeGuildId ? 1 : 0);
        inviteeData.WriteByteSeq(guid[1]);
        inviteeData.WriteByteSeq(guid[2]);
        inviteeData.WriteByteSeq(guid[6]);
        inviteeData.WriteString(invitee->GetText());
        inviteeData << uint8(inviteeLevel);
        inviteeData.WriteByteSeq(guid[7]);
        inviteeData << uint8(invitee->GetRank());
        inviteeData << uint64(invitee->GetInviteId());
        inviteeData.WriteByteSeq(guid[0]);
        inviteeData.WriteByteSeq(guid[3]);
        inviteeData.WriteByteSeq(guid[4]);
        inviteeData << uint8(invitee->GetStatus());
    }

    data.WriteBits(calendarEvent.GetTitle().size(), 8);
    data.WriteBit(creatorGuid[2]);
    data.WriteBit(creatorGuid[0]);
    data.WriteBit(guildGuid[4]);
    data.WriteBit(guildGuid[5]);
    data.WriteBit(creatorGuid[1]);
    data.WriteBit(creatorGuid[5]);
    data.WriteBit(creatorGuid[3]);
    data.WriteBit(guildGuid[6]);
    data.WriteBits(calendarEvent.GetDescription().size(), 11);
    data.WriteBit(guildGuid[1]);
    data.WriteBit(guildGuid[7]);
    data.WriteBit(creatorGuid[6]);
    data.WriteBit(guildGuid[2]);
    data.WriteBit(guildGuid[0]);
    data.WriteBit(creatorGuid[4]);
    data.WriteBit(guildGuid[3]);
    data.WriteBit(creatorGuid[7]);
    data.FlushBits();

    data.append(inviteeData);
    data.WriteByteSeq(creatorGuid[0]);
    data.WriteByteSeq(creatorGuid[5]);
    data << uint32(calendarEvent.GetFlags());
    data.WriteByteSeq(guildGuid[1]);
    data.WriteByteSeq(creatorGuid[7]);
    data.WriteByteSeq(creatorGuid[3]);
    data.AppendPackedTime(calendarEvent.GetEventTime());
    data.WriteByteSeq(guildGuid[6]);
    data.WriteByteSeq(creatorGuid[4]);
    data << int32(calendarEvent.GetDungeonId());
    data << uint32(0); // Lock Date? Always 0
    data.WriteByteSeq(guildGuid[4]);
    data << uint8(calendarEvent.GetType());
    data.WriteString(calendarEvent.GetTitle());
    data.WriteByteSeq(creatorGuid[6]);
    data.WriteByteSeq(guildGuid[3]);
    data.WriteByteSeq(creatorGuid[2]);
    data.WriteByteSeq(guildGuid[7]);
    data.WriteByteSeq(creatorGuid[1]);
    data.WriteString(calendarEvent.GetDescription());
    data.WriteByteSeq(guildGuid[2]);
    data.WriteByteSeq(guildGuid[5]);
    data.WriteByteSeq(guildGuid[0]);
    data << uint64(calendarEvent.GetEventId());
    data << uint8(sendType);

    player->SendDirectMessage(&data);
}

void CalendarMgr::SendCalendarEventInviteRemoveAlert(uint64 guid, CalendarEvent const& calendarEvent, CalendarInviteStatus status)
{
    if (Player* player = ObjectAccessor::FindPlayer(guid))
    {
        WorldPacket data(SMSG_CALENDAR_EVENT_INVITE_REMOVED_ALERT, 8 + 4 + 4 + 1);
        data << uint32(calendarEvent.GetFlags());
        data << uint8(status);
        data << uint64(calendarEvent.GetEventId());
        data.AppendPackedTime(calendarEvent.GetEventTime());

        player->SendDirectMessage(&data);
    }
}

void CalendarMgr::SendCalendarClearPendingAction(uint64 guid)
{
    if (Player* player = ObjectAccessor::FindPlayer(guid))
    {
        WorldPacket data(SMSG_CALENDAR_CLEAR_PENDING_ACTION, 0);
        player->SendDirectMessage(&data);
    }
}

void CalendarMgr::SendCalendarCommandResult(uint64 guid, CalendarError err, char const* param /*= NULL*/)
{
    if (Player* player = ObjectAccessor::FindPlayer(guid))
    {
        size_t length = param ? std::strlen(param) : 0;
        WorldPacket data(SMSG_CALENDAR_COMMAND_RESULT, 2 + 1 + 1 + length);
        data.WriteBits(length / 2, 8);
        data.WriteBit(length % 2);
        data.FlushBits();

        data << uint8(0);   // FIXME: Command
        data << uint8(err);
        switch (err)
        {
        case CALENDAR_ERROR_OTHER_INVITES_EXCEEDED:
        case CALENDAR_ERROR_ALREADY_INVITED_TO_EVENT_S:
        case CALENDAR_ERROR_IGNORING_YOU_S:
            data.WriteString(param ? param : "");
            break;
        default:
            break;
        }

        player->SendDirectMessage(&data);
    }
}

void CalendarMgr::SendPacketToAllEventRelatives(WorldPacket packet, CalendarEvent const& calendarEvent)
{
    // Send packet to all guild members
    if (calendarEvent.IsGuildEvent() || calendarEvent.IsGuildAnnouncement())
        if (Guild* guild = sGuildMgr->GetGuildById(calendarEvent.GetGuildId()))
            guild->BroadcastPacket(&packet);

    // Send packet to all invitees if event is non-guild, in other case only to non-guild invitees (packet was broadcasted for them)
    CalendarInviteStore invites = _invites[calendarEvent.GetEventId()];
    for (CalendarInviteStore::iterator itr = invites.begin(); itr != invites.end(); ++itr)
        if (Player* player = ObjectAccessor::FindPlayer((*itr)->GetInviteeGUID()))
            if (!calendarEvent.IsGuildEvent() || (calendarEvent.IsGuildEvent() && player->GetGuildId() != calendarEvent.GetGuildId()))
                player->SendDirectMessage(&packet);
}
