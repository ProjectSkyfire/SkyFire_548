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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ObjectMgr.h"
#include "ScriptMgr.h"

enum eSpells
{
    SPELL_XXXX = 0000,
};

enum eEvents
{
    EVENT_XXXX,
};

enum eCreatures
{
};

enum eSays
{
    SAY_XXXX,
};

enum eActions
{
    ACTION_XXXX,
};

class rare_mob_urdur_the_cauterizer : public CreatureScript
{
public:
    rare_mob_urdur_the_cauterizer() : CreatureScript("rare_mob_urdur_the_cauterizer") { }

    struct rare_mob_urdur_the_cauterizerAI : public BossAI
    {
        rare_mob_urdur_the_cauterizerAI(Creature* creature) : BossAI(creature, 0) { }

        EventMap events;

        void Reset()
        {
            events.Reset();
            _Reset();

            //events.ScheduleEvent(EVENT_XXXX, urand(7500, 15000));
        }

        void KilledUnit(Unit* who)
        {
        }

        void EnterCombat(Unit* /*attacker*/)
        {
        }

        void JustDied(Unit* /*killer*/)
        {
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
            case EVENT_XXXX:
            {
                events.ScheduleEvent(EVENT_XXXX, urand(7500, 15000));
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
        return new rare_mob_urdur_the_cauterizerAI(creature);
    }
};

void AddSC_rare_mob_urdur_the_cauterizer()
{
    new rare_mob_urdur_the_cauterizer();
}
