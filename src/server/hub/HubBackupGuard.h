/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#ifndef SKYFIRE_HUB_BACKUP_GUARD_H
#define SKYFIRE_HUB_BACKUP_GUARD_H
#include <mutex>
#include <atomic>
#include "Database/DatabaseEnv.h"
// Serialize restore admission with local service starts and direct account operations.
inline std::mutex HubBackupAdmission;
inline std::atomic<bool> HubNodeRestartActive{false};
inline bool HubBackupMaintenance()
{
    auto row = HubDatabase.Query(HubDatabase.GetPreparedStatement(HUB_SEL_BACKUP_WORKER));
    return !row || row->Fetch()[4].GetBool(); // Fail closed if recovery state cannot be read.
}
#endif
