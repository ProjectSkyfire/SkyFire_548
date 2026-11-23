/*
 * Ported from Legends of Azeroth 5.4.8.
 * Authors: Jason Dove, Kulyadev, lee, leelf00
 * See LICENSE.md for copyright information.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 or later.
 */
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "Player.h"
#include "ObjectAccessor.h"
#include "Containers.h"
#include "scarlet_monastery_mop.h"

enum ScarletTrashCreatures
{
    NPC_SCARLET_FANATIC = 58555,
    NPC_SCARLET_PURIFIER = 58569,
    NPC_SCARLET_ZEALOT = 58590,
    NPC_SPIRIT_OF_REDEMPTION = 58592,
    NPC_SCARLET_JUDICATOR = 58605,
    NPC_FUEL_BARREL = 59706,
    NPC_ZOMBIFIED_CORPSE = 59771,
    NPC_HOODED_CRUSADER_INTO = 64838
};

static bool HasHandleOneShotEmote(Creature* creature)
{
    uint32 spawnId = creature->GetDBTableGUIDLow();
    return spawnId == 8301104 || spawnId == 8301115 || spawnId == 8301110;
}

// Hooded Crusader 64838
class npc_scm_hooded_crusader : public CreatureScript
{
    public:
        npc_scm_hooded_crusader() : CreatureScript("npc_scm_hooded_crusader") { }

        enum eTalk
        {
            TALK_INSTANCE_ANN = 1,
        };

        struct npc_scm_hooded_crusaderAI : public ScriptedAI
        {
            npc_scm_hooded_crusaderAI(Creature* creature) : ScriptedAI(creature) { }

            bool InstanceAnnonced;

            void InitializeAI() OVERRIDE
            {
                InstanceAnnonced = false;
                me->HandleEmoteStateCommand(EMOTE_STATE_READY1H);
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                if (!InstanceAnnonced)
                {
                    InstanceAnnonced = true;
                    if (me->IsVisible())
                        Talk(TALK_INSTANCE_ANN);
                }
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE { }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_scm_hooded_crusaderAI(creature);

        }
};

class npc_scarlet_flamethower : public CreatureScript
{
    public:
        npc_scarlet_flamethower() : CreatureScript("npc_scarlet_flamethower") { }

        enum eEvents
        {
            EVENT_FLAMETHOWER_NONCOMBAT = 1,
            EVENT_FLAMETHOWER_COMBAT = 2
        };

        enum eSpells
        {
            SPELL_FLAMETHOWER = 115506
        };

        struct npc_scarlet_flamethowerAI : public ScriptedAI
        {
            npc_scarlet_flamethowerAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events, nonCombatEvents;

            void InitializeAI() OVERRIDE
            {
                nonCombatEvents.ScheduleEvent(EVENT_FLAMETHOWER_NONCOMBAT, urand(1500, 2000));
            }

            void Reset() OVERRIDE
            {
                events.Reset();
                nonCombatEvents.Reset();
                nonCombatEvents.ScheduleEvent(EVENT_FLAMETHOWER_NONCOMBAT, urand(1500, 2000));
            }


            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                nonCombatEvents.Reset();
                events.ScheduleEvent(EVENT_FLAMETHOWER_COMBAT, urand(1500, 2000));
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                me->SummonCreature(NPC_FUEL_BARREL, me->GetPositionX() + frand(-2.0f, 2.0f), me->GetPositionY() + frand(-1.5f, 1.5f), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);
                nonCombatEvents.Update(diff);

                while (uint32 eventId = nonCombatEvents.ExecuteEvent())
                {
                    if (eventId == EVENT_FLAMETHOWER_NONCOMBAT)
                    {
                        DoCastAOE(SPELL_FLAMETHOWER);
                        nonCombatEvents.ScheduleEvent(EVENT_FLAMETHOWER_NONCOMBAT, urand(6000, 8000));
                    }
                    break;
                }

                if (!UpdateVictim())
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    if (eventId == EVENT_FLAMETHOWER_COMBAT)
                    {
                        DoCast(SelectTarget(SELECT_TARGET_TOPAGGRO), SPELL_FLAMETHOWER);
                        events.ScheduleEvent(EVENT_FLAMETHOWER_COMBAT, urand(10000, 16000));
                    }
                    break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_scarlet_flamethowerAI(creature);
        }
};

// Fuel Tank 59706
class npc_scm_fuel_tank : public CreatureScript
{
    public:
        npc_scm_fuel_tank() : CreatureScript("npc_scm_fuel_tank") { }

