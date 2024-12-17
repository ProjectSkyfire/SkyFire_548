#
# This file is part of Project SkyFire https://www.projectskyfire.org. 
# See COPYRIGHT file for Copyright information
#
set(SFMT_STAT_SRCS
  ex-ran.cpp
  mersenne.cpp
  mother.cpp
  rancombi.cpp
  sfmt.cpp
  testirandomx.cpp
  userintf.cpp
)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch")
  message(STATUS "Adding sse2neon.")
  add_subdirectory(${CMAKE_SOURCE_DIR}/dep/sse2neon)
endif()

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch")
    ${CMAKE_SOURCE_DIR}/dep/sse2neon
  endif()
)

add_library(SFMT STATIC ${SFMT_STAT_SRCS})

