/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_ACHIEVEMENT_CRITERIA_PACKETS_H
#define SKYFIRE_ACHIEVEMENT_CRITERIA_PACKETS_H

#include "Define.h"
#include "DBCEnums.h"

namespace Skyfire
{
namespace Achievements
{
    inline bool IsLiveCriteriaProgressEligible(uint32 achievementFlags)
    {
        return (achievementFlags & (ACHIEVEMENT_FLAG_COUNTER | ACHIEVEMENT_FLAG_HIDDEN)) == 0;
    }

    inline bool IsLiveCriteriaTypeEligible(uint32 criteriaType)
    {
        switch (criteriaType)
        {
            case ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP:
            case ACHIEVEMENT_CRITERIA_TYPE_DEATH:
            case ACHIEVEMENT_CRITERIA_TYPE_DEATH_IN_DUNGEON:
            case ACHIEVEMENT_CRITERIA_TYPE_DEATHS_FROM:
            case ACHIEVEMENT_CRITERIA_TYPE_FLIGHT_PATHS_TAKEN:
                return false;
            default:
                return true;
        }
    }

    inline bool ShouldSendLiveCriteriaProgress(bool hasLinkedAchievement, bool hasLiveEligibleAchievement)
    {
        return hasLinkedAchievement && hasLiveEligibleAchievement;
    }
}
}

#endif
