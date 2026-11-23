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

    // Vanessa VanCleef (heroic)
    NPC_GENERAL_PURPOSE_DUMMY_JMF    = 45979,
    NPC_GLUBTOK_NIGHTMARE_FIRE_BUNNY = 51594,

    // Vanessa's Nightmare (heroic)
    NPC_VANESSAS_TRAP_BUNNY          = 49454,
    NPC_STEAM_VALVE                  = 49457,
    NPC_VANESSA_ANCHOR_BUNNY_JMF     = 51624,
    NPC_VANESSA_VANCLEEF_NIGHTMARE   = 49671,
    NPC_GLUBTOK_NIGHTMARE            = 49670,
    NPC_COLLAPSING_ICICLE            = 49481,
    NPC_HELIX_GEARBREAKER_NIGHTMARE  = 49674,
    NPC_NIGHTMARE_SKITTERLING        = 49493,
    NPC_DARKWEB_DEVOURER             = 49494,
    NPC_CHATTERING_HORROR            = 49495,
    NPC_FOE_REAPER_5000_NIGHTMARE    = 49681,
    NPC_VANESSA_LIGHTNING_PLATTER    = 49520,
    NPC_VANESSA_LIGHTNING_STALKER    = 49521,
    NPC_RIPSNARL_NIGHTMARE           = 49682,
    NPC_ENRAGED_WORGEN               = 49532,
    NPC_EMME_HARRINGTON              = 49534,
    NPC_ERIK_HARRINGTON              = 49535,
    NPC_CALISSA_HARRINGTON           = 49536,
    NPC_JAMES_HARRINGTON             = 49539,
};

enum eSpells
{
    SPELL_EXPLODE_MINE      = 89769,
    SPELL_OFF_LINE          = 88348,
    SPELL_NIGHTMARE_ELIXIR  = 92113,
    SPELL_VANESSAS_BLACKOUT = 92120,
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
    DATA_ACTIVATED_VALVE  = 10,
};

