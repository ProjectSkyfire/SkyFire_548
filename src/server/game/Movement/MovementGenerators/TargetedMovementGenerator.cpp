/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ByteBuffer.h"
#include "Chat.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "Errors.h"
#include "MoveSpline.h"
#include "MoveSplineInit.h"
#include "PetTransportSupport.h"
#include "Player.h"
#include "TargetedMovementGenerator.h"
#include "World.h"

#include <cmath>

namespace
{
    float const TransportPetFollowTolerance = 0.25f;
    float const ChaseDiveRelaunchZ = 2.0f;
    // Retail land chase relaunches about every 400ms. Far chase uses a shorter
    // tick plus a 5y dest slack so a fast cut does not keep the old spline
    // (10y/400ms felt like they ran to where you were).
    uint32 const ChaseRepathIntervalMs = 400;
    uint32 const ChaseFarRepathIntervalMs = 200;
    float const ChaseUnchangedDestDist = 1.0f;
    float const ChaseFollowLeadYards = 2.5f;
    float const ChaseAimMovedYards = 1.5f;
    float const ChaseUrgentDestOff = 8.0f;

    bool TargetIsActivelyMoving(Unit const* target, float lastAimX, float lastAimY, bool lastAimValid)
    {
        if (target->isMoving())
            return true;
        // Creature victims move on splines and often lack player movement flags.
        if (target->GetTypeId() != TypeID::TYPEID_PLAYER && !target->movespline->Finalized())
            return true;
        // Strafe start/stop drops MOVEMENTFLAG_MASK_MOVING for a tick while the
        // client is still relocating. Treat a real position delta as moving so
        // we do not StopMoving and swing as a start/stop hitch.
        if (lastAimValid)
        {
            float const dx = target->GetPositionX() - lastAimX;
            float const dy = target->GetPositionY() - lastAimY;
            if (dx * dx + dy * dy > 0.25f)
                return true;
        }
        return false;
    }

    bool GetTargetMoveDir(Unit const* target, float lastAimX, float lastAimY, bool lastAimValid, float& mx, float& my)
    {
        if (lastAimValid)
        {
            mx = target->GetPositionX() - lastAimX;
            my = target->GetPositionY() - lastAimY;
            if (mx * mx + my * my > 0.04f)
                return true;
        }

        mx = 0.0f;
        my = 0.0f;
        float const o = target->GetOrientation();
        float const c = std::cos(o);
        float const s = std::sin(o);
        uint32 const f = target->GetUnitMovementFlags();
        if (f & MOVEMENTFLAG_FORWARD)
        {
            mx += c;
            my += s;
        }
        if (f & MOVEMENTFLAG_BACKWARD)
        {
            mx -= c;
            my -= s;
        }
        if (f & MOVEMENTFLAG_STRAFE_LEFT)
        {
            mx -= s;
            my += c;
        }
        if (f & MOVEMENTFLAG_STRAFE_RIGHT)
        {
            mx += s;
            my -= c;
        }
        return (mx * mx + my * my) > 0.0001f;
    }

    float ChaseMeleeStopDistance(Unit const* owner, Unit const* target)
    {
        // One yard inside IsWithinMeleeRange (reaches + MELEE_RANGE). Stopping on
        // the swing edge is the sweet spot where they stand and never hit.
        float dist = owner->GetMeleeReach() + target->GetMeleeReach();
        if (dist < CONTACT_DISTANCE)
            dist = CONTACT_DISTANCE;
        return dist;
    }

    void GetOwnerLiveXY(Unit const* owner, float& x, float& y)
    {
        if (!owner->movespline->Finalized())
        {
            Movement::Location const loc = owner->movespline->ComputePosition();
            x = loc.x;
            y = loc.y;
            return;
        }
        x = owner->GetPositionX();
        y = owner->GetPositionY();
    }

