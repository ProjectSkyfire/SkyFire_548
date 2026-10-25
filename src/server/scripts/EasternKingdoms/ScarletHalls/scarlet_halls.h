/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SCARLET_HALLS_H_
#define SCARLET_HALLS_H_

#include "Map.h"
#include "Creature.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"

uint32 const EncounterCount = 4;

#define POINTS 16

const Position EndScenario        = { 1038.13f, 509.705f, 13.47f, 0.10f };
const Position JumpPoint          = { 1206.25f, 444.04f,  0.98f,  0.1f  };
const Position TeleportPoint      = { 1298.21f, 549.49f,  12.79f, 3.12f };
const Position ExplodingStartPos1 = {  905.77f, 534.20f,  12.90f, 6.23f };
const Position ExplodingStartPos2 = {  906.47f, 547.90f,  12.92f, 6.23f };
const Position DragonBreathMover  = { 1288.46f, 549.30f,  12.81f, 3.12f };

const Position BladeStormPoints[POINTS] =
{
    { 1201.52f, 438.68f, 0.98f,  1.86f },
    { 1198.41f, 445.94f, 0.98f,  1.0f  },
    { 1205.38f, 445.08f, 1.01f,  0.87f },
    { 1213.15f, 453.57f, 3.53f,  5.95f },
    { 1218.32f, 446.91f, 6.08f,  5.31f },
    { 1222.85f, 438.54f, 6.08f,  5.18f },
    { 1215.36f, 427.42f, 6.08f,  4.11f },
    { 1200.62f, 425.51f, 7.48f,  3.12f },
    { 1188.55f, 436.41f, 10.66f, 2.30f },
    { 1188.44f, 449.05f, 11.01f, 1.56f },
    { 1198.83f, 462.78f, 7.77f,  0.92f },
    { 1218.94f, 459.59f, 6.08f,  6.12f },
    { 1223.45f, 444.52f, 6.09f,  4.08f },
    { 1212.56f, 431.35f, 2.93f,  4.02f },
    { 1200.78f, 435.94f, 0.98f,  1.81f },
    { 1206.30f, 443.73f, 0.98f,  0.95f }
};

const Position DefenderPath1[6] =
{
    { 1187.91f, 438.71f, 11.04f, 5.12f },
    { 1195.04f, 428.00f, 8.63f,  5.29f },
    { 1211.36f, 425.51f, 6.08f,  6.13f },
    { 1225.26f, 441.73f, 6.08f,  0.97f },
    { 1219.72f, 447.40f, 6.08f,  2.33f },
    { 1205.54f, 458.19f, 1.15f,  2.50f }
};

const Position DefenderPath2[6] =
{
    { 1188.02f, 449.28f, 11.00f, 1.03f },
    { 1198.27f, 461.83f, 7.94f,  0.82f },
    { 1218.28f, 459.83f, 6.08f,  6.18f },
    { 1223.84f, 445.44f, 6.08f,  4.24f },
    { 1219.39f, 440.57f, 6.08f,  3.97f },
    { 1205.45f, 429.81f, 1.11f,  3.74f }
};

const Position HoodedCrusaderWp[2] =
{
    { 1276.34f, 549.60f, 12.82f, 0.02f },
    { 1295.33f, 549.61f, 12.79f, 6.24f }
};

enum DataTypes
{
    BOSS_HOUNDMASTER_BRAUN    = 0,
    BOSS_ARMSMASTER_HARLAN    = 1,
    BOSS_FLAMEWEAVER_KOEGLER  = 2,
    DATA_COMANDER_LINDON      = 3,

    DATA_HOODED_CRUSADER      = 4,
};

enum CreaturesIds
{
    NPC_HOUNDMASTER_BRAUN         = 59303,
    NPC_ARMSMASTER_HARLAN         = 58632,
    NPC_FLAMEWEAVER_KOEGLER       = 59150,
    NPC_OBEDIEND_HOUND            = 59309,
    NPC_COMANDER_LINDON           = 59191,
    NPC_MASTER_ARCHER             = 59175,
    NPC_SERGEANT_VERDONE          = 59302,
    NPC_SCARLET_GUARDIAN          = 59299,
    NPC_SCARLET_DEFENDER          = 58998,
    NPC_BOOK_CASE                 = 59155,
    NPC_REINFORCED_ARCHERY_TARGET = 59163,
    NPC_STAKE                     = 58701,
    NPC_CANNON_STALKER            = 58726,
    NPC_DRAGON_BREATH_TARGET      = 59198,
    NPC_HOODED_CRUSADER           = 64738,
    NPC_REFEREE                   = 66091,
    NPC_VIGILANT_WATCHMAN         = 58898,
    NPC_EXPLODING_SHOT_STALKER    = 59683,
    NPC_STARVING_HOUND            = 58876,
};

enum eActions
{
    ACTION_KODEX              = 0,
    ACTION_FIRE_AT_LINE       = 1,
    ACTION_MELEE              = 2,
    ACTION_GROUND_EXPLOSION   = 3,
};

enum eTalk
{
    TALK_INTRO_H              = 0,
    TALK_CODEX                = 1,
};

enum eSpells
{
    SPELL_STAGGERING_SHOT     = 113642,
    SPELL_HEROIC_DEFENSE      = 113436,
};

enum eGameObjects
{
    GO_COMANDER_LINDON_EXIT   = 212160,
    GO_HOUNDMASTER_BRAUN_EXIT = 210097,
    GO_ARMSMASTER_HARLAN_EXIT = 210480,
    GO_GOLD_COINS             = 210894,
};

enum Worldstates
{
    WORLDSTATE_HUMANE_SOCIETY = 12645,
};

#endif // SCARLET_HALLS_H_
