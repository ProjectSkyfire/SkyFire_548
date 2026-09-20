# Pending SQL Updates

Use this directory for SQL changes that need to be tested against a local or
staging database before they are promoted into the normal update stream.

By default, the server database updater does not read files from this directory.
On worldserver, enable `WorldDatabase.ImportPendingUpdates` and/or
`CharacterDatabase.ImportPendingUpdates` to import matching pending folders
during startup update checks. Hubserver uses `HubDatabase.ImportPendingUpdates`
for `pending_updates/hub`. Leave those options disabled for production.

When automatic import is disabled, apply pending updates manually while testing.
After an update is verified, move it into the matching `sql/updates/<database>`
directory so the normal database setup and update tracking system can apply it.

## Layout

- `auth` - pending auth database updates
- `characters` - pending character database updates
- `world` - pending world database updates
- `hub` - pending hub database updates (manual promotion until CI supports this domain)

## Promotion Checklist

1. Add the test SQL file to the matching pending folder.
2. Apply it manually to a test database, or enable the matching
   `ImportPendingUpdates` option on a non-production worldserver.
3. Verify server startup, affected commands, and any in-game behavior.
4. Name the file clearly for review. It does not need the final update name.
   The promotion script assigns `YYYY_MM_DD_<database>_NN.sql` automatically.
5. For auth, characters and world, push the pending SQL file and let the promotion
   workflow move it into `sql/updates/<database>`. Hub is not yet accepted by that
   workflow or its promotion script; promote hub updates manually as described below.
6. Let the database setup system record it through `skyfire_db_updates` and
   `db_update`.

## Hub update ordering

Place hub SQL directly in `sql/pending_updates/hub/`, with sortable names such as
`001_add_setting.sql`, then `002_backfill_setting.sql` when one depends on another.
Future CI support can assign release names in that order. CI and the promotion
script are unchanged in this phase; pushing hub pending SQL may fail the existing
workflow's domain validation until that support is added.

For now, review and move verified hub files to `sql/updates/hub/` manually using
`YYYY_MM_DD_hub_NN.sql`, with the next unused two-digit sequence for the date.
Preserve dependency order and never rename or edit released files. The startup
updater sorts filenames lexicographically; pending files join that same sort
when opted in. Do not make pending changes depend on an unapplied released file
that sorts after them; first bring the staging database up to date.

Tracking uses filenames. Renaming an already imported pending file for release
makes it a new update: use a fresh disposable staging database to test the promoted
stream, or reconcile the tracking record deliberately after verifying its content.
Production should import released updates only. See
[hub database setup](../../doc/ClusteringSetup.md#hub-database-setup-and-updates).
