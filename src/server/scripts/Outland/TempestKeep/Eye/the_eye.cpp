/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: The_Eye
SD%Complete: 100
SDComment:
SDCategory: Tempest Keep, The Eye
EndScriptData */

/* ContentData
npc_crystalcore_devastator
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "the_eye.h"

enum Spells
{
    SPELL_COUNTERCHARGE    = 35035,
    SPELL_KNOCKAWAY        = 22893,
};

class npc_crystalcore_devastator : public CreatureScript
{
    public:

        npc_crystalcore_devastator()
            : CreatureScript("npc_crystalcore_devastator")
        {
        }
        struct npc_crystalcore_devastatorAI : public ScriptedAI
        {
            npc_crystalcore_devastatorAI(Creature* creature) : ScriptedAI(creature) { }

            uint32 Knockaway_Timer;
            uint32 Countercharge_Timer;

            void Reset() OVERRIDE
            {
                Countercharge_Timer = 9000;
                Knockaway_Timer = 25000;
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                //Check if we have a current target
                //Knockaway_Timer
                if (Knockaway_Timer <= diff)
                {
                    DoCastVictim(SPELL_KNOCKAWAY, true);

                    // current aggro target is knocked away pick new target
                    Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0);

                    if (!target || target == me->GetVictim())
                        target = SelectTarget(SELECT_TARGET_TOPAGGRO, 1);

                    if (target)
                        me->TauntApply(target);

                    Knockaway_Timer = 23000;
                }
                else
                    Knockaway_Timer -= diff;

                //Countercharge_Timer
                if (Countercharge_Timer <= diff)
                {
                    DoCast(me, SPELL_COUNTERCHARGE);
                    Countercharge_Timer = 45000;
                }
                else
                    Countercharge_Timer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_crystalcore_devastatorAI(creature);
        }
};
void AddSC_the_eye()
{
    new npc_crystalcore_devastator();
}

