/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_DB2STORES_H
#define SKYFIRE_DB2STORES_H

#include "DB2Store.h"
#include "DB2Structure.h"
#include <map>
#include <string>

extern DB2Storage<BattlePetAbilityEffectEntry> sBattlePetAbilityEffectStore;
extern DB2Storage<BattlePetAbilityEntry> sBattlePetAbilityStore;
extern DB2Storage<BattlePetAbilityStateEntry> sBattlePetAbilityStateStore;
extern DB2Storage<BattlePetAbilityTurnEntry> sBattlePetAbilityTurnStore;
extern DB2Storage<BattlePetBreedQualityEntry> sBattlePetBreedQualityStore;
extern DB2Storage<BattlePetBreedStateEntry> sBattlePetBreedStateStore;
extern DB2Storage<BattlePetEffectPropertiesEntry> sBattlePetEffectPropertiesStore;
extern DB2Storage<BattlePetSpeciesEntry> sBattlePetSpeciesStore;
extern DB2Storage<BattlePetSpeciesStateEntry> sBattlePetSpeciesStateStore;
extern DB2Storage<BattlePetSpeciesXAbilityEntry> sBattlePetSpeciesXAbilityStore;
extern DB2Storage<BattlePetStateEntry> sBattlePetStateStore;
extern DB2Storage<BattlePetVisualEntry> sBattlePetVisualStore;
extern DB2Storage<ItemToBattlePetEntry> sItemToBattlePetStore;

extern BattlePetBreedSet sBattlePetBreedSet;
extern BattlePetItemXSpeciesStore sBattlePetItemXSpeciesStore;

typedef std::multimap<uint32, std::pair<uint32, uint8>> BattlePetAbilityTurnByAbilityStore;
extern BattlePetAbilityTurnByAbilityStore sBattlePetAbilityTurnByAbilityStore;

typedef std::map<uint32, BattlePetAbilityEffectEntry const*> BattlePetAbilityEffectByTurnStore;
extern BattlePetAbilityEffectByTurnStore sBattlePetAbilityEffectByTurnStore;

typedef std::map<uint8, float> BattlePetQualityMultiplierStore;
extern BattlePetQualityMultiplierStore sBattlePetQualityMultiplierStore;

typedef std::multimap<uint16, BattlePetSpeciesXAbilityEntry const*> BattlePetSpeciesXAbilityBySpeciesStore;
extern BattlePetSpeciesXAbilityBySpeciesStore sBattlePetSpeciesXAbilityBySpeciesStore;

extern DB2Storage<BroadcastTextEntry> sBroadcastTextStore;
extern DB2Storage<ItemEntry> sItemStore;
extern DB2Storage<ItemCurrencyCostEntry> sItemCurrencyCostStore;
extern DB2Storage<ItemExtendedCostEntry> sItemExtendedCostStore;
extern DB2Storage<ItemSparseEntry> sItemSparseStore;
extern DB2Storage<KeyChainEntry> sKeyChainStore;
//extern DB2Storage<SceneScriptEntry> sSceneScriptStore;
extern DB2Storage<SceneScriptPackageEntry> sSceneScriptPackageStore;
extern DB2Storage<SpellReagentsEntry> sSpellReagentsStore;

void LoadDB2Stores(std::string const& dataPath);

float BattlePetSpeciesMainStat(uint16 stateId, uint16 speciesId);
float BattlePetBreedMainStatModifier(uint16 stateId, uint8 breedId);
float BattlePetQualityMultiplier(uint8 quality);
uint32 BattlePetGetSummonSpell(uint16 speciesId);
bool BattlePetSpeciesHasFlag(uint16 speciesId, uint16 flag);
bool BattlePetHasSpeciesAbility(uint16 speciesId, uint8 abilitySlot, bool alternateAbility, uint8 level);
uint32 BattlePetGetSpeciesAbility(uint16 speciesId, uint8 abilitySlot, uint16 flags, uint8 level);

DB2StorageBase const* GetDB2Storage(uint32 type);

#endif
