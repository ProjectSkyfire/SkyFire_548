/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef _HUBDATABASE_H
#define _HUBDATABASE_H

#include "DatabaseWorkerPool.h"
#include "MySQLConnection.h"

class HubDatabaseConnection : public MySQLConnection
{
public:
    HubDatabaseConnection(MySQLConnectionInfo& connectionInfo) : MySQLConnection(connectionInfo) { }
    HubDatabaseConnection(Skyfire::DatabaseQueue* queue, MySQLConnectionInfo& connectionInfo)
        : MySQLConnection(queue, connectionInfo) { }

    void DoPrepareStatements();
};

typedef DatabaseWorkerPool<HubDatabaseConnection> HubDatabaseWorkerPool;

enum HubAdminAccessFlags : uint64
{
    HUB_ADMIN_ACCESS_VIEW_STATUS = 1ULL << 0,
    HUB_ADMIN_ACCESS_MANAGE_NODES = 1ULL << 1,
    HUB_ADMIN_ACCESS_OPERATE_NODES = 1ULL << 2,
    HUB_ADMIN_ACCESS_MANAGE_ADMINS = 1ULL << 3,
    HUB_ADMIN_ACCESS_REMOTE_LOGIN = 1ULL << 4,
    HUB_ADMIN_ACCESS_ALL_LOCAL = HUB_ADMIN_ACCESS_VIEW_STATUS | HUB_ADMIN_ACCESS_MANAGE_NODES |
        HUB_ADMIN_ACCESS_OPERATE_NODES | HUB_ADMIN_ACCESS_MANAGE_ADMINS,
    HUB_ADMIN_ACCESS_ALL = HUB_ADMIN_ACCESS_ALL_LOCAL | HUB_ADMIN_ACCESS_REMOTE_LOGIN
};

constexpr bool HubAdminCanLoginRemotely(uint64 accessFlags)
{
    return (accessFlags & HUB_ADMIN_ACCESS_REMOTE_LOGIN) != 0;
}

enum HubDatabaseStatements
{
    HUB_SEL_ACTIVE_ROUTING_NODES,
    HUB_SEL_ADMIN_BY_USERNAME,
    HUB_SEL_ADMINS,
    HUB_SEL_MANAGED_SERVICES,
    HUB_INS_ADMIN,
    HUB_UPSERT_WORLD_SERVICE,
    HUB_UPD_ADMIN_LAST_LOGIN,
    HUB_UPD_NODE_HEARTBEAT,
    MAX_HUBDATABASE_STATEMENTS
};

#endif
