/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
Name: event_commandscript
%Complete: 100
Comment: All event related commands
Category: commandscripts
EndScriptData */

#include "Chat.h"
#include "GameEventMgr.h"
#include "Language.h"
#include "Player.h"
#include "ScriptMgr.h"

class event_commandscript : public CommandScript
{
public:
    event_commandscript() : CommandScript("event_commandscript") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> eventCommandTable =
        {
            { "activelist", rbac::RBAC_PERM_COMMAND_EVENT_ACTIVELIST, true, &HandleEventActiveListCommand, "", },
            { "start",      rbac::RBAC_PERM_COMMAND_EVENT_START,      true, &HandleEventStartCommand,      "", },
            { "stop",       rbac::RBAC_PERM_COMMAND_EVENT_STOP,       true, &HandleEventStopCommand,       "", },
            { "",           rbac::RBAC_PERM_COMMAND_EVENT,            true, &HandleEventInfoCommand,       "", },
        };
        static std::vector<ChatCommand> commandTable =
        {
            { "event", rbac::RBAC_PERM_COMMAND_EVENT, false, NULL, "", eventCommandTable },
        };
        return commandTable;
    }

    static bool HandleEventActiveListCommand(ChatHandler* handler, char const* /*args*/)
    {
        uint32 counter = 0;

        GameEventMgr::GameEventDataMap const& events = sGameEventMgr->GetEventMap();
        GameEventMgr::ActiveEvents const& activeEvents = sGameEventMgr->GetActiveEventList();

        char const* active = handler->GetSkyFireString(LANG_ACTIVE);

        for (GameEventMgr::ActiveEvents::const_iterator itr = activeEvents.begin(); itr != activeEvents.end(); ++itr)
        {
            uint32 eventId = *itr;
            GameEventData const& eventData = events[eventId];

            if (handler->GetSession())
                handler->PSendSysMessage(LANG_EVENT_ENTRY_LIST_CHAT, eventId, eventId, eventData.description.c_str(), active);
            else
                handler->PSendSysMessage(LANG_EVENT_ENTRY_LIST_CONSOLE, eventId, eventData.description.c_str(), active);

            ++counter;
        }

        if (counter == 0)
            handler->SendSysMessage(LANG_NOEVENTFOUND);
        handler->SetSentErrorMessage(true);

        return true;
    }

    static bool HandleEventInfoCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        // id or [name] Shift-click form |color|Hgameevent:id|h[name]|h|r
        char* id =  handler->extractKeyFromLink((char*)args, "Hgameevent");
        if (!id)
            return false;

        uint32 eventId = atoi(id);

        GameEventMgr::GameEventDataMap const& events = sGameEventMgr->GetEventMap();

        if (eventId >= events.size())
        {
            handler->SendSysMessage(LANG_EVENT_NOT_EXIST);
            handler->SetSentErrorMessage(true);
            return false;
        }

        GameEventData const& eventData = events[eventId];
        if (!eventData.isValid())
        {
            handler->SendSysMessage(LANG_EVENT_NOT_EXIST);
            handler->SetSentErrorMessage(true);
            return false;
        }

        GameEventMgr::ActiveEvents const& activeEvents = sGameEventMgr->GetActiveEventList();
        bool active = activeEvents.find(eventId) != activeEvents.end();
        char const* activeStr = active ? handler->GetSkyFireString(LANG_ACTIVE) : "";

        std::string startTimeStr = TimeToTimestampStr(eventData.start);
        std::string endTimeStr = TimeToTimestampStr(eventData.end);

        uint32 delay = sGameEventMgr->NextCheck(eventId);
        time_t nextTime = time(NULL) + delay;
        std::string nextStr = nextTime >= eventData.start && nextTime < eventData.end ? TimeToTimestampStr(time(NULL) + delay) : "-";

        std::string occurenceStr = secsToTimeString(eventData.occurence * MINUTE);
        std::string lengthStr = secsToTimeString(eventData.length * MINUTE);

        handler->PSendSysMessage(LANG_EVENT_INFO, eventId, eventData.description.c_str(), activeStr,
            startTimeStr.c_str(), endTimeStr.c_str(), occurenceStr.c_str(), lengthStr.c_str(),
            nextStr.c_str());
        return true;
    }

    static bool HandleEventStartCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        // id or [name] Shift-click form |color|Hgameevent:id|h[name]|h|r
        char* id =  handler->extractKeyFromLink((char*)args, "Hgameevent");
        if (!id)
            return false;

        int32 eventId = atoi(id);

        GameEventMgr::GameEventDataMap const& events = sGameEventMgr->GetEventMap();

        if (eventId < 1 || uint32(eventId) >= events.size())
        {
            handler->SendSysMessage(LANG_EVENT_NOT_EXIST);
            handler->SetSentErrorMessage(true);
            return false;
        }

        GameEventData const& eventData = events[eventId];
        if (!eventData.isValid())
        {
            handler->SendSysMessage(LANG_EVENT_NOT_EXIST);
            handler->SetSentErrorMessage(true);
            return false;
        }

        GameEventMgr::ActiveEvents const& activeEvents = sGameEventMgr->GetActiveEventList();
        if (activeEvents.find(eventId) != activeEvents.end())
        {
            handler->PSendSysMessage(LANG_EVENT_ALREADY_ACTIVE, eventId);
            handler->SetSentErrorMessage(true);
            return false;
        }

        sGameEventMgr->StartEvent(eventId, true);
        return true;
    }

    static bool HandleEventStopCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        // id or [name] Shift-click form |color|Hgameevent:id|h[name]|h|r
        char* id =  handler->extractKeyFromLink((char*)args, "Hgameevent");
        if (!id)
            return false;

        int32 eventId = atoi(id);

        GameEventMgr::GameEventDataMap const& events = sGameEventMgr->GetEventMap();

        if (eventId < 1 || uint32(eventId) >= events.size())
        {
            handler->SendSysMessage(LANG_EVENT_NOT_EXIST);
            handler->SetSentErrorMessage(true);
            return false;
        }

        GameEventData const& eventData = events[eventId];
        if (!eventData.isValid())
        {
            handler->SendSysMessage(LANG_EVENT_NOT_EXIST);
            handler->SetSentErrorMessage(true);
            return false;
        }

        GameEventMgr::ActiveEvents const& activeEvents = sGameEventMgr->GetActiveEventList();

        if (activeEvents.find(eventId) == activeEvents.end())
        {
            handler->PSendSysMessage(LANG_EVENT_NOT_ACTIVE, eventId);
            handler->SetSentErrorMessage(true);
            return false;
        }

        sGameEventMgr->StopEvent(eventId, true);
        return true;
    }
};

void AddSC_event_commandscript()
{
    new event_commandscript();
}
