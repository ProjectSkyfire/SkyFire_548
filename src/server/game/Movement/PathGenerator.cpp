/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Creature.h"
#include "Log.h"
#include "Map.h"
#include "MMapFactory.h"
#include "MMapManager.h"
#include "MoveSpline.h"
#include "PathGenerator.h"

#include "DetourCommon.h"
#include "DetourNavMeshQuery.h"

#include <algorithm>
#include <cmath>

////////////////// PathGenerator //////////////////
PathGenerator::PathGenerator(const Unit* owner) :
    _polyLength(0), _type(PATHFIND_BLANK), _useStraightPath(false),
    _useChaseSteering(false), _steeringActive(false), _forceDestination(false), _pointPathLimit(MAX_POINT_PATH_LENGTH),
    _rawPathPointCount(0), _endPosition(G3D::Vector3::zero()), _steeringTarget(G3D::Vector3::zero()), _sourceUnit(owner), _navMesh(NULL),
    _navMeshQuery(NULL)
{
    memset(_pathPolyRefs, 0, sizeof(_pathPolyRefs));
    SF_LOG_DEBUG("maps", "++ PathGenerator::PathGenerator for %u \n", _sourceUnit->GetGUIDLow());

    uint32 mapId = _sourceUnit->GetMapId();
    if (MMAP::MMapFactory::IsPathfindingEnabled(mapId))
    {
        MMAP::MMapManager* mmap = MMAP::MMapFactory::createOrGetMMapManager();
        _navMesh = mmap->GetNavMesh(mapId, _sourceUnit->GetTerrainSwaps());
        _navMeshQuery = mmap->GetNavMeshQuery(mapId, _sourceUnit->GetInstanceId(), _sourceUnit->GetTerrainSwaps());
    }

    CreateFilter();
}

PathGenerator::~PathGenerator()
{
    SF_LOG_DEBUG("maps", "++ PathGenerator::~PathGenerator() for %u \n", _sourceUnit->GetGUIDLow());
}

bool PathGenerator::CalculatePath(float destX, float destY, float destZ, bool forceDest)
{
    float x, y, z;
    // Mid-spline recalcs must start from the live interpolated position.
    // GetPosition() only updates every 400ms, so a new path can begin behind
    // the creature and the client snaps backward, then forward again.
    if (!_sourceUnit->movespline->Finalized())
    {
        Movement::Location loc = _sourceUnit->movespline->ComputePosition();
        x = loc.x;
        y = loc.y;
        z = loc.z;
    }
    else
        _sourceUnit->GetPosition(x, y, z);

    if (!Skyfire::IsValidMapCoord(destX, destY, destZ) || !Skyfire::IsValidMapCoord(x, y, z))
        return false;

    G3D::Vector3 dest(destX, destY, destZ);
    SetEndPosition(dest);

    G3D::Vector3 start(x, y, z);
    SetStartPosition(start);

    _forceDestination = forceDest;

    SF_LOG_DEBUG("maps", "++ PathGenerator::CalculatePath() for %u \n", _sourceUnit->GetGUIDLow());

    // make sure navMesh works - we can run on map w/o mmap
    // check if the start and end point have a .mmtile loaded (can we pass via not loaded tile on the way?)
    if (!_navMesh || !_navMeshQuery || _sourceUnit->HasUnitState(UNIT_STATE_IGNORE_PATHFINDING) ||
        !HaveTile(start) || !HaveTile(dest))
    {
        BuildShortcut();
        _type = PathType(PATHFIND_NORMAL | PATHFIND_NOT_USING_PATH);
        ClampPathToHabitat();
        FinalizePathElevation();
        if (_pathPoints.size() < 2)
            _type = PathType(_type | PATHFIND_NOPATH);
        return true;
    }

    UpdateFilter();

    BuildPolyPath(start, dest);
    ClampPathToHabitat();
    FinalizePathElevation();
    if (_pathPoints.size() < 2)
        _type = PathType(_type | PATHFIND_NOPATH);
    return true;
}

dtPolyRef PathGenerator::GetPathPolyByPosition(dtPolyRef const* polyPath, uint32 polyPathSize, float const* point, float* distance) const
{
    if (!polyPath || !polyPathSize)
        return INVALID_POLYREF;

    dtPolyRef nearestPoly = INVALID_POLYREF;
    float minDist2d = FLT_MAX;
    float minDist3d = 0.0f;

    for (uint32 i = 0; i < polyPathSize; ++i)
    {
        float closestPoint[VERTEX_SIZE];
        if (dtStatusFailed(_navMeshQuery->closestPointOnPoly(polyPath[i], point, closestPoint)))
            continue;

        float d = dtVdist2DSqr(point, closestPoint);
        if (d < minDist2d)
        {
            minDist2d = d;
            nearestPoly = polyPath[i];
            minDist3d = dtVdistSqr(point, closestPoint);
        }

        if (minDist2d < 1.0f) // shortcut out - close enough for us
            break;
    }

    if (distance)
        *distance = dtSqrt(minDist3d);

    return (minDist2d < 3.0f) ? nearestPoly : INVALID_POLYREF;
}

dtPolyRef PathGenerator::GetPolyByLocation(float const* point, float* distance) const
{
    // first we check the current path
    // if the current path doesn't contain the current poly,
    // we need to use the expensive navMesh.findNearestPoly
    dtPolyRef polyRef = GetPathPolyByPosition(_pathPolyRefs, _polyLength, point, distance);
    if (polyRef != INVALID_POLYREF)
        return polyRef;

    // we don't have it in our old path
    // try to get it by findNearestPoly()
    // first try with low search box
    float extents[VERTEX_SIZE] = { 3.0f, 5.0f, 3.0f };    // bounds of poly search area
    float closestPoint[VERTEX_SIZE] = { 0.0f, 0.0f, 0.0f };
    if (dtStatusSucceed(_navMeshQuery->findNearestPoly(point, extents, &_filter, &polyRef, closestPoint)) && polyRef != INVALID_POLYREF)
    {
        *distance = dtVdist(closestPoint, point);
        return polyRef;
    }

    // still nothing ..
    // try with bigger search box
    // Note that the extent should not overlap more than 128 polygons in the navmesh (see dtNavMeshQuery::findNearestPoly)
    extents[1] = 50.0f;

    if (dtStatusSucceed(_navMeshQuery->findNearestPoly(point, extents, &_filter, &polyRef, closestPoint)) && polyRef != INVALID_POLYREF)
    {
        *distance = dtVdist(closestPoint, point);
        return polyRef;
    }

    // Dest in the other habitat (player in the lake, walker on the bank) sits
    // more than 3y off the allowed mesh. Find the shoreline poly instead of
    // NOPATH, which made walk-only mobs stand and then evade.
    extents[0] = 25.0f;
    extents[2] = 25.0f;
    if (dtStatusSucceed(_navMeshQuery->findNearestPoly(point, extents, &_filter, &polyRef, closestPoint)) && polyRef != INVALID_POLYREF)
    {
        *distance = dtVdist(closestPoint, point);
        return polyRef;
    }

    return INVALID_POLYREF;
}