    // Dest is the melee ring on the player-to-mob line. Far away, run at the
    // player. While they run in melee, the ring sits on our side of a short
    // lead so we follow without running through them.
    void GetSteeringChaseDest(Unit const* owner, Unit const* target, bool targetMoving,
        float lastAimX, float lastAimY, bool lastAimValid, float& x, float& y, float& z)
    {
        float const stop = ChaseMeleeStopDistance(owner, target);
        float ox, oy;
        GetOwnerLiveXY(owner, ox, oy);

        float const tx = target->GetPositionX();
        float const ty = target->GetPositionY();
        float dx = ox - tx;
        float dy = oy - ty;
        float len = std::sqrt(dx * dx + dy * dy);
        if (len < 0.05f)
        {
            dx = -std::cos(target->GetOrientation());
            dy = -std::sin(target->GetOrientation());
            len = 1.0f;
        }

        if (len > stop + 8.0f)
        {
            x = tx;
            y = ty;
        }
        else if (targetMoving)
        {
            // Melee ring on our side of where the player is going — never a
            // point past them. An 8y lead-through dest ran them out of swing
            // range (172814 log: ATTACK_START/STOP flicker).
            float fx = tx;
            float fy = ty;
            float mx, my;
            if (GetTargetMoveDir(target, lastAimX, lastAimY, lastAimValid, mx, my))
            {
                float const mlen = std::sqrt(mx * mx + my * my);
                float const lead = std::min(ChaseFollowLeadYards, stop);
                fx += mx / mlen * lead;
                fy += my / mlen * lead;
            }
            float rdx = ox - fx;
            float rdy = oy - fy;
            float rlen = std::sqrt(rdx * rdx + rdy * rdy);
            if (rlen < 0.05f)
            {
                x = ox;
                y = oy;
            }
            else
            {
                x = fx + rdx / rlen * stop;
                y = fy + rdy / rlen * stop;
            }
        }
        else if (len >= stop)
        {
            x = tx + dx / len * stop;
            y = ty + dy / len * stop;
        }
        else
        {
            x = ox;
            y = oy;
        }
        z = target->GetPositionZ();
    }

