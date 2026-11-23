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

#include "Vehicle.h"
#include "hour_of_twilight.h"

enum ScriptTexts
{
    SAY_DEATH                           = 0,
    SAY_EVENT_1                         = 1,
    SAY_EVENT_2                         = 2,
    SAY_LIGHT                           = 3,
    SAY_TWILIGHT                        = 4,
    SAY_EVENT_3                         = 5,
    SAY_EVENT_4                         = 6
};

enum Spells
{
    SPELL_HOLY_WALL                     = 102629,

    SPELL_SMITE                         = 104503,
    SPELL_RIGHTEOUS_SNEAR_AOE           = 103149,
    SPELL_RIGHTEOUS_SNEAR_AURA          = 103151,
    SPELL_RIGHTEOUS_SNEAR_DMG           = 103161,

    SPELL_PURIFYING_LIGHT               = 103565,
    SPELL_PURIFYING_LIGHT_TARGETING     = 103600,
    SPELL_PURIFYING_LIGHT_GROW          = 103579,
    SPELL_PURIFYING_LIGHT_SUMMON_1      = 103584,
    SPELL_PURIFYING_LIGHT_SUMMON_2      = 103585,
    SPELL_PURIFYING_LIGHT_SUMMON_3      = 103586,
    SPELL_PURIFYING_LIGHT_DUMMY         = 103578,
    SPELL_PURIFYING_BLAST               = 103648,
    SPELL_PURIFYING_BLAST_DMG           = 103651,
    SPELL_PURIFIED_LIGHT                = 103654, // to stalker
    SPELL_PURIFIED_DMG                  = 103653,

    SPELL_UNSTABLE_TWILIGHT_DUMMY       = 103766,

    SPELL_TWILIGHT_EPIPHANY             = 103754,
    SPELL_TWILIGHT_EPIPHANY_DMG         = 103755,
    SPELL_ENGULFING_TWILIGHT            = 103762,
    SPELL_TRANSFORM                     = 103765,

    SPELL_TWILIGHT_BLAST                = 104504,

    SPELL_TWILIGHT_SNEAR_AOE            = 103362,
    SPELL_TWILIGHT_SNEAR_AURA           = 103363,
    SPELL_TWILIGHT_SNEAR_DMG            = 103526,

    SPELL_CORRUPTING_TWILIGHT           = 103767,
    SPELL_CORRUPTING_TWILIGHT_TARGETING = 103768,
    SPELL_CORRUPTING_TWILIGHT_GROW      = 103773,
    SPELL_CORRUPTING_TWILIGHT_SUMMON_1  = 103770,
    SPELL_CORRUPTING_TWILIGHT_SUMMON_2  = 103771,
    SPELL_CORRUPTING_TWILIGHT_SUMMON_3  = 103772,
    SPELL_CORRUPTING_TWILIGHT_DUMMY     = 103769,
    SPELL_TWILIGHT_BOLT                 = 103776,
    SPELL_TWILIGHT_BOLT_DMG             = 103777,
    SPELL_TWILIGHT                      = 103774,
    SPELL_TWILIGHT_DMG                  = 103775,

    SPELL_WAVE_OF_TWILIGHT              = 103780,
    SPELL_WAVE_OF_TWILIGHT_DMG          = 103781,

    SPELL_WAVE_OF_LIGHT                 = 103678,
    SPELL_WAVE_OF_LIGHT_DMG             = 103684
};

enum Events
{
    EVENT_WAVE_OF_LIGHT                 = 1,
    EVENT_PURIFYING_LIGHT               = 2,
    EVENT_SMITE                         = 3,
    EVENT_RIGHTEOUS_SNEAR               = 4,
    EVENT_TWILIGHT_BLAST                = 5,
    EVENT_WAVE_OF_TWILIGHT              = 6,
    EVENT_CORRUPTING_TWILIGHT           = 7,
    EVENT_TWILIGHT_SNEAR                = 8,
    EVENT_CONTINUE                      = 9,
    EVENT_JUMP_2                        = 10,
    EVENT_INSIDE                        = 11,
    EVENT_ESCORT                        = 12,
    EVENT_ESCORT_DIFF                   = 13,
    EVENT_TALK_1                        = 14,
    EVENT_TALK_2                        = 15,
    EVENT_TALK_3                        = 16,
    EVENT_TALK_4                        = 17
};

