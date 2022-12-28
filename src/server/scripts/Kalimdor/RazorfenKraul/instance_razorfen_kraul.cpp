/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Instance_Razorfen_Kraul
SD%Complete:
SDComment:
SDCategory: Razorfen Kraul
EndScriptData */

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "razorfen_kraul.h"
#include "Player.h"

#define WARD_KEEPERS_NR 2

class instance_razorfen_kraul : public InstanceMapScript
{
public:
    instance_razorfen_kraul() : InstanceMapScript("instance_razorfen_kraul", 47) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const override
    {
        return new instance_razorfen_kraul_InstanceMapScript(map);
    }

    struct instance_razorfen_kraul_InstanceMapScript : public InstanceScript
    {
        instance_razorfen_kraul_InstanceMapScript(Map* map) : InstanceScript(map) { }

        uint64 DoorWardGUID;
        int WardKeeperDeath;

        void Initialize() override
        {
            WardKeeperDeath = 0;
            DoorWardGUID = 0;
        }

        Player* GetPlayerInMap()
        {
            Map::PlayerList const& players = instance->GetPlayers();

            if (!players.isEmpty())
            {
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                {
                    if (Player* player = itr->GetSource())
                        return player;
                }
            }
            SF_LOG_DEBUG("scripts", "Instance Razorfen Kraul: GetPlayerInMap, but PlayerList is empty!");
            return NULL;
        }

        void OnGameObjectCreate(GameObject* go) override
        {
            switch (go->GetEntry())
            {
                case 21099: DoorWardGUID = go->GetGUID(); break;
                case 20920: go->SetUInt32Value(GAMEOBJECT_FIELD_FACTION_TEMPLATE, 0); break; // big fat fugly hack
            }
        }

        void Update(uint32 /*diff*/) override
        {
            if (WardKeeperDeath == WARD_KEEPERS_NR)
                if (GameObject* go = instance->GetGameObject(DoorWardGUID))
                {
                    go->SetUInt32Value(GAMEOBJECT_FIELD_FLAGS, 33);
                    go->SetGoState(GOState::GO_STATE_ACTIVE);
                }
        }

        void SetData(uint32 type, uint32 /*data*/) override
        {
            switch (type)
            {
                case EVENT_WARD_KEEPER: WardKeeperDeath++; break;
            }
        }
    };
};

void AddSC_instance_razorfen_kraul()
{
    new instance_razorfen_kraul();
}
