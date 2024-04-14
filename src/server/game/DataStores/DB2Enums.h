/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef DB2_ENUMS_H
#define DB2_ENUMS_H

enum BattlePetSpeciesFlags
{
    BATTLE_PET_FLAG_NONE = 0x0000,
    BATTLE_PET_FLAG_LIMITED_ABILITIES = 0x0001, // battle pets with less than 6 abilites have this flag
    BATTLE_PET_FLAG_NOT_TRADABLE = 0x0002,
    BATTLE_PET_FLAG_NOT_ACCOUNT_BOUND = 0x0004,
    BATTLE_PET_FLAG_RELEASABLE = 0x0008,
    BATTLE_PET_FLAG_NOT_CAGEABLE = 0x0010,
    BATTLE_PET_FLAG_NOT_TAMEABLE = 0x0020,
    BATTLE_PET_FLAG_UNIQUE = 0x0040,
    BATTLE_PET_FLAG_COMPANION = 0x0080,
    BATTLE_PET_FLAG_UNKNOWN_2 = 0x0100,
    BATTLE_PET_FLAG_UNKNOWN_3 = 0x0200,
    BATTLE_PET_FLAG_ELITE = 0x0400,
    BATTLE_PET_FLAG_UNKNOWN_4 = 0x0800, // battle pets 89, 169 and 218, these also have not tradable flag but don't display message in game
    BATTLE_PET_FLAG_UNKNOWN_5 = 0x1000  // only seen on battle pet 316 (Cenarion Hatchling)
};

#endif
