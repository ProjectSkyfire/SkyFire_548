/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef DEF_SHATTERED_H
#define DEF_SHATTERED_H

uint32 const EncounterCount          = 3;
uint32 const VictimCount             = 3;

enum DataTypes
{
    DATA_NETHEKURSE                  = 0,
    DATA_OMROGG                      = 1,
    DATA_KARGATH                     = 2,

    // Heroic executioner event, not boss slots
    DATA_SHATTERED_EXECUTIONER       = 3,
    DATA_PRISONERS_EXECUTED          = 4,
    DATA_TEAM_IN_INSTANCE            = 5,
    DATA_FIRST_PRISONER              = 6,
    DATA_SECOND_PRISONER             = 7,
    DATA_THIRD_PRISONER              = 8,
    DATA_EXECUTIONER_EVENT           = 9
};

// Saved with the instance: the executioner GUID is empty after a reload both before the event and
// after it, so the GUID cannot tell whether the event may start
enum ExecutionerEventStates
{
    EXECUTIONER_EVENT_NOT_STARTED    = 0,
    EXECUTIONER_EVENT_RUNNING        = 1,
    EXECUTIONER_EVENT_DONE           = 2
};

enum CreatureIds
{
    NPC_GRAND_WARLOCK_NETHEKURSE     = 16807,
    NPC_KARGATH_BLADEFIST            = 16808,
    NPC_SHATTERED_EXECUTIONER        = 17301,

    // Alliance
    NPC_RANDY_WHIZZLESPROCKET        = 17288,
    NPC_CAPTAIN_ALINA                = 17290,
    NPC_ALLIANCE_VICTIM_1            = 17289,
    NPC_ALLIANCE_VICTIM_2            = 17292,

    // Horde
    NPC_DRISELLA                     = 17294,
    NPC_CAPTAIN_BONESHATTER          = 17296,
    NPC_HORDE_VICTIM_1               = 17295,
    NPC_HORDE_VICTIM_2               = 17297
};

enum GameobjectIds
{
    GO_GRAND_WARLOCK_CHAMBER_DOOR_1  = 182539,
    GO_GRAND_WARLOCK_CHAMBER_DOOR_2  = 182540
};

enum QuestIds
{
    QUEST_IMPRISONED_A               = 9524,
    QUEST_IMPRISONED_H               = 9525
};

enum InstanceSpells
{
    SPELL_KARGATH_EXECUTIONER_1      = 39288,
    SPELL_KARGATH_EXECUTIONER_2      = 39289,
    SPELL_KARGATH_EXECUTIONER_3      = 39290,
    SPELL_REMOVE_KARGATH_EXECUTIONER = 39291
};

enum Actions
{
    ACTION_EXECUTIONER_TAUNT         = 1
};

struct ExecutionerVictim
{
    uint32 allianceEntry;
    uint32 hordeEntry;
    Position pos;
};

Position const ExecutionerPos = { 152.8524f, -83.63912f, 2.021005f, 0.06981317f };

ExecutionerVictim const executionerVictims[VictimCount] =
{
    { NPC_CAPTAIN_ALINA,     NPC_CAPTAIN_BONESHATTER, { 138.8807f, -84.22707f, 1.992269f, 0.06981317f } },
    { NPC_ALLIANCE_VICTIM_1, NPC_HORDE_VICTIM_1,      { 151.2411f, -91.02930f, 2.019741f, 1.57079600f } },
    { NPC_ALLIANCE_VICTIM_2, NPC_HORDE_VICTIM_2,      { 151.0459f, -77.51981f, 2.021008f, 4.74729500f } }
};

#endif
