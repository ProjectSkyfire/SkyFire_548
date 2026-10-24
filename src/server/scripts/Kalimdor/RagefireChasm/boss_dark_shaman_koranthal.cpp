/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "InstanceScript.h"
#include "ragefire_chasm.h"

class boss_dark_shaman_koranthal : public CreatureScript
{
    public:
        boss_dark_shaman_koranthal() : CreatureScript("boss_dark_shaman_koranthal") { }

        enum iSpells
        {
            SPELL_TWISTED_ELEMENTS = 119300,
            SPELL_SHADOW_STORM     = 119971,
        };

        enum iEvents
        {
            EVENT_TWISTED_ELEMENTS = 1,
            EVENT_SHADOW_STORM     = 2,
        };

        struct boss_dark_shaman_koranthalAI : public BossAI
        {
            boss_dark_shaman_koranthalAI(Creature* creature) : BossAI(creature, BOSS_DARK_SHAMAN_CORANTHAL) { }

            void Reset() OVERRIDE
            {
                _Reset();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }

            void EnterEvadeMode() OVERRIDE
            {
                BossAI::EnterEvadeMode();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                events.ScheduleEvent(EVENT_TWISTED_ELEMENTS, urand(4 * IN_MILLISECONDS, 5 * IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_SHADOW_STORM, 20 * IN_MILLISECONDS);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
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
                        case EVENT_TWISTED_ELEMENTS:
                            if (me->HasAura(SPELL_SHADOW_STORM))
                            {
                                events.RescheduleEvent(EVENT_TWISTED_ELEMENTS, 8 * IN_MILLISECONDS);
                                break;
                            }

                            if (Unit* vict = me->GetVictim())
                                me->CastSpell(vict, SPELL_TWISTED_ELEMENTS, false);

                            events.ScheduleEvent(EVENT_TWISTED_ELEMENTS, urand(4 * IN_MILLISECONDS, 5 * IN_MILLISECONDS));
                            break;
                        case EVENT_SHADOW_STORM:
                            me->CastSpell(me, SPELL_SHADOW_STORM, false);
                            events.ScheduleEvent(EVENT_SHADOW_STORM, 20 * IN_MILLISECONDS);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_dark_shaman_koranthalAI(creature);
        }
};

void AddSC_boss_dark_shaman_koranthal()
{
    new boss_dark_shaman_koranthal();
}
