/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Logger.h"
#include "LogOperation.h"

LogOperation::~LogOperation()
{
    delete msg;
}

int LogOperation::call()
{
    if (logger && msg)
        logger->write(*msg);
    return 0;
}
