/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "shadowfang_keep.h"

enum ScriptTexts
{
    SAY_AGGRO                = 0,
    SAY_DEATH                = 1,
    SAY_ASPHYXIATE           = 2,
    SAY_STAY_OF_EXECUTION    = 3,
    SAY_DARK_ARCHANGEL_FORM  = 4,
    SAY_KILL                 = 5,
};

enum Events
{
    EVENT_PAIN_AND_SUFFERING = 1,
    EVENT_ASPHYXIATE         = 2,
    EVENT_STAY_OF_EXECUTION  = 3,
    EVENT_CALAMITY           = 4,
    EVENT_WRACKING_PAIN      = 5,
};

enum Spells
{
    SPELL_PAIN_AND_SUFFERING    = 93581,
    SPELL_ASPHYXIATE            = 93423,
    SPELL_ASPHYXIATE_DMG        = 93424,
    SPELL_STAY_OF_EXECUTION     = 93468,
    SPELL_STAY_OF_EXECUTION_H_T = 93706,
    SPELL_DARK_ARCHANGEL_FORM   = 93757,
    SPELL_DARK_ARCHANGEL_FORM_0 = 93766,
    SPELL_WRACKING_PAIN         = 93720,
    SPELL_CALAMITY              = 93812,
    SPELL_CALAMITY_DMG          = 93810,
};

class boss_baron_ashbury : public CreatureScript
{
    public:
        boss_baron_ashbury() : CreatureScript("boss_baron_ashbury") { }

        struct boss_baron_ashburyAI : public BossAI
        {
            boss_baron_ashburyAI(Creature* creature) : BossAI(creature, DATA_ASHBURY) { }

            bool bArchangel, bCombo;

            void InitializeAI() OVERRIDE
            {
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
                Reset();
            }

            void Reset() OVERRIDE
            {
                _Reset();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                bArchangel = false;
                bCombo = false;

                me->GetMap()->SetWorldState(WORLDSTATE_PARDON_DENIED, 1);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                Talk(SAY_AGGRO);
                events.ScheduleEvent(EVENT_ASPHYXIATE, 30000);
                if (IsHeroic())
                    events.ScheduleEvent(EVENT_WRACKING_PAIN, 15000);
                DoZoneInCombat();
            }

            void HealReceived(Unit* healer, uint32& /*heal*/) OVERRIDE
            {
                if (healer->GetGUID() == me->GetGUID())
                    me->GetMap()->SetWorldState(WORLDSTATE_PARDON_DENIED, 0);
            }

            void KilledUnit(Unit* /*victim*/) OVERRIDE
            {
                Talk(SAY_KILL);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();

                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_WRACKING_PAIN);
                    instance->SetData(DATA_ASHBURY, DONE);
                }

                Talk(SAY_DEATH);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (IsHeroic() && !HealthAbovePct(20) && !bArchangel)
                {
                    events.Reset();
                    bArchangel = true;
                    Talk(SAY_DARK_ARCHANGEL_FORM);
                    DoCast(me, SPELL_DARK_ARCHANGEL_FORM);
                    events.ScheduleEvent(EVENT_CALAMITY, 3000);
                }

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_PAIN_AND_SUFFERING:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 80.0f, true))
                                DoCast(target, SPELL_PAIN_AND_SUFFERING);
                            break;
                        case EVENT_ASPHYXIATE:
                            bCombo = true;
                            events.DelayEvents(7000);
                            Talk(SAY_ASPHYXIATE);
                            DoCast(SPELL_ASPHYXIATE);
                            events.ScheduleEvent(EVENT_STAY_OF_EXECUTION, 6100);
                            events.ScheduleEvent(EVENT_PAIN_AND_SUFFERING, urand(8000, 9000));
                            events.ScheduleEvent(EVENT_ASPHYXIATE, 45000);
                            break;
                        case EVENT_STAY_OF_EXECUTION:
                            Talk(SAY_STAY_OF_EXECUTION);
                            DoCast(SelectTarget(SELECT_TARGET_NEAREST), SPELL_STAY_OF_EXECUTION);
                            bCombo = false;
                            break;
                        case EVENT_CALAMITY:
                            DoCast(me, SPELL_CALAMITY);
                            break;
                        case EVENT_WRACKING_PAIN:
                            DoCast(me, SPELL_WRACKING_PAIN);
                            events.ScheduleEvent(EVENT_WRACKING_PAIN, 30000);
                            break;
                    }
                }

                if (!me->HasAura(SPELL_DARK_ARCHANGEL_FORM))
                    DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<boss_baron_ashburyAI>(creature);
        }
};

// Asphyxiate 93423
class spell_ashbery_asphyxate_dmg : public SpellScriptLoader
{
    public:
        spell_ashbery_asphyxate_dmg() : SpellScriptLoader("spell_ashbery_asphyxate_dmg") { }

        class spell_ashbery_asphyxate_dmg_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ashbery_asphyxate_dmg_AuraScript);

            // leaves every target at 1 health at most
            void PeriodicTick(AuraEffect const* /*aurEff*/)
            {
                Unit* target = GetOwner()->ToUnit();
                if (!target)
                    return;

                int32 damage = int32(0.2 * target->GetMaxHealth()) < int32(target->GetHealth()) ? int32(0.2 * target->GetMaxHealth()) : target->GetHealth() <= 1 ? 0 : int32(target->GetHealth() - 1);
                target->CastCustomSpell(target, SPELL_ASPHYXIATE_DMG, &damage, NULL, NULL, true);
            }

            void Register() OVERRIDE
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_ashbery_asphyxate_dmg_AuraScript::PeriodicTick, EFFECT_2, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_ashbery_asphyxate_dmg_AuraScript();
        }
};

void AddSC_boss_baron_ashbury()
{
    new boss_baron_ashbury();
    new spell_ashbery_asphyxate_dmg();
}
