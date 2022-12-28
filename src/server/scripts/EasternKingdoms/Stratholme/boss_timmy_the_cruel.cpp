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
    SPELL_RAVENOUSCLAW          = 17470
};

class boss_timmy_the_cruel : public CreatureScript
{
public:
    boss_timmy_the_cruel() : CreatureScript("boss_timmy_the_cruel") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_timmy_the_cruelAI(creature);
    }

    struct boss_timmy_the_cruelAI : public ScriptedAI
    {
        boss_timmy_the_cruelAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 RavenousClaw_Timer;
        bool HasYelled;

        void Reset() override
        {
            RavenousClaw_Timer = 10000;
            HasYelled = false;
        }

        void EnterCombat(Unit* /*who*/) override
        {
            if (!HasYelled)
            {
                Talk(SAY_SPAWN);
                HasYelled = true;
            }
        }

        void UpdateAI(uint32 diff) override
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            //RavenousClaw
            if (RavenousClaw_Timer <= diff)
            {
                //Cast
                DoCastVictim(SPELL_RAVENOUSCLAW);
                //15 seconds until we should cast this again
                RavenousClaw_Timer = 15000;
            } else RavenousClaw_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };

};

void AddSC_boss_timmy_the_cruel()
{
    new boss_timmy_the_cruel();
}
