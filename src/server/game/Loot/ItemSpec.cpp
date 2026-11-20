/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ItemSpec.h"
#include "ItemSpecRules.h"

#include "DBCStores.h"
#include "ItemPrototype.h"

#include <map>
#include <vector>

using namespace Skyfire::ItemSpec;

namespace
{
    // Built on first use, after the DBC stores are loaded; function-local statics initialise once across map threads.
    std::vector<ItemSpecRow> const& GetItemSpecRows()
    {
        static std::vector<ItemSpecRow> const rows = []
        {
            std::vector<ItemSpecRow> list;
            for (uint32 i = 0; i < sItemSpecStore.GetNumRows(); ++i)
                if (ItemSpecEntry const* entry = sItemSpecStore.LookupEntry(i))
                    list.push_back({ entry->MinLevel, entry->MaxLevel, entry->ItemType, entry->PrimaryStat, entry->SecondaryStat, entry->SpecializationId });
            return list;
        }();
        return rows;
    }

    std::multimap<uint32, uint32> const& GetItemSpecOverrides()
    {
        static std::multimap<uint32, uint32> const overrides = []
        {
            std::multimap<uint32, uint32> map;
            for (uint32 i = 0; i < sItemSpecOverrideStore.GetNumRows(); ++i)
                if (ItemSpecOverrideEntry const* entry = sItemSpecOverrideStore.LookupEntry(i))
                    map.emplace(entry->ItemId, entry->SpecializationId);
            return map;
        }();
        return overrides;
    }

    uint32 GetStatForWeapon(uint32 subClass)
    {
        switch (subClass)
        {
            case ITEM_SUBCLASS_WEAPON_AXE:          return ITEM_SPEC_STAT_ONE_HANDED_AXE;
            case ITEM_SUBCLASS_WEAPON_AXE2:         return ITEM_SPEC_STAT_TWO_HANDED_AXE;
            case ITEM_SUBCLASS_WEAPON_SWORD:        return ITEM_SPEC_STAT_ONE_HANDED_SWORD;
            case ITEM_SUBCLASS_WEAPON_SWORD2:       return ITEM_SPEC_STAT_TWO_HANDED_SWORD;
            case ITEM_SUBCLASS_WEAPON_MACE:         return ITEM_SPEC_STAT_ONE_HANDED_MACE;
            case ITEM_SUBCLASS_WEAPON_MACE2:        return ITEM_SPEC_STAT_TWO_HANDED_MACE;
            case ITEM_SUBCLASS_WEAPON_DAGGER:       return ITEM_SPEC_STAT_DAGGER;
            case ITEM_SUBCLASS_WEAPON_FIST_WEAPON:  return ITEM_SPEC_STAT_FIST_WEAPON;
            case ITEM_SUBCLASS_WEAPON_GUN:          return ITEM_SPEC_STAT_GUN;
            case ITEM_SUBCLASS_WEAPON_BOW:          return ITEM_SPEC_STAT_BOW;
            case ITEM_SUBCLASS_WEAPON_CROSSBOW:     return ITEM_SPEC_STAT_CROSSBOW;
            case ITEM_SUBCLASS_WEAPON_STAFF:        return ITEM_SPEC_STAT_STAFF;
            case ITEM_SUBCLASS_WEAPON_POLEARM:      return ITEM_SPEC_STAT_POLEARM;
            case ITEM_SUBCLASS_WEAPON_THROWN:       return ITEM_SPEC_STAT_THROWN;
            case ITEM_SUBCLASS_WEAPON_WAND:         return ITEM_SPEC_STAT_WAND;
            default:                                return ITEM_SPEC_STAT_NONE;
        }
    }

