/*
* Copyright (C) 2011-2018 Project SkyFire <http://www.projectskyfire.org/>
* Copyright (C) 2008-2018 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2018 MaNGOS <https://getmangos.com/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 3 of the License, or (at your
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

// Rare Mob: Chelon 72045 (Turtle)
// Progress: 100%

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ObjectMgr.h"
#include "ScriptMgr.h"

enum eSpells
{
    SPELL_GEYSER = 147572,
    SPELL_SHELL_SPIN = 147571,
    SPELL_SNAPPING_BITE = 147569,
};

enum eEvents
{
    EVENT_GEYSER,
    EVENT_SPIN,
    EVENT_BITE,
};

class rare_mob_chelon : public CreatureScript
{
public:
    rare_mob_chelon() : CreatureScript("rare_mob_chelon") { }

    struct rare_mob_chelonAI : public BossAI
    {
        rare_mob_chelonAI(Creature* creature) : BossAI(creature, 0) { }

        EventMap events;

        void Reset()
        {
            events.Reset();
            _Reset();
        }

        void EnterCombat(Unit* /*attacker*/)
        {
            events.ScheduleEvent(EVENT_GEYSER, urand(7500, 15000));
            events.ScheduleEvent(EVENT_SPIN, urand(7500, 15000));
            events.ScheduleEvent(EVENT_BITE, urand(7500, 15000));
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);
            switch (events.ExecuteEvent())
            {
            case EVENT_GEYSER:
            {
                me->CastSpell(me->GetVictim(), SPELL_GEYSER);
                events.ScheduleEvent(EVENT_GEYSER, urand(7500, 15000));
                break;
            }
            case EVENT_SPIN:
            {
                me->CastSpell(me->GetVictim(), SPELL_SHELL_SPIN);
                events.ScheduleEvent(EVENT_SPIN, urand(7500, 15000));
                break;
            }
            case EVENT_BITE:
            {
                me->CastSpell(me->GetVictim(), SPELL_SNAPPING_BITE);
                events.ScheduleEvent(EVENT_BITE, urand(7500, 15000));
                break;
            }
            default:
                break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new rare_mob_chelonAI(creature);
    }
};

void AddSC_rare_mob_chelon()
{
    new rare_mob_chelon();
}
