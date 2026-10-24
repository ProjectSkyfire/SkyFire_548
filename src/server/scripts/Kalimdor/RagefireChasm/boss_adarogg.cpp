/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "InstanceScript.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Player.h"
#include "ragefire_chasm.h"

enum Spells
{
    SPELL_INFERNO_CHARGE_EFF = 119299, // triggered by 119405, but actually by handle charge...
};

class boss_adarogg : public CreatureScript
{
    public:
        boss_adarogg() : CreatureScript("boss_adarogg") { }

        enum iSpells
        {
            SPELL_INFERNO_CHARGE     = 119405,
            SPELL_FLAME_BREATH       = 119420, // on victim
        };

        enum iEvents
        {
            EVENT_INFERNO_CHARGE = 1,
            EVENT_FLAME_BREATH   = 2,
        };

        struct boss_adaroggAI : public BossAI
        {
            boss_adaroggAI(Creature* creature) : BossAI(creature, BOSS_ADAROGG)
            {
                m_InfernoTarget = 0;
            }

            uint64 m_InfernoTarget;

            void Reset() OVERRIDE
            {
                _Reset();
                m_InfernoTarget = 0;

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }

            void EnterEvadeMode() OVERRIDE
            {
                BossAI::EnterEvadeMode();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                events.ScheduleEvent(EVENT_INFERNO_CHARGE, urand(11 * IN_MILLISECONDS, 12 * IN_MILLISECONDS));
            }

            void MovementInform(uint32 /*type*/, uint32 pointId) OVERRIDE
            {
                if (pointId == EVENT_CHARGE)
                    if (Player* InfernoTarget = ObjectAccessor::GetPlayer(*me, m_InfernoTarget))
                        DoCast(InfernoTarget, SPELL_INFERNO_CHARGE_EFF);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }

            uint64 GetGUID(int32 type) const OVERRIDE
            {
                if (type == TYPE_TARGET_ID)
                    return m_InfernoTarget;
                return 0;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_INFERNO_CHARGE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, NonTankTargetSelector(me)))
                            {
                                me->CastSpell(target, SPELL_INFERNO_CHARGE, false);
                                m_InfernoTarget = target->GetGUID();
                            }

                            events.ScheduleEvent(EVENT_INFERNO_CHARGE, urand(11 * IN_MILLISECONDS, 12 * IN_MILLISECONDS));
                            events.ScheduleEvent(EVENT_FLAME_BREATH, urand(4 * IN_MILLISECONDS, 6 * IN_MILLISECONDS));
                            break;
                        case EVENT_FLAME_BREATH:
                            if (Unit* vict = me->GetVictim())
                                me->CastSpell(vict, SPELL_FLAME_BREATH, false);
                            break;
                    }
                }

                if (!me->HasAura(SPELL_INFERNO_CHARGE))
                    DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_adaroggAI(creature);
        }
};

// Inferno Charge 119405
class spell_ragefire_inferno_charge : public SpellScriptLoader
{
    public:
        spell_ragefire_inferno_charge() : SpellScriptLoader("spell_ragefire_inferno_charge") { }

        class spell_ragefire_inferno_charge_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ragefire_inferno_charge_AuraScript);

            void OnAuraEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* caster = GetCaster();
                if (!caster || !caster->ToCreature() || !caster->ToCreature()->AI())
                    return;

                if (Player* InfernoTarget = ObjectAccessor::GetPlayer(*caster, caster->ToCreature()->AI()->GetGUID(TYPE_TARGET_ID)))
                    caster->GetMotionMaster()->MoveCharge(InfernoTarget->GetPositionX(), InfernoTarget->GetPositionY(), InfernoTarget->GetPositionZ(), 42.0f, EVENT_CHARGE);
            }

            void Register() OVERRIDE
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_ragefire_inferno_charge_AuraScript::OnAuraEffectRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_ragefire_inferno_charge_AuraScript();
        }
};

void AddSC_boss_adarogg()
{
    new boss_adarogg();
    new spell_ragefire_inferno_charge();
}
