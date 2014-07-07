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

#ifndef BATTLE_PET_H
#define BATTLE_PET_H

#include "Common.h"
#include "SharedDefines.h"

#define BATTLE_PET_MAX_LEVEL       25
#define BATTLE_PET_MAX_NAME_LENGTH 16

enum BattlePetStates
{
    BATTLE_PET_STATE_STAT_POWER         = 18,
    BATTLE_PET_STATE_STAT_STAMINA       = 19,
    BATTLE_PET_STATE_STAT_SPEED         = 20,
    BATTLE_PET_STATE_STAT_GENDER        = 78  // currently not used by Blizzard
};

enum BattlePetDbState
{
    BATTLE_PET_DB_STATE_NONE            = 0,
    BATTLE_PET_DB_STATE_DELETE          = 1,
    BATTLE_PET_DB_STATE_SAVE            = 2
};

class BattlePet
{
public:
    BattlePet(uint64 id, uint16 speciesId, std::string nickname, uint32 timestamp, uint8 level, uint16 xp, uint16 curHealth,
        uint16 maxHealth, uint16 power, uint16 speed, uint8 quality, uint8 breedId, uint16 flags)
        : m_id(id), m_species(speciesId), m_nickname(nickname), m_timestamp(timestamp), m_level(level), m_xp(xp), m_curHealth(curHealth), m_maxHealth(maxHealth),
        m_power(power), m_speed(speed), m_quality(quality), m_breed(breedId), m_flags(flags), m_dbState(BATTLE_PET_DB_STATE_NONE) { }

    BattlePet(uint64 id, uint16 speciesId, uint8 level, uint8 quality, uint8 breedId)
        : m_id(id), m_species(speciesId), m_breed(breedId), m_nickname(""), m_timestamp(0),m_level(level), m_xp(0), m_curHealth(0),
        m_maxHealth(0), m_power(0), m_speed(0), m_quality(quality), m_flags(0), m_dbState(BATTLE_PET_DB_STATE_NONE)
    {
        CalculateStats(true);
    }

    uint64 GetId() const { return m_id; }
    uint16 GetSpecies() const { return m_species; }
    std::string GetNickname() const { return m_nickname; }
    uint32 GetTimestamp() const { return m_timestamp; }
    uint8 GetLevel() const { return m_level; }
    uint16 GetXp() const { return m_xp; }
    uint16 GetCurrentHealth() const { return m_curHealth; }
    uint16 GetMaxHealth() const { return m_maxHealth; }
    uint16 GetPower() const { return m_power; }
    uint16 GetSpeed() const { return m_speed; }
    uint8 GetQuality() const { return m_quality; }
    uint8 GetBreed() const { return m_breed; }

    void SetNickname(std::string nickname);
    void SetTimestamp(uint32 timestamp) { m_timestamp = timestamp; }
    void SetQuality(ItemQualities quality);

    BattlePetDbState GetDbState() const { return m_dbState; }
    void SetDbState(BattlePetDbState state) { m_dbState = state; }

    uint8 GetFlags() const { return m_flags; }
    bool HasFlag(uint16 flag) const { return (m_flags & flag) != 0; }
    void SetFlag(uint16 flag);
    void UnSetFlag(uint16 flag);

    void CalculateStats(bool currentHealth = false);

private:
    uint64 m_id;
    uint16 m_species;
    std::string m_nickname;
    uint32 m_timestamp;
    uint8 m_level;
    uint16 m_xp;
    uint16 m_curHealth;
    uint16 m_maxHealth;
    uint16 m_power;
    uint16 m_speed;
    uint8 m_quality;
    uint8 m_breed;
    uint16 m_flags;

    BattlePetDbState m_dbState;
};

#endif
