/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_CREATUREAIFACTORY_H
#define SKYFIRE_CREATUREAIFACTORY_H

//#include "Policies/Singleton.h"
#include "ObjectRegistry.h"
#include "FactoryHolder.h"
#include "GameObjectAI.h"

struct SelectableAI : public FactoryHolder<CreatureAI>, public Permissible<Creature>
{
    SelectableAI(const char* id) : FactoryHolder<CreatureAI>(id) { }
};

template<class REAL_AI>
struct CreatureAIFactory : public SelectableAI
{
    CreatureAIFactory(const char* name) : SelectableAI(name) { }

    CreatureAI* Create(void*) const;

    int Permit(const Creature* c) const { return REAL_AI::Permissible(c); }
};

template<class REAL_AI>
inline CreatureAI*
CreatureAIFactory<REAL_AI>::Create(void* data) const
{
    Creature* creature = reinterpret_cast<Creature*>(data);
    return (new REAL_AI(creature));
}

typedef FactoryHolder<CreatureAI> CreatureAICreator;
typedef FactoryHolder<CreatureAI>::FactoryHolderRegistry CreatureAIRegistry;
typedef FactoryHolder<CreatureAI>::FactoryHolderRepository CreatureAIRepository;

//GO
struct SelectableGameObjectAI : public FactoryHolder<GameObjectAI>, public Permissible<GameObject>
{
    SelectableGameObjectAI(const char* id) : FactoryHolder<GameObjectAI>(id) { }
};

template<class REAL_GO_AI>
struct GameObjectAIFactory : public SelectableGameObjectAI
{
    GameObjectAIFactory(const char* name) : SelectableGameObjectAI(name) { }

    GameObjectAI* Create(void*) const;

    int Permit(const GameObject* g) const { return REAL_GO_AI::Permissible(g); }
};

template<class REAL_GO_AI>
inline GameObjectAI*
GameObjectAIFactory<REAL_GO_AI>::Create(void* data) const
{
    GameObject* go = reinterpret_cast<GameObject*>(data);
    return (new REAL_GO_AI(go));
}

typedef FactoryHolder<GameObjectAI> GameObjectAICreator;
typedef FactoryHolder<GameObjectAI>::FactoryHolderRegistry GameObjectAIRegistry;
typedef FactoryHolder<GameObjectAI>::FactoryHolderRepository GameObjectAIRepository;
#endif
