/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_GRIDSTATES_H
#define SKYFIRE_GRIDSTATES_H

#include "GridDefines.h"
#include "NGrid.h"

class Map;

class GridState
{
    public:
#ifdef SKYFIRE_DEBUG
#define MAGIC_TESTVAL 0xFBE823BA
        GridState() { i_Magic = MAGIC_TESTVAL; }
        bool checkMagic();
        void setMagic() { i_Magic = MAGIC_TESTVAL; }
        unsigned int i_Magic;
#endif
        virtual ~GridState() { };
        virtual void Update(Map &, NGridType&, GridInfo &, uint32 t_diff) const = 0;
};

class InvalidState : public GridState
{
    public:
        void Update(Map &, NGridType &, GridInfo &, uint32 t_diff) const;
};

class ActiveState : public GridState
{
    public:
        void Update(Map &, NGridType &, GridInfo &, uint32 t_diff) const;
};

class IdleState : public GridState
{
    public:
        void Update(Map &, NGridType &, GridInfo &, uint32 t_diff) const;
};

class RemovalState : public GridState
{
    public:
        void Update(Map &, NGridType &, GridInfo &, uint32 t_diff) const;
};
#endif
