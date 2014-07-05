/*
 * Copyright (C) 2011-2014 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2014 MaNGOS <http://getmangos.com/>
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

#ifndef TRINITY_DB2STORES_H
#define TRINITY_DB2STORES_H

#include "DB2Store.h"
#include "DB2Structure.h"
#include <string>

extern DB2Storage<BattlePetAbilityEntry> sBattlePetAbilityStore;
extern DB2Storage<BattlePetAbilityStateEntry> sBattlePetAbilityStateStore;
extern DB2Storage<BattlePetBreedStateEntry> sBattlePetBreedStateStore;
extern DB2Storage<BattlePetSpeciesEntry> sBattlePetSpeciesStore;
extern DB2Storage<BattlePetSpeciesStateEntry> sBattlePetSpeciesStateStore;
extern DB2Storage<BattlePetSpeciesXAbilityEntry> sBattlePetSpeciesXAbilityStore;
extern DB2Storage<BattlePetStateEntry> sBattlePetStateStore;
extern DB2Storage<ItemToBattlePetEntry> sItemToBattlePetStore;

extern BattlePetBreedSet sBattlePetBreedSet;
extern BattlePetItemXSpeciesStore sBattlePetItemXSpeciesStore;

extern DB2Storage<BroadcastTextEntry> sBroadcastTextStore;
extern DB2Storage<ItemEntry> sItemStore;
extern DB2Storage<ItemCurrencyCostEntry> sItemCurrencyCostStore;
extern DB2Storage<ItemExtendedCostEntry> sItemExtendedCostStore;
extern DB2Storage<ItemSparseEntry> sItemSparseStore;
extern DB2Storage<KeyChainEntry> sKeyChainStore;
extern DB2Storage<SceneScriptEntry> sSceneScriptStore;
extern DB2Storage<SpellReagentsEntry> sSpellReagentsStore;

void LoadDB2Stores(std::string const& dataPath);

float BattlePetSpeciesMainStat(uint16 stateId, uint16 speciesId);
float BattlePetBreedMainStatModifier(uint16 stateId, uint8 speciesId);
uint32 BattlePetGetSummonSpell(uint16 speciesId);
bool BattlePetSpeciesHasFlag(uint16 speciesId, uint16 flag);

DB2StorageBase const* GetDB2Storage(uint32 type);

#endif
