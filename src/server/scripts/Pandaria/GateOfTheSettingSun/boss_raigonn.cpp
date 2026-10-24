/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "MoveSplineInit.h"
#include "PassiveAI.h"
#include "Player.h"
#include "Vehicle.h"
#include "gate_of_the_setting_sun.h"

enum eSpells
{
    // Raigonn
    SPELL_IMPERVIOUS_CARAPACE       = 107118,
    SPELL_BATTERING_HEADBUTT_EMOTE  = 118685,
    SPELL_BATTERING_HEADBUTT        = 111671,
    SPELL_BATTERING_HEADBUTT_STUN   = 130772,
    SPELL_BROKEN_CARAPACE           = 111742,
    SPELL_BROKEN_CARAPACE_DAMAGE    = 107146,
    SPELL_FIXATE                    = 111723,
    SPELL_STOMP                     = 111728,

    // Protectorat
    SPELL_HIVE_MIND                 = 107314,

    // Engulfer
    SPELL_ENGULFING_WINDS           = 107274,

    // Swarm Bringer
    SPELL_SCREECHING_SWARM          = 111600,
};

enum eMisc
{
    WORLDSTATE_MANTID_SWARM         = 12252,
};

enum ePhases
{
    PHASE_WEAK_SPOT     = 1,
    PHASE_VULNERABILITY = 2,
};

enum eActions
{
    ACTION_WEAK_SPOT_DEAD   = 1,
};

enum eEvents
{
    EVENT_RAIGONN_CHARGE           = 1,
    EVENT_RAIGON_MOVE_BACK         = 2,
    EVENT_SUMMON_PROTECTORAT       = 3,
    EVENT_SUMMON_ENGULFER          = 4,
    EVENT_SUMMON_SWARM_BRINGER     = 5,
    EVENT_FIXATE                   = 6,
    EVENT_FIXATE_STOP              = 7,
    EVENT_STOMP                    = 8,
    EVENT_BATTERING_HEADBUTT_EMOTE = 9,
    EVENT_BATTERING_HEADBUTT       = 10,
    EVENT_INITIALIZE               = 11,
};

enum eTalks
{
    TALK_WEAK_POINT = 0,
    TALK_SWARM      = 1,
    TALK_ENGULFER   = 2,
    TALK_PROTECT    = 3,
};

Position const chargePos[4] =
{
    { 958.30f, 2386.92f, 297.43f, 0.0f },
    { 958.30f, 2458.59f, 300.29f, 0.0f },
    { 958.30f, 2241.68f, 296.10f, 0.0f },
    { 958.30f, 2330.15f, 296.18f, 0.0f }
};

class boss_raigonn : public CreatureScript
{
    public:
        boss_raigonn() : CreatureScript("boss_raigonn") { }

        struct boss_raigonnAI : public BossAI
        {
            boss_raigonnAI(Creature* creature) : BossAI(creature, DATA_RAIGONN), vehicle(creature->GetVehicleKit()), isInFight(false), Phase(PHASE_WEAK_SPOT) { }

            void Reset() OVERRIDE
            {
                _Reset();

                isInFight = false;
                me->SetReactState(REACT_PASSIVE);
                me->AddAura(SPELL_IMPERVIOUS_CARAPACE, me);
                me->SetFullHealth();

                Phase = PHASE_WEAK_SPOT;
                chargeEvents.Reset();
                chargeEvents.ScheduleEvent(EVENT_RAIGONN_CHARGE, 1 * IN_MILLISECONDS);
                chargeEvents.ScheduleEvent(EVENT_INITIALIZE, 1 * IN_MILLISECONDS);

                instance->SetData(DATA_RAIGONN, NOT_STARTED);
                instance->SetData(DATA_ARTILLERY_STATE, NOT_STARTED);

                me->GetMap()->SetWorldState(WORLDSTATE_MANTID_SWARM, 1);
            }

            Creature* GetWeakSpot() const
            {
                return instance->instance->GetCreature(instance->GetData64(NPC_WEAK_SPOT));
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                if (instance->GetBossState(DATA_RIMOK) != DONE)
                {
                    damage = 0;
                    return;
                }

                if (isInFight)
                    return;

                isInFight = true;
                _EnterCombat();
                Talk(TALK_WEAK_POINT);
                me->setRegeneratingHealth(false);
                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                events.ScheduleEvent(EVENT_SUMMON_PROTECTORAT, urand(15, 30) * IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_SUMMON_ENGULFER, urand(15, 30) * IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_SUMMON_SWARM_BRINGER, urand(15, 30) * IN_MILLISECONDS);

                if (Creature* weakPoint = GetWeakSpot())
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, weakPoint);
                    weakPoint->setFaction(16);
                    weakPoint->ClearUnitState(UNIT_STATE_UNATTACKABLE);
                    weakPoint->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                }

