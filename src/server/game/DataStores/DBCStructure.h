/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_DBCSTRUCTURE_H
#define SKYFIRE_DBCSTRUCTURE_H

#include "Common.h"
#include "DBCEnums.h"
#include "Define.h"
#include "Path.h"
#include "Util.h"

#include <map>
#include <set>
#include <vector>
#include <unordered_map>

// Structures using to access raw DBC data and required packing to portability

// GCC have alternative #pragma pack(N) syntax and old gcc version not support pack(push, N), also any gcc version not support it at some platform
#if defined(__GNUC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

struct AchievementEntry
{
    uint32    ID;                                           // 0
    int32    requiredFaction;                               // 1 -1=all, 0=horde, 1=alliance
    int32    mapID;                                         // 2 -1=none
    //uint32 parentAchievement;                             // 3 its Achievement parent (can`t start while parent uncomplete, use its Criteria if don`t have own, use its progress on begin)
    char* name;                                             // 4
    //char* description;                                    // 5
    //uint32    categoryId;                                 // 6
    uint32    points;                                       // 7 reward points
    //uint32 OrderInCategory;                               // 8
    uint32    flags;                                        // 9
    //uint32    icon;                                       // 10 icon (from SpellIcon.dbc)
    //char* reward;                                         // 11
    uint32 count;                                           // 12 - need this count of completed criterias (own or referenced achievement criterias)
    uint32 refAchievement;                                  // 13 - referenced achievement (counting of all completed criterias)
    uint32 criteriaTreeID;                                  // 14 - Pandaria
};

/*
struct AchievementCategoryEntry
{
    uint32    ID;                                           // 0
    uint32    parentCategory;                               // 1 -1 for main category
    //char* name;                                           // 2
    //uint32    sortOrder;                                  // 3
};


struct AchievementCriteriaEntry
{
    uint32  ID;                                             // 0
    uint32  achievement;                                    // 1
    uint32  type;                                           // 2
    union
    {
        // ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE          = 0
        /// @todo also used for player deaths..
        struct
        {
            uint32  creatureID;                             // 3
            uint32  creatureCount;                          // 4
        } kill_creature;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_BG                 = 1
        struct
        {
            uint32  bgMapID;                                // 3
            uint32  winCount;                               // 4
        } win_bg;
        // ACHIEVEMENT_CRITERIA_TYPE_FIND_ARTIFACT          = 3
        struct
        {
            uint32  unused;                                 // 3
            uint32  count;                                  // 4
        } find_artifact;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL            = 5
        // ACHIEVEMENT_CRITERIA_TYPE_REACH_GUILD_LEVEL      = 125
        struct
        {
            uint32  unused;                                 // 3
            uint32  level;                                  // 4
        } reach_level;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL      = 7
        struct
        {
            uint32  skillID;                                // 3
            uint32  skillLevel;                             // 4
        } reach_skill_level;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT   = 8
        struct
        {
            uint32  linkedAchievement;                      // 3
        } complete_achievement;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT   = 9
        struct
        {
            uint32  unused;                                 // 3
            uint32  totalQuestCount;                        // 4
        } complete_quest_count;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST_DAILY = 10
        struct
        {
            uint32  unused;                                 // 3
            uint32  numberOfDays;                           // 4
        } complete_daily_quest_daily;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE = 11
        struct
        {
            uint32  zoneID;                                 // 3
            uint32  questCount;                             // 4
        } complete_quests_in_zone;

        // ACHIEVEMENT_CRITERIA_TYPE_CURRENCY               = 12
        struct
        {
            uint32 currency;
            uint32 count;
        } currencyGain;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST   = 14
        struct
        {
            uint32  unused;                                 // 3
            uint32  questCount;                             // 4
        } complete_daily_quest;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND  = 15
        struct
        {
            uint32  mapID;                                  // 3
        } complete_battleground;

        // ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP           = 16
        struct
        {
            uint32  mapID;                                  // 3
        } death_at_map;

        // ACHIEVEMENT_CRITERIA_TYPE_DEATH_IN_DUNGEON       = 18
        struct
        {
            uint32  manLimit;                               // 3
        } death_in_dungeon;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_RAID          = 19
        struct
        {
            uint32  groupSize;                              // 3 can be 5, 10 or 25
        } complete_raid;

        // ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE     = 20
        struct
        {
            uint32  creatureEntry;                          // 3
        } killed_by_creature;

        // ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING     = 24
        struct
        {
            uint32  unused;                                 // 3
            uint32  fallHeight;                             // 4
        } fall_without_dying;

        // ACHIEVEMENT_CRITERIA_TYPE_DEATHS_FROM            = 26
        struct
        {
            uint32 type;                                    // 3, see enum EnviromentalDamage
        } death_from;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST         = 27
        struct
        {
            uint32  questID;                                // 3
            uint32  questCount;                             // 4
        } complete_quest;

        // ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET        = 28
        // ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2       = 69
        struct
        {
            uint32  spellID;                                // 3
            uint32  spellCount;                             // 4
        } be_spell_target;

        // ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL             = 29
        // ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2            = 110
        struct
        {
            uint32  spellID;                                // 3
            uint32  castCount;                              // 4
        } cast_spell;

        // ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE   = 30
        struct
        {
            uint32 objectiveId;                             // 3
            uint32 completeCount;                           // 4
        } bg_objective;

        // ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA = 31
        struct
        {
            uint32  areaID;                                 // 3 Reference to AreaTable.dbc
            uint32  killCount;                              // 4
        } honorable_kill_at_area;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA              = 32
        struct
        {
            uint32 mapID;                                   // 3 Reference to Map.dbc
            uint32 count;                                   // 4 Number of times that the arena must be won.
        } win_arena;

        // ACHIEVEMENT_CRITERIA_TYPE_PLAY_ARENA             = 33
        struct
        {
            uint32  mapID;                                  // 3 Reference to Map.dbc
        } play_arena;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL            = 34
        struct
        {
            uint32  spellID;                                // 3 Reference to Map.dbc
        } learn_spell;

        // ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM               = 36
        struct
        {
            uint32  itemID;                                 // 3
            uint32  itemCount;                              // 4
        } own_item;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA        = 37
        struct
        {
            uint32  unused;                                 // 3
            uint32  count;                                  // 4
        } win_rated_arena;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_TEAM_RATING    = 38
        struct
        {
            uint32  teamtype;                               // 3 {2, 3, 5}
        } highest_team_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_TEAM_RATING      = 39
        struct
        {
            uint32  teamtype;                               // 3 {2, 3, 5}
            uint32  teamrating;                             // 4
        } reach_team_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_PERSONAL_RATING = 39
        struct
        {
            uint32 teamtype; // 3 {2, 3, 5}
            uint32 PersonalRating; // 4
        } highest_personal_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL      = 40
        struct
        {
            uint32  skillID;                                // 3
            uint32  skillLevel;                             // 4 apprentice=1, journeyman=2, expert=3, artisan=4, master=5, grand master=6
        } learn_skill_level;

        // ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM               = 41
        struct
        {
            uint32  itemID;                                 // 3
            uint32  itemCount;                              // 4
        } use_item;

        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM              = 42
        struct
        {
            uint32  itemID;                                 // 3
            uint32  itemCount;                              // 4
        } loot_item;

        // ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA           = 43
        struct
        {
            /// @todo This rank is _NOT_ the index from AreaTable.dbc
            uint32  areaReference;                          // 3
        } explore_area;

        // ACHIEVEMENT_CRITERIA_TYPE_OWN_RANK               = 44
        struct
        {
            /// @todo This rank is _NOT_ the index from CharTitles.dbc
            uint32  rank;                                   // 3
        } own_rank;

        // ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT          = 45
        struct
        {
            uint32  unused;                                 // 3
            uint32  numberOfSlots;                          // 4
        } buy_bank_slot;

        // ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION        = 46
        struct
        {
            uint32  factionID;                              // 3
            uint32  reputationAmount;                       // 4 Total reputation amount, so 42000 = exalted
        } gain_reputation;

        // ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION= 47
        struct
        {
            uint32  unused;                                 // 3
            uint32  numberOfExaltedFactions;                // 4
        } gain_exalted_reputation;

        // ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP      = 48
        struct
        {
            uint32 unused;                                  // 3
            uint32 numberOfVisits;                          // 4
        } visit_barber;

        // ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM        = 49
        /// @todo where is the required itemlevel stored?
        struct
        {
            uint32  itemSlot;                               // 3
            uint32  count;                                  // 4
        } equip_epic_item;

        // ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT      = 50
        struct
        {
            uint32  rollValue;                              // 3
            uint32  count;                                  // 4
        } roll_need_on_loot;
       // ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED_ON_LOOT      = 51
        struct
        {
            uint32  rollValue;                              // 3
            uint32  count;                                  // 4
        } roll_greed_on_loot;

        // ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS               = 52
        struct
        {
            uint32  classID;                                // 3
            uint32  count;                                  // 4
        } hk_class;

        // ACHIEVEMENT_CRITERIA_TYPE_HK_RACE                = 53
        struct
        {
            uint32  raceID;                                 // 3
            uint32  count;                                  // 4
        } hk_race;

        // ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE               = 54
        /// @todo where is the information about the target stored?
        struct
        {
            uint32  emoteID;                                // 3 enum TextEmotes
            uint32  count;                                  // 4 count of emotes, always required special target or requirements
        } do_emote;
        // ACHIEVEMENT_CRITERIA_TYPE_DAMAGE_DONE            = 13
        // ACHIEVEMENT_CRITERIA_TYPE_HEALING_DONE           = 55
        // ACHIEVEMENT_CRITERIA_TYPE_GET_KILLING_BLOWS      = 56
        struct
        {
            uint32  unused;                                 // 3
            uint32  count;                                  // 4
        } healing_done;

        // ACHIEVEMENT_CRITERIA_TYPE_GET_KILLING_BLOWS      = 56
        struct
        {
            uint32  unused;
            uint32  killCount;
        } get_killing_blow;

        // ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM             = 57
        struct
        {
            uint32  itemID;                                 // 3
            uint32  count;                                  // 4
        } equip_item;

        // ACHIEVEMENT_CRITERIA_TYPE_MONEY_FROM_QUEST_REWARD= 62
        struct
        {
            uint32  unused;                                 // 3
            uint32  goldInCopper;                           // 4
        } quest_reward_money;

        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY             = 67
        struct
        {
            uint32  unused;                                 // 3
            uint32  goldInCopper;                           // 4
        } loot_money;

        // ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT         = 68
        struct
        {
            uint32  goEntry;                                // 3
            uint32  useCount;                               // 4
        } use_gameobject;

        // ACHIEVEMENT_CRITERIA_TYPE_SPECIAL_PVP_KILL       = 70
        /// @todo are those special criteria stored in the dbc or do we have to add another sql table?
        struct
        {
            uint32  unused;                                 // 3
            uint32  killCount;                              // 4
        } special_pvp_kill;

        // ACHIEVEMENT_CRITERIA_TYPE_FISH_IN_GAMEOBJECT     = 72
        struct
        {
            uint32  goEntry;                                // 3
            uint32  lootCount;                              // 4
        } fish_in_gameobject;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILLLINE_SPELLS = 75
        struct
        {
            uint32  skillLine;                              // 3
            uint32  spellCount;                             // 4
        } learn_skillline_spell;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_DUEL               = 76
        struct
        {
            uint32  unused;                                 // 3
            uint32  duelCount;                              // 4
        } win_duel;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_POWER          = 96
        struct
        {
            uint32  powerType;                              // 3 mana=0, 1=rage, 3=energy, 6=runic power
        } highest_power;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_STAT           = 97
        struct
        {
            uint32  statType;                               // 3 4=spirit, 3=int, 2=stamina, 1=agi, 0=strength
        } highest_stat;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_SPELLPOWER     = 98
        struct
        {
            uint32  spellSchool;                            // 3
        } highest_spellpower;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_RATING         = 100
        struct
        {
            uint32  ratingType;                             // 3
        } highest_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE              = 109
        struct
        {
            uint32  lootType;                               // 3 3=fishing, 2=pickpocket, 4=disentchant
            uint32  lootTypeCount;                          // 4
        } loot_type;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LINE       = 112
        struct
        {
            uint32  skillLine;                              // 3
            uint32  spellCount;                             // 4
        } learn_skill_line;

        // ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL    = 113
        struct
        {
            uint32  unused;                                 // 3
            uint32  killCount;                              // 4
        } honorable_kill;

        // ACHIEVEMENT_CRITERIA_TYPE_114_SUMMON             = 114
        struct
        {
            uint32  unused;                                 // 3
            uint32  unk;                                    // 4 unk is 1 true/false?
        } summons;
        // ACHIEVEMENT_CRITERIA_TYPE_EARN_ACHIEVEMENT_POINT = 115
        // ACHIEVEMENT_CRITERIA_TYPE_118_UNK                = 118
        // ACHIEVEMENT_CRITERIA_TYPE_LFG                    = 119
        struct
        {
            uint32  unused;
            uint32  dungeonsComplete;
        } use_lfg;
        // ACHIEVEMENT_CRITERIA_TYPE_120                    = 120
        // ACHIEVEMENT_CRITERIA_TYPE_121                    = 121
        // ACHIEVEMENT_CRITERIA_TYPE_122                    = 122
        // ACHIEVEMENT_CRITERIA_TYPE_123                    = 123
        // ACHIEVEMENT_CRITERIA_TYPE_GUILD_REPAIR_SPEND     = 124
        struct
        {
            uint32  unused;                                 // 3
            uint32  goldInCopper;                           // 4
        } guild_repair_money;

        // ACHIEVEMENT_CRITERIA_TYPE_GUILD_LEVEL            = 125
        struct
        {
            uint32  unused;                                 // 3
            uint32  level;                                  // 4
        } guild_level;
        // ACHIEVEMENT_CRITERIA_TYPE_GUILD_CRAFTING         = 126
        struct
        {
            uint32  unused;                                 // 3
            uint32  count;                                  // 4
        } guild_craft;
        // ACHIEVEMENT_CRITERIA_TYPE_GUILD_FISHING         = 127
        struct
        {
            uint32  unused;                                 // 3
            uint32  count;                                  // 4
        } guild_fishing;
        // ACHIEVEMENT_CRITERIA_TYPE_GUILD_BANK_TAB_BUY    = 128
        struct
        {
            uint32  unused;                                 // 3
            uint32  tab;                                    // 4
        } guild_tab_bought;
        // ACHIEVEMENT_CRITERIA_TYPE_EARN_GUILD_ACH_POINTS = 129
        struct
        {
            uint32  unused;                                 // 3
            uint32  count;                                    // 4
        } guild_ach_pts;
        // ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_BG          = 130
        struct
        {
            uint32  unused;                                 // 3
            uint32  count;                                    // 4
        } rated_bg_won;
        // ACHIEVEMENT_CRITERIA_TYPE_EARN_RATED_BG_RATING  = 132
        struct
        {
            uint32  unused;                                 // 3
            uint32  rating;                                 // 4
        } rated_bg_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_CREATE_GUILD_LOGO     = 133
        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT2 = 134
        struct
        {
            uint32  unused;                                 // 3
            uint32  totalQuestCount;                        // 4
        } complete_quest_count2;

        // ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL2  = 135
        struct
        {
            uint32  unused;                                 // 3
            uint32  killCount;                              // 4
        } honorable_kill2;

        // ACHIEVEMENT_CRITERIA_TYPE_EARN_CRITTER_KILLS    = 136
        struct
        {
            uint32  unused;                                 // 3
            uint32  killCount;                              // 4
        } critter_kills;
        // ACHIEVEMENT_CRITERIA_TYPE_137                   = 137
        // ACHIEVEMENT_CRITERIA_TYPE_GUILD_CHALLENGES      = 138
        struct
        {
            uint32  type;                                   // 3
            uint32  count;                                  // 4
        } guild_challenges;
        // ACHIEVEMENT_CRITERIA_TYPE_GUILD_CHALLENGES2     = 139
        struct
        {
            uint32  unused;                                 // 3
            uint32  count;                                  // 4
        } guild_challenges2;
        // ACHIEVEMENT_CRITERIA_TYPE_140_150_PLACEHOLDER   = 140-150
        struct
        {
            uint32  field3;                                 // 3 main requirement
            uint32  count;                                  // 4 main requirement count
        } raw;
    };

    //uint32 unk;                                           // 5

    struct
    {
        uint32  additionalRequirement_type;
        uint32  additionalRequirement_value;
    } additionalRequirements[MAX_CRITERIA_REQUIREMENTS];

    char*  name;                                            // 10        m_description_lang
    uint32  completionFlag;                                 // 11       m_flags
    uint32  timedCriteriaStartType;                         // 12       m_timer_start_event Only appears with timed achievements, seems to be the type of starting a timed Achievement, only type 1 and some of type 6 need manual starting
                                                            //              1: ByEventId(?) (serverside IDs),    2: ByQuestId,   5: ByCastSpellId(?)
                                                            //              6: BySpellIdTarget(some of these are unknown spells, some not, some maybe spells)
                                                            //              7: ByKillNpcId,  9: ByUseItemId
    uint32  timedCriteriaMiscId;                            // 13       m_timer_asset_id Alway appears with timed events, used internally to start the achievement, store
    uint32  timeLimit;                                      // 14       m_timer_time time limit in seconds
    uint32  showOrder;                                      // 15       m_ui_order  also used in achievement shift-links as index in state bitmask
    //uint32 unk1;                                          // 16 only one value, still unknown
    //uint32 unk2;                                          // 17 all zeros
    uint32 additionalConditionType[MAX_ADDITIONAL_CRITERIA_CONDITIONS];      // 18-20
    uint32 additionalConditionValue[MAX_ADDITIONAL_CRITERIA_CONDITIONS - 1]; // 21-22 WTF one column was cut off here in 4.3.4
};
*/
struct AreaTableEntry
{
    uint32  m_ID;                                           // 0
    uint32  m_ContinentID;                                  // 1
    uint32  m_ParentAreaID;                                 // 2 if 0 then it's zone, else it's zone id of this area
    uint32  m_AreaBit;                                      // 3, main index
    uint32  m_flags;                                        // 5,
    //uint32  m_flags2;                                     // 6, Pandaria
    //uint32  m_SoundProviderPreferences;                   // 7,
    //uint32  m_SoundProviderPreferencesUnderwater;         // 8,
    //uint32  m_AmbienceID;                                 // 9,
    //uint32  m_ZoneMusic;                                  // 10,
    //char*   m_ZoneName;                                   // 11, without whitespaces
    //uint32  m_IntroSound;                                 // 12
    int32   m_ExplorationLevel;                             // 13
    char*   m_AreaName;                                     // 14
    uint32  m_FactionGroupMask;                             // 15
    uint32  m_LiquidType[4];                                // 16-19 liquid override by type
    float   m_MinElevation;                                 // 20,
    //float   m_AmbientMultiplier;                          // 21 client only?
    //uint32  m_LightId;                                    // 22
    //uint32  m_MountFlags;                                 // 23 4.0.0 - Mounting related
    //uint32  m_UwIntroSound;                               // 24 4.0.0
    //uint32  m_UwZoneMusic;                                // 25 4.0.0
    //uint32  m_UwAmbience;                                 // 26 4.0.0
    //uint32  m_WorldPvPID;                                 // 27 Wintergrasp, TolBarad
    //uint32  m_PvPCombatWorldStateID                       // 28 worldstateid
    //uint32  m_WildPetLevelMin;                            // 28, Pandaria
    //uint32  m_WildPetLevelmax;                            // 29, Pandaria

