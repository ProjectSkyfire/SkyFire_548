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

#include "QuestDef.h"
#include "Player.h"
#include "World.h"
#include "ObjectMgr.h"

Quest::Quest(Field* questRecord)
{
    Id = questRecord[0].GetUInt32();
    Method = questRecord[1].GetUInt8();
    Level = questRecord[2].GetInt16();
    MinLevel = uint32(questRecord[3].GetInt16());
    MaxLevel = uint32(questRecord[4].GetInt16());
    ZoneOrSort = questRecord[5].GetInt16();
    Type = questRecord[6].GetUInt16();
    SuggestedPlayers = questRecord[7].GetUInt8();
    LimitTime = questRecord[8].GetUInt32();
    RequiredClasses = questRecord[9].GetUInt16();
    RequiredRaces = questRecord[10].GetUInt32();
    RequiredSkillId = questRecord[11].GetUInt16();
    RequiredSkillPoints = questRecord[12].GetUInt16();
    RequiredFactionId1 = questRecord[13].GetUInt16();
    RequiredFactionId2 = questRecord[14].GetUInt16();
    RequiredFactionValue1 = questRecord[15].GetInt32();
    RequiredFactionValue2 = questRecord[16].GetInt32();
    RequiredMinRepFaction = questRecord[17].GetUInt16();
    RequiredMaxRepFaction = questRecord[18].GetUInt16();
    RequiredMinRepValue = questRecord[19].GetInt32();
    RequiredMaxRepValue = questRecord[20].GetInt32();
    PrevQuestId = questRecord[21].GetInt32();
    NextQuestId = questRecord[22].GetInt32();
    ExclusiveGroup = questRecord[23].GetInt32();
    NextQuestIdChain = questRecord[24].GetUInt32();
    RewardXPId = questRecord[25].GetUInt8();
    RewardOrRequiredMoney = questRecord[26].GetInt32();
    RewardMoneyMaxLevel = questRecord[27].GetUInt32();
    RewardSpell = questRecord[28].GetUInt32();
    RewardSpellCast = questRecord[29].GetInt32();
    RewardHonor = questRecord[30].GetUInt32();
    RewardHonorMultiplier = questRecord[31].GetFloat();
    RewardMailTemplateId = questRecord[32].GetUInt32();
    RewardMailDelay = questRecord[33].GetUInt32();
    SourceItemId = questRecord[34].GetUInt32();
    SourceItemIdCount = questRecord[35].GetUInt8();
    SourceSpellid = questRecord[36].GetUInt32();
    Flags = questRecord[37].GetUInt32();
    Flags2 = questRecord[38].GetUInt32();
    SpecialFlags = questRecord[39].GetUInt8();
    MinimapTargetMark = questRecord[40].GetUInt8();
    RewardTitleId = questRecord[41].GetUInt8();
    RequiredPlayerKills = questRecord[42].GetUInt8();
    RewardTalents = questRecord[43].GetUInt8();
    RewardArenaPoints = questRecord[44].GetUInt16();
    RewardSkillId = questRecord[45].GetUInt16();
    RewardSkillPoints = questRecord[46].GetUInt8();
    RewardReputationMask = questRecord[47].GetUInt8();
    QuestGiverPortrait = questRecord[48].GetUInt32();
    QuestTurnInPortrait = questRecord[49].GetUInt32();
    RewardPackageItemId = questRecord[50].GetUInt32();

    for (int i = 0; i < QUEST_REWARDS_COUNT; ++i)
        RewardItemId[i] = questRecord[51+i].GetUInt32();

    for (int i = 0; i < QUEST_REWARDS_COUNT; ++i)
        RewardItemIdCount[i] = questRecord[55+i].GetUInt16();

    for (int i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        RewardChoiceItemId[i] = questRecord[59+i].GetUInt32();

    for (int i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        RewardChoiceItemCount[i] = questRecord[65+i].GetUInt16();

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        RewardFactionId[i] = questRecord[71+i].GetUInt16();

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        RewardFactionValueId[i] = questRecord[76+i].GetInt32();

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        RewardFactionValueIdOverride[i] = questRecord[81+i].GetInt32();

    PointMapId = questRecord[86].GetUInt16();
    PointX = questRecord[87].GetFloat();
    PointY = questRecord[88].GetFloat();
    PointOption = questRecord[89].GetUInt32();
    Title = questRecord[90].GetString();
    Objectives = questRecord[91].GetString();
    Details = questRecord[92].GetString();
    EndText = questRecord[93].GetString();
    CompletedText = questRecord[94].GetString();
    OfferRewardText = questRecord[95].GetString();
    RequestItemsText = questRecord[96].GetString();

    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        RequiredNpcOrGo[i] = questRecord[97+i].GetInt32();

    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        RequiredNpcOrGoCount[i] = questRecord[101+i].GetUInt16();

    for (int i = 0; i < QUEST_SOURCE_ITEM_IDS_COUNT; ++i)
        RequiredSourceItemId[i] = questRecord[105+i].GetUInt32();

    for (int i = 0; i < QUEST_SOURCE_ITEM_IDS_COUNT; ++i)
        RequiredSourceItemCount[i] = questRecord[109+i].GetUInt16();

    for (int i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; ++i)
        RequiredItemId[i] = questRecord[113+i].GetUInt32();

    for (int i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; ++i)
        RequiredItemCount[i] = questRecord[119+i].GetUInt16();

    RequiredSpell = questRecord[125].GetUInt32();

    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        ObjectiveText[i] = questRecord[126+i].GetString();

    for (int i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
        RewardCurrencyId[i] = questRecord[130+i].GetUInt16();

    for (int i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
        RewardCurrencyCount[i] = questRecord[134+i].GetUInt8();

    for (int i = 0; i < QUEST_REQUIRED_CURRENCY_COUNT; ++i)
        RequiredCurrencyId[i] = questRecord[138+i].GetUInt16();

    for (int i = 0; i < QUEST_REQUIRED_CURRENCY_COUNT; ++i)
        RequiredCurrencyCount[i] = questRecord[142+i].GetUInt8();

    QuestGiverTextWindow = questRecord[146].GetString();
    QuestGiverTargetName = questRecord[147].GetString();
    QuestTurnTextWindow = questRecord[148].GetString();
    QuestTurnTargetName = questRecord[149].GetString();
    SoundAccept = questRecord[150].GetUInt16();
    SoundTurnIn = questRecord[151].GetUInt16();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        DetailsEmote[i] = questRecord[152+i].GetUInt16();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        DetailsEmoteDelay[i] = questRecord[156+i].GetUInt32();

    EmoteOnIncomplete = questRecord[160].GetUInt16();
    EmoteOnComplete = questRecord[161].GetUInt16();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        OfferRewardEmote[i] = questRecord[162+i].GetInt16();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        OfferRewardEmoteDelay[i] = questRecord[166+i].GetInt32();

    // int32 WDBVerified = questRecord[169].GetInt32();

    if (SpecialFlags & QUEST_SPECIAL_FLAGS_AUTO_ACCEPT)
        Flags |= QUEST_FLAGS_AUTO_ACCEPT;

    _reqItemsCount = 0;
    _reqNpcOrGoCount = 0;
    _rewItemsCount = 0;
    _rewChoiceItemsCount = 0;
    _rewCurrencyCount = 0;
    _reqCurrencyCount = 0;

    for (int i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; ++i)
        if (RequiredItemId[i])
            ++_reqItemsCount;

    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        if (RequiredNpcOrGo[i])
            ++_reqNpcOrGoCount;

    for (int i = 0; i < QUEST_REWARDS_COUNT; ++i)
        if (RewardItemId[i])
            ++_rewItemsCount;

    for (int i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        if (RewardChoiceItemId[i])
            ++_rewChoiceItemsCount;

    for (int i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
        if (RewardCurrencyId[i])
            ++_rewCurrencyCount;

    for (int i = 0; i < QUEST_REQUIRED_CURRENCY_COUNT; ++i)
        if (RequiredCurrencyId[i])
            ++_reqCurrencyCount;
}

uint32 Quest::XPValue(Player* player) const
{
    if (player)
    {
        int32 quest_level = (Level == -1 ? player->getLevel() : Level);
        const QuestXPEntry* xpentry = sQuestXPStore.LookupEntry(quest_level);
        if (!xpentry)
            return 0;

        int32 diffFactor = 2 * (quest_level - player->getLevel()) + 20;
        if (diffFactor < 1)
            diffFactor = 1;
        else if (diffFactor > 10)
            diffFactor = 10;

        uint32 xp = diffFactor * xpentry->Exp[RewardXPId] / 10;
        if (xp <= 100)
            xp = 5 * ((xp + 2) / 5);
        else if (xp <= 500)
            xp = 10 * ((xp + 5) / 10);
        else if (xp <= 1000)
            xp = 25 * ((xp + 12) / 25);
        else
            xp = 50 * ((xp + 25) / 50);

        return xp;
    }

    return 0;
}

int32 Quest::GetRewOrReqMoney() const
{
    if (RewardOrRequiredMoney <= 0)
        return RewardOrRequiredMoney;

    return int32(RewardOrRequiredMoney * sWorld->getRate(RATE_DROP_MONEY));
}

uint32 Quest::GetRewMoneyMaxLevel() const
{
    if (HasFlag(QUEST_FLAGS_NO_MONEY_FROM_XP))
        return 0;

    return RewardMoneyMaxLevel;
}

bool Quest::IsAutoAccept() const
{
    return sWorld->getBoolConfig(CONFIG_QUEST_IGNORE_AUTO_ACCEPT) ? false : (Flags & QUEST_FLAGS_AUTO_ACCEPT);
}

bool Quest::IsAutoComplete() const
{
    return sWorld->getBoolConfig(CONFIG_QUEST_IGNORE_AUTO_COMPLETE) ? false : (Method == 0 || HasFlag(QUEST_FLAGS_AUTOCOMPLETE));
}

bool Quest::IsRaidQuest(Difficulty difficulty) const
{
    switch (Type)
    {
        case QUEST_TYPE_RAID:
            return true;
        case QUEST_TYPE_RAID_10:
            return (difficulty == RAID_DIFFICULTY_10MAN_NORMAL || difficulty == RAID_DIFFICULTY_10MAN_HEROIC);
        case QUEST_TYPE_RAID_25:
            return (difficulty == RAID_DIFFICULTY_25MAN_NORMAL || difficulty == RAID_DIFFICULTY_25MAN_HEROIC);
        default:
            break;
    }

    return false;
}

bool Quest::IsAllowedInRaid(Difficulty difficulty) const
{
    if (IsRaidQuest(difficulty))
        return true;

    return sWorld->getBoolConfig(CONFIG_QUEST_IGNORE_RAID);
}

uint32 Quest::CalculateHonorGain(uint8 level) const
{
    if (level > GT_MAX_LEVEL)
        level = GT_MAX_LEVEL;

    uint32 honor = 0;

    /*if (GetRewHonorAddition() > 0 || GetRewHonorMultiplier() > 0.0f)
    {
        // values stored from 0.. for 1...
        TeamContributionPointsEntry const* tc = sTeamContributionPointsStore.LookupEntry(level);
        if (!tc)
            return 0;

        honor = uint32(tc->value * GetRewHonorMultiplier() * 0.1f);
        honor += GetRewHonorAddition();
    }*/

    return honor;
}

// Note: These next two functions will need to be changed/extended once QuestPackageItem.db2 is implemented
bool Quest::IsRewChoiceItemValid(uint32 itemId) const
{
    for (uint8 i = 0; i < QUEST_REWARD_CHOICES_COUNT; i++)
        if (RewardChoiceItemId[i] == itemId)
            return true;

    return false;
}

uint32 Quest::GetRewChoiceItemCount(uint32 itemId) const
{
    for (uint8 i = 0; i < QUEST_REWARD_CHOICES_COUNT; i++)
        if (RewardChoiceItemId[i] == itemId)
            return RewardChoiceItemCount[i];

    return 0;
}
