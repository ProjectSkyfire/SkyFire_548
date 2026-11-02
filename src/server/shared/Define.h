/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_DEFINE_H
#define SKYFIRE_DEFINE_H

#include "CompilerDefs.h"

#include <cinttypes>
#include <cstddef>
#include <cstdint>

#define SKYFIRE_LITTLEENDIAN 0
#define SKYFIRE_BIGENDIAN    1

#if !defined(SKYFIRE_ENDIAN)
#  if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#    define SKYFIRE_ENDIAN SKYFIRE_BIGENDIAN
#  else
#    define SKYFIRE_ENDIAN SKYFIRE_LITTLEENDIAN
#  endif
#endif //SKYFIRE_ENDIAN

#if PLATFORM == PLATFORM_WINDOWS
#  define SKYFIRE_PATH_MAX MAX_PATH
#  ifndef DECLSPEC_NORETURN
#    define DECLSPEC_NORETURN __declspec(noreturn)
#  endif //DECLSPEC_NORETURN
#  ifndef DECLSPEC_DEPRECATED
#    define DECLSPEC_DEPRECATED __declspec(deprecated)
#  endif //DECLSPEC_DEPRECATED
#else //PLATFORM != PLATFORM_WINDOWS
#  include <limits.h>
#  ifndef PATH_MAX
#    define PATH_MAX 4096
#  endif
#  define SKYFIRE_PATH_MAX PATH_MAX
#  define DECLSPEC_NORETURN
#  define DECLSPEC_DEPRECATED
#endif //PLATFORM

#if !defined(COREDEBUG)
#  define SKYFIRE_INLINE inline
#else //COREDEBUG
#  if !defined(SKYFIRE_DEBUG)
#    define SKYFIRE_DEBUG
#  endif //SKYFIRE_DEBUG
#  define SKYFIRE_INLINE
#endif //!COREDEBUG

#if COMPILER == COMPILER_GNU
#  define ATTR_NORETURN __attribute__((noreturn))
#  define ATTR_PRINTF(F, V) __attribute__ ((format (printf, F, V)))
#  define ATTR_DEPRECATED __attribute__((deprecated))
#else //COMPILER != COMPILER_GNU
#  define ATTR_NORETURN
#  define ATTR_PRINTF(F, V)
#  define ATTR_DEPRECATED
#endif //COMPILER == COMPILER_GNU


#define OVERRIDE override
#define FINAL final


#define UI64FMTD "%" PRIu64
#define UI64LIT(N) UINT64_C(N)

#define SI64FMTD "%" PRId64
#define SI64LIT(N) INT64_C(N)

#define SIZEFMTD "%zu"

typedef std::int64_t int64;
typedef std::int32_t int32;
typedef std::int16_t int16;
typedef std::int8_t int8;
typedef std::uint64_t uint64;
typedef std::uint32_t uint32;
typedef std::uint16_t uint16;
typedef std::uint8_t uint8;

enum DBCFormer
{
    FT_NA = 'x',                                              //not used or unknown, 4 byte size
    FT_NA_BYTE = 'X',                                         //not used or unknown, byte
    FT_STRING = 's',                                          //char*
    FT_FLOAT = 'f',                                           //float
    FT_INT = 'i',                                             //uint32
    FT_BYTE = 'b',                                            //uint8
    FT_SORT = 'd',                                            //sorted by this field, field is not included
    FT_IND = 'n',                                             //the same, but parsed to data
    FT_SQL_PRESENT = 'p',                                     //Used in sql format to mark column present in sql dbc
    FT_SQL_ABSENT = 'a'                                       //Used in sql format to mark column absent in sql dbc
};

#endif //SKYFIRE_DEFINE_H
