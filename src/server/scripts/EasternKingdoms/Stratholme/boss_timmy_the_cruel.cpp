/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: boss_timmy_the_cruel
SD%Complete: 100
SDComment:
SDCategory: Stratholme
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum Says
{
    SAY_SPAWN                   = 0
};

enum Spells
{
    SPELL_RAVENOUSCLAW          = 17470,
    SPELL_THRASH                = 12787,
    SPELL_ENRAGE                = 8599
};

class boss_timmy_the_cruel : public CreatureScript
{
public:
    boss_timmy_the_cruel() : CreatureScript("boss_timmy_the_cruel") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_timmy_the_cruelAI(creature);
    }

    struct boss_timmy_the_cruelAI : public ScriptedAI
    {
        boss_timmy_the_cruelAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 RavenousClaw_Timer;
        bool HasYelled;
        bool Enraged;

        void Reset() OVERRIDE
        {
            RavenousClaw_Timer = 14000;
            HasYelled = false;
            Enraged = false;
            DoCast(me, SPELL_THRASH);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            if (!HasYelled)
            {
                Talk(SAY_SPAWN);
                HasYelled = true;
            }
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
        {
            if (!Enraged && me->HealthBelowPctDamaged(25, damage))
            {
                Enraged = true;
                DoCast(me, SPELL_ENRAGE, true);
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            //RavenousClaw
            if (RavenousClaw_Timer <= diff)
            {
                //Cast
                DoCastVictim(SPELL_RAVENOUSCLAW);
                RavenousClaw_Timer = urand(5000, 8000);
            } else RavenousClaw_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_timmy_the_cruel()
{
    new boss_timmy_the_cruel();
}
