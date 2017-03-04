
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

#include "Common.h"
#include "TicketInfo.h"
#include "TicketMgr.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "Language.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Chat.h"
#include "World.h"
#include "Player.h"
#include "Opcodes.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ TicketInfo
// Stores all common data needed for a Ticket : id, mapid, createtime, closedby, assignedTo, Position

TicketInfo::TicketInfo() : _ticketId(0), _mapId(0), _ticketCreateTime(0), _closedBy(0), _assignedTo(0) { }

TicketInfo::TicketInfo(Player* player) : _ticketId(0), _mapId(0), _ticketCreateTime(time(NULL)), _closedBy(0), _assignedTo(0)
{
    _playerGuid = player->GetGUID();
}

TicketInfo::~TicketInfo() { }

void TicketInfo::TeleportTo(Player* player) const
{
   player->TeleportTo(_mapId, _pos.x, _pos.y, _pos.z, 0.0f, 0);
}

std::string TicketInfo::GetAssignedToName() const
{
    std::string name;
    if (!_assignedTo)
        sObjectMgr->GetPlayerNameByGUID(_assignedTo, name);

    return name;
}

void TicketInfo::SetPosition(uint32 MapID, G3D::Vector3 pos)
{
    _mapId = MapID;
    _pos = pos;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// GM ticket

GmTicket::GmTicket() : _lastModifiedTime(0), _completed(false), _escalatedStatus(TICKET_UNASSIGNED),
_openedByGmStatus(GMTICKET_OPENEDBYGM_STATUS_NOT_OPENED), _needResponse(false){ }

GmTicket::GmTicket(Player* player) : TicketInfo(player), _lastModifiedTime(time(NULL)),
_completed(false), _escalatedStatus(TICKET_UNASSIGNED),_openedByGmStatus(GMTICKET_OPENEDBYGM_STATUS_NOT_OPENED), 
_needResponse(false)
{
    _ticketId = sTicketMgr->GenerateGmTicketId();
    _playerName = player->GetName();
}

GmTicket::~GmTicket() { }

void GmTicket::SetUnassigned()
{
    if (_escalatedStatus != TICKET_IN_ESCALATION_QUEUE)
        _escalatedStatus = TICKET_UNASSIGNED;

    _assignedTo = 0;
}

void GmTicket::SetChatLog(std::list<uint32> time, std::string const& log)
{
    std::stringstream ss(log);
    std::stringstream newss;
    std::string line;
    while (std::getline(ss, line) && !time.empty())
    {
        newss << secsToTimeString(time.front()) << ": " << line << "\n";
        time.pop_front();
    }

    _chatLog = newss.str();
}

void GmTicket::SetMessage(std::string const& message)
{
    _message = message;
    _lastModifiedTime = uint64(time(NULL));
}

void GmTicket::SetGmAction(bool needResponse, bool haveTicket)
{
    _needResponse = needResponse;
    _haveTicket = haveTicket;
}

void GmTicket::LoadFromDB(Field* fields)
{
    uint8 index = 0;
    _ticketId = fields[index].GetUInt32();
    _playerGuid = MAKE_NEW_GUID(fields[++index].GetUInt64(), 0, HIGHGUID_PLAYER);
    _playerName = fields[++index].GetString();
    _message = fields[++index].GetString();
    _ticketCreateTime = fields[++index].GetUInt32();
    _mapId = fields[++index].GetUInt16();
    _pos.x = fields[++index].GetFloat();
    _pos.y = fields[++index].GetFloat();
    _pos.z = fields[++index].GetFloat();
    _lastModifiedTime = fields[++index].GetUInt32();

    int64 closedBy = fields[++index].GetInt64();
    int64 assignedTo = fields[++index].GetUInt64();

    _closedBy = closedBy < 0 ? 0 : MAKE_NEW_GUID(uint64(closedBy), 0, HIGHGUID_PLAYER);
    _assignedTo = assignedTo < 0 ? 0 : MAKE_NEW_GUID(assignedTo, 0, HIGHGUID_PLAYER);

    _comment = fields[++index].GetString();
    _response = fields[++index].GetString();
    _completed = fields[++index].GetBool();
    _escalatedStatus = GMTicketEscalationStatus(fields[++index].GetUInt8());
    _viewed = GMTicketOpenedByGMStatus(fields[++index].GetUInt8());
    _haveTicket = fields[++index].GetBool();
}

void GmTicket::SaveToDB(SQLTransaction& trans) const
{
    uint8 index = 0;
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_GM_TICKET);
    stmt->setUInt32(index, _ticketId);
    stmt->setUInt64(++index, GUID_LOPART(_playerGuid));
    stmt->setString(++index, _playerName);
    stmt->setString(++index, _message);
    stmt->setUInt32(++index, _ticketCreateTime);
    stmt->setUInt16(++index, _mapId);
    stmt->setFloat(++index, _pos.x);
    stmt->setFloat(++index, _pos.y);
    stmt->setFloat(++index, _pos.z);
    stmt->setUInt32(++index, uint32(_lastModifiedTime));
    stmt->setInt64(++index, GUID_LOPART(_closedBy));
    stmt->setUInt64(++index, GUID_LOPART(_assignedTo));
    stmt->setString(++index, _comment);
    stmt->setString(++index, _response);
    stmt->setBool(++index, _completed);
    stmt->setUInt8(++index, uint8(_escalatedStatus));
    stmt->setBool(++index, _viewed);
    stmt->setBool(++index, _haveTicket);

    CharacterDatabase.ExecuteOrAppend(trans, stmt);
}

