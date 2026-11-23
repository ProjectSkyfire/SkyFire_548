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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellInfo.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"
#include "zulgurub.h"

enum Yells
{
    SAY_AGGRO               = 0,
    SAY_DREAMSTATE          = 1,
    SAY_DEATH               = 3,
    SAY_KILL                = 4,
    SAY_DREAMSTATE_ANNOUNCE = 5
};

enum Spells
{
    SPELL_EARTH_SHOCK       = 96650,
    SPELL_WRATH             = 96652,
    SPELL_AWAKEN_NIGHTMARES = 96658,
    SPELL_NIGHTMARE         = 96670,
    SPELL_WAKING_NIGHTMARE  = 96757,
    SPELL_CONSUME_SOUL      = 96758
};

enum Events
{
    EVENT_EARTH_SHOCK = 1,
    EVENT_WRATH       = 2
};

enum Points
{
    POINT_NIGHTMARE_TARGET = 522840
};

class npc_nightmare_illusion : public CreatureScript
{
    public:
        npc_nightmare_illusion() : CreatureScript("npc_nightmare_illusion") { }

        struct npc_nightmare_illusionAI : public ScriptedAI
        {
            npc_nightmare_illusionAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                SetCombatMovement(false);
                me->SetSpeed(MOVE_RUN, 0.25f, true);
                me->SetSpeed(MOVE_WALK, 0.25f, true);
                me->SetSpeed(MOVE_FLIGHT, 0.25f, true);
                //me->SetWalk(true);
                aiSet = false;
                nightmareOwner = 0;
                nightmareTarget = 0;
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                me->DespawnOrUnsummon();
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE
            {
                if (!nightmareTarget || !nightmareOwner)
                {
                    me->DespawnOrUnsummon();
                    return;
                }
                if (Player* player = ObjectAccessor::GetPlayer(*me, nightmareTarget))
                    if (!player->IsAlive())
                    {
                        me->DespawnOrUnsummon();
                        return;
                    }
                if (Unit* unit = ObjectAccessor::GetUnit(*me, nightmareTarget))
                    if (!unit->IsAlive())
                    {
                        me->DespawnOrUnsummon();
                        return;
                    }
                if (aiSet)
                    return;
                aiSet = true;
                if (Unit* target = ObjectAccessor::GetUnit(*me, nightmareTarget))
                {
                    me->SetTarget(nightmareTarget);
                    DoCast(target, SPELL_WAKING_NIGHTMARE, true);
                    Position pos = target->GetPosition();
                    me->GetMotionMaster()->Clear();
                    me->GetMotionMaster()->MovePoint(POINT_NIGHTMARE_TARGET, pos);
                }
            }

            void SetNightmareTarget(uint64 owner, uint64 target)
            {
                nightmareOwner = owner;
                nightmareTarget = target;
            }

            void MovementInform(uint32 /*type*/, uint32 pointId) OVERRIDE
            {
                if (pointId == POINT_NIGHTMARE_TARGET)
                {
                    if (Unit* target = ObjectAccessor::GetUnit(*me, nightmareTarget))
                        if (Unit* owner = ObjectAccessor::GetUnit(*me, nightmareOwner))
                        {
                            me->CastSpell(target, SPELL_CONSUME_SOUL, true);
                            if (Creature* ownerCreature = owner->ToCreature())
                                ownerCreature->AI()->KilledUnit(target);
                            me->DespawnOrUnsummon();
                        }
                }
            }

        protected:
            bool aiSet;
            uint64 nightmareOwner;
            uint64 nightmareTarget;

        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetZulGurubAI<npc_nightmare_illusionAI>(creature);
        }
};

class boss_hazzarah : public CreatureScript
{
    public:
        boss_hazzarah() : CreatureScript("boss_hazzarah") { }

        struct boss_hazzarahAI : public BossAI
        {
            boss_hazzarahAI(Creature* creature) : BossAI(creature, DATA_CACHE_OF_MADNESS_BOSS)
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
            }

            void Reset() OVERRIDE
            {
                _Reset();
                summonedAt66Pct = false;
                summonedAt33Pct = false;
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();
                Talk(SAY_AGGRO);

                events.ScheduleEvent(EVENT_EARTH_SHOCK, 8000);
                events.ScheduleEvent(EVENT_WRATH, 500);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
                Talk(SAY_DEATH);
            }

            void KilledUnit(Unit* /*victim*/) OVERRIDE
            {
                Talk(SAY_KILL);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (!summonedAt66Pct && HealthBelowPct(67))
                {
                    summonedAt66Pct = true;
                    NightmarePhase();
                }
                if (!summonedAt33Pct && HealthBelowPct(34))
                {
                    summonedAt33Pct = true;
                    NightmarePhase();
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_EARTH_SHOCK:
                            DoCast(me->GetVictim(), SPELL_EARTH_SHOCK);
                            events.ScheduleEvent(EVENT_EARTH_SHOCK, 10000);
                            break;
                        case EVENT_WRATH:
                            DoCast(me->GetVictim(), SPELL_WRATH);
                            events.ScheduleEvent(EVENT_WRATH, urand(3000, 5000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

            void NightmarePhase()
            {
                me->InterruptSpell(CURRENT_GENERIC_SPELL);
                me->InterruptSpell(CURRENT_CHANNELED_SPELL);

                std::list<Player*> nearbyPlayers;
                Skyfire::AnyPlayerInObjectRangeCheck check(me, 100.0f);
                Skyfire::PlayerListSearcher<Skyfire::AnyPlayerInObjectRangeCheck> searcher(me, nearbyPlayers, check);
                me->VisitNearbyWorldObject(100.0f, searcher);
                nearbyPlayers.remove_if([](Player* player) { return !player->IsAlive(); });

                // All but one of the players fall asleep, up to four of them
                if (nearbyPlayers.size() < 2)
                    return;

                Player* awake = Skyfire::Containers::SelectRandomContainerElement(nearbyPlayers);
                nearbyPlayers.remove(awake);
                if (nearbyPlayers.size() > 4)
                    Skyfire::Containers::RandomResizeList(nearbyPlayers, 4);

                for (Player* sleeper : nearbyPlayers)
                {
                    Position pos;
                    do
                    {
                        pos = sleeper->GetNearPosition(30.0f, float(rand_norm() * 2 * M_PI));
                    }
                    while (sleeper->GetDistance(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ()) < 20.0f);

                    if (TempSummon* summon = me->SummonCreature(NPC_NIGHTMARE_ILLUSION, pos, TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
                        if (npc_nightmare_illusion::npc_nightmare_illusionAI* ai = CAST_AI(npc_nightmare_illusion::npc_nightmare_illusionAI, summon->AI()))
                            ai->SetNightmareTarget(me->GetGUID(), sleeper->GetGUID());
                }

                // The player left awake is told what to do
                Talk(SAY_DREAMSTATE_ANNOUNCE, awake);
                Talk(SAY_DREAMSTATE);
                DoCast(me, SPELL_AWAKEN_NIGHTMARES);
            }

        protected:
            bool summonedAt66Pct;
            bool summonedAt33Pct;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetZulGurubAI<boss_hazzarahAI>(creature);
        }
};

void AddSC_boss_hazzarah()
{
    new npc_nightmare_illusion();
    new boss_hazzarah();
}
