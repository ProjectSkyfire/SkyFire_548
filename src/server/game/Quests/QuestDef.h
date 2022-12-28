/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRESERVER_QUEST_H
#define SKYFIRESERVER_QUEST_H

#include "Define.h"
#include "DatabaseEnv.h"
#include "SharedDefines.h"
#include "WorldPacket.h"
#include "DBCEnums.h"

#include <string>
#include <vector>

class Player;

class ObjectMgr;

#define MAX_QUEST_LOG_SIZE 25

#define QUEST_SOURCE_ITEM_IDS_COUNT 4
#define QUEST_REWARD_CHOICES_COUNT 6
#define QUEST_REWARDS_COUNT 4
#define QUEST_DEPLINK_COUNT 10
#define QUEST_REPUTATIONS_COUNT 5
#define QUEST_EMOTE_COUNT 4
#define QUEST_REWARD_CURRENCY_COUNT 4
#define QUEST_SOURCE_ITEM_COUNT 1

enum QuestFailedReason
{
    INVALIDREASON_DONT_HAVE_REQ                 = 0,
    INVALIDREASON_QUEST_FAILED_LOW_LEVEL        = 1,        // You are not high enough level for that quest.
    INVALIDREASON_QUEST_FAILED_WRONG_RACE       = 6,        // That quest is not available to your race.
    INVALIDREASON_QUEST_ALREADY_DONE            = 7,        // You have completed that quest.
    INVALIDREASON_QUEST_ONLY_ONE_TIMED          = 12,       // You can only be on one timed quest at a time.
    INVALIDREASON_QUEST_ALREADY_ON              = 13,       // You are already on that quest.
    INVALIDREASON_QUEST_FAILED_EXPANSION        = 16,       // This quest requires an expansion enabled account.
    INVALIDREASON_QUEST_ALREADY_ON2             = 18,       // You are already on that quest.
    INVALIDREASON_QUEST_FAILED_MISSING_ITEMS    = 21,       // You don't have the required items with you. Check storage.
    INVALIDREASON_QUEST_FAILED_NOT_ENOUGH_MONEY = 23,       // You don't have enough money for that quest.
    INVALIDREASON_DAILY_QUESTS_REMAINING        = 26,       // You have already completed 25 daily quests today.
    INVALIDREASON_QUEST_FAILED_CAIS             = 27,       // You cannot complete quests once you have reached tired time.
    INVALIDREASON_DAILY_QUEST_COMPLETED_TODAY   = 29        // You have completed that daily quest today.
};

enum QuestShareMessages
{
    QUEST_PARTY_MSG_SHARING_QUEST           = 0,
    QUEST_PARTY_MSG_CANT_TAKE_QUEST         = 1,
    QUEST_PARTY_MSG_ACCEPT_QUEST            = 2,
    QUEST_PARTY_MSG_DECLINE_QUEST           = 3,
    QUEST_PARTY_MSG_BUSY                    = 4,
    QUEST_PARTY_MSG_LOG_FULL                = 5,
    QUEST_PARTY_MSG_HAVE_QUEST              = 6,
    QUEST_PARTY_MSG_FINISH_QUEST            = 7,
    QUEST_PARTY_MSG_CANT_BE_SHARED_TODAY    = 8,
    QUEST_PARTY_MSG_SHARING_TIMER_EXPIRED   = 9,
    QUEST_PARTY_MSG_NOT_IN_PARTY            = 10
};

enum QuestTradeSkill
{
    QUEST_TRSKILL_NONE           = 0,
    QUEST_TRSKILL_ALCHEMY        = 1,
    QUEST_TRSKILL_BLACKSMITHING  = 2,
    QUEST_TRSKILL_COOKING        = 3,
    QUEST_TRSKILL_ENCHANTING     = 4,
    QUEST_TRSKILL_ENGINEERING    = 5,
    QUEST_TRSKILL_FIRSTAID       = 6,
    QUEST_TRSKILL_HERBALISM      = 7,
    QUEST_TRSKILL_LEATHERWORKING = 8,
    QUEST_TRSKILL_POISONS        = 9,
    QUEST_TRSKILL_TAILORING      = 10,
    QUEST_TRSKILL_MINING         = 11,
    QUEST_TRSKILL_FISHING        = 12,
    QUEST_TRSKILL_SKINNING       = 13,
    QUEST_TRSKILL_JEWELCRAFTING  = 14
};

