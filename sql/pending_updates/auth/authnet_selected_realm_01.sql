CREATE TABLE IF NOT EXISTS `account_authnet_session` (
  `account_id` int unsigned NOT NULL,
  `world_account_token` varchar(64) NOT NULL,
  `world_session_key` binary(40) NOT NULL,
  `auth_secret` binary(64) DEFAULT NULL,
  `connection_seed` int unsigned NOT NULL DEFAULT 0,
  `realm_field` int unsigned NOT NULL DEFAULT 0,
  `last_ip` varchar(45) NOT NULL DEFAULT '127.0.0.1',
  `locale` tinyint unsigned NOT NULL DEFAULT 0,
  `os` varchar(4) NOT NULL DEFAULT '',
  `expires_at` bigint unsigned NOT NULL DEFAULT 0,
  `created_at` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`account_id`),
  KEY `idx_authnet_session_token_ip` (`world_account_token`, `last_ip`, `expires_at`),
  KEY `idx_authnet_session_expires` (`expires_at`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 COMMENT='Authnet world session handoff';
