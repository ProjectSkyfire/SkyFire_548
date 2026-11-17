/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ScriptData
SDName: Boss_Rokmar_the_Crackler
SDCategory: Coilfang Reservoir, The Slave Pens
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "the_slave_pens.h"

enum Spells
{
    SPELL_GRIEVOUS_WOUND     = 31956,
    SPELL_GRIEVOUS_WOUND_H   = 38801,
    SPELL_ENSNARING_MOSS     = 31948,
    SPELL_WATER_SPIT         = 35008,
    SPELL_FRENZY             = 34970
};

enum Events
{
    EVENT_GRIEVOUS_WOUND     = 1,
    EVENT_ENSNARING_MOSS     = 2,
    EVENT_WATER_SPIT         = 3
};

class boss_rokmar_the_crackler : public CreatureScript
{
    public:
        boss_rokmar_the_crackler() : CreatureScript("boss_rokmar_the_crackler") { }

        struct boss_rokmar_the_cracklerAI : public BossAI
        {
            boss_rokmar_the_cracklerAI(Creature* creature) : BossAI(creature, DATA_ROKMAR_THE_CRACKLER) { }

            void Reset() OVERRIDE
            {
                _frenzied = false;
                _Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();
                events.ScheduleEvent(EVENT_GRIEVOUS_WOUND, urand(10000, 25000));
                events.ScheduleEvent(EVENT_ENSNARING_MOSS, urand(10000, 25000));
                events.ScheduleEvent(EVENT_WATER_SPIT, urand(5000, 20000));
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
            }

            void ExecuteEvent(uint32 eventId) OVERRIDE
            {
                switch (eventId)
                {
                    case EVENT_GRIEVOUS_WOUND:
                        DoCastVictim(DUNGEON_MODE(SPELL_GRIEVOUS_WOUND, SPELL_GRIEVOUS_WOUND_H));
                        events.ScheduleEvent(EVENT_GRIEVOUS_WOUND, urand(20000, 30000));
                        break;
                    case EVENT_ENSNARING_MOSS:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 30.0f))
                            DoCast(target, SPELL_ENSNARING_MOSS);
                        events.ScheduleEvent(EVENT_ENSNARING_MOSS, urand(20000, 30000));
                        break;
                    case EVENT_WATER_SPIT:
                        DoCastAOE(SPELL_WATER_SPIT);
                        events.ScheduleEvent(EVENT_WATER_SPIT, urand(10000, 25000));
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!_frenzied && UpdateVictim() && HealthBelowPct(20) && !me->HasUnitState(UNIT_STATE_CASTING))
                {
                    _frenzied = true;
                    DoCast(me, SPELL_FRENZY);
                }

                BossAI::UpdateAI(diff);
            }

        private:
            bool _frenzied;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_rokmar_the_cracklerAI(creature);
        }
};

void AddSC_boss_rokmar_the_crackler()
{
    new boss_rokmar_the_crackler();
}
