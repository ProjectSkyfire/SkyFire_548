/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "BattlePetMgr.h"
#include "BattlePetPackets.h"
#include "BattlePetSpawnMgr.h"
#include "ByteBuffer.h"
#include "Common.h"
#include "Creature.h"
#include "DatabaseEnv.h"
#include "DB2Stores.h"
#include "DB2Enums.h"
#include "DBCEnums.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Player.h"
#include "SpellAuraDefines.h"
#include "Util.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"

#include <algorithm>
#include <set>
#include <vector>

namespace
{
    CreatureTemplate const* GetBattlePetCreatureTemplate(BattlePet const* battlePet)
    {
        if (!battlePet)
            return NULL;

        BattlePetSpeciesEntry const* speciesEntry = sBattlePetSpeciesStore.LookupEntry(battlePet->GetSpecies());
        if (!speciesEntry)
            return NULL;

        return sObjectMgr->GetCreatureTemplate(speciesEntry->NpcId);
    }

    bool BattlePetNameMatches(std::string const& left, std::string const& right)
    {
        return !left.empty() && !right.empty() && !stricmp(left.c_str(), right.c_str());
    }

    uint8 BattlePetHealthPercent(uint32 health, uint32 maxHealth)
    {
        if (!maxHealth)
            return 0;

        return uint8(std::min<uint32>(100, health * 100 / maxHealth));
    }

    float BattlePetTrapFailedAttemptBonus(uint32 trapAbility)
    {
        switch (trapAbility)
        {
            case BATTLE_PET_ABILITY_TRAP:
                return 20.0f;
            case BATTLE_PET_ABILITY_STRONG_TRAP:
                return 25.0f;
            case BATTLE_PET_ABILITY_PRISTINE_TRAP:
                return 30.0f;
            case BATTLE_PET_ABILITY_GM_TRAP:
                return 100.0f;
            default:
                return 0.0f;
        }
    }

    float BattlePetTrapCaptureChance(uint32 trapAbility, uint8 failedAttempts)
    {
        if (trapAbility == BATTLE_PET_ABILITY_GM_TRAP)
            return 100.0f;

        float chance = 45.0f;
        float const bonus = BattlePetTrapFailedAttemptBonus(trapAbility);
        for (uint8 i = 0; i < failedAttempts; ++i)
            chance += (100.0f - chance) * bonus / 100.0f;

        return std::min<float>(99.0f, chance);
    }
}

BattlePetMgr::~BattlePetMgr()
{
    for (BattlePetSet::iterator itr = m_battlePetSet.begin(); itr != m_battlePetSet.end(); ++itr)
        delete* itr;

    m_battlePetSet.clear();
}

void BattlePetMgr::LoadFromDb(PreparedQueryResult result)
{
    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint64 id = fields[0].GetUInt64();
        uint16 speciesId = fields[1].GetUInt16();
        std::string nickname = fields[2].GetString();
        uint32 timestamp = fields[3].GetUInt32();
        uint8 level = fields[4].GetUInt8();
        uint16 xp = fields[5].GetUInt16();
        uint16 health = fields[6].GetUInt16();
        uint16 maxHealth = fields[7].GetUInt16();
        uint16 power = fields[8].GetUInt16();
        uint16 speed = fields[9].GetUInt16();
        uint8 quality = fields[10].GetUInt8();
        uint8 breedId = fields[11].GetUInt8();
        uint16 flags = fields[12].GetUInt16();

        if (!sBattlePetSpeciesStore.LookupEntry(speciesId))
        {
            SF_LOG_ERROR("sql.sql", "Species %u defined in `account_battle_pet` for Battle Pet %lu does not exist, skipped.", speciesId, (uint64)id);
            continue;
        }

        if (sBattlePetBreedSet.find(breedId) == sBattlePetBreedSet.end() && breedId != 0)
        {
            SF_LOG_ERROR("sql.sql", "Breed %u defined in `account_battle_pet` for Battle Pet %lu does not exist, skipped.", breedId, (uint64)id);
            continue;
        }

        // highest quality client supports, currently players can not obtain legendary pets on retail
        if (quality > ITEM_QUALITY_LEGENDARY)
        {
            SF_LOG_ERROR("sql.sql", "Quality %u defined in `account_battle_pet` for Battle Pet %lu is invalid, skipped.", quality, (uint64)id);
            continue;
        }

        // client supports up to level 255 (uint8)
        if (level > BATTLE_PET_MAX_LEVEL)
        {
            SF_LOG_ERROR("sql.sql", "Level %u defined in `account_battle_pet` for Battle Pet %lu is invalid, skipped.", level, (uint64)id);
            continue;
        }

        BattlePet* battlePet = new BattlePet(id, speciesId, nickname, timestamp, level, xp,
            health, maxHealth, power, speed, quality, breedId, flags);

        m_battlePetSet.insert(battlePet);
    } while (result->NextRow());

    UpdateBattlePetCollectionAchievements();
}

void BattlePetMgr::SaveToDb(SQLTransaction& trans)
{
    PersistActivePetBattleHealth();
    SaveSlotsToDb(trans);

    if (m_battlePetSet.empty())
        return;

    for (BattlePetSet::iterator itr = m_battlePetSet.begin(); itr != m_battlePetSet.end();)
    {
        BattlePet* battlePet = *itr;

        if (!battlePet)
        {
            itr = m_battlePetSet.erase(itr);
            continue;
        }

        switch (battlePet->GetDbState())
        {
            case BattlePetDbState::BATTLE_PET_DB_STATE_NONE:
                ++itr;
                break;
            case BattlePetDbState::BATTLE_PET_DB_STATE_DELETE:
            {
                PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ACCOUNT_BATTLE_PET);
                stmt->setUInt64(0, battlePet->GetId());
                trans->Append(stmt);

                itr = m_battlePetSet.erase(itr);
                delete battlePet;

                break;
            }
            case BattlePetDbState::BATTLE_PET_DB_STATE_SAVE:
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
                stmt->setUInt8(5, battlePet->GetLevel());
                stmt->setUInt16(6, battlePet->GetXp());
                stmt->setUInt16(7, battlePet->GetCurrentHealth());
                stmt->setUInt16(8, battlePet->GetMaxHealth());
                stmt->setUInt16(9, battlePet->GetPower());
                stmt->setUInt16(10, battlePet->GetSpeed());
                stmt->setUInt8(11, battlePet->GetQuality());
                stmt->setUInt8(12, battlePet->GetBreed());
                stmt->setUInt16(13, battlePet->GetFlags());
                trans->Append(stmt);

                battlePet->SetDbState(BattlePetDbState::BATTLE_PET_DB_STATE_NONE);
                ++itr;
                break;
            }
            default:
                ++itr;
                break;
        }
    }
}

void BattlePetMgr::SaveToDb()
{
    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    SaveToDb(trans);
    CharacterDatabase.CommitTransaction(trans);
}

