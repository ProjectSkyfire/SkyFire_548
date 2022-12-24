/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Void_Reaver
SD%Complete: 90
SDComment: Should reset if raid are out of room.
SDCategory: Tempest Keep, The Eye
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "the_eye.h"

enum Yells
{
    SAY_AGGRO                   = 0,
    SAY_SLAY                    = 1,
    SAY_DEATH                   = 2,
    SAY_POUNDING                = 3
};

enum Spells
{
    SPELL_POUNDING              = 34162,
    SPELL_ARCANE_ORB            = 34172,
    SPELL_KNOCK_AWAY            = 25778,
    SPELL_BERSERK               = 27680
};

class boss_void_reaver : public CreatureScript
{
public:
    boss_void_reaver() : CreatureScript("boss_void_reaver") { }

    struct boss_void_reaverAI : public ScriptedAI
    {
        boss_void_reaverAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 Pounding_Timer;
        uint32 ArcaneOrb_Timer;
        uint32 KnockAway_Timer;
        uint32 Berserk_Timer;

        bool Enraged;

        void Reset() OVERRIDE
        {
            Pounding_Timer = 15000;
            ArcaneOrb_Timer = 3000;
            KnockAway_Timer = 30000;
            Berserk_Timer = 600000;

            Enraged = false;

            if (instance && me->IsAlive())
                instance->SetData(DATA_VOIDREAVEREVENT, NOT_STARTED);
        }

        void KilledUnit(Unit* /*victim*/) OVERRIDE
        {
            Talk(SAY_SLAY);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            Talk(SAY_DEATH);
            DoZoneInCombat();

            if (instance)
                instance->SetData(DATA_VOIDREAVEREVENT, DONE);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            Talk(SAY_AGGRO);

            if (instance)
                instance->SetData(DATA_VOIDREAVEREVENT, IN_PROGRESS);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;
            // Pounding
            if (Pounding_Timer <= diff)
            {
                DoCastVictim(SPELL_POUNDING);
                Talk(SAY_POUNDING);
                Pounding_Timer = 15000; //cast time(3000) + cooldown time(12000)
            }
            else
                Pounding_Timer -= diff;
            // Arcane Orb
            if (ArcaneOrb_Timer <= diff)
            {
                Unit* target = NULL;
                std::list<HostileReference*> t_list = me->getThreatManager().getThreatList();
                std::vector<Unit*> target_list;
                for (std::list<HostileReference*>::const_iterator itr = t_list.begin(); itr != t_list.end(); ++itr)
                {
                    target = Unit::GetUnit(*me, (*itr)->getUnitGuid());
                    if (!target)
                        continue;
                    // exclude pets & totems, 18 yard radius minimum
                    if (target->GetTypeId() == TypeID::TYPEID_PLAYER && target->IsAlive() && !target->IsWithinDist(me, 18, false))
                        target_list.push_back(target);
                    target = NULL;
                }

                if (!target_list.empty())
                    target = *(target_list.begin() + rand() % target_list.size());
                else
                    target = me->GetVictim();

                if (target)
                    me->CastSpell(target, SPELL_ARCANE_ORB, false, NULL, NULL, 0);
                ArcaneOrb_Timer = 3000;
            }
            else
                ArcaneOrb_Timer -= diff;
            // Single Target knock back, reduces aggro
            if (KnockAway_Timer <= diff)
            {
                DoCastVictim(SPELL_KNOCK_AWAY);
                //Drop 25% aggro
                if (DoGetThreat(me->GetVictim()))
                    DoModifyThreatPercent(me->GetVictim(), -25);
                KnockAway_Timer = 30000;
            }
            else
                KnockAway_Timer -= diff;
            //Berserk
            if (Berserk_Timer < diff && !Enraged)
            {
                DoCast(me, SPELL_BERSERK);
                Enraged = true;
            }
            else
                Berserk_Timer -= diff;

            DoMeleeAttackIfReady();

            EnterEvadeIfOutOfCombatArea(diff);
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_void_reaverAI(creature);
    }
};

void AddSC_boss_void_reaver()
{
    new boss_void_reaver();
}

