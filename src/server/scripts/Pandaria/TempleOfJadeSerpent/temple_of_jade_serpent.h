/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef JADE_SERPENT_H_
#define JADE_SERPENT_H_

#define JadeSerpentScriptName "instance_temple_of_jade_serpent"

uint32 const EncounterCount = 4;

enum DataTypes
{
    // Encounter States/Boss GUIDs
    DATA_WISE_MARI                  = 0,
    DATA_LOREWALKER                 = 1,
    DATA_LIU                        = 2,
    DATA_SHA_OF_DOUBT               = 3,

    // Additional data
    DATA_SUN                        = 5,
    DATA_ZAO                        = 6,
    DATA_TRIAL                      = 7,
    DATA_SUN_STATE                  = 8,
    DATA_TWIN_DRAGONS_DOOR          = 9,
    DATA_LIU_STATE                  = 10,
    MAX_DATA
};

enum CreatureIds
{
    NPC_WISE_MARI                   = 56448,
    NPC_LOREWALKER_STONESTEP        = 56843,
    NPC_LIU_FLAMEHEART              = 56732,
    NPC_SHA_OF_DOUBT                = 56439,

    NPC_ZAO_SUN                     = 56915,
    NPC_ZAO_SUN_TRIGGER             = 56861,
    NPC_SCROLL                      = 57080,
    NPC_MINION_OF_DOUBT             = 57109,
    NPC_LESSER_SHA                  = 58319,
    NPC_ZAO_SUNSEEKER               = 58826,
    NPC_HAUNTING_SHA                = 58856,
    NPC_HAUNTING_SHA_2              = 58865,
    NPC_HAUNTING_SHA_3              = 59555,
    NPC_OSONG                       = 56872,
    NPC_STRIFE                      = 59726,
    NPC_PERIL                       = 59051,
    NPC_YU_LON                      = 56762,
    NPC_JADE_FIRE                   = 56893,
    NPC_FIGMENT_OF_DOUBT            = 56792,
    NPC_WISE_DRAGON                 = 59149,
    NPC_WELL                        = 56882,
    NPC_STRIF                       = 56871,
    NPC_HEROINE                     = 56873,
    NPC_MOON                        = 56874,
    NPC_GOLDEN_BEETLE               = 59545,
    NPC_CRYBABY_HOZEN               = 59552,
    NPC_THE_NODDING_TIGER           = 59544,
    NPC_THE_SONGBIRD_QUEEN          = 59553,
    NPC_TALKING_FISH                = 59546,
    NPC_JIANG                       = 59547,
    NPC_XIANG                       = 65317,
    NPC_SWIRLING_SUNFIRE            = 58815,
    NPC_FOUNTAIN_TRIGGER            = 56586,
    NPC_CORRUPT_DROPLET             = 62358,
    NPC_HYDROLANCE_TRIGGER          = 56542,
    NPC_CORRUPT_WATER_WISE          = 56511,
    NPC_WISE_STALKER                = 65393,
};

enum GameObjectIds
{
    // doors
    GO_DOOR_WISE_MARI               = 213545,
    GO_DOOR_LOREWALKER_STONESTEP    = 213549,
    GO_DOOR_SHA_OF_DOUBT            = 213548,
    GO_DOOR_LIU_FLAMEHEART          = 213544,
    GO_DOOR_WISE_MARI_EXIT          = 213550,
};

enum LorewalkerEventTypes
{
    EVENT_LOREWALKER_STONESTEP_SUNS  = 0,
    EVENT_LOREWALKER_STONESTEP_TRIAL = 1,
};

enum InstanceSpells
{
    SPELL_SHA_CORRUPTION            = 107034,
    SPELL_SHA_CORRUPTION_2          = 120000,
    SPELL_ROOT_SELF                 = 106822,
    SPELL_LOREWALKER_ALACRITY       = 122714,
    SPELL_MEDITATION                = 122715,
    SPELL_DRAW_SHA                  = 111393,
    SPELL_DRAW_SHA_2                = 111431,
    SPELL_EXTRACT_SHA               = 111768,
    SPELL_JUGGLER_JIANG             = 114745,
    SPELL_SINGING_SONGBIRD          = 114789,
    SPELL_HAUNTING_GAZE             = 114650,
    SPELL_JUGGLER_XIANG             = 114718,
    SPELL_WATER_BUBBLE              = 114549,
    SPELL_POSSESSED_BY_SHA          = 110164,
    SPELL_JADE_ESSENCE              = 106797,
    SPELL_TRANSFORM_VISUAL          = 74620,
    SPELL_FIGMENT_OF_DOUBT_CLONE    = 106935,
    SPELL_COPY_WEAPON               = 41054,
    SPELL_COPY_WEAPON_2             = 41055,
    SPELL_SUNFIRE_EXPLOSION         = 111737,
    SPELL_BLUSTERING_VORTEX         = 67422,
    SPELL_JADE_FIRE_PERIODIC        = 107108,
};

uint32 const LiuSummonDeathCount = 17;

Position const LiuSpawnPosition = { 929.787f, -2561.016f, 185.070f, 1.181f };
Position const WiseMariRoomCenter = { 1046.941f, -2560.606f, 174.9552f, 4.33f };

Position const MinionOfDoubtSummonPos[3] =
{
    { 900.559f, -2620.396f, 185.125f, 1.195f },
    { 917.999f, -2626.064f, 185.125f, 1.274f },
    { 909.022f, -2623.146f, 185.125f, 1.451f },
};

Position const SunSummonPos[5] =
{
    { 830.067f, -2466.660f, 179.240f, 0.0f },
    { 836.632f, -2467.159f, 178.139f, 0.0f },
    { 839.659f, -2469.159f, 182.496f, 0.0f },
    { 845.263f, -2469.179f, 178.209f, 0.0f },
    { 850.361f, -2474.320f, 178.196f, 0.0f },
};

Position const SunTriggerSummonPos[5] =
{
    { 830.067f, -2466.660f, 176.320f, 0.0f },
    { 836.632f, -2467.159f, 176.320f, 0.0f },
    { 839.659f, -2469.159f, 176.320f, 0.0f },
    { 845.263f, -2469.179f, 176.320f, 0.0f },
    { 850.361f, -2474.320f, 176.320f, 0.0f },
};

Position const FountainTriggerSummonPos[4] =
{
    { 1059.94f, -2581.65f, 176.14f,  0.0f },
    { 1023.31f, -2569.70f, 176.034f, 0.0f },
    { 1022.74f, -2544.29f, 173.776f, 0.0f },
    { 1075.23f, -2561.33f, 173.876f, 0.0f },
};

template<class AI>
AI* GetJadeSerpentAI(Creature* creature)
{
    return GetInstanceAI<AI>(creature, JadeSerpentScriptName);
}

#endif // JADE_SERPENT_H_
