# Native clustered chat daemon

This implementation provides the daemon, hub integration and optional world presence publication. Player chat
still runs in worldserver. It does **not** yet route messages, run GM commands,
own guild state, or write a database. Its `ready` status means its authenticated
health endpoint is available and the hub acknowledged its registration.

## Build and installation

Reconfigure CMake with `SERVERS` enabled and build/install `chatserver` along with
the updated hubserver. It uses the existing native Boost/OpenSSL/shared-library
dependencies, without linking the game library or requiring Python. CMake INSTALL
ships the executable and `chatserver.conf.dist`; copy the template to
`chatserver.conf`. Existing live configuration files are not overwritten.

The hub must understand service type 5 and capability 2048 before starting chat.
Older hubs reject that registration. No database schema migration is needed if
the managed data-service columns from `003_managed_data_services.sql` are present.

## Configuration

- Give every daemon a unique `Cluster.NodeKey` and matching certificate CN.
- Configure the hub hostname/port and CA. The node certificate needs clientAuth
  for hub registration and serverAuth plus an endpoint SAN for world connections.
  Certificate paths resolve relative to `chatserver.conf`.
- Set `Chat.BindIP`, `Chat.Port`, and `Cluster.AdvertiseAddress` for the host.
- Set `Chat.Realms = "1 2"` to serve multiple realms from one daemon. Up to 64
  distinct positive realm IDs are supported. Zero and duplicate IDs are rejected.
- `ConfVersion` is checked at startup against the chat serial in `ConfigVersion.h`.
  Missing or older serials log a warning with the expected version. Compare the
  shipped defaults before updating the serial; existing configs are never overwritten.
- Set `Chat.AllowedWorlds` to space-separated exact world certificate CNs, including
  standby identities as appropriate. An empty list fails startup. This allowlist
  authorizes health probes; it does not confer player or GM privileges.
- Set `Chat.WorldRealms = "world-primary=1 world-other=2"` to authorize presence
  publication for each certificate identity. Identities must also be in
  `Chat.AllowedWorlds`, and realms must be in `Chat.Realms`. An empty scope list
  permits health probes only.
- `Chat.MaxConnections` bounds concurrent TLS sessions (1–128, default 32).
  `Chat.RequestTimeout` bounds the entire handshake/probe/reply (1–30 seconds).

No DBC/DB2 data files are required for this phase. Region/channel eligibility will
use the shared client-data foundation when gameplay routing is implemented.

## Hub management

Create a `hub_managed_services` row with a unique service key such as `chat`,
`service_kind=5`, and `cluster_key` matching the chat configuration. Use the native
chat executable, configuration path and working directory for its launch paths.
For example, adapt this statement to the installation directory:

```sql
INSERT INTO hub_managed_services
  (service_key, name, executable_path, config_path, working_directory,
   enabled, service_kind, cluster_key)
VALUES
  ('chat', 'Chat Primary', 'chatserver.exe', 'chatserver.conf',
   'C:/SkyFire', 1, 5, 'skyfire-chat-primary');
```

On Linux use the installed `chatserver` executable path and the installed config
directory. Reload managed records with the hub console's `reload db_records` command, then
use `start chat`, `stop chat`, or `restart chat`. The existing role-checked web
service controls expose the same actions. The inherited control channel checks
its launch token; the daemon also checks that the expected cluster key matches
its configuration before starting.

Managed chat can restart while worlds are online. Coordinated Restart all also
includes managed chat nodes. Unmanaged chat nodes must be operated by their own
supervisor. Routing drain is not exposed yet because no gameplay routing exists.

Independent server-health refresh shows uptime, configured realms, connections,
service requests, published player presence and failures. Metrics expire after 15 seconds; an expired metric is
unavailable rather than zero. A lost hub registration withdraws managed readiness.
The current volatile presence service stays online during scheduled database
backups and does not prevent backup admission. This policy must change before
chat acquires durable mutations or an outbox.

## Service protocol and realm boundaries

The direct endpoint requires mutual TLS and an allowed world certificate CN.
Health uses one fixed 16-byte request per connection: ASCII `SFCH`, big-endian u16
version `1`, u16 operation `1` (health), u32 nonzero request ID, and u32 nonzero
realm ID. A served realm receives the same fields with operation `0x8001`.
Unknown realms, malformed requests, unsupported versions/operations and
unauthorized peers are closed. The deadline and connection limit bound retained
work; the main loop processes at most 64 asynchronous completions per iteration.

