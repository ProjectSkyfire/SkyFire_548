/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "BattlePet.h"
#include "BattlePetMgr.h"
#include "BattlePetPackets.h"
#include "BattlePetPvpState.h"
#include "Common.h"
#include "Creature.h"
#include "DB2Enums.h"
#include "DB2Stores.h"
#include "Item.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "TemporarySummon.h"
#include "WorldPacket.h"
#include "WorldSession.h"

#include <cmath>
#include <ctime>

namespace
{
    uint32 const PetBattlePvpProposalTimeout = 60;
    uint32 const PetBattlePvpChallengeTimeout = 60;
    PetBattlePvpStateStore& PetBattlePvpState = GetPetBattlePvpStateStore();

    bool ValidatePetBattleQueueRequest(Player& player);
    bool ValidatePetBattlePvpDuelTarget(Player& challenger, Player* target);

    void SendPetBattleDuelPopup(Player& challenger, Player& target)
    {
        ObjectGuid arbiterGuid = challenger.GetGUID();
        ObjectGuid casterGuid = challenger.GetGUID();

        WorldPacket data(SMSG_DUEL_REQUESTED, 8 + 8);
        data.WriteGuidMask(arbiterGuid, 5);
        data.WriteGuidMask(casterGuid, 4, 2, 7);
        data.WriteGuidMask(arbiterGuid, 0);
        data.WriteGuidMask(casterGuid, 5);
        data.WriteGuidMask(arbiterGuid, 4, 6);
        data.WriteGuidMask(casterGuid, 1, 3, 6);
        data.WriteGuidMask(arbiterGuid, 7, 3, 2, 1);
        data.WriteGuidMask(casterGuid, 0);

        data.WriteGuidBytes(arbiterGuid, 5, 3);
        data.WriteGuidBytes(casterGuid, 7, 4);
        data.WriteGuidBytes(arbiterGuid, 7);
        data.WriteGuidBytes(casterGuid, 3, 6, 0);
        data.WriteGuidBytes(arbiterGuid, 4);
        data.WriteGuidBytes(casterGuid, 2, 1);
        data.WriteGuidBytes(arbiterGuid, 0, 2, 6, 1);
        data.WriteGuidBytes(casterGuid, 5);

        target.SendDirectMessage(&data);
    }

    bool BuildWildInitialUpdatePetData(Creature const& wildBattlePet,
        Skyfire::BattlePetPackets::InitialUpdatePetData& wildPetData, uint8& wildPetBreed)
    {
        uint16 const speciesId = BattlePetSpeciesIdByNpcId(wildBattlePet.GetEntry());
        if (!speciesId)
            return false;

        wildPetBreed = sObjectMgr->BattlePetGetRandomBreed(speciesId);
        wildPetData = Skyfire::BattlePetPackets::BuildWildInitialUpdatePetData(
            uint64(wildBattlePet.GetGUID()), speciesId, BattlePetNormalizeWildLevel(wildBattlePet.getLevel()),
            sObjectMgr->BattlePetGetRandomQuality(speciesId), wildPetBreed);
        return true;
    }

    uint32 BattlePetInputDamageForEnemyAbility(uint32 abilityId, ActivePetBattle const& activeBattle)
    {
        if (!abilityId)
            return 0;

        uint16 const power = BattlePetPowerFromBattleState(activeBattle.EnemySpecies,
            activeBattle.EnemyLevel, activeBattle.EnemyQuality, activeBattle.EnemyBreed);
        return BattlePetDamageFromStats(abilityId, power, activeBattle.EnemyLevel);
    }

    uint32 BattlePetInputIncomingDamageReductionForEnemyAbility(uint32 abilityId, ActivePetBattle const& activeBattle)
    {
        if (!abilityId)
            return 0;

        uint16 const power = BattlePetPowerFromBattleState(activeBattle.EnemySpecies,
            activeBattle.EnemyLevel, activeBattle.EnemyQuality, activeBattle.EnemyBreed);
        return BattlePetIncomingDamageReductionFromStats(abilityId, power, activeBattle.EnemyLevel);
    }

    BattlePet const* GetActiveBattlePet(BattlePetMgr const& battlePetMgr)
    {
        ActivePetBattle const& activeBattle = battlePetMgr.GetActivePetBattle();
        return battlePetMgr.GetBattlePet(activeBattle.AllyPetID);
    }

    BattlePet const* GetPvpOpponentActiveBattlePet(Player const& player, ActivePetBattle const& activeBattle)
    {
        if (!activeBattle.IsPvP())
            return NULL;

        Player* opponent = ObjectAccessor::FindPlayer(activeBattle.EnemyGUID);
        if (!opponent)
            return NULL;

        BattlePetMgr* opponentBattlePetMgr = opponent->GetBattlePetMgr();
        if (!opponentBattlePetMgr || !opponentBattlePetMgr->HasActivePetBattle())
            return NULL;

        ActivePetBattle const& opponentBattle = opponentBattlePetMgr->GetActivePetBattle();
        if (!opponentBattle.IsPvP() || opponentBattle.EnemyGUID != player.GetGUID())
            return NULL;

        return GetActiveBattlePet(*opponentBattlePetMgr);
    }

    uint8 BattlePetActiveAbilitySlot(BattlePet const* battlePet, uint32 abilityId)
    {
        if (!battlePet || !abilityId)
            return BATTLE_PET_ABILITY_SLOT_INVALID;

        for (uint8 slot = 0; slot < BATTLE_PET_ABILITY_SLOT_COUNT; ++slot)
            if (BattlePetGetSpeciesAbility(battlePet->GetSpecies(), slot, battlePet->GetFlags(), battlePet->GetLevel()) == abilityId)
                return slot;

        return BATTLE_PET_ABILITY_SLOT_INVALID;
    }

    void SendBattlePetRoundResult(Player& player, Skyfire::BattlePetPackets::BattlePetRoundResult const& round)
    {
        WorldPacket packet = Skyfire::BattlePetPackets::BuildRoundResultPacket(round);
        player.SendDirectMessage(&packet);
    }

    void SendBattlePetFinalRound(Player& player, Skyfire::BattlePetPackets::BattlePetFinalRound const& finalRound)
    {
        WorldPacket finalRoundPacket = Skyfire::BattlePetPackets::BuildFinalRoundPacket(finalRound);
        player.SendDirectMessage(&finalRoundPacket);
    }

    void SendBattlePetFinished(Player& player)
    {
        WorldPacket finishedPacket = Skyfire::BattlePetPackets::BuildFinishedPacket();
        player.SendDirectMessage(&finishedPacket);
    }

    void ClearActivePetBattleAndSave(Player& player)
    {
        BattlePetMgr* battlePetMgr = player.GetBattlePetMgr();
        battlePetMgr->ClearActivePetBattle();
        battlePetMgr->SaveToDb();
    }

    uint8 OpponentWinnerForFinalRound(Skyfire::BattlePetPackets::BattlePetFinalRound const& finalRound)
    {
        if (finalRound.Winners[0])
            return PET_BATTLE_WINNER_ENEMY;

        if (finalRound.Winners[1])
            return PET_BATTLE_WINNER_ALLY;

        return PET_BATTLE_WINNER_NONE;
    }

    void FinishActivePvpPetBattleForOpponent(Player& player, uint64 opponentGuid, uint8 opponentWinner, bool abandoned)
    {
        if (!opponentGuid)
            return;

        Player* opponent = ObjectAccessor::FindPlayer(opponentGuid);
        if (!opponent)
            return;

        BattlePetMgr* opponentBattlePetMgr = opponent->GetBattlePetMgr();
        if (!opponentBattlePetMgr->HasActivePetBattle())
            return;

        ActivePetBattle const& opponentBattle = opponentBattlePetMgr->GetActivePetBattle();
        if (!opponentBattle.IsPvP() || opponentBattle.EnemyGUID != player.GetGUID())
            return;

        if (opponentWinner)
            opponentBattlePetMgr->FinishActivePetBattle(opponentWinner);

        Skyfire::BattlePetPackets::BattlePetFinalRound opponentFinalRound;
        if (opponentBattlePetMgr->BuildActivePetBattleFinalRound(abandoned, opponentFinalRound))
            SendBattlePetFinalRound(*opponent, opponentFinalRound);

        ClearActivePetBattleAndSave(*opponent);
    }

    void FinishActivePvpPetBattle(Player& player, uint64 opponentGuid,
        Skyfire::BattlePetPackets::BattlePetFinalRound const& playerFinalRound,
        uint8 opponentWinner, bool opponentAbandoned)
    {
        SendBattlePetFinalRound(player, playerFinalRound);
        FinishActivePvpPetBattleForOpponent(player, opponentGuid, opponentWinner, opponentAbandoned);
        ClearActivePetBattleAndSave(player);
    }

    void FinishActivePvpPetBattleAfterForfeit(Player& loser, uint64 opponentGuid,
        Skyfire::BattlePetPackets::BattlePetFinalRound const& loserFinalRound)
    {
        FinishActivePvpPetBattle(loser, opponentGuid, loserFinalRound, PET_BATTLE_WINNER_ALLY, true);
    }

    uint32 PetBattleQueueSlotResult(Player& player, uint8 slot)
    {
        BattlePetMgr* battlePetMgr = player.GetBattlePetMgr();
        if (!battlePetMgr->HasLoadoutSlot(slot))
            return Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_QUEUED;

        uint64 const petId = battlePetMgr->GetLoadoutSlot(slot);
        if (!petId)
            return Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_QUEUED;

        BattlePet* battlePet = battlePetMgr->GetBattlePet(petId);
        if (!battlePet)
            return Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_SLOT_NO_PET;

        if (!battlePet->GetSpecies())
            return Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_SLOT_NO_SPECIES;

        if (!battlePet->GetCurrentHealth())
            return Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_SLOT_DEAD;

        return Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_QUEUED;
    }