    // Mastery is deliberately absent: the client's ItemSpec.dbc has no value for it (TEST's 596 rows use
    // primary 0-2 and secondary 3-25, 26 = none), so no row can pair a primary stat with Mastery.
    uint32 GetStatForItemStat(int32 itemStat)
    {
        switch (itemStat)
        {
            case ITEM_MOD_INTELLECT:            return ITEM_SPEC_STAT_INTELLECT;
            case ITEM_MOD_AGILITY:              return ITEM_SPEC_STAT_AGILITY;
            case ITEM_MOD_STRENGTH:             return ITEM_SPEC_STAT_STRENGTH;
            case ITEM_MOD_SPIRIT:               return ITEM_SPEC_STAT_SPIRIT;
            case ITEM_MOD_HIT_RATING:           return ITEM_SPEC_STAT_HIT;
            case ITEM_MOD_DODGE_RATING:         return ITEM_SPEC_STAT_DODGE;
            case ITEM_MOD_PARRY_RATING:         return ITEM_SPEC_STAT_PARRY;
            case ITEM_MOD_CRIT_MELEE_RATING:
            case ITEM_MOD_CRIT_RANGED_RATING:
            case ITEM_MOD_CRIT_RATING:
            case ITEM_MOD_CRIT_SPELL_RATING:    return ITEM_SPEC_STAT_CRIT;
            case ITEM_MOD_HASTE_RATING:         return ITEM_SPEC_STAT_HASTE;
            default:                            return ITEM_SPEC_STAT_NONE;
        }
    }

    ItemSpecProfile BuildProfile(ItemTemplate const* proto)
    {
        ItemSpecProfile profile;
        if (proto->Class == ITEM_CLASS_WEAPON)
        {
            profile.Type = ITEM_SPEC_TYPE_WEAPON;
            profile.AddStat(GetStatForWeapon(proto->SubClass), false);
        }
        else if (proto->Class == ITEM_CLASS_ARMOR)
        {
            switch (proto->SubClass)
            {
                case ITEM_SUBCLASS_ARMOR_CLOTH:
                    if (proto->InventoryType != INVTYPE_CLOAK)  // cloaks are cloth for every class
                        profile.Type = ITEM_SPEC_TYPE_CLOTH;
                    break;
                case ITEM_SUBCLASS_ARMOR_LEATHER:   profile.Type = ITEM_SPEC_TYPE_LEATHER; break;
                case ITEM_SUBCLASS_ARMOR_MAIL:      profile.Type = ITEM_SPEC_TYPE_MAIL; break;
                case ITEM_SUBCLASS_ARMOR_PLATE:     profile.Type = ITEM_SPEC_TYPE_PLATE; break;
                case ITEM_SUBCLASS_ARMOR_SHIELD:    profile.AddStat(ITEM_SPEC_STAT_SHIELD, false); break;
                default:
                    if (proto->SubClass > ITEM_SUBCLASS_ARMOR_SHIELD && proto->SubClass <= ITEM_SUBCLASS_ARMOR_RELIC)
                    {
                        profile.Type = ITEM_SPEC_TYPE_RELIC;
                        profile.AddStat(ITEM_SPEC_STAT_RELIC, false);
                    }
                    break;
            }
        }

        ScalingStatDistributionEntry const* scaling = proto->ScalingStatDistribution
            ? sScalingStatDistributionStore.LookupEntry(proto->ScalingStatDistribution) : nullptr;
        for (uint32 i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
        {
            if (proto->ScalingStatDistribution)
            {
                if (scaling && scaling->Modifier[i])
                    profile.AddStat(GetStatForItemStat(scaling->StatMod[i]), true);
            }
            else
                profile.AddStat(GetStatForItemStat(int32(proto->ItemStat[i].ItemStatType)), true);
        }

        return profile;
    }
}

std::set<uint32> Skyfire::ItemSpec::GetItemSpecializations(ItemTemplate const* proto, uint32 level)
{
    std::set<uint32> specs;
    if (!proto)
        return specs;

    auto bounds = GetItemSpecOverrides().equal_range(proto->ItemId);
    for (auto itr = bounds.first; itr != bounds.second; ++itr)
        specs.insert(itr->second);

    if (specs.empty())
        specs = MatchSpecializations(BuildProfile(proto), level, GetItemSpecRows());

    return specs;
}
