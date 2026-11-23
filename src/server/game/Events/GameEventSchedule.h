/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_GAME_EVENT_SCHEDULE_H
#define SKYFIRE_GAME_EVENT_SCHEDULE_H

#include "Define.h"

#include <ctime>

namespace GameEventSchedule
{
    struct EventWindow
    {
        time_t Start;
        time_t End;
    };

    bool GetMonthlyFirstWeekdayWindow(time_t currentTime, int weekday, uint32 durationMinutes, EventWindow& window);
    bool IsMonthlyFirstWeekdayActive(time_t currentTime, int weekday, uint32 durationMinutes);
    uint32 GetSecondsUntilMonthlyFirstWeekdayTransition(time_t currentTime, int weekday, uint32 durationMinutes);

    // Seven local calendar days, starting the Sunday before November's fourth Thursday.
    bool GetPilgrimsBountyWindow(time_t currentTime, EventWindow& window);
    bool IsPilgrimsBountyActive(time_t currentTime);
    uint32 GetSecondsUntilPilgrimsBountyTransition(time_t currentTime);
}

#endif
