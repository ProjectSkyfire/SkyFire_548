/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "vmapexport.h"
#include "model.h"
#include "wmo.h"
#include "mpqfile.h"
#include <cassert>
#include <algorithm>
#include <cstdio>
#include <cstring>

extern HANDLE WorldMpq;

namespace
{
    bool RangeInFile(uint32 offset, uint32 bytes, size_t fileSize)
    {
        return uint64(offset) + bytes <= fileSize;
    }

    bool LoadBoundingCollision(MPQFile& f, uint32 m2start, ModelHeader const& header, Vec3D*& vertices, uint16*& indices, uint32& nVertices, uint32& nIndices)
    {
        if (!header.nBoundingTriangles || !header.nBoundingVertices)
            return false;
        if (!RangeInFile(m2start + header.ofsBoundingVertices, header.nBoundingVertices * 12, f.getSize()))
            return false;
        if (!RangeInFile(m2start + header.ofsBoundingTriangles, header.nBoundingTriangles * 2, f.getSize()))
            return false;

        f.seek(m2start);
        f.seekRelative(header.ofsBoundingVertices);
        vertices = new Vec3D[header.nBoundingVertices];
        f.read(vertices, header.nBoundingVertices * 12);
        for (uint32 i = 0; i < header.nBoundingVertices; ++i)
            vertices[i] = fixCoordSystem(vertices[i]);

        f.seek(m2start);
        f.seekRelative(header.ofsBoundingTriangles);
        indices = new uint16[header.nBoundingTriangles];
        f.read(indices, header.nBoundingTriangles * 2);
        nVertices = header.nBoundingVertices;
        nIndices = header.nBoundingTriangles;
        return true;
    }

}

Model::Model(std::string &filename) : filename(filename), vertices(0), indices(0), nVertices(0), nIndices(0)
{
    memset(&header, 0, sizeof(header));
}

bool Model::open()
{
    MPQFile f(WorldMpq, filename.c_str());

    if (f.isEof())
    {
        f.close();
        return false;
    }

    _unload();
    nVertices = 0;
    nIndices = 0;

    // MoP wraps MD20 in an MD21 chunk. Offsets are relative to the MD20 header.
    uint32 m2start = 0;
    char const* ptr = f.getBuffer();
    while (m2start + 4 < f.getSize() && memcmp(ptr, "MD20", 4) != 0)
    {
        ++m2start;
        ++ptr;
        if (m2start + sizeof(ModelHeader) > f.getSize())
        {
            f.close();
            return false;
        }
    }

    memcpy(&header, f.getBuffer() + m2start, sizeof(ModelHeader));

    // Only the bounding volume is real collision. Models without one are walked
    // through by the client, so giving them geometry desyncs server and client.
    bool const boundingOk = LoadBoundingCollision(f, m2start, header, vertices, indices, nVertices, nIndices);

    f.close();
    return boundingOk;
}

