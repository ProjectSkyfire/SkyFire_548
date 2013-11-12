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

#ifndef CONT_BUILDER_H
#define CONT_BUILDER_H
#include <string>
#include "WDT.h"
#include "Define.h"

class ContinentBuilder
{
public:
    ContinentBuilder(std::string continent, uint32 mapId, WDT* wdt, uint32 tn) :
        Continent(continent), TileMap(wdt), MapId(mapId),
        NumberOfThreads(tn), tileXMin(64), tileYMin(64), tileXMax(0), tileYMax(0)
        {}

    void Build(bool debug);
    void getTileBounds(uint32 tileX, uint32 tileY, float* verts, int vertCount, float* bmin, float* bmax);
    void CalculateTileBounds();
    float bmin[3];
    float bmax[3];
private:
    std::string Continent;
    WDT* TileMap;
    uint32 MapId;
    uint32 NumberOfThreads;
    int tileXMin;
    int tileYMin;
    int tileXMax;
    int tileYMax;
};
#endif