Hub registration uses realm zero because the daemon may serve multiple realms;
the chat metrics packet (cluster message 11) reports its actual realm list. This
does not publish authentication realm routes or grant ownership of those realms.

## World presence publication

Deploy matching hubserver, chatserver and worldserver builds for presence metrics
(version 2). The new hub also accepts version 1 metrics from the foundation daemon.
Enable `ChatService.Enable = 1` in each participating world configuration and set
`ChatService.Host`, `ChatService.Port` and `ChatService.NodeKey`. The publisher
reuses that world's cluster certificate and CA, verifying both endpoint hostname
and the chat certificate CN. `RealmID` must be explicitly positive. Standby worlds
start publishing only after active world startup; preloading alone sends no players.

Every five seconds, the world thread copies online character identities into a
bounded snapshot. DNS, TLS and network waits run on a separate worker. Only one
pending snapshot is retained; newer snapshots replace unsent older ones. There
are no database writes. Transport failures do not stop world startup, gameplay,
character saves or local chat. Publication success/failure transitions are logged.

Presence operation 2 uses the same 16-byte header followed by a big-endian u32
payload length (maximum 512 KiB). The payload is a length-prefixed 64-character
random generation, u64 sequence, u16 count and entries containing u32 account,
u64 character GUID, u64 login incarnation and length-prefixed UTF-8 character name.
All integers are big-endian. Success returns the header with operation `0x8002`.
Each request owns a short TLS connection; presence persists under a 15-second
lease. It is not tied to the lifetime of that one request socket.

At most 4,096 players per world snapshot, 128 world/realm registrations and 16,384
players globally are accepted. Duplicate names or GUIDs within a realm, stale
sequences and competing unexpired generations are rejected atomically. Identical
GUIDs/names in different realms are independent. A different world cannot replace
an existing player's live presence until its old owner expires or withdraws it.
An empty next snapshot withdraws logged-out players; crash or network loss expires
presence within 15 seconds of the last accepted snapshot. World restart may need
to wait out that lease. Presence is not character writer fencing or GM authorization.

## Gameplay routing phases

The following are required features, not implemented by the health endpoint:

| Feature | Required routing and authority |
| --- | --- |
| Whispers | Realm-qualified sender/recipient, session incarnation, ignore/visibility/faction checks, delivery outcome. |
| Party and raid | Realm-qualified group ID, membership revision, leader/assistant rules and subgroup scope. |
| Guild and officer | Realm-qualified guild ID and authoritative membership/rank permissions. Officer chat must not fan out to ordinary members. |
| Private channels | Realm-qualified channel identity; passwords, owner/moderator roles, bans, invites and moderation parity. |
| Public channels and addons | Zone/DBC eligibility, faction and language rules, registered addon prefixes and bounded fanout. |
| GM commands | Actor identity, realm/target, correlation ID, deadline, RBAC recheck by the authoritative executor and redacted audit. |

All presence, group, guild and channel keys must include the realm ID. Equal
numeric character/guild/group IDs or channel names in different realms must
never collide. Cross-realm chat is an explicit future policy, not implied by
hosting two realms in one daemon. Worldserver initially owns membership and
permission projections; stale projections fail closed. Durable guild/social
ownership requires typed character-service APIs and scoped writer fencing.

Next is whisper routing using the world-to-chat connection and realm/session presence,
followed by group, guild/officer and private-channel delivery. Local spatial chat
and emergency world commands remain available during a chat-service outage.

## Validation

`chat_protocol_tests` covers realm-qualified probes, malformed/truncated metrics,
duplicate/zero/excessive realm lists, service-role checks and lease ownership.
`chat_presence_tests` additionally covers cross-realm collisions, replay rejection,
relogin, conflicting owners, logout snapshots and expiry. After compiling, run
both with CTest together with `cluster_foundation_tests`.
Native compilation and live TLS/lifecycle validation remain required before
deploying this phase. Test at least two realms, disallowed certificates, unknown
realms, connection exhaustion, hub loss/recovery, managed restart and backup
admission. There is no in-game chat acceptance claim for this foundation.
