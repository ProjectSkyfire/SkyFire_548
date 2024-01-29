/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Ironaya
SD%Complete: 100
SDComment:
SDCategory: Uldaman
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum Ironaya
{
    SAY_AGGRO                   = 0,
    SPELL_ARCINGSMASH           = 8374,
    SPELL_KNOCKAWAY             = 10101,
    SPELL_WSTOMP                = 11876,
};

class boss_ironaya : public CreatureScript
{
public:
    boss_ironaya() : CreatureScript("boss_ironaya") { }

    struct boss_ironayaAI : public ScriptedAI
    {
        boss_ironayaAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 uiArcingTimer;
        bool bHasCastedWstomp;
        bool bHasCastedKnockaway;

        void Reset() OVERRIDE
        {
            uiArcingTimer = 3000;
            bHasCastedKnockaway = false;
            bHasCastedWstomp = false;
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

            //If we are <50% hp do knockaway ONCE
            if (!bHasCastedKnockaway && HealthBelowPct(50))
            {
                DoCastVictim(SPELL_KNOCKAWAY, true);

                // current aggro target is knocked away pick new target
                Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0);

                if (!target || target == me->GetVictim())
                    target = SelectTarget(SELECT_TARGET_TOPAGGRO, 1);

                if (target)
                    me->TauntApply(target);

                //Shouldn't cast this agian
                bHasCastedKnockaway = true;
            }

            //uiArcingTimer
            if (uiArcingTimer <= uiDiff)
            {
                DoCast(me, SPELL_ARCINGSMASH);
                uiArcingTimer = 13000;
            } else uiArcingTimer -= uiDiff;

            if (!bHasCastedWstomp && HealthBelowPct(25))
            {
                DoCast(me, SPELL_WSTOMP);
                bHasCastedWstomp = true;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_ironayaAI(creature);
    }
};

//This is the actual function called only once durring InitScripts()
//It must define all handled functions that are to be run in this script
void AddSC_boss_ironaya()
{
    new boss_ironaya();
}
