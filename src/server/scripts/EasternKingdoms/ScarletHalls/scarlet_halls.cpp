/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "InstanceScript.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Player.h"
#include "scarlet_halls.h"

enum Spells
{
    SPELL_HUNGRY_DOG          = 111582,
    SPELL_INQUISITION         = 111341,
    SPELL_SEAL_OF_BLOOD       = 111351,
    SPELL_RAZOR_EDGE          = 84146,
    SPELL_UNFLINCHING_RESOLVE = 111418,
    SPELL_HOLY_SMITE_N        = 9734,
    SPELL_HOLY_SMITE_H        = 128233,
    SPELL_HOLY_FIRE_N         = 17140,
    SPELL_HOLY_FIRE_H         = 128232,
    SPELL_FANATICISM          = 111376,
    SPELL_COMMANDING_SHOUT    = 80983,
    SPELL_DISARM              = 6713,
    SPELL_SHIELD_BARRIER      = 111324,
    SPELL_FIREBALL            = 128249,
    SPELL_FLAMESTRIKE         = 111667,
    SPELL_JOLT                = 114000,
    SPELL_SLASH_ARMOR         = 114011,
    SPELL_TOSS_STATUE         = 114022,
    SPELL_SCARLET_PROTECTION  = 114185,
    SPELL_FIREBALL_VALLEY     = 114182,
    SPELL_LEASHED             = 103169,
    SPELL_ARCHERY_VEHICLE     = 114877,
    SPELL_SUMMON_CANNONEER    = 113933,
    SPELL_FIRE_CANNON_DUMMY   = 113932,
    SPELL_CANNON_BLAST        = 113719,
    SPELL_PIERCING_ARROW      = 113564,
    SPELL_SHIELD_BREAKER      = 125492,
    SPELL_CARRYING_BUCKET     = 128164,
    SPELL_BEATDOWN            = 114184,
    SPELL_BLEEDING_WOUND      = 113855,
    SPELL_DOG_FOOD            = 111894,
    SPELL_EXPLODING_SHOT      = 113644,
    SPELL_DOG_LEAP            = 122929,
    SPELL_EATEN               = 122916,
    SPELL_PLAYER_VEHICLE_AURA = 113399,
};

enum Events
{
    EVENT_HUNGRY_DOG          = 1,
    EVENT_RAZOR_EDGE          = 2,
    EVENT_HOLY_SMITE          = 3,
    EVENT_HOLY_FIRE           = 4,
    EVENT_DISARM              = 5,
    EVENT_FIREBALL            = 6,
    EVENT_FLAMESTRIKE         = 7,
    EVENT_JOLT                = 8,
    EVENT_SLASH_ARMOR         = 9,
    EVENT_TOSS_STATUE         = 10,
    EVENT_SCARLET_PROTECTION  = 11,
    EVENT_FIREBALL_VALLEY     = 12,
    EVENT_CANNON_READY        = 13,
    EVENT_ESCORT              = 14,
    EVENT_FIRE_AT_LINE        = 15,
    EVENT_SHIELD_BREAKER      = 16,
    EVENT_BEATDOWN            = 17,
    EVENT_BLEEDING_WOUND      = 18,
    EVENT_EXPLODING_GROUND    = 19,
    EVENT_LEASHED             = 20,
    EVENT_FIRE_AT_ROW         = 21,
};

enum Weapons
{
    W_DRAGON_REACH            = 19353,
    W_REMORSE                 = 45142,
    W_MOULDERING_CORPSE       = 49793,
    W_MAGIC_SHIELD            = 45682,
};

// Starving Hound 58876
class npc_starving_hound : public CreatureScript
{
    public:
        npc_starving_hound() : CreatureScript("npc_starving_hound") { }

        struct npc_starving_houndAI : public ScriptedAI
        {
            npc_starving_houndAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;
            bool attacked;

            void Reset() OVERRIDE
            {
                events.Reset();
                attacked = false;
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_HUNGRY_DOG, urand(8000, 15000));
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                me->GetMap()->SetWorldState(WORLDSTATE_HUMANE_SOCIETY, 0);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                // Dog food on the watchman turns his hounds against him
                if (!attacked)
                {
                    Creature* owner = GetClosestCreatureWithEntry(me, NPC_VIGILANT_WATCHMAN, 30.0f, true);
                    if (owner && owner->HasAura(SPELL_DOG_FOOD))
                    {
                        attacked = true;
                        me->setFaction(15);
                        me->Attack(owner, true);
                    }
                }

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (events.ExecuteEvent() == EVENT_HUNGRY_DOG)
                    if (Unit* victim = me->GetVictim())
                        me->CastSpell(victim, SPELL_HUNGRY_DOG, false);

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_starving_houndAI(creature);
        }
};

