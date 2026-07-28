/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef BattlePetPackets_h__
#define BattlePetPackets_h__

#include "Common.h"
#include "WorldPacket.h"
#include "G3D/Vector3.h"

#include <string>
#include <vector>

class BattlePet;
struct ActivePetBattleTurn;

namespace Skyfire::BattlePetPackets
{
    struct InitialUpdatePetData
    {
        uint64 Id = 0;
        uint16 Level = 0;
        uint16 Quality = 0;
        uint32 Power = 0;
        uint32 MaxHealth = 0;
        uint32 Speed = 0;
        uint32 CurrentHealth = 0;
        uint16 Xp = 0;
        uint16 Flags = 0;
        std::string Nickname;
        uint32 Species = 0;
        uint32 Abilities[3] = { };
    };

    uint8 const INITIAL_UPDATE_MAX_TEAM_PETS = 3;

    struct InitialUpdateTeamData
    {
        InitialUpdatePetData Pets[INITIAL_UPDATE_MAX_TEAM_PETS];
        ObjectGuid OwnerGuid = 0;
        uint32 TrapAbility = 0;
        uint8 PetCount = 0;
        uint8 FrontPet = 0;
        uint8 TrapStatus = 0;
    };

    struct BattlePetInput
    {
        bool IgnoreAbandonPenalty = false;
        bool HasAbilityID = false;
        bool HasRoundID = false;
        bool HasMoveType = false;
        bool HasNewFrontPet = false;
        bool HasBattleInterrupted = false;
        bool HasDebugFlags = false;
        uint32 AbilityID = 0;
        uint32 RoundID = 0;
        uint8 MoveType = 0;
        int8 NewFrontPet = -1;
        uint8 BattleInterrupted = 0;
        uint8 DebugFlags = 0;
    };

    enum BattlePetInputMoveType
    {
        BATTLE_PET_INPUT_MOVE_NONE = 0,
        BATTLE_PET_INPUT_MOVE_ABILITY = 1,
        BATTLE_PET_INPUT_MOVE_SWAP = 2,
        BATTLE_PET_INPUT_MOVE_TRAP = 3,
        BATTLE_PET_INPUT_MOVE_FORFEIT = 4
    };

    enum BattlePetInputAction
    {
        BATTLE_PET_INPUT_ACTION_NONE = 0,
        BATTLE_PET_INPUT_ACTION_ABILITY = 1,
        BATTLE_PET_INPUT_ACTION_SWAP = 2,
        BATTLE_PET_INPUT_ACTION_FORFEIT = 3,
        BATTLE_PET_INPUT_ACTION_TRAP = 4
    };

    struct BattlePetInputCommand
    {
        uint8 Action = BATTLE_PET_INPUT_ACTION_NONE;
        bool HasRoundID = false;
        uint32 RoundID = 0;
        uint32 AbilityID = 0;
        uint8 MoveType = BATTLE_PET_INPUT_MOVE_NONE;
        int8 NewFrontPet = -1;
        bool IgnoreAbandonPenalty = false;
        uint8 BattleInterrupted = 0;
    };

    struct BattlePetFirstPetSelection
    {
        uint8 PetID = 0;
    };

    struct BattlePetRequestUpdate
    {
        ObjectGuid Guid = 0;
        bool Cancelled = false;
    };

    struct BattlePetRoundEffectTarget
    {
        int8 PetX = -1;
        uint8 Type = 0;
        bool HasRemainingHealth = false;
        int32 RemainingHealth = 0;
        bool HasStatus = false;
        uint32 Status = 0;
        bool HasStateID = false;
        uint32 StateID = 0;
        bool HasStateValue = false;
        uint32 StateValue = 0;
        bool HasAuraInstanceID = false;
        uint16 AuraInstanceID = 0;
        bool HasAuraAbilityID = false;
        uint32 AuraAbilityID = 0;
        bool HasRoundsRemaining = false;
        uint32 RoundsRemaining = 0;
        bool HasCurrentRound = false;
        uint32 CurrentRound = 0;
    };

    struct BattlePetRoundEffect
    {
        int8 CasterPBOID = -1;
        uint8 PetBattleEffectType = 0;
        uint16 Flags = 0;
        uint16 TurnInstanceID = 0;
        uint8 StackDepth = 0;
        uint32 AbilityEffectID = 0;
        uint16 SourceAuraInstanceID = 0;
        std::vector<BattlePetRoundEffectTarget> Targets;
    };

