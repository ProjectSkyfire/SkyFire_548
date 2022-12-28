/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Azshir_the_Sleepless
SD%Complete: 80
SDComment:
SDCategory: Scarlet Monastery
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum Spells
{
    SPELL_CALLOFTHEGRAVE            = 17831,
    SPELL_TERRIFY                   = 7399,
    SPELL_SOULSIPHON                = 7290
};

class boss_azshir_the_sleepless : public CreatureScript
{
public:
    boss_azshir_the_sleepless() : CreatureScript("boss_azshir_the_sleepless") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_azshir_the_sleeplessAI(creature);
    }

    struct boss_azshir_the_sleeplessAI : public ScriptedAI
    {
        boss_azshir_the_sleeplessAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 SoulSiphon_Timer;
        uint32 CallOftheGrave_Timer;
        uint32 Terrify_Timer;

        void Reset() OVERRIDE
        {
            SoulSiphon_Timer = 1;
            CallOftheGrave_Timer = 30000;
            Terrify_Timer = 20000;
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE { }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            //If we are <50% hp cast Soul Siphon rank 1
            if (!HealthAbovePct(50) && !me->IsNonMeleeSpellCasted(false))
            {
                //SoulSiphon_Timer
                if (SoulSiphon_Timer <= diff)
                {
                    DoCastVictim(SPELL_SOULSIPHON);
                    return;

                    //SoulSiphon_Timer = 20000;
                }
                else SoulSiphon_Timer -= diff;
            }

            //CallOfTheGrave_Timer
            if (CallOftheGrave_Timer <= diff)
            {
                DoCastVictim(SPELL_CALLOFTHEGRAVE);
                CallOftheGrave_Timer = 30000;
            }
            else CallOftheGrave_Timer -= diff;

            //Terrify_Timer
            if (Terrify_Timer <= diff)
            {
                DoCastVictim(SPELL_TERRIFY);
                Terrify_Timer = 20000;
            }
            else Terrify_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_azshir_the_sleepless()
{
    new boss_azshir_the_sleepless();
}
