/*
 * Copyright (C) 2011-2020 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2020 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2020 MaNGOS <https://www.getmangos.eu/>
 * Copyright (C) 2006-2014 ScriptDev2 <https://github.com/scriptdev2/scriptdev2/>
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

#ifndef JADE_SERPENT_H_
#define JADE_SERPENT_H_

#define JadeSerpentScriptName "instance_temple_of_jade_serpent"

uint32 const EncounterCount = 4;

enum DataTypes
{
    // Encounter States/Boss GUIDs
    DATA_WISE_MARI                  = 0,
    DATA_STONESTEP                  = 1,
    DATA_FLAMEHEART                 = 2,
    DATA_SHA_OF_DOUBT               = 3,
};

enum CreatureIds
{
    NPC_WISE_MARI                   = 56448,
    NPC_STONESTEP                   = 56843,
    NPC_FLAMEHEART                  = 56732,
    NPC_SHA_OF_DOUBT                = 56439,
};

enum GameObjectIds
{
    // doors
};

template<class AI>
AI* GetJadeSerpentAI(Creature* creature)
{
    return GetInstanceAI<AI>(creature, JadeSerpentScriptName);
}

#endif // JADE_SERPENT_H_