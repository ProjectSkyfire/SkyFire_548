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

#include "BattlePetMgr.h"
#include "ByteBuffer.h"
#include "Common.h"
#include "DatabaseEnv.h"
#include "DB2Enums.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Player.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"

BattlePetMgr::~BattlePetMgr()
{
    for (BattlePetSet::iterator itr = m_battlePetSet.begin(); itr != m_battlePetSet.end(); itr++)
        delete *itr;

    m_battlePetSet.clear();
}

void BattlePetMgr::LoadFromDb(PreparedQueryResult result)
{
    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint64 id            = fields[0].GetUInt64();
        uint16 speciesId     = fields[1].GetUInt16();
        std::string nickname = fields[2].GetString();
        uint32 timestamp     = fields[3].GetUInt32();
        uint8 level          = fields[4].GetUInt8();
        uint16 xp            = fields[5].GetUInt16();
        uint16 health        = fields[6].GetUInt16();
        uint16 maxHealth     = fields[7].GetUInt16();
        uint16 power         = fields[8].GetUInt16();
        uint16 speed         = fields[9].GetUInt16();
        uint8 quality        = fields[10].GetUInt8();
        uint8 breedId        = fields[11].GetUInt8();
        uint16 flags         = fields[12].GetUInt16();

        if (!sBattlePetSpeciesStore.LookupEntry(speciesId))
        {
            TC_LOG_ERROR("sql.sql", "Species %u defined in `account_battle_pet` for Battle Pet %lu does not exist, skipped.", speciesId, (uint64)id);
            continue;
        }

        if (sBattlePetBreedSet.find(breedId) == sBattlePetBreedSet.end() && breedId != 0)
        {
            TC_LOG_ERROR("sql.sql", "Breed %u defined in `account_battle_pet` for Battle Pet %lu does not exist, skipped.", breedId, (uint64)id);
            continue;
        }

        // highest quality client supports, currently players can not obtain legendary pets on retail
        if (quality > ITEM_QUALITY_LEGENDARY)
        {
            TC_LOG_ERROR("sql.sql", "Quality %u defined in `account_battle_pet` for Battle Pet %lu is invalid, skipped.", quality, (uint64)id);
            continue;
        }

        // client supports up to level 255 (uint8)
        if (level > BATTLE_PET_MAX_LEVEL)
        {
            TC_LOG_ERROR("sql.sql", "Level %u defined in `account_battle_pet` for Battle Pet %lu is invalid, skipped.", quality, (uint64)id);
            continue;
        }

        BattlePet* battlePet = new BattlePet(id, speciesId, nickname, timestamp, level, xp,
            health, maxHealth, power, speed, quality, breedId, flags);

        m_battlePetSet.insert(battlePet);
    } 
    while (result->NextRow());
}

void BattlePetMgr::SaveToDb(SQLTransaction& trans)
{
    SaveSlotsToDb(trans);

    if (m_battlePetSet.empty())
        return;

    BattlePetSet::iterator itr = m_battlePetSet.begin();
    while (itr != m_battlePetSet.end())
    {
        BattlePet* battlePet = *itr++;
        switch (battlePet->GetDbState())
        {
            case BATTLE_PET_DB_STATE_DELETE:
            {
                PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ACCOUNT_BATTLE_PET);
                stmt->setUInt64(0, battlePet->GetId());
                trans->Append(stmt);

                m_battlePetSet.erase(itr);
                delete battlePet;

                break;
            }
            case BATTLE_PET_DB_STATE_SAVE:
            {
                PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ACCOUNT_BATTLE_PET);
                stmt->setUInt64(0, battlePet->GetId());
                trans->Append(stmt);

                stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_ACCOUNT_BATTLE_PET);
                stmt->setUInt64(0, battlePet->GetId());
                stmt->setUInt32(1, m_owner->GetSession()->GetAccountId());
                stmt->setUInt16(2, battlePet->GetSpecies());
                stmt->setString(3, battlePet->GetNickname());
                stmt->setUInt32(4, battlePet->GetTimestamp());
                stmt->setUInt8 (5, battlePet->GetLevel());
                stmt->setUInt16(6, battlePet->GetXp());
                stmt->setUInt16(7, battlePet->GetCurrentHealth());
                stmt->setUInt16(8, battlePet->GetMaxHealth());
                stmt->setUInt16(9, battlePet->GetPower());
                stmt->setUInt16(10, battlePet->GetSpeed());
                stmt->setUInt8 (11, battlePet->GetQuality());
                stmt->setUInt8 (12, battlePet->GetBreed());
                stmt->setUInt16(13, battlePet->GetFlags());
                trans->Append(stmt);

                battlePet->SetDbState(BATTLE_PET_DB_STATE_NONE);
                break;
            }
        }
    }
}

