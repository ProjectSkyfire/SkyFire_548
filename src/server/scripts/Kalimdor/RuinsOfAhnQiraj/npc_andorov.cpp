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
                    {
                        // he leads his elites to the first wave; the battle starts when it is engaged
                        creature->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                        creature->SetWalk(false);
                        creature->GetMotionMaster()->MovePoint(0, -8978.11f, 1550.36f, 21.6f);
                    }

            player->CLOSE_GOSSIP_MENU();
            return true;
        }

        struct npc_andorovAI : public ScriptedAI
        {
            npc_andorovAI(Creature* creature) : ScriptedAI(creature), _lastState(NOT_STARTED)
            {
                _spawnPos = me->GetHomePosition();
            }

            void MovementInform(uint32 type, uint32 id) OVERRIDE
            {
                // at the battle he holds this ground between fights instead of walking back to the entrance
                if (type == POINT_MOTION_TYPE && id == 0)
                    me->SetHomePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
            }

            void JustReachedHome() OVERRIDE
            {
                me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE
            {
                if (InstanceScript* instance = me->GetInstanceScript())
                {
                    uint32 state = instance->GetData(DATA_RAJAXX_EVENT);
                    // the attack failed: he goes back to the entrance and can be asked again
                    if (_lastState == IN_PROGRESS && state == NOT_STARTED)
                    {
                        _lastState = state;
                        me->SetHomePosition(_spawnPos);
                        EnterEvadeMode();
                        return;
                    }
                    _lastState = state;
                }

                if (UpdateVictim())
                    DoMeleeAttackIfReady();
            }

        private:
            uint32 _lastState;
            Position _spawnPos;
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
