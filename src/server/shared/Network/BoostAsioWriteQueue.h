/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_BOOSTASIOWRITEQUEUE_H
#define SF_BOOSTASIOWRITEQUEUE_H

#include <boost/asio/bind_executor.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/write.hpp>
#include <boost/system/error_code.hpp>
#include <atomic>
#include <deque>
#include <functional>
#include <vector>

namespace Skyfire
{
namespace Net
{
    template<class AsyncWriteStream>
    class BoostAsioWriteQueue
    {
    public:
        typedef std::function<void(boost::system::error_code const&, size_t)> CompletionHandler;

        explicit BoostAsioWriteQueue(AsyncWriteStream& stream) :
            _stream(stream),
            _strand(boost::asio::make_strand(stream.get_executor())),
            _queue(),
            _pendingBytes(0),
            _writeInProgress(false)
        {
        }

        bool HasPendingOutput() const
        {
            return _pendingBytes.load(std::memory_order_acquire) != 0;
        }

        void Queue(std::vector<char> data, CompletionHandler handler)
        {
            if (data.empty())
            {
                boost::asio::post(_strand,
                    [handler = std::move(handler)]() mutable
                    {
                        if (handler)
                            handler(boost::system::error_code(), 0);
                    });
                return;
            }

            _pendingBytes.fetch_add(data.size(), std::memory_order_acq_rel);
            boost::asio::post(_strand,
                [this, data = std::move(data), handler = std::move(handler)]() mutable
                {
                    bool startWrite = !_writeInProgress && _queue.empty();
                    _queue.push_back(Entry(std::move(data), std::move(handler)));

                    if (startWrite)
                        StartWrite();
                });
        }

    private:
        struct Entry
        {
            Entry(std::vector<char> data, CompletionHandler handler) :
                Data(std::move(data)),
                Handler(std::move(handler))
            {
            }

            std::vector<char> Data;
            CompletionHandler Handler;
        };

        void StartWrite()
        {
            if (_queue.empty())
            {
                _writeInProgress = false;
                return;
            }

            _writeInProgress = true;
            boost::asio::async_write(_stream, boost::asio::buffer(_queue.front().Data),
                boost::asio::bind_executor(_strand,
                    [this](boost::system::error_code const& error, size_t transferredBytes)
                    {
                        HandleWrite(error, transferredBytes);
                    }));
        }

        void HandleWrite(boost::system::error_code const& error, size_t transferredBytes)
        {
            if (_queue.empty())
            {
                _writeInProgress = false;
                return;
            }

            Entry completed(std::move(_queue.front()));
            _queue.pop_front();
            SubtractPending(completed.Data.size());

            if (error)
            {
                std::deque<Entry> failedQueue;
                failedQueue.swap(_queue);

                for (typename std::deque<Entry>::iterator itr = failedQueue.begin(); itr != failedQueue.end(); ++itr)
                    SubtractPending(itr->Data.size());

                _writeInProgress = false;

                for (typename std::deque<Entry>::iterator itr = failedQueue.begin(); itr != failedQueue.end(); ++itr)
                    if (itr->Handler)
                        itr->Handler(error, 0);

                if (completed.Handler)
                    completed.Handler(error, transferredBytes);

                return;
            }

            if (!_queue.empty())
                StartWrite();
            else
                _writeInProgress = false;

            if (completed.Handler)
                completed.Handler(error, transferredBytes);
        }

        void SubtractPending(size_t bytes)
        {
            _pendingBytes.fetch_sub(bytes, std::memory_order_acq_rel);
        }

        AsyncWriteStream& _stream;
        boost::asio::strand<typename AsyncWriteStream::executor_type> _strand;
        std::deque<Entry> _queue;
        std::atomic<size_t> _pendingBytes;
        bool _writeInProgress;
    };
}
}

#endif