void BattlePetMgr::LoadSlotsFromDb(PreparedQueryResult result)
{
    if (!result)
    {
        SyncAchievementLoadoutRewards();
        return;
    }

    Field* fields = result->Fetch();

    uint64 slot1 = fields[0].GetUInt64();
    uint64 slot2 = fields[1].GetUInt64();
    uint64 slot3 = fields[2].GetUInt64();
    m_loadoutFlags = fields[3].GetUInt8();

    if (HasLoadoutFlag(BATTLE_PET_LOADOUT_SLOT_FLAG_SLOT_2) && !GetTrapAbility())
        SetLoadoutFlag(BATTLE_PET_LOADOUT_TRAP);

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

    for (std::set<uint8>::const_iterator citr = slotErrors.begin(); citr != slotErrors.end(); ++citr)
    {
        SF_LOG_ERROR("sql.sql", "Battle Pet slot %u in `account_battle_pet_slots` for account %u is invalid!",
            *citr, m_owner->GetSession()->GetAccountId());
    }

    bool hasError = slotErrors.size() > 0;
    if (hasError)
        m_loadoutSave = true;

    SetLoadoutSlot(BATTLE_PET_LOADOUT_SLOT_1, hasError ? 0 : slot1);
    SetLoadoutSlot(BATTLE_PET_LOADOUT_SLOT_2, hasError ? 0 : slot2);
    SetLoadoutSlot(BATTLE_PET_LOADOUT_SLOT_3, hasError ? 0 : slot3);
    SyncAchievementLoadoutRewards();
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
    stmt->setUInt8(4, GetLoadoutFlags());
    trans->Append(stmt);

    m_loadoutSave = false;
}

BattlePet* BattlePetMgr::GetBattlePet(uint64 id) const
{
    for (BattlePetSet::const_iterator itr = m_battlePetSet.begin(); itr != m_battlePetSet.end(); ++itr)
        if ((*itr)->GetId() == id)
            return *itr;

    return NULL;
}

uint32 BattlePetMgr::GetBattlePetCount() const
{
    uint32 counter = 0;

    for (BattlePetSet::const_iterator citr = m_battlePetSet.begin(); citr != m_battlePetSet.end(); ++citr)
    {
        BattlePet* battlePet = *citr;
        if (!battlePet || !battlePet->GetSpecies() || battlePet->GetDbState() == BattlePetDbState::BATTLE_PET_DB_STATE_DELETE)
            continue;

        counter++;
    }

    return counter;
}

uint8 BattlePetMgr::GetBattlePetCount(uint16 speciesId) const
{
    uint8 counter = 0;

    for (BattlePetSet::const_iterator citr = m_battlePetSet.begin(); citr != m_battlePetSet.end(); ++citr)
    {
        BattlePet* battlePet = *citr;
        if (!battlePet || battlePet->GetDbState() == BattlePetDbState::BATTLE_PET_DB_STATE_DELETE)
            continue;

        if (battlePet->GetSpecies() == speciesId)
            counter++;
    }

    return counter;
}

std::string BattlePetMgr::GetBattlePetSpeciesName(BattlePet const* battlePet) const
{
    CreatureTemplate const* creatureTemplate = GetBattlePetCreatureTemplate(battlePet);
    return creatureTemplate ? creatureTemplate->Name : std::string();
}

std::vector<BattlePet*> BattlePetMgr::FindBattlePetNameMatches(std::string const& name) const
{
    std::vector<BattlePet*> matches;
    if (name.empty())
        return matches;

    for (BattlePetSet::const_iterator citr = m_battlePetSet.begin(); citr != m_battlePetSet.end(); ++citr)
    {
        BattlePet* battlePet = *citr;
        if (!battlePet || battlePet->GetDbState() == BattlePetDbState::BATTLE_PET_DB_STATE_DELETE)
            continue;

        if (BattlePetNameMatches(battlePet->GetNickname(), name))
            matches.push_back(battlePet);
    }

    if (!matches.empty())
        return matches;

    for (BattlePetSet::const_iterator citr = m_battlePetSet.begin(); citr != m_battlePetSet.end(); ++citr)
    {
        BattlePet* battlePet = *citr;
        if (!battlePet || battlePet->GetDbState() == BattlePetDbState::BATTLE_PET_DB_STATE_DELETE)
            continue;

        if (BattlePetNameMatches(GetBattlePetSpeciesName(battlePet), name))
            matches.push_back(battlePet);
    }

    return matches;
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

    BattlePet* battlePet = GetBattlePet(m_summonLastId);
    if (!battlePet)
    {
        m_summonLastId = 0;
        return;
    }

    uint32 summonSpell = BattlePetGetSummonSpell(battlePet->GetSpecies());
    if (!summonSpell)
    {
        m_summonLastId = 0;
        return;
    }

    m_summonId = m_summonLastId;
    m_owner->CastSpell(m_owner, summonSpell, true);

    m_summonLastId = 0;
}

uint8 BattlePetMgr::GetLoadoutSlotForBattlePet(uint64 id) const
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
            SetLoadoutFlag(BATTLE_PET_LOADOUT_TRAP);
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

bool BattlePetMgr::HasLoadoutBattlePet() const
{
    for (uint8 i = 0; i < BATTLE_PET_MAX_LOADOUT_SLOTS; i++)
    {
        uint64 petId = GetLoadoutSlot(i);
        if (petId && GetBattlePet(petId))
            return true;
    }

    return false;
}

BattlePet* BattlePetMgr::GetFirstAliveLoadoutBattlePet() const
{
    for (uint8 i = 0; i < BATTLE_PET_MAX_LOADOUT_SLOTS; i++)
    {
        uint64 petId = GetLoadoutSlot(i);
        if (!petId)
            continue;

        BattlePet* battlePet = GetBattlePet(petId);
        if (battlePet && battlePet->GetCurrentHealth())
            return battlePet;
    }

    return NULL;
}

void BattlePetMgr::SetLoadoutFlag(uint8 flag)
{
    if (HasLoadoutFlag(flag))
        return;

    m_loadoutFlags |= flag;
    m_loadoutSave = true;
}

uint32 BattlePetMgr::GetTrapAbility() const
{
    uint32 const trapAbility = BattlePetTrapAbilityForLoadoutFlags(m_loadoutFlags);
    if (trapAbility == BATTLE_PET_ABILITY_GM_TRAP && (!m_owner || !m_owner->IsGameMaster()))
        return BATTLE_PET_ABILITY_TRAP;

    return trapAbility;
}

bool BattlePetMgr::ApplyAchievementLoadoutReward(uint32 achievementId)
{
    uint8 const slot = BattlePetLoadoutSlotForAchievement(achievementId);
    if (slot == BATTLE_PET_LOADOUT_SLOT_NONE)
        return false;

    if (HasLoadoutSlot(slot))
        return false;

    UnlockLoadoutSlot(slot);
    return true;
}

