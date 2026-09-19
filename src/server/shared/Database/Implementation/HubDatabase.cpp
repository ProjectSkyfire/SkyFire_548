/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "HubDatabase.h"

void HubDatabaseConnection::DoPrepareStatements()
{
    if (!m_reconnecting)
        m_stmts.resize(MAX_HUBDATABASE_STATEMENTS);

    PrepareStatement(HUB_INS_CONTROL_AUDIT,
        "INSERT INTO hub_control_audit(request_id,phase,actor,action,target,outcome) VALUES (?,?,?,?,?,?)", CONNECTION_SYNCH);
    PrepareStatement(HUB_SEL_CONTROL_AUDIT,
        "SELECT phase,actor,outcome FROM hub_control_audit WHERE request_id = ? ORDER BY id", CONNECTION_SYNCH);
    PrepareStatement(HUB_UPD_ADMIN_ACCESS,
        "UPDATE hub_admins SET access_flags = ? WHERE username = ?", CONNECTION_SYNCH);
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
    PrepareStatement(HUB_SEL_MANAGED_SERVICES,
        "SELECT service_key, name, executable_path, config_path, working_directory, enabled "
        "FROM hub_managed_services ORDER BY service_key", CONNECTION_SYNCH);
    PrepareStatement(HUB_UPSERT_WORLD_SERVICE,
        "INSERT INTO hub_managed_services (service_key,name,executable_path,config_path,working_directory,enabled) "
        "VALUES (?,?,?,?,?,1) ON DUPLICATE KEY UPDATE name=VALUES(name),executable_path=VALUES(executable_path),"
        "config_path=VALUES(config_path),working_directory=VALUES(working_directory)", CONNECTION_SYNCH);
    PrepareStatement(HUB_INS_ADMIN,
        "INSERT INTO hub_admins (username, password_hash, access_flags) VALUES (?, ?, ?)",
        CONNECTION_SYNCH);
    PrepareStatement(HUB_UPD_ADMIN_LAST_LOGIN,
        "UPDATE hub_admins SET last_login_at = CURRENT_TIMESTAMP WHERE username = ?",
        CONNECTION_ASYNC);
    PrepareStatement(HUB_UPD_NODE_HEARTBEAT,
        "UPDATE hub_nodes SET status = ?, current_load = ?, last_heartbeat_at = CURRENT_TIMESTAMP "
        "WHERE node_key = ?", CONNECTION_ASYNC);
    PrepareStatement(HUB_SEL_CLUSTER_POLICY,
        "SELECT node_key,name,capabilities,admin_state,service_type FROM hub_cluster_policy ORDER BY node_key", CONNECTION_SYNCH);
    PrepareStatement(HUB_SEL_CLUSTER_POLICY_BY_KEY,
        "SELECT admin_state FROM hub_cluster_policy WHERE node_key = ?", CONNECTION_SYNCH);
    PrepareStatement(HUB_UPSERT_CLUSTER_POLICY,
        "INSERT INTO hub_cluster_policy (node_key,name,capabilities,admin_state,updated_by,service_type) VALUES (?,?,?,?,?,?) "
        "ON DUPLICATE KEY UPDATE name=VALUES(name),capabilities=VALUES(capabilities),admin_state=VALUES(admin_state),updated_by=VALUES(updated_by),service_type=VALUES(service_type)", CONNECTION_SYNCH);
}
