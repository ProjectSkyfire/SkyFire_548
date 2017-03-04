/*
 * Copyright (C) 2011-2017 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2017 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2017 MaNGOS <https://www.getmangos.eu/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
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
