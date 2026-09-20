/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef CONFIG_H
#define CONFIG_H

#include "Platform/Singleton.h"
#include <list>
#include <map>
#include <mutex>
#include <string>
#include <utility>

typedef std::map<std::string, std::string> ConfigSection;
typedef std::map<std::string, ConfigSection> Config;

class ConfigMgr
{
    friend class Skyfire::Singleton<ConfigMgr, Skyfire::NullMutex>;
    friend class ConfigLoader;

    ConfigMgr() : _configLoaded(false) { }
    ~ConfigMgr() { }

public:
    /// Method used only for loading main configuration files (authserver.conf and worldserver.conf)
    bool LoadInitial(char const* file);

    /**
     * This method loads additional configuration files
     * It is recommended to use this method in WorldScript::OnConfigLoad hooks
     *
     * @return true if loading was successful
     */
    bool LoadMore(char const* file);

    bool Reload();

    std::string GetStringDefault(const char* name, const std::string& def);
    bool GetBoolDefault(const char* name, bool def);
    int GetIntDefault(const char* name, int def);
    float GetFloatDefault(const char* name, float def);

    std::string const& GetFilename();
    std::list<std::string> GetKeysByString(std::string const& name);

private:
    bool GetValueHelper(const char* name, std::string& result);
    bool LoadData(char const* file);

    typedef std::mutex LockType;
    typedef std::lock_guard<std::mutex> GuardType;

    std::string _filename;
    Config _config;
    bool _configLoaded;
    LockType _configLock;

    ConfigMgr(ConfigMgr const&);
    ConfigMgr& operator=(ConfigMgr const&);
};

// Read another service's configuration using identical parsing without changing global settings.
class ConfigLoader
{
public:
    static bool Load(std::string const& filename, Config& result)
    {
        ConfigMgr local;
        if (!local.LoadInitial(filename.c_str())) return false;
        result = std::move(local._config);
        return true;
    }
};

#define sConfigMgr Skyfire::Singleton<ConfigMgr, Skyfire::NullMutex>::instance()

#endif
