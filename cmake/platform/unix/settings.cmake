#
# This file is part of Project SkyFire https://www.projectskyfire.org. 
# See COPYRIGHT file for Copyright information
#

# set default configuration directory
if( NOT CONF_DIR )
  set(CONF_DIR ${CMAKE_INSTALL_PREFIX}/etc)
  message(STATUS "UNIX: Using default configuration directory")
endif()

# set default library directory
if( NOT LIBSDIR )
  set(LIBSDIR ${CMAKE_INSTALL_PREFIX}/lib)
  message(STATUS "UNIX: Using default library directory")
endif()

# configure uninstaller
configure_file(
  "${CMAKE_SOURCE_DIR}/cmake/platform/cmake_uninstall.in.cmake"
  "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
  @ONLY
)
message(STATUS "UNIX: Configuring uninstall target")

# create uninstaller target (allows for using "make uninstall")
add_custom_target(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
)
message(STATUS "UNIX: Created uninstall target")

message(STATUS "UNIX: Detected compiler: ${CMAKE_C_COMPILER}")
if(CMAKE_C_COMPILER MATCHES "gcc" OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
  execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
  if (NOT (GCC_VERSION VERSION_GREATER 14.1 OR GCC_VERSION VERSION_EQUAL 14.1))
    message(FATAL_ERROR "GCC: Compiler doesnt support c++26, requires g++ 14.1 or greater.")
  elseif(GCC_VERSION VERSION_GREATER 14.1 OR GCC_VERSION VERSION_EQUAL 14.1)
    include(${CMAKE_SOURCE_DIR}/cmake/compiler/gcc/settings.cmake)
  endif()
elseif(CMAKE_C_COMPILER MATCHES "icc")
  include(${CMAKE_SOURCE_DIR}/cmake/compiler/icc/settings.cmake)
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
  include(${CMAKE_SOURCE_DIR}/cmake/compiler/clang/settings.cmake)
  message(STATUS "CLANG: Using Clang on Unix.")
else()
add_definitions(-D_BUILD_DIRECTIVE='"${CMAKE_BUILD_TYPE}"')
endif()
