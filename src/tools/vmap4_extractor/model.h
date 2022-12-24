/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef MODEL_H
#define MODEL_H

#include "vec3d.h"
#include "modelheaders.h"
#include <vector>

class MPQFile;

Vec3D fixCoordSystem(Vec3D v);

class Model
{
private:
    void _unload()
    {
        delete[] vertices;
        delete[] indices;
        vertices = NULL;
        indices = NULL;
    }
    std::string filename;
public:
    ModelHeader header;
    Vec3D* vertices;
    uint16* indices;

    bool open();
    bool ConvertToVMAPModel(char const* outfilename);

    Model(std::string& filename);
    ~Model() { _unload(); }
};

class ModelInstance
{
public:
    uint32 id;
    Vec3D pos, rot;
    uint16 scale, flags;
    float sc;

    ModelInstance() : id(0), scale(0), flags(0), sc(0.0f) {}
    ModelInstance(MPQFile& f, char const* ModelInstName, uint32 mapID, uint32 tileX, uint32 tileY, FILE* pDirfile);

};

#endif
