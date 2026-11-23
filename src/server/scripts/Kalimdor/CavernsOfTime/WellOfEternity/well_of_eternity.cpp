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

#include "Vehicle.h"
#include "well_of_eternity.h"

#define MAX_DOOMGUARDS 15

enum ScriptedTextNozdormu
{
    SAY_NOZDORMU_EVENT_1    = 0,
    SAY_NOZDORMU_INTRO_1    = 1,
    SAY_NOZDORMU_INTRO_2    = 2,
    SAY_NOZDORMU_INTRO_3    = 3,
    SAY_NOZDORMU_INTRO_4    = 4,
};

enum ScriptedTextIllidan1
{
    SAY_ILLIDAN_1_INTRO_1       = 24, // Over here, in the shadows.
    SAY_ILLIDAN_1_INTRO_2       = 35, // I think we stand a better chance fighting alongside one another.

    SAY_ILLIDAN_1_INTRO_IDLE_1  = 25, // Explain your presence.
    SAY_ILLIDAN_1_INTRO_IDLE_2  = 36, // Talk with me now.
    SAY_ILLIDAN_1_INTRO_IDLE_3  = 37, // Are you here to help?

    SAY_ILLIDAN_1_MOVE_1        = 6,  // We now hide in shadows, hidden from our enemies.
    SAY_ILLIDAN_1_MOVE_2        = 13, // Come with me if you'd like to live long enough to see me save this world!
    SAY_ILLIDAN_1_MOVE_3        = 14, // So many demons, not enough time.
    SAY_ILLIDAN_1_MOVE_4        = 16, // I've seen a single Guardian Demon slaughter a hundred elves. Tread lightly.
    SAY_ILLIDAN_1_MOVE_5        = 11, // I will hold them back so we can get past. Be ready.
    SAY_ILLIDAN_1_MOVE_6        = 12, // My magic is fading. I'm going through!

    SAY_ILLIDAN_1_MOVE_7        = 17, // They come endlessly from the palace.

    SAY_ILLIDAN_1_MOVE_8        = 18, // The stench of sulfur and brimstone... These portals are as foul as the demons themselves.
    SAY_ILLIDAN_1_MOVE_9        = 19, // Cut this one down from the shadows.
    SAY_ILLIDAN_1_MOVE_10       = 20, // Let us shut down this final portal and finish this.

    SAY_ILLIDAN_1_MOVE_11       = 21, // The demons should all be leaving. We will be at the palace in no time.
    SAY_ILLIDAN_1_MOVE_12       = 22, // The demons are no longer pouring from the palace. We can move ahead.
    SAY_ILLIDAN_1_MOVE_13       = 23, // Too easy.

    SAY_ILLIDAN_1_WAIT_1        = 26, // Attack. I don't like to be kept waiting.
    SAY_ILLIDAN_1_WAIT_2        = 27, // I'll let you have the first kill. Don't make me regret that.
    SAY_ILLIDAN_1_WAIT_3        = 28, // Patience is not one of my virtues.

    SAY_ILLIDAN_1_PRECOMBAT_1   = 29, // Waiting to attack...
    SAY_ILLIDAN_1_PRECOMBAT_2   = 30, // They stand unaware that their deaths are moments away.

    SAY_ILLIDAN_1_POSTCOMBAT    = 7,  // We're leaving. Stay close.

    SAY_ILLIDAN_1_AGGRO_1       = 10, // Death to the Legion!
    SAY_ILLIDAN_1_AGGRO_2       = 8,  // My blades hunger.

    SAY_ILLIDAN_1_CRYSTAL_1     = 32, // Destroy the crystal so we can move on.
    SAY_ILLIDAN_1_CRYSTAL_2     = 33, // Smash the crystal. We need to move.
    SAY_ILLIDAN_1_CRYSTAL_3     = 34, // Destroy the portal energy focus!
    SAY_ILLIDAN_1_CRYSTAL_4     = 38, // Shut down the portal, and we will continue.

    SAY_ILLIDAN_1_BOSS_PRE      = 2,  // Another demon, ready to be slaughtered.
    SAY_ILLIDAN_1_BOSS_AGGRO    = 1,  // Nothing will stop me. Not even you, demon.
    SAY_ILLIDAN_1_BOSS_HIDES    = 0,  // Return to the shadows!
    SAY_ILLIDAN_1_BOSS_SPELL    = 5,  // Your magic is pathetic. Let me show you mine.
    SAY_ILLIDAN_1_BOSS_RECOVERY = 31, // My strength returns.

    SAY_ILLIDAN_1_OUTRO_1       = 3,  // The hunter became the prey.
    SAY_ILLIDAN_1_OUTRO_2       = 4,  // You did well, but for now I must continue alone. Good hunting.

    SAY_ILLIDAN_1_AZZINOTH_1    = 39, // Curious weapons you wield... such power.
    SAY_ILLIDAN_1_AZZINOTH_2    = 40, // You seem prepared.
};

enum ScriptedTextIllidan2
{
    SAY_ILLIDAN_2_INTRO_1       = 4,
    SAY_ILLIDAN_2_INTRO_2       = 5,
    SAY_ILLIDAN_2_INTRO_3       = 6,
    SAY_ILLIDAN_2_DEMONS        = 8,
    SAY_ILLIDAN_2_ELEMENTAL_1   = 9,
    SAY_ILLIDAN_2_ELEMENTAL_2   = 10,
    SAY_ILLIDAN_2_ELEMENTAL_3   = 11,
    SAY_ILLIDAN_2_ELEMENTAL_4   = 12,
    SAY_ILLIDAN_2_VAROTHEN      = 14,
    SAY_ILLIDAN_2_SWORD         = 15,
    SAY_ILLIDAN_2_BUFF_1        = 16,
    SAY_ILLIDAN_2_BUFF_2        = 17,
    SAY_ILLIDAN_2_BUFF_3        = 18,
    SAY_ILLIDAN_2_SPELL         = 19,
};

enum ScriptedTextMalfurion
{
    SAY_MALFURION_INTRO_1   = 4,
    SAY_MALFURION_INTRO_2   = 5,
};

enum ScriptedTextTyrande
{
    SAY_TYRANDE_INTRO_1     = 3,
    SAY_TYRANDE_INTRO_2     = 4,
    SAY_TYRANDE_ELEMENTAL   = 5,
    SAY_TYRANDE_DEMONS      = 7,
    SAY_TYRANDE_ELUNE       = 8,
    SAY_TYRANDE_CONTINUE    = 9,
    SAY_TYRANDE_ARROWS      = 10,
    SAY_TYRANDE_NO_1        = 11,
    SAY_TYRANDE_NO_2        = 12,
    SAY_TYRANDE_MALFURION   = 13,
    SAY_TYRANDE_MANY_DEMONS = 14,
};

enum Spells
{
    // Legion Demon
    SPELL_STRIKE_FEAR           = 103913,
    SPELL_CRUSHING_LEAP         = 108474,
    SPELL_CRUSHING_LEAP_DMG     = 108479,
    SPELL_CRUSHING_LEAP_VISUAL  = 108481,
    SPELL_SUMMON_FIRE_WALL      = 105243,
    SPELL_FIREWALL_COSMETIC     = 105250,
    SPELL_CAMOUFLAGE            = 105341,

    // Preevent
    SPELL_RETURN_TO_THE_SHADOWS = 105635,
    SPELL_SHADOWCLOAK_ILLIDAN   = 105915,
    SPELL_SHADOWCLOAK_PLAYERS   = 103004,
    SPELL_SHADOWCLOAK_PETS      = 110231,
    SPELL_SHADOW_WALK_AURA      = 102994,
    SPELL_SHADOW_WALK_STACKS    = 103020,
    SPELL_SHADOW_AMBUSHER       = 103018,
    SPELL_SHADOW_AMBUSHER_BUFF  = 103420,
    SPELL_SHADOWCLOAK_AGGRO     = 103530,
    SPELL_MARCHING_DEMON_REACT  = 105721,
    SPELL_MARCHING_DEMON_SOUND  = 110249,
    SPELL_AGGRO_ILLIDAN         = 104683,
    SPELL_WALL_OF_SHADOW        = 104400,
    SPELL_DISTRACT_MISSILE      = 110121,
    SPELL_DISTRACT_DEMONS_AURA  = 110082,
    SPELL_DEMON_GRIP            = 102561,
    SPELL_DEMON_GRIP_ROOT       = 102937,

    // Portal trash
    SPELL_ARCANE_ANNIHILATION   = 107865,
    SPELL_INFINITE_MANA         = 107880,
    SPELL_CARRION_SWARM         = 107840,
    SPELL_DEMONIC_WARDING       = 107900,

    // Portals
    SPELL_PORTAL_STATUS_ACTIVE              = 102456,
    SPELL_PORTAL_STATUS_SHUTTING_DOWN       = 102457,
    SPELL_PORTAL_SHUTDOWN_REQUEST           = 102609,
    SPELL_PORTAL_BEAM_BLUE_CONNECTOR        = 103957,
    SPELL_PORTAL_BEAM_BLUE_SELF             = 103984,
    SPELL_PORTAL_BEAM_GREEN_CONNECTOR       = 104034,
    SPELL_PORTAL_BEAM_GREEN_SELF            = 104038,
    SPELL_PORTAL_BEAM_RED_CONNECTOR         = 104039,
    SPELL_PORTAL_BEAM_RED_SELF              = 104040,
    SPELL_PORTAL_CONNECTOR_DEAD             = 105201,
    SPELL_FEL_CRYSTAL_ARC_MELTDOWN          = 105074,
    SPELL_FEL_CRYSTAL_STALKER_DESTRUCTION   = 105079,
    SPELL_FEL_CRYSTAL_STALKER_DESTROYED     = 105119,
    SPELL_FEL_CRYSTAL_STALKER_PERIODIC      = 105055,

    // Eternal Champion
    SPELL_QUEENS_BLADE          = 102260,
    SPELL_SHIMMERING_STRIKE     = 102257,

    // Eye of Legion
    SPELL_FEL_FLAMES            = 102356,

    // Enchanted Highmistress
    SPELL_FIREBALL              = 102265,
    SPELL_FIREBOMB              = 102263,
    SPELL_FROSTBOLT             = 102266,
    SPELL_BLIZZARD              = 102267,

    // Royal Handmaiden
    SPELL_SWEET_LULLABY         = 120245,

    // Illidan 2
    SPELL_WATERS_OF_ETERNITY    = 103952,
    SPELL_TAUNT                 = 104461,
    SPELL_DEMON_RUSH            = 104205,
    SPELL_DARKLANCE             = 104394,
    SPELL_AURA_OF_IMMOLATION    = 104379,
    SPELL_GIFT_OF_SARGERAS      = 104998,
    SPELL_GIFT_OF_SARGERAS_AOE  = 105009,

    // Tyrande
    SPELL_BLESSING_OF_ELUNE     = 103917,
    SPELL_HAND_OF_ELUNE         = 105072,
    SPELL_WRATH_OF_ELUNE_1      = 105073, // 30k damage
    SPELL_WRATH_OF_ELUNE_2      = 105075, // 300k damage
    SPELL_LUNAR_SHOT_1          = 104214, // 30k damage, single
    SPELL_LUNAR_SHOT_2          = 104313, // 300k damage, single
    SPELL_LUNAR_SHOT_3          = 104688, // 300k damage, aoe

    // Abyssal Doombringer
    SPELL_ABYSSAL_FLAMES        = 103992,

    // Doomguard Debilitator
    SPELL_DEBILITATING_FLAY         = 104678,
};

enum Events
{
    // Nozdormu
    EVENT_NOZDORMU_TALK_1   = 1,
    EVENT_NOZDORMU_TALK_2,
    EVENT_NOZDORMU_TALK_3,
    EVENT_NOZDORMU_TALK_4,

    // Intro
    EVENT_INTRO_TALK_1,
    EVENT_INTRO_TALK_2,
    EVENT_INTRO_TALK_3,
    EVENT_INTRO_TRASH_LEAVE,
    EVENT_INTRO_LEGION_DEMON_MOVE,
    EVENT_INTRO_CLOSE_WALL_1,
    EVENT_INTRO_CLOSE_WALL_2,
    EVENT_INTRO_CLOSE_WALL_3,
    EVENT_INTRO_CLOSE_WALL_4,

    // Legion Demon
    EVENT_STRIKE_FEAR,
    EVENT_CRUSHING_LEAP,

    // Illidan 1
    EVENT_ILLIDAN_1_WIPE_CHECK,
    EVENT_ILLIDAN_1_RESPAWN,
    EVENT_ILLIDAN_1_INITIAL_CLOAK,
    EVENT_ILLIDAN_1_NAGGING,
    EVENT_ILLIDAN_1_INTRO_1,
    EVENT_ILLIDAN_1_INTRO_2,
    EVENT_ILLIDAN_1_MOVE_1,
    EVENT_ILLIDAN_1_MOVE_2,
    EVENT_ILLIDAN_1_MOVE_6,
    EVENT_ILLIDAN_1_SAY_WAIT,
    EVENT_ILLIDAN_1_BOSS_SPAWN,
    EVENT_ILLIDAN_1_OUTRO_1,
    EVENT_ILLIDAN_1_AZZINOTH_2,

    // Portal trash
    EVENT_PERIODIC_SPELL,

    // Preevent
    EVENT_MARCHING_DEMON_SOUND,
    EVENT_GUARDIAN_FLEE,
    EVENT_SHUTDOWN_CHECK,
    EVENT_PORTAL_CONNECTOR_RECONNECT,
    EVENT_FEL_CRYSTAL_STALKER_GUARDS,
    EVENT_FEL_CRYSTAL_STALKER_DESTRUCTION,
    EVENT_FEL_CRYSTAL_STALKER_DESTROYED,

    // Eternal Champion
    EVENT_QUEENS_BLADE,
    EVENT_SHIMMERING_STRIKE,

    // Eye of Legion
    EVENT_FEL_FLAMES,

    // Enchanted Highmistress
    EVENT_FIREBALL,
    EVENT_FIREBOMB,
    EVENT_FROSTBOLT,
    EVENT_BLIZZARD,

    // Royal Handmaiden
    EVENT_SWEET_LULLABY,

    // Illidan 2
    EVENT_MALFURION_INTRO_1,
    EVENT_MALFURION_INTRO_2,
    EVENT_ILLIDAN_INTRO_2_2,
    EVENT_TYRANDE_INTRO_1,
    EVENT_ILLIDAN_INTRO_2_3,
    EVENT_TYRANDE_INTRO_2,
    EVENT_ILLIDAN_MOVE_2_1,
    EVENT_ILLIDAN_MOVE_2_2,
    EVENT_ILLIDAN_MOVE_2_3,
    EVENT_ILLIDAN_MOVE_2_4,
    EVENT_ILLIDAN_MOVE_2_5,
    EVENT_ILLIDAN_MOVE_2_6,
    EVENT_ILLIDAN_NEAR_ELEMENTAL_1,
    EVENT_ILLIDAN_NEAR_ELEMENTAL_2,
    EVENT_TYRANDE_NEAR_ELEMENTAL_1,
    EVENT_ILLIDAN_NEAR_ELEMENTAL_3,
    EVENT_ILLIDAN_NEAR_ELEMENTAL_4,
    EVENT_ILLIDAN_NEAR_ELEMENTAL_5,
    EVENT_ILLIDAN_MOVE_2_7,
    EVENT_ILLIDAN_MOVE_2_8,
    EVENT_ILLIDAN_MOVE_2_9,
    EVENT_TYRANDE_MOVE_1,
    EVENT_TYRANDE_MOVE_2,
    EVENT_TYRANDE_SHOT,
    EVENT_HAND_OF_ELUNE,

    // Abyssal Doombringer
    EVENT_ABYSSAL_FLAMES,
};

enum Adds
{
    NPC_NOZDORMU                    = 55624,
    NPC_LEGION_DEMON                = 55503,

    NPC_LEGION_DEMON_PREEVENT       = 54500,
    NPC_GUARDIAN_DEMON              = 54927,
    NPC_LEGION_PORTAL               = 54513,
    NPC_PORTAL_CONNECTOR_1          = 55541,
    NPC_PORTAL_CONNECTOR_2          = 55542,
    NPC_PORTAL_CONNECTOR_3          = 55543,
    NPC_CORRUPTED_ARCANIST          = 55654,
    NPC_DREADLORD_DEFENDER          = 55656,
    NPC_FEL_CRYSTAL                 = 55917,
    NPC_FEL_CRYSTAL_STALKER         = 55965,
    NPC_SHADOWCLOAK_STALKER         = 55154,
    NPC_SHADOWCLOAK_ILLIDAN_STALKER = 56389,
    NPC_WELL_OF_ETERNITY_STALKER    = 54506,

    // guardian demon 54927

    NPC_ETERNAL_CHAMPION            = 54612,
    NPC_EYE_OF_LEGION               = 54747,
    NPC_ENCHANTED_HIGHMISTRESS_1    = 56579,
    NPC_ENCHANTED_HIGHMISTRESS_2    = 54589,
    NPC_ROYAL_HANDMAIDEN            = 54645,

