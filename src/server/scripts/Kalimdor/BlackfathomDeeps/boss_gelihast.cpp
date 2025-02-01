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

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
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

        void Reset() OVERRIDE
        {
            netTimer = std::rand() % 4000 + 2000;
            if (instance)
                instance->SetData(TYPE_GELIHAST, NOT_STARTED);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            if (instance)
                instance->SetData(TYPE_GELIHAST, IN_PROGRESS);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            if (instance)
                instance->SetData(TYPE_GELIHAST, DONE);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            if (netTimer < diff)
            {
                DoCastVictim(SPELL_NET);
                netTimer = std::rand() % 7000 + 4000;
            } else netTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_gelihast()
{
    new boss_gelihast();
}
