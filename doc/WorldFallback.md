# Same-host world fallback

This phase provides a **cold standby**, controlled switchover and optional automatic
promotion after a confirmed primary crash. It supports one local pair per hub on
Windows or Linux. Players reconnect; unsaved state in a crashed world's memory
cannot be recovered by starting the standby. It is not cross-host fencing or live
session migration. A missed heartbeat never authorizes takeover.

## Prepare the pair

1. Compile/install hubserver and worldserver together, including web assets. Do not
   enable fallback with an older/unfenced world still running. Use one active hub.
2. Give the second world its own `.conf`, cluster node key/certificate, display name,
   logs, cache and port. Use the same worldserver executable for both managed rows.
   Keep identical explicit `RealmID`, `LoginDatabaseInfo`, `WorldDatabaseInfo`,
   `Cluster.HubHost` and `Cluster.HubPort`. With the character service enabled,
   both worlds must explicitly use the same `CharacterService.Host`,
   `CharacterService.Port` and `CharacterService.NodeKey`, with
   `CharacterService.Enable = 1` and no direct character database credentials.
   Legacy direct-database pairs instead require identical `CharacterDatabaseInfo`.
   Enable cluster registration, handoffs and the authserver live realm directory.
   Clustered worlds with a positive `RealmID` load that exact realm regardless of
   `realmlist.port`; the live cluster directory publishes the active node's port.
   Leave the static realm row intact when switching nodes. Worlds without an
   explicit cluster realm retain the legacy port-based realm discovery.
3. Create a local directory owned by the service identity, for example
   `C:/SkyFire/locks` or `/var/lib/skyfire/locks`. In **both** world configurations:

   ```ini
   World.OwnershipLock = "C:/SkyFire/locks/realm-1.lock"
   ```

   Use the same absolute local path. Every process that writes this realm's
   character database must honor it; older binaries, other hosts and external
   database writers are outside this protection. Do not put the file on a network
   share or delete/replace it while a world runs. The OS releases ownership on
   exit, including a crash, but the file remains. A leftover file is normal.
   Worldserver acquires ownership before database setup and holds it through
   database cleanup. Database CLI overrides are disabled when ownership is set.
4. Authorize the standby certificate identity in `Hub.MapData.Readers` and each
   provider's `allowed_world_nodes`. When using the character service, also add it
   to `characterserver.toml`'s `allowed_world_nodes`. Gracefully stop the active
   world before restarting data services to load these permissions. Prewarm the
   standby's separate cache using `fetch_maps.py --world-config PATH_TO_STANDBY_CONFIG`.
5. Add the standby through the existing web world-node configuration form, using
   service key `world-standby`, its own configuration path, the same executable
   path as `world`, and the correct working directory. Both records must be enabled;
   enabled is permission to launch, not an instruction to run both simultaneously.
6. Set these in hubserver.conf, then restart the hub normally:

   ```ini
   Hub.Fallback.Enable = 1
   Hub.Fallback.Primary = "world"
   Hub.Fallback.Standby = "world-standby"
   Hub.Fallback.Automatic = 0
   Hub.Fallback.Countdown = 60
   ```

   These settings are startup-only. Start the primary through the hub and wait for
   readiness. Leave standby stopped. Preflight rejects a pair with inconsistent
   databases, realm, hub endpoint, executable or lock path. Do not edit identity,
   database or ownership settings while either member runs.

The feature reuses durable restart maintenance in `hub_backup_worker`; no new SQL
is needed if coordinated node restart already works. Older hub databases need
`sql/pending_updates/hub/002_node_restarts.sql` if `restart_owner` is absent.
Do not replay that ALTER on a fresh base which already contains the column.

## Planned promotion

Click **Promote** on the stopped member's sidebar card, or use the local hub console:

```
fallback promote world-standby
fallback status
```

The operation reserves maintenance against backups/recovery and Restart all,
checks the target's map dependencies, sends `server shutdown <countdown>` to the
source and suppresses its soft restart. It waits for OS-confirmed process exit
and expiry/removal of all registrations owning the realm. It then probes the
ownership lock and launches the target, which acquires the lock independently.
Completion requires its ownership report, healthy process status and an enabled,
ready, unique world registration. Hub/web and auth remain online.

The source has countdown plus five minutes to stop; replacement has thirty minutes
to become ready. Neither timeout force-kills a world. A planned shutdown that exits
abnormally stops the operation for review instead of claiming a successful save.

To switch back, explicitly promote `world` after reviewing the pair. Ordinary
Start cannot launch one member while the other process is active or its exit is
unconfirmed. Promote requires a source this hub actually observed ready with
ownership; after a hub restart, inspect existing processes before normal Start.

