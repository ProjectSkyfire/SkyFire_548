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
SDName: Boss_Dalliah_the_Doomsayer
SDComment: The meeting of the two bosses in front of the prison is not scripted.
SDCategory: Tempest Keep, The Arcatraz
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "arcatraz.h"

enum Says
{
    SAY_AGGRO                       = 1,
    SAY_SLAY                        = 2,
    SAY_WHIRLWIND                   = 3,
    SAY_HEAL                        = 4,
    SAY_DEATH                       = 5,
    SAY_SOCCOTHRATES_DEATH          = 7,

    // Wrath-Scryer Soccothrates
    SAY_AGGRO_DALLIAH_FIRST         = 0,
    SAY_DALLIAH_25_PERCENT          = 5
};

enum Spells
{
    SPELL_GIFT_OF_THE_DOOMSAYER     = 36173,
    SPELL_WHIRLWIND                 = 36142,
    SPELL_HEAL                      = 36144,
    SPELL_SHADOW_WAVE               = 39016   // Heroic only
};

enum Events
{
    EVENT_GIFT_OF_THE_DOOMSAYER     = 1,
    EVENT_WHIRLWIND                 = 2,
    EVENT_HEAL                      = 3,
    EVENT_SHADOW_WAVE               = 4,
    EVENT_ME_FIRST                  = 5,
    EVENT_SOCCOTHRATES_DEATH        = 6
};

enum Misc
{
    ACTION_BANTER_DEATH             = 1,
    DEFAULT_WHIRLWIND_DURATION      = 6000
};

class boss_dalliah_the_doomsayer : public CreatureScript
{
    public:
        boss_dalliah_the_doomsayer() : CreatureScript("boss_dalliah_the_doomsayer") { }

        struct boss_dalliah_the_doomsayerAI : public ScriptedAI
        {
            boss_dalliah_the_doomsayerAI(Creature* creature) : ScriptedAI(creature), _soccothratesTaunt(false), _soccothratesDeath(false)
            {
                instance = creature->GetInstanceScript();
            }

            void Reset() OVERRIDE
            {
                events.Reset();
                _soccothratesTaunt = false;
                _soccothratesDeath = false;

                if (instance)
                    instance->SetData(TYPE_DALLIAH, NOT_STARTED);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                Talk(SAY_AGGRO);

                events.ScheduleEvent(EVENT_GIFT_OF_THE_DOOMSAYER, urand(0, 10000));
                events.ScheduleEvent(EVENT_WHIRLWIND, urand(5000, 10000));
                if (IsHeroic())
                    events.ScheduleEvent(EVENT_SHADOW_WAVE, urand(10000, 15000));
                events.ScheduleEvent(EVENT_ME_FIRST, 6000);

                if (instance)
                    instance->SetData(TYPE_DALLIAH, IN_PROGRESS);
            }

            void SetData(uint32 /*type*/, uint32 data) OVERRIDE
            {
                if (data == ACTION_BANTER_DEATH)
                {
                    _soccothratesDeath = true;
                    events.ScheduleEvent(EVENT_SOCCOTHRATES_DEATH, 6000);
                }
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                if (_soccothratesTaunt || !instance || !me->HealthBelowPctDamaged(25, damage))
                    return;

                _soccothratesTaunt = true;
                if (Creature* soccothrates = Unit::GetCreature(*me, instance->GetData64(DATA_SOCCOTHRATES)))
                    soccothrates->AI()->Talk(SAY_DALLIAH_25_PERCENT);
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

                instance->SetData(TYPE_DALLIAH, DONE);

                if (Creature* soccothrates = Unit::GetCreature(*me, instance->GetData64(DATA_SOCCOTHRATES)))
                    if (soccothrates->IsAlive() && !soccothrates->IsInCombat())
                        soccothrates->AI()->SetData(0, ACTION_BANTER_DEATH);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                {
                    if (_soccothratesDeath)
                    {
                        events.Update(diff);

                        if (events.ExecuteEvent() == EVENT_SOCCOTHRATES_DEATH)
                            Talk(SAY_SOCCOTHRATES_DEATH);
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
                        case EVENT_GIFT_OF_THE_DOOMSAYER:
                            DoCastVictim(SPELL_GIFT_OF_THE_DOOMSAYER);
                            events.ScheduleEvent(EVENT_GIFT_OF_THE_DOOMSAYER, urand(10000, 20000));
                            break;
                        case EVENT_WHIRLWIND:
                            DoCast(me, SPELL_WHIRLWIND);
                            if (roll_chance_i(30))
                                Talk(SAY_WHIRLWIND);
                            // Dalliah heals herself when the whirlwind ends
                            events.ScheduleEvent(EVENT_HEAL, GetWhirlwindDuration() + 500);
                            events.ScheduleEvent(EVENT_WHIRLWIND, urand(15000, 25000));
                            break;
                        case EVENT_HEAL:
                            DoCast(me, SPELL_HEAL);
                            if (roll_chance_i(50))
                                Talk(SAY_HEAL);
                            break;
                        case EVENT_SHADOW_WAVE:
                            DoCastVictim(SPELL_SHADOW_WAVE);
                            events.ScheduleEvent(EVENT_SHADOW_WAVE, urand(10000, 15000));
                            break;
                        case EVENT_ME_FIRST:
                            if (instance)
                                if (Creature* soccothrates = Unit::GetCreature(*me, instance->GetData64(DATA_SOCCOTHRATES)))
                                    if (soccothrates->IsAlive() && !soccothrates->IsInCombat())
                                        soccothrates->AI()->Talk(SAY_AGGRO_DALLIAH_FIRST);
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
            uint32 GetWhirlwindDuration() const
            {
                SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(SPELL_WHIRLWIND);
                int32 duration = spellInfo ? spellInfo->GetDuration() : 0;
                return duration > 0 ? uint32(duration) : uint32(DEFAULT_WHIRLWIND_DURATION);
            }

            InstanceScript* instance;
            EventMap events;
            bool _soccothratesTaunt;
            bool _soccothratesDeath;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_dalliah_the_doomsayerAI(creature);
        }
};

void AddSC_boss_dalliah_the_doomsayer()
{
    new boss_dalliah_the_doomsayer();
}
