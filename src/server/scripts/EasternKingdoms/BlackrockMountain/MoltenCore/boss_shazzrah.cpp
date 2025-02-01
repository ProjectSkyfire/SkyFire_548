/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Shazzrah
SD%Complete: 75
SDComment: Teleport NYI
SDCategory: Molten Core
EndScriptData */

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "molten_core.h"

enum Spells
{
    SPELL_ARCANE_EXPLOSION  = 19712,
    SPELL_SHAZZRAH_CURSE    = 19713,
    SPELL_MAGIC_GROUNDING   = 19714,
    SPELL_COUNTERSPELL      = 19715,
};

enum Events
{
    EVENT_ARCANE_EXPLOSION  = 1,
    EVENT_SHAZZRAH_CURSE    = 2,
    EVENT_MAGIC_GROUNDING   = 3,
    EVENT_COUNTERSPELL      = 4,
    EVENT_BLINK             = 5,
};

class boss_shazzrah : public CreatureScript
{
    public:
        boss_shazzrah() : CreatureScript("boss_shazzrah") { }

        struct boss_shazzrahAI : public BossAI
        {
            boss_shazzrahAI(Creature* creature) : BossAI(creature, BOSS_SHAZZRAH)
            {
            }

            void EnterCombat(Unit* target) OVERRIDE
            {
                BossAI::EnterCombat(target);
                events.ScheduleEvent(EVENT_ARCANE_EXPLOSION, 6000);
                events.ScheduleEvent(EVENT_SHAZZRAH_CURSE, 10000);
                events.ScheduleEvent(EVENT_MAGIC_GROUNDING, 24000);
                events.ScheduleEvent(EVENT_COUNTERSPELL, 15000);
                events.ScheduleEvent(EVENT_BLINK, 30000);
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
                        case EVENT_ARCANE_EXPLOSION:
                            DoCastVictim(SPELL_ARCANE_EXPLOSION);
                            events.ScheduleEvent(EVENT_ARCANE_EXPLOSION, std::rand() % 9000 + 5000);
                            break;
                        case EVENT_SHAZZRAH_CURSE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true, -EVENT_SHAZZRAH_CURSE))
                                DoCast(target, SPELL_SHAZZRAH_CURSE);
                            events.ScheduleEvent(EVENT_SHAZZRAH_CURSE, std::rand() % 30000 + 25000);
                            break;
                        case EVENT_MAGIC_GROUNDING:
                            DoCast(me, SPELL_MAGIC_GROUNDING);
                            events.ScheduleEvent(EVENT_MAGIC_GROUNDING, 35000);
                            break;
                        case EVENT_COUNTERSPELL:
                            DoCastVictim(SPELL_COUNTERSPELL);
                            events.ScheduleEvent(EVENT_COUNTERSPELL, std::rand() % 20000 + 16000);
                            break;
                        case EVENT_BLINK:
                            // Teleporting him to a random player and casting Arcane Explosion after that.
                            // Blink is not working cause of LoS System we need to do this hardcoded.
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true))
                            {
                                DoTeleportTo(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ());
                                DoCast(target, SPELL_ARCANE_EXPLOSION);
                                DoResetThreat();
                            }
                            events.ScheduleEvent(EVENT_BLINK, 45000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_shazzrahAI(creature);
        }
};

void AddSC_boss_shazzrah()
{
    new boss_shazzrah();
}
