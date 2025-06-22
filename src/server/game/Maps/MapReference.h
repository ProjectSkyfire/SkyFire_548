/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_MAPREFERENCE_H
#define SF_MAPREFERENCE_H

#include "Map.h"
#include "Reference.h"

class MapReference : public Reference<Map, Player>
{
protected:
    void targetObjectBuildLink()
    {
        // called from link()
        getTarget()->m_mapRefManager.insertFirst(this);
        getTarget()->m_mapRefManager.incSize();
    }
    void targetObjectDestroyLink()
    {
        // called from unlink()
        if (isValid()) getTarget()->m_mapRefManager.decSize();
    }
    void sourceObjectDestroyLink()
    {
        // called from invalidate()
        getTarget()->m_mapRefManager.decSize();
    }
public:
    MapReference() : Reference<Map, Player>() { }
    ~MapReference() { unlink(); }
    MapReference* next() { return (MapReference*)Reference<Map, Player>::next(); }
    MapReference const* next() const { return (MapReference const*)Reference<Map, Player>::next(); }
    MapReference* nockeck_prev() { return (MapReference*)Reference<Map, Player>::nocheck_prev(); }
    MapReference const* nocheck_prev() const { return (MapReference const*)Reference<Map, Player>::nocheck_prev(); }
};
#endif