    // isera 55393
    // alextraza 55394
    // bronze dragon 55391
    // shadowbat 57458
    // nozdormu 56102
    // soridormi 55395
    // red dragon 55366
    // green dragon 55390
    // the dragon soul 55078 aura 102845
    // portal to twisting nether 56087 aura 102918
    // shadowbat + highguard elite 55463 + 55426
};

enum Points
{
    POINT_ILLIDAN_1_INTRO_1         = 0,
    POINT_ILLIDAN_1_MOVE_3          = 1,
    POINT_ILLIDAN_1_MOVE_4          = 2,
    POINT_ILLIDAN_1_MOVE_5          = 5,
    POINT_ILLIDAN_1_MOVE_5_STOP     = 6,
    POINT_ILLIDAN_1_MOVE_6_CRYSTAL  = 7,
    POINT_ILLIDAN_1_MOVE_7          = 10,
    POINT_ILLIDAN_1_MOVE_7_CRYSTAL  = 15,
    POINT_ILLIDAN_1_MOVE_8          = 16,
    POINT_ILLIDAN_1_MOVE_9          = 22,
    POINT_ILLIDAN_1_MOVE_9_STOP     = 23,
    POINT_ILLIDAN_1_MOVE_10         = 24,
    POINT_ILLIDAN_1_MOVE_10_CRYSTAL = 26,
    POINT_ILLIDAN_1_MOVE_11         = 29,
    POINT_ILLIDAN_1_MOVE_12         = 33,
    POINT_ILLIDAN_1_MOVE_13         = 35,
    POINT_ILLIDAN_1_BOSS            = 38,
    POINT_ILLIDAN_1_OUTRO_2         = 40,
    POINT_ILLIDAN_1_OUTRO_JUMP_FROM = 46,
    POINT_ILLIDAN_1_OUTRO_JUMP_TO   = 47,
    POINT_ILLIDAN_1_OUTRO_DESPAWN   = 50,

    POINT_ILLIDAN_2_1   = 7,
    POINT_ILLIDAN_2_2   = 8,
    POINT_ILLIDAN_2_3   = 9,
    POINT_ILLIDAN_2_4   = 10,
    POINT_ILLIDAN_2_5   = 11,
    POINT_ILLIDAN_2_6   = 12,
    POINT_ILLIDAN_2_7   = 13,
    POINT_ILLIDAN_2_8   = 14,
    POINT_ILLIDAN_2_9   = 15,
    POINT_ILLIDAN_2_10  = 16,

    POINT_TYRANDE_1     = 17,
    POINT_TYRANDE_2     = 18,
    POINT_TYRANDE_3     = 19,
};

enum Actions
{
    ACTION_PEROTHARN_DEAD       = 1,
    ACTION_DOOMGUARD_DIED,
    ACTION_DOOMBRINGER_DIED,
    ACTION_TYRANDE_MOVE_1,
    ACTION_TYRANDE_MOVE_2,
    ACTION_MANNOROTH_AGGRO,
    ACTION_MANNOROTH_RESET,
    ACTION_DEBILITATING_OFF,
    ACTION_MANNOROTH_SARGERAS,
    ACTION_MANNOROTH_INFERNO,
    ACTION_MANNOROTH_END,

    ACTION_NOZDORMU_SKIP,
    ACTION_LEGION_DEMON_START,
    ACTION_INTRO_TRASH_LEAVE,
    ACTION_PORTAL_ACTIVE,
    ACTION_PORTAL_DISABLE,
    ACTION_FEL_CRYSTAL_STALKER_UNLOCK,
    ACTION_FEL_CRYSTAL_STALKER_MELTDOWN,
    ACTION_ILLIDAN_1_STOP_NAGGING,
    ACTION_ILLIDAN_1_PRESTART,
    ACTION_ILLIDAN_1_START,
    ACTION_ILLIDAN_1_CRYSTAL,
    ACTION_ILLIDAN_1_CRYSTAL_RESUME,
    ACTION_ILLIDAN_1_LEGION_DEMON_RESUME,
    ACTION_ILLIDAN_1_OUTRO_RESUME,
    ACTION_ILLIDAN_1_SKIP,
};

enum GameObjects
{
    GO_PORTAL_ENERGY_FOCUS_1    = 209366,
    GO_PORTAL_ENERGY_FOCUS_2    = 209447,
    GO_PORTAL_ENERGY_FOCUS_3    = 209448,
};

const Position illidan1Pos[] =
{
    { 3194.18f, -4893.46f, 194.35f, 0 }, // 00 INTRO
    { 3209.33f, -4895.08f, 194.34f, 0 }, // 01 MOVE_3
    { 3216.41f, -4895.89f, 194.18f, 0 }, // 02 MOVE_4
    { 3244.28f, -4899.36f, 180.98f, 0 }, // 03
    { 3253.35f, -4901.05f, 181.07f, 0 }, // 04
    { 3282.42f, -4899.89f, 181.07f, 0 }, // 05 MOVE_5
    { 3293.63f, -4901.82f, 181.07f, 0 }, // 06 MOVE_5_STOP
    { 3295.33f, -4982.47f, 181.07f, 0 }, // 07 MOVE_6_CRYSTAL
    { 3321.19f, -4996.78f, 181.07f, 0 }, // 08
    { 3333.38f, -5001.95f, 181.07f, 0 }, // 09
    { 3363.50f, -4993.62f, 181.07f, 0 }, // 10 MOVE_7
    { 3399.23f, -4978.25f, 181.07f, 0 }, // 11
    { 3406.35f, -4955.47f, 181.07f, 0 }, // 12
    { 3406.35f, -4955.47f, 181.07f, 0 }, // 13
    { 3410.58f, -4930.65f, 181.07f, 0 }, // 14
    { 3442.37f, -4887.15f, 181.07f, 0 }, // 15 MOVE_7_CRYSTAL
    { 3411.30f, -4864.23f, 181.07f, 0 }, // 16 MOVE_8
    { 3381.83f, -4831.82f, 181.07f, 0 }, // 17
    { 3383.31f, -4816.08f, 180.98f, 0 }, // 18
    { 3395.11f, -4791.74f, 194.18f, 0 }, // 19
    { 3398.64f, -4782.25f, 194.14f, 0 }, // 20
    { 3420.58f, -4768.12f, 194.14f, 0 }, // 21
    { 3434.49f, -4774.03f, 189.73f, 0 }, // 22 MOVE_9
    { 3447.59f, -4795.88f, 189.73f, 0 }, // 23 MOVE_9_STOP
    { 3456.04f, -4813.35f, 189.73f, 0 }, // 24 MOVE_10
    { 3461.63f, -4823.73f, 194.17f, 0 }, // 25
    { 3471.12f, -4839.83f, 194.21f, 0 }, // 26 MOVE_10_CRYSTAL
    { 3461.63f, -4823.73f, 194.17f, 0 }, // 27
    { 3456.04f, -4813.35f, 189.73f, 0 }, // 28
    { 3447.59f, -4795.88f, 189.73f, 0 }, // 29 MOVE_11
    { 3434.49f, -4774.03f, 189.73f, 0 }, // 30
    { 3420.58f, -4768.12f, 194.14f, 0 }, // 31
    { 3398.64f, -4782.25f, 194.14f, 0 }, // 32
    { 3395.11f, -4791.74f, 194.18f, 0 }, // 33 MOVE_12
    { 3383.31f, -4816.08f, 180.98f, 0 }, // 34
    { 3365.80f, -4851.31f, 181.07f, 0 }, // 35 MOVE_13
    { 3361.35f, -4863.21f, 181.07f, 0 }, // 36
    { 3332.09f, -4884.56f, 181.07f, 0 }, // 37
    { 3333.44f, -4886.74f, 181.07f, 0 }, // 38 BOSS
    { 3376.99f, -4954.95f, 181.07f, 0 }, // 39
    { 3376.20f, -4953.67f, 181.07f, 0 }, // 40 OUTRO_2
    { 3377.40f, -4956.33f, 181.31f, 0 }, // 41
    { 3390.24f, -4977.10f, 196.78f, 0 }, // 42
    { 3425.10f, -5036.94f, 196.78f, 0 }, // 43
    { 3440.27f, -5067.09f, 213.59f, 0 }, // 44
    { 3446.79f, -5084.66f, 213.59f, 0 }, // 45
    { 3444.20f, -5086.65f, 213.59f, 0 }, // 46 OUTRO_JUMP_FROM
    { 3425.48f, -5101.57f, 181.07f, 0 }, // 47 OUTRO_JUMP_TO
    { 3409.98f, -5129.06f, 181.07f, 0 }, // 48
    { 3404.55f, -5163.91f, 181.08f, 0 }, // 49
    { 3409.17f, -5193.60f, 183.91f, 0 }, // 50 OUTRO_DESPAWN
};

const Position illidan2Pos[10] =
{
    {3159.834961f, -5572.989746f, 18.292067f, 0.0f},
    {3168.384766f, -5581.995605f, 18.055605f, 0.0f},
    {3177.702148f, -5598.211426f, 17.121992f, 0.0f},
    {3184.541748f, -5616.823242f, 17.969137f, 0.0f},
    {3192.839844f, -5638.349121f, 18.085392f, 0.0f},
    {3205.317871f, -5666.366211f, 18.083393f, 0.0f},  // near elemental
    {3219.282227f, -5683.890137f, 18.111153f, 0.0f},  // attack elemental
    {3259.732178f, -5703.202637f, 17.23f,     6.15f}, // after elemental
    {3281.667236f, -5706.658203f, 16.42f,     6.09f}, // near varo'then
    {3339.285889f, -5730.857910f, 15.23f,     5.64f}  // near mannoroth
};

const Position tyrandePos = {3310.190674f, -5691.714355f, 14.78f, 1.78f};

class npc_well_of_eternity_stalker : public CreatureScript
{
    public:
        npc_well_of_eternity_stalker() : CreatureScript("npc_well_of_eternity_stalker") { }

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetWoEAI<npc_well_of_eternity_stalkerAI>(creature);
        }

        struct npc_well_of_eternity_stalkerAI : public ScriptedAI
        {
            npc_well_of_eternity_stalkerAI(Creature* creature) : ScriptedAI(creature) { }

            void UpdateAI(uint32 /*diff*/) OVERRIDE { }
        };
};

class npc_well_of_eternity_nozdormu : public CreatureScript
{
    public:
        npc_well_of_eternity_nozdormu() : CreatureScript("npc_well_of_eternity_nozdormu") { }

        struct npc_well_of_eternity_nozdormuAI : public ScriptedAI
        {
            npc_well_of_eternity_nozdormuAI(Creature* creature) : ScriptedAI(creature)
            {
                bTalk = false;
                instance = me->GetInstanceScript();
                npc_id = ((me->GetPositionX() > 3200.0f) ? 1 : 0);
            }

            void Reset() OVERRIDE
            {
                events.Reset();
                /*if (npc_id == 1)
                    if (instance && instance->GetData(DATA_EVENT_NOZDORMU) == DONE)
                        me->DespawnOrUnsummon(500);*/
            }

            void MoveInLineOfSight(Unit* who) OVERRIDE
            {
                if (npc_id != 1 || bTalk || !instance)
                    return;

                if (instance->GetData(DATA_EVENT_NOZDORMU) == DONE)
                    return;

                if (who->GetTypeId() != TypeID::TYPEID_PLAYER)
                    return;

                if (who->ToPlayer()->isBeingLoaded() || who->ToPlayer()->IsBeingTeleported())
                    return;

                if (me->GetDistance(who) > 5.0f)
                    return;

                bTalk = true;

                events.ScheduleEvent(EVENT_NOZDORMU_TALK_1, 5000);

                instance->SetData(DATA_EVENT_NOZDORMU, IN_PROGRESS);
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action != ACTION_NOZDORMU_SKIP)
                    return;

                events.Reset();
                bTalk = true;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!bTalk)
                    return;

                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_NOZDORMU_TALK_1:
                            Talk(SAY_NOZDORMU_INTRO_1);
                            events.ScheduleEvent(EVENT_NOZDORMU_TALK_2, 12000);
                            break;
                        case EVENT_NOZDORMU_TALK_2:
                            Talk(SAY_NOZDORMU_INTRO_2);
                            events.ScheduleEvent(EVENT_NOZDORMU_TALK_3, 7000);
                            break;
                        case EVENT_NOZDORMU_TALK_3:
                            Talk(SAY_NOZDORMU_INTRO_3);
                            events.ScheduleEvent(EVENT_NOZDORMU_TALK_4, 14000);
                            break;
                        case EVENT_NOZDORMU_TALK_4:
                            Talk(SAY_NOZDORMU_INTRO_4);
                            instance->SetData(DATA_EVENT_NOZDORMU, DONE);
                            //me->DespawnOrUnsummon(500);
                            break;
                    }
                }
            }

        private:
            bool bTalk;
            uint8 npc_id; // 1 - near entrance
            EventMap events;
            InstanceScript* instance;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetWoEAI<npc_well_of_eternity_nozdormuAI>(creature);
        }
};

class npc_well_of_eternity_legion_demon : public CreatureScript
{
    public:
        npc_well_of_eternity_legion_demon() : CreatureScript("npc_well_of_eternity_legion_demon") { }

        struct npc_well_of_eternity_legion_demonAI : public ScriptedAI
        {
            npc_well_of_eternity_legion_demonAI(Creature* creature) : ScriptedAI(creature)
            {
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
                instance = me->GetInstanceScript();
                secondDemon = me->GetPositionX() > 3200.0f;
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                me->ApplySpellImmune(0, IMMUNITY_ID, SPELL_SHADOWCLOAK_AGGRO, true);
                me->ApplySpellImmune(0, IMMUNITY_ID, SPELL_MARCHING_DEMON_REACT, true);
            }

            void Reset() OVERRIDE
            {
                events.Reset();
                if (secondDemon)
                {
                    if (instance && instance->GetData(DATA_EVENT_DEMON_2) == DONE)
                        me->DespawnOrUnsummon(500);
                }
                else
                {
                    if (instance && instance->GetData(DATA_EVENT_DEMON) == DONE)
                        me->DespawnOrUnsummon(500);
                }
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_STRIKE_FEAR, urand(5000, 7000));
                //DoCast(who, SPELL_CRUSHING_LEAP);
                if (secondDemon)
                    DoCastAOE(SPELL_AGGRO_ILLIDAN, true);
            }

            void SpellHit(Unit* caster, const SpellInfo* spell) OVERRIDE
            {
                if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                    return;
                if (spell->Id == SPELL_ARCHIVED_DEMON_1)
                    DoCast(caster, SPELL_ARCHIVED_DEMON_2, true);
                else if (spell->Id == SPELL_SHADOWCLOAK_AGGRO)
                {
                    caster->RemoveAura(SPELL_SHADOW_WALK_AURA);
                    AttackStart(caster);
                }
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (pointId == EVENT_JUMP)
                {
                    DoCastAOE(SPELL_CRUSHING_LEAP_DMG, true);
                    DoCastAOE(SPELL_CRUSHING_LEAP_VISUAL, true);
                }
                else if (type == POINT_MOTION_TYPE && pointId == 1)
                {
                    me->SetFacingTo(-M_PI/2);
                    DoCastAOE(SPELL_SUMMON_FIRE_WALL);
                }
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                instance->SetData(secondDemon ? DATA_EVENT_DEMON_2 : DATA_EVENT_DEMON, DONE);

                if (instance->GetData(DATA_EVENT_ILLIDAN_1) == IN_PROGRESS)
                    if (Creature* illidan = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_EVENT_ILLIDAN_1)))
                        illidan->AI()->DoAction(ACTION_ILLIDAN_1_LEGION_DEMON_RESUME);

                Map::PlayerList const &PlayerList = instance->instance->GetPlayers();
                if (!PlayerList.isEmpty())
                    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                        if (Player* player = i->GetSource())
                            if (me->GetDistance(player) <= 50.0f && player->GetQuestStatus(QUEST_DOCUMENTING_THE_TIMEWAYS) == QUEST_STATUS_INCOMPLETE)
                                player->CastSpell(me, SPELL_ARCHIVED_DEMON_1, true);
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action != ACTION_LEGION_DEMON_START)
                    return;

                me->GetMotionMaster()->MovePoint(1, { 3194.41f, -4946.16f, 189.52f, -M_PI/2 });
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_STRIKE_FEAR:
                            DoCastAOE(SPELL_STRIKE_FEAR);
                            events.ScheduleEvent(EVENT_STRIKE_FEAR, urand(10000, 15000));
                            break;
                        case EVENT_CRUSHING_LEAP:
                            //if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                            //    DoCastVictim(SPELL_CRUSHING_LEAP);
                            events.ScheduleEvent(EVENT_STRIKE_FEAR, urand(15000, 20000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            bool secondDemon;
            EventMap events;
            InstanceScript* instance;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetWoEAI<npc_well_of_eternity_legion_demonAI>(creature);
        }
};