    // helpers
    bool IsSanctuary() const
    {
        if (m_ContinentID == 609)
            return true;
        return (m_flags & AREA_FLAG_SANCTUARY);
    }
};

#define MAX_GROUP_AREA_IDS 6

struct AreaGroupEntry
{
    uint32  AreaGroupId;                                    // 0
    uint32  AreaId[MAX_GROUP_AREA_IDS];                     // 1-6
    uint32  nextGroup;                                      // 7 index of next group
};

/*
struct AreaPOIEntry
{
    uint32 id;              // 0
    uint32 icon[11];        // 1-11
    float x;                // 12
    float y;                // 13
    uint32 mapId;           // 14
    // uint32 val1;         // 15
    uint32 zoneId;          // 16
    // char* name;          // 17 - name
    // char* name2;         // 18 - name2
    uint32 worldState;      // 19
    // uint32 val2;         // 20
    // uint32 unk;          // 21
    // uint32 unk2;         // 21 - Pandaria
};
*/
struct AreaTriggerEntry
{
    uint32  id;                                             // 0        m_ID
    uint32  mapid;                                          // 1        m_ContinentID
    float   x;                                              // 2        m_x
    float   y;                                              // 3        m_y
    float   z;                                              // 4        m_z
    //uint32                                                // 5
    //uint32                                                // 6
    //uint32                                                // 7
    float   radius;                                         // 8        m_radius
    float   box_x;                                          // 9        m_box_length
    float   box_y;                                          // 10       m_box_width
    float   box_z;                                          // 11       m_box_heigh
    float   box_orientation;                                // 12       m_box_yaw
    //uint32  unk1;                                         // 13 - Pandaria
    //uint32  unk2;                                         // 14 - Pandaria
    //uint32  unk3;                                         // 15 - Pandaria
};

struct ArmorLocationEntry
{
  //uint32    InventoryType;                                // 0
  float     Value[5];                                       // 1-5 multiplier for armor types (cloth...plate, no armor?)
};

struct AuctionHouseEntry
{
    uint32    houseId;                                      // 0 index
    uint32    faction;                                      // 1 id of faction.dbc for player factions associated with city
    uint32    depositPercent;                               // 2 1/3 from real
    uint32    cutPercent;                                   // 3
    //char*     name;                                       // 4
};

struct BankBagSlotPricesEntry
{
    //uint32  ID;
    uint32  price;
};

struct BannedAddOnsEntry
{
    uint32 Id;
    // uint32 NameMD5[4];
    // uint32 VersionMD5[4];
    // uint32 Timestamp;
    // uint32 State;
};

struct BarberShopStyleEntry
{
    //uint32  Id;                                           // 0
    uint32  type;                                           // 1 value 0 -> hair, value 2 -> facialhair
    //char*   name;                                         // 2        m_DisplayName_lang
    //uint32  unk_name;                                     // 3        m_Description_lang
    //float   CostMultiplier;                               // 4        m_Cost_Modifier
    uint32  race;                                           // 5        m_race
    uint32  gender;                                         // 6        m_sex
    uint32  hair_id;                                        // 7        m_data (real ID to hair/facial hair)
};

struct BattlemasterListEntry
{
    uint32  id;                                             // 0
    int32   mapid[16];                                      // 1-16 mapid
    uint32  type;                                           // 17 (3 - BG, 4 - arena)
    //char* unkName;                                        // 18
    char* name;                                             // 19
    uint32 maxGroupSize;                                    // 20 maxGroupSize, used for checking if queue as group
    uint32 HolidayWorldStateId;                             // 21 new 3.1
    //uint32 minLevel;                                        // 22, min level (sync with PvPDifficulty.dbc content)
    //uint32 maxLevel;                                        // 23, max level (sync with PvPDifficulty.dbc content)
    //uint32 maxGroupSizeRated;                             // 24 4.0.1
    //uint32 unk;                                           // 25 - 4.0.6.13596
    //uint32 maxPlayers;                                    // 26 4.0.1
    //uint32 unk1;                                          // 27 4.0.3, value 2 for Rated Battlegrounds
    //uint32 unk2                                           // 28 - Pandaria
    //char* strType;                                        // 29 - Pandaria
};

#define MAX_OUTFIT_ITEMS 24

struct CharStartOutfitEntry
{
    //uint32 Id;                                            // 0
    uint8 Race;                                             // 1
    uint8 Class;                                            // 2
    uint8 Gender;                                           // 3
    //uint8 Unused;                                         // 4
    int32 ItemId[MAX_OUTFIT_ITEMS];                         // 5-28
    //int32 ItemDisplayId[MAX_OUTFIT_ITEMS];                // 29-52 not required at server side
    //int32 ItemInventorySlot[MAX_OUTFIT_ITEMS];            // 53-76 not required at server side
    //uint32 PetDisplayId;                                  // 77 Pet Model ID for starting pet
    //uint32 PetFamilyEntry;                                // 78 Pet Family Entry for starting pet
};

struct CharTitlesEntry
{
    //uint32  ID;                                           // 0, title ids, for example in Quest::GetCharTitleId()
    //uint32      unk1;                                     // 1 flags?
    char* name;                                             // 2        m_name_lang
    //char*       name2;                                    // 3        m_name1_lang
    uint32  bit_index;                                      // 4        m_mask_ID used in PLAYER_FIELD_PLAYER_TITLE and 1<<index in PLAYER__FIELD_KNOWN_TITLES
    //uint32                                                // 5
};

struct ChatChannelsEntry
{
    uint32  ChannelID;                                      // 0
    uint32  flags;                                          // 1
    //uint32                                                // 2        m_factionGroup
    char* pattern;                                          // 3        m_name_lang
    //char*       name;                                     // 4        m_shortcut_lang
};

struct ChrClassesEntry
{
    //uint32  ClassID;                                      // 0
    uint32  powerType;                                      // 1        m_DisplayPower
                                                            // 2        m_petNameToken
    char* name;                                             // 3        m_name_lang
    //char*       nameFemale;                               // 4        m_name_female_lang
    //char*       nameNeutralGender;                        // 5        m_name_male_lang
    //char*       capitalizedName                           // 6,       m_filename
    uint32  spellfamily;                                    // 7        m_spellClassSet
    //uint32 flags2;                                        // 8        m_flags (0x08 HasRelicSlot)
    uint32 CinematicSequence;                               // 9        m_cinematicSequenceID
    uint32 APPerStrenth;                                    // 10       Attack Power bonus per point of strength
    uint32 APPerAgility;                                    // 11       Attack Power bonus per point of agility
    uint32 RAPPerAgility;                                   // 12       Ranged Attack Power bonus per point of agility
    //uint32 unk1                                           // 13       Pandaria
    //uint32 unk1                                           // 14       Pandaria
    //uint32 unk1                                           // 15       Pandaria
    //uint32 unk1                                           // 16       Pandaria
    //uint32 unk1                                           // 17       Pandaria
};

