/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "PetTransportSupport.h"

#include <cmath>
#include <iostream>

namespace
{
    bool NearlyEqual(float left, float right)
    {
        return std::fabs(left - right) < 0.001f;
    }

    bool Expect(bool condition, char const* message)
    {
        if (condition)
            return true;

        std::cerr << message << '\n';
        return false;
    }

    bool TestFollowerPositionUsesOwnerFrame()
    {
        float const halfPi = 1.57079632679f;
        Skyfire::PetTransport::PassengerOffset const offset = Skyfire::PetTransport::CalculateFollowerPosition(
            10.0f, 20.0f, 3.0f, 0.0f, 0.5f, 0.75f, 1.0f, halfPi);

        bool passed = true;
        passed &= Expect(NearlyEqual(offset.X, 10.0f), "Pet follower X should be calculated from owner X");
        passed &= Expect(NearlyEqual(offset.Y, 22.25f), "Pet follower Y should include follow distance and object sizes");
        passed &= Expect(NearlyEqual(offset.Z, 3.0f), "Pet follower Z should stay on the owner's plane");
        passed &= Expect(NearlyEqual(offset.O, 0.0f), "Pet follower orientation should match owner orientation");
        return passed;
    }

    bool TestFollowerPositionClampsNegativeDistances()
    {
        Skyfire::PetTransport::PassengerOffset const offset = Skyfire::PetTransport::CalculateFollowerPosition(
            4.0f, 6.0f, 8.0f, 0.0f, -1.0f, -2.0f, -3.0f, 0.0f);

        bool passed = true;
        passed &= Expect(NearlyEqual(offset.X, 4.0f), "Negative sizes and follow distance should not pull the pet through the owner");
        passed &= Expect(NearlyEqual(offset.Y, 6.0f), "Negative sizes and follow distance should clamp to zero distance");
        passed &= Expect(NearlyEqual(offset.Z, 8.0f), "Clamped offset should keep the owner transport Z");
        return passed;
    }

    bool TestOnlyControlledLivingOwnerHunterPetsMirrorTransport()
    {
        bool passed = true;

        passed &= Expect(Skyfire::PetTransport::ShouldMirrorOwnerPet(true, true, true, true),
            "Controlled living hunter pets owned by the player should mirror transport state");
        passed &= Expect(!Skyfire::PetTransport::ShouldMirrorOwnerPet(false, true, true, true),
            "Non-hunter pets should not be forced onto the owner's transport");
        passed &= Expect(!Skyfire::PetTransport::ShouldMirrorOwnerPet(true, false, true, true),
            "Uncontrolled pets should not be forced onto the owner's transport");
        passed &= Expect(!Skyfire::PetTransport::ShouldMirrorOwnerPet(true, true, false, true),
            "Dead pets should not be forced onto the owner's transport");
        passed &= Expect(!Skyfire::PetTransport::ShouldMirrorOwnerPet(true, true, true, false),
            "Pets owned by another player should not mirror this owner's transport state");

        return passed;
    }

    bool TestPassengerOffsetLimitUsesAbsoluteCoordinates()
    {
        bool passed = true;

        passed &= Expect(Skyfire::PetTransport::IsPassengerOffsetWithinLimit(-50.0f, 50.0f, 0.0f, 50.0f),
            "Passenger offsets at the signed limit should be accepted");
        passed &= Expect(!Skyfire::PetTransport::IsPassengerOffsetWithinLimit(-51.0f, 0.0f, 0.0f, 50.0f),
            "Oversized negative passenger X offsets should be rejected");
        passed &= Expect(!Skyfire::PetTransport::IsPassengerOffsetWithinLimit(0.0f, -51.0f, 0.0f, 50.0f),
            "Oversized negative passenger Y offsets should be rejected");
        passed &= Expect(!Skyfire::PetTransport::IsPassengerOffsetWithinLimit(0.0f, 0.0f, -51.0f, 50.0f),
            "Oversized negative passenger Z offsets should be rejected");

        return passed;
    }