void BattlePetMgr::LoadSlotsFromDb(PreparedQueryResult result)
{
    if (!result)
        return;

    Field* fields = result->Fetch();

    uint64 slot1   = fields[0].GetUInt64();
    uint64 slot2   = fields[1].GetUInt64();
    uint64 slot3   = fields[2].GetUInt64();
    m_loadoutFlags = fields[3].GetUInt8();

    // update flag and spell state for new alt characters
    if (m_loadoutFlags != BATTLE_PET_LOADOUT_SLOT_FLAG_NONE && !m_owner->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_BATTLE_PET_ENABLED))
    {
        m_owner->SetFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_BATTLE_PET_ENABLED);

        m_owner->learnSpell(SPELL_BATTLE_PET_TRAINING_PASSIVE, false);
        m_owner->learnSpell(SPELL_TRACK_PETS, false);
        m_owner->learnSpell(SPELL_REVIVE_BATTLE_PETS, false);
    }

    std::set<uint8> slotErrors;

    if ((!HasLoadoutSlot(BATTLE_PET_LOADOUT_SLOT_1) || !GetBattlePet(slot1)) && slot1 != 0)
        slotErrors.insert(BATTLE_PET_LOADOUT_SLOT_1);
    if ((!HasLoadoutSlot(BATTLE_PET_LOADOUT_SLOT_2) || !GetBattlePet(slot2)) && slot2 != 0)
        slotErrors.insert(BATTLE_PET_LOADOUT_SLOT_2);
    if ((!HasLoadoutSlot(BATTLE_PET_LOADOUT_SLOT_3) || !GetBattlePet(slot3)) && slot3 != 0)
        slotErrors.insert(BATTLE_PET_LOADOUT_SLOT_3);

    for (std::set<uint8>::const_iterator citr = slotErrors.begin(); citr != slotErrors.end(); citr++)
    {
        TC_LOG_ERROR("sql.sql", "Battle Pet slot %u in `account_battle_pet_slots` for account %u is invalid!",
            *citr, m_owner->GetSession()->GetAccountId());
    }

    bool hasError = slotErrors.size() > 0;
    if (hasError)
        m_loadoutSave = true;

    SetLoadoutSlot(BATTLE_PET_LOADOUT_SLOT_1, hasError ? 0 : slot1);
    SetLoadoutSlot(BATTLE_PET_LOADOUT_SLOT_2, hasError ? 0 : slot2);
    SetLoadoutSlot(BATTLE_PET_LOADOUT_SLOT_3, hasError ? 0 : slot3);
}

void BattlePetMgr::SaveSlotsToDb(SQLTransaction& trans)
{
    if (!m_loadoutSave)
        return;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ACCOUNT_BATTLE_PET_SLOTS);
    stmt->setUInt32(0, m_owner->GetSession()->GetAccountId());
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_ACCOUNT_BATTLE_PET_SLOTS);
    stmt->setUInt32(0, m_owner->GetSession()->GetAccountId());
    stmt->setUInt64(1, GetLoadoutSlot(BATTLE_PET_LOADOUT_SLOT_1));
    stmt->setUInt64(2, GetLoadoutSlot(BATTLE_PET_LOADOUT_SLOT_2));
    stmt->setUInt64(3, GetLoadoutSlot(BATTLE_PET_LOADOUT_SLOT_3));
    stmt->setUInt8 (4, GetLoadoutFlags());
    trans->Append(stmt);

    m_loadoutSave = false;
}

