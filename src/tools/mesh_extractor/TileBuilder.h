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
 
#ifndef TILE_BUILD_H
#define TILE_BUILD_H
#include <string>
#include "Recast.h"

#include "Geometry.h"

class ContinentBuilder;
class WDT;

class TileBuilder
{
public:
    TileBuilder(ContinentBuilder* _cBuilder, std::string world, int x, int y, uint32 mapId);
    ~TileBuilder();

    void CalculateTileBounds(float*& bmin, float*& bmax, dtNavMeshParams& navMeshParams);
    uint8* Build(bool dbg, dtNavMeshParams& navMeshParams);

    std::string World;
    int X;
    int Y;
    int MapId;
    rcConfig Config;
    rcContext* Context;
    Geometry* _Geometry;
    uint32 DataSize;
    ContinentBuilder* cBuilder;
};
#endif