void PathGenerator::BuildPolyPath(G3D::Vector3 const& startPos, G3D::Vector3 const& endPos)
{
    // Swim shortcuts are only valid when BOTH ends are in water. A naga on a
    // bridge with the player in the drink below must use the mesh (ramp/shore),
    // not fall through the air.
    if (CanSwimAt(startPos.x, startPos.y, startPos.z) && CanSwimAt(endPos.x, endPos.y, endPos.z))
    {
        BuildShortcut();
        _type = PathType(PATHFIND_NORMAL | PATHFIND_NOT_USING_PATH);
        return;
    }

    // *** getting start/end poly logic ***

    float distToStartPoly, distToEndPoly;
    float startPoint[VERTEX_SIZE] = { startPos.y, startPos.z, startPos.x };
    float endPoint[VERTEX_SIZE] = { endPos.y, endPos.z, endPos.x };

    dtPolyRef startPoly = GetPolyByLocation(startPoint, &distToStartPoly);
    dtPolyRef endPoly = GetPolyByLocation(endPoint, &distToEndPoly);

    // we have a hole in our mesh
    // make shortcut path and mark it as NOPATH ( with flying and swimming exception )
    // its up to caller how he will use this info
    if (startPoly == INVALID_POLYREF || endPoly == INVALID_POLYREF)
    {
        SF_LOG_DEBUG("maps", "++ BuildPolyPath :: (startPoly == 0 || endPoly == 0)\n");
        BuildShortcut();
        bool path = _sourceUnit->GetTypeId() == TypeID::TYPEID_UNIT && _sourceUnit->ToCreature()->CanFly();

        bool waterPath = _sourceUnit->GetTypeId() == TypeID::TYPEID_UNIT && _sourceUnit->ToCreature()->CanSwim();
        if (waterPath)
        {
            // Check both start and end points, if they're both in water, then we can *safely* let the creature move
            for (uint32 i = 0; i < _pathPoints.size(); ++i)
            {
                ZLiquidStatus status = _sourceUnit->GetBaseMap()->getLiquidStatus(_pathPoints[i].x, _pathPoints[i].y, _pathPoints[i].z, MAP_ALL_LIQUIDS, NULL);
                // One of the points is not in the water, cancel movement.
                if (status == LIQUID_MAP_NO_WATER)
                {
                    waterPath = false;
                    break;
                }
            }
        }

        _type = (path || waterPath) ? PathType(PATHFIND_NORMAL | PATHFIND_NOT_USING_PATH) : PATHFIND_NOPATH;
        return;
    }

    // we may need a better number here
    bool farFromPoly = (distToStartPoly > 7.0f || distToEndPoly > 7.0f);
    if (farFromPoly)
    {
        SF_LOG_DEBUG("maps", "++ BuildPolyPath :: farFromPoly distToStartPoly=%.3f distToEndPoly=%.3f\n", distToStartPoly, distToEndPoly);

        bool buildShotrcut = false;
        if (_sourceUnit->GetTypeId() == TypeID::TYPEID_UNIT)
        {
            Creature* owner = (Creature*)_sourceUnit;
            // Swim-only creatures far from a poly used to shortcut even when the
            // dest was on land (Lake Frenzy walking onto the Everstill shore).
            // Both-in-water already shortcuts above; only flyers may skip the mesh here.
            if (owner->CanFly())
            {
                SF_LOG_DEBUG("maps", "++ BuildPolyPath :: flying case\n");
                buildShotrcut = true;
            }
        }

        if (buildShotrcut)
        {
            BuildShortcut();
            _type = PathType(PATHFIND_NORMAL | PATHFIND_NOT_USING_PATH);
            return;
        }
        else
        {
            float closestPoint[VERTEX_SIZE];
            // we may want to use closestPointOnPolyBoundary instead
            if (dtStatusSucceed(_navMeshQuery->closestPointOnPoly(endPoly, endPoint, closestPoint)))
            {
                dtVcopy(endPoint, closestPoint);
                SetActualEndPosition(G3D::Vector3(endPoint[2], endPoint[0], endPoint[1]));
            }

            _type = PATHFIND_INCOMPLETE;
        }
    }

    // *** poly path generating logic ***
    // start and end are on same polygon
    // just need to move in straight line
    if (startPoly == endPoly)
    {
        SF_LOG_DEBUG("maps", "++ BuildPolyPath :: (startPoly == endPoly)\n");

        if (!CanBuildDirectShortcut(startPos, endPos))
        {
            _pathPolyRefs[0] = startPoly;
            _polyLength = 1;
            _type = PATHFIND_INCOMPLETE;
            BuildPointPath(startPoint, endPoint);
            // A 1-point result is not a spline. Callers that ignore NOPATH
            // (random wander MoveTo) would Launch() and spam Validate.
            if (_pathPoints.size() < 2 || std::fabs(_pathPoints.back().z - startPos.z) > 2.5f)
            {
                _pathPoints.clear();
                SetActualEndPosition(startPos);
                _type = PATHFIND_NOPATH;
            }
            return;
        }

        BuildShortcut();

        _pathPolyRefs[0] = startPoly;
        _polyLength = 1;

        _type = farFromPoly ? PATHFIND_INCOMPLETE : PATHFIND_NORMAL;
        SF_LOG_DEBUG("maps", "++ BuildPolyPath :: path type %d\n", _type);
        return;
    }

    // look for startPoly/endPoly in current path
    /// @todo we can merge it with getPathPolyByPosition() loop
    bool startPolyFound = false;
    bool endPolyFound = false;
    uint32 pathStartIndex = 0;
    uint32 pathEndIndex = 0;

    if (_polyLength)
    {
        for (; pathStartIndex < _polyLength; ++pathStartIndex)
        {
            // here to carch few bugs
            ASSERT(_pathPolyRefs[pathStartIndex] != INVALID_POLYREF);

            if (_pathPolyRefs[pathStartIndex] == startPoly)
            {
                startPolyFound = true;
                break;
            }
        }

        for (pathEndIndex = _polyLength - 1; pathEndIndex > pathStartIndex; --pathEndIndex)
            if (_pathPolyRefs[pathEndIndex] == endPoly)
            {
                endPolyFound = true;
                break;
            }
    }

    if (startPolyFound && endPolyFound)
    {
        SF_LOG_DEBUG("maps", "++ BuildPolyPath :: (startPolyFound && endPolyFound)\n");

        // we moved along the path and the target did not move out of our old poly-path
        // our path is a simple subpath case, we have all the data we need
        // just "cut" it out

        _polyLength = pathEndIndex - pathStartIndex + 1;
        memmove(_pathPolyRefs, _pathPolyRefs + pathStartIndex, _polyLength * sizeof(dtPolyRef));
    }
    else if (startPolyFound && !endPolyFound)
    {
        SF_LOG_DEBUG("maps", "++ BuildPolyPath :: (startPolyFound && !endPolyFound)\n");

        // we are moving on the old path but target moved out
        // so we have atleast part of poly-path ready

        _polyLength -= pathStartIndex;

        // try to adjust the suffix of the path instead of recalculating entire length
        // at given interval the target cannot get too far from its last location
        // thus we have less poly to cover
        // sub-path of optimal path is optimal

        // take ~80% of the original length
        /// @todo play with the values here
        uint32 prefixPolyLength = uint32(_polyLength * 0.8f + 0.5f);
        memmove(_pathPolyRefs, _pathPolyRefs + pathStartIndex, prefixPolyLength * sizeof(dtPolyRef));

        dtPolyRef suffixStartPoly = _pathPolyRefs[prefixPolyLength - 1];

        // we need any point on our suffix start poly to generate poly-path, so we need last poly in prefix data
        float suffixEndPoint[VERTEX_SIZE];
        if (dtStatusFailed(_navMeshQuery->closestPointOnPoly(suffixStartPoly, endPoint, suffixEndPoint)))
        {
            // we can hit offmesh connection as last poly - closestPointOnPoly() don't like that
            // try to recover by using prev polyref
            --prefixPolyLength;
            suffixStartPoly = _pathPolyRefs[prefixPolyLength - 1];
            if (dtStatusFailed(_navMeshQuery->closestPointOnPoly(suffixStartPoly, endPoint, suffixEndPoint)))
            {
                // suffixStartPoly is still invalid, error state
                BuildShortcut();
                _type = PATHFIND_NOPATH;
                return;
            }
        }

        // generate suffix
        uint32 suffixPolyLength = 0;
        dtStatus dtResult = _navMeshQuery->findPath(
            suffixStartPoly,    // start polygon
            endPoly,            // end polygon
            suffixEndPoint,     // start position
            endPoint,           // end position
            &_filter,            // polygon search filter
            _pathPolyRefs + prefixPolyLength - 1,    // [out] path
            (int*)&suffixPolyLength,
            MAX_PATH_LENGTH - prefixPolyLength);   // max number of polygons in output path

        if (!suffixPolyLength || dtStatusFailed(dtResult))
        {
            // this is probably an error state, but we'll leave it
            // and hopefully recover on the next Update
            // we still need to copy our preffix
            SF_LOG_ERROR("maps", "%u's Path Build failed: 0 length path", _sourceUnit->GetGUIDLow());
        }

        SF_LOG_DEBUG("maps", "++  m_polyLength=%u prefixPolyLength=%u suffixPolyLength=%u \n", _polyLength, prefixPolyLength, suffixPolyLength);

        // new path = prefix + suffix - overlap
        _polyLength = prefixPolyLength + suffixPolyLength - 1;
    }
    else
    {
        SF_LOG_DEBUG("maps", "++ BuildPolyPath :: (!startPolyFound && !endPolyFound)\n");

        // either we have no path at all -> first run
        // or something went really wrong -> we aren't moving along the path to the target
        // just generate new path

        // free and invalidate old path data
        Clear();

        dtStatus dtResult = _navMeshQuery->findPath(
            startPoly,          // start polygon
            endPoly,            // end polygon
            startPoint,         // start position
            endPoint,           // end position
            &_filter,           // polygon search filter
            _pathPolyRefs,     // [out] path
            (int*)&_polyLength,
            MAX_PATH_LENGTH);   // max number of polygons in output path

        if (!_polyLength || dtStatusFailed(dtResult))
        {
            // only happens if we passed bad data to findPath(), or navmesh is messed up
            SF_LOG_ERROR("maps", "%u's Path Build failed: 0 length path", _sourceUnit->GetGUIDLow());
            BuildShortcut();
            _type = PATHFIND_NOPATH;
            return;
        }
    }

    // by now we know what type of path we can get
    if (_pathPolyRefs[_polyLength - 1] == endPoly && !(_type & PATHFIND_INCOMPLETE))
        _type = PATHFIND_NORMAL;
    else
        _type = PATHFIND_INCOMPLETE;

    // generate the point-path out of our up-to-date poly-path
    BuildPointPath(startPoint, endPoint);
}

