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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Vehicle.h"
#include "VehicleDefines.h"
#include "scarlet_halls.h"
#include "SpellScript.h"
#include "ObjectAccessor.h"
#include "MoveSplineInit.h"

static uint32 const NPC_SCARLET_DEFENDER = 58998;
const Position JumpPoint =          { 1206.25f, 444.04f,  0.98f,  0.1f  };
const Position BladeStormPoints [16] =
{
    { 1201.52f, 438.68f, 0.98f,  1.86f  },
    { 1198.41f, 445.94f, 0.98f,  1.0f   },
    { 1205.38f, 445.08f, 1.01f,  0.87f  },
    { 1213.15f, 453.57f, 3.53f,  5.95f  },
    { 1218.32f, 446.91f, 6.08f,  5.31f  },
    { 1222.85f, 438.54f, 6.08f,  5.18f  },
    { 1215.36f, 427.42f, 6.08f,  4.11f  },
    { 1200.62f, 425.51f, 7.48f,  3.12f  },
    { 1188.55f, 436.41f, 10.66f, 2.30f  },
    { 1188.44f, 449.05f, 11.01f, 1.56f  },
    { 1198.83f, 462.78f, 7.77f,  0.92f  },
    { 1218.94f, 459.59f, 6.08f,  6.12f  },
    { 1223.45f, 444.52f, 6.09f,  4.08f  },
    { 1212.56f, 431.35f, 2.93f,  4.02f  },
    { 1200.78f, 435.94f, 0.98f,  1.81f  },
    { 1206.30f, 443.73f, 0.98f,  0.95f  }
};

const Position DefenderPath1[6]=
{
    { 1187.91f, 438.71f, 11.04f, 5.12f },
    { 1195.04f, 428.00f, 8.63f,  5.29f },
    { 1211.36f, 425.51f, 6.08f,  6.13f },
    { 1225.26f, 441.73f, 6.08f,  0.97f },
    { 1219.72f, 447.40f, 6.08f,  2.33f },
    { 1205.54f, 458.19f, 1.15f,  2.50f }
};

const Position DefenderPath2[6]=
{
    { 1188.02f, 449.28f, 11.00f, 1.03f },
    { 1198.27f, 461.83f, 7.94f,  0.82f },
    { 1218.28f, 459.83f, 6.08f,  6.18f },
    { 1223.84f, 445.44f, 6.08f,  4.24f },
    { 1219.39f, 440.57f, 6.08f,  3.97f },
    { 1205.45f, 429.81f, 1.11f,  3.74f }
};


enum spells
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

enum events
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
    ACTION_FOLLOW_ME,
    ACTION_CAUGHT,
};

