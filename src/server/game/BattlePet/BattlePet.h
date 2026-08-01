/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef BATTLE_PET_H
#define BATTLE_PET_H

#include "Common.h"
#include "SharedDefines.h"

#include <vector>

#define BATTLE_PET_MAX_LEVEL       25
#define BATTLE_PET_MAX_NAME_LENGTH 16

constexpr uint8 BATTLE_PET_ABILITY_SLOT_COUNT = 3;
constexpr uint8 BATTLE_PET_ABILITY_SLOT_INVALID = BATTLE_PET_ABILITY_SLOT_COUNT;

enum BattlePetAchievementSource
{
    BATTLE_PET_ACHIEVEMENT_SOURCE_WILD = 0,
    BATTLE_PET_ACHIEVEMENT_SOURCE_PVP_DUEL = 1,
    BATTLE_PET_ACHIEVEMENT_SOURCE_PVP_MATCHMAKING = 2
};

inline char const* BattlePetAchievementSourceName(BattlePetAchievementSource source)
{
    switch (source)
    {
        case BATTLE_PET_ACHIEVEMENT_SOURCE_WILD:
            return "wild";
        case BATTLE_PET_ACHIEVEMENT_SOURCE_PVP_DUEL:
            return "pvp-duel";
        case BATTLE_PET_ACHIEVEMENT_SOURCE_PVP_MATCHMAKING:
            return "pvp-matchmaking";
        default:
            return "unknown";
    }
}

inline uint8 BattlePetCapturedLevel(uint8 level)
{
    if (level >= 21)
        return level - 2;

    if (level >= 16)
        return level - 1;

    return level;
}

inline uint32 BattlePetAchievementCriteriaPayload(uint8 quality, BattlePetAchievementSource source, uint8 healthPercent = 0)
{
    return uint32(quality) | (uint32(source) << 8) | (uint32(healthPercent) << 16);
}

inline uint32 BattlePetAchievementCriteriaPayload(uint8 quality, bool pvpBattle, uint8 healthPercent = 0)
{
    return BattlePetAchievementCriteriaPayload(quality,
        pvpBattle ? BATTLE_PET_ACHIEVEMENT_SOURCE_PVP_DUEL : BATTLE_PET_ACHIEVEMENT_SOURCE_WILD,
        healthPercent);
}

inline uint8 BattlePetAchievementCriteriaQuality(uint32 payload)
{
    return uint8(payload & 0xFF);
}

inline uint8 BattlePetAchievementCriteriaSource(uint32 payload)
{
    return uint8((payload >> 8) & 0xFF);
}

inline bool BattlePetAchievementCriteriaIsPvp(uint32 payload)
{
    return BattlePetAchievementCriteriaSource(payload) != BATTLE_PET_ACHIEVEMENT_SOURCE_WILD;
}

inline bool BattlePetAchievementCriteriaIsMatchmaking(uint32 payload)
{
    return BattlePetAchievementCriteriaSource(payload) == BATTLE_PET_ACHIEVEMENT_SOURCE_PVP_MATCHMAKING;
}

inline uint8 BattlePetAchievementCriteriaHealthPercent(uint32 payload)
{
    return uint8((payload >> 16) & 0xFF);
}

struct BattlePetAchievementContext
{
    uint16 Species = 0;
    uint32 FamilyMask = 0;
    uint8 Quality = 0;
    BattlePetAchievementSource Source = BATTLE_PET_ACHIEVEMENT_SOURCE_WILD;
    uint8 HealthPercent = 0;
    bool Won = false;
    bool Captured = false;

