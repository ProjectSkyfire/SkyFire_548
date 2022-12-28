/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "zulaman.h"

enum Says
{
    SAY_WAVE_1              = 0,
    SAY_WAVE_2              = 1,
    SAY_WAVE_3              = 2,
    SAY_WAVE_4              = 3,
    SAY_AGGRO               = 4,
    SAY_PLAYER_KILL         = 5,
    SAY_SURGE               = 6,
    EMOTE_SURGE             = 7,
    EMOTE_BEAR              = 8,
    SAY_BEAR                = 9,
    SAY_TROLL               = 10,
    SAY_DEATH               = 11
};

enum Spells
{
};

enum Events
{
};

class boss_nalorakk : public CreatureScript
{
    public:

        boss_nalorakk() : CreatureScript("boss_nalorakk") { }

        struct boss_nalorakkAI : public BossAI
        {
            boss_nalorakkAI(Creature* creature) : BossAI(creature, DATA_NALORAKK) { }

            void Reset() OVERRIDE
            {
                _Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                Talk(SAY_AGGRO);
                _EnterCombat();
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                Talk(SAY_DEATH);
                _JustDied();
            }

            void KilledUnit(Unit* victim) OVERRIDE
            {
                if (victim->GetTypeId() == TYPEID_PLAYER)
                    Talk(SAY_PLAYER_KILL);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;
                /*
                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        default:
                            break;
                    }
                }
                */

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_nalorakkAI(creature);
        }
};

void AddSC_boss_nalorakk()
{
    new boss_nalorakk();
}