                instance->SetBossState(DATA_RAIGONN, IN_PROGRESS);
                instance->SetData(DATA_RAIGONN, IN_PROGRESS);
                instance->SetData(DATA_ARTILLERY_STATE, IN_PROGRESS);
            }

            void EnterEvadeMode() OVERRIDE
            {
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                instance->SetBossState(DATA_RAIGONN, FAIL);
                instance->SetData(DATA_RAIGONN, FAIL);

                if (Creature* weakPoint = GetWeakSpot())
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, weakPoint);

                BossAI::EnterEvadeMode();

                me->RemoveAurasDueToSpell(SPELL_BROKEN_CARAPACE);
                me->RemoveAurasDueToSpell(SPELL_BROKEN_CARAPACE_DAMAGE);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FIXATE);
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action == ACTION_WEAK_SPOT_DEAD)
                {
                    me->CastStop();
                    me->StopMoving();
                    me->SetReactState(REACT_AGGRESSIVE);

                    events.Reset();
                    chargeEvents.Reset();
                    Phase = PHASE_VULNERABILITY;
                    events.ScheduleEvent(EVENT_FIXATE, 30 * IN_MILLISECONDS);
                    events.ScheduleEvent(EVENT_STOMP, 16 * IN_MILLISECONDS);

                    me->RemoveAurasDueToSpell(SPELL_IMPERVIOUS_CARAPACE);
                    me->CastSpell(me, SPELL_BROKEN_CARAPACE, true);
                    me->CastSpell(me, SPELL_BROKEN_CARAPACE_DAMAGE, true);

                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        AttackStart(target);

                    instance->SetData(DATA_ARTILLERY_STATE, SPECIAL);
                }
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                summons.Summon(summon);
            }

            void RemoveWeakSpotPassengers()
            {
                Creature* weakPoint = GetWeakSpot();
                if (!weakPoint)
                    return;

                Vehicle* weakVehicle = weakPoint->GetVehicleKit();
                if (!weakVehicle)
                    return;

                weakVehicle->RemoveAllPassengers();

                // The client needs a moment before the headbutt lands on the weak spot
                me->m_Events.Schedule(500, [this]()
                {
                    if (Creature* wPoint = GetWeakSpot())
                        me->CastSpell(wPoint->GetPositionX(), wPoint->GetPositionY(), wPoint->GetPositionZ(), SPELL_BATTERING_HEADBUTT, true);
                });
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (uint32 eventId = chargeEvents.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_RAIGON_MOVE_BACK:
                        {
                            uint8 baseMovement = instance->GetBossState(DATA_RIMOK) != DONE ? 0 : 2;
                            Movement::MoveSplineInit init(me);
                            init.MoveTo(chargePos[baseMovement + 1].GetPositionX(), chargePos[baseMovement + 1].GetPositionY(), chargePos[baseMovement + 1].GetPositionZ());
                            init.SetVelocity(5.0f);
                            init.SetOrientationInversed();
                            init.Launch();

                            chargeEvents.ScheduleEvent(EVENT_BATTERING_HEADBUTT_EMOTE, me->GetSplineDuration());
                            chargeEvents.ScheduleEvent(EVENT_RAIGONN_CHARGE, me->GetSplineDuration() + 2);
                            break;
                        }
                        case EVENT_BATTERING_HEADBUTT_EMOTE:
                            me->CastSpell(me, SPELL_BATTERING_HEADBUTT_EMOTE, false);
                            break;
                        case EVENT_RAIGONN_CHARGE:
                        {
                            me->CastStop();
                            uint8 baseMovement = instance->GetBossState(DATA_RIMOK) != DONE ? 0 : 2;
                            Movement::MoveSplineInit init(me);
                            init.MoveTo(chargePos[baseMovement].GetPositionX(), chargePos[baseMovement].GetPositionY(), chargePos[baseMovement].GetPositionZ());
                            init.SetVelocity(37.0f);
                            init.Launch();

                            chargeEvents.ScheduleEvent(EVENT_BATTERING_HEADBUTT, me->GetSplineDuration());
                            break;
                        }
                        case EVENT_BATTERING_HEADBUTT:
                            RemoveWeakSpotPassengers();
                            chargeEvents.ScheduleEvent(EVENT_RAIGON_MOVE_BACK, 3 * IN_MILLISECONDS);
                            break;
                        case EVENT_INITIALIZE:
                            if (vehicle)
                            {
                                if (Creature* weakSpot = me->SummonCreature(NPC_WEAK_SPOT, *me))
                                {
                                    weakSpot->EnterVehicle(me, 0);
                                    instance->SetData64(NPC_WEAK_SPOT, weakSpot->GetGUID());
                                }
                            }
                            break;
                        default:
                            break;
                    }
                }

                chargeEvents.Update(diff);
                events.Update(diff);

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SUMMON_PROTECTORAT:
                            Talk(TALK_PROTECT);
                            for (uint8 i = 0; i < 8; ++i)
                                if (Creature* summon = me->SummonCreature(NPC_KRIKTHIK_PROTECTORAT, frand(941.0f, 974.0f), 2374.85f, 296.67f, 4.73f, TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000))
                                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                        if (summon->IsAIEnabled)
                                            summon->AI()->AttackStart(target);

                            events.ScheduleEvent(EVENT_SUMMON_PROTECTORAT, urand(30, 45) * IN_MILLISECONDS);
                            break;
                        case EVENT_SUMMON_ENGULFER:
                            Talk(TALK_ENGULFER);
                            for (uint8 i = 0; i < 3; ++i)
                                me->SummonCreature(NPC_KRIKTHIK_ENGULFER, frand(941.0f, 974.0f), me->GetPositionY(), me->GetPositionZ() + 30.0f, 4.73f, TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000);

                            events.ScheduleEvent(EVENT_SUMMON_ENGULFER, urand(95, 105) * IN_MILLISECONDS);
                            break;
                        case EVENT_SUMMON_SWARM_BRINGER:
                            Talk(TALK_SWARM);
                            if (Creature* summon = me->SummonCreature(NPC_KRIKTHIK_SWARM_BRINGER, frand(941.0f, 974.0f), 2374.85f, 296.67f, 4.73f, TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000))
                                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                    if (summon->IsAIEnabled)
                                        summon->AI()->AttackStart(target);

                            events.ScheduleEvent(EVENT_SUMMON_ENGULFER, urand(35, 50) * IN_MILLISECONDS);
                            break;
                        case EVENT_FIXATE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 150.0f, true))
                            {
                                DoCast(target, SPELL_FIXATE);
                                AttackStart(target);
                            }

                            me->ClearUnitState(UNIT_STATE_CASTING);
                            me->SetReactState(REACT_PASSIVE);
                            events.ScheduleEvent(EVENT_FIXATE_STOP, 15 * IN_MILLISECONDS);
                            break;
                        case EVENT_FIXATE_STOP:
                            events.Reset();
                            me->SetReactState(REACT_AGGRESSIVE);
                            events.ScheduleEvent(EVENT_FIXATE, 30 * IN_MILLISECONDS);
                            events.ScheduleEvent(EVENT_STOMP, urand(14, 20) * IN_MILLISECONDS);
                            break;
                        case EVENT_STOMP:
                            me->CastSpell(me, SPELL_STOMP, false);
                            break;
                        default:
                            break;
                    }
                }

                if (Phase == PHASE_VULNERABILITY)
                    DoMeleeAttackIfReady();
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                summons.DespawnAll();
            }

            void GetPassengerEnterPosition(Unit* /*passenger*/, int8 /*seatId*/, Position& pos) OVERRIDE
            {
                pos.Relocate(11.7774f, 0.0f, 17.862f);
            }

        private:
            Vehicle* vehicle;
            bool isInFight;
            uint8 Phase;
            EventMap chargeEvents;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_raigonnAI(creature);
        }
};

