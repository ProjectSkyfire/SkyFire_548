/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef HALLS_OF_LIGHTNING_H_
#define HALLS_OF_LIGHTNING_H_

#define HoLScriptName "instance_halls_of_lightning"

uint32 const EncounterCount = 4;

enum DataTypes
{
    // Encounter States/Boss GUIDs
    DATA_BJARNGRIM          = 0,
    DATA_VOLKHAN            = 1,
    DATA_IONAR              = 2,
    DATA_LOKEN              = 3
};

enum CreaturesIds
{
    NPC_BJARNGRIM           = 28586,
    NPC_VOLKHAN             = 28587,
    NPC_IONAR               = 28546,
    NPC_LOKEN               = 28923
};

enum GameObjectIds
{
    GO_BJARNGRIM_DOOR       = 191416,
    GO_VOLKHAN_DOOR         = 191325,
    GO_IONAR_DOOR           = 191326,
    GO_LOKEN_DOOR           = 191324,
    GO_LOKEN_THRONE         = 192654
};

template<class AI>
AI* GetHallsOfLightningAI(Creature* creature)
{
    return GetInstanceAI<AI>(creature, HoLScriptName);
}

#endif // HALLS_OF_LIGHTNING_H_
