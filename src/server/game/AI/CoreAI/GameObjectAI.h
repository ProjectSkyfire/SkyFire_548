/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_GAMEOBJECTAI_H
#define SKYFIRE_GAMEOBJECTAI_H

#include "Define.h"
#include <list>
#include "Object.h"
#include "GameObject.h"
#include "CreatureAI.h"

class GameObjectAI
{
    protected:
        GameObject* const go;
    public:
        explicit GameObjectAI(GameObject* g) : go(g) { }
        virtual ~GameObjectAI() { }

        virtual void UpdateAI(uint32 /*diff*/) { }

        virtual void InitializeAI() { Reset(); }

        virtual void Reset() { }

        // Pass parameters between AI
        virtual void DoAction(int32 /*param = 0 */) { }
        virtual void SetGUID(uint64 /*guid*/, int32 /*id = 0 */) { }
        virtual uint64 GetGUID(int32 /*id = 0 */) const { return 0; }

        static int Permissible(GameObject const* go);

        virtual bool GossipHello(Player* /*player*/) { return false; }
        virtual bool GossipSelect(Player* /*player*/, uint32 /*sender*/, uint32 /*action*/) { return false; }
        virtual bool GossipSelectCode(Player* /*player*/, uint32 /*sender*/, uint32 /*action*/, char const* /*code*/) { return false; }
        virtual bool QuestAccept(Player* /*player*/, Quest const* /*quest*/) { return false; }
        virtual bool QuestReward(Player* /*player*/, Quest const* /*quest*/, uint32 /*opt*/) { return false; }
        virtual uint32 GetDialogStatus(Player* /*player*/) { return 100; }
        virtual void Destroyed(Player* /*player*/, uint32 /*eventId*/) { }
        virtual uint32 GetData(uint32 /*id*/) const { return 0; }
        virtual void SetData64(uint32 /*id*/, uint64 /*value*/) { }
        virtual uint64 GetData64(uint32 /*id*/) const { return 0; }
        virtual void SetData(uint32 /*id*/, uint32 /*value*/) { }
        virtual void OnGameEvent(bool /*start*/, uint16 /*eventId*/) { }
        virtual void OnStateChanged(uint32 /*state*/, Unit* /*unit*/) { }
        virtual void EventInform(uint32 /*eventId*/) { }
};

class NullGameObjectAI : public GameObjectAI
{
    public:
        explicit NullGameObjectAI(GameObject* g);

        void UpdateAI(uint32 /*diff*/) { }

        static int Permissible(GameObject const* /*go*/) { return PERMIT_BASE_IDLE; }
};
#endif
