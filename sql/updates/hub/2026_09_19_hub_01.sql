-- Scheduled execution, independent restore verification, and protected retention.
ALTER TABLE hub_backup_jobs
 ADD COLUMN verified_at timestamp NULL,
 ADD COLUMN verification_seconds int unsigned NOT NULL DEFAULT 0,
 ADD COLUMN pinned tinyint unsigned NOT NULL DEFAULT 0;
ALTER TABLE hub_backup_worker ADD COLUMN automation_status text NULL;
CREATE TABLE hub_backup_schedule_runs (
 target varchar(16) CHARACTER SET ascii COLLATE ascii_bin NOT NULL PRIMARY KEY,
 revision int unsigned NOT NULL,
 next_run bigint unsigned NOT NULL,
 last_job char(32) CHARACTER SET ascii COLLATE ascii_bin NOT NULL DEFAULT ''
) ENGINE=InnoDB;
