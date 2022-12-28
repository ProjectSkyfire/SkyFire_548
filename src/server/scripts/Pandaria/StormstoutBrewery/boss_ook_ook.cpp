/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "stormstout_brewery.h"
#include "Player.h"
#include "SpellScript.h"

enum Spells
{
    SPELL_GROUND_POUND = 106807,
    SPELL_GOING_BANANAS = 106651
};

enum Events
{
    EVENT_GROUND_POUND = 1,
    EVENT_BANANA_AURA  = 2,
};

class boss_ook_ook : public CreatureScript
{
public:
    boss_ook_ook() : CreatureScript("boss_ook_ook") { }

    struct boss_ook_ookAI : public BossAI
    {
        boss_ook_ookAI(Creature* creature) : BossAI(creature, DATA_OOK_OOK) { }

        void Reset() override
        {
            BossAI::Reset();
            me->SetReactState(REACT_DEFENSIVE);
            events.ScheduleEvent(EVENT_GROUND_POUND, DUNGEON_MODE(15000, 10000));
            events.ScheduleEvent(EVENT_BANANA_AURA, DUNGEON_MODE(12000, 7000));
        }

        void JustDied(Unit* /* killer */) override
        {
            _JustDied();
            //Talk(SAY_DEATH);
            instance->SetBossState(DATA_OOK_OOK, DONE);
        }

        void EnterCombat(Unit* victim) override
        {
            BossAI::EnterCombat(victim);
            //Talk(SAY_AGGRO);
        }

        void JustReachedHome() override
        {
            BossAI::JustReachedHome();
            instance->SetBossState(DATA_OOK_OOK, FAIL);
        }

        void KilledUnit(Unit* victim) override
        {
            //Talk(SAY_KILL);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim() || !CheckInRoom())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_GROUND_POUND:
                    {
                        DoCast(me, SPELL_GROUND_POUND);
                        events.ScheduleEvent(EVENT_GROUND_POUND, DUNGEON_MODE(15000, 10000));
                        break;
                    }
                    case EVENT_BANANA_AURA:
                    {
                        DoCast(me, SPELL_GOING_BANANAS);
                        events.ScheduleEvent(EVENT_GROUND_POUND, DUNGEON_MODE(12000, 7000));
                        break;
                    }
                default:
                    break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetStormstoutBreweryAI<boss_ook_ookAI>(creature);
    }
};

void AddSC_boss_ook_ook()
{
    new boss_ook_ook();
}
