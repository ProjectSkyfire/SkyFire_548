/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef DEF_DEADMINES_H
#define DEF_DEADMINES_H

#include "InstanceScript.h"

enum CannonState
{
    CANNON_NOT_USED,
    CANNON_BLAST_INITIATED,
    PIRATES_ATTACK,
    EVENT_DONE
};

enum NPCs
{
    NPC_GLUBTOK              = 47162,
    NPC_HELIX_GEARBREAKER    = 47296,
    NPC_LUMBERING_OAF        = 47297,
    NPC_FOE_REAPER_5000      = 43778,
    NPC_ADMIRAL_RIPSNARL     = 47626,
    NPC_CAPTAIN_COOKIE       = 47739,
    NPC_VANESSA_VANCLEEF     = 49541,
    NPC_VANESSA_SITTING      = 49429,
    NPC_NOTE_FROM_VANESSA    = 49564,
    // Horde npc
    NPC_KAGTHA               = 46889,
    NPC_SLINKY_SHARPSHIV     = 46906,
    NPC_MISS_MAYHEM          = 46902,
    NPC_MAYHEM_REAPER        = 46903,
    NPC_HAND_ASSASIN         = 46890,
    // Alliance npc
    NPC_HORATIO_LAINE        = 46612,
    NPC_INVESTIGATOR         = 46614,
    NPC_DEFENDER             = 50595,
    NPC_CRIME_SCENE_BOT      = 46613,
    // Trash
    NPC_MINING_POWDER        = 48284,
    NPC_DEFIAS_WATCHER       = 47403,
    NPC_DEFIAS_REAPER        = 47404,

    // Misc
    NPC_FROST_BLOSSOM_BUNNY  = 47284,
    NPC_FIRE_BLOSSOM_BUNNY   = 47282,
};

enum eSpells
{
    SPELL_EXPLODE_MINE = 89769,
    SPELL_OFF_LINE     = 88348,
};

enum Data
{
    DATA_GLUBTOK          = 0,
    DATA_HELIX            = 1,
    DATA_FOEREAPER        = 2,
    DATA_ADMIRAL          = 3,
    DATA_CAPTAIN          = 4,
    DATA_VANESSA          = 5,
    DATA_CANNON_EVENT     = 6,
    DATA_OAF              = 7,
    DATA_VANESSA_EVENT    = 8,
    DATA_TEAM_IN_INSTANCE = 9,
};

enum GameObjects
{
    GO_FOUNDRY_DOOR      = 16399,
    GO_FACTORY_DOOR      = 13965,
    GO_MAST_ROOM_DOOR    = 16400,
    GO_IRONCLAD_DOOR     = 16397,
    GO_DEFIAS_CANNON     = 16398,
    GO_DOOR_LEVER        = 101833,
    GO_HEAVY_DOOR        = 17153,
    GO_HEAVY_DOOR_2      = 17154,
    GO_GOBLIN_TELEPORTER = 208002,
};

enum Sounds
{
    SOUND_CANNONFIRE  = 1400,
    SOUND_DESTROYDOOR = 3079,
};

enum Worldstates
{
    WORLDSTATE_PROTOTYPE_PRODIGY    = 8938,
    WORLDSTATE_VANCLEEF_VINDICATION = 8993,
};

const Position centershipPos = {-63.167f, -819.315f, 41.27f, 6.25f};
const Position notePos = {-74.3611f, -820.014f, 40.3714f, 4.01426f};
const Position vanessaPos = {-75.5851f, -819.964f, 47.0673f, 6.17846f};

template<class AI>
CreatureAI* GetInstanceAI(Creature* creature)
{
    return GetInstanceAI<AI>(creature, "instance_deadmines");
}

#endif
