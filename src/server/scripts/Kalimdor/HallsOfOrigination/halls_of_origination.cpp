/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "GameObjectAI.h"
#include "GossipDef.h"
#include "InstanceScript.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Transport.h"
#include "halls_of_origination.h"

enum LiftOfTheMakers
{
    ACTION_FIRST_FLOOR  = 0,
    ACTION_SECOND_FLOOR = 1,
};

struct go_hoo_the_makers_lift_controller : public GameObjectAI
{
    go_hoo_the_makers_lift_controller(GameObject* go) : GameObjectAI(go) { }

    bool GossipSelect(Player* player, uint32 /*sender*/, uint32 action) OVERRIDE
    {
        player->PlayerTalkClass->ClearMenus();
        player->PlayerTalkClass->SendCloseGossip();

        InstanceScript* instance = go->GetInstanceScript();
        if (!instance)
            return false;

        GameObject* lift = ObjectAccessor::GetGameObject(*go, instance->GetData64(DATA_LIFT_OF_THE_MAKERS_GUID));
        if (!lift)
            return false;

        // The core only knows the first stop frame of a lift, so the third floor is not reachable yet
        switch (action)
        {
            case ACTION_FIRST_FLOOR:
                lift->SetGoState(GOState::GO_STATE_TRANSPORT_ACTIVE);
                break;
            case ACTION_SECOND_FLOOR:
                lift->SetGoState(GOState::GO_STATE_TRANSPORT_STOPPED);
                break;
            default:
                break;
        }

        return false;
    }
};

class transport_hoo_lift_of_the_makers : public TransportScript
{
    public:
        transport_hoo_lift_of_the_makers() : TransportScript("transport_hoo_lift_of_the_makers") { }

        void OnCreate(Transport* transport) OVERRIDE
        {
            // Local transports get no zone script, so the instance learns of the lift here. The lift is created at
            // its far stop, but it waits at the first floor.
            transport->SetGoState(GOState::GO_STATE_TRANSPORT_ACTIVE);

            if (InstanceScript* instance = transport->GetInstanceScript())
                instance->SetData64(DATA_LIFT_OF_THE_MAKERS_GUID, transport->GetGUID());
        }
};

void AddSC_halls_of_origination()
{
    RegisterGameObjectAI(go_hoo_the_makers_lift_controller);
    new transport_hoo_lift_of_the_makers();
}