enum QuestStatus
{
    QUEST_STATUS_NONE           = 0,
    QUEST_STATUS_COMPLETE       = 1,
    //QUEST_STATUS_UNAVAILABLE    = 2,
    QUEST_STATUS_INCOMPLETE     = 3,
    //QUEST_STATUS_AVAILABLE      = 4,
    QUEST_STATUS_FAILED         = 5,
    QUEST_STATUS_REWARDED       = 6,        // Not used in DB
    MAX_QUEST_STATUS
};

enum QuestGiverStatus
{
    DIALOG_STATUS_NONE                     = 0x000,
    DIALOG_STATUS_UNK                      = 0x001,
    DIALOG_STATUS_UNAVAILABLE              = 0x002,
    DIALOG_STATUS_LOW_LEVEL_AVAILABLE      = 0x004,
    DIALOG_STATUS_LOW_LEVEL_REWARD_REP     = 0x008,
    DIALOG_STATUS_LOW_LEVEL_AVAILABLE_REP  = 0x010,
    DIALOG_STATUS_INCOMPLETE               = 0x020,
    DIALOG_STATUS_REWARD_REP               = 0x040,
    DIALOG_STATUS_AVAILABLE_REP            = 0x080,
    DIALOG_STATUS_AVAILABLE                = 0x100,
    DIALOG_STATUS_REWARD2                  = 0x200,         // no yellow dot on minimap
    DIALOG_STATUS_REWARD                   = 0x400          // yellow dot on minimap
};

enum QuestFlags
{
    QUEST_FLAGS_NONE                      = 0x00000000,
    QUEST_FLAGS_STAY_ALIVE                = 0x00000001,   // Not used currently
    QUEST_FLAGS_START_EVENT_MUST_COMPLETE = 0x00000002,   // Not used currently. If player in party, all players that can accept this quest will receive confirmation box to accept quest CMSG_QUEST_CONFIRM_ACCEPT/SMSG_QUEST_CONFIRM_ACCEPT
    QUEST_FLAGS_COMPLETION_AREA_TRIGGER   = 0x00000004,   // Not used currently
    QUEST_FLAGS_PUSHABLE                  = 0x00000008,   // Can be shared: Player::CanShareQuest()
    QUEST_FLAGS_HAS_CONDITION             = 0x00000010,   // Not used currently
    QUEST_FLAGS_HIDE_REWARD_POI           = 0x00000020,   // Not used currently: Unsure of content
    QUEST_FLAGS_RAID_GROUP                = 0x00000040,   // Not used currently
    QUEST_FLAGS_EXPANSION_ONLY            = 0x00000080,   // Not used currently: Available if TBC expansion enabled only
    QUEST_FLAGS_NO_MONEY_FROM_XP          = 0x00000100,   // Not used currently: Experience is not converted to gold at max level
    QUEST_FLAGS_HIDE_REWARD               = 0x00000200,   // Items and money rewarded only sent in SMSG_QUESTGIVER_OFFER_REWARD (not in SMSG_QUESTGIVER_QUEST_DETAILS or in client quest log(SMSG_QUEST_QUERY_RESPONSE))
    QUEST_FLAGS_TRACKING_EVENT            = 0x00000400,   // These quests are automatically rewarded on quest complete and they will never appear in quest log client side.
    QUEST_FLAGS_DEPRECATE_REPUTATION      = 0x00000800,   // Not used currently
    QUEST_FLAGS_DAILY                     = 0x00001000,   // Used to know quest is Daily one
    QUEST_FLAGS_FLAGS_PVP                 = 0x00002000,   // Having this quest in log forces PvP flag
    QUEST_FLAGS_DEPRECATED                = 0x00004000,   // Used on quests that are not generically available
    QUEST_FLAGS_WEEKLY                    = 0x00008000,
    QUEST_FLAGS_AUTOCOMPLETE              = 0x00010000,   // auto complete
    QUEST_FLAGS_DISPLAY_ITEM_IN_TRACKER   = 0x00020000,   // Displays usable item in quest tracker
    QUEST_FLAGS_DISABLE_COMPLETION_TEXT   = 0x00040000,   // use Objective text as Complete text
    QUEST_FLAGS_AUTO_ACCEPT               = 0x00080000,   // The client recognizes this flag as auto-accept. However, NONE of the current quests (3.3.5a) have this flag. Maybe blizz used to use it, or will use it in the future.
    QUEST_FLAGS_PLAYER_CAST_ACCEPT        = 0x00100000,   // Quests with this flag player submit automatically by special button in player gui
    QUEST_FLAGS_PLAYER_CAST_COMPLETE      = 0x00200000,   // Automatically suggestion of accepting quest. Not from npc.
    QUEST_FLAGS_UPDATE_PHASE_SHIFT        = 0x00400000,
    QUEST_FLAGS_SOR_WHITELIST             = 0x00800000,
    QUEST_FLAGS_LAUNCH_GOSSIP_COMPLETE    = 0x01000000,
    QUEST_FLAGS_REMOVE_EXTRA_GET_ITEMS    = 0x02000000,
    QUEST_FLAGS_HIDE_UNTIL_DISCOVERED     = 0x04000000,
    QUEST_FLAGS_PORTRAIT_FROM_LOG         = 0x08000000,
    QUEST_FLAGS_SHOW_ITEM_WHEN_COMPLETE   = 0x10000000,
    QUEST_FLAGS_LAUNCH_GOSSIP_ACCEPT      = 0x20000000,
    QUEST_FLAGS_ITEM_GLOW_ON_COMPLETE     = 0x40000000,
    QUEST_FLAGS_FAIL_ON_LOGOUT            = 0x80000000
};

