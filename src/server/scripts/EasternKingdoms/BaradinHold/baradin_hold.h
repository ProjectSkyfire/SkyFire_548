/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef DEF_BARADIN_HOLD_H_
#define DEF_BARADIN_HOLD_H_

#include "Map.h"
#include "Creature.h"
#include "ObjectMgr.h"

#define BHScriptName "instance_baradin_hold"

uint32 const EncounterCount = 3;

enum DataTypes
{
    DATA_ARGALOTH           = 0,
    DATA_OCCUTHAR           = 1,
    DATA_ALIZABAL           = 2
};

enum CreatureIds
{
    BOSS_ARGALOTH           = 47120,
    BOSS_OCCUTHAR           = 52363,
    BOSS_ALIZABAL           = 55869,

    NPC_EYE_OF_OCCUTHAR     = 52389,
    NPC_FOCUS_FIRE_DUMMY    = 52369,
    NPC_OCCUTHAR_EYE        = 52368
};

enum GameObjectIds
{
    GO_ARGALOTH_DOOR        = 207619,
    GO_OCCUTHAR_DOOR        = 208953,
    GO_ALIZABAL_DOOR        = 209849
};

template<class AI>
CreatureAI* GetBaradinHoldAI(Creature* creature)
{
    if (InstanceMap* instance = creature->GetMap()->ToInstanceMap())
        if (instance->GetInstanceScript())
            if (instance->GetScriptId() == sObjectMgr->GetScriptId(BHScriptName))
                return new AI(creature);
    return NULL;
}

#endif
