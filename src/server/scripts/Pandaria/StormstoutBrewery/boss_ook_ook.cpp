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

enum Yells
{
    YELL_AGGRO = 0, // "Me gonna ook you in the dooker!"
    YELL_DEATH = 1, // "Ook! Oooook!!"
    YELL_KILL = 2, // "In the dooker!"
    YELL_SPELL1 = 3, // GOING BANANAS 1 - "Get Ooking party started!" sId 28800
    YELL_SPELL2 = 4, // GOING BANANAS 2 - "Come on and get your Ook on!" sId 28801
    YELL_SPELL3 = 5, // GOING BANANAS 3 - "We gonna Ook all night!" sId 28802
};

class boss_ook_ook : public CreatureScript
{
public:
    boss_ook_ook() : CreatureScript("boss_ook_ook") { }

    struct boss_ook_ookAI : public BossAI
    {
        uint8 SpellTalkId = 0;
        boss_ook_ookAI(Creature* creature) : BossAI(creature, DATA_OOK_OOK) { }

        void Reset() OVERRIDE
        {
            BossAI::Reset();
            me->SetReactState(REACT_DEFENSIVE);
            SpellTalkId = 0;
            events.ScheduleEvent(EVENT_GROUND_POUND, DUNGEON_MODE(15000, 10000));
            events.ScheduleEvent(EVENT_BANANA_AURA, DUNGEON_MODE(12000, 7000));
        }

        void JustDied(Unit* /* killer */) OVERRIDE
        {
            _JustDied();
            Talk(YELL_DEATH);
            instance->SetBossState(DATA_OOK_OOK, DONE);
        }

        void EnterCombat(Unit* victim) OVERRIDE
        {
            BossAI::EnterCombat(victim);
            Talk(YELL_AGGRO);
        }

        void JustReachedHome() OVERRIDE
        {
            BossAI::JustReachedHome();
            instance->SetBossState(DATA_OOK_OOK, FAIL);
        }

        void KilledUnit(Unit* victim) OVERRIDE
        {
            Talk(YELL_KILL);
        }

        void UpdateAI(uint32 diff) OVERRIDE
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
                        DoCastVictim(SPELL_GROUND_POUND);
                        events.ScheduleEvent(EVENT_GROUND_POUND, DUNGEON_MODE(15000, 10000));
                        break;
                    }
                    case EVENT_BANANA_AURA:
                    {
                        DoCast(me, SPELL_GOING_BANANAS);

                        if (SpellTalkId <= 2)
                        {
                            Talk(YELL_SPELL1 + SpellTalkId);
                            SpellTalkId++;
                        }

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

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return GetStormstoutBreweryAI<boss_ook_ookAI>(creature);
    }
};

void AddSC_boss_ook_ook()
{
    new boss_ook_ook();
}