struct ChrRacesEntry
{
    //uint32 RaceID;                                        // 0
    //uint32 Flags;                                         // 1 0x4 canMount, 0x8 hasBald, 0x10 bindStartArea 0x80 ForceHDModel 0x400 ReputationBonus 0x4000 NoStartingWeapon
    uint32 FactionID;                                       // 2 facton template id
    //uin32 ExplorationSoundID;                             // 3
    uint32 model_m;                                         // 4
    uint32 model_f;                                         // 5
    //char* ClientPrefix;                                   // 6
    //uint32 BaseLanguage;                                  // 7 (42-Neutral 7-Alliance 1-Horde)
    //uint32 CreatureType;                                  // 8 (All 7)
    //uint32 ResSickSpell;                                  // 9 (All 15007)
    //uint32 SplashSoundID;                                 // 10 (All 1096)
    //char* ClientFileString;                               // 11
    uint32 CinematicSequence;                               // 12 id from CinematicSequences.dbc
    uint32 TeamID;                                          // 13 (0 alliance, 1 horde, 2 neutral)
    char* name;                                             // 14 m_name_lang used for DBC language detection/selection
    //char* nameFemale;                                     // 15 ""
    //char* nameNeutralGender;                              // 16 ""
    //uint32 m_facialHairCustomization[2]                   // 17-18
    //uint32 m_hairCustomization                            // 19
    //uint32 m_enemyRace;                                   // 20 m_enemyRace
    //uint32 UnalteredVisualRaceID;                         // 21 (23 for worgens = Gilnean)
    //uint32 UaMaleCreatureSoundDataID;                     // 22 (Gilnean 3133)
    //uint32 UaFemaleCreatureSoundDataID;                   // 23 (Gilnean 3134)
    //uint32 CharComponentTextureLayout;                    // 24 (All 1, Pandaren 2)
    //uint32 DefaultClassID                                 // 25
    //uint32 CreateScreenFileDataID;                        // 26
    //uint32 SelectScreenFileDataID;                        // 27
    //float MaleCustomizeOffset[3];                         // 28 - 30
    //float FemaleCustomizeOffset[3];                       // 31 - 33
    //uint32 NeutralRaceID;                                 // 34
    //uint32 LowResScreenFileDataID;                        // 35
};

struct ChrPowerTypesEntry
{
   //uint32 entry;                                             // 0
   uint32 classId;                                             // 1
   uint32 power;                                               // 2
};

struct ChrSpecializationEntry
{
    uint32  Id;                                             // 0
    //char* unkName;                                        // 1
    uint32 classId;                                         // 2
    uint32 MasterySpellId;                                  // 3
    // uint32 unk                                           // 4 - prolly MasterySpellId[1] - always 0
    uint32 TabPage;                                         // 5
    uint32 PetTabPage;                                      // 6
    //uint32 RoleMask;                                      // 7
    // uint32 iconId                                        // 8
    // uint32 unk1                                          // 9
    // uint32 unk2                                          // 10
    // char* name                                           // 11
    // char* definition                                     // 12
    // char* unkName2                                       // 13
};

struct CinematicCameraEntry
{
    uint32      id;                                         // 0 index
    char*       filename;                                   // 1
    uint32      soundid;                                    // 2 in SoundEntries.dbc or 0
    float       EndPosX;                                    // 3
    float       EndPosY;                                    // 4
    float       EndPosZ;                                    // 5
    float       GlobalRotation;                             // 6
};

struct CinematicSequencesEntry
{
    uint32      Id;                                         // 0 index
    //uint32      unk1;                                     // 1 always 0
    uint32      cinematicCamera;                            // 2 id in CinematicCamera.dbc
};

struct CreatureDisplayInfoEntry
{
    //uint32      Displayid;                                // 0        m_ID
    uint32      ModelId;                                    // 1        m_modelID
                                                            // 2        m_soundID
                                                            // 3        m_extendedDisplayInfoID
    float       scale;                                      // 4        m_creatureModelScale
                                                            // 5        m_creatureModelAlpha
                                                            // 6-8      m_textureVariation[3]
                                                            // 9        m_portraitTextureName
                                                            // 10       m_sizeClass
                                                            // 11       m_bloodID
                                                            // 12       m_NPCSoundID
                                                            // 13       m_particleColorID
                                                            // 14       m_creatureGeosetData
                                                            // 15       m_objectEffectPackageID
                                                            // 16
                                                            // 17 - Panadria
                                                            // 18 - Pandaria
};

struct CreatureFamilyEntry
{
    uint32  ID;                                             // 0        m_ID
    float   minScale;                                       // 1        m_minScale
    uint32  minScaleLevel;                                  // 2        m_minScaleLevel
    float   maxScale;                                       // 3        m_maxScale
    uint32  maxScaleLevel;                                  // 4        m_maxScaleLevel
    uint32  skillLine[2];                                   // 5-6      m_skillLine
    uint32  petFoodMask;                                    // 7        m_petFoodMask
    int32   petTalentType;                                  // 8        m_petTalentType
                                                            // 9        m_categoryEnumID
    char* Name;                                             // 10       m_name_lang
                                                            // 11       m_iconFile
};

struct CreatureModelDataEntry
{
    //uint32 Id;
    //uint32 Flags;
    //char* ModelPath
    //uint32 Unk1;
    //float Scale;                                             // Used in calculation of unit collision data
    //int32 Unk2
    //int32 Unk3
    //uint32 Unk4
    //uint32 Unk5
    //float Unk6
    //uint32 Unk7
    //float Unk8
    //uint32 Unk9
    //uint32 Unk10
    //float CollisionWidth;
    float CollisionHeight;
    float MountHeight;                                       // Used in calculation of unit collision data when mounted
    //float Unks[11]
    //uint32 unk11
    //uint32 unk12
};

#define MAX_CREATURE_SPELL_DATA_SLOT 4

struct CreatureSpellDataEntry
{
    //uint32    ID;                                           // 0        m_ID
    uint32    spellId[MAX_CREATURE_SPELL_DATA_SLOT];        // 1-4      m_spells[4]
    //uint32    availability[MAX_CREATURE_SPELL_DATA_SLOT]; // 4-7      m_availability[4]
};

struct CreatureTypeEntry
{
    uint32    ID;                                           // 0        m_ID
    //char*   Name;                                         // 1        m_name_lang
    //uint32    no_expirience;                              // 2        m_flags no exp? critters, non-combat pets, gas cloud.
};

/* not used
struct CurrencyCategoryEntry
{
    uint32    ID;                                           // 0
    uint32    Unk1;                                         // 1        0 for known categories and 3 for unknown one (3.0.9)
    char*   Name[16];                                       // 2-17     name
    //                                                      // 18       string flags
};
*/

struct CriteriaEntry
{
    uint32 ID;                                              // 0
    uint32 type;                                            // 1
union
    {
        // ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE          = 0
        /// @todo also used for player deaths..
        struct
        {
            uint32  creatureID;                             // 2
        } kill_creature;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_BG                 = 1
        struct
        {
            uint32  bgMapID;                                // 2
        } win_bg;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL      = 7
        struct
        {
            uint32  skillID;                                // 2
        } reach_skill_level;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT   = 8
        struct
        {
            uint32  linkedAchievement;                      // 2
        } complete_achievement;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE = 11
        struct
        {
            uint32  zoneID;                                 // 2
        } complete_quests_in_zone;

        // ACHIEVEMENT_CRITERIA_TYPE_CURRENCY = 12
        struct
        {
            uint32 currency;
        } currencyGain;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND  = 15
        struct
        {
            uint32  mapID;                                  // 2
        } complete_battleground;

        // ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP           = 16
        struct
        {
            uint32  mapID;                                  // 2
        } death_at_map;

        // ACHIEVEMENT_CRITERIA_TYPE_DEATH_IN_DUNGEON       = 18
        struct
        {
            uint32  manLimit;                               // 2
        } death_in_dungeon;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_RAID          = 19
        struct
        {
            uint32  groupSize;                              // 2 can be 5, 10 or 25
        } complete_raid;

        // ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE     = 20
        struct
        {
            uint32  creatureEntry;                          // 2
        } killed_by_creature;

        // ACHIEVEMENT_CRITERIA_TYPE_DEATHS_FROM            = 26
        struct
        {
            uint32 type;                                    // 2, see enum EnviromentalDamage
        } death_from;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST         = 27
        struct
        {
            uint32  questID;                                // 2
        } complete_quest;

        // ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET        = 28
        // ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2       = 69
        struct
        {
            uint32  spellID;                                // 2
        } be_spell_target;

        // ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL             = 29
        // ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2            = 110
        struct
        {
            uint32  spellID;                                // 2
        } cast_spell;

        // ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE
        struct
        {
            uint32 objectiveId;                             // 2
        } bg_objective;

        // ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA = 31
        struct
        {
            uint32  areaID;                                 // 2 Reference to AreaTable.dbc
        } honorable_kill_at_area;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA              = 32
        struct
        {
            uint32 mapID;                                   // 2 Reference to Map.dbc
        } win_arena;

        // ACHIEVEMENT_CRITERIA_TYPE_PLAY_ARENA             = 33
        struct
        {
            uint32  mapID;                                  // 2 Reference to Map.dbc
        } play_arena;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL            = 34
        struct
        {
            uint32  spellID;                                // 2 Reference to Map.dbc
        } learn_spell;

        // ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM               = 36
        struct
        {
            uint32  itemID;                                 // 2
        } own_item;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_TEAM_RATING    = 38
        struct
        {
            uint32  teamtype;                               // 2 {2, 3, 5}
        } highest_team_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_TEAM_RATING      = 39
        struct
        {
            uint32  teamtype;                               // 2 {2, 3, 5}
        } reach_team_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_PERSONAL_RATING = 39
        struct
        {
            uint32 teamtype; // 2 {2, 3, 5}
        } highest_personal_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL      = 40
        struct
        {
            uint32  skillID;                                // 2
        } learn_skill_level;

        // ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM               = 41
        struct
        {
            uint32  itemID;                                 // 2
        } use_item;

        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM              = 42
        struct
        {
            uint32  itemID;                                 // 2
        } loot_item;

        // ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA           = 43
        struct
        {
            /// @todo This rank is _NOT_ the index from AreaTable.dbc
            uint32  areaReference;                          // 2
        } explore_area;

        // ACHIEVEMENT_CRITERIA_TYPE_OWN_RANK               = 44
        struct
        {
            /// @todo This rank is _NOT_ the index from CharTitles.dbc
            uint32  rank;                                   // 2
        } own_rank;

        // ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION        = 46
        struct
        {
            uint32  factionID;                              // 2
        } gain_reputation;

        // ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM        = 49
        /// @todo where is the required itemlevel stored?
        struct
        {
            uint32  itemSlot;                               // 2
        } equip_epic_item;

        // ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT      = 50
        struct
        {
            uint32  rollValue;                              // 2
        } roll_need_on_loot;
       // ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED_ON_LOOT      = 51
        struct
        {
            uint32  rollValue;                              // 2
        } roll_greed_on_loot;

        // ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS               = 52
        struct
        {
            uint32  classID;                                // 2
        } hk_class;

        // ACHIEVEMENT_CRITERIA_TYPE_HK_RACE                = 53
        struct
        {
            uint32  raceID;                                 // 2
        } hk_race;

        // ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE               = 54
        /// @todo where is the information about the target stored?
        struct
        {
            uint32  emoteID;                                // 2 enum TextEmotes
        } do_emote;

        // ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM             = 57
        struct
        {
            uint32  itemID;                                 // 2
        } equip_item;

        // ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT         = 68
        struct
        {
            uint32  goEntry;                                // 2
        } use_gameobject;

        // ACHIEVEMENT_CRITERIA_TYPE_FISH_IN_GAMEOBJECT     = 72
        struct
        {
            uint32  goEntry;                                // 2
        } fish_in_gameobject;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILLLINE_SPELLS = 75
        struct
        {
            uint32  skillLine;                              // 2
        } learn_skillline_spell;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_POWER          = 96
        struct
        {
            uint32  powerType;                              // 2 mana=0, 1=rage, 3=energy, 6=runic power
        } highest_power;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_STAT           = 97
        struct
        {
            uint32  statType;                               // 2 4=spirit, 3=int, 2=stamina, 1=agi, 0=strength
        } highest_stat;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_SPELLPOWER     = 98
        struct
        {
            uint32  spellSchool;                            // 2
        } highest_spellpower;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_RATING         = 100
        struct
        {
            uint32  ratingType;                             // 2
        } highest_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE              = 109
        struct
        {
            uint32  lootType;                               // 2 3=fishing, 2=pickpocket, 4=disentchant
        } loot_type;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LINE       = 112
        struct
        {
            uint32  skillLine;                              // 2
        } learn_skill_line;

        struct
        {
            uint32 criteriaArg1;                            // 2
        } raw;
    };

    uint32 timedCriteriaStartType;                          // 3
    uint32 timedCriteriaMiscId;                             // 4
    uint32 timeLimit;                                       // 5
    //uint32 unk;                                           // 6
    //uint32 unk;                                           // 7
    uint32 modifierTreeId;                                  // 8
    //uint32 completionFlag;                                // 9
    //uint32 worldStateId;                                  // 10
    //uint32 worldStateValue;                               // 11
};

struct CriteriaTreeEntry
{
    uint32 ID;                                              // 0
    uint32 criteriaID;                                      // 1
    uint32 criteriaCount;                                   // 2
    //uint32 unk;                                           // 3
    //uint32 op;                                            // 4
    uint32 parentID;                                        // 5
    //uint32 treeFlags;                                     // 6
    //char* name;                                           // 7
};

struct CurrencyTypesEntry
{
    uint32 ID;                                              // 0        not used
    uint32 Category;                                        // 1        may be category
    //char* name;                                           // 2
    //char* iconName;                                       // 3
    //uint32 unk4;                                          // 4        all 0
    //uint32 HasSubstitution;                               // 5        archaeology-related (?)
    //uint32 SubstitutionId;                                // 6
    uint32 TotalCap;                                        // 7
    uint32 WeekCap;                                         // 8
    uint32 Flags;                                           // 9
    //uint32 unk5                                           // 10 - Pandaria
    //char* description;                                    // 11
};