void PathGenerator::BuildPointPath(const float* startPoint, const float* endPoint)
{
    float pathPoints[MAX_POINT_PATH_LENGTH * VERTEX_SIZE];
    uint32 pointCount = 0;
    dtStatus dtResult = DT_FAILURE;
    if (_useStraightPath)
    {
        dtResult = _navMeshQuery->findStraightPath(
            startPoint,         // start position
            endPoint,           // end position
            _pathPolyRefs,     // current path
            _polyLength,       // lenth of current path
            pathPoints,         // [out] path corner points
            NULL,               // [out] flags
            NULL,               // [out] shortened path
            (int*)&pointCount,
            _pointPathLimit);   // maximum number of points/polygons to use
    }
    else
    {
        dtResult = FindSmoothPath(
            startPoint,         // start position
            endPoint,           // end position
            _pathPolyRefs,     // current path
            _polyLength,       // length of current path
            pathPoints,         // [out] path corner points
            (int*)&pointCount,
            _pointPathLimit);    // maximum number of points
    }

    if (pointCount < 2 || dtStatusFailed(dtResult))
    {
        // only happens if pass bad data to findStraightPath or navmesh is broken
        // single point paths can be generated here
        /// @todo check the exact cases
        SF_LOG_DEBUG("maps", "++ PathGenerator::BuildPointPath FAILED! path sized %d returned\n", pointCount);
        BuildShortcut();
        _type = PATHFIND_NOPATH;
        return;
    }
    else if (pointCount == _pointPathLimit)
    {
        // Keep the mesh prefix. Replacing it with a 2-point shortcut is how
        // NPCs "fell" off bridges instead of walking the long way around.
        SF_LOG_DEBUG("maps", "++ PathGenerator::BuildPointPath path hit point limit %d\n", pointCount);
        _rawPathPointCount = pointCount;
        _steeringActive = false;
        _pathPoints.resize(pointCount);
        for (uint32 i = 0; i < pointCount; ++i)
            _pathPoints[i] = G3D::Vector3(pathPoints[i * VERTEX_SIZE + 2], pathPoints[i * VERTEX_SIZE], pathPoints[i * VERTEX_SIZE + 1]);
        SetActualEndPosition(_pathPoints.back());
        _type = PATHFIND_SHORT;
        return;
    }

    _rawPathPointCount = pointCount;
    _steeringActive = false;
    _pathPoints.resize(pointCount);
    for (uint32 i = 0; i < pointCount; ++i)
        _pathPoints[i] = G3D::Vector3(pathPoints[i * VERTEX_SIZE + 2], pathPoints[i * VERTEX_SIZE], pathPoints[i * VERTEX_SIZE + 1]);

    // first point is always our current location - we need the next one
    SetActualEndPosition(_pathPoints[pointCount - 1]);

    // Keep the mesh path and only pull the last point to dest. Replacing the
    // whole path with a 2-point shortcut made pets/nagas fly and turtles stutter.
    if (_forceDestination &&
        (!(_type & PATHFIND_NORMAL) || !InRange(GetEndPosition(), GetActualEndPosition(), 1.0f, 1.0f)))
    {
        SetActualEndPosition(GetEndPosition());
        _pathPoints.back() = GetEndPosition();
        _type = PathType(PATHFIND_NORMAL | PATHFIND_NOT_USING_PATH);
    }

    SF_LOG_DEBUG("maps", "++ PathGenerator::BuildPointPath path type %d size %d poly-size %d\n", _type, pointCount, _polyLength);
}

