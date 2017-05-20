/*
* Copyright (C) 2011-2017 Project SkyFire <http://www.projectskyfire.org/>
* Copyright (C) 2008-2017 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2016 MaNGOS <http://getmangos.com/>
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

/* ScriptData
SDName: Boss_Ordos
SD%Complete: 55%
SDComment: Placeholder
SDCategory: Boss_Ordos
EndScriptData */

/* ContentData
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum Texts
{
    SAY_AGGRO         = 0, // Ordos yells: You will take my place on the eternal brazier.
    SAY_DEATH         = 1, // Ordos yells: The eternal fire will never be extinguished.
    SAY_SLAY          = 2, // Ordos yells: Up in smoke.
    SAY_ANCIENT_FLAME = 3, // Ordos yells: Your flesh will melt.
    SAY_ETERNAL_AGONY = 4, // Ordos yells: Your pain will be endless.
    SAY_POOL_OF_FIRE  = 5, // Ordos Yells: You will feel but a fraction of my agony.
    SAY_BURNING_SOUL  = 6  // Ordos Yells: Burn!
};

enum Spells
{
    SPELL_ANCIENT_FLAME     = 144695, // 40 SEC AFTER PULL
    SPELL_BURNING_SOUL      = 144689, // 20 SEC AFTER PULL // NEXT: 30 SEC LATER
    SPELL_ETERNAL_AGONY     = 144696, // ENRAGE SPELL AFTER 5 MINUTES
    SPELL_MAGMA_CRUSH       = 144688, // 10 SEC AFTER PULL // NEXT: 15 SEC LATER
    SPELL_POOL_OF_FIRE      = 144692  // 30 SEC AFTER PULL
};

enum Events
{
    EVENT_ANCIENT_FLAME    = 1,
    EVENT_BURNING_SOUL     = 2,
    EVENT_POOL_OF_FIRE     = 3,
    EVENT_MAGMA_CRUSH      = 4,
    EVENT_ETERNAL_AGONY    = 5
};

class boss_ordos : public CreatureScript
{
    public:
        boss_ordos() : CreatureScript("boss_ordos") { }

        struct boss_ordosAI : public ScriptedAI
        {
            boss_ordosAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void Reset() OVERRIDE
            {
                _events.ScheduleEvent(EVENT_MAGMA_CRUSH, urand(10000, 13000)); // 10-13
                
                _events.ScheduleEvent(EVENT_ANCIENT_FLAME, urand(40000, 45000)); // 40-45
                
                _events.ScheduleEvent(EVENT_BURNING_SOUL, urand(20000, 30000)); // 20-30
                
                _events.ScheduleEvent(EVENT_POOL_OF_FIRE, urand(30000, 45000)); // 30-40
            }

            void KilledUnit(Unit* victim) OVERRIDE
            {
                Talk(SAY_SLAY);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                Talk(SAY_DEATH);
            }
    
            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _events.ScheduleEvent(EVENT_ETERNAL_AGONY, 300000); // ENRAGE SPELL AFTER 5 MINUTES
                Talk(SAY_AGGRO);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                _events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ETERNAL_AGONY:
                            Talk(SAY_ETERNAL_AGONY);
                            DoCastVictim(SPELL_ETERNAL_AGONY);
                            _events.ScheduleEvent(EVENT_ETERNAL_AGONY, urand(10000, 25000));
                            break;
                        case EVENT_ANCIENT_FLAME:
                            Talk(SAY_ANCIENT_FLAME);
                            DoCast(me, SPELL_ANCIENT_FLAME);
                            _events.ScheduleEvent(EVENT_ANCIENT_FLAME, urand(40000, 45000));
                            break;
                        case EVENT_POOL_OF_FIRE:
                            Talk(SAY_POOL_OF_FIRE);
                            DoCast(me, SPELL_POOL_OF_FIRE);
                            _events.ScheduleEvent(EVENT_POOL_OF_FIRE, urand(30000, 40000));
                            break;
                        case EVENT_MAGMA_CRUSH:
                            DoCastVictim(SPELL_MAGMA_CRUSH);
                            _events.ScheduleEvent(EVENT_MAGMA_CRUSH, urand(7000, 27000));
                            break;
                        case EVENT_BURNING_SOUL:
                            Talk(SAY_BURNING_SOUL);
                            DoCastVictim(SPELL_BURNING_SOUL);
                            _events.ScheduleEvent(EVENT_BURNING_SOUL, urand(10000, 25000));
                            break;
                        default:
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }

            private:
                EventMap _events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_ordosAI(creature);
        }
};

void AddSC_boss_ordos()
{
    new boss_ordos();
}