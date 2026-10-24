/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "InstanceScript.h"
#include "SpellScript.h"
#include "deadmines.h"

enum ScriptedTexts
{
    SAY_AGGRO   = 0,
    SAY_DEATH   = 1,
    SAY_KILL    = 2,
    SAY_ADDS    = 3,
    SAY_BOMB_1  = 4,
    SAY_BOMB_2  = 5,
    SAY_INTRO_1 = 8,
    SAY_INTRO_2 = 9,
    SAY_INTRO_3 = 10,
    SAY_INTRO_4 = 11,
    SAY_INTRO_5 = 12,
};

enum Spells
{
    SPELL_DEADLY_BLADES         = 92622,
    SPELL_BACKSLASH_TARGETING   = 92620,
    SPELL_BACKSLASH             = 92619,
    SPELL_DEFLECTION            = 92614,
    SPELL_VENGEANCE_OF_VANCLEEF = 95542,
    SPELL_POWDER_EXPLOSION      = 96283,
    SPELL_SITTING               = 89279,
    SPELL_NOXIOUS_CONCOCTION    = 92100,
    SPELL_VANESSA_ACHIEVEMENT   = 95654,
    SPELL_SUMMON_ENFORCER       = 92616,
    SPELL_SUMMON_SHADOWGUARD    = 92618,
    SPELL_SUMMON_BLOOD_WIZARD   = 92617,
};

enum Adds
{
    NPC_DEFIAS_ENFORCER             = 49850,
    NPC_DEFIAS_SHADOWGUARD          = 49852,
    NPC_DEFIAS_BLOOD_WIZARD         = 49854,
};

enum Events
{
    EVENT_INTRO_1       = 1,
    EVENT_INTRO_2       = 2,
    EVENT_INTRO_3       = 3,
    EVENT_INTRO_4       = 4,
    EVENT_INTRO_5       = 5,
    EVENT_INTRO_6       = 6,
    EVENT_INTRO_7       = 7,
    EVENT_ADDS_1        = 8,
    EVENT_ADDS_2        = 9,
    EVENT_ADDS_3        = 10,
    EVENT_BACKSLASH     = 11,
    EVENT_DEFLECTION    = 12,
    EVENT_CONTINUE      = 13,
};

#define ACHIEVEMENT_EVENT 27527

const Position addsPos[3] =
{
    {-67.115807f, -829.708740f, 40.959011f, 1.344201f},
    {-69.161903f, -814.125977f, 40.722912f, 1.611237f},
    {-57.057121f, -816.067200f, 41.852272f, 5.565719f}
};

class boss_vanessa_vancleef : public CreatureScript
{
    public:
        boss_vanessa_vancleef() : CreatureScript("boss_vanessa_vancleef") { }

        struct boss_vanessa_vancleefAI : public BossAI
        {
            boss_vanessa_vancleefAI(Creature* creature) : BossAI(creature, DATA_VANESSA)
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
                me->SetReactState(REACT_DEFENSIVE);
                phase = 0;
            }

            void Reset() OVERRIDE
            {
                me->SetReactState(REACT_DEFENSIVE);
                _Reset();

                // The poison phase is not scripted, so the achievement stays available
                me->GetMap()->SetWorldState(WORLDSTATE_VANCLEEF_VINDICATION, 1);

                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    me->SetVisible(instance->GetData(DATA_VANESSA_EVENT) == DONE);
                }

                phase = 0;
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                me->GetMap()->SetWorldState(WORLDSTATE_VANCLEEF_VINDICATION, 1);

