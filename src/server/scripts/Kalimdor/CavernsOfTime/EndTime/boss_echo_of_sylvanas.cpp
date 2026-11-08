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

#include "end_time.h"

enum ScriptTexts
{
    SAY_AGGRO   = 0,
    SAY_DEATH   = 1,
    SAY_KILL    = 3,
    SAY_WIPE    = 2,
    SAY_SPELL   = 4,
};

enum Spells
{
    SPELL_SHRIEK_OF_THE_HIGHBORNE       = 101412,
    SPELL_UNHOLY_SHOT                   = 101411,
    SPELL_BLACK_ARROW                   = 101404,
    SPELL_WRACKING_PAIN_ANY             = 100865,
    SPELL_WRACKING_PAIN_AURA            = 101258,
    SPELL_WRACKING_PAIN_DMG             = 101257,
    SPELL_WRACKING_PAIN_BACK            = 101221,
    SPELL_DEATH_GRIP_AOE                = 101397,
    SPELL_DEATH_GRIP                    = 101987,
    SPELL_SUMMON_GHOULS                 = 102603, // before combat
    SPELL_TELEPORT                      = 101398,
    SPELL_CALL_OF_THE_HIGHBORNE         = 100686, // immune
    SPELL_CALL_OF_THE_HIGHBORNE_1       = 100867, // visual spawn ghouls
    SPELL_CALL_OF_THE_HIGHBORNE_2       = 105766, // visual back ghouls
    SPELL_CALL_OF_THE_HIGHBORNE_3       = 102581, // visual ?
    SPELL_CALL_OF_THE_HIGHBORNE_4       = 100862,
    SPELL_SPAWN_GHOUL                   = 101200,
    SPELL_SEEPING_SHADOWS_DUMMY         = 103175,
    SPELL_SEEPING_SHADOWS_AURA          = 103182,
    SPELL_SACRIFICE                     = 101348,
    SPELL_DWINDLE                       = 101259,
    SPELL_JUMP                          = 101517,
    SPELL_JUMP_SCRIPT                   = 101527,
    SPELL_JUMP_VEHICLE                  = 101528,
    SPELL_PERMANENT_FEIGH_DEATH         = 96733,
    SPELL_SHRINK                        = 101318,
    SPELL_BLIGHTED_ARROWS_SPAWN         = 101547,
    SPELL_BLIGHTED_ARROWS               = 101401,
    SPELL_BLIGHTED_ARROWS_EXPLOSION     = 100763,
};

enum Events
{
    EVENT_SHRIEK_OF_THE_HIGHBORNE   = 1,
    EVENT_UNHOLY_SHOT               = 2,
    EVENT_BLACK_ARROW               = 3,
    EVENT_DEATH_GRIP                = 4,
    EVENT_TELEPORT                  = 5,
    EVENT_TELEPORT_1                = 6,
    EVENT_SPAWN_GHOUL               = 7,
    EVENT_MOVE_GHOUL                = 8,
    EVENT_START                     = 9,
    EVENT_CHECK_OUT_OF_BOUNDS       = 10,
    EVENT_GHOUL_VISUAL_UPDATE_SCALE = 11,
    EVENT_BLIGHTED_ARROWS_PREP      = 12,
    EVENT_BLIGHTED_ARROWS_JUMP      = 13,
    EVENT_BLIGHTED_ARROWS_VOLLEY    = 14,
    EVENT_BLIGHTED_ARROWS_FALL      = 15,
    EVENT_BLIGHTED_ARROWS_END       = 16,
};

enum Adds
{
    NPC_GHOUL_VISUAL    = 54197,
    NPC_BRITTLE_GHOUL   = 54952,
    NPC_RISEN_GHOUL     = 54191,
    NPC_JUMP            = 54385,
    NPC_BLIGHTED_ARROW  = 54403,
};

enum Others
{
    DATA_GUID                   = 1,
    POINT_GHOUL                 = 2,
    ACTION_GHOUL                = 3,
    ACTION_KILL_GHOUL           = 4,
    ACTION_GHOUL_VISUAL_AURA    = 5,
    ACTION_GHOUL_VISUAL_MOVE    = 6,
};

const Position centerPos = {3839.42f, 911.249f, 56.0298f, 1.309f};
const float spawnDist = 32.8f;
const float angleOffset = 0;
const uint8 spawnCount = 8;
const float spawnAngle = 2 * M_PI / spawnCount;

