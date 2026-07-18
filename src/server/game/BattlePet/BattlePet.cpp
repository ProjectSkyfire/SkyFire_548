/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "BattlePet.h"
#include "Common.h"
#include "DB2Enums.h"
#include "DB2Stores.h"
#include "DBCStores.h"

#include <algorithm>
#include <cmath>
#include <unordered_map>

namespace
{
    enum BattlePetAbilityEffectProperties
    {
        BATTLE_PET_EFFECT_DAMAGE = 24,
        BATTLE_PET_EFFECT_RAMPING_DAMAGE = 27,
        BATTLE_PET_EFFECT_DAMAGE_TOGGLE_AURA = 76,
        BATTLE_PET_EFFECT_DAMAGE_HIT_STATE = 96,
        BATTLE_PET_EFFECT_EXTRA_ATTACK_FIRST = 103,
        BATTLE_PET_EFFECT_DAMAGE_NON_LETHAL = 149
    };

    typedef std::unordered_map<uint32, BattlePetAbilityEffectEntry const*> BattlePetAbilityEffectByTurnCache;
    typedef std::unordered_map<uint64, int32> BattlePetAbilityStateCache;
    typedef std::unordered_map<uint32, uint16> BattlePetSpeciesByNpcCache;

    bool BattlePetAbilityEffectDealsDamage(uint32 propertiesId)
    {
        switch (propertiesId)
        {
            case BATTLE_PET_EFFECT_DAMAGE:
            case BATTLE_PET_EFFECT_RAMPING_DAMAGE:
            case BATTLE_PET_EFFECT_DAMAGE_TOGGLE_AURA:
            case BATTLE_PET_EFFECT_DAMAGE_HIT_STATE:
            case BATTLE_PET_EFFECT_EXTRA_ATTACK_FIRST:
            case BATTLE_PET_EFFECT_DAMAGE_NON_LETHAL:
                return true;
            default:
                return false;
        }
    }

    BattlePetSpeciesByNpcCache const& BattlePetSpeciesByNpc()
    {
        static BattlePetSpeciesByNpcCache speciesByNpc;
        if (!speciesByNpc.empty())
            return speciesByNpc;

        for (uint32 speciesId = 0; speciesId < sBattlePetSpeciesStore.GetNumRows(); ++speciesId)
        {
            BattlePetSpeciesEntry const* speciesEntry = sBattlePetSpeciesStore.LookupEntry(speciesId);
            if (speciesEntry && speciesEntry->NpcId)
                speciesByNpc[speciesEntry->NpcId] = uint16(speciesId);
        }

        return speciesByNpc;
    }

    void BuildBattlePetAbilityEffectCaches(BattlePetAbilityEffectByTurnCache& firstEffectByTurn,
        BattlePetAbilityEffectByTurnCache& firstDamageEffectByTurn)
    {
        if (!firstEffectByTurn.empty())
            return;

        for (uint32 i = 0; i < sBattlePetAbilityEffectStore.GetNumRows(); ++i)
        {
            BattlePetAbilityEffectEntry const* effectEntry = sBattlePetAbilityEffectStore.LookupEntry(i);
            if (!effectEntry)
                continue;

            if (firstEffectByTurn.find(effectEntry->TurnEntryId) == firstEffectByTurn.end())
                firstEffectByTurn[effectEntry->TurnEntryId] = effectEntry;

            if (BattlePetAbilityEffectDealsDamage(effectEntry->PropertiesId)
                && firstDamageEffectByTurn.find(effectEntry->TurnEntryId) == firstDamageEffectByTurn.end())
                firstDamageEffectByTurn[effectEntry->TurnEntryId] = effectEntry;
        }
    }

    BattlePetAbilityEffectEntry const* BattlePetAbilityEffectForTurn(uint32 turnId, bool damageOnly)
    {
        static BattlePetAbilityEffectByTurnCache firstEffectByTurn;
        static BattlePetAbilityEffectByTurnCache firstDamageEffectByTurn;
        BuildBattlePetAbilityEffectCaches(firstEffectByTurn, firstDamageEffectByTurn);

        BattlePetAbilityEffectByTurnCache::const_iterator itr = firstDamageEffectByTurn.find(turnId);
        if (itr != firstDamageEffectByTurn.end())
            return itr->second;

        if (damageOnly)
            return nullptr;

        itr = firstEffectByTurn.find(turnId);
        return itr != firstEffectByTurn.end() ? itr->second : nullptr;
    }

