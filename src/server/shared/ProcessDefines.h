/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef _PROCESSDEFINES_H
#define _PROCESSDEFINES_H

#include "Define.h"

enum ServerProcessTypes
{
    SERVER_PROCESS_AUTHSERVER = 0,
    SERVER_PROCESS_WORLDSERVER = 1,

    NUM_SERVER_PROCESS_TYPES
};

namespace SkyFire::Impl
{
    struct CurrentServerProcessHolder
    {
        static ServerProcessTypes type() { return _type; }
        static ServerProcessTypes _type;
    };
}

#define THIS_SERVER_PROCESS (SkyFire::Impl::CurrentServerProcessHolder::type())

#endif
