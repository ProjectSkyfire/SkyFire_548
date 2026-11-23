/*
 * This file is part of Project SkyFire https://www.projectskyfire.org.
 * See LICENSE.md file for Copyright information.
 * Ported from The Cataclysm Preservation Project (TrinityCore 4.3.4).
 */

#include "ScriptMgr.h"
#include "GameObject.h"
#include "InstanceScript.h"
#include "Map.h"
#include "sunken_temple.h"

enum SunkenTempleObjects
{
    GO_ATALAI_STATUE_FIRST = 148830,
    GO_ATALAI_STATUE_LAST = 148835,
    GO_ATALAI_LIGHT = 148883,
    GO_ATALAI_FINAL_LIGHT = 148937,
    NPC_ATALALARION = 8580
};

static Position const atalalarionPosition = { -466.5134f, 95.19822f, -189.6463f, 0.03490658f };
static Position const statuePositions[] =
{
    { -515.553f, 95.25821f, -173.707f, 0.0f },
    { -419.8487f, 94.48368f, -173.707f, 0.0f },
    { -491.4003f, 135.9698f, -173.707f, 0.0f },
    { -491.4909f, 53.48179f, -173.707f, 0.0f },
    { -443.8549f, 136.1007f, -173.707f, 0.0f },
    { -443.4171f, 53.83124f, -173.707f, 0.0f }
};

class instance_sunken_temple : public InstanceMapScript
{
public:
    instance_sunken_temple() : InstanceMapScript("instance_sunken_temple", 109) { }

    struct instance_sunken_temple_InstanceMapScript : public InstanceScript
    {
        instance_sunken_temple_InstanceMapScript(InstanceMap* map) : InstanceScript(map), nextStatue(0)
        {
            for (uint8 i = 0; i < 6; ++i)
                statues[i] = 0;
        }

        uint64 statues[6];
        uint8 nextStatue;

        void OnGameObjectCreate(GameObject* go) OVERRIDE
        {
            if (go->GetEntry() >= GO_ATALAI_STATUE_FIRST && go->GetEntry() <= GO_ATALAI_STATUE_LAST)
                statues[go->GetEntry() - GO_ATALAI_STATUE_FIRST] = go->GetGUID();
        }

        void SetData(uint32 type, uint32 data) OVERRIDE
        {
            if (type != EVENT_STATE || nextStatue >= 6 || data != GO_ATALAI_STATUE_FIRST + nextStatue)
                return;

            GameObject* statue = instance->GetGameObject(statues[nextStatue]);
            if (!statue)
                return;

            statue->SummonGameObject(GO_ATALAI_LIGHT, statue->GetPositionX(), statue->GetPositionY(), statue->GetPositionZ(), 0, 0, 0, 0, 0, 0);
            statue->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_INTERACT_COND);
            ++nextStatue;
            if (nextStatue == 6)
            {
                for (uint8 i = 0; i < 6; ++i)
                    statue->SummonGameObject(GO_ATALAI_FINAL_LIGHT, statuePositions[i].GetPositionX(), statuePositions[i].GetPositionY(), statuePositions[i].GetPositionZ(), 0, 0, 0, 0, 0, 0);
                statue->SummonCreature(NPC_ATALALARION, atalalarionPosition, TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 7200);
            }
        }
    };

    InstanceScript* GetInstanceScript(InstanceMap* map) const OVERRIDE
    {
        return new instance_sunken_temple_InstanceMapScript(map);
    }
};

void AddSC_instance_sunken_temple()
{
    new instance_sunken_temple();
}
