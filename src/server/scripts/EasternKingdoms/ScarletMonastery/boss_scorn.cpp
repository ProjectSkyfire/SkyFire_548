/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Scorn
SD%Complete: 100
SDComment:
SDCategory: Scarlet Monastery
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum Spells
{
    SPELL_LICHSLAP                  = 28873,
    SPELL_FROSTBOLTVOLLEY           = 8398,
    SPELL_MINDFLAY                  = 17313,
    SPELL_FROSTNOVA                 = 15531
};

class boss_scorn : public CreatureScript
{
public:
    boss_scorn() : CreatureScript("boss_scorn") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_scornAI(creature);
    }

    struct boss_scornAI : public ScriptedAI
    {
        boss_scornAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 LichSlap_Timer;
        uint32 FrostboltVolley_Timer;
        uint32 MindFlay_Timer;
        uint32 FrostNova_Timer;

        void Reset() OVERRIDE
        {
            LichSlap_Timer = 45000;
            FrostboltVolley_Timer = 30000;
            MindFlay_Timer = 30000;
            FrostNova_Timer = 30000;
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE { }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            //LichSlap_Timer
            if (LichSlap_Timer <= diff)
            {
                DoCastVictim(SPELL_LICHSLAP);
                LichSlap_Timer = 45000;
            }
            else LichSlap_Timer -= diff;

            //FrostboltVolley_Timer
            if (FrostboltVolley_Timer <= diff)
            {
                DoCastVictim(SPELL_FROSTBOLTVOLLEY);
                FrostboltVolley_Timer = 20000;
            }
            else FrostboltVolley_Timer -= diff;

            //MindFlay_Timer
            if (MindFlay_Timer <= diff)
            {
                DoCastVictim(SPELL_MINDFLAY);
                MindFlay_Timer = 20000;
            }
            else MindFlay_Timer -= diff;

            //FrostNova_Timer
            if (FrostNova_Timer <= diff)
            {
                DoCastVictim(SPELL_FROSTNOVA);
                FrostNova_Timer = 15000;
            }
            else FrostNova_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_scorn()
{
    new boss_scorn();
}
