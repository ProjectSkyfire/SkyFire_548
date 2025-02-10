/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/// \addtogroup Skyfired
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

    void SetNoUseConfigDatabaseInfo(bool noUseConfigDatabaseInfo) { _noUseConfigDatabaseInfo = noUseConfigDatabaseInfo; }
    void SetDatabaseHost(const char* db_Host) { _dbHost = db_Host; }
    void SetDatabasePort(const char* db_Port) { _dbPort = db_Port; }
    void SetDatabaseUser(const char* db_User) { _dbUser = db_User; }
    void SetDatabasePassword(const char* db_Password) { _dbPassword = db_Password; }
    void SetDatabaseAuth(const char* authDB) { _authDB = authDB; }
    void SetDatabaseChars(const char* charactersDB) { _charactersDB = charactersDB; }
    void SetDatabaseWorld(const char* worldDB) { _worldDB = worldDB; }

private:
    bool _StartDB();
    void _StopDB();

    void ClearOnlineAccounts();

    bool _noUseConfigDatabaseInfo = false;
    const char* _dbHost;
    const char* _dbPort;
    const char* _dbUser;
    const char* _dbPassword;
    const char* _authDB;
    const char* _charactersDB;
    const char* _worldDB;
};

#define sMaster ACE_Singleton<Master, ACE_Null_Mutex>::instance()

#endif

/// @}
