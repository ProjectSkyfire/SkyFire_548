/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_SKILLDISCOVERY_H
#define SKYFIRE_SKILLDISCOVERY_H

#include "Common.h"

class Player;

void LoadSkillDiscoveryTable();
uint32 GetSkillDiscoverySpell(uint32 skillId, uint32 spellId, Player* player);
bool HasDiscoveredAllSpells(uint32 spellId, Player* player);
uint32 GetExplicitDiscoverySpell(uint32 spellId, Player* player);
#endif