struct DestructibleModelDataEntry
{
    //uint32  Id;
    uint32  DamagedDisplayId; 
    //uint32  DamagedUnk1;
    //uint32  DamagedUnk2;
    //uint32  DamagedUnk3;
    uint32  DestroyedDisplayId;
    //uint32  DestroyedUnk1;
    //uint32  DestroyedUnk2;
    //uint32  DestroyedUnk3;
    //uint32  DestroyedUnk4;
    uint32  RebuildingDisplayId;
    //uint32  RebuildingUnk1;
    //uint32  RebuildingUnk2;
    //uint32  RebuildingUnk3;
    //uint32  RebuildingUnk4;
    //uint32  SmokeDisplayId;
    //uint32  SmokeUnk1;
    //uint32  SmokeUnk2;
    //uint32  SmokeUnk3;
    //uint32  SmokeUnk4;
    //uint32  UnkDisplayid;
    //uint32  Unk6;
    //uint32  Unk7;
    //uint32  Unk8;
};

struct DifficultyEntry
{
    uint32 DiffID;
    uint32 DownscaleID;
    uint32 maptype;
    //uint32
    //uint32
    //uint32 SpawnMode;
    uint32 flags;
    //
    //
    //
    //
    //
};
/*
enum diffflag
{
    DIFFICULTY_FLAG_HEROIC = 0x01,
    DIFFICULTY_FLAG_DEFAULT = 0x02,
    DIFFICULTY_FLAG_CAN_SELECT = 0x04, // Player can select this difficulty in dropdown menu
    DIFFICULTY_FLAG_CHALLENGE_MODE = 0x08,
};
*/
struct DungeonEncounterEntry
{
    //uint32 id;                                            // 0        unique id
    uint32 mapId;                                           // 1        map id
    int32 difficulty;                                       // 2        instance mode
    //uint32 unk0;                                          // 3
    uint32 encounterIndex;                                  // 4        encounter index for creating completed mask
    char* encounterName;                                    // 5        encounter name
    //uint32 nameFlags;                                     // 6
    //uint32 unk1;                                          // 7
    //uint32 unk2;                                          // 8 - Pandaria
};

struct DurabilityCostsEntry
{
    //uint32    Itemlvl;                                    // 0
    uint32    multiplier[29];                               // 1-29
};

struct DurabilityQualityEntry
{
    //uint32    Id;                                         // 0
    float     quality_mod;                                  // 1
};

struct EmotesEntry
{
    uint32  Id;                                             // 0
    //char* Name;                                           // 1, internal name
    //uint32 AnimationId;                                   // 2, ref to animationData
    //uint32  Flags;                                          // 3, bitmask, may be unit_flags
    //uint32  EmoteType;                                      // 4, Can be 0, 1 or 2 (determine how emote are shown)
    //uint32  UnitStandState;                                 // 5, uncomfirmed, may be enum UnitStandStateType
    //uint32 SoundId;                                       // 6, ref to soundEntries
    //uint32 unk7                                           // 7
};

struct EmotesTextEntry
{
    //uint32  Id;
    uint32  textid;
};

struct FactionEntry
{
    uint32      ID;                                         // 0        m_ID
    int32       reputationListID;                           // 1        m_reputationIndex
    uint32      BaseRepRaceMask[4];                         // 2-5      m_reputationRaceMask
    uint32      BaseRepClassMask[4];                        // 6-9      m_reputationClassMask
    int32       BaseRepValue[4];                            // 10-13    m_reputationBase
    uint32      ReputationFlags[4];                         // 14-17    m_reputationFlags
    uint32      team;                                       // 18       m_parentFactionID
    float       spilloverRateIn;                            // 19       Faction gains incoming rep * spilloverRateIn
    float       spilloverRateOut;                           // 20       Faction outputs rep * spilloverRateOut as spillover reputation
    uint32      spilloverMaxRankIn;                         // 21       The highest rank the faction will profit from incoming spillover
    //uint32    spilloverRank_unk;                          // 22       It does not seem to be the max standing at which a faction outputs spillover ...so no idea
    char*       name;                                       // 23       m_name_lang
    //char*     description;                                // 24       m_description_lang
    //uint32    GroupExpansion;                             // 25       m_factionGroupExpansion
    //uint      unk1                                        // 26 - Pandaria
    //uint      unk2                                        // 27 - Pandaria

    // helpers
    bool CanHaveReputation() const
    {
        return reputationListID >= 0;
    }
};

#define MAX_FACTION_RELATIONS 4

struct FactionTemplateEntry
{
    uint32 ID;                                              // 0    m_ID
    uint32 faction;                                         // 1    m_faction
    uint32 factionFlags;                                    // 2    m_flags
    uint32 ourMask;                                         // 3    m_factionGroup
    uint32 friendlyMask;                                    // 4    m_friendGroup
    uint32 hostileMask;                                     // 5    m_enemyGroup
    uint32 enemyFaction[MAX_FACTION_RELATIONS];             // 6    m_enemies[MAX_FACTION_RELATIONS]
    uint32 friendFaction[MAX_FACTION_RELATIONS];            // 10   m_friend[MAX_FACTION_RELATIONS]

    // helpers
    bool IsFriendlyTo(FactionTemplateEntry const& entry) const
    {
        if (ID == entry.ID)
            return true;
        if (entry.faction)
        {
            for (int i = 0; i < MAX_FACTION_RELATIONS; ++i)
                if (enemyFaction[i] == entry.faction)
                    return false;
            for (int i = 0; i < MAX_FACTION_RELATIONS; ++i)
                if (friendFaction[i] == entry.faction)
                    return true;
        }
        return (friendlyMask & entry.ourMask) || (ourMask & entry.friendlyMask);
    }
    bool IsHostileTo(FactionTemplateEntry const& entry) const
    {
        if (ID == entry.ID)
            return false;
        if (entry.faction)
        {
            for (int i = 0; i < MAX_FACTION_RELATIONS; ++i)
                if (enemyFaction[i] == entry.faction)
                    return true;
            for (int i = 0; i < MAX_FACTION_RELATIONS; ++i)
                if (friendFaction[i] == entry.faction)
                    return false;
        }
        return (hostileMask & entry.ourMask) != 0;
    }
    bool IsHostileToPlayers() const { return (hostileMask & FACTION_MASK_PLAYER) !=0; }
    bool IsNeutralToAll() const
    {
        for (int i = 0; i < MAX_FACTION_RELATIONS; ++i)
            if (enemyFaction[i] != 0)
                return false;
        return hostileMask == 0 && friendlyMask == 0;
    }
    bool IsContestedGuardFaction() const { return (factionFlags & FACTION_TEMPLATE_FLAG_CONTESTED_GUARD) != 0; }
};

struct GameObjectDisplayInfoEntry
{
    //uint32   Displayid;                                // 0    m_ID
    //char*    filename;                                 // 1
    //uint32 unk1[10];                                   //2-11
    float    minX;
    float    minY;
    float    minZ;
    float    maxX;
    float    maxY;
    float    maxZ;
    //uint32  transport;                                 //18
};

struct GemPropertiesEntry
{
    //uint32      ID;                                       // 0        m_id
    uint32      spellitemenchantement;                      // 1        m_enchant_id
                                                            // 2        m_maxcount_inv
                                                            // 3        m_maxcount_item
    uint32      color;                                      // 4        m_type
    //uint32      minJewelCraftingSkill;                    // 5        m_minJewelCraftingSkill
};

struct GlyphPropertiesEntry
{
    //uint32  Id;
    uint32  SpellId;
    uint32  TypeFlags;
    //uint32  IconId;                                       // GlyphIconId (SpellIcon.dbc)
};

struct GlyphSlotEntry
{
    uint32  Id;
    uint32  TypeFlags;
    //uint32  Order;
};

// All Gt* DBC store data for 100 levels, some by 100 per class/race
#define GT_MAX_LEVEL    100
// gtOCTClassCombatRatingScalar.dbc stores data for 32 ratings, look at MAX_COMBAT_RATING for real used amount
#define GT_MAX_RATING   32

struct GtBarberShopCostBaseEntry
{
    //uint32 level;
    float   cost;
};

struct GtCombatRatingsEntry
{
    //uint32 level;
    float    ratio;
};

struct GtChanceToMeleeCritBaseEntry
{
    //uint32 level;
    float    base;
};

struct GtChanceToMeleeCritEntry
{
    //uint32 level;
    float    ratio;
};

struct GtChanceToSpellCritBaseEntry
{
    float    base;
};

struct GtNPCManaCostScalerEntry
{
    float    ratio;
};

struct GtChanceToSpellCritEntry
{
    float    ratio;
};

struct GtOCTClassCombatRatingScalarEntry
{
    float    ratio;
};

struct gtOCTHpPerStaminaEntry
{
    float    ratio;
};

/*
struct GtRegenHPPerSptEntry
{
    float    ratio;
};
*/
struct GtRegenMPPerSptEntry
{
    float    ratio;
};

struct GtSpellScalingEntry
{
    float value;
};

struct GtOCTBaseHPByClassEntry
{
    float ratio;
};

struct GtOCTBaseMPByClassEntry
{
    float ratio;
};

struct GuildPerkSpellsEntry
{
    //uint32 Id;
    uint32 Level;
    uint32 SpellId;
};

/* no used
struct HolidayDescriptionsEntry
{
    uint32 ID;                                              // 0, m_holidayDescriptionID
    //char*     name                                        // 1  m_name_lang
};
*/

/* no used
struct HolidayNamesEntry
{
    uint32 ID;                                              // 0, m_holidayNameID
    //char*     name                                        // 1  m_name_lang
};
*/

#define MAX_HOLIDAY_DURATIONS 10
#define MAX_HOLIDAY_DATES 26
#define MAX_HOLIDAY_FLAGS 10

struct HolidaysEntry
{
    uint32 Id;                                              // 0        m_ID
    //uint32 Duration[MAX_HOLIDAY_DURATIONS];               // 1-10     m_duration
    //uint32 Date[MAX_HOLIDAY_DATES];                       // 11-36    m_date (dates in unix time starting at January, 1, 2000)
    //uint32 Region;                                        // 37       m_region (wow region)
    //uint32 Looping;                                       // 38       m_looping
    //uint32 CalendarFlags[MAX_HOLIDAY_FLAGS];              // 39-48    m_calendarFlags
    //uint32 holidayNameId;                                 // 49       m_holidayNameID (HolidayNames.dbc)
    //uint32 holidayDescriptionId;                          // 50       m_holidayDescriptionID (HolidayDescriptions.dbc)
    //char* TextureFilename;                                // 51       m_textureFilename
    //uint32 Priority;                                      // 52       m_priority
    //uint32 CalendarFilterType;                            // 53       m_calendarFilterType (-1 = Fishing Contest, 0 = Unk, 1 = Darkmoon Festival, 2 = Yearly holiday)
    //uint32 flags;                                         // 54       m_flags (0 = Darkmoon Faire, Fishing Contest and Wotlk Launch, rest is 1)
};

// ImportPriceArmor.dbc
struct ImportPriceArmorEntry
{
    //uint32 InventoryType;                                 // 1        Id/InventoryType
    float ClothFactor;                                      // 2        Price factor cloth
    float LeatherFactor;                                    // 3        Price factor leather
    float MailFactor;                                       // 4        Price factor mail
    float PlateFactor;                                      // 5        Price factor plate
};

// ImportPriceQuality.dbc
struct ImportPriceQualityEntry
{
    //uint32 QualityId;                                     // 1        Quality Id (+1?)
    float Factor;                                           // 2        Price factor
};

// ImportPriceShield.dbc
struct ImportPriceShieldEntry
{
    //uint32 Id;                                            // 1        Unk id (only 1 and 2)
    float Factor;                                           // 2        Price factor
};

// ImportPriceWeapon.dbc
struct ImportPriceWeaponEntry
{
    //uint32 Id;                                            // 1        Unk id (mainhand - 0, offhand - 1, weapon - 2, 2hweapon - 3, ranged/rangedright/relic - 4)
    float Factor;                                           // 2        Price factor
};

// ItemPriceBase.dbc
struct ItemPriceBaseEntry
{
    //uint32 id
    //uint32 ItemLevel;                                       // 2        Item level (1 - 1000)
    float ArmorFactor;                                      // 3        Price factor for armor
    float WeaponFactor;                                     // 4        Price factor for weapons
};

struct ItemReforgeEntry
{
    //uint32 Id;
    uint32 SourceStat;
    float SourceMultiplier;
    uint32 FinalStat;
    float FinalMultiplier;
};

struct ItemArmorQualityEntry
{
    //uint32    Id;                                             // 0 item level
    float     Value[7];                                       // 1-7 multiplier for item quality
    //uint32    Id2;                                            // 8 item level
};

struct ItemArmorShieldEntry
{
    //uint32    Id;                                             // 0 item level
    //uint32    Id2;                                            // 1 item level
    float     Value[7];                                       // 2-8 multiplier for item quality
};

struct ItemArmorTotalEntry
{
    //uint32    Id;                                             // 0 item level
    //uint32    Id2;                                            // 1 item level
    float     Value[4];                                       // 2-5 multiplier for armor types (cloth...plate)
};

// ItemClass.dbc
struct ItemClassEntry
{
    //uint32    Class;                                          // 1 item class id
    //uint32  IsWeapon;                                       // 2 1 for weapon, 0 for everything else
    float     PriceFactor;                                    // 3 used to calculate certain prices
    //char*   Name;                                           // 4 class name
};

// common struct for:
// ItemDamageAmmo.dbc
// ItemDamageOneHand.dbc
// ItemDamageOneHandCaster.dbc
// ItemDamageRanged.dbc
// ItemDamageThrown.dbc
// ItemDamageTwoHand.dbc
// ItemDamageTwoHandCaster.dbc
// ItemDamageWand.dbc
struct ItemDamageEntry
{
    //uint32    Id;                                             // 0 item level
    float     DPS[7];                                         // 1-7 multiplier for item quality
    //uint32    Id2;                                            // 8 item level
};