        enum eSpells
        {
            SPELL_BARREL_EXPLOSION = 114952,
            SPELL_FUEL_BARREL      = 114875,
        };

        struct npc_scm_fuel_tankAI : public ScriptedAI
        {
            npc_scm_fuel_tankAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                me->CastSpell(me, SPELL_FUEL_BARREL, false);
            }

            void OnSpellClick(Unit* /*clicker*/, bool& result) OVERRIDE
            {
                me->CastSpell(me, SPELL_BARREL_EXPLOSION, false);
                result = true;
                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                me->DespawnOrUnsummon();
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                if (damage >= me->GetHealth())
                    damage = 0;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }

        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_scm_fuel_tankAI(creature);
        }
};

// Frenzied Spirit 60033
class npc_frenzied_spirit : public CreatureScript
{
    public:
        npc_frenzied_spirit() : CreatureScript("npc_frenzied_spirit") { }

        enum eSpells
        {
            SPELL_FRENZIED_SPIRIT = 115524,
        };

        enum eEvents
        {
            EVENT_FRENZIED        = 1,
        };

        struct npc_frenzied_spiritAI : public ScriptedAI
        {
            npc_frenzied_spiritAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();
                me->RemoveAllAuras();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_FRENZIED, 1000);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE { }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (!UpdateVictim())
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    if (eventId == EVENT_FRENZIED)
                    {
                        me->CastSpell(me, SPELL_FRENZIED_SPIRIT, false);
                        events.ScheduleEvent(EVENT_FRENZIED, urand(1000, 1500));
                    }
                    break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_frenzied_spiritAI(creature);
        }
};

// Scarlet Fanatic 58555
class npc_scarlet_fanatic : public CreatureScript
{
    public:
        npc_scarlet_fanatic() : CreatureScript("npc_scarlet_fanatic") { }

        enum eSpells
        {
            SPELL_FANATICAL_STRIKE = 110956,
            SPELL_HANDS_OF_PURITY  = 110954,
        };

        enum eEvents
        {
            EVENT_FANATICAL_STRIKE = 1,
            EVENT_HANDS_OF_PURITY  = 2,
            EVENT_REPRODUCE_EMOTE  = 3,
        };

        struct npc_scarlet_fanaticAI : public ScriptedAI
        {
            npc_scarlet_fanaticAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events, nonCombatEvents;
            std::vector<uint32> Emotes;

            void InitializeAI() OVERRIDE
            {
                if (me->GetPositionZ() < 11.40f && me->GetPositionZ() > 6.80f)
                {
                    Emotes.push_back(35);
                    Emotes.push_back(60);
                    Emotes.push_back(61);

                    nonCombatEvents.ScheduleEvent(EVENT_REPRODUCE_EMOTE, 2000);
                }
            }

            void Reset() OVERRIDE
            {
                events.Reset();
                nonCombatEvents.Reset();
                me->RemoveAllAuras();

                if (me->GetPositionZ() < 11.40f && me->GetPositionZ() > 6.80f)
                    nonCombatEvents.ScheduleEvent(EVENT_REPRODUCE_EMOTE, 2000);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_HANDS_OF_PURITY, 1000);
                events.ScheduleEvent(EVENT_FANATICAL_STRIKE, urand(2000, 4000));
                nonCombatEvents.Reset();
            }

            void JustDied(Unit* /*killer*/) OVERRIDE { }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                nonCombatEvents.Update(diff);

                while (uint32 eventId = nonCombatEvents.ExecuteEvent())
                {
                    if (eventId == EVENT_REPRODUCE_EMOTE)
                    {
                        if (!Emotes.empty())
                            me->HandleEmoteCommand(Skyfire::Containers::SelectRandomContainerElement(Emotes));

                        nonCombatEvents.ScheduleEvent(EVENT_REPRODUCE_EMOTE, 1500);
                    }
                    break;
                }

                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_HANDS_OF_PURITY:
                            me->CastSpell(me, SPELL_HANDS_OF_PURITY, false);
                            events.ScheduleEvent(EVENT_HANDS_OF_PURITY, 15000);
                            break;
                        case EVENT_FANATICAL_STRIKE:
                            if (Unit* victim = me->GetVictim())
                                me->CastSpell(victim, SPELL_FANATICAL_STRIKE, false);

