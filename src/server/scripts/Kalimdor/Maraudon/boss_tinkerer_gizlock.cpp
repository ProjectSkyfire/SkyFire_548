/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Tinkerer_Gizlock
SD%Complete: 100
SDComment:
SDCategory: Maraudon
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum Spells
{
    SAY_AGGRO               = 0,
    SPELL_BOMB              = 9143,
    SPELL_GOBLIN_DRAGON_GUN = 21833,
};

class boss_tinkerer_gizlock : public CreatureScript
{
public:
    boss_tinkerer_gizlock() : CreatureScript("boss_tinkerer_gizlock") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_tinkerer_gizlockAI(creature);
    }

    struct boss_tinkerer_gizlockAI : public ScriptedAI
    {
        boss_tinkerer_gizlockAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 uiBombTimer;
        uint32 uiGoblinDragonGunTimer;

        void Reset() OVERRIDE
        {
            uiBombTimer = 2000;
            uiGoblinDragonGunTimer = 13000;
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE 
        {
            Talk(SAY_AGGRO);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            //uiBombTimer
            if (uiBombTimer <= diff)
            {
                DoCastVictim(SPELL_BOMB);
                uiBombTimer = 2000;
            }
            else uiBombTimer -= diff;

            //uiGoblinDragonGunTimer
            if (uiGoblinDragonGunTimer <= diff)
            {
                DoCastVictim(SPELL_GOBLIN_DRAGON_GUN);
                uiGoblinDragonGunTimer = 17000;
            }
            else uiGoblinDragonGunTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_tinkerer_gizlock()
{
    new boss_tinkerer_gizlock();
}
