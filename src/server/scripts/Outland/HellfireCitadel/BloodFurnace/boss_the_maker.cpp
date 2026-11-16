/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_The_Maker
SD%Complete: 80
SDComment: Mind control no support
SDCategory: Hellfire Citadel, Blood Furnace
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "blood_furnace.h"

enum Yells
{
    SAY_AGGRO                   = 0,
    SAY_KILL                    = 1,
    SAY_DIE                     = 2
};

enum Spells
{
    SPELL_ACID_SPRAY            = 38153,
    SPELL_EXPLODING_BREAKER     = 30925,
    H_SPELL_EXPLODING_BREAKER   = 40059,
    SPELL_KNOCKDOWN             = 20276,
    SPELL_DOMINATION            = 30923
};

class boss_the_maker : public CreatureScript
{
public:
    boss_the_maker() : CreatureScript("boss_the_maker") { }

    struct boss_the_makerAI : public ScriptedAI
    {
        boss_the_makerAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 AcidSpray_Timer;
        uint32 ExplodingBreaker_Timer;
        uint32 Domination_Timer;
        uint32 Knockdown_Timer;

        void Reset() OVERRIDE
        {
            AcidSpray_Timer = 15000;
            ExplodingBreaker_Timer = 6000;
            Domination_Timer = urand(20000, 30000);
            Knockdown_Timer = 10000;

            if (!instance)
                return;

            instance->SetData(TYPE_THE_MAKER_EVENT, NOT_STARTED);
            instance->HandleGameObject(instance->GetData64(DATA_DOOR2), true);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            Talk(SAY_AGGRO);

            if (!instance)
                return;

            instance->SetData(TYPE_THE_MAKER_EVENT, IN_PROGRESS);
            instance->HandleGameObject(instance->GetData64(DATA_DOOR2), false);
        }

        void KilledUnit(Unit* /*victim*/) OVERRIDE
        {
            Talk(SAY_KILL);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            Talk(SAY_DIE);

            if (!instance)
                return;

            instance->SetData(TYPE_THE_MAKER_EVENT, DONE);
            instance->HandleGameObject(instance->GetData64(DATA_DOOR2), true);
            instance->HandleGameObject(instance->GetData64(DATA_DOOR3), true);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            if (AcidSpray_Timer <= diff)
            {
                DoCastVictim(SPELL_ACID_SPRAY);
                AcidSpray_Timer = 15000 + rand() % 8000;
            }
            else
                AcidSpray_Timer -= diff;

            if (ExplodingBreaker_Timer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, DUNGEON_MODE(SPELL_EXPLODING_BREAKER, H_SPELL_EXPLODING_BREAKER));
                ExplodingBreaker_Timer = 4000 + rand() % 8000;
            }
            else
                ExplodingBreaker_Timer -= diff;

            if (Domination_Timer <= diff)
            {
                // Position 1 skips the tank, so a lone player is never dominated.
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
                    DoCast(target, SPELL_DOMINATION);

                Domination_Timer = urand(20000, 30000);
            }
            else
                Domination_Timer -= diff;

            if (Knockdown_Timer <= diff)
            {
                DoCastVictim(SPELL_KNOCKDOWN);
                Knockdown_Timer = 4000 + rand() % 8000;
            }
            else
                Knockdown_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_the_makerAI(creature);
    }
};

void AddSC_boss_the_maker()
{
    new boss_the_maker();
}