                            events.ScheduleEvent(EVENT_FANATICAL_STRIKE, urand(5000, 6000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_scarlet_fanaticAI(creature);
        }
};

// Scarlet Judicator 58605
class npc_scarlet_judicator : public CreatureScript
{
    public:
        npc_scarlet_judicator() : CreatureScript("npc_scarlet_judicator") { }

        enum eEvents
        {
            EVENT_REPRODUCE_EMOTE = 1,
        };

        struct npc_scarlet_judicatorAI : public ScriptedAI
        {
            npc_scarlet_judicatorAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap nonCombatEvents;

            void InitializeAI() OVERRIDE
            {
                if (HasHandleOneShotEmote(me))
                    nonCombatEvents.ScheduleEvent(EVENT_REPRODUCE_EMOTE, 2000);
            }

            void Reset() OVERRIDE
            {
                nonCombatEvents.Reset();
                if (HasHandleOneShotEmote(me))
                    nonCombatEvents.ScheduleEvent(EVENT_REPRODUCE_EMOTE, 2000);
                me->RemoveAllAuras();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                nonCombatEvents.Reset();
            }

            void JustDied(Unit* /*killer*/) OVERRIDE { }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                nonCombatEvents.Update(diff);

                while (uint32 eventId = nonCombatEvents.ExecuteEvent())
                {
                    if (eventId == EVENT_REPRODUCE_EMOTE)
                    {
                        me->HandleEmoteCommand(EMOTE_ONESHOT_ATTACK1H);

                        nonCombatEvents.ScheduleEvent(EVENT_REPRODUCE_EMOTE, 1500);
                    }
                    break;
                }

                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_scarlet_judicatorAI(creature);
        }
};

// Pile of Corpses 59722
class npc_pile_corpses : public CreatureScript
{
    public:
        npc_pile_corpses() : CreatureScript("npc_pile_corpses") { }

        enum eSpells
        {
            SPELL_PILE_OF_CORPSES = 114951,
        };

        struct npc_pile_corpsesAI : public ScriptedAI
        {
            npc_pile_corpsesAI(Creature* creature) : ScriptedAI(creature), summons(me) { }

            uint32 frenzitimer;
            EventMap events;
            SummonList summons;

            void InitializeAI() OVERRIDE
            {
                me->setActive(true);
                Reset();
            }

            void Reset() OVERRIDE
            {
                events.Reset();
                me->RemoveAllAuras();
                me->ClearUnitState(UNIT_STATE_ROOT);
                summons.DespawnAll();
                DoCast(me, SPELL_PILE_OF_CORPSES);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                me->SetReactState(REACT_PASSIVE);
                me->AddUnitState(UNIT_STATE_ROOT);
                DoCast(me, SPELL_PILE_OF_CORPSES);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                summons.DespawnAll();
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (!UpdateVictim())
                    return;
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_pile_corpsesAI(creature);
        }
};

// Scarlet Centurion 59746
class npc_scarlet_centurion : public CreatureScript
{
    public:
        npc_scarlet_centurion() : CreatureScript("npc_scarlet_centurion") { }

        enum eSpells
        {
            SPELL_CLEAVE      = 115519,
            SPELL_RETALIATION = 115511,
        };

        enum eEvents
        {
            EVENT_CLEAVE      = 1,
            EVENT_ZOMBIE_RISE = 2,
        };

        struct npc_scarlet_centurionAI : public ScriptedAI
        {
            npc_scarlet_centurionAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events, nonCombatEvents;
            bool retal;

            void InitializeAI() OVERRIDE
            {
                retal = false;
                nonCombatEvents.ScheduleEvent(EVENT_ZOMBIE_RISE, 1500);
            }

            void Reset() OVERRIDE
            {
                events.Reset();
                nonCombatEvents.Reset();
                nonCombatEvents.ScheduleEvent(EVENT_ZOMBIE_RISE, 1500);
                retal = false;
                me->RemoveAllAuras();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                nonCombatEvents.Reset();
                events.ScheduleEvent(EVENT_CLEAVE, urand(2000, 4000));
            }

            void JustDied(Unit* /*killer*/) OVERRIDE { }

