/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "BattlePetPvpState.h"

#include <algorithm>

void PetBattlePvpStateStore::RemoveQueueEntry(uint64 playerGuid)
{
    _queue.erase(std::remove(_queue.begin(), _queue.end(), playerGuid), _queue.end());
}

void PetBattlePvpStateStore::AddQueueEntry(uint64 playerGuid)
{
    _queue.push_back(playerGuid);
}

void PetBattlePvpStateStore::AddProposalPair(uint64 playerGuid, uint64 opponentGuid, time_t createdAt)
{
    _proposals[playerGuid] = { opponentGuid, false, createdAt };
    _proposals[opponentGuid] = { playerGuid, false, createdAt };
}

void PetBattlePvpStateStore::ClearProposal(uint64 playerGuid)
{
    ProposalMap::iterator proposalItr = _proposals.find(playerGuid);
    if (proposalItr == _proposals.end())
        return;

    uint64 const opponentGuid = proposalItr->second.OpponentGuid;
    _proposals.erase(playerGuid);
    _proposals.erase(opponentGuid);
}

void PetBattlePvpStateStore::AddChallenge(PetBattlePvpChallenge const& challenge)
{
    _challengesByTarget[challenge.TargetGuid] = challenge;
    _challengeTargetByChallenger[challenge.ChallengerGuid] = challenge.TargetGuid;
}

void PetBattlePvpStateStore::EraseChallengeForTarget(uint64 targetGuid)
{
    ChallengeMap::iterator challengeItr = _challengesByTarget.find(targetGuid);
    if (challengeItr == _challengesByTarget.end())
        return;

    _challengeTargetByChallenger.erase(challengeItr->second.ChallengerGuid);
    _challengesByTarget.erase(challengeItr);
}

PetBattlePvpStateStore::ChallengeMap::iterator PetBattlePvpStateStore::FindChallengeForPlayer(uint64 playerGuid)
{
    ChallengeMap::iterator challengeItr = _challengesByTarget.find(playerGuid);
    if (challengeItr != _challengesByTarget.end())
        return challengeItr;

    ChallengeTargetMap::iterator targetItr = _challengeTargetByChallenger.find(playerGuid);
    if (targetItr == _challengeTargetByChallenger.end())
        return _challengesByTarget.end();

    return _challengesByTarget.find(targetItr->second);
}

PetBattlePvpStateStore& GetPetBattlePvpStateStore()
{
    static PetBattlePvpStateStore petBattlePvpStateStore;
    return petBattlePvpStateStore;
}
