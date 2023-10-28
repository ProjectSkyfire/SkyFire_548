/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Lord_Overheat
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
};

enum Spells
{
    SPELL_FIREBALL = 12466,
    SPELL_OVERHEAT = 86633,
    SPELL_RAIN_OF_FIRE = 86636
};
enum Events
{
    EVENT_FIREBALL = 1,
    EVENT_RAIN_OF_FIRE = 2,
    EVENT_OVERHEAT = 3
};

class boss_lord_overheat : public CreatureScript
{
public:
    boss_lord_overheat() : CreatureScript("boss_lord_overheat") { }

    struct boss_lord_overheatAI : public BossAI
    {
        boss_lord_overheatAI(Creature* creature) : BossAI(creature, DATA_LORD_OVERHEAT) { }

        void Reset() OVERRIDE
        {
            BossAI::Reset();
            events.ScheduleEvent(EVENT_FIREBALL, 1000);
            events.ScheduleEvent(EVENT_RAIN_OF_FIRE, 15000);
            events.ScheduleEvent(EVENT_OVERHEAT, 5000);
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
            instance->SetBossState(DATA_LORD_OVERHEAT, DONE);
        }

        void JustReachedHome() OVERRIDE
        {
            BossAI::JustReachedHome();
            instance->SetBossState(DATA_LORD_OVERHEAT, FAIL);
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
                case EVENT_FIREBALL:
                {
                    DoCastVictim(SPELL_FIREBALL);
                    events.ScheduleEvent(EVENT_FIREBALL, 4000);
                    break;
                }
                case EVENT_RAIN_OF_FIRE:
                {
                    DoCastVictim(SPELL_RAIN_OF_FIRE);
                    events.ScheduleEvent(EVENT_RAIN_OF_FIRE, 15000);
                    break;
                }
                case EVENT_OVERHEAT:
                {
                    DoCastVictim(SPELL_OVERHEAT);
                    events.ScheduleEvent(EVENT_OVERHEAT, 10000);
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
        return GetTheStockadeAI<boss_lord_overheatAI>(creature);
    }
};

void AddSC_boss_lord_overheat()
{
    new boss_lord_overheat();
};
