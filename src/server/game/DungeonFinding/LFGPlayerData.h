/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SF_LFGPLAYERDATA_H
#define SF_LFGPLAYERDATA_H

#include "LFG.h"

namespace lfg
{

/**
    Stores all lfg data needed about the player.
*/
class LfgPlayerData
{
    public:
        LfgPlayerData();
        ~LfgPlayerData();

        // General
        void SetState(LfgState state);
        void RestoreState();
        void SetTeam(uint8 team);
        void SetGroup(uint64 group);

        // Queue
        void SetRoles(uint8 roles);
        void SetComment(std::string const& comment);
        void SetSelectedDungeons(const LfgDungeonSet& dungeons);

        // General
        LfgState GetState() const;
        LfgState GetOldState() const;
        uint8 GetTeam() const;
        uint64 GetGroup() const;

        // Queue
        uint8 GetRoles() const;
        std::string const& GetComment() const;
        LfgDungeonSet const& GetSelectedDungeons() const;

    private:
        // General
        LfgState m_State;                                  ///< State if group in LFG
        LfgState m_OldState;                               ///< Old State - Used to restore state after failed Rolecheck/Proposal
        // Player
        uint8 m_Team;                                      ///< Player team - determines the queue to join
        uint64 m_Group;                                    ///< Original group of player when joined LFG

        // Queue
        uint8 m_Roles;                                     ///< Roles the player selected when joined LFG
        std::string m_Comment;                             ///< Player comment used when joined LFG
        LfgDungeonSet m_SelectedDungeons;                  ///< Selected Dungeons when joined LFG
};

} // namespace lfg

#endif
