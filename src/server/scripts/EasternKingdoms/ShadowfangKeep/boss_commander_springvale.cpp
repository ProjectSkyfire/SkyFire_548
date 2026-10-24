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
    SAY_ADDS  = 3,
};

enum Spells
{
    // commander springvale
    SPELL_DESECRATION               = 93687,
    SPELL_MALEFIC_STRIKE            = 93685,
    SPELL_SHIELD_OF_PERFIDIOUS      = 93693,
    SPELL_SHIELD_OF_PERFIDIOUS_DMG  = 93722,
    SPELL_UNHOLY_POWER              = 93686,
    SPELL_WORD_OF_SHAME             = 93852, // Heroic
    SPELL_SEPARATION_ANXIETY        = 96272, // Heroic
    SPELL_DESECRATION_AURA          = 93690,
    SPELL_DESECRATION_ARM           = 67803,

    // wailing guardsman
    SPELL_MORTAL_STRIKE             = 91801,
    SPELL_SCREAMS_OF_THE_PAST       = 7074,

    // tormented officer
    SPELL_FORSAKEN_ABILITY          = 7054,
    SPELL_SHIELD_WALL               = 91463,

    // both
    SPELL_UNHOLY_EMPOWERMENT        = 93844,
};

enum Events
{
    // commander springvale
    EVENT_DESECRATION           = 1,
    EVENT_MALEFIC_STRIKE        = 2,
    EVENT_SHIELD_OF_PERFIDIOUS  = 3,
    EVENT_WORD_OF_SHAME         = 4,
    EVENT_ADDS                  = 5,

    // wailing guardsman
    EVENT_MORTAL_STRIKE         = 6,
    EVENT_SCREAMS_OF_THE_PAST   = 7,

    // tormented officer
    EVENT_FORSAKEN_ABILITY      = 8,
    EVENT_SHIELD_WALL           = 9,

    // both
    EVENT_UNHOLY_EMPOWERMENT    = 10,
};

enum Adds
{
    NPC_SPRINGVALE_GUARD    = 50613,
    NPC_SPRINGVALE_OFFICER  = 50615,
    NPC_DESECRATION_BUNNY   = 50503,
};

const Position addSpawnPos[2] =
{
    { -250.85f, 2239.18f, 100.89f, 1.55f },
    { -249.71f, 2263.23f, 100.89f, 5.15f }
};

class boss_commander_springvale : public CreatureScript
{
    public:
        boss_commander_springvale() : CreatureScript("boss_commander_springvale") { }

        struct boss_commander_springvaleAI : public BossAI
        {
            boss_commander_springvaleAI(Creature* creature) : BossAI(creature, DATA_SPRINGVALE)
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
            }

            void Reset() OVERRIDE
            {
                _Reset();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                me->GetMap()->SetWorldState(WORLDSTATE_TO_THE_GROUND, 1);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                if (IsHeroic())
                    events.ScheduleEvent(EVENT_ADDS, 45000);
                events.ScheduleEvent(EVENT_MALEFIC_STRIKE, 9000);
                events.ScheduleEvent(EVENT_DESECRATION, 15000);
                Talk(SAY_AGGRO);
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
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_WORD_OF_SHAME);
                    instance->SetData(DATA_SPRINGVALE, DONE);
                }

                Talk(SAY_DEATH);
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
                        case EVENT_DESECRATION:
                            DoCast(SPELL_DESECRATION);
                            events.ScheduleEvent(EVENT_DESECRATION, 25000);
                            break;
                        case EVENT_MALEFIC_STRIKE:
                            DoCast(SPELL_MALEFIC_STRIKE);
                            events.ScheduleEvent(EVENT_MALEFIC_STRIKE, 9000);
                            break;
                        case EVENT_ADDS:
                            Talk(SAY_ADDS);
                            me->SummonCreature(NPC_SPRINGVALE_OFFICER, addSpawnPos[0], TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
                            me->SummonCreature(NPC_SPRINGVALE_GUARD, addSpawnPos[1], TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
                            events.ScheduleEvent(EVENT_ADDS, 45000);
                            break;
                        case EVENT_WORD_OF_SHAME:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(target, SPELL_WORD_OF_SHAME);
                            break;
                        case EVENT_SHIELD_OF_PERFIDIOUS:
                            DoCast(SPELL_SHIELD_OF_PERFIDIOUS);
                            break;
                    }
                }

                if (Aura* unholyPower = me->GetAura(SPELL_UNHOLY_POWER))
                {
                    if (unholyPower->GetStackAmount() >= 3)
                    {
                        me->RemoveAurasDueToSpell(SPELL_UNHOLY_POWER);

                        if (IsHeroic() && urand(0, 1))
                            events.ScheduleEvent(EVENT_WORD_OF_SHAME, 1000);
                        else
                            events.ScheduleEvent(EVENT_SHIELD_OF_PERFIDIOUS, 1000);
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<boss_commander_springvaleAI>(creature);
        }
};

