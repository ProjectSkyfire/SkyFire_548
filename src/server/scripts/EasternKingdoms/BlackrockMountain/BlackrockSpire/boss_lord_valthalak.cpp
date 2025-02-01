/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "blackrock_spire.h"

enum Spells
{
    SPELL_FRENZY                    = 8269,
    SPELL_SUMMON_SPECTRAL_ASSASSIN  = 27249,
    SPELL_SHADOW_BOLT_VOLLEY        = 27382,
    SPELL_SHADOW_WRATH              = 27286
};

enum Says
{
    EMOTE_FRENZY                    = 0
};

enum Events
{
    EVENT_SUMMON_SPECTRAL_ASSASSIN  = 1,
    EVENT_SHADOW_BOLT_VOLLEY        = 2,
    EVENT_SHADOW_WRATH              = 3
};

class boss_lord_valthalak : public CreatureScript
{
public:
    boss_lord_valthalak() : CreatureScript("boss_lord_valthalak") { }

    struct boss_lord_valthalakAI : public BossAI
    {
        boss_lord_valthalakAI(Creature* creature) : BossAI(creature, DATA_LORD_VALTHALAK) { }

        void Reset() OVERRIDE
        {
            _Reset();
            frenzy40 = false;
            frenzy15 = false;
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            _EnterCombat();
            events.ScheduleEvent(EVENT_SUMMON_SPECTRAL_ASSASSIN, std::rand() % 8000 + 6000);
            events.ScheduleEvent(EVENT_SHADOW_WRATH, std::rand() % 18000 + 9000);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            if (instance)
                instance->SetData(DATA_LORD_VALTHALAK, DONE);
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
                    case EVENT_SUMMON_SPECTRAL_ASSASSIN:
                        DoCast(me, SPELL_SUMMON_SPECTRAL_ASSASSIN);
                        events.ScheduleEvent(EVENT_SUMMON_SPECTRAL_ASSASSIN, std::rand() % 35000 + 30000);
                        break;
                    case EVENT_SHADOW_BOLT_VOLLEY:
                        DoCastVictim(SPELL_SHADOW_BOLT_VOLLEY);
                        events.ScheduleEvent(EVENT_SHADOW_BOLT_VOLLEY, std::rand() % 6000 + 4000);
                        break;
                    case EVENT_SHADOW_WRATH:
                        DoCastVictim(SPELL_SHADOW_WRATH);
                        events.ScheduleEvent(EVENT_SHADOW_WRATH, std::rand() % 24000 + 19000);
                        break;
                    default:
                        break;
                }
            }

            if (!frenzy40)
            {
                if (HealthBelowPct(40))
                {
                    DoCast(me, SPELL_FRENZY);
                    events.CancelEvent(EVENT_SUMMON_SPECTRAL_ASSASSIN);
                    frenzy40 = true;
                }
            }

            if (!frenzy15)
            {
                if (HealthBelowPct(15))
                {
                    DoCast(me, SPELL_FRENZY);
                    events.ScheduleEvent(EVENT_SHADOW_BOLT_VOLLEY, std::rand() % 14000 + 7000);
                    frenzy15 = true;
                }
            }

            DoMeleeAttackIfReady();
        }
        private:
            bool frenzy40;
            bool frenzy15;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_lord_valthalakAI(creature);
    }
};

void AddSC_boss_lord_valthalak()
{
    new boss_lord_valthalak();
}