    struct BattlePetRoundCooldown
    {
        uint32 AbilityID = 0;
        uint16 Cooldown = 0;
        uint16 Lockdown = 0;
        uint8 AbilitySlot = 0;
        uint8 PetPBOID = 0;
        bool HasPetPBOID = true;
    };

    struct BattlePetRoundResult
    {
        uint32 RoundID = 0;
        uint8 NextPetBattleState = 2;
        uint8 InputFlags[2] = { };
        uint8 TrapStatus[2] = { 2, 2 };
        std::vector<BattlePetRoundEffect> Effects;
        std::vector<uint8> DeadPets;
        std::vector<BattlePetRoundCooldown> Cooldowns;
    };

    enum BattlePetRoundInputFlag
    {
        BATTLE_PET_ROUND_INPUT_FLAG_NONE = 0,
        BATTLE_PET_ROUND_INPUT_FLAG_LOCK_ABILITY_1 = 0x01,
        BATTLE_PET_ROUND_INPUT_FLAG_LOCK_ABILITY_2 = 0x02,
        BATTLE_PET_ROUND_INPUT_FLAG_LOCK_PET_SWAP = 0x04,
        BATTLE_PET_ROUND_INPUT_FLAG_SELECT_NEW_FRONT_PET = 0x08
    };

    struct BattlePetFinalRoundPet
    {
        uint8 Pboid = 0;
        uint32 RemainingHealth = 0;
        uint16 InitialLevel = 0;
        uint16 NewLevel = 0;
        uint16 Xp = 0;
        uint32 MaxHealth = 0;
        bool SeenAction = false;
        bool Captured = false;
        bool Caged = false;
        bool AwardedXP = false;
    };

    struct BattlePetFinalRound
    {
        bool PvPBattle = false;
        bool Winners[2] = { };
        bool Abandoned = false;
        uint32 NpcCreatureID[2] = { };
        std::vector<BattlePetFinalRoundPet> Pets;
    };

