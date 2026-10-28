/*
* This file is part of the Legends of Azeroth Pandaria Project. See THANKS file for Copyright information
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
#include "SpellScript.h"
#include "InstanceScript.h"
#include "Player.h"
#include "ObjectAccessor.h"
#include "scarlet_halls.h"

enum EntranceSpells
{
    SPELL_HUNGRY_DOG = 111582, SPELL_LEASHED = 103169,
    SPELL_ARCHERY_VEHICLE = 114877,
    SPELL_PIERCING_ARROW = 113564, SPELL_SHIELD_BREAKER = 125492,
    SPELL_CARRYING_BUCKET = 128164, SPELL_BLEEDING_WOUND = 113855,
    SPELL_DOG_FOOD = 111894, SPELL_EXPLODING_SHOT = 113644,
    SPELL_DOG_LEAP = 122929, SPELL_EATEN = 122916,
    SPELL_PLAYER_VEHICLE_AURA = 113399, SPELL_HEROIC_DEFENSE = 113436
};
enum EntranceEvents
{
    EVENT_HUNGRY_DOG = 1, EVENT_FIRE_AT_LINE,
    EVENT_SHIELD_BREAKER, EVENT_EXPLODING_GROUND
};
enum EntranceActions { ACTION_FIRE_AT_LINE = 1, ACTION_MELEE, ACTION_GROUND_EXPLOSION };
enum EntranceCreatures { NPC_MASTER_ARCHER = 59175, NPC_STAKE = 58701,
    NPC_VIGILANT_WATCHMAN = 58898, NPC_STARVING_HOUND = 58876 };
const Position ExplodingStartPos1 = { 905.77f, 534.20f, 12.90f, 6.23f };
const Position ExplodingStartPos2 = { 906.47f, 547.90f, 12.92f, 6.23f };

// Starving hound 58876
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

                if (CanAttackMyOwner() && !attacked)
                {
                    attacked = true;
                    me->setFaction(15);

                    if (Unit* owner = GetClosestCreatureWithEntry(me, NPC_VIGILANT_WATCHMAN, 30.0f, true))
                        me->Attack(owner, true);
                }

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    if (eventId == EVENT_HUNGRY_DOG)
                    {
                        if (Unit* Vict = me->GetVictim())
                            me->CastSpell(Vict, SPELL_HUNGRY_DOG, false);
                    }
                    break;
                }

                DoMeleeAttackIfReady();
            }

        private:
            bool CanAttackMyOwner()
            {
                if (Unit* owner = GetClosestCreatureWithEntry(me, NPC_VIGILANT_WATCHMAN, 30.0f, true))
                    if (owner->HasAura(SPELL_DOG_FOOD))
                        return true;

                return false;
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

        enum eEvents
        {
            EVENT_LEASHED = 1,
        };

        struct npc_angry_houndAI : public ScriptedAI
        {
            npc_angry_houndAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap nonCombatEvents;
            bool AnyUnitAtStake;

            void Reset() OVERRIDE
            {
                if (Creature* stake = GetClosestCreatureWithEntry(me, NPC_STAKE, 20.0f, true))
                    stake->CastSpell(me, SPELL_LEASHED, false);

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                AnyUnitAtStake = false;
            }

            void InitializeAI() OVERRIDE
            {
                if (Creature* stake = GetClosestCreatureWithEntry(me, NPC_STAKE, 20.0f, true)) // we need solve problem when creature create is comes later then we are.
                    stake->CastSpell(me, SPELL_LEASHED, false);
                else nonCombatEvents.ScheduleEvent(EVENT_LEASHED, 1000);

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                AnyUnitAtStake = false;
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                me->GetMap()->SetWorldState(WORLDSTATE_HUMANE_SOCIETY, 0);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                nonCombatEvents.Update(diff);

                if (!AnyUnitAtStake)
                    if (Player* itr = me->FindNearestPlayer(10.0f))
                    {
                        if (me->GetDistance2d(itr) < 4.0f)
                        {
                            AnyUnitAtStake = true;
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                            me->Attack(itr, true);
                            me->GetMotionMaster()->MoveChase(itr);
                        }
                    }

                while (uint32 eventId = nonCombatEvents.ExecuteEvent())
                {
                    if (eventId == EVENT_LEASHED)
                    {
                        if (Creature* stake = GetClosestCreatureWithEntry(me, NPC_STAKE, 20.0f, true)) // till creature not`ll load
                            stake->CastSpell(me, SPELL_LEASHED, false);

                        else nonCombatEvents.ScheduleEvent(EVENT_LEASHED, 1000);
                    }
                    break;
                }

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
            npc_bucket_of_meaty_dog_foodAI(Creature* creature) : ScriptedAI(creature), _instance(creature->GetInstanceScript()) { }

            EventMap events;

            void IsSummonedBy(Unit* summoner) OVERRIDE { }

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

            void UpdateAI(uint32 diff) OVERRIDE { }

            private:
                InstanceScript* _instance;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_bucket_of_meaty_dog_foodAI(creature);
        }
};

// Reinforced  archery target 59163
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

        enum eWeapon
        {
            WEAP_BLOW  = 55306,
            WEAP_SWORD = 56696,
        };

        enum eTalks
        {
            TALK_INTRO       = 1,
            TALK_RANGE_REACH = 2,
            TALK_MELEE_REACH = 3,
        };

        struct npc_commander_lindonAI : public ScriptedAI
        {
            npc_commander_lindonAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript()) { }

            InstanceScript* instance;

            EventMap nonCombatEvents;
            bool Melee, initCombat, explosion;

            void InitializeAI() OVERRIDE
            {
                Reset();
            }

            void MoveInLineOfSight(Unit* who) OVERRIDE
            {
                if (!initCombat && me->IsAlive() && who && who->IsAlive() && who->GetTypeId() == TypeID::TYPEID_PLAYER)
                {
                    initCombat = true;
                    Talk(TALK_INTRO);
                    nonCombatEvents.ScheduleEvent(EVENT_FIRE_AT_LINE, urand(1000, 2000));
                    nonCombatEvents.ScheduleEvent(EVENT_SHIELD_BREAKER, urand(5000, 6000));
                    DoZoneInCombat();
                }
                ScriptedAI::MoveInLineOfSight(who);
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
                nonCombatEvents.Reset();
                Melee = false;
                initCombat = false;
                explosion = false;
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                if (instance && instance->GetData(DATA_COMANDER_LINDON) != DONE)
                    instance->SetData(DATA_COMANDER_LINDON, NOT_STARTED);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                HandleDoors();
                if (instance)
                    instance->SetData(DATA_COMANDER_LINDON, IN_PROGRESS);
            }

            void HandleDoors(bool reset = false)
            {
                if (instance)
                    if (GameObject* Idoor = GetClosestGameObjectWithEntry(me, GO_COMANDER_LINDON_EXIT, 150.0f))
                        instance->HandleGameObject(0, reset, Idoor);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                nonCombatEvents.Reset();
                if (instance)
                    instance->SetData(DATA_COMANDER_LINDON, DONE);

                HandleDoors(true);
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
                            float itter = 0.0f;
                            const Position currentPos = urand(0, 1) ? ExplodingStartPos1 : ExplodingStartPos2;

                            for (uint8 i = 0; i < 3; i++) // 3 pos for fire
                            {
                                me->CastSpell(currentPos.GetPositionX() + itter, currentPos.GetPositionY(), currentPos.GetPositionZ(), SPELL_EXPLODING_SHOT, true);
                                itter += 12.0f;
                            }
                            break;
                    }
                }

                if (!Melee)
                    if (Player* itr = me->FindNearestPlayer(10.0f))
                    {
                        Talk(TALK_MELEE_REACH);
                        nonCombatEvents.Reset();
                        Melee = true;
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                        me->AI()->AttackStart(itr);
                        me->GetMotionMaster()->MoveChase(itr);
                        SendArchersInCombat(ACTION_MELEE);
                        RemoveShieldFromPlayers();
                    }

                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }

        private:
            void SendArchersInCombat(uint32 action)
            {
                std::list<Creature*> Archers;
                GetCreatureListWithEntryInGrid(Archers, me, NPC_MASTER_ARCHER, 30.0f);

                for (auto&& itr : Archers)
                    if (itr->IsAlive())
                        itr->AI()->DoAction(action);
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
                std::list<Player*> PlayersWithShield;
                GetPlayerListInGrid(PlayersWithShield, me, 100.0f);

                for (auto&& itr : PlayersWithShield)
                    if (itr->HasAura(SPELL_HEROIC_DEFENSE))
                        itr->RemoveAura(SPELL_HEROIC_DEFENSE);
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

        enum eWeapon
        {
            WEAP_BLOW = 55306,
            WEAP_SWORD = 56696,
        };

        enum eEvents
        {
            EVENT_FIRE_AT_ROW    = 1,
            EVENT_BLEEDING_WOUND = 2,
        };

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
                    {
                        if (Player* itr = me->FindNearestPlayer(10.0f))
                        {
                            nonCombatEvents.Reset();
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                            me->AI()->AttackStart(itr);
                            me->GetMotionMaster()->MoveChase(itr);
                        }
                    }
                    break;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                nonCombatEvents.Update(diff);

                while (uint32 eventId = nonCombatEvents.ExecuteEvent())
                {
                    if (eventId == EVENT_FIRE_AT_ROW)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 60.0f, true))
                            me->CastSpell(target, SPELL_PIERCING_ARROW, false);

                        nonCombatEvents.ScheduleEvent(EVENT_FIRE_AT_ROW, urand(1000, 2500));
                    }
                    break;
                }

                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    if (eventId == EVENT_BLEEDING_WOUND)
                    {
                        if (Unit* vict = me->GetVictim())
                        {
                            if (me->GetDistance2d(vict) < 5.0f)
                                me->CastSpell(vict, SPELL_BLEEDING_WOUND, false);

                            events.ScheduleEvent(EVENT_BLEEDING_WOUND, urand(5500, 6500));
                        }
                    }
                    break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_scarlet_master_archerAI(creature);
        }
};

// Throw bucket meaty dog 113029
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

class EatenPredicate
{
    public:
        EatenPredicate() { }

        bool operator()(WorldObject* object)
        {
            return object && object->ToCreature() && object->ToCreature()->GetEntry() != NPC_VIGILANT_WATCHMAN;
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
                targets.remove_if(EatenPredicate());
            }

            void HandleHitEffect(SpellEffIndex /*effIndex*/)
            {
                if (Creature* target = GetHitCreature())
                {
                    target->StopMoving();
                    target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                    target->AddAura(SPELL_EATEN, target);

                    std::list<Creature*> Hounds;
                    GetCreatureListWithEntryInGrid(Hounds, target, NPC_STARVING_HOUND, 10.0f);

                    for (auto&& sHound : Hounds)
                    {
                        sHound->StopMoving();
                        sHound->CastSpell(target, SPELL_DOG_LEAP, false);
                        sHound->HandleEmoteStateCommand(EMOTE_STATE_EAT);
                    }
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
        if (Unit* owner = GetOwner()->ToUnit()) // not triggering by default for unk reason...
            owner->CastSpell(owner, SPELL_PLAYER_VEHICLE_AURA, true);
    }

    void Register() OVERRIDE
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_scarlet_halls_archery_target_vehicle_override::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};

