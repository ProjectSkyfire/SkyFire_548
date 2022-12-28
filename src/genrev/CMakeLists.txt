#
# This file is part of Project SkyFire https://www.projectskyfire.org. 
# See COPYRIGHT file for Copyright information
#

# Need to pass old ${CMAKE_BINARY_DIR} as param because its different at build stage
add_custom_target(revision.h ALL
  COMMAND ${CMAKE_COMMAND} -DNO_GIT=${WITHOUT_GIT} -DGIT_EXEC=${GIT_EXECUTABLE} -DBUILDDIR=${CMAKE_BINARY_DIR} -P ${CMAKE_SOURCE_DIR}/cmake/genrev.cmake
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