    float ChaseSplineDestOffPlayer(Unit const* owner, Unit const* target)
    {
        G3D::Vector3 const dest = owner->movespline->FinalDestination();
        float const dx = target->GetPositionX() - dest.x;
        float const dy = target->GetPositionY() - dest.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    bool ChaseShouldRepath(Unit const* owner, Unit const* target,
        float lastAimX, float lastAimY, float lastAimZ, bool hasLastAim)
    {
        if (owner->movespline->Finalized())
            return true;

        // Follow the live player, not the old dest. dest-to-player slack kept
        // the spline until they left a radius around where we were already
        // going, so a nearby juke finished the old path first.
        if (hasLastAim)
        {
            float const ax = target->GetPositionX() - lastAimX;
            float const ay = target->GetPositionY() - lastAimY;
            if (ax * ax + ay * ay > ChaseAimMovedYards * ChaseAimMovedYards)
                return true;
            if (std::fabs(target->GetPositionZ() - lastAimZ) >= ChaseDiveRelaunchZ)
                return true;
        }

        float ox, oy;
        GetOwnerLiveXY(owner, ox, oy);
        float const tx = target->GetPositionX();
        float const ty = target->GetPositionY();
        G3D::Vector3 const splineDest = owner->movespline->FinalDestination();
        float const toPlayerX = tx - ox;
        float const toPlayerY = ty - oy;
        float const toDestX = splineDest.x - ox;
        float const toDestY = splineDest.y - oy;
        float const playerDistSq = toPlayerX * toPlayerX + toPlayerY * toPlayerY;
        float const destDistSq = toDestX * toDestX + toDestY * toDestY;
        if (playerDistSq > 0.01f && destDistSq > 0.01f)
        {
            float const invP = 1.0f / std::sqrt(playerDistSq);
            float const invD = 1.0f / std::sqrt(destDistSq);
            float const headingDot = (toPlayerX * invP) * (toDestX * invD) + (toPlayerY * invP) * (toDestY * invD);
            if (headingDot < 0.90f) // ~25 degrees
                return true;
        }

        return false;
    }

    char const* GetPathTypeString(PathType type)
    {
        if (type & PATHFIND_NOPATH)
            return "NOPATH";
        if (type & PATHFIND_INCOMPLETE)
            return "INCOMPLETE";
        if (type & PATHFIND_NOT_USING_PATH)
            return "SHORTCUT";
        if (type & PATHFIND_SHORT)
            return "SHORT";
        if (type & PATHFIND_NORMAL)
            return "NORMAL";
        if (type & PATHFIND_SHORTCUT)
            return "SHORTCUT";
        return "BLANK";
    }

    void ReportLiveChaseDebug(Creature* owner, Unit* target, PathGenerator const* path, char const* reason, bool targetMoved, bool recalc)
    {
        if (!owner || !target || !owner->GetMap())
            return;

        uint32 const now = getMSTime();
        Map::PlayerList const& players = owner->GetMap()->GetPlayers();
        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            Player* player = itr->GetSource();
            if (!player || !player->GetSession() || !player->isDebugChaseLive || player->debugChaseCreatureGuid != owner->GetGUID())
                continue;

            if (now - player->debugChaseLastReportMSTime < 250 && !targetMoved && !recalc)
                continue;

            player->debugChaseLastReportMSTime = now;
            G3D::Vector3 splineDest = owner->movespline->FinalDestination();
            G3D::Vector3 steeringTarget = path ? path->GetSteeringTarget() : splineDest;
            ChatHandler handler(player->GetSession());
            handler.PSendSysMessage("[chase] %s -> %s reason=%s moved=%u recalc=%u melee=%u finalized=%u",
                owner->GetName().c_str(), target->GetName().c_str(), reason, targetMoved ? 1 : 0, recalc ? 1 : 0,
                owner->IsWithinMeleeRange(target) ? 1 : 0, owner->movespline->Finalized() ? 1 : 0);
            handler.PSendSysMessage("[chase] mob=(%.2f, %.2f, %.2f) target=(%.2f, %.2f, %.2f) spline=(%.2f, %.2f, %.2f) steer=(%.2f, %.2f, %.2f)",
                owner->GetPositionX(), owner->GetPositionY(), owner->GetPositionZ(),
                target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(),
                splineDest.x, splineDest.y, splineDest.z,
                steeringTarget.x, steeringTarget.y, steeringTarget.z);
            if (path)
            {
                handler.PSendSysMessage("[chase] pathType=%s points=" SIZEFMTD " raw=%u steering=%u actualEnd=(%.2f, %.2f, %.2f)",
                    GetPathTypeString(path->GetPathType()), path->GetPath().size(), path->GetRawPathPointCount(), path->IsUsingChaseSteering() ? 1 : 0,
                    path->GetActualEndPosition().x, path->GetActualEndPosition().y, path->GetActualEndPosition().z);
                if (player->IsGameMaster())
                    for (Movement::PointsArray::const_iterator point = path->GetPath().begin(); point != path->GetPath().end(); ++point)
                        player->SummonCreature(VISUAL_WAYPOINT, point->x, point->y, point->z, 0.0f, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 700);
            }
        }
    }
}

