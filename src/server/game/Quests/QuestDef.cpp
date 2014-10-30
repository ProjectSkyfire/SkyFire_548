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
    RequiredMinRepFaction = questRecord[13].GetUInt16();
    RequiredMaxRepFaction = questRecord[14].GetUInt16();
    RequiredMinRepValue = questRecord[15].GetInt32();
    RequiredMaxRepValue = questRecord[16].GetInt32();
    PrevQuestId = questRecord[17].GetInt32();
    NextQuestId = questRecord[18].GetInt32();
    ExclusiveGroup = questRecord[19].GetInt32();
    NextQuestIdChain = questRecord[20].GetUInt32();
    RewardXPId = questRecord[21].GetUInt8();
    RewardMoney = questRecord[22].GetInt32();
    RewardMoneyMaxLevel = questRecord[23].GetUInt32();
    RewardSpell = questRecord[24].GetUInt32();
    RewardSpellCast = questRecord[25].GetInt32();
    RewardHonor = questRecord[26].GetUInt32();
    RewardHonorMultiplier = questRecord[27].GetFloat();
    RewardMailTemplateId = questRecord[28].GetUInt32();
    RewardMailDelay = questRecord[29].GetUInt32();
    SourceItemId = questRecord[30].GetUInt32();
    SourceSpellid = questRecord[31].GetUInt32();
    Flags = questRecord[32].GetUInt32();
    Flags2 = questRecord[33].GetUInt32();
    SpecialFlags = questRecord[34].GetUInt8();
    MinimapTargetMark = questRecord[35].GetUInt8();
    RewardTitleId = questRecord[36].GetUInt8();
    RewardTalents = questRecord[37].GetUInt8();
    RewardArenaPoints = questRecord[38].GetUInt16();
    RewardSkillId = questRecord[39].GetUInt16();
    RewardSkillPoints = questRecord[40].GetUInt8();
    RewardReputationMask = questRecord[41].GetUInt8();
    QuestGiverPortrait = questRecord[42].GetUInt32();
    QuestTurnInPortrait = questRecord[43].GetUInt32();
    RewardPackageItemId = questRecord[44].GetUInt32();

    for (int i = 0; i < QUEST_REWARDS_COUNT; ++i)
        RewardItemId[i] = questRecord[45+i].GetUInt32();

    for (int i = 0; i < QUEST_REWARDS_COUNT; ++i)
        RewardItemIdCount[i] = questRecord[49+i].GetUInt16();

    for (int i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        RewardChoiceItemId[i] = questRecord[53+i].GetUInt32();

    for (int i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        RewardChoiceItemCount[i] = questRecord[59+ i].GetUInt16();

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        RewardFactionId[i] = questRecord[65+i].GetUInt16();

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        RewardFactionValueId[i] = questRecord[70+i].GetInt32();

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        RewardFactionValueIdOverride[i] = questRecord[75+i].GetInt32();

    PointMapId = questRecord[80].GetUInt16();
    PointX = questRecord[81].GetFloat();
    PointY = questRecord[82].GetFloat();
    PointOption = questRecord[83].GetUInt32();
    Title = questRecord[84].GetString();
    Objectives = questRecord[85].GetString();
    Details = questRecord[86].GetString();
    EndText = questRecord[87].GetString();
    CompletedText = questRecord[88].GetString();
    OfferRewardText = questRecord[89].GetString();
    RequestItemsText = questRecord[90].GetString();

    for (int i = 0; i < QUEST_SOURCE_ITEM_IDS_COUNT; ++i)
        RequiredSourceItemId[i] = questRecord[91+i].GetUInt32();

    for (int i = 0; i < QUEST_SOURCE_ITEM_IDS_COUNT; ++i)
        RequiredSourceItemCount[i] = questRecord[95+i].GetUInt16();

    for (int i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
        RewardCurrencyId[i] = questRecord[99+i].GetUInt16();

    for (int i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
        RewardCurrencyCount[i] = questRecord[103+i].GetUInt8();

    QuestGiverTextWindow = questRecord[107].GetString();
    QuestGiverTargetName = questRecord[108].GetString();
    QuestTurnTextWindow = questRecord[109].GetString();
    QuestTurnTargetName = questRecord[110].GetString();
    SoundAccept = questRecord[111].GetUInt16();
    SoundTurnIn = questRecord[112].GetUInt16();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        DetailsEmote[i] = questRecord[113+i].GetUInt16();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        DetailsEmoteDelay[i] = questRecord[117+i].GetUInt32();

    EmoteOnIncomplete = questRecord[121].GetUInt16();
    EmoteOnComplete = questRecord[122].GetUInt16();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        OfferRewardEmote[i] = questRecord[123+i].GetInt16();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        OfferRewardEmoteDelay[i] = questRecord[127+i].GetInt32();

    // int32 WDBVerified = questRecord[131].GetInt32();

    if (SpecialFlags & QUEST_SPECIAL_FLAGS_AUTO_ACCEPT)
        Flags |= QUEST_FLAGS_AUTO_ACCEPT;

    _rewItemsCount = 0;
    _rewChoiceItemsCount = 0;
    _rewCurrencyCount = 0;

    for (int i = 0; i < QUEST_REWARDS_COUNT; ++i)
        if (RewardItemId[i])
            ++_rewItemsCount;

    for (int i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        if (RewardChoiceItemId[i])
            ++_rewChoiceItemsCount;

    for (int i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
        if (RewardCurrencyId[i])
            ++_rewCurrencyCount;

    m_questObjecitveTypeCount.assign(QUEST_OBJECTIVE_TYPE_END, 0);
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

int32 Quest::GetRewMoney() const
{
    return int32(RewardMoney * sWorld->getRate(RATE_DROP_MONEY));
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

QuestObjective const* Quest::GetQuestObjective(uint32 objectiveId) const
{
    for (QuestObjectiveSet::const_iterator citr = m_questObjectives.begin(); citr != m_questObjectives.end(); citr++)
        if ((*citr)->Id == objectiveId)
            return *citr;

    return NULL;
}

QuestObjective const* Quest::GetQuestObjectiveXIndex(uint8 index) const
{
    for (QuestObjectiveSet::const_iterator citr = m_questObjectives.begin(); citr != m_questObjectives.end(); citr++)
        if ((*citr)->Index == index)
            return *citr;

    return NULL;
}

QuestObjective const* Quest::GetQuestObjectiveXObjectId(uint32 objectId) const
{
    for (QuestObjectiveSet::const_iterator citr = m_questObjectives.begin(); citr != m_questObjectives.end(); citr++)
        if ((*citr)->ObjectId == objectId)
            return *citr;

    return NULL;
}

uint8 Quest::GetQuestObjectiveCountType(uint8 type) const
{
    if (type >= QUEST_OBJECTIVE_TYPE_END)
        return 0;

    return m_questObjecitveTypeCount[type];
}