            void DamageTaken(Unit* killer, uint32 &damage) OVERRIDE
            {
                if (!retal && HealthBelowPct(20))
                {
                    retal = true;
                    me->CastSpell(me, SPELL_RETALIATION, false);
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                nonCombatEvents.Update(diff);

                while (uint32 eventId = nonCombatEvents.ExecuteEvent())
                {
                    if (eventId == EVENT_ZOMBIE_RISE)
                    {
                        if (Creature* zombie = GetClosestCreatureWithEntry(me, NPC_ZOMBIFIED_CORPSE, 10.0f, true))
                            me->Attack(zombie, true);

                        nonCombatEvents.ScheduleEvent(EVENT_ZOMBIE_RISE, urand(5000, 6000));
                    }
                    break;
                }

                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    if (eventId == EVENT_CLEAVE)
                    {
                        if (Unit* target = me->GetVictim())
                            me->CastSpell(target, SPELL_CLEAVE, true);

                        events.ScheduleEvent(EVENT_CLEAVE, urand(5000, 6000));
                    }
                    break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_scarlet_centurionAI(creature);
        }
};

// Zombified Corpse 59771
class npc_zombified_corpse : public CreatureScript
{
    public:
        npc_zombified_corpse() : CreatureScript("npc_zombified_corpse") { }

        enum eEvents
        {
            EVENT_COSMETIC = 1,
        };

        struct npc_zombified_corpseAI : public ScriptedAI
        {
            npc_zombified_corpseAI(Creature* creature) : ScriptedAI(creature) { }

            bool attacked;
            EventMap nonCombatEvents;

            void InitializeAI() OVERRIDE
            {
                me->setFaction(1962);
                attacked = false;
            }

            void Reset() OVERRIDE
            {
                attacked = false;
                nonCombatEvents.Reset();
            }

            void DamageTaken(Unit* killer, uint32 &damage) OVERRIDE
            {
                if (!attacked && killer && killer->GetTypeId() != TypeID::TYPEID_PLAYER)
                {
                    attacked = true;
                    me->Attack(killer, true);
                    nonCombatEvents.ScheduleEvent(EVENT_COSMETIC, urand(3000, 5000));
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                nonCombatEvents.Update(diff);

                while (uint32 eventId = nonCombatEvents.ExecuteEvent())
                {
                    if (eventId == EVENT_COSMETIC)
                        me->DisappearAndDie();
                    break;
                }

                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_zombified_corpseAI(creature);
        }
};

// Scarlet Zealot 58590
class npc_scarlet_zealot : public CreatureScript
{
    public:
        npc_scarlet_zealot() : CreatureScript("npc_scarlet_zealot") { }

        enum eSpells
        {
            SPELL_SMITE        = 111010,
            SPELL_HEAL         = 111008,
            SPELL_RESSURECTION = 111670,
        };

        enum eEvents
        {
            EVENT_SMITE        = 1,
            EVENT_HEAL         = 2,
            EVENT_RESSURECTION = 3,
        };

        struct npc_scarlet_zealotAI : public ScriptedAI
        {
            npc_scarlet_zealotAI(Creature* creature) : ScriptedAI(creature), ResTarget(0) { }

            EventMap events;
            uint64 ResTarget;

