/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "CertificateTools.h"
#include "Configuration/Config.h"
#include <filesystem>
namespace Skyfire::Certificates
{
    bool PeerAllowed(SSL* stream)
    {
        auto directory = std::filesystem::absolute(sConfigMgr->GetFilename()).parent_path();
        auto path = [&](char const* key) { auto value = sConfigMgr->GetStringDefault(key, ""); return value.empty() ? value : (directory / value).lexically_normal().string(); };
        return PeerAllowed(stream, path("Cluster.CA"), path("Cluster.CRL"));
    }
}