    uint64 BattlePetAbilityStateKey(uint32 abilityId, uint32 stateId)
    {
        return (uint64(abilityId) << 32) | stateId;
    }

    BattlePetAbilityStateCache const& BattlePetAbilityStates()
    {
        static BattlePetAbilityStateCache abilityStates;
        if (!abilityStates.empty())
            return abilityStates;

        for (uint32 i = 0; i < sBattlePetAbilityStateStore.GetNumRows(); ++i)
        {
            BattlePetAbilityStateEntry const* stateEntry = sBattlePetAbilityStateStore.LookupEntry(i);
            if (stateEntry)
                abilityStates[BattlePetAbilityStateKey(stateEntry->AbilityId, stateEntry->StateId)] = int32(stateEntry->Value);
        }

        return abilityStates;
    }
}

uint16 BattlePetHealthFromPercent(uint16 maxHealth, uint8 percent)
{
    if (!maxHealth || !percent)
        return 0;

    if (percent >= 100)
        return maxHealth;

    return uint16(std::min<uint32>(maxHealth, (uint32(maxHealth) * percent + 99) / 100));
}

uint16 BattlePetExperienceForNextLevel(uint8 level)
{
    if (!level || level >= BATTLE_PET_MAX_LEVEL)
        return 0;

    uint32 const levelIndex = uint32(level) - 1;
    GtBattlePetXpEntry const* base = sGtBattlePetXpStore.LookupEntry(100 + levelIndex);
    GtBattlePetXpEntry const* multiplier = sGtBattlePetXpStore.LookupEntry(levelIndex);
    if (base && multiplier)
    {
        float const threshold = base->value * multiplier->value;
        if (threshold > 0.0f)
            return uint16(std::min<uint32>(uint32(threshold + 0.5f), 0xFFFF));
    }

    return uint16(level) * 100;
}

uint16 BattlePetExperienceReward(uint8 petLevel, uint8 enemyLevel, uint8 participatingPetCount)
{
    if (!petLevel || !enemyLevel || !participatingPetCount || petLevel >= BATTLE_PET_MAX_LEVEL)
        return 0;

    int32 levelDifference = int32(enemyLevel) - int32(petLevel);
    levelDifference = std::min<int32>(levelDifference, 2);
    levelDifference = std::max<int32>(levelDifference, -4);

    uint32 const reward = (uint32(enemyLevel) + 9) * uint32(levelDifference + 5);
    return uint16(std::min<uint32>(reward / participatingPetCount, 0xFFFF));
}

uint8 BattlePetNormalizeWildLevel(uint8 level)
{
    if (!level)
        return 1;

    return std::min<uint8>(level, BATTLE_PET_MAX_LEVEL);
}

bool BattlePetSpeciesFlagsAllowWildCapture(uint32 flags)
{
    return (flags & BATTLE_PET_FLAG_NOT_TAMEABLE) == 0;
}

uint16 BattlePetSpeciesIdByNpcId(uint32 npcId)
{
    if (!npcId)
        return 0;

    BattlePetSpeciesByNpcCache const& speciesByNpc = BattlePetSpeciesByNpc();
    BattlePetSpeciesByNpcCache::const_iterator itr = speciesByNpc.find(npcId);
    return itr != speciesByNpc.end() ? itr->second : 0;
}

uint32 BattlePetSpeciesNpcId(uint16 speciesId)
{
    BattlePetSpeciesEntry const* speciesEntry = sBattlePetSpeciesStore.LookupEntry(speciesId);
    return speciesEntry ? speciesEntry->NpcId : 0;
}

uint32 BattlePetSpeciesFamilyMask(uint16 speciesId)
{
    BattlePetSpeciesEntry const* speciesEntry = sBattlePetSpeciesStore.LookupEntry(speciesId);
    if (!speciesEntry || speciesEntry->FamilyId >= 32)
        return 0;

    return uint32(1) << speciesEntry->FamilyId;
}

