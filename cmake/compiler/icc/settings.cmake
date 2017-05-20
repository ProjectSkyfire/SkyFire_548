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

# Set build-directive (used in core to tell which buildtype we used)
add_definitions(-D_BUILD_DIRECTIVE="'${CMAKE_BUILD_TYPE}'")

if(PLATFORM EQUAL 32)
  add_definitions(-axSSE2)
else()
  add_definitions(-xSSE2)
endif()

if( WITH_WARNINGS )
  add_definitions(-w1)
  message(STATUS "ICC: All warnings enabled")
endif()

if( WITH_COREDEBUG )
  add_definitions(-g)
  message(STATUS "ICC: Debug-flag set (-g)")
endif()
