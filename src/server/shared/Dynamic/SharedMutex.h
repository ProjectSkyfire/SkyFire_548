/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_SHAREDMUTEX_H
#define SKYFIRE_SHAREDMUTEX_H

#if COMPILER == COMPILER_MICROSOFT && _MSC_VER >= 1900
#include <shared_mutex>
#define SF_SHARED_MUTEX std::shared_mutex
#define SF_SHARED_GUARD std::shared_lock<std::shared_mutex>
#define SF_UNIQUE_GUARD std::unique_lock<std::shared_mutex>
#else
#include <mutex>
#define SF_SHARED_MUTEX std::mutex
#define SF_SHARED_GUARD std::lock_guard<std::mutex>
#define SF_UNIQUE_GUARD std::unique_lock<std::mutex>
#endif

#endif
