/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_SKILL_EXTRA_ITEMS_H
#define SKYFIRE_SKILL_EXTRA_ITEMS_H

#include "Common.h"

// predef classes used in functions
class Player;
// returns true and sets the appropriate info if the player can create extra items with the given spellId
bool CanCreateExtraItems(Player* player, uint32 spellId, float& additionalChance, uint8& additionalMax);
// function to load the extra item creation info from DB
void LoadSkillExtraItemTable();
#endif
