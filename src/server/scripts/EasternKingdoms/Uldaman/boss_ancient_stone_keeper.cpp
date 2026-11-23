/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Ancient_Stone_Keeper
SD%Complete: 100
SDComment:
SDCategory: Uldaman
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum AncientStoneKeeper
{
    NPC_SAND_STORM              = 7226,
    SPELL_SAND_STORM            = 10092,

    // First storm 5 seconds into the fight, second one 15 seconds later, each repeating every 45 seconds
    TIMER_FIRST_STORM           = 5000,
    TIMER_SECOND_STORM          = 20000,
    TIMER_STORM_REPEAT          = 45000
};

class boss_ancient_stone_keeper : public CreatureScript
{
public:
    boss_ancient_stone_keeper() : CreatureScript("boss_ancient_stone_keeper") { }

    struct boss_ancient_stone_keeperAI : public ScriptedAI
    {
        uint32 uiFirstStormTimer;
        uint32 uiSecondStormTimer;

        boss_ancient_stone_keeperAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() OVERRIDE
        {
            uiFirstStormTimer = TIMER_FIRST_STORM;
            uiSecondStormTimer = TIMER_SECOND_STORM;
        }

        // The storm gets its aura and a slow walk towards a nearby player here, the summon spells only create it
        void SummonSandStorm()
        {
            float x = 0.0f, y = 0.0f;
            GetPositionWithDistInOrientation(me, 10.0f, me->GetOrientation() + frand(-M_PI, M_PI), x, y);
            Creature* sandStorm = me->SummonCreature(NPC_SAND_STORM, x, y, me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 10000);
            if (!sandStorm)
                return;

            sandStorm->CastSpell(sandStorm, SPELL_SAND_STORM, true);
            sandStorm->SetSpeed(UnitMoveType::MOVE_RUN, 0.1f, true);
            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 10.0f, true))
                sandStorm->GetMotionMaster()->MoveChase(target, 1.0f, 1.0f);
        }

        void UpdateAI(uint32 uiDiff) OVERRIDE
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            if (uiFirstStormTimer <= uiDiff)
            {
                SummonSandStorm();
                uiFirstStormTimer = TIMER_STORM_REPEAT;
            }
            else
                uiFirstStormTimer -= uiDiff;

            if (uiSecondStormTimer <= uiDiff)
            {
                SummonSandStorm();
                uiSecondStormTimer = TIMER_STORM_REPEAT;
            }
            else
                uiSecondStormTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_ancient_stone_keeperAI(creature);
    }
};

void AddSC_boss_ancient_stone_keeper()
{
    new boss_ancient_stone_keeper();
}
