-- Persist scheduled maintenance sets and the exact services to restart.
CREATE TABLE hub_backup_cycles (
 id char(32) CHARACTER SET ascii COLLATE ascii_bin NOT NULL PRIMARY KEY,
 state varchar(16) CHARACTER SET ascii NOT NULL DEFAULT 'requested',
 active_slot tinyint GENERATED ALWAYS AS (CASE WHEN state IN ('completed','failed') THEN NULL ELSE 1 END) STORED,
 countdown_seconds int unsigned NOT NULL DEFAULT 3600,
 scheduled_at bigint unsigned NOT NULL,
 stop_at bigint unsigned NOT NULL DEFAULT 0,
 hub_owner char(32) CHARACTER SET ascii NOT NULL DEFAULT '',
 services text NOT NULL,
 message varchar(255) NOT NULL DEFAULT '',
 created_at timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
 updated_at timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
 UNIQUE KEY one_backup_cycle(active_slot)
) ENGINE=InnoDB;
CREATE TABLE hub_backup_cycle_targets (
 cycle_id char(32) CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
 target varchar(16) CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
 job_id char(32) CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
 PRIMARY KEY(cycle_id,target), UNIQUE KEY cycle_job(job_id)
) ENGINE=InnoDB;
ALTER TABLE hub_backup_jobs ADD COLUMN cycle_id char(32) CHARACTER SET ascii COLLATE ascii_bin NOT NULL DEFAULT '';
ALTER TABLE hub_backup_schedules ADD COLUMN time_zone varchar(8) CHARACTER SET ascii NOT NULL DEFAULT 'UTC';
ALTER TABLE hub_backup_schedule_runs ADD COLUMN zone_name varchar(64) CHARACTER SET ascii NOT NULL DEFAULT 'UTC';
