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

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch")
    ${CMAKE_SOURCE_DIR}/dep/sse2neon
  endif()
)

add_library(SFMT STATIC ${SFMT_STAT_SRCS})

target_link_libraries(SFMT ${ZLIB_LIBRARIES})
