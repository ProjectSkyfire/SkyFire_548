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

## Config files

Any `conf/*.conf.dist` file is copied next to the worldserver binary on build and
installed alongside `worldserver.conf`. Load values with the standard
`sConfigMgr->GetXOption(...)` API.
