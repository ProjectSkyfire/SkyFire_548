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

#ifndef DEF_ENDTIME_H
#define DEF_ENDTIME_H

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "InstanceScript.h"
#include "GameObject.h"
#include "Player.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellInfo.h"
#include "SpellScript.h"

#define EndTimeScriptName "instance_end_time"
#define MAX_FRAGMENTS_COUNT 16

enum Data
{
    DATA_ECHO_OF_SYLVANAS   = 0,
    DATA_ECHO_OF_BAINE      = 1,
    DATA_ECHO_OF_TYRANDE    = 2,
    DATA_ECHO_OF_JAINA      = 3,
    DATA_MUROZOND           = 4,
    DATA_JAINA_EVENT        = 5,
    DATA_ECHO_1             = 6,
    DATA_ECHO_2             = 7,
    DATA_FIRST_ENCOUNTER    = 8,
    DATA_SECOND_ENCOUNTER   = 9,
    DATA_TYRANDE_EVENT      = 10,
    DATA_NOZDORMU_1         = 11,
    DATA_NOZDORMU_2         = 12,
    DATA_NOZDORMU_3         = 13,
    DATA_NOZDORMU_4         = 14,
    DATA_PLATFORMS          = 15,
    DATA_IMAGE_OF_NOZDORMU  = 16,
    DATA_FRAGMENTS          = 17,
    DATA_HOURGLASS          = 18,
    DATA_NOZDORMU           = 19,
};

enum GameObjectIds
{
    MUROZOND_CACHE      = 209547,
    HOURGLASS_OF_TIME   = 209249,
    GO_ET_TELEPORT      = 209438,

    GO_PLATFORM_1       = 209670,
    GO_PLATFORM_2       = 209693,
    GO_PLATFORM_3       = 209694,
    GO_PLATFORM_4       = 209695,
    GO_FIRE_WALL        = 209990
};

enum CreatureIds
{
    NPC_ECHO_OF_JAINA       = 54445,
    NPC_ECHO_OF_BAINE       = 54431,
    NPC_ECHO_OF_SYLVANAS    = 54123,
    NPC_ECHO_OF_TYRANDE     = 54544,
    NPC_MUROZOND            = 54432,
    NPC_NOZDORMU            = 54751,
    NPC_IMAGE_OF_NOZDORMU   = 54867,
    NPC_BLINK_TARGET        = 54542,
};

enum WorldStatesET
{
    WORLDSTATE_SHOW_FRAGMENTS       = 6046,
    WORLDSTATE_FRAGMENTS_COLLECTED  = 6025,
};

enum QuestIds
{
    QUEST_MUROZOND  = 30096,
};

enum QuestSpells
{
    SPELL_ARCHIVED_JAINA    = 109284,
    SPELL_ARCHIVED_SYLVANAS = 109278,
    SPELL_ARCHIVED_BAINE    = 109288,
    SPELL_ARCHIVED_TYRANDE  = 109292,
};

enum CustomAreas
{
    AREA_RUBY       = 5790,
    AREA_BLUE       = 5793,
    AREA_EMERALD    = 5794,
    AREA_OBSIDIAN   = 5792,
};

enum CustomActions
{
    ACTION_TALK_BAINE       = 1,
    ACTION_TALK_JAINA       = 2,
    ACTION_TALK_SYLVANAS    = 3,
    ACTION_TALK_TYRANDE     = 4,
};

enum Worldstates
{
    WORLDSTATE_SEVERED_TIES = 9888,
};

template<class AI>
inline AI* GetEndTimeAI(Creature* creature)
{
    return GetInstanceAI<AI>(creature, EndTimeScriptName);
}

#endif