enum QuestFlags2
{
    QUEST_FLAGS2_NONE                                 = 0x0000,
    QUEST_FLAGS2_KEEP_ADDITIONAL_ITEMS                = 0x0001,
    QUEST_FLAGS2_SUPPRESS_GOSSIP_COMPLETE             = 0x0002,
    QUEST_FLAGS2_SUPPRESS_GOSSIP_ACCEPT               = 0x0004,
    QUEST_FLAGS2_DISALLOW_PLAYER_AS_QUESTGIVER        = 0x0008,
    QUEST_FLAGS2_DISPLAY_CLASS_CHOICE_REWARDS         = 0x0010,
    QUEST_FLAGS2_DISPLAY_SPEC_CHOICE_REWARDS          = 0x0020,
    QUEST_FLAGS2_REMOVE_FROM_LOG_ON_PERIDOIC_RESET    = 0x0040,
    QUEST_FLAGS2_ACCOUNT_LEVEL_QUEST                  = 0x0080,
    QUEST_FLAGS2_LEGENDARY_QUEST                      = 0x0100,
    QUEST_FLAGS2_NO_GUILD_XP                          = 0x0200,
    QUEST_FLAGS2_RESET_CACHE_ON_ACCEPT                = 0x0400,
    QUEST_FLAGS2_NO_ABANDON_ON_ANY_OBJECTIVE_COMPLETE = 0x0800,
    QUEST_FLAGS2_RECAST_ACCEPT_SPELL_ON_LOGIN         = 0x1000,
    QUEST_FLAGS2_UPDATE_ZONE_AURAS                    = 0x2000,
    QUEST_FLAGS2_NO_CREDIT_FOR_PROXY                  = 0x4000,
    QUEST_FLAGS2_DISPLAY_AS_DAILY_QUEST               = 0x8000
};

