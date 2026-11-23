/*
* This file is part of the Legends of Azeroth Pandaria Project. See THANKS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DEF_HOUROFTWILIGHT_H
#define DEF_HOUROFTWILIGHT_H

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "InstanceScript.h"
#include "GameObject.h"
#include "Player.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellInfo.h"
#include "SpellScript.h"

#define HoTScriptName "instance_hour_of_twilight"

const Position drakePos = { 4175.16f, 559.51f, 56.02f, 5.72f };
const Position TwilightDrakePos = { 4061.84f, 287.63f, 100.46f, 4.44f };
const Position teleportPos = {3938.491211f, 274.222351f, 13.97f, 3.26f};
const Position AsiraSpawnPoint = { 4159.77f, 543.09f, 57.96f, 0.10f };
const Position AsiraHomePoint = { 4248.82f, 584.81f, -5.81f, 5.99f };
const Position ThrallLastSpawnPoint = { 3937.20f, 276.69f, 14.05f, 3.12f };
const Position BenedictusSpawnPoint = { 3674.36f, 288.69f, -116.85f, 0.14f };
const Position BenedictusWayPoint = { 3715.25f, 290.72f, -96.43f, 0.00f };
const Position ThrallWaypoints_ArcurionPrepare{ 4787.41f, 75.91f, 70.60f, 3.69f };
const Position BreakTheWall{ 4756.64f, 57.87f, 66.47f, 3.71f };
const Position WaveSpawnPoint = { 3586.25f, 277.84f, -115.96f, 3.28f };
const Position WaveDespawnPoint = { 3508.25f, 268.58f, -115.96f, 3.32f };


#define MAX_POINT_ACURION 5
#define MAX_POINT_ASIRA 10
#define ARCURION_PREPARE_MAX_POINTS 2
#define ASIRA_PREPARE_MAX_POINTS 11
#define TAXI_MAX_POINTS 6
#define TAXI_TWILIGHT_DRAKE_POINTS 3
#define BENEDICTUS_PREPARE_POINTS 7

const Position ThrallWaypoints_Arcurion[MAX_POINT_ACURION]
{
    { 4930.14f, 270.86f, 96.97f, 4.45f   }, // first puck
    { 4915.23f, 240.36f, 98.05f, 4.15f   }, // none
    { 4899.95f, 215.21f, 99.28f, 4.13f   }, // second puck
    { 4872.08f, 168.32f, 98.50f, 4.54f   }, // third puck
    { 4863.89f, 158.00f, 97.32f, 3.94f   }  // replace yourself
};

const Position ThrallWaypoints_Asira[MAX_POINT_ASIRA]
{
    { 4695.80f, 13.42f,  65.00f, 2.75f   },
    { 4663.93f, 40.25f,  72.26f, 2.31f   },
    { 4654.91f, 99.00f,  87.58f, 1.65f   },
    { 4648.78f, 163.32f, 98.07f, 1.71f   },
    { 4615.48f, 242.58f, 94.65f, 1.97f   },
    { 4598.53f, 333.23f, 95.78f, 1.75f   },
    { 4562.40f, 365.92f, 89.10f, 2.42f   },
    { 4529.28f, 377.32f, 79.22f, 2.72f   },
    { 4469.60f, 457.73f, 54.58f, 2.17f   },
    { 4405.47f, 464.76f, 35.84f, 6.13f   }
};

const Position ThrallWaypoints_Asira_Prepare[ASIRA_PREPARE_MAX_POINTS]
{
    { 4412.12f, 449.76f,  35.64f, 4.64f  },
    { 4391.82f, 427.86f,  18.86f, 3.96f  }, // first puck
    { 4365.76f, 404.60f, -1.02f,  3.83f  },
    { 4320.91f, 410.57f, -8.08f,  3.59f  }, // second puck
    { 4343.70f, 456.44f, -7.94f,  1.18f  },
    { 4315.92f, 475.98f, -7.73f,  2.75f  },  // 3td puck
    { 4327.09f, 555.10f, -7.03f,  1.48f  },
    { 4339.65f, 569.43f, -0.60f,  0.85f  }, // 4td puck
    { 4327.89f, 556.73f, -6.83f,  3.91f  },
    { 4265.67f, 562.72f, -6.45f,  3.10f  }, // 5td puck
    { 4285.24f, 574.14f, -6.55f,  3.06f  }
};

const Position ThrallWaypoints_Benedict_Prepare[BENEDICTUS_PREPARE_POINTS]
{
    { 3928.91f, 282.14f,  10.68f, 3.46f  },
    { 3904.46f, 269.57f,  4.44f,  2.66f  },
    { 3893.72f, 273.60f,  2.22f,  2.84f  }, // First puck
    { 3835.67f, 280.23f, -21.02f, 2.97f  }, // Second puck
    { 3814.06f, 289.69f, -37.45f, 2.83f  },
    { 3762.18f, 285.34f, -63.52f, 3.24f  },
    { 3755.89f, 286.08f, -68.50f, 3.02f  }, // 3td puck
};

const Position ThrallWaypoints_To_Dragon_Soul[BENEDICTUS_PREPARE_POINTS]
{
    { 3742.62f, 290.14f, -81.02f,  3.19f },
    { 3675.58f, 285.05f, -117.35f, 3.22f },
    { 3596.37f, 277.67f, -120.15f, 3.27f },
    { 3584.04f, 276.27f, -114.20f, 3.25f },
    { 3570.86f, 275.23f, -114.07f, 3.20f },
    { 3566.78f, 274.65f, -115.89f, 3.21f },
    { 3563.51f, 274.46f, -115.96f, 3.24f } // replace
};

const Position BenedictusWaypoints[BENEDICTUS_PREPARE_POINTS]
{
    { 3672.37f, 285.07f, -118.55f, 3.25f },
    { 3626.50f, 280.60f, -120.14f, 3.24f }, // light wall
    { 3594.96f, 277.66f, -119.57f, 3.25f },
    { 3583.92f, 276.25f, -114.15f, 3.26f },
    { 3570.90f, 274.93f, -114.04f, 3.26f },
    { 3566.86f, 274.29f, -115.81f, 3.22f },
    { 3546.82f, 272.99f, -115.96f, 3.24f }
};

const Position TaxiPathPrepare[TAXI_MAX_POINTS]
{
    { 4239.41f, 510.68f,  25.68f, 5.29f  },
    { 4287.26f, 506.68f,  15.63f, 0.01f  },
    { 4305.96f, 533.12f,  8.91f,  0.97f  },
    { 4300.87f, 558.06f,  3.54f,  1.90f  },
    { 4295.28f, 578.15f, -1.75f,  1.87f  },
    { 4288.53f, 592.06f, -6.64f,  3.69f  }
};

const Position TaxiPath[TAXI_MAX_POINTS]
{
    { 4233.90f, 546.65f, 33.43f, 3.28f   },
    { 4175.65f, 546.17f, 61.85f, 3.23f   },
    { 4083.24f, 527.79f, 81.23f, 3.42f   },
    { 4004.46f, 467.65f, 74.36f, 3.90f   },
    { 3934.47f, 364.32f, 61.76f, 4.68f   },
    { 3922.03f, 300.64f, 49.49f, 4.57f   }
};

const Position AsiraTaxiPath[TAXI_MAX_POINTS]
{
    { 4194.21f, 545.53f, 46.34f, 5.59f   },
    { 4250.63f, 502.91f, 24.01f, 0.03f   },
    { 4260.57f, 514.04f, 22.37f, 0.90f   },
    { 4235.15f, 564.88f, 18.12f, 2.06f   },
    { 4242.98f, 603.57f, 17.56f, 1.36f   },
    { 4280.00f, 619.16f, 11.69f, 5.12f   }
};

const Position TwilightDrakeTaxi[TAXI_TWILIGHT_DRAKE_POINTS]
{
    { 4032.76f, 251.09f, 86.93f, 4.03f   },
    { 3995.83f, 249.41f, 71.45f, 2.92f   },
    { 3963.12f, 277.78f, 60.54f, 2.51f   }
};

const Position TentacleSpawnPos[TAXI_TWILIGHT_DRAKE_POINTS]
{
    {3895.93f, 285.83f, 1.60f, 5.19f},
    { 3819.90f, 293.88f, -34.81f, 3.03f },
    { 3736.00f, 298.81f, -83.08f, 5.52f }
};

enum Datas
{
    DATA_ARCURION   = 0,
    DATA_ASIRA      = 1,
    DATA_BENEDICTUS = 2,
};

#define THRALL_INTRO "Yes, Thrall."
#define THRALL_ARCURION "Lets move!"
#define THRALL_ASIRA "We are ready."
#define THRALL_BEFORE_BENEDICTUS "Yes Thrall, lets do this!"

enum CreatureIds
{
    NPC_THRALL_1                         = 54548, // on start
    NPC_THRALL_2                         = 55779, // before Arcurion
    NPC_THRALL_3                         = 54972, // before Asira's mobs
    NPC_THRALL_4                         = 54634, // before last boss' mobs
    NPC_THRALL_5                         = 54971, // fighting with last boss
    NPC_LIFE_WARDEN_1                    = 55415, // for thrall
    NPC_LIFE_WARDEN_2                    = 55549, // for players
    NPC_RISING_FIRE_TOTEM                = 55474,

    NPC_ARCURION                         = 54590,
    NPC_ASIRA                            = 54968,
    NPC_BENEDICTUS                       = 54938,

    // triggers
    NPC_FROZEN_SERVITOR_STALKER_ENTRANCE = 55389,
    NPC_FROZEN_SERVITOR_SPAWN_STALKER    = 54560,
    NPC_FROZEN_SERVITOR_SPAWN_STALKER_2  = 55384,
    NPC_FROZEN_SERVITOR_SPAWN_POINTS     = 54598,
    NPC_ARCURION_SPAWN_VISIAL_STALKER    = 57197,
    NPC_ICE_WALL_EXIT_STALKER            = 55728,
    NPC_ASIRA_TRASH_STALKER_PH1          = 53488,
    NPC_DARK_HAZE_STALKER                = 54628,
    NPC_LIGHT_WALL_STALKER               = 54955,

    // Arcurion trash
    NPC_FROZEN_SERVITOR                  = 54600,
    NPC_CRYSTALLINE_ELEMENTAL            = 55559,
    NPC_ICY_TOMB                         = 54995,

    // Asira trash
    NPC_TWILIGHT_ASSASSIN                = 55106,
    NPC_TWILIGHT_BRUISER                 = 55112,
    NPC_TWILIGHT_RANGER                  = 55107,
    NPC_TWILIGHT_THUG                    = 55111,
    NPC_TWILIGHT_SHADOW_WALKER           = 55109,

    // Benedictus trash
    NPC_FACELESS_BRUTE                   = 54632,
    NPC_FACELESS_BORER                   = 54686,
    NPC_FACELESS_SHADOW_WEAVER           = 54633,
    NPC_FLAILING_TENTACLE                = 54696,
    NPC_CORRUPTED_SLIME                  = 54646,
};

enum GameObjectIds
{
    GO_PORTAL_TO_ORGRIMMAR               = 209081,
    GO_PORTAL_TO_STORMWIND               = 209080,

    GO_GATE                              = 190236,

    GO_ICEWALL_2                         = 210048, // after Arcurion
    GO_ICEWALL_1                         = 210049, // before Arcurion
};

// no change this value!
enum iActions
{
    ACTION_ARCURION                      = 4,
    ACTION_ASIRA                         = 5,
    ACTION_BENEDICTUS                    = 6,
    ACTION_ICY_TOMB                      = 7,
    ACTION_ICY_TOMB_REMOVE               = 8,
    ACTION_TALK_FINISH_HIM               = 9,
    ACTION_TALK_ARCURION_DEATH           = 10,
    ACTION_SUMMONED_ASIRA                = 11,
    ACTION_ASIRA_FALL                    = 12,
    ACTION_FLY_TO_DRAGON_SOUL            = 13,
    ACTION_REMOVE_ALL_PASSENGERS         = 14,
    ACTION_COMBAT                        = 15,
    ACTION_WATER_SHIELD                  = 16
};

#define DoomHammer 56228
#define MagharStaff 50805
#define ImmaculateRecoveryStaff 32344
#define AsiraDaggers 68600

enum OtherSpells
{
    SPELL_TELEPORT_ARCURION_DEATH        = 108928,
};

template<class AI>
inline AI* GetHoTAI(Creature* creature)
{
    return GetInstanceAI<AI>(creature, HoTScriptName);
}

#endif
