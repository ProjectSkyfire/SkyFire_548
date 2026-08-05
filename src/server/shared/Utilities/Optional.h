/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef _OPTIONAL_H
#define _OPTIONAL_H

#include <optional>

//! Optional helper class to wrap optional values within.
template <class T>
using Optional = std::optional<T>;

#endif // _OPTIONAL_H
