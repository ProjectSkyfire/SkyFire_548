# Character service: ownership, login loads, and character saves

This experimental stage moves character database connections and SQL execution to
a separate `characterserver` process. When enabled, every operation through the
world's `CharacterDatabase` pool uses the service, including login reads, player
saves, inventory, pets, mail, guilds, and startup cleanup. The world does not open
a character MySQL connection or fall back to direct SQL if the service fails.

The character login holder now sends one `LoadCharacter` request containing the
character GUID, authenticated account ID, and declined-name option. The service
checks account ownership and selects all 42 login result slots in one
repeatable-read transaction. Missing characters and wrong-account requests return
an empty login result; database and transport failures remain fatal to the world.
Later gameplay-specific reads, such as pet loading, still use the SQL compatibility
interface.

`Player::SaveToDB` sends a dedicated `SaveCharacter` request. The world captures
the common live character fields once for creation and subsequent saves; it does
not send a primary SQL statement ID. The service chooses INSERT or UPDATE, adds
the GUID/realm/account identity fields, checks existing account ownership, and
commits the primary record, related component changes, and request receipt together.
Existing inventory, quest, mail, achievement, and other component serializers
still assemble their incremental statements in worldserver. Pet saves and other
standalone operations retain their existing transaction boundaries.

This is not yet a complete extraction of player serialization or a replicated
in-memory character model. It cannot recover updates that a world has not sent
before crashing. Periodic replicated state, semantic component deltas, and a fully
initialized gameplay standby are subsequent work.
[Preloaded process standby](WorldFallback.md) can wait
alongside the active world without opening character sessions.

## Installation

Build matching hub and world binaries. CMake build and INSTALL copy the Python
service, configuration template, and statement catalog into `characterserver/`
beside the installed server binaries.

This release uses character RPC version **2**. Stop both active and standby worlds
before deploying the matching world binary and Python assets, restart the character
service, then start the worlds. Older clients/services fail the handshake instead
of interpreting the new request layout. INSTALL includes `character_state.py`,
which defines the service-owned login plan. No additional database migration or
configuration setting is required for these load/save operations.

Python packages are installed separately:

```text
python -m venv character-venv
```

On Windows:

```text
character-venv\Scripts\python -m pip install -r characterserver\requirements.txt
```

On Linux:

```text
character-venv/bin/python -m pip install -r characterserver/requirements.txt
```

Use Python 3.11 or later. Copy `characterserver.toml.dist` to
`characterserver.toml`. Set the realm, node identity, numeric listener/advertised
address, hub endpoint, permitted world certificate identities, and MySQL settings.
Paths in this file are relative to the configuration file's directory.

Initialize or update the full character schema before enabling the service, with
all worlds and the character service stopped. The usual world character schema
updater is bypassed in service mode. This stage does not relocate that updater;
apply the release's character base and ordered updates offline. All tables must
use InnoDB. The service creates three `character_service_*` metadata tables in
the initialized schema and requires SELECT, INSERT, UPDATE, DELETE, and CREATE
privileges there. Grant this account access to that schema only. Backup/restore
operations may require additional privileges as described in the backup guide.

Issue the character service a certificate signed by the cluster CA, with its
`node_key` as the single common name, its advertised numeric IP in the SAN, and
both client and server TLS usage. World certificates must appear in
`allowed_world_nodes`. The hub must trust this CA and accept cluster registration.
If using hub node policies, register this identity as service type 4 with realm 0;
the service itself binds the real realm using `realm_id`.

Configure each participating world:

```ini
CharacterService.Enable = 1
CharacterService.NodeKey = "characters-1"
CharacterService.Host = "127.0.0.1"
CharacterService.Port = 54930
CharacterDatabaseInfo = ""
```

Use the existing `Cluster.CA`, `Cluster.Certificate`, and `Cluster.PrivateKey`
settings for the world's identity. They are resolved relative to its config file.
`RealmID` must match the service realm. Set `max_connections` to at least the sum
of the active world's synchronous and asynchronous character pool sizes. One
service owns one schema and one realm. The initial database executor is serial;
throughput and latency must be measured before increasing player counts.

Start the hub, then start the character service with its virtual environment's
Python executable:

```text
python characterserver/characterserver.py --config characterserver/characterserver.toml
```

