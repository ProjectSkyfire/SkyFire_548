# Restarting server nodes through the hub

The global status sidebar provides **Restart all** for operators with service-operation access. It refreshes currently running hub-managed world/auth services and registered mapservers while keeping the hub and web console online. Previously stopped nodes remain stopped. Backup workers and packet loggers are outside the restart set. Remote world/auth processes must be managed separately; their presence blocks this operation before shutdown begins.

## Installation

Build and INSTALL the updated hub and web assets, and deploy the updated `mapserver/mapserver.py`. Apply `sql/pending_updates/hub/002_node_restarts.sql` through the hub pending-update workflow for an existing database. The base schema includes this change for fresh installations. Preserve operator-owned configuration files.

Existing mapserver processes must be restarted once through their OS service/launcher after deployment to load restart support. Copying the script or compiling native servers does not refresh a running Python process. Restart all rejects older mapservers before sending any world shutdown command. Once upgraded, mapserver cards also expose an individual **Restart** action.

Set the player countdown in `hubserver.conf`:

```ini
Hub.Restart.Countdown = 60
```

The default is 60 seconds; accepted effective values are 0 through 3600. Zero starts graceful shutdown immediately, without a player warning interval.

## Sequence

1. Check running services, map dependencies, launch files and mapserver restart support. Reserve persistent maintenance only if no backup/restore/deletion job or scheduled backup cycle is active.
2. Send `server shutdown <seconds>` to each world through its hub control connection. Wait for a normal exit and character saving. The coordinator never force-kills a world.
3. Stop authentication services, then request mapserver process refresh over the authenticated cluster connection.
4. Wait for each mapserver to register ready with a new process generation and fresh metrics. A reconnect of the old process does not satisfy this check.
5. Start authentication services, then worlds after their map dependency checks. Release maintenance once all restarted services report ready.

Progress appears in the independently refreshed sidebar. World shutdown has the countdown plus five minutes to finish; mapserver refresh and subsequent startup share a thirty-minute deadline. Initial cache population can take time, so prewarm large continent caches before using this workflow.

Individual mapserver Restart refreshes only that provider. Worlds using already cached assets continue reading their local snapshots; new bootstrap/download work may need to retry while the provider restarts. The request is queued until the provider's next heartbeat. Process refresh closes its HTTPS listener and gives existing requests the server's normal cleanup grace period.

## Interrupted or failed operations

A failed step stops automatic progression and leaves persistent maintenance enabled. The sidebar reports the failure; the hub log records it. Maintenance also survives interruption of the hub itself, although detailed restart progress is held in memory. There is no automatic resume or forced world termination.

Review the logs and all node states. If worlds are still running or have a countdown pending, use their native console to shut them down gracefully and wait for normal exit. Stop any remaining authentication services through their normal OS controls as needed. Once managed services are stopped and recovery is safe, use **End maintenance** on the Backups page, then start the required services normally. Do not clear maintenance directly in SQL to bypass an unfinished backup or restore.

## Verification

`src/tests/mapserver_restart_test.py` validates lifecycle replies and an actual Python process replacement with paths containing spaces. `src/tests/mapserver_integration.py` checks authenticated restart delivery and a new ready generation with two HTTPS providers and a mock hub. `src/tests/node_restart_admission.py` uses a disposable database to exercise migration and competing maintenance/backup claims. `tools/dev/tests/hub_status.test.cjs` covers restart controls and independent polling. Native compilation and a live end-to-end restart remain required after installation.
