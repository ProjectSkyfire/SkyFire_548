/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "AccountDataUtils.h"

#include <cctype>

namespace
{
    std::string TrimAscii(std::string const& value)
    {
        std::string::size_type first = 0;
        while (first < value.size() && std::isspace(static_cast<unsigned char>(value[first])))
            ++first;

        std::string::size_type last = value.size();
        while (last > first && std::isspace(static_cast<unsigned char>(value[last - 1])))
            --last;

        return value.substr(first, last - first);
    }
}

namespace Skyfire
{
namespace AccountData
{
    bool HasEmptyCharacterChatChannels(std::string const& data)
    {
        bool inChannels = false;
        bool sawChannelLine = false;

        for (std::string::size_type lineStart = 0; lineStart <= data.size();)
        {
            std::string::size_type lineEnd = data.find_first_of("\r\n", lineStart);
            std::string const line = TrimAscii(data.substr(lineStart, lineEnd == std::string::npos ? std::string::npos : lineEnd - lineStart));

            if (!inChannels)
            {
                if (line == "CHANNELS")
                    inChannels = true;
            }
            else
            {
                if (line == "END")
                    return !sawChannelLine;

                if (!line.empty())
                    sawChannelLine = true;
            }

            if (lineEnd == std::string::npos)
                break;

            lineStart = lineEnd + 1;
            if (lineEnd + 1 < data.size() && data[lineEnd] == '\r' && data[lineEnd + 1] == '\n')
                ++lineStart;
        }

        return false;
    }
}
}