// Angry Hound 58674
class npc_angry_hound : public CreatureScript
{
    public:
        npc_angry_hound() : CreatureScript("npc_angry_hound") { }

        struct npc_angry_houndAI : public ScriptedAI
        {
            npc_angry_houndAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap nonCombatEvents;
            bool AnyUnitAtStake;

            void Reset() OVERRIDE
            {
                Leash();
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                AnyUnitAtStake = false;
            }

            void InitializeAI() OVERRIDE
            {
                // The stake may not be loaded yet
                if (!Leash())
                    nonCombatEvents.ScheduleEvent(EVENT_LEASHED, 1000);

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                AnyUnitAtStake = false;
            }

            bool Leash()
            {
                if (Creature* stake = GetClosestCreatureWithEntry(me, NPC_STAKE, 20.0f, true))
                {
                    stake->CastSpell(me, SPELL_LEASHED, false);
                    return true;
                }

                return false;
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                me->GetMap()->SetWorldState(WORLDSTATE_HUMANE_SOCIETY, 0);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                nonCombatEvents.Update(diff);

                if (!AnyUnitAtStake)
                {
                    if (Player* player = me->FindNearestPlayer(10.0f))
                    {
                        if (me->GetDistance2d(player) < 4.0f)
                        {
                            AnyUnitAtStake = true;
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                            me->Attack(player, true);
                            me->GetMotionMaster()->MoveChase(player);
                        }
                    }
                }

                if (nonCombatEvents.ExecuteEvent() == EVENT_LEASHED)
                    if (!Leash())
                        nonCombatEvents.ScheduleEvent(EVENT_LEASHED, 1000);

                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
                EnterEvadeIfOutOfCombatArea(diff, 4.0f);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_angry_houndAI(creature);
        }
};

// Bucket of Meaty Dog Food 65379
class npc_bucket_of_meaty_dog_food : public CreatureScript
{
    public:
        npc_bucket_of_meaty_dog_food() : CreatureScript("npc_bucket_of_meaty_dog_food") { }

        struct npc_bucket_of_meaty_dog_foodAI : public ScriptedAI
        {
            npc_bucket_of_meaty_dog_foodAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
            }

            void OnSpellClick(Unit* clicker, bool& /*result*/) OVERRIDE
            {
                clicker->CastSpell(clicker, SPELL_CARRYING_BUCKET, false);
                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                me->DespawnOrUnsummon(1500);
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE { }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_bucket_of_meaty_dog_foodAI(creature);
        }
};

// Reinforced Archery Target 59163
class npc_reinforced_archery_target : public CreatureScript
{
    public:
        npc_reinforced_archery_target() : CreatureScript("npc_reinforced_archery_target") { }

        enum eTalks
        {
            TALK_INTRO = 1,
        };

        struct npc_reinforced_archery_targetAI : public ScriptedAI
        {
            npc_reinforced_archery_targetAI(Creature* creature) : ScriptedAI(creature) { }

            bool hasRider;

            void Reset() OVERRIDE
            {
                me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                DoCast(me, SPELL_ARCHERY_VEHICLE, true);
                hasRider = false;
            }

            void OnSpellClick(Unit* clicker, bool& /*result*/) OVERRIDE
            {
                if (clicker->HasAura(SPELL_HEROIC_DEFENSE) || hasRider)
                    return;

                hasRider = true;
                Talk(TALK_INTRO);
                DoCast(clicker, SPELL_HEROIC_DEFENSE);
                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                if (damage >= me->GetHealth())
                    damage = 0;
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE { }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_reinforced_archery_targetAI(creature);
        }
};

// Commander Lindon 59191
class npc_commander_lindon : public CreatureScript
{
    public:
        npc_commander_lindon() : CreatureScript("npc_commander_lindon") { }

        enum eTalks
        {
            TALK_INTRO       = 1,
            TALK_RANGE_REACH = 2,
            TALK_MELEE_REACH = 3,
        };