enum QuestSpecialFlags
{
    QUEST_SPECIAL_FLAGS_NONE                 = 0x000,
    // Skyfire flags for set SpecialFlags in DB if required but used only at server
    QUEST_SPECIAL_FLAGS_REPEATABLE           = 0x001,   // Set by 1 in SpecialFlags from DB
    QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT = 0x002,   // Set by 2 in SpecialFlags from DB (if required area explore, spell SPELL_EFFECT_QUEST_COMPLETE casting, table `FECT_QUEST_COMPLETE casting, table `*_script` command SCRIPT_COMMAND_QUEST_EXPLORED use, set from script)
    QUEST_SPECIAL_FLAGS_AUTO_ACCEPT          = 0x004,   // Set by 4 in SpecialFlags in DB if the quest is to be auto-accepted.
    QUEST_SPECIAL_FLAGS_DF_QUEST             = 0x008,   // Set by 8 in SpecialFlags in DB if the quest is used by Dungeon Finder.
    QUEST_SPECIAL_FLAGS_MONTHLY              = 0x010,   // Set by 16 in SpecialFlags in DB if the quest is reset at the begining of the month
    QUEST_SPECIAL_FLAGS_CAST                 = 0x020,   // Set by 32 in SpecialFlags in DB if the quest requires RequiredOrNpcGo killcredit but NOT kill (a spell cast)
    // room for more custom flags

    QUEST_SPECIAL_FLAGS_DB_ALLOWED = QUEST_SPECIAL_FLAGS_REPEATABLE | QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT | QUEST_SPECIAL_FLAGS_AUTO_ACCEPT | QUEST_SPECIAL_FLAGS_DF_QUEST | QUEST_SPECIAL_FLAGS_MONTHLY | QUEST_SPECIAL_FLAGS_CAST,

    //QUEST_SPECIAL_FLAGS_DELIVER              = 0x080,   // Internal flag computed only
    //QUEST_SPECIAL_FLAGS_SPEAKTO              = 0x100,   // Internal flag computed only
    //QUEST_SPECIAL_FLAGS_KILL                 = 0x200,   // Internal flag computed only
    QUEST_SPECIAL_FLAGS_TIMED                = 0x400,   // Internal flag computed only
    //QUEST_SPECIAL_FLAGS_PLAYER_KILL          = 0x800    // Internal flag computed only
};

enum QuestObjectiveType
{
    QUEST_OBJECTIVE_TYPE_NPC              = 0,
    QUEST_OBJECTIVE_TYPE_ITEM             = 1,
    QUEST_OBJECTIVE_TYPE_GO               = 2,
    QUEST_OBJECTIVE_TYPE_NPC_INTERACT     = 3,
    QUEST_OBJECTIVE_TYPE_CURRENCY         = 4,
    QUEST_OBJECTIVE_TYPE_SPELL            = 5,
    QUEST_OBJECTIVE_TYPE_FACTION_REP      = 6,
    QUEST_OBJECTIVE_TYPE_FACTION_REP2     = 7,
    QUEST_OBJECTIVE_TYPE_MONEY            = 8,
    QUEST_OBJECTIVE_TYPE_PLAYER           = 9,
    QUEST_OBJECTIVE_TYPE_DUMMY            = 10,
    QUEST_OBJECTIVE_TYPE_PET_BATTLE_TAMER = 11,
    QUEST_OBJECTIVE_TYPE_PET_BATTLE_ELITE = 12,
    QUEST_OBJECTIVE_TYPE_PET_BATTLE_PVP   = 13,
    QUEST_OBJECTIVE_TYPE_END
};

struct QuestLocale
{
    StringVector Title;
    StringVector Details;
    StringVector Objectives;
    StringVector OfferRewardText;
    StringVector RequestItemsText;
    StringVector EndText;
    StringVector CompletedText;
    // new on 4.x
    StringVector QuestGiverTextWindow;
    StringVector QuestGiverTargetName;
    StringVector QuestTurnTextWindow;
    StringVector QuestTurnTargetName;
};

struct QuestObjectiveLocale
{
    StringVector Description;
};

typedef std::vector<uint32> VisualEffectVec;
typedef std::vector<uint8> ObjectiveTypeCountVec;

struct QuestObjective
{
    QuestObjective(uint32 id, uint8 index, uint8 type, uint32 objectId, int32 amount, uint32 flags, std::string description)
        : Id(id), Index(index), Type(type), ObjectId(objectId), Amount(amount), Flags(flags), Description(description) { }

