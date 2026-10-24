/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef GATE_SETTING_SUN_H_
#define GATE_SETTING_SUN_H_

uint32 const EncounterCount = 4;

Position const ElevatorCenterPos = { 1195.28f, 2304.54f, 430.85f, 3.07f };

enum DataTypes
{
    DATA_KIPTILAK               = 0,
    DATA_GADOK                  = 1,
    DATA_RIMOK                  = 2,
    DATA_RAIGONN                = 3,

    DATA_IN_FIGHT               = 4,

    DATA_OPEN_FIRST_DOOR        = 5,
    DATA_BRASIER_CLICKED        = 6,
    DATA_RANDOM_BOMBARDER       = 7,
    DATA_RANDOM_BOMB_STALKER    = 8,
    DATA_CORNER_A               = 10,
    DATA_CORNER_B               = 11,
    DATA_CORNER_C               = 12,
    DATA_EXPLOSION_CASTER       = 13,
    DATA_DEFENDER_A             = 14,
    DATA_DEFENDER_B             = 15,
    DATA_TRAINEE                = 16,
    DATA_FALL_EVENT             = 17,
    DATA_FALL_DEFENDERS         = 18,
    DATA_SIGNAL_FIRE            = 19,
    DATA_ROPE                   = 20,
    DATA_ARTILLERY_STATE        = 21,
    MAX_DATA
};

enum CreaturesIds
{
    NPC_KIPTILAK                = 56906,
    NPC_GADOK                   = 56589,
    NPC_RIMOK                   = 56636,
    NPC_RAIGONN                 = 56877,

    NPC_KRITHUK_BOMBARDER       = 56706,
    NPC_BOMB_STALKER            = 56709,
    NPC_EXPLOSION_TARGET_1      = 63992,
    NPC_EXPLOSION_TARGET_2      = 59721,
    NPC_EXPLOSION_TARGET_3      = 63991,
    NPC_EXPLOSION_CASTER        = 59723,
    NPC_KRITHIK_GLIDER          = 59885,
    NPC_FALL_DEFENDER           = 58202,
    NPC_KRITHIK_INFILTRATOR     = 58108,
    NPC_KRITHIK_WND_SHAPER      = 59801,
    NPC_KRITHIK_DEMOLISHER      = 56875,
    NPC_KRITHIK_SHAPER          = 56876,
    NPC_ARCHER_TARGET_SPAWNER   = 59886,
    NPC_ROPE                    = 64710,
    NPC_ROPE_SEAT               = 64740,
    NPC_CANNONEER               = 66905,
    NPC_SERPENTS_SPINE_DEFENDER = 65337,

    // Kip'Tilak
    NPC_STABLE_MUNITION         = 56917,

    NPC_EXPLOSION_BUNNY_N_M     = 56911,
    NPC_EXPLOSION_BUNNY_S_M     = 56918,
    NPC_EXPLOSION_BUNNY_E_M     = 56919,
    NPC_EXPLOSION_BUNNY_W_M     = 56920,

    NPC_EXPLOSION_BUNNY_N_P     = 59205,
    NPC_EXPLOSION_BUNNY_S_P     = 59206,
    NPC_EXPLOSION_BUNNY_E_P     = 59207,
    NPC_EXPLOSION_BUNNY_W_P     = 59208,

    // Gadok
    NPC_STALKER_NORTH_SOUTH     = 56932,
    NPC_STALKER_WEST_EAST       = 56913,

    NPC_KRIKTHIK_STRIKER        = 59778, // Appear on Gadok bombardment
    NPC_KRIKTHIK_DISRUPTOR      = 59794, // Bombard tower when Gadok is in fight
    NPC_SERPENTS_SPINE_TRAINEE  = 56745,
    NPC_SERPENTS_SPINE_DEF_A    = 56742,
    NPC_SERPENTS_SPINE_DEF_B    = 56743,

    // Rimok
    NPC_ADD_GENERATOR           = 59834,

    NPC_KRIKTHIK_SWARMER        = 59835,
    NPC_KRIKTHIK_SABOTEUR       = 60447,

    // Raigonn
    NPC_WEAK_SPOT               = 56895,

    NPC_KRIKTHIK_PROTECTORAT    = 56929,
    NPC_KRIKTHIK_ENGULFER       = 56912,
    NPC_KRIKTHIK_SWARM_BRINGER  = 56930,

    NPC_ARTILLERY               = 59819,
    NPC_ARTILLERY_TO_WALL       = 66904
};

enum ObjectsIds
{
    GO_KIPTILAK_ENTRANCE_DOOR   = 212982,
    GO_KIPTILAK_WALLS           = 214629,
    GO_KIPTILAK_MANTID_BOMBS    = 215757,
    GO_KIPTILAK_EXIT_DOOR       = 212983,

    GO_SIGNAL_FIRE              = 213507,

    GO_RIMAK_AFTER_DOOR         = 212985,

    GO_RAIGONN_DOOR             = 212986,
    GO_RAIGONN_AFTER_DOOR       = 214261,

    GO_GREATDOOR_SECOND_DOOR    = 214888,
    GO_WALL_C                   = 213198,
    GO_WALL_A                   = 213200,
    GO_WALL_B                   = 213199,
    GO_GREAT_DOOR_PHASE_1       = 211245,
    GO_GREAT_DOOR_PHASE_2       = 211246,
};

