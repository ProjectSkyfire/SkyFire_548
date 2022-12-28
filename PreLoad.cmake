#
# This file is part of Project SkyFire https://www.projectskyfire.org. 
# See COPYRIGHT file for Copyright information
#

# This file is run right before CMake starts configuring the sourcetree

# Example: Force CMAKE_INSTALL_PREFIX to be preloaded with something before
# doing the actual first "configure"-part - allows for hardforcing
# destinations elsewhere in the CMake buildsystem (commented out on purpose)

# Override CMAKE_INSTALL_PREFIX on Windows platforms
#if( WIN32 )
#  if( NOT CYGWIN )
#    set(CMAKE_INSTALL_PREFIX
#      "" CACHE PATH "Default install path")
#  endif()
#endif()
