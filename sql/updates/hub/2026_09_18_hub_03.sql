-- Persisted backup schedule preferences. Execution remains unavailable until backupserver is implemented.
CREATE TABLE IF NOT EXISTS `hub_backup_schedules` (
  `target` varchar(16) CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
  `enabled` tinyint unsigned NOT NULL DEFAULT 0,
  `mode` varchar(8) CHARACTER SET ascii COLLATE ascii_bin NOT NULL DEFAULT 'daily',
  `interval_minutes` int unsigned NOT NULL DEFAULT 60,
  `minute_of_day` int unsigned NOT NULL DEFAULT 180,
  `weekday` tinyint unsigned NOT NULL DEFAULT 0 COMMENT 'Monday=0; UTC',
  `revision` int unsigned NOT NULL DEFAULT 0,
  `last_request` char(32) CHARACTER SET ascii COLLATE ascii_bin NOT NULL DEFAULT '',
  `updated_by` varchar(64) NOT NULL DEFAULT '',
  `updated_at` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`target`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
INSERT IGNORE INTO hub_backup_schedules(target) VALUES ('auth'),('characters'),('world'),('hub');
