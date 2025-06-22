/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/*
Name: Boss_Blackheart_the_Inciter
%Complete: 75
Comment: Incite Chaos not functional since core lacks Mind Control support
Category: Auchindoun, Shadow Labyrinth
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "shadow_labyrinth.h"

enum BlackheartTheInciter
{
    SPELL_INCITE_CHAOS      = 33676,
    SPELL_INCITE_CHAOS_B    = 33684,                         //debuff applied to each member of party
    SPELL_CHARGE            = 33709,
    SPELL_WAR_STOMP         = 33707,

    SAY_INTRO               = 0,
    SAY_AGGRO               = 1,
    SAY_SLAY                = 2,
    SAY_HELP                = 3,
    SAY_DEATH               = 4,

    //below, not used
    SAY2_INTRO              = 5,
    SAY2_AGGRO              = 6,
    SAY2_SLAY               = 7,
    SAY2_HELP               = 8,
    SAY2_DEATH              = 9
};

enum Events
{
    EVENT_INCITE_CHAOS          = 1,
    EVENT_CHARGE_ATTACK         = 2,
    EVENT_WAR_STOMP             = 3
};

class boss_blackheart_the_inciter : public CreatureScript
{
    public:
        boss_blackheart_the_inciter() : CreatureScript("boss_blackheart_the_inciter") { }

        struct boss_blackheart_the_inciterAI : public BossAI
        {
            boss_blackheart_the_inciterAI(Creature* creature) : BossAI(creature, DATA_BLACKHEART_THE_INCITER) { }

            void Reset() OVERRIDE
            {
                _Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();
                events.ScheduleEvent(EVENT_INCITE_CHAOS, 20000);
                events.ScheduleEvent(EVENT_CHARGE_ATTACK, 5000);
                events.ScheduleEvent(EVENT_WAR_STOMP, 15000);

                Talk(SAY_AGGRO);
            }

            void KilledUnit(Unit* who) OVERRIDE
            {
                if (who->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(SAY_SLAY);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
                Talk(SAY_DEATH);
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
                        case EVENT_INCITE_CHAOS:
                        {
                            DoCast(me, SPELL_INCITE_CHAOS);

                            std::list<HostileReference*> t_list = me->getThreatManager().getThreatList();
                            for (std::list<HostileReference*>::const_iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
                            {
                                if (Unit* target = ObjectAccessor::GetUnit(*me, (*itr)->getUnitGuid()))
                                    if (target->GetTypeId() == TypeID::TYPEID_PLAYER)
                                        me->CastSpell(target, SPELL_INCITE_CHAOS_B, true);
                            }

                            DoResetThreat();
                            events.ScheduleEvent(EVENT_INCITE_CHAOS, 40000);
                            break;
                        }
                        case EVENT_CHARGE_ATTACK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, SPELL_CHARGE);
                            events.ScheduleEvent(EVENT_CHARGE, std::rand() % 25000 + 15000);
                            break;
                        case EVENT_WAR_STOMP:
                            DoCast(me, SPELL_WAR_STOMP);
                            events.ScheduleEvent(EVENT_WAR_STOMP, std::rand() % 24000 + 18000);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetShadowLabyrinthAI<boss_blackheart_the_inciterAI>(creature);
        }
};

void AddSC_boss_blackheart_the_inciter()
{
    new boss_blackheart_the_inciter();
}
