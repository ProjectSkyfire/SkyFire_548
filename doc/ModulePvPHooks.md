# Optional PvP module hooks

`PvPScript` in `ScriptMgr.h` is a code-only script type. Its default methods do
not change PvP state. The core does not load module-specific settings or SQL.

## Queue ownership

`SelectBGGroups(queue, bg, bracket, minimum, maximum, fill)` returns **handled**,
not **match ready**. The first script returning true owns that selection pass:

- `fill=false`: populate the queue's two selection pools for formation. The
  core creates a match only when both pools satisfy `minimum`. Empty pools mean
  the queue must wait; returning true still suppresses native premade selection.
- `fill=true`: populate pools for an existing match, including outstanding
  invitations in the capacity budget. The core sends invitations normally.
- `false`: use the original queue algorithm. With no registered script, this
  is always the result. Arenas keep the native formation path.

A provider changing `GroupQueueInfo::Team` must move the group into the matching
native queue bucket before returning. `NativeTeam` records the original team
for restoring uncommitted groups. Never change an already-issued invitation's
team; removal and expiration depend on its assigned team and instance ID.

`CanJoinBG` defaults to true. `HandleBGQueueAnnounce` defaults to false and may
replace an announcement only after the core's announcer settings allow it.

## Player and battleground lifecycle

`BeforeBGAddPlayer` runs before membership/counts are recorded;
`AfterBGAddPlayer` runs after raid assignment. `AfterBGRemovePlayer` runs after
team-specific cleanup and before the BG identity is cleared or the player is
transported. `BeforeBGStart` runs immediately before the doors open.

`Battleground::ChangePlayerTeam` is restricted to non-arena matches waiting to
start. It changes membership, counts, invitation reservations and raid. The
provider must update temporary faction/appearance and relocate the player to
the destination team's starting position.

`Player::SetTemporaryTeam`, `ClearTemporaryTeam`, `GetNativeTeam` and
`GetNativeTeamId` separate effective PvP allegiance from native identity. The
temporary value is never serialized and never changes the real race. Normal
channel operations use the native team. `OnFactionUpdate` and `OnResurrect`
allow a provider to maintain temporary faction/model after native resets.

`BeforeTeamChat` receives an already-validated language by reference.
`BeforeReputation` may redirect the faction and adjust the signed amount before
the original reputation hooks and spillover calculations run.

## Battlefield lifecycle

`BeforeBFEnter` runs before zone membership/vacancy checks, including the bulk
queue/zone invitation passes. Invitation passes use snapshots because a
provider may transfer membership. `BeforeBFJoin` chooses a team before raid/war
registration. `AfterBFJoin` reports success or failure; unsuccessful transfers
are rolled back before the failure callback.

`TransferPlayerTeam` validates the destination for existing war reservations,
moves zone/queue/war membership and invitation/kick deadlines, and updates an
existing battlefield raid. `HasWarVacancy(team, candidate)` excludes the
candidate's existing reservation. Population getters expose const containers.

`AfterBFLeave` runs after team-indexed cleanup. `BeforeBFEnd` runs with war
membership still available for module quest credit; `AfterBFEnd` runs after
native rewards and war cleanup. Restore temporary identity only after that
cleanup. `OnBFKill` observes Wintergrasp kills before native handling.

`CommandScript::ModifyCommands` can extend an existing command tree after all
scripts have contributed their commands. Its default leaves the tree unchanged.
