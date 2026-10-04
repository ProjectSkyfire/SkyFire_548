#!/usr/bin/env python3
#
# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
#
"""Exercise real module discovery and INSTALL rules without compiling servers.

Run with Python and CMake/Ninja on PATH. The harness deliberately enables no
compiler languages; it validates the generated target graph, not native code.
"""

import os
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[2]
CMAKE = os.environ.get("CMAKE_COMMAND", "cmake")


class ModuleConfigurationTests(unittest.TestCase):
    def configure(self, *, enabled=None, modules=True,
                  present=True, windows=False):
        temporary = tempfile.TemporaryDirectory(prefix="skyfire module test ")
        self.addCleanup(temporary.cleanup)
        directory = Path(temporary.name)
        source = directory / "source"
        source.mkdir()
        shutil.copytree(
            ROOT / "modules", source / "modules",
        )
        if present:
            fixture = source / "modules/mod-fixture"
            for folder in ("src", "conf", "assets"):
                (fixture / folder).mkdir(parents=True, exist_ok=True)
            (fixture / "src/Feature.cpp").write_text("void Addmod_fixtureScripts() {}")
            (fixture / "dependency.cpp").write_text("int fixture_dependency;")
            (fixture / "conf/fixture.conf.dist").write_text("""[worldserver]
Fixture.Enable = 0
""")
            (fixture / "assets/startup.txt").write_text("fixture")
            (fixture / "module.cmake").write_text("""option(MOD_FIXTURE "Build fixture" OFF)
set(MODULE_ENABLED ${MOD_FIXTURE})
""")
            (fixture / "CMakeLists.txt").write_text("""
add_library(mod_fixture_dependency STATIC dependency.cpp)
target_link_libraries(modules mod_fixture_dependency)
if(WIN32)
  add_custom_target(mod_fixture_assets)
  add_dependencies(modules mod_fixture_assets)
  install(DIRECTORY assets/ DESTINATION fixture_assets)
else()
  install(DIRECTORY assets/ DESTINATION ${CONF_DIR}/fixture_assets)
endif()
""")
        (source / "CMakeLists.txt").write_text("""
cmake_minimum_required(VERSION 3.27)
project(ModuleBoundaryTests NONE)
set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_COMMAND> -E true")
set(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_COMMAND> -E true")
# No compilers are enabled or invoked. Give static targets an archive language
# solely so CMake can generate their dependency and installation graph.
function(add_library name)
  _add_library(${name} ${ARGN})
  set_target_properties(${name} PROPERTIES LINKER_LANGUAGE CXX)
endfunction()
set(WIN32 ${TEST_WINDOWS})
set(MSVC ${TEST_WINDOWS})
set(CONF_DIR "${CMAKE_INSTALL_PREFIX}/etc")
file(WRITE "${CMAKE_BINARY_DIR}/dummy.cpp" "int module_test_placeholder;")
add_library(game STATIC "${CMAKE_BINARY_DIR}/dummy.cpp")
add_custom_target(revision.h)
if(TEST_MODULES)
  add_subdirectory(modules)
  file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/module-graph.txt"
    CONTENT "dependency=$<TARGET_EXISTS:mod_fixture_dependency>\\nmodule-links=$<TARGET_PROPERTY:modules,LINK_LIBRARIES>\\ngame-links=$<TARGET_PROPERTY:game,LINK_LIBRARIES>\\nmodule-sources=$<TARGET_PROPERTY:modules,SOURCES>\\n")
endif()
""", encoding="utf-8")
        build = directory / "build"
        install = directory / "install"
        command = [
            CMAKE, "-S", str(source), "-B", str(build), "-G", "Ninja",
            f"-DCMAKE_INSTALL_PREFIX={install.as_posix()}",
            f"-DTEST_MODULES={'ON' if modules else 'OFF'}",
            f"-DTEST_WINDOWS={'ON' if windows else 'OFF'}",
        ]
        if enabled is not None:
            command.append(f"-DMOD_FIXTURE={'ON' if enabled else 'OFF'}")
        self.run_cmake(command)
        self.run_cmake([CMAKE, "--install", str(build)])
        return build, install

    def run_cmake(self, command):
        result = subprocess.run(command, text=True, capture_output=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

    def assert_module(self, build, install, enabled, *, windows=False):
        loader = (build / "modules/ModulesLoader.cpp").read_text()
        graph = (build / "module-graph.txt").read_text()
        self.assertIn("Addmod_exampleScripts();", loader)
        self.assertEqual("Addmod_fixtureScripts();" in loader, enabled)
        self.assertIn(f"dependency={int(enabled)}\n", graph)
        self.assertIn("game-links=\n", graph)
        if enabled:
            self.assertIn("mod_fixture_dependency", graph)
            self.assertIn("mod-fixture/src/Feature.cpp", graph)
        else:
            self.assertNotIn("mod-fixture", graph)
        config = install if windows else install / "etc"
        self.assertEqual((config / "fixture.conf.dist").is_file(), enabled)
        self.assertEqual(
            (config / "fixture_assets/startup.txt").is_file(), enabled)
        self.assertEqual((config / "fixture_assets").is_dir(), enabled)
        self.assertFalse((config / "fixture.conf").exists())

    def test_default_excludes_optional_module(self):
        self.assert_module(*self.configure(), False)

    def test_enabled_linux_install(self):
        self.assert_module(*self.configure(enabled=True), True)

    def test_enabled_windows_install(self):
        build, install = self.configure(enabled=True, windows=True)
        self.assert_module(build, install, True, windows=True)
        self.assertIn("mod_fixture_assets", (build / "build.ninja").read_text())

    def test_module_folder_can_be_absent(self):
        self.assert_module(*self.configure(present=False), False)

    def test_modules_off_excludes_everything(self):
        build, install = self.configure(enabled=True, modules=False)
        self.assertFalse((build / "modules").exists())
        self.assertFalse((install / "etc/fixture.conf.dist").exists())


if __name__ == "__main__":
    unittest.main()
