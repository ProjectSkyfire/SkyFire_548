/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ByteBuffer.h"
#include "Common.h"
#include "Log.h"

#include <ace/Stack_Trace.h>
#include <sstream>

ByteBufferPositionException::ByteBufferPositionException(bool add, size_t pos,
                                                         size_t size, size_t valueSize)
{
    std::ostringstream ss;
    ACE_Stack_Trace trace;

    ss << "Attempted to " << (add ? "put" : "get") << " value with size: "
       << valueSize << " in ByteBuffer (pos: " << pos << " size: " << size
       << ")\n\n" << trace.c_str();

    message().assign(ss.str());
    printf("\nError Message => [%s]\n\n", message().c_str());
}

ByteBufferSourceException::ByteBufferSourceException(size_t pos, size_t size,
                                                     size_t valueSize)
{
    std::ostringstream ss;
    ACE_Stack_Trace trace;

    ss << "Attempted to put a "
       << (valueSize > 0 ? "NULL-pointer" : "zero-sized value")
       << " in ByteBuffer (pos: " << pos << " size: " << size << ")\n\n"
       << trace.c_str();

    message().assign(ss.str());
}

void ByteBuffer::print_storage() const
{
    if (!sLog->ShouldLog("network", LogLevel::LOG_LEVEL_TRACE)) // optimize disabled trace output
        return;

    std::ostringstream o;
    o << "STORAGE_SIZE: " << size();
    for (uint32 i = 0; i < size(); ++i)
        o << read<uint8>(i) << " - ";
    o << " ";

    SF_LOG_TRACE("network", "%s", o.str().c_str());
}

void ByteBuffer::textlike() const
{
    if (!sLog->ShouldLog("network", LogLevel::LOG_LEVEL_TRACE)) // optimize disabled trace output
        return;

    std::ostringstream o;
    o << "STORAGE_SIZE: " << size();
    for (uint32 i = 0; i < size(); ++i)
    {
        char buf[2];
        snprintf(buf, sizeof(buf), "%c", read<uint8>(i));
        o << buf;
    }
    o << " ";
    SF_LOG_TRACE("network", "%s", o.str().c_str());
}

void ByteBuffer::hexlike() const
{
    if (!sLog->ShouldLog("network", LogLevel::LOG_LEVEL_TRACE)) // optimize disabled trace output
        return;

    uint32 j = 1, k = 1;

    std::ostringstream o;
    o << "STORAGE_SIZE: " << size();

    for (uint32 i = 0; i < size(); ++i)
    {
        char buf[4];
        snprintf(buf, sizeof(buf), "%2X ", read<uint8>(i));
        if ((i == (j * 8)) && ((i != (k * 16))))
        {
            o << "| ";
            ++j;
        }
        else if (i == (k * 16))
        {
            o << "\n";
            ++k;
            ++j;
        }

        o << buf;
    }
    o << " ";
    SF_LOG_TRACE("network", "%s", o.str().c_str());
}
