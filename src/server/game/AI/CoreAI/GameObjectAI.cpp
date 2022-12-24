/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "GameObjectAI.h"

//GameObjectAI::GameObjectAI(GameObject* g) : go(g) { }
int GameObjectAI::Permissible(const GameObject* go)
{
    if (go->GetAIName() == "GameObjectAI")
        return PERMIT_BASE_SPECIAL;
    return PERMIT_BASE_NO;
}

NullGameObjectAI::NullGameObjectAI(GameObject* g) : GameObjectAI(g) { }
