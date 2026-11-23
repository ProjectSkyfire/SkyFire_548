/*
 * Ported from Legends of Azeroth 5.4.8.
 * Authors: Jason Dove, Kulyadev, lee, leelf00
 * See LICENSE.md for copyright information.
 */
/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "scarlet_monastery_mop.h"
#include <set>

enum KorloffSpells
{
    SPELL_BLAZING_FISTS = 114807,
    SPELL_BLAZING_FISTS_TRIGGERED = 114808,
    SPELL_FIRESTORM_KICK = 113764,
    SPELL_RISING_FLAME = 114410,
    SPELL_SCORCHED_EARTH = 114460,
    SPELL_FLYING_KICK = 114488,
    SPELL_FLYING_KICK_JUMP = 114487,
    SPELL_TRIGGER_SCORCHED_EARTH = 114464,
    SPELL_BURNING_MAN_1 = 125844,
    SPELL_BURNING_MAN_2 = 125852
};

enum KorloffMisc
{
    NPC_SCORCHED_EARTH_TRIGGER = 59507,
    NPC_TRAINING_DUMMY = 64446,
    WORLDSTATE_BURNING_MAN = 12259,
    DATA_BURNING_DUMMY = 0,
    ACTION_FIRESTORM_FINISHED = 1,
    EVENT_FLYING_KICK = 1,
    EVENT_BLAZING_FISTS = 2,
    TALK_AGGRO = 0,
    TALK_DEATH = 1,
    TALK_FIRESTORM = 2,
    TALK_FISTS = 3
};

class boss_brother_korloff : public CreatureScript
{
public:
    boss_brother_korloff() : CreatureScript("boss_brother_korloff") { }

    struct boss_brother_korloffAI : public BossAI
    {
        boss_brother_korloffAI(Creature* creature) : BossAI(creature, BOSS_BROTHER_KORLOFF), targetGuid(0), fistsTimer(0), lastHealth(100.0f), phaseTwo(false), scorchedEarthPending(false), kickInProgress(false) { }

        uint64 targetGuid;
        uint32 fistsTimer;
        float lastHealth;
        bool phaseTwo;
        bool scorchedEarthPending;
        bool kickInProgress;
        std::set<uint64> burningDummies;

        void ClearDummies()
        {
            std::list<Creature*> dummies;
            GetCreatureListWithEntryInGrid(dummies, me, NPC_TRAINING_DUMMY, 300.0f);
            for (std::list<Creature*>::iterator itr = dummies.begin(); itr != dummies.end(); ++itr)
                (*itr)->RemoveAurasDueToSpell(SPELL_BURNING_MAN_2);
        }

