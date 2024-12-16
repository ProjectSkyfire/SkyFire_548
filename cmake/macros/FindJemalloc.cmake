 #
# This file is part of Project SkyFire https://www.projectskyfire.org. 
# See COPYRIGHT file for Copyright information
#

find_path(JEMALLOC_ROOT_DIR NAMES include/jemalloc/jemalloc.h)

find_library(JEMALLOC_LIBRARIES NAMES jemalloc HINTS ${JEMALLOC_ROOT_DIR}/lib)

find_path(JEMALLOC_INCLUDE_DIR NAMES jemalloc/jemalloc.h HINTS ${JEMALLOC_ROOT_DIR}/include)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Jemalloc DEFAULT_MSG JEMALLOC_LIBRARIES JEMALLOC_INCLUDE_DIR)

mark_as_advanced(JEMALLOC_ROOT_DIR JEMALLOC_LIBRARIES JEMALLOC_INCLUDE_DIR)