// DATA_VANESSA_EVENT goes through these after the intro (NOT_STARTED, IN_PROGRESS, FAIL, DONE and SPECIAL are the encounter states)
enum NightmareStages
{
    NIGHTMARE_STAGE_TRAP       = 10,
    NIGHTMARE_STAGE_GLUBTOK    = 11,
    NIGHTMARE_STAGE_HELIX      = 12,
    NIGHTMARE_STAGE_FOE_REAPER = 13,
    NIGHTMARE_STAGE_RIPSNARL   = 14,
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

enum Achievements
{
    ACHIEVEMENT_EVENT = 27527,
};

const Position centershipPos = {-63.167f, -819.315f, 41.27f, 6.25f};
const Position notePos = {-74.3611f, -820.014f, 40.3714f, 4.01426f};
const Position vanessaPos = {-75.5851f, -819.964f, 47.0673f, 6.17846f};

// Vanessa's Nightmare
Position const trapBunnyPos = { -205.757f, -579.097f, 42.9862f, 0.0f };
Position const trapBunnyMoveDownPos = { -205.7569f, -579.0972f, 35.98623f, 0.0f };
Position const magmaTrapThrowPos = { -225.1973f, -563.7677f, 51.23737f, 0.0f };
Position const generalPurposeBunnyPos = { -205.4688f, -567.2049f, 21.06016f, 0.0f };
Position const steamValvePos[4] =
{
    { -199.1944f, -583.1545f, 37.33158f, 2.600541f },
    { -201.809f,  -572.6215f, 37.33158f, 4.171337f },
    { -209.6823f, -585.8924f, 37.33158f, 0.9599311f },
    { -212.3524f, -575.3976f, 37.33158f, 5.707227f }
};
Position const vanessaNightmareSpawnPos[] =
{
    { -230.717f, -563.0139f, 51.31293f, 1.047198f },
    { -172.809f, -576.9114f, 19.39756f, 4.817109f },
    { -159.3924f, -580.5174f, 19.39817f, 6.195919f },
    { -98.5816f, -662.6632f, 7.498006f,  3.490659f }
};
Position const glubtokNightmareIntroPos = { -229.3403f, -560.3629f, 51.31293f, 5.742133f };
Position const glubtokNightmareSpawnPos = { -174.8212f, -579.7552f, 19.39602f, 3.193953f };
Position const helixNightmareSpawnPos = { -172.6858f, -580.0469f, 19.39752f, 3.176499f };
Position const foeReaperNightmareIntroPos = { -155.4705f, -580.6945f, 19.39779f, 0.0f };
Position const foeReaperNightmareSpawnPos = { -101.4549f, -663.6493f, 7.505813f, 1.850049f };
Position const ripsnarlNightmareIntroPos = { -101.2135f, -663.4531f, 7.504823f, 1.867502f };
Position const emmeHarringtonSpawnPos = { -98.63194f, -721.6268f, 8.547067f, 1.53589f };
Position const enragedWorgenEmmeSpawnPos[] =
{
    {-97.79166f, -717.8542f, 8.668088f, 4.520403f },
    { -94.40278f, -719.7274f, 8.598646f,  3.560472f },
    { -101.9167f, -718.7552f, 8.726379f,  5.51524f },
};
Position const erikHarringtonSpawnPos = { 5.239583f, -763.0868f, 9.800426f, 2.007129f };
Position const enragedWorgenErikSpawnPos[] =
{
    { 3.326038f, -760.3063f, 9.713999f, 5.31516f },
    { 8.798013f, -762.2252f, 9.625132f, 3.379143f },
    { 4.232807f, -766.6125f, 9.804724f, 1.292649f }
};
Position const calissaHarringtonSpawnPos = { -83.86406f, -775.2837f, 28.37906f, 1.710423f };
Position const jamesHarringtonSpawnPos = { -83.16319f, -774.9636f, 26.90351f, 1.710423f };
Position const glubtokNightmareFireBunnyPos[] =
{
    { -192.2031f,   -595.9636f, 36.37407f,  2.80998f },
    { -220.625f,    -577.9618f, 21.06016f,  2.513274f },
    { -205.3056f,   -563.6285f, 21.06016f,  5.253441f },
    { -214.8958f,   -546.7136f, 19.3898f,   4.712389f },
    { -207.8004f,   -570.6441f, 21.06016f,  1.762783f },
    { -221.4653f,   -549.9358f, 19.3898f,   3.211406f },
    { -229.9635f,   -559.2552f, 19.3898f,   0.0f },
    { -216.8438f,   -568.9011f, 21.06016f,  3.909538f },
    { -235.9045f,   -563.3906f, 19.3898f,   0.0f },
    { -226.6736f,   -580.8316f, 20.43056f,  2.775074f },
    { -227.5226f,   -595.1979f, 20.42358f,  4.206244f },
    { -215.0399f,   -576.3941f, 21.06016f,  3.735005f },
    { -210.592f,    -583.4739f, 21.06016f,  0.0f },
    { -228.592f,    -553.1684f, 19.3898f,   5.550147f },
    { -220.4879f,   -596.382f,  21.95116f,  0.0f },
    { -190.4774f,   -552.2778f, 51.31293f,  5.305801f },
    { -195.6267f,   -550.4393f, 51.31293f,  3.752458f },
    { -209.7257f,   -557.1042f, 51.31293f,  3.525565f },
    { -187.9531f,   -567.0469f, 51.31293f,  5.305801f },
    { -185.2083f,   -562.4844f, 19.3898f,   0.9599311f },
    { -183.4236f,   -577.2674f, 46.87183f,  3.944444f },
    { -184.6528f,   -572.3663f, 49.27317f,  3.159046f },
    { -191.2205f,   -581.4965f, 21.06015f,  2.007129f },
    { -201.224f,    -570.6788f, 21.06016f,  3.577925f },
    { -198.4653f,   -580.757f,  21.06015f,  0.8901179f },
    { -196.8767f,   -574.9688f, 21.06016f,  4.29351f },
    { -194.1302f,   -548.3055f, 19.3898f,   4.153883f },
    { -187.3333f,   -550.8143f, 19.3898f,   3.385939f },
    { -186.9792f,   -556.8472f, 19.3898f,   4.153883f },
    { -196.5504f,   -587.7031f, 21.06015f,  1.27409f },
    { -241.5938f,   -578.6858f, 19.3898f,   2.775074f },
    { -226.1615f,   -573.8021f, 20.40991f,  5.218534f }
};
Position const nightmareSkitterlingPos[] =
{
    { -184.9618f, -552.9132f, 19.3898f, 0.0f },
    { -199.1406f, -573.6893f, 21.06016f, 0.0f },
    { -194.5851f, -578.3386f, 21.06015f, 0.0f },
    { -193.3264f, -569.809f, 21.06016f, 0.0f },
    { -185.7622f, -566.6059f, 19.3898f, 0.0f },
    { -181.401f, -561.684f, 19.3898f, 0.0f },
    { -221.2569f, -549.3316f, 19.3898f, 0.0f },
    { -215.125f, -546.0486f, 19.3898f, 0.0f },
    { -210.3854f, -550.6736f, 19.3898f, 0.0f },
    { -206.8715f, -545.3837f, 19.3898f, 0.0f },
    { -208.3247f, -556.5833f, 19.3898f, 0.0f },
    { -203.1233f, -550.8281f, 19.3898f, 0.0f },
    { -198.2465f, -546.9288f, 19.3898f, 0.0f },
    { -178.6389f, -564.4479f, 19.39f, 0.0f },
    { -193.3698f, -581.8663f, 21.06015f, 0.0f },
    { -188.1788f, -578.3715f, 21.06015f, 0.0f },
    { -185.3385f, -562.592f, 19.3898f, 0.0f },
    { -179.6458f, -557.9063f, 19.3898f, 0.0f },
    { -226.5208f, -587.257f, 19.53274f, 0.0f },
    { -199.3351f, -586.7952f, 21.06015f, 0.0f },
    { -207.6545f, -594.809f, 21.06015f, 0.0f },
    { -201.2587f, -593.309f, 21.06016f, 0.0f },
    { -224.2743f, -592.7917f, 20.29579f, 0.0f },
    { -211.9844f, -569.5417f, 21.06016f, 0.0f },
    { -208.9288f, -563.6094f, 21.06016f, 0.0f },
    { -200.8715f, -557.2518f, 19.3898f, 0.0f },
    { -196.2396f, -553.6927f, 19.3898f, 0.0f },
    { -192.1163f, -550.1024f, 19.3898f, 0.0f },
    { -202.4063f, -563.1337f, 21.06016f, 0.0f },
    { -195.9635f, -560.8889f, 20.58139f, 0.0f },
    { -190.1094f, -557.4722f, 19.3898f, 0.0f },
    { -199.3229f, -550.3785f, 19.3898f, 0.0f },
    { -195.2552f, -557.0087f, 19.3898f, 0.0f },
    { -190.1372f, -553.0625f, 19.3898f, 0.0f },
    { -185.2118f, -557.6406f, 19.3898f, 0.0f },
    { -190.6406f, -561.441f, 19.3898f, 0.0f },
    { -197.7413f, -564.3021f, 21.06016f, 0.0f },
    { -215.5365f, -549.8021f, 19.3898f, 0.0f },
    { -212.1719f, -555.0226f, 19.3898f, 0.0f },
    { -213.2344f, -561.8577f, 21.06016f, 0.0f },
    { -208.5451f, -568.1146f, 21.06016f, 0.0f },
    { -202.7257f, -572.5174f, 21.06016f, 0.0f },
    { -205.6354f, -559.1563f, 21.06016f, 0.0f },
    { -206.1615f, -553.3507f, 19.3898f, 0.0f },
    { -202.5451f, -546.118f, 19.3898f, 0.0f },
    { -204.1024f, -596.7518f, 21.06016f, 0.0f },
    { -204.0087f, -590.8386f, 21.06016f, 0.0f },
    { -198.8316f, -591.7239f, 21.06015f, 0.0f },
    { -195.4375f, -586.2934f, 21.06015f, 0.0f },
    { -197.6493f, -582.4653f, 21.06015f, 0.0f },
    { -219.1337f, -559.7899f, 19.3898f, 0.0f },
    { -221.4063f, -553.1406f, 19.3898f, 0.0f },
    { -216.1215f, -564.882f, 21.06016f, 0.0f },
    { -199.3125f, -578.1996f, 21.06016f, 0.0f },
    { -204.684f, -569.3542f, 21.06016f, 0.0f },
    { -197.8576f, -569.3489f, 21.06016f, 0.0f },
    { -191.7656f, -565.8559f, 21.06016f, 0.0f },
    { -213.4635f, -591.625f, 21.06015f, 0.0f },
    { -206.0781f, -588.0712f, 21.06016f, 0.0f },
    { -213.1997f, -585.125f, 21.06015f, 0.0f },
    { -220.5174f, -581.8143f, 21.06016f, 0.0f },
    { -182.8004f, -569.368f, 19.39167f, 0.0f },
    { -189.0191f, -571.2674f, 21.06016f, 0.0f },
    { -193.7674f, -574.7014f, 21.06016f, 0.0f },
    { -226.1285f, -581.059f, 20.79114f, 0.0f },
    { -218.6076f, -581.0746f, 21.06016f, 0.0f },
    { -216.1094f, -576.8264f, 21.06016f, 0.0f },
    { -222.1545f, -555.1077f, 19.3898f, 0.0f },
    { -212.6059f, -567.2239f, 21.06016f, 0.0f },
    { -203.9184f, -551.0608f, 19.3898f, 0.0f },
    { -202.8663f, -590.1371f, 21.06016f, 0.0f },
    { -196.4757f, -573.0833f, 21.06016f, 0.0f },
    { -185.9288f, -558.125f, 19.3898f, 0.0f },
    { -213.8767f, -559.3125f, 20.02806f, 0.0f },
    { -221.8142f, -556.3976f, 19.3898f, 0.0f },
    { -216.5017f, -553.9722f, 19.3898f, 0.0f },
    { -223.7483f, -574.9896f, 21.06015f, 0.0f },
    { -226.1215f, -563.0382f, 19.3898f, 0.0f },
    { -213.8559f, -574.6094f, 21.06016f, 0.0f },
    { -208.3854f, -571.8004f, 21.06016f, 0.0f },
    { -224.5625f, -568.1476f, 19.92673f, 0.0f },
    { -219.7083f, -588.6094f, 21.06015f, 0.0f },
    { -218.8872f, -570.5643f, 21.06016f, 0.0f },
    { -221.8264f, -562.2795f, 19.3898f, 0.0f },
    { -227.9063f, -559.566f, 19.3898f, 0.0f },
    { -230.4948f, -566.0191f, 19.3898f, 0.0f },
    { -227.0243f, -552.6736f, 19.3898f, 0.0f },
    { -231.566f, -572.1979f, 19.3898f, 0.0f },
    { -231.8802f, -578.5677f, 19.3898f, 0.0f },
    { -232.4462f, -562.8837f, 19.3898f, 0.0f },
    { -232.4514f, -586.2518f, 19.3898f, 0.0f },
    { -230.3767f, -592.8577f, 19.3898f, 0.0f },
    { -232.8004f, -591.1805f, 19.3898f, 0.0f },
    { -234.5208f, -559.4809f, 19.3898f, 0.0f },
    { -236.5712f, -565.5573f, 19.3898f, 0.0f },
    { -237.5799f, -571.5903f, 19.3898f, 0.0f },
    { -238.9063f, -578.1528f, 19.3898f, 0.0f },
    { -234.0608f, -598.0f, 19.95063f, 0.0f },
    { -236.5122f, -591.9913f, 19.3898f, 0.0f },
    { -238.8021f, -584.8594f, 19.3898f, 0.0f },
};
Position const darkwebDevourerPos[] =
{
    { -215.7064f, -553.3212f, 51.29544f, 0.7243207f },
    { -229.285f, -568.6863f, 51.31313f, 0.8218399f },
    { -231.3453f, -584.1043f, 51.31514f, 1.05134f }
};
Position const chatteringHorrorPos = { -233.625f, -570.5868f, 19.3898f, 0.0f };
float const lightningPlatterHeight[] =
{
    14.28754f,  // -120.9898 Y: -638.3776 Z: 12.34728
    16.38484f,  // -135.3954 Y: -623.0512 Z: 14.44327
    16.18771f,  // -120.1581 Y: -617.6207 Z: 14.24549
    17.47415f,  // -136.7385 Y: -604.6659 Z: 15.53128
    20.52248f,  // -130.4809 Y: -586.5009 Z: 18.57832
    21.08911f   // -143.004  Y: -574.9209 Z: 19.1443
};
Position const lightningPlatterPos[] =
{
    { -120.9898f, -638.3776f, 12.34728f, 0.03183358f },
    { -135.3954f, -623.0512f, 14.44327f, 0.03133297f },
    { -120.1581f, -617.6207f, 14.24549f, 6.252115f   },
    { -136.7385f, -604.6659f, 15.53128f, 0.03079747f },
    { -130.4809f, -586.5009f, 18.57832f, 0.03022512f },
    { -143.004f, -574.9209f, 19.1443f,   6.253215f   }
};

uint8 const LIGHTNING_STALKER_COUNT = 8;
float const lightningStalkerOffset[LIGHTNING_STALKER_COUNT] = { 8.0f, 6.0f, 4.0f, 2.0f, -2.0f, -4.0f, -6.0f, -8.0f };

template<class AI>
CreatureAI* GetInstanceAI(Creature* creature)
{
    return GetInstanceAI<AI>(creature, "instance_deadmines");
}

#endif