    void BuildPetBattleQueueSlotResults(Player& player,
        uint32 (&slotResults)[Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_SLOT_COUNT])
    {
        for (uint8 slot = 0; slot < Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_SLOT_COUNT; slot++)
            slotResults[slot] = PetBattleQueueSlotResult(player, slot);
    }

    void SendPetBattleQueueStatus(WorldSession& session, Player& player, uint32 status)
    {
        uint32 slotResults[Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_SLOT_COUNT] = { };
        BuildPetBattleQueueSlotResults(player, slotResults);

        WorldPacket data = Skyfire::BattlePetPackets::BuildQueueStatusPacket(player.GetGUID(), status, slotResults);
        session.SendPacket(&data);
    }

    void SendPetBattleQueueStatus(Player& player, uint32 status)
    {
        if (WorldSession* session = player.GetSession())
            SendPetBattleQueueStatus(*session, player, status);
    }

    void SendPetBattleQueueProposeMatch(Player& player)
    {
        if (WorldSession* session = player.GetSession())
        {
            WorldPacket data = Skyfire::BattlePetPackets::BuildQueueProposeMatchPacket();
            session->SendPacket(&data);
        }
    }

    void SendPetBattlePvpChallengeFailed(uint64 playerGuid)
    {
        if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
            player->GetBattlePetMgr()->SendPetBattleRequestFailed(PET_BATTLE_REQUEST_FAILED_INVALID_TARGET);
    }

    bool IsPetBattlePvpChallengeExpired(PetBattlePvpChallenge const& challenge)
    {
        return challenge.CreatedAt && time(NULL) - challenge.CreatedAt >= PetBattlePvpChallengeTimeout;
    }

    void ClearPetBattlePvpChallengeForTarget(uint64 targetGuid, bool notifyPlayers = false,
        uint64 exceptGuid = 0)
    {
        PetBattlePvpStateStore::ChallengeMap::iterator challengeItr =
            PetBattlePvpState.ChallengesByTarget().find(targetGuid);
        if (challengeItr == PetBattlePvpState.ChallengesByTarget().end())
            return;

        PetBattlePvpChallenge const challenge = challengeItr->second;
        PetBattlePvpState.EraseChallengeForTarget(targetGuid);

        if (!notifyPlayers)
            return;

        if (challenge.ChallengerGuid != exceptGuid)
            SendPetBattlePvpChallengeFailed(challenge.ChallengerGuid);

        if (challenge.TargetGuid != exceptGuid)
            SendPetBattlePvpChallengeFailed(challenge.TargetGuid);
    }

    void ClearPetBattlePvpChallengesFor(uint64 playerGuid, bool notifyPlayers = false,
        uint64 exceptGuid = 0)
    {
        ClearPetBattlePvpChallengeForTarget(playerGuid, notifyPlayers, exceptGuid);

        PetBattlePvpStateStore::ChallengeTargetMap::iterator targetItr =
            PetBattlePvpState.ChallengeTargetByChallenger().find(playerGuid);
        if (targetItr == PetBattlePvpState.ChallengeTargetByChallenger().end())
            return;

        ClearPetBattlePvpChallengeForTarget(targetItr->second, notifyPlayers, exceptGuid);
    }

    PetBattlePvpStateStore::ChallengeMap::iterator FindPetBattlePvpChallengeForPlayer(uint64 playerGuid)
    {
        return PetBattlePvpState.FindChallengeForPlayer(playerGuid);
    }

    bool ExpirePetBattlePvpChallenge(Player& player)
    {
        PetBattlePvpStateStore::ChallengeMap::iterator challengeItr =
            FindPetBattlePvpChallengeForPlayer(player.GetGUID());
        if (challengeItr == PetBattlePvpState.ChallengesByTarget().end()
            || !IsPetBattlePvpChallengeExpired(challengeItr->second))
            return false;

        ClearPetBattlePvpChallengeForTarget(challengeItr->first, true);
        return true;
    }

    bool BuildPlayerPetBattleTeam(Player& player, Skyfire::BattlePetPackets::InitialUpdateTeamData& team)
    {
        team = Skyfire::BattlePetPackets::InitialUpdateTeamData();

        BattlePetMgr* battlePetMgr = player.GetBattlePetMgr();
        BattlePet* activeBattlePet = battlePetMgr->GetFirstAliveLoadoutBattlePet();
        if (!activeBattlePet)
            return false;

        for (uint8 slot = 0; slot < BATTLE_PET_MAX_LOADOUT_SLOTS
            && team.PetCount < Skyfire::BattlePetPackets::INITIAL_UPDATE_MAX_TEAM_PETS; slot++)
        {
            BattlePet* loadoutBattlePet = battlePetMgr->GetBattlePet(battlePetMgr->GetLoadoutSlot(slot));
            if (!loadoutBattlePet)
                continue;

            uint8 const petIndex = team.PetCount;
            team.Pets[petIndex] = Skyfire::BattlePetPackets::BuildInitialUpdatePetData(*loadoutBattlePet);
            team.PetCount++;

            if (loadoutBattlePet == activeBattlePet)
                team.FrontPet = petIndex;
        }

        if (!team.PetCount)
            return false;

        team.OwnerGuid = player.GetGUID();
        team.TrapStatus = PET_BATTLE_TRAP_STATUS_UNAVAILABLE;
        return true;
    }

    void SetActivePetBattleTeam(BattlePetMgr& battlePetMgr,
        Skyfire::BattlePetPackets::InitialUpdateTeamData const& team)
    {
        for (uint8 petIndex = 0; petIndex < team.PetCount; petIndex++)
            battlePetMgr.SetActivePetBattleAllyPet(petIndex, team.Pets[petIndex].Id,
                team.Pets[petIndex].MaxHealth, team.Pets[petIndex].CurrentHealth);
    }

    G3D::Vector3 CalculatePetBattlePlayerPosition(Player const& player,
        G3D::Vector3 const& ownPetPosition, G3D::Vector3 const& opponentPetPosition)
    {
        float dx = opponentPetPosition.x - ownPetPosition.x;
        float dy = opponentPetPosition.y - ownPetPosition.y;
        float length = std::sqrt(dx * dx + dy * dy);
        if (length < 0.1f)
        {
            dx = std::cos(player.GetOrientation());
            dy = std::sin(player.GetOrientation());
            length = 1.0f;
        }

        dx /= length;
        dy /= length;

        constexpr float playerPetBackstep = 2.5f;
        return G3D::Vector3(ownPetPosition.x - dx * playerPetBackstep,
            ownPetPosition.y - dy * playerPetBackstep, ownPetPosition.z);
    }

    void PlacePlayerForPetBattle(Player& player, G3D::Vector3 const& ownPetPosition,
        G3D::Vector3 const& opponentPetPosition)
    {
        G3D::Vector3 const playerPosition =
            CalculatePetBattlePlayerPosition(player, ownPetPosition, opponentPetPosition);
        float const orientation = Position::NormalizeOrientation(
            std::atan2(opponentPetPosition.y - playerPosition.y,
                opponentPetPosition.x - playerPosition.x));

        player.NearTeleportTo(playerPosition.x, playerPosition.y, playerPosition.z, orientation);
        player.GetBattlePetMgr()->ApplyActivePetBattlePlayerState(
            opponentPetPosition.x, opponentPetPosition.y);
    }

    bool StartPetBattlePvpForPlayer(Player& player, Player& opponent,
        Skyfire::BattlePetPackets::InitialUpdateTeamData const& playerTeam,
        Skyfire::BattlePetPackets::InitialUpdateTeamData const& opponentTeam,
        G3D::Vector3 const& origin, G3D::Vector3 const* positions,
        float orientation, bool hasLocationResult, uint32 locationResult,
        BattlePetAchievementSource battleSource)
    {
        if (!playerTeam.PetCount || !opponentTeam.PetCount)
            return false;

        BattlePetMgr* battlePetMgr = player.GetBattlePetMgr();
        Skyfire::BattlePetPackets::InitialUpdatePetData const& activePet =
            playerTeam.Pets[playerTeam.FrontPet];
        Skyfire::BattlePetPackets::InitialUpdatePetData const& opponentPet =
            opponentTeam.Pets[opponentTeam.FrontPet];
        uint8 const enemyFrontPet = uint8(3 + opponentTeam.FrontPet);

        PlacePlayerForPetBattle(player, positions[0], positions[1]);

        WorldPacket finalizeLocation = Skyfire::BattlePetPackets::BuildFinalizeLocationPacket(
            origin, positions, true, orientation, hasLocationResult, locationResult);
        player.SendDirectMessage(&finalizeLocation);

        battlePetMgr->StartPvpPetBattle(opponent.GetGUID(),
            activePet.Id, activePet.MaxHealth, activePet.CurrentHealth,
            opponentPet.Id, opponentPet.MaxHealth, opponentPet.CurrentHealth,
            uint16(opponentPet.Species), uint8(opponentPet.Level), uint8(opponentPet.Quality),
            playerTeam.FrontPet, enemyFrontPet, battleSource);

        SetActivePetBattleTeam(*battlePetMgr, playerTeam);
        battlePetMgr->SelectActivePetBattleFrontPet(playerTeam.FrontPet);

        Skyfire::BattlePetPackets::InitialUpdateTeamData teams[2];
        teams[0] = playerTeam;
        teams[1] = opponentTeam;
        teams[0].TrapStatus = battlePetMgr->GetActivePetBattleTrapStatus();
        teams[1].TrapStatus = PET_BATTLE_TRAP_STATUS_UNAVAILABLE;

        WorldPacket initialUpdate = Skyfire::BattlePetPackets::BuildInitialUpdatePacket(
            ObjectGuid(0), teams, 2, true);
        player.SendDirectMessage(&initialUpdate);
        return true;
    }