After it registers as ready, start the world through the hub. The hub checks
the configured node identity, endpoint, readiness, available connections, and live
lease before launching a dependent world. The world also performs a TLS handshake
and exact statement-catalog check before opening its character pool. This Python
daemon can run independently or under [hub process supervision](ManagedDataServices.md)
for web Start, Stop, Restart, and live health metrics.
"Restart all" leaves it online while restarting gameplay nodes.

## Save and ownership guarantees

Each world boot has a fresh writer identity. All its pool connections share that
identity. Another world cannot acquire the schema while those sessions remain.
After they disconnect, takeover advances the durable writer epoch and permanently
fences the previous boot identity. Every request verifies the owner inside its
SQL transaction. A MySQL advisory lock prevents two service processes from owning
the same schema simultaneously.

Save batches and their request receipts commit in one transaction before the
service acknowledges success. Repeating an identical request cannot apply it
twice. Reusing an ID with different content is rejected. Legacy prepared TRUNCATE
operations become DELETE operations in service mode to avoid implicit commits.
Receipts from a permanently retired world are removed during takeover; receipts
for the current incarnation persist until then and need disk capacity monitoring.

The native client does not automatically retry an uncertain write. A rejected
operation, malformed reply, or transport failure terminates that world process
instead of reporting a successful save or an empty successful load. **This is
fail-closed experimental behavior, not graceful outage recovery:** unsent state
can still be lost. The service blocks requests without a live hub lease. Keep
the existing fallback ownership lock; preloaded standby waits before connecting.
Do not start two active
worlds against this realm. Restart dependent worlds after restarting the service.

Requests are limited to 8 MiB, transactions to 16,384 statements (including the
primary record in a character save), and responses to 64 MiB / one million rows
per result. The login response has one aggregate byte limit across all 42 slots;
an oversized result fails the whole load rather than returning partial character
data. Large realms may exceed startup query limits and
will need pagination. Public gameplay clients must never access this RPC port.

## Backups and recovery

Set `character_service_config` in the backup worker TOML to the character service
configuration so the worker reads credentials from there. Clear the old character
credentials from every world configuration. Scheduled/manual backups can run
after all world connections close; the idle character service may stay online.

Live web replacement of the character schema is blocked when this setting is
used. Service-aware restore fencing is not yet implemented. Restore that schema
offline with all worlds and the character service stopped, then start the service
and world in order. Restore the complete schema including service metadata.
Other database domains retain their existing restore workflow.

## Validation and development

`src/tests/character_service_test.py` verifies the catalog and protocol. Its optional
`--database-config <hubserver.conf>` integration fixture reads MySQL server
credentials and creates/removes only a randomly named disposable schema. It tests
atomic transactions, rollback, duplicate receipts, takeover fencing, service
restart, typed results, TLS identities, and hub registration. It also creates an
isolated schema from the character base table definitions to test real login loads
and create/update saves, account rejection, snapshot consistency under concurrent
external writes, aggregate response limits, and recovery after service restart.
The service login plan is checked against the native legacy holder layout.
Integration tests
also require `cryptography`; it is not a daemon runtime dependency.

When changing the native character statement definitions, regenerate the catalog:

```text
python src/server/characterserver/export_catalog.py --source . --output src/server/characterserver/statements.json
```

Ship the generated catalog and matching world binary together. Native compilation,
live login/save/logout, and fallback validation are still required before deployment.


## Database connection health and recovery

The daemon checks its MySQL connection and advisory ownership lock every five
seconds on the same serialized database worker used by character operations.
This also keeps idle connections active while worlds are stopped. No concurrent
health query can run inside a character transaction.

A lost connection or ownership lock marks the service unavailable and closes
existing world RPC connections. Recovery waits for their session accounting to
drain, opens a new database connection, reacquires the exclusive advisory lock,
and reapplies the session settings and schema checks before becoming ready. A
second character daemon holding that lock prevents recovery. World connections
must perform fresh authenticated handshakes and pass the existing epoch checks.

A failed BEGIN is handled like a failed query or COMMIT. The service never
automatically replays a failed write. Save receipts remain committed atomically
with their writes, so a matching request ID can establish that an uncertain
commit already occurred without applying it twice. This does not provide seamless
world survival: the existing native world client stops on an uncertain character
operation to prevent gameplay continuing on unsaved state.

Validation includes disposable MySQL tests for idle timeout, killed connections,
active-session draining, competing ownership, and lost commit acknowledgements.
The test runner accepts --database-config pointing to a private config containing
VerificationDatabaseInfo (or HubDatabaseInfo); tests create and remove only their
randomly named skyfire_character_test_* fixture schema.
