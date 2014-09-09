# Copyright (C) 2011-2014 Project SkyFire <http://www.projectskyfire.org/>
# Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

if(CMAKE_COMPILER_IS_GNUCXX AND NOT MINGW)
  add_definitions(-fno-delete-null-pointer-checks)
  
  execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
  if (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
    message(STATUS "GCC: C++11 for server activated.")
    add_definitions("-std=c++11")
  else()
    message(FATAL_ERROR "GCC: ${PROJECT_NAME} requires GCC 4.7 or greater.")
  endif()
endif()

if( SERVERS )
  set(sources_windows_Debugging
    ${CMAKE_SOURCE_DIR}/src/server/shared/Debugging/WheatyExceptionReport.cpp
    ${CMAKE_SOURCE_DIR}/src/server/shared/Debugging/WheatyExceptionReport.h
  )
  add_subdirectory(shared)
  add_subdirectory(game)
  add_subdirectory(collision)
  add_subdirectory(authserver)
  add_subdirectory(scripts)
  add_subdirectory(worldserver)
else()
  if( TOOLS )
    add_subdirectory(collision)
    add_subdirectory(shared)
  endif()
endif()