enum Adds
{
    NPC_TWILIGHT_SPARK                  = 55466,
    NPC_PURIFYING_LIGHT                 = 55377,
    NPC_PURIFYING_BLAST                 = 55427,
    NPC_WAVE_OF_VIRTUE                  = 55441,
    NPC_CORRUPTING_TWILIGHT             = 55467,
    NPC_TWILIGHT_BLAST                  = 55468,
    NPC_WAVE_OF_TWILIGHT                = 55469,
};

enum Actions
{
    ACTION_BLAST                        = 0
};

std::list <Creature*> GetSphereStalkers(WorldObject* owner, uint32 npc_entry)
{
    std::list<Creature*> StalkersInArea;
    GetCreatureListWithEntryInGrid(StalkersInArea, owner, npc_entry, 50.0f);

    return StalkersInArea;
}

class boss_archbishop_benedictus : public CreatureScript
{
    public:
        boss_archbishop_benedictus() : CreatureScript("boss_archbishop_benedictus") { }

        struct boss_archbishop_benedictusAI : public BossAI
        {
            boss_archbishop_benedictusAI(Creature* creature) : BossAI(creature, DATA_BENEDICTUS)
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
                me->setFaction(14);
                bPhase = false;
            }

            uint32 wp;
            EventMap events, nonCombatEvents;
            bool CanMove;

            void InitializeAI() OVERRIDE
            {
                if (!instance || static_cast<InstanceMap*>(me->GetMap())->GetScriptId() != sObjectMgr->GetScriptId(HoTScriptName))
                    me->IsAIEnabled = false;
                else if (!me->isDead())
                    Reset();
                wp = 0;
                CanMove = false;
            }

            void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
            {
                me->SetReactState(REACT_PASSIVE);
                me->setFaction(35);
                me->GetMotionMaster()->MovePoint(0, BenedictusWayPoint);
                SetEquipmentSlots(false, ImmaculateRecoveryStaff , 0, EQUIP_NO_CHANGE);
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                wp++;

                if (CanMove)
                    nonCombatEvents.ScheduleEvent(EVENT_ESCORT, urand(100, 200));

                switch (pointId)
                {
                    case 0:
                        Talk(SAY_EVENT_1);
                        nonCombatEvents.ScheduleEvent(EVENT_INSIDE, 2000);
                        break;
                    case 2:
                        nonCombatEvents.Reset();
                        me->CastSpell(me, SPELL_HOLY_WALL, false);
                        nonCombatEvents.ScheduleEvent(EVENT_ESCORT_DIFF, 2600);
                        break;
                    case 7:
                        nonCombatEvents.Reset();
                        me->SetFacingTo(me->GetOrientation() + M_PI);
                        Talk(SAY_EVENT_2);
                        nonCombatEvents.ScheduleEvent(EVENT_TALK_1, urand(2500, 3500));
                        me->SetHomePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
                        break;
                    default:
                        break;
                }
            }