    bool StartPetBattlePvp(Player& player, Player& opponent, G3D::Vector3 const& origin,
        G3D::Vector3 const* positions, float orientation, bool hasLocationResult, uint32 locationResult,
        BattlePetAchievementSource battleSource = BATTLE_PET_ACHIEVEMENT_SOURCE_PVP_DUEL)
    {
        if (!ValidatePetBattleQueueRequest(player) || !ValidatePetBattlePvpDuelTarget(player, &opponent))
            return false;

        Skyfire::BattlePetPackets::InitialUpdateTeamData playerTeam;
        Skyfire::BattlePetPackets::InitialUpdateTeamData opponentTeam;
        if (!BuildPlayerPetBattleTeam(player, playerTeam) || !BuildPlayerPetBattleTeam(opponent, opponentTeam))
        {
            player.GetBattlePetMgr()->SendPetBattleRequestFailed(PET_BATTLE_REQUEST_FAILED_INVALID_TARGET);
            opponent.GetBattlePetMgr()->SendPetBattleRequestFailed(PET_BATTLE_REQUEST_FAILED_INVALID_TARGET);
            return false;
        }

        G3D::Vector3 opponentPositions[2] = { positions[1], positions[0] };
        float const opponentOrientation = Position::NormalizeOrientation(orientation + 3.14159265358979323846f);

        bool const playerStarted = StartPetBattlePvpForPlayer(player, opponent, playerTeam, opponentTeam,
            origin, positions, orientation, hasLocationResult, locationResult, battleSource);
        bool const opponentStarted = StartPetBattlePvpForPlayer(opponent, player, opponentTeam, playerTeam,
            origin, opponentPositions, opponentOrientation, hasLocationResult, locationResult, battleSource);

        if (!playerStarted || !opponentStarted)
        {
            player.GetBattlePetMgr()->ClearActivePetBattle();
            opponent.GetBattlePetMgr()->ClearActivePetBattle();
            return false;
        }

        return true;
    }

    void BuildPetBattleQueueLocation(Player const& player, Player const& opponent,
        G3D::Vector3& origin, G3D::Vector3* positions, float& orientation)
    {
        origin = G3D::Vector3(player.GetPositionX(), player.GetPositionY(), player.GetPositionZ());

        float dx = opponent.GetPositionX() - player.GetPositionX();
        float dy = opponent.GetPositionY() - player.GetPositionY();
        float length = std::sqrt(dx * dx + dy * dy);
        if (length < 0.1f)
        {
            dx = std::cos(player.GetOrientation());
            dy = std::sin(player.GetOrientation());
            length = 1.0f;
        }

        dx /= length;
        dy /= length;

        float const centerX = (player.GetPositionX() + opponent.GetPositionX()) * 0.5f;
        float const centerY = (player.GetPositionY() + opponent.GetPositionY()) * 0.5f;
        float const centerZ = (player.GetPositionZ() + opponent.GetPositionZ()) * 0.5f;

        positions[0] = G3D::Vector3(centerX - dx * 1.5f, centerY - dy * 1.5f, centerZ);
        positions[1] = G3D::Vector3(centerX + dx * 1.5f, centerY + dy * 1.5f, centerZ);
        orientation = Skyfire::BattlePetPackets::CalculateBattlePetFacing(origin, positions);
    }

    bool IsPetBattlePvpQueueCandidate(Player* player)
    {
        if (!player)
            return false;

        BattlePetMgr* battlePetMgr = player->GetBattlePetMgr();
        return battlePetMgr && battlePetMgr->IsPetBattlePvpQueued()
            && !battlePetMgr->HasActivePetBattle()
            && battlePetMgr->HasLoadoutBattlePet()
            && battlePetMgr->GetFirstAliveLoadoutBattlePet();
    }

    void RemovePetBattlePvpQueueEntry(uint64 playerGuid)
    {
        PetBattlePvpState.RemoveQueueEntry(playerGuid);
    }

    void ClearPetBattlePvpProposal(uint64 playerGuid)
    {
        PetBattlePvpState.ClearProposal(playerGuid);
    }

    bool IsPetBattlePvpProposalExpired(PetBattlePvpProposal const& proposal)
    {
        return proposal.CreatedAt && time(NULL) - proposal.CreatedAt >= PetBattlePvpProposalTimeout;
    }

    void LeavePetBattlePvpQueue(Player& player, bool sendStatus,
        uint32 playerStatus = Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_REMOVED,
        uint32 opponentStatus = Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_REMOVED,
        bool sendOpponentStatus = true)
    {
        uint64 const playerGuid = player.GetGUID();
        RemovePetBattlePvpQueueEntry(playerGuid);

        PetBattlePvpStateStore::ProposalMap::iterator proposalItr = PetBattlePvpState.Proposals().find(playerGuid);
        uint64 const opponentGuid = proposalItr != PetBattlePvpState.Proposals().end() ? proposalItr->second.OpponentGuid : 0;
        ClearPetBattlePvpProposal(playerGuid);

        player.GetBattlePetMgr()->SetPetBattlePvpQueued(false);
        if (sendStatus)
            SendPetBattleQueueStatus(player, playerStatus);

        if (!opponentGuid)
            return;

        if (Player* opponent = ObjectAccessor::FindPlayer(opponentGuid))
        {
            opponent->GetBattlePetMgr()->SetPetBattlePvpQueued(false);
            if (sendOpponentStatus)
                SendPetBattleQueueStatus(*opponent, opponentStatus);
        }
    }

    bool ExpirePetBattlePvpProposal(Player& player)
    {
        PetBattlePvpStateStore::ProposalMap::iterator proposalItr =
            PetBattlePvpState.Proposals().find(player.GetGUID());
        if (proposalItr == PetBattlePvpState.Proposals().end() || !IsPetBattlePvpProposalExpired(proposalItr->second))
            return false;

        LeavePetBattlePvpQueue(player, true,
            Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_PROPOSAL_TIMEOUT,
            Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_PROPOSAL_TIMEOUT);
        return true;
    }

    uint64 TakePetBattlePvpQueueOpponent(Player& player)
    {
        uint64 const playerGuid = player.GetGUID();
        RemovePetBattlePvpQueueEntry(playerGuid);

        for (PetBattlePvpStateStore::QueueContainer::iterator itr = PetBattlePvpState.Queue().begin();
            itr != PetBattlePvpState.Queue().end();)
        {
            Player* opponent = ObjectAccessor::FindPlayer(*itr);
            if (!IsPetBattlePvpQueueCandidate(opponent))
            {
                if (opponent)
                    opponent->GetBattlePetMgr()->SetPetBattlePvpQueued(false);

                itr = PetBattlePvpState.Queue().erase(itr);
                continue;
            }

            uint64 const opponentGuid = *itr;
            PetBattlePvpState.Queue().erase(itr);
            return opponentGuid;
        }

        return 0;
    }

    void ProposePetBattlePvpMatch(Player& player, Player& opponent)
    {
        uint64 const playerGuid = player.GetGUID();
        uint64 const opponentGuid = opponent.GetGUID();
        time_t const createdAt = time(NULL);

        PetBattlePvpState.AddProposalPair(playerGuid, opponentGuid, createdAt);

        SendPetBattleQueueProposeMatch(player);
        SendPetBattleQueueProposeMatch(opponent);
    }

    void JoinPetBattlePvpQueue(WorldSession& session, Player& player)
    {
        BattlePetMgr* battlePetMgr = player.GetBattlePetMgr();
        uint64 const playerGuid = player.GetGUID();

        if (battlePetMgr->IsPetBattlePvpQueued())
        {
            SendPetBattleQueueStatus(session, player, Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_ALREADY_QUEUED);
            return;
        }

        ClearPetBattlePvpChallengesFor(playerGuid);
        ClearPetBattlePvpProposal(playerGuid);
        RemovePetBattlePvpQueueEntry(playerGuid);
        battlePetMgr->SetPetBattlePvpQueued(true);
        SendPetBattleQueueStatus(session, player, Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_QUEUED);

        uint64 const opponentGuid = TakePetBattlePvpQueueOpponent(player);
        if (Player* opponent = ObjectAccessor::FindPlayer(opponentGuid))
        {
            opponent->GetBattlePetMgr()->SetPetBattlePvpQueued(true);
            ProposePetBattlePvpMatch(player, *opponent);
            return;
        }

        PetBattlePvpState.AddQueueEntry(playerGuid);
    }

    void FailAcceptedPetBattlePvpQueueMatch(Player& player, Player* opponent)
    {
        LeavePetBattlePvpQueue(player, true,
            Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_JOIN_FAILED,
            Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_JOIN_FAILED,
            opponent != nullptr);

        if (opponent && opponent->GetBattlePetMgr()->IsPetBattlePvpQueued())
        {
            opponent->GetBattlePetMgr()->SetPetBattlePvpQueued(false);
            SendPetBattleQueueStatus(*opponent,
                Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_JOIN_FAILED);
        }
    }

    bool ReadPetBattleQueueProposeAccepted(WorldPacket& recvData)
    {
        if (recvData.rpos() >= recvData.size())
            return false;

        return recvData.ReadBit();
    }

