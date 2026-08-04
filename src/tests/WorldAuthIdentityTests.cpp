/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

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

    bool Contains(std::string const& haystack, char const* needle)
    {
        return haystack.find(needle) != std::string::npos;
    }
}

int main()
{
    std::string const worldSocket =
        ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/game/Server/WorldSocket.cpp");
    std::string const miscHandler =
        ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/game/Handlers/MiscHandler.cpp");
    std::string const loginDatabase =
        ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/shared/Database/Implementation/LoginDatabase.cpp");

    bool passed = true;
    passed &= Expect(Contains(worldSocket, "#include \"Auth/LoginIdentity.h\""),
        "World auth should include login identity classification.");
    passed &= Expect(Contains(worldSocket, "NormalizeLoginIdentity(account)"),
        "World auth should classify CMSG_AUTH_SESSION account names.");
    passed &= Expect(Contains(worldSocket, "LOGIN_SEL_ACCOUNT_INFO_BY_LOGIN_IDENTITY"),
        "World auth should resolve email-shaped account names through login identities.");
    passed &= Expect(Contains(loginDatabase, "LOGIN_SEL_ACCOUNT_INFO_BY_LOGIN_IDENTITY"),
        "Login database should prepare account-info lookup by login identity.");
    passed &= Expect(Contains(miscHandler, "ObjectGuid guid = GetPlayer() ? GetPlayer()->GetGUID() : 0;"),
        "Requested account data should be sent with the logged-in player GUID when available.");

    return passed ? 0 : 1;
}