    bool IsValid() const { return Species != 0; }
    bool IsPvp() const { return Source != BATTLE_PET_ACHIEVEMENT_SOURCE_WILD; }
    bool IsMatchmaking() const { return Source == BATTLE_PET_ACHIEVEMENT_SOURCE_PVP_MATCHMAKING; }
    uint32 Payload() const { return BattlePetAchievementCriteriaPayload(Quality, Source, HealthPercent); }
};

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
    BATTLE_PET_STATE_IS_DEAD = 1,
    BATTLE_PET_STATE_STAT_POWER = 18,
    BATTLE_PET_STATE_STAT_STAMINA = 19,
    BATTLE_PET_STATE_STAT_SPEED = 20,
    BATTLE_PET_STATE_MOD_DAMAGE_DEALT_PERCENT = 23,
    BATTLE_PET_STATE_UNTARGETABLE = 29,
    BATTLE_PET_STATE_MECHANIC_IS_UNDERGROUND = 30,
    BATTLE_PET_STATE_MECHANIC_IS_FLYING = 33,
    BATTLE_PET_STATE_WEATHER_RAIN = 60,
    BATTLE_PET_STATE_DAMAGE_TAKEN_FLAT = 71,
    BATTLE_PET_STATE_STAT_GENDER = 78,  // currently not used by Blizzard
    BATTLE_PET_STATE_MOD_PET_TYPE_DAMAGE_DEALT_PERCENT = 87,
    BATTLE_PET_STATE_MOD_PET_TYPE_ID = 89
};

enum BattlePetAbilityEffectProperties
{
    BATTLE_PET_EFFECT_HEAL = 23,
    BATTLE_PET_EFFECT_DAMAGE = 24,
    BATTLE_PET_EFFECT_APPLY_AURA = 26,
    BATTLE_PET_EFFECT_RAMPING_DAMAGE = 27,
    BATTLE_PET_EFFECT_APPLY_AURA_DURATION = 28,
    BATTLE_PET_EFFECT_DAMAGE_STATE_CONDITIONAL = 29,
    BATTLE_PET_EFFECT_REMOVE_AURA = 49,
    BATTLE_PET_EFFECT_APPLY_AURA_TO_TARGET = 50,
    BATTLE_PET_EFFECT_DAMAGE_TOGGLE_AURA = 76,
    BATTLE_PET_EFFECT_APPLY_WEATHER_AURA = 80,
    BATTLE_PET_EFFECT_DAMAGE_HIT_STATE = 96,
    BATTLE_PET_EFFECT_EXTRA_ATTACK_FIRST = 103,
    BATTLE_PET_EFFECT_POINTS_STATE_CONDITIONAL = 104,
    BATTLE_PET_EFFECT_DAMAGE_NON_LETHAL = 149,
    BATTLE_PET_EFFECT_APPLY_AURA_TO_TARGET_2 = 168
};

enum class BattlePetDbState
{
    BATTLE_PET_DB_STATE_NONE = 0,
    BATTLE_PET_DB_STATE_DELETE = 1,
    BATTLE_PET_DB_STATE_SAVE = 2
};

// flags used in 'account_battle_pet' db table
enum BattlePetJournalFlags
{
    BATTLE_PET_JOURNAL_FLAG_NONE = 0x00,
    BATTLE_PET_JOURNAL_FLAG_FAVORITES = 0x01,
    BATTLE_PET_JOURNAL_FLAG_COLLECTED = 0x02, // name dumped from client, use unknown
    BATTLE_PET_JOURNAL_FLAG_NOT_COLLECTED = 0x04,
    BATTLE_PET_JOURNAL_FLAG_UNKNOWN_1 = 0x08,
    BATTLE_PET_JOURNAL_FLAG_ABILITY_1 = 0x10, // ability flags are set if the second ability for that slot is selected
    BATTLE_PET_JOURNAL_FLAG_ABILITY_2 = 0x20, // ...
    BATTLE_PET_JOURNAL_FLAG_ABILITY_3 = 0x40  // ...
};

inline uint8 BattlePetAbilitySlotForJournalFlag(uint32 flag)
{
    switch (flag)
    {
        case BATTLE_PET_JOURNAL_FLAG_ABILITY_1:
            return 0;
        case BATTLE_PET_JOURNAL_FLAG_ABILITY_2:
            return 1;
        case BATTLE_PET_JOURNAL_FLAG_ABILITY_3:
            return 2;
        default:
            return BATTLE_PET_ABILITY_SLOT_INVALID;
    }
}

