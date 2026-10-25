/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "InstanceScript.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "MoveSplineInit.h"
#include "Vehicle.h"
#include "VehicleDefines.h"
#include "Player.h"
#include "scarlet_halls.h"

enum Spells
{
    SPELL_HEROIC_LEAP_DUMMY          = 111219, // emote for script
    SPELL_HEROIC_LEAP                = 111218,
    SPELL_DRAGONS_REACH              = 111217,
    SPELL_BERSERKER_RAGE             = 111221,
    SPELL_LIGHT_BLADES_APPLY_AURA    = 111216, // only visual bladestorm
    SPELL_LIGHT_BLADES               = 111215,
    SPELL_LIGHT_BLADES_AURA          = 112955,
    SPELL_LIGHT_BLADES_TRIGGERED     = 111394,
    SPELL_HEAVY_ARMOR                = 113959,
    SPELL_UNARMORED                  = 113969,
};

enum Events
{
    EVENT_DRAGONS_REACH = 1,
    EVENT_HEROIC_LEAP,
    EVENT_LIGHT_BLADES,
    EVENT_BACK,
    EVENT_CALL_FOR_HELP,

    // Misc
    EVENT_INIT_COMBAT,
};

enum Yells
{
    TALK_INTO,
    TALK_DEATH,
    TALK_HEROIC_LEAP,
    TALK_DRAGONS_REACH,
    TALK_CALL_FOR_HELP,
};

enum Actions
{
    ACTION_LIGHT_BLADES,
};

enum Types
{
    TYPE_LIGHT_BLADES = 1,
};

class boss_armsmaster_harlan : public CreatureScript
{
    public:
        boss_armsmaster_harlan() : CreatureScript("boss_armsmaster_harlan") { }

        struct boss_armsmaster_harlanAI : public BossAI
        {
            boss_armsmaster_harlanAI(Creature* creature) : BossAI(creature, BOSS_ARMSMASTER_HARLAN) { }

            bool bers;
            uint64 targetGuid;
            uint32 m_bSeats;

            void Reset() OVERRIDE
            {
                _Reset();
                events.Reset();
                me->setRegeneratingHealth(true);
                me->SetReactState(REACT_AGGRESSIVE);

                bers       = false;
                targetGuid = 0;
                m_bSeats   = 1;
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
                Talk(TALK_DEATH);

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }

            void EnterEvadeMode() OVERRIDE
            {
                BossAI::EnterEvadeMode();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                summons.DespawnAll();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();
                Talk(TALK_INTO);

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                GetEvents();
            }

            uint32 GetData(uint32 type) const OVERRIDE
            {
                if (type == TYPE_LIGHT_BLADES)
                    return m_bSeats;

                return 0;
            }

            void SetData(uint32 type, uint32 /*data*/) OVERRIDE
            {
                if (type == TYPE_LIGHT_BLADES)
                    m_bSeats++;
            }

            void GetPassengerExitPosition(Unit* /*passenger*/, int8 /*seatId*/, Position& pos) OVERRIDE
            {
                pos.Relocate(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY(), me->GetHomePosition().GetPositionZ());
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                if (actionId == ACTION_LIGHT_BLADES)
                {
                    Movement::MoveSplineInit init(me);
                    for (Position const& point : BladeStormPoints)
                        init.Path().push_back(G3D::Vector3(point.GetPositionX(), point.GetPositionY(), point.GetPositionZ()));

                    init.SetWalk(false);
                    init.Launch();
                    events.ScheduleEvent(EVENT_BACK, me->GetSplineDuration() + 1000);
                }
            }

            void MovementInform(uint32 /*type*/, uint32 pointId) OVERRIDE
            {
                if (pointId == EVENT_JUMP)
                {
                    DoCast(me, SPELL_HEROIC_LEAP);
                    DoCast(me, SPELL_LIGHT_BLADES_APPLY_AURA);
                    events.Reset();
                }
            }

            void GetEvents()
            {
                events.ScheduleEvent(EVENT_DRAGONS_REACH, urand(5000, 8000));
                events.ScheduleEvent(EVENT_CALL_FOR_HELP, urand(10000, 11000));
                events.ScheduleEvent(EVENT_HEROIC_LEAP, 25000);
            }

            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) OVERRIDE
            {
                if (!bers && HealthBelowPct(50))
                {
                    bers = true;
                    me->CastSpell(me, SPELL_BERSERKER_RAGE, false);
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
                        case EVENT_DRAGONS_REACH:
                            if (Unit* target = me->GetVictim())
                                me->CastSpell(target, SPELL_DRAGONS_REACH, false);

                            Talk(TALK_DRAGONS_REACH);
                            events.ScheduleEvent(EVENT_DRAGONS_REACH, urand(5000, 8000));
                            break;
                        case EVENT_HEROIC_LEAP:
                            me->CastSpell(me, SPELL_HEROIC_LEAP_DUMMY, false);
                            me->GetMotionMaster()->MoveJump(JumpPoint, 42.0f, 42.0f, EVENT_JUMP);
                            Talk(TALK_HEROIC_LEAP);

                            if (Unit* victim = me->GetVictim())
                                targetGuid = victim->GetGUID();

                            DoResetThreat();
                            me->AttackStop();
                            me->SetReactState(REACT_PASSIVE);
                            break;
                        case EVENT_BACK:
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->RemoveAura(SPELL_LIGHT_BLADES_APPLY_AURA);
                            if (instance)
                            {
                                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LIGHT_BLADES);
                                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LIGHT_BLADES_AURA);
                            }
                            m_bSeats = 1;

                            if (Vehicle* passengers = me->GetVehicleKit())
                                passengers->RemoveAllPassengers();

