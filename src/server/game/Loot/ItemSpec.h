/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_ITEM_SPEC_H
#define SKYFIRE_ITEM_SPEC_H

#include "Define.h"

#include <set>

struct ItemTemplate;

namespace Skyfire::ItemSpec
{
    // Specializations (ChrSpecialization.dbc ids) the item suits at this level: ItemSpecOverride.dbc
    // first, else worked out from ItemSpec.dbc as the client does. Empty means no spec restriction.
    std::set<uint32> GetItemSpecializations(ItemTemplate const* proto, uint32 level);
}

#endif
