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

// This is the NPC at the top of the isle, near Ordos.
// The one under the sanctuary is different, "Elite"

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ObjectMgr.h"
#include "ScriptMgr.h"

enum eSpells
{
    SPELL_BLAZING_BLOW = 148003,
    SPELL_CAUTERIZE = 147997,
    SPELL_CONJURE_ETERNAL_KILN = 148004,
    SPELL_CONJURE_FLARECORE_GOLEM = 148001,
    SPELL_FIRE_STORM = 147998,
    SPELL_TASTE_OF_BLOOD = 213738,
};

enum eEvents
{
    EVENT_BLAZING_BLOW = 1,
    EVENT_CAUTERIZE,
    EVENT_CONJURE_ETERNAL_KILN,
    EVENT_CONJURE_FLARECORE_GOLEM,
    EVENT_FIRE_STORM,
    EVENT_TASTE_OF_BLOOD,
};

enum eCreatures
{
    NPC_ETERNAL_KILN = 73528,
    NPC_FLARECORE_GOLEM = 73527,
};

class rare_mob_archereus_of_flame : public CreatureScript
{
public:
    rare_mob_archereus_of_flame() : CreatureScript("rare_mob_archereus_of_flame") { }

    struct rare_mob_archereus_of_flameAI : public BossAI
    {
        rare_mob_archereus_of_flameAI(Creature* creature) : BossAI(creature, 0) { }

        EventMap events;

        void Reset()
        {
            events.Reset();
            _Reset();
        }

        void KilledUnit(Unit* who)
        {

        }

        void EnterCombat(Unit* /*attacker*/)
        {
            events.ScheduleEvent(EVENT_BLAZING_BLOW, urand(1000, 2000));
            events.ScheduleEvent(EVENT_CAUTERIZE, urand(1000, 2000));
            events.ScheduleEvent(EVENT_CONJURE_ETERNAL_KILN, urand(1000, 2000));
            events.ScheduleEvent(EVENT_CONJURE_FLARECORE_GOLEM, urand(1000, 2000));
            events.ScheduleEvent(EVENT_FIRE_STORM, urand(1000, 2000));
            events.ScheduleEvent(EVENT_TASTE_OF_BLOOD, urand(1000, 2000));
        }

        void DespawnAllAdds(Unit* attacker)
        {

        }

        void JustDied(Unit* killer)
        {
            DespawnAllAdds(killer);
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
            case EVENT_BLAZING_BLOW:
            {
                events.ScheduleEvent(EVENT_BLAZING_BLOW, urand(7500, 15000)); // todo timer
                break;
            }
            case EVENT_CAUTERIZE:
            {
                events.ScheduleEvent(EVENT_CAUTERIZE, urand(7500, 15000)); // todo timer
                break;
            }
            case EVENT_CONJURE_ETERNAL_KILN:
            {
                events.ScheduleEvent(EVENT_CONJURE_ETERNAL_KILN, urand(7500, 15000)); // todo timer
                break;
            }
            case EVENT_CONJURE_FLARECORE_GOLEM:
            {
                events.ScheduleEvent(EVENT_CONJURE_FLARECORE_GOLEM, urand(7500, 15000)); // todo timer
                break;
            }
            case EVENT_FIRE_STORM:
            {
                events.ScheduleEvent(EVENT_FIRE_STORM, urand(7500, 15000)); // todo timer
                break;
            }
            case EVENT_TASTE_OF_BLOOD:
            {
                events.ScheduleEvent(EVENT_TASTE_OF_BLOOD, urand(7500, 15000)); // todo timer
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
        return new rare_mob_archereus_of_flameAI(creature);
    }
};

void AddSC_rare_mob_archereus_of_flame()
{
    new rare_mob_archereus_of_flame();
}
