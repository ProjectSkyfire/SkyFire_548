/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef _MMAP_MANAGER_H
#define _MMAP_MANAGER_H

#include "Define.h"
#include "DetourAlloc.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshQuery.h"
#include "World.h"
#include <string>
#include <unordered_map>
#include <set>

// move map related classes
namespace MMAP
{
    typedef UNORDERED_MAP<uint32, dtTileRef> MMapTileSet;
    typedef UNORDERED_MAP<uint32, dtNavMeshQuery*> NavMeshQuerySet;


    typedef std::set<uint32> TerrainSet;

    struct NavMeshHolder
    {
        // Pre-built navMesh
        dtNavMesh* navMesh;

        // List of terrain swap map ids used to build the navMesh
        TerrainSet terrainIds;

        MMapTileSet loadedTileRefs;
    };

    struct PhasedTile
    {
        unsigned char* data;
        MmapTileHeader fileHeader;
        int32 dataSize;
    };

    typedef UNORDERED_MAP<uint32, PhasedTile*> PhaseTileContainer;
    typedef UNORDERED_MAP<uint32, PhaseTileContainer> PhaseTileMap;


    typedef UNORDERED_MAP<uint32, TerrainSet> TerrainSetMap;

    class MMapData
    {
    public:
        MMapData(dtNavMesh* mesh, uint32 mapId) : navMesh(mesh), _mapId(mapId) { }
        
        ~MMapData();

        dtNavMesh* GetNavMesh(TerrainSet swaps);

        // we have to use single dtNavMeshQuery for every instance, since those are not thread safe
        NavMeshQuerySet navMeshQueries;     // instanceId to query

        dtNavMesh* navMesh;
        MMapTileSet loadedTileRefs;
        TerrainSetMap loadedPhasedTiles;

    private:
        uint32 _mapId;
        PhaseTileContainer _baseTiles;
        std::set<uint32> _activeSwaps;
        void RemoveSwap(PhasedTile* ptile, uint32 swap, uint32 packedXY);
        void AddSwap(PhasedTile* tile, uint32 swap, uint32 packedXY);
    };


    typedef UNORDERED_MAP<uint32, MMapData*> MMapDataSet;

    // singleton class
    // holds all all access to mmap loading unloading and meshes
    class MMapManager
    {
    public:
        MMapManager() : loadedTiles(0) { }
        ~MMapManager();

        bool loadMap(const std::string& basePath, uint32 mapId, int32 x, int32 y);
        bool unloadMap(uint32 mapId, int32 x, int32 y);
        bool unloadMap(uint32 mapId);
        bool unloadMapInstance(uint32 mapId, uint32 instanceId);

        // the returned [dtNavMeshQuery const*] is NOT threadsafe
        dtNavMeshQuery const* GetNavMeshQuery(uint32 mapId, uint32 instanceId, TerrainSet swaps);
        dtNavMesh const* GetNavMesh(uint32 mapId, TerrainSet swaps);

        uint32 getLoadedTilesCount() const { return loadedTiles; }
        uint32 getLoadedMapsCount() const { return loadedMMaps.size(); }

        void LoadPhaseTiles(uint32 mapId, int32 x, int32 y);
        void UnloadPhaseTile(uint32 mapId, int32 x, int32 y);
        PhaseTileContainer GetPhaseTileContainer(uint32 mapId) { return _phaseTiles[mapId]; }

    private:
        bool loadMapData(uint32 mapId);
        uint32 packTileID(int32 x, int32 y);

        MMapDataSet loadedMMaps;
        uint32 loadedTiles;

        PhasedTile* LoadTile(uint32 mapId, int32 x, int32 y);
        PhaseTileMap _phaseTiles;
    };
}

#endif