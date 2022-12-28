/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "blackfathom_deeps.h"

enum Spells
{
    SPELL_NET                                              = 6533
};

class boss_gelihast : public CreatureScript
{
public:
    boss_gelihast() : CreatureScript("boss_gelihast") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_gelihastAI(creature);
    }

    struct boss_gelihastAI : public ScriptedAI
    {
        boss_gelihastAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        uint32 netTimer;

        InstanceScript* instance;

        void Reset() override
        {
            netTimer = urand(2000, 4000);
            if (instance)
                instance->SetData(TYPE_GELIHAST, NOT_STARTED);
        }

        void EnterCombat(Unit* /*who*/) override
        {
            if (instance)
                instance->SetData(TYPE_GELIHAST, IN_PROGRESS);
        }

        void JustDied(Unit* /*killer*/) override
        {
            if (instance)
                instance->SetData(TYPE_GELIHAST, DONE);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            if (netTimer < diff)
            {
                DoCastVictim(SPELL_NET);
                netTimer = urand(4000, 7000);
            } else netTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_gelihast()
{
    new boss_gelihast();
}
