/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "BattlePet.h"
#include "Common.h"
#include "DB2Stores.h"

void BattlePet::CalculateStats(bool currentHealth)
{
    float basePower = BattlePetSpeciesMainStat(BATTLE_PET_STATE_STAT_POWER, m_species) +
        BattlePetBreedMainStatModifier(BATTLE_PET_STATE_STAT_POWER, m_breed);
    float baseHealth = BattlePetSpeciesMainStat(BATTLE_PET_STATE_STAT_STAMINA, m_species) +
        BattlePetBreedMainStatModifier(BATTLE_PET_STATE_STAT_STAMINA, m_breed);
    float baseSpeed = BattlePetSpeciesMainStat(BATTLE_PET_STATE_STAT_SPEED, m_species) +
        BattlePetBreedMainStatModifier(BATTLE_PET_STATE_STAT_SPEED, m_breed);

    float qualityMod = 1.0f; // ITEM_QUALITY_POOR
    switch (m_quality)
    {
        case ITEM_QUALITY_NORMAL:
            qualityMod = 1.1f;
            break;
        case ITEM_QUALITY_UNCOMMON:
            qualityMod = 1.2f;
            break;
        case ITEM_QUALITY_RARE:
            qualityMod = 1.3f;
            break;
        case ITEM_QUALITY_EPIC:
            qualityMod = 1.4f;
            break;
        case ITEM_QUALITY_LEGENDARY:
            qualityMod = 1.5f;
            break;
    }

    // No round in older cpp, just for compatibility
    m_maxHealth = floor(((baseHealth * 5.0f * m_level * qualityMod) + 100.0f) + 0.5f);

    if (currentHealth)
        m_curHealth = m_maxHealth;

    m_power = floor((basePower * m_level * qualityMod) + 0.5f);
    m_speed = floor((baseSpeed * m_level * qualityMod) + 0.5f);

    m_dbState = BattlePetDbState::BATTLE_PET_DB_STATE_SAVE;
}

void BattlePet::SetNickname(std::string const& nickname)
{
    m_nickname = nickname;
    m_dbState = BattlePetDbState::BATTLE_PET_DB_STATE_SAVE;
}

void BattlePet::SetQuality(ItemQualities quality)
{
    m_quality = quality;
    CalculateStats(false);
}

void BattlePet::SetFlag(uint16 flag)
{
    if (HasFlag(flag))
        return;

    m_flags |= flag;
    m_dbState = BattlePetDbState::BATTLE_PET_DB_STATE_SAVE;
}

void BattlePet::UnSetFlag(uint16 flag)
{
    if (!HasFlag(flag))
        return;

    m_flags &= ~flag;
    m_dbState = BattlePetDbState::BATTLE_PET_DB_STATE_SAVE;
}

//DEAD CODE
void BattlePet::SetLevel(uint8 level)
{
    m_level = level;
    CalculateStats(true);
}