enum eSettingSun
{
    CINEMATIC_SETTING_SUN       = 265
};

Position const KiptilakInto[3] =
{
    { 721.697f, 2329.171f, 388.002f, 1.541f },
    { 697.205f, 2304.709f, 388.034f, 3.874f },
    { 746.597f, 2304.626f, 387.947f, 6.255f },
};

Position const SummonPositionsFallEvent[4] =
{
    { 857.320f, 2291.856f, 358.106f, 3.140f },
    { 855.458f, 2301.396f, 358.106f, 3.140f },
    { 854.624f, 2310.943f, 358.106f, 3.140f },
    { 855.435f, 2320.992f, 358.106f, 3.140f }
};

float const JumpPositionsFallEvent[4][3] =
{
    { 836.120f, 2297.170f, 381.559f },
    { 836.120f, 2304.519f, 381.559f },
    { 836.120f, 2311.398f, 381.559f },
    { 836.120f, 2317.552f, 381.526f }
};

Position const SummonPositionsRimok[8] =
{
    { 1330.749f, 2306.479f, 394.850f, 5.644f },
    { 1326.307f, 2293.109f, 394.410f, 6.257f },
    { 1298.777f, 2311.440f, 387.603f, 5.894f },
    { 1298.012f, 2297.214f, 387.091f, 0.035f },
    { 1304.326f, 2304.470f, 388.528f, 5.950f },
    { 1335.018f, 2299.159f, 395.390f, 5.843f },
    { 1328.948f, 2297.317f, 394.766f, 5.784f },
    { 1298.430f, 2301.514f, 387.297f, 5.941f }
};

Position const PandarenCannoneerPos[4] =
{
    { 1190.299f, 2336.800f, 388.251f, 4.714f },
    { 1199.780f, 2336.760f, 388.247f, 4.471f },
    { 1200.399f, 2272.929f, 388.246f, 1.665f },
    { 1190.160f, 2272.199f, 388.243f, 1.665f }
};

float const LeftCannoneerWPPath[24][3] =
{
    { 1188.123f, 2319.76f,  388.1612f },
    { 1178.495f, 2313.403f, 388.2357f },
    { 1168.852f, 2305.604f, 388.1071f },
    { 1150.936f, 2306.339f, 388.0673f },
    { 1140.854f, 2311.228f, 381.2508f },
    { 1124.925f, 2312.639f, 381.5792f },
    { 1111.425f, 2311.043f, 381.5591f },
    { 1098.46f,  2311.507f, 381.5591f },
    { 1088.493f, 2317.63f,  381.2259f },
    { 1080.297f, 2312.149f, 381.0464f }, // next jump
    { 1092.336f, 2275.795f, 330.807f  }, // after jump
    { 1091.542f, 2260.017f, 329.409f  },
    { 1091.541f, 2260.017f, 329.408f  },
    { 1090.706f, 2251.694f, 323.673f  },
    { 1091.095f, 2244.616f, 318.7957f },
    { 1091.061f, 2225.082f, 314.2758f },
    { 1086.462f, 2219.052f, 314.2747f },
    { 1059.326f, 2221.595f, 311.2115f },
    { 1047.208f, 2223.288f, 311.1616f },
    { 1038.675f, 2223.446f, 305.3143f },
    { 1032.524f, 2223.503f, 299.2118f },
    { 1034.601f, 2238.993f, 296.2305f },
    { 1044.92f,  2247.226f, 296.2305f },
    { 1055.559f, 2246.688f, 296.2305f }
};

float const RightCannoneerWPPath[27][3] =
{
    { 1063.392f, 2242.182f, 296.2305f },
    { 1176.13f,  2302.667f, 388.1546f },
    { 1168.965f, 2303.122f, 388.1074f },
    { 1150.594f, 2301.951f, 388.0614f },
    { 1132.359f, 2299.894f, 381.5436f },
    { 1113.762f, 2299.998f, 381.5844f },
    { 1102.575f, 2299.806f, 381.551f  },
    { 1096.025f, 2296.811f, 381.4274f },
    { 1091.085f, 2296.2f,   381.0473f }, // next jump -> move -> jump
    { 1094.237f, 2264.516f, 330.802f  },
    { 1094.345f, 2259.939f, 329.355f  },
    { 1094.346f, 2259.940f, 329.356f  },
    { 1094.418f, 2256.959f, 327.301f  },
    { 1094.345f, 2253.330f, 324.800f  },
    { 1094.250f, 2248.529f, 321.535f  },
    { 1094.153f, 2239.344f, 315.1621f },
    { 1093.646f, 2226.181f, 314.2743f },
    { 1079.102f, 2214.837f, 314.2777f },
    { 1064.37f,  2214.695f, 311.1351f },
    { 1047.549f, 2216.675f, 311.094f  },
    { 1038.167f, 2215.882f, 304.7759f },
    { 1028.646f, 2217.188f, 296.2305f },
    { 1022.743f, 2223.578f, 296.2305f },
    { 1025.12f,  2236.528f, 296.2305f },
    { 1033.986f, 2246.701f, 296.2305f },
    { 1050.47f,  2251.889f, 296.2305f },
    { 1061.459f, 2241.667f, 296.2305f }
};

#endif // GATE_SETTING_SUN_H_
