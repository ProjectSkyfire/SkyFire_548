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

#ifndef BATTLE_PET_MGR_H
#define BATTLE_PET_MGR_H

#include "BattlePet.h"
#include "Common.h"
#include "TemporarySummon.h"

typedef std::set<BattlePet*> BattlePetSet;

#define BATTLE_PET_MAX_JOURNAL_SPECIES 3   // client modifications required to increase
#define BATTLE_PET_MAX_JOURNAL_PETS    500 // sent to client as 25 bits (theoretical max 33,554,431)
#define BATTLE_PET_MAX_LOADOUT_SLOTS   3

enum BattlePetLoadoutSlots
{
    BATTLE_PET_LOADOUT_SLOT_1                   = 0,
    BATTLE_PET_LOADOUT_SLOT_2                   = 1,
    BATTLE_PET_LOADOUT_SLOT_3                   = 2,
    BATTLE_PET_LOADOUT_SLOT_NONE
};

// custom flags used in 'account_battle_pet_slots' db table
enum BattlePetLoadoutFlags
{
    BATTLE_PET_LOADOUT_SLOT_FLAG_NONE           = 0x0,
    BATTLE_PET_LOADOUT_SLOT_FLAG_SLOT_1         = 0x1,
    BATTLE_PET_LOADOUT_SLOT_FLAG_SLOT_2         = 0x2,
    BATTLE_PET_LOADOUT_SLOT_FLAG_SLOT_3         = 0x4
};

// flags used in 'account_battle_pet' db table
enum BattlePetJournalFlags
{
    BATTLE_PET_JOURNAL_FLAG_NONE                = 0x00,
    BATTLE_PET_JOURNAL_FLAG_FAVORITES           = 0x01,
    BATTLE_PET_JOURNAL_FLAG_COLLECTED           = 0x02, // name dumped from client, use unknown
    BATTLE_PET_JOURNAL_FLAG_NOT_COLLECTED       = 0x04,
    BATTLE_PET_JOURNAL_FLAG_UNKNOWN_1           = 0x08,
    BATTLE_PET_JOURNAL_FLAG_ABILITY_1           = 0x10, // ability flags are set if the second ability for that slot is selected
    BATTLE_PET_JOURNAL_FLAG_ABILITY_2           = 0x20, // ...
    BATTLE_PET_JOURNAL_FLAG_ABILITY_3           = 0x40  // ...
};

enum BattlePetSpells
{
    SPELL_BATTLE_PET_TRAINING_PASSIVE           = 119467,
    SPELL_TRACK_PETS                            = 122026,
    SPELL_REVIVE_BATTLE_PETS                    = 125439
};

class BattlePetMgr
{
public:
    BattlePetMgr(Player* owner)
        : m_owner(owner), m_summon(NULL), m_summonId(0), m_summonLastId(0), m_loadoutFlags(0), m_loadoutSave(false) { }

    ~BattlePetMgr();

    Player* GetOwner() const { return m_owner; }
    BattlePet* GetBattlePet(uint64 id) const;
    uint8 GetBattlePetCount(uint16 speciesId) const;
    uint8 GetLoadoutSlotForBattlePet(uint64 id);

    uint64 GetCurrentSummonId() const { return m_summonId; }
    TempSummon* GetCurrentSummon() const { return m_summon; }
    void SetCurrentSummonId(uint64 summonId) { m_summonId = summonId; }
    void SetCurrentSummon(TempSummon* summon) { m_summon = summon; }

    void UnSummonCurrentBattlePet(bool temporary);
    void ResummonLastBattlePet();

    void LoadFromDb(PreparedQueryResult result);
    void LoadSlotsFromDb(PreparedQueryResult result);
    void SaveToDb(SQLTransaction& trans);
    void SaveSlotsToDb(SQLTransaction& trans);

    void UnlockLoadoutSlot(uint8 slot);
    bool HasLoadoutSlot(uint8 slot) const;
    uint64 GetLoadoutSlot(uint8 slot) const;
    void SetLoadoutSlot(uint8 slot, uint64 id, bool save = false);

    bool HasLoadoutFlag(uint8 flag) const { return (m_loadoutFlags & flag) != 0; };
    uint8 GetLoadoutFlags() const { return m_loadoutFlags; };
    void SetLoadoutFlag(uint8 flag);

    void Create(uint16 speciesId);
    void Delete(BattlePet* battlePet);

    void SendBattlePetDeleted(uint64 id);
    void SendBattlePetJournalLock();
    void SendBattlePetJournal();
    void SendBattlePetSlotUpdate(uint8 slot, bool notification, uint64 id = 0);
    void SendBattlePetUpdate(BattlePet* battlePet, bool notification);

private:
    Player* m_owner;
    BattlePetSet m_battlePetSet;

    TempSummon* m_summon;
    uint64 m_summonId;
    uint64 m_summonLastId;

    bool m_loadoutSave;
    uint8 m_loadoutFlags;
    uint64 m_loadout[BATTLE_PET_MAX_LOADOUT_SLOTS];
};

#endif