#define PATH_LENGTH 7
const Position legionDemonScenePathLeft[PATH_LENGTH] =
{
    { 3455.80f, -5080.19f, 213.68f, 2.14f }, // start
    { 3446.33f, -5064.96f, 213.60f, 2.14f }, // top of the first staircase
    { 3427.23f, -5035.29f, 196.78f, 2.14f }, // bottom of the first staircase
    { 3391.52f, -4978.99f, 196.87f, 2.14f }, // top of the second staircase
    { 3376.83f, -4955.74f, 181.08f, 2.14f }, // bottom of the second staircase
    { 3333.39f, -4887.34f, 181.08f, 2.14f }, // center
    { 3257.19f, -4941.38f, 181.68f, 2.14f + M_PI/2 }, // portal
};

const Position legionDemonScenePathRight[PATH_LENGTH] =
{
    { 3455.80f, -5080.19f, 213.68f, 2.14f }, // start
    { 3446.33f, -5064.96f, 213.60f, 2.14f }, // top of the first staircase
    { 3427.23f, -5035.29f, 196.78f, 2.14f }, // bottom of the first staircase
    { 3391.52f, -4978.99f, 196.87f, 2.14f }, // top of the second staircase
    { 3376.83f, -4955.74f, 181.08f, 2.14f }, // bottom of the second staircase
    { 3333.39f, -4887.34f, 181.08f, 2.14f }, // center
    { 3413.48f, -4842.81f, 181.70f, 2.14f - M_PI/2 }, // portal
};

const Position legionDemonScenePathForward[PATH_LENGTH] =
{
    { 3455.80f, -5080.19f, 213.68f, 2.14f }, // start
    { 3446.33f, -5064.96f, 213.60f, 2.14f }, // top of the first staircase
    { 3427.23f, -5035.29f, 196.78f, 2.14f }, // bottom of the first staircase
    { 3391.52f, -4978.99f, 196.87f, 2.14f }, // top of the second staircase
    { 3376.83f, -4955.74f, 181.08f, 2.14f }, // bottom of the second staircase
    { 3333.39f, -4887.34f, 181.08f, 2.14f }, // center
    { 3288.06f, -4818.26f, 181.47f, 2.14f }, // portal
};

#define GUID_GUARDIAN_LEFT 150105
#define GUID_GUARDIAN_RIGHT 150106
#define GUID_GUARDIAN_CENTER_1 150103
#define GUID_GUARDIAN_CENTER_2 150104
#define GUID_INTRO_TRASH_1 339886
#define GUID_INTRO_TRASH_2 339885
#define GUID_INTRO_TRASH_3 339887

enum LegionDemonPathDirection
{
    DIR_LEGION_DEMON_NOT_SET,
    DIR_LEGION_DEMON_LEFT,
    DIR_LEGION_DEMON_RIGHT,
    DIR_LEGION_DEMON_FORWARD,
};

enum LegionDemonFormationPosition
{
    FORMATION_LEGION_DEMON_NOT_SET,
    FORMATION_LEGION_DEMON_FORE_LEFT,
    FORMATION_LEGION_DEMON_FORE_RIGHT,
    FORMATION_LEGION_DEMON_MID_LEFT,
    FORMATION_LEGION_DEMON_MID_RIGHT,
    FORMATION_LEGION_DEMON_BACK_LEFT,
    FORMATION_LEGION_DEMON_BACK_RIGHT,
};

class npc_well_of_eternity_illidan_1 : public CreatureScript
{
    public:
        npc_well_of_eternity_illidan_1() : CreatureScript("npc_well_of_eternity_illidan_1") { }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            if (InstanceScript* instance = creature->GetInstanceScript())
            {
                if (instance->GetBossState(DATA_PEROTHARN) == DONE)
                {
                    player->ADD_GOSSIP_ITEM_DB(13163, 1, 0, 0);
                    player->SEND_GOSSIP_MENU(18958, creature->GetGUID());
                }
                else
                {
                    creature->AI()->DoAction(ACTION_ILLIDAN_1_STOP_NAGGING);
                    if (!creature->HasAura(SPELL_SHADOWCLOAK_ILLIDAN))
                    {
                        player->ADD_GOSSIP_ITEM_DB(13162, 0, 0, 0);
                        player->SEND_GOSSIP_MENU(18550, creature->GetGUID());
                    }
                    else
                    {
                        player->ADD_GOSSIP_ITEM_DB(13163, 0, 0, 0);
                        if (player->IsGameMaster())
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Skip to boss.", 0, 100);
                        player->SEND_GOSSIP_MENU(18551, creature->GetGUID());
                    }
                }
            }
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();
            player->CLOSE_GOSSIP_MENU();

