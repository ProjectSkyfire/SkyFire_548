/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef BATTLE_PET_BATTLE_STATE_H
#define BATTLE_PET_BATTLE_STATE_H

#include "BattlePet.h"
#include "Common.h"

#include <vector>

enum PetBattleWinner
{
    PET_BATTLE_WINNER_NONE = 0,
    PET_BATTLE_WINNER_ALLY = 1,
    PET_BATTLE_WINNER_ENEMY = 2
};

enum PetBattleTrapStatus
{
    PET_BATTLE_TRAP_STATUS_ACTIVE = 1,
    PET_BATTLE_TRAP_STATUS_UNAVAILABLE = 2,
    PET_BATTLE_TRAP_STATUS_DEAD_PET = 3,
    PET_BATTLE_TRAP_STATUS_HEALTH_TOO_HIGH = 4,
    PET_BATTLE_TRAP_STATUS_JOURNAL_FULL = 5,
    PET_BATTLE_TRAP_STATUS_NOT_CAPTURABLE = 6,
    PET_BATTLE_TRAP_STATUS_NPC_PET = 7,
    PET_BATTLE_TRAP_STATUS_ALREADY_TRAPPED = 8
};

enum ActivePetBattleTurnEffect
{
    ACTIVE_PET_BATTLE_TURN_EFFECT_NONE = 0,
    ACTIVE_PET_BATTLE_TURN_EFFECT_DAMAGE = 1,
    ACTIVE_PET_BATTLE_TURN_EFFECT_SWAP = 2,
    ACTIVE_PET_BATTLE_TURN_EFFECT_ABILITY = 3
};

struct ActivePetBattleCooldown
{
    uint8 PetPBOID = 0;
    uint8 AbilitySlot = 0;
    uint32 AbilityID = 0;
    uint16 Cooldown = 0;
    uint16 Lockdown = 0;
    uint8 NextTurnIndex = 0;
    bool OnCooldown = false;
};

struct ActivePetBattleTurn
{
    bool Accepted = false;
    bool HasRoundResult = false;
    bool HasFinalRound = false;
    bool TargetDied = false;
    bool Abandoned = false;
    bool Captured = false;
    bool RequiresFrontPet = false;
    uint8 EffectKind = ACTIVE_PET_BATTLE_TURN_EFFECT_NONE;
    uint8 CasterPet = 0;
    uint8 TargetPet = 0;
    uint8 Winner = PET_BATTLE_WINNER_NONE;
    uint32 RoundID = 0;
    uint32 RemainingHealth = 0;
    uint32 DamageDealt = 0;
    std::vector<ActivePetBattleCooldown> Cooldowns;
};

struct ActivePetBattlePetState
{
    uint64 PetID = 0;
    uint32 MaxHealth = 0;
    uint32 Health = 0;
};

struct ActivePetBattleAuraState
{
    uint32 IncomingDamageReduction = 0;
    uint8 Rounds = 0;
    uint32 AuraAbilityId = 0;
    bool Untargetable = false;
};

struct ActivePetBattleWeather
{
    uint32 AuraAbilityId = 0;
    uint8 Rounds = 0;
    uint8 BoostedFamilyId = 0;
    int32 FamilyDamagePct = 0;

    bool IsActive() const { return AuraAbilityId != 0 && Rounds != 0; }
};

struct ActivePetBattle
{
    static constexpr uint8 BATTLE_PET_MAX_ACTIVE_TEAM_PETS = 3;

    bool IsActive() const { return EnemyGUID != 0; }
    bool IsFinished() const { return Finished; }
    bool IsPvP() const { return PvPBattle; }
    bool IsMatchmaking() const { return Source == BATTLE_PET_ACHIEVEMENT_SOURCE_PVP_MATCHMAKING; }
    BattlePetAchievementSource GetAchievementSource() const { return Source; }
    bool CanAcceptInput(uint32 roundId) const { return CanAcceptRound(roundId); }
    bool CanTrap(bool canStoreBattlePet = true, bool enemyTameable = true) const
    {
        return GetTrapStatus(canStoreBattlePet, enemyTameable) == PET_BATTLE_TRAP_STATUS_ACTIVE;
    }

    bool SelectAllyFrontPet(uint8 frontPet)
    {
        if (!IsActive() || IsFinished() || !IsAllyPetAlive(frontPet))
            return false;

        AllyFrontPet = frontPet;
        WaitingForAllyFrontPet = false;
        SyncActiveAllyPet();
        return true;
    }

