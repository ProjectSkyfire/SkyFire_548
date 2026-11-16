/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "SFSoap.h"
#include "Threading/LockedQueue.h"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

// Match the world queue's close-and-drain protocol while SOAP requests race
// shutdown. Each request keeps its callback storage alive until completion.
static bool TestShutdownQueue()
{
    Skyfire::LockedQueue<SOAPCommand*, Skyfire::Mutex> queue;
    SOAPCommand pending;
    if (!queue.try_add(&pending))
        return false;

    std::atomic<unsigned> finished{0};
    std::atomic<bool> started{false};
    std::vector<std::thread> producers;
    for (unsigned i = 0; i < 8; ++i)
        producers.emplace_back([&]
        {
            started = true;
            for (unsigned n = 0; n < 300; ++n)
            {
                SOAPCommand command;
                if (!queue.try_add(&command))
                    command.complete(false);
                command.wait();
                if (!command.hasCommandSucceeded())
                    ++finished;
            }
        });

    while (!started.load())
        std::this_thread::yield();
    queue.cancel();
    SOAPCommand* command = nullptr;
    while (queue.next(command))
        command->complete(false);
    pending.wait();
    for (auto& producer : producers)
        producer.join();

    SOAPCommand late;
    if (queue.try_add(&late) || !queue.empty() || finished != 2400 || pending.hasCommandSucceeded())
        return false;
    // Closing twice must remain safe and must not resurrect the queue.
    queue.cancel();
    return !queue.try_add(&late);
}

int main()
{
    if (!TestShutdownQueue())
    {
        std::cerr << "SOAP shutdown queue failed to reject or complete requests\n";
        return 1;
    }
    SFSoapService service;

    if (service.IsRunning())
    {
        std::cerr << "New SFSoapService reported running\n";
        return 1;
    }

    if (service.StartWithRunner(std::function<void()>()))
    {
        std::cerr << "SFSoapService accepted an empty runner\n";
        return 1;
    }

    std::mutex lock;
    std::condition_variable changed;
    bool entered = false;
    bool finish = false;

    if (!service.StartWithRunner([&lock, &changed, &entered, &finish]
    {
        {
            std::lock_guard<std::mutex> guard(lock);
            entered = true;
        }

        changed.notify_all();

        std::unique_lock<std::mutex> waitLock(lock);
        changed.wait(waitLock, [&finish]
        {
            return finish;
        });
    }))
    {
        std::cerr << "SFSoapService rejected the first runner\n";
        return 1;
    }

    if (!service.IsRunning())
    {
        std::cerr << "SFSoapService did not report running after start\n";
        return 1;
    }

    {
        std::unique_lock<std::mutex> waitLock(lock);
        if (!changed.wait_for(waitLock, std::chrono::seconds(2), [&entered]
        {
            return entered;
        }))
        {
            std::cerr << "SFSoapService runner did not start\n";
            return 1;
        }
    }

    if (service.StartWithRunner([] { }))
    {
        std::cerr << "SFSoapService accepted a second runner while running\n";
        return 1;
    }

    {
        std::lock_guard<std::mutex> guard(lock);
        finish = true;
    }

    changed.notify_all();
    service.Join();

    if (service.IsRunning())
    {
        std::cerr << "SFSoapService still reported running after join\n";
        return 1;
    }

    service.Join();

    return 0;
}