enum eType
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
            bool spinning;
            uint32 m_bSeats;

            void CleanupPassengers()
            {
                if (Vehicle* vehicle = me->GetVehicleKit())
                    vehicle->RemoveAllPassengers();
                if (instance)
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LIGHT_BLADES_AURA);
            }

            void PassengerBoarded(Unit* passenger, int8 /*seat*/, bool apply) OVERRIDE
            {
                if (!apply)
                    passenger->RemoveAurasDueToSpell(SPELL_LIGHT_BLADES_AURA);
            }

            void Reset() OVERRIDE
            {
                CleanupPassengers();
                _Reset();
                events.Reset();
                me->setRegeneratingHealth(true);
                me->SetReactState(REACT_AGGRESSIVE);

                bers       = false;
                targetGuid = 0;
                spinning = false;
                m_bSeats   = 1;
            }

            void JustDied(Unit* killer) OVERRIDE
            {
                CleanupPassengers();
                _JustDied();

                Talk(TALK_DEATH);

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }

            void EnterEvadeMode() OVERRIDE
            {
                CleanupPassengers();
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

            void GetPassengerExitPosition(Unit*, int8, Position& pos) OVERRIDE
            {
                pos = { me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY(), me->GetHomePosition().GetPositionZ() };
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                if (actionId == ACTION_LIGHT_BLADES)
                {
                    Movement::MoveSplineInit init(me);
                    for (auto&& itr : BladeStormPoints)
                        init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

                    init.SetWalk(false);
                    init.Launch();
                    events.ScheduleEvent(EVENT_BACK, me->GetSplineDuration() + 1000);
                }
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type == EFFECT_MOTION_TYPE && pointId == EVENT_JUMP)
                {
                    events.Reset();
                    me->CastSpell(me, SPELL_HEROIC_LEAP, true);
                    me->CastSpell(me, SPELL_LIGHT_BLADES_APPLY_AURA, true);
                }
            }

            void GetEvents()
            {
                events.ScheduleEvent(EVENT_DRAGONS_REACH, urand(5000, 8000));
                events.ScheduleEvent(EVENT_CALL_FOR_HELP, urand(10000, 11000));
                events.ScheduleEvent(EVENT_HEROIC_LEAP, 25000);
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                if (!bers && me->HealthBelowPctDamaged(50, damage))
                {
                    bers = true;
                    me->CastSpell(me, SPELL_BERSERKER_RAGE, false);
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (spinning && instance && instance->IsWipe(100.0f, me))
                {
                    EnterEvadeMode();
                    return;
                }
                if (!spinning && !UpdateVictim())
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
                            spinning = true;
                            me->CastSpell(me, SPELL_HEROIC_LEAP_DUMMY, false);
                            me->GetMotionMaster()->MoveJump(JumpPoint, 42.0f, 42.0f, EVENT_JUMP);
                            Talk(TALK_HEROIC_LEAP);

                            if (Unit* vict = me->GetVictim())
                                targetGuid = vict->GetGUID();

                            me->getThreatManager().resetAllAggro();
                            me->AttackStop();
                            me->SetReactState(REACT_PASSIVE);
                            break;
                        case EVENT_BACK:
                        {
                            spinning = false;
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->RemoveAura(SPELL_LIGHT_BLADES_APPLY_AURA);
                            if (instance)
                                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LIGHT_BLADES);
                            m_bSeats = 1;

                            CleanupPassengers();

                            Unit* target = ObjectAccessor::GetUnit(*me, targetGuid);
                            if (!target || !target->IsAlive())
                                target = me->SelectNearestTarget(100.0f);
                            if (target)
                                me->AI()->AttackStart(target);

                            GetEvents();
                            break;
                        }
                        case EVENT_CALL_FOR_HELP:
                            Talk(TALK_CALL_FOR_HELP);
                            for (uint32 i = 0; i < 2; i++)
                                me->SummonCreature(NPC_SCARLET_DEFENDER, 1188.38f, 444.03f + frand(2.0, 4.0f), 11.17f, 0.07f, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);

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

// Scarlet defender 58998
struct npc_scarlet_defender : public ScriptedAI
{
    npc_scarlet_defender(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;
    uint32 curPos;

    void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
    {
        SetEquipmentSlots(false, 98316, 58998, 0);
        me->CastSpell(me, SPELL_HEAVY_ARMOR, false);
        me->SetReactState(REACT_PASSIVE);
        me->AttackStop();
        curPos = urand(0, 1);

        auto& path = curPos ? DefenderPath1 : DefenderPath2;
        Movement::MoveSplineInit init(me);
        for (auto&& itr : path)
            init.Path().push_back(G3D::Vector3(itr.GetPositionX(), itr.GetPositionY(), itr.GetPositionZ()));

        init.SetWalk(false);
        init.Launch();
        events.ScheduleEvent(EVENT_INIT_COMBAT, me->GetSplineDuration());

    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId == EVENT_INIT_COMBAT)
            {
                me->SetReactState(REACT_AGGRESSIVE);
                me->SetInCombatWithZone();
            }
            break;
        }

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

// Heavy armor 113959
class spell_heavy_armor : public AuraScript
{
    PrepareAuraScript(spell_heavy_armor);

public:

    void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
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

public:

    void HandleAfterCast()
    {
        if (Creature* caster = GetCaster()->ToCreature())
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

public:

    void HandleDummyEffect(SpellEffIndex effIndex)
    {
        if (Creature* caster = GetCaster()->ToCreature())
        {
            if (Unit* target = GetHitUnit())
            {
                if (uint32 seat = caster->AI()->GetData(TYPE_LIGHT_BLADES))
                {
                    if (!target->HasAura(SPELL_LIGHT_BLADES_AURA) && seat < 8)
                    {
                        if (Vehicle* vehicle = caster->GetVehicleKit())
                        {
                            if (vehicle->HasEmptySeat(int8(seat)))
                            {
                                target->CastSpell(target, SPELL_LIGHT_BLADES_AURA, true);
                                target->EnterVehicle(caster, int8(seat));
                                caster->AI()->SetData(TYPE_LIGHT_BLADES, 0);
                            }
                        }
                    }

                    caster->CastSpell(target, SPELL_LIGHT_BLADES, true);
                }
            }
        }
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_harlan_light_blades_proc::HandleDummyEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// Blades of Light Eff 111215
class spell_harlan_light_blades_eff : public SpellScript
{
    PrepareSpellScript(spell_harlan_light_blades_eff);

public:

    void HandleEffectHit(SpellEffIndex effIndex)
    {
        if (Unit* target = GetHitUnit())
        {
            if (GetHitDamage() >= int32(target->GetHealth()) && target->GetVehicleBase() == GetCaster())
            {
                // Exit through the control aura before applying lethal damage.
                target->ExitVehicle();
                target->RemoveAurasDueToSpell(SPELL_LIGHT_BLADES_AURA);
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
