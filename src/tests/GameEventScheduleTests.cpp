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

    bool TestPilgrimsBounty()
    {
        bool passed = true;
        GameEventSchedule::EventWindow window;
        for (int year = 2024; year <= 2036; ++year)
        {
            time_t reference = MakeLocalTime(year, 1, 1);
            passed &= Expect(GameEventSchedule::GetPilgrimsBountyWindow(reference, window), "Pilgrim window must be valid");
            tm start;
            Skyfire::LocalTime(window.Start, start);
            passed &= Expect(start.tm_mon == 10 && start.tm_wday == 0 && start.tm_mday >= 18 && start.tm_mday <= 24,
                "Pilgrim start must be the Sunday before the fourth Thursday of November");
            passed &= Expect(!GameEventSchedule::IsPilgrimsBountyActive(window.Start - 1)
                && GameEventSchedule::IsPilgrimsBountyActive(window.Start)
                && GameEventSchedule::IsPilgrimsBountyActive(window.End - 1)
                && !GameEventSchedule::IsPilgrimsBountyActive(window.End), "Pilgrim start/end boundaries failed");
            passed &= Expect(GameEventSchedule::GetSecondsUntilPilgrimsBountyTransition(window.Start - 1) == 1
                && GameEventSchedule::GetSecondsUntilPilgrimsBountyTransition(window.End - 1) == 1,
                "Pilgrim transition must occur at the exact boundary");
            time_t next = window.End + GameEventSchedule::GetSecondsUntilPilgrimsBountyTransition(window.End);
            GameEventSchedule::EventWindow nextWindow;
            passed &= Expect(GameEventSchedule::GetPilgrimsBountyWindow(MakeLocalTime(year + 1, 1, 1), nextWindow)
                && next == nextWindow.Start, "Pilgrim next-year transition failed");
        }
        passed &= Expect(GameEventSchedule::GetPilgrimsBountyWindow(MakeLocalTime(2026, 11, 22), window)
            && HasLocalDate(window.Start, 2026, 11, 22) && HasLocalDate(window.End, 2026, 11, 29),
            "Pilgrim 2026 window should be November 22 through November 28");
        return passed;
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
    passed &= TestPilgrimsBounty();
    passed &= TestSeptemberBoundaries();
    passed &= TestMonthlyTransitions();
    return passed ? 0 : 1;
}
