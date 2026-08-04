SET @add_email_login_converted_sql := (
    SELECT IF(
        COUNT(*) = 0,
        'ALTER TABLE `account` ADD COLUMN `email_login_converted` tinyint unsigned NOT NULL DEFAULT 0 COMMENT ''1 when the account self-converted to email login'' AFTER `reg_mail`',
        'DO 0'
    )
    FROM `INFORMATION_SCHEMA`.`COLUMNS`
    WHERE `TABLE_SCHEMA` = DATABASE()
      AND `TABLE_NAME` = 'account'
      AND `COLUMN_NAME` = 'email_login_converted'
);
PREPARE add_email_login_converted FROM @add_email_login_converted_sql;
EXECUTE add_email_login_converted;
DEALLOCATE PREPARE add_email_login_converted;

INSERT INTO `rbac_permissions` (`id`, `name`) VALUES
(232, 'Command: account convert'),
(233, 'Command: account convert email')
ON DUPLICATE KEY UPDATE `name`=VALUES(`name`);

INSERT IGNORE INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES
(199, 232),
(199, 233),
(232, 233);
