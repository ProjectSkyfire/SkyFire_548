/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Captain_Skarloc
SD%Complete: 75
SDComment: Missing adds, missing waypoints to move up to Thrall once spawned + speech before enter combat.
SDCategory: Caverns of Time, Old Hillsbrad Foothills
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "old_hillsbrad.h"

/*######################
# boss_captain_skarloc #
#######################*/

enum CaptainSkarloc
{
    SAY_ENTER                   = 0,
    SAY_TAUNT1                  = 1,
    SAY_TAUNT2                  = 2,
    SAY_SLAY                    = 3,
    SAY_DEATH                   = 4,

    SPELL_HOLY_LIGHT            = 29427,
    SPELL_CLEANSE               = 29380,
    SPELL_HAMMER_OF_JUSTICE     = 13005,
    SPELL_HOLY_SHIELD           = 31904,
    SPELL_DEVOTION_AURA         = 8258,
    SPELL_CONSECRATION          = 38385
};

class boss_captain_skarloc : public CreatureScript
{
public:
    boss_captain_skarloc() : CreatureScript("boss_captain_skarloc") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_captain_skarlocAI(creature);
    }

    struct boss_captain_skarlocAI : public ScriptedAI
    {
        boss_captain_skarlocAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 Holy_Light_Timer;
        uint32 Cleanse_Timer;
        uint32 HammerOfJustice_Timer;
        uint32 HolyShield_Timer;
        uint32 DevotionAura_Timer;
        uint32 Consecration_Timer;

        void Reset() OVERRIDE
        {
            Holy_Light_Timer = std::rand() % 30000 + 20000;
            Cleanse_Timer = 10000;
            HammerOfJustice_Timer = std::rand() % 35000 + 20000;
            HolyShield_Timer = 240000;
            DevotionAura_Timer = 3000;
            Consecration_Timer = 8000;
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            //This is not correct. Should taunt Thrall before engage in combat
            Talk(SAY_TAUNT1);
            Talk(SAY_TAUNT2);
        }

        void KilledUnit(Unit* /*victim*/) OVERRIDE
        {
            Talk(SAY_SLAY);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            Talk(SAY_DEATH);

            if (instance && instance->GetData(TYPE_THRALL_EVENT) == IN_PROGRESS)
                instance->SetData(TYPE_THRALL_PART1, DONE);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            //Holy_Light
            if (Holy_Light_Timer <= diff)
            {
                DoCast(me, SPELL_HOLY_LIGHT);
                Holy_Light_Timer = 30000;
            } else Holy_Light_Timer -= diff;

            //Cleanse
            if (Cleanse_Timer <= diff)
            {
                DoCast(me, SPELL_CLEANSE);
                Cleanse_Timer = 10000;
            } else Cleanse_Timer -= diff;

            //Hammer of Justice
            if (HammerOfJustice_Timer <= diff)
            {
                DoCastVictim(SPELL_HAMMER_OF_JUSTICE);
                HammerOfJustice_Timer = 60000;
            } else HammerOfJustice_Timer -= diff;

            //Holy Shield
            if (HolyShield_Timer <= diff)
            {
                DoCast(me, SPELL_HOLY_SHIELD);
                HolyShield_Timer = 240000;
            } else HolyShield_Timer -= diff;

            //Devotion_Aura
            if (DevotionAura_Timer <= diff)
            {
                DoCast(me, SPELL_DEVOTION_AURA);
                DevotionAura_Timer = std::rand() % 55000 + 45000;
            } else DevotionAura_Timer -= diff;

            //Consecration
            if (Consecration_Timer <= diff)
            {
                //DoCastVictim(SPELL_CONSECRATION);
                Consecration_Timer = std::rand() % 10000 + 5000;
            } else Consecration_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_captain_skarloc()
{
    new boss_captain_skarloc();
}
