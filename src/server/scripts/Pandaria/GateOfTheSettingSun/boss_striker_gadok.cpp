/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "MoveSplineInit.h"
#include "Player.h"
#include "Vehicle.h"
#include "VehicleDefines.h"
#include "gate_of_the_setting_sun.h"

struct StrafPointStruct
{
    uint8 pointIdBeginOutside;
    Position beginOutside;

    uint8 pointIdBegin;
    Position begin;

    uint8 pointIdEnd;
    Position end;

    uint8 pointIdOutside;
    Position outside;
};

enum eMovements
{
    POINT_NORTH_START     = 1,
    POINT_SOUTH_START     = 2,
    POINT_WEST_START      = 3,
    POINT_EAST_START      = 4,

    POINT_NORTH_END       = 5,
    POINT_SOUTH_END       = 6,
    POINT_WEST_END        = 7,
    POINT_EAST_END        = 8,

    POINT_NORTH_OUTSIDE   = 9,
    POINT_SOUTH_OUTSIDE   = 10,
    POINT_WEST_OUTSIDE    = 11,
    POINT_EAST_OUTSIDE    = 12,
    POINT_PHASE_MAIN      = 13,

    POINT_KRIKTHIK_CIRCLE = 14,

    MOV_NORTH_SOUTH       = 0,
    MOV_SOUTH_NORTH       = 1,
    MOV_WEST_EAST         = 2,
    MOV_EAST_WEST         = 3,
};

StrafPointStruct const StrafPoints[4] =
{
    { POINT_NORTH_OUTSIDE, { 1258.0f, 2304.644f, 438.0f, 0.0f }, POINT_NORTH_START, { 1238.007f, 2304.644f, 435.0f, 0.0f }, POINT_NORTH_END, { 1153.398f, 2304.578f, 435.0f, 0.0f }, POINT_SOUTH_OUTSIDE, { 1133.4f, 2304.578f, 438.0f, 0.0f } }, // North -> South
    { POINT_SOUTH_OUTSIDE, { 1133.4f, 2304.578f, 438.0f, 0.0f }, POINT_SOUTH_START, { 1153.398f, 2304.578f, 435.0f, 0.0f }, POINT_SOUTH_END, { 1238.007f, 2304.644f, 435.0f, 0.0f }, POINT_NORTH_OUTSIDE, { 1258.0f, 2304.644f, 438.0f, 0.0f } }, // South -> North
    { POINT_WEST_OUTSIDE,  { 1195.3f, 2366.941f, 438.0f, 0.0f }, POINT_WEST_START,  { 1195.299f, 2348.941f, 435.0f, 0.0f }, POINT_WEST_END,  { 1195.392f, 2263.441f, 435.0f, 0.0f }, POINT_EAST_OUTSIDE,  { 1195.4f, 2243.441f, 438.0f, 0.0f } }, // West  -> East
    { POINT_EAST_OUTSIDE,  { 1195.4f, 2243.441f, 438.0f, 0.0f }, POINT_EAST_START,  { 1195.392f, 2263.441f, 435.0f, 0.0f }, POINT_EAST_END,  { 1195.299f, 2348.941f, 435.0f, 0.0f }, POINT_WEST_OUTSIDE,  { 1195.3f, 2366.941f, 438.0f, 0.0f } }  // East  -> West
};

Position const CenterPos = { 1195.0f, 2304.0f, 438.0f, 0.0f };

enum eSpells
{
    SPELL_PREY_TIME              = 106933,
    SPELL_IMPALING_STRIKE        = 107047,

    SPELL_STRAFING_RUN           = 107342,
    SPELL_STRAFIND_RUN_DMG       = 116298,

    SPELL_PREY_TIME_VEHICLE_AURA = 106934,
    SPELL_EJECT_PASSENGERS       = 68576,

    // Disruptor
    SPELL_BOMB                   = 115110,
    SPELL_TELEPORT_VISUAL        = 52096,

    // Flak Cannon
    SPELL_FIRE_FLAK_CANNON       = 116554,
    SPELL_FLAK_CANNON_SHOT       = 116553
};

enum eEvents
{
    EVENT_PREY_TIME         = 1,
    EVENT_IMPALING_STRIKE   = 2,
    EVENT_DISRUPTOR_BOMBARD = 3,
    EVENT_SUMMONS           = 4,
    EVENT_MOVE              = 5,
    EVENT_CHECK             = 6,
    EVENT_CHECK_WIPE        = 7,
};

