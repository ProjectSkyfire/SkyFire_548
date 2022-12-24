/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "gundrak.h"

enum Spells
{
    SPELL_DETERMINED_STAB                         = 55104,
    SPELL_GROUND_TREMOR                           = 55142,
    SPELL_NUMBING_SHOUT                           = 55106,
    SPELL_DETERMINED_GORE                         = 55102,
    H_SPELL_DETERMINED_GORE                       = 59444,
    SPELL_QUAKE                                   = 55101,
    SPELL_NUMBING_ROAR                            = 55100,
    SPELL_MOJO_FRENZY                             = 55163,
    SPELL_TRANSFORMATION                          = 55098, //Periodic, The caster transforms into a powerful mammoth, increasing Physical damage done by 25% and granting immunity to Stun effects.
};

enum Says
{
    SAY_AGGRO                                     = 0,
    SAY_SLAY                                      = 1,
    SAY_DEATH                                     = 2,
    SAY_TRANSFORM                                 = 3,
    SAY_QUAKE                                     = 4,
    EMOTE_TRANSFORM                               = 5
};

enum Misc
{
    DATA_LESS_RABI                                = 1
};

class boss_moorabi : public CreatureScript
{
public:
    boss_moorabi() : CreatureScript("boss_moorabi") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_moorabiAI(creature);
    }

    struct boss_moorabiAI : public ScriptedAI
    {
        boss_moorabiAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        bool bPhase;

        uint32 uiNumblingShoutTimer;
        uint32 uiGroundTremorTimer;
        uint32 uiDeterminedStabTimer;
        uint32 uiTransformationTImer;

        void Reset() OVERRIDE
        {
            uiGroundTremorTimer = 18*IN_MILLISECONDS;
            uiNumblingShoutTimer =  10*IN_MILLISECONDS;
            uiDeterminedStabTimer = 20*IN_MILLISECONDS;
            uiTransformationTImer = 12*IN_MILLISECONDS;
            bPhase = false;

            if (instance)
                instance->SetData(DATA_MOORABI_EVENT, NOT_STARTED);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            Talk(SAY_AGGRO);
            DoCast(me, SPELL_MOJO_FRENZY, true);

            if (instance)
                instance->SetData(DATA_MOORABI_EVENT, IN_PROGRESS);
        }

        void UpdateAI(uint32 uiDiff) OVERRIDE
        {
            //Return since we have no target
             if (!UpdateVictim())
                 return;

            if (!bPhase && me->HasAura(SPELL_TRANSFORMATION))
            {
                bPhase = true;
                me->RemoveAura(SPELL_MOJO_FRENZY);
            }

            if (uiGroundTremorTimer <= uiDiff)
            {
                Talk(SAY_QUAKE);
                if (bPhase)
                    DoCastVictim(SPELL_QUAKE, true);
                else
                    DoCastVictim(SPELL_GROUND_TREMOR, true);
                uiGroundTremorTimer = 10*IN_MILLISECONDS;
            } else uiGroundTremorTimer -= uiDiff;

            if (uiNumblingShoutTimer <= uiDiff)
            {
                if (bPhase)
                    DoCastVictim(SPELL_NUMBING_ROAR, true);
                else
                    DoCastVictim(SPELL_NUMBING_SHOUT, true);
                uiNumblingShoutTimer = 10*IN_MILLISECONDS;
            } else uiNumblingShoutTimer -=uiDiff;

            if (uiDeterminedStabTimer <= uiDiff)
            {
                if (bPhase)
                    DoCastVictim(SPELL_DETERMINED_GORE);
                else
                    DoCastVictim(SPELL_DETERMINED_STAB, true);
                uiDeterminedStabTimer = 8*IN_MILLISECONDS;
            } else uiDeterminedStabTimer -=uiDiff;

            if (!bPhase && uiTransformationTImer <= uiDiff)
            {
                Talk(EMOTE_TRANSFORM);
                Talk(SAY_TRANSFORM);
                DoCast(me, SPELL_TRANSFORMATION, false);
                uiTransformationTImer = 10*IN_MILLISECONDS;
            } else uiTransformationTImer -= uiDiff;

            DoMeleeAttackIfReady();
         }

        uint32 GetData(uint32 type) const OVERRIDE
        {
            if (type == DATA_LESS_RABI)
                return bPhase ? 0 : 1;

            return 0;
        }

         void JustDied(Unit* /*killer*/) OVERRIDE
         {
            Talk(SAY_DEATH);

            if (instance)
                instance->SetData(DATA_MOORABI_EVENT, DONE);
        }

        void KilledUnit(Unit* victim) OVERRIDE
        {
            if (victim->GetTypeId() != TypeID::TYPEID_PLAYER)
                return;

            Talk(SAY_SLAY);
        }
    };
};

class achievement_less_rabi : public AchievementCriteriaScript
{
    public:
        achievement_less_rabi() : AchievementCriteriaScript("achievement_less_rabi")
        {
        }

        bool OnCheck(Player* /*player*/, Unit* target) OVERRIDE
        {
            if (!target)
                return false;

            if (Creature* Moorabi = target->ToCreature())
                if (Moorabi->AI()->GetData(DATA_LESS_RABI))
                    return true;

            return false;
        }
};

void AddSC_boss_moorabi()
{
    new boss_moorabi();
    new achievement_less_rabi();
}
