/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef _MAP_BUILDER_H
#define _MAP_BUILDER_H

#include <set>
#include <map>

#include "TerrainBuilder.h"
#include "IntermediateValues.h"

#include "Recast.h"
#include "DetourNavMesh.h"

using namespace VMAP;

// Keep G3D typedefs isolated from shared engine typedefs.

namespace MMAP
{
    typedef std::map<uint32, std::set<uint32>*> TileList;
    struct Tile
    {
        Tile() : chf(NULL), solid(NULL), cset(NULL), pmesh(NULL), dmesh(NULL) {}
        ~Tile()
        {
            rcFreeCompactHeightfield(chf);
            rcFreeContourSet(cset);
            rcFreeHeightField(solid);
            rcFreePolyMesh(pmesh);
            rcFreePolyMeshDetail(dmesh);
        }
        rcCompactHeightfield* chf;
        rcHeightfield* solid;
        rcContourSet* cset;
        rcPolyMesh* pmesh;
        rcPolyMeshDetail* dmesh;
    };

    class MapBuilder
    {
        public:
            MapBuilder(float maxWalkableAngle   = 55.0f,
                bool skipLiquid          = false,
                bool skipContinents      = false,
                bool skipJunkMaps        = true,
                bool skipBattlegrounds   = false,
                bool debugOutput         = false,
                bool bigBaseUnit         = false,
                const char* offMeshFilePath = NULL);

            ~MapBuilder();

            // builds all mmap tiles for the specified map id (ignores skip settings)
            void buildMap(uint32 mapID);
            void buildMeshFromFile(char* name);

            // builds an mmap tile for the specified map and its mesh
            void buildSingleTile(uint32 mapID, uint32 tileX, uint32 tileY);

            // builds list of maps, then builds all of mmap tiles (based on the skip settings)
            void buildAllMaps(unsigned int threads);

        private:
            // detect maps and tiles
            void discoverTiles();
            std::set<uint32>* getTileList(uint32 mapID);

            void buildNavMesh(uint32 mapID, dtNavMesh* &navMesh);

            void buildTile(uint32 mapID, uint32 tileX, uint32 tileY, dtNavMesh* navMesh);

            // move map building
            void buildMoveMapTile(uint32 mapID,
                uint32 tileX,
                uint32 tileY,
                MeshData &meshData,
                float bmin[3],
                float bmax[3],
                dtNavMesh* navMesh);

            void getTileBounds(uint32 tileX, uint32 tileY,
                float* verts, int vertCount,
                float* bmin, float* bmax);
            void getGridBounds(uint32 mapID, uint32 &minX, uint32 &minY, uint32 &maxX, uint32 &maxY);

            bool shouldSkipMap(uint32 mapID);
            bool isTransportMap(uint32 mapID);
            bool shouldSkipTile(uint32 mapID, uint32 tileX, uint32 tileY);

            // reads dbc/Map.dbc directly (as extracted by the map extractor) to answer
            // isContinentMap/isBattlegroundMap without a hardcoded id list
            void loadMapDbcData();
            bool isContinentMap(uint32 mapID);
            bool isBattlegroundMap(uint32 mapID);

            struct MapDbcEntry
            {
                uint32 mapType;
                uint32 flags;
            };
            std::map<uint32, MapDbcEntry> m_mapDbcData;
            bool m_mapDbcLoaded;

            TerrainBuilder* m_terrainBuilder;
            TileList m_tiles;

            bool m_debugOutput;

            const char* m_offMeshFilePath;
            bool m_skipContinents;
            bool m_skipJunkMaps;
            bool m_skipBattlegrounds;

            float m_maxWalkableAngle;
            bool m_bigBaseUnit;

            // build performance - not really used for now
            rcContext* m_rcContext;
    };

}

#endif