enum ePhases
{
    PHASE_MAIN        = 1,
    PHASE_NORTH_SOUTH = 2,
    PHASE_WEST_EAST   = 3
};

enum eActions
{
    ACTION_INTRO = 1
};

enum eTalks
{
    SAY_INTRO       = 0,
    SAY_AGGRO       = 1,
    SAY_DEATH       = 2,
    SAY_SLAY        = 3,
    SAY_STIKE_EMOTE = 4
};

#define RADIUS_CIRCLE   100.0f

class boss_striker_gadok : public CreatureScript
{
    public:
        boss_striker_gadok() : CreatureScript("boss_striker_gadok") { }

        struct boss_striker_gadokAI : public BossAI
        {
            boss_striker_gadokAI(Creature* creature) : BossAI(creature, DATA_GADOK) { }

            void InitializeAI() OVERRIDE
            {
                me->setActive(true);
                intro = false;
                Reset();
            }

            void Reset() OVERRIDE
            {
                _Reset();
                isStrafing = false;

                strafingEventProgress = 0;
                move                  = 0;
                curPct                = 70;
                targetGUID            = 0;
                x = 0.0f; y = 0.0f;
                me->GetMotionMaster()->Clear();
                events.Reset();
                exploitEvents.Reset();

                me->SetCanFly(true);
                me->SetSpeed(MOVE_FLIGHT, 3.5f);
                me->AddUnitMovementFlag(MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING);
                me->SetAnimTier(ANIM_TIER_HOVER);
                me->SetDisableGravity(true);
                me->SetReactState(REACT_AGGRESSIVE);

                instance->SetData(DATA_GADOK, PHASE_MAIN);
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                if (actionId == ACTION_INTRO && !intro)
                {
                    intro = true;
                    Talk(SAY_INTRO);
                }
            }

            void KilledUnit(Unit* victim) OVERRIDE
            {
                if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(SAY_SLAY);
            }

            void DoSpawnKrikThik(uint8 count)
            {
                for (uint8 i = 0; i < count; ++i)
                    SummonKrikThik(NPC_KRIKTHIK_DISRUPTOR);

                for (uint8 i = 0; i < count; ++i)
                    SummonKrikThik(NPC_KRIKTHIK_STRIKER);
            }

            TempSummon* SummonKrikThik(uint32 creatureId)
            {
                float angle = frand(0, 2 * M_PI);
                float posX = CenterPos.GetPositionX() + (RADIUS_CIRCLE * std::cos(angle));
                float posY = CenterPos.GetPositionY() + (RADIUS_CIRCLE * std::sin(angle));

                return me->SummonCreature(creatureId, posX, posY, CenterPos.GetPositionZ());
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();
                Talk(SAY_AGGRO);

                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                instance->SetData(DATA_GADOK, IN_PROGRESS);

                events.ScheduleEvent(EVENT_PREY_TIME, urand(9 * IN_MILLISECONDS, 15 * IN_MILLISECONDS), PHASE_MAIN);
                events.ScheduleEvent(EVENT_IMPALING_STRIKE, urand(13 * IN_MILLISECONDS, 19 * IN_MILLISECONDS), PHASE_MAIN);
                events.ScheduleEvent(EVENT_SUMMONS, 2 * IN_MILLISECONDS);
                exploitEvents.ScheduleEvent(EVENT_CHECK_WIPE, 1 * IN_MILLISECONDS);
            }

            void EnterEvadeMode() OVERRIDE
            {
                summons.DespawnAll();
                exploitEvents.Reset();
                isStrafing = false;
                strafingEventProgress = 0;
                me->RemoveChanneledCast(targetGUID);

                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                instance->SetBossState(DATA_GADOK, FAIL);

                _DespawnAtEvade();
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                switch (pointId)
                {
                    case POINT_NORTH_START:
                    case POINT_SOUTH_START:
                    case POINT_WEST_START:
                    case POINT_EAST_START:
                    case POINT_NORTH_END:
                    case POINT_SOUTH_END:
                    case POINT_WEST_END:
                    case POINT_EAST_END:
                    case POINT_NORTH_OUTSIDE:
                    case POINT_SOUTH_OUTSIDE:
                    case POINT_WEST_OUTSIDE:
                    case POINT_EAST_OUTSIDE:
                        DoStrafingEvent();
                        break;
                    default:
                        break;
                }
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                if (isStrafing && damage >= me->GetHealth())
                    damage = 0;

                if (curPct > 0 && HealthBelowPct(uint32(curPct)) && !isStrafing)
                {
                    curPct -= 40;

                    if (Unit* vict = me->GetVictim())
                    {
                        targetGUID = vict->GetGUID();
                        me->PrepareChanneledCast(me->GetOrientation());
                    }

                    DoStrafingEvent();
                }
            }

