/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "MMapManager.h"
#include "Log.h"
#include "DBCStores.h"
#include "MMapFactory.h"

namespace MMAP
{
    // ######################## MMapManager ########################
    MMapManager::~MMapManager()
    {
        for (MMapDataSet::iterator i = loadedMMaps.begin(); i != loadedMMaps.end(); ++i)
            delete i->second;

        // by now we should not have maps loaded
        // if we had, tiles in MMapData->mmapLoadedTiles, their actual data is lost!
    }

    bool MMapManager::loadMapData(uint32 mapId)
    {
        // we already have this map loaded?
        if (loadedMMaps.find(mapId) != loadedMMaps.end())
            return true;

        // load and init dtNavMesh - read parameters from file
        uint32 pathLen = sWorld->GetDataPath().length() + strlen("mmaps/%04i.mmap") + 1;
        char* fileName = new char[pathLen];
        snprintf(fileName, pathLen, (sWorld->GetDataPath() + "mmaps/%04i.mmap").c_str(), mapId);

        FILE* file = fopen(fileName, "rb");
        if (!file)
        {
            SF_LOG_DEBUG("maps", "MMAP:loadMapData: Error: Could not open mmap file '%s'", fileName);
            delete[] fileName;
            return false;
        }

        dtNavMeshParams params;
        int count = fread(&params, sizeof(dtNavMeshParams), 1, file);
        fclose(file);
        if (count != 1)
        {
            SF_LOG_DEBUG("maps", "MMAP:loadMapData: Error: Could not read params from file '%s'", fileName);
            delete[] fileName;
            return false;
        }

        dtNavMesh* mesh = dtAllocNavMesh();
        ASSERT(mesh);
        if (dtStatusFailed(mesh->init(&params)))
        {
            dtFreeNavMesh(mesh);
            SF_LOG_ERROR("maps", "MMAP:loadMapData: Failed to initialize dtNavMesh for mmap %04u from file %s", mapId, fileName);
            delete[] fileName;
            return false;
        }

        delete[] fileName;

        SF_LOG_INFO("maps", "MMAP:loadMapData: Loaded %04i.mmap", mapId);

        // store inside our map list
        MMapData* mmap_data = new MMapData(mesh, mapId);

        loadedMMaps.insert(std::pair<uint32, MMapData*>(mapId, mmap_data));
        return true;
    }

    uint32 MMapManager::packTileID(int32 x, int32 y)
    {
        return uint32(x << 16 | y);
    }

    bool MMapManager::loadMap(const std::string& /*basePath*/, uint32 mapId, int32 x, int32 y)
    {
        // make sure the mmap is loaded and ready to load tiles
        if (!loadMapData(mapId))
            return false;

        // get this mmap data
        MMapData* mmap = loadedMMaps[mapId];
        ASSERT(mmap->navMesh);

        // check if we already have this tile loaded
        uint32 packedGridPos = packTileID(x, y);
        if (mmap->loadedTileRefs.find(packedGridPos) != mmap->loadedTileRefs.end())
            return false;

        // load this tile :: mmaps/MMMM_XX_YY.mmtile
        uint32 pathLen = sWorld->GetDataPath().length() + strlen("mmaps/%04i_%02i_%02i.mmtile") + 1;
        char* fileName = new char[pathLen];

        snprintf(fileName, pathLen, (sWorld->GetDataPath() + "mmaps/%04i_%02i_%02i.mmtile").c_str(), mapId, x, y);

        FILE* file = fopen(fileName, "rb");
        if (!file)
        {
            SF_LOG_DEBUG("maps", "MMAP:loadMap: Could not open mmtile file '%s'", fileName);
            delete[] fileName;
            return false;
        }
        delete[] fileName;

        // read header
        MmapTileHeader fileHeader;
        if (fread(&fileHeader, sizeof(MmapTileHeader), 1, file) != 1 || fileHeader.mmapMagic != MMAP_MAGIC)
        {
            SF_LOG_ERROR("maps", "MMAP:loadMap: Bad header in mmap %04u_%02i_%02i.mmtile", mapId, x, y);
            fclose(file);
            return false;
        }

        if (fileHeader.mmapVersion != MMAP_VERSION)
        {
            SF_LOG_ERROR("maps", "MMAP:loadMap: %04u_%02i_%02i.mmtile was built with generator v%f, expected v%f",
                mapId, x, y, fileHeader.mmapVersion, MMAP_VERSION);
            fclose(file);
            return false;
        }

        unsigned char* data = (unsigned char*)dtAlloc(fileHeader.size, DT_ALLOC_PERM);
        ASSERT(data);

        size_t result = fread(data, fileHeader.size, 1, file);
        if (!result)
        {
            SF_LOG_ERROR("maps", "MMAP:loadMap: Bad header or data in mmap %04u_%02i_%02i.mmtile", mapId, x, y);
            fclose(file);
            return false;
        }

        fclose(file);

        dtMeshHeader* header = (dtMeshHeader*)data;
        dtTileRef tileRef = 0;

        // memory allocated for data is now managed by detour, and will be deallocated when the tile is removed
        if (dtStatusSucceed(mmap->navMesh->addTile(data, fileHeader.size, 0/*DT_TILE_FREE_DATA*/, 0, &tileRef)))
        {
            mmap->loadedTileRefs.insert(std::pair<uint32, dtTileRef>(packedGridPos, tileRef));
            ++loadedTiles;
            SF_LOG_INFO("maps", "MMAP:loadMap: Loaded mmtile %04i[%02i, %02i] into %04i[%02i, %02i]", mapId, x, y, mapId, header->x, header->y);

            LoadPhaseTiles(mapId, x, y);

            return true;
        }
        else
        {
            SF_LOG_ERROR("maps", "MMAP:loadMap: Could not load %04u_%02i_%02i.mmtile into navmesh", mapId, x, y);
            dtFree(data);
            return false;
        }

        return false;
    }