void PathGenerator::ShortenPathUntilDist(G3D::Vector3 const& target, float dist)
{
    _steeringActive = false;
    _steeringTarget = _pathPoints.empty() ? G3D::Vector3::zero() : _pathPoints.back();

    if (_pathPoints.size() < 2 || dist <= 0.0f)
        return;

    float const distSq = dist * dist;
    if (Dist3DSqr(_pathPoints.front(), target) < distSq)
        return;
    if (Dist3DSqr(_pathPoints.back(), target) >= distSq)
    {
        _steeringTarget = _pathPoints.back();
        return;
    }

    // Last vertex still outside melee. Segment [outside, outside+1] crosses the stop radius.
    // Never resize to 1 point — Launch() requires path.size() > 1 (hippogryph melee
    // often swallows every Recast vertex after start).
    size_t outside = _pathPoints.size() - 1;
    while (outside > 0 && Dist3DSqr(_pathPoints[outside], target) < distSq)
        --outside;

    G3D::Vector3 const& a = _pathPoints[outside];
    G3D::Vector3 const& b = _pathPoints[outside + 1];
    G3D::Vector3 const ab = b - a;
    G3D::Vector3 const at = a - target;
    float const A = ab.squaredLength();
    float t = 1.0f;
    if (A > 0.0001f)
    {
        float const B = 2.0f * at.dot(ab);
        float const C = at.squaredLength() - distSq;
        float const disc = B * B - 4.0f * A * C;
        if (disc >= 0.0f)
        {
            float const sqrtDisc = std::sqrt(disc);
            float const inv = 0.5f / A;
            float const t1 = (-B - sqrtDisc) * inv;
            float const t2 = (-B + sqrtDisc) * inv;
            bool have = false;
            if (t1 >= 0.0f && t1 <= 1.0f)
            {
                t = t1;
                have = true;
            }
            if (t2 >= 0.0f && t2 <= 1.0f && (!have || t2 < t))
                t = t2;
        }
    }

    G3D::Vector3 const cut(a.x + ab.x * t, a.y + ab.y * t, a.z + ab.z * t);
    _pathPoints.resize(outside + 1);
    _pathPoints.push_back(cut);
    SetActualEndPosition(cut);
    _steeringTarget = cut;
    _steeringActive = true;
}

void PathGenerator::DropSidewaysStart(G3D::Vector3 const& from, G3D::Vector3 const& aim)
{
    if (_pathPoints.size() < 2)
        return;

    float const dx = aim.x - from.x;
    float const dy = aim.y - from.y;
    float const destLenSq = dx * dx + dy * dy;
    if (destLenSq < 1.0f)
        return;

    auto onAimLine = [&](G3D::Vector3 const& p) -> bool
    {
        float const sx = p.x - from.x;
        float const sy = p.y - from.y;
        float const segLenSq = sx * sx + sy * sy;
        if (segLenSq < 0.25f)
            return false;
        float const dot = sx * dx + sy * dy;
        return dot > 0.0f && dot * dot > 0.25f * segLenSq * destLenSq;
    };

    // Recast's first vertex is often a poly corner 90° off the chase line.
    // A 2-point path cannot drop a middle vertex; pull that dest onto the
    // owner→player line so the first launch is not a left/right snap.
    if (_pathPoints.size() == 2)
    {
        G3D::Vector3& p = _pathPoints[1];
        if (onAimLine(p))
            return;
        float const sx = p.x - from.x;
        float const sy = p.y - from.y;
        float const keep = std::sqrt(sx * sx + sy * sy);
        float const invAim = 1.0f / std::sqrt(destLenSq);
        p.x = from.x + dx * invAim * keep;
        p.y = from.y + dy * invAim * keep;
        SetActualEndPosition(p);
        return;
    }

    while (_pathPoints.size() > 2)
    {
        G3D::Vector3 const& p = _pathPoints[1];
        float const sx = p.x - from.x;
        float const sy = p.y - from.y;
        float const segLenSq = sx * sx + sy * sy;
        if (segLenSq < 0.25f)
        {
            _pathPoints.erase(_pathPoints.begin() + 1);
            continue;
        }

        if (onAimLine(p))
            break;

        _pathPoints.erase(_pathPoints.begin() + 1);
    }
}

