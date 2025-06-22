/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef _WORKERTHREAD_H
#define _WORKERTHREAD_H

#include "Define.h"
#include <ace/Activation_Queue.h>
#include <ace/Task.h>

class MySQLConnection;

class DatabaseWorker : protected ACE_Task_Base
{
public:
    DatabaseWorker(ACE_Activation_Queue* new_queue, MySQLConnection* con);

    ///- Inherited from ACE_Task_Base
    int svc();
    int wait() { return ACE_Task_Base::wait(); }

private:
    ACE_Activation_Queue* m_queue;
    MySQLConnection* m_conn;
    DatabaseWorker(DatabaseWorker const& right) = delete;
    DatabaseWorker& operator=(DatabaseWorker const& right) = delete;
};

#endif
