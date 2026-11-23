/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Baron_Rivendare
SD%Complete: 70
SDComment: aura applied/defined in database
SDCategory: Stratholme
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "stratholme.h"

enum Texts
{
    EMOTE_RAISE_DEAD            = 0,
    EMOTE_DEATH_PACT            = 1
};

enum Spells
{
    SPELL_SHADOWBOLT            = 17393,
    SPELL_CLEAVE                = 845,
    SPELL_MORTALSTRIKE          = 16856,

    SPELL_RAISE_DEAD            = 17473,  // triggers Death Pact after 12 seconds

    SPELL_RAISE_DEAD1           = 17475,
    SPELL_RAISE_DEAD2           = 17476,
    SPELL_RAISE_DEAD3           = 17477,
    SPELL_RAISE_DEAD4           = 17478,
    SPELL_RAISE_DEAD5           = 17479,
    SPELL_RAISE_DEAD6           = 17480
};

uint32 const RaiseDeadSkeletons[6] =
{
    SPELL_RAISE_DEAD1, SPELL_RAISE_DEAD2, SPELL_RAISE_DEAD3,
    SPELL_RAISE_DEAD4, SPELL_RAISE_DEAD5, SPELL_RAISE_DEAD6
};

class boss_baron_rivendare : public CreatureScript
{
public:
    boss_baron_rivendare() : CreatureScript("boss_baron_rivendare") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_baron_rivendareAI(creature);
    }

    struct boss_baron_rivendareAI : public ScriptedAI
    {
        boss_baron_rivendareAI(Creature* creature) : ScriptedAI(creature), summons(me)
        {
            instance = me->GetInstanceScript();
        }

        InstanceScript* instance;
        SummonList summons;

        uint32 ShadowBolt_Timer;
        uint32 Cleave_Timer;
        uint32 MortalStrike_Timer;
        uint32 RaiseDead_Timer;
        uint32 SummonSkeletons_Timer;
        uint32 DeathPact_Timer;

        void Reset() OVERRIDE
        {
            summons.DespawnAll();
            ShadowBolt_Timer = 5000;
            Cleave_Timer = 8000;
            MortalStrike_Timer = 12000;
            RaiseDead_Timer = 10000;
            SummonSkeletons_Timer = 0;
            DeathPact_Timer = 0;
            if (instance && instance->GetData(TYPE_RAMSTEIN) == DONE)
                instance->SetData(TYPE_BARON, NOT_STARTED);
        }

        void AttackStart(Unit* who) OVERRIDE
        {
            if (instance)//can't use entercombat(), boss' dmg aura sets near players in combat, before entering the room's door
                instance->SetData(TYPE_BARON, IN_PROGRESS);
            ScriptedAI::AttackStart(who);
        }

        void JustSummoned(Creature* summoned) OVERRIDE
        {
            summons.Summon(summoned);
            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                summoned->AI()->AttackStart(target);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            summons.DespawnAll();
            if (instance)
                instance->SetData(TYPE_BARON, DONE);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            //ShadowBolt
            if (ShadowBolt_Timer <= diff)
            {
                if (SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCastVictim(SPELL_SHADOWBOLT);

                ShadowBolt_Timer = 10000;
            } else ShadowBolt_Timer -= diff;

            //Cleave
            if (Cleave_Timer <= diff)
            {
                DoCastVictim(SPELL_CLEAVE);
                //13 seconds until we should cast this again
                Cleave_Timer = 7000 + (rand()%10000);
            } else Cleave_Timer -= diff;

            //MortalStrike
            if (MortalStrike_Timer <= diff)
            {
                DoCastVictim(SPELL_MORTALSTRIKE);
                MortalStrike_Timer = 10000 + (rand()%15000);
            } else MortalStrike_Timer -= diff;

            //RaiseDead
            if (RaiseDead_Timer <= diff)
            {
                Talk(EMOTE_RAISE_DEAD);
                DoCast(me, SPELL_RAISE_DEAD);
                SummonSkeletons_Timer = 1000;
                DeathPact_Timer = 12000;
                RaiseDead_Timer = 20000;
            } else RaiseDead_Timer -= diff;

            //SummonSkeletons
            if (SummonSkeletons_Timer)
            {
                if (SummonSkeletons_Timer <= diff)
                {
                    for (uint32 spellId : RaiseDeadSkeletons)
                        DoCast(me, spellId, true);

                    SummonSkeletons_Timer = 0;
                } else SummonSkeletons_Timer -= diff;
            }

            //DeathPact
            if (DeathPact_Timer)
            {
                if (DeathPact_Timer <= diff)
                {
                    Talk(EMOTE_DEATH_PACT);
                    DeathPact_Timer = 0;
                } else DeathPact_Timer -= diff;
            }

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_baron_rivendare()
{
    new boss_baron_rivendare();
}