            if (InstanceScript* instance = creature->GetInstanceScript())
            {
                if (instance->GetBossState(DATA_PEROTHARN) == DONE)
                {
                    creature->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    creature->AI()->DoAction(ACTION_ILLIDAN_1_OUTRO_RESUME);
                }
                else
                {
                    if (!creature->HasAura(SPELL_SHADOWCLOAK_ILLIDAN))
                    {
                        creature->AI()->DoAction(ACTION_ILLIDAN_1_PRESTART);
                    }
                    else
                    {
                        if (action == 100)
                        {
                            // Preevent skip for GMs
                            if (InstanceScript* instance = creature->GetInstanceScript())
                            {
                                instance->SetData(DATA_EVENT_ILLIDAN_1, DONE);
                                instance->SetData(DATA_LEGION_DEMON_BLOCKED_PATH_LEFT, 1);
                                instance->SetData(DATA_LEGION_DEMON_BLOCKED_PATH_RIGHT, 1);
                                instance->SetData(DATA_LEGION_DEMON_BLOCKED_PATH_FORWARD, 1);
                            }
                            creature->AI()->DoAction(ACTION_ILLIDAN_1_SKIP);
                        }
                        else
                        {
                            creature->AI()->DoAction(ACTION_ILLIDAN_1_START);
                            if (InstanceScript* instance = creature->GetInstanceScript())
                                instance->SetData(DATA_EVENT_ILLIDAN_1, IN_PROGRESS);
                        }
                    }
                }
            }
            return true;
        }

        struct npc_well_of_eternity_illidan_1AI : public ScriptedAI
        {
            npc_well_of_eternity_illidan_1AI(Creature* creature) : ScriptedAI(creature)
            {
                bTalk = false;
                introPassed = false;
                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                instance = me->GetInstanceScript();
            }

            void Reset() OVERRIDE
            {
                waypointMovement = false;
                waypointMovementInProgress = false;
                waypointMovementDelay = 0;
                waypointIndex = -1;
                events.Reset();
                if (instance)
                    if (instance->GetData(DATA_EVENT_ILLIDAN_1) == DONE)
                        me->DespawnOrUnsummon(500);

                timerSpawnLegionDemon = 3000;
                victimCheckTimer = 0;
                events.ScheduleEvent(EVENT_ILLIDAN_1_INITIAL_CLOAK, 1000);
            }

            void EnterEvadeMode() OVERRIDE { }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                if (damage >= me->GetHealth())
                    damage = me->GetHealth() - 1;
            }

            void MoveInLineOfSight(Unit* who) OVERRIDE
            {
                if (bTalk || !instance)
                    return;

                if (who->GetTypeId() != TypeID::TYPEID_PLAYER)
                    return;

                if (me->GetDistance(who) > 40.0f)
                    return;

                bTalk = true;

                events.ScheduleEvent(EVENT_ILLIDAN_1_INTRO_1, 1000);
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE && type != EFFECT_MOTION_TYPE)
                    return;

                waypointMovementInProgress = false;

                switch (pointId)
                {
                    case POINT_ILLIDAN_1_INTRO_1:
                        if (introPassed)
                            return;
                        PauseMovement();
                        me->RemoveAurasDueToSpell(SPELL_SHADOWCLOAK_ILLIDAN);
                        events.ScheduleEvent(EVENT_ILLIDAN_1_INTRO_2, 1000);
                        break;
                    case POINT_ILLIDAN_1_MOVE_3:
                        me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP); // Failsafe
                        Talk(SAY_ILLIDAN_1_MOVE_3);
                        ResumeMovement(4000);
                        break;
                    case POINT_ILLIDAN_1_MOVE_4:
                        me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP); // Failsafe
                        Talk(SAY_ILLIDAN_1_MOVE_4);
                        break;
                    case POINT_ILLIDAN_1_MOVE_5:
                        Talk(SAY_ILLIDAN_1_MOVE_5);
                        break;
                    case POINT_ILLIDAN_1_MOVE_5_STOP:
                        PauseMovement();
                        events.ScheduleEvent(EVENT_ILLIDAN_1_MOVE_6, 22000);
                        DoCastAOE(SPELL_WALL_OF_SHADOW);
                        if (Creature* stalker = GetClosestCreatureWithEntry(me, NPC_WELL_OF_ETERNITY_STALKER, 50))
                            stalker->CastSpell(stalker, SPELL_DISTRACT_DEMONS_AURA);
                        if (InstanceScript* instance = me->GetInstanceScript())
                            instance->SetData(DATA_LEGION_DEMON_BLOCKED_PATH_LEFT, 2);
                        break;
                    case POINT_ILLIDAN_1_MOVE_6_CRYSTAL:
                        PauseMovement();
                        ClearNotAttackable();
                        Talk(RAND(SAY_ILLIDAN_1_PRECOMBAT_1, SAY_ILLIDAN_1_PRECOMBAT_2));
                        events.ScheduleEvent(EVENT_ILLIDAN_1_SAY_WAIT, 15000);
                        break;
                    case POINT_ILLIDAN_1_MOVE_7:
                        Talk(SAY_ILLIDAN_1_MOVE_7);
                        break;
                    case POINT_ILLIDAN_1_MOVE_7_CRYSTAL:
                        PauseMovement();
                        ClearNotAttackable();
                        Talk(RAND(SAY_ILLIDAN_1_PRECOMBAT_1, SAY_ILLIDAN_1_PRECOMBAT_2));
                        events.ScheduleEvent(EVENT_ILLIDAN_1_SAY_WAIT, 15000);
                        break;
                    case POINT_ILLIDAN_1_MOVE_8:
                        PauseMovement();
                        Talk(SAY_ILLIDAN_1_MOVE_8);
                        ResumeMovement(13000);
                        break;
                    case POINT_ILLIDAN_1_MOVE_9:
                        Talk(SAY_ILLIDAN_1_MOVE_9);
                        break;
                    case POINT_ILLIDAN_1_MOVE_9_STOP:
                        ClearNotAttackable();
                        PauseMovement();
                        break;
                    case POINT_ILLIDAN_1_MOVE_10:
                        PauseMovement();
                        Talk(SAY_ILLIDAN_1_MOVE_10);
                        ResumeMovement(1000);
                        break;
                    case POINT_ILLIDAN_1_MOVE_10_CRYSTAL:
                        PauseMovement();
                        ClearNotAttackable();
                        Talk(RAND(SAY_ILLIDAN_1_PRECOMBAT_1, SAY_ILLIDAN_1_PRECOMBAT_2));
                        events.ScheduleEvent(EVENT_ILLIDAN_1_SAY_WAIT, 15000);
                        break;
                    case POINT_ILLIDAN_1_MOVE_11:
                        PauseMovement();
                        Talk(SAY_ILLIDAN_1_MOVE_11);
                        ResumeMovement(4000);
                        // Teleporting him now to avoid messing up the visual
                        if (InstanceScript* instance = me->GetInstanceScript())
                            if (Creature* perotharn = instance->instance->GetCreature(instance->GetData64(DATA_PEROTHARN)))
                                perotharn->AI()->DoAction(3); // ACTION_TELEPORT_APPEAR
                        break;
                    case POINT_ILLIDAN_1_MOVE_12:
                        PauseMovement();
                        Talk(SAY_ILLIDAN_1_MOVE_12);
                        ResumeMovement(4000);
                        break;
                    case POINT_ILLIDAN_1_MOVE_13:
                        Talk(SAY_ILLIDAN_1_MOVE_13);
                        break;
                    case POINT_ILLIDAN_1_BOSS:
                        PauseMovement();
                        Talk(SAY_ILLIDAN_1_BOSS_PRE);
                        // Teleporting him now to avoid messing up the visual
                        if (InstanceScript* instance = me->GetInstanceScript())
                            if (Creature* perotharn = instance->instance->GetCreature(instance->GetData64(DATA_PEROTHARN)))
                                perotharn->AI()->DoAction(4); // ACTION_TELEPORT_CENTER
                        events.ScheduleEvent(EVENT_ILLIDAN_1_BOSS_SPAWN, 3000);
                        events.ScheduleEvent(EVENT_ILLIDAN_1_SAY_WAIT, 15000);
                        break;
                    case POINT_ILLIDAN_1_OUTRO_2:
                        PauseMovement();
                        me->SetOrientation(2.14f);
                        me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                        if (InstanceScript* instance = me->GetInstanceScript())
                        {
                            Map::PlayerList const& players = instance->instance->GetPlayers();
                            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                                if (Player* player = itr->GetSource())
                                    if (player->HasItemCount(32837) && player->HasItemCount(32838) || player->HasAchieved(426))
                                    {
                                        Talk(SAY_ILLIDAN_1_AZZINOTH_1, player);
                                        events.ScheduleEvent(EVENT_ILLIDAN_1_AZZINOTH_2, 5000);
                                    }
                        }
                        break;
                    case POINT_ILLIDAN_1_OUTRO_JUMP_FROM:
                        PauseMovement();
                        me->GetMotionMaster()->MoveJump(illidan1Pos[POINT_ILLIDAN_1_OUTRO_JUMP_TO].GetPositionX(), illidan1Pos[POINT_ILLIDAN_1_OUTRO_JUMP_TO].GetPositionY(), illidan1Pos[POINT_ILLIDAN_1_OUTRO_JUMP_TO].GetPositionZ(), 10, 30, /*10, */POINT_ILLIDAN_1_OUTRO_JUMP_TO); // TODO: port orientation for jump
                        break;
                    case POINT_ILLIDAN_1_OUTRO_JUMP_TO:
                        ++waypointIndex;
                        ResumeMovement();
                        break;
                    case POINT_ILLIDAN_1_OUTRO_DESPAWN:
                        PauseMovement();
                        me->DisappearAndDie();
                        break;
                    default:
                        break;
                }
            }

            void SpellHit(Unit* caster, const SpellInfo* spell) OVERRIDE
            {
                if (spell->Id == SPELL_AGGRO_ILLIDAN)
                {
                    me->RemoveAura(SPELL_SHADOWCLOAK_ILLIDAN);
                    AttackStart(caster);
                    if (!aggroTalkCooldown)
                    {
                        Talk(RAND(SAY_ILLIDAN_1_AGGRO_1, SAY_ILLIDAN_1_AGGRO_2));
                        aggroTalkCooldown = 10000;
                    }
                    events.CancelEvent(EVENT_ILLIDAN_1_SAY_WAIT);
                    victimCheckTimer = 2000;
                }
            }

            void DoAction(int32 action) OVERRIDE
            {
                switch (action)
                {
                    case ACTION_ILLIDAN_1_STOP_NAGGING:
                        events.CancelEvent(EVENT_ILLIDAN_1_NAGGING);
                        break;
                    case ACTION_ILLIDAN_1_PRESTART:
                        PrepareReturnToTheShadows();
                        DoCastAOE(SPELL_RETURN_TO_THE_SHADOWS);
                        DoCastAOE(SPELL_SHADOWCLOAK_ILLIDAN);
                        events.ScheduleEvent(EVENT_ILLIDAN_1_MOVE_1, 100);
                        break;
                    case ACTION_ILLIDAN_1_START:
                        if (introPassed || !bTalk)
                            return;
                        introPassed = true;
                        events.CancelEvent(EVENT_ILLIDAN_1_INTRO_2);
                        events.CancelEvent(EVENT_ILLIDAN_1_NAGGING); // Failsafe
                        if (InstanceScript* instance = me->GetInstanceScript())
                            if (instance->GetData(DATA_EVENT_ILLIDAN_1) != IN_PROGRESS)
                                instance->SetData(DATA_EVENT_ILLIDAN_1, IN_PROGRESS); // Failsafe
                        if (!me->HasAura(SPELL_SHADOWCLOAK_ILLIDAN))
                        {
                            PrepareReturnToTheShadows();
                            DoCastAOE(SPELL_RETURN_TO_THE_SHADOWS);
                            DoCastAOE(SPELL_SHADOWCLOAK_ILLIDAN);
                        }
                        me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                        events.ScheduleEvent(EVENT_ILLIDAN_1_MOVE_2, 100);
                        break;
                    case ACTION_ILLIDAN_1_CRYSTAL:
                        events.CancelEvent(EVENT_ILLIDAN_1_SAY_WAIT); // Failsafe (if players aggroed enemies far from Illidan)
                        Talk(RAND(SAY_ILLIDAN_1_CRYSTAL_1, SAY_ILLIDAN_1_CRYSTAL_2, SAY_ILLIDAN_1_CRYSTAL_3, SAY_ILLIDAN_1_CRYSTAL_4));
                        victimCheckTimer = 0;
                        break;
                    case ACTION_ILLIDAN_1_CRYSTAL_RESUME:
                        Talk(SAY_ILLIDAN_1_POSTCOMBAT);
                        PrepareReturnToTheShadows();
                        DoCastAOE(SPELL_RETURN_TO_THE_SHADOWS);
                        DoCastAOE(SPELL_SHADOWCLOAK_ILLIDAN);
                        events.CancelEvent(EVENT_ILLIDAN_1_WIPE_CHECK); // No need to check for wipes after the first crystal
                        ResumeMovement(2000);
                        break;
                    case ACTION_ILLIDAN_1_LEGION_DEMON_RESUME:
                        PrepareReturnToTheShadows();
                        DoCastAOE(SPELL_RETURN_TO_THE_SHADOWS);
                        DoCastAOE(SPELL_SHADOWCLOAK_ILLIDAN);
                        ResumeMovement(2000);
                        break;
                    case ACTION_PEROTHARN_DEAD:
                        events.ScheduleEvent(EVENT_ILLIDAN_1_OUTRO_1, 3000);
                        break;
                    case ACTION_ILLIDAN_1_OUTRO_RESUME:
                        if (me->isDead())
                            return;
                        if (!waypointMovement)
                        {
                            Talk(SAY_ILLIDAN_1_OUTRO_2);
                            me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                            ResumeMovement();
                        }
                        break;
                    case ACTION_ILLIDAN_1_SKIP:
                        waypointIndex = 35;
                        ResumeMovement();
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (aggroTalkCooldown)
                    if (aggroTalkCooldown <= diff)
                        aggroTalkCooldown = 0;
                    else aggroTalkCooldown -= diff;

                SpawnDemons(diff);
                DoWaypointMovement(diff);
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_ILLIDAN_1_WIPE_CHECK:
                        if (InstanceScript* instance = me->GetInstanceScript())
                        {
                            bool wipe = true;
                            Map::PlayerList const& players = instance->instance->GetPlayers();
                            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                            {
                                if (Player* player = itr->GetSource())
                                {
                                    if (player->IsInWorld() && player->GetMapId() == me->GetMapId() && player->IsAlive())
                                    {
                                        wipe = false;
                                        break;
                                    }
                                }
                            }
                            if (wipe)
                            {
                                // Reset stalker (it will revive guards)
                                if (Creature* stalker = GetClosestCreatureWithEntry(me, NPC_FEL_CRYSTAL_STALKER, 200))
                                    stalker->AI()->Reset();
                                if (Creature* stalker = GetClosestCreatureWithEntry(me, NPC_WELL_OF_ETERNITY_STALKER, 200))
                                    stalker->RemoveAurasDueToSpell(SPELL_DISTRACT_DEMONS_AURA);
                                if (InstanceScript* instance = me->GetInstanceScript())
                                    instance->SetData(DATA_LEGION_DEMON_BLOCKED_PATH_LEFT, 0);

                                bTalk = false;
                                me->Respawn(true);
                            }
                        }
                        events.ScheduleEvent(EVENT_ILLIDAN_1_WIPE_CHECK, 5000);
                        break;
                    case EVENT_ILLIDAN_1_RESPAWN:
                        break;
                    case EVENT_ILLIDAN_1_INITIAL_CLOAK:
                        DoCastAOE(SPELL_SHADOWCLOAK_ILLIDAN);
                        break;
                    case EVENT_ILLIDAN_1_INTRO_1:
                        Talk(SAY_ILLIDAN_1_INTRO_1);
                        ResumeMovement();
                        break;
                    case EVENT_ILLIDAN_1_INTRO_2:
                        Talk(SAY_ILLIDAN_1_INTRO_2);
                        me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                        events.ScheduleEvent(EVENT_ILLIDAN_1_NAGGING, 20000);
                        break;
                    case EVENT_ILLIDAN_1_NAGGING:
                        Talk(RAND(SAY_ILLIDAN_1_INTRO_IDLE_1, SAY_ILLIDAN_1_INTRO_IDLE_2, SAY_ILLIDAN_1_INTRO_IDLE_3));
                        events.ScheduleEvent(EVENT_ILLIDAN_1_NAGGING, 20000);
                        break;
                    case EVENT_ILLIDAN_1_MOVE_1:
                        Talk(SAY_ILLIDAN_1_MOVE_1);
                        break;
                    case EVENT_ILLIDAN_1_MOVE_2:
                        Talk(SAY_ILLIDAN_1_MOVE_2);
                        events.ScheduleEvent(EVENT_ILLIDAN_1_WIPE_CHECK, 5000);
                        ResumeMovement(4000);
                        break;
                    case EVENT_ILLIDAN_1_MOVE_6:
                    {
                        Talk(SAY_ILLIDAN_1_MOVE_6);
                        me->InterruptNonMeleeSpells(false);
                        me->RemoveAurasDueToSpell(SPELL_WALL_OF_SHADOW);
                        ResumeMovement();

                        std::list<Creature*> wallOfShadowStalkers;
                        GetCreatureListWithEntryInGrid(wallOfShadowStalkers, me, NPC_WELL_OF_ETERNITY_STALKER, 50);
                        for (std::list<Creature*>::iterator itr = wallOfShadowStalkers.begin(); itr != wallOfShadowStalkers.end(); ++itr)
                            if (Creature* wallOfShadowStalker = *itr)
                                wallOfShadowStalker->RemoveAurasDueToSpell(SPELL_DISTRACT_DEMONS_AURA);

                        if (InstanceScript* instance = me->GetInstanceScript())
                            instance->SetData(DATA_LEGION_DEMON_BLOCKED_PATH_LEFT, 0);
                        break;
                    }
                    case EVENT_ILLIDAN_1_BOSS_SPAWN:
                        events.CancelEvent(EVENT_ILLIDAN_1_WIPE_CHECK);
                        if (InstanceScript* instance = me->GetInstanceScript())
                            if (Creature* boss = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_PEROTHARN)))
                                boss->AI()->DoAction(1); // ACTION_START
                        break;
                    case EVENT_ILLIDAN_1_OUTRO_1:
                        Talk(SAY_ILLIDAN_1_OUTRO_1);
                        me->RemoveAurasDueToSpell(SPELL_SHADOWCLOAK_ILLIDAN);
                        ResumeMovement(1000);
                        break;
                    case EVENT_ILLIDAN_1_AZZINOTH_2:
                        if (InstanceScript* instance = me->GetInstanceScript())
                        {
                            Map::PlayerList const& players = instance->instance->GetPlayers();
                            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                                if (Player* player = itr->GetSource())
                                    if (player->HasItemCount(32837) && player->HasItemCount(32838) || player->HasAchieved(426))
                                        Talk(SAY_ILLIDAN_1_AZZINOTH_2, player);
                        }
                        break;
                    case EVENT_ILLIDAN_1_SAY_WAIT:
                        Talk(RAND(SAY_ILLIDAN_1_WAIT_1, SAY_ILLIDAN_1_WAIT_2, SAY_ILLIDAN_1_WAIT_3));
                        events.ScheduleEvent(EVENT_ILLIDAN_1_SAY_WAIT, 20000);
                        break;
                    default:
                        break;
                }

                if (!me->GetVictim() && !me->HasAura(SPELL_SHADOWCLOAK_ILLIDAN) && victimCheckTimer)
                {
                    if (victimCheckTimer <= diff)
                    {
                        victimCheckTimer = 2000;
                        Creature* victim;
                        if (!(victim = GetClosestCreatureWithEntry(me, NPC_CORRUPTED_ARCANIST, 20)))
                            if (!(victim = GetClosestCreatureWithEntry(me, NPC_DREADLORD_DEFENDER, 20)))
                                return;
                        AttackStart(victim);
                    }
                    else victimCheckTimer -= diff;
                }
                if (me->GetVictim())
                    DoMeleeAttackIfReady();
            }

            void SpawnDemons(const uint32 diff)
            {
                if (timerSpawnLegionDemon <= diff)
                {
                    timerSpawnLegionDemon += 2500;
                    if (instance && (!instance->GetData(DATA_LEGION_DEMON_BLOCKED_PATH_LEFT) || !instance->GetData(DATA_LEGION_DEMON_BLOCKED_PATH_RIGHT) || !instance->GetData(DATA_LEGION_DEMON_BLOCKED_PATH_FORWARD)))
                    {
                        for (LegionDemonFormationPosition pos = FORMATION_LEGION_DEMON_FORE_LEFT; pos <= FORMATION_LEGION_DEMON_BACK_RIGHT; pos = LegionDemonFormationPosition(pos + 1))
                        {
                            if (Creature* demon = me->SummonCreature(NPC_LEGION_DEMON_PREEVENT, legionDemonScenePathForward[0], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                            {
                                LegionDemonPathDirection dir;
                                switch (pos)
                                {
                                    case FORMATION_LEGION_DEMON_FORE_LEFT:
                                    case FORMATION_LEGION_DEMON_FORE_RIGHT:
                                        dir = DIR_LEGION_DEMON_FORWARD;
                                        break;
                                        break;
                                    case FORMATION_LEGION_DEMON_MID_LEFT:
                                    case FORMATION_LEGION_DEMON_BACK_LEFT:
                                        dir = DIR_LEGION_DEMON_LEFT;
                                        break;
                                    case FORMATION_LEGION_DEMON_MID_RIGHT:
                                    case FORMATION_LEGION_DEMON_BACK_RIGHT:
                                        dir = DIR_LEGION_DEMON_RIGHT;
                                        break;
                                    default:
                                        break;
                                }
                                demon->AI()->SetData(1, dir);
                                demon->AI()->SetData(2, pos);
                            }
                        }
                    }
                }
                else timerSpawnLegionDemon -= diff;
            }

            void ClearNotAttackable()
            {
                std::list<Creature*> enemies;
                GetCreatureListWithEntryInGrid(enemies, me, NPC_CORRUPTED_ARCANIST, 25);
                GetCreatureListWithEntryInGrid(enemies, me, NPC_DREADLORD_DEFENDER, 25);
                GetCreatureListWithEntryInGrid(enemies, me, NPC_LEGION_DEMON, 25);
                for (std::list<Creature*>::const_iterator itr = enemies.begin(); itr != enemies.end(); ++itr)
                {
                    if (Creature* enemy = *itr)
                    {
                        enemy->SetReactState(REACT_AGGRESSIVE);
                        enemy->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        enemy->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                        enemy->ApplySpellImmune(0, IMMUNITY_ID, SPELL_SHADOWCLOAK_AGGRO, false);
                        enemy->ApplySpellImmune(0, IMMUNITY_ID, SPELL_MARCHING_DEMON_REACT, false);
                    }
                }
            }

            void PrepareReturnToTheShadows()
            {
                Map::PlayerList const& players = instance->instance->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                {
                    if (Player* player = itr->GetSource())
                    {
                        if (player->IsInCombat())
                        {
                            player->CombatStop();
                            player->getHostileRefManager().deleteReferences();
                            if (Guardian* pet = player->GetGuardianPet())
                            {
                                if (pet->IsInCombat())
                                {
                                    pet->CombatStop();
                                    pet->getHostileRefManager().deleteReferences();
                                }
                            }
                        }
                    }
                }
            }

            void DoWaypointMovement(const uint32 diff)
            {
                if (waypointMovementDelay && waypointMovementDelay <= diff)
                {
                    waypointMovement = true;
                    waypointMovementDelay = 0;
                }
                else waypointMovementDelay -= diff;

                if (waypointMovement && !waypointMovementInProgress)
                {
                    ++waypointIndex;
                    waypointMovementInProgress = true;
                    me->GetMotionMaster()->MovePoint(waypointIndex, illidan1Pos[waypointIndex]);
                }
            }
            void PauseMovement()
            {
                waypointMovement = false;
                waypointMovementDelay = 0;
            }
            void ResumeMovement(uint32 delay = 0)
            {
                if (delay)
                {
                    waypointMovement = false;
                    waypointMovementInProgress = false;
                    waypointMovementDelay = delay;
                }
                else
                {
                    waypointMovement = true;
                    waypointMovementInProgress = false;
                    waypointMovementDelay = 0;
                }
            }

        private:
            bool bTalk;
            bool introPassed;
            bool waypointMovement;
            bool waypointMovementInProgress;
            uint32 waypointMovementDelay;
            int32 waypointIndex;
            uint32 timerSpawnLegionDemon;
            uint32 aggroTalkCooldown;
            uint32 victimCheckTimer;
            EventMap events;
            InstanceScript* instance;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetWoEAI<npc_well_of_eternity_illidan_1AI>(creature);
        }
};

class npc_well_of_eternity_legion_demon_preevent : CreatureScript
{
    public:
        npc_well_of_eternity_legion_demon_preevent() : CreatureScript("npc_well_of_eternity_legion_demon_preevent") { }

        struct npc_well_of_eternity_legion_demon_preeventAI : public ScriptedAI
        {
            npc_well_of_eternity_legion_demon_preeventAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                events.Reset();
                waypointIndex = 0;
                dir = DIR_LEGION_DEMON_NOT_SET;
                pos = FORMATION_LEGION_DEMON_NOT_SET;
            }

            void SpellHit(Unit* caster, const SpellInfo* spell) OVERRIDE
            {
                if (spell->Id == SPELL_SHADOWCLOAK_AGGRO)
                {
                    caster->RemoveAura(SPELL_SHADOW_WALK_AURA);
                    if (Creature* guardian = GetClosestCreatureWithEntry(me, NPC_GUARDIAN_DEMON, 200))
                        guardian->AI()->SetGUID(caster->GetGUID());
                }
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                me->CombatStop(true);
                me->getHostileRefManager().deleteReferences();
            }

            void MoveInLineOfSight(Unit* who) OVERRIDE
            {
                if (who->GetTypeId() == TypeID::TYPEID_PLAYER || who->GetTypeId() == TypeID::TYPEID_UNIT && who->GetCharmerOrOwner() && who->GetCharmerOrOwner()->GetTypeId() == TypeID::TYPEID_PLAYER)
                {
                    if (Player* player = who->ToPlayer())
                        if (player->IsGameMaster())
                            return;

                    if (!me->CanStartAttack(who, false))
                        return;

                    if (who->HasAura(SPELL_SHADOW_WALK_AURA) || who->HasAura(SPELL_SHADOWCLOAK_PETS))
                        return;

                    if (who->HasAura(SPELL_DEMON_GRIP))
                        return;

                    if (Creature* guardian = GetClosestCreatureWithEntry(me, NPC_GUARDIAN_DEMON, 200))
                        guardian->AI()->SetGUID(who->GetGUID());
                }
            }

            void EnterEvadeMode() OVERRIDE
            {
                CreatureAI::EnterEvadeMode();
                waypointIndex = 5;
                moveToNextWaypoint = true;
            }

            void SetData(uint32 type, uint32 data) OVERRIDE
            {
                if (type == 1)
                    dir = LegionDemonPathDirection(data);
                if (type == 2)
                    pos = LegionDemonFormationPosition(data);

                if (dir != DIR_LEGION_DEMON_NOT_SET && pos != FORMATION_LEGION_DEMON_NOT_SET)
                {
                    Position position(GetWaypoint(0));
                    OffsetPathPosition(position, 0);
                    me->NearTeleportTo(position.GetPositionX(), position.GetPositionY(), position.GetPositionZ(), position.GetOrientation());
                    events.ScheduleEvent(EVENT_MARCHING_DEMON_SOUND, 3000);

                    moveToNextWaypoint = true;
                }
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                if (pointId == 5)
                {
                    me->SetSpeed(MOVE_RUN, 1.28571f / 2, true);
                    me->SetSpeed(MOVE_WALK, 3.6f / 2, true);
                    me->SetWalk(true);
                    if (InstanceScript* instance = me->GetInstanceScript())
                        if (dir == DIR_LEGION_DEMON_LEFT && instance->GetData(DATA_LEGION_DEMON_BLOCKED_PATH_LEFT) ||
                            dir == DIR_LEGION_DEMON_RIGHT && instance->GetData(DATA_LEGION_DEMON_BLOCKED_PATH_RIGHT))
                            dir = DIR_LEGION_DEMON_FORWARD;
                }

                moveToNextWaypoint = true;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (moveToNextWaypoint)
                {
                    moveToNextWaypoint = false;
                    MoveToNextWaypoint();
                }

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_MARCHING_DEMON_SOUND:
                        DoCastAOE(SPELL_MARCHING_DEMON_SOUND);
                        events.ScheduleEvent(EVENT_MARCHING_DEMON_SOUND, 3000);
                        break;
                    default:
                        break;
                }

