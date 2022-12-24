/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SF_UPDATEFIELDFLAGS_H
#define SF_UPDATEFIELDFLAGS_H

#include "UpdateFields.h"
#include "Define.h"

enum UpdatefieldFlags
{
    UF_FLAG_NONE                = 0x000,
    UF_FLAG_PUBLIC              = 0x001,
    UF_FLAG_PRIVATE             = 0x002,
    UF_FLAG_OWNER               = 0x004,
    UF_FLAG_UNUSED              = 0x008,
    UF_FLAG_ITEM_OWNER          = 0x010,
    UF_FLAG_PARTY_MEMBER        = 0x020,
    UF_FLAG_SPECIAL_INFO        = 0x040,
    UF_FLAG_VIEWER_DEPENDENT    = 0x080,
    UF_FLAG_URGENT              = 0x100,
    UF_FLAG_URGENT_SELF_ONLY    = 0x200
};

extern uint32 ItemUpdateFieldFlags[CONTAINER_END];
extern uint32 UnitUpdateFieldFlags[PLAYER_END];
extern uint32 GameObjectUpdateFieldFlags[GAMEOBJECT_END];
extern uint32 DynamicObjectUpdateFieldFlags[DYNAMICOBJECT_END];
extern uint32 CorpseUpdateFieldFlags[CORPSE_END];
extern uint32 AreaTriggerUpdateFieldFlags[AREATRIGGER_END];

#endif // _UPDATEFIELDFLAGS_H
