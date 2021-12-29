/*
 * Copyright (C) 2011-2022 Project SkyFire <https://www.projectskyfire.org/>
 * Copyright (C) 2008-2022 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2022 MaNGOS <https://www.getmangos.eu/>
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

#include "ObjectMgr.h"
#include "WorldPacket.h"
#include "ArenaTeam.h"
#include "World.h"
#include "Group.h"
#include "ArenaTeamMgr.h"
#include "Player.h"
#include "WorldSession.h"
#include "Opcodes.h"

ArenaTeam::ArenaTeam()
    : TeamId(0), Type(0), TeamName(), CaptainGuid(0), BackgroundColor(0), EmblemStyle(0), EmblemColor(0),
    BorderStyle(0), BorderColor(0)
{
    Stats.WeekGames   = 0;
    Stats.SeasonGames = 0;
    Stats.Rank        = 0;
    Stats.Rating      = sWorld->getIntConfig(WorldIntConfigs::CONFIG_ARENA_START_RATING);
    Stats.WeekWins    = 0;
    Stats.SeasonWins  = 0;
}

ArenaTeam::~ArenaTeam()
{ }

void ArenaTeam::SendStats(WorldSession* session)
{
    WorldPacket data(SMSG_ARENA_TEAM_STATS, 4*7);
    data << uint32(GetId());                                // team id
    data << uint32(Stats.Rating);                           // rating
    data << uint32(Stats.WeekGames);                        // games this week
    data << uint32(Stats.WeekWins);                         // wins this week
    data << uint32(Stats.SeasonGames);                      // played this season
    data << uint32(Stats.SeasonWins);                       // wins this season
    data << uint32(Stats.Rank);                             // rank
    session->SendPacket(&data);
}

void ArenaTeam::NotifyStatsChanged()
{
    // This is called after a rated match ended
    // Updates arena team stats for every member of the team (not only the ones who participated!)
    for (MemberList::const_iterator itr = Members.begin(); itr != Members.end(); ++itr)
        if (Player* player = ObjectAccessor::FindPlayer(itr->Guid))
            SendStats(player->GetSession());
}

void ArenaTeamMember::ModifyPersonalRating(Player* player, int32 mod, uint32 type)
{
    if (int32(PersonalRating) + mod < 0)
        PersonalRating = 0;
    else
        PersonalRating += mod;

    if (player)
    {
        //player->SetArenaTeamInfoField(ArenaTeam::GetSlotByType(type), ARENA_TEAM_PERSONAL_RATING, PersonalRating);
        //player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_PERSONAL_RATING, PersonalRating, type);
    }
}

void ArenaTeamMember::ModifyMatchmakerRating(int32 mod, uint32 /*slot*/)
{
    if (int32(MatchMakerRating) + mod < 0)
        MatchMakerRating = 0;
    else
        MatchMakerRating += mod;
}

void ArenaTeam::BroadcastPacket(WorldPacket* packet)
{
    for (MemberList::const_iterator itr = Members.begin(); itr != Members.end(); ++itr)
        if (Player* player = ObjectAccessor::FindPlayer(itr->Guid))
            player->GetSession()->SendPacket(packet);
}

void ArenaTeam::BroadcastEvent(ArenaTeamEvents event, uint64 guid, uint8 strCount, std::string const& str1, std::string const& str2, std::string const& str3)
{
    WorldPacket data(SMSG_ARENA_TEAM_EVENT, 1+1+1);
    data << uint8(event);
    data << uint8(strCount);
    switch (strCount)
    {
        case 0:
            break;
        case 1:
            data << str1;
            break;
        case 2:
            data << str1 << str2;
            break;
        case 3:
            data << str1 << str2 << str3;
            break;
        default:
            SF_LOG_ERROR("bg.arena", "Unhandled strCount %u in ArenaTeam::BroadcastEvent", strCount);
            return;
    }

    if (guid)
        data << uint64(guid);

    BroadcastPacket(&data);

    SF_LOG_DEBUG("network", "WORLD: Sent SMSG_ARENA_TEAM_EVENT");
}

void ArenaTeam::MassInviteToEvent(WorldSession* session)
{
    WorldPacket data(SMSG_CALENDAR_ARENA_TEAM, (Members.size() - 1) * (4 + 8 + 1));
    data << uint32(Members.size() - 1);

    for (MemberList::const_iterator itr = Members.begin(); itr != Members.end(); ++itr)
    {
        if (itr->Guid != session->GetPlayer()->GetGUID())
        {
            data.appendPackGUID(itr->Guid);
            data << uint8(0); // unk
        }
    }

    session->SendPacket(&data);
}

uint8 ArenaTeam::GetSlotByType(uint32 type)
{
    switch (type)
    {
        case ARENA_TEAM_2v2: return 0;
        case ARENA_TEAM_3v3: return 1;
        case ARENA_TEAM_5v5: return 2;
        default:
            break;
    }
    SF_LOG_ERROR("bg.arena", "FATAL: Unknown arena team type %u for some arena team", type);
    return 0xFF;
}

