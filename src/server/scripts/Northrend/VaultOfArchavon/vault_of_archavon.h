/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef DEF_ARCHAVON_H
#define DEF_ARCHAVON_H

uint32 const EncounterCount = 4;

enum Data
{
    DATA_ARCHAVON       = 0,
    DATA_EMALON         = 1,
    DATA_KORALON        = 2,
    DATA_TORAVON        = 3,
};

enum CreatureIds
{
    NPC_ARCHAVON        = 31125,
    NPC_EMALON          = 33993,
    NPC_KORALON         = 35013,
    NPC_TORAVON         = 38433
};

enum AchievementCriteriaIds
{
    CRITERIA_EARTH_WIND_FIRE_10 = 12018,
    CRITERIA_EARTH_WIND_FIRE_25 = 12019,
};

enum AchievementSpells
{
    SPELL_EARTH_WIND_FIRE_ACHIEVEMENT_CHECK = 68308,
};

#endif
