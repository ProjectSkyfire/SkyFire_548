/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
* Ported from TrinityCore 3.3.5
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum GluttonTexts
{
    SAY_AGGRO = 0,
    SAY_SLAY = 1,
    SAY_HEALTH_50 = 2,
    SAY_HEALTH_15 = 3
};

enum GluttonSpells
{
    SPELL_THRASH_15 = 8876,
    SPELL_DISEASE_CLOUD = 12627,
    SPELL_THRASH_35 = 12787,
    SPELL_FRENZY = 12795
};

enum GluttonEvents
{
    EVENT_HEALTH_50 = 1,
    EVENT_HEALTH_15
};

class boss_glutton : public CreatureScript
{
public:
    boss_glutton() : CreatureScript("boss_glutton") { }

    struct boss_gluttonAI : public ScriptedAI
    {
        boss_gluttonAI(Creature* creature) : ScriptedAI(creature) { }

        EventMap events;
        bool health50;
        bool health15;

        void Reset() OVERRIDE
        {
            events.Reset();
            health50 = false;
            health15 = false;
            DoCast(me, SPELL_THRASH_15, true);
            DoCast(me, SPELL_DISEASE_CLOUD, true);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            Talk(SAY_AGGRO);
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
        {
            if (!health50 && me->HealthBelowPctDamaged(50, damage))
            {
                health50 = true;
                events.ScheduleEvent(EVENT_HEALTH_50, 0);
            }

            if (!health15 && me->HealthBelowPctDamaged(15, damage))
            {
                health15 = true;
                events.ScheduleEvent(EVENT_HEALTH_15, 0);
            }
        }

        void KilledUnit(Unit* /*victim*/) OVERRIDE
        {
            Talk(SAY_SLAY);
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
                    case EVENT_HEALTH_50:
                        Talk(SAY_HEALTH_50);
                        me->RemoveAurasDueToSpell(SPELL_THRASH_15);
                        DoCast(me, SPELL_THRASH_35);
                        break;
                    case EVENT_HEALTH_15:
                        Talk(SAY_HEALTH_15);
                        DoCast(me, SPELL_FRENZY);
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
        return new boss_gluttonAI(creature);
    }
};

void AddSC_boss_glutton()
{
    new boss_glutton();
}