uint8 ArenaTeam::GetTypeBySlot(uint8 slot)
{
    switch (slot)
    {
        case 0: return ARENA_TEAM_2v2;
        case 1: return ARENA_TEAM_3v3;
        case 2: return ARENA_TEAM_5v5;
        default:
            break;
    }
    SF_LOG_ERROR("bg.arena", "FATAL: Unknown arena team slot %u for some arena team", slot);
    return 0xFF;
}

bool ArenaTeam::IsMember(uint64 guid) const
{
    for (MemberList::const_iterator itr = Members.begin(); itr != Members.end(); ++itr)
        if (itr->Guid == guid)
            return true;

    return false;
}

uint32 ArenaTeam::GetAverageMMR(Group* group) const
{
    if (!group)
        return 0;

    uint32 matchMakerRating = 0;
    uint32 playerDivider = 0;
    for (MemberList::const_iterator itr = Members.begin(); itr != Members.end(); ++itr)
    {
        // Skip if player is not online
        if (!ObjectAccessor::FindPlayer(itr->Guid))
            continue;

        // Skip if player is not a member of group
        if (!group->IsMember(itr->Guid))
            continue;

        matchMakerRating += itr->MatchMakerRating;
        ++playerDivider;
    }

    // x/0 = crash
    if (playerDivider == 0)
        playerDivider = 1;

    matchMakerRating /= playerDivider;

    return matchMakerRating;
}

float ArenaTeam::GetChanceAgainst(uint32 ownRating, uint32 opponentRating)
{
    // Returns the chance to win against a team with the given rating, used in the rating adjustment calculation
    // ELO system
    return 1.0f / (1.0f + exp(log(10.0f) * (float)((float)opponentRating - (float)ownRating) / 650.0f));
}

int32 ArenaTeam::GetMatchmakerRatingMod(uint32 ownRating, uint32 opponentRating, bool won)
{
    // 'Chance' calculation - to beat the opponent
    // This is a simulation. Not much info on how it really works
    float chance = GetChanceAgainst(ownRating, opponentRating);
    float won_mod = (won) ? 1.0f : 0.0f;
    float mod = won_mod - chance;

    // Work in progress:
    /*
    // This is a simulation, as there is not much info on how it really works
    float confidence_mod = min(1.0f - fabs(mod), 0.5f);

    // Apply confidence factor to the mod:
    mod *= confidence_factor

    // And only after that update the new confidence factor
    confidence_factor -= ((confidence_factor - 1.0f) * confidence_mod) / confidence_factor;
    */

    // Real rating modification
    mod *= 24.0f;

    return (int32)ceil(mod);
}

int32 ArenaTeam::GetRatingMod(uint32 ownRating, uint32 opponentRating, bool won /*, float confidence_factor*/)
{
    // 'Chance' calculation - to beat the opponent
    // This is a simulation. Not much info on how it really works
    float chance = GetChanceAgainst(ownRating, opponentRating);
    float won_mod = (won) ? 1.0f : 0.0f;

    // Calculate the rating modification
    float mod;

    /// @todo Replace this hack with using the confidence factor (limiting the factor to 2.0f)
    if (won && ownRating < 1300)
    {
        if (ownRating < 1000)
            mod = 48.0f * (won_mod - chance);
        else
            mod = (24.0f + (24.0f * (1300.0f - float(ownRating)) / 300.0f)) * (won_mod - chance);
    }
    else
        mod = 24.0f * (won_mod - chance);

    return (int32)ceil(mod);
}

void ArenaTeam::FinishGame(int32 mod)
{
    // Rating can only drop to 0
    if (int32(Stats.Rating) + mod < 0)
        Stats.Rating = 0;
    else
    {
        Stats.Rating += mod;

        // Check if rating related achivements are met
        for (MemberList::iterator itr = Members.begin(); itr != Members.end(); ++itr)
            if (Player* member = ObjectAccessor::FindPlayer(itr->Guid))
                member->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_TEAM_RATING, Stats.Rating, Type);
    }

    // Update number of games played per season or week
    Stats.WeekGames += 1;
    Stats.SeasonGames += 1;

    // Update team's rank, start with rank 1 and increase until no team with more rating was found
    Stats.Rank = 1;
    ArenaTeamMgr::ArenaTeamContainer::const_iterator i = sArenaTeamMgr->GetArenaTeamMapBegin();
    for (; i != sArenaTeamMgr->GetArenaTeamMapEnd(); ++i)
    {
        if (i->second->GetType() == Type && i->second->GetStats().Rating > Stats.Rating)
            ++Stats.Rank;
    }
}