bool Model::ConvertToVMAPModel(const char * outfilename)
{
    int N[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
    FILE* output=fopen(outfilename, "wb");
    if (!output)
    {
        printf("Can't create the output file '%s'\n",outfilename);
        return false;
    }
    fwrite(szRawVMAPMagic, 8, 1, output);
    fwrite(&nVertices, sizeof(int), 1, output);
    uint32 nofgroups = 1;
    fwrite(&nofgroups,sizeof(uint32), 1, output);
    fwrite(N,4*3,1,output);// rootwmoid, flags, groupid
    fwrite(N,sizeof(float),3*2,output);//bbox, only needed for WMO currently
    fwrite(N,4,1,output);// liquidflags
    fwrite("GRP ",4,1,output);
    uint32 branches = 1;
    int wsize;
    wsize = sizeof(branches) + sizeof(uint32) * branches;
    fwrite(&wsize, sizeof(int), 1, output);
    fwrite(&branches,sizeof(branches), 1, output);
    fwrite(&nIndices,sizeof(uint32), 1, output);
    fwrite("INDX",4, 1, output);
    wsize = sizeof(uint32) + sizeof(unsigned short) * nIndices;
    fwrite(&wsize, sizeof(int), 1, output);
    fwrite(&nIndices, sizeof(uint32), 1, output);
    if (nIndices > 0)
    {
        for (uint32 i = 0; i < nIndices; ++i)
        {
            if ((i % 3) - 1 == 0 && i + 1 < nIndices)
            {
                uint16 tmp = indices[i];
                indices[i] = indices[i + 1];
                indices[i + 1] = tmp;
            }
        }
        fwrite(indices, sizeof(unsigned short), nIndices, output);
    }

    fwrite("VERT", 4, 1, output);
    wsize = sizeof(int) + sizeof(float) * 3 * nVertices;
    fwrite(&wsize, sizeof(int), 1, output);
    fwrite(&nVertices, sizeof(int), 1, output);
    if (nVertices >0)
    {
        for (uint32 vpos = 0; vpos < nVertices; ++vpos)
        {
            float tmp = vertices[vpos].y;
            vertices[vpos].y = -vertices[vpos].z;
            vertices[vpos].z = tmp;
        }

        fwrite(vertices, sizeof(float)*3, nVertices, output);
    }

    fclose(output);

    return true;
}


Vec3D fixCoordSystem(Vec3D v)
{
    return Vec3D(v.x, v.z, -v.y);
}

Vec3D fixCoordSystem2(Vec3D v)
{
    return Vec3D(v.x, v.z, v.y);
}

ModelInstance::ModelInstance(MPQFile& f, char const* ModelInstName, uint32 mapID, uint32 tileX, uint32 tileY, FILE *pDirfile) : id(0), scale(0), flags(0)
{
    float ff[3];
    f.read(&id, 4);
    f.read(ff, 12);
    pos = fixCoords(Vec3D(ff[0], ff[1], ff[2]));
    f.read(ff, 12);
    rot = Vec3D(ff[0], ff[1], ff[2]);
    f.read(&scale, 2);
    f.read(&flags, 2);
    // scale factor - divide by 1024. blizzard devs must be on crack, why not just use a float?
    sc = scale / 1024.0f;

    char tempname[512];
    snprintf(tempname, sizeof(tempname), "%s/%s", szWorkDirWmo, ModelInstName);
    FILE* input = fopen(tempname, "r+b");

    if (!input)
    {
        //printf("ModelInstance::ModelInstance couldn't open %s\n", tempname);
        return;
    }

    fseek(input, 8, SEEK_SET); // get the correct no of vertices
    int nVertices;
    int count = fread(&nVertices, sizeof (int), 1, input);
    fclose(input);

    if (count != 1 || nVertices == 0)
        return;

    uint16 adtId = 0;// not used for models
    uint32 flags = MOD_M2;
    if (tileX == 65 && tileY == 65)
        flags |= MOD_WORLDSPAWN;

    //write mapID, tileX, tileY, Flags, ID, Pos, Rot, Scale, name
    fwrite(&mapID, sizeof(uint32), 1, pDirfile);
    fwrite(&tileX, sizeof(uint32), 1, pDirfile);
    fwrite(&tileY, sizeof(uint32), 1, pDirfile);
    fwrite(&flags, sizeof(uint32), 1, pDirfile);
    fwrite(&adtId, sizeof(uint16), 1, pDirfile);
    fwrite(&id, sizeof(uint32), 1, pDirfile);
    fwrite(&pos, sizeof(float), 3, pDirfile);
    fwrite(&rot, sizeof(float), 3, pDirfile);
    fwrite(&sc, sizeof(float), 1, pDirfile);
    uint32 nlen=strlen(ModelInstName);
    fwrite(&nlen, sizeof(uint32), 1, pDirfile);
    fwrite(ModelInstName, sizeof(char), nlen, pDirfile);

    /* int realx1 = (int) ((float) pos.x / 533.333333f);
    int realy1 = (int) ((float) pos.z / 533.333333f);
    int realx2 = (int) ((float) pos.x / 533.333333f);
    int realy2 = (int) ((float) pos.z / 533.333333f);

    fprintf(pDirfile,"%s/%s %f,%f,%f_%f,%f,%f %f %d %d %d,%d %d\n",
        MapName,
        ModelInstName,
        (float) pos.x, (float) pos.y, (float) pos.z,
        (float) rot.x, (float) rot.y, (float) rot.z,
        sc,
        nVertices,
        realx1, realy1,
        realx2, realy2
        ); */
}
