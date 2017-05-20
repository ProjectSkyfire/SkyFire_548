/*
 * Copyright (C) 2011-2017 Project SkyFire <http://www.projectskyfire.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
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
