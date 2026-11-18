/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_ITEM_SPEC_RULES_H
#define SKYFIRE_ITEM_SPEC_RULES_H

#include <algorithm>
#include <cstdint>
#include <set>
#include <vector>

// Which specializations an item suits, as the 5.4.8 client works it out from ItemSpec.dbc.
// Kept free of game headers so unit tests can include it.
namespace Skyfire::ItemSpec
{
    // ItemSpec.dbc column ItemType
    enum ItemSpecType : std::uint32_t
    {
        ITEM_SPEC_TYPE_MISC    = 0,
        ITEM_SPEC_TYPE_CLOTH   = 1,
        ITEM_SPEC_TYPE_LEATHER = 2,
        ITEM_SPEC_TYPE_MAIL    = 3,
        ITEM_SPEC_TYPE_PLATE   = 4,
        ITEM_SPEC_TYPE_WEAPON  = 5,
        ITEM_SPEC_TYPE_RELIC   = 6,
    };

    // ItemSpec.dbc columns PrimaryStat and SecondaryStat
    enum ItemSpecStat : std::uint32_t
    {
        ITEM_SPEC_STAT_INTELLECT        = 0,
        ITEM_SPEC_STAT_AGILITY          = 1,
        ITEM_SPEC_STAT_STRENGTH         = 2,
        ITEM_SPEC_STAT_SPIRIT           = 3,
        ITEM_SPEC_STAT_HIT              = 4,
        ITEM_SPEC_STAT_DODGE            = 5,
        ITEM_SPEC_STAT_PARRY            = 6,
        ITEM_SPEC_STAT_ONE_HANDED_AXE   = 7,
        ITEM_SPEC_STAT_TWO_HANDED_AXE   = 8,
        ITEM_SPEC_STAT_ONE_HANDED_SWORD = 9,
        ITEM_SPEC_STAT_TWO_HANDED_SWORD = 10,
        ITEM_SPEC_STAT_ONE_HANDED_MACE  = 11,
        ITEM_SPEC_STAT_TWO_HANDED_MACE  = 12,
        ITEM_SPEC_STAT_DAGGER           = 13,
        ITEM_SPEC_STAT_FIST_WEAPON      = 14,
        ITEM_SPEC_STAT_GUN              = 15,
        ITEM_SPEC_STAT_BOW              = 16,
        ITEM_SPEC_STAT_CROSSBOW         = 17,
        ITEM_SPEC_STAT_STAFF            = 18,
        ITEM_SPEC_STAT_POLEARM          = 19,
        ITEM_SPEC_STAT_THROWN           = 20,
        ITEM_SPEC_STAT_WAND             = 21,
        ITEM_SPEC_STAT_SHIELD           = 22,
        ITEM_SPEC_STAT_RELIC            = 23,
        ITEM_SPEC_STAT_CRIT             = 24,
        ITEM_SPEC_STAT_HASTE            = 25,
        ITEM_SPEC_STAT_NONE             = 26,
    };

    struct ItemSpecRow
    {
        std::uint32_t MinLevel;
        std::uint32_t MaxLevel;
        std::uint32_t ItemType;
        std::uint32_t PrimaryStat;
        std::uint32_t SecondaryStat;        // ITEM_SPEC_STAT_NONE = the primary stat alone decides
        std::uint32_t SpecializationId;
    };

    // What the client reads off an item: its spec type, its weapon or shield kind and its stats.
    struct ItemSpecProfile
    {
        std::uint32_t Type = ITEM_SPEC_TYPE_MISC;
        std::vector<std::uint32_t> Stats;
        bool HasItemStat = false;           // at least one stat came from the item's stat list

        void AddStat(std::uint32_t stat, bool fromItemStats)
        {
            if (stat >= ITEM_SPEC_STAT_NONE)
                return;

            if (std::find(Stats.begin(), Stats.end(), stat) == Stats.end())
                Stats.push_back(stat);

            if (fromItemStats)
                HasItemStat = true;
        }

        bool Has(std::uint32_t stat) const { return std::find(Stats.begin(), Stats.end(), stat) != Stats.end(); }
    };

    // Specializations the item suits at this level. Empty means no spec restriction.
    // Rows must be in ItemSpec.dbc order: the rows of one item type are contiguous there.
    inline std::set<std::uint32_t> MatchSpecializations(ItemSpecProfile const& profile, std::uint32_t level, std::vector<ItemSpecRow> const& rows)
    {
        std::set<std::uint32_t> specs;
        if (!profile.HasItemStat || profile.Stats.empty())
            return specs;

        // A primary stat matched with one of the item's secondary stats narrows the item to those
        // specs, whatever the row order: its primary-only rows count only when no pairing matched.
        std::set<std::uint32_t> pairedPrimaries;
        std::vector<ItemSpecRow const*> primaryOnly;
        bool typeSeen = false;
        for (ItemSpecRow const& row : rows)
        {
            if (level < row.MinLevel || level > row.MaxLevel)
                continue;

            if (row.ItemType != profile.Type)
            {
                if (typeSeen)
                    break;
                continue;
            }

            typeSeen = true;
            if (!profile.Has(row.PrimaryStat))
                continue;

            if (row.SecondaryStat == ITEM_SPEC_STAT_NONE)
                primaryOnly.push_back(&row);
            else if (profile.Has(row.SecondaryStat))
            {
                specs.insert(row.SpecializationId);
                pairedPrimaries.insert(row.PrimaryStat);
            }
        }

        for (ItemSpecRow const* row : primaryOnly)
            if (!pairedPrimaries.count(row->PrimaryStat))
                specs.insert(row->SpecializationId);

        return specs;
    }
}

#endif
