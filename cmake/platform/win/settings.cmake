#
# This file is part of Project SkyFire https://www.projectskyfire.org. 
# See COPYRIGHT file for Copyright information
#

# Package overloads
set(ACE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/dep/acelite)
set(ACE_LIBRARY "ace")
set(BZIP2_LIBRARIES "bzip2")
set(ZLIB_LIBRARIES "zlib")

# We require at least Visual Studio 16.6 2019(aka 16.6) which has version nr 1926
IF(NOT FORCE_UNSUPPORTED_COMPILER AND MSVC_VERSION LESS 1926) 
  # MSVC 1925 contains internal compiler bug.
  IF(NOT FORCE_UNSUPPORTED_COMPILER AND MSVC_VERSION EQUAL 1925) #16.5
    MESSAGE(FATAL_ERROR "Visual Studio 16.5 is not supported, upgrade to 16.6 or newer!")
  ENDIF()
  # We require at least Visual Studio 16 2019(aka 16.0) which has version nr 1920.		
  IF(NOT FORCE_UNSUPPORTED_COMPILER AND MSVC_VERSION LESS 1920) #16.0	
    MESSAGE(FATAL_ERROR "Visual Studio 16 2019 or newer is required!")
  ENDIF()
ENDIF()

# check the CMake preload parameters (commented out by default)

# overload CMAKE_INSTALL_PREFIX if not being set properly
#if( WIN32 )
#  if( NOT CYGWIN )
#    if( NOT CMAKE_INSTALL_PREFIX )
#      set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/bin")
#    endif()
#  endif()
#endif()

if ( MSVC )
  message(STATUS "MSVC: Using MSVC on Windows.")
  include(${CMAKE_SOURCE_DIR}/cmake/compiler/msvc/settings.cmake)
elseif ( MINGW )
  include(${CMAKE_SOURCE_DIR}/cmake/compiler/mingw/settings.cmake)
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
  include(${CMAKE_SOURCE_DIR}/cmake/compiler/clang/settings.cmake)
  message(STATUS "CLANG: Using Clang on Windows.")
endif()
