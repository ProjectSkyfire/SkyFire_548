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
  if(TOOLS)
    add_subdirectory(bzip2)
  endif()
endif()

add_subdirectory(SFMT)
add_subdirectory(zlib)
add_subdirectory(g3dlite)
add_subdirectory(recastnavigation)
add_subdirectory(argon2)

if(ELUNA)
  add_subdirectory(lualib)
endif()

if(SERVERS)
  add_subdirectory(gsoap)
endif()

if(TOOLS)
  add_subdirectory(StormLib)
endif()