void BattlePetMgr::SyncAchievementLoadoutRewards()
{
    bool updated = false;

    if (m_owner->HasAchieved(BATTLE_PET_ACHIEVEMENT_NEWBIE))
        updated |= ApplyAchievementLoadoutReward(BATTLE_PET_ACHIEVEMENT_NEWBIE);

    if (m_owner->HasAchieved(BATTLE_PET_ACHIEVEMENT_JUST_A_PUP))
        updated |= ApplyAchievementLoadoutReward(BATTLE_PET_ACHIEVEMENT_JUST_A_PUP);

    if (updated)
        SaveToDb();
}

void BattlePetMgr::OnAchievementEarned(uint32 achievementId)
{
    if (ApplyAchievementLoadoutReward(achievementId))
        SaveToDb();
}

BattlePet* BattlePetMgr::Create(uint16 speciesId)
{
    uint8 const level = sWorld->getIntConfig(WorldIntConfigs::CONFIG_BATTLE_PET_INITIAL_LEVEL);
    uint8 const quality = sObjectMgr->BattlePetGetRandomQuality(speciesId);
    return Create(speciesId, level, quality);
}

BattlePet* BattlePetMgr::Create(uint16 speciesId, uint8 level, uint8 quality, bool notification)
{
    return Create(speciesId, level, quality, sObjectMgr->BattlePetGetRandomBreed(speciesId), notification);
}

BattlePet* BattlePetMgr::Create(uint16 speciesId, uint8 level, uint8 quality, uint8 breed, bool notification)
{
    if (!CanCreateBattlePet(speciesId))
        return NULL;

    if (!level)
        level = 1;
    else if (level > BATTLE_PET_MAX_LEVEL)
        level = BATTLE_PET_MAX_LEVEL;

    if (quality > ITEM_QUALITY_LEGENDARY)
        quality = ITEM_QUALITY_NORMAL;

    if (breed && sBattlePetBreedSet.find(breed) == sBattlePetBreedSet.end())
        breed = sObjectMgr->BattlePetGetRandomBreed(speciesId);

    uint64 id = sObjectMgr->BattlePetGetNewId();

    BattlePet* battlePet = new BattlePet(id, speciesId, level, quality, breed);
    m_battlePetSet.insert(battlePet);

    UpdateBattlePetCollectionAchievements(battlePet);

    if (notification)
        SendBattlePetUpdate(battlePet, true);

    return battlePet;
}

bool BattlePetMgr::CanCreateBattlePet(uint16 speciesId) const
{
    BattlePetSpeciesEntry const* speciesEntry = sBattlePetSpeciesStore.LookupEntry(speciesId);
    if (!speciesEntry)
        return false;

    if (!sObjectMgr->GetCreatureTemplate(speciesEntry->NpcId))
        return false;

    if (m_battlePetSet.size() >= BATTLE_PET_MAX_JOURNAL_PETS)
        return false;

    uint8 speciesCount = GetBattlePetCount(speciesId);
    if (BattlePetSpeciesHasFlag(speciesId, BATTLE_PET_FLAG_UNIQUE) && speciesCount >= 1)
        return false;

    if (speciesCount >= BATTLE_PET_MAX_JOURNAL_SPECIES)
        return false;

    return true;
}

void BattlePetMgr::Delete(BattlePet* battlePet)
{
    if (!battlePet)
        return;

    if (battlePet->GetDbState() == BattlePetDbState::BATTLE_PET_DB_STATE_DELETE)
        return;

    // this shouldn't happen since the client doesn't allow releasing of slotted Battle Pets
    uint8 srcSlot = GetLoadoutSlotForBattlePet(battlePet->GetId());
    if (srcSlot != BATTLE_PET_LOADOUT_SLOT_NONE)
    {
        SetLoadoutSlot(srcSlot, 0, true);
        SendBattlePetSlotUpdate(srcSlot, false);
    }

    battlePet->SetDbState(BattlePetDbState::BATTLE_PET_DB_STATE_DELETE);
    UpdateBattlePetCollectionAchievements();

    // alert client of deleted pet
    SendBattlePetDeleted(battlePet->GetId());
}

void BattlePetMgr::HealBattlePets(uint8 percent)
{
    if (!percent)
        return;

    for (BattlePetSet::const_iterator citr = m_battlePetSet.begin(); citr != m_battlePetSet.end(); ++citr)
    {
        BattlePet* battlePet = *citr;
        if (!battlePet || battlePet->GetDbState() == BattlePetDbState::BATTLE_PET_DB_STATE_DELETE)
            continue;

        uint16 const health = BattlePetHealthFromPercent(battlePet->GetMaxHealth(), percent);
        if (health <= battlePet->GetCurrentHealth())
            continue;

        battlePet->SetCurrentHealth(health);
        SendBattlePetUpdate(battlePet, false);
    }
}

void BattlePetMgr::StartWildPetBattle(uint64 enemyGuid, uint64 allyPetId, uint32 allyMaxHealth, uint32 allyHealth,
    uint64 enemyPetId, uint32 enemyMaxHealth, uint32 enemyHealth,
    uint16 enemySpecies, uint8 enemyLevel, uint8 enemyQuality, uint8 enemyBreed, uint8 allyFrontPet)
{
    m_petBattlePvpQueued = false;
    m_activePetBattle.StartWild(enemyGuid, allyPetId, allyMaxHealth, allyHealth,
        enemyPetId, enemyMaxHealth, enemyHealth, enemySpecies, enemyLevel, enemyQuality, enemyBreed, allyFrontPet);
}

void BattlePetMgr::StartPvpPetBattle(uint64 opponentGuid, uint64 allyPetId, uint32 allyMaxHealth, uint32 allyHealth,
    uint64 enemyPetId, uint32 enemyMaxHealth, uint32 enemyHealth,
    uint16 enemySpecies, uint8 enemyLevel, uint8 enemyQuality, uint8 allyFrontPet, uint8 enemyFrontPet,
    BattlePetAchievementSource source)
{
    m_petBattlePvpQueued = false;
    m_activePetBattle.StartPvp(opponentGuid, allyPetId, allyMaxHealth, allyHealth,
        enemyPetId, enemyMaxHealth, enemyHealth, enemySpecies, enemyLevel, enemyQuality,
        allyFrontPet, enemyFrontPet, source);
}

void BattlePetMgr::HideActivePetBattleWorldObject(Creature* creature)
{
    if (!m_owner || !creature || !m_activePetBattle.IsActive())
        return;

    m_activePetBattleWorldObjectGuid = creature->GetGUID();
    m_activePetBattleWorldObjectHidden = true;
    m_owner->UpdateVisibilityOf(creature);
}

void BattlePetMgr::ApplyActivePetBattlePlayerState(float faceX, float faceY)
{
    if (!m_owner)
        return;

    if (m_owner->IsMounted())
        m_owner->RemoveAurasByType(SPELL_AURA_MOUNTED);

    m_owner->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_IMMUNE_TO_NPC);
    m_owner->SetFacingTo(m_owner->GetAngle(faceX, faceY));
    m_owner->SetControlled(true, UNIT_STATE_ROOT);
    m_activePetBattlePlayerStateApplied = true;
}

