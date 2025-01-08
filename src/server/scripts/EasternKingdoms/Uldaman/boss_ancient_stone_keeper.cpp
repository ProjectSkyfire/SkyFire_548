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
    SPELL_SAND_STORM = 10094
};

class boss_ancient_stone_keeper : public CreatureScript
{
public:
    boss_ancient_stone_keeper() : CreatureScript("boss_ancient_stone_keeper") { }

    struct boss_ancient_stone_keeperAI : public ScriptedAI
    {
        uint32 uiSandStormTimer;

        boss_ancient_stone_keeperAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() OVERRIDE
        {
            uiSandStormTimer = 10000;
        }

        void UpdateAI(uint32 uiDiff) OVERRIDE
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            //uiSandStormTimer
            if (uiSandStormTimer <= uiDiff)
            {
                float x = 0.0f, y = 0.0f;
                GetPositionWithDistInOrientation(me, 10.0f, me->GetOrientation() + frand(-M_PI, M_PI), x, y);

                if (Creature* sandStorm = me->SummonCreature(7226, x, y, me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 10000))
                {
                    sandStorm->CastSpell(sandStorm, 10092, true);
                    sandStorm->SetSpeed(UnitMoveType::MOVE_RUN, 0.1f, true);

                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 10.0f, true))
                        sandStorm->GetMotionMaster()->MoveChase(target, 1.0f, 1.0f);
                }

                GetPositionWithDistInOrientation(me, 10.0f, me->GetOrientation() - frand(-M_PI, M_PI), x, y);

                if (Creature* sandStorm2 = me->SummonCreature(7226, x, y, me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 10000))
                {
                    sandStorm2->CastSpell(sandStorm2, 10092, true);
                    sandStorm2->SetSpeed(UnitMoveType::MOVE_RUN, 0.1f, true);

                    if (Unit* target2 = SelectTarget(SELECT_TARGET_RANDOM, 0, 10.0f, true))
                        sandStorm2->GetMotionMaster()->MoveChase(target2, 1.0f, 1.0f);
                }

                uiSandStormTimer = 11000;
            }
            else uiSandStormTimer -= uiDiff;

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
