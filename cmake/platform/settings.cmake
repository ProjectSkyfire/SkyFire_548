#
# This file is part of Project SkyFire https://www.projectskyfire.org. 
# See COPYRIGHT file for Copyright information
#

# set installation prefix
if( PREFIX )
  set(CMAKE_INSTALL_PREFIX "${PREFIX}")
else()
  if ( UNIX )
    set(CMAKE_INSTALL_PREFIX "/usr/local/skyfire-server")
  else()
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/bin")
  endif()
endif()

