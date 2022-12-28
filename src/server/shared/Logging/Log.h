/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRESERVER_LOG_H
#define SKYFIRESERVER_LOG_H

#include "Define.h"
#include "Appender.h"
#include "Logger.h"
#include "LogWorker.h"
#include "Dynamic/UnorderedMap.h"

#include <string>
#include <ace/Singleton.h>

#define LOGGER_ROOT "root"

class Log
{
    friend class ACE_Singleton<Log, ACE_Thread_Mutex>;

    typedef UNORDERED_MAP<std::string, Logger> LoggerMap;

    private:
        Log();
        ~Log();

    public:
        void LoadFromConfig();
        void Close();
        bool ShouldLog(std::string const& type, LogLevel level) const;
        bool SetLogLevel(std::string const& name, char const* level, bool isLogger = true);

        void outMessage(std::string const& f, LogLevel level, char const* str, ...) ATTR_PRINTF(4, 5);

        void outCommand(uint32 account, const char * str, ...) ATTR_PRINTF(3, 4);
        void outCharDump(char const* str, uint32 account_id, uint32 guid, char const* name);
        
        void SetRealmId(uint32 id);

    private:
        static std::string GetTimestampStr();
        void vlog(std::string const& f, LogLevel level, char const* str, va_list argptr);
        void write(LogMessage* msg) const;

        Logger const* GetLoggerByType(std::string const& type) const;
        Appender* GetAppenderByName(std::string const& name);
        uint8 NextAppenderId();
        void CreateAppenderFromConfig(std::string const& name);
        void CreateLoggerFromConfig(std::string const& name);
        void ReadAppendersFromConfig();
        void ReadLoggersFromConfig();

        AppenderMap appenders;
        LoggerMap loggers;
        uint8 AppenderId;

        std::string m_logsDir;
        std::string m_logsTimestamp;

        LogWorker* worker;
};

inline Logger const* Log::GetLoggerByType(std::string const& type) const
{
    LoggerMap::const_iterator it = loggers.find(type);
    if (it != loggers.end())
        return &(it->second);

    if (type == LOGGER_ROOT)
        return NULL;

    std::string parentLogger = LOGGER_ROOT;
    size_t found = type.find_last_of(".");
    if (found != std::string::npos)
        parentLogger = type.substr(0,found);

    return GetLoggerByType(parentLogger);
}

inline bool Log::ShouldLog(std::string const& type, LogLevel level) const
{
    // TODO: Use cache to store "Type.sub1.sub2": "Type" equivalence, should
    // Speed up in cases where requesting "Type.sub1.sub2" but only configured
    // Logger "Type"

    Logger const* logger = GetLoggerByType(type);
    if (!logger)
        return false;

    LogLevel logLevel = logger->getLogLevel();
    return logLevel != LogLevel::LOG_LEVEL_DISABLED && logLevel <= level;
}

inline void Log::outMessage(std::string const& filter, LogLevel level, const char * str, ...)
{
    va_list ap;
    va_start(ap, str);
    vlog(filter, level, str, ap);
    va_end(ap);
}

#define sLog ACE_Singleton<Log, ACE_Thread_Mutex>::instance()

#if COMPILER != COMPILER_MICROSOFT
#define SF_LOG_MESSAGE_BODY(filterType__, level__, ...)                 \
        do {                                                            \
            if (sLog->ShouldLog(filterType__, level__))                 \
                sLog->outMessage(filterType__, level__, __VA_ARGS__);   \
        } while (0)
#else
#define SF_LOG_MESSAGE_BODY(filterType__, level__, ...)                 \
        __pragma(warning(push))                                         \
        __pragma(warning(disable:4127))                                 \
        do {                                                            \
            if (sLog->ShouldLog(filterType__, level__))                 \
                sLog->outMessage(filterType__, level__, __VA_ARGS__);   \
        } while (0)                                                     \
        __pragma(warning(pop))
#endif

#define SF_LOG_TRACE(filterType__, ...) \
    SF_LOG_MESSAGE_BODY(filterType__, LogLevel::LOG_LEVEL_TRACE, __VA_ARGS__)

#define SF_LOG_DEBUG(filterType__, ...) \
    SF_LOG_MESSAGE_BODY(filterType__, LogLevel::LOG_LEVEL_DEBUG, __VA_ARGS__)

#define SF_LOG_INFO(filterType__, ...)  \
    SF_LOG_MESSAGE_BODY(filterType__, LogLevel::LOG_LEVEL_INFO, __VA_ARGS__)

#define SF_LOG_WARN(filterType__, ...)  \
    SF_LOG_MESSAGE_BODY(filterType__, LogLevel::LOG_LEVEL_WARN, __VA_ARGS__)

#define SF_LOG_ERROR(filterType__, ...) \
    SF_LOG_MESSAGE_BODY(filterType__, LogLevel::LOG_LEVEL_ERROR, __VA_ARGS__)

#define SF_LOG_FATAL(filterType__, ...) \
    SF_LOG_MESSAGE_BODY(filterType__, LogLevel::LOG_LEVEL_FATAL, __VA_ARGS__)

#endif