    PhasedTile* MMapManager::LoadTile(uint32 mapId, int32 x, int32 y)
    {
        // load this tile :: mmaps/MMMM_XX_YY.mmtile
        uint32 pathLen = sWorld->GetDataPath().length() + strlen("mmaps/%04i_%02i_%02i.mmtile") + 1;
        char* fileName = new char[pathLen];

        snprintf(fileName, pathLen, (sWorld->GetDataPath() + "mmaps/%04i_%02i_%02i.mmtile").c_str(), mapId, x, y);

        FILE* file = fopen(fileName, "rb");
        if (!file)
        {
            // Not all tiles have phased versions, don't flood this msg
            //SF_LOG_DEBUG("phase", "MMAP:LoadTile: Could not open mmtile file '%s'", fileName);
            delete[] fileName;
            return NULL;
        }
        delete[] fileName;

        PhasedTile* pTile = new PhasedTile();

        // read header
        if (fread(&pTile->fileHeader, sizeof(MmapTileHeader), 1, file) != 1 || pTile->fileHeader.mmapMagic != MMAP_MAGIC)
        {
            SF_LOG_ERROR("phase", "MMAP:LoadTile: Bad header in mmap %04u_%02i_%02i.mmtile", mapId, x, y);
            delete pTile;
            fclose(file);
            return NULL;
        }

        if (pTile->fileHeader.mmapVersion != MMAP_VERSION)
        {
            SF_LOG_ERROR("phase", "MMAP:LoadTile: %04u_%02i_%02i.mmtile was built with generator v%f, expected v%f",
                mapId, x, y, pTile->fileHeader.mmapVersion, MMAP_VERSION);
            delete pTile;
            fclose(file);
            return NULL;
        }

        pTile->data = (unsigned char*)dtAlloc(pTile->fileHeader.size, DT_ALLOC_PERM);
        ASSERT(pTile->data);

        size_t result = fread(pTile->data, pTile->fileHeader.size, 1, file);
        if (!result)
        {
            SF_LOG_ERROR("phase", "MMAP:LoadTile: Bad header or data in mmap %04u_%02i_%02i.mmtile", mapId, x, y);
            delete pTile;
            fclose(file);
            return NULL;
        }

        fclose(file);

        return pTile;
    }

    void MMapManager::LoadPhaseTiles(uint32 mapId, int32 x, int32 y)
    {
        SF_LOG_DEBUG("phase", "MMAP:LoadPhaseTiles: Loading phased mmtiles for map %u, x: %i, y: %i", mapId, x, y);

        uint32 packedGridPos = packTileID(x, y);

        for (uint32 i = 0; i < sMapStore.GetNumRows(); ++i)
        {
            if (const MapEntry * const map = sMapStore.LookupEntry(i))
            {
                if (map->rootPhaseMap == mapId)
                {
                    PhasedTile* data = LoadTile(map->MapID, x, y);
                    // only a few tiles have terrain swaps, do not write error for them
                    if (data)
                    {
                        SF_LOG_DEBUG("phase", "MMAP:LoadPhaseTiles: Loaded phased %04u_%02i_%02i.mmtile for root phase map %u", map->MapID, x, y, mapId);
                        _phaseTiles[map->MapID][packedGridPos] = data;
                    }
                }
            }
        }
    }

