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

#include "BattlePet.h"
#include "Common.h"

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

    m_maxHealth = roundf((baseHealth * 5.0f * m_level * qualityMod) + 100.0f);

    if (currentHealth)
        m_curHealth = m_maxHealth;

    m_power = roundf(basePower * m_level * qualityMod);
    m_speed = roundf(baseSpeed * m_level * qualityMod);

    m_saveRequired = true;
}

void BattlePet::SetNickname(std::string nickname)
{
    m_nickname = nickname;
    m_saveRequired = true;
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
    m_saveRequired = true;
}

void BattlePet::UnSetFlag(uint16 flag)
{
    if (!HasFlag(flag))
        return;

    m_flags &= ~flag;
    m_saveRequired = true;
}
