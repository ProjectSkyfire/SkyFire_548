/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* Script Data Start
SDName: Boss epoch
SDAuthor: Tartalo
SD%Complete: 80
SDComment: @todo Intro, consecutive attacks to a random target durin time wrap, adjust timers
SDCategory:
Script Data End */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "culling_of_stratholme.h"

enum Spells
{
    SPELL_CURSE_OF_EXERTION                     = 52772,
    SPELL_TIME_WARP                             = 52766, //Time slows down, reducing attack, casting and movement speed by 70% for 6 sec.
    SPELL_TIME_STOP                             = 58848, //Stops time in a 50 yard sphere for 2 sec.
    SPELL_WOUNDING_STRIKE                       = 52771, //Used only on the tank
    H_SPELL_WOUNDING_STRIKE                     = 58830
};

enum Yells
{
    SAY_INTRO                                   = 0,
    SAY_AGGRO                                   = 1,
    SAY_TIME_WARP                               = 2,
    SAY_SLAY                                    = 3,
    SAY_DEATH                                   = 4
};

class boss_epoch : public CreatureScript
{
public:
    boss_epoch() : CreatureScript("boss_epoch") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_epochAI(creature);
    }

    struct boss_epochAI : public ScriptedAI
    {
        boss_epochAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        uint8 uiStep;

        uint32 uiStepTimer;
        uint32 uiWoundingStrikeTimer;
        uint32 uiTimeWarpTimer;
        uint32 uiTimeStopTimer;
        uint32 uiCurseOfExertionTimer;

        InstanceScript* instance;

        void Reset() override
        {
            uiStep = 1;
            uiStepTimer = 26000;
            uiCurseOfExertionTimer = 9300;
            uiTimeWarpTimer = 25300;
            uiTimeStopTimer = 21300;
            uiWoundingStrikeTimer = 5300;

            if (instance)
                instance->SetData(DATA_EPOCH_EVENT, NOT_STARTED);
        }

        void EnterCombat(Unit* /*who*/) override
        {
            Talk(SAY_AGGRO);

            if (instance)
                instance->SetData(DATA_EPOCH_EVENT, IN_PROGRESS);
        }

        void UpdateAI(uint32 diff) override
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            if (uiCurseOfExertionTimer < diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    DoCast(target, SPELL_CURSE_OF_EXERTION);
                uiCurseOfExertionTimer = 9300;
            } else uiCurseOfExertionTimer -= diff;

            if (uiWoundingStrikeTimer < diff)
            {
                DoCastVictim(SPELL_WOUNDING_STRIKE);
                uiWoundingStrikeTimer = 5300;
            } else uiWoundingStrikeTimer -= diff;

            if (uiTimeStopTimer < diff)
            {
                DoCastAOE(SPELL_TIME_STOP);
                uiTimeStopTimer = 21300;
            } else uiTimeStopTimer -= diff;

            if (uiTimeWarpTimer < diff)
            {
                Talk(SAY_TIME_WARP);
                DoCastAOE(SPELL_TIME_WARP);
                uiTimeWarpTimer = 25300;
            } else uiTimeWarpTimer -= diff;

            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* /*killer*/) override
        {
            Talk(SAY_DEATH);

            if (instance)
                instance->SetData(DATA_EPOCH_EVENT, DONE);
        }

        void KilledUnit(Unit* victim) override
        {
            if (victim->GetTypeId() != TypeID::TYPEID_PLAYER)
                return;

            Talk(SAY_SLAY);
        }
    };
};

void AddSC_boss_epoch()
{
    new boss_epoch();
}
