/*
 * Copyright (C) 2011-2014 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2014 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WMODEL_HNDL_H
#define WMODEL_HNDL_H
#include "Define.h"
#include "Utils.h"
#include "WorldModelRoot.h"
#include "ObjectDataHandler.h"

#include <set>
#include <vector>

class ADT;

struct WorldModelDefinition : public IDefinition
{
public:
    WorldModelDefinition() {}

    uint32 MwidIndex;
    uint32 UniqueId;
    Vector3 UpperExtents;
    Vector3 LowerExtents;
    uint16 Flags;
    uint16 DoodadSet;

    static WorldModelDefinition Read(FILE* file);
};

class WorldModelHandler : public ObjectDataHandler
{
public:
    WorldModelHandler(ADT* adt);
    ~WorldModelHandler();

    std::vector<Vector3> Vertices;
    std::vector<Triangle<uint32> > Triangles;
    bool IsSane() { return _definitions && _paths; }
    void InsertModelGeometry(std::vector<Vector3>& verts, std::vector<Triangle<uint32> >& tris, WorldModelDefinition& def, WorldModelRoot* root);
protected:
    void ProcessInternal(ChunkedData* data);
private:
    void ReadDefinitions();
    void ReadModelPaths();
    std::set<uint32> _drawn;
    std::vector<WorldModelDefinition>* _definitions;
    std::vector<std::string>* _paths;
};
#endif