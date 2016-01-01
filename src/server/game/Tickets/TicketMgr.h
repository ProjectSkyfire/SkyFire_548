/*
* Copyright (C) 2011-2016 Project SkyFire <http://www.projectskyfire.org/>
* Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2016 MaNGOS <http://getmangos.com/>
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

#ifndef SF_TICKETMGR_H
#define SF_TICKETMGR_H

#include <string>
#include <ace/Singleton.h>

#include "ObjectMgr.h"
#include "TicketInfo.h"

typedef std::map<uint32, BugTicket*> BugTicketList;
typedef std::map<uint32, GmTicket*> GmTicketList;

class TicketMgr
{
    friend class ACE_Singleton<TicketMgr, ACE_Null_Mutex>;

private:
    TicketMgr();
    ~TicketMgr();

public:
    static TicketMgr* instance()
    {
        static TicketMgr instance;
        return &instance;
    }

    void Initialize();

    template<typename T> T* GetTicket(uint32 ticketId);
    template<typename T> uint32 GetOpenTicketCount() const;

    GmTicket* GetGmTicketByPlayerGuid(ObjectGuid playerGuid) const
    {
        for (GmTicketList::const_iterator itr = _gmTicketList.begin(); itr != _gmTicketList.end(); ++itr)
            if (itr->second->GetPlayerGuid() == playerGuid && !itr->second->IsClosed())
                return itr->second;

        return NULL;
    }

    GmTicket* GetTicket(uint32 ticketId)
    {
        GmTicketList::iterator itr = _gmTicketList.find(ticketId);
        if (itr != _gmTicketList.end())
            return itr->second;

        return NULL;
    }

    GmTicket* GetOldestOpenTicket()
    {
        for (GmTicketList::const_iterator itr = _gmTicketList.begin(); itr != _gmTicketList.end(); ++itr)
            if (itr->second && !itr->second->IsClosed() && !itr->second->IsCompleted())
                return itr->second;

        return NULL;
    }

    bool GetFeedBackSystemStatus() { return _feedbackSystemStatus; }
    bool GetGmTicketSystemStatus() { return _gmTicketSystemStatus; }

    uint64 GetLastChange() const { return _lastChange; }

    void SetFeedBackSystemStatus(bool status){ _feedbackSystemStatus = status; }
    void SetGmTicketSystemStatus(bool status){ _gmTicketSystemStatus = status; }

    void LoadGmTickets();
    void LoadBugTickets();

    void AddTicket(GmTicket* ticket);
    void AddTicket(BugTicket* ticket);

    template<typename T> void RemoveTicket(uint32 ticketId);
    template<typename T> void CloseTicket(uint32 ticketId, int64 closedBy);
    template<typename T> void ResetTickets();
    template<typename T> void ShowList(ChatHandler& handler) const;
    template<typename T> void ShowList(ChatHandler& handler, bool onlineOnly) const;
    template<typename T> void ShowClosedList(ChatHandler& handler) const;

    void ShowGmEscalatedList(ChatHandler& handler) const;
    void SendGmTicket(WorldSession* session, GmTicket* ticket) const;
    void SendGmTicketUpdate(Opcodes opcode, GMTicketResponse response, Player* player) const;
    void SendGmResponsee(WorldSession* session, GmTicket* ticket) const;
    void UpdateLastChange() { _lastChange = uint64(time(NULL)); }

    uint32 GenerateGmTicketId() { return ++_lastGmTicketId; }
    uint32 GenerateBugId() { return ++_lastBugId; }

private:
    bool _feedbackSystemStatus;
    bool _gmTicketSystemStatus;
    GmTicketList _gmTicketList;
    BugTicketList _bugTicketList;
    uint32 _lastGmTicketId;
    uint32 _lastBugId;
    uint64 _lastChange;
    uint32 _openGmTicketCount;
    uint32 _openBugTicketCount;
};

#define sTicketMgr ACE_Singleton<TicketMgr, ACE_Null_Mutex>::instance()

#endif // SF_TICKETMGR_H