/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SF_BATTLEGROUNDNA_H
#define SF_BATTLEGROUNDNA_H

#include "Battleground.h"

enum BattlegroundNAObjectTypes
{
    BG_NA_OBJECT_DOOR_1         = 0,
    BG_NA_OBJECT_DOOR_2         = 1,
    BG_NA_OBJECT_DOOR_3         = 2,
    BG_NA_OBJECT_DOOR_4         = 3,
    BG_NA_OBJECT_BUFF_1         = 4,
    BG_NA_OBJECT_BUFF_2         = 5,
    BG_NA_OBJECT_MAX            = 6
};

enum BattlegroundNAObjects
{
    BG_NA_OBJECT_TYPE_DOOR_1    = 183978,
    BG_NA_OBJECT_TYPE_DOOR_2    = 183980,
    BG_NA_OBJECT_TYPE_DOOR_3    = 183977,
    BG_NA_OBJECT_TYPE_DOOR_4    = 183979,
    BG_NA_OBJECT_TYPE_BUFF_1    = 184663,
    BG_NA_OBJECT_TYPE_BUFF_2    = 184664
};

class BattlegroundNA : public Battleground
{
    public:
        BattlegroundNA();
        ~BattlegroundNA() { }

        /* inherited from BattlegroundClass */
        void AddPlayer(Player* player) OVERRIDE;
        void StartingEventCloseDoors() OVERRIDE;
        void StartingEventOpenDoors() OVERRIDE;

        void RemovePlayer(Player* player, uint64 guid, uint32 team) OVERRIDE;
        void HandleAreaTrigger(Player* Source, uint32 Trigger) OVERRIDE;
        bool SetupBattleground() OVERRIDE;
        void Reset() OVERRIDE;
        void FillInitialWorldStates(WorldStateBuilder &builder) OVERRIDE;
        void HandleKillPlayer(Player* player, Player* killer) OVERRIDE;
};
#endif
