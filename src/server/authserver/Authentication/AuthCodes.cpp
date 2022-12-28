/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "AuthCodes.h"
#include <cstddef>

namespace AuthHelper
{
    static RealmBuildInfo const PostBcAcceptedClientBuilds[] =
    {
        {18414, 5, 4, 8, ' '},        // Last version MoP
        {18291, 5, 4, 8, ' '},
        {18019, 5, 4, 7, ' '},
        {17956, 5, 4, 7, ' '},
        {17930, 5, 4, 7, ' '},
        {17898, 5, 4, 7, ' '},
        {17688, 5, 4, 2, 'a'},
        {17658, 5, 4, 2, ' '},
        {17538, 5, 4, 1, ' '},
        {17399, 5, 4, 0, ' '},
        {17128, 5, 3, 0, ' '},
        {16769, 5, 2, 0, ' '},
        {16357, 5, 1, 0, 'a'},
        {16309, 5, 1, 0, ' '},
        {16135, 5, 0, 5, 'b'},
        {15595, 4, 3, 4, ' '},
        {14545, 4, 2, 2, ' '},
        {13623, 4, 0, 6, 'a'},
        {12340, 3, 3, 5, 'a'},
        {11723, 3, 3, 3, 'a'},
        {11403, 3, 3, 2, ' '},
        {11159, 3, 3, 0, 'a'},
        {10505, 3, 2, 2, 'a'},
        {9947,  3, 1, 3, ' '},
        {8606,  2, 4, 3, ' '},
        {0,     0, 0, 0, ' '}                                   // terminator
    };

    static RealmBuildInfo const PreBcAcceptedClientBuilds[] =
    {
        {6141,  1, 12, 3, ' '},
        {6005,  1, 12, 2, ' '},
        {5875,  1, 12, 1, ' '},
        {0,     0, 0, 0, ' '}                                   // terminator
    };

    bool IsPreBCAcceptedClientBuild(int build)
    {
        for (int i = 0; PreBcAcceptedClientBuilds[i].Build; ++i)
            if (PreBcAcceptedClientBuilds[i].Build == build)
                return true;

        return false;
    }

    bool IsPostBCAcceptedClientBuild(int build)
    {
        for (int i = 0; PostBcAcceptedClientBuilds[i].Build; ++i)
            if (PostBcAcceptedClientBuilds[i].Build == build)
                return true;

        return false;
    }

    bool IsAcceptedClientBuild(int build)
    {
        return (IsPostBCAcceptedClientBuild(build) || IsPreBCAcceptedClientBuild(build));
    }

    RealmBuildInfo const* GetBuildInfo(int build)
    {
        for (int i = 0; PostBcAcceptedClientBuilds[i].Build; ++i)
            if (PostBcAcceptedClientBuilds[i].Build == build)
                return &PostBcAcceptedClientBuilds[i];

        for (int i = 0; PreBcAcceptedClientBuilds[i].Build; ++i)
            if (PreBcAcceptedClientBuilds[i].Build == build)
                return &PreBcAcceptedClientBuilds[i];

        return NULL;
    }
};