class boss_echo_of_sylvanas : public CreatureScript
{
    public:
        boss_echo_of_sylvanas() : CreatureScript("boss_echo_of_sylvanas") { }

        struct boss_echo_of_sylvanasAI : public BossAI
        {
            boss_echo_of_sylvanasAI(Creature* creature) : BossAI(creature, DATA_ECHO_OF_SYLVANAS)
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
                me->CastSpell(me, SPELL_SUMMON_GHOULS, true);
            }

            uint8 ghouls;
            uint8 deadghouls;
            uint64 ghoulGuids[spawnCount];
            float blightedArrowsAngle;
            bool autoattackStopped;

            void Reset() OVERRIDE
            {
                _Reset();
                if (me->GetVehicleBase())
                    me->ExitVehicle();
                me->SetReactState(REACT_AGGRESSIVE);
                me->SetDisableGravity(false);
                me->SetCanFly(false);
                ghouls = 0;
                deadghouls = 0;
                for (uint8 i = 0; i < spawnCount; ++i)
                    ghoulGuids[i] = 0;
                blightedArrowsAngle = 0;
                autoattackStopped = false;
                me->GetMap()->SetWorldState(WORLDSTATE_SEVERED_TIES, 0);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
                Talk(SAY_DEATH);

                // Quest
                Map::PlayerList const &PlayerList = instance->instance->GetPlayers();
                if (!PlayerList.isEmpty())
                    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                        if (Player* player = i->GetSource())
                            if (me->GetDistance2d(player) <= 150.0f && player->GetQuestStatus(30097) == QUEST_STATUS_INCOMPLETE)
                                DoCast(player, SPELL_ARCHIVED_SYLVANAS, true);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                Talk(SAY_AGGRO);
                events.ScheduleEvent(EVENT_UNHOLY_SHOT, urand(5000, 20000));
                events.ScheduleEvent(EVENT_SHRIEK_OF_THE_HIGHBORNE, urand(5000, 20000));
                events.ScheduleEvent(EVENT_TELEPORT, 40000);
                me->RemoveAura(SPELL_SUMMON_GHOULS);
                summons.DespawnEntry(NPC_BRITTLE_GHOUL);
                deadghouls = 0;
                DoZoneInCombat();
                instance->SetBossState(DATA_ECHO_OF_SYLVANAS, IN_PROGRESS);
            }