    void StartWild(uint64 enemyGuid, uint64 allyPetId, uint32 allyMaxHealth, uint32 allyHealth,
        uint64 enemyPetId, uint32 enemyMaxHealth, uint32 enemyHealth,
        uint16 enemySpecies = 0, uint8 enemyLevel = 0, uint8 enemyQuality = 0,
        uint8 enemyBreed = 0, uint8 allyFrontPet = 0)
    {
        *this = ActivePetBattle();

        PvPBattle = false;
        Source = BATTLE_PET_ACHIEVEMENT_SOURCE_WILD;
        EnemyGUID = enemyGuid;
        EnemyPetID = enemyPetId;
        EnemyMaxHealth = enemyMaxHealth;
        EnemyHealth = NormalizeHealth(enemyHealth, enemyMaxHealth);
        EnemySpecies = enemySpecies;
        EnemyLevel = enemyLevel;
        EnemyQuality = enemyQuality;
        EnemyBreed = enemyBreed;
        AllyFrontPet = IsValidAllyPetIndex(allyFrontPet) ? allyFrontPet : 0;
        SetAllyPet(AllyFrontPet, allyPetId, allyMaxHealth, allyHealth);
    }

    void StartPvp(uint64 opponentGuid, uint64 allyPetId, uint32 allyMaxHealth, uint32 allyHealth,
        uint64 enemyPetId, uint32 enemyMaxHealth, uint32 enemyHealth,
        uint16 enemySpecies = 0, uint8 enemyLevel = 0, uint8 enemyQuality = 0,
        uint8 allyFrontPet = 0, uint8 enemyFrontPet = 3,
        BattlePetAchievementSource source = BATTLE_PET_ACHIEVEMENT_SOURCE_PVP_DUEL)
    {
        *this = ActivePetBattle();

        PvPBattle = true;
        Source = source == BATTLE_PET_ACHIEVEMENT_SOURCE_PVP_MATCHMAKING
            ? BATTLE_PET_ACHIEVEMENT_SOURCE_PVP_MATCHMAKING
            : BATTLE_PET_ACHIEVEMENT_SOURCE_PVP_DUEL;
        EnemyGUID = opponentGuid;
        EnemyPetID = enemyPetId;
        EnemyMaxHealth = enemyMaxHealth;
        EnemyHealth = NormalizeHealth(enemyHealth, enemyMaxHealth);
        EnemySpecies = enemySpecies;
        EnemyLevel = enemyLevel;
        EnemyQuality = enemyQuality;
        AllyFrontPet = IsValidAllyPetIndex(allyFrontPet) ? allyFrontPet : 0;
        EnemyFrontPet = enemyFrontPet;
        SetAllyPet(AllyFrontPet, allyPetId, allyMaxHealth, allyHealth);
    }

    uint32 ApplyAllyDamage(uint32 damage)
    {
        if (!IsAllyPetAlive(AllyFrontPet))
            return AllyTeam[AllyFrontPet].Health;

        if (AllyAura.Untargetable)
            return AllyTeam[AllyFrontPet].Health;

        damage = ApplyIncomingDamageReduction(AllyAura, damage);

        if (damage >= AllyTeam[AllyFrontPet].Health)
            AllyTeam[AllyFrontPet].Health = 0;
        else
            AllyTeam[AllyFrontPet].Health -= damage;

        uint32 const remainingHealth = AllyTeam[AllyFrontPet].Health;
        if (!remainingHealth)
        {
            if (HasAliveAllyPetOtherThan(AllyFrontPet))
                WaitingForAllyFrontPet = true;
            else
                Finish(PET_BATTLE_WINNER_ENEMY);
        }

        SyncActiveAllyPet();
        return remainingHealth;
    }

    uint32 ApplyAllyHealToPet(uint8 petIndex, uint32 amount)
    {
        if (!amount || !IsAllyPetAlive(petIndex))
            return HasAllyPet(petIndex) ? AllyTeam[petIndex].Health : 0;

        ActivePetBattlePetState& pet = AllyTeam[petIndex];
        uint32 const healed = std::min<uint32>(amount, pet.MaxHealth > pet.Health ? pet.MaxHealth - pet.Health : 0);
        pet.Health += healed;
        if (petIndex == AllyFrontPet)
            SyncActiveAllyPet();
        return pet.Health;
    }

    uint32 ApplyAllyHeal(uint32 amount)
    {
        return ApplyAllyHealToPet(AllyFrontPet, amount);
    }

    uint32 ApplyEnemyDamage(uint32 damage)
    {
        if (EnemyAura.Untargetable)
            return EnemyHealth;

        damage = ApplyIncomingDamageReduction(EnemyAura, damage);
        return ApplyDamage(EnemyHealth, damage, PET_BATTLE_WINNER_ALLY);
    }

