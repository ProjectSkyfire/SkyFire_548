/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_BATTLEGROUNDBE_H
#define SF_BATTLEGROUNDBE_H

#include "Battleground.h"

enum BattlegroundBEObjectTypes
{
    BG_BE_OBJECT_DOOR_1 = 0,
    BG_BE_OBJECT_DOOR_2 = 1,
    BG_BE_OBJECT_DOOR_3 = 2,
    BG_BE_OBJECT_DOOR_4 = 3,
    BG_BE_OBJECT_BUFF_1 = 4,
    BG_BE_OBJECT_BUFF_2 = 5,
    BG_BE_OBJECT_MAX = 6
};

enum BattlegroundBEObjects
{
    BG_BE_OBJECT_TYPE_DOOR_1 = 183971,
    BG_BE_OBJECT_TYPE_DOOR_2 = 183973,
    BG_BE_OBJECT_TYPE_DOOR_3 = 183970,
    BG_BE_OBJECT_TYPE_DOOR_4 = 183972,
    BG_BE_OBJECT_TYPE_BUFF_1 = 184663,
    BG_BE_OBJECT_TYPE_BUFF_2 = 184664
};

class BattlegroundBE : public Battleground
{
public:
    BattlegroundBE();
    ~BattlegroundBE() { }

    /* inherited from BattlegroundClass */
    void AddPlayer(Player* player) OVERRIDE;
    void StartingEventCloseDoors() OVERRIDE;
    void StartingEventOpenDoors() OVERRIDE;

    void RemovePlayer(Player* player, uint64 guid, uint32 team) OVERRIDE;
    void HandleAreaTrigger(Player* Source, uint32 Trigger) OVERRIDE;
    bool SetupBattleground() OVERRIDE;
    void Reset() OVERRIDE;
    void FillInitialWorldStates(WorldStateBuilder& builder) OVERRIDE;
    void HandleKillPlayer(Player* player, Player* killer) OVERRIDE;

    /* Scorekeeping */
    void UpdatePlayerScore(Player* Source, uint32 type, uint32 value, bool doAddHonor = true) OVERRIDE;
};
#endif