            void KilledUnit(Unit* victim) OVERRIDE
            {
                if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(SAY_KILL);
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action == ACTION_GHOUL)
                {
                    ghouls++;
                    if (ghouls >= 1)
                    {
                        DoCastAOE(SPELL_SACRIFICE);
                        summons.DespawnEntry(NPC_RISEN_GHOUL);
                        summons.DespawnEntry(NPC_GHOUL_VISUAL);
                        events.ScheduleEvent(EVENT_START, 2000);
                        ghouls = 0;
                    }
                }
                else if (action == ACTION_KILL_GHOUL)
                {
                    if (++deadghouls >= 2)
                        me->GetMap()->SetWorldState(WORLDSTATE_SEVERED_TIES, 1);
                }
            }

            void JustReachedHome() OVERRIDE
            {
                Talk(SAY_WIPE);
                DoCast(me, SPELL_SUMMON_GHOULS, true);
            }

            void SpawnAround(uint32 entry)
            {
                Unit* _first = NULL;
                Unit* _prev = NULL;
                for (uint8 i = 0; i < spawnCount; ++i)
                {
                    float x = me->GetPositionX() + cosf(angleOffset + spawnAngle * i) * spawnDist;
                    float y = me->GetPositionY() + sinf(angleOffset + spawnAngle * i) * spawnDist;
                    float z = me->GetMap()->GetHeight(x, y, me->GetPositionZ(), false);
                    if (Creature* summon = me->SummonCreature(entry, x, y, z, me->GetAngle(x, y) + M_PI))
                    {
                        if (entry == NPC_RISEN_GHOUL)
                        {
                            ghoulGuids[i] = summon->GetGUID();
                            if (_prev)
                            {
                                _prev->CastSpell(summon, SPELL_WRACKING_PAIN_ANY, true);
                                _prev->GetAI()->SetGUID(summon->GetGUID(), DATA_GUID);

                            }
                            _prev = summon;
                            if (i == 0)
                                _first = summon;
                        }
                    }
                }
                if (entry == NPC_RISEN_GHOUL)
                {
                    if (_first)
                    {
                        _prev->CastSpell(_first, SPELL_WRACKING_PAIN_ANY, true);
                        _prev->GetAI()->SetGUID(_first->GetGUID(), DATA_GUID);
                    }
                }
            }

            bool IsPlayerOutOfBounds(Player* player)
            {
                if (!player || !player->IsInWorld() || !player->IsAlive())
                    return false;

                float pAngle = me->GetAngle(player);
                for (uint8 i = 0; i < spawnCount + 1; ++i)
                {
                    if (!ghoulGuids[i % spawnCount])
                        continue;

                    float angle = angleOffset + spawnAngle * i;
                    if (pAngle < angle - spawnAngle / 2 || pAngle > angle + spawnAngle / 2)
                        continue;

                    if (Creature* ghoul = ObjectAccessor::GetCreature(*me, ghoulGuids[i % spawnCount]))
                    {
                        float dist = me->GetExactDist2d(ghoul);
                        if (me->GetExactDist2d(player) <= dist)
                            continue;

                        if (me->GetExactDist2d(player) >= dist + 10 + 20 * (dist / spawnDist))
                            continue;

                        if (ghoul->IsAlive())
                            return true;
                    }
                }

                return false;
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
                        case EVENT_UNHOLY_SHOT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_UNHOLY_SHOT);
                            events.ScheduleEvent(EVENT_UNHOLY_SHOT, urand(10000, 20000));
                            break;
                        case EVENT_SHRIEK_OF_THE_HIGHBORNE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_SHRIEK_OF_THE_HIGHBORNE);
                            events.ScheduleEvent(EVENT_SHRIEK_OF_THE_HIGHBORNE, urand(15000, 21000));
                            break;
                        case EVENT_TELEPORT:
                            events.CancelEvent(EVENT_UNHOLY_SHOT);
                            events.CancelEvent(EVENT_SHRIEK_OF_THE_HIGHBORNE);
                            me->SetReactState(REACT_PASSIVE);
                            me->AttackStop();
                            me->InterruptNonMeleeSpells(false);
                            DoCast(me, SPELL_TELEPORT, true);
                            events.ScheduleEvent(EVENT_TELEPORT_1, 2000);
                            break;
                        case EVENT_TELEPORT_1:
                            Talk(SAY_SPELL);
                            DoCast(me, SPELL_CALL_OF_THE_HIGHBORNE, true);
                            SpawnAround(NPC_GHOUL_VISUAL);
                            events.ScheduleEvent(EVENT_DEATH_GRIP, 3000);
                            events.ScheduleEvent(EVENT_SPAWN_GHOUL, 3000);
                            break;
                        case EVENT_DEATH_GRIP:
                            DoCastAOE(SPELL_DEATH_GRIP_AOE);
                            break;
                        case EVENT_SPAWN_GHOUL:
                            deadghouls = 0;
                            SpawnAround(NPC_RISEN_GHOUL);
                            events.ScheduleEvent(EVENT_CHECK_OUT_OF_BOUNDS, 2000);
                            break;
                        case EVENT_CHECK_OUT_OF_BOUNDS:
                        {
                            Map::PlayerList const& players = instance->instance->GetPlayers();
                            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                                if (Player* player = itr->GetSource())
                                    if (IsPlayerOutOfBounds(player))
                                        player->CastSpell(player, SPELL_WRACKING_PAIN_BACK, true);
                            events.ScheduleEvent(EVENT_CHECK_OUT_OF_BOUNDS, 1000);
                            break;
                        }
                        case EVENT_START:
                            me->RemoveAura(SPELL_CALL_OF_THE_HIGHBORNE);
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->GetMotionMaster()->MoveChase(me->GetVictim());
                            events.CancelEvent(EVENT_CHECK_OUT_OF_BOUNDS);
                            events.ScheduleEvent(EVENT_TELEPORT, 38000);
                            events.ScheduleEvent(EVENT_BLIGHTED_ARROWS_PREP, 500);
                            break;
                        case EVENT_BLIGHTED_ARROWS_PREP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            {
                                float dist = me->GetExactDist2d(target);
                                blightedArrowsAngle = me->GetAngle(target);
                                me->CastSpell(me->GetPositionX() + cosf(blightedArrowsAngle) * (dist - 5), me->GetPositionY() + sinf(blightedArrowsAngle) * (dist - 5), target->GetPositionZ(), SPELL_BLIGHTED_ARROWS_SPAWN, false);
                                me->CastSpell(me->GetPositionX() + cosf(blightedArrowsAngle) * (dist + 0), me->GetPositionY() + sinf(blightedArrowsAngle) * (dist + 0), target->GetPositionZ(), SPELL_BLIGHTED_ARROWS_SPAWN, false);
                                me->CastSpell(me->GetPositionX() + cosf(blightedArrowsAngle) * (dist + 5), me->GetPositionY() + sinf(blightedArrowsAngle) * (dist + 5), target->GetPositionZ(), SPELL_BLIGHTED_ARROWS_SPAWN, false);

                                autoattackStopped = true;
                                me->StopMoving();
                                me->GetMotionMaster()->Clear();
                                me->GetMotionMaster()->MoveIdle();
                                events.ScheduleEvent(EVENT_BLIGHTED_ARROWS_JUMP, 1000);
                            }
                            break;
                        case EVENT_BLIGHTED_ARROWS_JUMP:
                            if (Unit* jump = me->SummonCreature(NPC_JUMP, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 5, blightedArrowsAngle))
                                DoCast(jump, SPELL_JUMP_VEHICLE);
                            events.ScheduleEvent(EVENT_BLIGHTED_ARROWS_VOLLEY, 2000);
                            break;
                        case EVENT_BLIGHTED_ARROWS_VOLLEY:
                            DoCastAOE(SPELL_BLIGHTED_ARROWS);
                            events.ScheduleEvent(EVENT_BLIGHTED_ARROWS_FALL, 2000);
                            break;
                        case EVENT_BLIGHTED_ARROWS_FALL:
                            if (me->GetVehicleBase())
                                me->ExitVehicle();
                            me->GetMotionMaster()->Clear();
                            me->GetMotionMaster()->MoveFall();
                            events.ScheduleEvent(EVENT_BLIGHTED_ARROWS_END, 1000);
                            break;
                        case EVENT_BLIGHTED_ARROWS_END:
                            autoattackStopped = false;
                            me->NearTeleportTo(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation()); // Hack to fix vehicle bugs
                            me->GetMotionMaster()->MoveChase(me->GetVictim());
                            events.ScheduleEvent(EVENT_UNHOLY_SHOT, urand(5000, 20000));
                            events.ScheduleEvent(EVENT_SHRIEK_OF_THE_HIGHBORNE, urand(5000, 20000));
                            break;
                        default:
                            break;
                    }
                }

                if (!autoattackStopped)
                    DoMeleeAttackIfReady();
            }
        };

         CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetEndTimeAI<boss_echo_of_sylvanasAI>(creature);
        }
};

