/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef __EVENTPROCESSOR_H
#define __EVENTPROCESSOR_H

#include "Define.h"

#include <functional>
#include <map>

// Note. All times are in milliseconds here.

class BasicEvent
{
public:
    BasicEvent() : m_addTime(0), m_execTime(0) { to_Abort = false; }
    virtual ~BasicEvent() { }                           // override destructor to perform some actions on event removal

    // this method executes when the event is triggered
    // return false if event does not want to be deleted
    // e_time is execution time, p_time is update interval
    virtual bool Execute(uint64 /*e_time*/, uint32 /*p_time*/) { return true; }

    virtual bool IsDeletable() const { return true; }   // this event can be safely deleted

    virtual void Abort(uint64 /*e_time*/) { }           // this method executes when the event is aborted

    bool to_Abort;                                      // set by externals when the event is aborted, aborted events don't execute
    // and get Abort call when deleted

    // these can be used for time offset control
    uint64 m_addTime;                                   // time when the event was added to queue, filled by event handler
    uint64 m_execTime;                                  // planned time of next execution, filled by event handler
};

class GroupedEvent : public BasicEvent
{
public:
    typedef uint32 group_type;

    explicit GroupedEvent(group_type group) : m_group(group) { }

    group_type GetGroup() const { return m_group; }
    group_type GetGroupMask() const { return 1 << m_group; }

private:
    group_type m_group;
};

class FunctionEvent : public GroupedEvent
{
public:
    typedef std::function<void()> function_type;

    explicit FunctionEvent(function_type const& function, group_type group = 0) : GroupedEvent(group), m_function(function) { }
    bool Execute(uint64 /*e_time*/, uint32 /*p_time*/) OVERRIDE { m_function(); return true; }

protected:
    function_type m_function;
};

typedef std::multimap<uint64, BasicEvent*> EventList;

class EventProcessor
{
public:
    EventProcessor() : m_time(0), m_aborting(false) { }
    ~EventProcessor() { KillAllEvents(true); }

    void Update(uint32 p_time);
    template<class EventFilter>
    BasicEvent* FindEvent(EventFilter const& filter);
    void KillAllEvents(bool force);
    void AddEvent(BasicEvent* Event, uint64 e_time, bool set_addtime = true);
    uint64 CalculateTime(uint64 t_offset) const;

    void Schedule(uint32 delay, BasicEvent* Event) { AddEvent(Event, CalculateTime(delay)); }
    void Schedule(uint32 delay, FunctionEvent::function_type const& function) { Schedule(delay, new FunctionEvent(function)); }
    void Schedule(uint32 delay, GroupedEvent::group_type group, FunctionEvent::function_type const& function) { Schedule(delay, new FunctionEvent(function, group)); }
protected:
    uint64 m_time;
    EventList m_events;
    bool m_aborting;
};

template<class EventFilter>
BasicEvent* EventProcessor::FindEvent(EventFilter const& filter)
{
    for (auto&& event : m_events)
        if (filter(event.second))
            return event.second;

    return NULL;
}
#endif
