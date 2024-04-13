/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef LOGWORKER_H
#define LOGWORKER_H

#include "LogOperation.h"

#include <ace/Activation_Queue.h>
#include <ace/Task.h>

class LogWorker : protected ACE_Task_Base
{
public:
    LogWorker();
    ~LogWorker();

    typedef ACE_Message_Queue_Ex<LogOperation, ACE_MT_SYNCH> LogMessageQueueType;

    enum
    {
        HIGH_WATERMARK = 8 * 1024 * 1024,
        LOW_WATERMARK = 8 * 1024 * 1024
    };

    int enqueue(LogOperation* op);

private:
    virtual int svc();
    LogMessageQueueType m_queue;
};

#endif
