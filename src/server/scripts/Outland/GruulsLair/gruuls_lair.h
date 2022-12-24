/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef GRUULS_LAIR_H_
#define GRUULS_LAIR_H_

#define GLScriptName "instance_gruuls_lair"

uint32 const EncounterCount = 2;

enum DataTypes
{
    // Encounter States/Boss GUIDs
    DATA_MAULGAR                = 0,
    DATA_GRUUL                  = 1
};

enum CreatureIds
{
    NPC_MAULGAR                 = 18831,
    NPC_KROSH_FIREHAND          = 18832,
    NPC_OLM_THE_SUMMONER        = 18834,
    NPC_KIGGLER_THE_CRAZED      = 18835,
    NPC_BLINDEYE_THE_SEER       = 18836
};

enum GameObjectIds
{
    GO_MAULGAR_DOOR             = 184468,
    GO_GRUUL_DOOR               = 184662
};

template<class AI>
AI* GetGruulsLairAI(Creature* creature)
{
    return GetInstanceAI<AI>(creature, GLScriptName);
}

#endif // GRUULS_LAIR_H_