    void ReadPetBattlePvpDuelRequestLocation(WorldPacket& recvData, PetBattleRequest& petBattleRequest)
    {
        recvData >> petBattleRequest.Origin.z;

        for (uint8 i = 0; i < 2; i++)
        {
            recvData >> petBattleRequest.Positions[i].x;
            recvData >> petBattleRequest.Positions[i].z;
            recvData >> petBattleRequest.Positions[i].y;
        }

        recvData >> petBattleRequest.Origin.y;
        recvData >> petBattleRequest.Origin.x;

        petBattleRequest.LocationResult = 21;
        if (recvData.size() >= sizeof(uint32))
        {
            uint8 const* tail = recvData.contents() + recvData.size() - sizeof(uint32);
            petBattleRequest.LocationResult = uint32(tail[0])
                | (uint32(tail[1]) << 8)
                | (uint32(tail[2]) << 16)
                | (uint32(tail[3]) << 24);
        }
    }

    bool ValidatePetBattleQueueRequest(Player& player)
    {
        BattlePetMgr* battlePetMgr = player.GetBattlePetMgr();
        if (battlePetMgr->HasActivePetBattle())
        {
            battlePetMgr->SendPetBattleRequestFailed(PET_BATTLE_REQUEST_FAILED_INVALID_TARGET);
            return false;
        }

        if (player.IsInCombat())
        {
            battlePetMgr->SendPetBattleRequestFailed(PET_BATTLE_REQUEST_FAILED_INVALID_TARGET);
            return false;
        }

        if (!battlePetMgr->HasLoadoutBattlePet())
        {
            battlePetMgr->SendPetBattleRequestFailed(PET_BATTLE_REQUEST_FAILED_NO_PETS);
            return false;
        }

        if (!battlePetMgr->GetFirstAliveLoadoutBattlePet())
        {
            battlePetMgr->SendPetBattleRequestFailed(PET_BATTLE_REQUEST_FAILED_ALL_PETS_DEAD);
            return false;
        }

        return true;
    }

    bool ValidatePetBattlePvpDuelTarget(Player& challenger, Player* target)
    {
        BattlePetMgr* challengerBattlePetMgr = challenger.GetBattlePetMgr();
        BattlePetMgr* targetBattlePetMgr = target ? target->GetBattlePetMgr() : nullptr;
        if (!target || target == &challenger || !targetBattlePetMgr || targetBattlePetMgr->HasActivePetBattle()
            || target->GetMapId() != challenger.GetMapId())
        {
            challengerBattlePetMgr->SendPetBattleRequestFailed(PET_BATTLE_REQUEST_FAILED_INVALID_TARGET);
            return false;
        }

        if (target->IsInCombat())
        {
            challengerBattlePetMgr->SendPetBattleRequestFailed(PET_BATTLE_REQUEST_FAILED_INVALID_TARGET);
            return false;
        }

        if (!targetBattlePetMgr->HasLoadoutBattlePet())
        {
            challengerBattlePetMgr->SendPetBattleRequestFailed(PET_BATTLE_REQUEST_FAILED_NO_PETS);
            return false;
        }

        if (!targetBattlePetMgr->GetFirstAliveLoadoutBattlePet())
        {
            challengerBattlePetMgr->SendPetBattleRequestFailed(PET_BATTLE_REQUEST_FAILED_ALL_PETS_DEAD);
            return false;
        }

        return true;
    }

    bool HandlePendingPetBattlePvpChallengeResponse(Player& target,
        Skyfire::BattlePetPackets::BattlePetRequestUpdate const& request)
    {
        uint64 const targetGuid = target.GetGUID();
        PetBattlePvpStateStore::ChallengeMap::iterator challengeItr =
            PetBattlePvpState.ChallengesByTarget().find(targetGuid);
        if (challengeItr == PetBattlePvpState.ChallengesByTarget().end())
            return false;

        if (IsPetBattlePvpChallengeExpired(challengeItr->second))
        {
            ClearPetBattlePvpChallengeForTarget(targetGuid, true);
            return true;
        }

        PetBattlePvpChallenge challenge = challengeItr->second;
        Player* challenger = ObjectAccessor::FindPlayer(challenge.ChallengerGuid);
        if (!challenger)
        {
            ClearPetBattlePvpChallengeForTarget(targetGuid);
            target.GetBattlePetMgr()->SendPetBattleRequestFailed(PET_BATTLE_REQUEST_FAILED_INVALID_TARGET);
            return true;
        }

        if (request.Cancelled)
        {
            ClearPetBattlePvpChallengeForTarget(targetGuid);
            challenger->GetBattlePetMgr()->SendPetBattleRequestFailed(PET_BATTLE_REQUEST_FAILED_INVALID_TARGET);
            return true;
        }

        if (!challengeItr->second.PromptAcknowledged)
        {
            challengeItr->second.PromptAcknowledged = true;
            return true;
        }

        challenge = challengeItr->second;
        ClearPetBattlePvpChallengeForTarget(targetGuid);

        LeavePetBattlePvpQueue(*challenger, false);
        LeavePetBattlePvpQueue(target, false);

        if (!StartPetBattlePvp(*challenger, target, challenge.Origin, challenge.Positions,
            challenge.Orientation, challenge.HasLocationResult, challenge.LocationResult))
        {
            challenger->GetBattlePetMgr()->SendPetBattleRequestFailed(PET_BATTLE_REQUEST_FAILED_INVALID_TARGET);
            target.GetBattlePetMgr()->SendPetBattleRequestFailed(PET_BATTLE_REQUEST_FAILED_INVALID_TARGET);
        }

        return true;
    }

    bool CompletePendingPetBattlePvpChallenge(Player& target, bool accepted)
    {
        uint64 const targetGuid = target.GetGUID();
        PetBattlePvpStateStore::ChallengeMap::iterator challengeItr =
            PetBattlePvpState.ChallengesByTarget().find(targetGuid);
        if (challengeItr == PetBattlePvpState.ChallengesByTarget().end())
            return false;

        if (IsPetBattlePvpChallengeExpired(challengeItr->second))
        {
            ClearPetBattlePvpChallengeForTarget(targetGuid, true);
            return true;
        }

        PetBattlePvpChallenge challenge = challengeItr->second;
        Player* challenger = ObjectAccessor::FindPlayer(challenge.ChallengerGuid);
        ClearPetBattlePvpChallengeForTarget(targetGuid);

        if (!challenger)
        {
            target.GetBattlePetMgr()->SendPetBattleRequestFailed(PET_BATTLE_REQUEST_FAILED_INVALID_TARGET);
            return true;
        }

        if (!accepted)
        {
            challenger->GetBattlePetMgr()->SendPetBattleRequestFailed(PET_BATTLE_REQUEST_FAILED_INVALID_TARGET);
            return true;
        }

        LeavePetBattlePvpQueue(*challenger, false);
        LeavePetBattlePvpQueue(target, false);

        if (!StartPetBattlePvp(*challenger, target, challenge.Origin, challenge.Positions,
            challenge.Orientation, challenge.HasLocationResult, challenge.LocationResult))
        {
            challenger->GetBattlePetMgr()->SendPetBattleRequestFailed(PET_BATTLE_REQUEST_FAILED_INVALID_TARGET);
            target.GetBattlePetMgr()->SendPetBattleRequestFailed(PET_BATTLE_REQUEST_FAILED_INVALID_TARGET);
        }

        return true;
    }
}

void WorldSession::UpdatePetBattlePvpQueueState()
{
    Player* player = GetPlayer();
    if (!player)
        return;

    ExpirePetBattlePvpChallenge(*player);
    ExpirePetBattlePvpProposal(*player);
}

void WorldSession::ClearPetBattlePvpQueueState()
{
    Player* player = GetPlayer();
    if (!player)
        return;

    ClearPetBattlePvpChallengesFor(player->GetGUID(), true, player->GetGUID());
    LeavePetBattlePvpQueue(*player, false,
        Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_REMOVED,
        Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_OPPONENT_DECLINED);
}

void WorldSession::HandlePetBattleStartPvpMatchmaking(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_PET_BATTLE_START_PVP_MATCHMAKING");

    Player* player = GetPlayer();
    if (!player || !ValidatePetBattleQueueRequest(*player))
        return;

    JoinPetBattlePvpQueue(*this, *player);
}

void WorldSession::HandlePetBattleStopPvpMatchmaking(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_PET_BATTLE_STOP_PVP_MATCHMAKING");

    if (Player* player = GetPlayer())
        LeavePetBattlePvpQueue(*player, true);
}