BattlePetAbilityEffectEntry const* BattlePetAbilityEffectForAbility(uint32 abilityId, bool damageOnly)
{
    std::pair<BattlePetAbilityTurnByAbilityStore::const_iterator, BattlePetAbilityTurnByAbilityStore::const_iterator> turnRange =
        sBattlePetAbilityTurnByAbilityStore.equal_range(abilityId);

    BattlePetAbilityEffectEntry const* firstEffect = nullptr;

    for (BattlePetAbilityTurnByAbilityStore::const_iterator itr = turnRange.first; itr != turnRange.second; ++itr)
    {
        BattlePetAbilityEffectEntry const* effectEntry = BattlePetAbilityEffectForTurn(itr->second.first, damageOnly);
        if (!effectEntry)
            continue;

        if (damageOnly || BattlePetAbilityEffectDealsDamage(effectEntry->PropertiesId))
            return effectEntry;

        if (!firstEffect)
            firstEffect = effectEntry;
    }

    return damageOnly ? nullptr : firstEffect;
}

int32 BattlePetAbilityStateValue(uint32 abilityId, uint32 stateId)
{
    BattlePetAbilityStateCache const& abilityStates = BattlePetAbilityStates();
    BattlePetAbilityStateCache::const_iterator itr = abilityStates.find(BattlePetAbilityStateKey(abilityId, stateId));
    return itr != abilityStates.end() ? itr->second : 0;
}

uint32 BattlePetAbilityBasePoints(uint32 abilityId)
{
    BattlePetAbilityEffectEntry const* damageEffect = BattlePetAbilityEffectForAbility(abilityId, true);
    return damageEffect ? damageEffect->PropertyValues[0] : 0;
}

uint32 BattlePetScalePointsFromStats(uint32 points, uint16 power, uint8 level)
{
    if (!points)
        return 0;

    uint32 const safeLevel = std::max<uint32>(level, 1);
    uint32 const safePower = std::max<uint32>(power, safeLevel * 8);
    uint32 const powerBonusPct = (safePower * 5) / 100;
    uint32 const scaledPoints = points + ((points * powerBonusPct) / 100);
    return std::max<uint32>(1, scaledPoints);
}

uint32 BattlePetDamageFromStats(uint32 abilityId, uint16 power, uint8 level)
{
    if (!abilityId)
        return 0;

    return BattlePetScalePointsFromStats(BattlePetAbilityBasePoints(abilityId), power, level);
}

uint16 BattlePetPowerFromBattleState(uint16 species, uint8 level, uint8 quality, uint8 breed)
{
    if (!species)
        return level * 8;

    float const basePower = BattlePetSpeciesMainStat(BATTLE_PET_STATE_STAT_POWER, species) +
        BattlePetBreedMainStatModifier(BATTLE_PET_STATE_STAT_POWER, breed);
    float const qualityMod = BattlePetQualityMultiplier(quality);
    return uint16(std::max<float>(1.0f, std::floor((basePower * std::max<uint8>(level, 1) * qualityMod) + 0.5f)));
}

uint32 BattlePetInputDamageForAbility(uint32 abilityId, BattlePet const* caster)
{
    if (!abilityId)
        return 0;

    if (!caster)
        return BattlePetDamageFromStats(abilityId, 0, 1);

    return BattlePetDamageFromStats(abilityId, caster->GetPower(), caster->GetLevel());
}

uint32 BattlePetIncomingDamageReductionFromStats(uint32 abilityId, uint16 power, uint8 level)
{
    BattlePetAbilityEntry const* abilityEntry = sBattlePetAbilityStore.LookupEntry(abilityId);
    if (!abilityEntry || !abilityEntry->AuraAbilityId)
        return 0;

    int32 const stateValue = BattlePetAbilityStateValue(
        abilityEntry->AuraAbilityId, BATTLE_PET_STATE_DAMAGE_TAKEN_FLAT);
    if (stateValue >= 0)
        return 0;

    return BattlePetScalePointsFromStats(uint32(std::abs(stateValue)), power, level);
}

