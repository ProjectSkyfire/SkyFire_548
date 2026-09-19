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
