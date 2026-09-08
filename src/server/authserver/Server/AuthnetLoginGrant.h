/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_AUTHNETLOGINGRANT_H
#define SF_AUTHNETLOGINGRANT_H

#include "Define.h"

#include <string>

namespace Skyfire::Authnet
{
    bool IssueLoginGrant(uint32 accountId, std::string const& remoteAddress, uint32 ttlSeconds);
    bool HasLoginGrant(uint32 accountId, std::string const& remoteAddress);
    bool ConsumeLoginGrant(uint32 accountId, std::string const& remoteAddress);
}

#endif
