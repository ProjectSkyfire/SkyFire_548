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
#include "InstanceScript.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Player.h"
#include "ObjectAccessor.h"
#include "GridNotifiers.h"
#include "halls_of_origination.h"

enum Texts
{
    SAY_DEATH   = 0,
    SAY_AGGRO   = 1,
    SAY_KILL    = 2,
};

enum Spells
{
    // Setesh
    SPELL_CHAOS_BOLT            = 77069,
    //SPELL_REIGN_OF_CHAOS      = 77026,
    //SPELL_CHAOS_BLAST         = 76681,
    //SPELL_SEED_OF_CHAOS       = 76870,
    //SPELL_SUMMON_CHAOS_SEED   = 76888,

    // Void Sentinel
    SPELL_VOID_BARRIER          = 76959,
    SPELL_CHARGED_FISTS         = 77238,

    // Void Seeker
    SPELL_ANTI_MAGIC_PRISON     = 76903,
    SPELL_SHADOW_BOLT_VOLLEY    = 76146, //r
};

enum NPCs
{
    NPC_VOID_SENTINEL       = 41208,
    NPC_VOID_SEEKER         = 41371,
    NPC_VOID_WURM           = 41374,
    NPC_CHAOS_PORTAL        = 41055,
    NPC_REIGN_OF_CHAOS      = 41168, // 77026
    NPC_VOID_LORD           = 41364, // 77458
};

enum Events
{
    EVENT_CHAOS_BOLT            = 1,
    EVENT_SUMMON_CHAOS_PORTAL   = 2,

    EVENT_SUMMON_6              = 3,
    EVENT_SUMMON_10             = 4,
    EVENT_SUMMON_12             = 5,
    EVENT_SUMMON_15             = 6,

    EVENT_ANTI_MAGIC_PRISON     = 7,
    EVENT_SHADOW_BOLT_VOLLEY    = 8,

    EVENT_VOID_BARRIER          = 9,
    EVENT_CHARGED_FISTS         = 10,

    EVENT_MOVE                  = 11,
};

enum SeteshSummonTypes
{
    SETESH_SUMMON_WURM      = 1,
    SETESH_SUMMON_SENTINEL  = 2,
    SETESH_SUMMON_SEEKER    = 3,
};

const Position movepos[9] =
{
    { -481.55f,  14.15f, 343.92f, 2.07f },
    { -490.31f,  25.55f, 343.93f, 2.56f },
    { -508.35f,  30.95f, 343.94f, 3.08f },
    { -524.62f,  30.30f, 343.93f, 3.35f },
    { -534.63f,  22.76f, 343.92f, 4.08f },
    { -539.40f,  9.067f, 343.92f, 4.67f },
    { -537.78f, -3.28f,  343.92f, 5.11f },
    { -528.39f, -16.43f, 343.93f, 5.85f },
    { -513.85f, -19.04f, 343.93f, 6.15f }
};

class boss_setesh : public CreatureScript
{
    public:
        boss_setesh() : CreatureScript("boss_setesh") { }

        struct boss_seteshAI : public BossAI
        {
            boss_seteshAI(Creature* creature) : BossAI(creature, DATA_SETESH)
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
                me->SetReactState(REACT_AGGRESSIVE);

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                Talk (SAY_AGGRO);
                events.ScheduleEvent(EVENT_CHAOS_BOLT, 10000);
                events.ScheduleEvent(EVENT_SUMMON_CHAOS_PORTAL, 20000);
                events.ScheduleEvent(EVENT_MOVE, 2000);

                me->SetReactState(REACT_PASSIVE);

                DoZoneInCombat();
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type == POINT_MOTION_TYPE)
                {
                    switch (pointId)
                    {
                        case 1:
                            events.ScheduleEvent(EVENT_MOVE, 1000);
                            break;
                    }
                }
            }

            void KilledUnit(Unit* /*victim*/) OVERRIDE
            {
                Talk(SAY_KILL);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                Talk(SAY_DEATH);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while(uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CHAOS_BOLT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(target, SPELL_CHAOS_BOLT);
                            events.ScheduleEvent(EVENT_CHAOS_BOLT, urand(5000, 10000));
                            break;
                        case EVENT_SUMMON_CHAOS_PORTAL:
                            me->SummonCreature(NPC_CHAOS_PORTAL,
                                me->GetPositionX(),
                                me->GetPositionY(),
                                me->GetPositionZ(),
                                me->GetOrientation(),
                                IsHeroic() ? TempSummonType::TEMPSUMMON_DEAD_DESPAWN : TempSummonType::TEMPSUMMON_TIMED_DESPAWN,
                                IsHeroic() ? 0 : 35000);
                            events.ScheduleEvent(EVENT_SUMMON_CHAOS_PORTAL, urand(40000, 45000));
                            break;
                        case EVENT_MOVE:
                            me->GetMotionMaster()->MovePoint(1, movepos[urand(0, 8)]);
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_seteshAI(creature);
        }
};

class npc_setesh_chaos_portal : public CreatureScript
{
    public:
        npc_setesh_chaos_portal() : CreatureScript("npc_setesh_chaos_portal") { }

        struct npc_setesh_chaos_portalAI : public ScriptedAI
        {
            npc_setesh_chaos_portalAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
                SetCombatMovement(false);
            }