class npc_raigonn_weak_spot : public CreatureScript
{
    public:
        npc_raigonn_weak_spot() : CreatureScript("npc_raigonn_weak_spot") { }

        struct npc_raigonn_weak_spotAI : public ScriptedAI
        {
            npc_raigonn_weak_spotAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
            }

            void Reset() OVERRIDE
            {
                me->SetReactState(REACT_PASSIVE);
                me->setRegeneratingHealth(false);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    if (Creature* raigonn = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_RAIGONN)))
                        if (raigonn->IsAIEnabled)
                            raigonn->AI()->DoAction(ACTION_WEAK_SPOT_DEAD);
                }
            }

            void GetPassengerEnterPosition(Unit* passenger, int8 /*seatId*/, Position& /*pos*/) OVERRIDE
            {
                passenger->SetDisableGravity(true);
            }

            void GetPassengerExitPosition(Unit* passenger, int8 /*seatId*/, Position& /*pos*/) OVERRIDE
            {
                passenger->SetDisableGravity(false);
            }

        private:
            InstanceScript* instance;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_raigonn_weak_spotAI(creature);
        }
};

class npc_krikthik_protectorat : public CreatureScript
{
    public:
        npc_krikthik_protectorat() : CreatureScript("npc_krikthik_protectorat") { }