BattlePet* BattlePetMgr::GetBattlePet(uint64 id) const
{
    for (BattlePetSet::iterator itr = m_battlePetSet.begin(); itr != m_battlePetSet.end(); itr++)
        if ((*itr)->GetId() == id)
            return *itr;

    return NULL;
}

uint8 BattlePetMgr::GetBattlePetCount(uint16 speciesId) const
{
    uint8 counter = 0;

    for (BattlePetSet::const_iterator citr = m_battlePetSet.begin(); citr != m_battlePetSet.end(); citr++)
        if ((*citr)->GetSpecies() == speciesId)
            counter++;

    return counter;
}

void BattlePetMgr::UnSummonCurrentBattlePet(bool temporary)
{
    if (!m_summon || !m_summonId)
        return;

    m_summonLastId = temporary ? m_summonId : 0;
    m_summonId = 0;

    m_summon->UnSummon();
    m_summon = NULL;
}

void BattlePetMgr::ResummonLastBattlePet()
{
    if (!m_summonLastId)
        return;

    m_summonId = m_summonLastId;
    m_owner->CastSpell(m_owner, BattlePetGetSummonSpell(GetBattlePet(m_summonId)->GetSpecies()), true);

    m_summonLastId = 0;
}

uint8 BattlePetMgr::GetLoadoutSlotForBattlePet(uint64 id)
{
    for (uint8 i = 0; i < BATTLE_PET_MAX_LOADOUT_SLOTS; i++)
        if (GetLoadoutSlot(i) == id)
            return i;

    return BATTLE_PET_LOADOUT_SLOT_NONE;
}

void BattlePetMgr::UnlockLoadoutSlot(uint8 slot)
{
    if (HasLoadoutSlot(slot))
        return;

    if (slot >= BATTLE_PET_MAX_LOADOUT_SLOTS)
        return;

    switch (slot)
    {
        case BATTLE_PET_LOADOUT_SLOT_1:
            SetLoadoutFlag(BATTLE_PET_LOADOUT_SLOT_FLAG_SLOT_1);
            break;
        case BATTLE_PET_LOADOUT_SLOT_2:
            SetLoadoutFlag(BATTLE_PET_LOADOUT_SLOT_FLAG_SLOT_2);
            break;
        case BATTLE_PET_LOADOUT_SLOT_3:
            SetLoadoutFlag(BATTLE_PET_LOADOUT_SLOT_FLAG_SLOT_3);
            break;
    }

    SetLoadoutSlot(slot, 0);

    // alert client of new Battle Pet loadout slot
    SendBattlePetSlotUpdate(slot, true);
}

void BattlePetMgr::SetLoadoutSlot(uint8 slot, uint64 id, bool save)
{
    if (!HasLoadoutSlot(slot))
        return;

    m_loadout[slot] = id;

    if (save)
        m_loadoutSave = true;
}

uint64 BattlePetMgr::GetLoadoutSlot(uint8 slot) const
{
    if (!HasLoadoutSlot(slot))
        return 0;

    return m_loadout[slot];
}

bool BattlePetMgr::HasLoadoutSlot(uint8 slot) const
{
    if (!m_loadoutFlags || slot >= BATTLE_PET_MAX_LOADOUT_SLOTS)
        return false;

    switch (slot)
    {
        case BATTLE_PET_LOADOUT_SLOT_1:
            return HasLoadoutFlag(BATTLE_PET_LOADOUT_SLOT_FLAG_SLOT_1);
        case BATTLE_PET_LOADOUT_SLOT_2:
            return HasLoadoutFlag(BATTLE_PET_LOADOUT_SLOT_FLAG_SLOT_2);
        case BATTLE_PET_LOADOUT_SLOT_3:
            return HasLoadoutFlag(BATTLE_PET_LOADOUT_SLOT_FLAG_SLOT_3);
    }

    return false;
}

void BattlePetMgr::SetLoadoutFlag(uint8 flag)
{
    if (HasLoadoutFlag(flag))
        return;

    m_loadoutFlags |= flag;
    m_loadoutSave = true;
}