                if (UpdateVictim())
                    DoMeleeAttackIfReady();
            }

            void MoveToNextWaypoint()
            {
                ++waypointIndex;
                if (waypointIndex == PATH_LENGTH)
                {
                    me->DespawnOrUnsummon();
                    return;
                }

                Position position(GetWaypoint(waypointIndex));
                OffsetPathPosition(position, waypointIndex);
                me->GetMotionMaster()->MovePoint(waypointIndex, position);
            }

        private:
            EventMap events;
            bool moveToNextWaypoint;
            uint32 waypointIndex;
            LegionDemonPathDirection dir;
            LegionDemonFormationPosition pos;

            const Position GetWaypoint(int index)
            {
                if (index >= PATH_LENGTH)
                    return Position();

                switch (dir)
                {
                case DIR_LEGION_DEMON_LEFT: return legionDemonScenePathLeft[index];
                case DIR_LEGION_DEMON_RIGHT: return legionDemonScenePathRight[index];
                case DIR_LEGION_DEMON_FORWARD: return legionDemonScenePathForward[index];
                default: return Position();
                }
            }
            void OffsetPathPosition(Position& position, uint32 index)
            {
                float angle = position.GetOrientation();
                float dist = 8.94f;
                LegionDemonFormationPosition p = pos;

                // Stairs case, to avoid flying and dropping through floor
                if (index > 0 && index < 5)
                    if (p == FORMATION_LEGION_DEMON_FORE_LEFT || p == FORMATION_LEGION_DEMON_BACK_LEFT || p == FORMATION_LEGION_DEMON_MID_LEFT)
                        p = FORMATION_LEGION_DEMON_MID_LEFT;
                    else
                        p = FORMATION_LEGION_DEMON_MID_RIGHT;

                // Portal case
                if (index == 6)
                    if (p == FORMATION_LEGION_DEMON_MID_LEFT || p == FORMATION_LEGION_DEMON_BACK_RIGHT)
                        p = FORMATION_LEGION_DEMON_MID_RIGHT;
                    else if (p == FORMATION_LEGION_DEMON_BACK_LEFT || p == FORMATION_LEGION_DEMON_MID_RIGHT)
                        p = FORMATION_LEGION_DEMON_MID_LEFT;

                switch (p)
                {
                    case FORMATION_LEGION_DEMON_FORE_LEFT:
                        angle += 0.47f;
                        break;
                    case FORMATION_LEGION_DEMON_FORE_RIGHT:
                        angle -= 0.47f;
                        break;
                    case FORMATION_LEGION_DEMON_MID_LEFT:
                        angle += M_PI / 2;
                        dist = 4;
                        break;
                    case FORMATION_LEGION_DEMON_MID_RIGHT:
                        angle -= M_PI / 2;
                        dist = 4;
                        break;
                    case FORMATION_LEGION_DEMON_BACK_LEFT:
                        angle += M_PI - 0.47f;
                        break;
                    case FORMATION_LEGION_DEMON_BACK_RIGHT:
                        angle -= M_PI - 0.47f;
                        break;
                    default:
                        return;
                }
                float x, y, z;
                position.GetPosition(x, y, z);
                x += cosf(angle) * dist;
                y += sinf(angle) * dist;
                z = me->GetMap()->GetHeight(x, y, z);
                position.Relocate(x, y, z, angle);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetWoEAI<npc_well_of_eternity_legion_demon_preeventAI>(creature);
        }
};

class npc_well_of_eternity_guardian_demon : CreatureScript
{
    public:
        npc_well_of_eternity_guardian_demon() : CreatureScript("npc_well_of_eternity_guardian_demon") { }

        struct npc_well_of_eternity_guardian_demonAI : public ScriptedAI
        {
            npc_well_of_eternity_guardian_demonAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                events.Reset();
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                if (InstanceScript* instance = me->GetInstanceScript())
                    if (instance->GetData(DATA_EVENT_ILLIDAN_1) == DONE)
                        me->DespawnOrUnsummon(500);
            }

            void EnterCombat(Unit* who) OVERRIDE
            {
                if (who->HasAura(SPELL_SHADOW_WALK_AURA) || who->HasAura(SPELL_SHADOWCLOAK_PETS))
                {
                    me->CombatStop(true);
                    me->getHostileRefManager().deleteReferences();
                }
                else
                    SetGUID(who->GetGUID(), 0);
            }

            void KilledUnit(Unit* /*victim*/) OVERRIDE
            {
                Talk(RAND(2,3,4));
            }

            void SpellHit(Unit* caster, const SpellInfo* spell) OVERRIDE
            {
                if (spell->Id == SPELL_SHADOWCLOAK_AGGRO)
                {
                    caster->RemoveAura(SPELL_SHADOW_WALK_AURA);
                    SetGUID(caster->GetGUID(), 0);
                }
            }

            void MoveInLineOfSight(Unit* who) OVERRIDE
            {
                if (who->GetTypeId() == TypeID::TYPEID_PLAYER || who->GetTypeId() == TypeID::TYPEID_UNIT && who->GetCharmerOrOwner() && who->GetCharmerOrOwner()->GetTypeId() == TypeID::TYPEID_PLAYER)
                {
                    if (Player* player = who->ToPlayer())
                        if (player->IsGameMaster())
                            return;
                    if (!me->CanStartAttack(who, false))
                        return;
                    if (who->HasAura(SPELL_SHADOW_WALK_AURA) || who->HasAura(SPELL_SHADOWCLOAK_PETS))
                        return;

                    SetGUID(who->GetGUID(), 0);
                }
            }

            void SetGUID(uint64 guid, int32 /*type*/) OVERRIDE
            {
                if (Unit* target = ObjectAccessor::GetUnit(*me, guid))
                {
                    me->SetTarget(guid);
                    me->CombatStart(target);
                    DoCast(target, SPELL_DEMON_GRIP);
                    target->CastSpell(target, SPELL_DEMON_GRIP_ROOT, true);
                }
            }

            void SetData(uint32 type, uint32 data) OVERRIDE
            {
                if (type != 1)
                    return;

                if (data == 4)
                {
                    data = 3;
                    events.ScheduleEvent(EVENT_GUARDIAN_FLEE, urand(4000, 8000));
                }
                else
                    events.ScheduleEvent(EVENT_GUARDIAN_FLEE, urand(2000, 4000));

                point = data;
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;
                if (point == PATH_LENGTH - 1)
                    me->DespawnOrUnsummon();
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_GUARDIAN_FLEE:
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                        Talk(RAND(0, 1));
                        switch (point)
                        {
                            case 1:
                                me->GetMotionMaster()->MovePoint(PATH_LENGTH - 1, legionDemonScenePathLeft[PATH_LENGTH - 1]);
                                break;
                            case 2:
                                me->GetMotionMaster()->MovePoint(PATH_LENGTH - 1, legionDemonScenePathRight[PATH_LENGTH - 1]);
                                break;
                            case 3:
                                me->GetMotionMaster()->MovePoint(PATH_LENGTH - 1, legionDemonScenePathForward[PATH_LENGTH - 1]);
                                break;
                            default:
                                break;
                        }
                    default:
                        break;
                }

                UpdateVictim();
            }

        private:
            EventMap events;
            uint32 point;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetWoEAI<npc_well_of_eternity_guardian_demonAI>(creature);
        }
};

class npc_well_of_eternity_fel_crystal_stalker : public CreatureScript
{
    public:
        npc_well_of_eternity_fel_crystal_stalker() : CreatureScript("npc_well_of_eternity_fel_crystal_stalker") { }

        struct npc_well_of_eternity_fel_crystal_stalkerAI : public ScriptedAI
        {
            npc_well_of_eternity_fel_crystal_stalkerAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                events.Reset();
                if (InstanceScript* instance = me->GetInstanceScript())
                    if (instance->GetData(DATA_EVENT_ILLIDAN_1) != DONE)
                        events.ScheduleEvent(EVENT_FEL_CRYSTAL_STALKER_GUARDS, 1000);
            }

            void DoAction(int32 action) OVERRIDE
            {
                switch (action)
                {
                    case ACTION_FEL_CRYSTAL_STALKER_UNLOCK:
                        for (std::vector<uint64>::const_iterator itr = guardGuids.begin(); itr != guardGuids.end(); ++itr)
                            if (Creature* guard = ObjectAccessor::GetCreature(*me, *itr))
                                if (guard->IsAlive())
                                    return;

                        GameObject* portalEnergyFocus;
                        if (!(portalEnergyFocus = GetClosestGameObjectWithEntry(me, GO_PORTAL_ENERGY_FOCUS_1, 10)))
                            if (!(portalEnergyFocus = GetClosestGameObjectWithEntry(me, GO_PORTAL_ENERGY_FOCUS_2, 10)))
                                if (!(portalEnergyFocus = GetClosestGameObjectWithEntry(me, GO_PORTAL_ENERGY_FOCUS_3, 10)))
                                    return;

                        portalEnergyFocus->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_LOCKED);

                        if (InstanceScript* instance = me->GetInstanceScript())
                            if (Creature* illidan = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_EVENT_ILLIDAN_1)))
                                illidan->GetAI()->DoAction(ACTION_ILLIDAN_1_CRYSTAL);
                        break;
                    case ACTION_FEL_CRYSTAL_STALKER_MELTDOWN:
                        me->RemoveAurasDueToSpell(SPELL_FEL_CRYSTAL_STALKER_PERIODIC);
                        DoCastAOE(SPELL_FEL_CRYSTAL_ARC_MELTDOWN);
                        events.ScheduleEvent(EVENT_FEL_CRYSTAL_STALKER_DESTRUCTION, 3000);
                        events.ScheduleEvent(EVENT_FEL_CRYSTAL_STALKER_DESTROYED, 4000);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_FEL_CRYSTAL_STALKER_GUARDS:
                    {
                        guardGuids.clear();
                        std::list<Creature*> guards;
                        GetCreatureListWithEntryInGrid(guards, me, NPC_CORRUPTED_ARCANIST, 30);
                        GetCreatureListWithEntryInGrid(guards, me, NPC_DREADLORD_DEFENDER, 30);
                        for (std::list<Creature*>::const_iterator itr = guards.begin(); itr != guards.end(); ++itr)
                            if (Creature* guard = *itr)
                            {
                                if (guard->isDead())
                                    guard->Respawn(true);
                                guardGuids.push_back(guard->GetGUID());
                            }

                        if (guardGuids.size() != 3)
                            events.ScheduleEvent(EVENT_FEL_CRYSTAL_STALKER_GUARDS, 1000);
                        break;
                    }
                    case EVENT_FEL_CRYSTAL_STALKER_DESTRUCTION:
                    {
                        std::list<Creature*> crystals;
                        GetCreatureListWithEntryInGrid(crystals, me, NPC_FEL_CRYSTAL, 30);
                        for (std::list<Creature*>::iterator itr = crystals.begin(); itr != crystals.end(); ++itr)
                            if (Creature* crystal = *itr)
                                crystal->Kill(crystal);
                        break;
                    }
                    case EVENT_FEL_CRYSTAL_STALKER_DESTROYED:
                        me->CastSpell(me->GetPositionX(), me->GetPositionY(), me->GetMap()->GetHeight(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()), SPELL_FEL_CRYSTAL_STALKER_DESTRUCTION, false);
                        DoCastAOE(SPELL_FEL_CRYSTAL_STALKER_DESTROYED);

                        if (Creature* connector = GetClosestCreatureWithEntry(me, NPC_PORTAL_CONNECTOR_1, 30))
                            connector->GetAI()->DoAction(ACTION_PORTAL_DISABLE);

                        if (InstanceScript* instance = me->GetInstanceScript())
                            if (Creature* illidan = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_EVENT_ILLIDAN_1)))
                                illidan->GetAI()->DoAction(ACTION_ILLIDAN_1_CRYSTAL_RESUME);
                        break;
                    default:
                        break;
                }
            }

        private:
            EventMap events;
            std::vector<uint64> guardGuids;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetWoEAI<npc_well_of_eternity_fel_crystal_stalkerAI>(creature);
        }
};

class npc_well_of_eternity_portal_connector : CreatureScript
{
    public:
        npc_well_of_eternity_portal_connector() : CreatureScript("npc_well_of_eternity_portal_connector") { }

        struct npc_well_of_eternity_portal_connectorAI : public ScriptedAI
        {
            npc_well_of_eternity_portal_connectorAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                connectedToGuid = 0;
                portalGuid = 0;
                actionExecuted = false;
                events.Reset();
                Reconnect();
                events.ScheduleEvent(EVENT_PORTAL_CONNECTOR_RECONNECT, 10000);
                DoAction(ACTION_PORTAL_ACTIVE);
            }

            void Reconnect()
            {
                Unit* oldConnection = GetConnection();
                uint64 oldConnectionGuid = connectedToGuid;

                connectedToGuid = 0;
                portalGuid = 0;

                uint32 nextEntry = 0;
                switch (me->GetEntry())
                {
                    case NPC_PORTAL_CONNECTOR_1:
                        nextEntry = NPC_PORTAL_CONNECTOR_2;
                        break;
                    case NPC_PORTAL_CONNECTOR_2:
                        nextEntry = NPC_PORTAL_CONNECTOR_3;
                        break;
                    case NPC_PORTAL_CONNECTOR_3:
                        nextEntry = NPC_PORTAL_CONNECTOR_1;
                        break;
                    default:
                        return;
                }

                Creature* nextConnector = GetClosestCreatureWithEntry(me, nextEntry, 30);
                Creature* portal = GetClosestCreatureWithEntry(me, NPC_LEGION_PORTAL, 30);
                if (nextConnector && portal)
                {
                    if (me->GetExactDist2d(nextConnector) < me->GetExactDist2d(portal))
                        connectedToGuid = nextConnector->GetGUID();
                    else
                        portalGuid = portal->GetGUID();
                }
                else if (nextConnector)
                    connectedToGuid = nextConnector->GetGUID();
                else if (portal)
                    portalGuid = portal->GetGUID();

                if (oldConnection && oldConnectionGuid != connectedToGuid)
                    oldConnection->RemoveAurasDueToSpell(SPELL_PORTAL_BEAM_GREEN_CONNECTOR, me->GetGUID());
                if (Unit* newConnection = GetConnection())
                    DoCast(newConnection, SPELL_PORTAL_BEAM_GREEN_CONNECTOR);
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (actionExecuted)
                    return;
                actionExecuted = true;

                switch (action)
                {
                    case ACTION_PORTAL_ACTIVE:
                        break;
                    case ACTION_PORTAL_DISABLE:
                        events.CancelEvent(EVENT_PORTAL_CONNECTOR_RECONNECT);
                        me->RemoveAurasDueToSpell(SPELL_PORTAL_BEAM_GREEN_SELF);
                        me->RemoveAurasDueToSpell(SPELL_PORTAL_BEAM_GREEN_CONNECTOR);
                        DoCast(me, SPELL_PORTAL_BEAM_RED_SELF, true);
                        DoCast(me, SPELL_PORTAL_CONNECTOR_DEAD, true);
                        break;
                    default:
                        return;
                }

                if (Unit* connection = GetConnection())
                    connection->GetAI()->DoAction(action);
                if (Unit* portal = GetPortal())
                    portal->GetAI()->DoAction(action);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                actionExecuted = false;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_PORTAL_CONNECTOR_RECONNECT:
                        Reconnect();
                        events.ScheduleEvent(EVENT_PORTAL_CONNECTOR_RECONNECT, 10000);
                        break;
                    default:
                        break;
                }
            }

        private:
            EventMap events;
            uint64 connectedToGuid;
            uint64 portalGuid;
            bool actionExecuted; // Loop prevention

            Unit* GetConnection()
            {
                if (!connectedToGuid)
                    return NULL;
                return ObjectAccessor::GetUnit(*me, connectedToGuid);
            }
            Unit* GetPortal()
            {
                if (!portalGuid)
                    return NULL;
                return ObjectAccessor::GetUnit(*me, portalGuid);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetWoEAI<npc_well_of_eternity_portal_connectorAI>(creature);
        }
};

class npc_well_of_eternity_legion_portal : CreatureScript
{
    public:
        npc_well_of_eternity_legion_portal() : CreatureScript("npc_well_of_eternity_legion_portal") { }

        struct npc_well_of_eternity_legion_portalAI : public ScriptedAI
        {
            npc_well_of_eternity_legion_portalAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                shutdownRequested = false;
                events.Reset();
            }

            void DoAction(int32 action) OVERRIDE
            {
                switch (action)
                {
                    case ACTION_PORTAL_DISABLE:
                        DoCast(me, SPELL_PORTAL_SHUTDOWN_REQUEST);
                        DoCast(me, SPELL_PORTAL_STATUS_SHUTTING_DOWN);
                        shutdownRequested = true;
                        events.ScheduleEvent(EVENT_SHUTDOWN_CHECK, 1000);
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_SHUTDOWN_CHECK:
                        if (GetClosestCreatureWithEntry(me, NPC_LEGION_DEMON_PREEVENT, 75) || GetClosestCreatureWithEntry(me, NPC_GUARDIAN_DEMON, 25))
                            events.ScheduleEvent(EVENT_SHUTDOWN_CHECK, 1000);
                        else
                            me->DespawnOrUnsummon();
                        break;
                    default:
                        break;
                }
            }