                            if (Unit* target = ObjectAccessor::GetUnit(*me, targetGuid))
                                AttackStart(target);

                            GetEvents();
                            break;
                        case EVENT_CALL_FOR_HELP:
                            Talk(TALK_CALL_FOR_HELP);
                            for (uint32 i = 0; i < 2; i++)
                                me->SummonCreature(NPC_SCARLET_DEFENDER, 1188.38f, 444.03f + frand(2.0f, 4.0f), 11.17f, 0.07f, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);

                            events.ScheduleEvent(EVENT_CALL_FOR_HELP, urand(20000, 25000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_armsmaster_harlanAI(creature);
        }
};

// Scarlet Defender 58998
struct npc_scarlet_defender : public ScriptedAI
{
    npc_scarlet_defender(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
    {
        SetEquipmentSlots(false, 98316, 58998, 0);
        me->CastSpell(me, SPELL_HEAVY_ARMOR, false);
        me->SetReactState(REACT_PASSIVE);
        me->AttackStop();

        Position const* path = urand(0, 1) ? DefenderPath1 : DefenderPath2;
        Movement::MoveSplineInit init(me);
        for (uint8 i = 0; i < 6; ++i)
            init.Path().push_back(G3D::Vector3(path[i].GetPositionX(), path[i].GetPositionY(), path[i].GetPositionZ()));

        init.SetWalk(false);
        init.Launch();
        events.ScheduleEvent(EVENT_INIT_COMBAT, me->GetSplineDuration());
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        events.Update(diff);

        if (events.ExecuteEvent() == EVENT_INIT_COMBAT)
        {
            me->SetReactState(REACT_AGGRESSIVE);
            me->SetInCombatWithZone();
        }

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

// Heavy Armor 113959
class spell_heavy_armor : public AuraScript
{
    PrepareAuraScript(spell_heavy_armor);

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* caster = GetCaster())
            caster->CastSpell(caster, SPELL_UNARMORED, false);
    }

    void Register() OVERRIDE
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_heavy_armor::OnRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_REAL);
    }
};

// Light Blades 111216
class spell_harlan_light_blades : public SpellScript
{
    PrepareSpellScript(spell_harlan_light_blades);

    void HandleAfterCast()
    {
        if (Creature* caster = GetCaster()->ToCreature())
            if (caster->AI())
                caster->AI()->DoAction(ACTION_LIGHT_BLADES);
    }

    void Register() OVERRIDE
    {
        AfterCast += SpellCastFn(spell_harlan_light_blades::HandleAfterCast);
    }
};

// Light Blades 111394
class spell_harlan_light_blades_proc : public SpellScript
{
    PrepareSpellScript(spell_harlan_light_blades_proc);

    void HandleDummyEffect(SpellEffIndex /*effIndex*/)
    {
        Creature* caster = GetCaster()->ToCreature();
        Unit* target = GetHitUnit();
        if (!caster || !caster->AI() || !target)
            return;

        if (uint32 seat = caster->AI()->GetData(TYPE_LIGHT_BLADES))
        {
            // Vehicle Aura
            if (!target->HasAura(SPELL_LIGHT_BLADES_AURA) && seat < 8)
            {
                if (SpellInfo const* rideSpell = sSpellMgr->GetSpellInfo(VEHICLE_SPELL_RIDE_HARDCODED))
                {
                    int32 bp = rideSpell->Effects[EFFECT_0].BasePoints + int32(seat);
                    target->CastSpell(caster, SPELL_LIGHT_BLADES_AURA, true);
                    target->CastCustomSpell(caster, VEHICLE_SPELL_RIDE_HARDCODED, &bp, NULL, NULL, true);
                    caster->AI()->SetData(TYPE_LIGHT_BLADES, 0);
                }
            }

            caster->CastSpell(target, SPELL_LIGHT_BLADES, true);
        }
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_harlan_light_blades_proc::HandleDummyEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// Blades of Light 111215
class spell_harlan_light_blades_eff : public SpellScript
{
    PrepareSpellScript(spell_harlan_light_blades_eff);

    void HandleEffectHit(SpellEffIndex /*effIndex*/)
    {
        uint32 damage = GetHitDamage();

        if (Player* target = GetHitPlayer())
        {
            // A killing blow while riding the vehicle seat leaves the player stuck in it
            if (damage >= target->GetHealth() && target->HasAura(SPELL_LIGHT_BLADES_AURA))
            {
                SetHitDamage(0);
                target->RemoveAurasDueToSpell(SPELL_LIGHT_BLADES_AURA);
                target->_ExitVehicle();

                uint64 targetGuid = target->GetGUID();
                uint64 harlanGuid = target->GetInstanceScript() ? target->GetInstanceScript()->GetData64(BOSS_ARMSMASTER_HARLAN) : 0;
                target->m_Events.Schedule(200, [target, targetGuid, harlanGuid]()
                {
                    if (Creature* harlan = ObjectAccessor::GetCreature(*target, harlanGuid))
                        if (Player* player = ObjectAccessor::GetPlayer(*harlan, targetGuid))
                            harlan->CastSpell(player, SPELL_LIGHT_BLADES, true);
                });
            }
        }
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_harlan_light_blades_eff::HandleEffectHit, EFFECT_0, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE);
    }
};

void AddSC_boss_armsmaster_harlan()
{
    new boss_armsmaster_harlan();
    RegisterCreatureAI(npc_scarlet_defender);

    RegisterAuraScript(spell_heavy_armor);
    RegisterSpellScript(spell_harlan_light_blades);
    RegisterSpellScript(spell_harlan_light_blades_proc);
    RegisterSpellScript(spell_harlan_light_blades_eff);
}
