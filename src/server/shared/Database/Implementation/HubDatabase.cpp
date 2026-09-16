/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "HubDatabase.h"

void HubDatabaseConnection::DoPrepareStatements()
{
    if (!m_reconnecting)
        m_stmts.resize(MAX_HUBDATABASE_STATEMENTS);

    PrepareStatement(HUB_SEL_ACTIVE_ROUTING_NODES,
        "SELECT id, node_key, name, node_type, route_address, route_port, realm_id, region, "
        "priority, weight, capacity, current_load, fallback_node_id, maintenance, status "
        "FROM hub_nodes WHERE enabled = 1 ORDER BY priority, id", CONNECTION_SYNCH);
    PrepareStatement(HUB_SEL_ADMIN_BY_USERNAME,
        "SELECT id, username, password_hash, access_flags, enabled FROM hub_admins WHERE username = ? LIMIT 1",
        CONNECTION_SYNCH);
    PrepareStatement(HUB_SEL_ADMINS,
        "SELECT id, username, access_flags, enabled, last_login_at FROM hub_admins ORDER BY username",
        CONNECTION_SYNCH);
    PrepareStatement(HUB_UPD_NODE_HEARTBEAT,
        "UPDATE hub_nodes SET status = ?, current_load = ?, last_heartbeat_at = CURRENT_TIMESTAMP "
        "WHERE node_key = ?", CONNECTION_ASYNC);
}