template<class T, typename D>
void TargetedMovementGeneratorMedium<T, D>::_setTargetLocation(T* owner, bool updateDestination)
{
    if (!i_target.isValid() || !i_target->IsInWorld())
        return;

    if (owner->HasUnitState(UNIT_STATE_NOT_MOVE))
        return;

    if (owner->GetTypeId() == TypeID::TYPEID_UNIT && !i_target->isInAccessiblePlaceFor(owner->ToCreature()))
        return;

    float x, y, z;

    if (Skyfire::PetTransport::ShouldUseTransportLocalPetFollow(owner->IsPet(),
            owner->GetOwnerGUID() == i_target->GetGUID(), owner->GetTransport() != NULL,
            owner->GetTransport() && owner->GetTransport() == i_target->GetTransport(), i_offset > 0.0f))
    {
        Skyfire::PetTransport::PassengerOffset const offset = Skyfire::PetTransport::CalculateFollowerPosition(
            i_target->GetTransOffsetX(), i_target->GetTransOffsetY(), i_target->GetTransOffsetZ(), i_target->GetTransOffsetO(),
            i_target->GetCombatReach(), 0.0f, i_offset, i_angle);

        if (!Skyfire::PetTransport::ShouldMoveTransportPetToFollowOffset(owner->GetTransOffsetX(), owner->GetTransOffsetY(),
                owner->GetTransOffsetZ(), offset.X, offset.Y, offset.Z, TransportPetFollowTolerance))
            return;

        delete i_path;
        i_path = NULL;

        D::_addUnitStateMove(owner);
        i_targetReached = false;
        i_recalculateTravel = false;
        owner->AddUnitState(UNIT_STATE_CHASE);

        Movement::MoveSplineInit init(owner);
        init.DisableTransportPathTransformations();
        init.MoveTo(offset.X, offset.Y, offset.Z, false, true);
        init.SetWalk(((D*)this)->EnableWalking());
        init.Launch();
        return;
    }

    if (updateDestination || !i_path)
    {
        if (!i_offset)
        {
            // Path to the melee ring on the line from the player to the mob.
            // GetContactPoint orbits the hitbox; player center runs through them.
            if (((D*)this)->UseSteering())
            {
                bool const targetMoving = TargetIsActivelyMoving(i_target.getTarget(), i_lastAimX, i_lastAimY, i_hasLastAim);
                GetSteeringChaseDest(owner, i_target.getTarget(), targetMoving,
                    i_lastAimX, i_lastAimY, i_hasLastAim, x, y, z);
            }
            else
                i_target->GetContactPoint(owner, x, y, z);
        }
        else
        {
            float dist;
            float size;

            // Pets need special handling.
            // We need to subtract GetObjectSize() because it gets added back further down the chain
            //  and that makes pets too far away. Subtracting it allows pets to properly
            //  be (GetCombatReach() + i_offset) away.
            // Only applies when i_target is pet's owner otherwise pets and mobs end up
            //   doing a "dance" while fighting
            if (owner->IsPet() && i_target->GetTypeId() == TypeID::TYPEID_PLAYER)
            {
                dist = i_target->GetCombatReach();
                size = i_target->GetCombatReach() - i_target->GetObjectSize();
            }
            else
            {
                dist = i_offset + 1.0f;
                size = owner->GetObjectSize();
            }

            if (i_target->IsWithinDistInMap(owner, dist))
                return;

            // to at i_offset distance from target and i_angle from target facing
            i_target->GetClosePoint(x, y, z, size, i_offset, i_angle);
        }

        // GetClosePoint/GetContactPoint call UpdateAllowedPositionZ at the offset XY,
        // which lifts dest onto nearby vmaps (trees, roofs). Follow/chase at the
        // target's actual height; pathfinding owns ground Z.
        z = i_target->GetPositionZ();
    }
    else
    {
        // the destination has not changed, we just need to refresh the path (usually speed change)
        G3D::Vector3 end = i_path->GetEndPosition();
        x = end.x;
        y = end.y;
        z = end.z;
    }

    if (!i_path)
        i_path = new PathGenerator(owner);

    i_path->SetChaseSteering(((D*)this)->UseSteering());

    // allow pets to use shortcut if no path found when following their master
    bool forceDest = (owner->GetTypeId() == TypeID::TYPEID_UNIT && owner->ToCreature()->IsPet()
        && owner->HasUnitState(UNIT_STATE_FOLLOW));

    bool result = i_path->CalculatePath(x, y, z, forceDest);
    if (!result || (i_path->GetPathType() & PATHFIND_NOPATH))
    {
        // Cant reach target
        i_recalculateTravel = true;
        if (owner->GetTypeId() == TypeID::TYPEID_UNIT)
            ReportLiveChaseDebug(owner->ToCreature(), i_target.getTarget(), i_path, "nopath", false, true);
        return;
    }

    // Path to the player, then cut the end back to melee so we stop on the
    // hitbox instead of running through them. Do not trim the start of the path.
    // In melee the start is already inside the stop radius, so shorten is a
    // no-op; dest must be the near-side ring (see GetSteeringChaseDest).
    bool const steeringChase = ((D*)this)->UseSteering();
    bool const inMeleeNow = owner->IsWithinMeleeRange(i_target.getTarget());
    if (steeringChase && !inMeleeNow)
    {
        float meleeDist = ChaseMeleeStopDistance(owner, i_target.getTarget());
        i_path->ShortenPathUntilDist(G3D::Vector3(i_target->GetPositionX(), i_target->GetPositionY(), i_target->GetPositionZ()), meleeDist);
    }
    if (steeringChase)
    {
        float lx, ly;
        GetOwnerLiveXY(owner, lx, ly);
        i_path->DropSidewaysStart(G3D::Vector3(lx, ly, owner->GetPositionZ()),
            G3D::Vector3(i_target->GetPositionX(), i_target->GetPositionY(), i_target->GetPositionZ()));
    }

    if (i_path->GetPath().size() < 2)
    {
        // Already on top of the target: wait for the next 400ms tick instead of
        // retrying every AI update (that is the inbound close-in stutter).
        if (!steeringChase)
            i_recalculateTravel = true;
        if (owner->GetTypeId() == TypeID::TYPEID_UNIT)
            ReportLiveChaseDebug(owner->ToCreature(), i_target.getTarget(), i_path, "short-path", false, true);
        return;
    }

    if (steeringChase && updateDestination && !owner->movespline->Finalized())
    {
        G3D::Vector3 const& newDest = i_path->GetPath().back();
        G3D::Vector3 const curDest = owner->movespline->FinalDestination();
        float const dx = newDest.x - curDest.x;
        float const dy = newDest.y - curDest.y;
        if (dx * dx + dy * dy < ChaseUnchangedDestDist * ChaseUnchangedDestDist)
        {
            if (owner->GetTypeId() == TypeID::TYPEID_UNIT)
                ReportLiveChaseDebug(owner->ToCreature(), i_target.getTarget(), i_path, "dest-unchanged", true, false);
            return;
        }
    }

    D::_addUnitStateMove(owner);
    i_targetReached = false;
    i_recalculateTravel = false;
    owner->AddUnitState(UNIT_STATE_CHASE);

    Movement::MoveSplineInit init(owner);
    init.MovebyPath(i_path->GetPath());
    init.SetWalk(((D*)this)->EnableWalking());

    // Retail chase never sets SmoothGroundPath. The client extra-smooths vertices;
    // relaunching a smoothed spline every few hundred ms is the land "jiggle".
    if (steeringChase || (i_path->GetPathType() & PATHFIND_NOT_USING_PATH))
        init.DisableSmoothGroundPath();

    if (i_path->GetPathType() & PATHFIND_NOT_USING_PATH)
    {
        if (owner->GetTypeId() == TypeID::TYPEID_UNIT)
        {
            Creature* creature = owner->ToCreature();
            if (creature->CanSwim())
            {
                init.SetCanSwim(true);
                if (creature->IsInWater() || creature->IsUnderWater())
                {
                    creature->SetSwim(true);
                    creature->SetAnimTier(ANIM_TIER_SWIM);
                    // Swim base speed is ~4.7; land run is ~7-8. Chase in water
                    // at run speed so they do not crawl.
                    init.SetVelocity(std::max(owner->GetSpeed(MOVE_RUN), owner->GetSpeed(MOVE_SWIM)));
                }
            }
        }
    }

    // 152755 log: chase chord speed is ~8 y/s while the priest is at ~7. Cap
    // so an unsnared player can break off; snares still let them catch up.
    if (steeringChase && i_target->GetTypeId() == TypeID::TYPEID_PLAYER)
    {
        float const playerRun = i_target->GetSpeed(MOVE_RUN);
        if (playerRun >= 6.0f)
        {
            float const cap = playerRun * 0.93f;
            float vel = owner->GetSpeed(MOVE_RUN);
            if (owner->IsInWater() || owner->IsUnderWater())
                vel = std::max(vel, owner->GetSpeed(MOVE_SWIM));
            if (vel > cap)
                init.SetVelocity(cap);
        }
    }
    // Never SetFacing/Final_Target during chase or follow. The client treats that
    // as a spin-to-face at spline end, which reads as 360 snaps while kiting.
    init.Launch();
    if (updateDestination)
    {
        i_lastAimX = i_target->GetPositionX();
        i_lastAimY = i_target->GetPositionY();
        i_lastAimZ = i_target->GetPositionZ();
        i_hasLastAim = true;
    }
    if (owner->GetTypeId() == TypeID::TYPEID_UNIT)
        ReportLiveChaseDebug(owner->ToCreature(), i_target.getTarget(), i_path, updateDestination ? "target-update" : "refresh", updateDestination, false);
}

