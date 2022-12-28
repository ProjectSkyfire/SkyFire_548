#
# This file is part of Project SkyFire https://www.projectskyfire.org. 
# See COPYRIGHT file for Copyright information
#

file(GLOB sources *.cpp *.h)

set(gsoap_STAT_SRCS
   ${sources}
)

include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
)

# Little fix for MSVC / Windows platforms
add_definitions(-D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=0)

add_library(gsoap STATIC ${gsoap_STAT_SRCS})

set_target_properties(gsoap PROPERTIES LINKER_LANGUAGE CXX)
