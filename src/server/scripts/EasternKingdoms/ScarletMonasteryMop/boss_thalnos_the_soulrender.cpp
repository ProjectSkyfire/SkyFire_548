/*
 * Ported from Legends of Azeroth 5.4.8.
 * Authors: lee, Jason Dove, leelf00, Kulyadev
 * See LICENSE.md for copyright information.
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

#include "AreaTrigger.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "SpellScript.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "scarlet_monastery_mop.h"
#include <set>

enum Spells
{
    // Thalnos
    SPELL_CHANNEL_SOUL              = 60309, // visual to trigger
    SPELL_RITUAL_OF_SOULD           = 60429, // only aura
    SPELL_RAISE_FALLEN_CRUSADER     = 115139,
    SPELL_SPIRIT_GALE               = 115289,
    SPELL_EVICT_SOUL                = 115297,
    SPELL_SUMMON_EMPOWERING_SPIRITS = 115147,
    SPELL_SPIRIT_GALE_AURA          = 115291, // in area trigger

    // Minions
    SPELL_MIND_ROT                  = 115144,
    SPELL_CLONE_ME                  = 45204, // get ur vision
};

enum ThalnosData
{
    WIPE_CHECK_INTERVAL             = 2000
};

enum Events
{
    EVENT_SPIRIT_GALE               = 1,
    EVENT_RAISE_FALLEN_CRUSADER     = 2,
    EVENT_EVICT_SOUL                = 3,
    EVENT_EMPOWERING_SPIRITS        = 5
};

enum Yells
{
    TALK_INTO                       = 0,
    TALK_SLAY                       = 1,
    TALK_EVICT                      = 2,
    TALK_SPIRITS                    = 3,
    TALK_DEATH                      = 4,
    TALK_EVICTED_SOUL_ANN           = 5,
    TALK_EMPOWERED_SPIRIT_ANN       = 6,
};

class boss_thalnos_the_soulrender : public CreatureScript
{
    public:
        boss_thalnos_the_soulrender() : CreatureScript("boss_thalnos_the_soulrender") { }

        struct boss_thalnos_the_soulrenderAI : public BossAI
        {
            boss_thalnos_the_soulrenderAI(Creature* creature) : BossAI(creature, BOSS_THALNOS_THE_SOULRENDER) { }

            uint64 victimGUID;
            std::set<uint64> livingZombies;
            bool engaged;
            uint32 wipeCheckTimer;

            void InitializeAI() OVERRIDE
            {
                me->setActive(true);
                Reset();
            }

            void Reset() OVERRIDE
            {
                _Reset();
                DespawnSummoned();
                events.Reset();
                me->setRegeneratingHealth(true);
                me->SetReactState(REACT_AGGRESSIVE);
                victimGUID = 0;
                livingZombies.clear();
                engaged = false;
                wipeCheckTimer = WIPE_CHECK_INTERVAL;
                me->GetMap()->SetWorldState(WORLDSTATE_EMPOWERED_SPIRITUALIST, 0);

                if (instance)
                {
                    me->RemoveAllAreasTrigger();
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SPIRIT_GALE_AURA);
                }
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
                DespawnSummoned();
                Talk(TALK_DEATH);
                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    me->RemoveAllAreasTrigger();
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SPIRIT_GALE_AURA);
                }
            }

            void DespawnSummoned()
            {
                std::list<Creature*> NpcList;
                GetCreatureListWithEntryInGrid(NpcList, me, NPC_FALLEN_CRUSADER, 200.0f);
                GetCreatureListWithEntryInGrid(NpcList, me, NPC_EMPOWERED_ZOMBIE, 200.0f);
                GetCreatureListWithEntryInGrid(NpcList, me, NPC_EVICTED_SOUL, 200.0f);
                GetCreatureListWithEntryInGrid(NpcList, me, NPC_EMPOWERING_SPIRIT, 200.0f);
                for (auto&& itr : NpcList)
                    itr->DespawnOrUnsummon();
            }

            void EnterEvadeMode() OVERRIDE
            {
                BossAI::EnterEvadeMode();
                DespawnSummoned();
                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    me->RemoveAllAreasTrigger();
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SPIRIT_GALE_AURA);
                }
                summons.DespawnAll();
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                if (summon->GetEntry() == NPC_EMPOWERED_ZOMBIE)
                {
                    livingZombies.insert(summon->GetGUID());
                    if (livingZombies.size() >= 3)
                        me->GetMap()->SetWorldState(WORLDSTATE_EMPOWERED_SPIRITUALIST, 1);
                }

                BossAI::JustSummoned(summon);
                summon->setActive(true);
                summon->SetInCombatWithZone();
            }

            void SummonedCreatureDies(Creature* summon, Unit* /*killer*/) OVERRIDE
            {
                if (summon->GetEntry() == NPC_EMPOWERED_ZOMBIE)
                {
                    livingZombies.erase(summon->GetGUID());
                    if (livingZombies.size() < 3)
                        me->GetMap()->SetWorldState(WORLDSTATE_EMPOWERED_SPIRITUALIST, 0);
                }
            }

            void SummonedCreatureDespawn(Creature* summon) OVERRIDE
            {
                if (summon->GetEntry() == NPC_EMPOWERED_ZOMBIE && me->IsAlive())
                {
                    livingZombies.erase(summon->GetGUID());
                    if (livingZombies.size() < 3)
                        me->GetMap()->SetWorldState(WORLDSTATE_EMPOWERED_SPIRITUALIST, 0);
                }
                BossAI::SummonedCreatureDespawn(summon);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();
                engaged = true;
                Talk(TALK_INTO);
                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                }
                events.ScheduleEvent(EVENT_SPIRIT_GALE, 1500);
                events.ScheduleEvent(EVENT_RAISE_FALLEN_CRUSADER, urand(6500, 7000));
                events.ScheduleEvent(EVENT_EMPOWERING_SPIRITS, urand(40000, 42000));
                events.ScheduleEvent(EVENT_EVICT_SOUL, urand(25000, 26000));
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                if (actionId == ACTION_REMOVE_RAISE)
                    me->RemoveChanneledCast(victimGUID);
            }

            void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
            {
                // Nothing in this fight is meant to hurt Thalnos except the group
                if (attacker == me)
                    damage = 0;
            }

            // True while somebody who could still fight him is alive in the dungeon
            bool HasLivingPlayer() const
            {
                for (MapReference const& ref : me->GetMap()->GetPlayers())
                    if (Player* player = ref.GetSource())
                        if (player->IsAlive() && !player->IsGameMaster())
                            return true;
                return false;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                // After a wipe a stray attacker, such as a pet of a dead player, can keep him from evading
                if (engaged)
                {
                    if (wipeCheckTimer <= diff)
                    {
                        wipeCheckTimer = WIPE_CHECK_INTERVAL;
                        if (!HasLivingPlayer())
                        {
                            EnterEvadeMode();
                            return;
                        }
                    }
                    else
                        wipeCheckTimer -= diff;
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
                        case EVENT_SPIRIT_GALE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 150.0f, true))
                                me->CastSpell(target, SPELL_SPIRIT_GALE, false);
                            events.ScheduleEvent(EVENT_SPIRIT_GALE, urand(7500,13000));
                            break;
                        case EVENT_RAISE_FALLEN_CRUSADER:
                            Talk(TALK_SLAY);
                            if (Unit* vict = me->GetVictim())
                            {
                                victimGUID = vict->GetGUID();
                                me->PrepareChanneledCast(me->GetAngle(vict), SPELL_RAISE_FALLEN_CRUSADER);
                            }
                            break;
                        case EVENT_EVICT_SOUL:
                            Talk(TALK_EVICT);
                            {
                                Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, NonTankTargetSelector(me));
                                if (!target)
                                    target = me->GetVictim();
                                if (target)
                                {
                                    Talk(TALK_EVICTED_SOUL_ANN, target);
                                    me->CastSpell(target, SPELL_EVICT_SOUL, false);
                                }
                            }

                            events.ScheduleEvent(EVENT_EVICT_SOUL, urand(25000, 26000));
                            break;
                        case EVENT_EMPOWERING_SPIRITS:
                            Talk(TALK_SPIRITS);
                            Talk(TALK_EMPOWERED_SPIRIT_ANN);
                            DoCastAOE(SPELL_SUMMON_EMPOWERING_SPIRITS);
                            events.ScheduleEvent(EVENT_EMPOWERING_SPIRITS, urand(40000, 42000));
                            events.ScheduleEvent(EVENT_RAISE_FALLEN_CRUSADER, 20 * IN_MILLISECONDS);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_thalnos_the_soulrenderAI(creature);
        }
};

