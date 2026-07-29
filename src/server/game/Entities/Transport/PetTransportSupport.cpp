/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "PetTransportSupport.h"
#include <cmath>

namespace Skyfire
{
namespace PetTransport
{
    namespace
    {
        float ClampPositive(float value)
        {
            return value > 0.0f ? value : 0.0f;
        }
    }

    bool ShouldMirrorOwnerPet(bool isHunterPet, bool isControlled, bool isAlive, bool ownerMatches)
    {
        return isHunterPet && isControlled && isAlive && ownerMatches;
    }

    bool IsPassengerOffsetWithinLimit(float x, float y, float z, float limit)
    {
        float const maxOffset = ClampPositive(limit);
        return std::isfinite(x) && std::isfinite(y) && std::isfinite(z)
            && std::fabs(x) <= maxOffset && std::fabs(y) <= maxOffset && std::fabs(z) <= maxOffset;
    }

    bool ShouldRefreshPetTransportOffset(bool petAlreadyOnOwnerTransport, bool petPassengerOffsetInvalid)
    {
        return !petAlreadyOnOwnerTransport || petPassengerOffsetInvalid;
    }

    bool ShouldUseTransportLocalPetFollow(bool followerIsPet, bool targetIsOwner, bool followerHasTransport,
        bool targetOnSameTransport, bool hasFollowOffset)
    {
        return followerIsPet && targetIsOwner && followerHasTransport && targetOnSameTransport && hasFollowOffset;
    }

    bool ShouldMoveTransportPetToFollowOffset(float currentX, float currentY, float currentZ,
        float desiredX, float desiredY, float desiredZ, float tolerance)
    {
        if (!std::isfinite(currentX) || !std::isfinite(currentY) || !std::isfinite(currentZ)
            || !std::isfinite(desiredX) || !std::isfinite(desiredY) || !std::isfinite(desiredZ))
            return false;

        float const allowed = ClampPositive(tolerance);
        float const dx = currentX - desiredX;
        float const dy = currentY - desiredY;
        float const dz = currentZ - desiredZ;
        return dx * dx + dy * dy + dz * dz > allowed * allowed;
    }

    bool ShouldResumePetFollowOnTransport(bool ownerHasTransport, bool petOnOwnerTransport, bool petInWorld)
    {
        return ownerHasTransport && petOnOwnerTransport && petInWorld;
    }

    bool ShouldClearMountedPetStunForTransport(bool ownerMounted, bool ownerHasTransport, bool petStunned)
    {
        return ownerMounted && ownerHasTransport && petStunned;
    }

    bool ShouldSendTransportEnterSpline(bool petInWorld, bool petOnOwnerTransport, bool offsetRefreshed)
    {
        return petInWorld && petOnOwnerTransport && offsetRefreshed;
    }

    bool ShouldApplySplineGroundClamp(bool hasTransport, bool isCreature, bool isFalling)
    {
        return isCreature && !isFalling && !hasTransport;
    }

    bool ShouldTryBoardPetBeforeOwnerRangeRemoval(bool ownerHasTransport, bool petExists, bool petOnOwnerTransport)
    {
        return ownerHasTransport && petExists && !petOnOwnerTransport;
    }

    bool ShouldRemovePetForOwnerRange(bool petExists, bool petPossessed, bool petOnOwnerTransport,
        bool isWithinVisibilityRange, bool transportExitGraceActive)
    {
        return petExists && !petPossessed && !petOnOwnerTransport && !isWithinVisibilityRange && !transportExitGraceActive;
    }

    bool ShouldTemporarilyUnsummonMountedPet(bool inBattleground, bool hasFlyingMountSpeedAura)
    {
        return inBattleground || hasFlyingMountSpeedAura;
    }

    bool ShouldAttachHunterPetToOwnerTransport(bool ownerHasTransport, bool petIsHunterPet, bool petIsControlled,
        bool petIsAlive, bool ownerMatches, bool sameMap)
    {
        return ownerHasTransport && sameMap && ShouldMirrorOwnerPet(petIsHunterPet, petIsControlled, petIsAlive, ownerMatches);
    }

    bool ShouldReattachPetDuringOwnerTransportUpdate(bool ownerHasTransport, bool petIsHunterPet,
        bool petIsControlled, bool petIsAlive, bool ownerMatches, bool petOnOwnerTransport)
    {
        return ownerHasTransport && !petOnOwnerTransport
            && ShouldMirrorOwnerPet(petIsHunterPet, petIsControlled, petIsAlive, ownerMatches);
    }

    bool ShouldPlacePetNearOwnerOnTransportExit(bool ownerStillOnTransport)
    {
        return !ownerStillOnTransport;
    }

    PassengerOffset CalculateFollowerPosition(float ownerX, float ownerY, float ownerZ, float ownerO,
        float ownerObjectSize, float petObjectSize, float followDistance, float followAngle)
    {
        float const distance = ClampPositive(ownerObjectSize) + ClampPositive(petObjectSize) + ClampPositive(followDistance);
        float const angle = ownerO + followAngle;

        PassengerOffset offset;
        offset.X = ownerX + std::cos(angle) * distance;
        offset.Y = ownerY + std::sin(angle) * distance;
        offset.Z = ownerZ;
        offset.O = ownerO;
        return offset;
    }
}
}
