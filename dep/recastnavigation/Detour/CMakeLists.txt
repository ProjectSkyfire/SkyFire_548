#
# This file is part of Project SkyFire https://www.projectskyfire.org. 
# See COPYRIGHT file for Copyright information
#

set(Detour_STAT_SRCS
    DetourAlloc.cpp 
    DetourCommon.cpp 
    DetourNavMesh.cpp 
    DetourNavMeshBuilder.cpp 
    DetourNavMeshQuery.cpp 
    DetourNode.cpp 
)

if(WIN32)
  include_directories(
    ${CMAKE_SOURCE_DIR}/dep/zlib
  )
endif()

add_library(Detour STATIC ${Detour_STAT_SRCS})

target_link_libraries(Detour ${ZLIB_LIBRARIES})
