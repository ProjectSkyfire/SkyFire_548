-- Manual backup jobs are admitted by hubserver and executed by the backup worker.
CREATE TABLE IF NOT EXISTS hub_backup_worker (
 id tinyint unsigned NOT NULL PRIMARY KEY,
 owner char(32) CHARACTER SET ascii COLLATE ascii_bin NOT NULL DEFAULT '',
 lease_until timestamp NULL,
 targets varchar(64) CHARACTER SET ascii NOT NULL DEFAULT '',
 restore_enabled tinyint unsigned NOT NULL DEFAULT 0,
 archive_bytes bigint unsigned NOT NULL DEFAULT 0,
 archive_count int unsigned NOT NULL DEFAULT 0,
 storage_total bigint unsigned NOT NULL DEFAULT 0,
 storage_free bigint unsigned NOT NULL DEFAULT 0,
 maintenance tinyint unsigned NOT NULL DEFAULT 0,
 recovery_safe tinyint unsigned NOT NULL DEFAULT 1,
 services_stopped tinyint unsigned NOT NULL DEFAULT 0,
 hub_seen timestamp NULL
) ENGINE=InnoDB;
INSERT IGNORE INTO hub_backup_worker(id) VALUES (1);
CREATE TABLE IF NOT EXISTS hub_backup_jobs (
 id char(32) CHARACTER SET ascii COLLATE ascii_bin NOT NULL PRIMARY KEY,
 target varchar(16) CHARACTER SET ascii NOT NULL,
 actor varchar(64) NOT NULL,
 kind varchar(16) CHARACTER SET ascii NOT NULL DEFAULT 'backup',
 source_id char(32) CHARACTER SET ascii COLLATE ascii_bin NOT NULL DEFAULT '',
 state varchar(16) CHARACTER SET ascii NOT NULL DEFAULT 'queued',
 active_slot tinyint GENERATED ALWAYS AS (CASE WHEN state IN ('queued','running') THEN 1 ELSE NULL END) STORED,
 owner char(32) CHARACTER SET ascii COLLATE ascii_bin NOT NULL DEFAULT '',
 created_at timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
 finished_at timestamp NULL,
 bytes bigint unsigned NOT NULL DEFAULT 0,
 sha256 char(64) CHARACTER SET ascii NOT NULL DEFAULT '',
 message varchar(255) NOT NULL DEFAULT '',
 UNIQUE KEY one_active_backup(active_slot), KEY job_history(created_at)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
