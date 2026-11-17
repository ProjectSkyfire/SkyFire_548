/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef LOCKEDQUEUE_H
#define LOCKEDQUEUE_H

#include "Debugging/Errors.h"
#include "Platform/Threading.h"
#include <assert.h>
#include <cstddef>
#include <deque>
#include <mutex>

namespace Skyfire
{
    template <class T, class LockType, typename StorageType = std::deque<T> >
    class LockedQueue
    {
        //! Lock access to the queue.
        LockType _lock;

        //! Storage backing the queue.
        StorageType _queue;

        //! Cancellation flag.
        volatile bool _canceled;

    public:
        //! Create a LockedQueue.
        LockedQueue() : _canceled(false) { }

        //! Destroy a LockedQueue.
        virtual ~LockedQueue() { }

        //! Adds an item to the queue.
        std::size_t add(const T& item)
        {
            std::lock_guard<LockType> g(this->_lock);

            //ASSERT(!this->_canceled);
            // throw Cancellation_Exception();

            _queue.push_back(item);

            return _queue.size();
        }

        // Unlike add(), this opt-in operation rejects items after cancellation.
        // Checking the flag and enqueueing under one lock closes the shutdown race.
        bool try_add(const T& item)
        {
            std::lock_guard<LockType> g(this->_lock);
            if (_canceled)
                return false;
            _queue.push_back(item);
            return true;
        }

        //! Gets the next result in the queue, if any.
        bool next(T& result)
        {
            std::lock_guard<LockType> g(this->_lock);

            if (_queue.empty())
                return false;

            //ASSERT (!_queue.empty() || !this->_canceled);
            // throw Cancellation_Exception();
            result = _queue.front();
            _queue.pop_front();

            return true;
        }

        template<class Checker>
        bool next(T& result, Checker& check)
        {
            std::lock_guard<LockType> g(this->_lock);

            if (_queue.empty())
                return false;

            result = _queue.front();
            if (!check.Process(result))
                return false;

            _queue.pop_front();
            return true;
        }

        //! Peeks at the top of the queue. Check if the queue is empty before calling! Remember to unlock after use if autoUnlock == false.
        T& peek(bool autoUnlock = false)
        {
            lock();

            T& result = _queue.front();

            if (autoUnlock)
                unlock();

            return result;
        }

        //! Cancels the queue.
        void cancel()
        {
            lock();

            _canceled = true;

            unlock();
        }

        //! Checks if the queue is cancelled.
        bool cancelled()
        {
            std::lock_guard<LockType> g(this->_lock);
            return _canceled;
        }

        //! Locks the queue for access.
        void lock()
        {
            this->_lock.lock();
        }

        //! Unlocks the queue.
        void unlock()
        {
            this->_lock.unlock();
        }

        ///! Calls pop_front of the queue
        void pop_front()
        {
            std::lock_guard<LockType> g(this->_lock);
            _queue.pop_front();
        }

        ///! Checks if we're empty or not with locks held
        bool empty()
        {
            std::lock_guard<LockType> g(this->_lock);
            return _queue.empty();
        }

        ///! Returns current queue depth with locks held
        std::size_t size()
        {
            std::lock_guard<LockType> g(this->_lock);
            return _queue.size();
        }
    };
}

#endif
