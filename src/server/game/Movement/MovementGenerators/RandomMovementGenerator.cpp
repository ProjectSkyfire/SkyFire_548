/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Creature.h"
#include "CreatureGroups.h"
#include "Map.h"
#include "MapManager.h"
#include "MoveSpline.h"
#include "MoveSplineInit.h"
#include "ObjectAccessor.h"
#include "RandomMovementGenerator.h"
#include "Util.h"

#define RUNNING_CHANCE_RANDOMMV 20                                  //will be "1 / RUNNING_CHANCE_RANDOMMV"

#ifdef MAP_BASED_RAND_GEN
#define rand_norm() creature.rand_norm()
#endif

template<>
void RandomMovementGenerator<Creature>::_setRandomLocation(Creature* creature)
{
    float respX, respY, respZ, respO, destX, destY, destZ, travelDistZ;
    creature->GetHomePosition(respX, respY, respZ, respO);
    Map const* map = creature->GetBaseMap();

    // For 2D/3D system selection
    //bool is_land_ok  = creature.CanWalk();                // not used?
    //bool is_water_ok = creature.CanSwim();                // not used?
    bool is_air_ok = creature->CanFly();

    const float angle = float(rand_norm()) * static_cast<float>(M_PI * 2.0f);
    // Avoid near-zero steps: tiny XY deltas make the Z tolerance fail every retry.
    float range = float(rand_norm()) * wander_distance;
    if (wander_distance > 0.0f && range < wander_distance * 0.3f)
        range = wander_distance * 0.3f;

    const float distanceX = range * std::cos(angle);
    const float distanceY = range * std::sin(angle);

    destX = respX + distanceX;
    destY = respY + distanceY;

    // prevent invalid coordinates generation
    Skyfire::NormalizeMapCoord(destX);
    Skyfire::NormalizeMapCoord(destY);

    travelDistZ = distanceX * distanceX + distanceY * distanceY;

    if (is_air_ok)                                          // 3D system above ground and above water (flying mode)
    {
        // Limit height change
        const float distanceZ = float(rand_norm()) * sqrtf(travelDistZ) / 2.0f;
        destZ = respZ + distanceZ;
        float levelZ = map->GetWaterOrGroundLevel(destX, destY, destZ - 2.0f);

        // Problem here, we must fly above the ground and water, not under. Let's try on next tick
        if (levelZ >= destZ)
        {
            i_nextMoveTime.Reset(200);
            return;
        }
    }
    //else if (is_water_ok)                                 // 3D system under water and above ground (swimming mode)
    else                                                    // 2D only
    {
        float const maxDeltaZ = std::max(wander_distance, 5.0f) + 6.0f;
        float const adtZ = map->GetRawTerrainHeight(destX, destY);
        // Probe from the home layer. Looking from respZ+maxDeltaZ (16-21 yards)
        // makes GetHeight prefer tree/canopy vmaps over the ground.
        destZ = map->GetHeight(creature->GetPhaseMask(), destX, destY, respZ + 2.0f, true);

        // Uphill ADT is skipped when the query sits below it (z+2 < gridHeight).
        if (adtZ > INVALID_HEIGHT && (destZ <= INVALID_HEIGHT || (adtZ > destZ + 1.0f && fabs(adtZ - respZ) <= maxDeltaZ)))
            destZ = adtZ;

        // Keep low walkable doodads (roots ~3 yards) but not trunks/canopy.
        if (adtZ > INVALID_HEIGHT && destZ > adtZ + 3.5f && destZ > respZ + 3.5f)
            destZ = adtZ;

        if (destZ <= INVALID_HEIGHT || fabs(destZ - respZ) > maxDeltaZ)
        {
            i_nextMoveTime.Reset(200);
            return;
        }
    }

    if (is_air_ok)
        i_nextMoveTime.Reset(0);
    else
        i_nextMoveTime.Reset(std::rand() % 10000 + 500);

    creature->AddUnitState(UNIT_STATE_ROAMING_MOVE);

    Movement::MoveSplineInit init(creature);
    init.MoveTo(destX, destY, destZ, true);
    init.SetWalk(true);
    init.Launch();

    //Call for creature group update
    if (creature->GetFormation() && creature->GetFormation()->getLeader() == creature)
        creature->GetFormation()->LeaderMoveTo(destX, destY, destZ);
}

template<>
void RandomMovementGenerator<Creature>::DoInitialize(Creature* creature)
{
    if (!creature->IsAlive())
        return;

    if (!wander_distance)
        wander_distance = creature->GetRespawnRadius();

    creature->AddUnitState(UNIT_STATE_ROAMING | UNIT_STATE_ROAMING_MOVE);
    _setRandomLocation(creature);
}

template<>
void RandomMovementGenerator<Creature>::DoReset(Creature* creature)
{
    DoInitialize(creature);
}

template<>
void RandomMovementGenerator<Creature>::DoFinalize(Creature* creature)
{
    creature->ClearUnitState(UNIT_STATE_ROAMING | UNIT_STATE_ROAMING_MOVE);
    creature->SetWalk(false);
}

template<>
bool RandomMovementGenerator<Creature>::DoUpdate(Creature* creature, const uint32 diff)
{
    if (creature->IsInCombat())
        return true;

    if (creature->HasUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED | UNIT_STATE_DISTRACTED))
    {
        i_nextMoveTime.Reset(0);  // Expire the timer
        creature->ClearUnitState(UNIT_STATE_ROAMING_MOVE);
        return true;
    }

    if (creature->movespline->Finalized())
    {
        i_nextMoveTime.Update(diff);
        if (i_nextMoveTime.Passed())
            _setRandomLocation(creature);
    }
    return true;
}

template<>
bool RandomMovementGenerator<Creature>::GetResetPos(Creature* creature, float& x, float& y, float& z)
{
    float radius;
    creature->GetRespawnPosition(x, y, z, NULL, &radius);

    // use current if in range
    if (creature->IsWithinDist2d(x, y, radius))
        creature->GetPosition(x, y, z);

    return true;
}
