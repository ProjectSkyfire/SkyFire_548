# Native clustered chat daemon

The daemon provides realm-scoped presence, message routing and admission of
channel/guild administration and in-game GM requests. `ChatService.Messages`
opts worlds into the broader route; the legacy whisper-only option remains.
World adapters retain client packets, final gameplay/RBAC checks and durable
social/guild/channel state. Cross-world fanout and durable ownership extraction
are not implemented yet. See the September 24 routing section below for activation,
limits and the acceptance matrix. Native compilation and live validation are pending.

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
remain validated by the world adapter using its client-data stores.

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
supervisor. Routing drain is not exposed yet; stop/restart interrupts pending requests with bounded failure.

Independent server-health refresh shows uptime, configured realms, connections,
service requests, published player presence, accepted whisper relays and failures. Metrics expire after 15 seconds; an expired metric is
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
(version 3). The new hub also accepts versions 1 and 2 from earlier chat daemons.
Enable `ChatService.Enable = 1` in each participating world configuration and set
`ChatService.Host`, `ChatService.Port` and `ChatService.NodeKey`. The publisher
reuses that world's cluster certificate and CA, verifying both endpoint hostname
and the chat certificate CN. `RealmID` must be explicitly positive. Standby worlds
start publishing only after active world startup; preloading alone sends no players.

Every five seconds, the world thread copies online character identities into a
bounded snapshot. DNS, TLS and network waits run on a separate worker. Only one
pending snapshot is retained; newer snapshots replace unsent older ones. There
are no database writes. Transport failures do not stop world startup, gameplay,
character saves or local chat. Whispers explicitly configured for relay fail closed. Publication success/failure transitions are logged.

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

## Ordinary whisper relay (opt-in)

Set `ChatService.Whispers = 1` on a world with presence publication enabled and
matching hub/chat/world builds. The default is zero. This first relay supports two
players on the **same world node and realm**, matching the current single active
world per realm. It is not cross-node or cross-realm delivery. Newly logged-in
players need their next accepted presence snapshot before relayed whispers work.

The normal world handler parses commands, applies flood/mute/language/link checks
and validates the recipient before queuing. A bounded background worker sends
operation 3 to chatserver. Chatserver verifies the publishing world's certificate
scope and current sender/recipient generation, account and login incarnations,
then returns the unchanged message. Worldserver accepts only an exact reply on
that authenticated request and rechecks both login incarnations, mute status,
recipient whisper filter, whitelist, level, faction and GM silence before delivery.
The existing `Player::Whisper` path runs once, retaining chat hooks, sender informs,
whitelist updates and AFK/DND replies. Addon whispers and GM command parsing retain
their existing local paths.

There are at most 128 queued/in-flight/completed requests per world and four per
account. A request expires five seconds after admission, including queue, network
and completion wait. Network work never blocks a world tick. Relay failure reports
that the message was not sent; it never silently falls back to local delivery or
retries. Logout/relogin invalidates queued delivery. No chat text is logged or
persisted. The relay counter means the daemon returned a message, not that the
recipient actually received it.

Operation 3 uses the 16-byte header followed by a u32 payload length (maximum 512
bytes). Payload: generation string, u32 sender account, u64 sender GUID and login
incarnation, u64 recipient GUID and login incarnation, then a length-prefixed UTF-8
message of 1�255 bytes. The reply is the header with operation `0x8003` followed by
the identical payload (no length field); the caller already knows its exact size.
Malformed, stale, unauthorized or unserved requests close without an accepted reply.

`chat_whisper_tests` covers malformed text, account spoofing, realm/node isolation,
lease expiry and sender/recipient relogin. Live acceptance should cover normal
whispers, AFK/DND, mute/filter/faction restrictions, logout during a request, daemon
loss and successful recovery after presence is republished. Native tests and live
acceptance must be run after building; no test servers are modified by this change.

## Gameplay routing phases

Whisper relay is the first delivery slice. These are the full routing requirements:

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

Next is cross-node whisper delivery, followed by group, guild/officer and
private-channel delivery. Local spatial chat
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
admission. In-game whisper relay acceptance remains pending compilation and a two-player test.

