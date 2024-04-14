/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#ifndef SF_GRIDREFERENCE_H
#define SF_GRIDREFERENCE_H

#include "LinkedReference/Reference.h"

template<class OBJECT>
class GridRefManager;

template<class OBJECT>
class GridReference : public Reference<GridRefManager<OBJECT>, OBJECT>
{
protected:
    void targetObjectBuildLink()
    {
        // called from link()
        this->getTarget()->insertFirst(this);
        this->getTarget()->incSize();
    }
    void targetObjectDestroyLink()
    {
        // called from unlink()
        if (this->isValid()) this->getTarget()->decSize();
    }
    void sourceObjectDestroyLink()
    {
        // called from invalidate()
        this->getTarget()->decSize();
    }
public:
    GridReference() : Reference<GridRefManager<OBJECT>, OBJECT>() { }
    ~GridReference() { this->unlink(); }
    GridReference* next() { return (GridReference*)Reference<GridRefManager<OBJECT>, OBJECT>::next(); }
};
#endif
