/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef AHNKAHET_H_
#define AHNKAHET_H_

#define AhnKahetScriptName "instance_ahnkahet"

uint32 const EncounterCount = 5;

enum DataTypes
{
    // Encounter States/Boss GUIDs
    DATA_ELDER_NADOX                = 0,
    DATA_PRINCE_TALDARAM            = 1,
    DATA_JEDOGA_SHADOWSEEKER        = 2,
    DATA_AMANITAR                   = 3,
    DATA_HERALD_VOLAZJ              = 4,

    // Additional Data
    DATA_SPHERE_1                   = 5,
    DATA_SPHERE_2                   = 6,
    DATA_PRINCE_TALDARAM_PLATFORM   = 7,
    DATA_PL_JEDOGA_TARGET           = 8,
    DATA_ADD_JEDOGA_OPFER           = 9,
    DATA_ADD_JEDOGA_INITIAND        = 10,
    DATA_JEDOGA_TRIGGER_SWITCH      = 11,
    DATA_JEDOGA_RESET_INITIANDS     = 12,
    DATA_ALL_INITIAND_DEAD          = 13
};

enum CreatureIds
{
    NPC_ELDER_NADOX                 = 29309,
    NPC_PRINCE_TALDARAM             = 29308,
    NPC_JEDOGA_SHADOWSEEKER         = 29310,
    NPC_AMANITAR                    = 30258,
    NPC_HERALD_VOLAZJ               = 29311,

    // Elder Nadox
    NPC_AHNKAHAR_GUARDIAN           = 30176,
    NPC_AHNKAHAR_SWARMER            = 30178,

    // Jedoga Shadowseeker
    NPC_INITIAND                    = 30114,
    NPC_JEDOGA_CONTROLLER           = 30181,

    // Amanitar
    NPC_HEALTHY_MUSHROOM            = 30391,
    NPC_POISONOUS_MUSHROOM          = 30435,

    // Herald Volazj
    //NPC_TWISTED_VISAGE_1          = 30621,
    //NPC_TWISTED_VISAGE_2          = 30622,
    //NPC_TWISTED_VISAGE_3          = 30623,
    //NPC_TWISTED_VISAGE_4          = 30624,
    NPC_TWISTED_VISAGE              = 30625
};

enum GameObjectIds
{
    GO_PRINCE_TALDARAM_GATE         = 192236,
    GO_PRINCE_TALDARAM_PLATFORM     = 193564,
    GO_SPHERE_1                     = 193093,
    GO_SPHERE_2                     = 193094
};

template<class AI>
AI* GetAhnKahetAI(Creature* creature)
{
    return GetInstanceAI<AI>(creature, AhnKahetScriptName);
}

#endif // AHNKAHET_H_
