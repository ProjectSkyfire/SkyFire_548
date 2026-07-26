/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_AREATABLEUTILS_H
#define SKYFIRE_AREATABLEUTILS_H

#include "Define.h"

namespace Skyfire::AreaTable
{
    template<class ParentLookup>
    uint32 ResolveRootZoneId(uint32 areaId, ParentLookup parentLookup)
    {
        uint32 rootId = areaId;

        for (uint8 depth = 0; depth < 32; ++depth)
        {
            uint32 parentId = parentLookup(rootId);
            if (!parentId || parentId == rootId)
                return rootId;

            rootId = parentId;
        }

        return rootId;
    }

    uint32 GetRootZoneId(uint32 areaId);
}

#endif