            void Reset() OVERRIDE
            {
                events.Reset();
                ResTarget = 0;
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_HEAL, 10000);
                events.ScheduleEvent(EVENT_SMITE, urand(4000, 6000));
                events.ScheduleEvent(EVENT_RESSURECTION, 15000);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                me->SummonCreature(NPC_SPIRIT_OF_REDEMPTION, me->GetPositionX() + frand(-2.0f, 2.0f), me->GetPositionY() + frand(-1.5f, 1.5f), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 12000);
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                if (Unit* RessTarget = ObjectAccessor::GetUnit(*me, ResTarget))
                {
                    if (Creature* target = RessTarget->ToCreature())
                    {
                        target->Respawn();
                        if (Player* itr = target->FindNearestPlayer(35.0f))
                            target->AI()->AttackStart(itr);
                    }
                    ResTarget = 0;
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
                        case EVENT_HEAL:
                        {
                            me->CastSpell(me, SPELL_HEAL, false);

                            if (Unit* lowTarget = ObjectAccessor::GetUnit(*me, GetLowestFriendlyGUID()))
                                DoCast(lowTarget, SPELL_HEAL);

                            events.ScheduleEvent(EVENT_HEAL, urand(9000, 14000));
                            break;
                        }
                        case EVENT_SMITE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                me->CastSpell(target, SPELL_SMITE, false);

                            events.ScheduleEvent(EVENT_SMITE, urand(4000, 6000));
                            break;
                        case EVENT_RESSURECTION:
                            if (Unit* target = ObjectAccessor::GetUnit(*me, RessurectionTarget()))
                            {
                                ResTarget = target->GetGUID();
                                me->CastSpell(target, SPELL_RESSURECTION, false);
                            }

                            events.ScheduleEvent(EVENT_RESSURECTION, urand(9000, 180000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

            private:
                uint64 GetLowestFriendlyGUID()
                {
                    std::list<Creature*> tmpTargets;

                    GetCreatureListWithEntryInGrid(tmpTargets, me, NPC_SCARLET_FANATIC, 50.0f);
                    GetCreatureListWithEntryInGrid(tmpTargets, me, NPC_SCARLET_JUDICATOR, 50.0f);
                    GetCreatureListWithEntryInGrid(tmpTargets, me, NPC_SCARLET_ZEALOT, 50.0f);
                    GetCreatureListWithEntryInGrid(tmpTargets, me, NPC_SCARLET_PURIFIER, 50.0f);

                    if (tmpTargets.empty())
                        return 0;

                    tmpTargets.sort(Skyfire::HealthPctOrderPred());

                    if (Creature* lowestTarget = tmpTargets.front())
                        return lowestTarget->GetGUID();

                    return 0;
                }

                uint64 RessurectionTarget()
                {
                    std::list<Creature*> CorpsedScarlet;
                    GetCreatureListWithEntryInGrid(CorpsedScarlet, me, NPC_SCARLET_PURIFIER, 50.0f);
                    GetCreatureListWithEntryInGrid(CorpsedScarlet, me, NPC_SCARLET_FANATIC, 50.0f);
                    GetCreatureListWithEntryInGrid(CorpsedScarlet, me, NPC_SCARLET_ZEALOT, 50.0f);
                    GetCreatureListWithEntryInGrid(CorpsedScarlet, me, NPC_SCARLET_JUDICATOR, 50.0f);

                    for (auto&& itr : CorpsedScarlet)
                        if (!itr->IsAlive())
                            return itr->GetGUID();

                    return 0;
                }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_scarlet_zealotAI(creature);
        }
};

// Spirit of Redemption 58592
class npc_sc_spirit_of_redemption : public CreatureScript
{
    public:
        npc_sc_spirit_of_redemption() : CreatureScript("npc_sc_spirit_of_redemption") { }

        enum eSpells
        {
            SPELL_HEAL = 111008,
        };


        struct npc_sc_spirit_of_redemptionAI : public ScriptedAI
        {
            npc_sc_spirit_of_redemptionAI(Creature* creature) : ScriptedAI(creature) { }

            void IsSummonedBy(Unit* summoner) OVERRIDE
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE
            {
                if (!me->HasUnitState(UNIT_STATE_CASTING))
                    if (Unit* lowTarget = ObjectAccessor::GetUnit(*me, GetLowestFriendlyGUID()))
                        DoCast(lowTarget, SPELL_HEAL);
            }

            private:
                uint64 GetLowestFriendlyGUID()
                {
                    std::list<Creature*> tmpTargets;

                    GetCreatureListWithEntryInGrid(tmpTargets, me, NPC_SCARLET_FANATIC, 50.0f);
                    GetCreatureListWithEntryInGrid(tmpTargets, me, NPC_SCARLET_JUDICATOR, 50.0f);
                    GetCreatureListWithEntryInGrid(tmpTargets, me, NPC_SCARLET_ZEALOT, 50.0f);
                    GetCreatureListWithEntryInGrid(tmpTargets, me, NPC_SCARLET_PURIFIER, 50.0f);

                    if (tmpTargets.empty())
                        return 0;

                    tmpTargets.sort(Skyfire::HealthPctOrderPred());

                    if (Creature* lowestTarget = tmpTargets.front())
                        return lowestTarget->GetGUID();

                    return 0;
                }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_sc_spirit_of_redemptionAI(creature);
        }
};

// Scarlet purifier 58569
class npc_scarlet_purifier : public CreatureScript
{
    public:
        npc_scarlet_purifier() : CreatureScript("npc_scarlet_purifier") { }

        enum eSpells
        {
            SPELL_FLAMESTRIKE      = 110963,
            SPELL_PURIFYING_FLAMES = 110968,
        };

        enum eEvents
        {
            EVENT_FLAMESTRIKE      = 1,
            EVENT_PURIFYING_FLAMES = 2,
        };

        struct npc_scarlet_purifierAI : public ScriptedAI
        {
            npc_scarlet_purifierAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE { events.Reset(); }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_FLAMESTRIKE, 9000);
                events.ScheduleEvent(EVENT_PURIFYING_FLAMES, urand(4000, 6000));
            }

            void JustDied(Unit* /*killer*/) OVERRIDE { }

            void DamageTaken(Unit* killer, uint32 &damage) OVERRIDE { }

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
                        case EVENT_FLAMESTRIKE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_FARTHEST, 0, 0.0f, true))
                                me->CastSpell(target, SPELL_FLAMESTRIKE, false);

