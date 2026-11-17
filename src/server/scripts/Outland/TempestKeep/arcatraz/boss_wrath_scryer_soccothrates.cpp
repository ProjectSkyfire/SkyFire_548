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
SDName: Boss_Wrath_Scryer_Soccothrates
SDComment: The charge and the meeting of the two bosses in front of the prison are not scripted.
SDCategory: Tempest Keep, The Arcatraz
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "arcatraz.h"

enum Says
{
    SAY_AGGRO                       = 1,
    SAY_SLAY                        = 2,
    SAY_KNOCK_AWAY                  = 3,
    SAY_DEATH                       = 4,
    SAY_DALLIAH_DEATH               = 6,

    // Dalliah the Doomsayer
    SAY_AGGRO_SOCCOTHRATES_FIRST    = 0,
    SAY_SOCCOTHRATES_25_PERCENT     = 6
};

enum Spells
{
    SPELL_FELFIRE_SHOCK             = 35759,
    SPELL_FELFIRE_SHOCK_H           = 39006,
    SPELL_KNOCK_AWAY                = 36512
};

enum Events
{
    EVENT_FELFIRE_SHOCK             = 1,
    EVENT_KNOCK_AWAY                = 2,
    EVENT_ME_FIRST                  = 3,
    EVENT_DALLIAH_DEATH             = 4
};

enum Misc
{
    ACTION_BANTER_DEATH             = 1
};

class boss_wrath_scryer_soccothrates : public CreatureScript
{
    public:
        boss_wrath_scryer_soccothrates() : CreatureScript("boss_wrath_scryer_soccothrates") { }

        struct boss_wrath_scryer_soccothratesAI : public ScriptedAI
        {
            boss_wrath_scryer_soccothratesAI(Creature* creature) : ScriptedAI(creature), _dalliahTaunt(false), _dalliahDeath(false)
            {
                instance = creature->GetInstanceScript();
            }

            void Reset() OVERRIDE
            {
                events.Reset();
                _dalliahTaunt = false;
                _dalliahDeath = false;

                if (instance)
                    instance->SetData(TYPE_SOCCOTHRATES, NOT_STARTED);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                Talk(SAY_AGGRO);

                events.ScheduleEvent(EVENT_FELFIRE_SHOCK, urand(8000, 18000));
                events.ScheduleEvent(EVENT_KNOCK_AWAY, urand(25000, 30000));
                events.ScheduleEvent(EVENT_ME_FIRST, 6000);

                if (instance)
                    instance->SetData(TYPE_SOCCOTHRATES, IN_PROGRESS);
            }

            void SetData(uint32 /*type*/, uint32 data) OVERRIDE
            {
                if (data == ACTION_BANTER_DEATH)
                {
                    _dalliahDeath = true;
                    events.ScheduleEvent(EVENT_DALLIAH_DEATH, 6000);
                }
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                if (_dalliahTaunt || !instance || !me->HealthBelowPctDamaged(25, damage))
                    return;

                _dalliahTaunt = true;
                if (Creature* dalliah = Unit::GetCreature(*me, instance->GetData64(DATA_DALLIAH)))
                    dalliah->AI()->Talk(SAY_SOCCOTHRATES_25_PERCENT);
            }

            void KilledUnit(Unit* /*victim*/) OVERRIDE
            {
                Talk(SAY_SLAY);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                Talk(SAY_DEATH);

                if (!instance)
                    return;

                instance->SetData(TYPE_SOCCOTHRATES, DONE);

                if (Creature* dalliah = Unit::GetCreature(*me, instance->GetData64(DATA_DALLIAH)))
                    if (dalliah->IsAlive() && !dalliah->IsInCombat())
                        dalliah->AI()->SetData(0, ACTION_BANTER_DEATH);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                {
                    if (_dalliahDeath)
                    {
                        events.Update(diff);

                        if (events.ExecuteEvent() == EVENT_DALLIAH_DEATH)
                            Talk(SAY_DALLIAH_DEATH);
                    }

                    return;
                }

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_FELFIRE_SHOCK:
                            DoCastVictim(DUNGEON_MODE(SPELL_FELFIRE_SHOCK, SPELL_FELFIRE_SHOCK_H));
                            events.ScheduleEvent(EVENT_FELFIRE_SHOCK, urand(10000, 15000));
                            break;
                        case EVENT_KNOCK_AWAY:
                            DoCast(me, SPELL_KNOCK_AWAY);
                            if (roll_chance_i(50))
                                Talk(SAY_KNOCK_AWAY);
                            events.ScheduleEvent(EVENT_KNOCK_AWAY, urand(25000, 30000));
                            break;
                        case EVENT_ME_FIRST:
                            if (instance)
                                if (Creature* dalliah = Unit::GetCreature(*me, instance->GetData64(DATA_DALLIAH)))
                                    if (dalliah->IsAlive() && !dalliah->IsInCombat())
                                        dalliah->AI()->Talk(SAY_AGGRO_SOCCOTHRATES_FIRST);
                            break;
                        default:
                            break;
                    }

                    if (me->HasUnitState(UNIT_STATE_CASTING))
                        return;
                }

                DoMeleeAttackIfReady();
            }

        private:
            InstanceScript* instance;
            EventMap events;
            bool _dalliahTaunt;
            bool _dalliahDeath;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_wrath_scryer_soccothratesAI(creature);
        }
};

void AddSC_boss_wrath_scryer_soccothrates()
{
    new boss_wrath_scryer_soccothrates();
}