            void Reset() OVERRIDE
            {
                _Reset();

                bPhase = false;

                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TWILIGHT_EPIPHANY_DMG);
                DespawnCreatures(NPC_PURIFYING_BLAST);
                DespawnCreatures(NPC_TWILIGHT_BLAST);

            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                me->SetReactState(REACT_AGGRESSIVE);
                bPhase = false;
                _EnterCombat();
                events.ScheduleEvent(EVENT_PURIFYING_LIGHT, 10000);
                events.ScheduleEvent(EVENT_SMITE, urand(1000, 2000));
                events.ScheduleEvent(EVENT_RIGHTEOUS_SNEAR, urand(5000, 10000));
                events.ScheduleEvent(EVENT_WAVE_OF_LIGHT, 35000);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TWILIGHT_EPIPHANY_DMG);
                instance->SetBossState(DATA_BENEDICTUS, IN_PROGRESS);
                DoZoneInCombat();
                SendThrallInCombat();
            }

            void SendThrallInCombat()
            {
                if (Creature* Thrall = GetClosestCreatureWithEntry(me, NPC_THRALL_5, 100.0f))
                {
                    me->SetInCombatWith(Thrall);
                    Thrall->AI()->DoAction(ACTION_COMBAT);
                }
            }

            void SendThrallActivateShield()
            {
                if (Creature* Thrall = GetClosestCreatureWithEntry(me, NPC_THRALL_5, 100.0f))
                    Thrall->AI()->DoAction(ACTION_WATER_SHIELD);
            }

            void DoAction(int32 /*action*/) OVERRIDE { }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();

                Talk(SAY_DEATH);

                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TWILIGHT_EPIPHANY_DMG);
                DespawnCreatures(NPC_PURIFYING_BLAST);
                DespawnCreatures(NPC_TWILIGHT_BLAST);
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                BossAI::JustSummoned(summon);
                //if (summon->GetEntry() == NPC_PURIFYING_LIGHT)
                //    summon->EnterVehicle(me, -1, true);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                nonCombatEvents.Update(diff);
                events.Update(diff);

                if (uint32 eventId = nonCombatEvents.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_INSIDE:
                            if (Creature* Thrall = GetClosestCreatureWithEntry(me, NPC_THRALL_4, 100.0f))
                                Thrall->AI()->DoAction(ACTION_BENEDICTUS);
                            nonCombatEvents.ScheduleEvent(EVENT_ESCORT, 1500);
                            break;
                        case EVENT_ESCORT:
                            if (me->IsInCombat())
                                return;

                            if (!CanMove)
                                CanMove = true;

                            me->GetMotionMaster()->MovementExpired();
                            me->GetMotionMaster()->Clear();
                            if (me->GetUnitMovementFlags() == MOVEMENTFLAG_WALKING)
                                me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
                            me->GetMotionMaster()->MovePoint(wp, BenedictusWaypoints[wp - 1]);
                            break;
                        case EVENT_ESCORT_DIFF:
                            nonCombatEvents.ScheduleEvent(EVENT_ESCORT, urand(100, 200));
                            break;
                        case EVENT_TALK_1:
                            if (Creature* Thrall = GetClosestCreatureWithEntry(me, NPC_THRALL_5, 80.0f))
                                Thrall->AI()->DoAction(ACTION_BENEDICTUS);
                            nonCombatEvents.ScheduleEvent(EVENT_TALK_2, urand(2500, 3500));
                            break;
                        case EVENT_TALK_2:
                            Talk(SAY_EVENT_3);
                            nonCombatEvents.ScheduleEvent(EVENT_TALK_3, urand(2500, 3500));
                            break;
                        case EVENT_TALK_3:
                            if (Creature* Thrall = GetClosestCreatureWithEntry(me, NPC_THRALL_5, 80.0f))
                                Thrall->AI()->DoAction(ACTION_TALK_FINISH_HIM);
                            nonCombatEvents.ScheduleEvent(EVENT_TALK_4, urand(2500, 3500));
                            break;
                        case EVENT_TALK_4:
                            Talk(SAY_EVENT_4);
                            me->setFaction(14);
                            break;
                    }
                }

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (me->HealthBelowPct(60) && !bPhase)
                {
                    bPhase = true;

                    events.CancelEvent(EVENT_WAVE_OF_LIGHT);
                    events.CancelEvent(EVENT_PURIFYING_LIGHT);
                    events.CancelEvent(EVENT_SMITE);
                    events.CancelEvent(EVENT_RIGHTEOUS_SNEAR);
                    events.ScheduleEvent(EVENT_CONTINUE, 6000);
                    DoCast(me, SPELL_TWILIGHT_EPIPHANY);
                    return;
                }

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SMITE:
                            DoCastVictim(SPELL_SMITE);
                            events.ScheduleEvent(EVENT_SMITE, urand(5000, 7000));
                            break;
                        case EVENT_PURIFYING_LIGHT:
                            for (uint8 i = 0; i < 3; ++i)
                                me->SummonCreature(NPC_PURIFYING_LIGHT, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 15.0f, me->GetOrientation(), TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 20000);
                            DoCast(me, SPELL_PURIFYING_LIGHT);
                            for (auto itr : GetSphereStalkers(me, NPC_PURIFYING_LIGHT))
                                itr->AI()->DoAction(ACTION_BLAST);
                            events.ScheduleEvent(EVENT_PURIFYING_LIGHT, urand(15000, 20000));
                            break;
                        case EVENT_RIGHTEOUS_SNEAR:
                            DoCastAOE(SPELL_RIGHTEOUS_SNEAR_AOE);
                            events.ScheduleEvent(EVENT_RIGHTEOUS_SNEAR, urand(20000, 25000));
                            break;
                        case EVENT_CONTINUE:
                            DoCast(me, SPELL_TRANSFORM, true);
                            events.ScheduleEvent(EVENT_CORRUPTING_TWILIGHT, 10000);
                            events.ScheduleEvent(EVENT_TWILIGHT_BLAST, urand(1000, 2000));
                            events.ScheduleEvent(EVENT_TWILIGHT_SNEAR, urand(5000, 10000));
                            events.ScheduleEvent(EVENT_WAVE_OF_TWILIGHT, 35000);
                            break;
                        case EVENT_TWILIGHT_BLAST:
                            DoCastVictim(SPELL_TWILIGHT_BLAST);
                            events.ScheduleEvent(EVENT_TWILIGHT_BLAST, urand(5000, 7000));
                            break;
                        case EVENT_CORRUPTING_TWILIGHT:
                            for (uint8 i = 0; i < 3; ++i)
                                me->SummonCreature(NPC_TWILIGHT_BLAST, me->GetPositionX() + frand(-5.0f, 5.0f), me->GetPositionY() + frand(-3.0f, 3.0f), me->GetPositionZ() + 15.0f, me->GetOrientation(), TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 20000);
                            DoCast(me, SPELL_CORRUPTING_TWILIGHT);
                            for (auto itr : GetSphereStalkers(me, NPC_TWILIGHT_BLAST))
                                itr->AI()->DoAction(ACTION_BLAST);
                            events.ScheduleEvent(EVENT_CORRUPTING_TWILIGHT, urand(15000, 20000));
                            break;
                        case EVENT_TWILIGHT_SNEAR:
                            DoCastAOE(SPELL_TWILIGHT_SNEAR_AOE);
                            events.ScheduleEvent(EVENT_TWILIGHT_SNEAR, urand(20000, 25000));
                            break;
                        case EVENT_WAVE_OF_TWILIGHT:
                            me->SummonCreature(NPC_WAVE_OF_TWILIGHT, WaveSpawnPoint, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                            SendThrallActivateShield();
                            events.ScheduleEvent(EVENT_WAVE_OF_TWILIGHT, 50000);
                            break;
                        case EVENT_WAVE_OF_LIGHT:
                            me->SummonCreature(NPC_WAVE_OF_VIRTUE, WaveSpawnPoint, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                            SendThrallActivateShield();
                            events.ScheduleEvent(EVENT_WAVE_OF_LIGHT, 50000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            bool bPhase;

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
            return GetHoTAI<boss_archbishop_benedictusAI>(creature);
        }
};

// 55377, 55468
class npc_archbishop_benedictus_purifying_light : public CreatureScript
{
    public:
        npc_archbishop_benedictus_purifying_light() : CreatureScript("npc_archbishop_benedictus_purifying_light") { }

        struct npc_archbishop_benedictus_purifying_lightAI : public ScriptedAI
        {
            npc_archbishop_benedictus_purifying_lightAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap nonCombatEvents;

            void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetCanFly(true);
                me->SetDisableGravity(true);
                switch (me->GetEntry())
                {
                    case NPC_PURIFYING_LIGHT:
                        nonCombatEvents.ScheduleEvent(EVENT_PURIFYING_LIGHT, 2000);
                        break;
                    case NPC_TWILIGHT_BLAST:
                        nonCombatEvents.ScheduleEvent(EVENT_TWILIGHT_BLAST, 2000);
                        break;
                    default:
                        break;
                }
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                if (pointId == 0)
                {
                    me->RemoveAllAuras();
                    me->DespawnOrUnsummon(20000);

                    me->GetEntry() == NPC_PURIFYING_LIGHT ? me->AddAura(SPELL_PURIFIED_LIGHT, me) : me->AddAura(SPELL_TWILIGHT, me);
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                nonCombatEvents.Update(diff);

                if (uint32 eventId = nonCombatEvents.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_PURIFYING_LIGHT:
                        case EVENT_TWILIGHT_BLAST:
                            if (Unit* itr = SelectTarget(SELECT_TARGET_RANDOM))
                                me->GetMotionMaster()->MovePoint(0, itr->GetPositionX() + frand(-6.0, 6.0), me->GetPositionY() + frand(-4.0f, 4.0f), itr->GetPositionZ());
                            break;
                        default:
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetHoTAI<npc_archbishop_benedictus_purifying_lightAI>(creature);
        }
};

// Twilight | Light wave 55469, 55551
class npc_twilight_light_wave : public CreatureScript
{
    public:
        npc_twilight_light_wave() : CreatureScript("npc_twilight_light_wave") { }

        struct npc_twilight_light_waveAI : public ScriptedAI
        {
            npc_twilight_light_waveAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;
            uint32 timer;
            uint32 desp;

            void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
            {
                timer = 1000;
                me->GetMotionMaster()->MovePoint(0, WaveDespawnPoint);
                if (me->GetEntry() == NPC_WAVE_OF_VIRTUE)
                    me->AddAura(SPELL_WAVE_OF_LIGHT, me);
            }

            void Reset() OVERRIDE
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            }

            void InitializeAI() OVERRIDE
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                if (me->GetEntry() == NPC_WAVE_OF_VIRTUE)
                    me->AddAura(SPELL_WAVE_OF_LIGHT, me);
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                if (pointId == 0)
                    me->DespawnOrUnsummon();
            }

            void DoAction(int32 /*action*/) OVERRIDE { }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                damage = 0;
            }

            void GetPlayers()
            {
                std::list <Player*> PlayersInArea;
                GetPlayerListInGrid(PlayersInArea, me, 6.0f);

                if (PlayersInArea.empty())
                    return;

                for (auto itr : PlayersInArea)
                    if (itr->IsAlive())
                        if (!itr->HasAura(103688))
                            switch (me->GetEntry())
                        {
                            case NPC_WAVE_OF_TWILIGHT:
                                me->CastSpell(itr, SPELL_WAVE_OF_TWILIGHT_DMG, false);
                                break;
                            case NPC_WAVE_OF_VIRTUE:
                                me->CastSpell(itr, SPELL_WAVE_OF_LIGHT_DMG, false);
                                break;
                        }

            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (timer <= diff)
                {
                    GetPlayers();
                    timer = 1000;
                }
                else timer -= diff;

            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetHoTAI<npc_twilight_light_waveAI>(creature);
        }
};

class spell_archbishop_benedictus_righteous_snear_aoe : public SpellScriptLoader
{
    public:
        spell_archbishop_benedictus_righteous_snear_aoe() : SpellScriptLoader("spell_archbishop_benedictus_righteous_snear_aoe") { }

        class spell_archbishop_benedictus_righteous_snear_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_archbishop_benedictus_righteous_snear_aoe_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (!GetCaster())
                    return;

                if (targets.size() <= 1)
                    return;

                if (Creature* pBenedictus = GetCaster()->ToCreature())
                    if (Unit* pTank = pBenedictus->GetVictim())
                        targets.remove(pTank);

                if (targets.size() > 1)
                    Skyfire::Containers::RandomResizeList(targets, 1);
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->CastCustomSpell(((m_scriptSpellId == SPELL_RIGHTEOUS_SNEAR_AOE) ? SPELL_RIGHTEOUS_SNEAR_AURA : SPELL_TWILIGHT_SNEAR_AURA), SPELLVALUE_AURA_STACK, 2, GetHitUnit(), true);
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_archbishop_benedictus_righteous_snear_aoe_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnEffectHitTarget += SpellEffectFn(spell_archbishop_benedictus_righteous_snear_aoe_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_archbishop_benedictus_righteous_snear_aoe_SpellScript();
        }
};

// light wall cosmetic 102629
class spell_light_wall_cosmetic : public SpellScriptLoader
{
    public:
        spell_light_wall_cosmetic() : SpellScriptLoader("spell_light_wall_cosmetic") { }

        class spell_light_wall_cosmetic_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_light_wall_cosmetic_SpellScript);


            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if ([](WorldObject* target) -> bool
                {
                    if (Creature* creature = target->ToCreature())
                        return creature->GetEntry() != NPC_LIGHT_WALL_STALKER;

                    return true;
                });
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_light_wall_cosmetic_SpellScript::FilterTargets, EFFECT_0, TARGET_DEST_NEARBY_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_light_wall_cosmetic_SpellScript();
        }
};

void AddSC_boss_archbishop_benedictus()
{
    new boss_archbishop_benedictus();
    new npc_archbishop_benedictus_purifying_light();
    new npc_twilight_light_wave();
    new spell_archbishop_benedictus_righteous_snear_aoe();
    new spell_light_wall_cosmetic();
}