void BattlePetMgr::Create(uint16 speciesId)
{
    if (!sBattlePetSpeciesStore.LookupEntry(speciesId))
        return;

    if (m_battlePetSet.size() > BATTLE_PET_MAX_JOURNAL_PETS)
        return;

    uint8 speciesCount = GetBattlePetCount(speciesId);
    if (BattlePetSpeciesHasFlag(speciesId, BATTLE_PET_FLAG_UNIQUE) && speciesCount >= 1)
        return;

    if (speciesCount >= BATTLE_PET_MAX_JOURNAL_SPECIES)
        return;

    uint64 id     = sObjectMgr->BattlePetGetNewId();
    uint8 breed   = sObjectMgr->BattlePetGetRandomBreed(speciesId);
    uint8 quality = sObjectMgr->BattlePetGetRandomQuality(speciesId);
    uint8 level   = sWorld->getIntConfig(CONFIG_BATTLE_PET_INITIAL_LEVEL);

    BattlePet* battlePet = new BattlePet(id, speciesId, level, quality, breed);
    m_battlePetSet.insert(battlePet);

    // alert the client of a new Battle Pet
    SendBattlePetUpdate(battlePet, true);
}

void BattlePetMgr::Delete(BattlePet* battlePet)
{
    if (!battlePet)
        return;

    if (battlePet->GetDbState() == BATTLE_PET_DB_STATE_DELETE)
        return;

    // this shouldn't happen since the client doesn't allow releasing of slotted Battle Pets
    uint8 srcSlot = GetLoadoutSlotForBattlePet(battlePet->GetId());
    if (srcSlot != BATTLE_PET_LOADOUT_SLOT_NONE)
    {
        SetLoadoutSlot(srcSlot, 0, true);
        SendBattlePetSlotUpdate(srcSlot, false);
    }

    battlePet->SetDbState(BATTLE_PET_DB_STATE_DELETE);

    // alert client of deleted pet
    SendBattlePetDeleted(battlePet->GetId());
}

void BattlePetMgr::SendBattlePetDeleted(uint64 id)
{
    ObjectGuid petEntry = id;

    WorldPacket data(SMSG_BATTLE_PET_DELETED, 1 + 8);
    data.WriteBit(petEntry[0]);
    data.WriteBit(petEntry[4]);
    data.WriteBit(petEntry[7]);
    data.WriteBit(petEntry[6]);
    data.WriteBit(petEntry[1]);
    data.WriteBit(petEntry[5]);
    data.WriteBit(petEntry[2]);
    data.WriteBit(petEntry[3]);

    data.WriteByteSeq(petEntry[6]);
    data.WriteByteSeq(petEntry[1]);
    data.WriteByteSeq(petEntry[7]);
    data.WriteByteSeq(petEntry[0]);
    data.WriteByteSeq(petEntry[4]);
    data.WriteByteSeq(petEntry[3]);
    data.WriteByteSeq(petEntry[5]);
    data.WriteByteSeq(petEntry[2]);

    m_owner->GetSession()->SendPacket(&data);
}

void BattlePetMgr::SendBattlePetJournalLock()
{
    WorldPacket data(SMSG_BATTLE_PET_JOURNAL_LOCK_ACQUIRED, 0);
    m_owner->GetSession()->SendPacket(&data);

    /*WorldPacket data(SMSG_BATTLE_PET_JOURNAL_LOCK_DENINED, 0);
    m_owner->GetSession()->SendPacket(&data);*/
}