    void ActivateAllyIncomingDamageReduction(uint32 amount, uint8 rounds)
    {
        ActivateIncomingDamageReduction(AllyAura, amount, rounds);
    }

    void ActivateEnemyIncomingDamageReduction(uint32 amount, uint8 rounds)
    {
        ActivateIncomingDamageReduction(EnemyAura, amount, rounds);
    }

    void ActivateAllyCombatAura(uint32 auraAbilityId, uint8 rounds, bool untargetable)
    {
        ActivateCombatAura(AllyAura, auraAbilityId, rounds, untargetable);
    }

    void ActivateEnemyCombatAura(uint32 auraAbilityId, uint8 rounds, bool untargetable)
    {
        ActivateCombatAura(EnemyAura, auraAbilityId, rounds, untargetable);
    }

    void ClearAllyCombatAura(uint32 auraAbilityId = 0)
    {
        ClearCombatAura(AllyAura, auraAbilityId);
    }

    void ClearEnemyCombatAura(uint32 auraAbilityId = 0)
    {
        ClearCombatAura(EnemyAura, auraAbilityId);
    }

    bool IsAllyUntargetable() const { return AllyAura.Untargetable; }
    bool IsEnemyUntargetable() const { return EnemyAura.Untargetable; }

    uint32 ScaleEnemyOutgoingDamage(uint32 damage) const
    {
        if (!damage || !EnemyAura.AuraAbilityId || !EnemyAura.Rounds)
            return damage;

        int32 const pct = BattlePetAbilityStateValue(
            EnemyAura.AuraAbilityId, BATTLE_PET_STATE_MOD_DAMAGE_DEALT_PERCENT);
        if (!pct)
            return damage;

        int32 const scaled = int32(damage) + (int32(damage) * pct) / 100;
        return scaled > 0 ? uint32(scaled) : 0;
    }

    void ActivateWeather(uint32 auraAbilityId, uint8 rounds)
    {
        Weather = ActivePetBattleWeather();
        if (!auraAbilityId || !rounds)
            return;

        Weather.AuraAbilityId = auraAbilityId;
        Weather.Rounds = rounds;
        Weather.FamilyDamagePct = BattlePetAbilityStateValue(
            auraAbilityId, BATTLE_PET_STATE_MOD_PET_TYPE_DAMAGE_DEALT_PERCENT);
        int32 const familyId = BattlePetAbilityStateValue(
            auraAbilityId, BATTLE_PET_STATE_MOD_PET_TYPE_ID);
        Weather.BoostedFamilyId = familyId > 0 ? uint8(familyId) : 0;
    }

    void ActivateWeather(uint32 auraAbilityId, uint8 rounds, uint8 boostedFamilyId, int32 familyDamagePct)
    {
        Weather = ActivePetBattleWeather();
        if (!auraAbilityId || !rounds)
            return;

        Weather.AuraAbilityId = auraAbilityId;
        Weather.Rounds = rounds;
        Weather.BoostedFamilyId = boostedFamilyId;
        Weather.FamilyDamagePct = familyDamagePct;
    }

    void ClearWeather()
    {
        Weather = ActivePetBattleWeather();
    }

    uint32 ScaleAllyOutgoingDamage(uint32 damage, uint8 casterFamilyId) const
    {
        if (!damage || !Weather.IsActive() || !Weather.FamilyDamagePct)
            return damage;

        if (Weather.BoostedFamilyId && casterFamilyId != Weather.BoostedFamilyId)
            return damage;

        int32 const scaled = int32(damage) + (int32(damage) * Weather.FamilyDamagePct) / 100;
        return scaled > 0 ? uint32(scaled) : 0;
    }

    bool GetAllyAbilityChannel(uint8 petIndex, uint8& abilitySlot, uint32& abilityId, uint8& nextTurnIndex) const
    {
        if (!IsValidAllyPetIndex(petIndex))
            return false;

        for (uint8 slot = 0; slot < BATTLE_PET_ABILITY_SLOT_COUNT; ++slot)
        {
            ActivePetBattleCooldown const& cooldownState = AllyCooldowns[petIndex][slot];
            if (!cooldownState.OnCooldown || !cooldownState.NextTurnIndex)
                continue;

            abilitySlot = slot;
            abilityId = cooldownState.AbilityID;
            nextTurnIndex = cooldownState.NextTurnIndex;
            return true;
        }

        return false;
    }

    bool HasAllyAbilityChannel(uint8 petIndex = 0xFF) const
    {
        uint8 abilitySlot = 0;
        uint32 abilityId = 0;
        uint8 nextTurnIndex = 0;
        uint8 const pet = petIndex == 0xFF ? AllyFrontPet : petIndex;
        return GetAllyAbilityChannel(pet, abilitySlot, abilityId, nextTurnIndex);
    }

