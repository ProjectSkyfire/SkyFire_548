/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "PetTransportController.h"
#include "Map.h"
#include "MoveSplineInit.h"
#include "Pet.h"
#include "PetTransportSupport.h"
#include "Player.h"
#include "Transport.h"

namespace
{
    float const MaxPassengerOffset = 250.0f;
    uint32 const TransportExitGraceDuration = 3 * IN_MILLISECONDS;

    bool ShouldMirrorHunterPetOnTransport(Player const* owner, Pet const* pet)
    {
        if (!owner || !pet)
            return false;

        return Skyfire::PetTransport::ShouldMirrorOwnerPet(pet->getPetType() == PetType::HUNTER_PET,
            pet->isControlled(), pet->IsAlive(), pet->GetOwnerGUID() == owner->GetGUID());
    }

    bool NeedsPassengerOffsetRefresh(Pet const* pet)
    {
        return !Skyfire::PetTransport::IsPassengerOffsetWithinLimit(
            pet->GetTransOffsetX(), pet->GetTransOffsetY(), pet->GetTransOffsetZ(), MaxPassengerOffset);
    }

    Skyfire::PetTransport::PassengerOffset CalculatePetTransportOffset(Player* owner, Pet* pet, Transport* transport)
    {
        if (owner->GetTransport() == transport)
            return Skyfire::PetTransport::CalculateFollowerPosition(
                owner->GetTransOffsetX(), owner->GetTransOffsetY(), owner->GetTransOffsetZ(), owner->GetTransOffsetO(),
                owner->GetObjectSize(), pet->GetObjectSize(), PET_FOLLOW_DIST, pet->GetFollowAngle());

        Skyfire::PetTransport::PassengerOffset offset = Skyfire::PetTransport::CalculateFollowerPosition(
            owner->GetPositionX(), owner->GetPositionY(), owner->GetPositionZ(), owner->GetOrientation(),
            owner->GetObjectSize(), pet->GetObjectSize(), PET_FOLLOW_DIST, pet->GetFollowAngle());

        transport->CalculatePassengerOffset(offset.X, offset.Y, offset.Z, &offset.O);
        return offset;
    }

    void RelocatePetToTransportOffset(Pet* pet, Transport* transport, Skyfire::PetTransport::PassengerOffset const& offset)
    {
        pet->m_movementInfo.transport.pos.Relocate(offset.X, offset.Y, offset.Z, offset.O);

        float x = offset.X;
        float y = offset.Y;
        float z = offset.Z;
        float o = offset.O;
        transport->CalculatePassengerPosition(x, y, z, &o);

        if (pet->IsInWorld())
            pet->GetMap()->CreatureRelocation(pet, x, y, z, o, false);
        else
            pet->Relocate(x, y, z, o);

        pet->SetHomePosition(x, y, z, o);
        pet->SetTransportHomePosition(pet->m_movementInfo.transport.pos);
    }

    void SendTransportEnterSpline(Pet* pet, Skyfire::PetTransport::PassengerOffset const& offset)
    {
        Movement::MoveSplineInit init(pet);
        init.DisableTransportPathTransformations();
        init.MoveTo(offset.X, offset.Y, offset.Z, false, true);
        init.SetFacing(offset.O);
        init.SetTransportEnter();
        init.Launch();
    }

    void ResumeFollowerMotion(Player* owner, Pet* pet, Transport* transport, bool forceRestart)
    {
        if (!Skyfire::PetTransport::ShouldResumePetFollowOnTransport(owner->GetTransport() == transport,
                pet->GetTransport() == transport, pet->IsInWorld()))
            return;

        bool const clearMountedStun = Skyfire::PetTransport::ShouldClearMountedPetStunForTransport(owner->IsMounted(),
            owner->GetTransport() == transport, pet->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED));
        if (clearMountedStun)
            pet->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);

        bool wasFollowing = false;
        if (CharmInfo* charmInfo = pet->GetCharmInfo())
        {
            wasFollowing = charmInfo->IsFollowing();
            charmInfo->SetIsAtStay(false);
            charmInfo->SetIsReturning(false);
            charmInfo->SetIsFollowing(true);
        }

        if (forceRestart || clearMountedStun || !wasFollowing)
        {
            pet->GetMotionMaster()->Clear();
            pet->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, pet->GetFollowAngle());
        }
    }
}

namespace Skyfire
{
namespace PetTransport
{
    bool AttachHunterPetToOwnerTransport(Player* owner, Pet* pet, Transport* transport)
    {
        if (!owner || !pet || !transport)
            return false;

        if (!ShouldAttachHunterPetToOwnerTransport(owner->GetTransport() == transport,
                pet->getPetType() == PetType::HUNTER_PET, pet->isControlled(), pet->IsAlive(),
                pet->GetOwnerGUID() == owner->GetGUID(), pet->GetMap() == owner->GetMap()))
            return false;

        bool const alreadyOnTransport = pet->GetTransport() == transport;
        bool const offsetRefreshed = ShouldRefreshPetTransportOffset(alreadyOnTransport,
            alreadyOnTransport && NeedsPassengerOffsetRefresh(pet));
        if (!offsetRefreshed)
        {
            ResumeFollowerMotion(owner, pet, transport, false);
            return false;
        }

        PassengerOffset const offset = CalculatePetTransportOffset(owner, pet, transport);
        if (!IsPassengerOffsetWithinLimit(offset.X, offset.Y, offset.Z, MaxPassengerOffset))
            return false;

        if (Transport* oldTransport = pet->GetTransport())
            if (oldTransport != transport)
                oldTransport->RemovePassenger(pet);

        RelocatePetToTransportOffset(pet, transport, offset);

        if (!alreadyOnTransport)
            transport->AddPassenger(pet);

        pet->ClearTransportExitGrace();

        bool const petOnOwnerTransport = pet->GetTransport() == transport;
        if (ShouldSendTransportEnterSpline(pet->IsInWorld(), petOnOwnerTransport, offsetRefreshed))
            SendTransportEnterSpline(pet, offset);

        ResumeFollowerMotion(owner, pet, transport, true);
        return petOnOwnerTransport;
    }

    bool BoardOwnerHunterPet(Player* owner, Transport* transport)
    {
        if (!owner || !transport)
            return false;

        Pet* pet = owner->GetPet();
        if (!ShouldMirrorHunterPetOnTransport(owner, pet) || !pet->IsInWorld())
            return false;

        return AttachHunterPetToOwnerTransport(owner, pet, transport);
    }

    void RemoveOwnerHunterPet(Player* owner, Transport* transport)
    {
        if (!owner || !transport)
            return;

        Pet* pet = owner->GetPet();
        if (!ShouldMirrorHunterPetOnTransport(owner, pet) || pet->GetTransport() != transport)
            return;

        float x, y, z, o;
        if (ShouldPlacePetNearOwnerOnTransportExit(owner->GetTransport() == transport))
        {
            owner->GetClosePoint(x, y, z, pet->GetObjectSize(), PET_FOLLOW_DIST, pet->GetFollowAngle());
            o = owner->GetOrientation();
        }
        else
        {
            pet->m_movementInfo.transport.pos.GetPosition(x, y, z, o);
            transport->CalculatePassengerPosition(x, y, z, &o);
        }

        transport->RemovePassenger(pet);
        pet->StartTransportExitGrace(TransportExitGraceDuration);
        pet->GetMap()->CreatureRelocation(pet, x, y, z, o);
        pet->SetHomePosition(x, y, z, o);
        pet->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, pet->GetFollowAngle());
    }
}
}