    void MMapManager::UnloadPhaseTile(uint32 mapId, int32 x, int32 y)
    {
        SF_LOG_DEBUG("phase", "MMAP:UnloadPhaseTile: Unloading phased mmtile for map %u, x: %i, y: %i", mapId, x, y);

        uint32 packedGridPos = packTileID(x, y);

        const MapEntry* const map = sMapStore.LookupEntry(mapId); // map existence already checked when loading
        uint32 rootMapId = map->rootPhaseMap;

        if (_phaseTiles[mapId][packedGridPos])
        {
            SF_LOG_DEBUG("phase", "MMAP:UnloadPhaseTile: Unloaded phased %04u_%02i_%02i.mmtile for root phase map %u", mapId, x, y, rootMapId);
            delete _phaseTiles[mapId][packedGridPos]->data;
            delete _phaseTiles[mapId][packedGridPos];
            _phaseTiles[mapId].erase(packedGridPos);
        }
    }

    bool MMapManager::unloadMap(uint32 mapId, int32 x, int32 y)
    {
        // check if we have this map loaded
        if (loadedMMaps.find(mapId) == loadedMMaps.end())
        {
            // file may not exist, therefore not loaded
            SF_LOG_DEBUG("maps", "MMAP:unloadMap: Asked to unload not loaded navmesh map. %04u_%02i_%02i.mmtile", mapId, x, y);
            return false;
        }

        MMapData* mmap = loadedMMaps[mapId];

        // check if we have this tile loaded
        uint32 packedGridPos = packTileID(x, y);
        if (mmap->loadedTileRefs.find(packedGridPos) == mmap->loadedTileRefs.end())
        {
            // file may not exist, therefore not loaded
            SF_LOG_DEBUG("maps", "MMAP:unloadMap: Asked to unload not loaded navmesh tile. %04u_%02i_%02i.mmtile", mapId, x, y);
            return false;
        }

        dtTileRef tileRef = mmap->loadedTileRefs[packedGridPos];

        // unload, and mark as non loaded
        if (dtStatusFailed(mmap->navMesh->removeTile(tileRef, NULL, NULL)))
        {
            // this is technically a memory leak
            // if the grid is later reloaded, dtNavMesh::addTile will return error but no extra memory is used
            // we cannot recover from this error - assert out
            SF_LOG_ERROR("maps", "MMAP:unloadMap: Could not unload %04u_%02i_%02i.mmtile from navmesh", mapId, x, y);
            ASSERT(false);
        }
        else
        {
            mmap->loadedTileRefs.erase(packedGridPos);
            --loadedTiles;
            SF_LOG_INFO("maps", "MMAP:unloadMap: Unloaded mmtile %04i[%02i, %02i] from %04i", mapId, x, y, mapId);

            UnloadPhaseTile(mapId, x, y);
            return true;
        }

        return false;
    }

    bool MMapManager::unloadMap(uint32 mapId)
    {
        if (loadedMMaps.find(mapId) == loadedMMaps.end())
        {
            // file may not exist, therefore not loaded
            SF_LOG_DEBUG("maps", "MMAP:unloadMap: Asked to unload not loaded navmesh map %04u", mapId);
            return false;
        }

        // unload all tiles from given map
        MMapData* mmap = loadedMMaps[mapId];
        for (MMapTileSet::iterator i = mmap->loadedTileRefs.begin(); i != mmap->loadedTileRefs.end(); ++i)
        {
            uint32 x = (i->first >> 16);
            uint32 y = (i->first & 0x0000FFFF);
            if (dtStatusFailed(mmap->navMesh->removeTile(i->second, NULL, NULL)))
                SF_LOG_ERROR("maps", "MMAP:unloadMap: Could not unload %04u_%02i_%02i.mmtile from navmesh", mapId, x, y);
            else
            {
                UnloadPhaseTile(mapId, x, y);
                --loadedTiles;
                SF_LOG_INFO("maps", "MMAP:unloadMap: Unloaded mmtile %04i[%02i, %02i] from %04i", mapId, x, y, mapId);
            }
        }

        delete mmap;
        loadedMMaps.erase(mapId);
        SF_LOG_INFO("maps", "MMAP:unloadMap: Unloaded %04i.mmap", mapId);

        return true;
    }

