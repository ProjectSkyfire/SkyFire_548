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

#ifndef SF_TICKETINFO_H
#define SF_TICKETINFO_H

#include <string>
#include <ace/Singleton.h>

#include "ObjectMgr.h"

// from blizzard lua
enum GMTicketSystemStatus
{
    GMTICKET_QUEUE_STATUS_DISABLED = 0,
    GMTICKET_QUEUE_STATUS_ENABLED = 1
};

enum GMTicketStatus
{
    GMTICKET_STATUS_HASTEXT = 0x06,
    GMTICKET_STATUS_DEFAULT = 0x0A
};

enum GMTicketResponse
{
    GMTICKET_RESPONSE_ALREADY_EXIST = 1,
    GMTICKET_RESPONSE_CREATE_SUCCESS = 2,
    GMTICKET_RESPONSE_CREATE_ERROR = 3,
    GMTICKET_RESPONSE_UPDATE_SUCCESS = 4,
    GMTICKET_RESPONSE_UPDATE_ERROR = 5,
    GMTICKET_RESPONSE_TICKET_DELETED = 9
};

// from Blizzard LUA:
// GMTICKET_ASSIGNEDTOGM_STATUS_NOT_ASSIGNED = 0;    -- ticket is not currently assigned to a gm
// GMTICKET_ASSIGNEDTOGM_STATUS_ASSIGNED = 1;        -- ticket is assigned to a normal gm
// GMTICKET_ASSIGNEDTOGM_STATUS_ESCALATED = 2;        -- ticket is in the escalation queue
// 3 is a custom value and should never actually be sent
enum GMTicketEscalationStatus
{
    TICKET_UNASSIGNED = 0,
    TICKET_ASSIGNED = 1,
    TICKET_IN_ESCALATION_QUEUE = 2,
    TICKET_ESCALATED_ASSIGNED = 3
};

// from blizzard lua
enum GMTicketOpenedByGMStatus
{
    GMTICKET_OPENEDBYGM_STATUS_NOT_OPENED = 0,      // ticket has never been opened by a gm
    GMTICKET_OPENEDBYGM_STATUS_OPENED = 1       // ticket has been opened by a gm
};

enum LagReportType
{
    LAG_REPORT_TYPE_LOOT = 1,
    LAG_REPORT_TYPE_AUCTION_HOUSE = 2,
    LAG_REPORT_TYPE_MAIL = 3,
    LAG_REPORT_TYPE_CHAT = 4,
    LAG_REPORT_TYPE_MOVEMENT = 5,
    LAG_REPORT_TYPE_SPELL = 6
};

class ChatHandler;

class TicketInfo
{
public:
    TicketInfo();
    TicketInfo(Player* player);
    virtual ~TicketInfo();

    bool IsClosed() const { return _closedBy; }
    bool IsAssigned() const { return _assignedTo != 0; }
    bool IsFromPlayer(uint64 guid) const { return guid == _playerGuid; }
    bool IsAssignedTo(uint64 guid) const { return guid == _assignedTo; }
    bool IsAssignedNotTo(uint64 guid) const { return IsAssigned() && !IsAssignedTo(guid); }

    std::string const& GetComment() const { return _comment; }
    std::string const& GetPlayerName() const { return _playerName; }
    std::string GetAssignedToName() const;

    uint32 GetTicketId() const { return _ticketId; }
    uint32 GetMapId() const { return _mapId; }
    uint64 GetPlayerGuid() const { return _playerGuid; }
    uint64 GetAssignedToGUID() const { return _assignedTo; }

    Player* GetPlayer() const { return ObjectAccessor::FindPlayer(_playerGuid); }
    Player* GetAssignedPlayer() const { return ObjectAccessor::FindPlayer(_assignedTo); }

    void SetComment(std::string comment){ _comment = comment; }
    void SetPosition(uint32 MapID, G3D::Vector3 pos);
    void SetClosedBy(uint64 closer) { _closedBy = closer; }
    void SetComment(std::string &comment) { _comment = comment; }

    void TeleportTo(Player* player) const;

    virtual void SetAssignedTo(uint64 assignedTo, bool /*isAdmin*/ = false) { _assignedTo = assignedTo; }
    virtual void SetUnassigned() { _assignedTo = 0; }