void GmTicket::DeleteFromDB()
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GM_TICKET);
    stmt->setUInt32(0, _ticketId);
    CharacterDatabase.Execute(stmt);
}

std::string TicketInfo::FormatMessageString(ChatHandler& handler, const char* szClosedName, const char* szAssignedToName, const char* szUnassignedName, const char* szDeletedName) const
{
    std::stringstream ss;
    ss << handler.PGetParseString(LANG_COMMAND_TICKETLISTGUID, _ticketId);
    ss << handler.PGetParseString(LANG_COMMAND_TICKETLISTNAME, _playerName.c_str());
    if (szClosedName)
        ss << handler.PGetParseString(LANG_COMMAND_TICKETCLOSED, szClosedName);
    if (szAssignedToName)
        ss << handler.PGetParseString(LANG_COMMAND_TICKETLISTASSIGNEDTO, szAssignedToName);
    if (szUnassignedName)
        ss << handler.PGetParseString(LANG_COMMAND_TICKETLISTUNASSIGNED, szUnassignedName);
    if (szDeletedName)
        ss << handler.PGetParseString(LANG_COMMAND_TICKETDELETED, szDeletedName);
    return ss.str();
}

std::string GmTicket::FormatMessageString(ChatHandler& handler, bool detailed) const
{
    time_t curTime = time(NULL);
    Player* player = GetPlayer();

    std::stringstream ss;
    ss << handler.PGetParseString(LANG_COMMAND_TICKETLISTGUID, _ticketId);
    ss << handler.PGetParseString(LANG_COMMAND_TICKETLISTNAME, player ? player->GetName().c_str() : "NULL");
    ss << handler.PGetParseString(LANG_COMMAND_TICKETLISTAGECREATE, (secsToTimeString(curTime - _ticketCreateTime, true, false)).c_str());
    ss << handler.PGetParseString(LANG_COMMAND_TICKETLISTAGE, (secsToTimeString(curTime - _lastModifiedTime, true, false)).c_str());

    std::string name;
    if (sObjectMgr->GetPlayerNameByGUID(_assignedTo, name))
        ss << handler.PGetParseString(LANG_COMMAND_TICKETLISTASSIGNEDTO, name.c_str());

    if (detailed)
    {
        ss << handler.PGetParseString(LANG_COMMAND_TICKETLISTMESSAGE, _message.c_str());
        if (!_comment.empty())
            ss << handler.PGetParseString(LANG_COMMAND_TICKETLISTCOMMENT, _comment.c_str());
        if (!_response.empty())
            ss << handler.PGetParseString(LANG_COMMAND_TICKETLISTRESPONSE, _response.c_str());
    }
    return ss.str();
}

