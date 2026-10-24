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
    SAY_AGGRO = 0,
    SAY_DEATH = 1,
    SAY_KILL  = 2,
};

enum Spells
{
    SPELL_ICE_SHARDS                    = 93527,
    SPELL_CONJURE_POISONOUS_MIXTURE     = 93697,
    SPELL_CONJURE_FROST_MIXTURE         = 93505,

    SPELL_CONJURE_MYSTERY_TOXIN_1       = 93695,
    SPELL_CONJURE_MYSTERY_TOXIN_2       = 93563,

    SPELL_CONJURE_MYSTERY_PERIODIC      = 93562,

    SPELL_TOXIC_COAGULANT_AURA          = 93572,
    SPELL_TOXIC_COAGULANT_DMG           = 93617,

    SPELL_TOXIC_CATALYST_AURA           = 93573,
    SPELL_TOXIC_CATALYST_DMG            = 93689,
};

enum Events
{
    EVENT_ICE_SHARDS                = 1,
    EVENT_CONJURE_MYSTERY_TOXIN     = 2,
    EVENT_CONJURE_POISONOUS_MIXTURE = 3,
    EVENT_CONJURE_FROST_MIXTURE     = 4,
};

enum Adds
{
    NPC_MYSTERY_TOXIN_1 = 50522,
    NPC_MYSTERY_TOXIN_2 = 50439,
};

class boss_lord_valden : public CreatureScript
{
    public:
        boss_lord_valden() : CreatureScript("boss_lord_valden") { }

        struct boss_lord_valdenAI : public BossAI
        {
            boss_lord_valdenAI(Creature* creature) : BossAI(creature, DATA_VALDEN) { }

            bool bRed;

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

                me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING);
                bRed = false;
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                if (IsHeroic())
                    events.ScheduleEvent(EVENT_CONJURE_MYSTERY_TOXIN, 15000);

                events.ScheduleEvent(EVENT_ICE_SHARDS, 30000);
                events.ScheduleEvent(EVENT_CONJURE_POISONOUS_MIXTURE, 6000);
                events.ScheduleEvent(EVENT_CONJURE_FROST_MIXTURE, 7500);
                Talk(SAY_AGGRO);
                me->HandleEmoteCommand(EMOTE_STATE_NONE);
                DoZoneInCombat();
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();

                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    instance->SetData(DATA_VALDEN, DONE);
                }

                Talk(SAY_DEATH);
            }

            void KilledUnit(Unit* /*victim*/) OVERRIDE
            {
                Talk(SAY_KILL);
            }

            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) OVERRIDE
            {
                if (me->HealthBelowPct(30) && !bRed && IsHeroic())
                {
                    bRed = true;
                    DoCast(me, SPELL_CONJURE_MYSTERY_TOXIN_2);
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ICE_SHARDS:
                            DoCast(me, SPELL_ICE_SHARDS);
                            events.ScheduleEvent(EVENT_ICE_SHARDS, urand(20000, 25000));
                            break;
                        case EVENT_CONJURE_POISONOUS_MIXTURE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, NonTankTargetSelector(me)))
                                DoCast(target, SPELL_CONJURE_POISONOUS_MIXTURE);
                            else if (Unit* victim = me->GetVictim())
                                DoCast(victim, SPELL_CONJURE_POISONOUS_MIXTURE);

                            events.ScheduleEvent(EVENT_CONJURE_POISONOUS_MIXTURE, urand(8000, 14000));
                            break;
                        case EVENT_CONJURE_FROST_MIXTURE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, NonTankTargetSelector(me)))
                                DoCast(target, SPELL_CONJURE_FROST_MIXTURE);
                            else if (Unit* victim = me->GetVictim())
                                DoCast(victim, SPELL_CONJURE_FROST_MIXTURE);

                            events.ScheduleEvent(EVENT_CONJURE_FROST_MIXTURE, urand(8000, 14000));
                            break;
                        case EVENT_CONJURE_MYSTERY_TOXIN:
                            DoCast(me, SPELL_CONJURE_MYSTERY_TOXIN_1);
                            events.ScheduleEvent(EVENT_CONJURE_MYSTERY_TOXIN, 25000);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<boss_lord_valdenAI>(creature);
        }
};

class npc_valden_mystery_toxin : public CreatureScript
{
    public:
        npc_valden_mystery_toxin() : CreatureScript("npc_valden_mystery_toxin") { }

        struct npc_valden_mystery_toxinAI : public ScriptedAI
        {
            npc_valden_mystery_toxinAI(Creature* creature) : ScriptedAI(creature)
            {
                SetCombatMovement(false);
            }

            void Reset() OVERRIDE
            {
                switch (me->GetEntry())
                {
                    case NPC_MYSTERY_TOXIN_1:
                        DoCast(me, SPELL_TOXIC_COAGULANT_AURA);
                        break;
                    case NPC_MYSTERY_TOXIN_2:
                        DoCast(me, SPELL_TOXIC_CATALYST_DMG);
                        DoCast(me, SPELL_TOXIC_CATALYST_AURA);
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<npc_valden_mystery_toxinAI>(creature);
        }
};

class spell_valden_toxic_coagulant : public SpellScriptLoader
{
    public:
        spell_valden_toxic_coagulant() : SpellScriptLoader("spell_valden_toxic_coagulant") { }

        class spell_valden_toxic_coagulant_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_valden_toxic_coagulant_AuraScript);

            // moving sheds a stack
            void HandleTick(AuraEffect const* /*aurEff*/)
            {
                if (Unit* target = GetTarget())
                    if (target->isMoving())
                        target->RemoveAuraFromStack(SPELL_TOXIC_COAGULANT_DMG);
            }

            void Register() OVERRIDE
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_valden_toxic_coagulant_AuraScript::HandleTick, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_valden_toxic_coagulant_AuraScript();
        }
};

void AddSC_boss_lord_valden()
{
    new boss_lord_valden();
    new npc_valden_mystery_toxin();
    new spell_valden_toxic_coagulant();
}
