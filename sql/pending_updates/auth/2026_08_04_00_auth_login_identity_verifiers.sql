CREATE TABLE IF NOT EXISTS `account_login_identity` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `account_id` int unsigned NOT NULL,
  `identity_type` tinyint unsigned NOT NULL COMMENT '0 legacy username, 1 email',
  `identity` varchar(255) NOT NULL DEFAULT '',
  `identity_canonical` varchar(255) NOT NULL DEFAULT '',
  `salt` binary(32) NOT NULL,
  `verifier` binary(32) NOT NULL,
  `created_at` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `idx_account_login_identity_canonical` (`identity_type`,`identity_canonical`),
  KEY `idx_account_login_identity_account` (`account_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 COMMENT='Account login SRP identities';

INSERT INTO `account_login_identity` (`account_id`, `identity_type`, `identity`, `identity_canonical`, `salt`, `verifier`)
SELECT `id`, 0, `username`, UPPER(`username`), `salt`, `verifier`
FROM `account`
WHERE `username` <> ''
ON DUPLICATE KEY UPDATE
  `account_id` = VALUES(`account_id`),
  `identity_type` = VALUES(`identity_type`),
  `identity` = VALUES(`identity`),
  `salt` = VALUES(`salt`),
  `verifier` = VALUES(`verifier`),
  `updated_at` = CURRENT_TIMESTAMP;
