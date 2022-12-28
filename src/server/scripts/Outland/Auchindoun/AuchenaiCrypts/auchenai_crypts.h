/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef AUCHENAI_CRYPTS_H_
#define AUCHENAI_CRYPTS_H_

#define ACScriptName "instance_auchenai_crypts"

uint32 const EncounterCount = 2;

enum DataTypes
{
    // Encounter States/Boss GUIDs
    DATA_SHIRRAK_THE_DEAD_WATCHER   = 0,
    DATA_EXARCH_MALADAAR            = 1
};

enum CreatureIds
{
};

enum GameObjectIds
{
};

template<class AI>
AI* GetAuchenaiCryptsAI(Creature* creature)
{
    return GetInstanceAI<AI>(creature, ACScriptName);
}

#endif // AUCHENAI_CRYPTS_H_
