/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "Player.h"
#include "InstanceScript.h"
#include "ruins_of_ahnqiraj.h"

class npc_andorov : public CreatureScript
{
    public:
        npc_andorov() : CreatureScript("npc_andorov") { }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            if (InstanceScript* instance = creature->GetInstanceScript())
                if (instance->GetData(DATA_RAJAXX_EVENT) == NOT_STARTED)
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Let's start the battle!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) OVERRIDE
        {
            if (sender == GOSSIP_SENDER_MAIN && action == GOSSIP_ACTION_INFO_DEF)
                if (InstanceScript* instance = creature->GetInstanceScript())
                    if (instance->GetData(DATA_RAJAXX_EVENT) == NOT_STARTED)
                        instance->SetData(DATA_RAJAXX_EVENT, IN_PROGRESS);

            player->CLOSE_GOSSIP_MENU();
            return true;
        }

        struct npc_andorovAI : public ScriptedAI
        {
            npc_andorovAI(Creature* creature) : ScriptedAI(creature) { }

            void UpdateAI(uint32 /*diff*/) OVERRIDE
            {
                if (InstanceScript* instance = me->GetInstanceScript())
                    if (instance->GetData(DATA_RAJAXX_EVENT) == NOT_STARTED && !me->IsInEvadeMode() &&
                        (me->IsInCombat() || me->GetDistance(me->GetHomePosition()) > 1.0f))
                    {
                        EnterEvadeMode();
                        return;
                    }

                if (UpdateVictim())
                    DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_andorovAI(creature);
        }
};

class npc_kaldorei_elite : public CreatureScript
{
    public:
        npc_kaldorei_elite() : CreatureScript("npc_kaldorei_elite") { }

        struct npc_kaldorei_eliteAI : public ScriptedAI
        {
            npc_kaldorei_eliteAI(Creature* creature) : ScriptedAI(creature) { }

            void UpdateAI(uint32 /*diff*/) OVERRIDE
            {
                if (UpdateVictim())
                {
                    DoMeleeAttackIfReady();
                    return;
                }

                if (!me->IsInCombat() && !me->IsInEvadeMode() &&
                    me->GetMotionMaster()->GetCurrentMovementGeneratorType() != FOLLOW_MOTION_TYPE)
                    if (Creature* andorov = GetClosestCreatureWithEntry(me, 15471, 100.0f))
                        me->GetMotionMaster()->MoveFollow(andorov, 3.0f, (me->GetDBTableGUIDLow() % 4) * 1.5707963f);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_kaldorei_eliteAI(creature);
        }
};

void AddSC_npc_andorov()
{
    new npc_andorov();
    new npc_kaldorei_elite();
}