// Evicted Soul 59974
struct npc_evicted_soul : public ScriptedAI
{
    npc_evicted_soul(Creature* creature) : ScriptedAI(creature)
    {
        instance = creature->GetInstanceScript();
    }

    EventMap events;
    InstanceScript* instance;

    void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
    {
        if (Player* itr = me->FindNearestPlayer(VISIBLE_RANGE))
        {
            itr->CastSpell(me, SPELL_CLONE_ME, true); // coppy
            me->CombatStart(itr);
        }
    }

    void UpdateAI(uint32 /*diff*/) OVERRIDE
    {
        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        DoMeleeAttackIfReady();
    }
};

// Fallen Crusader 59884
struct npc_fallen_crusader : public ScriptedAI
{
    npc_fallen_crusader(Creature* creature) : ScriptedAI(creature), mindroot_timer(4000)
    {
        instance = creature->GetInstanceScript();
    }

    uint32 mindroot_timer;
    EventMap events;
    InstanceScript* instance;

    void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
    {
        mindroot_timer = urand(3500, 4200);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (mindroot_timer <= diff)
        {
            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 150.0f, true))
                DoCast(target, SPELL_MIND_ROT);

            mindroot_timer = urand(3500, 4200);
        }
        else
            mindroot_timer -= diff;

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        DoMeleeAttackIfReady();
    }
};