template<class T, typename D>
bool TargetedMovementGeneratorMedium<T, D>::DoUpdate(T* owner, uint32 time_diff)
{
    if (!i_target.isValid() || !i_target->IsInWorld())
        return false;

    if (!owner || !owner->IsAlive())
        return false;

    if (owner->HasUnitState(UNIT_STATE_NOT_MOVE))
    {
        D::_clearUnitStateMove(owner);
        return true;
    }

    // prevent movement while casting spells with cast time or channel time
    if (owner->HasUnitState(UNIT_STATE_CASTING))
    {
        if (!owner->IsStopped())
            owner->StopMoving();
        return true;
    }

    // prevent crash after creature killed pet
    if (static_cast<D*>(this)->_lostTarget(owner))
    {
        D::_clearUnitStateMove(owner);
        return true;
    }

    bool targetMoved = false;
    bool relaunched = false;
    bool const steeringChase = static_cast<D*>(this)->UseSteering();

    // Follow the live player, but not every AI tick. Retail land chase (36147/36304)
    // relaunches about every 400ms. 100ms + SmoothGroundPath was the kiting jiggle.
    if (steeringChase)
    {
        i_recheckDistance.Update(time_diff);
        Unit* target = i_target.getTarget();
        bool const inMelee = owner->IsWithinMeleeRange(target);
        bool const targetMoving = TargetIsActivelyMoving(target, i_lastAimX, i_lastAimY, i_hasLastAim);
        if (!inMelee)
        {
            bool const due = i_recheckDistance.Passed();
            if (due)
                i_recheckDistance.Reset(ChaseFarRepathIntervalMs);
            bool const urgent = !owner->movespline->Finalized()
                && ChaseSplineDestOffPlayer(owner, target) > ChaseUrgentDestOff;
            if (urgent || (due && ChaseShouldRepath(owner, target, i_lastAimX, i_lastAimY, i_lastAimZ, i_hasLastAim)))
                targetMoved = true;
        }
        else if (i_recheckDistance.Passed())
        {
            i_recheckDistance.Reset(ChaseRepathIntervalMs);
            if (targetMoving)
                targetMoved = true;
        }
    }
    else
    {
        i_recheckDistance.Update(time_diff);
        if (i_recheckDistance.Passed())
        {
            i_recheckDistance.Reset(100);
            float allowed_dist = owner->GetCombatReach() + sWorld->getRate(Rates::RATE_TARGET_POS_RECALCULATION_RANGE);
            G3D::Vector3 dest = owner->movespline->FinalDestination();

            bool const flyOrSwimShortcut = owner->GetTypeId() == TypeID::TYPEID_UNIT
                && (owner->ToCreature()->CanFly()
                    || (i_path && (i_path->GetPathType() & PATHFIND_NOT_USING_PATH)));

            if (owner->IsWithinMeleeRange(i_target.getTarget()))
                targetMoved = false;
            else if (flyOrSwimShortcut)
            {
                if (!owner->movespline->Finalized())
                    targetMoved = !i_target->IsWithinDist3d(dest.x, dest.y, dest.z, allowed_dist + NOMINAL_MELEE_RANGE * 2.0f);
                else
                    targetMoved = !i_target->IsWithinDist3d(dest.x, dest.y, dest.z, allowed_dist);
            }
            else
            {
                if (!owner->movespline->Finalized())
                    targetMoved = !i_target->IsWithinDist2d(dest.x, dest.y, allowed_dist + NOMINAL_MELEE_RANGE);
                else
                    targetMoved = !i_target->IsWithinDist2d(dest.x, dest.y, allowed_dist);
            }
        }
    }

    if (i_recalculateTravel || targetMoved)
    {
        _setTargetLocation(owner, targetMoved);
        relaunched = true;
    }

    Unit* target = i_target.getTarget();
    bool const inMelee = owner->IsWithinMeleeRange(target);

    // Keep auto-attack armed the whole time we are in contact. Following a
    // moving player never "arrives", so the old finalize-only Attack() left
    // them jogging in melee with no swings.
    if (steeringChase)
    {
        if (inMelee)
            static_cast<D*>(this)->_reachTarget(owner);
        else
            i_targetReached = false;
    }

    if (owner->movespline->Finalized())
    {
        bool const targetMoving = TargetIsActivelyMoving(target, i_lastAimX, i_lastAimY, i_hasLastAim);

        if (!relaunched && steeringChase && !inMelee)
        {
            _setTargetLocation(owner, true);
            relaunched = true;
        }

        if (!steeringChase || (inMelee && !targetMoving))
            static_cast<D*>(this)->MovementInform(owner);

        if (i_angle == 0.f && owner->IsStopped() && inMelee && !targetMoving
            && !owner->HasInArc(0.01f, target))
            owner->SetInFront(target);

        if (!i_targetReached)
        {
            i_targetReached = true;
            static_cast<D*>(this)->_reachTarget(owner);
        }
    }

    return true;
}

