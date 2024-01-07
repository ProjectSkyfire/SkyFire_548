#
# This file is part of Project SkyFire https://www.projectskyfire.org. 
# See COPYRIGHT file for Copyright information
#

# Set build-directive (used in core to tell which buildtype we used)
add_definitions(-D_BUILD_DIRECTIVE='"${CMAKE_BUILD_TYPE}"')

# Check C++23 compiler support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++23" COMPILER_SUPPORTS_CXX23)
if(COMPILER_SUPPORTS_CXX23)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23") #c++23
else()
  message(FATAL_ERROR "Error, SkyFire requires a compiler that supports C++23!")
endif()
if(WITH_CXX_20_STD)
  if(NOT WITH_CXX_DRAFT_STD AND NOT WITH_CXX_23_STD)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20") #c++20
    message(STATUS "GCC: C++20 Standard Enabled.")
  else()
    message(FATAL_ERROR "GCC: Only 1 CXX Standard can be used!")
  endif()
endif()
if(WITH_CXX_23_STD)
  if(NOT WITH_CXX_DRAFT_STD AND NOT WITH_CXX_20_STD)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23") #c++23
    message(STATUS "GCC: C++23 Standard Enabled.")
  else()
    message(FATAL_ERROR "GCC: Only 1 CXX Standard can be used!")
  endif()
endif()
if(WITH_CXX_DRAFT_STD)
  if(NOT WITH_CXX_23_STD AND NOT WITH CXX_20_STD)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2c") #c++26
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++2c") #c++26
    message(STATUS "GCC: C++ Draft Standard Enabled.")
  endif()
endif()

if(PLATFORM EQUAL 32)
  # Required on 32-bit systems to enable SSE2 (standard on x64)
  set(SSE_FLAGS "-msse2 -mfpmath=sse")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SSE_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SSE_FLAGS}")
endif()
add_definitions(-DHAVE_SSE2 -D__SSE2__)
message(STATUS "GCC: SFMT enabled, SSE2 flags forced")

if( WITH_WARNINGS )
  set(WARNING_FLAGS "-W -Wall -Wextra -Winit-self -Winvalid-pch -Wfatal-errors")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WARNING_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS} -Woverloaded-virtual")
  message(STATUS "GCC: All warnings enabled")
endif()

if( WITH_COREDEBUG )
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")
  message(STATUS "GCC: Debug-flags set (-g3)")
endif()
