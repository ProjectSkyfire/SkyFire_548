/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Hogger
SD%Complete: 90
SDComment:
SDCategory: The Stockades
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "instance_the_stockade.h"

enum Spells
{
    SPELL_ENRAGE = 86736,
    SPELL_MADDENING_CALL = 86620,
    SPELL_VICIOUS_SLICE = 86604
};
enum Events
{
    EVENT_VICIOUS_SLICE = 1,
    EVENT_MADDENING_CALL = 2,
    EVENT_ENRAGE = 3
};

class boss_hogger : public CreatureScript
{
public:
    boss_hogger() : CreatureScript("boss_hogger") { }

    struct boss_hoggerAI : public BossAI
    {
        boss_hoggerAI(Creature* creature) : BossAI(creature, DATA_HOGGER) { }

        void Reset() OVERRIDE
        {
            BossAI::Reset();
            events.ScheduleEvent(EVENT_VICIOUS_SLICE, DUNGEON_MODE(4000, 4000));
            events.ScheduleEvent(EVENT_MADDENING_CALL, DUNGEON_MODE(9000, 9000));
            events.ScheduleEvent(EVENT_ENRAGE, DUNGEON_MODE(300000, 300000));
        }

        void EnterCombat(Unit* victim) OVERRIDE
        {
            //Talk();
            BossAI::EnterCombat(victim);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            //Talk();
            _JustDied();
            instance->SetBossState(DATA_HOGGER, DONE);
        }

        void JustReachedHome() OVERRIDE
        {
            BossAI::JustReachedHome();
            instance->SetBossState(DATA_HOGGER, FAIL);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_VICIOUS_SLICE:
                {
                    DoCastVictim(SPELL_VICIOUS_SLICE);
                    events.ScheduleEvent(EVENT_VICIOUS_SLICE, 8000);
                    break;
                }
                case EVENT_MADDENING_CALL:
                {
                    DoCastVictim(SPELL_MADDENING_CALL);
                    events.ScheduleEvent(EVENT_MADDENING_CALL, 18000);
                    break;
                }
                case EVENT_ENRAGE:
                {
                    DoCast(me, SPELL_ENRAGE);
                    events.ScheduleEvent(EVENT_MADDENING_CALL, 300000);
                    break;
                }
                default:
                    break;
                }
            }
            BossAI::DoMeleeAttackIfReady();
        };
    };
    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return GetTheStockadeAI<boss_hoggerAI>(creature);
    }
};

void AddSC_boss_hogger()
{
    new boss_hogger();
};
