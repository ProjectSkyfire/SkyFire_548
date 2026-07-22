/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_LFGGROUPDATA_H
#define SF_LFGGROUPDATA_H

#include "LFG.h"
#include <map>

namespace lfg
{

    enum LfgGroupEnum
    {
        LFG_GROUP_MAX_KICKS = 3,
    };

    struct LfgGroupQueueData
    {
        LfgGroupQueueData();

        LfgState State;                                     ///< Current state in this queue
        LfgState OldState;                                  ///< Old state
        uint32 Dungeon;                                     ///< Dungeon entry
    };

    typedef std::map<uint8, LfgGroupQueueData> LfgGroupQueueDataContainer;

    /**
        Stores all lfg data needed about a group.
    */
    class LfgGroupData
    {
    public:
        LfgGroupData();
        ~LfgGroupData();

        bool IsLfgGroup() const;

        // General
        void SetState(LfgState state);
        void RestoreState();
        void AddPlayer(uint64 guid);
        uint8 RemovePlayer(uint64 guid);
        void RemoveAllPlayers();
        void SetLeader(uint64 guid);
        void SetActiveQueueId(uint8 queueId);

        // Dungeon
        void SetDungeon(uint32 dungeon);

        // VoteKick
        void DecreaseKicksLeft();

        // General
        LfgState GetState() const;
        LfgState GetOldState() const;
        LfgGuidSet const& GetPlayers() const;
        uint8 GetPlayerCount() const;
        uint64 GetLeader() const;
        uint8 GetActiveQueueId() const;
        LfgGroupQueueDataContainer const& GetQueues() const;

        // Dungeon
        uint32 GetDungeon(bool asId = true) const;

        // VoteKick
        uint8 GetKicksLeft() const;
        void SetVoteKick(bool active);
        bool IsVoteKickActive() const;

    private:
        LfgGroupQueueData& GetActiveQueueData();
        LfgGroupQueueData const& GetActiveQueueData() const;

        // General
        uint64 m_Leader;                                   ///< Leader GUID
        LfgGuidSet m_Players;                              ///< Players in group
        uint8 m_ActiveQueueId;                             ///< Active queue data owner
        LfgGroupQueueDataContainer m_Queues;               ///< Queue-scoped group data
        // Vote Kick
        uint8 m_KicksLeft;                                 ///< Number of kicks left
        bool m_VoteKickActive;
    };

} // namespace lfg

#endif