//-----------------------------------------------//
template<class T>
void ChaseMovementGenerator<T>::_reachTarget(T* owner)
{
    if (owner->IsWithinMeleeRange(this->i_target.getTarget()))
        owner->Attack(this->i_target.getTarget(), true);
}

template<>
void ChaseMovementGenerator<Player>::DoInitialize(Player* owner)
{
    owner->AddUnitState(UNIT_STATE_CHASE | UNIT_STATE_CHASE_MOVE);
    _setTargetLocation(owner, true);
}

template<>
void ChaseMovementGenerator<Creature>::DoInitialize(Creature* owner)
{
    owner->SetWalk(false);
    owner->AddUnitState(UNIT_STATE_CHASE | UNIT_STATE_CHASE_MOVE);
    this->i_steeringLastChainedSplineId = 0;
    _setTargetLocation(owner, true);
}

template<>
bool ChaseMovementGenerator<Creature>::UseSteering() const
{
    return this->i_offset == 0.0f && this->i_angle == 0.0f;
}

template<class T>
void ChaseMovementGenerator<T>::DoFinalize(T* owner)
{
    owner->ClearUnitState(UNIT_STATE_CHASE | UNIT_STATE_CHASE_MOVE);
}

template<class T>
void ChaseMovementGenerator<T>::DoReset(T* owner)
{
    DoInitialize(owner);
}

