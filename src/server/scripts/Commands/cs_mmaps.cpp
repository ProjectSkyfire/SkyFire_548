/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/**
* @file cs_mmaps.cpp
* @brief .mmap related commands
*
* This file contains the CommandScripts for all
* mmap sub-commands
*/

#include "CellImpl.h"
#include "Chat.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Map.h"
#include "MMapFactory.h"
#include "ObjectMgr.h"
#include "PathGenerator.h"
#include "Player.h"
#include "PointMovementGenerator.h"
#include "ScriptMgr.h"
#include "TargetedMovementGenerator.h"

#include <algorithm>
#include <cmath>

class mmaps_commandscript : public CommandScript
{
public:
    mmaps_commandscript() : CommandScript("mmaps_commandscript") { }

    std::vector<ChatCommand> GetCommands() const OVERRIDE
    {
        static std::vector<ChatCommand> mmapCommandTable =
        {
            { "loadedtiles", rbac::RBAC_PERM_COMMAND_MMAP_LOADEDTILES, false, &HandleMmapLoadedTilesCommand, "", },
            { "loc",         rbac::RBAC_PERM_COMMAND_MMAP_LOC,         false, &HandleMmapLocCommand,         "", },
            { "path",        rbac::RBAC_PERM_COMMAND_MMAP_PATH,        false, &HandleMmapPathCommand,        "", },
            { "stats",       rbac::RBAC_PERM_COMMAND_MMAP_STATS,       false, &HandleMmapStatsCommand,       "", },
            { "testarea",    rbac::RBAC_PERM_COMMAND_MMAP_TESTAREA,    false, &HandleMmapTestArea,           "", },
        };

        static std::vector<ChatCommand> commandTable =
        {
            { "mmap", rbac::RBAC_PERM_COMMAND_MMAP, true, NULL, "", mmapCommandTable  },
        };
        return commandTable;
    }

    // Grid index of a world coordinate, matching the mmtile file naming.
    static int32 TileOf(float coord)
    {
        return int32(32 - coord / SIZE_OF_GRIDS);
    }

    static float DistanceToTileEdge(float coord)
    {
        float const offset = fmod(fabs(coord), SIZE_OF_GRIDS);
        return std::min(offset, SIZE_OF_GRIDS - offset);
    }

