/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef HALLS_OF_STONE_H_
#define HALLS_OF_STONE_H_

#define HoSScriptName "instance_halls_of_stone"

uint32 const EncounterCount = 4;

enum DataTypes
{
    // Encounter States/Boss GUIDs
    DATA_KRYSTALLUS             = 0,
    DATA_MAIDEN_OF_GRIEF        = 1,
    DATA_BRANN_EVENT            = 2,
    DATA_SJONNIR                = 3,

    // Additional data
    DATA_KADDRAK                = 4,
    DATA_MARNAK                 = 5,
    DATA_ABEDNEUM               = 6,
    DATA_GO_TRIBUNAL_CONSOLE    = 7,
    DATA_GO_KADDRAK             = 8,
    DATA_GO_MARNAK              = 9,
    DATA_GO_ABEDNEUM            = 10,
    DATA_GO_SKY_FLOOR           = 11
};

enum CreatureIds
{
    NPC_MAIDEN                  = 27975,
    NPC_KRYSTALLUS              = 27977,
    NPC_SJONNIR                 = 27978,
    NPC_MARNAK                  = 30897,
    NPC_KADDRAK                 = 30898,
    NPC_ABEDNEUM                = 30899,
    NPC_BRANN                   = 28070
};

enum GameObjectIds
{
    GO_ABEDNEUM                 = 191669,
    GO_MARNAK                   = 191670,
    GO_KADDRAK                  = 191671,
    GO_MAIDEN_DOOR              = 191292,
    GO_BRANN_DOOR               = 191295,
    GO_SJONNIR_DOOR             = 191296,
    GO_TRIBUNAL_CONSOLE         = 193907,
    GO_TRIBUNAL_CHEST           = 190586,
    GO_TRIBUNAL_CHEST_HERO      = 193996,
    GO_TRIBUNAL_SKY_FLOOR       = 191527
};

template<class AI>
AI* GetHallsOfStoneAI(Creature* creature)
{
    return GetInstanceAI<AI>(creature, HoSScriptName);
}

#endif // HALLS_OF_STONE_H_