            uint8 SelectNextStartPoint()
            {
                // Gadok goes counter-clockwise around the tower
                switch (move)
                {
                    case MOV_NORTH_SOUTH:
                        return MOV_WEST_EAST;
                    case MOV_SOUTH_NORTH:
                        return MOV_EAST_WEST;
                    case MOV_WEST_EAST:
                        return MOV_NORTH_SOUTH;
                    case MOV_EAST_WEST:
                        return MOV_SOUTH_NORTH;
                    default:
                        break;
                }

                return MOV_NORTH_SOUTH;
            }

            void MoveToStrafPoint(uint8 pointId, Position const& pos)
            {
                me->GetMotionMaster()->MovePoint(pointId, pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ());
            }

            void DoStrafingEvent()
            {
                if (Vehicle* vehicle = me->GetVehicleKit())
                    if (!vehicle->HasEmptySeat(0))
                        me->CastSpell(me, SPELL_EJECT_PASSENGERS, TRIGGERED_FULL_MASK);

                switch (strafingEventProgress)
                {
                    case 0: // Begin, Gadok is at 70% or 30% health, he goes to the first POINT_START
                        isStrafing = true;
                        me->SetSpeed(MOVE_FLIGHT, 3.5f, true);
                        me->SetReactState(REACT_PASSIVE);
                        events.CancelEventGroup(PHASE_MAIN);

                        move = urand(MOV_NORTH_SOUTH, MOV_EAST_WEST);

                        instance->SetData(DATA_GADOK, move <= MOV_SOUTH_NORTH ? PHASE_NORTH_SOUTH : PHASE_WEST_EAST);

                        MoveToStrafPoint(StrafPoints[move].pointIdBegin, StrafPoints[move].begin);

                        for (uint8 i = 0; i < 2; ++i)
                            if (Creature* striker = SummonKrikThik(NPC_KRIKTHIK_STRIKER))
                                if (striker->AI())
                                    striker->AI()->DoAction(0);

                        ++strafingEventProgress;
                        events.ScheduleEvent(EVENT_DISRUPTOR_BOMBARD, 100);
                        break;
                    case 1: // We are at POINT_START, wait 2 sec then continue
                        ++strafingEventProgress;
                        events.ScheduleEvent(EVENT_DISRUPTOR_BOMBARD, 2000);
                        break;
                    case 2: // 2 sec passed, move to POINT_END with the spell
                        me->SetSpeed(MOVE_FLIGHT, 2.0f);
                        MoveToStrafPoint(StrafPoints[move].pointIdEnd, StrafPoints[move].end);
                        me->CastSpell(me, SPELL_STRAFING_RUN, true);

                        ++strafingEventProgress;
                        break;
                    case 3: // We are at POINT_END, let movement finish then continue
                        ++strafingEventProgress;
                        events.ScheduleEvent(EVENT_DISRUPTOR_BOMBARD, 50);
                        break;
                    case 4: // First strafing finished, we are at POINT_END and go to POINT_OUTSIDE
                        instance->SetData(DATA_GADOK, PHASE_MAIN);

                        MoveToStrafPoint(StrafPoints[move].pointIdOutside, StrafPoints[move].outside);
                        ++strafingEventProgress;
                        break;
                    case 5: // We are at POINT_OUTSIDE, let movement finish then continue
                        ++strafingEventProgress;
                        events.ScheduleEvent(EVENT_DISRUPTOR_BOMBARD, 50);
                        break;
                    case 6: // We are at POINT_OUTSIDE, go to the next POINT_OUTSIDE_START
                        move = SelectNextStartPoint();

                        MoveToStrafPoint(StrafPoints[move].pointIdBeginOutside, StrafPoints[move].beginOutside);
                        ++strafingEventProgress;
                        break;
                    case 7: // We are at POINT_OUTSIDE_START, let movement finish then continue
                        ++strafingEventProgress;
                        events.ScheduleEvent(EVENT_DISRUPTOR_BOMBARD, 50);
                        break;
                    case 8: // We are at POINT_OUTSIDE_START, go to POINT_START
                        instance->SetData(DATA_GADOK, move <= MOV_SOUTH_NORTH ? PHASE_NORTH_SOUTH : PHASE_WEST_EAST);

                        MoveToStrafPoint(StrafPoints[move].pointIdBegin, StrafPoints[move].begin);

                        ++strafingEventProgress;
                        break;
                    case 9: // Just arrived at the second POINT_START
                        ++strafingEventProgress;
                        events.ScheduleEvent(EVENT_DISRUPTOR_BOMBARD, 50);
                        break;
                    case 10: // Move to POINT_END with the spell
                        me->SetSpeed(MOVE_FLIGHT, 2.0f, true);
                        MoveToStrafPoint(StrafPoints[move].pointIdEnd, StrafPoints[move].end);
                        me->CastSpell(me, SPELL_STRAFING_RUN, true);

                        ++strafingEventProgress;
                        break;
                    case 11: // We are at POINT_END, let movement finish then continue
                        ++strafingEventProgress;
                        events.ScheduleEvent(EVENT_DISRUPTOR_BOMBARD, 50);
                        break;
                    case 12: // POINT_END, end the strafing event and go back to the fight
                        instance->SetData(DATA_GADOK, PHASE_MAIN);

                        StopStrafingEvent();

                        events.ScheduleEvent(EVENT_PREY_TIME, urand(9 * IN_MILLISECONDS, 15 * IN_MILLISECONDS), PHASE_MAIN);
                        events.ScheduleEvent(EVENT_IMPALING_STRIKE, urand(13 * IN_MILLISECONDS, 19 * IN_MILLISECONDS), PHASE_MAIN);
                        break;
                    default:
                        break;
                }
            }