void BattlePetMgr::ClearActivePetBattle()
{
    PersistActivePetBattleHealth();
    DespawnResolvedWildPetBattleWorldObject();
    ClearActivePetBattleWorldObjectState();
    m_activePetBattle = ActivePetBattle();
    m_petBattlePvpQueued = false;
    ClearActivePetBattlePlayerState();
}

void BattlePetMgr::DespawnResolvedWildPetBattleWorldObject()
{
    if (!m_owner || !m_owner->IsInWorld() || !m_activePetBattle.IsActive()
        || !m_activePetBattle.IsFinished() || m_activePetBattle.IsPvP())
        return;

    if (!m_activePetBattle.Captured && m_activePetBattle.Winner != PET_BATTLE_WINNER_ALLY)
        return;

    uint64 const worldObjectGuid = m_activePetBattle.EnemyGUID;
    if (Creature* creature = ObjectAccessor::GetCreature(*m_owner, worldObjectGuid))
    {
        if (sBattlePetSpawnMgr->ResolveWildBattle(creature))
        {
            if (m_activePetBattleWorldObjectGuid == worldObjectGuid)
            {
                m_activePetBattleWorldObjectGuid = 0;
                m_activePetBattleWorldObjectHidden = false;
            }
        }
        else if (creature->ToTempSummon())
            creature->DespawnOrUnsummon();
        else
        {
            if (creature->IsAlive())
                creature->setDeathState(DeathState::JUST_DIED);

            creature->RemoveCorpse();
            creature->SaveRespawnTime();
        }

        if (m_activePetBattleWorldObjectGuid == worldObjectGuid)
        {
            m_activePetBattleWorldObjectGuid = 0;
            m_activePetBattleWorldObjectHidden = false;
        }
    }
}

void BattlePetMgr::ClearActivePetBattleWorldObjectState()
{
    if (!m_activePetBattleWorldObjectHidden)
        return;

    uint64 const worldObjectGuid = m_activePetBattleWorldObjectGuid;
    m_activePetBattleWorldObjectGuid = 0;
    m_activePetBattleWorldObjectHidden = false;

    if (!m_owner || !m_owner->IsInWorld())
        return;

    if (Creature* creature = ObjectAccessor::GetCreature(*m_owner, worldObjectGuid))
        m_owner->UpdateVisibilityOf(creature);
}

void BattlePetMgr::ClearActivePetBattlePlayerState()
{
    if (!m_activePetBattlePlayerStateApplied)
        return;

    if (m_owner)
    {
        m_owner->SetControlled(false, UNIT_STATE_ROOT);
        m_owner->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_IMMUNE_TO_NPC);
    }

    m_activePetBattlePlayerStateApplied = false;
}

uint8 BattlePetMgr::GetActivePetBattleTrapStatus() const
{
    if (m_activePetBattle.IsPvP())
        return PET_BATTLE_TRAP_STATUS_UNAVAILABLE;

    if (!GetTrapAbility())
        return PET_BATTLE_TRAP_STATUS_UNAVAILABLE;

    uint16 const enemySpecies = m_activePetBattle.EnemySpecies;
    bool const canStoreBattlePet = enemySpecies && CanCreateBattlePet(enemySpecies);
    bool const enemyTameable = enemySpecies
        && !BattlePetSpeciesHasFlag(enemySpecies, BATTLE_PET_FLAG_NOT_TAMEABLE);

    return m_activePetBattle.GetTrapStatus(canStoreBattlePet, enemyTameable);
}

bool BattlePetMgr::SetActivePetBattleAllyPet(uint8 petIndex, uint64 petId, uint32 maxHealth, uint32 health)
{
    return m_activePetBattle.SetAllyPet(petIndex, petId, maxHealth, health);
}

bool BattlePetMgr::SelectActivePetBattleFrontPet(uint8 frontPet)
{
    return m_activePetBattle.SelectAllyFrontPet(frontPet);
}

bool BattlePetMgr::ApplyEnemyBattlePetDamage(uint32 damage, uint32 abilityEffectId,
    Skyfire::BattlePetPackets::BattlePetRoundResult& round)
{
    ActivePetBattleTurn const turn = m_activePetBattle.ApplyEnemyAbilityRound(m_activePetBattle.RoundID, damage);
    if (!turn.Accepted || !turn.HasRoundResult)
        return false;

    round = Skyfire::BattlePetPackets::BuildRoundResultFromTurn(turn, abilityEffectId);
    ApplyActivePetBattleRoundState(round);
    return true;
}

bool BattlePetMgr::ApplyBattlePetAbilityInput(uint32 roundId, uint32 damage, uint32 abilityEffectId,
    uint8 abilitySlot, uint32 abilityId, uint16 abilityCooldown,
    Skyfire::BattlePetPackets::BattlePetRoundResult& round,
    Skyfire::BattlePetPackets::BattlePetFinalRound* finalRound)
{
    ActivePetBattleTurn const turn = m_activePetBattle.ApplyAbilityRound(roundId, damage,
        abilitySlot, abilityId, abilityCooldown);
    if (!turn.Accepted || !turn.HasRoundResult)
        return false;

    round = Skyfire::BattlePetPackets::BuildRoundResultFromTurn(turn, abilityEffectId);
    ApplyActivePetBattleRoundState(round);

    if (finalRound && turn.HasFinalRound)
    {
        PersistActivePetBattleHealth();
        BuildActivePetBattleFinalRound(turn.Abandoned, *finalRound);
    }

    return true;
}

bool BattlePetMgr::ApplyBattlePetAbilityExchangeInput(uint32 roundId,
    uint32 allyDamage, uint32 allyAbilityEffectId,
    uint8 allyAbilitySlot, uint32 allyAbilityId, uint16 allyAbilityCooldown,
    uint32 enemyDamage, uint32 enemyAbilityEffectId,
    uint32 allyIncomingDamageReduction, uint8 allyIncomingDamageReductionRounds,
    uint32 enemyIncomingDamageReduction, uint8 enemyIncomingDamageReductionRounds,
    Skyfire::BattlePetPackets::BattlePetRoundResult& round,
    Skyfire::BattlePetPackets::BattlePetFinalRound* finalRound)
{
    (void)allyDamage;
    (void)allyAbilityEffectId;
    (void)allyAbilityCooldown;

    return ApplyBattlePetAbilityTurnExchangeInput(roundId, allyAbilitySlot, allyAbilityId, 1,
        0, 1, enemyDamage, enemyAbilityEffectId,
        allyIncomingDamageReduction, allyIncomingDamageReductionRounds,
        enemyIncomingDamageReduction, enemyIncomingDamageReductionRounds,
        round, finalRound);
}

