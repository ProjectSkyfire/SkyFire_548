#
# This file is part of Project SkyFire https://www.projectskyfire.org. 
# See COPYRIGHT file for Copyright information
#

if( CMAKE_COMPILER_IS_GNUCXX )
  add_definitions(--no-warnings)
elseif( MSVC )
  add_definitions(/W0)
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
  add_subdirectory(acelite)
  if(TOOLS)
    add_subdirectory(bzip2)
  endif()
endif()

if (CMAKE_SYSTEM_PROCESSOR MATCHES "aarch")
  add_subdirectory(SFMT)
  message(STATUS "Adding sse2neon.")
  add_subdirectory(sse2neon)
endif()

add_subdirectory(zlib)
add_subdirectory(g3dlite)
add_subdirectory(recastnavigation)

if(SERVERS)
  add_subdirectory(gsoap)
endif()

if(TOOLS)
  add_subdirectory(StormLib)
endif()