        struct npc_krikthik_protectoratAI : public ScriptedAI
        {
            npc_krikthik_protectoratAI(Creature* creature) : ScriptedAI(creature), hasCastHiveMind(false) { }

            void Reset() OVERRIDE
            {
                DoZoneInCombat();
                hasCastHiveMind = false;
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                if (!hasCastHiveMind && me->HealthBelowPctDamaged(20, damage))
                {
                    hasCastHiveMind = true;
                    me->CastSpell(me, SPELL_HIVE_MIND, true);
                }
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                me->GetMap()->SetWorldState(WORLDSTATE_MANTID_SWARM, 0);
            }

        private:
            bool hasCastHiveMind;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_krikthik_protectoratAI(creature);
        }
};

class npc_krikthik_engulfer : public CreatureScript
{
    public:
        npc_krikthik_engulfer() : CreatureScript("npc_krikthik_engulfer") { }

        enum eEvents
        {
            EVENT_ENGULFING_WINDS = 1
        };

        struct npc_krikthik_engulferAI : public ScriptedAI
        {
            npc_krikthik_engulferAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                me->SetReactState(REACT_PASSIVE);
                me->GetMotionMaster()->MoveRandom(25.0f);
                DoZoneInCombat();
                events.ScheduleEvent(EVENT_ENGULFING_WINDS, urand(7500, 12500));
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                me->GetMap()->SetWorldState(WORLDSTATE_MANTID_SWARM, 0);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (events.ExecuteEvent() == EVENT_ENGULFING_WINDS)
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 70.0f, true))
                        me->CastSpell(target, SPELL_ENGULFING_WINDS, false);
                    events.ScheduleEvent(EVENT_ENGULFING_WINDS, urand(7500, 12500));
                }
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_krikthik_engulferAI(creature);
        }
};

class npc_krikthik_swarm_bringer : public CreatureScript
{
    public:
        npc_krikthik_swarm_bringer() : CreatureScript("npc_krikthik_swarm_bringer") { }

        enum eEvents
        {
            EVENT_SCREECHING_SWARM = 1
        };

        struct npc_krikthik_swarm_bringerAI : public ScriptedAI
        {
            npc_krikthik_swarm_bringerAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                DoZoneInCombat();
                events.ScheduleEvent(EVENT_SCREECHING_SWARM, urand(17500, 22500));
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                me->GetMap()->SetWorldState(WORLDSTATE_MANTID_SWARM, 0);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (events.ExecuteEvent() == EVENT_SCREECHING_SWARM)
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 70.0f, true))
                        me->CastSpell(target, SPELL_SCREECHING_SWARM, false);
                    events.ScheduleEvent(EVENT_SCREECHING_SWARM, urand(17500, 22500));
                }

                DoMeleeAttackIfReady();
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_krikthik_swarm_bringerAI(creature);
        }
};

class vehicle_artillery : public CreatureScript
{
    public:
        vehicle_artillery() : CreatureScript("vehicle_artillery") { }

        struct vehicle_artilleryAI : public PassiveAI
        {
            vehicle_artilleryAI(Creature* creature) : PassiveAI(creature)
            {
                instance = creature->GetInstanceScript();
            }

