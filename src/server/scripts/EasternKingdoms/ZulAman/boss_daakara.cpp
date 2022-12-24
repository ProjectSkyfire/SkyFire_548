/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "zulaman.h"

enum Says
{
    SAY_INTRO                       = 0,
    SAY_AGGRO                       = 1,
    SAY_PLAYER_KILL                 = 2,
    SAY_FIRE_BREATH                 = 3,
    SAY_TRANSFORMS_BEAR             = 4,
    SAY_TRANSFORMS_EAGLE            = 5,
    SAY_TRANSFORMS_LYNX             = 6,
    SAY_TRANSFORMS_DRAGONHAWK       = 7,
    SAY_ABSORBS_BEAR_SPIRIT         = 8,
    SAY_ABSORBS_EAGLE_SPIRIT        = 9,
    SAY_ABSORBS_LYNX_SPIRIT         = 10,
    SAY_ABSORBS_DRAGONHAWK_SPIRIT   = 11,
    SAY_DEATH                       = 12
};

enum Spells
{
};

enum Events
{
};

class boss_daakara : public CreatureScript
{
    public:

        boss_daakara() : CreatureScript("boss_daakara") { }

        struct boss_daakaraAI : public BossAI
        {
            boss_daakaraAI(Creature* creature) : BossAI(creature, DATA_DAAKARA) { }

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
            return GetZulAmanAI<boss_daakaraAI>(creature);
        }
};

void AddSC_boss_daakara()
{
    new boss_daakara();
}
