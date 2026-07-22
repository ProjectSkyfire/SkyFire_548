/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "LFGPlayerData.h"

namespace lfg
{
    namespace
    {
        LfgPlayerQueueData const& EmptyPlayerQueueData()
        {
            static LfgPlayerQueueData empty;
            return empty;
        }
    }

    LfgReturnLocation::LfgReturnLocation() : IsSet(false), MapId(0), X(0.0f), Y(0.0f), Z(0.0f), O(0.0f)
    { }

    LfgPlayerQueueData::LfgPlayerQueueData() : State(LFG_STATE_NONE), OldState(LFG_STATE_NONE),
        Roles(0), Comment("")
    { }

    LfgPlayerData::LfgPlayerData() : m_Team(0), m_Group(0), m_ActiveQueueId(0)
    { }

    LfgPlayerData::~LfgPlayerData() { }

    void LfgPlayerData::SetState(LfgState state)
    {
        LfgPlayerQueueData& data = GetActiveQueueData();

        switch (state)
        {
            case LFG_STATE_NONE:
                data.State = LFG_STATE_NONE;
                data.OldState = LFG_STATE_NONE;
                m_Queues.erase(m_ActiveQueueId);
                m_ActiveQueueId = 0;
                return;
            case LFG_STATE_FINISHED_DUNGEON:
                data.Roles = 0;
                data.SelectedDungeons.clear();
                data.Comment.clear();
                // No break on purpose
            case LFG_STATE_DUNGEON:
                data.OldState = state;
                // No break on purpose
            default:
                data.State = state;
        }
    }

    void LfgPlayerData::RestoreState()
    {
        LfgPlayerQueueData& data = GetActiveQueueData();

        if (data.OldState == LFG_STATE_NONE)
        {
            data.SelectedDungeons.clear();
            data.Roles = 0;
        }

        data.State = data.OldState;
    }

    void LfgPlayerData::SetTeam(uint8 team)
    {
        m_Team = team;
    }

    void LfgPlayerData::SetGroup(uint64 group)
    {
        m_Group = group;
    }

    void LfgPlayerData::SetActiveQueueId(uint8 queueId)
    {
        m_ActiveQueueId = queueId;
        GetActiveQueueData();
    }

    void LfgPlayerData::SetRoles(uint8 roles)
    {
        GetActiveQueueData().Roles = roles;
    }

    void LfgPlayerData::SetComment(std::string const& comment)
    {
        GetActiveQueueData().Comment = comment;
    }

    void LfgPlayerData::SetSelectedDungeons(LfgDungeonSet const& dungeons)
    {
        GetActiveQueueData().SelectedDungeons = dungeons;
    }

    void LfgPlayerData::SetReturnLocation(uint32 mapId, float x, float y, float z, float o)
    {
        LfgReturnLocation& location = GetActiveQueueData().ReturnLocation;
        location.IsSet = true;
        location.MapId = mapId;
        location.X = x;
        location.Y = y;
        location.Z = z;
        location.O = o;
    }

    void LfgPlayerData::ClearReturnLocation()
    {
        LfgPlayerQueueDataContainer::iterator itr = m_Queues.find(m_ActiveQueueId);
        if (itr != m_Queues.end())
            itr->second.ReturnLocation = LfgReturnLocation();
    }

    LfgState LfgPlayerData::GetState() const
    {
        return GetActiveQueueData().State;
    }

    LfgState LfgPlayerData::GetOldState() const
    {
        return GetActiveQueueData().OldState;
    }

    uint8 LfgPlayerData::GetTeam() const
    {
        return m_Team;
    }

    uint64 LfgPlayerData::GetGroup() const
    {
        return m_Group;
    }

    uint8 LfgPlayerData::GetActiveQueueId() const
    {
        return m_ActiveQueueId;
    }

    LfgPlayerQueueDataContainer const& LfgPlayerData::GetQueues() const
    {
        return m_Queues;
    }

    uint8 LfgPlayerData::GetRoles() const
    {
        return GetActiveQueueData().Roles;
    }

    std::string const& LfgPlayerData::GetComment() const
    {
        return GetActiveQueueData().Comment;
    }

    LfgDungeonSet const& LfgPlayerData::GetSelectedDungeons() const
    {
        return GetActiveQueueData().SelectedDungeons;
    }

    LfgReturnLocation const& LfgPlayerData::GetReturnLocation() const
    {
        return GetActiveQueueData().ReturnLocation;
    }

    LfgPlayerQueueData& LfgPlayerData::GetActiveQueueData()
    {
        return m_Queues[m_ActiveQueueId];
    }

    LfgPlayerQueueData const& LfgPlayerData::GetActiveQueueData() const
    {
        LfgPlayerQueueDataContainer::const_iterator itr = m_Queues.find(m_ActiveQueueId);
        if (itr != m_Queues.end())
            return itr->second;

        return EmptyPlayerQueueData();
    }

} // namespace lfg