    bool TestTransportedPetsAreNotRemovedAsOutOfRange()
    {
        bool passed = true;

        passed &= Expect(Skyfire::PetTransport::ShouldTryBoardPetBeforeOwnerRangeRemoval(true, true, false),
            "Out-of-range pets should be reattached before removal when the owner is on a transport");
        passed &= Expect(!Skyfire::PetTransport::ShouldTryBoardPetBeforeOwnerRangeRemoval(false, true, false),
            "Pets should not be reattached without an owner transport");
        passed &= Expect(!Skyfire::PetTransport::ShouldTryBoardPetBeforeOwnerRangeRemoval(true, false, false),
            "Missing pets should not trigger transport reattachment");
        passed &= Expect(!Skyfire::PetTransport::ShouldTryBoardPetBeforeOwnerRangeRemoval(true, true, true),
            "Pets already riding the owner's transport do not need another reattach attempt");

        passed &= Expect(!Skyfire::PetTransport::ShouldRemovePetForOwnerRange(true, false, false, true),
            "Pets inside visibility range should be kept");
        passed &= Expect(!Skyfire::PetTransport::ShouldRemovePetForOwnerRange(true, true, false, false),
            "Possessed pets should be kept even when range checks fail");
        passed &= Expect(!Skyfire::PetTransport::ShouldRemovePetForOwnerRange(true, false, true, false),
            "Pets on the owner's transport should not be removed by world-space range checks");
        passed &= Expect(Skyfire::PetTransport::ShouldRemovePetForOwnerRange(true, false, false, false),
            "Unpossessed pets off transport and outside visibility range should still be removed");

        return passed;
    }

    bool TestMountedPetTemporaryUnsummonRules()
    {
        bool passed = true;

        passed &= Expect(!Skyfire::PetTransport::ShouldTemporarilyUnsummonMountedPet(false, false),
            "Ground-mounted pets should stay active so they can ride transports");
        passed &= Expect(Skyfire::PetTransport::ShouldTemporarilyUnsummonMountedPet(false, true),
            "Flying mounts should still temporarily unsummon pets");
        passed &= Expect(Skyfire::PetTransport::ShouldTemporarilyUnsummonMountedPet(true, false),
            "Battleground mount rules should still temporarily unsummon pets");

        return passed;
    }

    bool TestLoadedHunterPetsAttachToOwnerTransport()
    {
        bool passed = true;

        passed &= Expect(Skyfire::PetTransport::ShouldAttachHunterPetToOwnerTransport(true, true, true, true, true, true),
            "Loaded controlled hunter pets should attach when their owner is already on a transport");
        passed &= Expect(!Skyfire::PetTransport::ShouldAttachHunterPetToOwnerTransport(false, true, true, true, true, true),
            "Loaded pets should not attach when the owner is not on a transport");
        passed &= Expect(!Skyfire::PetTransport::ShouldAttachHunterPetToOwnerTransport(true, true, true, true, true, false),
            "Loaded pets should not attach across maps");

        return passed;
    }

    bool TestPetUpdateReattachesBeforeRangeRemoval()
    {
        bool passed = true;

        passed &= Expect(Skyfire::PetTransport::ShouldReattachPetDuringOwnerTransportUpdate(true, true, true, true, true, false),
            "A living controlled hunter pet should reattach during pet update before transport range cleanup");
        passed &= Expect(!Skyfire::PetTransport::ShouldReattachPetDuringOwnerTransportUpdate(true, true, true, true, true, true),
            "A pet already riding the owner's transport does not need update-time reattachment");
        passed &= Expect(!Skyfire::PetTransport::ShouldReattachPetDuringOwnerTransportUpdate(false, true, true, true, true, false),
            "Pets should not reattach during update when the owner is off transport");
        passed &= Expect(!Skyfire::PetTransport::ShouldReattachPetDuringOwnerTransportUpdate(true, false, true, true, true, false),
            "Non-hunter pets should not be forced through hunter pet transport reattachment");

        return passed;
    }

    bool TestPetExitPositionFollowsOwnerAfterTransportLeave()
    {
        bool passed = true;

        passed &= Expect(!Skyfire::PetTransport::ShouldPlacePetNearOwnerOnTransportExit(true),
            "Pets should keep their deck position while the owner is still on the transport");
        passed &= Expect(Skyfire::PetTransport::ShouldPlacePetNearOwnerOnTransportExit(false),
            "Pets should be placed near the owner after the owner has left the transport");

        return passed;
    }

    bool TestTransportedPetsKeepFollowerMotion()
    {
        bool passed = true;

        passed &= Expect(!Skyfire::PetTransport::ShouldRefreshPetTransportOffset(true, false),
            "Pets already on the owner's transport with a valid offset should not be snapped every owner movement update");
        passed &= Expect(Skyfire::PetTransport::ShouldRefreshPetTransportOffset(true, true),
            "Pets already on the owner's transport should be refreshed when their stored offset is invalid");
        passed &= Expect(Skyfire::PetTransport::ShouldRefreshPetTransportOffset(false, false),
            "Pets not yet on the owner's transport need an initial transport offset");

        passed &= Expect(Skyfire::PetTransport::ShouldResumePetFollowOnTransport(true, true, true),
            "Visible pets riding the owner's transport should run follow movement in the transport frame");
        passed &= Expect(!Skyfire::PetTransport::ShouldResumePetFollowOnTransport(true, false, true),
            "Pets that failed to attach to the owner's transport cannot resume transport follow movement");
        passed &= Expect(!Skyfire::PetTransport::ShouldResumePetFollowOnTransport(true, true, false),
            "Pets not yet in world should wait for their create object movement data");

        return passed;
    }