class npc_echo_of_sylvanas_ghoul : public CreatureScript
{
    public:
        npc_echo_of_sylvanas_ghoul() : CreatureScript("npc_echo_of_sylvanas_ghoul") { }

        struct npc_echo_of_sylvanas_ghoulAI : public ScriptedAI
        {
            npc_echo_of_sylvanas_ghoulAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                SetCombatMovement(false);
            }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action == ACTION_GHOUL_VISUAL_AURA)
                    DoCastAOE(SPELL_CALL_OF_THE_HIGHBORNE_2, true);
                else if (action == ACTION_GHOUL_VISUAL_MOVE)
                {
                    me->SetWalk(true);
                    me->SetCanFly(false);
                    me->SetSpeed(MOVE_RUN, 0.5f, true);
                    me->SetSpeed(MOVE_WALK, 0.5f, true);
                    me->SetSpeed(MOVE_FLIGHT, 0.5f, true);
                    me->SetSpeed(MOVE_SWIM, 0.265f, true);
                    me->GetMotionMaster()->MovePoint(POINT_GHOUL, centerPos);
                    DoCastAOE(SPELL_SHRINK);
                    //DoCast(me, SPELL_WRACKING_PAIN_AURA, true);
                    //events.ScheduleEvent(EVENT_GHOUL_VISUAL_UPDATE_SCALE, 1000);
                }
            }

            void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
            {
                DoCast(me, SPELL_CALL_OF_THE_HIGHBORNE_1, true);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_GHOUL_VISUAL_UPDATE_SCALE:
                            //me->SetObjectScale(0.25f + (me->GetExactDist2d(&centerPos) / spawnDist) * 0.75f);
                            events.ScheduleEvent(EVENT_GHOUL_VISUAL_UPDATE_SCALE, 1000);
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetEndTimeAI<npc_echo_of_sylvanas_ghoulAI>(creature);
        }
};

