/*
 * Copyright (C) 2005-2013 MaNGOS <http://www.getmangos.com/>
 * Copyright (C) 2008-2013 Trinity <http://www.trinitycore.org/>
 * Copyright (C) 2011-2013 Project SkyFire <http://www.projectskyfire.org/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include "Utils.h"

class Model
{
public:
    Model(std::string path);
    ~Model();

    void ReadVertices(FILE* stream);
    void ReadBoundingTriangles(FILE* stream);
    void ReadBoundingNormals(FILE* stream);
    ModelHeader Header;
    std::vector<Vector3> Vertices;
    std::vector<Vector3> Normals;
    std::vector<Triangle<uint16> > Triangles;
    bool IsCollidable;
    FILE* Stream;
    bool IsBad;
};
#endif