void BattlePetMgr::SendBattlePetJournal()
{
    uint32 petCount = 0;
    ByteBuffer journalData, slotData;

    // packet size is over estimated
    WorldPacket data(SMSG_BATTLE_PET_JOURNAL, 8 + m_battlePetSet.size() * (2 + 1 + 8 + 1 + 4 + 2 + 4 + 2 + 4 + 2 + 4 + 4 + 4 + 16 + 4 + 2)
        + BATTLE_PET_MAX_LOADOUT_SLOTS * (1 + 1 + 8 + 1));

    size_t writePos = data.bitwpos();
    data.WriteBits(petCount, 19);               // placeholder
    
    for (BattlePetSet::const_iterator citr = m_battlePetSet.begin(); citr != m_battlePetSet.end(); citr++)
    {
        BattlePet const* battlePet = *citr;
        if (battlePet->GetDbState() == BATTLE_PET_DB_STATE_DELETE)
            continue;

        CreatureTemplate const* creatureTemplate = sObjectMgr->GetCreatureTemplate(sBattlePetSpeciesStore.LookupEntry(battlePet->GetSpecies())->NpcId);
        if (!creatureTemplate)
            continue;

        ObjectGuid petEntry = battlePet->GetId();

        data.WriteBit(!battlePet->GetFlags());
        data.WriteBit(petEntry[3]);
        data.WriteBit(petEntry[7]);
        data.WriteBits(battlePet->GetNickname().size(), 7);
        data.WriteBit(0);                       // BATTLE_PET_FLAG_NOT_ACCOUNT_BOUND
        data.WriteBit(petEntry[0]);
        data.WriteBit(petEntry[2]);
        data.WriteBit(petEntry[6]);
        data.WriteBit(0);                       // unknown
        data.WriteBit(petEntry[1]);
        data.WriteBit(petEntry[5]);
        data.WriteBit(!battlePet->GetBreed());
        data.WriteBit(petEntry[4]);
        data.WriteBit(!battlePet->GetQuality());

        if (battlePet->GetQuality())
            journalData << uint8(battlePet->GetQuality());

        journalData << uint32(battlePet->GetPower());
        journalData.WriteByteSeq(petEntry[7]);
        journalData << uint16(battlePet->GetLevel());
        journalData << uint32(battlePet->GetCurrentHealth());

        if (battlePet->GetBreed())
            journalData << uint16(battlePet->GetBreed());

        journalData << uint32(battlePet->GetSpecies());
        journalData.WriteByteSeq(petEntry[2]);

        if (battlePet->GetFlags())
            journalData << uint16(battlePet->GetFlags());

        journalData << uint32(creatureTemplate->Entry);
        journalData << uint32(creatureTemplate->Modelid1);
        journalData << uint32(battlePet->GetSpeed());
        journalData.WriteString(battlePet->GetNickname());
        journalData.WriteByteSeq(petEntry[6]);
        journalData.WriteByteSeq(petEntry[5]);
        journalData << uint32(battlePet->GetMaxHealth());
        journalData.WriteByteSeq(petEntry[4]);
        journalData << uint16(battlePet->GetXp());
        journalData.WriteByteSeq(petEntry[0]);
        journalData.WriteByteSeq(petEntry[1]);
        journalData.WriteByteSeq(petEntry[3]);

        petCount++;
    }

    data.WriteBit(1);                           // slots enabled
    data.WriteBits(BATTLE_PET_MAX_LOADOUT_SLOTS, 25);

    for (uint8 i = 0; i < BATTLE_PET_MAX_LOADOUT_SLOTS; i++)
    {
        ObjectGuid loadoutEntry = GetLoadoutSlot(i);

        data.WriteBit(!HasLoadoutSlot(i));
        data.WriteBit(1);                       // unknown
        data.WriteBit(0);                       // has slot number
        data.WriteBit(0);                       // fake
        data.WriteBit(loadoutEntry[0]);
        data.WriteBit(loadoutEntry[1]);
        data.WriteBit(loadoutEntry[7]);
        data.WriteBit(loadoutEntry[6]);
        data.WriteBit(loadoutEntry[4]);
        data.WriteBit(loadoutEntry[2]);
        data.WriteBit(loadoutEntry[5]);
        data.WriteBit(loadoutEntry[3]);

        slotData.WriteByteSeq(loadoutEntry[5]);
        slotData.WriteByteSeq(loadoutEntry[1]);
        slotData.WriteByteSeq(loadoutEntry[7]);
        slotData.WriteByteSeq(loadoutEntry[2]);
        slotData.WriteByteSeq(loadoutEntry[3]);
        slotData.WriteByteSeq(loadoutEntry[0]);
        slotData.WriteByteSeq(loadoutEntry[4]);
        slotData.WriteByteSeq(loadoutEntry[6]);
        slotData << uint8(i);
    }

    data.FlushBits();
    data.PutBits(writePos, petCount, 19);

    data.append(slotData);
    data.append(journalData);
    data << uint16(0);                          // unknown

    m_owner->GetSession()->SendPacket(&data);
}