void WorldSession::HandlePetBattleQueueProposeMatchResult(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_PET_BATTLE_QUEUE_PROPOSE_MATCH_RESULT");

    Player* player = GetPlayer();
    if (!player)
        return;

    if (ExpirePetBattlePvpProposal(*player))
        return;

    uint64 const playerGuid = player->GetGUID();
    PetBattlePvpStateStore::ProposalMap::iterator proposalItr = PetBattlePvpState.Proposals().find(playerGuid);
    if (proposalItr == PetBattlePvpState.Proposals().end())
    {
        SendPetBattleQueueStatus(*this, *player, Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_NONE);
        return;
    }

    if (!ReadPetBattleQueueProposeAccepted(recvData))
    {
        LeavePetBattlePvpQueue(*player, true,
            Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_PROPOSAL_DECLINED,
            Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_OPPONENT_DECLINED);
        return;
    }

    proposalItr->second.Accepted = true;

    uint64 const opponentGuid = proposalItr->second.OpponentGuid;
    PetBattlePvpStateStore::ProposalMap::iterator opponentProposalItr =
        PetBattlePvpState.Proposals().find(opponentGuid);
    if (opponentProposalItr == PetBattlePvpState.Proposals().end())
    {
        LeavePetBattlePvpQueue(*player, true);
        return;
    }

    if (!opponentProposalItr->second.Accepted)
        return;

    Player* opponent = ObjectAccessor::FindPlayer(opponentGuid);
    if (!opponent)
    {
        FailAcceptedPetBattlePvpQueueMatch(*player, nullptr);
        return;
    }

    if (!ValidatePetBattleQueueRequest(*player) || !ValidatePetBattlePvpDuelTarget(*player, opponent))
    {
        FailAcceptedPetBattlePvpQueueMatch(*player, opponent);
        return;
    }

    ClearPetBattlePvpProposal(playerGuid);
    RemovePetBattlePvpQueueEntry(playerGuid);
    RemovePetBattlePvpQueueEntry(opponentGuid);

    player->GetBattlePetMgr()->SetPetBattlePvpQueued(false);
    opponent->GetBattlePetMgr()->SetPetBattlePvpQueued(false);

    SendPetBattleQueueStatus(*this, *player, Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_NONE);
    SendPetBattleQueueStatus(*opponent, Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_NONE);

    G3D::Vector3 origin;
    G3D::Vector3 positions[2];
    float orientation = 0.0f;
    BuildPetBattleQueueLocation(*player, *opponent, origin, positions, orientation);
    if (!StartPetBattlePvp(*player, *opponent, origin, positions, orientation, false, 0,
        BATTLE_PET_ACHIEVEMENT_SOURCE_PVP_MATCHMAKING))
    {
        SendPetBattleQueueStatus(*this, *player,
            Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_JOIN_FAILED);
        SendPetBattleQueueStatus(*opponent,
            Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_JOIN_FAILED);
    }
}

void WorldSession::HandlePetBattleRequestPvp(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_PET_BATTLE_REQUEST_PVP");

    PetBattleRequest petBattleRequest = { };
    ReadPetBattlePvpDuelRequestLocation(recvData, petBattleRequest);

    Player* player = GetPlayer();
    if (!player)
        return;

    if (!ValidatePetBattleQueueRequest(*player))
        return;

    Player* target = player->GetSelectedPlayer();
    if (!ValidatePetBattlePvpDuelTarget(*player, target))
        return;

    LeavePetBattlePvpQueue(*player, false);
    LeavePetBattlePvpQueue(*target, false);

    uint64 const playerGuid = player->GetGUID();
    uint64 const targetGuid = target->GetGUID();
    ClearPetBattlePvpChallengesFor(playerGuid, true, playerGuid);
    ClearPetBattlePvpChallengesFor(targetGuid, true, targetGuid);

    petBattleRequest.Orientation = Skyfire::BattlePetPackets::ResolveBattlePetFacing(
        false, petBattleRequest.Orientation, petBattleRequest.Origin, petBattleRequest.Positions);

    PetBattlePvpChallenge challenge;
    challenge.ChallengerGuid = playerGuid;
    challenge.TargetGuid = targetGuid;
    challenge.Origin = petBattleRequest.Origin;
    challenge.Positions[0] = petBattleRequest.Positions[0];
    challenge.Positions[1] = petBattleRequest.Positions[1];
    challenge.Orientation = petBattleRequest.Orientation;
    challenge.LocationResult = petBattleRequest.LocationResult;
    challenge.HasLocationResult = true;
    challenge.CreatedAt = time(NULL);
    PetBattlePvpState.AddChallenge(challenge);

    WorldPacket data = Skyfire::BattlePetPackets::BuildPvpChallengePacket(
        player->GetGUID(), petBattleRequest.Origin, petBattleRequest.Positions,
        true, petBattleRequest.Orientation, true, petBattleRequest.LocationResult);
    target->SendDirectMessage(&data);
    SendPetBattleDuelPopup(*player, *target);
}

bool WorldSession::HandlePendingPetBattlePvpDuelResponse(bool accepted)
{
    Player* player = GetPlayer();
    if (!player)
        return false;

    return CompletePendingPetBattlePvpChallenge(*player, accepted);
}

void WorldSession::HandleBattlePetDelete(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_BATTLE_PET_DELETE");

    ObjectGuid petEntry;

    recvData.ReadGuidMask(petEntry, 3, 5, 6, 2, 4, 0, 7, 1);
    recvData.ReadGuidBytes(petEntry, 4, 1, 5, 0, 7, 2, 3, 6);

    BattlePetMgr* battlePetMgr = GetPlayer()->GetBattlePetMgr();

    BattlePet* battlePet = battlePetMgr->GetBattlePet(petEntry);
    if (!battlePet)
    {
        SF_LOG_DEBUG("network", "CMSG_BATTLE_PET_DELETE - Player %u tryed to release Battle Pet %lu which it doesn't own!",
            GetPlayer()->GetGUIDLow(), (uint64)petEntry);
        return;
    }

    if (!BattlePetSpeciesHasFlag(battlePet->GetSpecies(), BATTLE_PET_FLAG_RELEASABLE))
    {
        SF_LOG_DEBUG("network", "CMSG_BATTLE_PET_DELETE - Player %u tryed to release Battle Pet %lu which isn't releasable!",
            GetPlayer()->GetGUIDLow(), (uint64)petEntry);
        return;
    }

    if (battlePetMgr->GetLoadoutSlotForBattlePet(petEntry) != BATTLE_PET_LOADOUT_SLOT_NONE)
    {
        SF_LOG_DEBUG("network", "CMSG_BATTLE_PET_DELETE - Player %u tryed to release slotted Battle Pet %lu!",
            GetPlayer()->GetGUIDLow(), (uint64)petEntry);
        return;
    }

    battlePetMgr->Delete(battlePet);
    battlePetMgr->SaveToDb();
}

#define BATTLE_PET_MAX_DECLINED_NAMES 5

void WorldSession::HandleBattlePetModifyName(WorldPacket& recvData)
{
    // TODO: finish this...
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_BATTLE_PET_MODIFY_NAME");

    ObjectGuid petEntry;
    uint8 nicknameLen;
    std::string nickname;
    bool hasDeclinedNames;
    uint8 declinedNameLen[BATTLE_PET_MAX_DECLINED_NAMES];

    petEntry[5] = recvData.ReadBit();
    petEntry[7] = recvData.ReadBit();
    petEntry[3] = recvData.ReadBit();
    petEntry[0] = recvData.ReadBit();
    petEntry[6] = recvData.ReadBit();
    nicknameLen = recvData.ReadBits(7);
    petEntry[2] = recvData.ReadBit();
    petEntry[1] = recvData.ReadBit();
    hasDeclinedNames = recvData.ReadBit();
    petEntry[4] = recvData.ReadBit();

    if (hasDeclinedNames)
        for (uint8 i = 0; i < BATTLE_PET_MAX_DECLINED_NAMES; i++)
            declinedNameLen[i] = recvData.ReadBits(7);

    recvData.ReadByteSeq(petEntry[3]);
    recvData.ReadByteSeq(petEntry[0]);
    recvData.ReadByteSeq(petEntry[6]);
    recvData.ReadByteSeq(petEntry[1]);
    recvData.ReadByteSeq(petEntry[5]);
    recvData.ReadByteSeq(petEntry[2]);
    recvData.ReadByteSeq(petEntry[4]);
    recvData.ReadByteSeq(petEntry[7]);
    nickname = recvData.ReadString(nicknameLen);

    if (hasDeclinedNames)
    {
        std::string declinedNames[BATTLE_PET_MAX_DECLINED_NAMES];
        for (uint8 i = 0; i < BATTLE_PET_MAX_DECLINED_NAMES; i++)
            declinedNames[i] = recvData.ReadString(declinedNameLen[i]);
    }

    BattlePetMgr* battlePetMgr = GetPlayer()->GetBattlePetMgr();

    BattlePet* battlePet = battlePetMgr->GetBattlePet(petEntry);
    if (!battlePet)
    {
        SF_LOG_DEBUG("network", "CMSG_BATTLE_PET_MODIFY_NAME - Player %u tryed to set the name for Battle Pet %lu which it doesn't own!",
            GetPlayer()->GetGUIDLow(), (uint64)petEntry);
        return;
    }

    if (nickname.size() > BATTLE_PET_MAX_NAME_LENGTH)
    {
        SF_LOG_DEBUG("network", "CMSG_BATTLE_PET_MODIFY_NAME - Player %u tryed to set the name for Battle Pet %lu with an invalid length!",
            GetPlayer()->GetGUIDLow(), (uint64)petEntry);
        return;
    }

    // TODO: check for invalid characters, ect...

    battlePet->SetNickname(nickname);
    battlePet->SetTimestamp((uint32)time(NULL));

    if (battlePetMgr->GetCurrentSummonId())
        battlePetMgr->GetCurrentSummon()->SetUInt32Value(UNIT_FIELD_BATTLE_PET_COMPANION_NAME_TIMESTAMP, battlePet->GetTimestamp());

    battlePetMgr->SaveToDb();
}

