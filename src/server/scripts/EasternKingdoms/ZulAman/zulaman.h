/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef DEF_ZULAMAN_H
#define DEF_ZULAMAN_H

uint32 const EncounterCount = 6;
#define ZulAmanScriptName "instance_zulaman"

enum DataTypes
{
    // BossState
    DATA_AKILZON                = 0,
    DATA_NALORAKK               = 1,
    DATA_JANALAI                = 2,
    DATA_HALAZZI                = 3,
    DATA_HEXLORD                = 4,
    DATA_DAAKARA                = 5,

    // Data64
    DATA_HEXLORD_TRIGGER,

    DATA_STRANGE_GONG,
    DATA_MASSIVE_GATE,

    // SetData
    DATA_ZULAMAN_STATE
};

enum CreatureIds
{
    NPC_AKILZON                 = 23574,
    NPC_NALORAKK                = 23576,
    NPC_JANALAI                 = 23578,
    NPC_HALAZZI                 = 23577,
    NPC_HEXLORD                 = 24239,
    NPC_DAAKARA                 = 23863,

    NPC_VOLJIN                  = 52924,
    NPC_HEXLORD_TRIGGER         = 24363
};

enum GameObjectIds
{
    GO_STRANGE_GONG             = 187359,
    GO_MASSIVE_GATE             = 186728,
};

enum ZulAmanEvents
{
    EVENT_START_ZULAMAN         = 15897,
    EVENT_UPDATE_ZULAMAN_TIMER  = 1,
};

enum ZulAmanAction
{
    ACTION_START_ZULAMAN        = 1
};

enum ZulAmanWorldStates
{
    WORLD_STATE_ZULAMAN_TIMER_ENABLED   = 3104,
    WORLD_STATE_ZULAMAN_TIMER           = 3106,
};

template<class AI>
CreatureAI* GetZulAmanAI(Creature* creature)
{
    if (InstanceMap* instance = creature->GetMap()->ToInstanceMap())
        if (instance->GetInstanceScript())
            if (instance->GetScriptId() == sObjectMgr->GetScriptId(ZulAmanScriptName))
                return new AI(creature);
    return NULL;
}

#endif