bool BattlePetMgr::ApplyBattlePetAbilityTurnExchangeInput(uint32 roundId,
    uint8 allyAbilitySlot, uint32 allyAbilityId, uint8 turnIndex,
    uint16 power, uint8 level,
    uint32 enemyDamage, uint32 enemyAbilityEffectId,
    uint32 allyIncomingDamageReduction, uint8 allyIncomingDamageReductionRounds,
    uint32 enemyIncomingDamageReduction, uint8 enemyIncomingDamageReductionRounds,
    Skyfire::BattlePetPackets::BattlePetRoundResult& round,
    Skyfire::BattlePetPackets::BattlePetFinalRound* finalRound)
{
    if (!allyAbilityId || !turnIndex || allyAbilitySlot >= BATTLE_PET_ABILITY_SLOT_COUNT)
        return false;

    uint8 const turnCount = BattlePetAbilityTurnCount(allyAbilityId);
    uint8 const safeTurnCount = turnCount ? turnCount : 1;
    uint8 const safeTurnIndex = std::min<uint8>(turnIndex, safeTurnCount);

    std::vector<BattlePetAbilityEffectEntry const*> const effects =
        BattlePetAbilityEffectsForTurn(allyAbilityId, safeTurnIndex);

    uint32 totalDamage = 0;
    uint32 killHealAmount = 0;
    uint32 killHealEffectId = 0;
    uint16 auraInstanceSeed = uint16(allyAbilityId ^ (safeTurnIndex << 8));
    bool processedCombatEffect = false;
    std::vector<Skyfire::BattlePetPackets::BattlePetRoundEffect> allyEffects;

    uint8 casterFamilyId = 0;
    if (BattlePet const* allyPet = GetBattlePet(m_activePetBattle.AllyPetID))
        casterFamilyId = BattlePetSpeciesFamilyId(allyPet->GetSpecies());

    for (BattlePetAbilityEffectEntry const* effectEntry : effects)
    {
        if (!effectEntry)
            continue;

        if (BattlePetAbilityEffectDealsDamage(effectEntry->PropertiesId))
        {
            uint32 hitDamage = BattlePetScalePointsFromStats(effectEntry->PropertyValues[0], power, level);
            hitDamage = m_activePetBattle.ScaleAllyOutgoingDamage(hitDamage, casterFamilyId);
            if (!hitDamage)
                continue;

            processedCombatEffect = true;
            totalDamage += hitDamage;
            uint32 const remainingHealth = m_activePetBattle.EnemyHealth > totalDamage
                ? m_activePetBattle.EnemyHealth - totalDamage
                : 0;
            allyEffects.push_back(Skyfire::BattlePetPackets::BuildDamageEffect(
                m_activePetBattle.AllyFrontPet, m_activePetBattle.EnemyFrontPet, int32(remainingHealth),
                effectEntry->Id, uint16(allyEffects.size() + 1)));
            continue;
        }

        // Cleansing Rain-style team heals: one effect per ally slot (EffectIndex 1-3 → slots 0-2).
        if (BattlePetAbilityEffectHeals(effectEntry->PropertiesId))
        {
            uint32 const healAmount = BattlePetScalePointsFromStats(effectEntry->PropertyValues[0], power, level);
            if (!healAmount)
                continue;

            uint8 petIndex = effectEntry->EffectIndex > 0
                ? uint8(effectEntry->EffectIndex - 1)
                : 0;
            if (petIndex >= ActivePetBattle::BATTLE_PET_MAX_ACTIVE_TEAM_PETS)
                continue;
            if (!m_activePetBattle.IsAllyPetAlive(petIndex))
                continue;

            processedCombatEffect = true;
            uint32 const remainingHealth = m_activePetBattle.ApplyAllyHealToPet(petIndex, healAmount);
            allyEffects.push_back(Skyfire::BattlePetPackets::BuildDamageEffect(
                m_activePetBattle.AllyFrontPet, petIndex, int32(remainingHealth),
                effectEntry->Id, uint16(allyEffects.size() + 1)));
            continue;
        }

        // Devour-style: Points with RequiredTargetState=Is_Dead restore health on kill.
        // Emitting these as TriggerAbility visuals corrupted the enemy health bar (0/0 zombies).
        if (BattlePetAbilityEffectIsKillHeal(effectEntry->PropertiesId, effectEntry))
        {
            uint32 const healPoints = BattlePetScalePointsFromStats(effectEntry->PropertyValues[0], power, level);
            if (healPoints && m_activePetBattle.EnemyHealth <= totalDamage)
            {
                processedCombatEffect = true;
                killHealAmount += healPoints;
                killHealEffectId = effectEntry->Id;
            }
            continue;
        }

        if (BattlePetAbilityEffectAppliesWeather(effectEntry->PropertiesId) && effectEntry->AuraAbilityId)
        {
            // Weather is battle-wide. Duration lives in Param[2] (Cleansing Rain = 9).
            // Do not emit AuraApply — same client HP corruption risk as Squawk.
            uint8 const duration = uint8(std::min<uint32>(effectEntry->PropertyValues[2]
                ? effectEntry->PropertyValues[2] : 1, 255));
            m_activePetBattle.ActivateWeather(effectEntry->AuraAbilityId, duration);
            processedCombatEffect = true;
            continue;
        }

        if (BattlePetAbilityEffectAppliesAura(effectEntry->PropertiesId) && effectEntry->AuraAbilityId)
        {
            // Duration: apply-aura-duration uses Param[2]; target auras (Squawk) also store Duration there.
            uint8 const duration = uint8(std::min<uint32>(effectEntry->PropertyValues[2]
                ? effectEntry->PropertyValues[2] : 1, 255));
            bool const untargetable = BattlePetAuraMakesUntargetable(effectEntry->AuraAbilityId);
            bool const toEnemy = BattlePetAbilityEffectAppliesAuraToTarget(effectEntry->PropertiesId);

            processedCombatEffect = true;
            if (toEnemy)
                m_activePetBattle.ActivateEnemyCombatAura(effectEntry->AuraAbilityId, duration, untargetable);
            else
                m_activePetBattle.ActivateAllyCombatAura(effectEntry->AuraAbilityId, duration, untargetable);

            // Only emit an aura packet for untargetable channel visuals (Lift-Off / Dive).
            // Self-buff AuraApply with a full aura target payload desynced the round bitstream
            // (fake pet deaths / ~131k heals on the wrong slot). Emerald Presence mitigation still
            // applies via IncomingDamageReduction; shield VFX stays deferred until the aura
            // target layout is verified against the client.
            if (untargetable && !toEnemy)
            {
                allyEffects.push_back(Skyfire::BattlePetPackets::BuildAuraEffect(
                    m_activePetBattle.AllyFrontPet, m_activePetBattle.AllyFrontPet, effectEntry->Id,
                    effectEntry->AuraAbilityId, duration, auraInstanceSeed++, true,
                    uint16(allyEffects.size() + 1)));
            }
            continue;
        }

        if (BattlePetAbilityEffectRemovesAura(effectEntry->PropertiesId) && effectEntry->AuraAbilityId)
        {
            // Land before the enemy replies this round. Skip a remove packet — SetHealth-style
            // stand-ins were logging "Lift-Off dealt 0 damage" and replaying a junk graphic.
            processedCombatEffect = true;
            m_activePetBattle.ClearAllyCombatAura(effectEntry->AuraAbilityId);
            continue;
        }

        // Unknown effect properties: do not emit speculative visuals (wrong PetBattleEffectType
        // desyncs client HP). Damage/aura handlers above cover the supported combat path.
    }

    if (allyEffects.empty() && !processedCombatEffect)
    {
        uint32 const fallbackEffectId = BattlePetInputEffectForAbilityTurn(allyAbilityId, safeTurnIndex);
        if (fallbackEffectId)
            allyEffects.push_back(Skyfire::BattlePetPackets::BuildDamageEffect(
                m_activePetBattle.AllyFrontPet, m_activePetBattle.EnemyFrontPet,
                int32(m_activePetBattle.EnemyHealth > totalDamage
                    ? m_activePetBattle.EnemyHealth - totalDamage
                    : 0),
                fallbackEffectId));
    }

    // Shield-style flat reduction still uses the cast ability's linked apply effect.
    if (safeTurnIndex == 1)
    {
        m_activePetBattle.ActivateAllyIncomingDamageReduction(
            allyIncomingDamageReduction, allyIncomingDamageReductionRounds);
        m_activePetBattle.ActivateEnemyIncomingDamageReduction(
            enemyIncomingDamageReduction, enemyIncomingDamageReductionRounds);
    }

    uint16 const abilityCooldown = BattlePetAbilityCooldown(allyAbilityId);
    uint16 const remainingLockdown = safeTurnIndex < safeTurnCount
        ? uint16(safeTurnCount - safeTurnIndex) : 0;
    uint8 const nextTurnIndex = remainingLockdown ? uint8(safeTurnIndex + 1) : 0;
    // Do not put the ability on its real cooldown while channeling — the client grays it out and
    // with channel input locks the player can only forfeit.
    uint16 const cooldownToApply = remainingLockdown ? 0 : abilityCooldown;

    uint32 const appliedEnemyDamage = m_activePetBattle.IsAllyUntargetable()
        ? 0
        : m_activePetBattle.ScaleEnemyOutgoingDamage(enemyDamage);

    ActivePetBattleTurn allyTurn;
    ActivePetBattleTurn enemyTurn;
    if (!m_activePetBattle.ApplyAbilityExchange(roundId, totalDamage, appliedEnemyDamage,
        allyAbilitySlot, allyAbilityId, cooldownToApply, remainingLockdown, nextTurnIndex,
        allyTurn, enemyTurn))
        return false;

    if (!allyTurn.Accepted || !allyTurn.HasRoundResult)
        return false;

    if (killHealAmount && allyTurn.TargetDied)
    {
        uint32 const healedHealth = m_activePetBattle.ApplyAllyHeal(killHealAmount);
        allyEffects.push_back(Skyfire::BattlePetPackets::BuildDamageEffect(
            m_activePetBattle.AllyFrontPet, m_activePetBattle.AllyFrontPet, int32(healedHealth),
            killHealEffectId ? killHealEffectId : 0, uint16(allyEffects.size() + 1)));
    }

    round.RoundID = roundId;
    Skyfire::BattlePetPackets::AppendRoundCooldowns(round, allyTurn);
    if (remainingLockdown && !allyTurn.HasFinalRound)
        Skyfire::BattlePetPackets::AppendChannelInputFlags(round, allyAbilitySlot);

    for (Skyfire::BattlePetPackets::BattlePetRoundEffect const& effect : allyEffects)
        round.Effects.push_back(effect);

    if (allyTurn.TargetDied)
    {
        round.DeadPets.push_back(allyTurn.TargetPet);
        Skyfire::BattlePetPackets::MarkRoundResultAsCatchOrKill(round);
    }

    if (enemyTurn.Accepted && enemyTurn.HasRoundResult)
    {
        if (appliedEnemyDamage)
        {
            round.Effects.push_back(Skyfire::BattlePetPackets::BuildDamageEffect(
                enemyTurn.CasterPet, enemyTurn.TargetPet, int32(enemyTurn.RemainingHealth),
                enemyAbilityEffectId ? enemyAbilityEffectId : 0, uint16(round.Effects.size() + 1)));
        }
        else if (enemyAbilityEffectId)
        {
            round.Effects.push_back(Skyfire::BattlePetPackets::BuildMissEffect(
                enemyTurn.CasterPet, enemyTurn.TargetPet, enemyAbilityEffectId,
                int32(enemyTurn.RemainingHealth), uint16(round.Effects.size() + 1)));
        }

        if (enemyTurn.TargetDied)
        {
            round.DeadPets.push_back(enemyTurn.TargetPet);
            Skyfire::BattlePetPackets::MarkRoundResultAsCatchOrKill(round);
        }

        if (enemyTurn.RequiresFrontPet)
            round.InputFlags[0] = Skyfire::BattlePetPackets::BATTLE_PET_ROUND_INPUT_FLAG_SELECT_NEW_FRONT_PET;
    }

    ApplyActivePetBattleRoundState(round);

    if (finalRound && (allyTurn.HasFinalRound || enemyTurn.HasFinalRound))
    {
        PersistActivePetBattleHealth();
        BuildActivePetBattleFinalRound(false, *finalRound);
    }

    return true;
}