template<class T>
void ChaseMovementGenerator<T>::MovementInform(T* /*unit*/) { }

template<>
void ChaseMovementGenerator<Creature>::MovementInform(Creature* unit)
{
    // Pass back the GUIDLow of the target. If it is pet's owner then PetAI will handle
    if (unit->AI())
        unit->AI()->MovementInform(CHASE_MOTION_TYPE, i_target.getTarget()->GetGUIDLow());
}

//-----------------------------------------------//
template<>
bool FollowMovementGenerator<Creature>::EnableWalking() const
{
    return i_target.isValid() && i_target->IsWalking();
}

template<>
bool FollowMovementGenerator<Player>::EnableWalking() const
{
    return false;
}

template<>
void FollowMovementGenerator<Player>::_updateSpeed(Player* /*owner*/)
{
    // nothing to do for Player
}

template<>
void FollowMovementGenerator<Creature>::_updateSpeed(Creature* owner)
{
    // pet only sync speed with owner
    /// Make sure we are not in the process of a map change (IsInWorld)
    if (!owner->IsPet() || !owner->IsInWorld() || !i_target.isValid() || i_target->GetGUID() != owner->GetOwnerGUID())
        return;

    owner->UpdateSpeed(MOVE_RUN, true);
    owner->UpdateSpeed(MOVE_WALK, true);
    owner->UpdateSpeed(MOVE_SWIM, true);
}

