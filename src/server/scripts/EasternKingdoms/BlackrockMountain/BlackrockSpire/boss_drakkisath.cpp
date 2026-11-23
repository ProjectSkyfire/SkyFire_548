/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "blackrock_spire.h"

enum Spells
{
    SPELL_CLEAVE                    = 845,
    SPELL_CONFLIGURATION            = 16805,
    SPELL_FLAMESTRIKE               = 16419,
    SPELL_PIERCE_ARMOR              = 6016,
    SPELL_RAGE                      = 16789
};

enum Events
{
    EVENT_CLEAVE                   = 1,
    EVENT_CONFLIGURATION           = 2,
    EVENT_FLAMESTRIKE              = 3,
    EVENT_PIERCE_ARMOR             = 4,
    EVENT_RAGE                     = 5
};

class boss_drakkisath : public CreatureScript
{
public:
    boss_drakkisath() : CreatureScript("boss_drakkisath") { }

    struct boss_drakkisathAI : public BossAI
    {
        boss_drakkisathAI(Creature* creature) : BossAI(creature, DATA_GENERAL_DRAKKISATH) { }

        void Reset() OVERRIDE
        {
            _Reset();
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            _EnterCombat();
            events.ScheduleEvent(EVENT_CLEAVE,    8000);
            events.ScheduleEvent(EVENT_CONFLIGURATION, 15000);
            events.ScheduleEvent(EVENT_FLAMESTRIKE, 12000);
            events.ScheduleEvent(EVENT_PIERCE_ARMOR, 10000);
            events.ScheduleEvent(EVENT_RAGE, 30000);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            _JustDied();
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
                    case EVENT_CLEAVE:
                        DoCastVictim(SPELL_CLEAVE);
                        events.ScheduleEvent(EVENT_CLEAVE, 8000);
                        break;
                    case EVENT_CONFLIGURATION:
                        DoCastVictim(SPELL_CONFLIGURATION);
                        events.ScheduleEvent(EVENT_CONFLIGURATION, 18000);
                        break;
                    case EVENT_FLAMESTRIKE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            DoCast(target, SPELL_FLAMESTRIKE);
                        events.ScheduleEvent(EVENT_FLAMESTRIKE, 20000);
                        break;
                    case EVENT_PIERCE_ARMOR:
                        DoCastVictim(SPELL_PIERCE_ARMOR);
                        events.ScheduleEvent(EVENT_PIERCE_ARMOR, 12000);
                        break;
                    case EVENT_RAGE:
                        DoCast(me, SPELL_RAGE);
                        events.ScheduleEvent(EVENT_RAGE, 45000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_drakkisathAI(creature);
    }
};

void AddSC_boss_drakkisath()
{
    new boss_drakkisath();
}
