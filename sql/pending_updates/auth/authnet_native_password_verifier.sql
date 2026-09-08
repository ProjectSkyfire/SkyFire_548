ALTER TABLE `account_login_identity`
  ADD COLUMN `authnet_salt` binary(32) DEFAULT NULL AFTER `verifier`,
  ADD COLUMN `authnet_verifier` binary(128) DEFAULT NULL AFTER `authnet_salt`;
