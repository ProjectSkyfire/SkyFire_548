/*
* This file is part of the Pandaria 5.4.8 Project. See THANKS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "lost_city_of_the_tolvir.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "InstanceScript.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Player.h"
#include "ObjectAccessor.h"
#include "GridNotifiers.h"
#include "AchievementMgr.h"

enum eSpells
{
    SPELL_ABSORB_STORMS              = 83151,
    SPELL_CALL_OF_SKY                = 84956,
    SPELL_DEFLECTING_WINDS           = 84589,
    SPELL_STORM_BOLT_PHASE_DW        = 73564,
    SPELL_STORM_BOLT_PHASE_S         = 91853,
    // Cloud Burst
    SPELL_CLOUD_BURST_SUMMON         = 83790,
    SPELL_CLOUD_BURST                = 83051,
    // Static Shock
    SPELL_STATIC_SHOCK_1             = 84546,
    SPELL_STATIC_SHOCK_2             = 84555,
    SPELL_STATIC_SHOCK_3             = 84556,
    // Wailing Winds
    SPELL_WAILING_WINDS              = 83094,
    SPELL_WAILING_WINDS_AURA         = 83066,
    // Servant of Siamat
    SPELL_THUNDER_CRASH              = 84521,
    SPELL_LIGHTNING_NOVA             = 84544,
    // Siamat Minion
    SPELL_CHAIN_LIGHTNING            = 83455,
    SPELL_TEMPEST_STORM_SUMMON       = 83414,
    SPELL_TEMPEST_STORM_TRANSFORM    = 83170,
    SPELL_LIGHTNING_CHARGE           = 91872,
    SPELL_LIGHTNING_CHARGE_AURA      = 93959,

};

enum eCreatures
{
    NPC_TEMPEST_STORM                = 44713,
    NPC_SERVANT_OF_SIAMAT_1          = 45259,
    NPC_SERVANT_OF_SIAMAT_2          = 45268,
    NPC_SETVANT_OF_SIAMAT            = 45269,
};

enum eActions
{
    ACTION_SERVANT_DEATH             = 0,
};

enum eTexts
{
    SAY_AGGRO                        = 0,
    SAY_WAILING_WINDS                = 2,
    SAY_KILL_PLAYER                  = 3,
    SAY_DEATH                        = 4,
};

enum ePhases
{
    PHASE_DEFLECTING_WINDS           = 1,
    PHASE_WAILING_WINDS              = 2,
    PHASE_SIAMAT                     = 3,
};

enum eEvents
{
    // Siamat
    EVENT_STATIC_SHOCK               = 1,
    EVENT_DEFLECTING_WINDS           = 2,
    EVENT_CLOUD_BURST                = 3,
    EVENT_CALL_OF_SKY                = 4,
    EVENT_WAILING_WINDS              = 5,
    EVENT_STORM_BOLT_DW              = 6,
    EVENT_STORM_BOLT_S               = 7,
    EVENT_ABSORB_STORMS              = 8,
    // Servant of Siamat
    EVENT_THUNDER_CRASH              = 1,
    EVENT_LIGHTNING_NOVA             = 2,
    EVENT_SERVANT_DIED               = 3,
    // Siamat Minion
    EVENT_CHAIN_LIGHTNING            = 1,
    // Cloud Burst
    EVENT_PERIODIC_CAST              = 1,
};

const uint32 StaticShock[3]=
{
    SPELL_STATIC_SHOCK_1,
    SPELL_STATIC_SHOCK_2,
    SPELL_STATIC_SHOCK_3,
};

#define    FLOR_COORD_Z    36.0f

enum SiamatMisc
{
    SERVANTS_OF_SIAMAT          = 3,
    SERVANT_INTERVAL            = 45000,    // the next servant comes after this long if the one before has not died
    SERVANT_AFTER_DEATH         = 500,
    CLOUD_BURST_DELAY           = 3000
};

class boss_siamat : public CreatureScript
{
    public:
        boss_siamat() : CreatureScript("boss_siamat") { }

        struct boss_siamatAI : public BossAI
        {
            boss_siamatAI(Creature* creature) : BossAI(creature, DATA_SIAMAT)
            {
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
                me->setActive(true);
            }

            uint8 uiStaticShockId;
            uint8 uiServantsDead;

            void Reset() OVERRIDE
            {
                _Reset();
                uiServantsDead = 0;

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                uiStaticShockId = 0;
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();

                // A sequence break (the instance refuses the fight) has sent him back to evade
                if (instance && instance->GetBossState(DATA_SIAMAT) != IN_PROGRESS)
                    return;

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                Talk(SAY_AGGRO);
                events.SetPhase(PHASE_DEFLECTING_WINDS);
                events.ScheduleEvent(EVENT_STATIC_SHOCK, 2000, 0, PHASE_DEFLECTING_WINDS);
                events.ScheduleEvent(EVENT_DEFLECTING_WINDS, 10000, 0, PHASE_DEFLECTING_WINDS);
                events.ScheduleEvent(EVENT_CALL_OF_SKY, 15000);
                events.ScheduleEvent(EVENT_CLOUD_BURST, 7000, 0, PHASE_DEFLECTING_WINDS);
                events.ScheduleEvent(EVENT_STORM_BOLT_DW, 500, 0, PHASE_DEFLECTING_WINDS);
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action != ACTION_SERVANT_DEATH)
                    return;

                ++uiServantsDead;

                // The next servant does not wait for its time once the one before has died
                if (uiServantsDead < SERVANTS_OF_SIAMAT && uiStaticShockId < SERVANTS_OF_SIAMAT && uiStaticShockId == uiServantsDead)
                    events.RescheduleEvent(EVENT_STATIC_SHOCK, SERVANT_AFTER_DEATH, 0, PHASE_DEFLECTING_WINDS);

                // The winds that deflect his enemies fall with the third of his servants
                if (uiServantsDead == SERVANTS_OF_SIAMAT)
                {
                    Talk(SAY_WAILING_WINDS);
                    me->RemoveAura(SPELL_DEFLECTING_WINDS);
                    me->CastSpell(me, SPELL_WAILING_WINDS_AURA, false);
                    events.SetPhase(PHASE_WAILING_WINDS);
                    events.ScheduleEvent(EVENT_WAILING_WINDS, 1000, 0, PHASE_WAILING_WINDS);
                    events.ScheduleEvent(EVENT_STORM_BOLT_S, urand(10000, 25000), 0, PHASE_SIAMAT);
                }
            }

            void KilledUnit(Unit* victim) OVERRIDE
            {
                if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                Talk(SAY_KILL_PLAYER);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                DespawnCreatures(NPC_TEMPEST_STORM);
                DespawnCreatures(NPC_SETVANT_OF_SIAMAT);
                Talk(SAY_DEATH);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING) && !events.IsInPhase(PHASE_WAILING_WINDS))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_STATIC_SHOCK:
                            {
                                uint8 dist = urand(5, 30);
                                float angle = frand(0, M_PI);
                                float x, y;
                                me->GetNearPoint2D(x, y, (float)dist, angle);
                                me->CastSpell(x, y, FLOR_COORD_Z, StaticShock[uiStaticShockId], false);
                                ++uiStaticShockId;

                                // One servant at a time: the next one comes in 45 seconds or as soon as this one dies
                                if (uiStaticShockId < SERVANTS_OF_SIAMAT)
                                    events.ScheduleEvent(EVENT_STATIC_SHOCK, SERVANT_INTERVAL, 0, PHASE_DEFLECTING_WINDS);
                            }
                            break;
                        case EVENT_DEFLECTING_WINDS:
                            me->CastSpell(me, SPELL_DEFLECTING_WINDS, false);
                            break;
                        case EVENT_CALL_OF_SKY:
                            {
                                uint8 dist = urand(5, 30);
                                float angle = frand(0, M_PI);
                                float x, y;
                                me->GetNearPoint2D(x, y, (float)dist, angle);
                                me->CastSpell(x, y, FLOR_COORD_Z, SPELL_CALL_OF_SKY, false);
                                events.ScheduleEvent(EVENT_CALL_OF_SKY, urand(15000, 35000));
                            }
                            break;
                        case EVENT_CLOUD_BURST:
                            {
                                events.ScheduleEvent(EVENT_CLOUD_BURST, urand(10000, 25000), 0, PHASE_DEFLECTING_WINDS);

                                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                {
                                    float x, y, z;
                                    target->GetPosition(x, y, z);
                                    me->CastSpell(x, y, z, SPELL_CLOUD_BURST_SUMMON, false);
                                }
                            }
                            break;
                        case EVENT_WAILING_WINDS:
                            {
                                me->CastSpell(me, SPELL_WAILING_WINDS, false);

                                // The phase goes on for as long as he channels anything (the channel is the aura or the winds themselves)
                                if (me->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                                    events.ScheduleEvent(EVENT_WAILING_WINDS, 1000, 0, PHASE_WAILING_WINDS);
                                else
                                {
                                    events.SetPhase(PHASE_SIAMAT);
                                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                                    me->GetMotionMaster()->MoveChase(me->GetVictim());
                                    events.ScheduleEvent(EVENT_ABSORB_STORMS, 15000, 0, PHASE_SIAMAT);
                                    events.ScheduleEvent(EVENT_STORM_BOLT_S, urand(10000, 25000), 0, PHASE_SIAMAT);
                                }
                            }
                            break;
                        case EVENT_ABSORB_STORMS:
                            me->CastSpell(me, SPELL_ABSORB_STORMS, false);
                            events.ScheduleEvent(EVENT_ABSORB_STORMS, 33000, 0, PHASE_SIAMAT);
                            break;
                        case EVENT_STORM_BOLT_DW:
                            {
                                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                    me->CastSpell(target, SPELL_STORM_BOLT_PHASE_DW, false);

                                events.ScheduleEvent(EVENT_STORM_BOLT_DW, 2500, 0, PHASE_DEFLECTING_WINDS);
                            }
                            break;
                        case EVENT_STORM_BOLT_S:
                            {
                                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                {
                                    // In the second stage the bolt hits for twice what it hit for in the first
                                    if (SpellInfo const* firstStage = sSpellMgr->GetSpellInfo(SPELL_STORM_BOLT_PHASE_DW))
                                    {
                                        int32 damage = 2 * firstStage->Effects[EFFECT_0].CalcValue(me);
                                        me->CastCustomSpell(target, SPELL_STORM_BOLT_PHASE_S, &damage, NULL, NULL, false);
                                    }
                                }

                                events.ScheduleEvent(EVENT_STORM_BOLT_S, urand(10000, 25000), 0, PHASE_SIAMAT);
                            }
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

            private:
                void DespawnCreatures(uint32 entry)
                {
                    std::list<Creature*> creatures;
                    GetCreatureListWithEntryInGrid(creatures, me, entry, 1000.0f);

                    if (creatures.empty())
                       return;

                    for (std::list<Creature*>::iterator iter = creatures.begin(); iter != creatures.end(); ++iter)
                         (*iter)->DespawnOrUnsummon();
                }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_siamatAI(creature);
        }
};

class npc_servant_of_siamat : public CreatureScript
{
    public:
        npc_servant_of_siamat() : CreatureScript("npc_servant_of_siamat") { }

        struct npc_servant_of_siamatAI : public ScriptedAI
        {
            npc_servant_of_siamatAI(Creature* creature) : ScriptedAI(creature)
            {
                me->AddUnitState(UNIT_STATE_IGNORE_PATHFINDING);
                instance = creature->GetInstanceScript();
                me->SetInCombatWithZone();
            }

            EventMap events;
            InstanceScript* instance;
            bool LightningCharge;

            void Reset() OVERRIDE
            {
                events.Reset();
                LightningCharge = false;
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_THUNDER_CRASH, 1000);
                events.ScheduleEvent(EVENT_LIGHTNING_NOVA, 5000);
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                if (!IsHeroic() || !instance)
                    return;

                if (damage >= me->GetHealth())
                {
                    damage = 0;
                    me->SetHealth(1);

                    if (!LightningCharge)
                    {
                        me->CastSpell(me, SPELL_LIGHTNING_CHARGE, false, NULL, NULL, instance->GetData64(DATA_SIAMAT));

                        LightningCharge = true;
                        me->SetReactState(REACT_PASSIVE);
                        me->AttackStop();
                        events.Reset();
                        events.ScheduleEvent(EVENT_SERVANT_DIED, 2000);
                    }
                }
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                if (me->GetEntry() == NPC_SERVANT_OF_SIAMAT_1 || me->GetEntry() == NPC_SERVANT_OF_SIAMAT_2 || me->GetEntry() == NPC_SETVANT_OF_SIAMAT)
                    if (Creature* siamat = me->FindNearestCreature(BOSS_SIAMAT, 300.0f))
                        if (siamat->IsAIEnabled)
                            siamat->AI()->DoAction(ACTION_SERVANT_DEATH);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                // Once the heroic servant has stopped fighting it has no victim, but it still has to die
                if (LightningCharge)
                {
                    events.Update(diff);
                    if (events.ExecuteEvent() == EVENT_SERVANT_DIED)
                        me->Kill(me);
                    return;
                }

                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SERVANT_DIED:
                            me->Kill(me);
                            break;
                        case EVENT_THUNDER_CRASH:
                            me->CastSpell(me, SPELL_THUNDER_CRASH, false);
                            events.ScheduleEvent(EVENT_THUNDER_CRASH, urand(7000, 15000));
                            break;
                        case EVENT_LIGHTNING_NOVA:
                            me->CastSpell(me->GetVictim(), SPELL_LIGHTNING_NOVA, false);
                            events.ScheduleEvent(EVENT_LIGHTNING_NOVA, urand(7000, 15000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_servant_of_siamatAI(creature);
        }
};

class npc_siamat_minion : public CreatureScript
{
    public:
        npc_siamat_minion() : CreatureScript("npc_siamat_minion") { }

        struct npc_siamat_minionAI : public ScriptedAI
        {
            npc_siamat_minionAI(Creature* creature) : ScriptedAI(creature)
            {
                me->AddUnitState(UNIT_STATE_IGNORE_PATHFINDING);
                me->SetInCombatWithZone();
            }

            EventMap events;
            bool TempestStorm;

            void Reset() OVERRIDE
            {
                events.Reset();
                events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, 1000);
                TempestStorm = false;
                me->AddAura(84550, me);
                me->SetObjectScale(1.0f);
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                if (summon->GetEntry() == NPC_TEMPEST_STORM)
                {
                    summon->SetReactState(REACT_PASSIVE);
                    summon->SetInCombatWithZone();
                    summon->GetMotionMaster()->MoveRandom(25.0f);
                    InstanceScript* instance = me->GetInstanceScript();

                    if (instance)
                        if (Creature* siamat = Unit::GetCreature(*me, instance->GetData64(DATA_SIAMAT)))
                            if (siamat->IsAIEnabled)
                                siamat->AI()->JustSummoned(summon);
                }
            }

            void SpellHit(Unit* /*caster*/, const SpellInfo* spell) OVERRIDE
            {
                if (spell->Id == SPELL_TEMPEST_STORM_TRANSFORM)
                {
                    me->CastSpell(me, SPELL_TEMPEST_STORM_SUMMON, false);
                    me->DespawnOrUnsummon(100);
                }
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                if (damage >= me->GetHealth())
                {
                    damage = 0;
                    me->SetHealth(1);

                    if (!TempestStorm)
                    {
                        events.Reset();
                        me->RemoveAllAuras();
                        TempestStorm = true;
                        me->SetReactState(REACT_PASSIVE);
                        me->AttackStop();
                        me->StopMoving();
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        me->CastSpell(me, SPELL_TEMPEST_STORM_TRANSFORM, false);
                    }
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (events.ExecuteEvent() == EVENT_CHAIN_LIGHTNING)
                {
                    me->CastSpell(me->GetVictim(), SPELL_CHAIN_LIGHTNING, false);
                    events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, 2000);
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_siamat_minionAI(creature);
        }
};

