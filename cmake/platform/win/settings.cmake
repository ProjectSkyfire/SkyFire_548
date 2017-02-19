# Copyright (C) 2011-2017 Project SkyFire <http://www.projectskyfire.org/
# Copyright (C) 2008-2017 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

# Platform-specfic options
option(USE_MYSQL_SOURCES "Use included MySQL-sources to build libraries" 1)

# Package overloads
set(ACE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/dep/acelite)
set(ACE_LIBRARY "ace")
set(BZIP2_LIBRARIES "bzip2")
set(ZLIB_LIBRARIES "zlib")

if((MSVC_VERSION EQUAL 1900) OR (MSVC_VERSION GREATER 1900))
  set( USE_MYSQL_SOURCES 0 )
  message(STATUS "MySQL: Disabled supplied MySQL sources")
endif((MSVC_VERSION EQUAL 1900) OR (MSVC_VERSION GREATER 1900))

if( USE_MYSQL_SOURCES )
  set(MYSQL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/dep/mysqllite/include)
  set(MYSQL_LIBRARY "libmysql")
  set( MYSQL_FOUND 1 )
  message(STATUS "Using supplied MySQL sources")
endif()

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
  include(${CMAKE_SOURCE_DIR}/cmake/compiler/msvc/settings.cmake)
elseif ( MINGW )
  include(${CMAKE_SOURCE_DIR}/cmake/compiler/mingw/settings.cmake)
endif()