    virtual void LoadFromDB(Field* fields) = 0;
    virtual void SaveToDB(SQLTransaction& trans) const = 0;
    virtual void DeleteFromDB() = 0;

    virtual std::string FormatMessageString(ChatHandler& handler, bool detailed = false) const = 0;
    virtual std::string FormatMessageString(ChatHandler& handler, const char* szClosedName, const char* szAssignedToName, const char* szUnassignedName, const char* szDeletedName) const;

protected:
    uint32 _ticketId;
    uint32 _ticketCreateTime;
    uint32 _mapId;
    uint64 _playerGuid;
    uint64 _closedBy;
    uint64 _assignedTo;
    std::string _comment;
    std::string _playerName;
    G3D::Vector3 _pos;
};

class GmTicket : public TicketInfo
{
public:
    GmTicket();
    GmTicket(Player* player);
    ~GmTicket();

    void AppendResponse(std::string const& response) { _response += response; }
    void SetEscalatedStatus(GMTicketEscalationStatus escalatedStatus) { _escalatedStatus = escalatedStatus; }
    void SetViewed() { _viewed = true; }
    void SetUnassigned() override;
    void SetChatLog(std::list<uint32> time, std::string const& log);
    void SetCompleted() { _completed = true; }
    void SetMessage(std::string const& message);
    void SetGmAction(bool needResponse, bool haveTicket);

    bool IsCompleted() const { return _completed; }
    bool GetViewed(){ return _viewed; }

    void  LoadFromDB(Field* fields) OVERRIDE;
    void  SaveToDB(SQLTransaction& trans) const OVERRIDE;
    void  DeleteFromDB() OVERRIDE;

    std::string FormatMessageString(ChatHandler& handler, bool detailed = false) const override;
    std::string FormatMessageString(ChatHandler& handler, const char* szClosedName, const char* szAssignedToName, const char* szUnassignedName, const char* szDeletedName) const override;

    std::string const& GetMessage() const { return _message; }
    std::string const& GetChatLog() const { return _chatLog; }
    std::string const& GetResponse() const { return _response; }

    uint64 GetLastModifiedTime() const { return _lastModifiedTime; }
    GMTicketEscalationStatus GetEscalatedStatus() const { return _escalatedStatus; }

private:
    uint64 _lastModifiedTime;
    GMTicketEscalationStatus _escalatedStatus;
    GMTicketOpenedByGMStatus _openedByGmStatus;
    bool _completed;
    bool _viewed;
    bool _needResponse; /// @todo find out the use of this, and then store it in DB
    bool _haveTicket;
    std::string _message;
    std::string _response;
    std::string _chatLog; // No need to store in db, will be refreshed every session client side
};

class BugTicket : public TicketInfo
{
public:
    BugTicket();
    BugTicket(Player* player, WorldPacket& bugPacket);
    ~BugTicket();

    std::string const& GetNote() const { return _bugnote; }

    void SetOrientation(float Orientation) { _Orientation = Orientation; }
    void SetNote(std::string const& bugnote) { _bugnote = bugnote; }

    void LoadFromDB(Field* fields) OVERRIDE;
    void SaveToDB(SQLTransaction& trans) const OVERRIDE;
    void DeleteFromDB() OVERRIDE;

    using TicketInfo::FormatMessageString;
    std::string FormatMessageString(ChatHandler& handler, bool detailed = false) const OVERRIDE;

private:
    float _Orientation;
    std::string _bugnote;
};

class SuggestTicket : public TicketInfo
{
public:
    SuggestTicket();
    SuggestTicket(Player* player, WorldPacket& bugPacket);
    ~SuggestTicket();

    std::string const& GetNote() const { return _suggestnote; }

    void SetOrientation(float Orientation) { _Orientation = Orientation; }
    void SetNote(std::string const& suggestnote) { _suggestnote = suggestnote; }

    void LoadFromDB(Field* fields) OVERRIDE;
    void SaveToDB(SQLTransaction& trans) const OVERRIDE;
    void DeleteFromDB() OVERRIDE;

    using TicketInfo::FormatMessageString;
    std::string FormatMessageString(ChatHandler& handler, bool detailed = false) const OVERRIDE;

private:
    float _Orientation;
    std::string _suggestnote;
};

#endif // SF_TICKETINFO_H