/*
struct ItemBagFamilyEntry
{
    uint32   ID;                                            // 0
    //char*     name;                                       // 1        m_name_lang
};

struct ItemDisplayInfoEntry
{
    uint32      ID;                                         // 0        m_ID
                                                            // 1        m_modelName[2]
                                                            // 2        m_modelTexture[2]
                                                            // 3        m_inventoryIcon
                                                            // 4        m_geosetGroup[3]
                                                            // 5        m_flags
                                                            // 6        m_spellVisualID
                                                            // 7        m_groupSoundIndex
                                                            // 8        m_helmetGeosetVis[2]
                                                            // 9        m_texture[2]
                                                            // 10       m_itemVisual[8]
                                                            // 11       m_particleColorID
};
*/
struct ItemDisenchantLootEntry
{
    uint32 Id;
    uint32 ItemClass;
    //int32 ItemSubClass;
    uint32 ItemQuality;
    uint32 MinItemLevel;
    uint32 MaxItemLevel;
    uint32 RequiredDisenchantSkill;
};

struct ItemLimitCategoryEntry
{
    //uint32      ID;                                       // 0 Id
    //char*     name;                                       // 1        m_name_lang
    uint32      maxCount;                                   // 2,       m_quantity max allowed equipped as item or in gem slot
    uint32      mode;                                       // 3,       m_flags 0 = have, 1 = equip (enum ItemLimitCategoryMode)
};

#define MAX_ITEM_ENCHANTMENT_EFFECTS 3

struct ItemRandomPropertiesEntry
{
    uint32    ID;                                           // 0        m_ID
    //char* internalName                                    // 1        m_Name
    uint32    enchant_id[MAX_ITEM_ENCHANTMENT_EFFECTS];     // 2-4      m_Enchantment
                                                            // 5-6      unused
    char* nameSuffix;                                       // 7        m_name_lang
};

struct ItemRandomSuffixEntry
{
    uint32    ID;                                           // 0        m_ID
    char* nameSuffix;                                       // 1        m_name_lang
                                                            // 2        m_internalName
    uint32    enchant_id[5];                                // 3-7      m_enchantment
    uint32    prefix[5];                                    // 8-12     m_allocationPct
};

//#define MAX_ITEM_SET_ITEMS 17
#define MAX_ITEM_SET_SPELLS 8

struct ItemSetEntry
{
    //uint32    id                                          // 0        m_ID
    char* name;                                             // 1        m_name_lang
    //uint32    itemId[MAX_ITEM_SET_ITEMS];                 // 2-18     m_itemID
    uint32    spells[MAX_ITEM_SET_SPELLS];                  // 19-26    m_setSpellID
    uint32    items_to_triggerspell[MAX_ITEM_SET_SPELLS];   // 27-34    m_setThreshold
    uint32    required_skill_id;                            // 35       m_requiredSkill
    uint32    required_skill_value;                         // 36       m_requiredSkillRank
};

struct LFGDungeonEntry
{
    uint32  m_ID;                                           // 0
    char*   m_Name;                                         // 1
    uint32  m_MinLevel;                                     // 2
    uint32  m_MaxLevel;                                     // 3
    uint32  m_TargetLevel;                                  // 4
    //uint32  m_TargetMinLevel;                               // 5
    //uint32  m_TargetMaxLevel;                               // 6
    int32   m_ContinentID;                                  // 7
    uint32  m_DifficultyID;                                 // 8 Difficulty.dbc
    uint32  m_Flags;                                        // 9
    uint32  m_Type;                                         // 10 (1-Dungeon, 2-Raid, 4-OpenWorld, 6-Random)
    //uint32  m_Faction;                                    // 11
    //char*   m_TextureFileName;                            // 12
    uint32  m_ExpansionLevel;                               // 13
    //uint32  m_OrderIndex;                                 // 14
    uint32  m_GroupID;                                      // 15
    //char*   m_Description;                                // 16
    //uint32  m_RandomDungeonID;                              // 17 RandomDungeonID assigned for this dungeon
    //uint32 m_CountTank;                                   // 18
    //uint32 m_CountHealer;                                 // 19
    //uint32 m_CountDamage;                                 // 20
    //uint32 m_CountTankMin;                                // 21
    //uint32 m_CountHealerMin;                              // 22
    //uint32 m_CountDamageMin;                              // 23
    //uint32 m_ScenarioID;                                  // 24
    //uint32 m_SubType;                                     // 25
    //uint32 m_BonusReputationAmount;                       // 26
    //uint32 m_MentorLevel;                                 // 27
    //uint32 m_MentorItemLevel;                             // 28

    // Helpers
    uint32 Entry() const { return m_ID + (m_Type << 24); }
};


struct LiquidTypeEntry
{
    uint32 Id;
    //char*  Name;
    //uint32 Flags;
    uint32 Type;
    //uint32 SoundId;
    uint32 SpellId;
    //float MaxDarkenDepth;
    //float FogDarkenIntensity;
    //float AmbDarkenIntensity;
    //float DirDarkenIntensity;
    //uint32 LightID;
    //float ParticleScale;
    //uint32 ParticleMovement;
    //uint32 ParticleTexSlots;
    //uint32 LiquidMaterialID;
    //char* Texture[6];
    //uint32 Color[2];
    //float Unk1[18];
    //uint32 Unk2[4];
};

#define MAX_LOCK_CASE 8

struct LockEntry
{
    //uint32      ID;                                       // 0        m_ID
    uint32      Type[MAX_LOCK_CASE];                        // 1-8      m_Type
    uint32      Index[MAX_LOCK_CASE];                       // 9-16     m_Index
    uint32      Skill[MAX_LOCK_CASE];                       // 17-24    m_Skill
    //uint32      Action[MAX_LOCK_CASE];                    // 25-32    m_Action
};

struct MailTemplateEntry
{
    //uint32      ID;                                       // 0
    //char*       subject;                                  // 1        m_subject_lang
    char* content;                                          // 2        m_body_lang
};

struct MapEntry
{
    uint32  MapID;                                          // 0
    //char*       internalname;                             // 1 unused
    uint32  map_type;                                       // 2
    //uint32 flags;                                         // 3
    //uint32 isPvp;                                         // 4
    char* name;                                             // 5        m_MapName_lang
    uint32  linked_zone;                                    // 6        m_areaTableID
    //char*     hordeIntro;                                 // 7        m_MapDescription0_lang
    //char*     allianceIntro;                              // 8        m_MapDescription1_lang
    //uint32  multimap_id;                                  // 9        m_LoadingScreenID (LoadingScreens.dbc)
    //float   BattlefieldMapIconScale;                      // 10       m_minimapIconScale
    int32   entrance_map;                                   // 11       m_corpseMapID map_id of entrance map in ghost mode (continent always and in most cases = normal entrance)
    float   entrance_x;                                     // 12       m_corpseX entrance x coordinate in ghost mode  (in most cases = normal entrance)
    float   entrance_y;                                     // 13       m_corpseY entrance y coordinate in ghost mode  (in most cases = normal entrance)
    //uint32  timeOfDayOverride;                            // 14       m_timeOfDayOverride
    uint32  addon;                                          // 15       m_expansionID
    //uint32  expireTime;                                   // 16       m_raidOffset
    //uint32 maxPlayers;                                    // 17       m_maxPlayers
    int32 rootPhaseMap;                                     // 18 new 4.0.0, mapid, related to phasing

    // Helpers
    uint32 Expansion() const { return addon; }

    bool IsDungeon() const { return map_type == MAP_DUNGEON; }
    bool IsNonRaidInstance() const { return map_type == MAP_DUNGEON || map_type == MAP_SCENARIO; }
    bool Instanceable() const { return map_type == MAP_DUNGEON || map_type == MAP_RAID || map_type == MAP_BATTLEGROUND || map_type == MAP_ARENA || map_type == MAP_SCENARIO; }
    bool IsRaid() const { return map_type == MAP_RAID; }
    bool IsBattleground() const { return map_type == MAP_BATTLEGROUND; }
    bool IsBattleArena() const { return map_type == MAP_ARENA; }
    bool IsScenario() const { return map_type == MAP_SCENARIO; }
    bool IsBattlegroundOrArena() const { return map_type == MAP_BATTLEGROUND || map_type == MAP_ARENA; }
    bool IsWorldMap() const { return map_type == MAP_COMMON; }

    bool IsInstance() const { return map_type == MAP_DUNGEON || map_type == MAP_RAID; }

    bool GetEntrancePos(int32 &mapid, float &x, float &y) const
    {
        if (entrance_map < 0)
            return false;
        mapid = entrance_map;
        x = entrance_x;
        y = entrance_y;
        return true;
    }

    bool IsContinent() const
    {
        return MapID == 0 || MapID == 1 || MapID == 530 || MapID == 571 || MapID == 860 || MapID == 870;
    }
};

struct MapDifficultyEntry
{
    //uint32      Id;                                       // 0
    uint32      MapId;                                      // 1
    uint32      Difficulty;                                 // 2 (for arenas: arena slot)
    char*       areaTriggerText;                            // 3        m_message_lang (text showed when transfer to map failed)
    uint32      resetTime;                                  // 4,       m_raidDuration in secs, 0 if no fixed reset time
    uint32      maxPlayers;                                 // 5,       m_maxPlayers some heroic versions have 0 when expected same amount as in normal version
    //char*       difficultyString;                         // 6        m_difficultystring
};


struct ModifierTreeEntry
{
    uint32 ID;                                              // 0
    uint32 conditionType;                                   // 1
    uint32 conditionValue[2];                               // 2 - 3
    //uint32 op;                                              // 4
    // uint32 unk;                                          // 5
    uint32 parent;                                          // 6
};

struct MountCapabilityEntry
{
    uint32 Id;
    uint32 Flags;
    uint32 RequiredRidingSkill;
    uint32 RequiredArea;
    uint32 RequiredAura;
    uint32 RequiredSpell;
    uint32 SpeedModSpell;
    int32  RequiredMap;
};

#define MAX_MOUNT_CAPABILITIES 24

struct MountTypeEntry
{
    //uint32 Id;
    uint32 MountCapability[MAX_MOUNT_CAPABILITIES];
};

struct MovieEntry
{
    uint32      Id;                                         // 0 index
    //char*       filename;                                 // 1
    //uint32      unk1;                                     // 2        m_volume
    //uint32      unk2;                                     // 3 4.0.0
};

struct NameGenEntry
{
    //uint32 id;
    char*  name;
    uint32 race;
    uint32 gender;
};

#define MAX_OVERRIDE_SPELL 10

struct OverrideSpellDataEntry
{
    //uint32      id;                                         // 0
    uint32      spellId[MAX_OVERRIDE_SPELL];                // 1-10
    //uint32      unk0;                                     // 11
    //char*     SpellBarName;                               // 12
};

struct PhaseEntry
{
    uint32 ID;                                            // 0
    //char* Name;                                         // 1
    //uint32 flag;                                        // 2
};

struct PhaseGroupEntry
{
    //uint32 ID;
    uint32 PhaseId;
    uint32 GroupId;
};

struct PvPDifficultyEntry
{
    //uint32      id;                                       // 0        m_ID
    uint32      mapId;                                      // 1
    uint32      bracketId;                                  // 2
    uint32      minLevel;                                   // 3
    uint32      maxLevel;                                   // 4
    uint32      difficulty;                                 // 5

    // helpers
    BattlegroundBracketId GetBracketId() const { return BattlegroundBracketId(bracketId); }
};

struct QuestFactionRewEntry
{
    //uint32      id;
    int32       QuestRewFactionValue[10];
};

struct QuestPOIPointEntry
{
    uint32 Id;                                              // 0
    //int32 X;                                              // 1
    //int32 Y;                                              // 2
    //uint32 BlobId;                                        // 3
};

struct QuestSortEntry
{
    uint32      id;                                         // 0        m_ID
    //char*       name;                                     // 1        m_SortName_lang
};

struct QuestXPEntry
{
    //uint32      id;
    uint32      Exp[10];
};

struct RandomPropertiesPointsEntry
{
    //uint32  Id;                                           // 0 hidden key
    uint32    EpicPropertiesPoints[5];                      // 1-5
    uint32    RarePropertiesPoints[5];                      // 6-10
    uint32    UncommonPropertiesPoints[5];                  // 11-15
};
/*
struct ResearchBranchEntry
{
    uint32 Id;                                            // 0
    //char* BranchName;                                   // 1
    //uint32 ResearchFieldId;                             // 2 research field (from ResearchField.dbc)
    //uint32 FragmentCurrencyId;                          // 3
    //char* Icon;                                         // 4
    //uint32 KeystoneItemId;                              // 5
};

//struct ResearchFieldEntry
//{
//    uint32 Id;                                            // 0
//    char* FieldName;                                      // 1
//    uint32 slot;                                          // 2
//};

struct ResearchProjectEntry
{
    uint32 Id;                                              // 0
    //char* ProjectName;                                    // 1
    //char* ProjectDescription;                             // 2
    uint32 Rarity;                                          // 3 0-common, 1-rare
    uint32 ResearchBranchId;                                // 4 branch id (from ResearchBranch.dbc)
    //uint32 SpellId;                                       // 5
    //uint32 KeystoneCount;                                 // 6
    //char* ProjectIcon;                                    // 7
    uint32 RequiredFragmentCount;                           // 8
};

struct ResearchSiteEntry
{
    uint32 Id;                                              // 0
    uint32 MapId;                                           // 1
    uint32 QuestPOIBlobId;                                  // 2 blob id (from QuestPOIBlob.dbc)
    //char* SiteName;                                       // 3
    //uint32 IconId;                                        // 4
};
*/
struct ScalingStatDistributionEntry
{
    //uint32  Id;                                           // 0
    int32   StatMod[10];                                    // 1-10
    uint32  Modifier[10];                                   // 11-20
    //uint32 unk1;                                          // 21
    uint32  MaxLevel;                                       // 22       m_maxlevel
};