uint16 BattlePetHealthFromPercent(uint16 maxHealth, uint8 percent);
uint16 BattlePetExperienceForNextLevel(uint8 level);
uint16 BattlePetExperienceReward(uint8 petLevel, uint8 enemyLevel, uint8 participatingPetCount = 1);
uint8 BattlePetNormalizeWildLevel(uint8 level);
bool BattlePetSpeciesFlagsAllowWildCapture(uint32 flags);

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
    void SetTimestamp(uint32 timestamp);
    void SetQuality(ItemQualities quality);
    void SetXp(uint16 xp);
    void SetCurrentHealth(uint16 health);

    BattlePetDbState GetDbState() const { return m_dbState; }
    void SetDbState(BattlePetDbState state) { m_dbState = state; }

    uint16 GetFlags() const { return m_flags; }
    bool HasFlag(uint16 flag) const { return (m_flags & flag) != 0; }
    void SetFlag(uint16 flag);
    void UnSetFlag(uint16 flag);
    void SetLevel(uint8 level);
    uint16 AddExperience(uint16 xp);

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

struct BattlePetAbilityEffectEntry;

uint16 BattlePetSpeciesIdByNpcId(uint32 npcId);
uint32 BattlePetSpeciesNpcId(uint16 speciesId);
uint32 BattlePetSpeciesFamilyMask(uint16 speciesId);
uint8 BattlePetSpeciesFamilyId(uint16 speciesId);
BattlePetAbilityEffectEntry const* BattlePetAbilityEffectForAbility(uint32 abilityId, bool damageOnly);
BattlePetAbilityEffectEntry const* BattlePetAbilityEffectForAbilityTurn(uint32 abilityId, uint8 turnIndex, bool damageOnly);
uint8 BattlePetAbilityTurnCount(uint32 abilityId);
std::vector<BattlePetAbilityEffectEntry const*> BattlePetAbilityEffectsForTurn(uint32 abilityId, uint8 turnIndex);
bool BattlePetAbilityEffectDealsDamage(uint32 propertiesId);
bool BattlePetAbilityEffectHeals(uint32 propertiesId);
bool BattlePetAbilityEffectAppliesAura(uint32 propertiesId);
bool BattlePetAbilityEffectAppliesAuraToTarget(uint32 propertiesId);
bool BattlePetAbilityEffectAppliesWeather(uint32 propertiesId);
bool BattlePetAbilityEffectRemovesAura(uint32 propertiesId);
bool BattlePetAbilityEffectIsKillHeal(uint32 propertiesId, BattlePetAbilityEffectEntry const* effectEntry);
bool BattlePetAuraMakesUntargetable(uint32 auraAbilityId);
int32 BattlePetAbilityStateValue(uint32 abilityId, uint32 stateId);
uint32 BattlePetAbilityBasePoints(uint32 abilityId);
uint32 BattlePetAbilityBasePointsForTurn(uint32 abilityId, uint8 turnIndex);
uint32 BattlePetScalePointsFromStats(uint32 points, uint16 power, uint8 level);
uint32 BattlePetDamageFromStats(uint32 abilityId, uint16 power, uint8 level);
uint32 BattlePetDamageFromStatsForTurn(uint32 abilityId, uint8 turnIndex, uint16 power, uint8 level);
uint16 BattlePetPowerFromBattleState(uint16 species, uint8 level, uint8 quality, uint8 breed);
uint32 BattlePetInputDamageForAbility(uint32 abilityId, BattlePet const* caster);
uint32 BattlePetInputDamageForAbilityTurn(uint32 abilityId, uint8 turnIndex, BattlePet const* caster);
uint32 BattlePetIncomingDamageReductionFromStats(uint32 abilityId, uint16 power, uint8 level);
uint8 BattlePetIncomingDamageReductionRounds(uint32 abilityId);
uint32 BattlePetInputIncomingDamageReductionForAbility(uint32 abilityId, BattlePet const* caster);
uint16 BattlePetAbilityCooldown(uint32 abilityId);
uint32 BattlePetInputEffectForAbility(uint32 abilityId);
uint32 BattlePetInputEffectForAbilityTurn(uint32 abilityId, uint8 turnIndex);
uint32 BattlePetAbilitySpellVisual(uint32 abilityId);

#endif
