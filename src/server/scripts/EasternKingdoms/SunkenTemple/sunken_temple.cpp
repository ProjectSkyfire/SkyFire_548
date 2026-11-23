/*
 * This file is part of Project SkyFire https://www.projectskyfire.org.
 * See LICENSE.md file for Copyright information.
 * Ported from The Cataclysm Preservation Project (TrinityCore 4.3.4).
 */

#include "ScriptMgr.h"
#include "GameObject.h"
#include "InstanceScript.h"
#include "Player.h"
#include "sunken_temple.h"

enum MalfurionMisc
{
    NPC_MALFURION_STORMRAGE = 15362,
    QUEST_THE_CHARGE_OF_DRAGONFLIGHTS = 8555,
    QUEST_ERANIKUS_TYRANT_OF_DREAMS = 8733
};

class at_malfurion_stormrage : public AreaTriggerScript
{
public:
    at_malfurion_stormrage() : AreaTriggerScript("at_malfurion_stormrage") { }

    bool OnTrigger(Player* player, AreaTriggerEntry const* /*at*/) OVERRIDE
    {
        if (player->GetInstanceScript() && !player->FindNearestCreature(NPC_MALFURION_STORMRAGE, 15.0f) &&
            player->GetQuestStatus(QUEST_THE_CHARGE_OF_DRAGONFLIGHTS) == QUEST_STATUS_REWARDED &&
            player->GetQuestStatus(QUEST_ERANIKUS_TYRANT_OF_DREAMS) != QUEST_STATUS_REWARDED)
            player->SummonCreature(NPC_MALFURION_STORMRAGE, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), -1.52f, TempSummonType::TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 100000);
        return false;
    }
};

class go_atalai_statue : public GameObjectScript
{
public:
    go_atalai_statue() : GameObjectScript("go_atalai_statue") { }

    bool OnGossipHello(Player* /*player*/, GameObject* go) OVERRIDE
    {
        if (InstanceScript* instance = go->GetInstanceScript())
            instance->SetData(EVENT_STATE, go->GetEntry());
        return false;
    }
};

void AddSC_sunken_temple()
{
    new at_malfurion_stormrage();
    new go_atalai_statue();
}