struct ScalingStatValuesEntry
{
    //uint32 Id;                                            // 0
    //uint32 Level;                                         // 1
    //uint32 ilvl;
    uint32 dpsMod[6];                                       // 2-8 DPS mod for level
    uint32 Spellpower;                                      // 9 spell power for level
    uint32 StatMultiplier[5];                               // 10-11 Multiplier for ScalingStatDistribution
    uint32 Armor[8][4];                                     // 12-47 Armor for level
    uint32 CloakArmor;                                      // 48 armor for cloak
    uint32 ShieldArmor;                                     // 49 - Pandaria

    uint32 GetStatMultiplier(uint32 inventoryType) const;
    uint32 GetArmor(uint32 inventoryType, uint32 armorType) const;
    uint32 GetDPSAndDamageMultiplier(uint32 subClass, bool isCasterWeapon, float* damageMultiplier) const;
};

//struct SkillLineCategoryEntry{
//    uint32    id;                                         // 0      m_ID
//    char*     name[16];                                   // 1-17   m_name_lang
//                                                          // 18 string flag
//    uint32    displayOrder;                               // 19     m_sortIndex
//};

//struct SkillRaceClassInfoEntry{
//    uint32    id;                                         // 0      m_ID
//    uint32    skillId;                                    // 1      m_skillID
//    uint32    raceMask;                                   // 2      m_raceMask
//    uint32    classMask;                                  // 3      m_classMask
//    uint32    flags;                                      // 4      m_flags
//    uint32    reqLevel;                                   // 5      m_minLevel
//    uint32    skillTierId;                                // 6      m_skillTierID
//    uint32    skillCostID;                                // 7      m_skillCostIndex
//};

//struct SkillTiersEntry{
//    uint32    id;                                         // 0      m_ID
//    uint32    skillValue[16];                             // 1-17   m_cost
//    uint32    maxSkillValue[16];                          // 18-32  m_valueMax
//};

struct SkillLineEntry
{
    uint32    id;                                           // 0        m_ID
    int32     categoryId;                                   // 1        m_categoryID
    char* name;                                             // 2        m_displayName_lang
    //char*     description;                                // 3        m_description_lang
    //uint32    spellIcon;                                  // 4        m_spellIconID
    //char*     alternateVerb;                              // 5        m_alternateVerb_lang
    uint32    canLink;                                      // 6        m_canLink (prof. with recipes)
    //uint32 parrentSkillLineID                             // 7
    //uint32 flags                                          // 8
};

struct SkillLineAbilityEntry
{
    //uint32    id;                                         // 0        m_ID
    uint32    skillId;                                      // 1        m_skillLine
    uint32    spellId;                                      // 2        m_spell
    uint32    racemask;                                     // 3        m_raceMask
    uint32    classmask;                                    // 4        m_classMask
    uint32    req_skill_value;                              // 5        m_minSkillLineRank
    uint32    forward_spellid;                              // 6        m_supercededBySpell
    uint32    learnOnGetSkill;                              // 7        m_acquireMethod
    uint32    max_value;                                    // 8        m_trivialSkillLineRankHigh
    uint32    min_value;                                    // 9        m_trivialSkillLineRankLow
    //uint32    character_points;                           // 10       m_characterPoints - Pandaria - No longer an array
    //uint32
    //uint32 tradeskillCategory
};

struct SoundEntriesEntry
{
    uint32    Id;                                           // 0        m_ID
    //uint32    Type;                                       // 1        m_soundType
    //char*     InternalName;                               // 2        m_name
    //char*     FileName[10];                               // 3-12     m_File[10]
    //uint32    Unk13[10];                                  // 13-22    m_Freq[10]
    //char*     Path;                                       // 23       m_DirectoryBase
                                                            // 24       m_volumeFloat
                                                            // 25       m_flags
                                                            // 26       m_minDistance
                                                            // 27       m_distanceCutoff
                                                            // 28       m_EAXDef
                                                            // 29       m_soundEntriesAdvancedID, new in 3.1
    //unk                                                   // 30       4.0.0
    //unk                                                   // 31       4.0.0
    //unk                                                   // 32       4.0.0
    //unk                                                   // 33       4.0.0
};

struct SpecializationSpellsEntry
{
    //uint32 Id;                // 0
    uint32 SpecializationId;    // 1
    uint32 SpellId;             // 2
    uint32 RemovesSpellId;      // 3
    //uint32 unk;               // 4
};

// SpellAuraOptions.dbc
struct SpellAuraOptionsEntry
{
    //uint32    Id;                                           // 0       m_ID
    //uint32  spellId;                                      // 1  - Pandaria
    //uint32  unk0;                                         // 2  - Panadraia always  after spellId
    uint32    StackAmount;                                  // 3       m_cumulativeAura
    uint32    procChance;                                   // 4       m_procChance
    uint32    procCharges;                                  // 5       m_procCharges
    uint32    procFlags;                                    // 6       m_procTypeMask
    //uint32  unk1                                          // 7 - Pandaria
    //uint32  unk2                                          // 8 - Pandaria
};

// SpellAuraRestrictions.dbc/
struct SpellAuraRestrictionsEntry
{
    //uint32    Id;                                         // 0        m_ID
    //uint32 spellId;                                       // 1  - Pandaria
    //uint32 unk0;                                          // 2  - Panadraia always  after spellId
    uint32    CasterAuraState;                              // 3       m_casterAuraState
    uint32    TargetAuraState;                              // 4       m_targetAuraState
    uint32    CasterAuraStateNot;                           // 5       m_excludeCasterAuraState
    uint32    TargetAuraStateNot;                           // 6       m_excludeTargetAuraState
    uint32    casterAuraSpell;                              // 7       m_casterAuraSpell
    uint32    targetAuraSpell;                              // 8       m_targetAuraSpell
    uint32    excludeCasterAuraSpell;                       // 9       m_excludeCasterAuraSpell
    uint32    excludeTargetAuraSpell;                       // 10      m_excludeTargetAuraSpell
};

// SpellCastingRequirements.dbc
struct SpellCastingRequirementsEntry
{
    //uint32    Id;                                           // 0        m_ID
    uint32    FacingCasterFlags;                            // 1       m_facingCasterFlags
    //uint32    MinFactionId;                               // 2      m_minFactionID not used
    //uint32    MinReputation;                              // 3      m_minReputation not used
    int32     AreaGroupId;                                  // 4      m_requiredAreaGroupId
    //uint32    RequiredAuraVision;                         // 5      m_requiredAuraVision not used
    uint32    RequiresSpellFocus;                           // 6       m_requiresSpellFocus
};

struct SpellCastTimesEntry
{
    //uint32    ID;                                           // 0
    int32     CastTime;                                     // 1
    //float     CastTimePerLevel;                           // 2 unsure / per skill?
    //int32     MinCastTime;                                // 3 unsure
};

// SpellCategories.dbc
struct SpellCategoriesEntry
{
    //uint32    Id;                                         // 0        m_ID
    //uint32 spellId;                                       // 1  - Pandaria
    //uint32 unk0;                                          // 2  - Pandaria always after spellId
    uint32    Category;                                     // 3        m_category
    uint32    DmgClass;                                     // 4        m_defenseType
    uint32    Dispel;                                       // 5        m_dispelType
    uint32    Mechanic;                                     // 6        m_mechanic
    uint32    PreventionType;                               // 7        m_preventionType
    uint32    StartRecoveryCategory;                        // 8        m_startRecoveryCategory
    //uint32 unk1;                                          // 9  - Pandaria
};

typedef std::set<uint32> SpellCategorySet;
typedef std::map<uint32, SpellCategorySet > SpellCategoryStore;

struct SpellCategoryEntry
{
    uint32 Id;
    uint32 Flags;
    // uint32 unk;
    // char* Name;
    // uint32 unk  - Pandaria
    // uint32 unk2 - Pandaria - could be shared CD
};

// SpellClassOptions.dbc
struct SpellClassOptionsEntry
{
    //uint32    Id;                                         // 0        m_ID
    //uint32    modalNextSpell;                             // 1       m_modalNextSpell not used
    flag128   SpellFamilyFlags;                             // 2-5
    uint32    SpellFamilyName;                              // 6       m_spellClassSet
};

// SpellCooldowns.dbc
struct SpellCooldownsEntry
{
    //uint32    Id;                                         // 0        m_ID
    //uint32 spellId;                                       // 1  - Pandaria
    //uint32 unk0;                                          // 2  - Panadraia always  after spellId
    uint32    CategoryRecoveryTime;                         // 3       m_categoryRecoveryTime
    uint32    RecoveryTime;                                 // 4       m_recoveryTime
    uint32    StartRecoveryTime;                            // 5      m_startRecoveryTime
};

struct SpellDurationEntry
{
    //uint32    ID;
    int32     Duration[3];
};

// Spell.dbc
struct SpellEntry
{
    uint32    Id;                                           // 0        m_ID
    char* SpellName;                                        // 1        m_name_lang
    //char* Rank;                                           // 2        m_nameSubtext_lang
    //char* Description;                                    // 3        m_description_lang not used
    //char* ToolTip;                                        // 4        m_auraDescription_lang not used
    uint32 runeCostID;                                      // 5        m_runeCostID
    //uint32 spellMissileID;                                // 6        m_spellMissileID not used
    //uint32  spellDescriptionVariableID;                   // 7        m_spellDescriptionVariableID, 3.2.0
    //float unk_f;                                          // 8        unk_f
    uint32 SpellScalingId;                                  // 9        SpellScaling.dbc
    uint32 SpellAuraOptionsId;                              // 10       SpellAuraOptions.dbc
    uint32 SpellAuraRestrictionsId;                         // 11       SpellAuraRestrictions.dbc
    uint32 SpellCastingRequirementsId;                      // 12       SpellCastingRequirements.dbc
    uint32 SpellCategoriesId;                               // 13       SpellCategories.dbc
    uint32 SpellClassOptionsId;                             // 14       SpellClassOptions.dbc
    uint32 SpellCooldownsId;                                // 15       SpellCooldowns.dbc
    uint32 SpellEquippedItemsId;                            // 16       SpellEquippedItems.dbc
    uint32 SpellInterruptsId;                               // 17       SpellInterrupts.dbc
    uint32 SpellLevelsId;                                   // 18       SpellLevels.dbc
    uint32 SpellReagentsId;                                 // 19       SpellReagents.db2
    uint32 SpellShapeshiftId;                               // 20       SpellShapeshift.dbc
    uint32 SpellTargetRestrictionsId;                       // 21       SpellTargetRestrictions.dbc
    uint32 SpellTotemsId;                                   // 22       SpellTotems.dbc
    //uint32 ResearchProject;                               // 23       ResearchProject.dbc
    uint32 SpellMiscId;                                     // 24       SpellMisc.dbc
};

// SpellEquippedItems.dbc
struct SpellEquippedItemsEntry
{
    //uint32    Id;                                         // 0        m_ID
    //uint32 spellId;                                       // 1  - Pandaria
    //uint32 unk0;                                          // 2  - Panadraia always  after spellId
    int32     EquippedItemClass;                            // 3       m_equippedItemClass (value)
    int32     EquippedItemInventoryTypeMask;                // 4       m_equippedItemInvTypes (mask)
    int32     EquippedItemSubClassMask;                     // 5       m_equippedItemSubclass (mask)
};

// SpellInterrupts.dbc
struct SpellInterruptsEntry
{
    //uint32    Id;                                         // 0        m_ID
    //uint32 spellId;                                       // 1  - Pandaria
    //uint32 unk0;                                          // 2  - Panadraia always  after spellId
    uint32    AuraInterruptFlags;                           // 3       m_auraInterruptFlags
    //uint32                                                // 4       4.0.0
    uint32    ChannelInterruptFlags;                        // 5       m_channelInterruptFlags
    //uint32                                                // 6       4.0.0
    uint32    InterruptFlags;                               // 7       m_interruptFlags
};

// SpellLevels.dbc
struct SpellLevelsEntry
{
    //uint32    Id;                                         // 0        m_ID
    //uint32 spellId;                                       // 1  - Pandaria
    //uint32 unk0;                                          // 2  - Panadraia always  after spellId
    uint32    baseLevel;                                    // 3       m_baseLevel
    uint32    maxLevel;                                     // 4       m_maxLevel
    uint32    spellLevel;                                   // 5       m_spellLevel
};

// SpellMisc.dbc
struct SpellMiscEntry
{
    //uint32    Id;                                         // 0        m_ID
    //uint32  SpellId                                       // 1
    //uint32    SpellDifficultyID;                          // 2        m_spellDifficultyID
    uint32    Attributes;                                   // 3        m_attribute
    uint32    AttributesEx;                                 // 4        m_attributesEx
    uint32    AttributesEx2;                                // 5        m_attributesExB
    uint32    AttributesEx3;                                // 6        m_attributesExC
    uint32    AttributesEx4;                                // 7        m_attributesExD
    uint32    AttributesEx5;                                // 8        m_attributesExE
    uint32    AttributesEx6;                                // 9        m_attributesExF
    uint32    AttributesEx7;                                // 10       m_attributesExG
    uint32    AttributesEx8;                                // 11       m_attributesExH
    uint32    AttributesEx9;                                // 12       m_attributesExI
    //uint32    AttributesEx10;                             // 13       m_attributesExJ
    //uint32    AttributesEx11;                             // 14       m_attributesExK
    //uint32    AttributesEx12;                             // 15       m_attributesExL
    //uint32    AttributesEx13;                             // 16       m_attributesExM
    uint32    CastingTimeIndex;                             // 17       m_castingTimeIndex
    uint32    DurationIndex;                                // 18       m_durationIndex
    uint32    rangeIndex;                                   // 19       m_rangeIndex
    float     speed;                                        // 20       m_speed
    uint32    SpellVisual[2];                               // 21-22    m_spellVisualID
    uint32    SpellIconID;                                  // 23       m_spellIconID
    uint32    activeIconID;                                 // 24       m_activeIconID
    uint32    SchoolMask;                                   // 25       m_schoolMask
};

