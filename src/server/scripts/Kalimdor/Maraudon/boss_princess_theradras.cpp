/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Princess_Theradras
SD%Complete: 100
SDComment:
SDCategory: Maraudon
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedCreature.h"

enum Spells
{
    SPELL_DUSTFIELD             = 21909,
    SPELL_BOULDER               = 21832,
    SPELL_THRASH                = 3391,
    SPELL_REPULSIVEGAZE         = 21869
};

class boss_princess_theradras : public CreatureScript
{
public:
    boss_princess_theradras() : CreatureScript("boss_princess_theradras") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_ptheradrasAI(creature);
    }

    struct boss_ptheradrasAI : public ScriptedAI
    {
        boss_ptheradrasAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 DustfieldTimer;
        uint32 BoulderTimer;
        uint32 ThrashTimer;
        uint32 RepulsiveGazeTimer;

        void Reset() OVERRIDE
        {
            DustfieldTimer = 8000;
            BoulderTimer = 2000;
            ThrashTimer = 5000;
            RepulsiveGazeTimer = 23000;
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE { }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            me->SummonCreature(12238, 28.067f, 61.875f, -123.405f, 4.67f, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 600000);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            //DustfieldTimer
            if (DustfieldTimer <= diff)
            {
                DoCast(me, SPELL_DUSTFIELD);
                DustfieldTimer = 14000;
            }
            else DustfieldTimer -= diff;

            //BoulderTimer
            if (BoulderTimer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_BOULDER);
                BoulderTimer = 10000;
            }
            else BoulderTimer -= diff;

            //RepulsiveGazeTimer
            if (RepulsiveGazeTimer <= diff)
            {
                DoCastVictim(SPELL_REPULSIVEGAZE);
                RepulsiveGazeTimer = 20000;
            }
            else RepulsiveGazeTimer -= diff;

            //ThrashTimer
            if (ThrashTimer <= diff)
            {
                DoCast(me, SPELL_THRASH);
                ThrashTimer = 18000;
            }
            else ThrashTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_ptheradras()
{
    new boss_princess_theradras();
}
