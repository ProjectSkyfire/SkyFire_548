# Clustering setup

See the [root README](../README.md#clustering-package-requirements) for the package
requirements for each component.

CMake **INSTALL** copies `web/`, `control/` and `backup/` next to the server binaries
(on Linux, under the install prefix's `bin/`). It does not install Python, PHP,
MySQL tools or Python packages. Existing local configuration files and virtual
environments are preserved. Re-run the appropriate requirements install after
upgrading these services.

From the installed server directory, install both Python services' dependencies
as follows. If using only one service, omit the other requirements file.

Windows PowerShell:

```powershell
py -3.11 -m venv .venv
.\.venv\Scripts\python.exe -m pip install -r control/requirements.txt -r backup/requirements.txt
```

Linux (install your distribution's Python `pip` and `venv` packages first):

```sh
python3 -m venv .venv
./.venv/bin/python -m pip install -r control/requirements.txt -r backup/requirements.txt
```

Use that virtual environment's Python to launch each service. Copy its
`*.toml.dist` file to `*.toml` and configure the executable, certificate and database
configuration paths before starting it. Python 3.11 or newer is required in both
examples; use the launcher for your installed version.

Initialize a new hub database from `sql/base/hub_database.sql`. For an existing
hub database, apply its outstanding `sql/updates/hub/` migrations in filename order
before starting the updated hub; scheduled shutdown cycles and timezone selection
require `2026_09_19_hub_02.sql`. Enable `Hub.Cluster.Enable` on the hub and
`Cluster.Enable` on each node, with the certificate, key, CA and node settings in
the corresponding `.conf.dist` files as the configuration reference.

For Central time scheduling, use `server_timezone = "America/Chicago"` in
`backup/backup.toml`, or `"system"` when the worker host is already configured for
Central time. Select **Server time** for each daily/weekly schedule in the web
interface; existing schedules remain UTC until edited. The timezone packages
handle CST/CDT transitions.

See the [control gateway setup](../src/server/hub/control/README.md) and
[backup, scheduling and recovery setup](../src/server/hub/backup/README.md) for service
startup, permissions and configuration details. Scheduled shutdown/restart cycles
require the hub to supervise the participating game services; external nodes must
be stopped separately before the backup can proceed.

## Hub database setup and updates

Hubserver uses the shared database setup/update engine before opening its database
pools, network listeners or managed services. CMake INSTALL includes the hub base,
released updates and pending directory under `sql/` beside the binaries. Configure
`HubDatabase.SqlPath` to that directory's absolute path (or the repository's full
`sql/` directory). Keep that SQL package from the same revision as the binary.

For a new installation:

```ini
HubDatabase.SqlPath = "D:/Test Server/SkyFire_Authnet/sql"
HubDatabase.AutoSetup = 1
HubDatabase.AutoCreate = 1
HubDatabase.AutoBaseline = 0
HubDatabase.AllowUpdateHashMismatch = 0
HubDatabase.ImportPendingUpdates = 0
```

On Linux, use the installed absolute path, for example `/opt/skyfire/bin/sql`.
`AutoSetup` imports `base/hub_database.sql` only when the configured schema is empty.
`AutoCreate` additionally permits creating a missing database; grant CREATE DATABASE
only if using that option. Both default off. An empty `SqlPath` disables setup/update
checks, preserving the existing manual installation behavior.

The current hub base includes the nine released migrations through
`2026_09_19_hub_02.sql`. A fresh install records those files in `skyfire_db_updates`
without replaying their DDL, then executes newer releases. That explicit included
list is maintained in `BuildHubDatabaseSetupPlan`; update it together with any
future base rollup, never mark arbitrary future updates as included. Existing
tracked schemas receive each unapplied file in filename order, even with
`AutoSetup = 0`. Applied files have stored content hashes; mismatches stop startup
unless `AllowUpdateHashMismatch = 1` explicitly permits a warning and skip.
Successful applied updates are also recorded in `db_update`.

For an existing manually maintained hub (including databases imported from the base
outside this updater), first back up the database and manually apply every outstanding
released migration. With pending imports disabled, enable `HubDatabase.AutoBaseline = 1`
for one startup to record the discovered releases, then turn it off. This option
**does not execute or verify those migrations**. Do not use it to repair a failed or
partially applied update. An existing schema without tracking stops startup unless
this explicit adoption option is enabled.

Before updating, stop the backup worker and any other hub/database writers, complete
active recovery work, and keep game services stopped until startup succeeds. The
updater refuses schema changes while the worker lease is live, backup jobs are active,
or recovery maintenance is unresolved. Allow a stopped worker's lease to expire before
updating. It serializes concurrent hub setup connections with a MySQL advisory lock;
that lock does not fence other applications. SQL or tracking failures stop startup.
MySQL DDL can commit before a later statement fails, so inspect/repair a partial
migration before retrying; the updater cannot roll it back automatically.

For staging only, set `HubDatabase.ImportPendingUpdates = 1` to include
`pending_updates/hub`. See the [pending SQL guide](../sql/pending_updates/README.md)
for dependency ordering and the limitations of promoting already imported pending
files. CI and the promotion script have not been modified to accept hub updates yet.