// Empowering Spirit 59893
struct npc_empowering_spirit : public ScriptedAI
{
    npc_empowering_spirit(Creature* creature) : ScriptedAI(creature), zombie_timer(5000)
    {
        instance = creature->GetInstanceScript();
    }

    uint32 zombie_timer;
    EventMap events;
    InstanceScript* instance;

    void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
    {
        zombie_timer = 5000;
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (zombie_timer <= diff)
        {
            if (Creature* thalnos = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(BOSS_THALNOS_THE_SOULRENDER) : 0))
                thalnos->SummonCreature(NPC_EMPOWERED_ZOMBIE, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0.0f, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);

            me->DespawnOrUnsummon();
        }
        else zombie_timer -= diff;

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        DoMeleeAttackIfReady();
    }
};

// Empowered Zombie 59930
struct npc_empowered_zombie : public ScriptedAI
{
    npc_empowered_zombie(Creature* creature) : ScriptedAI(creature)
    {
        instance = creature->GetInstanceScript();
    }

    EventMap events;
    InstanceScript* instance;

    void UpdateAI(uint32 /*diff*/) OVERRIDE
    {
        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        DoMeleeAttackIfReady();
    }
};

// Raise Fallen Crusader 115139
class spell_raise_fallen_crusader : public AuraScript
{
    PrepareAuraScript(spell_raise_fallen_crusader);

public:

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Creature* owner = GetOwner()->ToCreature())
            owner->AI()->DoAction(ACTION_REMOVE_RAISE);
    }

    void Register() OVERRIDE
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_raise_fallen_crusader::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
    }
};

// 220. Summoned by 115290 - Spirit Gale
class sat_spirit_gale : public IAreaTriggerAura
{
    bool CheckTriggering(WorldObject* triggering) OVERRIDE
    {
        return triggering->ToPlayer() && triggering->ToPlayer()->IsAlive();
    }

    // The aura targets its caster, so the player casts it on himself for Thalnos; cast by Thalnos it would burn him
    void ApplyGale(Player* player)
    {
        if (m_caster && !player->HasAura(SPELL_SPIRIT_GALE_AURA))
            player->CastSpell(player, SPELL_SPIRIT_GALE_AURA, true, NULL, NULL, m_caster->GetGUID());
    }

    void OnTriggeringApply(WorldObject* triggering) OVERRIDE
    {
        if (Player* player = triggering->ToPlayer())
            ApplyGale(player);
    }

    void OnTriggeringUpdate(WorldObject* triggering) OVERRIDE
    {
        if (Player* player = triggering->ToPlayer())
            ApplyGale(player);
    }

    void OnTriggeringRemove(WorldObject* triggering) OVERRIDE
    {
        if (Player* pItr = triggering->ToPlayer())
            pItr->RemoveAurasDueToSpell(SPELL_SPIRIT_GALE_AURA);
    }
};

void AddSC_boss_thalnos_the_soulrender()
{
    new boss_thalnos_the_soulrender();
    new creature_script<npc_evicted_soul>("npc_evicted_soul");
    new creature_script<npc_fallen_crusader>("npc_fallen_crusader");
    new creature_script<npc_empowering_spirit>("npc_empowering_spirit");
    new creature_script<npc_empowered_zombie>("npc_empowered_zombie");

    RegisterAuraScript(spell_raise_fallen_crusader);
    new atrigger_script<sat_spirit_gale>("sat_spirit_gale");
}
