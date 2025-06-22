/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Maiden_of_Virtue
SD%Complete: 100
SDComment:
SDCategory: Karazhan
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum MaidenOfVirtue
{
    SAY_AGGRO               = 0,
    SAY_SLAY                = 1,
    SAY_REPENTANCE          = 2,
    SAY_DEATH               = 3,

    SPELL_REPENTANCE        = 29511,
    SPELL_HOLYFIRE          = 29522,
    SPELL_HOLYWRATH         = 32445,
    SPELL_HOLYGROUND        = 29512,
    SPELL_BERSERK           = 26662,
};

class boss_maiden_of_virtue : public CreatureScript
{
public:
    boss_maiden_of_virtue() : CreatureScript("boss_maiden_of_virtue") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_maiden_of_virtueAI(creature);
    }

    struct boss_maiden_of_virtueAI : public ScriptedAI
    {
        boss_maiden_of_virtueAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 Repentance_Timer;
        uint32 Holyfire_Timer;
        uint32 Holywrath_Timer;
        uint32 Holyground_Timer;
        uint32 Enrage_Timer;

        bool Enraged;

        void Reset() OVERRIDE
        {
            Repentance_Timer    = 25000+(rand()%15000);
            Holyfire_Timer      = 8000+(rand()%17000);
            Holywrath_Timer     = 15000+(rand()%10000);
            Holyground_Timer    = 3000;
            Enrage_Timer        = 600000;

            Enraged = false;
        }

        void KilledUnit(Unit* /*Victim*/) OVERRIDE
        {
            if ((std::rand() % 1) == 0)
                Talk(SAY_SLAY);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            Talk(SAY_DEATH);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            Talk(SAY_AGGRO);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            if (Enrage_Timer < diff && !Enraged)
            {
                DoCast(me, SPELL_BERSERK, true);
                Enraged = true;
            } else Enrage_Timer -= diff;

            if (Holyground_Timer <= diff)
            {
                DoCast(me, SPELL_HOLYGROUND, true);   //Triggered so it doesn't interrupt her at all
                Holyground_Timer = 3000;
            } else Holyground_Timer -= diff;

            if (Repentance_Timer <= diff)
            {
                DoCastVictim(SPELL_REPENTANCE);
                Talk(SAY_REPENTANCE);

                Repentance_Timer = std::rand() % 35000 + 25000;        //A little randomness on that spell
            } else Repentance_Timer -= diff;

            if (Holyfire_Timer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    DoCast(target, SPELL_HOLYFIRE);

                Holyfire_Timer = std::rand() % 23000 + 8000;      //Anywhere from 8 to 23 seconds, good luck having several of those in a row!
            } else Holyfire_Timer -= diff;

            if (Holywrath_Timer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    DoCast(target, SPELL_HOLYWRATH);

                Holywrath_Timer = std::rand() % 25000 + 20000;        //20-30 secs sounds nice
            } else Holywrath_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_maiden_of_virtue()
{
    new boss_maiden_of_virtue();
}
