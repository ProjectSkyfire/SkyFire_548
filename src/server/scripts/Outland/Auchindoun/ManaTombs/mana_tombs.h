/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef MANA_TOMBS_H_
#define MANA_TOMBS_H_

#define MTScriptName "instance_mana_tombs"

uint32 const EncounterCount = 4;

enum DataTypes
{
    // Encounter States/Boss GUIDs
    DATA_PANDEMONIUS            = 0,
    DATA_TAVAROK                = 1,
    DATA_NEXUSPRINCE_SHAFFAR    = 2,
    DATA_YOR                    = 3
};

enum CreatureIds
{
};

enum GameObjectIds
{
};

template<class AI>
AI* GetManaTombsAI(Creature* creature)
{
    return GetInstanceAI<AI>(creature, MTScriptName);
}

#endif // MANA_TOMBS_H_
