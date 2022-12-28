/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_HASH_NAMESPACE_H
#define SKYFIRE_HASH_NAMESPACE_H

#include "Define.h"

#if COMPILER_HAS_CPP11_SUPPORT
# define HASH_NAMESPACE_START namespace std {
# define HASH_NAMESPACE_END }
#elif defined(_STLPORT_VERSION)
# define HASH_NAMESPACE_START namespace std {
# define HASH_NAMESPACE_END }
#elif COMPILER == COMPILER_MICROSOFT && _MSC_VER >= 1600 // VS100
# define HASH_NAMESPACE_START namespace std {
# define HASH_NAMESPACE_END }
#elif COMPILER == COMPILER_MICROSOFT && _MSC_VER >= 1500 && _HAS_TR1
# define HASH_NAMESPACE_START namespace std { namespace tr1 {
# define HASH_NAMESPACE_END } }
#elif COMPILER == COMPILER_MICROSOFT && _MSC_VER >= 1300
# define HASH_NAMESPACE_START namespace stdext {
# define HASH_NAMESPACE_END }

#if !_HAS_TRADITIONAL_STL
#ifndef HASH_NAMESPACE
#define HASH_NAMESPACE
#else

// can be not used by some platforms, so provide fake forward
HASH_NAMESPACE_START

template<class K>
class hash
{
public:
    size_t operator() (K const&);
};

HASH_NAMESPACE_END

#endif
#endif

#elif COMPILER == COMPILER_INTEL
# define HASH_NAMESPACE_START namespace std {
# define HASH_NAMESPACE_END }
#elif COMPILER == COMPILER_GNU && defined(__clang__) && defined(_LIBCPP_VERSION)
# define HASH_NAMESPACE_START namespace std {
# define HASH_NAMESPACE_END }
#elif COMPILER == COMPILER_GNU && GCC_VERSION > 40200
# define HASH_NAMESPACE_START namespace std { namespace tr1 {
# define HASH_NAMESPACE_END } }
#elif COMPILER == COMPILER_GNU && GCC_VERSION >= 30000
# define HASH_NAMESPACE_START namespace __gnu_cxx {
# define HASH_NAMESPACE_END }

#include <ext/hash_fun.h>
#include <string>

HASH_NAMESPACE_START

template<>
class hash<unsigned long long>
{
public:
    size_t operator()(const unsigned long long &__x) const { return (size_t)__x; }
};

template<typename T>
class hash<T *>
{
public:
    size_t operator()(T * const &__x) const { return (size_t)__x; }
};

template<> struct hash<std::string>
{
    size_t operator()(const std::string &__x) const
    {
        return hash<char const*>()(__x.c_str());
    }
};

HASH_NAMESPACE_END

#else
# define HASH_NAMESPACE_START namespace std {
# define HASH_NAMESPACE_END }
#endif

#if COMPILER != COMPILER_MICROSOFT

// Visual Studio use non standard hash calculation function, so provide fake forward for other
HASH_NAMESPACE_START

template<class K>
size_t hash_value(K const&);

HASH_NAMESPACE_END

#endif

#endif
