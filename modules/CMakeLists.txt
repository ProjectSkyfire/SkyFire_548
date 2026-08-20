#
# This file is part of Project SkyFire https://www.projectskyfire.org.
# See COPYRIGHT file for Copyright information
#
# SkyFire dynamic module system.
#
# Every immediate subdirectory of modules/ that contains a src/ folder with
# source files is treated as a module and compiled into the static "modules"
# library. Each module must implement a single loader function named:
#
#     void Add<ModuleFolderName>Scripts();
#
# where <ModuleFolderName> is the module directory name with every character
# that is not a letter or digit replaced by an underscore (e.g. the folder
# "mod-example" must implement "Addmod_exampleScripts").
#
# CMake generates modules/ModulesLoader.cpp from ModulesLoader.cpp.in which
# forward-declares and invokes every module loader from AddModulesScripts().
#

message(STATUS "")
message(STATUS "* Configuring modules")

set(MODULE_SOURCES)
set(MODULE_INCLUDE_DIRS)
set(MODULES_FORWARD_DECL "")
set(MODULES_INVOKE "")
set(MODULE_NAME_LIST "")
set(MODULE_CONF_ALL "")

file(GLOB MODULE_CANDIDATES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)

foreach(MODULE_DIR_NAME ${MODULE_CANDIDATES})
  set(MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${MODULE_DIR_NAME})

  if(IS_DIRECTORY ${MODULE_PATH} AND EXISTS ${MODULE_PATH}/src)
    file(GLOB_RECURSE MODULE_SRC_FILES
      ${MODULE_PATH}/src/*.cpp
      ${MODULE_PATH}/src/*.c
      ${MODULE_PATH}/src/*.cc
      ${MODULE_PATH}/src/*.h
      ${MODULE_PATH}/src/*.hpp)

    if(MODULE_SRC_FILES)
      list(APPEND MODULE_SOURCES ${MODULE_SRC_FILES})
      list(APPEND MODULE_NAME_LIST ${MODULE_DIR_NAME})

      # Add every directory containing a header as an include directory so that
      # modules with nested folder layouts resolve headers.
      file(GLOB_RECURSE MODULE_HEADER_FILES
        ${MODULE_PATH}/src/*.h
        ${MODULE_PATH}/src/*.hpp)
      list(APPEND MODULE_INCLUDE_DIRS ${MODULE_PATH}/src)
      foreach(HEADER_FILE ${MODULE_HEADER_FILES})
        get_filename_component(HEADER_DIR ${HEADER_FILE} DIRECTORY)
        list(APPEND MODULE_INCLUDE_DIRS ${HEADER_DIR})
      endforeach()

      # Derive the loader function name (replace non-alphanumeric with '_').
      string(REGEX REPLACE "[^a-zA-Z0-9]" "_" MODULE_SANITIZED ${MODULE_DIR_NAME})
      set(MODULES_FORWARD_DECL "${MODULES_FORWARD_DECL}void Add${MODULE_SANITIZED}Scripts();\n")
      set(MODULES_INVOKE "${MODULES_INVOKE}    Add${MODULE_SANITIZED}Scripts();\n")

      message(STATUS "  + module: ${MODULE_DIR_NAME}")

      # Collect module configuration files (staged/installed after the target exists).
      file(GLOB MODULE_CONF_FILES ${MODULE_PATH}/conf/*.conf.dist)
      list(APPEND MODULE_CONF_ALL ${MODULE_CONF_FILES})
      if(WIN32)
        install(FILES ${MODULE_CONF_FILES} DESTINATION ".")
      else()
        install(FILES ${MODULE_CONF_FILES} DESTINATION ${CONF_DIR})
      endif()
    endif()
  endif()
endforeach()

if(MODULE_INCLUDE_DIRS)
  list(REMOVE_DUPLICATES MODULE_INCLUDE_DIRS)
endif()

# Generate the aggregate module loader.
set(GENERATED_MODULES_LOADER ${CMAKE_CURRENT_BINARY_DIR}/ModulesLoader.cpp)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ModulesLoader.cpp.in ${GENERATED_MODULES_LOADER})

add_library(modules STATIC
  ${MODULE_SOURCES}
  ${GENERATED_MODULES_LOADER}
)

# Reuse the game target's include directories (game, shared, dependencies, etc.)
# so module code can include core headers exactly like core scripts do.
target_include_directories(modules
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${MODULE_INCLUDE_DIRS}
  PRIVATE
    ${CMAKE_BINARY_DIR}
    $<TARGET_PROPERTY:game,INCLUDE_DIRECTORIES>
)

target_link_libraries(modules game)

add_dependencies(modules game revision.h)

# Stage module *.conf.dist files next to the worldserver binary after build,
# mirroring how worldserver stages its own configuration file.
if(WIN32 AND MODULE_CONF_ALL)
  if(MSVC)
    set(MODULE_CONF_DEST ${CMAKE_BINARY_DIR}/bin/$(ConfigurationName))
  else()
    set(MODULE_CONF_DEST ${CMAKE_BINARY_DIR}/bin)
  endif()
  foreach(CONF_FILE ${MODULE_CONF_ALL})
    add_custom_command(TARGET modules POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E make_directory "${MODULE_CONF_DEST}"
      COMMAND ${CMAKE_COMMAND} -E copy "${CONF_FILE}" "${MODULE_CONF_DEST}/")
  endforeach()
endif()

if(MODULE_NAME_LIST)
  list(LENGTH MODULE_NAME_LIST MODULE_COUNT)
  message(STATUS "* Configured ${MODULE_COUNT} module(s)")
else()
  message(STATUS "* No modules found under modules/")
endif()
message(STATUS "")
