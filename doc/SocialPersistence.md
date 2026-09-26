# Character-service social persistence contract

This is an opt-in persistence foundation for the native chat daemon. It does not
yet migrate channel or guild administration from worldserver. Leave
`allowed_chat_nodes = []` until the native owner, legacy writer cutover and client
parity checks are implemented. Existing world character RPC remains version 2.

## Installation and authority

The pending character migration `001_social_domain_store.sql` creates five new
tables. It does not alter legacy guild/channel rows. Apply it through the character
schema update workflow before enabling chat identities. The daemon checks for the
tables at startup when the allowlist is nonempty. No CI changes are included.

Chat identities must be separate from `allowed_world_nodes`. Mutual TLS, certificate
revocation checks and a live hub lease are required for both roles. A chat identity
cannot call world SQL operations; world raw SQL cannot access the social tables.
The configured character service realm is the only realm that connection can use.
A multi-realm chat daemon needs one authorized persistence endpoint per realm.

The character service retains the database advisory lock. Social ownership is
separate for `(realm, channels)` and `(realm, guilds)` and does not take ownership
of player saves. An owner is identified by certificate node, process incarnation
and durable epoch. A replacement can attach only after the old domain sessions
have detached. Replacement retires the old incarnation permanently; old requests
cannot acquire ownership again or mutate the new owner's state.

## Wire format

Use the existing character service TLS endpoint and framing: network-order u32
payload size followed by the payload. Replies are framed the same way and begin
with status byte 0 for success or 1 for rejection. A rejection closes the session.
Strings use u32 byte length plus UTF-8 bytes. There is no SQL field in this API.

- Handshake: byte 16, byte 1 (social protocol version), u32 realm, string process
  incarnation (32 lowercase hexadecimal characters), string domain (`channels`
  or `guilds`). Success has no body. Epoch and domain bind to the connection.
- Operation 17: byte 17 and length-prefixed JSON `{ "after": "", "limit": 32 }`.
  Pages records by their binary UTF-8 key. Result has `head` and `records`; each
  record contains `key`, `revision`, and `document`. Tombstones have null documents.
- Operation 18: byte 18 and length-prefixed JSON with exactly `id`, `key`,
  `expected`, `actor`, and `document`. `id` is a 32-character lowercase hexadecimal
  request ID, `expected` is the last record revision (0 for a new key), and actor
  is the character GUID attested by the trusted chat authority. The result contains
  the committed `revision`.
- Operation 19: byte 19 and length-prefixed JSON `{ "after": 0, "limit": 32 }`.
  Returns `head` and ordered `events`, containing revision, key, document and actor.

JSON keys must be unique. Integers cannot be booleans. Unknown fields, unsupported
operations, oversized documents and invalid UTF-8 are rejected. A document is at
most 256 KiB; keys are at most 192 bytes; pages contain at most 32 records/events.
Frames remain bounded by the existing transport limit.

## Stored documents

Channel documents contain exactly `name`, `team`, `channel_id`, `announce`,
`ownership`, `password_verifier`, and `bans`. Plaintext passwords are rejected;
nonempty verifiers use `pbkdf2-sha256$iterations$salt$digest`, with a 16-byte hex salt
and 32-byte hex digest. The future channel owner must choose and enforce its
password policy. Presence, online channel members and moderator session state
must be reconstructed by the native owner; this API does not infer them.

Guild documents contain exactly `id`, `name`, `leader`, `motd`, `info`, `ranks`,
and `members`. Ranks contain `name` and `rights`; members contain `guid`, `rank`,
`public_note`, and `officer_note`. The leader must be a member, ranks must exist,
and member GUIDs must be unique. Bank, currency, inventory, progression and game
effects are deliberately outside this schema. This is not yet the complete guild
administration contract: pending invitations and their notifications need their
own typed state during the native-owner implementation.

The API validates persistence shape and ownership, not player permissions. The
authenticated chat owner must validate actor membership, current permissions,
realm scope and command intent before submitting a state change. Do not expose
this trusted-service API directly to clients or allow arbitrary web submissions.

## Commit, recovery and reconstruction

Each mutation atomically commits its record, receipt, ordered outbox event and
domain revision. Retrying the same request ID and content under the same owner
returns the original revision. Reusing an ID with different content is rejected.
Deletion writes a versioned tombstone rather than resetting the record to revision
zero. Lost acknowledgements must not be retried under a new request ID.

On startup, the native owner must reconstruct records and replay outbox events
before serving mutations. Record revisions make replay idempotent; a paginated
snapshot is not one global transaction, so record the first head and replay events
after it, applying only revisions newer than each reconstructed record. A successor
must reconstruct committed state before issuing any new mutation. Historical
receipts are not permission for a retired incarnation to reconnect.

Database recovery waits for both world and social sessions to drain before opening
a new ownership connection. The service does not automatically replay writes.
Outbox and receipt pruning is not enabled; introduce an acknowledged consumer
watermark and a documented retry window before adding retention.

## Native connection and recovery

The native chat daemon now contains a background persistence worker for each
configured realm. `Chat.Persistence.Enable = 0` preserves the current behavior.
When enabled, configure `Chat.Persistence.Realm.<id>.Host`, `.Port` and `.NodeKey`
for every `Chat.Realms` entry. Host is a numeric IP with a matching certificate
SAN; NodeKey must match the character service certificate identity. Client
credentials are the chat daemon's cluster certificate and key. No MySQL
credentials are added to chatserver.

Each worker attaches separately to channels and guilds, reads paginated snapshots,
and replays events from the initial head before marking its realm ready. Missing
events, regressing revisions and malformed records fail recovery. Each domain
cache is bounded to 8192 records and 4 MiB of serialized keys/documents. A ready
realm can continue handling requests while another realm recovers; the overall
hub readiness flag is false until all configured realms are ready.

The internal mutation API admits at most 32 outstanding requests per realm and
128 overall, including unconsumed results. Queued work expires before transmission
after ten seconds. Each network phase has a five-second deadline; SQL/TLS work
never runs on the chat event loop. Results distinguish committed, rejected,
unknown and not-sent outcomes. Connections and snapshots rebuild after failure;
unknown writes are not automatically resubmitted. Readiness and cache access are
synchronized with the worker; a stale revision still fails at the database.

Boost.JSON is compiled from the existing Boost headers in the native target; no
additional JSON binary package is required. Build chatserver and the
`social_snapshot_tests` target to validate the native implementation. The native
test covers paginated reconstruction, stale-event suppression, tombstones, gaps,
regressing heads, invalid revisions and cache bounds. Native compilation remains
pending; Python database/TLS tests do not substitute for this check.

## Validation and remaining cutover

The disposable MySQL/TLS tests cover separate role admission, world SQL rejection,
realm mismatch, CAS conflicts, tombstones, takeover fencing, continued world reads,
lost commit acknowledgement, and rollback when receipt insertion fails. Run
`src/tests/character_service_test.py --database-config <private fixture config>`.

Channel/guild command integration, invitations, legacy data import, disabling every old
writer, notification parity and cross-world fan-out remain required before this
can replace the working routing/admission path. Do not enable it as a substitute
for those changes.
