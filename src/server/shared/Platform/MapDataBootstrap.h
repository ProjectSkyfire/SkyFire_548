/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#ifndef SKYFIRE_MAP_DATA_BOOTSTRAP_H
#define SKYFIRE_MAP_DATA_BOOTSTRAP_H
#include <cstdint>
#include <set>
#include <string>
bool PrepareMapData(std::set<std::uint32_t>& maps, std::string& root, std::string& error);
#endif
