/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef ZONE_SCRIPT_H_
#define ZONE_SCRIPT_H_

#include "Common.h"
#include "Creature.h"

class GameObject;

class ZoneScript
{
    public:
        ZoneScript() { }
        virtual ~ZoneScript() { }

        virtual uint32 GetCreatureEntry(uint32 /*guidlow*/, CreatureData const* data) { return data->id; }
        virtual uint32 GetGameObjectEntry(uint32 /*guidlow*/, uint32 entry) { return entry; }

        virtual void OnCreatureCreate(Creature* ) { }
        virtual void OnCreatureRemove(Creature* ) { }

        virtual void OnGameObjectCreate(GameObject* ) { }
        virtual void OnGameObjectRemove(GameObject* ) { }

        virtual void OnUnitDeath(Unit*) { }

        //All-purpose data storage 64 bit
        virtual uint64 GetData64(uint32 /*DataId*/) const { return 0; }
        virtual void SetData64(uint32 /*DataId*/, uint64 /*Value*/) { }

        //All-purpose data storage 32 bit
        virtual uint32 GetData(uint32 /*DataId*/) const { return 0; }
        virtual void SetData(uint32 /*DataId*/, uint32 /*Value*/) { }

        virtual void ProcessEvent(WorldObject* /*obj*/, uint32 /*eventId*/) { }
};

#endif
