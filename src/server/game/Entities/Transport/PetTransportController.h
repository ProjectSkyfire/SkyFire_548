/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRESERVER_PET_TRANSPORT_CONTROLLER_H
#define SKYFIRESERVER_PET_TRANSPORT_CONTROLLER_H

class Pet;
class Player;
class Transport;

namespace Skyfire
{
namespace PetTransport
{
    bool AttachHunterPetToOwnerTransport(Player* owner, Pet* pet, Transport* transport);
    bool BoardOwnerHunterPet(Player* owner, Transport* transport);
    void RemoveOwnerHunterPet(Player* owner, Transport* transport);
}
}

#endif
