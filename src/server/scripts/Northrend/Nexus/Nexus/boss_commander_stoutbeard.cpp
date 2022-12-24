/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* Script Data Start
SDName: Boss Commander Stoutbeard
SDAuthor: LordVanMartin
SD%Complete:
SDComment:  Only Horde Heroic
SDCategory:
Script Data End */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum CommanderStoutbeard
{
    SPELL_BATTLE_SHOUT              = 31403,
    SPELL_CHARGE                    = 60067,
    SPELL_FRIGHTENING_SHOUT         = 19134,
    SPELL_WHIRLWIND_1               = 38619,
    SPELL_WHIRLWIND_2               = 38618
};

class boss_commander_stoutbeard : public CreatureScript
{
public:
    boss_commander_stoutbeard() : CreatureScript("boss_commander_stoutbeard") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_commander_stoutbeardAI(creature);
    }

    struct boss_commander_stoutbeardAI : public ScriptedAI
    {
        boss_commander_stoutbeardAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() OVERRIDE { }
        void AttackStart(Unit* /*who*/) OVERRIDE { }
        void MoveInLineOfSight(Unit* /*who*/) OVERRIDE { }

        void UpdateAI(uint32 /*diff*/) OVERRIDE
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

};

void AddSC_boss_commander_stoutbeard()
{
    new boss_commander_stoutbeard();
}
