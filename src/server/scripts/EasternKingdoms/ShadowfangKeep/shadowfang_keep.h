/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef DEF_SHADOWFANG_KEEP_H
#define DEF_SHADOWFANG_KEEP_H

#include "InstanceScript.h"

uint32 const EncounterCount = 5;

const Position AshberyJumpPos      = { -256.11f, 2117.14f, 81.26f,  1.23f };
const Position WaldenExitPos       = { -133.43f, 2164.15f, 128.94f, 5.60f };
const Position SilverlainaTablePos = { -269.68f, 2295.30f, 77.50f,  5.90f };

const Position PastAshberyPath[5] =
{
    { -253.55f, 2131.37f, 81.32f, 1.28f },
    { -249.15f, 2143.68f, 87.01f, 1.07f },
    { -237.88f, 2139.90f, 87.01f, 5.95f },
    { -235.07f, 2148.75f, 90.46f, 1.27f },
    { -241.16f, 2157.44f, 90.62f, 1.81f },
};

const Position PastSpringvalePath[3] =
{
    { -243.55f, 2254.26f, 100.89f, 5.31f },
    { -234.98f, 2256.00f, 101.35f, 0.20f },
    { -227.41f, 2257.63f, 102.75f, 3.44f },
};

const Position PastWaldenPath[3] =
{
    { -165.72f, 2180.66f, 129.25f, 5.86f },
    { -146.63f, 2172.99f, 127.95f, 5.92f },
    { -133.43f, 2164.15f, 128.94f, 5.60f },
};

const Position PreGodfreyPath[3] =
{
    { -159.11f, 2178.03f, 152.10f, 5.91f },
    { -153.85f, 2176.02f, 155.62f, 5.90f },
    { -119.38f, 2162.39f, 155.67f, 5.90f },
};

enum eData
{
    DATA_ASHBURY        = 0,
    DATA_SILVERLAINE    = 1,
    DATA_SPRINGVALE     = 2,
    DATA_VALDEN         = 3,
    DATA_GODFREY        = 4,
    DATA_INTRO          = 5,
    DATA_GUARDS         = 6,
    DATA_GUARDS2        = 7,
    DATA_TEAM           = 8,
    DATA_FRYE           = 9,
    DATA_HUMMEL         = 10,
    DATA_BAXTER         = 11,
    DATA_CROWN          = 12,
    DATA_FOLLOWER       = 13,
};

enum NPCs
{
    NPC_BELMONT             = 47293,
    NPC_GUARD_HORDE1        = 47030,
    NPC_GUARD_HORDE2        = 47031,
    NPC_GUARD_ALLY          = 47027,
    NPC_IVAR                = 47006,
    NPC_CROMUSH             = 47294,
    NPC_ASHBURY             = 46962,
    NPC_SILVERLAINE         = 3887,
    NPC_LUPINE_SPECTRE      = 50923,
    NPC_SPRINGVALE          = 4278,
    NPC_VALDEN              = 46963,
    NPC_GODFREY             = 46964,
    NPC_ODO_BLINDWATCHER    = 50857,
    NPC_WOLF_MASTER_NANDOS  = 50851,
    NPC_RETHILGORE          = 50834,
    NPC_RAZORCLAW_BUTHER    = 50869,
    NPC_STONE_SLEEPER       = 3870,

    // Love is in the Air
    NPC_APOTHECARY_HUMMEL   = 36296,
    NPC_APOTHECARY_FRYE     = 36272,
    NPC_APOTHECARY_BAXTER   = 36565,

    GO_COURTYARD_DOOR       = 18895,
    GO_SORCERER_DOOR        = 18972,
    GO_ARUGAL_DOOR          = 18971,
};

const uint32 FangEntries[] =
{
    NPC_BELMONT,
    NPC_GUARD_HORDE1,
    NPC_GUARD_HORDE2,
    NPC_GUARD_ALLY,
    NPC_IVAR,
};

enum Worldstates
{
    WORLDSTATE_PARDON_DENIED = 8830,
    WORLDSTATE_TO_THE_GROUND = 8832,
};

template<class AI>
CreatureAI* GetInstanceAI(Creature* creature)
{
    return GetInstanceAI<AI>(creature, "instance_shadowfang_keep");
}

#endif
