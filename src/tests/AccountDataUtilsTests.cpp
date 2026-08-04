/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "AccountDataUtils.h"

#include <iostream>
#include <string>

namespace
{
    bool Expect(bool condition, char const* message)
    {
        if (!condition)
            std::cerr << message << '\n';

        return condition;
    }
}

int main()
{
    bool passed = true;

    passed &= Expect(Skyfire::AccountData::HasEmptyCharacterChatChannels(
        "VERSION 5\n\nCHANNELS\nEND\n\nZONECHANNELS 35651587\n"),
        "Closed chat channel blocks with no channel entries should be treated as empty.");

    passed &= Expect(Skyfire::AccountData::HasEmptyCharacterChatChannels(
        "CHANNELS\r\n \r\n\t\r\nEND\r\n"),
        "Blank and whitespace-only lines inside the chat channel block should still be empty.");

    passed &= Expect(!Skyfire::AccountData::HasEmptyCharacterChatChannels(
        "CHANNELS\n1 General - Stormwind City\nEND\n"),
        "Populated chat channel blocks should be preserved.");

    passed &= Expect(!Skyfire::AccountData::HasEmptyCharacterChatChannels(
        "VERSION 5\nZONECHANNELS 35651587\n"),
        "Account data without a chat channel block should not be treated as empty chat data.");

    passed &= Expect(!Skyfire::AccountData::HasEmptyCharacterChatChannels(
        "CHANNELS\n"),
        "Unclosed chat channel blocks should be preserved.");

    return passed ? 0 : 1;
}
