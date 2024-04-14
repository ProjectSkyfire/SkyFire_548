/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_GRIDREFMANAGER
#define SF_GRIDREFMANAGER

#include "RefManager.h"

template<class OBJECT>
class GridReference;

template<class OBJECT>
class GridRefManager : public RefManager<GridRefManager<OBJECT>, OBJECT>
{
public:
    typedef LinkedListHead::Iterator< GridReference<OBJECT> > iterator;

    GridReference<OBJECT>* getFirst() { return (GridReference<OBJECT>*)RefManager<GridRefManager<OBJECT>, OBJECT>::getFirst(); }
    GridReference<OBJECT>* getLast() { return (GridReference<OBJECT>*)RefManager<GridRefManager<OBJECT>, OBJECT>::getLast(); }

    iterator begin() { return iterator(getFirst()); }
    iterator end() { return iterator(NULL); }
    iterator rbegin() { return iterator(getLast()); }
    iterator rend() { return iterator(NULL); }
};
#endif
