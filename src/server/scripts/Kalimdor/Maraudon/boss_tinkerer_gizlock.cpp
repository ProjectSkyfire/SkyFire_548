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
    SPELL_FLASH_BOMB        = 29419,
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
        boss_tinkerer_gizlockAI(Creature* creature) : ScriptedAI(creature)
        {
            uiBombTimer = 0;
            uiGoblinDragonGunTimer = 0;
            uiFlashBombTimer = 0;
        }

        void Reset() OVERRIDE
        {
            uiBombTimer = 4000;
            uiGoblinDragonGunTimer = urand(6000, 9000);
            uiFlashBombTimer = 25000;
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
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_BOMB);
                uiBombTimer = 12000;
            }
            else uiBombTimer -= diff;

            //uiGoblinDragonGunTimer
            if (uiGoblinDragonGunTimer <= diff)
            {
                DoCastVictim(SPELL_GOBLIN_DRAGON_GUN);
                uiGoblinDragonGunTimer = urand(16000, 19000);
            }
            else uiGoblinDragonGunTimer -= diff;

            if (uiFlashBombTimer <= diff)
            {
                DoCast(me, SPELL_FLASH_BOMB);
                uiFlashBombTimer = 25000;
            }
            else uiFlashBombTimer -= diff;

            DoMeleeAttackIfReady();
        }

    private:
        uint32 uiBombTimer;
        uint32 uiGoblinDragonGunTimer;
        uint32 uiFlashBombTimer;
    };
};

void AddSC_boss_tinkerer_gizlock()
{
    new boss_tinkerer_gizlock();
}
