/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "InstanceScript.h"
#include "SpellScript.h"
#include "deadmines.h"

enum ScriptTexts
{
    SAY_DEATH   = 0,
    SAY_KILL    = 1,
    SAY_SPELL1  = 2,
    SAY_SPELL3  = 3,
    SAY_SPELL4  = 4,
    SAY_AGGRO   = 5,
};

enum Spells
{
    SPELL_THRIST_OF_BLOOD               = 88736,
    SPELL_THRIST_OF_BLOOD_AURA          = 88737,
    SPELL_SWIPE                         = 88839,
    SPELL_GO_FOR_THE_THROAT             = 88836,
    SPELL_RIPSNARLS_FOG_AURA            = 89247,
    SPELL_RIPSNARL_ACHIEVEMENT          = 95647,
    SPELL_RIPSNARL_CANCEL_ACHIEVEMENT   = 95648,

    SPELL_FOG_1                         = 88768,
    SPELL_FOG_2                         = 88755,

    SPELL_VAPOR_SUMMON                  = 88831,

    SPELL_CONDENSATION_1                = 92013, // trigger 92014 after 8 secs
    SPELL_CONDENSE_1_1                  = 92014, // trigger 92015 + 92016
    SPELL_REMOVE_CONDENSE_AURA_1        = 92015,
    SPELL_CONDENSE_1_2                  = 92016, // transform, heal, trigger 92017,
    SPELL_CONDENSATION_2                = 92017, // trigger 92018 after 8 secs
    SPELL_CONDENSE_2_1                  = 92018, // trigger 92019 + 92020
    SPELL_REMOVE_CONDENSE_AURA_2        = 92019,
    SPELL_CONDENSE_2_2                  = 92020, // transform, heal, trigger 92021,
    SPELL_CONDENSATION_3                = 92021, // trigger 92031 after 8 secs
    SPELL_CONDENSE_3_1                  = 92031, // trigger 92030 + 92029,
    SPELL_REMOVE_CONDENSE_AURA_3        = 92030,
    SPELL_CONDENSE_3_2                  = 92029, // trasform + heal, trigger 92038
    SPELL_VAPOR_ANIMUS                  = 92038,
    SPELL_COALESCE                      = 92042,

    SPELL_SWIRLING_VAPOR                = 92007,
    SPELL_CONDENSING_VAPOR              = 92008,
    SPELL_FREEZING_VAPOR                = 92011,
};

enum Adds
{
    NPC_VAPOR   = 47714,
    NPC_DUMMY_2 = 45979, // 88755
    NPC_DUMMY_1 = 47242, // 88768
};

enum Events
{
    EVENT_SWIPE             = 1,
    EVENT_CONTINUE          = 2,
    EVENT_GO_FOR_THE_THROAT = 3,
    EVENT_SUMMON_VAPOR      = 4,
    EVENT_SWIRLING_VAPOR    = 5,
    EVENT_CONDENSING_VAPOR  = 6,
    EVENT_FREEZING_VAPOR    = 7,
    EVENT_COALESCE          = 8,
};

#define ACTION_COALESCE 1

const Position dummyPos[38] =
{
    {-62.6927f, -814.031f, 41.3843f, 0.0f},
    {-67.6128f, -814.219f, 40.944f,  0.0f},
    {-68.1302f, -822.918f, 40.888f,  0.0f},
    {-62.6597f, -823.653f, 41.4015f, 0.0f},
    {-56.901f,  -818.264f, 41.954f,  0.0f},
    {-72.9931f, -825.99f,  40.4834f, 0.0f},
    {-68.934f,  -829.365f, 40.8772f, 0.0f},
    {-60.9792f, -835.979f, 41.5982f, 0.0f},
    {-65.401f,  -833.352f, 41.2017f, 0.0f},
    {-56.9514f, -832.986f, 41.9731f, 0.0f},
    {-53.2882f, -810.453f, 42.2844f, 0.0f},
    {-73.9705f, -819.941f, 40.3588f, 0.0f},
    {-61.9878f, -829.885f, 41.5003f, 0.0f},
    {-56.4115f, -827.823f, 42.0163f, 0.0f},
    {-73.6458f, -814.604f, 40.402f,  0.0f},
    {-58.0365f, -812.931f, 41.8374f, 0.0f},
    {-51.8299f, -829.847f, 42.455f,  0.0f},
    {-47.4358f, -831.122f, 42.8763f, 0.0f},
    {-57.3889f, -822.721f, 41.915f,  0.0f},
    {-51.8837f, -833.491f, 42.4604f, 0.0f},
    {-75.5295f, -831.818f, 40.2749f, 0.0f},
    {-82.5642f, -830.498f, 39.7044f, 0.0f},
    {-47.6163f, -808.856f, 42.8273f, 0.0f},
    {-55.0469f, -805.922f, 42.1087f, 0.0f},
    {-60.0f,    -807.571f, 41.6455f, 0.0f},
    {-66.6615f, -805.149f, 41.0334f, 0.0f},
    {-72.8733f, -808.346f, 40.4807f, 0.0f},
    {-79.3229f, -806.665f, 39.93f,   0.0f},
    {-85.6667f, -808.549f, 39.4228f, 0.0f},
    {-94.2292f, -809.929f, 38.7469f, 0.0f},
    {-101.122f, -813.936f, 38.2438f, 0.0f},
    {-104.384f, -819.276f, 38.0203f, 0.0f},
    {-101.174f, -824.677f, 38.2535f, 0.0f},
    {-99.684f,  -818.974f, 38.3542f, 0.0f},
    {-95.3628f, -828.319f, 38.6867f, 0.0f},
    {-88.1024f, -831.63f,  39.2559f, 0.0f},
    {-79.2431f, -834.884f, 39.9758f, 0.0f},
    {-70.941f,  -835.523f, 40.6859f, 0.0f}
};