        private:
            bool shutdownRequested;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetWoEAI<npc_well_of_eternity_legion_portalAI>(creature);
        }
};

class npc_well_of_eternity_portal_trash : public CreatureScript
{
    public:
        npc_well_of_eternity_portal_trash() : CreatureScript("npc_well_of_eternity_portal_trash") { }

        struct npc_well_of_eternity_portal_trashAI : public ScriptedAI
        {
            npc_well_of_eternity_portal_trashAI(Creature* creature) : ScriptedAI(creature)
            {
                isArcanist = me->GetEntry() == NPC_CORRUPTED_ARCANIST;
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                me->ApplySpellImmune(0, IMMUNITY_ID, SPELL_SHADOWCLOAK_AGGRO, true);
                me->ApplySpellImmune(0, IMMUNITY_ID, SPELL_MARCHING_DEMON_REACT, true);
            }

            void Reset() OVERRIDE
            {
                events.Reset();
                oneTimeEventDone = false;
                if (me->GetDBTableGUIDLow() == GUID_INTRO_TRASH_1 || me->GetDBTableGUIDLow() == GUID_INTRO_TRASH_2 || me->GetDBTableGUIDLow() == GUID_INTRO_TRASH_3)
                    if (InstanceScript* instance = me->GetInstanceScript())
                        if (instance->GetData(DATA_EVENT_DEMON) == DONE)
                            me->DespawnOrUnsummon(1000);
            }

            void EnterCombat(Unit* who) OVERRIDE
            {
                if (who->HasAura(SPELL_SHADOW_WALK_AURA) || who->HasAura(SPELL_SHADOWCLOAK_PETS))
                {
                    me->CombatStop(true);
                    me->getHostileRefManager().deleteReferences();
                    return;
                }

                events.ScheduleEvent(EVENT_PERIODIC_SPELL, isArcanist ? 1 : urand(5000, 7500));
                if (isArcanist)
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

                if (me->GetDBTableGUIDLow() != GUID_INTRO_TRASH_1 && me->GetDBTableGUIDLow() != GUID_INTRO_TRASH_2 && me->GetDBTableGUIDLow() != GUID_INTRO_TRASH_3)
                    DoCastAOE(SPELL_AGGRO_ILLIDAN, true);
            }

            void SpellHit(Unit* caster, const SpellInfo* spell) OVERRIDE
            {
                if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                    return;
                if (spell->Id == SPELL_SHADOWCLOAK_AGGRO)
                {
                    caster->RemoveAura(SPELL_SHADOW_WALK_AURA);
                    AttackStart(caster);
                }
            }

            void MoveInLineOfSight(Unit* who) OVERRIDE
            {
                if (me->IsInCombat())
                    return;

                if (who->GetTypeId() == TypeID::TYPEID_PLAYER || who->GetTypeId() == TypeID::TYPEID_UNIT && who->GetCharmerOrOwner() && who->GetCharmerOrOwner()->GetTypeId() == TypeID::TYPEID_PLAYER)
                {
                    if (Player* player = who->ToPlayer())
                        if (player->IsGameMaster())
                            return;
                    if (!me->CanStartAttack(who, false))
                        return;
                    if (who->HasAura(SPELL_SHADOW_WALK_AURA) || who->HasAura(SPELL_SHADOWCLOAK_PETS))
                        return;

                    AttackStart(who);
                }
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                if (Creature* stalker = GetClosestCreatureWithEntry(me, NPC_FEL_CRYSTAL_STALKER, 100))
                    stalker->AI()->DoAction(ACTION_FEL_CRYSTAL_STALKER_UNLOCK);

                if (InstanceScript* instance = me->GetInstanceScript())
                    if (instance->GetData(DATA_EVENT_DEMON) == NOT_STARTED)
                        if (Creature* demon = GetClosestCreatureWithEntry(me, NPC_LEGION_DEMON, 100))
                            demon->AI()->DoAction(ACTION_LEGION_DEMON_START);
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type == WAYPOINT_MOTION_TYPE && pointId == 4)
                    me->SetVisible(false);
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action != ACTION_INTRO_TRASH_LEAVE)
                    return;

                switch (me->GetDBTableGUIDLow())
                {
                    case GUID_INTRO_TRASH_1:
                    case GUID_INTRO_TRASH_2:
                        me->GetMotionMaster()->MovePath(me->GetEntry() * 100, false);
                        break;
                    case GUID_INTRO_TRASH_3:
                        me->GetMotionMaster()->MovePath(me->GetEntry() * 100 + 1, false);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                switch (events.ExecuteEvent())
                {
                    case EVENT_PERIODIC_SPELL:
                        DoCastVictim(isArcanist ? SPELL_ARCANE_ANNIHILATION : SPELL_CARRION_SWARM);
                        events.ScheduleEvent(EVENT_PERIODIC_SPELL, isArcanist ? urand(3400, 4700) : urand(15000, 22500));
                        break;
                    default:
                        break;
                }

                if (!oneTimeEventDone)
                {
                    if (isArcanist && me->GetPower(POWER_MANA) < me->GetMaxPower(POWER_MANA) * 0.11)
                    {
                        DoCastAOE(SPELL_INFINITE_MANA);
                        oneTimeEventDone = true;
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                    }
                    else if (!isArcanist)
                    {
                        if (Creature* arcanist = GetClosestCreatureWithEntry(me, NPC_CORRUPTED_ARCANIST, 40))
                        {
                            if (arcanist->HealthBelowPct(40))
                            {
                                DoCast(arcanist, SPELL_DEMONIC_WARDING);
                                oneTimeEventDone = true;
                            }
                        }
                    }
                }

                if (!isArcanist || !me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE))
                    DoMeleeAttackIfReady();
            }

        private:
            bool isArcanist;
            bool oneTimeEventDone;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetWoEAI<npc_well_of_eternity_portal_trashAI>(creature);
        }
};

class npc_well_of_eternity_fire_wall_stalker : public CreatureScript
{
    public:
        npc_well_of_eternity_fire_wall_stalker() : CreatureScript("npc_well_of_eternity_fire_wall_stalker") { }

        struct npc_well_of_eternity_fire_wall_stalkerAI : public ScriptedAI
        {
            npc_well_of_eternity_fire_wall_stalkerAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action != 1)
                    return;

                events.ScheduleEvent(EVENT_INTRO_TALK_1, 1);
                events.ScheduleEvent(EVENT_INTRO_TALK_2, 5000);
                events.ScheduleEvent(EVENT_INTRO_TALK_3, 11000);
                events.ScheduleEvent(EVENT_INTRO_TRASH_LEAVE, 13000);
                events.ScheduleEvent(EVENT_INTRO_LEGION_DEMON_MOVE, 15000);
                events.ScheduleEvent(EVENT_INTRO_CLOSE_WALL_1, 17000);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_INTRO_TALK_1:
                        if (Creature* perotharn = GetClosestCreatureWithEntry(me, NPC_PEROTHARN, 50))
                            perotharn->AI()->Talk(3);
                        break;
                    case EVENT_INTRO_TALK_2:
                        if (Creature* perotharn = GetClosestCreatureWithEntry(me, NPC_PEROTHARN, 50))
                            perotharn->AI()->Talk(4);
                        break;
                    case EVENT_INTRO_TALK_3:
                        if (Creature* perotharn = GetClosestCreatureWithEntry(me, NPC_PEROTHARN, 50))
                            perotharn->AI()->Talk(5);
                        break;
                    case EVENT_INTRO_TRASH_LEAVE:
                    {
                        if (Creature* perotharn = GetClosestCreatureWithEntry(me, NPC_PEROTHARN, 50))
                            perotharn->CastSpell(perotharn, SPELL_CAMOUFLAGE);
                        std::list<Creature*> trash;
                        GetCreatureListWithEntryInGrid(trash, me, NPC_CORRUPTED_ARCANIST, 50);
                        GetCreatureListWithEntryInGrid(trash, me, NPC_DREADLORD_DEFENDER, 50);
                        for (std::list<Creature*>::iterator itr = trash.begin(); itr != trash.end(); ++itr)
                            if (Creature* creature = *itr)
                                creature->AI()->DoAction(ACTION_INTRO_TRASH_LEAVE);
                        break;
                    }
                    case EVENT_INTRO_LEGION_DEMON_MOVE:
                        if (Creature* demon = GetClosestCreatureWithEntry(me, NPC_LEGION_DEMON, 50))
                            demon->AI()->DoAction(ACTION_LEGION_DEMON_START);
                        break;
                    case EVENT_INTRO_CLOSE_WALL_1:
                        if (Creature* perotharn = GetClosestCreatureWithEntry(me, NPC_PEROTHARN, 50))
                        {
                            perotharn->NearTeleportTo(3336.70f, -4893.37f, 181.07f, 2.14f);
                            perotharn->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        }
                        me->CastSpell(3204.58f, -4936.87f, 189.52f, SPELL_FIREWALL_COSMETIC, false);
                        me->CastSpell(3183.76f, -4936.87f, 189.52f, SPELL_FIREWALL_COSMETIC, false);
                        events.ScheduleEvent(EVENT_INTRO_CLOSE_WALL_2, 500);
                        break;
                    case EVENT_INTRO_CLOSE_WALL_2:
                        me->CastSpell(3200.05f, -4938.33f, 189.52f, SPELL_FIREWALL_COSMETIC, false);
                        me->CastSpell(3188.29f, -4938.33f, 189.52f, SPELL_FIREWALL_COSMETIC, false);
                        events.ScheduleEvent(EVENT_INTRO_CLOSE_WALL_3, 500);
                        break;
                    case EVENT_INTRO_CLOSE_WALL_3:
                        me->CastSpell(3197.13f, -4938.33f, 189.52f, SPELL_FIREWALL_COSMETIC, false);
                        me->CastSpell(3191.21f, -4938.69f, 189.52f, SPELL_FIREWALL_COSMETIC, false);
                        events.ScheduleEvent(EVENT_INTRO_CLOSE_WALL_4, 500);
                        break;
                    case EVENT_INTRO_CLOSE_WALL_4:
                        if (Creature* demon = GetClosestCreatureWithEntry(me, NPC_LEGION_DEMON, 50))
                        {
                            demon->SetReactState(REACT_AGGRESSIVE);
                            demon->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            demon->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                        }
                        if (InstanceScript* instance = me->GetInstanceScript())
                            instance->SetData(DATA_EVENT_DEMON, IN_PROGRESS);
                        me->CastSpell(3194.43f, -4939.09f, 189.52f, SPELL_FIREWALL_COSMETIC, false);
                        break;
                    default:
                        break;
                }
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetWoEAI<npc_well_of_eternity_fire_wall_stalkerAI>(creature);
        }
};

class npc_well_of_eternity_shadowcloak_stalker : public CreatureScript
{
    public:
        npc_well_of_eternity_shadowcloak_stalker() : CreatureScript("npc_well_of_eternity_shadowcloak_stalker") { }

        struct npc_well_of_eternity_shadowcloak_stalkerAI : public ScriptedAI
        {
            npc_well_of_eternity_shadowcloak_stalkerAI(Creature* creature) : ScriptedAI(creature)
            {
                enterVehicleTimer = 1000;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!enterVehicleTimer)
                {
                    if (!me->GetVehicleBase())
                        me->DespawnOrUnsummon();
                    return;
                }
                if (me->GetVehicleBase())
                {
                    enterVehicleTimer = 0;
                    return;
                }
                if (TempSummon* summon = me->ToTempSummon())
                {
                    if (!summon->GetSummoner())
                    {
                        enterVehicleTimer = 0;
                        me->DespawnOrUnsummon();
                        return;
                    }
                    if (enterVehicleTimer <= diff)
                    {
                        enterVehicleTimer = 1000;
                        if (summon->GetSummoner()->IsVehicle())
                            me->EnterVehicle(summon->GetSummoner(), 0);
                        else
                        {
                            summon->GetSummoner()->RemoveAurasDueToSpell(SPELL_SHADOW_WALK_AURA);
                            summon->GetSummoner()->CastSpell(summon->GetSummoner(), SPELL_SHADOW_WALK_AURA);
                            enterVehicleTimer = 0;
                            me->DespawnOrUnsummon();
                        }
                    } else enterVehicleTimer -= diff;
                }
            }

        private:
            uint32 enterVehicleTimer;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetWoEAI<npc_well_of_eternity_shadowcloak_stalkerAI>(creature);
        }
};

class go_npc_well_of_eternity_portal_energy_focus : public GameObjectScript
{
    public:
        go_npc_well_of_eternity_portal_energy_focus() : GameObjectScript("go_npc_well_of_eternity_portal_energy_focus") { }

        void OnLootStateChanged(GameObject* go, uint32 state, Unit* /*unit*/) OVERRIDE
        {
            if (state != uint32(LootState::GO_JUST_DEACTIVATED))
                return;
            if (go->HasFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE))
                return;
            go->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);

            if (Creature* stalker = GetClosestCreatureWithEntry(go, NPC_FEL_CRYSTAL_STALKER, 10))
                stalker->GetAI()->DoAction(ACTION_FEL_CRYSTAL_STALKER_MELTDOWN);

            uint32 guardianPath = 0;
            uint32 guardianDbGuid1 = 0, guardianDbGuid2 = 0;
            uint32 creditEntry = 0;

            switch (go->GetEntry())
            {
                case GO_PORTAL_ENERGY_FOCUS_1:
                    if (InstanceScript* instance = go->GetInstanceScript())
                        instance->SetData(DATA_LEGION_DEMON_BLOCKED_PATH_LEFT, 1);
                    guardianDbGuid1 = guardianDbGuid2 = GUID_GUARDIAN_LEFT;
                    guardianPath = 1;
                    creditEntry = 58239;
                    break;
                case GO_PORTAL_ENERGY_FOCUS_2:
                    if (InstanceScript* instance = go->GetInstanceScript())
                        instance->SetData(DATA_LEGION_DEMON_BLOCKED_PATH_RIGHT, 1);
                    guardianDbGuid1 = guardianDbGuid2 = GUID_GUARDIAN_RIGHT;
                    guardianPath = 2;
                    creditEntry = 58240;
                    break;
                case GO_PORTAL_ENERGY_FOCUS_3:
                    if (InstanceScript* instance = go->GetInstanceScript())
                    {
                        instance->SetData(DATA_LEGION_DEMON_BLOCKED_PATH_FORWARD, 1);
                        instance->SetData(DATA_EVENT_ILLIDAN_1, DONE);
                    }
                    guardianDbGuid1 = GUID_GUARDIAN_CENTER_1;
                    guardianDbGuid2 = GUID_GUARDIAN_CENTER_2;
                    guardianPath = 3;
                    creditEntry = 58241;
                    break;
                default:
                    return;
            }

            std::list<Creature*> guardians;
            GetCreatureListWithEntryInGrid(guardians, go, NPC_GUARDIAN_DEMON, 200);
            for (std::list<Creature*>::iterator itr = guardians.begin(); itr != guardians.end(); ++itr)
                if (Creature* guardian = *itr)
                    if (guardian->GetDBTableGUIDLow() == guardianDbGuid1 || guardian->GetDBTableGUIDLow() == guardianDbGuid2)
                        guardian->AI()->SetData(1, guardianPath++);

            if (InstanceScript* instance = go->GetInstanceScript())
                WoEKilledMonsterCredit(instance, QUEST_IN_UNENDING_NUMBERS, creditEntry);
        }
};

class at_well_of_eternity_intro : public AreaTriggerScript
{
    public:
        at_well_of_eternity_intro() : AreaTriggerScript("at_well_of_eternity_intro") { }

        bool OnTrigger(Player* player, const AreaTriggerEntry* /*trigger*/) OVERRIDE
        {
            if (InstanceScript* instance = player->GetInstanceScript())
            {
                if (instance->GetData(DATA_EVENT_DEMON) == NOT_STARTED)
                {
                    instance->SetData(DATA_EVENT_DEMON, SPECIAL);
                    if (Creature* nozdormu = GetClosestCreatureWithEntry(player, NPC_NOZDORMU, 100))
                        nozdormu->AI()->DoAction(ACTION_NOZDORMU_SKIP);
                }
            }
            return true;
        }
};

class at_well_of_eternity_skip_illidan_intro : public AreaTriggerScript
{
    public:
        at_well_of_eternity_skip_illidan_intro() : AreaTriggerScript("at_well_of_eternity_skip_illidan_intro") { }

        bool OnTrigger(Player* player, const AreaTriggerEntry* /*trigger*/) OVERRIDE
        {
            if (InstanceScript* instance = player->GetInstanceScript())
                if (instance->GetData(DATA_EVENT_ILLIDAN_1) == NOT_STARTED)
                    if (Creature* illidan = instance->instance->GetCreature(instance->GetData64(DATA_EVENT_ILLIDAN_1)))
                        illidan->AI()->DoAction(ACTION_ILLIDAN_1_START);
            return true;
        }
};

