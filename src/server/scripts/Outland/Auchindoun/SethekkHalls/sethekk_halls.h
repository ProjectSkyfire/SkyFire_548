/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SETHEKK_HALLS_H_
#define SETHEKK_HALLS_H_

#define SHScriptName "instance_sethekk_halls"

uint32 const EncounterCount             = 3;

enum DataTypes
{
    // Encounter States/Boss GUIDs
    DATA_DARKWEAVER_SYTH                = 0,
    DATA_TALON_KING_IKISS               = 1,
    DATA_ANZU                           = 2
};

enum CreatureIds
{
    NPC_ANZU                            = 23035,
    NPC_BROOD_OF_ANZU                   = 23132
};

enum GameObjectIds
{
    GO_IKISS_DOOR                       = 177203
};

template<class AI>
AI* GetSethekkHallsAI(Creature* creature)
{
    return GetInstanceAI<AI>(creature, SHScriptName);
}

#endif // SETHEKK_HALLS_H_