class npc_springvale_wailing_guardsman : public CreatureScript
{
    public:
        npc_springvale_wailing_guardsman() : CreatureScript("npc_springvale_wailing_guardsman") { }

        struct npc_springvale_wailing_guardsmanAI : public ScriptedAI
        {
            npc_springvale_wailing_guardsmanAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
            }

            InstanceScript* instance;
            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_MORTAL_STRIKE, 3000);
                events.ScheduleEvent(EVENT_SCREAMS_OF_THE_PAST, urand(15000, 20000));
                events.ScheduleEvent(EVENT_UNHOLY_EMPOWERMENT, 15000);
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
                        case EVENT_MORTAL_STRIKE:
                            DoCastVictim(SPELL_MORTAL_STRIKE);
                            events.ScheduleEvent(EVENT_MORTAL_STRIKE, 10000);
                            break;
                        case EVENT_SCREAMS_OF_THE_PAST:
                            DoCast(SPELL_SCREAMS_OF_THE_PAST);
                            events.ScheduleEvent(EVENT_SCREAMS_OF_THE_PAST, urand(15000, 20000));
                            break;
                        case EVENT_UNHOLY_EMPOWERMENT:
                            if (Creature* springvale = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_SPRINGVALE)))
                                DoCast(springvale, SPELL_UNHOLY_EMPOWERMENT);
                            events.ScheduleEvent(EVENT_UNHOLY_EMPOWERMENT, 15000);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<npc_springvale_wailing_guardsmanAI>(creature);
        }
};

class npc_springvale_tormented_officer : public CreatureScript
{
    public:
        npc_springvale_tormented_officer() : CreatureScript("npc_springvale_tormented_officer") { }

        struct npc_springvale_tormented_officerAI : public ScriptedAI
        {
            npc_springvale_tormented_officerAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
            }

            InstanceScript* instance;
            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_UNHOLY_EMPOWERMENT, 15000);
                events.ScheduleEvent(EVENT_SHIELD_WALL, urand(20000, 25000));
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
                        case EVENT_SHIELD_WALL:
                            DoCast(SPELL_SHIELD_WALL);
                            break;
                        case EVENT_UNHOLY_EMPOWERMENT:
                            if (Creature* springvale = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_SPRINGVALE)))
                                DoCast(springvale, SPELL_UNHOLY_EMPOWERMENT);
                            events.ScheduleEvent(EVENT_UNHOLY_EMPOWERMENT, 15000);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<npc_springvale_tormented_officerAI>(creature);
        }
};

class npc_springvale_desecration_bunny : public CreatureScript
{
    public:
        npc_springvale_desecration_bunny() : CreatureScript("npc_springvale_desecration_bunny") { }

        struct npc_springvale_desecration_bunnyAI : public ScriptedAI
        {
            npc_springvale_desecration_bunnyAI(Creature* creature) : ScriptedAI(creature)
            {
                SetCombatMovement(false);
            }

            void Reset() OVERRIDE
            {
                DoCast(me, SPELL_DESECRATION_AURA);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<npc_springvale_desecration_bunnyAI>(creature);
        }
};

// Unholy Empowerment 93844
class spell_shadowfang_unholy_empowerment : public SpellScriptLoader
{
    public:
        spell_shadowfang_unholy_empowerment() : SpellScriptLoader("spell_shadowfang_unholy_empowerment") { }

        class spell_shadowfang_unholy_empowerment_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_shadowfang_unholy_empowerment_SpellScript);

            void HandleOnEffectHit(SpellEffIndex /*effIndex*/)
            {
                if (GetHitUnit() && GetHitUnit()->GetEntry() == NPC_SPRINGVALE)
                    GetCaster()->GetMap()->SetWorldState(WORLDSTATE_TO_THE_GROUND, 0);
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_shadowfang_unholy_empowerment_SpellScript::HandleOnEffectHit, EFFECT_1, SPELL_EFFECT_HEAL_PCT);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_shadowfang_unholy_empowerment_SpellScript();
        }
};

void AddSC_boss_commander_springvale()
{
    new boss_commander_springvale();
    new npc_springvale_wailing_guardsman();
    new npc_springvale_tormented_officer();
    new npc_springvale_desecration_bunny();
    new spell_shadowfang_unholy_empowerment();
}