            InstanceScript* instance;
            EventMap events;

            void SeteshSummon(SeteshSummonTypes type)
            {
                if (!instance)
                    return;

                if (Creature* pSetesh = instance->instance->GetCreature(instance->GetData64(DATA_SETESH)))
                {
                    switch (type)
                    {
                        case SETESH_SUMMON_WURM:
                            pSetesh->SummonCreature(NPC_VOID_WURM,
                                me->GetPositionX(),
                                me->GetPositionY(),
                                me->GetPositionZ(),
                                me->GetOrientation());
                            pSetesh->SummonCreature(NPC_VOID_WURM,
                                me->GetPositionX(),
                                me->GetPositionY(),
                                me->GetPositionZ(),
                                me->GetOrientation());
                            break;
                        case SETESH_SUMMON_SENTINEL:
                            pSetesh->SummonCreature(NPC_VOID_SENTINEL,
                                me->GetPositionX(),
                                me->GetPositionY(),
                                me->GetPositionZ(),
                                me->GetOrientation());
                            break;
                        case SETESH_SUMMON_SEEKER:
                            pSetesh->SummonCreature(NPC_VOID_SEEKER,
                                me->GetPositionX(),
                                me->GetPositionY(),
                                me->GetPositionZ(),
                                me->GetOrientation());
                            break;
                    }
                }
            }

            void Reset() OVERRIDE
            {
                events.Reset();
                me->SetReactState(REACT_PASSIVE);

                if (!IsHeroic())
                {
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
                    SeteshSummon(SETESH_SUMMON_WURM);
                    events.ScheduleEvent(EVENT_SUMMON_12, 12000);
                }
                else
                {
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
                    SeteshSummon(SETESH_SUMMON_SENTINEL);
                    events.ScheduleEvent(EVENT_SUMMON_6, 12000);
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                while(uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SUMMON_6:
                            SeteshSummon(SETESH_SUMMON_WURM);
                            events.ScheduleEvent(EVENT_SUMMON_10, 10000);
                            break;
                        case EVENT_SUMMON_10:
                            SeteshSummon(SETESH_SUMMON_SEEKER);
                            events.ScheduleEvent(EVENT_SUMMON_15, 15000);
                            break;
                        case EVENT_SUMMON_12:
                            SeteshSummon(SETESH_SUMMON_SEEKER);
                            events.ScheduleEvent(EVENT_SUMMON_15, 15000);
                            break;
                        case EVENT_SUMMON_15:
                            if (IsHeroic())
                            {
                                if (urand(0, 1))
                                    SeteshSummon(SETESH_SUMMON_WURM);
                                else
                                    SeteshSummon(SETESH_SUMMON_SENTINEL);
                                events.ScheduleEvent(EVENT_SUMMON_15, 15000);
                            }
                            else
                            {
                                SeteshSummon(SETESH_SUMMON_SENTINEL);
                                me->DespawnOrUnsummon();
                            }
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_setesh_chaos_portalAI(creature);
        }
};

class npc_setesh_void_sentinel : public CreatureScript
{
    public:
        npc_setesh_void_sentinel() : CreatureScript("npc_setesh_void_sentinel") { }

        struct npc_setesh_void_sentinelAI : public ScriptedAI
        {
            npc_setesh_void_sentinelAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            // The sentinel never scheduled its abilities, so Charged Fists could not be cast
            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_CHARGED_FISTS, urand(5000, 8000));
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while(uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CHARGED_FISTS:
                            DoCast(me, SPELL_CHARGED_FISTS);
                            events.ScheduleEvent(EVENT_CHARGED_FISTS, urand(18000, 22000));
                            break;
                        case EVENT_VOID_BARRIER:
                            DoCast(me, SPELL_VOID_BARRIER);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_setesh_void_sentinelAI(creature);
        }
};

class npc_setesh_void_seeker : public CreatureScript
{
    public:
        npc_setesh_void_seeker() : CreatureScript("npc_setesh_void_seeker") { }

        struct npc_setesh_void_seekerAI : public ScriptedAI
        {
            npc_setesh_void_seekerAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                if (urand(0, 1))
                    events.ScheduleEvent(EVENT_ANTI_MAGIC_PRISON, urand(3000, 5000));
                else
                    events.ScheduleEvent(EVENT_SHADOW_BOLT_VOLLEY, urand(3000, 5000));
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while(uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                    case EVENT_ANTI_MAGIC_PRISON:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(target, SPELL_ANTI_MAGIC_PRISON);
                        events.ScheduleEvent(EVENT_ANTI_MAGIC_PRISON, urand(31000, 33000));
                        break;
                    case EVENT_SHADOW_BOLT_VOLLEY:
                        DoCastAOE(SPELL_SHADOW_BOLT_VOLLEY);
                        events.ScheduleEvent(EVENT_SHADOW_BOLT_VOLLEY, urand(9000, 13000));
                        break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_setesh_void_seekerAI(creature);
        }
};

void AddSC_boss_setesh()
{
    new boss_setesh();
    new npc_setesh_chaos_portal();
    new npc_setesh_void_sentinel();
    new npc_setesh_void_seeker();
}
