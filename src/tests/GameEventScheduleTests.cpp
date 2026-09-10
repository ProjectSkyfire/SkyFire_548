/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "GameEventSchedule.h"
#include "TimeUtils.h"

#include <iostream>

namespace
{
    uint32 constexpr DarkmoonFaireDurationMinutes = 7 * 24 * 60;

    bool Expect(bool condition, char const* message)
    {
        if (!condition)
            std::cerr << message << '\n';

        return condition;
    }

    time_t MakeLocalTime(int year, int month, int day, int hour = 0, int minute = 0, int second = 0)
    {
        tm localTime = {};
        localTime.tm_year = year - 1900;
        localTime.tm_mon = month - 1;
        localTime.tm_mday = day;
        localTime.tm_hour = hour;
        localTime.tm_min = minute;
        localTime.tm_sec = second;
        localTime.tm_isdst = -1;
        return mktime(&localTime);
    }

    bool HasLocalDate(time_t value, int year, int month, int day, int hour = 0)
    {
        tm localTime;
        return Skyfire::LocalTime(value, localTime)
            && localTime.tm_year == year - 1900
            && localTime.tm_mon == month - 1
            && localTime.tm_mday == day
            && localTime.tm_hour == hour;
    }

    bool TestSeptemberBoundaries()
    {
        bool passed = true;

        passed &= Expect(!GameEventSchedule::IsMonthlyFirstWeekdayActive(MakeLocalTime(2026, 9, 5, 23, 59, 59), 0, DarkmoonFaireDurationMinutes),
            "Darkmoon Faire should be inactive before the first Sunday");
        passed &= Expect(GameEventSchedule::IsMonthlyFirstWeekdayActive(MakeLocalTime(2026, 9, 6), 0, DarkmoonFaireDurationMinutes),
            "Darkmoon Faire should start at midnight on the first Sunday");
        passed &= Expect(GameEventSchedule::IsMonthlyFirstWeekdayActive(MakeLocalTime(2026, 9, 12, 23, 59, 59), 0, DarkmoonFaireDurationMinutes),
            "Darkmoon Faire should remain active through the following Saturday");
        passed &= Expect(!GameEventSchedule::IsMonthlyFirstWeekdayActive(MakeLocalTime(2026, 9, 13), 0, DarkmoonFaireDurationMinutes),
            "Darkmoon Faire should stop at midnight on the following Sunday");

        return passed;
    }

    bool TestMonthlyTransitions()
    {
        bool passed = true;
        GameEventSchedule::EventWindow window;

        time_t beforeSeptemberFaire = MakeLocalTime(2026, 9, 5, 23, 59, 59);
        uint32 delay = GameEventSchedule::GetSecondsUntilMonthlyFirstWeekdayTransition(beforeSeptemberFaire, 0, DarkmoonFaireDurationMinutes);
        passed &= Expect(delay == 1 && HasLocalDate(beforeSeptemberFaire + delay, 2026, 9, 6),
            "The transition before the Faire should be the current month's first Sunday");

        passed &= Expect(GameEventSchedule::GetMonthlyFirstWeekdayWindow(MakeLocalTime(2026, 10, 4), 0, DarkmoonFaireDurationMinutes, window)
            && HasLocalDate(window.Start, 2026, 10, 4)
            && HasLocalDate(window.End, 2026, 10, 11),
            "October Darkmoon Faire should run from October 4 through October 10");
        passed &= Expect(GameEventSchedule::GetMonthlyFirstWeekdayWindow(MakeLocalTime(2026, 11, 1), 0, DarkmoonFaireDurationMinutes, window)
            && HasLocalDate(window.Start, 2026, 11, 1)
            && HasLocalDate(window.End, 2026, 11, 8),
            "November Darkmoon Faire should preserve local midnight across daylight saving time");
        passed &= Expect(GameEventSchedule::GetMonthlyFirstWeekdayWindow(MakeLocalTime(2026, 12, 6), 0, DarkmoonFaireDurationMinutes, window)
            && HasLocalDate(window.Start, 2026, 12, 6)
            && HasLocalDate(window.End, 2026, 12, 13),
            "December Darkmoon Faire should handle a 35-day gap between first Sundays");

        time_t afterSeptemberFaire = MakeLocalTime(2026, 9, 13);
        delay = GameEventSchedule::GetSecondsUntilMonthlyFirstWeekdayTransition(afterSeptemberFaire, 0, DarkmoonFaireDurationMinutes);
        passed &= Expect(HasLocalDate(afterSeptemberFaire + delay, 2026, 10, 4),
            "The next transition after September should be the October first Sunday");

        return passed;
    }
}

int main()
{
    bool passed = true;
    passed &= TestSeptemberBoundaries();
    passed &= TestMonthlyTransitions();
    return passed ? 0 : 1;
}
