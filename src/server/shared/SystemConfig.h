/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

// Core configuration options

#ifndef SKYFIRE_SYSTEMCONFIG_H
#define SKYFIRE_SYSTEMCONFIG_H

#include "Define.h"
#include "revision.h"

#define _PACKAGENAME "SkyFire 5.x.x"

// Format is YYYYMMDDRR where RR is the change in the conf file
// for that day.
#ifndef SKYFIREWORLD_CONFIG_VERSION
# define SKYFIREWORLD_CONFIG_VERSION 2024050400
#endif
#ifndef SKYFIREAUTH_CONFIG_VERSION
# define SKYFIREAUTH_CONFIG_VERSION 2017021900
#endif

#if SKYFIRE_ENDIAN == SKYFIRE_BIGENDIAN
# define _ENDIAN_STRING "big-endian"
#else
# define _ENDIAN_STRING "little-endian"
#endif

#if PLATFORM == PLATFORM_WINDOWS
# ifdef _WIN64
#  define _FULLVERSION _PACKAGENAME " rev. " VER_PRODUCTVERSION_STR " (Win64, " _BUILD_DIRECTIVE ")"
# else
#  define _FULLVERSION _PACKAGENAME " rev. " VER_PRODUCTVERSION_STR " (Win32, " _BUILD_DIRECTIVE ")"
# endif
#else
#  define _FULLVERSION _PACKAGENAME " rev. " VER_PRODUCTVERSION_STR " (Unix, " _BUILD_DIRECTIVE ")"
#endif
#endif
