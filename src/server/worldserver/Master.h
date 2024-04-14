/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/// \addtogroup Trinityd
/// @{
/// \file

#ifndef _MASTER_H
#define _MASTER_H

#include "Common.h"

/// Start the server
class Master
{
public:
    int Run();

private:
    bool _StartDB();
    void _StopDB();

    void ClearOnlineAccounts();
};

#define sMaster ACE_Singleton<Master, ACE_Null_Mutex>::instance()

#endif

/// @}
