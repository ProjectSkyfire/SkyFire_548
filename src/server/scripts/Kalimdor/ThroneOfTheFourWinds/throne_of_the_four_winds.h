/*
* This file is part of the Pandaria 5.4.8 Project. See THANKS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
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

#ifndef DEF_THRONEOFTHEFOURWINDS_H
#define DEF_THRONEOFTHEFOURWINDS_H

#include "ScriptedCreature.h"

enum Data
{
    DATA_CONCLAVE_OF_WIND           = 0,
    DATA_ALAKIR                     = 1,

    DATA_HEART_OF_WIND_10N          = 2,
    DATA_HEART_OF_WIND_25N          = 3,
    DATA_HEART_OF_WIND_10H          = 4,
    DATA_HEART_OF_WIND_25H          = 5,
};

enum CreatureIds
{
    BOSS_ANSHAL                     = 45870,
    BOSS_NEZIR                      = 45871,
    BOSS_ROHASH                     = 45872,
    BOSS_ALAKIR                     = 46753,

    NPC_SLIPSTREAM                  = 47066,
    NPC_WORLD_TRIGGER_CATCH_FALL    = 21252,
};

enum InstanceSpells
{
    // Council of Wind
    SPELL_PRE_COMBAT_EFFECT_ANSHAL  = 85537,
    SPELL_PRE_COMBAT_EFFECT_NEZIR   = 85532,
    SPELL_PRE_COMBAT_EFFECT_ROHASH  = 85538,

    // Fall Catcher
    SPELL_CATCH_FALL_SUMMON_TRIGGER = 85268,
    SPELL_CATCH_FALL_SUMMON_AURA    = 85269,
    SPELL_CATCH_FALL_VEHICLE        = 85282,
    SPELL_CATCH_FALL_JUMP_TRIGGER   = 85274,

    // Al'Akir
    SPELL_EYE_OF_THE_STORM          = 82724,
};

enum ObjectIds
{
    GO_FLOOR_ALAKIR                     = 207737,
    GO_HEART_OF_WIND_10N                = 207891,
    GO_HEART_OF_WIND_25N                = 207892,
    GO_HEART_OF_WIND_10H                = 207893,
    GO_HEART_OF_WIND_25H                = 207894,

    GO_WIND_DRAFTEFFECT_CENTER          = 207922,

    GO_WIND_DRAFTEFFECT_1               = 207923,
    GO_WIND_DRAFTEFFECT_2               = 207924,
    GO_WIND_DRAFTEFFECT_3               = 207925,
    GO_WIND_DRAFTEFFECT_4               = 207926,
    GO_WIND_DRAFTEFFECT_5               = 207929,
    GO_WIND_DRAFTEFFECT_6               = 207930,
    GO_WIND_DRAFTEFFECT_7               = 207927,
    GO_WIND_DRAFTEFFECT_8               = 207928,

    GO_DJINN_HEALING                    = 206699,
    GO_DJINN_FROST                      = 206700,
    GO_DJINN_TORNADO                    = 206701
};

enum areatrig
{
    AREA_TO4W = 5638,
};

Position const conclaveHomePositions[5] =
{
    { -47.953098f, 1053.439941f, 199.488678f, 4.764750f },     // Anshal
    { 189.393997f,  812.568970f, 199.488663f, 3.351030f },     // Nezir
    { -51.463501f,  576.250000f, 199.488678f, 1.518440f },     // Rohash
    { -287.79501f,  816.681030f, 199.572006f, 6.282060f },     // Raid Center Platform
    { -49.645800f,  815.082000f, 191.101000f, 3.124140f },     // Al'Akir
};

template<class AI>
CreatureAI* GetInstanceAI(Creature* creature)
{
    if (InstanceMap* instance = creature->GetMap()->ToInstanceMap())
        if (instance->GetInstanceScript())
            if (instance->GetScriptId() == sObjectMgr->GetScriptId("instance_throne_of_the_four_winds"))
                return new AI(creature);
    return NULL;
}

#endif