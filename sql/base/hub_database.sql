/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

DROP TABLE IF EXISTS `hub_nodes`;
CREATE TABLE `hub_nodes` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `node_key` varchar(64) NOT NULL COMMENT 'Stable node identifier used for registration and heartbeats',
  `name` varchar(100) NOT NULL,
  `node_type` tinyint unsigned NOT NULL DEFAULT '0' COMMENT '0 unspecified, 1 auth, 2 world, 3 service',
  `route_address` varchar(255) NOT NULL DEFAULT '127.0.0.1',
  `route_port` smallint unsigned NOT NULL DEFAULT '0',
  `realm_id` int unsigned DEFAULT NULL,
  `region` varchar(32) NOT NULL DEFAULT '',
  `priority` smallint unsigned NOT NULL DEFAULT '100' COMMENT 'Lower values are preferred',
  `weight` smallint unsigned NOT NULL DEFAULT '100' COMMENT 'Relative load-balancing weight',
  `capacity` int unsigned NOT NULL DEFAULT '0' COMMENT 'Zero means capacity is unspecified',
  `current_load` int unsigned NOT NULL DEFAULT '0',
  `fallback_node_id` int unsigned DEFAULT NULL,
  `enabled` tinyint unsigned NOT NULL DEFAULT '1',
  `maintenance` tinyint unsigned NOT NULL DEFAULT '0',
  `status` tinyint unsigned NOT NULL DEFAULT '0' COMMENT '0 unknown, 1 online, 2 offline, 3 degraded',
  `last_heartbeat_at` timestamp NULL DEFAULT NULL,
  `created_at` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `idx_hub_nodes_key` (`node_key`),
  KEY `idx_hub_nodes_route` (`node_type`,`region`,`realm_id`,`enabled`,`maintenance`,`priority`),
  KEY `idx_hub_nodes_fallback` (`fallback_node_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='Hub routing nodes';

DROP TABLE IF EXISTS `hub_managed_services`;
CREATE TABLE `hub_managed_services` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `service_key` varchar(64) NOT NULL COMMENT 'Stable service identifier used by hub commands',
  `name` varchar(100) NOT NULL,
  `executable_path` varchar(1024) NOT NULL COMMENT 'Absolute path or path relative to working_directory',
  `config_path` varchar(1024) NOT NULL COMMENT 'Absolute path or path relative to working_directory',
  `working_directory` varchar(1024) NOT NULL DEFAULT '.',
  `enabled` tinyint unsigned NOT NULL DEFAULT '1',
  `service_kind` tinyint unsigned NOT NULL DEFAULT '0' COMMENT '0 auth/world, 3 mapserver, 4 characterserver, 5 chatserver',
  `cluster_key` varchar(64) NOT NULL DEFAULT '',
  `created_at` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `idx_hub_managed_services_key` (`service_key`),
  KEY `idx_hub_managed_services_enabled` (`enabled`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='Processes managed exclusively by the hub server';

INSERT INTO `hub_managed_services`
  (`service_key`, `name`, `executable_path`, `config_path`, `working_directory`, `enabled`)
VALUES
  ('authnet', 'Authnet Server', 'authserver', 'authserver.conf', '.', 1),
  ('world', 'World Server', 'worldserver', 'worldserver.conf', '.', 1);

CREATE TABLE IF NOT EXISTS `hub_cluster_policy` (
  `service_type` tinyint unsigned NOT NULL DEFAULT '1' COMMENT '1 auth, 2 world',
  `node_key` varchar(64) CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
  `name` varchar(100) NOT NULL,
  `capabilities` int unsigned NOT NULL DEFAULT '0',
  `admin_state` tinyint unsigned NOT NULL DEFAULT '0' COMMENT '0 enabled, 1 draining, 2 disabled',
  `updated_by` varchar(64) NOT NULL,
  `updated_at` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`node_key`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

DROP TABLE IF EXISTS `hub_admins`;
CREATE TABLE `hub_admins` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(64) NOT NULL,
  `password_hash` varchar(255) NOT NULL DEFAULT '' COMMENT 'Encoded password hash; format defined by the admin protocol',
  `access_flags` bigint unsigned NOT NULL DEFAULT '1' COMMENT '1 view, 2 manage nodes, 4 operate nodes, 8 manage admins, 16 remote login',
  `enabled` tinyint unsigned NOT NULL DEFAULT '1',
  `last_login_at` timestamp NULL DEFAULT NULL,
  `created_at` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `idx_hub_admins_username` (`username`),
  KEY `idx_hub_admins_enabled` (`enabled`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='Hub server administrators';

-- Remote control audit records are append-only. No application API edits or deletes them.
CREATE TABLE IF NOT EXISTS `hub_control_audit` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `request_id` char(32) CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
  `phase` varchar(16) CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
  `actor` varchar(64) NOT NULL,
  `action` varchar(32) NOT NULL,
  `target` varchar(64) NOT NULL,
  `outcome` varchar(32) NOT NULL,
  `created_at` timestamp(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6),
  PRIMARY KEY (`id`),
  UNIQUE KEY `request_phase` (`request_id`,`phase`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
CREATE TRIGGER `hub_control_audit_no_update` BEFORE UPDATE ON `hub_control_audit`
  FOR EACH ROW SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Control audit records are immutable';
CREATE TRIGGER `hub_control_audit_no_delete` BEFORE DELETE ON `hub_control_audit`
  FOR EACH ROW SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Control audit records are immutable';

-- Persisted backup schedule preferences. Execution remains unavailable until backupserver is implemented.
CREATE TABLE IF NOT EXISTS `hub_backup_schedules` (
  `target` varchar(16) CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
  `enabled` tinyint unsigned NOT NULL DEFAULT 0,
  `mode` varchar(8) CHARACTER SET ascii COLLATE ascii_bin NOT NULL DEFAULT 'daily',
  `interval_minutes` int unsigned NOT NULL DEFAULT 60,
  `minute_of_day` int unsigned NOT NULL DEFAULT 180,
  `weekday` tinyint unsigned NOT NULL DEFAULT 0 COMMENT 'Monday=0; UTC',
  `revision` int unsigned NOT NULL DEFAULT 0,
  `last_request` char(32) CHARACTER SET ascii COLLATE ascii_bin NOT NULL DEFAULT '',
  `updated_by` varchar(64) NOT NULL DEFAULT '',
  `updated_at` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`target`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
INSERT IGNORE INTO hub_backup_schedules(target) VALUES ('auth'),('characters'),('world'),('hub');

/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Manual backup jobs are admitted by hubserver and executed by the backup worker.
CREATE TABLE IF NOT EXISTS hub_backup_worker (
 id tinyint unsigned NOT NULL PRIMARY KEY,
 owner char(32) CHARACTER SET ascii COLLATE ascii_bin NOT NULL DEFAULT '',
 lease_until timestamp NULL,
 targets varchar(64) CHARACTER SET ascii NOT NULL DEFAULT '',
 restore_enabled tinyint unsigned NOT NULL DEFAULT 0,
 archive_bytes bigint unsigned NOT NULL DEFAULT 0,
 archive_count int unsigned NOT NULL DEFAULT 0,
 storage_total bigint unsigned NOT NULL DEFAULT 0,
 storage_free bigint unsigned NOT NULL DEFAULT 0,
 maintenance tinyint unsigned NOT NULL DEFAULT 0,
 recovery_safe tinyint unsigned NOT NULL DEFAULT 1,
 services_stopped tinyint unsigned NOT NULL DEFAULT 0,
 restart_owner varchar(32) NOT NULL DEFAULT '',
 hub_seen timestamp NULL
) ENGINE=InnoDB;
INSERT IGNORE INTO hub_backup_worker(id) VALUES (1);
CREATE TABLE IF NOT EXISTS hub_backup_jobs (
 id char(32) CHARACTER SET ascii COLLATE ascii_bin NOT NULL PRIMARY KEY,
 target varchar(16) CHARACTER SET ascii NOT NULL,
 actor varchar(64) NOT NULL,
 kind varchar(16) CHARACTER SET ascii NOT NULL DEFAULT 'backup',
 source_id char(32) CHARACTER SET ascii COLLATE ascii_bin NOT NULL DEFAULT '',
 state varchar(16) CHARACTER SET ascii NOT NULL DEFAULT 'queued',
 active_slot tinyint GENERATED ALWAYS AS (CASE WHEN state IN ('queued','running') THEN 1 ELSE NULL END) STORED,
 owner char(32) CHARACTER SET ascii COLLATE ascii_bin NOT NULL DEFAULT '',
 created_at timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
 finished_at timestamp NULL,
 bytes bigint unsigned NOT NULL DEFAULT 0,
 sha256 char(64) CHARACTER SET ascii NOT NULL DEFAULT '',
 message varchar(255) NOT NULL DEFAULT '',
 UNIQUE KEY one_active_backup(active_slot), KEY job_history(created_at)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- Scheduled execution, independent restore verification, and protected retention.
ALTER TABLE hub_backup_jobs
 ADD COLUMN verified_at timestamp NULL,
 ADD COLUMN verification_seconds int unsigned NOT NULL DEFAULT 0,
 ADD COLUMN pinned tinyint unsigned NOT NULL DEFAULT 0;
ALTER TABLE hub_backup_worker ADD COLUMN automation_status text NULL;
CREATE TABLE hub_backup_schedule_runs (
 target varchar(16) CHARACTER SET ascii COLLATE ascii_bin NOT NULL PRIMARY KEY,
 revision int unsigned NOT NULL,
 next_run bigint unsigned NOT NULL,
 last_job char(32) CHARACTER SET ascii COLLATE ascii_bin NOT NULL DEFAULT ''
) ENGINE=InnoDB;

-- Persist scheduled maintenance sets and the exact services to restart.
CREATE TABLE hub_backup_cycles (
 id char(32) CHARACTER SET ascii COLLATE ascii_bin NOT NULL PRIMARY KEY,
 state varchar(16) CHARACTER SET ascii NOT NULL DEFAULT 'requested',
 active_slot tinyint GENERATED ALWAYS AS (CASE WHEN state IN ('completed','failed') THEN NULL ELSE 1 END) STORED,
 countdown_seconds int unsigned NOT NULL DEFAULT 3600,
 scheduled_at bigint unsigned NOT NULL,
 stop_at bigint unsigned NOT NULL DEFAULT 0,
 hub_owner char(32) CHARACTER SET ascii NOT NULL DEFAULT '',
 services text NOT NULL,
 message varchar(255) NOT NULL DEFAULT '',
 created_at timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
 updated_at timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
 UNIQUE KEY one_backup_cycle(active_slot)
) ENGINE=InnoDB;
CREATE TABLE hub_backup_cycle_targets (
 cycle_id char(32) CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
 target varchar(16) CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
 job_id char(32) CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
 PRIMARY KEY(cycle_id,target), UNIQUE KEY cycle_job(job_id)
) ENGINE=InnoDB;
ALTER TABLE hub_backup_jobs ADD COLUMN cycle_id char(32) CHARACTER SET ascii COLLATE ascii_bin NOT NULL DEFAULT '';
ALTER TABLE hub_backup_schedules ADD COLUMN time_zone varchar(8) CHARACTER SET ascii NOT NULL DEFAULT 'UTC';
ALTER TABLE hub_backup_schedule_runs ADD COLUMN zone_name varchar(64) CHARACTER SET ascii NOT NULL DEFAULT 'UTC';
