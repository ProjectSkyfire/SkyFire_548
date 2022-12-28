/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef _INTERMEDIATE_VALUES_H
#define _INTERMEDIATE_VALUES_H

#include "PathCommon.h"
#include "TerrainBuilder.h"
#include "Recast.h"
#include "DetourNavMesh.h"

namespace MMAP
{
    // this class gathers all debug info holding and output
    struct IntermediateValues
    {
        rcHeightfield* heightfield;
        rcCompactHeightfield* compactHeightfield;
        rcContourSet* contours;
        rcPolyMesh* polyMesh;
        rcPolyMeshDetail* polyMeshDetail;

        IntermediateValues() :  heightfield(NULL), compactHeightfield(NULL),
                                contours(NULL), polyMesh(NULL), polyMeshDetail(NULL) {}
        ~IntermediateValues();

        void writeIV(uint32 mapID, uint32 tileX, uint32 tileY);

        void debugWrite(FILE* file, const rcHeightfield* mesh);
        void debugWrite(FILE* file, const rcCompactHeightfield* chf);
        void debugWrite(FILE* file, const rcContourSet* cs);
        void debugWrite(FILE* file, const rcPolyMesh* mesh);
        void debugWrite(FILE* file, const rcPolyMeshDetail* mesh);

        void generateObjFile(uint32 mapID, uint32 tileX, uint32 tileY, MeshData &meshData);
    };
}
#endif
