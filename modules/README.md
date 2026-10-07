# SkyFire Modules

This directory holds optional, self-contained modules that are compiled into the
worldserver without editing the core.

## How it works

* At CMake configure time, every immediate subdirectory of `modules/` that
  contains a `src/` folder with source files is discovered automatically and
  compiled into a static `modules` library that is linked into `worldserver`.
* CMake generates `build/modules/ModulesLoader.cpp` (from
  `ModulesLoader.cpp.in`). It defines `AddModulesScripts()`, which the core calls
  at the end of `AddScripts()` in `src/server/game/Scripting/ScriptLoader.cpp`.
* You can disable the whole system with the CMake option `-DMODULES=0`.

## Module layout

```
modules/
  mod-yourmodule/
    src/                     # .cpp/.h compiled into the modules library
      mod_yourmodule.cpp
    conf/                    # optional; *.conf.dist staged next to worldserver
      yourmodule.conf.dist
    sql/                     # optional; applied manually / by your own tooling
    README.md
```

## Required loader function

Each module must implement exactly one loader function named:

```
void Add<ModuleFolderName>Scripts();
```

where `<ModuleFolderName>` is the folder name with every non-alphanumeric
character replaced by `_`. For example `mod-example` must implement
`Addmod_exampleScripts()`. Inside it, call your own `AddSC_*` registration
functions, e.g.:

```cpp
void AddSC_my_feature();          // defined in your module

void Addmod_exampleScripts()      // discovered + invoked automatically
{
    AddSC_my_feature();
}
```

Scripts register with the core exactly like built-in scripts (subclass
`PlayerScript`, `CommandScript`, `CreatureScript`, ... from `ScriptMgr.h`).

Movement-validation modules can use the [movement hooks](../doc/ModuleMovementHooks.md)
to inspect or reject client movement and track server-authorized movement changes.
Detection rules, reports and database tables remain owned by the external module.

## Config files

Any `conf/*.conf.dist` file is copied next to the worldserver binary on build and
installed alongside `worldserver.conf`. Load values with the standard
`sConfigMgr->GetXOption(...)` API.

## External Eluna module

Eluna is maintained in the separate [Eluna project](https://github.com/ProjectSkyFire-Modules/Eluna).
The core does not bundle, fetch, or build it by default. To install it yourself:

    git clone https://github.com/ProjectSkyFire-Modules/Eluna.git modules/mod-eluna

Then configure with -DMODULES=ON -DMOD_ELUNA=ON and follow that project's
configuration instructions. Module versions and updates are managed separately.

## Optional module build customization

A module may provide module.cmake to declare its build options and set
MODULE_ENABLED to FALSE to skip discovery. The default is TRUE for modules
without that file. This selection file is evaluated before collecting sources,
headers, configuration files, or loader registration.

An enabled module may also provide CMakeLists.txt. It is processed after the
aggregate modules target exists, allowing the module to link dependencies,
stage assets, and add install rules. Keep all module-specific build logic inside
the module folder so removing it requires no core changes.