// Archery Target Vehicle Override Eff 113399
class spell_scarlet_halls_archery_target_vehicle_override_eff : public SpellScript
{
    PrepareSpellScript(spell_scarlet_halls_archery_target_vehicle_override_eff);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        targets.remove_if([=](WorldObject* target) { return target && target->ToPlayer() && GetCaster() && target->ToPlayer()->HasAura(GetSpellInfo()->Id) && !target->ToPlayer()->HasAura(GetSpellInfo()->Id, GetCaster()->GetGUID()); });
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

    void HandleOnRemove(AuraEffect const* /*aureff*/, AuraEffectHandleModes /*mode*/)
    {
        if (GetCaster() && GetCaster()->ToCreature())
            GetCaster()->ToCreature()->DespawnOrUnsummon();
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

        bool OnTrigger(Player* player, AreaTriggerEntry const* trigger) OVERRIDE
        {
            if (Creature* lindon = ObjectAccessor::GetCreature(*player, player->GetInstanceScript() ? player->GetInstanceScript()->GetData64(DATA_COMANDER_LINDON) : 0))
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
    new spell_throw_bucket_meaty_dog();
    new spell_scarlet_halls_dog_food();
    RegisterAuraScript(spell_scarlet_halls_archery_target_vehicle_override);
    RegisterSpellScript(spell_scarlet_halls_archery_target_vehicle_override_eff);
    RegisterAuraScript(spell_scarlet_halls_heroic_defense);
    new AreaTrigger_at_commander_lindon();
}
