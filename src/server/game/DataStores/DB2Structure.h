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
struct BattlePetAbilityEntry
{
    uint32 AbilityId;                                       // 0 - battle pet ability id
    int32 FamilyId;                                         // 1 - battle pet family id or -1 for aura
    uint32 IconId;                                          // 2 - icon id
    uint32 Cooldown;                                        // 3 - cooldown in turns
    //uint32 Unk1;                                          // 4 - something to do with linked abilities
    //uint32 Unk2;                                          // 5
    LocalizedString* Name;                                  // 6 - name text
    LocalizedString* Description;                           // 7 - description text
};

struct BattlePetAbilityStateEntry
{
    uint32 Id;                                              // 0
    uint32 AbilityId;                                       // 1 - battle pet ability id (BattlePetAbility.db2)
    uint32 StateId;                                         // 2 - battle pet state id (BattlePetState.db2)
    uint32 Value;                                           // 3 - value for state
};

struct BattlePetBreedStateEntry
{
    uint32 Id;                                              // 0
    uint32 BreedId;                                         // 1 - battle pet breed id
    uint32 StateId;                                         // 2 - battle pet state id (BattlePetState.db2)
    uint32 Value;                                           // 3 - value for state
};

typedef std::set<uint32> BattlePetBreedSet;

struct BattlePetSpeciesEntry
{
    uint32 SpeciesId;                                       // 0 - battle pet species id
    uint32 NpcId;                                           // 1 - npc id
    uint32 IconId;                                          // 2 - icon id
    uint32 SpellId;                                         // 3 - summon spell id (Spell.dbc)
    uint32 FamilyId;                                        // 4 - battle pet family id
    //int32 Unk1;                                           // 5
    uint32 Flags;                                           // 6 - flags (see BattlePetSpeciesFlags enum)
    LocalizedString* Description;                           // 7 - description text, contains method to obtain and cost
    LocalizedString* Flavor;                                // 8 - flavor text
};

struct BattlePetSpeciesStateEntry
{
    uint32 Id;                                              // 0
    uint32 SpeciesId;                                       // 1 - battle pet species id (BattlePetSpecies.db2)
    uint32 StateId;                                         // 2 - battle pet state id (BattlePetState.db2)
    int32 Modifier;                                         // 3 - modifier value for state
};

struct BattlePetSpeciesXAbilityEntry
{
    uint32 Id;                                              // 0
    uint32 SpeciesId;                                       // 1 - battle pet species id (BattlePetSpecies.db2)
    uint32 AbilityId;                                       // 2 - battle pet ability id (BattlePetAbility.db2)
    uint32 RequiredLevel;                                   // 3 - required level to use this ability
    uint32 SlotId;                                          // 4 - ability slot id (0-2)
};

struct BattlePetStateEntry
{
    uint32 StateId;                                         // 0 - battle pet state id
    //uint32 Unk1;                                          // 1 - only states 21 and 34 (linked states?)
    LocalizedString* Name;                                  // 2 - name text
    uint32 Flags;                                           // 3 - flags
};

typedef std::map<uint32, uint16> BattlePetItemXSpeciesStore;

struct ItemToBattlePetEntry
{
    uint32 ItemId;                                          // 0 - item id
    uint32 SpeciesId;                                       // 1 - battle pet species id (BattlePetSpecies.db2)
};

struct BroadcastTextEntry
{
    uint32   ID;                                             // 0
    //uint32   Unk0;                                         // 1
    LocalizedString* text_0;                                 // 2
    LocalizedString* text_1;                                 // 3
    //uint32   Unk1;                                         // 4
    //uint32   Unk2;                                         // 5
    //uint32   Unk3;                                         // 6
    //uint32   Unk4;                                         // 7
    //uint32   Unk5;                                         // 8
    //uint32   Unk6;                                         // 9
    uint32   SoundID;                                        // 10
    //uint32   Unk7;                                         // 11
    //uint32   Unk8;                                         // 12
};

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
    uint32     Flags3;                                       // 4
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
    int32      ItemStatType[MAX_ITEM_PROTO_STATS];           // 25 - 34
    uint32     ItemStatValue[MAX_ITEM_PROTO_STATS];          // 35 - 44
    int32      ItemStatUnk1[MAX_ITEM_PROTO_STATS];           // 45 - 54
    int32      ItemStatUnk2[MAX_ITEM_PROTO_STATS];           // 55 - 64
    uint32     ScalingStatDistribution;                      // 65
    uint32     DamageType;                                   // 66
    uint32     Delay;                                        // 67
    float      RangedModRange;                               // 68
    int32      SpellId[MAX_ITEM_PROTO_SPELLS];               // 69 - 73
    int32      SpellTrigger[MAX_ITEM_PROTO_SPELLS];          // 74 - 78
    int32      SpellCharges[MAX_ITEM_PROTO_SPELLS];          // 79 - 83
    int32      SpellCooldown[MAX_ITEM_PROTO_SPELLS];         // 84 - 88
    int32      SpellCategory[MAX_ITEM_PROTO_SPELLS];         // 89 - 89
    int32      SpellCategoryCooldown[MAX_ITEM_PROTO_SPELLS]; // 94 - 98
    uint32     Bonding;                                      // 99
    LocalizedString* Name;                                   // 100
    LocalizedString* Name2;                                  // 101
    LocalizedString* Name3;                                  // 102
    LocalizedString* Name4;                                  // 103
    LocalizedString* Description;                            // 104
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

#define MAX_ITEM_EXT_COST_ITEMS         5
#define MAX_ITEM_EXT_COST_CURRENCIES    5

struct ItemExtendedCostEntry
{
    uint32      ID;                                         // 0 extended-cost entry id
    //uint32    reqhonorpoints;                             // 1 required honor points
    //uint32    reqarenapoints;                             // 2 required arena points
    uint32      RequiredArenaSlot;                          // 3 arena slot restrictions (min slot value)
    uint32      RequiredItem[MAX_ITEM_EXT_COST_ITEMS];      // 4-8 required item id
    uint32      RequiredItemCount[MAX_ITEM_EXT_COST_ITEMS]; // 9-13 required count of 1st item
    uint32      RequiredPersonalArenaRating;                // 14 required personal arena rating
    //uint32    ItemPurchaseGroup;                          // 15
    uint32      RequiredCurrency[MAX_ITEM_EXT_COST_CURRENCIES];// 16-20 required curency id
    uint32      RequiredCurrencyCount[MAX_ITEM_EXT_COST_CURRENCIES];// 21-25 required curency count
    uint32      RequiredFactionId;
    uint32      RequiredFactionStanding;
    uint32      RequirementFlags;
    uint32      RequiredGuildLevel;
    uint32      RequiredAchievement;
};

#define KEYCHAIN_SIZE   32

struct KeyChainEntry
{
    uint32      Id;
    uint8       Key[KEYCHAIN_SIZE];
};

// QuestPackageItem.db2
struct QuestPackageItemEntry
{
    uint32      ID;                                         // 0
    uint32      QuestPackageID;                             // 1
    uint32      ItemID;                                     // 2
    uint32      Unk1;                                       // 3
    uint32      Unk2;                                       // 4
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