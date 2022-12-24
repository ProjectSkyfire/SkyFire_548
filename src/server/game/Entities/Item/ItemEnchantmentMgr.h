/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SF_ITEM_ENCHANTMENT_MGR_H
#define SF_ITEM_ENCHANTMENT_MGR_H

#include "Common.h"

void LoadRandomEnchantmentsTable();
uint32 GetItemEnchantMod(int32 entry);
uint32 GenerateEnchSuffixFactor(uint32 item_id);
#endif
