/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRESERVER_ARENATEAM_H
#define SKYFIRESERVER_ARENATEAM_H

#include "Define.h"
#include "QueryResult.h"
#include <list>
#include <string>
#include <map>

class WorldSession;
class WorldPacket;
class Player;
class Group;

enum ArenaTeamCommandTypes
{
    ERR_ARENA_TEAM_CREATE_S                 = 0x00,
    ERR_ARENA_TEAM_INVITE_SS                = 0x01,
    ERR_ARENA_TEAM_QUIT_S                   = 0x03,
    ERR_ARENA_TEAM_FOUNDER_S                = 0x0E
};

enum ArenaTeamCommandErrors
{
    ERR_ARENA_TEAM_CREATED                  = 0x00,
    ERR_ARENA_TEAM_INTERNAL                 = 0x01,
    ERR_ALREADY_IN_ARENA_TEAM               = 0x02,
    ERR_ALREADY_IN_ARENA_TEAM_S             = 0x03,
    ERR_INVITED_TO_ARENA_TEAM               = 0x04,
    ERR_ALREADY_INVITED_TO_ARENA_TEAM_S     = 0x05,
    ERR_ARENA_TEAM_NAME_INVALID             = 0x06,
    ERR_ARENA_TEAM_NAME_EXISTS_S            = 0x07,
    ERR_ARENA_TEAM_LEADER_LEAVE_S           = 0x08,
    ERR_ARENA_TEAM_PERMISSIONS              = 0x08,
    ERR_ARENA_TEAM_PLAYER_NOT_IN_TEAM       = 0x09,
    ERR_ARENA_TEAM_PLAYER_NOT_IN_TEAM_SS    = 0x0A,
    ERR_ARENA_TEAM_PLAYER_NOT_FOUND_S       = 0x0B,
    ERR_ARENA_TEAM_NOT_ALLIED               = 0x0C,
    ERR_ARENA_TEAM_IGNORING_YOU_S           = 0x13,
    ERR_ARENA_TEAM_TARGET_TOO_LOW_S         = 0x15,
    ERR_ARENA_TEAM_TARGET_TOO_HIGH_S        = 0x16,
    ERR_ARENA_TEAM_TOO_MANY_MEMBERS_S       = 0x17,
    ERR_ARENA_TEAM_NOT_FOUND                = 0x1B,
    ERR_ARENA_TEAMS_LOCKED                  = 0x1E,
    ERR_ARENA_TEAM_TOO_MANY_CREATE          = 0x21,
};

enum ArenaTeamEvents
{
    ERR_ARENA_TEAM_JOIN_SS                  = 3,            // player name + arena team name
    ERR_ARENA_TEAM_LEAVE_SS                 = 4,            // player name + arena team name
    ERR_ARENA_TEAM_REMOVE_SSS               = 5,            // player name + arena team name + captain name
    ERR_ARENA_TEAM_LEADER_IS_SS             = 6,            // player name + arena team name
    ERR_ARENA_TEAM_LEADER_CHANGED_SSS       = 7,            // old captain + new captain + arena team name
    ERR_ARENA_TEAM_DISBANDED_S              = 8             // captain name + arena team name
};

enum ArenaTeamTypes
{
    ARENA_TEAM_2v2      = 2,
    ARENA_TEAM_3v3      = 3,
    ARENA_TEAM_5v5      = 5
};

struct ArenaTeamMember
{
    uint64 Guid;
    std::string Name;
    uint8 Class;
    uint16 WeekGames;
    uint16 WeekWins;
    uint16 SeasonGames;
    uint16 SeasonWins;
    uint16 PersonalRating;
    uint16 MatchMakerRating;

    void ModifyPersonalRating(Player* player, int32 mod, uint32 type);
    void ModifyMatchmakerRating(int32 mod, uint32 slot);
};

struct ArenaTeamStats
{
    uint16 Rating;
    uint16 WeekGames;
    uint16 WeekWins;
    uint16 SeasonGames;
    uint16 SeasonWins;
    uint32 Rank;
};

#define MAX_ARENA_SLOT 3                                    // 0..2 slots

class ArenaTeam
{
    public:
        ArenaTeam();
        ~ArenaTeam();

        typedef std::list<ArenaTeamMember> MemberList;

        uint32 GetId() const { return TeamId; }
        uint32 GetType() const { return Type; }
        uint8  GetSlot() const { return GetSlotByType(GetType()); }
        static uint8 GetSlotByType(uint32 type);
        static uint8 GetTypeBySlot(uint8 slot);
        uint64 GetCaptain() const { return CaptainGuid; }
        std::string const& GetName() const { return TeamName; }
        const ArenaTeamStats& GetStats() const { return Stats; }

        uint32 GetRating() const          { return Stats.Rating; }
        uint32 GetAverageMMR(Group* group) const;

        size_t GetMembersSize() const         { return Members.size(); }
        bool   Empty() const                  { return Members.empty(); }
        MemberList::iterator m_membersBegin() { return Members.begin(); }
        MemberList::iterator m_membersEnd()   { return Members.end(); }
        bool IsMember(uint64 guid) const;

        ArenaTeamMember* GetMember(uint64 guid);
        ArenaTeamMember* GetMember(std::string const& name);

        bool IsFighting() const;

        void BroadcastPacket(WorldPacket* packet);
        void BroadcastEvent(ArenaTeamEvents event, uint64 guid, uint8 strCount, std::string const& str1, std::string const& str2, std::string const& str3);
        void NotifyStatsChanged();

        void MassInviteToEvent(WorldSession* session);

        void SendStats(WorldSession* session);

        static int32 GetMatchmakerRatingMod(uint32 ownRating, uint32 opponentRating, bool won);
        static int32 GetRatingMod(uint32 ownRating, uint32 opponentRating, bool won);
        static float GetChanceAgainst(uint32 ownRating, uint32 opponentRating);

        int32 WonAgainst(uint32 ownMMRating, uint32 opponentMMRating, int32& rating_change);
        void MemberWon(Player* player, uint32 againstMatchmakerRating, int32 matchmakerRatingChange = 12);

        int32 LostAgainst(uint32 ownMMRating, uint32 opponentMMRating, int32& rating_change);
        void MemberLost(Player* player, uint32 againstMatchmakerRating, int32 matchmakerRatingChange = -12);
        void OfflineMemberLost(uint64 guid, uint32 againstMatchmakerRating, int32 matchmakerRatingChange = -12);

        void FinishWeek();
        void FinishGame(int32 mod);

    protected:
        uint32 TeamId;
        uint8  Type;
        std::string TeamName;
        uint64 CaptainGuid;

        uint32 BackgroundColor; // ARGB format
        uint8  EmblemStyle;     // icon id
        uint32 EmblemColor;     // ARGB format
        uint8  BorderStyle;     // border image id
        uint32 BorderColor;     // ARGB format

        MemberList Members;
        ArenaTeamStats Stats;
};
#endif
