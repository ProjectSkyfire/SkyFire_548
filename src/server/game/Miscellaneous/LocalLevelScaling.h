/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*
* Per-character open world scaling: the core's side of it.
*
* A creature keeps its authored level and stats for everyone. A module (mod-destiny-weaver)
* installs resolvers here that describe one character's own version of a creature: the level
* and pools that character is shown, and the level and armor their fight is rolled against.
* Every resolver is empty until a module installs it, so without one the core behaves exactly
* as before.
*/

#ifndef SF_LOCAL_LEVEL_SCALING_H
#define SF_LOCAL_LEVEL_SCALING_H

#include "Define.h"
#include "SharedDefines.h"
#include "UpdateFields.h"

#include <algorithm>
#include <atomic>

class Creature;
class Player;

namespace LocalLevelScaling
{
    // Level a viewer meets this creature at; 0 = the creature's own level.
    using ViewLevelResolver = uint8(*)(Player const* viewer, Creature const* creature);
    // Armor the viewer's version wears; false = the creature's own armor.
    using ViewArmorResolver = bool(*)(Player const* viewer, Creature const* creature, float& armor);
    // Rewrites one values-update field for one recipient; false = unchanged.
    using ViewFieldResolver = bool(*)(Player const* viewer, Creature const* creature, uint16 index, uint32& value);
    // Whether this character's quests play at their own level.
    using QuestScalingResolver = bool(*)(Player const* player);

    inline std::atomic<ViewLevelResolver> ViewLevelOwner{ nullptr };
    inline std::atomic<ViewArmorResolver> ViewArmorOwner{ nullptr };
    inline std::atomic<ViewFieldResolver> ViewFieldOwner{ nullptr };
    inline std::atomic<QuestScalingResolver> QuestScalingOwner{ nullptr };

    // Share of a scaled quest's money / experience kept at the far end of the level range (%).
    inline std::atomic<uint32> QuestMoneyKeepSharePercent{ 100 };
    inline std::atomic<uint32> QuestXpKeepSharePercent{ 100 };

    inline uint8 ViewLevelFor(Player const* viewer, Creature const* creature)
    {
        ViewLevelResolver owner = ViewLevelOwner.load(std::memory_order_relaxed);
        return owner && viewer && creature ? owner(viewer, creature) : 0;
    }

    inline bool ViewArmorFor(Player const* viewer, Creature const* creature, float& armor)
    {
        ViewArmorResolver owner = ViewArmorOwner.load(std::memory_order_relaxed);
        return owner && viewer && creature && owner(viewer, creature, armor);
    }

    // The fields a view may rewrite: level, health and the power pools.
    inline bool IsViewField(uint16 index)
    {
        return index == UNIT_FIELD_LEVEL || index == UNIT_FIELD_HEALTH || index == UNIT_FIELD_MAX_HEALTH
            || (index >= UNIT_FIELD_POWER && index < UNIT_FIELD_POWER + MAX_POWERS_PER_CLASS)
            || (index >= UNIT_FIELD_MAX_POWER && index < UNIT_FIELD_MAX_POWER + MAX_POWERS_PER_CLASS);
    }

    inline bool PatchViewField(Player const* viewer, Creature const* creature, uint16 index, uint32& value)
    {
        ViewFieldResolver owner = ViewFieldOwner.load(std::memory_order_relaxed);
        return owner && viewer && creature && owner(viewer, creature, index, value);
    }

    inline bool QuestScalingEnabled(Player const* player)
    {
        QuestScalingResolver owner = QuestScalingOwner.load(std::memory_order_relaxed);
        return owner && player && owner(player);
    }

    // A scaled quest plays at the character's level, never below its own.
    inline int32 ScaleQuestLevel(int32 questLevel, uint8 playerLevel)
    {
        if (questLevel <= 0)
            return playerLevel;
        return std::max<int32>(questLevel, playerLevel);
    }

    // Share of the level-appropriate reward a lifted quest keeps: floorPercent for a quest from
    // the bottom of the range, rising to 100 for a quest at the character's own level.
    inline uint32 RewardKeepPercent(uint32 floorPercent, int32 questLevel, int32 effectiveLevel)
    {
        if (floorPercent >= 100 || questLevel <= 0 || effectiveLevel <= 0 || questLevel >= effectiveLevel)
            return 100;
        uint32 const share = uint32(questLevel) * 100 / uint32(effectiveLevel);
        return floorPercent + (100 - floorPercent) * share / 100;
    }
}

#endif