void BattlePetMgr::SendBattlePetSlotUpdate(uint8 slot, bool notification, uint64 id)
{
    ObjectGuid petEntry = id;

    WorldPacket data(SMSG_BATTLE_PET_SLOT_UPDATE, 5 + 1);
    data.WriteBits(1, 25);
    data.WriteBit(0);                   // unknown

    {
        data.WriteBit(0);               // has slot number
        data.WriteBit(1);               // unknown
        data.WriteBit(0);               // unknown
        data.WriteBit(0);               // fake
        data.WriteBit(petEntry[4]);
        data.WriteBit(petEntry[5]);
        data.WriteBit(petEntry[2]);
        data.WriteBit(petEntry[1]);
        data.WriteBit(petEntry[0]);
        data.WriteBit(petEntry[3]);
        data.WriteBit(petEntry[7]);
        data.WriteBit(petEntry[6]);
    }

    data.WriteBit(notification);
    data.FlushBits();

    {
        data.WriteByteSeq(petEntry[0]);
        data.WriteByteSeq(petEntry[3]);
        data.WriteByteSeq(petEntry[2]);
        data.WriteByteSeq(petEntry[1]);
        data.WriteByteSeq(petEntry[6]);
        data.WriteByteSeq(petEntry[4]);
        data.WriteByteSeq(petEntry[5]);
        data.WriteByteSeq(petEntry[7]);
        data << uint8(slot);
    }

    m_owner->GetSession()->SendPacket(&data);
}

void BattlePetMgr::SendBattlePetUpdate(BattlePet* battlePet, bool notification)
{
    ObjectGuid petEntry = battlePet->GetId();

    CreatureTemplate const* creatureTemplate = sObjectMgr->GetCreatureTemplate(sBattlePetSpeciesStore.LookupEntry(battlePet->GetSpecies())->NpcId);
    if (!creatureTemplate)
        return;

    WorldPacket data(SMSG_BATTLE_PET_UPDATE, 4 + 1 + 8 + battlePet->GetNickname().size() + 4 + 4 + 4 + 4 + 2 + 4 + 4 + 1 + 2 + 2 + 4 + 2);
    data.WriteBits(1, 19);
    data.WriteBit(petEntry[4]);
    data.WriteBit(petEntry[1]);
    data.WriteBit(petEntry[7]);
    data.WriteBit(!battlePet->GetQuality());
    data.WriteBit(!battlePet->GetBreed());
    data.WriteBit(petEntry[5]);
    data.WriteBit(0);                           // unknown
    data.WriteBit(petEntry[2]);
    data.WriteBit(!battlePet->GetFlags());
    data.WriteBit(0);                           // BATTLE_PET_FLAG_NOT_ACCOUNT_BOUND
    data.WriteBit(petEntry[6]);
    data.WriteBit(petEntry[3]);
    data.WriteBits(battlePet->GetNickname().size(), 7);
    data.WriteBit(petEntry[0]);
    data.WriteBit(notification);
    data.FlushBits();

    data.WriteByteSeq(petEntry[1]);
    data.WriteString(battlePet->GetNickname());
    data << uint32(battlePet->GetCurrentHealth());
    data << uint32(battlePet->GetSpecies());
    data.WriteByteSeq(petEntry[0]);
    data << uint32(battlePet->GetPower());
    data << uint32(battlePet->GetSpeed());

    if (battlePet->GetBreed())
        data << uint16(battlePet->GetBreed());

    data.WriteByteSeq(petEntry[4]);
    data << uint32(creatureTemplate->Entry);
    data << uint32(battlePet->GetMaxHealth());
    data.WriteByteSeq(petEntry[6]);

    if (battlePet->GetQuality())
        data << uint8(battlePet->GetQuality());

    data.WriteByteSeq(petEntry[2]);
    data.WriteByteSeq(petEntry[3]);
    data << uint16(battlePet->GetXp());
    data.WriteByteSeq(petEntry[7]);

    if (battlePet->GetFlags())
        data << uint16(battlePet->GetFlags());

    data.WriteByteSeq(petEntry[5]);
    data << uint32(creatureTemplate->Modelid1);
    data << uint16(battlePet->GetLevel());

    m_owner->GetSession()->SendPacket(&data);
}
