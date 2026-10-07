# Movement validation modules

An optional anticheat belongs in its own repository, installed beneath
`modules/` using the existing module loader. Detectors, reports, commands,
configuration and SQL tables belong to that module. No anticheat tables or
configuration switches are required by the core.

## PlayerScript callbacks

`OnValidateMovement(player, mover, movement, opcode)` runs after the core's
GUID, position and transport-offset checks, before changing transport membership,
applying fall damage, updating movement state or broadcasting the movement.
Return `false` to discard the packet. The first rejection ends dispatch; another
module cannot override it. With no rejecting module, normal processing continues.

The player owns the session; the mover can be a controlled unit. The movement
argument is immutable, untrusted client data, including its original timestamp.
Transport validation checks coordinate bounds, not permission to board a claimed
transport. Never treat client flags, transport GUIDs or jump speeds as proof of
server authorization. Use server time and authoritative unit state.

`OnMovementApplied(player, mover, movement, opcode)` runs after an accepted
movement packet changes state. It receives the original client data; query the
mover for the resulting server position and transport membership. Vehicle packets
update orientation. Knockback acknowledgements and inactive-mover packets update
movement information rather than relocating the unit. Distinguish those opcodes
when maintaining position samples. Rejected packets never produce this callback.

Both callbacks cover normal movement, knockback acknowledgements and inactive
mover updates. They are not a generic callback for every network opcode. Other
acknowledgements that do not apply position information retain their existing
core handling.

`OnMovementChanged(player, mover, change)` reports successful teleport requests,
speed-rate changes and launched player splines. A delayed teleport can notify
once when queued and again when executed; reset baselines idempotently. A teleport
request is not a completed transfer: continue checking `IsBeingTeleported()` and
use the existing login/map-change hooks to establish fresh samples. For speed
changes query `GetSpeed()`; for splines inspect the active server spline.

`OnKnockback(player, mover, speedXY, speedZ)` supplies the server-issued impulse,
including player-controlled units. Vertical speed uses the outgoing knockback
packet convention (normally negative for an upward impulse). This also covers
player jumps implemented through `SendMoveKnockBack`.

Speed and spline notifications currently cover player units. Modules monitoring
controlled creatures must use the mover's authoritative speed and spline state;
they must not assume every server change produces a player notification.

## Callback responsibilities

These callbacks execute synchronously on the caller's game thread. Do not block
on SQL/network work, retain pointers or references beyond the callback, or assume
all players are updated on one thread. Synchronize shared module state as needed.
Do not change movement, teleport, kick or delete units inside movement callbacks.
Queue corrective actions for an existing `PlayerScript::OnUpdate` callback so the
movement handler can finish safely. Rejecting a packet alone does not correct the
client's local position; the module must arrange any required resynchronization.

Use the existing login/logout, save, update, map-change, world configuration and
command scripts for the remainder of the module. Handle movement exemptions such
as taxis, vehicles, flight auras, water walking and GM state using authoritative
core APIs. Keep detection thresholds and punishment policy out of the core.

## Validation before enabling enforcement

Build with and without the module. First run in report-only mode and exercise
walking, swimming, flying, transport boarding, vehicle control, knockbacks,
speed changes, near/far teleports and reconnects. Verify that rejected movement
does not change position, transport membership or fall damage, and that a second
module cannot override rejection. Test latency and delayed acknowledgements
before enabling automatic sanctions.
