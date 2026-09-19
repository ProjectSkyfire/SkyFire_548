# Hub remote control

Optional Python HTTPS/WebSocket gateway and PHP-rendered dashboard. Disabled by default.
The hub is the only authority for identities, permissions, database access, nodes and process
lifecycle. The gateway connects exclusively to the configured numeric loopback hub HTTP
listener. PHP renders a fixed shell once at startup; it receives no requests or credentials.
The existing local hub console remains available.

## Installation

CMake builds and INSTALL copy `control/` alongside `web/`. Existing `control.toml`, tokens,
certificates, audit files and virtual environments are preserved. Python/PHP runtimes and
packages are installed separately; CMake never downloads or executes them.

1. Install Python 3.11+ and a supported PHP 8.2+ CLI. Create a virtual environment and run
   its Python with `-m pip install -r requirements.txt`.
2. Copy `control.toml.dist` to `control.toml`. Set `php` to the PHP executable.
3. Create a private file containing 64 cryptographically random lowercase hex characters.
   Set `token_file` and hub `Web.ControlTokenFile` to that same file. Restrict this file and
   TLS keys to the service identity. Keep the hub web listener on numeric loopback with
   `Web.AllowRemote = 0`. Apply `sql/updates/hub/2026_09_18_hub_02.sql` first.
4. Configure a TLS certificate trusted by your browser, its private key, and matching
   HTTPS `public_origin`. For remote access explicitly set `allow_remote = true` and the
   desired numeric bind address. Do not expose the private hub HTTP port.
5. Use the local hub console to create an identity if needed, then explicitly grant a role:
   `admin role <username> <viewer|operator|administrator|recovery> [remote]`.
   The `remote` suffix is required for this HTTPS gateway, including localhost.
   Omit it to revoke gateway access. Existing identities are not elevated automatically.
6. Set `enabled = true`, start hubserver, then launch the virtual environment Python with
   `controlserver.py --config control.toml`. Run under your OS service manager as desired.
   Stopping this gateway does not stop game servers. No firewall rules are changed.

Viewer can monitor. Operator adds typed lifecycle commands. Administrator retains the local
hub's administrative permissions; this gateway deliberately exposes no general account,
SQL, file or process-configuration editor. Recovery grants monitoring plus a reserved
recovery bit; backup/restore actions will arrive in Phase 8. It does not grant lifecycle
operations. Hub sessions use absolute expiry and fresh credential/permission checks;
password changes, disabling an identity and removal of remote access revoke sessions.

## Protocol v1

All public routes start `/control/v1/`. Login and logout use POST; status, capabilities and
command results use GET. Cookie sessions require HTTPS and same-origin POSTs, plus
`X-Control-CSRF` on authenticated mutations. Login accepts `{username,password}`.
Status supplies the CSRF token. Credentials are never sent to PHP or stored in browser
storage. The backend never trusts forwarded client-address headers.

Command POST `/commands` accepts exactly:

```json
{"version":1,"type":"command","requestId":"0123456789abcdef0123456789abcdef","timestamp":"2026-09-18T20:00:00Z","payload":{"action":"world.restart","target":"world","seconds":300}}
```

Use a new 32-character lowercase hex request ID and a timestamp within 60 seconds.
Actions: `cluster.drain`, `cluster.disable`, `cluster.enable`, `service.start`,
`service.stop`, `world.restart`, `world.shutdown`, `world.cancel-restart`,
`world.cancel-shutdown`. Only restart/shutdown accept `seconds` (integer 0..86400).
Targets are existing node or enabled managed-service keys, never executable paths.
World lifecycle actions use the existing graceful save/shutdown path.

Responses/events/errors/capabilities share `version,type,requestId,source,timestamp,epoch,
sequence,payload`. Command acknowledgements return HTTP 202. `queued` means admitted;
`dispatched` means handed to the supervisor/world command channel, **not finished**.
Inspect monitoring for the actual transition. `unknown` means inspect server state before
submitting another command. The gateway never retries a mutation. Reusing an admitted
request ID is rejected, including after a gateway or hub restart through the hub audit.
Results are session-scoped in the gateway and expire after ten minutes in the hub.

WebSocket `/events` is monitoring-only. Within five seconds send:
`{"version":1,"type":"subscribe","csrf":"...","topics":["hub","nodes"]}`.
Omit topics to subscribe to all. Reconnect with `cursor:{epoch,sequence}` to replay a
short retained history. Filtered subscriptions get fresh snapshots. Events are ordered;
a gap, expired cursor, new epoch or `resync_required` requires replacing state from the
following snapshot. `command.result` carries the same command request ID. Heartbeats
are keepalives; they do not advance the sequence.

Limits: 4 KiB requests, 128 KiB events, 16 sessions, 2 sockets/session, 32 retained events,
8 pending events/socket, 2-second write deadline, 256 command results/session and
64 queued hub commands. Slow consumers receive explicit dropped-event counts and a
replacement snapshot; repeatedly slow sockets close with 1013. Polling is independent
of the page, and never changes form inputs or navigates the browser. Hub unavailability
marks values stale. Unsupported commands, extra parameters, shell, SQL, memory and
file operations are rejected. Backups report `available:false` until Phase 8.

## Audit and trust boundary

The hub records admission/rejection/dispatch in `hub_control_audit`; database triggers
reject UPDATE and DELETE. The gateway uses fsync-backed append-only HMAC-chained
JSONL records, including denied and malformed mutation requests, with correlation IDs.
Failure to persist admission prevents dispatch. Neither API exposes audit edits or
removal. Privileged database/OS administrators remain trusted: they can drop triggers,
replace keys or truncate whole valid log suffixes. External immutable archival is
required for protection against administrators. Preserve token and audit files together;
changing the token requires archiving the old log first. Run one gateway per audit file.
Do not place credentials or raw payloads in audit records.

## Validation

Build `hub_control_protocol_tests` and run it with CTest. Run
`node tools/dev/tests/hub_control_ui.test.js` for independent dashboard rendering and
reconnect checks. With the gateway dependencies installed, run
`python tools/dev/tests/hub_control.test.py --hub <built-hub> --hub-config <test-config>
--php <php-executable> --mysql <mysql-executable> --openssl <openssl-executable>`.
The integration test requires the audit migration, creates ephemeral TLS identities and
ports, starts only an isolated hub without managed children, and removes its temporary
hub identities and routing policy afterward. Immutable test audit records remain.
