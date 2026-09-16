CREATE TABLE IF NOT EXISTS `account_admin_events` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `action` varchar(24) NOT NULL,
  `account_id` int unsigned NOT NULL DEFAULT 0,
  `ip` varchar(15) NOT NULL DEFAULT '',
  `actor` varchar(64) NOT NULL,
  `created_at` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='Hub account audit and live moderation notifications';