                            events.ScheduleEvent(EVENT_FLAMESTRIKE, urand(15000, 20000));
                            break;
                        case EVENT_PURIFYING_FLAMES:
                            if (Unit* target = me->GetVictim())
                                me->CastSpell(target, SPELL_PURIFYING_FLAMES, false);

                            events.ScheduleEvent(EVENT_PURIFYING_FLAMES, urand(4000, 6000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_scarlet_purifierAI(creature);
        }
};
// Cleave 115519
class spell_sc_cleave : public SpellScriptLoader
{
    public:
        spell_sc_cleave() : SpellScriptLoader("spell_sc_cleave") { }

        class spell_sc_cleave_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sc_cleave_SpellScript);

        public:

            void RecalculateDamage(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();

                if (!caster)
                    return;

                if (InstanceScript* Iscript = caster->GetInstanceScript())
                {
                    if (Iscript->instance && Iscript->instance->IsHeroic())
                        SetHitDamage(40000);
                    else
                        SetHitDamage(urand(900, 1200));
                }
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_sc_cleave_SpellScript::RecalculateDamage, EFFECT_0, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_sc_cleave_SpellScript();
        }
};

// Ressurection 111670
class spell_sc_ressurection : public SpellScriptLoader
{
    public:
        spell_sc_ressurection() : SpellScriptLoader("spell_sc_ressurection") { }

        class spell_sc_ressurection_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sc_ressurection_SpellScript);

        public:


            void HandleAfterCast()
            {
                if (Unit* caster = GetCaster())
                    if (Creature* creature = caster->ToCreature())
                        creature->AI()->DoAction(0);
            }

            void Register() OVERRIDE
            {
                AfterCast += SpellCastFn(spell_sc_ressurection_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_sc_ressurection_SpellScript();
        }
};

class BarrelExplosionTargetSelector
{
    public:
        BarrelExplosionTargetSelector() { }

        bool operator()(WorldObject* object)
        {
            return object && object->ToPlayer();
        }
};

// Barrel explosion 114952
class spell_scm_barrel_explosion : public SpellScriptLoader
{
    public:
        spell_scm_barrel_explosion() : SpellScriptLoader("spell_scm_barrel_explosion") { }

        class spell_scm_barrel_explosion_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_scm_barrel_explosion_SpellScript);

        public:

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(BarrelExplosionTargetSelector());
            }


            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_scm_barrel_explosion_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENTRY);
            }
        };

        SpellScript *GetSpellScript() const OVERRIDE
        {
            return new spell_scm_barrel_explosion_SpellScript();
        }
};

// AreaTrigger 8364
class AreaTrigger_at_scarlet_monastery : public AreaTriggerScript
{
    public:
        AreaTrigger_at_scarlet_monastery() : AreaTriggerScript("AreaTrigger_at_scarlet_monastery") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* trigger) OVERRIDE
        {
            if (player->GetInstanceScript())
                if (Creature* HoodedCrusader = GetClosestCreatureWithEntry(player, NPC_HOODED_CRUSADER_INTO, 50.0f, true))
                    if (HoodedCrusader->AI())
                        HoodedCrusader->AI()->DoAction(1);

            return false;
        }
};

void AddSC_scarlet_monastery_mop()
{
    new npc_scm_hooded_crusader();
    new npc_scarlet_flamethower();
    new npc_scm_fuel_tank();
    new npc_frenzied_spirit();
    new npc_scarlet_fanatic();
    new npc_scarlet_judicator();
    new npc_pile_corpses();
    new npc_scarlet_centurion();
    new npc_zombified_corpse();
    new npc_scarlet_zealot();
    new npc_sc_spirit_of_redemption();
    new npc_scarlet_purifier();
    new spell_sc_cleave();
    new spell_sc_ressurection();
    new spell_scm_barrel_explosion();
    new AreaTrigger_at_scarlet_monastery();
}