        struct npc_commander_lindonAI : public BossAI
        {
            npc_commander_lindonAI(Creature* creature) : BossAI(creature, DATA_COMANDER_LINDON) { }

            EventMap nonCombatEvents;
            bool Melee, initCombat, explosion;

            void InitializeAI() OVERRIDE
            {
                Melee      = false;
                initCombat = false;
                explosion  = false;
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            }

            void MoveInLineOfSight(Unit* who) OVERRIDE
            {
                if (!initCombat && who && who->GetTypeId() == TypeID::TYPEID_PLAYER)
                {
                    initCombat = true;
                    Talk(TALK_INTRO);
                    nonCombatEvents.ScheduleEvent(EVENT_FIRE_AT_LINE, urand(1000, 2000));
                    nonCombatEvents.ScheduleEvent(EVENT_SHIELD_BREAKER, urand(5000, 6000));
                    DoZoneInCombat();
                }

                BossAI::MoveInLineOfSight(who);
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                if (actionId == ACTION_GROUND_EXPLOSION && !explosion)
                {
                    explosion = true;
                    Talk(TALK_RANGE_REACH);
                    nonCombatEvents.ScheduleEvent(EVENT_EXPLODING_GROUND, 500);
                }
            }

            void Reset() OVERRIDE
            {
                _Reset();
                Melee = false;
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                BossAI::JustSummoned(summon);
                summon->SetDisplayId(summon->GetCreatureTemplate()->Modelid1);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                nonCombatEvents.Update(diff);

                while (uint32 eventId = nonCombatEvents.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_FIRE_AT_LINE:
                            SendArchersInCombat(ACTION_FIRE_AT_LINE);
                            break;
                        case EVENT_SHIELD_BREAKER:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, ShieldBreakerPredicate(me)))
                                me->CastSpell(target, SPELL_SHIELD_BREAKER, false);

                            nonCombatEvents.ScheduleEvent(EVENT_SHIELD_BREAKER, urand(2000, 3000));
                            break;
                        case EVENT_EXPLODING_GROUND:
                        {
                            Position const& startPos = urand(0, 1) ? ExplodingStartPos1 : ExplodingStartPos2;

                            for (uint8 i = 0; i < 3; i++)
                                me->CastSpell(startPos.GetPositionX() + i * 12.0f, startPos.GetPositionY(), startPos.GetPositionZ(), SPELL_EXPLODING_SHOT, true);
                            break;
                        }
                    }
                }

                if (!Melee)
                {
                    if (Player* player = me->FindNearestPlayer(10.0f))
                    {
                        Talk(TALK_MELEE_REACH);
                        nonCombatEvents.Reset();
                        Melee = true;
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                        AttackStart(player);
                        me->GetMotionMaster()->MoveChase(player);
                        SendArchersInCombat(ACTION_MELEE);
                        RemoveShieldFromPlayers();
                    }
                }

                if (!UpdateVictim())
                    return;

                events.Update(diff);

                DoMeleeAttackIfReady();
            }

        private:
            void SendArchersInCombat(uint32 action)
            {
                std::list<Creature*> archers;
                GetCreatureListWithEntryInGrid(archers, me, NPC_MASTER_ARCHER, 30.0f);

                for (Creature* archer : archers)
                    if (archer->IsAlive() && archer->AI())
                        archer->AI()->DoAction(action);
            }

            struct ShieldBreakerPredicate
            {
                public:
                    ShieldBreakerPredicate(Creature const* me) : _me(me) { }
                    bool operator()(Unit const* target) const
                    {
                        return target && target->GetTypeId() == TypeID::TYPEID_PLAYER && target->GetDistance2d(_me) < 18.0f;
                    }

                private:
                    Creature const* _me;
            };

            void RemoveShieldFromPlayers()
            {
                std::list<Player*> players;
                GetPlayerListInGrid(players, me, 100.0f);

                for (Player* player : players)
                    player->RemoveAurasDueToSpell(SPELL_HEROIC_DEFENSE);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_commander_lindonAI(creature);
        }
};

// Master Archer 59175
class npc_scarlet_master_archer : public CreatureScript
{
    public:
        npc_scarlet_master_archer() : CreatureScript("npc_scarlet_master_archer") { }

        struct npc_scarlet_master_archerAI : public ScriptedAI
        {
            npc_scarlet_master_archerAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events, nonCombatEvents;