std::string GmTicket::FormatMessageString(ChatHandler& handler, const char* szClosedName, const char* szAssignedToName, const char* szUnassignedName, const char* szDeletedName) const
{
    std::stringstream ss;
    ss << handler.PGetParseString(LANG_COMMAND_TICKETLISTGUID, _ticketId);
    ss << handler.PGetParseString(LANG_COMMAND_TICKETLISTNAME, _playerName.c_str());
    if (szClosedName)
        ss << handler.PGetParseString(LANG_COMMAND_TICKETCLOSED, szClosedName);
    if (szAssignedToName)
        ss << handler.PGetParseString(LANG_COMMAND_TICKETLISTASSIGNEDTO, szAssignedToName);
    if (szUnassignedName)
        ss << handler.PGetParseString(LANG_COMMAND_TICKETLISTUNASSIGNED, szUnassignedName);
    if (szDeletedName)
        ss << handler.PGetParseString(LANG_COMMAND_TICKETDELETED, szDeletedName);
    return ss.str();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// BugTicket System

BugTicket::BugTicket() : _Orientation(0.0f) { }

BugTicket::BugTicket(Player* player, WorldPacket& bugPacket) : TicketInfo(player), _Orientation(0.0f)
{
    _ticketId = sTicketMgr->GenerateBugId();

    bugPacket >> _pos.x;
    bugPacket >> _pos.y;
    bugPacket >> _pos.z;
    bugPacket >> _Orientation;
    bugPacket >> _mapId;
    uint8 lenNote = bugPacket.ReadBits(10);
    _bugnote = bugPacket.ReadString(lenNote);
}

BugTicket::~BugTicket() { }

void BugTicket::LoadFromDB(Field* fields)
{
    uint8 index = 0;
    _ticketId = fields[index].GetUInt32();
    _playerGuid = MAKE_NEW_GUID(fields[++index].GetUInt64(), 0, HIGHGUID_PLAYER);
    _bugnote = fields[++index].GetString();
    _ticketCreateTime = fields[++index].GetUInt32();
    _mapId = fields[++index].GetUInt32();
    _pos.x = fields[++index].GetFloat();
    _pos.y = fields[++index].GetFloat();
    _pos.z = fields[++index].GetFloat();
    _Orientation = fields[++index].GetFloat();

    int64 closedBy = fields[++index].GetInt64();
    int64 assignedTo = fields[++index].GetUInt64();

    _closedBy = closedBy < 0 ? 0 : MAKE_NEW_GUID(uint64(closedBy), 0, HIGHGUID_PLAYER);
    _assignedTo = assignedTo < 0 ? 0 : MAKE_NEW_GUID(assignedTo, 0, HIGHGUID_PLAYER);

    _comment = fields[++index].GetString();
}

void BugTicket::SaveToDB(SQLTransaction& trans) const
{
    uint8 index = 0;
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_GM_BUG);
    stmt->setUInt32(index, _ticketId);
    stmt->setUInt32(++index, GUID_LOPART(_playerGuid));
    stmt->setString(++index, _bugnote);
    stmt->setUInt32(++index, _mapId);
    stmt->setFloat(++index, _pos.x);
    stmt->setFloat(++index, _pos.y);
    stmt->setFloat(++index, _pos.z);
    stmt->setFloat(++index, _Orientation);
    stmt->setUInt32(++index, GUID_LOPART(_closedBy));
    stmt->setUInt32(++index, GUID_LOPART(_assignedTo));
    stmt->setString(++index, _comment);

    CharacterDatabase.ExecuteOrAppend(trans, stmt);
}

void BugTicket::DeleteFromDB()
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GM_BUG);
    stmt->setUInt32(0, _ticketId);
    CharacterDatabase.Execute(stmt);
}

