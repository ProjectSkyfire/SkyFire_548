/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_High_Botanist_Freywinn
SD%Complete: 90
SDComment: some strange visual related to tree form(if aura lost before normal duration end). possible make summon&transform -process smoother(transform after delay)
SDCategory: Tempest Keep, The Botanica
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "the_botanica.h"

enum Says
{
    SAY_AGGRO                  = 0,
    SAY_KILL                   = 1,
    SAY_TREE                   = 2,
    SAY_SUMMON                 = 3,
    SAY_DEATH                  = 4,
    SAY_OOC_RANDOM             = 5
};

enum Spells
{
    SPELL_TRANQUILITY          = 34550,
    SPELL_TREE_FORM            = 34551,
    SPELL_SUMMON_FRAYER        = 34557,
    SPELL_PLANT_WHITE          = 34759,
    SPELL_PLANT_GREEN          = 34761,
    SPELL_PLANT_BLUE           = 34762,
    SPELL_PLANT_RED            = 34763
};

enum Creatures
{
    NPC_FRAYER                 = 19953
};

class boss_high_botanist_freywinn : public CreatureScript
{
    public:
        boss_high_botanist_freywinn() : CreatureScript("boss_high_botanist_freywinn") { }

        struct boss_high_botanist_freywinnAI : public BossAI
        {
            boss_high_botanist_freywinnAI(Creature* creature) : BossAI(creature, DATA_HIGH_BOTANIST_FREYWINN) { }

            std::list<uint64> Adds_List;

            uint32 SummonSeedling_Timer;
            uint32 TreeForm_Timer;
            uint32 MoveCheck_Timer;
            uint32 DeadAddsCount;
            bool MoveFree;

            void Reset() override
            {
                Adds_List.clear();

                SummonSeedling_Timer = 6000;
                TreeForm_Timer = 30000;
                MoveCheck_Timer = 1000;
                DeadAddsCount = 0;
                MoveFree = true;
            }

            void EnterCombat(Unit* /*who*/) override
            {
                Talk(SAY_AGGRO);
            }

            void JustSummoned(Creature* summoned) override
            {
                if (summoned->GetEntry() == NPC_FRAYER)
                    Adds_List.push_back(summoned->GetGUID());
            }

            void DoSummonSeedling()
            {
                switch (rand()%4)
                {
                    case 0: DoCast(me, SPELL_PLANT_WHITE); break;
                    case 1: DoCast(me, SPELL_PLANT_GREEN); break;
                    case 2: DoCast(me, SPELL_PLANT_BLUE); break;
                    case 3: DoCast(me, SPELL_PLANT_RED); break;
                }
            }

            void KilledUnit(Unit* /*victim*/) override
            {
                Talk(SAY_KILL);
            }

            void JustDied(Unit* /*killer*/) override
            {
                Talk(SAY_DEATH);
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                if (TreeForm_Timer <= diff)
                {
                    Talk(SAY_TREE);

                    if (me->IsNonMeleeSpellCasted(false))
                        me->InterruptNonMeleeSpells(true);

                    me->RemoveAllAuras();

                    DoCast(me, SPELL_SUMMON_FRAYER, true);
                    DoCast(me, SPELL_TRANQUILITY, true);
                    DoCast(me, SPELL_TREE_FORM, true);

                    me->GetMotionMaster()->MoveIdle();
                    MoveFree = false;

                    TreeForm_Timer = 75000;
                }
                else
                    TreeForm_Timer -= diff;

                if (!MoveFree)
                {
                    if (MoveCheck_Timer <= diff)
                    {
                        if (!Adds_List.empty())
                        {
                            for (std::list<uint64>::iterator itr = Adds_List.begin(); itr != Adds_List.end(); ++itr)
                            {
                                if (Unit* temp = Unit::GetUnit(*me, *itr))
                                {
                                    if (!temp->IsAlive())
                                    {
                                        Adds_List.erase(itr);
                                        ++DeadAddsCount;
                                        break;
                                    }
                                }
                            }
                        }

                        if (DeadAddsCount < 3 && TreeForm_Timer-30000 <= diff)
                            DeadAddsCount = 3;

                        if (DeadAddsCount >= 3)
                        {
                            Adds_List.clear();
                            DeadAddsCount = 0;

                            me->InterruptNonMeleeSpells(true);
                            me->RemoveAllAuras();
                            me->GetMotionMaster()->MoveChase(me->GetVictim());
                            MoveFree = true;
                        }
                        MoveCheck_Timer = 500;
                    }
                    else
                        MoveCheck_Timer -= diff;

                    return;
                }

                /*if (me->HasAura(SPELL_TREE_FORM, 0) || me->HasAura(SPELL_TRANQUILITY, 0))
                    return;*/

                //one random seedling every 5 secs, but not in tree form
                if (SummonSeedling_Timer <= diff)
                {
                    DoSummonSeedling();
                    SummonSeedling_Timer = 6000;
                }
                else
                    SummonSeedling_Timer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new boss_high_botanist_freywinnAI(creature);
        }
};

void AddSC_boss_high_botanist_freywinn()
{
    new boss_high_botanist_freywinn();
}