## Player routing and administrative admission (September 24)

`ChatService.Messages = 1` opts a world into the new routing path. It requires
`ChatService.Enable = 1` and includes the existing whisper relay even when the
older `ChatService.Whispers` option is zero. Standalone defaults remain disabled.
Rebuild/install **hubserver, worldserver and chatserver together**, then restart
chat and the worlds. The hub must accept version 4 chat metrics. No SQL migration
or new database credentials are needed for this routing stage.

The world publishes an authenticated, per-message audience projection. Chatserver
checks realm, certificate identity, world generation, audience revision, account,
player incarnation and speaking rights, then selects current listening members.
The reply contains recipient IDs and session incarnations, not client packets.
World applies current permissions again and encodes/delivers the existing client
packet. A lost reply or timeout does not fall back to an uncontrolled local send.
No request is retried as a new message. A rejected request reports failure.

Covered player-message paths:

- Say, yell and text chat emotes, preserving spatial visibility, phase, shared
  vision, vehicle/farsight and faction selection in the gameplay authority.
- Party/subgroup, raid, raid warning and instance/battleground group paths.
- Guild/officer speech and listening rights, ignores and addon-prefix filtering.
- Public/private channel messages with current membership, mute and ignore checks.
- Existing ordinary whispers and addon whisper/group/guild paths. Binary addon
  text stays byte-exact and has a separate admission budget.

Channel handlers and non-economic guild administration now request chat admission
before invoking their authoritative world adapter. This includes channel joins,
passwords, ownership/moderation, mute/kick/ban operations and lists; guild roster,
invites, membership, ranks, notes, MOTD/info, leadership and news operations.
In-game GM commands follow the same route and retain the player's identity and
RBAC checks. Console/hub commands remain on the existing operator control path.
Sensitive command arguments, passwords and raw client packets stay in a bounded
world-local pending request; chat receives the operation category/name. Nothing
executes after the actor's session has changed. A guild membership/invitation
change requires the operator to retry instead of applying an old request.

Human messages, addon messages and administration have separate pending limits;
requests expire after five seconds. All network I/O stays on the client worker.
The world consumes at most 32 fanout/control replies per tick. Spatial recipients
are collected with the existing grid visitor and rechecked before delivery.
Server health exposes routed messages, selected recipients and admitted controls.
These count service decisions, not confirmed client delivery or committed mutations.

### Ownership and remaining extraction

This stage routes same-world audiences within each served realm; it is **not**
cross-world fanout. Channel ownership/moderation state, guild/social state and
persistence still reside in worldserver's adapters. Moving that state into the
daemon requires typed character-service persistence, durable receipts/outbox,
restart recovery and fenced domain ownership. It must not be implemented by giving
chatserver unrestricted character-database credentials or by allowing two writers.
Guild bank, inventory, progression, gameplay effects, scripted NPC dialogue and
animation emotes remain with their existing authoritative services.

The complete chat-service extraction is therefore not finished by enabling this
switch. Cross-world delivery, service-owned channel/guild state, friends/ignore
projections and chat standby remain explicit follow-up gates.

### Build and live acceptance

Native compilation is performed by the operator. Run `chat_protocol_tests`,
`chat_presence_tests`, `chat_whisper_tests` and `chat_routing_tests` after rebuilding.
The routing suite exercises realm/session isolation, officer confidentiality,
revision invalidation, duplicate rejection, deadlines, binary addon text and
administrative recipient constraints.

With routing enabled, verify counters increase for each message/admin surface,
then stop chat: new routed operations must fail without local execution. Repeat
with a rank change, mute, kick, guild leave, logout/relogin and world fallback while
a reply is pending. Check raid subgroups, officer-only listeners, addon prefixes,
channel password secrecy and two distinct realms. Test authorized and unauthorized
GM commands and confirm console/hub maintenance commands remain usable. Finally
repeat with routing disabled to validate standalone behavior. A successful prior
local-chat test does not establish this new path's acceptance.
