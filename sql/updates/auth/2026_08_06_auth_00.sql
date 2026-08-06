CREATE TABLE IF NOT EXISTS `account_twofactor` (
  `account_id` int unsigned NOT NULL,
  `method` tinyint unsigned NOT NULL DEFAULT 1 COMMENT '1 TOTP',
  `secret_base32` varchar(64) NOT NULL DEFAULT '',
  `enabled` tinyint unsigned NOT NULL DEFAULT 0,
  `confirmed_at` timestamp NULL DEFAULT NULL,
  `last_used_step` bigint unsigned DEFAULT NULL,
  `created_at` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`account_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 COMMENT='Account two-factor authentication';

INSERT IGNORE INTO `account_twofactor` (`account_id`, `method`, `secret_base32`, `enabled`, `confirmed_at`, `last_used_step`)
SELECT `id`, 1, `token_key`, 1, CURRENT_TIMESTAMP, NULL
FROM `account`
WHERE `token_key` <> '';

INSERT INTO `rbac_permissions` (`id`, `name`) VALUES
(1012, 'Command: account 2fa'),
(1013, 'Command: account 2fa setup'),
(1014, 'Command: account 2fa confirm'),
(1015, 'Command: account 2fa status'),
(1016, 'Command: account 2fa disable'),
(1017, 'Command: account 2fa reset')
ON DUPLICATE KEY UPDATE `name`=VALUES(`name`);

INSERT IGNORE INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES
(199, 1012),
(199, 1013),
(199, 1014),
(199, 1015),
(199, 1016),
(196, 1017);