int32 ArenaTeam::WonAgainst(uint32 ownMMRating, uint32 opponentMMRating, int32& ratingChange)
{
    // Called when the team has won
    // Change in Matchmaker rating
    int32 mod = GetMatchmakerRatingMod(ownMMRating, opponentMMRating, true);

    // Change in Team Rating
    ratingChange = GetRatingMod(Stats.Rating, opponentMMRating, true);

    // Modify the team stats accordingly
    FinishGame(ratingChange);

    // Update number of wins per season and week
    Stats.WeekWins += 1;
    Stats.SeasonWins += 1;

    // Return the rating change, used to display it on the results screen
    return mod;
}

int32 ArenaTeam::LostAgainst(uint32 ownMMRating, uint32 opponentMMRating, int32& ratingChange)
{
    // Called when the team has lost
    // Change in Matchmaker Rating
    int32 mod = GetMatchmakerRatingMod(ownMMRating, opponentMMRating, false);

    // Change in Team Rating
    ratingChange = GetRatingMod(Stats.Rating, opponentMMRating, false);

    // Modify the team stats accordingly
    FinishGame(ratingChange);

    // return the rating change, used to display it on the results screen
    return mod;
}

void ArenaTeam::MemberLost(Player* player, uint32 againstMatchmakerRating, int32 matchmakerRatingChange)
{
    // Called for each participant of a match after losing
    for (MemberList::iterator itr = Members.begin(); itr != Members.end(); ++itr)
    {
        if (itr->Guid == player->GetGUID())
        {
            // Update personal rating
            int32 mod = GetRatingMod(itr->PersonalRating, againstMatchmakerRating, false);
            itr->ModifyPersonalRating(player, mod, GetType());

            // Update matchmaker rating
            itr->ModifyMatchmakerRating(matchmakerRatingChange, GetSlot());

            // Update personal played stats
            itr->WeekGames +=1;
            itr->SeasonGames +=1;

            // update the unit fields
            //player->SetArenaTeamInfoField(GetSlot(), ARENA_TEAM_GAMES_WEEK,  itr->WeekGames);
            //player->SetArenaTeamInfoField(GetSlot(), ARENA_TEAM_GAMES_SEASON,  itr->SeasonGames);
            return;
        }
    }
}

void ArenaTeam::OfflineMemberLost(uint64 guid, uint32 againstMatchmakerRating, int32 matchmakerRatingChange)
{
    // Called for offline player after ending rated arena match!
    for (MemberList::iterator itr = Members.begin(); itr != Members.end(); ++itr)
    {
        if (itr->Guid == guid)
        {
            // update personal rating
            int32 mod = GetRatingMod(itr->PersonalRating, againstMatchmakerRating, false);
            itr->ModifyPersonalRating(NULL, mod, GetType());

            // update matchmaker rating
            itr->ModifyMatchmakerRating(matchmakerRatingChange, GetSlot());

            // update personal played stats
            itr->WeekGames += 1;
            itr->SeasonGames += 1;
            return;
        }
    }
}

void ArenaTeam::MemberWon(Player* player, uint32 againstMatchmakerRating, int32 matchmakerRatingChange)
{
    // called for each participant after winning a match
    for (MemberList::iterator itr = Members.begin(); itr != Members.end(); ++itr)
    {
        if (itr->Guid == player->GetGUID())
        {
            // update personal rating
            int32 mod = GetRatingMod(itr->PersonalRating, againstMatchmakerRating, true);
            itr->ModifyPersonalRating(player, mod, GetType());

            // update matchmaker rating
            itr->ModifyMatchmakerRating(matchmakerRatingChange, GetSlot());

            // update personal stats
            itr->WeekGames +=1;
            itr->SeasonGames +=1;
            itr->SeasonWins += 1;
            itr->WeekWins += 1;
            // update unit fields
            //player->SetArenaTeamInfoField(GetSlot(), ARENA_TEAM_GAMES_WEEK, itr->WeekGames);
            //player->SetArenaTeamInfoField(GetSlot(), ARENA_TEAM_GAMES_SEASON, itr->SeasonGames);
            return;
        }
    }
}


void ArenaTeam::FinishWeek()
{
    // Reset team stats
    Stats.WeekGames = 0;
    Stats.WeekWins = 0;

    // Reset member stats
    for (MemberList::iterator itr = Members.begin(); itr != Members.end(); ++itr)
    {
        itr->WeekGames = 0;
        itr->WeekWins = 0;
    }
}

bool ArenaTeam::IsFighting() const
{
    for (MemberList::const_iterator itr = Members.begin(); itr != Members.end(); ++itr)
        if (Player* player = ObjectAccessor::FindPlayer(itr->Guid))
            if (player->GetMap()->IsBattleArena())
                return true;

    return false;
}

ArenaTeamMember* ArenaTeam::GetMember(const std::string& name)
{
    for (MemberList::iterator itr = Members.begin(); itr != Members.end(); ++itr)
        if (itr->Name == name)
            return &(*itr);

    return NULL;
}

ArenaTeamMember* ArenaTeam::GetMember(uint64 guid)
{
    for (MemberList::iterator itr = Members.begin(); itr != Members.end(); ++itr)
        if (itr->Guid == guid)
            return &(*itr);

    return NULL;
}
