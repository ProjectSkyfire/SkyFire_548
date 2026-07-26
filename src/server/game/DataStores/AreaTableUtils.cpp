/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "AreaTableUtils.h"
#include "DBCStores.h"

namespace Skyfire::AreaTable
{
    uint32 GetRootZoneId(uint32 areaId)
    {
        return ResolveRootZoneId(areaId, [](uint32 currentAreaId) -> uint32
        {
            if (AreaTableEntry const* areaEntry = GetAreaEntryByAreaID(currentAreaId))
                return areaEntry->m_ParentAreaID;

            return 0;
        });
    }
}
