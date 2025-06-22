/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Logger.h"

Logger::Logger() : name(""), level(LogLevel::LOG_LEVEL_DISABLED) { }

void Logger::Create(std::string const& _name, LogLevel _level)
{
    name = _name;
    level = _level;
}

std::string const& Logger::getName() const
{
    return name;
}

LogLevel Logger::getLogLevel() const
{
    return level;
}

void Logger::addAppender(uint8 id, Appender* appender)
{
    appenders[id] = appender;
}

void Logger::delAppender(uint8 id)
{
    appenders.erase(id);
}

void Logger::setLogLevel(LogLevel _level)
{
    level = _level;
}

void Logger::write(LogMessage& message) const
{
    if (level == LogLevel::LOG_LEVEL_DISABLED || level > message.level || message.text.empty())
    {
        //fprintf(stderr, "Logger::write: Logger %s, Level %u. Msg %s Level %u WRONG LEVEL MASK OR EMPTY MSG\n", getName().c_str(), message.level, message.text.c_str(), message.level); // DEBUG - RemoveMe
        return;
    }

    for (AppenderMap::const_iterator it = appenders.begin(); it != appenders.end(); ++it)
        if (it->second)
            it->second->write(message);
}
