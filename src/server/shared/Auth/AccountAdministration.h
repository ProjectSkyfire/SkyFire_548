/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#ifndef SKYFIRE_ACCOUNT_ADMINISTRATION_H
#define SKYFIRE_ACCOUNT_ADMINISTRATION_H

#include <map>
#include <string>

namespace Skyfire::Auth
{
    struct AccountAdminReply
    {
        int Status = 500;
        std::string Body;
    };

    // Shared implementation: worldserver owns online mutations, hub owns offline ones.
    class AccountAdministration
    {
    public:
        using Form = std::map<std::string, std::string>;
        static void SetEnabled(bool enabled);
        static bool IsEnabled();
        static bool IsReadAction(std::string const& action);
        static AccountAdminReply Handle(std::string const& action, Form const& form, std::string const& actor);
        static std::string EncodeRequest(std::string const& action, Form form, std::string const& actor);
        static AccountAdminReply HandleEncodedRequest(std::string const& encoded);
    };
}
#endif
