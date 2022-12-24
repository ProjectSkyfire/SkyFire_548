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
    DATA_STONESTEP                  = 1,
    DATA_FLAMEHEART                 = 2,
    DATA_SHA_OF_DOUBT               = 3,
};

enum CreatureIds
{
    NPC_WISE_MARI                   = 56448,
    NPC_STONESTEP                   = 56843,
    NPC_FLAMEHEART                  = 56732,
    NPC_SHA_OF_DOUBT                = 56439,
};

enum GameObjectIds
{
    // doors
};

template<class AI>
AI* GetJadeSerpentAI(Creature* creature)
{
    return GetInstanceAI<AI>(creature, JadeSerpentScriptName);
}

#endif // JADE_SERPENT_H_