class npc_echo_of_sylvanas_risen_ghoul : public CreatureScript
{
    public:
        npc_echo_of_sylvanas_risen_ghoul() : CreatureScript("npc_echo_of_sylvanas_risen_ghoul") { }

        struct npc_echo_of_sylvanas_risen_ghoulAI : public ScriptedAI
        {
            npc_echo_of_sylvanas_risen_ghoulAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
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
                SetCombatMovement(false);
            }

            EventMap events;
            uint64 _guid;
            uint64 _visualGuid;

            void Reset() OVERRIDE
            {
                events.Reset();
                _guid = 0;
                _visualGuid = 0;
                me->SetFloatValue(UNIT_FIELD_BOUNDING_RADIUS, 5.0f);
                me->SetFloatValue(UNIT_FIELD_COMBAT_REACH, 5.0f);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                me->RemoveAura(SPELL_WRACKING_PAIN_ANY);
                if (Creature* target = ObjectAccessor::GetCreature(*me, _guid))
                    target->RemoveAura(SPELL_WRACKING_PAIN_ANY);
                _guid = 0;
                if (Creature* target = ObjectAccessor::GetCreature(*me, _visualGuid))
                    target->DespawnOrUnsummon();
                _visualGuid = 0;
                if (Creature* pSylvanas = me->FindNearestCreature(NPC_ECHO_OF_SYLVANAS, 300.0f))
                    pSylvanas->GetAI()->DoAction(ACTION_KILL_GHOUL);
                me->DespawnOrUnsummon(500);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                DoCast(me, SPELL_SEEPING_SHADOWS_DUMMY, true);
                if (Creature* ghoulVisual = me->FindNearestCreature(NPC_GHOUL_VISUAL, 10))
                {
                    ghoulVisual->GetAI()->DoAction(ACTION_GHOUL_VISUAL_AURA);
                    _visualGuid = ghoulVisual->GetGUID();
                }
                events.ScheduleEvent(EVENT_MOVE_GHOUL, 2000);
            }

            void SetGUID(uint64 guid, int32 type) OVERRIDE
            {
                if (type == DATA_GUID)
                    _guid = guid;
            }

            uint64 GetGUID(int32 type) const OVERRIDE
            {
                if (type == DATA_GUID)
                    return _guid;

                return 0;
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type == POINT_MOTION_TYPE)
                    if (pointId == POINT_GHOUL)
                        if (Unit* pSylvanas = me->FindNearestCreature(NPC_ECHO_OF_SYLVANAS, 300.0f))
                            pSylvanas->GetAI()->DoAction(ACTION_GHOUL);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_MOVE_GHOUL:
                            me->SetWalk(true);
                            me->SetCanFly(false);
                            me->SetSpeed(MOVE_RUN, 0.5f, true);
                            me->SetSpeed(MOVE_WALK, 0.5f, true);
                            me->SetSpeed(MOVE_FLIGHT, 0.5f, true);
                            me->SetSpeed(MOVE_SWIM, 0.265f, true);
                            me->GetMotionMaster()->MovePoint(POINT_GHOUL, centerPos);
                            if (Creature* ghoulVisual = me->FindNearestCreature(NPC_GHOUL_VISUAL, 10))
                                ghoulVisual->GetAI()->DoAction(ACTION_GHOUL_VISUAL_MOVE);
                            DoCast(me, SPELL_WRACKING_PAIN_AURA, true);
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetEndTimeAI<npc_echo_of_sylvanas_risen_ghoulAI>(creature);
        }
};

class npc_echo_of_sylvanas_blighted_arrows : public CreatureScript
{
    public:
        npc_echo_of_sylvanas_blighted_arrows() : CreatureScript("npc_echo_of_sylvanas_blighted_arrows") { }

        struct npc_echo_of_sylvanas_blighted_arrowsAI : public ScriptedAI
        {
            npc_echo_of_sylvanas_blighted_arrowsAI(Creature* creature) : ScriptedAI(creature)
            {
                SetCombatMovement(false);
            }