    uint32 Id;
    uint8 Index;
    uint8 Type;
    uint32 ObjectId;
    int32 Amount;
    uint32 Flags;
    std::string Description;
    VisualEffectVec VisualEffects;
};

typedef std::set<QuestObjective*> QuestObjectiveSet;

// This Quest class provides a convenient way to access a few pretotaled (cached) quest details,
// all base quest information, and any utility functions such as generating the amount of
// xp to give
class Quest
{
    friend class ObjectMgr;
    public:
        Quest(Field* questRecord);
        uint32 XPValue(Player* player) const;

        bool HasFlag(uint32 flag) const { return (Flags & flag) != 0; }
        void SetFlag(uint32 flag) { Flags |= flag; }
        bool HasFlag2(uint32 flag) const { return (Flags2 & flag) != 0; }
        void SetFlag2(uint32 flag) { Flags2 |= flag; }

        bool HasSpecialFlag(uint32 flag) const { return (SpecialFlags & flag) != 0; }
        void SetSpecialFlag(uint32 flag) { SpecialFlags |= flag; }

        // table data accessors:
        uint32 GetQuestId() const { return Id; }
        uint32 GetQuestMethod() const { return Method; }
        int32  GetZoneOrSort() const { return ZoneOrSort; }
        uint32 GetMinLevel() const { return MinLevel; }
        uint32 GetMaxLevel() const { return MaxLevel; }
        int32  GetQuestLevel() const { return Level; }
        uint32 GetType() const { return Type; }
        uint32 GetRequiredClasses() const { return RequiredClasses; }
        uint32 GetRequiredRaces() const { return RequiredRaces; }
        uint32 GetRequiredSkill() const { return RequiredSkillId; }
        uint32 GetRequiredSkillValue() const { return RequiredSkillPoints; }
        uint32 GetRequiredMinRepFaction() const { return RequiredMinRepFaction; }
        int32  GetRequiredMinRepValue() const { return RequiredMinRepValue; }
        uint32 GetRequiredMaxRepFaction() const { return RequiredMaxRepFaction; }
        int32  GetRequiredMaxRepValue() const { return RequiredMaxRepValue; }
        uint32 GetSuggestedPlayers() const { return SuggestedPlayers; }
        uint32 GetLimitTime() const { return LimitTime; }
        int32  GetPrevQuestId() const { return PrevQuestId; }
        int32  GetNextQuestId() const { return NextQuestId; }
        int32  GetExclusiveGroup() const { return ExclusiveGroup; }
        uint32 GetNextQuestInChain() const { return NextQuestIdChain; }
        uint32 GetCharTitleId() const { return RewardTitleId; }
        uint32 GetBonusTalents() const { return RewardTalents; }
        int32  GetRewArenaPoints() const {return RewardArenaPoints; }
        uint32 GetXPId() const { return RewardXPId; }
        uint32 GetSrcItemId() const { return SourceItemId; }
        uint32 GetSrcSpell() const { return SourceSpellid; }
        std::string const& GetTitle() const { return Title; }
        std::string const& GetDetails() const { return Details; }
        std::string const& GetObjectives() const { return Objectives; }
        std::string const& GetOfferRewardText() const { return OfferRewardText; }
        std::string const& GetRequestItemsText() const { return RequestItemsText; }
        std::string const& GetEndText() const { return EndText; }
        std::string const& GetCompletedText() const { return CompletedText; }
        std::string const& GetQuestGiverTextWindow() const { return QuestGiverTextWindow; }
        std::string const& GetQuestGiverTargetName() const { return QuestGiverTargetName; }
        std::string const& GetQuestTurnTextWindow() const { return QuestTurnTextWindow; }
        std::string const& GetQuestTurnTargetName() const { return QuestTurnTargetName; }
        int32  GetRewMoney() const;
        uint32 GetRewHonorAddition() const { return RewardHonor; }
        float GetRewHonorMultiplier() const { return RewardHonorMultiplier; }
        uint32 GetRewMoneyMaxLevel() const; // use in XP calculation at client
        uint32 GetRewSpell() const { return RewardSpell; }
        int32  GetRewSpellCast() const { return RewardSpellCast; }
        uint32 GetRewMailTemplateId() const { return RewardMailTemplateId; }
        uint32 GetRewMailDelaySecs() const { return RewardMailDelay; }
        uint32 GetPointMapId() const { return PointMapId; }
        float  GetPointX() const { return PointX; }
        float  GetPointY() const { return PointY; }
        uint32 GetPointOpt() const { return PointOption; }
        uint32 GetSoundAccept() const { return SoundAccept; }
        uint32 GetSoundTurnIn() const { return SoundTurnIn; }
        uint32 GetIncompleteEmote() const { return EmoteOnIncomplete; }
        uint32 GetCompleteEmote() const { return EmoteOnComplete; }
        bool   IsRepeatable() const { return SpecialFlags & QUEST_SPECIAL_FLAGS_REPEATABLE; }
        bool   IsAutoAccept() const;
        bool   IsAutoComplete() const;
        uint32 GetFlags() const { return Flags; }
        uint32 GetFlags2() const { return Flags2; }
        uint32 GetSpecialFlags() const { return SpecialFlags; }
        uint32 GetMinimapTargetMark() const { return MinimapTargetMark; }
        uint32 GetRewardSkillId() const { return RewardSkillId; }
        uint32 GetRewardSkillPoints() const { return RewardSkillPoints; }
        uint32 GetRewardReputationMask() const { return RewardReputationMask; }
        uint32 GetQuestGiverPortrait() const { return QuestGiverPortrait; }
        uint32 GetQuestTurnInPortrait() const { return QuestTurnInPortrait; }
        uint32 GetRewChoiceItemCount(uint32 itemId) const;
        bool   IsDaily() const { return Flags & QUEST_FLAGS_DAILY; }
        bool   IsWeekly() const { return Flags & QUEST_FLAGS_WEEKLY; }
        bool   IsMonthly() const { return SpecialFlags & QUEST_SPECIAL_FLAGS_MONTHLY; }
        bool   IsSeasonal() const { return (ZoneOrSort == -QUEST_SORT_SEASONAL || ZoneOrSort == -QUEST_SORT_SPECIAL || ZoneOrSort == -QUEST_SORT_LUNAR_FESTIVAL || ZoneOrSort == -QUEST_SORT_MIDSUMMER || ZoneOrSort == -QUEST_SORT_BREWFEST || ZoneOrSort == -QUEST_SORT_LOVE_IS_IN_THE_AIR || ZoneOrSort == -QUEST_SORT_NOBLEGARDEN) && !IsRepeatable(); }
        bool   IsDailyOrWeekly() const { return Flags & (QUEST_FLAGS_DAILY | QUEST_FLAGS_WEEKLY); }
        bool   IsRaidQuest(DifficultyID difficulty) const;
        bool   IsAllowedInRaid(DifficultyID difficulty) const;
        bool   IsDFQuest() const { return SpecialFlags & QUEST_SPECIAL_FLAGS_DF_QUEST; }
        bool   IsRewChoiceItemValid(uint32 itemId) const;
        uint32 CalculateHonorGain(uint8 level) const;