std::string BugTicket::FormatMessageString(ChatHandler& handler, bool detailed) const
{
    time_t curTime = time(NULL);
    Player* player = GetPlayer();

    std::stringstream ss;
    ss << handler.PGetParseString(LANG_COMMAND_TICKETLISTGUID, _ticketId);
    ss << handler.PGetParseString(LANG_COMMAND_TICKETLISTNAME, player ? player->GetName().c_str() : "NULL");
    ss << handler.PGetParseString(LANG_COMMAND_TICKETLISTAGECREATE, (secsToTimeString(curTime - _ticketCreateTime, true, false)).c_str());

    std::string name;
    if (sObjectMgr->GetPlayerNameByGUID(_assignedTo, name))
        ss << handler.PGetParseString(LANG_COMMAND_TICKETLISTASSIGNEDTO, name.c_str());

    if (detailed)
    {
        ss << handler.PGetParseString(LANG_COMMAND_TICKETLISTMESSAGE, _bugnote.c_str());
        if (!_comment.empty())
            ss << handler.PGetParseString(LANG_COMMAND_TICKETLISTCOMMENT, _comment.c_str());
    }
    return ss.str();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// SuggestTicket System

SuggestTicket::SuggestTicket() : _Orientation(0.0f) { }

SuggestTicket::SuggestTicket(Player* player, WorldPacket& suggestPacket) : TicketInfo(player), _Orientation(0.0f)
{
    _ticketId = sTicketMgr->GenerateSuggestId();

    suggestPacket >> _pos.x;
    suggestPacket >> _pos.y;
    suggestPacket >> _pos.z;
    suggestPacket >> _Orientation;
    suggestPacket >> _mapId;
    uint8 lenNote = suggestPacket.ReadBits(10);
    _suggestnote = suggestPacket.ReadString(lenNote);
}

SuggestTicket::~SuggestTicket() { }

void SuggestTicket::LoadFromDB(Field* fields)
{
    uint8 index = 0;
    _ticketId = fields[index].GetUInt32();
    _playerGuid = MAKE_NEW_GUID(fields[++index].GetUInt64(), 0, HIGHGUID_PLAYER);
    _suggestnote = fields[++index].GetString();
    _ticketCreateTime = fields[++index].GetUInt32();
    _mapId = fields[++index].GetUInt32();
    _pos.x = fields[++index].GetFloat();
    _pos.y = fields[++index].GetFloat();
    _pos.z = fields[++index].GetFloat();
    _Orientation = fields[++index].GetFloat();

    int64 closedBy = fields[++index].GetInt64();
    int64 assignedTo = fields[++index].GetUInt64();

    _closedBy = closedBy < 0 ? 0: MAKE_NEW_GUID(uint64(closedBy), 0, HIGHGUID_PLAYER);
    _assignedTo = assignedTo < 0? 0: MAKE_NEW_GUID(assignedTo, 0, HIGHGUID_PLAYER);
    _comment = fields[++index].GetString();
}

void SuggestTicket::SaveToDB(SQLTransaction& trans) const
{
    uint8 index = 0;
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_GM_SUGGEST);
    stmt->setUInt32(index, _ticketId);
    stmt->setUInt32(++index, GUID_LOPART(_playerGuid));
    stmt->setString(++index, _suggestnote);
    stmt->setUInt32(++index, _mapId);
    stmt->setFloat(++index, _pos.x);
    stmt->setFloat(++index, _pos.y);
    stmt->setFloat(++index, _pos.z);
    stmt->setFloat(++index, _Orientation);
    stmt->setUInt32(++index, GUID_LOPART(_closedBy));
    stmt->setUInt32(++index, GUID_LOPART(_assignedTo));
    stmt->setString(++index, _comment);

    CharacterDatabase.ExecuteOrAppend(trans, stmt);
}

void SuggestTicket::DeleteFromDB()
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GM_SUGGEST);
    stmt->setUInt32(0, _ticketId);
    CharacterDatabase.Execute(stmt);
}

std::string SuggestTicket::FormatMessageString(ChatHandler& handler, bool detailed) const
{
    time_t curTime = time(NULL);
    Player* player = GetPlayer();

    std::stringstream ss;
    ss << handler.PGetParseString(LANG_COMMAND_TICKETLISTGUID, _ticketId);
    ss << handler.PGetParseString(LANG_COMMAND_TICKETLISTNAME, player ? player->GetName().c_str() : "NULL");
    ss << handler.PGetParseString(LANG_COMMAND_TICKETLISTAGECREATE, (secsToTimeString(curTime - _ticketCreateTime, true, false)).c_str());

    std::string name;
    if (sObjectMgr->GetPlayerNameByGUID(_assignedTo, name))
        ss << handler.PGetParseString(LANG_COMMAND_TICKETLISTASSIGNEDTO, name.c_str());

    if (detailed)
    {
        ss << handler.PGetParseString(LANG_COMMAND_TICKETLISTMESSAGE, _suggestnote.c_str());
        if (!_comment.empty())
            ss << handler.PGetParseString(LANG_COMMAND_TICKETLISTCOMMENT, _comment.c_str());
    }
    return ss.str();
}