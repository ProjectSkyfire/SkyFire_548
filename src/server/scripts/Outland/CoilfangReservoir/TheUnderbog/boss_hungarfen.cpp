/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Hungarfen
SD%Complete: 95
SDComment: Need confirmation if spell data are same in both modes. Summons should have faster rate in heroic
SDCategory: Coilfang Resevoir, Underbog
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum Spells
{
    SPELL_FOUL_SPORES           = 31673,
    SPELL_ACID_GEYSER           = 38739,

    SPELL_SPORE_CLOUD           = 34168,
    SPELL_PUTRID_MUSHROOM       = 31690,
    SPELL_GROW                  = 31698
};

class boss_hungarfen : public CreatureScript
{
public:
    boss_hungarfen() : CreatureScript("boss_hungarfen") { }

    struct boss_hungarfenAI : public ScriptedAI
    {
        boss_hungarfenAI(Creature* creature) : ScriptedAI(creature)
        {
            Root = false;
            Mushroom_Timer = 0;
            AcidGeyser_Timer = 0;
        }

        void Reset() OVERRIDE
        {
            Root = false;
            Mushroom_Timer = 5000;                              // 1 mushroom after 5s, then one per 10s. This should be different in heroic mode
            AcidGeyser_Timer = 10000;
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE { }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            if (!HealthAbovePct(20))
            {
                if (!Root)
                {
                    DoCast(me, SPELL_FOUL_SPORES);
                    Root = true;
                }
            }

            if (Mushroom_Timer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    me->SummonCreature(17990, target->GetPositionX()+(rand()%8), target->GetPositionY()+(rand()%8), target->GetPositionZ(), float(rand()%5), TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 22000);
                else
                    me->SummonCreature(17990, me->GetPositionX()+(rand()%8), me->GetPositionY()+(rand()%8), me->GetPositionZ(), float(rand()%5), TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 22000);

                Mushroom_Timer = 10000;
            } else Mushroom_Timer -= diff;

            if (AcidGeyser_Timer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_ACID_GEYSER);
                AcidGeyser_Timer = 10000+rand()%7500;
            } else AcidGeyser_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    private:
        bool Root;
        uint32 Mushroom_Timer;
        uint32 AcidGeyser_Timer;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_hungarfenAI(creature);
    }
};

class npc_underbog_mushroom : public CreatureScript
{
public:
    npc_underbog_mushroom() : CreatureScript("npc_underbog_mushroom") { }

    struct npc_underbog_mushroomAI : public ScriptedAI
    {
        npc_underbog_mushroomAI(Creature* creature) : ScriptedAI(creature)
        {
            Stop = false;
            Grow_Timer = 0;
            Shrink_Timer = 0;
        }

        void Reset() OVERRIDE
        {
            Stop = false;
            Grow_Timer = 0;
            Shrink_Timer = 20000;

            DoCast(me, SPELL_PUTRID_MUSHROOM, true);
            DoCast(me, SPELL_SPORE_CLOUD, true);
        }

        void MoveInLineOfSight(Unit* /*who*/) OVERRIDE { }


        void AttackStart(Unit* /*who*/) OVERRIDE { }

        void EnterCombat(Unit* /*who*/) OVERRIDE { }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (Stop)
                return;

            if (Grow_Timer <= diff)
            {
                DoCast(me, SPELL_GROW);
                Grow_Timer = 3000;
            } else Grow_Timer -= diff;

            if (Shrink_Timer <= diff)
            {
                me->RemoveAurasDueToSpell(SPELL_GROW);
                Stop = true;
            } else Shrink_Timer -= diff;
        }
    private:
        bool Stop;
        uint32 Grow_Timer;
        uint32 Shrink_Timer;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_underbog_mushroomAI(creature);
    }
};

void AddSC_boss_hungarfen()
{
    new boss_hungarfen();
    new npc_underbog_mushroom();
}
