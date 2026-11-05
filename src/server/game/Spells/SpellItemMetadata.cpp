/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "SpellItemMetadata.h"
#include "SharedDefines.h"

namespace Skyfire
{
namespace Spells
{
    namespace
    {
        uint32 const SPELL_DISARM_TRAP = 1842;
        uint32 const SPELL_MIND_NUMBING_POISON = 38615;
        uint32 const SPELL_GLOW_WORM = 64401;
        uint32 const SPELL_TEMP_ENCHANT_FIVE_MINUTES_1 = 29702;
        uint32 const SPELL_TEMP_ENCHANT_FIVE_MINUTES_2 = 37360;

        uint32 const SPELL_VISUAL_THIRTY_MINUTE_ENCHANT = 215;
        uint32 const SPELL_VISUAL_FISHING_POLE_BONUS = 563;
        uint32 const SPELL_VISUAL_SHAMAN_ROCKBITER = 0;

        uint32 const DURATION_FIVE_MINUTES = 300;
        uint32 const DURATION_TEN_MINUTES = 600;
        uint32 const DURATION_THIRTY_MINUTES = 1800;
        uint32 const DURATION_ONE_HOUR = 3600;
    }

    bool ShouldDeactivateOwnedTrapOnOpenLock(uint32 spellId, uint32 gameObjectType, bool hasOwner)
    {
        return spellId == SPELL_DISARM_TRAP && gameObjectType == GAMEOBJECT_TYPE_TRAP && hasOwner;
    }

    uint32 GetTemporaryItemEnchantDurationSeconds(uint32 spellId, uint32 spellFamilyName, uint32 spellVisual)
    {
        if (spellId == SPELL_MIND_NUMBING_POISON)
            return DURATION_THIRTY_MINUTES;

        if (spellFamilyName == SPELLFAMILY_ROGUE)
            return DURATION_ONE_HOUR;

        if (spellFamilyName == SPELLFAMILY_SHAMAN)
            return DURATION_ONE_HOUR;

        if (spellVisual == SPELL_VISUAL_THIRTY_MINUTE_ENCHANT)
            return DURATION_THIRTY_MINUTES;

        if (spellVisual == SPELL_VISUAL_FISHING_POLE_BONUS && spellId != SPELL_GLOW_WORM)
            return DURATION_TEN_MINUTES;

        if (spellVisual == SPELL_VISUAL_SHAMAN_ROCKBITER)
            return DURATION_THIRTY_MINUTES;

        if (spellId == SPELL_TEMP_ENCHANT_FIVE_MINUTES_1)
            return DURATION_FIVE_MINUTES;

        if (spellId == SPELL_TEMP_ENCHANT_FIVE_MINUTES_2)
            return DURATION_FIVE_MINUTES;

        return DURATION_ONE_HOUR;
    }
}
}