class at_well_of_eternity_perotharn_preevent_appear : public AreaTriggerScript
{
    public:
        at_well_of_eternity_perotharn_preevent_appear() : AreaTriggerScript("at_well_of_eternity_perotharn_preevent_appear") { }

        bool OnTrigger(Player* player, const AreaTriggerEntry* /*trigger*/) OVERRIDE
        {
            if (InstanceScript* instance = player->GetInstanceScript())
                if (instance->GetData(DATA_EVENT_ILLIDAN_1) == DONE && instance->GetBossState(DATA_PEROTHARN) == NOT_STARTED)
                    if (Creature* perotharn = instance->instance->GetCreature(instance->GetData64(DATA_PEROTHARN)))
                        if (perotharn->HasAura(SPELL_CAMOUFLAGE))
                            perotharn->AI()->DoAction(2); // ACTION_PREEVENT_APPEAR
            return true;
        }
};

class at_well_of_eternity_illidan_outro_resume : public AreaTriggerScript
{
    public:
        at_well_of_eternity_illidan_outro_resume() : AreaTriggerScript("at_well_of_eternity_illidan_outro_resume") { }

        bool OnTrigger(Player* player, const AreaTriggerEntry* /*trigger*/) OVERRIDE
        {
            if (InstanceScript* instance = player->GetInstanceScript())
                if (instance->GetData(DATA_EVENT_ILLIDAN_1) == DONE && instance->GetBossState(DATA_PEROTHARN) == DONE)
                    if (Creature* illidan = instance->instance->GetCreature(instance->GetData64(DATA_EVENT_ILLIDAN_1)))
                        illidan->AI()->DoAction(ACTION_ILLIDAN_1_OUTRO_RESUME);
            return true;
        }
};

class spell_well_of_eternity_shadow_walk : public SpellScriptLoader
{
    public:
        spell_well_of_eternity_shadow_walk() : SpellScriptLoader("spell_well_of_eternity_shadow_walk") { }

        class spell_well_of_eternity_shadow_walk_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_well_of_eternity_shadow_walk_AuraScript);

            bool Validate(SpellInfo const* /*entry*/) OVERRIDE
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SHADOW_AMBUSHER) || !sSpellMgr->GetSpellInfo(SPELL_SHADOW_AMBUSHER_BUFF) || !sSpellMgr->GetSpellInfo(SPELL_SHADOWCLOAK_PLAYERS) || !sSpellMgr->GetSpellInfo(SPELL_SHADOWCLOAK_PETS) || !sSpellMgr->GetSpellInfo(SPELL_SHADOW_WALK_STACKS))
                    return false;
                if (!sObjectMgr->GetCreatureTemplate(NPC_SHADOWCLOAK_STALKER))
                    return false;
                return true;
            }

            void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();

                target->CombatStop(true);
                target->getHostileRefManager().deleteReferences();

                target->CastSpell(target, SPELL_SHADOWCLOAK_PLAYERS);
                target->CastSpell(target, SPELL_SHADOWCLOAK_AGGRO);
                target->CastSpell(target, SPELL_SHADOW_AMBUSHER_BUFF);

                Position pos = target->GetPosition();
                if (Creature* stalker = target->SummonCreature(NPC_SHADOWCLOAK_STALKER, pos, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                    stalker->EnterVehicle(target);

                if (Guardian* pet = target->GetGuardianPet())
                    target->CastSpell(target, SPELL_SHADOWCLOAK_PETS);
            }

            void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();

                // Remove stalker
                if (target->IsVehicle() && target->GetVehicleKit())
                {
                    if (Unit* stalker = target->GetVehicleKit()->GetPassenger(0))
                    {
                        stalker->ExitVehicle();
                        stalker->ToCreature()->DespawnOrUnsummon();
                    }
                }

                // Remove Shadowcloak aura from player and pet
                target->RemoveAurasDueToSpell(SPELL_SHADOWCLOAK_PLAYERS);
                if (Guardian* pet = target->GetGuardianPet())
                    pet->RemoveAurasDueToSpell(SPELL_SHADOWCLOAK_PETS);

                // Remove Shadow Ambusher Stealth Buff
                target->RemoveAurasDueToSpell(SPELL_SHADOW_AMBUSHER_BUFF);

                // Apply Shadow Ambusher with custom duration
                if (target->HasAura(SPELL_SHADOW_WALK_STACKS))
                {
                    if (Aura* stacks = target->GetAura(SPELL_SHADOW_WALK_STACKS))
                        if (Aura* ambusher = target->AddAura(SPELL_SHADOW_AMBUSHER, target))
                            ambusher->SetDuration(stacks->GetStackAmount() * IN_MILLISECONDS);
                    target->RemoveAurasDueToSpell(SPELL_SHADOW_WALK_STACKS);
                }
            }

            void Register() OVERRIDE
            {
                AfterEffectApply += AuraEffectApplyFn(spell_well_of_eternity_shadow_walk_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_SET_VEHICLE_ID, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_well_of_eternity_shadow_walk_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_SET_VEHICLE_ID, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_well_of_eternity_shadow_walk_AuraScript();
        }
};

class spell_well_of_eternity_shadowcloak_illidan : public SpellScriptLoader
{
    public:
        spell_well_of_eternity_shadowcloak_illidan() : SpellScriptLoader("spell_well_of_eternity_shadowcloak_illidan") { }

        class spell_well_of_eternity_shadowcloak_illidan_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_well_of_eternity_shadowcloak_illidan_AuraScript);

            bool Validate(SpellInfo const* /*entry*/) OVERRIDE
            {
                if (!sObjectMgr->GetCreatureTemplate(NPC_SHADOWCLOAK_ILLIDAN_STALKER))
                    return false;
                return true;
            }

            void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();

                Position pos = target->GetPosition();
                if (Creature* stalker = target->SummonCreature(NPC_SHADOWCLOAK_ILLIDAN_STALKER, pos, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                    stalker->EnterVehicle(target);
            }

            void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();

                // Remove stalker
                if (target->IsVehicle() && target->GetVehicleKit())
                {
                    if (Unit* stalker = target->GetVehicleKit()->GetPassenger(0))
                    {
                        stalker->ExitVehicle();
                        stalker->ToCreature()->DespawnOrUnsummon();
                    }
                }
            }

            void Register() OVERRIDE
            {
                AfterEffectApply += AuraEffectApplyFn(spell_well_of_eternity_shadowcloak_illidan_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_SET_VEHICLE_ID, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_well_of_eternity_shadowcloak_illidan_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_SET_VEHICLE_ID, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_well_of_eternity_shadowcloak_illidan_AuraScript();
        }
};

class spell_well_of_eternity_fel_crystal_arc : public SpellScriptLoader
{
    public:
        spell_well_of_eternity_fel_crystal_arc() : SpellScriptLoader("spell_well_of_eternity_fel_crystal_arc") { }

        class spell_well_of_eternity_fel_crystal_arc_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_well_of_eternity_fel_crystal_arc_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                Skyfire::Containers::RandomResizeList(targets, 1);
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_well_of_eternity_fel_crystal_arc_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_well_of_eternity_fel_crystal_arc_SpellScript();
        }
};

class npc_well_of_eternity_eternal_champion : public CreatureScript
{
    public:
        npc_well_of_eternity_eternal_champion() : CreatureScript("npc_well_of_eternity_eternal_champion") { }

        struct npc_well_of_eternity_eternal_championAI : public ScriptedAI
        {
            npc_well_of_eternity_eternal_championAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_QUEENS_BLADE, urand(2000, 6000));
                events.ScheduleEvent(EVENT_SHIMMERING_STRIKE, urand(9000, 12000));
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_QUEENS_BLADE:
                            DoCastVictim(SPELL_QUEENS_BLADE);
                            events.ScheduleEvent(EVENT_QUEENS_BLADE, urand(6000, 8000));
                            break;
                        case EVENT_SHIMMERING_STRIKE:
                            DoCastAOE(SPELL_SHIMMERING_STRIKE);
                            events.ScheduleEvent(EVENT_SHIMMERING_STRIKE, urand(12000, 15000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetWoEAI<npc_well_of_eternity_eternal_championAI>(creature);
        }
};

class npc_well_of_eternity_eye_of_legion : public CreatureScript
{
    public:
        npc_well_of_eternity_eye_of_legion() : CreatureScript("npc_well_of_eternity_eye_of_legion") { }

        struct npc_well_of_eternity_eye_of_legionAI : public ScriptedAI
        {
            npc_well_of_eternity_eye_of_legionAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_FEL_FLAMES, urand(2000, 10000));
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_FEL_FLAMES:
                            DoCastVictim(SPELL_FEL_FLAMES);
                            events.ScheduleEvent(EVENT_FEL_FLAMES, urand(10000, 15000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetWoEAI<npc_well_of_eternity_eye_of_legionAI>(creature);
        }
};

class npc_well_of_eternity_enchanted_highmistress : public CreatureScript
{
    public:
        npc_well_of_eternity_enchanted_highmistress() : CreatureScript("npc_well_of_eternity_enchanted_highmistress") { }

        struct npc_well_of_eternity_enchanted_highmistressAI : public ScriptedAI
        {
            npc_well_of_eternity_enchanted_highmistressAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                if (me->GetEntry() == NPC_ENCHANTED_HIGHMISTRESS_1)
                {
                    events.ScheduleEvent(EVENT_FIREBALL, 1);
                    events.ScheduleEvent(EVENT_FIREBOMB, urand(7000, 12000));
                }
                else if (me->GetEntry() == NPC_ENCHANTED_HIGHMISTRESS_2)
                {
                    events.ScheduleEvent(EVENT_FROSTBOLT, 1);
                    events.ScheduleEvent(EVENT_BLIZZARD, urand(7000, 12000));
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_FIREBALL:
                            DoCastVictim(SPELL_FIREBALL);
                            events.ScheduleEvent(EVENT_FIREBALL, 2000);
                            break;
                        case EVENT_FIREBOMB:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_FIREBOMB);
                            events.ScheduleEvent(EVENT_FIREBOMB, urand(10000, 15000));
                            break;
                        case EVENT_FROSTBOLT:
                            DoCastVictim(SPELL_FROSTBOLT);
                            events.ScheduleEvent(EVENT_FROSTBOLT, 2000);
                            break;
                        case EVENT_BLIZZARD:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_BLIZZARD);
                            events.ScheduleEvent(EVENT_BLIZZARD, urand(20000, 25000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetWoEAI<npc_well_of_eternity_enchanted_highmistressAI>(creature);
        }
};

class npc_well_of_eternity_royal_handmaiden : public CreatureScript
{
    public:
        npc_well_of_eternity_royal_handmaiden() : CreatureScript("npc_well_of_eternity_royal_handmaiden") { }

        struct npc_well_of_eternity_royal_handmaidenAI : public ScriptedAI
        {
            npc_well_of_eternity_royal_handmaidenAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = me->GetInstanceScript();
            }

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_SWEET_LULLABY, urand(7000, 12000));
            }

            void SpellHit(Unit* caster, const SpellInfo* spell) OVERRIDE
            {
                if (spell->Id == SPELL_ARCHIVED_HANDMAIDEN_1)
                    DoCast(caster, SPELL_ARCHIVED_HANDMAIDEN_2, true);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                // Quest
                Map::PlayerList const &PlayerList = instance->instance->GetPlayers();
                if (!PlayerList.isEmpty())
                    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                        if (Player* player = i->GetSource())
                            if (me->GetDistance2d(player) <= 50.0f &&
                                player->GetQuestStatus(QUEST_DOCUMENTING_THE_TIMEWAYS) == QUEST_STATUS_INCOMPLETE)
                                player->CastSpell(me, SPELL_ARCHIVED_HANDMAIDEN_1, true);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SWEET_LULLABY:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_SWEET_LULLABY);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            EventMap events;
            InstanceScript* instance;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetWoEAI<npc_well_of_eternity_royal_handmaidenAI>(creature);
        }
};

class TrashRespawnWorker
{
    public:
        void operator()(Creature* creature) const
        {
            switch (creature->GetEntry())
            {
                case NPC_ABYSSAL:
                case NPC_VAROTHEN:
                case NPC_DOOMGUARD:
                    if (!creature->IsAlive())
                        creature->Respawn(true);
                    break;
            }
        }
};

static void TrashEvaded(Creature* creature)
{
    TrashRespawnWorker check;
    Skyfire::CreatureWorker<TrashRespawnWorker> worker(creature, check);
    creature->VisitNearbyGridObject(SIZE_OF_GRIDS, worker);
}

class npc_well_of_eternity_doomguard_annihilator : public CreatureScript
{
    public:
        npc_well_of_eternity_doomguard_annihilator() : CreatureScript("npc_well_of_eternity_doomguard_annihilator") { }

        struct npc_well_of_eternity_doomguard_annihilatorAI : public ScriptedAI
        {
            npc_well_of_eternity_doomguard_annihilatorAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE { }

            void EnterCombat(Unit* who) OVERRIDE
            {
                if (!who)
                    return;

                std::list<Creature*> creatures;
                GetCreatureListWithEntryInGrid(creatures, me, me->GetEntry(), 500.0f);
                if (!creatures.empty())
                    for (std::list<Creature*>::const_iterator itr = creatures.begin(); itr != creatures.end(); ++itr)
                        if (!(*itr)->IsInCombat())
                            (*itr)->AI()->AttackStart(who);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                if (InstanceScript* instance = me->GetInstanceScript())
                    if (Creature* pIllidan = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_EVENT_ILLIDAN_2)))
                        pIllidan->AI()->DoAction(ACTION_DOOMGUARD_DIED);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetWoEAI<npc_well_of_eternity_doomguard_annihilatorAI>(creature);
        }
};

class npc_well_of_eternity_illidan_2 : public CreatureScript
{
    public:
        npc_well_of_eternity_illidan_2() : CreatureScript("npc_well_of_eternity_illidan_2") { }

        struct npc_well_of_eternity_illidan_2AI : public ScriptedAI
        {
            npc_well_of_eternity_illidan_2AI(Creature* creature) : ScriptedAI(creature)
            {
                bTalk = false;
                bMove = false;
                curPoint = 0;
                doomguards = 0;
                instance = me->GetInstanceScript();
            }

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterEvadeMode() OVERRIDE { }

            void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
            {
                if (attacker->GetGUID() == me->GetGUID())
                    if (damage >= me->GetHealth())
                        damage = me->GetHealth() - 1;
            }