    bool MMapManager::unloadMapInstance(uint32 mapId, uint32 instanceId)
    {
        // check if we have this map loaded
        if (loadedMMaps.find(mapId) == loadedMMaps.end())
        {
            // file may not exist, therefore not loaded
            SF_LOG_DEBUG("maps", "MMAP:unloadMapInstance: Asked to unload not loaded navmesh map %04u", mapId);
            return false;
        }

        MMapData* mmap = loadedMMaps[mapId];
        if (mmap->navMeshQueries.find(instanceId) == mmap->navMeshQueries.end())
        {
            SF_LOG_DEBUG("maps", "MMAP:unloadMapInstance: Asked to unload not loaded dtNavMeshQuery mapId %04u instanceId %u", mapId, instanceId);
            return false;
        }

        dtNavMeshQuery* query = mmap->navMeshQueries[instanceId];

        dtFreeNavMeshQuery(query);
        mmap->navMeshQueries.erase(instanceId);
        SF_LOG_INFO("maps", "MMAP:unloadMapInstance: Unloaded mapId %04u instanceId %u", mapId, instanceId);

        return true;
    }

    dtNavMesh const* MMapManager::GetNavMesh(uint32 mapId, TerrainSet swaps)
    {
        if (loadedMMaps.find(mapId) == loadedMMaps.end())
            return NULL;

        return loadedMMaps[mapId]->GetNavMesh(swaps);
    }

    dtNavMeshQuery const* MMapManager::GetNavMeshQuery(uint32 mapId, uint32 instanceId, TerrainSet swaps)
    {
        if (loadedMMaps.find(mapId) == loadedMMaps.end())
            return NULL;

        MMapData* mmap = loadedMMaps[mapId];
        if (mmap->navMeshQueries.find(instanceId) == mmap->navMeshQueries.end())
        {
            // allocate mesh query
            dtNavMeshQuery* query = dtAllocNavMeshQuery();
            ASSERT(query);
            if (dtStatusFailed(query->init(mmap->GetNavMesh(swaps), 1024)))
            {
                dtFreeNavMeshQuery(query);
                SF_LOG_ERROR("maps", "MMAP:GetNavMeshQuery: Failed to initialize dtNavMeshQuery for mapId %04u instanceId %u", mapId, instanceId);
                return NULL;
            }

            SF_LOG_INFO("maps", "MMAP:GetNavMeshQuery: created dtNavMeshQuery for mapId %04u instanceId %u", mapId, instanceId);
            mmap->navMeshQueries.insert(std::pair<uint32, dtNavMeshQuery*>(instanceId, query));
        }

        return mmap->navMeshQueries[instanceId];
    }

    MMapData::~MMapData()
    {
        for (NavMeshQuerySet::iterator i = navMeshQueries.begin(); i != navMeshQueries.end(); ++i)
            dtFreeNavMeshQuery(i->second);

        dtFreeNavMesh(navMesh);

        for (PhaseTileContainer::iterator i = _baseTiles.begin(); i != _baseTiles.end(); ++i)
        {
            delete (*i).second->data;
            delete (*i).second;
        }
    }

    void MMapData::RemoveSwap(PhasedTile* ptile, uint32 swap, uint32 packedXY)
    {
        uint32 x = (packedXY >> 16);
        uint32 y = (packedXY & 0x0000FFFF);

        if (loadedPhasedTiles[swap].find(packedXY) == loadedPhasedTiles[swap].end())
        {
            SF_LOG_DEBUG("phase", "MMapData::RemoveSwap: mmtile %04u[%02i, %02i] unload skipped, due to not loaded", swap, x, y);
            return;
        }
        dtMeshHeader* header = (dtMeshHeader*)ptile->data;

        // remove old tile
        if (dtStatusFailed(navMesh->removeTile(loadedTileRefs[packedXY], NULL, NULL)))
            SF_LOG_ERROR("phase", "MMapData::RemoveSwap: Could not unload phased %04u_%02i_%02i.mmtile from navmesh", swap, x, y);
        else
        {
            SF_LOG_DEBUG("phase", "MMapData::RemoveSwap: Unloaded phased %04u_%02i_%02i.mmtile from navmesh", swap, x, y);

            // restore base tile
            if (dtStatusSucceed(navMesh->addTile(_baseTiles[packedXY]->data, _baseTiles[packedXY]->dataSize, 0, 0, &loadedTileRefs[packedXY])))
            {
                SF_LOG_DEBUG("phase", "MMapData::RemoveSwap: Loaded base mmtile %04u[%02i, %02i] into %04i[%02i, %02i]", _mapId, x, y, _mapId, header->x, header->y);
            }
            else
                SF_LOG_ERROR("phase", "MMapData::RemoveSwap: Could not load base %04u_%02i_%02i.mmtile to navmesh", _mapId, x, y);
        }

        loadedPhasedTiles[swap].erase(packedXY);

        if (loadedPhasedTiles[swap].empty())
        {
            _activeSwaps.erase(swap);
            SF_LOG_DEBUG("phase", "MMapData::RemoveSwap: Fully removed swap %u from map %u", swap, _mapId);
        }
    }

