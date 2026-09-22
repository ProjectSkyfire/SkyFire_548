# Native clustered chat daemon

This first implementation provides the daemon and hub integration. Player chat
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
- Set `Chat.AllowedWorlds` to space-separated exact world certificate CNs, including
  standby identities as appropriate. An empty list fails startup. This allowlist
  authorizes health probes only; it does not confer player or GM privileges.
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
health probes and failures. Metrics expire after 15 seconds; an expired metric is
unavailable rather than zero. A lost hub registration withdraws managed readiness.
The current stateless health service stays online during scheduled database
backups and does not prevent backup admission. This policy must change before
chat acquires durable mutations or an outbox.

## Service protocol and realm boundaries

The direct endpoint requires mutual TLS and an allowed world certificate CN.
It accepts one fixed 16-byte request per connection: ASCII `SFCH`, big-endian u16
version `1`, u16 operation `1` (health), u32 nonzero request ID, and u32 nonzero
realm ID. A served realm receives the same fields with operation `0x8001`.
Unknown realms, malformed requests, unsupported versions/operations and
unauthorized peers are closed. The deadline and connection limit bound retained
work; the main loop processes at most 64 asynchronous completions per iteration.

Hub registration uses realm zero because the daemon may serve multiple realms;
the chat metrics packet (cluster message 11) reports its actual realm list. This
does not publish authentication realm routes or grant ownership of those realms.

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

Next is the world-to-chat connection, realm/session presence and whisper routing,
followed by group, guild/officer and private-channel delivery. Local spatial chat
and emergency world commands remain available during a chat-service outage.

## Validation

`chat_protocol_tests` covers realm-qualified probes, malformed/truncated metrics,
duplicate/zero/excessive realm lists, service-role checks and lease ownership.
After compiling, run it with CTest together with `cluster_foundation_tests`.
Native compilation and live TLS/lifecycle validation remain required before
deploying this phase. Test at least two realms, disallowed certificates, unknown
realms, connection exhaustion, hub loss/recovery, managed restart and backup
admission. There is no in-game chat acceptance claim for this foundation.