    static bool HandleMmapPathCommand(ChatHandler* handler, char const* args)
    {
        if (!MMAP::MMapFactory::createOrGetMMapManager()->GetNavMesh(handler->GetSession()->GetPlayer()->GetMapId(), handler->GetSession()->GetPlayer()->GetTerrainSwaps()))
        {
            handler->PSendSysMessage("NavMesh not loaded for current map.");
            return true;
        }

        handler->PSendSysMessage("mmap path:");

        // units
        Player* player = handler->GetSession()->GetPlayer();
        Unit* target = handler->getSelectedUnit();
        if (!player || !target)
        {
            handler->PSendSysMessage("Invalid target/source selection.");
            return true;
        }

        char* para = strtok((char*)args, " ");

        bool useStraightPath = false;
        if (para && strcmp(para, "true") == 0)
            useStraightPath = true;

        // unit locations
        float x, y, z;
        player->GetPosition(x, y, z);

        // path
        PathGenerator path(target);
        path.SetUseStraightPath(useStraightPath);
        bool result = path.CalculatePath(x, y, z);

        Movement::PointsArray const& pointPath = path.GetPath();
        handler->PSendSysMessage("%s's path to %s:", target->GetName().c_str(), player->GetName().c_str());
        handler->PSendSysMessage("Building: %s", useStraightPath ? "StraightPath" : "SmoothPath");
        char const* typeExtra = "";
        if (path.GetPathType() & PATHFIND_NOPATH)
            typeExtra = " NOPATH";
        else if (path.GetPathType() & PATHFIND_INCOMPLETE)
            typeExtra = " INCOMPLETE";
        else if (path.GetPathType() & PATHFIND_NOT_USING_PATH)
            typeExtra = " ADT/shortcut";
        handler->PSendSysMessage("Result: %s - Length: " SIZEFMTD " - Type: %u%s",
            (result ? "true" : "false"), pointPath.size(), path.GetPathType(), typeExtra);

        G3D::Vector3 const& start = path.GetStartPosition();
        G3D::Vector3 const& end = path.GetEndPosition();
        G3D::Vector3 const& actualEnd = path.GetActualEndPosition();

        float path2d = 0.0f;
        for (uint32 i = 1; i < pointPath.size(); ++i)
        {
            float const pdx = pointPath[i].x - pointPath[i - 1].x;
            float const pdy = pointPath[i].y - pointPath[i - 1].y;
            path2d += sqrt(pdx * pdx + pdy * pdy);
        }
        float const sdx = end.x - start.x;
        float const sdy = end.y - start.y;
        float const straight2d = sqrt(sdx * sdx + sdy * sdy);
        handler->PSendSysMessage("2D: path=%.1f straight=%.1f", path2d, straight2d);

        handler->PSendSysMessage("StartPosition     (%.3f, %.3f, %.3f) tile [%02i,%02i]",
            start.x, start.y, start.z, TileOf(start.x), TileOf(start.y));
        handler->PSendSysMessage("EndPosition       (%.3f, %.3f, %.3f) tile [%02i,%02i]",
            end.x, end.y, end.z, TileOf(end.x), TileOf(end.y));
        handler->PSendSysMessage("ActualEndPosition (%.3f, %.3f, %.3f) tile [%02i,%02i]",
            actualEnd.x, actualEnd.y, actualEnd.z, TileOf(actualEnd.x), TileOf(actualEnd.y));

        // A path that dies on a tile edge did not fail to route, it failed to cross.
        if (path.GetPathType() & (PATHFIND_INCOMPLETE | PATHFIND_NOPATH))
        {
            float const edgeX = DistanceToTileEdge(actualEnd.x);
            float const edgeY = DistanceToTileEdge(actualEnd.y);
            if (edgeX < 0.5f || edgeY < 0.5f)
                handler->PSendSysMessage("Stops %.2f/%.2f yards from a tile edge - suspect tile stitching, not geometry.",
                    edgeX, edgeY);
        }

        Map const* map = target->GetMap();
        handler->PSendSysMessage("Dest height: destZ=%.3f adt=%.3f getHeight=%.3f",
            end.z,
            map->GetRawTerrainHeight(end.x, end.y),
            map->GetHeight(target->GetPhaseMask(), end.x, end.y, std::max(start.z, end.z) + 2.0f, true));

        uint32 underWalk = 0;
        uint32 overWalk = 0;
        float worstDz = 0.0f;
        float const probeZ = std::max(start.z, end.z) + 2.0f;
        for (uint32 i = 0; i < pointPath.size(); ++i)
        {
            float const walkZ = map->GetHeight(target->GetPhaseMask(), pointPath[i].x, pointPath[i].y, probeZ, true);
            if (walkZ <= INVALID_HEIGHT)
                continue;
            float const dz = pointPath[i].z - walkZ;
            if (dz < -1.0f)
            {
                ++underWalk;
                if (dz < worstDz)
                    worstDz = dz;
            }
            else if (dz > 1.5f)
                ++overWalk;
        }
        handler->PSendSysMessage("Off walkable: under=%u over=%u / " SIZEFMTD "  worst dz=%.2f",
            underWalk, overWalk, pointPath.size(), worstDz);

        if (path.GetPathType() & PATHFIND_NOPATH)
            handler->PSendSysMessage("NOPATH: orbs below are the raw straight line, the creature will not walk it.");

        if (!player->IsGameMaster())
            handler->PSendSysMessage("Enable GM mode to see the path points.");

        for (uint32 i = 0; i < pointPath.size(); ++i)
            player->SummonCreature(VISUAL_WAYPOINT, pointPath[i].x, pointPath[i].y, pointPath[i].z, 0, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 9000);

        return true;
    }

