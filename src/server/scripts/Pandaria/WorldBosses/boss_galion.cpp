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

enum eBosses
{
    BOSS_GALION,
};

enum eSpells
{
    SPELL_STOMP = 121787,
    SPELL_CANNON_BARRAGE = 121600,
    SPELL_FIRE_SHOT = 121673,
    SPELL_EMPALLING_PULL = 121747,
    SPELL_BERSERK = 47008,
};

enum eEvents
{
    EVENT_STOMP = 1,
    EVENT_CANNON = 2,
    EVENT_FIRE_SHOT = 3,
    EVENT_EMPALLING = 4,
    EVENT_SPAWN = 6,
    EVENT_BERSERK = 7,
};

enum eCreatures
{
    CREATURE_GALION = 62351,
};


class boss_galion : public CreatureScript
{
public:
    boss_galion() : CreatureScript("boss_galion") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_galion_AI(creature);
    }

    struct boss_galion_AI : public BossAI
    {
        boss_galion_AI(Creature* creature) : BossAI(creature, BOSS_GALION)
        {}
        void Reset()
        {
            events.Reset();
            _Reset();
        }

        void KilledUnit(Unit* u)
        {
        }

        void EnterCombat(Unit* unit)
        {
            events.ScheduleEvent(EVENT_STOMP, 50000);
            events.ScheduleEvent(EVENT_CANNON, 25000);
            events.ScheduleEvent(EVENT_SPAWN, 60000);
            events.ScheduleEvent(EVENT_FIRE_SHOT, 10000);
            events.ScheduleEvent(EVENT_BERSERK, 900000);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_STOMP:
                {
                    me->CastSpell(me, SPELL_STOMP, true);
                    events.ScheduleEvent(EVENT_STOMP, 60000);
                    break;
                }
                case EVENT_CANNON:
                {
                    me->CastSpell(me, SPELL_CANNON_BARRAGE, true);
                    events.ScheduleEvent(EVENT_CANNON, 60000);
                    break;
                }
                case EVENT_SPAWN:
                {
                    for (uint8 i = 0; i < 6; ++i)
                        me->SummonCreature(CREATURE_GALION, me->GetPositionX() + rand() % 5, me->GetPositionY() + 3 + rand() % 5, me->GetPositionZ() + 2, 10.0f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);

                    events.ScheduleEvent(EVENT_SPAWN, 60000);
                    break;
                }
                case EVENT_BERSERK:
                {
                    me->CastSpell(me, SPELL_BERSERK, false);
                    break;
                }
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

class npc_galion : public CreatureScript
{
public:
    npc_galion() : CreatureScript("npc_galion") { }

    struct npc_galionAI : public ScriptedAI
    {
        npc_galionAI(Creature* creature) : ScriptedAI(creature)
        {
        }
        EventMap events;
        void Reset()
        {
            events.Reset();
        }
        void EnterCombat(Unit* unit)
        {
            events.ScheduleEvent(EVENT_EMPALLING, 50000);
        }
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_EMPALLING:
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1))
                        me->CastSpell(target, SPELL_EMPALLING_PULL, true);
                    events.ScheduleEvent(EVENT_EMPALLING, 60000);
                    break;
                }
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_galionAI(creature);
    }
};

void AddSC_boss_galion()
{
    new boss_galion();
    new npc_galion();
}