            void StopStrafingEvent()
            {
                move                  = 0;
                strafingEventProgress = 0;
                me->SetSpeed(MOVE_FLIGHT, 1.134f, true);

                float ori = me->GetAngle(&ElevatorCenterPos);
                float dist = me->GetExactDist2d(&ElevatorCenterPos);
                GetPositionWithDistInOrientation(me, dist, ori, x, y);

                Movement::MoveSplineInit pInit(me);
                pInit.MoveTo(x + frand(-7.5f, 7.5f), y + 8.0f, 433.04f);
                pInit.Launch();

                me->m_Events.Schedule(me->GetSplineDuration(), 4, [this]()
                {
                    me->RemoveChanneledCast(targetGUID);
                    isStrafing = false;
                });
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                summons.Summon(summon);

                switch (summon->GetEntry())
                {
                    case NPC_KRIKTHIK_STRIKER:
                    case NPC_KRIKTHIK_DISRUPTOR:
                        summon->SetSpeed(MOVE_FLIGHT, 2);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                exploitEvents.Update(diff);

                if (exploitEvents.ExecuteEvent() == EVENT_CHECK_WIPE)
                {
                    if (!HasAnyPlayerInTower())
                    {
                        EnterEvadeMode();
                        return;
                    }

                    exploitEvents.ScheduleEvent(EVENT_CHECK_WIPE, 1 * IN_MILLISECONDS);
                }

                events.Update(diff);

                if (!UpdateVictim())
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_PREY_TIME:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 15.0f, true))
                                me->CastSpell(target, SPELL_PREY_TIME, false);
                            events.ScheduleEvent(EVENT_PREY_TIME, urand(9 * IN_MILLISECONDS, 15 * IN_MILLISECONDS) + 5000, PHASE_MAIN);
                            break;
                        case EVENT_IMPALING_STRIKE:
                            if (Unit* vict = me->GetVictim())
                                me->CastSpell(vict, SPELL_IMPALING_STRIKE, false);
                            events.ScheduleEvent(EVENT_IMPALING_STRIKE, urand(13 * IN_MILLISECONDS, 19 * IN_MILLISECONDS), PHASE_MAIN);
                            break;
                        case EVENT_SUMMONS:
                            DoSpawnKrikThik(2);
                            events.ScheduleEvent(EVENT_SUMMONS, urand(20 * IN_MILLISECONDS, 28 * IN_MILLISECONDS));
                            break;
                        case EVENT_DISRUPTOR_BOMBARD:
                            DoStrafingEvent();
                            break;
                        default:
                            break;
                    }
                }

                if (Vehicle* vehicle = me->GetVehicleKit())
                    if (vehicle->HasEmptySeat(0))
                        DoMeleeAttackIfReady();
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                instance->SaveToDB();

                Talk(SAY_DEATH);
                Talk(SAY_STIKE_EMOTE);
            }

        private:
            EventMap exploitEvents;
            bool isStrafing, intro;
            uint8 strafingEventProgress, move;
            int32 curPct;
            uint64 targetGUID;
            float x, y;

            bool HasAnyPlayerInTower()
            {
                // At least check that we are on the tower
                if (me->GetPositionZ() < 426.40f)
                    return false;

                std::list<Player*> pList;
                GetPlayerListInGrid(pList, me, 200.0f);

                for (Player* player : pList)
                    if (player->GetPositionZ() > 426.40f)
                        return true;

                return false;
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_striker_gadokAI(creature);
        }
};