## Automatic promotion

After a successful planned test, set `Hub.Fallback.Automatic = 1` and restart the
hub. Automatic promotion is one-way, primary to standby. It requires all of:

- Primary was previously ready and reported holding its ownership lock.
- The hub's OS process handle/wait confirms exit with an abnormal code.
- Exit was not a stop, shutdown/restart request, backup/restart cycle or hub shutdown.
- Standby preflight and maintenance admission succeed.

A failed bootstrap, lost heartbeat, failed OS process query, healthy but stalled
primary, clean shutdown or exit code 2 does not trigger promotion. There is one
attempt per observed crash, no automatic retry loop and no automatic failback.
The standby's own crash requires operator recovery.

### Preventive graceful switchover

With automatic mode enabled, optionally set `Hub.Fallback.DegradedUpdateMs` to a
measured world-tick threshold and `Hub.Fallback.DegradedSeconds` to the sustained
duration (minimum 15 seconds). The threshold defaults to 0, disabled. Short spikes,
stale reports and heartbeat loss do not qualify. Sustained fresh overload requests
the same graceful countdown/shutdown/save workflow before a possible crash.

This is an overload signal, not a crash prediction. Character saves stay in the
world's existing shutdown path; the hub never writes character records or starts
a concurrent emergency save thread. If the world cannot save/exit cleanly, promotion
fails with maintenance retained. Switching processes cannot fix a shared database
bottleneck, and abrupt crashes can still lose changes since the last save. Calibrate
thresholds in testing before enabling them. There is one request per sustained
overload episode, reset by recovery or loss of fresh eligibility.

An admitted operation leaves durable maintenance set on failure or hub loss. A
restarted hub does not guess which stage completed or automatically resume it.
Inspect both processes, logs and registry before releasing recovery maintenance.
If target starts but readiness fails, it may still be running: stop it gracefully
before starting the other member. Never remove the lock file to bypass ownership.

## Validation before rollout

- Build/run the `world_fallback_tests` CTest target: separate-process lock exclusion,
  normal/crash release, independent realm locks and automatic-exit eligibility.
- Run `tools/dev/tests/hub_status.test.cjs` with Playwright for promotion controls,
  permissions, stale status and independent polling.
- Test a planned promotion and failback with disposable character data; verify
  saved position/inventory and the new realm endpoint after reconnecting.
- Attempt a second fenced world directly and verify rejection before DB startup.
- Test a primary crash after a known save, standby bootstrap failure, provider
  outage, conflicting realm registration, maintenance contention and hub restart
  during promotion. Verify no duplicate writer, automatic retry or early release.
- Simulate heartbeat loss without process exit; standby must remain stopped.

Native compilation and live failure drills are required; browser fixtures and
policy tests alone do not establish runtime failover safety.

## Character service and future warm standby

The [character service](CharacterServer.md) now provides character database
reads/writes, exclusive writer epochs and rejection of retired world writers.
Both members use that same service; the target acquires a new writer generation
after the source disconnects. The local ownership lock remains required.
Explicit gameplay and session transfer rules are still needed for a running standby.
Centralizing queries alone does not transfer player objects, pending saves, combat
or other unsaved simulation state. Keep this cold-standby mode until those guarantees
are implemented and tested.

The character service must already hold character state when a world crashes; it
cannot reconstruct unreplicated state from a dead process. Its implementation
contract should include:

- Initial complete snapshots and ordered changes identified by character, owning
  world generation and monotonically increasing revision.
- Acknowledgments only after durable journal admission, with bounded queues and
  explicit backpressure. Repeated messages must be idempotent; gaps require resync.
- Consistent snapshot boundaries for coupled state such as inventory, currency,
  quests and position, rather than independently committing fragments from
  different world ticks. Database checkpoint publication must be transactional.
- Recovery from the latest complete acknowledged revision, with newer journal
  entries replayed in order. A database write failure must not discard the journal
  or advertise an incomplete snapshot as committed.
- Exclusive writer generations issued by the character service. Before handing
  a character to standby, revoke the old generation and reject its late writes;
  heartbeat loss alone must not allow both worlds to own the character.
- A defined gameplay boundary: operations requiring no acknowledged rollback must
  not be presented to the player as committed before the service acknowledges them.
  Changes never received by the service remain outside the recovery guarantee.

This is the next-phase design, not functionality provided by the current fallback
implementation. World simulation/session restoration and character-service
availability also need their own failure tests before enabling a running standby.
