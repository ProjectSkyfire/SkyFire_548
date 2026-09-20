# Mapserver terrain/collision proof of concept

A mapserver is a Python 3.11+ read-only asset daemon registered with the hub. Multiple named providers can export different continents. Worldserver downloads and SHA-256 verifies their assets before map initialization, then reads an immutable local cache. Players, scripts, movement, combat and map simulation still run in worldserver. This phase distributes asset storage and delivery; it does not yet distribute gameplay CPU or memory.

## Install and configure

1. Reconfigure CMake and build/install hubserver and worldserver together. INSTALL includes `mapserver/` and updated `.conf.dist` files. Existing operator configuration files are preserved. No SQL update or CI change is required.
2. Install Python 3.11+ and install `mapserver/requirements.txt` into the mapserver's virtual environment. Worldserver's fetch helper uses only the Python standard library.
3. Issue a separate cluster CA certificate for each provider. Its single common name must match `node_key`; its SAN must contain the advertised numeric IP. Enable both serverAuth and clientAuth usage. Use the existing world certificate for download authentication. The hub certificate must match `Cluster.HubHost`.
4. Copy `mapserver.toml.dist` to an operator-owned TOML file for each provider. Assign unique node keys and endpoints. Set `maps = [0]` for Eastern Kingdoms, `[1]` for Kalimdor, `[530]` for Outland or `[571]` for Northrend. An assignment includes only that exact map ID: instances, transports and phase-specific maps need their own assignments or remain local.
5. Set `data_root` to the extracted build-18414 data containing `maps`, `vmaps`, and `mmaps`. Each provider exports its selected tiles/root files plus all shared collision `.vmo` files and `GameObjectModels.dtree`. All providers and any remaining local assets must come from the same extraction set. Do not modify source files while a provider runs; stop it, replace the data, then restart it.
6. Add the world certificate CN to the provider's `allowed_world_nodes` and the hub's space-separated `Hub.MapData.Readers`. Both allowlists are required. Restrict access to the cluster and map HTTPS ports to participating servers.
7. Start each provider with `python mapserver/mapserver.py --config path/to/maps-eastern.toml`. Run it as an OS-managed service for unattended operation. It registers and heartbeats through the hub and appears in cluster status; hub managed-process Start/Stop does not launch this Python daemon.
8. Configure worldserver:

```ini
MapData.Enable = 1
MapData.Sources = "maps-eastern=0 maps-kalimdor=1"
MapData.Python = "D:/Test Server/SkyFire_Authnet/control/.venv/Scripts/python.exe"
MapData.Bootstrap = "mapserver/fetch_maps.py"
MapData.CachePath = "map-cache"
MapData.StartupTimeout = 1800
```

Set `Cluster.HubHost`, `Cluster.HubPort`, `Cluster.CA`, `Cluster.Certificate`, and `Cluster.PrivateKey` to the existing cluster connection. Use an appropriate Python executable on Linux. Paths are relative to the world configuration file (daemon paths are relative to its TOML file). On Linux with configuration under `etc`, set explicit absolute helper/cache paths to the installed `bin/mapserver` location. A bare Python executable uses PATH. These map settings take effect only on restart; configuration reload does not switch a running world's cache.

Each map can have one provider. Up to 16 providers and 64 selected map IDs are supported. There is no automatic provider failover or gameplay migration. Unknown/unready/drained/disabled providers block a new bootstrap. Direct asset transfers use mTLS HTTPS after hub discovery; asset bytes do not transit the hub.

## Nearby memory loading and health

The main continents' verified files stay in the local cache. Terrain, collision and navigation tiles load into worldserver memory when needed; nearby tiles are warmed ahead of player movement. This does not keep whole continents resident or move gameplay simulation to mapserver.

`MapData.PrefetchRadius = 1` warms the surrounding 3x3 grid area on configured base maps. Set 0 to disable or 2 for a 5x5 area. `MapData.PrefetchTilesPerSecond = 1` limits additional tile loads per base map per second (1..8). Player priority rotates. Existing grid expiry is refreshed while nearby; after players leave, normal grid unloading releases memory. Preloading does not spawn creatures. It stops scheduling additional loads after 5 ms of work; a single synchronous local tile load can exceed that allowance. Required tiles still use the existing immediate map loader during entry, teleports and movement.

Mapserver reports its assigned map IDs, uptime, CPU percentage, resident memory, active transfers, asset count, requests, failures and cumulative data sent through its authenticated cluster connection. Server Health includes CPU/memory/transfer/traffic graphs; the global sidebar includes compact map metrics. Updates use the existing independent status poll and expire after 15 seconds without a report. Traffic graphs reflect actual asset delivery; with a warm world cache, normal gameplay generates no mapserver network traffic. Counters reset on daemon restart and saturate at the protocol's uint32 limit (sent data is counted in KiB).

Build/install the updated hub and world together, and restart the mapserver Python daemons with the updated scripts: older hubs do not understand the new metrics frame. No SQL migration is required.

## Startup, cache and failures

Every hub-managed world start (console, web, soft restart and backup restart) first reads that node's configuration and checks all assigned providers against the live hub registry. An unavailable, unready, expired, drained, disabled or full provider blocks launch and names the dependency in the error. The world process is not spawned. Start the provider and retry. This uses the latest heartbeat lease, so worldserver additionally verifies discovery, HTTPS access and assets before map initialization to catch failures after preflight. Worlds with map distribution disabled keep their normal startup.

Before the first world launch, prewarm with `python mapserver/fetch_maps.py --world-config path/to/worldserver.conf`. The first download and full verification may take a while. The hub's existing managed-process startup deadline may show worldserver as unresponsive while a large cache is verified; do not include first-time cache population in an automated backup restart cycle.

Concurrent cache preparation waits for the cache lock up to `MapData.StartupTimeout`, allowing world startup during prewarming without an immediate lock error. World startup re-discovers providers and re-verifies cached data. Missing providers, conflicting shared collision hashes, invalid manifests, modified assets and corrupt published caches fail startup before readiness. The previous `active` marker is preserved after a failed transfer. A running world keeps its selected snapshot path and needs no mapserver network requests; provider outages do not interrupt its map reads. Selected maps never silently fall back to local files.

Snapshots consume local disk. Bounds are 100,000 files, 64 MiB per file, 16 GiB combined assets, and a 32 MiB manifest per provider. Shared models are deduplicated when identical. Downloads resume from verified files in an unpublished partial snapshot. Corrupt published snapshots require a new empty cache path; do not edit or delete a snapshot while any world uses it. Old snapshots are not automatically pruned. Keep the cache writable only by the world service account.

DBC/DB2 and unassigned maps continue to use `DataDir`. Disabling `MapData.Enable` restores the normal local map paths after restart, so retain the local extraction set for rollback.

## Verification

`python src/tests/mapserver_integration.py` uses temporary certificates/data, two real HTTPS mapserver daemons and a mock cluster listener. It checks registration, discovery, both map assignments, cache reuse, TLS identity denial, duplicate assignments, unavailable providers, corruption, and preservation of the active snapshot. OpenSSL CLI and aiohttp are required. CTest includes `map_data_directory_tests` for hub routing rules. `python src/tests/map_cache_lock_test.py` covers concurrent prewarming and timeout. Browser fixtures `tools/dev/tests/map_health.test.cjs` and `hub_status.test.cjs` cover map graphs, stale data and independent status updates. C++ compilation and real hub/world startup, movement/collision, cross-continent travel and provider-outage checks remain required before rollout.
