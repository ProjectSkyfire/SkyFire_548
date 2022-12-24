/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "zulaman.h"

enum Says
{
    SAY_AGGRO               = 0,
    SAY_PLAYER_KILL         = 1,
    SAY_SUMMON_HATCHER      = 2,
    SAY_FIRE_BOMB           = 3,
    SAY_HATCH_ALL_EGGS      = 4,
    EMOTE_FRENZY            = 5,
    SAY_DEATH               = 6
};

enum Spells
{
};

enum Events
{
};

class boss_janalai : public CreatureScript
{
    public:

        boss_janalai() : CreatureScript("boss_janalai") { }

        struct boss_janalaiAI : public BossAI
        {
            boss_janalaiAI(Creature* creature) : BossAI(creature, DATA_JANALAI) { }

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
            return GetZulAmanAI<boss_janalaiAI>(creature);
        }
};

void AddSC_boss_janalai()
{
    new boss_janalai();
}
