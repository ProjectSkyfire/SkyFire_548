/*
 * Copyright (C) 2011-2015 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2015 MaNGOS <http://getmangos.com/>
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

#include "LogWorker.h"

LogWorker::LogWorker()
    : m_queue(HIGH_WATERMARK, LOW_WATERMARK)
{
    ACE_Task_Base::activate(THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED, 1);
}

LogWorker::~LogWorker()
{
    m_queue.deactivate();
    wait();
}

int LogWorker::enqueue(LogOperation* op)
{
    return m_queue.enqueue(op);
}

int LogWorker::svc()
{
    while (1)
    {
        LogOperation* request;
        if (m_queue.dequeue(request) == -1)
            break;

        request->call();
        delete request;
    }

    return 0;
}