    void MMapData::AddSwap(PhasedTile* ptile, uint32 swap, uint32 packedXY)
    {
        uint32 x = (packedXY >> 16);
        uint32 y = (packedXY & 0x0000FFFF);

        if (loadedTileRefs.find(packedXY) == loadedTileRefs.end())
        {
            SF_LOG_DEBUG("phase", "MMapData::AddSwap: phased mmtile %04u[%02i, %02i] load skipped, due to not loaded base tile on map %u", swap, x, y, _mapId);
            return;
        }
        if (loadedPhasedTiles[swap].find(packedXY) != loadedPhasedTiles[swap].end())
        {
            SF_LOG_DEBUG("phase", "MMapData::AddSwap: WARNING! phased mmtile %04u[%02i, %02i] load skipped, due to already loaded on map %u", swap, x, y, _mapId);
            return;
        }


        dtMeshHeader* header = (dtMeshHeader*)ptile->data;

        const dtMeshTile* oldTile = navMesh->getTileByRef(loadedTileRefs[packedXY]);
        if (!oldTile)
        {
            SF_LOG_DEBUG("phase", "MMapData::AddSwap: phased mmtile %04u[%02i, %02i] load skipped, due to not loaded base tile ref on map %u", swap, x, y, _mapId);
            return;
        }

        uint32 old_x = oldTile->header->x;
        uint32 old_y = oldTile->header->y;

        // header xy is based on the swap map's tile set, wich doesn't have all the same tiles as root map, so copy the xy from the orignal header
        memcpy(ptile->data + 8, (char*)& old_x, 4);
        memcpy(ptile->data + 12, (char*)& old_y, 4);

        // the removed tile's data
        PhasedTile* pt = new PhasedTile();
        // remove old tile
        if (dtStatusFailed(navMesh->removeTile(loadedTileRefs[packedXY], &pt->data, &pt->dataSize)))
            SF_LOG_ERROR("phase", "MMapData::AddSwap: Could not unload %04u_%02i_%02i.mmtile from navmesh", _mapId, x, y);
        else
        {
            SF_LOG_DEBUG("phase", "MMapData::AddSwap: Unloaded %04u_%02i_%02i.mmtile from navmesh", _mapId, x, y);

            // store the removed data first time, this is the origonal, non-phased tile
            if (_baseTiles.find(packedXY) == _baseTiles.end())
                _baseTiles[packedXY] = pt;

            _activeSwaps.insert(swap);
            loadedPhasedTiles[swap].insert(packedXY);

            // add new swapped tile
            if (dtStatusSucceed(navMesh->addTile(ptile->data, ptile->fileHeader.size, 0, 0, &loadedTileRefs[packedXY])))
            {
                SF_LOG_DEBUG("phase", "MMapData::AddSwap: Loaded phased mmtile %04u[%02i, %02i] into %04i[%02i, %02i]", swap, x, y, _mapId, header->x, header->y);
            }
            else
                SF_LOG_ERROR("phase", "MMapData::AddSwap: Could not load %04u_%02i_%02i.mmtile to navmesh", swap, x, y);
        }
    }

    dtNavMesh* MMapData::GetNavMesh(TerrainSet swaps)
    {
        for (uint32 swap : _activeSwaps)
        {
            if (swaps.find(swap) == swaps.end()) // swap not active
            {
                PhaseTileContainer ptc = MMAP::MMapFactory::createOrGetMMapManager()->GetPhaseTileContainer(swap);
                for (PhaseTileContainer::const_iterator itr = ptc.begin(); itr != ptc.end(); ++itr)
                {
                    RemoveSwap(itr->second, swap, itr->first); // remove swap
                }
            }
        }

        if (!swaps.empty())
        {
            // for each of the calling unit's terrain swaps
            for (uint32 swap : swaps)
            {
                // for each of the terrain swap's xy tiles
                PhaseTileContainer ptc = MMAP::MMapFactory::createOrGetMMapManager()->GetPhaseTileContainer(swap);
                for (PhaseTileContainer::const_iterator itr = ptc.begin(); itr != ptc.end(); ++itr)
                {
                    if (_activeSwaps.find(swap) == _activeSwaps.end()) // swap not active
                    {
                        AddSwap(itr->second, swap, itr->first); // add swap
                    }
                }
            }
        }
        return navMesh;
    }
}
