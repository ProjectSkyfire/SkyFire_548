/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellAuraEffects.h"
#include "SpellScript.h"
#include "violet_hold.h"

//Spells
enum Spells
{
    SPELL_CORROSIVE_SALIVA                     = 54527,
    SPELL_OPTIC_LINK                           = 54396,
    SPELL_RAY_OF_PAIN                          = 54438,
    H_SPELL_RAY_OF_PAIN                        = 59523,
    SPELL_RAY_OF_SUFFERING                     = 54442,
    H_SPELL_RAY_OF_SUFFERING                   = 59524,

    // Visual
    SPELL_OPTIC_LINK_LEVEL_1                   = 54393,
    SPELL_OPTIC_LINK_LEVEL_2                   = 54394,
    SPELL_OPTIC_LINK_LEVEL_3                   = 54395
};

class boss_moragg : public CreatureScript
{
public:
    boss_moragg() : CreatureScript("boss_moragg") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_moraggAI(creature);
    }

    struct boss_moraggAI : public ScriptedAI
    {
        boss_moraggAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        uint32 uiOpticLinkTimer;
        uint32 uiCorrosiveSalivaTimer;

        InstanceScript* instance;

        void Reset() OVERRIDE
        {
            uiOpticLinkTimer = 10000;
            uiCorrosiveSalivaTimer = 5000;

            if (instance)
            {
                if (instance->GetData(DATA_WAVE_COUNT) == 6)
                    instance->SetData(DATA_1ST_BOSS_EVENT, NOT_STARTED);
                else if (instance->GetData(DATA_WAVE_COUNT) == 12)
                    instance->SetData(DATA_2ND_BOSS_EVENT, NOT_STARTED);
            }
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            if (instance)
            {
                if (GameObject* pDoor = instance->instance->GetGameObject(instance->GetData64(DATA_MORAGG_CELL)))
                    if (pDoor->GetGoState() == GOState::GO_STATE_READY)
                   {
                        EnterEvadeMode();
                        return;
                    }
                if (instance->GetData(DATA_WAVE_COUNT) == 6)
                    instance->SetData(DATA_1ST_BOSS_EVENT, IN_PROGRESS);
                else if (instance->GetData(DATA_WAVE_COUNT) == 12)
                    instance->SetData(DATA_2ND_BOSS_EVENT, IN_PROGRESS);
            }

            DoCast(me, DUNGEON_MODE(SPELL_RAY_OF_PAIN, H_SPELL_RAY_OF_PAIN));
            DoCast(me, DUNGEON_MODE(SPELL_RAY_OF_SUFFERING, H_SPELL_RAY_OF_SUFFERING));
        }

        void AttackStart(Unit* who) OVERRIDE
        {
            if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC) || me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                return;

            if (me->Attack(who, true))
            {
                me->AddThreat(who, 0.0f);
                me->SetInCombatWith(who);
                who->SetInCombatWith(me);
                DoStartMovement(who);
            }
        }

        void MoveInLineOfSight(Unit* /*who*/) OVERRIDE { }


        void UpdateAI(uint32 diff) OVERRIDE
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            if (uiOpticLinkTimer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    DoCast(target, SPELL_OPTIC_LINK);
                uiOpticLinkTimer = 15000;
            } else uiOpticLinkTimer -= diff;

            if (uiCorrosiveSalivaTimer <= diff)
            {
                DoCastVictim(SPELL_CORROSIVE_SALIVA);
                uiCorrosiveSalivaTimer = 10000;
            } else uiCorrosiveSalivaTimer -= diff;

            DoMeleeAttackIfReady();
        }
        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            if (instance)
            {
                if (instance->GetData(DATA_WAVE_COUNT) == 6)
                {
                    instance->SetData(DATA_1ST_BOSS_EVENT, DONE);
                    instance->SetData(DATA_WAVE_COUNT, 7);
                }
                else if (instance->GetData(DATA_WAVE_COUNT) == 12)
                {
                    instance->SetData(DATA_2ND_BOSS_EVENT, DONE);
                    instance->SetData(DATA_WAVE_COUNT, 13);
                }
            }
        }
    };
};

// 54438, 59523 - Ray of Pain
// 54442, 59524 - Ray of Suffering
class spell_moragg_ray : public AuraScript
{
    PrepareAuraScript(spell_moragg_ray);

    void OnPeriodic(AuraEffect const* aurEff)
    {
        PreventDefaultAction();

        if (UnitAI* ai = GetTarget()->GetAI())
            if (Unit* target = ai->SelectTarget(SELECT_TARGET_RANDOM, 0, 45.0f, true))
                GetTarget()->CastSpell(target, GetSpellInfo()->Effects[aurEff->GetEffIndex()].TriggerSpell, true, NULL, aurEff);
    }

    void Register() OVERRIDE
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_moragg_ray::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};

// 54396 - Optic Link
class spell_moragg_optic_link : public AuraScript
{
    PrepareAuraScript(spell_moragg_optic_link);

    void OnPeriodic(AuraEffect const* aurEff)
    {
        if (Unit* caster = GetCaster())
        {
            if (aurEff->GetTickNumber() >= 8)
                caster->CastSpell(GetTarget(), SPELL_OPTIC_LINK_LEVEL_3, true, NULL, aurEff);

            if (aurEff->GetTickNumber() >= 4)
                caster->CastSpell(GetTarget(), SPELL_OPTIC_LINK_LEVEL_2, true, NULL, aurEff);

            caster->CastSpell(GetTarget(), SPELL_OPTIC_LINK_LEVEL_1, true, NULL, aurEff);
        }
    }

    void OnUpdate(AuraEffect* aurEff)
    {
        switch (aurEff->GetTickNumber())
        {
            case 1:
                aurEff->SetAmount(aurEff->GetAmount() + 250); // base amount is 500
                break;
            case 4:
                aurEff->SetAmount(aurEff->GetAmount() * 2); // goes to 1500
                break;
            case 8:
                aurEff->SetAmount(aurEff->GetAmount() * 2); // goes to 3000
                break;
            default:
                break;
        }
    }

    void Register() OVERRIDE
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_moragg_optic_link::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        OnEffectUpdatePeriodic += AuraEffectUpdatePeriodicFn(spell_moragg_optic_link::OnUpdate, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
    }
};

void AddSC_boss_moragg()
{
    new boss_moragg();
    RegisterAuraScript(spell_moragg_ray);
    RegisterAuraScript(spell_moragg_optic_link);
}
