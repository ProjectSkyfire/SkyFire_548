/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_COMPILERDEFS_H
#define SKYFIRE_COMPILERDEFS_H

#define PLATFORM_WINDOWS 0
#define PLATFORM_UNIX    1
#define PLATFORM_APPLE   2
#define PLATFORM_INTEL   3

// must be first (win 64 also define _WIN32)
#if defined( _WIN64 )
#  define PLATFORM PLATFORM_WINDOWS
#elif defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#  define PLATFORM PLATFORM_WINDOWS
#elif defined( __APPLE_CC__ )
#  define PLATFORM PLATFORM_APPLE
#elif defined( __INTEL_COMPILER )
#  define PLATFORM PLATFORM_INTEL
#else
#  define PLATFORM PLATFORM_UNIX
#endif

#define COMPILER_MICROSOFT 0
#define COMPILER_GNU       1
#define COMPILER_BORLAND   2
#define COMPILER_INTEL     3
#define COMPILER_CLANG     4

#if defined _MSC_VER && _MSC_VER >= 1910
#include <stack>
#include <ratio>
#endif

#ifdef _MSC_VER
#  define COMPILER COMPILER_MICROSOFT
#elif defined( __BORLANDC__ )
#  define COMPILER COMPILER_BORLAND
#elif defined( __INTEL_COMPILER )
#  define COMPILER COMPILER_INTEL
#elif defined( __GNUC__ )
#  define COMPILER COMPILER_GNU
#  define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#elif defined(__clang__)
#  define COMPILER COMPILER_CLANG
#else
#  error "FATAL ERROR: Unknown compiler."
#endif

#if defined(__cplusplus) && __cplusplus == 201103L
#  define COMPILER_HAS_CPP11_SUPPORT 1
#else
#  define COMPILER_HAS_CPP11_SUPPORT 0
#endif

#if defined(__cplusplus) && __cplusplus == 201402L
#  define COMPILER_HAS_CPP14_SUPPORT 1
#else
#  define COMPILER_HAS_CPP14_SUPPORT 0
#endif

#if COMPILER == COMPILER_MICROSOFT && _MSC_VER >= 1910 && _HAS_CXX17
#define ACE_LACKS_AUTO_PTR 1
#endif

#endif