class npc_cloud_burst : public CreatureScript
{
    public:
        npc_cloud_burst() : CreatureScript("npc_cloud_burst") { }

        struct npc_cloud_burstAI : public ScriptedAI
        {
            npc_cloud_burstAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetInCombatWithZone();
                events.ScheduleEvent(EVENT_PERIODIC_CAST, CLOUD_BURST_DELAY);
            }

            EventMap events;

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                // The cloud bursts once, three seconds after it has formed
                if (events.ExecuteEvent() == EVENT_PERIODIC_CAST)
                {
                    me->CastSpell(me, SPELL_CLOUD_BURST, false);
                    me->DespawnOrUnsummon(500);
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_cloud_burstAI(creature);
        }
};

class spell_wailing_winds : public SpellScriptLoader
{
    public:
        spell_wailing_winds() : SpellScriptLoader("spell_wailing_winds") { }

        class spell_wailing_winds_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_wailing_winds_SpellScript);

            void RandomJump(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                Unit* target = GetHitUnit();

                if (!target)
                    return;

                uint8 roll = urand(0, 1);
                float angle = frand(0, M_PI);
                float SpeedXY = frand(10.0f, 30.0f);
                float SpeedZ = frand(10.0f, 15.0f);
                float x, y;

                if (!roll)
                {
                    target->GetPosition(x, y);
                    target->SetOrientation(angle);
                    target->KnockbackFrom(x, y, SpeedXY, SpeedZ);
                }
                else
                {
                    float dist = frand(10.0f, 30.0f);
                    target->GetNearPoint2D(x, y, dist, angle);
                    target->GetMotionMaster()->MoveJump(x, y, FLOR_COORD_Z, SpeedXY, SpeedZ);
                }
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_wailing_winds_SpellScript::RandomJump, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_wailing_winds_SpellScript();
        }
};

class spell_gathered_storms : public SpellScriptLoader
{
    public:
        spell_gathered_storms() : SpellScriptLoader("spell_gathered_storms") { }

        class spell_gathered_storms_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gathered_storms_SpellScript);

            void ApplyAura(SpellEffIndex effIndex)
            {
                Unit* target = GetHitUnit();

                if (!target)
                    return;

                if (Creature* storm = target->ToCreature())
                    if (storm->GetEntry() == NPC_TEMPEST_STORM)
                    {
                        storm->RemoveAllAuras();
                        storm->DespawnOrUnsummon(2000);
                        PreventHitDefaultEffect(effIndex);
                        return;
                    }
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_gathered_storms_SpellScript::ApplyAura, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_gathered_storms_SpellScript();
        }
};

void AddSC_boss_siamat()
{
    new boss_siamat();
    new npc_servant_of_siamat();
    new npc_siamat_minion();
    new npc_cloud_burst();

    new spell_wailing_winds();
    new spell_gathered_storms();
}