            void Reset() OVERRIDE
            {
                nonCombatEvents.Reset();
                events.Reset();
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_BLEEDING_WOUND, urand(2500, 4500));
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                switch (actionId)
                {
                    case ACTION_FIRE_AT_LINE:
                        nonCombatEvents.ScheduleEvent(EVENT_FIRE_AT_ROW, urand(1000, 2000));
                        break;
                    case ACTION_MELEE:
                        if (Player* player = me->FindNearestPlayer(10.0f))
                        {
                            nonCombatEvents.Reset();
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                            AttackStart(player);
                            me->GetMotionMaster()->MoveChase(player);
                        }
                        break;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                nonCombatEvents.Update(diff);

                if (nonCombatEvents.ExecuteEvent() == EVENT_FIRE_AT_ROW)
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 60.0f, true))
                        me->CastSpell(target, SPELL_PIERCING_ARROW, false);

                    nonCombatEvents.ScheduleEvent(EVENT_FIRE_AT_ROW, urand(1000, 2500));
                }

                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (events.ExecuteEvent() == EVENT_BLEEDING_WOUND)
                {
                    if (Unit* victim = me->GetVictim())
                    {
                        if (me->GetDistance2d(victim) < 5.0f)
                            me->CastSpell(victim, SPELL_BLEEDING_WOUND, false);

                        events.ScheduleEvent(EVENT_BLEEDING_WOUND, urand(5500, 6500));
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_scarlet_master_archerAI(creature);
        }
};

// Scarlet Scourge Hewer 58684
class npc_scarlet_scourge_hewer : public CreatureScript
{
    public:
        npc_scarlet_scourge_hewer() : CreatureScript("npc_scarlet_scourge_hewer") { }

        struct npc_scarlet_scourge_hewerAI : public ScriptedAI
        {
            npc_scarlet_scourge_hewerAI(Creature* creature) : ScriptedAI(creature) { }

            bool triggered;

            void Reset() OVERRIDE
            {
                triggered = false;
                SetEquipmentSlots(false, W_DRAGON_REACH, 0, EQUIP_NO_CHANGE);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                me->CastSpell(me, SPELL_SEAL_OF_BLOOD, false);
            }

            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) OVERRIDE
            {
                if (HealthBelowPct(50) && !triggered)
                {
                    triggered = true;
                    me->CastSpell(me, SPELL_INQUISITION, false);
                }
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_scarlet_scourge_hewerAI(creature);
        }
};

// Scarlet Myrmidon 58683
class npc_scarlet_myrmidon : public CreatureScript
{
    public:
        npc_scarlet_myrmidon() : CreatureScript("npc_scarlet_myrmidon") { }

        struct npc_scarlet_myrmidonAI : public ScriptedAI
        {
            npc_scarlet_myrmidonAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;
            bool triggered;

