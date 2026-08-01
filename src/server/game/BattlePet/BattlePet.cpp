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
    typedef std::unordered_map<uint32, BattlePetAbilityEffectEntry const*> BattlePetAbilityEffectByTurnCache;
    typedef std::unordered_map<uint64, int32> BattlePetAbilityStateCache;
    typedef std::unordered_map<uint32, uint16> BattlePetSpeciesByNpcCache;

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

        if (damageOnly)
        {
            BattlePetAbilityEffectByTurnCache::const_iterator itr = firstDamageEffectByTurn.find(turnId);
            return itr != firstDamageEffectByTurn.end() ? itr->second : nullptr;
        }

        BattlePetAbilityEffectByTurnCache::const_iterator damageItr = firstDamageEffectByTurn.find(turnId);
        if (damageItr != firstDamageEffectByTurn.end())
            return damageItr->second;

        BattlePetAbilityEffectByTurnCache::const_iterator itr = firstEffectByTurn.find(turnId);
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
                abilityStates[BattlePetAbilityStateKey(stateEntry->AbilityId, stateEntry->StateId)] = stateEntry->Value;
        }

        return abilityStates;
    }

    bool TurnIndexLess(std::pair<uint32, uint8> const& left, std::pair<uint32, uint8> const& right)
    {
        return left.second < right.second;
    }
}

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

bool BattlePetAbilityEffectHeals(uint32 propertiesId)
{
    return propertiesId == BATTLE_PET_EFFECT_HEAL;
}

bool BattlePetAbilityEffectAppliesAura(uint32 propertiesId)
{
    return propertiesId == BATTLE_PET_EFFECT_APPLY_AURA
        || propertiesId == BATTLE_PET_EFFECT_APPLY_AURA_DURATION
        || propertiesId == BATTLE_PET_EFFECT_APPLY_AURA_TO_TARGET
        || propertiesId == BATTLE_PET_EFFECT_APPLY_AURA_TO_TARGET_2;
}

bool BattlePetAbilityEffectAppliesAuraToTarget(uint32 propertiesId)
{
    return propertiesId == BATTLE_PET_EFFECT_APPLY_AURA_TO_TARGET
        || propertiesId == BATTLE_PET_EFFECT_APPLY_AURA_TO_TARGET_2;
}

bool BattlePetAbilityEffectAppliesWeather(uint32 propertiesId)
{
    return propertiesId == BATTLE_PET_EFFECT_APPLY_WEATHER_AURA;
}

bool BattlePetAbilityEffectRemovesAura(uint32 propertiesId)
{
    return propertiesId == BATTLE_PET_EFFECT_REMOVE_AURA;
}

bool BattlePetAbilityEffectIsKillHeal(uint32 propertiesId, BattlePetAbilityEffectEntry const* effectEntry)
{
    if (!effectEntry)
        return false;

    // Devour-style on-kill restore: Points + RequiredTargetState = Is_Dead.
    if (propertiesId != BATTLE_PET_EFFECT_POINTS_STATE_CONDITIONAL
        && propertiesId != BATTLE_PET_EFFECT_DAMAGE_STATE_CONDITIONAL)
        return false;

    return effectEntry->PropertyValues[3] == BATTLE_PET_STATE_IS_DEAD;
}