float PathGenerator::ResolveTerrainZ(float x, float y, float interpolatedZ) const
{
    G3D::Vector3 const& start = GetStartPosition();
    G3D::Vector3 const& dest = GetEndPosition();
    Map const* map = _sourceUnit->GetBaseMap();
    uint32 const phase = _sourceUnit->GetPhaseMask();

    float const destFloor = map->GetHeight(phase, dest.x, dest.y, dest.z + 2.0f, true);

    // Dest is on collision GetHeight does not have. Follow the chord; only lift out of ADT.
    if (destFloor > INVALID_HEIGHT && dest.z > destFloor + 2.0f)
    {
        float const floor = map->GetHeight(phase, x, y, interpolatedZ + 2.0f, true);
        if (floor > INVALID_HEIGHT && interpolatedZ < floor)
            return floor;
        return interpolatedZ;
    }

    float const probeZ = std::max(start.z, dest.z) + 2.0f;
    float const maxWalk = std::max(start.z, dest.z) + 3.0f;
    float const minWalk = std::min(start.z, dest.z) - 12.0f;
    float const floor = map->GetHeight(phase, x, y, probeZ, true);
    if (floor > INVALID_HEIGHT && floor <= maxWalk && floor >= minWalk)
        return floor;

    float const adt = map->GetRawTerrainHeight(x, y);
    if (adt > INVALID_HEIGHT && adt <= maxWalk && adt >= minWalk)
        return adt;

    return interpolatedZ;
}

// Only for shortcut paths, which are straight lines drawn without consulting geometry.
// Recast paths already sit on the mesh, which includes WMO floors; re-deriving their Z
// from GetHeight would drop cave paths onto the terrain below the cave.
void PathGenerator::DensifyGroundPath()
{
    if (_pathPoints.size() < 2)
        return;

    if (_sourceUnit->GetTypeId() != TypeID::TYPEID_UNIT || _sourceUnit->CanFly())
        return;

    Creature const* creature = _sourceUnit->ToCreature();
    if (!creature->CanWalk())
        return;

    // Walk the beach on terrain, then swim at dest depth. A 2-point chord from
    // land to a swimmer stays above the water for most of the run, so SetSwim
    // never fires and they skip across the surface.
    if (creature->CanSwim() && (CanSwimAt(GetStartPosition().x, GetStartPosition().y, GetStartPosition().z)
        || CanSwimAt(GetEndPosition().x, GetEndPosition().y, GetEndPosition().z)
        || CanSwimAt(GetActualEndPosition().x, GetActualEndPosition().y, GetActualEndPosition().z)))
    {
        DensifySwimPath();
        return;
    }

    if (CreatureTemplate const* info = creature->GetCreatureTemplate())
        if ((info->InhabitType & INHABIT_AIR) && !(info->InhabitType & INHABIT_GROUND))
            return;

    Movement::PointsArray densified;
    densified.reserve(_pathPoints.size() * 4);

    for (size_t i = 0; i < _pathPoints.size(); ++i)
    {
        if (i == 0)
        {
            G3D::Vector3 const& p = _pathPoints[i];
            densified.emplace_back(p.x, p.y, ResolveTerrainZ(p.x, p.y, p.z));
            continue;
        }

        G3D::Vector3 const& prev = densified.back();
        G3D::Vector3 const& next = _pathPoints[i];
        float const dx = next.x - prev.x;
        float const dy = next.y - prev.y;
        float const dist2d = sqrt(dx * dx + dy * dy);

        float const stepSize = 1.5f;
        if (dist2d > stepSize)
        {
            uint32 const steps = uint32(dist2d / stepSize);
            for (uint32 s = 1; s < steps; ++s)
            {
                float const t = float(s) / float(steps);
                float x = prev.x + dx * t;
                float y = prev.y + dy * t;
                float z = prev.z + (next.z - prev.z) * t;
                densified.emplace_back(x, y, ResolveTerrainZ(x, y, z));
            }
        }

        densified.emplace_back(next.x, next.y, ResolveTerrainZ(next.x, next.y, next.z));
    }

    _pathPoints.swap(densified);
}

void PathGenerator::DensifySwimPath()
{
    if (_pathPoints.size() < 2)
        return;

    Map const* map = _sourceUnit->GetBaseMap();
    G3D::Vector3 const dest = GetEndPosition();
    Movement::PointsArray densified;
    densified.reserve(_pathPoints.size() * 8);
    densified.push_back(_pathPoints.front());

    auto heightAt = [this, map, dest](float x, float y, float interpolatedZ) -> float
    {
        LiquidData liq;
        ZLiquidStatus status = map->getLiquidStatus(x, y, dest.z, MAP_ALL_LIQUIDS, &liq);
        if (status == LIQUID_MAP_NO_WATER)
            status = map->getLiquidStatus(x, y, interpolatedZ, MAP_ALL_LIQUIDS, &liq);

        // Any liquid at this XY means swim, not walk on the seabed.
        if (status != LIQUID_MAP_NO_WATER && liq.level > liq.depth_level + 0.5f)
        {
            // Chase/follow should swim at the destination depth when that point is also
            // in water. Evade/home to land should keep the creature at its current swim
            // depth until it reaches shore instead of dropping to the seabed.
            float z = (CanSwimAt(dest.x, dest.y, dest.z) ? dest.z : _pathPoints.front().z);
            float const minZ = liq.depth_level + 1.0f;
            float const maxZ = liq.level - 0.1f;
            if (z > maxZ)
                z = maxZ;
            if (z < minZ)
                z = minZ;
            return z;
        }

        return ResolveTerrainZ(x, y, interpolatedZ);
    };

    float const stepSize = 2.0f;
    for (size_t i = 1; i < _pathPoints.size(); ++i)
    {
        G3D::Vector3 const& prev = densified.back();
        G3D::Vector3 const& next = _pathPoints[i];
        float const dx = next.x - prev.x;
        float const dy = next.y - prev.y;
        float const dist2d = sqrt(dx * dx + dy * dy);
        if (dist2d > stepSize)
        {
            uint32 const steps = uint32(dist2d / stepSize);
            for (uint32 s = 1; s < steps; ++s)
            {
                float const t = float(s) / float(steps);
                float const x = prev.x + dx * t;
                float const y = prev.y + dy * t;
                float const z = prev.z + (next.z - prev.z) * t;
                densified.emplace_back(x, y, heightAt(x, y, z));
            }
        }

        densified.emplace_back(next.x, next.y, heightAt(next.x, next.y, next.z));
    }

    _pathPoints.swap(densified);
}

