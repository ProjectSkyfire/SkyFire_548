/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "shadowfang_keep.h"

enum ScriptTexts
{
    SAY_AGGRO = 0,
    SAY_DEATH = 1,
    SAY_KILL  = 2,
};

enum Events
{
    EVENT_VEIL_OF_SHADOW    = 1,
    EVENT_CURSED_VEIL_H     = 3,
};

enum Spells
{
    SPELL_VEIL_OF_SHADOW        = 23224,
    SPELL_CURSED_VEIL_H         = 93956,
    SPELL_SUMMON_WORGEN_SPIRIT  = 93857,

    SPELL_NANDOS_T              = 93899,
    SPELL_ODO_T                 = 93864,
    SPELL_RETHILGORE_T          = 93927,
    SPELL_RAZORCLAW_T           = 93924,
};

enum Adds
{
    NPC_RETHILGORE_DUMMY = 51085,
    NPC_RAZORCLAW_DUMMY  = 51080,
    NPC_ODO_DUMMY        = 50934,
    NPC_NANDOS_DUMMY     = 51047,
};

class boss_baron_silverlaine : public CreatureScript
{
    public:
        boss_baron_silverlaine() : CreatureScript("boss_baron_silverlaine") { }

        struct boss_baron_silverlaineAI : public BossAI
        {
            boss_baron_silverlaineAI(Creature* creature) : BossAI(creature, DATA_SILVERLAINE) { }

            uint8 phase;

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

                phase = 0;
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                Talk(SAY_AGGRO);
                events.ScheduleEvent(EVENT_VEIL_OF_SHADOW, 12000);
                DoZoneInCombat();
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
                    instance->SetData(DATA_SILVERLAINE, DONE);
                }

                Talk(SAY_DEATH);
            }

            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) OVERRIDE
            {
                if (HealthBelowPct(70) && phase == 0)
                {
                    phase = 1;
                    DoCast(SPELL_SUMMON_WORGEN_SPIRIT);
                    return;
                }
                if (HealthBelowPct(35) && phase == 1)
                {
                    phase = 2;
                    DoCast(SPELL_SUMMON_WORGEN_SPIRIT);
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
                    if (eventId == EVENT_VEIL_OF_SHADOW)
                    {
                        if (Unit* target = me->GetVictim())
                            DoCast(target, SPELL_VEIL_OF_SHADOW);

                        events.ScheduleEvent(EVENT_VEIL_OF_SHADOW, 12000);
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<boss_baron_silverlaineAI>(creature);
        }
};

// Silverlaine Worgens 50857, 50851, 50834, 50869
class npc_silverlaine_worgen : public CreatureScript
{
    public:
        npc_silverlaine_worgen() : CreatureScript("npc_silverlaine_worgen") { }

        enum iEvents
        {
            EVENT_SPECIAL_1 = 1,
            EVENT_SPECIAL_2 = 2,
        };

        enum iSpells
        {
            // Odo
            SPELL_HOWLING_RAGE         = 93931,
            SPELL_BLINDING_SHADOWS     = 93952,

            // Nandos
            SPELL_SUMMON_LUPINE_SPIRIT = 94199,
            SPELL_CLAW                 = 93861,

            // Rethilgore
            SPELL_SOUL_DRAIN           = 93863,

            // Razorclaw
            SPELL_RAVAGE               = 93914,
        };

        struct npc_silverlaine_worgenAI : public ScriptedAI
        {
            npc_silverlaine_worgenAI(Creature* creature) : ScriptedAI(creature), spell_type(0), summons(me) { }

            EventMap events;
            uint32 spell_type;
            SummonList summons;

            void Reset() OVERRIDE
            {
                events.Reset();
                summons.DespawnAll();
            }

            void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
            {
                if (InstanceScript* instance = me->GetInstanceScript())
                    if (Creature* silverlaine = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_SILVERLAINE)))
                        if (Unit* target = silverlaine->AI()->SelectTarget(SELECT_TARGET_RANDOM))
                            AttackStart(target);

                switch (me->GetEntry())
                {
                    case NPC_ODO_BLINDWATCHER:
                        spell_type = SPELL_BLINDING_SHADOWS;
                        break;
                    case NPC_WOLF_MASTER_NANDOS:
                        spell_type = SPELL_CLAW;
                        break;
                    case NPC_RETHILGORE:
                        spell_type = SPELL_SOUL_DRAIN;
                        break;
                    case NPC_RAZORCLAW_BUTHER:
                        spell_type = SPELL_RAVAGE;
                        break;
                }
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_SPECIAL_1, urand(5000, 8500));

                switch (me->GetEntry())
                {
                    case NPC_ODO_BLINDWATCHER:
                        events.ScheduleEvent(EVENT_SPECIAL_2, 12 * IN_MILLISECONDS);
                        break;
                    case NPC_WOLF_MASTER_NANDOS:
                        for (uint8 i = 0; i < 3; ++i)
                            DoCast(me, SPELL_SUMMON_LUPINE_SPIRIT);
                        break;
                }
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                summons.Summon(summon);
                DoZoneInCombat();
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
                        case EVENT_SPECIAL_1:
                            if (spell_type)
                            {
                                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, NonTankTargetSelector(me)))
                                    me->CastSpell(target, spell_type, false);
                                else if (Unit* victim = me->GetVictim())
                                    me->CastSpell(victim, spell_type, false);
                            }

                            events.ScheduleEvent(EVENT_SPECIAL_1, urand(9000, 14800));
                            break;
                        case EVENT_SPECIAL_2:
                            DoCast(me, SPELL_HOWLING_RAGE);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<npc_silverlaine_worgenAI>(creature);
        }
};