            void OnSpellClick(Unit* /*clicker*/, bool& /*result*/) OVERRIDE
            {
                if (!instance)
                    return;

                Creature* artillery = me;
                InstanceScript* script = instance;
                me->m_Events.Schedule(1500, [artillery, script]()
                {
                    Creature* weakSpot = ObjectAccessor::GetCreature(*artillery, script->GetData64(NPC_WEAK_SPOT));
                    if (!weakSpot || !weakSpot->GetVehicleKit() || !artillery->GetVehicleKit())
                        return;

                    if (Unit* passenger = artillery->GetVehicleKit()->GetPassenger(0))
                    {
                        passenger->ExitVehicle();

                        if (weakSpot->GetVehicleKit()->GetAvailableSeatCount())
                            passenger->EnterVehicle(weakSpot);
                    }
                });
            }

        private:
            InstanceScript* instance;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new vehicle_artilleryAI(creature);
        }
};

class npc_engulfing_winds : public CreatureScript
{
    public:
        npc_engulfing_winds() : CreatureScript("npc_engulfing_winds") { }

        enum eEvents
        {
            EVENT_RANDOM_MOVEMENT = 1
        };

        enum eSpells
        {
            SPELL_ENGULFING_WINDS_AURA = 107278
        };

        struct npc_engulfing_windsAI : public ScriptedAI
        {
            npc_engulfing_windsAI(Creature* creature) : ScriptedAI(creature) { }

            void InitializeAI() OVERRIDE
            {
                me->setActive(true);
                me->SetReactState(REACT_PASSIVE);
                me->CastSpell(me, SPELL_ENGULFING_WINDS_AURA, false);
                events.ScheduleEvent(EVENT_RANDOM_MOVEMENT, 2 * IN_MILLISECONDS);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (events.ExecuteEvent() == EVENT_RANDOM_MOVEMENT)
                {
                    me->GetMotionMaster()->MoveRandom(frand(13.f, 18.f));
                    events.ScheduleEvent(EVENT_RANDOM_MOVEMENT, urand(2, 4) * IN_MILLISECONDS);
                }
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_engulfing_windsAI(creature);
        }
};

class StompTargetSelector
{
    public:
        StompTargetSelector() { }

        bool operator()(WorldObject* object)
        {
            if (Creature* cre = object->ToCreature())
                if (cre->GetEntry() == 59820 || cre->GetEntry() == 58844 || cre->GetEntry() == 58824)
                    return false;

            return true;
        }
};

class spell_raigonn_stomp : public SpellScriptLoader
{
    public:
        spell_raigonn_stomp() : SpellScriptLoader("spell_raigonn_stomp") { }

        class spell_raigonn_stomp_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_raigonn_stomp_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targetList)
            {
                targetList.remove_if(StompTargetSelector());
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_raigonn_stomp_SpellScript::FilterTargets, EFFECT_2, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_raigonn_stomp_SpellScript();
        }
};

class BatteringHeadbuttEffectTargetSelector
{
    public:
        BatteringHeadbuttEffectTargetSelector() { }

        bool operator()(Player* object)
        {
            return object
                && object->GetPositionY() > 2372.0f && object->GetPositionY() < 2360.0f
                && object->GetPositionX() > 997.0f  && object->GetPositionX() < 919.0f;
        }
};

// Battering Headbutt 111671
class spell_raigonn_battering_headbutt : public SpellScriptLoader
{
    public:
        spell_raigonn_battering_headbutt() : SpellScriptLoader("spell_raigonn_battering_headbutt") { }

        class spell_raigonn_battering_headbutt_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_raigonn_battering_headbutt_SpellScript);

            void HandleSpellEffectHit(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    std::list<Player*> playersOnGates;
                    GetPlayerListInGrid(playersOnGates, caster, 100.0f);

                    playersOnGates.remove_if(BatteringHeadbuttEffectTargetSelector());

                    for (Player* player : playersOnGates)
                        if (!player->HasUnitMovementFlag(MOVEMENTFLAG_FALLING))
                            player->AddAura(SPELL_BATTERING_HEADBUTT_STUN, player);
                }
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_raigonn_battering_headbutt_SpellScript::HandleSpellEffectHit, EFFECT_2, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_raigonn_battering_headbutt_SpellScript();
        }
};

void AddSC_boss_raigonn()
{
    new boss_raigonn();
    new npc_raigonn_weak_spot();
    new npc_krikthik_protectorat();
    new npc_krikthik_engulfer();
    new npc_krikthik_swarm_bringer();
    new vehicle_artillery();
    new npc_engulfing_winds();
    new spell_raigonn_stomp();
    new spell_raigonn_battering_headbutt();
}