            void SpellHit(Unit* caster, const SpellInfo* spell) OVERRIDE
            {
                if (spell->Id != SPELL_BLIGHTED_ARROWS)
                    return;
                me->CastSpell(me, SPELL_BLIGHTED_ARROWS_EXPLOSION);
                me->DespawnOrUnsummon(1500);
            }

            void UpdateAI(uint32 diff) OVERRIDE { }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetEndTimeAI<npc_echo_of_sylvanas_blighted_arrowsAI>(creature);
        }
};

class spell_echo_of_sylvanas_wracking_pain_dmg : public SpellScriptLoader
{
    public:
        spell_echo_of_sylvanas_wracking_pain_dmg() : SpellScriptLoader("spell_echo_of_sylvanas_wracking_pain_dmg") { }

        class spell_echo_of_sylvanas_wracking_pain_dmg_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_echo_of_sylvanas_wracking_pain_dmg_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (!GetCaster() || !GetCaster()->IsAlive())
                {
                    targets.clear();
                    return;
                }

                if (!GetCaster()->GetAI())
                {
                    targets.clear();
                    return;
                }

                uint64 _guid = GetCaster()->GetAI()->GetGUID(DATA_GUID);
                if (Creature* target = ObjectAccessor::GetCreature(*GetCaster(), _guid))
                {
                    if (target->IsAlive())
                        targets.remove_if(WrackingPainTargetSelector(GetCaster(), target));
                    else
                        targets.clear();
                }
                else
                    targets.clear();
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_echo_of_sylvanas_wracking_pain_dmg_SpellScript::FilterTargets, EFFECT_0,TARGET_UNIT_SRC_AREA_ENEMY);
            }

        private:
            class WrackingPainTargetSelector
            {
                public:
                    WrackingPainTargetSelector(WorldObject* caster, WorldObject* target) : i_caster(caster), i_target(target) { }

                    bool operator()(WorldObject* unit)
                    {
                        if (unit->IsInBetween(i_caster, i_target))
                            return false;
                        return true;
                    }

                    WorldObject* i_caster;
                    WorldObject* i_target;
            };
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_echo_of_sylvanas_wracking_pain_dmg_SpellScript();
        }
};

class spell_echo_of_sylvanas_death_grip_aoe : public SpellScriptLoader
{
    public:
        spell_echo_of_sylvanas_death_grip_aoe() : SpellScriptLoader("spell_echo_of_sylvanas_death_grip_aoe") { }

        class spell_echo_of_sylvanas_death_grip_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_echo_of_sylvanas_death_grip_aoe_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetHitUnit()->CastSpell(GetCaster(), SPELL_DEATH_GRIP, true);
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_echo_of_sylvanas_death_grip_aoe_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_echo_of_sylvanas_death_grip_aoe_SpellScript();
        }
};

class spell_echo_of_sylvanas_seeping_shadows : public SpellScriptLoader
{
    public:
        spell_echo_of_sylvanas_seeping_shadows() : SpellScriptLoader("spell_echo_of_sylvanas_seeping_shadows") { }

        class spell_echo_of_sylvanas_seeping_shadows_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_echo_of_sylvanas_seeping_shadows_AuraScript);

            void HandlePeriodicTick(AuraEffect const* /*aurEff*/)
            {
                if (!GetCaster())
                    return;

                int32 amount = int32(0.2f * (100.0f - GetCaster()->GetHealthPct()));

                if (Aura* aur = GetCaster()->GetAura(103182))
                    aur->ModStackAmount(amount - aur->GetStackAmount());
                else
                    GetCaster()->CastCustomSpell(103182, SPELLVALUE_AURA_STACK, amount, GetCaster(), true);

            }

            void Register() OVERRIDE
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_echo_of_sylvanas_seeping_shadows_AuraScript::HandlePeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_echo_of_sylvanas_seeping_shadows_AuraScript();
        }
};

void AddSC_boss_echo_of_sylvanas()
{
    new boss_echo_of_sylvanas();
    new npc_echo_of_sylvanas_ghoul();
    new npc_echo_of_sylvanas_risen_ghoul();
    new npc_echo_of_sylvanas_blighted_arrows();
    new spell_echo_of_sylvanas_wracking_pain_dmg();
    new spell_echo_of_sylvanas_death_grip_aoe();
    new spell_echo_of_sylvanas_seeping_shadows();
}
