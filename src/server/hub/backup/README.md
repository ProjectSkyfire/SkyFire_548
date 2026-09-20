# Hub scheduled backup and recovery worker

Python 3.11+ and MySQL 8+ `mysql` / `mysqldump` are required. Install timezone support with
`python -m pip install -r backup/requirements.txt`. CMake build and INSTALL copy `backup/` alongside `web/` and `control/`;
existing `backup.toml` and archives are preserved. The worker runs independently of the hub.

## Setup

1. Back up the hub database and apply `sql/updates/hub/2026_09_19_hub_00.sql` and
   `2026_09_19_hub_01.sql`, then `2026_09_19_hub_02.sql` (plus the existing schedule migration) **before**
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
Encryption and remote archive storage are not implemented.

## Schedules, verification and retention

Operators and administrators can configure interval, daily and weekly schedules. Daily and
weekly schedules offer UTC or the server time zone. Existing schedules remain UTC until
explicitly edited. `server_timezone = "system"` resolves the worker host's OS timezone on
Windows/Linux; an IANA override such as `America/Chicago` is also accepted. Central uses
CST/CDT automatically. Repeated fall-back times run once at the first occurrence; nonexistent
spring-forward times shift forward by the DST gap (02:30 becomes 03:30). Interval schedules
measure elapsed minutes independently of DST. Next-run labels include the selected zone.

`shutdown_warning_seconds = 3600` sends each running, hub-managed world a normal
`server shutdown 3600` command one hour before the scheduled set. Players see the normal
in-game countdown. Occurrences due together share a durable maintenance set. The hub records
which services were running, blocks new starts and conflicting world commands, waits for
clean world exits and player saves, then gracefully stops authentication services. It does
not kill a world or use `server restart`, which would restart before the backups finish.
All cluster leases must also be offline before dumping. Nodes outside this hub's process
supervisor must be stopped separately; a live external node causes the set to wait/fail.

The worker serializes every member of the set and its verification attempt. Only after all
dumps complete does the hub restart the recorded services and wait for their ready messages
before releasing maintenance. Previously stopped services stay stopped. A dump that completed
but failed isolated verification is reported as unverified; it does not prevent restart.
Shutdown, dump or restart failures retain maintenance for operator review. No forced shutdown
is used for scheduled maintenance. After the countdown, a ten-minute graceful-stop timeout
reports failure. Hub restart during an active set fails closed because prior process ownership
cannot be safely reconstructed. Pending countdowns may still fire: stop/review services before
ending failed maintenance. An active set cannot be released through the maintenance button.

New schedules/edits leave a full warning interval before their first occurrence. Late runs
receive a full countdown rather than shortening the player warning. Missed occurrences
coalesce; intervals shorter than the warning cannot produce overlapping shutdown cycles.
A persisted cursor and atomic set/cursor transaction prevent replay across worker restarts.
Manual backup remains an immediate request and does not initiate this scheduled restart cycle.
The Backup page reports set progress, next run, last successful backup and last verified
restore independently of form editing. Archive protection and live restore remain admin-only.

Every new ordinary backup attempts an isolated restore after releasing its snapshot locks.
Verification creates an exclusive temporary database and a random temporary MySQL account
with privileges only on that database. Dump DEFINER headers are rebound to this restricted
account in a temporary copy. Import, table-count validation and CHECK TABLE must all succeed.
The original archive is unchanged. The stage and account are removed even after failure.
The configured database identity therefore needs CREATE USER, DROP USER and permission to
grant database-local privileges, in addition to CREATE/DROP DATABASE and dump access.
For full verification on servers with binary logging, configure `verification_config` to
point at a separate compatible MySQL instance with binary logging disabled. Its file uses
`VerificationDatabaseInfo = "host;port;user;password;mysql"`. Verification stages and accounts
are created on that instance; dump identity still refers to the original database. A hub
archive contains audit triggers, and MySQL's binary-logging restrictions may reject their
creation by a restricted account on the live instance. The worker does not change global
logging settings or grant SUPER to the importer. See the
[MySQL stored-program binary logging rules](https://dev.mysql.com/doc/refman/8.0/en/stored-programs-logging.html).
SQL events remain unsupported for automatic verification. Cross-database objects may fail
verification under these restrictions; they do not gain access to live databases.

A completed dump with failed restore verification remains available and is explicitly
reported as unverified. Verified time, duration and restored row counts are recorded in the
manifest; verified time and duration are also recorded in the hub. This checks that the dump
can be imported and tables are readable; it does not prove application correctness or that
separate domain snapshots form a coordinated recovery point. Interrupted verification can
leave a `skyfire_verify_*` database/account; inspect and remove those orphaned resources as
a DBA after confirming no worker is using them.

Set `retention_days` in backup.toml (default 30; 0 disables deletion). Once an hour, the worker
expires older completed archives only when a newer verified recovery point exists for that
domain. The newest verified point, pinned archives, all pre-restore rollback copies, unknown
archives, and active restore sources are protected. Administrators can pin/unpin ordinary
completed archives in job history. Retention records an audit event, preserves job history,
and deletes only the exact dump and manifest files, never a directory tree. Interrupted
deletions resume on the next pass. Back up archives externally before enabling retention
where a different storage policy is required.

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
Durable handoff tokens, when present, are cleared after hub restore or rollback so
consumed grants cannot be revived from an old snapshot. Manual DBA recovery must
also clear `hub_handoff_tokens` before starting any hub.
Keep services stopped for the entire command. Start the hub afterward; restored maintenance
blocks game-service starts until an administrator reviews recovery and ends maintenance.
The restored hub contains the administrators, policies and job history from the selected
recovery point. Archive files created later remain on disk even if absent from that history.
Do not use this command for a database version older than the installed hub schema.

## Validation

`tools/dev/tests/manual_backup.test.py` creates disposable databases and verifies a real dump,
corruption rejection, serialized jobs, staging/live restore, automatic rollback after an
injected failure and worker fencing. It never restores deployment databases.
Run it with `--domain auth`, `--domain characters`, and `--domain world`; each invocation also
tests offline hub recovery, UTC schedule boundaries/restarts, isolated verification and
retention protection. `--report PATH` writes measured durations. These small fixtures measure
functional recovery overhead, not production RTO. Operational RPO is time since the last
successful verified backup; scheduled maintenance waits can extend it without bound. Measure
production-sized isolated restores before committing to an RTO/RPO service objective.

`backup_recovery_drill.test.py` accepts `--mysqld`, `--mysql`, `--mysqldump`, `--work-directory`
and `--reports`. It initializes an exclusive local MySQL instance, runs all three game-domain
drills plus offline hub recovery, verifies cleanup, then shuts down only its own child server.
No installed server configurations, services or deployment databases are changed.