bool BattlePetAuraMakesUntargetable(uint32 auraAbilityId)
{
    if (!auraAbilityId)
        return false;

    return BattlePetAbilityStateValue(auraAbilityId, BATTLE_PET_STATE_UNTARGETABLE) != 0
        || BattlePetAbilityStateValue(auraAbilityId, BATTLE_PET_STATE_MECHANIC_IS_FLYING) != 0
        || BattlePetAbilityStateValue(auraAbilityId, BATTLE_PET_STATE_MECHANIC_IS_UNDERGROUND) != 0;
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

uint8 BattlePetSpeciesFamilyId(uint16 speciesId)
{
    BattlePetSpeciesEntry const* speciesEntry = sBattlePetSpeciesStore.LookupEntry(speciesId);
    return speciesEntry ? uint8(speciesEntry->FamilyId) : 0;
}

uint8 BattlePetAbilityTurnCount(uint32 abilityId)
{
    if (!abilityId)
        return 0;

    uint8 turnCount = 0;
    std::pair<BattlePetAbilityTurnByAbilityStore::const_iterator, BattlePetAbilityTurnByAbilityStore::const_iterator> turnRange =
        sBattlePetAbilityTurnByAbilityStore.equal_range(abilityId);

    for (BattlePetAbilityTurnByAbilityStore::const_iterator itr = turnRange.first; itr != turnRange.second; ++itr)
        turnCount = std::max<uint8>(turnCount, itr->second.second);

    return turnCount;
}

std::vector<BattlePetAbilityEffectEntry const*> BattlePetAbilityEffectsForTurn(uint32 abilityId, uint8 turnIndex)
{
    std::vector<BattlePetAbilityEffectEntry const*> effects;
    if (!abilityId || !turnIndex)
        return effects;

    std::pair<BattlePetAbilityTurnByAbilityStore::const_iterator, BattlePetAbilityTurnByAbilityStore::const_iterator> turnRange =
        sBattlePetAbilityTurnByAbilityStore.equal_range(abilityId);

    uint32 turnEntryId = 0;
    for (BattlePetAbilityTurnByAbilityStore::const_iterator itr = turnRange.first; itr != turnRange.second; ++itr)
    {
        if (itr->second.second != turnIndex)
            continue;

        turnEntryId = itr->second.first;
        break;
    }

    if (!turnEntryId)
        return effects;

    for (uint32 i = 0; i < sBattlePetAbilityEffectStore.GetNumRows(); ++i)
    {
        BattlePetAbilityEffectEntry const* effectEntry = sBattlePetAbilityEffectStore.LookupEntry(i);
        if (!effectEntry || effectEntry->TurnEntryId != turnEntryId)
            continue;

        effects.push_back(effectEntry);
    }

    std::sort(effects.begin(), effects.end(),
        [](BattlePetAbilityEffectEntry const* left, BattlePetAbilityEffectEntry const* right)
        {
            if (left->EffectIndex != right->EffectIndex)
                return left->EffectIndex < right->EffectIndex;
            return left->Id < right->Id;
        });

    return effects;
}

BattlePetAbilityEffectEntry const* BattlePetAbilityEffectForAbilityTurn(uint32 abilityId, uint8 turnIndex, bool damageOnly)
{
    std::vector<BattlePetAbilityEffectEntry const*> const effects = BattlePetAbilityEffectsForTurn(abilityId, turnIndex);
    BattlePetAbilityEffectEntry const* firstEffect = nullptr;

    for (BattlePetAbilityEffectEntry const* effectEntry : effects)
    {
        if (!effectEntry)
            continue;

        if (damageOnly || BattlePetAbilityEffectDealsDamage(effectEntry->PropertiesId))
        {
            if (BattlePetAbilityEffectDealsDamage(effectEntry->PropertiesId))
                return effectEntry;
            if (damageOnly)
                continue;
        }

        if (!firstEffect)
            firstEffect = effectEntry;
    }

    return damageOnly ? nullptr : firstEffect;
}

BattlePetAbilityEffectEntry const* BattlePetAbilityEffectForAbility(uint32 abilityId, bool damageOnly)
{
    // Prefer the earliest TurnIndex so multi-turn channels do not skip to later damage turns.
    std::pair<BattlePetAbilityTurnByAbilityStore::const_iterator, BattlePetAbilityTurnByAbilityStore::const_iterator> turnRange =
        sBattlePetAbilityTurnByAbilityStore.equal_range(abilityId);

    std::vector<std::pair<uint32, uint8>> turns;
    for (BattlePetAbilityTurnByAbilityStore::const_iterator itr = turnRange.first; itr != turnRange.second; ++itr)
        turns.push_back(itr->second);

    std::sort(turns.begin(), turns.end(), TurnIndexLess);

    BattlePetAbilityEffectEntry const* firstEffect = nullptr;
    for (std::pair<uint32, uint8> const& turn : turns)
    {
        BattlePetAbilityEffectEntry const* effectEntry = BattlePetAbilityEffectForTurn(turn.first, damageOnly);
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

uint32 BattlePetAbilityBasePointsForTurn(uint32 abilityId, uint8 turnIndex)
{
    BattlePetAbilityEffectEntry const* damageEffect = BattlePetAbilityEffectForAbilityTurn(abilityId, turnIndex, true);
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

uint32 BattlePetDamageFromStatsForTurn(uint32 abilityId, uint8 turnIndex, uint16 power, uint8 level)
{
    if (!abilityId || !turnIndex)
        return 0;

    return BattlePetScalePointsFromStats(BattlePetAbilityBasePointsForTurn(abilityId, turnIndex), power, level);
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

uint32 BattlePetInputDamageForAbilityTurn(uint32 abilityId, uint8 turnIndex, BattlePet const* caster)
{
    if (!abilityId || !turnIndex)
        return 0;

    if (!caster)
        return BattlePetDamageFromStatsForTurn(abilityId, turnIndex, 0, 1);

    return BattlePetDamageFromStatsForTurn(abilityId, turnIndex, caster->GetPower(), caster->GetLevel());
}

static BattlePetAbilityEffectEntry const* BattlePetIncomingDamageReductionEffect(uint32 abilityId)
{
    if (!abilityId)
        return nullptr;

    std::pair<BattlePetAbilityTurnByAbilityStore::const_iterator, BattlePetAbilityTurnByAbilityStore::const_iterator> turnRange =
        sBattlePetAbilityTurnByAbilityStore.equal_range(abilityId);

    for (BattlePetAbilityTurnByAbilityStore::const_iterator turnItr = turnRange.first; turnItr != turnRange.second; ++turnItr)
    {
        uint32 const turnId = turnItr->second.first;
        for (uint32 i = 0; i < sBattlePetAbilityEffectStore.GetNumRows(); ++i)
        {
            BattlePetAbilityEffectEntry const* effectEntry = sBattlePetAbilityEffectStore.LookupEntry(i);
            if (!effectEntry || effectEntry->TurnEntryId != turnId || !effectEntry->AuraAbilityId)
                continue;

            int32 const flatTaken = BattlePetAbilityStateValue(
                effectEntry->AuraAbilityId, BATTLE_PET_STATE_DAMAGE_TAKEN_FLAT);
            if (flatTaken < 0)
                return effectEntry;
        }
    }

    return nullptr;
}

uint32 BattlePetIncomingDamageReductionFromStats(uint32 abilityId, uint16 power, uint8 level)
{
    BattlePetAbilityEffectEntry const* effectEntry = BattlePetIncomingDamageReductionEffect(abilityId);
    if (!effectEntry)
        return 0;

    int32 const stateValue = BattlePetAbilityStateValue(
        effectEntry->AuraAbilityId, BATTLE_PET_STATE_DAMAGE_TAKEN_FLAT);
    if (stateValue >= 0)
        return 0;

    return BattlePetScalePointsFromStats(uint32(std::abs(stateValue)), power, level);
}

uint8 BattlePetIncomingDamageReductionRounds(uint32 abilityId)
{
    BattlePetAbilityEffectEntry const* effectEntry = BattlePetIncomingDamageReductionEffect(abilityId);
    if (!effectEntry)
        return 0;

    // Apply-aura effects store duration in Param[2] (BattlePetEffectProperties "Duration").
    return uint8(std::min<uint32>(effectEntry->PropertyValues[2], 255));
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

uint32 BattlePetInputEffectForAbilityTurn(uint32 abilityId, uint8 turnIndex)
{
    BattlePetAbilityEffectEntry const* effectEntry = BattlePetAbilityEffectForAbilityTurn(abilityId, turnIndex, false);
    return effectEntry ? effectEntry->Id : 0;
}

uint32 BattlePetAbilitySpellVisual(uint32 abilityId)
{
    BattlePetAbilityEntry const* abilityEntry = sBattlePetAbilityStore.LookupEntry(abilityId);
    if (!abilityEntry || !abilityEntry->VisualId)
        return 0;

    BattlePetVisualEntry const* visualEntry = sBattlePetVisualStore.LookupEntry(abilityEntry->VisualId);
    return visualEntry ? visualEntry->SpellVisualId : 0;
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