void PathGenerator::FinalizePathElevation()
{
    SettleLandPointsToTerrain();
    SmoothVerticalHops();
}

void PathGenerator::SettleLandPointsToTerrain()
{
    if (_pathPoints.size() < 2)
        return;
    if (_sourceUnit->GetTypeId() == TypeID::TYPEID_UNIT && _sourceUnit->CanFly())
        return;

    Map const* map = _sourceUnit->GetBaseMap();
    uint32 const phase = _sourceUnit->GetPhaseMask();
    G3D::Vector3 const& dest = GetEndPosition();

    // Skip the live start so Launch does not pop the unit. Recast's first land
    // poly after water is often 1-2y above ADT (murloc mmap path: dest 57.9,
    // actual 59.3).
    for (size_t i = 1; i < _pathPoints.size(); ++i)
    {
        G3D::Vector3& p = _pathPoints[i];
        if (map->IsInWater(p.x, p.y, p.z))
            continue;

        float const floor = map->GetHeight(phase, p.x, p.y, p.z + 2.0f, true);
        if (floor <= INVALID_HEIGHT)
            continue;
        float const dz = p.z - floor;
        if (dz > 0.5f && dz <= 2.5f)
            p.z = floor;
    }

    G3D::Vector3& last = _pathPoints.back();
    float const edx = last.x - dest.x;
    float const edy = last.y - dest.y;
    if (edx * edx + edy * edy <= 4.0f && std::fabs(last.z - dest.z) <= 2.5f)
        last.z = dest.z;

    SetActualEndPosition(last);
}

void PathGenerator::SmoothVerticalHops()
{
    if (_pathPoints.size() < 2)
        return;

    Movement::PointsArray out;
    out.reserve(_pathPoints.size() * 2);
    out.push_back(_pathPoints.front());

    float const maxStepZ = 0.5f;
    for (size_t i = 1; i < _pathPoints.size(); ++i)
    {
        G3D::Vector3 const& a = out.back();
        G3D::Vector3 const& b = _pathPoints[i];
        float const dz = b.z - a.z;
        if (std::fabs(dz) <= 1.0f || out.size() + 2 >= MAX_POINT_PATH_LENGTH)
        {
            out.push_back(b);
            continue;
        }

        uint32 steps = uint32(std::ceil(std::fabs(dz) / maxStepZ));
        if (steps < 2)
            steps = 2;
        while (out.size() + steps >= MAX_POINT_PATH_LENGTH && steps > 1)
            --steps;

        float const dx = b.x - a.x;
        float const dy = b.y - a.y;
        for (uint32 s = 1; s < steps; ++s)
        {
            float const t = float(s) / float(steps);
            out.emplace_back(a.x + dx * t, a.y + dy * t, a.z + dz * t);
        }
        out.push_back(b);
    }

    _pathPoints.swap(out);
    SetActualEndPosition(_pathPoints.back());
}

void PathGenerator::BuildShortcut()
{
    SF_LOG_DEBUG("maps", "++ BuildShortcut :: making shortcut\n");

    Clear();

    // make two point path, our curr pos is the start, and dest is the end
    _pathPoints.resize(2);

    // set start and a default next position
    _pathPoints[0] = GetStartPosition();
    _pathPoints[1] = GetActualEndPosition();

    DensifyGroundPath();
    _rawPathPointCount = _pathPoints.size();

    _type = PATHFIND_SHORTCUT;
}

bool PathGenerator::CanSwimAt(float x, float y, float z) const
{
    if (_sourceUnit->GetTypeId() != TypeID::TYPEID_UNIT)
        return false;
    Creature const* creature = _sourceUnit->ToCreature();
    return creature && creature->CanSwim() && _sourceUnit->GetBaseMap()->IsInWater(x, y, z);
}

bool PathGenerator::HabitatAllowsPoint(float x, float y, float z, bool allowWater, bool allowLand) const
{
    bool const inWater = _sourceUnit->GetBaseMap()->IsInWater(x, y, z);
    return inWater ? allowWater : allowLand;
}

void PathGenerator::ClampPathToHabitat()
{
    if (_pathPoints.size() < 2)
        return;
    if (_sourceUnit->GetTypeId() != TypeID::TYPEID_UNIT)
        return;

    Creature const* creature = _sourceUnit->ToCreature();
    if (!creature || creature->CanFly())
        return;

    bool const canWalk = creature->CanWalk();
    bool const canSwim = creature->CanSwim();
    // Amphibious (InhabitType 3): enter and leave water freely.
    if (canWalk && canSwim)
        return;

    bool const allowWater = canSwim;
    bool const allowLand = canWalk;
    auto allowed = [this, allowWater, allowLand](G3D::Vector3 const& p) -> bool
    {
        return HabitatAllowsPoint(p.x, p.y, p.z, allowWater, allowLand);
    };

    // Displaced into illegal terrain: keep the existing UpdateFilter cheat.
    if (!allowed(_pathPoints.front()))
        return;

    size_t lastGood = 0;
    for (size_t i = 1; i < _pathPoints.size(); ++i)
    {
        if (!allowed(_pathPoints[i]))
            break;
        lastGood = i;
    }

    if (lastGood + 1 == _pathPoints.size())
        return;

    G3D::Vector3 const& a = _pathPoints[lastGood];
    G3D::Vector3 const& b = _pathPoints[lastGood + 1];
    G3D::Vector3 edge = a;
    float lo = 0.0f;
    float hi = 1.0f;
    for (int iter = 0; iter < 8; ++iter)
    {
        float const mid = (lo + hi) * 0.5f;
        G3D::Vector3 const p = a + (b - a) * mid;
        if (allowed(p))
        {
            edge = p;
            lo = mid;
        }
        else
            hi = mid;
    }

    float const dx = edge.x - a.x;
    float const dy = edge.y - a.y;
    if (dx * dx + dy * dy < 0.05f * 0.05f)
    {
        _pathPoints.resize(lastGood + 1);
        if (!_pathPoints.empty())
            SetActualEndPosition(_pathPoints.back());
        _type = PathType(_type | PATHFIND_INCOMPLETE);
        return;
    }

    _pathPoints.resize(lastGood + 2);
    _pathPoints[lastGood + 1] = edge;
    SetActualEndPosition(edge);
    _type = PathType(_type | PATHFIND_INCOMPLETE);
}