class npc_silverlaine_worgen_spirit : public CreatureScript
{
    public:
        npc_silverlaine_worgen_spirit() : CreatureScript("npc_silverlaine_worgen_spirit") { }

        struct npc_silverlaine_worgen_spiritAI : public ScriptedAI
        {
            npc_silverlaine_worgen_spiritAI(Creature* creature) : ScriptedAI(creature), m_spell(0), instance(NULL) { }

            uint32 m_spell;
            InstanceScript* instance;

            void InitializeAI() OVERRIDE
            {
                instance = me->GetInstanceScript();
                SetCombatMovement(false);
            }

            void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
            {
                switch (me->GetEntry())
                {
                    case NPC_ODO_DUMMY:
                        m_spell = SPELL_ODO_T;
                        break;
                    case NPC_RETHILGORE_DUMMY:
                        m_spell = SPELL_RETHILGORE_T;
                        break;
                    case NPC_NANDOS_DUMMY:
                        m_spell = SPELL_NANDOS_T;
                        break;
                    case NPC_RAZORCLAW_DUMMY:
                        m_spell = SPELL_RAZORCLAW_T;
                        break;
                }

                if (instance && m_spell)
                    if (Creature* silverlaine = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_SILVERLAINE)))
                        silverlaine->CastSpell(me, m_spell, false);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<npc_silverlaine_worgen_spiritAI>(creature);
        }
};

class spell_silverlaine_summon_worgen_spirit : public SpellScriptLoader
{
    public:
        spell_silverlaine_summon_worgen_spirit() : SpellScriptLoader("spell_silverlaine_summon_worgen_spirit") { }

        class spell_silverlaine_summon_worgen_spirit_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_silverlaine_summon_worgen_spirit_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (!caster)
                    return;

                static uint32 const spiritSpells[4] = { 93925, 93921, 93859, 93896 };
                caster->CastSpell(caster, spiritSpells[urand(0, 3)], true);
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_silverlaine_summon_worgen_spirit_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_silverlaine_summon_worgen_spirit_SpellScript();
        }
};

void AddSC_boss_baron_silverlaine()
{
    new boss_baron_silverlaine();
    new npc_silverlaine_worgen();
    new npc_silverlaine_worgen_spirit();
    new spell_silverlaine_summon_worgen_spirit();
}