struct SpellRangeEntry
{
    uint32    ID;
    float     minRangeHostile;
    float     minRangeFriend;
    float     maxRangeHostile;
    float     maxRangeFriend;                               //friend means unattackable unit here
    uint32    type;
    //char*   Name;                                         // 6     m_displayName_lang
    //char*   ShortName;                                    // 7    m_displayNameShort_lang
};

// SpellScaling.dbc
struct SpellScalingEntry
{
    //uint32    Id;                                         // 0        m_ID
    int32     CastTimeMin;                                  // 1
    int32     CastTimeMax;                                  // 2
    int32     CastTimeMaxLevel;                             // 3
    int32     ScalingClass;                                 // 4        (index * 100) + charLevel - 1 => gtSpellScaling.dbc
    float     CoefBase;                                     // 5        some coefficient, mostly 1.0f
    int32     CoefLevelBase;                                // 6        some level
    //int32   unk1                                          // 7  - Pandaria
    //int32   unk2                                          // 8  - Pandaria
};

// SpellShapeshift.dbc
struct SpellShapeshiftEntry
{
    //uint32    Id;                                           // 0 - m_ID
    uint32    StancesNot;                                   // 3 - m_shapeshiftExclude
    // uint32 unk_320_2;                                    // 2 - 3.2.0
    uint32    Stances;                                      // 1 - m_shapeshiftMask
    // uint32 unk_320_3;                                    // 4 - 3.2.0
    // uint32    StanceBarOrder;                            // 5 - m_stanceBarOrder not used
};

// SpellTargetRestrictions.dbc
struct SpellTargetRestrictionsEntry
{
    //uint32    Id;                                           // 0        m_ID
    //uint32  spellId;                                      // 1  - Pandaria
    //uint32  unk0;                                         // 2  - Pandaria always after spellId
    //float  unk0;                                          // 3
    //uint32  unk0;                                         // 4  - Pandaria always  after spellId
    uint32    MaxAffectedTargets;                           // 5        m_maxTargets
    //uint32    MaxTargetLevel;                               // 6        m_maxTargetLevel
    uint32    TargetCreatureType;                           // 7       m_targetCreatureType
    uint32    Targets;                                      // 8       m_targets
};

#define MAX_SPELL_TOTEMS            2

// SpellTotems.dbc
struct SpellTotemsEntry
{
    //uint32    Id;                                             // 0        m_ID
    uint32    TotemCategory[MAX_SPELL_TOTEMS];                // 1        m_requiredTotemCategoryID
    uint32    Totem[MAX_SPELL_TOTEMS];                        // 2        m_totem
};

//////////////////////////////////////////////

// SpellEffect.dbc
struct SpellEffectEntry
{
    uint32    Id;                                           // 0         m_ID
    //uint32    Unk0;                                       // 1         unk - Pandaria
    uint32    Effect;                                       // 2         m_effect
    float     EffectValueMultiplier;                        // 3         m_effectValueMultiplier
    uint32    EffectApplyAuraName;                          // 4         m_effectAura
    uint32    EffectAuraTickCount;                          // 5         m_effectAuraTickCount
    int32     EffectBasePoints;                             // 6         m_effectBasePoints (don't must be used in spell/auras explicitly, must be used cached Spell::m_currentBasePoints)
    float     EffectBonusMultiplier;                        // 7         m_effectBonusMultiplier
    float     EffectDamageMultiplier;                       // 8         m_effectDamageMultiplier
    uint32    EffectChainTarget;                            // 9         m_effectChainTargets
    int32     EffectDieSides;                               // 10         m_effectDieSides
    uint32    EffectItemType;                               // 11        m_effectItemType
    uint32    EffectMechanic;                               // 12        m_effectMechanic
    int32     EffectMiscValue;                              // 13        m_effectMiscValue
    int32     EffectMiscValueB;                             // 14        m_effectMiscValueB
    float     EffectPointsPerComboPoint;                    // 15        m_effectPointsPerCombo
    uint32    EffectRadiusIndex;                            // 16        m_effectRadiusIndex - spellradius.dbc
    uint32    EffectRadiusMaxIndex;                         // 17        4.0.0
    float     EffectRealPointsPerLevel;                     // 18        m_effectRealPointsPerLevel
    flag128   EffectSpellClassMask;                         // 19 20 21 22 m_effectSpellClassMask1(2/3), effect 0
    uint32    EffectTriggerSpell;                           // 23        m_effectTriggerSpell
    //uint32  Unk0                                          // 24        unk - Pandaria
    uint32    EffectImplicitTargetA;                        // 25        m_implicitTargetA
    uint32    EffectImplicitTargetB;                        // 26        m_implicitTargetB
    uint32    EffectSpellId;                                // 27        new 4.0.0
    uint32    EffectIndex;                                  // 28        new 4.0.0
    //uint32  Unk0                                          // 29        4.2.0 only 0 or 1
};

#define MAX_SPELL_EFFECTS 32
#define MAX_EFFECT_MASK 0xFFFFFFFF // Full uint32

// SpellEffectScaling.dbc
struct SpellEffectScalingEntry
{
    uint32 Id;                      // 1  Id
    float Multiplier;               // 2
    float RandomPointsMultiplier;   // 3
    float OtherMultiplier;          // 4
    // float UnkMultiplier          // 5
    uint32 SpellEffectId;           // 6
};


typedef std::set<uint32> PetFamilySpellsSet;
typedef std::map<uint32, PetFamilySpellsSet > PetFamilySpellsStore;
typedef UNORDERED_MAP<uint32, std::list<SkillLineAbilityEntry const*> > SpellsPerClassStore;
typedef UNORDERED_MAP<uint32, uint32> ClassBySkillIdStore;
typedef UNORDERED_MAP<uint32, uint32> SpellEffectScallingByEffectId;

struct SpellDifficultyEntry
{
    uint32     ID;                                          // 0
    int32      SpellID[4];                     // 1-4 instance modes: 10N, 25N, 10H, 25H or Normal/Heroic if only 1-2 is set, if 3-4 is 0 then Mode-2
};

struct SpellFocusObjectEntry
{
    uint32    ID;                                           // 0
    //char*     Name;                                       // 1        m_name_lang
};

struct SpellRadiusEntry
{
    uint32    ID;
    float     RadiusMin;
    float     RadiusPerLevel;
    //float unkRadius;
    float     RadiusMax;
};

// SpellPower.dbc
struct SpellPowerEntry
{
    //uint32    Id;                                         // 0        m_ID
    uint32    spellId;                                    // 1  - Pandaria
    //uint32    RaidDifficulty;                             // 2  - Pandaria RaidDifficultyID
    uint32    powerType;                                    // 3       m_powerType
    uint32    manaCost;                                     // 4       m_manaCost
    uint32    manaCostPerlevel;                             // 5       m_manaCostPerLevel
    uint32    manaPerSecond;                                // 6       m_manaPerSecond
    uint32    manaPerSecondPerLevel;                        // 7       m_manaPerSecondPerLevel
    //uint32    PowerDisplayId;                             // 8       m_powerDisplayID - id from PowerDisplay.dbc, new in 3.1
    float     ManaCostPercentageFloat;                      // 9       4.3.0
    float     ChannelCostPercentageFloat;                   // 10 - Pandaria - ChannelCostPercentageFloat
    uint32     ShapeShiftSpellID;                           // 11 - Pandaria - ShapeShiftSpellID
    //float     HealthCostPercentageFloat;                  // 12 - Pandaria - HealthCostPercentageFloat
};

struct SpellRuneCostEntry
{
    uint32  ID;                                             // 0
    uint32  RuneCost[4];                                    // 1-4 (0=blood, 1=frost, 2=unholy, 3=death)
    uint32  runePowerGain;                                  // 5

    bool NoRuneCost() const { return RuneCost[0] == 0 && RuneCost[1] == 0 && RuneCost[2] == 0 && RuneCost[3] == 0; }
    bool NoRunicPowerGain() const { return runePowerGain == 0; }
};

#define MAX_SHAPESHIFT_SPELLS 8

struct SpellShapeshiftFormEntry
{
    uint32 ID;                                              // 0
    //uint32 buttonPosition;                                // 1 unused
    //char* Name;                                           // 2 unused
    uint32 flags1;                                          // 3
    int32  creatureType;                                    // 4 <=0 humanoid, other normal creature types
    //uint32 spellIcon;                                     // 5 unused, related to next field
    uint32 attackSpeed;                                     // 6
    // Models changed, 0 is main model, the rest 3 are unused.
    uint32 modelID_A;                                       // 7 alliance modelid (0 means no model)
    uint32 modelID_H;                                       // 8 horde modelid (but only for one form)
    //uint32 unk3;                                          // 9 unused always 0
    //uint32 unk4;                                          // 10 unused always 0
    uint32 stanceSpell[MAX_SHAPESHIFT_SPELLS];              // 11-18 spells which appear in the bar after shapeshifting
    uint32 mount_type;                                       // MountType.dbc
    //uint32 unk6;                                          // 20
};

struct SpellItemEnchantmentEntry
{
    uint32      ID;                                         // 0        m_ID
    //uint32      charges;                                  // 1        m_charges
    uint32      type[MAX_ITEM_ENCHANTMENT_EFFECTS];         // 2-4      m_effect[MAX_ITEM_ENCHANTMENT_EFFECTS]
    uint32      amount[MAX_ITEM_ENCHANTMENT_EFFECTS];       // 5-7      m_effectPointsMin[MAX_ITEM_ENCHANTMENT_EFFECTS]
    uint32      spellid[MAX_ITEM_ENCHANTMENT_EFFECTS];      // 8-10    m_effectArg[MAX_ITEM_ENCHANTMENT_EFFECTS]
    char*       description;                                // 11       m_name_lang
    uint32      aura_id;                                    // 12       m_itemVisual
    uint32      slot;                                       // 13       m_flags
    uint32      GemID;                                      // 14       m_src_itemID
    uint32      EnchantmentCondition;                       // 15       m_condition_id
    uint32      requiredSkill;                              // 16       m_requiredSkillID
    uint32      requiredSkillValue;                         // 17       m_requiredSkillRank
    uint32      requiredMinLevel;                           // 18       m_requiredMinLevel
    uint32      requiredMaxLevel;                           // 19       m_requiredMaxLevel
    //uint32      unk[MAX_ITEM_ENCHANTMENT_EFFECTS]         // 20-22    unk
    //float     unk2[MAX_ITEM_ENCHANTMENT_EFFECTS]          // 23-25    unk2
};

struct SpellItemEnchantmentConditionEntry
{
    uint32  ID;                                             // 0        m_ID
    uint8   Color[5];                                       // 1-5      m_lt_operandType[5]
    //uint32  LT_Operand[5];                                // 6-10     m_lt_operand[5]
    uint8   Comparator[5];                                  // 11-15    m_operator[5]
    uint8   CompareColor[5];                                // 15-20    m_rt_operandType[5]
    uint32  Value[5];                                       // 21-25    m_rt_operand[5]
    //uint8   Logic[5]                                      // 25-30    m_logic[5]
};

struct SummonPropertiesEntry
{
    uint32  Id;                                             // 0
    uint32  Category;                                       // 1, 0 - can't be controlled?, 1 - something guardian?, 2 - pet?, 3 - something controllable?, 4 - taxi/mount?
    uint32  Faction;                                        // 2, 14 rows > 0
    uint32  Type;                                           // 3, see enum
    int32   Slot;                                           // 4, 0-6
    uint32  Flags;                                          // 5
};

#define MAX_TALENT_RANK 1 // REMOVE ME !!!
#define MAX_PET_TALENT_RANK 1                               // use in calculations, expected <= MAX_TALENT_RANK
#define MAX_TALENT_TABS 4

struct TalentEntry
{
    uint32    TalentID;                                     // 0
    //uint32    unk0;                                       // 1
    uint32    Row;                                          // 2
    //uint32    Col;                                        // 3
    uint32    SpellId;                                      // 4
    //uint32    unk1;                                       // 5
    //uint32    unk2;                                       // 6
    //uint32    unk3;                                       // 7
    uint32    playerClass;                                  // 8
    uint32 OverrideSpellID;                                 // 9
    //char* description                                     // 10
};

struct TaxiNodesEntry
{
    //uint32    ID;                                           // 0        m_ID
    uint32    map_id;                                       // 1        m_ContinentID
    float     x;                                            // 2        m_x
    float     y;                                            // 3        m_y
    float     z;                                            // 4        m_z
    char* name;                                             // 5        m_Name_lang
    uint32    MountCreatureID[2];                           // 6-7      m_MountCreatureID[2]
    //uint32 conditionId                                    // 8 - Pandaria
    //uint32 Flags;                                         // 9        m_Flags
    //float mapOffset[2];                                   // 10-11
};

struct TaxiPathEntry
{
    uint32    ID;                                           // 0        m_ID
    uint32    from;                                         // 1        m_FromTaxiNode
    uint32    to;                                           // 2        m_ToTaxiNode
    uint32    price;                                        // 3        m_Cost
};

