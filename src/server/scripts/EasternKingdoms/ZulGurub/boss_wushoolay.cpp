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
#include "SpellInfo.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"
#include "zulgurub.h"

enum Yells
{
    SAY_AGGRO                   = 0,
    SAY_DEATH                   = 1,
    SAY_KILL                    = 2,
    SAY_LIGHTNING_NUKE          = 3,
    SAY_LIGHTNING_RUSH          = 4,
    SAY_LIGHTNING_NUKE_ANNOUNCE = 5
};

enum Spells
{
    SPELL_FORKED_LIGHTNING = 96712,
    SPELL_LIGHTNING_CLOUD  = 96710,
    SPELL_LIGHTNING_ROD    = 96698,
    SPELL_LIGHTNING_RUSH   = 96697
};

enum Events
{
    EVENT_FORKED_LIGHTNING = 1,
    EVENT_LIGHTNING_CLOUD  = 2,
    EVENT_LIGHTNING_RUSH   = 3
};

enum Points
{
    POINT_LIGHTNING_RUSH = 522860
};

class boss_wushoolay : public CreatureScript
{
    public:
        boss_wushoolay() : CreatureScript("boss_wushoolay") { }

        struct boss_wushoolayAI : public BossAI
        {
            boss_wushoolayAI(Creature* creature) : BossAI(creature, DATA_CACHE_OF_MADNESS_BOSS)
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
                rushTarget = 0;
                preRushTarget = 0;
                SetCombatMovement(true);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();
                Talk(SAY_AGGRO);

                events.ScheduleEvent(EVENT_FORKED_LIGHTNING, 15000);
                events.ScheduleEvent(EVENT_LIGHTNING_CLOUD, urand(15000, 45000));
                events.ScheduleEvent(EVENT_LIGHTNING_RUSH, 15500);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
                Talk(SAY_DEATH);
                me->SetCanFly(false);
                me->SetDisableGravity(false);
            }

            void KilledUnit(Unit* /*victim*/) OVERRIDE
            {
                Talk(SAY_KILL);
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
                        case EVENT_FORKED_LIGHTNING:
                            if (!me->HasAura(SPELL_LIGHTNING_RUSH))
                                DoCastAOE(SPELL_FORKED_LIGHTNING, true);
                            events.ScheduleEvent(EVENT_FORKED_LIGHTNING, 15000);
                            break;
                        case EVENT_LIGHTNING_CLOUD:
                            if (me->HasAura(SPELL_LIGHTNING_RUSH))
                                events.ScheduleEvent(EVENT_LIGHTNING_CLOUD, urand(2000, 5000));
                            else
                            {
                                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 30, true))
                                {
                                    float x, y, z;
                                    target->GetPosition(x, y, z);
                                    me->CastSpell(x, y, z, SPELL_LIGHTNING_CLOUD, true);
                                }
                                events.ScheduleEvent(EVENT_LIGHTNING_CLOUD, urand(15000, 45000));
                            }
                            break;
                        case EVENT_LIGHTNING_RUSH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                            {
                                Talk(SAY_LIGHTNING_RUSH);
                                me->StopMoving();
                                SetCombatMovement(false);
                                if (Unit* victim = me->GetVictim())
                                    preRushTarget = victim->GetGUID();
                                me->AttackStop();
                                me->SetTarget(rushTarget = target->GetGUID());
                                me->CastSpell(me, SPELL_LIGHTNING_RUSH);
                            }
                            events.ScheduleEvent(EVENT_LIGHTNING_RUSH, 25000);
                            break;
                        default:
                            break;
                    }
                }

                if (!me->HasAura(SPELL_LIGHTNING_RUSH))
                    DoMeleeAttackIfReady();
            }

            void SpellHit(Unit* /*caster*/, SpellInfo const* spell) OVERRIDE
            {
                switch (spell->Id)
                {
                    case SPELL_LIGHTNING_RUSH:
                        if (rushTarget)
                            if (Player* target = ObjectAccessor::GetPlayer(*me, rushTarget))
                            {
                                me->StopMoving();
                                SetCombatMovement(false);
                                me->SetCanFly(true);
                                me->SetTarget(rushTarget);
                                me->GetMotionMaster()->Clear();
                                me->GetMotionMaster()->MovePoint(POINT_LIGHTNING_RUSH, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ());
                            }
                        break;
                    default:
                        break;
                }
            }

            void MovementInform(uint32 /*type*/, uint32 pointId) OVERRIDE
            {
                switch (pointId)
                {
                    case POINT_LIGHTNING_RUSH:
                        me->RemoveAurasDueToSpell(SPELL_LIGHTNING_RUSH);
                        me->SetCanFly(false);
                        {
                            Unit* chaseTarget = ObjectAccessor::GetUnit(*me, preRushTarget);
                            if (!chaseTarget || !chaseTarget->IsAlive())
                                chaseTarget = me->GetVictim();
                            if (chaseTarget)
                            {
                                me->Attack(chaseTarget, true);
                                me->GetMotionMaster()->Clear();
                                me->GetMotionMaster()->MoveChase(chaseTarget);
                            }
                        }
                        SetCombatMovement(true);
                        if (SelectTarget(SELECT_TARGET_RANDOM, 0, 13, false))
                        {
                            std::list<Player*> nearbyPlayers;
                            Skyfire::AnyPlayerInObjectRangeCheck check(me, 13.0f);
                            Skyfire::PlayerListSearcher<Skyfire::AnyPlayerInObjectRangeCheck> searcher(me, nearbyPlayers, check);
                            me->VisitNearbyWorldObject(13.0f, searcher);
                            for (Player* nearby : nearbyPlayers)
                                Talk(SAY_LIGHTNING_NUKE_ANNOUNCE, nearby);
                            Talk(SAY_LIGHTNING_NUKE);
                            me->CastSpell(me, SPELL_LIGHTNING_ROD, false);
                        }
                        break;
                    default:
                        break;
                }
            }

        protected:
            uint64 rushTarget;
            uint64 preRushTarget;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetZulGurubAI<boss_wushoolayAI>(creature);
        }
};

void AddSC_boss_wushoolay()
{
    new boss_wushoolay();
}
