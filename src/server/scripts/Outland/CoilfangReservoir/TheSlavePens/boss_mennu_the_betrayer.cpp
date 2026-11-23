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
SDName: Boss_Mennu_the_Betrayer
SDCategory: Coilfang Reservoir, The Slave Pens
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "the_slave_pens.h"

enum Says
{
    SAY_AGGRO                     = 0,
    SAY_SLAY                      = 1,
    SAY_DEATH                     = 2
};

enum Spells
{
    SPELL_TAINTED_STONESKIN_TOTEM = 31985,
    SPELL_TAINTED_EARTHGRAB_TOTEM = 31981,
    SPELL_CORRUPTED_NOVA_TOTEM    = 31991,
    SPELL_MENNUS_HEALING_WARD     = 34980,
    SPELL_LIGHTNING_BOLT          = 35010
};

enum Events
{
    EVENT_TAINTED_STONESKIN_TOTEM = 1,
    EVENT_TAINTED_EARTHGRAB_TOTEM = 2,
    EVENT_CORRUPTED_NOVA_TOTEM    = 3,
    EVENT_MENNUS_HEALING_WARD     = 4,
    EVENT_LIGHTNING_BOLT          = 5
};

class boss_mennu_the_betrayer : public CreatureScript
{
    public:
        boss_mennu_the_betrayer() : CreatureScript("boss_mennu_the_betrayer") { }

        struct boss_mennu_the_betrayerAI : public BossAI
        {
            boss_mennu_the_betrayerAI(Creature* creature) : BossAI(creature, DATA_MENNU_THE_BETRAYER) { }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();
                Talk(SAY_AGGRO);
                events.ScheduleEvent(EVENT_TAINTED_STONESKIN_TOTEM, urand(15000, 20000));
                events.ScheduleEvent(EVENT_TAINTED_EARTHGRAB_TOTEM, urand(25000, 40000));
                events.ScheduleEvent(EVENT_CORRUPTED_NOVA_TOTEM, urand(25000, 35000));
                events.ScheduleEvent(EVENT_MENNUS_HEALING_WARD, urand(15000, 25000));
                events.ScheduleEvent(EVENT_LIGHTNING_BOLT, urand(10000, 20000));
            }

            void KilledUnit(Unit* /*victim*/) OVERRIDE
            {
                Talk(SAY_SLAY);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
                Talk(SAY_DEATH);
            }

            void ExecuteEvent(uint32 eventId) OVERRIDE
            {
                switch (eventId)
                {
                    case EVENT_TAINTED_STONESKIN_TOTEM:
                        DoCast(me, SPELL_TAINTED_STONESKIN_TOTEM);
                        events.ScheduleEvent(EVENT_TAINTED_STONESKIN_TOTEM, urand(30000, 35000));
                        break;
                    case EVENT_TAINTED_EARTHGRAB_TOTEM:
                        DoCast(me, SPELL_TAINTED_EARTHGRAB_TOTEM);
                        events.ScheduleEvent(EVENT_TAINTED_EARTHGRAB_TOTEM, urand(30000, 40000));
                        break;
                    case EVENT_CORRUPTED_NOVA_TOTEM:
                        DoCast(me, SPELL_CORRUPTED_NOVA_TOTEM);
                        events.ScheduleEvent(EVENT_CORRUPTED_NOVA_TOTEM, urand(35000, 40000));
                        break;
                    case EVENT_MENNUS_HEALING_WARD:
                        DoCast(me, SPELL_MENNUS_HEALING_WARD);
                        events.ScheduleEvent(EVENT_MENNUS_HEALING_WARD, urand(15000, 25000));
                        break;
                    case EVENT_LIGHTNING_BOLT:
                        DoCastVictim(SPELL_LIGHTNING_BOLT);
                        events.ScheduleEvent(EVENT_LIGHTNING_BOLT, urand(15000, 25000));
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_mennu_the_betrayerAI(creature);
        }
};

void AddSC_boss_mennu_the_betrayer()
{
    new boss_mennu_the_betrayer();
}