void WorldSession::HandleBattlePetQueryName(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_BATTLE_PET_QUERY_NAME");

    ObjectGuid petEntry, petguid;

    petguid[2] = recvData.ReadBit();
    petEntry[6] = recvData.ReadBit();
    petEntry[3] = recvData.ReadBit();
    petguid[3] = recvData.ReadBit();
    petEntry[7] = recvData.ReadBit();
    petguid[4] = recvData.ReadBit();
    petguid[1] = recvData.ReadBit();
    petguid[0] = recvData.ReadBit();
    petEntry[0] = recvData.ReadBit();
    petguid[7] = recvData.ReadBit();
    petguid[5] = recvData.ReadBit();
    petguid[6] = recvData.ReadBit();
    petEntry[1] = recvData.ReadBit();
    petEntry[2] = recvData.ReadBit();
    petEntry[5] = recvData.ReadBit();
    petEntry[4] = recvData.ReadBit();

    recvData.ReadByteSeq(petguid[5]);
    recvData.ReadByteSeq(petEntry[1]);
    recvData.ReadByteSeq(petguid[0]);
    recvData.ReadByteSeq(petEntry[4]);
    recvData.ReadByteSeq(petguid[3]);
    recvData.ReadByteSeq(petEntry[3]);
    recvData.ReadByteSeq(petguid[1]);
    recvData.ReadByteSeq(petguid[6]);
    recvData.ReadByteSeq(petEntry[6]);
    recvData.ReadByteSeq(petEntry[0]);
    recvData.ReadByteSeq(petEntry[2]);
    recvData.ReadByteSeq(petguid[7]);
    recvData.ReadByteSeq(petguid[2]);
    recvData.ReadByteSeq(petEntry[7]);
    recvData.ReadByteSeq(petguid[4]);
    recvData.ReadByteSeq(petEntry[5]);

    Unit* tempUnit = sObjectAccessor->FindUnit(petguid);
    if (!tempUnit)
    {
        SF_LOG_DEBUG("network", "CMSG_BATTLE_PET_QUERY_NAME - Player %u queried the name of Battle Pet %lu which doesnt't exist in world!",
            GetPlayer()->GetGUIDLow(), (uint64)petEntry);
        return;
    }

    Unit* ownerUnit = tempUnit->ToTempSummon()->GetSummoner();
    if (!ownerUnit)
        return;

    BattlePetMgr* battlePetMgr = ownerUnit->ToPlayer()->GetBattlePetMgr();

    BattlePet* battlePet = battlePetMgr->GetBattlePet(battlePetMgr->GetCurrentSummonId());
    if (!battlePet)
        return;

    BattlePetSpeciesEntry const* speciesEntry = sBattlePetSpeciesStore.LookupEntry(battlePet->GetSpecies());
    if (!speciesEntry)
        return;

    WorldPacket data(SMSG_BATTLE_PET_QUERY_NAME_RESPONSE, 8 + 4 + 4 + 5 + battlePet->GetNickname().size());
    data << uint64(petEntry);
    data << uint32(battlePet->GetTimestamp());
    data << uint32(speciesEntry->NpcId);
    data.WriteBit(1);               // has names
    data.WriteBits(battlePet->GetNickname().size(), 8);

    // TODO: finish declined names
    for (uint8 i = 0; i < BATTLE_PET_MAX_DECLINED_NAMES; i++)
        data.WriteBits(0, 7);

    data.WriteBit(0);               // allowed?
    data.FlushBits();

    data.WriteString(battlePet->GetNickname());

    SendPacket(&data);
}

void WorldSession::HandleBattlePetRequestJournal(WorldPacket& /*recvData*/)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_BATTLE_PET_REQUEST_JOURNAL");
    BattlePetMgr* battlePetMgr = GetPlayer()->GetBattlePetMgr();
    battlePetMgr->SendBattlePetJournal();
    battlePetMgr->SendBattlePetJournalLock();
}

void WorldSession::HandleBattlePetLearn(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_BATTLE_PET_LEARN");

    ObjectGuid itemGuid;
    recvData.ReadGuidMask(itemGuid, 7, 1, 0, 5, 4, 2, 3, 6);
    recvData.ReadGuidBytes(itemGuid, 2, 3, 4, 1, 5, 7, 0, 6);

    Player* player = GetPlayer();
    Item* item = player->GetItemByGuid(itemGuid);
    if (!item)
    {
        SF_LOG_DEBUG("network", "CMSG_BATTLE_PET_LEARN - Player %u tried to learn a battle pet from missing item " UI64FMTD,
            player->GetGUIDLow(), uint64(itemGuid));
        return;
    }

    BattlePetItemXSpeciesStore::const_iterator speciesItr = sBattlePetItemXSpeciesStore.find(item->GetEntry());
    if (speciesItr == sBattlePetItemXSpeciesStore.end() || !speciesItr->second)
    {
        SF_LOG_DEBUG("network", "CMSG_BATTLE_PET_LEARN - Player %u tried to learn a battle pet from non-pet item %u",
            player->GetGUIDLow(), item->GetEntry());
        return;
    }

    BattlePetMgr* battlePetMgr = player->GetBattlePetMgr();
    if (!battlePetMgr->Create(speciesItr->second))
        return;

    player->DestroyItem(item->GetBagSlot(), item->GetSlot(), true);
    battlePetMgr->SaveToDb();
}

void WorldSession::HandleBattlePetSetBattleSlot(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_BATTLE_PET_SET_BATTLE_SLOT");

    uint8 slot;
    ObjectGuid petEntry;

    recvData >> slot;

    recvData.ReadGuidMask(petEntry, 4, 6, 5, 7, 3, 1, 0, 2);
    recvData.ReadGuidBytes(petEntry, 1, 3, 5, 0, 7, 6, 4, 2);

    BattlePetMgr* battlePetMgr = GetPlayer()->GetBattlePetMgr();

    BattlePet* battlePet = battlePetMgr->GetBattlePet(petEntry);
    if (!battlePet)
    {
        SF_LOG_DEBUG("network", "CMSG_BATTLE_PET_SET_BATTLE_SLOT - Player %u tryed to add Battle Pet %lu to loadout which it doesn't own!",
            GetPlayer()->GetGUIDLow(), (uint64)petEntry);
        return;
    }

    if (!battlePetMgr->HasLoadoutSlot(slot))
    {
        SF_LOG_DEBUG("network", "CMSG_BATTLE_PET_SET_BATTLE_SLOT - Player %u tryed to add Battle Pet %lu into slot %u which is locked!",
            GetPlayer()->GetGUIDLow(), (uint64)petEntry, slot);
        return;
    }

    // this check is also done clientside
    if (BattlePetSpeciesHasFlag(battlePet->GetSpecies(), BATTLE_PET_FLAG_COMPANION))
    {
        SF_LOG_DEBUG("network", "CMSG_BATTLE_PET_SET_BATTLE_SLOT - Player %u tryed to add a compainion Battle Pet %lu into slot %u!",
            GetPlayer()->GetGUIDLow(), (uint64)petEntry, slot);
        return;
    }

    // sever handles slot swapping, find source slot and replace it with the destination slot
    uint64 const dstPetEntry = battlePetMgr->GetLoadoutSlot(slot);
    uint8 srcSlot = battlePetMgr->GetLoadoutSlotForBattlePet(petEntry);
    if (srcSlot != BATTLE_PET_LOADOUT_SLOT_NONE && srcSlot != slot)
    {
        battlePetMgr->SetLoadoutSlot(srcSlot, dstPetEntry, true);
        battlePetMgr->SendBattlePetSlotUpdate(srcSlot, false, dstPetEntry);
    }

    battlePetMgr->SetLoadoutSlot(slot, petEntry, true);
    battlePetMgr->SendBattlePetSlotUpdate(slot, false, petEntry);
    battlePetMgr->SaveToDb();
}

enum BattlePetFlagModes
{
    BATTLE_PET_FLAG_MODE_UNSET = 0,
    BATTLE_PET_FLAG_MODE_SET = 3
};

void WorldSession::HandleBattlePetSetFlags(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_BATTLE_PET_SET_FLAGS");

    ObjectGuid petEntry;
    uint32 flag;

    recvData >> flag;

    recvData.ReadGuidMask(petEntry, 5, 4);
    uint8 mode = recvData.ReadBits(2);
    recvData.ReadGuidMask(petEntry, 1, 4, 6, 3, 7, 0);

    recvData.ReadGuidBytes(petEntry, 4, 0, 7, 3, 1, 6, 2, 5);

    BattlePetMgr* battlePetMgr = GetPlayer()->GetBattlePetMgr();
    BattlePet* battlePet = battlePetMgr->GetBattlePet(petEntry);
    if (!battlePet)
    {
        SF_LOG_DEBUG("network", "CMSG_BATTLE_PET_SET_FLAGS - Player %u tryed to set the flags for Battle Pet %lu which it doesn't own!",
            GetPlayer()->GetGUIDLow(), (uint64)petEntry);
        return;
    }

    // list of flags the client can currently change
    if (flag != BATTLE_PET_JOURNAL_FLAG_FAVORITES
        && flag != BATTLE_PET_JOURNAL_FLAG_ABILITY_1
        && flag != BATTLE_PET_JOURNAL_FLAG_ABILITY_2
        && flag != BATTLE_PET_JOURNAL_FLAG_ABILITY_3)
    {
        SF_LOG_DEBUG("network", "CMSG_BATTLE_PET_SET_FLAGS - Player %u tryed to set an invalid Battle Pet flag %u!",
            GetPlayer()->GetGUIDLow(), flag);
        return;
    }

    if (mode != BATTLE_PET_FLAG_MODE_SET && mode != BATTLE_PET_FLAG_MODE_UNSET)
    {
        SF_LOG_DEBUG("network", "CMSG_BATTLE_PET_SET_FLAGS - Player %u tryed to set Battle Pet flag %u with invalid mode %u!",
            GetPlayer()->GetGUIDLow(), flag, mode);
        return;
    }

    uint8 const abilitySlot = BattlePetAbilitySlotForJournalFlag(flag);
    if (abilitySlot != BATTLE_PET_ABILITY_SLOT_INVALID)
    {
        bool const useAlternateAbility = mode == BATTLE_PET_FLAG_MODE_SET;
        if (!BattlePetHasSpeciesAbility(battlePet->GetSpecies(), abilitySlot, useAlternateAbility, battlePet->GetLevel()))
        {
            SF_LOG_DEBUG("network", "CMSG_BATTLE_PET_SET_FLAGS - Player %u tryed to select unavailable Battle Pet ability flag %u for pet %lu!",
                GetPlayer()->GetGUIDLow(), flag, (uint64)petEntry);
            return;
        }
    }

    uint16 const oldFlags = battlePet->GetFlags();
    switch (mode)
    {
        case BATTLE_PET_FLAG_MODE_SET:
            battlePet->SetFlag(flag);
            break;
        case BATTLE_PET_FLAG_MODE_UNSET:
            battlePet->UnSetFlag(flag);
            break;
    }

    if (oldFlags != battlePet->GetFlags())
    {
        battlePetMgr->SendBattlePetUpdate(battlePet, false);
        battlePetMgr->SaveToDb();
    }
}

