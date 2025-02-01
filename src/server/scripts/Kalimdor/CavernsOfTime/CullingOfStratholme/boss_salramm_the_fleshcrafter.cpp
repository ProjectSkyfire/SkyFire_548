/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* Script Data Start
SDName: Boss salramm
SDAuthor: Tartalo
SD%Complete: 80
SDComment: @todo Intro
SDCategory:
Script Data End */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "culling_of_stratholme.h"

enum Spells
{
    SPELL_CURSE_OF_TWISTED_FLESH                = 58845,
    SPELL_EXPLODE_GHOUL                         = 52480,
    H_SPELL_EXPLODE_GHOUL                       = 58825,
    SPELL_SHADOW_BOLT                           = 57725,
    H_SPELL_SHADOW_BOLT                         = 58828,
    SPELL_STEAL_FLESH                           = 52708,
    SPELL_SUMMON_GHOULS                         = 52451
};

enum Yells
{
    SAY_AGGRO                                   = 0,
    SAY_SPAWN                                   = 1,
    SAY_SLAY                                    = 2,
    SAY_DEATH                                   = 3,
    SAY_EXPLODE_GHOUL                           = 4,
    SAY_STEAL_FLESH                             = 5,
    SAY_SUMMON_GHOULS                           = 6
};

class boss_salramm : public CreatureScript
{
public:
    boss_salramm() : CreatureScript("boss_salramm") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_salrammAI(creature);
    }

    struct boss_salrammAI : public ScriptedAI
    {
        boss_salrammAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
            if (instance)
                Talk(SAY_SPAWN);
        }

        uint32 uiCurseFleshTimer;
        uint32 uiExplodeGhoulTimer;
        uint32 uiShadowBoltTimer;
        uint32 uiStealFleshTimer;
        uint32 uiSummonGhoulsTimer;

        InstanceScript* instance;

        void Reset() OVERRIDE
        {
             uiCurseFleshTimer = 30000;  //30s DBM
             uiExplodeGhoulTimer = std::rand() % 28000 + 25000; //approx 6 sec after summon ghouls
             uiShadowBoltTimer = std::rand() % 12000 + 8000; // approx 10s
             uiStealFleshTimer = 12345;
             uiSummonGhoulsTimer = std::rand() % 24000 + 19000; //on a video approx 24s after aggro

             if (instance)
                 instance->SetData(DATA_SALRAMM_EVENT, NOT_STARTED);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            Talk(SAY_AGGRO);

            if (instance)
                 instance->SetData(DATA_SALRAMM_EVENT, IN_PROGRESS);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            //Curse of twisted flesh timer
            if (uiCurseFleshTimer <= diff)
            {
                DoCastVictim(SPELL_CURSE_OF_TWISTED_FLESH);
                uiCurseFleshTimer = 37000;
            } else uiCurseFleshTimer -= diff;

            //Shadow bolt timer
            if (uiShadowBoltTimer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_SHADOW_BOLT);
                uiShadowBoltTimer = std::rand() % 12000 + 8000;
            } else uiShadowBoltTimer -= diff;

            //Steal Flesh timer
            if (uiStealFleshTimer <= diff)
            {
                Talk(SAY_STEAL_FLESH);
                if (Unit* random_pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(random_pTarget, SPELL_STEAL_FLESH);
                uiStealFleshTimer = 10000;
            } else uiStealFleshTimer -= diff;

            //Summon ghouls timer
            if (uiSummonGhoulsTimer <= diff)
            {
                Talk(SAY_SUMMON_GHOULS);
                if (Unit* random_pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(random_pTarget, SPELL_SUMMON_GHOULS);
                uiSummonGhoulsTimer = 10000;
            } else uiSummonGhoulsTimer -= diff;

            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            Talk(SAY_DEATH);

            if (instance)
                instance->SetData(DATA_SALRAMM_EVENT, DONE);
        }

        void KilledUnit(Unit* victim) OVERRIDE
        {
            if (victim->GetTypeId() != TypeID::TYPEID_PLAYER)
                return;

            Talk(SAY_SLAY);
        }
    };
};

void AddSC_boss_salramm()
{
    new boss_salramm();
}
