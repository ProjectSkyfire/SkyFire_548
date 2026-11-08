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

enum Andorov
{
    PATH_TO_BATTLE      = 1547100,
    PATH_TO_ENTRANCE    = 1547101,
    POINT_PATH_END      = 11
};

Position const BattlePos = { -8939.951f, 1551.127f, 21.566868f, 0.0f };

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
                        creature->GetMotionMaster()->MovePath(PATH_TO_BATTLE, false);
                    }

            player->CLOSE_GOSSIP_MENU();
            return true;
        }

        struct npc_andorovAI : public ScriptedAI
        {
            npc_andorovAI(Creature* creature) : ScriptedAI(creature), _lastState(NOT_STARTED), _returning(false) { }

            void JustRespawned() OVERRIDE
            {
                _returning = false;
                ScriptedAI::JustRespawned();
            }

            void MovementInform(uint32 type, uint32 id) OVERRIDE
            {
                if (type != WAYPOINT_MOTION_TYPE || id != POINT_PATH_END)
                    return;

                InstanceScript* instance = me->GetInstanceScript();
                if (_returning)
                {
                    _returning = false;
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
                else if (instance && instance->GetData(DATA_RAJAXX_EVENT) == NOT_STARTED)
                    instance->SetData(DATA_RAJAXX_EVENT, IN_PROGRESS);
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
                    // the attack failed: he walks back to the entrance and can be asked again
                    if (_lastState == IN_PROGRESS && state == NOT_STARTED && !_returning && me->GetExactDist2d(&BattlePos) < 80.0f)
                    {
                        _lastState = state;
                        _returning = true;
                        _EnterEvadeMode();
                        me->GetMotionMaster()->MovePath(PATH_TO_ENTRANCE, false);
                        return;
                    }
                    _lastState = state;
                }

                if (UpdateVictim())
                    DoMeleeAttackIfReady();
            }

        private:
            uint32 _lastState;
            bool _returning;
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
