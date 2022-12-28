/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Gehennas
SD%Complete: 90
SDComment: Adds MC NYI
SDCategory: Molten Core
EndScriptData */

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "molten_core.h"

enum Spells
{
    SPELL_GEHENNAS_CURSE    = 19716,
    SPELL_RAIN_OF_FIRE      = 19717,
    SPELL_SHADOW_BOLT       = 19728,
};

enum Events
{
    EVENT_GEHENNAS_CURSE    = 1,
    EVENT_RAIN_OF_FIRE      = 2,
    EVENT_SHADOW_BOLT       = 3,
};

class boss_gehennas : public CreatureScript
{
    public:
        boss_gehennas() : CreatureScript("boss_gehennas") { }

        struct boss_gehennasAI : public BossAI
        {
            boss_gehennasAI(Creature* creature) : BossAI(creature, BOSS_GEHENNAS)
            {
            }

            void EnterCombat(Unit* victim) override
            {
                BossAI::EnterCombat(victim);
                events.ScheduleEvent(EVENT_GEHENNAS_CURSE, 12000);
                events.ScheduleEvent(EVENT_RAIN_OF_FIRE, 10000);
                events.ScheduleEvent(EVENT_SHADOW_BOLT, 6000);
            }

            void UpdateAI(uint32 diff) override
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
                        case EVENT_GEHENNAS_CURSE:
                            DoCastVictim(SPELL_GEHENNAS_CURSE);
                            events.ScheduleEvent(EVENT_GEHENNAS_CURSE, urand(22000, 30000));
                            break;
                        case EVENT_RAIN_OF_FIRE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, SPELL_RAIN_OF_FIRE);
                            events.ScheduleEvent(EVENT_RAIN_OF_FIRE, urand(4000, 12000));
                            break;
                        case EVENT_SHADOW_BOLT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1))
                                DoCast(target, SPELL_SHADOW_BOLT);
                            events.ScheduleEvent(EVENT_SHADOW_BOLT, 7000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new boss_gehennasAI(creature);
        }
};

void AddSC_boss_gehennas()
{
    new boss_gehennas();
}
