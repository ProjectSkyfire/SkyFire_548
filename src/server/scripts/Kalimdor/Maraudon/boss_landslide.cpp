/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Landslide
SD%Complete: 100
SDComment:
SDCategory: Maraudon
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum Spells
{
    SPELL_KNOCKAWAY         = 18670,
    SPELL_TRAMPLE           = 5568,
    SPELL_LANDSLIDE         = 21808
};

class boss_landslide : public CreatureScript
{
public:
    boss_landslide() : CreatureScript("boss_landslide") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_landslideAI(creature);
    }

    struct boss_landslideAI : public ScriptedAI
    {
        boss_landslideAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 KnockAwayTimer;
        uint32 TrampleTimer;
        uint32 LandslideTimer;

        void Reset() OVERRIDE
        {
            KnockAwayTimer = 8000;
            TrampleTimer = 2000;
            LandslideTimer = 0;
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            //KnockAwayTimer
            if (KnockAwayTimer <= diff)
            {
                DoCastVictim(SPELL_KNOCKAWAY);
                KnockAwayTimer = 15000;
            }
            else KnockAwayTimer -= diff;

            //TrampleTimer
            if (TrampleTimer <= diff)
            {
                DoCast(me, SPELL_TRAMPLE);
                TrampleTimer = 8000;
            }
            else TrampleTimer -= diff;

            //Landslide
            if (HealthBelowPct(50))
            {
                if (LandslideTimer <= diff)
                {
                    me->InterruptNonMeleeSpells(false);
                    DoCast(me, SPELL_LANDSLIDE);
                    LandslideTimer = 60000;
                }
                else LandslideTimer -= diff;
            }

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_landslide()
{
    new boss_landslide();
}