void WorldSession::HandleBattlePetSummonCompanion(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_BATTLE_PET_SUMMON_COMPANION");

    ObjectGuid petEntry;

    recvData.ReadGuidMask(petEntry, 3, 2, 5, 0, 7, 1, 6, 4);
    recvData.ReadGuidBytes(petEntry, 6, 7, 3, 5, 0, 4, 1, 2);

    Player* player = GetPlayer();
    BattlePetMgr* battlePetMgr = player->GetBattlePetMgr();

    BattlePet* battlePet = battlePetMgr->GetBattlePet(petEntry);
    if (!battlePet)
    {
        SF_LOG_DEBUG("network", "CMSG_SUMMON_BATTLE_PET_COMPANION - Player %u tryed to summon battle pet companion %lu which it doesn't own!",
            player->GetGUIDLow(), (uint64)petEntry);
        return;
    }

    if (!battlePet->GetCurrentHealth())
    {
        SF_LOG_DEBUG("network", "CMSG_SUMMON_BATTLE_PET_COMPANION - Player %u tryed to summon battle pet companion %lu which is dead!",
            player->GetGUIDLow(), (uint64)petEntry);
        return;
    }

    if (battlePetMgr->GetCurrentSummonId() == petEntry)
        battlePetMgr->UnSummonCurrentBattlePet(false);
    else
    {
        if (uint32 summonSpell = BattlePetGetSummonSpell(battlePet->GetSpecies()))
        {
            battlePetMgr->SetCurrentSummonId(petEntry);
            player->CastSpell(player, summonSpell, true);
        }
    }
}

void WorldSession::HandleBattlePetInput(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_PET_BATTLE_INPUT");

    Skyfire::BattlePetPackets::BattlePetInput input = Skyfire::BattlePetPackets::ReadBattlePetInput(recvData);
    Skyfire::BattlePetPackets::BattlePetInputCommand command =
        Skyfire::BattlePetPackets::BuildBattlePetInputCommand(input);
    if (command.Action == Skyfire::BattlePetPackets::BATTLE_PET_INPUT_ACTION_NONE)
        return;

    Player* player = GetPlayer();
    BattlePetMgr* battlePetMgr = player->GetBattlePetMgr();
    if (!battlePetMgr->HasActivePetBattle())
        return;

    bool handled = false;
    bool sendRound = false;
    bool sendFinal = false;
    uint64 pvpForfeitOpponentGuid = 0;
    uint64 pvpFinalOpponentGuid = 0;
    Skyfire::BattlePetPackets::BattlePetRoundResult round;
    Skyfire::BattlePetPackets::BattlePetFinalRound finalRound;

    switch (command.Action)
    {
        case Skyfire::BattlePetPackets::BATTLE_PET_INPUT_ACTION_ABILITY:
        {
            ActivePetBattle const& activeBattle = battlePetMgr->GetActivePetBattle();
            uint32 const roundId = activeBattle.RoundID;
            uint32 const enemyAbilityId = BattlePetGetSpeciesAbility(
                activeBattle.EnemySpecies, 0, 0, activeBattle.EnemyLevel);
            BattlePet const* allyBattlePet = GetActiveBattlePet(*battlePetMgr);
            BattlePet const* enemyBattlePet = GetPvpOpponentActiveBattlePet(*player, activeBattle);
            uint32 const enemyDamage = enemyBattlePet
                ? BattlePetInputDamageForAbility(enemyAbilityId, enemyBattlePet)
                : BattlePetInputDamageForEnemyAbility(enemyAbilityId, activeBattle);
            uint8 const allyAbilitySlot = BattlePetActiveAbilitySlot(allyBattlePet, command.AbilityID);
            if (allyAbilitySlot == BATTLE_PET_ABILITY_SLOT_INVALID)
                return;

            if (activeBattle.IsPvP())
                pvpFinalOpponentGuid = activeBattle.EnemyGUID;

            handled = battlePetMgr->ApplyBattlePetAbilityExchangeInput(roundId,
                BattlePetInputDamageForAbility(command.AbilityID, allyBattlePet),
                BattlePetInputEffectForAbility(command.AbilityID),
                allyAbilitySlot, command.AbilityID, BattlePetAbilityCooldown(command.AbilityID),
                enemyDamage,
                BattlePetInputEffectForAbility(enemyAbilityId),
                BattlePetInputIncomingDamageReductionForAbility(command.AbilityID, allyBattlePet),
                BattlePetIncomingDamageReductionRounds(command.AbilityID),
                enemyBattlePet
                    ? BattlePetInputIncomingDamageReductionForAbility(enemyAbilityId, enemyBattlePet)
                    : BattlePetInputIncomingDamageReductionForEnemyAbility(enemyAbilityId, activeBattle),
                BattlePetIncomingDamageReductionRounds(enemyAbilityId),
                round, &finalRound);
            sendRound = handled;
            sendFinal = !finalRound.Pets.empty();
            break;
        }
        case Skyfire::BattlePetPackets::BATTLE_PET_INPUT_ACTION_SWAP:
            if (command.NewFrontPet < 0)
                return;

            handled = battlePetMgr->ApplyBattlePetSwapInput(battlePetMgr->GetActivePetBattle().RoundID,
                uint8(command.NewFrontPet), round);
            sendRound = handled;
            break;
        case Skyfire::BattlePetPackets::BATTLE_PET_INPUT_ACTION_FORFEIT:
        {
            ActivePetBattle const& activeBattle = battlePetMgr->GetActivePetBattle();
            if (activeBattle.IsPvP())
                pvpForfeitOpponentGuid = activeBattle.EnemyGUID;

            handled = battlePetMgr->ApplyBattlePetForfeitInput(battlePetMgr->GetActivePetBattle().RoundID, finalRound);
            sendFinal = handled;
            break;
        }
        case Skyfire::BattlePetPackets::BATTLE_PET_INPUT_ACTION_TRAP:
        {
            ActivePetBattle const& activeBattle = battlePetMgr->GetActivePetBattle();
            uint32 const enemyAbilityId = BattlePetGetSpeciesAbility(
                activeBattle.EnemySpecies, 0, 0, activeBattle.EnemyLevel);
            BattlePet const* enemyBattlePet = GetPvpOpponentActiveBattlePet(*player, activeBattle);
            uint32 const enemyDamage = enemyBattlePet
                ? BattlePetInputDamageForAbility(enemyAbilityId, enemyBattlePet)
                : BattlePetInputDamageForEnemyAbility(enemyAbilityId, activeBattle);

            handled = battlePetMgr->ApplyBattlePetTrapInput(battlePetMgr->GetActivePetBattle().RoundID,
                BattlePetInputEffectForAbility(battlePetMgr->GetTrapAbility()), enemyDamage,
                BattlePetInputEffectForAbility(enemyAbilityId), round, finalRound);
            sendRound = handled;
            sendFinal = handled && !finalRound.Pets.empty();
            break;
        }
        default:
            return;
    }

    if (!handled)
        return;

    if (sendRound)
        SendBattlePetRoundResult(*player, round);

    if (sendFinal)
    {
        if (pvpForfeitOpponentGuid)
            FinishActivePvpPetBattleAfterForfeit(*player, pvpForfeitOpponentGuid, finalRound);
        else if (pvpFinalOpponentGuid)
            FinishActivePvpPetBattle(*player, pvpFinalOpponentGuid, finalRound,
                OpponentWinnerForFinalRound(finalRound), false);
        else
        {
            SendBattlePetFinalRound(*player, finalRound);
            ClearActivePetBattleAndSave(*player);
        }
    }
}

void WorldSession::HandleBattlePetInputFirstPet(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_PET_BATTLE_SET_FRONT_PET");

    Skyfire::BattlePetPackets::BattlePetFirstPetSelection selection =
        Skyfire::BattlePetPackets::ReadBattlePetFirstPetSelection(recvData);

    Player* player = GetPlayer();
    BattlePetMgr* battlePetMgr = player->GetBattlePetMgr();
    if (!battlePetMgr->HasActivePetBattle())
        return;

    ActivePetBattle const& activeBattle = battlePetMgr->GetActivePetBattle();
    if (activeBattle.WaitingForAllyFrontPet)
    {
        Skyfire::BattlePetPackets::BattlePetRoundResult round;
        if (!battlePetMgr->ApplyBattlePetSwapInput(activeBattle.RoundID, selection.PetID, round))
            return;

        SendBattlePetRoundResult(*player, round);
        return;
    }

    if (!battlePetMgr->SelectActivePetBattleFrontPet(selection.PetID))
        return;

    ActivePetBattle const& selectedBattle = battlePetMgr->GetActivePetBattle();
    WorldPacket firstRound = Skyfire::BattlePetPackets::BuildFirstRoundPacket(
        selectedBattle.RoundID, selectedBattle.AllyFrontPet, selectedBattle.EnemyFrontPet,
        battlePetMgr->GetActivePetBattleTrapStatus(), PET_BATTLE_TRAP_STATUS_UNAVAILABLE);
    player->SendDirectMessage(&firstRound);
}

