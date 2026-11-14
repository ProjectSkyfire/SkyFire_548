/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "DB2Stores.h"
#include "DB2fmt.h"
#include "DBCStores.h"
#include "DBCfmt.h"

// Packet and battle-state tests run without extracted client data. Empty stores
// exercise the production fallback paths rather than loading a live world.
DB2Storage<BattlePetAbilityEffectEntry> sBattlePetAbilityEffectStore(BattlePetAbilityEffectfmt);
DB2Storage<BattlePetAbilityEntry> sBattlePetAbilityStore(BattlePetAbilityfmt);
DB2Storage<BattlePetAbilityStateEntry> sBattlePetAbilityStateStore(BattlePetAbilityStatefmt);
DB2Storage<BattlePetSpeciesEntry> sBattlePetSpeciesStore(BattlePetSpeciesfmt);
DB2Storage<BattlePetVisualEntry> sBattlePetVisualStore(BattlePetVisualfmt);
BattlePetAbilityTurnByAbilityStore sBattlePetAbilityTurnByAbilityStore;
DBCStorage<GtBattlePetXpEntry> sGtBattlePetXpStore(GtBattlePetXpfmt);
