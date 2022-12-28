#
# This file is part of Project SkyFire https://www.projectskyfire.org. 
# See COPYRIGHT file for Copyright information
#

file(GLOB_RECURSE mmap_gen_sources *.cpp *.h)

set(mmap_gen_Includes
  ${CMAKE_BINARY_DIR}
  ${ACE_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/dep/libmpq
  ${CMAKE_SOURCE_DIR}/dep/zlib
  ${CMAKE_SOURCE_DIR}/dep/bzip2
  ${CMAKE_SOURCE_DIR}/dep/g3dlite/include
  ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Recast
  ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Detour
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/game/Conditions
  ${CMAKE_SOURCE_DIR}/src/server/collision
  ${CMAKE_SOURCE_DIR}/src/server/collision/Management
  ${CMAKE_SOURCE_DIR}/src/server/collision/Maps
  ${CMAKE_SOURCE_DIR}/src/server/collision/Models
)

if( WIN32 )
  set(mmap_gen_Includes
    ${mmap_gen_Includes}
    ${CMAKE_SOURCE_DIR}/dep/libmpq/win
    )
endif()

include_directories(${mmap_gen_Includes})

add_executable(mmaps_generator ${mmap_gen_sources})

target_link_libraries(mmaps_generator
  collision
  g3dlib
  Recast
  Detour
  ${ACE_LIBRARY}
  ${BZIP2_LIBRARIES}
  ${ZLIB_LIBRARIES}
)

if( UNIX )
  install(TARGETS mmaps_generator DESTINATION bin)
elseif( WIN32 )
  install(TARGETS mmaps_generator DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()