            void MoveInLineOfSight(Unit* who) OVERRIDE
            {
                if (bTalk || !instance)
                    return;

                if (who->GetTypeId() != TypeID::TYPEID_PLAYER)
                    return;

                if (me->GetDistance(who) > 10.0f)
                    return;

                if (instance->GetBossState(DATA_MANNOROTH) == DONE)
                    return;

                if (instance)
                    TrashEvaded(me);

                bTalk = true;

                Talk(SAY_ILLIDAN_2_INTRO_1);

                events.ScheduleEvent(EVENT_MALFURION_INTRO_1, 7000);
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type == POINT_MOTION_TYPE)
                {
                    switch (pointId)
                    {
                        case POINT_ILLIDAN_2_1:
                            curPoint = 1;
                            events.ScheduleEvent(EVENT_ILLIDAN_MOVE_2_2, 100);
                            break;
                        case POINT_ILLIDAN_2_2:
                            curPoint = 2;
                            events.ScheduleEvent(EVENT_ILLIDAN_MOVE_2_3, 100);
                            break;
                        case POINT_ILLIDAN_2_3:
                            curPoint = 3;
                            events.ScheduleEvent(EVENT_ILLIDAN_MOVE_2_4, 100);
                            break;
                        case POINT_ILLIDAN_2_4:
                            curPoint = 4;
                            events.ScheduleEvent(EVENT_ILLIDAN_MOVE_2_5, 100);
                            break;
                        case POINT_ILLIDAN_2_5:
                            curPoint = 5;
                            events.ScheduleEvent(EVENT_ILLIDAN_MOVE_2_6, 100);
                            break;
                        case POINT_ILLIDAN_2_6:
                            events.ScheduleEvent(EVENT_ILLIDAN_NEAR_ELEMENTAL_1, 3000);
                            break;
                        case POINT_ILLIDAN_2_8:
                            curPoint = 8;
                            events.ScheduleEvent(EVENT_ILLIDAN_MOVE_2_9, 100);
                            break;
                        case POINT_ILLIDAN_2_9:
                            if (Creature* pVarothen = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_VAROTHEN)))
                                pVarothen->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                            if (Creature* pMannoroth = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_MANNOROTH)))
                                pMannoroth->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                            Talk(SAY_ILLIDAN_2_VAROTHEN);
                            break;
                        default:
                            break;
                    }
                }
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action == ACTION_DOOMGUARD_DIED)
                {
                    doomguards++;

                    if (doomguards >= MAX_DOOMGUARDS && !bMove)
                    {
                        bMove = true;
                        curPoint = 0;
                        events.ScheduleEvent(EVENT_ILLIDAN_MOVE_2_1, 5000);
                    }
                }
                else if (action == ACTION_DOOMBRINGER_DIED)
                {
                    curPoint = 7;
                    me->RemoveAura(SPELL_WATERS_OF_ETERNITY);
                    events.ScheduleEvent(EVENT_ILLIDAN_MOVE_2_8, 5000);
                }
                else if (action == ACTION_MANNOROTH_AGGRO)
                {
                    curPoint = 9;
                    me->GetMotionMaster()->MovePoint(POINT_ILLIDAN_2_10, illidan2Pos[curPoint]);
                    if (Creature* pTyrande = me->FindNearestCreature(NPC_TYRANDE, 100.0f))
                        pTyrande->AI()->DoAction(ACTION_MANNOROTH_AGGRO);
                }
                else if (action == ACTION_MANNOROTH_RESET)
                {
                    me->RemoveAllAuras();
                    me->InterruptNonMeleeSpells(false);
                    events.Reset();
                }

            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!bTalk && !bMove)
                    return;

                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_MALFURION_INTRO_1:
                            if (Creature* pMalfurion = me->FindNearestCreature(NPC_MALFURION, 100.0f))
                                pMalfurion->AI()->Talk(SAY_MALFURION_INTRO_1);
                            events.ScheduleEvent(EVENT_MALFURION_INTRO_2, 7000);
                            break;
                        case EVENT_MALFURION_INTRO_2:
                            if (Creature* pMalfurion = me->FindNearestCreature(NPC_MALFURION, 100.0f))
                                pMalfurion->AI()->Talk(SAY_MALFURION_INTRO_2);
                            events.ScheduleEvent(EVENT_ILLIDAN_INTRO_2_2, 7000);
                            break;
                        case EVENT_ILLIDAN_INTRO_2_2:
                            Talk(SAY_ILLIDAN_2_INTRO_2);
                            events.ScheduleEvent(EVENT_TYRANDE_INTRO_1, 7000);
                            break;
                        case EVENT_TYRANDE_INTRO_1:
                            if (Creature* pTyrande = me->FindNearestCreature(NPC_TYRANDE, 100.0f))
                                pTyrande->AI()->Talk(SAY_TYRANDE_INTRO_1);
                            events.ScheduleEvent(EVENT_ILLIDAN_INTRO_2_3, 9000);
                            break;
                        case EVENT_ILLIDAN_INTRO_2_3:
                            Talk(SAY_ILLIDAN_2_INTRO_3);
                            events.ScheduleEvent(EVENT_TYRANDE_INTRO_2, 5000);
                            break;
                        case EVENT_TYRANDE_INTRO_2:
                            if (Creature* pTyrande = me->FindNearestCreature(NPC_TYRANDE, 100.0f))
                            {
                                pTyrande->AI()->Talk(SAY_TYRANDE_INTRO_2);
                                pTyrande->CastSpell(pTyrande, SPELL_BLESSING_OF_ELUNE);
                            }
                            break;
                        case EVENT_ILLIDAN_MOVE_2_1:
                            if (Creature* pTyrande = me->FindNearestCreature(NPC_TYRANDE, 100.0f))
                                pTyrande->RemoveAura(SPELL_BLESSING_OF_ELUNE);
                            Talk(SAY_ILLIDAN_2_DEMONS);
                            me->GetMotionMaster()->MovePoint(POINT_ILLIDAN_2_1, illidan2Pos[curPoint]);
                            break;
                        case EVENT_ILLIDAN_MOVE_2_2:
                            if (Creature* pTyrande = me->FindNearestCreature(NPC_TYRANDE, 100.0f))
                                pTyrande->AI()->DoAction(ACTION_TYRANDE_MOVE_1);
                            me->GetMotionMaster()->MovePoint(POINT_ILLIDAN_2_2, illidan2Pos[curPoint]);
                            break;
                        case EVENT_ILLIDAN_MOVE_2_3:
                            me->GetMotionMaster()->MovePoint(POINT_ILLIDAN_2_3, illidan2Pos[curPoint]);
                            break;
                        case EVENT_ILLIDAN_MOVE_2_4:
                            me->GetMotionMaster()->MovePoint(POINT_ILLIDAN_2_4, illidan2Pos[curPoint]);
                            break;
                        case EVENT_ILLIDAN_MOVE_2_5:
                            me->GetMotionMaster()->MovePoint(POINT_ILLIDAN_2_5, illidan2Pos[curPoint]);
                            break;
                        case EVENT_ILLIDAN_MOVE_2_6:
                            me->GetMotionMaster()->MovePoint(POINT_ILLIDAN_2_6, illidan2Pos[curPoint]);
                            break;
                        case EVENT_ILLIDAN_NEAR_ELEMENTAL_1:
                            Talk(SAY_ILLIDAN_2_ELEMENTAL_1);
                            events.ScheduleEvent(EVENT_ILLIDAN_NEAR_ELEMENTAL_2, 5000);
                            break;
                        case EVENT_ILLIDAN_NEAR_ELEMENTAL_2:
                            Talk(SAY_ILLIDAN_2_ELEMENTAL_2);
                            events.ScheduleEvent(EVENT_TYRANDE_NEAR_ELEMENTAL_1, 5000);
                            break;
                        case EVENT_TYRANDE_NEAR_ELEMENTAL_1:
                            if (Creature* pTyrande = me->FindNearestCreature(NPC_TYRANDE, 100.0f))
                                pTyrande->AI()->Talk(SAY_TYRANDE_ELEMENTAL);
                            events.ScheduleEvent(EVENT_ILLIDAN_NEAR_ELEMENTAL_3, 7000);
                            break;
                        case EVENT_ILLIDAN_NEAR_ELEMENTAL_3:
                            Talk(SAY_ILLIDAN_2_ELEMENTAL_3);
                            events.ScheduleEvent(EVENT_ILLIDAN_NEAR_ELEMENTAL_4, 5000);
                            break;
                        case EVENT_ILLIDAN_NEAR_ELEMENTAL_4:
                            DoCast(me, SPELL_WATERS_OF_ETERNITY);
                            events.ScheduleEvent(EVENT_ILLIDAN_NEAR_ELEMENTAL_5, 5000);
                            break;
                        case EVENT_ILLIDAN_NEAR_ELEMENTAL_5:
                            Talk(SAY_ILLIDAN_2_ELEMENTAL_4);
                            curPoint = 6;
                            events.ScheduleEvent(EVENT_ILLIDAN_MOVE_2_7, 14000);
                            break;
                        case EVENT_ILLIDAN_MOVE_2_7:
                            if (Creature* pAbyssal = me->FindNearestCreature(NPC_ABYSSAL, 100.0f))
                                pAbyssal->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                            me->GetMotionMaster()->MovePoint(POINT_ILLIDAN_2_7, illidan2Pos[curPoint]);
                            break;
                        case EVENT_ILLIDAN_MOVE_2_8:
                            if (Creature* pTyrande = me->FindNearestCreature(NPC_TYRANDE, 100.0f))
                                pTyrande->AI()->DoAction(ACTION_TYRANDE_MOVE_2);
                            me->GetMotionMaster()->MovePoint(POINT_ILLIDAN_2_8, illidan2Pos[curPoint]);
                            break;
                        case EVENT_ILLIDAN_MOVE_2_9:
                            me->GetMotionMaster()->MovePoint(POINT_ILLIDAN_2_9, illidan2Pos[curPoint]);
                            break;
                        default:
                            break;
                    }
                }
            }

        private:
            bool bTalk;
            bool bMove;
            EventMap events;
            InstanceScript* instance;
            uint8 curPoint;
            uint8 doomguards;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetWoEAI<npc_well_of_eternity_illidan_2AI>(creature);
        }
};

class npc_well_of_eternity_tyrande : public CreatureScript
{
    public:
        npc_well_of_eternity_tyrande() : CreatureScript("npc_well_of_eternity_tyrande") { }

        struct npc_well_of_eternity_tyrandeAI : public ScriptedAI
        {
            npc_well_of_eternity_tyrandeAI(Creature* creature) : ScriptedAI(creature)
            {
                curPoint = 0;
                instance = me->GetInstanceScript();
                bDebilitating = false;
                uiDebilitatingCount = 0;
            }

            void EnterEvadeMode() OVERRIDE { }

            void SpellHit(Unit* /*caster*/, const SpellInfo* spell) OVERRIDE
            {
                if (spell->Id == 104678/*SPELL_DEBILITATING_FLAY*/)
                    if (!bDebilitating)
                    {
                        bDebilitating = true;
                        events.CancelEvent(EVENT_TYRANDE_SHOT);
                        me->InterruptNonMeleeSpells(false);
                        Talk(SAY_TYRANDE_ELUNE);
                        DoCast(me, SPELL_BLESSING_OF_ELUNE);
                    }
            }

            void DoAction(int32 action) OVERRIDE
            {
                switch (action)
                {
                    case ACTION_TYRANDE_MOVE_1:
                        curPoint = 0;
                        events.ScheduleEvent(EVENT_TYRANDE_MOVE_1, 100);
                        break;
                    case ACTION_TYRANDE_MOVE_2:
                        curPoint = 5;
                        events.ScheduleEvent(EVENT_TYRANDE_MOVE_2, 100);
                        break;
                    case ACTION_MANNOROTH_AGGRO:
                        me->GetMotionMaster()->MovePoint(POINT_TYRANDE_3, tyrandePos);
                        break;
                    case ACTION_MANNOROTH_RESET:
                        me->RemoveAllAuras();
                        me->InterruptNonMeleeSpells(false);
                        events.Reset();
                        bDebilitating = false;
                        uiDebilitatingCount = 0;
                        break;
                    case ACTION_DEBILITATING_OFF:
                        if (bDebilitating)
                        {
                            uiDebilitatingCount++;
                            if (uiDebilitatingCount >= 2)
                            {
                                bDebilitating = false;
                                uiDebilitatingCount = 0;
                                me->RemoveAura(SPELL_BLESSING_OF_ELUNE);
                                Talk(SAY_TYRANDE_CONTINUE);
                                events.ScheduleEvent(EVENT_TYRANDE_SHOT, 1000);
                                if (Creature* pMannoroth = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_MANNOROTH)))
                                    pMannoroth->AI()->DoAction(2); // ACTION_DEBILITATING_OFF
                            }
                        }
                        break;
                    case ACTION_MANNOROTH_SARGERAS:
                        events.CancelEvent(EVENT_TYRANDE_SHOT);
                        events.ScheduleEvent(EVENT_HAND_OF_ELUNE, 1000);
                        break;
                    case ACTION_MANNOROTH_INFERNO:
                        events.CancelEvent(EVENT_TYRANDE_SHOT);
                        events.CancelEvent(EVENT_HAND_OF_ELUNE);
                        me->RemoveAura(SPELL_HAND_OF_ELUNE);
                        DoCastAOE(SPELL_WRATH_OF_ELUNE_2, true);
                        break;
                    case ACTION_MANNOROTH_END:
                        me->RemoveAllAuras();
                        me->InterruptNonMeleeSpells(false);
                        events.Reset();
                        bDebilitating = false;
                        uiDebilitatingCount = 0;
                        break;
                    default:
                        break;
                }
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type == POINT_MOTION_TYPE)
                {
                    if (pointId == POINT_TYRANDE_1)
                    {
                        if (curPoint < 4)
                        {
                            curPoint++;
                            events.ScheduleEvent(EVENT_TYRANDE_MOVE_1, 100);
                        }
                    }
                    else if (pointId == POINT_TYRANDE_2)
                    {
                        if (curPoint < 7)
                        {
                            curPoint++;
                            events.ScheduleEvent(EVENT_TYRANDE_MOVE_2, 100);
                        }
                    }
                    else if (pointId == POINT_TYRANDE_3)
                    {
                        Talk(SAY_TYRANDE_DEMONS);
                        events.ScheduleEvent(EVENT_TYRANDE_SHOT, 3000);
                    }
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_TYRANDE_MOVE_1:
                            me->GetMotionMaster()->MovePoint(POINT_TYRANDE_1, illidan2Pos[curPoint]);
                            break;
                        case EVENT_TYRANDE_MOVE_2:
                            me->GetMotionMaster()->MovePoint(POINT_TYRANDE_2, illidan2Pos[curPoint]);
                            break;
                        case EVENT_TYRANDE_SHOT:
                            DoCastAOE(SPELL_LUNAR_SHOT_3);
                            events.ScheduleEvent(EVENT_TYRANDE_SHOT, 3000);
                            break;
                        case EVENT_HAND_OF_ELUNE:
                            if (!me->HasAura(SPELL_DEBILITATING_FLAY))
                                DoCast(me, SPELL_HAND_OF_ELUNE);
                            else
                                events.ScheduleEvent(EVENT_HAND_OF_ELUNE, 1000);
                            break;
                        default:
                            break;
                    }
                }
            }

        private:
            EventMap events;
            uint8 curPoint;
            InstanceScript* instance;
            bool bDebilitating;
            uint8 uiDebilitatingCount;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetWoEAI<npc_well_of_eternity_tyrandeAI>(creature);
        }
};

class npc_well_of_eternity_malfurion : public CreatureScript
{
    public:
        npc_well_of_eternity_malfurion() : CreatureScript("npc_well_of_eternity_malfurion") { }

        struct npc_well_of_eternity_malfurionAI : public ScriptedAI
        {
            npc_well_of_eternity_malfurionAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE { }

            void EnterEvadeMode() OVERRIDE { }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetWoEAI<npc_well_of_eternity_malfurionAI>(creature);
        }
};

class npc_well_of_eternity_abyssal_doombringer : public CreatureScript
{
    public:
        npc_well_of_eternity_abyssal_doombringer() : CreatureScript("npc_well_of_eternity_abyssal_doombringer") { }

        struct npc_well_of_eternity_abyssal_doombringerAI : public ScriptedAI
        {
            npc_well_of_eternity_abyssal_doombringerAI(Creature* creature) : ScriptedAI(creature)
            {
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
            }

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_ABYSSAL_FLAMES, 5000);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                if (InstanceScript* instance = me->GetInstanceScript())
                    if (Creature* pIllidan = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_EVENT_ILLIDAN_2)))
                        pIllidan->AI()->DoAction(ACTION_DOOMBRINGER_DIED);

                me->DespawnOrUnsummon(3000);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    if (eventId == EVENT_ABYSSAL_FLAMES)
                    {
                        DoCastAOE(SPELL_ABYSSAL_FLAMES);
                        events.ScheduleEvent(EVENT_ABYSSAL_FLAMES, 3000);
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetWoEAI<npc_well_of_eternity_abyssal_doombringerAI>(creature);
        }
};

class npc_well_of_eternity_portal_to_twisting_nether : public CreatureScript
{
    public:
        npc_well_of_eternity_portal_to_twisting_nether() : CreatureScript("npc_well_of_eternity_portal_to_twisting_nether") { }

        struct npc_well_of_eternity_portal_to_twisting_netherAI : public ScriptedAI
        {
            npc_well_of_eternity_portal_to_twisting_netherAI(Creature* creature) : ScriptedAI(creature) { }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetWoEAI<npc_well_of_eternity_portal_to_twisting_netherAI>(creature);
        }
};

void AddSC_well_of_eternity()
{
    new npc_well_of_eternity_stalker();

    // Start of the instance
    new npc_well_of_eternity_nozdormu();
    new npc_well_of_eternity_legion_demon();

    // Pre Peroth'arn
    new npc_well_of_eternity_illidan_1();
    new npc_well_of_eternity_legion_demon_preevent();
    new npc_well_of_eternity_guardian_demon();
    new npc_well_of_eternity_fel_crystal_stalker();
    new npc_well_of_eternity_portal_connector();
    new npc_well_of_eternity_legion_portal();
    new npc_well_of_eternity_portal_trash();
    new npc_well_of_eternity_fire_wall_stalker();
    new npc_well_of_eternity_shadowcloak_stalker();
    new go_npc_well_of_eternity_portal_energy_focus();
    new at_well_of_eternity_intro();
    new at_well_of_eternity_perotharn_preevent_appear();
    // FIXME: if players don't move to first trash pack after event started, pass can be closed and players can't complete instance
    //new at_well_of_eternity_skip_illidan_intro();
    new at_well_of_eternity_illidan_outro_resume();
    new spell_well_of_eternity_shadow_walk();
    new spell_well_of_eternity_shadowcloak_illidan();
    new spell_well_of_eternity_fel_crystal_arc();

    // Pre Queen Azshara
    new npc_well_of_eternity_eternal_champion();
    new npc_well_of_eternity_eye_of_legion();
    new npc_well_of_eternity_enchanted_highmistress();
    new npc_well_of_eternity_royal_handmaiden();
    new npc_well_of_eternity_doomguard_annihilator();

    // Pre Mannoroth
    new npc_well_of_eternity_illidan_2();
    new npc_well_of_eternity_tyrande();
    new npc_well_of_eternity_malfurion();
    new npc_well_of_eternity_abyssal_doombringer();
    new npc_well_of_eternity_portal_to_twisting_nether();
}