template<>
void FollowMovementGenerator<Player>::DoInitialize(Player* owner)
{
    owner->AddUnitState(UNIT_STATE_FOLLOW | UNIT_STATE_FOLLOW_MOVE);
    _updateSpeed(owner);
    _setTargetLocation(owner, true);
}

template<>
void FollowMovementGenerator<Creature>::DoInitialize(Creature* owner)
{
    owner->AddUnitState(UNIT_STATE_FOLLOW | UNIT_STATE_FOLLOW_MOVE);
    _updateSpeed(owner);
    _setTargetLocation(owner, true);
}

template<class T>
void FollowMovementGenerator<T>::DoFinalize(T* owner)
{
    owner->ClearUnitState(UNIT_STATE_FOLLOW | UNIT_STATE_FOLLOW_MOVE);
    _updateSpeed(owner);
}

template<class T>
void FollowMovementGenerator<T>::DoReset(T* owner)
{
    DoInitialize(owner);
}

template<class T>
void FollowMovementGenerator<T>::MovementInform(T* /*unit*/) { }

template<>
void FollowMovementGenerator<Creature>::MovementInform(Creature* unit)
{
    // Pass back the GUIDLow of the target. If it is pet's owner then PetAI will handle
    if (unit->AI())
        unit->AI()->MovementInform(FOLLOW_MOTION_TYPE, i_target.getTarget()->GetGUIDLow());
}

//-----------------------------------------------//
template void TargetedMovementGeneratorMedium<Player, ChaseMovementGenerator<Player> >::_setTargetLocation(Player*, bool);
template void TargetedMovementGeneratorMedium<Player, FollowMovementGenerator<Player> >::_setTargetLocation(Player*, bool);
template void TargetedMovementGeneratorMedium<Creature, ChaseMovementGenerator<Creature> >::_setTargetLocation(Creature*, bool);
template void TargetedMovementGeneratorMedium<Creature, FollowMovementGenerator<Creature> >::_setTargetLocation(Creature*, bool);
template bool TargetedMovementGeneratorMedium<Player, ChaseMovementGenerator<Player> >::DoUpdate(Player*, uint32);
template bool TargetedMovementGeneratorMedium<Player, FollowMovementGenerator<Player> >::DoUpdate(Player*, uint32);
template bool TargetedMovementGeneratorMedium<Creature, ChaseMovementGenerator<Creature> >::DoUpdate(Creature*, uint32);
template bool TargetedMovementGeneratorMedium<Creature, FollowMovementGenerator<Creature> >::DoUpdate(Creature*, uint32);

template void ChaseMovementGenerator<Player>::_reachTarget(Player*);
template void ChaseMovementGenerator<Creature>::_reachTarget(Creature*);
template void ChaseMovementGenerator<Player>::DoFinalize(Player*);
template void ChaseMovementGenerator<Creature>::DoFinalize(Creature*);
template void ChaseMovementGenerator<Player>::DoReset(Player*);
template void ChaseMovementGenerator<Creature>::DoReset(Creature*);
template void ChaseMovementGenerator<Player>::MovementInform(Player*);

template void FollowMovementGenerator<Player>::DoFinalize(Player*);
template void FollowMovementGenerator<Creature>::DoFinalize(Creature*);
template void FollowMovementGenerator<Player>::DoReset(Player*);
template void FollowMovementGenerator<Creature>::DoReset(Creature*);
template void FollowMovementGenerator<Player>::MovementInform(Player*);