    ActivePetBattleTurn ApplyAbilityRound(uint32 roundId, uint32 damage)
    {
        return ApplyAbilityRound(roundId, damage, BATTLE_PET_ABILITY_SLOT_INVALID, 0, 0);
    }

    ActivePetBattleTurn ApplyAbilityRound(uint32 roundId, uint32 damage, uint8 abilitySlot,
        uint32 abilityId, uint16 abilityCooldown)
    {
        ActivePetBattleTurn turn = CreateTurn(roundId);
        if (!CanAcceptRound(roundId))
            return turn;

        if (WaitingForAllyFrontPet)
            return turn;

        if (IsValidAbilitySlot(abilitySlot) && IsAllyAbilityOnCooldown(AllyFrontPet, abilitySlot))
            return turn;

        turn.Accepted = true;
        turn.HasRoundResult = true;
        turn.EffectKind = ACTIVE_PET_BATTLE_TURN_EFFECT_DAMAGE;
        turn.CasterPet = AllyFrontPet;
        turn.TargetPet = EnemyFrontPet;
        turn.RemainingHealth = ApplyEnemyDamage(damage);
        turn.TargetDied = turn.RemainingHealth == 0;
        turn.HasFinalRound = IsFinished();
        turn.Winner = Winner;

        StartAllyAbilityCooldown(AllyFrontPet, abilitySlot, abilityId, abilityCooldown);
        FinishAcceptedRound(turn);
        return turn;
    }

    ActivePetBattleTurn ApplyEnemyAbilityRound(uint32 roundId, uint32 damage)
    {
        ActivePetBattleTurn turn = CreateTurn(roundId);
        if (!CanAcceptRound(roundId))
            return turn;

        if (WaitingForAllyFrontPet)
            return turn;

        turn.Accepted = true;
        turn.HasRoundResult = true;
        turn.EffectKind = ACTIVE_PET_BATTLE_TURN_EFFECT_DAMAGE;
        turn.CasterPet = EnemyFrontPet;
        turn.TargetPet = AllyFrontPet;
        turn.RemainingHealth = ApplyAllyDamage(damage);
        turn.TargetDied = turn.RemainingHealth == 0;
        turn.RequiresFrontPet = turn.TargetDied && WaitingForAllyFrontPet;
        turn.HasFinalRound = IsFinished();
        turn.Winner = Winner;

        FinishAcceptedRound(turn);
        return turn;
    }

    bool ApplyAbilityExchange(uint32 roundId, uint32 allyDamage, uint32 enemyDamage,
        ActivePetBattleTurn& allyTurn, ActivePetBattleTurn& enemyTurn)
    {
        return ApplyAbilityExchange(roundId, allyDamage, enemyDamage, BATTLE_PET_ABILITY_SLOT_INVALID,
            0, 0, allyTurn, enemyTurn);
    }

    bool ApplyAbilityExchange(uint32 roundId, uint32 allyDamage, uint32 enemyDamage,
        uint8 allyAbilitySlot, uint32 allyAbilityId, uint16 allyAbilityCooldown,
        ActivePetBattleTurn& allyTurn, ActivePetBattleTurn& enemyTurn)
    {
        return ApplyAbilityExchange(roundId, allyDamage, enemyDamage, allyAbilitySlot, allyAbilityId,
            allyAbilityCooldown, 0, 0, allyTurn, enemyTurn);
    }

