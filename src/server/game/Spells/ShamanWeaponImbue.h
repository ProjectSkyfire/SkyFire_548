/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_SHAMAN_WEAPON_IMBUE_H
#define SF_SHAMAN_WEAPON_IMBUE_H

#include "Define.h"

namespace Skyfire
{
namespace Spells
{
    inline bool IsSelfCastShamanWeaponImbue(uint32 spellId)
    {
        switch (spellId)
        {
            case 8017:  // Rockbiter Weapon
            case 8024:  // Flametongue Weapon
            case 8033:  // Frostbrand Weapon
            case 8232:  // Windfury Weapon
            case 51730: // Earthliving Weapon
                return true;
            default:
                return false;
        }
    }

    inline uint8 GetSelfCastShamanWeaponImbueEquipmentSlot()
    {
        return 15; // EQUIPMENT_SLOT_MAINHAND
    }
}
}

#endif
