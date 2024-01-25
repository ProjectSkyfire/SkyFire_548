/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Grimlok
SD%Complete: 100
SDComment:
SDCategory: Uldaman
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum Grimlok
{
    SAY_AGGRO = 0,
    SAY_KILL = 1,
    SPELL_LIGHTNING_BOLT = 12167,
    SPELL_CHAIN_BOLT = 8292,
    SPELL_BLOODLUST = 6742,
    SPELL_SHRINK = 11892
};

class boss_grimlok : public CreatureScript
{
public:
    boss_grimlok() : CreatureScript("boss_grimlok") { }

    struct boss_grimlokAI : public ScriptedAI
    {
        boss_grimlokAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 uiLightningBoltTimer;
        uint32 uiChainBoltTimer;
        uint32 uiShrinkTimer;
        uint32 uiBloodlustTimer;

        void Reset() OVERRIDE
        {
            uiLightningBoltTimer = 10000;
            uiChainBoltTimer = 8000;
            uiShrinkTimer = 28000;
            uiBloodlustTimer = 25000;
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            Talk(SAY_AGGRO);
        }

        void KilledUnit(Unit* /*victim*/) OVERRIDE
        {
            Talk(SAY_KILL);
        }

        void UpdateAI(uint32 uiDiff) OVERRIDE
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            //uiLightningBoltTimer
            if (uiLightningBoltTimer <= uiDiff)
            {
                DoCastVictim(SPELL_LIGHTNING_BOLT);
                uiLightningBoltTimer = 2000;
            } else uiLightningBoltTimer -= uiDiff;

            //uiChainBoltTimer
            if (uiChainBoltTimer <= uiDiff)
            {
                DoCastVictim(SPELL_CHAIN_BOLT);
                uiChainBoltTimer = 9000;
            }
            else uiChainBoltTimer -= uiDiff;

            //uiBloodlustTimer
            if (uiBloodlustTimer <= uiDiff)
            {
                DoCast(me, SPELL_BLOODLUST);
                uiBloodlustTimer = 25000;
            }
            else uiBloodlustTimer -= uiDiff;

            //uiShrinkTimer
            if (uiShrinkTimer <= uiDiff)
            {
                DoCastVictim(SPELL_SHRINK);
                uiShrinkTimer = 28000;
            }
            else uiShrinkTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_grimlokAI(creature);
    }
};

void AddSC_boss_grimlok()
{
    new boss_grimlok();
}
