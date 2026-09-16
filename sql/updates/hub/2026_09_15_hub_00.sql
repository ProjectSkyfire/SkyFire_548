CREATE TABLE IF NOT EXISTS `hub_managed_services` (
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
  ('authnet', 'Authnet Server', 'authserver', 'authserver.conf', '.', 1)
ON DUPLICATE KEY UPDATE `service_key` = VALUES(`service_key`);