struct TaxiPathNodeEntry
{
                                                            // 0        m_ID
    uint32    path;                                         // 1        m_PathID
    uint32    index;                                        // 2        m_NodeIndex
    uint32    mapid;                                        // 3        m_ContinentID
    float     x;                                            // 4        m_LocX
    float     y;                                            // 5        m_LocY
    float     z;                                            // 6        m_LocZ
    uint32    actionFlag;                                   // 7        m_flags
    uint32    delay;                                        // 8        m_delay
    uint32    arrivalEventID;                               // 9        m_arrivalEventID
    uint32    departureEventID;                             // 10       m_departureEventID
};

struct TotemCategoryEntry
{
    //uint32    ID;                                         // 0
    //char*   name;                                         // 1        m_name_lang
    uint32    categoryType;                                 // 2        m_totemCategoryType (one for specialization)
    uint32    categoryMask;                                 // 3        m_totemCategoryMask (compatibility mask for same type: different for totems, compatible from high to low for rods)
};

struct UnitPowerBarEntry
{
    //uint32  Id;
    //uint32  MinPower;
    uint32  MaxPower;
    //uint32  Unk0;
    //uint32  Unk1;
    //float   Unk2;
    //float   Unk3;
    //uint32  BarType;
    //uint32  TextureFile[6];
    //uint32  Unk4[6];
    //uint32  DisplayFlags;
    //char*   PowerName;
    //char*   CostString;
    //char*   EmptyMessage;
    //char*   Tooltip;
    //float   StartInset;
    //float   EndInset;
};

struct TransportAnimationEntry
{
    //uint32  Id;
    uint32  TransportEntry;
    uint32  TimeSeg;
    //float   X;
    //float   Y;
    //float   Z;
    //uint32  MovementId;
};

struct TransportRotationEntry
{
    //uint32  Id;
    uint32  TransportEntry;
    uint32  TimeSeg;
    float   X;
    float   Y;
    float   Z;
    float   W;
};

#define MAX_VEHICLE_SEATS 8

struct VehicleEntry
{
    uint32  m_ID;                                             // 0
    uint32  m_flags;                                          // 1
    //uint32  flags2                                          // 2
    //float   m_turnSpeed;                                    // 3
    //float   m_pitchSpeed;                                   // 4
    //float   m_pitchMin;                                     // 5
    //float   m_pitchMax;                                     // 6
    uint32  m_seatID[MAX_VEHICLE_SEATS];                      // 7-14
    //float   m_mouseLookOffsetPitch;                         // 15
    //float   m_cameraFadeDistScalarMin;                      // 16
    //float   m_cameraFadeDistScalarMax;                      // 17
    //float   m_cameraPitchOffset;                            // 18
    //float   m_facingLimitLeft;                              // 19
    //float   m_facingLimitRight;                             // 20
    //float   m_msslTrgtTurnLingering;                        // 21
    //float   m_msslTrgtPitchLingering;                       // 22
    //float   m_msslTrgtMouseLingering;                       // 23
    //float   m_msslTrgtEndOpacity;                           // 24
    //float   m_msslTrgtArcSpeed;                             // 25
    //float   m_msslTrgtArcRepeat;                            // 26
    //float   m_msslTrgtArcWidth;                             // 27
    //float   m_msslTrgtImpactRadius[2];                      // 28-29
    //char* m_msslTrgtArcTexture;                             // 30
    //char* m_msslTrgtImpactTexture;                          // 31
    //char* m_msslTrgtImpactModel[2];                         // 32-33
    //float   m_cameraYawOffset;                              // 34
    //uint32  m_uiLocomotionType;                             // 35
    //float   m_msslTrgtImpactTexRadius;                      // 36
    //uint32  m_uiSeatIndicatorType;                          // 37
    uint32  m_powerType;                                      // 38
    //uint32                                                  // 39
    //uint32                                                  // 40
};

struct VehicleSeatEntry
{
    uint32  m_ID;                                             // 0
    uint32  m_flags;                                          // 1
    //int32   m_attachmentID;                                 // 2
    float   m_attachmentOffsetX;                              // 3
    float   m_attachmentOffsetY;                              // 4
    float   m_attachmentOffsetZ;                              // 5
    //float   m_enterPreDelay;                                // 6
    //float   m_enterSpeed;                                   // 7
    //float   m_enterGravity;                                 // 8
    //float   m_enterMinDuration;                             // 9
    //float   m_enterMaxDuration;                             // 10
    //float   m_enterMinArcHeight;                            // 11
    //float   m_enterMaxArcHeight;                            // 12
    //int32   m_enterAnimStart;                               // 13
    //int32   m_enterAnimLoop;                                // 14
    //int32   m_rideAnimStart;                                // 15
    //int32   m_rideAnimLoop;                                 // 16
    //int32   m_rideUpperAnimStart;                           // 17
    //int32   m_rideUpperAnimLoop;                            // 18
    //float   m_exitPreDelay;                                 // 19
    //float   m_exitSpeed;                                    // 20
    //float   m_exitGravity;                                  // 21
    //float   m_exitMinDuration;                              // 22
    //float   m_exitMaxDuration;                              // 23
    //float   m_exitMinArcHeight;                             // 24
    //float   m_exitMaxArcHeight;                             // 25
    //int32   m_exitAnimStart;                                // 26
    //int32   m_exitAnimLoop;                                 // 27
    //int32   m_exitAnimEnd;                                  // 28
    //float   m_passengerYaw;                                 // 29
    //float   m_passengerPitch;                               // 30
    //float   m_passengerRoll;                                // 31
    //int32   m_passengerAttachmentID;                        // 32
    //int32   m_vehicleEnterAnim;                             // 33
    //int32   m_vehicleExitAnim;                              // 34
    //int32   m_vehicleRideAnimLoop;                          // 35
    //int32   m_vehicleEnterAnimBone;                         // 36
    //int32   m_vehicleExitAnimBone;                          // 37
    //int32   m_vehicleRideAnimLoopBone;                      // 38
    //float   m_vehicleEnterAnimDelay;                        // 39
    //float   m_vehicleExitAnimDelay;                         // 40
    //uint32  m_vehicleAbilityDisplay;                        // 41
    //uint32  m_enterUISoundID;                               // 42
    //uint32  m_exitUISoundID;                                // 43
    uint32  m_flagsB;                                         // 44
                                                            // 45-56 added in 3.1, floats mostly

    bool CanEnterOrExit() const { return m_flags & VEHICLE_SEAT_FLAG_CAN_ENTER_OR_EXIT; }
    bool CanSwitchFromSeat() const { return m_flags & VEHICLE_SEAT_FLAG_CAN_SWITCH; }
    bool IsUsableByOverride() const
    {
        return (m_flags & (VEHICLE_SEAT_FLAG_UNCONTROLLED | VEHICLE_SEAT_FLAG_UNK18) ||
            (m_flagsB & (VEHICLE_SEAT_FLAG_B_USABLE_FORCED | VEHICLE_SEAT_FLAG_B_USABLE_FORCED_2 | VEHICLE_SEAT_FLAG_B_USABLE_FORCED_3 | VEHICLE_SEAT_FLAG_B_USABLE_FORCED_4)));
    }
    bool IsEjectable() const { return m_flagsB & VEHICLE_SEAT_FLAG_B_EJECTABLE; }
};

struct WMOAreaTableEntry
{
    //uint32 Id;                                              // 0 index
    int32 rootId;                                           // 1 used in root WMO
    int32 adtId;                                            // 2 used in adt file
    int32 groupId;                                          // 3 used in group WMO
    //uint32 field4;
    //uint32 field5;
    //uint32 field6;
    //uint32 field7;
    //uint32 field8;
    uint32 Flags;                                           // 9 used for indoor/outdoor determination
    uint32 areaId;                                          // 10 link to AreaTableEntry.ID
    //char *Name;                                           // 11       m_AreaName_lang
    //uint32 field12;                                       // 12
    //uint32 field13;                                       // 13
    //uint32 field14;                                       // 14
};

struct WorldMapAreaEntry
{
    //uint32  ID;                                           // 0
    uint32  map_id;                                         // 1
    //uint32  area_id;                                      // 2 index (continent 0 areas ignored)
    //char* internal_name                                   // 3
    float   y1;                                             // 4
    float   y2;                                             // 5
    float   x1;                                             // 6
    float   x2;                                             // 7
    int32   virtual_map_id;                                 // 8 -1 (map_id have correct map) other: virtual map where zone show (map_id - where zone in fact internally)
    // int32   dungeonMap_id;                               // 9 pointer to DungeonMap.dbc (owerride x1, x2, y1, y2 coordinates)
    // uint32  parentMapID;                                 // 10

    // uint32  minRecommendedLevel;                         // 12 Minimum recommended level displayed on world map
    // uint32  maxRecommendedLevel;                         // 13 Maximum recommended level displayed on world map
};

#define MAX_WORLD_MAP_OVERLAY_AREA_IDX 4

struct WorldMapOverlayEntry
{
    //uint32    ID;                                           // 0
    //uint32    worldMapAreaId;                             // 1 idx in WorldMapArea.dbc
    uint32    areatableID[MAX_WORLD_MAP_OVERLAY_AREA_IDX];  // 2-5
    //char* internal_name                                   // 6        m_textureName
                                                            // 7        m_textureWidth
                                                            // 8        m_textureHeight
                                                            // 9        m_offsetX
                                                            // 10       m_offsetY
                                                            // 11       m_hitRectTop
                                                            // 12       m_hitRectLeft
                                                            // 13       m_hitRectBottom
                                                            // 14       m_hitRectRight
                                                            // 15 - Pandaria
};

struct WorldSafeLocsEntry
{
    uint32    ID;                                           // 0
    uint32    map_id;                                       // 1
    float     x;                                            // 2
    float     y;                                            // 3
    float     z;                                            // 4
    //float unk1;                                           // 5
    //char*   name;                                         // 6        m_AreaName_lang
};

/*
struct WorldStateSounds
{
    uint32    ID;                                           // 0        Worldstate
    uint32    unk;                                          // 1
    uint32    areaTable;                                    // 2
    uint32    WMOAreaTable;                                 // 3
    uint32    zoneIntroMusicTable;                          // 4
    uint32    zoneIntroMusic;                               // 5
    uint32    zoneMusic;                                    // 6
    uint32    soundAmbience;                                // 7
    uint32    soundProviderPreferences;                     // 8
};
*/

/*
struct WorldStateUI
{
    uint32    ID;                                           // 0
    uint32    map_id;                                       // 1        Can be -1 to show up everywhere.
    uint32    zone;                                         // 2        Can be zero for "everywhere".
    uint32    phaseMask;                                    // 3        Phase this WorldState is avaliable in
    uint32    icon;                                         // 4        The icon that is used in the interface.
    char*     textureFilename;                              // 5
    char*     text;                                         // 6-21     The worldstate text
    char*     description;                                  // 22-38    Text shown when hovering mouse on icon
    uint32    worldstateID;                                 // 39       This is the actual ID used
    uint32    type;                                         // 40       0 = unknown, 1 = unknown, 2 = not shown in ui, 3 = wintergrasp
    uint32    unk1;                                         // 41
    uint32    unk2;                                         // 43
    uint32    unk3;                                         // 44-58
    uint32    unk4;                                         // 59-61    Used for some progress bars.
    uint32    unk7;                                         // 62       Unused in 3.3.5a
};
*/

// GCC have alternative #pragma pack() syntax and old gcc version not support pack(pop), also any gcc version not support it at some platform
#if defined(__GNUC__)
#pragma pack()
#else
#pragma pack(pop)
#endif

typedef std::unordered_map<uint32, std::set<uint32>> PhaseGroupContainer;

struct VectorArray
{
    std::vector<std::string> stringVectorArray[2];
};

typedef std::map<uint32, VectorArray> NameGenVectorArraysMap;

// Structures not used for casting to loaded DBC data and not required then packing
struct MapDifficulty
{
    MapDifficulty() : DifficultyID(0), ErrorMessage(""), resetTime(0), maxPlayers(0), hasErrorMessage(false) { }
    MapDifficulty(uint32 difficultyID, std::string _errorMessage, uint32 _resetTime, uint32 _maxPlayers, bool _hasErrorMessage)
        : DifficultyID(difficultyID), ErrorMessage(_errorMessage), resetTime(_resetTime), maxPlayers(_maxPlayers), hasErrorMessage(_hasErrorMessage) { }
    
    uint32 DifficultyID;
    std::string ErrorMessage;
    uint32 resetTime;
    uint32 maxPlayers;
    bool hasErrorMessage;
};

struct TalentSpellPos
{
    TalentSpellPos() : talent_id(0), rank(0) { }
    TalentSpellPos(uint16 _talent_id, uint8 _rank) : talent_id(_talent_id), rank(_rank) { }

    uint16 talent_id;
    uint8  rank;
};

typedef std::map<uint32, TalentSpellPos> TalentSpellPosMap;

struct TaxiPathBySourceAndDestination
{
    TaxiPathBySourceAndDestination() : ID(0), price(0) { }
    TaxiPathBySourceAndDestination(uint32 _id, uint32 _price) : ID(_id), price(_price) { }

    uint32    ID;
    uint32    price;
};
typedef std::map<uint32, TaxiPathBySourceAndDestination> TaxiPathSetForSource;
typedef std::map<uint32, TaxiPathSetForSource> TaxiPathSetBySource;

struct TaxiPathNodePtr
{
    TaxiPathNodePtr() : i_ptr(NULL) { }
    TaxiPathNodePtr(TaxiPathNodeEntry const* ptr) : i_ptr(ptr) { }
    TaxiPathNodeEntry const* i_ptr;
    operator TaxiPathNodeEntry const& () const { return *i_ptr; }
};

typedef Path<TaxiPathNodePtr, TaxiPathNodeEntry const> TaxiPathNodeList;
typedef std::vector<TaxiPathNodeList> TaxiPathNodesByPath;

#define TaxiMaskSize 162
typedef uint8 TaxiMask[TaxiMaskSize];
#endif
