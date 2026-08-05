#
# This file is part of Project SkyFire https://www.projectskyfire.org. 
# See COPYRIGHT file for Copyright information
#

# set up output paths for executable binaries (.exe-files, and .dll-files on DLL-capable platforms)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(CheckCXXCompilerFlag)

function(sf_scrub_cached_msvc_options variable)
    if(NOT DEFINED ${variable})
        return()
    endif()

    set(_value " ${${variable}} ")

    foreach(_option IN LISTS ARGN)
        string(REPLACE " ${_option} " " " _value "${_value}")
    endforeach()

    string(REGEX REPLACE " /Zm[0-9]+ " " " _value "${_value}")
    string(REGEX REPLACE " +" " " _value "${_value}")
    string(STRIP "${_value}" _value)
    set(${variable} "${_value}" CACHE STRING "" FORCE)
endfunction()

function(sf_add_msvc_compile_option option)
    add_compile_options(
        "$<$<COMPILE_LANG_AND_ID:C,MSVC>:${option}>"
        "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:${option}>"
    )
endfunction()

function(sf_add_msvc_cxx_compile_option option)
    add_compile_options("$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:${option}>")
endfunction()

set(SF_LEGACY_MSVC_CACHE_OPTIONS
    /MP
    /std:c++latest
    /wd4996
    /wd4355
    /wd4244
    /wd4267
    /wd4619
    /we4263
    /we4264
    /bigobj
)

foreach(_sf_flag_var
    CMAKE_C_FLAGS
    CMAKE_C_FLAGS_DEBUG
    CMAKE_C_FLAGS_RELEASE
    CMAKE_C_FLAGS_RELWITHDEBINFO
    CMAKE_C_FLAGS_MINSIZEREL
    CMAKE_CXX_FLAGS
    CMAKE_CXX_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_RELEASE
    CMAKE_CXX_FLAGS_RELWITHDEBINFO
    CMAKE_CXX_FLAGS_MINSIZEREL)
    sf_scrub_cached_msvc_options(${_sf_flag_var} ${SF_LEGACY_MSVC_CACHE_OPTIONS})
endforeach()

set(MSVC_EXPECTED_VERSION 19.42.34438.0)
if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS MSVC_EXPECTED_VERSION)
  message(FATAL_ERROR "MSVC: SkyFire requires version ${MSVC_EXPECTED_VERSION} (MSVC 2022) to build but found ${CMAKE_CXX_COMPILER_VERSION}")
endif()

set(MSVC_UNSUPPORTED_VERSION 19.39.33521.0)
if(CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL MSVC_UNSUPPORTED_VERSION)
  message(FATAL_ERROR "MSVC: Unsupported version ${CMAKE_CXX_COMPILER_VERSION} found.")
endif()

# set up output paths ofr static libraries etc (commented out - shown here as an example only)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# This definition is necessary to work around a bug with Intellisense described
# here: http://tinyurl.com/2cb428. Syntax highlighting is important for proper
# debugger functionality.
add_definitions("-D_WIN64")
message(STATUS "MSVC: 64-bit platform, enforced -D_WIN64 parameter")

#Enable extended object support for debug compiles on X64.
add_compile_options("$<$<AND:$<COMPILE_LANG_AND_ID:CXX,MSVC>,$<CONFIG:Debug>>:/bigobj>")
message(STATUS "MSVC: Enabled extended object-support for debug-compiles")

# Set build-directive (used in core to tell which buildtype we used)
add_definitions(-D_BUILD_DIRECTIVE=\\"$(ConfigurationName)\\")

# multithreaded compiling on VS
sf_add_msvc_compile_option(/MP)

# Define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES - eliminates the warning by changing the strcpy call to strcpy_s, which prevents buffer overruns
add_definitions(-D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES)
message(STATUS "MSVC: Overloaded standard names")

# Ignore warnings about older, less secure functions
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
message(STATUS "MSVC: Disabled NON-SECURE warnings")

#Ignore warnings about POSIX deprecation
add_definitions(-D_CRT_NONSTDC_NO_WARNINGS)
message(STATUS "MSVC: Disabled POSIX warnings")

if(NOT WITH_WARNINGS)
    sf_add_msvc_compile_option(/wd4996)
    sf_add_msvc_compile_option(/wd4355)
    sf_add_msvc_compile_option(/wd4244)
    sf_add_msvc_compile_option(/wd4267)
    sf_add_msvc_compile_option(/wd4619)
    message(STATUS "MSVC: Disabled generic compiletime warnings")
endif()

if(WITH_CXX_23_STD OR WITH_CXX_DRAFT_STD)
    sf_add_msvc_cxx_compile_option(/std:c++latest) #c++latest is currently c++23 in msvc 2022
endif()

if(WITH_CXX_23_STD)
    message(STATUS "MSVC: C++23 Draft Standard Enabled.")
endif()

if(WITH_CXX_DRAFT_STD)
    message(STATUS "MSVC: C++ Draft Standard Enabled.")
endif()

# Specify the maximum PreCompiled Header memory allocation limit
# Fixes a compiler-problem when using PCH - the /Ym flag is adjusted by the compiler in MSVC2012, hence we need to set an upper limit with /Zm to avoid discrepancies)
# (And yes, this is a verified , unresolved bug with MSVC... *sigh*)
sf_add_msvc_cxx_compile_option(/Zm50)

# Enable and treat as errors the following warnings to easily detect virtual function signature failures:
# 'function' : member function does not override any base class virtual member function
# 'virtual_function' : no override available for virtual member function from base 'class'; function is hidden
sf_add_msvc_cxx_compile_option(/we4263)
sf_add_msvc_cxx_compile_option(/we4264)

set(CMAKE_CXX_STANDARD_LIBRARIES "ws2_32.lib iphlpapi.lib netapi32.lib mswsock.lib psapi.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib")
set(CMAKE_C_STANDARD_LIBRARIES "ws2_32.lib iphlpapi.lib netapi32.lib mswsock.lib psapi.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib")
