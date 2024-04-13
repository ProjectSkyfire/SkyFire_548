/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef LOGOPERATION_H
#define LOGOPERATION_H

class Logger;
struct LogMessage;

class LogOperation
{
public:
    LogOperation(Logger const* _logger, LogMessage* _msg)
        : logger(_logger), msg(_msg)
    { }

    ~LogOperation();

    int call();

protected:
    Logger const* logger;
    LogMessage* msg;
};

#endif