        // multiple values
        uint32 RequiredSourceItemId[QUEST_SOURCE_ITEM_IDS_COUNT];
        uint32 RequiredSourceItemCount[QUEST_SOURCE_ITEM_IDS_COUNT];
        uint32 RewardChoiceItemId[QUEST_REWARD_CHOICES_COUNT];
        uint32 RewardChoiceItemCount[QUEST_REWARD_CHOICES_COUNT];
        uint32 RewardItemId[QUEST_REWARDS_COUNT];
        uint32 RewardItemIdCount[QUEST_REWARDS_COUNT];
        uint32 RewardFactionId[QUEST_REPUTATIONS_COUNT];
        int32  RewardFactionValueId[QUEST_REPUTATIONS_COUNT];
        int32  RewardFactionValueIdOverride[QUEST_REPUTATIONS_COUNT];
        uint32 DetailsEmote[QUEST_EMOTE_COUNT];
        uint32 DetailsEmoteDelay[QUEST_EMOTE_COUNT];
        uint32 OfferRewardEmote[QUEST_EMOTE_COUNT];
        uint32 OfferRewardEmoteDelay[QUEST_EMOTE_COUNT];
        // 4.x
        uint32 RewardCurrencyId[QUEST_REWARD_CURRENCY_COUNT];
        uint32 RewardCurrencyCount[QUEST_REWARD_CURRENCY_COUNT];