                _EnterCombat();

                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    instance->DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_VANESSA_ACHIEVEMENT);
                }

                Talk(SAY_AGGRO);

                events.ScheduleEvent(EVENT_BACKSLASH, urand(10000, 15000));

                DoZoneInCombat();
            }

            void KilledUnit(Unit* victim) OVERRIDE
            {
                if (victim && victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(SAY_KILL);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                Talk(SAY_DEATH);

                _JustDied();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (me->HealthBelowPct(50) && phase == 0)
                {
                    phase = 1;
                    Talk(SAY_ADDS);
                    events.ScheduleEvent(EVENT_ADDS_1, 1000);
                    return;
                }

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BACKSLASH:
                            DoCast(me, SPELL_DEADLY_BLADES);
                            events.ScheduleEvent(EVENT_CONTINUE, 6000);
                            break;
                        case EVENT_CONTINUE:
                            if (Unit* victim = me->GetVictim())
                                me->GetMotionMaster()->MoveChase(victim);
                            break;
                        case EVENT_ADDS_1:
                            me->SummonCreature(NPC_DEFIAS_ENFORCER, addsPos[0]);
                            events.ScheduleEvent(EVENT_ADDS_2, 5000);
                            break;
                        case EVENT_ADDS_2:
                            me->SummonCreature(NPC_DEFIAS_SHADOWGUARD, addsPos[1]);
                            events.ScheduleEvent(EVENT_ADDS_3, 5000);
                            break;
                        case EVENT_ADDS_3:
                            me->SummonCreature(NPC_DEFIAS_BLOOD_WIZARD, addsPos[2]);
                            events.ScheduleEvent(EVENT_ADDS_1, 60000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            uint8 phase;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<boss_vanessa_vancleefAI>(creature);
        }
};

class npc_vanessa_vancleef_a_note_from_vanessa : public CreatureScript
{
    public:
        npc_vanessa_vancleef_a_note_from_vanessa() : CreatureScript("npc_vanessa_vancleef_a_note_from_vanessa") { }

        bool OnGossipHello(Player* /*player*/, Creature* creature) OVERRIDE
        {
            InstanceScript* instance = creature->GetInstanceScript();
            if (!instance)
                return true;

            if (instance->GetBossState(DATA_CAPTAIN) != DONE)
                return true;

            if (instance->GetData(DATA_VANESSA_EVENT) != NOT_STARTED)
                return true;

            creature->SummonCreature(NPC_VANESSA_SITTING, vanessaPos);
            instance->SetData(DATA_VANESSA_EVENT, IN_PROGRESS);

            creature->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            creature->DespawnOrUnsummon(1000);
            return true;
        }
};

class npc_vanessa_vancleef_vanessa_sitting : public CreatureScript
{
    public:
        npc_vanessa_vancleef_vanessa_sitting() : CreatureScript("npc_vanessa_vancleef_vanessa_sitting") { }

        struct npc_vanessa_vancleef_vanessa_sittingAI : public ScriptedAI
        {
            npc_vanessa_vancleef_vanessa_sittingAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                me->setActive(true);
                instance = me->GetInstanceScript();
            }

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_INTRO_1, 1000);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_INTRO_1:
                            Talk(SAY_INTRO_1);
                            events.ScheduleEvent(EVENT_INTRO_2, 5000);
                            break;
                        case EVENT_INTRO_2:
                            Talk(SAY_INTRO_2);
                            events.ScheduleEvent(EVENT_INTRO_3, 8000);
                            break;
                        case EVENT_INTRO_3:
                            Talk(SAY_INTRO_3);
                            events.ScheduleEvent(EVENT_INTRO_4, 8000);
                            break;
                        case EVENT_INTRO_4:
                            Talk(SAY_INTRO_4);
                            events.ScheduleEvent(EVENT_INTRO_5, 5000);
                            break;
                        case EVENT_INTRO_5:
                            Talk(SAY_INTRO_5);
                            DoCastAOE(SPELL_NOXIOUS_CONCOCTION);
                            me->RemoveAura(SPELL_SITTING);
                            events.ScheduleEvent(EVENT_INTRO_6, 5000);
                            break;
                        case EVENT_INTRO_6:
                            me->GetMotionMaster()->MoveJump(centershipPos.GetPositionX(), centershipPos.GetPositionY(), centershipPos.GetPositionZ(), 5.0f, 10.0f);
                            events.ScheduleEvent(EVENT_INTRO_7, 5000);
                            break;
                        case EVENT_INTRO_7:
                            if (instance)
                            {
                                instance->SetData(DATA_VANESSA_EVENT, DONE);
                                instance->DoStartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEVEMENT_EVENT);
                            }
                            if (Creature* pVanessa = me->FindNearestCreature(NPC_VANESSA_VANCLEEF, 40.0f))
                                pVanessa->SetVisible(true);
                            me->DespawnOrUnsummon(1000);
                            break;
                    }
                }
            }

        private:
            InstanceScript* instance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetInstanceAI<npc_vanessa_vancleef_vanessa_sittingAI>(creature);
        }
};

// Backslash Targeting 92620
class spell_vanessa_vancleef_backslash_targeting : public SpellScriptLoader
{
    public:
        spell_vanessa_vancleef_backslash_targeting() : SpellScriptLoader("spell_vanessa_vancleef_backslash_targeting") { }

        class spell_vanessa_vancleef_backslash_targeting_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_vanessa_vancleef_backslash_targeting_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->CastSpell(GetHitUnit(), SPELL_BACKSLASH, true);
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_vanessa_vancleef_backslash_targeting_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_vanessa_vancleef_backslash_targeting_SpellScript();
        }
};

void AddSC_boss_vanessa_vancleef()
{
    new boss_vanessa_vancleef();
    new npc_vanessa_vancleef_a_note_from_vanessa();
    new npc_vanessa_vancleef_vanessa_sitting();
    new spell_vanessa_vancleef_backslash_targeting();
}
