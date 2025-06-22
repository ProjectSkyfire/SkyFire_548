/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum Spells
{
    SPELL_FIERYBURST                                       = 13900,
    SPELL_WARSTOMP                                         = 24375
};

enum Misc
{
    DATA_THRONE_DOOR                                       = 24 // not id or guid of doors but number of enum in blackrock_depths.h
};

class boss_magmus : public CreatureScript
{
public:
    boss_magmus() : CreatureScript("boss_magmus") { }

    struct boss_magmusAI : public ScriptedAI
    {
        boss_magmusAI(Creature* creature) : ScriptedAI(creature)
        {
            FieryBurst_Timer = 0;
            WarStomp_Timer = 0;
        }

        void Reset() OVERRIDE
        {
            FieryBurst_Timer = 5000;
            WarStomp_Timer =0;
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE { }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            //FieryBurst_Timer
            if (FieryBurst_Timer <= diff)
            {
                DoCastVictim(SPELL_FIERYBURST);
                FieryBurst_Timer = 6000;
            } else FieryBurst_Timer -= diff;

            //WarStomp_Timer
            if (HealthBelowPct(51))
            {
                if (WarStomp_Timer <= diff)
                {
                    DoCastVictim(SPELL_WARSTOMP);
                    WarStomp_Timer = 8000;
                } else WarStomp_Timer -= diff;
            }

            DoMeleeAttackIfReady();
        }
        // When he die open door to last chamber
        void JustDied(Unit* killer) OVERRIDE
        {
            if (InstanceScript* instance = killer->GetInstanceScript())
                instance->HandleGameObject(instance->GetData64(DATA_THRONE_DOOR), true);
        }

    private:
        uint32 FieryBurst_Timer;
        uint32 WarStomp_Timer;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_magmusAI(creature);
    }
};

void AddSC_boss_magmus()
{
    new boss_magmus();
}