    bool ApplyAbilityExchange(uint32 roundId, uint32 allyDamage, uint32 enemyDamage,
        uint8 allyAbilitySlot, uint32 allyAbilityId, uint16 allyAbilityCooldown,
        uint16 allyAbilityLockdown, uint8 allyNextTurnIndex,
        ActivePetBattleTurn& allyTurn, ActivePetBattleTurn& enemyTurn)
    {
        allyTurn = CreateTurn(roundId);
        enemyTurn = CreateTurn(roundId);

        if (!CanAcceptRound(roundId))
            return false;

        if (WaitingForAllyFrontPet)
            return false;

        bool const continuingChannel = IsContinuingAllyAbilityChannel(AllyFrontPet, allyAbilitySlot, allyAbilityId);
        if (IsValidAbilitySlot(allyAbilitySlot) && IsAllyAbilityOnCooldown(AllyFrontPet, allyAbilitySlot) && !continuingChannel)
            return false;

        allyTurn.Accepted = true;
        allyTurn.HasRoundResult = true;
        allyTurn.EffectKind = allyDamage ? ACTIVE_PET_BATTLE_TURN_EFFECT_DAMAGE : ACTIVE_PET_BATTLE_TURN_EFFECT_ABILITY;
        allyTurn.CasterPet = AllyFrontPet;
        allyTurn.TargetPet = EnemyFrontPet;
        allyTurn.DamageDealt = allyDamage;
        allyTurn.RemainingHealth = ApplyEnemyDamage(allyDamage);
        allyTurn.TargetDied = EnemyHealth == 0;
        allyTurn.HasFinalRound = IsFinished();
        allyTurn.Winner = Winner;

        if (!IsFinished())
        {
            uint32 const appliedEnemyDamage = IsAllyUntargetable() ? 0 : enemyDamage;
            enemyTurn.Accepted = true;
            enemyTurn.HasRoundResult = true;
            enemyTurn.EffectKind = appliedEnemyDamage ? ACTIVE_PET_BATTLE_TURN_EFFECT_DAMAGE : ACTIVE_PET_BATTLE_TURN_EFFECT_ABILITY;
            enemyTurn.CasterPet = EnemyFrontPet;
            enemyTurn.TargetPet = AllyFrontPet;
            enemyTurn.DamageDealt = appliedEnemyDamage;
            enemyTurn.RemainingHealth = ApplyAllyDamage(appliedEnemyDamage);
            enemyTurn.TargetDied = enemyTurn.RemainingHealth == 0 && appliedEnemyDamage > 0;
            enemyTurn.RequiresFrontPet = enemyTurn.TargetDied && WaitingForAllyFrontPet;
            enemyTurn.HasFinalRound = IsFinished();
            enemyTurn.Winner = Winner;
        }

        StartAllyAbilityCooldown(AllyFrontPet, allyAbilitySlot, allyAbilityId, allyAbilityCooldown,
            allyAbilityLockdown, allyNextTurnIndex);
        FinishAcceptedRound(allyTurn);
        return true;
    }

    ActivePetBattleTurn ApplySwapRound(uint32 roundId, uint8 newFrontPet)
    {
        ActivePetBattleTurn turn = CreateTurn(roundId);
        if (!CanAcceptRound(roundId))
            return turn;

        if (!IsAllyPetAlive(newFrontPet))
            return turn;

        turn.Accepted = true;
        turn.HasRoundResult = true;
        turn.EffectKind = ACTIVE_PET_BATTLE_TURN_EFFECT_SWAP;
        turn.CasterPet = AllyFrontPet;
        turn.TargetPet = newFrontPet;

        AllyFrontPet = newFrontPet;
        WaitingForAllyFrontPet = false;
        SyncActiveAllyPet();
        FinishAcceptedRound(turn);
        return turn;
    }

    ActivePetBattleTurn ApplyForfeit(uint32 roundId)
    {
        ActivePetBattleTurn turn = CreateTurn(roundId);
        if (!CanAcceptRound(roundId))
            return turn;

        Finish(PET_BATTLE_WINNER_ENEMY);

        turn.Accepted = true;
        turn.HasFinalRound = true;
        turn.Abandoned = true;
        turn.Winner = Winner;
        return turn;
    }

    ActivePetBattleTurn ApplyTrapRound(uint32 roundId, bool captured)
    {
        ActivePetBattleTurn turn = CreateTurn(roundId);
        if (!CanAcceptRound(roundId))
            return turn;

        if (WaitingForAllyFrontPet)
            return turn;

        if (!CanTrap())
            return turn;

        turn.Accepted = true;
        turn.Captured = captured;

        if (captured)
        {
            Finish(PET_BATTLE_WINNER_ALLY);
            Captured = true;

            turn.HasFinalRound = true;
            turn.Winner = Winner;
        }
        else
        {
            ++TrapFailedAttempts;
            turn.HasRoundResult = true;
            FinishAcceptedRound(turn);
        }

        return turn;
    }

    bool ApplyTrapRoundWithEnemyResponse(uint32 roundId, bool captured, uint32 enemyDamage,
        ActivePetBattleTurn& trapTurn, ActivePetBattleTurn& enemyTurn)
    {
        trapTurn = CreateTurn(roundId);
        enemyTurn = CreateTurn(roundId);

        if (!CanAcceptRound(roundId))
            return false;

        if (WaitingForAllyFrontPet)
            return false;

        if (!CanTrap())
            return false;

        trapTurn.Accepted = true;
        trapTurn.Captured = captured;

        if (captured)
        {
            Finish(PET_BATTLE_WINNER_ALLY);
            Captured = true;

            trapTurn.HasFinalRound = true;
            trapTurn.Winner = Winner;
            return true;
        }

        ++TrapFailedAttempts;
        trapTurn.HasRoundResult = true;

        enemyTurn.Accepted = true;
        enemyTurn.HasRoundResult = true;
        enemyTurn.EffectKind = ACTIVE_PET_BATTLE_TURN_EFFECT_DAMAGE;
        enemyTurn.CasterPet = EnemyFrontPet;
        enemyTurn.TargetPet = AllyFrontPet;
        enemyTurn.RemainingHealth = ApplyAllyDamage(enemyDamage);
        enemyTurn.TargetDied = enemyTurn.RemainingHealth == 0;
        enemyTurn.RequiresFrontPet = enemyTurn.TargetDied && WaitingForAllyFrontPet;
        enemyTurn.HasFinalRound = IsFinished();
        enemyTurn.Winner = Winner;

        FinishAcceptedRound(trapTurn);
        return true;
    }

