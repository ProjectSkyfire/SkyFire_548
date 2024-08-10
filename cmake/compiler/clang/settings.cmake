#
# This file is part of Project SkyFire https://www.projectskyfire.org. 
# See COPYRIGHT file for Copyright information
#


# Set build-directive (used in core to tell which buildtype we used)
add_definitions(-D_BUILD_DIRECTIVE='"${CMAKE_BUILD_TYPE}"')

# Check C++20 compiler support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++20" COMPILER_SUPPORTS_CXX20)
if(COMPILER_SUPPORTS_CXX20)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
else()
  message(FATAL_ERROR "Error, SkyFire requires a compiler that supports C++20!")
endif()

if (WIN32)
  find_library( CLANGRT_BUILTINS 
      NAMES
        clang_rt.builtins-x86_64
      PATHS
        "C:/Program Files/LLVM/lib/clang/18/lib/windows" )
endif()

#disable pragma pack warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-pragma-pack -Wno-pragma-once-outside-header -Wno-unused-value -Wno-c++11-narrowing")

if(WITH_WARNINGS)
  set(WARNING_FLAGS "-W -Wall -Wextra -Winit-self -Wfatal-errors")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WARNING_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS} -Woverloaded-virtual")
  message(STATUS "Clang: All warnings enabled")
endif()

if(WITH_COREDEBUG)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")
  message(STATUS "Clang: Debug-flags set (-g3)")
endif()