class boss_admiral_ripsnarl : public CreatureScript
{
    public:
        boss_admiral_ripsnarl() : CreatureScript("boss_admiral_ripsnarl") { }

        struct boss_admiral_ripsnarlAI : public BossAI
        {
            boss_admiral_ripsnarlAI(Creature* creature) : BossAI(creature, DATA_ADMIRAL)
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
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
                me->setActive(true);
                stage = 0;
            }

            void Reset() OVERRIDE
            {
                _Reset();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                me->SetReactState(REACT_AGGRESSIVE);
                me->SetVisible(true);

                stage = 0;
                SetFog(false);
            }

            void SetFog(bool enabled)
            {
                std::list<Creature*> dummies;
                GetCreatureListWithEntryInGrid(dummies, me, NPC_DUMMY_1, 75.0f);
                for (Creature* dummy : dummies)
                {
                    if (enabled)
                        dummy->CastSpell(dummy, SPELL_FOG_1, true);
                    else
                        dummy->RemoveAurasDueToSpell(SPELL_FOG_1);
                }

                std::list<Creature*> bigDummies;
                GetCreatureListWithEntryInGrid(bigDummies, me, NPC_DUMMY_2, 75.0f);
                for (Creature* dummy : bigDummies)
                {
                    if (enabled)
                        dummy->CastSpell(dummy, SPELL_FOG_2, true);
                    else
                        dummy->RemoveAurasDueToSpell(SPELL_FOG_2);
                }
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();

                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_RIPSNARL_ACHIEVEMENT);
                    instance->DoResetAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2, ACHIEVEMENT_CRITERIA_CONDITION_UNK13, 28179);
                }

                Talk(SAY_AGGRO);
                events.ScheduleEvent(EVENT_SWIPE, urand(5000, 10000));
                if (IsHeroic())
                    events.ScheduleEvent(EVENT_GO_FOR_THE_THROAT, urand(5000, 10000));

                DoZoneInCombat();
                SetFog(false);
            }

            void KilledUnit(Unit* victim) OVERRIDE
            {
                if (victim && victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(SAY_KILL);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();

                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_RIPSNARL_ACHIEVEMENT);
                }

                Talk(SAY_DEATH);
                SetFog(false);
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action == ACTION_COALESCE)
                    DoCastAOE(SPELL_RIPSNARL_ACHIEVEMENT);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (me->HealthBelowPct(75) && stage == 0)
                {
                    stage = 1;
                    EnterSpecialPhase();
                    for (uint8 i = 0; i < 38; i++)
                        me->SummonCreature(NPC_DUMMY_1, dummyPos[i]);
                    events.ScheduleEvent(EVENT_SUMMON_VAPOR, 5000);
                    SetFog(true);
                    return;
                }
                else if (me->HealthBelowPct(50) && stage == 1)
                {
                    stage = 2;
                    EnterSpecialPhase();
                    return;
                }
                else if (me->HealthBelowPct(25) && stage == 2)
                {
                    stage = 3;
                    EnterSpecialPhase();
                    me->SummonCreature(NPC_DUMMY_2, centershipPos);
                    return;
                }
                else if (IsHeroic() && me->HealthBelowPct(10) && stage == 3)
                {
                    stage = 4;
                    for (uint8 i = 0; i < 3; i++)
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                            DoCast(target, SPELL_VAPOR_SUMMON);
                    return;
                }

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SWIPE:
                            DoCastVictim(SPELL_SWIPE);
                            events.ScheduleEvent(EVENT_SWIPE, urand(8000, 10000));
                            break;
                        case EVENT_CONTINUE:
                            me->SetVisible(true);
                            me->SetReactState(REACT_AGGRESSIVE);
                            if (IsHeroic())
                                events.ScheduleEvent(EVENT_GO_FOR_THE_THROAT, urand(5000, 10000));

                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                            {
                                AttackStart(target);
                                DoCast(target, SPELL_GO_FOR_THE_THROAT);
                            }
                            break;
                        case EVENT_GO_FOR_THE_THROAT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_GO_FOR_THE_THROAT);
                            events.ScheduleEvent(EVENT_GO_FOR_THE_THROAT, urand(10000, 20000));
                            break;
                        case EVENT_SUMMON_VAPOR:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_VAPOR_SUMMON);
                            events.ScheduleEvent(EVENT_SUMMON_VAPOR, 5000);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            uint8 stage;

            void EnterSpecialPhase()
            {
                me->AttackStop();
                me->SetReactState(REACT_PASSIVE);
                events.CancelEvent(EVENT_GO_FOR_THE_THROAT);
                me->SetVisible(false);

                DoResetThreat();

                me->RemoveAurasDueToSpell(SPELL_THRIST_OF_BLOOD_AURA);

                Talk(SAY_SPELL4);

                events.ScheduleEvent(EVENT_CONTINUE, urand(15000, 30000));
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<boss_admiral_ripsnarlAI>(creature);
        }
};

