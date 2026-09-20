/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#ifndef SKYFIRE_WORLD_OWNERSHIP_LOCK_H
#define SKYFIRE_WORLD_OWNERSHIP_LOCK_H

#include <filesystem>
#include <string>
#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>
#endif

// Same-host exclusion, held from before database setup until Master has closed its pools.
// The file is persistent: unlinking a POSIX lock file would permit a second inode/owner.
class WorldOwnershipLock
{
public:
    WorldOwnershipLock() = default;
    WorldOwnershipLock(WorldOwnershipLock const&) = delete;
    WorldOwnershipLock& operator=(WorldOwnershipLock const&) = delete;
    ~WorldOwnershipLock()
    {
#ifdef _WIN32
        if (_handle != INVALID_HANDLE_VALUE) CloseHandle(_handle);
#else
        if (_handle >= 0) close(_handle);
#endif
    }
    bool Acquire(std::string const& name, std::string& error)
    {
        try
        {
            std::filesystem::path path(name);
            if (!path.is_absolute()) { error = "World.OwnershipLock must be an absolute local path."; return false; }
            path = std::filesystem::weakly_canonical(path);
#ifdef _WIN32
            if (GetDriveTypeW(path.root_path().c_str()) != DRIVE_FIXED)
            { error = "World ownership requires a local fixed disk, not a network/removable drive."; return false; }
            if (_handle != INVALID_HANDLE_VALUE) { error = "Ownership lock already acquired."; return false; }
            _handle = CreateFileW(path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
                nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
            OVERLAPPED offset{};
            if (_handle == INVALID_HANDLE_VALUE || !LockFileEx(_handle, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY, 0, 1, 0, &offset))
#else
            if (_handle >= 0) { error = "Ownership lock already acquired."; return false; }
            _handle = open(path.c_str(), O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW, 0600);
            if (_handle < 0 || flock(_handle, LOCK_EX | LOCK_NB) != 0)
#endif
            { error = "World ownership is already held, or its local lock file is inaccessible."; return false; }
            _path = path.string();
            return true;
        }
        catch (std::filesystem::filesystem_error const&)
        { error = "Cannot resolve world ownership lock path."; return false; }
    }
    std::string const& Path() const { return _path; }
private:
    std::string _path;
#ifdef _WIN32
    HANDLE _handle = INVALID_HANDLE_VALUE;
#else
    int _handle = -1;
#endif
};
#endif
