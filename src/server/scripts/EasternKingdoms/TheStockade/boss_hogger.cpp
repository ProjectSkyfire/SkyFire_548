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

enum Says
{
    SAY_AGGRO = 0,
    SAY_DEATH = 1,
    SAY_ENRAGE = 2
};

enum Spells
{
    SPELL_ENRAGE = 86736,
    SPELL_MADDENING_CALL = 86620,
    SPELL_VICIOUS_SLICE = 86604
};

enum Misc
{
    ENRAGE_HEALTH_PCT = 30
};

enum Events
{
    EVENT_VICIOUS_SLICE = 1,
    EVENT_MADDENING_CALL = 2
};

class boss_hogger : public CreatureScript
{
public:
    boss_hogger() : CreatureScript("boss_hogger") { }

    struct boss_hoggerAI : public BossAI
    {
        boss_hoggerAI(Creature* creature) : BossAI(creature, DATA_HOGGER), _enraged(false) { }

        void Reset() OVERRIDE
        {
            BossAI::Reset();
            _enraged = false;
            events.ScheduleEvent(EVENT_VICIOUS_SLICE, urand(3000, 4000));
            events.ScheduleEvent(EVENT_MADDENING_CALL, urand(6000, 8000));
        }

        void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) OVERRIDE
        {
            if (!_enraged && HealthBelowPct(ENRAGE_HEALTH_PCT))
            {
                _enraged = true;
                Talk(SAY_ENRAGE);
                DoCast(me, SPELL_ENRAGE);
            }
        }

        void EnterCombat(Unit* victim) OVERRIDE
        {
            Talk(SAY_AGGRO);
            BossAI::EnterCombat(victim);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            Talk(SAY_DEATH);
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
                    events.ScheduleEvent(EVENT_VICIOUS_SLICE, urand(8000, 12000));
                    break;
                }
                case EVENT_MADDENING_CALL:
                {
                    DoCastAOE(SPELL_MADDENING_CALL);
                    events.ScheduleEvent(EVENT_MADDENING_CALL, urand(19000, 20000));
                    break;
                }
                default:
                    break;
                }
            }
            BossAI::DoMeleeAttackIfReady();
        };

    private:
        bool _enraged;
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
