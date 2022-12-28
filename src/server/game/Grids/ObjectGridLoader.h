/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_OBJECTGRIDLOADER_H
#define SKYFIRE_OBJECTGRIDLOADER_H

#include "TypeList.h"
#include "Define.h"
#include "GridLoader.h"
#include "GridDefines.h"
#include "Cell.h"

class ObjectWorldLoader;

class ObjectGridLoader
{
    friend class ObjectWorldLoader;

    public:
        ObjectGridLoader(NGridType &grid, Map* map, const Cell &cell)
            : i_cell(cell), i_grid(grid), i_map(map), i_gameObjects(0), i_creatures(0), i_corpses (0)
            { }

        void Visit(GameObjectMapType &m);
        void Visit(CreatureMapType &m);
        void Visit(CorpseMapType &) const { }
        void Visit(DynamicObjectMapType&) const { }
        void Visit(AreaTriggerMapType &) const { }

        void LoadN(void);

        template<class T> static void SetObjectCell(T* obj, CellCoord const& cellCoord);

    private:
        Cell i_cell;
        NGridType &i_grid;
        Map* i_map;
        uint32 i_gameObjects;
        uint32 i_creatures;
        uint32 i_corpses;
};

//Stop the creatures before unloading the NGrid
class ObjectGridStoper
{
    public:
        void Visit(CreatureMapType &m);
        template<class T> void Visit(GridRefManager<T> &) { }
};

//Move the foreign creatures back to respawn positions before unloading the NGrid
class ObjectGridEvacuator
{
    public:
        void Visit(CreatureMapType &m);
        void Visit(GameObjectMapType &m);
        template<class T> void Visit(GridRefManager<T> &) { }
};

//Clean up and remove from world
class ObjectGridCleaner
{
    public:
        template<class T> void Visit(GridRefManager<T> &);
};

//Delete objects before deleting NGrid
class ObjectGridUnloader
{
    public:
        template<class T> void Visit(GridRefManager<T> &m);
};
#endif