class npc_krikthik_striker : public CreatureScript
{
    public:
        npc_krikthik_striker() : CreatureScript("npc_krikthik_striker") { }

        struct npc_krikthik_strikerAI : public ScriptedAI
        {
            npc_krikthik_strikerAI(Creature* creature) : ScriptedAI(creature), x(0.0f), y(0.0f) { }

            void Reset() OVERRIDE
            {
                me->SetSpeed(MOVE_RUN, 2.0f);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                x = 0.0f; y = 0.0f;
                HandleMoveToPlatform();
            }

            void HandleMoveToPlatform()
            {
                float ori = me->GetAngle(&ElevatorCenterPos);
                float dist = me->GetExactDist2d(&ElevatorCenterPos);
                GetPositionWithDistInOrientation(me, dist, ori, x, y);

                Movement::MoveSplineInit pInit(me);
                pInit.MoveTo(x + frand(-7.5f, 7.5f), y + 8.0f, 433.04f);

                pInit.SetWalk(false);
                pInit.Launch();
                nonCombatEvents.ScheduleEvent(EVENT_MOVE, me->GetSplineDuration());
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                nonCombatEvents.Update(diff);

                if (nonCombatEvents.ExecuteEvent() == EVENT_MOVE)
                {
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);

                    if (Player* player = me->FindNearestPlayer(60.0f))
                    {
                        me->Attack(player, true);
                        me->GetMotionMaster()->MoveChase(player);
                    }
                }

                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }

        private:
            EventMap nonCombatEvents;
            float x, y;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_krikthik_strikerAI(creature);
        }
};

class npc_krikthik_disruptor : public CreatureScript
{
    public:
        npc_krikthik_disruptor() : CreatureScript("npc_krikthik_disruptor") { }

        struct npc_krikthik_disruptorAI : public ScriptedAI
        {
            npc_krikthik_disruptorAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
            }

            void Reset() OVERRIDE
            {
                SetCombatMovement(false);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                events.ScheduleEvent(EVENT_CHECK, 500);
                actualAngle = me->GetAngle(CenterPos.GetPositionX(), CenterPos.GetPositionY());
                direction = urand(0, 1);

                myPositionZ = direction ? 435.0f : 440.0f;

                // Enable movements
                MovementInform(POINT_MOTION_TYPE, POINT_KRIKTHIK_CIRCLE);
                events.ScheduleEvent(EVENT_DISRUPTOR_BOMBARD, urand(5000, 20000));

                me->setActive(true);
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                if (pointId == POINT_KRIKTHIK_CIRCLE)
                    events.ScheduleEvent(EVENT_MOVE, 50);
            }

            void SelectNextWaypoint(float& x, float& y)
            {
                if (direction)
                    actualAngle -= M_PI / 8;
                else
                    actualAngle += M_PI / 8;

                x = CenterPos.GetPositionX() + (me->GetObjectSize() + RADIUS_CIRCLE) * std::cos(actualAngle);
                y = CenterPos.GetPositionY() + (me->GetObjectSize() + RADIUS_CIRCLE) * std::sin(actualAngle);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CHECK:
                            if (me->GetMap()->GetPlayers().isEmpty())
                            {
                                me->DespawnOrUnsummon();
                                return;
                            }
                            events.ScheduleEvent(EVENT_CHECK, 5 * IN_MILLISECONDS);
                            break;
                        case EVENT_MOVE:
                        {
                            float x = 0.0f;
                            float y = 0.0f;
                            SelectNextWaypoint(x, y);

                            me->GetMotionMaster()->MovePoint(POINT_KRIKTHIK_CIRCLE, x, y, myPositionZ);
                            break;
                        }
                        case EVENT_DISRUPTOR_BOMBARD:
                        {
                            if (instance && instance->GetBossState(DATA_GADOK) == IN_PROGRESS)
                            {
                                std::list<Player*> playerList;
                                GetPlayerListInGrid(playerList, me, 200.0f);
                                if (!playerList.empty())
                                    me->CastSpell(Skyfire::Containers::SelectRandomContainerElement(playerList), SPELL_BOMB, true);
                            }

                            events.ScheduleEvent(EVENT_DISRUPTOR_BOMBARD, urand(5000, 20000));
                            break;
                        }
                        default:
                            break;
                    }
                }
            }

        private:
            InstanceScript* instance;
            EventMap events;
            bool direction;
            float myPositionZ, actualAngle;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_krikthik_disruptorAI(creature);
        }
};

