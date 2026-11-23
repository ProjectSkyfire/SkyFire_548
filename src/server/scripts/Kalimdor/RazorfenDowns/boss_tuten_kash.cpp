/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
* Ported from TrinityCore 3.3.5
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum TutenKashSpells
{
    SPELL_VIRULENT_POISON    = 12254,
    SPELL_THRASH             = 8876,
    SPELL_WEB_SPRAY          = 12252,
    SPELL_CURSE_OF_TUTENKASH = 12255
};

enum TutenKashEvents
{
    EVENT_WEB_SPRAY = 1,
    EVENT_CURSE_OF_TUTENKASH
};

class boss_tuten_kash : public CreatureScript
{
public:
    boss_tuten_kash() : CreatureScript("boss_tuten_kash") { }

    struct boss_tuten_kashAI : public ScriptedAI
    {
        boss_tuten_kashAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() OVERRIDE
        {
            events.Reset();
            DoCast(me, SPELL_VIRULENT_POISON, true);
            DoCast(me, SPELL_THRASH, true);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            events.ScheduleEvent(EVENT_WEB_SPRAY, urand(10000, 20000));
            events.ScheduleEvent(EVENT_CURSE_OF_TUTENKASH, urand(10000, 15000));
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
                    case EVENT_WEB_SPRAY:
                        DoCast(me, SPELL_WEB_SPRAY);
                        events.ScheduleEvent(EVENT_WEB_SPRAY, urand(15000, 25000));
                        break;
                    case EVENT_CURSE_OF_TUTENKASH:
                        DoCast(me, SPELL_CURSE_OF_TUTENKASH);
                        events.ScheduleEvent(EVENT_CURSE_OF_TUTENKASH, urand(15000, 25000));
                        break;
                    default:
                        break;
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_tuten_kashAI(creature);
    }
};

void AddSC_boss_tuten_kash()
{
    new boss_tuten_kash();
}
