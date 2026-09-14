-- Add Authnet SRP fields used by the new login handshake.
-- Idempotent so existing dumps that already have the columns can skip the ALTER.

SET @add_authnet_salt_sql := (
    SELECT IF(
        COUNT(*) = 0,
        'ALTER TABLE `account_login_identity` ADD COLUMN `authnet_salt` binary(32) DEFAULT NULL AFTER `verifier`',
        'DO 0'
    )
    FROM `INFORMATION_SCHEMA`.`COLUMNS`
    WHERE `TABLE_SCHEMA` = DATABASE()
      AND `TABLE_NAME` = 'account_login_identity'
      AND `COLUMN_NAME` = 'authnet_salt'
);
PREPARE add_authnet_salt FROM @add_authnet_salt_sql;
EXECUTE add_authnet_salt;
DEALLOCATE PREPARE add_authnet_salt;

SET @add_authnet_verifier_sql := (
    SELECT IF(
        COUNT(*) = 0,
        'ALTER TABLE `account_login_identity` ADD COLUMN `authnet_verifier` binary(128) DEFAULT NULL AFTER `authnet_salt`',
        'DO 0'
    )
    FROM `INFORMATION_SCHEMA`.`COLUMNS`
    WHERE `TABLE_SCHEMA` = DATABASE()
      AND `TABLE_NAME` = 'account_login_identity'
      AND `COLUMN_NAME` = 'authnet_verifier'
);
PREPARE add_authnet_verifier FROM @add_authnet_verifier_sql;
EXECUTE add_authnet_verifier;
DEALLOCATE PREPARE add_authnet_verifier;
