 #
# This file is part of Project SkyFire https://www.projectskyfire.org. 
# See COPYRIGHT file for Copyright information
#

if (CMAKE_SYSTEM_PROCESSOR MATCHES "aarch")

find_path(SSE2NEON_INCLUDE_DIR
  NAMES
    sse2neon.h
  HINTS
    ${CMAKE_SOURCE_DIR}/dep/
  PATH_SUFFIXES
    sse2neon
  )

# handle the QUIETLY and REQUIRED arguments and set SSE2NEON_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(sse2neon DEFAULT_MSG SSE2NEON_INCLUDE_DIR)

if(SSE2NEON_FOUND)
  set(SSE2NEON_INCLUDE_DIRS ${SSE2NEON_INCLUDE_DIR})
endif()

mark_as_advanced(SSE2NEON_INCLUDE_DIR)

endif()