void WorldSession::HandleBattlePetFinalNotify(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_PET_BATTLE_FINAL_NOTIFY");

    Player* player = GetPlayer();
    if (!player)
        return;

    SendBattlePetFinished(*player);

    BattlePetMgr* battlePetMgr = player->GetBattlePetMgr();
    if (battlePetMgr->HasActivePetBattle())
        ClearActivePetBattleAndSave(*player);
}

void WorldSession::HandleBattlePetQuitNotify(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_PET_BATTLE_QUIT_NOTIFY");

    Player* player = GetPlayer();
    BattlePetMgr* battlePetMgr = player->GetBattlePetMgr();
    if (!battlePetMgr->HasActivePetBattle())
        return;

    ActivePetBattle const& activeBattle = battlePetMgr->GetActivePetBattle();
    uint64 const pvpOpponentGuid = activeBattle.IsPvP() ? activeBattle.EnemyGUID : 0;
    Skyfire::BattlePetPackets::BattlePetFinalRound finalRound;
    if (battlePetMgr->ApplyBattlePetForfeitInput(activeBattle.RoundID, finalRound))
    {
        if (pvpOpponentGuid)
        {
            FinishActivePvpPetBattleAfterForfeit(*player, pvpOpponentGuid, finalRound);
            return;
        }

        SendBattlePetFinalRound(*player, finalRound);
    }

    ClearActivePetBattleAndSave(*player);
}

void WorldSession::HandleBattlePetRequestUpdate(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_PET_BATTLE_REQUEST_UPDATE");

    Player* player = GetPlayer();
    if (!player)
        return;

    BattlePetMgr* battlePetMgr = player->GetBattlePetMgr();
    Skyfire::BattlePetPackets::BattlePetRequestUpdate request =
        Skyfire::BattlePetPackets::ReadBattlePetRequestUpdate(recvData);

    if (!battlePetMgr->HasActivePetBattle())
    {
        if (HandlePendingPetBattlePvpChallengeResponse(*player, request))
            return;

        if (ExpirePetBattlePvpProposal(*player))
            return;

        if (battlePetMgr->IsPetBattlePvpQueued())
            SendPetBattleQueueStatus(*this, *player, Skyfire::BattlePetPackets::PET_BATTLE_QUEUE_STATUS_QUEUED);

        return;
    }

    if (!request.Cancelled)
        return;

    ActivePetBattle const& activeBattle = battlePetMgr->GetActivePetBattle();
    uint64 const pvpOpponentGuid = activeBattle.IsPvP() ? activeBattle.EnemyGUID : 0;
    Skyfire::BattlePetPackets::BattlePetFinalRound finalRound;
    if (!battlePetMgr->ApplyBattlePetForfeitInput(activeBattle.RoundID, finalRound))
        return;

    if (pvpOpponentGuid)
        FinishActivePvpPetBattleAfterForfeit(*player, pvpOpponentGuid, finalRound);
    else
    {
        SendBattlePetFinalRound(*player, finalRound);
        ClearActivePetBattleAndSave(*player);
    }
}

void WorldSession::HandleBattlePetWildRequest(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_BATTLE_PET_WILD_REQUEST");
    ObjectGuid guid;
    bool missingOrientation;
    bool missingResult;

    PetBattleRequest petBattleRequest = { };

    for (uint8 i = 0; i < 2; i++) // team positions
    {
        recvData >> petBattleRequest.Positions[i].x;
        recvData >> petBattleRequest.Positions[i].z;
        recvData >> petBattleRequest.Positions[i].y;
    }

    // origin position
    recvData >> petBattleRequest.Origin.z;
    recvData >> petBattleRequest.Origin.y;
    recvData >> petBattleRequest.Origin.x;

    recvData.ReadGuidMask(guid, 0);
    missingOrientation = recvData.ReadBit();
    recvData.ReadGuidMask(guid, 6, 3, 5, 2, 7, 1, 4);
    missingResult = recvData.ReadBit();

    recvData.ReadGuidBytes(guid, 3, 6, 5, 2, 7, 1, 0, 4);

    if (!missingOrientation)
        recvData >> petBattleRequest.Orientation;

    if (!missingResult)
        recvData >> petBattleRequest.LocationResult;

    Player* player = GetPlayer();
    if (!player)
        return;

    BattlePetMgr* battlePetMgr = player->GetBattlePetMgr();
    if (player->IsInCombat())
    {
        battlePetMgr->SendPetBattleRequestFailed(PET_BATTLE_REQUEST_FAILED_INVALID_TARGET);
        return;
    }

    Creature* wildBattlePet = ObjectAccessor::GetCreatureOrPetOrVehicle(*player, guid);
    if (!wildBattlePet)
    {
        battlePetMgr->SendPetBattleRequestFailed(PET_BATTLE_REQUEST_FAILED_INVALID_TARGET);
        return;
    }

    if (!wildBattlePet->HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_WILDPET_CAPTURABLE))
    {
        battlePetMgr->SendPetBattleRequestFailed(PET_BATTLE_REQUEST_FAILED_INVALID_TARGET);
        return;
    }

    if (!battlePetMgr->HasLoadoutBattlePet())
    {
        battlePetMgr->SendPetBattleRequestFailed(PET_BATTLE_REQUEST_FAILED_NO_PETS);
        return;
    }

    BattlePet* activeBattlePet = battlePetMgr->GetFirstAliveLoadoutBattlePet();
    if (!activeBattlePet)
    {
        battlePetMgr->SendPetBattleRequestFailed(PET_BATTLE_REQUEST_FAILED_ALL_PETS_DEAD);
        return;
    }

    Skyfire::BattlePetPackets::InitialUpdateTeamData teams[2];
    uint8 activeBattlePetIndex = 0;
    for (uint8 slot = 0; slot < BATTLE_PET_MAX_LOADOUT_SLOTS
        && teams[0].PetCount < Skyfire::BattlePetPackets::INITIAL_UPDATE_MAX_TEAM_PETS; slot++)
    {
        BattlePet* loadoutBattlePet = battlePetMgr->GetBattlePet(battlePetMgr->GetLoadoutSlot(slot));
        if (!loadoutBattlePet)
            continue;

        uint8 const petIndex = teams[0].PetCount;
        teams[0].Pets[petIndex] = Skyfire::BattlePetPackets::BuildInitialUpdatePetData(*loadoutBattlePet);
        teams[0].PetCount++;

        if (loadoutBattlePet == activeBattlePet)
            activeBattlePetIndex = petIndex;
    }

    teams[0].FrontPet = activeBattlePetIndex;
    teams[0].OwnerGuid = player->GetGUID();
    teams[0].TrapAbility = battlePetMgr->GetTrapAbility();
    teams[1].PetCount = 1;
    teams[1].TrapStatus = PET_BATTLE_TRAP_STATUS_UNAVAILABLE;

    uint8 wildPetBreed = 0;
    if (!BuildWildInitialUpdatePetData(*wildBattlePet, teams[1].Pets[0], wildPetBreed))
    {
        battlePetMgr->SendPetBattleRequestFailed(PET_BATTLE_REQUEST_FAILED_INVALID_TARGET);
        return;
    }

    petBattleRequest.EnemyGUID = wildBattlePet->GetGUID();
    petBattleRequest.Type = PetBattleRequest::PET_BATTLE_TYPE_PVE;
    petBattleRequest.Challenger = player;
    petBattleRequest.Enemy = wildBattlePet;
    bool const hasClientOrientation = !missingOrientation;
    petBattleRequest.Orientation = Skyfire::BattlePetPackets::ResolveBattlePetFacing(
        hasClientOrientation, petBattleRequest.Orientation, petBattleRequest.Origin, petBattleRequest.Positions);

    battlePetMgr->ApplyActivePetBattlePlayerState(petBattleRequest.Positions[1].x, petBattleRequest.Positions[1].y);

    WorldPacket data = Skyfire::BattlePetPackets::BuildFinalizeLocationPacket(
        petBattleRequest.Origin, petBattleRequest.Positions, true, petBattleRequest.Orientation,
        !missingResult, petBattleRequest.LocationResult);
    player->SendDirectMessage(&data);

    battlePetMgr->StartWildPetBattle(petBattleRequest.EnemyGUID,
        activeBattlePet->GetId(), activeBattlePet->GetMaxHealth(), activeBattlePet->GetCurrentHealth(),
        teams[1].Pets[0].Id, teams[1].Pets[0].MaxHealth, teams[1].Pets[0].CurrentHealth,
        uint16(teams[1].Pets[0].Species), uint8(teams[1].Pets[0].Level), uint8(teams[1].Pets[0].Quality),
        wildPetBreed, activeBattlePetIndex);

    for (uint8 petIndex = 0; petIndex < teams[0].PetCount; petIndex++)
        battlePetMgr->SetActivePetBattleAllyPet(petIndex, teams[0].Pets[petIndex].Id,
            teams[0].Pets[petIndex].MaxHealth, teams[0].Pets[petIndex].CurrentHealth);

    battlePetMgr->SelectActivePetBattleFrontPet(activeBattlePetIndex);
    teams[0].TrapStatus = battlePetMgr->GetActivePetBattleTrapStatus();

    WorldPacket data2 = Skyfire::BattlePetPackets::BuildInitialUpdatePacket(
        petBattleRequest.EnemyGUID, teams, 2, petBattleRequest.Type != PetBattleRequest::PET_BATTLE_TYPE_PVE);
    player->SendDirectMessage(&data2);
    battlePetMgr->HideActivePetBattleWorldObject(wildBattlePet);
}

void WorldSession::HandleBattlePetCage(WorldPacket& recvData)
{

}
