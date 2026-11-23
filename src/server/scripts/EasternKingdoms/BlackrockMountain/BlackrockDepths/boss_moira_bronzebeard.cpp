/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum Spells
{
    SPELL_HEAL                                             = 15586,
    SPELL_RENEW                                            = 8362,
    SPELL_MINDBLAST                                        = 15587
};

class boss_moira_bronzebeard : public CreatureScript
{
public:
    boss_moira_bronzebeard() : CreatureScript("boss_moira_bronzebeard") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_moira_bronzebeardAI(creature);
    }

    struct boss_moira_bronzebeardAI : public ScriptedAI
    {
        boss_moira_bronzebeardAI(Creature* creature) : ScriptedAI(creature)
        {
            Heal_Timer = 0;
            Renew_Timer = 0;
            MindBlast_Timer = 0;
        }

        void Reset() OVERRIDE
        {
            Heal_Timer = 12000;                                 //These times are probably wrong
            Renew_Timer = 6000;
            MindBlast_Timer = 16000;
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE { }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            //Heal_Timer
            if (Heal_Timer <= diff)
            {
                if (Unit* target = DoSelectLowestHpFriendly(40.0f))
                    DoCast(target, SPELL_HEAL);
                Heal_Timer = 12000;
            } else Heal_Timer -= diff;

            //Renew_Timer
            if (Renew_Timer <= diff)
            {
                if (Unit* target = DoSelectLowestHpFriendly(40.0f))
                    DoCast(target, SPELL_RENEW);
                Renew_Timer = 15000;
            } else Renew_Timer -= diff;

            //MindBlast_Timer
            if (MindBlast_Timer <= diff)
            {
                DoCastVictim(SPELL_MINDBLAST);
                MindBlast_Timer = 14000;
            } else MindBlast_Timer -= diff;
        }

    private:
        uint32 Heal_Timer;
        uint32 Renew_Timer;
        uint32 MindBlast_Timer;
    };
};

void AddSC_boss_moira_bronzebeard()
{
    new boss_moira_bronzebeard();
}