    uint8 GetTrapStatus(bool canStoreBattlePet = true, bool enemyTameable = true) const
    {
        if (!IsActive() || !enemyTameable)
            return PET_BATTLE_TRAP_STATUS_NOT_CAPTURABLE;

        if (Captured)
            return PET_BATTLE_TRAP_STATUS_ALREADY_TRAPPED;

        if (!AllyHealth || !EnemyHealth)
            return PET_BATTLE_TRAP_STATUS_DEAD_PET;

        if (!canStoreBattlePet)
            return PET_BATTLE_TRAP_STATUS_JOURNAL_FULL;

        if (!IsEnemyHealthTrapEligible())
            return PET_BATTLE_TRAP_STATUS_HEALTH_TOO_HIGH;

        return PET_BATTLE_TRAP_STATUS_ACTIVE;
    }

    void AdvanceRound()
    {
        if (IsActive() && !Finished)
            ++RoundID;
    }

    void Finish(uint8 winner)
    {
        Finished = true;
        Winner = winner;
    }

    bool SetAllyPet(uint8 petIndex, uint64 petId, uint32 maxHealth, uint32 health)
    {
        if (!IsValidAllyPetIndex(petIndex) || !petId || !maxHealth)
            return false;

        AllyTeam[petIndex].PetID = petId;
        AllyTeam[petIndex].MaxHealth = maxHealth;
        AllyTeam[petIndex].Health = NormalizeHealth(health, maxHealth);

        if (petIndex == AllyFrontPet)
            SyncActiveAllyPet();

        return true;
    }

    bool HasAllyPet(uint8 petIndex) const
    {
        return IsValidAllyPetIndex(petIndex) && AllyTeam[petIndex].PetID != 0;
    }

    bool IsAllyPetAlive(uint8 petIndex) const
    {
        return HasAllyPet(petIndex) && AllyTeam[petIndex].Health != 0;
    }

    bool GetAllyPetHealth(uint64 petId, uint32& health) const
    {
        if (!petId)
            return false;

        for (uint8 i = 0; i < BATTLE_PET_MAX_ACTIVE_TEAM_PETS; ++i)
        {
            if (AllyTeam[i].PetID != petId)
                continue;

            health = AllyTeam[i].Health;
            return true;
        }

        return false;
    }

    uint16 GetAllyAbilityCooldown(uint8 petIndex, uint8 abilitySlot) const
    {
        if (!IsValidAllyPetIndex(petIndex) || !IsValidAbilitySlot(abilitySlot))
            return 0;

        return AllyCooldowns[petIndex][abilitySlot].Cooldown;
    }

    uint64 EnemyGUID = 0;
    uint64 AllyPetID = 0;
    uint64 EnemyPetID = 0;
    uint16 EnemySpecies = 0;
    uint32 RoundID = 0;
    uint32 AllyMaxHealth = 0;
    uint32 AllyHealth = 0;
    uint32 EnemyMaxHealth = 0;
    uint32 EnemyHealth = 0;
    bool Finished = false;
    bool Captured = false;
    bool PvPBattle = false;
    BattlePetAchievementSource Source = BATTLE_PET_ACHIEVEMENT_SOURCE_WILD;
    bool RewardsApplied = false;
    bool AchievementsApplied = false;
    uint8 EnemyLevel = 0;
    uint8 EnemyQuality = 0;
    uint8 EnemyBreed = 0;
    uint8 Winner = PET_BATTLE_WINNER_NONE;
    uint8 AllyFrontPet = 0;
    uint8 EnemyFrontPet = 3;
    uint8 TrapFailedAttempts = 0;
    bool WaitingForAllyFrontPet = false;
    ActivePetBattlePetState AllyTeam[BATTLE_PET_MAX_ACTIVE_TEAM_PETS];
    ActivePetBattleWeather Weather;

private:
    ActivePetBattleCooldown AllyCooldowns[BATTLE_PET_MAX_ACTIVE_TEAM_PETS][BATTLE_PET_ABILITY_SLOT_COUNT];
    ActivePetBattleAuraState AllyAura;
    ActivePetBattleAuraState EnemyAura;

