/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Revelosh
SD%Complete: 100
SDComment:
SDCategory: Uldaman
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum Revelosh
{
    SPELL_LIGHTNING_BOLT = 15801,
    SPELL_CHAIN_LIGHTNING = 16006
};

class boss_revelosh : public CreatureScript
{
public:
    boss_revelosh() : CreatureScript("boss_revelosh") { }

    struct boss_reveloshAI : public ScriptedAI
    {
        boss_reveloshAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 uiLightningBoltTimer;
        uint32 uiChainLightningTimer;

        void Reset() OVERRIDE
        {
            uiLightningBoltTimer = 4000;
            uiChainLightningTimer = 28000;
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
                uiLightningBoltTimer = 4000;
            } else uiLightningBoltTimer -= uiDiff;

            //uiChainLightningTimer
            if (uiChainLightningTimer <= uiDiff)
            {
                DoCastVictim(SPELL_CHAIN_LIGHTNING);
                uiChainLightningTimer = 20000;
            }
            else uiChainLightningTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_reveloshAI(creature);
    }
};

void AddSC_boss_revelosh()
{
    new boss_revelosh();
}
