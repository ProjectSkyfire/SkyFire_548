/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRESERVER_PET_TRANSPORT_SUPPORT_H
#define SKYFIRESERVER_PET_TRANSPORT_SUPPORT_H

namespace Skyfire
{
namespace PetTransport
{
    struct PassengerOffset
    {
        float X;
        float Y;
        float Z;
        float O;
    };

    bool ShouldMirrorOwnerPet(bool isHunterPet, bool isControlled, bool isAlive, bool ownerMatches);
    bool IsPassengerOffsetWithinLimit(float x, float y, float z, float limit);
    bool ShouldRefreshPetTransportOffset(bool petAlreadyOnOwnerTransport, bool petPassengerOffsetInvalid);
    bool ShouldUseTransportLocalPetFollow(bool followerIsPet, bool targetIsOwner, bool followerHasTransport,
        bool targetOnSameTransport, bool hasFollowOffset);
    bool ShouldMoveTransportPetToFollowOffset(float currentX, float currentY, float currentZ,
        float desiredX, float desiredY, float desiredZ, float tolerance);
    bool ShouldResumePetFollowOnTransport(bool ownerHasTransport, bool petOnOwnerTransport, bool petInWorld);
    bool ShouldClearMountedPetStunForTransport(bool ownerMounted, bool ownerHasTransport, bool petStunned);
    bool ShouldSendTransportEnterSpline(bool petInWorld, bool petOnOwnerTransport, bool offsetRefreshed);
    bool ShouldTryBoardPetBeforeOwnerRangeRemoval(bool ownerHasTransport, bool petExists, bool petOnOwnerTransport);
    bool ShouldRemovePetForOwnerRange(bool petExists, bool petPossessed, bool petOnOwnerTransport,
        bool isWithinVisibilityRange, bool transportExitGraceActive = false);
    bool ShouldTemporarilyUnsummonMountedPet(bool inBattleground, bool hasFlyingMountSpeedAura);
    bool ShouldAttachHunterPetToOwnerTransport(bool ownerHasTransport, bool petIsHunterPet, bool petIsControlled,
        bool petIsAlive, bool ownerMatches, bool sameMap);
    bool ShouldReattachPetDuringOwnerTransportUpdate(bool ownerHasTransport, bool petIsHunterPet,
        bool petIsControlled, bool petIsAlive, bool ownerMatches, bool petOnOwnerTransport);
    bool ShouldPlacePetNearOwnerOnTransportExit(bool ownerStillOnTransport);

    PassengerOffset CalculateFollowerPosition(float ownerX, float ownerY, float ownerZ, float ownerO,
        float ownerObjectSize, float petObjectSize, float followDistance, float followAngle);
}
}

#endif
