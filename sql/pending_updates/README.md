# Pending SQL Updates

Use this directory for SQL changes that need to be tested against a local or
staging database before they are promoted into the normal update stream.

The server database updater does not read files from this directory. Updates in
this area must be applied manually while testing. After an update is verified,
move it into the matching `sql/updates/<database>` directory so the normal
database setup and update tracking system can apply it.

## Layout

- `auth` - pending auth database updates
- `characters` - pending character database updates
- `world` - pending world database updates

## Promotion Checklist

1. Add the test SQL file to the matching pending folder.
2. Apply it manually to a test database.
3. Verify server startup, affected commands, and any in-game behavior.
4. Name the file clearly for review. It does not need the final update name.
   The promotion script assigns `YYYY_MM_DD_<database>_NN.sql` automatically.
5. Push the pending SQL file and let the promotion workflow move it into
   `sql/updates/<database>`.
6. Let the database setup system record it through `skyfire_db_updates` and
   `db_update`.
