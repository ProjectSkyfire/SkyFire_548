/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "LFGPlayerData.h"

namespace lfg
{

    LfgPlayerData::LfgPlayerData() : m_State(LFG_STATE_NONE), m_OldState(LFG_STATE_NONE),
        m_Team(0), m_Group(0), m_Roles(0), m_Comment("")
    { }

    LfgPlayerData::~LfgPlayerData() { }

    void LfgPlayerData::SetState(LfgState state)
    {
        switch (state)
        {
            case LFG_STATE_NONE:
            case LFG_STATE_FINISHED_DUNGEON:
                m_Roles = 0;
                m_SelectedDungeons.clear();
                m_Comment.clear();
                // No break on purpose
            case LFG_STATE_DUNGEON:
                m_OldState = state;
                // No break on purpose
            default:
                m_State = state;
        }
    }

    void LfgPlayerData::RestoreState()
    {
        if (m_OldState == LFG_STATE_NONE)
        {
            m_SelectedDungeons.clear();
            m_Roles = 0;
        }
        m_State = m_OldState;
    }

    void LfgPlayerData::SetTeam(uint8 team)
    {
        m_Team = team;
    }

    void LfgPlayerData::SetGroup(uint64 group)
    {
        m_Group = group;
    }

    void LfgPlayerData::SetRoles(uint8 roles)
    {
        m_Roles = roles;
    }

    void LfgPlayerData::SetComment(std::string const& comment)
    {
        m_Comment = comment;
    }

    void LfgPlayerData::SetSelectedDungeons(LfgDungeonSet const& dungeons)
    {
        m_SelectedDungeons = dungeons;
    }

    LfgState LfgPlayerData::GetState() const
    {
        return m_State;
    }

    LfgState LfgPlayerData::GetOldState() const
    {
        return m_OldState;
    }

    uint8 LfgPlayerData::GetTeam() const
    {
        return m_Team;
    }

    uint64 LfgPlayerData::GetGroup() const
    {
        return m_Group;
    }

    uint8 LfgPlayerData::GetRoles() const
    {
        return m_Roles;
    }

    std::string const& LfgPlayerData::GetComment() const
    {
        return m_Comment;
    }

    LfgDungeonSet const& LfgPlayerData::GetSelectedDungeons() const
    {
        return m_SelectedDungeons;
    }

} // namespace lfg