bool PathGenerator::CanBuildDirectShortcut(G3D::Vector3 const& from, G3D::Vector3 const& to) const
{
    if (_sourceUnit->GetTypeId() == TypeID::TYPEID_UNIT && _sourceUnit->ToCreature()->CanFly())
        return true;

    // Open-water chase may cut a 3D swim. Ground/ledges may not.
    if (CanSwimAt(from.x, from.y, from.z) && CanSwimAt(to.x, to.y, to.z))
        return true;

    return std::fabs(to.z - from.z) <= 2.5f;
}

void PathGenerator::CreateFilter()
{
    uint16 includeFlags = 0;
    uint16 excludeFlags = 0;

    if (_sourceUnit->GetTypeId() == TypeID::TYPEID_UNIT)
    {
        Creature* creature = (Creature*)_sourceUnit;
        if (creature->CanWalk())
            includeFlags |= NAV_GROUND;          // walk

        // creatures don't take environmental damage
        if (creature->CanSwim())
            includeFlags |= (NAV_WATER | NAV_MAGMA | NAV_SLIME);           // swim
    }
    else // assume Player
    {
        // perfect support not possible, just stay 'safe'
        includeFlags |= (NAV_GROUND | NAV_WATER | NAV_MAGMA | NAV_SLIME);
    }

    _filter.setIncludeFlags(includeFlags);
    _filter.setExcludeFlags(excludeFlags);

    UpdateFilter();
}

void PathGenerator::UpdateFilter()
{
    // allow creatures to cheat and use different movement types if they are moved
    // forcefully into terrain they can't normally move in
    if (_sourceUnit->IsInWater() || _sourceUnit->IsUnderWater())
    {
        uint16 includedFlags = _filter.getIncludeFlags();
        includedFlags |= GetNavTerrain(_sourceUnit->GetPositionX(),
            _sourceUnit->GetPositionY(),
            _sourceUnit->GetPositionZ());

        _filter.setIncludeFlags(includedFlags);
    }
}

NavTerrain PathGenerator::GetNavTerrain(float x, float y, float z)
{
    LiquidData data;
    ZLiquidStatus liquidStatus = _sourceUnit->GetBaseMap()->getLiquidStatus(x, y, z, MAP_ALL_LIQUIDS, &data);
    if (liquidStatus == LIQUID_MAP_NO_WATER)
        return NAV_GROUND;

    switch (data.type_flags)
    {
        case MAP_LIQUID_TYPE_WATER:
        case MAP_LIQUID_TYPE_OCEAN:
            return NAV_WATER;
        case MAP_LIQUID_TYPE_MAGMA:
            return NAV_MAGMA;
        case MAP_LIQUID_TYPE_SLIME:
            return NAV_SLIME;
        default:
            return NAV_GROUND;
    }
}

bool PathGenerator::HaveTile(const G3D::Vector3& p) const
{
    int tx = -1, ty = -1;
    float point[VERTEX_SIZE] = { p.y, p.z, p.x };

    _navMesh->calcTileLoc(point, &tx, &ty);

    /// Workaround
    /// For some reason, often the tx and ty variables wont get a valid value
    /// Use this check to prevent getting negative tile coords and crashing on getTileAt
    if (tx < 0 || ty < 0)
        return false;

    return (_navMesh->getTileAt(tx, ty, 0) != NULL);
}

uint32 PathGenerator::FixupCorridor(dtPolyRef* path, uint32 npath, uint32 maxPath, dtPolyRef const* visited, uint32 nvisited)
{
    int32 furthestPath = -1;
    int32 furthestVisited = -1;

    // Find furthest common polygon.
    for (int32 i = npath - 1; i >= 0; --i)
    {
        bool found = false;
        for (int32 j = nvisited - 1; j >= 0; --j)
        {
            if (path[i] == visited[j])
            {
                furthestPath = i;
                furthestVisited = j;
                found = true;
            }
        }
        if (found)
            break;
    }

    // If no intersection found just return current path.
    if (furthestPath == -1 || furthestVisited == -1)
        return npath;

    // Concatenate paths.

    // Adjust beginning of the buffer to include the visited.
    uint32 req = nvisited - furthestVisited;
    uint32 orig = uint32(furthestPath + 1) < npath ? furthestPath + 1 : npath;
    uint32 size = npath > orig ? npath - orig : 0;
    if (req + size > maxPath)
        size = maxPath - req;

    if (size)
        memmove(path + req, path + orig, size * sizeof(dtPolyRef));

    // Store visited
    for (uint32 i = 0; i < req; ++i)
        path[i] = visited[(nvisited - 1) - i];

    return req + size;
}

bool PathGenerator::GetSteerTarget(float const* startPos, float const* endPos,
    float minTargetDist, dtPolyRef const* path, uint32 pathSize,
    float* steerPos, unsigned char& steerPosFlag, dtPolyRef& steerPosRef)
{
    // Find steer target.
    static const uint32 MAX_STEER_POINTS = 3;
    float steerPath[MAX_STEER_POINTS * VERTEX_SIZE];
    unsigned char steerPathFlags[MAX_STEER_POINTS];
    dtPolyRef steerPathPolys[MAX_STEER_POINTS];
    uint32 nsteerPath = 0;
    dtStatus dtResult = _navMeshQuery->findStraightPath(startPos, endPos, path, pathSize,
        steerPath, steerPathFlags, steerPathPolys, (int*)&nsteerPath, MAX_STEER_POINTS);
    if (!nsteerPath || dtStatusFailed(dtResult))
        return false;

    // Find vertex far enough to steer to.
    uint32 ns = 0;
    while (ns < nsteerPath)
    {
        // Stop at Off-Mesh link or when point is further than slop away.
        if ((steerPathFlags[ns] & DT_STRAIGHTPATH_OFFMESH_CONNECTION) ||
            !InRangeYZX(&steerPath[ns * VERTEX_SIZE], startPos, minTargetDist, 1000.0f))
            break;
        ns++;
    }
    // Failed to find good point to steer to.
    if (ns >= nsteerPath)
        return false;

    dtVcopy(steerPos, &steerPath[ns * VERTEX_SIZE]);
    steerPos[1] = startPos[1];  // keep Z value
    steerPosFlag = steerPathFlags[ns];
    steerPosRef = steerPathPolys[ns];

    return true;
}

