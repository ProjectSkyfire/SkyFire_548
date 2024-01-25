/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Galgann_Firehammer
SD%Complete: 100
SDComment:
SDCategory: Uldaman
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum GalgannFirehammer
{
    SAY_AGGRO = 0,
    SPELL_FLAME_LASH = 3356,
    SPELL_AMPLIFY_FLAMES = 9482,
    SPELL_FIRE_NOVA = 11969
};

class boss_galgann_firehammer : public CreatureScript
{
public:
    boss_galgann_firehammer() : CreatureScript("boss_galgann_firehammer") { }

    struct boss_galgann_firehammerAI : public ScriptedAI
    {
        boss_galgann_firehammerAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 uiFlameLashTimer;
        uint32 uiAmplifyFlamesTimer;
        uint32 uiFireNovaTimer;

        void Reset() OVERRIDE
        {
            uiFlameLashTimer = 7000;
            uiAmplifyFlamesTimer = 9000;
            uiFireNovaTimer = 10000;
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            Talk(SAY_AGGRO);
        }

        void UpdateAI(uint32 uiDiff) OVERRIDE
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            //uiFlameLashTimer
            if (uiFlameLashTimer <= uiDiff)
            {
                DoCastVictim(SPELL_FLAME_LASH);
                uiFlameLashTimer = 20000;
            } else uiFlameLashTimer -= uiDiff;

            //uiAmplifyFlamesTimer
            if (uiAmplifyFlamesTimer <= uiDiff)
            {
                DoCastVictim(SPELL_AMPLIFY_FLAMES);
                uiAmplifyFlamesTimer = 20000;
            }
            else uiAmplifyFlamesTimer -= uiDiff;

            //uiFireNovaTimer
            if (uiFireNovaTimer <= uiDiff)
            {
                DoCastVictim(SPELL_FIRE_NOVA);
                uiFireNovaTimer = 6000;
            }
            else uiFireNovaTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_galgann_firehammerAI(creature);
    }
};

void AddSC_boss_galgann_firehammer()
{
    new boss_galgann_firehammer();
}