    InitialUpdatePetData BuildInitialUpdatePetData(BattlePet const& battlePet);
    InitialUpdatePetData BuildWildInitialUpdatePetData(uint64 id, uint16 speciesId, uint8 level, uint8 quality, uint8 breed);
    BattlePetInput ReadBattlePetInput(WorldPacket& data);
    BattlePetInputCommand BuildBattlePetInputCommand(BattlePetInput const& input);
    BattlePetFirstPetSelection ReadBattlePetFirstPetSelection(WorldPacket& data);
    BattlePetRequestUpdate ReadBattlePetRequestUpdate(WorldPacket& data);
    BattlePetRoundEffect BuildPetSwapEffect(uint8 casterPet, uint8 targetPet);
    BattlePetRoundEffect BuildDamageEffect(uint8 casterPet, uint8 targetPet, int32 remainingHealth,
        uint32 abilityEffectId, uint16 turnInstanceId = 1);
    BattlePetRoundEffect BuildAuraEffect(uint8 casterPet, uint8 targetPet, uint32 abilityEffectId,
        uint32 auraAbilityId, uint32 roundsRemaining, uint16 auraInstanceId, bool applying,
        uint16 turnInstanceId = 1);
    BattlePetRoundEffect BuildAbilityVisualEffect(uint8 casterPet, uint8 targetPet, uint32 abilityEffectId,
        uint16 turnInstanceId = 1);
    BattlePetRoundEffect BuildMissEffect(uint8 casterPet, uint8 targetPet, uint32 abilityEffectId,
        int32 remainingHealth, uint16 turnInstanceId = 1);
    BattlePetRoundEffect BuildCatchEffect(uint8 casterPet, uint8 targetPet, uint32 abilityEffectId,
        bool captured, uint16 turnInstanceId = 1);
    BattlePetRoundResult BuildDamageRoundResult(uint32 roundId, uint8 casterPet, uint8 targetPet,
        int32 remainingHealth, uint32 abilityEffectId, bool targetDied);
    BattlePetRoundResult BuildRoundResultFromTurn(ActivePetBattleTurn const& turn, uint32 abilityEffectId);
    void AppendRoundCooldowns(BattlePetRoundResult& round, ActivePetBattleTurn const& turn);
    void AppendChannelInputFlags(BattlePetRoundResult& round, uint8 abilitySlot);
    void MarkRoundResultAsCatchOrKill(BattlePetRoundResult& round);
    BattlePetFinalRound BuildFinalRoundState(bool allyWon, bool abandoned,
        std::vector<BattlePetFinalRoundPet> const& pets);
    BattlePetFinalRound BuildFinalRoundState(bool allyWon, bool abandoned,
        BattlePetFinalRoundPet const& allyPet, BattlePetFinalRoundPet const& enemyPet);
    WorldPacket BuildInitialUpdatePacket(ObjectGuid wildBattlePetGuid, InitialUpdateTeamData const* teams,
        uint8 teamCount, bool isPvP = false);
    WorldPacket BuildInitialUpdatePacket(ObjectGuid wildBattlePetGuid, InitialUpdatePetData const* teamPets,
        uint8 teamPetCount, bool isPvP = false);
    WorldPacket BuildInitialUpdatePacket(ObjectGuid wildBattlePetGuid, InitialUpdatePetData const& activePet,
        bool isPvP = false);
    WorldPacket BuildFirstRoundPacket(uint32 roundId, uint8 allyFrontPet, uint8 enemyFrontPet,
        uint8 allyTrapStatus = 2, uint8 enemyTrapStatus = 2);
    WorldPacket BuildFinishedPacket();
    float CalculateBattlePetFacing(G3D::Vector3 const& origin, G3D::Vector3 const* positions);
    float ResolveBattlePetFacing(bool hasClientOrientation, float clientOrientation,
        G3D::Vector3 const& origin, G3D::Vector3 const* positions);
    WorldPacket BuildRoundResultPacket(BattlePetRoundResult const& round);
    WorldPacket BuildFinalRoundPacket(BattlePetFinalRound const& finalRound);
    WorldPacket BuildFinalizeLocationPacket(G3D::Vector3 const& origin, G3D::Vector3 const* positions,
        bool hasOrientation, float orientation, bool hasLocationResult, uint32 locationResult);
    WorldPacket BuildPvpChallengePacket(ObjectGuid challengerGuid, G3D::Vector3 const& origin, G3D::Vector3 const* positions,
        bool hasOrientation, float orientation, bool hasLocationResult, uint32 locationResult);
    enum PetBattleQueueStatus
    {
        PET_BATTLE_QUEUE_STATUS_NONE = 0,
        PET_BATTLE_QUEUE_STATUS_QUEUED = 1,
        PET_BATTLE_QUEUE_STATUS_ALREADY_QUEUED = 2,
        PET_BATTLE_QUEUE_STATUS_JOIN_FAILED = 3,
        PET_BATTLE_QUEUE_STATUS_JOURNAL_LOCK = 4,
        PET_BATTLE_QUEUE_STATUS_REMOVED = 5,
        PET_BATTLE_QUEUE_STATUS_PROPOSAL_DECLINED = 6,
        PET_BATTLE_QUEUE_STATUS_PROPOSAL_TIMEOUT = 7,
        PET_BATTLE_QUEUE_STATUS_OPPONENT_DECLINED = 8,
        PET_BATTLE_QUEUE_STATUS_REQUEUED_INTERNAL = 9,
        PET_BATTLE_QUEUE_STATUS_REQUEUED_REMOVED = 10,
        PET_BATTLE_QUEUE_STATUS_SLOT_LOCKED = 11,
        PET_BATTLE_QUEUE_STATUS_SLOT_EMPTY = 12,
        PET_BATTLE_QUEUE_STATUS_SLOT_NO_TRACKER = 13,
        PET_BATTLE_QUEUE_STATUS_SLOT_NO_SPECIES = 14,
        PET_BATTLE_QUEUE_STATUS_SLOT_CANT_BATTLE = 15,
        PET_BATTLE_QUEUE_STATUS_SLOT_REVOKED = 16,
        PET_BATTLE_QUEUE_STATUS_SLOT_DEAD = 17,
        PET_BATTLE_QUEUE_STATUS_SLOT_NO_PET = 18,
        PET_BATTLE_QUEUE_STATUS_NOT_WHILE_NEUTRAL = 19
    };

    uint8 const PET_BATTLE_QUEUE_SLOT_COUNT = 3;

    WorldPacket BuildQueueStatusPacket(ObjectGuid requesterGuid, uint32 status,
        uint32 const* slotResults = NULL);
    WorldPacket BuildQueueProposeMatchPacket();
    WorldPacket BuildRequestFailedPacket(uint8 reason);
}

#endif // BattlePetPackets_h__