    bool CanAcceptRound(uint32 roundId) const
    {
        return IsActive() && !Finished && roundId == RoundID;
    }

    ActivePetBattleTurn CreateTurn(uint32 roundId) const
    {
        ActivePetBattleTurn turn;
        turn.RoundID = roundId;
        turn.Winner = Winner;
        return turn;
    }

    static uint32 NormalizeHealth(uint32 health, uint32 maxHealth)
    {
        return maxHealth && health > maxHealth ? maxHealth : health;
    }

    bool IsEnemyHealthTrapEligible() const
    {
        if (!EnemyMaxHealth)
            return false;

        return uint64(EnemyHealth) * 100 <= uint64(EnemyMaxHealth) * 35;
    }

    static bool IsValidAllyPetIndex(uint8 petIndex)
    {
        return petIndex < BATTLE_PET_MAX_ACTIVE_TEAM_PETS;
    }

    static bool IsValidAbilitySlot(uint8 abilitySlot)
    {
        return abilitySlot < BATTLE_PET_ABILITY_SLOT_COUNT;
    }

    bool IsAllyAbilityOnCooldown(uint8 petIndex, uint8 abilitySlot) const
    {
        if (!IsValidAllyPetIndex(petIndex) || !IsValidAbilitySlot(abilitySlot))
            return false;

        ActivePetBattleCooldown const& cooldownState = AllyCooldowns[petIndex][abilitySlot];
        return cooldownState.OnCooldown && cooldownState.Cooldown != 0 && cooldownState.Lockdown == 0;
    }

    bool IsContinuingAllyAbilityChannel(uint8 petIndex, uint8 abilitySlot, uint32 abilityId) const
    {
        if (!IsValidAllyPetIndex(petIndex) || !IsValidAbilitySlot(abilitySlot) || !abilityId)
            return false;

        ActivePetBattleCooldown const& cooldownState = AllyCooldowns[petIndex][abilitySlot];
        return cooldownState.OnCooldown
            && cooldownState.AbilityID == abilityId
            && cooldownState.NextTurnIndex > 0;
    }

    void StartAllyAbilityCooldown(uint8 petIndex, uint8 abilitySlot, uint32 abilityId, uint16 cooldown,
        uint16 lockdown = 0, uint8 nextTurnIndex = 0)
    {
        if (!IsValidAllyPetIndex(petIndex) || !IsValidAbilitySlot(abilitySlot) || !abilityId)
            return;

        ActivePetBattleCooldown& cooldownState = AllyCooldowns[petIndex][abilitySlot];
        bool const continuingChannel = cooldownState.OnCooldown
            && cooldownState.AbilityID == abilityId
            && (cooldownState.NextTurnIndex > 0 || nextTurnIndex > 0 || lockdown > 0);

        if (!cooldown && !lockdown && !nextTurnIndex && !continuingChannel)
            return;

        cooldownState.PetPBOID = petIndex;
        cooldownState.AbilitySlot = abilitySlot;
        cooldownState.AbilityID = abilityId;
        if (!continuingChannel || !cooldownState.Cooldown)
            cooldownState.Cooldown = cooldown;
        // Lockdown remains server-side channel bookkeeping; packets force it to 0 for the client.
        cooldownState.Lockdown = lockdown;
        cooldownState.NextTurnIndex = nextTurnIndex;
        cooldownState.OnCooldown = true;
    }

    void SnapshotAllyCooldowns(std::vector<ActivePetBattleCooldown>& cooldowns) const
    {
        for (uint8 petIndex = 0; petIndex < BATTLE_PET_MAX_ACTIVE_TEAM_PETS; ++petIndex)
        {
            for (uint8 abilitySlot = 0; abilitySlot < BATTLE_PET_ABILITY_SLOT_COUNT; ++abilitySlot)
            {
                ActivePetBattleCooldown const& cooldownState = AllyCooldowns[petIndex][abilitySlot];
                if (cooldownState.OnCooldown)
                    cooldowns.push_back(cooldownState);
            }
        }
    }

    void ClearExpiredAllyCooldowns()
    {
        for (uint8 petIndex = 0; petIndex < BATTLE_PET_MAX_ACTIVE_TEAM_PETS; ++petIndex)
        {
            for (uint8 abilitySlot = 0; abilitySlot < BATTLE_PET_ABILITY_SLOT_COUNT; ++abilitySlot)
            {
                ActivePetBattleCooldown& cooldownState = AllyCooldowns[petIndex][abilitySlot];
                if (cooldownState.OnCooldown && !cooldownState.Cooldown && !cooldownState.Lockdown
                    && !cooldownState.NextTurnIndex)
                    cooldownState = ActivePetBattleCooldown();
            }
        }
    }

