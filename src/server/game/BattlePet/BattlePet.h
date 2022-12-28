/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef BATTLE_PET_H
#define BATTLE_PET_H

#include "Common.h"
#include "SharedDefines.h"

#define BATTLE_PET_MAX_LEVEL       25
#define BATTLE_PET_MAX_NAME_LENGTH 16

/*
enum BattlePetTypeSuffix
{
    BATTLE_PET_TYPE_HUMANOID = 1,
    BATTLE_PET_TYPE_DRAGON,
    BATTLE_PET_TYPE_FLYING,
    BATTLE_PET_TYPE_UNDEAD,
    BATTLE_PET_TYPE_CRITTER,
    BATTLE_PET_TYPE_MAGICAL,
    BATTLE_PET_TYPE_ELEMENTAL,
    BATTLE_PET_TYPE_BEAST,
    BATTLE_PET_TYPE_AQUATIC,
    BATTLE_PET_TYPE_MECHANICAL
}

enum BattlePetTypePassives
{
    BATTLE_PET_TYPE_PASSIVE_CRITTER_ELUSIVE          = 236,
    BATTLE_PET_TYPE_PASSIVE_BEAST_ENRAGE             = 237,
    BATTLE_PET_TYPE_PASSIVE_HUMANOID_RECOVERY        = 238,
    BATTLE_PET_TYPE_PASSIVE_FLYING_SWIFTNESS         = 239,
    BATTLE_PET_TYPE_PASSIVE_AQUATIC_PURITY           = 240,
    BATTLE_PET_TYPE_PASSIVE_ELEMENTAL_WEATHER_IMMUNE = 241,
    BATTLE_PET_TYPE_PASSIVE_UNDEAD_DAMNED            = 242,
    BATTLE_PET_TYPE_PASSIVE_MAGICAL_SPELLSHIELD      = 243,
    BATTLE_PET_TYPE_PASSIVE_MECHANICAL_FAILSAFE      = 244,
    BATTLE_PET_TYPE_PASSIVE_DRAGON_EXECUTE           = 245,
}

enum BattlePetEvent
{
    BATTLE_PET_EVENT_ON_APPLY         = 0,
    BATTLE_PET_EVENT_ON_DAMAGE_TAKEN  = 1,
    BATTLE_PET_EVENT_ON_DAMAGE_DEALT  = 2,
    BATTLE_PET_EVENT_ON_HEAL_TAKEN    = 3,
    BATTLE_PET_EVENT_ON_HEAL_DEALT    = 4,
    BATTLE_PET_EVENT_ON_AURA_REMOVED  = 5,
    BATTLE_PET_EVENT_ON_ROUND_START   = 6,
    BATTLE_PET_EVENT_ON_ROUND_END     = 7,
    BATTLE_PET_EVENT_ON_TURN          = 8,
    BATTLE_PET_EVENT_ON_ABILITY       = 9,
    BATTLE_PET_EVENT_ON_SWAP_IN       = 10,
    BATTLE_PET_EVENT_ON_SWAP_OUT      = 11,
}
*/

enum BattlePetStates
{
    BATTLE_PET_STATE_STAT_POWER         = 18,
    BATTLE_PET_STATE_STAT_STAMINA       = 19,
    BATTLE_PET_STATE_STAT_SPEED         = 20,
    BATTLE_PET_STATE_STAT_GENDER        = 78  // currently not used by Blizzard
};

enum class BattlePetDbState
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
        m_power(power), m_speed(speed), m_quality(quality), m_breed(breedId), m_flags(flags), m_dbState(BattlePetDbState::BATTLE_PET_DB_STATE_NONE) { }

    BattlePet(uint64 id, uint16 speciesId, uint8 level, uint8 quality, uint8 breedId)
        : m_id(id), m_species(speciesId), m_nickname(""), m_timestamp(0), m_level(level), m_xp(0), m_curHealth(0), m_maxHealth(0),
        m_power(0), m_speed(0), m_quality(quality), m_breed(breedId), m_flags(0), m_dbState(BattlePetDbState::BATTLE_PET_DB_STATE_NONE)
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

    void SetNickname(std::string const& nickname);
    void SetTimestamp(uint32 timestamp) { m_timestamp = timestamp; }
    void SetQuality(ItemQualities quality);

    BattlePetDbState GetDbState() const { return m_dbState; }
    void SetDbState(BattlePetDbState state) { m_dbState = state; }

    uint8 GetFlags() const { return m_flags; }
    bool HasFlag(uint16 flag) const { return (m_flags & flag) != 0; }
    void SetFlag(uint16 flag);
    void UnSetFlag(uint16 flag);
    void SetLevel(uint8 level);

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
