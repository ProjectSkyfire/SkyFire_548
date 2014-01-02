/*
 * Copyright (C) 2011-2014 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2014 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TRINITY_DB2STRUCTURE_H
#define TRINITY_DB2STRUCTURE_H

#include "Common.h"
#include "ItemPrototype.h"

// GCC has alternative #pragma pack(N) syntax and old gcc version does not support pack(push, N), also any gcc version does not support it at some platform
#if defined(__GNUC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

// Structures used to access raw DB2 data and required packing to portability
struct ItemEntry
{
    uint32   ID;                                             // 0
    uint32   Class;                                          // 1
    uint32   SubClass;                                       // 2
    int32    SoundOverrideSubclass;                          // 3
    int32    Material;                                       // 4
    uint32   DisplayId;                                      // 5
    uint32   InventoryType;                                  // 6
    uint32   Sheath;                                         // 7
};

struct ItemCurrencyCostEntry
{
    //uint32  Id;
    uint32  ItemId;
};

struct ItemSparseEntry
{
    uint32     ID;                                           // 0
    uint32     Quality;                                      // 1
    uint32     Flags;                                        // 2
    uint32     Flags2;                                       // 3
    uint32     Unk540_1;                                     // 4 unk flag
    float      Unk430_1;                                     // 5
    float      Unk430_2;                                     // 6
    uint32     BuyCount;                                     // 7
    uint32     BuyPrice;                                     // 8
    uint32     SellPrice;                                    // 9
    uint32     InventoryType;                                // 10
    int32      AllowableClass;                               // 11
    int32      AllowableRace;                                // 12
    uint32     ItemLevel;                                    // 13
    int32      RequiredLevel;                                // 14
    uint32     RequiredSkill;                                // 15
    uint32     RequiredSkillRank;                            // 16
    uint32     RequiredSpell;                                // 17
    uint32     RequiredHonorRank;                            // 18
    uint32     RequiredCityRank;                             // 19
    uint32     RequiredReputationFaction;                    // 20
    uint32     RequiredReputationRank;                       // 21
    uint32     MaxCount;                                     // 22
    uint32     Stackable;                                    // 23
    uint32     ContainerSlots;                               // 24
    int32      ItemStatType[MAX_ITEM_PROTO_STATS];           // 25 - 35
    int32      ItemStatValue[MAX_ITEM_PROTO_STATS];          // 36 - 46
    int32      ItemStatUnk1[MAX_ITEM_PROTO_STATS];           // 47 - 57
    int32      ItemStatUnk2[MAX_ITEM_PROTO_STATS];           // 58 - 68
    uint32     ScalingStatDistribution;                      // 69
    uint32     DamageType;                                   // 70
    uint32     Delay;                                        // 71
    float      RangedModRange;                               // 72
    int32      SpellId[MAX_ITEM_PROTO_SPELLS];               //
    int32      SpellTrigger[MAX_ITEM_PROTO_SPELLS];          //
    int32      SpellCharges[MAX_ITEM_PROTO_SPELLS];          //
    int32      SpellCooldown[MAX_ITEM_PROTO_SPELLS];         //
    int32      SpellCategory[MAX_ITEM_PROTO_SPELLS];         // 
    int32      SpellCategoryCooldown[MAX_ITEM_PROTO_SPELLS]; //
    uint32     Bonding;                                      // 99
    char*      Name;                                         // 100
    char*      Name2;                                        // 101
    char*      Name3;                                        // 102
    char*      Name4;                                        // 103
    char*      Description;                                  // 104
    uint32     PageText;                                     // 105
    uint32     LanguageID;                                   // 106
    uint32     PageMaterial;                                 // 107
    uint32     StartQuest;                                   // 108
    uint32     LockID;                                       // 109
    int32      Material;                                     // 110
    uint32     Sheath;                                       // 111
    uint32     RandomProperty;                               // 112
    uint32     RandomSuffix;                                 // 113
    uint32     ItemSet;                                      // 114
    uint32     Area;                                         // 115
    uint32     Map;                                          // 116
    uint32     BagFamily;                                    // 117
    uint32     TotemCategory;                                // 118
    uint32     Color[MAX_ITEM_PROTO_SOCKETS];                // 119 - 121
    uint32     Content[MAX_ITEM_PROTO_SOCKETS];              // 122 - 124
    int32      SocketBonus;                                  // 125
    uint32     GemProperties;                                // 126
    float      ArmorDamageModifier;                          // 127
    uint32     Duration;                                     // 128
    uint32     ItemLimitCategory;                            // 129
    uint32     HolidayId;                                    // 130
    float      StatScalingFactor;                            // 131
    int32      CurrencySubstitutionId;                       // 132
    int32      CurrencySubstitutionCount;                    // 133
};

#define KEYCHAIN_SIZE   32

struct KeyChainEntry
{
    uint32      Id;
    uint8       Key[KEYCHAIN_SIZE];
};

// SceneScript.db2
struct SceneScriptEntry
{
    uint32 m_ID;                                             // 0         m_ID
    LocalizedString* m_name;                                 // 1         m_name
    LocalizedString* m_script;                               // 2         m_script
    uint32 m_prevScriptPartID;                               // 3         m_prevScriptPartID - Prev Script Part Id From Chain
    uint32 m_nextScriptPartID;                               // 4         m_nextScriptPartID - Next Script Part Id From Chain
};

// SpellReagents.db2
struct SpellReagentsEntry
{
    //uint32    Id;                                          // 0         m_ID
    int32     Reagent[MAX_SPELL_REAGENTS];                   // 1  - 10   m_reagent
    uint32    ReagentCount[MAX_SPELL_REAGENTS2];             // 11 - 20   m_reagentCount
};

// GCC has alternative #pragma pack(N) syntax and old gcc version does not support pack(push, N), also any gcc version does not support it at some platform
#if defined(__GNUC__)
#pragma pack()
#else
#pragma pack(pop)
#endif

#endif
