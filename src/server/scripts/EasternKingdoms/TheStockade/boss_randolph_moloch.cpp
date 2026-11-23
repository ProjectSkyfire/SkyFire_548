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
    SPELL_SHADOWSTEP = 55966,
    SPELL_WILDLY_STABBING = 86726
};
enum Misc
{
    VANISH_FIRST_HEALTH_PCT = 70,
    VANISH_SECOND_HEALTH_PCT = 40,
    SWEEP_RANGE = 5
};

enum Events
{
    EVENT_SWEEP = 1,
    EVENT_WILDLY_STABBING = 2,
    EVENT_VANISH = 3,
    EVENT_SHADOWSTEP = 4
};

class boss_randolph_moloch : public CreatureScript
{
public:
    boss_randolph_moloch() : CreatureScript("boss_randolph_moloch") { }

    struct boss_randolph_molochAI : public BossAI
    {
        boss_randolph_molochAI(Creature* creature) : BossAI(creature, DATA_RANDOLPH_MOLOCH), _vanishCount(0) { }

        void Reset() OVERRIDE
        {
            BossAI::Reset();
            _vanishCount = 0;
            me->SetReactState(REACT_AGGRESSIVE);
            events.ScheduleEvent(EVENT_SWEEP, 1000);
            events.ScheduleEvent(EVENT_WILDLY_STABBING, urand(3000, 5000));
        }

        void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) OVERRIDE
        {
            if ((_vanishCount == 0 && HealthBelowPct(VANISH_FIRST_HEALTH_PCT)) ||
                (_vanishCount == 1 && HealthBelowPct(VANISH_SECOND_HEALTH_PCT)))
            {
                ++_vanishCount;
                events.ScheduleEvent(EVENT_VANISH, 1000);
            }
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
                    if (me->IsInRange(me->GetVictim(), 0.0f, SWEEP_RANGE))
                    {
                        DoCastVictim(SPELL_SWEEP);
                        events.ScheduleEvent(EVENT_SWEEP, urand(8000, 9000));
                    }
                    else
                        events.ScheduleEvent(EVENT_SWEEP, 1000);
                    break;
                }
                case EVENT_WILDLY_STABBING:
                {
                    DoCastVictim(SPELL_WILDLY_STABBING);
                    events.ScheduleEvent(EVENT_WILDLY_STABBING, urand(11000, 14000));
                    break;
                }
                case EVENT_VANISH:
                {
                    Talk(SAY_VANISH);
                    me->RemoveAllAuras();
                    DoCast(me, SPELL_VANISH);
                    me->SetReactState(REACT_PASSIVE);
                    events.ScheduleEvent(EVENT_SHADOWSTEP, 2000);
                    break;
                }
                case EVENT_SHADOWSTEP:
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                        DoCast(target, SPELL_SHADOWSTEP);
                    me->SetReactState(REACT_AGGRESSIVE);
                    break;
                }
                default:
                    break;
                }
            }
            BossAI::DoMeleeAttackIfReady();
        };

    private:
        uint8 _vanishCount;
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