dtStatus PathGenerator::FindSmoothPath(float const* startPos, float const* endPos,
    dtPolyRef const* polyPath, uint32 polyPathSize,
    float* smoothPath, int* smoothPathSize, uint32 maxSmoothPathSize)
{
    *smoothPathSize = 0;
    uint32 nsmoothPath = 0;

    dtPolyRef polys[MAX_PATH_LENGTH];
    memcpy(polys, polyPath, sizeof(dtPolyRef) * polyPathSize);
    uint32 npolys = polyPathSize;

    float iterPos[VERTEX_SIZE], targetPos[VERTEX_SIZE];
    if (dtStatusFailed(_navMeshQuery->closestPointOnPolyBoundary(polys[0], startPos, iterPos)))
        return DT_FAILURE;

    if (dtStatusFailed(_navMeshQuery->closestPointOnPolyBoundary(polys[npolys - 1], endPos, targetPos)))
        return DT_FAILURE;

    dtVcopy(&smoothPath[nsmoothPath * VERTEX_SIZE], iterPos);
    nsmoothPath++;

    // Move towards target a small advancement at a time until target reached or
    // when ran out of memory to store the path.
    while (npolys && nsmoothPath < maxSmoothPathSize)
    {
        // Find location to steer towards.
        float steerPos[VERTEX_SIZE];
        unsigned char steerPosFlag;
        dtPolyRef steerPosRef = INVALID_POLYREF;

        if (!GetSteerTarget(iterPos, targetPos, SMOOTH_PATH_SLOP, polys, npolys, steerPos, steerPosFlag, steerPosRef))
            break;

        bool endOfPath = (steerPosFlag & DT_STRAIGHTPATH_END);
        bool offMeshConnection = (steerPosFlag & DT_STRAIGHTPATH_OFFMESH_CONNECTION);

        // Find movement delta.
        float delta[VERTEX_SIZE];
        dtVsub(delta, steerPos, iterPos);
        float len = dtSqrt(dtVdot(delta, delta));
        // If the steer target is end of path or off-mesh link, do not move past the location.
        if ((endOfPath || offMeshConnection) && len < SMOOTH_PATH_STEP_SIZE)
            len = 1.0f;
        else
            len = SMOOTH_PATH_STEP_SIZE / len;

        float moveTgt[VERTEX_SIZE];
        dtVmad(moveTgt, iterPos, delta, len);

        // Move
        float result[VERTEX_SIZE];
        const static uint32 MAX_VISIT_POLY = 16;
        dtPolyRef visited[MAX_VISIT_POLY];

        uint32 nvisited = 0;
        _navMeshQuery->moveAlongSurface(polys[0], iterPos, moveTgt, &_filter, result, visited, (int*)&nvisited, MAX_VISIT_POLY);
        npolys = FixupCorridor(polys, npolys, MAX_PATH_LENGTH, visited, nvisited);

        _navMeshQuery->getPolyHeight(polys[0], result, &result[1]);
        result[1] += 0.5f;
        dtVcopy(iterPos, result);

        // Handle end of path and off-mesh links when close enough.
        if (endOfPath && InRangeYZX(iterPos, steerPos, SMOOTH_PATH_SLOP, 1.0f))
        {
            // Reached end of path.
            dtVcopy(iterPos, targetPos);
            if (nsmoothPath < maxSmoothPathSize)
            {
                dtVcopy(&smoothPath[nsmoothPath * VERTEX_SIZE], iterPos);
                nsmoothPath++;
            }
            break;
        }
        else if (offMeshConnection && InRangeYZX(iterPos, steerPos, SMOOTH_PATH_SLOP, 1.0f))
        {
            // Advance the path up to and over the off-mesh connection.
            dtPolyRef prevRef = INVALID_POLYREF;
            dtPolyRef polyRef = polys[0];
            uint32 npos = 0;
            while (npos < npolys && polyRef != steerPosRef)
            {
                prevRef = polyRef;
                polyRef = polys[npos];
                npos++;
            }

            for (uint32 i = npos; i < npolys; ++i)
                polys[i - npos] = polys[i];

            npolys -= npos;

            // Handle the connection.
            float startPos[VERTEX_SIZE], endPos[VERTEX_SIZE];
            if (dtStatusSucceed(_navMesh->getOffMeshConnectionPolyEndPoints(prevRef, polyRef, startPos, endPos)))
            {
                if (nsmoothPath < maxSmoothPathSize)
                {
                    dtVcopy(&smoothPath[nsmoothPath * VERTEX_SIZE], startPos);
                    nsmoothPath++;
                }
                // Move position at the other side of the off-mesh link.
                dtVcopy(iterPos, endPos);
                _navMeshQuery->getPolyHeight(polys[0], iterPos, &iterPos[1]);
                iterPos[1] += 0.5f;
            }
        }

        // Store results.
        if (nsmoothPath < maxSmoothPathSize)
        {
            dtVcopy(&smoothPath[nsmoothPath * VERTEX_SIZE], iterPos);
            nsmoothPath++;
        }
    }

    *smoothPathSize = nsmoothPath;

    // this is most likely a loop
    return nsmoothPath < MAX_POINT_PATH_LENGTH ? DT_SUCCESS : DT_FAILURE;
}

bool PathGenerator::InRangeYZX(const float* v1, const float* v2, float r, float h) const
{
    const float dx = v2[0] - v1[0];
    const float dy = v2[1] - v1[1]; // elevation
    const float dz = v2[2] - v1[2];
    return (dx * dx + dz * dz) < r * r && fabsf(dy) < h;
}

bool PathGenerator::InRange(G3D::Vector3 const& p1, G3D::Vector3 const& p2, float r, float h) const
{
    G3D::Vector3 d = p1 - p2;
    return (d.x * d.x + d.y * d.y) < r * r && fabsf(d.z) < h;
}

float PathGenerator::Dist3DSqr(G3D::Vector3 const& p1, G3D::Vector3 const& p2) const
{
    return (p1 - p2).squaredLength();
}