    void DecrementAllyCooldowns()
    {
        for (uint8 petIndex = 0; petIndex < BATTLE_PET_MAX_ACTIVE_TEAM_PETS; ++petIndex)
        {
            for (uint8 abilitySlot = 0; abilitySlot < BATTLE_PET_ABILITY_SLOT_COUNT; ++abilitySlot)
            {
                ActivePetBattleCooldown& cooldownState = AllyCooldowns[petIndex][abilitySlot];
                if (!cooldownState.OnCooldown)
                    continue;

                // Lockdown is managed explicitly by the channel executor; only tick cooldown here.
                if (cooldownState.Cooldown)
                    --cooldownState.Cooldown;
            }
        }
    }

    static void ActivateIncomingDamageReduction(ActivePetBattleAuraState& aura, uint32 amount, uint8 rounds)
    {
        if (!amount || !rounds)
            return;

        aura.IncomingDamageReduction = amount;
        aura.Rounds = rounds;
    }

    static void ActivateCombatAura(ActivePetBattleAuraState& aura, uint32 auraAbilityId, uint8 rounds, bool untargetable)
    {
        if (!auraAbilityId)
            return;

        aura.AuraAbilityId = auraAbilityId;
        aura.Untargetable = untargetable;
        if (rounds)
            aura.Rounds = rounds;
        else if (untargetable)
            aura.Rounds = 1;
    }

    static void ClearCombatAura(ActivePetBattleAuraState& aura, uint32 auraAbilityId)
    {
        if (auraAbilityId && aura.AuraAbilityId && aura.AuraAbilityId != auraAbilityId)
            return;

        bool const keepReduction = aura.IncomingDamageReduction != 0 && aura.Rounds != 0
            && (!auraAbilityId || aura.AuraAbilityId != auraAbilityId);
        if (keepReduction)
        {
            aura.AuraAbilityId = 0;
            aura.Untargetable = false;
            return;
        }

        aura = ActivePetBattleAuraState();
    }

    static uint32 ApplyIncomingDamageReduction(ActivePetBattleAuraState const& aura, uint32 damage)
    {
        if (!damage || !aura.Rounds || !aura.IncomingDamageReduction)
            return damage;

        return damage > aura.IncomingDamageReduction ? damage - aura.IncomingDamageReduction : 0;
    }

    static void DecrementAura(ActivePetBattleAuraState& aura)
    {
        if (!aura.Rounds)
            return;

        --aura.Rounds;
        if (!aura.Rounds)
        {
            // Keep untargetable channel auras until explicitly removed by effect props.
            if (aura.Untargetable && aura.AuraAbilityId)
                aura.Rounds = 1;
            else
                aura = ActivePetBattleAuraState();
        }
    }

    void DecrementWeather()
    {
        if (!Weather.Rounds)
            return;

        --Weather.Rounds;
        if (!Weather.Rounds)
            Weather = ActivePetBattleWeather();
    }

    void DecrementAuras()
    {
        DecrementAura(AllyAura);
        DecrementAura(EnemyAura);
        DecrementWeather();
    }

    void FinishAcceptedRound(ActivePetBattleTurn& turn)
    {
        SnapshotAllyCooldowns(turn.Cooldowns);
        ClearExpiredAllyCooldowns();
        DecrementAllyCooldowns();
        DecrementAuras();
        AdvanceRound();
    }

    bool HasAliveAllyPetOtherThan(uint8 petIndex) const
    {
        for (uint8 i = 0; i < BATTLE_PET_MAX_ACTIVE_TEAM_PETS; ++i)
            if (i != petIndex && IsAllyPetAlive(i))
                return true;

        return false;
    }

    void SyncActiveAllyPet()
    {
        if (!HasAllyPet(AllyFrontPet))
        {
            AllyPetID = 0;
            AllyMaxHealth = 0;
            AllyHealth = 0;
            return;
        }

        ActivePetBattlePetState const& activePet = AllyTeam[AllyFrontPet];
        AllyPetID = activePet.PetID;
        AllyMaxHealth = activePet.MaxHealth;
        AllyHealth = activePet.Health;
    }

    uint32 ApplyDamage(uint32& health, uint32 damage, uint8 winner)
    {
        if (!IsActive() || Finished)
            return health;

        if (damage >= health)
        {
            health = 0;
            Finish(winner);
        }
        else
            health -= damage;

        return health;
    }
};

#endif // BATTLE_PET_BATTLE_STATE_H
