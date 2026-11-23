/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "Player.h"
#include "CreatureTextMgr.h"
#include "SpellScript.h"
#include "zulaman.h"

// Striking the gong opens the Massive Gate and starts the speed run timer
class go_strange_gong : public GameObjectScript
{
    public:
        go_strange_gong() : GameObjectScript("go_strange_gong") { }

        bool OnGossipHello(Player* /*player*/, GameObject* go) OVERRIDE
        {
            if (InstanceScript* instance = go->GetInstanceScript())
                if (instance->GetData(DATA_ZULAMAN_STATE) == NOT_STARTED)
                    instance->SetData(DATA_ZULAMAN_STATE, IN_PROGRESS);

            return false;
        }
};

void AddSC_zulaman()
{
    new go_strange_gong();
}
