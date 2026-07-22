/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "LFG.h"
#include "LFGGroupData.h"

namespace lfg
{
    namespace
    {
        LfgGroupQueueData const& EmptyGroupQueueData()
        {
            static LfgGroupQueueData empty;
            return empty;
        }
    }

    LfgGroupQueueData::LfgGroupQueueData() : State(LFG_STATE_NONE), OldState(LFG_STATE_NONE), Dungeon(0)
    { }

    LfgGroupData::LfgGroupData() : m_Leader(0), m_ActiveQueueId(0), m_KicksLeft(LFG_GROUP_MAX_KICKS),
        m_VoteKickActive(false)
    { }

    LfgGroupData::~LfgGroupData()
    { }

    bool LfgGroupData::IsLfgGroup() const
    {
        return GetOldState() != LFG_STATE_NONE;
    }

    void LfgGroupData::SetState(LfgState state)
    {
        LfgGroupQueueData& data = GetActiveQueueData();

        switch (state)
        {
            case LFG_STATE_NONE:
                data.State = LFG_STATE_NONE;
                data.OldState = LFG_STATE_NONE;
                data.Dungeon = 0;
                m_Queues.erase(m_ActiveQueueId);
                m_ActiveQueueId = 0;
                m_KicksLeft = LFG_GROUP_MAX_KICKS;
                return;
            case LFG_STATE_FINISHED_DUNGEON:
            case LFG_STATE_DUNGEON:
                data.OldState = state;
                // No break on purpose
            default:
                data.State = state;
        }
    }

    void LfgGroupData::RestoreState()
    {
        LfgGroupQueueData& data = GetActiveQueueData();
        data.State = data.OldState;
    }

    void LfgGroupData::AddPlayer(uint64 guid)
    {
        m_Players.insert(guid);
    }

    uint8 LfgGroupData::RemovePlayer(uint64 guid)
    {
        LfgGuidSet::iterator it = m_Players.find(guid);
        if (it != m_Players.end())
            m_Players.erase(it);
        return uint8(m_Players.size());
    }

    void LfgGroupData::RemoveAllPlayers()
    {
        m_Players.clear();
    }

    void LfgGroupData::SetLeader(uint64 guid)
    {
        m_Leader = guid;
    }

    void LfgGroupData::SetActiveQueueId(uint8 queueId)
    {
        m_ActiveQueueId = queueId;
        GetActiveQueueData();
    }

    void LfgGroupData::SetDungeon(uint32 dungeon)
    {
        GetActiveQueueData().Dungeon = dungeon;
    }

    void LfgGroupData::DecreaseKicksLeft()
    {
        if (m_KicksLeft)
            --m_KicksLeft;
    }

    LfgState LfgGroupData::GetState() const
    {
        return GetActiveQueueData().State;
    }

    LfgState LfgGroupData::GetOldState() const
    {
        return GetActiveQueueData().OldState;
    }

    LfgGuidSet const& LfgGroupData::GetPlayers() const
    {
        return m_Players;
    }

    uint8 LfgGroupData::GetPlayerCount() const
    {
        return m_Players.size();
    }

    uint64 LfgGroupData::GetLeader() const
    {
        return m_Leader;
    }

    uint8 LfgGroupData::GetActiveQueueId() const
    {
        return m_ActiveQueueId;
    }

    LfgGroupQueueDataContainer const& LfgGroupData::GetQueues() const
    {
        return m_Queues;
    }

    uint32 LfgGroupData::GetDungeon(bool asId /* = true */) const
    {
        uint32 dungeon = GetActiveQueueData().Dungeon;
        if (asId)
            return (dungeon & 0x00FFFFFF);
        else
            return dungeon;
    }

    uint8 LfgGroupData::GetKicksLeft() const
    {
        return m_KicksLeft;
    }

    void LfgGroupData::SetVoteKick(bool active)
    {
        m_VoteKickActive = active;
    }
    bool LfgGroupData::IsVoteKickActive() const
    {
        return m_VoteKickActive;
    }

    LfgGroupQueueData& LfgGroupData::GetActiveQueueData()
    {
        return m_Queues[m_ActiveQueueId];
    }

    LfgGroupQueueData const& LfgGroupData::GetActiveQueueData() const
    {
        LfgGroupQueueDataContainer::const_iterator itr = m_Queues.find(m_ActiveQueueId);
        if (itr != m_Queues.end())
            return itr->second;

        return EmptyGroupQueueData();
    }

} // namespace lfg
