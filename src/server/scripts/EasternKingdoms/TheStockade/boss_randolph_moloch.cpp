/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Randolf_Moloch
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
    SAY_VANISH = 2
};

enum Spells
{
    SPELL_SWEEP = 86729,
    SPELL_VANISH = 55964,
    SPELL_WILDLY_STABBING = 86726
};
enum Events
{
    EVENT_SWEEP = 1,
    EVENT_VANISH = 2,
    EVENT_WILDLY_STABBING = 3
};

class boss_randolph_moloch : public CreatureScript
{
public:
    boss_randolph_moloch() : CreatureScript("boss_randolph_moloch") { }

    struct boss_randolph_molochAI : public BossAI
    {
        boss_randolph_molochAI(Creature* creature) : BossAI(creature, DATA_RANDOLPH_MOLOCH) { }

        void Reset() OVERRIDE
        {
            BossAI::Reset();
            events.ScheduleEvent(EVENT_SWEEP, 5000);
            events.ScheduleEvent(EVENT_VANISH, 10000);
            events.ScheduleEvent(EVENT_WILDLY_STABBING, 6000);
        }

        void EnterCombat(Unit* victim) OVERRIDE
        {
            me->HandleEmoteCommand(EMOTE_ONESHOT_BOW);
            Talk(SAY_AGGRO);
            BossAI::EnterCombat(victim);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            Talk(SAY_DEATH);
            _JustDied();
            instance->SetBossState(DATA_RANDOLPH_MOLOCH, DONE);
        }

        void JustReachedHome() OVERRIDE
        {
            BossAI::JustReachedHome();
            instance->SetBossState(DATA_RANDOLPH_MOLOCH, FAIL);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_SWEEP:
                {
                    DoCastVictim(SPELL_SWEEP);
                    events.ScheduleEvent(EVENT_SWEEP, 5000);
                    break;
                }
                case EVENT_VANISH:
                {
                    //TODO: vanish should be cast at 35% and 65% HP and not reset boss
                    //if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                        //DoCast(target, SPELL_VANISH);
                    Talk(SAY_VANISH);
                    events.ScheduleEvent(EVENT_VANISH, 10000);
                    break;
                }
                case EVENT_WILDLY_STABBING:
                {
                    DoCastVictim(SPELL_WILDLY_STABBING);
                    events.ScheduleEvent(EVENT_WILDLY_STABBING, 6000);
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
        return GetTheStockadeAI<boss_randolph_molochAI>(creature);
    }
};

void AddSC_boss_randolph_moloch()
{
    new boss_randolph_moloch();
};