bool BattlePetMgr::ApplyBattlePetSwapInput(uint32 roundId, uint8 newFrontPet,
    Skyfire::BattlePetPackets::BattlePetRoundResult& round)
{
    ActivePetBattleTurn const turn = m_activePetBattle.ApplySwapRound(roundId, newFrontPet);
    if (!turn.Accepted || !turn.HasRoundResult)
        return false;

    round = Skyfire::BattlePetPackets::BuildRoundResultFromTurn(turn, 0);
    ApplyActivePetBattleRoundState(round);
    return true;
}

bool BattlePetMgr::ApplyBattlePetForfeitInput(uint32 roundId,
    Skyfire::BattlePetPackets::BattlePetFinalRound& finalRound)
{
    ActivePetBattleTurn const turn = m_activePetBattle.ApplyForfeit(roundId);
    if (!turn.Accepted || !turn.HasFinalRound)
        return false;

    PersistActivePetBattleHealth();
    return BuildActivePetBattleFinalRound(turn.Abandoned, finalRound);
}

bool BattlePetMgr::ApplyBattlePetTrapInput(uint32 roundId, uint32 trapAbilityEffectId,
    uint32 enemyDamage, uint32 enemyAbilityEffectId,
    Skyfire::BattlePetPackets::BattlePetRoundResult& round,
    Skyfire::BattlePetPackets::BattlePetFinalRound& finalRound)
{
    if (!m_activePetBattle.CanAcceptInput(roundId))
        return false;

    if (GetActivePetBattleTrapStatus() != PET_BATTLE_TRAP_STATUS_ACTIVE)
        return false;

    uint32 const trapAbility = GetTrapAbility();
    bool const captured = roll_chance_f(BattlePetTrapCaptureChance(trapAbility, m_activePetBattle.TrapFailedAttempts));

    ActivePetBattleTurn trapTurn;
    ActivePetBattleTurn enemyTurn;
    if (!m_activePetBattle.ApplyTrapRoundWithEnemyResponse(roundId, captured, enemyDamage, trapTurn, enemyTurn))
        return false;

    round.RoundID = trapTurn.RoundID;
    Skyfire::BattlePetPackets::AppendRoundCooldowns(round, trapTurn);
    round.Effects.push_back(Skyfire::BattlePetPackets::BuildCatchEffect(m_activePetBattle.AllyFrontPet,
        m_activePetBattle.EnemyFrontPet, trapAbilityEffectId, trapTurn.Captured, 1));
    if (trapTurn.Captured)
        Skyfire::BattlePetPackets::MarkRoundResultAsCatchOrKill(round);
    else if (enemyTurn.Accepted && enemyTurn.HasRoundResult)
    {
        round.Effects.push_back(Skyfire::BattlePetPackets::BuildDamageEffect(
            enemyTurn.CasterPet, enemyTurn.TargetPet, int32(enemyTurn.RemainingHealth), enemyAbilityEffectId, 2));

        if (enemyTurn.TargetDied)
            round.DeadPets.push_back(enemyTurn.TargetPet);

        if (enemyTurn.RequiresFrontPet)
            round.InputFlags[0] = Skyfire::BattlePetPackets::BATTLE_PET_ROUND_INPUT_FLAG_SELECT_NEW_FRONT_PET;
    }

    ApplyActivePetBattleRoundState(round);

    if (!trapTurn.Captured && !enemyTurn.HasFinalRound)
        return true;

    if (!trapTurn.HasFinalRound && !enemyTurn.HasFinalRound)
        return false;

    if (trapTurn.Captured)
    {
        uint8 const capturedLevel = BattlePetCapturedLevel(m_activePetBattle.EnemyLevel);
        BattlePet* capturedPet = Create(m_activePetBattle.EnemySpecies,
            capturedLevel, m_activePetBattle.EnemyQuality, m_activePetBattle.EnemyBreed);
        if (!capturedPet)
            return false;

        UpdateCapturedBattlePetAchievements(*capturedPet);
    }

    PersistActivePetBattleHealth();
    return BuildActivePetBattleFinalRound(false, finalRound, trapTurn.Captured);
}