    bool TestTransportedPetFollowUsesLocalDeckFrame()
    {
        bool passed = true;

        passed &= Expect(Skyfire::PetTransport::ShouldUseTransportLocalPetFollow(true, true, true, true, true),
            "Pets following their owner on the same transport should use a transport-local follow destination");
        passed &= Expect(!Skyfire::PetTransport::ShouldUseTransportLocalPetFollow(false, true, true, true, true),
            "Non-pet followers should keep normal follow movement");
        passed &= Expect(!Skyfire::PetTransport::ShouldUseTransportLocalPetFollow(true, false, true, true, true),
            "Pets should only use transport-local follow movement for their owner");
        passed &= Expect(!Skyfire::PetTransport::ShouldUseTransportLocalPetFollow(true, true, false, true, true),
            "Pets off transport should keep normal follow movement");
        passed &= Expect(!Skyfire::PetTransport::ShouldUseTransportLocalPetFollow(true, true, true, false, true),
            "Pets should not use local deck movement when the target is on another transport");

        passed &= Expect(Skyfire::PetTransport::ShouldMoveTransportPetToFollowOffset(1.0f, 2.0f, 3.0f, 1.0f, 2.0f, 3.5f, 0.25f),
            "Transported pets should refresh follow movement when only the local deck height changed");
        passed &= Expect(!Skyfire::PetTransport::ShouldMoveTransportPetToFollowOffset(1.0f, 2.0f, 3.0f, 1.05f, 2.05f, 3.05f, 0.25f),
            "Transported pets already close to the desired local offset should not be restarted every tick");

        return passed;
    }

    bool TestTransportExitGraceKeepsPetDuringWorldRelocation()
    {
        bool passed = true;

        passed &= Expect(!Skyfire::PetTransport::ShouldRemovePetForOwnerRange(true, false, false, false, true),
            "Pets just detached from a transport should survive the world-frame relocation tick");
        passed &= Expect(Skyfire::PetTransport::ShouldRemovePetForOwnerRange(true, false, false, false, false),
            "Out-of-range pets without transport exit grace should still be removed");

        return passed;
    }

    bool TestTransportBoardingClearsMountedPetStun()
    {
        bool passed = true;

        passed &= Expect(Skyfire::PetTransport::ShouldClearMountedPetStunForTransport(true, true, true),
            "A mounted owner's pet should be unstunned once both are riding a transport");
        passed &= Expect(!Skyfire::PetTransport::ShouldClearMountedPetStunForTransport(true, false, true),
            "Mounted pets should stay parked while the owner is mounted off transport");
        passed &= Expect(!Skyfire::PetTransport::ShouldClearMountedPetStunForTransport(false, true, true),
            "Unmounted owners do not need mounted pet stun cleanup");

        passed &= Expect(Skyfire::PetTransport::ShouldSendTransportEnterSpline(true, true, true),
            "Refreshing a visible transported pet should notify clients with a transport-enter movement");
        passed &= Expect(!Skyfire::PetTransport::ShouldSendTransportEnterSpline(true, true, false),
            "Already synchronized transported pets do not need another transport-enter movement");
        passed &= Expect(!Skyfire::PetTransport::ShouldSendTransportEnterSpline(false, true, true),
            "Pets loaded before entering world should rely on create object movement data");

        return passed;
    }
}

int main()
{
    bool passed = true;

    passed &= TestFollowerPositionUsesOwnerFrame();
    passed &= TestFollowerPositionClampsNegativeDistances();
    passed &= TestOnlyControlledLivingOwnerHunterPetsMirrorTransport();
    passed &= TestPassengerOffsetLimitUsesAbsoluteCoordinates();
    passed &= TestTransportedPetsAreNotRemovedAsOutOfRange();
    passed &= TestMountedPetTemporaryUnsummonRules();
    passed &= TestLoadedHunterPetsAttachToOwnerTransport();
    passed &= TestPetUpdateReattachesBeforeRangeRemoval();
    passed &= TestPetExitPositionFollowsOwnerAfterTransportLeave();
    passed &= TestTransportedPetsKeepFollowerMotion();
    passed &= TestTransportedPetFollowUsesLocalDeckFrame();
    passed &= TestTransportExitGraceKeepsPetDuringWorldRelocation();
    passed &= TestTransportBoardingClearsMountedPetStun();

    return passed ? 0 : 1;
}
