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

/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
