#
# This file is part of Project SkyFire https://www.projectskyfire.org. 
# See COPYRIGHT file for Copyright information
#

include_directories(
  ${CMAKE_SOURCE_DIR}/dep/g3dlite/include
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Debugging
  ${CMAKE_SOURCE_DIR}/src/server/collision
  ${CMAKE_SOURCE_DIR}/src/server/collision/Maps
  ${CMAKE_SOURCE_DIR}/src/server/collision/Models
  ${ACE_INCLUDE_DIR}
  ${ZLIB_INCLUDE_DIR}
)

add_executable(vmap4assembler VMapAssembler.cpp)
add_dependencies(vmap4assembler storm)

target_link_libraries(vmap4assembler
  collision
  g3dlib
  ${ZLIB_LIBRARIES}
)

if( UNIX )
  install(TARGETS vmap4assembler DESTINATION bin)
elseif( WIN32 )
  install(TARGETS vmap4assembler DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()
