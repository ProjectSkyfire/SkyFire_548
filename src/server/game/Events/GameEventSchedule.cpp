/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "GameEventSchedule.h"
#include "Platform/TimeUtils.h"

#include <limits>

namespace
{
    bool GetFirstWeekdayOfMonth(time_t referenceTime, int monthOffset, int weekday, time_t& result)
    {
        if (weekday < 0 || weekday > 6)
            return false;

        tm localTime;
        if (!Skyfire::LocalTime(referenceTime, localTime))
            return false;

        localTime.tm_mon += monthOffset;
        localTime.tm_mday = 1;
        localTime.tm_hour = 0;
        localTime.tm_min = 0;
        localTime.tm_sec = 0;
        localTime.tm_isdst = -1;

        time_t firstOfMonth = mktime(&localTime);
        if (firstOfMonth == time_t(-1) || !Skyfire::LocalTime(firstOfMonth, localTime))
            return false;

        localTime.tm_mday += (weekday - localTime.tm_wday + 7) % 7;
        localTime.tm_isdst = -1;
        result = mktime(&localTime);
        return result != time_t(-1);
    }

    bool AddLocalMinutes(time_t startTime, uint32 durationMinutes, time_t& result)
    {
        tm localTime;
        if (!Skyfire::LocalTime(startTime, localTime))
            return false;

        localTime.tm_mday += durationMinutes / (24 * 60);
        localTime.tm_min += durationMinutes % (24 * 60);
        localTime.tm_isdst = -1;
        result = mktime(&localTime);
        return result != time_t(-1);
    }

    uint32 GetSecondsUntil(time_t currentTime, time_t transitionTime)
    {
        if (transitionTime <= currentTime)
            return 0;

        double seconds = difftime(transitionTime, currentTime);
        if (seconds >= std::numeric_limits<uint32>::max())
            return std::numeric_limits<uint32>::max();

        return uint32(seconds);
    }
}

bool GameEventSchedule::GetMonthlyFirstWeekdayWindow(time_t currentTime, int weekday, uint32 durationMinutes, EventWindow& window)
{
    if (!durationMinutes || !GetFirstWeekdayOfMonth(currentTime, 0, weekday, window.Start))
        return false;

    if (currentTime < window.Start && !GetFirstWeekdayOfMonth(currentTime, -1, weekday, window.Start))
        return false;

    return AddLocalMinutes(window.Start, durationMinutes, window.End);
}

bool GameEventSchedule::IsMonthlyFirstWeekdayActive(time_t currentTime, int weekday, uint32 durationMinutes)
{
    EventWindow window;
    return GetMonthlyFirstWeekdayWindow(currentTime, weekday, durationMinutes, window)
        && currentTime >= window.Start
        && currentTime < window.End;
}

uint32 GameEventSchedule::GetSecondsUntilMonthlyFirstWeekdayTransition(time_t currentTime, int weekday, uint32 durationMinutes)
{
    time_t currentMonthStart;
    if (!durationMinutes || !GetFirstWeekdayOfMonth(currentTime, 0, weekday, currentMonthStart))
        return 0;

    if (currentTime < currentMonthStart)
        return GetSecondsUntil(currentTime, currentMonthStart);

    time_t currentMonthEnd;
    if (!AddLocalMinutes(currentMonthStart, durationMinutes, currentMonthEnd))
        return 0;

    if (currentTime < currentMonthEnd)
        return GetSecondsUntil(currentTime, currentMonthEnd);

    time_t nextStart;
    if (!GetFirstWeekdayOfMonth(currentTime, 1, weekday, nextStart))
        return 0;

    return GetSecondsUntil(currentTime, nextStart);
}
