/*
 * Copyright (C) 2011-2021 Project SkyFire <https://www.projectskyfire.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
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