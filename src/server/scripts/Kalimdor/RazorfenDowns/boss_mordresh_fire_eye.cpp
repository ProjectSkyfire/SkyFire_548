/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
* Ported from TrinityCore 3.3.5
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum MordreshTexts
{
    SAY_OOC_1 = 0,
    SAY_OOC_2 = 1,
    SAY_OOC_3 = 2,
    SAY_AGGRO = 3
};

enum MordreshSpells
{
    SPELL_FIREBALL = 12466,
    SPELL_FIRE_NOVA = 12470
};

enum MordreshEvents
{
    EVENT_OOC_1 = 1,
    EVENT_OOC_2,
    EVENT_OOC_3,
    EVENT_OOC_4,
    EVENT_FIREBALL,
    EVENT_FIRE_NOVA
};

class boss_mordresh_fire_eye : public CreatureScript
{
public:
    boss_mordresh_fire_eye() : CreatureScript("boss_mordresh_fire_eye") { }

    struct boss_mordresh_fire_eyeAI : public ScriptedAI
    {
        boss_mordresh_fire_eyeAI(Creature* creature) : ScriptedAI(creature) { }

        EventMap events;

        void Reset() OVERRIDE
        {
            events.Reset();
            events.ScheduleEvent(EVENT_OOC_1, 10000);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            events.Reset();
            Talk(SAY_AGGRO);
            events.ScheduleEvent(EVENT_FIREBALL, 0);
            events.ScheduleEvent(EVENT_FIRE_NOVA, urand(8000, 12000));
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            events.Update(diff);

            if (!UpdateVictim())
            {
                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_OOC_1:
                            Talk(SAY_OOC_1);
                            events.ScheduleEvent(EVENT_OOC_2, 8000);
                            break;
                        case EVENT_OOC_2:
                            Talk(SAY_OOC_2);
                            events.ScheduleEvent(EVENT_OOC_3, 3000);
                            break;
                        case EVENT_OOC_3:
                            me->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
                            events.ScheduleEvent(EVENT_OOC_4, 6000);
                            break;
                        case EVENT_OOC_4:
                            Talk(SAY_OOC_3);
                            events.ScheduleEvent(EVENT_OOC_1, 14000);
                            break;
                        default:
                            break;
                    }
                }
                return;
            }

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_FIREBALL:
                        DoCastVictim(SPELL_FIREBALL);
                        events.ScheduleEvent(EVENT_FIREBALL, urand(2400, 3800));
                        break;
                    case EVENT_FIRE_NOVA:
                        DoCast(me, SPELL_FIRE_NOVA);
                        events.ScheduleEvent(EVENT_FIRE_NOVA, urand(11000, 16000));
                        break;
                    default:
                        break;
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_mordresh_fire_eyeAI(creature);
    }
};

void AddSC_boss_mordresh_fire_eye()
{
    new boss_mordresh_fire_eye();
}