    static bool HandleMmapLocCommand(ChatHandler* handler, char const* /*args*/)
    {
        handler->PSendSysMessage("mmap tileloc:");

        // grid tile location, of the selected unit when there is one
        Player* player = handler->GetSession()->GetPlayer();
        WorldObject* subject = handler->getSelectedUnit();
        if (!subject)
            subject = player;

        int32 gx = TileOf(subject->GetPositionX());
        int32 gy = TileOf(subject->GetPositionY());

        handler->PSendSysMessage("%s at (%.3f, %.3f, %.3f)", subject->GetName().c_str(),
            subject->GetPositionX(), subject->GetPositionY(), subject->GetPositionZ());
        handler->PSendSysMessage("%04u_%02i_%02i.mmtile", player->GetMapId(), gx, gy);
        handler->PSendSysMessage("gridloc [%i, %i]  edge dist %.2f / %.2f",
            gx, gy, DistanceToTileEdge(subject->GetPositionX()), DistanceToTileEdge(subject->GetPositionY()));

        // calculate navmesh tile location
        dtNavMesh const* navmesh = MMAP::MMapFactory::createOrGetMMapManager()->GetNavMesh(handler->GetSession()->GetPlayer()->GetMapId(), handler->GetSession()->GetPlayer()->GetTerrainSwaps());
        dtNavMeshQuery const* navmeshquery = MMAP::MMapFactory::createOrGetMMapManager()->GetNavMeshQuery(handler->GetSession()->GetPlayer()->GetMapId(), player->GetInstanceId(), handler->GetSession()->GetPlayer()->GetTerrainSwaps());
        if (!navmesh || !navmeshquery)
        {
            handler->PSendSysMessage("NavMesh not loaded for current map.");
            return true;
        }

        float const* min = navmesh->getParams()->orig;
        float x, y, z;
        subject->GetPosition(x, y, z);
        float location[VERTEX_SIZE] = { y, z, x };
        float extents[VERTEX_SIZE] = { 3.0f, 5.0f, 3.0f };

        int32 tilex = int32((y - min[0]) / SIZE_OF_GRIDS);
        int32 tiley = int32((x - min[2]) / SIZE_OF_GRIDS);

        handler->PSendSysMessage("Calc   [%02i, %02i]", tilex, tiley);

        // navmesh poly -> navmesh tile location
        dtQueryFilter filter = dtQueryFilter();
        dtPolyRef polyRef = INVALID_POLYREF;
        float nearest[VERTEX_SIZE] = { 0.0f, 0.0f, 0.0f };
        if (dtStatusFailed(navmeshquery->findNearestPoly(location, extents, &filter, &polyRef, nearest)))
        {
            handler->PSendSysMessage("Dt     [??,??] (invalid poly, probably no tile loaded)");
            return true;
        }

        // How far the unit sits from walkable mesh. Anything but ~0 means it is
        // standing somewhere Recast did not consider walkable.
        float const offX = nearest[0] - location[0];
        float const offY = nearest[1] - location[1];
        float const offZ = nearest[2] - location[2];
        handler->PSendSysMessage("Nearest poly offset: %.2f yards",
            sqrt(offX * offX + offY * offY + offZ * offZ));

        if (polyRef == INVALID_POLYREF)
            handler->PSendSysMessage("Dt     [??, ??] (invalid poly, probably no tile loaded)");
        else
        {
            dtMeshTile const* tile;
            dtPoly const* poly;
            if (dtStatusSucceed(navmesh->getTileAndPolyByRef(polyRef, &tile, &poly)))
            {
                if (tile)
                {
                    handler->PSendSysMessage("Dt     [%02i,%02i]", tile->header->x, tile->header->y);
                    return false;
                }
            }

            handler->PSendSysMessage("Dt     [??,??] (no tile loaded)");
        }

        return true;
    }

    static bool HandleMmapLoadedTilesCommand(ChatHandler* handler, char const* /*args*/)
    {
        uint32 mapid = handler->GetSession()->GetPlayer()->GetMapId();
        dtNavMesh const* navmesh = MMAP::MMapFactory::createOrGetMMapManager()->GetNavMesh(mapid, handler->GetSession()->GetPlayer()->GetTerrainSwaps());
        dtNavMeshQuery const* navmeshquery = MMAP::MMapFactory::createOrGetMMapManager()->GetNavMeshQuery(mapid, handler->GetSession()->GetPlayer()->GetInstanceId(), handler->GetSession()->GetPlayer()->GetTerrainSwaps());
        if (!navmesh || !navmeshquery)
        {
            handler->PSendSysMessage("NavMesh not loaded for current map.");
            return true;
        }

        handler->PSendSysMessage("mmap loadedtiles:");

        for (int32 i = 0; i < navmesh->getMaxTiles(); ++i)
        {
            dtMeshTile const* tile = navmesh->getTile(i);
            if (!tile || !tile->header)
                continue;

            handler->PSendSysMessage("[%02i, %02i]", tile->header->x, tile->header->y);
        }

        return true;
    }