        uint32 GetRewChoiceItemsCount() const { return _rewChoiceItemsCount; }
        uint32 GetRewItemsCount() const { return _rewItemsCount; }
        uint32 GetRewCurrencyCount() const { return _rewCurrencyCount; }
        // 5.x
        uint32 GetRewardPackageItemId() const { return RewardPackageItemId; }

        typedef std::vector<int32> PrevQuests;
        PrevQuests prevQuests;
        typedef std::vector<uint32> PrevChainQuests;
        PrevChainQuests prevChainQuests;

        // objective stuff
        QuestObjectiveSet m_questObjectives;

        QuestObjective const* GetQuestObjective(uint32 objectiveId) const;
        QuestObjective const* GetQuestObjectiveXIndex(uint8 index) const;
        QuestObjective const* GetQuestObjectiveXObjectId(uint32 objectId) const;

        uint8 GetQuestObjectiveCount() const { return m_questObjectives.size(); }
        uint8 GetQuestObjectiveCountType(uint8 type) const;

        // cached data
    private:
        uint32 _rewChoiceItemsCount;
        uint32 _rewItemsCount;
        uint32 _rewCurrencyCount;
        ObjectiveTypeCountVec m_questObjecitveTypeCount;

        // table data
    protected:
        uint32 Id;
        uint32 Method;
        int32  ZoneOrSort;
        uint32 MinLevel;
        uint32 MaxLevel;
        int32  Level;
        uint32 Type;
        uint32 RequiredClasses;
        uint32 RequiredRaces;
        uint32 RequiredSkillId;
        uint32 RequiredSkillPoints;
        uint32 RequiredMinRepFaction;
        int32  RequiredMinRepValue;
        uint32 RequiredMaxRepFaction;
        int32  RequiredMaxRepValue;
        uint32 SuggestedPlayers;
        uint32 LimitTime;
        uint32 Flags;
        uint32 RewardTitleId;
        uint32 RequiredPlayerKills;
        uint32 RewardTalents;
        int32  RewardArenaPoints;
        int32  PrevQuestId;
        int32  NextQuestId;
        int32  ExclusiveGroup;
        uint32 NextQuestIdChain;
        uint32 RewardXPId;
        uint32 SourceItemId;
        uint32 SourceSpellid;
        std::string Title;
        std::string Details;
        std::string Objectives;
        std::string OfferRewardText;
        std::string RequestItemsText;
        std::string EndText;
        std::string CompletedText;
        uint32 RewardHonor;
        float RewardHonorMultiplier;
        int32  RewardMoney;
        uint32 RewardMoneyMaxLevel;
        uint32 RewardSpell;
        int32  RewardSpellCast;
        uint32 RewardMailTemplateId;
        uint32 RewardMailDelay;
        uint32 PointMapId;
        float  PointX;
        float  PointY;
        uint32 PointOption;
        uint32 EmoteOnIncomplete;
        uint32 EmoteOnComplete;
        // new in 4.x
        uint32 MinimapTargetMark;
        uint32 RewardSkillId;
        uint32 RewardSkillPoints;
        uint32 RewardReputationMask;
        uint32 QuestGiverPortrait;
        uint32 QuestTurnInPortrait;
        std::string QuestGiverTextWindow;
        std::string QuestGiverTargetName;
        std::string QuestTurnTextWindow;
        std::string QuestTurnTargetName;
        uint32 SoundAccept;
        uint32 SoundTurnIn;
        // new in 5.x
        uint32 Flags2;
        uint32 RewardPackageItemId;

        uint32 SpecialFlags; // custom flags, not sniffed/WDB
};

struct QuestStatusData
{
    QuestStatusData() : Status(QUEST_STATUS_NONE), Timer(0), Explored(false) { }

    QuestStatus Status;
    uint32 Timer;
    bool Explored;
};
#endif
