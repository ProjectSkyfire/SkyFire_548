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

// Rare Mob: Bufo 72775 (Frog)
// Progress: 100%

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ObjectMgr.h"
#include "ScriptMgr.h"

enum eSpells
{
    SPELL_TOXIC_SKIN = 147654,
    SPELL_GULP_FROG_TOXIN = 147655,
};

enum eEvents
{
    EVENT_TOXIC_SKIN = 1,
    EVENT_GULP_FROG_TOXIN = 2,
};

class rare_mob_bufo : public CreatureScript
{
public:
    rare_mob_bufo() : CreatureScript("rare_mob_bufo") { }

    struct rare_mob_bufoAI : public BossAI
    {
        rare_mob_bufoAI(Creature* creature) : BossAI(creature, 0) { }

        EventMap events;

        void Reset()
        {
            events.Reset();
            _Reset();
        }

        void EnterCombat(Unit* attacker)
        {
            events.ScheduleEvent(EVENT_TOXIC_SKIN, urand(10000, 15000));
            events.ScheduleEvent(EVENT_GULP_FROG_TOXIN, urand(10000, 15000));
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
            case EVENT_TOXIC_SKIN:
            {
                me->CastSpell(me->GetVictim(), SPELL_TOXIC_SKIN);
                events.ScheduleEvent(EVENT_TOXIC_SKIN, urand(7500, 15000));
                break;
            }
            case EVENT_GULP_FROG_TOXIN:
            {
                me->CastSpell(me->GetVictim(), SPELL_GULP_FROG_TOXIN);
                events.ScheduleEvent(EVENT_GULP_FROG_TOXIN, urand(7500, 15000));
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
        return new rare_mob_bufoAI(creature);
    }
};

void AddSC_rare_mob_bufo()
{
    new rare_mob_bufo();
}