    static bool HandleMmapStatsCommand(ChatHandler* handler, char const* /*args*/)
    {
        uint32 mapId = handler->GetSession()->GetPlayer()->GetMapId();
        handler->PSendSysMessage("mmap stats:");
        handler->PSendSysMessage("  global mmap pathfinding is %sabled", MMAP::MMapFactory::IsPathfindingEnabled(mapId) ? "en" : "dis");

        MMAP::MMapManager* manager = MMAP::MMapFactory::createOrGetMMapManager();
        handler->PSendSysMessage(" %u maps loaded with %u tiles overall", manager->getLoadedMapsCount(), manager->getLoadedTilesCount());

        dtNavMesh const* navmesh = manager->GetNavMesh(handler->GetSession()->GetPlayer()->GetMapId(), handler->GetSession()->GetPlayer()->GetTerrainSwaps());
        if (!navmesh)
        {
            handler->PSendSysMessage("NavMesh not loaded for current map.");
            return true;
        }

        uint32 tileCount = 0;
        uint32 nodeCount = 0;
        uint32 polyCount = 0;
        uint32 vertCount = 0;
        uint32 triCount = 0;
        uint32 triVertCount = 0;
        uint32 dataSize = 0;
        for (int32 i = 0; i < navmesh->getMaxTiles(); ++i)
        {
            dtMeshTile const* tile = navmesh->getTile(i);
            if (!tile || !tile->header)
                continue;

            tileCount++;
            nodeCount += tile->header->bvNodeCount;
            polyCount += tile->header->polyCount;
            vertCount += tile->header->vertCount;
            triCount += tile->header->detailTriCount;
            triVertCount += tile->header->detailVertCount;
            dataSize += tile->dataSize;
        }

        handler->PSendSysMessage("Navmesh stats:");
        handler->PSendSysMessage(" %u tiles loaded", tileCount);
        handler->PSendSysMessage(" %u BVTree nodes", nodeCount);
        handler->PSendSysMessage(" %u polygons (%u vertices)", polyCount, vertCount);
        handler->PSendSysMessage(" %u triangles (%u vertices)", triCount, triVertCount);
        handler->PSendSysMessage(" %.2f MB of data (not including pointers)", ((float)dataSize / sizeof(unsigned char)) / 1048576);

        return true;
    }

    static bool HandleMmapTestArea(ChatHandler* handler, char const* /*args*/)
    {
        float radius = 40.0f;
        WorldObject* object = handler->GetSession()->GetPlayer();

        CellCoord pair(Skyfire::ComputeCellCoord(object->GetPositionX(), object->GetPositionY()));
        Cell cell(pair);
        cell.SetNoCreate();

        std::list<Creature*> creatureList;

        Skyfire::AnyUnitInObjectRangeCheck go_check(object, radius);
        Skyfire::CreatureListSearcher<Skyfire::AnyUnitInObjectRangeCheck> go_search(object, creatureList, go_check);
        TypeContainerVisitor<Skyfire::CreatureListSearcher<Skyfire::AnyUnitInObjectRangeCheck>, GridTypeMapContainer> go_visit(go_search);

        // Get Creatures
        cell.Visit(pair, go_visit, *(object->GetMap()), *object, radius);

        if (!creatureList.empty())
        {
            handler->PSendSysMessage("Found " SIZEFMTD " Creatures.", creatureList.size());

            uint32 paths = 0;
            uint32 uStartTime = getMSTime();

            float gx, gy, gz;
            object->GetPosition(gx, gy, gz);
            for (std::list<Creature*>::iterator itr = creatureList.begin(); itr != creatureList.end(); ++itr)
            {
                PathGenerator path(*itr);
                path.CalculatePath(gx, gy, gz);
                ++paths;
            }

            uint32 uPathLoadTime = getMSTimeDiff(uStartTime, getMSTime());
            handler->PSendSysMessage("Generated %i paths in %i ms", paths, uPathLoadTime);
        }
        else
            handler->PSendSysMessage("No creatures in %f yard range.", radius);

        return true;
    }
};

void AddSC_mmaps_commandscript()
{
    new mmaps_commandscript();
}