            void Reset() OVERRIDE
            {
                triggered = false;
                events.Reset();
                SetEquipmentSlots(false, W_REMORSE, 0, EQUIP_NO_CHANGE);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_RAZOR_EDGE, urand(10000, 19000));
            }

            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) OVERRIDE
            {
                if (HealthBelowPct(60) && !triggered)
                {
                    triggered = true;
                    me->CastSpell(me, SPELL_UNFLINCHING_RESOLVE, false);
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (events.ExecuteEvent() == EVENT_RAZOR_EDGE)
                {
                    me->CastSpell(me, SPELL_RAZOR_EDGE, false);
                    events.ScheduleEvent(EVENT_RAZOR_EDGE, urand(10000, 19000));
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_scarlet_myrmidonAI(creature);
        }
};

// Scarlet Evangelist 58685
class npc_scarlet_evangelist : public CreatureScript
{
    public:
        npc_scarlet_evangelist() : CreatureScript("npc_scarlet_evangelist") { }

        struct npc_scarlet_evangelistAI : public ScriptedAI
        {
            npc_scarlet_evangelistAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;
            bool triggered;

            void Reset() OVERRIDE
            {
                triggered = false;
                events.Reset();
                SetEquipmentSlots(false, W_MOULDERING_CORPSE, 0, EQUIP_NO_CHANGE);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_HOLY_FIRE, urand(5000, 7000));
                events.ScheduleEvent(EVENT_HOLY_SMITE, urand(1500, 2500));
            }

            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) OVERRIDE
            {
                if (HealthBelowPct(60) && !triggered)
                {
                    triggered = true;
                    me->CastSpell(me, SPELL_FANATICISM, false);
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_HOLY_FIRE:
                            if (Unit* victim = me->GetVictim())
                                me->CastSpell(victim, IsHeroic() ? SPELL_HOLY_FIRE_H : SPELL_HOLY_FIRE_N, false);
                            events.ScheduleEvent(EVENT_HOLY_FIRE, urand(5000, 7000));
                            break;
                        case EVENT_HOLY_SMITE:
                            if (Unit* victim = me->GetVictim())
                                me->CastSpell(victim, IsHeroic() ? SPELL_HOLY_SMITE_H : SPELL_HOLY_SMITE_N, false);
                            events.ScheduleEvent(EVENT_HOLY_SMITE, urand(1500, 2500));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_scarlet_evangelistAI(creature);
        }
};

// Scarlet Cannoneer 59293
class npc_scarlet_cannoneer : public CreatureScript
{
    public:
        npc_scarlet_cannoneer() : CreatureScript("npc_scarlet_cannoneer") { }

        struct npc_scarlet_cannoneerAI : public ScriptedAI
        {
            npc_scarlet_cannoneerAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;
            bool triggered = false;
            uint64 CannonGuid = 0;

            void IsSummonedBy(Unit* summoner) OVERRIDE
            {
                if (!IsHeroic())
                {
                    me->SetLevel(28);
                    me->SetMaxHealth(1890);
                    me->SetHealth(1890);
                }

                if (summoner)
                    CannonGuid = summoner->GetGUID();

                triggered = false;
            }

            void Reset() OVERRIDE
            {
                me->SetReactState(REACT_PASSIVE);
                events.Reset();
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (!triggered)
                {
                    if (Player* player = me->FindNearestPlayer(20.0f))
                    {
                        triggered = true;
                        events.ScheduleEvent(EVENT_CANNON_READY, urand(1500, 4000));
                        me->Attack(player, false);
                    }
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (!UpdateVictim())
                    return;

                if (events.ExecuteEvent() == EVENT_CANNON_READY)
                {
                    me->CastSpell(me, SPELL_FIRE_CANNON_DUMMY, true);

                    if (Creature* cannonStalker = ObjectAccessor::GetCreature(*me, CannonGuid))
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 30.0f, true))
                            cannonStalker->CastSpell(target, SPELL_CANNON_BLAST, true);

                    events.ScheduleEvent(EVENT_CANNON_READY, urand(1500, 4000));
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_scarlet_cannoneerAI(creature);
        }
};

// Scarlet Cannon stalker 58726
class npc_cannon_stalker : public CreatureScript
{
    public:
        npc_cannon_stalker() : CreatureScript("npc_cannon_stalker") { }

        struct npc_cannon_stalkerAI : public ScriptedAI
        {
            npc_cannon_stalkerAI(Creature* creature) : ScriptedAI(creature) { }

            void InitializeAI() OVERRIDE
            {
                me->CastSpell(me, SPELL_SUMMON_CANNONEER, false);
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE { }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_cannon_stalkerAI(creature);
        }
};

// Scarlet Defender 58676
class npc_scarlet_halls_defender : public CreatureScript
{
    public:
        npc_scarlet_halls_defender() : CreatureScript("npc_scarlet_halls_defender") { }

        struct npc_scarlet_halls_defenderAI : public ScriptedAI
        {
            npc_scarlet_halls_defenderAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;
            bool triggered;

            void InitializeAI() OVERRIDE
            {
                SetEquipmentSlots(false, W_MOULDERING_CORPSE, 0, EQUIP_NO_CHANGE);
                Reset();
            }

            void Reset() OVERRIDE
            {
                triggered = false;
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_DISARM, urand(9000, 18000));
                me->CastSpell(me, SPELL_COMMANDING_SHOUT, false);
            }

            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) OVERRIDE
            {
                if (HealthBelowPct(60) && !triggered)
                {
                    triggered = true;
                    me->CastSpell(me, SPELL_SHIELD_BARRIER, false);
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (events.ExecuteEvent() == EVENT_DISARM)
                    if (Unit* victim = me->GetVictim())
                        me->CastSpell(victim, SPELL_DISARM, false);

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_scarlet_halls_defenderAI(creature);
        }
};

// Scarlet Evoker 58756
class npc_scarlet_evoker : public CreatureScript
{
    public:
        npc_scarlet_evoker() : CreatureScript("npc_scarlet_evoker") { }

        struct npc_scarlet_evokerAI : public ScriptedAI
        {
            npc_scarlet_evokerAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                SetEquipmentSlots(false, W_MOULDERING_CORPSE, 0, EQUIP_NO_CHANGE);
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_FIREBALL, urand(3000, 5000));
                events.ScheduleEvent(EVENT_FLAMESTRIKE, urand(7000, 15500));
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_FLAMESTRIKE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 150.0f, true))
                                me->CastSpell(target, SPELL_FLAMESTRIKE, false);
                            events.ScheduleEvent(EVENT_FLAMESTRIKE, urand(7000, 15500));
                            break;
                        case EVENT_FIREBALL:
                            if (Unit* victim = me->GetVictim())
                                me->CastSpell(victim, SPELL_FIREBALL, false);
                            events.ScheduleEvent(EVENT_FIREBALL, urand(3000, 5000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_scarlet_evokerAI(creature);
        }
};

// Scarlet Hall Guardian 59240
class npc_scarlet_hall_guardian : public CreatureScript
{
    public:
        npc_scarlet_hall_guardian() : CreatureScript("npc_scarlet_hall_guardian") { }

        struct npc_scarlet_hall_guardianAI : public ScriptedAI
        {
            npc_scarlet_hall_guardianAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();
                SetEquipmentSlots(false, W_REMORSE, W_MAGIC_SHIELD, EQUIP_NO_CHANGE);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_JOLT, urand(1500, 3000));
                events.ScheduleEvent(EVENT_SLASH_ARMOR, urand(7000, 14000));
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_JOLT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 150.0f, true))
                                me->CastSpell(target, SPELL_JOLT, false);
                            events.ScheduleEvent(EVENT_JOLT, urand(11000, 17000));
                            break;
                        case EVENT_SLASH_ARMOR:
                            if (Unit* victim = me->GetVictim())
                                me->CastSpell(victim, SPELL_SLASH_ARMOR, false);
                            events.ScheduleEvent(EVENT_SLASH_ARMOR, urand(7000, 14000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_scarlet_hall_guardianAI(creature);
        }
};

// Scarlet Treasurer 59241
class npc_scarlet_treasurer : public CreatureScript
{
    public:
        npc_scarlet_treasurer() : CreatureScript("npc_scarlet_treasurer") { }

        struct npc_scarlet_treasurerAI : public ScriptedAI
        {
            npc_scarlet_treasurerAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_TOSS_STATUE, urand(3 * IN_MILLISECONDS, 12 * IN_MILLISECONDS));
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                me->SummonGameObject(GO_GOLD_COINS, me->GetPositionX() + frand(-3.0f, 3.0f), me->GetPositionY() + frand(-1.9f, 1.9f), me->GetPositionZ(), me->GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f, 0);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (events.ExecuteEvent() == EVENT_TOSS_STATUE)
                {
                    if (Unit* victim = me->GetVictim())
                        DoCast(victim, SPELL_TOSS_STATUE);

                    events.ScheduleEvent(EVENT_TOSS_STATUE, urand(5 * IN_MILLISECONDS, 12 * IN_MILLISECONDS));
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_scarlet_treasurerAI(creature);
        }
};

// Hooded Crusader 64738
class npc_scarlet_hooded_crusader : public CreatureScript
{
    public:
        npc_scarlet_hooded_crusader() : CreatureScript("npc_scarlet_hooded_crusader") { }

        struct npc_scarlet_hooded_crusaderAI : public ScriptedAI
        {
            npc_scarlet_hooded_crusaderAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap nonCombatEvents;
            uint32 wp;

            void DoAction(int32 actionId) OVERRIDE
            {
                if (actionId == ACTION_KODEX)
                    nonCombatEvents.ScheduleEvent(EVENT_ESCORT, urand(100, 200));
            }

            void InitializeAI() OVERRIDE
            {
                wp = 0;
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                wp++;

                switch (pointId)
                {
                    case 0:
                        Talk(TALK_INTRO_H);
                        nonCombatEvents.ScheduleEvent(EVENT_ESCORT, 6000);
                        break;
                    case 1:
                        me->GetMotionMaster()->MovementExpired();
                        me->GetMotionMaster()->Clear();
                        Talk(TALK_CODEX);
                        break;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                nonCombatEvents.Update(diff);

                if (nonCombatEvents.ExecuteEvent() == EVENT_ESCORT && wp < 2)
                {
                    me->GetMotionMaster()->MovementExpired();
                    me->GetMotionMaster()->MovePoint(wp, HoodedCrusaderWp[wp]);
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_scarlet_hooded_crusaderAI(creature);
        }
};

// Scarlet Scholar 59372
class npc_scarlet_scholar : public CreatureScript
{
    public:
        npc_scarlet_scholar() : CreatureScript("npc_scarlet_scholar") { }

        struct npc_scarlet_scholarAI : public ScriptedAI
        {
            npc_scarlet_scholarAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();
                SetEquipmentSlots(false, W_MOULDERING_CORPSE, 0, EQUIP_NO_CHANGE);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_SCARLET_PROTECTION, urand(3000, 8000));
                events.ScheduleEvent(EVENT_FIREBALL_VALLEY, urand(9000, 18000));
                events.ScheduleEvent(EVENT_FIREBALL, 1000);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SCARLET_PROTECTION:
                            me->InterruptNonMeleeSpells(true);
                            DoCast(me, SPELL_SCARLET_PROTECTION);
                            events.ScheduleEvent(EVENT_SCARLET_PROTECTION, urand(34 * IN_MILLISECONDS, 65 * IN_MILLISECONDS));
                            break;
                        case EVENT_FIREBALL:
                            if (Unit* victim = me->GetVictim())
                                me->CastSpell(victim, SPELL_FIREBALL, false);

                            events.ScheduleEvent(EVENT_FIREBALL, urand(3000, 6500));
                            break;
                        case EVENT_FIREBALL_VALLEY:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 150.0f, true))
                                me->CastSpell(target, SPELL_FIREBALL_VALLEY, false);

                            events.ScheduleEvent(EVENT_FIREBALL_VALLEY, urand(9000, 18000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_scarlet_scholarAI(creature);
        }
};

// Scarlet Pupil 59373
class npc_scarlet_pupil : public CreatureScript
{
    public:
        npc_scarlet_pupil() : CreatureScript("npc_scarlet_pupil") { }

        struct npc_scarlet_pupilAI : public ScriptedAI
        {
            npc_scarlet_pupilAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_BEATDOWN, urand(2500, 6000));
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (events.ExecuteEvent() == EVENT_BEATDOWN)
                {
                    if (Unit* victim = me->GetVictim())
                        me->CastSpell(victim, SPELL_BEATDOWN, false);

                    events.ScheduleEvent(EVENT_BEATDOWN, urand(5000, 6500));
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_scarlet_pupilAI(creature);
        }
};

// Throw Bucket of Meaty Dog Food 113029
class spell_throw_bucket_meaty_dog : public SpellScriptLoader
{
    public:
        spell_throw_bucket_meaty_dog() : SpellScriptLoader("spell_throw_bucket_meaty_dog") { }

        class spell_throw_bucket_meaty_dog_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_throw_bucket_meaty_dog_SpellScript);

            SpellCastResult CheckCast()
            {
                Unit* caster = GetCaster();

                if (caster->IsInCombat())
                {
                    caster->RemoveAura(SPELL_CARRYING_BUCKET);
                    return SpellCastResult::SPELL_FAILED_AFFECTING_COMBAT;
                }

                return SpellCastResult::SPELL_CAST_OK;
            }

            void Register() OVERRIDE
            {
                OnCheckCast += SpellCheckCastFn(spell_throw_bucket_meaty_dog_SpellScript::CheckCast);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_throw_bucket_meaty_dog_SpellScript();
        }
};

// Dog Food 111894
class spell_scarlet_halls_dog_food : public SpellScriptLoader
{
    public:
        spell_scarlet_halls_dog_food() : SpellScriptLoader("spell_scarlet_halls_dog_food") { }

        class spell_scarlet_halls_dog_food_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_scarlet_halls_dog_food_SpellScript);

            void CheckTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if([](WorldObject* object)
                {
                    return !object->ToCreature() || object->ToCreature()->GetEntry() != NPC_VIGILANT_WATCHMAN;
                });
            }

            void HandleHitEffect(SpellEffIndex /*effIndex*/)
            {
                Creature* target = GetHitUnit() ? GetHitUnit()->ToCreature() : NULL;
                if (!target)
                    return;

                target->StopMoving();
                target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                target->AddAura(SPELL_EATEN, target);

                std::list<Creature*> hounds;
                GetCreatureListWithEntryInGrid(hounds, target, NPC_STARVING_HOUND, 10.0f);

                for (Creature* hound : hounds)
                {
                    hound->StopMoving();
                    hound->CastSpell(target, SPELL_DOG_LEAP, false);
                    hound->HandleEmoteStateCommand(EMOTE_STATE_EAT);
                }
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_scarlet_halls_dog_food_SpellScript::CheckTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
                OnEffectHitTarget += SpellEffectFn(spell_scarlet_halls_dog_food_SpellScript::HandleHitEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_scarlet_halls_dog_food_SpellScript();
        }
};

// Archery Target Vehicle Override 114877
class spell_scarlet_halls_archery_target_vehicle_override : public AuraScript
{
    PrepareAuraScript(spell_scarlet_halls_archery_target_vehicle_override);

    void OnPeriodic(AuraEffect const* /*aurEff*/)
    {
        // The periodic trigger isn't cast by the aura itself
        if (Unit* owner = GetOwner()->ToUnit())
            owner->CastSpell(owner, SPELL_PLAYER_VEHICLE_AURA, true);
    }

    void Register() OVERRIDE
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_scarlet_halls_archery_target_vehicle_override::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};

// Archery Target Vehicle Override 113399
class spell_scarlet_halls_archery_target_vehicle_override_eff : public SpellScript
{
    PrepareSpellScript(spell_scarlet_halls_archery_target_vehicle_override_eff);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();
        uint32 spellId = GetSpellInfo()->Id;
        targets.remove_if([caster, spellId](WorldObject* target)
        {
            Player* player = target->ToPlayer();
            return player && caster && player->HasAura(spellId) && !player->HasAura(spellId, caster->GetGUID());
        });
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_scarlet_halls_archery_target_vehicle_override_eff::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
    }
};

