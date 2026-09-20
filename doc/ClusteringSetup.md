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