uint8 BattlePetIncomingDamageReductionRounds(uint32 abilityId)
{
    BattlePetAbilityEntry const* abilityEntry = sBattlePetAbilityStore.LookupEntry(abilityId);
    if (!abilityEntry || !abilityEntry->AuraAbilityId)
        return 0;

    return uint8(std::min<uint32>(abilityEntry->AuraDuration, 255));
}

uint32 BattlePetInputIncomingDamageReductionForAbility(uint32 abilityId, BattlePet const* caster)
{
    if (!abilityId)
        return 0;

    if (!caster)
        return BattlePetIncomingDamageReductionFromStats(abilityId, 0, 1);

    return BattlePetIncomingDamageReductionFromStats(abilityId, caster->GetPower(), caster->GetLevel());
}

uint16 BattlePetAbilityCooldown(uint32 abilityId)
{
    BattlePetAbilityEntry const* abilityEntry = sBattlePetAbilityStore.LookupEntry(abilityId);
    if (!abilityEntry)
        return 0;

    return uint16(abilityEntry->Cooldown);
}

uint32 BattlePetInputEffectForAbility(uint32 abilityId)
{
    BattlePetAbilityEffectEntry const* effectEntry = BattlePetAbilityEffectForAbility(abilityId, false);
    return effectEntry ? effectEntry->Id : 0;
}

void BattlePet::CalculateStats(bool currentHealth)
{
    float basePower = BattlePetSpeciesMainStat(BATTLE_PET_STATE_STAT_POWER, m_species) +
        BattlePetBreedMainStatModifier(BATTLE_PET_STATE_STAT_POWER, m_breed);
    float baseHealth = BattlePetSpeciesMainStat(BATTLE_PET_STATE_STAT_STAMINA, m_species) +
        BattlePetBreedMainStatModifier(BATTLE_PET_STATE_STAT_STAMINA, m_breed);
    float baseSpeed = BattlePetSpeciesMainStat(BATTLE_PET_STATE_STAT_SPEED, m_species) +
        BattlePetBreedMainStatModifier(BATTLE_PET_STATE_STAT_SPEED, m_breed);

    float qualityMod = BattlePetQualityMultiplier(m_quality);

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

void BattlePet::SetTimestamp(uint32 timestamp)
{
    if (m_timestamp == timestamp)
        return;

    m_timestamp = timestamp;
    m_dbState = BattlePetDbState::BATTLE_PET_DB_STATE_SAVE;
}

void BattlePet::SetQuality(ItemQualities quality)
{
    m_quality = quality;
    CalculateStats(false);
}

void BattlePet::SetXp(uint16 xp)
{
    if (m_xp == xp)
        return;

    m_xp = xp;
    m_dbState = BattlePetDbState::BATTLE_PET_DB_STATE_SAVE;
}

void BattlePet::SetCurrentHealth(uint16 health)
{
    uint16 const newHealth = std::min<uint16>(health, m_maxHealth);
    if (m_curHealth == newHealth)
        return;

    m_curHealth = newHealth;
    m_dbState = BattlePetDbState::BATTLE_PET_DB_STATE_SAVE;
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

void BattlePet::SetLevel(uint8 level)
{
    if (!level)
        level = 1;
    else if (level > BATTLE_PET_MAX_LEVEL)
        level = BATTLE_PET_MAX_LEVEL;

    if (m_level == level)
        return;

    m_level = level;
    CalculateStats(true);
}

uint16 BattlePet::AddExperience(uint16 xp)
{
    if (!xp || m_level >= BATTLE_PET_MAX_LEVEL)
        return 0;

    uint32 remainingXp = uint32(m_xp) + xp;
    while (m_level < BATTLE_PET_MAX_LEVEL)
    {
        uint16 const nextLevelXp = BattlePetExperienceForNextLevel(m_level);
        if (!nextLevelXp || remainingXp < nextLevelXp)
            break;

        remainingXp -= nextLevelXp;
        SetLevel(m_level + 1);
    }

    if (m_level >= BATTLE_PET_MAX_LEVEL)
        remainingXp = 0;

    SetXp(uint16(std::min<uint32>(remainingXp, 0xFFFF)));
    return xp;
}
