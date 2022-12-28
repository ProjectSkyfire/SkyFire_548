/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "gundrak.h"

enum Spells
{
    SPELL_ECK_BERSERK                             = 55816, //Eck goes berserk, increasing his attack speed by 150% and all damage he deals by 500%.
    SPELL_ECK_BITE                                = 55813, //Eck bites down hard, inflicting 150% of his normal damage to an enemy.
    SPELL_ECK_SPIT                                = 55814, //Eck spits toxic bile at enemies in a cone in front of him, inflicting 2970 Nature damage and draining 220 mana every 1 sec for 3 sec.
    SPELL_ECK_SPRING_1                            = 55815, //Eck leaps at a distant target.  --> Drops aggro and charges a random player. Tank can simply taunt him back.
    SPELL_ECK_SPRING_2                            = 55837  //Eck leaps at a distant target.
};

static Position EckSpawnPoint = { 1643.877930f, 936.278015f, 107.204948f, 0.668432f };

class boss_eck : public CreatureScript
{
public:
    boss_eck() : CreatureScript("boss_eck") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_eckAI(creature);
    }

    struct boss_eckAI : public ScriptedAI
    {
        boss_eckAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        uint32 uiBerserkTimer;
        uint32 uiBiteTimer;
        uint32 uiSpitTimer;
        uint32 uiSpringTimer;

        bool bBerserk;

        InstanceScript* instance;

        void Reset() override
        {
            uiBerserkTimer = urand(60*IN_MILLISECONDS, 90*IN_MILLISECONDS); //60-90 secs according to wowwiki
            uiBiteTimer = 5*IN_MILLISECONDS;
            uiSpitTimer = 10*IN_MILLISECONDS;
            uiSpringTimer = 8*IN_MILLISECONDS;

            bBerserk = false;

            if (instance)
                instance->SetData(DATA_ECK_THE_FEROCIOUS_EVENT, NOT_STARTED);
        }

        void EnterCombat(Unit* /*who*/) override
        {
            if (instance)
                instance->SetData(DATA_ECK_THE_FEROCIOUS_EVENT, IN_PROGRESS);
        }

        void UpdateAI(uint32 diff) override
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            if (uiBiteTimer <= diff)
            {
                DoCastVictim(SPELL_ECK_BITE);
                uiBiteTimer = urand(8*IN_MILLISECONDS, 12*IN_MILLISECONDS);
            } else uiBiteTimer -= diff;

            if (uiSpitTimer <= diff)
            {
                DoCastVictim(SPELL_ECK_SPIT);
                uiSpitTimer = urand(6*IN_MILLISECONDS, 14*IN_MILLISECONDS);
            } else uiSpitTimer -= diff;

            if (uiSpringTimer <= diff)
            {
                Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1);
                if (target && target->GetTypeId() == TypeID::TYPEID_PLAYER)
                {
                    DoCast(target, RAND(SPELL_ECK_SPRING_1, SPELL_ECK_SPRING_2));
                    uiSpringTimer = urand(5*IN_MILLISECONDS, 10*IN_MILLISECONDS);
                }
            } else uiSpringTimer -= diff;

            //Berserk on timer or 20% of health
            if (!bBerserk)
            {
                if (uiBerserkTimer <= diff)
                {
                    DoCast(me, SPELL_ECK_BERSERK);
                    bBerserk = true;
                }
                else
                {
                    uiBerserkTimer -= diff;
                    if (HealthBelowPct(20))
                    {
                        DoCast(me, SPELL_ECK_BERSERK);
                        bBerserk = true;
                    }
                }
            }

            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* /*killer*/) override
        {
            if (instance)
                instance->SetData(DATA_ECK_THE_FEROCIOUS_EVENT, DONE);
        }
    };
};

class npc_ruins_dweller : public CreatureScript
{
public:
    npc_ruins_dweller() : CreatureScript("npc_ruins_dweller") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ruins_dwellerAI(creature);
    }

    struct npc_ruins_dwellerAI : public ScriptedAI
    {
        npc_ruins_dwellerAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        void JustDied(Unit* /*killer*/) override
        {
            if (instance)
            {
                instance->SetData64(DATA_RUIN_DWELLER_DIED, me->GetGUID());
                if (instance->GetData(DATA_ALIVE_RUIN_DWELLERS) == 0)
                    me->SummonCreature(CREATURE_ECK, EckSpawnPoint, TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 300*IN_MILLISECONDS);
            }
        }
    };
};

void AddSC_boss_eck()
{
    new boss_eck();
    new npc_ruins_dweller();
}
