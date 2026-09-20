<p align="center">
  <img src="contrib/skyfire_transp.png" alt="Project SkyFire logo" width="520">
</p>

## Build Status
[![windows-build](https://github.com/ProjectSkyfire/SkyFire_548/actions/workflows/windows.yml/badge.svg)](https://github.com/ProjectSkyfire/SkyFire_548/actions/workflows/windows.yml)
[![ubuntu-build](https://github.com/ProjectSkyfire/SkyFire_548/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/ProjectSkyfire/SkyFire_548/actions/workflows/ubuntu.yml)
[![CodeFactor](https://www.codefactor.io/repository/github/projectskyfire/skyfire_548/badge)](https://www.codefactor.io/repository/github/projectskyfire/skyfire_548)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/57a11392c3ed42dcae439669e893565f)](https://app.codacy.com/gh/ProjectSkyfire/SkyFire_548/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)

## Project Information
SkyFire is a *MMORPG* Framework based mostly on C++.

SkyFireEMU is a MMORPG Framework based mostly on C++. It is completely 
open-source, and is community supported. It is derived
from CactusEmu, TrinityCore, and MaNGOS, the Massive Network Game Object Servers, 
and is based on the code of their projects with extensive changes over time to optimize, 
improve and cleanup the code base at the same time as improving the in game mechanics
and functionality.

It is completely open source; community involvement is highly encouraged.

If you wish to contribute ideas or code please visit our site linked below or
make pull requests to our 
[Github repository](https://github.com/ProjectSkyfire/SkyFire_548/pulls).

For further information on the SkyFire project, please visit our project website at 
[projectskyfire.org](http://www.projectskyfire.org).

## Supported Client Version
**MoP 5.4.8 Build 18414**

## Requirements
+ Platform/Architecture:
  + Ubuntu 24.04 LTS: x86_64
  + Windows 11: x86_64
  + Windows 10: x86_64
  + Windows Server 2025: x86_64
  + Windows Server 2022: x86_64
+ Processor with SSE2 support.
+ C++23-capable compiler:
  + GCC 14.0 or newer on Linux.
  + MSVC 19.44.35217.0 or newer with Visual Studio 2022 on Windows.
+ CMake:
  + 3.27.7 or newer on Linux.
  + 4.1.2 or newer on Windows.
+ Boost 1.91.0. Set `BOOST_ROOT` when Boost is installed outside the system prefix.
+ OpenSSL 4.0.1 with the legacy provider module available.
+ MySQL client development libraries.
  + MySQL 9.6 is the tested Windows install.
  + On Ubuntu, install the distro client development package such as `default-libmysqlclient-dev`.
+ Ninja is recommended for Linux builds.

## Clustering package requirements

Clustering uses the C++ build requirements above. Deploy matching `hubserver`,
`authserver` and `worldserver` binaries together. The Python services below are
separate processes; install their dependencies on the hosts that run them.

| Component | Additional packages and runtime requirements |
| --- | --- |
| Hub and cluster nodes | MySQL server for the hub database, plus the existing auth, characters and world databases. OpenSSL runtime libraries and a CA-signed certificate/private key for each cluster identity; cluster registration requires mutual TLS. |
| Local hub web interface | A browser and the installed `web/` directory. No Python, PHP or Node.js runtime is required for this interface. |
| Optional HTTPS/WebSocket control gateway | Python 3.11+, `pip`, a Python virtual environment, PHP 8.2+ CLI, and `aiohttp==3.14.3` from `control/requirements.txt`. Also requires an HTTPS certificate/private key and the shared hub control token. |
| Backup and recovery worker | Python 3.11+, `pip`, a Python virtual environment, MySQL 8+ `mysql` and `mysqldump` executables, and `tzlocal>=5.2,<6` plus `tzdata>=2025.2` from `backup/requirements.txt`. Configure absolute paths to the MySQL tools and a writable backup volume. |
| Optional character service (experimental) | Python 3.11+, `pip`, and `PyMySQL[rsa]==1.1.2` from `characterserver/requirements.txt`; an initialized InnoDB character schema and a dedicated cluster certificate. See [character service setup and limitations](doc/CharacterServer.md). |
| Isolated restore verification | A separate compatible MySQL server instance with binary logging disabled, configured through `verification_config`. The verification identity needs permission to create/drop temporary databases and users and grant database-local privileges; see the backup guide. |

See [clustering setup notes](doc/ClusteringSetup.md) for package installation,
configuration, database migrations and service startup references.
See [managed data services](doc/ManagedDataServices.md) for web lifecycle controls
and character-server health metrics.

## Install
Detailed installation guides are available in the wiki for

[Windows](https://wiki.projectskyfire.org/index.php?title=Installation_(Windows_5xx)),
[Fedora 39](https://wiki.projectskyfire.org/index.php?title=Installation_(Fedora_39)),
[Ubuntu 24.04](https://wiki.projectskyfire.org/index.php?title=Installation_(Ubuntu_24.04_LTS)) and
[Mac OSX](https://wiki.projectskyfire.org/index.php?title=Installation_Mac_OS_X).


## Reporting issues
Please use the search function before you report issues.
[SkyFire Issue Tracker](https://github.com/ProjectSkyfire/SkyFire_548/issues).

## Submitting fixes
Fixes are submitted as pull requests.

[SkyFire Pull Request](https://github.com/ProjectSkyfire/SkyFire_548/pulls)

## Copyright
License: GPL 3.0

Read file [LICENSE](LICENSE.md)

## Authors &amp; Contributors
Read file [THANKS](THANKS.md)

## Links
Forum: [https://www.projectskyfire.org/](https://www.projectskyfire.org/)

Database Releases: [https://github.com/ProjectSkyfire/SkyFire_548/releases](https://github.com/ProjectSkyfire/SkyFire_548/releases)

Wiki: [https://wiki.projectskyfire.org](https://wiki.projectskyfire.org)

The optional terrain/collision mapserver proof of concept requires Python 3.11+ and `src/server/mapserver/requirements.txt` (aiohttp). Its world startup helper needs Python 3.11+ without third-party packages. See [mapserver setup and multi-continent assignments](doc/MapServer.md).
