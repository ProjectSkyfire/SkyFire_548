/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "blackrock_spire.h"

enum Spells
{
    SPELL_REND                      = 13738,
    SPELL_THRASH                    = 3391,
};

enum Says
{
    EMOTE_DEATH                     = 0
};

enum Events
{
    EVENT_REND                      = 1,
    EVENT_THRASH                    = 2,
};

const Position SummonLocation = { -167.9561f, -411.7844f, 76.23057f, 1.53589f };

class boss_halycon : public CreatureScript
{
public:
    boss_halycon() : CreatureScript("boss_halycon") { }

    struct boss_halyconAI : public BossAI
    {
        boss_halyconAI(Creature* creature) : BossAI(creature, DATA_HALYCON) { }

        void Reset() OVERRIDE
        {
            _Reset();
            Summoned = false;
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            _EnterCombat();
            events.ScheduleEvent(EVENT_REND, std::rand() % 20000 + 17000);
            events.ScheduleEvent(EVENT_THRASH, std::rand() % 12000 + 10000);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            me->SummonCreature(NPC_GIZRUL_THE_SLAVENER, SummonLocation, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 300000);
            Talk(EMOTE_DEATH);

            Summoned = true;
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
                    case EVENT_REND:
                        DoCastVictim(SPELL_REND);
                        events.ScheduleEvent(EVENT_REND, std::rand() % 10000 + 8000);
                        break;
                    case EVENT_THRASH:
                        DoCast(me, SPELL_THRASH);
                        break;
                    default:
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
        private:
            bool Summoned;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_halyconAI(creature);
    }
};

void AddSC_boss_halycon()
{
    new boss_halycon();
}
