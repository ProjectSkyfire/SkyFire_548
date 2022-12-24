/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Magistrate_Barthilas
SD%Complete: 70
SDComment:
SDCategory: Stratholme
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "stratholme.h"

enum Spells
{
    SPELL_DRAININGBLOW      = 16793,
    SPELL_CROWDPUMMEL       = 10887,
    SPELL_MIGHTYBLOW        = 14099,
    SPELL_FURIOUS_ANGER     = 16791
};

enum Models
{
    MODEL_NORMAL            = 10433,
    MODEL_HUMAN             = 3637
};

class boss_magistrate_barthilas : public CreatureScript
{
public:
    boss_magistrate_barthilas() : CreatureScript("boss_magistrate_barthilas") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_magistrate_barthilasAI(creature);
    }

    struct boss_magistrate_barthilasAI : public ScriptedAI
    {
        boss_magistrate_barthilasAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 DrainingBlow_Timer;
        uint32 CrowdPummel_Timer;
        uint32 MightyBlow_Timer;
        uint32 FuriousAnger_Timer;
        uint32 AngerCount;

        void Reset() OVERRIDE
        {
            DrainingBlow_Timer = 20000;
            CrowdPummel_Timer = 15000;
            MightyBlow_Timer = 10000;
            FuriousAnger_Timer = 5000;
            AngerCount = 0;

            if (me->IsAlive())
                me->SetDisplayId(MODEL_NORMAL);
            else
                me->SetDisplayId(MODEL_HUMAN);
        }

        void MoveInLineOfSight(Unit* who) OVERRIDE

        {
            //nothing to see here yet

            ScriptedAI::MoveInLineOfSight(who);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            me->SetDisplayId(MODEL_HUMAN);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            if (FuriousAnger_Timer <= diff)
            {
                FuriousAnger_Timer = 4000;
                if (AngerCount > 25)
                    return;

                ++AngerCount;
                DoCast(me, SPELL_FURIOUS_ANGER, false);
            } else FuriousAnger_Timer -= diff;

            //DrainingBlow
            if (DrainingBlow_Timer <= diff)
            {
                DoCastVictim(SPELL_DRAININGBLOW);
                DrainingBlow_Timer = 15000;
            } else DrainingBlow_Timer -= diff;

            //CrowdPummel
            if (CrowdPummel_Timer <= diff)
            {
                DoCastVictim(SPELL_CROWDPUMMEL);
                CrowdPummel_Timer = 15000;
            } else CrowdPummel_Timer -= diff;

            //MightyBlow
            if (MightyBlow_Timer <= diff)
            {
                DoCastVictim(SPELL_MIGHTYBLOW);
                MightyBlow_Timer = 20000;
            } else MightyBlow_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };

};

void AddSC_boss_magistrate_barthilas()
{
    new boss_magistrate_barthilas();
}
