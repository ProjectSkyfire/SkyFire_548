/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SF_STORMSTOUT_BREWERY_H_
#define SF_STORMSTOUT_BREWERY_H_

#define StormstoutBreweryScriptName "instance_stormstout_brewery"

uint32 const EncounterCount = 5;

enum DataTypes
{
    // Encounter States/Boss GUIDs
    DATA_BANANA_EVENT = 0,
    DATA_OOK_OOK      = 1,
    DATA_RABBIT_EVENT = 2,
    DATA_HOPTALLUS    = 3,
    DATA_YAN_ZHU      = 4,
};

enum CreatureIds
{
    NPC_OOK_OOK       = 56637,
    NPC_ANIMAL1       = 56927,
    NPC_ANIMAL2       = 59684,

    NPC_HOPTALLUS     = 56717,
    NPC_YAN_ZHU       = 59479,
};

enum GameObjectIds
{
    // doors
};

Position const OokOokLandPos = { -754.695f, 1348.25f, 147.35f, 1.83f };
Position const BananaFleePos = { -771.177f, 1407.86f, 139.57f, 1.86f };

Position const HoptallusLandPos = { -697.232f, 1260.42f, 162.79f, 0.33f };

template<class AI>
AI* GetStormstoutBreweryAI(Creature* creature)
{
    return GetInstanceAI<AI>(creature, StormstoutBreweryScriptName);
}

#endif // SF_STORMSTOUT_BREWERY_H_