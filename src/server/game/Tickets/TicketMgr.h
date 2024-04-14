/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_TICKETMGR_H
#define SF_TICKETMGR_H

#include <ace/Singleton.h>
#include <string>

#include "ObjectMgr.h"
#include "TicketInfo.h"

typedef std::map<uint32, BugTicket*> BugTicketList;
typedef std::map<uint32, GmTicket*> GmTicketList;
typedef std::map<uint32, SuggestTicket*> SuggestTicketList;

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

    bool GetFeedBackSystemStatus() const { return _feedbackSystemStatus; }
    bool GetGmTicketSystemStatus() const { return _gmTicketSystemStatus; }

    uint64 GetLastChange() const { return _lastChange; }

    void SetFeedBackSystemStatus(bool status) { _feedbackSystemStatus = status; }
    void SetGmTicketSystemStatus(bool status) { _gmTicketSystemStatus = status; }

    void LoadGmTickets();
    void LoadBugTickets();
    void LoadSuggestTickets();

    void AddTicket(GmTicket* ticket);
    void AddTicket(BugTicket* ticket);
    void AddTicket(SuggestTicket* ticket);

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
    uint32 GenerateSuggestId() { return ++_lastSuggestId; }

private:
    bool _feedbackSystemStatus;
    bool _gmTicketSystemStatus;
    GmTicketList _gmTicketList;
    BugTicketList _bugTicketList;
    SuggestTicketList _suggestTicketList;
    uint32 _lastGmTicketId;
    uint32 _lastBugId;
    uint32 _lastSuggestId;
    uint64 _lastChange;
    uint32 _openGmTicketCount;
    uint32 _openBugTicketCount;
    uint32 _openSuggestTicketCount;
};

#define sTicketMgr ACE_Singleton<TicketMgr, ACE_Null_Mutex>::instance()

#endif // SF_TICKETMGR_H