class npc_admiral_ripsnarl_vapor : public CreatureScript
{
    public:
        npc_admiral_ripsnarl_vapor() : CreatureScript("npc_admiral_ripsnarl_vapor") { }

        struct npc_admiral_ripsnarl_vaporAI : public ScriptedAI
        {
            npc_admiral_ripsnarl_vaporAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
            {
                if (IsHeroic())
                    DoCast(me, SPELL_CONDENSATION_1, true);
            }

            void SpellHit(Unit* /*caster*/, SpellInfo const* spell) OVERRIDE
            {
                if (spell->Id == SPELL_CONDENSE_1_1)
                    events.ScheduleEvent(EVENT_SWIRLING_VAPOR, urand(3000, 7000));
                else if (spell->Id == SPELL_CONDENSE_2_1)
                {
                    events.CancelEvent(EVENT_SWIRLING_VAPOR);
                    events.ScheduleEvent(EVENT_CONDENSING_VAPOR, urand(3000, 7000));
                }
                else if (spell->Id == SPELL_CONDENSE_3_1)
                {
                    events.CancelEvent(EVENT_CONDENSING_VAPOR);
                    events.ScheduleEvent(EVENT_FREEZING_VAPOR, urand(3000, 7000));
                    events.ScheduleEvent(EVENT_COALESCE, urand(3000, 10000));
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SWIRLING_VAPOR:
                            DoCastVictim(SPELL_SWIRLING_VAPOR);
                            break;
                        case EVENT_CONDENSING_VAPOR:
                            DoCastVictim(SPELL_CONDENSING_VAPOR);
                            break;
                        case EVENT_FREEZING_VAPOR:
                            DoCastVictim(SPELL_FREEZING_VAPOR);
                            break;
                        case EVENT_COALESCE:
                            DoCastAOE(SPELL_COALESCE);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<npc_admiral_ripsnarl_vaporAI>(creature);
        }
};

// Coalesce 92042
class spell_admiral_ripsnarl_coalesce : public SpellScriptLoader
{
    public:
        spell_admiral_ripsnarl_coalesce() : SpellScriptLoader("spell_admiral_ripsnarl_coalesce") { }

        class spell_admiral_ripsnarl_coalesce_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_admiral_ripsnarl_coalesce_SpellScript);

            void HandleAfterCast()
            {
                if (!GetCaster())
                    return;

                if (Creature* pAdmiral = GetCaster()->FindNearestCreature(NPC_ADMIRAL_RIPSNARL, 100.0f))
                    if (pAdmiral->AI())
                        pAdmiral->AI()->DoAction(ACTION_COALESCE);
            }

            void Register() OVERRIDE
            {
                AfterCast += SpellCastFn(spell_admiral_ripsnarl_coalesce_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_admiral_ripsnarl_coalesce_SpellScript();
        }
};

void AddSC_boss_admiral_ripsnarl()
{
    new boss_admiral_ripsnarl();
    new npc_admiral_ripsnarl_vapor();
    new spell_admiral_ripsnarl_coalesce();
}
