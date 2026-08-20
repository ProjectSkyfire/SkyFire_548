#
# This file is part of Project SkyFire https://www.projectskyfire.org.
# See COPYRIGHT file for Copyright information
#

if( UNIX )
  cmake_minimum_required(VERSION 3.27.7)
else()
  cmake_minimum_required(VERSION 4.1.2)
endif()

if (WIN32)
  #set clang as compiler if not msvc or mingw set
  if (NOT MSVC AND NOT MINGW)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_C_COMPILER "C:/Program Files/LLVM/bin/clang.exe")
    set(CMAKE_CXX_COMPILER "C:/Program Files/LLVM/bin/clang++.exe")
    set(CMAKE_RC_COMPILER "C:/Program Files/LLVM/bin/clang.exe")
  endif()
endif()

# Set projectname (must be done AFTER setting configurationtypes)
project(Project_Skyfire)

# CMake policies (can not be handled elsewhere)
if(POLICY CMP0005)
  cmake_policy(SET CMP0005 NEW)
endif()

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

# add this options before PROJECT keyword
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# Set RPATH-handing (CMake parameters)
set(CMAKE_SKIP_BUILD_RPATH 0)
set(CMAKE_BUILD_WITH_INSTALL_RPATH 0)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH 1)

# set macro-directory
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/macros")

# Set a default build type if none was specified
if( NOT CMAKE_BUILD_TYPE )
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE "Release")
endif()

include(CheckCXXSourceRuns)
include(CheckIncludeFiles)

# set default buildoptions and print them
include(cmake/options.cmake)

if(SKYFIRE_BUILD_TESTS)
  enable_testing()
endif()

# turn off PCH totally if enabled (hidden setting, mainly for devs)
if( NOPCH )
  set(USE_COREPCH 0)
  set(USE_SCRIPTPCH 0)
endif()

include(CheckPlatform)

set(OPENSSL_EXPECTED_VERSION 4.0.0)

set(BOOST_EXPECTED_VERSION 1.91.0)

if(BOOST_ROOT)
  file(TO_CMAKE_PATH "${BOOST_ROOT}" BOOST_ROOT_HINT)
elseif(DEFINED ENV{BOOST_ROOT})
  file(TO_CMAKE_PATH "$ENV{BOOST_ROOT}" BOOST_ROOT_HINT)
endif()

if(BOOST_ROOT_HINT)
  list(APPEND CMAKE_PREFIX_PATH "${BOOST_ROOT_HINT}")

  if(NOT Boost_DIR)
    file(GLOB BOOST_CONFIG_DIRS LIST_DIRECTORIES true
      "${BOOST_ROOT_HINT}/lib*/cmake/Boost-${BOOST_EXPECTED_VERSION}"
    )

    if(BOOST_CONFIG_DIRS)
      list(SORT BOOST_CONFIG_DIRS ORDER DESCENDING)
      list(GET BOOST_CONFIG_DIRS 0 Boost_DIR)
    endif()
  endif()
endif()

find_package(PCHSupport)
find_package(Boost ${BOOST_EXPECTED_VERSION} REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)
find_package(MySQL REQUIRED)

include(InstallRuntimeDependencies)

if( UNIX )
  find_package(Readline)
  find_package(ZLIB)
  find_package(BZip2)
  find_package(Jemalloc)

  if (CMAKE_SYSTEM_PROCESSOR MATCHES "aarch")
    find_package(sse2neon)
  endif()
endif()

if(NOT WITHOUT_GIT)
  find_package(Git)
endif()

# Find revision ID and hash of the sourcetree
include(cmake/genrev.cmake)

# print out the results before continuing
include(cmake/showoptions.cmake)

# add dependencies
add_subdirectory(dep)

# add core sources
add_subdirectory(src)

# add modules (must come after src so the 'game' target is available)
if(MODULES AND SERVERS)
  add_subdirectory(modules)
endif()
