/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef BATTLE_PET_PVP_STATE_H
#define BATTLE_PET_PVP_STATE_H

#include "Common.h"
#include "G3D/Vector3.h"

#include <ctime>
#include <map>
#include <vector>

struct PetBattlePvpProposal
{
    uint64 OpponentGuid = 0;
    bool Accepted = false;
    time_t CreatedAt = 0;
};

struct PetBattlePvpChallenge
{
    uint64 ChallengerGuid = 0;
    uint64 TargetGuid = 0;
    G3D::Vector3 Origin;
    G3D::Vector3 Positions[2];
    float Orientation = 0.0f;
    uint32 LocationResult = 0;
    bool HasLocationResult = false;
    bool PromptAcknowledged = false;
    time_t CreatedAt = 0;
};

class PetBattlePvpStateStore
{
public:
    typedef std::vector<uint64> QueueContainer;
    typedef std::map<uint64, PetBattlePvpProposal> ProposalMap;
    typedef std::map<uint64, PetBattlePvpChallenge> ChallengeMap;
    typedef std::map<uint64, uint64> ChallengeTargetMap;

    QueueContainer& Queue() { return _queue; }
    ProposalMap& Proposals() { return _proposals; }
    ChallengeMap& ChallengesByTarget() { return _challengesByTarget; }
    ChallengeTargetMap& ChallengeTargetByChallenger() { return _challengeTargetByChallenger; }

    void RemoveQueueEntry(uint64 playerGuid);
    void AddQueueEntry(uint64 playerGuid);
    void AddProposalPair(uint64 playerGuid, uint64 opponentGuid, time_t createdAt);
    void ClearProposal(uint64 playerGuid);
    void AddChallenge(PetBattlePvpChallenge const& challenge);
    void EraseChallengeForTarget(uint64 targetGuid);
    ChallengeMap::iterator FindChallengeForPlayer(uint64 playerGuid);

private:
    QueueContainer _queue;
    ProposalMap _proposals;
    ChallengeMap _challengesByTarget;
    ChallengeTargetMap _challengeTargetByChallenger;
};

PetBattlePvpStateStore& GetPetBattlePvpStateStore();

#endif