// Heroic Defense 113436
class spell_scarlet_halls_heroic_defense : public AuraScript
{
    PrepareAuraScript(spell_scarlet_halls_heroic_defense);

    void HandleOnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* caster = GetCaster())
            if (Creature* target = caster->ToCreature())
                target->DespawnOrUnsummon();
    }

    void Register() OVERRIDE
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_scarlet_halls_heroic_defense::HandleOnRemove, EFFECT_0, SPELL_AURA_CONTROL_VEHICLE, AURA_EFFECT_HANDLE_REAL);
    }
};

// AreaTrigger 7532
class AreaTrigger_at_commander_lindon : public AreaTriggerScript
{
    public:
        AreaTrigger_at_commander_lindon() : AreaTriggerScript("AreaTrigger_at_commander_lindon") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/) OVERRIDE
        {
            if (InstanceScript* instance = player->GetInstanceScript())
                if (Creature* lindon = ObjectAccessor::GetCreature(*player, instance->GetData64(DATA_COMANDER_LINDON)))
                    if (lindon->AI())
                        lindon->AI()->DoAction(ACTION_GROUND_EXPLOSION);

            if (player->HasAura(SPELL_HEROIC_DEFENSE) && player->GetPositionX() > 950.0f)
                player->RemoveAurasDueToSpell(SPELL_HEROIC_DEFENSE);

            return false;
        }
};

void AddSC_scarlet_halls()
{
    new npc_starving_hound();
    new npc_angry_hound();
    new npc_bucket_of_meaty_dog_food();
    new npc_reinforced_archery_target();
    new npc_commander_lindon();
    new npc_scarlet_master_archer();
    new npc_scarlet_scourge_hewer();
    new npc_scarlet_myrmidon();
    new npc_scarlet_evangelist();
    new npc_scarlet_cannoneer();
    new npc_cannon_stalker();
    new npc_scarlet_halls_defender();
    new npc_scarlet_evoker();
    new npc_scarlet_hall_guardian();
    new npc_scarlet_treasurer();
    new npc_scarlet_scholar();
    new npc_scarlet_hooded_crusader();
    new npc_scarlet_pupil();
    new spell_throw_bucket_meaty_dog();
    new spell_scarlet_halls_dog_food();
    RegisterAuraScript(spell_scarlet_halls_archery_target_vehicle_override);
    RegisterSpellScript(spell_scarlet_halls_archery_target_vehicle_override_eff);
    RegisterAuraScript(spell_scarlet_halls_heroic_defense);
    new AreaTrigger_at_commander_lindon();
}
