/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef DEF_STEAM_VAULT_H
#define DEF_STEAM_VAULT_H

#define SteamVaultScriptName "instance_steam_vault"

uint32 const EncounterCount = 3;

enum DataTypes
{
    DATA_HYDROMANCER_THESPIA        = 0,
    DATA_MEKGINEER_STEAMRIGGER      = 1,
    DATA_WARLORD_KALITHRESH         = 2,
    DATA_DISTILLER                  = 3
};

enum CreatureIds
{
    NPC_HYDROMANCER_THESPIA         = 17797,
    NPC_MEKGINEER_STEAMRIGGER       = 17796,
    NPC_WARLORD_KALITHRESH          = 17798
};

enum GameObjectIds
{
    GO_MAIN_CHAMBERS_DOOR           = 183049,
    GO_ACCESS_PANEL_HYDRO           = 184125,
    GO_ACCESS_PANEL_MEK             = 184126
};

template<class AI>
AI* GetSteamVaultAI(Creature* creature)
{
    return GetInstanceAI<AI>(creature, SteamVaultScriptName);
}

#endif
