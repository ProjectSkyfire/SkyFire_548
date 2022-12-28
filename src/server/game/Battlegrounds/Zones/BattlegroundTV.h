/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SF_BATTLEGROUNDTV_H
#define SF_BATTLEGROUNDTV_H

#include "Battleground.h"

enum BattlegroundTVObjectTypes
{
    BG_TV_OBJECT_DOOR_1         = 0,
    BG_TV_OBJECT_DOOR_2         = 1,
    BG_TV_OBJECT_BUFF_1         = 2,
    BG_TV_OBJECT_BUFF_2         = 3,
    BG_TV_OBJECT_MAX            = 4
};

enum BattlegroundTVObjects
{
    BG_TV_OBJECT_TYPE_DOOR_1    = 213196,
    BG_TV_OBJECT_TYPE_DOOR_2    = 213197,
    BG_TV_OBJECT_TYPE_BUFF_1    = 184663,
    BG_TV_OBJECT_TYPE_BUFF_2    = 184664
};

class BattlegroundTV : public Battleground
{
    public:
        BattlegroundTV();
        ~BattlegroundTV() { }

        /* inherited from BattlegroundClass */
        void AddPlayer(Player* player) override;
        void StartingEventCloseDoors() override;
        void StartingEventOpenDoors() override;

        void RemovePlayer(Player* player, uint64 guid, uint32 team) override;
        void HandleAreaTrigger(Player* Source, uint32 Trigger) override;
        bool SetupBattleground() override;
        void Reset() override;
        void FillInitialWorldStates(WorldStateBuilder& builder) override;
        void HandleKillPlayer(Player* player, Player* killer) override;

        /* Scorekeeping */
        void UpdatePlayerScore(Player* Source, uint32 type, uint32 value, bool doAddHonor = true) override;
};
#endif
