/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_LFGPLAYERDATA_H
#define SF_LFGPLAYERDATA_H

#include "LFG.h"
#include <map>

namespace lfg
{
    struct LfgReturnLocation
    {
        LfgReturnLocation();

        bool IsSet;
        uint32 MapId;
        float X;
        float Y;
        float Z;
        float O;
    };

    struct LfgPlayerQueueData
    {
        LfgPlayerQueueData();

        LfgState State;                                     ///< Current state in this queue
        LfgState OldState;                                  ///< Old state for rolecheck/proposal restore
        uint8 Roles;                                        ///< Roles the player selected when joined LFG
        std::string Comment;                                ///< Player comment used when joined LFG
        LfgDungeonSet SelectedDungeons;                     ///< Selected dungeons when joined LFG
        LfgReturnLocation ReturnLocation;                   ///< Player location before teleporting into LFG
    };

    typedef std::map<uint8, LfgPlayerQueueData> LfgPlayerQueueDataContainer;

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
        void SetActiveQueueId(uint8 queueId);

        // Queue
        void SetRoles(uint8 roles);
        void SetComment(std::string const& comment);
        void SetSelectedDungeons(const LfgDungeonSet& dungeons);
        void SetReturnLocation(uint32 mapId, float x, float y, float z, float o);
        void ClearReturnLocation();

        // General
        LfgState GetState() const;
        LfgState GetOldState() const;
        uint8 GetTeam() const;
        uint64 GetGroup() const;
        uint8 GetActiveQueueId() const;
        LfgPlayerQueueDataContainer const& GetQueues() const;

        // Queue
        uint8 GetRoles() const;
        std::string const& GetComment() const;
        LfgDungeonSet const& GetSelectedDungeons() const;
        LfgReturnLocation const& GetReturnLocation() const;

    private:
        LfgPlayerQueueData& GetActiveQueueData();
        LfgPlayerQueueData const& GetActiveQueueData() const;

        // General
        uint8 m_Team;                                      ///< Player team - determines the queue to join
        uint64 m_Group;                                    ///< Original group of player when joined LFG

        // Queue
        uint8 m_ActiveQueueId;                             ///< Active queue data owner
        LfgPlayerQueueDataContainer m_Queues;              ///< Queue-scoped player data
    };

} // namespace lfg

#endif