class npc_flak_cannon : public CreatureScript
{
    public:
        npc_flak_cannon() : CreatureScript("npc_flak_cannon") { }

        struct npc_flak_cannonAI : public ScriptedAI
        {
            npc_flak_cannonAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
            }

            void Reset() OVERRIDE { }

            void SpellHit(Unit* /*caster*/, SpellInfo const* spell) OVERRIDE
            {
                if (!instance || instance->GetBossState(DATA_GADOK) != DONE)
                    return;

                if (spell->Id == SPELL_FIRE_FLAK_CANNON)
                {
                    for (uint8 i = 0; i < 5; ++i)
                    {
                        if (Creature* bombarder = instance->instance->GetCreature(instance->GetData64(DATA_RANDOM_BOMBARDER)))
                        {
                            me->CastSpell(bombarder, SPELL_FLAK_CANNON_SHOT, true);
                            bombarder->GetMotionMaster()->MoveFall();
                            bombarder->DespawnOrUnsummon(2000);
                        }
                    }
                }
            }

        private:
            InstanceScript* instance;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_flak_cannonAI(creature);
        }
};

class spell_gadok_strafing : public SpellScriptLoader
{
    public:
        spell_gadok_strafing() : SpellScriptLoader("spell_gadok_strafing") { }

        class spell_gadok_strafing_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gadok_strafing_SpellScript);

            void HandleBeforeCast()
            {
                Unit* caster = GetCaster();
                if (!caster)
                    return;

                InstanceScript* instance = caster->GetInstanceScript();
                if (!instance)
                    return;

                uint32 actualStrafPhase = instance->GetData(DATA_GADOK);
                if (actualStrafPhase == PHASE_MAIN)
                    return;

                uint32 stalkerEntry = actualStrafPhase == PHASE_NORTH_SOUTH ? NPC_STALKER_NORTH_SOUTH : NPC_STALKER_WEST_EAST;

                std::list<Creature*> stalkerList;
                GetCreatureListWithEntryInGrid(stalkerList, caster, stalkerEntry, 10.0f);

                for (Creature* stalker : stalkerList)
                    if (!stalker->HasAura(SPELL_STRAFIND_RUN_DMG))
                        stalker->CastSpell(stalker, SPELL_STRAFIND_RUN_DMG, true);
            }

            void Register() OVERRIDE
            {
                BeforeCast += SpellCastFn(spell_gadok_strafing_SpellScript::HandleBeforeCast);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_gadok_strafing_SpellScript();
        }
};

class spell_prey_time : public SpellScriptLoader
{
    public:
        spell_prey_time() : SpellScriptLoader("spell_prey_time") { }

        class spell_prey_time_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_prey_time_SpellScript);

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();

                if (!caster || !target)
                    return;

                target->CastSpell(caster, uint32(GetSpellInfo()->Effects[EFFECT_1].BasePoints), false);
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_prey_time_SpellScript::HandleScript, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_prey_time_SpellScript();
        }
};

class PreyTimeTargetSelector
{
    public:
        PreyTimeTargetSelector() { }

        bool operator()(WorldObject* object)
        {
            if (Creature* cre = object->ToCreature())
                if (cre->GetEntry() == NPC_GADOK)
                    return false;

            return true;
        }
};

class spell_prey_time_vehicle_aura : public SpellScriptLoader
{
    public:
        spell_prey_time_vehicle_aura() : SpellScriptLoader("spell_prey_time_vehicle_aura") { }

        class spell_prey_time_vehicle_aura_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_prey_time_vehicle_aura_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(PreyTimeTargetSelector());
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_prey_time_vehicle_aura_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_prey_time_vehicle_aura_SpellScript();
        }
};

void AddSC_boss_striker_gadok()
{
    new boss_striker_gadok();
    new npc_krikthik_striker();
    new npc_krikthik_disruptor();
    new npc_flak_cannon();
    new spell_gadok_strafing();
    new spell_prey_time();
    new spell_prey_time_vehicle_aura();
}