void BattlePetMgr::ApplyActivePetBattleRoundState(
    Skyfire::BattlePetPackets::BattlePetRoundResult& round) const
{
    round.TrapStatus[0] = GetActivePetBattleTrapStatus();
    round.TrapStatus[1] = PET_BATTLE_TRAP_STATUS_UNAVAILABLE;
}

void BattlePetMgr::PersistActivePetBattleHealth()
{
    if (!m_activePetBattle.IsActive())
        return;

    for (BattlePetSet::const_iterator itr = m_battlePetSet.begin(); itr != m_battlePetSet.end(); ++itr)
    {
        BattlePet* battlePet = *itr;
        if (!battlePet || battlePet->GetDbState() == BattlePetDbState::BATTLE_PET_DB_STATE_DELETE)
            continue;

        uint32 activeHealth = 0;
        if (!m_activePetBattle.GetAllyPetHealth(battlePet->GetId(), activeHealth))
            continue;

        uint16 const health = activeHealth > battlePet->GetMaxHealth()
            ? battlePet->GetMaxHealth()
            : uint16(activeHealth);
        battlePet->SetCurrentHealth(health);
    }
}

void BattlePetMgr::FinishActivePetBattle(uint8 winner)
{
    if (m_activePetBattle.IsActive())
        m_activePetBattle.Finish(winner);
}

uint16 BattlePetMgr::RewardActivePetBattlePet(BattlePet* battlePet, bool awardExperience)
{
    if (!awardExperience || !battlePet || !m_activePetBattle.EnemyLevel)
        return 0;

    if (battlePet->GetLevel() >= BATTLE_PET_MAX_LEVEL)
        return 0;

    uint16 const reward = BattlePetExperienceReward(battlePet->GetLevel(), m_activePetBattle.EnemyLevel);
    return battlePet->AddExperience(reward);
}

uint32 BattlePetMgr::GetUniqueBattlePetSpeciesCount() const
{
    std::set<uint16> species;

    for (BattlePetSet::const_iterator itr = m_battlePetSet.begin(); itr != m_battlePetSet.end(); ++itr)
    {
        BattlePet const* battlePet = *itr;
        if (!battlePet || battlePet->GetDbState() == BattlePetDbState::BATTLE_PET_DB_STATE_DELETE)
            continue;

        species.insert(battlePet->GetSpecies());
    }

    return uint32(species.size());
}

BattlePetAchievementContext BattlePetMgr::BuildActivePetBattleAchievementContext(
    uint16 speciesId, uint8 quality, bool won, bool captured) const
{
    BattlePetAchievementContext context;
    context.Species = speciesId;
    context.FamilyMask = BattlePetSpeciesFamilyMask(speciesId);
    context.Quality = quality;
    context.Source = m_activePetBattle.GetAchievementSource();
    context.HealthPercent = BattlePetHealthPercent(m_activePetBattle.EnemyHealth, m_activePetBattle.EnemyMaxHealth);
    context.Won = won;
    context.Captured = captured;
    return context;
}

void BattlePetMgr::UpdateBattlePetCollectionAchievements(BattlePet const* battlePet)
{
    if (!m_owner)
        return;

    if (!battlePet)
    {
        for (BattlePetSet::const_iterator itr = m_battlePetSet.begin(); itr != m_battlePetSet.end(); ++itr)
        {
            BattlePet const* existingPet = *itr;
            if (!existingPet || !existingPet->GetSpecies() || existingPet->GetDbState() == BattlePetDbState::BATTLE_PET_DB_STATE_DELETE)
                continue;

            uint16 const speciesId = existingPet->GetSpecies();
            if (uint32 const npcId = BattlePetSpeciesNpcId(speciesId))
                m_owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COLLECT_BATTLE_PET, npcId);

            m_owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COLLECT_BATTLE_PET_SPECIES, speciesId);

            if (uint32 const summonSpell = BattlePetGetSummonSpell(speciesId))
                m_owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL, summonSpell);
        }

        m_owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_OWN_BATTLE_PET_COUNT, GetUniqueBattlePetSpeciesCount());
        return;
    }

    if (battlePet->GetDbState() != BattlePetDbState::BATTLE_PET_DB_STATE_DELETE)
    {
        uint16 const speciesId = battlePet->GetSpecies();
        if (uint32 const npcId = BattlePetSpeciesNpcId(speciesId))
            m_owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COLLECT_BATTLE_PET, npcId);

        m_owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COLLECT_BATTLE_PET_SPECIES, speciesId);

        if (uint32 const summonSpell = BattlePetGetSummonSpell(speciesId))
            m_owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL, summonSpell);
    }

    m_owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_OWN_BATTLE_PET_COUNT, GetUniqueBattlePetSpeciesCount());
}

