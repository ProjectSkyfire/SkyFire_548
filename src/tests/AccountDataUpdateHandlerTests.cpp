/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace
{
    bool Expect(bool condition, char const* message)
    {
        if (!condition)
            std::cerr << message << '\n';

        return condition;
    }

    std::string ReadFile(std::string const& path)
    {
        std::ifstream input(path.c_str());
        if (!input)
        {
            std::cerr << "Could not open file: " << path << '\n';
            return "";
        }

        std::ostringstream buffer;
        buffer << input.rdbuf();
        return buffer.str();
    }

    std::string ExtractFunction(std::string const& source, char const* signature)
    {
        size_t const start = source.find(signature);
        if (start == std::string::npos)
            return "";

        size_t const openBrace = source.find('{', start);
        if (openBrace == std::string::npos)
            return "";

        uint32_t depth = 0;
        for (size_t i = openBrace; i < source.size(); ++i)
        {
            if (source[i] == '{')
                ++depth;
            else if (source[i] == '}')
            {
                if (--depth == 0)
                    return source.substr(start, i - start + 1);
            }
        }

        return "";
    }

    bool Contains(std::string const& haystack, char const* needle)
    {
        return haystack.find(needle) != std::string::npos;
    }
}

int main()
{
    std::string const miscHandler =
        ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/game/Handlers/MiscHandler.cpp");
    std::string const handleUpdateAccountData =
        ExtractFunction(miscHandler, "void WorldSession::HandleUpdateAccountData(WorldPacket& recvData)");
    std::string const opcodes =
        ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/game/Server/Protocol/Opcodes.cpp");
    std::string const opcodeHeader =
        ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/game/Server/Protocol/Opcodes.h");

    bool passed = true;
    passed &= Expect(!handleUpdateAccountData.empty(),
        "HandleUpdateAccountData should exist in MiscHandler.cpp.");
    passed &= Expect(!Contains(handleUpdateAccountData, "SMSG_UPDATE_ACCOUNT_DATA_COMPLETE"),
        "HandleUpdateAccountData should not send an account data completion response.");
    passed &= Expect(!Contains(opcodes, "DEFINE_OPCODE_HANDLER(SMSG_UPDATE_ACCOUNT_DATA_COMPLETE"),
        "The unused account data completion response should not be registered in the opcode table.");
    passed &= Expect(!Contains(opcodeHeader, "SMSG_UPDATE_ACCOUNT_DATA_COMPLETE"),
        "The unused account data completion response should not remain in the server opcode enum.");
    passed &= Expect(Contains(handleUpdateAccountData, "recvData.read_skip(compCount)") &&
        Contains(handleUpdateAccountData, "recvData.ReadBits(3)"),
        "HandleUpdateAccountData should read the account data type after the compressed payload.");
    passed &= Expect(Contains(handleUpdateAccountData, "SetAccountData(UADType, timestamp, adata)"),
        "HandleUpdateAccountData should still persist non-empty account data updates.");
    passed &= Expect(Contains(handleUpdateAccountData, "SetAccountData(UADType, 0, \"\")"),
        "HandleUpdateAccountData should still erase empty account data updates.");

    return passed ? 0 : 1;
}
