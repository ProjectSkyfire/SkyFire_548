/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "MotionMaster.h"
#include "SpellInfo.h"
#include "culling_of_stratholme.h"

enum Spells
{
    SPELL_CORRUPTING_BLIGHT                     = 60588,
    SPELL_VOID_STRIKE                           = 60590,
    SPELL_CORRUPTION_OF_TIME_CHANNEL            = 60422,
    SPELL_CORRUPTION_OF_TIME_TARGET             = 60451
};

enum Misc
{
    POINT_TIME_RIFT                             = 1
};

enum Yells
{
    SAY_AGGRO                                   = 0,
    SAY_DEATH                                   = 1,
    SAY_FAIL                                    = 2
};

class boss_infinite_corruptor : public CreatureScript
{
public:
    boss_infinite_corruptor() : CreatureScript("boss_infinite_corruptor") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_infinite_corruptorAI(creature);
    }

    struct boss_infinite_corruptorAI : public ScriptedAI
    {
        boss_infinite_corruptorAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 uiBlightTimer;
        uint32 uiStrikeTimer;

        void Reset() OVERRIDE
        {
            uiBlightTimer = 7000;
            uiStrikeTimer = 5000;

            if (instance)
                instance->SetData(DATA_INFINITE_EVENT, NOT_STARTED);

            DoCastAOE(SPELL_CORRUPTION_OF_TIME_CHANNEL); // targets the Guardian of Time
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            Talk(SAY_AGGRO);
            if (instance)
                instance->SetData(DATA_INFINITE_EVENT, IN_PROGRESS);
        }

        void SpellHitTarget(Unit* target, SpellInfo const* spell) OVERRIDE
        {
            if (spell->Id == SPELL_CORRUPTION_OF_TIME_CHANNEL)
                target->CastSpell(target, SPELL_CORRUPTION_OF_TIME_TARGET, true);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            if (uiBlightTimer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 60.0f, true))
                    DoCast(target, SPELL_CORRUPTING_BLIGHT);
                uiBlightTimer = 15000;
            }
            else
                uiBlightTimer -= diff;

            if (uiStrikeTimer <= diff)
            {
                DoCastVictim(SPELL_VOID_STRIKE);
                uiStrikeTimer = 5000;
            }
            else
                uiStrikeTimer -= diff;

            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            Talk(SAY_DEATH);
            if (!instance)
                return;

            instance->SetData(DATA_INFINITE_EVENT, DONE);

            if (Creature* guardian = Unit::GetCreature(*me, instance->GetData64(DATA_GUARDIAN_OF_TIME)))
            {
                guardian->RemoveAurasDueToSpell(SPELL_CORRUPTION_OF_TIME_TARGET);
                guardian->DespawnOrUnsummon(5000);
            }

            if (Creature* rift = Unit::GetCreature(*me, instance->GetData64(DATA_TIME_RIFT)))
                rift->DespawnOrUnsummon();
        }

        void DoAction(int32 action) OVERRIDE
        {
            if (action != ACTION_CORRUPTOR_LEAVE)
                return;

            // The Guardian of Time is lost: the Corruptor walks back into his rift
            me->SetReactState(REACT_PASSIVE);
            me->CombatStop();
            Talk(SAY_FAIL);

            if (instance)
                if (Creature* rift = Unit::GetCreature(*me, instance->GetData64(DATA_TIME_RIFT)))
                {
                    me->GetMotionMaster()->MovePoint(POINT_TIME_RIFT, rift->GetPositionX(), rift->GetPositionY(), rift->GetPositionZ());
                    return;
                }

            me->DespawnOrUnsummon(2000);
        }

        void MovementInform(uint32 type, uint32 id) OVERRIDE
        {
            if (type == POINT_MOTION_TYPE && id == POINT_TIME_RIFT)
            {
                me->DespawnOrUnsummon(2000);
                if (Creature* rift = Unit::GetCreature(*me, instance->GetData64(DATA_TIME_RIFT)))
                    rift->DespawnOrUnsummon(2000);
            }
        }
    };
};

void AddSC_boss_infinite_corruptor()
{
    new boss_infinite_corruptor();
}
