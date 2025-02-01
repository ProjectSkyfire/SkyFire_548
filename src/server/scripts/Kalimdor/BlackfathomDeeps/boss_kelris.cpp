/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "blackfathom_deeps.h"

enum Spells
{
    SPELL_MIND_BLAST        = 15587,
    SPELL_SLEEP             = 8399,

    SAY_AGGRO               = 0,
    SAY_SLEEP               = 1,
    SAY_DEATH               = 2
};

class boss_kelris : public CreatureScript
{
public:
    boss_kelris() : CreatureScript("boss_kelris") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_kelrisAI(creature);
    }

    struct boss_kelrisAI : public ScriptedAI
    {
        boss_kelrisAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        uint32 mindBlastTimer;
        uint32 sleepTimer;

        InstanceScript* instance;

        void Reset() OVERRIDE
        {
            mindBlastTimer = std::rand() % 5000 + 2000;
            sleepTimer = std::rand() % 12000 + 9000;
            if (instance)
                instance->SetData(TYPE_KELRIS, NOT_STARTED);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            Talk(SAY_AGGRO);
            if (instance)
                instance->SetData(TYPE_KELRIS, IN_PROGRESS);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            Talk(SAY_DEATH);
            if (instance)
                instance->SetData(TYPE_KELRIS, DONE);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            if (mindBlastTimer < diff)
            {
                DoCastVictim(SPELL_MIND_BLAST);
                mindBlastTimer = std::rand() % 9000 + 7000;
            } else mindBlastTimer -= diff;

            if (sleepTimer < diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                {
                    Talk(SAY_SLEEP);
                    DoCast(target, SPELL_SLEEP);
                }
                sleepTimer = std::rand() % 20000 + 15000;
            } else sleepTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_kelris()
{
    new boss_kelris();
}
