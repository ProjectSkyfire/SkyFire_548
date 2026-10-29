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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum Spells
{
    SPELL_BATTLE_SHOUT           = 31403,
    SPELL_CHARGE                 = 60067,
    SPELL_FRIGHTENING_SHOUT      = 19134,
    SPELL_WHIRLWIND              = 38618,
    SPELL_FROZEN_PRISON          = 47543
};

enum Yells
{
    SAY_AGGRO                    = 0,
    SAY_KILL                     = 1,
    SAY_DEATH                    = 2
};

enum Events
{
    EVENT_CHARGE_COMMANDER       = 1,
    EVENT_WHIRLWIND,
    EVENT_FRIGHTENING_SHOUT
};

struct boss_nexus_commanders : public ScriptedAI
{
    boss_nexus_commanders(Creature* creature) : ScriptedAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        Talk(SAY_AGGRO);
        me->RemoveAurasDueToSpell(SPELL_FROZEN_PRISON);
        DoCast(me, SPELL_BATTLE_SHOUT);

        events.ScheduleEvent(EVENT_CHARGE_COMMANDER, urand(3000, 4000));
        events.ScheduleEvent(EVENT_WHIRLWIND, urand(6000, 8000));
        events.ScheduleEvent(EVENT_FRIGHTENING_SHOUT, urand(13000, 15000));
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
                case EVENT_CHARGE_COMMANDER:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                        DoCast(target, SPELL_CHARGE);
                    events.ScheduleEvent(EVENT_CHARGE_COMMANDER, urand(11000, 15000));
                    break;
                case EVENT_WHIRLWIND:
                    DoCast(me, SPELL_WHIRLWIND);
                    events.ScheduleEvent(EVENT_WHIRLWIND, urand(19500, 25000));
                    break;
                case EVENT_FRIGHTENING_SHOUT:
                    DoCastAOE(SPELL_FRIGHTENING_SHOUT);
                    events.ScheduleEvent(EVENT_FRIGHTENING_SHOUT, urand(45000, 55000));
                    break;
                default:
                    break;
            }

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
        }

        DoMeleeAttackIfReady();
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        Talk(SAY_DEATH);
    }

    void KilledUnit(Unit* who) OVERRIDE
    {
        if (who->GetTypeId() == TypeID::TYPEID_PLAYER)
            Talk(SAY_KILL);
    }

private:
    EventMap events;
};

void AddSC_boss_nexus_commanders()
{
    RegisterCreatureAI(boss_nexus_commanders);
}