        void RemovePlayerAuras()
        {
            if (instance)
            {
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BURNING_MAN_1);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BURNING_MAN_2);
            }
        }

        void Reset() OVERRIDE
        {
            _Reset();
            me->setActive(true);
            me->setRegeneratingHealth(true);
            me->SetReactState(REACT_AGGRESSIVE);
            targetGuid = 0;
            fistsTimer = 0;
            lastHealth = 100.0f;
            phaseTwo = false;
            scorchedEarthPending = false;
            kickInProgress = false;
            burningDummies.clear();
            ClearDummies();
            RemovePlayerAuras();
            me->GetMap()->SetWorldState(WORLDSTATE_BURNING_MAN, 0);
            if (instance)
                instance->DoUpdateWorldState(WORLDSTATE_BURNING_MAN, 0);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            _EnterCombat();
            if (!me->IsInCombat())
                return;
            Talk(TALK_AGGRO);
            if (instance)
                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
            lastHealth = me->GetHealthPct();
            events.ScheduleEvent(EVENT_FLYING_KICK, 10000);
            events.ScheduleEvent(EVENT_BLAZING_FISTS, 20000);
        }

        void EnterEvadeMode() OVERRIDE
        {
            if (instance)
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            BossAI::EnterEvadeMode();
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            _JustDied();
            Talk(TALK_DEATH);
            if (instance)
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            RemovePlayerAuras();
            ClearDummies();
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
        {
            if (!phaseTwo && me->HealthBelowPctDamaged(50, damage))
            {
                phaseTwo = true;
                scorchedEarthPending = true;
            }
        }

        void SetGUID(uint64 guid, int32 type = 0) OVERRIDE
        {
            if (type == DATA_BURNING_DUMMY && me->IsInCombat() && burningDummies.insert(guid).second && burningDummies.size() >= 6)
            {
                me->GetMap()->SetWorldState(WORLDSTATE_BURNING_MAN, 1);
                if (instance)
                    instance->DoUpdateWorldState(WORLDSTATE_BURNING_MAN, 1);
            }
        }

        void MovementInform(uint32 type, uint32 id) OVERRIDE
        {
            if (type == POINT_MOTION_TYPE && id == EVENT_CHARGE && me->IsInCombat())
            {
                Talk(TALK_FIRESTORM);
                if (!me->AddAura(SPELL_FIRESTORM_KICK, me))
                    DoAction(ACTION_FIRESTORM_FINISHED);
            }
        }

        void DoAction(int32 action) OVERRIDE
        {
            if (action == ACTION_FIRESTORM_FINISHED && kickInProgress && me->IsInCombat())
            {
                kickInProgress = false;
                me->RemoveChanneledCast(targetGuid);
                events.ScheduleEvent(EVENT_FLYING_KICK, 19000);
            }
        }

        struct FirestormTarget
        {
            explicit FirestormTarget(Creature const* creature) : me(creature) { }
            bool operator()(Unit const* target) const
            {
                return target && target->GetTypeId() == TypeID::TYPEID_PLAYER && target->GetDistance2d(me) > 5.0f;
            }
            Creature const* me;
        };

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            if (fistsTimer)
            {
                if (fistsTimer <= diff)
                {
                    fistsTimer = 0;
                    me->InterruptNonMeleeSpells(false);
                    me->RemoveChanneledCast(targetGuid);
                }
                else
                    fistsTimer -= diff;
            }

            events.Update(diff);
            if (me->HasUnitState(UNIT_STATE_CASTING) || fistsTimer || kickInProgress)
                return;

            if (scorchedEarthPending)
            {
                scorchedEarthPending = false;
                me->CastSpell(me, SPELL_SCORCHED_EARTH, false);
                return;
            }

            if (lastHealth - me->GetHealthPct() >= 10.0f)
            {
                lastHealth = me->GetHealthPct();
                me->CastSpell(me, SPELL_RISING_FLAME, false);
                return;
            }

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_BLAZING_FISTS:
                        if (Unit* victim = me->GetVictim())
                        {
                            targetGuid = victim->GetGUID();
                            Talk(TALK_FISTS);
                            me->PrepareChanneledCast(me->GetAngle(victim), SPELL_BLAZING_FISTS);
                            fistsTimer = 6500;
                        }
                        events.ScheduleEvent(EVENT_BLAZING_FISTS, 20000);
                        return;
                    case EVENT_FLYING_KICK:
                        if (Unit* victim = me->GetVictim())
                        {
                            targetGuid = victim->GetGUID();
                            Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, FirestormTarget(me));
                            Position pos;
                            if (target)
                                target->GetPosition(&pos);
                            else
                            {
                                target = victim;
                                me->GetFirstCollisionPosition(pos, frand(6.0f, 8.0f), 0.0f);
                            }
                            me->CastSpell(target, SPELL_FLYING_KICK, true);
                            kickInProgress = true;
                            me->PrepareChanneledCast(me->GetAngle(target));
                            me->GetMotionMaster()->MoveCharge(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), 35.0f, EVENT_CHARGE);
                        }
                        return;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE { return new boss_brother_korloffAI(creature); }
};

struct npc_trigger_scorched_flame : public ScriptedAI
{
    npc_trigger_scorched_flame(Creature* creature) : ScriptedAI(creature) { }
    void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
    {
        me->CastSpell(me, SPELL_TRIGGER_SCORCHED_EARTH, false);
        me->DespawnOrUnsummon(30000);
    }
};

class spell_scorched_earth : public AuraScript
{
    PrepareAuraScript(spell_scorched_earth);
public:
    void PeriodicTick(AuraEffect const* /*effect*/)
    {
        if (Unit* owner = GetOwner()->ToUnit())
            if (owner->isMoving())
                owner->SummonCreature(NPC_SCORCHED_EARTH_TRIGGER, owner->GetPositionX(), owner->GetPositionY(), owner->GetPositionZ(), owner->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
    }
    void Register() OVERRIDE
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_scorched_earth::PeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};

class spell_firestorm_kick : public AuraScript
{
    PrepareAuraScript(spell_firestorm_kick);
public:
    void OnRemove(AuraEffect const* /*effect*/, AuraEffectHandleModes /*mode*/)
    {
        if (Creature* owner = GetOwner()->ToCreature())
            owner->AI()->DoAction(ACTION_FIRESTORM_FINISHED);
    }
    void Register() OVERRIDE
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_firestorm_kick::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_korloff_burning_man : public AuraScript
{
    PrepareAuraScript(spell_korloff_burning_man);
public:
    void OnApply(AuraEffect const* /*effect*/, AuraEffectHandleModes /*mode*/)
    {
        if (GetOwner() && GetOwner()->GetEntry() == NPC_TRAINING_DUMMY)
            if (Unit* caster = GetCaster())
                if (Creature* creature = caster->ToCreature())
                    creature->AI()->SetGUID(GetOwner()->GetGUID(), DATA_BURNING_DUMMY);
    }
    void Register() OVERRIDE
    {
        OnEffectApply += AuraEffectApplyFn(spell_korloff_burning_man::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

void AddSC_boss_brother_korloff()
{
    new boss_brother_korloff();
    new creature_script<npc_trigger_scorched_flame>("npc_trigger_scorched_flame");
    RegisterAuraScript(spell_scorched_earth);
    RegisterAuraScript(spell_firestorm_kick);
    RegisterAuraScript(spell_korloff_burning_man);
}
