# Hub manual backup and recovery worker

Python 3.11+ and MySQL 8+ `mysql` / `mysqldump` are required. The worker has no third-party
Python dependencies. CMake build and INSTALL copy `backup/` alongside `web/` and `control/`;
existing `backup.toml` and archives are preserved. The worker runs independently of the hub.

## Setup

1. Back up the hub database and apply `sql/updates/hub/2026_09_19_hub_00.sql` **before**
   starting the new hub binary. New installations include these tables in `hub_database.sql`.
2. Copy `backup.toml.dist` to `backup.toml`. Set absolute MySQL client paths and the output
   directory. Point `hub_config` and `world_config` at this installation's configurations.
   On Linux, account for the installed config directory rather than the Windows defaults.
3. Run `python backup/backupserver.py --config backup/backup.toml` using the server service
   identity, or configure that command in your OS service manager. Only one worker may own
   a hub. The web Backup page shows when its lease is online.
4. Administrators can select a database and click **Backup now**. A unique request id,
   CSRF check, hub audit record and database uniqueness constraint prevent duplicate or
   concurrent jobs. Viewers can inspect storage usage and the latest 30 jobs.

The four fixed targets are auth and hub from hubserver.conf, and characters and world from
one explicit worldserver.conf. Domains must use distinct database names; aliases are refused. Browser requests cannot choose paths, SQL, executable names,
credentials or other databases. Access credentials are read locally and passed to MySQL in
its child environment, never in command-line arguments, logs or web responses.

The worker uses an InnoDB single-transaction dump with routines, events and triggers. For a database containing MyISAM or other nontransactional tables, gracefully stop all
game services first. The worker enters temporary maintenance and holds table-level READ
locks in a separate connection throughout the dump. These locks also block concurrent
inserts; the hub database remains writable for heartbeat and audit updates. Temporary
maintenance is released when the backup finishes or fails. Hub control tables must remain InnoDB. Do not run schema migrations during a backup. Each target
is a separate snapshot; separate databases are not a coordinated cluster recovery point.
The archive and manifest are published only after a successful dump and checksum readback.
This verifies file integrity, not restoration. The manifest explicitly records that distinction.
The manifest includes database identity/version, tool version, timestamps, table count,
file size and SHA-256. Directories containing `.partial` files are not completed recovery points.

The volume display reports filesystem capacity/free space and completed archive bytes/count,
including rollback copies. Inventory refreshes every 30 seconds. The storage directory is
restricted to the worker identity (plus SYSTEM on Windows). Use a dedicated local directory,
not a shared web root. Archive files cannot be downloaded through the hub web server.
Retention, encryption, automatic scheduled execution and remote archive storage are not
implemented. Existing saved schedule preferences remain inactive. Nothing deletes archives.

## Live restore: auth, characters or world

Enable `allow_live_restore` only where the hub manages **all** database writers. External
servers, scripts and schema migrations must also be stopped. This worker does not fence
independent installations sharing the same database. The database identity must have
CREATE/DROP DATABASE privileges for staging and replacement, in addition to dump/import
privileges. Use the same compatible MySQL version and schema expectations as the archive.
Archives containing scheduled SQL events are refused by automatic restore to avoid running
events during staging; those require a separately isolated offline recovery procedure.

1. Gracefully shut down every worldserver with its countdown and stop auth services. Wait
   for all managed services and cluster leases to be offline. Never kill worldserver to
   prepare for a restore; current character data must finish saving.
2. Select the recovery point on the Backup page, type `RESTORE characters` (or `auth` /
   `world`) and click **Restore live database**. The hub enters persistent maintenance,
   blocks managed starts and account operations, and queues the request.
3. The worker validates the archive checksum and database identity, restores it into an
   exclusively created `skyfire_restore_<job id>` database, and records table row counts.
   It then takes a fresh, checksum-verified rollback backup of the current live database.
4. Only after these checks does it drop/recreate the configured live database and import
   the selected dump. Table row counts must match the staging restore. The temporary stage
   is removed; the rollback archive and recovery journal remain protected on disk.
5. If replacement fails, the worker attempts to restore and verify the rollback archive.
   A worker crash or failed rollback leaves maintenance locked. Never delete a recovery
   journal or rollback archive while investigating a failed job.
6. Review the job result, click **End recovery maintenance**, then start services. This
   button is unavailable while work is active or recovery safety remains unverified.

The recovery journal is `<output_directory>/<restore job id>/recovery.json`. It names the
source, stage and rollback archive. After a crash during replacement, restore the rollback
archive offline with a DBA before marking `hub_backup_worker.recovery_safe=1`. The UI does
not provide a force-unlock that could silently start servers against a partial database.

## Hub database recovery

Hub recovery cannot replace the database while the hub is using it. First finish all jobs,
stop game services gracefully, then stop the hub and backup worker and wait at least 60 seconds.
The command verifies locally stopped hub/game processes, expired worker lease and the hub's
last recorded stopped-service state. It supports locally hosted database targets only.

Run with the exact configured hub database name as confirmation:

```
python backup/backupserver.py --config backup/backup.toml --offline-restore-hub ARCHIVE_ID --confirm-database YOUR_HUB_DATABASE
```

This performs staging verification, a fresh rollback backup, live replacement and rollback
on failure. Its `offline_restore_<id>/recovery.json` journal remains outside the database.
Keep services stopped for the entire command. Start the hub afterward; restored maintenance
blocks game-service starts until an administrator reviews recovery and ends maintenance.
The restored hub contains the administrators, policies and job history from the selected
recovery point. Archive files created later remain on disk even if absent from that history.
Do not use this command for a database version older than the installed hub schema.

## Validation

`tools/dev/tests/manual_backup.test.py` creates disposable databases and verifies a real dump,
corruption rejection, serialized jobs, staging/live restore, automatic rollback after an
injected failure and worker fencing. It never restores deployment databases.
