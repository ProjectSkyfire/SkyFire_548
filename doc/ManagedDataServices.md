# Character metrics and managed data services

The hub web console can start, stop, and restart local mapserver and characterserver
processes. The hub launches the installed Python interpreter directly and supervises
the child through inherited pipes on Windows and Linux. A shell is never involved.

## Upgrade and configure

Build matching hub/world binaries and use CMake INSTALL to deploy the updated web,
mapserver, and characterserver folders. Both Python folders now include
`hub_service.py`; characterserver also includes `metrics.py`. Install their existing
requirements into the Python environment used to launch them.

Apply `sql/pending_updates/hub/003_managed_data_services.sql`, either through the
hub database updater with pending updates enabled or with the established offline
database update procedure. The migration preserves existing auth/world definitions
and adds `service_kind` and `cluster_key` to `hub_managed_services`. No CI changes
are required.

Add one managed-service record per local Python process. For example, with paths
adapted to the installation:

```sql
INSERT INTO hub_managed_services
    (service_key,name,executable_path,config_path,working_directory,enabled,service_kind,cluster_key)
VALUES
    ('characters','Character service','C:/SkyFire/control/.venv/Scripts/python.exe',
     'C:/SkyFire/characterserver/characterserver.toml','C:/SkyFire',1,4,'characters-1'),
    ('maps-eastern','Eastern map service','C:/SkyFire/control/.venv/Scripts/python.exe',
     'C:/SkyFire/mapserver/eastern.toml','C:/SkyFire',1,3,'maps-eastern');
```

Use service kind **3** for mapserver and **4** for characterserver; **0** remains
the native auth/world default. `cluster_key` must equal the TOML `node_key` and
certificate identity. Each data service needs a unique cluster identity. Service
keys use letters, digits, dashes and underscores and must not use `world`, `world-*`,
`authnet`, or `all`. The Python script is the fixed
`mapserver.py` or `characterserver.py` alongside the TOML configuration, so no
arbitrary command or script argument is supplied through the web interface.

On Linux, use the environment's absolute `bin/python` path and Linux install paths.
Reload hub database records or restart the hub after editing stopped definitions.
Launch paths cannot change while their managed process is active.

For the initial transition, gracefully shut down worlds, stop the externally
launched Python daemons, and remove their separate startup-helper invocations.
The hub does not adopt or kill externally started processes. It rejects a duplicate
start when the same cluster identity is registered. Start data services from the
web console and wait for readiness before starting worlds. Offline managed services
remain visible with a Start button. Remote/unmanaged map providers retain their
existing cluster restart capability; remotely starting an offline process requires
a process supervisor on that host and is outside this feature.

## Lifecycle and data safety

Buttons require node-operation permission and a valid CSRF token. They go through
the existing hub command queue and its backup-maintenance guard.

Stop and individual Restart require all local worlds to finish graceful shutdown
and all world cluster registrations to disappear. Use the world console's
`server shutdown <seconds>` countdown first. The character service independently
rejects a stop while client connections or database requests remain. A mapserver
rejects it during an active transfer. Rejections leave the process running.

A managed restart waits for a clean child exit and the previous registration to
disappear, then launches a fresh process with new supervision pipes. An abnormal
exit does not trigger a requested clean restart. Daemons are never force-killed by
the normal Stop timeout. "Restart all" continues to keep the character service
online while worlds save, and refreshes mapservers through the existing coordinated
sequence. Scheduled backups leave idle data services online. Hub shutdown now keeps
cluster leases alive while worlds finish saving before stopping managed data services.

If the hub dies unexpectedly, Python daemons remain online instead of treating
pipe loss as a forced data-service stop. Recover these processes manually before
handing ownership to a replacement hub. The existing character-service hub-lease
requirement still applies; this does not introduce transparent hub failover.

Managed Python diagnostics append to `hub-<node_key>.log` beside their script.
Startup failures are also visible as an exited managed process and exit code.

## Character health metrics

The character service sends authenticated versioned metrics to the hub every five
seconds. Reports include uptime, CPU percentage of total logical CPU capacity,
resident memory, active connections, pending requests, completed reads/writes,
transaction batches, failures, average request latency, and time since the last
completed commit. Pending includes queued and executing SQL work. Latency
includes queue time and is an average since process startup. A write count is a
request count, not a count of saved characters. Database readiness reflects the
service's database failure state.

Reports expire after 15 seconds and become unavailable when the registry lease
expires. They do not imply that a stopped process is healthy. The global sidebar
updates without replacing controls or editing forms. The health page adds character
CPU, memory, connection, pending-request, latency, and failure graphs, with gaps for
unavailable samples. Historical graphs cover the current browser session only.

## Verification

Python tests cover actual inherited Windows/POSIX handles, launch-token rejection,
busy-stop rejection, clean stop, fresh launch handles, metrics counters, database
transactions, and the mutual-TLS registration path. Browser fixtures cover actions,
permissions, offline/stale metrics, graph selection, and preservation of operator
input. Native C++ compilation and live managed-daemon validation are required when
deploying a build.