void BattlePetMgr::UpdateCapturedBattlePetAchievements(BattlePet const& battlePet)
{
    if (!m_owner)
        return;

    BattlePetAchievementContext const context = BuildActivePetBattleAchievementContext(
        battlePet.GetSpecies(), battlePet.GetQuality(), true, true);
    if (!context.IsValid())
        return;

    m_owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_BATTLE_PET,
        context.Species, context.FamilyMask, context.Payload());
    m_owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_BATTLE_PET2,
        1, context.FamilyMask, context.Payload());
}

void BattlePetMgr::UpdateBattlePetLevelAchievement(BattlePet const& battlePet, uint8 initialLevel, uint8 finalLevel)
{
    if (!m_owner || finalLevel <= initialLevel)
        return;

    uint32 const familyMask = BattlePetSpeciesFamilyMask(battlePet.GetSpecies());
    for (uint8 level = initialLevel + 1; level <= finalLevel; ++level)
        m_owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_REACH_BATTLE_PET_LEVEL, level, familyMask);
}

void BattlePetMgr::UpdateFinishedPetBattleAchievements()
{
    if (!m_owner || !m_activePetBattle.IsFinished() || m_activePetBattle.AchievementsApplied)
        return;

    m_activePetBattle.AchievementsApplied = true;

    if (m_activePetBattle.Winner != PET_BATTLE_WINNER_ALLY)
        return;

    BattlePetAchievementContext const context = BuildActivePetBattleAchievementContext(
        m_activePetBattle.EnemySpecies, m_activePetBattle.EnemyQuality, true, false);
    if (!context.IsValid())
        return;

    m_owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_WIN_PET_BATTLE,
        context.Species, context.FamilyMask, context.Payload());
}

bool BattlePetMgr::BuildActivePetBattleFinalRound(bool abandoned,
    Skyfire::BattlePetPackets::BattlePetFinalRound& finalRound, bool captured)
{
    if (!m_activePetBattle.IsActive())
        return false;

    bool const awardExperience = !m_activePetBattle.IsPvP() && !abandoned && !captured && !m_activePetBattle.Captured
        && !m_activePetBattle.RewardsApplied && m_activePetBattle.Winner == PET_BATTLE_WINNER_ALLY;

    std::vector<Skyfire::BattlePetPackets::BattlePetFinalRoundPet> pets;
    for (uint8 i = 0; i < ActivePetBattle::BATTLE_PET_MAX_ACTIVE_TEAM_PETS; ++i)
    {
        if (!m_activePetBattle.HasAllyPet(i))
            continue;

        Skyfire::BattlePetPackets::BattlePetFinalRoundPet allyPet;
        BattlePet* battlePet = GetBattlePet(m_activePetBattle.AllyTeam[i].PetID);
        uint8 const initialLevel = battlePet ? battlePet->GetLevel() : 0;
        bool const petEarnedExperience = awardExperience && i == m_activePetBattle.AllyFrontPet
            && m_activePetBattle.AllyTeam[i].Health != 0;
        uint16 const awardedXp = RewardActivePetBattlePet(
            battlePet, petEarnedExperience);
        uint8 const finalLevel = battlePet ? battlePet->GetLevel() : initialLevel;
        uint16 const finalXp = battlePet ? battlePet->GetXp() : 0;

        if (battlePet)
            UpdateBattlePetLevelAchievement(*battlePet, initialLevel, finalLevel);

        allyPet.Pboid = i;
        allyPet.RemainingHealth = battlePet ? battlePet->GetCurrentHealth() : m_activePetBattle.AllyTeam[i].Health;
        allyPet.InitialLevel = initialLevel;
        allyPet.NewLevel = finalLevel;
        allyPet.Xp = finalXp;
        allyPet.MaxHealth = battlePet ? battlePet->GetMaxHealth() : m_activePetBattle.AllyTeam[i].MaxHealth;
        allyPet.SeenAction = true;
        allyPet.AwardedXP = awardedXp != 0 || finalLevel != initialLevel;
        pets.push_back(allyPet);
    }

    if (awardExperience)
        m_activePetBattle.RewardsApplied = true;

    UpdateFinishedPetBattleAchievements();

    Skyfire::BattlePetPackets::BattlePetFinalRoundPet enemyPet;
    enemyPet.Pboid = m_activePetBattle.EnemyFrontPet;
    enemyPet.RemainingHealth = m_activePetBattle.EnemyHealth;
    enemyPet.MaxHealth = m_activePetBattle.EnemyMaxHealth;
    enemyPet.SeenAction = true;
    enemyPet.Captured = captured || m_activePetBattle.Captured;
    enemyPet.Caged = enemyPet.Captured;
    pets.push_back(enemyPet);

    finalRound = Skyfire::BattlePetPackets::BuildFinalRoundState(
        m_activePetBattle.Winner == PET_BATTLE_WINNER_ALLY, abandoned, pets);
    finalRound.PvPBattle = m_activePetBattle.IsPvP();
    finalRound.Winners[0] = m_activePetBattle.Winner == PET_BATTLE_WINNER_ALLY;
    finalRound.Winners[1] = m_activePetBattle.Winner == PET_BATTLE_WINNER_ENEMY;

    return true;
}

void BattlePetMgr::SendBattlePetDeleted(uint64 id)
{
    ObjectGuid petEntry = id;

    WorldPacket data(SMSG_BATTLE_PET_DELETED, 1 + 8);
    data.WriteGuidMask(petEntry, 0, 4, 7, 6, 1, 5, 2, 3);
    data.WriteGuidBytes(petEntry, 6, 1, 7, 0, 4, 3, 5, 2);

    m_owner->GetSession()->SendPacket(&data);
}

void BattlePetMgr::SendBattlePetJournalLock()
{
    WorldPacket data(SMSG_BATTLE_PET_JOURNAL_LOCK_ACQUIRED, 0);
    m_owner->GetSession()->SendPacket(&data);

    /*WorldPacket data(SMSG_BATTLE_PET_JOURNAL_LOCK_DENINED, 0);
    m_owner->GetSession()->SendPacket(&data);*/
}

void BattlePetMgr::SendPetBattleRequestFailed(uint8 reason)
{
    WorldPacket data = Skyfire::BattlePetPackets::BuildRequestFailedPacket(reason);
    m_owner->GetSession()->SendPacket(&data);
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

    for (BattlePetSet::const_iterator citr = m_battlePetSet.begin(); citr != m_battlePetSet.end(); ++citr)
    {
        BattlePet const* battlePet = *citr;
        if (!battlePet)
            continue;

        if (battlePet->GetDbState() == BattlePetDbState::BATTLE_PET_DB_STATE_DELETE)
            continue;

        CreatureTemplate const* creatureTemplate = GetBattlePetCreatureTemplate(battlePet);
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
    if (!battlePet)
        return;

    ObjectGuid petEntry = battlePet->GetId();

    CreatureTemplate const* creatureTemplate = GetBattlePetCreatureTemplate(battlePet);
    if (!creatureTemplate)
        return;

    WorldPacket data(SMSG_BATTLE_PET_PET_UPDATES, 4 + 1 + 8 + battlePet->GetNickname().size() + 4 + 4 + 4 + 4 + 2 + 4 + 4 + 1 + 2 + 2 + 4 + 2);
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
